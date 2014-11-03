/* Ascent MMORPG Server
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
UpdateMask Player::m_visibleUpdateMask;
#define COLLISION_MOUNT_CHECK_INTERVAL 1000

// Taxi du DK, Full Azeroth et Kalimdor
static const uint32 DKNodesMask[NUM_TAXI_FIELDS] = 
{
	0xFFFFFFFB, //   1 à  32 // - ile des programmeurs
	0xF3FFFFFF, //  33 à  64
	0x317EFFFF, //  65 à  96
	0x00000000, //  97 à 128
	0x02004000, // 129 à 160
	0x001400E0, // 161 à 192
	0xC1C02014, // 193 à 224
	0x00012018, // 225 à 256
	0x00000380, // 257 à 288
	0x7C000C10, // 289 à 320 // + Les griffons du fort d'ebene
	0x00000000  // 321 à 352
};

Player::Player( uint32 guid ) : m_mailBox(guid)
{
	// Synchro
	SyncPlayerLatency     = 0;
	SyncCurrentTickNumber = 0;
	SyncPlayerTickNumber  = 0;
	SyncServeurTickNumber = 0; // Numero de la Synchro client/serveur
	SyncServeurTickCount  = 0; 
    SyncPlayerTickCount   = 0;

	m_pvpcombat = false;
	m_runemask = 0x3F;
	m_bgRatedQueue = false;
	m_objectTypeId = TYPEID_PLAYER;
	m_valuesCount = PLAYER_END;
	m_uint32Values = _fields;

	memset(m_uint32Values, 0,(PLAYER_END)*sizeof(uint32));
	m_updateMask.SetCount(PLAYER_END);
	SetUInt32Value( OBJECT_FIELD_TYPE,TYPE_PLAYER|TYPE_UNIT|TYPE_OBJECT);
	SetUInt32Value( OBJECT_FIELD_GUID,guid);
	m_wowGuid.Init(GetGUID());
	//m_deathRuneMasteryChance = 0;

	m_achievementInterface = new AchievementInterface(this);
	m_ItemInterface		 = new ItemInterface(this);

	m_finishingmovesdodge = false;
	iActivePet			  = 0;
	resurrector			 = 0;
	SpellCrtiticalStrikeRatingBonus = 0;
	SpellHasteRatingBonus   = 0;
	m_lifetapbonus		  = 0;
	info					= NULL;				 // Playercreate info
	SoulStone			   = 0;
	SoulStoneReceiver		= 0;
	bReincarnation			= false;
	m_furorChance			= 0;
	Seal					= 0;
	judgespell			  = 0;
	m_session			   = 0;
	TrackingSpell		   = 0;
	m_status				= 0;
	offhand_dmg_mod		 = 0.5;
	m_walkSpeed			= 2.5f;
	m_runSpeed			  = PLAYER_NORMAL_RUN_SPEED;
	m_isMoving			  = false;
	m_ShapeShifted		  = 0;
	m_curTarget			 = 0;
	m_curSelection		  = 0;
	m_lootGuid			  = 0;
	
	PlayerPetHasQueued	  = false;
	m_PetNumberMax		  = MAX_PET_NO;
	m_lastShotTime		  = 0;

	m_H_regenTimer			= 0;
	m_P_regenTimer			= 0;
	m_onTaxi				= false;
	
	m_taxi_pos_x			= 0;
	m_taxi_pos_y			= 0;
	m_taxi_pos_z			= 0;
	m_taxi_ride_time		= 0;

	// Attack related variables
	m_blockfromspellPCT	 = 0;
	m_blockfromspell		= 0;
	m_critfromspell		 = 0;
	m_spellcritfromspell	= 0;
	m_dodgefromspell		= 0;
	m_parryfromspell		= 0;
	m_hitfromspell		  = 0; 
	m_hitfrommeleespell	 = 0;
	m_meleeattackspeedmod   = 1.0f;
	m_rangedattackspeedmod  = 1.0f;
	
	m_cheatDeathRank = 0;

	m_healthfromspell	   = 0;
	m_manafromspell		 = 0;
	m_healthfromitems	   = 0;
	m_manafromitems		 = 0;

	m_talentresettimes	  = 0;

	m_nextSave			  = getMSTime() + sWorld.getIntRate(INTRATE_SAVE);

	m_currentSpell		  = NULL;
	m_resurrectHealth	   = m_resurrectMana = 0;

	m_GroupInviter		  = 0;
	
	Lfgcomment = "";
	
	for(int i=0;i<3;i++)
	{
		LfgType[i]=0;
		LfgDungeonId[i]=0;
	}
	
	m_Autojoin = false;
	m_AutoAddMem = false;
	LfmDungeonId=0;
	LfmType=0;

	m_invitersGuid		  = 0;

	m_currentMovement	   = MOVE_UNROOT;
	m_isGmInvisible		 = false;

    titanGrip = false;

	//DK
	m_invitersGuid		  = 0;

	//Trade
	ResetTradeVariables();
	mTradeTarget = 0;

	//Duel
	DuelingWith			 = NULL;
	m_duelCountdownTimer	= 0;
	m_duelStatus			= 0;
	m_duelState			 = DUEL_STATE_FINISHED;		// finished

	//WayPoint
	waypointunit			= NULL;

	//PVP
	//PvPTimeoutEnabled	   = false;

	//Tutorials
	for ( int aX = 0 ; aX < 8 ; aX++ )
		m_Tutorials[ aX ] = 0x00;

	m_lootGuid			  = 0;
	m_banned				= false;

	//Bind possition
	m_bind_pos_x			= 0;
	m_bind_pos_y			= 0;
	m_bind_pos_z			= 0;
	m_bind_mapid			= 0;
	m_bind_zoneid		   = 0;

	// Rest
	m_timeLogoff			= 0;
	m_isResting			 = 0;
	m_restState			 = 0;
	m_restAmount			= 0;
	m_afk_reason			= "";
	m_playedtime[0]		 = 0;
	m_playedtime[1]		 = 0;
	m_playedtime[2]		 = (uint32)UNIXTIME;

	m_AllowAreaTriggerPort  = true;

	// Battleground
	m_bgEntryPointMap	   = 0;
	m_bgEntryPointX		 = 0;	
	m_bgEntryPointY		 = 0;
	m_bgEntryPointZ		 = 0;
	m_bgEntryPointO		 = 0;
	for(uint32 i = 0; i < 3; ++i)
	{
		m_bgQueueType[i] = 0;
		m_bgQueueInstanceId[i] = 0;
		m_bgIsQueued[i] = false;
		m_bgQueueTime[i] = 0;
	}
	m_bg = 0;

	m_bgHasFlag			 = false;
	m_bgEntryPointInstance  = 0;

	// gm stuff
	//m_invincible			= false;
	bGMTagOn				= false;
	CooldownCheat		   = false;
	CastTimeCheat		   = false;
	PowerCheat			  = false;
	GodModeCheat			= false;
	FlyCheat				= false;
	
	//FIX for professions
	weapon_proficiency	  = 0x4000;//2^14
	//FIX for shit like shirt etc
	armor_proficiency	   = 1;

	m_bUnlimitedBreath	  = false;
	m_UnderwaterState	   = 0;
	m_UnderwaterTime		= 60000;
	m_UnderwaterMaxTime	 = 60000;
	m_UnderwaterLastDmg	 = getMSTime();
	m_SwimmingTime		  = 0;
	m_BreathDamageTimer	 = 0;

	//transport shit
	m_TransporterGUID		= 0;
	m_TransporterX			= 0.0f;
	m_TransporterY			= 0.0f;
	m_TransporterZ			= 0.0f;
	m_TransporterO			= 0.0f;
	m_TransporterUnk		= 0.0f;
	m_lockTransportVariables= false;

	// Autoshot variables
	m_AutoShotTarget		= 0;
	m_onAutoShot			= false;
	m_AutoShotDuration		= 0;
	m_AutoShotAttackTimer	= 0;
	m_AutoShotSpell			= NULL;

	m_AttackMsgTimer		= 0;

	timed_quest_slot		= 0;
	m_GM_SelectedGO			= NULL;

	for(uint32 x = 0;x < 7; x++)
	{
		FlatResistanceModifierPos[x] = 0;
		FlatResistanceModifierNeg[x] = 0;
		BaseResistanceModPctPos[x] = 0;
		BaseResistanceModPctNeg[x] = 0; 
		ResistanceModPctPos[x] = 0;
		ResistanceModPctNeg[x] = 0;
		SpellDelayResist[x] = 0;
		m_casted_amount[x] = 0;
	} 
		

	for(uint32 x = 0; x < 5; x++)
	{
		FlatStatModNeg[x] = 0;
		StatModPctNeg[x] = 0;
		TotalStatModPctNeg[x] = 0;
	}


	for(uint32 x = 0; x < 12; x++)
	{
		IncreaseDamageByType[x] = 0;
		IncreaseDamageByTypePCT[x] = 0;
		IncreaseCricticalByTypePCT[x] = 0;
	}

	PctIgnoreRegenModifier  = 0.10f; // Devrait etre basé sur ce qui reste, 
	m_retainedrage          = 0;
	DetectedRange		   = 0;

	m_targetIcon			= 0;
	bShouldHaveLootableOnCorpse = false;
	m_MountSpellId		  = 0;
	bHasBindDialogOpen	  = false;
	m_CurrentCharm		  = NULL;
	m_CurrentTransporter	= NULL;
	m_SummonedObject		= NULL;
	m_currentLoot		   = (uint64)NULL;
	pctReputationMod		= 0;
	roll					= 0;
	mUpdateCount			= 0;
    mCreationCount          = 0;
    bCreationBuffer.reserve(40000);
	bUpdateBuffer.reserve(30000);//ought to be > than enough ;)
	mOutOfRangeIds.reserve(1000);
	mOutOfRangeIdCount	  = 0;

	bProcessPending		 = false;
	for(int i = 0; i < 25; ++i)
		m_questlog[i] = NULL;

	CurrentGossipMenu	   = NULL;

	ResetHeartbeatCoords();
	cannibalize			 = false;

	m_AreaID				= 0;
	m_actionsDirty		  = false;
	cannibalizeCount		= 0;
	rageFromDamageDealt	 = 0;

	m_honorToday			= 0;
	m_honorYesterday		= 0;
	m_honorPoints		   = 0;
	m_killsToday			= 0;
	m_killsYesterday		= 0;
	m_killsLifetime		 = 0;

	m_honorless			 = false;	
	m_lastSeenWeather	   = 0;
	m_attacking			 = false;
	
	myCorpse				= 0;
	bCorpseCreateable	   = true;
	blinked				 = false;
	m_speedhackChances	  = 3;
	m_explorationTimer	  = getMSTime();
	linkTarget			  = 0;
	stack_cheat			 = false;
	triggerpass_cheat = false;
	m_pvpTimer			  = 0;
	m_globalCooldown = 0;
	m_lastHonorResetTime	= 0;
	memset(&mActions, 0, PLAYER_ACTION_BUTTON_SIZE);
	tutorialsDirty = true;
	m_TeleportState = 1;
	m_beingPushed = false;
	for(int i = 0; i < NUM_CHARTER_TYPES; ++i)
		m_charters[i]=NULL;
	for(int i = 0; i < NUM_ARENA_TEAM_TYPES; ++i)
		m_arenaTeams[i]=NULL;

	flying_aura = 0;
	resend_speed = false;
	rename_pending = false;
	iInstanceType		   = 0;
	memset(reputationByListId, 0, sizeof(FactionReputation*) * 128);

	m_comboTarget = 0;
	m_comboPoints = 0;

	SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER, 0.0f);
	SetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER, 0.0f);

	UpdateLastSpeeds();

	m_resist_critical[0]=m_resist_critical[1]=0;
	for (uint32 x =0;x<3;x++)
	{
		m_resist_hit[x]=0;
	}

	//m_ModInterrMRegenPCT = 0;
	//m_ModInterrMRegen =0;	
	m_RegenManaOnSpellResist=0;
	m_rap_mod_pct = 0;
	m_modblockabsorbvalue = 0;
	m_modblockvaluefromspells = 0;
	m_summoner = m_summonInstanceId = m_summonMapId = 0;
	m_lastMoveType = 0;
		for(int i = 0; i < 6; ++i)
		{
			m_runes[i] = baseRunes[i];
		}
	m_maxTalentPoints = 0;
	m_talentActiveSpec = 0;
	m_talentSpecsCount = 1;
	ok_to_remove = false;
	trigger_on_stun = 0;
	trigger_on_stun_chance = 100;
	m_modphyscritdmgPCT = 0;
	m_RootedCritChanceBonus = 0;
	//m_tempSummon = 0;
	m_spellcomboPoints = 0;
	for(uint8 i = 0; i < 3 ; ++i)
			m_pendingBattleground[i] = NULL;
	m_deathVision = false;
	m_retainComboPoints = false;
	last_heal_spell = NULL;
	m_playerInfo = NULL;
	m_sentTeleportPosition.ChangeCoords(999999.0f,999999.0f,999999.0f);
	m_speedChangeCounter=1;
	memset(&m_bgScore,0,sizeof(BGScore));
	m_arenaPoints = 0;
	memset(&m_spellIndexTypeTargets, 0, sizeof(uint64)*NUM_SPELL_TYPE_INDEX);
	m_base_runSpeed = m_runSpeed;
	m_base_walkSpeed = m_walkSpeed;
	m_arenateaminviteguid=0;
	m_arenaPoints=0;
	m_honorRolloverTime=0;
	hearth_of_wild_pct = 0;
	raidgrouponlysent=false;
	loot.gold=0;
	m_waterwalk=false;
	m_setwaterwalk=false;
	m_areaSpiritHealer_guid=0;
	m_CurrentTaxiPath=NULL;
	m_setflycheat = false;
	m_fallDisabledUntil = 0;
	m_lfgMatch = NULL;
	m_lfgInviterGuid = 0;
	m_mountCheckTimer = 0;
	m_taxiMapChangeNode = 0;
	_startMoveTime = 0;
	
#ifdef ENABLE_COMPRESSED_MOVEMENT
	m_movementBuffer.reserve(5000);
#endif

	_heartbeatDisable = 0;
	m_safeFall = 0;
	m_noFallDamage = false;
	z_axisposition = 0.0f;
	m_KickDelay = 0;
	m_speedhackCheckTimer = 0;
	_speedChangeInProgress = false;
	m_passOnLoot = false;
	m_changingMaps = true;
	m_outStealthDamageBonusPct = m_outStealthDamageBonusPeriod = m_outStealthDamageBonusTimer = 0;
	m_vampiricEmbrace = m_vampiricTouch = 0;
	m_lastMoveTime = 0;
	m_lastMovementPacketTimestamp = 0;
	m_cheatEngineChances = 2;
	mWeakenedSoul = false;
	mForbearance = false;
	mExhaustion = false;
	mHypothermia = false;
	mSated = false;
	mAvengingWrath = true;
	
	m_wratings.clear();
	m_QuestGOInProgress.clear();
	m_removequests.clear();
	m_finishedQuests.clear();
	quest_spells.clear();
	quest_mobs.clear();
	loginauras.clear();
	OnMeleeAuras.clear();
	m_Pets.clear();
	m_itemsets.clear();
	m_channels.clear();
	m_visibleObjects.clear();

#ifdef COLLISION
	m_flyhackCheckTimer = 0;
#endif

	/*runes
	for (uint8 i=0; i<RUNE_SLOTS; ++i)
	{
		m_runes[i].index = i;
		
		if (i < 2)
			m_runes[i].originalType = RUNE_TYPE_BLOOD;
		else if (i < 4)
			m_runes[i].originalType = RUNE_TYPE_FROST;
		else if (i < 6)
			m_runes[i].originalType = RUNE_TYPE_UNHOLY;
		else
			m_runes[i].originalType = RUNE_TYPE_DEATH; //so nothing breaks

		m_runes[i].runeType = m_runes[i].originalType;
		m_runes[i].activateTime = getMSTime();
	}*/

	m_AreaGoTriggered = false; // Lock pour OnActivateGoArea()

    // Synchro client/serveur
	SyncServeurTickNumber = 0;
	SyncPlayerTickNumber  = 0;
    SyncServeurTickCount  = 0;
	SyncPlayerTickCount   = 0;

	this->OnLogin();
}

void Player::OnLogin()
{
	Log.Notice("Connection New Player","Login Ok");
}


void Player::Init()
{
	Unit::Init();
}

Player::~Player()
{
	Player *pThis = this ; //prevent us going feeefee

	sEventMgr.RemoveEvents(this);

	if(!ok_to_remove)
	{
		Log.Error("Player","Deleted from non-logoutplayer!\n");
		OutputCrashLogLine("Player deleted from non-logoutplayer!");
#ifdef WIN32
		CStackWalker sw;
		sw.ShowCallstack();
#endif
	}

	objmgr.RemovePlayer(static_cast<Player *>(this));

	if(m_session)
	{
		m_session->SetPlayer(NULL);
	}

	Player * pTarget;
	if(mTradeTarget != 0)
	{
		pTarget = GetTradeTarget();
		if(pTarget)
			pTarget->mTradeTarget = 0;
	}

	pTarget = objmgr.GetPlayer(GetInviter());
	if(pTarget != NULL)
		pTarget->SetInviter(0);

	pTarget = NULL;

	if(m_Summon)
	{
		m_Summon->Dismiss(true);
		m_Summon->ClearPetOwner();
	}

	if (m_GM_SelectedGO)
		m_GM_SelectedGO = NULL;

	if (m_SummonedObject)
	{
		m_SummonedObject->Delete();
		m_SummonedObject = NULL;
	}

	if(DuelingWith != 0)
	{
		sLog.outError("Player destructor reached with duel target");
		EndDuel(0);
	}

	mTradeTarget = 0;

	CleanupGossipMenu();
	//ASSERT(!IsInWorld()); // Impossible que ca arrive, franchement celuila il sert plus a rien!! (Branruz)
    
	if(!IsInWorld())
	{
	 sEventMgr.RemoveEvents(this);

	// delete m_talenttree

	 CleanupChannels();
	 for(int i = 0; i < 25; ++i)
	 {
		if(m_questlog[i] != NULL)
		{
			delete m_questlog[i];
			m_questlog[i] = NULL;
		}
	 }

	 for(SplineMap::iterator itr = _splineMap.begin(); itr != _splineMap.end(); ++itr)
		delete itr->second;

	 _splineMap.clear();

	if(m_ItemInterface)
		delete m_ItemInterface;

	if(m_achievementInterface)
		delete m_achievementInterface;

	 for(ReputationMap::iterator itr = m_reputation.begin(); itr != m_reputation.end(); ++itr)
		delete itr->second;
	 m_reputation.clear();
	

	m_objectTypeId = TYPEID_UNUSED;

	if(m_playerInfo)
		m_playerInfo->m_loggedInPlayer=NULL;



	 while( delayedPackets.size() )
	 {
		WorldPacket * pck = delayedPackets.next();
		delete pck;
		pck = NULL;
	 }
	
	 if (m_session != NULL)
		m_session->SetPlayer(NULL);

//	SetSession(NULL);
// Note Randdrick :En commentaire car provoque un crash du CORE à la reconnection d'un Player. 
// A vérifier.
	 /*if (mSpellsUniqueTargets)
	{
		for (int i=0; i<hashmap_length(mSpellsUniqueTargets); i++)
		{
			uint64 *ids;
			uint32 guid;

			if (hashmap_get_index(mSpellsUniqueTargets, i, (int32*)&guid, (any_t*)&ids) == MAP_OK)
			{
				if (ids) free(ids);
			}
		}

		hashmap_free(mSpellsUniqueTargets);
		mSpellsUniqueTargets = NULL;
	}*/

	 if (m_lfgMatch != NULL)
	 {
		m_lfgMatch->lock.Acquire();
		m_lfgMatch->AcceptedPlayers.erase(this);
		m_lfgMatch->PendingPlayers.erase(this);
		m_lfgMatch->lock.Release();
	 }

	 if (m_CurrentTransporter != NULL)
	 {
		m_CurrentTransporter->RemovePlayer(this);
		m_CurrentTransporter = NULL;
		m_TransporterGUID = 0;
	 }

	 m_visibleFarsightObjects.clear();
	 gmTargets.clear();
	
	if(myCorpse) myCorpse = NULL;

	if (linkTarget != NULL) linkTarget = NULL;

	for (uint8 i=0; i < MAX_TRADE_ITEM; ++i) 
	{
		mTradeItems[i] = NULL;
	}

	m_wratings.clear();
	m_QuestGOInProgress.clear();
	m_removequests.clear();
	m_finishedQuests.clear();
	quest_spells.clear();
	quest_mobs.clear();
	loginauras.clear();
	OnMeleeAuras.clear();

	for(std::map<uint32, PlayerPet*>::iterator itr = m_Pets.begin(); itr != m_Pets.end(); ++itr)
		delete itr->second;
	m_Pets.clear();
	
	m_itemsets.clear();
	m_channels.clear();
	mSpells.clear();

	 objmgr.RemovePlayer(this);		
	
	} // end of if(!IsInWorld())
	else Log.Error("Player::Destructor()","FATAL: Probleme lié a la deconnection d'un joueur!!");
}

ASCENT_INLINE uint32 GetSpellForLanguage(uint32 SkillID)
{
	switch(SkillID)
	{
	case SKILL_LANG_COMMON:
		return 668;
		break;

	case SKILL_LANG_ORCISH:
		return 669;
		break;

	case SKILL_LANG_TAURAHE:
		return 670;
		break;

	case SKILL_LANG_DARNASSIAN:
		return 671;
		break;

	case SKILL_LANG_DWARVEN:
		return 672;
		break;

	case SKILL_LANG_THALASSIAN:
		return 813;
		break;

	case SKILL_LANG_DRACONIC:
		return 814;
		break;

	case SKILL_LANG_DEMON_TONGUE:
		return 815;
		break;

	case SKILL_LANG_TITAN:
		return 816;
		break;

	case SKILL_LANG_OLD_TONGUE:
		return 817;
		break;

	case SKILL_LANG_GNOMISH:
		return 7430;
		break;

	case SKILL_LANG_TROLL:
		return 7341;
		break;

	case SKILL_LANG_GUTTERSPEAK:
		return 17737;
		break;

	case SKILL_LANG_DRAENEI:
		return 29932;
		break;
	}

	return 0;
}

void Player::Destructor()
{
	Log.Debug("Player", "Appel Destructor()");
}

///====================================================================
///  Create
///  params: p_newChar
///  desc:   data from client to create a new character
///====================================================================
bool Player::Create(WorldPacket& data )
{
	uint8 race,class_,gender,skin,face,hairStyle,hairColor,facialHair,outfitId;

	// 10 races
	static uint16 player_item_disp[10]= { 8995,28483,25970,16672,25459,28261,8308,24919,25697,8995 };

	// unpack data into member variables
	data >> m_name;
	
	// correct capitalization
	CapitalizeString(m_name);

	data >> race >> class_ >> gender >> skin >> face;
	data >> hairStyle >> hairColor >> facialHair >> outfitId;

    // Debug
	sLog.outDetail("Creation d'un Perso: %s Race %d, Class %d\n",m_name.c_str(),race,class_);

    // Collision
#ifdef COLLISION
    QueryResult *PlayerHeight = WorldDatabase.Query("SELECT * FROM playercreateinfo_height WHERE raceid=%u",race);
	if(PlayerHeight)
	{
		Field *fields = PlayerHeight->Fetch(); 
		Taille = fields[2].GetFloat(); // Hauteur = Fct(Race)
		Log.Warning("PlayerCreateInfo","%s: Collision active, Taille %0.2f m pour race %u",m_name.c_str(),Taille,race);
	}
	else 
#endif
	{
		Taille = 1;
		Log.Warning("PlayerCreateInfo","%s: Taille manquante Race %u (Defaut=1)",m_name.c_str(),race);
	}

    //------

	info = objmgr.GetPlayerCreateInfo(race, class_);
	if(!info)
	{
		// info not found... disconnect
		//sCheatLog.writefromsession(m_session, "tried to create invalid player with race %u and class %u", race, class_);
		Log.Error("[Player::Create]","CHEATER (%u): Tentative de creation d'un personnage avec race %u et class %u",this->GetSession()->GetAccountId() ,race, class_);
		m_session->Disconnect();
		return false;
	}
	
	m_mapId = info->mapId;
	m_zoneId = info->zoneId;
	m_position.ChangeCoords(info->positionX, info->positionY, info->positionZ);
	m_bind_pos_x = info->positionX;
	m_bind_pos_y = info->positionY;
	m_bind_pos_z = info->positionZ;
	m_bind_mapid = info->mapId;
	m_bind_zoneid = info->zoneId;
	m_isResting = 0;
	m_restAmount = 0;
	m_restState = 0;

	// set race dbc
	myRace = dbcCharRace.LookupEntry(race);
	myClass = dbcCharClass.LookupEntry(class_);
	if(!myRace || !myClass)
	{
		// information not found
		sCheatLog.writefromsession(m_session, "tried to create invalid player with race %u and class %u, dbc info not found", race, class_);
		m_session->Disconnect();
		return false;
	}

	// Night-Elf Gnome Humain Dwarf Draenie
	// Blood-Elf Orc Tauren Troll Undead 1
    // Broken Fel-Orc Forest-Troll Goblin Ice-Troll Naga Northrend-Skeleton Skeleton Taunka Tuskan Vrykul = 2
	if(myRace->TeamId == 0) // Ok 335.12340 (dans ChrRaces.dbc)
		m_team = FACTION_ALLY;   
	else if(myRace->TeamId == 1)
		m_team = FACTION_HORDE;  
	else // A Finir (Branruz)
	{
       Log.Warning("Player::Create","Faction %d inconnu <== Report this to dev",myRace->TeamId);
       m_team = FACTION_HORDE;  
	}
	uint8 powertype = myClass->power_type;

	// Automatically add the race's taxi hub to the character's taximask at creation time ( 1 << (taxi_node_id-1) )
	memset(m_taximask,0,NUM_TAXI_FIELDS);
	// Ascent313 rev5
	if(class_ == DEATHKNIGHT)
	{
		for(uint8 i=0;i < NUM_TAXI_FIELDS;++i) 
			m_taximask[i] |= DKNodesMask[i];
	}
    // race dependant taxi node
	switch(race)
	{
		case RACE_TAUREN:	SetTaximaskNode(22);						break; // Thunder Bluff
		case RACE_HUMAN:	SetTaximaskNode(2);							break; // Stormwind
		case RACE_DWARF:	SetTaximaskNode(6);							break; // Ironforge
		case RACE_GNOME:	SetTaximaskNode(6);							break; // Ironforge
		case RACE_ORC:		SetTaximaskNode(23);						break; // Ogrimmar
		case RACE_TROLL:	SetTaximaskNode(23);						break; // Ogrimmar
		case RACE_UNDEAD:	SetTaximaskNode(11);						break; // Undercity
		case RACE_NIGHTELF:	{SetTaximaskNode(26); SetTaximaskNode(27);}	break; // Auberdine / Rut'theran
		case RACE_BLOODELF:	SetTaximaskNode(82);						break; // Silvermoon
		case RACE_DRAENEI:	SetTaximaskNode(94);						break; // L'exodar
	}
	// team dependant taxi node
	switch(m_team)
	{ 
	  case FACTION_ALLY : SetTaximaskNode(100); // Bastion de l'honneur
		                  break;  

      case FACTION_HORDE : SetTaximaskNode(99); // Thrallmar
		                  break;
	  default: break;
	}

	// Set Starting stats for char
	//SetFloatValue(OBJECT_FIELD_SCALE_X, ((race==RACE_TAUREN)?1.3f:1.0f));
	SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
	SetUInt32Value(UNIT_FIELD_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_POWER1, info->mana );
	SetUInt32Value(UNIT_FIELD_POWER2, 0 ); // rage
	SetUInt32Value(UNIT_FIELD_POWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_POWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_POWER5, PET_UNHAPPY_VALUE); // >> 1 ); // Pet Hapiness
	SetUInt32Value(UNIT_FIELD_POWER6, 8); // Nombre de Rune ?
	SetUInt32Value(UNIT_FIELD_POWER7, 0); // Puissance Runic
   
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_MAXPOWER1, info->mana );
	SetUInt32Value(UNIT_FIELD_MAXPOWER2, info->rage );
	SetUInt32Value(UNIT_FIELD_MAXPOWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_MAXPOWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_MAXPOWER5, PET_HAPPY_VALUE);
	SetUInt32Value(UNIT_FIELD_MAXPOWER6, 8);
	SetUInt32Value(UNIT_FIELD_MAXPOWER7, 130 );

	// /!\ Doit etre def avant ApplyLevelInfo (Branruz) 
    SetUInt32Value(UNIT_FIELD_STAT0, info->strength );
	SetUInt32Value(UNIT_FIELD_STAT1, info->ability );
	SetUInt32Value(UNIT_FIELD_STAT2, info->stamina );
	SetUInt32Value(UNIT_FIELD_STAT3, info->intellect );
	SetUInt32Value(UNIT_FIELD_STAT4, info->spirit );

	// /!\ Doit etre def avant ApplyLevelInfo (Branruz) 
    SetByte( UNIT_FIELD_BYTES_0, 0, race );
    SetByte( UNIT_FIELD_BYTES_0, 1, class_ );
    SetByte( UNIT_FIELD_BYTES_0, 2, gender );
    SetByte( UNIT_FIELD_BYTES_0, 3, powertype);

	SetClassString(); // Pour AI des pnjs

	//UNIT_FIELD_BYTES_1	(standstate) | (unk1) | (unk2) | (attackstate)
	//SetByte( UNIT_FIELD_BYTES_1, 0, standstate );
	SetByte(UNIT_FIELD_BYTES_1, 1,REBELIOUS); 
    //SetByte( UNIT_FIELD_BYTES_1, 3, attackstate );

	//SetUInt32Value(UNIT_FIELD_BYTES_2, (0x28 << 8) );
    //SetUInt32Value(UNIT_FIELD_BYTES_2,(uint32)(U_FIELD_BYTES_FLAG_PVP << 8)); // ArcEmu ?? Pourkoi ?? (Branruz)
	//SetByte( UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_PVP );

	SetUInt32Value(PLAYER_FIELD_MAX_LEVEL, sWorld.m_levelCap);
	uint32 lvl = sWorld.m_startLevel;
	if((lvl < 1) || (lvl > (sWorld.m_levelCap - 1))) 
	{
		if(lvl < 1) lvl = 1;
		else lvl = sWorld.m_levelCap - 1;
		Log.Error("[ascent-world.conf]","Le Start Level doit etre compris entre 1 et 249 (%u)",sWorld.m_startLevel);
		sWorld.m_startLevel = lvl; //  re-init pour le reste du code (Branruz)
	}

	InitGlyphSlots();
	InitGlyphsForLevel();

	// lookup level information
	lvlinfo = objmgr.GetLevelInfo(race, class_, lvl); 
	if(lvlinfo) 
	{
		if (class_ == DEATHKNIGHT) 
		{
			if(lvl > 55) ApplyLevelInfo(lvlinfo, lvl);
			else         ApplyLevelInfo(lvlinfo, 55);
		}
		else             ApplyLevelInfo(lvlinfo, lvl);
	}
	else
	{
		Log.Error("[Player::Create]","FATAL: (%s) LevelInfo n'existe pas: Class: %u, Race: %u, Level: %u",
			                                                             GetName(),class_,race,lvl);
		m_session->Disconnect();
		return false;
	}

	//THIS IS NEEDED (A cause de l'ApplyLevelInfo qui ne devrait servir qu'au levelUp)
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_BASE_MANA, info->mana );
	SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, info->factiontemplate );

	// Posture ?
	if(class_ == WARRIOR) SetShapeShift(FORM_BATTLESTANCE);

	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);

	SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 0.388999998569489f );
	SetFloatValue(UNIT_FIELD_COMBATREACH, 1.5f);
	if(race != RACE_BLOODELF)
	{
		SetUInt32Value(UNIT_FIELD_DISPLAYID, info->displayId + gender );
		SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, info->displayId + gender );
	} 
	else	
	{
		SetUInt32Value(UNIT_FIELD_DISPLAYID, info->displayId - gender );
		SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, info->displayId - gender );
	}
	//SetFloatValue(UNIT_FIELD_MINDAMAGE, info->mindmg );
	//SetFloatValue(UNIT_FIELD_MAXDAMAGE, info->maxdmg );
	SetUInt32Value(UNIT_FIELD_ATTACK_POWER, info->attackpower );

	//SetUInt32Value(UNIT_FIELD_RESISTANCES_1,0);  // Holy : Not Used
	SetUInt32Value(UNIT_FIELD_RESISTANCES_2,0);  // Feu
	SetUInt32Value(UNIT_FIELD_RESISTANCES_4,0);  // Givre
    SetUInt32Value(UNIT_FIELD_RESISTANCES_6,0);  // Arcane
	BaseResistance[RESISTANCE_ARCANE] = 0;
	BaseResistance[RESISTANCE_FIRE]   = 0;
	BaseResistance[RESISTANCE_FROST]  = 0;

	if (class_ == DEATHKNIGHT) 
	{
		SetUInt32Value(UNIT_FIELD_RESISTANCES_3,0);  // 0 de Nature pour le DK
	    SetUInt32Value(UNIT_FIELD_RESISTANCES_5,5);  // +5 d'Ombre pour le DK
        BaseResistance[RESISTANCE_NATURE] = 0;
        BaseResistance[RESISTANCE_SHADOW] = 5; 
	}
	else                       
	{
		SetUInt32Value(UNIT_FIELD_RESISTANCES_3,10); // +10 de Nature pour les autres
	    SetUInt32Value(UNIT_FIELD_RESISTANCES_5,0);  // 0 d'Ombre  pour les autres
		BaseResistance[RESISTANCE_NATURE] = 10;
        BaseResistance[RESISTANCE_SHADOW] = 0;
	}
 
	SetUInt32Value(PLAYER_BYTES, ((skin) | (face << 8) | (hairStyle << 16) | (hairColor << 24)));
	//PLAYER_BYTES_2							   GM ON/OFF	 BANKBAGSLOTS   RESTEDSTATE
   // SetUInt32Value(PLAYER_BYTES_2, (facialHair | (0xEE << 8) | (0x01 << 16) | (0x02 << 24)));
	SetUInt32Value(PLAYER_BYTES_2, (facialHair /*| (0xEE << 8)*/  | (0x02 << 24)));//no bank slot by default!

	//PLAYER_BYTES_3						   DRUNKENSTATE				 PVPRANK
	SetUInt32Value(PLAYER_BYTES_3, ((gender) | (0x00 << 8) | (0x00 << 16) | (GetPVPRank() << 24)));
	SetUInt32Value(PLAYER_NEXT_LEVEL_XP, 400);
	SetUInt32Value(PLAYER_FIELD_BYTES, 0x08 );
	SetUInt32Value(PLAYER_CHARACTER_POINTS2,2); // MaxProfessions (A Voir - Branruz)
	SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
	//SetUInt32Value(PLAYER_FIELD_MAX_LEVEL, sWorld.m_levelCap(this));
	
    SetUInt32Value( PLAYER_GUILDID, 0 );
    SetUInt32Value( PLAYER_GUILDRANK, 0 );
    SetUInt32Value( PLAYER_GUILD_TIMESTAMP, 0 );

    for(int i = 0; i < KNOWN_TITLES_SIZE; ++i)
        SetUInt64Value(PLAYER__FIELD_KNOWN_TITLES + i, 0);  // 0=disabled
    SetUInt32Value( PLAYER_CHOSEN_TITLE, 0 );

    SetUInt32Value( PLAYER_FIELD_KILLS, 0 );
    SetUInt32Value( PLAYER_FIELD_LIFETIME_HONORBALE_KILLS, 0 );
    SetUInt32Value( PLAYER_FIELD_TODAY_CONTRIBUTION, 0 );
    SetUInt32Value( PLAYER_FIELD_YESTERDAY_CONTRIBUTION, 0 );
	
	for(uint32 x=0;x<7;x++)
		SetFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT+x, 1.00);

	SetUInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX, 0xEEEEEEEE);

	m_StableSlotCount = 0;

	for(std::set<uint32>::iterator sp = info->spell_list.begin();sp!=info->spell_list.end();sp++)
	{
		mSpells.insert((*sp));
	}

	m_FirstLogin = true;

   // Skill du DK a sa creation, http://www.wowwiki.com/Starting_a_death_knight 
	/*
     Sacs: 10 champigons noir (lv55) et 30pa

	Posture: Presence de sang (equivalent de la posture combat wawa)

	Competence de classe (48266): Givre(771) Impie(772) Sang(770) ( + RuneForger(776) sur quete) 

	53428	Runeforge	Permet au chevalier de la mort d'orner son arme de runes.
    53424	Runeforge	
	         Apprend au chevalier de la mort a orner de runes ses armes. 
			 Vous apprendrez les sorts suivants  : Rune de Brise-épée,     (54446)
			                                       Rune de Fracasse-épée,  (53323)
												   Rune de Lichemort,      (53331)
												   Rune de Cendreglace,    (53341)
												   Rune de Trancheglace,   (53343) 
												   Rune de Brise-sort,     (54447)
												   Rune de fracasse-sort   (53342)
												   et Rune du Croisé déchu.(53344)
   
	Grimoire: 
		General:Ambidexie (674), 
		        Rapidité (seulement au fort), Resist Nature, (RuneForge sur quete = 53424), 
			          Parade (3127) , esquive (81) , Attaque 

       Attaque: 3606 ??
	   Givre: Fievre de givre (55095), Focalisation runique (61455), Toucher de glace (45477)
	   Impie: Frappe de peste (45462), Peste de sang (59879 ), Poigne de la mort (49576) , Voile mortel (52375)
       Sang : Deviation Energetique (49410), Frappe de sang (45902), (Posture Presence de sang (48266))
       */
    //------------------
	skilllineentry * se;
	for(std::list<CreateInfo_SkillStruct>::iterator ss = info->skills.begin(); ss!=info->skills.end(); ss++)
	{
		se = dbcSkillLine.LookupEntry(ss->skillid);
		if(se->type != SKILL_TYPE_LANGUAGE)
		{
			if( sWorld.m_startLevel > 1 )
			{
				_AddSkillLine(se->id, sWorld.m_startLevel * 5, sWorld.m_startLevel * 5 );
			}
			else
				_AddSkillLine(se->id, ss->currentval, ss->maxval);
		}
	}
	_UpdateMaxSkillCounts();
	
	_InitialReputation();

	// Add actionbars
	for(std::list<CreateInfo_ActionBarStruct>::iterator itr = info->actionbars.begin();itr!=info->actionbars.end();++itr)
	{
		setAction(itr->button, itr->action, itr->type, itr->misc);
	}

	EquipInit(info);

	sHookInterface.OnCharacterCreate(this);

	load_health = m_uint32Values[UNIT_FIELD_HEALTH];
	load_mana = m_uint32Values[UNIT_FIELD_POWER1];

	if(class_ == DEATHKNIGHT) // On sauvegarde systematiquement au cas ou les pre-requis change
	{
		CharacterDatabase.Query("UPDATE `_blizzrequirements` SET AlreadyDK=%u WHERE acct_id = %u",this->GetGUID() ,this->GetSession()->GetAccountId());	   
	}	

	return true;
}

void Player::EquipInit(PlayerCreateInfo *EquipInfo)
{

	for(std::list<CreateInfo_ItemStruct>::iterator is = EquipInfo->items.begin(); is!=EquipInfo->items.end(); is++)
	{
		if ( (*is).protoid != 0)
		{
			Item *item=objmgr.CreateItem((*is).protoid,this);
			if(item)
			{
				item->SetUInt32Value(ITEM_FIELD_STACK_COUNT,(*is).amount);
				if((*is).slot<INVENTORY_SLOT_BAG_END)
				{
					if( !GetItemInterface()->SafeAddItem(item, INVENTORY_SLOT_NOT_SET, (*is).slot) )
						item->Delete();
				}
				else
				{
					if( !GetItemInterface()->AddItemToFreeSlot(item) )
						item->Delete();
				}
			}
		}
	}

}
void Player::Update( uint32 p_time )
{
	if(!IsInWorld())
		return;

	Unit::Update( p_time );
	uint32 mstime = getMSTime();

	if(m_attacking)
	{
		// Check attack timer.
		if(mstime >= m_attackTimer)	_EventAttack(false);

		if(m_duelWield && mstime >= m_attackTimer_1) _EventAttack(true);

		//Log.Warning("Player::Update","%s est en combat",this->GetName());
	}

	if( m_onAutoShot)
	{
		if( m_AutoShotAttackTimer > p_time )
		{
			//sLog.outDebug( "HUNTER AUTOSHOT 0) %i, %i", m_AutoShotAttackTimer, p_time );
			m_AutoShotAttackTimer -= p_time;
		}
		else
		{
			//sLog.outDebug( "HUNTER AUTOSHOT 1) %i", p_time );
			EventRepeatSpell(); 
		}
	}
	else if(m_AutoShotAttackTimer > 0)
	{
		if(m_AutoShotAttackTimer > p_time)
			m_AutoShotAttackTimer -= p_time;
		else
			m_AutoShotAttackTimer = 0;
	}

	
	// Breathing
	if( m_UnderwaterState & UNDERWATERSTATE_UNDERWATER )
	{
		//sLog.outDebug("%s: m_UnderwaterState: 0x%08X",this->GetName(),m_UnderwaterState);

		if(m_MountSpellId) 
		{
			RemoveAura(m_MountSpellId); // Remove des montures dans l'eau (Branruz)
			m_MountSpellId = 0;
		}

		// keep subtracting timer
		if( m_UnderwaterTime )
		{
			// not taking dmg yet
			if(p_time >= m_UnderwaterTime)
				m_UnderwaterTime = 0;
			else
				m_UnderwaterTime -= p_time;
		}

		if( !m_UnderwaterTime )
		{
			// check last damage dealt timestamp, and if enough time has elapsed deal damage
			if( mstime >= m_UnderwaterLastDmg )
			{
				uint32 damage = m_uint32Values[UNIT_FIELD_MAXHEALTH] / 10;
				WorldPacket data(SMSG_ENVIRONMENTALDAMAGELOG, 21);
				data << GetGUID() << uint8(DAMAGE_DROWNING) << damage << uint64(0);
				SendMessageToSet(&data, true);

				DealDamage(this, damage, 0, 0, 0);
				m_UnderwaterLastDmg = mstime + 1000;
			}
		}
	}
	else
	{
		// check if we're not on a full breath timer
		if(m_UnderwaterTime < m_UnderwaterMaxTime)
		{
			// regenning
			m_UnderwaterTime += (p_time * 10);

			if(m_UnderwaterTime >= m_UnderwaterMaxTime)
			{
				m_UnderwaterTime = m_UnderwaterMaxTime;
				StopMirrorTimer(1);
			}
		}
	}

	// Lava Damage
	if(m_UnderwaterState & UNDERWATERSTATE_LAVA)
	{
		// check last damage dealt timestamp, and if enough time has elapsed deal damage
		if(mstime >= m_UnderwaterLastDmg)
		{
			uint32 damage = m_uint32Values[UNIT_FIELD_MAXHEALTH] / 5;
			WorldPacket data(SMSG_ENVIRONMENTALDAMAGELOG, 21);
			data << GetGUID() << uint8(DAMAGE_LAVA) << damage << uint64(0);
			SendMessageToSet(&data, true);

			DealDamage(this, damage, 0, 0, 0);
			m_UnderwaterLastDmg = mstime + 1000;
		}
	}

	// Autosave
	if(mstime >= m_nextSave)
		SaveToDB(false);

	if(m_CurrentTransporter && !m_lockTransportVariables)
	{
		// Update our position, using trnasporter X/Y
		float c_tposx = m_CurrentTransporter->GetPositionX() + m_TransporterX;
		float c_tposy = m_CurrentTransporter->GetPositionY() + m_TransporterY;
		float c_tposz = m_CurrentTransporter->GetPositionZ() + m_TransporterZ;
		SetPosition(c_tposx, c_tposy, c_tposz, GetOrientation(), false);
	}

	if(m_CurrentVehicle)
	{
		// Update our position
		float vposx = m_CurrentVehicle->GetPositionX();
		float vposy = m_CurrentVehicle->GetPositionY();
		float vposz = m_CurrentVehicle->GetPositionZ();
		float vposo = m_CurrentVehicle->GetOrientation();
		SetPosition(vposx, vposy, vposz, vposo, false);
	}

	// Exploration
	if(mstime >= m_explorationTimer)
	{
		//Log.Notice("[m_explorationTimer]","(%s) Exploration de la zone proche",this->GetName());
		_EventExploration();
		m_explorationTimer = mstime + 3000;
	}

	if(m_pvpTimer)
	{
		if(p_time >= m_pvpTimer)
		{
			RemovePvPFlag();
			m_pvpTimer = 0;
		}
		else
			m_pvpTimer -= p_time;
	}

#ifdef COLLISION
	if(m_MountSpellId != 0)
	{
		if( mstime >= m_mountCheckTimer )
		{
			if( CollideInterface.IsIndoor( m_mapId, m_position.x, m_position.y, m_position.z ) )
			{
				RemoveAura( m_MountSpellId );
				m_MountSpellId = 0;
			}
			else
			{
				m_mountCheckTimer = mstime + COLLISION_MOUNT_CHECK_INTERVAL;
			}
		}
	}
#endif

	if( mstime >= m_speedhackCheckTimer )
	{
		_SpeedhackCheck( mstime );
		m_speedhackCheckTimer = mstime + 1000;
	}

#ifdef COLLISION
	if( mstime >= m_flyhackCheckTimer )
	{
		_FlyhackCheck();
		m_flyhackCheckTimer = mstime + 10000; 
	}
#endif
}

void Player::EventDismount(uint32 money, float x, float y, float z)
{
	ModUnsigned32Value( PLAYER_FIELD_COINAGE , -(int32)money );

	SetPosition(x, y, z, true);
	if(!m_taxiPaths.size())
		SetTaxiState(false);

	SetTaxiPath(NULL);
	UnSetTaxiPos();
	m_taxi_ride_time = 0;

	SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID , 0);
	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

	SetPlayerSpeed(RUN, m_runSpeed);

	sEventMgr.RemoveEvents(this, EVENT_PLAYER_TAXI_INTERPOLATE);

	// Save to database on dismount
	SaveToDB(false);

	// If we have multiple "trips" to do, "jump" on the next one :p
	if(m_taxiPaths.size())
	{
		TaxiPath * p = *m_taxiPaths.begin();
	
		m_taxiPaths.erase(m_taxiPaths.begin());
		TaxiStart(p, taxi_model_id, 0);
	}

	ResetHeartbeatCoords();
}

void Player::EventTaxiDismount(TaxiPathNode* pn)
{
	DBCTaxiPathNode* node = dbcTaxiPathNode.LookupEntry(pn->id);

	if (node == NULL || node->ArrivalEventId == 0) return;

	sScriptMgr.CreateScriptClassForEvent(node->ArrivalEventId, this, NULL, NULL);
}

void Player::_EventAttack( bool offhand )
{
	//Log.Warning("Player","_EventAttack");

	if (m_currentSpell)
	{
		m_currentSpell->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
		setAttackTimer(500, offhand);
		return;
	}

	if(!CanAct())
		return;

	Unit *pVictim = NULL;
	if(m_curSelection)
		pVictim = GetMapMgr()->GetUnit(m_curSelection);
	
	//Can't find victim, stop attacking
	if (!pVictim)
	{
		sLog.outDetail("Player::Update:  No valid current selection to attack, stopping attack");
		setHRegenTimer(5000); //prevent clicking off creature for a quick heal
		EventAttackStop();
		return;
	}

	if( !isAttackable( this, pVictim, false ) )
	{
		setHRegenTimer(5000);
		EventAttackStop();
		return;
	}

	if (!canReachWithAttack(pVictim))
	{
		if(m_AttackMsgTimer != 1)
		{
			m_session->OutPacket(SMSG_ATTACKSWING_NOTINRANGE);
			m_AttackMsgTimer = 1;
		}
		setAttackTimer(300, offhand);
	}
	else if(!isInFront(pVictim))
	{
		// We still have to do this one.
		if(m_AttackMsgTimer != 2)
		{
			m_session->OutPacket(SMSG_ATTACKSWING_BADFACING);
			m_AttackMsgTimer = 2;
		}
		setAttackTimer(300, offhand);
	}
	else
	{
		m_AttackMsgTimer = 0;
		
		// Set to weapon time.
		setAttackTimer(0, offhand);

		//pvp timeout reset
		if(pVictim->IsPlayer())
		{
			if (static_cast< Player* >(pVictim)->cannibalize)
			{
				sEventMgr.RemoveEvents(pVictim, EVENT_CANNIBALIZE);
				pVictim->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
				static_cast< Player* >(pVictim)->cannibalize = false;
			}
		}

		if(this->IsStealth())
		{
			RemoveAura( m_stealth );
			SetStealth(0);
		}

		if (!GetOnMeleeSpell() || offhand)
		{
			Strike( pVictim, ( offhand ? OFFHAND : MELEE ), NULL, 0, 0, 0, false, false );
				
		} 
		else 
		{ 
			SpellEntry *spellInfo = dbcSpell.LookupEntry(GetOnMeleeSpell());
			uint8 ecn = GetOnMeleeSpellExtraCN();				
			SetOnMeleeSpell(0, 0); // ascent5218 - rev214
			Spell *spell = new Spell(this,spellInfo,true,NULL);
			SpellCastTargets targets;
			targets.m_target = GetMapMgr()->GetUnit(GetSelection());
			spell->extra_cast_number = ecn;				
			spell->prepare(&targets);
		}
	}
}

void Player::_EventCharmAttack()
{
	if(!m_CurrentCharm)
		return;

	Unit *pVictim = NULL;
	if(!IsInWorld())
	{
		m_CurrentCharm=NULL;
		sEventMgr.RemoveEvents(this,EVENT_PLAYER_CHARM_ATTACK);
		return;
	}

	if(m_curSelection == 0)
	{
		sEventMgr.RemoveEvents(this, EVENT_PLAYER_CHARM_ATTACK);
		return;
	}

	pVictim= GetMapMgr()->GetUnit(m_curSelection);

	//Can't find victim, stop attacking
	if (!pVictim)
	{
		sLog.outError( "WORLD: "I64FMT" doesn't exist.",m_curSelection);
		sLog.outDetail("Player::Update:  No valid current selection to attack, stopping attack\n");
		this->setHRegenTimer(5000); //prevent clicking off creature for a quick heal
		clearStateFlag(UF_ATTACKING);
		EventAttackStop();
	}
	else
	{
		if (!m_CurrentCharm->canReachWithAttack(pVictim))
		{
			if(m_AttackMsgTimer == 0)
			{
				//m_session->OutPacket(SMSG_ATTACKSWING_NOTINRANGE);
				m_AttackMsgTimer = 2000;		// 2 sec till next msg.
			}
			// Shorten, so there isnt a delay when the client IS in the right position.
			sEventMgr.ModifyEventTimeLeft(this, EVENT_PLAYER_CHARM_ATTACK, 100);	
		}
		else if(!m_CurrentCharm->isInFront(pVictim))
		{
			if(m_AttackMsgTimer == 0)
			{
				m_session->OutPacket(SMSG_ATTACKSWING_BADFACING);
				m_AttackMsgTimer = 2000;		// 2 sec till next msg.
			}
			// Shorten, so there isnt a delay when the client IS in the right position.
			sEventMgr.ModifyEventTimeLeft(this, EVENT_PLAYER_CHARM_ATTACK, 100);	
		}
		else
		{
			//if(pVictim->GetTypeId() == TYPEID_UNIT)
			//	pVictim->GetAIInterface()->StopMovement(5000);

			//pvp timeout reset
			/*if(pVictim->IsPlayer())
			{
				if( static_cast< Player* >( pVictim )->DuelingWith == NULL)//Dueling doesn't trigger PVP
					static_cast< Player* >( pVictim )->PvPTimeoutUpdate(false); //update targets timer

				if(DuelingWith == NULL)//Dueling doesn't trigger PVP
					PvPTimeoutUpdate(false); //update casters timer
			}*/

			if (!m_CurrentCharm->GetOnMeleeSpell())
			{
				m_CurrentCharm->Strike( pVictim, MELEE, NULL, 0, 0, 0, false, false );
			} 
			else 
			{ 
				SpellEntry *spellInfo = dbcSpell.LookupEntry(m_CurrentCharm->GetOnMeleeSpell());
				uint8 ecn = GetOnMeleeSpellExtraCN();
				m_CurrentCharm->SetOnMeleeSpell(0,0); // Ascent5218 - Randrick 214
				Spell *spell = new Spell(m_CurrentCharm,spellInfo,true,NULL);
				SpellCastTargets targets;
				targets.m_target = GetMapMgr()->GetUnit(GetSelection());
				spell->extra_cast_number = ecn;
				spell->prepare(&targets);
				//delete spell;		 // deleted automatically, no need to do this.
			}
		}
	}   
}

void Player::EventAttackStart()
{
	m_attacking = true;
	if(m_MountSpellId) RemoveAura(m_MountSpellId);
}

void Player::EventAttackStop()
{
	if(m_CurrentCharm != NULL)
		sEventMgr.RemoveEvents(this, EVENT_PLAYER_CHARM_ATTACK);

	m_attacking = false;

	//Log.Warning("EventAttackStop","EventAttackStop");
}

void Player::_EventExploration()
{

	if (isDead()) 	
	{
		//Log.Notice("[_EventExploration]","(%s) Pas d'exploration, le joueur est mort!",this->GetName());
		return;
	}

	if (!IsInWorld()) 	
	{
		Log.Error("[_EventExploration]","(%s) N'est pas present dans la map!",this->GetName());
		return;
	}

	if(m_position.x > _maxX || m_position.x < _minX || m_position.y > _maxY || m_position.y < _minY)
	{
		Log.Error("[_EventExploration]","(%s) Position Hors Limite!",this->GetName());
		return;
	}

	if(GetMapMgr()->GetCellByCoords(GetPositionX(),GetPositionY()) == NULL) 
	{
		Log.Error("[_EventExploration]","(%s) CellByCoords non trouvee!",this->GetName());
		return;
	}
	
    
	uint16 AreaId = GetMapMgr()->GetAreaID(GetPositionX(),GetPositionY(),GetPositionZ());
	//float PosZ_Land = GetMapMgr()->GetLandHeight(GetPositionX(),GetPositionY());
	//Log.Notice("[_EventExploration()]","GetLandHeight = %0.5f, Player: %0.5f",PosZ_Land,GetPositionZ());
	
	if(!AreaId || AreaId == 0xFFFF)
	{
		// Dans un donjon ? (Branruz)
		//Log.Error("[_EventExploration]","(%s) AreaId %u non trouvee! <- Report this to developper",this->GetName(),AreaId);
		//Log.Error("[_EventExploration]","Coor  X: %f Y:%f",m_position.x,m_position.y);
		return;
	}

	// Debug
	//Log.Notice("[_EventExploration]","(%s) AreaId %u (%u)",this->GetName(),AreaId,this->GetAreaID());
	//Log.Notice("[_EventExploration]","Coor X: %f Y:%f",m_position.x,m_position.y);

	// AreaId fix for undercity and ironforge.  This will now enable rest for these 2 cities.
	// since they're both on the same map, only 1 map id check
	if (GetMapId() == 0) 
	{
		// Check for Undercity, Tirisfal Glades, and Ruins of Lordaeron, if neither, skip
		if (AreaId == 153 || AreaId == 85 || m_AreaID == 1497) 
		{
			float ss_x = m_position.x;
		    float ss_y = m_position.y;
		    float ss_z = m_position.z;
			// ruins check     
			if (ss_z < 74) 
			{
				// box with coord 1536,174 -> 1858,353; and z < 62.5 for reachable areas   
				if (ss_y > 174 && ss_y < 353 && ss_x > 1536 && ss_x < 1858) 
				{
					AreaId = 1497;
				}
			}
			// inner city check
			if (ss_z < 38) 
			{
				// box with coord 1238, 11 -> 1823, 640; and z < 38 for undeground
				if (ss_y > 11 && ss_y < 640 && ss_x > 1238 && ss_x < 1823) 
				{
					AreaId = 1497;
				}
			}
			// todo bat tunnel, only goes part way, but should be fine for now
		}
		// Check for Ironforge, and Gates of IronForge.. if neither skip
		else if (AreaId == 809 || m_AreaID == 1537) 
		{
			float ss_x = m_position.x;
		    float ss_y = m_position.y;
		    float ss_z = m_position.z;
			// height check
			if (ss_z > 480) 
			{
				// box with coord -5097.3, -828 -> -4570, -1349.3; and z > 480.
				if (ss_y > -1349.3 && ss_y < -828 && ss_x > -5097.3 && ss_x < -4570) 
				{
					AreaId = 1537;
				}
			}
		}
		
		else if (AreaId == 4356 || AreaId == 4357 || AreaId == 4298) // F*ck MapMgr, juste x et y, Fort d'ebene
		{
			//float ss_x = m_position.x;
		    //float ss_y = m_position.y;
		    float ss_z = m_position.z;
			if ((ss_z > 300) && (ss_z < 390)) 
			{
				    this->SetMapId(609);
					AreaId = 4281;
			}
		}
		

	}
	//
	else if (GetMapId() == 571)  // F*ck MapMgr, juste x et y, zone de Dalaran
	{
		if(AreaId == 2817 || AreaId == 4553 || AreaId == 4551 || AreaId == 4555 || AreaId == 4556) 
		{
			//float ss_x = m_position.x;
		    //float ss_y = m_position.y;
		    float ss_z = m_position.z;

			if(ss_z > 500.0) 
			{
				AreaId = 4395;
			}
		}
	}
	else if (GetMapId() == 609)  // F*ck MapMgr, juste x et y, Fort d'ebene
	{
		if (AreaId == 4356 || AreaId == 4357 || AreaId == 4298) // F*ck MapMgr, juste x et y, Fort d'ebene
		{
			//float ss_x = m_position.x;
		    //float ss_y = m_position.y;
		    float ss_z = m_position.z; 

			if (ss_z > 300) // 400) 
			{
					AreaId = 4342;
			}
		}
	}

	AreaTable * at = dbcArea.LookupEntry(AreaId);

	//Log.Notice("_EventExploration","(%s) MapId %u AreaId: %u",this->GetName(),GetMapId(),AreaId);
	if(at == 0) return;
	//Log.Notice("_EventExploration","(%s) Area  : %s",this->GetName(),at->name);

	//---------------  Debug ---------------
	/*char areaname[200];
	if(at)
	{
		snprintf(areaname,80,"Area: %s (%d)",at->name,at->AreaId);
	}
	else
	{
		sprintf(areaname, "Area inconnu");
	}
    sChatHandler.BlueSystemMessageToPlr(this,areaname);*/
	//----------------

	int offset = at->explorationFlag / 8; //32; // c'est un Byte pas un DWORD (Branruz)
	uint8 val = (uint8)(1 << (at->explorationFlag % 8)); //32)); // c'est un Byte pas un DWORD (Branruz)
	uint8 currFields = this->GetByte(PLAYER_EXPLORED_ZONES_1 + (offset / 4),(offset %4)); //GetUInt32Value(offset);
	                    

	OnActivateGoArea(GetMapId()); // Update interaction du Player a proximité d'un Go (ou l'inverse)
	UpdateSpellEffectArea();      // Update effet d'aura des Pnj/Mob a proximité du player
    //UpdateZoneMisc(this,m_AreaID,AreaId); // Update (si necessaire) du player lorsqu'il change de zone
	if(AreaId != m_AreaID) // Changement de zone, on update le player si necessaire
	{
		m_AreaID = AreaId;
	}
	
	UpdatePvPArea();                        // Update flag PvP/PvE du player

	if(GetGroup()) GetGroup()->UpdateOutOfRangePlayer(this, 128, true, NULL);
	
	// Zone update, this really should update to a parent zone if one exists.
	//  Will show correct location on your character screen, as well zoneid in DB will have correct value
	//  for any web sites that access that data.
	// test (Branruz): On laisse l'AreaId comme Id principal, on ne remonte plus pour avoir ZoneId=0
	if(at->ZoneId == 0 && (m_zoneId != AreaId)) 
	{
		ZoneUpdate(AreaId);
	}
	
	if ((at->ZoneId != 0) && (m_zoneId != at->ZoneId)) 
	{                                                     
		ZoneUpdate(at->ZoneId);                           
	}                                                     


	bool rest_on = false;
	// Check for a restable area
	if(at->category == AREAC_SANCTUARY) 
	{
		rest_on = true;
	}
    else if((at->AreaFlags & AREA_CITY_AREA) ) // Capitales seulement // || (at->AreaFlags & AREA_CITY))
	{
		
		// check faction
		if((at->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == FACTION_ALLY) || 
		   (at->category == AREAC_HORDE_TERRITORY && GetTeam() == FACTION_HORDE) )
		{
			rest_on = true;
		}
	}
	else
	{
        //second AT check for subzones.
        if(at->ZoneId)
        {
            AreaTable * at2 = dbcArea.LookupEntry(at->ZoneId);
            
			if(at2 && (at2->category == AREAC_SANCTUARY)) 
            {
		     rest_on = true;
	        }
            else if(at2 && (at2->AreaFlags & AREA_CITY_AREA ) ) // Capitales seulement // || (at->AreaFlags & AREA_CITY))
            {
                if((at2->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == FACTION_ALLY) || 
					(at2->category == AREAC_HORDE_TERRITORY && GetTeam() == FACTION_HORDE) )
				{
					rest_on = true;
				}

            }

		}
	}

	if (rest_on) // 'zzz' a mettre s'il n'y est pas
	{
		if(!m_isResting) ApplyPlayerRestState(true);
	}
	else
	{
		if(m_isResting) // On l'enleve le 'zzz' s'il y est
		{
/*
			const LocationVector & loc = GetPosition();
			if(!CollideInterface.IsIndoor(GetMapId(), loc.x, loc.y, loc.z + 2.0f)) // quel rapport avec le isResting ??
*/
				ApplyPlayerRestState(false);
		}
	}

	if( !(currFields & val) )//&& !GetTaxiState() && !m_TransporterGUID)//Unexplored Area	// bur: we dont want to explore new areas when on taxi
	{                                                                                       // brz: ben pourquoi ?
		Log.Success("[Player::_EventExploration]","%s : Decouverte de la zone %u (%s) Val: 0x%02X ( XZone: %u, Octet: %u )",
			                       this->GetName(),offset,at->name,(currFields | val),(offset / 4),(offset %4));

		this->SetByte((PLAYER_EXPLORED_ZONES_1 + (offset / 4)),(offset %4),(uint8)(currFields | val) );

		if(getLevel() < sWorld.m_levelCap)
		{
		 uint32 explore_xp = at->level * 10;
		 WorldPacket data(SMSG_EXPLORATION_EXPERIENCE, 8);
		 data << at->AreaId << explore_xp;
		 m_session->SendPacket(&data);

		 if(explore_xp) GiveXP(explore_xp, 0, false);
		}
	}
	// HearthStone
	if( !GetTaxiState() && !m_TransporterGUID )
		GetAchievementInterface()->HandleAchievementCriteriaExploreArea( at->AreaId, GetUInt32Value(offset) );

}

void Player::EventDeath()
{
	if (m_state & UF_ATTACKING)
		EventAttackStop();

	if (m_onTaxi)
		sEventMgr.RemoveEvents(this, EVENT_PLAYER_TAXI_DISMOUNT);

	if(!IS_INSTANCE(GetMapId()) && !sEventMgr.HasEvent(this,EVENT_PLAYER_FORECED_RESURECT)) //Should never be true 
		sEventMgr.AddEvent(this,&Player::EventRepopRequestedPlayer,EVENT_PLAYER_FORECED_RESURECT,PLAYER_FORCED_RESURECT_INTERVAL,1,0); //in case he forgets to release spirit (afk or something)
}

///  This function sends the message displaying the purple XP gain for the char
///  It assumes you will send out an UpdateObject packet at a later time.
void Player::GiveXP(uint32 xp, const uint64 &guid, bool allowbonus)
{
	if ( xp < 1 )
		return;

	if(getLevel() >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
		return;

	uint32 restxp = xp;

	//add reststate bonus (except for quests)
	if(m_restState == RESTSTATE_RESTED && allowbonus)
	{
		restxp = SubtractRestXP(xp);
		xp += restxp;
	}

	UpdateRestState();
	SendLogXPGain(guid,xp,restxp,guid == 0 ? true : false);

	int32 newxp = m_uint32Values[PLAYER_XP] + xp;
	int32 nextlevelxp = lvlinfo->XPToNextLevel;
	uint32 level = m_uint32Values[UNIT_FIELD_LEVEL];
	LevelInfo * li;
	bool levelup = false;

	while(newxp >= nextlevelxp && newxp > 0)
	{
		++level;
		li = objmgr.GetLevelInfo(getRace(), getClass(), level);
		newxp -= nextlevelxp;
		nextlevelxp = li->XPToNextLevel;
		levelup = true;

		if(level >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
			break;
	}

	if(level > GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
		level = GetUInt32Value(PLAYER_FIELD_MAX_LEVEL);

	if(levelup)
	{
		m_playedtime[0] = 0; //Reset the "Current level played time"

		LevelInfo * oldlevel = lvlinfo;
		lvlinfo = objmgr.GetLevelInfo(getRace(), getClass(), level);

		ApplyLevelInfo(lvlinfo, level);

		// Generate Level Info Packet and Send to client
        SendLevelupInfo(
            level,
            lvlinfo->HP - oldlevel->HP,
            lvlinfo->Mana - oldlevel->Mana,
            lvlinfo->Stat[0] - oldlevel->Stat[0],
            lvlinfo->Stat[1] - oldlevel->Stat[1],
            lvlinfo->Stat[2] - oldlevel->Stat[2],
            lvlinfo->Stat[3] - oldlevel->Stat[3],
            lvlinfo->Stat[4] - oldlevel->Stat[4]);
	
		if( getClass() == WARLOCK && GetSummon() && GetSummon()->IsInWorld() && GetSummon()->isAlive())
		{
			GetSummon()->ModUnsigned32Value( UNIT_FIELD_LEVEL, 1 );
			GetSummon()->ApplyStatsForLevel();
		}
		InitGlyphsForLevel();

		_UpdateMaxSkillCounts();

        GetAchievementInterface()->HandleAchievementCriteriaLevelUp( getLevel() );
	}

	// Set the update bit
	SetUInt32Value(PLAYER_XP, newxp);
	
	HandleProc(PROC_ON_XP_OR_HONOR_KILL, this, NULL);
	m_procCounter = 0;
}

void Player::smsg_InitialSpells()
{
	PlayerCooldownMap::iterator itr, itr2;

	uint16 spellCount = (uint16)mSpells.size();
	size_t itemCount = m_cooldownMap[0].size() + m_cooldownMap[1].size();
	uint32 mstime = getMSTime();
	size_t pos;

	WorldPacket data(SMSG_INITIAL_SPELLS, 5 + (spellCount * 4) + (itemCount * 4) );
	data << uint8(0);
	data << uint16(spellCount); // spell count

	SpellSet::iterator sitr;
	for (sitr = mSpells.begin(); sitr != mSpells.end(); ++sitr)
	{
		// todo: check out when we should send 0x0 and when we should send 0xeeee
		// this is not slot,values is always eeee or 0,seems to be cooldown
		data << uint32(*sitr);				   // spell id
		data << uint16(0x0000);	 
	}

	pos = data.wpos();
	data << uint16( 0 );		// placeholder

	itemCount = 0;
	for( itr = m_cooldownMap[COOLDOWN_TYPE_SPELL].begin(); itr != m_cooldownMap[COOLDOWN_TYPE_SPELL].end(); )
	{
		itr2 = itr++;

		// don't keep around expired cooldowns
		if( itr2->second.ExpireTime < mstime || (itr2->second.ExpireTime - mstime) < 10000 )
		{
			m_cooldownMap[COOLDOWN_TYPE_SPELL].erase( itr2 );
			continue;
		}

		data << uint32( itr2->first );						// spell id
		data << uint16( itr2->second.ItemId );				// item id
		data << uint16( 0 );								// spell category
		data << uint32( itr2->second.ExpireTime - mstime );	// cooldown remaining in ms (for spell)
		data << uint32( 0 );								// cooldown remaining in ms (for category)

		++itemCount;

#ifdef _DEBUG
		Log.Debug("InitialSpells", "sending spell cooldown for spell %u to %u ms", itr2->first, itr2->second.ExpireTime - mstime);
#endif
	}

	for( itr = m_cooldownMap[COOLDOWN_TYPE_CATEGORY].begin(); itr != m_cooldownMap[COOLDOWN_TYPE_CATEGORY].end(); )
	{
		itr2 = itr++;

		// don't keep around expired cooldowns
		if( itr2->second.ExpireTime < mstime || (itr2->second.ExpireTime - mstime) < 10000 )
		{
			m_cooldownMap[COOLDOWN_TYPE_CATEGORY].erase( itr2 );
			continue;
		}

		data << uint32( itr2->second.SpellId );				// spell id
		data << uint16( itr2->second.ItemId );				// item id
		data << uint16( itr2->first );						// spell category
		data << uint32( 0 );								// cooldown remaining in ms (for spell)
		data << uint32( itr2->second.ExpireTime - mstime );	// cooldown remaining in ms (for category)

		++itemCount;

#ifdef _DEBUG
		Log.Debug("InitialSpells", "sending category cooldown for cat %u to %u ms", itr2->first, itr2->second.ExpireTime - mstime);
#endif
	}

	*(uint16*)&data.contents()[pos] = (uint16)itemCount;

	GetSession()->SendPacket(&data);

	// Est-ce encore necessaire ??, aucune trace sur Offi - Test (Branruz)
	//uint32 v = 0;
	//GetSession()->OutPacket(0x041d, 4, &v); // SMSG_SERVER_BUCK_DATA

	//Log::getSingleton( ).outDetail( "CHARACTER: Sent Initial Spells" );
}

void Player::BuildFullTalentsInfo(WorldPacket *data, bool self)
{
	*data << uint32(GetUInt32Value(PLAYER_CHARACTER_POINTS1)); // Unspent talents
	// TODO: probably shouldn't send both specs if target is not self
	*data << uint8(m_talentSpecsCount);
	*data << uint8(m_talentActiveSpec);
	for(uint8 s = 0; s < m_talentSpecsCount; s++)
	{
		PlayerSpec spec = m_specs[s];
		// Send Talents
		*data << uint8(spec.talents.size());
		std::map<uint32, uint8>::iterator itr;
		for(itr = spec.talents.begin(); itr != spec.talents.end(); itr++)
		{
			*data << uint32(itr->first);	// TalentId
			*data << uint8(itr->second);	// TalentRank
		}
		if(self)
		{
			// Send Glyph info
			*data << uint8(GLYPHS_COUNT);
			for(uint8 i = 0; i < GLYPHS_COUNT; i++)
			{
				*data << uint16(spec.glyphs[i]);
			}
		} else
		{
			*data << uint8(0);	// glyphs not sent when inspecting another player
		}
	}
}

void Player::BuildPetTalentsInfo(WorldPacket *data)
{
	Pet * pPet = NULL;
	pPet = GetSummon();
	if(pPet == NULL || !pPet->IsPet())
		return;

	*data << uint32(pPet->GetUnspentPetTalentPoints());
	*data << uint8(pPet->m_talents.size());

	for(PetTalentMap::iterator itr = pPet->m_talents.begin(); itr != pPet->m_talents.end(); ++itr)
	{
		*data << uint32(itr->first);
		*data << uint8(itr->second);
	}
}

/*void Player::smsg_TalentsInfo(bool update, uint32 newTalentId, uint8 newTalentRank)
{
	WorldPacket data(SMSG_TALENTS_INFO, 1000);
	update = false;
	data << uint8(update);
	if(update)	// send just the update
	{
		uint8 count = 1;
		data << uint32(GetUInt32Value(PLAYER_CHARACTER_POINTS1)); // Unspent talents
		data << uint8(count);
		for(uint8 i = 0; i < count; i++)
		{
			data << uint32(newTalentId); // unk	talentid?
			data << uint8(1); // unk	rank?
		}
	} else	// initialize sending all info
	{
		BuildFullTalentsInfo(&data, true);
	}
	GetSession()->SendPacket(&data);
}*/

void Player::smsg_TalentsInfo(bool pet)
{
	WorldPacket data(SMSG_TALENTS_INFO, 1000);
	data << uint8(pet?1:0);
	if(pet)
		BuildPetTalentsInfo(&data);
	else	// initialize sending all info
		BuildFullTalentsInfo(&data, true);

	GetSession()->SendPacket(&data);
}

void Player::_SavePet(QueryBuffer * buf)
{
	if(!m_Summon) return;

	if(m_Summon->IsPNJ()) return; // Pas de sauvegarde pour un Bot, juste la pour test ou fun

	// Remove any existing info
	if(buf == NULL)
	{
	 CharacterDatabase.Execute("DELETE FROM playerpets WHERE ownerguid=%u", GetUInt32Value(OBJECT_FIELD_GUID));
	 CharacterDatabase.Execute("DELETE FROM playerpetactionbar WHERE ownerguid=%u", GetLowGUID());
	}
	else
	{
	 buf->AddQuery("DELETE FROM playerpets WHERE ownerguid=%u", GetUInt32Value(OBJECT_FIELD_GUID));
	 buf->AddQuery("DELETE FROM playerpetactionbar WHERE ownerguid=%u", GetLowGUID());
	}

	if(m_Summon->IsInWorld()&&m_Summon->GetPetOwner()==this)	// update PlayerPets array with current pet's info
	{
		PlayerPet*pPet = GetPlayerPet(m_Summon->m_PetNumber);
		if(!pPet || pPet->active == false) m_Summon->UpdatePetInfo(true);
		else                               m_Summon->UpdatePetInfo(false);

		if(!m_Summon->Summon)	   // is a pet
		{
			// save pet spells
			PetSpellMap::iterator itr = m_Summon->mSpells.begin();
			uint32 pn = m_Summon->m_PetNumber;
			if(buf == NULL)
				CharacterDatabase.Execute("DELETE FROM playerpetspells WHERE petnumber=%u AND ownerguid=%u", pn, GetLowGUID());
			else
				buf->AddQuery("DELETE FROM playerpetspells WHERE petnumber=%u AND ownerguid=%u", pn, GetLowGUID());

			for(; itr != m_Summon->mSpells.end(); ++itr)
			{
				if(buf == NULL)
					CharacterDatabase.Execute("INSERT INTO playerpetspells VALUES(%u, %u, %u, %u)", GetLowGUID(), pn, itr->first->Id, itr->second);
				else
					buf->AddQuery("INSERT INTO playerpetspells VALUES(%u, %u, %u, %u)", GetLowGUID(), pn, itr->first->Id, itr->second);
			}
			if(buf == NULL)
				CharacterDatabase.Execute("DELETE FROM playerpettalents WHERE petnumber=%u AND ownerguid=%u", pn, GetLowGUID());
			else
				buf->AddQuery("DELETE FROM playerpettalents WHERE petnumber=%u AND ownerguid=%u", pn, GetLowGUID());

			PetTalentMap::iterator itr2 = m_Summon->m_talents.begin();
			for(; itr2 != m_Summon->m_talents.end(); ++itr2)
			{
				if(buf == NULL)
					CharacterDatabase.Execute("INSERT INTO playerpettalents VALUES(%u, %u, %u, %u)", GetLowGUID(), pn, itr2->first, itr2->second);
				else
					buf->AddQuery("INSERT INTO playerpettalents VALUES(%u, %u, %u, %u)", GetLowGUID(), pn, itr2->first, itr2->second);
			}			
		}
	}

	std::stringstream ss;

	for(std::map<uint32, PlayerPet*>::iterator itr = m_Pets.begin(); itr != m_Pets.end(); itr++)
	{
		if(itr->second == NULL) 
		{
			Log.Error("_SavePet","PlayerPets: PetNumber %u n'existe plus dans la base <--- Prevenir l'Admin du serveur",itr->first);
			Log.Error("        ","PlayerPets: Player %s",GetName());
			continue; // 
		}

		ss.rdbuf()->str("");
		ss << "REPLACE INTO playerpets VALUES('";
		ss << GetLowGUID() << "','";                                       // OwnerGuid
		ss << itr->second->number << "','";                                // PetNumber  
		ss << CharacterDatabase.EscapeString(itr->second->name) << "','";  // Name
		ss << itr->second->entry  << "','";                                // Entry
		ss << itr->second->fields << "','";                                // Fields
		ss << itr->second->xp     << "','";                                // Xp
		ss << (itr->second->active ?  1 : 0) + itr->second->stablestate * 10 << "','"; // Active
		ss << itr->second->level << "','";             // Level
		ss << itr->second->loyaltypts << "','";        // CurrentLoyalty = LoyaltyPts
		ss << itr->second->loyaltylvl << "','";      // LoyaltyLevel ( 1 à 6 )
		ss << itr->second->happinessupdate << "','";   // HappinessUpdate
		ss << (itr->second->summon ?  1 : 0) << "')";  // 
			
		if(buf == NULL) CharacterDatabase.ExecuteNA(ss.str().c_str());
		else            buf->AddQueryStr(ss.str());

		ss.rdbuf()->str("");

		ss << "REPLACE INTO playerpetactionbar VALUES('";
		// save action bar
		ss << GetLowGUID() << "','"
		<< itr->second->number << "','";
		for(uint8 i = 0; i < 10; ++i)
		{
			ss << itr->second->actionbarspell[i] << "','";
		}
		for(uint8 i = 0; i < 9; ++i)
		{
			ss << itr->second->actionbarspellstate[i] << "','";
		}
		ss << itr->second->actionbarspellstate[9] << "')";

		if(buf == NULL) CharacterDatabase.ExecuteNA(ss.str().c_str());
		else            buf->AddQueryStr(ss.str());
	}
}

void Player::_SavePetSpells(QueryBuffer * buf)
{	
	// Remove any existing
	if(buf == NULL)
		CharacterDatabase.Execute("DELETE FROM playersummonspells WHERE ownerguid=%u", GetLowGUID());
	else
		buf->AddQuery("DELETE FROM playersummonspells WHERE ownerguid=%u", GetLowGUID());

	// Save summon spells
	map<uint32, set<uint32> >::iterator itr = SummonSpells.begin();
	for(; itr != SummonSpells.end(); ++itr)
	{
		set<uint32>::iterator it = itr->second.begin();
		for(; it != itr->second.end(); ++it)
		{
			if(buf == NULL)
				CharacterDatabase.Execute("INSERT INTO playersummonspells VALUES(%u, %u, %u)", GetLowGUID(), itr->first, (*it));
			else
				buf->AddQuery("INSERT INTO playersummonspells VALUES(%u, %u, %u)", GetLowGUID(), itr->first, (*it));
		}
	}
}

bool Player::HasKnownTitle(uint32 TitleNumber)
{
	uint64 titlemask = 1 << TitleNumber;
	if (titlemask & GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES))
		return true;
	return false;
}

void Player::AddKnownTitle(uint32 TitleNumber)
{
	if(TitleNumber < 1 || TitleNumber > TITLE_END)
		return;  // Title doesn't exist
	uint64 TitleFlag = ((uint64)1) << TitleNumber;
	SetUInt64Value(PLAYER__FIELD_KNOWN_TITLES, GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) | TitleFlag);
}

void Player::RemoveKnownTitle(uint32 TitleNumber)
{
	if(TitleNumber < 1 || TitleNumber > TITLE_END)
		return;  // Title doesn't exist
	if(TitleNumber == GetUInt32Value(PLAYER_CHOSEN_TITLE)) // if it's the chosen title, remove it
		SetUInt32Value(PLAYER_CHOSEN_TITLE, 0);
	uint64 TitleFlag = ((uint64)1) << TitleNumber;
	SetUInt64Value(PLAYER__FIELD_KNOWN_TITLES, GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) & ~TitleFlag);
}

void Player::AddSummonSpell(uint32 Entry, uint32 SpellID)
{
	SpellEntry * sp = dbcSpell.LookupEntry(SpellID);
	map<uint32, set<uint32> >::iterator itr = SummonSpells.find(Entry);
	if(itr == SummonSpells.end())
		SummonSpells[Entry].insert(SpellID);
	else
	{
		set<uint32>::iterator it3;
		for(set<uint32>::iterator it2 = itr->second.begin(); it2 != itr->second.end();)
		{
			it3 = it2++;
			if(dbcSpell.LookupEntry(*it3)->NameHash == sp->NameHash)
				itr->second.erase(it3);
		}
		itr->second.insert(SpellID);
	}
}

void Player::RemoveSummonSpell(uint32 Entry, uint32 SpellID)
{
	map<uint32, set<uint32> >::iterator itr = SummonSpells.find(Entry);
	if(itr != SummonSpells.end())
	{
		itr->second.erase(SpellID);
		if(itr->second.size() == 0)
			SummonSpells.erase(itr);
	}
}

set<uint32>* Player::GetSummonSpells(uint32 Entry)
{
	map<uint32, set<uint32> >::iterator itr = SummonSpells.find(Entry);
	if(itr != SummonSpells.end())
	{
		return &(itr->second);
	}
	return 0;
}

void Player::_LoadPet(QueryResult * result)
{
	if(!result) return;

	do
	{
		Field *fields = result->Fetch();
		fields = result->Fetch();

		PlayerPet *pet = new PlayerPet;
		pet->number          = fields[1].GetUInt32();
		pet->name	         = fields[2].GetString();
		pet->entry           = fields[3].GetUInt32();
		pet->fields          = fields[4].GetString();
		pet->xp	             = fields[5].GetUInt32();
		pet->active          = (fields[6].GetInt8()%10 > 0) ? true : false;
		pet->stablestate     = fields[6].GetInt8() / 10.0; 
		pet->level           = fields[7].GetUInt32();
		pet->loyaltypts      = fields[8].GetUInt32(); // currentloyalty = LoyaltyPts
		pet->loyaltylvl      = fields[9].GetUInt32(); // loyaltylvl     = LoyaltyLvL
		pet->happinessupdate = fields[10].GetUInt32();
		pet->summon          = ((fields[11].GetUInt32()>0) ? true : false);

		m_Pets[pet->number] = pet;
		if(pet->active)
		{
			if(iActivePet)  // how the hell can this happen
			{
				//printf("pet warning - >1 active pet.. weird..");
			}
			else
			{
				iActivePet = pet->number;
			}
		}	   
		
		if(pet->number > m_PetNumberMax) m_PetNumberMax = pet->number;

	}while(result->NextRow());
}

void Player::_LoadPetActionBar(QueryResult * result)
{
	if(!result)
		return;

	if(!m_Pets.size())
		return;

	do
	{
		Field *fields = result->Fetch();
		PlayerPet* pet = NULL;
		uint32 number  = fields[1].GetUInt32();
		pet = m_Pets[number];
		if(!pet)
			continue;

		for(uint8 i = 0; i < 10; ++i)
		{
			pet->actionbarspell[i] = fields[2+i].GetUInt32();
		}
		for(uint8 i = 0; i < 10; ++i)
		{
			pet->actionbarspellstate[i] = fields[12+i].GetUInt32();
		}
	}while(result->NextRow());
}

void Player::SpawnPet(uint32 pet_number)
{
	std::map<uint32, PlayerPet*>::iterator itr = m_Pets.find(pet_number);
	if(itr == m_Pets.end())
	{
		sLog.outError("PET SYSTEM: %s tried to load invalid pet %d", this->GetName(), pet_number);
		return;
	}

	if( m_Summon != NULL ) // Secu (Randdrick, rev398)
	{
		m_Summon->Remove(true, true, true);
		m_Summon = NULL;
	}

	Pet *pPet = objmgr.CreatePet();
	pPet->SetInstanceID(GetInstanceID());
	pPet->LoadFromDB(this, itr->second);
	/*if( IsPvPFlagged() )
		pPet->SetPvPFlag();*/
}

void Player::_LoadPetSpells(QueryResult * result)
{
	//std::stringstream query;
	//std::map<uint32, std::list<uint32>* >::iterator itr;
	uint32 entry = 0;
	uint32 spell = 0;

	if(result)
	{
		do
		{
			Field *fields = result->Fetch();
			entry = fields[1].GetUInt32();
			spell = fields[2].GetUInt32();
			AddSummonSpell(entry, spell);
		}
		while( result->NextRow() ); 
	}
}

void Player::addSpell(uint32 spell_id)
{
	SpellSet::iterator iter = mSpells.find(spell_id);
	if(iter != mSpells.end())
		return;
		
	mSpells.insert(spell_id);
	if(IsInWorld())
		m_session->OutPacket(SMSG_LEARNED_SPELL, 4, &spell_id);

	// Check if we're a deleted spell
	iter = mDeletedSpells.find(spell_id);
	if(iter != mDeletedSpells.end())
		mDeletedSpells.erase(iter);

	// Check if we're logging in.
	if(!IsInWorld())
		return;
/*		
	SpellEntry * spell = dbcSpell.LookupEntry(spell_id);

	if((spell->attributes & ATTRIBUTES_PASSIVE || (spell->Effect[0] == SPELL_EFFECT_LEARN_SPELL ||
		spell->Effect[1] == SPELL_EFFECT_LEARN_SPELL ||
		spell->Effect[2] == SPELL_EFFECT_LEARN_SPELL) 
		&& ( (spell->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET) == 0 
		|| ( (spell->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET) && GetSummon() ) ) ))
	{
		if( spell->shapeshiftMask && !( (uint32)1 << (GetShapeShift()-1) & spell->shapeshiftMask ) )
		{
			// do nothing
		}
		else
		{
			Spell* sp = new Spell(this, spell, true, NULL);
			SpellCastTargets tgt(this);
			sp->prepare(&tgt);
		}
	}
*/	
	// Add the skill line for this spell if we don't already have it.
	skilllinespell * sk = objmgr.GetSpellSkill(spell_id);
	if(sk && !_HasSkillLine(sk->skilline))
	{
		skilllineentry * skill = dbcSkillLine.LookupEntry(sk->skilline);
		SpellEntry * spell = dbcSpell.LookupEntry(spell_id);
		uint32 max = 1;
		switch(skill->type)
		{
			case SKILL_TYPE_PROFESSION:
				max=75*((spell->RankNumber)+1);
				ModUnsigned32Value( PLAYER_CHARACTER_POINTS2, -1 ); // we are learning a proffesion, so substract a point.
				break;
			case SKILL_TYPE_SECONDARY:
				max=75*((spell->RankNumber)+1);
				break;
			case SKILL_TYPE_WEAPON:
				max=5*getLevel();
				break;
			case SKILL_TYPE_CLASS:
			case SKILL_TYPE_ARMOR:
				if(skill->id == SKILL_LOCKPICKING)
					max=5*getLevel();
				break;
		};

		_AddSkillLine(sk->skilline, 1, max);
		_UpdateMaxSkillCounts();
	}
}

//===================================================================================================================
//  Set Create Player Bits -- Sets bits required for creating a player in the updateMask.
//  Note:  Doesn't set Quest or Inventory bits
//  updateMask - the updatemask to hold the set bits
//===================================================================================================================
void Player::_SetCreateBits(UpdateMask *updateMask, Player *target) const
{
	if(target == this)
	{
		Object::_SetCreateBits(updateMask, target);
	}
	else
	{
		for(uint32 index = 0; index < m_valuesCount; index++)
		{
			if(m_uint32Values[index] != 0 && Player::m_visibleUpdateMask.GetBit(index))
				updateMask->SetBit(index);
		}
	}
}


void Player::_SetUpdateBits(UpdateMask *updateMask, Player *target) const
{
	if(target == this)
	{
		Object::_SetUpdateBits(updateMask, target);
	}
	else
	{
		Object::_SetUpdateBits(updateMask, target);
		*updateMask &= Player::m_visibleUpdateMask;
	}
}


void Player::InitVisibleUpdateBits()
{
	Player::m_visibleUpdateMask.SetCount(PLAYER_END);
	Player::m_visibleUpdateMask.SetBit(OBJECT_FIELD_GUID);
	Player::m_visibleUpdateMask.SetBit(OBJECT_FIELD_ENTRY);
	Player::m_visibleUpdateMask.SetBit(OBJECT_FIELD_TYPE);
	Player::m_visibleUpdateMask.SetBit(OBJECT_FIELD_SCALE_X);
    
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_CHARM);
    Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_CHARM + 1);
	
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_SUMMON);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_SUMMON+1);
	
    Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_CHARMEDBY + 0);
    Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_CHARMEDBY + 1);
	
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_TARGET);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_TARGET+1);
	
    Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_CHANNEL_OBJECT);
    Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_CHANNEL_OBJECT + 1);
	
    Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BYTES_0);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_HEALTH);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER1);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER2);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER3);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER4);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER5);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER6);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_POWER7);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXHEALTH);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER1);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER2);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER3);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER4);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER5);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER6);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MAXPOWER7);

	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_LEVEL);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_FACTIONTEMPLATE);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BYTES_0);
    Player::m_visibleUpdateMask.SetBit(UNIT_VIRTUAL_ITEM_SLOT_ID + 0);
    Player::m_visibleUpdateMask.SetBit(UNIT_VIRTUAL_ITEM_SLOT_ID + 1);
    Player::m_visibleUpdateMask.SetBit(UNIT_VIRTUAL_ITEM_SLOT_ID + 2);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_FLAGS);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_FLAGS_2);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_AURASTATE);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BASEATTACKTIME);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BASEATTACKTIME+1);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BOUNDINGRADIUS);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_COMBATREACH);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_DISPLAYID);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_NATIVEDISPLAYID);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MOUNTDISPLAYID);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BYTES_1);
	//Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_MOUNTDISPLAYID);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_PET_LGUID);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_PET_NAME_TIMESTAMP);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_CHANNEL_OBJECT);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_CHANNEL_OBJECT+1);
	Player::m_visibleUpdateMask.SetBit(UNIT_CHANNEL_SPELL);
	Player::m_visibleUpdateMask.SetBit(UNIT_DYNAMIC_FLAGS);
	
	Player::m_visibleUpdateMask.SetBit(UNIT_CHANNEL_SPELL);
    Player::m_visibleUpdateMask.SetBit(UNIT_MOD_CAST_SPEED);
    Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BASE_MANA);
    Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BYTES_2);
    Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_HOVERHEIGHT);
	
	Player::m_visibleUpdateMask.SetBit(PLAYER_FLAGS);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES_2);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES_3);
	Player::m_visibleUpdateMask.SetBit(PLAYER_GUILD_TIMESTAMP);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_TEAM);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_ARBITER);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_ARBITER+1);
	Player::m_visibleUpdateMask.SetBit(PLAYER_GUILDID);
	Player::m_visibleUpdateMask.SetBit(PLAYER_GUILDRANK);
	
	/*Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_HOVERHEIGHT);
	Player::m_visibleUpdateMask.SetBit(PLAYER_FLAGS);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES_2);
	Player::m_visibleUpdateMask.SetBit(PLAYER_BYTES_3);
	Player::m_visibleUpdateMask.SetBit(PLAYER_GUILD_TIMESTAMP);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_TEAM);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_ARBITER);
	Player::m_visibleUpdateMask.SetBit(PLAYER_DUEL_ARBITER+1);
	Player::m_visibleUpdateMask.SetBit(PLAYER_GUILDID);
	Player::m_visibleUpdateMask.SetBit(PLAYER_GUILDRANK);
	Player::m_visibleUpdateMask.SetBit(UNIT_FIELD_BYTES_2);*/

	/* Provoque un crash client dans certaine zone si 2 players au moins (plante aussi appear et summon)
	// Ce n'est pas les runes: A Verifier
	//runes
	Player::m_visibleUpdateMask.SetBit(PLAYER_RUNE_REGEN_1);
	Player::m_visibleUpdateMask.SetBit(PLAYER_RUNE_REGEN_1_1);
	Player::m_visibleUpdateMask.SetBit(PLAYER_RUNE_REGEN_1_2);
	Player::m_visibleUpdateMask.SetBit(PLAYER_RUNE_REGEN_1_3);
*/

	for(uint16 i = PLAYER_QUEST_LOG_1_1; i < PLAYER_QUEST_LOG_25_2; i+=4)
		Player::m_visibleUpdateMask.SetBit(i);

    for(uint16 i = 0; i < EQUIPMENT_SLOT_END; ++i)
    {
        uint32 offset = i * PLAYER_VISIBLE_ITEM_LENGTH;

        // item entry
        Player::m_visibleUpdateMask.SetBit(PLAYER_VISIBLE_ITEM_1_ENTRYID + offset);
        // enchant
        Player::m_visibleUpdateMask.SetBit(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + offset);
    }

	Player::m_visibleUpdateMask.SetBit(PLAYER_CHOSEN_TITLE);
}


void Player::DestroyForPlayer( Player *target ) const
{
	Unit::DestroyForPlayer( target );
}

#define IS_ARENA(x) ( (x) >= BATTLEGROUND_ARENA_2V2 && (x) <= BATTLEGROUND_ARENA_5V5 )

void Player::SaveToDB(bool bNewCharacter /* =false */)
{
	bool in_arena = false;
	QueryBuffer * buf = NULL;
	uint32 AccountVerified = 0;
	uint32 playerDB_acct = 0;
	// uint32 playerDB_guid = 0; Note Randdrick : ne sert à rien. Ca fera un warning de moins
	bool Bad_Acct_Id = false;

    //if(!bNewCharacter) buf = new QueryBuffer;

    AccountVerified = this->GetSession()->GetAccountId();

    if(sWorld.m_DebugPlayers)
	{
	 if(bNewCharacter)  Log.Notice("[Player::SaveToDB]","(%s) Creation 1iere sauvegarde Database en cours...",this->GetName());
	 else               Log.Notice("[Player::SaveToDB]","(%s) Sauvegarde Database en cours...",this->GetName());
	}

	if(!bNewCharacter)
	{
		buf = new QueryBuffer;

	   // Verif de l'AccountId
	   Bad_Acct_Id = false;
	   QueryResult *result = CharacterDatabase.Query("SELECT `acct` FROM `characters` WHERE `name` LIKE %c%s%c", '"',this->GetName(),'"');

	   if(result)
	   {
	    playerDB_acct = result->Fetch()[0].GetUInt32();
		if(sWorld.m_DebugPlayers) Log.Notice("[Player::SaveToDB]","(%s) Acct: %u AcctDb: %u LowGuid: %u",this->GetName(),AccountVerified,playerDB_acct,this->GetLowGUID());
	    if(AccountVerified != playerDB_acct) 
		{
			Log.Error("Player::SaveToDB","Erreur d'account (%u vs %u) sur player %u - %s",AccountVerified,playerDB_acct,this->GetLowGUID(),this->GetName());
			Log.Error("Player::SaveToDB","Mise a jour memoire avec AcctId DB sur player %u - %s",this->GetLowGUID(),this->GetName());
			AccountVerified = playerDB_acct; // Mise a jour de l'AccountId
		}
	   }
	   else 
	   {
          Log.Error("Player::SaveToDB","L'account (%u) n'existe plus dans la base (%s)",this->GetLowGUID(),this->GetName());
	      //OutputCrashLogLine("SaveToDB:L'account (%u) n'existe plus dans la base (%s)",this->GetLowGUID(),this->GetName());
	      return;
	   }

	}

	 if(Bad_Acct_Id)
	   {
	    Log.Error("Player::SaveToDB","Account (%u) erronee in game (%u - %s)",AccountVerified,this->GetLowGUID(),this->GetName());
	    //OutputCrashLogLine("SaveToDB:Account (%u) erronee in game (%u - %s)",playerDB_acct,this->GetLowGUID(),this->GetName());
	    return;
	   }
	
	if( m_bg != NULL && IS_ARENA( m_bg->GetType() ) ) in_arena = true;

	if(m_uint32Values[PLAYER_CHARACTER_POINTS2]>2)
		m_uint32Values[PLAYER_CHARACTER_POINTS2] = 2; // Available_prof_points ?? A Verif (Branruz) 
 
	//Calc played times
	uint32 playedt = (uint32)UNIXTIME - m_playedtime[2];
	m_playedtime[0] += playedt;
	m_playedtime[1] += playedt;
	m_playedtime[2] += playedt;
	
	std::stringstream ss;
	ss << "REPLACE INTO characters VALUES ("
		
	<< (uint32)this->GetLowGUID() << ", "     // guid
	<< (uint32)AccountVerified << ","         // acct

	// stat saving
	<< "'" << this->GetName() << "', " //m_name << "', "                 // Name
	<< uint32(getRace()) << ","               // Race 
	<< uint32(getClass()) << ","              // Classe 
	<< uint32(getGender()) << ",";            // Genre

	if(m_uint32Values[UNIT_FIELD_FACTIONTEMPLATE] != info->factiontemplate) // custom_faction
		ss << m_uint32Values[UNIT_FIELD_FACTIONTEMPLATE] << ",";
	else
		ss << "0,";

	ss << uint32(getLevel()) << "," 	  //Level
	<< m_uint32Values[PLAYER_XP] << ","   //Xp
	
	// dump exploration data
	<< "'";

	for(uint32 i = 0; i < PLAYER_EXPLORED_ZONES_MAX; ++i) // de 0 à 128 zones dword (Branruz)
		ss << this->GetUInt32Value(PLAYER_EXPLORED_ZONES_1 + i) << ","; // exploration_data

	ss << "','0', "; //skip saving oldstyle skills, just fill with 0

	uint32 player_flags = m_uint32Values[PLAYER_FLAGS]; 
	{
		// Remove un-needed and problematic player flags from being saved :p
		if(player_flags & PLAYER_FLAG_PARTY_LEADER)     player_flags &= ~PLAYER_FLAG_PARTY_LEADER;
		if(player_flags & PLAYER_FLAG_AFK)              player_flags &= ~PLAYER_FLAG_AFK;
		if(player_flags & PLAYER_FLAG_DND)              player_flags &= ~PLAYER_FLAG_DND;
		if(player_flags & PLAYER_FLAG_GM)               player_flags &= ~PLAYER_FLAG_GM;
        // Sur l'offi, ces flags ne sont pas reset (Branruz) PVP et PVE
    	//if(player_flags & PLAYER_FLAG_PVP_TOGGLE)       player_flags &= ~PLAYER_FLAG_PVP_TOGGLE;
		//if(player_flags & PLAYER_FLAG_FREE_FOR_ALL_PVP) player_flags &= ~PLAYER_FLAG_FREE_FOR_ALL_PVP;
		//if(player_flags & PLAYER_FLAG_PVP_TIMER)        player_flags &= ~PLAYER_FLAG_PVP_TIMER;
	}

	ss << m_uint32Values[PLAYER_FIELD_WATCHED_FACTION_INDEX] << "," // watched_faction_index
	<< m_uint32Values[PLAYER_CHOSEN_TITLE] << ","                   // selected_pvp_title
	<< GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES) << ","            // available_pvp_titles1  
	<< GetUInt64Value(PLAYER__FIELD_KNOWN_TITLES1) << ","           // available_pvp_titles1  
	<< m_uint32Values[PLAYER_FIELD_COINAGE] << ","                  // gold
	<< m_uint32Values[PLAYER_AMMO_ID] << ","                        // ammo_id
	<< m_uint32Values[PLAYER_CHARACTER_POINTS2] << ","              // available_prof_point
	<< m_maxTalentPoints << ","                                     // total_talent_points
	<< load_health << ","                                           // current_hp
	<< load_mana << ","                                             // current_power
	<< uint32(GetPVPRank()) << ","                                  // pvprank
	<< m_uint32Values[PLAYER_BYTES] << ","                          // bytes
	<< m_uint32Values[PLAYER_BYTES_2] << ","                        // bytes2
	<< player_flags << ","                                          // player_flags
	<< m_uint32Values[PLAYER_FIELD_BYTES] << ",";                   // player

	if( in_arena )
	{
		// if its an arena, save the entry coords instead
		ss << m_bgEntryPointX << ", ";     // PositionX
		ss << m_bgEntryPointY << ", ";     // PositionY
		ss << m_bgEntryPointZ << ", ";     // PositionZ
		ss << m_bgEntryPointO << ", ";     // orientation
		ss << m_bgEntryPointMap << ", ";   // mapId
	}
	else
	{
		// save the normal position
		ss << m_position.x << ", "
			<< m_position.y << ", "
			<< m_position.z << ", "
			<< m_position.o << ", "
			<< m_mapId << ", ";
	}

	ss << m_zoneId << ", '";              // ZoneId
		
	for(uint32 i = 0; i < NUM_TAXI_FIELDS; i++ )
		ss << m_taximask[i] << " ";       // Taximask
	ss << "', ";
	
	ss << m_banned << ", '";              // banned
	ss << CharacterDatabase.EscapeString(m_banreason) << "', "; // banReason
	ss << (uint32)UNIXTIME << ",";           // timestamp
	
	//online state
	if(GetSession()->_loggingOut || bNewCharacter)
	{
		ss << "0,";
	}else
	{
		ss << "1,";                    // Online
	}

	ss << m_bind_pos_x	<< ", ";      // bindpositionX
	ss << m_bind_pos_y	<< ", ";      // bindpositionY
	ss << m_bind_pos_z	<< ", ";      // bindpositionZ
	ss << m_bind_mapid	<< ", ";      // bindMapId
	ss << m_bind_zoneid	<< ", ";      // bindZoneId
		
	ss << uint32(m_isResting)  << ", "; // isResting
	ss << uint32(m_restState)  << ", "; // restState
	ss << uint32(m_restAmount) << ", '"; // restTime
	  
	ss << uint32(m_playedtime[0]) << " ";  // playedTime
	ss << uint32(m_playedtime[1]) << " ";
	ss << uint32(playedt)		  << " ', ";
	ss << uint32(m_deathState)	  << ", ";   // deathState

	ss << m_talentresettimes << ", "; // TalentresetTime
	ss << m_FirstLogin		 << ", "; // first_login
	ss << rename_pending     << "," ; // forced_rename_pending
	ss << m_arenaPoints      << ",";  // arenaPoints
	ss << (uint32)m_StableSlotCount << ","; //totalstableslot
	
	// instances
	if( in_arena )
	{
		ss << m_bgEntryPointInstance << ", ";
	}
	else
	{
		ss << m_instanceId		   << ", ";    // instanceId
	}

	ss << m_bgEntryPointMap	  << ", "      // EntryPointMap
	<< m_bgEntryPointX		<< ", "        // entrypointX
	<< m_bgEntryPointY		<< ", "        // entrypointY
	<< m_bgEntryPointZ		<< ", "        // entrypointZ
	<< m_bgEntryPointO		<< ", "        // entrypointO
	<< m_bgEntryPointInstance << ", ";     // entryPointInstance

	// taxi
	if(m_onTaxi&&m_CurrentTaxiPath) 
	{
		ss << m_CurrentTaxiPath->GetID() << ", ";         // taxi_path
		ss << lastNode << ", ";                           // taxi_lastnode
		ss << GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID);  // taxi_mountId
	} 
	else 
	{
		ss << "0, 0, 0";
	}
	
	ss << "," << (m_CurrentTransporter ? m_CurrentTransporter->GetEntry() : (uint32)0); // transporter
	ss << ",'" << m_TransporterX << "','"; // transporter_xdiff
	ss << m_TransporterY << "','";         // transporter_ydiff
	ss << m_TransporterZ << "'";           // transporter_zdiff
	ss << ",'";

	// Dump deleted spell data to stringstream
	SpellSet::iterator spellItr;
	spellItr = mDeletedSpells.begin();
	for(; spellItr != mDeletedSpells.end(); ++spellItr)
	{
		ss << uint32(*spellItr) << ",";                     // deleted_spells
	}

	ss << "','";
	// Dump reputation data
	ReputationMap::iterator iter = m_reputation.begin();
	for(; iter != m_reputation.end(); ++iter)
	{
		ss << int32(iter->first) << "," ;                     // reputation
		ss << int32(iter->second->flag) << "," ;
		ss << int32(iter->second->baseStanding) << ","; 
		ss << int32(iter->second->standing) << ",";
	}
	ss << "','";
	
	// Add player action bars
	for(uint32 i = 0; i < PLAYER_ACTION_BUTTON_COUNT; ++i)
	{
		ss << uint32(mActions[i].Action) << ","           // actions
			<< uint32(mActions[i].Type) << ","
			<< uint32(mActions[i].Misc) << ",";
	}
	ss << "','";

	if(!bNewCharacter) SaveAuras(ss);         // Auras

	//ss << LoadAuras;
	ss << "','";

	// Add player finished quests
	set<uint32>::iterator fq = m_finishedQuests.begin();
	for(; fq != m_finishedQuests.end(); ++fq)
	{
		ss << (*fq) << ",";                       // finished_quests
	}

	ss << "','";
	// Add player finished daily quests
	fq = m_finishedDailyQuests.begin();
	for(; fq != m_finishedDailyQuests.end(); ++fq)
	{
		ss << (*fq) << ",";                      // finished_daily_quests
	}

	ss << "', ";
	ss << m_honorRolloverTime << ", ";
	ss << m_killsToday << ", " << m_killsYesterday << ", " << m_killsLifetime << ", ";
	ss << m_honorToday << ", " << m_honorYesterday << ", ";
	ss << m_honorPoints << ", ";
   	ss << iInstanceType << ", "; // Difficulty
    ss << (float)Taille << ", "; // Taille du player (Gestion des collisions)
	ss << m_Phase << ", ";
	ss << uint32(m_talentActiveSpec) << ", ";
	ss << uint32(m_talentSpecsCount) << ", ";

	ss << "0)";	// force_reset_talents
	
	if(bNewCharacter)
		CharacterDatabase.WaitExecuteNA(ss.str().c_str());
	else
		buf->AddQueryStr(ss.str());

	//Save Other related player stuff

	sHookInterface.OnPlayerSaveToDB(this, buf);

	// Skills
	_SaveSkillsToDB(buf);

	// Talents
	_SaveTalentsToDB(buf);

	// Spells
	_SaveSpellsToDB(buf);

	// Glyphs
	_SaveGlyphsToDB(buf);

	// Inventory
	 GetItemInterface()->mSaveItemsToDatabase(bNewCharacter, buf);

	// save quest progress
	_SaveQuestLogEntry(buf);

	// Tutorials
	_SaveTutorials(buf);

	// GM Ticket
	objmgr.SaveGMTicket(GetGUID(), buf);

	// Cooldown Items
	_SavePlayerCooldowns( buf );
	
	// Pets
	if(getClass() == HUNTER || getClass() == WARLOCK ||(getClass()==WARRIOR && sWorld.m_WarriorPetAllowed))
	{
		_SavePet(buf);
		_SavePetSpells(buf);
	}

    // Achievements (HearthStone)
	GetAchievementInterface()->SaveToDB( buf );

	m_nextSave = getMSTime() + sWorld.getIntRate(INTRATE_SAVE);

	if(buf)
		CharacterDatabase.AddQueryBuffer(buf);

	if(sWorld.m_DebugPlayers)
	{
	 Log.Notice("[Player::SaveToDB]","(%s) Sauvegarde Database Ok.",this->GetName());
	}
}

void Player::_SaveSkillsToDB(QueryBuffer * buf)
{
	// if we have nothing to save why save?
	if (m_skills.size() == 0)
		return;

	if(buf == NULL)
		CharacterDatabase.Execute("DELETE FROM playerskills WHERE Player_Guid = %u", GetLowGUID() );
	else
		buf->AddQuery("DELETE FROM playerskills WHERE Player_Guid = %u", GetLowGUID() );

	std::stringstream ss;
	ss << "INSERT INTO playerskills (Player_Guid, skill_id, type, currentlvl, maxlvl ) VALUES ";
	uint32 iI = ((uint32)m_skills.size())-1;
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end() ; ++itr)
	{
		if(itr->first)
		{
			ss	<< "(" << GetLowGUID() << ","
				<< itr->first << ","
				<< itr->second.Skill->type << ","
				<< itr->second.CurrentValue << ","
				<< itr->second.MaximumValue << ")";
			if (iI > 0)
				ss << ",";
		}
		iI -= 1;
	}

	if(buf == NULL)
		CharacterDatabase.Execute(ss.str().c_str());
	else
		buf->AddQueryStr(ss.str());
}

void Player::_LoadGlyphs(QueryResult * result)
{
	// init with 0s just in case
	for(uint8 s = 0; s < MAX_SPEC_COUNT; s++)
	{
		for(uint32 i=0; i < GLYPHS_COUNT; i++)
		{
			m_specs[s].glyphs[i] = 0;
		}
	}
	// Load info from DB
	if(result)
	{
		do 
		{
			Field *fields = result->Fetch();
			uint8 spec = fields[1].GetInt8();
			if(spec >= MAX_SPEC_COUNT)
			{
				sLog.outDebug("Out of range spec number [%d] for player with GUID [%d] in playerglyphs", 
					spec, fields[0].GetUInt32());
				continue;
			}
			for(uint32 i=0; i < GLYPHS_COUNT; i++)
			{
				m_specs[spec].glyphs[i] = fields[2 + i].GetUInt16();
			}
		} while(result->NextRow());
	}
}

void Player::_SaveGlyphsToDB(QueryBuffer * buf)
{
	bool empty = true;
	for(uint8 s = 0; s < m_talentSpecsCount; s++)
	{
		for(uint32 i=0; i < GLYPHS_COUNT; i++)
		{
			if(m_specs[s].glyphs[i] != 0)
			{
				empty = false;
				break;
			}
		}
	}
	if(empty)
		return;	// nothing to save

	for(uint8 s = 0; s < m_talentSpecsCount; s++)
	{
		std::stringstream ss;
		ss << "REPLACE INTO playerglyphs (guid, spec, glyph1, glyph2, glyph3, glyph4, glyph5, glyph6) VALUES "
			<< "(" << GetLowGUID() << ","
			<< uint32(s) << ",";
		for(uint32 i = 0; i < GLYPHS_COUNT; i++)
		{
			ss << uint32(m_specs[s].glyphs[i]);
			if(i != GLYPHS_COUNT - 1)
				ss << ",";
			else
				ss << ")";
		}

		if(buf == NULL)
			CharacterDatabase.Execute(ss.str().c_str());
		else
			buf->AddQueryStr(ss.str());
	}
}

void Player::_LoadSpells(QueryResult * result)
{
	if(result)
	{
		do 
		{
			Field *fields = result->Fetch();
			SpellEntry * spProto = dbcSpell.LookupEntryForced(fields[0].GetInt32());
			if(spProto)
				mSpells.insert(spProto->Id);
		} while(result->NextRow());
	}
}

void Player::_SaveSpellsToDB(QueryBuffer * buf)
{
	// delete old first
	if(buf == NULL)
		CharacterDatabase.Execute("DELETE FROM playerspells WHERE guid = %u", GetLowGUID() );
	else
		buf->AddQuery("DELETE FROM playerspells WHERE guid = %u", GetLowGUID() );

	// Dump spell data to stringstream
	std::stringstream ss;
	ss << "INSERT INTO playerspells (guid, spellid) VALUES ";
	SpellSet::iterator spellItr = mSpells.begin();
	bool first = true;
	for(; spellItr != mSpells.end(); ++spellItr)
	{
		SpellEntry * sp = dbcSpell.LookupEntry( *spellItr );
		if( !sp || sp->shapeshiftMask == FORM_ZOMBIE )
			continue;
		if(!first)
			ss << ",";
		else
			first = false;
		ss << "("<< GetLowGUID() << "," << uint32(*spellItr) << ")";
	}
	if(buf == NULL)
		CharacterDatabase.Execute(ss.str().c_str());
	else
		buf->AddQueryStr(ss.str());
}

void Player::_LoadTalents(QueryResult * result)
{
	// Load info from DB
	uint32 talentId;
	uint8 talentRank;
	if(result)
	{
		do 
		{
			Field *fields = result->Fetch();
			uint8 spec = fields[0].GetInt8();
			if(spec >= MAX_SPEC_COUNT)
			{
				sLog.outDebug("Out of range spec number [%d] for player with GUID [%d] in playertalents", 
					spec, GetLowGUID());
				continue;
			}
			talentId = fields[1].GetUInt32();
			talentRank = fields[2].GetUInt8();
			m_specs[spec].talents.insert(make_pair(talentId, talentRank));
		} while(result->NextRow());
	}
}

void Player::_SaveTalentsToDB(QueryBuffer * buf)
{
	// delete old talents first
	if(buf == NULL)
		CharacterDatabase.Execute("DELETE FROM playertalents WHERE guid = %u", GetLowGUID() );
	else
		buf->AddQuery("DELETE FROM playertalents WHERE guid = %u", GetLowGUID() );

	for(uint8 s = 0; s < m_talentSpecsCount; s++)
	{
		if(s > MAX_SPEC_COUNT)
			break;
		std::map<uint32, uint8> *talents = &m_specs[s].talents;
		std::map<uint32, uint8>::iterator itr;
		for(itr = talents->begin(); itr != talents->end(); itr++)
		{
			std::stringstream ss;
			ss << "INSERT INTO playertalents (guid, spec, tid, rank) VALUES "
				<< "(" << GetLowGUID() << "," 
				<< uint32(s) << "," 
				<< itr->first << ","
				<< uint32(itr->second) << ")";
			if(buf == NULL)
				CharacterDatabase.Execute(ss.str().c_str());
			else
				buf->AddQueryStr(ss.str());
		}
	}
}


void Player::_SaveQuestLogEntry(QueryBuffer * buf)
{
	for(std::set<uint32>::iterator itr = m_removequests.begin(); itr != m_removequests.end(); ++itr)
	{
		if(buf == NULL)
			CharacterDatabase.Execute("DELETE FROM questlog WHERE player_guid=%u AND quest_id=%u", GetLowGUID(), (*itr));
		else
			buf->AddQuery("DELETE FROM questlog WHERE player_guid=%u AND quest_id=%u", GetLowGUID(), (*itr));
	}

	m_removequests.clear();

	for(int i = 0; i < 25; ++i)
	{
		if(m_questlog[i] != NULL)
			m_questlog[i]->SaveToDB(buf);
	}
}

bool Player::canCast(SpellEntry *m_spellInfo)
{
	if (m_spellInfo->EquippedItemClass != 0)
	{
		if( disarmed )
			return false;

		if(this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND))
		{
			if((int32)this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND)->GetProto()->Class == m_spellInfo->EquippedItemClass)
			{
				if (m_spellInfo->EquippedItemSubClass != 0)
				{
					if (m_spellInfo->EquippedItemSubClass != 173555 && 
						m_spellInfo->EquippedItemSubClass != 96 && 
						m_spellInfo->EquippedItemSubClass != 262156)
					{
						if (pow(2.0,(this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND)->GetProto()->SubClass) != m_spellInfo->EquippedItemSubClass))
							return false;
					}
				}
			}
		}
		else if(m_spellInfo->EquippedItemSubClass == 173555)
			return false;

		if (this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED))
		{
			if((int32)this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED)->GetProto()->Class == m_spellInfo->EquippedItemClass)
			{
				if (m_spellInfo->EquippedItemSubClass != 0)
				{
					if (m_spellInfo->EquippedItemSubClass != 173555 && m_spellInfo->EquippedItemSubClass != 96 && m_spellInfo->EquippedItemSubClass != 262156)
					{
						if (pow(2.0,(this->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED)->GetProto()->SubClass) != m_spellInfo->EquippedItemSubClass))							return false;
					}
				}
			}
		}
		else if
			(m_spellInfo->EquippedItemSubClass == 262156)
			return false;
	}
	return true;
}

void Player::RemovePendingPlayer()
{
	if(m_session)
	{
		uint8 respons = 0x42;		// CHAR_LOGIN_NO_CHARACTER
		m_session->OutPacket(SMSG_CHARACTER_LOGIN_FAILED, 1, &respons);
		m_session->m_loggingInPlayer = NULL;
	}

	ok_to_remove = true;
	delete this;
}

bool Player::LoadFromDB(uint32 guid)
{
	// Attention, l'ordre du spool est important
/*Fetch*/ AsyncQuery * q = new AsyncQuery( new SQLClassCallbackP0<Player>(this, &Player::LoadFromDBProc) );
/*[ 0]*/  q->AddQuery("SELECT * FROM characters WHERE guid=%u AND forced_rename_pending = 0",guid);
/*[ 1]*/  q->AddQuery("SELECT * FROM tutorials WHERE playerId=%u",guid);
/*[ 2]*/  q->AddQuery("SELECT cooldown_type, cooldown_misc, cooldown_expire_time, cooldown_spellid, cooldown_itemid FROM playercooldowns WHERE player_guid=%u", guid);
/*[ 3]*/  q->AddQuery("SELECT * FROM questlog WHERE player_guid=%u",guid);
/*[ 4]*/  q->AddQuery("SELECT * FROM playeritems WHERE ownerguid=%u ORDER BY containerslot ASC", guid);
/*[ 5]*/  q->AddQuery("SELECT * FROM playerpets WHERE ownerguid=%u ORDER BY petnumber", guid);
/*[ 6]*/  q->AddQuery("SELECT * FROM playersummonspells where ownerguid=%u ORDER BY entryid", guid);
/*[ 7]*/  q->AddQuery("SELECT * FROM mailbox WHERE player_guid = %u", guid);

	// social
/*[ 8]*/  q->AddQuery("SELECT friend_guid, note FROM social_friends WHERE character_guid = %u", guid);
/*[ 9]*/  q->AddQuery("SELECT character_guid FROM social_friends WHERE friend_guid = %u", guid);
/*[10]*/  q->AddQuery("SELECT ignore_guid FROM social_ignores WHERE character_guid = %u", guid);

	//Achievements
/*[11]*/   q->AddQuery("SELECT * from achievements WHERE player = %u", guid); 

	//skills
/*[12]*/  q->AddQuery("SELECT * FROM playerskills WHERE player_guid = %u AND type <> %u ORDER BY skill_id ASC, currentlvl DESC", guid,SKILL_TYPE_LANGUAGE ); //load skill, skip languages

	//pet action bar
/*[13]*/  q->AddQuery("SELECT * FROM playerpetactionbar WHERE ownerguid=%u ORDER BY petnumber", guid);

	//Talents
/*[14]*/  q->AddQuery("SELECT spec, tid, rank FROM playertalents WHERE guid = %u", guid);
	
	//Glyphs
/*[15]*/  q->AddQuery("SELECT * FROM playerglyphs WHERE guid = %u", guid);

	//Spells
/*[16]*/  q->AddQuery("SELECT spellid FROM playerspells WHERE guid = %u", guid);

	// queue it!
	m_uint32Values[OBJECT_FIELD_GUID] = guid;
	CharacterDatabase.QueueAsyncQuery(q);
	return true;
}

void Player::LoadFromDBProc(QueryResultVector & results)
{
	char *waadptr;
	uint32 field_index = 2;
#define get_next_field fields[field_index++]

	// set playerinfo
	m_playerInfo = objmgr.GetPlayerInfo(GetLowGUID());
	if( m_playerInfo == NULL )
	{
		Log.Error("Player","Pas d'information sur le personnage. Connexion annulée.");
		RemovePendingPlayer();
		return;
	}

	if(GetSession() == NULL || results.size() < 8)		// should have 8 query results for aplayer load.
	{
		Log.Error("Player","Pas d'information sur la session. Connexion annulée");
		RemovePendingPlayer();
		return;
	}

	QueryResult *result = results[0].result;
	if(!result)
	{
		sLog.outError("Player login query failed., guid %u\n", m_playerInfo);
		RemovePendingPlayer();
		return;
	}

	Field *fields = result->Fetch();

	if(fields[1].GetUInt32() != m_session->GetAccountId())
	{
		sCheatLog.writefromsession(m_session, "player tried to load character not belonging to them (guid %u, on account %u)",
			fields[0].GetUInt32(), fields[1].GetUInt32());
		RemovePendingPlayer();
		return;
	}

	uint32 banned = fields[33].GetUInt32();
	if(banned && (banned < 100 || banned > (uint32)UNIXTIME))
	{
		sLog.outDetail("Player","Le joueur est banni");
		RemovePendingPlayer();
		return;
	}

	// Load name
	m_name = get_next_field.GetString();
   
	// Load race/class from fields
	setRace(get_next_field.GetUInt8());
	setClass(get_next_field.GetUInt8());
	setGender(get_next_field.GetUInt8());
	uint32 cfaction = get_next_field.GetUInt32();
	
	// set race dbc
	myRace = dbcCharRace.LookupEntryForced(getRace());
	myClass = dbcCharClass.LookupEntryForced(getClass());
	if(!myClass || !myRace)
	{
		// bad character
		sLog.outError("guid %u failed to login, no race or class dbc found. (race %u class %u)\n", (unsigned int)GetLowGUID(), (unsigned int)getRace(), (unsigned int)getClass());
		RemovePendingPlayer();
		return;
	}

	if(myRace->TeamId == 0) // 332.11403 (chrRaces.dbc)
	{
		m_bgTeam = m_team = FACTION_ALLY;
	}
	else
	{
		m_bgTeam = m_team = FACTION_HORDE;
	}

	SetNoseLevel();

	// set power type
	SetPowerType(myClass->power_type);

	// obtain player create info
	info = objmgr.GetPlayerCreateInfo(getRace(), getClass());
	//assert(info); // Grrrr Encore un Assert (Branruz)
	if(!info)
	{
		Log.Error("Player::LoadFromDBProc","Info player %s non trouvé!!",this->GetName());
		RemovePendingPlayer();
		return;
	}

	// set level
	m_uint32Values[UNIT_FIELD_LEVEL] = get_next_field.GetUInt32();
	if(m_uint32Values[UNIT_FIELD_LEVEL] > sWorld.m_levelCap)
		m_uint32Values[UNIT_FIELD_LEVEL] = sWorld.m_levelCap ; 

	// level dependant taxi node
	if(m_uint32Values[UNIT_FIELD_LEVEL]>=68)
		SetTaximaskNode(213);	//Shattered Sun Staging Area

	// obtain level/stats information
	lvlinfo = objmgr.GetLevelInfo(getRace(), getClass(), getLevel());
	
	if(!lvlinfo)
	{
		Log.Error("[Player::LoadFromDBProc]","guid %u level %u class %u race %u levelinfo non trouvé!\n", (unsigned int)GetLowGUID(), (unsigned int)getLevel(), (unsigned int)getClass(), (unsigned int)getRace());
		RemovePendingPlayer();
		return;
	}
	
	CalculateBaseStats();

	// set xp
	m_uint32Values[PLAYER_XP] = get_next_field.GetUInt32();
	
	// Process exploration data.
	uint32 Counter = 0;
	char * end;
	char * start = (char*)get_next_field.GetString();//buff;
	while(Counter < PLAYER_EXPLORED_ZONES_MAX) // (Branruz)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		SetUInt32Value(PLAYER_EXPLORED_ZONES_1 + Counter, (uint32)atoi(start)); // Dans la base ce sont des DWORD, ca prend moins de place (Branruz)
		start = end +1;
		Counter++;
	}

	QueryResult *checkskills = results[12].result;
	if(checkskills)
	{
		_LoadSkills(results[12].result);
		field_index++;
		sLog.outDetail("WorldSession","Skills loaded");
	}
	else 
	{
		// old format
		Counter = 0;
		start = (char*)get_next_field.GetString();//buff old system;
		const ItemProf * prof;
		if(!strchr(start, ' ') && !strchr(start,';'))
		{
			// no skills - reset to defaults 
			for(std::list<CreateInfo_SkillStruct>::iterator ss = info->skills.begin(); ss!=info->skills.end(); ss++)
			{
				if(ss->skillid && ss->currentval && ss->maxval && !::GetSpellForLanguage(ss->skillid))
					_AddSkillLine(ss->skillid, ss->currentval, ss->maxval);		
			}
		}
		else
		{
			char * f = strdup(start);
			start = f;

			uint32 v1,v2,v3;
			PlayerSkill sk;
			for(;;)
			{
				end = strchr(start, ';');
				if(!end)
					break;

				*end = 0;
				v1 = atol(start);
				start = end + 1;

				end = strchr(start, ';');
				if(!end)
					break;

				*end = 0;
				v2 = atol(start);
				start = end + 1;

				end = strchr(start, ';');
				if(!end)
					break;

				v3 = atol(start);
				start = end + 1;

				/* add the skill */
				if(v1)
				{
					sk.Reset(v1);
					sk.CurrentValue = v2;
					sk.MaximumValue = v3;
					if (v1 == SKILL_RIDING)
						sk.CurrentValue = sk.MaximumValue;
					m_skills.insert(make_pair(v1, sk));
	
					prof = GetProficiencyBySkill(v1);
					if(prof)
					{
						if(prof->itemclass==4)
							armor_proficiency|=prof->subclass;
						else
							weapon_proficiency|=prof->subclass;
					}
				}
			}
			free(f);
			_UpdateMaxSkillCounts();
			sLog.outDetail("Player","loaded old style skills for player %s", m_name.c_str());
		}
	}


	// set the rest of the shit
	m_uint32Values[PLAYER_FIELD_WATCHED_FACTION_INDEX]  = get_next_field.GetUInt32();
	m_uint32Values[ PLAYER_CHOSEN_TITLE ]					= get_next_field.GetUInt32();
	SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES, get_next_field.GetUInt64() );
	SetUInt64Value( PLAYER__FIELD_KNOWN_TITLES1, get_next_field.GetUInt64() );
	m_uint32Values[PLAYER_FIELD_COINAGE]				= get_next_field.GetUInt32();
	m_uint32Values[PLAYER_AMMO_ID]					  = get_next_field.GetUInt32();
	m_uint32Values[PLAYER_CHARACTER_POINTS2]			= get_next_field.GetUInt32();
	m_maxTalentPoints								= get_next_field.GetUInt16();
	load_health										 = get_next_field.GetUInt32();
	load_mana										   = get_next_field.GetUInt32();

	uint8 pvprank = get_next_field.GetUInt8();
	SetUInt32Value(PLAYER_BYTES, get_next_field.GetUInt32());
	SetUInt32Value(PLAYER_BYTES_2, get_next_field.GetUInt32());
	SetUInt32Value(PLAYER_BYTES_3, getGender() | (pvprank << 24));
	SetUInt32Value(PLAYER_FLAGS, get_next_field.GetUInt32());
	SetUInt32Value(PLAYER_FIELD_BYTES, get_next_field.GetUInt32());

	m_position.x	= get_next_field.GetFloat();
	m_position.y	= get_next_field.GetFloat();
	m_position.z	= get_next_field.GetFloat();
	m_position.o	= get_next_field.GetFloat();

	m_mapId			= get_next_field.GetUInt32();
	m_zoneId		= get_next_field.GetUInt32();

	// Calculate the base stats now they're all loaded
	for(uint32 i = 0; i < STAT_MAX; ++i)
		CalcStat(i);

  //  for(uint32 x = PLAYER_SPELL_CRIT_PERCENTAGE1; x < PLAYER_SPELL_CRIT_PERCENTAGE06 + 1; ++x)
	///	SetFloatValue(x, 0.0f);

	for(uint32 x = PLAYER_FIELD_MOD_DAMAGE_DONE_PCT; x < PLAYER_FIELD_MOD_HEALING_DONE_POS; ++x)
		SetFloatValue(x, 1.0f);

	// Normal processing...
	UpdateStats();

	// Initialize 'normal' fields
	//SetFloatValue(OBJECT_FIELD_SCALE_X, ((getRace()==RACE_TAUREN)?1.3f:1.0f));
	SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
	SetUInt32Value(UNIT_FIELD_POWER2, 0); // Rage = 0
	SetUInt32Value(UNIT_FIELD_POWER3, info->focus);
	SetUInt32Value(UNIT_FIELD_POWER4, info->energy );
	//Andy: dk
	SetUInt32Value(UNIT_FIELD_POWER6, 8);
	SetUInt32Value(UNIT_FIELD_POWER7, 0); // Runic
	/*SetFloatValue(PLAYER_RUNE_REGEN_1, 0.1f);
	SetFloatValue(PLAYER_RUNE_REGEN_1_1, 0.1f);
	SetFloatValue(PLAYER_RUNE_REGEN_1_2, 0.1f);
	SetFloatValue(PLAYER_RUNE_REGEN_1_3, 0.1f);

    SetFlag(UNIT_FIELD_FLAGS_2, 0x800);*/

 
	SetUInt32Value(UNIT_FIELD_MAXPOWER2, info->rage );
	SetUInt32Value(UNIT_FIELD_MAXPOWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_MAXPOWER4, info->energy );
	//Andy: dk
	SetUInt32Value(UNIT_FIELD_MAXPOWER6, 8);
	SetUInt32Value(UNIT_FIELD_MAXPOWER7, 1000); // Runic

	if(getClass() == WARRIOR) SetShapeShift(FORM_BATTLESTANCE);

	// We're players!
	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
	SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 0.388999998569489f );
	SetFloatValue(UNIT_FIELD_COMBATREACH, 1.5f);

	if(getRace() != RACE_BLOODELF)
	{
		SetUInt32Value(UNIT_FIELD_DISPLAYID, info->displayId + getGender() );
		SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, info->displayId + getGender() );
	} else	
	{
		SetUInt32Value(UNIT_FIELD_DISPLAYID, info->displayId - getGender() );
		SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, info->displayId - getGender() );
	}

	SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
	// SetUInt32Value(PLAYER_FIELD_MAX_LEVEL, sWorld.m_levelCap);
	
	SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, info->factiontemplate);
	if(cfaction)
	{
		SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, info->factiontemplate);
		// re-calculate team
		switch(cfaction)
		{
		case 1:	// human
		case 3:	// dwarf
		case 4: // Elf de la nuit
		case 8:	// gnome
		case 927:	// dreinei
			m_team = m_bgTeam = FACTION_ALLY;
			break;

		case 914: // Elf de sang
		case   2: // Orc
		case   6: // Tauren
		case   9: // Troll
		case   5: // Mort Vivant
		default :
			m_team = m_bgTeam = FACTION_HORDE;
			break;
		}
	}
 
	LoadTaxiMask( get_next_field.GetString() );

	m_banned = get_next_field.GetUInt32(); //Character ban
	m_banreason = get_next_field.GetString();
	m_timeLogoff = get_next_field.GetUInt32();
	field_index++; //Player is Online!
	
	m_bind_pos_x = get_next_field.GetFloat();
	m_bind_pos_y = get_next_field.GetFloat();
	m_bind_pos_z = get_next_field.GetFloat();
	m_bind_mapid = get_next_field.GetUInt32();
	m_bind_zoneid = get_next_field.GetUInt32();

	m_isResting = get_next_field.GetUInt8();
	m_restState = get_next_field.GetUInt8();
	m_restAmount = get_next_field.GetUInt32();


	std::string tmpStr = get_next_field.GetString();
	char* next_token;
	//m_playedtime[0] = (uint32)atoi((const char*)strtok_s((char*)tmpStr.c_str()," ",&next_token));
	//m_playedtime[1] = (uint32)atoi((const char*)strtok_s(NULL," ",&next_token));
    waadptr = strtok_s((char*)tmpStr.c_str()," ",&next_token);
	if(waadptr)
	{
	 m_playedtime[0] = (uint32)atoi(waadptr);
     waadptr = strtok_s(NULL," ",&next_token);
	 if(waadptr) m_playedtime[1] = (uint32)atoi(waadptr);
	 else m_playedtime[1] = m_playedtime[0]+1; // Coherence
	}
	else
	{
	 m_playedtime[0] = 0;
	 m_playedtime[1] = 0;
	}
	

	m_deathState = (DeathState)get_next_field.GetUInt32();
	if(load_health && m_deathState == JUST_DIED)
	{
		m_deathState = CORPSE;
		load_health = 0;
	}
	SetUInt32Value(UNIT_FIELD_HEALTH, load_health);

	m_talentresettimes = get_next_field.GetUInt32();
	m_FirstLogin = get_next_field.GetBool();
	rename_pending = get_next_field.GetBool();
	m_arenaPoints = get_next_field.GetUInt32();
/*	for(uint32 z = 0; z < NUM_CHARTER_TYPES; ++z)
		m_charters[z] = objmgr.GetCharterByGuid(GetGUID(), (CharterTypes)z);
	for(uint32 z = 0; z < NUM_ARENA_TEAM_TYPES; ++z)
	{
		m_arenaTeams[z] = objmgr.GetArenaTeamByGuid(GetLowGUID(), z);
		if(m_arenaTeams[z] != NULL)
		{
			SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (z*7), m_arenaTeams[z]->m_id);
			if(m_arenaTeams[z]->m_leader == GetLowGUID())
				SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (z*7) + 1, 0);
			else
				SetUInt32Value(PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + (z*7) + 1, 1);
		}
	}
*/
	m_StableSlotCount = get_next_field.GetUInt32();
	m_instanceId = get_next_field.GetUInt32();
	m_bgEntryPointMap = get_next_field.GetUInt32();
	m_bgEntryPointX = get_next_field.GetFloat();
	m_bgEntryPointY = get_next_field.GetFloat();
	m_bgEntryPointZ = get_next_field.GetFloat();
	m_bgEntryPointO = get_next_field.GetFloat();
	m_bgEntryPointInstance = get_next_field.GetUInt32();	

	uint32 taxipath = get_next_field.GetUInt32();
	TaxiPath *path = NULL;
	if(taxipath)
	{
		path = sTaxiMgr.GetTaxiPath(taxipath);
		lastNode = get_next_field.GetUInt32();
		if(path)
		{
			SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, get_next_field.GetUInt32());
			SetTaxiPath(path);
			m_onTaxi = true;
		}
		else
			field_index++;
	}
	else
	{
		field_index++;
		field_index++;
	}

	m_TransporterGUID = get_next_field.GetUInt32();
	if(m_TransporterGUID)
	{
		Transporter * t = objmgr.GetTransporter(GUID_LOPART(m_TransporterGUID));
		m_TransporterGUID = t ? t->GetGUID() : 0;
	}

	m_TransporterX = get_next_field.GetFloat();
	m_TransporterY = get_next_field.GetFloat();
	m_TransporterZ = get_next_field.GetFloat();
/*	
	// Load Spells from CSV data.
	start = (char*)get_next_field.GetString();//buff;
	SpellEntry * spProto;
	while(true) 
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		//mSpells.insert(atol(start));
		spProto = dbcSpell.LookupEntryForced(atol(start));
		/*
		if (spProto)
		{
			if (spProto->Id == __Titan_s_Grip_Rank_1) // 46917 //Titan's Grip Bug fix. 
				titanGrip = true;
		}*/

//#define _language_fix_ 
/*#ifndef _language_fix_
		if(spProto)
			mSpells.insert(spProto->Id);
#else		
		if(spProto)
		{
			skilllinespell * _spell = objmgr.GetSpellSkill(spProto->Id);
			if (_spell)
			{
				skilllineentry * _skill = dbcSkillLine.LookupEntry(_spell->skilline);
				if (_skill && _skill->type != SKILL_TYPE_LANGUAGE)
				{
					mSpells.insert(spProto->Id);
				}
			}
			else
			{
				mSpells.insert(spProto->Id);
			}
		}
#endif
//#undef _language_fix_
			
		start = end +1;
	}
*/
	start = (char*)get_next_field.GetString();//buff;
	while(true) 
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		SpellEntry * spProto = NULL;
		spProto = dbcSpell.LookupEntryForced(atol(start));
		if(spProto)
			mDeletedSpells.insert(spProto->Id);
		start = end +1;
	}

	// Load Reputatation CSV Data
	start =(char*) get_next_field.GetString();
	FactionDBC * factdbc ;
	FactionReputation * rep;
	uint32 id;
	int32 basestanding;
	int32 standing;
	uint32 fflag;
	while(true) 
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		id = atol(start);
		start = end +1;

		end = strchr(start,',');
		if(!end)break;
		*end=0;
		fflag = atol(start);
		start = end +1;
		
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		basestanding = atoi(start);//atol(start);
		start = end +1;
		
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		standing  = atoi(start);// atol(start);
		start = end +1;

		// listid stuff
		factdbc = dbcFaction.LookupEntryForced(id);
		if(!factdbc) continue;
		ReputationMap::iterator rtr = m_reputation.find(id);
		if(rtr != m_reputation.end())
			delete rtr->second;

		rep = NULL;
		rep = new FactionReputation;
		rep->baseStanding = basestanding;
		rep->standing = standing;
		rep->flag = fflag;
		m_reputation[id]=rep;

		// do listid stuff
		if(factdbc->RepListId >= 0)
			reputationByListId[factdbc->RepListId] = rep;
	}

	if(!m_reputation.size())
		_InitialReputation();

	// Load saved actionbars
	start =  (char*)get_next_field.GetString();
	Counter =0;
	while(Counter < PLAYER_ACTION_BUTTON_COUNT)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		mActions[Counter].Action = (uint16)atol(start);
		start = end +1;
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		mActions[Counter].Type = (uint8)atol(start);
		start = end +1;
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		mActions[Counter++].Misc = (uint8)atol(start);
		start = end +1;
	}
	
	//LoadAuras = get_next_field.GetString();
	start = (char*)get_next_field.GetString();//buff;
	do 
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		LoginAura la;
		la.id = atol(start);
		start = end +1;
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		la.dur = atol(start);
		start = end +1;
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		la.m_flags = atol(start);
		start = end +1;		
		loginauras.push_back(la);
	} while(true);

	// Load saved finished quests

	start =  (char*)get_next_field.GetString();
	while(true)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		m_finishedQuests.insert(atol(start));
		start = end +1;
	}
	GetAchievementInterface()->HandleAchievementCriteriaQuestCount( (uint32)m_finishedQuests.size() );

	// Load saved finished daily quests
	DailyMutex.Acquire();
	start =  (char*)get_next_field.GetString();
	/*while(true)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		SetUInt32Value((uint32)PLAYER_FIELD_DAILY_QUESTS_1 + (uint32)m_finisheddailyQuests.size(), atol(start));
		m_finisheddailyQuests.insert(atol(start));
		start = end + 1;
	}
	DailyMutex.Release();

	m_honorRolloverTime = get_next_field.GetUInt32();
	m_killsToday = get_next_field.GetUInt32();
	m_killsYesterday = get_next_field.GetUInt32();
	m_killsLifetime = get_next_field.GetUInt32();
	
	m_honorToday = get_next_field.GetUInt32();
	m_honorYesterday = get_next_field.GetUInt32();
	m_honorPoints = get_next_field.GetUInt32();
	
	iInstanceType = get_next_field.GetUInt32();		

	bool reset_talents = get_next_field.GetBool();

	if (reset_talents)
		sEventMgr.AddEvent(this, &Player::Reset_Talents, EVENT_UNK, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);	

	start = (char*)get_next_field.GetString();
	
	 uint8 glyphindex = 0;
	 while(glyphindex < 6)
	 {
       if(start != NULL)
	   { 
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		SetUInt32Value(PLAYER_FIELD_GLYPHS_1 + glyphindex, atol(start));
		++glyphindex;
		start = end + 1;
	   }
	   else SetUInt32Value(PLAYER_FIELD_GLYPHS_1 + glyphindex,0);
	}
	if(start == NULL) Log.Error("Characters","Table non a jour, il manque la colonne 'glyphs'");
	
	RolloverHonor();

	HonorHandler::RecalculateHonorFields(this);
	
	for(uint32 x=0;x<5;x++)
		BaseStats[x]=GetUInt32Value(UNIT_FIELD_STAT0+x);
  
	_setFaction();
 	InitGlyphSlots();
	InitGlyphsForLevel();*/
	
	while(true)
	{
		end = strchr(start,',');
		if(!end)break;
		*end=0;
		SetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + (uint32)m_finishedDailyQuests.size(), (uint32)atol(start));
		m_finishedDailyQuests.insert(atol(start));
		start = end +1;
	}
	DailyMutex.Release();
	
	m_honorRolloverTime = get_next_field.GetUInt32();
	m_killsToday = get_next_field.GetUInt32();
	m_killsYesterday = get_next_field.GetUInt32();
	m_killsLifetime = get_next_field.GetUInt32();
	
	m_honorToday = get_next_field.GetUInt32();
	m_honorYesterday = get_next_field.GetUInt32();
	m_honorPoints = get_next_field.GetUInt32();

	RolloverHonor();
    iInstanceType = get_next_field.GetUInt32();

	Taille = get_next_field.GetUInt32(); // Taille du player (Gestion des collisions)
	m_Phase = get_next_field.GetUInt32();

	HonorHandler::RecalculateHonorFields(this);
	
	for(uint32 x=0;x<5;x++)
		BaseStats[x]=GetUInt32Value(UNIT_FIELD_STAT0+x);
  
	_setFaction();
	InitGlyphSlots();
	InitGlyphsForLevel();
	
	//class fixes
	switch(getClass())
	{
	case PALADIN:
		armor_proficiency|=(1<<7);//LIBRAM
		break;
	case DRUID:
		armor_proficiency|=(1<<8);//IDOL
		break;
	case SHAMAN:
		armor_proficiency|=(1<<9);//TOTEM
		break;
	case DEATHKNIGHT: 
		armor_proficiency|=(1<<10);//SIGILS
		break;
	case WARRIOR: if(!sWorld.m_WarriorPetAllowed) break;    // WaadFun
	case WARLOCK:
	case HUNTER:
		_LoadPet(results[5].result);
        _LoadPetSpells(results[6].result);
		_LoadPetActionBar(results[13].result);
	break;
	}

	if(m_session->CanUseCommand('c'))
		_AddLanguages(true);
	else
		_AddLanguages(false);

	OnlineTime	= (uint32)UNIXTIME;
	if(GetGuildId())
		SetUInt32Value(PLAYER_GUILD_TIMESTAMP, (uint32)UNIXTIME);
		
	m_talentActiveSpec = get_next_field.GetUInt32();
	m_talentSpecsCount = get_next_field.GetUInt32();
	if(m_talentSpecsCount > MAX_SPEC_COUNT)
		m_talentSpecsCount = MAX_SPEC_COUNT;
	if(m_talentActiveSpec >= m_talentSpecsCount )
		m_talentActiveSpec = 0;

	bool needTalentReset = get_next_field.GetBool();
	if( needTalentReset )
	{
		Reset_Talents();
	}
	
#undef get_next_field

	// load properties
	_LoadTalents(results[14].result);
	_LoadGlyphs(results[15].result);
	_LoadSpells(results[16].result);
	_LoadTutorials(results[1].result);
	_LoadPlayerCooldowns(results[2].result);
	_LoadQuestLogEntry(results[3].result);
	m_ItemInterface->mLoadItemsFromDatabase(results[4].result);
	m_mailBox.Load(results[7].result);

	// SOCIAL
	if( results[8].result != NULL )			// this query is "who are our friends?"
	{
		result = results[8].result;
		do 
		{
			fields = result->Fetch();
			if( strlen( fields[1].GetString() ) )
				m_friends.insert( make_pair( fields[0].GetUInt32(), strdup(fields[1].GetString()) ) );
			else
				m_friends.insert( make_pair( fields[0].GetUInt32(), (char*)NULL) );

		} while (result->NextRow());
	}

	if( results[9].result != NULL )			// this query is "who has us in their friends?"
	{
		result = results[9].result;
		do 
		{
			m_hasFriendList.insert( result->Fetch()[0].GetUInt32() );
		} while (result->NextRow());
	}

	if( results[10].result != NULL )		// this query is "who are we ignoring"
	{
		result = results[10].result;
		do 
		{
			m_ignores.insert( result->Fetch()[0].GetUInt32() );
		} while (result->NextRow());
	}

	// END SOCIAL
	
	// Load achievements
	GetAchievementInterface()->LoadFromDB( results[11].result );

	// Set correct maximum level
	SetUInt32Value(PLAYER_FIELD_MAX_LEVEL, sWorld.m_levelCap);

	UpdateTalentInspectBuffer();
	SetFlag(UNIT_FIELD_FLAGS_2,UNIT_FLAG2_REGENERATE_POWER); // enables automatic power regen
	m_session->FullLogin(this);
	if(m_session)
		m_session->m_loggingInPlayer=NULL;

	if( !isAlive() )
		myCorpse = objmgr.GetCorpseByOwner(GetLowGUID());
/*		
	//Send an update for our power type
	WorldPacket power_update(SMSG_POWER_UPDATE, 20);
	power_update << GetNewGUID();
	power_update << uint8(myClass->power_type);
	power_update << GetUInt32Value(UNIT_FIELD_POWER1 + myClass->power_type);
	CopyAndSendDelayedPacket(&power_update);

	//Set glyph  slot values
	for (uint8 i=0; i<8; ++i)
	{
		SetUInt32Value(PLAYER_FIELD_GLYPH_SLOTS_1 + i, 21 + i); //official does this, wtf is it?
	}

	if (getClass() == DEATHKNIGHT)
		m_runemask = 0x3F; //enable runes

	//Add glyph stuff

	UpdateKnownGlyphs();
	for (uint8 i=0; i<6; ++i)
	{
		if (GetUInt32Value(PLAYER_FIELD_GLYPHS_1 + i))
		{
			GlyphPropertiesEntry* glyph = dbcGlyphProperties.LookupEntry(GetUInt32Value(PLAYER_FIELD_GLYPHS_1 + i));
			if (glyph != NULL)
			{
				//remove any possible saved auras
				RemoveAura(glyph->spellId);

				Spell* sp = new Spell(this, dbcSpell.LookupEntry(glyph->spellId), true, NULL);
				SpellCastTargets t(this);
				sp->prepare(&t);
			}
		}
	}	
	//Test stuff past here
	SetUInt32Value(PLAYER_FIELD_KNOWN_CURRENCIES, 0xFFFFFFFF);		
*/
	  // JcJ_Mode (Branruz)
    if(HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE))
	{
		// PvPUpdate() serait mieux, a finir (Branruz)
	  SetPvPFlag();
	}
	
	// Saisons d'arenes ( Branruz - Team Waad )
    SendWorldStateUpdate(WS_SEASON_ARENA,sWorld.m_ArenaSeasonOnTime); 
	SendWorldStateUpdate(WS_SEASON_NUMBER,sWorld.m_ArenaSeasonNumber);

	Group* pGroup = this->GetGroup();
	if(pGroup)
	{
		Log.Notice("[Player::LoadFromDbProc]","%s est dans un groupe a la connexion!",this->GetName());
	}
}

void Player::RolloverHonor()
{
	uint32 current_val = (g_localTime.tm_year << 16) | g_localTime.tm_yday;
	if( current_val != m_honorRolloverTime )
	{
		m_honorRolloverTime = current_val;
		m_honorYesterday = m_honorToday;
		m_killsYesterday = m_killsToday;
		m_honorToday = m_killsToday = 0;
	}
}

bool Player::HasSpell(uint32 spell)
{
	return mSpells.find(spell) != mSpells.end();
}

bool Player::HasTalentWithSpellID( uint32 SpellId )
{
	std::map<uint32, uint8> *talents = &m_specs[m_talentActiveSpec].talents;
	std::map<uint32, uint8>::iterator itr = talents->begin();
	SpellEntry * sp;

	for( ; itr != talents->end(); itr++ )
	{
		TalentEntry *te = dbcTalent.LookupEntryForced(itr->first);
		if( te == NULL ) continue;

		sp = dbcSpell.LookupEntry( te->RankID[itr->second] );
		if( sp != NULL && sp->Id == SpellId )
			return true;
	}
	return false;
}

void Player::_LoadQuestLogEntry(QueryResult * result)
{
	QuestLogEntry *entry;
	Quest *quest;
	Field *fields;
	uint32 questid;
	uint32 baseindex;
	
	// clear all fields
	for(int i = 0; i < PLAYER_QUEST_LOG_MAX; ++i)
	{
		baseindex = PLAYER_QUEST_LOG_1_1 + (i * PLAYER_QUEST_SLOT_MAX); // Def dans UpdateField.h
		SetUInt32Value(baseindex + QUEST_ID_INDEX, 0);      // Id
		SetUInt32Value(baseindex + QUEST_STATE_INDEX, 0);   // State
		SetUInt32Value(baseindex + QUEST_FIELD_INDEX, 0);   // Field (Count offset)
		SetUInt32Value(baseindex + QUEST_FIELD_INDEX+1, 0); // Field (Count offset)
		SetUInt32Value(baseindex + QUEST_TIMER_INDEX, 0);   // Timer
	}

	int slot = 0;

	if(result)
	{
		do 
		{
			fields = result->Fetch();
			questid = fields[1].GetUInt32();
			quest = QuestStorage.LookupEntry(questid);
			slot = fields[2].GetUInt32();
			// ASSERT(slot != -1); // Epurage des Assert (Branruz)
			if(slot < 0)
			{
				return;
			}
			
			// remove on next save if bad quest
			if(!quest)
			{
				m_removequests.insert(questid);
				continue;
			}
			if(m_questlog[slot] != 0)
				continue;
			
			entry = NULL;
			entry = new QuestLogEntry;
			entry->Init(quest, this, slot);
			entry->LoadFromDB(fields);
			entry->UpdatePlayerFields();

		} while(result->NextRow());
	}
}

QuestLogEntry* Player::GetQuestLogForEntry(uint32 quest)
{
	for(int i = 0; i < 25; ++i)
	{
		if(m_questlog[i] == ((QuestLogEntry*)0x00000001))
			m_questlog[i] = NULL;

		if(m_questlog[i] != NULL)
		{
			if(m_questlog[i]->GetQuest() && m_questlog[i]->GetQuest()->id == quest)
				return m_questlog[i];
		}
	}
	return NULL;
	//uint32 x = PLAYER_QUEST_LOG_1_1;
	//uint32 y = 0;
	//for(; x < PLAYER_VISIBLE_ITEM_1_CREATOR && y < 25; x += 3, y++)
	//{
	//	if(m_uint32Values[x] == quest)
	//		return m_questlog[y];
	//}
	//return NULL;
}

void Player::SetQuestLogSlot(QuestLogEntry *entry, uint32 slot)
{
	m_questlog[slot] = entry;
}

void Player::AddToWorld()
{
	FlyCheat = false;
	m_setflycheat=false;
	// check transporter
	if(m_TransporterGUID && m_CurrentTransporter)
	{
		SetPosition(m_CurrentTransporter->GetPositionX() + m_TransporterX,
			m_CurrentTransporter->GetPositionY() + m_TransporterY,
			m_CurrentTransporter->GetPositionZ() + m_TransporterZ,
			GetOrientation(), false);
	}

	// If we join an invalid instance and get booted out, this will prevent our stats from doubling :P
	if(IsInWorld())
		return;

	m_beingPushed = true;
	Object::AddToWorld();
	
	// Add failed.
	if(m_mapMgr == NULL)
	{
		// eject from instance
		m_beingPushed = false;
		EjectFromInstance();
		return;
	}

	if(m_session)
		m_session->SetInstance(m_mapMgr->GetInstanceID());
}

void Player::AddToWorld(MapMgr * pMapMgr)
{
	FlyCheat = false;
	m_setflycheat=false;
	// check transporter
	if(m_TransporterGUID && m_CurrentTransporter)
	{
		SetPosition(m_CurrentTransporter->GetPositionX() + m_TransporterX,
			m_CurrentTransporter->GetPositionY() + m_TransporterY,
			m_CurrentTransporter->GetPositionZ() + m_TransporterZ,
			GetOrientation(), false);
	}

	// If we join an invalid instance and get booted out, this will prevent our stats from doubling :P
	if(IsInWorld())
		return;

	m_beingPushed = true;
	Object::AddToWorld(pMapMgr);

	// Add failed.
	if(m_mapMgr == NULL)
	{
		// eject from instance
		m_beingPushed = false;
		EjectFromInstance();
		return;
	}

	if(m_session)
		m_session->SetInstance(m_mapMgr->GetInstanceID());
}

void Player::OnPrePushToWorld()
{
	SendInitialLogonPackets();
}

void Player::OnPushToWorld()
{
	// Process create packet
	if( m_mapMgr != NULL )
		ProcessPendingUpdates(&m_mapMgr->m_updateBuildBuffer, &m_mapMgr->m_compressionBuffer);

	if(m_TeleportState == 2)   // Worldport Ack
		OnWorldPortAck();

	ResetSpeedHack();
	m_beingPushed = false;
	AddItemsToWorld();
	m_lockTransportVariables = false;

	// delay the unlock movement packet
	// Cette trame doit etre envoyé regulierement au client pour synchro avec le serveur (Branruz)
	 OnTimeSyncRequest(); // (Branruz)

	// sWorld.mInWorldPlayerCount++;

	// Update PVP Situation
	LoginPvPSetup();

	Unit::OnPushToWorld();
   
	if(m_FirstLogin)
	{
		sHookInterface.OnFirstEnterWorld(this);
		if (getClass() != DEATHKNIGHT)
		{
			LevelInfo * Info = objmgr.GetLevelInfo(getRace(), getClass(), sWorld.m_startLevel);
			ApplyLevelInfo(Info, sWorld.m_startLevel);
		}
		else
		{
			LevelInfo * Info = objmgr.GetLevelInfo(getRace(), getClass(),55);
			ApplyLevelInfo(Info,55);
		}
		m_FirstLogin = false;
	}
    // Envoi des worlstates au player a la ko, sinon l'update zone le fera par la suite
	if( this->GetMapMgr() != NULL )
		this->GetMapMgr()->GetDBStateManager().SendWorldStates(this); 

	sHookInterface.OnEnterWorld(this);
	if(sWorld.m_forceGMTag && m_session->HasGMPermissions() && !m_session->CanUseCommand('z'))
	{
	 m_session->GetPlayer()->bGMTagOn = true;
	 m_session->GetPlayer()->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_GM);	// <GM> & Blizz symbol
	}

	if(m_TeleportState == 1)		// First world enter
		CompleteLoading();

	m_TeleportState = 0;

	if(GetTaxiState())
	{
		if( m_taxiMapChangeNode != 0 )
		{
			lastNode = m_taxiMapChangeNode;
		}

		// Create HAS to be sent before this!
		// ProcessPendingUpdates();
		if(lastNode > NUM_TAXI_NODES) lastNode = 0; // Secu

		TaxiStart(GetTaxiPath(),GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID),lastNode);

		m_taxiMapChangeNode = 0;
	}

	if(flying_aura && m_mapId != 530)
	{
		RemoveAura(flying_aura);
		flying_aura = 0;
	}

	ResetHeartbeatCoords();
	_heartbeatDisable = 0;
	_speedChangeInProgress =false;
	m_lastMoveType = 0;
	
	/* send weather */
	sWeatherMgr.SendWeather(this);

	if( load_health > 0 )
	{
		if( load_health > m_uint32Values[UNIT_FIELD_MAXHEALTH] )
			SetUInt32Value( UNIT_FIELD_HEALTH, m_uint32Values[UNIT_FIELD_MAXHEALTH] );
		else
			SetUInt32Value(UNIT_FIELD_HEALTH, load_health);
	}

	if( load_mana > m_uint32Values[UNIT_FIELD_MAXPOWER1] )
		SetUInt32Value( UNIT_FIELD_POWER1, m_uint32Values[UNIT_FIELD_MAXPOWER1] );
	else
		SetUInt32Value(UNIT_FIELD_POWER1, load_mana);

	if( !GetSession()->HasGMPermissions() )
		GetItemInterface()->CheckAreaItems(); 

#ifdef ENABLE_COMPRESSED_MOVEMENT
	//sEventMgr.AddEvent(this, &Player::EventDumpCompressedMovement, EVENT_PLAYER_FLUSH_MOVEMENT, World::m_movementCompressInterval, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	MovementCompressor->AddPlayer(this);
#endif

	if( m_mapMgr && m_mapMgr->m_battleground != NULL && m_bg != m_mapMgr->m_battleground )
	{
		m_bg = m_mapMgr->m_battleground;
		m_bg->PortPlayer( this, true );
	}

	if( m_bg != NULL )
		m_bg->OnPlayerPushed( this );

	z_axisposition = 0.0f;
	m_changingMaps = false;
	
	SendPowerUpdate();
}
// Experimentation Synchro Client<->Serveur (Branruz)
// A FAIRE: - Poser un event de 2sec à la création du player
//          - 
void Player::OnTimeSyncRequest()
{
 	WorldPacket * data = new WorldPacket(SMSG_TIME_SYNC_REQ, 4);
	if(SyncCurrentTickNumber > 100) SyncCurrentTickNumber  = 0;
	SyncCurrentTickNumber++;
	SyncServeurTickNumber = SyncCurrentTickNumber;
	SyncServeurTickCount = getMSTime();
	*data << uint32(SyncServeurTickNumber);
	GetSession()->SendPacket(data);
	Log.Notice("[SMSG_TIME_SYNC_REQ]","%s TickCount %u",GetName(),SyncCurrentTickNumber);
} //------------------------

void Player::ResetHeartbeatCoords()
{
	_lastHeartbeatPosition = m_position;
	_lastHeartbeatV = m_runSpeed;
	if( m_isMoving )
		_startMoveTime = m_lastMoveTime;
	else
		_startMoveTime = 0;

	m_cheatEngineChances = 2;
	//_lastHeartbeatT = getMSTime();
}

void Player::RemoveFromWorld()
{
	EndDuel(0);

	if(raidgrouponlysent)
		event_RemoveEvents(EVENT_PLAYER_EJECT_FROM_INSTANCE);

	load_health = m_uint32Values[UNIT_FIELD_HEALTH];
	load_mana = m_uint32Values[UNIT_FIELD_POWER1];

	m_resurrectHealth = 0;
	m_resurrectMana = 0;
	resurrector = 0;

	if(m_bg)
	{
		m_bg->RemovePlayer(this, true);
		m_bg = NULL;
	}
/* En commentaire pour l'instant (Branruz)
	if(m_tempSummon)
	{
		m_tempSummon->SafeDelete();

		m_tempSummon = 0;
		SetUInt64Value(UNIT_FIELD_SUMMON, 0);
	}*/

	// Cancel trade if it's active.
	Player * pTarget;
	if(mTradeTarget != 0)
	{
		pTarget = GetTradeTarget();
		if(pTarget)
			pTarget->ResetTradeVariables();

		ResetTradeVariables();
	}
	//clear buyback
	GetItemInterface()->EmptyBuyBack();
	
	for(uint32 x=0; x<NUM_SUMMON_SLOTS; x++)
	{
		if(m_summonslot[x])
			m_summonslot[x]->SafeDelete();
	}

	ResetHeartbeatCoords();
	ClearSplinePackets();

	// En cas de remove du player, le pet l'est aussi mais en faite, 
	// il s'agit plus d'un dismiss ( On renvoi le familier en gros )
	if(m_Summon)
	{
		m_Summon->GetAIInterface()->SetPetOwner(0);
		m_Summon->Remove(false, true, false);
		// m_Summon->ClearPetOwner(); Fait avec m_Summon->Remove
		m_Summon=NULL; // Si le player est tp, le familier ne devrait pas etre reset.
	}

	if(m_SummonedObject)
	{
		if(m_SummonedObject->GetInstanceID() != GetInstanceID())
		{
			sEventMgr.AddEvent(m_SummonedObject, &Object::Delete, EVENT_GAMEOBJECT_EXPIRE, 100, 1,0);
		}
		else
		{
			if(m_SummonedObject->GetTypeId() == TYPEID_PLAYER)
			{

			}
			else
			{
				if(m_SummonedObject->IsInWorld())
				{
					m_SummonedObject->RemoveFromWorld(true);
				}
				delete m_SummonedObject;
			}
		}
		m_SummonedObject = NULL;
	}

	if(IsInWorld())
	{
		RemoveItemsFromWorld();
		Unit::RemoveFromWorld(false);
	}

	sWorld.mInWorldPlayerCount--;
#ifdef ENABLE_COMPRESSED_MOVEMENT
	MovementCompressor->RemovePlayer(this);
	m_movementBufferLock.Acquire();
	m_movementBuffer.clear();
	m_movementBufferLock.Release();
	//sEventMgr.RemoveEvents(this, EVENT_PLAYER_FLUSH_MOVEMENT);
	
#endif

	if(GetTaxiState())
		event_RemoveEvents( EVENT_PLAYER_TAXI_INTERPOLATE );
		
	if( m_CurrentTransporter && !m_lockTransportVariables )
	{
		m_CurrentTransporter->RemovePlayer(this);
		m_CurrentTransporter = NULL;
		m_TransporterGUID = 0;
	}

	if( m_CurrentVehicle )
	{
		m_CurrentVehicle->RemovePassenger(this);
		m_CurrentVehicle = NULL;
	}		

	m_changingMaps = true;
}


// TODO: perhaps item should just have a list of mods, that will simplify code
void Player::_ApplyItemMods(Item* item, int8 slot, bool apply, bool justdrokedown /* = false */, bool skip_stat_apply /* = false  */)
{
	if (slot >= INVENTORY_SLOT_BAG_END) return;
    //ASSERT( item );
	if(!item) 
	{
		Log.Error("Player::_ApplyItemMods","Item NULL [%s]",this->GetName());
		return;
	}

	ItemPrototype* proto = item->GetProto();

	//fast check to skip mod applying if the item doesn't meat the requirements.
	if( item->GetUInt32Value( ITEM_FIELD_DURABILITY ) == 0 && item->GetUInt32Value( ITEM_FIELD_MAXDURABILITY ) && justdrokedown == false )
	{
		return;
	}

	//check for rnd prop
	item->ApplyRandomProperties( true );

	//Items Set check
	uint32 setid = proto->ItemSet;

	/* Les stats sont conservés par Set d'item, je vois pas pourquoi on changerait
	   par d'autres stats qui ne correspondront plus à ce que vois le player
	   Code en commentaire à  virer dans une ou 2 rev. (Pour infos Devs)
	   La plus part de ces items ne sont pas dans la base d'ailleurs 
       select * from `items` where `itemset` != 0 order by `itemset` asc;
	   - Branruz - r435

	// These season pvp itemsets are interchangeable and each set group has the same
	// bonuses if you have a full set made up of parts from any of the 3 similar sets
	// you will get the highest sets bonus

	// TODO: make a config for server so they can configure which season is active season

	// * Gladiator's Battlegear
	if( setid == 701 || setid == 736 || setid == 567 )
		setid = 746;
	
	// * Gladiator's Dreadgear
	if( setid == 702 || setid == 734 || setid == 568 )
		setid = 734;

	// * Gladiator's Earthshaker
	if( setid == 703 || setid == 732 || setid == 578 )
		setid= 732;

	// * Gladiator's Felshroud
	if( setid == 704 || setid == 735 || setid == 615 )
		setid = 735;

	// * Gladiator's Investiture
	if( setid == 705 || setid == 728 || setid == 687 )
		setid = 728;

	// * Gladiator's Pursuit
	if( setid == 706 || setid == 723 || setid == 586 )
		setid = 723;

	// * Gladiator's Raiment
	if( setid == 707 || setid == 729 || setid == 581 )
		setid = 729;

	// * Gladiator's Redemption
	if( setid == 708 || setid == 725 || setid == 690 )
		setid = 725;

	// * Gladiator's Refuge
	if( setid == 709 || setid == 720 || setid == 685 )
		setid = 720;

	// * Gladiator's Regalia
	if( setid == 710 || setid == 724 || setid == 579 )
		setid = 724;

	// * Gladiator's Sanctuary
	if( setid == 711 || setid == 721 || setid == 584 )
		setid = 721;

	// * Gladiator's Thunderfist
	if( setid == 712 || setid == 733 || setid == 580 )
		setid = 733;

	// * Gladiator's Vestments
	if( setid == 713 || setid == 730 || setid == 577 )
		setid = 730;

	// * Gladiator's Vindication
	if( setid == 714 || setid == 726 || setid == 583 )
		setid = 726;

	// * Gladiator's Wartide
	if( setid == 715 || setid == 731 || setid == 686 )
		setid = 731;

	// * Gladiator's Wildhide
	if( setid == 716 || setid == 722 || setid == 585 )
		setid = 722;
		*/

	// Set
	if( setid != 0 )
	{
		ItemSetEntry* set = dbcItemSet.LookupEntry( setid );
		//ASSERT( set );
		if(!set)
		{
			Log.Error("Player::_ApplyItemMods","%s Erreur SetId %u",this->GetName(),setid);
			return;
		}

		ItemSet* Set = NULL;
		std::list<ItemSet>::iterator i;
		for( i = m_itemsets.begin(); i != m_itemsets.end(); i++ )
		{
			if( i->setid == setid )
			{   
				Set = &(*i);
				break;
			}
		}

		if( apply )
		{
			if( Set == NULL ) 
			{
				Set = new ItemSet;
				memset( Set, 0, sizeof( ItemSet ) );
				Set->itemscount = 1;
				Set->setid = setid;
			}
			else
				Set->itemscount++;

			if( !set->RequiredSkillID || ( _GetSkillLineCurrent( set->RequiredSkillID, true ) >= set->RequiredSkillAmt ) )
			{
				for( uint32 x=0;x<8;x++)
				{
					if( Set->itemscount==set->itemscount[x])
					{//cast new spell
						SpellEntry *info = dbcSpell.LookupEntry( set->SpellID[x] );
						Spell * spell = new Spell( this, info, true, NULL );
						SpellCastTargets targets;
						targets.m_target = this;
						spell->prepare( &targets );
					}
				}
			}
			if( i == m_itemsets.end() )
			{
				m_itemsets.push_back( *Set );
				delete Set;
			}
		}
		else
		{
			if( Set )
			{
				for( uint32 x = 0; x < 8; x++ )
				if( Set->itemscount == set->itemscount[x] )
				{
					this->RemoveAura( set->SpellID[x], GetGUID() );
				}
	   
				if(!(--Set->itemscount))
				m_itemsets.erase(i);
			}
		}
	}
 
	// Armor
	if( proto->Armor )
	{
		if( apply )BaseResistance[0 ]+= proto->Armor;  
		else  BaseResistance[0] -= proto->Armor;
		CalcResistance( 0 );
	}

	// Resistances
	//TODO: FIXME: can there be negative resistances from items?
	if( proto->FireRes )
	{
		if( apply )
			FlatResistanceModifierPos[2] += proto->FireRes;
		else
			FlatResistanceModifierPos[2] -= proto->FireRes;
		CalcResistance( 2 );
	}
		
	if( proto->NatureRes )
	{
		if( apply )
			FlatResistanceModifierPos[3] += proto->NatureRes;
		else
			FlatResistanceModifierPos[3] -= proto->NatureRes;
		CalcResistance( 3 );
	}

	if( proto->FrostRes )
	{
		if( apply )
			FlatResistanceModifierPos[4] += proto->FrostRes;
		else
			FlatResistanceModifierPos[4] -= proto->FrostRes;
		CalcResistance( 4 );	
	}

	if( proto->ShadowRes )
	{
		if( apply )
			FlatResistanceModifierPos[5] += proto->ShadowRes;
		else
			FlatResistanceModifierPos[5] -= proto->ShadowRes;
		CalcResistance( 5 );	
	}
	 
	if( proto->ArcaneRes )
	{
		if( apply )
			FlatResistanceModifierPos[6] += proto->ArcaneRes;
		else
			FlatResistanceModifierPos[6] -= proto->ArcaneRes;
		CalcResistance( 6 );
	}
		/* Heirloom scaling items Note Randdrick  en attente d'implémentation.-
	if(proto->ScalingStatsEntry != 0){
		int i = 0;
		ScalingStatDistributionEntry *ssdrow = dbcScalingStatDistribution.LookupEntry( proto->ScalingStatsEntry );
		ScalingStatValuesEntry *ssvrow = NULL;
		uint32 StatType;
		uint32 StatMod;
		uint32 plrLevel = getLevel();
		int32 StatMultiplier;
		int32 StatValue;
		int32 col = 0;

		// this is needed because the heirloom items don't scale over lvl80
		if(plrLevel > 80)
			plrLevel = 80;

        
        DBCStorage<ScalingStatValuesEntry>::iterator itr;

        for(itr = dbcScalingStatValues.begin(); itr != dbcScalingStatValues.end(); ++itr)
            if( (*itr)->level == plrLevel ){
                ssvrow = *itr;
                break;
            }
       
		// Not going to put a check here since unless you put a random id/flag in the tables these should never return NULL 

		// Calculating the stats correct for our level and applying them 
		for(i = 0; ssdrow->stat[i] != -1; i++){
			StatType = ssdrow->stat[i];
			StatMod  = ssdrow->statmodifier[i];
			col = GetStatScalingStatValueColumn(proto,SCALINGSTATSTAT);
			if( col == -1 )
				continue;
			StatMultiplier = ssvrow->multiplier[col];
			StatValue = StatMod*StatMultiplier/10000;
			ModifyBonuses(StatType,StatValue,apply);
		}

		if((proto->ScalingStatsFlag & 32768) && i < 10){
			StatType = ssdrow->stat[i];
			StatMod  = ssdrow->statmodifier[i];
			col = GetStatScalingStatValueColumn(proto,SCALINGSTATSPELLPOWER);
			if(col != -1){
				StatMultiplier = ssvrow->multiplier[col];
				StatValue = StatMod*StatMultiplier/10000;
				ModifyBonuses(45,StatValue,apply);
			}
		}

		// Calculating the Armor correct for our level and applying it 
		col = GetStatScalingStatValueColumn(proto,SCALINGSTATARMOR);
		if(col != -1)
		{
			uint32 scaledarmorval = ssvrow->multiplier[ col ];
			if( apply )BaseResistance[0 ]+= scaledarmorval;
			else  BaseResistance[0] -= scaledarmorval;
			CalcResistance( 0 );
		}

		// Calculating the damages correct for our level and applying it 
		col = GetStatScalingStatValueColumn(proto,SCALINGSTATDAMAGE);
		if(col != -1){
			uint32 scaleddps = ssvrow->multiplier [ col ];
			float dpsmod = 1.0;

			if (proto->ScalingStatsFlag & 0x1400)
				dpsmod = 0.2f;
			else dpsmod = 0.3f;

			float scaledmindmg = (scaleddps - (scaleddps * dpsmod)) * (proto->Delay/1000);
			float scaledmaxdmg = (scaleddps * (dpsmod+1.0f)) * (proto->Delay/1000);

			if( proto->InventoryType == INVTYPE_RANGED || proto->InventoryType == INVTYPE_RANGEDRIGHT || proto->InventoryType == INVTYPE_THROWN )
			{
				BaseRangedDamage[0] += apply ? scaledmindmg : -scaledmindmg;
				BaseRangedDamage[1] += apply ? scaledmaxdmg : -scaledmaxdmg;
			}
			else
			{
				if( slot == EQUIPMENT_SLOT_OFFHAND )
				{
					BaseOffhandDamage[0] = apply ? scaledmindmg : 0;
					BaseOffhandDamage[1] = apply ? scaledmaxdmg : 0;
				}
				else
				{
					BaseDamage[0] = apply ? scaledmindmg : 0;
					BaseDamage[1] = apply ? scaledmaxdmg : 0;
				}
			}
		}

	// Normal items 
	} else {
	*/
	// Stats
	for( int i = 0; i < 10; i++ )
	{
		int32 val = proto->Stats[i].Value;

		if( val == 0 ) continue;
		if(proto->Stats[i].Type < ITEM_STAT_TYPE_MAX)
		{
         //Log.Warning("[_ApplyItemMods]","Modify bonus, Item %u Stat[%u], Type %u, Val: %d, Apply: %s",
		 //	                                 proto->ItemId,i,proto->Stats[i].Type,val,apply ? "Oui":"Non");
		 ModifyBonuses( proto->Stats[i].Type, apply ? val : -val );
		}
		else
		{
			Log.Error("[Player::_ApplyItemMods]","Item %u Stat[%u] wrong, Type %u, Val: %d, Apply: %s",
													proto->ItemId,i,proto->Stats[i].Type,val,apply ? "Oui":"Non");
		}

	}

	// Damage
	if( proto->Damage[0].Min )
	{
		if( proto->InventoryType == INVTYPE_RANGED || proto->InventoryType == INVTYPE_RANGEDRIGHT || proto->InventoryType == INVTYPE_THROWN )	
		{	
			BaseRangedDamage[0] += apply ? proto->Damage[0].Min : -proto->Damage[0].Min;
			BaseRangedDamage[1] += apply ? proto->Damage[0].Max : -proto->Damage[0].Max;
		}
		else
		{
			if( slot == EQUIPMENT_SLOT_OFFHAND )
			{
				BaseOffhandDamage[0] = apply ? proto->Damage[0].Min : 0;
				BaseOffhandDamage[1] = apply ? proto->Damage[0].Max : 0;
			}
			else
			{
				BaseDamage[0] = apply ? proto->Damage[0].Min : 1;
				BaseDamage[1] = apply ? proto->Damage[0].Max : 1;
			}
		}
	}

	// Misc
	if( apply )
	{
		// Apply all enchantment bonuses
		item->ApplyEnchantmentBonuses();

		for( int k = 0; k < 5; k++ )
		{
			// stupid fucked dbs
			if( item->GetProto()->Spells[k].Id == 0 )
				continue;

			if( item->GetProto()->Spells[k].Trigger == 1 )
			{
				SpellEntry* spells = dbcSpell.LookupEntry( item->GetProto()->Spells[k].Id );
				if( spells->shapeshiftMask )
				{
					AddShapeShiftSpell( spells->Id );
					continue;
				}

				Spell *spell = new Spell( this, spells ,true, NULL );
				SpellCastTargets targets;
				targets.m_target = this;
				spell->castedItemId = item->GetEntry();
				spell->prepare( &targets );

			}
			else if( item->GetProto()->Spells[k].Trigger == 2 )
			{
				ProcTriggerSpell ts;
				memset(&ts, 0, sizeof(ProcTriggerSpell));
				ts.origId = 0;
				ts.spellId = item->GetProto()->Spells[k].Id;
				ts.procChance = 5;
				ts.caster = this->GetGUID();
				ts.procFlags = PROC_ON_MELEE_HIT;
				if(slot == EQUIPMENT_SLOT_MAINHAND)
					ts.weapon_damage_type = 1; // Proc only on main hand attacks
				else if(slot == EQUIPMENT_SLOT_OFFHAND)
					ts.weapon_damage_type = 2; // Proc only on off hand attacks
				else
					ts.weapon_damage_type = 0; // Doesn't depend on weapon
				ts.deleted = false;
				m_procSpells.push_front( ts );			
			}
		}
	}
	else
	{
		// Remove certain "enchantments" that are not actually enchantments - WTF! Damn you, Blizzard.
		// Classic example: http://www.wowhead.com/?spell=29720
		// (Greater Ward of Shielding) - Absorbs 4000 damage. Requires a shield.
		// Doesn't get removed when we unequip the shield. Why? Because it's just an Aura. Not an enchantment.
			// damn bitwise values

		for( uint32 x = 0 ; x <= MAX_POSITIVE_AURAS ; x ++ )
		{
			if( m_auras[x] )
			{   // A virer apres le debug des Auras
				//Log.Notice( "Player" , "Aura %u, Id %u (%s) [%08X,%08X]" , x , m_auras[x]->GetSpellId() , m_auras[x]->GetSpellProto()->Name ,m_auras[x]->GetSpellProto()->EquippedItemClass , m_auras[x]->GetSpellProto()->EquippedItemSubClass );
				if( m_auras[x]->GetSpellProto()->EquippedItemClass && m_auras[x]->GetSpellProto()->EquippedItemSubClass )
				{
					if( (int32)item->GetProto()->Class == m_auras[x]->GetSpellProto()->EquippedItemClass && ( 1 << item->GetProto()->SubClass ) & m_auras[x]->GetSpellProto()->EquippedItemSubClass ) // fucking bits
						m_auras[x]->Remove();
				}
			}
		}
		// Remove all enchantment bonuses
		item->RemoveEnchantmentBonuses();
		for( int k = 0; k < 5; k++ )
		{
			if( item->GetProto()->Spells[k].Trigger == 1 )
			{
				SpellEntry* spells = dbcSpell.LookupEntry( item->GetProto()->Spells[k].Id );
				if( spells->shapeshiftMask )
					RemoveShapeShiftSpell( spells->Id );
				else
					RemoveAura( item->GetProto()->Spells[k].Id ); 
			}
			else if( item->GetProto()->Spells[k].Trigger == 2 )
			{
				std::list<struct ProcTriggerSpell>::iterator i;
				// Debug: i changed this a bit the if was not indented to the for
				// so it just set last one to deleted looks like unintended behaviour
				// because you can just use end()-1 to remove last so i put the if
				// into the for
				for( i = m_procSpells.begin(); i != m_procSpells.end(); i++ )
				{
					if( (*i).spellId == item->GetProto()->Spells[k].Id && !(*i).deleted )
					{
						//m_procSpells.erase(i);
						i->deleted = true;
						break;
					}
				}
			}
		}
	}
	
	if( !skip_stat_apply )
		UpdateStats();
}


void Player::SetMovement(uint8 pType, uint32 flag)
{
	WorldPacket data(20);
//	uint8 buf[20];
	bool MouvementOk = true;
	//StackPacket data(SMSG_FORCE_MOVE_ROOT, buf, 20);
	
	switch(pType)
	{
	case MOVE_ROOT:
		{
			data.SetOpcode(SMSG_FORCE_MOVE_ROOT);
			data << GetNewGUID();
			data << flag;
			m_currentMovement = MOVE_ROOT;
		}break;
	case MOVE_UNROOT:
		{
			data.SetOpcode(SMSG_FORCE_MOVE_UNROOT);
			data << GetNewGUID();
			data << flag;
			m_currentMovement = MOVE_UNROOT;
		}break;
	case MOVE_WATER_WALK:
		{
			m_setwaterwalk=true;
			data.SetOpcode(SMSG_MOVE_WATER_WALK);
			data << GetNewGUID();
			data << flag;
		}break;
	case MOVE_LAND_WALK:
		{
			m_setwaterwalk=false;
			data.SetOpcode(SMSG_MOVE_LAND_WALK);
			data << GetNewGUID();
			data << flag;
		}break;
	default:
		Log.Error("[Player::SetMovement]","%s Mouvement inconnu: %u ,Flag: 0x%04X (%u)",this->GetName(),pType,flag,flag);
		MouvementOk = false;
		break;
	}

    if(MouvementOk) SendMessageToSet(&data, true);
	else free(&data);
}

void Player::SetPlayerSpeed(uint8 SpeedType, float value)
{
	bool SetSpeedOk = true;
	WorldPacket data(200);
	/*
	data << GetNewGUID();
	data << m_speedChangeCounter++;
	
	if(SpeedType == RUN)			// nfi what this is.. :/
		data << uint8(1);

	data << value;*/
	if( value < 0.1f )
		value = 0.1f;

	//uint8 buf[200];
	//StackPacket data(SMSG_FORCE_RUN_SPEED_CHANGE, buf, 200);
	/*if(SpeedType==FLY)
	{
		data << GetNewGUID();
		data << m_speedChangeCounter++;

		if(SpeedType == RUN)			// nfi what this is.. :/
			data << uint8(1);

		data << value;
	}
	else
	{
		data << GetNewGUID();
		data << uint32(0);
		data << uint8(0);
		data << uint32(getMSTime());
		data << GetPosition();
		data << m_position.o;
		data << uint32(0);
		data << value;
	}*/
	if( SpeedType != SWIMBACK )
	{
		data << GetNewGUID();
		data << m_speedChangeCounter++;
		if( SpeedType == RUN )
			data << uint8(1);

		data << value;
	}
	else
	{
		data << GetNewGUID();
		data << uint32(0);
		data << uint8(0);
		data << uint32(getMSTime());
		data << m_position.x;
		data << m_position.y;
		data << m_position.z;
		data << m_position.o;
		data << uint32(0);
		data << value;
	}
	
	switch(SpeedType)
	{
	case RUN:
		{
			if(value == m_lastRunSpeed)
				return;

			data.SetOpcode(SMSG_FORCE_RUN_SPEED_CHANGE);
			m_runSpeed = value;
			m_lastRunSpeed = value;
		}break;
	case RUNBACK:
		{
			if(value == m_lastRunBackSpeed)
				return;

			data.SetOpcode(SMSG_FORCE_RUN_BACK_SPEED_CHANGE);
			m_backWalkSpeed = value;
			m_lastRunBackSpeed = value;
		}break;
	case SWIM:
		{
			if(value == m_lastSwimSpeed)
				return;

			data.SetOpcode(SMSG_FORCE_SWIM_SPEED_CHANGE);
			m_swimSpeed = value;
			m_lastSwimSpeed = value;
		}break;
	case SWIMBACK:
		{
			if(value == m_lastBackSwimSpeed)
				break;

			data.SetOpcode(SMSG_FORCE_SWIM_BACK_SPEED_CHANGE);
			m_backSwimSpeed = value;
			m_lastBackSwimSpeed = value;
		}break;
	case FLY:
		{
			if(value == m_lastFlySpeed)
				return;

			//data.SetOpcode(SMSG_FORCE_MOVE_SET_FLY_SPEED);
			data.SetOpcode(SMSG_FORCE_FLIGHT_SPEED_CHANGE);
			m_flySpeed = value;
			m_lastFlySpeed = value;
		}break;
	default:
		Log.Error("[Player::SetPlayerSpeed]","%s ,SpeedType inconnu : %u, Value: %.5f",this->GetName(),SpeedType,value);
		SetSpeedOk = false;
		break;
	}
	
	if(SetSpeedOk) 
	{
		SendMessageToSet(&data , true);
	    // dont mess up on these
	    _speedChangeInProgress = true;
	}
	else
	{
		free(&data);
	}
}

void Player::BuildPlayerRepop()
{
	SetUInt32Value( UNIT_FIELD_HEALTH, 1 );

	/*
	 __Ghost____2				45681
     __Ghost____3				45682
	*/
	//8326 --for all races but ne,  9036 20584--ne
	SpellCastTargets tgt;
	tgt.m_target=this;
   
	if(getRace()==RACE_NIGHTELF)
	{
		SpellEntry *inf=dbcSpell.LookupEntry(__Ghost____1); // 20584
		Spell *sp = new Spell(this,inf,true,NULL);
		sp->prepare(&tgt);
		//inf=dbcSpell.LookupEntry(__Ghost____0); // 9036
		//sp=new Spell(this,inf,true,NULL);
		//sp->prepare(&tgt);
	}
	else
	{
	
		SpellEntry *inf=dbcSpell.LookupEntry(__Ghost); // 8326
		Spell*sp=new Spell(this,inf,true,NULL);
		sp->prepare(&tgt);
	}

	StopMirrorTimer(0);
	StopMirrorTimer(1);
	StopMirrorTimer(2);
	
	SetFlag(PLAYER_FLAGS, PLAYER_FLAG_DEATH_WORLD_ENABLE);

	SetMovement(MOVE_UNROOT, 1);
	SetMovement(MOVE_WATER_WALK, 1);
}

Corpse* Player::RepopRequestedPlayer()
{
	//Log.Warning("RepopRequestedPlayer","Player: %s",this->GetName());

	if( myCorpse != NULL )
	{
		GetSession()->SendNotification( NOTIFICATION_MESSAGE_NO_PERMISSION );
		return(NULL);
	}

	if( m_CurrentTransporter != NULL )
	{
		m_CurrentTransporter->RemovePlayer( this );
		m_CurrentTransporter = NULL;
		m_TransporterGUID = 0;

		ResurrectPlayer(NULL);
		RepopAtGraveyard( GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId() );
		return(NULL);
	}

	if( m_CurrentVehicle != NULL )
	{
		m_CurrentVehicle->RemovePassenger( this );
		m_CurrentVehicle = NULL;

		ResurrectPlayer(NULL);
		RepopAtGraveyard( GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId() );
		return(NULL);
	}

	sEventMgr.RemoveEvents( this, EVENT_PLAYER_FORECED_RESURECT ); //in case somebody resurrected us before this event happened

	// Set death state to corpse, that way players will lose visibility
	setDeathState( CORPSE );
	
	// Update visibility, that way people wont see running corpses :P
	UpdateVisibility();

	// If we're in battleground, remove the skinnable flag.. has bad effects heheh
	RemoveFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE );

	bool corpse = (m_bg != NULL) ? m_bg->CreateCorpse( this ) : true;
    Corpse* ret = NULL;

	if( corpse )
		ret = CreateCorpse();
	
	BuildPlayerRepop();

	if( corpse )
	{
		SpawnCorpseBody();

		/* Send Spirit Healer Location */
		//WorldPacket data( SMSG_SPIRIT_HEALER_POS, 16 );
		//Log.Warning("RepopRequestedPlayer","Send SMSG_DEATH_RELEASE_LOC, MapId: %u",m_mapId);
		WorldPacket data( SMSG_DEATH_RELEASE_LOC, 16 );
		data << m_mapId << m_position;
		m_session->SendPacket( &data );

		/* Corpse reclaim delay */
		//Log.Warning("RepopRequestedPlayer","Send SMSG_CORPSE_RECLAIM_DELAY (%u ms)",CORPSE_RECLAIM_TIME_MS);
		WorldPacket data2( SMSG_CORPSE_RECLAIM_DELAY, 4 );
		data2 << (uint32)( CORPSE_RECLAIM_TIME_MS );
		GetSession()->SendPacket( &data2 );
	}

	if( myCorpse != NULL )
		myCorpse->ResetDeathClock();

	//Log.Warning("RepopRequestedPlayer","Repop at GraveYard");
	MapInfo * pMapinfo = NULL;
	pMapinfo = WorldMapInfoStorage.LookupEntry( GetMapId() );
	if( pMapinfo != NULL )
	{
		if( pMapinfo->type == INSTANCE_NULL || pMapinfo->type == INSTANCE_PVP )
		{
			RepopAtGraveyard( GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId() );
		}
		else
		{
			RepopAtGraveyard( pMapinfo->repopx, pMapinfo->repopy, pMapinfo->repopz, pMapinfo->repopmapid );
		}
	}
	else
	{
		RepopAtGraveyard( GetPositionX(), GetPositionY(), GetPositionZ(), GetMapId() );
	}

	return(ret) ;
}
//-----------------------------------------------
void Player::ResurrectPlayer(Player* pResurrector)
{
	sEventMgr.RemoveEvents(this,EVENT_PLAYER_FORECED_RESURECT); //in case somebody resurected us before this event happened

	if( m_resurrectHealth )
		SetUInt32Value( UNIT_FIELD_HEALTH, (uint32)min( m_resurrectHealth, (uint32)(float2int32((float)m_uint32Values[UNIT_FIELD_MAXHEALTH]*0.25) )) );

	if( m_resurrectMana )
		SetUInt32Value( UNIT_FIELD_POWER1, (uint32)min( m_resurrectMana, (uint32)(float2int32((float)m_uint32Values[UNIT_FIELD_MAXPOWER1]*0.25) )) );

	m_resurrectHealth = m_resurrectMana = 0;

	SpawnCorpseBones();
	
	if(!sWorld.m_DebugPlayers) // Standard
	{
	 if(getRace()==RACE_NIGHTELF)
	 {
		RemoveAura(__Ghost____1); // 20584
		//RemoveAura(9036);
	 }
	 else RemoveAura(__Ghost); // 8326
	}
    else  // Debug Aura Ghost
	{
	 if(this->HasAura(__Ghost)) 
	 {
		Log.Notice("[Player::ResurrectPlayer()]","(%s) RemoveAura(__Ghost)",this->GetName());
		RemoveAura(__Ghost);     // 8326
	 }
	 if(this->HasAura(__Ghost____0)) 
	 {
		Log.Notice("[Player::ResurrectPlayer()]","(%s) RemoveAura(__Ghost___0)",this->GetName());
		RemoveAura(__Ghost____0); // 9036
	 }
	 if(this->HasAura(__Ghost____1)) 
	 {
		Log.Notice("[Player::ResurrectPlayer()]","(%s) RemoveAura(__Ghost___1)",this->GetName());
		RemoveAura(__Ghost____1); // 20584
	 }
	 if(this->HasAura(__Ghost____2)) 
	 {
		Log.Notice("[Player::ResurrectPlayer()]","(%s) RemoveAura(__Ghost___2)",this->GetName());
		RemoveAura(__Ghost____2); // 45681
	 }
	 if(this->HasAura(__Ghost____3)) 
	 {
		Log.Notice("[Player::ResurrectPlayer()]","(%s) RemoveAura(__Ghost___3)",this->GetName());
		RemoveAura(__Ghost____3); // 45682
	 }
	}

	//--------------------

	RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_DEATH_WORLD_ENABLE);
	setDeathState(ALIVE);
	UpdateVisibility();
	SetMovement(MOVE_LAND_WALK, 1);

	if(pResurrector != NULL && pResurrector->IsInWorld() && this->IsInWorld())
	{
		/*
		Player * p= objmgr.GetPlayer(resurrector);
		resurrector=0;

		if(p == 0) return;
		//_Relocate(p->GetMapMgr()->GetMapId(), p->GetPosition(),false,false);
		SafeTeleport(p->GetMapId(), p->GetInstanceID(), p->GetPosition());
		*/
		//make sure corpse and resurrector are on the same map.
		if( GetMapId() == pResurrector->GetMapId() )
		{
			if( m_resurrectLocation.x == 0.0f && m_resurrectLocation.y == 0.0f && m_resurrectLocation.z == 0.0f )
			{
				SafeTeleport(pResurrector->GetMapId(),pResurrector->GetInstanceID(),pResurrector->GetPosition());
			}
			else
			{
				SafeTeleport(GetMapId(), GetInstanceID(), m_resurrectLocation);
				m_resurrectLocation.ChangeCoords(0.0f, 0.0f, 0.0f);
			}
		}
	}
	else
	{
		// update player counts in arenas
		if( m_bg != NULL )
		{
			if(m_bg->IsArena()) ((Arena *)(m_bg))->UpdatePlayerCounts();			
		}
	}
}
//-----------------------------------------------
void Player::KillPlayer()
{
	setDeathState(JUST_DIED);

	//remove guardians!
	for (std::set<Creature*>::iterator itr=m_summonedCreatures.begin(); itr!=m_summonedCreatures.end(); ++itr)
		(*itr)->SafeDelete();
	m_summonedCreatures.clear();

	// Battleground stuff
	if(m_bg) m_bg->HookOnPlayerDeath(this);

	EventDeath();
	
	m_session->OutPacket(SMSG_CANCEL_COMBAT);
	m_session->OutPacket(SMSG_CANCEL_AUTO_REPEAT);

	SetMovement(MOVE_ROOT, 0);

	StopMirrorTimer(0);
	StopMirrorTimer(1);
	StopMirrorTimer(2);

	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED); //player death animation, also can be used with DYNAMIC_FLAGS <- huh???
	SetUInt32Value( UNIT_DYNAMIC_FLAGS, 0x00 );
	if(GetPowerType() == POWER_TYPE_RAGE) //rage resets on death
	{
		SetRage(0);
		SetUInt32Value(UNIT_FIELD_POWER2, 0);
	}
	if(this->getClass() == DEATHKNIGHT)
	{
		SetUInt32Value(UNIT_FIELD_POWER7, 0);
	}

	// combo points reset upon death
	NullComboPoints();

	GetAchievementInterface()->HandleAchievementCriteriaConditionDeath();
	GetAchievementInterface()->HandleAchievementCriteriaDeath();

	sHookInterface.OnDeath(this);
}

Corpse* Player::CreateCorpse()
{
	Corpse *pCorpse;
	uint32 /*_uf,*/ _pb, _pb2, _cfb1, _cfb2;

	objmgr.DelinkPlayerCorpses(this);
	if(!this->bCorpseCreateable)
	{
		bCorpseCreateable = true;   // for next time
		return(NULL); // no corpse allowed!
	}

	pCorpse = objmgr.CreateCorpse();
	pCorpse->SetInstanceID(GetInstanceID());
	pCorpse->Create(this, GetMapId(), GetPositionX(),
		GetPositionY(), GetPositionZ(), GetOrientation());

	//_uf = GetUInt32Value(UNIT_FIELD_BYTES_0); // OBSOLETE (Branruz)
	_pb = GetUInt32Value(PLAYER_BYTES);
	_pb2 = GetUInt32Value(PLAYER_BYTES_2);

	uint8 race	   = GetByte( UNIT_FIELD_BYTES_0, 0); // (uint8)(_uf);
	uint8 skin	   = (uint8)(_pb);
	uint8 face	   = (uint8)(_pb >> 8);
	uint8 hairstyle  = (uint8)(_pb >> 16);
	uint8 haircolor  = (uint8)(_pb >> 24);
	uint8 facialhair = (uint8)(_pb2);

	_cfb1 = ((0x00) | (race << 8) | (0x00 << 16) | (skin << 24));
	_cfb2 = ((face) | (hairstyle << 8) | (haircolor << 16) | (facialhair << 24));

	pCorpse->SetZoneId( GetZoneId() );
	pCorpse->SetUInt32Value( CORPSE_FIELD_BYTES_1, _cfb1 );
	pCorpse->SetUInt32Value( CORPSE_FIELD_BYTES_2, _cfb2 );
	pCorpse->SetUInt32Value( CORPSE_FIELD_FLAGS, 4 );
	pCorpse->SetUInt32Value( CORPSE_FIELD_DISPLAY_ID, GetUInt32Value(UNIT_FIELD_DISPLAYID) );

	/*
	if(m_bg) // En BG, le corps loot des Po
	{
		// remove our lootable flags
		RemoveFlag(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_LOOTABLE);
		RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
		
		loot.gold = 0;

		pCorpse->generateLoot();
		if(bShouldHaveLootableOnCorpse)
		{
			pCorpse->SetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS, 1); // sets it so you can loot the plyr
		}
		else
		{
			// hope this works
			pCorpse->SetUInt32Value(CORPSE_FIELD_FLAGS, 60);
		}

		// now that our corpse is created, don't do it again
		bShouldHaveLootableOnCorpse = false;
	}
	else
	{
		pCorpse->loot.gold = 0;
	}
	*/

	uint32 iDisplayID = 0;
	uint16 iIventoryType = 0;
	uint32 _cfi = 0;
	Item * pItem;
	for (int8 i = 0; i < EQUIPMENT_SLOT_END; i++)
	{
		pItem = GetItemInterface()->GetInventoryItem(i);
		if(pItem)
		{
			iDisplayID = pItem->GetProto()->DisplayInfoID;
			iIventoryType = (uint16)pItem->GetProto()->InventoryType;

			_cfi =  (uint16(iDisplayID)) | (iIventoryType)<< 24;
			pCorpse->SetUInt32Value(CORPSE_FIELD_ITEM + i,_cfi);
		}
	}

	/*
	// are we going to bones straight away?
	if(m_insigniaTaken)
	{
		m_insigniaTaken = false;   // for next time
		pCorpse->SetUInt32Value(CORPSE_FIELD_FLAGS, 5);
		pCorpse->SetUInt64Value(CORPSE_FIELD_OWNER, 0); // remove corpse owner association
		//remove item association
		for (int i = 0; i < EQUIPMENT_SLOT_END; i++)
		{
			if(pCorpse->GetUInt32Value(CORPSE_FIELD_ITEM + i))
				pCorpse->SetUInt32Value(CORPSE_FIELD_ITEM + i, 0);
		}
		pCorpse->SetCorpseState(CORPSE_STATE_BONES);
	}
	else
	{
		//save corpse in db for future use
		if(m_mapMgr && m_mapMgr->GetMapInfo()->type == INSTANCE_NULL)
			pCorpse->SaveToDB();

		myCorpse = pCorpse;

		// insignia stuff
		if( m_bg != NULL && m_bg->SupportsPlayerLoot() )
		{
			if( !m_insigniaTaken )
				pCorpse->SetFlag(CORPSE_FIELD_FLAGS, 60);
		}
	}

	*/

	// spawn
	if( m_mapMgr == NULL )
		pCorpse->AddToWorld();
	else
		pCorpse->PushToWorld(m_mapMgr);

	// add deletion event if bone corpse
	if( pCorpse->GetUInt64Value(CORPSE_FIELD_OWNER) == 0 )
		objmgr.CorpseAddEventDespawn(pCorpse);

	//save corpse in db for future use
	pCorpse->SaveToDB();

	return pCorpse;
}

void Player::SpawnCorpseBody()
{
	Corpse *pCorpse;

	pCorpse = objmgr.GetCorpseByOwner(this->GetLowGUID());
	if(pCorpse && !pCorpse->IsInWorld())
	{
		if(bShouldHaveLootableOnCorpse && pCorpse->GetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS) != 1)
			pCorpse->SetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS, 1); // sets it so you can loot the plyr

		if(m_mapMgr == 0)
			pCorpse->AddToWorld();
		else
			pCorpse->PushToWorld(m_mapMgr);
	}
	myCorpse = pCorpse;
}

void Player::SpawnCorpseBones()
{
	Corpse *pCorpse;
	pCorpse = objmgr.GetCorpseByOwner(GetLowGUID());
	myCorpse = 0;
	if(pCorpse)
	{
		if (pCorpse->IsInWorld() && pCorpse->GetCorpseState() == CORPSE_STATE_BODY)
		{
			if(pCorpse->GetInstanceID() != GetInstanceID())
			{
				sEventMgr.AddEvent(pCorpse, &Corpse::SpawnBones, EVENT_CORPSE_SPAWN_BONES, 100, 1,0);
			}
			else
				pCorpse->SpawnBones();
		}
		else
		{
			//Cheater!
		}
	}
}

void Player::DeathDurabilityLoss(double percent)
{
	m_session->OutPacket(SMSG_DURABILITY_DAMAGE_DEATH);
	uint32 pDurability;
	uint32 pMaxDurability;
	int32 pNewDurability;
	Item * pItem;

	for (int8 i = 0; i < EQUIPMENT_SLOT_END; i++)
	{
		pItem = GetItemInterface()->GetInventoryItem(i);
		if(pItem)
		{
			pMaxDurability = pItem->GetUInt32Value(ITEM_FIELD_MAXDURABILITY);
			pDurability =  pItem->GetUInt32Value(ITEM_FIELD_DURABILITY);
			if(pDurability)
			{
				pNewDurability = (uint32)(pMaxDurability*percent);
				pNewDurability = (pDurability - pNewDurability);
				if(pNewDurability < 0) 
					pNewDurability = 0;

				if(pNewDurability <= 0) 
				{ 
					ApplyItemMods(pItem, i, false, true);
				}

				pItem->SetUInt32Value(ITEM_FIELD_DURABILITY,(uint32)pNewDurability);
				pItem->m_isDirty = true;
			}
		}
	}
}

void Player::RepopAtGraveyard(float ox, float oy, float oz, uint32 mapid)
{   
	bool first = true;
	//float closestX = 0, closestY = 0, closestZ = 0, closestO = 0;
	StorageContainerIterator<GraveyardTeleport> * itr;


	//Log.Warning("RepopAtGraveyard","%s recherche le cim le plus proche.",this->GetName());
	//Log.Warning("                ","mapid: %u, X: %0.5f, Y:%0.5f, Z: %0.5f",mapid,ox,oy,oz);

	LocationVector src(ox, oy, oz);
	LocationVector dest(0, 0, 0, 0);
	LocationVector temp;
	float closest_dist = 999999.0f;
	float dist;

	if(m_bg && m_bg->HookHandleRepop(this))
	{
		return;
	}
	else
	{
/*		uint32 areaid = sInstanceMgr.GetMap(mapid)->GetAreaID(ox,oy,oz);
		AreaTable * at = dbcArea.LookupEntry(areaid);
		if(!at) 
		{
            //Log.Warning("RepopAtGraveyard","Zone non trouvee.");
			return;
		}

		uint32 mzone = ( at->ZoneId ? at->ZoneId : at->AreaId);
*/
		itr = GraveyardStorage.MakeIterator();
		GraveyardTeleport *pGrave = NULL;
		while(!itr->AtEnd())
		{
			pGrave = itr->Get();
			if(pGrave->MapId == mapid && pGrave->FactionID == GetTeam() ||
			   pGrave->MapId == mapid && pGrave->FactionID == 3)
			{
				//Log.Warning("                ","Cim proche: %u",pGrave->ID);
				temp.ChangeCoords(pGrave->X, pGrave->Y, pGrave->Z);
				dist = src.DistanceSq(temp);
				if( first || dist < closest_dist )
				{
					first = false;
					closest_dist = dist;
					dest = temp;
				}
			}

			if(!itr->Inc())
				break;
		}
		
		/* Fix Arcmu : defaults to last graveyard, if none fit the criteria.
		Keeps the player from hanging out to dry. */
		if(first && pGrave != NULL)//crappy Databases with no graveyards.
		{
			dest.ChangeCoords(pGrave->X, pGrave->Y, pGrave->Z);
			first = false;
		}
		
		itr->Destruct();
	}

	if(/*sHookInterface.OnRepop(this) && */!first)
	{
		SafeTeleport(mapid, 0, dest);
	}


//	//correct method as it works on official server, and does not require any damn sql
//	//no factions! no zones! no sqls! 1word: blizz-like
//	float closestX , closestY , closestZ ;
//	uint32 entries=sWorldSafeLocsStore.GetNumRows();
//	GraveyardEntry*g;
//	uint32 mymapid=mapid
//	float mx=ox,my=oy;
//	float last_distance=9e10;
//
//	for(uint32 x=0;x<entries;x++)
//	{
//		g=sWorldSafeLocsStore.LookupEntry(x);
//		if(g->mapid!=mymapid)continue;
//		float distance=(mx-g->x)*(mx-g->x)+(my-g->y)*(my-g->y);
//		if(distance<last_distance)
//		{
//			closestX=g->x;
//			closestY=g->y;
//			closestZ=g->z;
//			last_distance=distance;
//		}
//	
//	
//	}
//	if(last_distance<1e10)
//#endif

	
}

#ifdef CLUSTERING
void Player::JoinedChannel(uint32 channelId)
{
	m_channels.insert(channelId);
}

void Player::LeftChannel(uint32 channelId)
{
	m_channels.erase(channelId);
}
#else
void Player::JoinedChannel(Channel *c)
{
	c = channelmgr.GetChannel("LookingForGroup",this);
	if(!c)
		return;
}

void Player::LeftChannel(Channel *c)
{
	c = channelmgr.GetChannel("LookingForGroup",this);
	if(!c)
		return;
}
#endif

void Player::CleanupChannels()
{
#ifdef CLUSTERING
	std::set<uint32>::iterator i;
	std::vector<uint32> channels;
	uint32 cid;

	for(i = m_channels.begin(); i != m_channels.end();)
	{
		cid = *i;
		++i;
		channels.push_back(cid);
	}
	WorldPacket data(ICMSG_CHANNEL_UPDATE, (sizeof(std::vector<uint32>::size_type) * channels.size()) + 5);
	data << uint8(PART_ALL_CHANNELS); //part all channels
	data << GetLowGUID();
	data << channels;
	sClusterInterface.SendPacket(&data);
#else
	set<Channel *>::iterator i;
	Channel * c;
	for(i = m_channels.begin(); i != m_channels.end();)
	{
		c = *i;
		++i;
		
		c->Part(this);
	}
#endif
}

void Player::SendInitialActions()
{
/*#ifndef USING_BIG_ENDIAN
	m_session->OutPacket(SMSG_ACTION_BUTTONS, PLAYER_ACTION_BUTTON_SIZE, &mActions);
#else
	// we can't do this the fast way on ppc, due to endianness 
	WorldPacket data(SMSG_ACTION_BUTTONS, PLAYER_ACTION_BUTTON_SIZE);
	for(uint32 i = 0; i < PLAYER_ACTION_BUTTON_SIZE; ++i)
	{
		data << mActions[i].Action << mActions[i].Type << mActions[i].Misc;
	}
	m_session->SendPacket(&data);
#endif
*/
	uint8 buffer[PLAYER_ACTION_BUTTON_SIZE + 1];
	StackPacket data(SMSG_ACTION_BUTTONS, buffer, PLAYER_ACTION_BUTTON_SIZE + 1);
	data << uint8(1);	// some bool - 0 or 1. seems to work both ways
	data.Write((uint8*)&mActions, PLAYER_ACTION_BUTTON_SIZE);
	m_session->SendPacket(&data);
	//m_session->OutPacket(SMSG_ACTION_BUTTONS, PLAYER_ACTION_BUTTON_SIZE, &mActions);
}

void Player::setAction(uint8 button, uint16 action, uint8 type, uint8 misc)
{
	if (button >= PLAYER_ACTION_BUTTON_COUNT - 1)
		return;

	mActions[button].Action = action;
	mActions[button].Type = type;
	mActions[button].Misc = misc;
}

//Groupcheck
bool Player::IsGroupMember(Player *plyr)
{
	if(m_playerInfo->m_Group != NULL)
		return m_playerInfo->m_Group->HasMember(plyr->m_playerInfo);

	return false;
}

int32 Player::GetOpenQuestSlot()
{
	for (uint32 i = 0; i < 25; ++i)
		if (m_questlog[i] == NULL)
			return i;
	return -1;
}

void Player::AddToFinishedQuests(uint32 quest_id)
{
	if (m_finishedQuests.find(quest_id) != m_finishedQuests.end())
		return;

	m_finishedQuests.insert(quest_id);
	GetAchievementInterface()->HandleAchievementCriteriaQuestCount( (uint32)m_finishedQuests.size() );
}

bool Player::HasFinishedQuest(uint32 quest_id)
{
	return (m_finishedQuests.find(quest_id) != m_finishedQuests.end());
}

bool Player::GetQuestRewardStatus(uint32 quest_id)
{
	return HasFinishedQuest(quest_id);
}

// From HearthStone
void Player::AddToFinishedDailyQuests(uint32 quest_id)
{
	if(m_finishedDailyQuests.size() >= 25)
		return;

	DailyMutex.Acquire();
	SetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + (uint32)m_finishedDailyQuests.size(), quest_id);
	m_finishedDailyQuests.insert(quest_id);
	DailyMutex.Release();
}

bool Player::HasFinishedDailyQuest(uint32 quest_id)
{
	return (m_finishedDailyQuests.find(quest_id) != m_finishedDailyQuests.end());
}

void Player::_LoadSkills(QueryResult * result)
{	
	int32 proff_counter = 2;
	if(result)
	{
		const ItemProf * prof;
		do 
		{
			PlayerSkill sk;
			uint32 v1;
			Field *fields = result->Fetch();
			v1 = fields[1].GetUInt32();
			sk.Reset(v1);

			if ( sWorld.CheckProfessions && fields[2].GetUInt32() == SKILL_TYPE_PROFESSION )
			{
				proff_counter--;
				if( proff_counter < 0 )
				{
					proff_counter = 0;
					
					sCheatLog.writefromsession(GetSession(),"Bug_Primary_Professions Player %s [%d] \n", GetName(), GetLowGUID());

					const char * message = "Your character has more then 2 primary professions.\n You have 5 minutes remaining to unlearn and relog.";

					// Send warning after 2 minutes, as he might miss it if it's send inmedeately.
					sEventMgr.AddEvent( this, &Player::_Warn, message, EVENT_UNIT_SENDMESSAGE, 60000, 1, 0);
					sEventMgr.AddEvent( this, &Player::_Kick, EVENT_PLAYER_KICK, 360000, 1, 0 );
				}
			}
			sk.CurrentValue = fields[3].GetUInt32();
			sk.MaximumValue = fields[4].GetUInt32();
			m_skills.insert(make_pair(v1,sk));

			prof = GetProficiencyBySkill(v1);
			if(prof)
			{
				if(prof->itemclass==4)
					armor_proficiency|=prof->subclass;
				else
					weapon_proficiency|=prof->subclass;
			}
		} while(result->NextRow());
	}
	else // no result from db set up from create_info instead
	{
		for(std::list<CreateInfo_SkillStruct>::iterator ss = info->skills.begin(); ss!=info->skills.end(); ss++)
		{
			if(ss->skillid && ss->currentval && ss->maxval && !::GetSpellForLanguage(ss->skillid))
				_AddSkillLine(ss->skillid, ss->currentval, ss->maxval);		
		}
	}
	//Update , GM's can still learn more
	SetUInt32Value( PLAYER_CHARACTER_POINTS2, ( GetSession()->HasGMPermissions()? 2 : proff_counter ) );
	_UpdateMaxSkillCounts();
}

//From Mangos Project
void Player::_LoadTutorials(QueryResult * result)
{	
	if(result)
	{
		 Field *fields = result->Fetch();
		 for (int iI=0; iI<8; iI++) 
			 m_Tutorials[iI] = fields[iI + 1].GetUInt32();
	}
	tutorialsDirty = false;
}

void Player::_SaveTutorials(QueryBuffer * buf)
{
	if(tutorialsDirty)
	{
		if(buf == NULL)
			CharacterDatabase.Execute("REPLACE INTO tutorials VALUES('%u','%u','%u','%u','%u','%u','%u','%u','%u')", GetLowGUID(), m_Tutorials[0], m_Tutorials[1], m_Tutorials[2], m_Tutorials[3], m_Tutorials[4], m_Tutorials[5], m_Tutorials[6], m_Tutorials[7]);
		else
			buf->AddQuery("REPLACE INTO tutorials VALUES('%u','%u','%u','%u','%u','%u','%u','%u','%u')", GetLowGUID(), m_Tutorials[0], m_Tutorials[1], m_Tutorials[2], m_Tutorials[3], m_Tutorials[4], m_Tutorials[5], m_Tutorials[6], m_Tutorials[7]);

		tutorialsDirty = false;
	}
}

uint32 Player::GetTutorialInt(uint32 intId )
{
	//ASSERT( intId < 8 ); (Nettoyage Assert) (Branruz)
	if(intId < 8) return m_Tutorials[intId];
	else          return m_Tutorials[0];
}

void Player::SetTutorialInt(uint32 intId, uint32 value)
{
	if(intId >= 8) return;

	// ASSERT( (intId < 8) ); ne sert a rien
	m_Tutorials[intId] = value;
	tutorialsDirty = true;
}

//Player stats calculation for saving at lvl up, etc
/*void Player::CalcBaseStats()
{//static_cast< Player* >( this )->getClass() == HUNTER ||
	//TODO take into account base stats at create
	uint32 AP, RAP;
	//Save AttAck power
	if(getClass() == ROGUE || getClass() == HUNTER || (getClass()==WARRIOR && sWorld.m_WarriorPetAllowed))
	{
		AP = GetBaseUInt32Value(UNIT_FIELD_STAT0) + GetBaseUInt32Value(UNIT_FIELD_STAT1);
		RAP = (GetBaseUInt32Value(UNIT_FIELD_STAT1) * 2);
		SetBaseUInt32Value(UNIT_FIELD_ATTACK_POWER, AP);
		SetBaseUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER, RAP);
	}
	else
	{
		AP = (GetBaseUInt32Value(UNIT_FIELD_STAT0) * 2);
		RAP = (GetBaseUInt32Value(UNIT_FIELD_STAT1) * 2);
		SetBaseUInt32Value(UNIT_FIELD_ATTACK_POWER, AP);
		SetBaseUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER, RAP);
	}

}*/

void Player::UpdateHit(int32 hit)
{
   /*std::list<Affect*>::iterator i;
	Affect::ModList::const_iterator j;
	Affect *aff;
	uint32 in = hit;
	for (i = GetAffectBegin(); i != GetAffectEnd(); i++)
	{
		aff = *i;
		for (j = aff->GetModList().begin();j != aff->GetModList().end(); j++)
		{
			Modifier mod = (*j);
			if ((mod.GetType() == SPELL_AURA_MOD_HIT_CHANCE))
			{
				SpellEntry *spellInfo = sSpellStore.LookupEntry(aff->GetSpellId());
				if (this->canCast(spellInfo))
					in += mod.GetAmount();
			}
		}
	}
	SetHitFromSpell(in);*/
}

void Player::UpdateChances()
{
	uint32 pClass = (uint32)getClass();
	uint32 pLevel = (getLevel() > sWorld.m_levelCap) ? sWorld.m_levelCap : getLevel();

	float tmp = 0;
	float defence_contribution = 0;

	// defence contribution estimate
	defence_contribution = ( float( _GetSkillLineCurrent( SKILL_DEFENSE, true ) ) - ( float( pLevel ) * 5.0f ) ) * 0.04f;
	defence_contribution += CalcRating( PLAYER_RATING_MODIFIER_DEFENCE ) * 0.04f;

	// dodge
	tmp = baseDodge[pClass] + float( GetUInt32Value( UNIT_FIELD_STAT1 ) / dodgeRatio[pLevel-1][pClass] );
	tmp += CalcRating( PLAYER_RATING_MODIFIER_DODGE ) + this->GetDodgeFromSpell();
	tmp += defence_contribution;
	if( tmp < 0.0f )tmp = 0.0f;

	//SetFloatValue( PLAYER_DODGE_PERCENTAGE, min( tmp, 95.0f ) );
	SetFloatValue( PLAYER_DODGE_PERCENTAGE, RandomFloat( min( tmp, 95.0f ) ) ); // Test 

	// block
	Item* it = this->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
	if( it != NULL && it->GetProto()->InventoryType == INVTYPE_SHIELD )
	{
		tmp = 5.0f + CalcRating( PLAYER_RATING_MODIFIER_BLOCK ) + GetBlockFromSpell();
		tmp += defence_contribution;
		if( tmp < 0.0f )tmp = 0.0f;
	}
	else
		tmp = 0.0f;
	
	SetFloatValue( PLAYER_BLOCK_PERCENTAGE, min( tmp, 95.0f ) );

	//parry
	tmp = 5.0f + CalcRating( PLAYER_RATING_MODIFIER_PARRY ) + GetParryFromSpell();
	if(pClass == DEATHKNIGHT) // DK gets 1/4 of strength as parry rating
	{
		tmp += CalcPercentForRating(PLAYER_RATING_MODIFIER_PARRY, GetUInt32Value(UNIT_FIELD_STAT0) / 4);
	}	
	tmp += defence_contribution;
	if( tmp < 0.0f )tmp = 0.0f;

	SetFloatValue( PLAYER_PARRY_PERCENTAGE, std::max( 0.0f, std::min( tmp, 95.0f ) ) ); //let us not use negative parry. Some spells decrease it

	//critical
	gtFloat* baseCrit = dbcMeleeCritBase.LookupEntry(pClass-1);
	gtFloat* CritPerAgi = dbcMeleeCrit.LookupEntry(pLevel - 1 + (pClass-1)*100);

	tmp = 100*(baseCrit->val + GetUInt32Value( UNIT_FIELD_STAT1 ) * CritPerAgi->val);

	//std::list<WeaponModifier>::iterator i = tocritchance.begin();
	map< uint32, WeaponModifier >::iterator itr = tocritchance.begin();

	Item* tItemMelee = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
	Item* tItemRanged = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );

	float melee_bonus = 0;
	float ranged_bonus = 0;

	//-1 = any weapon

	for(; itr != tocritchance.end(); ++itr )
	{
		if( itr->second.wclass == ( uint32 )-1 || ( tItemMelee != NULL && ( 1 << tItemMelee->GetProto()->SubClass & itr->second.subclass ) ) )
		{
			melee_bonus += itr->second.value;
		}
		if( itr->second.wclass == ( uint32 )-1 || ( tItemRanged != NULL && ( 1 << tItemRanged->GetProto()->SubClass & itr->second.subclass ) ) )
		{
			ranged_bonus += itr->second.value;
		}
	}

	float cr = tmp + CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT ) + melee_bonus;
	SetFloatValue( PLAYER_CRIT_PERCENTAGE, min( cr, 95.0f ) );

	float rcr = tmp + CalcRating( PLAYER_RATING_MODIFIER_RANGED_CRIT ) + ranged_bonus;
	SetFloatValue( PLAYER_RANGED_CRIT_PERCENTAGE, min( rcr, 95.0f ) );

	gtFloat* SpellCritBase  = dbcSpellCritBase.LookupEntry(pClass-1);
	gtFloat* SpellCritPerInt = dbcSpellCrit.LookupEntry(pLevel - 1 + (pClass-1)*100);

	spellcritperc = 100*(SpellCritBase->val + GetUInt32Value( UNIT_FIELD_STAT3 ) * SpellCritPerInt->val) +
		this->GetSpellCritFromSpell() +
		this->CalcRating( PLAYER_RATING_MODIFIER_SPELL_CRIT );
	UpdateChanceFields();
}

void Player::UpdateChanceFields()
{
	// Update spell crit values in fields
	for(uint32 i = 0; i < 7; ++i)
	{
		SetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + i, SpellCritChanceSchool[i]+spellcritperc);
	}
}

void Player::UpdateAttackSpeed()
{
	uint32 speed=2000;
	Item *weap ;
	if(GetShapeShift()==FORM_CAT)//cat form
	{
		speed = 1000;
	}else if(GetShapeShift()==FORM_BEAR || GetShapeShift()==FORM_DIREBEAR)
	{
		speed = 2500;
	}
	else //regular
	if( !disarmed )
	{
		weap = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND) ;
		if( weap != NULL )
			speed = weap->GetProto()->Delay;
	}
	SetUInt32Value( UNIT_FIELD_BASEATTACKTIME, 
		( uint32 )( ( speed * (( 100.0f - ( float )m_meleeattackspeedmod ) / 100.0f ) ) * ( ( 100.0f - CalcRating( PLAYER_RATING_MODIFIER_MELEE_HASTE ) ) / 100.0f ) ) );
	
	weap = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
	if( weap != NULL && weap->GetProto()->Class == ITEM_CLASS_WEAPON) // 2 is a weapon  
	{
		speed = weap->GetProto()->Delay;
		//SetUInt32Value( UNIT_FIELD_BASEATTACKTIME_01, 
		SetUInt32Value( UNIT_FIELD_RANGEDATTACKTIME, 
					    ( uint32 )( ( speed * ( ( 100.0f - ( float )m_meleeattackspeedmod ) / 100.0f ) ) * ( ( 100.0f - CalcRating( PLAYER_RATING_MODIFIER_MELEE_HASTE ) ) / 100.0f ) ) );
	}
	  
	weap = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
	if( weap != NULL )
	{
		speed = weap->GetProto()->Delay;
		SetUInt32Value( UNIT_FIELD_RANGEDATTACKTIME,
					    ( uint32 )( ( speed * ( ( 100.0f - (float)m_rangedattackspeedmod ) / 100.0f ) ) * ( ( 100.0f - CalcRating(PLAYER_RATING_MODIFIER_RANGED_HASTE)) / 100.0f ) ) );
	}
}

void Player::UpdateStats()
{   
	UpdateAttackSpeed();

	// formulas from wowwiki - Calcul de la puissance d'attaque (Attaque Power)
    //Log.Warning("Player::UpdateStats","....");

	int32 AP = 0;
	int32 RAP = 0;
	int32 stam = 0;
	// --- Notes Randdrick :  Nouvelles variables
	int32 BaseHealthStam = 0; // HPs génénérés par l'endurance : DEJA INCLUS DANS LA BASE HP
	int32 BonMalStam = 0; // Bonus-Malus Endurance ( Stamina)
	int32 TotalStam = 0; // Total Stamina
	int32 BonMal = 0;	// Bonus-Malus
	// --- Fin nouvelles variables
	int32 intl = 0;

	uint32 str = GetUInt32Value(UNIT_FIELD_STAT0);
	uint32 agi = GetUInt32Value(UNIT_FIELD_STAT1);
	uint32 lev = getLevel();

	// Attack power
	uint32 cl = getClass();   
	switch (cl)
	{
	case DRUID:
        //(Strength x 2) - 20           
        AP = str * 2 - 20;
        //Agility - 10
        RAP = agi - 10;
    
        if( GetShapeShift() == FORM_MOONKIN )
        {
            //(Strength x 2) + (Character Level x 1.5) - 20
            AP += float2int32( static_cast<float>(lev) * 1.5f );
        }
        if( GetShapeShift() == FORM_CAT )
		{
            //(Strength x 2) + Agility + (Character Level x 2) - 20
            AP += agi + (lev *2);
        }
        if( GetShapeShift() == FORM_BEAR || GetShapeShift() == FORM_DIREBEAR )
        {
            //(Strength x 2) + (Character Level x 3) - 20
            AP += (lev *3);
		}
		break;


	case ROGUE:
		//Strength + Agility + (Character Level x 2) - 20
		AP = str + agi + (lev *2) - 20;
		//Character Level + Agility - 10
		RAP = lev + agi - 10;

		break;


	case HUNTER:
		//Strength + Agility + (Character Level x 2) - 20
		 AP = str + agi + (lev *2) - 20;
		//(Character Level x 2) + Agility - 10
		RAP = (lev *2) + agi - 10;

		break;

	case SHAMAN:   
		//(Strength) + (Agility) + (Character Level x 2) - 20
		AP = str + agi + (lev *2) - 20;
		//Agility - 10
		RAP = agi - 10;
		
		break;

	case PALADIN:
		//(Strength x 2) + (Character Level x 3) - 20
		AP = (str *2) + (lev *3) - 20;
		//Agility - 10
		RAP = agi - 10;
	
		break;

	case WARRIOR:
		//(Strength x 2) + (Character Level x 3) - 20
		AP = (str *2) + (lev *3) - 20;
		//Character Level + Agility - 10
		RAP = lev + agi - 10;
		
		break;
		
	case DEATHKNIGHT:
		//(Strength x2) - 20
		AP = (str *2) - 20;
		break;

	default:    //mage,priest,warlock
		//Strength - 10
		AP = str - 10;
		//Agility - 10 	
		RAP = agi - 10;
	}

	/* modifiers */
	RAP += int32(float(float(m_rap_mod_pct) * float(float(m_uint32Values[UNIT_FIELD_STAT3]) / 100.0f)));

	if( RAP < 0 ) RAP = 0;
	if( AP < 0 )  AP = 0;

	SetUInt32Value( UNIT_FIELD_ATTACK_POWER, AP );
	SetUInt32Value( UNIT_FIELD_RANGED_ATTACK_POWER, RAP ); 

	LevelInfo* lvlinfo = objmgr.GetLevelInfo( this->getRace(), this->getClass(), lev );

	if( lvlinfo != NULL )
	{
		stam = lvlinfo->Stat[STAT_STAMINA];
		intl = lvlinfo->Stat[STAT_INTELLECT];
		BaseHealthStam = (std::max(20, stam)) + (std::max(0, stam - 20) * 10); // Note randdrick : Formule de calcul des HPs dépendants du Stamina ( endurance ) 		
	}

	int32 hp = GetUInt32Value( UNIT_FIELD_BASE_HEALTH );

	BonMalStam = GetUInt32Value( UNIT_FIELD_POSSTAT2 ) - GetUInt32Value( UNIT_FIELD_NEGSTAT2 );

	if (BonMalStam != NULL) // On ne calcul le bonus-malus QUE SI l'endurance est modifiée.
	{
		TotalStam = stam + BonMalStam;
		if ( TotalStam > 20 )
			BonMal = BonMalStam *10; //  10 points de Hp par point d'endurance > 20
		else if ( TotalStam > 0) // 1 point de Hp supplémentaire par point d'endurance <= 20
			BonMal = (hp - BaseHealthStam ) + TotalStam;
		else if (TotalStam  <= 0) // Evite une endurance négative
			hp = hp - BaseHealthStam; 
	}

	int32 res = hp + /*std::max(20, stam) + std::max(0, stam - 20) * 10 + */ BonMal + m_healthfromspell + m_healthfromitems; // Note Randdrick : le bonus lié à l'endurance est déjà pris en compte dans le calcul des hps
	if (res < 1)
		res = 1;

	int32 oldmaxhp = GetUInt32Value( UNIT_FIELD_MAXHEALTH );

	// if( res < hp ) res = hp; Note Randdrick : un malus peut entrainner un nombre de Hp inférieur à ceux qu'ils devraient être pour un niveau donné
	SetUInt32Value( UNIT_FIELD_MAXHEALTH, res );

	if( ( int32 )GetUInt32Value( UNIT_FIELD_HEALTH ) > res )
		SetUInt32Value( UNIT_FIELD_HEALTH, res );
	else if( ( cl == DRUID) && ( GetShapeShift() == FORM_BEAR || GetShapeShift() == FORM_DIREBEAR ) )
	{
		res = float2int32( ( float )GetUInt32Value( UNIT_FIELD_MAXHEALTH ) * ( float )GetUInt32Value( UNIT_FIELD_HEALTH ) / float( oldmaxhp ) );
		SetUInt32Value( UNIT_FIELD_HEALTH, res );
	}

	//if( cl != WARRIOR && cl != ROGUE )
	if( cl != WARRIOR && cl != ROGUE && cl != DEATHKNIGHT )
	{
		// MP
		int32 mana = GetUInt32Value( UNIT_FIELD_BASE_MANA );

		intl += GetUInt32Value( UNIT_FIELD_POSSTAT3 ) - GetUInt32Value( UNIT_FIELD_NEGSTAT3 );

		res = mana + std::max(20, intl) + std::max(0, intl - 20) * 15 + m_manafromspell + m_manafromitems;
		if( res < mana )
			res = mana;	
		SetUInt32Value(UNIT_FIELD_MAXPOWER1, res);

		if((int32)GetUInt32Value(UNIT_FIELD_POWER1)>res)
			SetUInt32Value(UNIT_FIELD_POWER1,res);

		//Manaregen
		/*const static float ClassMultiplier[12] = {0.0f,0.0f,0.2f,0.2f,0.0f,0.25f,0.0f,0.2f,0.25f,0.2f,0.0f,0.225f};
		const static float ClassFlatMod[12] = {0.0f,0.0f,15.0f,15.0f,0.0f,12.5f,0.0f,15.0f,12.5f,15.0f,0.0f,15.0f};
		uint32 Spirit = GetUInt32Value( UNIT_FIELD_STAT4 );
		float amt = (Spirit*ClassMultiplier[cl]+ClassFlatMod[cl])*PctPowerRegenModifier[POWER_TYPE_MANA]*0.5f;
		SetFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER,amt+m_ModInterrMRegen/5.0f);
		SetFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER,amt*m_ModInterrMRegenPCT/100.0f+m_ModInterrMRegen/5.0f);*/
		
		const float baseRegen[250] = { 
		0.034965f, 0.034191f, 0.033465f, 0.032526f, 0.031661f, 0.031076f, 0.030523f, 0.029994f, 0.029307f, 0.028661f, // lv1 à 10
		0.027584f, 0.026215f, 0.025381f, 0.024300f, 0.023345f, 0.022748f, 0.021958f, 0.021386f, 0.020790f, 0.020121f, // lv11 à 20
		0.019733f, 0.019155f, 0.018819f, 0.018316f, 0.017936f, 0.017576f, 0.017201f, 0.016919f, 0.016581f, 0.016233f, // lv21 à 30
		0.015994f, 0.015707f, 0.015464f, 0.015204f, 0.014956f, 0.014744f, 0.014495f, 0.014302f, 0.014094f, 0.013895f, // lv31 à 40
		0.013724f, 0.013522f, 0.013363f, 0.013175f, 0.012996f, 0.012853f, 0.012687f, 0.012539f, 0.012384f, 0.012233f, // lv41 à 50
		0.012113f, 0.011973f, 0.011859f, 0.011714f, 0.011575f, 0.011473f, 0.011342f, 0.011245f, 0.011110f, 0.010999f, // lv51 à 60
		0.010700f, 0.010522f, 0.010290f, 0.010119f, 0.009968f, 0.009808f, 0.009651f, 0.009553f, 0.009445f, 0.009327f, // lv61 à 70
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv71 à 80
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv81 à 90
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv91 à 100
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv101 à 110
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv111 à 120
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv121 à 130
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv131 à 140
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv141 à 150
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv151 à 160
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv161 à 170
        0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv171 à 180
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv181 à 190
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv191 à 200
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv201 à 210
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv211 à 220
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv221 à 230
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv231 à 240
		0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, 0.009327f, // lv241 à 250
		};

		uint32 lvl = getLevel();
		if(lvl > 250) lvl = 250;
		
		//float amt = 0.001f + sqrtf((float)GetUInt32Value(UNIT_FIELD_STAT3)) * GetUInt32Value(UNIT_FIELD_STAT4) * baseRegen[lev-1];
		float amt = ( 0.001f + sqrt((float)GetUInt32Value( UNIT_FIELD_INTELLECT )) * GetUInt32Value( UNIT_FIELD_SPIRIT ) * baseRegen[lev-1] ) * PctPowerRegenModifier[POWER_TYPE_MANA];
		SetFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER, amt + m_ModInterrMRegen[POWER_TYPE_MANA] / 5.0f);
		SetFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER, amt * m_ModInterrMRegenPCT[POWER_TYPE_MANA] / 100.0f + m_ModInterrMRegen[POWER_TYPE_MANA] / 5.0f);

		
		//the rest are just 0 + flag
		for (uint32 rdi=POWER_TYPE_MANA; rdi<NUM_POWER_TYPES; ++rdi)
		{
			if ( rdi>POWER_TYPE_MANA )
			{
			SetFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER + rdi, m_ModInterrMRegen[POWER_TYPE_MANA + rdi] / 5.0f);
			SetFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER + rdi, m_ModInterrMRegen[POWER_TYPE_MANA + rdi] / 5.0f);
			}
		}
		
	}

	/////////////////////RATINGS STUFF/////////////////
	float cast_speed = CalcRating( PLAYER_RATING_MODIFIER_SPELL_HASTE );
	//if( cast_speed >= 50 ) 		// spell haste/slow is limited to 100% fast
	//	cast_speed = 50;
	if( cast_speed != SpellHasteRatingBonus )
	{
		ModFloatValue( UNIT_MOD_CAST_SPEED, ( SpellHasteRatingBonus - cast_speed ) / 100.0f );

		SpellHasteRatingBonus = cast_speed;
	}
	////////////////////RATINGS STUFF//////////////////////

	// Shield Block
	Item* shield = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
	if( shield != NULL && shield->GetProto()->InventoryType == INVTYPE_SHIELD )
	{
		float block_multiplier = ( 100.0f + float( m_modblockabsorbvalue ) ) / 100.0f;
		if( block_multiplier < 1.0f )block_multiplier = 1.0f;

		int32 blockable_damage = float2int32( float( shield->GetProto()->Block ) +( float( m_modblockvaluefromspells + GetUInt32Value( PLAYER_RATING_MODIFIER_BLOCK ) ) * block_multiplier ) + ( ( float( str ) / 20.0f ) - 1.0f ) );
		SetUInt32Value( PLAYER_SHIELD_BLOCK, blockable_damage );
	}
	else
	{
		SetUInt32Value( PLAYER_SHIELD_BLOCK, 0 );
	}

	UpdateChances();
	CalcDamage();
}

uint32 Player::SubtractRestXP(uint32 amount)
{
	if(GetUInt32Value(UNIT_FIELD_LEVEL) >= GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))		// Save CPU, don't waste time on this if you've reached max_level
		amount = 0;

	int32 restAmount = m_restAmount - (amount << 1); // remember , we are dealing with xp without restbonus, so multiply by 2

	if( restAmount < 0)
		m_restAmount = 0;
	else
		m_restAmount = restAmount;

	Log.Debug("REST","Subtracted %d rest XP to a total of %d", amount, m_restAmount);
	UpdateRestState();		// Update clients interface with new values.
	return amount;
}

void Player::AddCalculatedRestXP(uint32 seconds)
{
	// At level one, players will all start in the normal tier. 
	// When a player rests in a city or at an inn they will gain rest bonus at a very slow rate. 
	// Eight hours of rest will be needed for a player to gain one "bubble" of rest bonus. 
	// At any given time, players will be able to accumulate a maximum of 30 "bubbles" worth of rest bonus which 
	// translates into approximately 1.5 levels worth of rested play (before your character returns to normal rest state).
	// Thanks to the comforts of a warm bed and a hearty meal, players who rest or log out at an Inn will 
	// accumulate rest credit four times faster than players logged off outside of an Inn or City. 
	// Players who log out anywhere else in the world will earn rest credit four times slower.
	// http://www.worldofwarcraft.com/info/basics/resting.html


	// Define xp for a full bar ( = 20 bubbles)
	uint32 xp_to_lvl = uint32(lvlinfo->XPToNextLevel);

	// get RestXP multiplier from config.
	float bubblerate = sWorld.getRate(RATE_RESTXP);

	// One bubble (5% of xp_to_level) for every 8 hours logged out.
	// if multiplier RestXP (from ascent.config) is f.e 2, you only need 4hrs/bubble.
	uint32 rested_xp = uint32(0.05f * xp_to_lvl * ( seconds / (3600 * ( 8 / bubblerate))));

	// if we are at a resting area rest_XP goes 4 times faster (making it 1 bubble every 2 hrs)
	if (m_isResting)
		rested_xp <<= 2;

	// Add result to accumulated rested XP
	m_restAmount += uint32(rested_xp);

	// and set limit to be max 1.5 * 20 bubbles * multiplier (1.5 * xp_to_level * multiplier)
	if (m_restAmount > xp_to_lvl + (uint32)((float)( xp_to_lvl >> 1 ) * bubblerate ))
		m_restAmount = xp_to_lvl + (uint32)((float)( xp_to_lvl >> 1 ) * bubblerate );

	Log.Debug("REST","Add %d rest XP to a total of %d, RestState %d", rested_xp, m_restAmount,m_isResting);

	// Update clients interface with new values.
	UpdateRestState();
}

void Player::UpdateRestState()
{
	if(m_restAmount && (GetUInt32Value(UNIT_FIELD_LEVEL) < GetUInt32Value(PLAYER_FIELD_MAX_LEVEL)))
		m_restState = RESTSTATE_RESTED;
	else
		m_restState = RESTSTATE_NORMAL;

	// Update RestState 100%/200%
	SetUInt32Value(PLAYER_BYTES_2, ((GetUInt32Value(PLAYER_BYTES_2) & 0x00FFFFFF) | (m_restState << 24)));

	//update needle (weird, works at 1/2 rate)
	SetUInt32Value(PLAYER_REST_STATE_EXPERIENCE, m_restAmount >> 1);
}

void Player::ApplyPlayerRestState(bool apply)
{
	if(apply)
	{
		m_restState = RESTSTATE_RESTED;
		m_isResting = true;
		SetFlag(PLAYER_FLAGS, PLAYER_FLAG_RESTING);	//put zzz icon
	}
	else
	{
		m_isResting = false;
		RemoveFlag(PLAYER_FLAGS,PLAYER_FLAG_RESTING);	//remove zzz icon
	}
	UpdateRestState();
}

#define CORPSE_VIEW_DISTANCE 1600 // 40*40

bool Player::CanSee(Object* obj) // * Invisibility & Stealth Detection - Partha *
{
	if (obj == this) return true;

	/* I'm a GM, I can see EVERYTHING! :D */
	//if( bGMTagOn ) return true; // pour le debug, c'est pas terrible (Branruz)

	// We can't see any objects in another phase
	// unless they're in ALL_PHASES
	if( !PhasedCanInteract(obj) ) return false;  // GESTION DU PHASING (Branruz)

	uint32 object_type = obj->GetTypeId();

	if(getDeathState() == CORPSE) // we are dead and we have released our spirit
	{
		if(object_type == TYPEID_PLAYER)
		{
			Player *pObj = static_cast< Player* >(obj);

			if(myCorpse && myCorpse->GetDistanceSq(obj) <= CORPSE_VIEW_DISTANCE)
				return !pObj->m_isGmInvisible; // we can see all players within range of our corpse except invisible GMs

			if(m_deathVision) // if we have arena death-vision we can see all players except invisible GMs
				return !pObj->m_isGmInvisible;

			return (pObj->getDeathState() == CORPSE); // we can only see players that are spirits
		}

		if(myCorpse)
		{
			if(myCorpse == obj) 
				return true;

			if(myCorpse->GetDistanceSq(obj) <= CORPSE_VIEW_DISTANCE)
				return true; // we can see everything within range of our corpse
		}

		if(m_deathVision) // if we have arena death-vision we can see everything
		{
			if(obj->IsPlayer() && static_cast<Player *>(obj)->IsStealth())
				return false;

			return true;
		}

		if(object_type == TYPEID_UNIT)
		{
			Unit *uObj = static_cast<Unit *>(obj);

			return(uObj->IsSpiritHealer()); // we can't see any NPCs except spirit-healers
		}

		return false;
	}
	//------------------------------------------------------------------
	if (!(m_Phase & obj->m_Phase)) //What you can't see, you can't see, no need to check things further.
		return false;
		
	// mage invisibility - ugh. - Hearstone
	/*
	if( IsPlayer() && m_mageInvisibility )
	{
		if( object_type == TYPEID_PLAYER )
			return static_cast<Player *>(obj)->m_mageInvisibility;

		if( object_type == TYPEID_UNIT )
			return false;

		return true;
	}*/
	
	switch(object_type) // we are alive or we haven't released our spirit yet
	{			
		case TYPEID_PLAYER:
			{
				Player *pObj = static_cast< Player* >(obj);

				if(pObj->m_auracount[SPELL_AURA_MOD_INVISIBILITY]) // Invisibility - Detection of Players
				{
					if(pObj->getDeathState() == CORPSE)
						return bGMTagOn; // only GM can see players that are spirits

					if(GetGroup() && pObj->GetGroup() == GetGroup() // can see invisible group members except when dueling them
							&& DuelingWith != pObj)
						return true;

					if(pObj->stalkedby == GetGUID()) // Hunter's Mark / MindVision is visible to the caster
						return true;

					if(m_invisDetect[INVIS_FLAG_NORMAL] < 1 // can't see invisible without proper detection
							|| pObj->m_isGmInvisible) // can't see invisible GM
						return bGMTagOn; // GM can see invisible players
				}

				if(pObj->IsStealth()) // Stealth Detection (  I Hate Rogues :P  )
				{
					if(GetGroup() && pObj->GetGroup() == GetGroup() // can see stealthed group members except when dueling them
							&& DuelingWith != pObj)
						return true;

					if(pObj->stalkedby == GetGUID()) // Hunter's Mark / MindVision is visible to the caster
						return true;

					if(isInFront(pObj)) // stealthed player is in front of us
					{
						// Detection Range = 5yds + (Detection Skill - Stealth Skill)/5
						if(getLevel() < sWorld.m_levelCap) // 70)
							detectRange = 5.0f + getLevel() + 0.2f * (float)(GetStealthDetectBonus() - pObj->GetStealthLevel());
						else
							detectRange = 75.0f + 0.2f * (float)(GetStealthDetectBonus() - pObj->GetStealthLevel());
						// Hehe... stealth skill is increased by 5 each level and detection skill is increased by 5 each level too.
						// This way, a level 80 should easily be able to detect a level 4 rogue (level 4 because that's when you get stealth)
						//	detectRange += 0.2f * ( getLevel() - pObj->getLevel() );
						if(detectRange < 1.0f) detectRange = 1.0f; // Minimum Detection Range = 1yd
					}
					else // stealthed player is behind us
					{
						if(GetStealthDetectBonus() > 1000) return true; // immune to stealth
						else detectRange = 0.0f;
					}

					detectRange += GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS); // adjust range for size of player
					if(pObj->IsUnit()) detectRange += pObj->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS); // adjust range for size of stealthed player
					//sLog.outString( "Player::CanSee(%s): detect range = %f yards (%f ingame units), cansee = %s , distance = %f" , pObj->GetName() , detectRange , detectRange * detectRange , ( GetDistance2dSq(pObj) > detectRange * detectRange ) ? "yes" : "no" , GetDistanceSq(pObj) );
					if(GetDistanceSq(pObj) > detectRange * detectRange)
						return bGMTagOn; // GM can see stealthed players
				}

				return true;
			}
		//------------------------------------------------------------------

		case TYPEID_UNIT:
			{	
				Unit *uObj = static_cast<Unit *>(obj);
					
				if(uObj->IsSpiritHealer()) // can't see spirit-healers when alive
					return false;

				if(uObj->m_auracount[SPELL_AURA_MOD_INVISIBILITY]// Invisibility - Detection of Units
						&& m_invisDetect[uObj->m_invisFlag] < 1) // can't see invisible without proper detection
					return bGMTagOn; // GM can see invisible units

				return true;
			}
		//------------------------------------------------------------------

		case TYPEID_GAMEOBJECT:
			{
				GameObject *gObj = static_cast<GameObject *>(obj);

				if(gObj->invisible) // Invisibility - Detection of GameObjects
				{
					uint64 owner = gObj->GetUInt64Value(OBJECT_FIELD_CREATED_BY);

					if(GetGUID() == owner) // the owner of an object can always see it
						return true;

					if(GetGroup())
					{
						// OBSOLETE (Branruz)
						//PlayerInfo * inf = objmgr.GetPlayerInfo((uint32)owner);
						//if(inf && GetGroup()->HasMember(inf)) return true;

						Player * gplr = NULL;
						gplr = GetMapMgr()->GetPlayer((uint32)owner);
						if(gplr != NULL && GetGroup()->HasMember(gplr)) return true;
					}

					if(m_invisDetect[gObj->invisibilityFlag] < 1) // can't see invisible without proper detection
						return bGMTagOn; // GM can see invisible objects
				}

				return true;
			}					
		//------------------------------------------------------------------

		default:
			return true;
	}
}

void Player::AddInRangeObject(Object* pObj)
{
	//Send taxi move if we're on a taxi
	if (m_CurrentTaxiPath && (pObj->GetTypeId() == TYPEID_PLAYER))
	{
		uint32 ntime = getMSTime();

		if (ntime > m_taxi_ride_time)
			m_CurrentTaxiPath->SendMoveForTime( this, static_cast< Player* >( pObj ), ntime - m_taxi_ride_time);
		/*else
			m_CurrentTaxiPath->SendMoveForTime( this, static_cast< Player* >( pObj ), m_taxi_ride_time - ntime);*/
	}

	Unit::AddInRangeObject(pObj);

	//if the object is a unit send a move packet if they have a destination
	if(pObj->GetTypeId() == TYPEID_UNIT)
	{
		//add an event to send move update have to send guid as pointer was causing a crash :(
		//sEventMgr.AddEvent( static_cast< Creature* >( pObj )->GetAIInterface(), &AIInterface::SendCurrentMove, this->GetGUID(), EVENT_UNIT_SENDMOVE, 200, 1);
		static_cast< Creature* >( pObj )->GetAIInterface()->SendCurrentMove(this);
	}


	//unit based objects, send aura data
	if (pObj->IsUnit())
	{
		Unit* u=static_cast<Unit*>(pObj);

		
		if (GetSession() != NULL)
		{
			for (uint32 i=0; i<MAX_AURAS; ++i)
			{
				if (u->m_auras[i] != NULL)
				{
					uint8 flags = u->m_auras[i]->IsPositive()? 0x1F : 0x09; // Pas sur que les flags soit les bons

					if (u->m_auras[i]->GetDuration(INVOC_DURATION) > 0) flags |= 0x20;
					WorldPacket data(SMSG_AURA_UPDATE_ALL, 28 * MAX_AURAS);
					FastGUIDPack(data, u->GetGUID());
					data << (uint8)u->m_auras[i]->m_visualSlot; //flags unknown
					data << (uint32)u->m_auras[i]->m_spellProto->Id;
					if (flags & 0x20)
					{
						data << (int32)u->m_auras[i]->GetDuration(INVOC_DURATION);
						data << (uint32)u->m_auras[i]->GetTimeLeft(INVOC_DURATION);
					}
					data << flags; 
					data << (uint32)u->m_auras[i]->m_stackcount;
					GetSession()->SendPacket(&data);
				}
			}
		}
	}
}

void Player::OnRemoveInRangeObject(Object* pObj)
{
	//if (/*!CanSee(pObj) && */IsVisible(pObj))
	//{
		//RemoveVisibleObject(pObj);
	//}
	/* En commentaire pour l'instant (Branruz)
	if(m_tempSummon == pObj)
	{
		m_tempSummon->RemoveFromWorld(false, true);
		if(m_tempSummon)
			m_tempSummon->SafeDelete();

		m_tempSummon = 0;
		SetUInt64Value(UNIT_FIELD_SUMMON, 0);
	}*/

	m_visibleObjects.erase(pObj);
	Unit::OnRemoveInRangeObject(pObj);

	if( pObj == m_CurrentCharm )
	{
		/*Note Randdrick : Faut pas oublier de commenter celui-là aussi, Branruz
		Unit * p = m_CurrentCharm; */

		if(pObj == m_CurrentVehicle)
		{
			m_CurrentVehicle->RemovePassenger( this );
		}
		else
			this->UnPossess();
		if(m_currentSpell)
			m_currentSpell->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
		m_CurrentCharm=NULL;

		/* Ne sert a rien, en commentaire pour l'instant (Branruz)
		if( p->m_temp_summon&&p->GetTypeId() == TYPEID_UNIT )
			static_cast< Creature* >( p )->SafeDelete();
			*/
	}
 
	// En fait le pet devrait suivre le player s'il est tp(Branruz)
	// et avoir un renvoi si la distance est perdu.....
	//
	if(pObj == m_Summon)
	{
		if(m_Summon->IsSummon())
		{
			m_Summon->Dismiss(true);
		}
		else
		{
			m_Summon->Remove(true, true, false);
		}
		if(m_Summon)
		{
			m_Summon->ClearPetOwner();
			m_Summon = 0;
		}
	}

	/* wehee loop unrolling */
/*	if(m_spellTypeTargets[0] == pObj)
		m_spellTypeTargets[0] = NULL;
	if(m_spellTypeTargets[1] == pObj)
		m_spellTypeTargets[1] = NULL;
	if(m_spellTypeTargets[2] == pObj)
		m_spellTypeTargets[2] = NULL;*/
	if(pObj->IsUnit())
	{
		for(uint32 x = 0; x < NUM_SPELL_TYPE_INDEX; ++x)
			if(m_spellIndexTypeTargets[x] == pObj->GetGUID())
				m_spellIndexTypeTargets[x] = 0;
	}
}

void Player::ClearInRangeSet()
{
	m_visibleObjects.clear();
	Unit::ClearInRangeSet();
}

void Player::EventCannibalize(uint32 amount)
{
	uint32 amt = (uint32)((float)(GetUInt32Value(UNIT_FIELD_MAXHEALTH)*amount)/100.0);
	
	uint32 newHealth = GetUInt32Value(UNIT_FIELD_HEALTH) + amt;
	
	if(newHealth <= GetUInt32Value(UNIT_FIELD_MAXHEALTH))
		SetUInt32Value(UNIT_FIELD_HEALTH, newHealth);
	else
		SetUInt32Value(UNIT_FIELD_HEALTH, GetUInt32Value(UNIT_FIELD_MAXHEALTH));

	cannibalizeCount++;
	if(cannibalizeCount == 5)
		SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);

	WorldPacket data(SMSG_PERIODICAURALOG, 38);
	data << GetNewGUID();				   // caster guid
	data << GetNewGUID();				   // target guid
	data << (uint32)(__Cannibalize_Racial);	// spellid 20577
	data << (uint32)1;					  // unknown?? need resource?
	data << (uint32)FLAG_PERIODIC_HEAL;		// aura school
	data << amt;							// amount of done to target / heal / damage
	data << (uint32)0;					  // unknown in some sniff this was 0x0F
	SendMessageToSet(&data, true);
}

void Player::EventReduceDrunk(bool full)
{
	uint8 drunk = ((GetUInt32Value(PLAYER_BYTES_3) >> 8) & 0xFF);
	if(full) drunk = 0;
	else drunk -= 10;
	SetUInt32Value(PLAYER_BYTES_3, ((GetUInt32Value(PLAYER_BYTES_3) & 0xFFFF00FF) | (drunk << 8)));
	if(drunk == 0) sEventMgr.RemoveEvents(this, EVENT_PLAYER_REDUCEDRUNK);
}

void Player::LoadTaxiMask(const char* data)
{
	vector<string> tokens = StrSplit(data, " ");

	int index;
	vector<string>::iterator iter;

	for (iter = tokens.begin(), index = 0;
		(index < 8) && (iter != tokens.end()); ++iter, ++index)
	{
		m_taximask[index] = atol((*iter).c_str());
	}
}

bool Player::HasQuestForItem(uint32 itemid)
{
	Quest *qst;
	for( uint32 i = 0; i < 25; ++i )
	{
		if( m_questlog[i] != NULL )
		{
			qst = m_questlog[i]->GetQuest();

			// Check the item_quest_association table for an entry related to this item
			QuestAssociationList *tempList = QuestMgr::getSingleton().GetQuestAssociationListForItemId( itemid );
			if( tempList != NULL )
			{
				QuestAssociationList::iterator it;
				for (it = tempList->begin(); it != tempList->end(); ++it)
				{
					if ( ((*it)->qst == qst) && (GetItemInterface()->GetItemCount( itemid ) < (*it)->item_count) )
					{
						return true;
					} // end if
				} // end for
			} // end if
			
			// No item_quest association found, check the quest requirements
			if( !qst->count_required_item )
				continue;

			for( uint32 j = 0; j < 6; ++j )
				if( qst->required_item[j] == itemid && ( GetItemInterface()->GetItemCount( itemid ) < qst->required_itemcount[j] ) )
					return true;
		}
	}
	return false;
}

/*Loot type MUST be
1-corpse, go
2-skinning/herbalism/minning
3-Fishing
*/
void Player::SendLoot(uint64 guid,uint8 loot_type)
{	
	Group * m_Group = m_playerInfo->m_Group;
	if(!IsInWorld()) return;
	Loot * pLoot = NULL;
	uint32 guidtype = GET_TYPE_FROM_GUID(guid);
	int8 loot_method = -1;

	if(guidtype == HIGHGUID_TYPE_UNIT)
	{
		Creature* pCreature = GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(!pCreature)return;
		pLoot=&pCreature->loot;
		m_currentLoot = pCreature->GetGUID();
		loot_method = pCreature->m_lootMethod;
	}else if(guidtype == HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject* pGO = GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(!pGO)return;
		//pGO->SetUInt32Value(GAMEOBJECT_STATE,0);
		pGO->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
		pLoot=&pGO->loot;
		m_currentLoot = pGO->GetGUID();
	}
	else if((guidtype == HIGHGUID_TYPE_PLAYER) )
	{
		Player *p=GetMapMgr()->GetPlayer((uint32)guid);
		if(!p)return;
		pLoot=&p->loot;
		m_currentLoot = p->GetGUID();
	}
	else if( (guidtype == HIGHGUID_TYPE_CORPSE))
	{
		Corpse *pCorpse = objmgr.GetCorpse((uint32)guid);
		if(!pCorpse)return;
		pLoot=&pCorpse->loot;
		m_currentLoot = pCorpse->GetGUID();
	}
	else if( (guidtype == HIGHGUID_TYPE_ITEM) )
	{
		Item *pItem = GetItemInterface()->GetItemByGUID(guid);
		if(!pItem)
			return;
		pLoot = pItem->loot;
		m_currentLoot = pItem->GetGUID();
	}

	if(!pLoot)
	{
		// something whack happened.. damn cheaters..
		return;
	}

	if( loot_method < 0 )
	{
		// not set
		if( m_Group != NULL )
			loot_method = m_Group->GetMethod();
		else
			loot_method = PARTY_LOOT_FFA;
	}

	// add to looter set
	pLoot->looters.insert(GetLowGUID());
		
	WorldPacket data, data2(32);
	data.SetOpcode (SMSG_LOOT_RESPONSE);
   
   
	m_lootGuid = guid;

			
	data << uint64(guid);
	data << uint8(loot_type);//loot_type;
	data << uint32(pLoot->gold);
	data << uint8(0);//loot size reserve

	  
	std::vector<__LootItem>::iterator iter=pLoot->items.begin();
	uint32 count=0;
	uint8 slottype = 0;
   
	for(uint32 x=0;iter!=pLoot->items.end();iter++,x++)
	{ 
		if (iter->iItemsCount == 0)
			continue;

		LooterSet::iterator itr = iter->has_looted.find(GetLowGUID());
		if (iter->has_looted.end() != itr)
			continue;

		ItemPrototype* itemProto =iter->item.itemproto;
		if (!itemProto)		   
			continue;
        //quest items check. type 4/5
        //quest items that dont start quests.
        if((itemProto->Bonding == ITEM_BIND_QUEST) && !(itemProto->QuestId) && !HasQuestForItem(iter->item.itemproto->ItemId))
            continue;
        if((itemProto->Bonding == ITEM_BIND_QUEST2) && !(itemProto->QuestId) && !HasQuestForItem(iter->item.itemproto->ItemId))
            continue;

        //quest items that start quests need special check to avoid drops all the time.
        if((itemProto->Bonding == ITEM_BIND_QUEST) && (itemProto->QuestId) && GetQuestLogForEntry(itemProto->QuestId))
            continue;
        if((itemProto->Bonding == ITEM_BIND_QUEST2) && (itemProto->QuestId) && GetQuestLogForEntry(itemProto->QuestId))
            continue;

        if((itemProto->Bonding == ITEM_BIND_QUEST) && (itemProto->QuestId) && HasFinishedQuest(itemProto->QuestId))
            continue;
        if((itemProto->Bonding == ITEM_BIND_QUEST2) && (itemProto->QuestId) && HasFinishedQuest(itemProto->QuestId))
            continue;

        //check for starting item quests that need questlines.
        if((itemProto->QuestId && itemProto->Bonding != ITEM_BIND_QUEST && itemProto->Bonding != ITEM_BIND_QUEST2))
        {
            bool HasRequiredQuests = true;
            Quest * pQuest = QuestStorage.LookupEntry(itemProto->QuestId);
            if(pQuest)
            {
               //check if its a questline.
                for(uint32 i = 0; i < pQuest->count_requiredquests; i++)
                {
                    if(pQuest->required_quests[i])
                    {
                        if(!HasFinishedQuest(pQuest->required_quests[i]) || GetQuestLogForEntry(pQuest->required_quests[i]))
                        {
                            HasRequiredQuests = false;
                            break;
                        }
                    }
                }
                if(!HasRequiredQuests)
                    continue;
            }
        } 


		slottype = 0;
		if(m_Group != NULL && loot_type < 2)
		{
			switch(loot_method)
			{
			case PARTY_LOOT_MASTER:
				slottype = 2;
				break;
			case PARTY_LOOT_GROUP:
			case PARTY_LOOT_RR:
			case PARTY_LOOT_NBG:
				slottype = 1;
				break;
			default:
				slottype = 0;
				break;
			}
			// only quality items are distributed
			if(itemProto->Quality < m_Group->GetThreshold())
			{
				slottype = 0;
			}

			/* if all people passed anyone can loot it? :P */
			if(iter->passed)
				slottype = 0;					// All players passed on the loot

			//if it is ffa loot and not an masterlooter
			if(iter->ffa_loot && slottype != 2)
				slottype = 0;
		}

		data << uint8(x); 
		data << uint32(itemProto->ItemId);
		data << uint32(iter->iItemsCount);//nr of items of this type
		data << uint32(iter->item.displayid); 
		//data << uint32(iter->iRandomSuffix ? iter->iRandomSuffix->id : 0);
		//data << uint32(iter->iRandomProperty ? iter->iRandomProperty->ID : 0);
		if(iter->iRandomSuffix)
		{
			data << Item::GenerateRandomSuffixFactor(itemProto);
			data << uint32(-int32(iter->iRandomSuffix->id));
		}
		else if(iter->iRandomProperty)
		{
			data << uint32(0);
			data << uint32(iter->iRandomProperty->ID);
		}
		else
		{
			data << uint32(0);
			data << uint32(0);
		}

		data << slottype;   // "still being rolled for" flag
		
		if(slottype == 1)
		{
			if(iter->roll == NULL && !iter->passed)
			{
				int32 ipid = 0;
				uint32 factor=0;
				if(iter->iRandomProperty)
					ipid=iter->iRandomProperty->ID;
				else if(iter->iRandomSuffix)
				{
					ipid = -int32(iter->iRandomSuffix->id);
					factor=Item::GenerateRandomSuffixFactor(iter->item.itemproto);
				}

				if(iter->item.itemproto)
				{
					iter->roll = new LootRoll(60000, (m_Group != NULL ? m_Group->MemberCount() : 1),  guid, x, iter->item.itemproto->ItemId, factor, uint32(ipid), GetMapMgr());
					
					data2.Initialize(SMSG_LOOT_START_ROLL);
					data2 << guid;
					data2 << x;
					data2 << uint32(iter->item.itemproto->ItemId);
					data2 << uint32(factor);
					if(iter->iRandomProperty)
						data2 << uint32(iter->iRandomProperty->ID);
					else if(iter->iRandomSuffix)
						data2 << uint32(ipid);
					else
						data2 << uint32(0);

					data2 << uint32(iter->iItemsCount);
					data2 << uint32(60000); // countdown
					data2 << uint8(7);
				}

				Group * pGroup = m_playerInfo->m_Group;
				if(pGroup)
				{
					pGroup->Lock();
					for(uint32 i = 0; i < pGroup->GetSubGroupCount(); ++i)
					{
						for(GroupMembersSet::iterator itr = pGroup->GetSubGroup(i)->GetGroupMembersBegin(); itr != pGroup->GetSubGroup(i)->GetGroupMembersEnd(); ++itr)
						{
							if((*itr)->m_loggedInPlayer && (*itr)->m_loggedInPlayer->GetItemInterface()->CanReceiveItem(itemProto, iter->iItemsCount) == 0)
							{
								if( (*itr)->m_loggedInPlayer->m_passOnLoot )
									iter->roll->PlayerRolled( (*itr)->m_loggedInPlayer, 3 );		// passed
								else
									(*itr)->m_loggedInPlayer->GetSession()->SendPacket(&data2);
							}
						}
					}
					pGroup->Unlock();
				}
				else
				{
					GetSession()->SendPacket(&data2);
				}
			}			
		}
		count++;
	}
	data.wpos (13);
	data << (uint8)count;

	GetSession ()->SendPacket(&data);
	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOOTING);
}

void Player::SendDualTalentConfirm()
{
	uint32 talentCost = sWorld.dualTalentTrainCost;
	// Here we should send a confirmation box, but there is no documented opcode for it
	if( GetUInt32Value(PLAYER_FIELD_COINAGE) >= talentCost )
	{
		// Cast the learning spell
		CastSpell( this, 63624, true);
		ModUnsigned32Value(PLAYER_FIELD_COINAGE, -(int32)talentCost);
	}
	else
	{
		// Not enough money!
		sChatHandler.SystemMessage(m_session, "You must have at least %u copper to use this function.", talentCost);
	}
	/*WorldPacket data();
	data << GetGUID();
	data << sWorld.dualTalentTrainCost;
	GetSession()->SendPacket( &data );*/
}

void Player::EventAllowTiggerPort(bool enable)
{
	m_AllowAreaTriggerPort = enable;
}

uint32 Player::CalcTalentResetCost(uint32 resetnum)
{
	
	if(resetnum ==0 ) 
		return  10000;
	else
	{
		if(resetnum>10)
		return  500000;
		else return resetnum*50000;
	}
}

void Player::SendTalentResetConfirm()
{
	WorldPacket data(MSG_TALENT_WIPE_CONFIRM, 12);
	data << GetGUID();
	data << CalcTalentResetCost(GetTalentResetTimes());
	GetSession()->SendPacket(&data);
}
void Player::SendPetUntrainConfirm()
{
	Pet* pPet = GetSummon();
	if( pPet == NULL )
		return;
	WorldPacket data( SMSG_PET_UNLEARN_CONFIRM, 12 );
	data << pPet->GetGUID();
	data << pPet->GetUntrainCost();
	GetSession()->SendPacket( &data );
}

int32 Player::CanShootRangedWeapon( uint32 spellid, Unit* target, bool autoshot )
{
	SpellEntry* spellinfo = dbcSpell.LookupEntry( spellid );

	if(spellinfo == NULL)
		return -1;

	// Check if Morphed
	if(polySpell > 0)
		return SPELL_FAILED_NOT_SHAPESHIFT;

	// Check ammo
	Item* itm = GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
	if(itm == NULL)
		return SPELL_FAILED_NO_AMMO;

	// Check ammo level
	ItemPrototype * iprot=ItemPrototypeStorage.LookupEntry(GetUInt32Value(PLAYER_AMMO_ID));
	if (iprot == NULL)
		return SPELL_FAILED_NO_AMMO;
	else if(getLevel() < iprot->RequiredLevel && itm->GetProto()->SubClass != 19) //wands dont require ammo, lol
		return SPELL_FAILED_LOWLEVEL;

	// Player has clicked off target. Fail spell.
	if(m_curSelection != m_AutoShotTarget)
		return SPELL_FAILED_INTERRUPTED;

	// Check if target is allready dead
	if(target->isDead())
		return SPELL_FAILED_TARGETS_DEAD;

	// Check if we aren't casting another spell allready
	if(GetCurrentSpell())
		return -1;
   
	// Supalosa - The hunter ability Auto Shot is using Shoot range, which is 5 yards shorter.
	// So we'll use 114, which is the correct 35 yard range used by the other Hunter abilities (arcane shot, concussive shot...)
	uint8 fail = 0;
	uint32 rIndex = autoshot ? 114 : spellinfo->rangeIndex;
	SpellRange* range = dbcSpellRange.LookupEntry(rIndex);
	if(!range) return(0);
	float minr = GetMinRange(range);
	float dist = CalcDistance(this, target) - target->GetSize() - GetSize();
	float maxr = GetMaxRange(range);
	float boundingradius = GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS);
	if(target->IsUnit())  boundingradius +=target->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS);


	if( spellinfo->SpellGroupType[0] || spellinfo->SpellGroupType[1] || spellinfo->SpellGroupType[2])
	{
		SM_FFValue( this->SM_FRange, &maxr, spellinfo );
		SM_PFValue( this->SM_PRange, &maxr, spellinfo );
	}
	
	if(dist > maxr + boundingradius)
		fail = SPELL_FAILED_OUT_OF_RANGE;

	if (minr > 0 && dist < minr - boundingradius)
		fail = SPELL_FAILED_TOO_CLOSE;
		
	// Check if in line of sight (need collision detection).
#ifdef COLLISION
    const LocationVector &loc1 = this->GetPositionNC();
    const LocationVector &loc2 = target->GetPositionNC();
	            
    float SourceTaille = this->GetSize();
    float TargetTaille = target->GetSize();
	if (GetMapId() == target->GetMapId() && !CollideInterface.CheckLOS(GetMapId(),loc1.x,loc1.y,loc1.z+SourceTaille,
		                                                                          loc2.x,loc2.y,loc2.z+TargetTaille))
		return SPELL_FAILED_LINE_OF_SIGHT;
#endif

	if(fail > 0)
	{
		packetSMSG_CASTRESULT cr;
		cr.SpellId = autoshot ? 75 : spellid;
		cr.ErrorMessage = fail;
		cr.MultiCast = 0;
		m_session->OutPacket( SMSG_CAST_FAILED, sizeof(packetSMSG_CASTRESULT), &cr );
		if( fail != SPELL_FAILED_OUT_OF_RANGE )
		{
			uint32 spellid2 = autoshot ? 75 : spellid;
			m_session->OutPacket( SMSG_CANCEL_AUTO_REPEAT, 4, &spellid2 );
		}
		return fail;
	}

	return 0;
}

void Player::EventRepeatSpell()
{
	if( !m_curSelection || !IsInWorld() )
		return;
	
	Unit* target = GetMapMgr()->GetUnit( m_curSelection );
	if( target == NULL )
	{
		m_AutoShotAttackTimer = 500; //avoid flooding client with error mesages
		m_onAutoShot = false;
		//sLog.outDebug( "Can't cast Autoshot: Target changed! (Timer: %u)" , m_AutoShotAttackTimer );
		return;
	}

	m_AutoShotDuration = m_uint32Values[UNIT_FIELD_RANGEDATTACKTIME];

	if( m_isMoving )
	{
		//sLog.outDebug( "HUNTER AUTOSHOT 2) %i, %i", m_AutoShotAttackTimer, m_AutoShotDuration );
		//m_AutoShotAttackTimer = m_AutoShotDuration;//avoid flooding client with error mesages
		//sLog.outDebug( "Can't cast Autoshot: You're moving! (Timer: %u)" , m_AutoShotAttackTimer );
		m_AutoShotAttackTimer = 500; // shoot when we can
		return;
	}

	int32 f = this->CanShootRangedWeapon( m_AutoShotSpell->Id, target, true );

	if( f != 0 )
	{
		if( f != SPELL_FAILED_OUT_OF_RANGE && GetCurrentSpell() == NULL ) //autoshot won't stop if your casting a spell (steady shot f.e)
		{
			m_AutoShotAttackTimer = 0; 
			m_onAutoShot=false;
		}
		else
		{
			m_AutoShotAttackTimer = m_AutoShotDuration; //avoid flooding client with error mesages
		}
		return;
	}
	else
	{		
		m_AutoShotAttackTimer = m_AutoShotDuration;
	
		Spell* sp = new Spell( this, m_AutoShotSpell, true, NULL );
		SpellCastTargets tgt;
		tgt.m_target = GetMapMgr()->GetUnit(GetSelection());
		tgt.m_targetMask = TARGET_FLAG_UNIT;
		sp->prepare( &tgt );
	}
}

void Player::removeSpellByHashName(uint32 hash)
{
	SpellSet::iterator it,iter;
	
	for(iter= mSpells.begin();iter != mSpells.end();)
	{
		it = iter++;
		uint32 SpellID = *it;
		SpellEntry *e = dbcSpell.LookupEntry(SpellID);
		if(e->NameHash == hash)
		{
			if(info->spell_list.find(e->Id) != info->spell_list.end())
				continue;

			RemoveAura(SpellID,GetGUID());
#ifdef USING_BIG_ENDIAN
			uint32 swapped = swap32(SpellID);
			m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &swapped);
#else
			m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &SpellID);		
#endif
			mSpells.erase(it);
		}
	}

	for(iter= mDeletedSpells.begin();iter != mDeletedSpells.end();)
	{
		it = iter++;
		uint32 SpellID = *it;
		SpellEntry *e = dbcSpell.LookupEntry(SpellID);
		if(e->NameHash == hash)
		{
			if(info->spell_list.find(e->Id) != info->spell_list.end())
				continue;

			RemoveAura(SpellID,GetGUID());
#ifdef USING_BIG_ENDIAN
			uint32 swapped = swap32(SpellID);
			m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &swapped);
#else
			m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &SpellID);		
#endif
			mDeletedSpells.erase(it);
		}
	}
}

bool Player::removeSpell(uint32 SpellID, bool MoveToDeleted, bool SupercededSpell, uint32 SupercededSpellID)
{
	SpellSet::iterator iter = mSpells.find(SpellID);
	if(iter != mSpells.end())
	{
		mSpells.erase(iter);
		RemoveAura(SpellID,GetGUID());
		
		SpellEntry * spell = dbcSpell.LookupEntry(SpellID);

		for (uint32 i=0; i<3; ++i)
		{
			if (spell->Effect[i] == SPELL_EFFECT_LEARN_SPELL)
			{
				SpellEntry* to_learn = dbcSpell.LookupEntry(spell->EffectTriggerSpell[i]);
				if (to_learn != NULL)
					removeSpell(to_learn->Id, MoveToDeleted, SupercededSpell, SupercededSpellID);
			}
		}		
	}
	else
	{
		return false;
	}

	if(MoveToDeleted)
		mDeletedSpells.insert(SpellID);

	if(!IsInWorld())
		return true;

	if(SupercededSpell)
	{
		WorldPacket data(SMSG_SUPERCEDED_SPELL, 8);
		data << SpellID << SupercededSpellID;
		m_session->SendPacket(&data);
	}
	else
	{
#ifdef USING_BIG_ENDIAN
		uint32 swapped = swap32(SpellID);
		m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &swapped);
#else
		m_session->OutPacket(SMSG_REMOVED_SPELL, 4, &SpellID);		
#endif
	}

	return true;
}

void Player::EventActivateGameObject(GameObject* obj)
{
	obj->BuildFieldUpdatePacket(this, GAMEOBJECT_DYNAMIC, 1);
}

void Player::EventDeActivateGameObject(GameObject* obj)
{
	obj->BuildFieldUpdatePacket(this, GAMEOBJECT_DYNAMIC, 0);
}

void Player::EventTimedQuestExpire(Quest *qst, QuestLogEntry *qle, uint32 log_slot)
{
	if(qle) // Si qle est NULL, la quete a été validée et n'existe plus dans le LogEntry
	{
	 WorldPacket fail;
	 sQuestMgr.BuildQuestFailed(&fail, qst->id);
	 GetSession()->SendPacket(&fail);
	 CALL_QUESTSCRIPT_EVENT(qle, OnQuestCancel)(this);
	 qle->Finish();
	}
}

void Player::SendInitialLogonPackets()
{
	// Initial Packets... they seem to be re-sent on port.
	m_session->OutPacket(SMSG_QUEST_FORCE_REMOVE, 4, &m_timeLogoff);

    WorldPacket data(SMSG_BINDPOINTUPDATE, 32);
    data << m_bind_pos_x;
    data << m_bind_pos_y;
    data << m_bind_pos_z;
    data << m_bind_mapid;
    data << m_bind_zoneid;
	
    m_session->SendPacket( &data );

	//Proficiencies
    //SendSetProficiency(4,armor_proficiency);
    //SendSetProficiency(2,weapon_proficiency);
	packetSMSG_SET_PROFICICENCY pr;
	pr.ItemClass = 4;
	pr.Profinciency = armor_proficiency;
	m_session->OutPacket( SMSG_SET_PROFICIENCY, sizeof(packetSMSG_SET_PROFICICENCY), &pr );
	pr.ItemClass = 2;
	pr.Profinciency = weapon_proficiency;
	m_session->OutPacket( SMSG_SET_PROFICIENCY, sizeof(packetSMSG_SET_PROFICICENCY), &pr );
  
	//Tutorial Flags
	data.Initialize( SMSG_TUTORIAL_FLAGS );
	for (int i = 0; i < 8; i++)
		data << uint32( m_Tutorials[i] );
	m_session->SendPacket(&data);

	//Initial Spells
	smsg_InitialSpells();
	
	data.Initialize(SMSG_SEND_UNLEARN_SPELLS);
	data << uint32(0); // count, for(count) uint32;
	GetSession()->SendPacket( &data );

	//Initial Actions
	SendInitialActions();

	//Factions
	smsg_InitialFactions();


    /* Some minor documentation about the time field
    // MOVE THIS DOCUMENATION TO THE WIKI
    
    minute's = 0x0000003F                  00000000000000000000000000111111
    hour's   = 0x000007C0                  00000000000000000000011111000000
    weekdays = 0x00003800                  00000000000000000011100000000000
    days     = 0x000FC000                  00000000000011111100000000000000
    months   = 0x00F00000                  00000000111100000000000000000000
    years    = 0x1F000000                  00011111000000000000000000000000
    unk	     = 0xE0000000                  11100000000000000000000000000000
    */

	data.Initialize(SMSG_LOGIN_SETTIMESPEED);
	
	time_t minutes = sWorld.GetGameTime( ) / 60;
	time_t hours = minutes / 60; minutes %= 60;
    time_t gameTime = 0;

	time_t basetime = UNIXTIME;
	uint32 DayOfTheWeek;
	if(localtime(&basetime)->tm_wday == 0)
		DayOfTheWeek = 6;					//	It's Sunday
	else
		DayOfTheWeek = localtime(&basetime)->tm_wday - 1;		//	0b111 = (any) day, 0 = Monday ect)
	uint32 DayOfTheMonth = localtime(&basetime)->tm_mday - 1;	//	Day - 1 (0 is actual 1) its now the 20e here.
	uint32 CurrentMonth = localtime(&basetime)->tm_mon;			//	Month - 1 (0 is actual 1) same as above.
	uint32 CurrentYear = localtime(&basetime)->tm_year - 100;	//	2000 + this number results in a correct value for this crap.
 
	#define MINUTE_BITMASK      0x0000003F
    #define HOUR_BITMASK        0x000007C0
    #define WEEKDAY_BITMASK     0x00003800
    #define DAY_BITMASK         0x000FC000
    #define MONTH_BITMASK       0x00F00000
    #define YEAR_BITMASK        0x1F000000
    #define UNK_BITMASK         0xE0000000

    #define MINUTE_SHIFTMASK    0
    #define HOUR_SHIFTMASK      6
    #define WEEKDAY_SHIFTMASK   11
    #define DAY_SHIFTMASK       14
    #define MONTH_SHIFTMASK     20
    #define YEAR_SHIFTMASK      24
    #define UNK_SHIFTMASK       29

    gameTime = ((minutes << MINUTE_SHIFTMASK) & MINUTE_BITMASK);
    gameTime|= ((hours << HOUR_SHIFTMASK) & HOUR_BITMASK);
    gameTime|= ((DayOfTheWeek << WEEKDAY_SHIFTMASK) & WEEKDAY_BITMASK);
    gameTime|= ((DayOfTheMonth << DAY_SHIFTMASK) & DAY_BITMASK);
    gameTime|= ((CurrentMonth << MONTH_SHIFTMASK) & MONTH_BITMASK);
    gameTime|= ((CurrentYear << YEAR_SHIFTMASK) & YEAR_BITMASK);

    data << (uint32)gameTime;
	data << (float)0.0166666669777748f;  // Normal Game Speed
	data << uint32( 0 ); // 3.1.2
	
	m_session->SendPacket( &data );

	Log.Debug("WORLD","Sent initial logon packets for %s.", GetName());
	//sLog.outDetail("WORLD:SMSG_BINDPOINTUPDATE SMSG_LOGIN_SETTIMESPEED SMSG_SET_PROFICIENCY SMSG_TUTORIAL_FLAGS");
}

void Player::Reset_Spells()
{
	PlayerCreateInfo *info = objmgr.GetPlayerCreateInfo(getRace(), getClass());
    //ASSERT(info);
	if(!info)
	{
		Log.Error("Player::Reset_Spells()","Les tables Createplayer_info pour Race %u et Classe %u n'existe pas! (%s)",this->getRace(),this->getClass(),this->GetName());
		return;
	}


	std::list<uint32> spelllist;
	
	for(SpellSet::iterator itr = mSpells.begin(); itr!=mSpells.end(); itr++)
	{
		spelllist.push_back((*itr));
	}

	for(std::list<uint32>::iterator itr = spelllist.begin(); itr!=spelllist.end(); itr++)
	{
		removeSpell((*itr), false, false, 0);
	}

	for(std::set<uint32>::iterator sp = info->spell_list.begin();sp!=info->spell_list.end();sp++)
	{
		if(*sp)
		{
			addSpell(*sp);
		}
	}
}

void Player::ResetTitansGrip()
{
	if(titanGrip || !GetItemInterface())
		return;

	Item *mainhand = GetItemInterface()->GetInventoryItem(INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_MAINHAND);
	Item *offhand = new (GetItemInterface()->GetInventoryItem(INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_OFFHAND)) Item();
	if(offhand && (offhand->GetProto()->InventoryType == INVTYPE_2HWEAPON ||
		mainhand && mainhand->GetProto()->InventoryType == INVTYPE_2HWEAPON))
	{
		// we need to de-equip this
		offhand = GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_OFFHAND, false);
		if( offhand == NULL )
		{
			delete offhand;
			offhand = NULL;
			return;     // should never happen
		}
		SlotResult result = GetItemInterface()->FindFreeInventorySlot(offhand->GetProto());
		if( !result.Result )
		{
			// no free slots for this item. try to send it by mail
			offhand->RemoveFromWorld();
			offhand->SetOwner( NULL );
			offhand->SaveToDB( INVENTORY_SLOT_NOT_SET, 0, true, NULL );
			//sMailSystem.DeliverMessage(GetGUID(),"Your offhand item", "", 0, 0, offhand->GetUInt32Value(OBJECT_FIELD_GUID), 1, true);
			delete offhand;
			offhand = NULL;
		}
		else if( !GetItemInterface()->SafeAddItem(offhand, result.ContainerSlot, result.Slot) )
			if( !GetItemInterface()->AddItemToFreeSlot(offhand) )   // shouldn't happen either.
			{
				delete offhand;
				offhand = NULL;
			}
	}
	//Note Randdrick : suppression du pointeur offhand déclaré au début par un new.
	if(offhand)
	{
		delete offhand;
		offhand = NULL;
	}
}

void Player::Reset_Talents()
{
	std::map<uint32, uint8> *talents = &m_specs[m_talentActiveSpec].talents;
	std::map<uint32, uint8>::iterator itr;
	for(itr = talents->begin(); itr != talents->end(); itr++)
	{
		TalentEntry *te = dbcTalent.LookupEntryForced(itr->first);
		if(!te)
			continue;
		RemoveTalent(te->RankID[itr->second]);
	}
	talents->clear();

	// The dual wield skill for shamans can only be added by talents.
	// so when reset, the dual wield skill should be removed too.
	// (see also void Spell::SpellEffectDualWield)
	if( getClass()==SHAMAN && _HasSkillLine( SKILL_DUAL_WIELD ) )
		_RemoveSkillLine( SKILL_DUAL_WIELD );

	SetUInt32Value(PLAYER_CHARACTER_POINTS1, GetMaxTalentPoints()); 

	if( getClass() == WARRIOR )
	{	
		titanGrip = false;
		ResetTitansGrip();
	}
	smsg_TalentsInfo(false);
	
	/*unsigned int numRows = dbcTalent.GetNumRows();
	for (unsigned int i = 0; i < numRows; i++)		  // Loop through all talents.
	{
		TalentEntry *tmpTalent = dbcTalent.LookupRow(i);
		if(!tmpTalent)
			continue; //should not ocur
		//this is a normal talent (i hope )
		for (int j = 0; j < 5; j++)
		{
			if (tmpTalent->RankID[j] != 0)
			{
				SpellEntry *spellInfo;
				spellInfo = dbcSpell.LookupEntry( tmpTalent->RankID[j] );
				if(spellInfo)
				{
					for(int k=0;k<3;k++)
						if(spellInfo->Effect[k] == SPELL_EFFECT_LEARN_SPELL)
						{
							//removeSpell(spellInfo->EffectTriggerSpell[k], false, 0, 0);
							//remove higher ranks of this spell too (like earth shield lvl 1 is talent and the rest is thought from trainer) 
							SpellEntry *spellInfo2;
							spellInfo2 = dbcSpell.LookupEntry( spellInfo->EffectTriggerSpell[k] );
							if(spellInfo2)
								removeSpellByHashName(spellInfo2->NameHash);
						}
					//remove them all in 1 shot
					removeSpellByHashName(spellInfo->NameHash);
				}
			}
			else
				break;
		}
	}
	uint32 l=getLevel();
	if(l>9)
	{
		SetUInt32Value(PLAYER_CHARACTER_POINTS1, l - 9); 
	}
	else
	{
		SetUInt32Value(PLAYER_CHARACTER_POINTS1, 0); 
	}

	if( getClass() == WARRIOR )
	{	
		titanGrip = false;
		ResetTitansGrip();
	}

	CharacterDatabase.Execute("UPDATE `characters` SET `pendingTalentReset` = 0 WHERE `guid` = %u", GetLowGUID());*/
}

uint16 Player::GetMaxTalentPoints()
{
	// see if we have a custom value
	if(m_maxTalentPoints > 0)
		return m_maxTalentPoints;

	// otherwise use blizzlike value
	uint32 l = getLevel();
	if(l > 9)
	{
		return l - 9; 
	}
	else
	{
		return 0; 
	}
}

void Player::ApplySpec(uint8 spec, bool init)
{
	if(spec > m_talentSpecsCount || spec > MAX_SPEC_COUNT)
		return;
	std::map<uint32, uint8> *talents;
	std::map<uint32, uint8>::iterator itr;

	if(!init)	// unapply old spec
	{
		talents = &m_specs[m_talentActiveSpec].talents;
		for(itr = talents->begin(); itr != talents->end(); itr++)
		{
			TalentEntry * talentInfo = dbcTalent.LookupEntryForced(itr->first);
			if(!talentInfo || itr->second > 4)
				continue;
			RemoveTalent(talentInfo->RankID[itr->second]);
		}
		if( getClass() == WARRIOR )
		{	
			titanGrip = false;
			ResetTitansGrip();
		}

		//Dismiss any pets
		if(GetSummon())
		{
			if(GetSummon()->GetUInt32Value(UNIT_CREATED_BY_SPELL) > 0)
				GetSummon()->Dismiss(false);				// warlock summon -> dismiss
			else
				GetSummon()->Remove(false, true, true);	// hunter pet -> just remove for later re-call
		}
	}
	
	// apply new spec
	talents = &m_specs[spec].talents;
	uint32 spentPoints = 0;
	for(itr = talents->begin(); itr != talents->end(); itr++)
	{
		TalentEntry * talentInfo = dbcTalent.LookupEntryForced(itr->first);
		if(!talentInfo || itr->second > 4)
			continue;
		ApplyTalent(talentInfo->RankID[itr->second]);
		spentPoints += itr->second + 1;
	}
	m_talentActiveSpec = spec;
	// update available Talent Points
	uint32 maxTalentPoints = GetMaxTalentPoints();
	uint32 newTalentPoints;
	if(spentPoints >= maxTalentPoints)
		newTalentPoints = 0;	// just in case
	else
		newTalentPoints = maxTalentPoints - spentPoints;
	SetUInt32Value(PLAYER_CHARACTER_POINTS1, newTalentPoints);

	// Apply glyphs
	for(uint32 i = 0; i < GLYPHS_COUNT; i++)
	{
		UnapplyGlyph(i);
		SetGlyph(i, m_specs[m_talentActiveSpec].glyphs[i]);
	}
	smsg_TalentsInfo(false);
}

void Player::ApplyTalent(uint32 spellid)
{
	SpellEntry *spellInfo = dbcSpell.LookupEntryForced( spellid );
	if(!spellInfo)
		return;	// not found

	if(!(spellInfo->attributes & ATTRIBUTES_PASSIVE))
	{
		addSpell(spellid);	// in this case we need to learn the spell itself
	}

	if( (spellInfo->attributes & ATTRIBUTES_PASSIVE || (spellInfo->Effect[0] == SPELL_EFFECT_LEARN_SPELL ||
		spellInfo->Effect[1] == SPELL_EFFECT_LEARN_SPELL ||
		spellInfo->Effect[2] == SPELL_EFFECT_LEARN_SPELL) 
		&& ( (spellInfo->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET) == 0 || ( (spellInfo->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET) && GetSummon() ) ) )
		)
	{
		if( spellInfo->shapeshiftMask && !( (uint32)1 << (GetShapeShift()-1) & spellInfo->shapeshiftMask ) )
		{
			// do nothing
		}
		else
		{
			Spell *sp = new Spell(this, spellInfo, true, NULL);
			SpellCastTargets tgt;
			tgt.m_target = (Object *)this; // tgt.m_unitTarget=GetGUID();
			sp->prepare(&tgt);
		}
	}
}

void Player::RemoveTalent(uint32 spellid)
{
	SpellEntry * sp = dbcSpell.LookupEntryForced(spellid);
	if(!sp)
		return; // not found

	for(uint32 k = 0; k < 3; ++k)
	{
		if(sp->Effect[k] == SPELL_EFFECT_LEARN_SPELL)
		{
			SpellEntry * sp2 = dbcSpell.LookupEntryForced(sp->EffectTriggerSpell[k]);
			if(!sp2) continue;
			removeSpellByHashName(sp2->NameHash);
		}
	}
	if(!(sp->attributes & ATTRIBUTES_PASSIVE))
	{	// in this case we need to remove the spells we learned from this talent
		// TODO we need to store them somewhere so that player doesn't have to relearn all ranks again
		removeSpellByHashName(sp->NameHash);
	}

	for(uint32 x=0;x < MAX_AURAS + MAX_PASSIVE_AURAS; x++)
	{
		if(m_auras[x] && m_auras[x]->GetSpellProto()->NameHash == sp->NameHash)
		{
			m_auras[x]->Remove();
		}
	}
}

void Player::LearnTalent(uint32 talent_id, uint32 requested_rank)
{
	unsigned int i;
	if (requested_rank > 4)
		return;

	TalentEntry * talentInfo = dbcTalent.LookupEntryForced(talent_id);
	if(!talentInfo)return;

	uint32 CurTalentPoints = GetUInt32Value(PLAYER_CHARACTER_POINTS1);
	std::map<uint32, uint8> *talents = &m_specs[m_talentActiveSpec].talents;
	uint8 currentRank = 0;
	std::map<uint32, uint8>::iterator itr = talents->find(talent_id);
	if(itr != talents->end())
		currentRank = itr->second + 1;

	if(currentRank >= requested_rank + 1)
		return; // player already knows requested or higher rank for this talent

	uint32 RequiredTalentPoints = requested_rank + 1 - currentRank;
	if(CurTalentPoints < RequiredTalentPoints )
		return; // player doesn't have enough points to get this rank for this talent

	// Check if it requires another talent
	if (talentInfo->DependsOn > 0)
	{
		TalentEntry *depTalentInfo = NULL;
		depTalentInfo = dbcTalent.LookupEntryForced(talentInfo->DependsOn);
		if (depTalentInfo)
		{
			itr = talents->find(talentInfo->DependsOn);
			if(itr == talents->end())
				return;	// player doesn't have the talent this one depends on
			if(talentInfo->DependsOnRank > itr->second)
				return;	// player doesn't have the talent rank this one depends on
		}
	}

	// Check that the requested talent belongs to a tree from player's class
	uint32 tTree = talentInfo->TalentTree;
	uint32 cl = getClass();

	for(i = 0; i < 3; ++i)
		if(tTree == TalentTreesPerClass[cl][i])
			break;

	if(i == 3)
	{
		// cheater!
		GetSession()->Disconnect();
		return;
	}

	// Find out how many points we have in this field
	uint32 spentPoints = 0;
	if (talentInfo->Row > 0)
	{
		for(itr = talents->begin(); itr != talents->end(); itr++)
		{
			TalentEntry *tmpTalent = dbcTalent.LookupEntryForced(itr->first);
			if (tmpTalent->TalentTree == tTree)
			{
				spentPoints += itr->second + 1;
			}
		}
	}

	uint32 spellid = talentInfo->RankID[requested_rank];
	if( spellid == 0 )
	{
		sLog.outString("Talent: %u Rank: %u = 0", talent_id, requested_rank);
		return;
	}

	if(spentPoints < (talentInfo->Row * 5))			 // Min points spent
	{
		return;
	}

	(*talents)[talent_id] = requested_rank;
	SetUInt32Value(PLAYER_CHARACTER_POINTS1, CurTalentPoints - RequiredTalentPoints);
	// More cheat death hackage! :)
	if(spellid == 31330)
		m_cheatDeathRank = 3;
	else if(spellid == 31329)
		m_cheatDeathRank = 2;
	else if(spellid == 31328)
		m_cheatDeathRank = 1;	 

	if(requested_rank > 0 )	// remove old rank aura
	{
		uint32 respellid = talentInfo->RankID[currentRank - 1];
		if(respellid)
		{
			RemoveAura(respellid);
		}
	}

	ApplyTalent(spellid);
}

void Player::Reset_ToLevel1()
{
	RemoveAllAuras();

	// clear aura fields
	// Aura: update
	if(this->getClass() == DEATHKNIGHT) SetUInt32Value(UNIT_FIELD_LEVEL, 55);
	else                                SetUInt32Value(UNIT_FIELD_LEVEL, 1);
	PlayerCreateInfo *info = objmgr.GetPlayerCreateInfo(getRace(), getClass());
	//ASSERT(info);
	if(!info)
	{
		Log.Error("Player::Reset_ToLevel1()","Les tables Createplayer_info pour Race %u et Classe %u n'existe pas! (%s)",this->getRace(),this->getClass(),this->GetName());
		return;
	}
    
	SetUInt32Value(UNIT_FIELD_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_POWER1, info->mana );
	SetUInt32Value(UNIT_FIELD_POWER2, 0 ); // this gets devided by 10
	SetUInt32Value(UNIT_FIELD_POWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_POWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_POWER7, 0 );
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, info->health);
	SetUInt32Value(UNIT_FIELD_BASE_MANA, info->mana);
	SetUInt32Value(UNIT_FIELD_MAXPOWER1, info->mana );
	SetUInt32Value(UNIT_FIELD_MAXPOWER2, info->rage );
	SetUInt32Value(UNIT_FIELD_MAXPOWER3, info->focus );
	SetUInt32Value(UNIT_FIELD_MAXPOWER4, info->energy );
	SetUInt32Value(UNIT_FIELD_MAXPOWER7, 1000 );
	SetUInt32Value(UNIT_FIELD_STAT0, info->strength );
	SetUInt32Value(UNIT_FIELD_STAT1, info->ability );
	SetUInt32Value(UNIT_FIELD_STAT2, info->stamina );
	SetUInt32Value(UNIT_FIELD_STAT3, info->intellect );
	SetUInt32Value(UNIT_FIELD_STAT4, info->spirit );
	SetUInt32Value(UNIT_FIELD_ATTACK_POWER, info->attackpower );
	SetUInt32Value(PLAYER_CHARACTER_POINTS1,0);
	SetUInt32Value(PLAYER_CHARACTER_POINTS2,2);
	for(uint32 x=0;x<7;x++)
		SetFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT+x, 1.00);

}

void Player::CalcResistance(uint32 type)
{
	int32 res;
	int32 pos;
	int32 neg;
	//ASSERT(type < 7);
    if(type >= 7)
	{
		Log.Error("Player::CalcResistance","Type est >= 7 (%s)",this->GetName());
		return;
	}

	pos=(BaseResistance[type]*BaseResistanceModPctPos[type])/100;
	neg=(BaseResistance[type]*BaseResistanceModPctNeg[type])/100;

	pos+=FlatResistanceModifierPos[type];
	neg+=FlatResistanceModifierNeg[type];
	res = BaseResistance[type] + pos-neg;
	if(type==0)res+=m_uint32Values[UNIT_FIELD_STAT1]*2;//fix armor from agi
	if(res<0)res=0;
	pos+=(res*ResistanceModPctPos[type])/100;
	neg+=(res*ResistanceModPctNeg[type])/100;
	res=pos-neg+BaseResistance[type];
	if(type==0)res+=m_uint32Values[UNIT_FIELD_STAT1]*2;//fix armor from agi

	if( res < 0 )
		res = 1;

	SetUInt32Value(UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE+type,pos);
	SetUInt32Value(UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE+type,neg);
	SetUInt32Value(UNIT_FIELD_RESISTANCES+type, res > 0 ? res : 0);

}


void Player::UpdateNearbyGameObjects()
{
	//Log.Notice("[Player::UpdateNearbyGameObjects]","%s est proche d'un Go",this->GetName());
	for (Object::InRangeSet::iterator itr = GetInRangeSetBegin(); itr != GetInRangeSetEnd(); ++itr)
	{
		if((*itr)->GetTypeId() == TYPEID_GAMEOBJECT)
		{
			bool activate_quest_object = false;
			GameObject *go = ((GameObject*)*itr);
			QuestLogEntry *qle;
			GameObjectInfo *info;

			info = go->GetInfo();
			bool deactivate = false;
			if(info &&
				(info->goMap.size() || info->itemMap.size()) )
			{
				for(GameObjectGOMap::iterator itr = go->GetInfo()->goMap.begin();
					itr != go->GetInfo()->goMap.end();
					++itr)
				{
					qle = GetQuestLogForEntry(itr->first->id);
					if(qle)
					{
						for(uint32 i = 0; i < qle->GetQuest()->count_required_mob; ++i)
						{
							if(qle->GetQuest()->required_mob[i] == go->GetEntry() &&
								qle->GetMobCount(i) < qle->GetQuest()->required_mobcount[i])
							{
								activate_quest_object = true;
								break;
							}
						}
						if(activate_quest_object)
							break;
					}
				}

				if(!activate_quest_object)
				{
					for(GameObjectItemMap::iterator itr = go->GetInfo()->itemMap.begin();
						itr != go->GetInfo()->itemMap.end();
						++itr)
					{
						for(std::map<uint32, uint32>::iterator it2 = itr->second.begin();
							it2 != itr->second.end(); 
							++it2)
						{
							if(GetItemInterface()->GetItemCount(it2->first) < it2->second)
							{
								activate_quest_object = true;
								break;
							}
						}
						if(activate_quest_object)
							break;
					}
				}

				if(!activate_quest_object)
				{
					deactivate = true;
				}
			}
			bool bPassed = !deactivate;
			//if((*itr)->GetUInt32Value(GAMEOBJECT_TYPE_ID) == GAMEOBJECT_TYPE_QUESTGIVER)
            if((*itr)->GetByte(GAMEOBJECT_BYTES_1, 1) == GAMEOBJECT_TYPE_QUESTGIVER)
			{
				if(((GameObject*)(*itr))->m_quests)
				{
					if(((GameObject*)(*itr))->m_quests->size() > 0)
					{
						std::list<QuestRelation*>::iterator itr2 = ((GameObject*)(*itr))->m_quests->begin();
						for(;itr2!=((GameObject*)(*itr))->m_quests->end();++itr2)
						{
							uint32 status = sQuestMgr.CalcQuestStatus(NULL, this, (*itr2)->qst, (*itr2)->type, false);
							if(status == QMGR_QUEST_CHAT || status == QMGR_QUEST_AVAILABLE || status == QMGR_QUEST_REPEATABLE || status == QMGR_QUEST_FINISHED)
							{
								// Activate gameobject
								EventActivateGameObject((GameObject*)(*itr));
								bPassed = true;
								break;
							}
						}
					}
				}
			}
			if(!bPassed) 
				EventDeActivateGameObject((GameObject*)(*itr));
		}
	}
}


void Player::EventTaxiInterpolate()
{
	if(!m_CurrentTaxiPath || m_mapMgr==NULL) return;

	float x,y,z;
	uint32 ntime = getMSTime();

	if (ntime > m_taxi_ride_time)
		m_CurrentTaxiPath->SetPosForTime(x, y, z, ntime - m_taxi_ride_time, &lastNode, m_mapId);
	else
		m_CurrentTaxiPath->SetPosForTime(x, y, z, m_taxi_ride_time - ntime, &lastNode, m_mapId);

	if(x < _minX || x > _maxX || y < _minY || y > _maxX)
		return;

	SetPosition(x,y,z,0);
}

void Player::TaxiStart(TaxiPath *path, uint32 modelid, uint32 start_node)
{
	if (GetCurrentSpell()) GetCurrentSpell()->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"

	if(start_node > NUM_TAXI_NODES) return;

	//Log.Warning("TaxiStart","TaxiPathId: %u, PathNode %u",path->GetID(),path->GetPathNode(start_node)->id);
	//Log.Warning("         ","From %u To %u",path->GetFromNode(),path->GetToNode());
           
	//handle departure event :)
	DBCTaxiPathNode* node = dbcTaxiPathNode.LookupEntry(path->GetPathNode(start_node)->id);
	if (node != NULL && node->DepartureEventId != 0)
	{
		//Log.Warning("TaxiStart","Node %u Event",node->Id);
		sScriptMgr.CreateScriptClassForEvent(node->DepartureEventId, this, NULL, NULL);
	}
	int32 mapchangeid = -1;
	float mapchangex;
	float mapchangey;
	float mapchangez;
	uint32 cn = m_taxiMapChangeNode;

	m_taxiMapChangeNode = 0;

	if(this->m_MountSpellId) RemoveAura(m_MountSpellId);
	//also remove morph spells
	if(GetUInt32Value(UNIT_FIELD_DISPLAYID)!=GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID))
	{
		RemoveAllAuraType(SPELL_AURA_TRANSFORM);
		RemoveAllAuraType(SPELL_AURA_MOD_SHAPESHIFT);
	}

	SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, modelid );
	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

	SetTaxiPath(path);
	SetTaxiPos();
	SetTaxiState(true);
	m_taxi_ride_time = getMSTime();

	//uint32 traveltime = uint32(path->getLength() * TAXI_TRAVEL_SPEED); // 36.7407
	float traveldist = 0;
   
	// temporary workaround for taximodes with changing map 
	if (   path->GetID() == 766 // from 68 to 83
		|| path->GetID() == 767 // from 83 to 68
		|| path->GetID() == 771 // from 68 to 205
		|| path->GetID() == 772 // from 205 to 68
		|| path->GetID() == 775 // from 67 to 205
		|| path->GetID() == 776 // from 205 to 67
		|| path->GetID() == 796 // from 67 to 213
		|| path->GetID() == 797 // from 213 to 67
		|| path->GetID() == 807) // from 6 to 213
    {
		Log.Warning("TaxiStart","Jump To end");
		JumpToEndTaxiNode(path);
		return;
	}

	float lastx = 0, lasty = 0, lastz = 0;
	TaxiPathNode *firstNode = path->GetPathNode(start_node);
	uint32 add_time = 0;
	if(start_node)
	{
		TaxiPathNode *pn = path->GetPathNode(0);
		float dist = 0;
		lastx = pn->x;
		lasty = pn->y;
		lastz = pn->z;
		for(uint32 i = 1; i <= start_node; ++i)
		{
			pn = path->GetPathNode(i);
			if(!pn)
			{
				JumpToEndTaxiNode(path);
				return;
			}

			dist += CalcDistance(lastx, lasty, lastz, pn->x, pn->y, pn->z);
			lastx = pn->x;
			lasty = pn->y;
			lastz = pn->z;
		}
		add_time = uint32( dist * TAXI_TRAVEL_SPEED );
		lastx = lasty = lastz = 0;
	}
	size_t endn = path->GetNodeCount();
	if(m_taxiPaths.size()) 
		endn-= 2;

	for(uint32 i = start_node; i < endn; ++i)
	{
		TaxiPathNode *pn = path->GetPathNode(i);

		// temporary workaround for taximodes with changing map
		if (!pn || 
			path->GetID() == 766 || 
			path->GetID() == 767 || 
			path->GetID() == 771 || 
			path->GetID() == 772)
		{
			JumpToEndTaxiNode(path);
			Log.Warning("TaxiNode","Changement de map %u",path->GetID());
			return;
		}

		if( pn->mapid != m_mapId )
		{
			endn = (i - 1);
			m_taxiMapChangeNode = i;

			mapchangeid = (int32)pn->mapid;
			mapchangex = pn->x;
			mapchangey = pn->y;
			mapchangez = pn->z;
			break;
		}

		if(!lastx || !lasty || !lastz)
		{
			lastx = pn->x;
			lasty = pn->y;
			lastz = pn->z;
		} 
		else 
		{			
			float dist = CalcDistance(lastx,lasty,lastz,
				pn->x,pn->y,pn->z);
			traveldist += dist;
			lastx = pn->x;
			lasty = pn->y;
			lastz = pn->z;
		}
	}

	uint32 traveltime = uint32(traveldist * TAXI_TRAVEL_SPEED);

	if( (start_node > endn) || ((endn - start_node) > 200) )
	{
		Log.Warning("Taxi","start_node(%u) > endn(%u) || ((endn(%u) - start_node(%u)) > 200)",
			start_node,endn,endn,start_node);
		return;
	}

	sLog.outDebug("Taxi: SMSG_MONSTER_MOVE");

	/*
	MONSTER_MOVE_FLAG_WALK		= 0x0,
	MONSTER_MOVE_FLAG_RUN		= 0x1000,
	MONSTER_MOVE_FLAG_TELEPORT	= 0x100,
	MONSTER_MOVE_FLAG_FLY		= 0x3000,
	*/

	//Log.Warning("TaxiPathNode","First Id:%u Map:%u X:%.03f Y:%.03f Z:%.03f",firstNode->id,firstNode->mapid,firstNode->x,firstNode->y,firstNode->z);

	WorldPacket data(SMSG_MONSTER_MOVE, 38 + ( (endn - start_node) * 12 ) ); // 12 = 3*4 octets ( x y z )
	data << GetNewGUID();                                 // +8 
	data << uint8(0);
	data << firstNode->x << firstNode->y << firstNode->z; // +12
	data << m_taxi_ride_time;                             // +4
	data << uint8( 0 );                                   // +1
	data << uint32( MONSTER_MOVE_FLAG_FLY); //  0x3000 // +4
	data << uint32( traveltime );                         // +4

	if(!cn)
		m_taxi_ride_time -= add_time;
	
	data << uint32( endn - start_node );
//	uint32 timer = 0, nodecount = 0;
//	TaxiPathNode *lastnode = NULL;

	for(uint32 i = start_node; i < endn; i++)
	{
		TaxiPathNode *pn = path->GetPathNode(i);
		if(!pn)
		{
			JumpToEndTaxiNode(path);
			return;
		}

		//Log.Warning("TaxiPathNode","Id:%u Map:%u X:%5.05f Y:%5.05f Z:%5.05f",pn->id,pn->mapid,pn->x,pn->y,pn->z);

		data << pn->x << pn->y << pn->z;
	}

	SendMessageToSet(&data, true);
  
	sEventMgr.AddEvent(this, &Player::EventTaxiInterpolate, 
		EVENT_PLAYER_TAXI_INTERPOLATE, 900, 0,0);

	if( mapchangeid < 0 )
	{
		TaxiPathNode *pn = path->GetPathNode((uint32)path->GetNodeCount() - 1);
		sEventMgr.AddEvent(this, &Player::EventTaxiDismount, pn, EVENT_UNK, traveltime, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.AddEvent(this, &Player::EventDismount, path->GetPrice(), 
			pn->x, pn->y, pn->z, EVENT_PLAYER_TAXI_DISMOUNT, traveltime, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT); 
	}
	else
	{
		sEventMgr.AddEvent( this, &Player::EventTeleport, (uint32)mapchangeid, mapchangex, mapchangey, mapchangez, EVENT_PLAYER_TELEPORT, traveltime, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Player::JumpToEndTaxiNode(TaxiPath * path)
{
	// this should *always* be safe in case it cant build your position on the path!
	TaxiPathNode * pathnode = path->GetPathNode((uint32)path->GetNodeCount()-1);
	if(!pathnode) return;

	ModUnsigned32Value( PLAYER_FIELD_COINAGE , -(int32)path->GetPrice());

	SetTaxiState(false);
	SetTaxiPath(NULL);
	UnSetTaxiPos();
	m_taxi_ride_time = 0;

	SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID , 0);
	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

	SetPlayerSpeed(RUN, m_runSpeed);

	SafeTeleport(pathnode->mapid, 0, LocationVector(pathnode->x, pathnode->y, pathnode->z));

	// Start next path if any remaining
	if(m_taxiPaths.size())
	{
		TaxiPath * p = *m_taxiPaths.begin();
		m_taxiPaths.erase(m_taxiPaths.begin());
		TaxiStart(p, taxi_model_id, 0);
	}
}

void Player::RemoveSpellsFromLine(uint32 skill_line)
{
	uint32 cnt = dbcSkillLineSpell.GetNumRows();
	for(uint32 i=0; i < cnt; i++)
	{
		skilllinespell * sp = dbcSkillLineSpell.LookupRow(i);
		if(sp)
		{
			if(sp->skilline == skill_line)
			{
				// Check ourselves for this spell, and remove it..
					removeSpell(sp->spell, 0, 0, 0);
			}
		}
	}
}

void Player::CalcStat(uint32 type)
{
	int32 res;
	//ASSERT( type < 5 );
    if(type >= 5)
	{
		Log.Error("Player::CalcStat","Type est >= 5 (%s)",this->GetName());
		return;
	}


	int32 pos = (BaseStats[type] * StatModPct[type] ) / 100 + FlatStatMod[type];
	int32 neg = (BaseStats[type] * StatModPctNeg[type] ) / 100 + FlatStatModNeg[type];
	res = pos + BaseStats[type] - neg;
	if( res <= 0 )
	{
		res = 1;
	}
	else
	{
		pos += ( res * static_cast< Player* >( this )->TotalStatModPct[type] ) / 100;
		neg += ( res * static_cast< Player* >( this )->TotalStatModPctNeg[type] ) / 100;
		res = pos + BaseStats[type] - neg;
		if( res <= 0 )
			res = 1;
	}

	SetUInt32Value( UNIT_FIELD_POSSTAT0 + type, pos );
	SetUInt32Value( UNIT_FIELD_NEGSTAT0 + type, neg );
	SetUInt32Value( UNIT_FIELD_STAT0 + type, res > 0 ?res : 0 );
	if( type == 1 )
	   CalcResistance( 0 );
}

void Player::RegenerateMana(bool is_interrupted)
{
	// ATTENTION: Le client calcul de son coté a partir d'une valeur rafraichi par le serveur
	//            La valeur Mana++ toute les 2 secondes doit etre affinée 
    //
	//           Les champs UNIT_FIELD_MAXPOWERx sont géré comme des uint32
	//           Le montant du regen est un float, on perd donc les 100ieme a chaque tour
	//           ex: Actuel mana level60 = 44.349032/2secs, donc a chaque tour on perd 0.349032 (ttes les 2sec)
	//               car en fait c'est plutot 44 qui est ajouté a la valeur precedente 
    //	             en fait il manque le +0.349032 n'est pas ajouté, 
	//               on perd en fait 1pts toute les 6 secs, en gros on perd une 100n de points sur un regen complet
	//               En fait, ca passe completement inapercu vu du player s'il n'est pas trop rapide ou qu'il a du GamePlay)
	// ( Branruz - 332.11403 )

	uint32 cur = GetUInt32Value(UNIT_FIELD_POWER1);
	uint32 mm = GetUInt32Value(UNIT_FIELD_MAXPOWER1);

    if(cur >= mm) return;

	float amt = (is_interrupted) ? GetFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER) : GetFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER);	
	amt *= 2; //floats are Mana Regen Per Sec. Regen Applied every 2 secs so real value =X*2 . Shady (oui c'est exact, Branruz)
	//Log.Warning("Player::RegenerateMana","Mult1: %f",amt);
	
	//Apply shit from conf file
	amt *= sWorld.getRate(RATE_POWER1);
    //Log.Warning("Player::RegenerateMana","+Mult2: %f",amt);

	if((amt<=1.0)&&(amt>0)) //this fixes regen like 0.98
	{
		if(is_interrupted) return;
		cur++;
	}
	else
		cur += float2int32(amt); // Ca ne va pas, juste une approche, il manque les 100iemes

    // cur = cur + ???; // On compense la latence ( a affiner plus tard en fonction du level ) (Branruz)

	uint32 power = (cur >= mm) ? mm : cur;
	SetUInt32Value(UNIT_FIELD_POWER1, power);


	Log.Warning("Player::RegenerateMana","(%s) Mana %u/%u IRQS: %s, ",this->GetName(),cur,mm,(is_interrupted)?"Oui":"Non");

	/* - NE PAS VIRER -
	WorldPacket data(SMSG_POWER_UPDATE, 14);
	FastGUIDPack(data, GetGUID());
	data << uint8(0);
	data << power;
	SendMessageToSet(&data, true);
	*/
}

void Player::RegenerateHealth( bool inCombat )
{

	gtFloat* HPRegenBase,* HPRegen;

	float amt  = 1.0;
	uint32 cur = GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 mh  = GetUInt32Value(UNIT_FIELD_MAXHEALTH);

	sLog.outDebug("RegenerateHealth: %s Heal %u/%u ,En combat: %s",this->GetName(),cur,mh,(inCombat)?"Oui":"Non");

	if(cur >= mh) return;

	// En combat: PctIgnoreRegenModifier est recalculé en fonction du heal restant
	//            moins il reste de Heal, moins est la regen auto
	//            Pourcent en combat = PourcentEnCombatParDefaut * Pourcent_de_Heal_restant
	// Prob: En bas level, une creature peut taper moins fort que le heal ne regen....bon, a voir qd meme!!
	//       Reduit a 10% en combat, a reduire encore surement....
    if(inCombat) // Test d'abord si combat ou pas, obtimisation....
	{
		if(this->getLevel() < 10) return; // Ne concerne pas les player bas level

		if(PctIgnoreRegenModifier!=0)
		{
		 float ModifPctInRestHeal = PctIgnoreRegenModifier;
         float PctResting = (float)(mh-cur)/(float)(mh);
         ModifPctInRestHeal *= PctResting;
		 if(ModifPctInRestHeal < 1) ModifPctInRestHeal = 0;
		 amt = ModifPctInRestHeal;
		 if(!amt) return;
		}
		else return; // Pas de Regen en combat si PctIgnoreRegenModifier = 0 (Branruz)
	}

    // Level Max Player = 100 en 332.11403 ,hmmm surement pour faire des mega Gm ;)
	HPRegenBase = dbcHPRegenBase.LookupEntry((getLevel()-1) + ((getClass()-1)*100)); 
	HPRegen     = dbcHPRegen.LookupEntry((getLevel()-1) + ((getClass()-1)*100));     
	
	if((HPRegen==NULL) || (HPRegenBase==NULL)) // Le sWorld.m_levelCap est > 100 (pour l'instant ?) (Secu - Branruz)
	{
      HPRegenBase = dbcHPRegenBase.LookupEntry(99 + ((getClass()-1)*100)); 
	  HPRegen     = dbcHPRegen.LookupEntry(99 + ((getClass()-1)*100));
	}
    /*
	UNIT_FIELD_STAT0 Force
	UNIT_FIELD_STAT1 Abilité
	UNIT_FIELD_STAT2 Endu
	UNIT_FIELD_STAT3 Intell
	UNIT_FIELD_STAT4 Esprit -> Augmente le Heal
	*/
	amt *= ( (m_uint32Values[UNIT_FIELD_STAT4]*HPRegen->val)+ (HPRegenBase->val*100)); 
	
	amt *= sWorld.getRate(RATE_HEALTH);//Apply shit from conf file

	if(PctRegenModifier) amt+= (amt * PctRegenModifier) / 100.0;

	// Arriver a l'auberge ? (le zzz)
	if(m_isResting) amt = amt * 1.33f; // wowwiki: Health Regeneration is increased by 33% while sitting.

	if(amt != 0)
	{
		if(amt > 0)
		{
			if(amt <= 1.0f) cur++;        //this fixes regen like 0.98
			else            cur += float2int32(amt);

			SetUInt32Value(UNIT_FIELD_HEALTH,(cur>=mh) ? mh : cur);
		}
		else
			DealDamage(this, float2int32(amt), 0, 0, 0); // Pourquoi virer le DealDamage ?? (Branruz)
	}
}

void Player::LooseRage(int32 decayValue)
{
	//Rage is lost at a rate of 3 rage every 3 seconds. 
	//The Anger Management talent changes this to 2 rage every 3 seconds.
	uint32 cur = GetUInt32Value(UNIT_FIELD_POWER2);
    uint32 newrage = ((int)cur <= decayValue) ? 0 : cur-decayValue;

    if (newrage > 1000 ) newrage = 1000;

    SetUInt32Value(UNIT_FIELD_POWER2,newrage);
	
	WorldPacket data(SMSG_POWER_UPDATE, 14);
	FastGUIDPack(data, GetGUID());
	data << uint8(POWER_TYPE_RAGE);
	data << newrage;
	SendMessageToSet(&data, true);
 }

 void Player::LooseRunic(int32 decayValue)
{
	uint32 cur = GetUInt32Value(UNIT_FIELD_POWER7);
	uint32 newrp = ((int)cur <= decayValue) ? 0 : cur - decayValue;
	if (newrp > 1000 )
		newrp = 1000;

	SetUInt32Value(UNIT_FIELD_POWER7, newrp);

}
 
void Player::SetRage(int32 value)
{
    uint32 newrage = value;
    if (newrage > 1000 )
	  newrage = 1000;

    SetUInt32Value(UNIT_FIELD_POWER2, newrage);

	WorldPacket data(SMSG_POWER_UPDATE, 14);
	data << GetNewGUID();
	data << uint8(POWER_TYPE_RAGE);
	data << newrage;
	SendMessageToSet(&data, true);
}

void Player::RegenerateEnergy()
{
	uint32 cur = GetUInt32Value(UNIT_FIELD_POWER4);
	uint32 mh = GetUInt32Value(UNIT_FIELD_MAXPOWER4);
	if( cur >= mh )
		return;

	float amt = 2.0f * PctPowerRegenModifier[POWER_TYPE_ENERGY];

	cur += float2int32(amt);
	mana_reg += (amt - cur);
	if( mana_reg > 1 )
	{
		cur++;
		mana_reg--;
	}

	SetUInt32Value(UNIT_FIELD_POWER4,(cur>=mh) ? mh : cur);
}
uint32 Player::GeneratePetNumber()
{
	//uint32 val = m_PetNumberMax + 1;
	uint32 i;
	for (i = 1; i < m_PetNumberMax; i++)
	{
	 if(m_Pets.find(i) == m_Pets.end())	break;
		 //return i; // found a free one
	}
    return i; // Normalement la valeur de i ne doit pas depasser le Pets.end
	          // le i++ du for() sera = au m_PetNumberMax si le break ne se produit pas.

	//return(m_PetNumberMax + 1); // Ne doit pas arriver
}

void Player::RemovePlayerPet(uint32 pet_number)
{
	std::map<uint32, PlayerPet*>::iterator itr = m_Pets.find(pet_number);
	if(itr != m_Pets.end())
	{
		delete itr->second;
		m_Pets.erase(itr);
		EventDismissPet();
	}
}

#ifndef CLUSTERING
void Player::_Relocate(uint32 mapid, const LocationVector & v, bool sendpending, bool force_new_world, uint32 instance_id)
{
	//this func must only be called when switching between maps!
	WorldPacket data(41);
	if(sendpending && (mapid != m_mapId) && force_new_world)
	{
		data.SetOpcode(SMSG_TRANSFER_PENDING);
		data << mapid;
		GetSession()->SendPacket(&data);
	}

	//are we changing maps?
	if(m_mapId != mapid || force_new_world)
	{
		//Preteleport will try to find an instance (saved or active), or create a new one if none found.
		uint32 status = sInstanceMgr.PreTeleport(mapid, this, instance_id);
		if(status != INSTANCE_OK)
		{
			data.Initialize(SMSG_TRANSFER_ABORTED);
			data << mapid << status;
			GetSession()->SendPacket(&data);
			return;
		}

		//did we get a new instanceid?
		if(instance_id)
			m_instanceId=instance_id;

		//remove us from this map
		if(IsInWorld())
		{
			RemoveFromWorld();
		}

		// SMSG_NEW_WORLD : 61020000 ADC61345 875AB4C5 F05D1743 DB0F493F
		data.Initialize(SMSG_NEW_WORLD);
		data << (uint32)mapid << v << v.o;
		GetSession()->SendPacket( &data );
		SetMapId(mapid);
		SetPlayerStatus(TRANSFER_PENDING);
	}
	else
	{

		// we are on same map allready, no further checks needed,
		// send teleport ack msg
		WorldPacket * data = BuildTeleportAckMsg(v);
		m_session->SendPacket(data);
		delete data;

		//reset transporter if we where on one.
		if( m_CurrentTransporter && !m_lockTransportVariables )
		{
			m_CurrentTransporter->RemovePlayer(this);
			m_CurrentTransporter = NULL;
			m_TransporterGUID = 0;
		}
	}

	//update position
	m_sentTeleportPosition = v;
	SetPosition(v);
	ResetHeartbeatCoords();

	z_axisposition = 0.0f;
}
#endif

void Player::UpdateKnownCurrencies(uint32 ItemId, bool apply)
{
	if(CurrencyTypesEntry * ctEntry = dbcCurrencyTypes.LookupEntryForced(ItemId))
	{
		if(apply)
			SetFlag64(PLAYER_FIELD_KNOWN_CURRENCIES,(1 << (ctEntry->BitIndex-1)));
		else
			RemoveFlag64(PLAYER_FIELD_KNOWN_CURRENCIES,(1 << (ctEntry->BitIndex-1)));
	}
}

// Player::AddItemsToWorld
// Adds all items to world, applies any modifiers for them.

void Player::AddItemsToWorld()
{
	Item * pItem;
	for(uint32 i = 0; i < INVENTORY_CURRENCY_END; i++)
	{
		pItem = GetItemInterface()->GetInventoryItem(i);
		if( pItem != NULL )
		{
			pItem->PushToWorld(m_mapMgr);
			
			if(i < INVENTORY_SLOT_BAG_END)	  // only equipment slots get mods.
			{
				_ApplyItemMods(pItem, i, true, false, true);
			}

			if(pItem->IsContainer() && GetItemInterface()->IsBagSlot(i))
			{
				for(uint32 e=0; e < pItem->GetProto()->ContainerSlots; e++)
				{
					Item *item = ((Container*)pItem)->GetItem(e);
					if(item)
					{
						item->PushToWorld(m_mapMgr);
					}
				}
			}
		}
	}

	UpdateStats();
}

// Player::RemoveItemsFromWorld
// Removes all items from world, reverses any modifiers.

void Player::RemoveItemsFromWorld()
{
	Item * pItem;
	for(uint32 i = 0; i < INVENTORY_KEYRING_END; i++)
	{
		pItem = m_ItemInterface->GetInventoryItem((int8)i);
		if(pItem)
		{
			if(pItem->IsInWorld())
			{
				if(i < INVENTORY_SLOT_BAG_END)	  // only equipment slots get mods.
				{
					_ApplyItemMods(pItem, i, false, false, true);
				}
				pItem->RemoveFromWorld();
			}
	
			if(pItem->IsContainer() && GetItemInterface()->IsBagSlot(i))
			{
				for(uint32 e=0; e < pItem->GetProto()->ContainerSlots; e++)
				{
					Item *item = ((Container*)pItem)->GetItem(e);
					if(item && item->IsInWorld())
					{
						item->RemoveFromWorld();
					}
				}
			}
		}
	}

	UpdateStats();
}

uint32 Player::BuildCreateUpdateBlockForPlayer(ByteBuffer *data, Player *target )
{
	int count = 0;
	if(target == this)
	{
		// we need to send create objects for all items.
		count += GetItemInterface()->m_CreateForPlayer(data);
	}
	count += Unit::BuildCreateUpdateBlockForPlayer(data, target);
	return count;
}

void Player::_Warn(const char *message)
{
	sChatHandler.RedSystemMessage(GetSession(), message);
}

void Player::Kick(uint32 delay /* = 0 */ )
{
	if(!delay)
	{
		m_KickDelay = 0;
		_Kick();
	} 
	else 
	{
		m_KickDelay = delay;
		sEventMgr.AddEvent(this, &Player::_Kick, EVENT_PLAYER_KICK, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Player::_Kick()
{
    if(!m_KickDelay)
	{
		sEventMgr.RemoveEvents(this, EVENT_PLAYER_KICK);
		// remove now
		//GetSession()->LogoutPlayer(true);
		m_session->Disconnect();
	} 
	else 
	{
		if((m_KickDelay - 1000) < 500)
		{
			m_KickDelay = 0;
		} 
		else 
		{
			m_KickDelay -= 1000;
		}
		sChatHandler.BlueSystemMessageToPlr(this, "You will be removed from the server in %u seconds.", (uint32)(m_KickDelay/1000));
	}
}

bool Player::HasDeletedSpell(uint32 spell)
{
	return (mDeletedSpells.count(spell) > 0);
}

void Player::ClearCooldownForSpell(uint32 spell_id)
{
	if(!IsInWorld()) return;

	WorldPacket data(12);
	data.SetOpcode(SMSG_CLEAR_COOLDOWN);
	data << spell_id << GetGUID();
	GetSession()->SendPacket(&data);

	// remove cooldown data from Server side lists
	uint32 i;
	PlayerCooldownMap::iterator itr, itr2;
	SpellEntry * spe = dbcSpell.LookupEntry(spell_id);
	if(!spe) return;

	for(i = 0; i < NUM_COOLDOWN_TYPES; ++i)
	{
		for( itr = m_cooldownMap[i].begin(); itr != m_cooldownMap[i].end(); )
		{
			itr2 = itr++;
			if( ( i == COOLDOWN_TYPE_CATEGORY && itr2->first == spe->Category ) ||
				( i == COOLDOWN_TYPE_SPELL && itr2->first == spe->Id ) )
			{
				m_cooldownMap[i].erase( itr2 );
			}
		}
	}
}

void Player::ClearCooldownsOnLine(uint32 skill_line, uint32 called_from)
{
	// found an easier way.. loop spells, check skill line
	SpellSet::const_iterator itr = mSpells.begin();
	skilllinespell *sk;
	for(; itr != mSpells.end(); ++itr)
	{
		if((*itr) == called_from)	   // skip calling spell.. otherwise spammies! :D
			continue;

		sk = objmgr.GetSpellSkill((*itr));
		if(sk && sk->skilline == skill_line)
			ClearCooldownForSpell((*itr));
	}
}

void Player::ResetAllCooldowns()
{
	uint32 guid = (uint32)GetSelection();

	switch(getClass())
	{
		case WARRIOR:
		{
			ClearCooldownsOnLine(26, guid);
			ClearCooldownsOnLine(256, guid);
			ClearCooldownsOnLine(257 , guid);
		} break;
		case PALADIN:
		{
			ClearCooldownsOnLine(594, guid);
			ClearCooldownsOnLine(267, guid);
			ClearCooldownsOnLine(184, guid);
		} break;
		case HUNTER:
		{
			ClearCooldownsOnLine(50, guid);
			ClearCooldownsOnLine(51, guid);
			ClearCooldownsOnLine(163, guid);
		} break;
		case ROGUE:
		{
			ClearCooldownsOnLine(253, guid);
			ClearCooldownsOnLine(38, guid);
			ClearCooldownsOnLine(39, guid);
		} break;
		case PRIEST:
		{
			ClearCooldownsOnLine(56, guid);
			ClearCooldownsOnLine(78, guid);
			ClearCooldownsOnLine(613, guid);
		} break;
		case SHAMAN:
		{
			ClearCooldownsOnLine(373, guid);
			ClearCooldownsOnLine(374, guid);
			ClearCooldownsOnLine(375, guid);
		} break;
		case MAGE:
		{
			ClearCooldownsOnLine(6, guid);
			ClearCooldownsOnLine(8, guid);
			ClearCooldownsOnLine(237, guid);
		} break;
		case WARLOCK:
		{
			ClearCooldownsOnLine(355, guid);
			ClearCooldownsOnLine(354, guid);
			ClearCooldownsOnLine(593, guid);
		} break;
		case DRUID:
		{
			ClearCooldownsOnLine(573, guid);
			ClearCooldownsOnLine(574, guid);
			ClearCooldownsOnLine(134, guid);
		} break;
		case DEATHKNIGHT:
		{
			ClearCooldownsOnLine(770, guid); 
			ClearCooldownsOnLine(771, guid); 
			ClearCooldownsOnLine(772, guid); 
		} break;
		default: return; break;
	}
}

void Player::PushUpdateData(ByteBuffer *data, uint32 updatecount)
{
	// imagine the bytebuffer getting appended from 2 threads at once! :D
	_bufferS.Acquire();

	// unfortunately there is no guarantee that all data will be compressed at a ratio
	// that will fit into 2^16 bytes ( stupid client limitation on server packets )
	// so if we get more than 63KB of update data, force an update and then append it
	// to the clean buffer.
	if( (data->size() + bUpdateBuffer.size() ) >= 63000 )
	{
		if( IsInWorld() )
			ProcessPendingUpdates(&m_mapMgr->m_updateBuildBuffer, &m_mapMgr->m_compressionBuffer);
		else
			bUpdateBuffer.clear();
	}

	mUpdateCount += updatecount;
	bUpdateBuffer.append(*data);

	// add to process queue
	if(m_mapMgr && !bProcessPending)
	{
		bProcessPending = true;
		m_mapMgr->PushToProcessed(this);
	}
	
	_bufferS.Release();
}

void Player::PushUpdateData(StackBuffer *data, uint32 updatecount)
{
	// imagine the bytebuffer getting appended from 2 threads at once! :D
	_bufferS.Acquire();

	// unfortunately there is no guarantee that all data will be compressed at a ratio
	// that will fit into 2^16 bytes ( stupid client limitation on server packets )
	// so if we get more than 63KB of update data, force an update and then append it
	// to the clean buffer.
	if( (data->GetSize() + bUpdateBuffer.size() ) >= 63000 )
	{
		if( IsInWorld() )
			ProcessPendingUpdates(&m_mapMgr->m_updateBuildBuffer, &m_mapMgr->m_compressionBuffer);
		else
			bUpdateBuffer.clear();
	}
	
	mUpdateCount += updatecount;
	bUpdateBuffer.append((const uint8*)data->GetBufferPointer(), data->GetSize());

	// add to process queue
	if(m_mapMgr && !bProcessPending)
	{
		bProcessPending = true;
		m_mapMgr->PushToProcessed(this);
	}

	_bufferS.Release();
}

void Player::PushOutOfRange(const WoWGuid & guid)
{
	_bufferS.Acquire();
	mOutOfRangeIds << guid;
	++mOutOfRangeIdCount;

	// add to process queue
	if(m_mapMgr && !bProcessPending)
	{
		bProcessPending = true;
		m_mapMgr->PushToProcessed(this);
	}
	_bufferS.Release();
}

void Player::PushCreationData(ByteBuffer *data, uint32 updatecount)
{
    // imagine the bytebuffer getting appended from 2 threads at once! :D
	_bufferS.Acquire();

	// unfortunately there is no guarantee that all data will be compressed at a ratio
	// that will fit into 2^16 bytes ( stupid client limitation on server packets )
	// so if we get more than 63KB of update data, force an update and then append it
	// to the clean buffer.
	if( (data->size() + bCreationBuffer.size() + mOutOfRangeIds.size() ) >= 63000 )
	{
		if( IsInWorld() )
			ProcessPendingUpdates(&m_mapMgr->m_updateBuildBuffer, &m_mapMgr->m_compressionBuffer);
		else
			bCreationBuffer.clear();
	}

	mCreationCount += updatecount;
	bCreationBuffer.append(*data);

	// add to process queue
	if(m_mapMgr && !bProcessPending)
	{
		bProcessPending = true;
		m_mapMgr->PushToProcessed(this);
	}
	
	_bufferS.Release();

}

void Player::ProcessPendingUpdates(ByteBuffer *pBuildBuffer, ByteBuffer *pCompressionBuffer)
{
	//we might be getting moved :P
	if (GetSession() == NULL)
		return;

	_bufferS.Acquire();
    if(!bUpdateBuffer.size() && !mOutOfRangeIds.size() && !bCreationBuffer.size())
	{
		_bufferS.Release();
		return;
	}

	size_t bBuffer_size =  (bCreationBuffer.size() > bUpdateBuffer.size() ? bCreationBuffer.size() : bUpdateBuffer.size()) + 10 + (mOutOfRangeIds.size() * 9);
    //uint8 * update_buffer = new uint8[bBuffer_size];
	pBuildBuffer->resize(bBuffer_size);
	uint8 * update_buffer = (uint8*)pBuildBuffer->contents();
	size_t c = 0;

    //build out of range updates if creation updates are queued
    if(bCreationBuffer.size() || mOutOfRangeIdCount)
    {
        *(uint32*)&update_buffer[c] = ((mOutOfRangeIds.size() > 0) ? (mCreationCount + 1) : mCreationCount);
		c += 4;

            // append any out of range updates
	    if(mOutOfRangeIdCount)
	    {
		    update_buffer[c]				= UPDATETYPE_OUT_OF_RANGE_OBJECTS;
			++c;
            *(uint32*)&update_buffer[c]	 = mOutOfRangeIdCount;
			c += 4;
		    memcpy(&update_buffer[c], mOutOfRangeIds.contents(), mOutOfRangeIds.size());
			c += mOutOfRangeIds.size();
		    mOutOfRangeIds.clear();
		    mOutOfRangeIdCount = 0;
	    }

        if(bCreationBuffer.size())
			memcpy(&update_buffer[c], bCreationBuffer.contents(), bCreationBuffer.size());
		c += bCreationBuffer.size();

        // clear our update buffer
	    bCreationBuffer.clear();
	    mCreationCount = 0;

        // compress update packet
	    // while we said 350 before, i'm gonna make it 500 :D
	    if(c < (size_t)sWorld.compression_threshold || !CompressAndSendUpdateBuffer((uint32)c, update_buffer, pCompressionBuffer))
	    {
		    // send uncompressed packet -> because we failed
		    m_session->OutPacket(SMSG_UPDATE_OBJECT, (uint16)c, update_buffer);
	    }
    }

	if(bUpdateBuffer.size())
	{
		c = 0;
		*(uint32*)&update_buffer[c] = ((mOutOfRangeIds.size() > 0) ? (mUpdateCount + 1) : mUpdateCount);
		c += 4;
		memcpy(&update_buffer[c], bUpdateBuffer.contents(), bUpdateBuffer.size());  c += bUpdateBuffer.size();

		// clear our update buffer
		bUpdateBuffer.clear();
		mUpdateCount = 0;

		// compress update packet
		// while we said 350 before, i'm gonna make it 500 :D
		if(c < (size_t)sWorld.compression_threshold || !CompressAndSendUpdateBuffer((uint32)c, update_buffer, pCompressionBuffer))
		{
			// send uncompressed packet -> because we failed
			m_session->OutPacket(SMSG_UPDATE_OBJECT, (uint16)c, update_buffer);
		}
	}
	
	bProcessPending = false;
	_bufferS.Release();
	//delete [] update_buffer;
	pBuildBuffer->clear();

	// send any delayed packets
	WorldPacket * pck;
	while(delayedPackets.size())
	{
		pck = delayedPackets.next();
		//printf("Delayed packet opcode %u sent.\n", pck->GetOpcode());
		m_session->SendPacket(pck);
		delete pck;
		pck=NULL;
	}

	// resend speed if needed
	if(resend_speed)
	{
		SetPlayerSpeed(RUN, m_runSpeed);
		SetPlayerSpeed(FLY, m_flySpeed);
		resend_speed=false;
	}
}

bool Player::CompressAndSendUpdateBuffer(uint32 size, const uint8* update_buffer, ByteBuffer *pCompressionBuffer)
{
	uint32 destsize = size + size/10 + 16;
	int rate = sWorld.getIntRate(INTRATE_COMPRESSION);
	if(size >= 40000 && rate < 6)
		rate = 6;

	// set up stream
	z_stream stream;
	stream.zalloc = 0;
	stream.zfree  = 0;
	stream.opaque = 0;
	
	if(deflateInit(&stream, rate) != Z_OK)
	{
		sLog.outError("deflateInit failed.");
		return false;
	}

	//uint8 *buffer = new uint8[destsize];
	//memset(buffer,0,destsize);	/* fix umr - burlex */
	pCompressionBuffer->resize(destsize);
	uint8 *buffer = (uint8*)pCompressionBuffer->contents();
	
	// set up stream pointers
	stream.next_out  = (Bytef*)buffer+4;
	stream.avail_out = destsize;
	stream.next_in   = (Bytef*)update_buffer;
	stream.avail_in  = size;

	// call the actual process
	if(deflate(&stream, Z_NO_FLUSH) != Z_OK ||
		stream.avail_in != 0)
	{
		sLog.outError("deflate failed.");
		delete [] buffer;
		return false;
	}

	// finish the deflate
	if(deflate(&stream, Z_FINISH) != Z_STREAM_END)
	{
		sLog.outError("deflate failed: did not end stream");
		delete [] buffer;
		return false;
	}

	// finish up
	if(deflateEnd(&stream) != Z_OK)
	{
		sLog.outError("deflateEnd failed.");
		delete [] buffer;
		return false;
	}

	// fill in the full size of the compressed stream
#ifdef USING_BIG_ENDIAN
	*(uint32*)&buffer[0] = swap32(size);
#else
	*(uint32*)&buffer[0] = size;
#endif

	// send it
	m_session->OutPacket(SMSG_COMPRESSED_UPDATE_OBJECT, (uint16)stream.total_out + 4, buffer);

	// cleanup memory
	//delete [] buffer;
	pCompressionBuffer->clear();
	
	return true;
}

void Player::ClearAllPendingUpdates()
{
	_bufferS.Acquire();
	bProcessPending = false;
	mUpdateCount = 0;
	bUpdateBuffer.clear();
	_bufferS.Release();
}

void Player::AddSplinePacket(uint64 guid, ByteBuffer* packet)
{
	SplineMap::iterator itr = _splineMap.find(guid);
	if(itr != _splineMap.end())
	{
		delete itr->second;
		_splineMap.erase(itr);
	}
	_splineMap.insert( SplineMap::value_type( guid, packet ) );
}

ByteBuffer* Player::GetAndRemoveSplinePacket(uint64 guid)
{
	SplineMap::iterator itr = _splineMap.find(guid);
	if(itr != _splineMap.end())
	{
		ByteBuffer *buf = itr->second;
		_splineMap.erase(itr);
		return buf;
	}
	return NULL;
}

void Player::ClearSplinePackets()
{
	SplineMap::iterator it2;
	for(SplineMap::iterator itr = _splineMap.begin(); itr != _splineMap.end();)
	{
		it2 = itr;
		++itr;
		delete it2->second;
		_splineMap.erase(it2);
	}
	_splineMap.clear();
}



bool Player::ExitInstance()
{
	if(!m_bgEntryPointX)
		return false;

	RemoveFromWorld();

	SafeTeleport(m_bgEntryPointMap, m_bgEntryPointInstance, LocationVector(
		m_bgEntryPointX, m_bgEntryPointY, m_bgEntryPointZ, m_bgEntryPointO));

	return true;
}

void Player::SaveEntryPoint(uint32 mapId)
{	
	if(IS_INSTANCE(GetMapId()))
		return; // dont save if we're not on the main continent.
	//otherwise we could end up in an endless loop :P
	MapInfo * pMapinfo = WorldMapInfoStorage.LookupEntry(mapId);

	if(pMapinfo)
	{
		m_bgEntryPointX = pMapinfo->repopx;
		m_bgEntryPointY = pMapinfo->repopy;
		m_bgEntryPointZ = pMapinfo->repopz;
		m_bgEntryPointO = GetOrientation();
		m_bgEntryPointMap = pMapinfo->repopmapid;
		m_bgEntryPointInstance = GetInstanceID();
	}
	else
	{
		m_bgEntryPointMap	 = 0;
		m_bgEntryPointX		 = 0;	
		m_bgEntryPointY		 = 0;
		m_bgEntryPointZ		 = 0;
		m_bgEntryPointO		 = 0;
		m_bgEntryPointInstance  = 0;
	}
}

void Player::CleanupGossipMenu()
{
	if(CurrentGossipMenu)
	{
		delete CurrentGossipMenu;
		CurrentGossipMenu = NULL;
	}
}

void Player::Gossip_Complete()
{
	GetSession()->OutPacket(SMSG_GOSSIP_COMPLETE, 0, NULL);
	CleanupGossipMenu();
}

void Player::ForceAreaUpdate()
{
	// force update area id
	if(m_position.x > _maxX || m_position.x < _minX || m_position.y > _maxY || m_position.y < _minY || !IsInWorld())
		return;

	m_areaDBC = NULL;
	m_AreaID = m_mapMgr->GetAreaID(m_position.x, m_position.y, m_position.z);
	if( m_AreaID == 0xffff )
		m_AreaID = 0;
	else
		m_areaDBC = dbcArea.LookupEntryForced(m_AreaID);

	if( m_areaDBC != NULL )
	{
		// parent id actually would be a better name
		if( m_areaDBC->ZoneId && m_zoneId != m_areaDBC->ZoneId )
			m_zoneId = m_areaDBC->ZoneId;
	}
}

void Player::ZoneUpdate(uint32 ZoneId)
{
	m_zoneId = ZoneId;
	/* how the f*ck is this happening */
	if( m_playerInfo == NULL )
	{
		m_playerInfo = objmgr.GetPlayerInfo(GetLowGUID());
		if( m_playerInfo == NULL )
		{
			m_session->Disconnect();
			return;
		}
	}

	m_playerInfo->lastZone = ZoneId;
	sHookInterface.OnZone(this, ZoneId); // Script

	AreaTable * at = dbcArea.LookupEntry(GetAreaID());

	if (at && !(at->AreaFlags & AREA_ALLOW_FLYING) && flying_aura)
	{
		RemoveAura(flying_aura);
		flying_aura = 0;
	}

	if(at && (at->category == AREAC_SANCTUARY || at->AreaFlags & AREA_SANCTUARY))
	{
		Unit * pUnit = (GetSelection() == 0) ? 0 : (m_mapMgr ? m_mapMgr->GetUnit(GetSelection()) : 0);
		Player* pUnitFrom = NULL;

		if (pUnit != NULL)
			pUnitFrom = pUnit->GetPlayerFrom();

		if(pUnitFrom != NULL && DuelingWith != pUnitFrom)
		{
			EventAttackStop();
			smsg_AttackStop(pUnit);
		}

		if(m_currentSpell)
		{
			Unit * target = m_currentSpell->GetUnitTarget();
			Player* ptargetfrom = target->GetPlayerFrom();
			if(ptargetfrom != NULL && ptargetfrom != DuelingWith)
				m_currentSpell->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
		}
	}

#ifdef OPTIMIZED_PLAYER_SAVING
	save_Zone();
#endif

	// send new world states
	ForceAreaUpdate();

	at = dbcArea.LookupEntryForced(m_zoneId);
	if( m_session->HasGMPermissions() && m_zoneId != 0 )
	{
		if( at != NULL )
		{
			BroadcastMessage("Entered zone: %s.", at->name);
		}
	}

	if( !m_channels.empty() && at )
	{
#ifdef CLUSTERING
		std::set<uint32>::iterator i;
		std::vector<uint32> channels;
		uint32 cid;

		for(i = m_channels.begin(); i != m_channels.end();)
		{
			cid = *i;
			++i;
			channels.push_back(cid);
		}
		WorldPacket data(ICMSG_CHANNEL_UPDATE, 4 + (sizeof(std::vector<uint32>::size_type) * channels.size()) + 4 + 4 + 4);
		data << uint8(UPDATE_CHANNELS_ON_ZONE_CHANGE); //update channels on zone change
		data << GetLowGUID();
		data << channels;
		data << GetAreaID();
		data << GetZoneId();
		data << GetMapId();
		sClusterInterface.SendPacket(&data);
#else
		// change to zone name, not area name
		for(std::set<Channel*>::iterator itr = m_channels.begin(), nextitr ; itr != m_channels.end() ; itr = nextitr)
		{
			nextitr = itr;
			++nextitr;
			Channel* chn;
			chn = (*itr);
			// Check if this is a custom channel (i.e. global)
			if(!((*itr)->m_flags & 0x10))
				continue;

			if(chn->m_flags & 0x40)   // LookingForGroup - constant among all zones
				continue;

			char updatedName[95];
			ChatChannelDBC* pDBC;
			pDBC = dbcChatChannels.LookupEntryForced(chn->m_id);
			if(!pDBC)
			{
				Log.Error("ChannelMgr" , "Invalid channel entry %u for %s" , chn->m_id , chn->m_name.c_str());
				return;
			}
			//for( int i = 0 ; i <= 15 ; i ++ )
			//	Log.Notice( "asfssdf" , "%u %s" , i , pDBC->name_pattern[i] );
			snprintf(updatedName , 95 , pDBC->pattern , at->name);
			Channel* newChannel = channelmgr.GetCreateChannel(updatedName , NULL , chn->m_id);
			if(newChannel == NULL)
			{
				Log.Error("ChannelMgr" , "Could not create channel %s!" , updatedName);
				return; // whoops?
			}
			//Log.Notice( "ChannelMgr" , "LEAVING CHANNEL %s" , chn->m_name.c_str() );
			//Log.Notice( "ChannelMgr" , "JOINING CHANNEL %s" , newChannel->m_name.c_str() );
			if(chn != newChannel)   // perhaps there's no need
			{
				// join new channel
				newChannel->AttemptJoin(this , "");
				// leave the old channel

				chn->Part(this);
			}
		}
#endif
	/*std::map<uint32, AreaTable*>::iterator iter = sWorld.mZoneIDToTable.find(ZoneId);
	if(iter == sWorld.mZoneIDToTable.end())
		return;

	AreaTable *p = iter->second;
	if(p->AreaId != m_AreaID)
	{
		m_AreaID = p->AreaId;
		UpdatePVPStatus(m_AreaID);
	}

	sLog.outDetail("ZONE_UPDATE: Player %s entered zone %s", GetName(), sAreaStore.LookupString((int)p->name));*/

	//GetMapMgr()->SendInitialStates(this);
	if( IsInWorld() )				// should be
		m_mapMgr->GetDBStateManager().SendWorldStates(this);	
		//this->GetMapMgr()->GetDBStateManager().SendWorldStates(this);

	}
}

void Player::SendTradeUpdate()
{
	Player * pTarget = GetTradeTarget();
	if(!pTarget)
		return;

	WorldPacket data(SMSG_TRADE_STATUS_EXTENDED, 100);
	data << uint8(1);
	data << uint32(0x19);
	data << m_tradeSequence;
	data << m_tradeSequence++;
	data << mTradeGold << uint32(0);
		
	// Items
	for(uint32 Index = 0; Index < 7; ++Index)
	{
		data << uint8(Index);
		Item *pItem = mTradeItems[Index];
		if(pItem != 0)
		{
			ItemPrototype * pProto = pItem->GetProto();
			//ASSERT(pProto != 0);
            if(!pProto)
	        {
			 Log.Error("Player::SendTradeUpdate","ItemProto (player %s) non trouvé, slot %u!! (Item %u)",this->GetName(),pItem->GetEntry(),Index);
		     continue;
	        }

			data << pProto->ItemId;
			data << pProto->DisplayInfoID;
			data << pItem->GetUInt32Value(ITEM_FIELD_STACK_COUNT);  // Amount		   OK
			
			// Enchantment stuff
			data << uint32(0);									  // unknown
			data << pItem->GetUInt64Value(ITEM_FIELD_GIFTCREATOR);  // gift creator	 OK
			data << pItem->GetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1);  // Item Enchantment OK
			data << uint32(0);									  // unknown
			data << uint32(0);									  // unknown
			data << uint32(0);									  // unknown
			data << pItem->GetUInt64Value(ITEM_FIELD_CREATOR);	  // item creator	 OK
			data << pItem->GetUInt32Value(ITEM_FIELD_STACK_COUNT);  // Spell Charges	OK

			data << uint32(0);									  // seems like time stamp or something like that
			data << pItem->GetUInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID);
			data << pProto->LockId;								 // lock ID		  OK
			data << pItem->GetUInt32Value(ITEM_FIELD_MAXDURABILITY);
			data << pItem->GetUInt32Value(ITEM_FIELD_DURABILITY);
		}
		else
		{
			for(uint8 j = 0; j < 18; j++)
				data << uint32(0);
		}
	}

	pTarget->GetSession()->SendPacket(&data);
}

void Player::RequestDuel(Player *pTarget)
{
	// We Already Dueling or have already Requested a Duel

	if( DuelingWith != NULL )
		return;

	if( m_duelState != DUEL_STATE_FINISHED )
		return;

	SetDuelState( DUEL_STATE_REQUESTED );

	//Setup Duel
	pTarget->DuelingWith = this;
	DuelingWith = pTarget;

	//Get Flags position
	float dist = CalcDistance(pTarget);
	dist = dist * 0.5f; //half way
	float x = (GetPositionX() + pTarget->GetPositionX()*dist)/(1+dist) + cos(GetOrientation()+(float(M_PI)/2))*2;
	float y = (GetPositionY() + pTarget->GetPositionY()*dist)/(1+dist) + sin(GetOrientation()+(float(M_PI)/2))*2;
	float z = (GetPositionZ() + pTarget->GetPositionZ()*dist)/(1+dist);

	//Create flag/arbiter
	GameObject* pGameObj = GetMapMgr()->CreateGameObject(21680);
	if( pGameObj == NULL || !pGameObj->CreateFromProto(21680,GetMapId(), x, y, z, GetOrientation()))
		return;
	pGameObj->SetInstanceID(GetInstanceID());

	//Spawn the Flag
	pGameObj->SetUInt64Value(OBJECT_FIELD_CREATED_BY, GetGUID());
	pGameObj->SetUInt32Value(GAMEOBJECT_FACTION, GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	pGameObj->SetUInt32Value(GAMEOBJECT_LEVEL, GetUInt32Value(UNIT_FIELD_LEVEL));

	//Assign the Flag 
	SetUInt64Value(PLAYER_DUEL_ARBITER,pGameObj->GetGUID());
	pTarget->SetUInt64Value(PLAYER_DUEL_ARBITER,pGameObj->GetGUID());

	pGameObj->PushToWorld(m_mapMgr);

	WorldPacket data(SMSG_DUEL_REQUESTED, 16);
	data << pGameObj->GetGUID();
	data << GetGUID();
	pTarget->GetSession()->SendPacket(&data);
}

void Player::DuelCountdown()
{
	if( DuelingWith == NULL )
		return;

	m_duelCountdownTimer -= 1000;

	if( m_duelCountdownTimer < 0 )
		m_duelCountdownTimer = 0;

	if( m_duelCountdownTimer == 0 )
	{
		// Start Duel.
		SetRage(0);
		DuelingWith->SetRage(0);

		//Give the players a Team
		DuelingWith->SetUInt32Value( PLAYER_DUEL_TEAM, 1 ); // Duel Requester
		SetUInt32Value( PLAYER_DUEL_TEAM, 2 );

		SetDuelState( DUEL_STATE_STARTED );
		DuelingWith->SetDuelState( DUEL_STATE_STARTED );

		sEventMgr.AddEvent( this, &Player::DuelBoundaryTest, EVENT_PLAYER_DUEL_BOUNDARY_CHECK, 500, 0, 0 );
		sEventMgr.AddEvent( DuelingWith, &Player::DuelBoundaryTest, EVENT_PLAYER_DUEL_BOUNDARY_CHECK, 500, 0, 0 );
	}
}

void Player::DuelBoundaryTest()
{
	//check if in bounds
	if(!IsInWorld())
		return;

	GameObject * pGameObject = GetMapMgr()->GetGameObject(GetUInt32Value(PLAYER_DUEL_ARBITER));
	if(!pGameObject)
	{
		EndDuel(DUEL_WINNER_RETREAT);
		return;
	}

	float Dist = CalcDistance((Object*)pGameObject);

	if(Dist > 75.0f)
	{
		// Out of bounds
		if(m_duelStatus == DUEL_STATUS_OUTOFBOUNDS)
		{
			// we already know, decrease timer by 500
			m_duelCountdownTimer -= 500;
			if(m_duelCountdownTimer == 0)
			{
				// Times up :p
				DuelingWith->EndDuel(DUEL_WINNER_RETREAT);
			}
		}
		else
		{
			// we just went out of bounds
			// set timer
			m_duelCountdownTimer = 10000;

			// let us know
#ifdef USING_BIG_ENDIAN
			uint32 swapped = swap32(m_duelCountdownTimer);
			m_session->OutPacket(SMSG_DUEL_OUTOFBOUNDS, 4, &swapped);
#else
			m_session->OutPacket(SMSG_DUEL_OUTOFBOUNDS, 4, &m_duelCountdownTimer);
#endif
			m_duelStatus = DUEL_STATUS_OUTOFBOUNDS;
		}
	}
	else
	{
		// we're in range
		if(m_duelStatus == DUEL_STATUS_OUTOFBOUNDS)
		{
			// just came back in range
			m_session->OutPacket(SMSG_DUEL_INBOUNDS);
			m_duelStatus = DUEL_STATUS_INBOUNDS;
		}
	}
}

void Player::EndDuel(uint8 WinCondition)
{
	if( m_duelState == DUEL_STATE_FINISHED )
		return;

	// Remove the events
	sEventMgr.RemoveEvents( this, EVENT_PLAYER_DUEL_COUNTDOWN );
	sEventMgr.RemoveEvents( this, EVENT_PLAYER_DUEL_BOUNDARY_CHECK );

	for( uint32 x = 0; x < MAX_AURAS; ++x )
	{
		if( m_auras[x] == NULL )
			continue;
		if( m_auras[x]->WasCastInDuel() )
			m_auras[x]->Remove();
	}

	m_duelState = DUEL_STATE_FINISHED;

	if( DuelingWith == NULL )
		return;

	sEventMgr.RemoveEvents( DuelingWith, EVENT_PLAYER_DUEL_BOUNDARY_CHECK );
	sEventMgr.RemoveEvents( DuelingWith, EVENT_PLAYER_DUEL_COUNTDOWN );

	// spells waiting to hit
	sEventMgr.RemoveEvents( this, EVENT_SPELL_DAMAGE_HIT);

	for( uint32 x = 0; x < MAX_AURAS; ++x )
	{
		if( DuelingWith->m_auras[x] == NULL )
			continue;
		if( DuelingWith->m_auras[x]->WasCastInDuel() )
			DuelingWith->m_auras[x]->Remove();
	}

	DuelingWith->m_duelState = DUEL_STATE_FINISHED;

	//Announce Winner
	WorldPacket data( SMSG_DUEL_WINNER, 500 );
	data << uint8( WinCondition );
	data << GetName() << DuelingWith->GetName();
	SendMessageToSet( &data, true );

	data.Initialize( SMSG_DUEL_COMPLETE );
	data << uint8( 1 );
	SendMessageToSet( &data, true );

	//Clear Duel Related Stuff

	GameObject* arbiter = GetMapMgr() ? GetMapMgr()->GetGameObject(GetUInt32Value(PLAYER_DUEL_ARBITER)) : 0;

	if( arbiter != NULL )
		arbiter->Delete();

	SetUInt64Value( PLAYER_DUEL_ARBITER, 0 );
	DuelingWith->SetUInt64Value( PLAYER_DUEL_ARBITER, 0 );

	SetUInt32Value( PLAYER_DUEL_TEAM, 0 );
	DuelingWith->SetUInt32Value( PLAYER_DUEL_TEAM, 0 );

	EventAttackStop();
	DuelingWith->EventAttackStop();

	// Call off pet
	if( this->GetSummon() != NULL )
	{
		this->GetSummon()->CombatStatus.Vanished();
		this->GetSummon()->GetAIInterface()->SetUnitToFollow( this );
		this->GetSummon()->GetAIInterface()->HandleEvent( EVENT_FOLLOWOWNER, this->GetSummon(), 0 );
		this->GetSummon()->GetAIInterface()->WipeTargetList();
	}

	//setup immunity to duel spells!
	m_duelImmunity = true;
	sEventMgr.AddEvent(static_cast<Unit*>(this), &Unit::RemoveDuelImmunity, EVENT_UNK, 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	if (GetSummon() != NULL)
	{
		GetSummon()->m_duelImmunity = true;
		sEventMgr.AddEvent(static_cast<Unit*>(GetSummon()), &Unit::RemoveDuelImmunity, EVENT_UNK, 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	DuelingWith->m_duelImmunity = true;
	sEventMgr.AddEvent(static_cast<Unit*>(DuelingWith), &Unit::RemoveDuelImmunity, EVENT_UNK, 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	if (DuelingWith->GetSummon() != NULL)
	{
		DuelingWith->GetSummon()->m_duelImmunity = true;
		sEventMgr.AddEvent(static_cast<Unit*>(DuelingWith->GetSummon()), &Unit::RemoveDuelImmunity, EVENT_UNK, 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}

	//Stop Players attacking so they don't kill the other player
	if (GetSession() != NULL)
	GetSession()->OutPacket(SMSG_CANCEL_COMBAT);
	if (DuelingWith->GetSession() != NULL)
	DuelingWith->GetSession()->OutPacket(SMSG_CANCEL_COMBAT);

	smsg_AttackStop( DuelingWith );
	DuelingWith->smsg_AttackStop( this );

	DuelingWith->m_duelCountdownTimer = 0;
	m_duelCountdownTimer = 0;

	DuelingWith->DuelingWith = NULL;
	DuelingWith = NULL;
}

void Player::StopMirrorTimer(uint32 Type)
{
#ifdef USING_BIG_ENDIAN
	uint32 swapped = swap32(Type);
	m_session->OutPacket(SMSG_STOP_MIRROR_TIMER, 4, &swapped);
#else
	m_session->OutPacket(SMSG_STOP_MIRROR_TIMER, 4, &Type);
#endif
}

void Player::EventTeleport(uint32 mapid, float x, float y, float z)
{
	SafeTeleport(mapid, 0, LocationVector(x, y, z));
}

void Player::ApplyLevelInfo(LevelInfo* Info, uint32 Level)
{
	uint8 class_ = getClass();
	// Apply level
	SetUInt32Value(UNIT_FIELD_LEVEL, Level);

	// Set next level conditions
	SetUInt32Value(PLAYER_NEXT_LEVEL_XP, Info->XPToNextLevel);

	// Set stats
	for(uint32 i = 0; i < STAT_MAX; ++i)
	{
		BaseStats[i] = Info->Stat[i];
		CalcStat(i);
	}


	// Calculate talentpoints
	uint32 TalentPoints = 0;
	if(class_ == DEATHKNIGHT && Level == 55) TalentPoints = 0;
	else if(Level >= 10)                     TalentPoints = Level - 9;

	SetUInt32Value(PLAYER_CHARACTER_POINTS1, TalentPoints);

	//--- _/!\_ Def avant UpdateStats _/!\_
	// Set base fields
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, Info->HP);
	SetUInt32Value(UNIT_FIELD_BASE_MANA, Info->Mana);

	// Set health / mana
    SetUInt32Value(UNIT_FIELD_HEALTH, Info->HP);
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, Info->HP);
	SetUInt32Value(UNIT_FIELD_POWER1, Info->Mana);
	SetUInt32Value(UNIT_FIELD_MAXPOWER1, Info->Mana);
    //----------------------

	_UpdateMaxSkillCounts();
	UpdateStats();
	
	if(m_playerInfo !=NULL) // Creation d'un perso: m_playerInfo est NULL, Guid n'existe pas encore (Branruz)
		m_playerInfo->lastLevel = Level;
	
	GetAchievementInterface()->HandleAchievementCriteriaLevelUp( getLevel() );
	InitGlyphsForLevel();

	sLog.outDetail("Player %s set parameters to level %u", GetName(), Level);
}

void Player::BroadcastMessage(const char* Format, ...)
{
	if (GetSession() == NULL)
		return;

	va_list l;
	va_start(l, Format);
	char Message[1024];
	vsnprintf(Message, 1024, Format, l);
	va_end(l);

	WorldPacket * data = sChatHandler.FillSystemMessageData(Message);
	//m_session->SendPacket(data);
	GetSession()->SendPacket(data);
	delete data;
}
/*
const double BaseRating []= {
	2.5,//weapon_skill_ranged!!!!
	1.5,//defense=comba_r_1
	12,//dodge
	20,//parry=3
	5,//block=4
	10,//melee hit
	10,//ranged hit
	8,//spell hit=7
	14,//melee critical strike=8
	14,//ranged critical strike=9
	14,//spell critical strike=10
	0,//
	0,
	0,
	25,//resilience=14
	25,//resil .... meaning unknown
	25,//resil .... meaning unknown
	10,//MELEE_HASTE_RATING=17
	10,//RANGED_HASTE_RATING=18
	10,//spell_haste_rating = 19???
	2.5,//melee weapon skill==20
	2.5,//melee second hand=21

};
*/
float Player::CalcPercentForRating( uint32 index, uint32 rating )
{
	uint32 relative_index = index - (PLAYER_FIELD_COMBAT_RATING_1);
	/*if( relative_index <= 10 || ( relative_index >= 14 && relative_index <= 21 ) )
	{
		double rating = (double)m_uint32Values[index];
		int level = getLevel();
		if( level < 10 )//this is not dirty fix -> it is from wowwiki
			level = 10;
		double cost;
		if( level < 60 )
			cost = ( double( level ) - 8.0 ) / 52.0;
		else
			cost = 82.0 / ( 262.0 - 3.0 *  double( level ) );
		return float( rating / ( BaseRating[relative_index] * cost ) );
	}
	else
		return 0.0f;*/
	
	uint32 level = m_uint32Values[UNIT_FIELD_LEVEL];
	if( level > 100 )
		level = 100;

	if(level < 34 && (index == PLAYER_RATING_MODIFIER_BLOCK || index == PLAYER_RATING_MODIFIER_PARRY || index == PLAYER_RATING_MODIFIER_DEFENCE))
		level = 34;

	CombatRatingDBC * pDBCEntry = dbcCombatRating.LookupEntryForced( relative_index * 100 + level - 1 );
	if( pDBCEntry == NULL )
		return (float) rating;
	else
		return (rating / pDBCEntry->val);
}

bool Player::SafeTeleport(uint32 MapID, uint32 InstanceID, float X, float Y, float Z, float O)
{
	return SafeTeleport(MapID, InstanceID, LocationVector(X, Y, Z, O));
}

bool Player::SafeTeleport(uint32 MapID, uint32 InstanceID, const LocationVector & vec)
{
	//Log.Notice("[Player::SafeTeleport] en cours","%s MapId: %u, InstanceId: %u, X: %.5f, Y:%.5f ,z:%.5f",
	//	                                                this->GetName(),MapID,InstanceID,vec.x,vec.y,vec.z);  // Debug (Branruz)
	
	bool instance = false;
	MapInfo * mi = WorldMapInfoStorage.LookupEntry(MapID);

	if(!mi)
	{
		Log.Error("SafeTeleport","(%s) La map n'existe pas dans WorldMap_Info <- Check your DB! (Id: %u)",this->GetName(),MapID);
		return(false);
	}

	if(InstanceID && (uint32)m_instanceId != InstanceID)
	{
		instance = true;
#ifndef CLUSTERING		
		this->SetInstanceID(InstanceID);
#endif
#ifndef COLLISION
		// if we are mounted remove it
		if( m_MountSpellId )
			RemoveAura( m_MountSpellId );
#endif
	}
	else if(m_mapId != MapID)
	{
		instance = true;
#ifndef COLLISION
		// if we are mounted remove it
		if( m_MountSpellId )
			RemoveAura( m_MountSpellId );
#endif
	}

	// make sure player does not drown when teleporting from under water
	if (m_UnderwaterState & UNDERWATERSTATE_UNDERWATER)
		m_UnderwaterState &= ~UNDERWATERSTATE_UNDERWATER;

	/*if(flying_aura && MapID != 530)
	{
		RemoveAura(flying_aura);
		flying_aura = 0;
	}*/

	// Lookup map info
	if(mi && mi->flags & WMI_INSTANCE_XPACK_01 && !m_session->HasFlag( ACCOUNT_FLAG_XPACK_01 | ACCOUNT_FLAG_XPACK_02) )
	{
		WorldPacket msg(SMSG_MOTD, 50);
		msg << uint32(3) << "Vous devez avoir l'extension Burning Crusade pour acceder a cette partie." << uint8(0);
		m_session->SendPacket(&msg);
		return false;
	}

	if(mi && mi->flags & WMI_INSTANCE_XPACK_02 && !m_session->HasFlag(ACCOUNT_FLAG_XPACK_02))
	{
		WorldPacket msg(SMSG_MOTD, 50);
		msg << uint32(3) << "Vous devez avoir l'extension Wrath of the Lich King pour acceder a cette partie." << uint8(0);
		m_session->SendPacket(&msg);
		return false;
	}
	//--- Branruz
	// Si le familier est spawn, on le despawn avant le teleport du player
	Pet *pPet = this->GetSummon();
	if(pPet) pPet->Remove(false, true, true); // no safedelete
	//--------
#ifndef CLUSTERING	
	_Relocate(MapID, vec, true, instance, InstanceID);
#else
	if(instance)
	{
		WorldPacket data(SMSG_TRANSFER_PENDING, 4);
		data << uint32(MapID);
		GetSession()->SendPacket(&data);
		sClusterInterface.RequestTransfer(this, MapID, InstanceID, vec);
		return true;
	}

	m_sentTeleportPosition = vec;
	SetPosition(vec);
	ResetHeartbeatCoords();

	WorldPacket * data = BuildTeleportAckMsg(vec);
	m_session->SendPacket(data);
	delete data;
#endif
	//Log.Notice("[Player::SafeTeleport] Ok","%s",this->GetName()); // Debug (Branruz)
	return true;
}

void Player::SafeTeleport(MapMgr * mgr, const LocationVector & vec)
{

	//Log.Notice("[Player::SafeTeleport] en cours","%s MapId: %u, X: %.5f, Y:%.5f ,z:%.5f",
	//	                                     this->GetName(),mgr->GetMapId(),vec.x,vec.y,vec.z);  // Debug (Branruz)
                                       // BC                     Northrend
	if(flying_aura && (mgr->GetMapId()!=530) && (mgr->GetMapId()!=571)) 
	{
		RemoveAura(flying_aura);
		flying_aura=0;
	}

	if(IsInWorld())
		RemoveFromWorld();

	m_mapId = mgr->GetMapId();
	m_instanceId = mgr->GetInstanceID();
	WorldPacket data(SMSG_TRANSFER_PENDING, 20);
	data << mgr->GetMapId();
	GetSession()->SendPacket(&data);

	data.Initialize(SMSG_NEW_WORLD);
	data << mgr->GetMapId() << vec << vec.o;
	GetSession()->SendPacket(&data);

	SetPlayerStatus(TRANSFER_PENDING);
	m_sentTeleportPosition = vec;
	SetPosition(vec);
	ResetHeartbeatCoords();
	//Log.Notice("[Player::SafeTeleport] Ok","%s",this->GetName());  // Debug (Branruz)
}

void Player::SetGuildId(uint32 guildId)
{
	if(IsInWorld())
	{
		const uint32 field = PLAYER_GUILDID;
		sEventMgr.AddEvent(((Object*)this), &Object::EventSetUInt32Value, field, guildId, EVENT_PLAYER_SEND_PACKET, 1,
			1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	else
	{
		SetUInt32Value(PLAYER_GUILDID,guildId);
	}
}

void Player::SetGuildRank(uint32 guildRank)
{
	if(IsInWorld())
	{
		const uint32 field = PLAYER_GUILDRANK;
		sEventMgr.AddEvent(((Object*)this), &Object::EventSetUInt32Value, field, guildRank, EVENT_PLAYER_SEND_PACKET, 1,
			1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	else
	{
		SetUInt32Value(PLAYER_GUILDRANK,guildRank);
	}
}

void Player::UpdatePvPArea()
{
	/*
	Category: (Num)
	AREAC_CONTESTED          = 0,
    AREAC_ALLIANCE_TERRITORY = 2,
    AREAC_HORDE_TERRITORY    = 4,
    AREAC_SANCTUARY          = 6,
    Flag: (Bitmap)
	AREA_00000001           = 0x00000001, // ?? Dun Morogh / The Storm Peaks (Branruz)
    AREA_CITY_CAPITAL       = 0x00000008, // Ville/Capitale (Branruz)
	AREA_CITY_CAPITAL2      = 0x00000010, // Ville/Capitale (Branruz)
	AREA_CITY_AREA          = 0x00000020, // Ok proche des capitales seulement (Branruz) 
    AREA_NEUTRAL_AREA       = 0x00000040, // Ok (Branruz)
    AREA_PVP_ARENA          = 0x00000080, // Ok (Branruz)
	AREA_CITY_CAPITAL3      = 0x00000100, // Ville/Capitale (Branruz)
//	AREA_CITY               = 0x00000200, // (Branruz) ,
	AREA_0x00000400         = 0x00000400, // ?? Programmer Isle / Dragonblight (Branruz)
    AREA_SANCTUARY          = 0x00000800, // Shattrath / Dalaran
    AREA_ISLAND             = 0x00001000, // pas verif
	AREA_ALLOW_FLYING		= 0x00004000, // NOk, a Voir
    AREA_PVP_OBJECTIVE_AREA = 0x00008000, // Ok , genre "Tour a prendre" (Branruz)
	AREA_FFA                = 0x00010000, // ?? Blade's Edge Arena (Branruz)
	AREA_CITY               = 0x00200000, // Ville/Capitale/Village (Branruz) 
	AREA_CITY_ARENA         = 0x04000000, // ?? Undercity / Dalaran Arena (Branruz)
	*/
	//Log.Notice("[UpdatePvPArea]","(%s) AreaId   : %u",this->GetName(),m_AreaID);
   
	AreaTable *at,*at2; 
		
	at = dbcArea.LookupEntry(m_AreaID);
	if(at == 0) 
	{
		Log.Error("[UpdatePvPArea]","(%s) AreaId (%u) n'existe pas!",this->GetName(),m_AreaID);
		return;
	}

	// Debug (Branruz)
	//Log.Notice("[UpdatePvPArea]","(%s - %u) Area: %s",this->GetName(),this->GetTeam(),at->name);
	switch(at->category)
	{
	case AREAC_CONTESTED: 
	case AREAC_ALLIANCE_TERRITORY: 
    case AREAC_HORDE_TERRITORY:
    case AREAC_SANCTUARY: break;
	default: Log.Error("[UpdatePvPArea]","(%s) Category: %04X Area: %s Id: %u <-- Report this to dev",
				                                  this->GetName(),at->category,at->AreaId,at->name);
		return;
	}

	// ***** Verif Prioritaire ***********
    // Shattrath / Dalaran : Mode JcJ Off - PvP/PvE, pas de timer
	if(at->AreaFlags & AREA_SANCTUARY) 
	{
		if(IsPvPFlagged())
	    {
		 //Log.Notice("[UpdatePvPArea]","Zone: %s - %04X",at->name,at->AreaFlags);	 
		 //Log.Notice("[UpdatePvPArea]","(%s) Mode PvP Off, Sanctuaire",this->GetName());
         StopPvPTimer();
         RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE); // Menu -> "Joueur contre Joueur" -> "Deactiver"
         RemoveFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TIMER);
		 RemovePvPFlag(); // Icon sur le joueur
		}

		if(IsPvPFlaggedFFA()) // Normalement ne peut pas arriver sauf sur un Tp
		{
         RemovePvPFlagFFA();
		}
		return;
	}
	else if(at->AreaFlags & AREA_PVP_ARENA) // FFA
	{
        if(!IsPvPFlagged() || !HasFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TOGGLE) ) // sur un Tp
	    {
          SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE); // Menu -> "Joueur contre Joueur" -> "Activer"
	      RemoveFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TIMER);
	      StopPvPTimer();
	      SetPvPFlag();
		  //Log.Notice("[UpdatePvPArea]","Zone: %s - %04X",at->name,at->AreaFlags);
	      //Log.Notice("[UpdatePvPArea]","(%s) Arene, Mode PvP On",this->GetName());
	    }

		if(!IsPvPFlaggedFFA()) // Normalement ne peut pas arriver
		{
         SetPvPFlagFFA();
		}

		return;
	}
	else if(at->AreaFlags &  AREA_PVP_OBJECTIVE_AREA) // Tour a prendre / Zone JcJ
	{                               // Timer en cours ??
       if(!IsPvPFlagged() || !HasFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TOGGLE) ) // sur un Tp
	    {
          SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE); // Menu -> "Joueur contre Joueur" -> "Activer"
	      RemoveFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TIMER);
	      StopPvPTimer();
	      SetPvPFlag();
		  //Log.Notice("[UpdatePvPArea]","Zone: %s - %04X",at->name,at->AreaFlags);
	      //Log.Notice("[UpdatePvPArea]","(%s) Zone JcJ, Mode PvP On",this->GetName());
	    }

		if(IsPvPFlaggedFFA()) // Normalement ne peut pas arriver sauf sur un Tp
		{
         RemovePvPFlagFFA();
		}
		return;
	}

    // ***** Verif Secondaire ***********
	if(at->ZoneId) // ZoneId rattachée ?
	{
	 at2 = dbcArea.LookupEntry(at->ZoneId);
	 if(at2 == 0) 
	 {
	 	Log.Error("[UpdatePvPArea]","(%s) AreaId (%u) n'existe pas! <-- Report this to developper",this->GetName(),at->ZoneId);
		return;
	 }
	 at = at2;
	}

    if((at->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == FACTION_ALLY) || 
	   (at->category == AREAC_HORDE_TERRITORY    && GetTeam() == FACTION_HORDE)) 
	{
		// Territoire Allié, on laisse comme c'est, chacun fait ce qu'il veut
		// quelque soit le type de serveur PvP/PvE
		//Log.Notice("[UpdatePvPArea]","Zone: %s - %04X",at->name,at->AreaFlags);
        //Log.Notice("[UpdatePvPArea]","(%s) Zone Ami, Mode PvP reste comme il est",this->GetName());
		if(IsPvPFlaggedFFA()) // Normalement ne peut pas arriver sauf sur un Tp
		{
         RemovePvPFlagFFA();
		}
		return;
	}

	if((at->category == AREAC_ALLIANCE_TERRITORY && GetTeam() == FACTION_HORDE) || 
	   (at->category == AREAC_HORDE_TERRITORY    && GetTeam() == FACTION_ALLY)) 
     {  // JcJ Auto Global en territoire ennemi, Serveur PvP
       if(sWorld.GetRealmType() == REALM_PVP) 
	   {                            // Timer On ??
         if(!IsPvPFlagged() || !HasFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TOGGLE) )
	     {
          SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE); // Menu -> "Joueur contre Joueur" -> "Activer"
	      RemoveFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TIMER);
	      StopPvPTimer();
	      SetPvPFlag();
		  //Log.Notice("[UpdatePvPArea]","Zone: %s - %04X",at->name,at->AreaFlags);
	      //Log.Notice("[UpdatePvPArea]","(%s) Territoire Enemi, Mode PvP On",this->GetName());
	     }

		 if(IsPvPFlaggedFFA()) // Normalement ne peut pas arriver sauf sur un Tp
		 {
          RemovePvPFlagFFA();
		 }
		 return;
	   }     // JcJ Auto seulement dans les capitales pour le PvE
	   else if((at->AreaFlags & AREA_CITY_AREA)) // || (at->AreaFlags & AREA_CITY)) 
       {                                              // Timer On 
         if(!IsPvPFlagged() || !HasFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TOGGLE) )
	     {
          SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE); // Menu -> "Joueur contre Joueur" -> "Activer"
	      RemoveFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TIMER);
	      StopPvPTimer();
	      SetPvPFlag();
		  //Log.Notice("[UpdatePvPArea]","Zone: %s - %04X",at->name,at->AreaFlags);
	      //Log.Notice("[UpdatePvPArea]","(%s) Ville Enemie, Mode PvP On",this->GetName());
	     }

		 if(IsPvPFlaggedFFA()) // Normalement ne peut pas arriver sauf sur un Tp
		 {
          RemovePvPFlagFFA();
		 }

		 return;
	   }

	 }
	else if(sWorld.GetRealmType() == REALM_PVP)
	{
		if(at->category == AREAC_CONTESTED)
		{                                              // Timer On 
         if(!IsPvPFlagged() || !HasFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TOGGLE) )
	     {
          SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE); // Menu -> "Joueur contre Joueur" -> "Activer"
	      RemoveFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TIMER);
	      StopPvPTimer();
	      SetPvPFlag();
		  //Log.Notice("[UpdatePvPArea]","Zone: %s - %04X",at->name,at->AreaFlags);
	      //Log.Notice("[UpdatePvPArea]","(%s) Territoire Consteste, Mode PvP On",this->GetName());
		 }
		}
 
		if(IsPvPFlaggedFFA()) // Normalement ne peut pas arriver sauf sur un Tp
		{
         RemovePvPFlagFFA();
		}

		return;
	}

}

void Player::BuildFlagUpdateForNonGroupSet(uint32 index, uint32 flag)
{
    Object *curObj;
    for (Object::InRangeSet::iterator iter = GetInRangeSetBegin(); iter != GetInRangeSetEnd();)
	{
		curObj = *iter;
		iter++;
        if(curObj->IsPlayer())
        {
            Group* pGroup = static_cast< Player* >( curObj )->GetGroup();
            if( pGroup != NULL && pGroup == GetGroup())
            {
				BuildFieldUpdatePacket( static_cast< Player* >( curObj ), index, flag );
            }
        }
    }
}

void Player::LoginPvPSetup()
{
	// Make sure we know our area ID.
	//Log.Notice("[LoginPvPSetup]","(%s) Refresh position",this->GetName());
	_EventExploration();
	OnPositionChange();

    /*if(isAlive())
        CastSpell(this, PLAYER_HONORLESS_TARGET_SPELL, true); ( code incomplet ( Crash ) )*/

	AreaTable *at,*at2;

	at = dbcArea.LookupEntry(m_AreaID);
	if(at == 0)
	{
    Log.Error("[LoginPvPSetup]","(%s) AreaId %u non trouvé! <- Report this to developper",this->GetName(),m_AreaID);
    return;
	}

	if(at->ZoneId)
	{
		at2 = dbcArea.LookupEntry(at->ZoneId);
		if(at2 == 0)
		{
			Log.Error("[LoginPvPSetup]","(%s) AreaId %u non trouvé! <- Report this to developper",this->GetName(),at->ZoneId);
			return;
		}
		
		at = at2 ;
	}
	
	if ( at != NULL && isAlive() && ( at->category == AREAC_CONTESTED || ( GetTeam() == FACTION_ALLY && at->category == AREAC_HORDE_TERRITORY ) || ( GetTeam() == FACTION_HORDE && at->category == AREAC_ALLIANCE_TERRITORY ) ) )
		CastSpell(this, PLAYER_HONORLESS_TARGET_SPELL, true);
}

void Player::PvPToggle()
{
	// Le systeme de reconnaissance doit fonctionner comme ca: (Reconnaissance de Zone et PvP/PvE)
	// Le joueur est identifié dans une AreaId, le type de territoire ou il se trouve
	// est rataché au champ ZoneId dont l'AreaId depend,et seulement si ZoneId est different de 0
	// Si ZoneId est = 0 , le type de territoire de l'AreaId  est le bon (propriete: AreaTable.Category)
	// Les AraId qui dependent d'une zoneId ont le champ Category = 0, 
	// donc pris comme "Territoire Contestés", 
	// ZoneId doit devenir dans ce cas le nouvel AreaId pour avoir le bon type de territoire (Branruz)

   AreaTable *at,*at2;

   at = dbcArea.LookupEntry(m_AreaID); // Ou se trouve le player ??
   if(at == 0)
   {
    Log.Error("[PvPToggle]","(%s) AreaId %u non trouvé! <- Report this to developper",this->GetName(),m_AreaID);
    return;
   } 

   // Recherche de la zone Id ratachée
   if(at->ZoneId) // F*ck Blizz, pourquoi faire simple qd on peut faire compliquer
   {
    at2 = dbcArea.LookupEntry(at->ZoneId);
	if(at2 == 0)
    {
     Log.Error("[PvPToggle]","(%s) AreaId %u non trouvé! <- Report this to developper",this->GetName(),at->ZoneId);
     return;
    }

	at = at2 ; // AreaId = ZoneId
   }

   if(!HasFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE)) // Met le Flag
   {
    if(at->category != AREAC_SANCTUARY) // Hors Sanctuaire
    {
	 //Log.Notice("[PvPToggle]","(%s) Mode PvP On",this->GetName());
	 SetFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE); // Menu -> "Joueur contre Joueur" -> "Activer"
	 RemoveFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TIMER);
	 StopPvPTimer();
	 SetPvPFlag();
    }
    else
    {
	   //Log.Notice("[PvPToggle]","(%s) Mode PvP reste Off, Sanctuaire",this->GetName());
	   sChatHandler.GreenSystemMessageToPlr(this,"Vous ne pouvez pas mettre votre mode JcJ dans un sanctuaire");
    }
   }
   else // Veut enlever son Flag
   {
    switch(at->category)
    {
     case AREAC_CONTESTED :	if(sWorld.GetRealmType() == REALM_PVP) 
							{
							 //Log.Notice("[PvPToggle]","(%s) Mode PvP reste On, Territoire conteste",this->GetName());
			                 //Log.Notice("[PvPToggle]","PvP: AreaId %u , %s",at->AreaId, at->name);
			                 sChatHandler.GreenSystemMessageToPlr(this,"Vous ne pouvez pas enlever votre Mode JcJ en territoire consteste.");	
							 return;
							}
							break;

	 case AREAC_ALLIANCE_TERRITORY :
		                    if(this->GetTeam() == FACTION_HORDE)
							{
                             // On ne peut pas enlever son JcJ dans une capitale ennemy - Commun PvP/PvE
	                         if(at->AreaFlags & AREA_CITY_AREA)
							 {
							  //Log.Notice("[PvPToggle]","(%s) Mode PvP reste On, Capitale ennemi",this->GetName());
							  //Log.Notice("[PvPToggle]","PvP: AreaId %u (%u) , %s",at->AreaId,this->m_AreaID, at->name);
			                  sChatHandler.GreenSystemMessageToPlr(this,"Vous ne pouvez pas enlever votre Mode JcJ dans une capitale ennemi.");	
							  return;
							 }
							 else if(sWorld.GetRealmType() == REALM_PVP) 
							 {
							  //Log.Notice("[PvPToggle]","(%s) Mode PvP reste On, Territoire ennemi",this->GetName());
			                  //Log.Notice("[PvPToggle]","PvP: AreaId %u , %s",at->AreaId, at->name);
			                  sChatHandler.GreenSystemMessageToPlr(this,"Vous ne pouvez pas enlever votre Mode JcJ en territoire ennemi.");	
							  return;
							 }
							} 
                            break;

	 case AREAC_HORDE_TERRITORY :
		                    if(this->GetTeam() == FACTION_ALLY)
							{
                             // On ne peut pas enlever son JcJ dans une capitale ennemy - Commun PvP/PvE
	                         if(at->AreaFlags & AREA_CITY_AREA)
							 {
							  //Log.Notice("[PvPToggle]","(%s) Mode PvP reste On, Capitale ennemi",this->GetName());
			                  //Log.Notice("[PvPToggle]","PvP: AreaId %u , %s",at->AreaId, at->name);
			                  sChatHandler.GreenSystemMessageToPlr(this,"Vous ne pouvez pas enlever votre Mode JcJ dans une capitale ennemi.");	
							  return;
							 }
							 else if(sWorld.GetRealmType() == REALM_PVP) 
							 {
							  //Log.Notice("[PvPToggle]","(%s) Mode PvP reste On, Territoire ennemi",this->GetName());
			                  //Log.Notice("[PvPToggle]","PvP: AreaId %u , %s",at->AreaId, at->name);
			                  sChatHandler.GreenSystemMessageToPlr(this,"Vous ne pouvez pas enlever votre Mode JcJ en territoire ennemi.");	
							  return;
							 }
							}
                            break;

	 case AREAC_SANCTUARY : // Normalement l'UpdatePvPArea() l'a viré
          return;

	 default:
		  Log.Error("[PvPToggle]","(%s) AreaId %u, Category %u non trouvé! <- Report this to developper",
			  this->GetName(),at->AreaId,at->category);
          return;
    }

	//Log.Notice("[PvPToggle]","(%s) Mode PvP Off (5 min)",this->GetName());
    ResetPvPTimer();
    RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_PVP_TOGGLE); // Menu -> "Joueur contre Joueur" -> "Deactiver"
    SetFlag(PLAYER_FLAGS,PLAYER_FLAG_PVP_TIMER);

	if(IsPvPFlaggedFFA()) // Au cas ou
    {
     RemovePvPFlagFFA();
    }

   }
}

void Player::ResetPvPTimer()
{
	 m_pvpTimer = sWorld.getIntRate(INTRATE_PVPTIMER);
}

void Player::CalculateBaseStats()
{
	if(!lvlinfo) return;

	memcpy(BaseStats, lvlinfo->Stat, sizeof(uint32) * 5);

	LevelInfo * levelone = objmgr.GetLevelInfo(this->getRace(),this->getClass(),1);
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, lvlinfo->HP);
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, lvlinfo->HP - (lvlinfo->Stat[2]-levelone->Stat[2])*10);
	SetUInt32Value(PLAYER_NEXT_LEVEL_XP, lvlinfo->XPToNextLevel);
	
	
	if(GetPowerType() == POWER_TYPE_MANA)
	{
		SetUInt32Value(UNIT_FIELD_BASE_MANA, lvlinfo->Mana - (lvlinfo->Stat[3]-levelone->Stat[3])*15);
		SetUInt32Value(UNIT_FIELD_MAXPOWER1, lvlinfo->Mana);
	}
}

void Player::CompleteLoading()
{	
	// cast passive initial spells	  -- grep note: these shouldnt require plyr to be in world
	SpellSet::iterator itr;
	SpellEntry *info;
	SpellCastTargets targets;
	targets.m_target = this;
	targets.m_targetMask = TARGET_FLAG_UNIT; // 0x2;

	for (itr = mSpells.begin(); itr != mSpells.end(); ++itr)
	{
		info = dbcSpell.LookupEntry(*itr);

		if(	info  
			&& (info->attributes & ATTRIBUTES_PASSIVE)  // passive
			&& !( info->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET ) //on pet summon talents
			 ) 
		{
			if( info->shapeshiftMask )
			{
				if( !( ((uint32)1 << (GetShapeShift()-1)) & info->shapeshiftMask ) )
					continue;
			}

			Spell* spell = NULL;
			spell=new Spell(this,info,true,NULL);
			spell->prepare(&targets);
		}
	}

	ApplySpec(m_talentActiveSpec, true);

	if(!isDead())//only add aura's to the living (death aura set elsewhere)
	{
		std::list<LoginAura>::iterator i,i2;
		for(i =  loginauras.begin();i!=loginauras.end();)
		{
			i2 = i++;

			// this stuff REALLY needs to be fixed - Burlex
			SpellEntry * sp = dbcSpell.LookupEntry((*i2).id);
			SpellDuration* sd = dbcSpellDuration.LookupEntry(sp->durationIndex);

			//do not load auras that only exist while pet exist. We should recast these when pet is created anyway
			if ( sp->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET )
				continue;

			Aura* a = NULL;
			a = new Aura(sp, GetDuration(sd,INVOC_DURATION), INVOC_DURATION, (Object *)this,(Unit *)this);
			for(uint32 x =0;x<3;x++)
			{
				if(sp->Effect[x]==SPELL_EFFECT_APPLY_AURA)
					a->AddMod(sp->EffectApplyAuraName[x],sp->EffectBasePoints[x]+1,sp->EffectMiscValue[x],x);
			}
			AddAura(a, NULL);
			loginauras.erase(i2);
		}
		loginauras.clear();

	 // warrior has to have battle stance
	 if( getClass() == WARRIOR )
	 {
		// battle stance passive // Mode "Combat" (Branruz)
		CastSpell(this, dbcSpell.LookupEntry(__Battle_Stance ), true); // 2457
	 }
	} // End if(!isDead())

    /* 
	// this needs to be after the cast of passive spells, because it will cast ghost form, after the remove making it in ghost alive, if no corpse.
	//death system checkout
	if(GetUInt32Value(UNIT_FIELD_HEALTH) <= 0 && !HasFlag(PLAYER_FLAGS, PLAYER_FLAG_DEATH_WORLD_ENABLE))
	{
		setDeathState(CORPSE);
	}
	else if(HasFlag(PLAYER_FLAGS, PLAYER_FLAG_DEATH_WORLD_ENABLE))
	{
		// Check if we have an existing corpse.
		Corpse * corpse = objmgr.GetCorpseByOwner(GetLowGUID());
		if(corpse == 0)
		{
			sEventMgr.AddEvent(this, &Player::RepopAtGraveyard, GetPositionX(),GetPositionY(),GetPositionZ(), GetMapId(), EVENT_PLAYER_CHECKFORCHEATS, 1000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
		else
		{
			// Set proper deathstate
			setDeathState(CORPSE);
		}
	}
	else if(getDeathState() == JUST_DIED && !HasActiveAura(__Ghost)) // 8326
	{
		//RepopRequestedPlayer();
		sEventMgr.AddEvent(this, &Player::RepopRequestedPlayer, EVENT_PLAYER_CHECKFORCHEATS, 1000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	} */

	if(iActivePet)
		SpawnPet(iActivePet);	   // only spawn if >0


	// useless logon spell
	/*
	SpellEntry* sp=dbcSpell.LookupEntry(__LOGINEFFECT); // 836
	Spell *logonspell = new Spell(this, sp, false, NULL);
	logonspell->prepare(&targets);
	*/

	// Banned
	if(IsBanned())
	{
		Kick(10000);
		BroadcastMessage("Ce personnage n'est pas autorise a jouer.");
		BroadcastMessage("Vous avez ete banni, Raison: %s", GetBanReason().c_str());
	}

	if(m_playerInfo->m_Group)
	{
		sEventMgr.AddEvent(this, &Player::EventGroupFullUpdate, EVENT_UNK, 100, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		//m_playerInfo->m_Group->Update();
	}

	if(raidgrouponlysent)
	{
		WorldPacket data2(SMSG_RAID_GROUP_ONLY, 8);
		data2 << uint32(0xFFFFFFFF) << uint32(0);
		GetSession()->SendPacket(&data2);
		raidgrouponlysent=false;
	}

	sInstanceMgr.BuildSavedInstancesForPlayer(this);
}

void Player::OnWorldPortAck()
{
	//only rezz if player is porting to a instance portal
	MapInfo *pMapinfo = WorldMapInfoStorage.LookupEntry(GetMapId());
	if(isDead())
	{
		if(pMapinfo)
		{
			if(pMapinfo->type != INSTANCE_NULL)
				ResurrectPlayer(NULL);
		}
	}

	if(pMapinfo)
	{
		WorldPacket data(4);
		if(pMapinfo->HasFlag(WMI_INSTANCE_WELCOME) && GetMapMgr())
		{
			std::string welcome_msg;
			welcome_msg = "Vous etes dans ";
			welcome_msg += pMapinfo->name;
			welcome_msg += ". ";
            if(pMapinfo->type != INSTANCE_NONRAID && m_mapMgr->pInstance)
			{
				/*welcome_msg += "This instance is scheduled to reset on ";
				welcome_msg += asctime(localtime(&m_mapMgr->pInstance->m_expiration));*/
				welcome_msg += "Cette instance sera réinitialisée ";
				welcome_msg += ConvertTimeStampToDataTime((uint32)m_mapMgr->pInstance->m_expiration);
			}
			sChatHandler.SystemMessage(m_session, welcome_msg.c_str());
		}
	}

	ResetHeartbeatCoords();
}

/*void Player::ModifyBonuses(uint32 type,int32 val)
{
	// Added some updateXXXX calls so when an item modifies a stat they get updated
	// also since this is used by auras now it will handle it for those
	switch (type) 
		{
		case POWER:
			ModUnsigned32Value( UNIT_FIELD_MAXPOWER1, val );
			m_manafromitems += val;
			break;
		case HEALTH:
			ModUnsigned32Value( UNIT_FIELD_MAXHEALTH, val );
			m_healthfromitems += val;
			break;
		case AGILITY: // modify agility				
			FlatStatMod[1] += val;
			CalcStat( 1 );
			break;
		case STRENGTH: //modify strength
			FlatStatMod[0] += val;
			CalcStat( 0 );
			break;
		case INTELLECT: //modify intellect 
			FlatStatMod[3] += val;
			CalcStat( 3 );
			break;
		 case SPIRIT: //modify spirit
			FlatStatMod[4] += val;
			CalcStat( 4 );
			break;
		case STAMINA: //modify stamina
			FlatStatMod[2] += val;
			CalcStat( 2 );
			break;
		case WEAPON_SKILL_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_SKILL, val ); // ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL, val ); // melee main hand
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_OFF_HAND_SKILL, val ); // melee off hand
			}break;
		case DEFENSE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_DEFENCE, val );
			}break;
		case DODGE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_DODGE, val );
			}break;
		case PARRY_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_PARRY, val );
			}break;
		case SHIELD_BLOCK_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_BLOCK, val );
			}break;
		case MELEE_HIT_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT, val );
			}break;
		case RANGED_HIT_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT, val );
			}break;
		case SPELL_HIT_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT, val );
			}break;
		case MELEE_CRITICAL_STRIKE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_CRIT, val );
			}break;
		case RANGED_CRITICAL_STRIKE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_CRIT, val );
			}break;
		case SPELL_CRITICAL_STRIKE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_CRIT, val );
			}break;
		case MELEE_HIT_AVOIDANCE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT_AVOIDANCE, val );
			}break;
		case RANGED_HIT_AVOIDANCE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT_AVOIDANCE, val );
			}break;
		case SPELL_HIT_AVOIDANCE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT_AVOIDANCE, val );
			}break;
		case MELEE_CRITICAL_AVOIDANCE_RATING:
			{

			}break;
		case RANGED_CRITICAL_AVOIDANCE_RATING:
			{

			}break;
		case SPELL_CRITICAL_AVOIDANCE_RATING:
			{

			}break;
		case MELEE_HASTE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HASTE, val );//melee
			}break;
		case RANGED_HASTE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HASTE, val );//ranged
			}break;
		case SPELL_HASTE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HASTE, val );//spell
			}break;
		case HIT_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT, val );
			}break;
		case CRITICAL_STRIKE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_CRIT, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_CRIT, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_CRIT, val );
			}break;
		case HIT_AVOIDANCE_RATING:// this is guessed based on layout of other fields
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT_AVOIDANCE, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT_AVOIDANCE, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT_AVOIDANCE, val );//spell
			}break;
		case CRITICAL_AVOIDANCE_RATING:
			{
				// todo. what is it?
			}break;
		case EXPERTISE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_EXPERTISE, val );
			}break;
		case RESILIENCE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_CRIT_RESILIENCE, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE, val );//spell
			}break;
		case HASTE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HASTE, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HASTE, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HASTE, val );
			}break;
		case ATTACK_POWER:
			{
				ModUnsigned32Value( UNIT_FIELD_ATTACK_POWER_MODS, val );
				ModUnsigned32Value( UNIT_FIELD_RANGED_ATTACK_POWER_MODS, val );
			}break;
		case RANGED_ATTACK_POWER:
			{
				ModUnsigned32Value( UNIT_FIELD_RANGED_ATTACK_POWER_MODS, val );
			}break;
		case FERAL_ATTACK_POWER:
			{
				// todo
			}break;
		case SPELL_HEALING_DONE:
			{
				for(uint32 school=1;school < 7; ++school)
				{
					HealDoneMod[school] += val;
				}
				ModUnsigned32Value( PLAYER_FIELD_MOD_HEALING_DONE_POS, val );
			}break;
		case SPELL_DAMAGE_DONE:
			{
				for(uint32 school=1;school < 7; ++school)
				{
					ModUnsigned32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + school, val );
				}
			}break;
		case MANA_REGENERATION:
			{
				m_ModInterrMRegen += val;
			}break;
		case ARMOR_PENETRATION_RATING:
			{
				ModUnsigned32Value(PLAYER_RATING_MODIFIER_ARMOR_PENETRATION_RATING, val);
			}break;
		case SPELL_POWER:
			{
				for(uint32 school=1;school < 7; ++school)
				{
					ModUnsigned32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + school, val );
					HealDoneMod[school] += val;
				}
				ModUnsigned32Value( PLAYER_FIELD_MOD_HEALING_DONE_POS, val );
			}break;
		}
}*/

bool Player::CanSignCharter(Charter * charter, Player * requester)
{
	if(charter == NULL)
		return false;

	if(charter->CharterType >= CHARTER_TYPE_ARENA_2V2 && m_arenaTeams[charter->CharterType-1] != NULL)
		return false;
	
	if(charter->CharterType == CHARTER_TYPE_GUILD && IsInGuild())
		return false;

	if(m_charters[charter->CharterType] || requester->GetTeam() != GetTeam())
		return false;
	else
		return true;
}

void Player::SaveAuras(stringstream &ss)
{
   
	// Add player auras
//	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		if(m_auras[x])
		{
			Aura *aur=m_auras[x];
			bool skip = false;
			for(uint32 i = 0; i < 3; ++i)
			{
				if(aur->m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA ||
					aur->m_spellProto->Effect[i] == SPELL_EFFECT_ADD_FARSIGHT)
				{
					skip = true;
					break;
				}
			}

			if ((aur->GetDuration(INVOC_DURATION) == -1) && !(aur->m_spellProto->attributesExC & ATTRIBUTESEXC_CAN_PERSIST_AND_CASTED_WHILE_DEAD))
				skip = true;

			if( aur->pSpellId )
				skip = true; //these auras were gained due to some proc. We do not save these eighter to avoid exploits of not removing them

			if ( aur->m_spellProto->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET )
				skip = true;

			// skipped spells due to bugs
/*			why these should't be save??
			switch(aur->m_spellProto->Id)
			{
			case 12043: // Presence of mind
			case 11129: // Combustion
			case 28682: // Combustion proc
			case 16188: // Natures Swiftness
			case 17116: // Natures Swiftness
			case 34936: // Backlash
			case 35076: // Blessing of A'dal
			case 23333:	// WSG
			case 23335:	// WSG
			case 45438:	// Ice Block
			case 642:
			case 1020:	//divine shield
			case 32724:
			case 32725:
			case 32727:
			case 32071:		// Hellfire Superority
			case 32049:		// Hellfire Superority
			case 44521:		// Preparation?

				skip = true;
				break;
			}*/

			//disabled proc spells until proper loading is fixed. Some spells tend to block or not remove when restored
			if(aur->GetSpellProto()->procFlags)
			{
//			 sLog.outDebug("skipping aura %d because has flags %d",aur->GetSpellId(),aur->GetSpellProto()->procFlags);
			      if(aur->GetSpellProto()->procFlags & PROC_ON_ANY_HOSTILE_ACTION) skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_XP_OR_HONOR_KILL)   skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_MELEE_HIT)          skip = true; 
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_MELEE_HIT_VICTIM)   skip = true; 
			 //else if(aur->GetSpellProto()->procFlags & PROC_ON_MELEE_ABILITY_LAND) skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_MELEE_ABILITY_LAND_VICTIM) skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_RANGED_HIT)          skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_RANGED_HIT_VICTIM)   skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_RANGED_ABILITY_LAND) skip = true;
			 //else if(aur->GetSpellProto()->procFlags & PROC_ON_RANGED_ABILITY_LAND_VICTIM) skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_CAST_SPELL)         skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_CAST_SPELL_VICTIM)  skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_CRIT_ATTACK)        skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_CRIT_ATTACK_VICTIM) skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_HEALSPELL_LAND)     skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_HEALSPELL_LAND_VICTIM) skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_HARMFULSPELL_LAND)  skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_HARMFULSPELL_LAND_VICTIM) skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_DOT_DAMAGE)         skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_DOT_DAMAGE_VICTIM)  skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_ANY_DAMAGE_VICTIM)  skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_TRAP_TRIGGER)       skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_AUTO_SHOT_HIT)      skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_OFFHAND_HIT)        skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_DIE)                skip = true; 
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_DODGE_VICTIM)       skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_RESIST_VICTIM)      skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_REMOVEONUSE)           skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_MISC)                  skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_BLOCK_VICTIM)       skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_ON_SPELL_CRIT_HIT)     skip = true;
			 else if(aur->GetSpellProto()->procFlags & PROC_TARGET_SELF)           skip = true;
			} 

			//disabled proc spells until proper loading is fixed. We cannot recover the charges that were used up. Will implement later
			if(aur->GetSpellProto()->procCharges)
			{
//				sLog.outDebug("skipping aura %d because has proccharges %d",aur->GetSpellId(),aur->GetSpellProto()->procCharges);
				skip = true;
			}
			//we are going to cast passive spells anyway on login so no need to save auras for them
            if(aur->IsPassive() && !(aur->GetSpellProto()->attributesEx & 1024)) // ATTRIBUTESEX_UNK12 0x0400 ??
				skip = true;
			//shapeshift
//			if(m_ShapeShifted==aur->m_spellProto->Id)
//				skip=true;

			if(skip)continue;

			int32 d = aur->GetTimeLeft(INVOC_DURATION);
			if(d > 300000) //if(d > 3000)
			{
				char _brzbuff[80];
				sprintf(_brzbuff,"%u,%u,%u,",aur->GetSpellId(),d,aur->GetFlags());
				ss << _brzbuff;
				// Obsolete, prob d'interpretation uint8/char sur GetFlags()
				// Provoque une erreur a la sauvegarde de la base character
				//ss  << aur->GetSpellId() << "," << d << "," << aur->GetFlags() << ","; 

				//Log.Notice("[Player::SaveAuras]","(%s) SaveAura %u, Flag: %u",
				//	                 this->GetName(),aur->GetSpellId(),aur->GetFlags());
			}
		}
	}
}

void Player::SetShapeShift(uint8 ss)
{
	uint8 old_ss = GetByte( UNIT_FIELD_BYTES_2, 3 );
	SetByte( UNIT_FIELD_BYTES_2, 3, ss );

	//remove auras that we should not have
	for( uint32 x = 0; x < MAX_AURAS + MAX_PASSIVE_AURAS; x++ )
	{
		if( m_auras[x] != NULL )
		{
			uint32 reqss = m_auras[x]->GetSpellProto()->shapeshiftMask;
			if( reqss != 0 && m_auras[x]->IsPositive() )
			{
				if( old_ss > 0 && old_ss != 28)
				{
					if(  ( ((uint32)1 << (old_ss-1)) & reqss ) &&		// we were in the form that required it
						!( ((uint32)1 << (ss-1) & reqss) ) )			// new form doesnt have the right form
					{
						m_auras[x]->Remove();
						continue;
					}
				}
			}

			if( this->getClass() == DRUID )
			{
				for (uint32 y = 0; y < 3; ++y )
				{
					switch( m_auras[x]->GetSpellProto()->EffectApplyAuraName[y])
					{
					case SPELL_AURA_MOD_ROOT: //Root
					case SPELL_AURA_MOD_DECREASE_SPEED: //Movement speed
					case SPELL_AURA_MOD_CONFUSE:  //Confuse (polymorph)
						{
							m_auras[x]->Remove();
						}
						break;
					default:
						break;
					}

					if( m_auras[x] == NULL )
						break;
				}
			}
		} 
	}

	// apply any talents/spells we have that apply only in this form.
	set<uint32>::iterator itr;
	SpellEntry * sp;
	Spell * spe;
	SpellCastTargets t(this);

	for( itr = mSpells.begin(); itr != mSpells.end(); ++itr )
	{
		sp = dbcSpell.LookupEntry( *itr );
		if( sp->apply_on_shapeshift_change || sp->attributes & 64 )		// passive/talent
		{
			if( sp->shapeshiftMask && ((uint32)1 << (ss-1)) & sp->shapeshiftMask )
			{
				spe = new Spell( this, sp, true, NULL );
				spe->prepare( &t );
			}
		}
	}

	// now dummy-handler stupid hacky fixed shapeshift spells (leader of the pack, etc)
	for( itr = mShapeShiftSpells.begin(); itr != mShapeShiftSpells.end(); )
	{
		sp = dbcSpell.LookupEntry( *itr );
		++itr;

		if( sp->shapeshiftMask && ((uint32)1 << (ss-1)) & sp->shapeshiftMask )
		{
			spe = new Spell( this, sp, true, NULL );
			spe->prepare( &t );
		}
	}

	// kill speedhack detection for 2 seconds (not needed with new code but bleh)
	DelaySpeedHack( 2000 );

	UpdateStats();
	UpdateChances();
}

void Player::CalcDamage()
{
	float delta;
	float r;
	int ss = GetShapeShift();
	Item *itMH = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
	Item *itOH = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
	Item *itSR = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);

/////////////////MAIN HAND
		float ap_bonus = GetAP()/14000.0f;
		delta = (float)GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS ) - (float)GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_NEG );

		if(IsInFeralForm())
		{
			uint32 lev = getLevel();
			/*if(ss==FORM_CAT)
				r = delta + ap_bonus * 1000.0;
			else
				r = delta + ap_bonus * 2500.0;*/

			if(ss == FORM_CAT)
				r = lev + delta + ap_bonus * 1000.0f;
			else
				r = lev + delta + ap_bonus * 2500.0f;
			
			//SetFloatValue(UNIT_FIELD_MINDAMAGE,r);
			//SetFloatValue(UNIT_FIELD_MAXDAMAGE,r);

			r *= 0.9f;
			r *= 1.1f;

			SetFloatValue(UNIT_FIELD_MINDAMAGE,r>0?r:0);
			SetFloatValue(UNIT_FIELD_MAXDAMAGE,r>0?r:0);

			return;
		}
//////no druid ss	
		uint32 speed=2000;		
		
		if(!disarmed)
		{	
			if(itMH)
				speed = itMH->GetProto()->Delay;
		}
		
		float bonus=ap_bonus*speed;
		float tmp = 1;
		map<uint32, WeaponModifier>::iterator i;
		for(i = damagedone.begin();i!=damagedone.end();i++)
		{
			if((i->second.wclass == (uint32)-1) || //any weapon
				(itMH && ((1 << itMH->GetProto()->SubClass) & i->second.subclass) )
				)
					tmp+=i->second.value/100.0f;
		}
		
		r = BaseDamage[0]+delta+bonus;
		r *= tmp;
		SetFloatValue(UNIT_FIELD_MINDAMAGE,r>0?r:0);
	
		r = BaseDamage[1]+delta+bonus;
		r *= tmp;
		SetFloatValue(UNIT_FIELD_MAXDAMAGE,r>0?r:0);

		uint32 cr = 0;
		if( itMH )
		{
			if( static_cast< Player* >( this )->m_wratings.size() )
			{
				std::map<uint32, uint32>::iterator itr = m_wratings.find( itMH->GetProto()->SubClass );
				if( itr != m_wratings.end() )
					cr=itr->second;
			}
		}
		SetUInt32Value( PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL, cr );
		/////////////// MAIN HAND END

		/////////////// OFF HAND START
		cr = 0;
		
		if(itOH)
		{
			if(!disarmed)
			{
				speed =itOH->GetProto()->Delay;
			}
			else speed  = 2000;
			
			bonus = ap_bonus * speed;
			i = damagedone.begin();
			tmp = 1;
			for(;i!=damagedone.end();i++)
			{
				if((i->second.wclass==(uint32)-1) || //any weapon
					(( (1 << itOH->GetProto()->SubClass) & i->second.subclass)  )
					)
					tmp+=i->second.value/100.0f;
			}

			r = (BaseOffhandDamage[0]+delta+bonus)*offhand_dmg_mod;
			r *= tmp;
			SetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE,r>0?r:0);
			r = (BaseOffhandDamage[1]+delta+bonus)*offhand_dmg_mod;
			r *= tmp;
			SetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE,r>0?r:0);
			if(m_wratings.size ())
			{
				std::map<uint32, uint32>::iterator itr=m_wratings.find(itOH->GetProto()->SubClass);
				if(itr!=m_wratings.end())
					cr=itr->second;
			}
		}
		SetUInt32Value( PLAYER_RATING_MODIFIER_MELEE_OFF_HAND_SKILL, cr );

/////////////second hand end
///////////////////////////RANGED
		cr=0;
		if(itSR)
		{
			i = damagedone.begin();
			tmp = 1;
			for(;i != damagedone.end();i++)
			{
				if( 
					(i->second.wclass == (uint32)-1) || //any weapon
					( ((1 << itSR->GetProto()->SubClass) & i->second.subclass)  )
					)
				{
					tmp+=i->second.value/100.0f;
				}
			}

			if(itSR->GetProto()->SubClass != 19)//wands do not have bonuses from RAP & ammo
			{
//				ap_bonus = (GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER)+(int32)GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS))/14000.0;
				//modified by Zack : please try to use premade functions if possible to avoid forgetting stuff
				ap_bonus = GetRAP()/14000.0f;
				bonus = ap_bonus*itSR->GetProto()->Delay;
				
				if(GetUInt32Value(PLAYER_AMMO_ID))
				{
					ItemPrototype * xproto=ItemPrototypeStorage.LookupEntry(GetUInt32Value(PLAYER_AMMO_ID));
					if(xproto)
					{
						bonus+=((xproto->Damage[0].Min+xproto->Damage[0].Max)*itSR->GetProto()->Delay)/2000.0f;
					}
				}
			}else bonus =0;
			
			r = BaseRangedDamage[0]+delta+bonus;
			r *= tmp;
			SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE,r>0?r:0);

			r = BaseRangedDamage[1]+delta+bonus;
			r *= tmp;
			SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE,r>0?r:0);
			
		
			if(m_wratings.size ())
			{
				std::map<uint32, uint32>::iterator i=m_wratings.find(itSR->GetProto()->SubClass);
				if(i != m_wratings.end())
					cr=i->second;
			}
		
		}
		SetUInt32Value( PLAYER_RATING_MODIFIER_RANGED_SKILL, cr );

/////////////////////////////////RANGED end

		//tmp = 1;
		tmp = 0;
		for(i = damagedone.begin();i != damagedone.end();i++)
		if(i->second.wclass==(uint32)-1)  //any weapon
			tmp += i->second.value/100.0f;
		
		//display only modifiers for any weapon

		// OMG?
		ModFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT ,tmp);
 
}

uint32 Player::GetMainMeleeDamage(uint32 AP_owerride)
{
	float min_dmg,max_dmg;
	float delta;
	float r;
	int ss = GetShapeShift();
/////////////////MAIN HAND
	float ap_bonus;
	if(AP_owerride) 
		ap_bonus = AP_owerride/14000.0f;
	else 
		ap_bonus = GetAP()/14000.0f;
	delta = (float)GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS ) - (float)GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_NEG );
	if(IsInFeralForm())
	{
		uint32 lev = getLevel();
		if(ss == FORM_CAT)
			r = lev + delta + ap_bonus * 1000.0f;
		else
			r = lev + delta + ap_bonus * 2500.0f;
		min_dmg = r * 0.9f;
		max_dmg = r * 1.1f;
		return float2int32(std::max((min_dmg + max_dmg)/2.0f,0.0f));
	}
//////no druid ss	
	uint32 speed=2000;
	Item *it = GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
	if(!disarmed)
	{	
		if(it)
			speed = it->GetProto()->Delay;
	}
	float bonus=ap_bonus*speed;
	float tmp = 1;
	map<uint32, WeaponModifier>::iterator i;
	for(i = damagedone.begin();i!=damagedone.end();i++)
	{
		if((i->second.wclass == (uint32)-1) || //any weapon
			(it && ((1 << it->GetProto()->SubClass) & i->second.subclass) )
			)
				tmp+=i->second.value/100.0f;
	}
	
	r = BaseDamage[0]+delta+bonus;
	r *= tmp;
	min_dmg = r * 0.9f;
	r = BaseDamage[1]+delta+bonus;
	r *= tmp;
	max_dmg = r * 1.1f;

	return float2int32(std::max((min_dmg + max_dmg)/2.0f,0.0f));
}

void Player::EventPortToGM(Player *p)
{
	SafeTeleport(p->GetMapId(),p->GetInstanceID(),p->GetPosition());
}

void Player::UpdateComboPoints()
{
	// fuck bytebuffers :D
	unsigned char buffer[10];
	uint32 c = 2;

	// check for overflow
	if(m_comboPoints > 5)
		m_comboPoints = 5;
	
	if(m_comboPoints < 0)
		m_comboPoints = 0;

	if(m_comboTarget != 0)
	{
		Unit * target = (m_mapMgr != NULL) ? m_mapMgr->GetUnit(m_comboTarget) : NULL;
		if(!target || target->isDead() || GetSelection() != m_comboTarget)
		{
			buffer[0] = buffer[1] = 0;
		}
		else
		{
			c = FastGUIDPack(m_comboTarget, buffer, 0);
			buffer[c++] = m_comboPoints;
		}
	}
	else
		buffer[0] = buffer[1] = 0;

	m_session->OutPacket(SMSG_UPDATE_COMBO_POINTS, c, buffer);
}

void Player::SendAreaTriggerMessage(const char * message, ...)
{
	va_list ap;
	va_start(ap, message);
	char msg[500];
	vsnprintf(msg, 500, message, ap);
	va_end(ap);

	WorldPacket data(SMSG_AREA_TRIGGER_MESSAGE, 6 + strlen(msg));
	data << (uint32)0 << msg << (uint8)0x00;
	m_session->SendPacket(&data);
}

void Player::removeSoulStone()
{
	if(!this->SoulStone) return;
	uint32 sSoulStone = 0;
	switch(this->SoulStone)
	{
	case 3026:
		{
			sSoulStone = 20707;
		}break;
	case 20758:
		{
			sSoulStone = 20762;
		}break;
	case 20759:
		{
			sSoulStone = 20763;
		}break;
	case 20760:
		{
			sSoulStone = 20764;
		}break;
	case 20761:
		{
			sSoulStone = 20765;
		}break;
	case 27240:
		{
			sSoulStone = 27239;
		}break;
	case 47882:
		{
			sSoulStone = 47883;
		}break;
	}
	this->RemoveAura(sSoulStone);
	this->SoulStone = this->SoulStoneReceiver = 0; //just incase
}

void Player::SoftDisconnect()
{/*
      sEventMgr.RemoveEvents(this, EVENT_PLAYER_SOFT_DISCONNECT);
	  WorldSession *session=GetSession();
      session->LogoutPlayer(true);
	  session->Disconnect();*/

	// NOOB NOOB BIG BIG NOOB NOOB
	m_session->Disconnect();
}

void Player::SetNoseLevel()
{
	// Set the height of the player
	switch (getRace())
	{
		case RACE_HUMAN:
		// female
			if (getGender()) m_noseLevel = 1.72f;
			// male
			else m_noseLevel = 1.78f;
		break;
		case RACE_ORC:
			if (getGender()) m_noseLevel = 1.82f;
			else m_noseLevel = 1.98f;
		break;
		case RACE_DWARF:
		if (getGender()) m_noseLevel = 1.27f;
			else m_noseLevel = 1.4f;
		break;
		case RACE_NIGHTELF:
			if (getGender()) m_noseLevel = 1.84f;
			else m_noseLevel = 2.13f;
		break;
		case RACE_UNDEAD:
			if (getGender()) m_noseLevel = 1.61f;
			else m_noseLevel = 1.8f;
		break;
		case RACE_TAUREN:
			if (getGender()) m_noseLevel = 2.48f;
			else m_noseLevel = 2.01f;
		break;
		case RACE_GNOME:
			if (getGender()) m_noseLevel = 1.06f;
			else m_noseLevel = 1.04f;
		break;
		case RACE_TROLL:
			if (getGender()) m_noseLevel = 2.02f;
			else m_noseLevel = 1.93f;
		break;
		case RACE_BLOODELF:
			if (getGender()) m_noseLevel = 1.83f;
			else m_noseLevel = 1.93f;
		break;
		case RACE_DRAENEI:
			if (getGender()) m_noseLevel = 2.09f;
			else m_noseLevel = 2.36f;
		break;
	}
}

void Player::Possess(Unit * pTarget)
{
	if( m_Summon || m_CurrentCharm )
		return;

	ResetHeartbeatCoords();
	if( pTarget->IsPlayer() )
		static_cast<Player *>(pTarget)->ResetHeartbeatCoords();

	m_CurrentCharm = pTarget;
	if(pTarget->GetTypeId() == TYPEID_UNIT)
	{
		// unit-only stuff.
		pTarget->setAItoUse(false);
		pTarget->GetAIInterface()->StopMovement(0);
		pTarget->m_redirectSpellPackets = this;
	}
	m_noInterrupt++;
	SetUInt64Value(UNIT_FIELD_CHARM, pTarget->GetGUID());
	SetUInt64Value(PLAYER_FARSIGHT, pTarget->GetGUID());
	pTarget->GetMapMgr()->ChangeFarsightLocation(static_cast<Player *>(this), pTarget, true);
	//ResetHeartbeatCoords();

	pTarget->SetUInt64Value(UNIT_FIELD_CHARMEDBY, GetGUID());
	pTarget->SetCharmTempVal(pTarget->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	pTarget->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	pTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);

	SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);
	
	/* send "switch mover" packet */
	WorldPacket data1(SMSG_CLIENT_CONTROL_UPDATE, 10);		/* burlex: this should be renamed SMSG_SWITCH_ACTIVE_MOVER :P */
	data1 << pTarget->GetNewGUID() << uint8(1);
	m_session->SendPacket(&data1);

	/* update target faction set */
	pTarget->_setFaction();
	pTarget->UpdateOppFactionSet();

	list<uint32> avail_spells;
	for(list<AI_Spell*>::iterator itr = pTarget->GetAIInterface()->m_spells.begin(); itr != pTarget->GetAIInterface()->m_spells.end(); ++itr)
	{
		if((*itr)->agent == AGENT_SPELL)
			avail_spells.push_back((*itr)->spell->Id);
	}
	list<uint32>::iterator itr = avail_spells.begin();

	/* build + send pet_spells packet */
	// En commentaire pour l'instant (Branruz)
	// A quoi sert cette var ?? 
	// if(pTarget->m_temp_summon) return; 

	WorldPacket data(SMSG_PET_SPELLS, pTarget->GetAIInterface()->m_spells.size() * 4 + 18);
	data << pTarget->GetGUID();
	data << uint16(0x0000);//unk1 ( Family )
	// Inversion Unk2 vs Unk3 ( 3 avant 2 pour le test - Branruz)
	data << uint32(0x00000000);//unk3 
	data << uint32(0x00000101);//unk2 ( State|Action|00|00 )

	// First spell is attack.
	data << uint32(PET_SPELL_ATTACK);

	// Send the actionbar
	for(uint32 i = 1; i < 10; ++i)
	{
		if(itr != avail_spells.end())
		{
			data << uint16((*itr)) << uint16(DEFAULT_SPELL_STATE); // 0x8100, pas d'AutoShot en cours
			++itr;
		}
		else
			data << uint16(0) << uint8(0) << uint8(i+8); // uint8(i+5); // +8 en test (Branruz)
	}

	// Send the rest of the spells.
	data << uint8(avail_spells.size());
	for(itr = avail_spells.begin(); itr != avail_spells.end(); ++itr)
		data << uint16(*itr) << uint16(DEFAULT_SPELL_STATE);
	
	data << uint8(0);
	m_session->SendPacket(&data);
}

void Player::UnPossess()
{
	if( m_Summon || !m_CurrentCharm )
		return;

	Unit * pTarget = m_CurrentCharm; 
	m_CurrentCharm = 0;

	if(pTarget->GetTypeId() == TYPEID_UNIT)
	{
		// unit-only stuff.
		pTarget->setAItoUse(true);
		pTarget->m_redirectSpellPackets = 0;
	}

	ResetHeartbeatCoords();

	m_noInterrupt--;
	SetUInt64Value(PLAYER_FARSIGHT, 0);
	pTarget->GetMapMgr()->ChangeFarsightLocation(static_cast<Player *>(this), pTarget, false);
	SetUInt64Value(UNIT_FIELD_CHARM, 0);
	pTarget->SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);

	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);
	pTarget->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED_CREATURE);
	pTarget->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, pTarget->GetCharmTempVal());
	pTarget->_setFaction();
	pTarget->UpdateOppFactionSet();

	/* send "switch mover" packet */
	WorldPacket data(SMSG_CLIENT_CONTROL_UPDATE, 10);
	data << GetNewGUID() << uint8(1);
	m_session->SendPacket(&data);

	// En commentaire pour l'instant (Branruz)
	// A quoi sert cette var ?? 
	// if(pTarget->m_temp_summon) return;

	data.Initialize(SMSG_PET_SPELLS);
	data << uint64(0);
	m_session->SendPacket(&data);

}

void Player::SummonRequest(uint32 Requestor, uint32 ZoneID, uint32 MapID, uint32 InstanceID, const LocationVector & Position)
{
	m_summonInstanceId = InstanceID;
	m_summonPos = Position;
	m_summoner = Requestor;
	m_summonMapId = MapID;

	WorldPacket data(SMSG_SUMMON_REQUEST, 16);
	data << uint64(Requestor) << ZoneID << uint32(120000);		// 2 minutes
	m_session->SendPacket(&data);
}

void Player::RemoveFromBattlegroundQueue(uint32 queueSlot, bool forced)
{
	if(queueSlot > 2) return;
	if( m_pendingBattleground[queueSlot] )
	{
		m_pendingBattleground[queueSlot]->RemovePendingPlayer((Player *)(this));
		m_pendingBattleground[queueSlot] = NULL;
	}
	m_bgIsQueued[queueSlot] = false;
	m_bgQueueType[queueSlot] = 0;
	m_bgQueueInstanceId[queueSlot] = 0;

	BattlegroundManager.SendBattlegroundQueueStatus((Player *)(this), queueSlot);
	sEventMgr.RemoveEvents((Player *)(this), EVENT_BATTLEGROUND_QUEUE_UPDATE_SLOT_1 + queueSlot);

	if(forced)
	    sChatHandler.SystemMessage(m_session, "You were removed from the queue for the battleground for not joining after 2 minutes.");
}

#ifdef CLUSTERING
void Player::EventRemoveAndDelete()
{

}
#endif

void Player::_AddSkillLine(uint32 SkillLine, uint32 Curr_sk, uint32 Max_sk)
{
	skilllineentry * CheckedSkill = dbcSkillLine.LookupEntry(SkillLine);
	if (!CheckedSkill) //skill doesn't exist, exit here
		return;

	// force to be within limits
	Curr_sk = ( (Curr_sk > 1275) ? 1275 : ( (Curr_sk <1) ? 1 : Curr_sk ) );
	Max_sk = ( (Max_sk > 1275) ? 1275 : ((Max_sk <1) ? 1 : Max_sk ) );

	ItemProf * prof=((ItemProf *)GetProficiencyBySkill(SkillLine));
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr != m_skills.end())
	{
		if( (Curr_sk > itr->second.CurrentValue && Max_sk >= itr->second.MaximumValue) || (Curr_sk == itr->second.CurrentValue && Max_sk > itr->second.MaximumValue) )
		{
			itr->second.CurrentValue = Curr_sk;
			itr->second.MaximumValue = Max_sk;
			_UpdateMaxSkillCounts();
		}
	}
	else
	{
		PlayerSkill inf;
		inf.Skill = CheckedSkill;
		inf.MaximumValue = Max_sk;
		inf.CurrentValue = ( inf.Skill->id != SKILL_RIDING ? Curr_sk : Max_sk );
		inf.BonusValue = 0;
		m_skills.insert( make_pair( SkillLine, inf ) );
		_UpdateSkillFields();
	}
	//Add to proficiency
	if(prof)
	{
		packetSMSG_SET_PROFICICENCY pr;
		pr.ItemClass = prof->itemclass;
		if(prof->itemclass==4)
		{
				armor_proficiency|=prof->subclass;
				//SendSetProficiency(prof->itemclass,armor_proficiency);
				pr.Profinciency = armor_proficiency;
		}
		else
		{
				weapon_proficiency|=prof->subclass;
				//SendSetProficiency(prof->itemclass,weapon_proficiency);
				pr.Profinciency = weapon_proficiency;
		}
		m_session->OutPacket( SMSG_SET_PROFICIENCY, sizeof( packetSMSG_SET_PROFICICENCY ), &pr );
	}

	// hackfix for poisons
	if(SkillLine==SKILL_POISONS && !HasSpell(2842))
		addSpell(2842);
	// hackfix for runeforging
	if(SkillLine==SKILL_RUNEFORGING)
	{
		if(!HasSpell(53341))
			addSpell(53341); // Rune of Cinderglacier
		if(!HasSpell(53343))
			addSpell(53343); // Rune of Razorice
	}
}

void Player::_UpdateSkillFields()
{
	uint32 f = PLAYER_SKILL_INFO_1_1;
	/* Set the valid skills */
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end();)
	{
		if(!itr->first)
		{
			SkillMap::iterator it2 = itr++;
			m_skills.erase(it2);
			continue;
		}

		//ASSERT(f <= PLAYER_CHARACTER_POINTS1);
        if(f > PLAYER_CHARACTER_POINTS1)
	    {
		 Log.Error("Player::_UpdateSkillFields","FATAL: Declaration UpdateField.h PLAYER_SKILL_INFO_1_1 n'a pas la bonne valeur");
		 return;
	    }


		if(itr->second.Skill->type == SKILL_TYPE_PROFESSION)
			SetUInt32Value(f++, itr->first | 0x10000);
		else
			SetUInt32Value(f++, itr->first);

		SetUInt32Value(f++, (itr->second.MaximumValue << 16) | itr->second.CurrentValue);
		SetUInt32Value(f++, itr->second.BonusValue);
		GetAchievementInterface()->HandleAchievementCriteriaReachSkillLevel( itr->second.Skill->id, itr->second.CurrentValue );

		switch(itr->second.Skill->id)
		{
			case SKILL_HERBALISM:
			{
				uint32 skill_base = getRace() == RACE_TAUREN ? 90 : 75;
				if( itr->second.CurrentValue >= skill_base + 375 && !HasSpell( 55503 ) )
				{
					removeSpell(55502,false,false,0);//can't use name_hash
					addSpell( 55503 );												// Lifeblood Rank 6
				}
				else if( itr->second.CurrentValue >= skill_base + 300 && !HasSpell( 55502 ) )
				{
					removeSpell(55501,false,false,0);
					addSpell( 55502 );												// Lifeblood Rank 5
				}
				else if( itr->second.CurrentValue >= skill_base + 225 && !HasSpell( 55501 ) )
				{
					removeSpell(55500,false,false,0);
					addSpell( 55501 );												// Lifeblood Rank 4
				}
				else if( itr->second.CurrentValue >= skill_base + 150 && !HasSpell( 55500 ) )
				{
					removeSpell(55480,false,false,0);
					addSpell( 55500 );												// Lifeblood Rank 3
				}
				else if( itr->second.CurrentValue >= skill_base + 75 && !HasSpell( 55480 ) )
				{
					removeSpell(55428,false,false,0);
					addSpell( 55480 );												// Lifeblood Rank 2
				}
			}break;
			case SKILL_SKINNING:
			{
				if( itr->second.CurrentValue >= 450 && !HasSpell( 53666 ) )
				{
					removeSpell(53665,false,false,0);//can't use namehash here either
					addSpell( 53666 );												// Master of Anatomy Rank 6
				}
				else if( itr->second.CurrentValue >= 375 && !HasSpell( 53665 ) )
				{
					removeSpell(53664,false,false,0);
					addSpell( 53665 );												// Master of Anatomy Rank 5
				}
				else if( itr->second.CurrentValue >= 300 && !HasSpell( 53664 ) )
				{
					removeSpell(53663,false,false,0);
					addSpell( 53664 );												// Master of Anatomy Rank 4
				}
				else if( itr->second.CurrentValue >= 225 && !HasSpell( 53663 ) )
				{
					removeSpell(53662,false,false,0);
					addSpell( 53663 );												// Master of Anatomy Rank 3
				}
				else if( itr->second.CurrentValue >= 150 && !HasSpell( 53662 ) )
				{
					removeSpell(53125,false,false,0);
					addSpell( 53662 );												// Master of Anatomy Rank 2
				}
				else if( itr->second.CurrentValue >= 75 && !HasSpell( 53125 ) )
				{
					addSpell( 53125 );												// Master of Anatomy Rank 1
				}
			}break;
			case SKILL_MINING:
			{
				if( itr->second.CurrentValue >= 450 && !HasSpell( 53040 ) )
				{
					removeSpell(53124,false,false,0);//can't use namehash here either
					addSpell( 53040 );												// Toughness Rank 6
				}
				else if( itr->second.CurrentValue >= 375 && !HasSpell( 53124 ) )
				{
					removeSpell(53123,false,false,0);
					addSpell( 53124 );												// Toughness Rank 5
				}
				else if( itr->second.CurrentValue >= 300 && !HasSpell( 53123 ) )
				{
					removeSpell(53122,false,false,0);
					addSpell( 53123 );												// Toughness Rank 4
				}
				else if( itr->second.CurrentValue >= 225 && !HasSpell( 53122 ) )
				{
					removeSpell(53121,false,false,0);
					addSpell( 53122 );												// Toughness Rank 3
				}
				else if( itr->second.CurrentValue >= 150 && !HasSpell( 53121 ) )
				{
					removeSpell(53120,false,false,0);
					addSpell( 53121 );												// Toughness Rank 2
				}
				else if( itr->second.CurrentValue >= 75 && !HasSpell( 53120 ) )
				{
					addSpell( 53120 );												// Toughness Rank 1
				}
			}break;
		}
		++itr;
	}

	/* Null out the rest of the fields */
	for(; f < PLAYER_CHARACTER_POINTS1; ++f)
	{
		if(m_uint32Values[f] != 0)
			SetUInt32Value(f, 0);
	}
}

bool Player::_HasSkillLine(uint32 SkillLine)
{
	return (m_skills.find(SkillLine) != m_skills.end());
}

void Player::_AdvanceSkillLine(uint32 SkillLine, uint32 Count /* = 1 */)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
	{
		/* Add it */
		_AddSkillLine(SkillLine, Count, getLevel() * 5);
		_UpdateMaxSkillCounts();
	}
	else
	{	
		uint32 curr_sk = itr->second.CurrentValue;
		itr->second.CurrentValue = min(curr_sk + Count,itr->second.MaximumValue);
		if (itr->second.CurrentValue != curr_sk)
			_UpdateSkillFields();
	}
}

uint32 Player::_GetSkillLineMax(uint32 SkillLine)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	return (itr == m_skills.end()) ? 0 : itr->second.MaximumValue;
}

uint32 Player::_GetSkillLineCurrent(uint32 SkillLine, bool IncludeBonus /* = true */)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
		return 0;

	return (IncludeBonus ? itr->second.CurrentValue + itr->second.BonusValue : itr->second.CurrentValue);
}

void Player::_RemoveSkillLine(uint32 SkillLine)
{
	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
		return;

	m_skills.erase(itr);
	_UpdateSkillFields();
}

void Player::_UpdateMaxSkillCounts()
{
	bool dirty = false;
	uint32 new_max;
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->second.Skill->type == SKILL_TYPE_WEAPON || itr->second.Skill->id == SKILL_LOCKPICKING || itr->second.Skill->id == SKILL_POISONS)
		{
			new_max = 5 * getLevel();
		}
		else if (itr->second.Skill->type == SKILL_TYPE_LANGUAGE)
		{
			new_max = 300;
		}
		else if (itr->second.Skill->type == SKILL_TYPE_PROFESSION || itr->second.Skill->type == SKILL_TYPE_SECONDARY)
		{
			new_max = itr->second.MaximumValue;
			if (new_max >= ((sWorld.m_levelCap+5)*5)) new_max = (sWorld.m_levelCap+5)*5;
		}
		else
		{
			new_max = 1;
		}

		// force to be within limits - Secu
		if (new_max > 1275) new_max = 1275;
		if (new_max < 1)   new_max = 1;

	
		if(itr->second.MaximumValue != new_max)
		{
			dirty = true;
			itr->second.MaximumValue = new_max;
		}
		if (itr->second.CurrentValue > new_max)
		{
			dirty = true;
			itr->second.CurrentValue = new_max;
		}
	}

	if(dirty)
		_UpdateSkillFields();
}

void Player::_ModifySkillBonus(uint32 SkillLine, int32 Delta)
{
	if( m_skills.empty() )
		return;

	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
		return;

	itr->second.BonusValue += Delta;
	_UpdateSkillFields();
}

void Player::_ModifySkillBonusByType(uint32 SkillType, int32 Delta)
{
	bool dirty = false;
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->second.Skill->type == SkillType)
		{
			itr->second.BonusValue += Delta;
			dirty=true;
		}
	}

	if(dirty)
		_UpdateSkillFields();
}

/** Maybe this formula needs to be checked?
 * - Burlex
 */

float PlayerSkill::GetSkillUpChance()
{
	float diff = float(MaximumValue - CurrentValue);
	return (diff * 100.0f / float(MaximumValue)) / 3.0f;
}

void Player::_RemoveLanguages()
{
	for(SkillMap::iterator itr = m_skills.begin(), it2; itr != m_skills.end();)
	{
		if(itr->second.Skill->type == SKILL_TYPE_LANGUAGE)
		{
			it2 = itr++;
			m_skills.erase(it2);
		}
		else
			++itr;
	}
}

void PlayerSkill::Reset(uint32 Id)
{
	MaximumValue = 0;
	CurrentValue = 0;
	BonusValue = 0;
	Skill = (Id == 0) ? NULL : dbcSkillLine.LookupEntry(Id);
}

void Player::_AddLanguages(bool All)
{
	/** This function should only be used at login, and after _RemoveLanguages is called.
	 * Otherwise weird stuff could happen :P
	 * - Burlex
	 */

	PlayerSkill sk;
	skilllineentry * en;
	uint32 spell_id;
	static uint32 skills[] = { SKILL_LANG_COMMON, SKILL_LANG_ORCISH, SKILL_LANG_DWARVEN, SKILL_LANG_DARNASSIAN, SKILL_LANG_TAURAHE, SKILL_LANG_THALASSIAN,
		SKILL_LANG_TROLL, SKILL_LANG_GUTTERSPEAK, SKILL_LANG_DRAENEI, 0 };
	
	if(All)
	{
		for(uint32 i = 0; skills[i] != 0; ++i)
		{
			if(!skills[i])
				break;

            sk.Reset(skills[i]);
			sk.MaximumValue = sk.CurrentValue = 300;
			m_skills.insert( make_pair(skills[i], sk) );
			spell_id = ::GetSpellForLanguage(skills[i]);
			if(spell_id)
				addSpell(spell_id);
		}
	}
	else
	{
		for(list<CreateInfo_SkillStruct>::iterator itr = info->skills.begin(); itr != info->skills.end(); ++itr)
		{
			en = dbcSkillLine.LookupEntry(itr->skillid);
			if(en->type == SKILL_TYPE_LANGUAGE)
			{
				sk.Reset(itr->skillid);
				sk.MaximumValue = sk.CurrentValue = 300;
				m_skills.insert( make_pair(itr->skillid, sk) );
				spell_id = ::GetSpellForLanguage(itr->skillid);
				if(spell_id)
					addSpell(spell_id);
			}
		}
	}

	_UpdateSkillFields();
}

float Player::GetSkillUpChance(uint32 id)
{
	SkillMap::iterator itr = m_skills.find(id);
	if(itr == m_skills.end())
		return 0.0f;

	return itr->second.GetSkillUpChance();
}

void Player::_RemoveAllSkills()
{
	m_skills.clear();
	_UpdateSkillFields();
}

void Player::_AdvanceAllSkills(uint32 count)
{
	bool dirty=false;
	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->second.CurrentValue != itr->second.MaximumValue)
		{
			itr->second.CurrentValue += count;
			if(itr->second.CurrentValue >= itr->second.MaximumValue)
				itr->second.CurrentValue = itr->second.MaximumValue;
			dirty=true;
		}
	}

	if(dirty)
		_UpdateSkillFields();
}

void Player::_ModifySkillMaximum(uint32 SkillLine, uint32 NewMax)
{
	// force to be within limits
	NewMax = ( NewMax > 375 ? 375 : NewMax );

	SkillMap::iterator itr = m_skills.find(SkillLine);
	if(itr == m_skills.end())
		return;

	if(NewMax > itr->second.MaximumValue)
	{
		if(SkillLine == SKILL_RIDING)
			itr->second.CurrentValue = NewMax;

		itr->second.MaximumValue = NewMax;
		_UpdateSkillFields();
	}
}

void Player::RemoveSpellTargets(uint32 Type)
{
	if(m_spellIndexTypeTargets[Type] != 0)
	{
		Unit *pUnit = m_mapMgr ? m_mapMgr->GetUnit(m_spellIndexTypeTargets[Type]) : NULL;
		if(pUnit)
            pUnit->RemoveAurasByBuffIndexType(Type, GetGUID());

		m_spellIndexTypeTargets[Type] = 0;
	}
}

void Player::RemoveSpellIndexReferences(uint32 Type)
{
	m_spellIndexTypeTargets[Type] = 0;
}

void Player::SetSpellTargetType(uint32 Type, Unit* target)
{
	m_spellIndexTypeTargets[Type] = target->GetGUID();
}

void Player::RecalculateHonor()
{
	HonorHandler::RecalculateHonorFields(this);
}

//wooot, crapy code rulez.....NOT
void Player::EventTalentHearthOfWildChange(bool apply)
{
	if(!hearth_of_wild_pct)
		return;

	//druid hearth of the wild should add more features based on form
	int tval;
	if(apply)
		tval = hearth_of_wild_pct;
	else tval = -hearth_of_wild_pct;

	uint32 SS=GetShapeShift();

	//increase stamina if :
	if(SS==FORM_BEAR || SS==FORM_DIREBEAR)
	{
		TotalStatModPct[STAT_STAMINA] += tval; 
		CalcStat(STAT_STAMINA);	
		UpdateStats();
		UpdateChances();
	}
	//increase attackpower if :
	else if(SS==FORM_CAT)
	{
		SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER,GetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER)+float(tval/200.0f));
		SetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER, GetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER)+float(tval/200.0f));
		UpdateStats();
		UpdateChances();
	}
}

/************************************************************************/
/* New Save System                                                      */
/************************************************************************/
#ifdef OPTIMIZED_PLAYER_SAVING

void Player::save_LevelXP()
{
	CharacterDatabase.Execute("UPDATE characters SET level = %u, xp = %u WHERE guid = %u", m_uint32Values[UNIT_FIELD_LEVEL], m_uint32Values[PLAYER_XP],this->GetLowGUID());// m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_PositionHP()
{
	CharacterDatabase.Execute("UPDATE characters SET current_hp = %u, current_power = %u, positionX = '%f', positionY = '%f', positionZ = '%f', orientation = '%f', mapId = %u, instance_id = %u WHERE guid = %u",
		m_uint32Values[UNIT_FIELD_HEALTH], m_uint32Values[UNIT_FIELD_POWER1+GetPowerType()], m_position.x, m_position.y, m_position.z, m_position.o, m_mapId, m_instanceId,this->GetLowGUID()); // m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_Gold()
{
	CharacterDatabase.Execute("UPDATE characters SET gold = %u WHERE guid = %u", m_uint32Values[PLAYER_FIELD_COINAGE],this->GetLowGUID()); // m_uint32Values[OBJECT_FIELD_GUID]);
}

void Player::save_GuildData()
{
	if(myGuild)
	{
		string escaped_note = m_playerInfo->publicNote ? CharacterDatabase.EscapeString(m_playerInfo->publicNote) : "";
		string escaped_note2 = m_playerInfo->officerNote ?  CharacterDatabase.EscapeString(m_playerInfo->officerNote) : "";
		CharacterDatabase.Execute("UPDATE characters SET guildid=%u, guildRank=%u, publicNote='%s', officerNote='%s' WHERE guid = %u",
			GetGuildId(), GetGuildRank(), escaped_note.c_str(), escaped_note2.c_str(), this->GetLowGUID()); //m_uint32Values[OBJECT_FIELD_GUID]);
	}
	else
	{
		CharacterDatabase.Execute("UPDATE characters SET guildid = 0, guildRank = 0, publicNote = '', officerNote = '' WHERE guid = %u",
			this->GetLowGUID()); //m_uint32Values[OBJECT_FIELD_GUID]);
	}
}

void Player::save_ExploreData()
{
	char buffer[2100] = {0}; // 4 caracteres * 512 zones = 2048, un peut de marge (346 zones max sur la 3.0.9)
	int p = 0;
	for(uint32 i = 0; i < PLAYER_EXPLORED_ZONES_MAX; ++i) // (Branruz)
	{
		p += sprintf(&buffer[p], "%u,", this->GetUInt32Value(PLAYER_EXPLORED_ZONES_1 + i);
	
		if(p >= 2090) // secu (Branruz)
	    {
	     Log.Error("Player::save_ExploreData","Buffer PLAYER_EXPLORED_ZONES_1 overflow (%s) <- Report this to Ascent Dev",this->GetName());
	     return;
	    }
	}

	CharacterDatabase.Execute("UPDATE characters SET exploration_data = '%s' WHERE guid = %u", buffer, this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
}

void Player::save_Honor()
{
	CharacterDatabase.Execute("UPDATE characters SET honorPointsToAdd = %u, killsToday = %u, killsYesterday = %u, killsLifeTime = %u, honorToday = %u, honorYesterday = %u, honorPoints = %u, arenaPoints = %u WHERE guid = %u",
		m_honorPointsToAdd, m_killsToday, m_killsYesterday, m_killsLifetime, m_honorToday, m_honorYesterday, m_honorPoints, m_arenaPoints, this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
}

void Player::save_Skills()
{
	char buffer[6000] = {0};
	int p = 0;

	for(SkillMap::iterator itr = m_skills.begin(); itr != m_skills.end(); ++itr)
	{
		if(itr->first && itr->second.Skill->type != SKILL_TYPE_LANGUAGE)
			p += sprintf(&buffer[p], "%u;%u;%u;", itr->first, itr->second.CurrentValue, itr->second.MaximumValue);
	}

	//ASSERT(p < 6000);
	if(p >= 6000)
	{
	 Log.Error("Player::save_Skills","Trop de SkillLines pour le joueur %s",this->GetName());
	 return;
	}

	CharacterDatabase.Execute("UPDATE characters SET skills = '%s' WHERE guid = %u", buffer, this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
}

void Player::save_Reputation()
{
	char buffer[10000] = {0};
	int p = 0;

	ReputationMap::iterator iter = m_reputation.begin();
	for(; iter != m_reputation.end(); ++iter)
	{
		p += sprintf(&buffer[p], "%d,%d,%d,%d,",
			iter->first, iter->second->flag, iter->second->baseStanding, iter->second->standing);
	}

	//ASSERT(p < 10000);
	if(p >= 10000)
	{
	 Log.Error("Player::save_Reputation","Trop de Reputation pour le joueur %s",this->GetName());
	 return;
	}
	CharacterDatabase.Execute("UPDATE characters SET reputation = '%s' WHERE guid = %u", buffer, this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
}

void Player::save_Actions()
{
	char buffer[2048] = {0};
	int p = 0;

	for(uint32 i = 0; i < 120; ++i)
	{
		p += sprintf(&buffer[p], "%u,%u,%u,", mActions[i].Action, mActions[i].Misc, mActions[i].Type);
	}

	//ASSERT(p < 2048);
	if(p >= 2048)
	{
	 Log.Error("Player::save_Actions","Trop d'icon dans la barre d'action pour le joueur %s",this->GetName());
	 return;
	}
	CharacterDatabase.Execute("UPDATE characters SET actions = '%s' WHERE guid = %u", buffer, this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
}

void Player::save_BindPosition()
{
	CharacterDatabase.Execute("UPDATE characters SET bindpositionX = '%f', bindpositionY = '%f', bindpositionZ = '%f', bindmapId = %u, bindzoneId = %u WHERE guid = %u",
		m_bind_pos_x, m_bind_pos_y, m_bind_pos_z, m_bind_mapid, m_bind_zoneid, this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
}

void Player::save_EntryPoint()
{
	CharacterDatabase.Execute("UPDATE characters SET entrypointmap = %u, entrypointx = '%f', entrypointy = '%f', entrypointz = '%f', entrypointo = '%f', entrypointinstance = %u WHERE guid = %u",
		m_bgEntryPointMap, m_bgEntryPointX, m_bgEntryPointY, m_bgEntryPointZ, m_bgEntryPointO, m_bgEntryPointInstance, this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
}

void Player::save_Taxi()
{
	char buffer[1024] = {0};
	int p = 0;
	for(uint32 i = 0; i < 8; ++i)
		p += sprintf(&buffer[p], "%u ", m_taximask[i]);

	if(m_onTaxi) {
		CharacterDatabase.Execute("UPDATE characters SET taximask = '%s', taxi_path = %u, taxi_lastnode = %u, taxi_mountid = %u WHERE guid = %u",
			buffer, m_CurrentTaxiPath->GetID(), lastNode, m_uint32Values[UNIT_FIELD_MOUNTDISPLAYID], this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
	}
	else
	{
		CharacterDatabase.Execute("UPDATE characters SET taximask = '%s', taxi_path = 0, taxi_lastnode = 0, taxi_mountid = 0 WHERE guid = %u",
			buffer,this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
	}
}

void Player::save_Zone()
{
	CharacterDatabase.Execute("UPDATE characters SET zoneId = %u WHERE guid = %u", m_zoneId, this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
}

void Player::save_Spells()
{
	char buffer[10000] = {0};
	char buffer2[10000] = {0};
	uint32 p=0,q=0;

	SpellSet::iterator itr = mSpells.begin();
	for(; itr != mSpells.end(); ++itr)
	{
		p += sprintf(&buffer[p], "%u,", *itr);
	}

	for(itr = mDeletedSpells.begin(); itr != mDeletedSpells.end(); ++itr)
	{
		q += sprintf(&buffer2[q], "%u,", *itr);
	}

	CharacterDatabase.Execute("UPDATE characters SET spells = '%s', deleted_spells = '%s' WHERE guid = %u",
		buffer, buffer2, this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
}

void Player::save_InstanceType()
{
	CharacterDatabase.Execute("UPDATE characters SET instancetype = %u WHERE guid = %u", iInstanceType, this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
}

void Player::save_Misc()
{
	uint32 playedt = UNIXTIME - m_playedtime[2];
	m_playedtime[0] += playedt;
	m_playedtime[1] += playedt;
	m_playedtime[2] += playedt;

	uint32 player_flags = m_uint32Values[PLAYER_FLAGS];
	{
		// Remove un-needed and problematic player flags from being saved :p
		if(player_flags & PLAYER_FLAG_PARTY_LEADER)     player_flags &= ~PLAYER_FLAG_PARTY_LEADER;
		if(player_flags & PLAYER_FLAG_AFK)              player_flags &= ~PLAYER_FLAG_AFK;
		if(player_flags & PLAYER_FLAG_DND)              player_flags &= ~PLAYER_FLAG_DND;
		if(player_flags & PLAYER_FLAG_GM)               player_flags &= ~PLAYER_FLAG_GM;
		//if(player_flags & PLAYER_FLAG_PVP_TOGGLE)       player_flags &= ~PLAYER_FLAG_PVP_TOGGLE;
		//if(player_flags & PLAYER_FLAG_FREE_FOR_ALL_PVP) player_flags &= ~PLAYER_FLAG_FREE_FOR_ALL_PVP;
		//if(player_flags & PLAYER_FLAG_PVP_TIMER)        player_flags &= ~PLAYER_FLAG_PVP_TIMER;
	}

	CharacterDatabase.Execute("UPDATE characters SET totalstableslots = %u, first_login = 0, TalentResetTimes = %u, playedTime = '%u %u %u ', isResting = %u, restState = %u, restTime = %u, timestamp = %u, watched_faction_index = %u, ammo_id = %u, available_prof_points = %u, available_talent_points = %u, bytes = %u, bytes2 = %u, player_flags = %u, player_bytes = %u",
		GetStableSlotCount(), GetTalentResetTimes(), m_playedtime[0], m_playedtime[1], playedt, uint32(m_isResting), uint32(m_restState), m_restAmount, UNIXTIME,
		m_uint32Values[PLAYER_FIELD_WATCHED_FACTION_INDEX], m_uint32Values[PLAYER_AMMO_ID], m_uint32Values[PLAYER_CHARACTER_POINTS2],
		m_uint32Values[PLAYER_CHARACTER_POINTS1], m_uint32Values[PLAYER_BYTES], m_uint32Values[PLAYER_BYTES_2], player_flags,
		m_uint32Values[PLAYER_FIELD_BYTES],
		this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
}

void Player::save_PVP()
{
	CharacterDatabase.Execute("UPDATE characters SET pvprank = %u, selected_pvp_title = %u, available_pvp_title = %u WHERE guid = %u",
		uint32(GetPVPRank()), m_uint32Values[PLAYER_CHOSEN_TITLE], m_uint32Values[PLAYER__FIELD_KNOWN_TITLES], this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
}

void Player::save_Auras()
{
	char buffer[10000];
	uint32 p =0;

	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			Aura *aur=m_auras[x];
			bool skip = false;
			for(uint32 i = 0; i < 3; ++i)
			{
				if(aur->m_spellProto->Effect[i] == SPELL_EFFECT_APPLY_AREA_AURA ||
					aur->m_spellProto->Effect[i] == SPELL_EFFECT_ADD_FARSIGHT)
				{
					skip = true;
					break;
				}
			}

			if (aur->GetDuration() == -1 && !(aur->m_spellProto->Flags4 & CAN_PERSIST_AND_CASTED_WHILE_DEAD))
				skip = true;

			// skipped spells due to bugs
/*			switch(aur->m_spellProto->Id)
			{
			case 12043: // Presence of mind
			case 11129: // Combustion
			case 28682: // Combustion proc
			case 16188: // Natures Swiftness
			case 17116: // Natures Swiftness
			case 34936: // Backlash
			case 35076: // Blessing of A'dal
			case 23333:	// WSG
			case 23335:	// WSG
				skip = true;
				break;
			}
*/
			//disabled proc spells until proper loading is fixed. Some spells tend to block or not remove when restored
			if(aur->GetSpellProto()->procFlags)
			{
				//				sLog.outDebug("skipping aura %d because has flags %d",aur->GetSpellId(),aur->GetSpellProto()->procFlags);
				skip = true;
			}
			//disabled proc spells until proper loading is fixed. We cannot recover the charges that were used up. Will implement later
			if(aur->GetSpellProto()->procCharges)
			{
				//				sLog.outDebug("skipping aura %d because has proccharges %d",aur->GetSpellId(),aur->GetSpellProto()->procCharges);
				skip = true;
			}
			//we are going to cast passive spells anyway on login so no need to save auras for them
			if(aur->IsPassive() && !(aur->GetSpellProto()->attributesEx & 1024)) // ATTRIBUTESEX_UNK12 ?? 
				skip = true;

			if(skip)continue;
			uint32 d=aur->GetTimeLeft();
			if(d>3000)
			{
				//Log.Notice("[Player::save_Auras()]","(%s) Aura %u Time %u",this->GetName(),aur->GetSpellId(),d);
				p += sprintf(buffer, "%u,%u,", aur->GetSpellId(), d);
			}
		}
	}

	CharacterDatabase.Execute("UPDATE characters SET auras = '%s' WHERE guid = %u", buffer, this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)
}

#endif

#ifdef CLUSTERING


#endif


void Player::EventGroupFullUpdate()
{
	if(m_playerInfo->m_Group)
	{
		//m_playerInfo->m_Group->Update();
		m_playerInfo->m_Group->UpdateAllOutOfRangePlayersFor(this);
	}
}

void Player::EjectFromInstance()
{
	if(m_bgEntryPointX && m_bgEntryPointY && m_bgEntryPointZ && !IS_INSTANCE(m_bgEntryPointMap))
	{
		if(SafeTeleport(m_bgEntryPointMap, m_bgEntryPointInstance, m_bgEntryPointX, m_bgEntryPointY, m_bgEntryPointZ, m_bgEntryPointO))
			return;
	}

	SafeTeleport(m_bind_mapid, 0, m_bind_pos_x, m_bind_pos_y, m_bind_pos_z, 0);
}

bool Player::HasQuestSpell(uint32 spellid) //Only for Cast Quests
{
	if (quest_spells.size()>0 && quest_spells.find(spellid) != quest_spells.end())
		return true;
	return false;
}
void Player::RemoveQuestSpell(uint32 spellid) //Only for Cast Quests
{
	if (quest_spells.size()>0)
		quest_spells.erase(spellid);
}

bool Player::HasQuestMob(uint32 entry) //Only for Kill Quests
{
	if (quest_mobs.size()>0 && quest_mobs.find(entry) != quest_mobs.end())
		return true;
	return false;
}

bool Player::HasQuest(uint32 entry) 
{
	for(uint32 i=0;i<25;i++)
	{
		if ( m_questlog[i]->GetQuest()->id == entry)
		return true;
	}
	return false;
}

void Player::RemoveQuestMob(uint32 entry) //Only for Kill Quests
{
	if (quest_mobs.size()>0)
		quest_mobs.erase(entry);
}

PlayerInfo::~PlayerInfo()
{
	if(m_Group)
		m_Group->RemovePlayer(this);
}

void Player::CopyAndSendDelayedPacket(WorldPacket * data)
{
	WorldPacket * data2 = new WorldPacket(*data);
	delayedPackets.add(data2);
}

void Player::SendMeetingStoneQueue(uint32 DungeonId, uint8 Status)
{
	WorldPacket data(SMSG_MEETINGSTONE_SETQUEUE, 5);
	data << DungeonId << Status;
	m_session->SendPacket(&data);
}

void Player::PartLFGChannel()
{
#ifndef CLUSTERING
	Channel * pChannel = channelmgr.GetChannel("LookingForGroup", this);
	if( pChannel == NULL )
		return;

	/*for(list<Channel*>::iterator itr = m_channels.begin(); itr != m_channels.end(); ++itr)
	{
		if( (*itr) == pChannel )
		{
			pChannel->Part(this);
			return;
		}
	}*/
	if( m_channels.find( pChannel) == m_channels.end() )
		return;

	pChannel->Part( this );

#endif
}

//if we charmed or simply summoned a pet, this function should get called
void Player::EventSummonPet( Pet *new_pet )
{
	if ( !new_pet ) 
	{
		Log.Error("[Player::EventSummonPet]","new_pet == NULL!");
		return ; //another wtf error, pas possible que ca arrive ?? (Branruz)
	}

	SpellSet::iterator it,iter;
	if(mSpells.size() == 0) // Normalement ne peut pas arriver ??
	{
		Log.Error("[Player::EventSummonPet]","mSpells == NULL!");
		return;
	}
	for(iter= mSpells.begin();iter != mSpells.end();) // mSpells == NULL ?? 
	{
		it = iter++;
		uint32 SpellID = *it;
		SpellEntry *spellInfo = dbcSpell.LookupEntry(SpellID);
		if(spellInfo == NULL) continue;
		if( spellInfo->c_is_flags & SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ) // Spell du player sur lui meme
		{
			this->RemoveAllAuras( SpellID, this->GetGUID() ); //this is required since unit::addaura does not check for talent stacking
			SpellCastTargets targets( this );
			Spell *spell = new Spell(this, spellInfo ,true, NULL);	//we cast it as a proc spell, maybe we should not !
			spell->prepare(&targets);
		}
		if( spellInfo->c_is_flags & SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET ) // Spell du player sur le Familier
		{
			this->RemoveAllAuras( SpellID, this->GetGUID() ); //this is required since unit::addaura does not check for talent stacking
			SpellCastTargets targets( new_pet );
			Spell *spell = new Spell(this, spellInfo ,true, NULL);	//we cast it as a proc spell, maybe we should not !
			spell->prepare(&targets);
		}
	}
	//there are talents that stop working after you gain pet
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x] && m_auras[x]->GetSpellProto()->c_is_flags & SPELL_FLAG_IS_EXPIREING_ON_PET)
			m_auras[x]->Remove();
	}
	//pet should inherit some of the talents from caster
	//new_pet->InheritSMMods(); //not required yet. We cast full spell to have visual effect too
}

//if pet/charm died or whatever hapened we should call this function
//!! note function might get called multiple times :P
void Player::EventDismissPet()
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
		if(m_auras[x] && m_auras[x]->GetSpellProto()->c_is_flags & SPELL_FLAG_IS_EXPIREING_WITH_PET)
			m_auras[x]->Remove();
}

#ifdef ENABLE_COMPRESSED_MOVEMENT

CMovementCompressorThread *MovementCompressor;

void Player::AppendMovementData(uint32 op, uint32 sz, const uint8* data)
{
	//printf("AppendMovementData(%u, %u, 0x%.8X)\n", op, sz, data);
	m_movementBufferLock.Acquire();
	m_movementBuffer << uint8(sz + 2);
	m_movementBuffer << uint16(op);
	m_movementBuffer.append( data, sz );
	m_movementBufferLock.Release();
}

bool CMovementCompressorThread::run()
{
	set<Player*>::iterator itr;
	while(running)
	{
		m_listLock.Acquire();
		for(itr = m_players.begin(); itr != m_players.end(); ++itr)
		{
			(*itr)->EventDumpCompressedMovement();
		}
		m_listLock.Release();
		Sleep(World::m_movementCompressInterval);
	}

	return true;
}

void CMovementCompressorThread::AddPlayer(Player * pPlayer)
{
	m_listLock.Acquire();
	m_players.insert(pPlayer);
	m_listLock.Release();
}

void CMovementCompressorThread::RemovePlayer(Player * pPlayer)
{
	m_listLock.Acquire();
	m_players.erase(pPlayer);
	m_listLock.Release();
}

void Player::EventDumpCompressedMovement()
{
	if( m_movementBuffer.size() == 0 )
		return;

	m_movementBufferLock.Acquire();
	uint32 size = m_movementBuffer.size();
	uint32 destsize = size + size/10 + 16;
	int rate = World::m_movementCompressRate;
	if(size >= 40000 && rate < 6)
		rate = 6;
	if(size <= 100)
		rate = 0;			// don't bother compressing packet smaller than this, zlib doesnt really handle them well

	// set up stream
	z_stream stream;
	stream.zalloc = 0;
	stream.zfree  = 0;
	stream.opaque = 0;

	if(deflateInit(&stream, rate) != Z_OK)
	{
		sLog.outError("deflateInit failed.");
		m_movementBufferLock.Release();
		return;
	}

	uint8 *buffer = new uint8[destsize];

	// set up stream pointers
	stream.next_out  = (Bytef*)buffer+4;
	stream.avail_out = destsize;
	stream.next_in   = (Bytef*)m_movementBuffer.contents();
	stream.avail_in  = size;

	// call the actual process
	if(deflate(&stream, Z_NO_FLUSH) != Z_OK ||
		stream.avail_in != 0)
	{
		sLog.outError("deflate failed.");
		delete [] buffer;
		m_movementBufferLock.Release();
		return;
	}

	// finish the deflate
	if(deflate(&stream, Z_FINISH) != Z_STREAM_END)
	{
		sLog.outError("deflate failed: did not end stream");
		delete [] buffer;
		m_movementBufferLock.Release();
		return;
	}

	// finish up
	if(deflateEnd(&stream) != Z_OK)
	{
		sLog.outError("deflateEnd failed.");
		delete [] buffer;
		m_movementBufferLock.Release();
		return;
	}

	// fill in the full size of the compressed stream
#ifdef USING_BIG_ENDIAN
	*(uint32*)&buffer[0] = swap32(size);
#else
	*(uint32*)&buffer[0] = size;
#endif

	// send it
	m_session->OutPacket(763, (uint16)stream.total_out + 4, buffer);
	//printf("Compressed move compressed from %u bytes to %u bytes.\n", m_movementBuffer.size(), stream.total_out + 4);

	// cleanup memory
	delete [] buffer;
	m_movementBuffer.clear();
	m_movementBufferLock.Release();
}
#endif

void Player::AddShapeShiftSpell(uint32 id)
{
	SpellEntry * sp = dbcSpell.LookupEntry( id );
	mShapeShiftSpells.insert( id );

	if( sp->shapeshiftMask && ((uint32)1 << (GetShapeShift()-1)) & sp->shapeshiftMask )
	{
		Spell * spe = new Spell( this, sp, true, NULL );
		SpellCastTargets t(this);
		spe->prepare( &t );
	}
}

void Player::RemoveShapeShiftSpell(uint32 id)
{
	mShapeShiftSpells.erase( id );
	RemoveAura( id );
}

// COOLDOWNS
void Player::_Cooldown_Add(uint32 Type, uint32 Misc, uint32 Time, uint32 SpellId, uint32 ItemId)
{
	PlayerCooldownMap::iterator itr = m_cooldownMap[Type].find( Misc );
	if( itr != m_cooldownMap[Type].end( ) )
	{
		if( itr->second.ExpireTime < Time )
		{
			itr->second.ExpireTime = Time;
			itr->second.ItemId = ItemId;
			itr->second.SpellId = SpellId;
		}
	}
	else
	{
		PlayerCooldown cd;
		cd.ExpireTime = Time;
		cd.ItemId = ItemId;
		cd.SpellId = SpellId;

		m_cooldownMap[Type].insert( make_pair( Misc, cd ) );
	}

#ifdef _DEBUG
	Log.Debug("Cooldown", "added cooldown for type %u misc %u time %u item %u spell %u", Type, Misc, Time - getMSTime(), ItemId, SpellId);
#endif
}

void Player::Cooldown_Add(SpellEntry * pSpell, Item * pItemCaster)
{
	uint32 mstime = getMSTime();
	int32 cool_time;

	if( pSpell->categoryRecoveryTime > 0 && pSpell->Category )
	{
		cool_time = pSpell->categoryRecoveryTime;
		if( pSpell->SpellGroupType[0] || pSpell->SpellGroupType[1] || pSpell->SpellGroupType[2])
		{
			SM_FIValue(SM_FCooldownTime, &cool_time, pSpell);
			SM_PIValue(SM_PCooldownTime, &cool_time, pSpell);
		}

		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, pSpell->Category, mstime + cool_time, pSpell->Id, pItemCaster ? pItemCaster->GetProto()->ItemId : 0 );
	}
	
	if( pSpell->recoveryTime > 0 )
	{
		cool_time = pSpell->recoveryTime;
		if( pSpell->SpellGroupType[0] || pSpell->SpellGroupType[1] || pSpell->SpellGroupType[2] )
		{
			SM_FIValue(SM_FCooldownTime, &cool_time, pSpell);
			SM_PIValue(SM_PCooldownTime, &cool_time, pSpell);
		}

		_Cooldown_Add( COOLDOWN_TYPE_SPELL, pSpell->Id, mstime + cool_time, pSpell->Id, pItemCaster ? pItemCaster->GetProto()->ItemId : 0 );
	}
}

void Player::Cooldown_AddStart(SpellEntry * pSpell)
{
	if( pSpell->StartrecoveryTime == 0 )
		return;

	uint32 mstime = getMSTime();
	uint32 atime = float2int32( float(pSpell->StartrecoveryTime) * m_floatValues[UNIT_MOD_CAST_SPEED] );
	if( atime <= 0 )
		return;
	if( atime > 1500 )
		atime = 1500; // global cooldown is decreased by spell haste, but it's not INCREASED by spell slow.
	if (atime < 1000)
		atime = 1000; // global cooldown cannot go lower then 1 second through haste

	if( pSpell->StartRecoveryCategory )		// if we have a different cool category to the actual spell category - only used by few spells
		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, pSpell->StartRecoveryCategory, mstime + atime, pSpell->Id, 0 );
	/*else if( pSpell->Category )				// cooldowns are grouped
		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, pSpell->Category, mstime + pSpell->StartrecoveryTime, pSpell->Id, 0 );*/
	else									// no category, so it's a gcd
	{
#ifdef _DEBUG
		Log.Debug("Cooldown", "Global cooldown adding: %u ms", atime );
#endif
		m_globalCooldown = mstime + atime;
	}
}

void Player::Cooldown_OnCancel(SpellEntry *pSpell)
{
	if( pSpell->StartrecoveryTime == 0 || CooldownCheat)
		return;

	uint32 mstime = getMSTime();
	int32 atime = float2int32( float(pSpell->StartrecoveryTime) * m_floatValues[UNIT_MOD_CAST_SPEED] );
	if( atime <= 0 )
		return;

	if( pSpell->StartRecoveryCategory )		// if we have a different cool category to the actual spell category - only used by few spells
		m_cooldownMap[COOLDOWN_TYPE_CATEGORY].erase(pSpell->StartRecoveryCategory);
	else									// no category, so it's a gcd
		m_globalCooldown = mstime;
}

bool Player::Cooldown_CanCast(SpellEntry * pSpell)
{
	PlayerCooldownMap::iterator itr;
	uint32 mstime = getMSTime();

	if( CooldownCheat )
		return true;

	if( pSpell->Category )
	{
		itr = m_cooldownMap[COOLDOWN_TYPE_CATEGORY].find( pSpell->Category );
		if( itr != m_cooldownMap[COOLDOWN_TYPE_CATEGORY].end( ) )
		{
			if( mstime < itr->second.ExpireTime )
				return false;
			else
				m_cooldownMap[COOLDOWN_TYPE_CATEGORY].erase( itr );
		}		
	}

	itr = m_cooldownMap[COOLDOWN_TYPE_SPELL].find( pSpell->Id );
	if( itr != m_cooldownMap[COOLDOWN_TYPE_SPELL].end( ) )
	{
		if( mstime < itr->second.ExpireTime )
			return false;
		else
			m_cooldownMap[COOLDOWN_TYPE_SPELL].erase( itr );
	}

	if( pSpell->StartrecoveryTime && m_globalCooldown )			/* gcd doesn't affect spells without a cooldown it seems */
	{
		if( mstime < m_globalCooldown )
			return false;
		else
			m_globalCooldown = 0;
	}

	return true;
}

void Player::Cooldown_AddItem(ItemPrototype * pProto, uint32 x)
{
	if( pProto->Spells[x].CategoryCooldown <= 0 && pProto->Spells[x].Cooldown <= 0 )
		return;

	ItemSpell * isp = &pProto->Spells[x];
	uint32 mstime = getMSTime();

	if( isp->CategoryCooldown > 0)
		_Cooldown_Add( COOLDOWN_TYPE_CATEGORY, isp->Category, isp->CategoryCooldown + mstime, isp->Id, pProto->ItemId );

	if( isp->Cooldown > 0 )
		_Cooldown_Add( COOLDOWN_TYPE_SPELL, isp->Id, isp->Cooldown + mstime, isp->Id, pProto->ItemId );
}

bool Player::Cooldown_CanCast(ItemPrototype * pProto, uint32 x)
{
	PlayerCooldownMap::iterator itr;
	ItemSpell * isp = &pProto->Spells[x];
	uint32 mstime = getMSTime();

	if( isp->Category )
	{
		itr = m_cooldownMap[COOLDOWN_TYPE_CATEGORY].find( isp->Category );
		if( itr != m_cooldownMap[COOLDOWN_TYPE_CATEGORY].end( ) )
		{
			if( mstime < itr->second.ExpireTime )
				return false;
			else
				m_cooldownMap[COOLDOWN_TYPE_CATEGORY].erase( itr );
		}	
	}

	itr = m_cooldownMap[COOLDOWN_TYPE_SPELL].find( isp->Id );
	if( itr != m_cooldownMap[COOLDOWN_TYPE_SPELL].end( ) )
	{
		if( mstime < itr->second.ExpireTime )
			return false;
		else
			m_cooldownMap[COOLDOWN_TYPE_SPELL].erase( itr );
	}

	return true;
}

#define COOLDOWN_SKIP_SAVE_IF_MS_LESS_THAN 10000

void Player::_SavePlayerCooldowns(QueryBuffer * buf)
{
	PlayerCooldownMap::iterator itr;
	PlayerCooldownMap::iterator itr2;
	uint32 i;
	uint32 seconds;
	uint32 mstime = getMSTime();

	// clear them (this should be replaced with an update queue later)
	if( buf != NULL )
		buf->AddQuery("DELETE FROM playercooldowns WHERE player_guid = %u",this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)	// 0 is guid always
	else
		CharacterDatabase.Execute("DELETE FROM playercooldowns WHERE player_guid = %u", this->GetLowGUID()); //  m_uint32Values[OBJECT_FIELD_GUID] hmmm louche (Branruz)		// 0 is guid always

	for( i = 0; i < NUM_COOLDOWN_TYPES; ++i )
	{
		itr = m_cooldownMap[i].begin( );
		for( ; itr != m_cooldownMap[i].end( ); )
		{
			itr2 = itr++;

			// expired ones - no point saving, nor keeping them around, wipe em
			if( mstime >= itr2->second.ExpireTime )
			{
				m_cooldownMap[i].erase( itr2 );
				continue;
			}
			
			// skip small cooldowns which will end up expiring by the time we log in anyway
			if( ( itr2->second.ExpireTime - mstime ) < COOLDOWN_SKIP_SAVE_IF_MS_LESS_THAN )
				continue;

			// work out the cooldown expire time in unix timestamp format
			// burlex's reason: 30 day overflow of 32bit integer, also
			// under windows we use GetTickCount() which is the system uptime, if we reboot
			// the server all these timestamps will appear to be messed up.
			
			seconds = (itr2->second.ExpireTime - mstime) / 1000;
			// this shouldnt ever be nonzero because of our check before, so no check needed
			
			if( buf != NULL )
			{
				buf->AddQuery( "INSERT INTO playercooldowns VALUES(%u, %u, %u, %u, %u, %u)", this->GetLowGUID(), //m_uint32Values[OBJECT_FIELD_GUID],
					i, itr2->first, seconds + (uint32)UNIXTIME, itr2->second.SpellId, itr2->second.ItemId );
			}
			else
			{
				CharacterDatabase.Execute( "INSERT INTO playercooldowns VALUES(%u, %u, %u, %u, %u, %u)", this->GetLowGUID(), //m_uint32Values[OBJECT_FIELD_GUID],
					i, itr2->first, seconds + (uint32)UNIXTIME, itr2->second.SpellId, itr2->second.ItemId );
			}
		}
	}
}

void Player::_LoadPlayerCooldowns(QueryResult * result)
{
	if( result == NULL )
		return;

	// we should only really call getMSTime() once to avoid user->system transitions, plus
	// the cost of calling a function for every cooldown the player has
	uint32 mstime = getMSTime();
	uint32 type;
	uint32 misc;
	uint32 rtime;
	uint32 realtime;
	uint32 itemid;
	uint32 spellid;
	PlayerCooldown cd;

	do 
	{
		type = result->Fetch()[0].GetUInt32(); // Spell: 0 , Category: 1
		misc = result->Fetch()[1].GetUInt32();
		rtime = result->Fetch()[2].GetUInt32(); // Expire time
		spellid = result->Fetch()[3].GetUInt32();
		itemid = result->Fetch()[4].GetUInt32();

		if( type >= NUM_COOLDOWN_TYPES )
		{
			Log.Error("[Player::_LoadPlayerCooldowns]","(%s - %u) Cooldowns type mauvais %u !",this->GetName(),this->GetGUID(),type);
            continue;
		}

		// remember the cooldowns were saved in unix timestamp format for the reasons outlined above,
		// so restore them back to mstime upon loading

		if( (uint32)UNIXTIME > rtime )
			continue;

		rtime -= (uint32)UNIXTIME;

		if( rtime < 10 )
			continue;

		realtime = mstime + ( ( rtime ) * 1000 );

		// apply it back into cooldown map
		cd.ExpireTime = realtime;
		cd.ItemId = itemid;
		cd.SpellId = spellid;
		m_cooldownMap[type].insert( make_pair( misc, cd ) );

	} while ( result->NextRow( ) );
}

#ifdef COLLISION
void Player::_FlyhackCheck()
{
	/*if(!sWorld.antihack_flight || m_TransporterGUID != 0 || GetTaxiState() || (sWorld.no_antihack_on_gm && GetSession()->HasGMPermissions()))
		return;

	MovementInfo * mi = GetSession()->GetMovementInfo();
	if(!mi) return; //wtf?

	// Falling, CCs, etc. All stuff that could potentially trap a player in mid-air.
	if(!(mi->flags & MOVEFLAG_FALLING) && !(mi->flags & MOVEFLAG_SWIMMING) && !(mi->flags & MOVEFLAG_LEVITATE)&& 
		!(m_special_state & UNIT_STATE_CHARM || m_special_state & UNIT_STATE_FEAR || m_special_state & UNIT_STATE_ROOT || m_special_state & UNIT_STATE_STUN || m_special_state & UNIT_STATE_POLYMORPH || m_special_state & UNIT_STATE_CONFUSE || m_special_state & UNIT_STATE_FROZEN)
		&& !flying_aura && !FlyCheat)
	{
		float t_height = CollideInterface.GetHeight(GetMapId(), GetPositionX(), GetPositionY(), GetPositionZ() + 2.0f);
		if(t_height == 99999.0f || t_height == NO_WMO_HEIGHT )
			t_height = GetMapMgr()->GetLandHeight(GetPositionX(), GetPositionY());
			if(t_height == 99999.0f || t_height == 0.0f) // Can't rely on anyone these days...
				return;

		float p_height = GetPositionZ();

		int32 diff = float2int32(p_height - t_height);
		if(diff < 0)
			diff = -diff;

		if(t_height != p_height && (uint32)diff > sWorld.flyhack_threshold)
		{
			// Fly hax!
			EventTeleport(GetMapId(), GetPositionX(), GetPositionY(), t_height + 2.0f); // relog fix.
			sCheatLog.writefromsession(GetSession(), "Caught fly hacking on map %u hovering %u over the terrain.", GetMapId(), diff);
			//GetSession()->Disconnect();
		}
	}*/
}
#endif

void Player::_SpeedhackCheck(uint32 mstime)
{
if( sWorld.antihack_speed && !m_TransporterGUID && !(m_special_state & UNIT_STATE_CONFUSE) && !m_uint32Values[UNIT_FIELD_CHARMEDBY] && !GetTaxiState() && m_isMoving && GetMapMgr())
	{
		if( ( sWorld.no_antihack_on_gm && GetSession()->HasGMPermissions() ) )
			return; // do not check GMs speed been the config tells us not to.
		if( m_position == _lastHeartbeatPosition && m_isMoving )
		{
			// this means the client is probably lagging. don't update the timestamp, don't do anything until we start to receive
			// packets again (give the poor laggers a chance to catch up)
			return;
		}

		// simplified; just take the fastest speed. less chance of fuckups too
		float speed = ( flying_aura ) ? m_flySpeed : m_runSpeed;
		if( flying_aura )
		{
			if( m_runSpeed > m_flySpeed )
				speed = m_runSpeed;
		}

		if( m_swimSpeed > speed )
			speed = m_swimSpeed;

		if( speed != _lastHeartbeatV )
		{
			if( m_isMoving )
				_startMoveTime = m_lastMoveTime;
			else
				_startMoveTime = 0;

			_lastHeartbeatPosition = m_position;
			_lastHeartbeatV = speed;
			return;
		}

		if( !_heartbeatDisable && !m_uint32Values[UNIT_FIELD_CHARM] && m_TransporterGUID == 0 && !_speedChangeInProgress )
		{
			// latency compensation a little
			speed += 0.25f; 

			float distance = m_position.Distance2D( _lastHeartbeatPosition );
			uint32 time_diff = m_lastMoveTime - _startMoveTime;
			uint32 move_time = float2int32( ( distance / ( speed * 0.001f ) ) );
			int32 difference = time_diff - move_time;
			sLog.outDebug("Player","SpeedhackCheck: speed=%f diff=%i dist=%f move=%u tdiff=%u", speed, difference, distance, move_time, time_diff );
			if( difference < World::m_speedHackThreshold )
			{
				if( m_speedhackChances != 0 )
				{
					SetMovement( MOVE_ROOT, 1 );
					BroadcastMessage( "Speedhack detected. Please contact an admin with the below information if you believe this is a false detection." );
					BroadcastMessage( "You will be disconnected in 10 seconds." );
					BroadcastMessage( MSG_COLOR_WHITE"speed: %f diff: %i dist: %f move: %u tdiff: %u\n", speed, difference, distance, move_time, time_diff );
					sCheatLog.writefromsession(GetSession(), "Speed hack detected! Distance: %i, Speed: %f, Move: %u, tdiff: %u", distance, speed, move_time, time_diff);
					if(m_bg)
						m_bg->RemovePlayer(this, false);

					//SafeTeleport(GetMapMgr(), m_lastHeartbeatPosition);
					sEventMgr.AddEvent(this, &Player::_Kick, EVENT_PLAYER_KICK, 10000, 1, 0 );
					m_speedhackChances = 0;
				}
			}
		}
	}

}

void Player::_Disconnect()
{
	m_session->Disconnect();
}

void Player::ResetSpeedHack()
{
	ResetHeartbeatCoords();
	_heartbeatDisable = 0;
}

void Player::DelaySpeedHack(uint32 ms)
{
	uint32 t;
	_heartbeatDisable = 1;

	if( event_GetTimeLeft( EVENT_PLAYER_RESET_HEARTBEAT, &t ) )
	{
		if( t > ms )		// dont override a slower reset
			return;

		// override it
		event_ModifyTimeAndTimeLeft( EVENT_PLAYER_RESET_HEARTBEAT, ms );
		return;
	}

	// add a new event
	sEventMgr.AddEvent( this, &Player::ResetSpeedHack, EVENT_PLAYER_RESET_HEARTBEAT, ms, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
}

/************************************************************************/
/* SOCIAL                                                               */
/************************************************************************/

void Player::Social_AddFriend(const char * name, const char * note)
{
	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	map<uint32, char*>::iterator itr;
	PlayerInfo * info;
	//Player * pTarget;

	// lookup the player
	info = objmgr.GetPlayerInfoByName(name);
	if( info == NULL )
	{
		data << uint8(FRIEND_NOT_FOUND);
		m_session->SendPacket(&data);
		return;
	}

	// gm check - commented until i come up with a cleaner way of doing this, -comp.
	/*pTarget = m_session->GetPlayer()->GetMapMgr()->GetPlayer(info->guid);
	if( pTarget->GetSession()->HasGMPermissions() && !m_session->HasGMPermissions() && !sWorld.allow_gm_friends)
	{
		data << uint8(FRIEND_NOT_FOUND);
		m_session->SendPacket(&data);
		return;
	}*/

	// team check
	if( info->team != m_playerInfo->team )
	{
		data << uint8(FRIEND_ENEMY) << uint64(info->guid);
		m_session->SendPacket(&data);
		return;
	}

	// are we ourselves?
	if( info == m_playerInfo )
	{
		data << uint8(FRIEND_SELF) << GetGUID();
		m_session->SendPacket(&data);
		return;
	}

	m_socialLock.Acquire();
	itr = m_friends.find(info->guid);
	if( itr != m_friends.end() )
	{
		data << uint8(FRIEND_ALREADY) << uint64(info->guid);
		m_session->SendPacket(&data);
		m_socialLock.Release();
		return;
	}

	if( info->m_loggedInPlayer != NULL )
	{
		data << uint8(FRIEND_ADDED_ONLINE);
		data << uint64(info->guid);
		if( note != NULL )
			data << note;
		else
			data << uint8(0);

		data << uint8(1);
		data << info->m_loggedInPlayer->GetZoneId();
		data << info->lastLevel;
		data << uint32(info->cl);

		info->m_loggedInPlayer->m_socialLock.Acquire();
		info->m_loggedInPlayer->m_hasFriendList.insert( GetLowGUID() );
		info->m_loggedInPlayer->m_socialLock.Release();
	}
	else
	{
		data << uint8(FRIEND_ADDED_OFFLINE);
		data << uint64(info->guid);
	}

	if( note != NULL )
		m_friends.insert( make_pair(info->guid, strdup(note)) );
	else
		m_friends.insert( make_pair(info->guid, (char*)NULL) );

	m_socialLock.Release();
	m_session->SendPacket(&data);

	// dump into the db
	CharacterDatabase.Execute("INSERT INTO social_friends VALUES(%u, %u, \"%s\")",
		GetLowGUID(), info->guid, note ? CharacterDatabase.EscapeString(string(note)).c_str() : "");
}

void Player::Social_RemoveFriend(uint32 guid)
{
	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	map<uint32, char*>::iterator itr;

	// are we ourselves?
	if( guid == GetLowGUID() )
	{
		data << uint8(FRIEND_SELF) << GetGUID();
		m_session->SendPacket(&data);
		return;
	}

	m_socialLock.Acquire();
	itr = m_friends.find(guid);
	if( itr != m_friends.end() )
	{
		if( itr->second != NULL )
			free(itr->second);

		m_friends.erase(itr);
	}

	data << uint8(FRIEND_REMOVED);
	data << uint64(guid);

	m_socialLock.Release();

	Player * pl = objmgr.GetPlayer( (uint32)guid );
	if( pl != NULL )
	{
		pl->m_socialLock.Acquire();
		pl->m_hasFriendList.erase( GetLowGUID() );
		pl->m_socialLock.Release();
	}

	m_session->SendPacket(&data);

	// remove from the db
	CharacterDatabase.Execute("DELETE FROM social_friends WHERE character_guid = %u AND friend_guid = %u", 
		GetLowGUID(), (uint32)guid);
}

void Player::Social_SetNote(uint32 guid, const char * note)
{
	map<uint32,char*>::iterator itr;

	m_socialLock.Acquire();
	itr = m_friends.find(guid);

	if( itr == m_friends.end() )
	{
		m_socialLock.Release();
		return;
	}

	if( itr->second != NULL )
		free(itr->second);

	if( note != NULL )
		itr->second = strdup( note );
	else
		itr->second = NULL;

	m_socialLock.Release();
	CharacterDatabase.Execute("UPDATE social_friends SET note = \"%s\" WHERE character_guid = %u AND friend_guid = %u",
		note ? CharacterDatabase.EscapeString(string(note)).c_str() : "", GetLowGUID(), guid);
}

void Player::Social_AddIgnore(const char * name)
{
	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	set<uint32>::iterator itr;
	PlayerInfo * info;

	// lookup the player
	info = objmgr.GetPlayerInfoByName(name);
	if( info == NULL )
	{
		data << uint8(FRIEND_IGNORE_NOT_FOUND);
		m_session->SendPacket(&data);
		return;
	}

	// are we ourselves?
	if( info == m_playerInfo )
	{
		data << uint8(FRIEND_IGNORE_SELF) << GetGUID();
		m_session->SendPacket(&data);
		return;
	}

	m_socialLock.Acquire();
	itr = m_ignores.find(info->guid);
	if( itr != m_ignores.end() )
	{
		data << uint8(FRIEND_IGNORE_ALREADY) << uint64(info->guid);
		m_session->SendPacket(&data);
		m_socialLock.Release();
		return;
	}

	data << uint8(FRIEND_IGNORE_ADDED);
	data << uint64(info->guid);

	m_ignores.insert( info->guid );

	m_socialLock.Release();
	m_session->SendPacket(&data);

	// dump into db
	CharacterDatabase.Execute("INSERT INTO social_ignores VALUES(%u, %u)", GetLowGUID(), info->guid);
}

void Player::Social_RemoveIgnore(uint32 guid)
{
	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	set<uint32>::iterator itr;

	// are we ourselves?
	if( guid == GetLowGUID() )
	{
		data << uint8(FRIEND_IGNORE_SELF) << GetGUID();
		m_session->SendPacket(&data);
		return;
	}

	m_socialLock.Acquire();
	itr = m_ignores.find(guid);
	if( itr != m_ignores.end() )
	{
		m_ignores.erase(itr);
	}

	data << uint8(FRIEND_IGNORE_REMOVED);
	data << uint64(guid);

	m_socialLock.Release();

	m_session->SendPacket(&data);

	// remove from the db
	CharacterDatabase.Execute("DELETE FROM social_ignores WHERE character_guid = %u AND ignore_guid = %u", 
		GetLowGUID(), (uint32)guid);
}

bool Player::Social_IsIgnoring(PlayerInfo * m_info)
{
	bool res;
	m_socialLock.Acquire();
	if( m_ignores.find( m_info->guid ) == m_ignores.end() )
		res = false;
	else
		res = true;

	m_socialLock.Release();
	return res;
}

bool Player::Social_IsIgnoring(uint32 guid)
{
	bool res;
	m_socialLock.Acquire();
	if( m_ignores.find( guid ) == m_ignores.end() )
		res = false;
	else
		res = true;

	m_socialLock.Release();
	return res;
}

void Player::Social_TellFriendsOnline()
{
	if( m_hasFriendList.empty() )
		return;

	WorldPacket data(SMSG_FRIEND_STATUS, 22);
	set<uint32>::iterator itr;
	Player * pl;
	data << uint8( FRIEND_ONLINE ) << GetGUID() << uint8( 1 );
	data << GetAreaID() << getLevel() << uint32(getClass());

	m_socialLock.Acquire();
	for( itr = m_hasFriendList.begin(); itr != m_hasFriendList.end(); ++itr )
	{
		pl = objmgr.GetPlayer(*itr);
		if( pl != NULL )
			pl->GetSession()->SendPacket(&data);
	}
	m_socialLock.Release();
}

void Player::Social_TellFriendsOffline()
{
	if( m_hasFriendList.empty() )
		return;

	WorldPacket data(SMSG_FRIEND_STATUS, 10);
	set<uint32>::iterator itr;
	Player * pl;
	data << uint8( FRIEND_OFFLINE ) << GetGUID() << uint8( 0 );

	m_socialLock.Acquire();
	for( itr = m_hasFriendList.begin(); itr != m_hasFriendList.end(); ++itr )
	{
		pl = objmgr.GetPlayer(*itr);
		if( pl != NULL )
			pl->GetSession()->SendPacket(&data);
	}
	m_socialLock.Release();
}

void Player::Social_SendFriendList(uint32 flag)
{

	//if( m_hasFriendList.empty() ) // Si le joueur n'a pas d'amis alors ça retourne une liste vide. (Crash)
	//	return;

	WorldPacket data(SMSG_CONTACT_LIST, 500);
	map<uint32,char*>::iterator itr;
	set<uint32>::iterator itr2;
	Player * plr;
	uint32 NbGuys;

	m_socialLock.Acquire();

	// Prise en compte Liste Vide // (Branruz) // ne sert plus car c'est gérer par m_hasFriendList.empty (Crash)
	// SMSG_CONTACT_LIST : 
    // 07000000
    // 00000000

	data << uint32(flag);

	NbGuys = uint32( m_friends.size() + m_ignores.size() ); 

	data << uint32(NbGuys);

    if(!NbGuys) // (Branruz)
	{
		m_socialLock.Release();
	    m_session->SendPacket(&data);
		return;
	}

	for( itr = m_friends.begin(); itr != m_friends.end(); ++itr )
	{
		// guid
		data << uint64( itr->first );

		// friend/ignore flag.
		// 1 - friend
		// 2 - ignore
		// 3 - muted?
		data << uint32( 1 );

		// player note
		if( itr->second != NULL )
			data << itr->second;
		else
			data << uint8(0);

		// online/offline flag
		plr = objmgr.GetPlayer( itr->first );
		if( plr != NULL )
		{
			data << uint8( 1 );
			data << plr->GetZoneId();
			data << plr->getLevel();
			data << uint32( plr->getClass() );
		}
		else
			data << uint8( 0 );
	}

	for( itr2 = m_ignores.begin(); itr2 != m_ignores.end(); ++itr2 )
	{
		// guid
		data << uint64( (*itr2) );
		
		// ignore flag - 2
		data << uint32( 2 );

		// no note
		data << uint8( 0 );
	}

	m_socialLock.Release();
	m_session->SendPacket(&data);
}

void Player::VampiricSpell(uint32 dmg, Unit* pTarget)
{
	float fdmg = float(dmg);
	uint32 bonus;
	int32 perc;
	Group * pGroup = GetGroup();
	SubGroup * pSubGroup = (pGroup != NULL) ? pGroup->GetSubGroup(GetSubGroup()) : NULL;
	GroupMembersSet::iterator itr;

	if( ( !m_vampiricEmbrace && !m_vampiricTouch ) || getClass() != PRIEST )
		return;

	if( m_vampiricEmbrace > 0 && pTarget->m_hasVampiricEmbrace > 0 && pTarget->HasAurasOfNameHashWithCaster(SPELL_HASH_VAMPIRIC_EMBRACE, this) )
	{
		perc = 15;
		//Andy: Come back to this, WTF IS IT?
		//SM_FIValue(SM_FSPELL_VALUE, &perc, 4);

		bonus = float2int32(fdmg * (float(perc)/100.0f));
		if( bonus > 0 )
		{
			Heal(this, 15286, bonus);
			
			// loop party
			if( pSubGroup != NULL )
			{
				for( itr = pSubGroup->GetGroupMembersBegin(); itr != pSubGroup->GetGroupMembersEnd(); ++itr )
				{
					if( (*itr)->m_loggedInPlayer != NULL && (*itr) != m_playerInfo && (*itr)->m_loggedInPlayer->isAlive() )
						Heal( (*itr)->m_loggedInPlayer, 15286, bonus );
				}
			}
		}
	}

	if( m_vampiricTouch > 0 && pTarget->m_hasVampiricTouch > 0 && pTarget->HasAurasOfNameHashWithCaster(SPELL_HASH_VAMPIRIC_TOUCH, this) )
	{
		perc = 5;
		//SM_FIValue(SM_FSPELL_VALUE, &perc, 4);

		bonus = float2int32(fdmg * (float(perc)/100.0f));
		if( bonus > 0 )
		{
			Energize(this, 34919, bonus, POWER_TYPE_MANA);

			// loop party
			if( pSubGroup != NULL )
			{
				for( itr = pSubGroup->GetGroupMembersBegin(); itr != pSubGroup->GetGroupMembersEnd(); ++itr )
				{
					if( (*itr)->m_loggedInPlayer != NULL && (*itr) != m_playerInfo && (*itr)->m_loggedInPlayer->isAlive() && (*itr)->m_loggedInPlayer->GetPowerType() == POWER_TYPE_MANA )
						Energize((*itr)->m_loggedInPlayer, 34919, bonus, POWER_TYPE_MANA);
				}
			}
		}
	}
}

void Player::RemoveQuestsFromLine(uint32 skill_line)
{
	for (int i = 0; i < 25; i++)
	{
		if (m_questlog[i] == NULL)
			continue;

		Quest* qst = m_questlog[i]->GetQuest();
		if (qst && qst->required_tradeskill == skill_line)
		{
			m_questlog[i]->Finish();

			// Remove all items given by the questgiver at the beginning
			for(uint32 j = 0; j < 4; j++)
				if(qst->RecItemDuringQuestId[j])
					GetItemInterface()->RemoveItemAmt(qst->RecItemDuringQuestId[j], 1 );
			if(qst->time > 0)
				timed_quest_slot = 0;
		}
	}

	//for (set<uint32>::iterator itr = m_finishedQuests.begin(); itr != m_finishedQuests.end(); ++itr)
	set<uint32>::iterator itr = m_finishedQuests.begin();
	while(itr != m_finishedQuests.end())
	{
		Quest * qst = QuestStorage.LookupEntry((*itr));
		if (qst != NULL && qst->required_tradeskill == skill_line)
			m_finishedQuests.erase(itr);

		itr++;
	}

	UpdateNearbyGameObjects();
}

uint32 Player::GetMaxRankByHash(uint32 namehash)
{
	uint32 currentrank = 0;
	uint32 currentspellid = 0;

	for (SpellSet::iterator itr=mSpells.begin(); itr!=mSpells.end(); ++itr)
	{
		SpellEntry* sp=dbcSpell.LookupEntry(*itr);

		if (sp != NULL && sp->NameHash == namehash && sp->RankNumber > currentrank)
		{
			currentrank = sp->RankNumber;
			currentspellid = sp->Id;
		}
	}

	return currentspellid;
}

void Player::WipeDailyQuests()
{
	m_finisheddailyQuests.clear();

	//sChatHandler.SystemMessage(GetSession(), "You can only complete 25 more daily quests today.");

	//clear the fields
	for (uint32 i=0; i<25; ++i)
		SetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1 + i, 0);
}

uint32 Player::GetMaxPersonalRating()
{
	uint32 maxrating = 0;
	int i;

	//ASSERT(m_playerInfo != NULL);
    if(!m_playerInfo)
	{
	 Log.Error("Player::GetMaxPersonalRating","GetMemberByGuid returned NULL for player %s",this->GetName());
	 return maxrating;
	}


	for (i=0; i<NUM_ARENA_TEAM_TYPES; i++)
	{
		if(m_arenaTeams[i] != NULL)
		{
			ArenaTeamMember *m = m_arenaTeams[i]->GetMemberByGuid(m_playerInfo->guid);
			if (m)
			{
				if (m->PersonalRating > maxrating) maxrating = m->PersonalRating;
			}
			else
			{
				sLog.outError("%s: GetMemberByGuid returned NULL for player guid = %u\n", __FUNCTION__, m_playerInfo->guid);
			}
		}
	}

	return maxrating;
}

/*void Player::HandleClusterRemove()
{
#ifdef CLUSTERING
	RemoveAllAuras();
	if (IsInWorld())
		RemoveFromWorld();

	sEventMgr.AddEvent(this, &Player::RemovePlayerFromWorld, EVENT_UNK, 30000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT | EVENT_FLAG_DELETES_OBJECT);
	
	if (GetSession() != NULL)
	{
		GetSession()->SetPlayer(NULL);
		if (GetSession()->GetSocket() != NULL)
		{
			uint32 sessionid = GetSession()->GetSocket()->GetSessionId();
			sClusterInterface.DestroySession(sessionid);
		}
		SetSession(NULL);
	}
	ObjectMgr::getSingleton().RemovePlayer(this);
#endif
}

//This must be an event to stay in the correct context!
void Player::EventClusterMapChange(uint32 mapid, uint32 instanceid, LocationVector location)
{
	WorldPacket data;
	uint32 status = sInstanceMgr.PreTeleport(mapid, this, instanceid);
	if(status != INSTANCE_OK)
	{
		data.Initialize(SMSG_TRANSFER_ABORTED);
		data << mapid << status;
		GetSession()->SendPacket(&data);
		return;
	}

	if(instanceid)
		m_instanceId = instanceid;

	if(IsInWorld())
	{
		RemoveFromWorld();
	}

	data.Initialize(SMSG_NEW_WORLD);
	data << (uint32)mapid << location << location.o;
	GetSession()->SendPacket( &data );
	SetMapId(mapid);


	SetPlayerStatus(TRANSFER_PENDING);
	m_sentTeleportPosition = location;
	SetPosition(location);
	ResetHeartbeatCoords();
	z_axisposition = 0.0f;
}*/


void Player::RetroactiveCompleteQuests()
{
	set<uint32>::iterator itr = m_finishedQuests.begin();
	for(; itr != m_finishedQuests.end(); ++itr)
	{
		Quest * pQuest = QuestStorage.LookupEntry( *itr );
		if(!pQuest) continue;

		GetAchievementInterface()->HandleAchievementCriteriaCompleteQuestsInZone( pQuest->zone_id );
	}
}

// Update glyphs after level change
void Player::InitGlyphsForLevel()
{
	// Enable number of glyphs depending on level
	uint32 level = getLevel();
	uint32 glyph_mask = 0; 
	if(level >= 80)
		glyph_mask = 6;
	else if(level >= 70)
		glyph_mask = 5;
	else if(level >= 50)
		glyph_mask = 4;
	else if(level >= 30)
		glyph_mask = 3;
	else if(level >= 15)
		glyph_mask = 2;
	SetUInt32Value(PLAYER_GLYPHS_ENABLED, (1 << glyph_mask) -1 );
}

void Player::InitGlyphSlots()
{
	for(uint32 i = 0; i < 6; ++i)
		SetUInt32Value(PLAYER_FIELD_GLYPH_SLOTS_1 + i, 21 + i);
}

void Player::UnapplyGlyph(uint32 slot)
{
	if(slot > 5)
		return; // Glyph doesn't exist
	// Get info
	uint32 glyphId = GetUInt32Value(PLAYER_FIELD_GLYPHS_1 + slot);
	if(glyphId == 0)
		return;
	GlyphPropertyEntry *glyph = dbcGlyphProperty.LookupEntry(glyphId);
	if(!glyph)
		return;
	SetUInt32Value(PLAYER_FIELD_GLYPHS_1 + slot, 0);
	RemoveAllAuras(glyph->SpellId, NULL);
}
                                //  M  m  m  M  m  M  
static const uint32 glyphType[6] = {0, 1, 1, 0, 1, 0};

uint8 Player::SetGlyph(uint32 slot, uint32 glyphId)
{
	if(slot < 0 || slot > 6)
		return SPELL_FAILED_INVALID_GLYPH;
	// Get info
	GlyphPropertyEntry *glyph = dbcGlyphProperty.LookupEntry(glyphId);
	if(!glyph)
		return SPELL_FAILED_INVALID_GLYPH;

	uint32 typeGlyph = glyph->TypeFlag;
	for(uint32 x = 0; x < GLYPHS_COUNT; ++x)
	{
		if(m_specs[m_talentActiveSpec].glyphs[x] == glyphId && slot != x)
			return SPELL_FAILED_UNIQUE_GLYPH;
	}
	// GlyphProperties.dbc - 5 Glyphes possedent des types different de 0 ou 1
	// 21 - 22 - 61 - 82 - 121
	// (glyphType[slot] != glyph->TypeFlag) Obsolete pour prendre en compte les Flags
    // Correction par filtre, on verra bien ce que ca donne (Branruz)
	// Note: 0 = Glyphe Majeur, 1 = Glyphe Mineur
	if(typeGlyph > 1) typeGlyph &= 0x00000001; // Filtre 

	if( (glyphType[slot] != typeGlyph) || // Glyph type doesn't match
			(GetUInt32Value(PLAYER_GLYPHS_ENABLED) & (1 << slot)) == 0) // slot is not enabled
		return SPELL_FAILED_INVALID_GLYPH;

	UnapplyGlyph(slot);
	SetUInt32Value(PLAYER_FIELD_GLYPHS_1 + slot, glyphId);
	m_specs[m_talentActiveSpec].glyphs[slot] = glyphId;
	CastSpell(this, glyph->SpellId, true);	// Apply the glyph effect
	return 0;
}

/*void Player::ConvertRune(uint8 index, uint8 value)
{
	ASSERT(index < 6);
	m_runes[index] = value;
	m_runemask |= (1 << index);
	if(value >= RUNE_TYPE_RECHARGING)
		return;

	WorldPacket data(SMSG_CONVERT_RUNE, 2);
	data << (uint8)index;
	data << (uint8)value;
	if( GetSession() )
		GetSession()->SendPacket(&data);
}*/

bool Player::CanUseRunes(uint8 blood, uint8 frost, uint8 unholy)
{
	uint8 death = 0;
	for(uint8 i = 0; i < 6; ++i)
	{
		if( m_runes[ i ] == RUNE_TYPE_BLOOD && blood )
			blood--;
		if( m_runes[ i ] == RUNE_TYPE_FROST && frost )
			frost--;
		if( m_runes[ i ] == RUNE_TYPE_UNHOLY && unholy )
			unholy--;

		if( m_runes[ i ] == RUNE_TYPE_DEATH )
			death++;
	}

	uint8 res = blood + frost + unholy;
	if( res == 0 )
		return true;

	if( death >= (blood + frost + unholy) )
		return true;

	return false;
}

void Player::ScheduleRuneRefresh(uint8 index, bool forceDeathRune)
{
	sEventMgr.RemoveEvents(this, EVENT_PLAYER_RUNE_REGEN + index);
	sEventMgr.AddEvent(this, &Player::ConvertRune, (uint8)index, (forceDeathRune ? uint8(RUNE_TYPE_DEATH) : baseRunes[index]), EVENT_PLAYER_RUNE_REGEN + index, 10000, 0, 0);
}

void Player::UseRunes(uint8 RuneBlood, uint8 RuneFrost, uint8 RuneUnholy, SpellEntry* pSpell)
{
	uint8 death = 0;
	for(uint8 i = 0; i < 6; ++i)
	{
		if( m_runes[ i ] == RUNE_TYPE_BLOOD && RuneBlood )
		{
			RuneBlood--;
			m_runemask &= ~(1 << i);
			m_runes[ i ] = RUNE_TYPE_RECHARGING;
			ScheduleRuneRefresh(i);
			continue;
		}
		if( m_runes[ i ] == RUNE_TYPE_FROST && RuneFrost )
		{
			RuneFrost--;
			m_runemask &= ~(1 << i);
			m_runes[ i ] = RUNE_TYPE_RECHARGING;

			if( pSpell && pSpell->NameHash == SPELL_HASH_DEATH_STRIKE || pSpell->NameHash == SPELL_HASH_OBLITERATE && Rand(pSpell->procChance) )
				ScheduleRuneRefresh(i, true);
			else
				ScheduleRuneRefresh(i);
			continue;
		}
		if( m_runes[ i ] == RUNE_TYPE_UNHOLY && RuneUnholy )
		{
			RuneUnholy--;
			m_runemask &= ~(1 << i);
			m_runes[ i ] = RUNE_TYPE_RECHARGING;
			if( pSpell && pSpell->NameHash == SPELL_HASH_DEATH_STRIKE || pSpell->NameHash == SPELL_HASH_OBLITERATE && Rand(pSpell->procChance) )
				ScheduleRuneRefresh(i, true);
			else
				ScheduleRuneRefresh(i);
			continue;
		}

		if( m_runes[ i ] == RUNE_TYPE_DEATH )
			death++;
	}

	uint8 res = RuneBlood + RuneFrost + RuneUnholy;

	if( res == 0 )
		return;

	for(uint8 i = 0; i < 6; ++i)
	{
		if( m_runes[ i ] == RUNE_TYPE_DEATH && res )
		{
			res--;
			m_runemask &= ~(1 << i);
			m_runes[ i ] = RUNE_TYPE_RECHARGING;
			ScheduleRuneRefresh(i);
		}
	}
}

uint8 Player::TheoreticalUseRunes(uint8 RuneBlood, uint8 RuneFrost, uint8 RuneUnholy)
{
	uint8 runemask = m_runemask;
	uint8 death = 0;
	for(uint8 i = 0; i < 6; ++i)
	{
		if( m_runes[ i ] == RUNE_TYPE_BLOOD && RuneBlood )
		{
			RuneBlood--;
			runemask &= ~(1 << i);
		}
		if( m_runes[ i ] == RUNE_TYPE_FROST && RuneFrost )
		{
			RuneFrost--;
			runemask &= ~(1 << i);
		}
		if( m_runes[ i ] == RUNE_TYPE_UNHOLY && RuneUnholy )
		{
			RuneUnholy--;
			runemask &= ~(1 << i);
		}

		if( m_runes[ i ] == RUNE_TYPE_DEATH )
			death++;
	}

	uint8 res = RuneBlood + RuneFrost + RuneUnholy;

	if( res == 0 )
		return runemask;

	for(uint8 i = 0; i < 6; ++i)
	{
		if( m_runes[ i ] == RUNE_TYPE_DEATH && res )
		{
			res--;
			runemask &= ~(1 << i);
		}
	}
	
	return runemask;
}
const char* Player::GetObjectName()
{
	return GetName();
}

void Player::ClearRuneCooldown(uint8 index)
{
	WorldPacket data(SMSG_ADD_RUNE_POWER, 4);
	data << uint32(1 << index);
	GetSession()->SendPacket(&data);
}

void Player::OnPositionChange() // Déplacement des joueurs dans les vehicules 
{
	/*Unit::OnPositionChange();
	uint32 newareaid = GetMapMgr()->GetAreaID(GetPositionX(), GetPositionY(), GetPositionZ());
	uint32 oldareaid = m_AreaID;
	m_AreaID = newareaid;

	//get area zone
	AreaTable* newat = dbcArea.LookupEntry(newareaid);
	AreaTable* oldat = dbcArea.LookupEntry(oldareaid);

	std::multimap<uint32, uint32>::iterator itr;

	if (oldat == newat)
		return;

	//Remove invalid auras
	if (oldat != NULL)
	{
		itr = m_zoneidauras.find(oldat->ZoneId);
		for (; itr != m_zoneidauras.upper_bound(oldat->ZoneId); ++itr)
			sEventMgr.AddEvent((Unit *)(this), &Unit::EventRemoveAura, itr->second, EVENT_AURA_REMOVE, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		itr = m_areaidauras.find(oldat->AreaId);
		for (; itr != m_areaidauras.upper_bound(oldat->AreaId); ++itr)
			sEventMgr.AddEvent((Unit *)(this), &Unit::EventRemoveAura, itr->second, EVENT_AURA_REMOVE, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}

	if (newat != NULL)
	{
		itr = m_zoneidauras.find(newat->ZoneId);
		for (; itr != m_zoneidauras.upper_bound(oldat->ZoneId); ++itr)
			sEventMgr.AddEvent((Unit *)(this), &Unit::EventCastSpell, (Unit *)(this), dbcSpell.LookupEntry(itr->second), EVENT_AURA_REMOVE, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		itr = m_areaidauras.find(newat->AreaId);
		for (; itr != m_areaidauras.upper_bound(newat->AreaId); ++itr)
			sEventMgr.AddEvent((Unit *)(this), &Unit::EventCastSpell, (Unit *)(this), dbcSpell.LookupEntry(itr->second), EVENT_AURA_REMOVE, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
*/
}

void Player::UpdateKnownGlyphs()
{
	uint32 Level = GetUInt32Value(UNIT_FIELD_LEVEL);
	//Apply glyphs :D
	if (Level >= 15) SetFlag(PLAYER_GLYPHS_ENABLED, 0x01 | 0x02); //first 2 glyphs are at 15
	if (Level >= 30) SetFlag(PLAYER_GLYPHS_ENABLED, 0x04);
	if (Level >= 50) SetFlag(PLAYER_GLYPHS_ENABLED, 0x08);
	if (Level >= 70) SetFlag(PLAYER_GLYPHS_ENABLED, 0x10);
	if (Level >= 80) SetFlag(PLAYER_GLYPHS_ENABLED, 0x20);
}
// Waad - Pas sur que ce soit le bon endroit pour rafraichir les Auras (Branruz)
void Player::UpdateSpellEffectArea()
{
	// Appliquer les effets d'aura sur les Pnj/mob a proximité
	// Verifier aussi le flag JcJ de la cible et sa faction (ami/ennemi)
}
// Waad - Pas sur que ce soit le bon endroit pour declencher les Go (Branruz)
// Recherche d'un Go particulier qui Bug (non clickable ou sans AreaTrigger)
// ATTENTION: Vous devrez sortir et re entrer dans la zone pour reactivé le Go
void Player::OnActivateGoArea(uint32 zone_id) 
{
	GameObject *GObj = NULL;
	std::set<Object*>::iterator Itr = this->GetInRangeSetBegin();
	std::set<Object*>::iterator Itr2 = this->GetInRangeSetEnd();
    float Distance_Declenchement = -1.0f; // Reglage de la distance minimum de declenchement
	bool Stop_Boucle = false;

    // Filtre sur la zone, histoire de gagner du temps processeur
	/*
	switch(zone_id) // Ajouter ici les zone_id/map_id a scanner
	{
	 case 609:  // Fort d'ebene
		 Log.Notice("DEBUG Go","%s est au Fort d'ebene",this->GetName());
		 break;
	 default: 
		 Log.Notice("DEBUG Go","%s n'est pas dans la zone de recherche",this->GetName());
		 return;
	}*/


	for( ; Itr != Itr2; Itr++ )
	{
		if( (*Itr)->GetTypeId() == TYPEID_GAMEOBJECT )
		{  
			switch((*Itr)->GetEntry())
			{
			 //	case Id_Nouveau_Go:
			 //	          Distance_Declenchement = x.xf; // 0.0 est permis
			 //			  break;
			 case 191539: // 1.007 au centre, Doodad_Nox_portal_purple_bossroom17 (Premier Portail, Fort d'ebene 1ier Etage)
				          Distance_Declenchement = 1.15f;
						  break;
			 case 191538: // Doodad_Nox_portal_purple_bossroom01 (2ieme Portail, Fort d'eben, Rez de chaussée)
                          Distance_Declenchement = 1.15f;
						  break;
			             
			 default: //Log.Notice("DEBUG Go","Pas de ScriptGo proche");
				      continue;
			}
            
			if(Distance_Declenchement >= 0)
			{
 			 //Log.Notice("DEBUG Go","Distance de declenchement : %f",Distance_Declenchement);

			 if( this->CalcDistance( *Itr ) <= Distance_Declenchement ) 
		     { 
				if(m_AreaGoTriggered) 
				{
					//Log.Warning("GObject","%s est deja proche de %s",this->GetName(),(*Itr)->GetObjectName());
					break; // Deja Trig, on attend la sortie de la zone
				}

				m_AreaGoTriggered = true; // Pour eviter de declencher a la file.....
				GObj = (GameObject *)(*Itr);
				CALL_GO_EVENT_SCRIPT_POS(GObj, OnActivatePosition)(this); // Script activer, One Shot
				Stop_Boucle = true;
          	    //Log.Notice("DEBUG Go","Declenchement du Go %u par %s",(*Itr)->GetEntry(),this->GetName());
				//m_AreaGoTriggered = false; // Ok pour redeclenchement a la prochaine maj du timer
			 }
			 else
			 {
                m_AreaGoTriggered = false; // Ok, le player est sorti de la zone de declenchement
			 }
			}
		}
		if(Stop_Boucle) break;
	}

}
//Ascent 313 rev5
bool Player::SetTaximaskNode(uint32 nodeidx)
{
	uint8  field   = uint8((nodeidx - 1) / 32);
	uint32 submask = 1<<((nodeidx-1)%32);
	if ((m_taximask[field] & submask) != submask )
	{
		m_taximask[field] |= submask;
		return true;
	}
return false;
}
//Ascent 313 rev5
void Player::GroupUninvite(Player *targetPlayer, PlayerInfo *targetInfo)
{

	if ( !InGroup() || targetInfo->m_Group != GetGroup() )
	{
		GetSession()->SendPartyCommandResult(this, 0, "", ERR_PARTY_IS_NOT_IN_YOUR_PARTY);
		return;
	}

	if ( !IsGroupLeader() || targetInfo->m_Group->HasFlag(GROUP_FLAG_BATTLEGROUND_GROUP) )	// bg group
	{
		if(this != targetPlayer)
		{
			GetSession()->SendPartyCommandResult(this, 0, "", ERR_PARTY_YOU_ARE_NOT_LEADER);
			return;
		}
	}

	if(m_bg)
		return;

	Group *group = GetGroup();

	if(group)
		group->RemovePlayer(targetInfo);
}

void Player::UpdateTalentInspectBuffer()
{
	memset(m_talentInspectBuffer, 0, TALENT_INSPECT_BYTES);

	uint32 talent_tab_pos = 0;
	uint32 talent_max_rank;
	uint32 talent_tab_id;
	uint32 talent_index;
	uint32 rank_index;
	uint32 rank_slot;
	uint32 rank_offset;
	uint32 i;

	for( i = 0; i < 3; ++i )
	{
		talent_tab_id = sWorld.InspectTalentTabPages[getClass()][i];

		for( uint32 j = 0; j < dbcTalent.GetNumRows(); ++j )
		{
			TalentEntry const* talent_info = dbcTalent.LookupRow( j );

			//sLog.outDebug( "HandleInspectOpcode: i(%i) j(%i)", i, j );

			if( talent_info == NULL )
				continue;

			//sLog.outDebug( "HandleInspectOpcode: talent_info->TalentTree(%i) talent_tab_id(%i)", talent_info->TalentTree, talent_tab_id );

			if( talent_info->TalentTree != talent_tab_id )
				continue;

			talent_max_rank = 0;
			for( uint32 k = 5; k > 0; --k )
			{
				//sLog.outDebug( "HandleInspectOpcode: k(%i) RankID(%i) HasSpell(%i) TalentTree(%i) Tab(%i)", k, talent_info->RankID[k - 1], player->HasSpell( talent_info->RankID[k - 1] ), talent_info->TalentTree, talent_tab_id );
				if( talent_info->RankID[k - 1] != 0 && HasSpell( talent_info->RankID[k - 1] ) )
				{
					talent_max_rank = k;
					break;
				}
			}

			//sLog.outDebug( "HandleInspectOpcode: RankID(%i) talent_max_rank(%i)", talent_info->RankID[talent_max_rank-1], talent_max_rank );

			if( talent_max_rank <= 0 )
				continue;

			talent_index = talent_tab_pos;

			std::map< uint32, uint32 >::iterator itr = sWorld.InspectTalentTabPos.find( talent_info->TalentID );

			if( itr != sWorld.InspectTalentTabPos.end() )
				talent_index += itr->second;
			//else
			//sLog.outDebug( "HandleInspectOpcode: talent(%i) rank_id(%i) talent_index(%i) talent_tab_pos(%i) rank_index(%i) rank_slot(%i) rank_offset(%i)", talent_info->TalentID, talent_info->RankID[talent_max_rank-1], talent_index, talent_tab_pos, rank_index, rank_slot, rank_offset );

			rank_index = ( uint32( ( talent_index + talent_max_rank - 1 ) / 7 ) ) * 8  + ( uint32( ( talent_index + talent_max_rank - 1 ) % 7 ) );
			rank_slot = rank_index / 8;
			rank_offset = rank_index % 8;

			if( rank_slot < TALENT_INSPECT_BYTES )
			{
				uint32 v = (uint32)m_talentInspectBuffer[rank_slot];
				v |= ( 1 << rank_offset );
				m_talentInspectBuffer[rank_offset] |= v;
			};

			sLog.outDebug( "Player","HandleInspectOpcode: talent(%i) talent_max_rank(%i) rank_id(%i) talent_index(%i) talent_tab_pos(%i) rank_index(%i) rank_slot(%i) rank_offset(%i)", talent_info->TalentID, talent_max_rank, talent_info->RankID[talent_max_rank-1], talent_index, talent_tab_pos, rank_index, rank_slot, rank_offset );
		}

		std::map< uint32, uint32 >::iterator itr = sWorld.InspectTalentTabSize.find( talent_tab_id );

		if( itr != sWorld.InspectTalentTabSize.end() )
			talent_tab_pos += itr->second;

	}
}

void Player::EventCheckCurrencies()
{
	ItemIterator itr(m_ItemInterface);
	itr.BeginSearch();
	for(; !itr.End(); itr.Increment())
	{
		if(itr.Grab() == NULL)
			continue;

		if(itr->GetProto() == NULL || (itr->GetProto()->BagFamily & ITEM_TYPE_PVP) == 0)
			continue;

		if(itr.GetCurrentSlot() >= INVENTORY_CURRENCY_1 && itr.GetCurrentSlot() <= INVENTORY_CURRENCY_32)
			continue;

		uint32 count = itr->GetCount();
		Item * i = new (m_ItemInterface->SafeRemoveAndRetreiveItemByGuid(itr->GetGUID(), false)) Item();

		uint32 slot = m_ItemInterface->GetInventorySlotById(itr->GetEntry());
		while(slot != (uint32)-1 && slot < INVENTORY_CURRENCY_1)
		{
			Item * itm = new (m_ItemInterface->SafeRemoveAndRetreiveItemFromSlot(INVENTORY_SLOT_NOT_SET, slot, false)) Item();
			count += itm->GetCount();
			if(itm->IsInWorld())
				itm->RemoveFromWorld();
			itm->DeleteFromDB();
			delete itm;
			itm = NULL;

			slot = m_ItemInterface->GetInventorySlotById(itr->GetEntry());
		}

		if(slot == INVENTORY_SLOT_NOT_SET)
		{
			// No currency item with this entry in currencies slots
			Item * itm = objmgr.CreateItem(itr->GetEntry(), this);
			itm->SetCount(count);
			if(m_ItemInterface->AddItemToFreeSlot(itm) == ADD_ITEM_RESULT_OK)
				itm->m_isDirty = true;
		}
		else
		{
			Item * itm = m_ItemInterface->GetInventoryItem(slot);
			count += itm->GetCount();
			itm->SetCount(count);
			itm->m_isDirty = true;
		}

		if(i->IsInWorld())
			i->RemoveFromWorld();
		i->DeleteFromDB();
		delete i;
		i = NULL;
	}
	itr.EndSearch();
}
//----------------------------------------------------
// Battleground
//----------------------------------------------------
bool Player::HasBattlegroundQueueSlot()
{
	if( m_bgIsQueued[0] &&  m_bgIsQueued[1] && m_bgIsQueued[2] )
		return false;

	return true;
}
//----------------------------------------------------
uint32 Player::GetBGQueueSlot()
{
	for(uint32 i = 0; i < 3; ++i)
	{
		if( !m_bgIsQueued[i] )
			return i;
	}

	return 0; // Using 0 so if bad code comes up, we don't make an access violation :P
}
//----------------------------------------------------
uint32 Player::HasBGQueueSlotOfType(uint32 type)
{
	for(uint32 i = 0; i < 3; ++i)
	{
		if( m_bgIsQueued[i] &&
			m_bgQueueType[i] == type)
			return i;
	}

	return 4;
}
//----------------------------------------------------
uint32 Player::GetBGQueueSlotByBGType(uint32 type)
{
	for(uint32 i = 0; i < 3; ++i)
	{
		if( m_bgQueueType[i] == type)
			return i;
	}
	return 0;
}
//----------------------------------------------------
uint8 Player::ConfirmPlayerTarget( SpellCastTargets * targets,Object * _Caster,uint32 _Id, uint32 _EffectImplicitTarget )
{
	uint32 PlayerTargetEffect = _EffectImplicitTarget;
	uint32 spellId = _Id;
	SpellEntry *spellInfo = dbcSpell.LookupEntryForced(spellId);
	if (!spellInfo)
	{
		Log.Error("HandleUseItemOpcode","Unknown spell id %i\n", spellId);
		return(SPELL_FAILED_SPELL_UNAVAILABLE);
	}


 switch(PlayerTargetEffect)
 {

	case EFF_TARGET_NONE			: // 0, , Cible pas necessaire (Concerne directement le caster et/ou AoE global)
	   Log.Warning("[ConfirmPlayerTarget]","EFF_TARGET_NONE, Target <<= _Caster (%d)",_Caster->GetEntry());
       targets->m_target = _Caster; // On tente (Branruz)
	   break;

	case EFF_TARGET_SELF			: // 1, Target=Player, deja init
	   break;

	case EFF_TARGET_PET: //5 - Target: Pet
		{
			if(_Caster->IsPlayer() && ((Player *)_Caster)->GetSummon())
			{
				uint64 guidPetTarget = ((Player *)_Caster)->GetSummon()->GetGUID();
				Pet * pet = ((Player *)_Caster)->GetMapMgr()->GetPet(GET_LOWGUID_PART(guidPetTarget));
				if(pet) targets->m_target = pet;				
					else 
					{
						Log.Error("ConfirmPlayerTarget","Familier NULL, report this to devs.");
						return(SPELL_FAILED_NO_PET);
					}
			}
		}
	return( SPELL_CANCAST_OK);

	case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT :// 16
	case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER :// 22,
	case EFF_TARGET_IN_FRONT_OF_CASTER: //24,
	case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED: //28,
		{

			float r = spellInfo->base_range_or_radius_sqr;

			if( _Caster->IsPlayer() )
			{
				uint64 guidTarget = ((Player *)_Caster)->GetSelection();
				if(guidTarget == 0) return(SPELL_FAILED_BAD_IMPLICIT_TARGETS); //  "Vous n'avez pas de cible"
				Creature *unit = ((Player *)_Caster)->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guidTarget));

				if(!unit)              return(SPELL_FAILED_BAD_TARGETS); //  "Cible incorrect" "Cet objet n'est pas une cible autorisée"
				if(!unit->IsInWorld()) return(SPELL_FAILED_CANT_CAST_ON_TAPPED); // "Cible indisponible"

				Unit *selected = ((Player *)_Caster)->GetMapMgr()->GetUnit(((Player *)_Caster)->GetSelection());

				if(isAttackable(((Player *)_Caster),selected,!(spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)) && selected != ((Player *)_Caster))
					targets->m_target = unit;
			}
			else if( _Caster->IsUnit() )
			{
				if(	((Unit *)_Caster)->GetAIInterface()->GetNextTarget() &&
					isAttackable(((Unit *)_Caster),((Unit *)_Caster)->GetAIInterface()->GetNextTarget(),!(spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)) &&
					((Unit *)_Caster)->GetDistanceSq(((Unit *)_Caster)->GetAIInterface()->GetNextTarget()) <= r)
				{
					uint64 guidTarget = ((Unit *)_Caster)->GetAIInterface()->GetNextTarget()->GetGUID();
					if(guidTarget == 0) return(SPELL_FAILED_BAD_IMPLICIT_TARGETS); //  "Vous n'avez pas de cible"
					Creature *unit = ((Unit *)_Caster)->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guidTarget));

					if(!unit)              return(SPELL_FAILED_BAD_TARGETS); //  "Cible incorrect" "Cet objet n'est pas une cible autorisée"
					if(!unit->IsInWorld()) return(SPELL_FAILED_CANT_CAST_ON_TAPPED); // "Cible indisponible"

					targets->m_target = unit;
				}
				if(((Unit *)_Caster)->GetAIInterface()->getAITargetsCount())
				{
					//try to get most hated creature
					TargetMap *m_aiTargets = ((Unit *)_Caster)->GetAIInterface()->GetAITargets();
					TargetMap::iterator itr;
					for(itr = m_aiTargets->begin(); itr != m_aiTargets->end();itr++)
					{
						if( //m_caster->GetMapMgr()->GetUnit(itr->first->GetGUID()) &&// itr->first->GetMapMgr() == m_caster->GetMapMgr() && 
							itr->first->isAlive() &&
							((Unit *)_Caster)->GetDistanceSq(itr->first) <= r &&
							isAttackable(((Unit *)_Caster),itr->first,!(spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
						{
							uint64 guidTarget = itr->first->GetGUID();
							if(guidTarget == 0) return(SPELL_FAILED_BAD_IMPLICIT_TARGETS); //  "Vous n'avez pas de cible"
							Creature *unit = ((Unit *)_Caster)->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guidTarget));

							if(!unit)              return(SPELL_FAILED_BAD_TARGETS); //  "Cible incorrect" "Cet objet n'est pas une cible autorisée"
							if(!unit->IsInWorld()) return(SPELL_FAILED_CANT_CAST_ON_TAPPED); // "Cible indisponible"

							targets->m_target = unit;
							break;
						}
					}
				}
			}
		}
		return( SPELL_CANCAST_OK);
		
   case EFF_TARGET_AoF              : // 2
   case EFF_TARGET_TO_SELECT  		: // 25, // Meilleur nom, anciennement EFF_TARGET_DUEL (Branruz)
	   // La cible est celle que le player à selectionné
	   if(_Caster->IsPlayer())
	   {
	     uint64 guidTarget = ((Player *)_Caster)->GetSelection();
	     if(guidTarget == 0) return(SPELL_FAILED_BAD_IMPLICIT_TARGETS); //  "Vous n'avez pas de cible"
		 
		 Creature *unit = ((Player *)_Caster)->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guidTarget));
		 if(!unit)              return(SPELL_FAILED_BAD_TARGETS); //  "Cible incorrect" "Cet objet n'est pas une cible autorisée"
		 if(!unit->IsInWorld()) return(SPELL_FAILED_CANT_CAST_ON_TAPPED); // "Cible indisponible"
		 targets->m_target = unit;
	   }
	   return( SPELL_CANCAST_OK);

   case EFF_TARGET_GAMEOBJECT		 : // 23,
	   {
			if(_Caster->IsPlayer())
			{
				uint64 guidTarget = ((Player *)_Caster)->GetSelection();
		  
				if(guidTarget == 0) 
				{   // On cherche le GO le plus pret si ya pas de selection reel (click droit sur un item puis click sur le Gob)
					GameObject *GObj = NULL;

					std::set<Object*>::iterator Itr = ((Player *)_Caster)->GetInRangeSetBegin();
					std::set<Object*>::iterator Itr2 = ((Player *)_Caster)->GetInRangeSetEnd();
					float cDist = 9999.0f;
					float nDist = 0.0f;
					for( ; Itr != Itr2; Itr++ )
					{
						if( (*Itr)->GetTypeId() == TYPEID_GAMEOBJECT )
						{
							if( (nDist = ((Player *)_Caster)->CalcDistance( *Itr )) < cDist )
							{
								cDist = nDist;
								nDist = 0.0f;
								GObj = (GameObject*)(*Itr);
							}
						}
					}
					if(GObj == NULL) return(SPELL_FAILED_BAD_IMPLICIT_TARGETS); //  "Vous n'avez pas de cible"
						targets->m_target = GObj;			  
				}
				else // On prend la selection
				{
					GameObject *GObj = ((Player *)_Caster)->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guidTarget));
					if(GObj == NULL) return(SPELL_FAILED_BAD_IMPLICIT_TARGETS); //  "Vous n'avez pas de cible"
						targets->m_target = GObj;
				}
			}
	   }
	   return( SPELL_CANCAST_OK);

	case EFF_TARGET_SCRIPTED_GAMEOBJECT	:// 40,
	case EFF_TARGET_TOTEM_EARTH: //41,
	case EFF_TARGET_TOTEM_WATER: //42,
	case EFF_TARGET_TOTEM_AIR: //43,
	case EFF_TARGET_TOTEM_FIRE:// Totem
		{
			for(uint32 i=0;i < 3;i++)
			{
				if(spellInfo->EffectImplicitTargetB[i])
				{
					if( ((Player *)_Caster)->IsPlayer() )
					{
						SummonPropertiesEntry* summonprop = dbcSummonProperties.LookupEntryForced( spellInfo->EffectMiscValueB[i] );
						if(!summonprop) return(SPELL_FAILED_SUMMON_PENDING);
						uint32 slot = summonprop->slot;					

						if(((Player *)_Caster)->m_summonslot[slot] != 0)
						{
							uint64 guidTarget = ((Player *)_Caster)->m_summonslot[slot]->GetGUID();
							if(guidTarget == 0) return(SPELL_FAILED_BAD_IMPLICIT_TARGETS); //  "Vous n'avez pas de cible"
							Creature *unit = ((Player *)_Caster)->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guidTarget));

							if(!unit)              return(SPELL_FAILED_BAD_TARGETS); //  "Cible incorrect" "Cet objet n'est pas une cible autorisée"
							if(!unit->IsInWorld()) return(SPELL_FAILED_CANT_CAST_ON_TAPPED); // "Cible indisponible"
							targets->m_target = unit;
						}
					}					
				}	
			}
		}
		return( SPELL_CANCAST_OK);

    //----------- Non gerés ----------
   case EFF_TARGET_FRIENDLY : // 3, // this is wrong, its actually a friendly single target
   case EFF_TARGET_SINGLE_ENEMY		: // 6,
   case EFF_TARGET_SCRIPTED_TARGET	: // 7,
   case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS : // 8,
   case EFF_TARGET_HEARTSTONE_LOCATION		 : // 9,
   case EFF_TARGET_ALL_ENEMY_IN_AREA		 : // 15,
   // case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT : // 16,
   case EFF_TARGET_TELEPORT_LOCATION	    : // 17,
   case EFF_TARGET_LOCATION_TO_SUMMON	    : // 18,
   case EFF_TARGET_ALL_PARTY_AROUND_CASTER	: // 20,
   case EFF_TARGET_SINGLE_FRIEND			:// 21,  
   // case EFF_TARGET_IN_FRONT_OF_CASTER : // 24,
   case EFF_TARGET_GAMEOBJECT_ITEM	: // 26,
   case EFF_TARGET_PET_MASTER		: // 27,
   // case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED: // 28,
   case EFF_TARGET_ALL_PARTY_IN_AREA_CHANNELED:	// 29,
   case EFF_TARGET_ALL_FRIENDLY_IN_AREA	: // 30,
   case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS_OVER_TIME: // 31,
   case EFF_TARGET_MINION		: // 32,
   case EFF_TARGET_ALL_PARTY_IN_AREA: // 33,
   case EFF_TARGET_SINGLE_PARTY		: // 35,
   case EFF_TARGET_PET_SUMMON_LOCATION:	// 36,
   case EFF_TARGET_ALL_PARTY			://  37,
   case EFF_TARGET_SCRIPTED_OR_SINGLE_TARGET://  38,
   case EFF_TARGET_SELF_FISHING			://  39,
   // case EFF_TARGET_SCRIPTED_GAMEOBJECT	:// 40,
   // case EFF_TARGET_TOTEM_EARTH	:	//  41,
   // case EFF_TARGET_TOTEM_WATER	:	//  42,
   // case EFF_TARGET_TOTEM_AIR		://  43,
   // case EFF_TARGET_TOTEM_FIRE			://  44,
   case EFF_TARGET_CHAIN					://  45,
   case EFF_TARGET_SCIPTED_OBJECT_LOCATION	://  46,
   case EFF_TARGET_DYNAMIC_OBJECT				://  47,//not sure exactly where is used
   case EFF_TARGET_MULTIPLE_SUMMON_LOCATION		://  48,
   case EFF_TARGET_MULTIPLE_SUMMON_PET_LOCATION	://  49,
   case EFF_TARGET_SUMMON_LOCATION		://  50,
   case EFF_TARGET_CALIRI_EGS				://  51,
   case EFF_TARGET_LOCATION_NEAR_CASTER		://  52,
   case EFF_TARGET_CURRENT_SELECTION			://  53,
   case EFF_TARGET_TARGET_AT_ORIENTATION_TO_CASTER://  54,
//   case EFF_TARGET_LOCATION_INFRONT_CASTER	://  55,
   case EFF_TARGET_PARTY_MEMBER				://  57,
   case EFF_TARGET_TARGET_FOR_VISUAL_EFFECT	://  59,
   case EFF_TARGET_SCRIPTED_TARGET2			://  60,
   case EFF_TARGET_AREAEFFECT_PARTY_AND_CLASS://  61,
   case EFF_TARGET_NATURE_SUMMON_LOCATION	://  63, 
   case EFF_TARGET_BEHIND_TARGET_LOCATION		://  65, 
   case EFF_TARGET_MULTIPLE_GUARDIAN_SUMMON_LOCATION://  72,
   case EFF_TARGET_NETHETDRAKE_SUMMON_LOCATION		://  73,
   case EFF_TARGET_SCRIPTED_LOCATION				://  74,
   case EFF_TARGET_LOCATION_INFRONT_CASTER_AT_RANGE	://  75,
   case EFF_TARGET_ENEMYS_IN_ARE_CHANNELED_WITH_EXCEPTIONS ://  76,
   case EFF_TARGET_SELECTED_ENEMY_CHANNELED		://  77,
   case EFF_TARGET_SELECTED_ENEMY_DEADLY_POISON	://  86, 

   case EFF_TARGET_UNK80 : // 80, //targetted aoe summon
   case EFF_TARGET_UNK87 : // 87, //just seems to be a simple aoe target, with a little circle
   case EFF_TARGET_UNK88 : // 88, //even smaller aoe circle
   case EFF_TARGET_UNK90 : // 90, //target non-combat pet :P
   default : Log.Warning("[ConfirmTarget]","ImplicitTarget %d non defini.",_EffectImplicitTarget);
	   return( SPELL_CANCAST_OK);
 }
 return( SPELL_CANCAST_OK);
}
//----------------------------------------------------
