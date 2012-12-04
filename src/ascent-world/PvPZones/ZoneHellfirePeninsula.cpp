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

// Tower Counter Value
#define WORLDSTATE_HELLFIRE_ALLIANCE_TOWERS_CONTROLLED			2476
#define WORLDSTATE_HELLFIRE_HORDE_TOWERS_CONTROLLED				2478

// Tower Counter Display
#define WORLDSTATE_HELLFIRE_TOWER_DISPLAY_ALLIANCE				2490
#define WORLDSTATE_HELLFIRE_TOWER_DISPLAY_HORDE					2489

// PvP Capture Bar
#define WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_DISPLAY				2473
#define WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_VALUE				2474

// The Stadium Control Point
#define WORLDSTATE_HELLFIRE_STADIUM_NEUTRAL						2472
#define WORLDSTATE_HELLFIRE_STADIUM_ALLIANCE					2471
#define WORLDSTATE_HELLFIRE_STADIUM_HORDE						2470

// The Overlook Control Point
#define WORLDSTATE_HELLFIRE_OVERLOOK_NEUTRAL					2482
#define WORLDSTATE_HELLFIRE_OVERLOOK_ALLIANCE					2480
#define WORLDSTATE_HELLFIRE_OVERLOOK_HORDE						2481

// Broken Hill Control Point
#define WORLDSTATE_HELLFIRE_BROKENHILL_NEUTRAL					2485
#define WORLDSTATE_HELLFIRE_BROKENHILL_ALLIANCE					2483
#define WORLDSTATE_HELLFIRE_BROKENHILL_HORDE					2484

// Zone ID, used in many places.
#ifndef ZONE_HELLFIRE_PENINSULA	
#define ZONE_HELLFIRE_PENINSULA									3483
#endif

// Some customizable defines.
// Maybe move these to config?

#define HELLFIRE_PENINSULA_BANNER_RANGE 900
#define HELLFIRE_PENINSULA_UPDATE_PERIOD 5000
#define HELLFIRE_PENINSULA_CAPTURE_RATE 20

// Towers
enum HellFirePeninsula_Towers
{
	HELLFIRE_PENINSULA_TOWER_STADIUM,
	HELLFIRE_PENINSULA_TOWER_OVERLOOK,
	HELLFIRE_PENINSULA_TOWER_BROKENHILL,
	HELLFIRE_PENINSULA_TOWER_COUNT,
};

// Tower GameObject Ids
#define HELLFIRE_PENINSULA_TOWER_WEST 182173
#define HELLFIRE_PENINSULA_TOWER_NORTH 182174
#define HELLFIRE_PENINSULA_TOWER_SOUTH 182175

// Buff Ids
#define HELLFIRE_SUPERORITY_ALLIANCE 32071
#define HELLFIRE_SUPERORITY_HORDE 32049

// cap it at 25 so the banner always gets removed.
#define HELLFIRE_PENINSULA_DELTA_TICK	25   // AIUpdate rafraichi ttes les 5secs pour debug, Prise = 25sec, sur Offi = #10min/ Ticks=1 donne #8min30

// Owners of the towers, used for save/restore
int32 HellFirePeninsula_g_towerOwners[HELLFIRE_PENINSULA_TOWER_COUNT] = { -1, -1, -1 };

// global variables
uint32 HellFirePeninsula_g_hordeTowers = 0;
uint32 HellFirePeninsula_g_allianceTowers = 0;
int32 HellFirePeninsula_g_superiorTeam = -1;			// SUPERIORITY

// Fields to update visual view of the client map
static const uint32 HellFirePeninsula_g_hordeStateFields[3] = { WORLDSTATE_HELLFIRE_STADIUM_HORDE, WORLDSTATE_HELLFIRE_OVERLOOK_HORDE, WORLDSTATE_HELLFIRE_BROKENHILL_HORDE };
static const uint32 HellFirePeninsula_g_allianceStateFields[3] = { WORLDSTATE_HELLFIRE_STADIUM_ALLIANCE, WORLDSTATE_HELLFIRE_OVERLOOK_ALLIANCE, WORLDSTATE_HELLFIRE_BROKENHILL_ALLIANCE };
static const uint32 HellFirePeninsula_g_neutralStateFields[3] = { WORLDSTATE_HELLFIRE_STADIUM_NEUTRAL, WORLDSTATE_HELLFIRE_OVERLOOK_NEUTRAL, WORLDSTATE_HELLFIRE_BROKENHILL_NEUTRAL };

