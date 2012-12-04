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

// Quête : L’épreuve de la légion  par Randdrick pour WAAD
class DemonPortalGuardian : public GameObjectAIScript
{
public:
	DemonPortalGuardian(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new DemonPortalGuardian(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(pPlayer->GetQuestLogForEntry(5581) && pPlayer->GetItemInterface()->GetItemCount(14547, 1))
		{
			float SSX = pPlayer->GetPositionX();
			float SSY = pPlayer->GetPositionY();
			float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();
				
			pPlayer->GetMapMgr()->GetInterface()->SpawnCreature(11937, SSX+1, SSY, SSZ, SSO, true, false, 0, 0)->Despawn(600000, 0);
		}
		else
		{
			pPlayer->BroadcastMessage("Vous devez posséder la main d'Iruxos pour invoquer les Gardiens du portail");
		}
	}
};

void SetupQueteEpreuveLegion(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(177243, &DemonPortalGuardian::Create);
}
