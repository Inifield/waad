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

#include "StdAfx.h"

/**
 * World PvP
 * Terokkar Forest
 */

// Zone ID
#define ZONE_TEROKKAR_FOREST									3519

#define WORLDSTATE_TEROKKAR_TOWER1_NEUTRAL						2681
#define WORLDSTATE_TEROKKAR_TOWER1_ALLIANCE						2683
#define WORLDSTATE_TEROKKAR_TOWER1_HORDE						2682

#define WORLDSTATE_TEROKKAR_TOWER2_NEUTRAL						2686
#define WORLDSTATE_TEROKKAR_TOWER2_ALLIANCE						2684
#define WORLDSTATE_TEROKKAR_TOWER2_HORDE						2685

#define WORLDSTATE_TEROKKAR_TOWER3_NEUTRAL						2690
#define WORLDSTATE_TEROKKAR_TOWER3_ALLIANCE						2688
#define WORLDSTATE_TEROKKAR_TOWER3_HORDE						2689

#define WORLDSTATE_TEROKKAR_TOWER4_NEUTRAL						2696
#define WORLDSTATE_TEROKKAR_TOWER4_ALLIANCE						2694
#define WORLDSTATE_TEROKKAR_TOWER4_HORDE						2695

#define WORLDSTATE_TEROKKAR_TOWER5_NEUTRAL						2693
#define WORLDSTATE_TEROKKAR_TOWER5_ALLIANCE						2691
#define WORLDSTATE_TEROKKAR_TOWER5_HORDE						2692

#define WORLDSTATE_TEROKKAR_ALLIANCE_TOWERS_CONTROLLED			2621
#define WORLDSTATE_TEROKKAR_HORDE_TOWERS_CONTROLLED				2622

#define WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_DISPLAY				2623
#define WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_VALUE				2625

#define WORLDSTATE_TEROKKAR_TOWER_DISPLAY						2620
//DELETE FROM gameobject_spawns WHERE `Map` = 530 AND `entry` in (183104, 183411, 182301, 183412, 183413, 183414);

#define TEROKKAR_FOREST_BANNER_RANGE 2000
#define TEROKKAR_FOREST_UPDATE_PERIOD 5000
#define TEROKKAR_FOREST_CAPTURE_RATE 20 // Prise du flag en 25sec max pour debug, Offi = Tick : 1 => #8.30 min ( #4.15min / #4.15min )

// Towers
enum Terokkar_Forest_Towers
{
	TEROKKAR_FOREST_TOWER_1, // North-west
	TEROKKAR_FOREST_TOWER_2, // North
	TEROKKAR_FOREST_TOWER_3, // North-east
	TEROKKAR_FOREST_TOWER_4, // South-east
	TEROKKAR_FOREST_TOWER_5, // South
	TEROKKAR_FOREST_TOWER_COUNT,
};

// Tower GameObject Ids
#define TEROKKAR_FOREST_G_TOWER_1 183104
#define TEROKKAR_FOREST_G_TOWER_2 183411
#define TEROKKAR_FOREST_G_TOWER_3 183412
#define TEROKKAR_FOREST_G_TOWER_4 183413
#define TEROKKAR_FOREST_G_TOWER_5 183414

#define BLESSING_OF_AUCHINDOUND 33377

// Owners of the towers, used for save/restore
int32 Terokkar_Forest_g_towerOwners[TEROKKAR_FOREST_TOWER_COUNT] = { -1, -1, -1, -1, -1 };

// global variables
uint32 Terokkar_Forest_g_hordeTowers = 0;
uint32 Terokkar_Forest_g_allianceTowers = 0;
int32 Terokkar_Forest_g_superiorTeam = -1;			// SUPERIORITY

// Fields to update visual view of the client map
static const uint32 Terokkar_Forest_g_hordeStateFields[5]		= {	WORLDSTATE_TEROKKAR_TOWER1_HORDE,	WORLDSTATE_TEROKKAR_TOWER2_HORDE,	WORLDSTATE_TEROKKAR_TOWER3_HORDE,	WORLDSTATE_TEROKKAR_TOWER4_HORDE,	WORLDSTATE_TEROKKAR_TOWER5_HORDE };
static const uint32 Terokkar_Forest_g_allianceStateFields[5]	= {	WORLDSTATE_TEROKKAR_TOWER1_ALLIANCE,WORLDSTATE_TEROKKAR_TOWER2_ALLIANCE,WORLDSTATE_TEROKKAR_TOWER3_ALLIANCE,WORLDSTATE_TEROKKAR_TOWER4_ALLIANCE,WORLDSTATE_TEROKKAR_TOWER5_ALLIANCE };
static const uint32 Terokkar_Forest_g_neutralStateFields[5]		= {	WORLDSTATE_TEROKKAR_TOWER1_NEUTRAL,	WORLDSTATE_TEROKKAR_TOWER2_NEUTRAL,	WORLDSTATE_TEROKKAR_TOWER3_NEUTRAL,	WORLDSTATE_TEROKKAR_TOWER4_NEUTRAL,	WORLDSTATE_TEROKKAR_TOWER5_NEUTRAL };

