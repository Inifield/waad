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

#include "StdAfx.h"
#include "AuthCodes.h"
#include "svn_revision.h"

bool ChatHandler::HandleRenameAllCharacter(const char * args, WorldSession * m_session)
{
	uint32 uCount = 0;
	uint32 ts = getMSTime();
	QueryResult * result = CharacterDatabase.Query("SELECT guid, name FROM characters");
	if( result )
	{
		do
		{
			uint32 uGuid = result->Fetch()[0].GetUInt32();
			const char * pName = result->Fetch()[1].GetString();
			size_t szLen = strlen(pName);

			if( !VerifyName(pName, szLen) )
			{
				printf("changement de nom du personnage %s, %u\n", pName,uGuid);
                Player * pPlayer = objmgr.GetPlayer(uGuid);
				if( pPlayer != NULL )
				{
					pPlayer->rename_pending = true;
					pPlayer->GetSession()->SystemMessage("Votre personnage a eu une demande de changement de nom, vous serez incité à rebaptiser votre personnage à l'établissement de la connexion suivante dans la conformité avec les règles du serveur.");
				}

				CharacterDatabase.WaitExecute("UPDATE characters SET forced_rename_pending = 1 WHERE guid = %u", uGuid);
				++uCount;
			}

		} while (result->NextRow());
		delete result;
	}

	SystemMessage(m_session, "Procédure complétée en %u ms. %u personnage(s) obligatoiré de se rebaptiser.", getMSTime() - ts, uCount);
	return true;
}

void CapitalizeString(string& arg)
{
	if(arg.length() == 0) return;
	arg[0] = toupper(arg[0]);
	for(uint32 x = 1; x < arg.size(); ++x)
		arg[x] = tolower(arg[x]);
}

void WorldSession::CharacterEnumProc(QueryResult * result)
{
	struct player_item
	{
		uint32 displayid;
		uint8 invtype;
		uint32 enchantment; // added in 2.4
	};

	player_item items[23];
	int8 slot;
	int8 containerslot;
	uint32 i;
	ItemPrototype * proto;
	QueryResult * res;
	CreatureInfo *info = NULL;
	//uint8 num = 0;
	uint8 race;
	uint32 LevelMaxfound = 0;
	uint32 LevelFound = 1; 
	uint64 DK_Guid = 0;
	uint64 Char_Guid = 0;
	_side = -1; // side should be set on every enumeration for safety

	uint32 numchar;

	if(result)
		numchar = result->GetRowCount();
	else
		numchar = 0;

	// should be more than enough.. 200 bytes per char..
	WorldPacket data(SMSG_CHAR_ENUM, (result ? result->GetRowCount() * 200 : 1));

	// parse m_characters and build a mighty packet of
	// characters to send to the client.
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
				static uint8 sides[RACE_DRAENEI+1] = { 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0 };
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

			if((Class==WARLOCK) || (Class == HUNTER) || ((Class == WARRIOR) && sWorld.m_WarriorPetAllowed) )
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
			if(Class==DEATHKNIGHT) DK_Guid = Char_Guid;
			//Log.Notice("[CharacterEnumProc]","LevelMaxfound %u , LevelFound %u",(uint32)LevelMaxfound ,(uint32)LevelFound);
			//Log.Notice("[CharacterEnumProc]","Guid Personnage %u",Char_Guid);

			memset(items, 0, sizeof(player_item) * 23);
			uint32 enchantid;
			EnchantEntry * enc;
			memset(items, 0, sizeof(player_item) * 23);
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

	//data.put<uint8>(0, num);

	//Log.Debug("Character Enum", "Built in %u ms.", getMSTime() - start_time);
	SendPacket( &data );

	// Update Pre-Requis (Branruz)
	//Log.Notice("[CharacterEnumProc]","acct_id: %u,AlreadyDK: %u, MaxLevelGet: %u ",this->GetAccountId(),(uint32)DK_Guid, (uint32)LevelMaxfound);
	CharacterDatabase.Query("REPLACE INTO `_blizzrequirements` (`acct_id`,`AlreadyDK`,`MaxLevelGet`) VALUES (%u,%u,%u)",this->GetAccountId(),(uint32)DK_Guid,(uint32)LevelMaxfound);
}

void WorldSession::HandleCharEnumOpcode( WorldPacket & recv_data )
{	
	AsyncQuery * q = new AsyncQuery( new SQLClassCallbackP1<World, uint32>(World::getSingletonPtr(), &World::CharacterEnumProc, GetAccountId()) );
	q->AddQuery("SELECT guid, level, race, class, gender, bytes, bytes2, name, positionX, positionY, positionZ, mapId, zoneId, banned, restState, deathstate, forced_rename_pending, player_flags, guild_data.guildid FROM characters LEFT JOIN guild_data ON characters.guid = guild_data.playerid WHERE acct=%u ORDER BY guid ASC LIMIT 10", GetAccountId());
	CharacterDatabase.QueueAsyncQuery(q);
}

