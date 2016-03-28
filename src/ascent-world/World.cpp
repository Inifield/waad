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

initialiseSingleton( World );

DayWatcherThread* dw = NULL;

float World::m_movementCompressThreshold;
float World::m_movementCompressThresholdCreatures;
uint32 World::m_movementCompressRate;
uint32 World::m_movementCompressInterval;
float World::m_speedHackThreshold;
float World::m_wallhackthreshold;
float World::m_speedHackLatencyMultiplier;
uint32 World::m_speedHackResetInterval;
uint32 World::m_CEThreshold;

World::World()
{
	m_socketworker_count = 0;
	m_nagleswitch = 0;
	m_playerLimit = 0;
	m_allowMovement = true;
	m_gmTicketSystem = true;

	GmClientChannel = "";

	m_StartTime = 0;
	eventholder = new EventableObjectHolder(-1);
	m_holder = eventholder;
	m_event_Instanceid = eventholder->GetInstanceID();

	mQueueUpdateInterval = 10000;
	PeakSessionCount = 0;
	mInWorldPlayerCount = 0;
	mAcceptedConnections = 0;
	HordePlayers = 0;
	AlliancePlayers = 0;
	gm_skip_attunement = false;
	CheckProfessions = true;
	show_gm_in_who_list = true;
	//allow_gm_friends = true;
	map_unload_time=0;

	SocketSendBufSize = WORLDSOCKET_SENDBUF_SIZE;
	SocketRecvBufSize = WORLDSOCKET_RECVBUF_SIZE;
	m_startLevel=1;
	m_levelCap=250;
	//m_genLevelCap=250; // finalement il sert a rien celui la!
	m_limitedNames=false;
	m_banTable = NULL;
	m_speedHackThreshold = -500.0f;
	m_speedHackLatencyMultiplier = 0.0f;
	m_speedHackResetInterval = 5000;
	m_CEThreshold = 10000;
	m_forceGMTag = false;
	m_showKick = true;

	// BGs Configuration
	m_WarsongsBG_Open = false;
    m_WarsongsBG_Min  = 5;
    m_ArathiBG_Open = false;;
    m_ArathiBG_Min  = 5;
    m_NetherStormBG_Open = false;;
    m_NetherStormBG_Min = 5;
    m_AlteracValleyBG_Open = false;;
    m_AlteracValleyBG_Min = 5;
    m_SotaBG_Open = false;;
    m_SotaBG_Min = 5;
}

void CleanupRandomNumberGenerators();
void World::LogoutPlayers()
{
	Log.Notice("World", "Logging out players...");
	for(SessionMap::iterator i=m_sessions.begin();i!=m_sessions.end();i++)
	{
		(i->second)->LogoutPlayer(true);
	}

	Log.Notice("World", "Deleting sessions...");
	WorldSession * Session;
	for(SessionMap::iterator i=m_sessions.begin();i!=m_sessions.end();)
	{
		Session = i->second;
		++i;

		DeleteSession(Session);
		//delete p;
	}
}

World::~World()
{
	Log.Notice("LocalizationMgr", "~LocalizationMgr()");
	sLocalizationMgr.Shutdown();

	Log.Notice("WorldLog", "~WorldLog()");
	WorldLog::DestroySingleton();

	Log.Notice("ChatHandler", "~ChatHandler()");
	ChatHandler::DestroySingleton();

	Log.Notice("ObjectMgr", "~ObjectMgr()");
	ObjectMgr::DestroySingleton();
	
	Log.Notice("LootMgr", "~LootMgr()");
	LootMgr::DestroySingleton();
	
	Log.Notice("LfgMgr", "~LfgMgr()");
	LfgMgr::DestroySingleton();

	Log.Notice("ChannelMgr", "~ChannelMgr()");
	ChannelMgr::DestroySingleton();
	Log.Notice("QuestMgr", "~QuestMgr()");
	QuestMgr::DestroySingleton();
  
	Log.Notice("WeatherMgr", "~WeatherMgr()");
	WeatherMgr::DestroySingleton();

	Log.Notice("TaxiMgr", "~TaxiMgr()");
	TaxiMgr::DestroySingleton();
	
	Log.Notice("BattlegroundMgr", "~BattlegroundMgr()");
	CBattlegroundManager::DestroySingleton();

	Log.Notice("InstanceMgr", "~InstanceMgr()");
	sInstanceMgr.Shutdown();

	// Ajout cinemania : Suppression du WorldAnnounceMgr
	Log.Notice("WorldAnnounceMgr", "~WorldAnnounceMgr()");
	WorldAnnounceMgr::DestroySingleton();
	WorldAnnounceStorage::DestroySingleton();

	//sLog.outString("Deleting Thread Manager..");
	//delete ThreadMgr::getSingletonPtr();
	Log.Notice("WordFilter", "~WordFilter()");
	delete g_chatFilter;
	delete g_characterNameFilter;

	Log.Notice("Rnd", "~Rnd()");
	CleanupRandomNumberGenerators();

	for( AreaTriggerMap::iterator i = m_AreaTriggerMap.begin( ); i != m_AreaTriggerMap.end( ); ++ i ) 
	{
		delete i->second;
	}

	//eventholder = 0;
	delete eventholder;

	Log.Notice("GMSecurityMgr", "~GMSecurityMgr()");
	Storage_Cleanup();
	GMSecurityMgr::DestroySingleton();

	if(sWorld.m_IrcUseChatIrc) Log.Notice("WaadIrcRelay", "~WaadIrcRelay()"); // Juste le message
	WaadIrcRelay::DestroySingleton();

	for(list<SpellEntry*>::iterator itr = dummyspells.begin(); itr != dummyspells.end(); ++itr)
		delete *itr;
}


WorldSession* World::FindSession(uint32 id)
{
	m_sessionlock.AcquireReadLock();
	WorldSession * ret = 0;
	SessionMap::const_iterator itr = m_sessions.find(id);

	if(itr != m_sessions.end())
		ret = itr->second;
	
	m_sessionlock.ReleaseReadLock();

	return ret;
}

void World::RemoveSession(uint32 id)
{
	m_sessionlock.AcquireWriteLock();

	SessionMap::iterator itr = m_sessions.find(id);

	if(itr != m_sessions.end())
	{
		delete itr->second;
		m_sessions.erase(itr);
	}

	m_sessionlock.ReleaseWriteLock();
}

void World::AddSession(WorldSession* s)
{
	if(!s)
	{
		sLog.outError("AddSession NULL!");
		return;
	}

	Log.Debug("AddSession","%s",s->GetAccountName().c_str());

	m_sessionlock.AcquireWriteLock();

	//ASSERT(s);
	m_sessions[s->GetAccountId()] = s;

	if(m_sessions.size() >  PeakSessionCount)
		PeakSessionCount = (uint32)m_sessions.size();

   // ArcEmu, 322 
   WorldPacket data(SMSG_ACCOUNT_DATA_TIMES, 41);
   data << uint32( UNIXTIME );  // unix time of something
   data << uint8(1);
   data << uint32(GLOBAL_CACHE_MASK);
   for(uint32 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
   {
	if(GLOBAL_CACHE_MASK & (1 << i))
	{
	 data << uint32( 0 );
	}
   }
   s->SendPacket(&data);

	m_sessionlock.ReleaseWriteLock();
}

void World::AddGlobalSession(WorldSession *session)
{
	if(!session)
		return;

	SessionsMutex.Acquire();
	Sessions.insert(session);
	SessionsMutex.Release();
}

void World::RemoveGlobalSession(WorldSession *session)
{
	SessionsMutex.Acquire();
	Sessions.erase(session);
	SessionsMutex.Release();
}

bool BasicTaskExecutor::run()
{
	/* Set thread priority, this is a bitch for multiplatform :P */
#ifdef WIN32
	switch(priority)
	{
		case BTE_PRIORITY_LOW:
			::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_LOWEST );
			break;

		case BTW_PRIORITY_HIGH:
			::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL );
			break;

		default:		// BTW_PRIORITY_MED
			::SetThreadPriority( ::GetCurrentThread(), THREAD_PRIORITY_NORMAL );
			break;
	}
#else
	struct sched_param param;
	switch(priority)
	{
	case BTE_PRIORITY_LOW:
		param.sched_priority = 0;
		break;

	case BTW_PRIORITY_HIGH:
		param.sched_priority = 10;
		break;

	default:		// BTW_PRIORITY_MED
		param.sched_priority = 5;
		break;
	}
	pthread_setschedparam(pthread_self(), SCHED_OTHER, &param);
#endif

	// Execute the task in our new context.
	cb->execute();
#ifdef WIN32
	::SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
#else
	param.sched_priority = 5;
	pthread_setschedparam(pthread_self(), SCHED_OTHER, &param);
#endif

	return true;
}

//void Apply112SpellFixes();
void ApplyExtraDataFixes();
void ApplyNormalFixes();
void ApplySpellUniques();

void PreStartQueries()
{
	QueryResult * result;

	result = WorldDatabase.Query("SELECT * FROM prestartqueries ORDER BY seq ASC");
	if(result)
	{
		Log.Notice("DataBase","Found and executing %u prestart queries on World tables.",result->GetRowCount());
		do
		{
			Field * f = result->Fetch();
			string e_query =  f[0].GetString();
			WorldDatabase.Execute(e_query.c_str());
		}while(result->NextRow());

		delete result;
		WorldDatabase.Execute("DELETE FROM prestartqueries WHERE SingleShot = 1;");
	}

	result = CharacterDatabase.Query("SELECT * FROM prestartqueries ORDER BY seq ASC");
	if(result)
	{
		Log.Notice("DataBase","Found and executing %u prestart queries on Character tables.",result->GetRowCount());
		do
		{
			Field * f = result->Fetch();
			string e_query =  f[0].GetString();
			CharacterDatabase.Execute(e_query.c_str());
		}while(result->NextRow());

		delete result;
		CharacterDatabase.Execute("DELETE FROM prestartqueries WHERE SingleShot = 1;");
	}
}

bool World::SetInitialWorldSettings()
{
	//Perform pre-starting queries on World- and Character-DataBase
	PreStartQueries();

	char _bufftemp_[1024];

	Log.Line();
	Player::InitVisibleUpdateBits();

	CharacterDatabase.WaitExecute("UPDATE characters SET online = 0 WHERE online = 1");
	//CharacterDatabase.WaitExecute("UPDATE characters SET level = 70 WHERE level > 70");
	CharacterDatabase.WaitExecute("UPDATE characters SET banned=0,banReason='' WHERE banned > 100 AND banned < %u", UNIXTIME);
   
	m_lastTick = UNIXTIME;

	// TODO: clean this
	time_t tiempo;
	char hour[3];
	char minute[3];
	char second[3];
	struct tm *tmPtr;
	tiempo = UNIXTIME;
	tmPtr = localtime(&tiempo);
	strftime( hour, 3, "%H", tmPtr );
	strftime( minute, 3, "%M", tmPtr );
	strftime( second, 3, "%S", tmPtr );
	m_gameTime = (3600*atoi(hour))+(atoi(minute)*60)+(atoi(second)); // server starts at noon

	// TODO: clean this
	// fill in emotes table
	// it appears not every emote has an animation
	mPrices[1] = 10;
	mPrices[4] = 80;
	mPrices[6] = 150;
	mPrices[8] = 200;
	mPrices[10] = 300;
	mPrices[12] = 800;
	mPrices[14] = 900;
	mPrices[16] = 1800;
	mPrices[18] = 2200;
	mPrices[20] = 2300;
	mPrices[22] = 3600;
	mPrices[24] = 4200;
	mPrices[26] = 6700;
	mPrices[28] = 7200;
	mPrices[30] = 8000;
	mPrices[32] = 11000;
	mPrices[34] = 14000;
	mPrices[36] = 16000;
	mPrices[38] = 18000;
	mPrices[40] = 20000;
	mPrices[42] = 27000;
	mPrices[44] = 32000;
	mPrices[46] = 37000;
	mPrices[48] = 42000;
	mPrices[50] = 47000;
	mPrices[52] = 52000;
	mPrices[54] = 57000;
	mPrices[56] = 62000;
	mPrices[58] = 67000;
	mPrices[60] = 72000;

	// Start

	uint32 start_time = getMSTime();

	Log.Notice( "World", "Loading DBC files..." );
	if( !LoadDBCs() )
	{
		Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "One or more of the DBC files are missing.", "These are absolutely necessary for the server to function.", "The server will not start without them.", NULL);
		return false;
	}

	/* Convert area table ids/flags */
	DBCFile area;

	sprintf(_bufftemp_,"%s/AreaTable.dbc",DBCPath);
	if( !area.open( _bufftemp_ ) )
	{
		Log.Error( "World", "Cannot find file %s",_bufftemp_ );
		return false;
	}
    Log.Notice("DBC:","Loading %s",_bufftemp_);
	uint32 flag_, area_, zone_;
	for(uint32 i = 0; i < area.getRecordCount(); ++i)
	{
		area_ = area.getRecord(i).getUInt(0);
		flag_ = area.getRecord(i).getUInt(3);
		zone_ = area.getRecord(i).getUInt(2);

		mAreaIDToTable[flag_] = dbcArea.LookupEntryForced(area_);
		if(mZoneIDToTable.find(zone_) != mZoneIDToTable.end())
		{
			if(mZoneIDToTable[zone_]->AreaFlags != 312 &&
				mAreaIDToTable[flag_]->AreaFlags == 312)
			{
				// over ride.
				mZoneIDToTable[zone_] = mAreaIDToTable[flag_];
			}
		}
		else
		{
			mZoneIDToTable[zone_] = mAreaIDToTable[flag_];
		}
	}
	ApplyNormalFixes();

	new ObjectMgr;
	new QuestMgr;
	new LootMgr;
	new LfgMgr;
	new WeatherMgr;
	new TaxiMgr;
	new AddonMgr;
	new WorldLog;
	new ChatHandler;
	new GMSecurityMgr;

	// grep: this only has to be done once between version updates
	// to re-fill the table.

	/*sLog.outString("Filling spell replacements table...");
	FillSpellReplacementsTable();
	sLog.outString("");*/

