/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2011 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the Ascent Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the Ascent is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#include "RStdAfx.h"

static const uint32 LanguageSkills[NUM_LANGUAGES] = {
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

#ifndef _GAME
void RPlayerInfo::RBroadcastMessage(const char* Format, ...)
{
	if (GetSession() == NULL)
		return;

	va_list l;
	va_start(l, Format);
	char Message[1024];
	vsnprintf(Message, 1024, Format, l);
	va_end(l);

	WorldPacket * data = sChatHandler.FillSystemMessageData(Message);
	GetSession()->SendPacket(data);
	delete data;
}
#endif //_GAME

void Session::HandleMessagechatOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 9);
	WorldPacket *data;
	//CHECK_INWORLD_RETURN;

	uint32 type;
	int32 lang;

	recv_data >> type;
	// were only going to handle these types here, otherwise send it to the world server for processing
	if( type != CHAT_MSG_WHISPER && type != CHAT_MSG_CHANNEL )
	{
		GetServer()->SendWoWPacket(this, &recv_data);
	}
	recv_data >> lang;

	if( lang >= NUM_LANGUAGES )
		return;

	/*
	if(GetPlayer()->IsBanned())
		{
			SystemMessage("You cannot do that when banned.");
			return;
		}*/
	

	string msg, misc;

	// special misc
	if( type == CHAT_MSG_CHANNEL || type == CHAT_MSG_WHISPER )
	{
		recv_data >> misc;
		recv_data >> msg;
	}
	else
		recv_data >> msg;

	/*
	if(!sHookInterface.OnChat(TO_PLAYER(_player), type, lang, msg, misc))
			return;*/
	
#ifndef _GAME
	//arghhh STFU. I'm not giving you gold or items NOOB
	switch(type)
	{
	   case CHAT_MSG_WHISPER:
	   case CHAT_MSG_CHANNEL:
		  {
			RPlayerInfo* player = sClientMgr.GetRPlayer(misc.c_str());
			if(player)
			{
				if( m_muted && m_muted >= (uint32)UNIXTIME )
				{
					player->RBroadcastMessage("Vous etes muet a cause d'un moderateur.");
					player->RBroadcastMessage("Vous pourrez de nouveau parler dans %s secondes.", ConvertTimeStampToString(m_muted - (uint32)UNIXTIME).c_str());
					return;
				}
			}
		  }break;
	 }
#endif //_GAME

	// Idiots spamming giant pictures through the chat system
	if( msg.find("|TInterface") != string::npos || msg.find("\n") != string::npos)
	{
		sChatHandler.SystemMessage(this, "Votre message a été bloqué.");
		return;
	}

	if(  msg.find("|c") != string::npos && msg.find("|H") == string::npos )
	{
		return;
	}

	switch(type)
	{
	case CHAT_MSG_WHISPER:
		{
			RPlayerInfo* player = sClientMgr.GetRPlayer(misc.c_str());
			if(!player)
			{
				ASCENT_TOLOWER(misc);
				if( misc == "console" )
				{
					Log.Notice("Whisper","%s to %s: %s", GetPlayer()->Name, misc.c_str(), msg.c_str());
					return;
				}
				else
				{
					data = new WorldPacket(SMSG_CHAT_PLAYER_NOT_FOUND, misc.length() + 1);
					*data << misc;
					SendPacket(data);
					delete data;
				}
				break;
			}

			// Check that the player isn't a gm
			if(!GetPlayer()->GMPermissions.size() && player->GMPermissions.size())
			{
				// Build automated reply
				string Reply = "Ce MJ n'a aucun ticket ouvert de votre part et n'a pas reçu votre requête. S'il vous plaît, soumettez une nouvelle demande de ticket si vous deviez parler à un MJ. Ceci est un message automatique.";
				data = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.c_str(), uint64(player->Guid), 3);
				SendPacket(data);
				delete data;
				break;
			}

			if(lang > 0 && LanguageSkills[lang] /* && _player->_HasSkillLine(LanguageSkills[lang]) == false*/)
				return;

			if(lang==0 && !CanUseCommand("c"))
				return;
#ifndef _GAME
			if( player->Social_IsIgnoring( GetPlayer()->Guid ))
			{
				data = sChatHandler.FillMessageData( CHAT_MSG_IGNORED, LANG_UNIVERSAL,  msg.c_str(), uint64(GetPlayer()->Guid), GetPlayer()->GMPermissions.size() ? 4 : 0 );
				SendPacket(data);
				delete data;
			}
#endif //_GAME
			else
			
			{				
				data = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, ((CanUseCommand("c") || player->GetSession()->CanUseCommand("c")) && lang != -1) ? LANG_UNIVERSAL : lang,  msg.c_str(), uint64(GetPlayer()->Guid), GetPlayer()->GMPermissions.size() ? 4 : 0 );
				player->GetSession()->SendPacket(data);
				delete data;
			}

			//Sent the to Users id as the channel, this should be fine as it's not used for whisper

			data = sChatHandler.FillMessageData(CHAT_MSG_WHISPER_INFORM, LANG_UNIVERSAL,msg.c_str(), uint64(player->Guid), player->GMPermissions.size() ? 4 : 0  );
			SendPacket(data);
			delete data;

			/*if(player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_AFK))
			{
				// Has AFK flag, autorespond.
				data = sChatHandler.FillMessageData(CHAT_MSG_AFK, LANG_UNIVERSAL,  player->m_afk_reason.c_str(),uint64(player->Guid), GetPlayer->GMPermissions.size() ? 4 : 0);
				SendPacket(data);
				delete data;
			}
			else if(player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_DND))
			{
				// Has AFK flag, autorespond.
				data = sChatHandler.FillMessageData(CHAT_MSG_DND, LANG_UNIVERSAL, player->m_afk_reason.c_str(),uint64(player->Guid), GetPlayer->GMPermissions.size() ? 4 : 0);
				SendPacket(data);
				delete data;
			}*/	
			

		} break;
	case CHAT_MSG_CHANNEL:
		{
			if (sChatHandler.ParseCommands(msg.c_str(), this) > 0)
				break;

			Channel *chn = channelmgr.GetChannel(misc.c_str(),GetPlayer()); 
			if(chn != NULL)
				chn->Say(GetPlayer(),msg.c_str(), NULL, false);
		} break;

	default:
		sLog.outDebug("CHAT: unknown msg type %u, lang: %u", type, lang);
	}
}

/*
void Session::HandleReportSpamOpcode(WorldPacket & recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 29);

	// the 0 in the out packet is unknown
	GetPlayer()->GetSession()->OutPacket(SMSG_COMPLAIN_RESULT, 1, 0 );
}
*/