void WorldSession::LoadAccountDataProc(QueryResult * result)
{
	size_t len;
	const char * data;
	char * d;

	if(!result)
	{
		CharacterDatabase.Execute("INSERT INTO account_data VALUES(%u, '', '', '', '', '', '', '', '', '')", _accountId);
		return;
	}

	for(uint32 i = 0; i < 7; ++i)
	{
		data = result->Fetch()[1+i].GetString();
		len = data ? strlen(data) : 0;
		if(len > 1)
		{
			d = new char[len+1];
			memcpy(d, data, len+1);
			SetAccountData(i, d, true, (uint32)len);
		}
	}
}

void WorldSession::HandleCharCreateOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 10);
	std::string name;
	uint8 race, class_;

	recv_data >> name >> race >> class_;
	recv_data.rpos(0);

	sLog.outDetail("CMSG_CHAR_CREATE: name %s, race %u, class_ %u",name.c_str(),race,class_);

	if(!VerifyName(name.c_str(), name.length(), sWorld.m_limitedNames))
	{
		OutPacket(SMSG_CHAR_CREATE, 1,"\x55"); // CHAR_NAME_RESERVED
		sLog.outDetail("CMSG_CHAR_CREATE: CHAR_NAME_RESERVED");
		return;
	}

	if(g_characterNameFilter->Parse(name, false))
	{
		OutPacket(SMSG_CHAR_CREATE, 1,"\x52"); // CHAR_NAME_INVALID_CHARACTER
		sLog.outDetail("CMSG_CHAR_CREATE: CHAR_NAME_INVALID_CHARACTER");
		return;
	}

	if(objmgr.GetPlayerInfoByName(name.c_str()) != 0)
	{
		OutPacket(SMSG_CHAR_CREATE, 1, "\x32"); // CHAR_CREATE_IN_USE
		sLog.outDetail("CMSG_CHAR_CREATE: CHAR_CREATE_IN_USE");
		return;
	}

	// check that the account CAN create TBC characters, if we're making some
	if((race == RACE_BLOODELF) && (!this->HasFlag(ACCOUNT_FLAG_XPACK_01)) )
	{
		OutPacket(SMSG_CHAR_CREATE, 1, "\x38"); // CHAR_CREATE_ONLY_EXISTING
		sLog.outDetail("CMSG_CHAR_CREATE: CHAR_CREATE_ONLY_EXISTING 1");
		return;
	}

	if( class_ == DEATHKNIGHT )
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

		   if( (DK_Guid != 0)  && sWorld.m_OneDeathKnight) 
		   {
			Log.Notice("[Blizz Prerequis]","%s veut creer un DK mais il en possede deja un!",this->GetAccountName().c_str());
		    OutPacket(SMSG_CHAR_CREATE, 1, "\x3C"); // CHAR_CREATE_ERROR_HERO_CLASS_LIMIT
	        return;
		   }

		   if(MaxLevelPlayerGet < sWorld.m_MinAlreadyLevel) 
		   {
		    Log.Notice("[Blizz Prerequis]","%s veut creer un DK mais il n'a aucun personnage de level > a %u!",this->GetAccountName().c_str(),sWorld.m_MinAlreadyLevel);
		    OutPacket(SMSG_CHAR_CREATE, 1, "\x3B"); // CHAR_CREATE_ERROR_NEED_LVL_55_CHAR
	        return;
		   }

	   }
	}

	//if(sHookInterface.OnNewCharacter(race, class_, this, name.c_str())) return; // A deliberer (Branruz)
	sHookInterface.OnNewCharacter(race, class_, this, name.c_str());

	QueryResult * result = CharacterDatabase.Query("SELECT COUNT(*) FROM banned_names WHERE name = '%s'", CharacterDatabase.EscapeString(name).c_str());
	if(result)
	{
		if(result->Fetch()[0].GetUInt32() > 0)
		{
			// That name is banned!
			OutPacket(SMSG_CHAR_CREATE, 1, "\x54"); // CHAR_NAME_PROFANE
			delete result;
			return;
		}
		delete result;
	}
	// loading characters

	//checking number of chars is useless since client will not allow to create more than 10 chars
	//as the 'create' button will not appear (unless we want to decrease maximum number of characters)

	Player * pNewChar = objmgr.CreatePlayer();
	pNewChar->SetSession(this);
	if(!pNewChar->Create( recv_data ))
	{
		// failed.
		pNewChar->ok_to_remove = true;
		delete pNewChar;
		return;
	}

	//Same Faction limitation only applies to PVP and RPPVP realms :)
	uint32 realmType = sLogonCommHandler.GetRealmType();
	if(!HasGMPermissions() && (realmType == REALMTYPE_PVP || realmType == REALMTYPE_RPPVP) && _side >= 0)
	{
		if( ((pNewChar->GetTeam()== 0) && (_side == 1)) || ((pNewChar->GetTeam()== 1) && (_side == 0)) )
		{
			pNewChar->ok_to_remove = true;
			delete pNewChar;
			WorldPacket data(1);
			data.SetOpcode(SMSG_CHAR_CREATE);
			data << (uint8)ALL_CHARS_ON_PVP_REALM_MUST_AT_SAME_SIDE+1;
			SendPacket( &data );
			return;
		}
	}
	pNewChar->UnSetBanned();
	pNewChar->addSpell(22027);	  // Remove Insignia

	if(pNewChar->getClass() == WARLOCK)
	{
		pNewChar->AddSummonSpell(416, 3110);		// imp fireball
		pNewChar->AddSummonSpell(417, 19505);
		pNewChar->AddSummonSpell(1860, 3716);
		pNewChar->AddSummonSpell(1863, 7814);
	}
	else if(pNewChar->getClass() == DEATHKNIGHT)
	{   // En test  (Branruz - Randdrick)
		pNewChar->AddSummonSpell(42541,42541);		// rune de Sang 
		pNewChar->AddSummonSpell(42991,42991);		// rune de Sang 
		pNewChar->AddSummonSpell(42996,42996);		// rune de Givre 
		pNewChar->AddSummonSpell(42997,42997);		// rune de Givre 
		pNewChar->AddSummonSpell(42998,42998);		// rune Impie 
		pNewChar->AddSummonSpell(43000,43000);		// rune Impie 

	}

	pNewChar->SaveToDB(true);	

	PlayerInfo *pn=new PlayerInfo ;
	memset(pn, 0, sizeof(PlayerInfo));
	pn->guid = pNewChar->GetLowGUID();
	pn->name = strdup(pNewChar->GetName());
	pn->cl = pNewChar->getClass();
	pn->race = pNewChar->getRace();
	pn->gender = pNewChar->getGender();
	pn->m_Group=0;
	pn->subGroup=0;
	pn->m_loggedInPlayer=NULL;
	pn->team = pNewChar->GetTeam ();
	pn->guild=NULL;
	pn->guildRank=NULL;
	pn->guildMember=NULL;