#define MAKE_TASK(sp, ptr) tl.AddTask(new Task(new CallbackP0<sp>(sp::getSingletonPtr(), &sp::ptr)))
	// Fill the task list with jobs to do.
	TaskList tl;
	Storage_FillTaskList(tl);

	// spawn worker threads (2 * number of cpus)
	tl.spawn();

	/* storage stuff has to be loaded first */
	tl.wait();

	Storage_LoadAdditionalTables();

	MAKE_TASK(ObjectMgr, LoadPlayerCreateInfo);
	MAKE_TASK(ObjectMgr, LoadPlayersInfo);
	tl.wait();

	MAKE_TASK(ObjectMgr, LoadAchievements);
	if(Config.MainConfig.GetBoolDefault("Startup", "BackgroundWaypointLoading", false))
	{
		ThreadPool.ExecuteTask(new BasicTaskExecutor(new CallbackP0<ObjectMgr>(ObjectMgr::getSingletonPtr(), &ObjectMgr::LoadCreatureWaypoints), 
			BTE_PRIORITY_MED));
	}
	else
	{
		MAKE_TASK(ObjectMgr, LoadCreatureWaypoints);
	}
	MAKE_TASK(ObjectMgr, LoadTrainers);
	MAKE_TASK(ObjectMgr, LoadTotemSpells);
	MAKE_TASK(ObjectMgr, LoadSpellSkills);
	MAKE_TASK(ObjectMgr, LoadSpellOverride);
	MAKE_TASK(ObjectMgr, LoadVendors);
	MAKE_TASK(ObjectMgr, LoadAIThreatToSpellId);
	// MAKE_TASK(ObjectMgr, LoadSpellFixes);
	MAKE_TASK(ObjectMgr, LoadSpellProcs);
	MAKE_TASK(ObjectMgr, LoadSpellEffectsOverride);
	MAKE_TASK(ObjectMgr, LoadDefaultPetSpells);
	MAKE_TASK(ObjectMgr, LoadPetSpellCooldowns);
	MAKE_TASK(ObjectMgr, LoadGuildCharters);
	MAKE_TASK(ObjectMgr, LoadGMTickets);
	MAKE_TASK(ObjectMgr, LoadPetLevelupSpellMap);
	MAKE_TASK(AddonMgr, LoadFromDB);
	MAKE_TASK(ObjectMgr, SetHighestGuids);
	MAKE_TASK(ObjectMgr, LoadReputationModifiers);
	MAKE_TASK(ObjectMgr, LoadMonsterSay);
	MAKE_TASK(WeatherMgr, LoadFromDB);
	MAKE_TASK(ObjectMgr,LoadGroups);

	MAKE_TASK(ObjectMgr, LoadExtraCreatureProtoStuff);
	MAKE_TASK(ObjectMgr, LoadExtraItemStuff);
	MAKE_TASK(QuestMgr, LoadExtraQuestStuff);
	MAKE_TASK(ObjectMgr, LoadPointsOfInterest);
	MAKE_TASK(ObjectMgr, LoadQuestPOI);
	MAKE_TASK(ObjectMgr, LoadArenaTeams);

#undef MAKE_TASK

	// wait for all loading to complete.
	tl.wait();
	sLocalizationMgr.Reload(false);

	Log.Notice("GMSecurityMgr", "Loading...");
	CommandTableStorage::getSingleton().Load();
	Log.Notice("GMSecurityMgr", "LoadTableCommand...");
	sGMSecurityMgr.LoadConf();
	Log.Notice("GMSecurityMgr", "LoadConf...");
	sGMSecurityMgr.Init();
	Log.Notice("GMSecurityMgr", "Init Done.");
	
	Log.Notice("WordFilter", "Loading...");
	g_characterNameFilter = new WordFilter();
	g_chatFilter = new WordFilter();
	g_characterNameFilter->Load("wordfilter_character_names");
	g_chatFilter->Load("wordfilter_chat");

	Log.Notice("WordFilter", "Done.");

	sLog.outString("");
	Log.Notice("World", "Database loaded in %ums.", getMSTime() - start_time);
	sLog.outString("");

#ifdef COLLISION
	CollideInterface.Init();
#endif

	// calling this puts all maps into our task list.
	sInstanceMgr.Load(&tl);

	// wait for the events to complete.
	tl.wait();

	// wait for them to exit, now.
	tl.kill();
	tl.waitForThreadsToExit();
	sLog.outString("");
	LoadNameGenData();

	Log.Notice("World", "Object size: %u bytes", sizeof(Object));
	Log.Notice("World", "Unit size: %u bytes", sizeof(Unit) + sizeof(AIInterface));
	Log.Notice("World", "Creature size: %u bytes", sizeof(Creature) + sizeof(AIInterface));
	Log.Notice("World", "Player size: %u bytes", sizeof(Player) + sizeof(ItemInterface) + 50000 + 30000 + 1000 + sizeof(AIInterface));
	Log.Notice("World", "GameObject size: %u bytes", sizeof(GameObject));
	Log.Notice("World", "Vehicle size: %u bytes", sizeof(Vehicle) + sizeof(AIInterface));
	

// ------------------------------------------------------------------------------------------------

	Log.Notice("World","Starting Transport System...");
	objmgr.LoadTransporters();

	// start mail system
	Log.Notice("World","Starting Mail System...");
	MailSystem::getSingleton().StartMailSystem();

	Log.Notice("World", "Starting Auction System...");
	new AuctionMgr;
	sAuctionMgr.LoadAuctionHouses();

	m_queueUpdateTimer = mQueueUpdateInterval;
	if(Config.MainConfig.GetBoolDefault("Startup", "BackgroundLootLoading", true))
	{
		Log.Notice("World", "Backgrounding loot loading...");

		// loot background loading in a lower priority thread.
		ThreadPool.ExecuteTask(new BasicTaskExecutor(new CallbackP0<LootMgr>(LootMgr::getSingletonPtr(), &LootMgr::LoadLoot), 
			BTE_PRIORITY_LOW));
	}
	else
	{
		Log.Notice("World", "Loading loot in foreground...");
		lootmgr.LoadLoot();
	}

	Log.Notice("World", "Loading Channel config...");
	Channel::LoadConfSettings();
	// Maitres des Arenes
	m_ArenaSeasonOnTime = Config.MainConfig.GetIntDefault("ArenaSeason", "SeasonOnTime", 0);
	m_ArenaSeasonNumber = Config.MainConfig.GetIntDefault("ArenaSeason", "SeasonNumber", 1);
	string MasterSubNameArena;
	if(Config.MainConfig.GetString("ArenaSeason", "MasterSubNameContent", &MasterSubNameArena)) 
		strncpy(m_ArenaSubnameContent,MasterSubNameArena.c_str(),75);
	else                               
		strcpy(m_ArenaSubnameContent,"Arenes");
	
	// Debug (Branruz)
	//Log.Notice("*** Debug_Arene","Maitre des [%s]",m_ArenaSubnameContent);

	Log.Notice("Arenes", "Saison %u %s",m_ArenaSeasonNumber,(m_ArenaSeasonOnTime) ? "Ouverte" : "Terminee");
	Log.Notice("BattlegroundManager", "Starting...");
	CBattlegroundManager* BattlegroundMgr(new CBattlegroundManager);
	BattlegroundMgr->Init();

	if(m_WarsongsBG_Min > 0)
	 Log.Notice( "BattleGround", "Goulet des ChanteGuerres: %s (%d vs %d joueurs)",
		(m_WarsongsBG_Open) ? "Ouvert" : "Indisponible",m_WarsongsBG_Min,m_WarsongsBG_Min);
	else
		Log.Notice( "BattleGround", "Goulet des ChanteGuerres: %s ,(Entree libre)",
		   (m_WarsongsBG_Open) ? "Ouvert" : "Indisponible");

	if(m_ArathiBG_Min > 0)
	 Log.Notice( "BattleGround", "Bassin d'Arathi: %s , (%d vs %d joueurs)",
		(m_ArathiBG_Open) ? "Ouvert" : "Indisponible",m_ArathiBG_Min,m_ArathiBG_Min);
	else
		Log.Notice( "BattleGround", "Bassin d'Arathi: %s ,(Entree libre)",
		   (m_ArathiBG_Open) ? "Ouvert" : "Indisponible");

    if(m_AlteracValleyBG_Min > 0)
	 Log.Notice( "BattleGround", "Vallee d'Alterac: %s , (%d vs %d joueurs)",
		(m_AlteracValleyBG_Open) ? "Ouvert" : "Indisponible",m_AlteracValleyBG_Min,m_AlteracValleyBG_Min);
	else
		Log.Notice( "BattleGround", "Vallee d'Alterac: %s ,(Entree libre)",
		   (m_AlteracValleyBG_Open) ? "Ouvert" : "Indisponible");

	if(m_NetherStormBG_Min > 0)
	 Log.Notice( "BattleGround", "L'Oeil du Cyclone: %s , (%d vs %d joueurs)",
		(m_NetherStormBG_Open) ? "Ouvert" : "Indisponible",m_NetherStormBG_Min,m_NetherStormBG_Min);
	else
		Log.Notice( "BattleGround", "L'Oeil du Cyclone: %s ,(Entree libre)",
		   (m_NetherStormBG_Open) ? "Ouvert" : "Indisponible");

	if(m_SotaBG_Min > 0)
	 Log.Notice( "BattleGround", "Le rivage des Anciens: %s , %d vs %d joueurs minimum",
		(m_SotaBG_Open) ? "Ouvert" : "Indisponible",m_SotaBG_Min,m_SotaBG_Min);
	else
		Log.Notice( "BattleGround", "Le rivage des Anciens: %s ,(Entree libre)",
		   (m_SotaBG_Open) ? "Ouvert" : "Indisponible");

	// Debug (Branruz)
	//Log.Notice("*** Debug_BG","Maitre de guerre [%s]",m_WarSongSubnameContent);
	//Log.Notice("*** Debug_BG","Maitre de guerre [%s]",m_ArathiSubnameContent);
	//Log.Notice("*** Debug_BG","Maitre de guerre [%s]",m_NetherStormSubnameContent);
	//Log.Notice("*** Debug_BG","Maitre de guerre [%s]",m_AlteracSubnameContent);
	//Log.Notice("*** Debug_BG","Maitre de guerre [%s]",m_SotaSubnameContent);

    //** Lecture des zones de Debug (Branruz)
	m_DebugTransporters = Config.MainConfig.GetIntDefault("DebugArea", "TransporterGoId", 0);
	m_DebugPlayers = Config.MainConfig.GetIntDefault("DebugArea", "Debug_Player", 0);
    if(m_DebugTransporters) Log.Warning("[DebugArea]", "Suivi console: Transporter %u",m_DebugTransporters);
	if(m_DebugPlayers)      Log.Warning("[DebugArea]", "Suivi console: Suivi des joueurs");

	//** zone WaadFun
    m_WarriorPetAllowed = Config.MainConfig.GetIntDefault("WaadFun", "WarriorPetAllowed", 0);
	m_JdRPetListenning  = Config.MainConfig.GetIntDefault("WaadFun", "JdRPetListenning", 0);
	if(m_WarriorPetAllowed) Log.Success("[WaadFun]","Fun: Les familers pour Guerriers sont autorises.");
	if(m_JdRPetListenning)  Log.Success("[WaadFun]","JdR: Les familers sont a l'ecoute en fonction de leur loyaute.");

	//** Miscellaneous
    m_Latin2UTF8 = Config.MainConfig.GetIntDefault("Miscellaneous", "Latin2UTF8", 0);
	Log.Success("Miscellaneous","Conversion Latin1 vers UTF8 : %s.",(m_Latin2UTF8) ? "Oui":"Non");

	//** Relay Irc
	m_IrcUseChatIrc     = Config.MainConfig.GetIntDefault("RelayChatIrc", "UseChatIrc"    , 0);     
    m_IrcServPort       = Config.MainConfig.GetIntDefault("RelayChatIrc", "ServPort"      , 6667);
	m_IrcPlayerConnect  = Config.MainConfig.GetIntDefault("RelayChatIrc", "PlayerConnect" , 1);
	m_IrcChannelWisp    = Config.MainConfig.GetIntDefault("RelayChatIrc", "ChannelWisp"   , 1);
	m_IrcChannelGuilde  = Config.MainConfig.GetIntDefault("RelayChatIrc", "ChannelGuilde" , 1);
	m_IrcChannelGeneral = Config.MainConfig.GetIntDefault("RelayChatIrc", "ChannelGeneral", 1);

	if(!Config.MainConfig.GetString("RelayChatIrc", "ServHostIp", &m_IrcServHostIp)) m_IrcServHostIp = "127.0.0.1";
    if(!Config.MainConfig.GetString("RelayChatIrc", "Chan"      , &m_IrcChan))       m_IrcChan    = "#chat_to_join";
    if(!Config.MainConfig.GetString("RelayChatIrc", "BotNick"   , &m_IrcBotNick))    m_IrcBotNick = "BotFromMyWowServer";
    if(!Config.MainConfig.GetString("RelayChatIrc", "BotName"   , &m_IrcBotName))    m_IrcBotName = "WaadIrcServerRelay";
    if(!Config.MainConfig.GetString("RelayChatIrc", "BotText"   , &m_IrcBotText))    m_IrcBotText = "Waad World Server Relay actif.";
	if(m_IrcUseChatIrc) Log.Success("[WaadIrcRelay]","Le relay Irc est utilisé.");

	//------------------------------

	Log.Notice("WorldAnnounceManager", "Starting...");
	new WorldAnnounceMgr;
	new WorldAnnounceStorage;
	sWorldAnnounceMgr.LoadConfig();

	dw = new DayWatcherThread();
	ThreadPool.ExecuteTask( dw );

	ThreadPool.ExecuteTask( new CharacterLoaderThread() );

