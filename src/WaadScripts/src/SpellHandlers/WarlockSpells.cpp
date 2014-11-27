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

class UnstableAffliction : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(UnstableAffliction);
	UnstableAffliction(Spell* pSpell) : SpellScript(pSpell) {}

	void OnDispel(Aura* pDispelledAura, Spell* pDispellingSpell)
	{
		Unit* caster=pDispelledAura->GetUnitCaster();

		if (!pDispellingSpell->m_caster->IsUnit() || caster == NULL)
			return;

		/*
		#define __Unstable_Affliction				31117
		#define __Unstable_Affliction___0				34438
		#define __Unstable_Affliction___1				34439
		#define __Unstable_Affliction___2				35183
		#define __Unstable_Affliction___3				43523
		*/                                                                         // 31117
		Spell* sp=new Spell(pDispellingSpell->m_caster, dbcSpell.LookupEntry(__Unstable_Affliction ), true, NULL);
		sp->forced_basepoints[0]=pDispelledAura->m_spellProto->EffectBasePoints[0] * 9;
		SpellCastTargets t;
		t.m_target = pDispellingSpell->m_caster;
		sp->prepare(&t);
	}
};

class CurseOfDoom : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(CurseOfDoom);
	CurseOfDoom(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target = _spell->GetUnitTarget();
		if (u_target != NULL && u_target->IsPlayer())
			return SPELL_FAILED_TARGET_IS_PLAYER;

		return SPELL_CANCAST_OK;
	}

	void OnExpire()
	{
		Unit* target=_spell->GetUnitTarget();

		if (target == NULL || !target->isDead())
			return;

		if (RandomUInt(100) < 20)           // 18662
			target->CastSpell(target, __Curse_of_Doom_Effect , true);
	}
};

class LifeTap : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(LifeTap);
	int32 valueforplayer;	

	LifeTap(Spell* pSpell) : SpellScript(pSpell)
	{ 
		count = 0;
		damage = 0;
	}
	void CalculateEffect(uint32 EffectIndex, Unit* target, int32* value)
	{
		if (!_spell->m_caster->IsUnit()) return;

		float amount = *value;

		*value += ((Unit *)_spell->m_caster)->GetSpellDmgBonus(((Unit *)_spell->m_caster), _spell->m_spellInfo, *value, false);

		/*
		#define __Improved_Life_Tap_Rank_1				18182
		#define __Improved_Life_Tap_Rank_2				18183
		*/
		if (((Unit *)_spell->m_caster)->HasAura(__Improved_Life_Tap_Rank_1)) // 18182
			amount *= 1.1f;
		if (((Unit *)_spell->m_caster)->HasAura(__Improved_Life_Tap_Rank_2)) // 18183
			amount *= 1.2f;

		*value=float2int32(amount);

		valueforplayer = *value;

		uint32 mod;	// spirit bonus coefficient multiplied by 2
		if(_spell->GetSpellProto()->Id == 1454) mod = 2;
		else if(_spell->GetSpellProto()->Id == 1455) mod = 3;
		else if(_spell->GetSpellProto()->Id == 1456) mod = 4;
		else if(_spell->GetSpellProto()->Id == 11687) mod = 5;
		else mod = 6;

		damage = _spell->GetSpellProto()->EffectBasePoints[EffectIndex] + 1 + mod * target->GetSpirit() / 2;
		
		if (damage < 69)
			damage = 69;		
	}
	
	//En commentaire car sa gestion n'est pas prise en charge au niveau du Core
	/*SpellCastError CanCast(bool tolerate)
	{
		if (!_spell->m_caster->IsUnit())
			return SPELL_FAILED_BAD_TARGETS;

		if (((Unit *)_spell->m_caster)->GetUInt32Value(UNIT_FIELD_HEALTH) <= (uint32)_spell->CalculateEffect(0, ((Unit *)_spell->m_caster)))
			return SPELL_FAILED_FIZZLE;
		
		if(damage >= ((Unit *)_spell->m_caster)->GetHealth() || ++count >1)
		{
			Log.Warning("LifeTap", "le sort a été lancé %u fois",count);
			return SPELL_FAILED_SUCCESS;
		}

		return SPELL_CANCAST_OK;
	}*/

	void DummyEffect(uint32 EffectIndex)
	{
		//On pose un compteur et on limite la cible au player uniquement car ce spell possède une zone d'effet.
		if(damage >= ((Unit *)_spell->m_caster)->GetHealth() || ++count >1)
			return;

		if (!_spell->m_caster->IsUnit()) return;

		((Unit *)_spell->m_caster)->DealDamage(((Unit *)_spell->m_caster), damage, 0, 0, _spell->m_spellInfo->Id);
		((Unit *)_spell->m_caster)->Energize(((Unit *)_spell->m_caster), _spell->m_spellInfo->Id, valueforplayer, 0);

		//give extra points to player's pet (mana feed)
		if (_spell->m_caster->IsPlayer() && _spell->damage > valueforplayer && ((Player *)_spell->m_caster)->GetSummon() != NULL)
			((Player *)_spell->m_caster)->Energize(((Unit *)_spell->m_caster)->GetSummon(), _spell->m_spellInfo->Id, damage - valueforplayer, 0);

		//On donne les points de mana restant au joueur d'un groupe
		if (_spell->m_caster->IsPlayer() && _spell->damage > valueforplayer && ((Player *)_spell->m_caster)->IsGroupMember(((Player *)_spell->GetPlayerTarget())) != NULL)
			((Player *)_spell->m_caster)->Energize(((Unit *)_spell->GetPlayerTarget()), _spell->m_spellInfo->Id, damage - valueforplayer, 0);
	}
};

