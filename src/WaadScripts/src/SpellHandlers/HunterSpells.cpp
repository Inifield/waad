/****************************************************************************
 *
 * SpellHandler Plugin
 * Copyright (c) 2007 Team Ascent
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons,
 * 543 Howard Street, 5th Floor, San Francisco, California, 94105, USA.
 *
 * EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW, IN NO EVENT WILL LICENSOR BE LIABLE TO YOU
 * ON ANY LEGAL THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES
 * ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK, EVEN IF LICENSOR HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 */

#include "StdAfx.h"
#include "Setup.h"

bool Refocus(uint32 i, Spell * pSpell)
{
    Player * playerTarget = pSpell->GetPlayerTarget();
    if(playerTarget == 0) return true;

    SpellSet::const_iterator itr = playerTarget->mSpells.begin();
    for(; itr != playerTarget->mSpells.end(); ++itr)
    {
        if((*itr) == __Refocus) continue; // (24531) skip calling spell.. otherwise spammies! :D
            
/*
#define __Aimed_Shot_Rank_1				19434
#define __Aimed_Shot_Rank_2				20900
#define __Aimed_Shot_Rank_3				20901
#define __Aimed_Shot_Rank_4				20902
#define __Aimed_Shot_Rank_5				20903
#define __Aimed_Shot_Rank_6				20904
#define __Aimed_Shot_Rank_6___0			27632
#define __Aimed_Shot_Rank_6___1			44271
#define __Aimed_Shot_Rank_7				27065
#define __Aimed_Shot_Rank_8				49049
#define __Aimed_Shot_Rank_9				49050

#define __Multi_Shot_Rank_1				2643
#define __Multi_Shot_Rank_2				14288
#define __Multi_Shot_Rank_3				14289
#define __Multi_Shot_Rank_4				14290
#define __Multi_Shot_Rank_5				25294
#define __Multi_Shot_Rank_6				27021
#define __Multi_Shot_Rank_7				49047
#define __Multi_Shot_Rank_8				49048

#define __Volley_Rank_1				1510
#define __Volley_Rank_1___0			42243
#define __Volley_Rank_2				14294
#define __Volley_Rank_2___0			42244
#define __Volley_Rank_3				14295
#define __Volley_Rank_3___0			42245
#define __Volley_Rank_4				27022
#define __Volley_Rank_4___0			42234
#define __Volley_Rank_5				58431
#define __Volley_Rank_5___0			58432
#define __Volley_Rank_6				58433
#define __Volley_Rank_6___0			58434
*/
        if((*itr) == __Aimed_Shot_Rank_1 || 
		   (*itr) == __Aimed_Shot_Rank_2 || 
		   (*itr) == __Aimed_Shot_Rank_3 || 
		   (*itr) == __Aimed_Shot_Rank_4 || 
		   (*itr) == __Aimed_Shot_Rank_5 || 
		   (*itr) == __Aimed_Shot_Rank_6 || 
		   (*itr) == __Aimed_Shot_Rank_7 || // 27632 
		   (*itr) == __Aimed_Shot_Rank_8 || 
		   (*itr) == __Aimed_Shot_Rank_9 ||

		   (*itr) == __Multi_Shot  || // 14443
		   //(*itr) == __Multi_Shot___0  || // 18651 
		   //(*itr) == __Multi_Shot___1  || // 20735 
		   //(*itr) == __Multi_Shot___2  || // 21390 
		   (*itr) == __Multi_Shot_Rank_1  || 
		   (*itr) == __Multi_Shot_Rank_2 || 
		   (*itr) == __Multi_Shot_Rank_3 || 
		   (*itr) == __Multi_Shot_Rank_4 || 
		   (*itr) == __Multi_Shot_Rank_5 || 
		   (*itr) == __Multi_Shot_Rank_6 || 
		   (*itr) == __Multi_Shot_Rank_7 || 
		   (*itr) == __Multi_Shot_Rank_8 ||           

		   (*itr) == __Volley_Rank_1 || 
		   (*itr) == __Volley_Rank_2 || 
		   (*itr) == __Volley_Rank_3 || 
		   (*itr) == __Volley_Rank_4 || 
		   (*itr) == __Volley_Rank_5 || 
		   (*itr) == __Volley_Rank_6 ||

		   //(*itr) == 1540 || // N'existe plus
		   (*itr) == __Volley  || // 22908

		   /*
#define __Arcane_Shot_Rank_1				3044
#define __Arcane_Shot_Rank_2				14281
#define __Arcane_Shot_Rank_3				14282
#define __Arcane_Shot_Rank_4				14283
#define __Arcane_Shot_Rank_5				14284
#define __Arcane_Shot_Rank_6				14285
#define __Arcane_Shot_Rank_7				14286
#define __Arcane_Shot_Rank_8				14287
#define __Arcane_Shot_Rank_9				27019
#define __Arcane_Shot_Rank_10				49044
#define __Arcane_Shot_Rank_11				49045
		   */
		   (*itr) == __Arcane_Shot_Rank_1  || 
		   (*itr) == __Arcane_Shot_Rank_2  || 
		   (*itr) == __Arcane_Shot_Rank_3  || 
		   (*itr) == __Arcane_Shot_Rank_4  || 
		   (*itr) == __Arcane_Shot_Rank_5  || 
		   (*itr) == __Arcane_Shot_Rank_6  || 
		   (*itr) == __Arcane_Shot_Rank_7  || 
		   (*itr) == __Arcane_Shot_Rank_8  || 
		   (*itr) == __Arcane_Shot_Rank_9  || 
		   (*itr) == __Arcane_Shot_Rank_10 ||
		   (*itr) == __Arcane_Shot_Rank_11    )
                       playerTarget->ClearCooldownForSpell((*itr));
    }
    return true;
}

