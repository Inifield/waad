/* Scripts du champ de bataille de la vallé d'Alterac par Randdrick pour la communauté Waad
Basé sur le travail de la team Moon++  - Projet Moon -
Soumis à la licence Creative Common - Paternité - pas d'utilisation commerciale -
Pour en savoir plus : http://creativecommons.org/licenses/by-nc/2.0/fr/
*/
#include "StdAfx.h"
#include "Setup.h"


// Gardes - défenseurs

#define CAPORAL_NOREG 13447 // Caporal Noreg Foudrepique
#define DEFENSEUR_FOUDRE 12050 //Défenseur Foudrepique
#define DEFENSEUR_VET 13331 //Défenseur Vétéran

#define SHIELD_BLOCK 22121 // Maîtrise du blocage
#define REVENGE 19130 // Vengeance

class CaporalNoregAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CaporalNoregAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    CaporalNoregAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		spells[0].info = dbcSpell.LookupEntry(SHIELD_BLOCK);
		spells[0].targettype = TARGET_SELF; 
		spells[0].instant = true;
		spells[0].perctrigger = 50.0f;
		spells[0].attackstoptimer = 1000;
		
		spells[1].info = dbcSpell.LookupEntry(REVENGE);
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = true;
		spells[1].perctrigger = 50.0f;
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

class DefenseursAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DefenseursAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    DefenseursAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		spells[0].info = dbcSpell.LookupEntry(SHIELD_BLOCK);
		spells[0].targettype = TARGET_SELF; 
		spells[0].instant = true;
		spells[0].perctrigger = 20.0f;
		spells[0].attackstoptimer = 1000;
		
		spells[1].info = dbcSpell.LookupEntry(REVENGE);
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = true;
		spells[1].perctrigger = 50.0f;
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


// Gardes de Froidemine - Début -

#define GARDE_FROIDEMINE 13089 // Garde de Froidemine
#define GARDE_FROIDEMINE_AGU 13534 // Garde de Froidemine Aguerri

#define STIKE 15580 // Frappe
#define BACKHAND 6253 // Revers

class GardeFroidemineAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GardeFroidemineAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    GardeFroidemineAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		spells[0].info = dbcSpell.LookupEntry(STIKE);
		spells[0].targettype = TARGET_ATTACKING; 
		spells[0].instant = true;
		spells[0].perctrigger = 30.0f;
		spells[0].maxdist2cast = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(BACKHAND);
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = true;
		spells[1].perctrigger = 30.0f;
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

// Gardes de Froidemine - Fin -

//Gardes de Gouffrefer -Début -

#define GARDE_GOUFFREFER 13080 // Garde de Gouffrefer
#define GARDE_GOUFFREFER_AGU 13552 // Garde de Gouffrefer Aguerri
#define GARDE_GOUFFREFER_VET 13553 // Garde de Gouffrefer Aguerri

#define CLEAVE 15284 // Enchainement
#define MORTALSTRIKE 16856 // Frappe mortelle

class GardeGouffreferAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GardeGouffreferAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    GardeGouffreferAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		spells[0].info = dbcSpell.LookupEntry(CLEAVE);
		spells[0].targettype = TARGET_ATTACKING; 
		spells[0].instant = true;
		spells[0].perctrigger = 30.0f;
		spells[0].maxdist2cast = 5.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(MORTALSTRIKE);
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = true;
		spells[1].perctrigger = 30.0f;
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

//Gardes de Gouffrefer -Fin-

// Garde de guerre Foudrepique - Début -
#define GARDE_GUERRE_FOUDRE 14284 // Garde de guerre Foudrepique

#define IMPROVEDBLOCKING 3248 // Blocage amélioré
#define CHARGE 22120 // Charge

class GardeGuerreFoudreAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GardeGuerreFoudreAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    GardeGuerreFoudreAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		spells[0].info = dbcSpell.LookupEntry(IMPROVEDBLOCKING);
		spells[0].targettype = TARGET_SELF; 
		spells[0].instant = true;
		spells[0].perctrigger = 100.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CHARGE);
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = true;
		spells[1].perctrigger = 100.0f;
		spells[1].maxdist2cast = 30.0f;
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
// Garde de guerre Foudrepique - Fin -

