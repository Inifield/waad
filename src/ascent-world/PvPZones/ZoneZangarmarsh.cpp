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

//DELETE FROM gameobject_spawns WHERE `Map` = 530 AND `entry` in (182523, 182522);

#define ZANGARMARSH_BANNER_RANGE 900
#define ZANGARMARSH_UPDATE_PERIOD 5000
#define ZANGARMARSH_CAPTURE_RATE 20

// Towers
enum Zangarmarsh_Towers
{
	ZANGARMARSH_TOWER_EAST,
	ZANGARMARSH_TOWER_WEST,
	ZANGARMARSH_TOWER_COUNT,
};

#define ZONE_ZANGARMARSH										3521

// tower stuff
#define WORLDSTATE_ZANGARMARSH_EAST_NEUTRAL_MAP					2652
#define WORLDSTATE_ZANGARMARSH_EAST_ALLIANCE_MAP				2650
#define WORLDSTATE_ZANGARMARSH_EAST_HORDE_MAP					2651

#define WORLDSTATE_ZANGARMARSH_EAST_NEUTRAL_UI					2560
#define WORLDSTATE_ZANGARMARSH_EAST_ALLIANCE_UI					2558
#define WORLDSTATE_ZANGARMARSH_EAST_HORDE_UI					2559

#define WORLDSTATE_ZANGARMARSH_WEST_NEUTRAL_MAP					2646
#define WORLDSTATE_ZANGARMARSH_WEST_ALLIANCE_MAP				2644
#define WORLDSTATE_ZANGARMARSH_WEST_HORDE_MAP					2645

#define WORLDSTATE_ZANGARMARSH_WEST_NEUTRAL_UI					2557
#define WORLDSTATE_ZANGARMARSH_WEST_ALLIANCE_UI					2555
#define WORLDSTATE_ZANGARMARSH_WEST_HORDE_UI					2556

// capture bars
#define WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_WEST			2527
#define WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_WEST			2528

#define WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_EAST			2533
#define WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_EAST			2534

// flag

//ally
#define WORLDSTATE_ZANGARMARSH_SCOUT_READY_ALLIANCE		2655
#define WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_ALLIANCE	2656

//horde
#define WORLDSTATE_ZANGARMARSH_SCOUT_READY_HORDE		2658
#define WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_HORDE	2657

#define WORLDSTATE_ZANGARMARSH_CITY_NEUTRAL				2647
#define WORLDSTATE_ZANGARMARSH_CITY_ALLIANCE			2648
#define WORLDSTATE_ZANGARMARSH_CITY_HORDE				2649


// Tower GameObject Ids
#define ZANGARMARSH_G_TOWER_EAST 182523
#define ZANGARMARSH_G_TOWER_WEST 182522

#define TWIN_SPIRE_BLESSING 33779

// Owners of the towers, used for save/restore
int32 ZMg_towerOwners[ZANGARMARSH_TOWER_COUNT] = { -1, -1 };

// global variables
uint32 ZMg_hordeTowers = 0;
uint32 ZMg_allianceTowers = 0;
int32 ZMg_superiorTeam = -1;			// SUPERIORITY
int32 ZMCityOwners = -1;

// Visual view for MAP
static const uint32 Zangarmarsh_g_hordeStateFields_MAP[ZANGARMARSH_TOWER_COUNT]		= {	WORLDSTATE_ZANGARMARSH_EAST_HORDE_MAP,		WORLDSTATE_ZANGARMARSH_WEST_HORDE_MAP};
static const uint32 Zangarmarsh_g_allianceStateFields_MAP[ZANGARMARSH_TOWER_COUNT]	= {	WORLDSTATE_ZANGARMARSH_EAST_ALLIANCE_MAP,	WORLDSTATE_ZANGARMARSH_WEST_ALLIANCE_MAP};
static const uint32 Zangarmarsh_g_neutralStateFields_MAP[ZANGARMARSH_TOWER_COUNT]	= {	WORLDSTATE_ZANGARMARSH_EAST_NEUTRAL_MAP,	WORLDSTATE_ZANGARMARSH_WEST_NEUTRAL_MAP};

