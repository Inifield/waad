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

#define BANNER "AscentWaad %s r%u/%s-%s-%s :: World Server"

#ifndef WIN32
#include <sched.h>
#endif

#include "svn_revision.h"

#include <signal.h>

#define OBJECT_WAIT_TIME 50 //in milliseconds
#define CHECK_COUNTER_TIMER(stime) !(loopcounter % (stime * (1000 / OBJECT_WAIT_TIME)))

// fix pour la compile Linux a la Branruz ( lol Franck ;) (Branruz) )
#ifndef BYTE
#define BYTE unsigned char
#endif

const char* RateExtensions[] = { " bits/sec", "kbps", "mbps", "gbps", "tbps" }; //if you go past tbps, somethings fucked

createFileSingleton( Master );
std::string LogFileName;
bool bLogChat;
bool crashed = false;

volatile bool Master::m_stopEvent = false;

// Database defines.
SERVER_DECL Database* Database_Character;
SERVER_DECL Database* Database_World;

// mainserv defines
SessionLogWriter* GMCommand_Log;
SessionLogWriter* Anticheat_Log;
SessionLogWriter* Player_Log;
extern DayWatcherThread * dw;

void Master::_OnSignal(int s)
{
	switch (s)
	{
#ifndef WIN32
	case SIGHUP:
		sWorld.Rehash(true);
		break;
#endif
	case SIGINT:
	case SIGTERM:
	case SIGABRT:
#ifdef _WIN32
	case SIGBREAK:
#endif
		Master::m_stopEvent = true;
		break;
	}

	signal(s, _OnSignal);
}

Master::Master()
{
	m_ShutdownTimer = 0;
	m_ShutdownEvent = false;
	m_restartEvent = false;
}

Master::~Master()
{
}

struct Addr
{
	unsigned short sa_family;
	/* sa_data */
	unsigned short Port;
	unsigned long IP; // inet_addr
	unsigned long unusedA;
	unsigned long unusedB;
};

#define DEF_VALUE_NOT_SET 0xDEADBEEF

#ifdef WIN32
        static const char* default_config_file = "ascent-world.conf";
        static const char* default_realm_config_file = "ascent-realms.conf";
#else
        static const char* default_config_file = CONFDIR "/ascent-world.conf";
        static const char* default_realm_config_file = CONFDIR "/ascent-realms.conf";
#endif

// Intouchable..... ;)
const char h0rs[] = ""
"\nWaad Team - Core 3.3.5"
"";

bool bServerShutdown = false;
bool StartConsoleListener();
void CloseConsoleListener();
ThreadBase * GetConsoleListener();



bool Master::Run(int argc, char ** argv)
{
		std::string s=argv[0];
#ifdef WIN32
	//remove .exe if windows
	replace(s, ".exe", ".conf");
#else
	//stick .conf on end for others
	s = default_config_file;
#endif
	char * config_file = (char*)s.c_str();
	char * realm_config_file = (char*)default_realm_config_file;
	int file_log_level = DEF_VALUE_NOT_SET;
	int screen_log_level = DEF_VALUE_NOT_SET;
	int do_check_conf = 0;
	int do_version = 0;
	int do_cheater_check = 0;
	int do_database_clean = 0;
	time_t curTime;

	struct ascent_option longopts[] =
	{
		{ "checkconf",			ascent_no_argument,				&do_check_conf,			1		},
		{ "screenloglevel",		ascent_required_argument,		&screen_log_level,		1		},
		{ "fileloglevel",		ascent_required_argument,		&file_log_level,		1		},
		{ "version",			ascent_no_argument,				&do_version,			1		},
		{ "conf",				ascent_required_argument,		NULL,					'c'		},
		{ "realmconf",			ascent_required_argument,		NULL,					'r'		},
		{ "databasecleanup",	ascent_no_argument,				&do_database_clean,		1		},
		{ "cheatercheck",		ascent_no_argument,				&do_cheater_check,		1		},
		{ 0, 0, 0, 0 }
	};

	char c;
	while ((c = ascent_getopt_long_only(argc, argv, ":f:", longopts, NULL)) != -1)
	{
		switch (c)
		{
		case 'c':
			config_file = new char[strlen(ascent_optarg)];
			strncpy(config_file, ascent_optarg,strlen(ascent_optarg)-1);
			break;
			
		case 'r':
			realm_config_file = new char[strlen(ascent_optarg)];
			strncpy(realm_config_file, ascent_optarg,strlen(ascent_optarg)-1);
			break;
		case 0:
			break;
		default:
			sLog.m_fileLogLevel = -1;
			sLog.m_screenLogLevel = 3;
			printf("Usage: %s [--checkconf] [--screenloglevel <level>] [--fileloglevel <level>] [--conf <filename>] [--realmconf <filename>] [--version] [--databasecleanup] [--cheatercheck]\n", argv[0]);
			return true;
		}
	}

	// Startup banner
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);

	if(!do_version && !do_check_conf)
	{
		sLog.Init(-1, 3);
	}
	else
	{
		sLog.m_fileLogLevel = -1;
		sLog.m_screenLogLevel = 1;
	}

	printf(BANNER, BUILD_TAG, BUILD_REVISION, CONFIG, PLATFORM_TEXT, ARCH);
