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

//Summon Screecher Spirit - 12699
class SummonScreecherSpirit : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(SummonScreecherSpirit);
	SummonScreecherSpirit(Spell* pSpell) : SpellScript(pSpell) {}

	SpellCastError CanCast(bool tolerate)
	{
		Unit* target=_spell->GetUnitTarget();

		if(target == NULL || target->GetEntry() != 5307 || target->isAlive())
			return SPELL_FAILED_BAD_TARGETS;

		return SPELL_CANCAST_OK;
	}
};

class TagMurloc : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(TagMurloc);
	TagMurloc(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* target=_spell->GetUnitTarget();

		if(target == NULL || target->GetEntry() != 17326)
			return SPELL_FAILED_BAD_TARGETS;

		return SPELL_CANCAST_OK;
	}
};

class AdministerAntidote : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(AdministerAntidote);
	AdministerAntidote(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* target=_spell->GetUnitTarget();

		if(target == NULL || target->GetEntry() != 16880)
			return SPELL_FAILED_BAD_TARGETS;

		return SPELL_CANCAST_OK;
	}
};

class YennikusRelease : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(YennikusRelease);
	YennikusRelease(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* target=_spell->GetUnitTarget();

		if(target == NULL || target->GetEntry() != 2530)
			return SPELL_FAILED_BAD_TARGETS;

		return SPELL_CANCAST_OK;
	}
};

class RinasDiminutionPowder : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(RinasDiminutionPowder);
	RinasDiminutionPowder(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* target=_spell->GetUnitTarget();

		if(target == NULL || target->GetEntry() != 20058)
			return SPELL_FAILED_BAD_TARGETS;

		return SPELL_CANCAST_OK;
	}
};

class LiquidFire : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(LiquidFire);
	LiquidFire(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Creature *corpse = _spell->m_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_spell->m_caster->GetPositionX(), _spell->m_caster->GetPositionY(), _spell->m_caster->GetPositionZ(), 18240);
		if(corpse != NULL)
			if (_spell->m_caster->CalcDistance(corpse) > 5)
				return SPELL_FAILED_NOT_HERE;

		return SPELL_CANCAST_OK;
	}
};

class Fumping : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Fumping);
	Fumping(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Creature *cleft = _spell->m_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_spell->m_caster->GetPositionX(), _spell->m_caster->GetPositionY(), _spell->m_caster->GetPositionZ(), 22105);
		if(cleft == NULL || cleft->isAlive())
			return SPELL_FAILED_NOT_HERE;

		return SPELL_CANCAST_OK;
	}
};

class PlaceDraeneiBanner : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(PlaceDraeneiBanner);
	PlaceDraeneiBanner(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Creature *corpse = _spell->m_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_spell->m_caster->GetPositionX(), _spell->m_caster->GetPositionY(), _spell->m_caster->GetPositionZ(), 17701);
		if(corpse != NULL)
			if (_spell->m_caster->CalcDistance(corpse) > 5  || corpse->isAlive())
				return SPELL_FAILED_NOT_HERE;

		return SPELL_CANCAST_OK;
	}
};

class DemonBroiledSurprise : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(DemonBroiledSurprise);
	DemonBroiledSurprise(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Creature *abysal = _spell->m_caster->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_spell->m_caster->GetPositionX(), _spell->m_caster->GetPositionY(), _spell->m_caster->GetPositionZ(), 19973);
		if(!_spell->m_caster->IsPlayer() || abysal == NULL || abysal->isAlive())
			return SPELL_FAILED_NOT_HERE;

		if(!(((Player *)_spell->m_caster)->GetItemInterface()->GetItemCount(31672, 0) > 1 && 
			 ((Player *)_spell->m_caster)->GetItemInterface()->GetItemCount(31673, 0) > 0 && 
			 ((Player *)_spell->m_caster)->CalcDistance(abysal) < 10))
			return SPELL_FAILED_NOT_HERE;

		return SPELL_CANCAST_OK;
	}
};

class ReindeerTransformation : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(ReindeerTransformation);
	ReindeerTransformation(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		if (!_spell->m_caster->IsPlayer() || !((Player *)_spell->m_caster)->IsMounted())
			return SPELL_FAILED_ONLY_MOUNTED;

		return SPELL_CANCAST_OK;
	}
};

