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

class Prowl : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Prowl);
	Prowl(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		if (_spell->m_caster->IsUnit() == NULL || ((Unit *)_spell->m_caster)->CombatStatus.IsInCombat())
			return SPELL_FAILED_TARGET_IN_COMBAT;

		if (_spell->m_caster->IsPlayer() != NULL && ((Player *)_spell->m_caster)->m_bgHasFlag)
			return SPELL_FAILED_SPELL_UNAVAILABLE;

		return SPELL_CANCAST_OK;
	}
};

class LifeBloom : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(LifeBloom);
	LifeBloom(Spell* pSpell) : SpellScript(pSpell) {}

	void OnDispel(Aura* pDispelledAura, Spell* pDispellingSpell)
	{
		OnExpire(pDispelledAura); //does same thing
	}

	void OnExpire(Aura* pExpiredAura)
	{
		Unit* u_caster = pExpiredAura->GetUnitCaster();
		Unit* u_target = pExpiredAura->GetTarget();

		if (u_caster == NULL || u_target == NULL)
			return;

		Spell s(u_caster, pExpiredAura->m_spellProto, true, NULL);
		s.SetUnitTarget(pExpiredAura->GetTarget());
		//get bonus :)
		uint32 healamount = pExpiredAura->m_spellProto->EffectBasePoints[1] + 1 + u_caster->GetSpellDmgBonus(u_target, pExpiredAura->m_spellProto, pExpiredAura->m_spellProto->EffectBasePoints[1] + 1, false);
		s.Heal(healamount);
	}
};

class Rake : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Rake);
	Rake(Spell* pSpell) : SpellScript(pSpell) {}
	void CalculateEffect(uint32 EffectIndex, Unit* target, int32* value)
	{
		//Rake the target for ${$AP/100+$m1} bleed damage and an additional ${$m2*3+$AP*0.06} damage over $d.
		if( _spell->m_caster->IsUnit() != NULL ) 
		{
			float ap = (float)((Unit *)_spell->m_caster)->GetAP();
			if(EffectIndex == 0)
				*value+=(uint32)ceilf((ap*0.01f));	// / 100
			else if(EffectIndex == 1)
				*value=(int32)ceilf((float(*value * 3) + ceilf((ap*0.06f))) / 3.0f);
		}
	}
};

class Rip : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Rip);
	Rip(Spell* pSpell) : SpellScript(pSpell) {}
	void CalculateEffect(uint32 EffectIndex, Unit* target, int32* value)
	{
		if( _spell->m_caster->IsUnit() != NULL ) 
			*value += (uint32)(((Unit *)_spell->m_caster)->GetAP()*0.04f);
	}
};

class Swipe : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Swipe);
	Swipe(Spell* pSpell) : SpellScript(pSpell) {}
	void CalculateEffect(uint32 EffectIndex, Unit* target, int32* value)
	{
		if( _spell->m_caster->IsUnit() != NULL )
			*value += float2int32(((Unit *)_spell->m_caster)->GetAP()*0.06f);
	}
};

class TreeOfLife : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(TreeOfLife);
	TreeOfLife(Spell* pSpell) : SpellScript(pSpell) {}
	void OnCast()
	{
		/*
		#define __Tree_of_Life_Passive				5420
		#define __Tree_of_Life_Passive___0			34123
		*/
		Spell* s=new Spell(_spell->m_caster, dbcSpell.LookupEntry(__Tree_of_Life_Passive ), true, NULL); // 5420
		SpellCastTargets t(_spell->m_caster);
		if(s) s->prepare(&t);
		Spell* s2=new Spell(_spell->m_caster, dbcSpell.LookupEntry(__Tree_of_Life_Passive____0), true, NULL); // 34123
		if(s2) s2->prepare(&t);
	}

	void OnRemove(Aura* pRemovedAura)
	{
		Unit* caster=pRemovedAura->GetUnitCaster();

		if (caster == NULL)
			return;

		//remove 5420 and 34123 where caster guids match :)
		for(uint32 x=0; x<MAX_AURAS+MAX_PASSIVE_AURAS; ++x)
		{
			if (caster->m_auras[x] == NULL || caster->m_auras[x]->m_casterGuid != caster->GetGUID())
				continue;

			if (caster->m_auras[x]->m_spellProto->Id != __Tree_of_Life_Passive && // 5420 
				caster->m_auras[x]->m_spellProto->Id != __Tree_of_Life_Passive____0 ) // 24123 ?? erreur __Primal_Batskin_Bracers
				continue;

			caster->m_auras[x]->m_wasremoved = true;
			caster->m_auras[x]->Remove();
		}
	}
};