#ifdef ENABLE_COMPRESSED_MOVEMENT
	Log.Notice("World", "Starting MovementCompressorThread...");
	MovementCompressor = new CMovementCompressorThread();
	ThreadPool.ExecuteTask( MovementCompressor );
#endif

	// Preload and compile talent and talent tab data to speed up talent inspect

	uint32 talent_max_rank;
	uint32 talent_pos;
	uint32 talent_class;

    for( uint32 i = 0; i < dbcTalent.GetNumRows(); ++i )
    {
        TalentEntry const* talent_info = dbcTalent.LookupRow( i );
		if( talent_info == NULL )
			continue;

		TalentTabEntry const* tab_info = dbcTalentTab.LookupEntry( talent_info->TalentTree );
		if( tab_info == NULL )
			continue;

        talent_max_rank = 0;
        for( uint32 j = 5; j > 0; --j )
        {
            if( talent_info->RankID[j - 1] )
            {
                talent_max_rank = j;
                break;
            }
		}

		InspectTalentTabBit[( talent_info->Row << 24 ) + ( talent_info->Col << 16 ) + talent_info->TalentID] = talent_max_rank;
		InspectTalentTabSize[talent_info->TalentTree] += talent_max_rank;
	}

	for( uint32 i = 0; i < dbcTalentTab.GetNumRows(); ++i )
	{
		TalentTabEntry const* tab_info = dbcTalentTab.LookupRow( i );
		if( tab_info == NULL )
			continue;

		talent_pos = 0;
        
		for( talent_class = 0; talent_class < 12; ++talent_class )
		{
			if( tab_info->ClassMask & ( 1 << talent_class ) )
				break;
		}

		InspectTalentTabPages[talent_class + 1][tab_info->TabPage] = tab_info->TalentTabID;

		for( std::map< uint32, uint32 >::iterator itr = InspectTalentTabBit.begin(); itr != InspectTalentTabBit.end(); ++itr )
		{
			uint32 talent_id = itr->first & 0xFFFF;
			TalentEntry const* talent_info = dbcTalent.LookupEntry( talent_id );
			if( talent_info == NULL )
				continue;

			if( talent_info->TalentTree != tab_info->TalentTabID )
				continue;

			InspectTalentTabPos[talent_id] = talent_pos;
			talent_pos += itr->second;
		}
	}

	sEventMgr.AddEvent(this, &World::CheckForExpiredInstances, EVENT_WORLD_UPDATEAUCTIONS, 120000, 0, 0);
	return true;
}

void World::Update(time_t diff)
{
	eventholder->Update((uint32)diff);
	sAuctionMgr.Update();
	_UpdateGameTime();
	UpdateQueuedSessions((uint32)diff);
#ifdef SESSION_CAP
	if( GetSessionCount() >= SESSION_CAP )
		TerminateProcess(GetCurrentProcess(),0);
#endif
}


void World::SendGlobalMessage(WorldPacket *packet, WorldSession *self)
{
	m_sessionlock.AcquireReadLock();

	SessionMap::iterator itr;
	for (itr = m_sessions.begin(); itr != m_sessions.end(); itr++)
	{
		if (itr->second->GetPlayer() &&
			itr->second->GetPlayer()->IsInWorld()
			&& itr->second != self)  // dont send to self!
		{
			itr->second->SendPacket(packet);
		}
	}

	m_sessionlock.ReleaseReadLock();
}
void World::SendGamemasterMessage(WorldPacket *packet, WorldSession *self)
{
 	m_sessionlock.AcquireReadLock();
 	SessionMap::iterator itr;
 	for(itr = m_sessions.begin(); itr != m_sessions.end(); itr++)
 	{
	  if (itr->second->GetPlayer() &&
	  itr->second->GetPlayer()->IsInWorld()
	  && itr->second != self)  // dont send to self!
	  {
		if(sGMSecurityMgr.UseGamemasterAnnounce(itr->second->GetPermissions()))
 		itr->second->SendPacket(packet);
	  }
 	}
 	m_sessionlock.ReleaseReadLock();
}

void World::SendFactionMessage(WorldPacket *packet, uint8 teamId)
{
	m_sessionlock.AcquireReadLock();
	SessionMap::iterator itr;
	Player * plr;
	for(itr = m_sessions.begin(); itr != m_sessions.end(); itr++)
	{
		plr = itr->second->GetPlayer();
		if(!plr || !plr->IsInWorld())
			continue;

		if(plr->GetTeam() == teamId)
			itr->second->SendPacket(packet);
	}
	m_sessionlock.ReleaseReadLock();
}

void World::SendZoneMessage(WorldPacket *packet, uint32 zoneid, WorldSession *self)
{
	m_sessionlock.AcquireReadLock();

	SessionMap::iterator itr;
	for (itr = m_sessions.begin(); itr != m_sessions.end(); itr++)
	{
		if (itr->second->GetPlayer() &&
			itr->second->GetPlayer()->IsInWorld()
			&& itr->second != self)  // dont send to self!
		{
			if (itr->second->GetPlayer()->GetZoneId() == zoneid)
				itr->second->SendPacket(packet);
		}
	}

	m_sessionlock.ReleaseReadLock();
}

void World::SendGMWorldText(const char* text, WorldSession *self)
{
	char buffer_text[512];
	strncpy(buffer_text,ChangeLatin2UTF8(text),500);

    uint32 textLen = (uint32)(strlen(buffer_text) + 1);

    WorldPacket data(textLen + 40);

	data.Initialize(SMSG_MESSAGECHAT);
	data << uint8(CHAT_MSG_SYSTEM);
	data << uint32(LANG_UNIVERSAL);
	
	data << (uint64)0;
	data << (uint32)0;
	data << (uint64)0;

	data << textLen;
	data << &buffer_text[0];
	data << uint8(0);
	SendGamemasterMessage(&data, self);
}

void World::SendInstanceMessage(WorldPacket *packet, uint32 instanceid, WorldSession *self)
{
	m_sessionlock.AcquireReadLock();

	SessionMap::iterator itr;
	for (itr = m_sessions.begin(); itr != m_sessions.end(); itr++)
	{
		if (itr->second->GetPlayer() &&
			itr->second->GetPlayer()->IsInWorld()
			&& itr->second != self)  // dont send to self!
		{
			if (itr->second->GetPlayer()->GetInstanceID() == (int32)instanceid)
				itr->second->SendPacket(packet);
		}
	}

	m_sessionlock.ReleaseReadLock();
}

void World::SendWorldText(const char* text, WorldSession *self)
{
	char buffer_text[512];
	strncpy(buffer_text,ChangeLatin2UTF8(text),500);

    uint32 textLen = (uint32)(strlen(buffer_text) + 1);

    WorldPacket data(textLen + 40);

	data.Initialize(SMSG_MESSAGECHAT);
	data << uint8(CHAT_MSG_SYSTEM);
	data << uint32(LANG_UNIVERSAL);
	
	data << (uint64)0; // Who cares about guid when there's no nickname displayed heh ?
	data << (uint32)0;
	data << (uint64)0;

	data << textLen;
	data << &buffer_text[0];
	data << uint8(0);

	SendGlobalMessage(&data, self);
	if(announce_output)
	sLog.outString("> %s", text);
}

void World::SendWorldWideScreenText(const char *text, WorldSession *self)
{
	WorldPacket data(256);
	data.Initialize(SMSG_AREA_TRIGGER_MESSAGE);
	data << (uint32)0 << text << (uint8)0x00;
	SendGlobalMessage(&data, self);
	if(announce_output)
	sLog.outString("> %s", text);
}

void World::UpdateSessions(uint32 diff)
{
	SessionSet::iterator itr, it2;
	WorldSession *GlobalSession;
	int result;
	SessionsMutex.Acquire();
	for(itr = Sessions.begin(); itr != Sessions.end();)
	{
		GlobalSession = (*itr);
		it2 = itr;
		++itr;
		//We have been moved to mapmgr, remove us here.
		if( GlobalSession->GetInstance() != 0 )
		{
			Sessions.erase(it2);
			continue;
		}
		result = GlobalSession->Update(0);
		if(result)
		{
			if(result == 1)//socket don't exist anymore, delete from worldsessions.
				DeleteSession(GlobalSession);

			//We have been (re-)moved to mapmgr, remove us here.
			Sessions.erase(it2);
		}
	}
	SessionsMutex.Release();
}

std::string World::GenerateName(uint32 type)
{
	if(_namegendata[type].size() == 0)
		return "ERR";

	uint32 ent = RandomUInt((uint32)_namegendata[type].size()-1);
	return _namegendata[type].at(ent).name;
}

