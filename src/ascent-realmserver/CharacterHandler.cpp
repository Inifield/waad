/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentcommunity.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "RStdAfx.h"
#include "../ascent-shared/AuthCodes.h"

#define EQUIPMENT_SLOT_MAINHAND	    15 // Main droite
#define EQUIPMENT_SLOT_OFFHAND	    16 // Main Gauche
#define PLAYER_FLAG_NOHELM	 0x000400
#define PLAYER_FLAG_NOCLOAK	 0x000800

void Session::HandleCharacterEnum(WorldPacket & pck)
{
	struct player_item
	{
		uint32 displayid;
		uint8 invtype;
		uint32 enchantment; // added in 2.4
	};

	Log.Debug("Session","Chargement de la liste des personnages");

	player_item items[23];
	int8 slot;
	int8 containerslot;
	uint32 i;
	ItemPrototype * proto;
	// loading characters
	QueryResult* result = CharacterDatabase.Query("SELECT guid, level, race, class, gender, bytes, bytes2, name, positionX, positionY, positionZ, mapId, zoneId, banned, restState, deathstate, forced_rename_pending, player_flags, guild_data.guildid FROM characters LEFT JOIN guild_data ON characters.guid = guild_data.playerid WHERE acct=%u ORDER BY guid ASC LIMIT 10", GetAccountId());
	QueryResult * res;
	CreatureInfo *info = NULL;
	//uint8 num = 0;
	uint8 race;
	uint32 LevelMaxfound = 0;
	uint32 LevelFound = 1;
	uint32 start_time = getMSTime();
	uint64 DK_Guid = 0;
	uint64 Char_Guid = 0;
	uint8 _side = -1; // side should be set on every enumeration for safety

	uint32 numchar;

	if(result)
		numchar = result->GetRowCount();
	else
		numchar = 0;

	// should be more than enough.. 200 bytes per char..
	WorldPacket data((result ? result->GetRowCount() * 200 : 1));

	// parse m_characters and build a mighty packet of
	// characters to send to the client.
	data.SetOpcode(SMSG_CHAR_ENUM);
	data << uint8(numchar);
	if( result )
	{
		uint64 guid;
		uint8 Class;
		uint32 bytes2;
		uint32 flags;
		uint32 banned;
		Field *fields;
		do
		{
			fields = result->Fetch();
			guid = fields[0].GetUInt32();
			bytes2 = fields[6].GetUInt32();
			Class = fields[3].GetUInt8();
			flags = fields[17].GetUInt32();
			race = fields[2].GetUInt8();

			if( _side < 0 )
			{
				// work out the side
				static uint8 sides[11/*RACE_DRAENEI*/+1] = { 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0 };
				_side = sides[race];
			}

			/* build character enum, w0000t :p */
			Char_Guid = fields[0].GetUInt64();		// guid
			data << (uint64) Char_Guid;
			data << fields[7].GetString();		// name
			data << race;						// race
			data << Class;						// class
			data << fields[4].GetUInt8();		// gender
			data << fields[5].GetUInt32();		// PLAYER_BYTES
			data << uint8(bytes2 & 0xFF);		// facial hair
			LevelFound = fields[1].GetUInt8();		// Level
			data << uint8(LevelFound) ;
			data << fields[12].GetUInt32();		// zoneid
			data << fields[11].GetUInt32();		// Mapid
			data << fields[8].GetFloat();		// X
			data << fields[9].GetFloat();		// Y
			data << fields[10].GetFloat();		// Z
			data << fields[18].GetUInt32();		// GuildID

            banned = fields[13].GetUInt32();
			if(banned && (banned<10 || banned > (uint32)UNIXTIME))
				data << uint32(0x01A04040);
			else
			{
				if(fields[16].GetUInt32() != 0)
					data << uint32(0x00A04342);
				else if(fields[15].GetUInt32() != 0)
					data << (uint32)8704; // Dead (displaying as Ghost)
				else
					data << uint32(1);		// alive
			}

			data << uint32(0);					//Added in 3.0.2
			//data << fields[14].GetUInt8();		// Rest State -- obsolete 322
			data << uint8(0);					// Added in 3.2.0

			if(Class == 9 /*WARLOCK*/ || Class == 3 /*HUNTER*/) 
			{
				res = CharacterDatabase.Query("SELECT entry FROM playerpets WHERE ownerguid="I64FMTD" AND ( active MOD 10 ) =1", guid);

				if(res)
				{
					info = CreatureNameStorage.LookupEntry(res->Fetch()[0].GetUInt32());
					delete res;
				}
				else
					info=NULL;
			}
			else
				info=NULL;

			if(info)  //PET INFO uint32 displayid,	uint32 level,		 uint32 familyid
			{
				data << uint32(info->Male_DisplayID) << uint32(10) << uint32(info->Family);
			}
			else
				data << uint32(0) << uint32(0) << uint32(0);

			res = CharacterDatabase.Query("SELECT containerslot, slot, entry, enchantments FROM playeritems WHERE ownerguid=%u", GUID_LOPART(guid));

            // Pre-requis (Branruz)
			if(LevelFound > LevelMaxfound) LevelMaxfound = LevelFound;
			if(Class== 6 /*DEATHKNIGHT*/) DK_Guid = Char_Guid;
			//Log.Notice("[CharacterEnumProc]","LevelMaxfound %u , LevelFound %u",(uint32)LevelMaxfound ,(uint32)LevelFound);
			//Log.Notice("[CharacterEnumProc]","Guid Personnage %u",Char_Guid);

			memset(items, 0, sizeof(player_item) * 23);
			uint32 enchantid;
			EnchantEntry * enc;
			if(res)			
			{
				do
				{
					containerslot = res->Fetch()[0].GetInt8();
					slot = res->Fetch()[1].GetInt8();
					if( containerslot == -1 && slot < 19 && slot >= 0 )
					{
						proto = ItemPrototypeStorage.LookupEntry(res->Fetch()[2].GetUInt32());
						if(proto)
						{
							// slot0 = head, slot14 = cloak
							
							if(!(slot == 0 && (flags & (uint32)PLAYER_FLAG_NOHELM) != 0) && !(slot == 14 && (flags & (uint32)PLAYER_FLAG_NOCLOAK) != 0)) {
								items[slot].displayid = proto->DisplayInfoID;
								items[slot].invtype = proto->InventoryType;
								// weapon glows
								if( slot == EQUIPMENT_SLOT_MAINHAND || slot == EQUIPMENT_SLOT_OFFHAND )
								{
									// get enchant visual ID
									const char * enchant_field = res->Fetch()[3].GetString();
									if( sscanf( enchant_field , "%u,0,0;" , (unsigned int *)&enchantid ) == 1 && enchantid > 0 )
									{
										enc = dbcEnchant.LookupEntry( enchantid );
										if( enc != NULL )
											items[slot].enchantment = enc->visual;
										else
											items[slot].enchantment = 0;
									}
								}
							}
						}
					}
				} while(res->NextRow());
				delete res;
			}

			for( i = 0; i < 23; ++i )
			{
				data << items[i].displayid << items[i].invtype << uint32(items[i].enchantment);
			}

		  
			//num++;
		}
		while( result->NextRow() );
	}

	Log.Debug("Character Enum", "Built in %u ms.", getMSTime() - start_time);
	SendPacket( &data );

	// Update Pre-Requis (Branruz)
	//Log.Notice("[CharacterEnumProc]","acct_id: %u,AlreadyDK: %u, MaxLevelGet: %u ",this->GetAccountId(),(uint32)DK_Guid, (uint32)LevelMaxfound);
	CharacterDatabase.Query("REPLACE INTO `_blizzrequirements` (`acct_id`,`AlreadyDK`,`MaxLevelGet`) VALUES (%u,%u,%u)",this->GetAccountId(),(uint32)DK_Guid,(uint32)LevelMaxfound);
}

