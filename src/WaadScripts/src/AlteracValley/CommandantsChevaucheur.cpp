/* Scripts du champ de bataille de la vallé d'Alterac par Randdrick pour la communauté Waad
Basé sur le travail de la team Moon++  - Projet Moon -
Soumis à la licence Creative Common - Paternité - pas d'utilisation commerciale -
Pour en savoir plus : http://creativecommons.org/licenses/by-nc/2.0/fr/
*/
#include "StdAfx.h"
#include "Setup.h"


// Commandant Chevaucheur AI

#define COMMANDANT_CHEVAUCHEUR_FOUDREPIQUE 13577 //Commandant chevaucheur de l'alliance
#define CLEAVE 15284 // Enchainement
#define MORTALSTRIKE 16856 // Frappe mortelle

class COMMANDANTCHEVAUCHEURFOUDRE : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(COMMANDANTCHEVAUCHEURFOUDRE);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    COMMANDANTCHEVAUCHEURFOUDRE(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		spells[0].info = dbcSpell.LookupEntry(CLEAVE);
		spells[0].targettype = TARGET_ATTACKING; 
		spells[0].instant = true;
		spells[0].perctrigger = 40.0f;
		spells[0].maxdist2cast = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(MORTALSTRIKE);
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = true;
		spells[1].perctrigger = 40.0f;
		spells[1].maxdist2cast = 5.0f;
		spells[1].attackstoptimer = 1000;
    }
    
    void OnCombatStart(Unit* mTarget)
    {
        RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	    void OnCombatStop(Unit *mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(Unit * mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
    }

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    Unit *target = NULL;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
protected:

	int nrspells;
};

void SetupCommandantAlterac(ScriptMgr * mgr)
{
	mgr->register_creature_script(COMMANDANT_CHEVAUCHEUR_FOUDREPIQUE, &COMMANDANTCHEVAUCHEURFOUDRE::Create);
}