bool Readiness(uint32 i, Spell * pSpell)
{
    if(!pSpell->m_caster->IsPlayer()) return true;

    ((Player *)pSpell->m_caster)->ClearCooldownsOnLine(50 , pSpell->m_spellInfo->Id);//Beast Mastery
    ((Player *)pSpell->m_caster)->ClearCooldownsOnLine(163, pSpell->m_spellInfo->Id);//Marksmanship
    ((Player *)pSpell->m_caster)->ClearCooldownsOnLine(51 , pSpell->m_spellInfo->Id);//Survival
    return true;
}

class RevivePet : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(RevivePet);
	RevivePet(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		/*
		#define __Revive_Pet				982
		#define __Revive_Rank_1				50769
		#define __Revive_Rank_2				50768
		#define __Revive_Rank_3				50767
		#define __Revive_Rank_4				50766
		#define __Revive_Rank_5				50765
		#define __Revive_Rank_6				50764
		#define __Revive_Rank_7				50763
		*/
		if (_spell->m_caster->IsPlayer() == NULL || ((Player *)_spell->m_caster)->GetSummon() == NULL)
			return SPELL_FAILED_NO_PET;
			
		if (!((Player *)_spell->m_caster)->GetSummon()->isDead())
			return SPELL_FAILED_TARGET_NOT_DEAD;

		return SPELL_CANCAST_OK;
	}
};

class SteadyShot : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(SteadyShot);
	SteadyShot(Spell* pSpell) : SpellScript(pSpell) {}
	void CalculateEffect(uint32 EffectIndex, Unit* target, int32* value)
	{
		if((EffectIndex == 0) && (_spell->m_caster != NULL) )
		{
			if( _spell->m_caster->IsPlayer() )
			{
				Item *it;
				if(((Player *)_spell->m_caster)->GetItemInterface())
				{
					it = ((Player *)_spell->m_caster)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
					if(it)
					{
						float weapondmg = it->GetProto()->Damage[0].Min + RandomFloat( it->GetProto()->Damage[0].Max );
						*value += float2int32(150 + weapondmg/float(it->GetProto()->Delay/1000.0f)*2.8f);
					}
				}
			}
			if(target && target->IsDazed())
				*value += 175;
			*value += (uint32)(((Player *)_spell->m_caster)->GetRAP()*0.2);
		}
	}
};

class MongooseBite : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(MongooseBite);
	MongooseBite(Spell* pSpell) : SpellScript(pSpell) {}
	void CalculateEffect(uint32 EffectIndex, Unit* target, int32* value)
	{
		if( _spell->m_caster->IsUnit() != NULL )
			*value += ((Unit *)_spell->m_caster)->GetAP()/5;
	}
};

class BestialWrath : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(BestialWrath);
	BestialWrath(Spell* pSpell) : SpellScript(pSpell) {}

	void OnCast()
	{
		/*
		#define __The_Beast_Within				34471
		#define __The_Beast_Within___0			34692
		#define __The_Beast_Within___1			38373
		#define __The_Beast_Within___2			50098
		*/
		//owner casts on pet, so check caster auras
		if (!_spell->m_caster->IsPet())	return; // Randdrick

		if (((Pet *)_spell->m_caster)->HasAura(__The_Beast_Within____0 )) // 34692
		{                                                                 // 34471
			Spell* sp = new Spell(((Pet *)_spell->m_caster), dbcSpell.LookupEntry(__The_Beast_Within), true, NULL);
			SpellCastTargets t(((Pet *)_spell->m_caster));
			if(sp) sp->prepare(&t);
		}
	}
};

void SetupHunterSpells(ScriptMgr * mgr)
{
    mgr->register_dummy_spell(__Refocus   , &Refocus); // 24531
    mgr->register_dummy_spell(__Readiness , &Readiness); // 23989

	mgr->register_spell_script(__Revive_Pet , &RevivePet::Create); // 982

	/*
	#define __Steady_Shot_Rank_1				56641
	#define __Steady_Shot_Rank_2				34120
	#define __Steady_Shot_Rank_3				49051
	#define __Steady_Shot_Rank_4				49052
	*/
	mgr->register_spell_script(__Steady_Shot_Rank_1 , &SteadyShot::Create); // 34128
	mgr->register_spell_script(__Steady_Shot_Rank_2 , &SteadyShot::Create);
	mgr->register_spell_script(__Steady_Shot_Rank_3 , &SteadyShot::Create);
	mgr->register_spell_script(__Steady_Shot_Rank_4 , &SteadyShot::Create);
	/*
	#define __Mongoose_Bite_Rank_1				1495
	#define __Mongoose_Bite_Rank_2				14269
	#define __Mongoose_Bite_Rank_3				14270
	#define __Mongoose_Bite_Rank_4				14271
	#define __Mongoose_Bite_Rank_5				36916
	#define __Mongoose_Bite_Rank_6				53339
	*/
	mgr->register_spell_script(__Mongoose_Bite_Rank_1 , &MongooseBite::Create); // 1495
	mgr->register_spell_script(__Mongoose_Bite_Rank_2 , &MongooseBite::Create); // 14269
	mgr->register_spell_script(__Mongoose_Bite_Rank_3 , &MongooseBite::Create); // 14270
	mgr->register_spell_script(__Mongoose_Bite_Rank_4 , &MongooseBite::Create); // 14271
	mgr->register_spell_script(__Mongoose_Bite_Rank_5 , &MongooseBite::Create); // 36916
	mgr->register_spell_script(__Mongoose_Bite_Rank_6 , &BestialWrath::Create); // 19574
}