void World::DeleteSession(WorldSession *session)
{
	m_sessionlock.AcquireWriteLock();
	// remove from big map
	m_sessions.erase(session->GetAccountId());

	m_sessionlock.ReleaseWriteLock();

	// delete us
	session->Delete();
}

uint32 World::GetNonGmSessionCount()
{
	m_sessionlock.AcquireReadLock();

	uint32 total = (uint32)m_sessions.size();

	SessionMap::const_iterator itr = m_sessions.begin();
	for( ; itr != m_sessions.end(); itr++ )
	{
		if( (itr->second)->HasGMPermissions() )
			total--;
	}

	m_sessionlock.ReleaseReadLock();

	return total;
}

uint32 World::AddQueuedSocket(WorldSocket* Socket)
{
	// Since we have multiple socket threads, better guard for this one,
	// we don't want heap corruption ;)
	queueMutex.Acquire();

	// Add socket to list
	mQueuedSessions.push_back(Socket);
	queueMutex.Release();
	// Return queue position
	return (uint32)mQueuedSessions.size();
}

void World::RemoveQueuedSocket(WorldSocket* Socket)
{
	// Since we have multiple socket threads, better guard for this one,
	// we don't want heap corruption ;)
	queueMutex.Acquire();

	// Find socket in list
	QueueSet::iterator iter = mQueuedSessions.begin();
	for(; iter != mQueuedSessions.end(); ++iter)
	{
		if((*iter) == Socket)
		{
			// Remove from the queue and abort.
			// This will be slow (Removing from middle of a vector!) but it won't
			// get called very much, so it's not really a big deal.

			mQueuedSessions.erase(iter);
			queueMutex.Release();
			return;
		}
	}
	queueMutex.Release();
}

uint32 World::GetQueuePos(WorldSocket* Socket)
{
	// Since we have multiple socket threads, better guard for this one,
	// we don't want heap corruption ;)
	queueMutex.Acquire();

	// Find socket in list
	QueueSet::iterator iter = mQueuedSessions.begin();
	uint32 QueuePos = 1;
	for(; iter != mQueuedSessions.end(); ++iter, ++QueuePos)
	{
		if((*iter) == Socket)
		{
			queueMutex.Release();
			// Return our queue position.
			return QueuePos;
		}
	}
	queueMutex.Release();
	// We shouldn't get here..
	return 1;
}

void World::UpdateQueuedSessions(uint32 diff)
{

	if(diff >= m_queueUpdateTimer) 
	{
		m_queueUpdateTimer = mQueueUpdateInterval;
		queueMutex.Acquire();

		if(mQueuedSessions.size() == 0)
		{
			queueMutex.Release();
			return;
		}
		
		while(m_sessions.size() < m_playerLimit && mQueuedSessions.size())
		{
			// Yay. We can let another player in now.
			// Grab the first fucker from the queue, but guard of course, since
			// this is in a different thread again.

			QueueSet::iterator iter = mQueuedSessions.begin();
			WorldSocket * QueuedSocket = *iter;
			mQueuedSessions.erase(iter);

			// Welcome, sucker.
			if(QueuedSocket->GetSession())
			{
				QueuedSocket->GetSession()->deleteMutex.Acquire();
				QueuedSocket->Authenticate();
				QueuedSocket->GetSession()->deleteMutex.Release();
			}
		}

		if(mQueuedSessions.size() == 0)
		{
			queueMutex.Release();
			return;
		}

		// Update the remaining queue members.
		QueueSet::iterator iter = mQueuedSessions.begin();
		uint32 Position = 1;
		while(iter != mQueuedSessions.end())
		{
			WorldSocket *tmpSocket = *iter;
			++iter;
			tmpSocket->UpdateQueuePosition(Position++);
		}
		queueMutex.Release();
	} 
	else 
	{
		m_queueUpdateTimer -= diff;
	}

}

void World::SaveAllPlayers()
{
	if(!(ObjectMgr::getSingletonPtr()))
		return;

	sLog.outString("Saving all players to database...");
	uint32 count = 0;
	PlayerStorageMap::const_iterator itr;
		// Servers started and obviously runing. lets save all players.
	uint32 mt;
	objmgr._playerslock.AcquireReadLock();   
	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
		{
			if(itr->second->GetSession())
			{
				mt = getMSTime();
				itr->second->SaveToDB(false);
				sLog.outString("Saved player `%s` (level %u) in %ums.", itr->second->GetName(), itr->second->GetUInt32Value(UNIT_FIELD_LEVEL), getMSTime() - mt);
				++count;
			}
		}
	objmgr._playerslock.ReleaseReadLock();
	sLog.outString("Saved %u players.", count);
}

WorldSession* World::FindSessionByName(const char * Name)//case insensetive
{
	m_sessionlock.AcquireReadLock();

	// loop sessions, see if we can find him
	SessionMap::iterator itr = m_sessions.begin();
	for(; itr != m_sessions.end(); ++itr)
	{
		if(!stricmp(itr->second->GetAccountName().c_str(),Name))
		{
			m_sessionlock.ReleaseReadLock();
			return itr->second;
		}
	}
	m_sessionlock.ReleaseReadLock();
	return 0;
}

void World::ShutdownClasses()
{
	Log.Notice("AddonMgr", "~AddonMgr()");
	sAddonMgr.SaveToDB();
	delete AddonMgr::getSingletonPtr();

	Log.Notice("AuctionMgr", "~AuctionMgr()");
	delete AuctionMgr::getSingletonPtr();
	Log.Notice("LootMgr", "~LootMgr()");
	delete LootMgr::getSingletonPtr();

	Log.Notice("MailSystem", "~MailSystem()");
	delete MailSystem::getSingletonPtr();
}

void World::GetStats(uint32 * GMCount, float * AverageLatency)
{
	int gm = 0;
	int count = 0;
	int avg = 0;
	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
	{
		if(itr->second->GetSession())
		{
			count++;
			avg += itr->second->GetSession()->GetLatency();
			if(itr->second->GetSession()->HasPermissions())
				gm++;
		}			
	}
	objmgr._playerslock.ReleaseReadLock();

	*AverageLatency = count ? (float)((float)avg / (float)count) : 0;
	*GMCount = gm;
}

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
	for(set<Task*>::iterator itr = tasks.begin(); itr != tasks.end(); ++itr)
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
		if(threadcount>8)
			threadcount=8;
#endif
	}
	else
		threadcount = 1;

	Log.Line();
	Log.Notice("World", "Beginning %s server startup with %u threads.", (threadcount == 1) ? "progressive" : "parallel", threadcount);
	Log.Line();

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
		for(set<Task*>::iterator itr = tasks.begin(); itr != tasks.end(); ++itr)
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
	THREAD_TRY_EXECUTION
	{
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
	}
	THREAD_HANDLE_CRASH
	return true;
}

void TaskList::waitForThreadsToExit()
{
	while(thread_count)
	{
		Sleep(20);
	}
}

void World::DeleteObject(Object * obj)
{
	delete obj;
}