// updates clients visual counter, and adds the buffs to players if needed
ASCENT_INLINE void Terokkar_Forest_UpdateTowerCount(MapMgr *mgr)
{
	mgr->GetDBStateManager().UpdateWorldState(WORLDSTATE_TEROKKAR_ALLIANCE_TOWERS_CONTROLLED, Terokkar_Forest_g_allianceTowers);
	mgr->GetDBStateManager().UpdateWorldState(WORLDSTATE_TEROKKAR_HORDE_TOWERS_CONTROLLED, Terokkar_Forest_g_hordeTowers);

	if( (Terokkar_Forest_g_superiorTeam == FACTION_ALLY) && (Terokkar_Forest_g_allianceTowers != 5) )
	{
		mgr->RemovePositiveAuraFromPlayers(0, BLESSING_OF_AUCHINDOUND);
		Terokkar_Forest_g_superiorTeam = FACTION_NEUTRAL;
	}

	if( (Terokkar_Forest_g_superiorTeam == FACTION_HORDE) && (Terokkar_Forest_g_hordeTowers != 5) )
	{
		mgr->RemovePositiveAuraFromPlayers(1, BLESSING_OF_AUCHINDOUND);
		Terokkar_Forest_g_superiorTeam = FACTION_NEUTRAL;
	}

	if( (Terokkar_Forest_g_allianceTowers == 5) && (Terokkar_Forest_g_superiorTeam != FACTION_ALLY) )
	{
		Terokkar_Forest_g_superiorTeam = FACTION_ALLY;
		mgr->CastSpellOnPlayers(0, BLESSING_OF_AUCHINDOUND);
	}

	if( (Terokkar_Forest_g_hordeTowers == 5) && (Terokkar_Forest_g_superiorTeam != FACTION_HORDE) )
	{
		Terokkar_Forest_g_superiorTeam = FACTION_HORDE;
		mgr->CastSpellOnPlayers(1, BLESSING_OF_AUCHINDOUND);
	}
}

class TerokkarForestBannerAI : public GameObjectAIScript
{
	map<uint32, uint32> StoredPlayers;
	uint32 Status;
	const char * ControlPointName;
	uint32 towerid;
	uint32 m_bannerStatus;

public:

	TerokkarForestBannerAI(GameObject *go) : GameObjectAIScript(go)
	{
		m_bannerStatus = PVPZONE_BANNER_STATUS_NEUTRAL;
		Status = 50;

		switch(go->GetEntry())
		{
		case TEROKKAR_FOREST_G_TOWER_1:
			ControlPointName = "tour Nord-Ouest";
			towerid = TEROKKAR_FOREST_TOWER_1;
			break;

		case TEROKKAR_FOREST_G_TOWER_2:
			ControlPointName = "tour Nord";
			towerid = TEROKKAR_FOREST_TOWER_2;
			break;

		case TEROKKAR_FOREST_G_TOWER_3:
			ControlPointName = "tour Nord-Est";
			towerid = TEROKKAR_FOREST_TOWER_3;
			break;

		case TEROKKAR_FOREST_G_TOWER_4:
			ControlPointName = "tour Sud-Est";
			towerid = TEROKKAR_FOREST_TOWER_4;
			break;

		case TEROKKAR_FOREST_G_TOWER_5:
			ControlPointName = "tour Sud";
			towerid = TEROKKAR_FOREST_TOWER_5;
			break;

		default:
			ControlPointName = "<Unknown>";
			break;
		}
	}

