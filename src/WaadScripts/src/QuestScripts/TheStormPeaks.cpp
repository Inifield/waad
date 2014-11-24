/*
 * ArcScripts for Ascent MMORPG Server
 * Copyright (C) 2010-2014 WaadAscent Team <http://arbonne.games-rpg.net/>
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "Setup.h"

// The Gifts of Loken
class LokensFury : public GameObjectAIScript
{
	public:
		LokensFury(GameObject* goinstance) : GameObjectAIScript(goinstance) {};
		static GameObjectAIScript *Create(GameObject * GO) { return new LokensFury(GO); }

		void OnActivate(Player* pPlayer)
		{
			QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( 12965 );
			if(qle)
			{
				qle->SetMobCount( 0, 1 );
				qle->SendUpdateAddKill(0,0);
				qle->UpdatePlayerFields();
			}
		};

};

class LokensPower : public GameObjectAIScript
{
	public:
		LokensPower(GameObject* goinstance) : GameObjectAIScript(goinstance) {};
		static GameObjectAIScript *Create(GameObject * GO) { return new LokensPower(GO); }

		void OnActivate(Player* pPlayer)
		{
			QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( 12965 );
			if(qle)
			{
				qle->SetMobCount( 0, 1 );
				qle->SendUpdateAddKill(0,0);
				qle->UpdatePlayerFields();
			}
		};

};

class LokensFavor : public GameObjectAIScript
{
	public:
		LokensFavor(GameObject* goinstance) : GameObjectAIScript(goinstance) {};
		static GameObjectAIScript *Create(GameObject * GO) { return new LokensFavor(GO); }

		void OnActivate(Player* pPlayer)
		{
			QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( 12965 );
			if(qle)
			{
				qle->SetMobCount( 0, 1 );
				qle->SendUpdateAddKill(0,0);
				qle->UpdatePlayerFields();
			}
		};

};

#define GOSSIP_SCOUTMENU1 "Êtes-vous d'accord ? Je suis venu pour vous ramener à GFrosthold si vous pouvez le supporter."
#define GOSSIP_SCOUTMENU2 "Je suis désolé de ne pas avoir été ici plus tôt. Qu'est-ce qui s'est passé ?"
#define GOSSIP_SCOUTMENU3 "Je vais chercher de l'aide. Accrochez-vous."

class SCRIPT_DECL MissingScout_Gossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 13611, plr);
			if(plr->HasQuest(12864))
				Menu->AddItem(0, GOSSIP_SCOUTMENU1, 1);

			Menu->SendTo(plr);
		}
		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			Creature* pCreature = (pObject->IsCreature()) ? (static_cast<Creature *>(pObject)) : NULL;
			if(pCreature == NULL)
				return;

			GossipMenu* Menu;
			switch(IntId)
			{
				case 1:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 13612, plr);
						Menu->AddItem(0, GOSSIP_SCOUTMENU2, 2);
						Menu->SendTo(plr);
					}
					break;
				case 2:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 13613, plr);
						Menu->AddItem(0, GOSSIP_SCOUTMENU3, 3);
						Menu->SendTo(plr);
					}
					break;
				case 3:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 13614, plr);
						Menu->SendTo(plr);

						QuestLogEntry* qle = plr->GetQuestLogForEntry(12864);
						if(qle == NULL || qle->GetMobCount(0) != 0)
							return;

						qle->SetMobCount(0, 1);
						qle->SendUpdateAddKill(0,0);
						qle->UpdatePlayerFields();
					}
					break;
			}
		}
};

void SetupTheStormPeaks(ScriptMgr* mgr)
{
	// The Gifts of Loken
	mgr->register_gameobject_script(192120, &LokensFury::Create);
	mgr->register_gameobject_script(192121, &LokensPower::Create);
	mgr->register_gameobject_script(192122, &LokensFavor::Create);
	GossipScript* MissingScoutGossip = new MissingScout_Gossip;
	mgr->register_gossip_script(29811, MissingScoutGossip);
}