#ifdef VOICE_CHAT
	pn->groupVoiceId = -1;
#endif
	objmgr.AddPlayerInfo(pn);

	pNewChar->ok_to_remove = true;
	delete  pNewChar;

	// CHAR_CREATE_SUCCESS
	OutPacket(SMSG_CHAR_CREATE, 1, "\x2F");

	sLogonCommHandler.UpdateAccountCount(GetAccountId(), 1);
}

/* Last Checked at patch 3.0.2 Specfic SMSG_CHAR_CREATE Error Codes:

    RESPONSE_SUCCESS = 0,
    RESPONSE_FAILURE = 1,
    RESPONSE_CANCELLED = 2,
    RESPONSE_DISCONNECTED = 3,
    RESPONSE_FAILED_TO_CONNECT = 4,
    RESPONSE_VERSION_MISMATCH = 5,
    CSTATUS_CONNECTING = 6,
    CSTATUS_NEGOTIATING_SECURITY = 7,
    CSTATUS_NEGOTIATION_COMPLETE = 8,
    CSTATUS_NEGOTIATION_FAILED = 9,
    CSTATUS_AUTHENTICATING = 10,
    AUTH_OK = 11,
    AUTH_FAILED = 12,
    AUTH_REJECT = 13,
    AUTH_BAD_SERVER_PROOF = 14,
    AUTH_UNAVAILABLE = 15,
    AUTH_SYSTEM_ERROR = 16,
    AUTH_BILLING_ERROR = 17,
    AUTH_BILLING_EXPIRED = 18,
    AUTH_VERSION_MISMATCH = 19,
    AUTH_UNKNOWN_ACCOUNT = 20,
    AUTH_INCORRECT_PASSWORD = 21,
    AUTH_SESSION_EXPIRED = 22,
    AUTH_SERVER_SHUTTING_DOWN = 23,
    AUTH_ALREADY_LOGGING_IN = 24,
    AUTH_LOGIN_SERVER_NOT_FOUND = 25,
    AUTH_WAIT_QUEUE = 26,
    AUTH_BANNED = 27,
    AUTH_ALREADY_ONLINE = 28,
    AUTH_NO_TIME = 29,
    AUTH_DB_BUSY = 30,
    AUTH_SUSPENDED = 31,
    AUTH_PARENTAL_CONTROL = 32,
    AUTH_LOCKED_ENFORCED = 33,
    REALM_LIST_SUCCESS = 34,
    REALM_LIST_FAILED = 35,
    REALM_LIST_INVALID = 36,
    REALM_LIST_REALM_NOT_FOUND = 37,
    ACCOUNT_CREATE_IN_PROGRESS = 38,
    ACCOUNT_CREATE_SUCCESS = 39,
    ACCOUNT_CREATE_FAILED = 40,
    CHAR_LIST_RETRIEVED = 41,
    CHAR_LIST_FAILED = 42,
    CHAR_CREATE_IN_PROGRESS = 43,
    CHAR_CREATE_SUCCESS = 44,
    CHAR_CREATE_ERROR = 45,
    CHAR_CREATE_FAILED = 46,
    CHAR_CREATE_DISABLED = 47,
    CHAR_CREATE_PVP_TEAMS_VIOLATION = 48,
    CHAR_CREATE_SERVER_LIMIT = 49,
    CHAR_CREATE_ACCOUNT_LIMIT = 50,
    CHAR_CREATE_SERVER_QUEUE = 51,
    CHAR_CREATE_ONLY_EXISTING = 52,
    CHAR_CREATE_EXPANSION = 53,
    CHAR_CREATE_EXPANSION_CLASS = 54,
    CHAR_CREATE_LEVEL_REQUIREMENT = 55,
    CHAR_CREATE_UNIQUE_CLASS_LIMIT = 56,
    CHAR_DELETE_IN_PROGRESS = 57,
    CHAR_DELETE_SUCCESS = 58,0x3A
    CHAR_DELETE_FAILED = 59, 0x3B
    CHAR_DELETE_FAILED_LOCKED_FOR_TRANSFER = 60, 0x3C
    CHAR_DELETE_FAILED_GUILD_LEADER = 61, 0x3D
    CHAR_DELETE_FAILED_ARENA_CAPTAIN = 62, 0x3E
    CHAR_LOGIN_IN_PROGRESS = 63,
    CHAR_LOGIN_SUCCESS = 64,
    CHAR_LOGIN_NO_WORLD = 65,
    CHAR_LOGIN_DUPLICATE_CHARACTER = 66,
    CHAR_LOGIN_NO_INSTANCES = 67,
    CHAR_LOGIN_FAILED = 68,
    CHAR_LOGIN_DISABLED = 69,
    CHAR_LOGIN_NO_CHARACTER = 70,
    CHAR_LOGIN_LOCKED_FOR_TRANSFER = 71,
    CHAR_LOGIN_LOCKED_BY_BILLING = 72,
    CHAR_NAME_SUCCESS = 73,
    CHAR_NAME_FAILURE = 74,
    CHAR_NAME_NO_NAME = 75,
    CHAR_NAME_TOO_SHORT = 76,
    CHAR_NAME_TOO_LONG = 77,
    CHAR_NAME_INVALID_CHARACTER = 78,
    CHAR_NAME_MIXED_LANGUAGES = 79,
    CHAR_NAME_PROFANE = 80,
    CHAR_NAME_RESERVED = 81,
    CHAR_NAME_INVALID_APOSTROPHE = 82,
    CHAR_NAME_MULTIPLE_APOSTROPHES = 83,
    CHAR_NAME_THREE_CONSECUTIVE = 84,
    CHAR_NAME_INVALID_SPACE = 85,
    CHAR_NAME_CONSECUTIVE_SPACES = 86,
    CHAR_NAME_RUSSIAN_CONSECUTIVE_SILENT_CHARACTERS = 87,
    CHAR_NAME_RUSSIAN_SILENT_CHARACTER_AT_BEGINNING_OR_END = 88,
    CHAR_NAME_DECLENSION_DOESNT_MATCH_BASE_NAME = 89,
*/