// Visual view for UI
static const uint32 Zangarmarsh_g_hordeStateFields_UI[ZANGARMARSH_TOWER_COUNT]		= {	WORLDSTATE_ZANGARMARSH_EAST_HORDE_UI,		WORLDSTATE_ZANGARMARSH_WEST_HORDE_UI};
static const uint32 Zangarmarsh_g_allianceStateFields_UI[ZANGARMARSH_TOWER_COUNT]	= {	WORLDSTATE_ZANGARMARSH_EAST_ALLIANCE_UI,	WORLDSTATE_ZANGARMARSH_WEST_ALLIANCE_UI};
static const uint32 Zangarmarsh_g_neutralStateFields_UI[ZANGARMARSH_TOWER_COUNT]	= {	WORLDSTATE_ZANGARMARSH_EAST_NEUTRAL_UI,		WORLDSTATE_ZANGARMARSH_WEST_NEUTRAL_UI};

void ZMSpawnBanners(MapMgr *bmgr, int32 side);

void Zangarmarsh_SetGrave(MapMgr *pmgr)
{
	if(!pmgr || pmgr->GetMapId() != 530)
		return;

	// any better solution?
	uint32 gOwners = ZMCityOwners == FACTION_ALLY ? FACTION_ALLY : FACTION_HORDE;

	if( ZMCityOwners == FACTION_NEUTRAL ) gOwners = 3;

	StorageContainerIterator<GraveyardTeleport> * itr;
	itr = GraveyardStorage.MakeIterator();
	while(!itr->AtEnd())
	{
		GraveyardTeleport *pGrave = itr->Get();
		if( pGrave->ID == 142 )
		{
			pGrave->FactionID = gOwners;
			break;
		}
		if(!itr->Inc())
			break;
	}
	itr->Destruct();

	if( ZMCityOwners == FACTION_ALLY )
	{
		pmgr->GetDBStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_ALLIANCE, 1);
		pmgr->GetDBStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_HORDE, 0);			
		pmgr->GetDBStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_NEUTRAL, 0);
	}
	else if( ZMCityOwners == FACTION_HORDE )
	{
		pmgr->GetDBStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_HORDE, 1);
		pmgr->GetDBStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_ALLIANCE, 0);
		pmgr->GetDBStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_NEUTRAL, 0);
	}
	else
	{
		pmgr->GetDBStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_HORDE, 0);
		pmgr->GetDBStateManager().CreateWorldState(WORLDSTATE_ZANGARMARSH_CITY_ALLIANCE, 0);
	}
}

ASCENT_INLINE void UpdateTowerCountZM(MapMgr *mgr)
{
	if(!mgr)
		return;

	if( (ZMg_superiorTeam == FACTION_ALLY) && (ZMg_allianceTowers != ZANGARMARSH_TOWER_COUNT) )
	{
		mgr->GetDBStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_ALLIANCE, 1);
		mgr->GetDBStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_READY_ALLIANCE, 0);
		ZMg_superiorTeam = FACTION_NEUTRAL;
	}

	if( (ZMg_superiorTeam == FACTION_HORDE) && (ZMg_hordeTowers != ZANGARMARSH_TOWER_COUNT) )
	{
		mgr->GetDBStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_HORDE, 1);
		mgr->GetDBStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_READY_HORDE, 0);
		ZMg_superiorTeam = FACTION_NEUTRAL;
	}

	if( (ZMg_allianceTowers == ZANGARMARSH_TOWER_COUNT) && (ZMg_superiorTeam != FACTION_HORDE) )
	{
		ZMg_superiorTeam = FACTION_HORDE;
		mgr->GetDBStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_ALLIANCE, 0);
		mgr->GetDBStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_READY_ALLIANCE, 1);
	}

	if( (ZMg_hordeTowers == ZANGARMARSH_TOWER_COUNT) && (ZMg_superiorTeam != FACTION_ALLY) )
	{
		ZMg_superiorTeam = FACTION_ALLY;
		mgr->GetDBStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_NOT_READY_HORDE, 0);
		mgr->GetDBStateManager().UpdateWorldState(WORLDSTATE_ZANGARMARSH_SCOUT_READY_HORDE, 1);
	}
}

class ZangarmarshBannerAI : public GameObjectAIScript
{
	map<uint32, uint32> StoredPlayers;
	uint32 Status;
	const char * ControlPointName;
	uint32 towerid;
	uint32 m_bannerStatus;

public:

