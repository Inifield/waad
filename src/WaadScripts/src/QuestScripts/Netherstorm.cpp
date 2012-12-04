/*
 * WEmu Scripts for WEmu MMORPG Server
 * Copyright (C) 2008 WEmu Team
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
#include "EAS/EasyFunctions.h"

class Veronia : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(Plr->GetQuestLogForEntry(10652))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
			Menu->AddItem( 0, "I'm ready", 1);
			Menu->SendTo(Plr);
		}
	}

	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature* creat = static_cast<Creature*>(pObject);
		switch(IntId)
		{
		case 1:
			creat->CastSpell(Plr, dbcSpell.LookupEntry(34905), true);
			break;
		}
	}
}; 

bool TheArchmageStaff(uint32 i, Spell * pSpell)
{
	if(!pSpell->m_caster->IsPlayer())
		return true;

	Player *plr = (Player*)pSpell->m_caster;
	
	QuestLogEntry *qle = plr->GetQuestLogForEntry(10173);
	if(!qle)
		return true;

	sEAS.SpawnCreature(plr, 19493, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(),0, 0);
	return true;
}

bool FindingTheKeymaster(uint32 i, Spell * pSpell)
{
	if(!pSpell->m_caster->IsPlayer())
		return true;

	Player *plr = (Player*)pSpell->m_caster;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(10256);
	if(!qle)
		return true;

	qle->SetMobCount(0, qle->GetMobCount(0)+1);
	qle->SendUpdateAddKill(0,0); // A Verif: Trouver le GUID (Branruz)
	qle->UpdatePlayerFields();
	return true;
}

bool TheFleshLies(uint32 i, Spell * pSpell)
{
	if(!pSpell->m_caster->IsPlayer())
	return true;

	Player *plr = (Player*)pSpell->m_caster;
	Creature *target = (Creature*)plr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(plr->GetPositionX(), plr->GetPositionY() , plr->GetPositionZ(), 20561);

	if(target == NULL)
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry(10345);
  
	if(qle == NULL)
		return true;

	GameObject *obj = NULL;

	if(qle && qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
	{
		qle->SetMobCount(0, qle->GetMobCount(0)+1);
		qle->SendUpdateAddKill(0,0); // A Verif: Trouver le GUID (Branruz)
		
		obj = sEAS.SpawnGameobject(plr, 183816, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 1);
		sEAS.GameobjectDelete(obj, 1*30*1000);
	}
	target->Despawn(2000, 60*1000);
	plr->UpdateNearbyGameObjects();
	qle->UpdatePlayerFields();
	return true;
}

void SetupNetherstorm(ScriptMgr* mgr)
{
	mgr->register_gossip_script(20162, CREATE_GOSSIPSCRIPT(Veronia));
	mgr->register_dummy_spell(34992, &TheArchmageStaff);
	mgr->register_dummy_spell(34717, &FindingTheKeymaster);
	mgr->register_dummy_spell(35372, &TheFleshLies);
}