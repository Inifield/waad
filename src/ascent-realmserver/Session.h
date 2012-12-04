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

#ifndef _R_SESSION_H
#define _R_SESSION_H

#define NUM_ACCOUNT_DATA_TYPES		8
// L'un est l'inverse de l'autre... PER_CHARACTER_CACHE_MASK = ~GLOBAL_CACHE_MASK 
// Pour les valeur, voir AccountDataType
#define GLOBAL_CACHE_MASK         (uint8)0x15 // 0001 0101
#define PER_CHARACTER_CACHE_MASK  (uint8)0xEA // 1110 1010

typedef void(Session::*SessionPacketHandler)(WorldPacket&);

struct OpcodeHandler
{
	uint16 status;
	void (Session::*handler)(WorldPacket& recvPacket);
};

enum SessionStatus
{
	STATUS_AUTHED = 0,
	STATUS_LOGGEDIN
};

enum AccountFlags
{
	ACCOUNT_FLAG_VIP		 = 0x1,
	ACCOUNT_FLAG_NO_AUTOJOIN = 0x2,
	//ACCOUNT_FLAG_XTEND_INFO  = 0x4,
	ACCOUNT_FLAG_XPACK_01	= 0x8,
	ACCOUNT_FLAG_XPACK_02	= 0x10,
};

extern OpcodeHandler Handlers[NUM_MSG_TYPES];

class SERVER_DECL Session
{
public:
	friend class WorldSocket;
	friend class CharacterStorage;
	Session(uint32 id);
	~Session();

protected:
	FastQueue<WorldPacket*, Mutex> m_readQueue;
	WorldSocket * m_socket;
	WServer * m_server;
	WServer * m_nextServer;
	uint32 m_sessionId;
	uint32 m_accountId;
	uint32 m_ClientBuild;
	RPlayerInfo * m_currentPlayer;
	uint32 m_latency;
	uint32 m_accountFlags;
	string m_GMPermissions;
	string m_accountName;
	uint32 m_build;
	uint32 language;
	static SessionPacketHandler Handlers[NUM_MSG_TYPES];
	bool m_loadedPlayerData;

public:
	bool deleted;
	static void InitHandlers();
	void Update();
	uint32 m_muted;
	uint32 m_lastPing;

	ASCENT_INLINE RPlayerInfo * GetPlayer() { return m_currentPlayer; }

	ASCENT_INLINE void ClearCurrentPlayer() { m_currentPlayer = 0; }
	ASCENT_INLINE void ClearServers() { m_nextServer = m_server = 0; }
	ASCENT_INLINE void SetNextServer() { m_server = m_nextServer; }
	ASCENT_INLINE void SetNextServer(WServer* s) { m_nextServer = s; };
	ASCENT_INLINE void SetServer(WServer * s) { m_server = s; }
	ASCENT_INLINE WServer * GetServer() { return m_server; }
	ASCENT_INLINE WorldSocket * GetSocket() { return m_socket; }
	ASCENT_INLINE uint32 GetAccountId() { return m_accountId; }
	ASCENT_INLINE uint32 GetSessionId() { return m_sessionId; }
	ASCENT_INLINE std::string GetAccountPermissions() { return m_GMPermissions; }
	ASCENT_INLINE std::string GetAccountName() { return m_accountName; }
	ASCENT_INLINE uint32 GetAccountFlags() { return m_accountFlags; }
	ASCENT_INLINE uint32 GetClientBuild() { return m_ClientBuild; }	
	ASCENT_INLINE void SetSocket(WorldSocket *sock)
	{
		m_socket = sock;
	}
	
	bool CanUseCommand(std::string cmdstr)
	{
		if(m_GMPermissions.find("a") != string::npos)
			return true;
		
		if(cmdstr.length() <= 1)
			return (m_GMPermissions.find(cmdstr) != string::npos);
		else
		{
			for(size_t i = 0; i < cmdstr.length(); i++)
			{
				if(m_GMPermissions.find(cmdstr[i]) == string::npos)
					return false;
			}
		}
		return true;
	}

	bool HasFlag(uint32 flag) { return (m_accountFlags & flag) != 0; }

	void SendPacket(WorldPacket * data)
	{
		if(m_socket && m_socket->IsConnected())
			m_socket->SendPacket(data);
	}

	ASCENT_INLINE void SendPacket(StackPacket * packet)
	{
		if(m_socket && m_socket->IsConnected())
			m_socket->SendPacket(packet);
	}

	void OutPacket(uint16 opcode, uint16 len, const void* data)
	{
		if(m_socket && m_socket->IsConnected())
			m_socket->OutPacket(opcode, len, data);
	}
	
	void Disconnect()
	{
		if(m_socket && m_socket->IsConnected())
			m_socket->Disconnect();
	}

	void HandlePlayerLogin(WorldPacket & pck);
	void HandleCharacterEnum(WorldPacket & pck);
	void HandleCharacterEnumProc();

	void HandleCharacterCreate(WorldPacket & pck);
	void HandleCharacterDelete(WorldPacket & pck);
	void HandleCharacterRename(WorldPacket & pck);
	
	void HandleItemQuerySingleOpcode(WorldPacket & pck);
	void HandleCreatureQueryOpcode(WorldPacket & pck);
	void HandleGameObjectQueryOpcode(WorldPacket & pck);
	void HandleItemPageQueryOpcode(WorldPacket & pck);
	void HandleNpcTextQueryOpcode(WorldPacket & pck);

	void HandleRealmSplitQuery(WorldPacket & pck);

	void HandleReadyForAccountDataTimes( WorldPacket & recv_data );

	void HandleNameQueryOpcode( WorldPacket & recv_data );
	void HandleQueryTimeOpcode( WorldPacket & recv_data );
	void HandlePageTextQueryOpcode( WorldPacket & recv_data );
	void HandleItemNameQueryOpcode( WorldPacket & recv_data );	

	void SendAccountDataTimes(uint32 mask);
	
	/// Channel Opcodes (ChannelHandler.cpp)
	void HandleChannelJoin(WorldPacket& recvPacket);
	void HandleChannelLeave(WorldPacket& recvPacket);
	void HandleChannelList(WorldPacket& recvPacket);
	void HandleChannelPassword(WorldPacket& recvPacket);
	void HandleChannelSetOwner(WorldPacket& recvPacket);
	void HandleChannelOwner(WorldPacket& recvPacket);
	void HandleChannelModerator(WorldPacket& recvPacket);
	void HandleChannelUnmoderator(WorldPacket& recvPacket);
	void HandleChannelMute(WorldPacket& recvPacket);
	void HandleChannelUnmute(WorldPacket& recvPacket);
	void HandleChannelInvite(WorldPacket& recvPacket);
	void HandleChannelKick(WorldPacket& recvPacket);
	void HandleChannelBan(WorldPacket& recvPacket);
	void HandleChannelUnban(WorldPacket& recvPacket);
	void HandleChannelAnnounce(WorldPacket& recvPacket);
	void HandleChannelModerate(WorldPacket& recvPacket);
	void HandleChannelNumMembersQuery(WorldPacket & recvPacket);
	void HandleChannelRosterQuery(WorldPacket & recvPacket);
	void HandleMessagechatOpcode( WorldPacket & recv_data );
};

#endif


