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

// Quête : La runeforge : se préparer à la bataille par Crash pour WAAD

bool Seprepareralabataille(uint32 i, Spell* pSpell)
{
	if ( pSpell == NULL || pSpell->m_caster == NULL || !pSpell->m_caster->IsPlayer() )
	return true;

	Player *pPlayer = static_cast< Player* >( pSpell->m_caster );

	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 12842 );
	if ( pQuest != NULL )
	{
		if ( pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0] )
		{
			pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
			pQuest->SendUpdateAddKill( 0,0 );
			pQuest->UpdatePlayerFields();
			pQuest->SendQuestComplete();
		}
	}
	return true;
};

// La quête aurait du fonctionner parfaitement, cependant elle reste commenté pour l'instant car
// il y a un problème avec le data1 ou le type, le personnage continue a essayer d'ouvrir avec la clef. ( A VOIR )
// Quête : La faim sans fin par Crash pour WAAD
/*
#define INITIE_1				29519
#define INITIE_2				29565
#define INITIE_3				29567
#define INITIE_4				29520

class PrisonAcherus : GameObjectAIScript
{
public:
    PrisonAcherus(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
    static GameObjectAIScript *Create(GameObject* GO) 
	{ 
	return new PrisonAcherus(GO); 
	}

    void OnActivate(Player* pPlayer)
    {
        QuestLogEntry * pQuest = pPlayer->GetQuestLogForEntry(12848);
        if(!pQuest)
        return;
            
            float SSX = pPlayer->GetPositionX();
            float SSY = pPlayer->GetPositionY();
            float SSZ = pPlayer->GetPositionZ();
            
            Creature* pCreature = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ);
		     
			if(!pCreature || !pCreature->isAlive())
				return;
			
			if (pCreature->GetEntry() == INITIE_1 || pCreature->GetEntry() == INITIE_2 || pCreature->GetEntry() == INITIE_3 || pCreature->GetEntry() == INITIE_4)
			{
			pPlayer->SendChatMessage(CHAT_MSG_SAY, LANG_UNIVERSAL, "Je gagnerai ma liberté pour quitter cet endroit maudit !");
            pCreature->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
			pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Que vos peurs vous consument !");
            pCreature->GetAIInterface()->SetNextTarget(pPlayer);
            pCreature->GetAIInterface()->AttackReaction(pPlayer, 1, 0);
			
			if(pQuest->GetMobCount(0) != 0)
			return;

			pQuest->SetMobCount(0, 1);
			pQuest->SendUpdateAddKill(0,0);
			pQuest->UpdatePlayerFields();
			}
		
    }
};*/

void SetupScarletEnclave(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(53341, &Seprepareralabataille);
	mgr->register_dummy_spell(53343, &Seprepareralabataille);
	/*
	mgr->register_gameobject_script(191588, &PrisonAcherus::Create);
	mgr->register_gameobject_script(191577, &PrisonAcherus::Create);
    mgr->register_gameobject_script(191580, &PrisonAcherus::Create);
    mgr->register_gameobject_script(191581, &PrisonAcherus::Create);
    mgr->register_gameobject_script(191582, &PrisonAcherus::Create);
    mgr->register_gameobject_script(191583, &PrisonAcherus::Create);
    mgr->register_gameobject_script(191584, &PrisonAcherus::Create);
    mgr->register_gameobject_script(191585, &PrisonAcherus::Create);
    mgr->register_gameobject_script(191586, &PrisonAcherus::Create);
    mgr->register_gameobject_script(191587, &PrisonAcherus::Create);
    mgr->register_gameobject_script(191589, &PrisonAcherus::Create);
    mgr->register_gameobject_script(191590, &PrisonAcherus::Create);*/
}