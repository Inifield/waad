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

bool Preparation(uint32 i, Spell * pSpell)
{
    Player * playerTarget = ((Player *)pSpell->m_caster);
    if(playerTarget == 0) return true;

    playerTarget->ClearCooldownsOnLine(39, pSpell->m_spellInfo->Id); // line - subtlety
    playerTarget->ClearCooldownsOnLine(38, pSpell->m_spellInfo->Id); // line - combat
    playerTarget->ClearCooldownsOnLine(253, pSpell->m_spellInfo->Id);// line - assassination
    return true;
}

class Ambush : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Ambush);
	Ambush(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		if (!_spell->m_caster->IsUnit() || !((Unit *)_spell->m_caster)->IsStealth())
			return SPELL_FAILED_ONLY_STEALTHED;

		if (_spell->m_caster->IsPlayer())
		{
			Item * pMainHand = ((Player *)_spell->m_caster)->GetItemInterface()->GetInventoryItem( INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_MAINHAND );
			if( !pMainHand || pMainHand->GetProto()->Class != 2 || pMainHand->GetProto()->SubClass != 15 )
				return SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND;
		}

		return SPELL_CANCAST_OK;
	}
};

class Backstab : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Backstab);
	Backstab(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		if (_spell->m_caster->IsPlayer())
		{
			Item * pMainHand = ((Player *)_spell->m_caster)->GetItemInterface()->GetInventoryItem( INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_MAINHAND );
			if( !pMainHand || pMainHand->GetProto()->Class != 2 || pMainHand->GetProto()->SubClass != 15 )
				return SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND;
		}

		return SPELL_CANCAST_OK;
	}
};

class Stealth : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Stealth);
	Stealth(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		if (!_spell->m_caster->IsUnit() || ((Unit *)_spell->m_caster)->CombatStatus.IsInCombat())
			return SPELL_FAILED_TARGET_IN_COMBAT;

		if (_spell->m_caster->IsPlayer() && ((Player *)_spell->m_caster)->m_bgHasFlag)
			return SPELL_FAILED_SPELL_UNAVAILABLE;

		return SPELL_CANCAST_OK;
	}
};

class Vanish : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Vanish);
	Vanish(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		if (_spell->m_caster->IsPlayer() && ((Player *)_spell->m_caster)->m_bgHasFlag)
			return SPELL_FAILED_SPELL_UNAVAILABLE;

		return SPELL_CANCAST_OK;
	}
};

class Garrote : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Garrote);
	Garrote(Spell* pSpell) : SpellScript(pSpell) {}
	void CalculateEffect(uint32 EffectIndex, Unit* target, int32* value)
	{
		// WoWWiki says +( 0.18 * attack power / number of ticks )
		// Tooltip gives no specific reading, but says ", increased by your attack power.".
		if( _spell->m_caster->IsUnit() )
		{
			float ap = (float)((Unit *)_spell->m_caster)->GetAP();
			if( EffectIndex == 0 )
			{
				*value += (uint32) ceilf( ( ap * 0.18f ) / 6 );
			}
		}
	}
};

class Rupture : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Rupture);
	Rupture(Spell* pSpell) : SpellScript(pSpell) {}
	void CalculateEffect(uint32 EffectIndex, Unit* target, int32* value)
	{
		/* 
		1pt = Attack Power * 0.04 + x
		2pt = Attack Power * 0.10 + y
		3pt = Attack Power * 0.18 + z
		4pt = Attack Power * 0.21 + a
		5pt = Attack Power * 0.24 + b
		*/
		if( _spell->m_caster->IsPlayer() )
		{
			float ap = (float)((Player *)_spell->m_caster)->GetAP();
			float cp = (float)((Player *)_spell->m_caster)->m_comboPoints;
			*value += (uint32) ceilf( ( ap * ( 0.04f * cp ) ) / ( ( 6 + ( cp * 2 ) ) / 2 ) );
		}
	}
};