	ZangarmarshBannerAI(GameObject *go) : GameObjectAIScript(go)
	{
		m_bannerStatus = PVPZONE_BANNER_STATUS_NEUTRAL;
		Status = 50;

		switch(go->GetEntry())
		{
		case ZANGARMARSH_G_TOWER_EAST:
			ControlPointName = "tour de l'Est";
			towerid = ZANGARMARSH_TOWER_EAST;
			break;

		case ZANGARMARSH_G_TOWER_WEST:
			ControlPointName = "tour de l'Ouest";
			towerid = ZANGARMARSH_TOWER_WEST;
			break;

		default:
			ControlPointName = "<ControlPoint inconnu>";
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

			in_range = (_gameobject->GetDistanceSq((*itr)) <= ZANGARMARSH_BANNER_RANGE) ? true : false;

			it2 = StoredPlayers.find((*itr)->GetLowGUID());
			if( it2 == StoredPlayers.end() )
			{
				if( in_range )
				{
					_gameobject->GetMapMgr()->GetDBStateManager().SendWorldStates((*itr)); // Init WorldStates sur l'ecran du player
					if( towerid == ZANGARMARSH_TOWER_WEST )
					{
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_WEST, 1);
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_WEST, Status);
					}
					else
					{
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_EAST, 1);
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_EAST, Status);
					}
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
					if( towerid == ZANGARMARSH_TOWER_WEST )
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_WEST, 0);
					else
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_EAST, 0);
					StoredPlayers.erase(it2);
				}
				else
				{
					if( towerid == ZANGARMARSH_TOWER_WEST )
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_WEST, Status);
					else
						(*itr)->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_VALUE_EAST, Status);
					it2->second = timeptr;
					if( is_valid )
						plrcounts[(*itr)->GetTeam()]++;
				}
			}
		}

		// handle stuff for the last tick
		if( (Status == 100) && (m_bannerStatus != PVPZONE_BANNER_STATUS_ALLIANCE) )
		{
			m_bannerStatus = PVPZONE_BANNER_STATUS_ALLIANCE;

			// send message to everyone in the zone, has been captured by the Alliance
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_ZANGARMARSH, ZONE_ZANGARMARSH, "|cffffff00La %s a ete prise par l'Alliance!|r", ControlPointName);

			// tower update
			ZMg_allianceTowers++;
			UpdateTowerCountZM(_gameobject->GetMapMgr());

			// state update
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_neutralStateFields_MAP[towerid], 0);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_allianceStateFields_MAP[towerid], 1);
			
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_neutralStateFields_UI[towerid], 0);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_allianceStateFields_UI[towerid], 1);

			// woot
			ZMg_towerOwners[towerid] = FACTION_ALLY;
			UpdateInDB();
		}
		else if( (Status == 0) && (m_bannerStatus != PVPZONE_BANNER_STATUS_HORDE) )
		{
			m_bannerStatus = PVPZONE_BANNER_STATUS_HORDE;

			// send message to everyone in the zone, has been captured by the Horde
			_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_ZANGARMARSH, ZONE_ZANGARMARSH, "|cffffff00%s a ete prise par le Horde!|r", ControlPointName);
			
			// tower update
			ZMg_hordeTowers++;
			UpdateTowerCountZM(_gameobject->GetMapMgr());

			// state update
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_neutralStateFields_MAP[towerid], 0);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_hordeStateFields_MAP[towerid], 1);

			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_neutralStateFields_UI[towerid], 0);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_hordeStateFields_UI[towerid], 1);

			// woot
			ZMg_towerOwners[towerid] = FACTION_HORDE;
			UpdateInDB();
		}
		else if( m_bannerStatus != PVPZONE_BANNER_STATUS_NEUTRAL )
		{
			// if the difference for the faction is >50, change to neutral
			if( (m_bannerStatus == PVPZONE_BANNER_STATUS_ALLIANCE) && (Status <= 50) )
			{
				// send message: The Alliance has lost control of point xxx
				m_bannerStatus = PVPZONE_BANNER_STATUS_NEUTRAL;
				
				ZMg_allianceTowers--;
				UpdateTowerCountZM(_gameobject->GetMapMgr());

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_ZANGARMARSH, ZONE_ZANGARMARSH, "|cffffff00L'Alliance perd le control de la %s!|r", ControlPointName);

				// state update
				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_allianceStateFields_MAP[towerid], 0);
				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_neutralStateFields_MAP[towerid], 1);

				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_allianceStateFields_UI[towerid], 0);
				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_neutralStateFields_UI[towerid], 1);

				// woot
				ZMg_towerOwners[towerid] = FACTION_NEUTRAL;
				UpdateInDB();
			}
			else if( (m_bannerStatus == PVPZONE_BANNER_STATUS_HORDE) && (Status >= 50) )
			{
				// send message: The Alliance has lost control of point xxx
				m_bannerStatus = PVPZONE_BANNER_STATUS_NEUTRAL;

				ZMg_hordeTowers--;
				UpdateTowerCountZM(_gameobject->GetMapMgr());

				_gameobject->GetMapMgr()->SendPvPCaptureMessage(ZONE_ZANGARMARSH, ZONE_ZANGARMARSH, "|cffffff00La Horde perd le controle de la %s!|r", ControlPointName);

				// state update
				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_hordeStateFields_MAP[towerid], 0);
				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_neutralStateFields_MAP[towerid], 1);

				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_hordeStateFields_UI[towerid], 0);
				_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_neutralStateFields_UI[towerid], 1);

				// woot
				ZMg_towerOwners[towerid] = FACTION_NEUTRAL;
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
					if( towerid == ZANGARMARSH_TOWER_WEST )
						plr->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_WEST, 0);
					else
						plr->SendWorldStateUpdate(WORLDSTATE_ZANGARMARSH_CAPTURE_BAR_DISPLAY_EAST, 0);
				}

				StoredPlayers.erase(it3);
			}
		}

		// work out current status for next tick
		uint32 delta;
		if( plrcounts[0] > plrcounts[1] )
		{
			delta = plrcounts[0] - plrcounts[1];
			delta *= ZANGARMARSH_CAPTURE_RATE;

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
			delta *= ZANGARMARSH_CAPTURE_RATE;
			
			// cap it at 25 so the banner always gets removed.
			if( delta > 25 )
				delta = 25;

			if( delta > Status )
				Status = 0;
			else
				Status -= delta;
		}
	}
	void OnSpawn()
	{
		m_bannerStatus = PVPZONE_BANNER_STATUS_NEUTRAL;

		// preloaded data, do we have any?
		if( ZMg_towerOwners[towerid] == FACTION_HORDE )
		{
			m_bannerStatus = PVPZONE_BANNER_STATUS_HORDE;
			Status = 0;

			// state update
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_hordeStateFields_MAP[towerid], 1);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_neutralStateFields_MAP[towerid], 0);

			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_hordeStateFields_UI[towerid], 1);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_neutralStateFields_UI[towerid], 0);

			// countz
			ZMg_hordeTowers++;
			UpdateTowerCountZM(_gameobject->GetMapMgr());
		}
		else if( ZMg_towerOwners[towerid] == FACTION_ALLY )
		{
			m_bannerStatus = PVPZONE_BANNER_STATUS_ALLIANCE;
			Status = 100;

			// state update
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_allianceStateFields_MAP[towerid], 1);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_neutralStateFields_MAP[towerid], 0);

			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_allianceStateFields_UI[towerid], 1);
			_gameobject->GetMapMgr()->GetDBStateManager().UpdateWorldState(Zangarmarsh_g_neutralStateFields_UI[towerid], 0);

			// countz
			ZMg_allianceTowers++;
			UpdateTowerCountZM(_gameobject->GetMapMgr());
		}

		// start the event timer
		RegisterAIUpdateEvent(ZANGARMARSH_UPDATE_PERIOD);
	}

	//////////////////////////////////////////////////////////////////////////
	// Save Data To DB
	//////////////////////////////////////////////////////////////////////////
	void UpdateInDB()
	{
		static const char * fieldnames[ZANGARMARSH_TOWER_COUNT] = { "Zangarmarsh-TowerWest-status", "Zangarmarsh-TowerEast-status" };
		const char * msg = "-1";
		if( Status == 100 )
			msg = "0";
		else
			msg = "1";
			
		WorldStateManager::SetPersistantSetting(fieldnames[towerid], msg);
	}
};