void WorldSession::HandleCharDeleteOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 8);
	uint8 fail = CHAR_DELETE_SUCCESS;

	uint64 guid;
	recv_data >> guid;

	if(objmgr.GetPlayer((uint32)guid) != NULL)
	{
		// "Char deletion failed"
		fail = CHAR_DELETE_FAILED;
	}
	else
	{
		fail = DeleteCharacter((uint32)guid);
	}
	OutPacket(SMSG_CHAR_DELETE, 1, &fail);
}

uint8 WorldSession::DeleteCharacter(uint32 guid)
{
	PlayerInfo * inf = objmgr.GetPlayerInfo(guid);
	if( inf != NULL && inf->m_loggedInPlayer == NULL )
	{
		QueryResult * result = CharacterDatabase.Query("SELECT name FROM characters WHERE guid = %u AND acct = %u", (uint32)guid, _accountId);
		if(!result)
			return CHAR_DELETE_FAILED;

		if(inf->guild)
		{
			if(inf->guild->GetGuildLeader()==inf->guid)
				return CHAR_DELETE_FAILED_GUILD_LEADER;
			else
				inf->guild->RemoveGuildMember(inf,NULL);
		}

		string name = result->Fetch()[0].GetString();
		delete result;

		for(int i = 0; i < NUM_CHARTER_TYPES; ++i)
		{
			if( inf->charterId[i] != 0 )
			{
				Charter *pCharter = objmgr.GetCharter(inf->charterId[i], (CharterTypes)i);
				if( pCharter->LeaderGuid == inf->guid )
					pCharter->Destroy();
				else
					pCharter->RemoveSignature(inf->guid);
			}
		}

		for(int i = 0; i < NUM_ARENA_TEAM_TYPES; ++i)
		{
			if( inf->arenaTeam[i] != NULL )
			{
				if( inf->arenaTeam[i]->m_leader == guid )
					return CHAR_DELETE_FAILED_GUILD_LEADER;
				else
					inf->arenaTeam[i]->RemoveMember(inf);
			}
		}

		/*if( _socket != NULL )
			sPlrLog.write("Account: %s | IP: %s >> Deleted player %s", GetAccountName().c_str(), GetSocket()->GetRemoteIP().c_str(), name.c_str());*/

		sPlrLog.writefromsession(this, "deleted character %s (GUID: %u)", name.c_str(), (uint32)guid);

		CharacterDatabase.WaitExecute("DELETE FROM characters WHERE guid = %u", (uint32)guid);

		Corpse* c=objmgr.GetCorpseByOwner((uint32)guid);
		if(c)
			CharacterDatabase.Execute("DELETE FROM corpses WHERE guid = %u", c->GetLowGUID());

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
		objmgr.DeletePlayerInfo((uint32)guid);
		return CHAR_DELETE_SUCCESS;
	}

	return CHAR_DELETE_FAILED;
}