class MinorHealthStone : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(MinorHealthStone);

	MinorHealthStone(Spell * pSpell) : SpellScript(pSpell){}

	void ScriptEffect(uint32 EffectIndex)
	{
		if(!((Player *)_spell->m_caster))
			return;

		if(((Player *)_spell->m_caster)->HasSpell(18692))
			_spell->CreateItem(19004);
		else if(((Player *)_spell->m_caster)->HasSpell(18693))
			_spell->CreateItem(19005);
		else
			_spell->CreateItem(5512);
	}
};

class LesserHealthStone : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(LesserHealthStone);

	LesserHealthStone(Spell * pSpell) : SpellScript(pSpell){}

	void ScriptEffect(uint32 EffectIndex)
	{
			if(!((Player *)_spell->m_caster))
				return;

		if(((Player *)_spell->m_caster)->HasSpell(18693))
			_spell->CreateItem(19007);
		else if(((Player *)_spell->m_caster)->HasSpell(18692))
			_spell->CreateItem(19006);
		else
			_spell->CreateItem(5511);
	}
};

class HealthStone : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(MinorHealthStone);

	HealthStone(Spell * pSpell) : SpellScript(pSpell){}

	void ScriptEffect(uint32 EffectIndex)
	{
		if(!((Player *)_spell->m_caster))
			return;

		if(((Player *)_spell->m_caster)->HasSpell(18693))
			_spell->CreateItem(19009);
		else if(((Player *)_spell->m_caster)->HasSpell(18692))
			_spell->CreateItem(19008);
		else
			_spell->CreateItem(5509);
	}
};

class GreaterHealthStone : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(MinorHealthStone);

	GreaterHealthStone(Spell * pSpell) : SpellScript(pSpell){}
	
	void ScriptEffect(uint32 EffectIndex)
	{
		if(!((Player *)_spell->m_caster))
			return;

		if(((Player *)_spell->m_caster)->HasSpell(18693))
			_spell->CreateItem(19011);
		else if(((Player *)_spell->m_caster)->HasSpell(18692))
			_spell->CreateItem(19010);
		else
			_spell->CreateItem(5510);
	}
};

class MajorHealthStone : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(MinorHealthStone);

	MajorHealthStone(Spell * pSpell) : SpellScript(pSpell){}

	void ScriptEffect(uint32 EffectIndex)
	{
		if(!((Player *)_spell->m_caster))
			return;

		if(((Player *)_spell->m_caster)->HasSpell(18693))
			_spell->CreateItem(19013);
		else if(((Player *)_spell->m_caster)->HasSpell(18692))
			_spell->CreateItem(19012);
		else
			_spell->CreateItem(9421);
	}
};

class MasterHealthStone : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(MinorHealthStone);

	MasterHealthStone(Spell * pSpell) : SpellScript(pSpell){}

	void ScriptEffect(uint32 EffectIndex)
	{
		if(!((Player *)_spell->m_caster))
			return;

		if(((Player *)_spell->m_caster)->HasSpell(18693))
			_spell->CreateItem(22105);
		else if(((Player *)_spell->m_caster)->HasSpell(18692))
			_spell->CreateItem(22104);
		else
			_spell->CreateItem(22103);
	}
};

