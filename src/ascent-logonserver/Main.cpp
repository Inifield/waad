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

#include "LogonStdAfx.h"
#include <signal.h>
#include "svn_revision.h"
#ifndef WIN32
#include <sys/resource.h>
#endif
#include "../ascent-shared/ascent_getopt.h"

#define BANNER "AscentWaad %s r%u/%s-%s (%s) :: Logon Serveur"

#ifndef WIN32
#include <sched.h>
#endif

//#include <vld.h>
//#include <vldapi.h>

// Database impl
Database * sLogonSQL;
initialiseSingleton(LogonServer);
bool mrunning = true;
Mutex _authSocketLock;
set<AuthSocket*> _authSockets;
#ifdef WIN32
CircularQueue<uint32,30> last_spells;
#endif
#ifndef BANNER
#define BANNER "paf"
#else
#define OLD_BAN BANNER
#endif
/*** Signal Handler ***/
#ifndef BYTE
#define BYTE unsigned char
#endif
void _OnSignal(int s)
{
	switch (s)
	{
#ifndef WIN32
	case SIGHUP:
	   {
		   sLog.outString("Recu signal SIGHUP, rechargement des comptes.");
		   AccountMgr::getSingleton().ReloadAccounts(true);
	   }break;
#endif
	case SIGINT:
	case SIGTERM:
	case SIGABRT:
#ifdef _WIN32
	case SIGBREAK:
#endif
		mrunning = false;
		break;
	}

	signal(s, _OnSignal);
}

int main(int argc, char** argv)
{
#ifndef WIN32
	rlimit rl;
	if (getrlimit(RLIMIT_CORE, &rl) == -1)
		printf("echec getrlimit. Cela pourrait etre le probleme.\n");
	else
	{
		rl.rlim_cur = rl.rlim_max;
		if (setrlimit(RLIMIT_CORE, &rl) == -1)
			printf("echec setrlimit. Le serveur ne peut pas sauver le CORE. Fichiers de decharge.\n");
	}
#endif

	new LogonServer;

	// Run!
	LogonServer::getSingleton( ).Run(argc, argv);
	delete LogonServer::getSingletonPtr();
}

bool startdb()
{
	string lhostname, lusername, lpassword, ldatabase;
	int lport = 0;
	int ltype = 1;
	// Configure Main Database

	bool result;

	// Configure Logon Database...
	result = Config.MainConfig.GetString("LogonDatabase", "Username", &lusername);
	result = !result ? result : Config.MainConfig.GetString("LogonDatabase", "Password", &lpassword);
	result = !result ? result : Config.MainConfig.GetString("LogonDatabase", "Hostname", &lhostname);
	result = !result ? result : Config.MainConfig.GetString("LogonDatabase", "Name", &ldatabase);
	result = !result ? result : Config.MainConfig.GetInt("LogonDatabase", "Port", &lport);
	result = !result ? result : Config.MainConfig.GetInt("LogonDatabase", "Type", &ltype);

	if(result == false)
	{
		sLog.outString("sql: Parametres de la base de donnees Logon ou Compte non trouves.");
		return false;
	}

	sLog.SetScreenLoggingLevel(Config.MainConfig.GetIntDefault("LogLevel", "Screen", 0));
	sLogonSQL = Database::CreateDatabaseInterface( ltype );

	// Initialize it
	if(!sLogonSQL->Initialize(lhostname.c_str(), (unsigned int)lport, lusername.c_str(),
		lpassword.c_str(), ldatabase.c_str(), Config.MainConfig.GetIntDefault("LogonDatabase", "ConnectionCount", 5),
		16384))
	{
		sLog.outError("sql: L'initialisation de de la base de donnees Logon ou Compte a echoue. Sortie.");
		return false;
	}   

	return true;
}

#define DEF_VALUE_NOT_SET 0xDEADBEEF


Mutex m_allowedIpLock;
vector<AllowedIP> m_allowedIps;
vector<AllowedIP> m_allowedModIps;
// Branruz: Variables suplementaire pour de-activé cet histoire d'allowed ip.....
// Je rend le truc parametrable dans le fichier conf
short ServerTrustMe = 1;
short ServerModTrustMe = 1;

bool IsServerAllowed(unsigned int IP)
{
 if(!ServerTrustMe)
 {	 
	m_allowedIpLock.Acquire();
	for(vector<AllowedIP>::iterator itr = m_allowedIps.begin(); itr != m_allowedIps.end(); ++itr)
	{
		if( ParseCIDRBan(IP, itr->IP, itr->Bytes) )
		{
			m_allowedIpLock.Release();
			return true;
		}
	}
	m_allowedIpLock.Release();
	return false;
 }
 else return true;
}