void World::Rehash(bool load)
{
	if(load)
	{
		#ifdef WIN32
		Config.MainConfig.SetSource("ascent-world.conf", true);
		#else
		Config.MainConfig.SetSource((char*)CONFDIR "/ascent-world.conf", true);
		#endif
	}

	if(!ChannelMgr::getSingletonPtr())
		new ChannelMgr;
	if(!MailSystem::getSingletonPtr())
		new MailSystem;

	channelmgr.seperatechannels = Config.MainConfig.GetBoolDefault("Server", "SeperateChatChannels", false);
	MapPath = Config.MainConfig.GetStringDefault("Terrain", "MapPath", "maps");
	vMapPath = Config.MainConfig.GetStringDefault("Terrain", "vMapPath", "vmaps");
	UnloadMapFiles = Config.MainConfig.GetBoolDefault("Terrain", "UnloadMapFiles", true);
	strncpy(DBCPath,Config.MainConfig.GetStringDefault("DataBaseClient", "DBCPath", "dbc").c_str(),MAX_PATH );
    if(strlen(DBCPath) >= MAX_PATH)
	{
		Log.Error("World.conf","Fatal: Le repertoire des DBCs est trop long! (%u caracteres max)",MAX_PATH-10);
		Log.Error("World.conf","       Retour au nom par defaut 'dbc'");
	    strcpy(DBCPath,"dbc");
	}
	BreathingEnabled = Config.MainConfig.GetBoolDefault("Server", "EnableBreathing", true);
	SendStatsOnJoin = Config.MainConfig.GetBoolDefault("Server", "SendStatsOnJoin", true);
	compression_threshold = Config.MainConfig.GetIntDefault("Server", "CompressionThreshold", 1000);

	// load regeneration rates.
	setRate(RATE_HEALTH,Config.MainConfig.GetFloatDefault("Rates", "Health",1));
	setRate(RATE_POWER1,Config.MainConfig.GetFloatDefault("Rates", "Power1",1));
	setRate(RATE_POWER2,Config.MainConfig.GetFloatDefault("Rates", "Power2",1));
	setRate(RATE_POWER3,Config.MainConfig.GetFloatDefault("Rates", "Power3",1));
	setRate(RATE_POWER4,Config.MainConfig.GetFloatDefault("Rates", "Power4",1));	
	setRate(RATE_DROP0,Config.MainConfig.GetFloatDefault("Rates", "DropGrey",1));
    setRate(RATE_DROP1,Config.MainConfig.GetFloatDefault("Rates", "DropWhite",1));
    setRate(RATE_DROP2,Config.MainConfig.GetFloatDefault("Rates", "DropGreen",1));
    setRate(RATE_DROP3,Config.MainConfig.GetFloatDefault("Rates", "DropBlue",1));
    setRate(RATE_DROP4,Config.MainConfig.GetFloatDefault("Rates", "DropPurple",1));
    setRate(RATE_DROP5,Config.MainConfig.GetFloatDefault("Rates", "DropOrange",1));
    setRate(RATE_DROP6,Config.MainConfig.GetFloatDefault("Rates", "DropArtifact",1));
	setRate(RATE_XP,Config.MainConfig.GetFloatDefault("Rates", "XP",1));
	setRate(RATE_RESTXP,Config.MainConfig.GetFloatDefault("Rates", "RestXP", 1));
	setRate(RATE_QUESTXP,Config.MainConfig.GetFloatDefault("Rates", "QuestXP", 1));
	setIntRate(INTRATE_SAVE, Config.MainConfig.GetIntDefault("Rates", "Save", 1));
	setRate(RATE_MONEY, Config.MainConfig.GetFloatDefault("Rates", "DropMoney", 1.0f));
	setRate(RATE_QUESTREPUTATION, Config.MainConfig.GetFloatDefault("Rates", "QuestReputation", 1.0f));
	setRate(RATE_KILLREPUTATION, Config.MainConfig.GetFloatDefault("Rates", "KillReputation", 1.0f));
	setRate(RATE_HONOR, Config.MainConfig.GetFloatDefault("Rates", "Honor", 1.0f));
	setRate(RATE_SKILLCHANCE, Config.MainConfig.GetFloatDefault("Rates", "SkillChance", 1.0f));
	setRate(RATE_SKILLRATE, Config.MainConfig.GetFloatDefault("Rates", "SkillRate", 1.0f));
	setIntRate(INTRATE_COMPRESSION, Config.MainConfig.GetIntDefault("Rates", "Compression", 1));
	setIntRate(INTRATE_PVPTIMER, Config.MainConfig.GetIntDefault("Rates", "PvPTimer", 300000));
	setRate(RATE_ARENAPOINTMULTIPLIER2X, Config.MainConfig.GetFloatDefault("Rates", "ArenaMultiplier2x", 1.0f));
	setRate(RATE_ARENAPOINTMULTIPLIER3X, Config.MainConfig.GetFloatDefault("Rates", "ArenaMultiplier3x", 1.0f));
	setRate(RATE_ARENAPOINTMULTIPLIER5X, Config.MainConfig.GetFloatDefault("Rates", "ArenaMultiplier5x", 1.0f));
	SetPlayerLimit(Config.MainConfig.GetIntDefault("Server", "PlayerLimit", 1000));
	SetMotd(Config.MainConfig.GetStringDefault("Server", "Motd", "Ascent Default MOTD").c_str());
	mQueueUpdateInterval = Config.MainConfig.GetIntDefault("Server", "QueueUpdateInterval", 5000);
	SetKickAFKPlayerTime(Config.MainConfig.GetIntDefault("Server", "KickAFKPlayers", 0));
	sLog.SetScreenLoggingLevel(Config.MainConfig.GetIntDefault("LogLevel", "Screen", 1));
	sLog.SetFileLoggingLevel(Config.MainConfig.GetIntDefault("LogLevel", "File", -1));
	Log.log_level = Config.MainConfig.GetIntDefault("LogLevel", "Screen", 1);
	gm_skip_attunement = Config.MainConfig.GetBoolDefault("Server", "SkipAttunementsForGM", true);	
	CheckProfessions = Config.MainConfig.GetBoolDefault("Server", "CheckProfessions", false);
	ServerPreloading = Config.MainConfig.GetBoolDefault("Startup", "Preloading", false);

	SocketRecvBufSize = Config.MainConfig.GetIntDefault("WorldSocket", "RecvBufSize", WORLDSOCKET_RECVBUF_SIZE);
	SocketSendBufSize = Config.MainConfig.GetIntDefault("WorldSocket", "SendBufSize", WORLDSOCKET_SENDBUF_SIZE);
    // Recuperation de la config BG (Branruz)
	string MasterSubNameBG;
	// Goulet
	m_WarsongsBG_Open = Config.MainConfig.GetBoolDefault("BattleGround_Warsongs", "State", true);
    m_WarsongsBG_Min  = Config.MainConfig.GetIntDefault("BattleGround_Warsongs", "Min_Player", 5);
	if(Config.MainConfig.GetString("BattleGround_Warsongs", "MasterSubNameContent", &MasterSubNameBG)) 
		strncpy(m_WarSongSubnameContent,MasterSubNameBG.c_str(),75);
	else                               
		strcpy(m_WarSongSubnameContent,"Chanteguerres");
	// Arathi
	m_ArathiBG_Open = Config.MainConfig.GetBoolDefault("BattleGround_Arathi", "State", true);
    m_ArathiBG_Min  = Config.MainConfig.GetIntDefault("BattleGround_Arathi", "Min_Player", 5);
	if(Config.MainConfig.GetString("BattleGround_Arathi", "MasterSubNameContent", &MasterSubNameBG)) 
		strncpy(m_ArathiSubnameContent,MasterSubNameBG.c_str(),75);
	else                               
		strcpy(m_ArathiSubnameContent,"Arathi");
	// Oeil du cyclone
    m_NetherStormBG_Open = Config.MainConfig.GetBoolDefault("BattleGround_NetherStorm", "State", true);
    m_NetherStormBG_Min  = Config.MainConfig.GetIntDefault("BattleGround_NetherStorm", "Min_Player", 5);   
	if(Config.MainConfig.GetString("BattleGround_NetherStorm", "MasterSubNameContent", &MasterSubNameBG)) 
		strncpy(m_NetherStormSubnameContent,MasterSubNameBG.c_str(),75);
	else                               
		strcpy(m_NetherStormSubnameContent,"Oeil du cyclone");
	// Alterac
    m_AlteracValleyBG_Open = Config.MainConfig.GetBoolDefault("BattleGround_Alterac", "State", true);
    m_AlteracValleyBG_Min  = Config.MainConfig.GetIntDefault("BattleGround_Alterac", "Min_Player", 5);
	if(Config.MainConfig.GetString("BattleGround_Alterac", "MasterSubNameContent", &MasterSubNameBG)) 
		strncpy(m_AlteracSubnameContent,MasterSubNameBG.c_str(),75);
	else                               
		strcpy(m_AlteracSubnameContent,"Alterac");
	// Rivage des Anciens
	m_SotaBG_Open = Config.MainConfig.GetBoolDefault("BattleGround_Sota", "State", false);
    m_SotaBG_Min  = Config.MainConfig.GetIntDefault("BattleGround_Sota", "Min_Player", 5);
	if(Config.MainConfig.GetString("BattleGround_Sota", "MasterSubNameContent", &MasterSubNameBG)) 
		strncpy(m_SotaSubnameContent,MasterSubNameBG.c_str(),75);
	else                               
		strcpy(m_SotaSubnameContent,"rivage des Anciens");
	
    // Verification console!
    // Ajout Branruz, Parametrage des Bgs via Ascent-world.conf et Banzaiiiiii!!!!! ;)
   	InitPointeurBGs();
    //--------------------------------------------------------------------------

	bool log_enabled = Config.MainConfig.GetBoolDefault("Log", "Cheaters", false);
	if(Anticheat_Log->IsOpen())
	{
		if(!log_enabled)
			Anticheat_Log->Close();
	}
	else
		if(log_enabled)
			Anticheat_Log->Open();

	log_enabled = Config.MainConfig.GetBoolDefault("Log", "GMCommands", false);
	if(GMCommand_Log->IsOpen())
	{
		if(!log_enabled)
			GMCommand_Log->Close();
	}
	else
		if(log_enabled)
			GMCommand_Log->Open();

	log_enabled = Config.MainConfig.GetBoolDefault("Log", "Player", false);
	if(Player_Log->IsOpen())
	{
		if(!log_enabled)
			Player_Log->Close();
	}
	else
	{
		if(log_enabled)
			Player_Log->Open();
	}

#ifdef WIN32
	DWORD current_priority_class = GetPriorityClass( GetCurrentProcess() );
	bool high = Config.MainConfig.GetBoolDefault( "Server", "AdjustPriority", false );

	if( high )
	{
		if( current_priority_class != HIGH_PRIORITY_CLASS )
			SetPriorityClass( GetCurrentProcess(), HIGH_PRIORITY_CLASS );
	}
	else
	{
		if( current_priority_class != NORMAL_PRIORITY_CLASS )
			SetPriorityClass( GetCurrentProcess(), NORMAL_PRIORITY_CLASS );
	}
#endif

	if(!Config.MainConfig.GetString("GMClient", "GmClientChannel", &GmClientChannel))
	{
		GmClientChannel = "";
	}

	m_reqGmForCommands = !Config.MainConfig.GetBoolDefault("Server", "AllowPlayerCommands", false);
	m_lfgForNonLfg = Config.MainConfig.GetBoolDefault("Server", "EnableLFGJoin", false);

	realmtype = Config.MainConfig.GetBoolDefault("Server", "RealmType", false);
	TimeOut= uint32(1000* Config.MainConfig.GetIntDefault("Server", "ConnectionTimeout", 180) );

	uint32 config_flags = 0;
	if(Config.MainConfig.GetBoolDefault("Mail", "DisablePostageCostsForGM", true))
		config_flags |= MAIL_FLAG_NO_COST_FOR_GM;

	if(Config.MainConfig.GetBoolDefault("Mail", "DisablePostageCosts", false))
		config_flags |= MAIL_FLAG_DISABLE_POSTAGE_COSTS;

	if(Config.MainConfig.GetBoolDefault("Mail", "DisablePostageDelayItems", true))
		config_flags |= MAIL_FLAG_DISABLE_HOUR_DELAY_FOR_ITEMS;

	if(Config.MainConfig.GetBoolDefault("Mail", "DisableMessageExpiry", false))
		config_flags |= MAIL_FLAG_NO_EXPIRY;

	if(Config.MainConfig.GetBoolDefault("Mail", "EnableInterfactionMail", true))
		config_flags |= MAIL_FLAG_CAN_SEND_TO_OPPOSITE_FACTION;

	if(Config.MainConfig.GetBoolDefault("Mail", "EnableInterfactionForGM", true))
		config_flags |= MAIL_FLAG_CAN_SEND_TO_OPPOSITE_FACTION_GM;

	sMailSystem.config_flags = config_flags;
	flood_lines = Config.MainConfig.GetIntDefault("FloodProtection", "Lines", 0);
	flood_seconds = Config.MainConfig.GetIntDefault("FloodProtection", "Seconds", 0);
	flood_message = Config.MainConfig.GetBoolDefault("FloodProtection", "SendMessage", false);
	show_gm_in_who_list = Config.MainConfig.GetBoolDefault("Server", "ShowGMInWhoList", true);
	//allow_gm_friends = Config.MainConfig.GetBoolDefault("Server", "AllowGMFriends", false);
	if(!flood_lines || !flood_seconds)
		flood_lines = flood_seconds = 0;

	announce_tag = Config.MainConfig.GetStringDefault("Announce", "Tag", "Staff");
	GMAdminTag = Config.MainConfig.GetBoolDefault("Announce", "GMAdminTag", false);
	NameinAnnounce = Config.MainConfig.GetBoolDefault("Announce", "NameinAnnounce", true);
	NameinWAnnounce = Config.MainConfig.GetBoolDefault("Announce", "NameinWAnnounce", true);
	announce_output = Config.MainConfig.GetBoolDefault("Announce", "ShowInConsole", true);
	ann_namecolor = Config.MainConfig.GetStringDefault("Color", "AnnNameColor", "|c1f40af20");
	ann_gmtagcolor = Config.MainConfig.GetStringDefault("Color", "AnnGMTagColor", "|cffff6060");
	ann_tagcolor = Config.MainConfig.GetStringDefault("Color", "AnnTagColor", "|cff00ccff");
	ann_msgcolor = Config.MainConfig.GetStringDefault("Color", "AnnMsgColor", "|cffffcc00");

	map_unload_time=Config.MainConfig.GetIntDefault("Server", "MapUnloadTime", 0);

	antihack_teleport = Config.MainConfig.GetBoolDefault("AntiHack", "Teleport", true);
	antihack_speed = Config.MainConfig.GetBoolDefault("AntiHack", "Speed", true);
	antihack_flight = Config.MainConfig.GetBoolDefault("AntiHack", "Flight", true);
	flyhack_threshold = Config.MainConfig.GetIntDefault("AntiHack", "FlightThreshold", 8);
	no_antihack_on_gm = Config.MainConfig.GetBoolDefault("AntiHack", "DisableOnGM", false);
	m_speedHackThreshold = Config.MainConfig.GetFloatDefault("AntiHack", "SpeedThreshold", -600.0f);
	SpeedhackProtection = antihack_speed;
	m_startLevel = Config.MainConfig.GetIntDefault("Server", "StartingLevel", 1);
	if(m_startLevel > 250) {m_startLevel = 250;} //force the startLevel to not be higher than 250
	m_levelCap = Config.MainConfig.GetIntDefault("Server", "LevelCap", 250);
	if(m_levelCap < 55) m_levelCap = 56; // Force le level max à 56 au minimum
	m_genLevelCap = Config.MainConfig.GetIntDefault("Server", "GenLevelCap", 250);
	m_limitedNames = Config.MainConfig.GetBoolDefault("Server", "LimitedNames", true);
	m_useAccountData = Config.MainConfig.GetBoolDefault("Server", "UseAccountData", false);
	m_forceGMTag = Config.MainConfig.GetBoolDefault("Server", "ForceGMTag", false);
	m_showKick = Config.MainConfig.GetBoolDefault("Server", "ShowKickMessage", true);
	// === Wrath Blizzard Requierement ====== 
    m_OneDeathKnight = Config.MainConfig.GetBoolDefault("BlizzRequi", "OneDeathKnight", true);
	m_MinAlreadyLevel = Config.MainConfig.GetIntDefault("BlizzRequi", "MinAlreadyLevel", 55);
	// ======================================
	m_movementCompressInterval = Config.MainConfig.GetIntDefault("Movement", "FlushInterval", 1000);
	m_movementCompressRate = Config.MainConfig.GetIntDefault("Movement", "CompressRate", 1);
	
	m_movementCompressThresholdCreatures = Config.MainConfig.GetFloatDefault("Movement", "CompressThresholdCreatures", 15.0f);
	m_movementCompressThresholdCreatures *= m_movementCompressThresholdCreatures;

	m_movementCompressThreshold = Config.MainConfig.GetFloatDefault("Movement", "CompressThreshold", 25.0f);
	m_movementCompressThreshold *= m_movementCompressThreshold;		// square it to avoid sqrt() on checks
	
	m_speedHackThreshold = Config.MainConfig.GetFloatDefault("AntiHack", "SpeedThreshold", -500.0f);
	m_speedHackLatencyMultiplier = Config.MainConfig.GetFloatDefault("AntiHack", "SpeedLatencyCompensation", 0.25f);
	m_speedHackResetInterval = Config.MainConfig.GetIntDefault("AntiHack", "SpeedResetPeriod", 5000);
	antihack_cheatengine = Config.MainConfig.GetBoolDefault("AntiHack", "CheatEngine", false);
	m_CEThreshold = Config.MainConfig.GetIntDefault("AntiHack", "CheatEngineTimeDiff", 10000);
	m_wallhackthreshold = Config.MainConfig.GetFloatDefault("AntiHack", "WallHackThreshold", 5.0f);
	// ======================================
	
	m_socketworker_count = Config.MainConfig.GetIntDefault("Network", "SocketWorkers", 0);
	m_nagleswitch = Config.MainConfig.GetIntDefault("Network", "NagleSwitch", 100);

	// Dual Talent Specialization costs
	dualTalentTrainCost = Config.MainConfig.GetIntDefault("Server", "DualTalentPrice", 1000);
	if(dualTalentTrainCost)
		dualTalentTrainCost *= 10000;   // Convert to gold
		
	if( m_banTable != NULL )
		free( m_banTable );

	m_banTable = NULL;
	string s = Config.MainConfig.GetStringDefault( "Server", "BanTable", "" );
	if( !s.empty() )
		m_banTable = strdup( s.c_str() );

	if( load )
		Channel::LoadConfSettings();
}

