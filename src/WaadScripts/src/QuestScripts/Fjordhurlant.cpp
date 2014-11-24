/*
 * WaadAscent Scripts for Ascent MMORPG Server by Crash
 * Copyright (C) 2010 WaadAscent Team <http://arbonne.games-rpg.net/>
 *
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 *
 */

#include "StdAfx.h"
#include "Setup.h"


class NorthFleet : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(NorthFleet);
		NorthFleet(Creature* pCreature) : CreatureAIScript(pCreature)  {}

		void OnDied(Unit* mKiller)
		{
			if(mKiller->IsPlayer())
			{
				QuestLogEntry* qle = static_cast<Player *>(mKiller)->GetQuestLogForEntry(11230);
				if(qle != NULL)
				{
					if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[ 0 ])
					{
						uint32 newcount = qle->GetMobCount(0) + 1;
						qle->SetMobCount(0, newcount);
						qle->SendUpdateAddKill(0,0);
						qle->UpdatePlayerFields();
						return;
					}
				}
			}
		}
};
class ChillmereScourge : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ChillmereScourge);
		ChillmereScourge(Creature* pCreature) : CreatureAIScript(pCreature)  {}

		void OnDied(Unit* mKiller)
		{
			if(mKiller->IsPlayer())
			{
				QuestLogEntry* qle = static_cast<Player *>(mKiller)->GetQuestLogForEntry(11397);
				if(qle != NULL)
				{
					if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[ 0 ])
					{
						uint32 newcount = qle->GetMobCount(0) + 1;
						qle->SetMobCount(0, newcount);
						qle->SendUpdateAddKill(0,0);
						qle->UpdatePlayerFields();
						return;
					}
				}
			}
		}
};
class Baleheim : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Baleheim);
		Baleheim(Creature* pCreature) : CreatureAIScript(pCreature)  {}

		void OnDied(Unit* mKiller)
		{
			if(mKiller->IsPlayer())
			{
				QuestLogEntry* qle = static_cast<Player *>(mKiller)->GetQuestLogForEntry(11283);
				if(qle != NULL)
				{
					if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[ 0 ])
					{
						uint32 newcount = qle->GetMobCount(0) + 1;
						qle->SetMobCount(0, newcount);
						qle->SendUpdateAddKill(0,0);
						qle->UpdatePlayerFields();
						return;
					}
				}
			}
		}
};

class Plaguethis_Gossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 40002, plr);
			Menu->AddItem(0, "Vers où aimeriez-vous voler ?", 2);
			if(plr->GetQuestLogForEntry(11332) != NULL)
				Menu->AddItem(0, "Greer, j'ai besoin d'un griffon de vol et quelques bombes à lacher sur New Agamand !", 1);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* Code)
		{
			Creature* pCreature = (pObject->IsCreature()) ? static_cast<Creature *>(pObject) : NULL;
			if(pCreature == NULL)
				return;

			switch(IntId)
			{
				case 1:
					{
						Item* item;
						item = objmgr.CreateItem(33634 , plr);
						item->SetCount(10);

						if(!plr->GetItemInterface()->AddItemToFreeSlot(item))
						{
							plr->GetSession()->SendNotification("Pas d'emplacement libre trouvé dans votre inventaire !");
							item->Delete();
						}
						else
						{
							plr->GetSession()->SendItemPushResult(item, false, true, false, true, plr->GetItemInterface()->LastSearchResult()->ContainerSlot, 
																  plr->GetItemInterface()->LastSearchResult()->Slot, item->GetCount());

						}

						if(pCreature->GetEntry() == 23859)
						{
							TaxiPath* path = sTaxiMgr.GetTaxiPath(745);
							plr->TaxiStart(path, 17759, 0);
						}
						break;
					}
				case 2:
					{
						plr->GetSession()->SendTaxiList(pCreature);
						break;
					}
			}
		}
};

void SetupFjordhurlant(ScriptMgr * mgr)
{
	GossipScript* Plague = new Plaguethis_Gossip();  // thx  Dzjhenghiz
	mgr->register_gossip_script(23859, Plague);

	mgr->register_creature_script(23643, &ChillmereScourge::Create);
	mgr->register_creature_script(23645, &ChillmereScourge::Create);
	mgr->register_creature_script(23644, &ChillmereScourge::Create);
	mgr->register_creature_script(24540, &ChillmereScourge::Create);
	mgr->register_creature_script(24485, &ChillmereScourge::Create);
	mgr->register_creature_script(23653, &Baleheim::Create);
	mgr->register_creature_script(23655, &Baleheim::Create);
	mgr->register_creature_script(24015, &Baleheim::Create);
	mgr->register_creature_script(23866, &NorthFleet::Create);
	mgr->register_creature_script(23934, &NorthFleet::Create);
	mgr->register_creature_script(23946, &NorthFleet::Create);
	mgr->register_creature_script(23794, &NorthFleet::Create);
	mgr->register_creature_script(23793, &NorthFleet::Create);
}