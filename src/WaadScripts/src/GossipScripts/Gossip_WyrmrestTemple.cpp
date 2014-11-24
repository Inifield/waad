/* Adapted by WaadAscent Scripts for Ascent MMORPG Server
 * FrozenThrone Scripts
 * Copyright (C) 2008-2014 Waad   Team <http://arbonne.games-rpg.net/>
 * Copyright (C) 2008-2009 FrozenThrone Shard <http://www.dowlee.it/ft>
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

//	Wyrmrest Temple Flighter
// Top to Bottom & Top to Middle
#define CN_TOP 26949
#define GOSSIP_TOP_TO_BOTTOM "Oui, s'il vous plaît, je voudrais revenir au niveau du temple de la terre."
#define GOSSIP_TOP_TO_MIDDLE "J'aimerais aller, Seigneur Afrasastrasz, au milieu du temple."
#define NPCTEXT_TOP 12714
// Middle to Top & Middle to Bottom
#define CN_MIDDLE 27575
#define GOSSIP_MIDDLE_TO_TOP "Mon Seigneur, j'ai besoin de me rendre au sommet du temple."
#define GOSSIP_MIDDLE_TO_BOTTOM "Puis-je obtenir un taxi pour revenir au niveau du sol, Seigneur Afrasastrasz ?"
#define NPCTEXT_MIDDLE 12887
// Bottom to Top & Bottom to Middle
#define CN_BOTTOM 26443
#define GOSSIP_BOTTOM_TO_TOP "Steward, s'il vous plaît permettez-moi de monter un des Drake à la chambre de la reine au sommet du temple."
#define GOSSIP_BOTTOM_TO_MIDDLE "Seigneur Afrasastrasz, pouvez-vous demander à un Drake de me prendre au milieu du temple ?"
#define NPCTEXT_BOTTOM 12713

class SCRIPT_DECL WyrmrestTemple_FlightGossip : public GossipScript
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			GossipMenu *Menu;
			switch(pObject->GetEntry())
			{
				
				case CN_TOP:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu,pObject->GetGUID(), NPCTEXT_TOP, plr);
						Menu->AddItem( 0, GOSSIP_TOP_TO_BOTTOM, 1);
						Menu->AddItem( 0, GOSSIP_TOP_TO_MIDDLE, 2);
					}
					break;
				case CN_MIDDLE:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu,pObject->GetGUID(), NPCTEXT_MIDDLE, plr);
						Menu->AddItem( 0, GOSSIP_MIDDLE_TO_TOP, 3);
						Menu->AddItem( 0, GOSSIP_MIDDLE_TO_BOTTOM, 4);
					}
					break;
				case CN_BOTTOM:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu,pObject->GetGUID(), NPCTEXT_BOTTOM, plr);
						Menu->AddItem( 0, GOSSIP_BOTTOM_TO_TOP, 5);
						Menu->AddItem( 0, GOSSIP_BOTTOM_TO_MIDDLE, 6);
					}
					break;
			}

			Menu->SendTo(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			plr->Gossip_Complete();
			switch(Id)
			{
				case 1:
					plr->TaxiStart(sTaxiMgr.GetTaxiPath(879), 6371, 0);     // Drake
					break;
				case 2:
					plr->TaxiStart(sTaxiMgr.GetTaxiPath(880), 6371, 0);     // Drake
					break;
				case 3:
					plr->TaxiStart(sTaxiMgr.GetTaxiPath(881), 6371, 0);     // Drake
					break;
				case 4:
					plr->TaxiStart(sTaxiMgr.GetTaxiPath(882), 6371, 0);     // Drake
					break;
				case 5:
					plr->TaxiStart(sTaxiMgr.GetTaxiPath(878), 6371, 0);     // Drake
					break;
				case 6:
					plr->TaxiStart(sTaxiMgr.GetTaxiPath(883), 6371, 0);     // Drake
					break;
				default:
					break;
			}
		}

};

void SetupWyrmrestTempleGossip(ScriptMgr* mgr)
{
	GossipScript * WyrmrestTempleFlightGossip =(GossipScript*) new WyrmrestTemple_FlightGossip;

	mgr->register_gossip_script(CN_TOP, WyrmrestTempleFlightGossip);	// Torastrasza <Majordomo to the Ruling Council>
	mgr->register_gossip_script(CN_MIDDLE, WyrmrestTempleFlightGossip);	// Lord Afrasastrasz <Commander of Wyrmrest Temple Defenses>
	mgr->register_gossip_script(CN_BOTTOM, WyrmrestTempleFlightGossip);	// Tariolstrasz <Steward of Wyrmrest Temple>
}
