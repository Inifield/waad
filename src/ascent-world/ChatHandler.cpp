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

extern std::string LogFileName;
extern bool bLogChat;

static const uint32 LanguageSkills[NUM_LANGUAGES] = 
{
	0,				//  0 - UNIVERSAL	
	109,			//  1 - ORCISH		
	113,			//  2 - DARNASSIAN	
	115,			//  3 - TAURAHE		
	0,				//  4 -	Unused			
	0,				//  5 -	Unused					
	111,			//  6 - DWARVISH	
	98,				//  7 - COMMON		
	139,			//  8 - DEMON TONGUE
	140,			//  9 - TITAN	
	137,			// 10 - THALSSIAN
	138,			// 11 - DRACONIC
	0,				// 12 - KALIMAG
	313,			// 13 - GNOMISH
	315,			// 14 - TROLL
	0,				// 15 -
	0,				// 16 -
	0,				// 17 -
	0,				// 18 -
	0,				// 19 -
	0,				// 20 -
	0,				// 21 -
	0,				// 22 -
	0,				// 23 -
	0,				// 24 -
	0,				// 25 -
	0,				// 26 -
	0,				// 27 -
	0,				// 28 -
	0,				// 29 -
	0,				// 30 -
	0,				// 31 -
	0,				// 32 -
	673,			// 33 - Bas-parler / GutterSpeak
	0,				// 34 -
	759,			// 35 - Dranei
	0,              // 36 - Zombie (Pointillés) 
	0,              // 37 - Gnome (Code Binaire)
	0,              // 38 - Goblin (Code Binaire)
};

