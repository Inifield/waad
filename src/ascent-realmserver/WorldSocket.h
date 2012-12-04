/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
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

#ifndef REALMSERVER_WORLDSOCKET_H
#define REALMSERVER_WORLDSOCKET_H

#define WORLDSOCKET_SENDBUF_SIZE 131078
#define WORLDSOCKET_RECVBUF_SIZE 16384

class WorldPacket;
class SocketHandler;
class Session;

enum OUTPACKET_RESULT
{
	OUTPACKET_RESULT_SUCCESS = 1,
	OUTPACKET_RESULT_NO_ROOM_IN_BUFFER = 2,
	OUTPACKET_RESULT_NOT_CONNECTED = 3,
	OUTPACKET_RESULT_SOCKET_ERROR = 4,
};

class WorldSocket : public Socket
{
public:
	bool Authed;
	WorldSocket(SOCKET fd);
	~WorldSocket();

	// vs8 fix - send null on empty buffer
	ASCENT_INLINE void SendPacket(WorldPacket* packet) { if(!packet) return; OutPacket(packet->GetOpcode(), packet->size(), (packet->size() ? (const void*)packet->contents() : NULL)); }
	ASCENT_INLINE void SendPacket(StackPacket * packet) { if(!packet) return; OutPacket(packet->GetOpcode(), packet->GetSize(), (packet->GetSize() ? (const void*)packet->GetBufferPointer() : NULL)); }

	void __fastcall OutPacket(uint16 opcode, size_t len, const void* data);
	OUTPACKET_RESULT __fastcall _OutPacket(uint16 opcode, size_t len, const void* data);
   
	ASCENT_INLINE uint32 GetLatency() { return _latency; }

	void Authenticate();
	void InformationRetreiveCallback(WorldPacket & recvData, uint32 requestid);

	void OnRead();
	void OnConnect();
	void OnDisconnect();

	void SendAddonInfoPacket(WorldPacket *source, uint32 pos, Session *m_session);
	void UpdateQueuedPackets();
	
protected:
	
	void _HandleAuthSession(WorldPacket* recvPacket);
	void _HandlePing(WorldPacket* recvPacket);

private:

	uint8 K[40];
	uint32 mOpcode;
	uint32 mRemaining;
	uint32 mSize;
	uint32 mSeed;
	uint32 mClientSeed;
	uint32 mClientBuild;
	uint32 mRequestID;

	WorldPacket * pAuthenticationPacket;
	string * m_fullAccountName;
	WowCrypt _crypt;
	uint32 _latency;

	Session * m_session;
	
	Mutex queueLock;
};

#endif
