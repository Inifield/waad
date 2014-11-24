/* Adapted by WaadAscent Scripts for Ascent MMORPG Server
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2008-2014 Waad   Team <http://arbonne.games-rpg.net/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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

class CassaCrimsonwing_Gossip : public GossipScript
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 11224, plr);

			if(plr->GetQuestLogForEntry(11142) != NULL)
				Menu->AddItem( 0, "Dame Jaina m'a dit de vous suggérer d'utiliser l'aide d'un griffon pour sonder l'Île d'Alcaz.", 1);

			Menu->SendTo(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			plr->GetQuestLogForEntry(11142)->SendQuestComplete();
			plr->TaxiStart(sTaxiMgr.GetTaxiPath(724), 1147, 0);     // Gryph
		}

		void Destroy() { delete this; }

};

class CaptainGarranVimes_Gossip : public GossipScript
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			//Send quests and gossip menu.
			uint32 Text = objmgr.GetGossipTextForNpc(pObject->GetEntry());
			if(NpcTextStorage.LookupEntry(Text) == NULL)
				Text = 1;

			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), Text, plr);

			if((plr->GetQuestLogForEntry(11123) != NULL) || (plr->GetQuestRewardStatus(11123) == 0))
				Menu->AddItem( 0, "Avez-vous entendu parlé de l'auberge de Shady Rest ?", 0);

			Menu->SendTo(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1794, plr);
			Menu->SendTo(plr);
		}

		void Destroy() { delete this; }
};

void SetupTheramoreGossip(ScriptMgr* mgr)
{
	mgr->register_gossip_script(23704, new CassaCrimsonwing_Gossip); // Cassa Crimsonwing
	mgr->register_gossip_script(4944, new CaptainGarranVimes_Gossip); // Captain Garran Vimes
}
