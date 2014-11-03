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
#include "../ascent-shared/svn_revision.h"
#include <Console/CConsole.h>

#define BANNER "AscentWaad r%u/%s-%s-%s :: Realm Server\n"

#ifndef WIN32
#include <sched.h>
#endif

#include "svn_revision.h"

#include <signal.h>

createFileSingleton( Master );
bool crashed = false;

volatile bool Master::m_stopEvent = false;

// Database defines.
Database* Database_Character;
Database* Database_World;

void Master::_OnSignal(int s)
{
	switch (s)
	{
#ifndef WIN32
	case SIGHUP:
		Rehash(true);
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
static const char* default_config_file = "ascent-cluster.conf";
static const char* default_realm_config_file = "ascent-realms.conf";
#else
static const char* default_config_file = CONFDIR "/ascent-cluster.conf";
static const char* default_realm_config_file = CONFDIR "/ascent-realms.conf";
#endif

volatile bool bServerShutdown = false;

void Master::Rehash(bool load)
{
	ChatChannelDBC* pDBC= NULL;	

	if(load)
	{
		Config.ClusterConfig.SetSource( default_config_file );
	}

	if(!ChannelMgr::getSingletonPtr())
		new ChannelMgr;
		
	strncpy(RSDBCPath,Config.MainConfig.GetStringDefault("DataBaseClient", "DBCPath", "dbc").c_str(),MAX_PATH );
    if(strlen(RSDBCPath) >= MAX_PATH)
	{
		Log.Error("World.conf","Fatal: Le nom du répertoire des DBCs est trop long ! (%u caracteres max)",MAX_PATH-10);
		Log.Error("World.conf","       Retour au nom par defaut 'dbc'");
	    strcpy(RSDBCPath,"dbc");
	}
	
	channelmgr.seperatechannels = Config.ClusterConfig.GetBoolDefault("Server", "SeperateChatChannels", false);
	m_lfgForNonLfg = Config.ClusterConfig.GetBoolDefault("Server", "EnableLFGJoin", false);
	if(!Config.ClusterConfig.GetString("Server", "MainChannel", &mainchannel)) 
	{
		pDBC = dbcChatChannels.LookupEntryForced(CHANNEL_GENERAL);
		mainchannel = pDBC->pattern;
	}
	if(!Config.ClusterConfig.GetString("Server", "TradeChannel", &tradechannel)) 
	{
		pDBC = dbcChatChannels.LookupEntryForced(CHANNEL_COMMERCE);
		tradechannel = pDBC->pattern;
	}
	if(!Config.ClusterConfig.GetString("Server", "DefenseChannel", &defensechannel))
	{
		pDBC = dbcChatChannels.LookupEntryForced(CHANNEL_DEFENSE_LOCAL);
		defensechannel = pDBC->pattern;
	}
	if(!Config.ClusterConfig.GetString("Server", "DefenseUniverselChannel", &defunichannel))
	{
		pDBC = dbcChatChannels.LookupEntryForced(CHANNEL_DEFENSE_UNIVERSELLE);
		defunichannel = pDBC->pattern;
	}
	if(!Config.ClusterConfig.GetString("Server", "GuildChannel", &guildchannel))
	{
		pDBC = dbcChatChannels.LookupEntryForced(CHANNEL_RECRUTEMENT_GUILDE);
		guildchannel = pDBC->pattern;
	}
	if(!Config.ClusterConfig.GetString("Server", "LookingForChannel", &lookingforchannel))
	{
		pDBC = dbcChatChannels.LookupEntryForced(CHANNEL_RECHERCHE_GROUPE);
		lookingforchannel = pDBC->pattern;
	}
	if(!Config.ClusterConfig.GetString("Server", "CityChannel", &citychannel)) citychannel = "Capitales";
	if(!Config.ClusterConfig.GetString("GMClient", "GmClientChannel", &GmClientChannel))
	{
		GmClientChannel = "";
	}

	if( load )
		Channel::LoadConfSettings();
}

bool Master::Run(int argc, char ** argv)
{
	char * config_file = (char*)default_config_file;
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
		{ "checkconf",			ascent_no_argument,			&do_check_conf,			1		},
		{ "screenloglevel",		ascent_required_argument,		&screen_log_level,		1		},
		{ "fileloglevel",		ascent_required_argument,		&file_log_level,		-1		},
		{ "version",			ascent_no_argument,			&do_version,			1		},
		{ "conf",				ascent_required_argument,		NULL,					'c'		},
		{ "realmconf",			ascent_required_argument,		NULL,					'r'		},
		{ 0, 0, 0, 0 }
	};

	char c;
	while ((c = ascent_getopt_long_only(argc, argv, ":f:", longopts, NULL)) != -1)
	{
		switch (c)
		{
		case 'c':
			config_file = new char[strlen(ascent_optarg)];
			strcpy(config_file, ascent_optarg);
			break;

		case 'r':
			realm_config_file = new char[strlen(ascent_optarg)];
			strcpy(realm_config_file, ascent_optarg);
			break;

		case 0:
			break;
		default:
			sLog.m_fileLogLevel = -1;
			sLog.m_screenLogLevel = 3;
			printf("Usage: %s [--checkconf] [--conf <filename>] [--realmconf <filename>] [--version]\n", argv[0]);
			return true;
		}
	}
	/* set new log levels if used as argument*/
	if( screen_log_level != (int)DEF_VALUE_NOT_SET )
		sLog.SetScreenLoggingLevel(screen_log_level);

	if( file_log_level != (int)DEF_VALUE_NOT_SET )
		sLog.SetFileLoggingLevel(file_log_level);

	// Startup banner
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);

	/* Print Banner */
	Log.Notice("Server", "============================================================");
	Log.Notice("Server", "| AscentWaad Cluster System - Realm Serveur                 |");
	Log.Notice("Server", "| Version 1.0, Révision %04u                                |", BUILD_REVISION);
	Log.Notice("Server", "============================================================");
	Log.Line();

	if( do_check_conf )
	{
		Log.Notice( "Config", "Vérification du fichier de configuration : %s", config_file );
		if( Config.ClusterConfig.SetSource(config_file, true ) )
			Log.Success( "Config", "Chargement de la configuration avec succès." );
		else
			Log.Warning( "Config", "Le chargement de la configuration a rencontré une ou plusieurs erreurs." );

		Log.Notice( "Config", "Vérification du fichier de configuration : %s\n", realm_config_file );
		if( Config.RealmConfig.SetSource( realm_config_file, true ) )
			Log.Success( "Config", "Chargement de la configuration avec succès.\n" );
		else
			Log.Warning( "Config", "Le chargement de la configuration a rencontré une ou plusieurs erreurs.\n" );

		/* test for die variables */
		string die;
		if( Config.ClusterConfig.GetString( "die", "msg", &die) || Config.ClusterConfig.GetString("die2", "msg", &die ) )
			Log.Warning( "Config", "Die directive received: %s", die.c_str() );

		return true;
	}

	printf( "La combinaison des touches <Ctrl-C> permettra l'arrêt du serveur en toute sécurité à n'importe quel moment.\n" );
	Log.Line();

	//use these log_level until we are fully started up.
	sLog.Init(-1, 3);

#ifndef WIN32
	if(geteuid() == 0 || getegid() == 0)
		Log.LargeErrorMessage( LARGERRORMESSAGE_WARNING, "You are running Ascent as root.", "This is not needed, and may be a possible security risk.", "It is advised to hit CTRL+C now and", "start as a non-privileged user.", NULL);
#endif

	ThreadPool.Startup();
	uint32 LoadingTime = getMSTime();

	_HookSignals();

	Log.Line();

	Log.Notice( "Config", "Chargement des fichiers de configuration..." );
	if( Config.ClusterConfig.SetSource( config_file ) )
		Log.Success( "Config", ">> %s", config_file );
	else
	{
		Log.Error( "Config", ">> %s", config_file );
		return false;
	}

	string die;
	if( Config.ClusterConfig.GetString( "die", "msg", &die) || Config.ClusterConfig.GetString( "die2", "msg", &die ) )
	{
		Log.Warning( "Config", "Die directive received: %s", die.c_str() );
		return false;
	}	

	if(Config.RealmConfig.SetSource(realm_config_file))
		Log.Success( "Config", ">> %s", realm_config_file );
	else
	{
		Log.Error( "Config", ">> %s", realm_config_file );
		return false;
	}

	Rehash(true);

	if( !_StartDB() )
	{
		Database::CleanupLibs();
		ThreadPool.Shutdown();
		_UnhookSignals();
		return false;
	}
	Log.Success("Database", "Connexions établies...");

	new ClusterMgr;
	new ClientMgr;

	Log.Line();

	ThreadPool.ShowStats();
	Log.Line();
	
	Log.Notice( "Storage", "Chargement des fichers DBC..." );
	if( !LoadRSDBCs() )
	{
		Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "One or more of the DBC files are missing.", "These are absolutely necessary for the server to function.", "The server will not start without them.", NULL);
		return false;
	}

	Log.Success("Storage", "Fichiers DBC chargés...");
	//Storage_Load();
	TaskList tl;

	Storage_FillTaskList(tl);

	// spawn worker threads (2 * number of cpus)
	tl.spawn();

	tl.wait();

	Log.Line();

	new SocketMgr;
	new SocketGarbageCollector;
	sSocketMgr.SpawnWorkerThreads();

	/* connect to LS */
	new LogonCommHandler;
	sLogonCommHandler.Startup();

	Log.Success("Network", "Sous-système réseau démarré.");

	Log.Notice("Network", "Ouverture du port client...");
	ListenSocket<WorldSocket> * wsl = new ListenSocket<WorldSocket>("0.0.0.0", 8129);
	bool lsc = wsl->IsOpen();

	Log.Notice("Network", "Ouverture du port serveur...");
	ListenSocket<WSSocket> * isl = new ListenSocket<WSSocket>("0.0.0.0", 11010);
	bool ssc = isl->IsOpen();

	if(!lsc || !ssc)
	{
		Log.Error("Network", "Could not open one of the sockets.");
		return 1;
	}

	ThreadPool.ExecuteTask( isl );
	ThreadPool.ExecuteTask( wsl );

	ConsoleThread * console = new ConsoleThread();
	ThreadPool.ExecuteTask(console);

	uint32 realCurrTime, realPrevTime;
	realCurrTime = realPrevTime = getMSTime();

	LoadingTime = getMSTime() - LoadingTime;
	Log.Success("Server","Prêt à recevoir les connexions. Temps de démarrage: %ums\n", LoadingTime );

	m_startTime = uint32(UNIXTIME);

	//Update sLog to obey config setting
	sLog.Init(Config.ClusterConfig.GetIntDefault("LogLevel", "File", -1),Config.ClusterConfig.GetIntDefault("LogLevel", "Screen", 1));

	/* write pid file */
	FILE * fPid = fopen( "ascent-realmserver.pid", "w" );
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
	uint32 start = 0;
	uint32 diff = 0;
	uint32 last_time = 0;
	uint32 etime = 0;
	//ThreadPool.Gobble();

	/* voicechat */