class DemonicHealthStone : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(MinorHealthStone);

	DemonicHealthStone(Spell * pSpell) : SpellScript(pSpell){}

	void ScriptEffect(uint32 EffectIndex)
	{
		if(!((Player *)_spell->m_caster))
			return;

		if(((Player *)_spell->m_caster)->HasSpell(18693))
			_spell->CreateItem(36891);
		else if(((Player *)_spell->m_caster)->HasSpell(18692))
			_spell->CreateItem(36890);
		else
			_spell->CreateItem(36889);
	}
};
class FelHealthStone : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(MinorHealthStone);

	FelHealthStone(Spell * pSpell) : SpellScript(pSpell){}

	void ScriptEffect(uint32 EffectIndex)
	{
		if(!((Player *)_spell->m_caster))
			return;

		if(((Player *)_spell->m_caster)->HasSpell(18693))
			_spell->CreateItem(36894);
		else if(((Player *)_spell->m_caster)->HasSpell(18692))
			_spell->CreateItem(36893);
		else
			_spell->CreateItem(36892);
	}
};

class SoulLink : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(SoulLink);

	SoulLink(Spell* pSpell) : SpellScript(pSpell) {}
	DamageSplitTarget splitter;
	Player* source;

	void DummyEffect(uint32 EffectIndex)
	{
		if (!_spell->m_caster->IsPlayer() || ((Player *)_spell->m_caster)->GetSummon() == NULL)
			return;

		splitter.m_flatDamageSplit = 0;
		splitter.m_spellId = _spell->m_spellInfo->Id;
		splitter.m_pctDamageSplit = 0.2f;
		splitter.damage_type = 64 | 32 | 16 | 8 | 4 | 2 | 1;
		splitter.creator = (void*)this;
		splitter.m_target = ((Player *)_spell->m_caster)->GetSummon()->GetGUID();
		((Player *)_spell->m_caster)->m_damageSplitTargets.push_back(splitter);
		source = ((Player *)_spell->m_caster);
	}

	void OnRemove(Aura* pRemovedAura)
	{
		if (!_spell->m_caster->IsPlayer()) return;

		for (std::list<DamageSplitTarget>::iterator i=source->m_damageSplitTargets.begin(); i!=source->m_damageSplitTargets.end(); ++i)
		{
			if ((*i).creator == this)
			{
				source->m_damageSplitTargets.erase(i);
				break;
			}
		}
	}
};

class DrainLife : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(DrainLife);
	DrainLife(Spell* pSpell) : SpellScript(pSpell) {}

	void CalculateEffect(uint32 EffectIndex, Unit* target, int32* value)
	{
		Unit* u_target = _spell->GetUnitTarget();

		if (!_spell->m_caster->IsUnit() || u_target == NULL) return;

		float mod=1.0f;
		
		Aura* aur = NULL;

		for(uint32 x = MAX_AURAS; x < MAX_PASSIVE_AURAS; ++x)
		{
			if (u_target->m_auras[x] != NULL && u_target->m_auras[x]->m_spellProto->NameHash == SPELL_HASH_SOUL_SIPHON)
				aur = u_target->m_auras[x];
		}

		if (aur == NULL)
			return;

		for(int i = MAX_POSITIVE_AURAS; i <= MAX_AURAS; i++)
			if(u_target->m_auras[i] && objmgr.GetSpellSkill(u_target->m_auras[i]->GetSpellId())->Id == SKILL_AFFLICTION)
					mod += aur->m_spellProto->EffectBasePoints[0] /100;

		if (mod > aur->m_spellProto->EffectBasePoints[1])
			mod = aur->m_spellProto->EffectBasePoints[1];

		*value *= float2int32(mod);
	}
};

class Conflagrate : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Conflagrate);
	Conflagrate(Spell * pSpell) : SpellScript(pSpell) {}
	Unit* u_target;
	SpellCastError CanCast(bool tolerate)
	{
		u_target = _spell->GetUnitTarget();
		if( u_target == NULL || !u_target->HasFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_IMMOLATE))
			return SPELL_FAILED_CASTER_AURASTATE;

		return SPELL_CANCAST_OK;
	}
	void OnEffect(uint32 EffectIndex)
	{
		if( u_target && u_target->HasAurasWithNameHash(SPELL_HASH_IMMOLATE) )
			u_target->RemoveAuraByNameHash(SPELL_HASH_IMMOLATE);
	}
};

