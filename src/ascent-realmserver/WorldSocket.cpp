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
// Class WorldSocket - Main network code functions, handles
// reading/writing of all packets.

#include "RStdAfx.h"
#include "../ascent-shared/AuthCodes.h"

#pragma pack(push, 1)
struct ClientPktHeader
{
	uint16 size;
	uint32 cmd;
};

struct ServerPktHeader
{
	uint16 size;
	uint16 cmd;
};
#pragma pack(pop)

WorldSocket::WorldSocket(SOCKET fd) : Socket(fd, WORLDSOCKET_SENDBUF_SIZE, WORLDSOCKET_RECVBUF_SIZE)
{
	mSize = mOpcode = mRemaining = 0;
	_latency = 0;
	m_session = NULL;
	mSeed = rand() % 0xFFFFFFF0 + 10;
	pAuthenticationPacket = NULL;
	mRequestID = 0;
	Authed = false;
	m_fullAccountName = NULL;
}

WorldSocket::~WorldSocket()
{

}

void WorldSocket::OnDisconnect()
{
	if(mRequestID != 0)
	{
		sLogonCommHandler.UnauthedSocketClose(mRequestID);
		mRequestID = 0;
	}

	if( m_session )
		sClientMgr.DestroySession(m_session->m_sessionId);
}

void WorldSocket::OutPacket(uint16 opcode, size_t len, const void* data)
{
	OUTPACKET_RESULT res;
	if( (len + 10) > WORLDSOCKET_SENDBUF_SIZE )
	{
		printf("WARNING: Tried to send a packet of %u bytes (which is too large) to a socket. Opcode was: %u (0x%03X)\n", (unsigned int)len, (unsigned int)opcode, (unsigned int)opcode);
		return;
	}

	res = _OutPacket(opcode, len, data);
	if(res == OUTPACKET_RESULT_SUCCESS)
		return;

	if(res == OUTPACKET_RESULT_NO_ROOM_IN_BUFFER)
	{
		/* queue the packet */
		queueLock.Acquire();
		WorldPacket * pck = new WorldPacket(opcode, len);
		if(len) pck->append((const uint8*)data, len);
		m_session->m_readQueue.Push(pck);
		queueLock.Release();
	}
}

void WorldSocket::UpdateQueuedPackets()
{
	queueLock.Acquire();
	if(!m_session->m_readQueue.HasItems())
	{
		queueLock.Release();
		return;
	}

	WorldPacket * pck;
	while((pck = m_session->m_readQueue.front()))
	{
		/* try to push out as many as you can */
		switch(_OutPacket(pck->GetOpcode(), pck->size(), pck->size() ? pck->contents() : NULL))
		{
		case OUTPACKET_RESULT_SUCCESS:
			{
				delete pck;
				pck = NULL;
				m_session->m_readQueue.pop_front();
			}break;

		case OUTPACKET_RESULT_NO_ROOM_IN_BUFFER:
			{
				/* still connected */
				queueLock.Release();
				return;
			}break;

		default:
			{
				/* kill everything in the buffer */
				while((pck == m_session->m_readQueue.Pop()))
				{
					delete pck;
					pck = NULL;
				}
				queueLock.Release();
				return;
			}break;
		}
	}
	queueLock.Release();
}