class SmiteDemon : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(SmiteDemon);
	SmiteDemon(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target=_spell->GetUnitTarget();

		if (u_target == NULL || u_target->IsPlayer() || u_target->getClass() != TARGET_TYPE_DEMON)
			return SPELL_FAILED_SPELL_UNAVAILABLE;

		return SPELL_CANCAST_OK;
	}
};

class AbsorbEyeofGrillok : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(AbsorbEyeofGrillok);
	AbsorbEyeofGrillok(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		//this is currently disabled
		return SPELL_FAILED_SPELL_UNAVAILABLE;
	}
};

class TheSeersPresence : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(TheSeersPresence);
	TheSeersPresence(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target=_spell->GetUnitTarget();

		if (u_target == NULL || !u_target->IsCreature() || u_target->GetEntry() != 20132)
			return SPELL_FAILED_BAD_TARGETS;

		return SPELL_CANCAST_OK;
	}
};

class BlackwhelpNet : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(BlackwhelpNet);
	BlackwhelpNet(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target=_spell->GetUnitTarget();

		if (u_target == NULL || !u_target->IsCreature() || u_target->GetEntry() != 21387)
			return SPELL_FAILED_BAD_TARGETS;

		return SPELL_CANCAST_OK;
	}
};

class ConvertingSentry : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(ConvertingSentry);
	ConvertingSentry(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target=_spell->GetUnitTarget();

		if (u_target == NULL || !u_target->IsCreature() || u_target->GetEntry() != 24972)
			return SPELL_FAILED_BAD_TARGETS;

		if (!u_target->isDead())
			return SPELL_FAILED_TARGET_NOT_DEAD;

		return SPELL_CANCAST_OK;
	}
};

class CarindasRetribution : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(CarindasRetribution);
	CarindasRetribution(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target=_spell->GetUnitTarget();

		if (u_target == NULL || !u_target->IsCreature() || u_target->GetEntry() != 24972)
			return SPELL_FAILED_BAD_TARGETS;

		if (!u_target->isDead())
			return SPELL_FAILED_TARGET_NOT_DEAD;

		return SPELL_CANCAST_OK;
	}
};

class DummyPeriodicTrigger : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(DummyPeriodicTrigger);
	uint64 target_guid;
	DummyPeriodicTrigger(Spell* pSpell) : SpellScript(pSpell) { target_guid = 0; }

	void DummyAura(bool apply, Aura* aura)
	{
		Unit* target = aura->GetTarget();
		if (target == NULL)
			return;

		target_guid = target->GetGUID();
	}

	void OnAuraAdd(Aura* a)
	{
		for (uint8 i=0; i<a->m_modcount; ++i)
		{
			if (a->m_modList[i].m_type == SPELL_AURA_PERIODIC_TRIGGER_SPELL)
			{
				//were on the right spell :)
				a->SetPeriodicTarget(target_guid);
				return;
			}
		}
	}
};