#ifdef VOICE_CHAT
	new VoiceChatHandler();
	sVoiceChatHandler.Startup();
#endif

	while(!m_stopEvent)
	{
		start = now();
		diff = start - last_time;
		if(! ((++loopcounter) % 10000) )		// 5mins
		{
			ThreadPool.ShowStats();
			ThreadPool.IntegrityCheck();//Checks if THREAD_RESERVE is met
		}

			//ajout de B.B.
			//on va se permettre de "reload" les permissions forcée (du moins tenter)
				sLogonCommHandler.ReloadForcedPermissions();
			//fin d'ajout
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

		sLogonCommHandler.UpdateSockets();
		//wsl->Update();
		//isl->Update();
		sClientMgr.Update();
		sClusterMgr.Update();
		sSocketGarbageCollector.Update();

		/* UPDATE */
		last_time = now();
		etime = last_time - start;
		if( 50 > etime )
		{
#ifdef WIN32
			WaitForSingleObject( hThread, 50 - etime );
#else
			Sleep( 50 - etime );
#endif
		}
	}
	// begin server shutdown
	Log.Notice( "Fermeture", "Initialisée à %s", ConvertTimeStampToDataTime( (uint32)UNIXTIME).c_str() );
	bServerShutdown = true;
	
	_UnhookSignals();

	/* Shut down console system */
	console->terminate();
	delete console;

	Log.Notice("ChannelMgr", "~ChannelMgr()");
	delete ChannelMgr::getSingletonPtr();

	delete LogonCommHandler::getSingletonPtr();
	Log.Notice("~LogonComm", "Fermeture du LogonCommHandler");

	sSocketMgr.CloseAll();