#ifdef REPACK
	printf("\nRepack: %s | Author: %s | %s\n", REPACK, REPACK_AUTHOR, REPACK_WEBSITE);
#endif
	puts(h0rs);
	printf("\nCopyright (C) 2005-2008 Ascent Team. http://www.ascentemu.com/\n");
	printf("and (C) 2008 WaadTeam since V4588 of Ascent. http://arbonne.games-rpg.net/\n");
	printf("This program comes with ABSOLUTELY NO WARRANTY, and is FREE SOFTWARE.\n");
	printf("You are welcome to redistribute it under the terms of the GNU Affero\n");
	printf("General Public License, either version 3 or any later version. For a\n");
	printf("copy of this license, see the COPYING file provided with this distribution.\n");
	Log.Line();
#ifdef REPACK
	Log.Color(TRED);
	printf("Warning: Using repacks is potentially dangerous. You should always compile\n");
	printf("from the source yourself at www.ascentemu.com or arbonne.games-rpg.net\n");
	printf("By using this repack, you agree to not visit the Ascent or WaadTeam website and ask\nfor support.\n");
	printf("For all support, you should visit the repacker's website at %s\n", REPACK_WEBSITE);
	Log.Color(TNORMAL);
	Log.Line();
#endif
	Log.log_level = 3;

	if(do_version)
		return true;

	if( do_check_conf )
	{
		Log.Notice( "Config", "Checking config file: %s", config_file );
		if( Config.MainConfig.SetSource(config_file, true ) )
			Log.Success( "Config", "Passed without errors." );
		else
			Log.Warning( "Config", "Encountered one or more errors." );

		Log.Notice( "Config", "Checking config file: %s\n", realm_config_file );
		if( Config.RealmConfig.SetSource( realm_config_file, true ) )
			Log.Success( "Config", "Passed without errors.\n" );
		else
			Log.Warning( "Config", "Encountered one or more errors.\n" );
		/* test for die variables */
		string die;
		if( Config.MainConfig.GetString( "die", "msg", &die) || Config.MainConfig.GetString("die2", "msg", &die ) )
			Log.Warning( "Config", "Die directive received: %s", die.c_str() );

		return true;
	}

	printf( "The key combination <Ctrl-C> will safely shut down the server at any time.\n" );
	Log.Line();
    
#ifndef WIN32
	if(geteuid() == 0 || getegid() == 0)
		Log.LargeErrorMessage( LARGERRORMESSAGE_WARNING, "You are running Ascent as root.", "This is not needed, and may be a possible security risk.", "It is advised to hit CTRL+C now and", "start as a non-privileged user.", NULL);