//////////////////////////////////////////////////////////////////////////
// Scouts AI
//////////////////////////////////////////////////////////////////////////

class SCRIPT_DECL ZMScouts : public GossipScript
{
public:
	void GossipHello(Object *pObject, Player *plr, bool AutoSend)
	{
		uint32 Team = plr->GetTeam();
		if(Team > 1) Team = 1;

		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), Team == FACTION_ALLY ? 9433 : 9750, plr);

		if( pObject->GetUInt32Value(UNIT_NPC_FLAGS) & UNIT_NPC_FLAG_VENDOR )
			Menu->AddItem(GOSSIP_ICON_GOSSIP_VENDOR, "Je souhaiterais voir vos marchandises.", 3 );

		if( ZMg_superiorTeam == Team && ZMCityOwners != Team && !plr->HasAura( Team == FACTION_ALLY ? 32430 : 32431 ) )
			Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, Team == FACTION_ALLY ? "Donnez moi le drapeau, Je le planterais dans la tour centrale pour la gloire de l'Alliance!" : "Donnez moi le drapeau, Je le planterais dans la tour centrale pour la gloire de la Horde!" , Team+1);
		if(AutoSend)
			Menu->SendTo(plr);
    }

    void GossipSelectOption(Object *pObject, Player *plr, uint32 Id, uint32 IntId, const char * Code)
    {
		if( !plr )
			return;
		
		Creature *pCreature = NULL;
		pCreature = pObject->IsCreature() ?(( Creature *)pObject ) : NULL;
		if( !pCreature )
			return;

		if( IntId == 3 )
		{
			plr->GetSession()->SendInventoryList(pCreature);
			return;
		}

		uint32 CastSpellID = IntId == 1 ? 32430 : 32431;
		if( CastSpellID != 0 )
			if( !plr->HasAura( CastSpellID ) )
				pCreature->CastSpell(plr,CastSpellID,true);
		return;
    }
};

