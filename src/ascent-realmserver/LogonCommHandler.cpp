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
initialiseSingleton(LogonCommHandler);

LogonCommHandler::LogonCommHandler()
{
	idhigh = 1;
	next_request = 1;
	string strkey = Config.ClusterConfig.GetStringDefault("Cluster", "Key", "r3m0t3b5d");
	pings = !Config.RealmConfig.GetBoolDefault("LogonServer", "DisablePings", false);
	string logon_pass = Config.RealmConfig.GetStringDefault("LogonServer", "RemotePassword", "r3m0t3");
	
	// sha1 hash it
	Sha1Hash hash;
	hash.UpdateData(logon_pass);
	hash.Finalize();
	memcpy(sql_passhash, hash.GetDigest(), 20);
	
	/* hash the cluster key */
	Sha1Hash k;
	k.UpdateData(strkey);
	k.Finalize();
	memcpy(key, k.GetDigest(), 20);
}

LogonCommHandler::~LogonCommHandler()
{
	for(set<LogonServer*>::iterator i = servers.begin(); i != servers.end(); ++i)
		delete (*i);

	for(set<Realm*>::iterator i = realms.begin(); i != realms.end(); ++i)
		delete (*i);
}

LogonCommClientSocket * LogonCommHandler::ConnectToLogon(string Address, uint32 Port)
{
	LogonCommClientSocket * conn = ConnectTCPSocket<LogonCommClientSocket>(Address.c_str(), Port);	
	return conn;
}

void LogonCommHandler::RequestAddition(LogonCommClientSocket * Socket)
{
	set<Realm*>::iterator itr = realms.begin();
	WorldPacket data(RCMSG_REGISTER_REALM, 100);
	for(; itr != realms.end(); ++itr)
	{
		data.clear();

		// Add realm to the packet
		Realm * realm = *itr;
		data << realm->Name;
		data << realm->Address;
		data << realm->Colour;
		data << realm->Icon;
		data << realm->TimeZone;
		data << realm->Population;
		Socket->SendPacket(&data,false);
	}
}

void LogonCommHandler::Startup()
{
	// Try to connect to all logons.
	sLog.outColor(TNORMAL, "\n >> loading realms and logon server definitions... ");
	LoadRealmConfiguration();

	sLog.outColor(TNORMAL, " >> attempting to connect to all logon servers... \n");

	for(set<LogonServer*>::iterator itr = servers.begin(); itr != servers.end(); ++itr)
		Connect(*itr);

	sLog.outColor(TNORMAL, "\n");
}

void LogonCommHandler::ReloadForcedPermissions() //ajout par B.B.
{
	//Log.Notice("LogonCommClient", "Mise a jour des permissions player.");

	QueryResult * result = CharacterDatabase.Query("SELECT * FROM account_forced_permissions");

	forced_permissions.clear(); //on purge les permissions forc�es

	if( result != NULL )
	{
		do 
		{
			string acct = result->Fetch()[0].GetString();
			string perm = result->Fetch()[1].GetString();

			ASCENT_TOUPPER(acct);
            forced_permissions.insert(make_pair(acct,perm));

		} while (result->NextRow());
		delete result;
	}
}

const string* LogonCommHandler::GetForcedPermissions(string& username)
{
	ForcedPermissionMap::iterator itr = forced_permissions.find(username);
	if(itr == forced_permissions.end())
		return NULL;

	return &itr->second;
}
void LogonCommHandler::Connect(LogonServer * server)
{
	sLog.outColor(TNORMAL, "	>> connecting to `%s` on `%s:%u`...", server->Name.c_str(), server->Address.c_str(), server->Port);
	server->RetryTime = (uint32)UNIXTIME + 10;
	server->Registered = false;
	LogonCommClientSocket * conn = ConnectToLogon(server->Address, server->Port);
	logons[server] = conn;
	if(conn == 0)
	{
		sLog.outColor(TRED, " fail!\n	   server connection failed. I will try again later.");
		sLog.outColor(TNORMAL, "\n");
		return;
	}
	sLog.outColor(TGREEN, " ok!\n");
	sLog.outColor(TNORMAL, "        >> authenticating...\n");
	sLog.outColor(TNORMAL, "        >> ");
	uint32 tt = uint32(time(NULL) + 10);
	conn->SendChallenge();
	sLog.outColor(TNORMAL, "        >> result:");
	while(!conn->authenticated)
	{
		if((uint32)UNIXTIME >= tt)
		{
			sLog.outColor(TYELLOW, " timeout.\n");
			return;
		}

		Sleep(50);
	}

	if(conn->authenticated != 1)
	{
		sLog.outColor(TRED, " failure.\n");
		logons[server] = 0;
		conn->Disconnect();
		return;
	}
	else
		sLog.outColor(TGREEN, " ok!\n");

	// Send the initial ping
	conn->SendPing();

	sLog.outColor(TNORMAL, "        >> registering realms... ");
	conn->_id = server->ID;

	RequestAddition(conn);

	uint32 st = (uint32)UNIXTIME + 10;

	// Wait for register ACK
	while(server->Registered == false)
	{
		// Don't wait more than.. like 10 seconds for a registration
		if((uint32)UNIXTIME >= st)
		{
			sLog.outColor(TYELLOW, "timeout.");
			logons[server] = 0;
			conn->Disconnect();
			break;
		}
		Sleep(50);
	}

	if(!server->Registered)
		return;

	// Wait for all realms to register
	Sleep(200);

	sLog.outColor(TNORMAL, "\n        >> ping test: ");
	sLog.outColor(TYELLOW, "%ums", conn->latency);
	sLog.outColor(TNORMAL, "\n");
}