class SoulShatter : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(SoulShatter);
	SoulShatter(Spell * pSpell) : SpellScript(pSpell) {}

	void DummyEffect(uint32 EffectIndex)
	{
		if (_spell->GetUnitTarget() == NULL)
			return;

		_spell->GetUnitTarget()->GetAIInterface()->modThreatByPtr(((Unit *)_spell->m_caster), 
			_spell->GetUnitTarget()->GetAIInterface()->getThreatByPtr(((Unit *)_spell->m_caster)) * (_spell->m_spellInfo->EffectBasePoints[EffectIndex] / 100));
	}
};

class DemonicKnowledge : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(DemonicKnowledge);
	int32 amountadded;
	DemonicKnowledge(Spell * pSpell) : SpellScript(pSpell) { amountadded=0; }


	void DummyAura(bool apply, Aura* aura)
	{
		if ( aura->GetTarget()->IsPet() )
		{
			Unit* PetOwner;
			if ( static_cast< Pet* >( aura->GetTarget() )->GetPetOwner() )
			{
				PetOwner = static_cast< Pet* >( aura->GetTarget() )->GetPetOwner();

				if (amountadded == 0)
				{
					uint32 val1 = aura->GetTarget()->GetUInt32Value( UNIT_FIELD_STAT2 ); // stamina
					uint32 val2 = aura->GetTarget()->GetUInt32Value( UNIT_FIELD_STAT3 ); // intelect
					uint32 val0 = val1+val2;
					float amount = val0 * aura->mod->m_amount / 100;
					amountadded=float2int32(amount);
				}

				int32 val;

				if( apply )
					val = amountadded;
				else
				{
					val = -amountadded;
					amountadded = 0.0f;
				}

				for (uint32 x=0;x<7;x++)
					PetOwner->ModUnsigned32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + x, val);
					
				PetOwner->CalcDamage();
			}
		}
	}
};

class WarlockPetScaling01 : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(WarlockPetScaling01);
	WarlockPetScaling01(Spell* pSpell) : SpellScript(pSpell) {}

	void CalculateEffect(uint32 EffectIndex, Unit* target, int32* value)
	{
		if (!_spell->m_caster->IsUnit() || !_spell->m_caster->IsPet() || static_cast<Pet*>(((Unit *)_spell->m_caster))->GetOwner() == NULL)
			return;

		Pet* pet=static_cast<Pet*>(_spell->m_caster);
		uint32 MaxSchoolBonus=0;
		for (int32 i=0; i<7; i++)
			if (pet->GetOwner()->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + i) > MaxSchoolBonus)
				MaxSchoolBonus = pet->GetOwner()->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + i);

		switch (EffectIndex)
		{
		case 0: *value = float2int32(pet->GetOwner()->GetUInt32Value(UNIT_FIELD_STAT2) * 0.3f); break;
		case 1: *value = float2int32(MaxSchoolBonus * 0.57f); break;
		case 2: *value = float2int32(MaxSchoolBonus * 0.15f); break;
		}
	}
};

