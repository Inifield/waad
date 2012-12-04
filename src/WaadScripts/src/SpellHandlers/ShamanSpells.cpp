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

/************************************************************************/
/* Spell Defs                                                           */
/************************************************************************/

bool RockbiterWeapon(uint32 i, Spell* pSpell)
{
    uint32 enchantment_entry = 0;
    switch(pSpell->m_spellInfo->RankNumber)
    {
    case 1:
        enchantment_entry = 3021;
        break;
    case 2:
        enchantment_entry = 3024;
        break;
    case 3:
        enchantment_entry = 3027;
        break;
    case 4:
        enchantment_entry = 3030;
        break;
    case 5:
        enchantment_entry = 3033;
        break;
    case 6:
        enchantment_entry = 3036;
        break;
    case 7:
        enchantment_entry = 3039;
        break;
    case 8:
        enchantment_entry = 3042;
        break;
    case 9:
        enchantment_entry = 3018;
        break;
    }

	if(!enchantment_entry || !pSpell->m_caster->IsPlayer())
        return true;

    Item * item = ((Player *)pSpell->m_caster)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
    EnchantEntry * enchant = dbcEnchant.LookupEntry(enchantment_entry);
    if(!item || !enchant)
        return true;

    int32 Slot = item->HasEnchantment(enchant->Id);
    if(Slot >= 0)
        item->ModifyEnchantmentTime(Slot, 1800);
    else
    {
		//check if enchantment slot 1 is taken. If there was no enchantment there function will quit
		item->RemoveEnchantment(1);
		//this will also apply bonuses for us
        Slot = item->AddEnchantment(enchant, 1800, false, true, false, 1);   // 5min
        if(Slot < 0) return true;
    }

    sLog.outDebug("ShamanSpells.cpp :: Rockbiter Weapon Rank %u, enchant %u, slot %u", pSpell->m_spellInfo->RankNumber,
        enchantment_entry, Slot);
    
    return true;
}

class LinkedHPElemental : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(LinkedHPElemental);
	LinkedHPElemental(Creature* pCreature) : CreatureAIScript(pCreature) { }

	void OnLoad()
	{
		//we should be in world here, lets get totem
		Unit* totem=_unit->GetMapMgr()->GetUnit(_unit->GetUInt64Value(UNIT_FIELD_CREATEDBY));

		if (totem == NULL) //WTF
			return;

		_unit->ShareHealthWithUnit(totem);
	}
};

void SetupShamanSpells(ScriptMgr * mgr)
{
	/*
	#define __Rockbiter_Weapon				33640
	#define __Rockbiter_Weapon_Rank_1		8017
	#define __Rockbiter_Weapon_Rank_1___0	36494
	#define __Rockbiter_Weapon_Rank_1___1	36495
	#define __Rockbiter_Weapon_Rank_1___2	36496
	#define __Rockbiter_Weapon_Rank_1___3	36744
	#define __Rockbiter_Weapon_Rank_2		8018
	#define __Rockbiter_Weapon_Rank_2___0	36750
	#define __Rockbiter_Weapon_Rank_2___1	36751
	#define __Rockbiter_Weapon_Rank_2___2	36752
	#define __Rockbiter_Weapon_Rank_2___3	36753
	#define __Rockbiter_Weapon_Rank_3		8019
	#define __Rockbiter_Weapon_Rank_3___0	36754
	#define __Rockbiter_Weapon_Rank_3___1	36755
	#define __Rockbiter_Weapon_Rank_3___2	36756
	#define __Rockbiter_Weapon_Rank_3___3	36757
	#define __Rockbiter_Weapon_Rank_4		10399
	#define __Rockbiter_Weapon_Rank_4___0	36758
	#define __Rockbiter_Weapon_Rank_4___1	36759
	#define __Rockbiter_Weapon_Rank_4___2	36760
	#define __Rockbiter_Weapon_Rank_4___3	36761
	*/
    mgr->register_dummy_spell(__Rockbiter_Weapon_Rank_1, &RockbiterWeapon); // rank 1
    mgr->register_dummy_spell(__Rockbiter_Weapon_Rank_2, &RockbiterWeapon); // rank 2
    mgr->register_dummy_spell(__Rockbiter_Weapon_Rank_3, &RockbiterWeapon); // rank 3
    mgr->register_dummy_spell(__Rockbiter_Weapon_Rank_4, &RockbiterWeapon);// rank 4
	/* N'existe plus ? (Branruz)
    mgr->register_dummy_spell(16314, &RockbiterWeapon);// rank 5
    mgr->register_dummy_spell(16315, &RockbiterWeapon);// rank 6
    mgr->register_dummy_spell(16316, &RockbiterWeapon);// rank 7
	mgr->register_dummy_spell(25479, &RockbiterWeapon);// rank 8
    mgr->register_dummy_spell(25485, &RockbiterWeapon);// rank 9
	*/

	// N'existe plus
	//mgr->register_creature_script(15352, &LinkedHPElemental::Create);
	//mgr->register_creature_script(15438, &LinkedHPElemental::Create);


}
