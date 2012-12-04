/* Scripts du champ de bataille de la vallé d'Alterac par Randdrick pour la communauté Waad
Basé sur le travail de la team Moon++  - Projet Moon -
Soumis à la licence Creative Common - Paternité - pas d'utilisation commerciale -
Pour en savoir plus : http://creativecommons.org/licenses/by-nc/2.0/fr/
*/
#include "StdAfx.h"
#include "Setup.h"

// Chef d'escadrille Foudrepique

#define CHEF_ICHMAN 13434 //Chef d'escadrille Ichman
#define CHEF_SLIDORE 13438 //Chef d'escadrille Slidore
#define CHEF_VIPORE 13439 //Chef d'escadrille Vipore
#define STRIKE 15580 // Frappe
#define REND 16509 // Pourfendre

class CHEFFOUDREPIQUE : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CHEFFOUDREPIQUE);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    CHEFFOUDREPIQUE(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		     spells[0].info = dbcSpell.LookupEntry(STRIKE);
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = true;
		     spells[0].perctrigger = 40.0f;
			 spells[0].maxdist2cast = 5.0f;
		     spells[0].attackstoptimer = 1000;

		     spells[1].info = dbcSpell.LookupEntry(REND);
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

// Chefs d'escadrille Gouffrefer

#define CHEF_JEZTOR 13434 //Chef d'escadrille Ichman
#define CHEF_MULVERICK 13181 //Chef d'escadrille Mulverick
#define SUNDER_ARMOR 16509 // Fracasser armure
#define STRIKE 15580 // Frappe


class CHEFGOUFFREFER : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CHEFGOUFFREFER );
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    CHEFGOUFFREFER (Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		     spells[0].info = dbcSpell.LookupEntry(SUNDER_ARMOR);
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = true;
		     spells[0].perctrigger = 40.0f;
			 spells[0].cooldown = 30.0f;
			 spells[0].maxdist2cast = 5.0f;
		     spells[0].attackstoptimer = 1000;

		     spells[1].info = dbcSpell.LookupEntry(STRIKE);
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

void SetupChefsEscadrille(ScriptMgr * mgr)
{
mgr->register_creature_script(CHEF_ICHMAN, &CHEFFOUDREPIQUE::Create);
mgr->register_creature_script(CHEF_SLIDORE, &CHEFFOUDREPIQUE::Create);
mgr->register_creature_script(CHEF_VIPORE, &CHEFFOUDREPIQUE::Create);
mgr->register_creature_script(CHEF_JEZTOR, &CHEFGOUFFREFER ::Create);
mgr->register_creature_script(CHEF_MULVERICK, &CHEFGOUFFREFER ::Create);
}