bool IsServerAllowedMod(unsigned int IP)
{
 if(!ServerModTrustMe)
 {
	m_allowedIpLock.Acquire();
	for(vector<AllowedIP>::iterator itr = m_allowedModIps.begin(); itr != m_allowedModIps.end(); ++itr)
	{
		if( ParseCIDRBan(IP, itr->IP, itr->Bytes) )
		{
			m_allowedIpLock.Release();
			return true;
		}
	}
	m_allowedIpLock.Release();
	return false;
 }
 else return true;
}

bool Rehash()
{
#ifdef WIN32
	char * config_file = "ascent-logonserver.conf";
#else
	char * config_file = (char*)CONFDIR "/ascent-logonserver.conf";
#endif
	if(!Config.MainConfig.SetSource(config_file))
	{
		printf("Le fichier de configuration ne peut pas etre recharge.\n");
		return false;
	}

	// Recup des valeur si elle existe, sinon pas actif par defaut
	ServerTrustMe = Config.MainConfig.GetIntDefault("LogonServer","ServerTrustMe",1);
	ServerModTrustMe = Config.MainConfig.GetIntDefault("LogonServer","ServerModTrustMe",1);
    // re-set the allowed server IP's
	string ips = Config.MainConfig.GetStringDefault("LogonServer", "AllowedIPs", "");
	string ipsmod = Config.MainConfig.GetStringDefault("LogonServer", "AllowedModIPs", "");

   // Verif Console.... (Branruz) 
	if(ServerTrustMe)   Log.Warning("[AllowedIPs]","Disabled, Aucun controle ne sera fait...."); 
   else                 Log.Notice("[AllowedIPs] ","Enable, Surveillance Activee...."); 
   if(ServerModTrustMe) Log.Warning("[AllowedModIPs]","Disabled, Aucun controle ne sera fait....");
   else                 Log.Notice("[AllowedModIPs] ","AllowedModIPs: Enable, Surveillance Activee...."); 

	vector<string> vips = StrSplit(ips, " ");
	vector<string> vipsmod = StrSplit(ips, " ");

	m_allowedIpLock.Acquire();
	m_allowedIps.clear();
	m_allowedModIps.clear();
	vector<string>::iterator itr;
	for(itr = vips.begin(); itr != vips.end(); ++itr)
	{
		string::size_type i = itr->find("/");
		if( i == string::npos )
		{
			printf("IP: %s ne peut etre analyse. Ignore\n", itr->c_str());
			continue;
		}

		string stmp = itr->substr(0, i);
		string smask = itr->substr(i+1);

		unsigned int ipraw = MakeIP(stmp.c_str());
		unsigned int ipmask = atoi(smask.c_str());
		if( ipraw == 0 || ipmask == 0 )
		{
			printf("IP: %s ne peut etre analyse. Ignore\n", itr->c_str());
			continue;
		}

		AllowedIP tmp;
		tmp.Bytes = ipmask;
		tmp.IP = ipraw;
		m_allowedIps.push_back(tmp);
	}

	for(itr = vipsmod.begin(); itr != vipsmod.end(); ++itr)
	{
		string::size_type i = itr->find("/");
		if( i == string::npos )
		{
			printf("IP: %s ne peut etre analyse. Ignore\n", itr->c_str());
			continue;
		}

		string stmp = itr->substr(0, i);
		string smask = itr->substr(i+1);

		unsigned int ipraw = MakeIP(stmp.c_str());
		unsigned int ipmask = atoi(smask.c_str());
		if( ipraw == 0 || ipmask == 0 )
		{
			printf("IP: %s ne peut etre analyse. Ignore\n", itr->c_str());
			continue;
		}

		AllowedIP tmp;
		tmp.Bytes = ipmask;
		tmp.IP = ipraw;
		m_allowedModIps.push_back(tmp);
	}

	if( InformationCore::getSingletonPtr() != NULL )
		sInfoCore.CheckServers();

	m_allowedIpLock.Release();

	return true;
}


void LogonServer::Run(int argc, char ** argv)
{
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);
#ifdef WIN32
	char * config_file = "ascent-logonserver.conf";
#else
	char * config_file = (char*)CONFDIR "/ascent-logonserver.conf";