// updates clients visual counter, and adds the buffs to players if needed
ASCENT_INLINE void HellFirePeninsula_UpdateTowerCount(MapMgr *mgr)
{
	if(!mgr)
		return;

	mgr->GetDBStateManager().UpdateWorldState(WORLDSTATE_HELLFIRE_ALLIANCE_TOWERS_CONTROLLED, HellFirePeninsula_g_allianceTowers);
	mgr->GetDBStateManager().UpdateWorldState(WORLDSTATE_HELLFIRE_HORDE_TOWERS_CONTROLLED, HellFirePeninsula_g_hordeTowers);

	if( HellFirePeninsula_g_superiorTeam == 0 && HellFirePeninsula_g_allianceTowers != 3 )
	{
		mgr->RemovePositiveAuraFromPlayers(0, HELLFIRE_SUPERORITY_ALLIANCE);
		HellFirePeninsula_g_superiorTeam = -1;
	}

	if( HellFirePeninsula_g_superiorTeam == 1 && HellFirePeninsula_g_hordeTowers != 3 )
	{
		mgr->RemovePositiveAuraFromPlayers(1, HELLFIRE_SUPERORITY_HORDE);
		HellFirePeninsula_g_superiorTeam = -1;
	}

	if( HellFirePeninsula_g_allianceTowers == 3 && HellFirePeninsula_g_superiorTeam != 0 )
	{
		HellFirePeninsula_g_superiorTeam = 0;
		mgr->CastSpellOnPlayers(0, HELLFIRE_SUPERORITY_ALLIANCE);
	}

	if( HellFirePeninsula_g_hordeTowers == 3 && HellFirePeninsula_g_superiorTeam != 1 )
	{
		HellFirePeninsula_g_superiorTeam = 1;
		mgr->CastSpellOnPlayers(1, HELLFIRE_SUPERORITY_HORDE);
	}
}

// Dans GameObjectArtKit.dbc
static const uint32 HellfirePeninsula_artkits_towers[3][3] = 
{
//  Neutre Ally Horde
	{ 69,   67,  68 }, // Le Stade
	{ 63,   62,  61 }, // Le surplond
	{ 66,   65,  64 }, // La Colline Brisée
};


// flag poles                                                 N   A  H
static const uint32 HellfirePeninsula_artkits_flagpole[3] = { 21, 2, 1 };
//----------------------------

class HellfirePeninsulaBannerAI : public GameObjectAIScript
{
	map<uint32, uint32> StoredPlayers;
	uint32 Status;
	const char * ControlPointName;
	uint32 towerid;
	uint32 m_bannerStatus;

public:
	GameObject *pBanner;

	static GameObjectAIScript *Create(GameObject * GO) 
	{ 
		return new HellfirePeninsulaBannerAI(GO); 
	}

	HellfirePeninsulaBannerAI(GameObject *go) : GameObjectAIScript(go)
	{
		m_bannerStatus = PVPZONE_BANNER_STATUS_NEUTRAL;
		Status = 50;

		switch(go->GetEntry())
		{
		case HELLFIRE_PENINSULA_TOWER_WEST:
			ControlPointName = "stade";
			towerid = HELLFIRE_PENINSULA_TOWER_STADIUM;
			break;

		case HELLFIRE_PENINSULA_TOWER_NORTH:
			ControlPointName = "surplond";
			towerid = HELLFIRE_PENINSULA_TOWER_OVERLOOK;
			break;

		case HELLFIRE_PENINSULA_TOWER_SOUTH:
			ControlPointName = "colline brisee";
			towerid = HELLFIRE_PENINSULA_TOWER_BROKENHILL;
			break;

		default:
			ControlPointName = "<Unknown>";
			break;
		}
	}
    /*
	void OnActivatePosition(Player * pPlayer)
	{
		
		// Deja init qd le player entre dans la zone
		switch(towerid)
		{
         case HELLFIRE_PENINSULA_TOWER_STADIUM: 
		 case HELLFIRE_PENINSULA_TOWER_BROKENHILL:
		 case HELLFIRE_PENINSULA_TOWER_OVERLOOK:
			 Log.Success("[Zone PvP]","%s est %s %s",pPlayer->GetName(),(ControlPointName[0]=='s') ? "au":"a la",ControlPointName);
			                 break;

		 default: Log.Error("[Zone PvP]","(%s) Peninsule des Flammes infernales, Tour inconnu!",pPlayer->GetName());
			      break;
		}
		
	}*/

