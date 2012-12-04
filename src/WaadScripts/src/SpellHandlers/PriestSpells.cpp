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

class MindControl : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(MindControl);
	MindControl(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target=_spell->GetUnitTarget();
		if (!_spell->m_caster->IsUnit() || u_target == NULL || int32(u_target->getLevel()) > _spell->m_spellInfo->EffectBasePoints[0]+1 + int32(((Unit *)_spell->m_caster)->getLevel() - _spell->m_spellInfo->spellLevel))
			return SPELL_FAILED_HIGHLEVEL;

		if (u_target->IsCreature() && (static_cast<Creature*>(u_target)->GetCreatureName() != NULL || static_cast<Creature*>(u_target)->GetCreatureName()->Rank > ELITE_ELITE))
			return SPELL_FAILED_HIGHLEVEL;

		return SPELL_CANCAST_OK;
	}
};

class PowerWordShield : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(PowerWordShield);
	Unit* u_target;
	PowerWordShield(Spell* pSpell) : SpellScript(pSpell) { u_target = NULL; }
	SpellCastError CanCast(bool tolerate)
	{
		Unit* u_target=_spell->GetUnitTarget();
		if (u_target == NULL || u_target->HasAura(6788))
			return SPELL_FAILED_DAMAGE_IMMUNE;

		return SPELL_CANCAST_OK;
	}

	void OnCast()
	{
		if (_spell->m_caster->IsUnit() && u_target != NULL)
			((Unit *)_spell->m_caster)->CastSpell(u_target, 6788, true);
	}
};

class PrayerOfMending : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(PrayerOfMending);
	PrayerOfMending(Spell* pSpell) : SpellScript(pSpell) {}
};

void SetupPriestSpells(ScriptMgr * mgr)
{
	/*
	#define __Mind_Control				605
	#define __Mind_Control___0				11446
	#define __Mind_Control___1				15690
	#define __Mind_Control___2				36797
	#define __Mind_Control___3				36798
	#define __Mind_Control___4				43550
	#define __Mind_Control___5				43871
	#define __Mind_Control___6				43875
	#define __Mind_Control___7				45112
	*/
	mgr->register_spell_script(__Mind_Control, &MindControl::Create);
	//mgr->register_spell_script(10911, &MindControl::Create); // N'existe plus
	//mgr->register_spell_script(10912, &MindControl::Create); // N'existe plus

	/*
	#define __Power_Word__Shield_Rank_1				17
	#define __Power_Word__Shield_Rank_2				592
	#define __Power_Word__Shield_Rank_3				600
	#define __Power_Word__Shield_Rank_4				3747
	#define __Power_Word__Shield_Rank_5				6065
	#define __Power_Word__Shield_Rank_6				6066
	#define __Power_Word__Shield_Rank_7				10898
	#define __Power_Word__Shield_Rank_8				10899
	#define __Power_Word__Shield_Rank_9				10900
	#define __Power_Word__Shield_Rank_10			10901
	#define __Power_Word__Shield_Rank_10___0		27607
	#define __Power_Word__Shield_Rank_11			25217
	#define __Power_Word__Shield_Rank_12			25218
	#define __Power_Word__Shield_Rank_13			48065
	#define __Power_Word__Shield_Rank_14			48066
	*/
	mgr->register_spell_script(__Power_Word__Shield_Rank_1, &PowerWordShield::Create);
	mgr->register_spell_script(__Power_Word__Shield_Rank_2, &PowerWordShield::Create);
	mgr->register_spell_script(__Power_Word__Shield_Rank_3, &PowerWordShield::Create);
	mgr->register_spell_script(__Power_Word__Shield_Rank_4, &PowerWordShield::Create);
	mgr->register_spell_script(__Power_Word__Shield_Rank_5, &PowerWordShield::Create);
	mgr->register_spell_script(__Power_Word__Shield_Rank_6, &PowerWordShield::Create);
	mgr->register_spell_script(__Power_Word__Shield_Rank_7, &PowerWordShield::Create);
	mgr->register_spell_script(__Power_Word__Shield_Rank_8, &PowerWordShield::Create);
	mgr->register_spell_script(__Power_Word__Shield_Rank_9, &PowerWordShield::Create);
	mgr->register_spell_script(__Power_Word__Shield_Rank_10, &PowerWordShield::Create);
	mgr->register_spell_script(__Power_Word__Shield_Rank_11, &PowerWordShield::Create);
	mgr->register_spell_script(__Power_Word__Shield_Rank_12, &PowerWordShield::Create);
	mgr->register_spell_script(__Power_Word__Shield_Rank_13, &PowerWordShield::Create);
	mgr->register_spell_script(__Power_Word__Shield_Rank_14, &PowerWordShield::Create);

	/*
	#define __Prayer_of_Mending_Rank_1				33076
	#define __Prayer_of_Mending_Rank_1___0			41635
	#define __Prayer_of_Mending_Rank_2				48110
	#define __Prayer_of_Mending_Rank_2___0			48112
	#define __Prayer_of_Mending_Rank_3				48111
	#define __Prayer_of_Mending_Rank_3___0			48113
	*/
	mgr->register_spell_script(__Prayer_of_Mending_Rank_1____0, &PrayerOfMending::Create); // 41635
	mgr->register_spell_script(__Prayer_of_Mending_Rank_2____0, &PrayerOfMending::Create);
	mgr->register_spell_script(__Prayer_of_Mending_Rank_3____0, &PrayerOfMending::Create);
}