void WorldSession::HandleMessagechatOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 9);
	WorldPacket *data;
	if(!_player->IsInWorld()) return;

	uint32 type;
	int32 lang;
	string misc ;
	string msg ;

	recv_data >> type;
	recv_data >> lang;

	Log.Warning("HandleMessagechatOpcode","(%s) Chat Opcode recu: Type: %u, Lang: %u",GetPlayer()->GetName(),type,lang);


	if( lang >= NUM_LANGUAGES ) return;

	if(GetPlayer()->IsBanned())
	{
		GetPlayer()->BroadcastMessage("Vous ne pouvez pas parler car vous etes banni.");
		return;
	}

    // flood detection, wheeee! 
	if(lang != -1 && !HasPermissions() && sWorld.flood_lines)
	{	
		if(UNIXTIME >= floodTime)
		{
			floodLines = 0;
			floodTime = UNIXTIME + sWorld.flood_seconds;
		}

		if((++floodLines) > sWorld.flood_lines)
		{
			if(sWorld.flood_message)
			{
				_player->BroadcastMessage("Votre message a déclenché la protection flood du côté serveur."); 
				_player->BroadcastMessage("Vous pourrez de nouveau parler dans %u secondes.", floodTime - UNIXTIME);
			}

			return;
		}
	}
	
	// special misc , meilleur gestion (HearthStone)
	if( type == CHAT_MSG_CHANNEL || type == CHAT_MSG_WHISPER )
	{
		recv_data >> misc;
		recv_data >> msg;
	}
	else
		recv_data >> msg;


	// Script eventuel
	sHookInterface.OnChat(_player, type, lang, msg.c_str(), misc.c_str());	 

	//--------------------------
	//arghhh STFU. I'm not giving you gold or items NOOB
	switch(type)
	{
	   case CHAT_MSG_EMOTE:
	   case CHAT_MSG_SAY:
	   case CHAT_MSG_YELL:
	   case CHAT_MSG_WHISPER:
	   case CHAT_MSG_CHANNEL:
		  {
			if( m_muted && m_muted >= (uint32)UNIXTIME )
			{
				_player->BroadcastMessage("Vous etes muet a cause d'un moderateur.");
				_player->BroadcastMessage("Vous pourrez de nouveau parler dans %s secondes.", ConvertTimeStampToString(m_muted - (uint32)UNIXTIME).c_str());
				return;
			}
		  }break;
	 }


	if( type == CHAT_MSG_RAID && GetPlayer()->m_bg )     	type = CHAT_MSG_BATTLEGROUND;
	if( type == CHAT_MSG_RAID_LEADER && GetPlayer()->m_bg ) type = CHAT_MSG_BATTLEGROUND_LEADER;

	switch(type)
	{
	 case CHAT_MSG_EMOTE:
		{
         if(strlen(msg.c_str())==0) break;
		 
			if(GetPlayer()->m_modlanguage >=0)
				data = sChatHandler.FillMessageData( CHAT_MSG_EMOTE, GetPlayer()->m_modlanguage,  msg.c_str(), _player->GetGUID(), _player->bGMTagOn ? 4 : 0 );
			else	
				data = sChatHandler.FillMessageData( CHAT_MSG_EMOTE, sGMSecurityMgr.UseUniversalLang(permissions) ? LANG_UNIVERSAL : lang,  msg.c_str(), _player->GetGUID(), _player->bGMTagOn ? 4 : 0 );

			GetPlayer()->SendMessageToSet( data, true ,true );

			//sLog.outString("[emote] %s: %s", _player->GetName(), msg.c_str());
			delete data;
		}break;

	 case CHAT_MSG_SAY: // Concerne aussi les pnjs ;) (Brz)
		{
			if (sChatHandler.ParseCommands(msg.c_str(), this) > 0)
				break;

			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Votre message a été bloqué par un filtre du côté serveur.");
				return;
			}

			if(GetPlayer()->m_modlanguage >=0)
			{
				data = sChatHandler.FillMessageData( CHAT_MSG_SAY, GetPlayer()->m_modlanguage,  msg.c_str(), _player->GetGUID(), _player->bGMTagOn ? 4 : 0 );
				// Ally et H2 devrait pouvoir se parler via le /s, (Modif: Branruz) NB: a tester
				//data = sChatHandler.FillMessageData( CHAT_MSG_SAY, LANG_UNIVERSAL,  msg.c_str(), _player->GetGUID(), _player->bGMTagOn ? 4 : 0 );
				GetPlayer()->SendMessageToSet( data, true );
			}
			else
			{
				if(lang > 0 && LanguageSkills[lang] && _player->_HasSkillLine(LanguageSkills[lang]) == false)
					return;

				if(lang==0 && !sGMSecurityMgr.UseUniversalLang(permissions)) return; 

				data = sChatHandler.FillMessageData( CHAT_MSG_SAY, lang, msg.c_str(), _player->GetGUID(), _player->bGMTagOn ? 4 : 0 );
				SendChatPacket(data, 1, lang, this);
				for(set<Player*>::iterator itr = _player->m_inRangePlayers.begin(); itr != _player->m_inRangePlayers.end(); ++itr)
				{
					(*itr)->GetSession()->SendChatPacket(data, 1, lang, this);
				}
			}
			//sLog.outString("[say] %s: %s", _player->GetName(), msg.c_str());
			delete data;

						//--------------
			// Essai pour trouver s'il y a un PNJ capable de parler autour du player (Brz)
	        // Dans ce cas, on lui passe la phrase du chat et on arme un flag de discussion eventuel
	        // Le PNJ entend et parle, yeahhhhhhhhhhhh!!!!! (Filtre de la langue à faire)
            for(set<Object*>::iterator itr = GetPlayer()->m_objectsInRange.begin(); itr != GetPlayer()->m_objectsInRange.end(); ++itr)
	        {
		     if ((*itr)->IsUnit() && !(*itr)->IsPlayer()) // Seulement un PNJ
		     {
		      if( ((Unit *)(*itr))->IsSpeaker())
		      {
                ((Unit *)(*itr))->SearchBestResponse(GetPlayer(),msg);
		      }		 
			 }
			}          
			//---------------
		} break;

	 case CHAT_MSG_PARTY:
	 case CHAT_MSG_RAID:
	 case CHAT_MSG_RAID_LEADER:
	 case CHAT_MSG_RAID_WARNING:
		{
			if (sChatHandler.ParseCommands(msg.c_str(), this) > 0)
				break;
			
			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Votre message a été bloqué par un filtre de côté serveur.");
				return;
			}

			Group *pGroup = _player->GetGroup();
			if(pGroup == NULL) break;
			
			if(GetPlayer()->m_modlanguage >=0)
				data=sChatHandler.FillMessageData( type, GetPlayer()->m_modlanguage,  msg.c_str(), _player->GetGUID(), _player->bGMTagOn ? 4 : 0 );
			else
				data=sChatHandler.FillMessageData( type, (sGMSecurityMgr.UseUniversalLang(permissions) && lang != -1) ? LANG_UNIVERSAL : lang, msg.c_str(), _player->GetGUID(), _player->bGMTagOn ? 4 : 0);
			if(type == CHAT_MSG_PARTY && pGroup->GetGroupType() == GROUP_TYPE_RAID)
			{
				// only send to that subgroup
				SubGroup * sgr = _player->GetGroup() ?
					_player->GetGroup()->GetSubGroup(_player->GetSubGroup()) : 0;

				if(sgr)
				{
					_player->GetGroup()->Lock();
					for(GroupMembersSet::iterator itr = sgr->GetGroupMembersBegin(); itr != sgr->GetGroupMembersEnd(); ++itr)
					{
						if((*itr)->m_loggedInPlayer)
							(*itr)->m_loggedInPlayer->GetSession()->SendChatPacket(data, 1, lang, this);
					}
					_player->GetGroup()->Unlock();
				}
			}
			else
			{
				SubGroup * sgr;
				for(uint32 i = 0; i < _player->GetGroup()->GetSubGroupCount(); ++i)
				{
					sgr = _player->GetGroup()->GetSubGroup(i);
					_player->GetGroup()->Lock();
					for(GroupMembersSet::iterator itr = sgr->GetGroupMembersBegin(); itr != sgr->GetGroupMembersEnd(); ++itr)
					{
						if((*itr)->m_loggedInPlayer)
							(*itr)->m_loggedInPlayer->GetSession()->SendChatPacket(data, 1, lang, this);
					}
					_player->GetGroup()->Unlock();
				}
			}
			//sLog.outString("[party] %s: %s", _player->GetName(), msg.c_str());
			delete data;
			
		} break;

	 case CHAT_MSG_GUILD:
		{
			
			if (sChatHandler.ParseCommands(msg.c_str(), this) > 0)
			{
				break;
			}

			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Your chat message was blocked by a server-side filter.");
				return;
			}

			if(_player->m_playerInfo->guild)
				_player->m_playerInfo->guild->GuildChat(msg.c_str(), this, lang);
		} break;

	 case CHAT_MSG_OFFICER:
		{
			if (sChatHandler.ParseCommands(msg.c_str(), this) > 0)
				break;

			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Votre message a été bloqué par un filtre de côté serveur.");
				return;
			}

			if(_player->m_playerInfo->guild)
				_player->m_playerInfo->guild->OfficerChat(msg.c_str(), this, lang);
		} break;

	 case CHAT_MSG_YELL:  // Concerne aussi les pnjs ;) (Brz) (A Finir)
		{
     		if (sChatHandler.ParseCommands(msg.c_str(), this) > 0)
				break;

			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Votre message a été bloqué par un filtre de côté serveur.");
				return;
			}
			if(lang > 0 && LanguageSkills[lang] && _player->_HasSkillLine(LanguageSkills[lang]) == false)
				return;

			if(lang==0 && !sGMSecurityMgr.UseUniversalLang(permissions)) return; 

			if(GetPlayer()->m_modlanguage >=0)
				data = sChatHandler.FillMessageData( CHAT_MSG_YELL, GetPlayer()->m_modlanguage,  msg.c_str(), _player->GetGUID(), _player->bGMTagOn ? 4 : 0 );
			else
				data = sChatHandler.FillMessageData( CHAT_MSG_YELL, (sGMSecurityMgr.UseUniversalLang(permissions) && lang != -1) ? LANG_UNIVERSAL : lang,  msg.c_str(), _player->GetGUID(), _player->bGMTagOn ? 4 : 0 );

			_player->GetMapMgr()->SendChatMessageToCellPlayers(_player, data, 2, 1, lang, this);
			delete data;
			//sLog.outString("[yell] %s: %s", _player->GetName(), msg.c_str());
		} break;

	 case CHAT_MSG_WHISPER:  // Concerne aussi les pnjs ;) (Brz) (A Finir)
		{
    		if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Votre message a été bloqué par un filtre de côté serveur.");
				return;
			}
		 
			Player *player = objmgr.GetPlayer(misc.c_str(), false);
			if(!player)
			{
				data = new WorldPacket(SMSG_CHAT_PLAYER_NOT_FOUND, misc.length() + 1);
				*data << misc;
				SendPacket(data);
				delete data;
				break;
			}

			// Check that the player isn't a gm with his status on
			if(!_player->GetSession()->HasPermissions() && player->bGMTagOn && player->gmTargets.count(_player) == 0)
			{
				// Build automated reply
				string Reply = "Ce MJ n'a aucun ticket ouvert de votre part et n'a pas reçu votre requête. S'il vous plaît, soumettez une nouvelle demande de ticket si vous deviez parler à un MJ. Ceci est un message automatique.";
				data = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.c_str(), player->GetGUID(), 3);
				SendPacket(data);
				delete data;
				break;
			}

			if(lang > 0 && LanguageSkills[lang] && _player->_HasSkillLine(LanguageSkills[lang]) == false)
				return;

			if(lang==0 && !sGMSecurityMgr.UseUniversalLang(permissions)) return; 

			if( player->Social_IsIgnoring( _player->GetLowGUID() ) )
			{
				data = sChatHandler.FillMessageData( CHAT_MSG_IGNORED, LANG_UNIVERSAL,  msg.c_str(), _player->GetGUID(), _player->bGMTagOn ? 4 : 0 );
				SendPacket(data);
				delete data;
			}
			else
			{
				if(GetPlayer()->m_modlanguage >=0)
					
					data = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, GetPlayer()->m_modlanguage,  msg.c_str(), _player->GetGUID(), _player->bGMTagOn ? 4 : 0 );
				    // En commentaire: Ally et H2 devrait pouvoir se wisp, espion es-tu la!!! lol (Modif: Branruz)
                    //  data = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL,  msg.c_str(), _player->GetGUID(), _player->bGMTagOn ? 4 : 0 );
				else
					data = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, ((sGMSecurityMgr.UseUniversalLang(permissions) || sGMSecurityMgr.UseUniversalLang(player->GetSession()->GetPermissions())) && lang != -1) ? LANG_UNIVERSAL : lang,  msg.c_str(), _player->GetGUID(), _player->bGMTagOn ? 4 : 0 );

				player->GetSession()->SendPacket(data);
				delete data;
			}

			//Sent the to Users id as the channel, this should be fine as it's not used for wisper
		  
			data = sChatHandler.FillMessageData(CHAT_MSG_WHISPER_INFORM, LANG_UNIVERSAL,msg.c_str(), player->GetGUID(), player->bGMTagOn ? 4 : 0  );
			SendPacket(data);
			delete data;

			if(player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_AFK))
			{
				// Has AFK flag, autorespond.
				data = sChatHandler.FillMessageData(CHAT_MSG_AFK, LANG_UNIVERSAL,  player->m_afk_reason.c_str(),player->GetGUID(), _player->bGMTagOn ? 4 : 0);
				SendPacket(data);
				delete data;
			}
			else if(player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_DND))
			{
				// Has AFK flag, autorespond.
				data = sChatHandler.FillMessageData(CHAT_MSG_DND, LANG_UNIVERSAL, player->m_afk_reason.c_str(),player->GetGUID(), _player->bGMTagOn ? 4 : 0);
				SendPacket(data);
				delete data;
			}

			//sLog.outString("[whisper] %s to %s: %s", _player->GetName(), to.c_str(), msg.c_str());
		} break;

	 case CHAT_MSG_CHANNEL:
		{
 
			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Votre message a ete bloque par un filtre de cote serveur.");
				return;
			}

			if (sChatHandler.ParseCommands(msg.c_str(), this) > 0)
				break;

			Channel *chn = channelmgr.GetChannel(misc.c_str(),GetPlayer()); 
			if(chn) 
				chn->Say(GetPlayer(),msg.c_str(), NULL, false);
			//sLog.outString("[%s] %s: %s", channel.c_str(), _player->GetName(), msg.c_str());
		} break;

	 case CHAT_MSG_AFK:
		{
			GetPlayer()->SetAFKReason(msg);
            //Log.Notice("[CHAT_MSG_AFK]","Recu....");
			
			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Votre message a ete bloque par un filtre de cote serveur.");
				return;
			}
            
			/* WorldPacket *data, WorldSession* session, uint32 type, uint32 language, const char *channelName, const char *message*/
			if(GetPlayer()->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_AFK))
			{
				//Log.Notice("[Player AFK]","Remove AFK Flag for %s",GetPlayer()->GetName());

				GetPlayer()->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_AFK);

				if(sWorld.GetKickAFKPlayerTime())
					sEventMgr.RemoveEvents(GetPlayer(),EVENT_PLAYER_SOFT_DISCONNECT);
			}
			else
			{
				//Log.Notice("[Player AFK]","AFK Flag set for %s",GetPlayer()->GetName());

				GetPlayer()->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_AFK);
				if(sWorld.GetKickAFKPlayerTime())
					sEventMgr.AddEvent(GetPlayer(),&Player::SoftDisconnect,EVENT_PLAYER_SOFT_DISCONNECT,sWorld.GetKickAFKPlayerTime(),1,0);

				if( GetPlayer()->m_bg ) // HearthStone
				{
					GetPlayer()->m_bg->RemovePlayer( GetPlayer(), false );
					//GetPlayer()->BroadcastMessage("You have been kicked from %s for inactivity.", GetPlayer()->m_bg->GetName());
				}
			}			
		} break;

	 case CHAT_MSG_DND:
		{
			GetPlayer()->SetAFKReason(msg);

			if(g_chatFilter->Parse(msg) == true)
			{
				SystemMessage("Votre message a été bloqué par un filtre de côté serveur.");
				return;
			}

			if(GetPlayer()->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_DND))
			{
				GetPlayer()->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_DND);

				Log.Notice("[Player AFK]","Remove DND Flag for %s",GetPlayer()->GetName());
			}
			else
			{
				GetPlayer()->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_DND);

				Log.Notice("[Player AFK]","DND Flag set for %s",GetPlayer()->GetName());
			}		  
		} break;

	default:
		sLog.outError("CHAT: message de type inconnue %u, langue: %u", type, lang);
	}
}