	void AIUpdate()
	{
		uint32 plrcounts[2] = { 0, 0 };

		// details:
		//   loop through inrange players, for new ones, send the enable CP worldstate.
		//   the value of the map is a timestamp of the last update, to avoid cpu time wasted
		//   doing lookups of objects that have already been updated

		set<Player *>::iterator itr = _gameobject->GetInRangePlayerSetBegin();		
		set<Player *>::iterator itrend = _gameobject->GetInRangePlayerSetEnd();
		map<uint32,uint32>::iterator it2, it3;
		uint32 timeptr = (uint32)UNIXTIME;
		bool in_range;
		bool is_valid;
		Player *plr;
		
		for(; itr != itrend; ++itr)
		{
			if( !(*itr)->IsPvPFlagged() || (*itr)->IsStealth() )
				is_valid = false;
			else
				is_valid = true;

			in_range = (_gameobject->GetDistanceSq((*itr)) <= TEROKKAR_FOREST_BANNER_RANGE) ? true : false;

			it2 = StoredPlayers.find((*itr)->GetLowGUID());
			if( it2 == StoredPlayers.end() )
			{
				// new player :)
				if( in_range )
				{
					_gameobject->GetMapMgr()->GetDBStateManager().SendWorldStates((*itr)); // Init WorldStates sur l'ecran du player
					(*itr)->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_DISPLAY, 1);
					(*itr)->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_TOWER_DISPLAY, 1);
					(*itr)->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_VALUE, Status);
					StoredPlayers.insert(make_pair((*itr)->GetLowGUID(), timeptr));

					if( is_valid )
						plrcounts[(*itr)->GetTeam()]++;
				}
			}
			else
			{
				// oldie
				if( !in_range )
				{
					(*itr)->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_DISPLAY, 0);
					(*itr)->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_TOWER_DISPLAY, 0);
					StoredPlayers.erase(it2);
				}
				else
				{
					(*itr)->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_VALUE, Status);
					it2->second = timeptr;
					if( is_valid )
						plrcounts[(*itr)->GetTeam()]++;
				}
			}
		}

		// handle stuff for the last tick
		if( Status == 100 && m_bannerStatus != PVPZONE_BANNER_STATUS_ALLIANCE )
		{
			m_bannerStatus = PVPZONE_BANNER_STATUS_ALLIANCE;
			SetArtKit();

			// send message to everyone in the zone, has been captured by the Alliance
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_TEROKKAR_FOREST, ZONE_TEROKKAR_FOREST, "|cffffff00La %s a ete prise par l'Alliance!|r", ControlPointName);

			// tower update
			Terokkar_Forest_g_allianceTowers++;
			Terokkar_Forest_UpdateTowerCount(_gameobject->GetMapMgr());

			// state update
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Terokkar_Forest_g_neutralStateFields[towerid], 0);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Terokkar_Forest_g_allianceStateFields[towerid], 1);

			// woot
			Terokkar_Forest_g_towerOwners[towerid] = 1;
			UpdateInDB();
		}
		else if( Status == 0 && m_bannerStatus != PVPZONE_BANNER_STATUS_HORDE )
		{
			m_bannerStatus = PVPZONE_BANNER_STATUS_HORDE;
			SetArtKit();

			// send message to everyone in the zone, has been captured by the Horde
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_TEROKKAR_FOREST, ZONE_TEROKKAR_FOREST, "|cffffff00La %s a ete prise par la Horde!|r", ControlPointName);
			
			// tower update
			Terokkar_Forest_g_hordeTowers++;
			Terokkar_Forest_UpdateTowerCount(_gameobject->GetMapMgr());

			// state update
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Terokkar_Forest_g_neutralStateFields[towerid], 0);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Terokkar_Forest_g_hordeStateFields[towerid], 1);

			// woot
			Terokkar_Forest_g_towerOwners[towerid] = 0;
			UpdateInDB();
		}
		else if( m_bannerStatus != PVPZONE_BANNER_STATUS_NEUTRAL )
		{
			// if the difference for the faction is >50, change to neutral
			if( m_bannerStatus == PVPZONE_BANNER_STATUS_ALLIANCE && Status <= 50 )
			{
				// send message: The Alliance has lost control of point xxx
				m_bannerStatus = PVPZONE_BANNER_STATUS_NEUTRAL;
				SetArtKit();
				
				Terokkar_Forest_g_allianceTowers--;
				Terokkar_Forest_UpdateTowerCount(_gameobject->GetMapMgr());

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_TEROKKAR_FOREST, ZONE_TEROKKAR_FOREST, "|cffffff00L'Alliance a perdu le controle de la %s!|r", ControlPointName);

				// state update
				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Terokkar_Forest_g_allianceStateFields[towerid], 0);
				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Terokkar_Forest_g_neutralStateFields[towerid], 1);

				// woot
				Terokkar_Forest_g_towerOwners[towerid] = FACTION_NEUTRAL;
				UpdateInDB();
			}
			else if( m_bannerStatus == PVPZONE_BANNER_STATUS_HORDE && Status >= 50 )
			{
				// send message: The Alliance has lost control of point xxx
				m_bannerStatus = PVPZONE_BANNER_STATUS_NEUTRAL;
				SetArtKit();

				Terokkar_Forest_g_hordeTowers--;
				Terokkar_Forest_UpdateTowerCount(_gameobject->GetMapMgr());

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_TEROKKAR_FOREST, ZONE_TEROKKAR_FOREST, "|cffffff00La Horde a perdu le controle de la %s!|r", ControlPointName);

				// state update
				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Terokkar_Forest_g_hordeStateFields[towerid], 0);
				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Terokkar_Forest_g_neutralStateFields[towerid], 1);

				// woot
				Terokkar_Forest_g_towerOwners[towerid] = FACTION_NEUTRAL;
				UpdateInDB();
			}
		}

		// send any out of range players the disable flag
		for(it2 = StoredPlayers.begin(); it2 != StoredPlayers.end(); )
		{
			it3 = it2;
			++it2;

			if( it3->second != timeptr )
			{
				plr = _gameobject->GetMapMgr()->GetPlayer(it3->first);
				
				// they WILL be out of range at this point. this is guaranteed. means they left the set rly quickly.
				if( plr )
				{
					plr->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_PVP_CAPTURE_BAR_DISPLAY, 0);
					plr->SendWorldStateUpdate(WORLDSTATE_TEROKKAR_TOWER_DISPLAY, 0);
				}

				StoredPlayers.erase(it3);
			}
		}

		// work out current status for next tick
		uint32 delta;
		if( plrcounts[0] > plrcounts[1] )
		{
			delta = plrcounts[0] - plrcounts[1];
			delta *= TEROKKAR_FOREST_CAPTURE_RATE;

			// cap it at 25 so the banner always gets removed.
			if( delta > 25 )
				delta = 25;

			Status += delta;
			if( Status >= 100 )
				Status = 100;
		}
		else if( plrcounts[1] > plrcounts[0] )
		{
			delta = plrcounts[1] - plrcounts[0];
			delta *= TEROKKAR_FOREST_CAPTURE_RATE;
			
			// cap it at 25 so the banner always gets removed.
			if( delta > 25 )
				delta = 25;

			if( delta > Status )
				Status = 0;
			else
				Status -= delta;
		}
	}

	void SetArtKit()
	{                                            // N   A  H   
		static const uint32 artkits_flagpole[3] = { 21, 2, 1 };
//		_gameobject->SetUInt32Value(GAMEOBJECT_ARTKIT, artkits_flagpole[m_bannerStatus]);
		_gameobject->SetGoArtKit(artkits_flagpole[m_bannerStatus]);
	}

	void OnSpawn()
	{
		m_bannerStatus = PVPZONE_BANNER_STATUS_NEUTRAL;

		// preloaded data, do we have any?
		if( Terokkar_Forest_g_towerOwners[towerid] == FACTION_HORDE )
		{
			m_bannerStatus = PVPZONE_BANNER_STATUS_HORDE;
			Status = 0;

			// state update
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Terokkar_Forest_g_hordeStateFields[towerid], 1);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Terokkar_Forest_g_neutralStateFields[towerid], 0);

			// countz
			Terokkar_Forest_g_hordeTowers++;
			Terokkar_Forest_UpdateTowerCount(_gameobject->GetMapMgr());
			SetArtKit();
		}
		else if( Terokkar_Forest_g_towerOwners[towerid] == FACTION_ALLY )
		{
			m_bannerStatus = PVPZONE_BANNER_STATUS_ALLIANCE;
			Status = 100;

			// state update
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Terokkar_Forest_g_allianceStateFields[towerid], 1);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Terokkar_Forest_g_neutralStateFields[towerid], 0);

			// countz
			Terokkar_Forest_g_allianceTowers++;
			Terokkar_Forest_UpdateTowerCount(_gameobject->GetMapMgr());
			SetArtKit();
		}

		// start the event timer
		RegisterAIUpdateEvent(TEROKKAR_FOREST_UPDATE_PERIOD);
	}

	//////////////////////////////////////////////////////////////////////////
	// Save Data To DB
	//////////////////////////////////////////////////////////////////////////
	void UpdateInDB()
	{
		static const char * fieldnames[TEROKKAR_FOREST_TOWER_COUNT] = { "Terokkar-Tower1-status", "Terokkar-Tower2-status", "Terokkar-Tower3-status", "Terokkar-Tower4-status", "Terokkar-Tower5-status" };
		const char * msg = "-1";
		if( Status == 100 )
			msg = "0";
		else
			msg = "1";
			
		WorldStateManager::SetPersistantSetting(fieldnames[towerid], msg);
	}

	//////////////////////////////////////////////////////////////////////////
	// Zone Hook
	//////////////////////////////////////////////////////////////////////////

	void OnZoneHook(Player *plr, uint32 Zone, uint32 OldZone)
	{
		if(!plr) return;

		if( Zone == ZONE_TEROKKAR_FOREST )
		{
			if( Terokkar_Forest_g_superiorTeam == plr->GetTeam() )
				plr->CastSpell(plr, dbcSpell.LookupEntry(BLESSING_OF_AUCHINDOUND), true);
		}
		else if( OldZone == ZONE_TEROKKAR_FOREST )
		{
			if( Terokkar_Forest_g_superiorTeam == plr->GetTeam() )
				plr->RemovePositiveAura(BLESSING_OF_AUCHINDOUND);
		}
	}


};