void Session::HandlePlayerLogin(WorldPacket & pck)
{
	WorldPacket data(SMSG_CHARACTER_LOGIN_FAILED, 30);
	LocationVector LoginCoord;
	Instance * dest;
	//ASSERT(!m_currentPlayer);
	if(!m_currentPlayer)
		return;
	uint64 guid;
	pck >> guid;

	if(sClientMgr.GetRPlayer((uint32)guid) != NULL)
	{
		data << uint8(CHAR_LOGIN_DUPLICATE_CHARACTER);
		SendPacket(&data);
		return;
	}

	m_currentPlayer = sClientMgr.CreateRPlayer((uint32)guid);
	RPlayerInfo * p = m_currentPlayer;

	/* Load player data */
	QueryResult * result = CharacterDatabase.Query("SELECT acct, name, level, guild_data.guildid, positionX, positionY, zoneId, mapId, race, class, gender, instance_id, entrypointmap, entrypointx, entrypointy, entrypointz, entrypointo, difficulty FROM characters LEFT JOIN guild_data ON characters.guid = guild_data.playerid WHERE guid = %u", guid);
	if(result)
	{
		Field * f = result->Fetch();
		p->AccountId = f[0].GetUInt32();
		p->Name = f[1].GetString();
		p->Level = f[2].GetUInt32();
		p->GuildId = f[3].GetUInt32();
		p->PositionX = f[4].GetFloat();
		p->PositionY = f[5].GetFloat();
		p->ZoneId = f[6].GetUInt32();
		p->MapId = f[7].GetUInt32();
		p->Race = f[8].GetUInt8();
		p->Class = f[9].GetUInt8();
		p->Gender = f[10].GetUInt8();
		p->Latency = m_latency;
		p->GMPermissions = m_GMPermissions;
		p->Account_Flags = m_accountFlags;
		p->InstanceId = f[11].GetUInt32();
		p->RecoveryMapId = f[12].GetUInt32();
		p->RecoveryPosition.ChangeCoords(f[13].GetFloat(), f[14].GetFloat(), f[15].GetFloat(), f[16].GetFloat());
		p->iInstanceType = f[17].GetUInt32();
		p->ClientBuild = m_ClientBuild;
		p->session = this;

		uint8 team = 0;
		// work out the side
		static uint8 teams[12] = { 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0 };
		team = teams[p->Race];

		p->Team = team;

		delete result;
	}
	else
	{
		data << uint8(CHAR_LOGIN_NO_CHARACTER);
		SendPacket(&data);
		sClientMgr.DestroyRPlayerInfo((uint32)guid);
		m_currentPlayer = NULL;
		return;
	}

	if(IS_MAIN_MAP(m_currentPlayer->MapId))
	{
		/* we're on a continent, try to find the world server we're going to */
		dest = sClusterMgr.GetInstanceByMapId(m_currentPlayer->MapId);		
	}
	else
	{
		/* we're in an instanced map, try to find the world server we're going to */
		dest = sClusterMgr.GetInstanceByInstanceId(m_currentPlayer->InstanceId);

		if(dest && dest->MapId != m_currentPlayer->MapId)
		{
			/* our instance has been deleted or no longer valid, world server probably crashed */
			MapInfo * info = WorldMapInfoStorage.LookupEntry(dest->MapId);
			if(info)
			{
				Log.Error("HandlePlayerLogin", "Instance has been deleted or no longer valid, attempting repop on map %u", info->repopmapid);
				m_currentPlayer->MapId = info->repopmapid;
				LoginCoord.x = info->repopx;
				LoginCoord.y = info->repopy;
				LoginCoord.z = info->repopz;
				LoginCoord.o = 0.0f;

				/* obtain instance */
				dest = sClusterMgr.GetInstanceByMapId(m_currentPlayer->MapId);
				if(dest)
				{
					data.SetOpcode(SMSG_NEW_WORLD);
					data << m_currentPlayer->MapId << LoginCoord << float(0);
					SendPacket(&data);
					data.clear();
				}
				else
				{
					dest = NULL;
					Log.Error("HandlePlayerLogin", "Repop failed, could not find instance, for map %u", m_currentPlayer->MapId);
				}

			}
			else
			{
				dest = NULL;
				Log.Error("HandlePlayerLogin", "Repop failed, no map info, for map %u", m_currentPlayer->MapId);
			}
		}

		if(!dest)
		{
			Log.Error("HandlePlayerLogin", "Could not find instance, attempting recovery");
			/* our instance has been deleted or no longer valid */
			m_currentPlayer->MapId = m_currentPlayer->RecoveryMapId;
			LoginCoord = m_currentPlayer->RecoveryPosition;

			/* obtain instance */
			dest = sClusterMgr.GetInstanceByMapId(m_currentPlayer->MapId);
			if(dest)
			{
				data.SetOpcode(SMSG_NEW_WORLD);
                data << m_currentPlayer->MapId << m_currentPlayer->RecoveryPosition << float(0);
				SendPacket(&data);
				data.clear();
			}
		}
	}

	if(!dest || !dest->Server)		// Shouldn't happen
	{
		Log.Error("CharacterHandler", "World server is down!");
		/* world server is down */
		data << uint8(CHAR_LOGIN_NO_WORLD);
		SendPacket(&data);
		sClientMgr.DestroyRPlayerInfo((uint32)guid);
		m_currentPlayer = NULL;
		return;
	}

	/* log the player into that WS */
	data.SetOpcode(ISMSG_PLAYER_LOGIN);

	/* append info */
	data << uint32(guid) << uint32(dest->MapId) << uint32(dest->InstanceId);

	/* append the account information */
	data << uint32(m_accountId) << uint32(m_accountFlags) << uint32(m_sessionId)
		<< m_GMPermissions << m_accountName << m_ClientBuild;

	dest->Server->SendPacket(&data);
	m_nextServer = dest->Server;
}