	//////////////////////////////////////////////////////////////////////////
    // Zone Hook
    //////////////////////////////////////////////////////////////////////////

    void OnZoneHook(Player *pPlayer, uint32 Zone, uint32 OldZone)
    {
	 static uint32 spellids[2] = { HELLFIRE_SUPERORITY_ALLIANCE, HELLFIRE_SUPERORITY_HORDE };
	 if( Zone == ZONE_HELLFIRE_PENINSULA )
	 {
		if( HellFirePeninsula_g_superiorTeam == pPlayer->GetTeam() )
			pPlayer->CastSpell(pPlayer, dbcSpell.LookupEntry(spellids[pPlayer->GetTeam()]), true);
	 }
	 else if( OldZone == ZONE_HELLFIRE_PENINSULA )
	 {
		if( HellFirePeninsula_g_superiorTeam == pPlayer->GetTeam() )
			pPlayer->RemovePositiveAura(spellids[pPlayer->GetTeam()]);
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
		Player *plr = NULL;
		
        //Log.Success("[HellFirePenninsula]","AIUpdate begin");

		for(; itr != itrend; ++itr)
		{
			if( !(*itr)->IsPvPFlagged() || (*itr)->IsStealth() )
				is_valid = false;
			else
				is_valid = true;

			in_range = (_gameobject->GetDistanceSq((*itr)) <= HELLFIRE_PENINSULA_BANNER_RANGE) ? true : false;

			it2 = StoredPlayers.find((*itr)->GetLowGUID());
			if( it2 == StoredPlayers.end() )
			{
				// new player :)
				if( in_range )
				{
					Log.Success("[HellFirePenninsula]","New Player %s in range of %s",(*itr)->GetName(),ControlPointName);
					_gameobject->GetMapMgr()->GetDBStateManager().SendWorldStates((*itr)); // Init WorldStates sur l'ecran du player
					(*itr)->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_DISPLAY, 1);
					(*itr)->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_VALUE, Status);
					StoredPlayers.insert(make_pair((*itr)->GetLowGUID(), timeptr));

					if( is_valid )
					{
						plrcounts[(*itr)->GetTeam()]++;
					}
				}
			}
			else
			{
				// oldie
				if( !in_range )
				{
					Log.Success("[HellFirePenninsula]","Player %s not in range of %s",(*itr)->GetName(),ControlPointName);
					(*itr)->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_DISPLAY, 0); // Barre de capture
					(*itr)->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_TOWER_DISPLAY_ALLIANCE, 0);  // Les tours
					(*itr)->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_TOWER_DISPLAY_HORDE, 0);
					//_gameobject->GetMapMgr()->GetDBStateManager().ClearWorldStates((*itr)); // Global sur toute la zone
					StoredPlayers.erase(it2);
				}
				else
				{
					Log.Success("[HellFirePenninsula]","Player %s in range of %s",(*itr)->GetName(),ControlPointName);
					(*itr)->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_VALUE, Status);
					it2->second = timeptr;
					if( is_valid )
					{
						plrcounts[(*itr)->GetTeam()]++;

					}
				}
			}
		}

		// handle stuff for the last tick
		if( Status == 100 && (m_bannerStatus != PVPZONE_BANNER_STATUS_ALLIANCE) )
		{
			m_bannerStatus = PVPZONE_BANNER_STATUS_ALLIANCE;
			SetArtKit();

			// send message to everyone in the zone, has been captured by the Alliance
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_HELLFIRE_PENINSULA, ZONE_HELLFIRE_PENINSULA, "|cffffff00L%c %s a ete pris%spar l'Alliance!|r", 
				(ControlPointName[0]=='c') ? 'a':'e', ControlPointName,(ControlPointName[0]=='c') ? "e ":" ");

			// tower update
			HellFirePeninsula_g_allianceTowers++;
			HellFirePeninsula_UpdateTowerCount(_gameobject->GetMapMgr());

			// state update
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(HellFirePeninsula_g_neutralStateFields[towerid], 0);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(HellFirePeninsula_g_allianceStateFields[towerid], 1);

			// woot
			HellFirePeninsula_g_towerOwners[towerid] = FACTION_ALLY; 
			UpdateInDB();
		}
		else if( Status == 0 && (m_bannerStatus != PVPZONE_BANNER_STATUS_HORDE) )
		{
			m_bannerStatus = PVPZONE_BANNER_STATUS_HORDE;
			SetArtKit();

			// send message to everyone in the zone, has been captured by the Horde
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_HELLFIRE_PENINSULA, ZONE_HELLFIRE_PENINSULA, "|cffffff00L%c %s a ete pris%spar la Horde!|r", 
				                                         (ControlPointName[0]=='c') ? 'a':'e', ControlPointName,(ControlPointName[0]=='c') ? "e ":" ");
			
			// tower update
			HellFirePeninsula_g_hordeTowers++;
			HellFirePeninsula_UpdateTowerCount(_gameobject->GetMapMgr());

			// state update
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(HellFirePeninsula_g_neutralStateFields[towerid], 0);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(HellFirePeninsula_g_hordeStateFields[towerid], 1);

			// woot
			HellFirePeninsula_g_towerOwners[towerid] = FACTION_HORDE; 
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
				
				HellFirePeninsula_g_allianceTowers--;
				HellFirePeninsula_UpdateTowerCount(_gameobject->GetMapMgr());

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_HELLFIRE_PENINSULA, ZONE_HELLFIRE_PENINSULA, "|cffffff00L'Alliance a perdu le controle %s %s!|r", 
					(ControlPointName[0]=='c') ? "de la":"du",ControlPointName);

				// state update
				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(HellFirePeninsula_g_allianceStateFields[towerid], 0);
				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(HellFirePeninsula_g_neutralStateFields[towerid], 1);

				// woot
				HellFirePeninsula_g_towerOwners[towerid] = FACTION_NEUTRAL;
				UpdateInDB();
			}
			else if( m_bannerStatus == PVPZONE_BANNER_STATUS_HORDE && Status >= 50 )
			{
				// send message: The Alliance has lost control of point xxx
				m_bannerStatus = PVPZONE_BANNER_STATUS_NEUTRAL;
				SetArtKit();

				HellFirePeninsula_g_hordeTowers--;
				HellFirePeninsula_UpdateTowerCount(_gameobject->GetMapMgr());

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_HELLFIRE_PENINSULA, ZONE_HELLFIRE_PENINSULA, "|cffffff00La Horde a perdu le controle %s %s!|r", 
					                                           (ControlPointName[0]=='c') ? "de la":"du",ControlPointName);

				// state update
				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(HellFirePeninsula_g_hordeStateFields[towerid], 0);
				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(HellFirePeninsula_g_neutralStateFields[towerid], 1);

				// woot
				HellFirePeninsula_g_towerOwners[towerid] = FACTION_NEUTRAL;
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
				if( plr != NULL )
				{
					plr->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_PVP_CAPTURE_BAR_DISPLAY, 0);
					plr->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_TOWER_DISPLAY_ALLIANCE, 0);
					plr->SendWorldStateUpdate(WORLDSTATE_HELLFIRE_TOWER_DISPLAY_HORDE, 0);
				}

				StoredPlayers.erase(it3);
			}
		}

		// work out current status for next tick
		uint32 delta;
		if( plrcounts[FACTION_ALLY] > plrcounts[FACTION_HORDE] )
		{
			delta = plrcounts[FACTION_ALLY] - plrcounts[FACTION_HORDE];
			delta *= HELLFIRE_PENINSULA_CAPTURE_RATE;

			// cap it at 25 so the banner always gets removed.
			if( delta > HELLFIRE_PENINSULA_DELTA_TICK ) delta = HELLFIRE_PENINSULA_DELTA_TICK;

			Status += delta;
			if( Status >= 100 )
				Status = 100;
		}
		else if( plrcounts[FACTION_HORDE] > plrcounts[FACTION_ALLY] )
		{
			delta = plrcounts[FACTION_HORDE] - plrcounts[FACTION_ALLY];
			delta *= HELLFIRE_PENINSULA_CAPTURE_RATE;
			
			// cap it at 25 so the banner always gets removed.
			if( delta > HELLFIRE_PENINSULA_DELTA_TICK ) delta = HELLFIRE_PENINSULA_DELTA_TICK;

			if( delta > Status )
				Status = 0;
			else
				Status -= delta;
		}

		//Log.Success("[HellFirePenninsula]","AIUpdate end.");
	}

	void SetArtKit()
	{
		// Mise en place des drapeaux
		_gameobject->SetGoArtKit(HellfirePeninsula_artkits_flagpole[m_bannerStatus]);
		pBanner->SetGoArtKit(HellfirePeninsula_artkits_towers[towerid][m_bannerStatus]);
	}

	void OnSpawn()
	{
		m_bannerStatus = PVPZONE_BANNER_STATUS_NEUTRAL;

		// preloaded data, do we have any?
		if( HellFirePeninsula_g_towerOwners[towerid] == FACTION_HORDE )
		{
			m_bannerStatus = PVPZONE_BANNER_STATUS_HORDE;
			Status = 0;

			// state update
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(HellFirePeninsula_g_hordeStateFields[towerid], 1);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(HellFirePeninsula_g_neutralStateFields[towerid], 0);

			// countz
			HellFirePeninsula_g_hordeTowers++;
			HellFirePeninsula_UpdateTowerCount(_gameobject->GetMapMgr());
			SetArtKit();
			
		}
		else if( HellFirePeninsula_g_towerOwners[towerid] == FACTION_ALLY )
		{
			m_bannerStatus = PVPZONE_BANNER_STATUS_ALLIANCE;
			Status = 100;

			// state update
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(HellFirePeninsula_g_allianceStateFields[towerid], 1);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(HellFirePeninsula_g_neutralStateFields[towerid], 0);

			// countz
			HellFirePeninsula_g_allianceTowers++;
			HellFirePeninsula_UpdateTowerCount(_gameobject->GetMapMgr());
			SetArtKit();
			
		}
		
		//Log.Success("[HellFirePenninsula]","Nb de Tour Alliance : %u", HellFirePeninsula_g_allianceTowers);
		//Log.Success("[HellFirePenninsula]","Nb de Tour Horde    : %u", HellFirePeninsula_g_hordeTowers);
		// start the event timer
		RegisterAIUpdateEvent(HELLFIRE_PENINSULA_UPDATE_PERIOD);
	}

	//////////////////////////////////////////////////////////////////////////
	// Save Data To DB
	//////////////////////////////////////////////////////////////////////////
	void UpdateInDB()
	{
		static const char * fieldnames[HELLFIRE_PENINSULA_TOWER_COUNT] = { "hellfire-stadium-status", "hellfire-overlook-status", "hellfire-brokenhill-status" };
		const char * msg = "-1";
		if( Status == 100 )
			msg = "0";
		else
			msg = "1";
			
		WorldStateManager::SetPersistantSetting(fieldnames[towerid], msg);
	}
};