void WorldSession::HandleCharRenameOpcode(WorldPacket & recv_data)
{
	WorldPacket data(SMSG_CHAR_RENAME, recv_data.size() + 1);

	uint64 guid;
	string name;
	recv_data >> guid >> name;

	PlayerInfo * pi = objmgr.GetPlayerInfo((uint32)guid);
	if(pi == 0) return;

	QueryResult * result = CharacterDatabase.Query("SELECT forced_rename_pending FROM characters WHERE guid = %u AND acct = %u", 
		(uint32)guid, _accountId);
	if(result == 0)
	{
		delete result;
		return;
	}
	delete result;

	// Check name for rule violation.
	const char * szName=name.c_str();
	for(uint32 x=0;x<strlen(szName);x++)
	{
		if((int)szName[x]<65||((int)szName[x]>90&&(int)szName[x]<97)||(int)szName[x]>122)
		{
			data << uint8(0x32);
			data << guid << name;
			SendPacket(&data);
			return;
		}
	}

	QueryResult * result2 = CharacterDatabase.Query("SELECT COUNT(*) FROM banned_names WHERE name = '%s'", CharacterDatabase.EscapeString(name).c_str());
	if(result2)
	{
		if(result2->Fetch()[0].GetUInt32() > 0)
		{
			// That name is banned!
			data << uint8(0x31);
			data << guid << name;
			SendPacket(&data);
		}
		delete result2;
	}

	// Check if name is in use.
	if(objmgr.GetPlayerInfoByName(name.c_str()) != 0)
	{
		data << uint8(0x32);
		data << guid << name;
		SendPacket(&data);
		return;
	}

	// correct capitalization
	CapitalizeString(name);
	objmgr.RenamePlayerInfo(pi, pi->name, name.c_str());

	sPlrLog.writefromsession(this, "un renomage est en suspens. Personnage rebaptisé %s (GUID: %u) to %s.", pi->name, pi->guid, name.c_str());

	// If we're here, the name is okay.
	free(pi->name);
	pi->name = strdup(name.c_str());
	CharacterDatabase.WaitExecute("UPDATE characters SET name = '%s' WHERE guid = %u", name.c_str(), (uint32)guid);
	CharacterDatabase.WaitExecute("UPDATE characters SET forced_rename_pending = 0 WHERE guid = %u", (uint32)guid);
	
	data << uint8(0) << guid << name;
	SendPacket(&data);
}

bool WorldSession::AttemptPlayerLogin(uint32 guid, uint32 mapid, uint32 instance)
{
	sLog.outDebug("WORLD: Recvd Player Logon Message" );
 
	if(objmgr.GetPlayer(guid) != NULL || m_loggingInPlayer || _player)
	{
		// A character with that name already exists 0x3E
		uint8 respons = CHAR_LOGIN_DUPLICATE_CHARACTER;
		OutPacket(SMSG_CHARACTER_LOGIN_FAILED, 1, &respons);
		return false;
	}

	Player* plr = new Player(guid);
	ASSERT(plr);
	plr->SetSession(this);
	m_bIsWLevelSet = false;
	
	Log.Debug("WorldSession", "Async loading player %u", guid);
	m_loggingInPlayer = plr;
	plr->LoadFromDB(guid);
	return true;
}

void WorldSession::HandlePlayerLoginOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 playerGuid = 0;

	recv_data >> playerGuid; // this is the GUID selected by the player

	sLog.outDebug( "WORLD: Connection d'un personnage (%u)" ,(uint32)playerGuid);

	if(objmgr.GetPlayer((uint32)playerGuid) != NULL || m_loggingInPlayer || _player)
	{
		// A character with that name already exists 0x3E
		uint8 respons = CHAR_LOGIN_DUPLICATE_CHARACTER;
		OutPacket(SMSG_CHARACTER_LOGIN_FAILED, 1, &respons);
		return;
	}

	Player* plr = new Player((uint32)playerGuid);
	//ASSERT(plr);
	if(plr)
	{
	 plr->SetSession(this);
	 m_bIsWLevelSet = false;
	
	 Log.Debug("WorldSession", "Chargement Async du joueur %u", (uint32)playerGuid);
	 m_loggingInPlayer = plr;
	 plr->LoadFromDB((uint32)playerGuid);
	}
	else 
	{
		Log.Error("[WorldSession::HandlePlayerLoginOpcode]","Le personnage n'existe pas! <- Report this to devs.");
		uint8 respons = CHAR_LOGIN_NO_CHARACTER;
		OutPacket(SMSG_CHARACTER_LOGIN_FAILED, 1, &respons);
		return;
	}
}