//////////////////////////////////////////////////////////////////////////
// Object Spawn Data
//////////////////////////////////////////////////////////////////////////

void TFSpawnObjects(MapMgr *pmgr)
{
	if(!pmgr || pmgr->GetMapId() != 530)
		return;

	const static PvPZone_sgodata godata[] = 
//         entry       X      Y         Z     facing   O1 O2     O3       O4      state flag faction scale is_banner        
	{
		{ 183104,-3081.65f,5335.03f,17.1853f,-2.14675f, 0, 0, 0.878817f,-0.477159f, 1  , 32  , 0    ,  1 ,   0},
		{ 183411,-2939.9f ,4788.73f,18.987f ,2.77507f , 0, 0, 0.983255f,0.182236f , 1  , 32  , 0    ,  1 ,   0},
		{ 183412,-3174.94f,4440.97f,16.2281f,1.86750f , 0, 0, 0.803857f,0.594823f , 1  , 32  , 0    ,  1 ,   0},
		{ 183413,-3603.31f,4529.15f,20.9077f,0.994838f, 0, 0, 0.477159f,0.878817f , 1  , 32  , 0    ,  1 ,   0},
		{ 183414,-3812.37f,4899.3f,17.7249f ,0.087266f, 0, 0, 0.043619f,0.999048f , 1  , 32  , 0    ,  1 ,   0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ,
	};

	uint32 i;
	const PvPZone_sgodata *p;
	for(i = 0; i < 5; ++i)
	{
		p = &godata[i];

		GameObject *pGo = NULL;
		pGo = pmgr->GetInterface()->SpawnGameObject(p->entry, p->posx, p->posy, p->posz, p->facing, false, 0, 0, 1);
		if( !pGo )
			continue;

		pGo->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, p->state);
		pGo->SetUInt32Value(GAMEOBJECT_FLAGS, p->flags);
		pGo->SetUInt32Value(GAMEOBJECT_FACTION, p->faction);

		for(uint32 j = 0; j < 4; ++j)
		{
			pGo->SetFloatValue(GAMEOBJECT_PARENTROTATION+j, p->orientation[j]);
		}

		// now make his script
		pGo->SetScript(new TerokkarForestBannerAI(pGo));

		pGo->PushToWorld(pmgr);

		pGo->GetScript()->OnSpawn();
	}
}

