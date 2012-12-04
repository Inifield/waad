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

SessionPacketHandler Session::Handlers[NUM_MSG_TYPES];
void Session::InitHandlers()
{
	memset(Handlers, 0, sizeof(void*) * NUM_MSG_TYPES);
	Handlers[CMSG_PLAYER_LOGIN] = &Session::HandlePlayerLogin;
	Handlers[CMSG_CHAR_ENUM] = &Session::HandleCharacterEnum;
	Handlers[CMSG_CHAR_CREATE] = &Session::HandleCharacterCreate;
	Handlers[CMSG_CHAR_DELETE] = &Session::HandleCharacterDelete;
	//Handlers[CMSG_CHAR_RENAME] = &Session::HandleCharacterRename;
	Handlers[CMSG_ITEM_QUERY_SINGLE] = &Session::HandleItemQuerySingleOpcode;
	Handlers[CMSG_REALM_SPLIT] = &Session::HandleRealmSplitQuery;
	Handlers[CMSG_CREATURE_QUERY] = &Session::HandleCreatureQueryOpcode;
	Handlers[CMSG_GAMEOBJECT_QUERY] = &Session::HandleGameObjectQueryOpcode;
	Handlers[CMSG_ITEM_NAME_QUERY] = &Session::HandleItemNameQueryOpcode;
	Handlers[CMSG_PAGE_TEXT_QUERY] = &Session::HandlePageTextQueryOpcode;
	Handlers[CMSG_QUERY_TIME] = &Session::HandleQueryTimeOpcode;
	Handlers[CMSG_NAME_QUERY] = &Session::HandleNameQueryOpcode;
	Handlers[CMSG_READY_FOR_ACCOUNT_DATA_TIMES]	= &Session::HandleReadyForAccountDataTimes;
	
	// Channels
	Handlers[CMSG_JOIN_CHANNEL]	= &Session::HandleChannelJoin;
	Handlers[CMSG_LEAVE_CHANNEL] = &Session::HandleChannelLeave;
	Handlers[CMSG_CHANNEL_LIST]	= &Session::HandleChannelList;
	Handlers[CMSG_CHANNEL_PASSWORD]	= &Session::HandleChannelPassword;
	Handlers[CMSG_CHANNEL_SET_OWNER] = &Session::HandleChannelSetOwner;
	Handlers[CMSG_CHANNEL_OWNER] = &Session::HandleChannelOwner;
	Handlers[CMSG_CHANNEL_MODERATOR] = &Session::HandleChannelModerator;
	Handlers[CMSG_CHANNEL_UNMODERATOR] = &Session::HandleChannelUnmoderator;
	Handlers[CMSG_CHANNEL_MUTE]	= &Session::HandleChannelMute;
	Handlers[CMSG_CHANNEL_UNMUTE] = &Session::HandleChannelUnmute;
	Handlers[CMSG_CHANNEL_INVITE] = &Session::HandleChannelInvite;
	Handlers[CMSG_CHANNEL_KICK]	= &Session::HandleChannelKick;
	Handlers[CMSG_CHANNEL_BAN] = &Session::HandleChannelBan;
	Handlers[CMSG_CHANNEL_UNBAN] = &Session::HandleChannelUnban;
	Handlers[CMSG_CHANNEL_ANNOUNCEMENTS] = &Session::HandleChannelAnnounce;
	Handlers[CMSG_CHANNEL_MODERATE]	= &Session::HandleChannelModerate;
	Handlers[CMSG_GET_CHANNEL_MEMBER_COUNT]	= &Session::HandleChannelNumMembersQuery;
	Handlers[CMSG_CHANNEL_DISPLAY_LIST]	= &Session::HandleChannelRosterQuery;
	Handlers[CMSG_MESSAGECHAT] = &Session::HandleMessagechatOpcode;
}

Session::Session(uint32 id) : m_sessionId(id)
{
	deleted = false;
	m_socket = 0;
	m_server = 0;
	m_accountId = 0;
	m_currentPlayer = 0;
	m_latency = 0;
	m_accountFlags = 0;
	m_build = 0;
	m_nextServer = 0;
}

Session::~Session()
{
}

void Session::HandleReadyForAccountDataTimes(WorldPacket& /*recv_data*/)
{
    // empty opcode
    Log.Debug("Session", "CMSG_READY_FOR_ACCOUNT_DATA_TIMES");

    SendAccountDataTimes(GLOBAL_CACHE_MASK);
}

void Session::SendAccountDataTimes(uint32 mask)
{
    WorldPacket data( SMSG_ACCOUNT_DATA_TIMES, 4+1+4+8*4 ); // changed in WotLK
    data << uint32( UNIXTIME );                             // unix time of something
    data << uint8(1);
    data << uint32(mask);                                   // type mask
    for(uint32 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
		if(mask & (1 << i)){
            //data << uint32(GetAccountData(AccountDataType(i))->Time);// also unix time
			data << uint32( 0 );
		}
    SendPacket(&data);
}

void Session::Update()
{
	WorldPacket * pck;
	OpcodeHandler * Handler = NULL;

	uint16 opcode;
	while((pck = m_readQueue.Pop()))
	{
		if(!pck)
		{
			Log.Error("Session::Update","Fatal: Packet Error (NULL)");
		}
		else
			opcode = pck->GetOpcode();

		/* can we handle it ourselves? */
		if(Session::Handlers[opcode] != 0)
		{
			sLog.outDebug("[Session] : Traitement de l'Opcode 0x%04X", opcode);
			(this->*Session::Handlers[opcode])(*pck);
		}
		else
		{
			sLog.outDebug("[Session] : Traitement envoye au World Server");

			/* no? pass it back to the worker server for handling. */
			if(m_server) m_server->SendWoWPacket(this, pck);
		}

		if(pck->GetOpcode() >= NUM_MSG_TYPES)
			sLog.outError("[Session] : Received out of range packet with opcode 0x%04X", pck->GetOpcode());
		else
		{			
			// Valid Packet :>
			Handler = (OpcodeHandler *)(&Handlers[pck->GetOpcode()]);
			if(Handler->handler == 0)
			{
				sLog.outDebug("[Session] : Reception d'un paquet pour gestion avec l'opcode: 0x%04X",
					pck->GetOpcode());
			}
			else
			{
				(this->*Handler->handler)(*pck);
			}
		}
	}
	delete pck;
}