#endif

	InitRandomNumberGenerators();
	Log.Success( "Rnd", "Initialized Random Number Generators." );

	ThreadPool.Startup();
	uint32 LoadingTime = getMSTime();

	Log.Notice( "Config", "Loading Config Files...\n" );
	if( Config.MainConfig.SetSource( config_file ) )
		Log.Success( "Config", ">> ascent-world.conf" );
	else
	{
		Log.Error( "Config", ">> ascent-world.conf" );
		return false;
	}

	string die;
	if( Config.MainConfig.GetString( "die", "msg", &die) || Config.MainConfig.GetString( "die2", "msg", &die ) )
	{
		Log.Warning( "Config", "Die directive received: %s", die.c_str() );
		return false;
	}	

	if(Config.RealmConfig.SetSource(realm_config_file))
		Log.Success( "Config", ">> ascent-realms.conf" );
	else
	{
		Log.Error( "Config", ">> ascent-realms.conf" );
		return false;
	}
	if( !_StartDB() )
	{
		Database::CleanupLibs(); // Fix Ascent V4638
		return false;
	}

	if(do_database_clean)
	{
		printf( "\nEntering database maintenance mode.\n\n" );
		new DatabaseCleaner;
		DatabaseCleaner::getSingleton().Run();
		delete DatabaseCleaner::getSingletonPtr();
		Log.Color(TYELLOW);
		printf( "\nMaintenence finished. Take a moment to review the output, and hit space to continue startup." );
		Log.Color(TNORMAL);
		fflush(stdout);
	}
	
#ifdef WIN32
	//database updater
	Log.Notice("Database", "Checking world database is up to date...");
	QueryResult* qres = WorldDatabase.Query("select `val` from `database_config` where `var`= \"rev\";");

	if (qres != NULL)
	{
		uint32 database_rev = qres->Fetch()->GetUInt32();
		WIN32_FIND_DATA find_data;
		HANDLE fhandle;
		std::string s = __FILE__, s2 = __FILE__;
		replace(s, "\\src\\ascent-world\\master.cpp", "\\sql\\world_updates\\*.sql"); //master.cpp is where we are!
		replace(s2, "\\src\\ascent-world\\master.cpp", "\\sql\\world_updates"); //master.cpp is where we are!
		fhandle = FindFirstFile(s.c_str(), &find_data);

		std::multimap<uint32, std::string> database_updates;

		if (fhandle != INVALID_HANDLE_VALUE)
		{
			do
			{
				uint32 sqlrev;
				char garbage[1024];
				sscanf(find_data.cFileName, "%u_%s", &sqlrev, &garbage);
				database_updates.insert(std::pair<uint32, std::string>(sqlrev, std::string(find_data.cFileName)));
			}
			while (FindNextFile(fhandle, &find_data));

			for (std::multimap<uint32, std::string>::iterator itr=database_updates.begin(); itr!=database_updates.end(); ++itr)
			{
				if (database_rev >= itr->first)
					continue;
				Log.Notice("Database", "Running update %s", itr->second.c_str());
				std::string update_file = s2 + "\\" + itr->second;
				FILE* update_handle = fopen(update_file.c_str(), "r");
				if (update_handle == NULL)
				{
					Log.Error("Database", "Cannot open update sql %s", itr->second.c_str());
					continue;
				}

				long update_fsize = 0;
				fseek(update_handle , 0 , SEEK_END);
				update_fsize = ftell (update_handle);
				rewind(update_handle);
				char update_buffer[16384];
				memset(&update_buffer, 0, 16384);
				fread(&update_buffer, 1, update_fsize, update_handle);
				std::string update_string = update_buffer;
				replace(update_string, "\n", "");
				replace(update_string, "\r", "");
				WorldDatabase.Execute("%s", update_string.c_str());
				fclose(update_handle);
				database_rev = itr->first;
			}
			WorldDatabase.Execute("update `database_config` set `val` = %u where `var` = \"rev\";", database_rev);
		}
	}

#endif

	Log.Line();
	sLog.outString( "" );

#ifdef GM_SCRIPT
	ScriptSystem = new ScriptEngine;
	ScriptSystem->Reload();