void SetupOldSpellCPPSuff(ScriptMgr * mgr)
{
	mgr->register_spell_script(__Summon_Screecher_Spirit, &SummonScreecherSpirit::Create); // 12699
	/*
	#define __Tag_Murloc				30875
    #define __Tag_Murloc___0			30877
	*/
	mgr->register_spell_script(__Tag_Murloc____0     , &TagMurloc::Create); // 30877
	mgr->register_spell_script(__Administer_Antidote, &AdministerAntidote::Create); // 34665
	mgr->register_spell_script(__Yenniku_s_Release  , &YennikusRelease::Create); // 3607
	mgr->register_spell_script(__Rina_s_Diminution_Powder, &RinasDiminutionPowder::Create); //36310
	mgr->register_spell_script(__Liquid_Fire             , &LiquidFire::Create); // 32143
	/*
	#define __Fumping				39238
    #define __Fumping___0			39246
	*/
	mgr->register_spell_script(__Fumping____0, &Fumping::Create); // 39246
	mgr->register_spell_script(__Place_Draenei_Banner, &PlaceDraeneiBanner::Create); // 30988
	mgr->register_spell_script(__Demon_Broiled_Surprise, &DemonBroiledSurprise::Create); // 43723
	/*
	#define __Reindeer_Transformation				5154
	#define __Reindeer_Transformation___0			25860
	*/
	mgr->register_spell_script(__Reindeer_Transformation____0, &ReindeerTransformation::Create);  // 25860
	mgr->register_spell_script(__Smite_Demon, &SmiteDemon::Create); //demonslayer enchant 13907
	mgr->register_spell_script(__Absorb_Eye_of_Grillok , &AbsorbEyeofGrillok::Create); // 38554
	mgr->register_spell_script(__The_Seer_s_Presence , &TheSeersPresence::Create); // 36314
	mgr->register_spell_script(__Blackwhelp_Net, &BlackwhelpNet::Create); // 38177
	mgr->register_spell_script(__Converting_Sentry, &ConvertingSentry::Create); // 44997
	mgr->register_spell_script(__Carinda_s_Retribution, &CarindasRetribution::Create); // 30077

	//Dummy periodic triggers, anyone know anymore? basically period triggers with a dummy aura to get targets
	//Arcane Missiles
	/*
	#define __Arcane_Missiles_Rank_1				5143
	#define __Arcane_Missiles_Rank_2				5144
	#define __Arcane_Missiles_Rank_2___0			7269
	#define __Arcane_Missiles_Rank_3				5145
	#define __Arcane_Missiles_Rank_3___0			7270
	#define __Arcane_Missiles_Rank_4				8416
	#define __Arcane_Missiles_Rank_4___0			8419
	#define __Arcane_Missiles_Rank_5				8417
	#define __Arcane_Missiles_Rank_5___0			8418
	#define __Arcane_Missiles_Rank_6				10211
	#define __Arcane_Missiles_Rank_6___0			10273
	#define __Arcane_Missiles_Rank_7				10212
	#define __Arcane_Missiles_Rank_7___0			10274
	#define __Arcane_Missiles_Rank_8				25345
	#define __Arcane_Missiles_Rank_8___0			25346
	#define __Arcane_Missiles_Rank_9				27075
	#define __Arcane_Missiles_Rank_9___0			27076
	#define __Arcane_Missiles_Rank_10				38699
	#define __Arcane_Missiles_Rank_10___0			38700
	#define __Arcane_Missiles_Rank_11				38703
	#define __Arcane_Missiles_Rank_11___0			38704
	#define __Arcane_Missiles_Rank_12				42843
	#define __Arcane_Missiles_Rank_12___0			42844
	#define __Arcane_Missiles_Rank_13				42845
	#define __Arcane_Missiles_Rank_13___0			42846
	*/
	mgr->register_spell_script(__Arcane_Missiles_Rank_1, &DummyPeriodicTrigger::Create);
	mgr->register_spell_script(__Arcane_Missiles_Rank_2, &DummyPeriodicTrigger::Create);
	mgr->register_spell_script(__Arcane_Missiles_Rank_3, &DummyPeriodicTrigger::Create);
	mgr->register_spell_script(__Arcane_Missiles_Rank_4, &DummyPeriodicTrigger::Create);
	mgr->register_spell_script(__Arcane_Missiles_Rank_5, &DummyPeriodicTrigger::Create);
	mgr->register_spell_script(__Arcane_Missiles_Rank_6, &DummyPeriodicTrigger::Create);
	mgr->register_spell_script(__Arcane_Missiles_Rank_7, &DummyPeriodicTrigger::Create);
	mgr->register_spell_script(__Arcane_Missiles_Rank_8, &DummyPeriodicTrigger::Create);
	mgr->register_spell_script(__Arcane_Missiles_Rank_9, &DummyPeriodicTrigger::Create);
	mgr->register_spell_script(__Arcane_Missiles_Rank_10, &DummyPeriodicTrigger::Create);
	mgr->register_spell_script(__Arcane_Missiles_Rank_11 , &DummyPeriodicTrigger::Create); // 38704
	mgr->register_spell_script(__Arcane_Missiles_Rank_12, &DummyPeriodicTrigger::Create);
	mgr->register_spell_script(__Arcane_Missiles_Rank_13, &DummyPeriodicTrigger::Create);

	//Tame Beast
	/*
	#define __Tame_Beast				1515
	#define __Tame_Beast___0			1579
	#define __Tame_Beast___1			13481
	*/
	mgr->register_spell_script(__Tame_Beast, &DummyPeriodicTrigger::Create);

}