void Session::HandleRealmSplitQuery(WorldPacket & pck)
{
	uint32 v;
	pck >> v;

	WorldPacket data(SMSG_REALM_SPLIT, 17);
	data << v << uint32(0);
	data << "01/01/01";
	SendPacket(&data);
}

void Session::HandleCharacterCreate( WorldPacket & pck )
{
	CHECK_PACKET_SIZE(pck, 10);	
	std::string name;
	uint8 race, class_;

	pck >> name >> race >> class_;
	pck.rpos(0);

	Log.Debug("CMSG_CHAR_CREATE: name %s, race %u, class_ %u",name.c_str(),race,class_);

	if(!VerifyName(name.c_str(), name.length(), true))
	{
		GetSocket()->OutPacket(SMSG_CHAR_CREATE, 1, "\x32");
		sLog.outDetail("CMSG_CHAR_CREATE: CHAR_CREATE_IN_USE");
		return;
	}

	if(sClientMgr.GetRPlayer(name.c_str()) != NULL)
	{
		OutPacket(SMSG_CHAR_CREATE, 1, "\x32"); // CHAR_CREATE_IN_USE
		sLog.outDetail("CMSG_CHAR_CREATE: CHAR_CREATE_IN_USE");
		return;
	}

	QueryResult * result = Database_Character->Query("SELECT COUNT(*) FROM banned_names WHERE name = '%s'", CharacterDatabase.EscapeString(name).c_str());
	if(result)
	{
		if(result->Fetch()[0].GetUInt32() > 0)
		{
			// That name is banned!
			GetSocket()->OutPacket(SMSG_CHAR_CREATE, 1, "\x33"); // You cannot use that name
			sLog.outDetail("CMSG_CHAR_CREATE: CHAR_CREATE_DISABLED");
			delete result;
			return;
		}
		delete result;
	}

	// check that the account CAN create TBC characters, if we're making some
	if((race == 10 /*RACE_BLOODELF*/) && (!this->HasFlag(ACCOUNT_FLAG_XPACK_01)) )
	{
		OutPacket(SMSG_CHAR_CREATE, 1, "\x38"); // CHAR_CREATE_ONLY_EXISTING
		sLog.outDetail("CMSG_CHAR_CREATE: CHAR_CREATE_ONLY_EXISTING 1");
		return;
	}

	if( class_ == 6 /*DEATHKNIGHT*/ )
	{
		if(!this->HasFlag(ACCOUNT_FLAG_XPACK_02)) 
		{                               
		 OutPacket(SMSG_CHAR_CREATE, 1, "\x38"); // CHAR_CREATE_ONLY_EXISTING
		 sLog.outDetail("CMSG_CHAR_CREATE: CHAR_CREATE_ONLY_EXISTING 2");
		 return;
		}

	   QueryResult * result;
	   result = CharacterDatabase.Query("SELECT * FROM `_blizzrequirements` where acct_id = %u", this->GetAccountId());
	   if(!result) // Ne peut pas arriver, la table se met a jour au chargement de la liste des persos (Branruz)
	   {
		   Log.Error("[Creation d'un DK]","AccountId player (%u) non trouvé dans la table BlizzRequirements <- Report this to Devs.",this->GetAccountId());
		   OutPacket(SMSG_CHAR_CREATE, 1, "\x30"); // CHAR_CREATE_ERROR
	       return;
	   }
	   else
	   {
		   Field * fields = result->Fetch();
		   uint32 AcctId            = fields[0].GetUInt32();
		   uint32 DK_Guid           = fields[1].GetUInt32();
		   uint32 MaxLevelPlayerGet = fields[2].GetUInt32();

		   if( (DK_Guid != 0)) 
		   {
			Log.Notice("[Blizz Prerequis]","%s veut creer un DK mais il en possede deja un!",this->GetAccountName().c_str());
		    OutPacket(SMSG_CHAR_CREATE, 1, "\x3C"); // CHAR_CREATE_ERROR_HERO_CLASS_LIMIT
	        return;
		   }

		   if(MaxLevelPlayerGet < 55) 
		   {
		    Log.Notice("[Blizz Prerequis]","%s veut creer un DK mais il n'a aucun personnage de level > a 55!",this->GetAccountName().c_str());
		    OutPacket(SMSG_CHAR_CREATE, 1, "\x3B"); // CHAR_CREATE_ERROR_NEED_LVL_55_CHAR
	        return;
		   }

	   }
	}

	Instance* i = sClusterMgr.GetAnyInstance();

	if (i == NULL)
	{
		Log.Warning("CHAR_CREATE","Il n'y a pas de World en ligne pour realiser l'operation");
		GetSocket()->OutPacket(SMSG_CHAR_CREATE, 1, "\x32");
		return;
	}
	
	if (GetAccountId())
	{
		/* log the player into that WS */
		WorldPacket data(ISMSG_CREATE_PLAYER, 4+6+pck.size());
	
		/* append Packet SMSG_CREATE_PLAYER information */
		data << GetAccountId() << pck.GetOpcode() << uint32(pck.size());
		data.resize(10 + pck.size());
		memcpy((void*)(data.contents() + 10), pck.contents(), pck.size());

		i->Server->SendPacket(&data);
		
		sLogonCommHandler.UpdateAccountCount(GetAccountId(), 1);
	}
	else
	{
		// Normalement impossible, sauf si le Realmserver à planté. Dans ce cas, on interdit la création du personnage.
		Log.Warning("CHAR_CREATE","Il n'existe pas de session avec cet id de compte: %u",GetAccountId());
		GetSocket()->OutPacket(SMSG_CHAR_CREATE, 1, "\x32");
		return;
	}
}

