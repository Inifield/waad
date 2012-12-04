/* Scripts du champ de bataille de la vallé d'Alterac par Randdrick pour la communauté Waad
Basé sur le travail de la team Moon++  - Projet Moon -
Soumis à la licence Creative Common - Paternité - pas d'utilisation commerciale -
Pour en savoir plus : http://creativecommons.org/licenses/by-nc/2.0/fr/
*/
#include "StdAfx.h"
#include "Setup.h"

// Aggi Grondécrase

#define AGGI_GRONDECRASE 13086 //Aggi Grondécrase
#define SHIELD_SLAM 15655 // Heurt de bouclier
#define SHIELD_BLOCK 12169 // Maîtrise du blocage

class AGGIGRONDECRASE : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AGGIGRONDECRASE);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    AGGIGRONDECRASE(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		     spells[0].info = dbcSpell.LookupEntry(SHIELD_SLAM);
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = true;
		     spells[0].perctrigger = 20.0f;
			 spells[0].maxdist2cast = 5.0f;
		     spells[0].attackstoptimer = 1000;

		     spells[1].info = dbcSpell.LookupEntry(SHIELD_BLOCK);
		     spells[1].targettype = TARGET_SELF;
		     spells[1].instant = true;
		     spells[1].perctrigger = 50.0f;
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


// Alpha Follepatte

#define ALPHA_FOLLEPATTE 11840 //Alpha Follepatte
#define ENRAGE 8599 // Enrager
#define UPPERCUT 18072 // Uppercut

class ALPHAFOLLEPATTE : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ALPHAFOLLEPATTE);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    ALPHAFOLLEPATTE(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		     spells[0].info = dbcSpell.LookupEntry(ENRAGE);
		     spells[0].targettype = TARGET_SELF;
		     spells[0].instant = true;
		     spells[0].perctrigger = 20.0f;
		     spells[0].attackstoptimer = 1000;

		     spells[1].info = dbcSpell.LookupEntry(SHIELD_BLOCK);
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = true;
		     spells[1].perctrigger = 20.0f;
			 spells[1].maxdist2cast = 10.0f;			 
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

// Archidruide Ranfarouche

#define ARCHIDRUIDE_RANFAROUCHE 13442 //Archidruide Ranfarouche
#define MOONFIRE 22206 // Eclat lunaire
#define REJUVENATION 15981 // Récupération
#define ENTANGLING_ROOTS 22127 // Sarments

class ARCHIDRUIDERANFAROUCHE : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ARCHIDRUIDERANFAROUCHE);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    ARCHIDRUIDERANFAROUCHE(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		     spells[0].info = dbcSpell.LookupEntry(MOONFIRE);
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = true;
		     spells[0].perctrigger = 20.0f;
			 spells[0].maxdist2cast = 30.0f;
		     spells[0].attackstoptimer = 1000;

		     spells[1].info = dbcSpell.LookupEntry(REJUVENATION);
		     spells[1].targettype = TARGET_DESTINATION;
		     spells[1].instant = true;
		     spells[1].perctrigger = 20.0f; 
		     spells[1].attackstoptimer = 1000;
			 
			 spells[2].info = dbcSpell.LookupEntry(ENTANGLING_ROOTS);
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = false;
			 spells[2].casttime = 1.5f;
		     spells[2].perctrigger = 20.0f;
		     spells[2].attackstoptimer = 1000;
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

// Chaman de Gouffrefer

#define CHAMAN_GOUFFREFER 11600 //Chaman de Gouffrefer
#define LIGHTNING_BOLT 15801 // Eclair
#define EARTHBIND_TOTEM 15786 // Totem de lien terrestre
#define HEALING_WAVE 12492 // Vague de soins

class CHAMANGOUFFREFER : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CHAMANGOUFFREFER);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    CHAMANGOUFFREFER(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		     spells[0].info = dbcSpell.LookupEntry(LIGHTNING_BOLT);
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = false;
			 spells[0].casttime = 3.0f;
		     spells[0].perctrigger = 20.0f;
			 spells[0].maxdist2cast = 40.0f;
		     spells[0].attackstoptimer = 1000;

		     spells[1].info = dbcSpell.LookupEntry(EARTHBIND_TOTEM);
		     spells[1].targettype = TARGET_SELF;
		     spells[1].instant = true;
		     spells[1].perctrigger = 20.0f;	 
		     spells[1].attackstoptimer = 1000;
			 
			 spells[2].info = dbcSpell.LookupEntry(HEALING_WAVE);
		     spells[2].targettype = TARGET_RANDOM_FRIEND;
		     spells[2].instant = false;
			 spells[2].casttime = 3.0f;
		     spells[2].perctrigger = 20.0f;
			 spells[2].mindist2cast = 0.0f;
			 spells[2].maxdist2cast = 40.0f;
			 spells[2].minhp2cast = 0;
			 spells[2].maxhp2cast = 100;
		     spells[2].attackstoptimer = 1000;
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
protected:

	int nrspells;
};