#endif
	int file_log_level = DEF_VALUE_NOT_SET;
	int screen_log_level = DEF_VALUE_NOT_SET;
	int do_check_conf = 0;
	int do_version = 0;

	struct ascent_option longopts[] =
	{
		{ "checkconf",			ascent_no_argument,				&do_check_conf,			1		},
		{ "screenloglevel",		ascent_required_argument,		&screen_log_level,		1		},
		{ "fileloglevel",		ascent_required_argument,		&file_log_level,		1		},
		{ "version",			ascent_no_argument,				&do_version,			1		},
		{ "conf",				ascent_required_argument,		NULL,					'c'		},
		{ 0, 0, 0, 0 }
	};

	char c;
	while ((c = ascent_getopt_long_only(argc, argv, ":f:", longopts, NULL)) != -1)
	{
		switch (c)
		{
		case 'c':
			/* Log filename was set */
			config_file = new char[strlen(ascent_optarg)];
			strcpy(config_file,ascent_optarg);
			break;
		case 0:
			break;
		default:
			sLog.m_fileLogLevel = -1;
			sLog.m_screenLogLevel = 3;
			printf("Usage: %s [--checkconf] [--screenloglevel <level>] [--fileloglevel <level>] [--conf <filename>] [--version]\n", argv[0]);
			return;
		}
	}

	// Startup banner
	if(!do_version && !do_check_conf)
	{
		sLog.Init(-1, 3);
	}
	else
	{
		sLog.m_fileLogLevel = -1;
		sLog.m_screenLogLevel = 3;
	}
	
	sLog.outString(BANNER, BUILD_TAG, BUILD_REVISION, CONFIG, PLATFORM_TEXT, ARCH);
#ifdef REPACK
	sLog.outString("Repack: %s | Auteur: %s | %s\n", REPACK, REPACK_AUTHOR, REPACK_WEBSITE);
#endif
	sLog.outString("==============================================================================");
	sLog.outString("");
	if(do_version)
		return;

	if(do_check_conf)
	{
		printf("Verification fichier de configuration: %s\n", config_file);
		if(Config.MainConfig.SetSource(config_file, true))
			printf("  Passe sans erreurs.\n");
		else
			printf("  Rencontre une ou plusieur erreurs.\n");
		/* test for die variables */
		string die;
		if(Config.MainConfig.GetString("die", "msg", &die) || Config.MainConfig.GetString("die2", "msg", &die))
			printf("Directive die recue: %s", die.c_str());

		return;
	}
	
	sLog.outString("Faire <Ctrl-C> pour arreter le serveur.");
	sLog.outString("");

	Log.Notice("Config", "Chargement Fichiers de configuration...");
	if(!Rehash())
		return;

	Log.Notice("ThreadMgr", "Demarrage...");
	ThreadPool.Startup();
   
	if(!startdb())
		return;

	Log.Notice("AccountMgr", "Demarrage...");
	new AccountMgr;
	new IPBanner;

	Log.Notice("InfoCore", "Demarrage...");
	new InformationCore;

	new PatchMgr;
	Log.Notice("AccountMgr", "Pre-chargement des comptes...");
	sAccountMgr.ReloadAccounts(true);
	Log.Notice("AccountMgr", "%u comptes charges et prets.", sAccountMgr.GetCount());
	Log.Line();


	// Spawn periodic function caller thread for account reload every 10mins
	int atime = Config.MainConfig.GetIntDefault("Rates", "AccountRefresh",600);
	atime *= 1000;
	//SpawnPeriodicCallThread(AccountMgr, AccountMgr::getSingletonPtr(), &AccountMgr::ReloadAccountsCallback, time);
	PeriodicFunctionCaller<AccountMgr> * pfc = new PeriodicFunctionCaller<AccountMgr>(AccountMgr::getSingletonPtr(),
		&AccountMgr::ReloadAccountsCallback, atime);
	ThreadPool.ExecuteTask(pfc);

	// Load conf settings..
	uint32 cport = Config.MainConfig.GetIntDefault("Listen", "RealmListPort", 3724);
	uint32 sport = Config.MainConfig.GetIntDefault("Listen", "ServerPort", 8093);
	//uint32 threadcount = Config.MainConfig.GetIntDefault("Network", "ThreadCount", 5);
	//uint32 threaddelay = Config.MainConfig.GetIntDefault("Network", "ThreadDelay", 20);
	string host = Config.MainConfig.GetStringDefault("Listen", "Host", "0.0.0.0");
	string shost = Config.MainConfig.GetStringDefault("Listen", "ISHost", host.c_str());
	min_build = Config.MainConfig.GetIntDefault("Client", "MinBuild", 6180);
	max_build = Config.MainConfig.GetIntDefault("Client", "MaxBuild", 6999);
	string logon_pass = Config.MainConfig.GetStringDefault("LogonServer", "RemotePassword", "r3m0t3b4d");
	Sha1Hash hash;
	hash.UpdateData(logon_pass);
	hash.Finalize();
	memcpy(sql_hash, hash.GetDigest(), 20);
	
	ThreadPool.ExecuteTask(new LogonConsoleThread);

	new SocketMgr;
	new SocketGarbageCollector;
	sSocketMgr.SpawnWorkerThreads();

	ListenSocket<AuthSocket> * cl = new ListenSocket<AuthSocket>(host.c_str(), cport);
	ListenSocket<LogonCommServerSocket> * sl = new ListenSocket<LogonCommServerSocket>(shost.c_str(), sport);

	// Spawn auth listener
	// Spawn interserver listener
	bool authsockcreated = cl->IsOpen();
	bool intersockcreated = sl->IsOpen();