void SetupPvP_TerokkarForest(MapMgr *mgr)
{
	// register instance hooker: Ne pas oublier le OnHook(...)
	
	// load data
	const string Tower1 = WorldStateManager::GetPersistantSetting("Terokkar-Tower1-status", "-1");
	const string Tower2 = WorldStateManager::GetPersistantSetting("Terokkar-Tower2-status", "-1");
	const string Tower3 = WorldStateManager::GetPersistantSetting("Terokkar-Tower3-status", "-1");
	const string Tower4 = WorldStateManager::GetPersistantSetting("Terokkar-Tower4-status", "-1");
	const string Tower5 = WorldStateManager::GetPersistantSetting("Terokkar-Tower5-status", "-1");

	Terokkar_Forest_g_towerOwners[TEROKKAR_FOREST_TOWER_1] = atoi(Tower1.c_str());
	Terokkar_Forest_g_towerOwners[TEROKKAR_FOREST_TOWER_2] = atoi(Tower2.c_str());
	Terokkar_Forest_g_towerOwners[TEROKKAR_FOREST_TOWER_3] = atoi(Tower3.c_str());
	Terokkar_Forest_g_towerOwners[TEROKKAR_FOREST_TOWER_4] = atoi(Tower4.c_str());
	Terokkar_Forest_g_towerOwners[TEROKKAR_FOREST_TOWER_5] = atoi(Tower5.c_str());

	TFSpawnObjects(mgr);
}