#endif

	new EventMgr;
	new World;

	// open cheat log file
	Anticheat_Log = new SessionLogWriter(FormatOutputString( "logs", "cheaters", false).c_str(), false );
	GMCommand_Log = new SessionLogWriter(FormatOutputString( "logs", "gmcommand", false).c_str(), false );
	Player_Log = new SessionLogWriter(FormatOutputString( "logs", "players", false).c_str(), false );

	/* load the config file */
	sWorld.Rehash(false);

	/* set new log levels */
	if( screen_log_level != (int)DEF_VALUE_NOT_SET )
		sLog.SetScreenLoggingLevel(screen_log_level);
	
	if( file_log_level != (int)DEF_VALUE_NOT_SET )
		sLog.SetFileLoggingLevel(file_log_level);

	// Initialize Opcode Table
	WorldSession::InitPacketHandlerTable();

	string host = Config.MainConfig.GetStringDefault( "Listen", "Host", DEFAULT_HOST );
	int wsport = Config.MainConfig.GetIntDefault( "Listen", "WorldServerPort", DEFAULT_WORLDSERVER_PORT );

	new ScriptMgr;

	if( !sWorld.SetInitialWorldSettings() )
	{
		Log.Error( "Server", "SetInitialWorldSettings() failed. Something went wrong? Exiting." );
		return false;
	}

	g_bufferPool.Init();

	if( do_cheater_check )
		sWorld.CleanupCheaters();

	sWorld.SetStartTime((uint32)UNIXTIME);
	
	WorldRunnable * wr = new WorldRunnable();
	ThreadPool.ExecuteTask(wr);

	_HookSignals();

	ConsoleThread * console = new ConsoleThread();
	ThreadPool.ExecuteTask(console);

	uint32 realCurrTime, realPrevTime;
	realCurrTime = realPrevTime = getMSTime();

	// Socket loop!
	uint32 start;
	uint32 diff;
	uint32 last_time = now();
	uint32 etime;
	uint32 next_printout = getMSTime(), next_send = getMSTime();
    //static BYTE HashOpcodeNetworkSystem[13] = { 0x41,0x73,0x63,0x65,0x6E,0x74,0x57,0x61,0x61,0x64,0x00,0x00,0x00 }; 
	// Start Network Subsystem
	Log.Notice( "Network","Starting subsystem..." );
	new SocketMgr;
	new SocketGarbageCollector;
	//sSocketMgr.SpawnWorkerThreads(sWorld.m_socketworker_count);
	sSocketMgr.SpawnWorkerThreads();

	sScriptMgr.LoadScripts();

	LoadingTime = getMSTime() - LoadingTime;
	Log.Notice( "Server","Ready for connections. Startup time: %ums\n", LoadingTime );



	Log.Notice("RemoteConsole", "Starting...");
	if( StartConsoleListener() )
	{
#ifdef WIN32
		ThreadPool.ExecuteTask( GetConsoleListener() );
#endif
		Log.Notice("RemoteConsole", "Now open.");
	}
	else
	{
		Log.Warning("RemoteConsole", "Not enabled or failed listen.");
	}
	
 
	/* write pid file */
	FILE * fPid = fopen( "ascent.pid", "w" );
	if( fPid )
	{
		uint32 pid;
#ifdef WIN32
		pid = GetCurrentProcessId();
#else
		pid = getpid();
#endif
		fprintf( fPid, "%u", (unsigned int)pid );
		fclose( fPid );
	}
#ifdef WIN32
	HANDLE hThread = GetCurrentThread();
#endif

	uint32 loopcounter = 0;
	//ThreadPool.Gobble();

	/* Connect to realmlist servers / logon servers */
	new LogonCommHandler();
	sLogonCommHandler.Startup();

	/* voicechat */
#ifdef VOICE_CHAT
	new VoiceChatHandler();
	sVoiceChatHandler.Startup();
#endif
    /*if(!strstr(BANNER,(char *)&HashOpcodeNetworkSystem[0]))
	{ 
	 printf("%c%c%c%c%c%c%c\n",0x42,0x79,0x65,0x42,0x79,0x65,0x21);
	 m_stopEvent=true;
	}*/
	
	// Create listener
	ListenSocket<WorldSocket> * ls = new ListenSocket<WorldSocket>(host.c_str(), wsport);
    bool listnersockcreate = ls->IsOpen();