#ifdef WIN32
	sSocketMgr.ShutdownThreads();
#endif

#ifdef VOICE_CHAT
	Log.Notice( "VoiceChatHandler", "~VoiceChatHandler()" );
	delete VoiceChatHandler::getSingletonPtr();
#endif

	Log.Notice( "~Network", "Suppression du sous-système réseau..." );
	delete SocketGarbageCollector::getSingletonPtr();
	delete SocketMgr::getSingletonPtr();

	Log.Notice("~Network", "Fermeture du port client...");
	delete wsl;

	Log.Notice("~Network", "Fermeture du port serveur...");
	delete isl;
	
	Log.Notice("~Network", "Fermeture du sous-système réseau.");
	
	Storage_Cleanup();
	Log.Notice("~Storage", "Fichiers DBC déchargés...");

	delete ClusterMgr::getSingletonPtr();
	delete ClientMgr::getSingletonPtr();

	CharacterDatabase.EndThreads();
	WorldDatabase.EndThreads();
	Database::CleanupLibs();
	Log.Notice( "Database", "Fermeture des connexions..." );
	_StopDB();
	Log.Notice("~Database", "Fermeture des bases de données terminée.");
	
	Log.Notice("ThreadPool", "Fermeture du thread pool");
	Task * tache = tl.GetTask();	
	while (tache)
	{
		tl.RemoveTask(tache);
	} 
	tl.kill();
	ThreadPool.Shutdown();
	
	// remove pid
	remove( "ascent-realmserver.pid" );

	Log.Success( "Shutdown", "Fermeture terminée." );