void World::LoadNameGenData()
{
	char _bufftemp_[1024];
	DBCFile dbc;

	sprintf(_bufftemp_,"%s/NameGen.dbc",DBCPath);
	if( !dbc.open( _bufftemp_ )) 
	{
		Log.Error( "World", "Cannot find file %s",_bufftemp_ );
		return;
	}
    Log.Notice("DBC:","Loading %s",_bufftemp_);
	for(uint32 i = 0; i < dbc.getRecordCount(); ++i)
	{
		NameGenData d;
		if(dbc.getRecord(i).getString(1)==NULL)
			continue;

		d.name = string(dbc.getRecord(i).getString(1));
		d.type = dbc.getRecord(i).getUInt(3);
		_namegendata[d.type].push_back(d);
	}
}

void World::CharacterEnumProc(QueryResultVector& results, uint32 AccountId)
{
	WorldSession * s = FindSession(AccountId);
	if(s == NULL)
		return;

	s->CharacterEnumProc(results[0].result);
}

void World::LoadAccountDataProc(QueryResultVector& results, uint32 AccountId)
{
	WorldSession * s = FindSession(AccountId);
	if(s == NULL)
		return;

	s->LoadAccountDataProc(results[0].result);
}

void World::CleanupCheaters()
{
	/*uint32 guid;
	string name;
	uint32 cl;
	uint32 level;
	uint32 talentpts;
	char * start, *end;
	Field * f;
	uint32 should_talents;
	uint32 used_talents;
	SpellEntry * sp;

	QueryResult * result = CharacterDatabase.Query("SELECT guid, name, class, level, available_talent_points, spells FROM characters");
	if(result == NULL)
		return;

	do 
	{
		f = result->Fetch();
		guid = f[0].GetUInt32();
		name = string(f[1].GetString());
		cl = f[2].GetUInt32();
		level = f[3].GetUInt32();
		talentpts = f[4].GetUInt32();
		start = f[5].GetString();
		should_talents = (level<10 ? 0 : level - 9);
		used_talents -= 
        		

		start = (char*)get_next_field.GetString();//buff;
		while(true) 
		{
			end = strchr(start,',');
			if(!end)break;
			*end=0;
			sp = dbcSpell.LookupEntry(atol(start));
			start = end +1;

			if(sp->talent_tree)

		}

	} while(result->NextRow());*/

}

void World::CheckForExpiredInstances()
{
	sInstanceMgr.CheckForExpiredInstances();
}

#define LOAD_THREAD_SLEEP 180

void CharacterLoaderThread::OnShutdown()
{
	running=false;
#ifdef WIN32
	SetEvent(hEvent);
#else
	pthread_cond_signal(&cond);
#endif
}

CharacterLoaderThread::CharacterLoaderThread()
{

}

CharacterLoaderThread::~CharacterLoaderThread()
{
#ifdef WIN32
	CloseHandle(hEvent);
#else
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
#endif
}

bool CharacterLoaderThread::run()
{
#ifdef WIN32
	hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
#else
	struct timeval now;
	struct timespec tv;

	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
#endif
	running=true;
	for(;;)
	{
		// Get a single connection to maintain for the whole process.
		DatabaseConnection * con = CharacterDatabase.GetFreeConnection();

		// this hasn't been updated in some time, enable it if you want to fix/use it
#if 0
		sWorld.PollCharacterInsertQueue(con);
#endif

		sWorld.PollMailboxInsertQueue(con);
		/* While this looks weird, it ensures the system doesn't waste time switching to these contexts.
		   WaitForSingleObject will suspend the thread,
		   and on unix, select will as well. - Burlex
			*/
		con->Busy.Release();
#ifdef WIN32
		WaitForSingleObject(hEvent,LOAD_THREAD_SLEEP*1000);
#else
		gettimeofday(&now, NULL);
		tv.tv_sec = now.tv_sec + LOAD_THREAD_SLEEP;
		tv.tv_nsec = now.tv_usec * 1000;
		pthread_mutex_lock(&mutex);
		pthread_cond_timedwait(&cond, &mutex, &tv);
		pthread_mutex_unlock(&mutex);
#endif
		if(!running)
			break;
	}

	return true;
}

void World::PollMailboxInsertQueue(DatabaseConnection * con)
{
	QueryResult * result;
	Field * f;
	Item * pItem;
	uint32 itemid;
	uint32 stackcount;

	result = CharacterDatabase.FQuery("SELECT * FROM mailbox_insert_queue", con);
	if( result != NULL )
	{
		Log.Notice("MailboxQueue", "Sending queued messages....");
		do 
		{
			f = result->Fetch();
			itemid = f[6].GetUInt32();
			stackcount = f[7].GetUInt32();
			
			if( itemid != 0 )
			{
				pItem = objmgr.CreateItem( itemid, NULL );
				if( pItem != NULL )
				{
					pItem->SetUInt32Value( ITEM_FIELD_STACK_COUNT, stackcount );
					pItem->SaveToDB( 0, 0, true, NULL );
				}
			}
			else
				pItem = NULL;

			Log.Notice("MailboxQueue", "Sending message to %u (item: %u)...", f[1].GetUInt32(), itemid);
			sMailSystem.SendAutomatedMessage( 0, f[0].GetUInt64(), f[1].GetUInt64(), f[2].GetString(), f[3].GetString(), f[5].GetUInt32(),
				0, pItem ? pItem->GetGUID() : 0, f[4].GetUInt32() );

			if( pItem != NULL )
				delete pItem;

		} while ( result->NextRow() );
		delete result;
		Log.Notice("MailboxQueue", "Done.");
		CharacterDatabase.FWaitExecute("DELETE FROM mailbox_insert_queue", con);
	}
}

struct insert_playeritem
{
	uint32 ownerguid;
	uint32 entry;
	uint32 wrapped_item_id;
	uint32 wrapped_creator;
	uint32 creator;
	uint32 count;
	uint32 charges;
	uint32 flags;
	uint32 randomprop;
	uint32 randomsuffix;
	uint32 itemtext;
	uint32 durability;
	int32 containerslot;
	int32 slot;
	string enchantments;
};

struct insert_playerskill
{
	uint32 player_guid;
	uint32 skill_id;
	uint32 type;
	uint32 currentlvl;
	uint32 maxlvl;
};

struct insert_playerquest 
{ 
	uint32 player_guid; 
	uint32 quest_id; 
	uint32 slot; 
	uint32 time_left; 
	uint32 explored_area1; 
	uint32 explored_area2; 
	uint32 explored_area3; 
	uint32 explored_area4; 
	uint32 mob_kill1; 
	uint32 mob_kill2; 
	uint32 mob_kill3; 
	uint32 mob_kill4; 
	uint32 slain;
};

struct insert_playerglyph 
{ 
	uint32 player_guid; 
	uint32 spec; 
	uint32 glyph1; 
	uint32 glyph2; 
	uint32 glyph3; 
	uint32 glyph4; 
	uint32 glyph5; 
	uint32 glyph6; 
};

struct insert_playertalent 
{ 
	uint32 player_guid; 
	uint32 spec; 
	uint32 tid; 
	uint32 rank; 
};

struct insert_playerspell 
{ 
	uint32 player_guid; 
	uint32 spellid; 
};