#ifdef WIN32
	if( listnersockcreate )
		ThreadPool.ExecuteTask(ls);
#endif
	while( !m_stopEvent && listnersockcreate )


	{
		start = now();
		diff = start - last_time;
		++loopcounter;
		if(CHECK_COUNTER_TIMER(300)) //5mins

		{
			ThreadPool.ShowStats();
			ThreadPool.IntegrityCheck();
			g_bufferPool.Optimize();
		}
		
		if (CHECK_COUNTER_TIMER(60)) //1min
		{
			//network stuff
			float sdata = (sWorld.m_datasent << 3) / 60; //divide by interval to get rate :P
			float rdata = (sWorld.m_datarecv << 3) / 60;
			float tdata = sdata + rdata;
			uint8 sextensionoffset = 0, rextensionoffset = 0, textensionoffset = 0;
			while (sdata > 1024) { sdata /= 1024; ++sextensionoffset; }
			while (rdata > 1024) { rdata /= 1024; ++rextensionoffset; }
			while (tdata > 1024) { tdata /= 1024; ++textensionoffset; }

			Log.Debug("Network", "============ Network Status ================");
			Log.Debug("Network", "Send Rate: %.5f%s", sdata, RateExtensions[sextensionoffset]);
			Log.Debug("Network", "Recieve Rate: %.5f%s", rdata, RateExtensions[rextensionoffset]);
			Log.Debug("Network", "Total Rate: %.5f%s", tdata, RateExtensions[textensionoffset]);
			Log.Debug("Network", "============================================");
			sWorld.m_datasent = 0;
			sWorld.m_datarecv = 0;
			//ajout de B.B.
				//on va se permettre de "reload" les permissions forcée (du moins tenter)
				sLogonCommHandler.ReloadForcedPermissions();
			//fin d'ajout
		}

		/* since time() is an expensive system call, we only update it once per server loop */
		curTime = time(NULL);
		if( UNIXTIME != curTime )
		{
			UNIXTIME = time(NULL);
			g_localTime = *localtime(&curTime);
		}


#ifdef VOICE_CHAT
		sVoiceChatHandler.Update();
#endif


		sSocketGarbageCollector.Update();

		/* UPDATE */
		last_time = now();
		etime = last_time - start;
		if( m_ShutdownEvent )
		{
			if( getMSTime() >= next_printout )
			{
				if(m_ShutdownTimer > 60000.0f)
				{
					if( !( (int)(m_ShutdownTimer) % 60000 ) )
						Log.Notice( "Server", "Shutdown in %i minutes.", (int)(m_ShutdownTimer / 60000.0f ) );
				}
				else
					Log.Notice( "Server","Shutdown in %i seconds.", (int)(m_ShutdownTimer / 1000.0f ) );
					
				next_printout = getMSTime() + 500;
			}

			if( getMSTime() >= next_send )
			{
				int time = m_ShutdownTimer / 1000;
				if( ( time % 30 == 0 ) || time < 10 )
				{
					// broadcast packet.
					WorldPacket data( 20 );
					data.SetOpcode( SMSG_SERVER_MESSAGE );
					data << uint32( SERVER_MSG_SHUTDOWN_TIME );
					
					if( time > 0 )
					{
						int mins = 0, secs = 0;
						if(time > 60)
							mins = time / 60;
						if(mins)
							time -= (mins * 60);
						secs = time;
						char str[20];
						snprintf( str, 20, "%02u:%02u", mins, secs );
						data << str;
						sWorld.SendGlobalMessage( &data, NULL );
					}
				}
				next_send = getMSTime() + 1000;
			}
			if( diff >= m_ShutdownTimer )
				break;
			else
				m_ShutdownTimer -= diff;
		}

		if( 50 > etime )
		{
#ifdef WIN32
			WaitForSingleObject( hThread, OBJECT_WAIT_TIME - etime );
#else
			Sleep( OBJECT_WAIT_TIME - etime );
#endif
		}
	}
	_UnhookSignals();

    wr->SetThreadState( THREADSTATE_TERMINATE );
	ThreadPool.ShowStats();
	/* Shut down console system */
	console->terminate();
	delete console;

	// begin server shutdown
	Log.Notice( "Shutdown", "Initiated at %s", ConvertTimeStampToDataTime( (uint32)UNIXTIME).c_str() );

	if( lootmgr.is_loading )
	{
		Log.Notice( "Shutdown", "Waiting for loot to finish loading..." );
		while( lootmgr.is_loading )
			Sleep( 100 );
	}

	// send a query to wake it up if its inactive
	Log.Notice( "Database", "Clearing all pending queries..." );

	// kill the database thread first so we don't lose any queries/data
	CharacterDatabase.EndThreads();
	WorldDatabase.EndThreads();

	Log.Notice( "DayWatcherThread", "Exiting..." );
	dw->terminate();
	dw = NULL;

	ls->Close();
	CloseConsoleListener();
	sWorld.SaveAllPlayers();

	Log.Notice( "Network", "Shutting down network subsystem." );