void WorldSession::FullLogin(Player * plr)
{
	Log.Debug("WorldSession", "Chargement complet du joueur %u", plr->GetLowGUID());
	SetPlayer(plr);
	m_MoverWoWGuid.Init(plr->GetGUID());

	WorldPacket datab(MSG_SET_DUNGEON_DIFFICULTY, 20);
	datab << plr->iInstanceType;
	datab << uint32(0x01);
	datab << uint32(0x00);
	SendPacket(&datab);

	/* Il est de trop celui la (Branruz)
	WorldPacket datat(SMSG_MOTD, 50);
	datat << uint32(0x04);
	datat << sWorld.GetMotd();
	SendPacket(&datat);
	*/

	/* world preload */
	packetSMSG_LOGIN_VERIFY_WORLD vwpck;
	vwpck.MapId = plr->GetMapId();
	vwpck.O = plr->GetOrientation();
	vwpck.X = plr->GetPositionX();
	vwpck.Y = plr->GetPositionY();
	vwpck.Z = plr->GetPositionZ();
	OutPacket( SMSG_LOGIN_VERIFY_WORLD, sizeof(packetSMSG_LOGIN_VERIFY_WORLD), &vwpck );

	// send voicechat state - active/inactive
	/*
	{SERVER} Packet: (0x03C7) UNKNOWN PacketSize = 2
	|------------------------------------------------|----------------|
	|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
	|------------------------------------------------|----------------|
	|02 01							               |..              |
	-------------------------------------------------------------------
	*/

#ifdef VOICE_CHAT
	datab.Initialize(SMSG_FEATURE_SYSTEM_STATUS);
	datab << uint8(2) << uint8(sVoiceChatHandler.CanUseVoiceChat() ? 1 : 0);
	SendPacket(&datab);
#else
	datab.Initialize(SMSG_FEATURE_SYSTEM_STATUS);
	datab << uint8(2) << uint8(0);
#endif

	WorldPacket dataldm(SMSG_LEARNED_DANCE_MOVES, 4+4);

	dataldm << uint32( 0 );
	dataldm << uint32( 0 );

	SendPacket(&dataldm);
	
	plr->UpdateAttackSpeed();
	/*if(plr->getLevel()>70)
		plr->SetUInt32Value(UNIT_FIELD_LEVEL,70);*/

	// enable trigger cheat by default
	plr->triggerpass_cheat = HasGMPermissions();

	// Make sure our name exists (for premade system)
	PlayerInfo * info = objmgr.GetPlayerInfo(plr->GetLowGUID());
	if(info == 0)
	{
		info = new PlayerInfo;
		memset(info, 0, sizeof(PlayerInfo));
		info->cl = plr->getClass();
		info->gender = plr->getGender();
		info->guid = plr->GetLowGUID();
		info->name = strdup(plr->GetName());
		info->lastLevel = plr->getLevel();
		info->lastOnline = UNIXTIME;
		info->lastZone = plr->GetZoneId();
		info->race = plr->getRace();
		info->team = plr->GetTeam();
		info->guild=NULL;
		info->guildRank=NULL;
		info->guildMember=NULL;
		info->m_Group=0;
		info->subGroup=0;
		objmgr.AddPlayerInfo(info);
	}
	plr->m_playerInfo = info;
	if(plr->m_playerInfo->guild)
	{
		plr->m_uint32Values[PLAYER_GUILDID] = plr->m_playerInfo->guild->GetGuildId();
		plr->m_uint32Values[PLAYER_GUILDRANK] = plr->m_playerInfo->guildRank->iId;
	}

	for(uint32 z = 0; z < NUM_ARENA_TEAM_TYPES; ++z)
	{
		if(_player->m_playerInfo->arenaTeam[z] != NULL)
		{
			_player->SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (z*7), _player->m_playerInfo->arenaTeam[z]->m_id);
			if(_player->m_playerInfo->arenaTeam[z]->m_leader == _player->GetLowGUID())
				_player->SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (z*7) + 1, 0);
			else
				_player->SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (z*7) + 1, 1);
		}
	}

	info->m_loggedInPlayer = plr;

	// account data == UI config
	SendAccountDataTimes(PER_CHARACTER_CACHE_MASK);

	 _player->ResetTitansGrip();

	// Set TIME OF LOGIN
	CharacterDatabase.Execute (
		"UPDATE characters SET online = 1 WHERE guid = %u" , plr->GetLowGUID());

	bool enter_world = true;

	// Find our transporter and add us if we're on one.
	//-- Pourquoi ce code etait en commentaire (Note: Branruz) 
	if(plr->m_TransporterGUID != 0)
	{
		Transporter * pTrans = objmgr.GetTransporter(GUID_LOPART(plr->m_TransporterGUID));
		if(pTrans)
		{
			if(plr->isDead())
			{
				plr->ResurrectPlayer(NULL);
				plr->SetUInt32Value(UNIT_FIELD_HEALTH, plr->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				plr->SetUInt32Value(UNIT_FIELD_POWER1, plr->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
			}

			float c_tposx = pTrans->GetPositionX() + plr->m_TransporterX;
			float c_tposy = pTrans->GetPositionY() + plr->m_TransporterY;
			float c_tposz = pTrans->GetPositionZ() + plr->m_TransporterZ;
			if(plr->GetMapId() != pTrans->GetMapId())	   // loaded wrong map
			{
				plr->SetMapId(pTrans->GetMapId());

				WorldPacket dataw(SMSG_NEW_WORLD, 20);
				dataw << pTrans->GetMapId() << c_tposx << c_tposy << c_tposz << plr->GetOrientation();
				SendPacket(&dataw);

				// shit is sent in worldport ack.
				enter_world = false;
			}

			plr->SetPosition(c_tposx, c_tposy, c_tposz, plr->GetOrientation(), false);
			plr->m_CurrentTransporter = pTrans;
			pTrans->AddPlayer(plr);
		}
	}
	
	if(plr->GetTeam() == FACTION_HORDE)
		sWorld.HordePlayers++;
	else
		sWorld.AlliancePlayers++;

	if(plr->m_FirstLogin && !HasGMPermissions())
	{
		uint32 racecinematic = plr->myRace->CinematicId;
#ifdef USING_BIG_ENDIAN
		swap32(&racecinematic);
#endif
		OutPacket(SMSG_TRIGGER_CINEMATIC, 4, &racecinematic);
	}
	Log.Notice("WORLD","%s vient de se connecter.",plr->GetName() );
	// Login time, will be used for played time calc
	plr->m_playedtime[2] = (uint32)UNIXTIME;

	//Issue a message telling all guild members that this player has signed on
	if(plr->IsInGuild())
	{
		Guild *pGuild = plr->m_playerInfo->guild;
		if(pGuild)
		{
			WorldPacket data(50);
			data.Initialize(SMSG_GUILD_EVENT);
			data << uint8(GUILD_EVENT_MOTD);
			data << uint8(0x01);
			if(pGuild->GetMOTD())
				data << pGuild->GetMOTD();
			else
				data << uint8(0);
			SendPacket(&data);

			pGuild->LogGuildEvent(GUILD_EVENT_HASCOMEONLINE, 1, plr->GetName());
		}
	}

	// Send online status to people having this char in friendlist
	_player->Social_TellFriendsOnline();
	// send friend list (for ignores)
	_player->Social_SendFriendList(7);

	// Send revision (if enabled)
#ifdef WIN32
	_player->BroadcastMessage("Server: %sAscentWaad %s r%u/%s-Win-%s",MSG_COLOR_WHITE, BUILD_TAG,BUILD_REVISION, CONFIG, ARCH); 
	_player->BroadcastMessage("Server: %s(arbonne.games-rpg.net/www.ascentcommunity.com)",MSG_COLOR_LIGHTBLUE);		
#else
	_player->BroadcastMessage("Server: %sAscentWaad %s r%u/%s-%s",MSG_COLOR_WHITE, BUILD_TAG,BUILD_REVISION, PLATFORM_TEXT, ARCH);
	_player->BroadcastMessage("Server: %s(arbonne.games-rpg.net/www.ascentcommunity.com)",MSG_COLOR_LIGHTBLUE);	
#endif

	if(sWorld.SendStatsOnJoin)
	{
		//_player->BroadcastMessage("Joueurs en ligne: %s%u |rMaximum: %s%u|r Connections acceptées: %s%u",
		//	MSG_COLOR_WHITE, sWorld.GetSessionCount(), MSG_COLOR_WHITE, sWorld.PeakSessionCount, MSG_COLOR_WHITE, sWorld.mAcceptedConnections);
		_player->BroadcastMessage("Joueurs en ligne: %s%u",MSG_COLOR_WHITE, sWorld.GetSessionCount());
	
	}
    
	/* Send MOTD
	_player->BroadcastMessage(sWorld.GetMotd());*/
	
	// server Message Of The Day
    SendMOTD(); 

	/* send to gms
	if( HasGMPermissions() )
		sWorld.SendMessageToGMs(this, "GM %s (%s) is now online. (Permissions: [%s])", _player->GetName(), GetAccountNameS(), GetPermissions());
	*/

	//Set current RestState
	if( plr->m_isResting) 		// We are in a resting zone, turn on Zzz
		plr->ApplyPlayerRestState(true);

	//Check if there is a time difference between lastlogoff and now
	if( plr->m_timeLogoff > 0 && plr->GetUInt32Value(UNIT_FIELD_LEVEL) < plr->GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))	// if timelogoff = 0 then it's the first login
	{
		uint32 currenttime = (uint32)UNIXTIME;
		uint32 timediff = currenttime - plr->m_timeLogoff;

		//Calculate rest bonus
		if( timediff > 0 )
			plr->AddCalculatedRestXP(timediff);
	}

	sHookInterface.OnEnterWorld2(_player);

	if(info->m_Group)
		info->m_Group->Update();

	// Retroactive: Level achievement
	_player->GetAchievementInterface()->HandleAchievementCriteriaLevelUp( _player->getLevel() );
	// Retroactive: Bank slots: broken atm :(
	//_player->GetAchievementInterface()->HandleAchievementCriteriaBuyBankSlot(true);

	// Send achievement data!
	if( _player->GetAchievementInterface()->HasAchievements() )
	{
		WorldPacket * data = _player->GetAchievementInterface()->BuildAchievementData();
		_player->CopyAndSendDelayedPacket(data);
		delete data;
	}

	if(enter_world && !_player->GetMapMgr())
	{
		plr->AddToWorld();
	}

	objmgr.AddPlayer(_player);
	SendAccountDataTimes(GLOBAL_CACHE_MASK);

	//send the worldstates to the player	
}
bool ChatHandler::HandleRenameCommand(const char * args, WorldSession * m_session)
{
	// prevent buffer overflow
	if(strlen(args) > 100)
		return false;

	char name1[100];
	char name2[100];

	if(sscanf(args, "%s %s", name1, name2) != 2)
		return false;

	if(VerifyName(name2, strlen(name2)) == false)
	{
		RedSystemMessage(m_session, "Ce nom est invalide ou contient des caractères invalides.");
		return true;
	}

	string new_name = name2;
	PlayerInfo * pi = objmgr.GetPlayerInfoByName(name1);
	if(pi == 0)
	{
		RedSystemMessage(m_session, "Joueur non trouvé avec ce nom.");
		return true;
	}

	if( objmgr.GetPlayerInfoByName(new_name.c_str()) != NULL )
	{
		RedSystemMessage(m_session, "Joueur trouvé avec ce nom.");
		return true;
	}

	objmgr.RenamePlayerInfo(pi, pi->name, new_name.c_str());

	free(pi->name);
	pi->name = strdup(new_name.c_str());

	// look in world for him
	Player * plr = objmgr.GetPlayer(pi->guid);
	if(plr != 0)
	{
		plr->SetName(new_name);
		BlueSystemMessageToPlr(plr, "%s change votre nom en '%s'.", m_session->GetPlayer()->GetName(), new_name.c_str());
		plr->SaveToDB(false);
	}
	else
	{
		CharacterDatabase.WaitExecute("UPDATE characters SET name = '%s' WHERE guid = %u", CharacterDatabase.EscapeString(new_name).c_str(), (uint32)pi->guid);
	}

	GreenSystemMessage(m_session, "Nom Changé de '%s' en '%s'.", name1, name2);
	sGMLog.writefromsession(m_session, "Personnage rebaptisé %s (GUID: %u) en %s", name1, pi->guid, name2);
	sPlrLog.writefromsession(m_session, "MJ rebatise le personnage %s (GUID: %u) en %s", name1, pi->guid, name2);
	return true;
}