void World::PollCharacterInsertQueue(DatabaseConnection * con)
{
	QueryResult * result = CharacterDatabase.FQuery("SELECT guid FROM characters_insert_queue", con);
	if(!result)
		return;
	else
		delete result;

	// Our local stuff..
	Field * f;
	map<uint32, vector<insert_playeritem> > itemMap;
	map<uint32, vector<insert_playeritem> >::iterator itr;
	map<uint32, vector<insert_playerskill> > skillMap;
	map<uint32, vector<insert_playerskill> >::iterator itr1;
	map<uint32, vector<insert_playerquest> > questMap;
	map<uint32, vector<insert_playerquest> >::iterator itr2;
	map<uint32, vector<insert_playerglyph> > glyphMap;
	map<uint32, vector<insert_playerglyph> >::iterator itr3;
	map<uint32, vector<insert_playertalent> > talentMap;
	map<uint32, vector<insert_playertalent> >::iterator itr4;
	map<uint32, vector<insert_playerspell> > spellMap;
	map<uint32, vector<insert_playerspell> >::iterator itr5;
	insert_playeritem ipi;
	insert_playerskill ips;
	insert_playerquest ipq;
	insert_playerglyph ipg;
	insert_playertalent ipt;
	insert_playerspell ipsp;

	bool has_results = false;
	/*map<uint32, vector<insert_playeritem> > itemMap;
	map<uint32,vector<insert_playeritem> >::iterator itr;
	Field * f;
	insert_playeritem ipi;*/                          
	//static const char * characterTableFormat = "uSuuuuuussuuuuuuuuuuuuuuffffuususuufffuuuuusuuuUssuuuuuuffffuuuuufffssssssuuuuuuuu"; // 
	                                           // Le 'x' du debut correspond essentiellement au Guid, spécifique a la lecture table 'characters'
	static const char * characterTableFormat =   "xuSuuuuuussuuuuuuuuuuuuuuuffffuususuufffuuuuusuuuUuuuuffffuuuuufffssssssuuuuuuuuuuu";

	// Lock the tables to prevent any more inserts
	CharacterDatabase.FWaitExecute("LOCK TABLES characters_insert_queue WRITE, playeritems_insert_queue WRITE, playerskills_insert_queue WRITE, questlog_insert_queue WRITE, playerglyphs_insert_queue WRITE, playerspells_insert_queue WRITE, playertalents_insert_queue WRITE", con);

	// Caching will save us doing additional queries and slowing down the db.
	// Cache all items in memory.
	result = CharacterDatabase.FQuery("SELECT * FROM playeritems_insert_queue", con);
	if(result)
	{
		do 
		{
			f = result->Fetch();
			
			ipi.ownerguid = f[0].GetUInt32();
			//skip itemguid, we'll generate a new one.
			ipi.entry = f[1].GetUInt32();
			ipi.wrapped_item_id = f[2].GetUInt32();
			ipi.wrapped_creator = f[3].GetUInt32();
			ipi.creator = f[4].GetUInt32();
			ipi.count = f[5].GetUInt32();
			ipi.charges = f[6].GetUInt32();
			ipi.flags = f[7].GetUInt32();
			ipi.randomprop = f[8].GetUInt32();
			ipi.randomsuffix = f[9].GetUInt32();
			ipi.itemtext = f[10].GetUInt32();
			ipi.durability = f[11].GetUInt32();
			ipi.containerslot = f[12].GetInt32();
			ipi.slot = f[13].GetInt32();
			ipi.enchantments = string(f[14].GetString());

			itr = itemMap.find(ipi.ownerguid);
			if(itr == itemMap.end())
			{
				vector<insert_playeritem> to_insert;
				to_insert.push_back(ipi);
				itemMap.insert(make_pair(ipi.ownerguid,to_insert));
			}
			else
			{
				itr->second.push_back(ipi);
			}
		
		} while(result->NextRow());
		delete result;
	}

	// Cache all skills in memory.
	result = CharacterDatabase.FQuery("SELECT * FROM playerskills_insert_queue", con);
	if(result)
	{
		do 
		{
			f = result->Fetch();

			ips.player_guid = f[0].GetUInt32();
			ips.skill_id = f[1].GetUInt32();
			ips.type = f[2].GetUInt32();
			ips.currentlvl = f[3].GetUInt32();
			ips.maxlvl = f[4].GetUInt32();

			itr1 = skillMap.find(ips.player_guid);
			if(itr1 == skillMap.end())
			{
				vector<insert_playerskill> to_insert;
				to_insert.push_back(ips);
				skillMap.insert(make_pair(ips.player_guid,to_insert));
			}
			else
			{
				itr1->second.push_back(ips);
			}
		} while(result->NextRow());
		delete result;
	}

	// Cache all questlogs in memory.
	result = CharacterDatabase.FQuery("SELECT * FROM questlog_insert_queue", con);
	if(result)
	{
		do 
		{
			f = result->Fetch();

			ipq.player_guid = f[0].GetUInt32();
			ipq.quest_id = f[1].GetUInt32();
			ipq.slot = f[2].GetUInt32();
			ipq.time_left = f[3].GetUInt32();
			ipq.explored_area1 = f[4].GetUInt32();
			ipq.explored_area2 = f[5].GetUInt32();
			ipq.explored_area3 = f[6].GetUInt32();
			ipq.explored_area4 = f[7].GetUInt32();
			ipq.mob_kill1 = f[8].GetUInt32();
			ipq.mob_kill2 = f[9].GetUInt32();
			ipq.mob_kill3 = f[10].GetUInt32();
			ipq.mob_kill4 = f[11].GetUInt32();
			ipq.slain = f[12].GetUInt32();

			itr2 = questMap.find(ipq.player_guid);
			if(itr2 == questMap.end())
			{
				vector<insert_playerquest> to_insert;
				to_insert.push_back(ipq);
				questMap.insert(make_pair(ipq.player_guid,to_insert));
			}
			else
			{
				itr2->second.push_back(ipq);
			}

		} while(result->NextRow());
		delete result;
	}

	// Cache all glyphs in memory.
	result = CharacterDatabase.FQuery("SELECT * FROM playerglyphs_insert_queue", con);
	if(result)
	{
		do 
		{
			f = result->Fetch();

			ipg.player_guid = f[0].GetUInt32();
			ipg.spec = f[1].GetUInt32();
			ipg.glyph1 = f[2].GetUInt32();
			ipg.glyph2 = f[3].GetUInt32();
			ipg.glyph3 = f[4].GetUInt32();
			ipg.glyph4 = f[5].GetUInt32();
			ipg.glyph5 = f[6].GetUInt32();
			ipg.glyph6 = f[7].GetUInt32();

			itr3 = glyphMap.find(ipg.player_guid);
			if(itr3 == glyphMap.end())
			{
				vector<insert_playerglyph> to_insert;
				to_insert.push_back(ipg);
				glyphMap.insert(make_pair(ipg.player_guid,to_insert));
			}
			else
			{
				itr3->second.push_back(ipg);
			}

		} while(result->NextRow());
		delete result;
	}

	// Cache all talents in memory.
	result = CharacterDatabase.FQuery("SELECT * FROM playertalents_insert_queue", con);
	if(result)
	{
		do 
		{
			f = result->Fetch();

			ipt.player_guid = f[0].GetUInt32();
			ipt.spec = f[1].GetUInt32();
			ipt.tid = f[2].GetUInt32();
			ipt.rank = f[3].GetUInt32();

			itr4 = talentMap.find(ipt.player_guid);
			if(itr4 == talentMap.end())
			{
				vector<insert_playertalent> to_insert;
				to_insert.push_back(ipt);
				talentMap.insert(make_pair(ipt.player_guid,to_insert));
			}
			else
			{
				itr4->second.push_back(ipt);
			}

		} while(result->NextRow());
		delete result;
	}

	// Cache all spells in memory.
	result = CharacterDatabase.FQuery("SELECT * FROM playerspells_insert_queue", con);
	if(result)
	{
		do 
		{
			f = result->Fetch();

			ipsp.player_guid = f[0].GetUInt32();
			ipsp.spellid = f[1].GetUInt32();

			itr5 = spellMap.find(ipsp.player_guid);
			if(itr5 == spellMap.end())
			{
				vector<insert_playerspell> to_insert;
				to_insert.push_back(ipsp);
				spellMap.insert(make_pair(ipsp.player_guid,to_insert));
			}
			else
			{
				itr5->second.push_back(ipsp);
			}

		} while(result->NextRow());
		delete result;
	}

	// Load the characters, and assign them their new guids, and insert them into the main db.
	result = CharacterDatabase.FQuery("SELECT * FROM characters_insert_queue", con);

	// Can be unlocked now.
	CharacterDatabase.FWaitExecute("UNLOCK TABLES", con);

	if(result)
	{
		uint32 guid;
		std::stringstream ss;
		uint32 queuesize = result->GetRowCount();
		do 
		{
			f = result->Fetch();
			char * p = (char*)characterTableFormat;
			uint32 i = 0;
			guid = f[0].GetUInt32();

			//Generate a new player guid
			uint32 new_guid = objmgr.GenerateLowGuid(HIGHGUID_TYPE_PLAYER);
			
			// Create his playerinfo in the server
			PlayerInfo * inf = new PlayerInfo();
			memset(inf, 0, sizeof(PlayerInfo));
			inf->guid = new_guid;
			inf->acct = f[1].GetUInt32();
#ifdef VOICE_CHAT
			inf->groupVoiceId = -1;
#endif
			inf->cl = f[4].GetUInt32();
			inf->race=f[3].GetUInt32();
			inf->gender = f[5].GetUInt32();
			inf->lastLevel = f[7].GetUInt32();
			inf->lastOnline = UNIXTIME;
			switch(inf->race)
			{
			case RACE_HUMAN:
			case RACE_GNOME:
			case RACE_DWARF:
			case RACE_NIGHTELF:
			case RACE_DRAENEI:
				{
					inf->team=0;
				}break;
			default:
				{
					inf->team=1;
				}break;
			}

			// Build our query
			ss << "INSERT INTO characters VALUES(" << new_guid;
			while(*p != 0)
			{
				switch(*p)
				{
				case 's':
					ss << ",'" << CharacterDatabase.EscapeString(f[i].GetString(), con) << "'";
					break;

				case 'f':
					ss << ",'" << f[i].GetFloat() << "'";
					break;

				case 'S':
					{
						// this is the character name, append a hex version of the guid to it to prevent name clashes.
						char newname[100];
						snprintf(newname,20,"%5s%X",f[i].GetString(),new_guid);
						ss << ",'" << CharacterDatabase.EscapeString(newname,con) << "'";
						inf->name = strdup(newname);
					}break;

				case 'U':
					{
						// this is our forced rename field. force it to one.
						ss << ",1";
					}break;

				case 'x':
					{
						// players guid (we generate a new one) 
					}break;
				default:
					ss << "," << f[i].GetUInt32();
					break;
				}

				++i;
				++p;
			}

			ss << ")";
			//Execute the query
			CharacterDatabase.FWaitExecute(ss.str().c_str(),con);

			// Add playerinfo to the objectmgr
			objmgr.AddPlayerInfo(inf);

			// grab all his items, assign them their new guids and insert them
			itr = itemMap.find(guid);
			if(itr != itemMap.end())
			{
				for(vector<insert_playeritem>::iterator vtr = itr->second.begin(); vtr != itr->second.end(); ++vtr)
				{
					//Generate a new item guid
					uint32 new_item_guid = objmgr.GenerateLowGuid(HIGHGUID_TYPE_ITEM);

					//empty our query string
					ss.rdbuf()->str("");

					// Build query

					ss << "INSERT INTO playeritems VALUES(";
					ss << new_guid << ","
						<< new_item_guid << ","
						<< (*vtr).entry << ","
						<< (*vtr).wrapped_item_id << ","
						<< (*vtr).wrapped_creator << ","
						<< (*vtr).creator << ","
						<< (*vtr).count << ","
						<< (*vtr).charges << ","
						<< (*vtr).flags << ","
						<< (*vtr).randomprop << ","
						<< (*vtr).randomsuffix << ","
						<< (*vtr).itemtext << ","
						<< (*vtr).durability << ","
						<< (*vtr).containerslot << ","
						<< (*vtr).slot << ",'"
						<< (*vtr).enchantments << "')";
					//Execute query
					CharacterDatabase.FWaitExecute(ss.str().c_str(),con);
				}
			}

			// grab all his skills, assign them their new guids and insert them
			itr1 = skillMap.find(guid);
			if(itr1 != skillMap.end())
			{
				for(vector<insert_playerskill>::iterator vtr1 = itr1->second.begin(); vtr1 != itr1->second.end(); ++vtr1)
				{
					//empty our query string
					ss.rdbuf()->str("");

					// Build query
					ss << "INSERT INTO playerskills VALUES(";
					ss << new_guid << ","
						<< (*vtr1).skill_id << ","
						<< (*vtr1).type << ","
						<< (*vtr1).currentlvl << ","
						<< (*vtr1).maxlvl << " )";
					//Execute query
					CharacterDatabase.FWaitExecute(ss.str().c_str(),con);
				}
			}

			// grab all his quests, assign them their new guids and insert them
			itr2 = questMap.find(guid);
			if(itr2 != questMap.end())
			{
				for(vector<insert_playerquest>::iterator vtr2 = itr2->second.begin(); vtr2 != itr2->second.end(); ++vtr2)
				{
					//empty our query string
					ss.rdbuf()->str("");

					// Build query
					ss << "INSERT INTO questlog VALUES(";
					ss << new_guid << ","
						<< (*vtr2).quest_id << ","
						<< (*vtr2).slot << ","
						<< (*vtr2).time_left << ","
						<< (*vtr2).explored_area1 << ","
						<< (*vtr2).explored_area2 << ","
						<< (*vtr2).explored_area3 << ","
						<< (*vtr2).explored_area4 << ","
						<< (*vtr2).mob_kill1 << ","
						<< (*vtr2).mob_kill2 << ","
						<< (*vtr2).mob_kill3 << ","
						<< (*vtr2).mob_kill4 << ","
						<< (*vtr2).slain << ")";
					//Execute query
					CharacterDatabase.FWaitExecute(ss.str().c_str(),con);
				}
			}

			// grab all his glyphs, assign them their new guids and insert them
			itr3 = glyphMap.find(guid);
			if(itr3 != glyphMap.end())
			{
				for(vector<insert_playerglyph>::iterator vtr3 = itr3->second.begin(); vtr3 != itr3->second.end(); ++vtr3)
				{
					//empty our query string
					ss.rdbuf()->str("");

					// Build query
					ss << "INSERT INTO playerglyphs VALUES(";
					ss << new_guid << ","
						<< (*vtr3).spec << ","
						<< (*vtr3).glyph1 << ","
						<< (*vtr3).glyph2 << ","
						<< (*vtr3).glyph3 << ","
						<< (*vtr3).glyph4 << ","
						<< (*vtr3).glyph5 << ","
						<< (*vtr3).glyph6 << " )";
					//Execute query
					CharacterDatabase.FWaitExecute(ss.str().c_str(),con);
				}
			}

			// grab all his talents, assign them their new guids and insert them
			itr4 = talentMap.find(guid);
			if(itr4 != talentMap.end())
			{
				for(vector<insert_playertalent>::iterator vtr4 = itr4->second.begin(); vtr4 != itr4->second.end(); ++vtr4)
				{
					//empty our query string
					ss.rdbuf()->str("");

					// Build query
					ss << "INSERT INTO playertalents VALUES(";
					ss << new_guid << ","
						<< (*vtr4).spec << ","
						<< (*vtr4).tid << ","
						<< (*vtr4).rank << " )";
					//Execute query
					CharacterDatabase.FWaitExecute(ss.str().c_str(),con);
				}
			}

			// grab all his spells, assign them their new guids and insert them
			itr5 = spellMap.find(guid);
			if(itr5 != spellMap.end())
			{
				for(vector<insert_playerspell>::iterator vtr5 = itr5->second.begin(); vtr5 != itr5->second.end(); ++vtr5)
				{
					//empty our query string
					ss.rdbuf()->str("");

					// Build query
					ss << "INSERT INTO playerspells VALUES(";
					ss << new_guid << ","
						<< (*vtr5).spellid << " )";
					//Execute query
					CharacterDatabase.FWaitExecute(ss.str().c_str(),con);
				}
			}

			//empty our query string
			ss.rdbuf()->str("");
		} while(result->NextRow());
		has_results = true;
		Log.Notice("CharacterLoader","Imported %u character(s) from external queue",queuesize);
		delete result;
	}

	// Clear all the data in the tables.
	if(has_results)
	{
		CharacterDatabase.FWaitExecute("DELETE FROM characters_insert_queue", con);
		CharacterDatabase.FWaitExecute("DELETE FROM playeritems_insert_queue", con);
		CharacterDatabase.FWaitExecute("DELETE FROM playerskills_insert_queue", con);
		CharacterDatabase.FWaitExecute("DELETE FROM questlog_insert_queue", con);
		CharacterDatabase.FWaitExecute("DELETE FROM playerglyphs_insert_queue", con);
		CharacterDatabase.FWaitExecute("DELETE FROM playertalents_insert_queue", con);
		CharacterDatabase.FWaitExecute("DELETE FROM playerspells_insert_queue", con);
	}
}