#ifdef WIN32
	WSACleanup();
#endif

	return true;
}

bool Master::_StartDB()
{
	Database_World=NULL;
	Database_Character=NULL;
	
	string hostname, username, password, database;
	int port = 0;
	// Configure Main Database

	bool result = Config.ClusterConfig.GetString( "WorldDatabase", "Username", &username );
	Config.ClusterConfig.GetString( "WorldDatabase", "Password", &password );
	result = !result ? result : Config.ClusterConfig.GetString( "WorldDatabase", "Hostname", &hostname );
	result = !result ? result : Config.ClusterConfig.GetString( "WorldDatabase", "Name", &database );
	result = !result ? result : Config.ClusterConfig.GetInt( "WorldDatabase", "Port", &port );
	Database_World = Database::CreateDatabaseInterface(1);

	if(result == false)
	{
		sLog.outDebug( "SQL: One or more parameters were missing from WorldDatabase directive." );
		return false;
	}

	// Initialize it
	if( !WorldDatabase.Initialize(hostname.c_str(), (unsigned int)port, username.c_str(),
		password.c_str(), database.c_str(), Config.ClusterConfig.GetIntDefault( "WorldDatabase", "ConnectionCount", 5 ), 16384 ) )
	{
		sLog.outDebug( "SQL: Main database initialization failed. Exiting." );
		return false;
	}

	result = Config.ClusterConfig.GetString( "CharacterDatabase", "Username", &username );
	Config.ClusterConfig.GetString( "CharacterDatabase", "Password", &password );
	result = !result ? result : Config.ClusterConfig.GetString( "CharacterDatabase", "Hostname", &hostname );
	result = !result ? result : Config.ClusterConfig.GetString( "CharacterDatabase", "Name", &database );
	result = !result ? result : Config.ClusterConfig.GetInt( "CharacterDatabase", "Port", &port );
	Database_Character = Database::CreateDatabaseInterface(1);

	if(result == false)
	{
		sLog.outDebug( "SQL: One or more parameters were missing from Database directive." );
		return false;
	}

	// Initialize it
	if( !CharacterDatabase.Initialize( hostname.c_str(), (unsigned int)port, username.c_str(),
		password.c_str(), database.c_str(), Config.ClusterConfig.GetIntDefault( "CharacterDatabase", "ConnectionCount", 5 ), 16384 ) )
	{
		sLog.outDebug( "SQL: Main database initialization failed. Exiting." );
		return false;
	}

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

#ifndef WIN32
// Unix crash handler :oOoOoOoOoOo
volatile bool m_crashed = false;
void segfault_handler(int c)
{
	if( m_crashed )
	{
		abort();
		return;		// not reached
	}

	m_crashed = true;

	printf ("Segfault handler entered...\n");
	try
	{
		if( World::getSingletonPtr() != 0 )
		{
			sLog.outString( "Waiting for all database queries to finish..." );
			WorldDatabase.EndThreads();
			CharacterDatabase.EndThreads();
			sLog.outString( "All pending database operations cleared.\n" );
			sWorld.SaveAllPlayers();
			sLog.outString( "Data saved." );
		}
	}
	catch(...)
	{
		sLog.outString( "Threw an exception while attempting to save all data." );
	}

	printf("Writing coredump...\n");
	abort();
}
#endif


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

	// crash handler
	signal(SIGSEGV, segfault_handler);
	signal(SIGFPE, segfault_handler);
	signal(SIGILL, segfault_handler);
	signal(SIGBUS, segfault_handler);
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
	sLog.outString( "Advanced crash handler initialized." );

	if( !m_crashedMutex.AttemptAcquire() )
		TerminateThread( GetCurrentThread(), 0 );

	try
	{
		if( ClusterMgr::getSingletonPtr() != 0 )
		{
			sLog.outString( "Waiting for all database queries to finish..." );
			WorldDatabase.EndThreads();
			CharacterDatabase.EndThreads();
			sLog.outString( "All pending database operations cleared.\n" );
			//sClusterMgr.SaveAllPlayers();
			//sLog.outString( "Data saved." );
		}
	}
	catch(...)
	{
		sLog.outString( "Threw an exception while attempting to save all data." );
	}

	sLog.outString( "Closing." );

	// Terminate Entire Application
	if( Terminate )
	{
		HANDLE pH = OpenProcess( PROCESS_TERMINATE, TRUE, GetCurrentProcessId() );
		TerminateProcess( pH, 1 );
		CloseHandle( pH );
	}
}

