/*
 * ArcEmu scripts adapted by WaadAscent Scripts for Ascent MMORPG Server
 * Copyright (C) 2008-2014 Waad   Team <http://arbonne.games-rpg.net/>
 * Copyright (C) 2009-2012 ArcEmu Team <http://www.arcemu.org/>
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
 
 class DedicationOfHonorAI : public GameObjectAIScript
{
	public:
		
		DedicationOfHonorAI(GameObject* go) : GameObjectAIScript(go){}
		static GameObjectAIScript *Create(GameObject * GO) { return new DedicationOfHonorAI(GO); }
		~DedicationOfHonorAI() {}

		void OnActivate(Player* player)
		{
			GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu,_gameobject->GetGUID(), 15921, player);
			Menu->AddItem( 0, "Voir la chute du Roi Lich.",1);
		}
};

class DedicationOfHonorGossip : public GossipScript
{
	public:
		DedicationOfHonorGossip() : GossipScript(){}

		void OnSelectOption(Object* object, Player* player, uint32 Id, const char* enteredcode)
		{
			uint32 id = 16;	//video id
			player->GetSession()->OutPacket(SMSG_TRIGGER_MOVIE, sizeof(uint32), &id);
			player->Gossip_Complete();
		}
};

void SetupDalaranGossip(ScriptMgr* mgr)
{
	mgr->register_gameobject_script(202443, &DedicationOfHonorAI::Create);
	mgr->register_go_gossip_script(202443, new DedicationOfHonorGossip);
}