//////////////////////////////////////////////////////////////////////////
// Object Spawn Data
//////////////////////////////////////////////////////////////////////////
void HellFirePeninsula_SpawnObjects(MapMgr *pmgr)
{
	if(!pmgr) return;
	if(pmgr->GetMapId() != 530) return;

	const static PvPZone_sgodata godata[] = {
//         entry       X         Y         Z        facing   O1 O2       O3       O4        state flag faction scale is_banner        
		{ 182173, -290.016f, 3702.42f, 56.6729f,  0.0349066f, 0, 0, 0.0174524f , 0.999848f , 1   , 32  , 0    , 1 ,    0},	// stadium
		{ 182174, -184.889f, 3476.93f, 38.205f , -0.0174535f, 0, 0, 0.00872664f, -0.999962f, 1   , 32  , 0    , 1 ,    0},	// overlook
		{ 182175, -471.462f, 3451.09f, 34.6432f,   0.174533f, 0, 0, 0.0871557f , 0.996195f , 1   , 32  , 0    , 1 ,    0},	// brokenhill
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0},
	};

	const static PvPZone_sgodata godata_banner[] = {
//         entry       X         Y         Z        facing  O1 O2       O3       O4       state flag faction scale is_banner 
		{ 183515, -289.61f , 3696.83f, 75.9447f, 3.12414f , 0, 0, 0.999962f  ,  0.00872656f, 1   , 32 , 1375  , 1     ,1}, // stadium
		{ 182525, -187.887f, 3459.38f, 60.0403f, -3.12414f, 0, 0, 0.00872664f, -0.00872653f, 1   , 32 , 1375  , 1     ,1}, // overlook
		{ 183514, -467.078f, 3528.17f, 64.7121f, 3.14159f , 0, 0, 0.0871557f ,  1          , 1   , 32 , 1375  , 1     ,1}, // brokenhill
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0},
	};

	uint32 i;
	const PvPZone_sgodata *p, *p2;
	for(i = 0; i < 3; ++i)
	{
		p = &godata[i];
		p2 = &godata_banner[i];

		GameObject *pGo = pmgr->GetInterface()->SpawnGameObject(p->entry, p->posx, p->posy, p->posz, p->facing, false, 0, 0, 1);
		if( pGo == NULL )
		{
			Log.Warning("HellFirePeninsula","GameObject %u n'existe pas dans la base!",p->entry);
			continue;
		}

		GameObject *pGo2 = pmgr->GetInterface()->SpawnGameObject(p2->entry, p2->posx, p2->posy, p2->posz, p2->facing, false, 0, 0, 1);
		if( pGo2 == NULL )
		{
			Log.Warning("HellFirePeninsula","GameObject %u n'existe pas dans la base!",p->entry);
			continue;
		}

		pGo->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, p->state);
		pGo2->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, p2->state);
		pGo->SetUInt32Value(GAMEOBJECT_FLAGS, p->flags);
		pGo2->SetUInt32Value(GAMEOBJECT_FLAGS, p2->flags);
		pGo->SetUInt32Value(GAMEOBJECT_FACTION, p->faction);
		pGo2->SetUInt32Value(GAMEOBJECT_FACTION, p2->faction);

		for(uint32 j = 0; j < 4; ++j)
		{
			pGo->SetFloatValue(GAMEOBJECT_PARENTROTATION+j, p->orientation[j]);
			pGo2->SetFloatValue(GAMEOBJECT_PARENTROTATION+j, p2->orientation[j]);
		}

		// now make his script
		pGo->SetScript(new HellfirePeninsulaBannerAI(pGo));
		((HellfirePeninsulaBannerAI*)pGo->GetScript())->pBanner = pGo2;

		pGo->PushToWorld(pmgr);
		pGo2->PushToWorld(pmgr);

		pGo->GetScript()->OnSpawn();
		//Log.Success("[SpawnObjects]","Spawned Go %u for pvp on Hellfire Penninsula.", p->entry);
		//Log.Success("[SpawnObjects]","Spawned Go %u for pvp on Hellfire Penninsula.", p2->entry);
	}
}

void SetupPvP_HellfirePeninsula(MapMgr *mgr)
{
	// load data
	const string tstadium = WorldStateManager::GetPersistantSetting("hellfire-stadium-status", "-1");
	const string toverlook = WorldStateManager::GetPersistantSetting("hellfire-overlook-status", "-1");
	const string tbrokenhill = WorldStateManager::GetPersistantSetting("hellfire-brokenhill-status", "-1");

	HellFirePeninsula_g_towerOwners[HELLFIRE_PENINSULA_TOWER_STADIUM] = atoi(tstadium.c_str());
	HellFirePeninsula_g_towerOwners[HELLFIRE_PENINSULA_TOWER_OVERLOOK] = atoi(toverlook.c_str());
	HellFirePeninsula_g_towerOwners[HELLFIRE_PENINSULA_TOWER_BROKENHILL] = atoi(tbrokenhill.c_str());

    HellFirePeninsula_SpawnObjects(mgr);
}



