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

bool ColdSnap(uint32 i, Spell * pSpell)
{
	if(!pSpell->m_caster->IsPlayer()) return true;
    ((Player *)pSpell->m_caster)->ClearCooldownsOnLine(6, pSpell->m_spellInfo->Id);
    return true;
}

class IceBlock : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(IceBlock);
	IceBlock(Spell* pSpell) : SpellScript(pSpell) {}
	SpellCastError CanCast(bool tolerate)
	{
		if (!_spell->m_caster->IsUnit() || ((Unit *)_spell->m_caster)->HasAura(41425))
			return SPELL_FAILED_DAMAGE_IMMUNE;

		return SPELL_CANCAST_OK;
	}

	void OnCast()
	{
		if (_spell->m_caster->IsUnit())
			((Unit *)_spell->m_caster)->CastSpell(((Unit *)_spell->m_caster), 41425, true);
	}
};

class Hypothermia : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Hypothermia);
	Hypothermia(Spell* pSpell) : SpellScript(pSpell) {}

	void OnAuraAdd(Aura* a)
	{
		a->SetNegative(100);
	}
};

bool Invocationelementaireeau(uint32 i, Spell *pSpell)
{
	Unit *caster = ((Unit *)pSpell->m_caster);
	if( caster == NULL )
		return true;

	if( caster->HasAura(70937) )
		caster->CastSpell(caster, 70908, true);
	else
		caster->CastSpell(caster, 70907, true);

	return true;
}

void SetupMageSpells(ScriptMgr * mgr)
{
    mgr->register_dummy_spell(__Cold_Snap , &ColdSnap); // 11958
	mgr->register_spell_script(__Hypothermia , &Hypothermia::Create); // 41425

	/*
	#define __Ice_Block				27619
	#define __Ice_Block___0			36911
	#define __Ice_Block___1			41590
	#define __Ice_Block___2			45438
	#define __Ice_Block___3			45776
	#define __Ice_Block___4			46604
	#define __Ice_Block___5			46882
	#define __Ice_Block___6			56124
	#define __Ice_Block___7			56644
	*/
	mgr->register_spell_script(__Ice_Block____2 , &IceBlock::Create); // 45438
	mgr->register_dummy_spell(31687, &Invocationelementaireeau);


}