OUTPACKET_RESULT WorldSocket::_OutPacket(uint16 opcode, size_t len, const void* data)
{
	bool rv;
	if(!IsConnected())
		return OUTPACKET_RESULT_NOT_CONNECTED;

	BurstBegin();
	//if((m_writeByteCount + len + 4) >= m_writeBufferSize)
	if( GetWriteBuffer().GetSpace() < (len+4) )
	{
		BurstEnd();
		return OUTPACKET_RESULT_NO_ROOM_IN_BUFFER;
	}

	// Encrypt the packet
	// First, create the header.
	ServerPktHeader Header;
	Header.cmd = opcode;
	Header.size = ntohs((uint16)len + 2);
    _crypt.EncryptSend((uint8*)&Header, sizeof (ServerPktHeader));

	// Pass the header to our send buffer
	rv = BurstSend((const uint8*)&Header, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(len > 0 && rv)
	{
		rv = BurstSend((const uint8*)data, (uint32)len);
	}

	if(rv) BurstPush();
	BurstEnd();
	return rv ? OUTPACKET_RESULT_SUCCESS : OUTPACKET_RESULT_SOCKET_ERROR;
}

/*void WorldSocket::OutPacket(uint16 opcode, size_t len, const void* data)
{
	bool rv;
	if(opcode == 0 || !IsConnected())
		return;

	BurstBegin();

	// Encrypt the packet
	// First, create the header.
	ServerPktHeader Header;
	Header.cmd = opcode;
	Header.size = ntohs((uint16)len + 2);
    _crypt.EncryptSend((uint8*)&Header, sizeof (ServerPktHeader));

	// Pass the header to our send buffer
	rv = BurstSend((const uint8*)&Header, 4);

	// Pass the rest of the packet to our send buffer (if there is any)
	if(len > 0 && rv)
	{
		rv = BurstSend((const uint8*)data, (uint32)len);
	}

	if(rv) BurstPush();
	BurstEnd();
}*/

void WorldSocket::OnConnect()
{
	_latency = getMSTime();

	//OutPacket(SMSG_AUTH_CHALLENGE, 4, &mSeed); // Obsolete 322

	// 322 - OpenAscent
	WorldPacket data (SMSG_AUTH_CHALLENGE, 25);
	data << uint32(1);			
	data << mSeed;
	data << uint32(0xF3539DA3);	
	data << uint32(0x6E8547B9);	
	data << uint32(0x9A6AA2F8);	
	data << uint32(0xA4F170F4);
	SendPacket(&data);
	
}

void WorldSocket::_HandleAuthSession(WorldPacket* recvPacket)
{
	std::string account;
	uint32 unk1, unk2;
	uint64 unk3;
	uint32 unk4, unk5, unk6;

	_latency = getMSTime() - _latency;

	try
	{
		*recvPacket >> mClientBuild;
		*recvPacket >> unk1;
		*recvPacket >> account;
		*recvPacket >> unk2;
		*recvPacket >> mClientSeed;
		*recvPacket >> unk3;
		*recvPacket >> unk4;
		*recvPacket >> unk5;
		*recvPacket >> unk6;
	}
	catch(ByteBuffer::error &)
	{
		sLog.outError("Incomplete copy of AUTH_SESSION Received.");
		return;
	}

	// Send out a request for this account.
	mRequestID = sLogonCommHandler.ClientConnected(account, this);
	
	if(mRequestID == 0xFFFFFFFF)
	{
		Disconnect();
		return;
	}

	// shitty hash !
	m_fullAccountName = new string( account );

	// Set the authentication packet 
	pAuthenticationPacket = recvPacket;
}

void WorldSocket::InformationRetreiveCallback(WorldPacket & recvData, uint32 requestid)
{
	if(requestid != mRequestID)
		return;

	uint32 error;
	recvData >> error;

	if(error != 0 || pAuthenticationPacket == NULL)
	{
		// something happened wrong @ the logon server
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x0D");
		return;
	}

	// Extract account information from the packet.
	string AccountName;
	const string * ForcedPermissions;
	uint32 AccountID;
	string GMFlags;
	uint8 AccountFlags;

	recvData >> AccountID >> AccountName >> GMFlags >> AccountFlags;
	ForcedPermissions = sLogonCommHandler.GetForcedPermissions(AccountName);
	if( ForcedPermissions != NULL )
	{
		GMFlags.assign(ForcedPermissions->c_str());
		Log.Debug("[GM FORCED PERMISSIONS]", "Il y a une permission forcee pour le compte No. %u (%s)", AccountID, AccountName.c_str() );

	}
	
	printf( " >> Recuperation des informations pour le logon: `%s` ID %u (request %u)", AccountName.c_str(), AccountID, mRequestID);
	printf("\n");
	//	sLog.outColor(TNORMAL, "\n");

	mRequestID = 0;
	// Pull the session key and encrypted key
	//uint8 K[40];
	recvData.read(K, 40);

	_crypt.Init(K);

	BigNumber BNK;
	BNK.SetBinary(K, 40);

	//checking if player is already connected
	//disconnect current player and login this one(blizzlike)

	string lang = "frFR";
	if(recvData.rpos() != recvData.wpos())
		recvData.read((uint8*)lang.data(), 4);

	Session * session = sClientMgr.CreateSession(AccountID);
	if(!session)
	{
		/* we are already logged in. send auth failed. (if anyone has a better error lemme know :P) */
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x0D");
		printf("Duplicate client error.\n");
		return;
	}

	m_session = session;
	session->m_socket = this;
    Sha1Hash sha;

	uint8 digest[20];
	pAuthenticationPacket->read(digest, 20);

	uint32 t = 0;
	if( m_fullAccountName == NULL )				// should never happen !
		sha.UpdateData(AccountName);
	else
	{
		sha.UpdateData(*m_fullAccountName);

		// this is unused now. we may as well free up the memory.
		delete m_fullAccountName;
		m_fullAccountName = NULL;
	}

	sha.UpdateData((uint8 *)&t, 4);
	sha.UpdateData((uint8 *)&mClientSeed, 4);
	sha.UpdateData((uint8 *)&mSeed, 4);
	sha.UpdateBigNumbers(&BNK, NULL);
	sha.Finalize();

	if (memcmp(sha.GetDigest(), digest, 20))
	{
		// AUTH_UNKNOWN_ACCOUNT = 21
		OutPacket(SMSG_AUTH_RESPONSE, 1, "\x15");
		return;
	}

	// Allocate session
	m_session->m_accountFlags = AccountFlags;
	m_session->m_GMPermissions = GMFlags;
	m_session->m_accountId = AccountID;
	m_session->m_latency = _latency;
	m_session->m_accountName = AccountName;
	m_session->m_ClientBuild = mClientBuild;
	//m_session->language = sLocalizationMgr.GetLanguageId(lang);

	Log.Notice("Auth", "%s from %s:%u [%ums]", AccountName.c_str(), GetRemoteIP().c_str(), GetRemotePort(), _latency);
	Authenticate();
}

void WorldSocket::Authenticate()
{
	if(m_session->m_accountFlags & 16)
		OutPacket(SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x02");
	else if(m_session->m_accountFlags & 8)
		OutPacket(SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x01");
	else
		OutPacket(SMSG_AUTH_RESPONSE, 11, "\x0C\x30\x78\x00\x00\x00\x00\x00\x00\x00\x00");

	SendAddonInfoPacket(pAuthenticationPacket, (uint32)pAuthenticationPacket->rpos(), m_session);

	delete pAuthenticationPacket;
	pAuthenticationPacket = 0;
}

void WorldSocket::_HandlePing(WorldPacket* recvPacket)
{
	uint32 ping;
	if(recvPacket->size() < 4)
	{
		sLog.outString("Socket closed due to incomplete ping packet.");
		Disconnect();
		return;
	}

	*recvPacket >> ping;
	*recvPacket >> _latency;
	OutPacket(SMSG_PONG, 4, &ping);
}

void WorldSocket::OnRead()
{
	for(;;)
	{
		// Check for the header if we don't have any bytes to wait for.
		if(mRemaining == 0)
		{
			if(GetReadBuffer().GetSize() < 6)
			{
				// No header in the packet, let's wait.
				return;
			}

			// Copy from packet buffer into header local var
			ClientPktHeader Header;
			GetReadBuffer().Read((uint8*)&Header, 6);

			// Decrypt the header
			_crypt.DecryptRecv((uint8*)&Header, sizeof (ClientPktHeader));
			mRemaining = mSize = ntohs(Header.size) - 4;
			mOpcode = Header.cmd;
		}

		WorldPacket * Packet;

		if(mRemaining > 0)
		{
			if( GetReadBuffer().GetSize() < mRemaining )
			{
				// We have a fragmented packet. Wait for the complete one before proceeding.
				return;
			}
		}

		Packet = new WorldPacket(mOpcode, mSize);
		Packet->resize(mSize);

		if(mRemaining > 0)
		{
			// Copy from packet buffer into our actual buffer.
			///Read(mRemaining, (uint8*)Packet->contents());
			GetReadBuffer().Read((uint8*)Packet->contents(), mRemaining);
		}

		/*sWorldLog.LogPacket(mSize, mOpcode, mSize ? Packet->contents() : NULL, 0);*/
		mRemaining = mSize = mOpcode = 0;

		// Check for packets that we handle
		sLog.outDebug("[WorldSocket] : Reception de l'Opcode:0x%04X", Packet->GetOpcode());
		switch(Packet->GetOpcode())
		{
		case CMSG_PING:
			{
				if(!m_session || !m_session->m_currentPlayer)
				{
					_HandlePing(Packet);
					delete Packet;
					Packet = NULL;
				}
				else
				{
					m_session->m_readQueue.Push(Packet);
					//m_session->Update();
				}
			}break;
		case CMSG_AUTH_SESSION:
			{
				_HandleAuthSession(Packet);
			}break;
		default:
			{
				if(m_session)
				{
					sLog.outDebug("[WorldSocket] : OnReadPacket 0x%04X",Packet->GetOpcode());
					m_session->m_readQueue.Push(Packet);
					//m_session->Update(); // Note Randdrick : OBLIGATOIRE  ! On update la session pour permettre le traitement du paquet.
				}
				else 
				{
					sLog.outError("m_session est null. Le chargement ne peut pas continuer");
					delete Packet;
					Packet = NULL;
				}
			}break;
		}
	}
}

// hacky key
static uint8 PublicKey[265] = { 0x02, 0x01, 0x01, 0xC3, 0x5B, 0x50, 0x84, 0xB9, 0x3E, 0x32, 0x42, 0x8C, 0xD0, 0xC7, 0x48, 0xFA, 0x0E, 0x5D, 0x54, 0x5A, 0xA3, 0x0E, 0x14, 0xBA, 0x9E, 0x0D, 0xB9, 0x5D, 0x8B, 0xEE, 0xB6, 0x84, 0x93, 0x45, 0x75, 0xFF, 0x31, 0xFE, 0x2F, 0x64, 0x3F, 0x3D, 0x6D, 0x07, 0xD9, 0x44, 0x9B, 0x40, 0x85, 0x59, 0x34, 0x4E, 0x10, 0xE1, 0xE7, 0x43, 0x69, 0xEF, 0x7C, 0x16, 0xFC, 0xB4, 0xED, 0x1B, 0x95, 0x28, 0xA8, 0x23, 0x76, 0x51, 0x31, 0x57, 0x30, 0x2B, 0x79, 0x08, 0x50, 0x10, 0x1C, 0x4A, 0x1A, 0x2C, 0xC8, 0x8B, 0x8F, 0x05, 0x2D, 0x22, 0x3D, 0xDB, 0x5A, 0x24, 0x7A, 0x0F, 0x13, 0x50, 0x37, 0x8F, 0x5A, 0xCC, 0x9E, 0x04, 0x44, 0x0E, 0x87, 0x01, 0xD4, 0xA3, 0x15, 0x94, 0x16, 0x34, 0xC6, 0xC2, 0xC3, 0xFB, 0x49, 0xFE, 0xE1, 0xF9, 0xDA, 0x8C, 0x50, 0x3C, 0xBE, 0x2C, 0xBB, 0x57, 0xED, 0x46, 0xB9, 0xAD, 0x8B, 0xC6, 0xDF, 0x0E, 0xD6, 0x0F, 0xBE, 0x80, 0xB3, 0x8B, 0x1E, 0x77, 0xCF, 0xAD, 0x22, 0xCF, 0xB7, 0x4B, 0xCF, 0xFB, 0xF0, 0x6B, 0x11, 0x45, 0x2D, 0x7A, 0x81, 0x18, 0xF2, 0x92, 0x7E, 0x98, 0x56, 0x5D, 0x5E, 0x69, 0x72, 0x0A, 0x0D, 0x03, 0x0A, 0x85, 0xA2, 0x85, 0x9C, 0xCB, 0xFB, 0x56, 0x6E, 0x8F, 0x44, 0xBB, 0x8F, 0x02, 0x22, 0x68, 0x63, 0x97, 0xBC, 0x85, 0xBA, 0xA8, 0xF7, 0xB5, 0x40, 0x68, 0x3C, 0x77, 0x86, 0x6F, 0x4B, 0xD7, 0x88, 0xCA, 0x8A, 0xD7, 0xCE, 0x36, 0xF0, 0x45, 0x6E, 0xD5, 0x64, 0x79, 0x0F, 0x17, 0xFC, 0x64, 0xDD, 0x10, 0x6F, 0xF3, 0xF5, 0xE0, 0xA6, 0xC3, 0xFB, 0x1B, 0x8C, 0x29, 0xEF, 0x8E, 0xE5, 0x34, 0xCB, 0xD1, 0x2A, 0xCE, 0x79, 0xC3, 0x9A, 0x0D, 0x36, 0xEA, 0x01, 0xE0, 0xAA, 0x91, 0x20, 0x54, 0xF0, 0x72, 0xD8, 0x1E, 0xC7, 0x89, 0xD2, 0x00, 0x00, 0x00, 0x00, 0x00};

void WorldSocket::SendAddonInfoPacket(WorldPacket *source, uint32 pos, Session *m_session)
{
	WorldPacket returnpacket;
	returnpacket.Initialize(SMSG_ADDON_INFO);	// SMSG_ADDON_INFO

	uint32 realsize;
	uLongf rsize;

	try
	{
		*source >> realsize;
	}
	catch (ByteBuffer::error &)
	{
		sLog.outDebug("Avertissement : Envoie inachevé de session auth.");
		return;
	}

	rsize = realsize;
	size_t position = source->rpos();

	ByteBuffer unpacked;
	unpacked.resize(realsize);

	if((source->size() - position) < 4 || realsize == 0)
	{
		// we shouldn't get here.. but just in case this will stop any crash here.
		sLog.outDebug("Avertissement : Envoie inachevé de session auth.");
		return;
	}

	int32 result = uncompress((uint8*)unpacked.contents(), &rsize, (uint8*)(*source).contents() + position, (uLong)((*source).size() - position));

	if(result != Z_OK)
	{
		//sLog.outError("La décompression de la section d'addon de CMSG_AUTH_SESSION a échoué.");
		Log.Error("CMSG_AUTH_SESSION","Section d'Addon en erreur");
		return;
	}
	else 
	{
		//sLog.outDetail("La décompression de la section d'addon de CMSG_AUTH_SESSION a réussi.");
		Log.Debug("CMSG_AUTH_SESSION","Section d'Addon Ok");
	}
	
	uint8 Enable; // based on the parsed files from retool
	uint32 crc;
	uint32 unknown;
	
	std::string name;

	uint32 addoncount;
	unpacked >> addoncount;

	uint8 unk;
	uint8 unk1;
	uint8 unk2;
	for(uint32 i = 0; i < addoncount; ++i)
	{
		if(unpacked.rpos() >= unpacked.size())
			break;

		unpacked >> name;
		unpacked >> Enable;
		unpacked >> crc;
		unpacked >> unknown;
		
		// Hacky fix, Yea I know its a hacky fix I will make a proper handler one's I got the crc crap
/*		if (crc != 0x4C1C776D) // CRC of public key version 2.0.1
			returnpacket.append(PublicKey,264); // part of the hacky fix
		else
			returnpacket << uint8(0x02) << uint8(0x01) << uint8(0x00) << uint32(0) << uint8(0);*/
		/*if(!AppendPublicKey(returnpacket, name, crc))
			returnpacket << uint8(1) << uint8(0) << uint8(0);*/

		unk = (Enable ? 2 : 1);
		returnpacket << unk;
		unk1 = (Enable ? 1 : 0);
		returnpacket << unk1;
		if (unk1)
		{
			if(crc != 0x4C1C776D)
			{
				returnpacket << uint8(1);
				returnpacket.append(PublicKey,264);
			}
			else
				returnpacket << uint8(0);

			returnpacket << uint32(0);
		}

		unk2 = (Enable ? 0 : 1);
		returnpacket << unk2;
		if (unk2)
			returnpacket << uint8(0);
	}

	/*unknown 4 bytes at the end of the packet. Stays 0 for me. Tried custom addons, deleting, faulty etc. It stays 0.
	*/
	returnpacket << uint32(0); //Some additional count for additional records, but we won't send them.

	m_session->SendPacket(&returnpacket);
}