void LogonCommHandler::AdditionAck(uint32 ID, uint32 ServID)
{
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	for(; itr != logons.end(); ++itr)
	{
		if(itr->first->ID == ID)
		{
			itr->first->ServerID = ServID;
			itr->first->Registered = true;
			return;
		}
	}
}

void LogonCommHandler::UpdateSockets()
{
	mapLock.Acquire();

	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	LogonCommClientSocket * cs;
	uint32 t = (uint32)UNIXTIME;
	for(; itr != logons.end(); ++itr)
	{
		cs = itr->second;
		if(cs != 0)
		{
			if(!pings) continue;

            if(cs->IsDeleted() || !cs->IsConnected())
            {
                cs->_id = 0;
                itr->second = 0;
                continue;
            }

			if(cs->last_pong < t && ((t - cs->last_pong) > 60))
			{
				// no pong for 60 seconds -> remove the socket
				printf(" >> realm id %u connection dropped due to pong timeout.\n", (unsigned int)itr->first->ID);
				cs->_id = 0;
				cs->Disconnect();
				itr->second = 0;
				continue;
			}

			if( (t - cs->last_ping) > 15 )
			{
				// send a ping packet.
				cs->SendPing();
			}
		}
		else
		{
			// check retry time
			if(t >= itr->first->RetryTime)
			{
				Connect(itr->first);
			}
		}
	}
	mapLock.Release();
}

void LogonCommHandler::ConnectionDropped(uint32 ID)
{
	mapLock.Acquire();
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	for(; itr != logons.end(); ++itr)
	{
		if(itr->first->ID == ID && itr->second != 0)
		{
			sLog.outColor(TNORMAL, " >> realm id %u connection was dropped unexpectedly. reconnecting next loop.", ID);
			sLog.outColor(TNORMAL, "\n");
			itr->second = 0;
			break;
		}
	}
	mapLock.Release();
}

uint32 LogonCommHandler::ClientConnected(string AccountName, WorldSocket * Socket)
{
	uint32 request_id = next_request++;
	size_t i = 0;
	const char * acct = AccountName.c_str();
	printf( " >> sending request for account information: `%s` (request %u).", AccountName.c_str(), request_id);
	//  sLog.outColor(TNORMAL, "\n");

	// Send request packet to server.
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0)
	{
		// No valid logonserver is connected.
		return (uint32)-1;
	}

	LogonCommClientSocket * s = itr->second;
	if( s == NULL )
		return (uint32)-1;

	pendingLock.Acquire();

	WorldPacket data(RCMSG_REQUEST_SESSION, 100);
	data << request_id;

	// strip the shitty hash from it
	for(; acct[i] != '#' && acct[i] != '\0'; ++i )
		data.append( &acct[i], 1 );

	data.append( "\0", 1 );
	s->SendPacket(&data,false);

	pending_logons[request_id] = Socket;
	pendingLock.Release();

	return request_id;	
}

void LogonCommHandler::UnauthedSocketClose(uint32 id)
{
	pendingLock.Acquire();
	pending_logons.erase(id);
	pendingLock.Release();
}

void LogonCommHandler::RemoveUnauthedSocket(uint32 id)
{
	pending_logons.erase(id);
}