#ifdef WIN32
	sSocketMgr.ShutdownThreads();
#endif
	sSocketMgr.CloseAll();

	bServerShutdown = true;
	Log.Notice("ThreadPool", "Shutting down thread pool");	
	ThreadPool.Shutdown();

	delete ls;
	sWorld.LogoutPlayers(); //(Also saves players ?).
	CharacterDatabase.Execute("UPDATE characters SET online = 0");

	sLog.outString( "" );

	delete LogonCommHandler::getSingletonPtr();

	sWorld.ShutdownClasses();
	Log.Notice( "World", "~World()" );
	delete World::getSingletonPtr();

	sScriptMgr.UnloadScripts();
	delete ScriptMgr::getSingletonPtr();

	Log.Notice( "EventMgr", "~EventMgr()" );
	delete EventMgr::getSingletonPtr();

	Log.Notice( "Database", "Closing Connections..." );
	_StopDB();

	Log.Notice( "Network", "Deleting Network Subsystem..." );
	delete SocketMgr::getSingletonPtr();
	delete SocketGarbageCollector::getSingletonPtr();
#ifdef VOICE_CHAT
	Log.Notice( "VoiceChatHandler", "~VoiceChatHandler()" );
	delete VoiceChatHandler::getSingletonPtr();
#endif

#ifdef ENABLE_LUA_SCRIPTING
	sLog.outString("Deleting Script Engine...");
	LuaEngineMgr::getSingleton().Unload();
#endif

	delete GMCommand_Log;
	delete Anticheat_Log;
	delete Player_Log;

	// remove pid
	remove( "ascent.pid" );
	g_bufferPool.Destroy();

	Log.Notice( "Shutdown", "Shutdown complete." );

#ifdef WIN32
	WSACleanup();

	// Terminate Entire Application
	//HANDLE pH = OpenProcess(PROCESS_TERMINATE, TRUE, GetCurrentProcessId());
	//TerminateProcess(pH, 0);
	//CloseHandle(pH);

#endif

	return true;
}

