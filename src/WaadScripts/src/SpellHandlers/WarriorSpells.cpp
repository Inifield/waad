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

bool Charge(uint32 i, Spell* pSpell)
{
    uint32 rage_to_gen;

	if(!pSpell->m_caster->IsPlayer()) return true;

    switch(pSpell->m_spellInfo->Id)
    {
     case __Charge_Rank_1: rage_to_gen =  90; break;
     case __Charge_Rank_2: rage_to_gen = 120; break;
	 case __Charge_Rank_3: rage_to_gen = 150; break;
	 case __Charge_Rank_4: rage_to_gen = 180; break;
	 case __Charge_Rank_5: rage_to_gen = 210; break;
     default:
	 case __Charge_Rank_6: rage_to_gen = 240; break;
    }

	for(set<uint32>::iterator itr = ((Player *)pSpell->m_caster)->mSpells.begin(); itr != ((Player *)pSpell->m_caster)->mSpells.end(); ++itr)
	{
		if(*itr == __Improved_Charge_Rank_2) rage_to_gen += 60;
		if(*itr == __Improved_Charge_Rank_1) rage_to_gen += 30;
	}

    // Add the rage to the caster
    uint32 val = ((Player *)pSpell->m_caster)->GetUInt32Value(UNIT_FIELD_POWER2);
    uint32 max = ((Player *)pSpell->m_caster)->GetUInt32Value(UNIT_FIELD_MAXPOWER2);
    val += rage_to_gen;
    if(val > max)
        val = max;
    
    ((Player *)pSpell->m_caster)->SetUInt32Value(UNIT_FIELD_POWER2, val);
  //  script_debuglog("WarriorSpells.cpp :: Charge generate %u rage on "I64FMT, rage_to_gen, pSpell->u_caster->GetGUID());
    return true;
}

bool Execute(uint32 i, Spell* pSpell)
{
    //uint32 uSpellId = pSpell->m_spellInfo->Id;
    uint32 base_dmg = pSpell->damage;
    /*
    Attempt to finish off a wounded foe, causing 125 damage and converting each extra point
    of rage into 3 additional damage.  Only usable on enemies that have less than 20% health.
    */

    Unit * target = pSpell->GetUnitTarget();
	if(!target || !pSpell->m_caster->IsUnit()) return true;

    // "Only usable on enemies that have less than 20% health."
    if(target->GetHealthPct() > 20)
    {
        // send failed
        pSpell->SendCastResult(SPELL_FAILED_BAD_TARGETS);
        return true;
    }

    // get the caster's rage points, and convert them
    // formula is 3 damage * spell rank * rage points
    uint32 add_damage = (3 * pSpell->m_spellInfo->RankNumber);
    add_damage *= ((Unit *)pSpell->m_caster)->GetUInt32Value(UNIT_FIELD_POWER2) / 10;   // rage is *10 always
    
    // send spell damage log
	//pSpell->u_caster->SpellNonMeleeDamageLog(target, 20647, base_dmg + add_damage, false);
	SpellEntry *sp_for_the_logs = dbcSpell.LookupEntry(20647); // __Execute__0
	if(!sp_for_the_logs) return true;

	((Unit *)pSpell->m_caster)->Strike( target, MELEE, sp_for_the_logs, base_dmg + add_damage, 0, 0, true, true );
	// zero rage
    ((Unit *)pSpell->m_caster)->SetUInt32Value(UNIT_FIELD_POWER2, 0);
    return true;
}

bool Bloodrage(uint32 i, Spell* pSpell)
{
  // Put the player in Combat (gotta find when to put him ooc)


  return true;
}

class VictoryRush : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(VictoryRush);
	VictoryRush(Spell* pSpell) : SpellScript(pSpell) {}
	void CalculateEffect(uint32 EffectIndex, Unit* target, int32* value)
	{
		if(EffectIndex == 0 && _spell->m_caster->IsUnit()) 
			*value = (*value * ((Unit *)_spell->m_caster)->GetAP())/100;
	}
};

class Devastate : public SpellScript
{
public:
	ADD_SPELL_FACTORY_FUNCTION(Devastate);
	Devastate(Spell* pSpell) : SpellScript(pSpell) {}
	void DummyMeleeEffect(uint32 EffectIndex)
	{
		Unit* u_target=_spell->GetUnitTarget();

		if (u_target == NULL || !_spell->m_caster->IsPlayer())
			return;

		//count the number of sunder armors on target
		uint32 sunder_count=0;
		SpellEntry* sp=dbcSpell.LookupEntry(((Player *)_spell->m_caster)->GetMaxRankByHash(SPELL_HASH_SUNDER_ARMOR));

		for(uint32 x = MAX_POSITIVE_AURAS; x < MAX_AURAS; ++x)
		{
			if(u_target->m_auras[x] != NULL && u_target->m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_SUNDER_ARMOR)
			{
				sunder_count++;
				sp=u_target->m_auras[x]->GetSpellProto();
			}
		}
		if(sp == NULL)
			return;

		//we should also cast sunder armor effect on target with or without dmg
		Spell *spell = new Spell(((Player *)_spell->m_caster), sp, true, NULL);
		SpellCastTargets targets(u_target);
		spell->prepare(&targets);

		_spell->damage = _spell->damage * sunder_count;
	}
};


