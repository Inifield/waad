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

/*
	How to add a new paladin spell to the dummy spell handler:

		1) Add a new function to handle the spell at the end of this file but before the
		   SetupPaladinSpells() function. SetupPaladinSpells() must always be the last function.

		2) Register the dummy spell by adding a new line to the end of the list in the
		   SetupPaladinSpells() function.

	Please look at how the other spells are handled and try to use the
	same variable names and formatting style in your new spell handler.
*/

// *****************************************************************************

bool SealOfRighteousness(uint32 i, Aura* pAura, bool apply)
{
	if(i != 0) return false;

	uint32 applyId = 0;
	Unit * u_caster = pAura->GetUnitCaster(); //pointeur de type unit qui pointe le lanceur

	if(u_caster == 0 || !u_caster->IsPlayer()) return false; //arret du script si aucun lanceur ou si ce n'est pas un pj

	switch(pAura->m_spellProto->Id)
	{
	default:
	case __Seal_of_Righteousness:         // Rank 1: "Melee attacks cause an additional $/87;20187s3 to $/25;20187s3 Holy damage."
	case __Seal_of_Righteousness____0:
			applyId = __Judgement_of_Righteousness_Rank_1;
			break;
			/* Spells n'existe plus sur 3.0.9 (Branruz)
	case 20287:         // Rank 2: $/25;20280s3
			applyId = 20280;
			break;
	case 20288:         // Rank 3:
			applyId = 20281;
			break;
	case 20289:         // Rank 4
			applyId = 20282;
			break;
	case 20290:         // Rank 5
			applyId = 20283;
			break;
	case 20291:         // Rank 6
			applyId = 20284;
			break;
	case 20292:         // Rank 7
			applyId = 20285;
			break;
	case 20293:         // Rank 8
			applyId = 20286;
			break;
	case 27155:         // Rank 9
			applyId = 27157;
			break;
			*/
	}

	SpellEntry * m_spellInfo = dbcSpell.LookupEntry(applyId);

	if(apply == true)
	{
		int32 value = 0;
		//float randomPointsPerLevel = m_spellInfo->EffectDicePerLevel[2]; //plus en fonction en 3.3.5.a (B.B.)
		int32 basePoints = m_spellInfo->EffectBasePoints[2] + 1;
		int32 randomPoints = m_spellInfo->EffectDieSides[2];
		if(u_caster)
				randomPoints += u_caster->getLevel();// * (int32)randomPointsPerLevel; plus en fonction en 3.3.5.a (B.B.)

		if(randomPoints <= 1)
				value = basePoints;
		else
				value = basePoints + rand()  %randomPoints;

		//this may be dangerous but let it be
		/*if(m_spellInfo->SpellGroupType)
		{
				SM_FIValue(u_caster->SM_FDummy,&value,m_spellInfo->SpellGroupType);
				SM_PIValue(u_caster->SM_PDummy,&value,m_spellInfo->SpellGroupType);
		}*/

		// add spell damage!
		uint32 max_dmg = value / 21;
		uint32 min_dmg = value / 27;
		((Player*)u_caster)->AddOnStrikeSpellDamage(pAura->m_spellProto->Id, min_dmg, max_dmg);

		// set the seal business
		if(u_caster->GetTypeId() == TYPEID_PLAYER)
		{
				((Player*)u_caster)->judgespell = applyId;
				((Player*)u_caster)->Seal = pAura->m_spellProto->Id;
		}
		u_caster->SetFlag(UNIT_FIELD_AURASTATE, 16);
	}
	else
	{
			((Player*)u_caster)->RemoveOnStrikeSpellDamage(pAura->m_spellProto->Id);
			// set the seal business
			if(u_caster->GetTypeId() == TYPEID_PLAYER)
			{
					((Player*)u_caster)->judgespell = 0;
					((Player*)u_caster)->Seal = 0;
			}
			u_caster->RemoveFlag(UNIT_FIELD_AURASTATE, 16);
	}

	return true;
}

// -----------------------------------------------------------------------------

bool HolyShock(uint32 i, Spell *pSpell)
{
	Unit *target = pSpell->GetUnitTarget();
	if(!pSpell->m_caster->IsPlayer() || !target) return true;

	uint32 newspell = 0;

	if(isAttackable(((Player *)pSpell->m_caster),target)) // if its an enemy
	{
		switch(pSpell->m_spellInfo->Id)
		{
		case __Holy_Shock_Rank_1: newspell = __Holy_Shock_Rank_1____0; break;
		case __Holy_Shock_Rank_2: newspell = __Holy_Shock_Rank_2____0; break;
		case __Holy_Shock_Rank_3: newspell = __Holy_Shock_Rank_3____0; break;
		case __Holy_Shock_Rank_4: newspell = __Holy_Shock_Rank_4____0; break;
		case __Holy_Shock_Rank_5: newspell = __Holy_Shock_Rank_5____0; break;
        case __Holy_Shock_Rank_6: newspell = __Holy_Shock_Rank_6____0; break;
		default:
        case __Holy_Shock_Rank_7: newspell = __Holy_Shock_Rank_7____0; break;
		}
	}
	else // if its friendly
	{
		switch(pSpell->m_spellInfo->Id)
		{
		case __Holy_Shock_Rank_1: newspell = __Holy_Shock_Rank_1____1; break;
		case __Holy_Shock_Rank_2: newspell = __Holy_Shock_Rank_2____1; break;
		case __Holy_Shock_Rank_3: newspell = __Holy_Shock_Rank_3____1; break;
		case __Holy_Shock_Rank_4: newspell = __Holy_Shock_Rank_4____1; break;
		case __Holy_Shock_Rank_5: newspell = __Holy_Shock_Rank_5____1; break;
		case __Holy_Shock_Rank_6: newspell = __Holy_Shock_Rank_6____1; break;
		default:
		case __Holy_Shock_Rank_7: newspell = __Holy_Shock_Rank_7____1; break;

		}
	}

	SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
	if(!spInfo) return true;

	((Player *)pSpell->m_caster)->CastSpell(target, spInfo, true);
	return true;
}


