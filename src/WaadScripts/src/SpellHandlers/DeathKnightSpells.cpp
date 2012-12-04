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

class DeathGrip : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(DeathGrip);
	DeathGrip(Spell* pSpell) : SpellScript(pSpell) {}

	void DummyEffect(uint32 EffectIndex)
	{
		if (_spell->GetUnitTarget() != NULL)
		{
			SpellEntry* sp=dbcSpell.LookupEntry(__Death_Grip ); // 49560
			if(!sp) return;
			Spell *spell = new Spell(((Player *)_spell->m_caster), sp, true, NULL);
		    SpellCastTargets targets(_spell->GetUnitTarget());
		    spell->prepare(&targets);
		}
	}
};

class DeathGrip2 : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(DeathGrip2);
	DeathGrip2(Spell *pSpell) : SpellScript(pSpell) {}

	void DummyEffect(uint32 EffectIndex)
	{
		Unit *u = _spell->GetUnitTarget();
		if (u != NULL)
		{
			//Spell *nextspell = objmgr.CreateSpell(u, dbcSpell.LookupEntry(57604), true, NULLAUR);
            SpellEntry* sp=dbcSpell.LookupEntry(__Death_Grip____7 ); // 57604
			if(!sp) return;
			Spell *spell = new Spell(((Player *)_spell->m_caster), sp, true, NULL);

			float r = _spell->GetRadius(EffectIndex);

			float angle = _spell->m_caster->calcRadAngle(_spell->m_caster->GetPositionX(), _spell->m_caster->GetPositionY(), u->GetPositionX(), u->GetPositionY());

			float x = _spell->m_caster->GetPositionX() + (r * cosf(angle));
			float y = _spell->m_caster->GetPositionY() + (r * sinf(angle));
			float z = _spell->m_caster->GetPositionZ(); //todo: get land height

			SpellCastTargets t;
			t.m_targetMask |= TARGET_FLAG_DEST_LOCATION;
			t.m_destX = x;
			t.m_destY = y;
			t.m_destZ = z;

			//nextspell->prepare(&t);
			spell->prepare(&t);
		}
	}
};

void SetupDeathKnightSpells(ScriptMgr* mgr)
{
	/*
	#define __Death_Grip				49560
	#define __Death_Grip___0			49575 // Effet	Summon Guardian
	#define __Death_Grip___1			49576
	#define __Death_Grip___2			51399
	#define __Death_Grip___3			53276
	#define __Death_Grip___4			55719
	#define __Death_Grip___5			57602
	#define __Death_Grip___6			57603
	#define __Death_Grip___7			57604
	#define __Death_Grip___8			61094
	*/
	mgr->register_spell_script(__Death_Grip     , &DeathGrip2::Create); // 49560
	mgr->register_spell_script(__Death_Grip____1 , &DeathGrip::Create); // 49576
	
}