class ZMCityBannerAI : public GameObjectAIScript
{
public:
	ZMCityBannerAI(GameObject *goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject *GO) { return new ZMCityBannerAI(GO); }

	void OnActivate(Player *pPlayer)
	{
		if( !pPlayer )
			return;

		uint32 pTeam = pPlayer->GetTeam();
		if(pTeam > FACTION_HORDE) pTeam = FACTION_HORDE;

		if( (ZMg_superiorTeam != pTeam) || (ZMCityOwners == pTeam) )
			return;

		uint32 flagaura = ( pTeam == FACTION_ALLY ? 32430 : 32431 );

		if( pPlayer->HasAura( flagaura ) )
			pPlayer->RemoveAura( flagaura );
		else
			return; // we must have flag to capture city
		
		ZMCityOwners = pTeam; // we are now city owners! Yay
		//brrr....
		const char * cOwners = (ZMCityOwners == FACTION_ALLY) ? "0" : "1";

		if( ZMCityOwners == FACTION_NEUTRAL ) cOwners = "-1"; // just in case
			

		WorldStateManager::SetPersistantSetting("Zangarmarsh-city-owners", cOwners);
			
		Zangarmarsh_SetGrave(_gameobject->GetMapMgr());

		uint32 oppositeTeam = ( pTeam == FACTION_ALLY ? FACTION_HORDE : FACTION_ALLY );

		_gameobject->GetMapMgr()->CastSpellOnPlayers(pTeam, TWIN_SPIRE_BLESSING);
		_gameobject->GetMapMgr()->RemovePositiveAuraFromPlayers(oppositeTeam, TWIN_SPIRE_BLESSING);

		if( _gameobject )
			_gameobject->Despawn(0);

		ZMSpawnBanners( pPlayer->GetMapMgr(), pTeam ); // spawn faction banner
	}

	
	//////////////////////////////////////////////////////////////////////////
	// Zone Hook
	//////////////////////////////////////////////////////////////////////////
	void OnZoneHook(Player *plr, uint32 Zone, uint32 OldZone)
	{
		if(!plr) return;
		if( Zone == ZONE_ZANGARMARSH )
		{
			if( ZMCityOwners == plr->GetTeam() )
				plr->CastSpell(plr, dbcSpell.LookupEntry(TWIN_SPIRE_BLESSING), true);
		}
		else if( OldZone == ZONE_ZANGARMARSH )
		{
			if( ZMCityOwners == plr->GetTeam() )
				plr->RemovePositiveAura(TWIN_SPIRE_BLESSING);
		}
	}
};