class CrusaderStrike : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(CrusaderStrike);
	CrusaderStrike(Spell* pSpell) : SpellScript(pSpell) {}
	
	void DummyMeleeEffect(uint32 EffectIndex)
	{
		Unit* u_target=_spell->GetUnitTarget();

		if (u_target == NULL)
			return;

		for(int x = MAX_POSITIVE_AURAS; x<=MAX_AURAS; ++x)
		{
			if(u_target->m_auras[x] && Spell::IsJudgement(u_target->m_auras[x]->GetSpellProto()))
				u_target->m_auras[x]->HandleStackAdd();
		}
	}
};

// ADD NEW FUNCTIONS ABOVE THIS LINE
// *****************************************************************************

void SetupPaladinSpells(ScriptMgr * mgr)
{
	/*
	#define __Seal_of_Righteousness				20154
	#define __Seal_of_Righteousness___0			21084
	#define __Seal_of_Righteousness___1			25742
	*/
	mgr->register_dummy_aura( __Seal_of_Righteousness, &SealOfRighteousness); // Judgement of Justice (Rank 1)
	mgr->register_dummy_aura( __Seal_of_Righteousness____0, &SealOfRighteousness); // Seal of Righteousness (Rank 1)
	//mgr->register_dummy_aura( 20287, &SealOfRighteousness); // Seal of Righteousness (Rank 2)
	//mgr->register_dummy_aura( 20288, &SealOfRighteousness); // Seal of Righteousness (Rank 3)
	//mgr->register_dummy_aura( 20289, &SealOfRighteousness); // Seal of Righteousness (Rank 4)
	//mgr->register_dummy_aura( 20290, &SealOfRighteousness); // Seal of Righteousness (Rank 5)
	//mgr->register_dummy_aura( 20291, &SealOfRighteousness); // Seal of Righteousness (Rank 6)
	//mgr->register_dummy_aura( 20292, &SealOfRighteousness); // Seal of Righteousness (Rank 7)
	//mgr->register_dummy_aura( 20293, &SealOfRighteousness); // Seal of Righteousness (Rank 8)
	//mgr->register_dummy_aura( 27155, &SealOfRighteousness); // Seal of Righteousness (Rank 9)

	/*
	#define __Holy_Shock_Rank_1				20473
	#define __Holy_Shock_Rank_1___0			25912
	#define __Holy_Shock_Rank_1___1			25914
	#define __Holy_Shock_Rank_2				20929
	#define __Holy_Shock_Rank_2___0			25911
	#define __Holy_Shock_Rank_2___1			25913
	#define __Holy_Shock_Rank_3				20930
	#define __Holy_Shock_Rank_3___0			25902
	#define __Holy_Shock_Rank_3___1			25903
	#define __Holy_Shock_Rank_4				27174
	#define __Holy_Shock_Rank_4___0			27175
	#define __Holy_Shock_Rank_4___1			27176
	#define __Holy_Shock_Rank_5				33072
	#define __Holy_Shock_Rank_5___0			33073
	#define __Holy_Shock_Rank_5___1			33074
	#define __Holy_Shock_Rank_6				48820
	#define __Holy_Shock_Rank_6___0			48822
	#define __Holy_Shock_Rank_6___1			48824
	#define __Holy_Shock_Rank_7				48821
	#define __Holy_Shock_Rank_7___0			48823
	#define __Holy_Shock_Rank_7___1			48825
	*/
	mgr->register_dummy_spell(__Holy_Shock_Rank_1, &HolyShock);           // Holy Shock rank 1
	mgr->register_dummy_spell(__Holy_Shock_Rank_2, &HolyShock);           // Holy Shock rank 2
	mgr->register_dummy_spell(__Holy_Shock_Rank_3, &HolyShock);           // Holy Shock rank 3
	mgr->register_dummy_spell(__Holy_Shock_Rank_4, &HolyShock);           // Holy Shock rank 4
	mgr->register_dummy_spell(__Holy_Shock_Rank_5, &HolyShock);           // Holy Shock rank 5
	mgr->register_dummy_spell(__Holy_Shock_Rank_6, &HolyShock);           
	mgr->register_dummy_spell(__Holy_Shock_Rank_7, &HolyShock);           

	/*
	#define __Crusader_Strike_Rank_1	35509 
	*/
	mgr->register_spell_script(__Crusader_Strike_Rank_1, &CrusaderStrike::Create); // 35395


// REGISTER NEW DUMMY SPELLS ABOVE THIS LINE
// *****************************************************************************
}