class TreeOfLifePassive : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(TreeOfLifePassive);
	TreeOfLifePassive(Spell* pSpell) : SpellScript(pSpell) { }
	void CalculateEffect(uint32 EffectIndex, Unit* target, int32* value)
	{
		//effect value is spirit * 0.25f, SAVE VALUE, spirit might change and it might remove a different value
		if (EffectIndex != 0 || _spell->m_caster->IsUnit() == NULL)
			return;

		*value = float2int32(((Unit *)_spell->m_caster)->GetUInt32Value(UNIT_FIELD_STAT4) * 0.25f);
	}
};

void SetupDruidSpells(ScriptMgr * mgr)
{
	/*
	#define __Prowl_Rank_1				5215
	#define __Prowl_Rank_1___0			8152
	#define __Prowl_Rank_1___1			24450
	#define __Prowl_Rank_1___2			24451
	#define __Prowl_Rank_2				6783
	#define __Prowl_Rank_2___0			24452
	#define __Prowl_Rank_2___1			24454
	#define __Prowl_Rank_3				9913
	#define __Prowl_Rank_3___0			24453
	#define __Prowl_Rank_3___1			24455
	*/
	mgr->register_spell_script(__Prowl_Rank_1, &Prowl::Create); // 5215
	mgr->register_spell_script(__Prowl_Rank_2, &Prowl::Create); // 6783
	mgr->register_spell_script(__Prowl_Rank_3, &Prowl::Create); // 9913

	/*
	#define __Lifebloom_Rank_1				33763
	#define __Lifebloom_Rank_1___0			43422
	#define __Lifebloom_Rank_1___1			52552
	#define __Lifebloom_Rank_1___2			53692
	#define __Lifebloom_Rank_1___3			57763
	#define __Lifebloom_Rank_2				48450
	#define __Lifebloom_Rank_3				48451
	*/
	mgr->register_spell_script(__Lifebloom_Rank_1 , &LifeBloom::Create); // 33763
	mgr->register_spell_script(__Lifebloom_Rank_2 , &LifeBloom::Create); // 48450
	mgr->register_spell_script(__Lifebloom_Rank_3 , &LifeBloom::Create); // 48451

	/*
	#define __Rake_Rank_1				1822
	#define __Rake_Rank_1___0			59881
	#define __Rake_Rank_2				1823
	#define __Rake_Rank_2___0			59882
	#define __Rake_Rank_3				1824
	#define __Rake_Rank_3___0			59883
	#define __Rake_Rank_4				9904
	#define __Rake_Rank_4___0			59884
	#define __Rake_Rank_5				27003
	#define __Rake_Rank_5___0			59885
	#define __Rake_Rank_6				48573
	#define __Rake_Rank_6___0			59886
	#define __Rake_Rank_7				48574
	*/
	mgr->register_spell_script(__Rake_Rank_1 , &Rake::Create); // 1822
	mgr->register_spell_script(__Rake_Rank_2 , &Rake::Create); // 1823
	mgr->register_spell_script(__Rake_Rank_3 , &Rake::Create); // 1824
	mgr->register_spell_script(__Rake_Rank_4 , &Rake::Create); // 9904
	mgr->register_spell_script(__Rake_Rank_5 , &Rake::Create); // 27003
	mgr->register_spell_script(__Rake_Rank_6 , &Rake::Create);
	mgr->register_spell_script(__Rake_Rank_7 , &Rake::Create);

	/*
	#define __Rip_Rank_1				1079
	#define __Rip_Rank_2				9492
	#define __Rip_Rank_3				9493
	#define __Rip_Rank_4				9752
	#define __Rip_Rank_5				9894
	#define __Rip_Rank_6				9896
	#define __Rip_Rank_7				27008
	#define __Rip_Rank_8				49799
	#define __Rip_Rank_9				49800
	*/
	mgr->register_spell_script(__Rip_Rank_1, &Rip::Create);
	mgr->register_spell_script(__Rip_Rank_2, &Rip::Create);
	mgr->register_spell_script(__Rip_Rank_3, &Rip::Create);
	mgr->register_spell_script(__Rip_Rank_4, &Rip::Create);
	mgr->register_spell_script(__Rip_Rank_5, &Rip::Create);
	mgr->register_spell_script(__Rip_Rank_6, &Rip::Create);
	mgr->register_spell_script(__Rip_Rank_7, &Rip::Create);
	mgr->register_spell_script(__Rip_Rank_8, &Rip::Create);
	mgr->register_spell_script(__Rip_Rank_9, &Rip::Create);


	/*
	#define __Tree_of_Life_Shapeshift				33891
	#define __Tree_of_Life_Shapeshift___0			48371
	#define __Tree_of_Life_Shapeshift___1			53691
	*/
	mgr->register_spell_script(__Tree_of_Life_Shapeshift , &TreeOfLife::Create); // 33891

	/*
	#define __Tree_of_Life_Passive				5420
	#define __Tree_of_Life_Passive___0			34123
	*/
	mgr->register_spell_script(__Tree_of_Life_Passive , &TreeOfLifePassive::Create); // 34123
}