// Garde de guerre Loup-de-givre - Début -
#define GARDE_GUERRE_LOUP 14284 // Garde de guerre Loup-de-givre

#define CHARGE 22120 // Charge

class GardeGuerreLoupAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GardeGuerreLoupAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    GardeGuerreLoupAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		spells[0].info = dbcSpell.LookupEntry(CHARGE);
		spells[0].targettype = TARGET_ATTACKING; 
		spells[0].instant = true;
		spells[0].perctrigger = 100.0f;
		spells[0].maxdist2cast = 30.0f;
		spells[0].attackstoptimer = 1000;
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
// Garde de guerre Loup-de-givre - Fin -

// Garde Foudrepique - Début -
#define GARDE_FOUDREPIQUE 12127 // Garde Foudrepique

#define STRIKE 11976 // Frappe
#define CHARGE 22120 // Charge

class GardeFoudrepiqueAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GardeFoudrepiqueAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    GardeFoudrepiqueAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		spells[0].info = dbcSpell.LookupEntry(STRIKE);
		spells[0].targettype = TARGET_ATTACKING; 
		spells[0].instant = true;
		spells[0].perctrigger = 30.0f;
		spells[0].maxdist2cast = 5.0f;		
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(CHARGE);
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = true;
		spells[1].perctrigger = 30.0f;
		spells[1].maxdist2cast = 30.0f;
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
// Garde Foudrepique - Fin -

//Gardiens - Début - 

# define GARDIEN_AGUERRI	13328 // Gardien aguerri
# define GARDIEN_LOUP	12053 // Gardien Loup-de-givre
# define GARDIEN_VETERAN	13332 // Gardien Vétéran

#define SHIELDBLOCK 12169 // Maîtrise du blocage
#define REVENGE 19130 // Vengeance

class GardiensAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(GardiensAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    GardiensAI(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		spells[0].info = dbcSpell.LookupEntry(SHIELDBLOCK);
		spells[0].targettype = TARGET_SELF; 
		spells[0].instant = true;
		spells[0].perctrigger = 30.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(REVENGE);
		spells[1].targettype = TARGET_ATTACKING; 
		spells[1].instant = true;
		spells[1].perctrigger = 30.0f;
		spells[1].maxdist2cast = 30.0f;
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

//Gardiens - Fin -

void SetupGardesAlterac(ScriptMgr * mgr)
{
	mgr->register_creature_script(CAPORAL_NOREG, &CaporalNoregAI::Create);
	mgr->register_creature_script(DEFENSEUR_FOUDRE, &DefenseursAI::Create);
	mgr->register_creature_script(DEFENSEUR_VET, &DefenseursAI::Create);
	mgr->register_creature_script(GARDE_FROIDEMINE, &GardeFroidemineAI::Create);
	mgr->register_creature_script(GARDE_FROIDEMINE_AGU, &GardeFroidemineAI::Create);
	mgr->register_creature_script(GARDE_GOUFFREFER, &GardeGouffreferAI::Create);
	mgr->register_creature_script(GARDE_GOUFFREFER_AGU, &GardeGouffreferAI::Create);
	mgr->register_creature_script(GARDE_GOUFFREFER_VET, &GardeGouffreferAI::Create);
	mgr->register_creature_script(GARDE_GUERRE_FOUDRE, &GardeGuerreFoudreAI::Create);
	mgr->register_creature_script(GARDE_GUERRE_LOUP, &GardeGuerreLoupAI::Create);
	mgr->register_creature_script(GARDE_FOUDREPIQUE, &GardeFoudrepiqueAI::Create);	
	mgr->register_creature_script(GARDIEN_AGUERRI, &GardiensAI::Create);
	mgr->register_creature_script(GARDIEN_LOUP, &GardiensAI::Create);
	mgr->register_creature_script(GARDIEN_VETERAN, &GardiensAI::Create);	
}