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

/*--------------------------------------------------------------------------------------------------------*/
// Fix quête L'espion des nains par Crash pour AscentWaad

class Prospecteur_garde_enclume : public QuestScript
{
public:
	void OnQuestStart(Player * mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
			return;

		Creature *prospecteur = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), mTarget->GetPositionZ(), 15420);
		prospecteur->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
		prospecteur->_setFaction();
	}

	void OnQuestCancel(Player *pPlayer)
	{
		if ( pPlayer == NULL ) return;

		Creature *prospecteur = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 15420);
		prospecteur->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35);
		prospecteur->_setFaction();
	}

	void OnDied(Unit * mKiller)
	{
		if ( !mKiller->IsPlayer() )
			return;

		Creature *prospecteur = mKiller->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mKiller->GetPositionX(), mKiller->GetPositionY(), mKiller->GetPositionZ(), 15420);
		prospecteur->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 14);
		prospecteur->_setFaction();
	}

	void OnQuestComplete(Player* pPlayer, QuestLogEntry *qLogEntry)
	{
		if ( pPlayer == NULL )
			return;

		Creature *prospecteur = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 15420);
		prospecteur->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35);
		prospecteur->_setFaction();
	}
};

void SetupEversongWoods(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(28247, &PoweringOurDefenses);
	mgr->register_quest_script(8483, CREATE_QUESTSCRIPT(Prospecteur_garde_enclume));
}