//////////////////////////////////////////////////////////////////////////
// Object Spawn Data
//////////////////////////////////////////////////////////////////////////
void ZMSpawnBanners(MapMgr *bmgr, int32 side)
{
	if(!bmgr)
		return;
	// -1 = neutral
	//  0 = alliance
	//  1 = horde

	//DELETE FROM gameobject_spawns WHERE `Map` = 530 AND `entry` in (182529, 182527, 182528);
	const static PvPZone_sgodata gobdata[] = {
//         entry       X         Y         Z        facing   O1  O2       O3       O4    state flag faction scale is_banner        
		{ 182529, 253.54f, 7083.81f, 36.7728f,   -0.017453f,  0, 0, 0.008727f, -0.999962f, 1  , 0   , 0 ,    1   ,  0},
		{ 182527, 253.54f, 7083.81f, 36.7728f,   -0.017453f,  0, 0, 0.008727f, -0.999962f, 1  , 0   , 0 ,    1   ,  0},
		{ 182528, 253.54f, 7083.81f, 36.7728f,   -0.017453f,  0, 0, 0.008727f, -0.999962f, 1  , 0   , 0 ,    1   ,  0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0},
	};

	uint32 i = side+1;
	if( i > 2 ) // how does that happen? oO
		i = 2;
	const PvPZone_sgodata *b;
	b = &gobdata[i];

	GameObject *bGo = NULL;
	bGo = bmgr->GetInterface()->SpawnGameObject(b->entry, b->posx, b->posy, b->posz, b->facing, false, 0, 0,1);
	if( !bGo )
		return;

	bGo->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, b->state);
	bGo->SetUInt32Value(GAMEOBJECT_FLAGS, b->flags);
	bGo->SetUInt32Value(GAMEOBJECT_FACTION, b->faction);

	for(uint32 j = 0; j < 4; ++j)
	{
		bGo->SetFloatValue(GAMEOBJECT_PARENTROTATION+j, b->orientation[j]);
	}

	bGo->PushToWorld(bmgr);
}

void ZMSpawnObjects(MapMgr *pmgr)
{
	if(!pmgr || pmgr->GetMapId() != 530)
		return;

	ZMSpawnBanners(pmgr,ZMCityOwners);

	const static PvPZone_sgodata godata[] = {
//         entry                         X         Y         Z      facing   O1  O2       O3       O4    state flag faction scale is_banner 
		{ ZANGARMARSH_G_TOWER_EAST, 303.243f, 6841.36f, 40.1245f, -1.58825f,  0,  0, 0.71325f, -0.700909f, 1,   32,   0,     1 ,    0},
		{ ZANGARMARSH_G_TOWER_WEST, 336.466f, 7340.26f, 41.4984f, -1.58825f,  0,  0, 0.71325f, -0.700909f, 1,   32,   0,     1 ,    0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0},
	};

	uint32 i;
	const PvPZone_sgodata *p;

	for(i = 0; i < 2; ++i)
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
		pGo->SetScript(new ZangarmarshBannerAI(pGo));

		pGo->PushToWorld(pmgr);

		pGo->GetScript()->OnSpawn();
	}


}

void Zangarmarsh_Tokens(Player *pPlayer, Player *pVictim)
{
	if( !pPlayer || !pVictim )
		return;
	if( !pPlayer->HasAura(TWIN_SPIRE_BLESSING) || pPlayer->GetTeam() == pVictim->GetTeam() )
		return;

	uint32 TokenSpell = pPlayer->GetTeam() == FACTION_ALLY ? 32155 : 32158;

	pPlayer->CastSpell(pPlayer,TokenSpell,true);
}

void SetupPvP_Zangarmarsh(MapMgr *mgr)
{
	// load data
	const string Tower1 = WorldStateManager::GetPersistantSetting("Zangarmarsh-TowerWest-status", "-1");
	const string Tower2 = WorldStateManager::GetPersistantSetting("Zangarmarsh-TowerEast-status", "-1");
	const string City = WorldStateManager::GetPersistantSetting("Zangarmarsh-city-owners", "-1");

	ZMg_towerOwners[ZANGARMARSH_TOWER_WEST] = atoi(Tower1.c_str());
	ZMg_towerOwners[ZANGARMARSH_TOWER_EAST] = atoi(Tower2.c_str());
	ZMCityOwners = atoi(City.c_str());

	// City banners AI

	//mgr->register_gameobject_script(182529, &ZMCityBannerAI::Create);
	//ScriptMgr::register_gameobject_script(182529,ZMCityBannerAI::Create);
	//mgr->register_gameobject_script(182527, &ZMCityBannerAI::Create);
	//mgr->register_gameobject_script(182528, &ZMCityBannerAI::Create);
	// scouts AI
	//GossipScript *zms = (GossipScript*) new ZMScouts();
	//mgr->register_gossip_script(18564, zms);
	//mgr->register_gossip_script(18581, zms);

	
	//mgr->register_hook(SERVER_HOOK_EVENT_ON_KILL_PLAYER, (void*)Tokens);
	//mgr->register_hook(SERVER_HOOK_EVENT_ON_ZONE, ZMZoneHook);

	//ZMSpawnObjects(mgr);
	//Zangarmarsh_SetGrave(mgr);
}


