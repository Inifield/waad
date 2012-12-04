/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
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

// A Faire: Controler que les temps de spawns des Cims est le meme que ceux de la prise des tours!
#define AV_BUFF_RESPAWN_TIME_CIM  300000 // 5 min
#define AV_BUFF_RESPAWN_TIME_TOUR 300000 // 5 min

enum AVControlPoints
{
    // Alliance
	AV_CONTROL_POINT_DUNBALDAR =0,           // 0 Dun Baldar ( Vandar Foudrepique)
	AV_CONTROL_POINT_POSTE_STORMPIKE,        // 1 Poste de Secours Stormpike
	AV_CONTROL_POINT_FORTIN_NORD_DUNBALDAR , // 2 Fortin Nord de DunBaldar 
	AV_CONTROL_POINT_FORTIN_SUD_DUNBALDAR, 	 // 3 Fortin Sud de DunBaldar 	
	AV_CONTROL_POINT_CIM_STORMPIKE, 	     // 4 Flag Cimetiere Stormpike 	    
	AV_CONTROL_POINT_AILE_DE_GLACE, 	     // 5 Fortin de l'Aile de Glace  
	AV_CONTROL_POINT_CIM_STONEHEARTH, 	     // 6 Flag Cimetiere StoneHearth  
	AV_CONTROL_POINT_AV_POSTE_STONEHEARTH, 	 // 7 Avant poste de StoneHearth (Capitaine Belinda Gitepierre ) 
	AV_CONTROL_POINT_FORTIN_STONEHEARTH,     // 8 Fortin de StoneHearth (Commandant Randdolph ) 
	// Neutre (Entouré de Troll Hache-Hiver hostile ( WinterAxe ) )
	AV_CONTROL_POINT_CIM_DES_NEIGES, 	     // 9 Flag du cimetiere des neiges   
	//Horde
	AV_CONTROL_POINT_GARNISON_GLACESANG, 	 // 10 Garnison GlaceSang ( Capitaine Galvangar )   
	AV_CONTROL_POINT_TOUR_GLACESANG, 	     // 11 Tour GlaceSang 	   
	AV_CONTROL_POINT_CIM_GLACESANG, 	     // 12 Flag Cimetiere GlaceSang
	AV_CONTROL_POINT_TOUR_DE_LA_HALTE, 	     // 13 Tour de la Halte ( Commandant Louis Philips + Drapeau )
	AV_CONTROL_POINT_CIM_FROSTWOLF, 	     // 14 Flag Cimetiere FrostWolf	
	AV_CONTROL_POINT_TOUR_ORIENT_FROSTWOLF,  // 15 Tour Orientale FrostWolf   
	AV_CONTROL_POINT_TOUR_OCCID_FROSTWOLF, 	 // 16 Tour Occidentale FrostWolf      
	AV_CONTROL_POINT_DONJON_FROSTWOLF, 	     // 17 DONJON_FROSTWOLF (Drek'Dar)   
	AV_CONTROL_POINT_HUTTE_FROSTWOLF,        // 18 Flag cimetiere Hutte FrostWolf
	AV_NUM_CONTROL_POINTS
};

enum AVSpawnTypes
{
	AV_SPAWN_TYPE_NEUTRAL							= 0,
	AV_SPAWN_TYPE_ALLIANCE_ASSAULT					,
	AV_SPAWN_TYPE_HORDE_ASSAULT						,
	AV_SPAWN_TYPE_ALLIANCE_CONTROLLED           	,
	AV_SPAWN_TYPE_HORDE_CONTROLLED           		,
	AV_NUM_SPAWN_TYPES								
};

class AlteracValley : public CBattleground
{
	
public:
	GameObject * m_drapeauCimDesNeiges[4];
	GameObject * m_controlPoints[AV_NUM_CONTROL_POINTS];
	GameObject * m_controlPointAuras[AV_NUM_CONTROL_POINTS];
	GameObject * m_controlPointsTopTowers[AV_NUM_CONTROL_POINTS];
	GameObject * m_controlPointAurasTopTowers[AV_NUM_CONTROL_POINTS];

	
protected:
	list<GameObject*> m_gates;
	
	uint32 m_DestroyedTower[AV_NUM_CONTROL_POINTS];
	uint32 m_capturedBases[2];
	uint32 m_lastRenforts[2];
	bool m_nearingVictory[2];
	int32 m_basesOwnedBy[AV_NUM_CONTROL_POINTS];
	int32 m_basesAssaultedBy[AV_NUM_CONTROL_POINTS];
	Creature * m_spiritGuides[AV_NUM_CONTROL_POINTS];
    bool DefFlag[AV_NUM_CONTROL_POINTS][2];    	 

public:
	AlteracValley(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t);
	~AlteracValley();
	virtual void Init();

	void HookOnPlayerDeath(Player * plr);
	void HookFlagDrop(Player * plr, GameObject * obj);
	void HookFlagStand(Player * plr, GameObject * obj);
	void HookOnMount(Player * plr);
	void HookOnAreaTrigger(Player * plr, uint32 id);
	void HookOnShadowSight();
	bool HookHandleRepop(Player * plr);
	void OnAddPlayer(Player * plr);
	void OnRemovePlayer(Player * plr);
	void OnCreate();
	void HookOnPlayerKill(Player * plr, Unit * pVictim);
	void HookOnHK(Player * plr);
	void SpawnBuff(uint32 x);
	LocationVector GetStartingCoords(uint32 Team);
	void DropFlag(Player * plr);

	static CBattleground * Create(MapMgr * m, uint32 i, uint32 l, uint32 t) { return new AlteracValley(m, i, l, t); }

	const char * GetName() { return "la vallee d'Alterac"; }
	void OnStart();

	void EventUpdateResources(uint32 Team);
	bool HookSlowLockOpen(GameObject * pGo, Player * pPlayer, Spell * pSpell);
	void SetIsWeekend(bool isweekend);

	// 
	void SpawnControlPoint(uint32 Id, uint32 Type);
	void SpawnControlTopTowers(uint32 Id, uint32 Type);
	void SpawnDrapCimDesNeiges(uint32 Type);
	void SpawnArchers(int32 Id, uint32 Team);
	void FireGuards(uint32 Id, uint32 Team);
	void CaptureControlPoint(uint32 Id, uint32 Team);
	void AssaultControlPoint(Player * pPlayer, uint32 Id);
	// A Faire:
	// Les pnjs et quetes: En cours
	// Drek et Vandar + Les differents commandant!
	// Les prises de mines 
	
};