class Envenom : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Envenom);
	Envenom(Spell* pSpell) : SpellScript(pSpell) {}
	void CalculateEffect(uint32 EffectIndex, Unit* target, int32* value)
	{
		if( _spell->m_caster->IsPlayer() && EffectIndex == 0 )//Envenom
		{
			*value *= ((Player *)_spell->m_caster)->m_comboPoints;
			*value += (uint32)(((Player *)_spell->m_caster)->GetAP() * (0.03f * ((Player *)_spell->m_caster)->m_comboPoints));
			_spell->m_requiresCP = true;
		}
	}
};

class Eviscerate : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Eviscerate);
	Eviscerate(Spell* pSpell) : SpellScript(pSpell) {}
	void CalculateEffect(uint32 EffectIndex, Unit* target, int32* value)
	{
		if( _spell->m_caster->IsPlayer() && EffectIndex == 0 )//Envenom
		{
			value += (uint32)( ((Player *)_spell->m_caster)->GetAP() * ( 0.03 * ((Player *)_spell->m_caster)->m_comboPoints ) );
		}
	}
};

class Hemorrhage : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Hemorrhage);
	Hemorrhage(Spell* pSpell) : SpellScript(pSpell) {}
	
	void DummyMeleeEffect(uint32 EffectIndex)
	{
		Unit* u_target=_spell->GetUnitTarget();

		if (u_target == NULL || !_spell->m_caster->IsPlayer())
			return;

		((Player *)_spell->m_caster)->AddComboPoints(u_target->GetGUID(), 1);
	}
};