/* Module info */
void SetupWarriorSpells(ScriptMgr * mgr)
{
	/* 3.0.9
	#define __Charge_Rank_1				100
    #define __Charge_Rank_1___0			7370
    #define __Charge_Rank_1___1			25999
    #define __Charge_Rank_1___2			50582  
    #define __Charge_Rank_2				6178
    #define __Charge_Rank_2___0			26184
    #define __Charge_Rank_3				11578
    #define __Charge_Rank_3___0			26185
    #define __Charge_Rank_4				26186
    #define __Charge_Rank_5				26202
    #define __Charge_Rank_6				28343
	*/
	mgr->register_spell_script(34428, &VictoryRush::Create);
    /**** Charge ****/
    mgr->register_dummy_spell(__Charge_Rank_1, &Charge); 
    mgr->register_dummy_spell(__Charge_Rank_2, &Charge); 
    mgr->register_dummy_spell(__Charge_Rank_3, &Charge); 
	mgr->register_dummy_spell(__Charge_Rank_4, &Charge); 
    mgr->register_dummy_spell(__Charge_Rank_5, &Charge); 
    mgr->register_dummy_spell(__Charge_Rank_6, &Charge);

    /**** Execute ****/
    /* log isn't working */
    /*
      WORLD: got cast spell packet, spellId - 25236, data length = 9
      Spell::cast 25236, Unit: 2
      WORLD: Spell effect id = 3, damage = 925
      WORLD: Recvd CMSG_ATTACKSWING Message
      WORLD: Sent SMSG_ATTACKSTART
      Player::Update:  No valid current selection to attack, stopping attack
    */
	/*
	#define __Execute_Rank_1				5308
	#define __Execute_Rank_2				20658
	#define __Execute_Rank_3				20660
	#define __Execute_Rank_4				20661
	#define __Execute_Rank_5				20662
	#define __Execute_Rank_6				25234
	#define __Execute_Rank_7				25236
	#define __Execute_Rank_8				47470
	#define __Execute_Rank_9				47471
	*/
    mgr->register_dummy_spell(__Execute_Rank_1, &Execute);     // Rank 1
    mgr->register_dummy_spell(__Execute_Rank_2, &Execute);    // Rank 2
    mgr->register_dummy_spell(__Execute_Rank_3, &Execute);    // Rank 3
    mgr->register_dummy_spell(__Execute_Rank_4, &Execute);    // Rank 4
    mgr->register_dummy_spell(__Execute_Rank_5, &Execute);    // Rank 5
    mgr->register_dummy_spell(__Execute_Rank_6, &Execute);    // Rank 6
    mgr->register_dummy_spell(__Execute_Rank_7, &Execute);    // Rank 7
	mgr->register_dummy_spell(__Execute_Rank_8, &Execute);    // Rank 6
    mgr->register_dummy_spell(__Execute_Rank_9, &Execute);    // Rank 7

    /**** Bloodrage ****/
    /* server debug when blood rages is cast */
    /*

    WORLD: got cast spell packet, spellId - 2687, data length = 6
    Spell::cast 2687, Unit: 2
    WORLD: Spell effect id = 30, damage = 100
    WORLD: Spell effect id = 64, damage = 1
    Spell::cast 29131, Unit: 2
    WORLD: Spell effect id = 6, damage = 10
    WORLD: Spell effect id = 6, damage = 1
    WORLD: target = 2 , Spell Aura id = 24, SpellId  = 29131, i = 0, apply = true
    WORLD: target = 2 , Spell Aura id = 94, SpellId  = 29131, i = 1, apply = true
    WORLD: target = 2 , Spell Aura id = 24, SpellId  = 29131, i = 0, apply = false
    WORLD: target = 2 , Spell Aura id = 94, SpellId  = 29131, i = 1, apply = false
    
    */
	/*
	#define __Bloodrage				2687
    #define __Bloodrage___0			29131
	*/
    mgr->register_dummy_spell(__Bloodrage    ,  &Bloodrage);    // Bloodrage
    mgr->register_dummy_spell(__Bloodrage____0, &Bloodrage);   // Bloodrage this is also called

/*
	#define __Devastate_Rank_1				20243
	#define __Devastate_Rank_2				30016
	#define __Devastate_Rank_3				30022
	#define __Devastate_Rank_4				47497
    #define __Devastate_Rank_5				47498
	*/
	mgr->register_spell_script(__Devastate_Rank_1, &Devastate::Create);
	mgr->register_spell_script(__Devastate_Rank_2, &Devastate::Create);
	mgr->register_spell_script(__Devastate_Rank_3, &Devastate::Create);
	mgr->register_spell_script(__Devastate_Rank_4, &Devastate::Create);
	mgr->register_spell_script(__Devastate_Rank_5, &Devastate::Create);
}