void WorldSession::HandleAlterAppearance(WorldPacket & recv_data)
{
	/*
	data << uint32(0) // ok
	data << uint32(1) // not enough money
	data << uint32(2) // you must be sitting on the barber's chair

	GameObjectPointer barberChair = _player->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(_player->GetPositionX(), _player->GetPositionY(), _player->GetPositionZ(), BARBERCHAIR_ID);
	if(!barberChair || _player->GetStandState() != STANDSTATE_SIT_MEDIUM_CHAIR)
	{
		data << uint32(2);
		SendPacket(&data);
		return;
	}
	*/


	sLog.outDebug("WORLD: CMSG_ALTER_APPEARANCE");
	CHECK_PACKET_SIZE(recv_data, 12);

	uint32 hair, colour, facialhair;
	recv_data >> hair >> colour >> facialhair;

	BarberShopStyleEntry * Hair = dbcBarberShopStyle.LookupEntry(hair);
	BarberShopStyleEntry * facialHair = dbcBarberShopStyle.LookupEntry(facialhair);
	if(!facialHair || !Hair)
		return;

	uint8 newHair = Hair->hair_id;
	uint8 newFacialHair = facialHair->hair_id;
	uint32 level = _player->getLevel();
	float cost = 0;
	uint8 oldHair = _player->GetByte(PLAYER_BYTES, 2);
	uint8 oldColour = _player->GetByte(PLAYER_BYTES, 3);
	uint8 oldFacialHair = _player->GetByte(PLAYER_BYTES_2, 0);

	if(oldHair == newHair && oldColour == (uint8)colour && oldFacialHair == newFacialHair)
		return;

	if(level >= 100)
		level = 100;

	gtFloat *cutcosts = dbcBarberShopPrices.LookupEntry(level - 1);
	if(!cutcosts)
		return;

	WorldPacket data(SMSG_BARBER_SHOP_RESULT, 4);

	if(oldHair != newHair)
		cost += cutcosts->val;

    if((oldColour != colour) && (oldHair == newHair))
        cost += cutcosts->val * 0.5f;

    if(oldFacialHair != newFacialHair)
        cost += cutcosts->val * 0.75f;

	if(_player->GetUInt32Value(PLAYER_FIELD_COINAGE) < cost)
	{
		data << uint32(1); // not enough money
		SendPacket(&data);
		return;
	}

	data << uint32(0); // ok
	SendPacket(&data);
	_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -(int32)cost);
	_player->SetByte(PLAYER_BYTES, 2, newHair);
	_player->SetByte(PLAYER_BYTES, 3, (uint8)colour);
	_player->SetByte(PLAYER_BYTES_2, 0, newFacialHair);
	_player->SetStandState(0);
}