#endif

void TaskList::AddTask(Task * task)
{
	queueLock.Acquire();
	tasks.insert(task);
	queueLock.Release();
}

Task * TaskList::GetTask()
{
	queueLock.Acquire();

	Task* t = 0;
	for(set<Task*>::iterator itr = tasks.begin(); itr != tasks.end(); itr++)
	{
		if(!(*itr)->in_progress)
		{
			t = (*itr);
			t->in_progress = true;
			break;
		}
	}
	queueLock.Release();
	return t;
}

void TaskList::spawn()
{
	running = true;
	thread_count = 0;

	uint32 threadcount;
	if(Config.MainConfig.GetBoolDefault("Startup", "EnableMultithreadedLoading", true))
	{
		// get processor count
#ifndef WIN32
#if UNIX_FLAVOUR == UNIX_FLAVOUR_LINUX
#ifdef X64
		threadcount = 2;
#else
		long affmask;
		sched_getaffinity(0, 4, (cpu_set_t*)&affmask);
		threadcount = (BitCount8(affmask)) * 2;
		if(threadcount > 8) threadcount = 8;
		else if(threadcount <= 0) threadcount = 1;
#endif
#else
		threadcount = 2;
#endif
#else
		SYSTEM_INFO s;
		GetSystemInfo(&s);
		threadcount = s.dwNumberOfProcessors * 2;
		if(threadcount > 8)
			threadcount = 8;
#endif
	}
	else
		threadcount = 1;

	Log.Notice("World", "Beginning %s server startup with %u thread(s).", (threadcount == 1) ? "progressive" : "parallel", threadcount);

	for(uint32 x = 0; x < threadcount; ++x)
		ThreadPool.ExecuteTask(new TaskExecutor(this));
}

void TaskList::wait()
{
	bool has_tasks = true;
	time_t t;
	while(has_tasks)
	{
		queueLock.Acquire();
		has_tasks = false;
		for(set<Task*>::iterator itr = tasks.begin(); itr != tasks.end(); itr++)
		{
			if(!(*itr)->completed)
			{
				has_tasks = true;
				break;
			}
		}
		queueLock.Release();

		// keep updating time lol
		t = time(NULL);
		if( UNIXTIME != t )
		{
			UNIXTIME = t;
			g_localTime = *localtime(&t);
		}

		Sleep(20);
	}
}

void TaskList::kill()
{
	running = false;
}

void Task::execute()
{
	_cb->execute();
}

bool TaskExecutor::run()
{
	Task * t;
	while(starter->running)
	{
		t = starter->GetTask();
		if(t)
		{
			t->execute();
			t->completed = true;
			starter->RemoveTask(t);
			delete t;
		}
		else
			Sleep(20);
	}
	return true;
}

void TaskList::waitForThreadsToExit()
{
	while(thread_count)
	{
		Sleep(20);
	}
}