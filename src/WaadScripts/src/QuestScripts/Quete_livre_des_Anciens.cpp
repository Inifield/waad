 /*
 * WaadAscent Scripts for Ascent MMORPG Server
 * Copyright (C) 2008 WaadAscent Team <http://arbonne.games-rpg.net/>
 *
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 *
 */
 
#include "StdAfx.h"
#include "Setup.h"

// Quête : Le livre des Anciens par Randdrick pour WAAD
class LordKragaru : public GameObjectAIScript
{
public:
	LordKragaru(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new LordKragaru(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->GetQuestLogForEntry(6027) && pPlayer->GetItemInterface()->GetItemCount(15766, 1))
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();
				
			pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(12369, SSX+1, SSY, SSZ, SSO, true, false, 0, 0)->Despawn(600000, 0);
		}
		else
		{
			pPlayer->BroadcastMessage("Vous devez posséder la Gemme du Serpent pour invoquer le Gardien du Livre des Anciens");
		}
	}
};

void SetupQueteLivreAnciens(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(177673, &LordKragaru::Create);
}