void SetupRogueSpells(ScriptMgr * mgr)
{
    	/*
	#define __Preparation				14185
	#define __Preparation___0			44521
	*/
    //mgr->register_dummy_spell(__Preparation, &Preparation);

	/*
	#define __Backstab_Rank_1				53
	#define __Backstab_Rank_2				2589
	#define __Backstab_Rank_3				2590
	#define __Backstab_Rank_4				2591
	#define __Backstab_Rank_5				8721
	#define __Backstab_Rank_6				11279
	#define __Backstab_Rank_7				11280
	#define __Backstab_Rank_8				11281
	#define __Backstab_Rank_9				25300
	#define __Backstab_Rank_10				26863
	#define __Backstab_Rank_11				48656
	#define __Backstab_Rank_12				48657
	*/

	mgr->register_spell_script(__Backstab_Rank_1, &Backstab::Create);
	mgr->register_spell_script(__Backstab_Rank_2, &Backstab::Create);
	mgr->register_spell_script(__Backstab_Rank_3, &Backstab::Create);
	mgr->register_spell_script(__Backstab_Rank_4, &Backstab::Create);
	mgr->register_spell_script(__Backstab_Rank_5, &Backstab::Create);
	mgr->register_spell_script(__Backstab_Rank_6, &Backstab::Create);
	mgr->register_spell_script(__Backstab_Rank_7, &Backstab::Create);
	mgr->register_spell_script(__Backstab_Rank_8, &Backstab::Create);
	mgr->register_spell_script(__Backstab_Rank_9, &Backstab::Create);
	mgr->register_spell_script(__Backstab_Rank_10, &Backstab::Create);
	mgr->register_spell_script(__Backstab_Rank_11, &Backstab::Create);
	mgr->register_spell_script(__Backstab_Rank_12, &Backstab::Create);

	/*
	#define __Ambush_Rank_1				8676
	#define __Ambush_Rank_2				8724
	#define __Ambush_Rank_3				8725
	#define __Ambush_Rank_4				11267
	#define __Ambush_Rank_5				11268
	#define __Ambush_Rank_6				11269
	#define __Ambush_Rank_7				27441
	#define __Ambush_Rank_8				48689
	#define __Ambush_Rank_9				48690
	#define __Ambush_Rank_10			48691
	*/
	mgr->register_spell_script(__Ambush_Rank_1, &Ambush::Create);
	mgr->register_spell_script(__Ambush_Rank_2, &Ambush::Create);
	mgr->register_spell_script(__Ambush_Rank_3, &Ambush::Create);
	mgr->register_spell_script(__Ambush_Rank_4, &Ambush::Create);
	mgr->register_spell_script(__Ambush_Rank_5, &Ambush::Create);
	mgr->register_spell_script(__Ambush_Rank_6, &Ambush::Create);
	mgr->register_spell_script(__Ambush_Rank_7, &Ambush::Create);
	mgr->register_spell_script(__Ambush_Rank_8, &Ambush::Create);
	mgr->register_spell_script(__Ambush_Rank_9, &Ambush::Create);
	mgr->register_spell_script(__Ambush_Rank_10, &Ambush::Create);

	/*
	#define __Stealth_Rank_1				1784
	#define __Stealth_Rank_2				1785
	#define __Stealth_Rank_3				1786
	#define __Stealth_Rank_4				1787
	*/
	mgr->register_spell_script(__Stealth       , &Stealth::Create); // 332.11403
	mgr->register_spell_script(__Stealth_Rank_2, &Stealth::Create);
	mgr->register_spell_script(__Stealth_Rank_3, &Stealth::Create);
	mgr->register_spell_script(__Stealth_Rank_4, &Stealth::Create);

	/*
	#define __Vanish_Rank_1				1856
	#define __Vanish_Rank_1___0			11327
	#define __Vanish_Rank_2				1857
	#define __Vanish_Rank_2___0			11329
	#define __Vanish_Rank_2___1			27617
	#define __Vanish_Rank_3				26888
	#define __Vanish_Rank_3___0			26889
	*/
	mgr->register_spell_script(__Vanish_Rank_1, &Vanish::Create);
	mgr->register_spell_script(__Vanish_Rank_2, &Vanish::Create);
	mgr->register_spell_script(__Vanish_Rank_3____0, &Vanish::Create); // 26889

	/*
	#define __Garrote_Rank_1				703
	#define __Garrote_Rank_2				8631
	#define __Garrote_Rank_3				8632
	#define __Garrote_Rank_4				8633
	#define __Garrote_Rank_4___0			8818
	#define __Garrote_Rank_5				11289
	#define __Garrote_Rank_6				11290
	#define __Garrote_Rank_7				26839
	#define __Garrote_Rank_8				26884
	#define __Garrote_Rank_9				48675
	#define __Garrote_Rank_10				48676
	*/
	mgr->register_spell_script(__Garrote_Rank_1, &Garrote::Create);
	mgr->register_spell_script(__Garrote_Rank_2, &Garrote::Create);
	mgr->register_spell_script(__Garrote_Rank_3, &Garrote::Create);
	mgr->register_spell_script(__Garrote_Rank_4, &Garrote::Create);
	mgr->register_spell_script(__Garrote_Rank_5, &Garrote::Create);
	mgr->register_spell_script(__Garrote_Rank_6, &Garrote::Create);
	mgr->register_spell_script(__Garrote_Rank_7, &Garrote::Create);
	mgr->register_spell_script(__Garrote_Rank_8, &Garrote::Create);
	mgr->register_spell_script(__Garrote_Rank_9, &Garrote::Create);
	mgr->register_spell_script(__Garrote_Rank_10, &Garrote::Create);

	/*
	#define __Rupture_Rank_1				1943
	#define __Rupture_Rank_2				8639
	#define __Rupture_Rank_3				8640
	#define __Rupture_Rank_4				11273
	#define __Rupture_Rank_5				11274
	#define __Rupture_Rank_6				11275
	#define __Rupture_Rank_7				26867
	#define __Rupture_Rank_8				48671
	#define __Rupture_Rank_9				48672
	*/
	mgr->register_spell_script(__Rupture_Rank_1, &Rupture::Create);
	mgr->register_spell_script(__Rupture_Rank_2, &Rupture::Create);
	mgr->register_spell_script(__Rupture_Rank_3, &Rupture::Create);
	mgr->register_spell_script(__Rupture_Rank_4, &Rupture::Create);
	mgr->register_spell_script(__Rupture_Rank_5, &Rupture::Create);
	mgr->register_spell_script(__Rupture_Rank_6, &Rupture::Create);
	mgr->register_spell_script(__Rupture_Rank_7, &Rupture::Create);
	mgr->register_spell_script(__Rupture_Rank_8, &Rupture::Create);
	mgr->register_spell_script(__Rupture_Rank_9, &Rupture::Create);

	/*
	#define __Envenom_Rank_1				32645
	#define __Envenom_Rank_2				32684
	#define __Envenom_Rank_2___0			39967
	#define __Envenom_Rank_3				57992
	#define __Envenom_Rank_4				57993
	*/
	mgr->register_spell_script(__Envenom_Rank_1, &Envenom::Create);
	mgr->register_spell_script(__Envenom_Rank_2, &Envenom::Create);
	mgr->register_spell_script(__Envenom_Rank_3, &Envenom::Create);
	mgr->register_spell_script(__Envenom_Rank_4, &Envenom::Create);

	/*
	#define __Eviscerate_Rank_1				2098
	#define __Eviscerate_Rank_2				6760
	#define __Eviscerate_Rank_3				6761
	#define __Eviscerate_Rank_4				6762
	#define __Eviscerate_Rank_5				8623
	#define __Eviscerate_Rank_6				8624
	#define __Eviscerate_Rank_7				11299
	#define __Eviscerate_Rank_8				11300
	#define __Eviscerate_Rank_9				31016
	#define __Eviscerate_Rank_10			26865
	#define __Eviscerate_Rank_11			48667
	#define __Eviscerate_Rank_12			48668
	*/
	mgr->register_spell_script(__Eviscerate_Rank_1, &Eviscerate::Create);
	mgr->register_spell_script(__Eviscerate_Rank_2, &Eviscerate::Create);
	mgr->register_spell_script(__Eviscerate_Rank_3, &Eviscerate::Create);
	mgr->register_spell_script(__Eviscerate_Rank_4, &Eviscerate::Create);
	mgr->register_spell_script(__Eviscerate_Rank_5, &Eviscerate::Create);
	mgr->register_spell_script(__Eviscerate_Rank_6, &Eviscerate::Create);
	mgr->register_spell_script(__Eviscerate_Rank_7, &Eviscerate::Create);
	mgr->register_spell_script(__Eviscerate_Rank_8, &Eviscerate::Create);
	mgr->register_spell_script(__Eviscerate_Rank_9, &Eviscerate::Create);
	mgr->register_spell_script(__Eviscerate_Rank_10, &Eviscerate::Create);
	mgr->register_spell_script(__Eviscerate_Rank_11, &Eviscerate::Create);
	mgr->register_spell_script(__Eviscerate_Rank_12, &Eviscerate::Create);

	/*
	#define __Hemorrhage_Rank_1				16511
	#define __Hemorrhage_Rank_2				17347
	#define __Hemorrhage_Rank_3				17348
	#define __Hemorrhage_Rank_4				26864
	#define __Hemorrhage_Rank_5				48660
	*/
	mgr->register_spell_script(__Hemorrhage_Rank_1, &Hemorrhage::Create);
	mgr->register_spell_script(__Hemorrhage_Rank_2, &Hemorrhage::Create);
	mgr->register_spell_script(__Hemorrhage_Rank_3, &Hemorrhage::Create);
	mgr->register_spell_script(__Hemorrhage_Rank_4, &Hemorrhage::Create);
	mgr->register_spell_script(__Hemorrhage_Rank_5, &Hemorrhage::Create);
}