void World::DisconnectUsersWithAccount(const char * account, WorldSession * m_session)
{
	SessionMap::iterator itr;
	WorldSession * session;
	m_sessionlock.AcquireReadLock();
	bool FoundUser = false;
	for(itr = m_sessions.begin(); itr != m_sessions.end();)
	{
		session = itr->second;
		++itr;

		if(!stricmp(account, session->GetAccountNameS()))
		{
			FoundUser = true;
			m_session->SystemMessage("Disconnecting user with account `%s` IP `%s` Player `%s`.", session->GetAccountNameS(), 
				session->GetSocket() ? session->GetSocket()->GetRemoteIP().c_str() : "noip", session->GetPlayer() ? session->GetPlayer()->GetName() : "noplayer");

			session->Disconnect();
		}
	}
	m_sessionlock.ReleaseReadLock();
	if ( FoundUser == false )
		m_session->SystemMessage("There is nobody online with account [%s]",account);
}

void World::DisconnectUsersWithIP(const char * ip, WorldSession * m_session)
{
	SessionMap::iterator itr;
	WorldSession * session;
	m_sessionlock.AcquireReadLock();
	bool FoundUser = false;
	for(itr = m_sessions.begin(); itr != m_sessions.end();)
	{
		session = itr->second;
		++itr;

		if(!session->GetSocket())
			continue;

		string ip2 = session->GetSocket()->GetRemoteIP().c_str();
		if(!stricmp(ip, ip2.c_str()))
		{
			FoundUser = true;
			m_session->SystemMessage("Disconnecting user with account `%s` IP `%s` Player `%s`.", session->GetAccountNameS(), 
				ip2.c_str(), session->GetPlayer() ? session->GetPlayer()->GetName() : "noplayer");

			session->Disconnect();
		}
	}
	if ( FoundUser == false )
		m_session->SystemMessage("There is nobody online with ip [%s]",ip);
	m_sessionlock.ReleaseReadLock();
}

void World::DisconnectUsersWithPlayerName(const char * plr, WorldSession * m_session)
{
	SessionMap::iterator itr;
	WorldSession * session;
	m_sessionlock.AcquireReadLock();
	bool FoundUser = false;
	for(itr = m_sessions.begin(); itr != m_sessions.end();)
	{
		session = itr->second;
		++itr;

		if(!session->GetPlayer())
			continue;

		if(!stricmp(plr, session->GetPlayer()->GetName()))
		{
			m_session->SystemMessage("Disconnecting user with account `%s` IP `%s` Player `%s`.", session->GetAccountNameS(), 
				session->GetSocket() ? session->GetSocket()->GetRemoteIP().c_str() : "noip", session->GetPlayer() ? session->GetPlayer()->GetName() : "noplayer");

			session->Disconnect();
		}
	}
	if (FoundUser == false)
		m_session->SystemMessage("There is no body online with the name [%s]",plr);
	m_sessionlock.ReleaseReadLock();
}

string World::GetUptimeString()
{
	char str[300];
	time_t pTime = (time_t)UNIXTIME - m_StartTime;
	tm * tmv = gmtime(&pTime);

	snprintf(str, 300, "%u days, %u hours, %u minutes, %u seconds.", tmv->tm_yday, tmv->tm_hour, tmv->tm_min, tmv->tm_sec);
	return string(str);
}

//---------------------------------------
// Changement de la zone de texte en UTF8
char *World::ChangeLatin2UTF8(const char *zone_text)
{
 if(!sWorld.m_Latin2UTF8) return((char *)zone_text);

 uint32 i,j,size_text;
 static char Tbuffer[512]; // +512 Octets

 size_text = (uint32)strlen(zone_text);

 //Log.Warning("ChangeLatin2UTF8","Source: %s",zone_text);

 if(size_text < 2) return((char*)zone_text); // Secu: mauvais alignement en lecture (Patch+Old DBCs)

 if(size_text > 250) return((char *)zone_text);

 memset(Tbuffer,0x00,512);
 j=0;
 for(i=0;i < size_text ;++i)
 {
  if( ((uint32)(*(zone_text+i)) & 0xC0) && ((uint32)(*(zone_text+i+1)) & 0x80) ) // Deja UTF8 (sans BOM)
  { 
      //Log.Warning("ChangeLatin2UTF8","UTF8 detection, index %d",i);
	  return((char *)zone_text);
  } 

  Tbuffer[j] = *(zone_text+i);

  if((uint32)(*(zone_text+i)) < 128) { ++j; continue; } // Pas d'UTF8

  //Log.Warning("ChangeLatin2UTF8","UTF8 insertion, index %d",i);

  Tbuffer[j++] = 0xC0 | ((((unsigned char)(*(zone_text+i))) >> 6) & 0x1F); // 1 ier octet
  Tbuffer[j++] = 0x80 | (*(zone_text+i) & 0x3F);                           // 2 ieme octet
  //Log.Warning("ChangeLatin2UTF8","0x%02X -> 0x%02X 0x%02X",(unsigned char)(*(zone_text+i)),(unsigned char)Tbuffer[j-2],(unsigned char)Tbuffer[j-1]);
 } // End for(_ind_=...

 //Log.Warning("ChangeLatin2UTF8","Cible: %s",Tbuffer);

 return(&Tbuffer[0]);

} // End Procedure ChangeLatin2UTF8
//-----------------------------------------------------------------------------------------

void World::UpdateShutdownStatus()
{
	uint32 time_left = ((uint32)UNIXTIME > m_shutdownTime) ? 0 : m_shutdownTime - (uint32)UNIXTIME;
	uint32 time_period = 1;

	if( time_left && m_shutdownTime )
	{
		// determine period
		if( time_left <= 30 )
		{
			// every 1 sec
			time_period = 1;
		}
		else if( time_left <= (TIME_MINUTE * 2) )
		{
			// every 30 secs
			time_period = 30;
		}
		else
		{
			// every minute
			time_period = 60;
		}

		// time to send a new packet?
		if( ( (uint32)UNIXTIME - m_shutdownLastTime ) >= time_period )
		{
			// send message
			m_shutdownLastTime = (uint32)UNIXTIME;

			WorldPacket data(SMSG_SERVER_MESSAGE, 200);
			if( m_shutdownType == SERVER_SHUTDOWN_TYPE_RESTART )
				data << uint32(SERVER_MSG_RESTART_TIME);
			else
				data << uint32(SERVER_MSG_SHUTDOWN_TIME);

			char tbuf[100];
			snprintf(tbuf, 100, "%02u:%02u", (time_left / 60), (time_left % 60));
			data << tbuf;
			SendGlobalMessage(&data, NULL);

			printf("Server shutdown in %s.\n", tbuf);
		}
	}
	else
	{
		// shutting down?
		sEventMgr.RemoveEvents(this, EVENT_WORLD_SHUTDOWN);
		if( m_shutdownTime )
		{
			SendWorldText("Server is saving and shutting down. You will be disconnected shortly.", NULL);
			Master::m_stopEvent = true;
		}
		else
		{
			WorldPacket data(SMSG_SERVER_MESSAGE, 200);
			if( m_shutdownTime == SERVER_SHUTDOWN_TYPE_RESTART )
				data << uint32(SERVER_MSG_RESTART_CANCELLED);
			else
				data << uint32(SERVER_MSG_SHUTDOWN_CANCELLED);

			data << uint8(0);
			SendGlobalMessage(&data, NULL);
		}
	}
}

void World::CancelShutdown()
{
	m_shutdownTime = 0;
	m_shutdownType = 0;
	m_shutdownLastTime = 0;
}

void World::QueueShutdown(uint32 delay, uint32 type)
{
	// set parameters
	m_shutdownLastTime = 0;
	m_shutdownTime = (uint32)UNIXTIME + delay;
	m_shutdownType = type;

	// add event
	sEventMgr.AddEvent(this, &World::UpdateShutdownStatus, EVENT_WORLD_SHUTDOWN, 50, 0, 0);

	// send message
	char buf[1000];
	snprintf(buf, 1000, "Server %s initiated. Server will save and shut down in approx. %u seconds.", type == SERVER_SHUTDOWN_TYPE_RESTART ? "restart" : "shutdown", delay);
	SendWorldText(buf, NULL);
}


void World::BackupDB()
{
#ifndef WIN32
	const char *tables[] =
	{ "account_data", "account_forced_permissions", "achievements", "arenateams", "auctions",
	  "banned_names", "characters", "characters_insert_queue", "charters", "corpses", "gm_tickets",
	  "groups", "guild_bankitems", "guild_banklogs", "guild_banktabs",
	  "guild_data", "guild_logs", "guild_ranks", "guilds",
	  "instances", "mailbox", "mailbox_insert_queue",
	  "playercooldowns", "playeritems", "playeritems_insert_queue", "playerpets",
	  "playerpetspells", "playerpettalents", "playersummons", "playersummonspells", "questlog",
	  "server_settings", "social_friends", "social_ignores", "tutorials",
	  "worldstate_save_data", NULL };
	char cmd[1024];
	char datestr[256];
	char path[256];
	std::string user, pass;
	std::string host, name;
	struct tm tm;
	time_t t;
	int i;

	user = Config.MainConfig.GetStringDefault("CharacterDatabase", "Username", "");
	pass = Config.MainConfig.GetStringDefault("CharacterDatabase", "Password", "");
	host = Config.MainConfig.GetStringDefault("CharacterDatabase", "Hostname", "");
	name = Config.MainConfig.GetStringDefault("CharacterDatabase", "Name", "");
	t = time(NULL);
	localtime_r(&t, &tm);
	strftime(datestr, 256, "%Y.%m.%d", &tm);

	snprintf(path, 256, "bk/%s", datestr);
	snprintf(cmd, 1024, "mkdir -p %s", path);

	sLog.outString("Backing up character db into %s", path);

	for (i=0; tables[i] != NULL; i++)
	{
		snprintf(cmd, 1024, "mkdir -p %s", path);
		system(cmd);

		snprintf(cmd, 1024, "mysqldump -u\"%s\" -p\"%s\" -h\"%s\" --result-file=\"%s/%s.sql\" \"%s\" \"%s\"", user.c_str(), pass.c_str(), host.c_str(), path, tables[i], name.c_str(), tables[i]);
		system(cmd);
	}

	sLog.outString("Done!");
#endif
}