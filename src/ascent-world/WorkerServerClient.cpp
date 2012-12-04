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
#ifdef CLUSTERING

WSClient::WSClient(SOCKET fd) : Socket(fd, 1024576, 1024576)
{
	_remaining = 0;
	_cmd = 0;
}

WSClient::~WSClient()
{

}

void WSClient::OnRead()
{
	for(;;)
	{
		if(!_cmd)
		{
			if(GetReadBuffer().GetSize() < 6)
				break;

			GetReadBuffer().Read(&_cmd, 2);
			GetReadBuffer().Read(&_remaining, 4);
		}

		if(_remaining && GetReadBuffer().GetSize() < _remaining)
			break;

		if(_cmd == ISMSG_WOW_PACKET)
		{
			/* optimized version for packet passing, to reduce latency! ;) */
			uint32 sid = 0;
			uint16 op = 0;
			uint32 sz = 0;
			GetReadBuffer().Read(&sid, 4);
			GetReadBuffer().Read(&op, 2);
			GetReadBuffer().Read(&sz, 4);
			
			WorldSession * session = sClusterInterface.GetSession(sid);
			WorldPacket * pck;
			if(session != NULL)
			{
				pck = new WorldPacket(op, sz);
				if (sz > 0)
				{
					pck->resize(sz);
					GetReadBuffer().Read((void*)pck->contents(), sz);
				}
				session->QueuePacket(pck);
			}
			else 
			{
				delete pck;
				pck = NULL;
			}
			_cmd = 0;
			continue;
		}

		WorldPacket * pck = new WorldPacket(_cmd, _remaining);
		_cmd = 0;
		pck->resize(_remaining);
		GetReadBuffer().Read((void*)pck->contents(), _remaining);

		/* we could handle auth here */
		switch(_cmd)
		{
		case ISMSG_AUTH_REQUEST:
			sClusterInterface.HandleAuthRequest(*pck);
			delete pck;
			break;
		/*case ISMSG_AUTH_RESULT:
			sClusterInterface.HandleAuthResult(*pck);
			delete pck;
			break;
		case ISMSG_REGISTER_RESULT:
			sClusterInterface.HandleRegisterResult(*pck);
			delete pck;
			break;
		case ISMSG_CREATE_INSTANCE:
			sClusterInterface.HandleCreateInstance(*pck);
			delete pck;
			break;
		case ISMSG_PLAYER_LOGIN:
			sClusterInterface.HandlePlayerLogin(*pck);
			delete pck;
			break;
		case ISMSG_WOW_PACKET:
			sClusterInterface.HandleWoWPacket(*pck);
			delete pck;
			break;
		case ISMSG_TELEPORT_RESULT:
			sClusterInterface.HandleTeleportResult(*pck);
			delete pck;
			break;
		case ISMSG_SESSION_REMOVED:
			sClusterInterface.HandleSessionRemoved(*pck);
			delete pck;
			break;
		case ISMSG_SAVE_ALL_PLAYERS:
			sClusterInterface.HandleSaveAllPlayers(*pck);
			delete pck;
			break;
		case ISMSG_TRANSPORTER_MAP_CHANGE:
			sClusterInterface.HandleTransporterMapChange(*pck);
			delete pck;
			break;
		case ISMSG_PLAYER_TELEPORT:
			sClusterInterface.HandlePlayerTeleport(*pck);
			delete pck;
			break;
		case ISMSG_CREATE_PLAYER:
			sClusterInterface.HandleCreatePlayer(*pck);
			delete pck;
			break;
		case ISMSG_PACKED_PLAYER_INFO:
			sClusterInterface.HandlePackedPlayerInfo(*pck);
			delete pck;
			break;
		case ISMSG_DESTROY_PLAYER_INFO:
			sClusterInterface.HandleDestroyPlayerInfo(*pck);
			delete pck;
			break;
		case ISMSG_PLAYER_INFO:
			sClusterInterface.HandlePlayerInfo(*pck);
			delete pck;
			break;
		case ISMSG_CHANNEL_ACTION:
			sClusterInterface.HandleChannelAction(*pck);
			delete pck;
			break;
		case ISMSG_CHANNEL_LFG_DUNGEON_STATUS_REQUEST:
			sClusterInterface.HandleChannelLFGDungeonStatusRequest(*pck);
			delete pck;
			break;*/
		default:
			sClusterInterface.QueuePacket(pck);
		}		
	}
}

void WSClient::OnConnect()
{
	sClusterInterface.SetSocket(this);
}

void WSClient::SendPacket(WorldPacket * data)
{
	bool rv;
	uint32 size = data->size();
	uint16 opcode = data->GetOpcode();
	if(!IsConnected())
		return;

	BurstBegin();

	// Pass the header to our send buffer
	rv = BurstSend((const uint8*)&opcode, 2);
	rv = BurstSend((const uint8*)&size, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(size > 0 && rv)
		rv = BurstSend((const uint8*)data->contents(), size);

	if(rv) BurstPush();
	BurstEnd();
}

void WSClient::OnDisconnect()
{
	sClusterInterface.ConnectionDropped();
	sSocketGarbageCollector.QueueSocket(this);
}

#endif