/*
void LogonCommHandler::LogonDatabaseSQLExecute(const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	char query[1024];
	vsnprintf(query, 1024, str, ap);
	va_end(ap);
	string q = string(query);

	WorldPacket data(RCMSG_SQL_EXECUTE, q.size()+1);
	data << q;
	
	// Send request packet to server.
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}
	itr->second->SendPacket(&data);
}

void LogonCommHandler::LogonDatabaseReloadAccounts()
{
	WorldPacket data(RCMSG_RELOAD_ACCOUNTS, 1);
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}
	itr->second->SendPacket(&data);
}
*/
void LogonCommHandler::LoadRealmConfiguration()
{
	LogonServer * ls = new LogonServer;
	ls->ID = idhigh++;
	ls->Address = Config.RealmConfig.GetStringDefault("LogonServer", "IpOrHost", "localhost");
	ls->Port = Config.RealmConfig.GetIntDefault("LogonServer", "Port", 8093);
	ls->Name = Config.RealmConfig.GetStringDefault("LogonServer", "Name", "UnkLogon");
	servers.insert(ls);
	sLog.outColor(TYELLOW, "1 servers, ");

	uint32 realmcount = Config.RealmConfig.GetIntDefault("LogonServer", "RealmCount", 1);
	if(realmcount == 0)
	{
		sLog.outColor(TRED, "\n   >> no realms found. this server will not be online anywhere!\n");
	}
	else
	{
		for(uint32 i = 1; i < realmcount+1; ++i)
		{
			Realm * realm = new Realm;
			realm->Name = Config.RealmConfig.GetStringVA("Name", "SomeRealm", "Realm%u", i);
			realm->Address = Config.RealmConfig.GetStringVA("Address", "127.0.0.1:8129", "Realm%u", i);
			realm->Colour = Config.RealmConfig.GetIntVA("Colour", 1, "Realm%u", i);
			realm->TimeZone = Config.RealmConfig.GetIntVA("TimeZone", 1, "Realm%u", i);
			realm->Population = Config.RealmConfig.GetFloatVA("Population", 0, "Realm%u", i);
			string rt = Config.RealmConfig.GetStringVA("Icon", "Normal", "Realm%u", i);
			uint32 type;

			// process realm type
			if(!stricmp(rt.c_str(), "pvp"))
				type = REALMTYPE_PVP;
			else if(!stricmp(rt.c_str(), "rp"))
				type = REALMTYPE_RP;
			else if(!stricmp(rt.c_str(), "rppvp"))
				type = REALMTYPE_RPPVP;
			else
				type = REALMTYPE_NORMAL;

			_realmType = type;

			realm->Icon = type;
			realms.insert(realm);
		}
		sLog.outColor(TBLUE, "%u realms.\n", realmcount);
	}
}

void LogonCommHandler::UpdateAccountCount(uint32 account_id, uint8 add)
{
	// Send request packet to server.
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}
	itr->second->UpdateAccountCount(account_id, add);
}

void LogonCommHandler::TestConsoleLogon(string& username, string& password, uint32 requestnum)
{
	string newuser = username;
	string newpass = password;
	string srpstr;

	ASCENT_TOUPPER(newuser);
	ASCENT_TOUPPER(newpass);

	srpstr = newuser + ":" + newpass;

	// Send request packet to server.
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}

	Sha1Hash hash;
	hash.UpdateData(srpstr);
	hash.Finalize();

	WorldPacket data(RCMSG_TEST_CONSOLE_LOGIN, 100);
	data << requestnum;
	data << newuser;
	data.append(hash.GetDigest(), 20);

	itr->second->SendPacket(&data, false);
}

// db funcs
void LogonCommHandler::Account_SetBanned(const char * account, uint32 banned)
{
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(1);		// 1 = ban
	data << account;
	data << banned;
	itr->second->SendPacket(&data, false);
}

void LogonCommHandler::Account_SetGM(const char * account, const char * flags)
{
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(2);		// 2 = set gm
	data << account;
	data << flags;
	itr->second->SendPacket(&data, false);
}

void LogonCommHandler::Account_SetMute(const char * account, uint32 muted)
{
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(3);		// 3 = mute
	data << account;
	data << muted;
	itr->second->SendPacket(&data, false);
}

void LogonCommHandler::IPBan_Add(const char * ip, uint32 duration)
{
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(4);		// 4 = ipban add
	data << ip;
	data << duration;
	itr->second->SendPacket(&data, false);
}

void LogonCommHandler::IPBan_Remove(const char * ip)
{
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(5);		// 5 = ipban remove
	data << ip;
	itr->second->SendPacket(&data, false);
}

void LogonCommHandler::Account_SetOneDK(const uint32 _GuidPlayer, bool OneDKCreated)
{
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(6);		// 6 = set Dk created
	data << (uint32)_GuidPlayer;
	data << (uint32)OneDKCreated;
	itr->second->SendPacket(&data, false);
}
