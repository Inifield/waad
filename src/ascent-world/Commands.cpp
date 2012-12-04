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
#include <svn_revision.h>
#include "ObjectMgr.h"
#include "Master.h"
/*
#define isalpha(c)  {isupper(c) || islower(c))
#define isupper(c)  (c >=  'A' && c <= 'Z')
#define islower(c)  (c >=  'a' && c <= 'z')
*/

class ChatHandler;

/////////////////////////////////////////////////
//  Normal User Chat Commands
//

bool ChatHandler::ShowHelpForCommand(WorldSession *m_session, ChatCommand *table, const char* cmd)
{
	for(uint32 i = 0; table[i].Name != NULL; i++)
	{
		if(!hasStringAbbr(table[i].Name, cmd))
			continue;

		if(m_session->CanUseCommand(table[i].CommandGroup))
			continue;

		if(table[i].ChildCommands != NULL)
		{
			// C'est quoi ca: strtok_s(NULL, " "); sans init du token ??? (Branruz)
			//cmd = strtok_s(NULL, " ",&next_token);
			//if(cmd && ShowHelpForCommand(m_session, table[i].ChildCommands, cmd))

            ShowHelpForCommand(m_session, table[i].ChildCommands, cmd);
			return true;
		}

		if(table[i].Help == "")
		{
			SystemMessage(m_session, "Il n'y a pas d'aide pour cette Commande");
			return true;
		}

		SendMultilineMessage(m_session, table[i].Help.c_str());

		return true;
	}

	return false;
}

bool ChatHandler::HandleHelpCommand(const char* args, WorldSession *m_session)
{
//	ChatCommand *table = getCommandTable();
	WorldPacket data;

	if(!*args)
		return false;

	char* next_token;
	char* cmd = strtok_s((char*)args, " ",&next_token);
	if(!cmd)
		return false;

	if(!ShowHelpForCommand(m_session, CommandTableStorage::getSingleton().Get(), cmd))
	{
		RedSystemMessage(m_session, "Il n'y a pas d'aide pour cette commande, ou celle ci n'existe pas.");
	}

	return true;
}

bool ChatHandler::HandleCommandsCommand(const char* args, WorldSession *m_session)
{
	ChatCommand *table = CommandTableStorage::getSingleton().Get();
	WorldPacket data;

	std::string output;
	uint32 count = 0;

	output = "Commandes disponibles: \n\n";

	for(uint32 i = 0; table[i].Name != NULL; i++)
	{
		if(*args && !hasStringAbbr(table[i].Name, (char*)args))
			continue;

		if(table[i].CommandGroup != '\0' && !m_session->CanUseCommand(table[i].CommandGroup))
			continue;

		switch(table[i].CommandGroup)
		{
		case 'A':
			{
				output+="|cffff6060";
				output+=table[i].Name;
				output+="|r, ";
			}
			break;
		case 'm':
			{
				output+="|cff00ffff";
				output+=table[i].Name;
				output+=", ";
			}
			break;
		case 'c':
			{
				output += "|cff00ff00";
				output += table[i].Name;
				output += "|r, ";
			}break;
		default:
			{
				output+="|cff00ccff";
				output+=table[i].Name;
				output+="|r, ";
			}
			break;
		}

		count++;
		if(count == 5)  // 5 per line
		{
			SystemMessage(m_session, output.c_str());
			output = "";
			count = 0;
		}

	}

	return true;
}

bool ChatHandler::HandleStartCommand(const char* args, WorldSession *m_session)
{
	std::string race;
	Player *m_plyr = getSelectedChar(m_session, false);

	uint8 raceid = 0;
	uint8 classid = 0;

	if (strlen(args) > 0)
	{
		race = args;
		if(race == "human")
			raceid = 1;
		else if(race == "orc")
			raceid = 2;
		else if(race == "dwarf")
			raceid = 3;
		else if(race == "nightelf" || race == "ne")
			raceid = 4;
		else if(race == "undead")
			raceid = 5;
		else if(race == "tauren")
			raceid = 6;
		else if(race == "gnome")
			raceid = 7;
		else if(race == "troll")
			raceid = 8;
		else if(race == "bloodelf" || race == "be")
			raceid = 10;
		else if(race == "draenei")
			raceid = 11;
		else if(race == "deathknight" || race == "dk")
			classid = 6;
		else
		{
			RedSystemMessage(m_session, "Invalid start location! Valid locations are: human, dwarf, gnome, nightelf, draenei, orc, troll, tauren, undead, bloodelf, deathknight");
			return true;
		}
	}
	else
	{
		raceid = m_plyr->getRace();
		classid = m_plyr->getClass();
		race = "his";
	}

	// find the first matching one
	PlayerCreateInfo *info = NULL;
	for(uint8 i = 1; i <= 11; i++)
	{
		info = objmgr.GetPlayerCreateInfo( ( raceid ? raceid : i ), ( classid ? classid : i ) );
		if(info != NULL) break;
	}
	
	if(info == NULL)
	{
		RedSystemMessage(m_session, "Internal error: Could not find create info for race %u and class %u.", raceid, classid);
		return false;
	}


	GreenSystemMessage(m_session, "Teleporting %s to %s starting location.", m_plyr->GetName(), race.c_str());

	//m_session->GetPlayer()->SafeTeleport(info->mapId, 0, LocationVector(info->positionX, info->positionY, info->positionZ));
	m_plyr->SafeTeleport(info->mapId, 0, LocationVector(info->positionX, info->positionY, info->positionZ));
	return true;
}

bool ChatHandler::HandleInfoCommand(const char* args, WorldSession *m_session)
{
	WorldPacket data;

	
	uint32 clientsNum = (uint32)sWorld.GetSessionCount();

	int gm = 0;
	int count = 0;
	int avg = 0;
	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		if(itr->second->GetSession())
		{
			count++;
			avg += itr->second->GetSession()->GetLatency();
			if(itr->second->GetSession()->HasPermissions())
				gm++;
		}			
	}
	objmgr._playerslock.ReleaseReadLock();
	GreenSystemMessage(m_session, "Server Revision: |r%sAscentWaad r%u/%s-%s-%s %s", MSG_COLOR_WHITE,
		BUILD_REVISION, CONFIG, PLATFORM_TEXT, ARCH, MSG_COLOR_LIGHTBLUE);
	GreenSystemMessage(m_session, "( arbonne.games-rpg.net / www.ascentemulator.net )");
	GreenSystemMessage(m_session, "Server Uptime: |r%s", sWorld.GetUptimeString().c_str());
	GreenSystemMessage(m_session, "Current Players: |r%d (%d GMs, %d queued)", clientsNum, gm,  0);
	GreenSystemMessage(m_session, "Active Thread Count: |r%u", ThreadPool.GetActiveThreadCount());
	GreenSystemMessage(m_session, "Free Thread Count: |r%u", ThreadPool.GetFreeThreadCount());
	GreenSystemMessage(m_session, "Average Latency: |r%.3fms", (float)((float)avg / (float)count));
	GreenSystemMessage(m_session, "SQL Query Cache Size (World): |r%u queries delayed", WorldDatabase.GetQueueSize());
	GreenSystemMessage(m_session, "SQL Query Cache Size (Character): |r%u queries delayed", CharacterDatabase.GetQueueSize());

	return true;
}

bool ChatHandler::HandleNYICommand(const char* args, WorldSession *m_session)
{
	RedSystemMessage(m_session, "Pas encore implemente.");
	return true;
}

bool ChatHandler::HandleDismountCommand(const char* args, WorldSession *m_session)
{
	Unit *m_target = NULL;

	Player *p_target = getSelectedChar(m_session, false);

	if(p_target)
		m_target = p_target;
	else
	{
		Creature *m_crt = getSelectedCreature(m_session, false);
		if(m_crt)
			m_target = m_crt;
	}

	if(!m_target)
	{
		RedSystemMessage(m_session, "Pas de cible trouvee.");
		return true;
	}

	if(m_target->GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID) == 0)
	{
		RedSystemMessage(m_session, "%s n'est pas sur une monture.",(m_target->GetObjectName())?(m_target->GetObjectName()):"(Inconnu)");
		return true;
	}

	if(p_target && p_target->m_MountSpellId)
		p_target->RemoveAura(p_target->m_MountSpellId);

	m_target->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , 0);
	//m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);

	BlueSystemMessage(m_session,  "%s n'est plus sur une monture.",(m_target->GetObjectName())?(m_target->GetObjectName()):"(Inconnu)");
	return true;
}


bool ChatHandler::HandleSaveCommand(const char* args, WorldSession *m_session)
{
	WorldPacket data;
	if(m_session->GetPlayer()->m_nextSave < 300000 || m_session->CanUseCommand('s')) //5min out of 10 left so 5 min since last save
	{
		m_session->GetPlayer()->SaveToDB(false);
		GreenSystemMessage(m_session, "%s sauvegardé dans la Base de Donnee",m_session->GetPlayer()->GetName());
	}
	else
	{
		RedSystemMessage(m_session, "Vous pouvez sauvegarder seulement toutes les 5mn.");
	}
	m_session->SendPacket( &data );
	return true;
}

bool ChatHandler::HandleGMListCommand(const char* args, WorldSession *m_session)
{
	WorldPacket data;
	bool first = true;

	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		if(itr->second->GetSession()->HasPermissions())
		{
			if(first)
				GreenSystemMessage(m_session, "Maitre(s) de Jeu connectés sur le serveur:");

			SystemMessage(m_session, "%s [%s]", itr->second->GetName(), 
				(itr->second->GetSession()->HasSuperAdminPermissions()) ? "SuperAdmin" : 
				(itr->second->GetSession()->HasAdminPermissions()) ? "Admin" :
				(itr->second->GetSession()->HasGMPermissions()) ? "Maitre de Jeu" : "Adviseur");

			first = false;
		}
	}
	objmgr._playerslock.ReleaseReadLock();

	if(first)
		SystemMessage(m_session, "Il n'y a aucun Maitre du jeu connecté.");

	return true;
}

bool ChatHandler::HandleRangeCheckCommand( const char *args , WorldSession *m_session )
{
	WorldPacket data;
	uint64 guid = m_session->GetPlayer()->GetSelection();
	m_session->SystemMessage( "=== RANGE CHECK ===" );
	if (guid == 0)
	{
		m_session->SystemMessage("Aucune selection.");
		return true;
	}

	Unit *unit = m_session->GetPlayer()->GetMapMgr()->GetUnit( guid );
	if(!unit)
	{
		m_session->SystemMessage("Selection invalide.");
		return true;
	}
	float DistSq = unit->GetDistanceSq( static_cast<Object*>(m_session->GetPlayer()) );
	m_session->SystemMessage( "GetDistanceSq  :   %u" , FL2UINT( DistSq ) );
	LocationVector locvec( m_session->GetPlayer()->GetPositionX() , m_session->GetPlayer()->GetPositionY() , m_session->GetPlayer()->GetPositionZ() );
	float DistReal = unit->CalcDistance( locvec );
	m_session->SystemMessage( "CalcDistance   :   %u" , FL2UINT( DistReal ) );
	float Dist2DSq = unit->GetDistance2dSq( static_cast<Object*>(m_session->GetPlayer()) );
	m_session->SystemMessage( "GetDistance2dSq:   %u" , FL2UINT( Dist2DSq ) );
	return true;
}

bool ChatHandler::HandleGmLogCommentCommand( const char *args , WorldSession *m_session )
{
	if(!args || !strlen(args)) return false;
	BlueSystemMessage(m_session, "Added Log Comment: %s",args);
	sGMLog.writefromsession(m_session,"Log Comment: %s", args);
	return true;
}

bool ChatHandler::HandleRatingsCommand( const char *args , WorldSession *m_session )
{
	m_session->SystemMessage("Ratings!!!");
	Player* m_plyr = getSelectedChar(m_session, false);
	for( uint32 i = 0; i < 24; i++ )
	{
		m_plyr->ModUnsigned32Value( PLAYER_FIELD_COMBAT_RATING_1 + i, i );
	}
	m_plyr->UpdateStats();
	return true;
}

/////////////////////////////////////////////////
//  GM Chat Commands
//

bool ChatHandler::HandleAnnounceCommand(const char* args, WorldSession *m_session)
{
	if( !*args || strlen(args) < 4 || strchr(args, '%'))
	{
		m_session->SystemMessage("Les annonces ne peuvent contenir le caractere %% et doivent etre d'au moins de 4 caracteres.");
		return true;
	}

	char msg[1024];
	string input2;
	input2 = sWorld.ann_tagcolor;
	input2 += "[";
	input2 += sWorld.announce_tag + "]";
	if(sWorld.GMAdminTag)
	{
		input2 += sWorld.ann_gmtagcolor;
		if(m_session->HasSuperAdminPermissions()) input2+="<SuperAdmin>";
		else if(m_session->HasAdminPermissions()) input2+="<Admin>";
		else if(m_session->HasPermissions()) input2+="<GM>";
	}

	if(sWorld.NameinAnnounce)
	{
		input2+=sWorld.ann_namecolor;
		input2+="|Hplayer:";
		input2+=m_session->GetPlayer()->GetName();
		input2+="|h[";
		input2+=m_session->GetPlayer()->GetName();
		input2+="]|h";
	}
	input2+=": ";
	input2+=sWorld.ann_msgcolor;
	snprintf((char*)msg, 1024, "%s%s", input2.c_str(), args);

	sWorld.SendWorldText(msg); // send message
	sGMLog.writefromsession(m_session, "utilise la commande announce, [%s]", args);
	return true;
}

bool ChatHandler::HandleWAnnounceCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	char pAnnounce[1024];
	string input3;
	input3 = sWorld.ann_tagcolor;
	input3 +="[";
	input3 += sWorld.announce_tag;
	input3 += "]";
	if(sWorld.GMAdminTag)
	{
		input3 += sWorld.ann_gmtagcolor;
		if(m_session->HasSuperAdminPermissions()) input3+="<SuperAdmin>";
		else if(m_session->HasAdminPermissions()) input3+="<Admin>";
		else if(m_session->HasGMPermissions()) input3+="<GM>";
	}
	if(sWorld.NameinWAnnounce)
	{
	input3+=sWorld.ann_namecolor;
	input3+="[";
	input3+=m_session->GetPlayer()->GetName();
	input3+="]";
	}
	input3+=": ";
	input3+=sWorld.ann_msgcolor;
	snprintf((char*)pAnnounce, 1024, "%s%s", input3.c_str(), args);

	sWorld.SendWorldWideScreenText(pAnnounce); // send message
	sGMLog.writefromsession(m_session, "utilise la commande wannounce [%s]", args);
	return true;
}

bool ChatHandler::HandleGMAnnounceCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
	{
	  RedSystemMessage(m_session, "Pas d'argument...");
	  return false;
	}

	char GMAnnounce[1024];
	string input2;
	input2 = "|cffff6060[TEAM]<";
	if(m_session->HasSuperAdminPermissions()) input2+="SuperAdmin";
	else if(m_session->HasAdminPermissions()) input2+="Admin";
	else if(m_session->HasGMPermissions()) input2+="GM";
	input2+=">|r|c1f40af20|Hplayer:";
	input2+=m_session->GetPlayer()->GetName();
	input2+="|h";
	input2+=+m_session->GetPlayer()->GetName();
	input2+="|h";
	input2+=":|r ";
	snprintf((char*)GMAnnounce, 1024, "%s%s", input2.c_str(), args);
	sWorld.SendGMWorldText(GMAnnounce);
	sGMLog.writefromsession(m_session, "utilise la commande team announce, [%s]", args);
	return true;
}

bool ChatHandler::HandleGMOnCommand(const char* args, WorldSession *m_session)
{
	/*uint32 newbytes = m_session->GetPlayer( )->GetUInt32Value(PLAYER_BYTES_2) | 0x8;
	m_session->GetPlayer( )->SetUInt32Value( PLAYER_BYTES_2, newbytes);

	GreenSystemMessage(m_session, "GM Flag Set.");*/
	GreenSystemMessage(m_session, "Setting GM Flag on yourself...");
	if(m_session->GetPlayer()->bGMTagOn)
		RedSystemMessage(m_session, "GM Flag is already set on. Use !gmoff to disable it.");
	else
	{
		m_session->GetPlayer()->bGMTagOn = true;
		m_session->GetPlayer()->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_GM);	// <GM>
		BlueSystemMessage(m_session, "GM Flag Set. It will appear above your name and in chat messages until you use !gmoff.");
	}

	return true;
}

bool ChatHandler::HandleGMOffCommand(const char* args, WorldSession *m_session)
{
	//uint32 newbytes = m_session->GetPlayer( )->GetUInt32Value(PLAYER_BYTES_2) & ~(0x8);
	//m_session->GetPlayer( )->SetUInt32Value( PLAYER_BYTES_2, newbytes);

	//GreenSystemMessage(m_session, "GM Flag Unset.");
	if(sWorld.m_forceGMTag && !m_session->HasSuperAdminPermissions())
	{
		GreenSystemMessage(m_session, "Vous etes force d'utiliser le tag GM!");
		return false;
	}
	else
	{
	GreenSystemMessage(m_session, "Unsetting GM Flag on yourself...");
	if(!m_session->GetPlayer()->bGMTagOn)
		RedSystemMessage(m_session, "GM Flag not set. Use !gmon to enable it.");
	else
	{
		m_session->GetPlayer()->bGMTagOn = false;
		m_session->GetPlayer()->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_GM);	// <GM>
		BlueSystemMessage(m_session, "GM Flag Removed. <GM> Will no longer show in chat messages or above your name.");
	}
	return true;
	}
}

bool ChatHandler::HandleGPSCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	Object *obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid);
	if (guid != 0)
	{
		if(!obj)
		{
			SystemMessage(m_session, "Vous devez selectionner un joueur ou une creature.");
			return true;
		}
	}
	else
		obj = (Object*)m_session->GetPlayer();

	AreaTable * at = dbcArea.LookupEntry(obj->GetMapMgr()->GetAreaID(obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ()));
	if(!at) return true;

	char buf[256];
	snprintf((char*)buf, 256, "|cff00ff00Current Position: |cffffffffMap: |cff00ff00%d |cffffffffZone: |cff00ff00%u |cffffffffX: |cff00ff00%f |cffffffffY: |cff00ff00%f |cffffffffZ: |cff00ff00%f |cffffffffOrientation: |cff00ff00%f|r",
		(unsigned int)obj->GetMapId(), at->ZoneId, obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ(), obj->GetOrientation());
	
	
	SystemMessage(m_session, buf);

	return true;
}

bool ChatHandler::HandleKickCommand(const char* args, WorldSession *m_session)
{

	if(!*args)
	return false;

	bool SilentKick = false;

	char *next_token;
	char *pname = strtok_s((char*)args, " ",&next_token);

	if(!pname)
	{
		RedSystemMessage(m_session, "Aucun nom specifie.");
		return true;
	}
	Player *chr = objmgr.GetPlayer((const char*)pname, false);
	if (chr)
	{
		char *reason = strtok_s(NULL, " ",&next_token);

		std::string kickreason = "Aucune raison";
		if(reason)
			kickreason = reason;
		char* pSilentKick = strtok_s(NULL, "\n",&next_token);
		
		if (pSilentKick)
			SilentKick = (atoi(pSilentKick)>0?true:false);

		BlueSystemMessage(m_session, "Attempting to kick %s from the server for \"%s\".", chr->GetName(), kickreason.c_str());
		sGMLog.writefromsession(m_session, "Kicked player %s from the server for %s", chr->GetName(), kickreason.c_str());
		if(!m_session->HasSuperAdminPermissions() && chr->GetSession()->HasSuperAdminPermissions())
		{
			RedSystemMessage(m_session, "You cannot kick %s, as they are a higher level gm than you.", chr->GetName());
			return true;
		}
		/*if(m_session->GetSecurity() < chr->GetSession()->GetSecurity())
		{
			SystemMessage(m_session, "You cannot kick %s, as he is a higher GM level than you.", chr->GetName());
			return true;
		}*/ // we might have to re-work this

		if(SilentKick == false && sWorld.m_showKick)
		{
			char msg[200];
			snprintf(msg, 200, "%sGM: %s a ete kicke du serveur par %s. Raison: %s", MSG_COLOR_RED, chr->GetName(), m_session->GetPlayer()->GetName(), kickreason.c_str());
			sWorld.SendWorldText(msg, NULL);
			//sWorld.SendIRCMessage(msg);
		}
		
		SystemMessageToPlr(chr, "Vous etes kické du serveur par %s. Raison: %s", m_session->GetPlayer()->GetName(), kickreason.c_str());
		
		WorldPacket data;
		data.Initialize(SMSG_FORCE_MOVE_ROOT);
		data << chr->GetNewGUID();
		data << uint32(1);
		chr->SendMessageToSet(&data, true);
		
		chr->Kick(6000);
		
		return true;
	} 
	else 
	{
		RedSystemMessage(m_session, "Le joueur n'est pas en ligne pour le moment.");
		return true;
	}
	return true;
}

bool ChatHandler::HandleAddFriendCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	char *next_token;
	char *fname = strtok_s((char*)args, " ",&next_token);
	char *note = "";
	if(!fname)
	{
		RedSystemMessage(m_session, "No friend to add specified.");
		return false;
	}

	Player* target = this->getSelectedChar(m_session, true);
	if(!target) 
		return false;
	target->Social_AddFriend(fname, note);
	return true;
}

bool ChatHandler::HandleAddInvItemCommand(const char *args, WorldSession *m_session)
{
	uint32 itemid, count=1;
	int32 randomprop=0;

	if(strlen(args) < 1)
	{
		return false;
	}

	if(sscanf(args, "%u %u %d", &itemid, &count, &randomprop) < 1)
		return false;

	Player *chr = getSelectedChar(m_session);
	if (chr == NULL) return true;
	
	ItemPrototype* it = ItemPrototypeStorage.LookupEntry(itemid);
	if(it)
	{
		sGMLog.writefromsession(m_session, "utilise la commande add item, item id %u [%s] a %s", it->ItemId, it->Name1, chr->GetName());
		Item *item;
		item = objmgr.CreateItem( itemid, chr);
		item->SetUInt32Value(ITEM_FIELD_STACK_COUNT, ((count > it->MaxCount) ? it->MaxCount : count));
		if(it->Bonding==ITEM_BIND_ON_PICKUP)
			item->SoulBind();
		if(randomprop!=0)
		{
			if(randomprop<0)
				item->SetRandomSuffix(abs(int(randomprop)));
			else
				item->SetRandomProperty(randomprop);

			item->ApplyRandomProperties(false);
		}
	  
		if(!chr->GetItemInterface()->AddItemToFreeSlot(item))
		{
			m_session->SendNotification("Aucune place libre n'a ete trouve dans votre inventaire!");
			item->Delete();
			//delete item;
			return true;
		}

		char messagetext[128];
		snprintf(messagetext, 128, "Adding item %d (%s) to %s's inventory.",(unsigned int)it->ItemId,it->Name1, chr->GetName());
		SystemMessage(m_session, messagetext);
		snprintf(messagetext, 128, "%s added item %d (%s) to your inventory.", m_session->GetPlayer()->GetName(), (unsigned int)itemid, it->Name1);
		SystemMessageToPlr(chr,  messagetext);

		SlotResult *lr = chr->GetItemInterface()->LastSearchResult();
		chr->GetSession()->SendItemPushResult(item,false,true,false,true,lr->ContainerSlot,lr->Slot,count);

		return true;
	} else {
		RedSystemMessage(m_session, "Item %d n'est pas un idem valide!",itemid);
		return true;
	}
}

bool ChatHandler::HandleSummonCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	sGMLog.writefromsession(m_session, "summoned %s", args);

	Player *chr = objmgr.GetPlayer(args, false);
	if (chr)
	{
		// send message to user
		char buf[256];
		char buf0[256];
		if(chr->IsBeingTeleported()==true) 
		{
			snprintf((char*)buf,256, "%s a deja ete teleporte.", chr->GetName());
			SystemMessage(m_session, buf);
			return true;
		}
		snprintf((char*)buf,256, "Vous faites apparaitre %s devant vous.", chr->GetName());
		SystemMessage(m_session, buf);

		if(!m_session->GetPlayer()->m_isGmInvisible)
		{
			// send message to player
			snprintf((char*)buf0,256, "Vous apparaissez devant %s a sa requete.", m_session->GetPlayer()->GetName());
			SystemMessageToPlr(chr, buf0);
		}

		Player * plr = m_session->GetPlayer();

		if(plr->GetMapMgr() == chr->GetMapMgr())
			chr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), plr->GetPosition());
			//chr->_Relocate(plr->GetMapId(),plr->GetPosition(),false,false,plr->GetInstanceID());
		else
		{
			sEventMgr.AddEvent(chr,&Player::EventPortToGM,plr,0,1,1,0);
		}
	}
	else
	{
		PlayerInfo * pinfo = objmgr.GetPlayerInfoByName(args);
		if(!pinfo)
		{
			char buf[256];
			snprintf((char*)buf,256,"%s n'existe pas ou n'est pas connecte!", args);
			SystemMessage(m_session, buf);
		}
		else
		{
			Player * pPlayer = m_session->GetPlayer();
			char query[512];
			snprintf((char*) &query,512, "UPDATE characters SET mapId = %u, positionX = %f, positionY = %f, positionZ = %f, zoneId = %u WHERE guid = %u;",	pPlayer->GetMapId(), pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), pPlayer->GetZoneId(), pinfo->guid);
			CharacterDatabase.Execute(query);
			char buf[256];
			snprintf((char*)buf,256,"[%s Offline] Teleportation enregistree.", pinfo->name);
			SystemMessage(m_session, buf);
		}
	}
	return true;
}

bool ChatHandler::HandleAppearCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	Player *chr = objmgr.GetPlayer(args, false);
	if (chr)
	{
		char buf[256];
		if( chr->IsBeingTeleported() ) {
			snprintf((char*)buf,256, "%s a deja ete teleporte.", chr->GetName());
			SystemMessage(m_session, buf);
			return true;
		}
		snprintf((char*)buf,256, "Vous apparaissez devant %s.", chr->GetName());  // -- europa
		SystemMessage(m_session, buf);

		if(!m_session->GetPlayer()->m_isGmInvisible)
		{
			char buf0[256];
			snprintf((char*)buf0,256, "%s apparait devant vous.", m_session->GetPlayer()->GetName());
			SystemMessageToPlr(chr, buf0);
		}

		//m_session->GetPlayer()->SafeTeleport(chr->GetMapId(), chr->GetInstanceID(), chr->GetPosition());
		//If the GM is on the same map as the player, use the normal safeteleport method
		if ( m_session->GetPlayer()->GetMapId() == chr->GetMapId() && m_session->GetPlayer()->GetInstanceID() == chr->GetInstanceID() )
			m_session->GetPlayer()->SafeTeleport(chr->GetMapId(),chr->GetInstanceID(),chr->GetPosition());
		else
			m_session->GetPlayer()->SafeTeleport(chr->GetMapMgr(), chr->GetPosition());
		//The player and GM are not on the same map. We use this method so we can port to BG's (Above method doesn't support them)
	}
	else
	{
		char buf[256];
		snprintf((char*)buf,256, "%s n'existe pas ou n'est pas connecte!", args);
		SystemMessage(m_session, buf);
	}

	return true;
}

bool ChatHandler::HandleTaxiCheatCommand(const char* args, WorldSession *m_session)
{
	if (!*args)
		return false;

	int flag = atoi((char*)args);

	Player *chr = getSelectedChar(m_session);
	if (chr == NULL) return true;
	
	char buf[256];

	// send message to user
	if (flag != 0)
	{
		snprintf((char*)buf,256, "%s has all taxi nodes now.", chr->GetName());
	}
	else
	{
		snprintf((char*)buf,256, "%s has no more taxi nodes now.", chr->GetName());
	}
	GreenSystemMessage(m_session, buf);
	
	// send message to player
	if (flag != 0)
	{
		snprintf((char*)buf,256, "%s has given you all taxi nodes.",
			m_session->GetPlayer()->GetName());
	}
	else
	{
		snprintf((char*)buf,256, "%s has deleted all your taxi nodes.",
			m_session->GetPlayer()->GetName());
	}
	SystemMessage(m_session, buf);

	for (uint8 i=0; i<8; i++)
	{
		if (flag != 0)
		{
			m_session->GetPlayer()->SetTaximask(i, 0xFFFFFFFF);
		}
		else
		{
			m_session->GetPlayer()->SetTaximask(i, 0);
		}
	}

	return true;
}

bool ChatHandler::HandleModifySpeedCommand(const char* args, WorldSession *m_session)
{
	WorldPacket data;

	if (!*args)
		return false;

	float Speed = (float)atof((char*)args);

	if (Speed > 255 || Speed < 1)
	{
		RedSystemMessage(m_session, "Incorrect value. Range is 1..255");
		return true;
	}

	Player *chr = getSelectedChar(m_session);
	if( chr == NULL )
		return true;
	
	char buf[256];

	// send message to user
	BlueSystemMessage(m_session, "You set the speed of %s to %2.2f.", chr->GetName(), Speed);

	// send message to player
	snprintf((char*)buf,256, "%s set your speed to %2.2f.", m_session->GetPlayer()->GetName(), Speed);
	SystemMessage(chr->GetSession(), buf);

	chr->SetPlayerSpeed(RUN, Speed);
	chr->SetPlayerSpeed(SWIM, Speed);
	chr->SetPlayerSpeed(RUNBACK, Speed);
	chr->SetPlayerSpeed(FLY, Speed);

	return true;
}

bool ChatHandler::HandleLearnSkillCommand(const char *args, WorldSession *m_session)
{
	uint32 skill, min, max;
	min = max = 1;
	char *next_token;
	char *pSkill = strtok_s((char*)args, " ",&next_token);
	if(!pSkill)
		return false;
	else
		skill = atol(pSkill);

	BlueSystemMessage(m_session, "Adding skill line %d", skill);

	char *pMin = strtok_s(NULL, " ",&next_token);
	if(pMin)
	{
		min = atol(pMin);
		char *pMax = strtok_s(NULL, "\n",&next_token);
		if(pMax)
			max = atol(pMax);
	} else {
		return false;
	}

	Player *plr = getSelectedChar(m_session, true);
	if(!plr) return false;
	if(plr->GetTypeId() != TYPEID_PLAYER) return false;
	sGMLog.writefromsession(m_session, "used add skill of %u %u %u on %s", skill, min, max, plr->GetName());

	plr->_AddSkillLine(skill, min, max);   

	return true;
}

bool ChatHandler::HandleModifySkillCommand(const char *args, WorldSession *m_session)
{
	uint32 skill, min, max;
	min = max = 1;
	char *next_token;
	char *pSkill = strtok_s((char*)args, " ",&next_token);
	if(!pSkill)
		return false;
	else
		skill = atol(pSkill);
	
	char *pMin = strtok_s(NULL, " ",&next_token);
	uint32 cnt = 0;
	if(!pMin)
		cnt = 1;
	else
		cnt = atol(pMin);

	skill = atol(pSkill);
	
	BlueSystemMessage(m_session, "Modifying skill line %d. Advancing %d times.", skill, cnt);

	Player *plr = getSelectedChar(m_session, true);
	if(!plr) plr = m_session->GetPlayer();
	if(!plr) return false;
	sGMLog.writefromsession(m_session, "used modify skill of %u %u on %s", skill, cnt,plr->GetName());

	if(!plr->_HasSkillLine(skill))
	{
		SystemMessage(m_session, "Does not have skill line, adding.");
		plr->_AddSkillLine(skill, 1, 300);   
	} else {
		plr->_AdvanceSkillLine(skill,cnt);
	}	   

	return true;
}

/// DGM: Get skill level command for getting information about a skill
bool ChatHandler::HandleGetSkillLevelCommand(const char *args, WorldSession *m_session)
{
	uint32 skill = 0;
	char *next_token;
	char *pSkill = strtok_s((char*)args, " ",&next_token);
	if(!pSkill)
		return false;
	else 
		skill = atol(pSkill);

	Player *plr = getSelectedChar(m_session, true);
	if(!plr) return false;

	if(skill > SkillNameManager->maxskill)
	{
		BlueSystemMessage(m_session, "Skill: %u does not exists", skill);
		return false;
	}

    char * SkillName = SkillNameManager->SkillNames[skill];

    if (SkillName==0)
    {
        BlueSystemMessage(m_session, "Skill: %u does not exists", skill);
        return false;
    }
    
    if (!plr->_HasSkillLine(skill))
    {
        BlueSystemMessage(m_session, "Player does not have %s skill.", SkillName);
        return false;
    }

	uint32 nobonus = plr->_GetSkillLineCurrent(skill,false);
	uint32 bonus = plr->_GetSkillLineCurrent(skill,true) - nobonus;
    uint32 max = plr->_GetSkillLineMax(skill);

    BlueSystemMessage(m_session, "Player's %s skill has level: %u maxlevel: %u. (+ %u bonus)", SkillName,max,nobonus, bonus);
	return true;
}

bool ChatHandler::HandleGetSkillsInfoCommand(const char *args, WorldSession *m_session)
{
    Player *plr = getSelectedChar(m_session, true);
    if(!plr) return false;
    
    uint32 nobonus = 0;
    int32  bonus = 0;
    uint32 max = 0;

    BlueSystemMessage(m_session, "Player: %s has skills", plr->GetName() );

    for (uint32 SkillId = 0; SkillId <= SkillNameManager->maxskill; SkillId++)
    {
        if (plr->_HasSkillLine(SkillId))
        {
            char * SkillName = SkillNameManager->SkillNames[SkillId];
            if (!SkillName)
            {
                RedSystemMessage(m_session, "Invalid skill: %u", SkillId);
                continue;
            }

            nobonus = plr->_GetSkillLineCurrent(SkillId,false);
            bonus = plr->_GetSkillLineCurrent(SkillId,true) - nobonus;
            max = plr->_GetSkillLineMax(SkillId);

            BlueSystemMessage(m_session, "  %s: Value: %u, MaxValue: %u. (+ %d bonus)", SkillName, nobonus,max, bonus);
        }
    }

    return true;
}

bool ChatHandler::HandleRemoveSkillCommand(const char *args, WorldSession *m_session)
{
	uint32 skill = 0;
	char *next_token;
	char *pSkill = strtok_s((char*)args, " ",&next_token);
	if(!pSkill)
		return false;
	else 
		skill = atol(pSkill);
	BlueSystemMessage(m_session, "Removing skill line %d", skill);

	Player *plr = getSelectedChar(m_session, true);
	if(plr && plr->_HasSkillLine(skill) ) //fix bug; removing skill twice will mess up skills
	{
		plr->_RemoveSkillLine(skill);
		sGMLog.writefromsession(m_session, "used remove skill of %u on %s", skill, plr->GetName());
		SystemMessageToPlr(plr, "%s removed skill line %d from you. ", m_session->GetPlayer()->GetName(), skill);
	}
	else
	{
		BlueSystemMessage(m_session, "Player doesn't have skill line %d", skill);
	}
	return true;
}

bool ChatHandler::HandleEmoteCommand(const char* args, WorldSession *m_session)
{
	uint32 emote_state;
	uint32 RecDB;
	char Buff[128];

	int valcount = sscanf(args, "%u %u", (uint32*)&emote_state, (uint32*)&RecDB);
	if(valcount == 0) return false;

	//uint32 emote = atoi((char*)args);
	Unit* target = this->getSelectedCreature(m_session);
	if(!target) return false;

	target->SetUInt32Value(UNIT_NPC_EMOTESTATE,emote_state);
	BlueSystemMessage(m_session,"%s: emote %u",((Creature *)target)->GetCreatureName()->Name,emote_state);

    if(valcount == 2)
	{
		if(RecDB)
		{ 
		 snprintf(Buff, 128,"UPDATE creature_spawns SET emote_state = '%u' WHERE id = '%u'", (uint32)emote_state, (uint32)((Creature *)target)->GetSQL_id());
		 WorldDatabase.Execute(Buff);
		 BlueSystemMessage(m_session,"NPC (%s) emote sauvegardé",((Creature *)target)->GetCreatureName()->Name);
		}
		else BlueSystemMessage(m_session,"NPC emote : aucune sauvegarde");
	}
	else BlueSystemMessage(m_session,"NPC emote : aucune sauvegarde");
	
	return true;
}

bool ChatHandler::HandleModifyGoldCommand(const char* args, WorldSession *m_session)
{
//	WorldPacket data;

	if ( *args == 0 )
		return false;

	Player *chr = getSelectedChar( m_session, true );
	if( chr == NULL ) return true;

	int32 total   = atoi( (char*)args );

	// gold = total / 10000;
	// silver = (total / 100) % 100;
	// copper = total % 100;
	uint32 gold   = (uint32) floor( (float)int32abs( total ) / 10000.0f );
	uint32 silver = (uint32) floor( ((float)int32abs( total ) / 100.0f) ) % 100;
	uint32 copper = int32abs2uint32( total ) % 100;
	
	sGMLog.writefromsession( m_session, "used modify gold on %s, gold: %d", chr->GetName(), total );

	int32 newgold = chr->GetUInt32Value( PLAYER_FIELD_COINAGE ) + total;

	if(newgold < 0)
	{
		BlueSystemMessage( m_session, "Taking all gold from %s's backpack...", chr->GetName() );
		GreenSystemMessageToPlr(chr, "%s took the all gold from your backpack.", m_session->GetPlayer()->GetName());
		newgold = 0;
	}
	else
	{
		if(total >= 0) {
			BlueSystemMessage( m_session,
				"Adding %u gold, %u silver, %u copper to %s's backpack...",
				gold, silver, copper,
				chr->GetName() );

			GreenSystemMessageToPlr( chr, "%s added %u gold, %u silver, %u copper to your backpack.",
				m_session->GetPlayer()->GetName(),
				gold, silver, copper );
		}
		else
		{
			BlueSystemMessage( m_session,
				"Taking %u gold, %u silver, %u copper from %s's backpack...",
				gold, silver, copper,
				chr->GetName() );

			GreenSystemMessageToPlr( chr, "%s took %u gold, %u silver, %u copper from your backpack.",
				m_session->GetPlayer()->GetName(),
				gold, silver, copper );
		}
	}

	chr->SetUInt32Value( PLAYER_FIELD_COINAGE, newgold );
	
	return true;
}

bool ChatHandler::HandleTriggerCommand(const char* args, WorldSession* m_session)
{
	int32 instance_id;
	uint32 trigger_id;
	int valcount = sscanf(args, "%u %d", (unsigned int*)&trigger_id, (int*)&instance_id);
	if(!valcount)
		return false;
	if(valcount == 1)
		instance_id = 0;

	AreaTrigger *pTrigger = AreaTriggerStorage.LookupEntry(trigger_id);
	if(trigger_id == 0 || pTrigger == NULL)
	{
		RedSystemMessage(m_session, "Could not find trigger %s", (args == NULL ? "NULL" : args));
		return true;
	}

	m_session->GetPlayer()->SafeTeleport(pTrigger->Mapid, instance_id, LocationVector(pTrigger->x, pTrigger->y,
			pTrigger->z, pTrigger->o));

	BlueSystemMessage(m_session, "Teleported to trigger %u on [%u][%.2f][%.2f][%.2f]", pTrigger->AreaTriggerID,
		pTrigger->Mapid, pTrigger->x, pTrigger->y, pTrigger->z);
	return true;
}

bool ChatHandler::HandleUnlearnCommand(const char* args, WorldSession * m_session)
{
	Player * plr = getSelectedChar(m_session, true);
	if(plr == 0)
		return true;

	uint32 SpellId = atol(args);
	if(SpellId == 0)
	{
		RedSystemMessage(m_session, "You must specify a spell id.");
		return true;
	}

	sGMLog.writefromsession(m_session, "removed spell %u from %s", SpellId, plr->GetName());

	if(plr->HasSpell(SpellId))
	{
		GreenSystemMessageToPlr(plr, "Removed spell %u.", SpellId);
		plr->removeSpell(SpellId, false, false, 0);
	}
	else
	{
		RedSystemMessage(m_session, "That player does not have spell %u learnt.", SpellId);
	}

	return true;
}

bool ChatHandler::HandleNpcSpawnLinkCommand(const char* args, WorldSession *m_session)
{
	uint32 id;
	char sql[512];
	Creature* target = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(m_session->GetPlayer()->GetSelection()));
	if (!target)
		return false;

	int valcount = sscanf(args, "%u", (unsigned int*)&id);
	if(valcount)
	{
		snprintf(sql, 512, "UPDATE creature_spawns SET respawnlink = '%u' WHERE id = '%u'", (unsigned int)id, (unsigned int)target->GetSQL_id());
		WorldDatabase.Execute( sql );
		BlueSystemMessage(m_session, "Spawn linking for this NPC has been updated: %u", id);
	}
	else
	{
		RedSystemMessage(m_session, "Sql entry invalid %u", id);
	}

	return true;
}

bool ChatHandler::HandleResetReputationCommand(const char *args, WorldSession *m_session)
{
	Player *plr = getSelectedChar(m_session);
	if(!plr)
	{
		SystemMessage(m_session, "Selectionner un joueur ou vous d'abord.");
		return true;
	}
	
	plr->_InitialReputation();
	SystemMessage(m_session, "Relog pour que les modifications prennent effet.");
	sGMLog.writefromsession(m_session, "Utilisation de la reinitialisation de reputation pour %s", plr->GetName());
	return true;
}

bool ChatHandler::HandleInvincibleCommand(const char *args, WorldSession *m_session)
{
	Player *chr = getSelectedChar(m_session);
	char msg[100];
	if(chr)
	{
		chr->bInvincible = !chr->bInvincible;
		snprintf(msg, 100, "Vous ete Invincible %s", chr->bInvincible ? "ON. Vous aurez peut-etre a sortir et re-entrer dans cette zone pour que les modifications prennent effet." : "OFF. Sortez et re-entrez dans cette zone pour que ce changement prenne effet.");
	} else {
		snprintf(msg, 100, "Selectionner un joueur ou vous d'abord.");
	}
	if(chr!=m_session->GetPlayer()&&chr)
		sGMLog.writefromsession(m_session, "toggled invincibility on %s", chr->GetName());
	SystemMessage(m_session, msg);
	return true;
}

bool ChatHandler::HandleInvisibleCommand(const char *args, WorldSession *m_session)
{
	char msg[256];
	memset(msg,0x00,256);
	Player* pChar =m_session->GetPlayer();

	snprintf(msg, 250, "Invisibility and Invincibility are now ");
	if(pChar->m_isGmInvisible)
	{
		pChar->m_isGmInvisible = false;
		pChar->m_auracount[SPELL_AURA_MOD_INVISIBILITY]--;
		pChar->bInvincible = false;
		pChar->Social_TellFriendsOnline();
		snprintf(msg, 250, "%s OFF.", msg);
	} else {
		pChar->m_isGmInvisible = true;
		pChar->m_auracount[SPELL_AURA_MOD_INVISIBILITY]++;
		pChar->bInvincible = true;
		pChar->Social_TellFriendsOffline();		
		snprintf(msg, 250, "%s ON.", msg);
	}

	snprintf(msg, 250, "%s You may have to leave and re-enter this zone for changes to take effect.", msg);

	GreenSystemMessage(m_session, (const char*)msg);
	return true;
}

bool ChatHandler::CreateGuildCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	Player * ptarget = getSelectedChar(m_session);
	if(!ptarget) return false;

	if(strlen((char*)args)>75)
	{
		// send message to user
		char buf[256];
		memset(buf,0x00,256);
		snprintf((char*)buf,250,"The name was too long by %i", strlen((char*)args)-75);
		SystemMessage(m_session, buf);
		return true;
	}

	for (uint32 i = 0; i < strlen(args); i++) {
		if(!isalpha(args[i]) && args[i]!=' ') {
			SystemMessage(m_session, "Error, name can only contain chars A-Z and a-z.");
			return true;
		}
	}

	Charter tempCharter(0, ptarget->GetLowGUID(), CHARTER_TYPE_GUILD);
	tempCharter.SignatureCount=0;
	tempCharter.GuildName = string(args);

	Guild * pGuild = Guild::Create();
	pGuild->CreateFromCharter(&tempCharter, ptarget->GetSession());
	SystemMessage(m_session, "Guild created");
	return true;
}

bool ChatHandler::HandleDeleteCommand(const char* args, WorldSession *m_session)
{

	uint64 guid = m_session->GetPlayer()->GetSelection(); //on chope la cible du joueur
	if (guid == 0) //on test si il y a quelquechose de ciblé
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	Creature *unit = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!unit) //on test si la cible est un npc
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}
	sGMLog.writefromsession(m_session, "used npc delete, sqlid %u, creature %s, pos %f %f %f",
		unit->GetSQL_id(), unit->GetCreatureName() ? unit->GetCreatureName()->Name : "wtfbbqhax", unit->GetPositionX(), unit->GetPositionY(),
		unit->GetPositionZ()); //l'entrée dans le gmlog...
	if(unit->m_spawn == 0) //chek si la creature à une entrée dans la DB
		return false;
	BlueSystemMessage(m_session, "Suppression de la Creature ID %u", unit->spawnid); //echo qui dit que l'on del le npc

	if(unit->IsInWorld()) //si le npc est en jeu
	{
		if(unit->m_spawn) //delete dans la DB? ( Pas necessaire cette condition, tu test au-dessus si l'entré existe)
		{
			uint32 cellx=float2int32(((_maxX-unit->m_spawn->x)/_cellSize));
			uint32 celly=float2int32(((_maxY-unit->m_spawn->y)/_cellSize));
			if(cellx <= _sizeX && celly <= _sizeY)
			{
				CellSpawns * c = unit->GetMapMgr()->GetBaseMap()->GetSpawnsListAndCreate(cellx, celly);
				for(CreatureSpawnList::iterator itr = c->CreatureSpawns.begin(); itr != c->CreatureSpawns.end(); ++itr)
					if((*itr) == unit->m_spawn)
					{
						c->CreatureSpawns.erase(itr);
						break;
					}
				delete unit->m_spawn;
			}
		}
		unit->RemoveFromWorld(false,true); //delete IG du npc
	}
	unit->DeleteFromDB(); //delete DB du npc

	unit->SafeDelete(); // Suppression des entrées lié au pet et aux events eventuels lie au npc

	return true;
}

bool ChatHandler::HandleRespawnCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection(); //on chope la cible du joueur
	if (guid == 0) //on test si il y a quelquechose de ciblé
	{
		SystemMessage(m_session, "Pas de sélection.");
		return true;
	}

	Creature *unit = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!unit) //on test si la cible est un npc
	{
		SystemMessage(m_session, "Vous devez sélectionner une créature.");
		return true;
	}

	if(unit->IsInWorld())
	{
		unit->Despawn(100, 200);
	}

	return true;
}
//------------
bool ChatHandler::HandleMorphCommand(const char* args, WorldSession *m_session)
{
	
	//m_session->GetPlayer()->Morph(displayId);
	// .modify displayid <dispId>
	SystemMessage(m_session, "Commande: .modify displayid <dispId>");
	return true;
}
//------------
bool ChatHandler::HandleDeMorphCommand(const char* args, WorldSession *m_session)
{
	sLog.outError("Demorphed %s",m_session->GetPlayer()->GetName());
	m_session->GetPlayer()->DeMorph();
	return true;
}

bool ChatHandler::HandleItemCommand(const char* args, WorldSession *m_session)
{
	char* next_token;
	char* pitem = strtok_s((char*)args, " ",&next_token);
	if (!pitem)
		return false;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	Creature * pCreature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}

	uint32 item = atoi(pitem);
	int amount = -1;

	char* pamount = strtok_s(NULL, " ",&next_token);
	if (pamount)
		amount = atoi(pamount);

	ItemPrototype* tmpItem = ItemPrototypeStorage.LookupEntry(item);

	std::stringstream sstext;
	if(tmpItem)
	{
		std::stringstream ss;
		ss << "INSERT INTO vendors VALUES ('" << pCreature->GetUInt32Value(OBJECT_FIELD_ENTRY) << "', '" << item << "', '" << amount << "', 0, 0 )" << '\0';
		WorldDatabase.Execute( ss.str().c_str() );

		pCreature->AddVendorItem(item, amount);

		sstext << "Item '" << item << "' '" << tmpItem->Name1 << "' Added to list" << '\0';
	}
	else
	{
		sstext << "Item '" << item << "' Not Found in Database." << '\0';
	}

	sGMLog.writefromsession(m_session, "added item %u to vendor %u", pCreature->GetEntry(), item);
	SystemMessage(m_session,  sstext.str().c_str());

	return true;
}

bool ChatHandler::HandleItemRemoveCommand(const char* args, WorldSession *m_session)
{
	char* next_token;
	char* iguid = strtok_s((char*)args, " ",&next_token);
	if (!iguid)
		return false;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	Creature * pCreature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}

	uint32 itemguid = atoi(iguid);
	int slot = pCreature->GetSlotByItemId(itemguid);

	std::stringstream sstext;
	if(slot != -1)
	{
		uint32 guidlow = GUID_LOPART(guid);

		std::stringstream ss;
		ss << "DELETE FROM vendors WHERE entry = " << guidlow << " AND item = " << itemguid << '\0';
		WorldDatabase.Execute( ss.str().c_str() );

		pCreature->RemoveVendorItem(itemguid);
		ItemPrototype* tmpItem = ItemPrototypeStorage.LookupEntry(itemguid);
		if(tmpItem)
		{
			sstext << "Item '" << itemguid << "' '" << tmpItem->Name1 << "' Deleted from list" << '\0';
		}
		else
		{
			sstext << "Item '" << itemguid << "' Deleted from list" << '\0';
		}

	}
	else
	{
		sstext << "Item '" << itemguid << "' Not Found in List." << '\0';
	}

	SystemMessage(m_session, sstext.str().c_str());

	return true;
}

bool ChatHandler::HandleNPCFlagCommand(const char* args, WorldSession *m_session)
{
	if (!*args)
		return false;

	uint32 npcFlags = (uint32) atoi((char*)args);

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	Creature * pCreature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}

	pCreature->SetUInt32Value(UNIT_NPC_FLAGS , npcFlags);
	pCreature->SaveToDB();
	SystemMessage(m_session, "Value saved, you may need to rejoin or clean your client cache.");

	return true;
}

bool ChatHandler::HandleSaveAllCommand(const char *args, WorldSession *m_session)
{
	PlayerStorageMap::const_iterator itr;
	uint32 stime = now();
	uint32 count = 0;
	objmgr._playerslock.AcquireReadLock();
	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		if(itr->second->GetSession())
		{
			itr->second->SaveToDB(false);
			count++;
		}
	}
	objmgr._playerslock.ReleaseReadLock();
	char msg[100];
	snprintf(msg, 100, "Sauvegarde de %d joueur(s) en Ligne en %d msec.", (int)count, int((uint32)now() - stime));
	sWorld.SendWorldText(msg);
	sWorld.SendWorldWideScreenText(msg);
	sGMLog.writefromsession(m_session, "saved all players");
	//sWorld.SendIRCMessage(msg);
	return true;
}

bool ChatHandler::HandleKillCommand(const char *args, WorldSession *m_session)
{
	Unit * target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(target == 0)
	{
		RedSystemMessage(m_session, "Une Selection valide est requise.");
		return true;
	}

	switch(target->GetTypeId())
	{
	case TYPEID_PLAYER:
		sGMLog.writefromsession(m_session, "Commande Kill sur %s", static_cast< Player* >( target )->GetName() );
		break;

	case TYPEID_UNIT:
		sGMLog.writefromsession(m_session, "Commande Kill sur %s", static_cast< Creature* >( target )->GetCreatureName() ? static_cast< Creature* >( target )->GetCreatureName()->Name : "unknown");
		break;
	}
	

	// If we're killing a player, send a message indicating a gm killed them.
	if(target->IsPlayer())
	{
		Player * plr = static_cast< Player* >(target);
		m_session->GetPlayer()->DealDamage(plr, plr->GetUInt32Value(UNIT_FIELD_HEALTH),0,0,0);
		//plr->SetUInt32Value(UNIT_FIELD_HEALTH, 0);
		plr->KillPlayer();
		BlueSystemMessageToPlr(plr, "%s vous a tue avec une commande MJ.", m_session->GetPlayer()->GetName());
	}
	else
	{

		// Cast insta-kill.
		SpellEntry * se = dbcSpell.LookupEntry(__Death_Touch); // 5
		if(se == 0) return false;

		SpellCastTargets targets(target);
		Spell * sp = new Spell(m_session->GetPlayer(), se, true, 0);
		sp->prepare(&targets);

/*		SpellEntry * se = dbcSpell.LookupEntry(20479);
		if(se == 0) return false;
		
		SpellCastTargets targets(target->GetGUID());
		Spell * sp = new Spell(target, se, true, 0);
		sp->prepare(&targets);*/
	}

	return true;
}

bool ChatHandler::HandleKillByPlrCommand( const char *args , WorldSession *m_session )
{
	Player *plr = objmgr.GetPlayer(args, false);
	if(!plr)
	{
		RedSystemMessage(m_session, "Player %s is not online or does not exist.", args);
		return true;
	}

	if(plr->isDead())
	{
		RedSystemMessage(m_session, "Player %s is already dead.", args);
	} else {
		plr->SetUInt32Value(UNIT_FIELD_HEALTH, 0); // Die, insect
		plr->KillPlayer();
		BlueSystemMessageToPlr(plr, "You were killed by %s with a GM command.", m_session->GetPlayer()->GetName());
		GreenSystemMessage(m_session, "Killed player %s.", args);
		sGMLog.writefromsession(m_session, "remote killed "I64FMT" (Name: %s)", plr->GetGUID(), plr->GetNameString() );

	}
	return true;
}

bool ChatHandler::HandleCastSpellCommand(const char* args, WorldSession *m_session)
{
	Unit *caster = m_session->GetPlayer();
	Unit *target = getSelectedChar(m_session, false);
	if(!target)
		target = getSelectedCreature(m_session, false);
	if(!target)
	{
		RedSystemMessage(m_session, "Vous devez selectionnez une character ou une creature.");
		return false;
	}

	uint32 spellid = atol(args);
	SpellEntry *spellentry = dbcSpell.LookupEntry(spellid);
	if(!spellentry)
	{
		RedSystemMessage(m_session, "Invalid spell id!");
		return false;
	}
	
	Spell *sp = new Spell(caster, spellentry, false, NULL);
	if(!sp)
	{
		RedSystemMessage(m_session, "Spell failed creation!");
		delete sp;
		return false;
	}

	BlueSystemMessage(m_session, "Casting spell %d on target.", spellid);
	SpellCastTargets targets;
	targets.m_target = target;
	sp->prepare(&targets);
	return true;
}

bool ChatHandler::HandleCastSpellNECommand(const char* args, WorldSession *m_session)
{
	Unit *caster = m_session->GetPlayer();
	Unit *target = getSelectedChar(m_session, false);
	if(!target)
		target = getSelectedCreature(m_session, false);
	if(!target)
	{
		RedSystemMessage(m_session, "Vous devez selectionnez une character ou une creature.");
		return false;
	}

	uint32 spellId = atol(args);
	SpellEntry *spellentry = dbcSpell.LookupEntry(spellId);
	if(!spellentry)
	{
		RedSystemMessage(m_session, "Invalid spell id!");
		return false;
	}
	BlueSystemMessage(m_session, "Casting spell %d on target.", spellId);

	WorldPacket data;

	data.Initialize( SMSG_SPELL_START );
	data << caster->GetNewGUID();
	data << caster->GetNewGUID();
	data << spellId;
	data << uint8(0);
	data << uint16(0);
	data << uint32(0);
	data << uint16(2);
	data << target->GetGUID();
	//		WPAssert(data.size() == 36);
	m_session->SendPacket( &data );

	data.Initialize( SMSG_SPELL_GO );
	data << caster->GetNewGUID();
	data << caster->GetNewGUID();
	data << spellId;
	data << uint8(0) << uint8(1) << uint8(1);
	data << target->GetGUID();
	data << uint8(0);
	data << uint16(2);
	data << target->GetGUID();
	//		WPAssert(data.size() == 42);
	m_session->SendPacket( &data );
	return true;
}

bool ChatHandler::HandleMonsterSayCommand(const char* args, WorldSession *m_session)
{
	Unit *crt = getSelectedCreature(m_session, false);
	if(!crt)
		crt = getSelectedChar(m_session, false);

	if(!crt)
	{
		RedSystemMessage(m_session, "Please select a creature or player before using this command.");
		return true;
	}
	if(crt->GetTypeId() == TYPEID_PLAYER)
	{
		WorldPacket * data = this->FillMessageData(CHAT_MSG_SAY, LANG_UNIVERSAL, args, crt->GetGUID(), 0);
		crt->SendMessageToSet(data, true);
		delete data;
	}
	else
	{
		crt->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, args);
	}

	return true;
}

bool ChatHandler::HandleMonsterYellCommand(const char* args, WorldSession *m_session)
{
	Unit *crt = getSelectedCreature(m_session, false);
	if(!crt)
		crt = getSelectedChar(m_session, false);

	if(!crt)
	{
		RedSystemMessage(m_session, "Please select a creature or player before using this command.");
		return true;
	}
	if(crt->GetTypeId() == TYPEID_PLAYER)
	{
		WorldPacket * data = this->FillMessageData(CHAT_MSG_YELL, LANG_UNIVERSAL, args, crt->GetGUID(), 0);
		crt->SendMessageToSet(data, true);
		delete data;
	}
	else
	{
		crt->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, args);
	}

	return true;
}

bool ChatHandler::HandleGOSelect(const char *args, WorldSession *m_session)
{
	GameObject *GObj = NULL;

	std::set<Object*>::iterator Itr = m_session->GetPlayer()->GetInRangeSetBegin();
	std::set<Object*>::iterator Itr2 = m_session->GetPlayer()->GetInRangeSetEnd();
	float cDist = 9999.0f;
	float nDist = 0.0f;
	bool bUseNext = false;

	if(args)
	{
		if(args[0] == '1')
		{
			if(m_session->GetPlayer()->m_GM_SelectedGO == NULL)
				bUseNext = true;

			for(;;Itr++)
			{
				if(Itr == Itr2 && GObj == NULL && bUseNext)
					Itr = m_session->GetPlayer()->GetInRangeSetBegin();
				else if(Itr == Itr2)
					break;

				if((*Itr)->GetTypeId() == TYPEID_GAMEOBJECT)
				{
					// Find the current go, move to the next one
					if(bUseNext)
					{
						// Select the first.
						GObj = static_cast<GameObject*>(*Itr);
						break;
					} else {
						if(((*Itr) == m_session->GetPlayer()->m_GM_SelectedGO))
						{
							// Found him. Move to the next one, or beginning if we're at the end
							bUseNext = true;
						}
					}
				}
			}
		}
	}
	if(!GObj)
	{
		for( ; Itr != Itr2; Itr++ )
		{
			if( (*Itr)->GetTypeId() == TYPEID_GAMEOBJECT )
			{
				if( (nDist = m_session->GetPlayer()->CalcDistance( *Itr )) < cDist )
				{
					cDist = nDist;
					nDist = 0.0f;
					GObj = (GameObject*)(*Itr);
				}
			}
		}
	}


	if( GObj == NULL )
	{
		RedSystemMessage(m_session, "No inrange GameObject found.");
		return true;
	}

	m_session->GetPlayer()->m_GM_SelectedGO = GObj;

	GreenSystemMessage(m_session, "Selected GameObject [ %s ] which is %.3f meters away from you.",
		GameObjectNameStorage.LookupEntry(GObj->GetEntry())->Name, m_session->GetPlayer()->CalcDistance(GObj));

	return true;
}

bool ChatHandler::HandleGODelete(const char *args, WorldSession *m_session)
{
   uint32 DeleteDB; 
   int _Argv;

    _Argv = sscanf(args, "%u",&DeleteDB);

	if(!_Argv) DeleteDB = 1;

	GameObject *GObj = m_session->GetPlayer()->m_GM_SelectedGO;
	if( !GObj )
	{
		RedSystemMessage(m_session, "Pas de GameObject Selectionne...");
		return true;
	}
	
	if(GObj->m_spawn != 0 && GObj->m_spawn->entry == GObj->GetEntry())
	{
		uint32 cellx=float2int32(((_maxX-GObj->m_spawn->x)/_cellSize));
		uint32 celly=float2int32(((_maxY-GObj->m_spawn->y)/_cellSize));

		if(cellx < _sizeX && celly < _sizeY)
		{
			//m_session->GetPlayer()->GetMapMgr()->GetBaseMap()->GetSpawnsListAndCreate(cellx,celly)->GOSpawns.erase(GObj->m_spawn);
			CellSpawns * c = GObj->GetMapMgr()->GetBaseMap()->GetSpawnsListAndCreate(cellx, celly);
			for(GOSpawnList::iterator itr = c->GOSpawns.begin(); itr != c->GOSpawns.end(); ++itr)
				if((*itr) == GObj->m_spawn)
				{
					c->GOSpawns.erase(itr);
					break;
				}
			if(DeleteDB) GObj->DeleteFromDB();
			GObj->m_spawn = 0;
			delete GObj->m_spawn;
		}
	}
	GObj->Despawn(0);
	GObj->Delete();

	m_session->GetPlayer()->m_GM_SelectedGO = NULL;

	if(DeleteDB) GreenSystemMessage(m_session, "GameObject supprime de la base");
	else         GreenSystemMessage(m_session, "GameObject conserve dans la base");

	return true;
}
//---------------------
bool ChatHandler::HandleGODisplay(const char *args, WorldSession *m_session)
{
	std::stringstream sstext;

    uint32 NewDisplay = 0;
	char* next_token;
    char* pNewDisplay = strtok_s((char*)args, " ",&next_token);
	if (pNewDisplay) NewDisplay = atoi(pNewDisplay);

	if(!NewDisplay) return(false);

	GameObject *GObj = NULL;

	GObj = m_session->GetPlayer()->m_GM_SelectedGO;
	if( !GObj )
	{
		RedSystemMessage(m_session, "Pas de GameObject Selectionne...");
		return true;
	}

	if(!GObj->HasFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_DESPAWN))
	{
	 GObj->SetUInt32Value(GAMEOBJECT_FLAGS, GO_FLAG_NO_DESPAWN);
	 GreenSystemMessage(m_session,"Flag mis a GO_FLAG_NO_DESPAWN");
	}

	GObj->SetUInt32Value(GAMEOBJECT_DISPLAYID, NewDisplay);

	return true;
}
//---------------------
bool ChatHandler::HandleGODamage(const char *args, WorldSession *m_session)
{
	std::stringstream sstext;

    int32 gDamage = 0;
	char* next_token;
    char* pDamage = strtok_s((char*)args, " ",&next_token);
	if (pDamage) gDamage = atoi(pDamage);

	GameObject *GObj = NULL;

	GObj = m_session->GetPlayer()->m_GM_SelectedGO;
	if( !GObj )
	{
		RedSystemMessage(m_session, "Pas de GameObject Selectionne...");
		return true;
	}

    if(gDamage == 0) 
	{
		GreenSystemMessage(m_session,"Health: %u / %u",GObj->Health,GObj->HealthMax);
		return(true);
	}

	if(gDamage == -1) 
	{
		GreenSystemMessage(m_session,"Rebuild Go");
		GObj->Rebuild(); 
		return(true);
	}

	if(!GObj->HasFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_DESPAWN))
	{
	 GObj->SetUInt32Value(GAMEOBJECT_FLAGS, GO_FLAG_NO_DESPAWN);
	 GreenSystemMessage(m_session,"Flag mis a GO_FLAG_NO_DESPAWN");
	}

	GObj->TakeDamage(gDamage);
	GreenSystemMessage(m_session,"Health Go: %u / %u",GObj->Health,GObj->HealthMax);

	return true;
}
//---------------------
bool ChatHandler::HandleGOSpawn(const char *args, WorldSession *m_session)
{
	std::stringstream sstext;

	char* next_token;
	char* pEntryID = strtok_s((char*)args, " ",&next_token);

	if (!pEntryID) return false;

	uint32 EntryID  = atoi(pEntryID);

	bool Save = false;
    char* pSave = strtok_s(NULL, " ",&next_token);

	if (pSave) Save = (atoi(pSave)>0?true:false);

	GameObjectInfo* goi = GameObjectNameStorage.LookupEntry(EntryID);
	if(!goi)
	{
		sstext << "GameObject Info '" << EntryID << "' Not Found" << '\0';
		SystemMessage(m_session, sstext.str().c_str());
		return true;
	}

	sLog.outDebug("Spawning GameObject By Entry '%u'", EntryID);
	sstext << "Spawning GameObject By Entry '" << EntryID << "'" << '\0';
	SystemMessage(m_session, sstext.str().c_str());

	GameObject *go = m_session->GetPlayer()->GetMapMgr()->CreateGameObject(EntryID);
	
	Player *chr = m_session->GetPlayer();
	uint32 mapid = chr->GetMapId();
	float x = chr->GetPositionX();
	float y = chr->GetPositionY();
	float z = chr->GetPositionZ();
	float o = chr->GetOrientation();

	go->SetInstanceID(chr->GetInstanceID());
	go->CreateFromProto(EntryID,mapid,x,y,z,o);
	
	/* fuck blizz coordinate system */
	//go->SetFloatValue(GAMEOBJECT_PARENTROTATION_2, sinf(o / 2));
	//go->SetFloatValue(GAMEOBJECT_PARENTROTATION_3, cosf(o / 2));
	go->PushToWorld(m_session->GetPlayer()->GetMapMgr());

	// Create sapwn instance
	GOSpawn * gs = new GOSpawn;
	gs->entry = go->GetEntry();
	gs->facing = go->GetOrientation();
	gs->faction = go->GetUInt32Value(GAMEOBJECT_FACTION);

    // Debug des Go destructible (normalement c la base qui gere ca)
	if(goi->Type == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING)
	{
		if(!go->HasFlag(GAMEOBJECT_FLAGS,GO_FLAG_NO_DESPAWN))
		{
			GreenSystemMessage(m_session,"%u: Le Go est Destructible,le flag devrait comporter 0x%08X (Flag: 0x%08X)", 
				goi->ID,GO_FLAG_NO_DESPAWN, go->GetUInt32Value(GAMEOBJECT_FLAGS));

			go->SetFlag(GAMEOBJECT_FLAGS,GO_FLAG_NO_DESPAWN);
		}
	}
	//-----------

	gs->flags = go->GetUInt32Value(GAMEOBJECT_FLAGS);
	gs->id = objmgr.GenerateGameObjectSpawnID();
	//gs->o = go->GetUInt64Value(GAMEOBJECT_PARENTROTATION);
	//gs->o1 = go->GetFloatValue(GAMEOBJECT_PARENTROTATION_1); //GAMEOBJECT_PARENTROTATION);
	//gs->o2 = go->GetFloatValue(GAMEOBJECT_PARENTROTATION_2);
	//gs->o3 = go->GetFloatValue(GAMEOBJECT_PARENTROTATION_3);
	gs->scale = go->GetFloatValue(OBJECT_FIELD_SCALE_X);
	gs->x = go->GetPositionX();
	gs->y = go->GetPositionY();
	gs->z = go->GetPositionZ();
	gs->state = go->GetByte(GAMEOBJECT_BYTES_1, 0);
	//gs->stateNpcLink = 0;

	uint32 cx = m_session->GetPlayer()->GetMapMgr()->GetPosX(m_session->GetPlayer()->GetPositionX());
	uint32 cy = m_session->GetPlayer()->GetMapMgr()->GetPosY(m_session->GetPlayer()->GetPositionY());

	m_session->GetPlayer()->GetMapMgr()->GetBaseMap()->GetSpawnsListAndCreate(cx,cy)->GOSpawns.push_back(gs);
	go->m_spawn = gs;

	//go->AddToWorld();

	if(Save == true)
	{
		// If we're saving, create template and add index
		go->SaveToDB();
	}
	return true;
}
//------------------------
bool ChatHandler::HandleGOInfo(const char *args, WorldSession *m_session)
{
	std::string name;
	GameObjectInfo *GOInfo = NULL;
	GameObject *GObj = NULL;

	GObj = m_session->GetPlayer()->m_GM_SelectedGO;
	if( !GObj )
	{
		RedSystemMessage(m_session, "Pas de GameObject Selectionne...");
		return true;
	}

	//switch( GObj->GetUInt32Value(GAMEOBJECT_TYPE_ID) )
	switch( GObj->GetByte(GAMEOBJECT_BYTES_1, 1) )
	{
	case GAMEOBJECT_TYPE_DOOR:		  name = "Door";	break;
	case GAMEOBJECT_TYPE_BUTTON:		name = "Button";	break;
	case GAMEOBJECT_TYPE_QUESTGIVER:	name = "Quest Giver";	break;
	case GAMEOBJECT_TYPE_CHEST:		 name = "Chest";	break;
	case GAMEOBJECT_TYPE_BINDER:		name = "Binder";	break;
	case GAMEOBJECT_TYPE_GENERIC:	   name = "Generic";	break;
	case GAMEOBJECT_TYPE_TRAP:		  name = "Trap";	break;
	case GAMEOBJECT_TYPE_CHAIR:		 name = "Chair";	break;
	case GAMEOBJECT_TYPE_SPELL_FOCUS:   name = "Spell Focus";	break;
	case GAMEOBJECT_TYPE_TEXT:		  name = "Text";	break;
	case GAMEOBJECT_TYPE_GOOBER:		name = "Goober";	break;
	case GAMEOBJECT_TYPE_TRANSPORT:	 name = "Transport";	break;
	case GAMEOBJECT_TYPE_AREADAMAGE:	name = "Area Damage";	break;
	case GAMEOBJECT_TYPE_CAMERA:		name = "Camera";	break;
	case GAMEOBJECT_TYPE_MAP_OBJECT:	name = "Map Object";	break;
	case GAMEOBJECT_TYPE_MO_TRANSPORT:  name = "Mo Transport";	break;
	case GAMEOBJECT_TYPE_DUEL_ARBITER:  name = "Duel Arbiter";	break;
	case GAMEOBJECT_TYPE_FISHINGNODE:   name = "Fishing Node";	break;
	case GAMEOBJECT_TYPE_RITUAL:		name = "Ritual";	break;
	case GAMEOBJECT_TYPE_MAILBOX:	   name = "Mailbox";	break;
	case GAMEOBJECT_TYPE_AUCTIONHOUSE:  name = "Auction House";	break;
	case GAMEOBJECT_TYPE_GUARDPOST:	 name = "Guard Post";	break;
	case GAMEOBJECT_TYPE_SPELLCASTER:  name = "Spell Caster";	break;
	case GAMEOBJECT_TYPE_MEETINGSTONE:  name = "Meeting Stone";	break;
	case GAMEOBJECT_TYPE_FLAGSTAND:	 name = "Flag Stand";	break;
	case GAMEOBJECT_TYPE_FISHINGHOLE:   name = "Fishing Hole";	break;
	case GAMEOBJECT_TYPE_FLAGDROP:	  name = "Flag Drop";	break;
	case GAMEOBJECT_TYPE_MINI_GAME:	        name = "Mini Game";	break;
	case GAMEOBJECT_TYPE_LOTTERY_KIOSK:	    name = "Loterie";	break;
	case GAMEOBJECT_TYPE_CAPTURE_POINT:	    name = "Point de capture";	break;
	case GAMEOBJECT_TYPE_AURA_GENERATOR:	name = "Aura generator";	break;
	case GAMEOBJECT_TYPE_DUNGEON_DIFFICULTY: name = "Dongeon Difficulty";	break;
	case GAMEOBJECT_TYPE_BARBER_CHAIR:	    name = "Chaise de barbier";	break;
	case GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING:	name = "Destructible";	break;
	case GAMEOBJECT_TYPE_GUILD_BANK:	    name = "Bank de guilde";	break;
	case GAMEOBJECT_TYPE_TRAPDOOR:	        name = "Trap Door";	break;	
	case GAMEOBJECT_TYPE_SPECIALGO:	        name = "Special";	break;
	default:							name = "Unknown type. WTF?";	break; //OMG?! This should NEVER happen o.O
 	}
	
    

	GOInfo = GameObjectNameStorage.LookupEntry(GObj->GetEntry());
	if( !GOInfo )
	{
		RedSystemMessage(m_session, "This GameObject doesn't have template, you won't be able to get some informations nor to spawn a GO with this entry.");
		return true;
	}
	else
	{
	BlueSystemMessage(m_session,"Informations:");
	GreenSystemMessage(m_session,"Entry   :%s %d",MSG_COLOR_LIGHTBLUE, GObj->GetEntry());
	GreenSystemMessage(m_session,"Model   :%s %d",MSG_COLOR_LIGHTBLUE, GObj->GetUInt32Value(GAMEOBJECT_DISPLAYID));
	GreenSystemMessage(m_session,"State   :%s %d",MSG_COLOR_LIGHTBLUE, GObj->GetByte(GAMEOBJECT_BYTES_1, 0));

	GreenSystemMessage(m_session,"Flags   :%s %d",MSG_COLOR_LIGHTBLUE, GObj->GetUInt32Value(GAMEOBJECT_FLAGS));

	GreenSystemMessage(m_session,"DynFlags:%s %d",MSG_COLOR_LIGHTBLUE, GObj->GetUInt32Value(GAMEOBJECT_DYNAMIC));
	GreenSystemMessage(m_session,"Faction :%s %d",MSG_COLOR_LIGHTBLUE, GObj->GetUInt32Value(GAMEOBJECT_FACTION));
	GreenSystemMessage(m_session,"Type  :%s %d - %s",MSG_COLOR_LIGHTBLUE, GObj->GetByte(GAMEOBJECT_BYTES_1, 1),name.c_str());
	GreenSystemMessage(m_session," "); //force whitespace line :/
	GreenSystemMessage(m_session,"Distance:%s %d metres",MSG_COLOR_LIGHTBLUE, uint32(GObj->CalcDistance((Object*)m_session->GetPlayer()))); //brackets much :D
	GreenSystemMessage(m_session,"Name    :%s %s",MSG_COLOR_LIGHTBLUE, GOInfo->Name);
	GreenSystemMessage(m_session,"Size    :%s %f",MSG_COLOR_LIGHTBLUE, GObj->GetFloatValue(OBJECT_FIELD_SCALE_X));
    //GreenSystemMessage(m_session," "); //force whitespace line :/
//	GreenSystemMessage(m_session,"Rotation        :%s %f",MSG_COLOR_LIGHTBLUE, GObj->GetFloatValue(GAMEOBJECT_ROTATION));
	GreenSystemMessage(m_session,"Parent Rotation :%s %f",MSG_COLOR_LIGHTBLUE, GObj->GetFloatValue(GAMEOBJECT_PARENTROTATION));
	GreenSystemMessage(m_session,"Parent Rotation1:%s %f",MSG_COLOR_LIGHTBLUE, GObj->GetFloatValue(GAMEOBJECT_PARENTROTATION_1));
	GreenSystemMessage(m_session,"Parent Rotation2:%s %f",MSG_COLOR_LIGHTBLUE, GObj->GetFloatValue(GAMEOBJECT_PARENTROTATION_2));
	GreenSystemMessage(m_session,"Parent Rotation3:%s %f",MSG_COLOR_LIGHTBLUE, GObj->GetFloatValue(GAMEOBJECT_PARENTROTATION_3));
	}
	return true;
}

bool ChatHandler::HandleGOEnable(const char *args, WorldSession *m_session)
{
	GameObject *GObj = NULL;

	GObj = m_session->GetPlayer()->m_GM_SelectedGO;
	if( !GObj )
	{
		RedSystemMessage(m_session, "Pas de GameObject Selectionne...");
		return true;
	}
	if(GObj->GetUInt32Value(GAMEOBJECT_DYNAMIC) == 1)
	{
		// Deactivate
		GObj->SetUInt32Value(GAMEOBJECT_DYNAMIC, 0);
		BlueSystemMessage(m_session, "Gameobject activated.");
	} else {
		// /Activate
		GObj->SetUInt32Value(GAMEOBJECT_DYNAMIC, 1);
		BlueSystemMessage(m_session, "Gameobject deactivated.");
	}
	
	return true;
}

bool ChatHandler::HandleGOActivate(const char* args, WorldSession *m_session)
{
	GameObject *GObj = NULL;

	GObj = m_session->GetPlayer()->m_GM_SelectedGO;
	if( !GObj )
	{
		RedSystemMessage(m_session, "Pas de GameObject Selectionne...");
		return true;
	}
	if(GObj->GetByte(GAMEOBJECT_BYTES_1, 0) & 0x01) // Si actif, on deactive
	{
		// Close/Deactivate
		GObj->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
		//BlueSystemMessage(m_session, "Gameobject Ferme/Deactive.");
	} 
	else // si pas actif, on active
	{
		// Open/Activate
		GObj->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
	    //BlueSystemMessage(m_session, "Gameobject Ouvert/Actif." );
	}
	BlueSystemMessage(m_session, "Gameobject opened/closed.");
	return true;
}

bool ChatHandler::HandleGOScale(const char* args, WorldSession* m_session)
{
    uint32 RecDB = 0;
	float NewScale;
    int _Argv;

    _Argv = sscanf(args, "%f %u",&NewScale, &RecDB);

	if(!_Argv)
	{
		RedSystemMessage(m_session, "Usage: .gobject scale <float> [0:1]");
		return false;
	}

    if(_Argv <=2) RecDB = 0;

	GameObject *go = m_session->GetPlayer()->m_GM_SelectedGO;
	if( !go )
	{
		RedSystemMessage(m_session, "Pas de GameObject Selectionne...");
		return true;
	}
	
    
	if(!NewScale) NewScale = 1.0;
	go->SetFloatValue(OBJECT_FIELD_SCALE_X, NewScale);

	// despawn and respawn
    uint32 NewGuid = m_session->GetPlayer()->GetMapMgr()->GenerateGameobjectGuid();
	go->RemoveFromWorld(true);
	go->SetNewGuid(NewGuid);
	if(RecDB != 0) 
	{
		go->SaveToDB(); // Apres le NewGuid ??
		RedSystemMessage(m_session, "Nouvelle taille sauvegarde: %.3f", NewScale);
	}
	else RedSystemMessage(m_session, "Nouvelle taille non sauvegarde: %.3f", NewScale);

	go->PushToWorld(m_session->GetPlayer()->GetMapMgr());

	m_session->GetPlayer()->m_GM_SelectedGO = go;
	return true;
}

bool ChatHandler::HandleReviveStringcommand(const char* args, WorldSession* m_session)
{
	Player *plr = objmgr.GetPlayer(args, false);
	if(!plr)
	{
		RedSystemMessage(m_session, "Impossible de trouver le joueur %s.", args);
		return true;
	}

	if(plr->isDead())
	{
		if(plr->GetInstanceID() == m_session->GetPlayer()->GetInstanceID())
			plr->RemoteRevive();
		else
			sEventMgr.AddEvent(plr, &Player::RemoteRevive, EVENT_PLAYER_REST, 1, 1,0);

		GreenSystemMessage(m_session, "Refaire vivre le joueur %s.", args);
	} else {
		GreenSystemMessage(m_session, "Le joueur %s n'est pas mort.", args);
	}
	return true;
}

bool ChatHandler::HandleMountCommand(const char *args, WorldSession *m_session)
{
	if(!args)
	{
		RedSystemMessage(m_session, "Aucun modele specifie");
		return true;
	}
	uint32 modelid = atol(args);
	if(!modelid)
	{
		RedSystemMessage(m_session, "Aucun modele specifie");
		return true;
	}

	Unit *m_target = NULL;
	Player *m_plyr = getSelectedChar(m_session, false);
	if(m_plyr)
		m_target = m_plyr;
	else
	{
		Creature *m_crt = getSelectedCreature(m_session, false);
		if(m_crt)
			m_target = m_crt;
	}

	if(!m_target)
	{
		RedSystemMessage(m_session, "No target found.");
		return true;
	}
	
	if(m_target->GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID) != 0)
	{
		RedSystemMessage(m_session, "Target is already mounted.");
		return true;
	}

	m_target->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , modelid);
	//m_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
	
	BlueSystemMessage(m_session, "Now mounted with model %d.", modelid);
	return true;
}

// Nouvelle gestion (Branruz) => .npc AddAiAgent 
bool ChatHandler::HandleAddAIAgentCommand(const char* args, WorldSession *m_session)
{
	uint16 ArgMissing = 0;
	bool RecordBase = false;
	char *agent,*next_token,*procEvent,*procChance,*procCount;
	char *spellId,*spellType,*spelltargetType,*spellCooldown;
	char *floatMisc1,*Misc2,*_RecordBase;


	agent = strtok_s((char*)args, " ",&next_token);
	if(!agent) ArgMissing = 0x0001 ; 

	if(!ArgMissing) 
	{
		procEvent = strtok_s(NULL, " ",&next_token);
	    if(!procEvent) ArgMissing = 0x0002 ;
	}

	if(!ArgMissing) 
	{
		procChance = strtok_s(NULL, " ",&next_token);
	    if(!procChance) ArgMissing = 0x0004 ;
	}

	if(!ArgMissing) 
	{
		procCount = strtok_s(NULL, " ",&next_token);
	    if(!procCount) ArgMissing = 0x0008 ; 
	}

	if(!ArgMissing) 
	{
		spellId = strtok_s(NULL, " ",&next_token);
	    if(!spellId) ArgMissing = 0x0010 ; 
	}

	if(!ArgMissing) 
	{
		spellType = strtok_s(NULL, " ",&next_token);
	    if(!spellType) ArgMissing = 0x0020 ;
	}

	if(!ArgMissing) 
	{
		spelltargetType = strtok_s(NULL, " ",&next_token);
	    if(!spelltargetType) ArgMissing = 0x0040 ;
	}

	if(!ArgMissing) 
	{
		spellCooldown = strtok_s(NULL, " ",&next_token);
	    if(!spellCooldown) ArgMissing = 0x0080 ; 
	}

	if(!ArgMissing) 
	{
		floatMisc1 = strtok_s(NULL, " ",&next_token);
	    if(!floatMisc1) ArgMissing = 0x0100 ; 
	}

	if(!ArgMissing) 
	{
		Misc2 = strtok_s(NULL, " ",&next_token);
	    if(!Misc2) ArgMissing = 0x0200 ; 
	}

	_RecordBase = strtok_s(NULL, " ",&next_token);
	if(_RecordBase)
	{
     if(atoi(_RecordBase)) RecordBase = true;
	 else                  RecordBase = false;
	}

	if(ArgMissing)
	{                             //   0001    0002    0004      0008     0010      0020         0040       0080        0100       0200
//		GreenSystemMessagem_session, "<Type> <Event> <Chance> <MaxCount> <Spell> <SpellType> <TargetType> <CoolDown> <FloatMisc1> <Misc2> [Record : <0/1>]");
		return(false);
	}

    if((atoi(agent) >= AI_MAX_AGENT) || (atoi(agent)==0) )
	{
		RedSystemMessage(m_session, "Type d'agent disponible: AGENT_MELEE=1, AGENT_RANGED=2, AGENT_FLEE=3, AGENT_SPELL=4, AGENT_CALLFORHELP=5");
		return(true);
	}

	if(atoi(procEvent) >= AI_MAX_EVENT)
	{
		RedSystemMessage(m_session, "Type d'Event disponible: EVENT_ENTERCOMBAT    =  0");
		RedSystemMessage(m_session, "                         EVENT_LEAVECOMBAT    =  1");
		RedSystemMessage(m_session, "                         EVENT_DAMAGEDEALT    =  2");
		RedSystemMessage(m_session, "                         EVENT_DAMAGETAKEN    =  3");
		RedSystemMessage(m_session, "                         EVENT_FEAR           =  4");
		RedSystemMessage(m_session, "                         EVENT_UNFEAR         =  5");
		RedSystemMessage(m_session, "                         EVENT_FOLLOWOWNER    =  6");
		RedSystemMessage(m_session, "                         EVENT_WANDER         =  7");
		RedSystemMessage(m_session, "                         EVENT_UNWANDER       =  8");
		RedSystemMessage(m_session, "                         EVENT_UNITDIED       =  9");
		RedSystemMessage(m_session, "                         EVENT_POSSESS        = 10");
		RedSystemMessage(m_session, "                         EVENT_UNPOSSESS      = 11");
		RedSystemMessage(m_session, "                         EVENT_HASPOSSESSED   = 12");
		RedSystemMessage(m_session, "                         EVENT_HASUNPOSSESSED = 13");
		return(true);
	}

	if(atoi(procChance) > 100) // 100% max (on va le dire comme ca)
	{
      RedSystemMessage(m_session, "Chance > à 100%");
	  return(true);
	}

	if(!dbcSpell.LookupEntry(atoi(spellId)))
	{
        RedSystemMessage(m_session, "Le spell %d n'existe pas!",atoi(spellId));
		return(true);
	}

	if(atoi(spellType) >= AI_MAX_SPELLTYPE)
	{
		RedSystemMessage(m_session, "SpellType disponible: STYPE_NULL      =  0");
		RedSystemMessage(m_session, "                      STYPE_ROOT      =  1");
		RedSystemMessage(m_session, "                      STYPE_HEAL      =  2");
		RedSystemMessage(m_session, "                      STYPE_STUN      =  3");
		RedSystemMessage(m_session, "                      STYPE_FEAR      =  4");
		RedSystemMessage(m_session, "                      STYPE_SILENCE   =  5");
		RedSystemMessage(m_session, "                      STYPE_CURSE     =  6");
		RedSystemMessage(m_session, "                      STYPE_AOEDAMAGE =  7");
		RedSystemMessage(m_session, "                      STYPE_DAMAGE    =  8");
		RedSystemMessage(m_session, "                      STYPE_SUMMON    =  9");
		RedSystemMessage(m_session, "                      STYPE_BUFF      = 10");
		RedSystemMessage(m_session, "                      STYPE_DEBUFF    = 11");
        return(true);
	}

	if(atoi(spelltargetType) >= AI_MAX_TTYPE)
	{
	 RedSystemMessage(m_session, "TargetType disponible: TTYPE_NULL         =  0");
	 RedSystemMessage(m_session, "                       TTYPE_SINGLETARGET =  1");
	 RedSystemMessage(m_session, "                       TTYPE_DESTINATION  =  2");
	 RedSystemMessage(m_session, "                       TTYPE_SOURCE       =  3");
	 RedSystemMessage(m_session, "                       TTYPE_CASTER       =  4");
	 RedSystemMessage(m_session, "                       TTYPE_OWNER        =  5");  
	 return(true);
	}
	
	if(atoi(spellCooldown) < 5000)
	{
      RedSystemMessage(m_session, "Le CoolDown ne doit pas etre inferieur à 5000 (5 Sec)");
	  return(true);
	}
    

    // ****** Memo du nouvel AI Agent
	Unit* target = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(m_session->GetPlayer()->GetSelection()));
	if(!target)
	{
		RedSystemMessage(m_session, "Vous devez selectionner un PNJ/Creature!");
		return(false);
	}

	if(RecordBase)
	{
	 std::stringstream qry;
	 qry << "INSERT INTO ai_agents set";
	 qry << " entry = '" << target->GetUInt32Value(OBJECT_FIELD_ENTRY) << "',";
	 qry << " type = '" << atoi(agent)  << "',";
	 qry << " event = '" << atoi(procEvent) << "',";
	 qry << " chance = '" << atoi(procChance) << "',";
	 qry << " maxcount = '" << 0 << "',"; // atoi(procCount) << "',"; // Debug, neutralisé pour l'instant (Branruz)
	 qry << " spell = '" << atoi(spellId) << "',";
	 qry << " spelltype = '" << atoi(spellType) << "',";
	 qry << " targettype = '" << atoi(spelltargetType) << "',";
	 qry << " cooldown = '" << atoi(spellCooldown) << "',";
	 qry << " floatMisc1 = '" << atof(floatMisc1) << "',";
     qry << " Misc2  ='" << atoi(Misc2)<< "';";
    
	 if(!WorldDatabase.Execute( qry.str().c_str( ) ) )
	 {
        RedSystemMessage(m_session, "L'AI Agent n'est pas enregistré dans la base suite a un probleme SQL!");
		RecordBase = false;
	 }
	}

	AI_Spell * sp = new AI_Spell;
	sp->agent        = atoi(agent);
	sp->event        = atoi(procEvent);
	sp->procChance   = atoi(procChance);
	sp->procCount    = atoi(procCount);
	sp->spell        = dbcSpell.LookupEntry(atoi(spellId));
	sp->spellType    = atoi(spellType);
	sp->spelltargetType = atoi(spelltargetType);
	sp->floatMisc1   = (float)atof(floatMisc1);
	sp->Misc2        = (uint32)atof(Misc2);
	sp->cooldown     = (uint32)atoi(spellCooldown);
	sp->procCount    = 0;
	sp->procCounter  = 0;
	sp->cooldowntime = 0;
	sp->custom_pointer = false;
	sp->minrange = GetMinRange(dbcSpellRange.LookupEntry(dbcSpell.LookupEntry(atoi(spellId))->rangeIndex));
	sp->maxrange = GetMaxRange(dbcSpellRange.LookupEntry(dbcSpell.LookupEntry(atoi(spellId))->rangeIndex));

	// AGENT_MELEE=1, AGENT_RANGED=2, AGENT_FLEE=3, AGENT_SPELL=4, AGENT_CALLFORHELP=5
	if(sp->agent == AGENT_CALLFORHELP) target->GetAIInterface()->m_canCallForHelp = true;
	else if(sp->agent == AGENT_FLEE)   target->GetAIInterface()->m_canFlee = true;
	else if(sp->agent == AGENT_RANGED) target->GetAIInterface()->m_canRangedAttack = true;
	else                               target->GetAIInterface()->addSpellToList(sp);

	if(RecordBase) GreenSystemMessage(m_session, "Ai Agent ajouté et sauvegardé avec succes!");
	else           GreenSystemMessage(m_session, "Ai Agent ajouté avec succes! (Pas de sauvegarde)");
	return(true);
}

// Liste les AIAgents pour le pnj selectionné (Lecture dans la base)
bool ChatHandler::HandleListAIAgentBaseCommand(const char* args, WorldSession *m_session)
{
	Unit* target = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(m_session->GetPlayer()->GetSelection()));
	if(!target)
	{
		RedSystemMessage(m_session, "Vous devez selectionner une Creature!");
		return false;
	}

	if(target->GetObjectName()!=NULL) GreenSystemMessage(m_session,"List des AIAgents de %s",target->GetObjectName());
	else                              GreenSystemMessage(m_session,"List des AIAgents de %u",target->GetEntry());


	std::stringstream ss;
	ss << "SELECT * FROM ai_agents where entry=" << target->GetUInt32Value(OBJECT_FIELD_ENTRY);
	QueryResult *result = WorldDatabase.Query( ss.str().c_str() );

	if( !result )
		return false;

	do
	{
		Field *fields = result->Fetch();
		GreenSystemMessage(m_session, "Agent: %5u | SpellId: %5u | Event: %5u | Chance: %5u | Count: %5u",
			                     fields[1].GetUInt16(),fields[5].GetUInt32(),fields[2].GetUInt32(),
								 fields[3].GetUInt32(),fields[4].GetUInt32());
	} while( result->NextRow() );

	delete result;

	return true;

}

//-----------------------------------------------------------------------------------------
// List AIAgent que possede le PNJ (Pas lu dans la base mais sur le pnj)
bool ChatHandler::HandleListAIAgentCommand(const char* args, WorldSession *m_session)
{
	bool Gotcha=false;

	Unit* target = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(m_session->GetPlayer()->GetSelection()));
	if(!target)
	{
		RedSystemMessage(m_session, "Vous devez sélectionner une Creature!");
		return false;
	}

	GreenSystemMessage(m_session, "Liste AIAgent de %s (%u)",target->GetObjectName(),target->GetEntry());
	for(list<AI_Spell*>::iterator it = target->GetAIInterface()->m_spells.begin(); it != target->GetAIInterface()->m_spells.end(); ++it)
	{
		Gotcha = true;
		GreenSystemMessage(m_session, "Agent: %5u | SpellId: %5u | Event: %5u | Chance: %5u | Count: %5u",
			                     (*it)->agent,(*it)->spell->Id,(*it)->event,(*it)->procChance,(*it)->procCount);
	}
	if(!Gotcha) GreenSystemMessage(m_session, "Aucun AIAgent trouvé pour ce pnj");
    return true;
}
//-----------------------------------------------------------------------------------------
bool ChatHandler::HandleTargetTypeAIAgentCommand(const char* args, WorldSession *m_session)
{
	uint32 AISpell = 0;
	uint32 targetType = 0;
	bool TargetTypeChange = false;
    int _Argv;

    _Argv = sscanf(args, "%u %u",&AISpell, &targetType);

	if(_Argv <= 0)
	{
		GreenSystemMessage(m_session, "Type disponible:");
		GreenSystemMessage(m_session, " 0 : Aucun");
		GreenSystemMessage(m_session, " 1 : Une seule cible");
		GreenSystemMessage(m_session, " 2 : Destination");
		GreenSystemMessage(m_session, " 3 : Source");
		GreenSystemMessage(m_session, " 4 : Soi-meme");
		GreenSystemMessage(m_session, " 5 : Le familier");
	    // Waad
		GreenSystemMessage(m_session, " 6 : Le dernier qui a tapé");
		GreenSystemMessage(m_session, " 7 : Celui qui menace le plus");
		GreenSystemMessage(m_session, " 8 : Au hasard : Players, PNJ, totems, pets, etc. amicaux pour le PNJ");
		GreenSystemMessage(m_session, " 9 : Au hasard : Players, PNJ, totems, pets, etc. hostiles pour le PNJ");
		GreenSystemMessage(m_session, "10 : Au hasard : Uniquement les players hostiles pour le PNJ");
		GreenSystemMessage(m_session, "11 : Au hasard : Uniquement les players amicaux pour le PNJ");
		GreenSystemMessage(m_session, "12 : Players amicaux les plus proches");
		GreenSystemMessage(m_session, "13 : Players hostiles les plus proches");
		GreenSystemMessage(m_session, "14 : Players, PNJs, totems, pets, etc. amicaux les plus proches");
		GreenSystemMessage(m_session, "15 : Players, PNJ, totems, pets, etc. hostiles les plus proches");
		GreenSystemMessage(m_session, "16 : Au hasard : Players, PNJ, totems, pets, etc. amicaux pour le PNJ, mais pas celui qui génère le plus de menace.");
		GreenSystemMessage(m_session, "17 : Au hasard : Players, PNJ, totems, pets, etc. hostiles pour le PNJ, mais pas celui qui génère le plus de menace.");
		GreenSystemMessage(m_session, "18 : Au hasard : Uniquement les players amicaux pour le PNJ, mais pas celui qui génère le plus de menace.");
		GreenSystemMessage(m_session, "19 : Au hasard : Uniquement les players hostiles pour le PNJ, mais pas celui qui génère le plus de menace.");
		GreenSystemMessage(m_session, "20 : PNJs, totems, pets, etc. amicaux les plus proches, mais pas celui qui génère le plus de menace.");
		GreenSystemMessage(m_session, "21 : Players, PNJ, totems, pets, etc. hostiles les plus proches, mais pas celui qui génère le plus de menace.");
		GreenSystemMessage(m_session, "22 : Au hasard : Players, PNJ, totems, pets, etc. blessés, amicaux pour le PNJ");
		GreenSystemMessage(m_session, "23 : Au hasard : Players, PNJ, totems, pets, etc. blessés, hostiles pour le PNJ");
		GreenSystemMessage(m_session, "24 : Au hasard : Uniquement les players blessés, amicaux pour le PNJ");
		GreenSystemMessage(m_session, "25 : Au hasard : Uniquement les players blessés, hostiles pour le PNJ");
		GreenSystemMessage(m_session, "26 : Le Player qui génère en second, le plus de menace.");
		return(true);
	}

    Unit* target = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(m_session->GetPlayer()->GetSelection()));
	if(!target)
	{
		RedSystemMessage(m_session, "Vous devez sélectionner une Creature!");
		return(false);
	}

	if(targetType > AI_MAX_TTYPE)
	{
		RedSystemMessage(m_session, "Max Type = %u",(AI_MAX_TTYPE-1));
		return(false);
	}

	for(list<AI_Spell*>::iterator it = target->GetAIInterface()->m_spells.begin(); it != target->GetAIInterface()->m_spells.end(); ++it)
	{
		if((*it)->entryId == AISpell)
		{
			(*it)->spelltargetType = targetType;
			TargetTypeChange = true;
		}
	}

	if(TargetTypeChange) GreenSystemMessage(m_session, "TargetType %u de l'AISpell %u a été changé.");
	else                 GreenSystemMessage(m_session, "TargetType %u ou l'AISpell %u n'a pas été trouvé.");

	return(true);
}
//-------------------------------------------------------------------------------------------
bool ChatHandler::HandleGOAnimProgress(const char * args, WorldSession * m_session)
{
	if(!m_session->GetPlayer()->m_GM_SelectedGO)
		return false;

	uint32 ap = atol(args);
	//m_session->GetPlayer()->m_GM_SelectedGO->SetUInt32Value(GAMEOBJECT_ANIMPROGRESS, ap);
	m_session->GetPlayer()->m_GM_SelectedGO->SetByte(GAMEOBJECT_BYTES_1, 3, ap);
	BlueSystemMessage(m_session, "Set ANIMPROGRESS to %u", ap);
	return true;
}

bool ChatHandler::HandleGOExport(const char * args, WorldSession * m_session)
{
	/*if(!m_session->GetPlayer()->m_GM_SelectedGO)
		return false;
	
	std::stringstream name;
	if (*args)
	{
		name << "GO_" << args << ".sql";
	}
	else
	{
		name << "GO_" << m_session->GetPlayer()->m_GM_SelectedGO->GetEntry() << ".sql";
	}
   
	m_session->GetPlayer()->m_GM_SelectedGO->SaveToFile(name);

	BlueSystemMessage(m_session, "Go saved to: %s", name.str().c_str());*/

	Creature * pCreature = getSelectedCreature(m_session, true);
	if(!pCreature) return true;
	WorldDatabase.WaitExecute("INSERT INTO creature_names_export SELECT * FROM creature_names WHERE entry = %u", pCreature->GetEntry());
	WorldDatabase.WaitExecute("INSERT INTO creature_proto_export SELECT * FROM creature_proto WHERE entry = %u", pCreature->GetEntry());
	WorldDatabase.WaitExecute("INSERT INTO vendors_export SELECT * FROM vendors WHERE entry = %u", pCreature->GetEntry());
	QueryResult * qr = WorldDatabase.Query("SELECT * FROM vendors WHERE entry = %u", pCreature->GetEntry());
	if(qr != NULL)
	{
		do 
		{
			WorldDatabase.WaitExecute("INSERT INTO items_export SELECT * FROM items WHERE entry = %u", qr->Fetch()[1].GetUInt32());
		} while (qr->NextRow());
		delete qr;
	}
	return true;
}

bool ChatHandler::HandleNpcComeCommand(const char* args, WorldSession* m_session)
{
	// moves npc to players location
	Player * plr = m_session->GetPlayer();
	Creature * crt = getSelectedCreature(m_session, true);
	if(!crt) return true;

	crt->GetAIInterface()->MoveTo(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), plr->GetOrientation());
	return true;
}

bool ChatHandler::HandleNpcCastSelfCommand(const char* args, WorldSession* m_session)
{
	if(!args || strlen(args) < 2)
	{
		RedSystemMessage(m_session, "Id du sort non specifie.");
		return true;
	}

	uint32 spellid = atol(args);
	Creature * crt = getSelectedCreature(m_session, true);
	if(!crt) return true;

	SpellEntry* sp=dbcSpell.LookupEntry(spellid);
	if (!sp)
	{
		RedSystemMessage(m_session, "id du sort invalide");
		return true;
	}

	crt->CastSpell(crt, sp, false);

	return true;
}

bool ChatHandler::HandleNpcCastMeCommand(const char* args, WorldSession* m_session)
{
	if(!args || strlen(args) < 2)
	{
		RedSystemMessage(m_session, "No spellid specified.");
		return true;
	}

	uint32 spellid = atol(args);

	Creature * crt = getSelectedCreature(m_session, true);
	if(!crt) return true;

	SpellEntry* sp=dbcSpell.LookupEntry(spellid);
	if (!sp)
	{
		RedSystemMessage(m_session, "Invalid spell id");
		return true;
	}

	crt->CastSpell(m_session->GetPlayer(), sp, false);

	return true;
}

/////////////////////////////////////////////////
//  Admin Chat Commands
//

bool ChatHandler::HandleWorldPortCommand(const char* args, WorldSession *m_session)
{
	float x, y, z;
	uint32 mapid;
	if(sscanf(args, "%u %f %f %f", (unsigned int*)&mapid, &x, &y, &z) != 4)
		return false;

	if(x >= _maxX || x <= _minX || y <= _minY || y >= _maxY)
		return false;

	LocationVector vec(x, y, z);
	m_session->GetPlayer()->SafeTeleport(mapid, 0, vec);
	return true;
}

bool ChatHandler::HandleClearCooldownsCommand(const char *args, WorldSession *m_session)
{
	uint32 guid = (uint32)m_session->GetPlayer()->GetSelection();
	Player *plr = getSelectedChar(m_session, true);
	
	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}
	if(!plr) return false;

	if(plr->getClass()==WARRIOR)
	{
		plr->ClearCooldownsOnLine(26, guid);
		plr->ClearCooldownsOnLine(256, guid);
		plr->ClearCooldownsOnLine(257 , guid);
		BlueSystemMessage(m_session, "Cleared all Warrior cooldowns.");
		return true;
	}
	if(plr->getClass()==PALADIN)
	{
		plr->ClearCooldownsOnLine(594, guid);
		plr->ClearCooldownsOnLine(267, guid);
		plr->ClearCooldownsOnLine(184, guid);
		BlueSystemMessage(m_session, "Cleared all Paladin cooldowns.");
		return true;
	}
	if(plr->getClass()==HUNTER)
	{
		plr->ClearCooldownsOnLine(50, guid);
		plr->ClearCooldownsOnLine(51, guid);
		plr->ClearCooldownsOnLine(163, guid);
		BlueSystemMessage(m_session, "Cleared all Hunter cooldowns.");
		return true;
	}
	if(plr->getClass()==ROGUE)
	{
		plr->ClearCooldownsOnLine(253, guid);
		plr->ClearCooldownsOnLine(38, guid);
		plr->ClearCooldownsOnLine(39, guid);
		BlueSystemMessage(m_session, "Cleared all Rogue cooldowns.");
		return true;
	}
	if(plr->getClass()==PRIEST)
	{
		plr->ClearCooldownsOnLine(56, guid);
		plr->ClearCooldownsOnLine(78, guid);
		plr->ClearCooldownsOnLine(613, guid);
		BlueSystemMessage(m_session, "Cleared all Priest cooldowns.");
		return true;
	}
	if(plr->getClass()==SHAMAN)
	{
		plr->ClearCooldownsOnLine(373, guid);
		plr->ClearCooldownsOnLine(374, guid);
		plr->ClearCooldownsOnLine(375, guid);
		BlueSystemMessage(m_session, "Cleared all Shaman cooldowns.");
		return true;
	}
	if(plr->getClass()==MAGE)
	{
		plr->ClearCooldownsOnLine(6, guid);
		plr->ClearCooldownsOnLine(8, guid);
		plr->ClearCooldownsOnLine(237, guid);
		BlueSystemMessage(m_session, "Cleared all Mage cooldowns.");
		return true;
	}
	if(plr->getClass()==WARLOCK)
	{
		plr->ClearCooldownsOnLine(355, guid);
		plr->ClearCooldownsOnLine(354, guid);
		plr->ClearCooldownsOnLine(593, guid);
		BlueSystemMessage(m_session, "Cleared all Warlock cooldowns.");
		return true;
	}
	if(plr->getClass()==DRUID)
	{
		plr->ClearCooldownsOnLine(573, guid);
		plr->ClearCooldownsOnLine(574, guid);
		plr->ClearCooldownsOnLine(134, guid);
		BlueSystemMessage(m_session, "Cleared all Druid cooldowns.");
		return true;
	}
	if(plr->getClass()==DEATHKNIGHT)
	{
		plr->ClearCooldownsOnLine(770, guid); 
		plr->ClearCooldownsOnLine(771, guid); 
		plr->ClearCooldownsOnLine(772, guid); 
		BlueSystemMessage(m_session, "Cleared all Death Knight cooldowns.");
		return true;
	}
	return true;
}

bool ChatHandler::HandleLearnCommand(const char* args, WorldSession *m_session)
{
	if (!*args)
		return false;

	Player *plr = getSelectedChar(m_session, true);
	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}
	if(!plr) return false;


	if(stricmp(args, "all")==0)
	{
		sGMLog.writefromsession(m_session, "%s apprend tout les sors de sa classe.", plr->GetName());
		static uint32 spellarray[DRUID+1][9999] = {
			    { 0 },		// CLASS 0
/*WARRIOR*/		{ 71, 2458, 355, 469, 674, 676, 750, 871, 1161, 1680, 1719, 2048, 2565, 2687, 3411, 5246, 6554, 7620, 11578, 11585, 12678, 18499, 20230, 23920, 25203, 25208, 25212, 25225, 25231, 25236, 25242, 25264, 25266, 25275, 29704, 29707, 30357, 34428, 0 },		// CLASS 1
/*PALADIN*/		{ 750, 1020, 1038, 1044, 1152, 3127, 4987, 5502, 5573, 7620, 10278, 10308, 10322, 10324, 10326, 19746, 19752, 20271, 20347, 20348, 20356, 20772, 20773, 25780, 25895, 25898, 27136, 27137, 27138, 27139, 27140, 27141, 27142, 27143, 27144, 27145, 27148, 27149, 27150, 27151, 27152, 27153, 27154, 27155, 27158, 27169, 27173, 27180, 31789, 31801, 31884, 31895, 32223, 33388, 33391, 33776, 34090, 34091, 0 },		// CLASS 2
/*HUNTER*/		{ 136, 674, 781, 883, 982, 1002, 1130, 1462, 1494, 1495, 1499, 1510, 1513, 1515, 1543, 1978, 2641, 2643, 2974, 3018, 3034, 3043, 3044, 3045, 3111, 3127, 3661, 3662, 5116, 5118, 5384, 6197, 6991, 13159, 13161, 13163, 13165, 13542, 13543, 13544, 13549, 13550, 13551, 13552, 13553, 13554, 13555, 13795, 13809, 13813, 14260, 14261, 14262, 14263, 14264, 14265, 14266, 14267, 14268, 14269, 14270, 14271, 14272, 14273, 14274, 14279, 14280, 14281, 14282, 14283, 14284, 14285, 14286, 14287, 14288, 14289, 14290, 14294, 14295, 14302, 14303, 14304, 14305, 14310, 14311, 14316, 14317, 14318, 14319, 14320, 14321, 14322, 14323, 14324, 14325, 14326, 14327, 15629, 15630, 15631, 15632, 19801, 19878, 19879, 19880, 19882, 19883, 19884, 19885, 20043, 20190, 20736, 25294, 25295, 25296, 27014, 27015, 27016, 27018, 27019, 27020, 27021, 27022, 27023, 27025, 27044, 27045, 27046, 34026, 34074, 34120, 34477, 34600, 36916, 0 },		// CLASS 3
/*ROGUE*/		{ 674, 921, 1725, 1787, 1804, 1833, 1842, 1860, 2094, 2836, 2842, 3127, 5938, 6774, 8643, 8681, 11297, 11305, 26669, 26679, 26862, 26863, 26865, 26866, 26867, 26884, 26889, 27441, 27448, 31224, 32684, 38764, 38768, 0 },		// CLASS 4
/*PRIEST*/		{ 17, 139, 453, 527, 528, 552, 586, 588, 589, 591, 592, 594, 596, 598, 600, 602, 605, 970, 976, 984, 988, 992, 996, 1004, 1006, 1243, 1244, 1245, 1706, 2006, 2010, 2052, 2053, 2054, 2055, 2060, 2061, 2096, 2767, 2791, 3747, 6060, 6063, 6064, 6065, 6066, 6074, 6075, 6076, 6077, 6078, 7128, 8092, 8102, 8103, 8104, 8105, 8106, 8122, 8124, 8129, 8131, 8192, 9472, 9473, 9474, 9484, 9485, 9578, 9579, 9592, 10874, 10875, 10876, 10880, 10881, 10888, 10890, 10892, 10893, 10894, 10898, 10899, 10900, 10901, 10909, 10911, 10912, 10915, 10916, 10917, 10927, 10928, 10929, 10933, 10934, 10937, 10938, 10941, 10942, 10945, 10946, 10947, 10951, 10952, 10953, 10955, 10957, 10960, 10961, 10963, 10964, 10965, 14914, 15261, 15262, 15263, 15264, 15265, 15266, 15267, 19289, 19291, 19292, 19293, 20770, 25210, 25213, 25217, 25218, 25221, 25222, 25233, 25235, 25308, 25314, 25315, 25316, 25363, 25364, 25367, 25368, 25372, 25375, 25379, 25380, 25384, 25389, 25429, 25431, 25435, 25450, 25596, 32375, 32379, 32546, 32996, 33076, 34433, 34863, 34864, 34865, 34866, 0 },		// CLASS 5
/*DEATHKNIGHT*/	{ 48721, 49939, 49940, 49941, 49998, 49999, 45463, 49923, 49924, 45902, 49926, 49927, 49928, 49929, 49930, 48680, 49913, 49914, 49915, 49916, 47802, 48743, 49934, 45524, 48263, 48266, 47528, 49910, 49911, 49912, 45477, 49896, 49903, 49904, 49909, 47568, 46584, 43265, 49936, 49937, 49938, 45462, 49917, 49918, 49919, 49920, 49921, 47541, 49892, 49893, 49894, 49895, 45843, 49932, 49933, 48265, 42650, 48707, 48778, 57623, 57532, 56815, 57330, 56222, 3714, 48792, 52286, 52285, 53428, 49142, 49410, 50977, 53341, 53331, 53343, 54447, 53342, 54446, 53323, 53344 },		// CLASS 6
/*SHAMAN*/		{ 131, 324, 325, 332, 370, 421, 526, 529, 546, 547, 548, 556, 905, 913, 915, 930, 939, 943, 945, 959, 1064, 1535, 2008, 2484, 2645, 2860, 2870, 3599, 5394, 5675, 5730, 6041, 6196, 6363, 6364, 6365, 6375, 6377, 6390, 6391, 6392, 6495, 8004, 8005, 8008, 8010, 8012, 8017, 8024, 8027, 8030, 8033, 8038, 8042, 8044, 8045, 8046, 8050, 8052, 8053, 8056, 8058, 8071, 8075, 8134, 8143, 8154, 8155, 8160, 8161, 8166, 8170, 8177, 8181, 8184, 8190, 8227, 8232, 8235, 8249, 8498, 8499, 8512, 8835, 10391, 10392, 10395, 10396, 10406, 10407, 10408, 10412, 10413, 10414, 10427, 10428, 10431, 10432, 10437, 10438, 10442, 10447, 10448, 10456, 10462, 10463, 10466, 10467, 10468, 10472, 10473, 10478, 10479, 10486, 10495, 10496, 10497, 10526, 10537, 10538, 10585, 10595, 10600, 10601, 10605, 10613, 10622, 10623, 10627, 11314, 11315, 15107, 15111, 15112, 15207, 15208, 16339, 16341, 16342, 16355, 16356, 16362, 16387, 20608, 20609, 20610, 20776, 20777, 24398, 25357, 25359, 25361, 25391, 25396, 25420, 25422, 25423, 25439, 25442, 25448, 25449, 25454, 25457, 25464, 25469, 25472, 25489, 25500, 25505, 25508, 25509, 25525, 25528, 25533, 25546, 25547, 25557, 25560, 25563, 25567, 25570, 25574, 25577, 25908, 29228, 33736, 36936, 0 },		// CLASS 7
/*MAGE*/		{ 10, 66, 116, 118, 120, 122, 130, 143, 145, 205, 475, 543, 587, 597, 604, 759, 837, 865, 990, 1008, 1449, 1459, 1460, 1461, 1463, 1953, 2120, 2121, 2136, 2137, 2138, 2139, 2855, 2948, 3140, 3552, 5143, 5144, 5145, 5504, 5505, 5506, 6117, 6127, 6129, 6131, 6141, 6143, 7300, 7301, 7302, 7320, 7322, 8400, 8401, 8402, 8406, 8407, 8408, 8412, 8413, 8416, 8417, 8422, 8423, 8427, 8437, 8438, 8439, 8444, 8445, 8446, 8450, 8451, 8455, 8457, 8458, 8461, 8462, 8492, 8494, 8495, 10053, 10054, 10138, 10139, 10140, 10144, 10145, 10148, 10149, 10150, 10151, 10156, 10157, 10159, 10160, 10161, 10169, 10170, 10173, 10174, 10177, 10179, 10180, 10181, 10185, 10186, 10187, 10191, 10192, 10193, 10197, 10199, 10201, 10202, 10205, 10206, 10207, 10211, 10212, 10215, 10216, 10219, 10220, 10223, 10225, 10230, 12051, 12824, 12825, 12826, 22782, 22783, 25304, 25306, 25345, 27070, 27071, 27072, 27073, 27074, 27075, 27078, 27079, 27080, 27082, 27085, 27086, 27087, 27088, 27101, 27124, 27125, 27126, 27128, 27130, 27131, 28609, 28612, 30449, 30451, 30455, 30482, 32796, 33717, 33944, 33946, 37420, 38699, 0 },		// CLASS 8
/*WARLOCK*/		{ 126, 132, 172, 348, 603, 688, 689, 691, 693, 695, 696, 697, 698, 699, 702, 704, 705, 706, 707, 709, 710, 712, 755, 980, 1010, 1014, 1086, 1088, 1094, 1098, 1106, 1108, 1120, 1122, 1454, 1455, 1456, 1490, 1714, 1949, 2362, 2941, 3698, 3699, 3700, 5138, 5484, 5500, 5676, 5697, 5699, 5740, 5782, 5784, 6201, 6202, 6205, 6213, 6215, 6217, 6219, 6222, 6223, 6226, 6229, 6353, 6366, 6789, 7641, 7646, 7648, 7651, 7658, 7659, 8288, 8289, 11659, 11660, 11661, 11665, 11667, 11668, 11671, 11672, 11675, 11677, 11678, 11683, 11684, 11687, 11688, 11689, 11693, 11694, 11695, 11699, 11700, 11703, 11704, 11707, 11708, 11711, 11712, 11713, 11717, 11719, 11721, 11722, 11725, 11726, 11729, 11730, 11733, 11734, 11735, 11739, 11740, 17727, 17728, 17862, 17919, 17920, 17921, 17922, 17923, 17924, 17925, 17926, 17928, 17937, 18540, 18647, 20752, 20755, 20756, 20757, 23161, 25307, 25309, 25311, 27209, 27210, 27211, 27212, 27213, 27215, 27216, 27217, 27218, 27219, 27220, 27221, 27222, 27223, 27224, 27226, 27228, 27229, 27230, 27238, 27243, 27259, 27260, 28172, 28176, 28189, 28610, 29722, 29858, 29893, 30459, 30545, 30908, 30909, 30910, 32231, 0 },		// CLASS 9
			    { 0 },		// CLASS 10
/*DRUID*/	    { 99, 339, 467, 740, 768, 769, 770, 774, 778, 779, 780, 782, 783, 1058, 1062, 1066, 1075, 1079, 1082, 1126, 1178, 1430, 1735, 1822, 1823, 1824, 1850, 2090, 2091, 2637, 2782, 2893, 2908, 2912, 3025, 3029, 3137, 3627, 5177, 5178, 5179, 5180, 5186, 5187, 5188, 5189, 5195, 5196, 5201, 5209, 5211, 5215, 5217, 5221, 5225, 5229, 5232, 5234, 6756, 6778, 6780, 6783, 6785, 6787, 6793, 6795, 6798, 6800, 6807, 6808, 6809, 8903, 8905, 8907, 8910, 8914, 8918, 8921, 8924, 8925, 8926, 8927, 8928, 8929, 8936, 8938, 8939, 8940, 8941, 8946, 8949, 8950, 8951, 8955, 8972, 8983, 8992, 8998, 9005, 9490, 9492, 9493, 9634, 9745, 9747, 9749, 9750, 9752, 9754, 9756, 9758, 9821, 9823, 9827, 9829, 9830, 9833, 9834, 9835, 9839, 9840, 9841, 9845, 9846, 9849, 9850, 9852, 9853, 9856, 9857, 9858, 9862, 9863, 9866, 9867, 9875, 9876, 9880, 9881, 9884, 9885, 9888, 9889, 9894, 9896, 9898, 9901, 9904, 9907, 9908, 9910, 9912, 9913, 16914, 16952, 16954, 16958, 16961, 17401, 17402, 18657, 18658, 18960, 20484, 20719, 20739, 20742, 20747, 20748, 22568, 22570, 22812, 22827, 22828, 22829, 22842, 22895, 22896, 24248, 25297, 25298, 25299, 26978, 26979, 26980, 26981, 26982, 26983, 26984, 26985, 26986, 26987, 26988, 26989, 26990, 26992, 26993, 26994, 26995, 26996, 26997, 26998, 26999, 27000, 27001, 27002, 27003, 27005, 27006, 27008, 27012, 29166, 31018, 33357, 33745, 33763, 33786, 33943, 34090, 0 },		// CLASS 11
		};

		uint32 c = plr->getClass();
		for(uint32 i = 0; spellarray[c][i] != 0; ++i)
		{
			plr->addSpell(spellarray[c][i]);
		}

		static uint32 paladinspellarray[RACE_DRAENEI+1][2] = {
			{ 0 },		// RACE 0
			{ 13819, 0 },		// HUMAN
			{ 0 },		// ORC
			{ 13819, 0 },		// DWARF
			{ 0 },		// NIGHTELF
			{ 0 },		// UNDEAD
			{ 0 },		// TAUREN
			{ 0 },		// GNOME
			{ 0 },		// TROLL
			{ 34769, 0 },		// BLOODELF
			{ 13819, 0 },		// DRAENEI
		};

		static uint32 priestspellarray[RACE_DRAENEI+1][16] = {
			{ 0 },		// RACE 0
			{ 13896, 13908, 19236, 19238, 19240, 19241, 19242, 19243, 19271, 19273, 19274, 19275, 25437, 25441, 0 },		// HUMAN
			{ 0 },		// ORC
			{ 13908, 19236, 19238, 19240, 44041, 19241, 19242, 19243, 25437, 44043, 44044, 44045, 44046, 44047, 0 },		// DWARF
			{ 2651, 10797, 19296, 19299, 19302, 19303, 19304, 19305, 25446, 0 },		// NIGHTELF
			{ 2652, 2944, 19261, 19262, 19264, 19265, 19266, 19276, 19277, 19278, 19279, 19280, 25461, 25467, 0 },		// UNDEAD
			{ 0 },		// TAUREN
			{ 0 },		// GNOME
			{ 9035, 18137, 19281, 19282, 19283, 19284, 19285, 19308, 19309, 19310, 19311, 19312, 25470, 25477, 0 },		// TROLL
			{ 2652, 19261, 19262, 19264, 19265, 19266, 25461, 32676, 0 },		// BLOODELF
			{ 32548, 44041, 44043, 44044, 44045, 44046, 44047, 0 },		// DRAENEI
		};

		static uint32 shamanspellarray[RACE_DRAENEI+1][2] = {
			{ 0 },		// RACE 0
			{ 0 },		// HUMAN
			{ 2825, 0 },		// ORC
			{ 0 },		// DWARF
			{ 0 },		// NIGHTELF
			{ 0 },		// UNDEAD
			{ 2825, 0 },		// TAUREN
			{ 0 },		// GNOME
			{ 2825, 0 },		// TROLL
			{ 0 },		// BLOODELF
			{ 32182, 0 },		// DRAENEI
		};

		static uint32 magespellarray[RACE_DRAENEI+1][11] = {
			{ 0 },		// RACE 0
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 0 },		// HUMAN
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 0 },		// ORC
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 0 },		// DWARF
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 0 },		// NIGHTELF
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 0 },		// UNDEAD
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 0 },		// TAUREN
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 0 },		// GNOME
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 0 },		// TROLL
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 0 },		// BLOODELF
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 0 },		// DRAENEI
		};

		uint32 r = plr->getRace();
		switch(c)
		{
		case PALADIN:
			for( uint32 i = 0; paladinspellarray[r][i] != 0; ++i )
			{
				plr->addSpell( paladinspellarray[r][i] );
			}
			break;
		case PRIEST:
			for( uint32 i = 0; priestspellarray[r][i] != 0; ++i )
			{
				plr->addSpell( priestspellarray[r][i] );
			}
			break;
		case MAGE:
			for( uint32 i = 0; magespellarray[r][i] != 0; ++i )
			{
				plr->addSpell( magespellarray[r][i] );
			}
			break;
		case SHAMAN:
			for( uint32 i = 0; shamanspellarray[r][i] != 0; ++i )
			{
				plr->addSpell( shamanspellarray[r][i] );
			}
			break;
		}
		return true;
	}

	uint32 spell = atol((char*)args);
	sGMLog.writefromsession(m_session, "taught %s spell %u", plr->GetName(), spell);

	SpellEntry * sp = dbcSpell.LookupEntry(spell);
	if(!plr->GetSession()->HasGMPermissions() && (sp->Effect[0]==SPELL_EFFECT_INSTANT_KILL||sp->Effect[1]==SPELL_EFFECT_INSTANT_KILL||sp->Effect[2]==SPELL_EFFECT_INSTANT_KILL))
	{
		SystemMessage(m_session, "don't be an idiot and teach players instakill spells. this action has been logged.");
		return true;
	}

	if (plr->HasSpell(spell)) // check to see if char already knows
	{
		std::string OutStr = plr->GetName();
		OutStr += " Connait deja cette attaque.";

		SystemMessage(m_session, OutStr.c_str());
		return true;
	}

	plr->addSpell(spell);
	BlueSystemMessageToPlr(plr, "%s taught you Spell %d", m_session->GetPlayer()->GetName(), spell);

	return true;
}

bool ChatHandler::HandleReviveCommand(const char* args, WorldSession *m_session)
{
	Player* SelectedPlayer = getSelectedChar(m_session, true);
	if(!SelectedPlayer) return true;

	
	SelectedPlayer->SetMovement(MOVE_UNROOT, 1);
	SelectedPlayer->ResurrectPlayer(NULL);
	SelectedPlayer->SetUInt32Value(UNIT_FIELD_HEALTH, SelectedPlayer->GetUInt32Value(UNIT_FIELD_MAXHEALTH) );
	return true;
}

bool ChatHandler::HandleExploreCheatCommand(const char* args, WorldSession *m_session)
{
	if (!*args)
		return false;

	int flag = atoi((char*)args);

	Player *chr = getSelectedChar(m_session);
	if (chr == NULL)
	{
		SystemMessage(m_session, "Pas de personnage selectionne.");
		return false;
	}

	char buf[256];

	// send message to user
	if (flag != 0)
	{
		snprintf((char*)buf,256,"%s a explore toutes les zones.", chr->GetName());
	}
	else
	{
		snprintf((char*)buf,256,"%s n'a plus aucunes zones d'explorees.", chr->GetName());
	}
	SystemMessage(m_session, buf);

	// send message to player
	if (flag != 0)
	{
		snprintf((char*)buf,256,"%s a explore toutes les zones pour vous.",
			m_session->GetPlayer()->GetName());
	}
	else
	{
		snprintf((char*)buf,256,"%s vous a cacher toutes les zones.", 
			m_session->GetPlayer()->GetName());
	}
	SystemMessage(m_session,  buf);


	for (uint8 i=0; i < PLAYER_EXPLORED_ZONES_MAX; i++)
	{
		if (flag != 0)
		{
			chr->SetFlag(PLAYER_EXPLORED_ZONES_1+i,0xFFFFFFFF);
		}
		else
		{
			chr->SetFlag(PLAYER_EXPLORED_ZONES_1+i,0x00000000);
		}
	}

	return true;
}

bool ChatHandler::HandleBanCharacterCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	// this is rather complicated due to ban reasons being able to have spaces. so we'll have to some c string magic
	// rather than just sscanf'ing it.
	char * pCharacter = (char*)args;
	char * pBanDuration = strchr(pCharacter, ' ');
	PlayerInfo * pInfo = NULL;
	if(pBanDuration == NULL)
		return false;

	char * pReason = strchr(pBanDuration+1, ' ');
	if(pReason == NULL)
		return false;

	// zero them out to create sepearate strings.
	*pBanDuration = 0;
	++pBanDuration;
	*pReason = 0;
	++pReason;

	int32 BanTime = GetTimePeriodFromString(pBanDuration);
	if(BanTime < 1)
		return false;

	Player * pPlayer = objmgr.GetPlayer(pCharacter, false);
	if(pPlayer == NULL)
	{
		pInfo = objmgr.GetPlayerInfoByName(pCharacter);
		if(pInfo == NULL)
		{
			SystemMessage(m_session, "Joueur introuvable.");
			return true;
		}

		SystemMessage(m_session, "Banning player '%s' in database for '%s'.", pCharacter, pReason);
		string escaped_reason = CharacterDatabase.EscapeString(string(pReason));
		
		CharacterDatabase.Execute("UPDATE characters SET banned = %u, banReason = '%s' WHERE guid = %u",
			BanTime ? BanTime+(uint32)UNIXTIME : 1, escaped_reason.c_str(), pInfo->guid);
	}
	else
	{
		SystemMessage(m_session, "Banning player '%s' ingame for '%s'.", pCharacter, pReason);
		string sReason = string(pReason);
		uint32 uBanTime = BanTime ? BanTime+(uint32)UNIXTIME : 1;
		pPlayer->SetBanned(uBanTime, sReason);
		pInfo = pPlayer->m_playerInfo;
	}

	SystemMessage(m_session, "This ban is due to expire %s%s.", BanTime ? "on " : "", BanTime ? ConvertTimeStampToDataTime(BanTime+(uint32)UNIXTIME).c_str() : "Never");
	if(pPlayer)
	{
		SystemMessage(m_session, "Kicking %s.", pPlayer->GetName());
		pPlayer->Kick();
	}

	sGMLog.writefromsession(m_session, "used ban character on %s reason %s for %s", pCharacter, pReason, BanTime ? ConvertTimeStampToString(BanTime).c_str() : "ever");
	if( sWorld.m_banTable && pInfo )
	{
		CharacterDatabase.Execute("INSERT INTO %s VALUES('%s', '%s', %u, %u, '%s')", sWorld.m_banTable,
			m_session->GetPlayer()->GetName(), pInfo->name, (uint32)UNIXTIME, (uint32)UNIXTIME + BanTime, CharacterDatabase.EscapeString(string(pReason)).c_str() );
	}
	return true;
}

bool ChatHandler::HandleUnBanCharacterCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	char Character[255];
	if(sscanf(args, "%s", Character) == 0)
	{
		RedSystemMessage(m_session, "A character name and reason is required.");
		return true;
	}

	// Check if player is in world.
	Player * pPlayer = ObjectMgr::getSingleton( ).GetPlayer(Character, false);
	if(pPlayer != 0)
	{
		GreenSystemMessage(m_session, "Unbanned player %s ingame.",pPlayer->GetName());
		pPlayer->UnSetBanned();
	}
	else
	{
		GreenSystemMessage(m_session, "Player %s not found ingame.", Character);
	}

	// Ban in database
	CharacterDatabase.Execute("UPDATE characters SET banned = 0 WHERE name = '%s'", CharacterDatabase.EscapeString(string(Character)).c_str());

	SystemMessage(m_session, "Unbanned character %s in database.", Character);
	sGMLog.writefromsession(m_session, "used unban character on %s", Character);
	return true;
}

bool ChatHandler::HandleGMTicketGetAllCommand(const char* args, WorldSession *m_session)
{
#ifdef CLUSTERING
	string message = "GmTicket 2";
	WorldPacket data(ICMSG_CHANNEL_ACTION, 1 + sWorld.getGmClientChannel().size() + 4 + message.size() + 4 + 1);
	data << uint8(CHANNEL_SAY);
	data << sWorld.getGmClientChannel();
	data << m_session->GetPlayer()->GetLowGUID();
	data << message;
	data << m_session->GetPlayer()->GetLowGUID();
	data << bool(true);
	sClusterInterface.SendPacket(&data);
#else
	Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(),m_session->GetPlayer());
	if(!chn)
		return false;

	chn->Say(m_session->GetPlayer(), "GmTicket 2", m_session->GetPlayer(), true);
#endif	
	for(GmTicketList::iterator itr = objmgr.GM_TicketList.begin(); itr != objmgr.GM_TicketList.end(); itr++)
	{
		uint32 cont = 0;
		uint32 zone = 0;
		Player* plr = objmgr.GetPlayer((uint32)(*itr)->guid);
		/*if(plr)
		{
			if(plr->IsInWorld())
			{
				zone = plr->GetZoneId();
				cont = plr->GetMapId();
			}
		}*/
		if( plr == NULL || !plr->IsInWorld() )
			continue;

		cont = plr->GetMapId();
		zone = plr->GetZoneId();

			std::stringstream str;
			str << "GmTicket 0,";
			str << (*itr)->name.c_str() << ","  << (*itr)->level << ","  << (*itr)->map << ",";
			str << zone;
#ifdef CLUSTERING
			string message = str.str();
			WorldPacket data(ICMSG_CHANNEL_ACTION, 1 + sWorld.getGmClientChannel().size() + 4 + message.size() + 4 + 1);
			data << uint8(CHANNEL_SAY);
			data << sWorld.getGmClientChannel();
			data << m_session->GetPlayer()->GetLowGUID();
			data << message;
			data << m_session->GetPlayer()->GetLowGUID();
			data << bool(true);
			sClusterInterface.SendPacket(&data);
#else
			chn->Say(m_session->GetPlayer(),str.str().c_str(), m_session->GetPlayer(), true);
#endif			
	}

	return true;
}

bool ChatHandler::HandleGMTicketGetByIdCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;


	GmTicketList::iterator i;
	for(i = objmgr.GM_TicketList.begin(); i != objmgr.GM_TicketList.end(); i++)
	{
		if(strcmp((*i)->name.c_str(), args) == 0)
		{
#ifdef CLUSTERING
			std::stringstream str;
			str << "GmTicket 3,";
			str << (*i)->name.c_str() << "," << (*i)->message;
#else
			Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(),m_session->GetPlayer());
			if(!chn)
				return false;

			std::stringstream str;
			str << "GmTicket 3,";
			str << (*i)->name.c_str() << "," << (*i)->message;
			chn->Say(m_session->GetPlayer(),str.str().c_str(), m_session->GetPlayer(), true);
#endif
		}
	}

	return true;
}

bool ChatHandler::HandleGMTicketDelByIdCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	GmTicketList::iterator i;
	int64 guid = -1;
	for(i = objmgr.GM_TicketList.begin(); i != objmgr.GM_TicketList.end(); i++)
	{
		if(strcmp((*i)->name.c_str(), args) == 0)
		{
			guid = (int64)(*i)->guid;
			break;
		}
	}
	if(guid != -1)
	{
		objmgr.remGMTicket(guid);

		std::stringstream str;
		str << "GmTicket 1," << args;
		
#ifndef CLUSTERING
		Channel *chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(),m_session->GetPlayer());
		if(!chn)
			return false;
#endif

#ifdef CLUSTERING 
		string message = str.str();
		WorldPacket data(ICMSG_CHANNEL_ACTION, 1 + sWorld.getGmClientChannel().size() + 4 + message.size() + 4 + 1);
		data << uint8(CHANNEL_SAY);
		data << sWorld.getGmClientChannel();
		data << m_session->GetPlayer()->GetLowGUID();
		data << message;
		data << m_session->GetPlayer()->GetLowGUID();
		data << bool(true);
		sClusterInterface.SendPacket(&data);
#else
		chn->Say(m_session->GetPlayer(), str.str().c_str(), NULL, true);
#endif

		Player* plr = objmgr.GetPlayer((uint32)guid);
		if(!plr)
			return true;
		if(!plr->IsInWorld())
			return true;

		WorldPacket data2(SMSG_GMTICKET_DELETETICKET, 4);
		data2 << uint32(9);

		plr->GetSession()->SendPacket( &data2 );
	}

	return true;
}

bool ChatHandler::HandleAddSkillCommand(const char* args, WorldSession *m_session)
{
	char buf[256];
	Player* target = objmgr.GetPlayer((uint32)m_session->GetPlayer()->GetSelection());

	if(!target) {
		SystemMessage(m_session, "Select A Player first.");
		return true;
	}

	uint32 skillline;
	uint16 cur, max;

	char* next_token;
	char* pSkillline = strtok_s((char*)args, " ",&next_token);
	if (!pSkillline)
		return false;

	char* pCurrent = strtok_s(NULL, " ",&next_token);
	if (!pCurrent)
		return false;

	char* pMax = strtok_s(NULL, " ",&next_token);
	if (!pMax)
		return false;

	skillline = (uint32)atol(pSkillline);
	cur = (uint16)atol(pCurrent);
	max = (uint16)atol(pMax);

	target->_AddSkillLine(skillline,cur,max);

	snprintf(buf,256,"SkillLine: %u CurrentValue %u Max Value %u Added.",(unsigned int)skillline,(unsigned int)cur,(unsigned int)max);
	sGMLog.writefromsession(m_session, "added skill line %u (%u/%u) to %s", skillline, cur, max, target->GetName());
	SystemMessage(m_session, buf);

	return true;
}

bool ChatHandler::HandleNpcSpeakCommand(const char *args, WorldSession *m_session)
{
  Creature *crt = getSelectedCreature(m_session);
  if(!crt) return(false);

  if((!crt->IsUnit()) || (crt->IsPlayer())) 
  {
	  RedSystemMessage(m_session, "Vous devez selectionner un Pnj");
	  return(true);
  }

  uint32 Pnj_is_Speaker = 0;

  char* next_token;
  char* pPnj_is_Speaker = strtok_s((char*)args, " ",&next_token);
  if (!pPnj_is_Speaker) return(false);
  Pnj_is_Speaker = (uint32)atoi(pPnj_is_Speaker);

  if(Pnj_is_Speaker) 
  {   // Une fois que le pnj sait parler, il conserve cet etat pour toujours.
	  // Le flag ne sert plus qu'a lui couper la parole par la suite ;)
	  ((Unit *)crt)->SetPNJIsSpeaker(true);
	  GreenSystemMessage(m_session, "%s sait maintenant parler en langage naturel",crt->GetCreatureName()->Name);
  }
  else
  {
	  ((Unit *)crt)->SetPNJIsSpeaker(false);
	  GreenSystemMessage(m_session, "%s coupe le sifflet de %s",m_session->GetPlayer()->GetName(),crt->GetCreatureName()->Name);
  }
  return(true);
}

bool ChatHandler::HandleNpcInfoCommand(const char *args, WorldSession *m_session)
{
	char msg[512]; 
	uint32 Npc_Race,Npc_family,Npc_Class,Npc_Genre,Npc_TypePower;

	uint32 Hguid = GET_TYPE_FROM_GUID(m_session->GetPlayer()->GetSelection());
	uint32 _Entry = GET_ENTRY_FROM_GUID(m_session->GetPlayer()->GetSelection());
	// Quartet 1-3   : 0x000 : Player - 0xF13 : NPC - 0xF14 : Pet - 0xF15 : Vehicle 
    // Quartet 4-10  : NPC Id
    // Quartet 11-16 : NPC SpawnId
	//           1 111111 
	// 123 4567890 123456  
	// 000 0000000 000000
	if(!_Entry || !Hguid) BlueSystemMessage(m_session, "Pas de Selection: Type : 0x%08X, Entry:%d",Hguid,_Entry);
	else                  BlueSystemMessage(m_session, "Selection: Type : 0x%08X, Entry:%d",Hguid,_Entry);
                  
	Creature *crt = getSelectedCreature(m_session);
	if(!crt) return false;
	if(crt->GetCreatureName())
		BlueSystemMessage(m_session, "Information sur %s", crt->GetCreatureName()->Name);

	ColorSystemMessage(m_session, MSG_COLOR_RED, "Entry ID    : %u", crt->GetUInt32Value(OBJECT_FIELD_ENTRY));
	ColorSystemMessage(m_session, MSG_COLOR_RED, "SQL Entry ID: %u", crt->GetSQL_id());

		// show byte
		Npc_family    = crt->GetByte(UNIT_FIELD_BYTES_0,0);
		Npc_Class     = crt->GetByte(UNIT_FIELD_BYTES_0,1);
		
		if(Npc_Class!=7) // Note Randdrick : Si la classe est différente de 7 ( humanoide ) alors il s'agit d'une famille de NPC ( ex : famille des loups )
			Npc_Race = 0;
		else 
			Npc_Race      = crt->GetByte(UNIT_FIELD_BYTES_0,0);

		switch(Npc_Race)
		{
		case RACE_HUMAN    : strcpy(msg,"Humain, "); break;
		case RACE_ORC      : strcpy(msg,"Orc, ");    break;
		case RACE_DWARF    : strcpy(msg,"Nain, ");   break;
		case RACE_NIGHTELF : strcpy(msg,"Elf de la nuit, "); break;
		case RACE_UNDEAD   : strcpy(msg,"Mort Vivant, "); break;
		case RACE_TAUREN   : strcpy(msg,"Tauren, "); break;
		case RACE_GNOME    : strcpy(msg,"Gnome, "); break;
		case RACE_TROLL    : strcpy(msg,"Troll, "); break;
		case RACE_GOBLIN   : strcpy(msg,"Goblin, "); break;
		case RACE_BLOODELF : strcpy(msg,"Elf de sang, "); break;
		case RACE_DRAENEI  : strcpy(msg,"Draeni, "); break;
		case RACE_FELORC    : strcpy(msg,"Gangr'orc, "); break;
		case RACE_NAGA      : strcpy(msg,"Naga, "); break;
		case RACE_BROKEN    : strcpy(msg,"Roué"); break; 
		case RACE_SKELETON  : strcpy(msg,"Squelette, "); break; 
		case RACE_VRYKUL    : strcpy(msg,"Vrykul, "); break; 
		case RACE_TUSKAR    : strcpy(msg,"Rohart, "); break;
		case RACE_FORTESTROLL :strcpy(msg,"Troll des forets, "); break; 
		case RACE_TAUNKA      : strcpy(msg,"Taunka, "); break;
		case RACE_LK_SQUELETON  : strcpy(msg,"Squelette du Northrend, "); break; 
		case RACE_ICE_TROLL   : strcpy(msg,"Troll des glaces, "); break; 
		default : strcpy(msg,"Race inconnue, "); 
		       RedSystemMessage(m_session, "Race inconnue: %u",Npc_Race); break;
		}


	// Npc_Class     = crt->GetByte(UNIT_FIELD_BYTES_0,1);
		
	if(crt->IsPlayer())
	{
	 switch(Npc_Class)
	 {
	  //case UNK_CLASS1 : strcat(msg,"Class inconnu, "); break;
	  case WARRIOR    : strcat(msg,"Guerrier, "); break;
	  case PALADIN    : strcat(msg,"Paladin, "); break;
	  case HUNTER     : strcat(msg,"Chasseur, "); break;
	  case ROGUE      : strcat(msg,"Voleur, "); break;
	  case PRIEST     : strcat(msg,"Pretre, "); break;
	  case DEATHKNIGHT: strcat(msg,"Chevalier de la mort, "); break;
	  case SHAMAN     : strcat(msg,"Shaman, "); break;
	  case MAGE       : strcat(msg,"Mage, "); break;
	  case WARLOCK    : strcat(msg,"Demoniste, "); break;
	  // case UNK_CLASS2 : strcat(msg,"Class inconnu, "); break;
	  case DRUID      : strcat(msg,"Druide"); break;
	  default : //strcat(msg,"Class inconnu, ");
		       RedSystemMessage(m_session, "Classe inconnue: %u",Npc_Class); 
			   break;
	 }
	}
    else
	{
		switch(Npc_Class)
		{
		 case NOUNITTYPE : strcat(msg,"non definie,"); break;
		 case BEAST		 : strcat(msg,"Bete,"); break;
		 case DRAGONSKIN : strcat(msg,"Dragon,"); break;
		 case DEMON		 : strcat(msg,"Demon,"); break;
		 case ELEMENTAL	 : strcat(msg,"Elementaire,"); break;
		 case GIANT		 : strcat(msg,"Geant,"); break;
		 case UNDEAD     : strcat(msg,"Mort vivant,"); break;
		 case HUMANOID	 : strcat(msg,"Humain,"); break;
		 case CRITTER	 : strcat(msg,"Bestiole,"); break;	 
		 case MECHANICAL : strcat(msg,"Robot,"); break;	   
		 case UNIT_TYPE_MISC : strcat(msg,"Special,"); break;	   
         default : //strcat(msg," ");
			       RedSystemMessage(m_session, "Classe inconnue: %u",Npc_Class); 
			       break;
		}
		if(Npc_Class!=7)
		{
			switch(Npc_family)
			{	
			case FAMILY_WOLF		  	: strcat(msg,"Loup,"); break; 
			case FAMILY_CAT     	  	: strcat(msg,"Chat,"); break; 
			case FAMILY_SPIDER       	: strcat(msg,"Araignée,"); break; 
			case FAMILY_BEAR         	: strcat(msg,"Ours,"); break; 
			case FAMILY_BOAR         	: strcat(msg,"Sanglier,"); break; 
			case FAMILY_CROCILISK    	: strcat(msg,"Crocodilistique,"); break; 
			case FAMILY_CARRION_BIRD 	: strcat(msg,"Charognard,"); break; 
			case FAMILY_CRAB         	: strcat(msg,"Crâbe,"); break; 
			case FAMILY_GORILLA      	: strcat(msg,"Gorille,"); break; 
			case FAMILY_RAPTOR		  	: strcat(msg,"Raptor,"); break; 
			case FAMILY_TALLSTRIDER  	: strcat(msg,"Haut-trotteur,"); break; 
			case FAMILY_FELHUNTER	  	: strcat(msg,"Chasseur corrompu,"); break; 
			case FAMILY_VOIDWALKER   	: strcat(msg,"Marcheur du vent,"); break; 
			case FAMILY_SUCCUBUS     	: strcat(msg,"Succube,"); break; 
			case FAMILY_DOOMGUARD	  	: strcat(msg,"Garde funeste,"); break; 
			case FAMILY_SCORPID      	: strcat(msg,"Scorpion,"); break; 
			case FAMILY_TURTLE       	: strcat(msg,"Tortue,"); break; 
			case FAMILY_IMP		  	: strcat(msg,"Diablotin,"); break; 
			case FAMILY_BAT          	: strcat(msg,"Chauve-souris,"); break; 
			case FAMILY_HYENA        	: strcat(msg,"Hyène,"); break;
			case FAMILY_BIRD_OF_PREY  	: strcat(msg,"Oiseau de proie,"); break;
			case FAMILY_WIND_SERPENT   : strcat(msg,"Serpent du vent,"); break;
			case FAMILY_REMOTE_CONTROL : strcat(msg,"Golem télécommandé,"); break;
			case FAMILY_FELGUARD       : strcat(msg,"Gangregarde,"); break;
			case FAMILY_DRAGONHAWK     : strcat(msg,"Faucon-Dragon,"); break;
			case FAMILY_RAVAGER        : strcat(msg,"Ravageur,"); break;
			case FAMILY_WARP_STALKER   : strcat(msg,"Traqueur dimensionel,"); break;
			case FAMILY_SPOREBAT       : strcat(msg,"Sporoptère,"); break;
			case FAMILY_NETHER_RAY     : strcat(msg,"Raie du néant,"); break;
			case FAMILY_SERPENT        : strcat(msg,"Serpent,"); break;
			case FAMILY_MOTH			: strcat(msg,"Phalène,"); break;
			case FAMILY_CHIMAERA       : strcat(msg,"Chimère,"); break;
			case FAMILY_DEVILSAUR      : strcat(msg,"Diablosaure,"); break;
			case FAMILY_GHOUL          : strcat(msg,"Ghoule,"); break;
			case FAMILY_SILITHID       : strcat(msg,"Silithide,"); break;
			case FAMILY_WORM           : strcat(msg,"Ver,"); break;
			case FAMILY_RHINO          : strcat(msg,"Rhinocéros,"); break;
			case FAMILY_WASP           : strcat(msg,"Guêpe,"); break;
			case FAMILY_CORE_HOUND     : strcat(msg,"Chien du Magma,"); break;
			case FAMILY_SPIRIT_BEAST   : strcat(msg,"Esprit de bête,"); break;
			default : //strcat(msg,"Class inconnu, ");
				RedSystemMessage(m_session, "Famille inconnue: %u",Npc_family); 
				break;
			}
		}
	}

	Npc_Genre = crt->GetByte(UNIT_FIELD_BYTES_0,2);
	uint32 len_msg = (uint32)strlen(msg);
    sprintf(&msg[len_msg],"%s", (Npc_Genre)?"Male":"Femelle");
	GreenSystemMessage(m_session, msg);

	//snprintf(msg,512,"GUID: %u\nFaction: %d\nNPCFlags: %d\nDisplayID: %d", (uint32)guid, (int)crt->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), (int)crt->GetUInt32Value(UNIT_NPC_FLAGS), (int)crt->GetUInt32Value(UNIT_FIELD_DISPLAYID));
	snprintf(msg,512,"Faction: %d\nNPCFlags: 0x%08X\nDisplayID: %d",(int)crt->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), (int)crt->GetUInt32Value(UNIT_NPC_FLAGS), (int)crt->GetUInt32Value(UNIT_FIELD_DISPLAYID));
	SystemMessage(m_session, msg);
//	if(crt->m_faction)
	GreenSystemMessage(m_session, "Combat Support: Friendly Mask 0x%08X", crt->m_faction->FriendlyMask);
	GreenSystemMessage(m_session, "Combat Support: Ennemy   Mask 0x%08X", crt->m_faction->HostileMask);
	
	GreenSystemMessage(m_session, "Base Health: %u (%u/%u)", crt->GetUInt32Value(UNIT_FIELD_BASE_HEALTH),
		crt->GetUInt32Value(UNIT_FIELD_HEALTH),crt->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
	
	Npc_TypePower = crt->GetByte(UNIT_FIELD_BYTES_0,3);
    
	switch(Npc_TypePower)
	{
	 case POWER_TYPE_HEALTH : 	 
		 GreenSystemMessage(m_session, "Power Type: Health"); break;
	 case POWER_TYPE_MANA: 	     
		 GreenSystemMessage(m_session, "Power Type: Mana %u (%u/%u)",crt->GetUInt32Value(UNIT_FIELD_BASE_MANA),
		                crt->GetUInt32Value(UNIT_FIELD_POWER1),crt->GetUInt32Value(UNIT_FIELD_MAXPOWER1)); 
		 break;
	 case POWER_TYPE_RAGE: 	     
		 GreenSystemMessage(m_session, "Power Type: Rage %u/%u",crt->GetUInt32Value(UNIT_FIELD_POWER2),crt->GetUInt32Value(UNIT_FIELD_MAXPOWER2)); 
		 break;
	 case POWER_TYPE_FOCUS: 	 
		 GreenSystemMessage(m_session, "Power Type: Focus %u/%u",crt->GetUInt32Value(UNIT_FIELD_POWER3),crt->GetUInt32Value(UNIT_FIELD_MAXPOWER3)); 
		 break;
	 case POWER_TYPE_ENERGY: 	 
		 GreenSystemMessage(m_session, "Power Type: Energy %u/%u",crt->GetUInt32Value(UNIT_FIELD_POWER4),crt->GetUInt32Value(UNIT_FIELD_MAXPOWER4)); 
		 break;
	 case POWER_TYPE_HAPPINESS: 
		 GreenSystemMessage(m_session, "Power Type: Happiness (Pet) %u/%u",crt->GetUInt32Value(UNIT_FIELD_POWER5),crt->GetUInt32Value(UNIT_FIELD_MAXPOWER5)); 
		 break;
	 case POWER_TYPE_RUNES: 	 
		 GreenSystemMessage(m_session, "Power Type: Rune %u/%u",crt->GetUInt32Value(UNIT_FIELD_POWER6),crt->GetUInt32Value(UNIT_FIELD_MAXPOWER6)); 
		 break;
	 case POWER_TYPE_RUNIC_POWER:
		 GreenSystemMessage(m_session, "Power Type: Runique %u/%u",crt->GetUInt32Value(UNIT_FIELD_POWER7),crt->GetUInt32Value(UNIT_FIELD_MAXPOWER7)); 
		 break;
	 default : 	RedSystemMessage(m_session, "PowerType inconnu: %u", Npc_TypePower); break;

	}

	GreenSystemMessage(m_session, "Base Armor : %u", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES));
	GreenSystemMessage(m_session, "Base Holy  : %u", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_1));
	GreenSystemMessage(m_session, "Base Fire  : %u", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_2));
	GreenSystemMessage(m_session, "Base Nature: %u", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_3));
	GreenSystemMessage(m_session, "Base Frost : %u", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_4));
	GreenSystemMessage(m_session, "Base Shadow: %u", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_5));
	GreenSystemMessage(m_session, "Base Arcane: %u", crt->GetUInt32Value(UNIT_FIELD_RESISTANCES_6));
	GreenSystemMessage(m_session, "Damage min/max: %f/%f", crt->GetFloatValue(UNIT_FIELD_MINDAMAGE),crt->GetFloatValue(UNIT_FIELD_MAXDAMAGE));

    if(crt->IsSpeaker())
	{
	 if(crt->m_AI_Speak) GreenSystemMessage(m_session, "Ce pnj peut parler");
	 else                GreenSystemMessage(m_session, "Ce pnj n'a encore parler à personne");
	}

	return true;
}

bool ChatHandler::HandleIncreaseWeaponSkill(const char *args, WorldSession *m_session)
{
	char *next_token;
	char *pMin = strtok_s((char*)args, " ",&next_token);
	uint32 cnt = 0;
	if(!pMin)
		cnt = 1;
	else
		cnt = atol(pMin);

	Player *pr = getSelectedChar(m_session, true);
	
	uint32 SubClassSkill = 0;
	if(!pr) pr = m_session->GetPlayer();
	if(!pr) return false;
	Item *it = pr->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
	ItemPrototype* proto = NULL;
	if (!it)
		it = pr->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
	if (it)
		proto = it->GetProto();
	if (proto)
	{
		switch(proto->SubClass)
		{
			// Weapons
		case 0:	// 1 handed axes
			SubClassSkill = SKILL_AXES;
			break;
		case 1:	// 2 handed axes
			SubClassSkill = SKILL_2H_AXES;
			break;		
		case 2:	// bows
			SubClassSkill = SKILL_BOWS;
			break;
		case 3:	// guns
			SubClassSkill = SKILL_GUNS;
			break;		
		case 4:	// 1 handed mace
			SubClassSkill = SKILL_MACES;
			break;
		case 5:	// 2 handed mace
			SubClassSkill = SKILL_2H_MACES;
			break;		
		case 6:	// polearms
			SubClassSkill = SKILL_POLEARMS;
			break;
		case 7: // 1 handed sword
			SubClassSkill = SKILL_SWORDS;
			break;
		case 8: // 2 handed sword
			SubClassSkill = SKILL_2H_SWORDS;
			break;
		case 9: // obsolete
			SubClassSkill = 136;
			break;
		case 10: //1 handed exotic
			SubClassSkill = 136;
			break;
		case 11: // 2 handed exotic
			SubClassSkill = 0;
			break;
		case 12: // fist
			SubClassSkill = SKILL_FIST_WEAPONS;
			break;
		case 13: // misc
			SubClassSkill = 0;
			break;
		case 15: // daggers
			SubClassSkill = SKILL_DAGGERS;
			break;
		case 16: // thrown
			SubClassSkill = SKILL_THROWN;
			break;
		case 17: // spears
			SubClassSkill = SKILL_SPEARS;
			break;
		case 18: // crossbows
			SubClassSkill = SKILL_CROSSBOWS;
			break;
		case 19: // wands
			SubClassSkill = SKILL_WANDS;
			break;
		case 20: // fishing
			SubClassSkill = SKILL_FISHING;
			break;
		}
	} 
	else
	{
		SubClassSkill = 162;
	}

	if(!SubClassSkill)
	{
		RedSystemMessage(m_session, "Can't find skill ID :-/");
		return false;
	}

	uint32 skill = SubClassSkill;

	BlueSystemMessage(m_session, "Modifying skill line %d. Advancing %d times.", skill, cnt);
	sGMLog.writefromsession(m_session, "increased weapon skill of %s by %u", pr->GetName(), cnt);

	if(!pr->_HasSkillLine(skill))
	{
		SystemMessage(m_session, "Does not have skill line, adding.");
		pr->_AddSkillLine(skill, 1, 300);   
	} 
	else 
	{
		pr->_AdvanceSkillLine(skill,cnt);
	}	   
	return true;	
}


bool ChatHandler::HandleResetTalentsCommand(const char* args, WorldSession *m_session)
{
	Player *plr = this->getSelectedChar(m_session);
	if(!plr) return true;

	plr->Reset_Talents();

	SystemMessage(m_session, "Reset talents of %s.", plr->GetName());;
	BlueSystemMessageToPlr(plr, "%s reset all your talents.", m_session->GetPlayer()->GetName());
	sGMLog.writefromsession(m_session, "reset talents of %s", plr->GetName());
	return true;
}

bool ChatHandler::HandleResetSpellsCommand(const char* args, WorldSession *m_session)
{
	Player *plr = this->getSelectedChar(m_session);
	if(!plr) return true;

	plr->Reset_Spells();
	
	SystemMessage(m_session, "Reset spells of %s to level 1.", plr->GetName());;
	BlueSystemMessage(m_session, "%s reset all your spells to starting values.", m_session->GetPlayer()->GetName());
	sGMLog.writefromsession(m_session, "reset spells of %s", plr->GetName());
	return true;
}

bool ChatHandler::HandleAccountLevelCommand(const char * args, WorldSession * m_session)
{
    if(!*args) return false;

	char account[100];
	char gmlevel[100];
	int argc = sscanf(args, "%s %s", account, gmlevel);
	if(argc != 2)
		return false;

	sLogonCommHandler.Account_SetGM( account, gmlevel );

	GreenSystemMessage(m_session, "Account '%s' level has been updated to '%s'. The change will be effective immediately.", account, gmlevel);
	sGMLog.writefromsession(m_session, "set account %s flags to %s", account, gmlevel);

	return true;
}

bool ChatHandler::HandleAccountUnbanCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;
	char * pAccount = (char*)args;
	
	sLogonCommHandler.Account_SetBanned( pAccount, 0 );
	GreenSystemMessage(m_session, "Account '%s' has been unbanned. This change will be effective immediately.", pAccount);
	
	sGMLog.writefromsession(m_session, "unbanned account %s", pAccount);
	return true;
}

bool ChatHandler::HandleAccountBannedCommand(const char * args, WorldSession * m_session)
{
    if(!*args) return false;

	/*char account[100];
	uint32 banned;
	int argc = sscanf(args, "%s %u", account, (unsigned int*)&banned);
	if(argc != 2)
		return false;*/

	char * pAccount = (char*)args;
	char * pDuration = strchr(pAccount, ' ');
	if( pDuration == NULL )
		return false;
	*pDuration = 0;
	++pDuration;

	int32 timeperiod = GetTimePeriodFromString(pDuration);
	if( timeperiod < 0 )
		return false;

	uint32 banned = (timeperiod ? (uint32)UNIXTIME+timeperiod : 1);

	/*stringstream my_sql;
	my_sql << "UPDATE accounts SET banned = " << banned << " WHERE login = '" << CharacterDatabase.EscapeString(string(pAccount)) << "'";

	sLogonCommHandler.LogonDatabaseSQLExecute(my_sql.str().c_str());
	sLogonCommHandler.LogonDatabaseReloadAccounts();*/
	sLogonCommHandler.Account_SetBanned(pAccount, banned);

	GreenSystemMessage(m_session, "Account '%s' has been banned %s%s. The change will be effective immediately.", pAccount, 
		timeperiod ? "until " : "forever", timeperiod ? ConvertTimeStampToDataTime(timeperiod+(uint32)UNIXTIME).c_str() : "");

	sWorld.DisconnectUsersWithAccount(pAccount, m_session);
	sGMLog.writefromsession(m_session, "banned account %s until %s", pAccount, timeperiod ? ConvertTimeStampToDataTime(timeperiod+(uint32)UNIXTIME).c_str() : "permanant");
	return true;
}

bool ChatHandler::HandleAccountMuteCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	char * pAccount = (char*)args;
	char * pDuration = strchr(pAccount, ' ');
	if(pDuration == NULL)
		return false;
	*pDuration = 0;
	++pDuration;

	int32 timeperiod = GetTimePeriodFromString(pDuration);
	if(timeperiod <= 0)
		return false;

	uint32 banned = (uint32)UNIXTIME+timeperiod;

	sLogonCommHandler.Account_SetMute( pAccount, banned );

	string tsstr = ConvertTimeStampToDataTime(timeperiod+(uint32)UNIXTIME);
	GreenSystemMessage(m_session, "Account '%s' has been muted until %s. The change will be effective immediately.", pAccount, 
		tsstr.c_str());

	sGMLog.writefromsession(m_session, "mutex account %s until %s", pAccount, ConvertTimeStampToDataTime(timeperiod+(uint32)UNIXTIME).c_str());

	WorldSession * pSession = sWorld.FindSessionByName(pAccount);
	if( pSession != NULL )
	{
		pSession->m_muted = banned;
		pSession->SystemMessage("Your voice has been muted until %s by a GM. Until this time, you will not be able to speak in any form.", tsstr.c_str());
	}

	return true;
}

bool ChatHandler::HandleAccountUnmuteCommand(const char * args, WorldSession * m_session)
{
	sLogonCommHandler.Account_SetMute( args, 0 );

	GreenSystemMessage(m_session, "Account '%s' has been unmuted.", args);
	WorldSession * pSession = sWorld.FindSessionByName(args);
	if( pSession != NULL )
	{
		pSession->m_muted = 0;
		pSession->SystemMessage("Your voice has restored. You may speak again.");
	}

	return true;
}

bool ChatHandler::HandleGetTransporterTime(const char* args, WorldSession* m_session)
{
	//Player *plyr = m_session->GetPlayer();
	Creature * crt = getSelectedCreature(m_session, false);
	if( crt == NULL )
		return false;

	WorldPacket data(SMSG_ATTACKERSTATEUPDATE, 1000);
	data << uint32(0x00000102);
	data << crt->GetNewGUID();
	data << m_session->GetPlayer()->GetNewGUID();
	
	data << uint32(6);
	data << uint8(1);
	data << uint32(1);
	data << uint32(0x40c00000);
	data << uint32(6);
	data << uint32(0);
	data << uint32(0);
	data << uint32(1);
	data << uint32(0x000003e8);
	data << uint32(0);
	data << uint32(0);
	m_session->SendPacket(&data);
	return true;
}

bool ChatHandler::HandleRemoveAurasCommand(const char *args, WorldSession *m_session)
{
	Player *plr = getSelectedChar(m_session, true);
	if(!plr) return false;

	BlueSystemMessage(m_session, "Removing all auras...");
	for(uint32 i = 0; i < MAX_AURAS; ++i)
	{
		if(plr->m_auras[i] != 0) plr->m_auras[i]->Remove();
	}
	return true;
}

bool ChatHandler::HandleRemoveRessurectionSickessAuraCommand(const char *args, WorldSession *m_session)
{
	Player *plr = getSelectedChar(m_session, true);
	if(!plr) return false;

	BlueSystemMessage(m_session, "Removing ressurection sickness...");
	plr->RemoveAura( 15007 );
	return true;
}

bool ChatHandler::HandleParalyzeCommand(const char* args, WorldSession *m_session)
{
	//Player *plr = getSelectedChar(m_session, true);
	//if(!plr) return false;
	Unit *plr = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!plr || plr->GetTypeId() != TYPEID_PLAYER)
	{
		RedSystemMessage(m_session, "Cible invalide.");
		return true;
	}

	BlueSystemMessage(m_session, "Rooting target.");
	BlueSystemMessageToPlr( static_cast< Player* >( plr ), "You have been rooted by %s.", m_session->GetPlayer()->GetName() );
	WorldPacket data;
	data.Initialize(SMSG_FORCE_MOVE_ROOT);
	data << plr->GetNewGUID();
	data << uint32(1);

	plr->SendMessageToSet(&data, true);
	return true;
}

bool ChatHandler::HandleUnParalyzeCommand(const char* args, WorldSession *m_session)
{
	//Player *plr = getSelectedChar(m_session, true);
	//if(!plr) return false;
	Unit *plr = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!plr || plr->GetTypeId() != TYPEID_PLAYER)
	{
		RedSystemMessage(m_session, "Cible invalide.");
		return true;
	}
	
	BlueSystemMessage(m_session, "Unrooting target.");
	BlueSystemMessageToPlr( static_cast< Player* >( plr ), "You have been unrooted by %s.", m_session->GetPlayer()->GetName() );
	WorldPacket data;
	data.Initialize(SMSG_FORCE_MOVE_UNROOT);
	data << plr->GetNewGUID();
	data << uint32(5);

	plr->SendMessageToSet(&data, true);
	return true;
}

bool ChatHandler::HandleSetMotdCommand(const char* args, WorldSession* m_session)
{
	if(!args || strlen(args) < 2)
	{
		RedSystemMessage(m_session, "Vous devez specifier un message.");
		return true;
	}

	GreenSystemMessage(m_session, "Motd a ete modifie: %s", args);
	World::getSingleton().SetMotd(args);
	sGMLog.writefromsession(m_session, "Set MOTD to %s", args);
	return true;
}

bool ChatHandler::HandleAddItemSetCommand(const char* args, WorldSession* m_session)
{
	uint32 setid = (args ? atoi(args) : 0);
	if(!setid)
	{
		RedSystemMessage(m_session, "You must specify a setid.");
		return true;
	}

	Player *chr = getSelectedChar(m_session);
	if (chr == NULL) {
	RedSystemMessage(m_session, "Unable to select character.");
	return true;
	}

	ItemSetEntry *entry = dbcItemSet.LookupEntry(setid);
	std::list<ItemPrototype*>* l = objmgr.GetListForItemSet(setid);
	if(!entry || !l)
	{
		RedSystemMessage(m_session, "Invalid item set.");
		return true;
	}
	//const char* setname = sItemSetStore.LookupString(entry->name);
	BlueSystemMessage(m_session, "Searching item set %u...", setid);
	uint32 start = getMSTime();
	sGMLog.writefromsession(m_session, "used add item set command, set %u, target %s", setid, chr->GetName());
	for(std::list<ItemPrototype*>::iterator itr = l->begin(); itr != l->end(); ++itr)
	{
		Item *itm = objmgr.CreateItem((*itr)->ItemId, m_session->GetPlayer());
		if(!itm) continue;
		if(itm->GetProto()->Bonding == ITEM_BIND_ON_PICKUP)
			itm->SoulBind();

		if(!chr->GetItemInterface()->AddItemToFreeSlot(itm))
		{
			m_session->SendNotification("No free slots left!");
			delete itm;
			return true;
		} else {
			//SystemMessage(m_session, "Added item: %s [%u]", (*itr)->Name1, (*itr)->ItemId);
			SlotResult * le = chr->GetItemInterface()->LastSearchResult();
			chr->GetSession()->SendItemPushResult(itm,false,true,false,true,le->ContainerSlot,le->Slot,1);
		}
	}
	GreenSystemMessage(m_session, "Added set to inventory complete. Time: %u ms", getMSTime() - start);
	return true;
}

bool ChatHandler::HandleExitInstanceCommand(const char* args, WorldSession* m_session)
{
	BlueSystemMessage(m_session, "Attempting to exit from instance...");
	bool result = m_session->GetPlayer()->ExitInstance();
	if(!result)
	{
		RedSystemMessage(m_session, "Entry points not found.");
		return true;
	} else {
		GreenSystemMessage(m_session, "Removal successful.");
		return true;
	}
}

bool ChatHandler::HandleCastTimeCheatCommand(const char* args, WorldSession* m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if(!plyr) return true;

	bool val = plyr->CastTimeCheat;
	BlueSystemMessage(m_session, "%s cast time cheat on %s.", val ? "Deactivating" : "Activating", plyr->GetName());
	GreenSystemMessageToPlr(plyr, "%s %s a cast time cheat on you.", m_session->GetPlayer()->GetName(), val ? "deactivated" : "activated");

	plyr->CastTimeCheat = !val;
	sGMLog.writefromsession(m_session, "%s cast time cheat on %s", val ? "disabled" : "enabled", plyr->GetName());
	return true;
}

bool ChatHandler::HandleCooldownCheatCommand(const char* args, WorldSession* m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if(!plyr) return true;

	bool val = plyr->CooldownCheat;
	BlueSystemMessage(m_session, "%s cooldown cheat on %s.", val ? "Deactivating" : "Activating", plyr->GetName());
	GreenSystemMessageToPlr(plyr, "%s %s a cooldown cheat on you.", m_session->GetPlayer()->GetName(), val ? "deactivated" : "activated");

	plyr->CooldownCheat = !val;
	sGMLog.writefromsession(m_session, "%s cooldown cheat on %s", val ? "disabled" : "enabled", plyr->GetName());

	return true;
}

bool ChatHandler::HandleGodModeCommand(const char* args, WorldSession* m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if(!plyr) return true;

	bool val = plyr->GodModeCheat;
	BlueSystemMessage(m_session, "%s godmode cheat on %s.", val ? "Deactivating" : "Activating", plyr->GetName());
	GreenSystemMessageToPlr(plyr, "%s %s a godmode cheat on you.", m_session->GetPlayer()->GetName(), val ? "deactivated" : "activated");

	plyr->GodModeCheat = !val;
	sGMLog.writefromsession(m_session, "%s godmode cheat on %s", val ? "disabled" : "enabled", plyr->GetName());
	return true;
}

bool ChatHandler::HandlePowerCheatCommand(const char* args, WorldSession* m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if(!plyr) return true;

	bool val = plyr->PowerCheat;
	BlueSystemMessage(m_session, "%s power cheat on %s.", val ? "Deactivating" : "Activating", plyr->GetName());
	GreenSystemMessageToPlr(plyr, "%s %s a power cheat on you.", m_session->GetPlayer()->GetName(), val ? "deactivated" : "activated");

	plyr->PowerCheat = !val;
	sGMLog.writefromsession(m_session, "%s powertime cheat on %s", val ? "disabled" : "enabled", plyr->GetName());
	return true;
}

bool ChatHandler::HandleShowCheatsCommand(const char* args, WorldSession* m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if(!plyr) return true;

	uint32 active = 0, inactive = 0;
#define print_cheat_status(CheatName, CheatVariable) SystemMessage(m_session, "%s%s: %s%s", MSG_COLOR_LIGHTBLUE, CheatName, \
		CheatVariable ? MSG_COLOR_LIGHTRED : MSG_COLOR_GREEN, CheatVariable ? "Active" : "Inactive");  \
		if(CheatVariable) \
		active++; \
		else \
		inactive++; 

	GreenSystemMessage(m_session, "Showing cheat status for: %s", plyr->GetName());
	print_cheat_status("Cooldown", plyr->CooldownCheat);
	print_cheat_status("CastTime", plyr->CastTimeCheat);
	print_cheat_status("GodMode", plyr->GodModeCheat);
	print_cheat_status("Power", plyr->PowerCheat);
	print_cheat_status("Fly", plyr->FlyCheat);
	print_cheat_status("AuraStack", plyr->stack_cheat);
	SystemMessage(m_session, "%u cheats active, %u inactive.", active, inactive);

#undef print_cheat_status

	return true;
}

bool ChatHandler::HandleFlyCommand(const char* args, WorldSession* m_session)
{
	WorldPacket fly(835, 13);
	
	Player *chr = getSelectedChar(m_session);
	
	if(!chr)
		chr = m_session->GetPlayer();
	
	chr->m_setflycheat = true;
	fly << chr->GetNewGUID();
	fly << uint32(2);
	chr->SendMessageToSet(&fly, true);
	BlueSystemMessage(chr->GetSession(), "Flying mode enabled.");
	return 1;
}

bool ChatHandler::HandleLandCommand(const char* args, WorldSession* m_session)
{
	WorldPacket fly(836, 13);
	
	Player *chr = getSelectedChar(m_session);
	
	if(!chr)
		chr = m_session->GetPlayer();
	
	chr->m_setflycheat = false;
	fly << chr->GetNewGUID();
	fly << uint32(5);
	chr->SendMessageToSet(&fly, true);
	BlueSystemMessage(chr->GetSession(), "Flying mode disabled.");
	return 1;
}

bool ChatHandler::HandleDBReloadCommand(const char* args, WorldSession* m_session)
{
	char str[200];
	if(!*args || strlen(args) < 3)
		return false;

	uint32 mstime = getMSTime();
	snprintf(str, 200, "%s%s initiated server-side reload of table `%s`. The server may experience some lag while this occurs.",
		MSG_COLOR_LIGHTRED, m_session->GetPlayer()->GetName(), args);
	sWorld.SendWorldText(str, 0);
	if(!Storage_ReloadTable(args))
		snprintf(str, 256, "%sDatabase reload failed.", MSG_COLOR_LIGHTRED);
	else
		snprintf(str, 256, "%sDatabase reload completed in %u ms.", MSG_COLOR_LIGHTBLUE, (unsigned int)(getMSTime() - mstime));
	sWorld.SendWorldText(str, 0);
	sGMLog.writefromsession(m_session, "reloaded table %s", args);
	return true;
}

bool ChatHandler::HandleFlySpeedCheatCommand(const char* args, WorldSession* m_session)
{
	float Speed = (float)atof(args);
	if(Speed == 0)
		Speed = 20;

	Player * plr = getSelectedChar(m_session);
	if(plr == 0)
		return true;

	BlueSystemMessage(m_session, "Setting the fly speed of %s to %f.", plr->GetName(), Speed);
	GreenSystemMessage(plr->GetSession(), "%s set your fly speed to %f.", m_session->GetPlayer()->GetName(), Speed);
	
	WorldPacket data(SMSG_FORCE_FLIGHT_SPEED_CHANGE, 16);
	data << plr->GetNewGUID();
	data << uint32(0) << Speed;
	plr->SendMessageToSet(&data, true);
	
	plr->m_flySpeed = Speed;
	
	return true;
}

bool ChatHandler::HandleModifyLevelCommand(const char* args, WorldSession* m_session)
{
	Player * plr = getSelectedChar(m_session, true);
	if(plr == 0) return true;

	uint32 Level = args ? atol(args) : 0;
	if(Level == 0 || Level > sWorld.m_levelCap)  // Correction de la condition N°2 par B.B.
	{
		RedSystemMessage(m_session, "Une valeur doit etre spécifié : de 1 à %u max.",sWorld.m_levelCap);
		return true;
	}

	if(Level == plr->getLevel()) 
	{
		RedSystemMessage(m_session, "%s est déja à ce niveau (%u).",plr->GetName(),sWorld.m_levelCap);
		return(true);
	}

	sGMLog.writefromsession(m_session, "Passage au level %u de %s",Level, plr->GetName());

	// lookup level information
	LevelInfo * Info = objmgr.GetLevelInfo(plr->getRace(), plr->getClass(), Level);
	if(Info == 0)
	{
		RedSystemMessage(m_session, "Les informations du level de %s n'ont pas été trouvées.",plr->GetName());
		return true;
	}

	GreenSystemMessageToPlr(plr, "%s vous a attribué le niveau %u.", m_session->GetPlayer()->GetName(), Level);
	plr->ApplyLevelInfo(Info, Level); // Set level
	BlueSystemMessage(m_session, "%s atteint le niveau %u.", plr->GetName(), Level);
	return true;
}

bool ChatHandler::HandleCreatePetCommand(const char* args, WorldSession* m_session)
{
/*	if(!args || strlen(args) < 2)
		return false;

	uint32 Entry = atol(args);
	if(!Entry)
		return false;
	CreatureProto * pTemplate = CreatureProtoStorage.LookupEntry(Entry);
	CreatureInfo * pCreatureInfo = CreatureNameStorage.LookupEntry(Entry);
	if(!pTemplate || !pCreatureInfo)
	{
		RedSystemMessage(m_session, "Invalid creature spawn template: %u", Entry);
		return true;
	}
	Player * plr = m_session->GetPlayer();

	// spawn a creature of this id to create from
	Creature * pCreature = new Creature(HIGHGUID_UNIT ,1);//no need in guid
	CreatureSpawn * sp = new CreatureSpawn;
	sp->id = 1;
	sp->bytes = 0;
	sp->bytes2 = 0;
	sp->displayid = pCreatureInfo->Male_DisplayID;
	sp->emote_state = 0;
	sp->entry = pCreatureInfo->Id;
	sp->factionid = pTemplate->Faction;
	sp->flags = 0;
	sp->form = 0;
	sp->movetype = 0;
	sp->o = plr->GetOrientation();
	sp->x = plr->GetPositionX();
	sp->y = plr->GetPositionY();
	//sp->respawnNpcLink = 0;
	sp->stand_state = 0;
	sp->channel_spell=sp->channel_target_creature=sp->channel_target_go=0;
	pCreature->Load(sp, (uint32)NULL, NULL);

	Pet *old_tame = plr->GetSummon();
	if(old_tame != NULL)
	{
		old_tame->Dismiss(true);
	}

	// create a pet from this creature
	Pet * pPet = objmgr.CreatePet();
	pPet->SetInstanceID(plr->GetInstanceID());
	pPet->SetMapId(plr->GetMapId());
	pPet->CreateAsSummon(Entry, pCreatureInfo, pCreature, plr, NULL, CREATURE_TYPE_PET, 0);

	// remove the temp creature
	delete sp;
	delete pCreature;

	sGMLog.writefromsession(m_session, "used create pet entry %u", Entry);
*/
	return true;
}


//#ifdef USE_SPECIFIC_AIAGENTS
//this is custom stuff !
bool ChatHandler::HandlePetSpawnAIBot(const char* args, WorldSession *m_session)
{
	if (!*args) return(false);

	if( !m_session->GetPlayer() ) return(false); // wtf ?, oui c clair (Brz)

/*
	uint32 botprice = m_session->GetPlayer()->GetUInt32Value(UNIT_FIELD_LEVEL)*10000; //1 gold per level ?

	if( m_session->GetPlayer()->GetUInt32Value(PLAYER_FIELD_COINAGE) < botprice )
	{
		GreenSystemMessage(m_session, "You need a total of %u coins to afford a bot", botprice);
		return false;
	}
*/
	uint8 botType = (uint8)atof((char*)args);

	if ( botType!=0 )
	{
		RedSystemMessage(m_session, "Incorrect value. Accepting value 0 only = healbot :)");
		return true;
	}

	uint32 Entry;
	char name[50];
	uint8 race = m_session->GetPlayer()->getRace();

	if( race == RACE_HUMAN || race == RACE_DWARF || race == RACE_NIGHTELF || race == RACE_GNOME || race == RACE_DRAENEI )
	{
		Entry = 1826;  // Mage ecarlate
		strcpy( name, "|cffff6060Mage ecarlate" );
	}
	else
	{
		Entry = 5473; // Ogre mage Cognedune
		strcpy( name, "|cffff6060Ogre mage Cognedune" );
	}

	CreatureProto * pTemplate = CreatureProtoStorage.LookupEntry(Entry);
	CreatureInfo * pCreatureInfo = CreatureNameStorage.LookupEntry(Entry);
	if(!pTemplate || !pCreatureInfo)
	{
		RedSystemMessage(m_session, "Id %d (%s) n'existe pas dans la Db, voir avec l'admin.",Entry,name);
		return(true);
	}
	Player * plr = m_session->GetPlayer();

	// spawn a creature of this id to create from
	//Creature * pCreature = new Creature; //(HIGHGUID_TYPE_UNIT);//no need in guid
	Creature * pCreature = plr->GetMapMgr()->CreateCreature(Entry);
	if(!pCreature) 
	{
		RedSystemMessage(m_session, "Erreur de création, Id %d (%s).",Entry,name);
		return(false);
	}

	if(Entry == 1826) // Mage ecarlate
	{
	 pCreature->SetByte(UNIT_FIELD_BYTES_0,2,(pCreatureInfo->Female_DisplayID)?0:1); // Genre M/F
	 pCreature->SetByte(UNIT_FIELD_BYTES_0,1,HUMANOID);
	 pCreature->SetByte(UNIT_FIELD_BYTES_0,0,RACE_HUMAN);
	}
	else // Ogre mage Cognedune
	{
		pCreature->SetByte(UNIT_FIELD_BYTES_0,2,(pCreatureInfo->Female_DisplayID)?0:1); // Genre M/F
		pCreature->SetByte(UNIT_FIELD_BYTES_0,1,HUMANOID);
	    pCreature->SetByte(UNIT_FIELD_BYTES_0,0,RACE_HUMAN); 
	}

	CreatureSpawn * sp = new CreatureSpawn;
	sp->id = objmgr.GenerateCreatureSpawnID();
	sp->bytes0 = pCreature->GetUInt32Value(UNIT_FIELD_BYTES_0);
	sp->bytes1 = pCreature->GetUInt32Value(UNIT_FIELD_BYTES_1);
	sp->bytes2 = 0;
	if(pCreatureInfo->Female_DisplayID) sp->displayid = pCreatureInfo->Female_DisplayID;
	else                                sp->displayid = pCreatureInfo->Male_DisplayID;
	sp->emote_state = 0;
	sp->entry = pCreatureInfo->Id;
	sp->factionid = plr->GetFactionId(); // pTemplate->Faction;
	sp->flags = 0;
	sp->form = 0;
	sp->movetype = 0;
	sp->o = plr->GetOrientation();
	sp->x = plr->GetPositionX()+2;
	sp->y = plr->GetPositionY()+2;
	sp->z = plr->GetPositionZ();
	sp->phase = NORMAL_PHASING;
	sp->channel_spell=sp->channel_target_creature=sp->channel_target_go=0;
	pCreature->Load(sp, (uint32)NULL, NULL);

	 // A virer si utilisation du Pet qui suit
    //pCreature->PushToWorld(m_session->GetPlayer()->GetMapMgr());
	//plr->GetMapMgr()->GetBaseMap()->GetSpawnsListAndCreate(sp->x,sp->y)->CreatureSpawns.push_back(sp);     // Add spawn to map

	
	//------------

	Pet *old_tame = plr->GetSummon();
	if(old_tame != NULL)
	{
		old_tame->Dismiss(true);
	}

	// create a pet from this creature
	Pet * pPet = objmgr.CreatePet();
    if(!pPet)
	{
      delete sp;
	  delete pCreature;
	  RedSystemMessage(m_session, "Erreur de création du 'PetPnj' (%s)",name);
	  return(true);
	} 

	pPet->SetInstanceID(plr->GetInstanceID());
	pPet->SetMapId(plr->GetMapId());

	pPet->SetFloatValue ( OBJECT_FIELD_SCALE_X, pTemplate->Scale / 2); //we do not wish to block visualy other players

	AiAgentHealSupport *new_interface = new AiAgentHealSupport; 

	if(!new_interface)
	{
	  delete pPet;
      delete sp;
	  delete pCreature;

	  RedSystemMessage(m_session, "Erreur de création de l'interface %s",name);
	  return(true);
	}

	pPet->ReplaceAIInterface( (AIInterface *) new_interface ); 
	new_interface->Init(pPet, AITYPE_AGRO, MOVEMENTTYPE_NONE,plr);

	pPet->CreateAsSummon(Entry, pCreatureInfo, pCreature, plr, NULL, CREATURE_TYPE_PET, 0);

	pPet->Rename(name);

	//healer bot should not have any specific ations
	pPet->SetActionBarSlot(0,PET_SPELL_FOLLOW);
	pPet->SetActionBarSlot(1,PET_SPELL_STAY);
	pPet->SetActionBarSlot(2,0);
	pPet->SetActionBarSlot(3,0);
	pPet->SetActionBarSlot(4,0);
	pPet->SetActionBarSlot(5,0);
	pPet->SetActionBarSlot(6,0);
	pPet->SetActionBarSlot(7,0);
	pPet->SetActionBarSlot(8,0);
	pPet->SetActionBarSlot(9,0);
	pPet->SendSpellsToOwner();

	// Ai Fun WaadTeam
	pPet->SetIsPNJ(true);        // Waad Hacky fix, mouahahahahahah.....yeahhhh ;) (Brz)
	pPet->SetPNJIsSpeaker(true); // Yeah, le Bot parleeeeeeeeee (Brz)

	// remove the temp creature
	delete sp;
	delete pCreature;

	BlueSystemMessage(m_session, "%s vous suit pour vous soigner",name);
	return(true);
}
//#endif

bool ChatHandler::HandleAddPetSpellCommand(const char* args, WorldSession* m_session)
{
	Player * plr = m_session->GetPlayer();
	Pet * pPet = plr->GetSummon();
	if(pPet == 0)
	{
		RedSystemMessage(m_session, "You have no pet.");
		return true;
	}

	uint32 SpellId = atol(args);
	SpellEntry * spell = dbcSpell.LookupEntry(SpellId);
	if(!SpellId || !spell)
	{
		RedSystemMessage(m_session, "Invalid spell id requested.");
		return true;
	}

	pPet->AddSpell(spell, true);
	GreenSystemMessage(m_session, "Added spell %u to your pet.", SpellId);
	return true;
}

bool ChatHandler::HandleRemovePetSpellCommand(const char* args, WorldSession* m_session)
{
	Player * plr = m_session->GetPlayer();
	Pet * pPet = plr->GetSummon();
	if(pPet == 0)
	{
		RedSystemMessage(m_session, "You have no pet.");
		return true;
	}

	uint32 SpellId = atol(args);
	SpellEntry * spell = dbcSpell.LookupEntry(SpellId);
	if(!SpellId || !spell)
	{
		RedSystemMessage(m_session, "Invalid spell id requested.");
		return true;
	}

	pPet->RemoveSpell(SpellId);
	GreenSystemMessage(m_session, "Added spell %u to your pet.", SpellId);
	return true;
}

bool ChatHandler::HandleRenamePetCommand(const char* args, WorldSession* m_session)
{
	Player * plr = m_session->GetPlayer();
	Pet * pPet = plr->GetSummon();
	if(pPet == 0)
	{
		RedSystemMessage(m_session, "You have no pet.");
		return true;
	}

	if(strlen(args) < 1)
	{
		RedSystemMessage(m_session, "You must specify a name.");
		return true;
	}

	GreenSystemMessage(m_session, "Renamed your pet to %s.", args);
	pPet->Rename(args);
	return true;
}

bool ChatHandler::HandleShutdownCommand(const char* args, WorldSession* m_session)
{
	uint32 shutdowntime = atol(args);
	if(!args)
		shutdowntime = 5;

	
	char msg[500];
	snprintf(msg, 500, "%sServer shutdown initiated by %s, shutting down in %u seconds.", MSG_COLOR_LIGHTBLUE,
		m_session->GetPlayer()->GetName(), (unsigned int)shutdowntime);

	sWorld.SendWorldText(msg);
	sGMLog.writefromsession(m_session, "initiated server shutdown timer %u sec", shutdowntime);
	shutdowntime *= 1000;
	sMaster.m_ShutdownTimer = shutdowntime;
	sMaster.m_ShutdownEvent = true;
	sMaster.m_restartEvent = false;
	return true;
}

bool ChatHandler::HandleShutdownRestartCommand(const char* args, WorldSession* m_session)
{
	uint32 shutdowntime = atol(args);
	if(!args)
		shutdowntime = 5;

	char msg[500];
	snprintf(msg, 500, "%sServer restart initiated by %s, shutting down in %u seconds.", MSG_COLOR_LIGHTBLUE,
		m_session->GetPlayer()->GetName(), (unsigned int)shutdowntime);

	sGMLog.writefromsession(m_session, "initiated server restart timer %u sec", shutdowntime);
	sWorld.SendWorldText(msg);
		shutdowntime *= 1000;
	sMaster.m_ShutdownTimer = shutdowntime;
	sMaster.m_ShutdownEvent = true;
	sMaster.m_restartEvent = true;
	return true;
}

bool ChatHandler::HandleAllowWhispersCommand(const char* args, WorldSession* m_session)
{
	if(args == 0 || strlen(args) < 2) return false;
	Player * plr = objmgr.GetPlayer(args, false);
	if(!plr)
	{
		RedSystemMessage(m_session, "Joueur introuvable.");
		return true;
	}

	m_session->GetPlayer()->gmTargets.insert(plr);
	BlueSystemMessage(m_session, "Now accepting whispers from %s.", plr->GetName());
	return true;
}

bool ChatHandler::HandleBlockWhispersCommand(const char* args, WorldSession* m_session)
{
	if(args == 0 || strlen(args) < 2) return false;
	Player * plr = objmgr.GetPlayer(args, false);
	if(!plr)
	{
		RedSystemMessage(m_session, "Joueur introuvable.");
		return true;
	}

	m_session->GetPlayer()->gmTargets.erase(plr);
	BlueSystemMessage(m_session, "Now blocking whispers from %s.", plr->GetName());
	return true;
}

bool ChatHandler::HandleAdvanceAllSkillsCommand(const char* args, WorldSession* m_session)
{
	uint32 amt = args ? atol(args) : 0;
	if(!amt)
	{
		RedSystemMessage(m_session, "Un montant pour incrementer est necessaire.");
		return true;
	}

	Player * plr = getSelectedChar(m_session);
	if(!plr)
		return true;


	plr->_AdvanceAllSkills(amt);
	GreenSystemMessageToPlr(plr, "Advanced all your skill lines by %u points.", amt);
	sGMLog.writefromsession(m_session, "advanced all skills by %u on %s", amt, plr->GetName());
	return true;
}

bool ChatHandler::HandleKillByPlayerCommand(const char* args, WorldSession* m_session)
{
	if(!args || strlen(args) < 2)
	{
		RedSystemMessage(m_session, "A player's name is required.");
		return true;
	}

	sWorld.DisconnectUsersWithPlayerName(args,m_session);
	return true;
}

bool ChatHandler::HandleKillBySessionCommand(const char* args, WorldSession* m_session)
{
	if(!args || strlen(args) < 2)
	{
		RedSystemMessage(m_session, "A player's name is required.");
		return true;
	}

	sWorld.DisconnectUsersWithAccount(args,m_session);
	return true;
}
bool ChatHandler::HandleKillByIPCommand(const char* args, WorldSession* m_session)
{
	if(!args || strlen(args) < 2)
	{
		RedSystemMessage(m_session, "An IP is required.");
		return true;
	}

	sWorld.DisconnectUsersWithIP(args,m_session);
	return true;
}

bool ChatHandler::HandleMassSummonCommand(const char* args, WorldSession* m_session)
{
	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	Player * summoner = m_session->GetPlayer();
	Player * plr;
	uint32 c=0;
	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		plr = itr->second;
		if(plr->GetSession() && plr->IsInWorld())
		{
			//plr->SafeTeleport(summoner->GetMapId(), summoner->GetInstanceID(), summoner->GetPosition());
			/* let's do this the blizz way */
			plr->SummonRequest(summoner->GetLowGUID(), summoner->GetZoneId(), summoner->GetMapId(), summoner->GetInstanceID(), summoner->GetPosition());
			++c;
		}
	}
	sGMLog.writefromsession(m_session, "requested a mass summon of %u players.", c);
	objmgr._playerslock.ReleaseReadLock();
	return true;
}

bool ChatHandler::HandleCastAllCommand(const char* args, WorldSession* m_session)
{
	if(!args || strlen(args) < 2)
	{
		RedSystemMessage(m_session, "No spellid specified.");
		return true;
	}
	Player * plr;
	uint32 spellid = atol(args);
	SpellEntry * info = dbcSpell.LookupEntry(spellid);
	if(!info)
	{
		RedSystemMessage(m_session, "Invalid spell specified.");
		return true;
	}

	// this makes sure no moron casts a learn spell on everybody and wrecks the server
	for (int i = 0; i < 3; i++)
	{
		if (info->Effect[i] == 36) //SPELL_EFFECT_LEARN_SPELL - 36
		{
			sGMLog.writefromsession(m_session, "used wrong / learnall castall command, spellid %u", spellid);
			RedSystemMessage(m_session, "Learn spell specified.");
			return true;
		}
	}

	sGMLog.writefromsession(m_session, "used castall command, spellid %u", spellid);

	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		plr = itr->second;
		if(plr->GetSession() && plr->IsInWorld())
		{
			if(plr->GetMapMgr() != m_session->GetPlayer()->GetMapMgr())
			{
				sEventMgr.AddEvent( static_cast< Unit* >( plr ), &Unit::EventCastSpell, static_cast< Unit* >( plr ), info, EVENT_PLAYER_CHECKFORCHEATS, 100, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
			}
			else
			{
				Spell * sp = new Spell(plr, info, true, 0);
				SpellCastTargets targets(plr);
				sp->prepare(&targets);
			}
		}
	}
	objmgr._playerslock.ReleaseReadLock();

	BlueSystemMessage(m_session, "Casted spell %u on all players!", spellid);
	return true;
}

bool ChatHandler::HandleNpcReturnCommand(const char* args, WorldSession* m_session)
{
	Creature * creature = getSelectedCreature(m_session);
	if(!creature || !creature->m_spawn) return true;

	// return to respawn coords
	float x = creature->m_spawn->x;
	float y = creature->m_spawn->y;
	float z = creature->m_spawn->z;
	float o = creature->m_spawn->o;
	
	// restart movement
	creature->GetAIInterface()->SetAIState(STATE_IDLE);
	creature->GetAIInterface()->WipeHateList();
	creature->GetAIInterface()->WipeTargetList();
	creature->GetAIInterface()->MoveTo(x, y, z, o);

	return true;
}

bool ChatHandler::HandleModPeriodCommand(const char* args, WorldSession * m_session)
{
	Transporter * trans = m_session->GetPlayer()->m_CurrentTransporter;
	if(trans == 0)
	{
		RedSystemMessage(m_session, "Vous devez etre sur un transporteur.");
		return true;
	}

	uint32 np = args ? atol(args) : 0;
	if(np == 0)
	{
		RedSystemMessage(m_session, "A time in ms must be specified.");
		return true;
	}

	trans->SetPeriod(np);
	BlueSystemMessage(m_session, "Period of %s set to %u.", trans->GetInfo()->Name, np);
	return true;
}

bool ChatHandler::HandleFormationLink1Command(const char* args, WorldSession * m_session)
{
	// set formation "master"
	Creature * pCreature = getSelectedCreature(m_session, true);
	if(pCreature == 0) return true;

	m_session->GetPlayer()->linkTarget = pCreature;
	BlueSystemMessage(m_session, "Linkup \"master\" set to %s.", pCreature->GetCreatureName()->Name);
	return true;
}

bool ChatHandler::HandleFormationLink2Command(const char* args, WorldSession * m_session)
{
	// set formation "slave" with distance and angle
	float ang, dist;
	if(*args == 0 || sscanf(args, "%f %f", &dist, &ang) != 2)
	{
		RedSystemMessage(m_session, "You must specify a distance and angle.");
		return true;
	}

	if(m_session->GetPlayer()->linkTarget == 0 || m_session->GetPlayer()->linkTarget->GetTypeId() != TYPEID_UNIT)
	{
		RedSystemMessage(m_session, "Master not selected. select the master, and use formationlink1.");
		return true;
	}

	Creature * slave = getSelectedCreature(m_session, true);
	if(slave == 0) return true;

	slave->GetAIInterface()->m_formationFollowDistance = dist;
	slave->GetAIInterface()->m_formationFollowAngle = ang;
	slave->GetAIInterface()->m_formationLinkTarget = static_cast< Creature* >( m_session->GetPlayer()->linkTarget );
	slave->GetAIInterface()->m_formationLinkSqlId = slave->GetAIInterface()->m_formationLinkTarget->GetSQL_id();
	slave->GetAIInterface()->SetUnitToFollowAngle(ang);
	
	// add to db
	WorldDatabase.Execute("INSERT INTO creature_formations VALUES(%u, %u, '%f', '%f')", 
		slave->GetSQL_id(), slave->GetAIInterface()->m_formationLinkSqlId, ang, dist);

	BlueSystemMessage(m_session, "%s linked up to %s with a distance of %f at %f radians.", slave->GetCreatureName()->Name, 
		static_cast< Creature* >( m_session->GetPlayer()->linkTarget )->GetCreatureName()->Name, dist, ang );

	return true;
}

bool ChatHandler::HandleNpcFollowCommand(const char* args, WorldSession * m_session)
{
	Creature * creature = getSelectedCreature(m_session, true);
	if(!creature) return true;

	creature->GetAIInterface()->SetUnitToFollow(m_session->GetPlayer());
	creature->GetAIInterface()->SetFollowDistance(3.0f);
	return true;
}

bool ChatHandler::HandleFormationClearCommand(const char* args, WorldSession * m_session)
{
	Creature * c = getSelectedCreature(m_session, true);
	if(!c) return true;

	c->GetAIInterface()->m_formationLinkSqlId = 0;
	c->GetAIInterface()->m_formationLinkTarget = 0;
	c->GetAIInterface()->m_formationFollowAngle = 0.0f;
	c->GetAIInterface()->m_formationFollowDistance = 0.0f;
	c->GetAIInterface()->SetUnitToFollow(0);
	
	WorldDatabase.Execute("DELETE FROM creature_formations WHERE spawn_id=%u", c->GetSQL_id());
	return true;
}

bool ChatHandler::HandleNullFollowCommand(const char* args, WorldSession * m_session)
{
	Creature * c = getSelectedCreature(m_session, true);
	if(!c) return true;

	// restart movement
	c->GetAIInterface()->SetAIState(STATE_IDLE);
	c->GetAIInterface()->SetUnitToFollow(0);
	return true;
}

bool ChatHandler::HandleStackCheatCommand(const char* args, WorldSession * m_session)
{
	Player * plyr = getSelectedChar(m_session, true);
	if(!plyr) return true;

	bool val = plyr->stack_cheat;
	BlueSystemMessage(m_session, "%s aura stack cheat on %s.", val ? "Deactivating" : "Activating", plyr->GetName());
	GreenSystemMessageToPlr(plyr, "%s %s an aura stack cheat on you.", m_session->GetPlayer()->GetName(), val ? "deactivated" : "activated");

	plyr->stack_cheat = !val;
	sGMLog.writefromsession(m_session, "used stack cheat on %s", plyr->GetName());
	return true;
}

bool ChatHandler::HandleTriggerpassCheatCommand(const char* args, WorldSession * m_session)
{
	Player * plr = getSelectedChar(m_session, true);
	if (!plr)
		return true;

	bool val = plr->triggerpass_cheat;
	BlueSystemMessage(m_session, "%s areatrigger prerequisites immunity cheat on %s.", val ? "Deactivated" : "Activated", plr->GetName());
	GreenSystemMessageToPlr(plr, "%s %s areatrigger prerequisites immunity cheat on you.", m_session->GetPlayer()->GetName(), val ? "deactivated" : "activated");

	plr->triggerpass_cheat = !val;
	sGMLog.writefromsession(m_session, "used areatrigger cheat on %s", plr->GetName());
	return true;
}

bool ChatHandler::HandleResetSkillsCommand(const char* args, WorldSession * m_session)
{
	skilllineentry * se;
	Player * plr = getSelectedChar(m_session, true);
	if(!plr) return true;

	plr->_RemoveAllSkills();

	// Load skills from create info.
	PlayerCreateInfo * info = objmgr.GetPlayerCreateInfo(plr->getRace(), plr->getClass());
	if(!info) return true;

	for(std::list<CreateInfo_SkillStruct>::iterator ss = info->skills.begin(); ss!=info->skills.end(); ss++)
	{
		se = dbcSkillLine.LookupEntry(ss->skillid);
		if(se->type != SKILL_TYPE_LANGUAGE && ss->skillid && ss->currentval && ss->maxval)
			plr->_AddSkillLine(ss->skillid, ss->currentval, ss->maxval);		
	}
	//Chances depend on stats must be in this order!
	plr->UpdateStats();
	plr->UpdateChances();
	plr->_UpdateMaxSkillCounts();
	plr->_AddLanguages(false);
	BlueSystemMessage(m_session, "Reset skills to default.");
	sGMLog.writefromsession(m_session, "Les skills de %s ont ete remis a leur valeur par defaut", plr->GetName());
	return true;
}

bool ChatHandler::HandlePlayerInfo(const char* args, WorldSession * m_session)
{
	Player * plr;
	if(strlen(args) >= 2) // char name can be 2 letters
	{
		plr = objmgr.GetPlayer(args, false);
		if(!plr)
		{
			RedSystemMessage(m_session, "Unable to locate player %s.", args);		
			return true;
		}
	}
	else
		plr = getSelectedChar(m_session, true);
	
	if(!plr) return true;
	if(!plr->GetSession())
	{
		RedSystemMessage(m_session, "ERROR: this player hasn't got any session !");
		return true;
	}
	if(!plr->GetSession()->GetSocket())
	{
		RedSystemMessage(m_session, "ERROR: this player hasn't got any socket !");
		return true;
	}
	WorldSession* sess = plr->GetSession();

//	char* infos = new char[128];
	static const char* classes[12] =
	{"None","Warrior", "Paladin", "Hunter", "Rogue", "Priest", "DeathKnight", "Shaman", "Mage", "Warlock", "None", "Druid"};
	static const char* races[12] =
	{"None","Human","Orc","Dwarf","Night Elf","Undead","Tauren","Gnome","Troll","None","Blood Elf","Draenei"};

	char playedLevel[64];
	char playedTotal[64];

	int seconds = (plr->GetPlayedtime())[0];
	int mins=0;
	int hours=0;
	int days=0;
	if(seconds >= 60)
	{
		mins = seconds / 60;
		if(mins)
		{
			seconds -= mins*60;
			if(mins >= 60)
			{
				hours = mins / 60;
				if(hours)
				{
					mins -= hours*60;
					if(hours >= 24)
					{
						days = hours/24;
						if(days)
							hours -= days*24;
					}
				}
			}
		}
	}
	snprintf(playedLevel, 64, "[%d days, %d hours, %d minutes, %d seconds]", days, hours, mins, seconds);

	seconds = (plr->GetPlayedtime())[1];
	mins=0;
	hours=0;
	days=0;
	if(seconds >= 60)
	{
		mins = seconds / 60;
		if(mins)
		{
			seconds -= mins*60;
			if(mins >= 60)
			{
				hours = mins / 60;
				if(hours)
				{
					mins -= hours*60;
					if(hours >= 24)
					{
						days = hours/24;
						if(days)
							hours -= days*24;
					}
				}
			}
		}
	}
	snprintf(playedTotal, 64, "[%d days, %d hours, %d minutes, %d seconds]", days, hours, mins, seconds);

	GreenSystemMessage(m_session, "%s is a %s %s %s", plr->GetName(),
		(plr->getGender()?"Female":"Male"), races[plr->getRace()], classes[plr->getClass()]);

	BlueSystemMessage(m_session, "%s has played %s at this level",(plr->getGender()?"She":"He"), playedLevel);
	BlueSystemMessage(m_session, "and %s overall", playedTotal);

	BlueSystemMessage(m_session, "%s is connecting from account '%s'[%u] with permissions '%s'",
		(plr->getGender()?"She":"He"), sess->GetAccountName().c_str(), sess->GetAccountId(), sGMSecurityMgr.Serialize(sess->GetPermissions()).c_str());

	if(sess->HasFlag(ACCOUNT_FLAG_XPACK_02))
		BlueSystemMessage(m_session, "%s uses Wrath of the Lich King (build %u)", (plr->getGender()?"She":"He"),sess->GetClientBuild() );
	else if(sess->HasFlag(ACCOUNT_FLAG_XPACK_01))
		BlueSystemMessage(m_session, "%s uses WoW Burning Crusade (build %u)", (plr->getGender()?"She":"He"),sess->GetClientBuild());
	else 	
		BlueSystemMessage(m_session, "%s uses World of Warcraft (build %u)", (plr->getGender()?"She":"He"),sess->GetClientBuild());

	BlueSystemMessage(m_session, "%s IP is '%s', and has a latency of %ums", (plr->getGender()?"Her":"His"),
		sess->GetSocket()->GetRemoteIP().c_str(), sess->GetLatency());

	return true;
}

bool ChatHandler::HandleGlobalPlaySoundCommand(const char* args, WorldSession * m_session)
{
	if(!*args) return false;
	uint32 sound = atoi(args);
	if(!sound) return false;

	WorldPacket data(SMSG_PLAY_SOUND, 4);
	data << sound;
	sWorld.SendGlobalMessage(&data, 0);
	BlueSystemMessage(m_session, "Broadcasted sound %u to server.", sound);
	sGMLog.writefromsession(m_session, "used play all command soundid %u", sound);
	return true;
}

bool ChatHandler::HandleIPBanCommand(const char * args, WorldSession * m_session)
{
	char * pIp = (char*)args;
	char * pDuration = strchr(pIp, ' ');
	if(pDuration == NULL)
		return false;
	*pDuration = 0;
	++pDuration;

	int32 timeperiod = GetTimePeriodFromString(pDuration);
	if(timeperiod < 1)
		return false;

	uint32 o1, o2, o3, o4;
	if ( sscanf(pIp, "%3u.%3u.%3u.%3u", (unsigned int*)&o1, (unsigned int*)&o2, (unsigned int*)&o3, (unsigned int*)&o4) != 4
			|| o1 > 255 || o2 > 255 || o3 > 255 || o4 > 255)
	{
		RedSystemMessage(m_session, "Invalid IPv4 address [%s]", pIp);
		return true;	// error in syntax, but we wont remind client of command usage
	}

	time_t expire_time;
	if ( timeperiod == 0)		// permanent ban
		expire_time = 0;
	else
		expire_time = UNIXTIME + (time_t)timeperiod;
	string IP = pIp;
	string::size_type i = IP.find("/");
	if (i == string::npos)
	{
		RedSystemMessage(m_session, "Lack of CIDR address assumes a 32bit match (if you don't understand, dont worry, it worked)");
		IP.append("/32");
		pIp = (char*)IP.c_str(); //is this correct? - optical
	}
	SystemMessage(m_session, "Adding [%s] to IP ban table, expires %s", pIp, (expire_time == 0)? "Never" : ctime( &expire_time ));
	sLogonCommHandler.IPBan_Add( pIp, (uint32)expire_time );
	sWorld.DisconnectUsersWithIP(IP.substr(0,IP.find("/")).c_str(), m_session);
	sGMLog.writefromsession(m_session, "banned ip address %s, expires %s", pIp, (expire_time == 0)? "Never" : ctime( &expire_time ));
	return true;
}

bool ChatHandler::HandleIPUnBanCommand(const char * args, WorldSession * m_session)
{
	string pIp = args;
	if (pIp.length() == 0)
		return false;

	if (pIp.find("/") == string::npos)
	{
		RedSystemMessage(m_session, "Lack of CIDR address assumes a 32bit match (if you don't understand, dont worry, it worked)");
		pIp.append("/32");
	}
	/**
	 * We can afford to be less fussy with the validty of the IP address given since
	 * we are only attempting to remove it.
	 * Sadly, we can only blindly execute SQL statements on the logonserver so we have
	 * no idea if the address existed and so the account/IPBanner cache requires reloading.
	 */

	SystemMessage(m_session,"Deleting [%s] from ip ban table if it exists",pIp.c_str());
	sLogonCommHandler.IPBan_Remove( pIp.c_str() );
	sGMLog.writefromsession(m_session, "unbanned ip address %s", pIp.c_str());
	return true;
}

bool ChatHandler::HandleCreatureSpawnCommand(const char *args, WorldSession *m_session)
{
	uint32 entry = atol(args);
	if(entry == 0)
		return false;

	CreatureProto * proto = CreatureProtoStorage.LookupEntry(entry);
	CreatureInfo * info = CreatureNameStorage.LookupEntry(entry);
	if(proto == 0 || info == 0)
	{
		RedSystemMessage(m_session, "Invalid entry id.");
		return true;
	}

	CreatureSpawn * sp = new CreatureSpawn;
	//sp->displayid = info->DisplayID;
	info->GenerateModelId(&sp->displayid);
	sp->entry = entry;
	sp->form = 0;
	sp->id = objmgr.GenerateCreatureSpawnID();
	sp->movetype = 0;
	sp->x = m_session->GetPlayer()->GetPositionX();
	sp->y = m_session->GetPlayer()->GetPositionY();
	sp->z = m_session->GetPlayer()->GetPositionZ();
	sp->o = m_session->GetPlayer()->GetOrientation();
	sp->emote_state =0;
	sp->flags = 0;
	sp->factionid = proto->Faction;
	sp->bytes0=0;
	sp->bytes1=0;
	sp->bytes2=0;
	//sp->respawnNpcLink = 0;
	sp->stand_state = 0;
	sp->phase   = NORMAL_PHASING;
	sp->vehicle = 0;
	sp->channel_spell=sp->channel_target_creature=sp->channel_target_go=0;


	Creature * p = m_session->GetPlayer()->GetMapMgr()->CreateCreature(entry);
	ASSERT(p);
	p->Load(sp, (uint32)NULL, NULL);
	p->PushToWorld(m_session->GetPlayer()->GetMapMgr());
	
	uint32 x = m_session->GetPlayer()->GetMapMgr()->GetPosX(m_session->GetPlayer()->GetPositionX());
	uint32 y = m_session->GetPlayer()->GetMapMgr()->GetPosY(m_session->GetPlayer()->GetPositionY());

	// Add spawn to map
	m_session->GetPlayer()->GetMapMgr()->GetBaseMap()->GetSpawnsListAndCreate(
		x,
		y)->CreatureSpawns.push_back(sp);

	BlueSystemMessage(m_session, "Spawned a creature `%s` with entry %u at %f %f %f on map %u", info->Name, 
		entry, sp->x, sp->y, sp->z, m_session->GetPlayer()->GetMapId());

	// Save it to the database.
	p->SaveToDB();

	sGMLog.writefromsession(m_session, "spawned a %s at %u %f %f %f", info->Name, m_session->GetPlayer()->GetMapId(),sp->x,sp->y,sp->z);

	return true;
}

bool ChatHandler::HandleCreatureSetPhasing(const char *args, WorldSession *m_session)
{
	int32 phasing = 0;
	uint32 save_to_db = 0;

	if (!*args) return false;

	sscanf(args, "%d %u", &phasing, &save_to_db);

	if((phasing < ALL_PHASES) && (phasing >= MAX_PHASEMODE) )
	{
		SystemMessage(m_session,"Phasing : ALL_PHASES = -1");
		SystemMessage(m_session,"          NO_PHASING = 0 "); 
		SystemMessage(m_session,"          PHASE_x    = de 1 a 10");
		return true;
	}

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	Creature * pCreature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}

	
	pCreature->SetPhase(phasing);
	if(save_to_db) pCreature->SaveToDB();
	    
	SystemMessage(m_session, "Nouveau Phasing Ok (%d), Sauvegarde: %s",(save_to_db)?"Oui":"Non");

	return true;
}


bool ChatHandler::HandleRemoveItemCommand(const char * args, WorldSession * m_session)
{
	uint32 item_id;
	int32 count, ocount;
	int argc = sscanf(args, "%u %u", (unsigned int*)&item_id, (unsigned int*)&count);
	if(argc == 1)
		count = 1;
	else if(argc != 2 || !count)
		return false;

	ocount = count;
	Player * plr = getSelectedChar(m_session, true);
	if(!plr) return true;

	// loop until they're all gone.
	int32 loop_count = 0;
	int32 start_count = plr->GetItemInterface()->GetItemCount(item_id, true);
	int32 start_count2 = start_count;
	if(count > start_count)
		count = start_count;

	while(start_count >= count && (count > 0) && loop_count < 20)	 // Prevent a loop here.
	{
		plr->GetItemInterface()->RemoveItemAmt(item_id, count);
		start_count2 = plr->GetItemInterface()->GetItemCount(item_id, true);
		count -= (start_count - start_count2);
		start_count = start_count2;
		++loop_count;
	}

	sGMLog.writefromsession(m_session, "used remove item id %u count %u from %s", item_id, ocount, plr->GetName());
	BlueSystemMessage(m_session, "Removing %u copies of item %u from %s's inventory.", ocount, item_id, plr->GetName());
	BlueSystemMessage(plr->GetSession(), "%s removed %u copies of item %u from your inventory.", m_session->GetPlayer()->GetName(), ocount, item_id);
	return true;
}

bool ChatHandler::HandleForceRenameCommand(const char * args, WorldSession * m_session)
{
	// prevent buffer overflow
	if(strlen(args) > 100)
		return false;

	string tmp = string(args);
	PlayerInfo * pi = objmgr.GetPlayerInfoByName(tmp.c_str());
	if(pi == 0)
	{
		RedSystemMessage(m_session, "Player with that name not found.");
		return true;
	}

	Player * plr = objmgr.GetPlayer((uint32)pi->guid);
	if(plr == 0)
	{
		CharacterDatabase.Execute("UPDATE characters SET forced_rename_pending = 1 WHERE guid = %u", (uint32)pi->guid);
	}
	else
	{
		plr->rename_pending = true;
		plr->SaveToDB(false);
		BlueSystemMessageToPlr(plr, "%s forced your character to be renamed next logon.", m_session->GetPlayer()->GetName());
	}

	CharacterDatabase.Execute("INSERT INTO banned_names VALUES('%s')", CharacterDatabase.EscapeString(string(pi->name)).c_str());
	GreenSystemMessage(m_session, "Forcing %s to rename his character next logon.", args);
	sGMLog.writefromsession(m_session, "forced %s to rename his charater (%u)", pi->name, pi->guid);
	return true;
}

bool ChatHandler::HandleGetStandingCommand(const char * args, WorldSession * m_session)
{
	uint32 faction = atoi(args);
	Player * plr = getSelectedChar(m_session, true);
	if(!plr) return true;

	int32 standing = plr->GetStanding(faction);
	int32 bstanding = plr->GetBaseStanding(faction);

	GreenSystemMessage(m_session, "Reputation for faction %u:", faction);
	SystemMessage(m_session,	  "   Base Standing: %d", bstanding);
	BlueSystemMessage(m_session,  "   Standing: %d", standing);
	return true;
}

bool ChatHandler::HandleSetStandingCommand(const char * args, WorldSession * m_session)
{
	uint32 faction;
	int32 standing;
	if(sscanf(args, "%u %d", (unsigned int*)&faction, (unsigned int*)&standing) != 2)
		return false;
	Player * plr = getSelectedChar(m_session, true);
	if(!plr) return true;;

	BlueSystemMessage(m_session, "Setting standing of %u to %d on %s.", faction, standing, plr->GetName());
	plr->SetStanding(faction, standing);
	GreenSystemMessageToPlr(plr, "%s set your standing of faction %u to %d.", m_session->GetPlayer()->GetName(), faction, standing);
	sGMLog.writefromsession(m_session, "set standing of faction %u to %u for %s", faction,standing,plr->GetName());
	return true;
}

void SendHighlightedName(WorldSession * m_session, char* full_name, string& lowercase_name, string& highlight, uint32 id, bool item)
{
	char message[1024];
	char start[50];
	start[0] = message[0] = 0;
	
	snprintf(start, 50, "%s %u: %s", item ? "Item" : "Creature", (unsigned int)id, MSG_COLOR_WHITE);

	string::size_type hlen = highlight.length();
	string fullname = string(full_name);
	string::size_type offset = lowercase_name.find(highlight);
	string::size_type remaining = fullname.size() - offset - hlen;
	strcat(message, start);
	strncat(message, fullname.c_str(), offset);
	if(remaining > 0)
	{
		strcat(message, MSG_COLOR_LIGHTRED);
		strncat(message, (fullname.c_str() + offset), hlen);
		strcat(message, MSG_COLOR_WHITE);
		strncat(message, (fullname.c_str() + offset + hlen), remaining);
	}

	sChatHandler.SystemMessage(m_session, message);
}

void SendHighlightedSpellName(WorldSession * m_session, char* full_name, string& lowercase_name, string& highlight, uint32 id, uint32 level)
{
	char message[1024];
	char start[50];
	char end[10];
	start[0] = message[0] = 0;
	
	snprintf(start, 50, "Spell %u: %s", (unsigned int)id, MSG_COLOR_WHITE);
	snprintf(end, 10, " (%u)", (unsigned int)level);

	string::size_type hlen = highlight.length();
	string fullname = string(full_name);
	string::size_type offset = lowercase_name.find(highlight);
	string::size_type remaining = fullname.size() - offset - hlen;
	strcat(message, start);
	strncat(message, fullname.c_str(), offset);
	if(remaining > 0)
	{
		strcat(message, MSG_COLOR_LIGHTRED);
		strncat(message, (fullname.c_str() + offset), hlen);
		strcat(message, MSG_COLOR_WHITE);
		strncat(message, (fullname.c_str() + offset + hlen), remaining);
	}
	strcat(message, end);

	sChatHandler.SystemMessage(m_session, message);
}

void SendHighlightedSkillName(WorldSession * m_session, char* full_name, string& lowercase_name, string& highlight, uint32 id, uint32 maxV)
{
	char message[1024];
	char start[50];
	start[0] = message[0] = 0;
	
	snprintf(start, 50, "Skill %u: (%u) %s",(unsigned int)id, maxV, MSG_COLOR_WHITE);

	string::size_type hlen = highlight.length();
	string fullname = string(full_name);
	string::size_type offset = lowercase_name.find(highlight);
	string::size_type remaining = fullname.size() - offset - hlen;
	strcat(message, start);
	strncat(message, fullname.c_str(), offset);
	if(remaining > 0)
	{
		strcat(message, MSG_COLOR_LIGHTRED);
		strncat(message, (fullname.c_str() + offset), hlen);
		strcat(message, MSG_COLOR_WHITE);
		strncat(message, (fullname.c_str() + offset + hlen), remaining);
	}

	sChatHandler.SystemMessage(m_session, message);
}
/// Attention il s'agit d'une version simplifiée du is_numeric. 
/// Pour la version complète il faut utiliser les fonctions spécifiques du framework.
bool is_numeric(std::string str)
{
	WDFK::set<char> digits;
	digits << "0123456789";
	return !str.empty() && digits.find(str[0]);
}
bool ChatHandler::HandleLookupItemCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	string x = string(args);
	ASCENT_TOLOWER(x);
	if(x.length() < 4)
	{
		RedSystemMessage(m_session, "Your search string must be at least 5 characters long.");
		return true;
	}

	StorageContainerIterator<ItemPrototype> * itr = ItemPrototypeStorage.MakeIterator();

	BlueSystemMessage(m_session, "Starting search of item `%s`...", x.c_str());
	uint32 t = getMSTime();
	ItemPrototype * it;
	uint32 count = 0;
	while(!itr->AtEnd())
	{
		it = itr->Get();
		if(FindXinYString(x, it->lowercase_name))
		{
			// Print out the name in a cool highlighted fashion
			SendHighlightedName(m_session, it->Name1, it->lowercase_name, x, it->ItemId, true);
			++count;
			if(count == 25)
			{
				RedSystemMessage(m_session, "More than 25 results returned. aborting.");
				break;
			}
		}
		
		if(!itr->Inc())
			break;
	}
	itr->Destruct();

	BlueSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);
	return true;
}

bool ChatHandler::HandleLookupCreatureCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	string x = string(args);
	ASCENT_TOLOWER(x);
	if(x.length() < 4)
	{
		RedSystemMessage(m_session, "Your search string must be at least 5 characters long.");
		return true;
	}

	StorageContainerIterator<CreatureInfo> * itr = CreatureNameStorage.MakeIterator();

	GreenSystemMessage(m_session, "Starting search of creature `%s`...", x.c_str());
	uint32 t = getMSTime();
	CreatureInfo * i;
	uint32 count = 0;
	while(!itr->AtEnd())
	{
		i = itr->Get();
		if(FindXinYString(x, i->lowercase_name))
		{
			// Print out the name in a cool highlighted fashion
			SendHighlightedName(m_session, i->Name, i->lowercase_name, x, i->Id, false);

			++count;
			if(count == 25)
			{
				RedSystemMessage(m_session, "More than 25 results returned. aborting.");
				break;
			}
		}
		if(!itr->Inc())
			break;
	}
	itr->Destruct();

	GreenSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);
	return true;
}
// cinemania
bool ChatHandler::HandleLookupSpellCommand(const char* args, WorldSession* m_session)
{
	if(!*args) return false;

	string x = string(args);
	ASCENT_TOLOWER(x);

	if (x.empty())
	{
		RedSystemMessage(m_session, "Votre chaine/id de recherche doit faire au moins 1 caractere.");
		return true;
	}

	BlueSystemMessage(m_session, "Debut de recherche du sort `%s`...", x.c_str());
	uint32 t = getMSTime();
	/// test si on fait une recherche par id ou par nom
	if (atoi(x.c_str()) != 0) 
	{
		/// Par spell ID
		uint32 spellId = (uint32)atoi(x.c_str());
		SpellEntry *spell = dbcSpell.LookupEntryForced(spellId);
		if (spell != NULL)
		{
			GreenSystemMessage(m_session, "%u : %s (%u)", spell->Id, spell->Name, spell->spellLevel);
		}
		else
			RedSystemMessage(m_session, "Il n'existe pas de sort ayant l'ID %s..", args);
	}
	else
	{
		/// Par nom
		uint32 count = 0;
		for (uint32 i = 0; i < dbcSpell.GetNumRows(); ++i)
		{
			SpellEntry* spell = dbcSpell.LookupRow(i);
			std::string sln = spell->Name;
			ASCENT_TOLOWER(sln);

			if(FindXinYString(x, sln))
			{
				SendHighlightedSpellName(m_session, spell->Name, sln, x, spell->Id, spell->spellLevel);

				++count;
				if(count == 25)
				{
					RedSystemMessage(m_session, "Plus de 25 resultats retournes.");
					break;
				}
			}
		}
	}
	BlueSystemMessage(m_session, "Recherche completee en %u ms.", getMSTime() - t);
	return true;
}

bool ChatHandler::HandleLookupAchievementCommand(const char * args, WorldSession * m_session)
{
	if(!*args) 
		return false;

	string x = string(args);
	ASCENT_TOLOWER(x);

	if (x.empty())
	{
		RedSystemMessage(m_session, "Votre chaine/id de recherche doit faire au moins 1 caractere.");
		return true;
	}

	BlueSystemMessage(m_session, "Debut de recherche du haut-fait `%s`...", x.c_str());
	uint32 t = getMSTime();

	if (atoi(x.c_str()) != 0)
	{
		/// Par ID
		uint32 achID = (uint32)atoi(x.c_str());
		AchievementEntry * ae = dbcAchievement.LookupEntryForced(achID);
		if (ae != NULL)
			GreenSystemMessage(m_session, "%u - %s.", ae->ID, ae->name);
		else
			RedSystemMessage(m_session, "Il n'existe pas de haut-fait ayant l'ID %s.", args);
	}
	else
	{
		/// Par nom
		uint32 count = 0;
		for (uint32 i = 0; i < dbcAchievement.GetNumRows(); ++i)
		{
			AchievementEntry * ae = dbcAchievement.LookupRow(i);
			std::string sln = ae->name;
			ASCENT_TOLOWER(sln);

			if(FindXinYString(x, sln))
			{
				BlueSystemMessage(m_session,"%u - %s.", ae->ID, ae->name);

				++count;
				if(count == 25)
				{
					RedSystemMessage(m_session, "Plus de 25 resultats retournes.");
					break;
				}
			}
		}
	}
	BlueSystemMessage(m_session, "Recherche completee en %u ms.", getMSTime() - t);
	return true;
}

bool ChatHandler::HandleGORotate(const char * args, WorldSession * m_session)
{
	// ArcEmu
	char Axis;
	float deg;
	uint32 RecDB = 0;
	int _Argv;  

	_Argv = sscanf(args, "%c %f %u", &Axis, &deg, &RecDB);

	if(_Argv <=1)
	{ // .gobject rotate axe valeur [0:1]
	 RedSystemMessage(m_session, "Usage: .gobject rotate <axe,x:y:o> <valeur_degre> [0:1]...");
	 return true;
    }
    if(_Argv <=2) RecDB = 0;

	GameObject *go = m_session->GetPlayer()->m_GM_SelectedGO;
	if( !go )
	{   
		RedSystemMessage(m_session, "Pas de GameObject selectionné...");
		return true;
	}

    //if(deg == 0.0f) return false;

	// Convert the argument to radians
	float rad = deg * (float(M_PI) / 180.0f);

    switch(tolower(Axis))
	{
	case 'x':
		go->ModFloatValue(GAMEOBJECT_PARENTROTATION, rad);
		break;
	case 'y':
		go->ModFloatValue(GAMEOBJECT_PARENTROTATION_1, rad);
		break;
	case 'o':
		if(m_session->GetPlayer()){
			float ori = m_session->GetPlayer()->GetOrientation();
			go->SetFloatValue(GAMEOBJECT_PARENTROTATION_2, sinf(ori / 2));
			go->SetFloatValue(GAMEOBJECT_PARENTROTATION_3, cosf(ori / 2));}
		break;
	default:
		RedSystemMessage(m_session, "Axe Invalide");
		RedSystemMessage(m_session, "Usage: .gobject rotate axe (x:y:o) valeur [0:1]...");
		return true;
	}
	//---------
	// despawn and respawn
	//go->Despawn(1000);
    uint32 NewGuid = m_session->GetPlayer()->GetMapMgr()->GenerateGameobjectGuid();
	go->RemoveFromWorld(true);
	go->SetNewGuid(NewGuid);
	if(RecDB != 0) 
	{
		go->SaveToDB(); // Apres le NewGuid ??
		RedSystemMessage(m_session, "Nouvelle rotation sauvegarde!");
	}
	else RedSystemMessage(m_session, "Rotation non sauvegarde!");

	go->PushToWorld(m_session->GetPlayer()->GetMapMgr());

	m_session->GetPlayer()->m_GM_SelectedGO = go;
	return true;
}

bool ChatHandler::HandleGOMove(const char * args, WorldSession * m_session)
{
	// move the go to player's coordinates
	GameObject *go = m_session->GetPlayer()->m_GM_SelectedGO;
	if( !go )
	{
		RedSystemMessage(m_session, "No selected GameObject...");
		return true;
	}

	go->RemoveFromWorld(true);
	go->SetPosition(m_session->GetPlayer()->GetPosition());
	// disparu 332.11403
	//go->SetFloatValue(GAMEOBJECT_POS_X, m_session->GetPlayer()->GetPositionX());
	//go->SetFloatValue(GAMEOBJECT_POS_Y, m_session->GetPlayer()->GetPositionY());
	//go->SetFloatValue(GAMEOBJECT_POS_Z, m_session->GetPlayer()->GetPositionZ());
	//go->SetFloatValue(GAMEOBJECT_FACING, m_session->GetPlayer()->GetOrientation());
	go->SetNewGuid(m_session->GetPlayer()->GetMapMgr()->GenerateGameobjectGuid());
	go->SaveToDB();
	go->PushToWorld(m_session->GetPlayer()->GetMapMgr());
	return true;
}

bool ChatHandler::HandleNpcPossessCommand(const char * args, WorldSession * m_session)
{
	Unit * pTarget = getSelectedChar(m_session, false);
	if(!pTarget)
	{
		pTarget = getSelectedCreature(m_session, false);
		if(pTarget && (pTarget->IsPet() || pTarget->GetUInt32Value(UNIT_FIELD_CREATEDBY) != 0))
			return false;
	}
		
	if(!pTarget)
	{
		RedSystemMessage(m_session, "You must select a player/creature.");
		return true;
	}

	m_session->GetPlayer()->Possess(pTarget);
	BlueSystemMessage(m_session, "Possessed "I64FMT, pTarget->GetGUID());
	return true;
}

bool ChatHandler::HandleNpcUnPossessCommand(const char * args, WorldSession * m_session)
{
	Creature * creature = getSelectedCreature(m_session);
 	m_session->GetPlayer()->UnPossess();

	if(creature)
	{
			// restart movement
			creature->GetAIInterface()->SetAIState(STATE_IDLE);
			creature->GetAIInterface()->WipeHateList();
			creature->GetAIInterface()->WipeTargetList();

			if(creature->m_spawn)
			{
				// return to respawn coords
				float x = creature->m_spawn->x;
				float y = creature->m_spawn->y;
				float z = creature->m_spawn->z;
				float o = creature->m_spawn->o;
				creature->GetAIInterface()->MoveTo(x, y, z, o);
			}
	}
	GreenSystemMessage(m_session, "Removed any possessed targets.");
	return true;
}

bool ChatHandler::HandleRehashCommand(const char * args, WorldSession * m_session)
{
	/* rehashes */
	char msg[250];
	snprintf(msg, 250, "%s is rehashing config file.", m_session->GetPlayer()->GetName());
	sWorld.SendWorldWideScreenText(msg, 0);
	sWorld.SendWorldText(msg, 0);
	sWorld.Rehash(true);
	return true;
}

struct spell_thingo
{
	uint32 type;
	uint32 target;
};

list<SpellEntry*> aiagent_spells;
map<uint32, spell_thingo> aiagent_extra;

bool ChatHandler::HandleAIAgentDebugBegin(const char * args, WorldSession * m_session)
{
	QueryResult * result = WorldDatabase.Query("SELECT DISTINCT spell FROM ai_agents");
	if(!result) return false;

	do 
	{
		SpellEntry * se = dbcSpell.LookupEntryForced(result->Fetch()[0].GetUInt32());
		if(se)
			aiagent_spells.push_back(se);
	} while(result->NextRow());
	delete result;

	for(list<SpellEntry*>::iterator itr = aiagent_spells.begin(); itr != aiagent_spells.end(); ++itr)
	{
		result = WorldDatabase.Query("SELECT * FROM ai_agents WHERE spell = %u", (*itr)->Id);
		ASSERT(result);
		spell_thingo t;
		t.type = result->Fetch()[6].GetUInt32();
		t.target = result->Fetch()[7].GetUInt32();
		delete result;
		aiagent_extra[(*itr)->Id] = t;
	}

	GreenSystemMessage(m_session, "Loaded %u spells for testing.", aiagent_spells.size());
	return true;
}

SpellCastTargets SetTargets(SpellEntry * sp, uint32 type, uint32 targettype, Unit * dst, Creature * src)
{
	SpellCastTargets targets;
	targets.m_target = 0;
	targets.m_srcX = 0;
	targets.m_srcY = 0;
	targets.m_srcZ = 0;
	targets.m_destX = 0;
	targets.m_destY = 0;
	targets.m_destZ = 0;

	if(targettype == TTYPE_SINGLETARGET) 
	{
		targets.m_targetMask = TARGET_FLAG_UNIT; // 2;
		targets.m_target = dst;
	}
	else if(targettype == TTYPE_SOURCE) 
	{
		targets.m_targetMask = TARGET_FLAG_SOURCE_LOCATION; // 32;
		targets.m_srcX = src->GetPositionX();
		targets.m_srcY = src->GetPositionY();
		targets.m_srcZ = src->GetPositionZ();
	}
	else if(targettype == TTYPE_DESTINATION)  
	{
		targets.m_targetMask = TARGET_FLAG_DEST_LOCATION; // 64;
		targets.m_destX = dst->GetPositionX();
		targets.m_destY = dst->GetPositionY();
		targets.m_destZ = dst->GetPositionZ();
	}

	return targets;
};

bool ChatHandler::HandleAIAgentDebugContinue(const char * args, WorldSession * m_session)
{
	uint32 count = atoi(args);
	if(!count) return false;

	Creature * pCreature = getSelectedCreature(m_session, true);
	if(!pCreature) return true;

	Player * pPlayer = m_session->GetPlayer();

	for(uint32 i = 0; i < count; ++i)
	{
		if(!aiagent_spells.size())
			break;

		SpellEntry * sp = *aiagent_spells.begin();
		aiagent_spells.erase(aiagent_spells.begin());
		BlueSystemMessage(m_session, "Casting %u, "MSG_COLOR_SUBWHITE"%u remaining.", sp->Id, aiagent_spells.size());

		map<uint32, spell_thingo>::iterator it = aiagent_extra.find(sp->Id);
		ASSERT(it != aiagent_extra.end());

		SpellCastTargets targets;
		if(it->second.type == STYPE_BUFF)
			targets = SetTargets(sp, it->second.type, it->second.type, pCreature, pCreature );
		else
			targets = SetTargets(sp, it->second.type, it->second.type, pPlayer, pCreature );

		pCreature->GetAIInterface()->CastSpell(pCreature, sp, targets);
	}

	if(!aiagent_spells.size())
		RedSystemMessage(m_session, "Finished.");
	/*else
		BlueSystemMessage(m_session, "Got %u remaining.", aiagent_spells.size());*/
	return true;
}

bool ChatHandler::HandleAIAgentDebugSkip(const char * args, WorldSession * m_session)
{
	uint32 count = atoi(args);
	if(!count) return false;

	for(uint32 i = 0; i < count; ++i)
	{
		if(!aiagent_spells.size())
			break;

		aiagent_spells.erase(aiagent_spells.begin());
	}
	BlueSystemMessage(m_session, "Erased %u spells.", count);
	return true;
}

bool ChatHandler::HandleRenameGuildCommand(const char* args, WorldSession *m_session)
{
	Player * plr = getSelectedChar(m_session);
	if(!plr || !plr->GetGuildId() || !args || !strlen(args)) return false;


	return true;
}

//People seem to get stuck in guilds from time to time. This should be helpfull. -DGM
bool ChatHandler::HandleGuildRemovePlayerCommand(const char* args, WorldSession *m_session)
{
	Player * plr = getSelectedChar(m_session);
	if(!plr || !plr->GetGuildId()) return false;
	return true;
}

//-DGM
bool ChatHandler::HandleGuildDisbandCommand(const char* args, WorldSession *m_session)
{
	Player * plr = getSelectedChar(m_session);
	if(!plr || !plr->GetGuildId() || !args || !strlen(args)) return false;
	return true;
}

//-DGM
bool ChatHandler::HandleGuildMembersCommand(const char* args, WorldSession *m_session)
{
	Player * plr = getSelectedChar(m_session);
	if(!plr || !plr->GetGuildId()) return false;
	return true;
}

bool ChatHandler::HandleCreateArenaTeamCommands(const char * args, WorldSession * m_session)
{
	uint32 arena_team_type;
	char name[1000];
	uint32 real_type;
	Player * plr = getSelectedChar(m_session, true);
	if(sscanf(args, "%u %s", &arena_team_type, name) != 2)
	{
		SystemMessage(m_session, "Invalid syntax.");
		return true;
	}

	switch(arena_team_type)
	{
	case 2:
		real_type=0;
		break;
	case 3:
		real_type=1;
		break;
	case 5:
		real_type=2;
		break;
	default:
		SystemMessage(m_session, "Invalid arena team type specified.");
		return true;
	}

	if(!plr)
		return true;

	if(plr->m_arenaTeams[real_type] != NULL)
	{
		SystemMessage(m_session, "Already in an arena team of that type.");
		return true;
	}

	ArenaTeam * t = new ArenaTeam(real_type,objmgr.GenerateArenaTeamId());
	t->m_emblemStyle=22;
	t->m_emblemColour=4292133532UL;
	t->m_borderColour=4294931722UL;
	t->m_borderStyle=1;
	t->m_backgroundColour=4284906803UL;
	t->m_leader=plr->GetLowGUID();
	t->m_name = string(name);
	t->AddMember(plr->m_playerInfo);
	objmgr.AddArenaTeam(t);
	SystemMessage(m_session, "created arena team.");
	return true;
}

bool ChatHandler::HandleWhisperBlockCommand(const char * args, WorldSession * m_session)
{
	if(m_session->GetPlayer()->bGMTagOn)
		return false;

	m_session->GetPlayer()->bGMTagOn = true;
	return true;
}

bool ChatHandler::HandleGenderChanger(const char* args, WorldSession *m_session)
{
	int gender;
	Player* target = objmgr.GetPlayer((uint32)m_session->GetPlayer()->GetSelection());
	if(!target) {
		SystemMessage(m_session, "Select A Player first.");
		return true;
	}
	if (!*args)
	{
		if (target->getGender()== 1)
			gender = 0;
		else
			gender = 1;
	}
	else
		gender = min((int)atoi((char*)args),1);
	target->setGender(gender);
	SystemMessage(m_session, "Gender changed to %u",gender);
	return true;
}

bool ChatHandler::HandleDispelAllCommand(const char * args, WorldSession * m_session)
{
	uint32 pos=0;
	if(*args)
		pos=atoi(args);

	Player * plr;

	sGMLog.writefromsession(m_session, "used dispelall command, pos %u", pos);

	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		plr = itr->second;
		if(plr->GetSession() && plr->IsInWorld())
		{
			if(plr->GetMapMgr() != m_session->GetPlayer()->GetMapMgr())
			{
				sEventMgr.AddEvent( static_cast< Unit* >( plr ), &Unit::DispelAll, pos ? true : false, EVENT_PLAYER_CHECKFORCHEATS, 100, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
			}
			else
			{
				plr->DispelAll(pos?true:false);
			}
		}
	}
	sGMLog.writefromsession(m_session, "used mass dispel");
	objmgr._playerslock.ReleaseReadLock();

	BlueSystemMessage(m_session, "Dispel action done.");
	return true;
}

bool ChatHandler::HandleShowItems(const char * args, WorldSession * m_session)
{
	Player * plr = getSelectedChar(m_session, true);
	if(!plr) return true;

	ItemIterator itr(plr->GetItemInterface());
	itr.BeginSearch();
	for(; !itr.End(); itr.Increment())
	{
		SystemMessage(m_session, "Item %s count %u", (*itr)->GetProto()->Name1, (*itr)->GetUInt32Value(ITEM_FIELD_STACK_COUNT));
	}
	itr.EndSearch();

	SkillIterator itr2(plr);
	itr2.BeginSearch();
	for(; !itr2.End(); itr2.Increment())
	{
		SystemMessage(m_session, "Skill %u %u/%u", itr2->Skill->id, itr2->CurrentValue, itr2->MaximumValue);
	}
	itr2.EndSearch();

	return true;
}

bool ChatHandler::HandleCollisionTestIndoor(const char * args, WorldSession * m_session)
{
#ifdef COLLISION
	Player * plr = m_session->GetPlayer();
	const LocationVector & loc = plr->GetPosition();
	bool res = CollideInterface.IsIndoor(plr->GetMapId(), loc.x, loc.y, loc.z + 2.0f);
	SystemMessage(m_session, "Result was: %s.", res ? "indoors" : "outside");
	return true;
#else
	SystemMessage(m_session, "Ascent was not compiled with collision support.");
	return true;
#endif
}

bool ChatHandler::HandleCollisionTestLOS(const char * args, WorldSession * m_session)
{
#ifdef COLLISION
	float pTaille,cTaille;

	Object * pObj = NULL;
	Creature * pCreature = getSelectedCreature(m_session, false);
	Player * pPlayer = getSelectedChar(m_session, false);

	cTaille = 1; // Pas 0
	if(pCreature)
	{
		pObj = pCreature;
		cTaille = pCreature->Taille * pCreature->GetFloatValue(OBJECT_FIELD_SCALE_X);
	}
	else if(pPlayer)
	{
		pObj = pPlayer;
		cTaille = pPlayer->Taille * pPlayer->GetFloatValue(OBJECT_FIELD_SCALE_X);
	}

	if(pObj == NULL)
	{
		SystemMessage(m_session, "Cible invalide.");
		return true;
	}

	const LocationVector & loc2 = pObj->GetPosition();
	const LocationVector & loc1 = m_session->GetPlayer()->GetPosition();

	pTaille = m_session->GetPlayer()->Taille;

	// Test si le passage est possible entre 2 points dans l'espace, pas d'orientation géré. 
	bool res = CollideInterface.CheckLOS(pObj->GetMapId(), loc1.x, loc1.y, loc1.z+pTaille, loc2.x, loc2.y, loc2.z+cTaille);

	SystemMessage(m_session, "Taille Player %0.2f , Taille Creature %0.2f",pTaille,cTaille);
	SystemMessage(m_session, "Resultat %s .", res ? "in LOS" : "not in LOS");
	return true;
#else
	SystemMessage(m_session, "Ascent was not compiled with collision support.");
	return true;
#endif
}

bool ChatHandler::HandleCollisionGetHeight(const char * args, WorldSession * m_session)
{
#ifdef COLLISION
	Player * plr = m_session->GetPlayer();
	float z = CollideInterface.GetHeight(plr->GetMapId(), plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ() + 2.0f);
	float z2 = CollideInterface.GetHeight(plr->GetMapId(), plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ() + 5.0f);
	float z3 = CollideInterface.GetHeight(plr->GetMapId(), plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ());

	SystemMessage(m_session, "Results were: %f %f %f", z, z2, z3);
	return true;
#else
	SystemMessage(m_session, "Ascent was not compiled with collision support.");
	return true;
#endif
}

bool ChatHandler::HandleLevelUpCommand(const char* args, WorldSession *m_session)
{
	char bufftemp[80];

	Player *plr = getSelectedChar(m_session, true);
	if(!plr) plr = m_session->GetPlayer();
	if(!plr) 
	{
		RedSystemMessage(m_session, "Aucun joueur trouvé !");
		return(false);
	}

	uint32 player_level = plr->getLevel();
	sprintf(bufftemp,"%u",player_level+1);
	if(player_level > sWorld.m_levelCap)
	{
	  RedSystemMessage(m_session, "%s est déja au level maximum (%u)!",plr->GetName(),sWorld.m_levelCap);
	  return(true); // Pas de message
	}
	bool Result = HandleModifyLevelCommand(bufftemp,m_session);
	return(Result);
}

bool ChatHandler::HandleFixScaleCommand(const char * args, WorldSession * m_session)
{
	Creature * pCreature = getSelectedCreature(m_session, true);
	if( pCreature == NULL )
		return true;

	float sc = (float)atof(args);
	if(sc < 0.1f)
	{
		return false;
	}

	pCreature->SetFloatValue(OBJECT_FIELD_SCALE_X, sc);
	pCreature->proto->Scale = sc;
	WorldDatabase.Execute("UPDATE creature_proto SET scale = '%f' WHERE entry = %u", sc, pCreature->GetEntry());
	return true;
}

bool ChatHandler::HandleAddTrainerSpellCommand( const char * args, WorldSession * m_session )
{
	Creature * pCreature = getSelectedCreature(m_session, true);
	if( pCreature == NULL )
		return true;

	uint32 spellid, cost, reqspell, reqlevel, delspell;
	if( sscanf(args, "%u %u %u %u %u", &spellid, &cost, &reqspell, &reqlevel, &delspell) != 5 )
		return false;

	Trainer * pTrainer =  pCreature->GetTrainer();
	if( pTrainer == NULL )
	{
		RedSystemMessage(m_session, "Target is not a trainer.");
		return true;
	}

	SpellEntry* pSpell = dbcSpell.LookupEntryForced(spellid);
	if(pSpell==NULL)
	{
		RedSystemMessage(m_session, "Invalid spell.");
		return true;
	}

	if( pSpell->Effect[0] == SPELL_EFFECT_INSTANT_KILL || pSpell->Effect[1] == SPELL_EFFECT_INSTANT_KILL || pSpell->Effect[2] == SPELL_EFFECT_INSTANT_KILL )
	{
		RedSystemMessage(m_session, "No. You're not doing that.");
		return true;
	}

	TrainerSpell sp;
	sp.Cost = cost;
	sp.IsProfession = false;
	sp.pLearnSpell = pSpell;
	sp.pCastRealSpell = NULL;
	sp.pCastSpell = NULL;
	sp.RequiredLevel = reqlevel;
	sp.RequiredSpell = reqspell;
	sp.DeleteSpell = delspell;

	pTrainer->Spells.push_back(sp);
	pTrainer->SpellCount++;
	
	SystemMessage(m_session, "Added spell %u (%s) to trainer.", pSpell->Id, pSpell->Name);
	sGMLog.writefromsession(m_session, "added spell %u to trainer %u", spellid, pCreature->GetEntry());
	WorldDatabase.Execute("INSERT INTO trainer_spells VALUES(%u, %u, %u, %u, %u, %u, %u, %u, %u, %u)", 
		pCreature->GetEntry(), (int)0, pSpell->Id, cost, reqspell, (int)0, (int)0, reqlevel, delspell, (int)0);

	return true;
}

bool ChatHandler::HandleWSCommand(const char* args, WorldSession* m_session)
{
	if(!args || strlen(args) < 3)
	{
		RedSystemMessage(m_session, "No spellid specified.");
		return true;
	}

	char* next_token;
	char* s = strtok_s((char*)args, " ",&next_token);
	if (!s)
		return false;

	uint32 state  = atol(s);
	
	uint32 value=0;
	s = strtok_s(NULL, " ",&next_token);
	if (s)
		value = atol(s);

	WorldPacket data(SMSG_UPDATE_WORLD_STATE, 8);
	data << state << value;
	m_session->GetPlayer()->SendMessageToSet(&data, true);

	return true;
}
// (Branruz)
bool ChatHandler::HandlePvPCommand( const char *args , WorldSession *m_session )
{
	int PlayerField;
	uint32 FieldValue, SetPvPField;
	bool HelpRequired = false;
	Player *chr = NULL;

   if(sscanf(args, "%u %u %u", (int *)&PlayerField, (uint32*)&FieldValue,(uint32*)&SetPvPField) != 3)
   {
	   HelpRequired = true;
	   PlayerField  = -1;
   }

   if(PlayerField <= 0)
   {
    chr = getSelectedChar( m_session, true ); // Sur quelqu'un
    if( chr == NULL ) chr = m_session->GetPlayer();   // Sur lui-meme
    if( chr == NULL ) 
	{
		RedSystemMessage(m_session, "Player non trouvee!");
		return true;
	}
   }

   if(PlayerField == 0) // m_uint32Values[UNIT_FIELD_FLAGS];
   {
	   switch(FieldValue)
	   {
	    case 0 : // UNIT_FLAG_PVP - 0x00001000
			if(SetPvPField) chr->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP);
			else            chr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP);
			GreenSystemMessage(m_session, "%s UNIT_FIELD_FLAGS:UNIT_FLAG_PVP to %s",(SetPvPField) ? "Set" : "Remove",chr->GetName());
			break;
		default : HelpRequired = true; break;
	   }
   }
   else if(PlayerField == 1) // ByteFlag(UNIT_FIELD_BYTES_2)
   {
	   switch(FieldValue)
	   {
		 case 0 : // U_FIELD_BYTES_FLAG_PVP - 0x01
			 if(SetPvPField) chr->SetByteFlag(UNIT_FIELD_BYTES_2   , 1, U_FIELD_BYTES_FLAG_PVP);
			 else            chr->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_PVP);
			 GreenSystemMessage(m_session, "%s UNIT_FIELD_BYTES_2:U_FIELD_BYTES_FLAG_PVP to %s",(SetPvPField) ? "Set" : "Remove",chr->GetName());
			break;

		case 1 : // U_FIELD_BYTES_FLAG_FFA_PVP - 0x04
			if(SetPvPField) chr->SetByteFlag(UNIT_FIELD_BYTES_2   , 1, U_FIELD_BYTES_FLAG_FFA_PVP);
			else            chr->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_FFA_PVP);
			GreenSystemMessage(m_session, "%s UNIT_FIELD_BYTES_2:U_FIELD_BYTES_FLAG_FFA_PVP to %s",(SetPvPField) ? "Set" : "Remove",chr->GetName());
			break;

		default : HelpRequired = true; break;
	   }
   }
   else if(PlayerField == 2) // m_uint32Values[PLAYER_FLAGS];
   {
	   switch(FieldValue)
	   {
		case 0 : // PLAYER_FLAG_PVP_TOGGLE - 0x000200
			if(SetPvPField) chr->SetFlag(PLAYER_FLAGS   ,PLAYER_FLAG_PVP_TOGGLE);
			else            chr->RemoveFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TOGGLE);
			GreenSystemMessage(m_session, "%s PLAYER_FLAGS:PLAYER_FLAG_PVP_TOGGLE to %s",(SetPvPField) ? "Set" : "Remove",chr->GetName());
			break;

		case 1 : // PLAYER_FLAG_PVP_TIMER - 0x040000
			if(SetPvPField) 
			{
				chr->SetFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TIMER);
				chr->ResetPvPTimer();
			}
			else            
			{
				chr->RemoveFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TIMER);
				chr->StopPvPTimer();
			}
			GreenSystemMessage(m_session, "%s PLAYER_FLAGS:PLAYER_FLAG_PVP_TIMER to %s",(SetPvPField) ? "Set" : "Remove",chr->GetName());
			break;

		case 2 : // PLAYER_FLAG_FREE_FOR_ALL_PVP - 0x000080
			if(SetPvPField) chr->SetFlag(PLAYER_FLAGS   ,PLAYER_FLAG_FREE_FOR_ALL_PVP);
			else            chr->RemoveFlag(PLAYER_FLAGS,PLAYER_FLAG_FREE_FOR_ALL_PVP);
			GreenSystemMessage(m_session, "%s PLAYER_FLAGS:PLAYER_FLAG_FREE_FOR_ALL_PVP to %s",(SetPvPField) ? "Set" : "Remove",chr->GetName());
			break;

		default : HelpRequired = true; break;
	   }
   }
   else // PlayerField Inconnu
   {
	   HelpRequired = true;
   }
   
   if(HelpRequired)
   {
	 GreenSystemMessage(m_session, "Command: .debug pvp <PlayerField> <Valeur> <0:1>");
     GreenSystemMessage(m_session, "Command: <0:1> : 1 Set, 0 Remove");
     GreenSystemMessage(m_session, " ");
     GreenSystemMessage(m_session, "PlayerField               Valeur");
     GreenSystemMessage(m_session, "<0> : UnitFieldFlags      <0> : UNIT_FLAG_PVP"); // 0x00001000
     GreenSystemMessage(m_session, "<1> : UnitFieldBytes2     <0> : U_FIELD_BYTES_FLAG_PVP");     // 0x01
     GreenSystemMessage(m_session, "                          <1> : U_FIELD_BYTES_FLAG_FFA_PVP"); // 0x04
     GreenSystemMessage(m_session, "<2> : PlayerFlags         <0> : PLAYER_FLAG_PVP_TOGGLE");// 0x000200
     GreenSystemMessage(m_session, "                          <1> : PLAYER_FLAG_PVP_TIMER"); // 0x040000
     GreenSystemMessage(m_session, "                          <2> : PLAYER_FLAG_FREE_FOR_ALL_PVP"); //(0x000080)
   }

	return true;
}
// (Branruz)
bool ChatHandler::HandleAddPlayerAura( const char *args , WorldSession *m_session )
{
	int PlayerAuraId;
	Player *chr;

   if(sscanf(args, "%u", (int *)&PlayerAuraId) != 1)
   {
	   GreenSystemMessage(m_session, "Command: .debug addaura <auraId>");
   }

     chr = getSelectedChar( m_session, true ); // Sur quelqu'un
    if( chr == NULL ) chr = m_session->GetPlayer();   // Sur lui-meme
    if( chr == NULL ) 
	{
		RedSystemMessage(m_session, "Player non trouvee!");
		return true;
	}
	GreenSystemMessage(m_session, "Add Aura %u to player %s",PlayerAuraId,chr->GetName() ); 

	return true;
}

bool ChatHandler::HandleChatIrc( const char *args , WorldSession *m_session )
{
 Player *chr;
 char _buffer_[255];

 snprintf(_buffer_,250,"%s",args);
 chr = m_session->GetPlayer();

 GreenSystemMessage(m_session, "%s dit",chr->GetName(),_buffer_ ); 
 /*
 if(m_ChatIrc)
 {
   SendToChatIrc(chr->GetName(),_buffer_);
 }
 */
 return true;

}
//-------------------------
bool ChatHandler::HandleAchievementGetName( const char *args , WorldSession *m_session )
{	
	if(!args) return(false);

    uint32 achID = 0; 

	achID = atoi(args);

	if(achID == 0) return(false);

	AchievementEntry * ae = dbcAchievement.LookupEntryForced(achID);
	if(!ae)
	{
		
		RedSystemMessage(m_session, "Le haut-fait %u n'existe pas.", achID);
		return true;
	}

	//Log.Success("AchievementGetName","Haut-fait %u : %s", ae->ID, ae->name);
	GreenSystemMessage(m_session, "Haut-fait %u : %s", ae->ID, ae->name);

	return(true);
}
//-------------------------
bool ChatHandler::HandleAchievementComplete( const char *args , WorldSession *m_session )
{	
	if(!args)
		return false;

	uint32 achID = 0;
	Player * m_plr = getSelectedChar(m_session, true);

	if(sscanf(args, "%u", (uint32 *)&achID) != 1)
	{
		GreenSystemMessage(m_session, "Commande : .achievement complete <achID>");
		return true;
	}

	if( m_plr == NULL ) 
		m_plr = m_session->GetPlayer();

	if( m_plr == NULL)
	{
		GreenSystemMessage(m_session, "Aucun joueur selectionne !");
	}

	AchievementEntry * ae = dbcAchievement.LookupEntryForced(achID);
	if(!ae)
	{
		RedSystemMessage(m_session, "Le haut-fait %u n'existe pas.", achID);
		return true;
	}

	AchievementData * ad = m_plr->GetAchievementInterface()->GetAchievementDataByAchievementID(achID);

	if(ad)
	{	
		// On vérifie si le joueur à déjà accompli le HF.
		if(ad->completed)
		{
			RedSystemMessage(m_session, "Le joueur %s a deja accompli ce haut-fait.", m_plr->GetName());
			return true;
		}

		ad->completed = true;
		ad->date = (uint32)time(NULL);

		if( m_plr->IsInWorld() )
			m_plr->GetSession()->SendPacket( m_plr->GetAchievementInterface()->BuildAchievementEarned(ad) );
		
		GreenSystemMessage(m_session, "Achevement du haut-fait %u du joueur %s.", achID, m_plr->GetName());
	}
	else
	{
	  RedSystemMessage(m_session, "Le joueur %s ne possede pas le haut-fait %u.", m_plr->GetName(),achID);
	}
	
	return true;
}

bool ChatHandler::HandleRecallGoCommand(const char* args, WorldSession *m_session)
{
	if( args == NULL )
		return false;

	if( !*args )
		return false;

	if( m_session == NULL )
		return false;

	QueryResult *result = WorldDatabase.Query( "SELECT * FROM recall ORDER BY name" );

	if( result == NULL)
		return false;

	do
	{
		Field* fields = result->Fetch();
		const char* locname = fields[1].GetString();
		uint32 locmap = fields[2].GetUInt32();
		float x = fields[3].GetFloat();
		float y = fields[4].GetFloat();
		float z = fields[5].GetFloat();

		if( strnicmp( const_cast< char* >( args ), locname, strlen( args ) ) == 0 )
		{
			if( m_session->GetPlayer() != NULL )
			{
				m_session->GetPlayer()->SafeTeleport(locmap, 0, LocationVector(x, y, z));
				delete result;
				return true;
			}
			else
			{
				delete result;
				return false;
			}
		}

	}while (result->NextRow());

	delete result;
	return false;
}

bool ChatHandler::HandleRecallAddCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;
	
	QueryResult *result = WorldDatabase.Query( "SELECT name FROM recall" );
	if(!result)
		return false;
	do
	{
		Field *fields = result->Fetch();
		const char * locname = fields[0].GetString();
		if (strncmp((char*)args,locname,strlen(locname))==0)
		{
			RedSystemMessage(m_session, "Name in use, please use another name for your location.");
			delete result;
			return true;
		}
	}while (result->NextRow());
	delete result;

	Player* plr = m_session->GetPlayer();
	std::stringstream ss;
	
	string rc_locname = string(args);

	ss << "INSERT INTO recall (name, mapid, positionX, positionY, positionZ) VALUES ('"
	<< WorldDatabase.EscapeString(rc_locname).c_str() << "' , "
	<< plr->GetMapId() << ", "
	<< plr->GetPositionX() << ", " 
	<< plr->GetPositionY() << ", "
	<< plr->GetPositionZ() << ");";
	WorldDatabase.Execute( ss.str( ).c_str( ) );

	char buf[256]; 
	snprintf((char*)buf, 256, "Added location to DB with MapID: %d, X: %f, Y: %f, Z: %f",
		(unsigned int)plr->GetMapId(), plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ());
	GreenSystemMessage(m_session, buf);
	sGMLog.writefromsession(m_session, "used recall add, added \"%s\" location to database.", rc_locname.c_str());

	return true;
}

bool ChatHandler::HandleRecallDelCommand(const char* args, WorldSession *m_session)
{
	   if(!*args)
		return false;

	QueryResult *result = WorldDatabase.Query( "SELECT id,name FROM recall" );
	if(!result)
		return false;

	do
	{
		Field *fields = result->Fetch();
		float id = fields[0].GetFloat();
		const char * locname = fields[1].GetString();

		if (strnicmp((char*)args,locname,strlen(locname))==0)
		{
			std::stringstream ss;
			ss << "DELETE FROM recall WHERE id = "<< (int)id <<";";
			WorldDatabase.Execute( ss.str( ).c_str( ) );
			GreenSystemMessage(m_session, "Recall location removed.");
			sGMLog.writefromsession(m_session, "used recall delete, removed \"%s\" location from database.", args);
			delete result;
			return true;
		}

	}while (result->NextRow());

	delete result;
	return false;
}

bool ChatHandler::HandleRecallListCommand(const char* args, WorldSession *m_session)
{
	QueryResult *result = WorldDatabase.Query( "SELECT id,name FROM recall ORDER BY name" );
	if(!result)
		return false;
	std::string recout;
	uint32 count = 0;

	recout = "|cff00ff00Recall locations|r:\n\n";
	do
	{
		Field *fields = result->Fetch();
		//float id = fields[0].GetFloat();
		const char * locname = fields[1].GetString();
		recout += "|cff00ccff";
		recout += locname;
		recout += "|r, ";
		count++;
		
		if(count == 5)
		{
			recout += "\n";
			count = 0;
		}
	}while (result->NextRow());
	SendMultilineMessage(m_session, recout.c_str());

	delete result;
	return true;
}

bool ChatHandler::HandleRecallPortPlayerCommand(const char* args, WorldSession * m_session)
{
	char location[255];
	char player[255];
	if(sscanf(args, "%s %s", player, location) != 2)
		return false;

	Player * plr = objmgr.GetPlayer(player, false);
	if(!plr) return false;

	QueryResult *result = WorldDatabase.Query( "SELECT * FROM recall ORDER BY name" );
	if(!result)
		return false;

	do
	{
		Field *fields = result->Fetch();
		const char * locname = fields[1].GetString();
		uint32 locmap = fields[2].GetUInt32();
		float x = fields[3].GetFloat();
		float y = fields[4].GetFloat();
		float z = fields[5].GetFloat();

		if (strnicmp((char*)location,locname,strlen(args))==0)
		{
			if(plr->GetInstanceID() != m_session->GetPlayer()->GetInstanceID())
				sEventMgr.AddEvent(plr, &Player::EventSafeTeleport, locmap, uint32(0), LocationVector(x, y, z), EVENT_PLAYER_TELEPORT, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			else
				plr->SafeTeleport(locmap, 0, LocationVector(x, y, z));
			delete result;
			return true;
		}

	}while (result->NextRow());

	delete result;
	return false;
}

bool ChatHandler::HandleSetBGScoreCommand(const char* args, WorldSession *m_session)
{
	return true;
}

bool ChatHandler::HandleStartBGCommand(const char *args, WorldSession *m_session)
{
	return true;
}

bool ChatHandler::HandlePauseBGCommand(const char *args, WorldSession *m_session)
{
	return true;
}

bool ChatHandler::HandleBGInfoCommnad(const char *args, WorldSession *m_session)
{
	return true;
}

bool ChatHandler::HandleBattlegroundCommand(const char* args, WorldSession *m_session)
{
	/*
	uint32 type = atoi(args);
	if(type != 2 && type != 3 && type != 4 && type != 5 && type != 7)
		return false;

	Player * plr = getSelectedChar(m_session, true);
	if(!plr) return true;
	BattlegroundManager.HandleBattlegroundListPacket(plr->GetSession(), atoi(args));
	*/
	return true;
}

bool ChatHandler::HandleSetWorldStateCommand(const char* args, WorldSession *m_session)
{
	uint32 id,val;
	if(sscanf(args,"%u %u", &id,&val)!=2)
		return false;

	if(m_session->GetPlayer()->m_bg)
		m_session->GetPlayer()->m_bg->SetWorldState(id,val);
	//m_mapMgr->GetDBStateManager().UpdateWorldState(id,val);

	return true;
}

bool ChatHandler::HandlePlaySoundCommand(const char* args, WorldSession *m_session)
{
	return true;
}
/*
bool ChatHandler::HandleSetBattlefieldStatusCommand(const char* args, WorldSession *m_session)
{
	uint32 type = atoi(args);
	BattlegroundManager.SendBattlefieldStatus(m_session->GetPlayer(), 1, type, 0 , 0, m_session->GetPlayer()->GetMapId(),0);
	return true;
}
*/
bool ChatHandler::HandleBattlegroundExitCommand(const char* args, WorldSession* m_session)
{
	return true;
}

bool ChatHandler::HandleDebugInFrontCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	Object *obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid);

	if (guid != 0)
	{
		if(!obj)
		{
			SystemMessage(m_session, "Vous devez sélectionner un personnage ou une creature");
			return true;
		}
	}
	else
		obj = (Object*)m_session->GetPlayer();

	char buf[256];
	snprintf((char*)buf, 256, "%d", m_session->GetPlayer()->isInFront((Unit *)obj));

	SystemMessage(m_session, buf);

	return true;
}

bool ChatHandler::HandleShowReactionCommand(const char* args, WorldSession *m_session)
{
	Object *obj = NULL;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
	{
		obj = (Object*)m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	}

	if(!obj)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}


	char* next_token;
	char* pReaction = strtok_s((char*)args, " ",&next_token);
	if (!pReaction)
		return false;

	uint32 Reaction  = atoi(pReaction);

	WorldPacket data(SMSG_AI_REACTION, 12);
	data << obj->GetGUID() << uint32(Reaction);
	m_session->SendPacket( &data );

	std::stringstream sstext;
	sstext << "Sent Reaction of " << Reaction << " to " << obj->GetUIdFromGUID() << '\0';

	SystemMessage(m_session,  sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleDistanceCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	Object *obj = (Object*)m_session->GetPlayer()->GetMapMgr()->GetUnit(guid);

	if (guid != 0)
	{
		if(!obj)
		{
			SystemMessage(m_session, "Vous devez sélectionner un personnage ou une creature");
			return true;
		}
	}
	else
		obj = (Object*)m_session->GetPlayer();

	float dist = m_session->GetPlayer()->CalcDistance(obj);
	std::stringstream sstext;
	sstext << "Distance is: " << dist <<'\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleMoveInfoCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	Object *obj = (Object*)m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	
	if(!obj)
	{
		SystemMessage(m_session, "Vous devez sélectionner un personnage ou une creature");
		return true;
	}

	float dist = m_session->GetPlayer()->CalcDistance(obj);
	bool minfront = obj->isInFront(m_session->GetPlayer());
	bool pinfront = m_session->GetPlayer()->isInFront(obj);
	uint32 movetype = ((Creature *)obj)->GetAIInterface()->getMoveType();
	bool run = ((Creature *)obj)->GetAIInterface()->getMoveRunFlag();
	uint32 attackerscount = (uint32)((Creature *)obj)->GetAIInterface()->getAITargetsCount();
	uint32 creatureState = ((Creature *)obj)->GetAIInterface()->m_creatureState;
	uint32 curwp = ((Creature *)obj)->GetAIInterface()->getCurrentWaypoint();
	uint32 aistate = ((Creature *)obj)->GetAIInterface()->getAIState();
	uint32 aitype = ((Creature *)obj)->GetAIInterface()->getAIType();
	uint32 aiagent = ((Creature *)obj)->GetAIInterface()->getCurrentAgent();
	uint32 lowfollow = 0;
	uint32 highfollow = 0;
	/*if(unitToFollow == NULL)
	{
		lowfollow = 0;
		highfollow = 0;
	}
	else
	{
		lowfollow = unitToFollow->GetGUIDLow();
		highfollow = unitToFollow->GetGUIDHigh();;
	}*/

	std::stringstream sstext;
	sstext << "Following Unit: Low: " << lowfollow << " High: " << highfollow << "\n";
	sstext << "Distance is: " << dist << "\n";
	sstext << "Mob Facing Player: " << minfront << " Player Facing Mob: " << pinfront << "\n";
	sstext << "Attackers Count: " << attackerscount << "\n";
	sstext << "Creature State: " << creatureState << " Run: " << run << "\n";
	sstext << "AIState: " << aistate << " AIType: " << aitype << " AIAgent: " << aiagent << "\n";
	sstext << "MoveType: " << movetype << " Current Waypoint: " << curwp << "\n";

	SendMultilineMessage(m_session, sstext.str().c_str());
	//FillSystemMessageData(&data, sstext.str().c_str());
	//m_session->SendPacket( &data );

	return true;
}

bool ChatHandler::HandleAIMoveCommand(const char* args, WorldSession *m_session)
{
	Object *obj = NULL;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
	{
		obj = (Object*)m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	}

	if(obj == NULL)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}

	//m_session->GetPlayer()->GetOrientation();

	uint32 Move  = 1;
	uint32 Run  = 0;
	uint32 Time = 0;
	uint32 Meth = 0;

	char* next_token;
	char* pMove = strtok_s((char*)args, " ",&next_token);
	if (pMove)
		Move  = atoi(pMove);

	char* pRun = strtok_s(NULL, " ",&next_token);
	if (pRun)
		Run  = atoi(pRun);

	char* pTime = strtok_s(NULL, " ",&next_token);
	if (pTime)
		Time  = atoi(pTime);

	char* pMeth = strtok_s(NULL, " ",&next_token);
	if (pMeth)
		Meth  = atoi(pMeth);

	float x = m_session->GetPlayer()->GetPositionX();
	float y = m_session->GetPlayer()->GetPositionY();
	float z = m_session->GetPlayer()->GetPositionZ();
	float o = m_session->GetPlayer()->GetOrientation();
	((Creature *)obj)->GetAIInterface()->setMoveRunFlag((Run>0?true:false));
	float distance = ((Creature *)obj)->CalcDistance(x,y,z);
	if(Move == 1)
	{
		if(Meth == 1)
		{
			float q = distance-0.5f;
			x = (((Creature *)obj)->GetPositionX()+x*q)/(1+q);
			y = (((Creature *)obj)->GetPositionY()+y*q)/(1+q);
			z = (((Creature *)obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 2)
		{
			float q = distance-1;
			x = (((Creature *)obj)->GetPositionX()+x*q)/(1+q);
			y = (((Creature *)obj)->GetPositionY()+y*q)/(1+q);
			z = (((Creature *)obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 3)
		{
			float q = distance-2;
			x = (((Creature *)obj)->GetPositionX()+x*q)/(1+q);
			y = (((Creature *)obj)->GetPositionY()+y*q)/(1+q);
			z = (((Creature *)obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 4)
		{
			float q = distance-2.5f;
			x = (((Creature *)obj)->GetPositionX()+x*q)/(1+q);
			y = (((Creature *)obj)->GetPositionY()+y*q)/(1+q);
			z = (((Creature *)obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 5)
		{
			float q = distance-3;
			x = (((Creature *)obj)->GetPositionX()+x*q)/(1+q);
			y = (((Creature *)obj)->GetPositionY()+y*q)/(1+q);
			z = (((Creature *)obj)->GetPositionZ()+z*q)/(1+q);
		}
		else if(Meth == 6)
		{
			float q = distance-3.5f;
			x = (((Creature *)obj)->GetPositionX()+x*q)/(1+q);
			y = (((Creature *)obj)->GetPositionY()+y*q)/(1+q);
			z = (((Creature *)obj)->GetPositionZ()+z*q)/(1+q);
		}
		else
		{
			float q = distance-4;
			x = (((Creature *)obj)->GetPositionX()+x*q)/(1+q);
			y = (((Creature *)obj)->GetPositionY()+y*q)/(1+q);
			z = (((Creature *)obj)->GetPositionZ()+z*q)/(1+q);
		}
		((Creature *)obj)->GetAIInterface()->MoveTo(x,y,z,0);
	}
	else
	{
		uint32 moveTime = 0;
		if(!Time)
		{
			//float dx = x - ((Creature *)obj)->GetPositionX();
			//float dy = y - ((Creature *)obj)->GetPositionY();
			//float dz = z - ((Creature *)obj)->GetPositionZ();

			//float distance = sqrt((dx*dx) + (dy*dy) + (dz*dz));
			if(!distance)
			{
				SystemMessage(m_session, "The Creature is already there.");
				return true;
			}

			float moveSpeed = 1.0f;
			if(!Run)
			{
				moveSpeed = 2.5f*0.001f;
			}
			else
			{
				moveSpeed = 7.0f*0.001f;
			}

			moveTime = uint32(distance / moveSpeed);
		}
		else
		{
			moveTime = Time;
		}
		//((Creature *)obj)->setMovementState(MOVING);
		((Creature *)obj)->GetAIInterface()->SendMoveToPacket(x,y,z,o,moveTime,Run);
	}
	return true;
}

bool ChatHandler::HandleFaceCommand(const char* args, WorldSession *m_session)
{

	Object *obj = NULL;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid != 0)
	{
		obj = (Object*)m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	}

	if(obj == NULL)
	{
		SystemMessage(m_session,  "You should select a creature.");
		return true;
	}

	uint32 Orentation = 0;
	char* next_token;
	char* pOrentation = strtok_s((char*)args, " ",&next_token);
	if (pOrentation)
		Orentation  = atoi(pOrentation);

	/* Convert to Blizzards Format */
	float theOrientation = Orentation/(360/float(6.28));

	obj->SetPosition(obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ(), theOrientation, false);

	/*
	data.Initialize( SMSG_MONSTER_MOVE );
	data << obj->GetGUID();
	data << obj->GetPositionX() << obj->GetPositionY() << obj->GetPositionZ() << obj->GetOrientation();
	data << uint8(1);
	
	data << uint32(0x100); //run
	data << uint32(0); //time
	data << uint32(2);
	data << obj->GetPositionX() << obj->GetPositionY() << obj->GetPositionZ() << theOrientation;
	*/
	//UpdateData upd;

	// update movment for others
	//obj->BuildMovementUpdateBlock(&upd,0);
	//upd.BuildPacket( &data );
	//GetSession()->SendPacket( &packet );
	//obj->BuildMovementUpdateBlock(data,0)
	//obj->SendMessageToSet(&data,false);
	sLog.outDebug("facing sent");
	return true;
	//((Creature *)obj)->AI_MoveTo(obj->GetPositionX()+0.1,obj->GetPositionY()+0.1,obj->GetPositionZ()+0.1,theOrientation);
}
/*

bool ChatHandler::HandleAIMoveCommand(const char* args)
{
WorldPacket data;
Object *obj = NULL;

uint64 guid = m_session->GetPlayer()->GetSelection();
if (guid != 0)
{
obj = (Object*)objmgr.GetCreature(guid);
}

if(obj == NULL)
{
FillSystemMessageData(&data, "You should select a creature.");
m_session->SendPacket( &data );
return true;
}

uint8 Value1  = 0;
bool Run  = 0;
uint32 Value2 = 1;
uint32 Value3 = 0;
bool ToFrom = 0;

char* next_token;
char* pValue1 = strtok_s((char*)args, " ",&next_token);
if (pValue1)
Value1  = static_cast<uint8>(atol(pValue1));

char* pRun = strtok_s(NULL, " ",&next_token);
if (pRun)
Run  = atoi(pRun);

char* pValue2 = strtok_s(NULL, " ",&next_token);
if (pValue2)
Value2  = atoi(pValue2);

char* pValue3 = strtok_s(NULL, " ",&next_token);
if (pValue3)
Value3  = atoi(pValue3);

char* pToFrom = strtok_s(NULL, " ",&next_token);
if (pToFrom)
ToFrom  = atoi(pToFrom);

float fromX = ((Creature *)obj)->GetPositionX();
float fromY = ((Creature *)obj)->GetPositionY();
float fromZ = ((Creature *)obj)->GetPositionZ();
float fromO = ((Creature *)obj)->GetOrientation();
float toX = m_session->GetPlayer()->GetPositionX();
float toY = m_session->GetPlayer()->GetPositionY();
float toZ = m_session->GetPlayer()->GetPositionZ();
float toO = m_session->GetPlayer()->GetOrientation();

float distance = ((Creature *)obj)->CalcDistance((Object *)m_session->GetPlayer());
uint32 moveSpeed = 0;
if(!Run)
{
moveSpeed = 2.5f*0.001f;
}
else
{
moveSpeed = 7.0f*0.001f;
}
uint32 moveTime = (uint32) (distance / moveSpeed);

data.Initialize( SMSG_MONSTER_MOVE );
data << guid;
if(ToFrom)
{
data << toX << toY << toZ << toO;
}
else
{
data << fromX << fromY << fromZ << fromO;
}
data << uint8(Value1);
if(Value1 != 1)
{
data << uint32(Run ? 0x00000100 : 0x00000000);
data << moveTime;
data << Value2;
if(ToFrom)
{
data << fromX << fromY << fromZ;
if(Value2 > 1)
{
data << fromO;
}
}
else
{
data << toX << toY << toZ;
if(Value2 > 1)
{
data << toO;
}

}
if(Value2 > 2)
{
data << Value3;
}
}
//((Creature *)obj)->m_m_timeToMove = moveTime;
//m_moveTimer =  UNIT_MOVEMENT_INTERPOLATE_INTERVAL; // update every few msecs

//	m_creatureState = MOVING;
((Creature *)obj)->SendMessageToSet( &data, false );
return true;
}
*/
bool ChatHandler::HandleSetBytesCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	Object *obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid);

	if (guid != 0)
	{
		if(!obj)
		{
			SystemMessage(m_session, "Vous devez sélectionner un personnage ou une creature");
			return true;
		}
	}
	else
		obj = (Object*)m_session->GetPlayer();

	char* next_token;
	char* pBytesIndex = strtok_s((char*)args, " ",&next_token);
	if (!pBytesIndex)
		return false;

	uint32 BytesIndex  = atoi(pBytesIndex);

	char* pValue1 = strtok_s(NULL, " ",&next_token);
	if (!pValue1)
		return false;

	uint8 Value1  = static_cast<uint8>(atol(pValue1));

	char* pValue2 = strtok_s(NULL, " ",&next_token);
	if (!pValue2)
		return false;

	uint8 Value2  = static_cast<uint8>(atol(pValue2));

	char* pValue3 = strtok_s(NULL, " ",&next_token);
	if (!pValue3)
		return false;

	uint8 Value3  = static_cast<uint8>(atol(pValue3));

	char* pValue4 = strtok_s(NULL, " ",&next_token);
	if (!pValue4)
		return false;

	uint8 Value4  = static_cast<uint8>(atol(pValue4));

	std::stringstream sstext;
	sstext << "Set Field " << BytesIndex << " bytes to " << uint16((uint8)Value1) << " " << uint16((uint8)Value2) << " " << uint16((uint8)Value3) << " " << uint16((uint8)Value4) << '\0';
	obj->SetUInt32Value(BytesIndex, ( ( Value1 ) | ( Value2 << 8 ) | ( Value3 << 16 ) | ( Value4 << 24 ) ) );
	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleGetBytesCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	Object *obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid);

	if (guid != 0)
	{
		if(!obj)
		{
			SystemMessage(m_session, "Vous devez sélectionner un personnage ou une creature");
			return true;
		}
	}
	else
		obj = (Object*)m_session->GetPlayer();

	char* next_token;
	char* pBytesIndex = strtok_s((char*)args, " ",&next_token);
	if (!pBytesIndex)
		return false;

	uint32 BytesIndex  = atoi(pBytesIndex);
	uint32 theBytes = obj->GetUInt32Value(BytesIndex);
	/*
	switch (Bytes)
	{
	case 0:
	theBytes = obj->GetUInt32Value(UNIT_FIELD_BYTES_0);
	break;
	case 1:
	theBytes = obj->GetUInt32Value(UNIT_FIELD_BYTES_1);
	break;
	case 2:
	theBytes = obj->GetUInt32Value(UNIT_FIELD_BYTES_2);
	break;
	}
	*/
	std::stringstream sstext;
	sstext << "bytes for Field " << BytesIndex << " are " << uint16((uint8)theBytes & 0xFF) << " " << uint16((uint8)(theBytes >> 8) & 0xFF) << " ";
	sstext << uint16((uint8)(theBytes >> 16) & 0xFF) << " " << uint16((uint8)(theBytes >> 24) & 0xFF) << '\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}
bool ChatHandler::HandleDebugLandWalk(const char* args, WorldSession *m_session)
{
	Player *chr = getSelectedChar(m_session);
	char buf[256];

	if (chr == NULL) // Ignatich: what should NOT happen but just in case...
	{
		SystemMessage(m_session, "No character selected.");
		return false;
	}
	chr->SetMovement(MOVE_LAND_WALK,8);
	snprintf((char*)buf,256, "Land Walk Test Ran.");
	SystemMessage(m_session, buf);
	return true;
}

bool ChatHandler::HandleDebugWaterWalk(const char* args, WorldSession *m_session)
{
	Player *chr = getSelectedChar(m_session);
	char buf[256];

	if (chr == NULL) // Ignatich: what should NOT happen but just in case...
	{
		SystemMessage(m_session, "No character selected.");
		return false;
	}
	chr->SetMovement(MOVE_WATER_WALK, 4);
	snprintf((char*)buf,256, "Water Walk Test Ran.");
	SystemMessage(m_session,  buf);
	return true;
}

bool ChatHandler::HandleDebugUnroot(const char* args, WorldSession *m_session)
{
	Player *chr = getSelectedChar(m_session);
	char buf[256];

	if (chr == NULL) // Ignatich: what should NOT happen but just in case...
	{
		SystemMessage(m_session,  "No character selected.");
		return false;
	}

	chr->SetMovement(MOVE_UNROOT,5);

	snprintf((char*)buf,256, "UnRoot Test Ran.");
	SystemMessage(m_session, buf);
	return true;
}

bool ChatHandler::HandleDebugRoot(const char* args, WorldSession *m_session)
{
	Player *chr = getSelectedChar(m_session);
	char buf[256];

	if (chr == NULL) // Ignatich: what should NOT happen but just in case...
	{
		SystemMessage(m_session, "No character selected.");
		return true;
	}
	chr->SetMovement(MOVE_ROOT,1);
	snprintf((char*)buf,256, "Root Test Ran.");
	SystemMessage(m_session, buf);
	return true;
}

bool ChatHandler::HandleAggroRangeCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	Unit *obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid);
	if (guid != 0)
	{
		if(!obj)
		{
			SystemMessage(m_session,  "Vous devez sélectionner un personnage ou une creature");
			return true;
		}
	}
	else
	{
		SystemMessage(m_session, "Vous devez sélectionner un personnage ou une creature");
		return true;
	}

	float aggroRange = obj->GetAIInterface()->_CalcAggroRange(m_session->GetPlayer());
	std::stringstream sstext;
	sstext << "Aggrorange is: " << sqrtf(aggroRange) <<'\0';

	SystemMessage(m_session, sstext.str().c_str());

	return true;
}

bool ChatHandler::HandleKnockBackCommand(const char* args, WorldSession *m_session)
{
	/*
	char* next_token;
	char* X = strtok_s((char*)args, " ",&next_token);
	if (!X)
		return false;
	char* Y = strtok_s(NULL, " ",&next_token);
	if (!Y)
		return false;
	char* Z = strtok_s(NULL, " ",&next_token);
	if (!Z)
		return false;
	char* O = strtok_s(NULL, " ",&next_token);
	if (!O)
		return false;

	WorldPacket data(SMSG_MOVE_KNOCK_BACK, 25);
	data << m_session->GetPlayer()->GetNewGUID();
	data << float(atof(X)) << float(atof(Y)) << float(atof(Z)) << float(atof(O));

	m_session->GetPlayer()->SendMessageToSet(&data, true);*/

	
	float f = args ? (float)atof(args) : 0.0f;
	if(f == 0.0f)
		f = 5.0f;

	//Player * plr = m_session->GetPlayer();

	float dx =sinf(m_session->GetPlayer()->GetOrientation());
	float dy =cosf(m_session->GetPlayer()->GetOrientation());

	float z = f*0.66f;

	WorldPacket data(SMSG_MOVE_KNOCK_BACK, 50);
	data << m_session->GetPlayer()->GetNewGUID();
	data << getMSTime();
	data << dy << dx;
	data << f;
	data << z;
	m_session->GetPlayer()->SendMessageToSet(&data, true);

	return true;
}

bool ChatHandler::HandleFadeCommand(const char* args, WorldSession *m_session)
{
	Unit* target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!target)
		target = m_session->GetPlayer();
	char* next_token;
	char* v = strtok_s((char*)args, " ",&next_token);
	if(!v)
		return false;

	target->ModThreatModifyer(atoi(v));

	std::stringstream sstext;
	sstext << "threat is now reduced by: " << target->GetThreatModifyer() <<'\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleThreatModCommand(const char* args, WorldSession *m_session)
{
	Unit* target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!target)
		target = m_session->GetPlayer();
	char* next_token;
	char* v = strtok_s((char*)args, " ",&next_token);
	if(!v)
		return false;

	target->ModGeneratedThreatModifyer(atoi(v));

	std::stringstream sstext;
	sstext << "new threat caused is now reduced by: " << target->GetGeneratedThreatModifyer() << "%" <<'\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleCalcThreatCommand(const char* args, WorldSession *m_session)
{
	Unit* target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!target)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}
	char* next_token;
	char* dmg = strtok_s((char*)args, " ",&next_token);
	if(!dmg)
		return false;
	char* spellId = strtok_s(NULL, " ",&next_token);
	if(!spellId)
		return false;

	uint32 threat = target->GetAIInterface()->_CalcThreat(atol(dmg), dbcSpell.LookupEntry( atoi( spellId ) ), m_session->GetPlayer());

	std::stringstream sstext;
	sstext << "generated threat is: " << threat <<'\0';

	SystemMessage(m_session, sstext.str().c_str());
	return true;
}

bool ChatHandler::HandleThreatListCommand(const char* args, WorldSession *m_session)
{
	Unit* target = NULL;
	target = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
	if(!target)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}

	std::stringstream sstext;
	sstext << "threatlist of creature: " << GUID_LOPART(m_session->GetPlayer()->GetSelection()) << " " << GUID_HIPART(m_session->GetPlayer()->GetSelection()) << '\n';
	TargetMap::iterator itr;
	for(itr = target->GetAIInterface()->GetAITargets()->begin(); itr != target->GetAIInterface()->GetAITargets()->end();)
	{
		if(!itr->second)
		{
			++itr;
			continue;
		}
		sstext << "guid: " << itr->first->GetGUID() << " | threat: " << itr->second << "| threat after mod: " << (itr->second + itr->first->GetThreatModifyer()) << "\n";
		++itr;
	}

	SendMultilineMessage(m_session, sstext.str().c_str());
	return true;
}
bool ChatHandler::HandleSendItemPushResult(const char* args, WorldSession* m_session)
{
	uint32 itemid, count, type, unk1, unk2, unk3, unk4;
	char* arg = const_cast<char*>(args),*next_token;
	itemid = atol(strtok_s(arg, " ",&next_token));
	if(!itemid) return false;
	count = atol(strtok_s(NULL, " ",&next_token));
	type = atol(strtok_s(NULL, " ",&next_token));
	unk1 = atol(strtok_s(NULL, " ",&next_token));
	unk2 = atol(strtok_s(NULL, " ",&next_token));
	unk3 = atol(strtok_s(NULL, " ",&next_token));
	unk4 = atol(strtok_s(NULL, " ",&next_token));

	// lookup item
//	ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(itemid);

	WorldPacket data;
	data.SetOpcode(SMSG_ITEM_PUSH_RESULT);

	data << m_session->GetPlayer()->GetGUID();			   // recivee_guid
	data << type << uint32(1);  // unk
	data << count;			  // count
	data << uint8(0xFF);				// uint8 unk const 0xFF
	data << unk1;	   // unk
	data << itemid;
	data << unk2;		  // unk
	data << unk3;		 // random prop
	data << unk4;
	m_session->SendPacket(&data);
	return true;
	//data << ((proto != NULL) ? proto->Quality : uint32(0)); // quality
}

bool ChatHandler::HandleModifyBitCommand(const char* args, WorldSession* m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	Object *obj = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid);

	if (guid != 0)
	{
		if(!obj)
		{
			SystemMessage(m_session, "Vous devez sélectionner un personnage ou une creature");
			return true;
		}
	}
	else
		obj = (Object*)m_session->GetPlayer();

	char* next_token;
	char* pField = strtok_s((char*)args, " ",&next_token);
	if (!pField)
		return false;

	char* pBit = strtok_s(NULL, " ",&next_token);
	if (!pBit)
		return false;

	uint16 field = atoi(pField);
	uint32 bit   = atoi(pBit);

	if (field < 1 || field >= PLAYER_END)
	{
		SystemMessage(m_session, "Incorrect values.");
		return true;
	}

	if (bit < 1 || bit > 32)
	{
		SystemMessage(m_session, "Incorrect values.");
		return true;
	}

	char buf[256];

	if ( obj->HasFlag( field, (1<<(bit-1)) ) )
	{
		obj->RemoveFlag( field, (1<<(bit-1)) );
		snprintf((char*)buf,256, "Removed bit %i in field %i.", (unsigned int)bit, (unsigned int)field);
	}
	else
	{
		obj->SetFlag( field, (1<<(bit-1)) );
		snprintf((char*)buf,256, "Set bit %i in field %i.", (unsigned int)bit, (unsigned int)field);
	}

	SystemMessage(m_session, buf);
	return true;
}

bool ChatHandler::HandleModifyValueCommand(const char* args,  WorldSession* m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	Object *obj = (Object*)m_session->GetPlayer()->GetMapMgr()->GetUnit(guid);

	if (guid != 0)
	{
		if(!obj)
		{
			SystemMessage(m_session, "Vous devez sélectionner un personnage ou une creature");
			return true;
		}
	}
	else
		obj = (Object*)m_session->GetPlayer();

	char* next_token;
	char* pField = strtok_s((char*)args, " ",&next_token);
	if (!pField)
		return false;

	char* pValue = strtok_s(NULL, " ",&next_token);
	if (!pValue)
		return false;

	uint16 field = atoi(pField);
	uint32 value   = atoi(pValue);

	if (field < 1 || field >= PLAYER_END)
	{
		SystemMessage(m_session, "Incorrect Field.");
		return true;
	}
/*
	if (value > sizeof(uint32))
	{
		FillSystemMessageData(&data, m_session, "Incorrect Value.");
		m_session->SendPacket( &data );
		return true;
	}
*/
	char buf[256];
	uint32 oldValue = obj->GetUInt32Value(field);
	obj->SetUInt32Value(field,value);

	snprintf((char*)buf,256,"Set Field %i from %i to %i.", (unsigned int)field, (unsigned int)oldValue, (unsigned int)value);

	if( obj->IsPlayer() )
		static_cast< Player* >( obj )->UpdateChances();

	SystemMessage(m_session, buf);

	return true;
}

bool ChatHandler::HandleDebugDumpCoordsCommmand(const char * args, WorldSession * m_session)
{
	Player* p = m_session->GetPlayer();
	//char buffer[200] = {0};
	FILE * f = fopen("C:\\script_dump.txt", "a");
	if(!f) return true;

	fprintf(f, "mob.CreateWaypoint(%f, %f, %f, %f, 0, 0, 0);\n", p->GetPositionX(), p->GetPositionY(), p->GetPositionZ(),
		p->GetOrientation());
	fclose(f);
	return true;
}

//As requested by WaRxHeAd for database development.
//This should really only be available in special cases and NEVER on real servers... -DGM
bool ChatHandler::HandleSQLQueryCommand(const char* args, WorldSession *m_session)
{
	#ifdef _ONLY_FOOLS_TRY_THIS_
	if(!*args)
	{
		RedSystemMessage(m_session, "No query given.");
		return false;
	}
	
	bool isok = WorldDatabase.Execute(args);

	if(isok)
		GreenSystemMessage(m_session, "Query was executed successfully.");
	else
		RedSystemMessage(m_session, "Query failed to execute.");

	#endif

	return true;
}

//#define _ONLY_FOOLS_TRY_THIS_

bool ChatHandler::HandleSendpacket(const char * args, WorldSession * m_session)
{
#ifdef _ONLY_FOOLS_TRY_THIS_

    uint32 arg_len = strlen(args);
    char * xstring = new char [arg_len];
    memcpy(xstring, args,arg_len);

    for (uint32 i = 0; i < arg_len; i++)
    {
        if (xstring[i] == ' ')
        {
            xstring[i] = '\0';
        }
    }

    // we receive our packet as hex, that means we get it like ff ff ff ff
    // the opcode consists of 2 bytes

    if (!xstring)
    {
        sLog.outDebug("[Debug][Sendpacket] Packet is invalid");
        return false;
    }

    WorldPacket data(arg_len);

    uint32 loop = 0;
    uint16 opcodex = 0;
    uint16 opcodez = 0;

    // get the opcode
    sscanf(xstring , "%x", &opcodex);

    // should be around here
    sscanf(&xstring[3] , "%x", &opcodez);

    opcodex =  opcodex << 8;
    opcodex |= opcodez;
    data.Initialize(opcodex);

    
    int j = 3;
    int x = 0;
    do 
    {
        if (xstring[j] == '\0')
        {
            uint32 HexValue;
            sscanf(&xstring[j+1], "%x", &HexValue);
            if (HexValue > 0xFF)
            {
                sLog.outDebug("[Debug][Sendpacket] Packet is invalid");
                return false;
            }
            data << uint8(HexValue);
            //j++;
        }
        j++;
    } while(j < arg_len);

    data.hexlike();

    m_session->SendPacket(&data);

    sLog.outDebug("[Debug][Sendpacket] Packet was send");
#endif
    return true;
}

ASCENT_INLINE std::string MyConvertIntToString(const int arg)
{
	stringstream out;
	out << arg;
	return out.str();
}

ASCENT_INLINE std::string MyConvertFloatToString(const float arg)
{
	stringstream out;
	out << arg;
	return out.str();
}

string RemoveQuestFromPlayer(Player *plr, Quest *qst)
{
	bool has_quest = false;
	std::string recout = "|cff00ff00";

	//if (plr->HasQuests())
    //	{
	if (plr->HasFinishedQuest(qst->id))
	{
		if( plr->m_finishedQuests.find(qst->id) != plr->m_finishedQuests.end())
	    {
		 plr->m_finishedQuests.erase(qst->id);
		 recout += "Quest removed from finished quests history.\n\n";
		 has_quest = true;
	    }
		
	}
		//else
	if ( plr->HasFinishedDailyQuest(qst->id) )
	{
		if( plr->m_finishedDailyQuests.find(qst->id) != plr->m_finishedDailyQuests.end())
		{
			plr->m_finishedDailyQuests.erase(qst->id);
			recout += "Quest removed from finished dailies history.\n\n";
			has_quest = true;
		}
	}

    if (plr->HasQuests())
	{
		QuestLogEntry * qLogEntry = plr->GetQuestLogForEntry(qst->id);
		if (qLogEntry)
		{	
			CALL_QUESTSCRIPT_EVENT(qLogEntry, OnQuestCancel)(plr);
			qLogEntry->Finish();
			// Remove all items given by the questgiver at the beginning
			for(uint32 i = 0; i < 4; ++i)
			{
				
				if(qst->RecItemDuringQuestId[i])
					plr->GetItemInterface()->RemoveItemAmt(qst->RecItemDuringQuestId[i], 1 );
			}
			if(qst->time > 0) plr->timed_quest_slot = 0;
			plr->UpdateNearbyGameObjects();
			recout += "Quest removed from current questlog.";
		}
		else
			recout += "Aucune quete trouvée pour ce joueur.";
	}

	
    if(!has_quest)
	{
		recout += "Aucune quete a supprimer.";
	}

	recout += "\n\n";

	return recout;
}

bool ChatHandler::HandleQuestLookupCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	string x = string(args);
	ASCENT_TOLOWER(x);
	if(x.length() < 4)
	{
		RedSystemMessage(m_session, "Your search string must be at least 5 characters long.");
		return true;
	}

	BlueSystemMessage(m_session, "Starting search of quests `%s`...", x.c_str());
	uint32 t = getMSTime();

	StorageContainerIterator<Quest> * itr = QuestStorage.MakeIterator();

	Quest * i;
	uint32 count = 0;
	string y;
	string recout;

	while(!itr->AtEnd())
	{
		i = itr->Get();

		y = string(i->title);
		ASCENT_TOLOWER(y);

		if(FindXinYString(x, y))
		{
			string questid = MyConvertIntToString(i->id);
			const char * questtitle = i->title;
			recout = "|cff00ccff";
			recout += questid.c_str();
			recout += ": ";
			recout += questtitle;
			recout += "\n";
			SendMultilineMessage(m_session, recout.c_str());

			++count;
			if(count == 25)
			{
				RedSystemMessage(m_session, "More than 25 results returned. aborting.");
				break;
			}
		}
		if(!itr->Inc())
			break;
	}
	itr->Destruct();

	if (count == 0)
	{
		recout = "|cff00ccffNo matches found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
	}

	BlueSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);

	return true;
}

bool ChatHandler::HandleQuestStatusCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	Player *plr = getSelectedChar(m_session, true);
	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}

	uint32 quest_id = atol(args);
	std::string recout = "|cff00ff00";

	Quest * qst = QuestStorage.LookupEntry(quest_id);
	if(qst)
	{
		if (plr->HasFinishedQuest(quest_id))
			recout += "Player has already completed that quest.";
		else
		{
			QuestLogEntry * IsPlrOnQuest = plr->GetQuestLogForEntry(quest_id);
			if (IsPlrOnQuest)
				recout += "Player is currently on that quest.";
			else
				recout += "Player has NOT finished that quest.";
		}
	}
	else
	{
		recout += "Quest Id [";
		recout += args;
		recout += "] was not found and unable to add it to the player's quest log.";
	}

	recout += "\n\n";

	SendMultilineMessage(m_session, recout.c_str());

	return true;
}

bool ChatHandler::HandleQuestStartCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	Player *plr = getSelectedChar(m_session, true);
	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}

	uint32 quest_id = atol(args);
	std::string recout = "|cff00ff00";

	Quest * qst = QuestStorage.LookupEntry(quest_id);
	if(qst)
	{
		if (plr->HasFinishedQuest(quest_id))
			recout += "Le joueur a deja fait cette quete.";
		else
		{
			QuestLogEntry * IsPlrOnQuest = plr->GetQuestLogForEntry(quest_id);
			if (IsPlrOnQuest)
				recout += "Le joueur fait deja cette quete.";
			else
			{
				int32 open_slot = plr->GetOpenQuestSlot();

				if (open_slot == -1)
				{
					sQuestMgr.SendQuestLogFull(plr);
					recout += "Le journal de quete du joueur est plien.";
				}
				else
				{
					QuestLogEntry *qle = new QuestLogEntry();
					qle->Init(qst, plr, (uint32)open_slot);
					qle->UpdatePlayerFields();
		
					// If the quest should give any items on begin, give them the items.
					for(uint32 i = 0; i < 4; ++i)
					{
						if(qst->RecItemDuringQuestId[i])
						{
							Item *item = objmgr.CreateItem( qst->RecItemDuringQuestId[i], plr);
							if(!plr->GetItemInterface()->AddItemToFreeSlot(item))
								delete item;
						}
					}

					if(qst->srcitem && qst->srcitem != qst->RecItemDuringQuestId[0])
					{
						Item * item = objmgr.CreateItem( qst->srcitem, plr);
						if(item)
						{
							item->SetUInt32Value(ITEM_FIELD_STACK_COUNT, qst->srcitemcount ? qst->srcitemcount : 1);
							if(!plr->GetItemInterface()->AddItemToFreeSlot(item))
								delete item;
						}
					}
				

					//if(qst->count_required_item || qst_giver->GetTypeId() == TYPEID_GAMEOBJECT)	// gameobject quests deactivate
					//	plr->UpdateNearbyGameObjects();
					//ScriptSystem->OnQuestEvent(qst, static_cast< Creature* >( qst_giver ), _player, QUEST_EVENT_ON_ACCEPT);
				
					// sHookInterface.OnQuestAccept( plr, qst );
					try
					{
					 sHookInterface.OnQuestAccept( plr, qst, NULL );
					}
					catch(...)
					{
						Log.Error("HandleQuestStartCommand","sHookInterface.OnQuestAccept(%s,%u)",plr->GetName(),qst->id);
					}

					recout += "La quete a ete ajoute au journal de quete du joueur.";
				}
			}
		}
	}
	else
	{
		recout += "Quest Id [";
		recout += args;
		recout += "] n'a pas ete trouvee (n'existe pas).";
	}

	recout += "\n\n";

	SendMultilineMessage(m_session, recout.c_str());

	return true;
}

bool ChatHandler::HandleQuestFinishCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	Player *plr = getSelectedChar(m_session, true);
	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}

	uint32 quest_id = atol(args);
	std::string recout = "|cff00ff00";

	Quest * qst = QuestStorage.LookupEntry(quest_id);
	if(qst)
	{
		if (plr->HasFinishedQuest(quest_id) || plr->HasFinishedDailyQuest(quest_id))
			recout += "Player has already completed that quest.\n\n";
		else if(qst->is_repeatable == REPEATABLE_DAILY && plr->GetFinishedDailiesCount() >= 25)
			recout += "Player has reached the maximum number of completed daily quests.\n\n";
		else
		{
			QuestLogEntry * IsPlrOnQuest = plr->GetQuestLogForEntry(quest_id);
			if (IsPlrOnQuest)
			{	
				uint32 giver_id = 0;
				std::string my_query = "";

				my_query = "SELECT id FROM creature_quest_starter WHERE quest = " + string(args);
				QueryResult *creatureResult = WorldDatabase.Query(my_query.c_str());

				if(creatureResult)
				{
					Field *creatureFields = creatureResult->Fetch();
					giver_id = creatureFields[0].GetUInt32();
					delete creatureResult;
				}
				else
				{
					my_query = "SELECT id FROM gameobject_quest_starter WHERE quest = " + string(args);
					QueryResult *objectResult = WorldDatabase.Query(my_query.c_str());
					if(objectResult)
					{
						Field *objectFields = objectResult->Fetch();
						giver_id = objectFields[0].GetUInt32();
						delete objectResult;
					}
				}

				if(giver_id == 0)
					SystemMessage(m_session, "Unable to find quest giver creature or object.");
				else
				{
					// I need some way to get the guid without targeting the creature or looking through all the spawns...
					Object *quest_giver = NULL;

					for(uint32 guid=1; guid < plr->GetMapMgr()->m_CreatureArraySize; guid++)
					{
						Creature *pCreature = plr->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
						if(pCreature)
						{
							if(pCreature->GetEntry() == giver_id) //found creature
							{
								quest_giver = (Object*)pCreature;
								guid = plr->GetMapMgr()->m_CreatureArraySize;
							}
						}
					}

					if(quest_giver)
					{
						GreenSystemMessage(m_session, "Found a quest_giver creature.");
						//WorldPacket data;
						//sQuestMgr.BuildOfferReward(&data, qst, quest_giver, 1);
						//m_session->SendPacket(&data);
						sQuestMgr.GiveQuestRewardReputation(plr, qst, quest_giver);
					}
					else
						RedSystemMessage(m_session, "Unable to find quest_giver object.");
				}

				sQuestMgr.GenerateQuestXP(plr, qst);
				sQuestMgr.BuildQuestComplete(plr, qst);

				IsPlrOnQuest->Finish();
				recout += "Player was on that quest, but has now completed it.";
			}
			else
				recout += "The quest has now been completed for that player.";

			switch (qst->is_repeatable)
			{
			case 0: plr->AddToFinishedQuests(quest_id); break;
			case 2: plr->m_finisheddailyQuests.insert(quest_id); break;
			}
		}
	}
	else
	{
		recout += "Quest Id [";
		recout += args;
		recout += "] was not found and unable to add it to the player's quest log.";
	}

	recout += "\n\n";

	SendMultilineMessage(m_session, recout.c_str());

	return true;
}

bool ChatHandler::HandleQuestItemCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	std::string my_item_lookup = "SELECT item, item_count FROM gameobject_quest_item_binding WHERE quest = " + string(args);

	QueryResult *result = WorldDatabase.Query(my_item_lookup.c_str());

	std::string recout;

	if(!result)
	{
		recout = "|cff00ccffNo matches found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
		return true;
	}

	recout = "|cff00ff00Quest item matches: itemid: count -> Name\n\n";
	SendMultilineMessage(m_session, recout.c_str());

	uint32 count = 0;

	do
	{
		Field *fields = result->Fetch();
		uint32 id = fields[0].GetUInt32();
		string itemid  = MyConvertIntToString(id);
		string itemcnt = MyConvertIntToString(fields[1].GetUInt32());
		ItemPrototype* tmpItem = ItemPrototypeStorage.LookupEntry(id);
		recout = "|cff00ccff";
		recout += itemid.c_str();
		recout += ": ";
		recout += itemcnt.c_str();
		recout += " -> ";
		recout += tmpItem->Name1;
		recout += "\n";

		SendMultilineMessage(m_session, recout.c_str());

		count++;
		
		if(count == 25)
		{
			RedSystemMessage(m_session, "More than 25 results returned. aborting.");
			break;
		}
	}while (result->NextRow());

	delete result;

	if (count == 0)
	{
		recout = "|cff00ccffNo matches found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
	}

	return true;
}

bool ChatHandler::HandleQuestGiverCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	std::string recout;

	std::string my_query1 = "SELECT id FROM creature_quest_starter WHERE quest = " + string(args);
	QueryResult *objectResult1 = WorldDatabase.Query(my_query1.c_str());

	if(objectResult1)
	{
		Field *fields = objectResult1->Fetch();
		std::string creatureId1 = MyConvertIntToString(fields[0].GetUInt32());

		delete objectResult1;

		std::string creatureName1 = "N/A";
		CreatureInfo *creatureResult1 = CreatureNameStorage.LookupEntry(atol(creatureId1.c_str()));
		if (creatureResult1)
		{
			creatureName1 = creatureResult1->Name;

			my_query1 = "SELECT id FROM creature_spawns WHERE entry = " + creatureId1;
			QueryResult *spawnResult1 = WorldDatabase.Query(my_query1.c_str());

			string spawnId1;
			if(spawnResult1)
			{
				Field *fields = spawnResult1->Fetch();
				spawnId1 = fields[0].GetString();

				delete spawnResult1;
			}
			else
				spawnId1 = "N/A";

			recout = "|cff00ccffQuest Starter found: creature id, spawnid, name\n\n";
			SendMultilineMessage(m_session, recout.c_str());

			recout = "|cff00ccff";
			recout += creatureId1.c_str();
			recout += ", ";
			recout += spawnId1.c_str();
			recout += ", ";
			recout += creatureName1.c_str();
			recout += "\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}
		else
		{
			recout = "|cff00ccffNo creature quest starter info found.\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}

	}
	else
	{
		recout = "|cff00ccffNo creature quest starters found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
	}

	std::string my_query2 = "SELECT id FROM gameobject_quest_starter WHERE quest = " + string(args);
	QueryResult *objectResult2 = WorldDatabase.Query(my_query2.c_str());

	if(objectResult2)
	{
		Field *fields = objectResult2->Fetch();
		std::string itemId2 = MyConvertIntToString(fields[0].GetUInt32());

		delete objectResult2;

		std::string itemName2 = "N/A";
		ItemPrototype *itemResult2 = ItemPrototypeStorage.LookupEntry(atol(itemId2.c_str()));
		if (itemResult2)
		{
			itemName2 = itemResult2->Name1;

			my_query2 = "SELECT id FROM gameobject_spawns WHERE entry = " + itemId2;
			QueryResult *spawnResult2 = WorldDatabase.Query(my_query2.c_str());

			string spawnId2;
			if(spawnResult2)
			{
				Field *fields = spawnResult2->Fetch();
				spawnId2 = fields[0].GetString();

				delete spawnResult2;
			}
			else
				spawnId2 = "N/A";

			recout = "|cff00ccffQuest starter found: object id, spawnid, name\n\n";
			SendMultilineMessage(m_session, recout.c_str());

			recout = "|cff00ccff";
			recout += itemId2.c_str();
			recout += ", ";
			recout += spawnId2.c_str();
			recout += ", ";
			recout += itemName2.c_str();
			recout += "\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}
		else
		{
			recout = "|cff00ccffNo object quest starter info found.\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}
	}
	else
	{
		recout = "|cff00ccffNo object quest starters found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
	}

	return true;
}

bool ChatHandler::HandleQuestListCommand(const char * args, WorldSession * m_session)
{
	uint32 quest_giver = 0;
	if(*args)
		quest_giver = atol(args);
	else
	{
		uint64 guid = m_session->GetPlayer()->GetSelection();
		if (guid == 0)
		{
			SystemMessage(m_session, "You must target an npc or specify an id.");
			return true;
		}

		Creature *unit = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(unit)
		{
			if (!unit->isQuestGiver())
			{
				SystemMessage(m_session, "Unit is not a valid quest giver.");
				return true;
			}

			if (!unit->HasQuests())
			{
				SystemMessage(m_session, "NPC does not have any quests.");
				return true;
			}

			quest_giver = unit->GetEntry();
		}
	}

	string recout = "|cff00ff00Quest matches: id: title\n\n";
	SendMultilineMessage(m_session, recout.c_str());

	uint32 count = 0;
	uint32 quest_id = 0;
	Quest * qst;
	Field *fields;

	if(quest_giver == 0)
	{
		Player *plr = getSelectedChar(m_session, true);
		if(!plr)
		{
			plr = m_session->GetPlayer();
			SystemMessage(m_session, "Auto-targeting self.");
		}

		if(plr)
		{
			if(plr->HasQuests())
			{
				QueryResult *playerResult = CharacterDatabase.Query("SELECT quest_id FROM questlog WHERE player_guid=%u", plr->GetLowGUID());
				if(playerResult)
				{
					do 
					{
						fields = playerResult->Fetch();
						quest_id = fields[0].GetUInt32();

						qst = QuestStorage.LookupEntry(quest_id);

						string qid  = MyConvertIntToString(quest_id);
						const char * qname = qst->title;

						recout = "|cff00ccff";
						recout += qid.c_str();
						recout += ": ";
						recout += qname;
						recout += "\n";

						SendMultilineMessage(m_session, recout.c_str());

						count++;
						
						if(count == 25)
						{
							RedSystemMessage(m_session, "More than 25 results returned. aborting.");
							break;
						}

					} while(playerResult->NextRow());

					delete playerResult;
				}
			}
		}
	}
	else
	{
		QueryResult *creatureResult = WorldDatabase.Query("SELECT quest FROM creature_quest_starter WHERE id = %u", quest_giver);

		if(!creatureResult)
		{
			recout = "|cff00ccffNo quests found for the specified NPC id.\n\n";
			SendMultilineMessage(m_session, recout.c_str());
			return true;
		}

		do
		{
			Field *fields = creatureResult->Fetch();
			uint32 quest_id = fields[0].GetUInt32();

			qst = QuestStorage.LookupEntry(quest_id);
			if(qst==NULL)
				continue;

			string qid  = MyConvertIntToString(quest_id);
			const char * qname = qst->title;

			recout = "|cff00ccff";
			recout += qid.c_str();
			recout += ": ";
			recout += qname;
			recout += "\n";

			SendMultilineMessage(m_session, recout.c_str());

			count++;
			
			if(count == 25)
			{
				RedSystemMessage(m_session, "More than 25 results returned. aborting.");
				break;
			}
		}while (creatureResult->NextRow());

		delete creatureResult;
	}

	if (count == 0)
	{
		recout = "|cff00ccffNo matches found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
	}

	return true;
}

bool ChatHandler::HandleQuestAddStartCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	Creature *unit = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!unit)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	if (!unit->isQuestGiver())
	{
		SystemMessage(m_session, "Unit is not a valid quest giver.");
		return false;
	}

	uint32 quest_id = atol(args);
	Quest * qst = QuestStorage.LookupEntry(quest_id);

	if (qst == NULL)
	{
		SystemMessage(m_session, "Invalid quest selected, unable to add quest to the specified NPC.");
		return false;
	}

	std::string quest_giver = MyConvertIntToString(unit->GetEntry());

	std::string my_query1 = "SELECT id FROM creature_quest_starter WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult1 = WorldDatabase.Query(my_query1.c_str());
	if (selectResult1)
	{
		delete selectResult1;
		SystemMessage(m_session, "Quest was already found for the specified NPC.");
	}
	else
	{
		std::string my_insert1 = "INSERT INTO creature_quest_starter (id, quest) VALUES (" + quest_giver + "," + string(args) + ")";
		QueryResult *insertResult1 = WorldDatabase.Query(my_insert1.c_str());
		if (insertResult1)
			delete insertResult1;
	}

	std::string my_query2 = "SELECT id FROM gameobject_quest_starter WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult2 = WorldDatabase.Query(my_query2.c_str());
	if (selectResult2)
		delete selectResult2;
	else
	{
		std::string my_insert2 = "INSERT INTO gameobject_quest_starter (id, quest) VALUES (" + quest_giver + "," + string(args) + ")";
		QueryResult *insertResult2 = WorldDatabase.Query(my_insert2.c_str());
		if (insertResult2)
			delete insertResult2;
	}

	sQuestMgr.LoadExtraQuestStuff();

	QuestRelation *qstrel = new QuestRelation;
	qstrel->qst = qst;
	qstrel->type = QUESTGIVER_QUEST_START;
	uint8 qstrelid = (uint8)unit->GetQuestRelation(quest_id);
	unit->FindQuest(quest_id, qstrelid);
	unit->AddQuest(qstrel);
	unit->_LoadQuests();

	const char * qname = qst->title;

	std::string recout = "|cff00ff00Added Quest to NPC as starter: ";
	recout += "|cff00ccff";
	recout += qname;
	recout += "\n\n";
	SendMultilineMessage(m_session, recout.c_str());

	return true;
}

bool ChatHandler::HandleQuestAddFinishCommand(const char * args, WorldSession * m_session)
{
	if(!*args)
		return false;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	Creature *unit = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!unit)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	if (!unit->isQuestGiver())
	{
		SystemMessage(m_session, "Unit is not a valid quest giver.");
		return false;
	}

	uint32 quest_id = atol(args);
	Quest * qst = QuestStorage.LookupEntry(quest_id);

	if (qst == NULL)
	{
		SystemMessage(m_session, "Invalid quest selected, unable to add quest to the specified NPC.");
		return false;
	}

	std::string quest_giver = MyConvertIntToString(unit->GetEntry());

	std::string my_query1 = "SELECT id FROM creature_quest_finisher WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult1 = WorldDatabase.Query(my_query1.c_str());
	if (selectResult1)
	{
		delete selectResult1;
		SystemMessage(m_session, "Quest was already found for the specified NPC.");
	}
	else
	{
		std::string my_insert1 = "INSERT INTO creature_quest_finisher (id, quest) VALUES (" + quest_giver + "," + string(args) + ")";
		QueryResult *insertResult1 = WorldDatabase.Query(my_insert1.c_str());
		if (insertResult1)
			delete insertResult1;
	}

	std::string my_query2 = "SELECT id FROM gameobject_quest_finisher WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult2 = WorldDatabase.Query(my_query2.c_str());
	if (selectResult2)
		delete selectResult2;
	else
	{
		string my_insert2 = "INSERT INTO gameobject_quest_finisher (id, quest) VALUES (" + quest_giver + "," + string(args) + ")";
		QueryResult *insertResult2 = WorldDatabase.Query(my_insert2.c_str());
		if (insertResult2)
			delete insertResult2;
	}

	sQuestMgr.LoadExtraQuestStuff();

	QuestRelation *qstrel = new QuestRelation;
	qstrel->qst = qst;
	qstrel->type = QUESTGIVER_QUEST_END;
	uint8 qstrelid = (uint8)unit->GetQuestRelation(quest_id);
	unit->FindQuest(quest_id, qstrelid);
	unit->AddQuest(qstrel);
	unit->_LoadQuests();

	const char * qname = qst->title;

	std::string recout = "|cff00ff00Added Quest to NPC as finisher: ";
	recout += "|cff00ccff";
	recout += qname;
	recout += "\n\n";
	SendMultilineMessage(m_session, recout.c_str());

	return true;
}

bool ChatHandler::HandleQuestAddBothCommand(const char * args, WorldSession * m_session)
{
	if(!*args)
		return false;

	bool bValid = ChatHandler::HandleQuestAddStartCommand(args, m_session);

	if (bValid)
		ChatHandler::HandleQuestAddFinishCommand(args, m_session);

	return true;
}

bool ChatHandler::HandleQuestDelStartCommand(const char * args, WorldSession * m_session)
{
	if(!*args)
		return false;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	Creature *unit = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!unit)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	if (!unit->isQuestGiver())
	{
		SystemMessage(m_session, "Unit is not a valid quest giver.");
		return false;
	}

	uint32 quest_id = atol(args);
	Quest * qst = QuestStorage.LookupEntry(quest_id);

	if (qst == NULL)
	{
		SystemMessage(m_session, "Invalid Quest selected.");
		return false;
	}

	std::string quest_giver = MyConvertIntToString(unit->GetEntry());

	std::string my_query1 = "SELECT id FROM creature_quest_starter WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult1 = WorldDatabase.Query(my_query1.c_str());
	if (selectResult1)
		delete selectResult1;
	else
	{
		SystemMessage(m_session, "Quest was NOT found for the specified NPC.");
		return false;
	}

	std::string my_delete1 = "DELETE FROM creature_quest_starter WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *deleteResult1 = WorldDatabase.Query(my_delete1.c_str());
	if (deleteResult1)
		delete deleteResult1;

	std::string my_query2 = "SELECT id FROM gameobject_quest_starter WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult2 = WorldDatabase.Query(my_query2.c_str());
	if (selectResult2)
	{
		delete selectResult2;

		std::string my_delete2 = "DELETE FROM gameobject_quest_starter WHERE id = " + quest_giver + " AND quest = " + string(args);
		QueryResult *deleteResult2 = WorldDatabase.Query(my_delete2.c_str());
		if (deleteResult2)
			delete deleteResult2;
	}

	sQuestMgr.LoadExtraQuestStuff();

	QuestRelation *qstrel = new QuestRelation;
	qstrel->qst = qst;
	qstrel->type = QUESTGIVER_QUEST_START;
	uint8 qstrelid = (uint8)unit->GetQuestRelation(quest_id);
	unit->FindQuest(quest_id, qstrelid);
	unit->DeleteQuest(qstrel);
	unit->_LoadQuests();

	const char * qname = qst->title;

	std::string recout = "|cff00ff00Deleted Quest from NPC: ";
	recout += "|cff00ccff";
	recout += qname;
	recout += "\n\n";
	SendMultilineMessage(m_session, recout.c_str());

	return true;
}

bool ChatHandler::HandleQuestDelFinishCommand(const char * args, WorldSession * m_session)
{
	if(!*args)
		return false;

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	Creature *unit = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!unit)
	{
		SystemMessage(m_session, "You must target an npc.");
		return false;
	}

	if (!unit->isQuestGiver())
	{
		SystemMessage(m_session, "Unit is not a valid quest giver.");
		return false;
	}

	uint32 quest_id = atol(args);
	Quest * qst = QuestStorage.LookupEntry(quest_id);

	if (qst == NULL)
	{
		SystemMessage(m_session, "Invalid Quest selected.");
		return false;
	}

	std::string quest_giver = MyConvertIntToString(unit->GetEntry());

	std::string my_query1 = "SELECT id FROM creature_quest_finisher WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult1 = WorldDatabase.Query(my_query1.c_str());
	if (selectResult1)
		delete selectResult1;
	else
	{
		SystemMessage(m_session, "Quest was NOT found for the specified NPC.");
		return true;
	}

	std::string my_delete1 = "DELETE FROM creature_quest_finisher WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *deleteResult1 = WorldDatabase.Query(my_delete1.c_str());
	if (deleteResult1)
		delete deleteResult1;

	std::string my_query2 = "SELECT id FROM gameobject_quest_finisher WHERE id = " + quest_giver + " AND quest = " + string(args);
	QueryResult *selectResult2 = WorldDatabase.Query(my_query2.c_str());
	if (selectResult2)
	{
		delete selectResult2;

		std::string my_delete2 = "DELETE FROM gameobject_quest_finisher WHERE id = " + quest_giver + " AND quest = " + string(args);
		QueryResult *deleteResult2 = WorldDatabase.Query(my_delete2.c_str());
		if (deleteResult2)
			delete deleteResult2;
	}

	sQuestMgr.LoadExtraQuestStuff();

	QuestRelation *qstrel = new QuestRelation;
	qstrel->qst = qst;
	qstrel->type = QUESTGIVER_QUEST_END;
	uint8 qstrelid = (uint8)unit->GetQuestRelation(quest_id);
	unit->FindQuest(quest_id, qstrelid);
	unit->DeleteQuest(qstrel);
	unit->_LoadQuests();

	const char * qname = qst->title;

	std::string recout = "|cff00ff00Deleted Quest from NPC: ";
	recout += "|cff00ccff";
	recout += qname;
	recout += "\n\n";
	SendMultilineMessage(m_session, recout.c_str());

	return true;
}

bool ChatHandler::HandleQuestDelBothCommand(const char * args, WorldSession * m_session)
{
	if(!*args)
		return false;

	bool bValid = ChatHandler::HandleQuestDelStartCommand(args, m_session);

	if (bValid)
		ChatHandler::HandleQuestDelFinishCommand(args, m_session);

	return true;
}

bool ChatHandler::HandleQuestFinisherCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	std::string recout;

	std::string my_query1 = "SELECT id FROM creature_quest_finisher WHERE quest = " + string(args);
	QueryResult *objectResult1 = WorldDatabase.Query(my_query1.c_str());

	if(objectResult1)
	{
		Field *fields = objectResult1->Fetch();
		std::string creatureId1 = MyConvertIntToString(fields[0].GetUInt32());

		delete objectResult1;

		std::string creatureName1 = "N/A";
		CreatureInfo *creatureResult1 = CreatureNameStorage.LookupEntry(atol(creatureId1.c_str()));

		if(creatureResult1)
		{
			creatureName1 = creatureResult1->Name;

			my_query1 = "SELECT id FROM creature_spawns WHERE entry = " + creatureId1;
			QueryResult *spawnResult1 = WorldDatabase.Query(my_query1.c_str());

			string spawnId1;
			if(spawnResult1)
			{
				Field *fields = spawnResult1->Fetch();
				spawnId1 = fields[0].GetString();

				delete spawnResult1;
			}
			else
				spawnId1 = "N/A";

			recout = "|cff00ccffQuest Finisher found: creature id, spawnid, name\n\n";
			SendMultilineMessage(m_session, recout.c_str());

			recout = "|cff00ccff";
			recout += creatureId1.c_str();
			recout += ", ";
			recout += spawnId1.c_str();
			recout += ", ";
			recout += creatureName1.c_str();
			recout += "\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}
		else
		{
			recout = "|cff00ccffNo creature quest finisher info found.\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}
	}
	else
	{
		recout = "|cff00ccffNo creature quest finishers found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
	}

	std::string my_query2 = "SELECT id FROM gameobject_quest_finisher WHERE quest = " + string(args);
	QueryResult *objectResult2 = WorldDatabase.Query(my_query2.c_str());

	if(objectResult2)
	{
		Field *fields = objectResult2->Fetch();
		std::string itemId2 = MyConvertIntToString(fields[0].GetUInt32());

		delete objectResult2;

		std::string itemName2 = "N/A";
		ItemPrototype *itemResult2 = ItemPrototypeStorage.LookupEntry(atol(itemId2.c_str()));
		if (itemResult2)
		{
			itemName2 = itemResult2->Name1;

			my_query2 = "SELECT id FROM gameobject_spawns WHERE entry = " + itemId2;
			QueryResult *spawnResult2 = WorldDatabase.Query(my_query2.c_str());

			string spawnId2;
			if(spawnResult2)
			{
				Field *fields = spawnResult2->Fetch();
				spawnId2 = fields[0].GetString();

				delete spawnResult2;
			}
			else
				spawnId2 = "N/A";

			recout = "|cff00ccffQuest Finisher found: object id, spawnid, name\n\n";
			SendMultilineMessage(m_session, recout.c_str());

			recout = "|cff00ccff";
			recout += itemId2.c_str();
			recout += ", ";
			recout += spawnId2.c_str();
			recout += ", ";
			recout += itemName2.c_str();
			recout += "\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}
		else
		{
			recout = "|cff00ccffNo object quest finisher info found.\n\n";
			SendMultilineMessage(m_session, recout.c_str());
		}
	}
	else
	{
		recout = "|cff00ccffNo object quest finishers found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
	}

	return true;
}

bool ChatHandler::HandleQuestSpawnCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	std::string recout;

	std::string my_query = "SELECT id FROM creature_quest_starter WHERE quest = " + string(args);
	QueryResult *objectResult = WorldDatabase.Query(my_query.c_str());

	string starterId;
	if(objectResult)
	{
		Field *fields = objectResult->Fetch();
		starterId = MyConvertIntToString(fields[0].GetUInt32());
	}
	else
	{
		recout = "|cff00ccffNo quest starters found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
		return true;
	}

	delete objectResult;

	std::string starterName = "N/A";
	CreatureInfo *creatureResult = CreatureNameStorage.LookupEntry(atol(starterId.c_str()));

	if(creatureResult)
		starterName = creatureResult->Name;
	else
	{
		recout = "|cff00ccffNo quest starter info found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
		return true;
	}

	my_query = "SELECT map, position_x, position_y, position_z FROM creature_spawns WHERE entry = " + starterId;
	QueryResult *spawnResult = WorldDatabase.Query(my_query.c_str());

	if(!spawnResult)
	{
		recout = "|cff00ccffNo spawn location for quest starter was found.\n\n";
		SendMultilineMessage(m_session, recout.c_str());
		return true;
	}

	Field *fields = spawnResult->Fetch();
	uint32 locmap = fields[0].GetUInt32();
	float x = fields[1].GetFloat();
	float y = fields[2].GetFloat();
	float z = fields[3].GetFloat();

	delete spawnResult;

	recout = "|cff00ccffPorting to Quest Starter/Giver: id, name\n\n";
	SendMultilineMessage(m_session, recout.c_str());

	recout = "|cff00ccff";
	recout += starterId.c_str();
	recout += ", ";
    recout += starterName.c_str();
	recout += "\n\n";
	SendMultilineMessage(m_session, recout.c_str());

	m_session->GetPlayer()->SafeTeleport(locmap, 0, LocationVector(x, y, z));

	return true;
}

bool ChatHandler::HandleQuestLoadCommand(const char * args, WorldSession * m_session)
{
	BlueSystemMessage(m_session, "Load of quests from the database has been initiated ...", "");
	uint32 t = getMSTime();

	sQuestMgr.LoadExtraQuestStuff();

	BlueSystemMessage(m_session, "Load completed in %u ms.", getMSTime() - t);

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
		return true;

	Creature *unit = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!unit)
		return true;

	if (!unit->isQuestGiver())
		return true;

	// If player targeted a questgiver assume they want the NPC reloaded, too
	unit->_LoadQuests();

	return true;
}

bool ChatHandler::HandleQuestRemoveCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	Player *plr = getSelectedChar(m_session, true);
	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}

	string recout = "";
	uint32 quest_id = atol(args);
	Quest * qst = QuestStorage.LookupEntry(quest_id);

	if(qst)
		recout = RemoveQuestFromPlayer(plr, qst);
	else
		recout = "Invalid quest selected, unable to remove.\n\n";

	SystemMessage(m_session, recout.c_str());

	return true;
}

bool ChatHandler::HandleQuestRewardCommand(const char * args, WorldSession * m_session)
{
	if(!*args) return false;

	string recout = "";

	recout += "\n\n";

	SendMultilineMessage(m_session, recout.c_str());

	return true;
}
//---------------------------------------------------
bool ChatHandler::HandleModifyHappinessCommand(const char* args, WorldSession *m_session)
{
	uint32 Happiness_Value = 0;
	uint32 Loyalty_Value   = 0;
	int32 NbParam = 0;
	PlayerPet *plrPet=NULL;

	if(!*args)
	{
		HandleModifyHapinesseCommandHelp(m_session);
		return(false);
	}

	Player *plr = getSelectedChar(m_session, true);
    if(!plr) // Secu, ne peut pas arriver (Branruz)
	{
	   // Le player a sélectionné un pnj
	   return(true);
	}

	Pet *plr_pet = plr->GetSummon();
	if(!plr_pet)
	{
        GreenSystemMessage(m_session, "Un familier doit etre invoqué.");
		return(true);
	}

    NbParam = sscanf(args, "%u %u", &Happiness_Value, &Loyalty_Value);
	if( NbParam < 1)
	{
		return(false);
	}

    //GreenSystemMessage(m_session, "Param          : %u",NbParam);
	//GreenSystemMessage(m_session, "Happiness_Value: %u",Happiness_Value);
	//GreenSystemMessage(m_session, "Loyalty_Value  : %u",Loyalty_Value);

	if(NbParam >= 1)
	{
	 switch(Happiness_Value)
	 {
	  case 1 : // UNHAPPY
	  case 2 : // CONTENT
	  case 3 : // HAPPY 
		      
		  plrPet = plr->GetPlayerPet(plr->GetUnstabledPetNumber());
		  if(!plrPet) // Secu: Ne peut pas arriver (Branruz)
		  {
			  RedSystemMessage(m_session, "Familier non trouvé, HappinessValue: %u",HappinessValue[Happiness_Value]);
			  return(true);
		  }
		  plr_pet->SetUInt32Value(UNIT_FIELD_POWER5, HappinessValue[Happiness_Value]);
		  GreenSystemMessage(m_session, "%s : UNIT_FIELD_POWER5 = %u",plrPet->name.c_str(),HappinessValue[Happiness_Value]);
		  break;

	  default : 
		       HandleModifyHapinesseCommandHelp(m_session);
		       return(false);
	 }
	}

    if(NbParam == 2)
	{
	 switch(Loyalty_Value) // Facultatif
	 {                     // Ecoute du player
   	  case 1 : // REBELIOUS	  - 15% - Rebel           UNHAPPY 
	  case 2 : // UNRULY	  - 30% - Indiscipliné
	  case 3 : // SUBMISIVE	  - 45% - Soumis
      case 4 : // DEPENDABLE  - 60% - Sûr             CONTENT
	  case 5 : // FAITHFUL	  - 75% - Fidele
	  case 6 : // BEST_FRIEND - 100% - Meilleur ami    HAPPY
		      plr_pet->SetLoyaltyLevel(Loyalty_Value);
			  plr_pet->SetLoyaltyPts(LoyLvlRange[Loyalty_Value]);
		      GreenSystemMessage(m_session, "%s : Loyalty = %u (Index %u)",plrPet->name.c_str(),LoyLvlRange[Loyalty_Value],Loyalty_Value);
		      return(true);

	  default : HandleModifyHapinesseCommandHelp(m_session);
		        return(false);
	 }
	}

	return(true); 

}
void ChatHandler::HandleModifyHapinesseCommandHelp(WorldSession *plrSession)
{
 // PET_UNHAPPY_VALUE	   0
 // PET_CONTENT_VALUE  333000
 // PET_HAPPY_VALUE    666000
 // HappinessRange[4] = { 0, PET_UNHAPPY_VALUE, PET_CONTENT_VALUE, PET_HAPPY_VALUE }; 
 // => SetUInt32Value(UNIT_FIELD_POWER5, HappinessRange[i]);
 GreenSystemMessage(plrSession, "Happiness : 1 - UNHAPPY"); // Mécontent  :o(
 GreenSystemMessage(plrSession, "            2 - CONTENT"); // Content    :o|
 GreenSystemMessage(plrSession, "            3 - HAPPY");   // Joyeux     :o)

 // LoyLvlRange[7] = { 0, 150, 300, 450, 600, 900, 1200 }; => SetByte( UNIT_FIELD_BYTES_1, 1, LoyLvlRange[i]);
 GreenSystemMessage(plrSession, "Loyalty   : 1 - REBELIOUS");   // Rebel        - Mécontent  :o(
 GreenSystemMessage(plrSession, "            2 - UNRULY");      // Indiscipliné
 GreenSystemMessage(plrSession, "            3 - SUBMISIVE");   // Soumis
 GreenSystemMessage(plrSession, "            4 - DEPENDABLE");  // Sûr          - Content    :o|
 GreenSystemMessage(plrSession, "            5 - FAITHFUL");    // Fidele        
 GreenSystemMessage(plrSession, "            6 - BEST_FRIEND"); // Meilleur ami - Joyeux     :o)
}
//---------------------------------------------------