void Session::HandleCharacterDelete(WorldPacket & pck)
{
	CHECK_PACKET_SIZE(pck, 8);
	WorldPacket data;
	
	std::string name;	

	uint64 guid;
	pck >> guid;
	
	m_currentPlayer = sClientMgr.CreateRPlayer((uint32)guid);
	RPlayerInfo * info = m_currentPlayer;
	Session * s = sClientMgr.GetSessionByRPInfo(info);
	
	Log.Debug("CHAR_DELETE","L'OPCODE est %u",pck.GetOpcode());
	Log.Debug("CHAR_DELETE","Le guid est %u",info->Guid);
	Log.Debug("CHAR_DELETE","Son AccountID est %u",info->AccountId);
	Log.Debug("CHAR_DELETE","L'info de Session est %u",GetAccountId());
	Log.Debug("CHAR_DELETE","Ses references sont %u",info->references);	

	if( info->Guid != NULL && s == NULL)
	{
		QueryResult * result = CharacterDatabase.Query("SELECT name FROM characters WHERE guid = %u AND acct = %u", (uint32)guid, GetAccountId());
		if(!result)
		{
			Log.Warning("CHAR_DELETE","Ne trouve pas le personnage par la recherche du Guid %u et du Acct %u",(uint32)guid, GetAccountId());
			OutPacket(SMSG_CHAR_DELETE, 1, "\x48"); //CHAR_DELETE_FAILED
			return;
		}

		if(info->GuildId)
		{
			QueryResult * resultGuild = CharacterDatabase.Query("SELECT leaderGuid FROM guilds WHERE guid = %u", (uint32)guid);
			if(resultGuild)
			{
				Log.Warning("CHAR_DELETE","Le personnage est le chef de la Guilde : %u",info->GuildId);
				OutPacket(SMSG_CHAR_DELETE, 1, "\x4A"); //CHAR_DELETE_FAILED_GUILD_LEADER
				delete resultGuild;
				return;
			}
		}

		if(info->Team)
		{
			QueryResult * resultTeam = CharacterDatabase.Query("SELECT leader FROM arenateams WHERE id = %u", (uint32)guid);
			if(resultTeam)
			{
				Log.Warning("CHAR_DELETE","Le personnage est le leader de la Team : %u",info->Team);
				OutPacket(SMSG_CHAR_DELETE, 1, "\x4B"); //CHAR_DELETE_FAILED_GUILD_LEADER
				delete resultTeam;
				return;
			}
		}

		delete result;

		Instance* i = sClusterMgr.GetAnyInstance();

		if (i == NULL)
		{
			Log.Warning("CHAR_DELETE","Il n' y a pas de World en ligne pour realiser l'operation");
			OutPacket(SMSG_CHAR_DELETE, 1, "\x48"); //CHAR_DELETE_FAILED
			return;
		}
		else
		{
			/* log the player into that WS */
			data.SetOpcode(ISMSG_DELETE_PLAYER);
			data << guid;
			i->Server->SendPacket(&data);
		}
		
		CharacterDatabase.WaitExecute("DELETE FROM characters WHERE guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM achievements WHERE player = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM auctions WHERE owner = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM charters WHERE leaderGuid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM gm_tickets WHERE guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM guild_data WHERE playerid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM instances WHERE creator_guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM mailbox WHERE player_guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playercooldowns WHERE player_guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playerglyphs WHERE guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playeritems WHERE ownerguid=%u",(uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playerpets WHERE ownerguid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playerpetspells WHERE ownerguid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playerskills WHERE player_guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playerspells WHERE guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playersummonspells WHERE ownerguid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playertalents WHERE guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM questlog WHERE player_guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM social_friends WHERE character_guid = %u OR friend_guid = %u", (uint32)guid, (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM social_ignores WHERE character_guid = %u OR ignore_guid = %u", (uint32)guid, (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM tutorials WHERE playerId = %u", (uint32)guid);

		/* remove player info */
		sClientMgr.DestroyRPlayerInfo((uint32)guid);
		m_currentPlayer = NULL;

		OutPacket(SMSG_CHAR_DELETE, 1, "\x47"); //CHAR_DELETE_SUCCESS
		return;
	}
	else
	{
		if(info->Guid == NULL)
		{
			Log.Warning("CHAR_DELETE","Le personnage n'existe pas ou a deja ete supprime");
		}
		else
		{
			Log.Warning("CHAR_DELETE","Le personnage existe dans le World");
		}
		OutPacket(SMSG_CHAR_DELETE, 1, "\x48"); //CHAR_DELETE_FAILED
	}
}