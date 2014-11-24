/*
 * Yelly Database Scripts
 * Copyright (C) 2008-2014 WaadAscent Team <http://arbonne.games-rpg.net/>
 * Copyright (C) 2006-2008 U2 Team <http://www.undzwei.eu/>
 * Copyright (C) 2007-2008 Yelly Team
 * Copyright (C) 2009 WhyScripts Team <http://www.whydb.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "Setup.h"
#include "EAS\EasyFunctions.h"

// quest #8304 - Dearest Natalia
class SCRIPT_DECL DearestNatalia1 : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* Plr)
		{
			GossipMenu* Menu;
			QuestLogEntry* en = Plr->GetQuestLogForEntry(8304);

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7736, Plr);

			if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
				Menu->AddItem(0, "Salutations, Rutgar. Le Commandant m'a demandé de glaner quelques informations au sujet de la disparition de sa femme.", 3);

			Menu->SendTo(Plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:
					GossipHello(pObject, Plr);
					break;
				case 3:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7755, Plr);
						Menu->AddItem(0, "Cela semble dangereux.", 4);
						Menu->SendTo(Plr);
					}
					break;
				case 4:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7756, Plr);
						Menu->AddItem(0, "Qu'est-ce qui lui est arrivé après ?", 5);
						Menu->SendTo(Plr);
					}
					break;
				case 5:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7757, Plr);
						Menu->AddItem(0, "Natalia ?", 6);
						Menu->SendTo(Plr);
					}
					break;
				case 6:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7758, Plr);
						Menu->AddItem(0, "Quelles sont les exigences ?", 7);
						Menu->SendTo(Plr);
					}
					break;
				case 7:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7759, Plr);
						Menu->AddItem(0, "Perdu ? Que voulez-vous dire ?", 8);
						Menu->SendTo(Plr);
					}
					break;
				case 8:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7760, Plr);
						Menu->AddItem(0, "Possédée par quoi ?", 9);
						Menu->SendTo(Plr);
					}
					break;
				case 9:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7761, Plr);
						Menu->AddItem(0, "Je reviendrai une fois j'aurai réparé ce gâchis.", 10);
						Menu->SendTo(Plr);
					}
					break;
				case 10:
					{
						QuestLogEntry* en = Plr->GetQuestLogForEntry(8304);
						if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
						{
							en->SetMobCount(1, 1);
							en->SendUpdateAddKill(1,0);
							en->UpdatePlayerFields();
						}
					}
					break;
			}
		}
};

class SCRIPT_DECL DearestNatalia2 : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* Plr)
		{
			GossipMenu* Menu;
			QuestLogEntry* en = Plr->GetQuestLogForEntry(8304);

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7735, Plr);

			// you need to speak to Rutgar first !!
			if(en && (en->GetMobCount(0) < en->GetQuest()->required_mobcount[0]) && (en->GetMobCount(1) == 1))
				Menu->AddItem(0, "Bonjour, Frankal. J'ai entendu dire que vous pourriez avoir quelques renseignements sur les allées et venues de Maîtresse Natalia Mar'alith.", 3);

			Menu->SendTo(Plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:
					GossipHello(pObject, Plr);
					break;
				case 3:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7762, Plr);
						Menu->AddItem(0, "C'est ce que je aime entendre.", 4);
						Menu->SendTo(Plr);
					}
					break;
				case 4:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7763, Plr);
						Menu->AddItem(0, "C'est étrange.", 5);
						Menu->SendTo(Plr);
					}
					break;
				case 5:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7764, Plr);
						Menu->AddItem(0, "Ne pouviez-vous pas gérer une seule prêtresse elfe de la nuit ?", 6);
						Menu->SendTo(Plr);
					}
					break;
				case 6:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7765, Plr);
						Menu->AddItem(0, "J'ai eu l'intention de vous poser des questions sur ce singe.", 7);
						Menu->SendTo(Plr);
					}
					break;
				case 7:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7766, Plr);
						Menu->AddItem(0, "Alors quoi ?", 8);
						Menu->SendTo(Plr);
					}
					break;
				case 8:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7767, Plr);
						Menu->AddItem(0, "Quelle histoire ! Alors elle est allée dans la Ruche de Regal et c'était la dernière fois que vous l'avez vu ?", 9);
						Menu->SendTo(Plr);
					}
					break;
				case 9:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7768, Plr);
						Menu->AddItem(0, "Merci pour les informations, Frankal.", 10);
						Menu->SendTo(Plr);
					}
					break;
				case 10:
					{
						QuestLogEntry* en = Plr->GetQuestLogForEntry(8304);
						if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
						{
							en->SetMobCount(0, 1);
							en->SendUpdateAddKill(0,0);
							en->UpdatePlayerFields();
						}
					}
					break;
			}
		}
};

#define GOSSIP1 "Que savez-vous à son propos ?"
#define GOSSIP2 "je suis tout ouïe, Demitrian."
#define GOSSIP3 "Continuez, s'il-vous plait."
#define GOSSIP4 "Une bataille ?"
#define GOSSIP5 "<Nod>"
#define GOSSIP6 "Pris au dépourvu ? Comment ?"
#define GOSSIP7 "Oh qu'est-ce que Ragnaros doit faire ensuite ?"

class SCRIPT_DECL Highlord_demitrianGossip : public GossipScript
{
	public:
		
		void GossipHello(Object* pObject, Player* pPlayer)
		{
			GossipMenu* Menu;
			//Send quests and gossip menu.
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 6812, pPlayer);
			
			if (pPlayer->HasQuestForItem(18563) && pPlayer->HasQuestForItem(18564))
			{
				if (pPlayer->HasQuestForItem(19016))
				{
				Menu->AddItem(0, GOSSIP1, 1);
				}
			}
			Menu->SendTo(pPlayer);
		};

		void GossipSelectOption(Object* pObject, Player*  pPlayer, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 1:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 6842, pPlayer);

						Menu->AddItem(0, GOSSIP2, 2);
						Menu->SendTo(pPlayer);
					}break;
				case 2:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 6843, pPlayer);

						Menu->AddItem(0, GOSSIP3, 3);
						Menu->SendTo(pPlayer);
					}break;
				case 3:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 6844, pPlayer);

						Menu->AddItem(0, GOSSIP4, 4);
						Menu->SendTo(pPlayer);
					}break;
				case 4:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 6867, pPlayer);

						Menu->AddItem(0, GOSSIP5, 5);
						Menu->SendTo(pPlayer);
					}break;
				case 5:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 6868, pPlayer);

						Menu->AddItem(0, GOSSIP6, 6);
						Menu->SendTo(pPlayer);
					}break;
				case 6:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 6869, pPlayer);

						Menu->AddItem(0, GOSSIP7, 7);
						Menu->SendTo(pPlayer);
					}break;
				case 7:
					sEAS.AddItem(19016, pPlayer);
					pPlayer->Gossip_Complete();
					break;
			};
	};
};

class Thunderan : public QuestScript
{
	public:
		void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			sEAS.SpawnCreature(mTarget, 14435, -6241.0f, 1715.0f, 4.8f, 0.605017f,0);
		}
};

void SetupSilithus(ScriptMgr* mgr)
{
	GossipScript* dearestNatalia1 = new DearestNatalia1();
	GossipScript* dearestNatalia2 = new DearestNatalia2();
	GossipScript* highlord_demitrianGossip = new Highlord_demitrianGossip();
	mgr->register_gossip_script(15170, dearestNatalia1);
	mgr->register_gossip_script(15171, dearestNatalia2);
	mgr->register_gossip_script(14347, highlord_demitrianGossip);
	mgr->register_quest_script(7786, new Thunderan);
}