// Chaman Follepatte

#define CHAMAN_FOLLEPATTE 11837 //Chaman Follepatte
#define CONE_COLD 12557 // Cône de froid
#define SUMMON_ICE_TOTEM 18975 // Totem de lien terrestre


class CHAMANFOLLEPATTE : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(CHAMANFOLLEPATTE);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    CHAMANFOLLEPATTE(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		     spells[0].info = dbcSpell.LookupEntry(LIGHTNING_BOLT);
		     spells[0].targettype = TARGET_SELF;
		     spells[0].instant = true;
		     spells[0].perctrigger = 20.0f;
		     spells[0].attackstoptimer = 1000;

		     spells[1].info = dbcSpell.LookupEntry(EARTHBIND_TOTEM);
		     spells[1].targettype = TARGET_DESTINATION;
		     spells[1].instant = true;
		     spells[1].perctrigger = 20.0f;	 
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

// Druide du Bosquet

#define DRUIDE_BOSQUET 13443 //Druide du Bosquet
#define ENTANGLING_ROOTS 22127 // Sarments
#define STARFIRE 21668 // Feu stellaire
#define THORMS 22128 // Epines

class DRUIDEBOSQUET : public CreatureAIScript 
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(DRUIDEBOSQUET);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    DRUIDEBOSQUET(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		     spells[0].info = dbcSpell.LookupEntry(ENTANGLING_ROOTS);
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = true;
		     spells[0].perctrigger = 20.0f;
			 spells[0].maxdist2cast = 30.0f;			 
		     spells[0].attackstoptimer = 1000;

		     spells[1].info = dbcSpell.LookupEntry(STARFIRE);
		     spells[1].targettype = TARGET_RANDOM_DESTINATION;
		     spells[1].instant = false;
			 spells[1].casttime = 2.0f;
		     spells[1].perctrigger = 20.0f;	
			 spells[1].mindist2cast = 0.0f;
			 spells[1].maxdist2cast = 30.0f;		 
		     spells[1].attackstoptimer = 1000;

		     spells[2].info = dbcSpell.LookupEntry(THORMS);
		     spells[2].targettype = TARGET_RANDOM_DESTINATION;
		     spells[2].instant = false;
			 spells[2].casttime = 1.5f;
		     spells[2].perctrigger = 20.0f;	
			 spells[2].mindist2cast = 0.0f;
			 spells[2].maxdist2cast = 30.0f;			 
		     spells[2].attackstoptimer = 1000;				 
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
protected:

	int nrspells;
};

// Ecumeurs et envahisseurs
#define ECUMEUR_GOUFFREFER 13081 //Ecumeur de Gouffrefer
#define ECUMEUR_GOUFFREFER_AGUERRI 13543 //Ecumeur de Gouffrefer aguerri
#define ENVAHISSEUR_FROIDEMINE 11678 // Envahisseur de Froidemine
#define ENVAHISSEUR_FROIDEMINE_AGUERRI 13549 // Envahisseur de Froidemine aguerri
#define ENVAHISSEUR_FROIDEMINE_CHAMPION 13551 // Envahisseur de Froidemine champion
#define ENVAHISSEUR_FROIDEMINE_VETERAN 13550 // Envahisseur de Froidemine veteran

#define HAMSTRING 9080 // Brise-genou
#define STRIKE 12057 // Frappe
#define REND 16509 // Pourfendre

class ECUMEURSENVAHISSEURS : public CreatureAIScript 
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ECUMEURSENVAHISSEURS);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    ECUMEURSENVAHISSEURS(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		     spells[0].info = dbcSpell.LookupEntry(HAMSTRING);
		     spells[0].targettype = TARGET_ATTACKING;
		     spells[0].instant = true;
		     spells[0].perctrigger = 20.0f;
			 spells[0].maxdist2cast = 5.0f;			 
		     spells[0].attackstoptimer = 1000;

		     spells[1].info = dbcSpell.LookupEntry(STRIKE);
		     spells[1].targettype = TARGET_ATTACKING;
		     spells[1].instant = true;
		     spells[1].perctrigger = 20.0f;	
			 spells[1].maxdist2cast = 5.0f;		 
		     spells[1].attackstoptimer = 1000;

		     spells[2].info = dbcSpell.LookupEntry(REND);
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = true;
		     spells[2].perctrigger = 20.0f;	
			 spells[2].maxdist2cast = 5.0f;
		     spells[2].attackstoptimer = 1000;				 
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

// Explorateurs
#define EXPLORATEUR_GOUFFREFER 13099 //Explorateur de Gouffrefer
#define EXPLORATEUR_GOUFFREFER_AGUERRI 13540 //Explorateur  de Gouffrefer aguerri
#define EXPLORATEUR_FROIDEMINE 13096 // Explorateur  de Froidemine
#define EXPLORATEUR_FROIDEMINE_AGUERRI 13546 // Explorateur  de Froidemine aguerri
#define EXPLORATEUR_FROIDEMINE_CHAMPION 13548 // Explorateur  de Froidemine champion
#define EXPLORATEUR_FROIDEMINE_VETERAN 13547 // Explorateur  de Froidemine veteran

#define FROST_ARMOR 12544 // Armure de givre
#define FIREBALL 15242 // Boule de feu
#define CONE_OF_COLD 15244 // Cône de froid
#define FIRE_BLAST 14145 // Trait de feu

class EXPLORATEURS : public CreatureAIScript 
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(EXPLORATEURS);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    EXPLORATEURS(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}		
		     spells[0].info = dbcSpell.LookupEntry(FROST_ARMOR);
		     spells[0].targettype = TARGET_SELF;
		     spells[0].instant = true;
		     spells[0].perctrigger = 20.0f;		 
		     spells[0].attackstoptimer = 1000;

		     spells[1].info = dbcSpell.LookupEntry(FIREBALL);
		     spells[1].targettype = TARGET_RANDOM_DESTINATION;
		     spells[1].instant = false;
			 spells[1].casttime = 3.0f;
		     spells[1].perctrigger = 20.0f;
			 spells[1].mindist2cast = 0.0f;
			 spells[1].maxdist2cast = 40.0f;
		     spells[1].attackstoptimer = 1000;

		     spells[2].info = dbcSpell.LookupEntry(CONE_OF_COLD);
		     spells[2].targettype = TARGET_SELF;
		     spells[2].instant = true;
		     spells[2].perctrigger = 20.0f;	
		     spells[2].attackstoptimer = 1000;

		     spells[3].info = dbcSpell.LookupEntry(FIRE_BLAST);
		     spells[3].targettype = TARGET_ATTACKING;
		     spells[3].instant = true;
		     spells[3].perctrigger = 20.0f;	
			 spells[3].maxdist2cast = 20.0f;
		     spells[3].attackstoptimer = 1000;				 
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
					if (!spells[i].instant)
						_unit->GetAIInterface()->StopMovement(1);

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
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<Unit*> TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(set<Object*>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					Unit* RandomTarget = NULL;
					RandomTarget = (Unit*)(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			Unit * RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
protected:

	int nrspells;
};

void SetupNpc(ScriptMgr * mgr)
{
	mgr->register_creature_script(AGGI_GRONDECRASE, &AGGIGRONDECRASE::Create);
	mgr->register_creature_script(ALPHA_FOLLEPATTE, &ALPHAFOLLEPATTE::Create);
	mgr->register_creature_script(ARCHIDRUIDE_RANFAROUCHE, &ARCHIDRUIDERANFAROUCHE::Create);
	mgr->register_creature_script(CHAMAN_GOUFFREFER, &CHAMANGOUFFREFER::Create);
	mgr->register_creature_script(CHAMAN_FOLLEPATTE, &CHAMANFOLLEPATTE::Create);
	mgr->register_creature_script(DRUIDE_BOSQUET, &DRUIDEBOSQUET::Create);
	mgr->register_creature_script(ECUMEUR_GOUFFREFER, &ECUMEURSENVAHISSEURS::Create);
	mgr->register_creature_script(ECUMEUR_GOUFFREFER_AGUERRI, &ECUMEURSENVAHISSEURS::Create);
	mgr->register_creature_script(ENVAHISSEUR_FROIDEMINE, &ECUMEURSENVAHISSEURS::Create);
	mgr->register_creature_script(ENVAHISSEUR_FROIDEMINE_AGUERRI, &ECUMEURSENVAHISSEURS::Create);
	mgr->register_creature_script(ENVAHISSEUR_FROIDEMINE_CHAMPION, &ECUMEURSENVAHISSEURS::Create);
	mgr->register_creature_script(ENVAHISSEUR_FROIDEMINE_VETERAN, &ECUMEURSENVAHISSEURS::Create);
	mgr->register_creature_script(EXPLORATEUR_GOUFFREFER, &EXPLORATEURS::Create);
	mgr->register_creature_script(EXPLORATEUR_GOUFFREFER_AGUERRI, &EXPLORATEURS::Create);
	mgr->register_creature_script(EXPLORATEUR_FROIDEMINE, &EXPLORATEURS::Create);
	mgr->register_creature_script(EXPLORATEUR_FROIDEMINE_AGUERRI, &EXPLORATEURS::Create);
	mgr->register_creature_script(EXPLORATEUR_FROIDEMINE_CHAMPION, &EXPLORATEURS::Create);
	mgr->register_creature_script(EXPLORATEUR_FROIDEMINE_VETERAN, &EXPLORATEURS::Create);

}	