void WorldSession::HandleTextEmoteOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 16);
	
	if(!_player->IsInWorld() || !_player->isAlive()) 
		return;

	uint64 guid;
	uint32
		text_emote,
		unk,
		namelen =1;
	const char* name =" ";

	recv_data >> text_emote;
	recv_data >> unk;
	recv_data >> guid;

	Unit * pUnit = _player->GetMapMgr()->GetUnit(guid);
	if(pUnit)
	{
		if(pUnit->IsPlayer())
		{
			name = static_cast< Player* >( pUnit )->GetName();
			namelen = (uint32)strlen(name) + 1;
		}
		else if(pUnit->GetTypeId() == TYPEID_UNIT)
		{
			Creature * p = static_cast<Creature*>(pUnit);
			if(p->GetCreatureName())
			{
				name = p->GetCreatureName()->Name;
				namelen = (uint32)strlen(name) + 1;
			}
			else
			{
				name = 0;
				namelen = 0;
			}
		}
	}

	EmoteTextEntry *em = dbcEmoteEntry.LookupEntry(text_emote);
	if(em)
	{
		WorldPacket data(SMSG_EMOTE, 28 + namelen);

		// sHookInterface.OnEmote(_player, (EmoteType)em->textid);
		sHookInterface.OnEmote(_player, (EmoteType)em->textid, pUnit); // Marcelo's Patch for Sun ++ and WaadScripts
		if(pUnit)
			CALL_SCRIPT_EVENT(pUnit,OnEmote)(_player,(EmoteType)em->textid);

        switch(em->textid)
        {
            case EMOTE_STATE_SLEEP:
            case EMOTE_STATE_SIT:
            case EMOTE_STATE_KNEEL:
			case EMOTE_STATE_DANCE:
				{
					_player->SetUInt32Value(UNIT_NPC_EMOTESTATE, em->textid);
				}break;
		}

		data << (uint32)em->textid;
		data << (uint64)GetPlayer()->GetGUID();
		GetPlayer()->SendMessageToSet(&data, true); //If player receives his own emote, his animation stops.

		data.Initialize(SMSG_TEXT_EMOTE);
		data << (uint64)GetPlayer()->GetGUID();
		data << (uint32)text_emote;
		data << unk;
		data << (uint32)namelen;
		if( namelen > 1 )   data.append(name, namelen);
		else				data << (uint8)0x00;

		GetPlayer()->SendMessageToSet(&data, true);

		GetPlayer()->GetAchievementInterface()->HandleAchievementCriteriaDoEmote(em->Id, pUnit);
	}
}

void WorldSession::HandleReportSpamOpcode(WorldPacket & recvPacket)
{
	/*CHECK_PACKET_SIZE(recvPacket, 29);

    // the 0 in the out packet is unknown
    GetPlayer()->GetSession()->OutPacket(SMSG_REPORT_SPAM_RESPONSE, 1, 0 );

	/* This whole thing is guess-work */
	/*uint8 unk1;
	uint64 reportedGuid;
	uint32 unk2;
	uint32 messagetype;
	uint32 unk3;
	uint32 unk4;
	std::string message;
	recvPacket >> unk1 >> reportedGuid >> unk2 >> messagetype >> unk3 >> unk4 >> message;

	Player * rPlayer = objmgr.GetPlayer((uint32)reportedGuid);
	if(!rPlayer)
		return;*/

}