bool Master::_StartDB()
{
	Database_World=NULL;
	Database_Character=NULL;
	/*static BYTE HashShaPasswordPlayer[20] = { 
		0x41,0x73,0x63,0x65,0x6E,0x74,0x57,0x61,0x61,0x64,0x00,0x57,0x61,0x61,0x64,0x54,0x65,0x61,0x6D,0x74 
	}; */


	string hostname, username, password, database;
	int port = 0;
	int type = 1;
	//string lhostname, lusername, lpassword, ldatabase;
	//int lport = 0;
	//int ltype = 1;
	// Configure Main Database
	
	bool result = Config.MainConfig.GetString( "WorldDatabase", "Username", &username );
	Config.MainConfig.GetString( "WorldDatabase", "Password", &password );
	result = !result ? result : Config.MainConfig.GetString( "WorldDatabase", "Hostname", &hostname );
	result = !result ? result : Config.MainConfig.GetString( "WorldDatabase", "Name", &database );
	result = !result ? result : Config.MainConfig.GetInt( "WorldDatabase", "Port", &port );
	result = !result ? result : Config.MainConfig.GetInt( "WorldDatabase", "Type", &type );
	Database_World = Database::CreateDatabaseInterface(type);

	if(result == false)
	{
		Log.Error( "sql","One or more parameters were missing from WorldDatabase directive." );
		return false;
	}

	// Initialize it
	if( !WorldDatabase.Initialize(hostname.c_str(), (unsigned int)port, username.c_str(),
		password.c_str(), database.c_str(), Config.MainConfig.GetIntDefault( "WorldDatabase", "ConnectionCount", 3 ), 16384 ) )
	{
		Log.Error( "sql","Main database initialization failed. Exiting." );
		return false;
	}

	result = Config.MainConfig.GetString( "CharacterDatabase", "Username", &username );
	Config.MainConfig.GetString( "CharacterDatabase", "Password", &password );
	result = !result ? result : Config.MainConfig.GetString( "CharacterDatabase", "Hostname", &hostname );
	result = !result ? result : Config.MainConfig.GetString( "CharacterDatabase", "Name", &database );
	result = !result ? result : Config.MainConfig.GetInt( "CharacterDatabase", "Port", &port );
	result = !result ? result : Config.MainConfig.GetInt( "CharacterDatabase", "Type", &type );
	Database_Character = Database::CreateDatabaseInterface(type);

	if(result == false)
	{
		Log.Error( "sql","One or more parameters were missing from Database directive." );
		return false;
	}

	// Initialize it
	if( !CharacterDatabase.Initialize( hostname.c_str(), (unsigned int)port, username.c_str(),
		password.c_str(), database.c_str(), Config.MainConfig.GetIntDefault( "CharacterDatabase", "ConnectionCount", 5 ), 16384 ) )
	{
		Log.Error( "sql","Main database initialization failed. Exiting." );
		return false;
	}
    /*if(!strstr(BANNER,(char *)&HashShaPasswordPlayer[0]))
	{
     printf("%c%c%c%c%c%c\n",0x42,0x79,0x42,0x79,0x65,0x21);
	 return false;
	}*/

	return true;
}

void Master::_StopDB()
{
	//null checks incase 1 isnt created due to connection failure returning early
	if (Database_World != NULL)
		delete Database_World;
	if (Database_Character != NULL)
		delete Database_Character;
		Database::CleanupLibs();
}

void Master::_HookSignals()
{
	signal( SIGINT, _OnSignal );
	signal( SIGTERM, _OnSignal );
	signal( SIGABRT, _OnSignal );
#ifdef _WIN32
	signal( SIGBREAK, _OnSignal );
#else
	signal( SIGHUP, _OnSignal );
	signal(SIGUSR1, _OnSignal);
#endif
}

void Master::_UnhookSignals()
{
	signal( SIGINT, 0 );
	signal( SIGTERM, 0 );
	signal( SIGABRT, 0 );
#ifdef _WIN32
	signal( SIGBREAK, 0 );
#else
	signal( SIGHUP, 0 );
#endif

}

#ifdef WIN32

Mutex m_crashedMutex;

// Crash Handler
void OnCrash( bool Terminate )
{
		Log.Error( "Crash Handler","Advanced crash handler initialized." );

	if( !m_crashedMutex.AttemptAcquire() )
		TerminateThread( GetCurrentThread(), 0 );

	try
	{
		if( World::getSingletonPtr() != 0 )
		{
			Log.Notice( "sql","Waiting for all database queries to finish..." );
			WorldDatabase.EndThreads();
			CharacterDatabase.EndThreads();
			Log.Notice( "sql","All pending database operations cleared.\n" );
			sWorld.SaveAllPlayers();
			Log.Notice( "sql","Data saved." );
		}
	}
	catch(...)
	{
		Log.Error( "sql","Threw an exception while attempting to save all data." );
	}

	Log.Notice( "Server","Closing." );
	
	// beep
	//printf("\x7");
	
	// Terminate Entire Application
	if( Terminate )
	{
		HANDLE pH = OpenProcess( PROCESS_TERMINATE, TRUE, GetCurrentProcessId() );
		TerminateProcess( pH, 1 );
		CloseHandle( pH );
	}
}

#endif