void SetupWarlockSpells(ScriptMgr * mgr)
{
	/*
	 #define __Unstable_Affliction_Rank_1				30108
     #define __Unstable_Affliction_Rank_2				30404
     #define __Unstable_Affliction_Rank_3				30405
     #define __Unstable_Affliction_Rank_3___0			43522
     #define __Unstable_Affliction_Rank_4				47841
     #define __Unstable_Affliction_Rank_5				47843
	*/
	mgr->register_spell_script(__Unstable_Affliction_Rank_1, &UnstableAffliction::Create);
	mgr->register_spell_script(__Unstable_Affliction_Rank_2, &UnstableAffliction::Create);
	mgr->register_spell_script(__Unstable_Affliction_Rank_3, &UnstableAffliction::Create);
	mgr->register_spell_script(__Unstable_Affliction_Rank_4, &UnstableAffliction::Create);
	mgr->register_spell_script(__Unstable_Affliction_Rank_5, &UnstableAffliction::Create);

	/*
	#define __Curse_of_Doom_Rank_1				603
    #define __Curse_of_Doom_Rank_2				30910
    #define __Curse_of_Doom_Rank_3				47867
	*/
	mgr->register_spell_script(__Curse_of_Doom_Rank_1, &CurseOfDoom::Create);
	mgr->register_spell_script(__Curse_of_Doom_Rank_2, &CurseOfDoom::Create);
	mgr->register_spell_script(__Curse_of_Doom_Rank_3, &CurseOfDoom::Create);

	/*
	#define __Life_Tap_Rank_1				1454
	#define __Life_Tap_Rank_2				1455
	#define __Life_Tap_Rank_3				1456
	#define __Life_Tap_Rank_4				11687
	#define __Life_Tap_Rank_5				11688
	#define __Life_Tap_Rank_6				11689
	#define __Life_Tap_Rank_7				27222
	#define __Life_Tap_Rank_8				57946
	*/
	mgr->register_spell_script(__Life_Tap_Rank_1, &LifeTap::Create);
	mgr->register_spell_script(__Life_Tap_Rank_2, &LifeTap::Create);
	mgr->register_spell_script(__Life_Tap_Rank_3, &LifeTap::Create);
	mgr->register_spell_script(__Life_Tap_Rank_4, &LifeTap::Create);
	mgr->register_spell_script(__Life_Tap_Rank_5, &LifeTap::Create);
	mgr->register_spell_script(__Life_Tap_Rank_6, &LifeTap::Create);
	mgr->register_spell_script(__Life_Tap_Rank_7, &LifeTap::Create);
	mgr->register_spell_script(__Life_Tap_Rank_8, &LifeTap::Create);

	/*
	#define __Soul_Link				19028
    #define __Soul_Link___0			25228
	*/
	mgr->register_spell_script(__Soul_Link, &SoulLink::Create);

	/////////////////////Scripted Effect //////////////////////////
	mgr->register_spell_script(6201, &MinorHealthStone::Create);
	mgr->register_spell_script(6202, &LesserHealthStone::Create);
	mgr->register_spell_script(5699, &HealthStone::Create);
	mgr->register_spell_script(11729, &GreaterHealthStone::Create);
	mgr->register_spell_script(11730, &MajorHealthStone::Create);
	mgr->register_spell_script(27230, &MasterHealthStone::Create);
	mgr->register_spell_script(47871, &DemonicHealthStone::Create);
	mgr->register_spell_script(47878, &FelHealthStone::Create);

	/*
	#define __Drain_Life_Rank_1				689
	#define __Drain_Life_Rank_2				699
	#define __Drain_Life_Rank_3				709
	#define __Drain_Life_Rank_4				7651
	#define __Drain_Life_Rank_5				11699
	#define __Drain_Life_Rank_6				11700
	#define __Drain_Life_Rank_7				27219
	#define __Drain_Life_Rank_8				27220
	#define __Drain_Life_Rank_9				47857
	*/
	mgr->register_spell_script(__Drain_Life_Rank_1, &DrainLife::Create);
	mgr->register_spell_script(__Drain_Life_Rank_2, &DrainLife::Create);
	mgr->register_spell_script(__Drain_Life_Rank_3, &DrainLife::Create);
	mgr->register_spell_script(__Drain_Life_Rank_4, &DrainLife::Create);
	mgr->register_spell_script(__Drain_Life_Rank_5, &DrainLife::Create);
	mgr->register_spell_script(__Drain_Life_Rank_6, &DrainLife::Create);
	mgr->register_spell_script(__Drain_Life_Rank_7, &DrainLife::Create);
	mgr->register_spell_script(__Drain_Life_Rank_8, &DrainLife::Create);
	mgr->register_spell_script(__Drain_Life_Rank_9, &DrainLife::Create);

	/*
	#define __Conflagrate_Rank_1				17962
	*/
	mgr->register_spell_script(__Conflagrate, &Conflagrate::Create);

	/*
	#define __Soulshatter				29858
	#define __Soulshatter___0			32835
	*/
	mgr->register_spell_script(__Soulshatter, &SoulShatter::Create);

	/*
	#define __Demonic_Knowledge				35696
	#define __Demonic_Knowledge___0			39576
	#define __Demonic_Knowledge_Rank_1		35691
	#define __Demonic_Knowledge_Rank_2		35692
    #define __Demonic_Knowledge_Rank_3		35693
	*/
	//mgr->register_spell_script(35696, &DemonicKnowledge::Create);
	mgr->register_spell_script(__Demonic_Knowledge_Rank_1, &DemonicKnowledge::Create);
	mgr->register_spell_script(__Demonic_Knowledge_Rank_2, &DemonicKnowledge::Create);
	mgr->register_spell_script(__Demonic_Knowledge_Rank_3, &DemonicKnowledge::Create);

	/*
	#define __Warlock_Pet_Scaling_01				34947
	#define __Warlock_Pet_Scaling_02				34956
	#define __Warlock_Pet_Scaling_03				34957
	#define __Warlock_Pet_Scaling_04				34958
	#define __Warlock_Pet_Scaling_05				61013
	*/
	mgr->register_spell_script(__Warlock_Pet_Scaling_01, &WarlockPetScaling01::Create);
}
