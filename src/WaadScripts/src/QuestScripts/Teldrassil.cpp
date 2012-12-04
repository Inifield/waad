/*
 * Waad Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2010 Waad Team <http://arbonne.games-rpg.net/>
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

#define _BRUME_ID_				3568
#define ESCORTE_BRUME_QUESTID	938
#define ARYNIA					3519

class Brume : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(Brume);
	Brume(Creature* pCreature) : CreatureAIScript(pCreature) 
	{
		RegisterAIUpdateEvent(1000); // en msec
	}

	void AIUpdate()
    {
		char _buff_texte_[255];
		char _buff_texte_2[255];

		float SSX = _unit->GetPositionX();
		float SSY = _unit->GetPositionY();
		float SSZ = _unit->GetPositionZ();

		Creature* pArynia = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, ARYNIA);
		if(pArynia == NULL)
		{
			return;
		}
		Creature* pBrume = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, _BRUME_ID_);
		if(pBrume == NULL)
		{
			return;
		}
		
		for(set<Player*>::iterator itr = _unit->m_inRangePlayers.begin(); itr != _unit->m_inRangePlayers.end(); ++itr)
		{
			Player *pEscorter;
			pEscorter = *itr;

			 if(_unit->GetAIInterface()->GetUnitToFollow() == pEscorter)
			 {
				 if(pEscorter->GetDistance2dSq(pArynia) < 200)
				 {
					 snprintf(_buff_texte_,250,"Par tous les diables ! Je craignais ne plus jamais vous revoir ! Oui, je vais bien, ne vous inquietez pas pour moi.\n Vous devez vous reposer et retrouver vos forces.");
					 pArynia->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,_buff_texte_ );
					 snprintf(_buff_texte_2,250,"Brume grogne en guise de remerciement avant de s'éloigner dans la foret.");
					 pBrume->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL,_buff_texte_2 );
					 pBrume->m_escorter = NULL;
					 pBrume->GetAIInterface()->SetAIState(STATE_IDLE);
					 pBrume->GetAIInterface()->SetUnitToFollow(NULL);
					 // Sur offi brume court dans les bois, ici un Despawn fera l'affaire.
					 pBrume->Despawn(5000,1000);
				 }
			 }
		}

	}
	
};

class Quete_de_Brume : public QuestScript 
{ 
public:

	void OnQuestStart( Player * mTarget, QuestLogEntry * qLogEntry)
	{
		if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL ) 
		{
			Log.Error("Quete_de_Brume","Player non present"); // Ne peut pas arriver (normalement ;) )
			return;
		}

		float SSX = mTarget->GetPositionX();
		float SSY = mTarget->GetPositionY();
		float SSZ = mTarget->GetPositionZ();


		Creature* pBrume = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, _BRUME_ID_);
		if(pBrume == NULL)
		{
			Log.Error("Quete_de_Brume","%s n'est pas avec Brume",mTarget->GetName());
			return;
		}

		pBrume->m_escorter = mTarget;   
		pBrume->GetAIInterface()->SetUnitToFollow(mTarget);
	    pBrume->GetAIInterface()->SetFollowDistance(3.0f);
		pBrume->GetAIInterface()->ResetMoveTimer();

		
	}

	void OnQuestCancel( Player * mTarget)
	{
		float SSX = mTarget->GetPositionX();
		float SSY = mTarget->GetPositionY();
		float SSZ = mTarget->GetPositionZ();

		Creature* pBrume = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, _BRUME_ID_);
		if(pBrume == NULL)
		{
			Log.Error("Quete_de_Brume","%s n'est pas avec Brume",mTarget->GetName());
			return;
		}
		pBrume->m_escorter = NULL;
		pBrume->GetAIInterface()->SetAIState(STATE_IDLE);
	    pBrume->GetAIInterface()->SetUnitToFollow(NULL);
		pBrume->Despawn(5000,1000);
	}
};


void SetupTeldrassil(ScriptMgr *mgr)
{
  mgr->register_quest_script(ESCORTE_BRUME_QUESTID, CREATE_QUESTSCRIPT(Quete_de_Brume));	// Escorte	
  mgr->register_creature_script(_BRUME_ID_, &Brume::Create);
}