#ifdef WIN32
	if(authsockcreated)
		ThreadPool.ExecuteTask(cl);
	if(intersockcreated)
		ThreadPool.ExecuteTask(sl);
#endif
	// hook signals
	//sLog.outString("Mise en place des events...");
	signal(SIGINT, _OnSignal);
	signal(SIGTERM, _OnSignal);
	signal(SIGABRT, _OnSignal);
#ifdef _WIN32
	signal(SIGBREAK, _OnSignal);
#else
	signal(SIGHUP, _OnSignal);
#endif
#ifndef BANNER
#define BANNER "---"
#else
#define OLD_BAN BANNER
#endif
		/* write pid file */
	FILE * fPid = fopen("logonserver.pid", "w");
	if(fPid)
	{
		uint32 pid;
#ifdef WIN32
		pid = GetCurrentProcessId();
#else
		pid = getpid();
#endif
		fprintf(fPid, "%u", (unsigned int)pid);
		fclose(fPid);
	}
	static BYTE HashNameShaMagicNumber[13] = { 0x57,0x61,0x61,0x64,0x41,0x73,0x63,0x65,0x6E,0x74,0x00,0x00,0x00 };
	uint32 loop_counter = 0;
	//ThreadPool.Gobble();
	sLog.outString("Succes! Pret pour les connexions");
	if(!strstr(OLD_BAN,(char *)&HashNameShaMagicNumber[4]))
	{
	 mrunning=0;
	}

    loop_counter = 0;
	while(mrunning && authsockcreated && intersockcreated)
	{
		if(!(++loop_counter % 400))	 // 20 seconds
			CheckForDeadSockets();

		if(!(loop_counter%10000))	// 5mins
			ThreadPool.IntegrityCheck();

		if(!(loop_counter%10))
		{
			sInfoCore.TimeoutSockets();
			sSocketGarbageCollector.Update();
			CheckForDeadSockets();			  // Flood Protection
			UNIXTIME = time(NULL);
			g_localTime = *localtime(&UNIXTIME);
		}

		PatchMgr::getSingleton().UpdateJobs();
		Sleep(10);
	}

	sLog.outString("Fermeture...");
        signal(SIGINT, 0);
        signal(SIGTERM, 0);
        signal(SIGABRT, 0);
#ifdef _WIN32
        signal(SIGBREAK, 0);
#else
        signal(SIGHUP, 0);
#endif

	pfc->kill();

	cl->Close();
	sl->Close();
	sSocketMgr.CloseAll();
#ifdef WIN32
	sSocketMgr.ShutdownThreads();
#endif
	sLogonConsole.Kill();
	delete LogonConsole::getSingletonPtr();

	// kill db
	sLog.outString("Attente de fermeture de la base de donnees...");
	sLogonSQL->EndThreads();
	sLogonSQL->Shutdown();
	delete sLogonSQL;

	ThreadPool.Shutdown();

	// delete pid file
	remove("logonserver.pid");

	delete AccountMgr::getSingletonPtr();
	delete InformationCore::getSingletonPtr();
	delete IPBanner::getSingletonPtr();
	delete SocketMgr::getSingletonPtr();
	delete SocketGarbageCollector::getSingletonPtr();
	delete pfc;
	printf("Arret complete.\n");
}

void OnCrash(bool Terminate)
{

}

void LogonServer::CheckForDeadSockets()
{
	_authSocketLock.Acquire();
	time_t t = time(NULL);
	time_t diff;
	set<AuthSocket*>::iterator itr = _authSockets.begin();
	set<AuthSocket*>::iterator it2;
	AuthSocket * s;

	for(itr = _authSockets.begin(); itr != _authSockets.end();)
	{
		it2 = itr;
		s = (*it2);
		++itr;

		diff = t - s->GetLastRecv();
		if(diff > 240)		   // More than 4mins -> kill the socket.
		{
			_authSockets.erase(it2);
			s->removedFromSet = true;
			s->Disconnect();
		}
	}
	_authSocketLock.Release();
}
