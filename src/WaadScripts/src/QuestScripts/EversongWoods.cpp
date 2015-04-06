/*
 * Sun++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentcommunity.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://sunplusplus.info//>
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
#include "../InstanceScripts/Base.h"

bool PoweringOurDefenses(uint32 i, Spell* pSpell)
{
	if(pSpell->m_caster->IsPlayer() == false)
    return true;

	Player * plr = (Player*)pSpell->m_caster;
	if( plr == NULL )
		return true;

	QuestLogEntry *qle = plr->GetQuestLogForEntry( 8490 );
	if( qle == NULL )
		return true;

	// Angelis : Need to script the scourge attack

	if( qle && qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0] )
	{
		qle->SetMobCount(0, qle->GetMobCount(0)+1);
		qle->SendUpdateAddKill(0,0); // A Verif: Trouver le GUID (Branruz)
		qle->UpdatePlayerFields();
	}
	return true;
}

static Coords Prospecteur_garde_enclumeWaypoints[] =
{
	{ 9294.834f, -6681.092f, 22.428f, 1.284f, 0 },
	{ 9297.834f, -6671.092f, 22.387f, 0.793f, 0 },
	{ 9310.375f, -6658.936f, 22.43f,  2.046f, 0 },
	{ 9306.596f, -6650.905f, 25.222f, 2.666f, 0 },
	{ 9299.666f, -6648.099f, 28.39f,  3.468f, 0 },
	{ 9292.345f, -6650.509f, 30.908f, 4.249f, 0 },
	{ 9289.426f, -6657.825f, 31.829f, 6.154f, 0 },
	{ 9294.095f, -6658.863f, 34.482f, 6.063f, 0 },
};

class Prospecteur_garde_enclumeGossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* Plr, bool AutoSend);
		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* EnteredCode);
		void GossipEnd(Object* pObject, Player* Plr) {};
};

void Prospecteur_garde_enclumeGossip::GossipHello(Object* pObject, Player* Plr, bool AutoSend)
{
	GossipMenu* Menu;
	objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
	Menu->AddItem(0, "Montrez-moi...", 1);

	if(AutoSend)
		Menu->SendTo(Plr);
}

void Prospecteur_garde_enclumeGossip::GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* EnteredCode)
{
	if(!pObject->IsCreature())
		return;
	Creature* _unit = static_cast<Creature *>(pObject);
	switch(IntId)
	{
		case 1:
			{
				QuestLogEntry* qLogEntry = Plr->GetQuestLogForEntry(8483);
				if(qLogEntry != NULL)
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Suivez-moi !");
					_unit->m_custom_waypoint_map = new WayPointMap;
					_unit->GetAIInterface()->SetWaypointMap(_unit->m_custom_waypoint_map);
					WayPoint* wp = new WayPoint;
					wp->id = 1;
					wp->x = _unit->GetSpawnX();
					wp->y = _unit->GetSpawnY();
					wp->z = _unit->GetSpawnZ() + 2.05f;
					wp->o = _unit->GetSpawnO();
					wp->flags = 256;
					wp->backwardskinid = wp->forwardskinid = _unit->GetCreatureInfo()->Male_DisplayID;
					wp->backwardemoteid = wp->forwardemoteid = 0;
					wp->backwardemoteoneshot = wp->forwardemoteoneshot = false;
					wp->waittime = 0;
					_unit->m_custom_waypoint_map->push_back(wp);
					for(uint32 i = 0; i < sizeof(Prospecteur_garde_enclumeWaypoints) / sizeof(Coords); i++)
					{
						wp = new WayPoint;
						wp->id = i + 2;
						wp->x = Prospecteur_garde_enclumeWaypoints[i].mX;
						wp->y = Prospecteur_garde_enclumeWaypoints[i].mY;
						wp->z = Prospecteur_garde_enclumeWaypoints[i].mZ;
						wp->o = Prospecteur_garde_enclumeWaypoints[i].mO;
						wp->flags = 256;
						wp->backwardskinid = wp->forwardskinid = _unit->GetCreatureInfo()->Male_DisplayID;
						wp->backwardemoteid = wp->forwardemoteid = 0;
						wp->backwardemoteoneshot = wp->forwardemoteoneshot = false;
						wp->waittime = 1000;
						_unit->m_custom_waypoint_map->push_back(wp);
					}
				}
				else
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Je n'ai rien pour vous. Disparaissez !");
				}
				GossipEnd(pObject, Plr);
			}
			break;
	}
}

class Prospecteur_garde_enclumeAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Prospecteur_garde_enclumeAI);
		Prospecteur_garde_enclumeAI(Creature* c) : CreatureAIScript(c)
		{
		}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == sizeof(Prospecteur_garde_enclumeWaypoints) / sizeof(Coords) && bForwards)
			{
				_unit->GetAIInterface()->SetWaypointMap(NULL);
				_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
				_unit->_setFaction();
				RegisterAIUpdateEvent(10000);
			}
			else if(iWaypointId == 2 && !bForwards)
			{
				_unit->GetAIInterface()->SetWaypointMap(NULL);
				_unit->m_custom_waypoint_map = NULL;
				_unit->GetAIInterface()->MoveTo(_unit->GetSpawnX(), _unit->GetSpawnY(), _unit->GetSpawnZ() + 2.05f, _unit->GetSpawnO());
				_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35);
				_unit->_setFaction();
			}
		}

		void AIUpdate()
		{
			if(!_unit->CombatStatus.IsInCombat())
			{
				RemoveAIUpdateEvent();
				_unit->GetAIInterface()->SetWaypointMap(_unit->m_custom_waypoint_map);
			}
		}

		void OnDied(Unit* mKiller)
		{
			RemoveAIUpdateEvent();
			if(_unit->GetAIInterface()->GetWayPointsCount() != NULL)
				_unit->GetAIInterface()->SetWaypointMap(NULL);
			else if(_unit->m_custom_waypoint_map != NULL)
			{
				for(WayPointMap::iterator itr = _unit->m_custom_waypoint_map->begin(); itr != _unit->m_custom_waypoint_map->end(); ++itr)
					delete(*itr);
				delete _unit->m_custom_waypoint_map;
			}
			_unit->m_custom_waypoint_map = NULL;

				_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35);
				_unit->_setFaction();
		}
};
/*--------------------------------------------------------------------------------------------------------*/
/* Fix quête L'espion des nains par Crash pour AscentWaad

class Prospecteur_garde_enclume : public QuestScript
{
public:
	void OnQuestStart(Player * mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;

		Creature *prospecteur = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), mTarget->GetPositionZ(), 15420);
		if(!prospecteur)
		{
			Log.Warning("WAADSCRIPT","Impossible de trouver le prospecteur");
			return;
		}
		prospecteur->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
		prospecteur->_setFaction();
	}

	void OnQuestCancel(Player *pPlayer)
	{
		if ( pPlayer == NULL ) return;

		Creature *prospecteur = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 15420);
		if(!prospecteur)
		{
			Log.Warning("WAADSCRIPT","Impossible de trouver le prospecteur");
			return;
		}
		prospecteur->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35);
		prospecteur->_setFaction();
	}

	void OnDied(Unit * mKiller)
	{
		if ( !mKiller->IsPlayer() )
			return;

		Creature *prospecteur = mKiller->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mKiller->GetPositionX(), mKiller->GetPositionY(), mKiller->GetPositionZ(), 15420);
		if(!prospecteur)
		{
			Log.Warning("WAADSCRIPT","Impossible de trouver le prospecteur");
			return;
		}
		prospecteur->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
		prospecteur->_setFaction();
	}

	void OnQuestComplete(Player* pPlayer, QuestLogEntry *qLogEntry)
	{
		if ( pPlayer == NULL )
			return;

		Creature *prospecteur = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 15420);
		if(!prospecteur)
		{
			Log.Warning("WAADSCRIPT","Impossible de trouver le prospecteur");
			return;
		}
		prospecteur->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35);
		prospecteur->_setFaction();
	}
};*/

void SetupEversongWoods(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(28247, &PoweringOurDefenses);
	mgr->register_gossip_script(15420, new Prospecteur_garde_enclumeGossip);
	mgr->register_creature_script(15420, &Prospecteur_garde_enclumeAI::Create);
}
