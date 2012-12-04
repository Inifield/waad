/*
 * BBscrips :D for WaadAscent Scripts for Ascent MMORPG Server
 * Copyright (C) 2008 WaadAscent Team <http://arbonne.games-rpg.net/>
 *
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 *
 */
 
#include "StdAfx.h"
#include "Setup.h"

// Quête : Arikara par B.B. pour WAAD
// VERIF SQL: DELETE FROM creature_spawns WHERE entry='10882'
class Arikara : public GameObjectAIScript
{
public:
	Arikara(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new Arikara(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->GetQuestLogForEntry(5088) && pPlayer->GetItemInterface()->GetItemCount(12785, 1))
		{	
			pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(10882, -5003.858398f, -2108.339111f, 88.612335f, 0.797164f, true, false, 0, 0)->Despawn(600000, 0);
		}
		else
		{
			pPlayer->BroadcastMessage("Vous devez posseder la poudre d'incendia pour invoquer Arikara");
		}
	}
};


void SetupQueteArikara(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(175944, &Arikara::Create);
}