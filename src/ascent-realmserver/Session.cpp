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
	Handlers[CMSG_PLAYER_LOGIN]					= &Session::HandlePlayerLogin;
	Handlers[CMSG_CHAR_ENUM]					= &Session::HandleCharacterEnum;
	Handlers[CMSG_CHAR_CREATE]					= &Session::HandleCharacterCreate;
	Handlers[CMSG_CHAR_DELETE]					= &Session::HandleCharacterDelete;
	Handlers[CMSG_CHAR_RENAME]					= &Session::HandleCharacterRename;
	Handlers[CMSG_UPDATE_ACCOUNT_DATA]			= &Session::HandleUpdateAccountData;
	Handlers[CMSG_REQUEST_ACCOUNT_DATA]			= &Session::HandleRequestAccountData;
	Handlers[CMSG_ITEM_QUERY_SINGLE]			= &Session::HandleItemQuerySingleOpcode;
	Handlers[CMSG_REALM_SPLIT]					= &Session::HandleRealmSplitQuery;
	Handlers[CMSG_CREATURE_QUERY]				= &Session::HandleCreatureQueryOpcode;
	Handlers[CMSG_GAMEOBJECT_QUERY]				= &Session::HandleGameObjectQueryOpcode;
	Handlers[CMSG_ITEM_NAME_QUERY]				= &Session::HandleItemNameQueryOpcode;
	Handlers[CMSG_PAGE_TEXT_QUERY]				= &Session::HandlePageTextQueryOpcode;
	Handlers[CMSG_QUERY_TIME]					= &Session::HandleQueryTimeOpcode;
	Handlers[CMSG_NAME_QUERY]					= &Session::HandleNameQueryOpcode;
	Handlers[CMSG_READY_FOR_ACCOUNT_DATA_TIMES]	= &Session::HandleReadyForAccountDataTimes;
	
	// Channels
	Handlers[CMSG_JOIN_CHANNEL]					= &Session::HandleChannelJoin;
	Handlers[CMSG_LEAVE_CHANNEL]				= &Session::HandleChannelLeave;
	Handlers[CMSG_CHANNEL_LIST]					= &Session::HandleChannelList;
	Handlers[CMSG_CHANNEL_PASSWORD]				= &Session::HandleChannelPassword;
	Handlers[CMSG_CHANNEL_SET_OWNER]			= &Session::HandleChannelSetOwner;
	Handlers[CMSG_CHANNEL_OWNER]				= &Session::HandleChannelOwner;
	Handlers[CMSG_CHANNEL_MODERATOR]			= &Session::HandleChannelModerator;
	Handlers[CMSG_CHANNEL_UNMODERATOR]			= &Session::HandleChannelUnmoderator;
	Handlers[CMSG_CHANNEL_MUTE]					= &Session::HandleChannelMute;
	Handlers[CMSG_CHANNEL_UNMUTE]				= &Session::HandleChannelUnmute;
	Handlers[CMSG_CHANNEL_INVITE]				= &Session::HandleChannelInvite;
	Handlers[CMSG_CHANNEL_KICK]					= &Session::HandleChannelKick;
	Handlers[CMSG_CHANNEL_BAN]					= &Session::HandleChannelBan;
	Handlers[CMSG_CHANNEL_UNBAN]				= &Session::HandleChannelUnban;
	Handlers[CMSG_CHANNEL_ANNOUNCEMENTS]		= &Session::HandleChannelAnnounce;
	Handlers[CMSG_CHANNEL_MODERATE]				= &Session::HandleChannelModerate;
	Handlers[CMSG_GET_CHANNEL_MEMBER_COUNT]		= &Session::HandleChannelNumMembersQuery;
	Handlers[CMSG_CHANNEL_DISPLAY_LIST]			= &Session::HandleChannelRosterQuery;
	Handlers[CMSG_MESSAGECHAT]					= &Session::HandleMessagechatOpcode;
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

	for(uint32 x = 0; x < 8; x++)
	{
		sAccountData[x].sz = NULL;
		sAccountData[x].data = NULL;
	}
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
	bool errorpacket = false; //Par défaut, un paquet est toujours valide

	if(!((++_updatecount) % 2) && m_socket)
		m_socket->UpdateQueuedPackets();

	WorldPacket * pck;
	OpcodeHandler * Handler; 	
	
	uint16 opcode;
	while((pck = m_readQueue.Pop()))
	{
		if(!pck)
		{
			Log.Error("Session::Update","Fatal: Packet Error (NULL)");
			errorpacket = true;
		}
		else
			opcode = pck->GetOpcode();

		/* On regarde si le RealmServer peut gérer lui-même l'Opcode reçu. */
		if(Handlers[opcode] != 0)
		{
			sLog.outDebug("[Session] : Traitement de l'Opcode par le Realm Serveur %s (0x%04X)",
				LookupName(pck->GetOpcode(), g_worldOpcodeNames), opcode);
			(this->*Session::Handlers[opcode])(*pck);
		}
		else
		{
			sLog.outDebug("[Session] : Traitement envoye au World Server");

			/* On ne supprime pas les paquets envoyés pour traitement au World Serveur */
			if(m_server) m_server->SendWoWPacket(this, pck);
			else
			{
				/* Sauf si le paquet est en erreur ou ne peut-être géré ( ex : World déconnecté )*/
				sLog.outError("[Session] : Réception d'un paquet non géré avec l'opcode: %s (0x%04X)",
					LookupName(pck->GetOpcode(), g_worldOpcodeNames), pck->GetOpcode());
				errorpacket = true;
			}
		}

		if(pck->GetOpcode() >= NUM_MSG_TYPES)
		{
			sLog.outError("[Session] : Received out of range packet with opcode %s (0x%04X)", 
				LookupName(pck->GetOpcode(), g_worldOpcodeNames), pck->GetOpcode());
			errorpacket = true;
		}
		else
		{
			// < Paquets valides >
			Handler = (OpcodeHandler *)(&Handlers[pck->GetOpcode()]);			
			if(Handler->handler == 0)
			{
				/* Les paquets traités par le World serveur n'ont plus lieu d'être. On les supprimes */
				sLog.outDebug("[Session] : Réception d'un paquet pour gestion avec l'opcode: %s (0x%04X)",
					LookupName(pck->GetOpcode(), g_worldOpcodeNames), pck->GetOpcode());
				errorpacket = true;
			}
			else
			{
				/* Si un paquet n'a pas été traité, on le renvoie dans la boucle */
				(this->*Handler->handler)(*pck);
			}
		}

		if(errorpacket == true) // Si le paquet est en "erreur", on le supprime pour éviter un crash du core
			delete pck;
	}
}