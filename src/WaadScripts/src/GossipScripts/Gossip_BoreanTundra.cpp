 /* WaadAscent Scripts for Ascent MMORPG Server
 * Copyright (C) 2008-2014 Waad   Team <http://arbonne.games-rpg.net/>
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

class TiareGossipScript : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player * plr, bool AutoSend)
		{
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu,pObject->GetGUID(), 1, plr);
			 Menu->AddItem( 0, "Téléportez-moi à Amber Ledge !", 1);
			        
			if(AutoSend)
				Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
		{
			Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
			if(pObject->GetTypeId()!=TYPEID_UNIT)
				return;
		
			switch(IntId)
			{
				case 1:
				{
					plr->Gossip_Complete();
					pCreature->CastSpell(plr, dbcSpell.LookupEntry(50135), true);
				}break;		
			}
		}

		void Destroy()
		{
			delete this;
		}
};

void SetupBoreanTundraGossip(ScriptMgr* mgr)
{
	GossipScript * TiareGossipScriptGossip = (GossipScript*) new TiareGossipScript;
	mgr->register_gossip_script(30051, TiareGossipScriptGossip); // // Tiare
}
