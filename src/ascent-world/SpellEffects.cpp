/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentcommunity.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

// 164 Max 335.12340
pSpellEffect SpellEffectsHandler[TOTAL_SPELL_EFFECTS]=
{
		&Spell::SpellEffectNULL,                //SPELL_EFFECT_NULL - 0
		&Spell::SpellEffectInstantKill,         //SPELL_EFFECT_INSTAKILL - 1
		&Spell::SpellEffectSchoolDMG,			//SPELL_EFFECT_SCHOOL_DAMAGE - 2
		&Spell::SpellEffectDummy,				//SPELL_EFFECT_DUMMY - 3
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_PORTAL_TELEPORT - 4
		&Spell::SpellEffectTeleportUnits,		//SPELL_EFFECT_TELEPORT_UNITS - 5
		&Spell::SpellEffectApplyAura,			//SPELL_EFFECT_APPLY_AURA - 6
		&Spell::SpellEffectEnvironmentalDamage, //SPELL_EFFECT_ENVIRONMENTAL_DAMAGE - 7
		&Spell::SpellEffectPowerDrain,			//SPELL_EFFECT_POWER_DRAIN - 8
		&Spell::SpellEffectHealthLeech,			//SPELL_EFFECT_HEALTH_LEECH - 9
		&Spell::SpellEffectHeal,				//SPELL_EFFECT_HEAL - 10
		&Spell::SpellEffectBind,				//SPELL_EFFECT_BIND - 11
		&Spell::SpellEffectTeleportUnits,		//SPELL_EFFECT_PORTAL - 12 (27)(Goldshire Portal) (13912)(Princess Summons Portal) 
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_RITUAL_BASE - 13
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_RITUAL_SPECIALIZE - 14
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_RITUAL_ACTIVATE_PORTAL - 15
		&Spell::SpellEffectQuestComplete,		//SPELL_EFFECT_QUEST_COMPLETE - 16
		&Spell::SpellEffectWeapondamageNoschool,//SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL - 17
		&Spell::SpellEffectResurrect,			//SPELL_EFFECT_RESURRECT - 18
		&Spell::SpellEffectAddExtraAttacks,		//SPELL_EFFECT_ADD_EXTRA_ATTACKS - 19
		&Spell::SpellEffectDodge,				//SPELL_EFFECT_DODGE - 20
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_EVADE - 21 (108)(Evade (DND)) 
		&Spell::SpellEffectParry,				//SPELL_EFFECT_PARRY - 22
		&Spell::SpellEffectBlock,				//SPELL_EFFECT_BLOCK - 23
		&Spell::SpellEffectCreateItem,			//SPELL_EFFECT_CREATE_ITEM - 24
		&Spell::SpellEffectWeapon,				//SPELL_EFFECT_WEAPON - 25
		&Spell::SpellEffectDefense,				//SPELL_EFFECT_DEFENSE - 26
		&Spell::SpellEffectPersistentAA,		//SPELL_EFFECT_PERSISTENT_AREA_AURA - 27
		&Spell::SpellEffectSummon,				//SPELL_EFFECT_SUMMON - 28
		&Spell::SpellEffectLeap,				//SPELL_EFFECT_LEAP - 29
		&Spell::SpellEffectEnergize,			//SPELL_EFFECT_ENERGIZE - 30
		&Spell::SpellEffectWeaponDmgPerc,		//SPELL_EFFECT_WEAPON_PERCENT_DAMAGE - 31
		&Spell::SpellEffectTriggerMissile,		//SPELL_EFFECT_TRIGGER_MISSILE - 32
		&Spell::SpellEffectOpenLock,			//SPELL_EFFECT_OPEN_LOCK - 33
		&Spell::SpellEffectTranformItem,		//SPELL_EFFECT_TRANSFORM_ITEM - 34
		&Spell::SpellEffectApplyAA,				//SPELL_EFFECT_APPLY_AREA_AURA - 35
		&Spell::SpellEffectLearnSpell,			//SPELL_EFFECT_LEARN_SPELL - 36
		&Spell::SpellEffectSpellDefense,		//SPELL_EFFECT_SPELL_DEFENSE - 37
		&Spell::SpellEffectDispel,				//SPELL_EFFECT_DISPEL - 38
		&Spell::SpellEffectLanguage,			//SPELL_EFFECT_LANGUAGE - 39 
		&Spell::SpellEffectDualWield,			//SPELL_EFFECT_DUAL_WIELD - 40
		&Spell::SpellEffectJumpMove,			//SPELL_EFFECT_JUMP - 41
		&Spell::SpellEffectJumpMove,            //SPELL_EFFECT_JUMP_MOVE - 42 Ascent5218
		&Spell::SpellEffectTeleportUnitsFaceCaster,	//SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER  43 MegaBigBoss
		&Spell::SpellEffectSkillStep,			//SPELL_EFFECT_SKILL_STEP - 44
		&Spell::SpellEffectAddHonor,			//SPELL_ADD_HONOR - 45
		&Spell::SpellEffectSpawn,				//SPELL_EFFECT_SPAWN - 46
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_TRADE_SKILL - 47 Metiers (Branruz)
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_STEALTH - 48
		&Spell::SpellEffectDetect,				//SPELL_EFFECT_DETECT - 49
		&Spell::SpellEffectSummonObject,		//SPELL_EFFECT_SUMMON_OBJECT - 50
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_FORCE_CRITICAL_HIT - 51 NA
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_GUARANTEE_HIT - 52 NA
		&Spell::SpellEffectEnchantItem,			//SPELL_EFFECT_ENCHANT_ITEM - 53
		&Spell::SpellEffectEnchantItemTemporary,//SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY - 54
		&Spell::SpellEffectTameCreature,		//SPELL_EFFECT_TAMECREATURE - 55
		&Spell::SpellEffectSummonPet,			//SPELL_EFFECT_SUMMON_PET	- 56
		&Spell::SpellEffectLearnPetSpell,		//SPELL_EFFECT_LEARN_PET_SPELL - 57
		&Spell::SpellEffectWeapondamage,		//SPELL_EFFECT_WEAPON_DAMAGE - 58
		&Spell::SpellEffectOpenLockItem,		//SPELL_EFFECT_OPEN_LOCK_ITEM - 59
		&Spell::SpellEffectProficiency,			//SPELL_EFFECT_PROFICIENCY - 60
		&Spell::SpellEffectSendEvent,			//SPELL_EFFECT_SEND_EVENT - 61
		&Spell::SpellEffectPowerBurn,			//SPELL_EFFECT_POWER_BURN - 62
		&Spell::SpellEffectThreat,				//SPELL_EFFECT_THREAT - 63
		&Spell::SpellEffectTriggerSpell,		//SPELL_EFFECT_TRIGGER_SPELL - 64
		&Spell::SpellEffectApplyRaidAA,			//SPELL_EFFECT_RAID_AREA_AURA - 65
		//&Spell::SpellEffectHealthFunnel,		//SPELL_EFFECT_HEALTH_FUNNEL - 65
		&Spell::SpellEffectPowerFunnel,			//SPELL_EFFECT_POWER_FUNNEL - 66
		&Spell::SpellEffectHealMaxHealth,		//SPELL_EFFECT_HEAL_MAX_HEALTH - 67
		&Spell::SpellEffectInterruptCast,		//SPELL_EFFECT_INTERRUPT_CAST - 68
		&Spell::SpellEffectDistract,			//SPELL_EFFECT_DISTRACT - 69
		&Spell::SpellEffectPull,				//SPELL_EFFECT_PULL - 70 (15051)(Distract Move) 
		&Spell::SpellEffectPickpocket,			//SPELL_EFFECT_PICKPOCKET - 71
		&Spell::SpellEffectAddFarsight,			//SPELL_EFFECT_ADD_FARSIGHT - 72
		&Spell::SpellEffectPull,				//SPELL_EFFECT_SUMMON_POSSESSED - 73 (46331)(Trainer: Untrain Talents)
		&Spell::SpellEffectApplyGlyph,			//SPELL_EFFECT_APPLY_GLYPH - 74
		&Spell::SpellEffectHealMechanical,		//SPELL_EFFECT_HEAL_MECHANICAL - 75
		&Spell::SpellEffectSummonObjectWild,	//SPELL_EFFECT_SUMMON_OBJECT_WILD - 76
		&Spell::SpellEffectScriptEffect,		//SPELL_EFFECT_SCRIPT_EFFECT - 77
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_ATTACK - 78
		&Spell::SpellEffectSanctuary,			//SPELL_EFFECT_SANCTUARY - 79
		&Spell::SpellEffectAddComboPoints,		//SPELL_EFFECT_ADD_COMBO_POINTS - 80
		&Spell::SpellEffectCreateHouse,			//SPELL_EFFECT_CREATE_HOUSE - 81
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_BIND_SIGHT - 82 (6955)(Gnome Car Camera) 
		&Spell::SpellEffectDuel,				//SPELL_EFFECT_DUEL - 83
		&Spell::SpellEffectStuck,				//SPELL_EFFECT_STUCK - 84
		&Spell::SpellEffectSummonPlayer,		//SPELL_EFFECT_SUMMON_PLAYER - 85
		&Spell::SpellEffectActivateObject,		//SPELL_EFFECT_ACTIVATE_OBJECT - 86
		&Spell::SpellEffectWMODamage,			//SPELL_EFFECT_WMO_DAMAGE - 87
		&Spell::SpellEffectWMORepair,			//SPELL_EFFECT_WMO_REPAIR - 88
		&Spell::SpellEffectChangeWMOState,		//SPELL_EFFECT_WMO_CHANGE - 89
		&Spell::SpellEffectKillCredit,			//SPELL_EFFECT_KILL_CREDIT - 90 Kill credit but only for single person
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_THREAT_ALL - 91
		&Spell::SpellEffectEnchantHeldItem,		//SPELL_EFFECT_ENCHANT_HELD_ITEM - 92
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_SUMMON_PHANTASM - 93 OLD
		&Spell::SpellEffectSelfResurrect,		//SPELL_EFFECT_SELF_RESURRECT - 94
		&Spell::SpellEffectSkinning,			//SPELL_EFFECT_SKINNING - 95
		&Spell::SpellEffectCharge,				//SPELL_EFFECT_CHARGE - 96
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_SUMMON_CRITTER - 97
		&Spell::SpellEffectKnockBack,			//SPELL_EFFECT_KNOCK_BACK - 98
		&Spell::SpellEffectDisenchant,			//SPELL_EFFECT_DISENCHANT - 99
		&Spell::SpellEffectInebriate,			//SPELL_EFFECT_INEBRIATE - 100
		&Spell::SpellEffectFeedPet,				//SPELL_EFFECT_FEED_PET - 101
		&Spell::SpellEffectDismissPet,			//SPELL_EFFECT_DISMISS_PET - 102
		&Spell::SpellEffectReputation,			//SPELL_EFFECT_REPUTATION - 103
		&Spell::SpellEffectSummonObjectSlot,	//SPELL_EFFECT_SUMMON_OBJECT_SLOT1 - 104
		&Spell::SpellEffectSummonObjectSlot,	//SPELL_EFFECT_SUMMON_OBJECT_SLOT2 - 105
		&Spell::SpellEffectSummonObjectSlot,	//SPELL_EFFECT_SUMMON_OBJECT_SLOT3 - 106
		&Spell::SpellEffectSummonObjectSlot,	//SPELL_EFFECT_SUMMON_OBJECT_SLOT4 - 107
		&Spell::SpellEffectDispelMechanic,		//SPELL_EFFECT_DISPEL_MECHANIC - 108
		&Spell::SpellEffectSummonDeadPet,		//SPELL_EFFECT_SUMMON_DEAD_PET - 109
		&Spell::SpellEffectDestroyAllTotems,	//SPELL_EFFECT_DESTROY_ALL_TOTEMS - 110
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_DURABILITY_DAMAGE - 111
		&Spell::SpellEffectSummonDemon,			//SPELL_EFFECT_SUMMON_DEMON - 112
		&Spell::SpellEffectResurrectNew,		//SPELL_EFFECT_RESURRECT_NEW - 113
		&Spell::SpellEffectAttackMe,			//SPELL_EFFECT_ATTACK_ME - 114
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_DURABILITY_DAMAGE_PCT - 115
		&Spell::SpellEffectSkinPlayerCorpse,	//SPELL_EFFECT_SKIN_PLAYER_CORPSE - 116
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_SPIRIT_HEAL - 117//(22012)(Spirit Heal) 
		&Spell::SpellEffectSkill,				//SPELL_EFFECT_SKILL - 118
		&Spell::SpellEffectApplyPetAura,		//SPELL_EFFECT_APPLY_PET_AURA - 119
		&Spell::SpellEffectNULL,				//SPELL_EFFECT_TELEPORT_GRAVEYARD - 120//(24253)(Graveyard Teleport Test) 
		&Spell::SpellEffectDummyMelee,			//SPELL_EFFECT_DUMMYMELEE	- 121
		&Spell::SpellEffectNULL,				//unknown - 122 //not used
		&Spell::SpellEffectSendTaxi,			//123 SPELL_EFFECT_SEND_TAXI  taxi/flight related (misc value is taxi path id)
		&Spell::SpellEffectPlayerPull,			//SPELL_EFFECT_PLAYER_PULL - 124 - http://thottbot.com/e2312
		&Spell::SpellEffectNULL,				//unknown - 125 // Reduce Threat by % //http://www.thottbot.com/?sp=32835
		&Spell::SpellEffectSpellSteal,			//SPELL_EFFECT_SPELL_STEAL - 126 // Steal Beneficial Buff (Magic) //http://www.thottbot.com/?sp=30449
		&Spell::SpellEffectProspecting,			//unknown - 127 // Search 5 ore of a base metal for precious gems.  This will destroy the ore in the process.
		&Spell::SpellEffectApplyAura128,		//unknown - 128 // Adjust a stats by %: Mod Stat // ITS FLAT
		&Spell::SpellEffectApplyAura128,		// unknown - 129 // Mod Dmg % (Spells) Aura ?? (Branruz)
		&Spell::SpellEffectRedirectThreat,		// unknown - 130 // redirect threat
		&Spell::SpellEffectNULL,				// unknown - 131 // test spell
		&Spell::SpellEffectPlayMusic,			// unknown - 132 // play music
		&Spell::SpellEffectForgetSpecialization,// SPELL_EFFECT_FORGET_SPECIALIZATION - 133 // http://www.thottbot.com/s36441 // I think this is a gm/npc spell
		&Spell::SpellEffectKillCredit,			// unknown - 134 // give quest credit
		&Spell::SpellEffectNULL,				// unknown - 135 // npc's call pet o.o (23498)(Call Pet) 	135
		&Spell::SpellEffectRestoreHealthPct,	// unknown - 136 // restore health pct
		&Spell::SpellEffectRestoreManaPct,		// unknown - 137 // restore mana pct
		&Spell::SpellEffectDisengage,			// 138 - knockback backwards :3
		&Spell::SpellEffectClearFinishedQuest,	// unknown - 139 // clear quest from finished o.o
		&Spell::SpellEffectTeleportUnits,		// SPELL_EFFECT_TELEPORT_UNITS -140 -
		&Spell::SpellEffectTriggerSpell,		// unknown - 141 // triggers spell, magic one,  (Mother spell) http://www.thottbot.com/s41065		
		&Spell::SpellEffectTriggerSpellWithValue,// SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE - 142 // triggers some kind of "Put spell on target" thing... (dono for sure) http://www.thottbot.com/s40872 and http://www.thottbot.com/s33076
		&Spell::SpellEffectApplyDemonAura,		// 143 - Apply pet aura for summons :3
		// AscentWaad
		&Spell::SpellEffectKnockBack,			// 144 - Spectral Blast 
		&Spell::SpellEffectPull,				// SPELL_EFFECT_TRACTOR_BEAM_FROM_DEST - 145 
		&Spell::SpellEffectActivateRune,		// 146 - Finish a rune's cooldown
		&Spell::SpellEffectFailQuest,			// 147 // (46070)(Torch Tossing Training Failure) 
		&Spell::SpellEffectTriggerSpell,		// 148 // (43509)(Orb of Fire) 
		&Spell::SpellEffectCharge,				// SPELL_EFFECT_CHARGE2 - 149 - // (44348)(Rush/charge) 
		&Spell::SpellEffectStartQuest,			// SPELL_EFFECT_START_QUEST - 150 -
		&Spell::SpellEffectTriggerSpell,		// 151 // (698)(Ritual of Summoning)  SPELL_EFFECT_TRIGGER_SPELL_2 (HearthStone)
		&Spell::SpellEffectSummonPlayer,		// 152 // (45927)(Summon Friend) 
		&Spell::SpellEffectCreatePet,			// 153 // Pet summon ?? (du spell 45927 à 46730)
		&Spell::SpellEffectNULL,				// 154 // not used
		&Spell::SpellEffectTitanGrip,			// 155 // add socket / Titan's Grip (HearthStone)
		&Spell::SpellEffectAddPrismaticSocket,	// 156 - Add Socket TODO : AddPrismaticSocket.
		&Spell::SpellEffectCreateRandomItem,	// 157 // create item (inscription)  (HearthStone)
		&Spell::SpellEffectMilling,	            // 158 // milling (HearthStone)
		&Spell::SpellEffectAllowPetRename,		// 159 // allow hunter pet rename
		&Spell::SpellEffectTriggerSpell,		// 160 - Only 1 spell has this and it has a trigger spell so fuck it!
		&Spell::SpellEffectLearnSpec,			// 161 - Relatif à la Double Spé (Apprendre/Oublié) (BB)
		&Spell::SpellEffectActivateTalentSpec,	// 162 - Relatif a la Double Spé (Activer Primaire/Secondaire) (BB)
		&Spell::SpellEffectNULL,				// 163 - not used
		&Spell::SpellEffectRemoveAura,			// 164 - Remove Aura.		
		&Spell::SpellEffectNULL,				// 165 - Reserve Patch 339
		&Spell::SpellEffectNULL,				// 166 - Reserve Patch 339
		&Spell::SpellEffectNULL,				// 167 - Reserve Patch 339
		&Spell::SpellEffectNULL,				// 168 - Reserve Patch 339
		&Spell::SpellEffectNULL,				// 169 - Reserve Patch 339
};

// A appeler juste apres avoir charger le spell.dbc
// Pas encore utiliser, je charge les valeurs dans le SpellFixes.cpp
void Spell::LoadSpellEffect(void)
{
 int cmpt_SpellEffect = 0;
 SpellEntry *_spell;
 QueryResult *result = WorldDatabase.Query("SELECT * FROM spells_effects");
 if(result)
 {
  uint32 entry;
  do 
  {
   entry   = result->Fetch()[0].GetUInt32();
   // FIXME: Le probleme est l'alignement des spelleffect en fonction de l'entry
   //        Comment remplir d'une maniere rapide la structure des spells a partir du DBC
   //        ou a partir de la base, NB: ya plus de 42000 spells
   _spell = dbcSpell.LookupEntry(entry);
   if(_spell)
   {
	cmpt_SpellEffect++;
    _spell->Effect[0] = result->Fetch()[1].GetUInt32();
    _spell->Effect[1] = result->Fetch()[2].GetUInt32();
    _spell->Effect[2] = result->Fetch()[3].GetUInt32();
   }
   else
   {
	   Log.Error("SpellEffect","Le spell %u n'existe plus dans spell.dbc!",entry);
   }
   
  } while(result->NextRow());
  Log.Notice("Spell", "%u Spells Effects loaded", cmpt_SpellEffect);
  delete result;
 }
 else Log.Error("Spell", "Table 'Spells_Effects' non trouvee, mettez a jour votre base World!");

}


void Spell::SpellEffectNULL(uint32 i)
{
	sLog.outDebug("Unhandled spell effect %u in spell %u.\n",m_spellInfo->Effect[i],m_spellInfo->Id);
}

void Spell::SpellEffectInstantKill(uint32 i)
{
	if(!unitTarget || !unitTarget->isAlive())
		return;

	//Sacrifice: if spell caster has "void walker" pet, pet dies and spell caster gets a 
	/*Sacrifices the Voidwalker, giving its owner a shield that will absorb 
	305 damage for 30 sec. While the shield holds, spellcasting will not be \
	interrupted by damage.*/

	/*
	Demonic Sacrifice

	When activated, sacrifices your summoned demon to grant you an effect that lasts 
	30 minutes. The effect is canceled if any Demon is summoned. 
	Imp: Increases your Fire damage by 15%. 
	Voidwalker: Restores 3% of total Health every 4 sec. 
	Succubus: Increases your Shadow damage by 15%. 
	Felhunter: Restores 2% of total Mana every 4 sec.

	When activated, sacrifices your summoned demon to grant you an effect that lasts $18789d.  The effect is canceled if any Demon is summoned.

	Imp: Increases your Fire damage by $18789s1%.

	Voidwalker: Restores $18790s1% of total Health every $18790t1 sec.

	Succubus: Increases your Shadow damage by $18791s1%.

	Felhunter: Restores $18792s1% of total Mana every $18792t1 sec.

	*/
	uint32 spellId = m_spellInfo->Id;

	switch(spellId)
	{                                // __Goblin_Bomb ??
	case 3617://Goblin Bomb Suicide // N'existe plus 3.0.9 (Branruz)
		{
			if(m_caster->GetTypeId() != TYPEID_UNIT)
				break;
			Unit *caster = m_caster->GetMapMgr()->GetPlayer(m_caster->GetUInt32Value(UNIT_FIELD_SUMMONEDBY));
			caster->summonPet->RemoveFromWorld(false,true);
			delete caster->summonPet;
			caster->summonPet = NULL;
		}break;
		// __Lash_of_Pain				15968
	 //case __Lash_of_Pain___0: //				21987
	 //case __Lash_of_Pain___1: //				32202
	 //case __Lash_of_Pain___2: //				36864
	 //case __Lash_of_Pain___3: //				38852
	 //case __Lash_of_Pain___4: //				41353
	 //case __Lash_of_Pain___5: //				44640
	 case __Lash_of_Pain_Rank_1: //				7814
	 //case __Lash_of_Pain_Rank_1___0: //				7876
	 case __Lash_of_Pain_Rank_2	: //			7815
	 //case __Lash_of_Pain_Rank_2___0: //				7877
	 //case __Lash_of_Pain_Rank_2___1: //				20398
	 case __Lash_of_Pain_Rank_3	: //			7816
	 //case __Lash_of_Pain_Rank_3___0: //				7878
	 //case __Lash_of_Pain_Rank_3___1: //				20399
	 case __Lash_of_Pain_Rank_4	: //			11778
	 //case __Lash_of_Pain_Rank_4___0	: //			11781
	 //case __Lash_of_Pain_Rank_4___1	: //			20400
	 case __Lash_of_Pain_Rank_5		: //		11779
	 //case __Lash_of_Pain_Rank_5___0	: //			11782
	 //case __Lash_of_Pain_Rank_5___1	: //			20401
	 case __Lash_of_Pain_Rank_6		: //		11780
	 //case __Lash_of_Pain_Rank_6___0	: //			11783
	 //case __Lash_of_Pain_Rank_6___1	: //			20402
	 case __Lash_of_Pain_Rank_7		: //		27274
	 //case __Lash_of_Pain_Rank_7___0	: //			27493
	 case __Lash_of_Pain_Rank_8		: //		47991
	 case __Lash_of_Pain_Rank_9		: //		47992
		{
		}break;

	 case __Encapsulate_Voidwalker: //29364:	// encapsulate voidwalker
		{
			return;
		}break;
	 case __Demonic_Sacrifice: // 18788: //Demonic Sacrifice (508745)
		uint32 spellid1 = 0;
		switch(unitTarget->GetEntry())
		{
			case 416: //Imp
			{   
				spellid1 = 18789; // __Burning_Wish
		
			}break;
			case 417: //Felhunter
			{
				spellid1 = 18792; // __Fel_Energy
		
			}break;
			case 1860: //VoidWalker
			{
				spellid1 = 18790; // __Fel_Stamina
			}break;
			case 1863: //Succubus
			{
				spellid1 = 18791; // __Touch_of_Shadow
			}break;
			case 17252: //felguard
			{
				spellid1 = 35701; // __Touch_of_Shadow___0
			}break;
		}
		//now caster gains this buff
		if (spellid1 && spellid1 != 0)
		{
			((Unit *)m_caster)->CastSpell(m_caster, dbcSpell.LookupEntry(spellid1), true);
		}
	}

	switch( m_spellInfo->NameHash ) // 1050
	{
		/* SPELL_HASH_SACRIFICE
		Coût	Aucun
		Portée	0 mètres (Soi-même)
		Incantation	1 seconde
		Recharge	6 secondes
		Effet #1	School Damage (Sacré)
		Valeur : 1388 à 1612
		Rayon : 20 mètres
		Effet #2	School Damage (Sacré)
		Valeur : 1850 à 2150
		*/
	case SPELL_HASH_SACRIFICE:
		{
			if( !m_caster->IsPet() ) return;

			static_cast<Pet*>(m_caster)->Dismiss( true );
			return;

		}break;
			/*
		Coût	Aucun
		Portée	50000 mètres (Partout)
		Incantation	Instantanée
		Recharge	n/d
		Effet	Instakill
		*/
	case SPELL_HASH_DEATH_TOUCH : // 5 - Report TheLycan
		{ // Appelé par le spell 13716 __AOD (SPELL_HASH_AOD) (Proc Trigger Spell)
          // Tue la cible instantanement
		}break;
	default:
		{
			// moar cheaters
			/*
			if( p_caster == NULL || (u_caster != NULL && u_caster->IsPet() ) )
				return;
            
			if( p_caster->GetSession()->HasPermissions() == 0 )
				return;
			*/	
			Player *p = m_caster->GetPlayerFrom();
			if(p != NULL)
			{
				WorldSession* s = p->GetSession(); 
				if (s)
					s->SystemMessage("The spell %s (%u) is not yet implemented, please report to devs.", m_spellInfo->Name, m_spellInfo->Id);
			}
		}
	}
	//instant kill effects don't have a log
	//m_caster->SpellNonMeleeDamageLog(unitTarget, m_spellInfo->Id, unitTarget->GetUInt32Value(UNIT_FIELD_HEALTH), true);
	m_caster->DealDamage(unitTarget, unitTarget->GetUInt32Value(UNIT_FIELD_HEALTH), 0, 0, 0);
}

void Spell::SpellEffectSchoolDMG(uint32 i) // dmg school
{
	bool static_damage=false;

	// UnitTarget ne suffit pas, il faut controler les sors de zones
	// attributesEx & ATTRIBUTESEX_AREA_OF_EFFECT donne le type de spell
	// EffectImplicitTargetA[0] == EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT
	// EffectImplicitTargetA[1] == EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED

	if(!unitTarget || !unitTarget->isAlive())
	{
		//Log.Warning("SpellEffectSchoolDMG","Pas de Unit");
		return;
	}

	 // Debug
	/*Log.Warning("SpellEffectSchoolDMG","Begin, Spell %u",this->GetSpellProto()->Id);

	if(unitTarget->GetObjectName() != NULL)
	 Log.Warning("SpellEffectSchoolDMG","%s (%u) se prend School Damage",unitTarget->GetObjectName(),unitTarget->GetEntry());
	else
		Log.Warning("SpellEffectSchoolDMG","%u se prend School Damage",unitTarget->GetEntry());


	if(m_caster != NULL) Log.Warning("SpellEffectSchoolDMG","m_caster : %u",m_caster->GetEntry());
	else                 Log.Warning("SpellEffectSchoolDMG","m_caster NULL");
	if(u_caster != NULL) Log.Warning("SpellEffectSchoolDMG","u_caster : %u",u_caster->GetEntry());
	else                 Log.Warning("SpellEffectSchoolDMG","u_caster NULL");
	if(p_caster != NULL) Log.Warning("SpellEffectSchoolDMG","p_caster : %u",p_caster->GetEntry());
	else                 Log.Warning("SpellEffectSchoolDMG","p_caster NULL");
	if(i_caster != NULL) Log.Warning("SpellEffectSchoolDMG","i_caster : %u",i_caster->GetEntry());
	else                 Log.Warning("SpellEffectSchoolDMG","i_caster NULL");
	if(g_caster != NULL) Log.Warning("SpellEffectSchoolDMG","g_caster : %u",g_caster->GetEntry());
	else                 Log.Warning("SpellEffectSchoolDMG","g_caster NULL");
    */

	uint32 spellschool = GetBestSchoolForSpell(unitTarget);

	if(unitTarget->SchoolImmunityList[spellschool])
	{
		/*
		if(unitTarget->GetObjectName() != NULL)
		   Log.Warning("SpellEffectSchoolDMG","%s (%u) est immunise, echec",unitTarget->GetObjectName(),unitTarget->GetEntry());
		else
			Log.Warning("SpellEffectSchoolDMG","La cible %u est immunise, echec",unitTarget->GetEntry());
		*/

		SendCastResult(SPELL_FAILED_IMMUNE);
		return;
	}

	//normal spells have 1.0 as amptitude, special spells that can chain have their correct amptitude
   
	uint32 dmg = float2int32(damage * amp);

	float reduce = m_spellInfo->effectChainAmplitude[i] * 100.0f;
		
	if(m_caster->IsUnit())
	{
	 if(m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2])
	 {
		SM_FFValue(((Unit *)m_caster)->SM_PJumpReduce, &reduce, m_spellInfo);
	 }
	}
	
	amp = amp * reduce / 100;

	//damage is limited to the MIN damage of 15 targets, if theres more then 10 targets
	//dont apply to chain spells

	// SPELLPARSER ICI
	if (m_spellInfo->EffectChainTarget == 0 && dmg * m_orderedObjects.size() > ((uint32)m_spellInfo->EffectBasePoints[i] + 1) * 15)
		dmg = (uint32)((float)(m_spellInfo->EffectBasePoints[i] + 1) * 15.0 / (float)m_orderedObjects.size());

	switch(m_spellInfo->NameHash)
	{
	 case SPELL_HASH_ARCANE_SHOT: //hunter - arcane shot
		{
			if(m_caster->IsUnit()) dmg += float2int32(float(((Unit *)m_caster)->GetRAP())*0.15f);

			dmg *= float2int32( 0.9f + RandomFloat( 0.2f ) ); // randomised damage
		}break;
	 case SPELL_HASH_GORE: // boar/ravager: Gore (50% chance of double damage)
		{
			dmg *= Rand( 50 ) ? 2 : 1;
		}
		break;
	 case SPELL_HASH_BLOODTHIRST: // 0xCBC738B8:	// Bloodthirst
		{
               dmg = ((Unit *)m_caster)->GetAP()*(m_spellInfo->EffectBasePoints[0]+1) / 100;
		}break;
	 case SPELL_HASH_SHIELD_SLAM: //2189817683UL:	// Shield Slam - damage is increased by block value
		{
			if( m_caster->IsPlayer() )
			{
				Item *it = ((Player *)m_caster)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
				if( it && it->GetProto()->InventoryType == INVTYPE_SHIELD )
				{
					float block_multiplier = ( 100.0f + float( ((Player *)m_caster)->m_modblockabsorbvalue ) ) / 100.0f;
					if( block_multiplier < 1.0f )block_multiplier = 1.0f;
					int32 blockable_damage = float2int32( float( it->GetProto()->Block ) +
						                     ( float( ((Player *)m_caster)->m_modblockvaluefromspells + 
											 ((Player *)m_caster)->GetUInt32Value( PLAYER_RATING_MODIFIER_BLOCK ) ) * block_multiplier ) + 
											 ( ( float( ((Player *)m_caster)->GetUInt32Value( UNIT_FIELD_STAT0 ) ) / 20.0f ) - 1.0f ) );
					dmg += blockable_damage;
				}
			}
		}break;

	 case SPELL_HASH_MOLTEN_ARMOR: // 0xf79e1873: // fire armor, is static damage
		static_damage = true;
		break;
	}

	// check for no more damage left (chains)
	if(!dmg) 
	{
		//Log.Warning("SpellEffectSchoolDMG","Pas de dommage sur la cible, echec");
		SendCastResult(SPELL_FAILED_IMMUNE); // "Insensible"
		return;
	}
	/**************************************************************************
	* This handles the correct damage of "Judgement of Command" (all ranks)
	**************************************************************************/
	if (m_spellInfo->NameHash == SPELL_HASH_JUDGEMENT_OF_COMMAND && !unitTarget->IsStunned())
			dmg = dmg >> 1;

	if(m_spellInfo->speed > 0)
	{
		//Log.Warning("SpellEffectSchoolDMG","En deplacement");
		m_caster->SpellNonMeleeDamageLog(unitTarget,m_spellInfo->Id, dmg, pSpellId==0, false, false, spellschool);
	}
	else
	{
	 Log.Warning("SpellEffectSchoolDMG","Defense type : %u",GetType());
	 if( GetType() == DEFENSE_TYPE_MAGIC )		
	 {
		//Log.Warning("SpellEffectSchoolDMG","DEFENSE_TYPE_MAGIC");
		m_caster->SpellNonMeleeDamageLog( unitTarget, m_spellInfo->Id, dmg, pSpellId == 0, static_damage, false, spellschool );
	 }
	 else 
	 {
		if( m_caster->IsUnit() ) 
		{
			//Log.Warning("SpellEffectSchoolDMG","u_caster : ",u_caster->GetEntry());
			uint32 _type;
			if( GetType() == DEFENSE_TYPE_RANGED )
				_type = RANGED;
			else
			{
				if (m_spellInfo->attributesExC & ATTRIBUTESEXC_OFFHAND) // 0x01000000
					_type =  OFFHAND;
				else
					_type = MELEE;
			}

			//Log.Warning("SpellEffectSchoolDMG","%u fait un Strike",u_caster->GetEntry());
			((Unit *)m_caster)->Strike( unitTarget, _type, m_spellInfo, 0, 0, dmg, pSpellId == 0, true );
		}
		else Log.Error("SpellEffectSchoolDMG","m_caster NULL");
	 }
	}  

	//Log.Warning("SpellEffectSchoolDMG","End");
}

void Spell::SpellEffectDummy(uint32 i) // Dummy(Scripted events)
{
	uint32 spellId = m_spellInfo->Id;
	uint32 spellHashes = m_spellInfo->NameHash;

	if (m_spellScript != NULL)
		m_spellScript->DummyEffect(i);
	
	// Try a dummy SpellHandler
	if(sScriptMgr.CallScriptedDummySpell(spellId, i, this))
		return;

	switch(spellHashes) 
	{
	/*****************************************
	 *	Class Spells
	 *****************************************/	

	/*************************
	 * MAGE SPELLS
	 *************************
	 * IDs:
	 *	11189 Frost Warding   -	RANK 1,		STATUS: DONE
	 *  28332 Frost Warding   -	RANK 2,		STATUS: DONE
	 *  11094 Molten Shields  -	RANK 1,		STATUS: DONE
	 *  13043 Molten Shields  -	RANK 2,		STATUS: DONE

	 *  --------------------------------------------
	 *************************/
	
	/*
		Frost Warding
		Increases the armor and resistances given by your Frost Armor and Ice Armor spells by X%.  In addition, gives your Frost Ward a X% chance to reflect Frost spells and effects while active.
		
		Effect #1	Apply Aura: Add % Modifier (8)

		Effect #2	Apply Aura: Dummy
		11189, 28332
	*/
	    //case __Frost_Warding_Rank_1 : // 11189:
	    //case __Frost_Warding_Rank_2 : // 28332:
	    case SPELL_HASH_FROST_WARDING :
		{
			if(!unitTarget) break;

			for(std::list<struct ReflectSpellSchool*>::iterator i = unitTarget->m_reflectSpellSchool.begin(), i2;i != unitTarget->m_reflectSpellSchool.end();)
				if(m_spellInfo->Id == (*i)->spellId)
				{
					i2 = i++;
					unitTarget->m_reflectSpellSchool.erase(i2);
				}
				else
					++i;

			ReflectSpellSchool *rss = new ReflectSpellSchool;
			rss->chance = m_spellInfo->procChance;
			rss->spellId = m_spellInfo->Id;
			rss->require_aura_hash = SPELL_HASH_FROST_WARD; 
			rss->school = SCHOOL_MASK_FROST;		

			unitTarget->m_reflectSpellSchool.push_back(rss);
		}break;
	/*
		Molten Shields	Rank 2
		Causes your Fire Ward to have a 20% chance to reflect Fire spells while active. In addition, your Molten Armor has a 100% chance to affect ranged and spell attacks.
		Effect #1	Apply Aura: Dummy
		11904,13043
	*/
	    //case __Molten_Shields_Rank_1: // 11904:
	    //case __Molten_Shields_Rank_2: //13043:
	    case SPELL_HASH_MOLTEN_SHIELDS:
		{
			if(!unitTarget) break;

			for(std::list<struct ReflectSpellSchool*>::iterator i = unitTarget->m_reflectSpellSchool.begin(), i2;i != unitTarget->m_reflectSpellSchool.end();)
				if(m_spellInfo->Id == (*i)->spellId)
				{
					i2 = i++;
					unitTarget->m_reflectSpellSchool.erase(i2);
				}
				else
					++i;

			ReflectSpellSchool *rss = new ReflectSpellSchool;
			rss->chance = m_spellInfo->EffectBasePoints[0];
			rss->spellId = m_spellInfo->Id;
			rss->require_aura_hash = SPELL_HASH_FIRE_WARD; 
			rss->school = SCHOOL_MASK_FIRE;		

			unitTarget->m_reflectSpellSchool.push_back(rss);
		}break;

	/*************************
	 * WARRIOR SPELLS
	 *************************
	 * IDs:
	 * NO SPELLS
	 *  --------------------------------------------
	 *************************/

	/*************************
	 * ROGUE SPELLS
	 *************************
	 * IDs:
	 * 14185 Preparation		STATUS: DONE
	 *  --------------------------------------------
	 * 35729 Cloak of Shadows	STATUS: DONE
	 *  --------------------------------------------
	 *************************/

	/*
		Preparation
		When activated, this ability immediately finishes the cooldown on your Evasion, Sprint, Vanish, Cold Blood, Shadowstep and Premeditation abilities.		
		
		Effect	Dummy
	*/
	    //case __Preparation: // 14185:
	    case SPELL_HASH_PREPARATION:
		{
			if( !m_caster->IsPlayer() ) return;

			// Incomplet, Verifier les Rankings en 335.12340
			uint32 ClearSpellId[11] =
			{
			5277,  /* Evasion - Rank 1 */
			26669, /* Evasion - Rank 2 */
			2983,  /* Sprint  - Rank 1 */
			8696,  /* Sprint  - Rank 2 */
			11305, /* Sprint  - Rank 3 */
			1856,  /* Vanish  - Rank 1 */
			1857,  /* Vanish  - Rank 2 */
			26889, /* Vanish  - Rank 3 */
			14177, /* Cold Blood       */
			14183, /* Premeditation    */
			36554  /* Shadowstep       */
			};

			for(i = 0; i < 11; ++i)
			{
				if( ((Player *)m_caster)->HasSpell( ClearSpellId[i] ) )
					((Player *)m_caster)->ClearCooldownForSpell( ClearSpellId[i] );
			}
		}break;
	/*
		Cloak of Shadows
		Instantly removes all existing harmful spell effects and increases your chance to resist all spells by 90% for 5 sec.  Does not remove effects that prevent you from using Cloak of Shadows.
		
		Effect #1	Apply Aura: Mod Attacker Spell Hit Chance (126)
			Value: -90
		Effect #2	Trigger Spell
			Spell #35729 <--- THIS SPELL <<<--- N'existe plus 335.12340
            __Cloak_of_Shadows				31224
	*/
		case SPELL_HASH_CLOAK_OF_SHADOWS:
     	{
			if( !unitTarget || !unitTarget->isAlive())
				return;

			Aura * pAura;
			for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; ++i)
			{
				pAura = unitTarget->m_auras[i];
				if( pAura != NULL && !pAura->IsPassive() && !pAura->IsPositive() && !(pAura->GetSpellProto()->attributes & ATTRIBUTES_IGNORE_INVULNERABILITY) &&
					pAura->GetSpellProto()->School != 0 )
				{
					pAura->Remove();
				}
			}
		}break;

		/*
			Cheat death
			You have a 33% chance that an attack which would otherwise kill you will instead reduce you to 10% of your maximum health. In addition, all damage taken will be reduced by up to 90% for 3 sec (modified by resilience).  This effect cannot occur more than once per minute.
		*/
		
	/*************************
	 * DRUID SPELLS
	 *************************
	 * IDs:
	 * 34297 Improved Leader of the Pack RANK 1		STATUS: DONE
	 * 34300 Improved Leader of the Pack RANK 2		STATUS: DONE
	 *  --------------------------------------------
	 *************************/

	/*
		Improved Leader of the Pack
		Your Leader of the Pack ability also causes affected targets to have a X% chance to heal themselves for X% of their total health when they critically hit with a melee or ranged attack.  The healing effect cannot occur more than once every 6 sec.
		
		Effect #1	Apply Aura: Add Flat Modifier (12)
			Value: X
		Effect #2	Dummy
			Value: 100
	*/
	    //case __Improved_Leader_of_the_Pack_Rank_1: // 34297
	    //case __Improved_Leader_of_the_Pack_Rank_2: // 34300
	    case SPELL_HASH_IMPROVED_LEADER_OF_THE_PACK:
		{
			if (!m_caster->IsPlayer()) return;

			ProcTriggerSpell ILotP;
			ILotP.origId     = __Improved_Leader_of_the_Pack; // 34299; 335.12340
			ILotP.spellId    = __Improved_Leader_of_the_Pack; // 34299; 335.12340
			ILotP.procChance = 100;
			ILotP.procFlags = PROC_ON_CRIT_ATTACK;
			ILotP.deleted = false;
			ILotP.caster = ((Player *)m_caster)->GetGUID();
			ILotP.LastTrigger = 0;
			ILotP.weapon_damage_type = 0;
			((Player *)m_caster)->m_procSpells.push_back(ILotP);
		}break;
	/*************************
	 * HUNTER SPELLS
	 *************************
	 * IDs:
	 * 35029 Focused Fire RANK 1		STATUS: ToDo
	 * 35030 Focused Fire RANK 2		STATUS: ToDo
	 *  --------------------------------------------
	 *************************/

	/*
		Focused Fire
		All damage caused by you is increased by 1% while your pet is active and the critical strike chance of your Kill Command ability is increased by 10%.
		
		Effect #1	Dummy
			Value: 1

		Effect #2	Apply Aura: Add Flat Modifier (7)
			Value: 10

	*/
//	case 35029:
//	case 35030:
//      case SPELL_HASH_FOCUSED_FIRE :
//		{
//		}break;

	/*************************
	 * PALADIN SPELLS
	 *************************
	 * IDs:
	 * 31789 Righteous Defense		STATUS: DONE
	 *  --------------------------------------------
	 * 18350 illumination			STATUS: DONE
	 *  --------------------------------------------
	 *************************/

	/*
		Righteous Defense
		Come to the defense of a friendly target, commanding up to 3 enemies attacking the target to attack the Paladin instead.
		
		Effect #1	Dummy
			Radius: 5 yards

		Effect #2	Trigger Spell
			Spell #31980
	*/
	    case SPELL_HASH_RIGHTEOUS_DEFENSE: // 31789
		{
			//we will try to lure 3 enemies from our target
			if(!unitTarget) break;
			if(!m_caster->IsUnit()) break;

			Unit *targets[3];
			int targets_got=0;
			for(std::set<Object*>::iterator itr = unitTarget->GetInRangeSetBegin(), i2; itr != unitTarget->GetInRangeSetEnd(); )
			{
				i2 = itr++;
				// don't add objects that are not units and that are dead
				if((*i2)->GetTypeId()!= TYPEID_UNIT || !((Unit*)(*i2))->isAlive())
					continue;
		        
				Creature *cr=((Creature*)(*i2));
				if(cr->GetAIInterface()->GetNextTarget()==unitTarget)
					targets[targets_got++]=cr;
				if(targets_got==3)
					break;
			}
			for(int i=0;i<targets_got;i++)
			{
				//set threat to this target so we are the msot hated
				uint32 threat_to_him = targets[i]->GetAIInterface()->getThreatByPtr( unitTarget );
				uint32 threat_to_us = targets[i]->GetAIInterface()->getThreatByPtr((Unit *)m_caster);
				int threat_dif = threat_to_him - threat_to_us;
				if(threat_dif>0)//should nto happen
					targets[i]->GetAIInterface()->modThreatByPtr((Unit *)m_caster,threat_dif);
				targets[i]->GetAIInterface()->AttackReaction((Unit *)m_caster,1,0);
				targets[i]->GetAIInterface()->SetNextTarget((Unit *)m_caster);
			}
		}break;
	/*
		Illumination
		After getting a critical effect from your Flash of Light, Holy Light, or Holy Shock heal spell, gives you a X% chance to gain mana equal to 60% of the base cost of the spell.
		
		Effect #1	Apply Aura: Proc Trigger Spell
			Proc chance: 20%
			Spell #18350 <-- THIS SPELL	
		Effect #2	Apply Aura: Override Class Scripts (2689)
			Value: 60
	*/
	/*case SPELL_HASH_ILLUMINATION:
		{
			if(!p_caster) return;
			SpellEntry * sp = p_caster->last_heal_spell ? p_caster->last_heal_spell : m_spellInfo;
			uint32 cost = float2int32( float( float(sp->manaCost) * 0.6f ) );
			uint32 basecost = cost;
			SendHealManaSpellOnPlayer(p_caster, p_caster, cost, 0);
			cost+=p_caster->GetUInt32Value(UNIT_FIELD_POWER1);
			if(cost>p_caster->GetUInt32Value(UNIT_FIELD_MAXPOWER1))
				p_caster->SetUInt32Value(UNIT_FIELD_POWER1,p_caster->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
			else
				p_caster->SetUInt32Value(UNIT_FIELD_POWER1,cost);

			WorldPacket datamr(SMSG_SPELLENERGIZELOG, 30);
			datamr << unitTarget->GetNewGUID();
			datamr << u_caster->GetNewGUID();
			datamr << uint32(20272);
			datamr << uint32(0);
			datamr << uint32( basecost );
			u_caster->SendMessageToSet(&datamr,true);
		}break;*/

	/*************************
	 * PRIEST SPELLS
	 *************************
	 * IDs:
	 * NO SPELLS
	 *  --------------------------------------------
	 *************************/
		case SPELL_HASH_PRAYER_OF_MENDING :
		{
				switch(spellId)
				{
		         // Par numero, Prayer of mending et derivé
	             case 33076: // Prayer of mending 
	             case 48112: // Prayer of mending 
	             case 48113: // Prayer of mending 
	             //
		         {
			      if( !m_caster->IsPlayer() ) return;
				  if(!((Player *)m_caster)->GetGroup() == NULL ) return;

			      ((Player *)m_caster)->GetGroup()->m_prayerOfMendingCount = 5;		// 5 jumpz
			      ((Player *)m_caster)->GetGroup()->m_prayerOfMendingTarget = this->GetUnitTarget()->GetLowGUID();
			      if( ((Player *)m_caster)->GetGroup()->m_prayerOfMendingTarget == NULL )
				                ((Player *)m_caster)->GetGroup()->m_prayerOfMendingTarget = ((Player *)m_caster)->GetLowGUID();
		         }break;

	             case 41637: // Prayer of mending 
	 	         {
			      if( !m_caster->IsPlayer()) return;
				  if( !pSpellId ) return;

			      SpellEntry *mspell = dbcSpell.LookupEntry( pSpellId );

			      // perform actual heal
			      ((Player *)m_caster)->Heal(((Player *)m_caster), m_spellInfo->Id, mspell->EffectBasePoints[0]);

			      // remove parent aura
			      ((Player *)m_caster)->RemoveAura(mspell->Id);

			      // find a new party member within 20 yards
			      if( ((Player *)m_caster)->GetGroup() != NULL )
			      {
				   Group *grp = ((Player *)m_caster)->GetGroup();
				   if( grp->m_prayerOfMendingCount > 0 )
				   {
					// decrement counter
					--grp->m_prayerOfMendingCount;
					
					vector< Player * > possible_targets;
					SubGroup *sg;
					grp->Lock();
					for(uint32 sgid = 0; sgid < grp->GetSubGroupCount(); ++sgid)
					{
						sg = ((Player *)m_caster)->GetGroup()->GetSubGroup(sgid);
						for(GroupMembersSet::iterator itr = sg->GetGroupMembersBegin(); itr != sg->GetGroupMembersEnd(); ++itr)
						{
							if( (*itr)->m_loggedInPlayer != NULL && (*itr)->m_loggedInPlayer->GetInstanceID() == ((Player *)m_caster)->GetInstanceID() &&
								((*itr)->m_loggedInPlayer != m_caster) && (*itr)->m_loggedInPlayer->GetDistance2dSq(((Player *)m_caster)) <= 400.0f )
							{
								// target ok
								possible_targets.push_back((*itr)->m_loggedInPlayer);
							}
						}
					}
					grp->Unlock();

					// pick a new target
					if( possible_targets.size() > 0 )
					{
						Player *tgt;
						if( possible_targets.size() > 1 )
							tgt = (Player *)possible_targets[RandomUInt((uint32)(possible_targets.size())-1)];
						else
							tgt = (Player *)possible_targets[0];

						// cast us on them
						Spell *nsp = new Spell(((Player *)m_caster), dbcSpell.LookupEntry(mspell->Id), true, NULL);
						/*SpellCastTargets tgts(tgt->GetGUID()); //error C2664
						nsp->prepare(&tgts);*/
						grp->m_prayerOfMendingTarget = tgt->GetLowGUID();
					}
					else
					{
						// clear the data
						grp->m_prayerOfMendingCount = 0;
						grp->m_prayerOfMendingTarget = 0;
					}
				   }
				   else grp->m_prayerOfMendingTarget = 0;
			      }
                 }break;
				} // End of Switch(SpellId)
		}break;
	/*************************
	 * SHAMAN SPELLS
	 *************************
	 * IDs:
	 * NO SPELLS
	 *  --------------------------------------------
	 *************************/

	/*************************
	 * WARLOCK SPELLS
	 *************************
	 * IDs:
	 * 19028 Soul Link				STATUS: ToDo
	 *  --------------------------------------------
	 * 19028 soul link effect		STATUS: DONE
	 *  --------------------------------------------
	 *************************/

	/*
		Soul Link
		When active, 20% of all damage taken by the caster is taken by your Imp, Voidwalker, Succubus, Felhunter, Felguard, or enslaved demon instead.  That damage cannot be prevented.  In addition, both the demon and master will inflict 5% more damage.  Lasts as long as the demon is active and controlled.
		
		Effect	Dummy
	*/

	case SPELL_HASH_SOUL_LINK :
	{
		switch(spellId)
		{
//	     case 19028:
//		 {
//		 }break;

	     /*
		 Soul Link - EFFECT
		 20% of damage taken by master is split with demon. All damage increased by 5%.
	     */
	     case 25228:
		 {
			if(!m_caster->IsUnit())            return;
			if(!((Unit *)m_caster)->isAlive()) return;
			if(!unitTarget)                    return;
			if(!unitTarget->isAlive())         return;

			uint32 pet_dmg = this->forced_basepoints[0]*20/100;
			unitTarget->ModUnsigned32Value(UNIT_FIELD_HEALTH,pet_dmg);
			unitTarget->DealDamage(((Unit *)m_caster),pet_dmg,0,0,25228,true);
		 }break;
		}

	} break;

	/*************************
		Non-Class spells
		- Done
	 *************************/
	/*
		Poultryizer
		Turns the target into a chicken for 15 sec.   Well, that is assuming the transmogrification polarity has not been reversed...
	*/
	case SPELL_HASH_POULTRYIZER: // 30507
	{
			if( !unitTarget || !unitTarget->isAlive())
				return;

			((Unit *)m_caster)->CastSpell(unitTarget->GetGUID(),30501,true);
	}break;
	/*
		Six Demon Bag
		Blasts enemies in front of you with the power of wind, fire, all that kind of thing!
	*/
	case SPELL_HASH_SIX_DEMON_BAG: // 14537
	{
			if( !unitTarget || !unitTarget->isAlive())
				return;

			uint32 ClearSpellId[6] = { 8401,8408,930,118,1680,10159 };
			int min = 1;
			uint32 effect = min + int( ((6-min)+1) * rand() / (RAND_MAX + 1.0) );
			
			((Unit *)m_caster)->CastSpell(unitTarget, ClearSpellId[effect] ,true);
	}break;

	case SPELL_HASH_EXTRACT_GAS : // 30427 
	{
			bool check = false;
			uint32 cloudtype = 0;
			Creature *creature = NULL;

			if(!m_caster->IsPlayer()) return;

			for(Object::InRangeSet::iterator i = ((Player *)m_caster)->GetInRangeSetBegin(); i != ((Player *)m_caster)->GetInRangeSetEnd(); ++i)
			{
				if((*i)->GetTypeId() == TYPEID_UNIT)
				{
					creature=static_cast<Creature *>((*i));
					cloudtype=creature->GetEntry();
					if(cloudtype == 24222 || cloudtype == 17408 || cloudtype == 17407 || cloudtype == 17378)
					{
						if(((Player *)m_caster)->GetDistance2dSq((*i)) < 400)
						{
							((Player *)m_caster)->SetSelection(creature->GetGUID());
							check = true;
							break;
						}
					}
				}
			}
			
			if(check)
			{
				uint32 item,count = 3+(rand()%3);
			
				if (cloudtype==24222) item=22572;//-air
				if (cloudtype==17408) item=22576;//-mana
				if (cloudtype==17407) item=22577;//-shadow
				if (cloudtype==17378) item=22578;//-water

				Item *add = ((Player *)m_caster)->GetItemInterface()->FindItemLessMax(item, count, false);
				if (!add)
				{
					ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(item);
					SlotResult slotresult;

					slotresult = ((Player *)m_caster)->GetItemInterface()->FindFreeInventorySlot(proto);
					if(!slotresult.Result)
					{
						((Player *)m_caster)->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
						return;
					}
					Item * it=objmgr.CreateItem(item,((Player *)m_caster));  
					it->SetUInt32Value( ITEM_FIELD_STACK_COUNT, count);
					((Player *)m_caster)->GetItemInterface()->SafeAddItem(it,slotresult.ContainerSlot, slotresult.Slot);
					creature->Despawn(3500,creature->proto->RespawnTime);
				}
				else
				{
					add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + count);
					add->m_isDirty = true;
					creature->Despawn(3500,creature->proto->RespawnTime);
				}
			}
	}break;

	//curse of agony(18230) = periodic damage increased in 
	//flag 2031678
	/*case 34120:
		{//steady shot
		if(unitTarget)
		if(unitTarget->IsDazed())
		{
//			u_caster->SpellNonMeleeDamageLog(unitTarget,spellId,damage,false);
			u_caster->SpellNonMeleeDamageLog(unitTarget,spellId,damage,pSpellId==0);
		}
	
		}break;
	*/
	//case 974:
	//case 32593:
	//case 32594:
    case SPELL_HASH_EARTH_SHIELD :
	{
			if(!pSpellId) return;

			SpellEntry *spellInfo = dbcSpell.LookupEntry(pSpellId);
			if(!spellInfo) return;
			uint32 heal32 = CalculateEffect(i,((Unit *)m_caster));
			unitTarget=((Unit *)m_caster); // Should heal caster :p

			if(heal32) Heal(heal32);
	}break;
	case SPELL_HASH_ARCANE_TORRENT : // 28730 - Racial (??) //Arcane Torrent (Mana)
	{
			// for each mana tap, gives you (2.17*level+9.136) mana
			if( !unitTarget ) return;
			if( !m_caster->IsPlayer()) return;

			if(!unitTarget->isAlive() || unitTarget->getClass() == ROGUE)
				return;

			uint32 count = 0;
			for(uint32 x = 0; x < MAX_AURAS; ++x)
			{
				if(unitTarget->m_auras[x] && unitTarget->m_auras[x]->GetSpellId() == 28734)
				{
					unitTarget->m_auras[x]->Remove();
					++count;
				}
			}

			uint32 gain = (uint32)(count * (2.17*((Player *)m_caster)->getLevel()+9.136));
			((Player *)m_caster)->Energize(unitTarget,28730,gain,POWER_TYPE_MANA);
	}break;
	case SPELL_HASH_MANA_TIDE_TOTEM : // Global
	{
	    if(spellId == 39610) // Mana Tide Totem 1
		{
			if(unitTarget == NULL || unitTarget->isDead() || unitTarget->getClass() == WARRIOR || unitTarget->getClass() == ROGUE)
 				return;
 			uint32 gain = (uint32) (unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1)*0.06);
			unitTarget->Energize(unitTarget,16191,gain,POWER_TYPE_MANA);
		}break;

	}break;
	case SPELL_HASH_JUDGEMENT_OF_COMMAND: // 20425: //Judgement of Command
	//case 20961: //Judgement of Command
	//case 20962: //Judgement of Command
	//case 20967: //Judgement of Command
	//case 20968: //Judgement of Command
	//case 27172: //Judgement of Command
	{
			uint32 SpellID = m_spellInfo->EffectBasePoints[i]+1;
			Spell * spell=new Spell(m_caster,dbcSpell.LookupEntry(SpellID),true,NULL);
			SpellCastTargets targets;
			targets.m_target = unitTarget;
			spell->prepare(&targets);
	}break;

	case SPELL_HASH_CANNIBALIZE: // 20577 - Racial
	{
		if(!m_caster->IsPlayer()) return;

			bool check = false;
			float rad = GetRadius(i);
			rad *= rad;
			for(Object::InRangeSet::iterator i = ((Player *)m_caster)->GetInRangeSetBegin(); i != ((Player *)m_caster)->GetInRangeSetEnd(); ++i)
			{
				if((*i)->GetTypeId() == TYPEID_UNIT)
				{
					if(static_cast<Creature *>((*i))->getDeathState() == CORPSE)
					{
						CreatureInfo *cn = static_cast<Creature *>((*i))->GetCreatureName();
							if(cn && cn->Type == HUMANOID || cn->Type == UNDEAD)
							{
								if(((Player *)m_caster)->GetDistance2dSq((*i)) < rad)
								{
									check = true;
									break;
								}
							}
						
					}
				}
			}
			
			if(check)
			{
				((Player *)m_caster)->cannibalize = true;
				((Player *)m_caster)->cannibalizeCount = 0;
				sEventMgr.AddEvent(((Player *)m_caster), &Player::EventCannibalize, uint32(7),
					EVENT_CANNIBALIZE, 2000, 5,0);
				((Player *)m_caster)->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_CANNIBALIZE);
			}
	}break;
	case SPELL_HASH_ARCANITE_DRAGONLING :
	{
			uint32 spell_id;
			switch(m_spellInfo->Id)
			{
                case 23074: spell_id = 19804; break; // Arcanite Dragonling
                case 23075: spell_id = 12749; break; // Mithril Mechanical Dragonling
                case 23076: spell_id =  4073; break; // Mechanical Dragonling
                case 23133: spell_id = 13166; break; // Gnomish Battle Chicken
				default: 
					return;
			}
			((Unit *)m_caster)->CastSpell(((Unit *)m_caster),spell_id,true);
	}break;
	case SPELL_HASH_GIFT_OF_LIFE : // 23725
	{
			if(!playerTarget) break;

			SpellCastTargets tgt;
			tgt.m_target = playerTarget;
			SpellEntry * inf =dbcSpell.LookupEntry(23782);
			Spell * spe = new Spell(((Unit *)m_caster),inf,true,NULL);
			spe->prepare(&tgt);

		}break;
	case SPELL_HASH_LAST_STAND : // 12975
	{
			if(!playerTarget) break;

			SpellCastTargets tgt;
			tgt.m_target = playerTarget;
			SpellEntry * inf =dbcSpell.LookupEntry(12976);
			Spell * spe = new Spell(((Unit *)m_caster),inf,true,NULL);
			spe->prepare(&tgt);
	}break;
	/*************************
		Non-Class spells
		- ToDo
	 *************************/
	/*
	case SPELL_HASH_RED_FIREWORK : // 6668 - Red Firework
		{
			// Shoots a firework into the air that bursts into a thousand red stars
		}break;
	case 8344:// Universal Remote
		{
			//FIXME:Allows control of a mechanical target for a short time.  It may not always work and may just root the machine or make it very very angry.  Gnomish engineering at its finest.
		}break;
	case 9976:// Polly Eats the E.C.A.C.
		{
			//FIXME: Don't know what this does
		}break;
	case 10137:// Fizzule's Whistle
		{
			//FIXME:Blow on the whistle to let Fizzule know you're an ally
			//This item comes after a finish of quest at venture co.
			//You must whistle this every time you reach there to make Fizzule
			//ally to you.
		}break;
	case 11540:// Blue Firework
		{
			//Shoots a firework into the air that bursts into a thousand blue stars
		}break;
	case 11541:// Green Firework
		{
			//Shoots a firework into the air that bursts into a thousand green stars
		}break;
	case 11542:// Red Streaks Firework
		{
			//Shoots a firework into the air that bursts into a thousand red streaks
		}break;
	case 11543:// Red, White and Blue Firework
		{
			//Shoots a firework into the air that bursts into red, white and blue stars
		}break;
	case 11544:// Yellow Rose Firework
		{
			//Shoots a firework into the air that bursts in a yellow pattern
		}break;
	case 12151:// Summon Atal'ai Skeleton
		{
			//8324	Atal'ai Skeleton

			//FIXME:Add here remove in time event
		}break;
	case 13535:// Tame Beast
		{
			
		}break;
	case 13006:// Shrink Ray
		{
			//FIXME:Schematic is learned from the gnomish engineering trainer. The gnomish/gobblin engineering decision is made when you are lvl40+ and your engineering is 200+. Interestingly, however, when this item fails to shrink the target, it can do a variety of things, such as...
			//-Make you bigger (attack power +250)
			//-Make you smaller (attack power -250)
			//-Make them bigger (same effect as above)
			//-Make your entire party bigger
			//-Make your entire party smaller
			//-Make every attacking enemy bigger
			//-Make ever attacking enemy smaller
			//Works to your advantage for the most part (about 70% of the time), but don't use in high-pressure situations, unless you're going to die if you don't. Could tip the scales the wrong way.
			//Search for spells of this


			//13004 - grow <- this one
			//13010 - shrink <-this one
			//
		}break;
	case 13180:// Gnomish Mind Control Cap
		{
			// FIXME:Take control of humanoid target by chance(it can be player)
		}break;
	case 13278:// Gnomish Death Ray
		{
			// FIXME:The devices charges over time using your life force and then directs a burst of energy at your opponent
			//Drops life			
		}break;
	case 13280:// Gnomish Death Ray
		{
			//FIXME: Drop life
		}break;
	case 17816:// Sharp Dresser
		{
			//Impress others with your fashion sense
		}break;
	case 21343:// Snowball
		{
		}break;
	case 23645:// Hourglass Sand
		{
			//Indeed used at the Chromo fight in BWL. Chromo has a stunning debuff, uncleansable, unless you have hourglass sand. This debuff will stun you every 4 seconds, for 4 seconds. It is resisted a lot though. Mage's and other casters usually have to do this fight with the debuff on, healers, tanks and hunters will get some to cure themselves from the debuff
		}break;
	case 24325:// Pagle's Point Cast - Create Mudskunk Lure
		{
			//FIXME:Load with 5 Zulian Mudskunks, and then cast from Pagle's Point in Zul'Gurub
		}
	case 24392:// Frosty Zap
		{
			//FIXME:Your Frostbolt spells have a 6% chance to restore 50 mana when cast.
			//damage == 50
		}break;
	case 25822:// Firecrackers
		{
			//FIXME:Find firecrackers
		}break;
	case 26373:// Lunar Invititation
		{
			//FIXME: Teleports the caster from within Greater Moonlight
		}break;
	case 26374:// Elune's Candle
		{
			//FIXME:Shoots a firework at target
		}break;
	case 26889:// Give Friendship Bracelet
		{
			//Give to a Heartbroken player to cheer them up
			//laugh emote
		}break;
	case 27662:// Throw Cupid's Dart
		{
			//FIXME:Shoot a player, and Kwee Q. Peddlefeet will find them! (Only works on players with no current critter pets.)
		}break;
	case 28414:// Call Ashbringer
		{
			//http://www.thottbot.com/?i=53974
		}break;
	case 28806:// Toss Fuel on Bonfire
		{
			//FIXME:Dont know what this dummy does
		}break;
	case 7669:// Bethor's Potion
		{
			// related to Hex of Ravenclaw,
			// its a dispell spell.
			//FIXME:Dont know whats the usage of this dummy
		}break;
	case 8283:// Snufflenose Command
		{
			//FIXME:Quest Blueleaf Tubers
			//For use on a Snufflenose Gopher
		}break;
	case 8913:// Sacred Cleansing
		{
			//FIXME:Removes the protective enchantments around Morbent Fel
			//Quest Morbent Fel
		}break;
	case 9962://Capture Treant
		{
			//Quest Treant Muisek 
		}break;
	case 10113:// Flare Gun's flare
		{
			//FIXME:Quest Deep Cover
			//1543 may need to cast this
			//2 flares and the /salute
		}break;
	case 10617:// Release Rageclaw
		{
			//Quest Druid of the Claw
			//Use on the fallen body of Rageclaw
		}break;
	case 11402:// Shay's Bell
		{
			//FIXME:Quest Wandering Shay
			//Ring to call Shay back to you
		}break;
	case 11548:// Summon Spider God
		{
			//FIXME:Quest Summoning Shadra  (Elite)
			//Use at the Shadra'Alor Altar to summon the spider god
		}break;
	case 11610:// Gammerita Turtle Camera
		{
			//Quest The Super Snapper FX 
		}break;
	case 11886:// Capture Wildkin
		{
			//Quest Testing the Vessel
			//Shrink and Capture a Fallen Wildkin
		}break;
	case 11887:// Capture Hippogryph
		{
			//FIXME:Same with 11888
			//Quest Hippogryph Muisek 
		}break;
	case SPELL_HASH_CAPTURE_FAERIE_DRAGON : // 11888 - Capture Faerie Dragon
		{
			//FIXME:Check Faerie Dragon Muisek is killed or not if its killed update quest
			//And allow create of fearie Dragon which is effect 1
			//Quest: Faerie Dragon Muisek
		}break;
	case SPELL_HASH_CAPTURE_MOUNTAIN_GIANT : // 11889 - Capture Mountain Giant
		{
			//FIXME:Same with 11888
			//Quest: Mountain Giant Muisek
		}break;
	case SPELL_HASH_SUMMON_ECHEYAKEE : // 12189 - Summon Echeyakee
		{
			//3475	Echeyakee

			//FIXME:Quest Echeyakee
		}break;
	case SPELL_HASH_XIGGS_SIGNAL_FLARE : // 12283 - Xiggs Signal Flare
		{
			//Quest Signal for Pickup
			//To be used at the makeshift helipad in Azshara. It will summon Pilot Xiggs Fuselighter to pick up the tablet rubbings
		}break;
	case SPELL_HASH_FEL_CURSE : // 12938 - Fel Curse
		{
			//FIXME:Makes near target killable(servants of Razelikh the Defiler)
		}break;
	case SPELL_HASH_BLAZERUNNER_DISPEL : // 14247 - Blazerunner Dispel
		{
			//FIXME:Quest Aquementas and some more
		}break;
	case SPELL_HASH_CAPTURE_GRARK : // 14250 - Capture Grark
		{
			//Quest Precarious Predicament
		}break;
	case SPELL_HASH_DARK_IRON_DRUNK_MUG : // 14813 - Rocknot's Ale (à changé en 335)
		{
			//you throw the mug
			//and the guy gets pissed well everyone gets pissed and he crushes the door so you can get past
			//maybe after like 30 seconds so you can get past.  but lke I said I have never done it myself 
			//so i am not 100% sure what happens.
		}break;

	case SPELL_HASH_CAPTURE_WORG_PUP : // 15998 - Capture Worg Pup
		{
			//FIXME:Ends Kibler's Exotic Pets  (Dungeon) quest
		}break;
	case SPELL_HASH_RELEASING_CORRUPT_OOZE : // 16031 - Releasing Corrupt Ooze
		{
			//FIXME:Released ooze moves to master ooze and "Merged Ooze Sample"
			//occurs after some time.This item helps to finish quest
		}break;
	case SPELL_HASH_TEMPERATURE_READING : // 16378 - Temperature Reading
		{
			//FIXME:Quest Finding the Source
			//Take a reading of the temperature at a hot spot.
		}break;
	case SPELL_HASH_RELEASE_UMI_S_YETI : // 17166 - Release Umi's Yeti
		{
			//Quest Are We There, Yeti?
			//Select Umi's friend and click to release the Mechanical Yeti
		}break;
	case SPELL_HASH_TEST_FETID_SKULL : // 17271 - Test Fetid Skull
		{
			//FIXME:Marauders of Darrowshire
			//Wave over a Fetid skull to test its resonance
		}break;
	case SPELL_HASH_KODO_KOMBOBULATOR : // 18153 -Kodo Kombobulator
		{
			//FIXME:Kodo Roundup Quest
			//Kodo Kombobulator on any Ancient, Aged, or Dying Kodo to lure the Kodo to follow (one at a time)
		}break;
	case SPELL_HASH_PLACING_SMOKEY_S_EXPLOSIVES : // 19250 - Placing Smokey's Explosives
		{
			//This is something related to quest i think
		}break;
	case SPELL_HASH_APPLY_SALVE : // 19512 - Apply Salve
		{
			//FIXME:Cure a sickly animal afflicted by the taint of poisoning
		}break;
	case SPELL_HASH_TRIAGE : // 20804 - Triage 
		{
			//Quest Triage
			//Use on Injured, Badly Injured, and Critically Injured Soldiers
		}break;
	case SPELL_HASH_MELODIOUS_RAPTURE : // 21050 - Melodious Rapture
		{
			//Quest Deeprun Rat Roundup 
		}break;
	case SPELL_HASH_ASPECT_OF_NEPTULON : // 21332 - Aspect of Neptulon
		{
			//FIXME:Used on plagued water elementals in Eastern Plaguelands
			//Quest:Poisoned Water
		}break;
	case SPELL_HASH_MANIFEST_SPIRIT : // 21960 - Manifest Spirit
		{
			//FIXME:Forces the spirits of the first centaur Kahns to manifest in the physical world
			//thats a quest
			//its for maraudon i think
			//u use that on the spirit mobs
			//to release them
		}break;
	case SPELL_HASH_TRANSMOGRIFY_ : // 23359 - Transmogrify!
		{
			//Quest Zapped Giants 
			//Zap a Feralas giant into a more manageable form
		}break;
	case SPELL_HASH_SUMMON_MOR_GRAYHOOF_DND : // 27184 - Summon Mor Grayhoof
		{
			//16044	Mor Grayhoof Trigger
			//16080	Mor Grayhoof

			//Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
			//and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
		}break;
	case SPELL_HASH_SUMMON_ISALIEN_DND : // 27190 - Summon Isalien
		{
			//16045	Isalien Trigger
			//16097	Isalien

			//Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
			//and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
		}break;
	case SPELL_HASH_SUMMON_JARIEN_AND_SOTHOS_DND : // 27191 - Summon the remains of Jarien and Sothos
		{
			//16046	Jarien and Sothos Trigger
			//16101	Jarien
			//16103	Spirit of Jarien
			
			//16102	Sothos
			//16104	Spirit of Sothos
			//Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
			//and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
		}break;
	case SPELL_HASH_SUMMON_KORMOK_DND : // 27201 - Summon the spirit of Kormok
		{
			//16047	Kormok Trigger
			//16118	Kormok
			//Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
			//and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
		}break;
	case SPELL_HASH_SUMMON_LORD_VALTHALAK_DND : // 27202 - Summon Lord Valthalak
		{
			// 16042	Lord Valthalak
			// 16048	Lord Valthalak Trigger
			// 16073	Spirit of Lord Valthalak
			//Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
			//and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
		}break;
	case SPELL_HASH_SUMMON_VARIOUS_DND : // 27203 - Summon the spirits of the dead at haunted locations
		{
			//Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
			//and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
		}break;
	case SPELL_HASH_SUMMON_THELRIN_DND : // 27517 - Use this banner at the Arena in Blackrock Depths to challenge Theldren
		{
			// This is used to make Theldren spawn at the place where it used
			// I couldnt find theldrin, and his men in creature names database
			// Someone has to write this and this is related to The Challange quest
			// By moving to the center grate, you trigger the arena event. 
			// A random group of mobs (spiders, worms, bats, raptors) spawns, 
			// and you have to kill them. After the last one dies, and a small 
			// break, a boss mob spawns. Successfully completing this event 
			// turns the arena spectators from red to yellow
		}break;
		*/
	}										 
}

void Spell::SpellEffectTeleportUnits( uint32 i )  // Teleport Units
{
	uint32 spellId = m_spellInfo->Id;

	if( unitTarget == NULL )
		return;

	// Try a dummy SpellHandler
	if( sScriptMgr.CallScriptedDummySpell( spellId, i, this ) )
		return;

	// Shadowstep
	if( m_spellInfo->NameHash == SPELL_HASH_SHADOWSTEP && m_caster->IsPlayer() && m_caster->IsInWorld() )
	{
		/* this is rather tricky actually. we have to calculate the orientation of the creature/player, and then calculate a little bit of distance behind that. */
		float ang;
		if( unitTarget == m_caster )
		{
			/* try to get a selection */
 			unitTarget = m_caster->GetMapMgr()->GetUnit(((Player *)m_caster)->GetSelection());
//			if( (unitTarget == NULL ) || !isHostile(p_caster, unitTarget) || (unitTarget->CalcDistance(p_caster) > 25.0f)) //removed by Zack : no idea why hostile is used. Isattackable should give a wider solution range
			if( (unitTarget == NULL ) || !isAttackable(((Player *)m_caster), unitTarget, !(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED) ) || (unitTarget->CalcDistance(((Player *)m_caster)) > 25.0f))
				return;
		}

		if( unitTarget->GetTypeId() == TYPEID_UNIT )
		{
			if( unitTarget->GetUInt64Value( UNIT_FIELD_TARGET ) != 0 )
			{
				/* We're chasing a target. We have to calculate the angle to this target, this is our orientation. */
				ang = m_caster->calcAngle(m_caster->GetPositionX(), m_caster->GetPositionY(), unitTarget->GetPositionX(), unitTarget->GetPositionY());

				/* convert degree angle to radians */
				ang = ang * float(M_PI) / 180.0f;
			}
			else
			{
				/* Our orientation has already been set. */
				ang = unitTarget->GetOrientation();
			}
		}
		else
		{
			/* Players orientation is sent in movement packets */
			ang = unitTarget->GetOrientation();
		}

		// avoid teleporting into the model on scaled models
		if( unitTarget->GetTypeId() == TYPEID_UNIT )
		{
		 const static float shadowstep_distance = 1.6f * unitTarget->GetFloatValue(OBJECT_FIELD_SCALE_X);
		 float new_x = unitTarget->GetPositionX() - (shadowstep_distance * cosf(ang));
		 float new_y = unitTarget->GetPositionY() - (shadowstep_distance * sinf(ang));
		
		
		 /* Send a movement packet to "charge" at this target. Similar to warrior charge. */
		 ((Player *)m_caster)->z_axisposition = 0.0f;
		 ((Player *)m_caster)->SafeTeleport(((Player *)m_caster)->GetMapId(), ((Player *)m_caster)->GetInstanceID(), LocationVector(new_x, new_y, (unitTarget->GetPositionZ() + 0.1f), unitTarget->GetOrientation()));
        }

		return;
	}

	/* TODO: Remove Player From bg */

	if(unitTarget->GetTypeId() == TYPEID_PLAYER)
		HandleTeleport(spellId, unitTarget);
}
void Spell::SpellEffectApplyAura(uint32 i)  // Apply Aura
{
	int32 miscValue = 0; // Pour AddMod(...)
	bool boss = false;

	if(unitTarget == NULL)
	{
	  //Log.Error("[SpellEffectApplyAura]","Echec unitTarget NULL <-- Report this to devs.");
	  return;
	}

	if( unitTarget->isDead() /*&& !(m_spellInfo->Flags4 & CAN_PERSIST_AND_CASTED_WHILE_DEAD)*/ )
		return;

	//we shouldn't apply fireball dot if we have fireball glyph -- Hearstone -- Modifié par Randdrick
	if( m_spellInfo->NameHash == SPELL_HASH_FIREBALL && m_caster->IsPlayer() && ((Player *)m_caster)->FindAuraPosByNameHash(SPELL_HASH_GLYPH_OF_FIREBALL) )
		return;

	//dont apply frostbolt dot with frostbolt glyph -- Hearstone -- Modifié par Randdrick
	if( m_spellInfo->NameHash == SPELL_HASH_FROSTBOLT && m_caster->IsPlayer() && ((Player *)m_caster)->FindAuraPosByNameHash( SPELL_HASH_GLYPH_OF_FROSTBOLT ) )
		return;
	
	// avoid map corruption.
	if(unitTarget->GetInstanceID() != m_caster->GetInstanceID())
	{
		Log.Error("[SpellEffectApplyAura]","Echec: probleme de position dans la map, map corrupt!");
		return;
	}
	 
	if(!m_caster) // Impossible normalement, lié a un prob de codage ?
    {
		Log.Error("[SpellEffectApplyAura]","Echec: Caster non trouvee! <-- Report this to devs.");
		return;
	}

	if (unitTarget->IsCreature())
	{
		Creature* c = static_cast<Creature*>(unitTarget);
		Log.Warning("[SpellEffectApplyAura]", "Le NPC sur lequel s'applique l'aura est: %s", c->GetCreatureName()->Name);
		if (c != NULL && c->GetCreatureName() != NULL && c->GetCreatureName()->Rank == ELITE_WORLDBOSS)
		{
			switch(m_spellInfo->EffectApplyAuraName[i])
			{
			case SPELL_AURA_MOD_CONFUSE:  // confuse
			case SPELL_AURA_MOD_CHARM:  // charm
			case SPELL_AURA_MOD_FEAR:  // fear
			case SPELL_AURA_MOD_STUN: // stun
			case SPELL_AURA_MOD_PACIFY: // pacify
			case SPELL_AURA_MOD_ROOT: // root
			case SPELL_AURA_MOD_SILENCE: // silence
			//case SPELL_AURA_MOD_INCREASE_SPEED: // increase speed, why can't bosses go faster?
			case SPELL_AURA_MOD_DECREASE_SPEED: // decrease speed
				boss = true; //this aura cant be applied, at all
			}
		}
	}
	
	// Randdrick : On n'applique pas les auras ci-dessus si il s'agit d'un Elite
	if ( boss == true) 
	{
		Log.Notice("[SpellEffectApplyAura]","Cette aura ne s'applique pas sur un Elite ");
		return;
	}

	//Log.Warning("[SpellEffectApplyAura]","Caster: %d (%s)",m_caster->GetEntry(),m_caster->IsPlayer()?"Player":"Creature");
	//Log.Warning("[SpellEffectApplyAura]","Cible: %d  (%s)",unitTarget->GetEntry(),unitTarget->IsPlayer()?"Player":"Creature");
	
	//check if we already have stronger aura
	Aura* pAura = NULL;

	//Initialisation du caster
	Object* caster = NULL;

	// -----------------------------------------
	// Recherche si la cible a deja la meme aura (le meme SpellApllyAuraName[x])
	std::map<uint32,Aura* >::iterator itr=unitTarget->tmpAura.find(m_spellInfo->Id);
	if(itr==unitTarget->tmpAura.end()) 
	{
		int32 Duration = GetDuration(INVOC_DURATION);

		if (m_spellInfo->channelInterruptFlags != 0 && m_caster->IsUnit())
			Duration *= ((Unit *)m_caster)->GetFloatValue(UNIT_MOD_CAST_SPEED);

		// Handle diminishing returns, if it should be resisted, it'll make duration 0 here.
		if(!(m_spellInfo->attributes & ATTRIBUTES_PASSIVE)) // Passive
			::ApplyDiminishingReturnTimer(&Duration, unitTarget, m_spellInfo);

		if(Duration == 0)
		{
			//maybe add some resist messege to client here ?
			SendCastResult(SPELL_FAILED_IMMUNE);
			return;
		}
		
		if(m_caster->IsGO() && ((GameObject *)m_caster)->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && ((GameObject *)m_caster)->m_summoner)
			caster = ((GameObject *)m_caster)->m_summoner;
		else
			caster = m_caster;
		
		//--------------------
		// Création de l'aura
		pAura = new Aura(m_spellInfo, Duration, INVOC_DURATION, caster, unitTarget);
		if(pAura == NULL) return;
	
		// -----------------------------------------------------------
		// Recherche si ya pas deja une aura d'un meme groupe de spell
		Aura* oldaura = NULL;
		// Le UniqueGroupe2 est global. Par exemple, cible les auras genre 'Aspect' 
		if (m_spellInfo->UniqueGroup2 != 0)
		{
			Log.Warning("[SpellEffectApplyAura]","Spell: %u (%s), Aura %u -> UniqueGroupe2 : %u",
				m_spellInfo->Id,m_spellInfo->Name,m_spellInfo->EffectApplyAuraName[i],m_spellInfo->UniqueGroup2);
			/* 
			// Pourquoi l'aura serait-elle seulement positive ? ou est-ce un toggle ? (On/Off/On)
			// Et pourquoi virer toute les auras avec le meme groupe2 ? (devrait etre fait sur m_aurascast plutot)
			// A voir si on s'en sert plus tard, franchement la tout d'suite, je vois pas l'interet
			// ( - Branruz - )
			for(uint32 x=0; x < MAX_POSITIVE_AURAS; ++x)
			{
				if(unitTarget->m_auras[x] && 
					unitTarget->m_auras[x]->m_spellProto->UniqueGroup2 == m_spellInfo->UniqueGroup2)
				{
					unitTarget->m_auras[x]->m_wasremoved = true;
					unitTarget->m_auras[x]->Remove();
				}
			}
			*/
		}
		// Le UniqueGroupe est un filtre plus fin, genre par exemple, 'Aspect du guepard' ou ' Aspect du faucon'
		// Cette fois ci on remplace (enventuellement) oldaura par la nouvelle aura 
		if (m_spellInfo->UniqueGroup != 0)
		{
			Log.Warning("[SpellEffectApplyAura]","Spell: %u (%s)",m_spellInfo->Id,m_spellInfo->Name);
			Log.Warning("                      ","Aura %u -> UniqueGroupe      %u",m_spellInfo->EffectApplyAuraName[i],m_spellInfo->UniqueGroup);
			Log.Warning("                      ","Aura %u -> UniqueTargetBased %u",m_spellInfo->EffectApplyAuraName[i],m_spellInfo->UniqueTargetBased);
        
			if (m_spellInfo->UniqueTargetBased == 0)
			{
				oldaura = unitTarget->FindAuraByUniqueGroup(m_spellInfo->UniqueGroup, m_caster->GetGUID());
			} 
			if(oldaura)
			{
				Log.Warning("                      ","oldAura : Spell %u",oldaura->GetSpellProto()->Id);
			}
			/*else 
			{
				// target based, on peut peut-etre l'ajouté aux autres auras
				if (m_spellInfo->UniqueTargetBased & 1) 
				{
					oldaura = unitTarget->FindAuraByUniqueGroup(m_spellInfo->UniqueGroup);
				}
				// ----------------------------------------------------------------------
				// Test si seulement 1 par caster, on vire l'aura
				// genre on passe de 'Aspect du guepard' à ' Aspect du faucon', car les 2 en meme temps ca le fait pas trop, ptdr... (Brz)
		 
				if (m_spellInfo->UniqueTargetBased & 2) // only one aura can be alive from the caster at any time
				{
					std::multimap<uint32, Aura*>::iterator itr = ((Unit *)m_caster)->m_aurascast.find(m_spellInfo->UniqueGroup);
					if (itr != ((Unit *)m_caster)->m_aurascast.end())
					{
						if (oldaura == itr->second) 
						{
							oldaura = NULL; 
							//itr->second->Remove(); // A voir
						}
						itr->second->Remove();
					}
				}
				// ----------------------------------------------------------------------
			}*/

			if( (oldaura != NULL) && (Duration > 0)) // && (Duration > 0) test (Branruz)
			{
				//Log.Notice("[SpellEffectApplyAura]","Echec: Spell %u, oldaura non null: %d sec",m_spellInfo->Id,(oldaura->GetDuration(INVOC_DURATION)));
				oldaura->SetDuration(Duration,INVOC_DURATION);
				oldaura->HandleStackAdd();
				m_auraTargets.insert(unitTarget);
				Log.Warning("[SpellEffectApplyAura]","Out: Aura insert target: %u",unitTarget->GetEntry());
				return;
			}
		}
		
		pAura->pSpellId = pSpellId; //this is required for triggered spells

		//Note Randdrick : Si la même aura est appliquée sur la cible par un caster, si elle n'est pas terminée
		// et si elle n'est pas cummulative, alors on la supprime avant de la poser à nouveau.
		oldaura = unitTarget->FindAura(m_spellInfo->Id);
		if( (oldaura != NULL) && (Duration > 0) && (m_spellInfo->cumulativeAura == 0) )
		{
			Log.Notice("[SpellEffectApplyAura]","L'aura pour le Spell %u n'est pas cummulative. Elle existe deja: %d msec",m_spellInfo->Id,(oldaura->GetDuration(INVOC_DURATION)));
			unitTarget->RemoveAura(oldaura);
			oldaura = NULL;
		}

		unitTarget->tmpAura[m_spellInfo->Id] = pAura; // equiv insert
		m_spellScript->AddRef(pAura);
		sEventMgr.AddEvent(this, &Spell::NewHandleAddAura, unitTarget->GetGUID(), EVENT_SPELL_HIT, 100, 1, 0);
	}
	else
	{
		pAura=itr->second;
	} 
	miscValue = m_spellInfo->EffectMiscValue[i];

	if(((Item *)m_caster) && m_caster->IsPlayer() && m_spellInfo->EffectApplyAuraName[i]==SPELL_AURA_PROC_TRIGGER_SPELL)
		miscValue = ((Player *)m_caster)->GetItemInterface()->GetInventorySlotByGuid( ((Item *)m_caster)->GetGUID() ); // Need to know on which hands attacks spell should proc

/*		
		//Ancienne gestion
		//Note Randdrick : remplacé par sEventMgr.AddEvent(this, &Spell::NewHandleAddAura, unitTarget->GetGUID(), EVENT_SPELL_HIT, 100, 1, 0);
		if (m_spellScript != NULL) m_spellScript->AddRef(pAura);
		m_auraTargets.insert(unitTarget);
		m_auras.insert(pAura);
		//add the aura
		HandleAddAura(unitTarget, pAura);
		hadEffect=true;

*/
    
	pAura->AddMod(m_spellInfo->EffectApplyAuraName[i],damage,miscValue,i);
	if(pAura->m_modList[i].m_type)
	{
		Log.Warning("[SpellEffectApplyAura]","Spell %u Aura %u ModList[%u] => Aura : %u, Misc : %u",m_spellInfo->Id,
			m_spellInfo->EffectApplyAuraName[i],i,pAura->m_modList[i].m_type,pAura->m_modList[i].m_miscValue);		
	}

	/*Log.Notice("[SpellEffectApplyAura]","Aura %u ModList[%u] => Aura : %u, Type : %u",
		m_spellInfo->EffectApplyAuraName[i],i,pAura->m_modList[i].index,pAura->m_modList[i].m_type);*/
}
/*void Spell::SpellEffectPowerDrain(uint32 i)  // Power Drain
{
	if(!unitTarget || !unitTarget->isAlive())
		return;

	uint32 powerField = UNIT_FIELD_POWER1+m_spellInfo->EffectMiscValue[i];
	uint32 curPower = unitTarget->GetUInt32Value(powerField);
	if( powerField == UNIT_FIELD_POWER1 && unitTarget->IsPlayer() )
	{
		// Resilience - reduces the effect of mana drains by (CalcRating*2)%.

		damage *= float2int32( 1 - ( ( static_cast<Player*>(unitTarget)->CalcRating( PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE ) * 2 ) / 100.0f ) );
	}
	uint32 amt=damage+((u_caster->GetDamageDoneMod(m_spellInfo->School)*80)/100);
	if(amt>curPower)
	{
		amt=curPower;
	}
	unitTarget->SetUInt32Value(powerField,curPower-amt);
	uint32 m=u_caster->GetUInt32Value(UNIT_FIELD_MAXPOWER1+m_spellInfo->EffectMiscValue[i]);
	if(u_caster->GetUInt32Value(powerField)+amt>m)
		u_caster->SetUInt32Value(powerField, m);
	else
		u_caster->SetUInt32Value(powerField, u_caster->GetUInt32Value(powerField)+amt);	

	SendHealManaSpellOnPlayer(u_caster, u_caster, amt, m_spellInfo->EffectMiscValue[i]);
}*/

void Spell::SpellEffectPowerDrain(uint32 i)  // Power Drain
{
	if(!unitTarget || !unitTarget->isAlive())
		return;

	uint32 powerField = UNIT_FIELD_POWER1+m_spellInfo->EffectMiscValue[i];
	uint32 curPower = unitTarget->GetUInt32Value(powerField);
	uint32 amt = ((Unit *)m_caster)->GetSpellDmgBonus(unitTarget, m_spellInfo, damage, false, false);
	
	if( GetPlayerTarget() )
		amt *= float2int32( 1 - ( ( static_cast<Player *>(GetPlayerTarget())->CalcRating( PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE ) * 2 ) / 100.0f ) );
	
	if(amt > curPower)
		amt = curPower;

	unitTarget->SetUInt32Value(powerField, curPower - amt);
	((Unit *)m_caster)->Energize(((Unit *)m_caster), pSpellId ? pSpellId : m_spellInfo->Id, amt, m_spellInfo->EffectMiscValue[i]);
	unitTarget->SendPowerUpdate();
}

void Spell::SpellEffectHealthLeech(uint32 i) // Health Leech
{
	if(!unitTarget)            return;
	if(!unitTarget->isAlive()) return;
	if(!m_caster->IsUnit())    return;

	uint32 curHealth = unitTarget->GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 amt = damage;
	if(amt > curHealth)
	{
		amt = curHealth;
	}
	((Unit *)m_caster)->DealDamage(unitTarget, damage, 0, 0, m_spellInfo->Id);

	if(((Unit *)m_caster)->isAlive())
	{
	 uint32 playerCurHealth = m_caster->GetUInt32Value(UNIT_FIELD_HEALTH);
	 uint32 playerMaxHealth = m_caster->GetUInt32Value(UNIT_FIELD_MAXHEALTH);

	 if(playerCurHealth + amt > playerMaxHealth)
	 {
		((Unit *)m_caster)->SetUInt32Value(UNIT_FIELD_HEALTH, playerMaxHealth);
	 }
	 else
	 {
		((Unit *)m_caster)->SetUInt32Value(UNIT_FIELD_HEALTH, playerCurHealth + amt);		   
	 }
	}
}

void Spell::SpellEffectHeal(uint32 i) // Heal
{
	int32 dmg = float2int32(damage * amp);

	float reduce = m_spellInfo->effectChainAmplitude[i] * 100.0f;
	if(m_caster->IsUnit())
	{
	 if(m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2])
	 {
		SM_FFValue(((Unit *)m_caster)->SM_PJumpReduce, &reduce, m_spellInfo);
	 }
	 }

	amp = amp * reduce / 100;

		//yep, the usual special case. This one is shaman talent : Nature's guardian
		//health is below 30%, we have a mother spell to get value from
		switch (m_spellInfo->Id)
		{
		case 31616:
			{
				if(unitTarget && unitTarget->IsPlayer() && pSpellId && unitTarget->GetHealthPct()<30)
				{
					//check for that 10 second cooldown
					SpellEntry *spellInfo = dbcSpell.LookupEntry(pSpellId );
					if(spellInfo)
					{
						//heal value is receivad by the level of current active talent :s
						//maybe we should use CalculateEffect(uint32 i) to gain SM benefits
						int32 value = 0;
						int32 basePoints = spellInfo->EffectBasePoints[i]+1;//+(m_caster->getLevel()*basePointsPerLevel);
						int32 randomPoints = spellInfo->EffectDieSides[i];
						if(randomPoints <= 1)
							value = basePoints;
						else
							value = basePoints + rand() % randomPoints;
						//the value is in percent. Until now it's a fixed 10%
						Heal(unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*value/100);
					}
				}
			}break;
		case 34299: //Druid: Improved Leader of the PAck
			{
				if (!unitTarget->IsPlayer() || !unitTarget->isAlive())
					break;

				Player* mPlayer = static_cast< Player* >( unitTarget );
				if( !mPlayer->IsInFeralForm() || (
					mPlayer->GetShapeShift() != FORM_CAT &&
					mPlayer->GetShapeShift() != FORM_BEAR &&
					mPlayer->GetShapeShift() != FORM_DIREBEAR ) )
					break;
				uint32 max = mPlayer->GetUInt32Value( UNIT_FIELD_MAXHEALTH );
				uint32 val = float2int32( ( ( mPlayer->FindAura( 34300 ) ) ? 0.04f : 0.02f ) * max );
				if (val)
					mPlayer->Heal(mPlayer,34299,(uint32)(val));
			}break;
		case 22845: // Druid: Frenzied Regeneration
			{
				if (!unitTarget->IsPlayer() || !unitTarget->isAlive())
					break;
				Player* mPlayer = static_cast< Player* >( unitTarget );
				if (!mPlayer->IsInFeralForm() || 
					(mPlayer->GetShapeShift() != FORM_BEAR &&
					mPlayer->GetShapeShift() != FORM_DIREBEAR))
					break;
				uint32 val = mPlayer->GetUInt32Value(UNIT_FIELD_POWER2);
				if (val>100)
					val = 100;
				mPlayer->SetRage(mPlayer->GetUInt32Value(UNIT_FIELD_POWER2) - val);
				if (val)
					mPlayer->Heal(mPlayer,22845,uint32(val*2.5f));
			}break;
		case 18562: //druid - swiftmend
			{
				if( unitTarget )
				{
					uint32 new_dmg = 0;
					//consume rejuvenetaion and regrowth
					Aura * taura = unitTarget->FindAuraPosByNameHash( SPELL_HASH_REGROWTH ); //Regrowth
					if( taura != NULL && taura->GetSpellProto() != NULL)
					{
						uint32 amplitude = taura->GetSpellProto()->EffectAmplitude[1] / 1000;
						if( !amplitude )
							amplitude = 3;

						//our hapiness is that we did not store the aura mod amount so we have to recalc it
						Spell *spell = new Spell( m_caster, taura->GetSpellProto(), false, NULL );				
						uint32 healamount = spell->CalculateEffect( 1, unitTarget );  
						delete spell;
						new_dmg = healamount * 18 / amplitude;

						unitTarget->RemoveAura( taura );

						//do not remove flag if we still can cast it again
						if( !unitTarget->HasAurasWithNameHash( SPELL_HASH_REJUVENATION ) )
						{
							unitTarget->RemoveFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_REJUVENATE );	
							sEventMgr.RemoveEvents( unitTarget, EVENT_REJUVENATION_FLAG_EXPIRE );
						}
					}
					else
					{
						taura = unitTarget->FindAuraPosByNameHash( SPELL_HASH_REJUVENATION );//Rejuvenation
						if( taura != NULL && taura->GetSpellProto() != NULL )
						{
							uint32 amplitude = taura->GetSpellProto()->EffectAmplitude[0] / 1000;
							if( !amplitude )
								amplitude = 3;

							//our hapiness is that we did not store the aura mod amount so we have to recalc it
							Spell *spell = new Spell( m_caster, taura->GetSpellProto(), false, NULL );				
							uint32 healamount = spell->CalculateEffect( 0, unitTarget );  
							delete spell;
							new_dmg = healamount * 12 / amplitude;

							unitTarget->RemoveAura( taura );

							unitTarget->RemoveFlag( UNIT_FIELD_AURASTATE,AURASTATE_FLAG_REJUVENATE );	
							sEventMgr.RemoveEvents( unitTarget,EVENT_REJUVENATION_FLAG_EXPIRE );
						}
					}
					if( new_dmg > 0 )
						Heal( (int32)new_dmg );
				}
			}break;
		default:
			Heal((int32)dmg);
			break;
		}
}

void Spell::SpellEffectBind(uint32 i) 
{
	if(!playerTarget || !playerTarget->isAlive() || !m_caster)
		return;

	WorldPacket data(45);
	//uint32 areaid = playerTarget->GetZoneId();
	// Note Randdrick : On récupère l'areaId par rapport à la position du joueur
	uint32 areaid = playerTarget->GetMapMgr()->GetAreaID(playerTarget->GetPositionX(), playerTarget->GetPositionY(), playerTarget->GetPositionZ());
	if( areaid == 0xffff )
		areaid = 0;

	uint32 mapid = playerTarget->GetMapId();
	if(m_spellInfo->EffectMiscValue[i])
	{
		areaid = m_spellInfo->EffectMiscValue[i];
		AreaTable * at = dbcArea.LookupEntryForced(areaid);
		if(!at)
			return;
		mapid = at->mapId;
	}

	playerTarget->SetBindPoint(playerTarget->GetPositionX(), playerTarget->GetPositionY(), playerTarget->GetPositionZ(), mapid, areaid);

	data.Initialize(SMSG_BINDPOINTUPDATE);
	data << playerTarget->GetBindPositionX() << playerTarget->GetBindPositionY() << playerTarget->GetBindPositionZ() << playerTarget->GetBindMapId() << playerTarget->GetBindZoneId();
	playerTarget->GetSession()->SendPacket( &data );

	data.Initialize(SMSG_PLAYERBOUND);
	data << m_caster->GetGUID() << playerTarget->GetBindZoneId();
	playerTarget->GetSession()->SendPacket(&data);
}

void Spell::SpellEffectQuestComplete(uint32 i) // Quest Complete
{
	//misc value is id of the quest to complete
}

//wand->
void Spell::SpellEffectWeapondamageNoschool(uint32 i) // Weapon damage + (no School)
{
	if(!unitTarget)         return;
	if(!m_caster->IsUnit()) return;

	((Unit *)m_caster)->Strike( unitTarget, ( GetType() == DEFENSE_TYPE_RANGED ? RANGED : MELEE ), m_spellInfo, damage, 0, 0, false, true );
}

void Spell::SpellEffectAddExtraAttacks(uint32 i) // Add Extra Attacks
{
	if(!m_caster->IsUnit()) return;

	((Unit *)m_caster)->m_extraattacks += damage;		
}

void Spell::SpellEffectDodge(uint32 i)
{
	//i think this actually enbles the skill to be able to dodge melee+ranged attacks
	//value is static and sets value directly which will be modified by other factors
	//this is only basic value and will be overwiten elsewhere !!!
//	if(unitTarget->IsPlayer())
//		unitTarget->SetFloatValue(PLAYER_DODGE_PERCENTAGE,damage);
}

void Spell::SpellEffectParry(uint32 i)
{
	if(unitTarget)
		unitTarget->setcanperry(true);
}

void Spell::SpellEffectBlock(uint32 i)
{
	//i think this actually enbles the skill to be able to block melee+ranged attacks
	//value is static and sets value directly which will be modified by other factors
//	if(unitTarget->IsPlayer())
//		unitTarget->SetFloatValue(PLAYER_BLOCK_PERCENTAGE,damage);
}

void Spell::SpellEffectCreateItem(uint32 i) // Create item 
{
	if(!m_caster->IsPlayer()) return;

	
	Item* newItem;
	Item *add;
	uint8 slot;
	SlotResult slotresult;

	skilllinespell* skill = objmgr.GetSpellSkill(m_spellInfo->Id);
    //Log.Notice("[Spell::SpellEffectCreateItem]","m_spellInfo %u",m_spellInfo->Id);
	for(int j=0; j<3; j++) // now create the Items
	{
        if (!m_spellInfo->EffectItemType[j]) continue; // Pas la peine de crée si EffectItemType=0

		ItemPrototype *m_itemProto;
		m_itemProto = ItemPrototypeStorage.LookupEntry( m_spellInfo->EffectItemType[j] );
		if (!m_itemProto)
			 continue;
        //Log.Notice("[Spell::SpellEffectCreateItem]","m_spellInfo %u, EffectItemType[%u],%u",m_spellInfo->Id,j,m_spellInfo->EffectItemType[j]);
		uint32 item_count = 0;
		if (m_itemProto->Class != ITEM_CLASS_CONSUMABLE || m_spellInfo->SpellFamilyName != 3) //SpellFamilyName 3 is mage
			item_count = damage;
		else if(((Player *)m_caster)->getLevel() >= m_spellInfo->spellLevel)
			item_count = ((((Player *)m_caster)->getLevel() - (m_spellInfo->spellLevel-1))*damage);

		if(!item_count)
			item_count = damage;

		//conjure water ranks 7,8 & 9 and conjure food ranks 7 & 8 have different starting amounts
		// tailoring specializations get +1 cloth bonus
		switch(m_spellInfo->Id)
		{
		case __Conjure_Food_Rank_7: // 27389
		case __Conjure_Water_Rank_7: // 10140
		case 37420: //Conjure Water 8
			item_count += 8;
			break;
		case __Shadowcloth: // 36686
			if(((Player *)m_caster)->HasSpell(26801)) item_count += 1;
			break;
		case __Primal_Mooncloth: // 26751
			if(((Player *)m_caster)->HasSpell(26798)) item_count += 1;
			break;
		case __Spellcloth: // 31373
			if(((Player *)m_caster)->HasSpell(26797)) item_count += 1;
			break;
		}

		if (skill && skill->skilline == SKILL_ALCHEMY)
		{
			//Potion Master
			if (strstr(m_itemProto->Name1, "Potion"))
			{
				if(((Player *)m_caster)->HasSpell(__Potion_Master)) // 28675
					while (Rand(20) && item_count<10) item_count++;
			}
			//Elixir Master
			if (strstr(m_itemProto->Name1, "Elixir") || strstr(m_itemProto->Name1, "Flask"))
			{
				if(((Player *)m_caster)->HasSpell(__Elixir_Master )) // 28677
					while (Rand(20) && item_count<10) item_count++;
			}
			//Transmutation Master
			if (m_spellInfo->Category == 310)
			{
				if(((Player *)m_caster)->HasSpell(__Transmutation_Master)) // 28672
					while (Rand(20) && item_count<10) item_count++;
			}
		}

		// item count cannot be more than allowed in a single stack
		if (item_count > m_itemProto->MaxCount)
			item_count = m_itemProto->MaxCount;

		// item count cannot be more than item unique value
		if (m_itemProto->Unique && item_count > m_itemProto->Unique)
			item_count = m_itemProto->Unique;

		if(((Player *)m_caster)->GetItemInterface()->CanReceiveItem(m_itemProto, item_count)) //reversed since it sends >1 as invalid and 0 as valid
		{
			SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
			return;
		}

		slot = 0;
		add = ((Player *)m_caster)->GetItemInterface()->FindItemLessMax(m_spellInfo->EffectItemType[j],1, false);
		if (!add)
		{
			slotresult = ((Player *)m_caster)->GetItemInterface()->FindFreeInventorySlot(m_itemProto);
			if(!slotresult.Result)
			{
				  SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
				  return;
			}
			
			newItem=objmgr.CreateItem(m_spellInfo->EffectItemType[i],((Player *)m_caster));
			newItem->SetUInt64Value(ITEM_FIELD_CREATOR,m_caster->GetGUID());
			newItem->SetUInt32Value(ITEM_FIELD_STACK_COUNT, item_count);

			if (m_itemProto->RandomPropId)
			{
				RandomProps * iRandomProperty = lootmgr.GetRandomProperties(m_itemProto);
				newItem->SetRandomProperty(iRandomProperty->ID);
				newItem->ApplyRandomProperties(false);
			}
			if (m_itemProto->RandomSuffixId)
			{
				ItemRandomSuffixEntry * iRandomSuffix = lootmgr.GetRandomSuffix(m_itemProto);
				newItem->SetRandomSuffix(iRandomSuffix->id);
				newItem->ApplyRandomProperties(false);
			}

			if(((Player *)m_caster)->GetItemInterface()->SafeAddItem(newItem,slotresult.ContainerSlot, slotresult.Slot))
			{
				/*WorldPacket data(45);
				((Player *)m_caster)->GetSession()->BuildItemPushResult(&data, ((Player *)m_caster)->GetGUID(), 1, item_count, m_spellInfo->EffectSpellGroupRelation[i] ,0,0xFF,1,0xFFFFFFFF);
				((Player *)m_caster)->SendMessageToSet(&data, true);*/
				((Player *)m_caster)->GetSession()->SendItemPushResult(newItem,true,false,true,true,slotresult.ContainerSlot,slotresult.Slot,item_count);
			} 
			else 
			{
				//delete newItem;
                newItem = NULL;
			}

			if(skill)
				DetermineSkillUp(skill->skilline);
		} 
		else 
		{
			//scale item_count down if total stack will be more than 20
			if(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + item_count > 20)
			{
				uint32 item_count_filled;
				item_count_filled = 20 - add->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
				add->SetCount(20);
				add->m_isDirty = true;

				slotresult = ((Player *)m_caster)->GetItemInterface()->FindFreeInventorySlot(m_itemProto);
				if(!slotresult.Result)
					item_count = item_count_filled;
				else
				{
					//Log.Notice("[Spell::SpellEffectCreateItem]","m_spellInfo %u",m_spellInfo->Id);
					newItem = objmgr.CreateItem(m_spellInfo->EffectItemType[i],((Player *)m_caster));
					newItem->SetUInt64Value(ITEM_FIELD_CREATOR,m_caster->GetGUID());
					newItem->SetUInt32Value(ITEM_FIELD_STACK_COUNT, item_count - item_count_filled);
					if(!((Player *)m_caster)->GetItemInterface()->SafeAddItem(newItem,slotresult.ContainerSlot, slotresult.Slot))
					{
						delete newItem;
						item_count = item_count_filled;
					}
					else
						((Player *)m_caster)->GetSession()->SendItemPushResult(newItem, true, false, true, true, slotresult.ContainerSlot, slotresult.Slot, item_count-item_count_filled);
                }
			}
			else
			{
				add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + item_count);
				add->m_isDirty = true;
				((Player *)m_caster)->GetSession()->SendItemPushResult(add, true,false,true,false,((Player *)m_caster)->GetItemInterface()->GetBagSlotByGuid(add->GetGUID()),0xFFFFFFFF,item_count);
			}

			/*WorldPacket data(45);
			p_caster->GetSession()->BuildItemPushResult(&data, p_caster->GetGUID(), 1, item_count, m_spellInfo->EffectSpellGroupRelation[i] ,0,0xFF,1,0xFFFFFFFF);
			p_caster->SendMessageToSet(&data, true);*/
			if(skill)
				DetermineSkillUp(skill->skilline);
		}
	}	   
}

void Spell::SpellEffectWeapon(uint32 i)
{
	if( playerTarget == NULL )
		return;

	uint32 skill = 0;
	uint32 spell = 0;

	switch( this->m_spellInfo->Id )
	{
	case 201:    // one-handed swords
		{
			skill = SKILL_SWORDS;
		}break;
	case 202:   // two-handed swords
		{
			skill = SKILL_2H_SWORDS;
		}break;
	case 203:   // Unarmed
		{
			skill = SKILL_UNARMED;
		}break;
	case 199:   // two-handed maces
		{
			skill = SKILL_2H_MACES;
		}break;
	case 198:   // one-handed maces
		{
			skill = SKILL_MACES;
		}break;
	case 197:   // two-handed axes
		{
			skill = SKILL_2H_AXES;
		}break;
	case 196:   // one-handed axes
		{
			skill = SKILL_AXES;
		}break;
	case 5011: // crossbows
		{
			skill = SKILL_CROSSBOWS;
			spell = SPELL_RANGED_GENERAL;
		}break;
	case 227:   // staves
		{
			skill = SKILL_STAVES;
		}break;
	case 1180:  // daggers
		{
			skill = SKILL_DAGGERS;
		}break;
	case 200:   // polearms
		{
			skill = SKILL_POLEARMS;
		}break;
	case 15590: // fist weapons
		{
			skill = SKILL_UNARMED;
		}break;
	case 264:   // bows
		{
			skill = SKILL_BOWS;
			spell = SPELL_RANGED_GENERAL;
		}break;
	case 266: // guns
		{
			skill = SKILL_GUNS;
			spell = SPELL_RANGED_GENERAL;
		}break;
	case 2567:  // thrown
		{
			skill = SKILL_THROWN;
		}break;
	case 5009:  // wands
		{
			skill = SKILL_WANDS;
			spell = SPELL_RANGED_GENERAL;
		}break;
	//case 3386:  // spears
	//	skill = 0;   // ??!!
	//	break;
	default:
		{
			skill = 0;
			sLog.outDebug("WARNING: Could not determine skill for spell id %d (SPELL_EFFECT_WEAPON)", this->m_spellInfo->Id);
		}break;
	}

	// Don't add skills to players logging in.
	/*if((m_spellInfo->attributes & ATTRIBUTES_PASSIVE) && playerTarget->m_TeleportState == 1)
		return;*/

	if(skill)
	{
		if(spell)
			playerTarget->addSpell(spell);
		
		// if we do not have the skill line
		if(!playerTarget->_HasSkillLine(skill))
		{
			playerTarget->_AddSkillLine(skill, 1, playerTarget->getLevel()*5);
		}
		else // unhandled.... if we have the skill line
		{
		}
	}
}

void Spell::SpellEffectDefense(uint32 i)
{
	//i think this actually enbles the skill to be able to use defense
	//value is static and sets value directly which will be modified by other factors
	//this is only basic value and will be overwiten elsewhere !!!
//	if(unitTarget->IsPlayer())
//		unitTarget->SetFloatValue(UNIT_FIELD_RESISTANCES,damage);
}

void Spell::SpellEffectPersistentAA(uint32 i) // Persistent Area Aura
{
	if(m_AreaAura == true || !m_caster->IsInWorld())
		return;
	//create only 1 dyn object
	int32 dur = GetDuration(INVOC_DURATION);
	float r = GetRadius(i);

	//Note: this code seems to be useless
	//this must be only source point or dest point
	//this AREA aura it's apllied on area
	//it can'be on unit or self or item or object
	//uncomment it if i'm wrong
	//We are thinking in general so it might be useful later DK
	
	// grep: this is a hack!
	// our shitty dynobj system doesnt support GO casters, so we gotta
	// kinda have 2 summoners for traps that apply AA.
	DynamicObject * dynObj = m_caster->GetMapMgr()->CreateDynamicObject();
	 
	if(m_caster->IsGO() && ((GameObject *)m_caster)->m_summoner && !unitTarget)
	{
		//only 1 trap can be active at any one time
		if (((GameObject *)m_caster)->GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID) == GAMEOBJECT_TYPE_TRAP && ((GameObject *)m_caster)->m_summoner->dynObj != NULL)
			((GameObject *)m_caster)->m_summoner->dynObj->m_aliveDuration = 1;

		Unit * caster = ((GameObject *)m_caster)->m_summoner;
		dynObj->Create(caster, this, m_caster->GetPositionX(), m_caster->GetPositionY(), 
			m_caster->GetPositionZ(), (uint32) dur, r);
		m_AreaAura = true;
		return;
	}
		
	switch(m_targets.m_targetMask)
	{		
	case TARGET_FLAG_SELF:
		{
			dynObj->Create(m_caster->IsUnit() ? ((Unit *)m_caster) : ((GameObject *)m_caster)->m_summoner, this,	((Unit *)m_caster)->GetPositionX(), 
				((Unit *)m_caster)->GetPositionY(), ((Unit *)m_caster)->GetPositionZ(), dur,r);		 
		}break;
	case TARGET_FLAG_UNIT:
		{
			if(!unitTarget||!unitTarget->isAlive())
			{
				dynObj->Remove();
				return;
			}

			dynObj->Create(m_caster->IsUnit() ? ((Unit *)m_caster) : ((GameObject *)m_caster)->m_summoner, this, unitTarget->GetPositionX(), 
				unitTarget->GetPositionY(), unitTarget->GetPositionZ(),dur, r);
		}break;
	case TARGET_FLAG_OBJECT:
		{
			if(!unitTarget||!unitTarget->isAlive())
			{
				dynObj->Remove();
				return;
			}

			dynObj->Create(m_caster->IsUnit() ? ((Unit *)m_caster) : ((GameObject *)m_caster)->m_summoner, this, unitTarget->GetPositionX(), unitTarget->GetPositionY(), unitTarget->GetPositionZ(),
				dur, r);
		}break;
	case TARGET_FLAG_SOURCE_LOCATION:
		{
			dynObj->Create(m_caster->IsUnit() ? ((Unit *)m_caster) : ((GameObject *)m_caster)->m_summoner, this, m_targets.m_srcX,
				m_targets.m_srcY, m_targets.m_srcZ, dur,r);
		}break;
	case TARGET_FLAG_DEST_LOCATION:
		{
			dynObj->Create(m_caster->IsUnit() ? ((Unit *)m_caster) : ((GameObject *)m_caster)->m_summoner, this,
				m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ,dur,r);
		}break;
	default:
		return;
	}

	dynObj->SetInstanceID(m_caster->GetInstanceID());
	
	if(m_caster->IsUnit() && m_spellInfo->channelInterruptFlags > 0)
	{
		((Unit *)m_caster)->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT,dynObj->GetGUID());
		((Unit *)m_caster)->SetUInt32Value(UNIT_CHANNEL_SPELL,m_spellInfo->Id);
	}
	m_AreaAura = true;	
}

void Spell::SpellEffectSummon(uint32 i) // Summon
{
	if(!m_caster->IsInWorld()) return;
	if(!m_caster->IsUnit())    return;
	if (summonveh == true)	   return;
	
	if (((Unit *)m_caster)->m_summonedCreatures.size() > 0) return; // Deja summon (Branruz)

	SummonPropertiesEntry* props=dbcSummonProperties.LookupEntry(m_spellInfo->EffectMiscValueB[i]);
	if (props == NULL)
		return;

	//must remove summons in the same slot :)
	if (((Unit *)m_caster)->m_summonslot[props->slot] != 0 && props->slot != 0) //slot 0 can have infinite of them up
		((Unit *)m_caster)->m_summonslot[props->slot]->SafeDelete();

	//damage is health for totems
	//int32 summonamount = props->type == SUMMON_TYPE_TOTEM? 1 : damage;
	int32 summonamount = 1;

	if (props->flags & 2) summonamount = 1;

	for (int32 amount=0; amount<summonamount; ++amount)
	{

		//if we have no dest, the dest is the caster position
		float x=m_targets.m_destX, y=m_targets.m_destY, z=m_targets.m_destZ, o=0;

		if (x == 0 && y == 0 && z == 0)
		{
			if (unitTarget != NULL)
			{
			 x = unitTarget->GetPositionX();
			 y = unitTarget->GetPositionY();
			 z = unitTarget->GetPositionZ();
			 o = unitTarget->GetOrientation();
			}
			else
			{
				x = m_caster->GetPositionX();
				y = m_caster->GetPositionY();
				z = m_caster->GetPositionZ();
				o = m_caster->GetOrientation();
			}
		}


		switch(props->slot)
		{
		case 1: 
			x -= 1.5f;
			y -= 1.5f;
			break;
		case 2: 
			x -= 1.5f;
			y += 1.5f;
			break;
		case 3:  
			x += 1.5f;
			y -= 1.5f;
			break;
		case 4: 
			x += 1.5f;
			y += 1.5f;
			break;
		default:
			break;
		}

		/* This is for summon water elemenal, etc */
		CreatureInfo * ci = CreatureNameStorage.LookupEntry(m_spellInfo->EffectMiscValue[i]);
		CreatureProto * cp = CreatureProtoStorage.LookupEntry(m_spellInfo->EffectMiscValue[i]);
		
		if( !ci || !cp )
			return;

		Creature* summon = NULL;
		Vehicle* veh = NULL;
		
		if (props->controltype == CREATURE_TYPE_PET) //pets need rewrote, this is SHIT
		{
			summon=objmgr.CreatePet();
			summon->SetInstanceID(((Unit *)m_caster)->GetInstanceID());
			static_cast<Pet*>(summon)->CreateAsSummon(m_spellInfo->EffectMiscValue[i], ci, NULL, 
				                               ((Unit *)m_caster), m_spellInfo, 1, GetDuration(INVOC_DURATION));
			((Unit *)m_caster)->SetUInt64Value(UNIT_FIELD_SUMMON, summon->GetGUID());
		}
		else if (props->controltype == CREATURE_TYPE_VEHICLE)
		{
			if(cp->vehicle_entry <0 )
			{
			
				Log.Error("SpellEffect", "Creature %u, has invalid vehicle_entry, please check your creature_proto table", m_spellInfo->EffectMiscValue[i]);
				return;
			}
			// Note Randdrick : Cas ou le vehicule est crée à partir d'un spell de type monture
			if( i != 0 && m_spellInfo->Effect[i-1] == SPELL_EFFECT_APPLY_AURA && m_spellInfo->EffectApplyAuraName[i-1] == SPELL_AURA_MOUNTED )
			{
				veh = ((Unit *)m_caster)->GetMapMgr()->CreateVehicle( m_spellInfo->EffectMiscValue[i] );
				if(veh == NULL)
					return;
				veh->m_CreatedFromSpell = true;
				veh->m_mountSpell = m_spellInfo->EffectBasePoints[i];
				veh->Load( cp, x, y, z, o);
				veh->SetInstanceID( ((Unit *)m_caster)->GetInstanceID() );
				veh->PushToWorld( ((Unit *)m_caster)->GetMapMgr() ); // we can do this safely since we're in the mapmgr's context
				veh->InitSeats( cp->vehicle_entry, ((Player *)m_caster) );
			}
			else // Note Randdrick : Cas ou le vehicule est crée à partir du spell Summon
			{
				summon = ((Unit *)m_caster)->GetMapMgr()->CreateVehicle( m_spellInfo->EffectMiscValue[i] );
				summon->Load( cp, x, y, z, o);
				summon->SetInstanceID(((Unit *)m_caster)->GetInstanceID());
								
				if (!summon || !summon->IsVehicle())
					return;
				
				veh = ((Vehicle *)summon);
				
				if(!veh->IsInWorld())
					veh->PushToWorld( ((Unit *)m_caster)->GetMapMgr() );

				veh->SendSpells(cp->Id, ((Player *)m_caster));
				
				if (m_spellInfo->EffectBasePoints[i])
				{
					SpellEntry* spellProto = dbcSpell.LookupEntry(m_spellInfo->EffectBasePoints[i]+1);
					if (spellProto)
						((Unit *)m_caster)->CastSpell(summon,spellProto,true);
				}
			}
			summonveh = true;
		}
		else
		{
			summon=((Unit *)m_caster)->GetMapMgr()->CreateCreature(cp->Id);
			summon->Load(cp, x, y, z, o);
		}

		if (summon == NULL) //something went wrong
			return;

		//summons should never respawn
		summon->m_noRespawn = true;


		//wild shouldnt set anything...
		if (props->type != SUMMON_TYPE_WILD)
		{
			//factions
			summon->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, props->factionId? props->factionId : ((Unit *)m_caster)->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			summon->_setFaction();

			summon->GetAIInterface()->SetUnitToFollow(((Unit *)m_caster));
			summon->GetAIInterface()->SetUnitToFollowAngle(float(-(M_PI/2)));
			summon->GetAIInterface()->SetFollowDistance(3.0f);
			summon->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, ((Unit *)m_caster)->GetGUID());
			summon->SetUInt64Value(UNIT_FIELD_CREATEDBY, ((Unit *)m_caster)->GetGUID());
			summon->SetUInt32Value(UNIT_FIELD_FLAGS, ((Unit *)m_caster)->GetUInt32Value(UNIT_FIELD_FLAGS));
			summon->SetSummonOwner(((Unit *)m_caster));
			summon->SetSummonSlot(props->slot);
			((Unit *)m_caster)->m_summonslot[props->slot] = summon;
		}

		if(m_spellInfo->EffectMiscValue[i] == 510 && summon->IsPet())	// Water Elemental
		{
			static_cast<Pet*>(summon)->AddSpell(dbcSpell.LookupEntry(31707), true);
			static_cast<Pet*>(summon)->AddSpell(dbcSpell.LookupEntry(33395), true);
		}

		//guardians have multiple things, and they move around the caster when theres more of them, all cool like
		if (props->type == SUMMON_TYPE_GUARDIAN)
		{
			summon->GetAIInterface()->Init(summon, AITYPE_PET, MOVEMENTTYPE_NONE, ((Unit *)m_caster));
			((Unit *)m_caster)->m_summonedCreatures.insert(summon);

			//we now have to fix every other angle ROFL
			if (((Unit *)m_caster)->m_summonedCreatures.size() > 0)
			{
				float angle_for_each_spawn = -float(M_PI) * 2 / ((Unit *)m_caster)->m_summonedCreatures.size();
				int i=0;
				for (std::set<Creature*>::iterator itr=((Unit *)m_caster)->m_summonedCreatures.begin(); itr!=((Unit *)m_caster)->m_summonedCreatures.end(); ++itr, ++i)
				{
					(*itr)->GetAIInterface()->SetUnitToFollow(((Unit *)m_caster));
					(*itr)->GetAIInterface()->SetUnitToFollowAngle(angle_for_each_spawn * i);
					(*itr)->GetAIInterface()->SetFollowDistance(3.0f);
				}
			}
		}

		if (props->type == SUMMON_TYPE_TOTEM)
		{
			SpellEntry * TotemSpell = ObjectMgr::getSingleton().GetTotemSpell(m_spellInfo->Id);
			if(TotemSpell == 0) 
			{
				sLog.outDebug("Totem %s (%u) does not have any spells to cast, exiting\n", ci->Name, ci->Id);
				summon->SafeDelete();
				return;
			}

			//tell client to show the cool UI, we need to delay this because the totem isnt pushed yet
			if (m_caster->IsPlayer())
			{
				WorldPacket* data = new WorldPacket(SMSG_TOTEM_CREATED, 17);
				*data << uint8(props->slot - 1); // -1 because first totem is slot 1 in dbcs
				*data << summon->GetGUID();
				*data << GetDuration(INVOC_DURATION);
				*data << m_spellInfo->Id;
				((Player *)m_caster)->delayedPackets.add(data);
			}
			
			
			uint32 displayID = 0;

			if( m_caster->IsPlayer() && (((Player *)m_caster)->GetTeam() == FACTION_ALLY ) )
			{
				if ( ci->Female_DisplayID != 0 )
				{
					displayID = ci->Female_DisplayID; //this is the nice solution provided by emsy
				}
				else //this is the case when you are using a blizzlike db
				{   // A VERIFIER POURQUOI (Branruz)
					if( ci->Male_DisplayID == 4587 )      displayID = 19075;
					else if( ci->Male_DisplayID == 4588 ) displayID = 19073;
					else if( ci->Male_DisplayID == 4589 ) displayID = 19074;
					else if( ci->Male_DisplayID == 4590 ) displayID = 19071;
					else if( ci->Male_DisplayID == 4683 ) displayID = 19074;
					else displayID = ci->Male_DisplayID;
				}
			}
			else
			{
				displayID = ci->Male_DisplayID;
			}

			// Set up the creature.
			summon->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, 0);

			summon->SetUInt32Value(OBJECT_FIELD_ENTRY, ci->Id);
			summon->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
			//summon->SetUInt32Value(UNIT_FIELD_HEALTH, damage);
			//summon->SetUInt32Value(UNIT_FIELD_MAXHEALTH, damage);
			summon->SetUInt32Value(UNIT_FIELD_POWER3, ((Unit *)m_caster)->getLevel() * 30);   // Focus
			summon->SetUInt32Value(UNIT_FIELD_MAXPOWER3, ((Unit *)m_caster)->getLevel() * 30);
			summon->SetUInt32Value(UNIT_FIELD_LEVEL, ((Unit *)m_caster)->getLevel());
			//summon->SetUInt32Value(UNIT_FIELD_BYTES_0, (1 << 8) | (2 << 16) | (1 << 24));
			//summon->SetByte(UNIT_FIELD_BYTES_0,0,0);
			summon->SetByte(UNIT_FIELD_BYTES_0,1,1);
			summon->SetByte(UNIT_FIELD_BYTES_0,2,2); // Power type Focus
			summon->SetByte(UNIT_FIELD_BYTES_0,3,1); // ou Power Type ? (Rage ?)

			summon->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_SELF_RES);
			summon->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 1.0f);
			summon->SetFloatValue(UNIT_FIELD_COMBATREACH, 1.0f);
			summon->SetUInt32Value(UNIT_FIELD_DISPLAYID, displayID);
			summon->SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, ci->Male_DisplayID); //blizzlike :P
			summon->SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
			summon->SetUInt32Value(UNIT_CREATED_BY_SPELL, m_spellInfo->Id);
			summon->SetUInt32Value(UNIT_FIELD_BYTES_2, 1 | (0x28 << 8));

			summon->InheritSMMods(((Unit *)m_caster));

			uint32 j;
			for( j = 0; j < 3; ++j )
			{
				if( TotemSpell->Effect[j] == SPELL_EFFECT_APPLY_AREA_AURA || TotemSpell->Effect[j] == SPELL_EFFECT_PERSISTENT_AREA_AURA || TotemSpell->EffectApplyAuraName[j] == SPELL_AURA_PERIODIC_TRIGGER_SPELL )
				{
					break;
				}
			}

			if(j != 3)
			{
				// We're an area aura. Simple. Disable AI and cast the spell.
				summon->DisableAI();
				summon->GetAIInterface()->totemspell = m_spellInfo;

				Spell * pSpell = new Spell(summon, TotemSpell, true, 0);

				SpellCastTargets targets;
				targets.m_destX = summon->GetPositionX();
				targets.m_destY = summon->GetPositionY();
				targets.m_destZ = summon->GetPositionZ();
				targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;

				//we need to push to world here if we are not in world, spell will require us in world
				if (!summon->IsInWorld())
					summon->PushToWorld(m_caster->GetMapMgr());

				pSpell->prepare(&targets);
			}
			else
			{
				// We're a casting totem. Switch AI on, and tell it to cast this spell.
				summon->EnableAI();
				summon->GetAIInterface()->Init(summon, AITYPE_TOTEM, MOVEMENTTYPE_NONE, ((Unit *)m_caster));
				summon->GetAIInterface()->totemspell = TotemSpell;
				uint32 timer = 0;	// need a proper resource for this.
				uint32 time = 3000; // totems should cast instantly first, except for fire nova

				switch(TotemSpell->Id)
				{
				case 8349: //Fire Nova Totem 1
				case 8502: //Fire Nova Totem 2
				case 8503: //Fire Nova Totem 3
				case 11306: //Fire Nova Totem 4
				case 11307: //Fire Nova Totem 5
				case 25535: //Fire Nova Totem 6
				case 25537: //Fire Nova Totem 7
					timer =  4000;
					time = 4000;
					break;
				case 33663:
				case 32982:
					time = 180000; //3mins, duration 2mins, never cast twice
				default:break;
				}

				summon->GetAIInterface()->m_totemspelltimer = timer;
				summon->GetAIInterface()->m_totemspelltime = time;
			}
		}

		if (!summon->IsInWorld()) summon->PushToWorld(((Unit *)m_caster)->GetMapMgr());
/*
		if (props->type == SUMMON_TYPE_TOTEM)
			sEventMgr.AddEvent(summon, &Creature::SafeDelete, EVENT_SUMMON_EXPIRE, GetDuration(), 1, 0);
		else
			sEventMgr.AddEvent(static_cast<Object*>(summon), &Object::KillSelf, EVENT_SUMMON_EXPIRE, GetDuration(), 1, 0);
*/

		if (props->flags & 2)
		{
			summon->SetUInt32Value(UNIT_FIELD_HEALTH, damage);
			summon->SetUInt32Value(UNIT_FIELD_MAXHEALTH, damage);
		}

		if (GetDuration(INVOC_DURATION) > 0)
		{
			sEventMgr.AddEvent(static_cast<Object*>(summon), &Object::KillSelf, EVENT_SUMMON_EXPIRE, GetDuration(INVOC_DURATION), 1, 0);
			sEventMgr.AddEvent(summon, &Creature::SafeDelete, EVENT_SUMMON_EXPIRE, GetDuration(INVOC_DURATION) + 500, 1, 
				                                                                    EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}

		if (m_spellInfo->Id == 1122) //inferno...
		{
			Spell* s=new Spell(((Unit *)m_caster), dbcSpell.LookupEntry(11726), true, NULL);
			SpellCastTargets t(summon);
			s->prepare(&t);
		}
	}
}

void Spell::SpellEffectLeap(uint32 i) // Leap
{
	float radius = GetRadius(i);

	if(!m_caster->IsUnit()) return;

	// remove movement impeding auras
	((Unit *)m_caster)->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN);

	if(((Player *)m_caster)->m_bg && !((Player *)m_caster)->m_bg->HasStarted()) return;

	// just in case
	for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; ++i)
	{
		if( ((Unit *)m_caster)->m_auras[i] != NULL )
		{
			for(uint32 j = 0; j < 3; ++j)
			{
				if( ((Unit *)m_caster)->m_auras[i]->GetSpellProto()->EffectApplyAuraName[j] == SPELL_AURA_MOD_STUN || 
					((Unit *)m_caster)->m_auras[i]->GetSpellProto()->EffectApplyAuraName[j] == SPELL_AURA_MOD_ROOT )
				{
					((Unit *)m_caster)->m_auras[i]->Remove();
					break;
				}
			}
		}
	}

	MapInfo* info = WorldMapInfoStorage.LookupEntry(m_caster->GetMapId());
	if (info != NULL && info->collision == 0)
	{
		if (!m_caster->IsPlayer()) return;

		((Player *)m_caster)->blinked = true;

		WorldPacket data(SMSG_MOVE_KNOCK_BACK, 50);
		data << ((Player *)m_caster)->GetNewGUID();
		data << getMSTime();
		data << cosf(((Player *)m_caster)->GetOrientation()) << sinf(((Player *)m_caster)->GetOrientation());
		data << radius;
		data << float(-10.0f);
		((Player *)m_caster)->GetSession()->SendPacket(&data);
		//((Player *)m_caster)->SendMessageToSet(&data, true); // C'est quoi cette trame ? SMSG_MOVE_KNOCK_BACK
		((Player *)m_caster)->blinked = true;
		((Player *)m_caster)->DelaySpeedHack( 5000 );
		++((Player *)m_caster)->_heartbeatDisable;
		((Player *)m_caster)->z_axisposition = 0.0f;

		return;
	}

	float ori = ((Unit *)m_caster)->GetOrientation();				
	float posX = ((Unit *)m_caster)->GetPositionX()+(radius*(cosf(ori)));
	float posY = ((Unit *)m_caster)->GetPositionY()+(radius*(sinf(ori)));   
	float posZ = ((Unit *)m_caster)->GetPositionZ();                                 // +2.0 ?? SpellEffectLeap c'est quoi (Branruz)
	float z = CollideInterface.GetHeight(((Unit *)m_caster)->GetMapId(), posX, posY, ((Unit *)m_caster)->GetPositionZ() + 2.0f); 
	if(z == NO_WMO_HEIGHT)		// not found height, or on adt
		z = ((Unit *)m_caster)->GetMapMgr()->GetLandHeight(posX,posY,posZ);

	if( fabs( z - ((Unit *)m_caster)->GetPositionZ() ) >= 10.0f )
		return;

	LocationVector dest(posX, posY, z + 2.0f, ori);
	LocationVector destest(posX, posY, dest.z, ori);
	LocationVector src(((Unit *)m_caster)->GetPositionX(), ((Unit *)m_caster)->GetPositionY(), ((Unit *)m_caster)->GetPositionZ() + 2.0f);

	if(CollideInterface.GetFirstPoint(((Unit *)m_caster)->GetMapId(), src, destest, dest, -1.5f))
	{
		// hit an object new point is in dest.
		// is this necessary?
		//dest.z = CollideInterface.GetHeight(((Unit *)m_caster)->GetMapId(), dest.x, dest.y, dest.z + 2.0f);
	}
	else
		dest.z = z;

	dest.o = ((Unit *)m_caster)->GetOrientation();

	if (m_caster->IsPlayer())
	{
		((Player *)m_caster)->blinked = true;
		((Player *)m_caster)->SafeTeleport( ((Player *)m_caster)->GetMapId(), ((Player *)m_caster)->GetInstanceID(), dest );
		((Player *)m_caster)->DelaySpeedHack( 5000 );
		++((Player *)m_caster)->_heartbeatDisable;
		((Player *)m_caster)->z_axisposition = 0.0f;
	}
	else if (m_caster->IsUnit())
	{
		((Unit *)m_caster)->SetPosition(dest, false);
	}
}

/*void Spell::SpellEffectEnergize(uint32 i) // Energize
{
	if(!unitTarget || !unitTarget->isAlive())
		return;

	uint32 POWER_TYPE=UNIT_FIELD_POWER1+m_spellInfo->EffectMiscValue[i];

	uint32 curEnergy = (uint32)unitTarget->GetUInt32Value(POWER_TYPE);
	uint32 maxEnergy = (uint32)unitTarget->GetUInt32Value(POWER_TYPE+6);
	uint32 modEnergy;
	//yess there is always someone special : shamanistic rage - talent
	if(m_spellInfo->Id==30824)
		modEnergy = damage*GetUnitTarget()->GetAP()/100;
	//paladin illumination
	else if(m_spellInfo->Id==20272 && ProcedOnSpell)
	{
		SpellEntry *motherspell=dbcSpell.LookupEntry(pSpellId);
		if(motherspell)
			modEnergy = (motherspell->EffectBasePoints[0]+1)*ProcedOnSpell->manaCost/100;
	}
	
	//Paladin - Judgements of the wise
	else if(m_spellInfo->Id==31930 && ProcedOnSpell)
	{
			if(unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1)!= 0)
			modEnergy = uint32(unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1)*0.15);
	}	
	
	//paladin - Spiritual Attunement 
	else if(m_spellInfo->Id==31786 && ProcedOnSpell)
	{
		SpellEntry *motherspell=dbcSpell.LookupEntry(pSpellId);
		if(motherspell)
		{
			//heal amount from procspell (we only proced on a heal spell)
			uint32 healamt=0;
			if(ProcedOnSpell->Effect[0]==SPELL_EFFECT_HEAL || ProcedOnSpell->Effect[0]==SPELL_EFFECT_SCRIPT_EFFECT)
				healamt=ProcedOnSpell->EffectBasePoints[0]+1;
			else if(ProcedOnSpell->Effect[1]==SPELL_EFFECT_HEAL || ProcedOnSpell->Effect[1]==SPELL_EFFECT_SCRIPT_EFFECT)
				healamt=ProcedOnSpell->EffectBasePoints[1]+1;
			else if(ProcedOnSpell->Effect[2]==SPELL_EFFECT_HEAL || ProcedOnSpell->Effect[2]==SPELL_EFFECT_SCRIPT_EFFECT)
				healamt=ProcedOnSpell->EffectBasePoints[2]+1;
			modEnergy = (motherspell->EffectBasePoints[0]+1)*(healamt)/100;
		}
	}
	else if (m_spellInfo->Id==2687){
		modEnergy = damage;
		if( p_caster != NULL )
		{
			//for(set<uint32>::iterator itr = p_caster->mSpells.begin(); itr != p_caster->mSpells.end(); ++itr)
			{
				if(*itr == 12818)
					modEnergy += 60;
				else if(*itr == 12301)
					modEnergy += 30;
			}//
			if(p_caster->mSpells.find(12818) != p_caster->mSpells.end())
				modEnergy += 60;
			if(p_caster->mSpells.find(12301) != p_caster->mSpells.end())
				modEnergy += 30;
		}
	}
	else  
        modEnergy = damage;

	if(unitTarget->HasAura(17619)) 
	{ 
		modEnergy = uint32(modEnergy*1.4f);      
	} 
	SendHealManaSpellOnPlayer(u_caster, unitTarget, modEnergy, m_spellInfo->EffectMiscValue[i]);

	if(modEnergy + curEnergy > maxEnergy)
		unitTarget->SetUInt32Value(POWER_TYPE,maxEnergy);
	else
		unitTarget->SetUInt32Value(POWER_TYPE,modEnergy + curEnergy);
}*/

void Spell::SpellEffectEnergize(uint32 i) // Energize
{
	if(!unitTarget || !unitTarget->isAlive())
		return;

	uint32 modEnergy = 0;

	//Log.Warning("SpellEffectEnergize","m_spellInfo->Id : %u",m_spellInfo->Id);

	switch( m_spellInfo->Id )
	{
	case 57669: // replenishment - SPELL_HASH_REPLENISHMENT
		{
			modEnergy = float2int32(0.0025f * unitTarget->GetUInt32Value(UNIT_FIELD_BASE_MANA));
		}break;
	case 31930: // SPELL_HASH_JUDGEMENTS_OF_THE_WISE
		{
			if( m_caster->IsUnit() )
				modEnergy = float2int32(0.25f * ((Unit *)m_caster)->GetUInt32Value(UNIT_FIELD_BASE_MANA));
		}break;
	case 31786: // SPELL_HASH_SPIRITUAL_ATTUNEMENT
		{
			if( ProcedOnSpell )
			{
				SpellEntry *motherspell=dbcSpell.LookupEntry(pSpellId);
				if(motherspell)
				{
					//heal amount from procspell (we only proced on a heal spell)
					uint32 healamt=0;
					if(ProcedOnSpell->Effect[0]==SPELL_EFFECT_HEAL || ProcedOnSpell->Effect[0]==SPELL_EFFECT_SCRIPT_EFFECT)
						healamt=ProcedOnSpell->EffectBasePoints[0]+1;
					else if(ProcedOnSpell->Effect[1]==SPELL_EFFECT_HEAL || ProcedOnSpell->Effect[1]==SPELL_EFFECT_SCRIPT_EFFECT)
						healamt=ProcedOnSpell->EffectBasePoints[1]+1;
					else if(ProcedOnSpell->Effect[2]==SPELL_EFFECT_HEAL || ProcedOnSpell->Effect[2]==SPELL_EFFECT_SCRIPT_EFFECT)
						healamt=ProcedOnSpell->EffectBasePoints[2]+1;
					modEnergy = (motherspell->EffectBasePoints[0]+1)*(healamt)/100;
				}
			}
		}break;
	case 2687: // SPELL_HASH_BLOODRAGE
		{
			modEnergy = damage;
			if( m_caster->IsPlayer())
			{
				if(((Player *)m_caster)->mSpells.find(12818) != ((Player *)m_caster)->mSpells.end())
					modEnergy += 60;

				if(((Player *)m_caster)->mSpells.find(12301) != ((Player *)m_caster)->mSpells.end())
					modEnergy += 30;
			}
		}break;
	case 20268: // SPELL_HASH_JUDGEMENT_OF_WISDOM
	case 29442: // SPELL_HASH_MAGIC_ABSORPTION
		{
			if( unitTarget != NULL )
			{
				modEnergy = float2int32(unitTarget->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) * 0.02f);
			}
		}break;
	case 47755: // SPELL_HASH_RAPTURE
		{
			modEnergy = forced_basepoints[0];
		}break;
		// SPELL_HASH_MINOR_HEALING_POTION - SPELL_HASH_MINOR_MANA_POTION
		// SPELL_HASH_MINOR_REJUVENATION_POTION - 
		// SPELL_HASH_LESSER_HEALING_POTION - 
	default: // SPELL_HASH_RESTORE_MANA - SPELL_HASH_HEALING_POTION 
		{
			modEnergy = damage;
		}break;

	}
	((Unit *)m_caster)->Energize(unitTarget, m_spellInfo->logsId ? m_spellInfo->logsId : (pSpellId ? pSpellId : m_spellInfo->Id), 
		modEnergy, m_spellInfo->EffectMiscValue[i]);
}

void Spell::SpellEffectWeaponDmgPerc(uint32 i) // Weapon Percent damage
{
	if(!unitTarget)         return;
	if(!m_caster->IsUnit()) return;

	if( GetType() == DEFENSE_TYPE_MAGIC )
	{
		float fdmg = (float)CalculateDamage( ((Unit *)m_caster), unitTarget, MELEE, m_spellInfo );
		uint32 dmg = float2int32(fdmg*(float(damage/100.0f)));
		((Unit *)m_caster)->SpellNonMeleeDamageLog(unitTarget, m_spellInfo->Id, dmg, false, false, false);
	}
	else
	{
		uint32 _type;
		if( GetType() == DEFENSE_TYPE_RANGED )
			_type = RANGED;
		else
		{
			if (m_spellInfo->attributesExC & ATTRIBUTESEXC_OFFHAND) // 0x1000000)
				_type =  OFFHAND;
			else
				_type = MELEE;
		}

		((Unit *)m_caster)->Strike( unitTarget, _type, m_spellInfo, add_damage, damage, 0, false, true );
	}
}

void Spell::SpellEffectTriggerMissile(uint32 i) // Trigger Missile
{
	//Used by mortar team
	//Triggers area affect spell at destinatiom
	if(!m_caster)
		return;

	uint32 spellid = m_spellInfo->EffectTriggerSpell[i];
	if(spellid == 0) return;

	SpellEntry *spInfo = dbcSpell.LookupEntry(spellid);
	if(!spInfo) return;

	float spellRadius = GetRadius(i);

	//triggered missile handles the radius and stuff
	if (spellRadius == 0.0f)
	{
		Spell* newspell=new Spell(m_caster, spInfo, true, NULL);
		SpellCastTargets t;

		t=m_targets;

		newspell->prepare(&t);
		return;
	}

	for(std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
	{
		if(!((*itr)->IsUnit()) || !((Unit*)(*itr))->isAlive())
			continue;
		Unit *t=(Unit*)(*itr);
	
		float r;
		float d=m_targets.m_destX-t->GetPositionX();
		r=d*d;
		d=m_targets.m_destY-t->GetPositionY();
		r+=d*d;
		d=m_targets.m_destZ-t->GetPositionZ();
		r+=d*d;
		if(sqrt(r)> spellRadius)
			continue;
		
		if(!isAttackable(m_caster, (Unit*)(*itr)))//Fixme only enemy targets?
			continue;

		Spell*sp=new Spell(m_caster,spInfo,true,NULL);
		SpellCastTargets tgt(*itr);
		sp->prepare(&tgt);
	}
}

void Spell::SpellEffectOpenLock(uint32 i) // Open Lock
{
	if(!m_caster->IsPlayer())
	{
		Log.Warning("SpellEffectOpenLock","Player introuvable!");
		return;
	}
	else Log.Warning("SpellEffectOpenLock","%s deverouille un Go avec une clef.",((Player *)m_caster)->GetName());
	
	uint8 loottype = 0;

	uint32 locktype=m_spellInfo->EffectMiscValue[i];
	switch(locktype)
	{
	    // case LOCKTYPE_NONE : // Spell 29384 , Item: 23480 (Allumez les balises/Allumer les torches)
		case LOCKTYPE_PICKLOCK:
		{
			uint32 v = 0;
			uint32 lockskill = ((Player *)m_caster)->_GetSkillLineCurrent(SKILL_LOCKPICKING);

			if(itemTarget)
			{	
				if(!itemTarget->locked)
				return;
						
				LockEntry *lock = dbcLock.LookupEntry( itemTarget->GetProto()->LockId );
				if(!lock) return;
				for(int i=0;i<5;i++)
					if(lock->locktype[i] == 2 && lock->minlockskill[i] && lockskill >= lock->minlockskill[i])
					{
						v = lock->minlockskill[i];
						itemTarget->locked = false;
						itemTarget->SetFlag(ITEM_FIELD_FLAGS,4); // unlock
						DetermineSkillUp(SKILL_LOCKPICKING,v/5);
						break;
					}
			}
			else if(gameObjTarget)
			{
				GameObjectInfo *info = GameObjectNameStorage.LookupEntry(gameObjTarget->GetEntry());
				if(!info || gameObjTarget->GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE) == 0) return;
				LockEntry *lock = dbcLock.LookupEntry( info->raw.SpellFocus ); // 
				if(lock == 0)
					return;

				for(int i=0;i<5;i++)
				{
					if(lock->locktype[i] == 2 && lock->minlockskill[i] && lockskill >= lock->minlockskill[i])
					{
						v = lock->minlockskill[i];
						gameObjTarget->SetUInt32Value(GAMEOBJECT_FLAGS, GO_FLAG_NOT_IN_USE);
						gameObjTarget->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, 1);
						//Add Fill GO loot here
						if(gameObjTarget->loot.items.size() == 0)
						{
							lootmgr.FillGOLoot(&gameObjTarget->loot,gameObjTarget->GetEntry(), gameObjTarget->GetMapMgr() ? (gameObjTarget->GetMapMgr()->iInstanceMode ? true : false) : false);
							DetermineSkillUp(SKILL_LOCKPICKING,v/5); //to prevent free skill up
						}
						loottype = LOOT_CORPSE;
						//End of it
						break;
					}
				}
			}
		}
		case LOCKTYPE_HERBALISM:
		{
			if(!gameObjTarget) return;	  
			
			uint32 v = gameObjTarget->GetGOReqSkill();
			bool bAlreadyUsed = false;
		 
			if(Rand(100.0f)) // 3% chance to fail//why?
			{
				if( static_cast< Player* >( m_caster )->_GetSkillLineCurrent( SKILL_HERBALISM ) < v )
				{
					//SendCastResult(SPELL_FAILED_LOW_CASTLEVEL);
					return;
				}
				else
				{
					if( gameObjTarget->loot.items.size() == 0 )
					{
						lootmgr.FillGOLoot(&gameObjTarget->loot,gameObjTarget->GetEntry(), gameObjTarget->GetMapMgr() ? (gameObjTarget->GetMapMgr()->iInstanceMode ? true : false) : false);
					}
					else
						bAlreadyUsed = true;
				}
				loottype = LOOT_SKINNING;
			}
			else
			{
				/*
				if(rand()%100 <= 30)
				{
					//30% chance to not be able to reskin on fail
					((Creature*)unitTarget)->Skinned = true;
					WorldPacket *pkt=unitTarget->BuildFieldUpdatePacket(UNIT_FIELD_FLAGS,0);
					static_cast< Player* >( m_caster )->GetSession()->SendPacket(pkt);
					delete pkt;

				}*/
				SendCastResult(SPELL_FAILED_TRY_AGAIN);
			}
			//Skill up
			if(!bAlreadyUsed) //Avoid cheats with opening/closing without taking the loot
				DetermineSkillUp(SKILL_HERBALISM,v/5); 
		}
		break;
		case LOCKTYPE_MINING:
		{
			if(!gameObjTarget) return;

			uint32 v = gameObjTarget->GetGOReqSkill();
			bool bAlreadyUsed = false;

			if( Rand( 100.0f ) ) // 3% chance to fail//why?
			{
				if( static_cast< Player* >( m_caster )->_GetSkillLineCurrent( SKILL_MINING ) < v )
				{
					//SendCastResult(SPELL_FAILED_LOW_CASTLEVEL);
					return;
				}
				else if( gameObjTarget->loot.items.size() == 0 )
				{
					lootmgr.FillGOLoot(&gameObjTarget->loot,gameObjTarget->GetEntry(), gameObjTarget->GetMapMgr() ? (gameObjTarget->GetMapMgr()->iInstanceMode ? true : false) : false);
				}	
				else
					bAlreadyUsed = true;

				loottype = LOOT_SKINNING;
			}
			else
			{
				SendCastResult(SPELL_FAILED_TRY_AGAIN);
			}
			//Skill up
			if(!bAlreadyUsed) //Avoid cheats with opening/closing without taking the loot
				DetermineSkillUp(SKILL_MINING,v/5);
		}
		break;
		case LOCKTYPE_SLOW_OPEN: // used for BG go's
		{
			if(!gameObjTarget ) return;
			if(m_caster->IsPlayer() && ((Player *)m_caster)->m_bg)
			{
			 if( ((Player *)m_caster)->m_bg->HookSlowLockOpen(gameObjTarget,((Player *)m_caster),this)) return;
			}

			uint32 spellid = gameObjTarget->GetInfo()->goober.spellId /*Unknown1*/ ? 23932 : gameObjTarget->GetInfo()->goober.spellId; //Unknown1
			SpellEntry *en=dbcSpell.LookupEntry(spellid);
			Spell *sp=new Spell(m_caster,en,true,NULL);
			SpellCastTargets tgt;
			tgt.m_target=gameObjTarget;
			sp->prepare(&tgt);
			return;
		}	
		break;
		case LOCKTYPE_QUICK_CLOSE:
			{
				if(!gameObjTarget ) return;
				gameObjTarget->EventCloseDoor();
			}
		break;
		default://not profession
		{
			if(!gameObjTarget ) 
			{
				Log.Warning("SpellEffectOpenLock","pas de gameObjTarget.");
				return;
			}

			switch(gameObjTarget->GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID))
			{
			 // Activation/deactivation
             case GAMEOBJECT_TYPE_BUTTON :
				   // Log.Warning("SpellEffectOpenLock","GAMEOBJECT_TYPE_BUTTON: Activation/deactivation.");
				   // if(gameObjTarget->GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE))
				   //      gameObjTarget->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE,0);
				   //	else 
				   //		 gameObjTarget->SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE,1);   
			 // Tel Que
			 case GAMEOBJECT_TYPE_GOOBER : 
				    Log.Warning("SpellEffectOpenLock","Call Onactivate()");
					CALL_GO_SCRIPT_EVENT(gameObjTarget, OnActivate)(static_cast<Player*>(m_caster));
			  default : break;
			}
			
			if(sQuestMgr.OnActivateQuestGiver(gameObjTarget, ((Player *)m_caster)))
				return;

			if(sQuestMgr.OnGameObjectActivate(((Player *)m_caster), gameObjTarget))
			{
				((Player *)m_caster)->UpdateNearbyGameObjects();
				return;
			}

			if(gameObjTarget->loot.items.size() == 0)
			{
				lootmgr.FillGOLoot(&gameObjTarget->loot,gameObjTarget->GetEntry(), gameObjTarget->GetMapMgr() ? (gameObjTarget->GetMapMgr()->iInstanceMode ? true : false) : false);
			}
			loottype= LOOT_CORPSE ;
		}
		break;
	};
	if( gameObjTarget != NULL && (gameObjTarget->GetByte(GAMEOBJECT_BYTES_1, 1) == GAMEOBJECT_TYPE_CHEST))
		static_cast< Player* >( m_caster )->SendLoot( gameObjTarget->GetGUID(), loottype );
}

void Spell::SpellEffectOpenLockItem(uint32 i)
{
	Unit* caster = ((Unit *)m_caster);
	if(!caster && m_caster->IsItem()) caster = ((Item *)m_caster)->GetOwner();

	if(!gameObjTarget || !gameObjTarget->IsInWorld()) 
		return;
	
	if( caster && caster->IsPlayer() && sQuestMgr.OnGameObjectActivate( (static_cast<Player*>(caster)), gameObjTarget ) )
		static_cast<Player*>(caster)->UpdateNearbyGameObjects();

	CALL_GO_SCRIPT_EVENT(gameObjTarget, OnActivate)(static_cast<Player*>(caster));
	gameObjTarget->SetByte(GAMEOBJECT_BYTES_1, 0, 0);	

	if( gameObjTarget->GetEntry() == 183146) // Bond-o-tron 4000
	{
		gameObjTarget->Despawn(1);
		return;
	}

	if( gameObjTarget->GetByte(GAMEOBJECT_BYTES_1, 1) == GAMEOBJECT_TYPE_CHEST)
	{
		lootmgr.FillGOLoot(&gameObjTarget->loot,gameObjTarget->GetEntry(), gameObjTarget->GetMapMgr() ? (gameObjTarget->GetMapMgr()->iInstanceMode ? true : false) : false);
		if(gameObjTarget->loot.items.size() > 0)
		{
			((Player*)caster)->SendLoot(gameObjTarget->GetGUID(),LOOT_CORPSE);
		}
	}

	if( gameObjTarget->GetByte(GAMEOBJECT_BYTES_1, 1) == GAMEOBJECT_TYPE_DOOR)
		gameObjTarget->SetUInt32Value(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE | GO_FLAG_NO_DESPAWN); // 33);

	if(gameObjTarget->GetMapMgr()->GetMapInfo()->type==INSTANCE_NULL)//dont close doors for instances
		sEventMgr.AddEvent(gameObjTarget,&GameObject::EventCloseDoor, EVENT_GAMEOBJECT_DOOR_CLOSE,10000,1,0);
	
	sEventMgr.AddEvent(gameObjTarget, &GameObject::Despawn, (uint32)1, EVENT_GAMEOBJECT_ITEM_SPAWN, 6*60*1000, 1, 0);
}

void Spell::SpellEffectProficiency(uint32 i)
{
	uint32 skill = 0;
	skilllinespell* skillability = objmgr.GetSpellSkill(m_spellInfo->Id);
	if (skillability)
		skill = skillability->skilline;
	skilllineentry* sk = dbcSkillLine.LookupEntry(skill);
	if(skill)
	{
		if(playerTarget)
		{
			if(playerTarget->_HasSkillLine(skill))
			{
				// Increase it by one
			   // playerTarget->AdvanceSkillLine(skill);
			}
			else
			{
				// Don't add skills to players logging in.
				/*if((m_spellInfo->attributes & 64) && playerTarget->m_TeleportState == 1)
					return;*/

				if(sk && sk->type == SKILL_TYPE_WEAPON)
					playerTarget->_AddSkillLine(skill, 1, 5*playerTarget->getLevel());
				else
					playerTarget->_AddSkillLine(skill, 1, 1);				
			}
		}
	}
}

void Spell::SpellEffectSendEvent(uint32 i) //Send Event
{
	//This is mostly used to trigger events on quests or some places

	//use target, could be null, but were passing spell too so caster can be taken from spell easier
	sScriptMgr.CreateScriptClassForEvent(m_spellInfo->EffectMiscValue[i], unitTarget, NULL, this);

	uint32 spellId = m_spellInfo->Id;

	// Try a dummy SpellHandler
	if(sScriptMgr.CallScriptedDummySpell(spellId, i, this))
		return;

	switch(spellId)
	{

	// Place Loot
	case 25720: // Places the Bag of Gold at the designated Drop-Off Point.
		{

		}break;

	// Item - Cleansing Vial DND
	case 29297: // Empty the vial near the Bones of Aggonar to cleanse the waters of their demonic taint.
		{
			QuestLogEntry *en=((Player *)m_caster)->GetQuestLogForEntry(9427);
			if(!en)
				return;
			//Log.Warning("SpellEffectSendEvent","SendQuestComplete");
			en->SendQuestComplete();
		}break;

	//Warlock: Summon Succubus Quest
	case 8674:
	case 9223:
	case 9224:
		{
			CreatureInfo * ci = CreatureNameStorage.LookupEntry(5677);
			CreatureProto * cp = CreatureProtoStorage.LookupEntry(5677);
			if( !ci || !cp )
				return;

		   Creature * pCreature = ((Player *)m_caster)->GetMapMgr()->CreateCreature(cp->Id);
		   pCreature->Load(cp, ((Player *)m_caster)->GetPositionX(), ((Player *)m_caster)->GetPositionY(), 
			                   ((Player *)m_caster)->GetPositionZ(),((Player *)m_caster)->GetOrientation());
		   pCreature->_setFaction();
		   pCreature->GetAIInterface()->Init(pCreature,AITYPE_AGRO,MOVEMENTTYPE_NONE);
		   pCreature->GetAIInterface()->taunt(((Player *)m_caster),true);
		   pCreature->_setFaction();
		   pCreature->PushToWorld(((Player *)m_caster)->GetMapMgr());
		   sEventMgr.AddEvent(pCreature, &Creature::SafeDelete, EVENT_CREATURE_REMOVE_CORPSE,60000, 1, 0);
		}break;

	//Warlock: Summon Voidwalker Quest
	case 30208:
	case 9221:
	case 9222:
	case 7728:
		{
			CreatureInfo * ci = CreatureNameStorage.LookupEntry(5676);
			CreatureProto * cp = CreatureProtoStorage.LookupEntry(5676);
			if( !ci || !cp )
				return;

		   Creature * pCreature = ((Player *)m_caster)->GetMapMgr()->CreateCreature(cp->Id);
		   pCreature->Load(cp, ((Player *)m_caster)->GetPositionX(), ((Player *)m_caster)->GetPositionY(), 
			                   ((Player *)m_caster)->GetPositionZ(), ((Player *)m_caster)->GetOrientation());
		   pCreature->_setFaction();
		   pCreature->GetAIInterface()->Init(pCreature,AITYPE_AGRO,MOVEMENTTYPE_NONE);
		   pCreature->GetAIInterface()->taunt(((Player *)m_caster),true);
		   pCreature->_setFaction();
		   pCreature->PushToWorld(((Player *)m_caster)->GetMapMgr());
		   sEventMgr.AddEvent(pCreature, &Creature::SafeDelete, EVENT_CREATURE_REMOVE_CORPSE,60000, 1, 0);
		}break;

	//Warlock: Summon Felhunter Quest
	case 8712:
		{
			CreatureInfo * ci = CreatureNameStorage.LookupEntry(6268);
			CreatureProto * cp = CreatureProtoStorage.LookupEntry(6268);
			if( !ci || !cp )
				return;

		   Creature * pCreature = ((Player *)m_caster)->GetMapMgr()->CreateCreature(cp->Id);
		   pCreature->Load(cp, ((Player *)m_caster)->GetPositionX(), ((Player *)m_caster)->GetPositionY(), 
			                   ((Player *)m_caster)->GetPositionZ(), ((Player *)m_caster)->GetOrientation());
		   pCreature->_setFaction();
		   pCreature->GetAIInterface()->Init(pCreature,AITYPE_AGRO,MOVEMENTTYPE_NONE);
		   pCreature->GetAIInterface()->taunt(((Player *)m_caster),true);
		   pCreature->_setFaction();
		   pCreature->PushToWorld(((Player *)m_caster)->GetMapMgr());
		   sEventMgr.AddEvent(pCreature, &Creature::SafeDelete, EVENT_CREATURE_REMOVE_CORPSE,60000, 1, 0);
		}break;

	}
}

void Spell::SpellEffectApplyAA(uint32 i) // Apply Area Aura
{
	if(!unitTarget || !unitTarget->isAlive())
		return;
	if(((Unit *)m_caster)!=unitTarget)
		return;

	Aura* pAura = NULL;
	std::map<uint32,Aura*>::iterator itr=unitTarget->tmpAura.find(m_spellInfo->Id);
	if(itr==unitTarget->tmpAura.end())
	{
		Aura* oldaura = NULL;

		//we always remove UniqueGroup2, cant have 2 ranks of blessings/greaters/combination on a target
		if (m_spellInfo->UniqueGroup2 != 0)
		{
			for(uint32 x=0; x < MAX_POSITIVE_AURAS; ++x)
			{
				if(unitTarget->m_auras[x] && unitTarget->m_auras[x]->m_spellProto->UniqueGroup2 == m_spellInfo->UniqueGroup2)
				{
					unitTarget->m_auras[x]->m_wasremoved = true;
					unitTarget->m_auras[x]->Remove();
				}
			}
		}

		if (m_spellInfo->UniqueGroup != 0)
		{
			if (m_spellInfo->UniqueTargetBased == 0)
				oldaura = unitTarget->FindAuraByUniqueGroup(m_spellInfo->UniqueGroup, m_caster->GetGUID());
			//only allowed 1 per target, sunder for example
			if (m_spellInfo->UniqueTargetBased & 1)
			{
				oldaura = unitTarget->FindAuraByUniqueGroup(m_spellInfo->UniqueGroup);
			}
			//only allowed 1 per caster
			if (m_spellInfo->UniqueTargetBased & 2)
			{
				std::multimap<uint32, Aura*>::iterator itr = ((Unit *)m_caster)->m_aurascast.find(m_spellInfo->UniqueGroup);
				if (itr != ((Unit *)m_caster)->m_aurascast.end())
				{
					if (oldaura == itr->second)
						oldaura = NULL;
					itr->second->Remove();
				}
			}

			//different spell, we have to remove the old aura
			if (oldaura != NULL)
			{
				unitTarget->RemoveAura(oldaura);
				oldaura = NULL; //this makes it so we use a new aura :P
			}
		}

		uint32 d = GetDuration(INVOC_DURATION); //debug
		pAura=new Aura(m_spellInfo, GetDuration(INVOC_DURATION), INVOC_DURATION, m_caster, unitTarget);
		pAura->m_AreaAuraOwner = true;

		//the tmpmap aura shouldn't have time modified, ever
		//pAura->SetTimeLeft(GetDuration());

		if (m_spellScript != NULL)
			m_spellScript->AddRef(pAura);
		
		unitTarget->tmpAura[m_spellInfo->Id]=pAura;
	
		float r=GetRadius(i);
		if(!sEventMgr.HasEvent(pAura, EVENT_AREAAURA_UPDATE))		/* only add it once */
		{
			sEventMgr.AddEvent(pAura, &Aura::EventUpdateAA, r*r, EVENT_AREAAURA_UPDATE, 100, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			//sEventMgr.AddEvent(pAura, &Aura::EventUpdateAA, r*r, EVENT_AREAAURA_UPDATE, 1,    1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);	un de trop !! (Brz)
		}
	}
	else 
	{
		pAura=itr->second;	
	}

	pAura->AddMod(m_spellInfo->EffectApplyAuraName[i],damage,m_spellInfo->EffectMiscValue[i],i);
}

void Spell::SpellEffectLearnSpell(uint32 i) // Learn Spell
{
	if(playerTarget == 0 && unitTarget && unitTarget->IsPet())
	{
		// bug in target map fill?
		//playerTarget = m_caster->GetMapMgr()->GetPlayer((uint32)m_targets.m_unitTarget);
		SpellEffectLearnPetSpell(i);
		return;
	}
                             // 483                           // 55884
	if( m_spellInfo->Id == __Learning || m_spellInfo->Id == __Learning____0 )		// "Learning"
	{
		if( !m_caster->IsItem()) return; //|| !m_caster->IsPlayer() ) return;

		uint32 spellid = 0;
		for(int i = 0; i < 5; ++i)
		{
			if( ((Item *)m_caster)->GetProto()->Spells[i].Trigger == LEARNING && ((Item *)m_caster)->GetProto()->Spells[i].Id != 0 )
			{
				spellid = ((Item *)m_caster)->GetProto()->Spells[i].Id;
				Log.Warning("SPELLLEARNING", "Le spell a apprendre est %u",spellid );
				break;
			}
		}

		if( !spellid || !dbcSpell.LookupEntryForced(spellid) )
			return;

		// learn me!
		Player * plr = ((Item *)m_caster)->GetPlayerFromItem();
		if (plr)
			plr->addSpell(spellid);

		// no normal handler
		return;
	}

	if(playerTarget)
	{
		/*if(u_caster && isHostile(playerTarget, u_caster))
			return;*/

		uint32 spellToLearn = m_spellInfo->EffectTriggerSpell[i];
		playerTarget->addSpell(spellToLearn);

		if(spellToLearn == 2366)
			playerTarget->addSpell(32605);

		if(spellToLearn == 2575)
			playerTarget->addSpell(32606);

		//smth is wrong here, first we add this spell to player then we may cast it on player...
		SpellEntry *spellinfo = dbcSpell.LookupEntry(spellToLearn);
 		//remove specializations
		switch(spellinfo->Id)
 		{
		case 26801: //Shadoweave Tailoring
			playerTarget->removeSpell(26798,false,false,0); //Mooncloth Tailoring
			playerTarget->removeSpell(26797,false,false,0); //Spellfire Tailoring
 			break;
		case 26798: // Mooncloth Tailoring
			playerTarget->removeSpell(26801,false,false,0); //Shadoweave Tailoring
			playerTarget->removeSpell(26797,false,false,0); //Spellfire Tailoring
 			break;
		case 26797: //Spellfire Tailoring
			playerTarget->removeSpell(26801,false,false,0); //Shadoweave Tailoring
			playerTarget->removeSpell(26798,false,false,0); //Mooncloth Tailoring
 			break;
 		case 10656: //Dragonscale Leatherworking
			playerTarget->removeSpell(10658,false,false,0); //Elemental Leatherworking
			playerTarget->removeSpell(10660,false,false,0); //Tribal Leatherworking
 			break;
 		case 10658: //Elemental Leatherworking
			playerTarget->removeSpell(10656,false,false,0); //Dragonscale Leatherworking
			playerTarget->removeSpell(10660,false,false,0); //Tribal Leatherworking
 			break;
 		case 10660: //Tribal Leatherworking
			playerTarget->removeSpell(10656,false,false,0); //Dragonscale Leatherworking
			playerTarget->removeSpell(10658,false,false,0); //Elemental Leatherworking
 			break;
 		case 28677: //Elixir Master
			playerTarget->removeSpell(28675,false,false,0); //Potion Master
			playerTarget->removeSpell(28672,false,false,0); //Transmutation Maste
 			break;
 		case 28675: //Potion Master
			playerTarget->removeSpell(28677,false,false,0); //Elixir Master
			playerTarget->removeSpell(28672,false,false,0); //Transmutation Maste
 			break;
 		case 28672: //Transmutation Master
			playerTarget->removeSpell(28675,false,false,0); //Potion Master
			playerTarget->removeSpell(28677,false,false,0); //Elixir Master
 			break;
 		case 20219: //Gnomish Engineer
			playerTarget->removeSpell(20222,false,false,0); //Goblin Engineer
 			break;
 		case 20222: //Goblin Engineer
			playerTarget->removeSpell(20219,false,false,0); //Gnomish Engineer
 			break;
 		case 9788: //Armorsmith
			playerTarget->removeSpell(9787,false,false,0); //Weaponsmith
			playerTarget->removeSpell(17039,false,false,0); //Master Swordsmith
			playerTarget->removeSpell(17040,false,false,0); //Master Hammersmith
			playerTarget->removeSpell(17041,false,false,0); //Master Axesmith
 			break;
 		case 9787: //Weaponsmith
			playerTarget->removeSpell(9788,false,false,0); //Armorsmith
 			break;
 		case 17041: //Master Axesmith
			playerTarget->removeSpell(9788,false,false,0); //Armorsmith
			playerTarget->removeSpell(17040,false,false,0); //Master Hammersmith
			playerTarget->removeSpell(17039,false,false,0); //Master Swordsmith
 			break;
 		case 17040: //Master Hammersmith
			playerTarget->removeSpell(9788,false,false,0); //Armorsmith
			playerTarget->removeSpell(17039,false,false,0); //Master Swordsmith
			playerTarget->removeSpell(17041,false,false,0); //Master Axesmith
 			break;
 		case 17039: //Master Swordsmith
			playerTarget->removeSpell(9788,false,false,0); //Armorsmith
			playerTarget->removeSpell(17040,false,false,0); //Master Hammersmith
			playerTarget->removeSpell(17041,false,false,0); //Master Axesmith
 			break;
 		}
		for(uint32 i=0;i<3;i++)
			if(spellinfo->Effect[i] == SPELL_EFFECT_WEAPON ||
			   spellinfo->Effect[i] == SPELL_EFFECT_PROFICIENCY ||
			   spellinfo->Effect[i] == SPELL_EFFECT_DUAL_WIELD )
			{
				Spell *sp = new Spell(unitTarget, spellinfo, true, NULL);
				SpellCastTargets targets;
				targets.m_target = unitTarget;
				targets.m_targetMask = TARGET_FLAG_UNIT; // 0x02;
				sp->prepare(&targets);
				break;
			}	  
		return;
	}

	// if we got here... try via pet spells..
	SpellEffectLearnPetSpell(i);
}

void Spell::SpellEffectSpellDefense(uint32 i)
{
	//used to enable this ability. We use it all the time ...
}

void Spell::SpellEffectLearnPetSpell(uint32 i)
{
	/*if(unitTarget && m_caster->GetTypeId() == TYPEID_PLAYER)
	{
		if(unitTarget->IsPet() && unitTarget->GetTypeId() == TYPEID_UNIT)
		{
			static_cast< Player* >(m_caster)->AddPetSpell(m_spellInfo->EffectTriggerSpell[i], unitTarget->GetEntry());
		}
	}*/

	if(unitTarget && unitTarget->IsPet() && m_caster->IsPlayer())
	{
		Pet * pPet = static_cast<Pet*>( unitTarget );
		if(pPet->IsSummon())
			((Player *)m_caster)->AddSummonSpell(unitTarget->GetEntry(), m_spellInfo->EffectTriggerSpell[i]);
		
		pPet->AddSpell( dbcSpell.LookupEntry( m_spellInfo->EffectTriggerSpell[i] ), true );

		// Send Packet
		/*
		WorldPacket data(SMSG_SET_EXTRA_AURA_INFO_OBSOLETE, 22);
		data << pPet->GetGUID() << uint8(0) << uint32(m_spellInfo->EffectTriggerSpell[i]) << uint32(-1) << uint32(0);
		p_caster->GetSession()->SendPacket(&data);
		*/
	}
}

void Spell::SpellEffectDispel(uint32 i) // Dispel
{
	if(!m_caster->IsUnit()) return;
	if(!unitTarget) return;

	std::map<Aura*, bool> ToRemove;
	Aura *aur;
	uint32 start,end;
	if(isAttackable(((Unit *)m_caster),unitTarget))
	{
		start=0;
		end=MAX_POSITIVE_AURAS;
	}
	else
	{
		start=MAX_POSITIVE_AURAS;
		end=MAX_AURAS;
	}
	
	WorldPacket data(SMSG_SPELLDISPELLOG, 16);

	for(uint32 x=start;x<end;x++)
	if(unitTarget->m_auras[x])
	{
		aur = unitTarget->m_auras[x];
		//Nothing can dispel resurrection sickness;
		if(!aur->IsPassive() && !(aur->GetSpellProto()->attributes & ATTRIBUTES_IGNORE_INVULNERABILITY))
		{
			if(m_spellInfo->dispelType == DISPEL_ALL)
			{
				//unitTarget->HandleProc( PROC_ON_PRE_DISPELL_AURA_VICTIM , u_caster , m_spellInfo, aur->GetSpellId() );
				
				data.clear();
				data << m_caster->GetNewGUID();
				data << unitTarget->GetNewGUID();
				data << (uint32)1;//probably dispel type
				data << aur->GetSpellId();
				m_caster->SendMessageToSet(&data,true);

				ToRemove.insert(std::pair<Aura*, bool>(aur, false));

				if(!--damage)
					break;;
			}
			else if(aur->GetSpellProto()->dispelType == m_spellInfo->EffectMiscValue[i])
			{
				data.clear();
				data << m_caster->GetNewGUID();
				data << unitTarget->GetNewGUID();
				data << (uint32)1;
				data << aur->GetSpellId();
				m_caster->SendMessageToSet(&data,true);

				ToRemove.insert(std::pair<Aura*, bool>(aur, true));

				if(!--damage)
					break;
			}	
		}
	} 

	for (std::map<Aura*, bool>::iterator itr=ToRemove.begin(); itr!=ToRemove.end(); ++itr)
	{
		if (itr->first->m_spellScript != NULL)
			itr->first->m_spellScript->OnDispel(itr->first, this);

		//OnDispel might cause damage, which might call a duel end :P
		if (!unitTarget->HasAura(itr->first))
			continue;

		if (itr->second)
			unitTarget->RemoveAllAuras(itr->first->GetSpellProto()->Id, itr->first->GetCasterGUID(),NULL);
		else
			itr->first->Remove();

	}
}

void Spell::SpellEffectLanguage(uint32 i)
{
	if(m_caster->GetTypeId() != TYPEID_PLAYER)
		return;

	Player* pPlayer = static_cast<Player*>( m_caster );

	if(!pPlayer->GetSession()->HasGMPermissions())
	{
		if(pPlayer->GetTeam() == FACTION_ALLY)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_COMMON ) )
				pPlayer->_AddSkillLine( SKILL_LANG_COMMON, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_COMMON );

		if(pPlayer->GetTeam() == FACTION_HORDE)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_ORCISH ) )
				pPlayer->_AddSkillLine( SKILL_LANG_ORCISH, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_ORCISH );

		if(pPlayer->getRace() == RACE_DWARF)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_DWARVEN ) )
				pPlayer->_AddSkillLine( SKILL_LANG_DWARVEN, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_DWARVEN );

		if(pPlayer->getRace() == RACE_NIGHTELF)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_DARNASSIAN ) )
				pPlayer->_AddSkillLine( SKILL_LANG_DARNASSIAN, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_DARNASSIAN );

		if(pPlayer->getRace() == RACE_UNDEAD)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_GUTTERSPEAK ) )
				pPlayer->_AddSkillLine( SKILL_LANG_GUTTERSPEAK, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_GUTTERSPEAK );

		if(pPlayer->getRace() == RACE_TAUREN)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_TAURAHE ) )
				pPlayer->_AddSkillLine( SKILL_LANG_TAURAHE, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_TAURAHE );

		if(pPlayer->getRace() == RACE_GNOME)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_GNOMISH ) )
				pPlayer->_AddSkillLine( SKILL_LANG_GNOMISH, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_GNOMISH );

		if(pPlayer->getRace() == RACE_TROLL)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_TROLL ) )
				pPlayer->_AddSkillLine( SKILL_LANG_TROLL, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_TROLL );

		if(pPlayer->getRace() == RACE_BLOODELF)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_THALASSIAN ) )
				pPlayer->_AddSkillLine( SKILL_LANG_THALASSIAN, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_THALASSIAN );

		if(pPlayer->getRace() == RACE_DRAENEI)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_DRAENEI ) )
				pPlayer->_AddSkillLine( SKILL_LANG_DRAENEI, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_DRAENEI );
	}
}

void Spell::SpellEffectDualWield(uint32 i)
{
	if(m_caster->GetTypeId() != TYPEID_PLAYER) 
		return;

	Player *pPlayer = static_cast< Player* >( m_caster );

	if( !pPlayer->_HasSkillLine( SKILL_DUAL_WIELD ) )
		 pPlayer->_AddSkillLine( SKILL_DUAL_WIELD, 1, 1 );
	
		// Increase it by one
		//dual wield is 1/1 , it never increases it's not even displayed in skills tab

	//note: probably here must be not caster but unitVictim
}

void Spell::SpellEffectSkillStep(uint32 i) // Skill Step
{
	Player*target;
	if(m_caster->GetTypeId() != TYPEID_PLAYER)
	{
		// Check targets
		if( m_targets.m_target && m_targets.m_target->IsPlayer() )
			target = static_cast<Player*>(m_targets.m_target);
		else
			return;
	}
	else
	{
		target = static_cast< Player* >( m_caster );
	}
	
	uint32 skill = m_spellInfo->EffectMiscValue[i];
	if( skill == 242 )
		skill = SKILL_LOCKPICKING; // somehow for lockpicking misc is different than the skill :s

	skilllineentry* sk = dbcSkillLine.LookupEntry( skill );

	if( sk == NULL )
		return;

	uint32 max = 1;
	switch( sk->type )
	{
		case SKILL_TYPE_PROFESSION:
		case SKILL_TYPE_SECONDARY:
			max = damage * 75;
			break;
		case SKILL_TYPE_WEAPON:
			max = 5 * target->getLevel();
			break;
		case SKILL_TYPE_CLASS:
		case SKILL_TYPE_ARMOR:
			if( skill == SKILL_LOCKPICKING )
				max = damage * 75;
			else
				max = 1;
			break;
		default: //u cant learn other types in game
			return;
	};

	if( target->_HasSkillLine( skill ) )
	{
		target->_ModifySkillMaximum( skill, max );
	}		
	else
	{
		// Don't add skills to players logging in.
		/*if((m_spellInfo->attributes & 64) && playerTarget->m_TeleportState == 1)
			return;*/

		if( sk->type == SKILL_TYPE_PROFESSION )
			target->ModUnsigned32Value( PLAYER_CHARACTER_POINTS2, -1 );
	  
		if( skill == SKILL_RIDING )
			target->_AddSkillLine( skill, max, max );
		else
			target->_AddSkillLine( skill, 1, max );
	}

	//professions fix, for unknow reason when u learn profession it 
	//does not teach find herbs for herbalism etc. moreover there is no spell
	//in spell.dbc that would teach u this. It means blizz does it in some tricky way too
	switch( skill )
	{
	case SKILL_ALCHEMY:
		target->addSpell( 2330 );//Minor Healing Potion
		target->addSpell( 2329 );//Elixir of Lion's Strength
		target->addSpell( 7183 );//Elixir of Minor Defense
		break;
	case SKILL_ENCHANTING:
		target->addSpell( 7418 );//Enchant Bracer - Minor Health
		target->addSpell( 7428 );//Enchant Bracer - Minor Deflection
		target->addSpell( 7421 );//Runed Copper Rod
		target->addSpell( 13262 );//Disenchant
		break;
	case SKILL_HERBALISM:
		target->addSpell( 2383 );//find herbs
		break;
	case SKILL_MINING:
		target->addSpell( 2657 );//smelt copper
		target->addSpell( 2656 );//smelting
		target->addSpell( 2580 );//find minerals
		break;
	case SKILL_FIRST_AID:
		target->addSpell( 3275 );//Linen Bandage
		break;
	case SKILL_TAILORING:
		target->addSpell( 2963 );//Bolt of Linen Cloth

		target->addSpell( 2387 );//Linen Cloak
		target->addSpell( 2393 );//White Linen Shirt
		target->addSpell( 3915 );//Brown Linen Shirt
		target->addSpell( 12044 );//Simple Linen Pants
		break;
	case SKILL_LEATHERWORKING:
		target->addSpell( 2149 );//Handstitched Leather Boots
		target->addSpell( 2152 );//Light Armor Kit
		target->addSpell( 2881 );//Light Leather
		target->addSpell( 7126 );//Handstitched Leather Vest
		target->addSpell( 9058 );//Handstitched Leather Cloak
		target->addSpell( 9059 );//Handstitched Leather Bracers
		break;
	case SKILL_ENGINERING:
		target->addSpell( 3918 );//Rough Blasting Powder
		target->addSpell( 3919 );//Rough Dynamite
		target->addSpell( 3920 );//Crafted Light Shot
		break;
	case SKILL_COOKING:
		target->addSpell( 2538 );//Charred Wolf Meat
		target->addSpell( 2540 );//Roasted Boar Meat
		target->addSpell( 818 );//Basic Campfire
		target->addSpell( 8604 );//Herb Baked Egg
		break;
	case SKILL_BLACKSMITHING:
		target->addSpell( 2660 );//Rough Sharpening Stone
		target->addSpell( 2663 );//Copper Bracers
		target->addSpell( 12260 );//Rough Copper Vest
		target->addSpell( 2662 );//Copper Chain Pants
		target->addSpell( 3115 );//Rough Weightstone
		break;
	case SKILL_JEWELCRAFTING:
		target->addSpell( 25255 );// Delicate Copper Wire
		target->addSpell( 25493 );// Braided Copper Ring
		target->addSpell( 26925 );// Woven Copper Ring
		target->addSpell( 32259 );// Rough Stone Statue
		break;
	};
}

void Spell::SpellEffectDetect(uint32 i)
{
	if( ((Unit *)m_caster) == NULL )
		return;
	/* Crow:
	Makes me afraid to see what this us used for.
	Notes below...
	*/

	// Crow: We'll just do a visibility update....
	((Unit *)m_caster)->UpdateVisibility();
}

void Spell::SpellEffectSummonObject(uint32 i)
{
	if( !m_caster->IsPlayer() )	return;

	uint32 entry = m_spellInfo->EffectMiscValue[i];

	uint32 mapid = ((Player *)m_caster)->GetMapId();
	float px = ((Player *)m_caster)->GetPositionX();
	float py = ((Player *)m_caster)->GetPositionY();
	float pz = ((Player *)m_caster)->GetPositionZ();
	float orient = ((Player *)m_caster)->GetOrientation();
	float posx = 0, posy = 0, posz = 0;
	float co = cos( orient );
	float si = sin( orient );
	float radius = GetRadius(i);

	
	if( entry == GO_FISHING_BOBBER )
	{
		MapMgr * map = ((Player *)m_caster)->GetMapMgr();
		Spell * spell = ((Player *)m_caster)->GetCurrentSpell();

		float r;
		for( r = 20; r > 10; r-- )
		{
			posx = px+r*co;
			posy = py+r*si;
			if( !(map->GetWaterType( posx, posy ) & 1) )//water 
				continue;
			posz = map->GetWaterHeight( posx, posy );
			if( posz > map->GetLandHeight( posx, posy, pz ) )//water 
				break;
		}
		   
		// Todo / Fix me: This should be loaded / cached
		uint32 zone = ((Player *)m_caster)->GetAreaID();
		if( zone == 0 ) // If the player's area ID is 0, use the zone ID instead
			zone = ((Player *)m_caster)->GetZoneId();

		uint32 minskill;
		FishingZoneEntry *fishentry = FishingZoneStorage.LookupEntry( zone );
		if( !fishentry ) // Check database if there is fishing area / zone information, if not, return
			return;
		
		// Todo / Fix me: Add fishskill to the calculations
		minskill = fishentry->MinSkill;
		spell->SendChannelStart( 20000 ); // 30 seconds
		/*spell->SendSpellStart();
		spell->SendCastResult(SPELL_CANCAST_OK);
		spell->SendSpellGo ();*/

		GameObject *go = ((Player *)m_caster)->GetMapMgr()->CreateGameObject(GO_FISHING_BOBBER);

		go->CreateFromProto(GO_FISHING_BOBBER, mapid, posx, posy, posz, orient);
		go->SetUInt32Value(GAMEOBJECT_FLAGS, GO_FLAG_NOT_IN_USE);
		go->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
		go->SetUInt64Value(OBJECT_FIELD_CREATED_BY, ((Player *)m_caster)->GetGUID());
		((Player *)m_caster)->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, go->GetGUID());
			 
		go->SetInstanceID( ((Player *)m_caster)->GetInstanceID() );
		go->PushToWorld( ((Player *)m_caster)->GetMapMgr() );
	   
		if( lootmgr.IsFishable( zone ) ) // Only set a 'splash' if there is any loot in this area / zone
		{
			uint32 seconds = RandomUInt(17) + 2;
			sEventMgr.AddEvent( go, &GameObject::FishHooked, static_cast< Player* >( m_caster ), EVENT_GAMEOBJECT_FISH_HOOKED, seconds * 1000, 1, 0 );
		}
		sEventMgr.AddEvent( go, &GameObject::EndFishing, static_cast< Player* >( m_caster ),false, EVENT_GAMEOBJECT_END_FISHING, 20000, 1, 0 );
		((Player *)m_caster)->SetSummonedObject( go );
	}
	else
	{
		//portal, lightwell
		posx = px * (co + radius);
		posy = py * (si + radius);

#ifdef COLLISION
		//if not in LOS, then put summon on the caster
		const LocationVector &loc1 = m_caster->GetPositionNC();
        float SourceTaille = ((Player *)m_caster)->GetSize();
        
		if (!CollideInterface.CheckLOS(m_caster->GetMapId(),loc1.x,loc1.y,loc1.z + SourceTaille, //2.0f, 
			                                                posx  ,posy  ,loc1.z + SourceTaille )) // 2.0f))
		{
			posx = ((Player *)m_caster)->GetPositionX();
			posy = ((Player *)m_caster)->GetPositionY();
		}
#endif		
		GameObjectInfo * goI = GameObjectNameStorage.LookupEntry(entry);
		if(!goI)
		{
			if( m_caster->IsPlayer() ) // Forcement un player a cet endroit (Branruz)
			{
				sChatHandler.BlueSystemMessage(((Player *)m_caster)->GetSession(),
				"non-existant gameobject %u tried to be created by SpellEffectSummonObject. Report to devs!", entry);
			}
			return;
		}
		GameObject *go = ((Player *)m_caster)->GetMapMgr()->CreateGameObject(entry);
		
		go->SetInstanceID(((Player *)m_caster)->GetInstanceID());
		go->CreateFromProto(entry,mapid,posx,posy,pz,orient);
		go->SetByte(GAMEOBJECT_BYTES_1, 0, 1);		
		go->SetUInt64Value(OBJECT_FIELD_CREATED_BY,((Player *)m_caster)->GetGUID());
		go->PushToWorld(((Player *)m_caster)->GetMapMgr());	  
		sEventMgr.AddEvent(go, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_EXPIRE, GetDuration(INVOC_DURATION), 1,0);
		if(entry ==17032)//this is a portal
		{//enable it for party only
			go->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
			//disable by default     00000001 00000000 00000000 00000000
			WorldPacket *pkt = go->BuildFieldUpdatePacket(GAMEOBJECT_BYTES_1, 1 << 24);
			SubGroup * pGroup = ((Player *)m_caster)->GetGroup() ?
				((Player *)m_caster)->GetGroup()->GetSubGroup(((Player *)m_caster)->GetSubGroup()) : 0;

			if(pGroup)
			{
				((Player *)m_caster)->GetGroup()->Lock();
				for(GroupMembersSet::iterator itr = pGroup->GetGroupMembersBegin();
					itr != pGroup->GetGroupMembersEnd(); ++itr)
				{
					if((*itr)->m_loggedInPlayer && (((Player *)m_caster) != (*itr)->m_loggedInPlayer) )
						(*itr)->m_loggedInPlayer->GetSession()->SendPacket(pkt);
				}
				((Player *)m_caster)->GetGroup()->Unlock();
			}
			delete pkt;
		}
		else if(entry == 36727 || entry == 177193) // Portal of Summoning and portal of doom
		{
			Player * pTarget = ((Player *)m_caster)->GetMapMgr()->GetPlayer((uint32)((Player *)m_caster)->GetSelection());
			if(!pTarget)
				return;

			go->m_ritualmembers[0] = ((Player *)m_caster)->GetLowGUID();
			go->m_ritualcaster = ((Player *)m_caster)->GetLowGUID();
			go->m_ritualtarget = pTarget->GetLowGUID();
			go->m_ritualspell = m_spellInfo->Id;	 
		}
		else//Lightwell,if there is some other type -- add it
		{
			go->charges=5;//Max 5 charges
		}
		((Player *)m_caster)->SetSummonedObject(go);		
	}
}

void Spell::SpellEffectEnchantItem(uint32 i) // Enchant Item Permanent
{
	if(!itemTarget) return;
	if(!m_caster->IsPlayer()) return;

	EnchantEntry * Enchantment = dbcEnchant.LookupEntry(m_spellInfo->EffectMiscValue[i]);
	if(!Enchantment) return;

	if(((Player *)m_caster)->GetSession()->HasPermissions() != NULL)
		sGMLog.writefromsession(((Player *)m_caster)->GetSession(), "enchanted item for %s", itemTarget->GetOwner()->GetName());

	//remove other perm enchantment that was enchanted by profession
	itemTarget->RemoveProfessionEnchant();
	int32 Slot = itemTarget->AddEnchantment(Enchantment, 0, true, true, false, 0);
	if(Slot < 0)
		return; // Apply failed
			
	DetermineSkillUp(SKILL_ENCHANTING);
}

void Spell::SpellEffectEnchantItemTemporary(uint32 i)  // Enchant Item Temporary
{
	if(!itemTarget) return;
	if(!m_caster->IsPlayer()) return;

	uint32 Duration = damage > 1 ? damage : 1800;

	EnchantEntry * Enchantment = dbcEnchant.LookupEntry(m_spellInfo->EffectMiscValue[i]);
	if(!Enchantment) return;

	itemTarget->RemoveEnchantment(1);
	int32 Slot = itemTarget->AddEnchantment(Enchantment, Duration, false, true, false, 1);
	if(Slot < 0)
		return; // Apply failed

	skilllinespell* skill = objmgr.GetSpellSkill(m_spellInfo->Id);
	if(skill)
		DetermineSkillUp(skill->skilline,itemTarget->GetProto()->ItemLevel);
}

void Spell::SpellEffectTameCreature(uint32 i)
{
	if (!unitTarget->IsCreature()) return;
	Creature *tame = (Creature*)(unitTarget);
	//Creature *tame = ((unitTarget->GetTypeId() == TYPEID_UNIT) ? ((Creature*)unitTarget) : 0);

	if(!tame) return;

	uint8 result = SPELL_CANCAST_OK;

	if(!tame || 
	   !m_caster->IsPlayer() || 
	   !((Player *)m_caster)->isAlive() || 
	   !tame->isAlive() || 
	   (((Player *)m_caster)->getClass() != HUNTER) || 
	   ((((Player *)m_caster)->getClass() != WARRIOR) && sWorld.m_WarriorPetAllowed) ) 
		                                                         result = SPELL_FAILED_BAD_TARGETS;
	else if(!tame->GetCreatureName())                            result = SPELL_FAILED_BAD_TARGETS;
	else if(tame->GetCreatureName()->Type != BEAST)              result = SPELL_FAILED_BAD_TARGETS;
	else if(tame->getLevel() > ((Player *)m_caster)->getLevel()) result = SPELL_FAILED_HIGHLEVEL;
	else if(((Player *)m_caster)->GeneratePetNumber() == 0)      result = SPELL_FAILED_BAD_TARGETS;
	else if(!tame->GetCreatureName()->Family)                    result = SPELL_FAILED_BAD_TARGETS;
	else if(((Player *)m_caster)->GetSummon() || 
		    ((Player *)m_caster)->GetUnstabledPetNumber())       result = SPELL_FAILED_ALREADY_HAVE_SUMMON;

	CreatureFamilyEntry *cf = dbcCreatureFamily.LookupEntry(tame->GetCreatureName()->Family);
	if(cf && !cf->tameable)
		result = SPELL_FAILED_BAD_TARGETS;

	if(result != SPELL_CANCAST_OK)
	{
		SendCastResult(result);
		return;
	}

	// Remove target
	tame->GetAIInterface()->HandleEvent(EVENT_LEAVECOMBAT, ((Player *)m_caster), 0);
	Pet *pPet = objmgr.CreatePet();
	pPet->SetInstanceID(((Player *)m_caster)->GetInstanceID());                                         // Unit ??
	pPet->CreateAsSummon(tame->GetEntry(), tame->GetCreatureName(), tame, static_cast<Unit*>(m_caster), NULL, CREATURE_TYPE_PET, 0,NULL);

	tame->Despawn(0,tame->proto? tame->proto->RespawnTime:0);
}

void Spell::SpellEffectSummonPet(uint32 i) //summon - pet
{
	if(m_spellInfo->Id == 883)
	{
		// "Call Pet" spell
		if(!m_caster->IsPlayer()) return;

		if(((Player *)m_caster)->GetSummon() != 0)
		{
			((Player *)m_caster)->GetSession()->SendNotification("Vous avez déja un familier.");
			return;
		}

		uint32 petno = ((Player *)m_caster)->GetUnstabledPetNumber();

		if(petno)
		{
			((Player *)m_caster)->SpawnPet(petno);
			Pet* summon = ((Player *)m_caster)->GetSummon();

			if (summon == NULL)
				return;

			SpellCastTargets t(summon);
			Spell* s=new Spell(summon, dbcSpell.LookupEntry(__Hunter_Pet_Scaling_01), true, NULL);  // 34902
			Spell* s2=new Spell(summon, dbcSpell.LookupEntry(__Hunter_Pet_Scaling_02), true, NULL); // 34903
			Spell* s3=new Spell(summon, dbcSpell.LookupEntry(__Hunter_Pet_Scaling_03), true, NULL); // 34904
			// __Hunter_Pet_Scaling_04	61017
			s->prepare(&t);
			s2->prepare(&t);
			s3->prepare(&t);
		}
		else
		{
			WorldPacket data(SMSG_AREA_TRIGGER_MESSAGE, 50);
			data << uint32(0) << "Vous n'avez pas de familier." << uint8(0);
			((Player *)m_caster)->GetSession()->SendPacket(&data);
		}
		return;
	}
	
	//uint32 entryId = m_spellInfo->EffectMiscValue[i];

	//VoidWalker:torment, sacrifice, suffering, consume shadows
	//Succubus:lash of pain, soothing kiss, seduce , lesser invisibility
	//felhunter:	 Devour Magic,Paranoia,Spell Lock,	Tainted Blood
 
	if(!m_caster->IsUnit()) return; // Unit ?? Pas player ???
	
	// remove old pet
	Pet *old = ((Unit *)m_caster)->GetSummon();
	if(old) old->Dismiss(false);		
	
	CreatureInfo *ci = CreatureNameStorage.LookupEntry(m_spellInfo->EffectMiscValue[i]);
	if(ci)
	{
		//if demonic sacrifice auras are still active, remove them
		//uint32 spids[] = { 18789, 18790, 18791, 18792, 35701, 0 };
		//p_caster->RemoveAuras(spids);
		((Unit *)m_caster)->RemoveAura(18789);
		((Unit *)m_caster)->RemoveAura(18790);
		((Unit *)m_caster)->RemoveAura(18791);
		((Unit *)m_caster)->RemoveAura(18792);
		((Unit *)m_caster)->RemoveAura(35701);

		Pet *summon = objmgr.CreatePet();
		summon->SetInstanceID(m_caster->GetInstanceID());
		summon->CreateAsSummon(m_spellInfo->EffectMiscValue[i], ci, NULL, ((Unit *)m_caster), m_spellInfo, CREATURE_TYPE_CRITTER, 0);

		//cast scaling if warlock summon
		switch (m_spellInfo->Id)
		{
		case 688:
		case 691:
		case 697:
		case 712:
		case 30146:
		{
			//cast the scaling spells :P
			SpellCastTargets t(summon);
			Spell* s=new Spell(summon, dbcSpell.LookupEntry(34947), true, NULL);
			Spell* s2=new Spell(summon, dbcSpell.LookupEntry(34956), true, NULL);
			Spell* s3=new Spell(summon, dbcSpell.LookupEntry(34957), true, NULL);
			Spell* s4=new Spell(summon, dbcSpell.LookupEntry(34958), true, NULL);
			s->prepare(&t);
			s2->prepare(&t);
			s3->prepare(&t);
			s4->prepare(&t);
			break;
		}
		default: break;
		}
	}
}

void Spell::SpellEffectWeapondamage( uint32 i ) // Weapon damage +
{
	if( unitTarget == NULL ) return;
	if( !m_caster->IsUnit()) return;

	//Hackfix for Mangle
	if( m_spellInfo->NameHash == SPELL_HASH_MANGLE__CAT_ && m_caster->IsPlayer() )
		static_cast< Player* >( m_caster )->AddComboPoints( unitTarget->GetGUID(), 1 );

	// Hacky fix for druid spells where it would "double attack".
	if( m_spellInfo->Effect[2] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE || m_spellInfo->Effect[1] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE )
	{
		add_damage += damage;
		return;
	}

	uint32 _type;
	if( GetType() == DEFENSE_TYPE_RANGED )
		_type = RANGED;
	else
	{              
		if (m_spellInfo->attributesExC & ATTRIBUTESEXC_OFFHAND)
			_type =  OFFHAND;
		else
			_type = MELEE;
	}
	((Unit *)m_caster)->Strike( unitTarget, _type, m_spellInfo, damage, 0, 0, false, true );
}

void Spell::SpellEffectPowerBurn(uint32 i) // power burn
{
	if(!unitTarget)          	return;
	if(!unitTarget->isAlive())  return;
	if (unitTarget->GetPowerType() != POWER_TYPE_MANA) return;

	if( unitTarget->IsPlayer() )
	{
		// Resilience - reduces the effect of mana drains by (CalcRating*2)%.

		damage *= float2int32( 1 - ( ( static_cast<Player*>(unitTarget)->CalcRating( PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE ) * 2 ) / 100.0f ) );
	}
	int32 mana = (int32)min( (int32)unitTarget->GetUInt32Value( UNIT_FIELD_POWER1 ), damage );
	unitTarget->ModUnsigned32Value(UNIT_FIELD_POWER1,-mana);
	
	m_caster->SpellNonMeleeDamageLog(unitTarget,m_spellInfo->Id, (uint32)(mana * m_spellInfo->EffectMultipleValue[i]), pSpellId==0,true);   
}

void Spell::SpellEffectThreat(uint32 i) // Threat
{
	if(!unitTarget || !unitTarget->isAlive() )
		return;

	bool chck = unitTarget->GetAIInterface()->modThreatByPtr(((Unit *)m_caster),m_spellInfo->EffectBasePoints[i]);
	if(chck == false)
		unitTarget->GetAIInterface()->AttackReaction(((Unit *)m_caster),1,0);	
}

void Spell::SpellEffectTriggerSpell(uint32 i) // Trigger Spell
{
	
	if(!unitTarget || !m_caster)
		return;

	SpellEntry *spe = NULL;
	spe = dbcSpell.LookupEntryForced(GetSpellProto()->EffectTriggerSpell[i]);
	if(spe == NULL )
		return;

	if(GetSpellProto() == spe) // Infinite loop fix.
		return;

	Spell* sp=new Spell(m_caster,spe,true,NULL);

	Log.Warning("SpellEffectTriggerSpell", "La cible est %u",unitTarget->GetGUID());
	
	SpellCastTargets tgt( (spe->procFlags & PROC_TARGET_SELF) ? m_caster : unitTarget );
	sp->prepare(&tgt);

	//if(m_spellInfo->EffectTriggerSpell[i] != 0)
	  //  TriggerSpellId = m_spellInfo->EffectTriggerSpell[i];	
}

void Spell::SpellEffectApplyRaidAA(uint32 i) // Raid Area Aura
{
	SpellEffectApplyAA(i);
}

void Spell::SpellEffectPowerFunnel(uint32 i) // Power Funnel
{
	if(!unitTarget)
		return;		
	if(!unitTarget->isAlive() || !unitTarget->IsPet())
		return;

	//does not exist
}

/*void Spell::SpellEffectHealMaxHealth(uint32 i)   // Heal Max Health
{
	if( unitTarget == NULL || !unitTarget->isAlive() )
		return;

	uint32 dif = unitTarget->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) - unitTarget->GetUInt32Value( UNIT_FIELD_HEALTH );
	if( !dif )
	{
		SendCastResult( SPELL_FAILED_ALREADY_AT_FULL_HEALTH );
		return;
	}

	SendHealSpellOnPlayer(m_caster, unitTarget, dif, false );

	unitTarget->ModUnsigned32Value( UNIT_FIELD_HEALTH, dif );
}*/

void Spell::SpellEffectHealMaxHealth(uint32 i)   // Heal Max Health
{
	if( unitTarget == NULL || !unitTarget->isAlive() )
		return;

	uint32 dif = unitTarget->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) - unitTarget->GetUInt32Value( UNIT_FIELD_HEALTH );
	if( !dif )
	{
		SendCastResult( SPELL_FAILED_ALREADY_AT_FULL_HEALTH );
		return;
	}

	if( unitTarget->GetTypeId() == TYPEID_PLAYER)
	{
		 SendHealSpellOnPlayer( static_cast<Player *>( m_caster ), static_cast<Player *>( unitTarget ), dif, false, 0, pSpellId ? pSpellId : m_spellInfo->Id );
	}
	unitTarget->ModUnsigned32Value( UNIT_FIELD_HEALTH, dif );
}

void Spell::SpellEffectInterruptCast(uint32 i) // Interrupt Cast
{
	if(!unitTarget || !unitTarget->isAlive())
		return;
	// can't apply stuns/fear/polymorph/root etc on boss
	if(unitTarget->GetTypeId()==TYPEID_UNIT)
	{
		Creature * c = (Creature*)( unitTarget );
		if (c&&c->GetCreatureName()&&c->GetCreatureName()->Rank == ELITE_WORLDBOSS)
			return;
	}
	// FIXME:This thing prevent target from spell casting too but cant find.
	uint32 school=0;
	if(unitTarget->GetCurrentSpell())
	{
		school=unitTarget->GetCurrentSpell()->m_spellInfo->School;
	}
	unitTarget->InterruptSpell();
	if(school)//prevent from casts in this school
	{
		unitTarget->SchoolCastPrevent[school]=GetDuration(INVOC_DURATION)+getMSTime();
		// TODO: visual!
	}
}

void Spell::SpellEffectDistract(uint32 i) // Distract
{
	//spellId 1725 Distract:Throws a distraction attracting the all monsters for ten sec's
	if(!unitTarget)
		return;
	if(!unitTarget->isAlive())
		return;

	if(m_targets.m_destX != 0.0f || m_targets.m_destY != 0.0f || m_targets.m_destZ != 0.0f)
	{
//		unitTarget->GetAIInterface()->MoveTo(m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, 0);
		uint32 Stare_duration=GetDuration(INVOC_DURATION);
		float o=unitTarget->GetOrientation();
		if(Stare_duration>30*60*1000)
			Stare_duration=10000;//if we try to stare for more then a half an hour then better not stare at all :P (bug)
		float newo=unitTarget->calcRadAngle(unitTarget->GetPositionX(),unitTarget->GetPositionY(),m_targets.m_destX,m_targets.m_destY);
		unitTarget->GetAIInterface()->StopMovement(Stare_duration);
		unitTarget->SetFacing(newo);
		sEventMgr.AddEvent(unitTarget, &Unit::SetFacing, o, EVENT_CREATURE_UPDATE, Stare_duration, 1, 0);
	}

	//Smoke Emitter 164870
	//Smoke Emitter Big 179066
	//Unit Field Target of 
}

void Spell::SpellEffectPickpocket(uint32 i) // pickpocket
{
	//Show random loot based on roll,	
	if(!unitTarget) return; // impossible..
	if(!m_caster->IsPlayer()) return;

	if(unitTarget->GetTypeId() != TYPEID_UNIT)
		return;

	Creature *target = static_cast<Creature*>( unitTarget );
	if(target->IsPickPocketed() || (target->GetCreatureName() && target->GetCreatureName()->Type != HUMANOID))
	{
		SendCastResult(SPELL_FAILED_TARGET_NO_POCKETS);
		return;
	}
			
  //lootmgr.FillPickpocketingLoot(&((Creature*)unitTarget)->loot,unitTarget->GetEntry());
  lootmgr.FillPickpocketingLoot(&unitTarget->loot,unitTarget->GetEntry());

	uint32 _rank = ((Creature*)unitTarget)->GetCreatureName() ? ((Creature*)unitTarget)->GetCreatureName()->Rank : 0;
	unitTarget->loot.gold = float2int32((_rank+1) * unitTarget->getLevel() * (RandomUInt(5) + 1) * sWorld.getRate(RATE_MONEY));

	((Player *)m_caster)->SendLoot(unitTarget->GetGUID(), LOOT_PICKPOCKETING );
	target->SetPickPocketed(true);
}

void Spell::SpellEffectAddFarsight(uint32 i) // Add Farsight
{
	if(!m_caster->IsPlayer()) return;

	/*float x = m_targets.m_destX;
	float y = m_targets.m_destY;
	float z = m_targets.m_destZ;
	if(x == 0)
		x = m_targets.m_srcX;
	if(y == 0)
		y = m_targets.m_srcY;
	if(z == 0)
		z = m_targets.m_srcZ;*/

	DynamicObject * dynObj = ((Player *)m_caster)->GetMapMgr()->CreateDynamicObject();
	dynObj->Create(((Player *)m_caster), this, m_targets.m_destX, m_targets.m_destY, m_targets.m_srcZ, GetDuration(INVOC_DURATION), GetRadius(i));
	if( dynObj == NULL ) //i <3 burlex :P
	{
		delete dynObj;
		return;
	}
    dynObj->SetUInt32Value(OBJECT_FIELD_TYPE, 65);           // a identifier (Brz)
    dynObj->SetUInt32Value(DYNAMICOBJECT_BYTES, 0x80000002); // a identifier (Brz)
	dynObj->AddToWorld(((Player *)m_caster)->GetMapMgr());
	//dynObj->SetInstanceID(p_caster->GetInstanceID());	
	((Player *)m_caster)->SetUInt64Value(PLAYER_FARSIGHT, dynObj->GetGUID());
	((Player *)m_caster)->SetUInt32Value(PLAYER_FARSIGHT, dynObj->GetLowGUID());
	((Player *)m_caster)->GetMapMgr()->ChangeFarsightLocation(((Player *)m_caster), m_targets.m_destX, m_targets.m_destY, true);
}

void Spell::SpellEffectHealMechanical(uint32 i)
{
	if(!unitTarget) return;

	if(unitTarget->GetTypeId() != TYPEID_UNIT) return;

	if(((Creature*)unitTarget)->GetCreatureName()->Type != MECHANICAL) return;

	Heal((int32)damage);
}

void Spell::SpellEffectSummonObjectWild(uint32 i)
{
	if(!m_caster->IsUnit()) return;
  
	// spawn a new one
	GameObject *GoSummon = ((Unit *)m_caster)->GetMapMgr()->CreateGameObject(m_spellInfo->EffectMiscValue[i]);
	if(!GoSummon->CreateFromProto(m_spellInfo->EffectMiscValue[i],
		((Unit *)m_caster)->GetMapId(), ((Unit *)m_caster)->GetPositionX(), 
		((Unit *)m_caster)->GetPositionY(), ((Unit *)m_caster)->GetPositionZ(), ((Unit *)m_caster)->GetOrientation() ))
	{
		delete GoSummon;
		return;
	}
	
	GoSummon->SetInstanceID(((Unit *)m_caster)->GetInstanceID());
	GoSummon->SetUInt32Value(GAMEOBJECT_LEVEL, ((Unit *)m_caster)->getLevel());
	GoSummon->SetUInt64Value(OBJECT_FIELD_CREATED_BY, ((Unit *)m_caster)->GetGUID());
	GoSummon->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
	GoSummon->PushToWorld(((Unit *)m_caster)->GetMapMgr());
	GoSummon->SetSummoned(((Unit *)m_caster));
	
	sEventMgr.AddEvent(GoSummon, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_EXPIRE, GetDuration(INVOC_DURATION), 1,0);
}

void Spell::SpellEffectScriptEffect(uint32 i) // Script Effect
{
	uint32 spellId = m_spellInfo->Id;

	if (m_spellScript != NULL)
		m_spellScript->ScriptEffect(i);

	// Try a dummy SpellHandler
	if(sScriptMgr.CallScriptedDummySpell(spellId, i, this))
		return;

	switch(spellId)
	{

	// Arcane Missiles
	/*
	case 5143://Rank 1
	case 5144://Rank 2
	case 5145://Rank 3
	case 8416://Rank 4
	case 8417://Rank 5
	case 10211://Rank 6
	case 10212://Rank 7
	case 25345://Rank 8
	{
		if(m_tmpAffect == 0)
		{
			Affect* aff = new Affect(m_spellInfo,GetDuration(sSpellDuration.LookupEntry(m_spellInfo->durationIndex)),m_GetGUID(),m_caster);
			m_tmpAffect = aff;
		}
		if(m_spellInfo->EffectBasePoints[0] < 0)
			m_tmpAffect->SetNegative();

		m_tmpAffect->SetPeriodicTriggerSpell(m_spellInfo->EffectTriggerSpell[0],m_spellInfo->EffectAmplitude[0],damage);
	}break;
	*/

	// Warlock Healthstones, just how much health does a lock need?
	case 6201:		// Minor Healthstone
		if (((Player *)m_caster)->HasSpell(18692))
		{
			CreateItem(19004);
			break;
		}
		if (((Player *)m_caster)->HasSpell(18693))
		{
			CreateItem(19005);
			break;
		}
		CreateItem(5512);
		break;
	case 6202:		// Lesser Healthstone
		if (((Player *)m_caster)->HasSpell(18693))	// Improved Healthstone (2)
		{
			CreateItem(19007);
			break;
		}
		if (((Player *)m_caster)->HasSpell(18692))	// Improved Healthstone (1)
		{
			CreateItem(19006);
			break;
		}
		CreateItem(5511);
		break;
	case 5699:		// Healthstone
		if (((Player *)m_caster)->HasSpell(18693))	// Improved Healthstone (2)
		{
			CreateItem(19009);
			break;
		}
		if (((Player *)m_caster)->HasSpell(18692))	// Improved Healthstone (1)
		{
			CreateItem(19008);
			break;
		}
		CreateItem(5509);
		break;
	case 11729:		// Greater Healthstone
		if (((Player *)m_caster)->HasSpell(18693))	// Improved Healthstone (2)
		{
			CreateItem(19011);
			break;
		}
		if (((Player *)m_caster)->HasSpell(18692))	// Improved Healthstone (1)
		{
			CreateItem(19010);
			break;
		}
		CreateItem(5510);
		break;
	case 11730:		// Major Healthstone
		if (((Player *)m_caster)->HasSpell(18693))	// Improved Healthstone (2)
		{
			CreateItem(19013);
			break;
		}
		if (((Player *)m_caster)->HasSpell(18692))	// Improved Healthstone (1)
		{
			CreateItem(19012);
			break;
		}
		CreateItem(9421);
		break;
	case 27230:		// Master Healthstone
		if (((Player *)m_caster)->HasSpell(18693))	// Improved Healthstone (2)
		{
			CreateItem(22105);
			break;
		}
		if (((Player *)m_caster)->HasSpell(18692))	// Improved Healthstone (1)
		{
			CreateItem(22104);
			break;
		}
		CreateItem(22103);
		break;

	// Holy Light
	case 635:
	case 639:
	case 647:
	case 1026:
	case 1042:
	case 3472:
	case 10328:
	case 10329:
	case 10348:
	case 25292:
	case 27135:
	case 27136:
	//Flash of light
	case 19750:
	case 19939:
	case 19940:
	case 19941:
	case 19942:
	case 19943:
	case 27137:
		if( ((Player *)m_caster)->HasAura( 20216 ) )
			Heal((int32)damage, true);
		else
			Heal((int32)damage);
	break;

	// Judgement
	case 20271:
		{
			if(!unitTarget) return;
			if(!m_caster->IsPlayer()) return;

			SpellEntry*en=dbcSpell.LookupEntry(((Player *)m_caster)->judgespell);
			Spell *sp=new Spell(((Player *)m_caster),en,true,NULL);
			SpellCastTargets tgt(unitTarget);
			//tgt.m_target=unitTarget;
			//tgt.m_targetMask=TARGET_FLAG_UNIT;
			sp->judgement = true;
			sp->prepare(&tgt);
			((Player *)m_caster)->RemoveAura(((Player *)m_caster)->Seal);
		}break;
	//warlock - Master Demonologist
	case 23784:
		{
			if( !m_caster->IsPlayer()) return;
			if( unitTarget == NULL) 	return; //can't imagine how this talent got to anybody else then a player casted on pet

			uint32 casted_spell_id = 0 ;
			uint32 inc_resist_by_level = 0 ;
			uint32 inc_resist_by_level_spell = 0 ;
			if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 416 ) //in case it is imp
				casted_spell_id = 23759 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1860 ) //VoidWalker
				casted_spell_id = 23760 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1863 ) //Succubus
				casted_spell_id = 23761 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 417 ) //Felhunter
			{
				casted_spell_id = 0 ;
				inc_resist_by_level_spell = 23762 ;
				inc_resist_by_level = 20 ;
			}
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 17252 ) //Felguard
			{
				casted_spell_id = 35702 ;
				inc_resist_by_level_spell = 23762 ;
				inc_resist_by_level = 10 ;
			}
			if( casted_spell_id )
			{
				//for self
				Spell *sp = new Spell( ((Player *)m_caster), dbcSpell.LookupEntry( casted_spell_id ), true, NULL );
				SpellCastTargets tgt( ((Player *)m_caster) );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell(  unitTarget, dbcSpell.LookupEntry( casted_spell_id ), true, NULL );
				SpellCastTargets tgt1( unitTarget );
				sp->prepare( &tgt1 );
			}
			if( inc_resist_by_level_spell )
			{
				//for self
				Spell *sp = new Spell( ((Player *)m_caster), dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULL );
				sp->forced_basepoints[0] = ((Player *)m_caster)->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt( ((Player *)m_caster) );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell( unitTarget, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULL );
				sp->forced_basepoints[0] = unitTarget->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt1( unitTarget );
				sp->prepare( &tgt1 );
			}
		}break;
	case 23830:
		{
			if(!m_caster->IsPlayer()) return; 
			if(unitTarget == NULL)    return; //can't imagine how this talent got to anybody else then a player casted on pet
			uint32 casted_spell_id = 0 ;
			uint32 inc_resist_by_level = 0 ;
			uint32 inc_resist_by_level_spell = 0 ;
			if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 416 ) //in case it is imp
				casted_spell_id = 23826 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1860 ) //VoidWalker
				casted_spell_id = 23841 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1863 ) //Succubus
				casted_spell_id = 23833 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 417 ) //Felhunter
			{
				casted_spell_id = 1 ;
				inc_resist_by_level_spell = 23837 ;
				inc_resist_by_level = 40 ;
			}
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 17252 ) //Felguard
			{
				casted_spell_id = 35703 ;
				inc_resist_by_level_spell = 23837 ;
				inc_resist_by_level = 20 ;
			}
			if( casted_spell_id )
			{
				//for self
				Spell *sp = new Spell( ((Player *)m_caster), dbcSpell.LookupEntry( casted_spell_id ), true, NULL );
				SpellCastTargets tgt( ((Player *)m_caster) );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell(  unitTarget, dbcSpell.LookupEntry( casted_spell_id ), true, NULL );
				SpellCastTargets tgt1( unitTarget );
				sp->prepare( &tgt1 );
			}
			if( inc_resist_by_level_spell )
			{
				//for self
				Spell *sp = new Spell( ((Player *)m_caster), dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULL );
				sp->forced_basepoints[0] = ((Player *)m_caster)->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt( ((Player *)m_caster) );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell( unitTarget, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULL );
				sp->forced_basepoints[0] = unitTarget->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt1( unitTarget );
				sp->prepare( &tgt1 );
			}
		}break;
	case 23831:
		{
			if( !m_caster->IsPlayer() ) return;
			if( unitTarget == NULL) 	return; //can't imagine how this talent got to anybody else then a player casted on pet
			uint32 casted_spell_id = 0 ;
			uint32 inc_resist_by_level = 0 ;
			uint32 inc_resist_by_level_spell = 0 ;
			if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 416 ) //in case it is imp
				casted_spell_id = 23827 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1860 ) //VoidWalker
				casted_spell_id = 23842 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1863 ) //Succubus
				casted_spell_id = 23834 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 417 ) //Felhunter
			{
				casted_spell_id = 0 ;
				inc_resist_by_level_spell = 23838 ;
				inc_resist_by_level = 60 ;
			}
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 17252 ) //Felguard
			{
				casted_spell_id = 35704 ;
				inc_resist_by_level_spell = 23838 ;
				inc_resist_by_level = 30 ;
			}
			if( casted_spell_id )
			{
				//for self
				Spell *sp = new Spell( ((Player *)m_caster), dbcSpell.LookupEntry( casted_spell_id ), true, NULL );
				SpellCastTargets tgt( ((Player *)m_caster) );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell(  unitTarget, dbcSpell.LookupEntry( casted_spell_id ), true, NULL );
				SpellCastTargets tgt1( unitTarget );
				sp->prepare( &tgt1 );
			}
			if( inc_resist_by_level_spell )
			{
				//for self
				Spell *sp = new Spell( ((Player *)m_caster), dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULL );
				sp->forced_basepoints[0] = ((Player *)m_caster)->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt( ((Player *)m_caster) );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell( unitTarget, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULL );
				sp->forced_basepoints[0] = unitTarget->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt1( unitTarget );
				sp->prepare( &tgt1 );
			}
		}break;
	case 23832:
		{
			if( !m_caster->IsPlayer()) return;
			if( unitTarget == NULL)	return; //can't imagine how this talent got to anybody else then a player casted on pet
			uint32 casted_spell_id = 0 ;
			uint32 inc_resist_by_level = 0 ;
			uint32 inc_resist_by_level_spell = 0 ;
			if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 416 ) //in case it is imp
				casted_spell_id = 23828 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1860 ) //VoidWalker
				casted_spell_id = 23843 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1863 ) //Succubus
				casted_spell_id = 23835 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 417 ) //Felhunter
			{
				casted_spell_id = 0 ;
				inc_resist_by_level_spell = 23839 ;
				inc_resist_by_level = 80 ;
			}
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 17252 ) //Felguard
			{
				casted_spell_id = 35705 ;
				inc_resist_by_level_spell = 23839 ;
				inc_resist_by_level = 40 ;
			}
			if( casted_spell_id )
			{
				//for self
				Spell *sp = new Spell( ((Player *)m_caster), dbcSpell.LookupEntry( casted_spell_id ), true, NULL );
				SpellCastTargets tgt( ((Player *)m_caster) );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell(  unitTarget, dbcSpell.LookupEntry( casted_spell_id ), true, NULL );
				SpellCastTargets tgt1( unitTarget );
				sp->prepare( &tgt1 );
			}
			if( inc_resist_by_level_spell )
			{
				//for self
				Spell *sp = new Spell( ((Player *)m_caster), dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULL );
				sp->forced_basepoints[0] = ((Player *)m_caster)->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt( ((Player *)m_caster) );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell( unitTarget, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULL );
				sp->forced_basepoints[0] = unitTarget->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt1( unitTarget );
				sp->prepare( &tgt1 );
			}
		}break;
	case 35708:
		{
			if( !m_caster->IsPlayer()) return;
			if ( unitTarget == NULL)   return; //can't imagine how this talent got to anybody else then a player casted on pet

			uint32 casted_spell_id = 0 ;
			uint32 inc_resist_by_level = 0 ;
			uint32 inc_resist_by_level_spell = 0 ;
			if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 416 ) //in case it is imp
				casted_spell_id = 23829 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1860 ) //VoidWalker
				casted_spell_id = 23844 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1863 ) //Succubus
				casted_spell_id = 23836 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 417 ) //Felhunter
			{
				casted_spell_id = 0 ;
				inc_resist_by_level_spell = 23840 ;
				inc_resist_by_level = 100 ;
			}
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 17252 ) //Felguard
			{
				casted_spell_id = 35706 ;
				inc_resist_by_level_spell = 23840 ;
				inc_resist_by_level = 50 ;
			}
			if( casted_spell_id )
			{
				//for self
				Spell *sp = new Spell( ((Player *)m_caster), dbcSpell.LookupEntry( casted_spell_id ), true, NULL );
				SpellCastTargets tgt( ((Player *)m_caster) );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell(  unitTarget, dbcSpell.LookupEntry( casted_spell_id ), true, NULL );
				SpellCastTargets tgt1( unitTarget );
				sp->prepare( &tgt1 );
			}
			if( inc_resist_by_level_spell )
			{
				//for self
				Spell *sp = new Spell( ((Player *)m_caster), dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULL );
				sp->forced_basepoints[0] = ((Player *)m_caster)->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt( ((Player *)m_caster) );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell( unitTarget, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULL );
				sp->forced_basepoints[0] = unitTarget->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt1( unitTarget );
				sp->prepare( &tgt1 );
			}
		}break;
	}
}

void Spell::SpellEffectSanctuary(uint32 i) // Stop all attacks made to you
{
	if(!m_caster->IsUnit())
		return;

	Object::InRangeSet::iterator itr = ((Unit *)m_caster)->GetInRangeOppFactsSetBegin();
	Object::InRangeSet::iterator itr_end = ((Unit *)m_caster)->GetInRangeOppFactsSetEnd();
	Unit * pUnit = NULL;

	for( ; itr != itr_end; ++itr ) {
		pUnit = static_cast<Unit*>(*itr);

		if( pUnit->GetTypeId() == TYPEID_UNIT )
			pUnit->GetAIInterface()->RemoveThreatByPtr( unitTarget );
	}
}

void Spell::SpellEffectAddComboPoints(uint32 i) // Add Combo Points
{
	if(!m_caster->IsPlayer()) return;
  
	//if this is a procspell Ruthlessness (maybe others later)
	if(pSpellId && m_spellInfo->Id==14157)
	{
		//it seems this combo adding procspell is going to change combopoint count before they will get reseted. We add it after the reset
		/* burlex - this is wrong, and exploitable.. :/ if someone uses this they will have unlimited combo points */
		//re-enabled this by Zack. Explained why it is used + recheked to make sure initialization is good ...
		// while casting a spell talent will trigger uppon the spell prepare faze
		// the effect of the talent is to add 1 combo point but when triggering spell finishes it will clear the extra combo point
		((Player *)m_caster)->m_spellcomboPoints += damage;
		return;
	}
	((Player *)m_caster)->AddComboPoints(((Player *)m_caster)->GetSelection(), damage);
}

void Spell::SpellEffectCreateHouse(uint32 i) // Create House
{


}

void Spell::SpellEffectDuel(uint32 i) // Duel
{
	if(!m_caster->IsPlayer()) return;
	if(!((Player *)m_caster)->isAlive()) return;

	if (((Player *)m_caster)->IsStealth())
	{
		SendCastResult(SPELL_FAILED_CANT_DUEL_WHILE_STEALTHED);
		return; // Player is stealth
	}
	if(!playerTarget) return;
	if(playerTarget == ((Player *)m_caster)) return;

	/* not implemented yet
	TODO: dueling zones ? ( SPELL_FAILED_NO_DUELING )
	if (player->IsInvisible())
	{
		SendCastResult(SPELL_FAILED_CANT_DUEL_WHILE_INVISIBLE);
		return;
	}
	*/

	//Player *pTarget = sObjHolder.GetObject<Player>(player->GetSelection());	   //  hacky.. and will screw up if plr is deselected..
	if (!playerTarget)
	{
		SendCastResult(SPELL_FAILED_BAD_TARGETS);
		return; // invalid Target
	}
	if (!playerTarget->isAlive())
	{
		SendCastResult(SPELL_FAILED_TARGETS_DEAD);
		return; // Target not alive
	}
	if (playerTarget->hasStateFlag(UF_ATTACKING))
	{
		SendCastResult(SPELL_FAILED_TARGET_IN_COMBAT);
		return; // Target in combat with another unit
	}
	if (playerTarget->DuelingWith != NULL)
	{
		SendCastResult(SPELL_FAILED_TARGET_DUELING);
		return; // Already Dueling
	}

	((Player *)m_caster)->RequestDuel(playerTarget);
}

void Spell::SpellEffectStuck(uint32 i)
{
    if(!playerTarget || playerTarget != ((Player *)m_caster))
        return;

	sEventMgr.AddEvent(playerTarget,&Player::EventTeleport,playerTarget->GetBindMapId(),playerTarget->GetBindPositionX(),playerTarget->GetBindPositionY(),
		playerTarget->GetBindPositionZ(),EVENT_PLAYER_TELEPORT,50,1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	/*
	playerTarget->SafeTeleport(playerTarget->GetBindMapId(), 0, playerTarget->GetBindPositionX(), playerTarget->GetBindPositionY(), playerTarget->GetBindPositionZ(), 3.14f);*/
}

void Spell::SpellEffectSummonPlayer(uint32 i)
{
	if(!playerTarget)
		return;

	/* --Old Checks.  Updated to 2.4.2 -BlizzHackerD
	if(m_caster->GetMapMgr()->GetMapInfo() && m_caster->GetMapMgr()->GetMapInfo()->type != INSTANCE_NULL && m_caster->GetMapId() != playerTarget->GetMapId())
		return;
	*/
	
	if(!m_caster->GetMapMgr()->GetMapInfo())
		// MapInfo doesn't exist.
		return;

	if(!playerTarget->triggerpass_cheat)
	// Only run requirement checks if player doesn't have the trigger cheat enabled.
	{
		if(playerTarget->getLevel() < m_caster->GetMapMgr()->GetMapInfo()->minlevel && m_caster->GetMapMgr()->GetMapInfo()->mapid != 530)
			// Summoned Player doesn't meet the level requirements for the map, and the map isn't Outland.
			return;

		if(m_caster->GetMapMgr()->GetMapInfo()->required_quest_A1 && !playerTarget->HasFinishedQuest(m_caster->GetMapMgr()->GetMapInfo()->required_quest_A1))
			// The Map has a quest requirement for Ally and the target player has not completed it yet.
			return;

		if(m_caster->GetMapMgr()->GetMapInfo()->required_quest_H2 && !playerTarget->HasFinishedQuest(m_caster->GetMapMgr()->GetMapInfo()->required_quest_H2))
			// The Map has a quest requirement for Horde and the target player has not completed it yet.
			return;

		if(m_caster->GetMapMgr()->GetMapInfo()->required_item && !playerTarget->GetItemInterface()->GetItemCount(m_caster->GetMapMgr()->GetMapInfo()->required_item, false))
			// The Map has an item requirement and the target player doesn't have that item in their inventory.
			return;

		if(m_caster->GetMapMgr()->GetMapInfo()->heroic_key_1 || m_caster->GetMapMgr()->GetMapInfo()->heroic_key_2)
		{
			// The Map has one or more heroic keys.
			if(m_caster->GetMapMgr()->GetMapInfo()->heroic_key_1 && playerTarget->GetItemInterface()->GetItemCount(m_caster->GetMapMgr()->GetMapInfo()->heroic_key_1, false) == 0)
			{
				// Player doesn't have a valid key for key 1.
				if(m_caster->GetMapMgr()->GetMapInfo()->heroic_key_2 && playerTarget->GetItemInterface()->GetItemCount(m_caster->GetMapMgr()->GetMapInfo()->heroic_key_2, false) == 0)
				{
					// Player doesn't have a valid key for key 2.
					return;
				}
			}
		}			
	}
	
	playerTarget->SummonRequest(m_caster->GetLowGUID(), m_caster->GetZoneId(), m_caster->GetMapId(),
		m_caster->GetInstanceID(), m_caster->GetPosition());
}

void Spell::SpellEffectActivateObject(uint32 i) // Activate Object
{
	/*if(!p_caster)
		return;

	if(!gameObjTarget)
		return;

	gameObjTarget->SetUInt32Value(GAMEOBJECT_DYNAMIC, 1);

	sEventMgr.AddEvent(gameObjTarget, &GameObject::Deactivate, EVENT_GAMEOBJECT_DEACTIVATE, GetDuration(), 1);*/
}

void Spell::SpellEffectSelfResurrect(uint32 i)
{
	if(!m_caster->IsPlayer())    return;
	if(playerTarget->isAlive())  return;
	uint32 mana;
	uint32 health;
	uint32 class_=unitTarget->getClass();
	
	switch(m_spellInfo->Id)
	{
	case 3026:
	case 20758:
	case 20759:
	case 20760:
	case 20761:
	case 27240:
		{
			health = m_spellInfo->EffectMiscValue[i];
			mana = -damage;
		}break;
	case 21169: //Reincarnation. Ressurect with 20% health and mana
		{
			health = uint32(unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.20);
			mana = uint32(unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1)*0.20);
		}
		break;
	default:
		{
			if(damage < 0) return;
			health = uint32(unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*damage/100);
			mana = uint32(unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1)*damage/100);
		}break;
        }

	if(class_ == WARRIOR || class_ == ROGUE)
		mana=0;
	
	playerTarget->m_resurrectHealth = health;
	playerTarget->m_resurrectMana = mana;

	playerTarget->ResurrectPlayer(NULL);
	playerTarget->SetMovement(MOVE_UNROOT, 1);

	playerTarget->SetUInt32Value(PLAYER_SELF_RES_SPELL, 0);

	if(m_spellInfo->Id==__Reincarnation) AddCooldown(); // 21169

}

void Spell::SpellEffectSkinning(uint32 i)
{
	if( unitTarget == NULL )
		return;

	uint32 sk = static_cast< Player* >( m_caster )->_GetSkillLineCurrent( SKILL_SKINNING );
	uint32 lvl = unitTarget->getLevel();

	if( ( sk >= lvl * 5 ) || ( ( sk + 100 ) >= lvl * 10 ) )
	{
		//Fill loot for Skinning
		//lootmgr.FillSkinningLoot(&((Creature*)unitTarget)->loot,unitTarget->GetEntry());
		lootmgr.FillSkinningLoot(&unitTarget->loot,unitTarget->GetEntry());
		static_cast< Player* >( m_caster )->SendLoot( unitTarget->GetGUID(), LOOT_SKINNING );
		
		//Not skinable again
		((Creature*)unitTarget)->Skinned = true;
		unitTarget->BuildFieldUpdatePacket( ((Player *)m_caster), UNIT_FIELD_FLAGS, 0 );

		//still lootable
		//pkt=unitTarget->BuildFieldUpdatePacket(UNIT_DYNAMIC_FLAGS,U_DYN_FLAG_LOOTABLE);
		//static_cast< Player* >( m_caster )->GetSession()->SendPacket(pkt);
		//delete pkt;	 
		DetermineSkillUp(SKILL_SKINNING,sk<lvl*5?sk/5:lvl);
	}
	else
	{
		SendCastResult(SPELL_FAILED_TARGET_UNSKINNABLE);
	}   
			
//	DetermineSkillUp(SKILL_SKINNING,unitTarget->getLevel());
}

void Spell::SpellEffectCharge(uint32 i)
{
	if(!unitTarget) return;
	if(!unitTarget->IsUnit()) return;
	if(!m_caster->IsUnit()) return;
	//if(!p_caster) who said units can't charge? :P
	//	return;
	if(!unitTarget->isAlive()) return;
    if (((Unit *)m_caster)->IsStunned() || ((Unit *)m_caster)->m_auracount[SPELL_AURA_MOD_ROOT] || 
		((Unit *)m_caster)->IsPacified() || ((Unit *)m_caster)->IsFeared())
        return;

	float x, y, z;
	float dx,dy;

	//if(unitTarget->GetTypeId() == TYPEID_UNIT)
	//	if(unitTarget->GetAIInterface())
	//		unitTarget->GetAIInterface()->StopMovement(5000);
	if(unitTarget->GetPositionX() == 0.0f || unitTarget->GetPositionY() == 0.0f)
		return;
	
	dx=unitTarget->GetPositionX()-((Unit *)m_caster)->GetPositionX();
	dy=unitTarget->GetPositionY()-((Unit *)m_caster)->GetPositionY();
	if(dx == 0.0f || dy == 0.0f)
		return;

	float d = sqrt(dx*dx+dy*dy)-unitTarget->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS)-((Unit *)m_caster)->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS);
	float alpha = atanf(dy/dx);
	if(dx<0)
		alpha += float(M_PI);

	x = d * cosf(alpha) + m_caster->GetPositionX();
	y = d * sinf(alpha) + m_caster->GetPositionY();
	z = unitTarget->GetPositionZ();

	uint32 time = uint32( (m_caster->CalcDistance(unitTarget) / ((((Unit *)m_caster)->m_runSpeed * 3.5) * 0.001f)) + 0.5);

	((Unit *)m_caster)->GetAIInterface()->SendMoveToPacket(x, y, z, alpha, time, MONSTER_MOVE_FLAG_RUN);
	((Unit *)m_caster)->SetPosition(x,y,z,alpha,true);

	if(unitTarget && unitTarget->GetTypeId() == TYPEID_UNIT)
		unitTarget->GetAIInterface()->StopMovement(time);	
	
	((Unit *)m_caster)->addStateFlag(UF_ATTACKING);
	if(unitTarget)
		((Unit *)m_caster)->smsg_AttackStart( unitTarget );

	((Unit *)m_caster)->setAttackTimer(time, false);
	((Unit *)m_caster)->setAttackTimer(time, true);
	if( m_caster->IsPlayer() )
	{
		((Player *)m_caster)->EventAttackStart();
		((Player *)m_caster)->ResetHeartbeatCoords();
		((Player *)m_caster)->DelaySpeedHack( time + 1000 );
		((Player *)m_caster)->z_axisposition = 0.0f;
	}
}

void Spell::SpellEffectPlayerPull( uint32 i )
{
	if( unitTarget == NULL || !unitTarget->isAlive()) return;
	if( !unitTarget->IsUnit()) return;
	if( !m_caster->IsUnit()) return;

	// calculate destination, use target to target for Z, we don't increase distance for Z difference :)
	float pullD = unitTarget->CalcDistance( m_caster->GetPositionX(), m_caster->GetPositionY(), unitTarget->GetPositionZ() ) - unitTarget->GetFloatValue( UNIT_FIELD_BOUNDINGRADIUS ) - m_caster->GetFloatValue( UNIT_FIELD_BOUNDINGRADIUS ) - 1.0f;
	float pullO = unitTarget->calcRadAngle( unitTarget->GetPositionX(), unitTarget->GetPositionY(), m_caster->GetPositionX(), m_caster->GetPositionY() );
	float pullX = unitTarget->GetPositionX() + pullD * cosf( pullO );
	float pullY = unitTarget->GetPositionY() + pullD * sinf( pullO );
	float pullZ = m_caster->GetPositionZ() + 0.3f;
	uint32 time = uint32( pullD * 42.0f );

	unitTarget->SetOrientation( pullO );

	if (pullD == 0)
		return;

	if (playerTarget != NULL)
	{
		WorldPacket data(SMSG_MOVE_KNOCK_BACK, 50);
		data << playerTarget->GetNewGUID();
		data << getMSTime();
		data << cosf(pullO) << sinf(pullO);
		data << float(pullD);
		data << float(-(float)m_spellInfo->EffectMiscValue[i]/10);
		playerTarget->GetSession()->SendPacket(&data);
		playerTarget->DelaySpeedHack(5000);
	}
	else
	{
		unitTarget->SetPosition(pullX, pullY, pullZ, pullO);
		WorldPacket data( SMSG_MONSTER_MOVE, 60 );
		data << unitTarget->GetNewGUID();
		data << unitTarget->GetPositionX() << unitTarget->GetPositionY() << unitTarget->GetPositionZ();
		data << getMSTime();
		data << uint8( 4 );
		data << pullO;
		data << uint32(0x2);
		data << time;
		data << uint32( 1 );
		data << pullX << pullY << pullZ;
		unitTarget->SendMessageToSet( &data, true ); 
	}

	//remove mount auras if possible
	if (playerTarget)
	{
		if (playerTarget->m_MountSpellId != 0)
			playerTarget->RemoveAura(playerTarget->m_MountSpellId);
		if (playerTarget->flying_aura != 0 && playerTarget->GetShapeShift() == 0)
			playerTarget->RemoveAura(playerTarget->flying_aura);
	}
}

void Spell::SpellEffectKnockBack(uint32 i)
{
	if(unitTarget == NULL || !unitTarget->isAlive())
		return;

	float dx, dy;
	float value1 = float(m_spellInfo->EffectBasePoints[i]+1);
	float value2 = float(m_spellInfo->EffectMiscValue[i]);
	float proportion;
	value2 ? proportion = value1/value2 : proportion = 0;

    if(proportion)
	{
		value1 = value1 / (10 * proportion);
		value2 = value2 / 10 * proportion;
	}
	else
	{
		value2 = value1 / 10;
		value1 = 0.1f;
	}

	float angle = m_caster->calcAngle(m_caster->GetPositionX(), m_caster->GetPositionY(), unitTarget->GetPositionX(), unitTarget->GetPositionY()) * float(M_PI) / 180.0f;

	dx = cosf(angle);
	dy = sinf(angle);

	//set the position
	if (unitTarget->IsCreature())
	{
		float x=unitTarget->GetPositionX()+(value1*dx);
		float y=unitTarget->GetPositionY()+(value1*dy);
		float z=unitTarget->GetPositionZ();
		unitTarget->SetPosition(x, y, z, 0);
		WorldPacket data(SMSG_MONSTER_MOVE, 50);
		data << unitTarget->GetNewGUID();
		data << uint8(0);
		data << unitTarget->GetPositionX();
		data << unitTarget->GetPositionY();
		data << unitTarget->GetPositionZ();
		data << getMSTime();
		data << uint8( 4 );
		data << unitTarget->calcRadAngle(unitTarget->GetPositionX(), unitTarget->GetPositionY(), m_caster->GetPositionX(), m_caster->GetPositionY());
		data << uint32(0x00000002);
		data << 500;
		data << uint32(1);
		data << x << y << z;
		unitTarget->SendMessageToSet(&data, true);
		unitTarget->GetAIInterface()->StopMovement(2000);
	}

	//unitTarget->SendMessageToSet(&data, true);

	if (playerTarget != NULL)
	{
		WorldPacket data(SMSG_MOVE_KNOCK_BACK, 50);
		data << playerTarget->GetNewGUID();
		data << getMSTime();
		data << dx << dy;
		data << value1;
		data << -value2;
		playerTarget->blinked = true;
		playerTarget->GetSession()->SendPacket(&data);

		//remove mount auras if possible
		if (playerTarget->m_MountSpellId != 0)
			playerTarget->RemoveAura(playerTarget->m_MountSpellId);
		if (playerTarget->flying_aura != 0 && playerTarget->GetShapeShift() == 0)
			playerTarget->RemoveAura(playerTarget->flying_aura);
	}
}

void Spell::SpellEffectDisenchant(uint32 i)
{
	if(!m_caster->IsPlayer()) return;

	Player* caster = static_cast< Player* >( m_caster );

	if (m_targets.m_target == NULL || !m_targets.m_target->IsItem() || static_cast<Item*>(m_targets.m_target)->IsContainer())
	{
		SendCastResult(SPELL_FAILED_CANT_BE_DISENCHANTED);
		return;
	}

	Item* it = static_cast<Item*>(m_targets.m_target);

	//Check for skill first, we can increase it upto 75 
	uint32 skill=caster->_GetSkillLineCurrent( SKILL_ENCHANTING );
	if(skill < 75)//can up skill
	{
		if(Rand(float(100-skill*100.0/75.0)))
		{
			caster->_AdvanceSkillLine(SKILL_ENCHANTING, float2int32( 1.0f * sWorld.getRate(RATE_SKILLRATE)));
		}
	}
	//Fill disenchanting loot
	caster->SetLootGUID(it->GetGUID());
	if(!it->loot)
	{
		it->loot = new Loot;
		lootmgr.FillDisenchantingLoot(it->loot, it->GetEntry());
	}
	if ( it->loot->items.size() > 0 )
	{
		Log.Debug("SpellEffect","Succesfully disenchanted item %d", uint32(itemTarget->GetEntry()));
		((Player *)m_caster)->SendLoot( itemTarget->GetGUID(), LOOT_DISENCHANTING );
	} 
	else
	{
		Log.Debug("SpellEffect","Disenchanting failed, item %d has no loot", uint32(itemTarget->GetEntry()));
		SendCastResult(SPELL_FAILED_CANT_BE_DISENCHANTED);
	}
	//if(it==i_caster) i_caster=NULL; OBSOLETE (Brz)
}

void Spell::SpellEffectInebriate(uint32 i) // lets get drunk!
{
	if(!m_caster->IsPlayer()) return;

	// Drunkee!
	uint8 b2 = m_caster->GetByte(PLAYER_BYTES_3,1);
	b2 += damage;	// 10 beers will get you smassssshed!
  
	((Player *)m_caster)->SetByte(PLAYER_BYTES_3,1, (b2 > 90) ? 90 : b2);
	sEventMgr.RemoveEvents(((Player *)m_caster), EVENT_PLAYER_REDUCEDRUNK);
	sEventMgr.AddEvent(((Player *)m_caster), &Player::EventReduceDrunk, false, EVENT_PLAYER_REDUCEDRUNK, 300000, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Spell::SpellEffectFeedPet(uint32 i)  // Feed Pet
{   // Spell : 6991, Eff1=101, Eff2=64  Misc1 1539, Misc2 51284
	// Target 0x10 (16d), Attrib 0x18010010 AttribEx 0x00020000
	// food flags and food level are checked in Spell::CanCast()
	Log.Warning("Familier","SpellEffectFeedPet : Nourrir le familier (Param: %u)",i);

	if(!m_targets.m_target)
	{
	 Log.Error("Familier","Nourrir: m_target = NULL <- Report this to devs."); // Impossible
	 return;
	}

	if(!m_targets.m_target->IsItem())
	{
	 Log.Error("Familier","Nourrir: Erreur, n'est pas un item "); // Impossible
	 return;
	}

	if(!m_caster->IsPlayer()) // Seulement le player
	{
		return;
	}
	
	Pet *pPet = ((Player *)m_caster)->GetSummon();
	if(!pPet) 
	{
        Log.Error("Familier","Nourrir: Pas de familier"); // Impossible
		return;
	}

	/**	Cast feed pet effect
	- effect is item level and pet level dependent, aura ticks are 35, 17, 8 (*1000) happiness
	- http://petopia.brashendeavors.net/html/articles/basics_feeding.shtml */
	int8 deltaLvl = pPet->getLevel() - ((Item *)m_targets.m_target)->GetProto()->ItemLevel;
	damage /= 1000; //damage of Feed pet spell is 35000
	if(deltaLvl > 10) damage = damage >> 1;//divide by 2
	if(deltaLvl > 20) damage = damage >> 1;
	damage *= 1000;
	//------------- LOYALTY/HAPPINESS ------------- http://www.wowwiki.com/Feed_Pet
	// If the level of the food is lower 35 levels lower then the pet it gains 4 Happiness per tick, // Waad JdR ;)
	// If the level of the food is 35-25 levels lower then the pet it gains 8 Happiness per tick, 
	// If the level of the food is level 25-15 it gains 17 happiness, 
	// If the level of the food is 15 levels lower or higher it gains 35 or 45 happiness. 
	// If the level of the food is 15 levels higher it gains 55 happiness. // Waad JdR ;)
	int32 happy_value  = pPet->GetLoyaltyPts();   // LoyaltyPts, Le tick
	// (+): Le level de l'item est plus petit que le level du familier
    // (0): Le level de l'item est = au level du familier
	// (-): Le level de l'item est plus grand que le level du familier
	     if( deltaLvl >= 35 ) happy_value +=  4;  // ItemLevel as at less 35+ lower PetLevel
	else if( deltaLvl >= 25 ) happy_value +=  8;  // ItemLevel as at less 35-25 lower PetLevel
	else if( deltaLvl >= 15 ) happy_value += 17;  // ItemLevel as at less 25-15 lower PetLevel 
	else if( deltaLvl >= ((int8)pPet->getLevel())) happy_value += 35;  // ItemLevel as less -15 lower PetLevel 
	else if( deltaLvl <= -15) happy_value += 45;  // ItemLevel as less than PetLevel+15
	else                      happy_value += 55;  // ItemLevel as more than PetLevel+15 
	pPet->SetLoyaltyPts((uint32)happy_value); 
    //--------------------------------------------

	if(m_spellInfo->EffectTriggerSpell[i])
	{
	 SpellEntry *spellInfo = dbcSpell.LookupEntry(m_spellInfo->EffectTriggerSpell[i]);
	 Spell *sp= new Spell(((Player *)m_caster),spellInfo,true,NULL);
	 sp->forced_basepoints[0] = damage - 1; // A verifier... (Branruz)
	 SpellCastTargets tgt;
	 tgt.m_target=pPet;
	 sp->prepare(&tgt);
	}

	if(((Item *)m_targets.m_target)->GetUInt32Value(ITEM_FIELD_STACK_COUNT)>1) // Decompte
	{
		((Item *)m_targets.m_target)->ModUnsigned32Value(ITEM_FIELD_STACK_COUNT, -1);
		((Item *)m_targets.m_target)->m_isDirty=true;
	}
	else // Plus d'item 
	{
		((Player *)m_caster)->GetItemInterface()->SafeFullRemoveItemByGuid(((Item *)this->m_targets.m_target)->GetGUID());
		m_targets.m_target = NULL;
	}
}

void Spell::SpellEffectReputation(uint32 i)
{
	if(!playerTarget)
		return;

	//playerTarget->modReputation(m_spellInfo->EffectMiscValue[i], damage, true);
	playerTarget->ModStanding(m_spellInfo->EffectMiscValue[i], damage);
}

void Spell::SpellEffectSummonObjectSlot(uint32 i)
{
	if(!m_caster->IsUnit()) return;
	if(!((Unit *)m_caster)->IsInWorld()) return;

	GameObject *GoSummon = NULL;

	uint32 slot=m_spellInfo->Effect[i] - SPELL_EFFECT_SUMMON_OBJECT_SLOT1;
	GoSummon = ((Unit *)m_caster)->m_ObjectSlots[slot] ? ((Unit *)m_caster)->GetMapMgr()->GetGameObject(((Unit *)m_caster)->m_ObjectSlots[slot]) : NULL;
	((Unit *)m_caster)->m_ObjectSlots[slot] = 0;
	
	if( GoSummon )
	{
		if(GoSummon->GetInstanceID() != ((Unit *)m_caster)->GetInstanceID())
			GoSummon->ExpireAndDelete();
		else
		{
			if( GoSummon->IsInWorld() )
				GoSummon->RemoveFromWorld(true);
			delete GoSummon;
		}
	}

   
	// spawn a new one
	GoSummon = ((Unit *)m_caster)->GetMapMgr()->CreateGameObject(m_spellInfo->EffectMiscValue[i]);
   if(! GoSummon->CreateFromProto(m_spellInfo->EffectMiscValue[i],
		((Unit *)m_caster)->GetMapId(), ((Unit *)m_caster)->GetPositionX(), 
		((Unit *)m_caster)->GetPositionY(), ((Unit *)m_caster)->GetPositionZ(), ((Unit *)m_caster)->GetOrientation() ))
   {
		//delete GoSummon;
		return;
   }
	
	GoSummon->SetUInt32Value(GAMEOBJECT_LEVEL, ((Unit *)m_caster)->getLevel());
	GoSummon->SetUInt64Value(OBJECT_FIELD_CREATED_BY, ((Unit *)m_caster)->GetGUID()); 
	GoSummon->SetInstanceID(((Unit *)m_caster)->GetInstanceID());

	if(GoSummon->GetByte(GAMEOBJECT_BYTES_1, 1) == GAMEOBJECT_TYPE_TRAP)
	{
		GoSummon->invisible = true;
		GoSummon->invisibilityFlag = INVIS_FLAG_TRAP;
		GoSummon->charges = 1;
		GoSummon->checkrate = 1;
		sEventMgr.AddEvent(GoSummon, &GameObject::TrapSearchTarget, EVENT_GAMEOBJECT_TRAP_SEARCH_TARGET, 100, 0,0);
	}
	else
	{
		sEventMgr.AddEvent(GoSummon, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_EXPIRE, GetDuration(INVOC_DURATION), 1,0);
	}
	GoSummon->PushToWorld(((Unit *)m_caster)->GetMapMgr());
	GoSummon->SetSummoned(((Unit *)m_caster));
	((Unit *)m_caster)->m_ObjectSlots[slot] = GoSummon->GetUIdFromGUID();
}

void Spell::SpellEffectDispelMechanic(uint32 i)
{
	if( !unitTarget || !unitTarget->isAlive() )
		return;
	int32 sMisc = (int32)m_spellInfo->EffectMiscValue[i];
	uint32 x;
	SpellEntry *p;

	if( (unitTarget && m_caster->IsUnit()) || !isAttackable( m_caster, unitTarget ) )
	{
		for( x = MAX_POSITIVE_AURAS ; x<MAX_AURAS ; x++ )
		{
			if( unitTarget->m_auras[x] != NULL && !unitTarget->m_auras[x]->IsPositive())
			{
				p = unitTarget->m_auras[x]->GetSpellProto();
				if( Spell::HasMechanic(p, sMisc) )
				{
					unitTarget->m_auras[x]->Remove();
				}
			}
		}
	}
	else
	{
		for( x = 0 ; x<MAX_POSITIVE_AURAS ; x++ )
		{
			if( unitTarget->m_auras[x] != NULL && unitTarget->m_auras[x]->IsPositive())
			{
				p = unitTarget->m_auras[x]->GetSpellProto();
				if( Spell::HasMechanic(p, sMisc) )
				{
					unitTarget->m_auras[x]->Remove();
				}
			}
		}
	}

	unitTarget->RemoveAllAurasByMechanic( m_spellInfo->EffectMiscValue[i] , m_spellInfo->EffectBasePoints[i] , true );
	if( playerTarget && m_spellInfo->NameHash == SPELL_HASH_DAZED && playerTarget->IsMounted() )
		playerTarget->RemoveAura(playerTarget->m_MountSpellId);
}

void Spell::SpellEffectSummonDeadPet(uint32 i)
{
	if(!m_caster->IsPlayer()) return;

	Pet* pPet = ((Player *)m_caster)->GetSummon();

	if(pPet)
	{
		pPet->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
		pPet->SetUInt32Value(UNIT_FIELD_HEALTH, (uint32)(pPet->GetUInt32Value(UNIT_FIELD_MAXHEALTH) * (damage / 100)));
		pPet->setDeathState(ALIVE);
		pPet->GetAIInterface()->HandleEvent(EVENT_FOLLOWOWNER, pPet, 0);
		sEventMgr.RemoveEvents(pPet, EVENT_PET_DELAYED_REMOVE);
	}
}

/* This effect has 2 functions
 * 1. It delete's all current totems from the player
 * 2. It returns a percentage of the mana back to the player
 *
 * Bur kick my ass if this is not safe:P
*/

/*void Spell::SpellEffectDestroyAllTotems(uint32 i)
{
	if(!p_caster || !p_caster->IsInWorld()) return;

	float RetreivedMana = 0.0f;
	for(uint32 x=1;x<5;x++)
	{
		// atm totems are considert creature's
		if(p_caster->m_summonslot[x])
		{
			uint32 SpellID = p_caster->m_summonslot[x]->GetUInt32Value(UNIT_CREATED_BY_SPELL);
			SpellEntry * sp = dbcSpell.LookupEntry(SpellID);
			if (sp == NULL)
				continue;

			float pts = float(m_spellInfo->EffectBasePoints[i]+1) / 100.0f;
			RetreivedMana += float(sp->manaCost) * pts;

			p_caster->m_summonslot[x]->SafeDelete();
		}

		if(p_caster->m_ObjectSlots[x])
		{
			GameObject * obj = p_caster->GetMapMgr()->GetGameObject(p_caster->m_ObjectSlots[x]);
			if(obj)
			{
				obj->ExpireAndDelete();
			}
			p_caster->m_ObjectSlots[x] = 0;
		}
	}

	// get the current mana, get the max mana. Calc if we overflow
	SendHealManaSpellOnPlayer(m_caster, m_caster, (uint32)RetreivedMana, 0);
	RetreivedMana += float(m_caster->GetUInt32Value(UNIT_FIELD_POWER1));
	uint32 max = m_caster->GetUInt32Value(UNIT_FIELD_MAXPOWER1);
	if((uint32)RetreivedMana > max)
		RetreivedMana = (float)max;
	m_caster->SetUInt32Value(UNIT_FIELD_POWER1, (uint32)RetreivedMana);
}*/

void Spell::SpellEffectDestroyAllTotems(uint32 i)
{
	if(!m_caster->IsPlayer()) return;
	if(!((Player *)m_caster)->IsInWorld()) return;

	uint32 energize_amt = 0;
	for(uint32 x=SUMMON_TYPE_TOTEM_1;x<SUMMON_TYPE_TOTEM_4+1;x++)
	{
		SummonPropertiesEntry * spe = dbcSummonProperties.LookupEntryForced(x);
		if(!spe) continue;

		// Ahh this code sucks
		if( x != SUMMON_TYPE_TOTEM_1 && x!= SUMMON_TYPE_TOTEM_2 && x != SUMMON_TYPE_TOTEM_3 && x != SUMMON_TYPE_TOTEM_4 )
			continue;

		uint32 slot = spe->slot;
		// atm totems are considert creature's
		if(((Player *)m_caster)->m_summonslot[slot])
		{
			uint32 SpellID = ((Player *)m_caster)->m_summonslot[slot]->GetUInt32Value(UNIT_CREATED_BY_SPELL);
			SpellEntry * sp = dbcSpell.LookupEntry(SpellID);
			if (!sp)
				continue;

			if( sp->manaCost )
				energize_amt += float2int32( (sp->manaCost) * (damage/100.0f) );
			else if( sp->ManaCostPercentage )
				energize_amt += float2int32(((((Player *)m_caster)->GetUInt32Value(UNIT_FIELD_BASE_MANA)*sp->ManaCostPercentage)/100.0f) * (damage/100.0f) );

			((Player *)m_caster)->m_summonslot[slot]->TotemExpire();
			((Player *)m_caster)->m_summonslot[slot] = NULL;
		}
	}

	if( energize_amt > 0 )
		((Player *)m_caster)->Energize(((Player *)m_caster), m_spellInfo->Id, energize_amt, POWER_TYPE_MANA);
}

void Spell::SpellEffectSummonDemon(uint32 i)
{
	if (!m_caster->IsUnit())	return;
	Pet *pPet = ((Player *)m_caster)->GetSummon();

	if(pPet != NULL)
		pPet->Dismiss(false);

	CreatureInfo* ci = CreatureNameStorage.LookupEntry(m_spellInfo->EffectMiscValue[i]);
	if(ci != NULL)
	{
		pPet = objmgr.CreatePet();
		pPet->SetInstanceID(((Player *)m_caster)->GetInstanceID());
		pPet->CreateAsSummon(m_spellInfo->EffectMiscValue[i], ci, NULL, ((Player *)m_caster), m_spellInfo, CREATURE_TYPE_CRITTER, GetDuration(INVOC_DURATION), this);
	}
}

void Spell::SpellEffectResurrect(uint32 i) // Resurrect (Flat)
{
	if(!playerTarget)
	{
		if(!corpseTarget)
		{
			// unit resurrection handler
			if(unitTarget)
			{
				if(unitTarget->GetTypeId()==TYPEID_UNIT && unitTarget->IsPet() && unitTarget->isDead())
				{
					uint32 hlth = ((uint32)m_spellInfo->EffectBasePoints[i] > unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH)) ? unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH) : (uint32)m_spellInfo->EffectBasePoints[i];
					uint32 mana = ((uint32)m_spellInfo->EffectBasePoints[i] > unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1)) ? unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1) : (uint32)m_spellInfo->EffectBasePoints[i];

					if(!unitTarget->IsPet())
					{
						sEventMgr.RemoveEvents(unitTarget, EVENT_CREATURE_REMOVE_CORPSE);
					}
					else
					{
						sEventMgr.RemoveEvents(unitTarget, EVENT_PET_DELAYED_REMOVE);
						sEventMgr.RemoveEvents(unitTarget, EVENT_CREATURE_REMOVE_CORPSE);
					}
					unitTarget->SetUInt32Value(UNIT_FIELD_HEALTH, hlth);
					unitTarget->SetUInt32Value(UNIT_FIELD_POWER1, mana);
					unitTarget->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
					unitTarget->setDeathState(ALIVE);
					((Creature*)unitTarget)->Tagged=false;
					((Creature*)unitTarget)->TaggerGuid=false;
					((Creature*)unitTarget)->loot.gold=0;
					//((Creature*)unitTarget)->loot.looters.clear();
					((Creature*)unitTarget)->ClearLoot();
					((Creature*)unitTarget)->loot.items.clear();
				}
			}
			return;
		}
		playerTarget = objmgr.GetPlayer(corpseTarget->GetUInt32Value(CORPSE_FIELD_OWNER));
		if(!playerTarget) return;
	}

	if(playerTarget->isAlive() || !playerTarget->IsInWorld())
		return;

	uint32 health = m_spellInfo->EffectBasePoints[i];
	uint32 mana = m_spellInfo->EffectMiscValue[i];
	
	playerTarget->m_resurrectHealth = health;
	playerTarget->m_resurrectMana = mana;

	SendResurrectRequest(playerTarget);   
	playerTarget->SetMovement(MOVE_UNROOT, 1);
}

void Spell::SpellEffectAttackMe(uint32 i)
{
	if(!unitTarget)
		return;
	if(!unitTarget->isAlive())
		return;

	unitTarget->GetAIInterface()->AttackReaction(((Unit *)m_caster),1,0);	
}

void Spell::SpellEffectSkinPlayerCorpse(uint32 i)
{
	Corpse * corpse = 0;
	if(!playerTarget)
	{
		// means we're "skinning" a corpse
		if (m_targets.m_target != NULL && m_targets.m_target->GetTypeId() == TYPEID_CORPSE)
		corpse = static_cast<Corpse*>(m_targets.m_target);  // hacky
	}
	else if(playerTarget->getDeathState() == CORPSE)	// repopped while we were casting 
	{
		corpse = objmgr.GetCorpse(playerTarget->GetLowGUID());
	}

	if(!m_caster->IsPlayer()) 
		return;
 
	if(playerTarget && !corpse)
	{
		if(!playerTarget->m_bg || !playerTarget->isDead())
			return;

		// Set all the lootable stuff on the player. If he repops before we've looted, we'll set the flags
		// on corpse then :p

		playerTarget->bShouldHaveLootableOnCorpse = false;
		playerTarget->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
		playerTarget->SetFlag(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_LOOTABLE);

		// Send the loot.
		((Player *)m_caster)->SendLoot(playerTarget->GetGUID(), LOOT_SKINNING);

		// Send a message to the died player, telling him he has to resurrect at the graveyard.
		// Send an empty corpse location too, :P
		
		playerTarget->GetSession()->OutPacket(SMSG_PLAYER_SKINNED, 1, "\x00");
		playerTarget->GetSession()->OutPacket(MSG_CORPSE_QUERY, 1, "\x00");

		// don't allow him to spawn a corpse
		playerTarget->bCorpseCreateable = false;

		// and.. force him to the graveyard and repop him.
		playerTarget->RepopRequestedPlayer();

	}else if(corpse)
	{
		// find the corpses' owner
		Player * owner = objmgr.GetPlayer(corpse->GetUInt32Value(CORPSE_FIELD_OWNER));
		if(owner)
		{
			if(owner->m_bg == NULL)
				return;

			owner->GetSession()->OutPacket(SMSG_PLAYER_SKINNED, 1, "\x00");
			owner->GetSession()->OutPacket(MSG_CORPSE_QUERY, 1, "\x00");
		}

		if(corpse->GetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS) != 1)
			corpse->SetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS, 1); // sets it so you can loot the plyr
		
		// remove skinnable flag
		corpse->SetUInt32Value(CORPSE_FIELD_FLAGS, 5);

		// remove owner association
		corpse->SetUInt64Value(CORPSE_FIELD_OWNER, 0);
		corpse->SetCorpseState(CORPSE_STATE_BONES);
		corpse->DeleteFromDB();
		objmgr.CorpseAddEventDespawn(corpse);

		// send loot
		((Player *)m_caster)->SendLoot(corpse->GetGUID(), LOOT_SKINNING);
	}
}

void Spell::SpellEffectSkill(uint32 i)
{
	// Used by professions only
	// Effect should be renamed in RequireSkill

	if ( !m_caster->IsPlayer()) return;
	if ( ((Player *)m_caster)->_GetSkillLineMax(m_spellInfo->EffectMiscValue[i]) >= uint32( damage * 75 ) ) return;
	
	if ( ((Player *)m_caster)->_HasSkillLine( m_spellInfo->EffectMiscValue[i]) )
		((Player *)m_caster)->_ModifySkillMaximum( m_spellInfo->EffectMiscValue[i], uint32( damage * 75 ) );
	else
		((Player *)m_caster)->_AddSkillLine( m_spellInfo->EffectMiscValue[i], 1, uint32( damage * 75 ) );
}

void Spell::SpellEffectApplyPetAura(uint32 i)
{
	SpellEffectApplyAura(i);
}

void Spell::SpellEffectDummyMelee( uint32 i ) // Normalized Weapon damage +
{
	if( unitTarget == NULL ) return;
	if( !m_caster->IsPlayer() ) return;

	if (m_spellScript != NULL)
		m_spellScript->DummyMeleeEffect(i);

	if( m_spellInfo->NameHash == SPELL_HASH_OVERPOWER ) //warrior : overpower - let us clear the event and the combopoint count
	{
		((Player *)m_caster)->NullComboPoints(); //some say that we should only remove 1 point per dodge. Due to cooldown you can't cast it twice anyway..
		sEventMgr.RemoveEvents( ((Player *)m_caster), EVENT_COMBO_POINT_CLEAR_FOR_TARGET );
	}

	switch( m_spellInfo->Id )
	{
			// AMBUSH
	case 8676: add_damage = 70; return; break;			// r1
	case 8724: add_damage = 100; return; break;			// r2
	case 8725: add_damage = 125; return; break;			// r3
	case 11267: add_damage = 185; return; break;		// r4
	case 11268: add_damage = 230; return; break;		// r5
	case 11269: add_damage = 290; return; break;		// r6
	case 27441: add_damage = 335; return; break;		// r7

			// BACKSTAB
	case 53: add_damage = 15; return; break;			// r1
	case 2589: add_damage = 30; return; break;			// r2
	case 2590: add_damage = 48; return; break;			// r3
	case 2591: add_damage = 69; return; break;			// r4
	case 8721: add_damage = 90; return; break;			// r5
	case 11279: add_damage = 135; return; break;		// r6
	case 11280: add_damage = 165; return; break;		// r7
	case 11281: add_damage = 210; return; break;		// r8
	case 25300: add_damage = 225; return; break;		// r9
	case 26863: add_damage = 255; return; break;		// r10
	}

	// rogue ambush etc
	for (uint32 x =0;x<3;x++)
		if(m_spellInfo->Effect[x] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE)
		{
			add_damage = damage * (m_spellInfo->EffectBasePoints[x]+1) /100;
			return;
		}

	//rogue - mutilate ads dmg if target is poisoned
	uint32 pct_dmg_mod = 100;
	if(	m_spellInfo->NameHash == SPELL_HASH_MUTILATE && unitTarget->IsPoisoned() )
		pct_dmg_mod = 150;

	uint32 _type;
	if( GetType() == DEFENSE_TYPE_RANGED )
		_type = RANGED;
	else
	{            
		if (m_spellInfo->attributesExC & ATTRIBUTESEXC_OFFHAND)
			_type =  OFFHAND;
		else
			_type = MELEE;
	}
	((Player *)m_caster)->Strike( unitTarget, _type, m_spellInfo, damage, pct_dmg_mod, 0, false, true );
}

void Spell::SpellEffectSendTaxi( uint32 i )
{
	if ( playerTarget == NULL || !playerTarget->isAlive() ) //|| u_caster == NULL )
		return;

	if( playerTarget->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER ) )
		return;

	TaxiPath* taxipath = sTaxiMgr.GetTaxiPath( m_spellInfo->EffectMiscValue[i] );

	if( !taxipath )
		return;

	TaxiNode* taxinode = sTaxiMgr.GetTaxiNode( taxipath->GetFromNode() );

	if( !taxinode )
		return;

	uint32 mount_entry = 0;
	uint32 modelid = 0;

	if( playerTarget->GetTeam() )		// HORDE
	{
		mount_entry = taxinode->horde_mount;
		if( !mount_entry )
			mount_entry = taxinode->alliance_mount;

		CreatureInfo* ci = CreatureNameStorage.LookupEntry( mount_entry );
		if( !ci )
			return;

		modelid = ci->Female_DisplayID;
		if( !modelid ) 
			return;
	}
	else								// ALLIANCE
	{
		mount_entry = taxinode->alliance_mount;
		if( !mount_entry )
			mount_entry = taxinode->horde_mount;

		CreatureInfo* ci = CreatureNameStorage.LookupEntry( mount_entry );
		if( !ci ) 
			return;

		modelid = ci->Male_DisplayID;
		if( !modelid ) 
			return;
	}

	playerTarget->TaxiStart( taxipath, modelid, 0 );
}

void Spell::SpellEffectSpellSteal( uint32 i )
{
	//Log.Notice("[Spell::SpellEffectSpellSteal]","In progress...");

	if (!unitTarget || !m_caster->IsUnit() || !unitTarget->isAlive())
		return;

	Aura* aur = NULL;
	uint32 start,end;
	int32 spells_to_steal = damage;
	if(isAttackable(((Unit *)m_caster),unitTarget))
	{
		start=0;
		end=MAX_POSITIVE_AURAS;
	}
	else
		return;
	
	WorldPacket data(SMSG_SPELLDISPELLOG, 16);

	for (uint32 x=start; x<end; x++)
	if(unitTarget->m_auras[x] != NULL)
	{
		aur = unitTarget->m_auras[x];
		if(aur->GetSpellId() != 15007 && !aur->IsPassive() && aur->IsPositive()) //Nothing can dispel resurrection sickness
		{
			if(aur->GetSpellProto()->dispelType == DISPEL_MAGIC)
			{
				data.clear();
				data << ((Unit *)m_caster)->GetNewGUID();
				data << unitTarget->GetNewGUID();
				data << (uint32)1;
				data << aur->GetSpellId();
				m_caster->SendMessageToSet(&data,true);                           // L'aura max est limité a 2 min
				Aura* aura = new Aura(aur->GetSpellProto(), (aur->GetDuration(INVOC_DURATION) > 120000) ? 120000 : aur->GetDuration(INVOC_DURATION),
					                                                                                    INVOC_DURATION,((Unit *)m_caster), ((Unit *)m_caster));
				aura->SetTimeLeft(aur->GetTimeLeft(INVOC_DURATION),INVOC_DURATION);
				for (uint8 i=0; i<aur->m_modcount; ++i)
					aura->m_modList[i] = aur->m_modList[i];
				aura->m_modcount = aur->m_modcount;
				((Unit *)m_caster)->AddAura(aura, m_spellScript);
				unitTarget->RemoveAura(aur);
				if( --spells_to_steal <= 0 )
					break; //exit loop now
			}			
		}
	}   
}

void Spell::SpellEffectProspecting(uint32 i)
{
	if(!m_caster->IsPlayer()) return;

	if(!itemTarget) // this should never happen
	{
		SendCastResult(SPELL_FAILED_CANT_BE_PROSPECTED);
		return;
	}

	//Fill Prospecting loot
	((Player *)m_caster)->SetLootGUID(itemTarget->GetGUID());
	if( !itemTarget->loot )
		{
			itemTarget->loot = new Loot;
			lootmgr.FillProspectingLoot( itemTarget->loot , itemTarget->GetEntry());
		}

	if ( itemTarget->loot->items.size() > 0 )
	{
		Log.Debug("SpellEffect","Succesfully prospected item %d", uint32(itemTarget->GetEntry()));
		((Player *)m_caster)->SendLoot( itemTarget->GetGUID(), LOOT_PROSPECTING );
	} 
	else // this should never happen either
	{
		Log.Debug("SpellEffect","Prospecting failed, item %d has no loot", uint32(itemTarget->GetEntry()));
		SendCastResult(SPELL_FAILED_CANT_BE_PROSPECTED);
	}
}

void Spell::SpellEffectResurrectNew(uint32 i)
{
	//base p =hp,misc mana
	if(!playerTarget)
	{
		if(!corpseTarget)
		{
			// unit resurrection handler
			if(unitTarget)
			{
				if(unitTarget->GetTypeId()==TYPEID_UNIT && unitTarget->IsPet() && unitTarget->isDead())
				{
					uint32 hlth = ((uint32)m_spellInfo->EffectBasePoints[i] > unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH)) ? unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH) : (uint32)m_spellInfo->EffectBasePoints[i];
					uint32 mana = ((uint32)m_spellInfo->EffectBasePoints[i] > unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1)) ? unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1) : (uint32)m_spellInfo->EffectBasePoints[i];

					if(!unitTarget->IsPet())
					{
						sEventMgr.RemoveEvents(unitTarget, EVENT_CREATURE_REMOVE_CORPSE);
					}
					else
					{
						sEventMgr.RemoveEvents(unitTarget, EVENT_PET_DELAYED_REMOVE);
						sEventMgr.RemoveEvents(unitTarget, EVENT_CREATURE_REMOVE_CORPSE);
					}
					unitTarget->SetUInt32Value(UNIT_FIELD_HEALTH, hlth);
					unitTarget->SetUInt32Value(UNIT_FIELD_POWER1, mana);
					unitTarget->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
					unitTarget->setDeathState(ALIVE);
					((Creature*)unitTarget)->Tagged=false;
					((Creature*)unitTarget)->TaggerGuid=false;
					((Creature*)unitTarget)->loot.gold=0;
					//((Creature*)unitTarget)->loot.looters.clear();
					((Creature*)unitTarget)->ClearLoot();
					((Creature*)unitTarget)->loot.items.clear();
				}
			}

			return;
		}
		playerTarget = objmgr.GetPlayer(corpseTarget->GetUInt32Value(CORPSE_FIELD_OWNER));
		if(!playerTarget) return;
	}

	if(playerTarget->isAlive() || !playerTarget->IsInWorld())
		return;
   //resurr
	playerTarget->resurrector = ((Player *)m_caster)->GetLowGUID();
	playerTarget->m_resurrectHealth = damage;
	playerTarget->m_resurrectMana = m_spellInfo->EffectMiscValue[i];
    playerTarget->m_resurrectLocation = ((Player *)m_caster)->GetPosition(); // Alleluia!!!!
	SendResurrectRequest(playerTarget);
}

void Spell::SpellEffectTranformItem(uint32 i)
{
	bool result;
	AddItemResult result2;

	if(!m_caster->IsItem()) return;
	uint32 itemid = m_spellInfo->EffectItemType[i];
    //Log.Notice("[Spell::SpellEffectTranformItem]","m_spellInfo %u",m_spellInfo->Id); 
	if(itemid == 0)
	{
		return;
	}


	//Save durability of the old item
	Player * owner=((Item *)m_caster)->GetOwner();
	uint32 dur= ((Item *)m_caster)->GetUInt32Value(ITEM_FIELD_DURABILITY);
	//	int8 slot=owner->GetItemInterface()->GetInventorySlotByGuid(i_caster->GetGUID());
	//	uint32 invt=i_caster->GetProto()->InventoryType;

	   result  = owner->GetItemInterface()->SafeFullRemoveItemByGuid(((Item *)m_caster)->GetGUID());
	if(!result)
	{
		//something went wrong if this happen, item doesnt exist, so it wasnt destroyed.
		Log.Error("[Spell::SpellEffectTranformItem]","FATAL: l'item n'existe pas!)");
		return;
	}

	m_caster=NULL; // Supprime caster item 
    
	Item *it=objmgr.CreateItem(itemid,owner);
	it->SetDurability(dur);
	
   //additem
	result2 = owner->GetItemInterface()->AddItemToFreeSlot(it);
	if(!result2) //should never get here
	{ 
		owner->GetItemInterface()->BuildInventoryChangeError(NULL,NULL,INV_ERR_BAG_FULL);
		//delete it;
	}
}

void Spell::SpellEffectEnvironmentalDamage(uint32 i)
{
	if(!playerTarget)
		return;

	if(playerTarget->SchoolImmunityList[m_spellInfo->School])
	{
		SendCastResult(SPELL_FAILED_IMMUNE);
		return;
	}
	//this is GO, not unit	
	m_caster->SpellNonMeleeDamageLog(playerTarget, m_spellInfo->Id, damage, pSpellId==0);
  
	WorldPacket data(SMSG_ENVIRONMENTALDAMAGELOG, 13);
	data << playerTarget->GetGUID();
	data << uint8(DAMAGE_FIRE);
	data << uint32(damage);
	playerTarget->SendMessageToSet( &data, true );
}

void Spell::SpellEffectDismissPet(uint32 i)
{
	// remove pet.. but don't delete so it can be called later
	if(!m_caster->IsPlayer()) return;

	Pet *pPet = ((Player *)m_caster)->GetSummon();
	if(!pPet) return;
	pPet->Remove(true, true, true);
}

void Spell::SpellEffectEnchantHeldItem( uint32 i )
{
	if( playerTarget == NULL )
		return;

	Item * item = playerTarget->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
	if( item == NULL )
		return;

	uint32 Duration = 1800; // Needs to be found in dbc.. I guess?
	switch( m_spellInfo->NameHash )
	{
		case SPELL_HASH_FLAMETONGUE_TOTEM:
		case SPELL_HASH_WINDFURY_TOTEM: // Windfury Totem Effect
		{   
			Duration = 10;
		}
	}
	EnchantEntry * Enchantment = dbcEnchant.LookupEntry( m_spellInfo->EffectMiscValue[i] );
	
	if( Enchantment == NULL )
		return;

	if (!item->HasEnchantmentInSlot(1) || item->GetEnchantment(1)->Enchantment->Id == Enchantment->Id)
		item->AddEnchantment( Enchantment, Duration, false, true, false, 1 );
}

void Spell::SpellEffectAddHonor(uint32 i)
{
	if( playerTarget == NULL )
		return;

	HonorHandler::AddHonorPointsToPlayer( playerTarget, m_spellInfo->EffectImplicitTargetA[i] );
}

void Spell::SpellEffectSpawn(uint32 i)
{
	// this effect is mostly for custom teleporting
	//switch(m_spellInfo->Id) 
	switch(m_spellInfo->NameHash)
	{             
	  case SPELL_HASH_ARUGAL_SPAWN_IN_SPELL: // Test (Branruz)
	  //case Arugal_spawn_in_spell: // 10418 - Arugal spawn-in spell , teleports it to 3 locations randomly sneeking players (bastard ;P)   
	  { 
		if(!m_caster->IsUnit()) return;
		if(m_caster->IsPlayer()) return;
		 
		static float coord[3][3]= {{-108.9034f,2129.5678f,144.9210f},{-108.9034f,2155.5678f,155.678f},{-77.9034f,2155.5678f,155.678f}};
		
		int i = (int)(rand()%3);
		((Unit *)m_caster)->GetAIInterface()->SendMoveToPacket(coord[i][0],coord[i][1],coord[i][2],0.0f,0,((Unit *)m_caster)->GetAIInterface()->getMoveFlags());
	  }
	}
}

void Spell::SpellEffectApplyAura128(uint32 i)
{
	// Test (Branruz)
	if( m_caster )
	{
		//if(((Unit *)m_caster)->GetTypeId() == TYPEID_UNIT) 
        if( m_caster->IsUnit() )
	    {
		 SpellEffectApplyAA(i);
	    }
	    else //-------------------
	    {
	     if(m_spellInfo->EffectApplyAuraName[i] != SPELL_AURA_NONE)
		                          SpellEffectApplyAura(i);
	   }
	}
	else Log.Error("[Spell::SpellEffectApplyAura128]","m_caster = NULL (Spell %u Effect index %u)",m_spellInfo->Id,i);
}

void Spell::SpellEffectRedirectThreat(uint32 i)
{
	if(!((Player *)m_caster) || !playerTarget)
		return;
	if(GetSpellProto()->NameHash == SPELL_HASH_TRICKS_OF_THE_TRADE)
		((Player *)m_caster)->CastSpell(playerTarget,57933,true);

	((Player *)m_caster)->SetRedirectThreat(playerTarget,GetSpellProto()->EffectBasePoints[i]+1,GetDuration(INVOC_DURATION));
}

void Spell::SpellEffectTriggerSpellWithValue(uint32 i)
{
	if( unitTarget == NULL )
		return;

	SpellEntry* TriggeredSpell = dbcSpell.LookupEntryForced(m_spellInfo->EffectTriggerSpell[i]);
	if( TriggeredSpell == NULL )
		return;

	Spell* sp=new Spell(m_caster,dbcSpell.LookupEntry(TriggeredSpell->Id),true,NULL);

	if(sp == NULL) return;

	for(uint32 x=0;x<3;x++)
	{
		sp->forced_basepoints[x] = TriggeredSpell->EffectBasePoints[i];
	}

	sp->triggered_id=m_spellInfo->Id;
	sp->forced_modifier = forced_modifier;
	sp->forced_applications = forced_applications;

	SpellCastTargets tgt(unitTarget);
	sp->prepare(&tgt);
}

void Spell::SpellEffectApplyDemonAura( uint32 i )
{
	if (((Unit *)m_caster) == NULL || ((Unit *)m_caster)->IsPet() || static_cast<Pet *>(m_caster)->GetPetOwner() == NULL)
		return;
	Aura* pAura = NULL;

	pAura = new Aura(GetSpellProto(), GetDuration(INVOC_DURATION), INVOC_DURATION,((Unit *)m_caster), ((Unit *)m_caster));
	Aura* otheraura = new Aura(GetSpellProto(), GetDuration(INVOC_DURATION), INVOC_DURATION,((Unit *)m_caster), static_cast<Pet *>(m_caster)->GetPetOwner());
	pAura->targets.insert(static_cast<Pet *>(m_caster)->GetPetOwner()->GetUIdFromGUID());
	for (uint32 j=0; j<3; ++j)
	{
		pAura->AddMod(GetSpellProto()->EffectApplyAuraName[j], j == i? damage : CalculateEffect(j, unitTarget), GetSpellProto()->EffectMiscValue[j], j);
		otheraura->AddMod(GetSpellProto()->EffectApplyAuraName[j], j == i? damage : CalculateEffect(j, unitTarget), GetSpellProto()->EffectMiscValue[j], j);
	}

	((Unit *)m_caster)->AddAura(pAura, m_spellScript);
	static_cast<Pet *>(m_caster)->GetPetOwner()->AddAura(otheraura, NULL);
}

void Spell::SpellEffectPlayMusic(uint32 i)
{
	if (unitTarget == NULL)
		return;

	unitTarget->PlaySoundToSet(m_spellInfo->EffectMiscValue[i]);
}

void Spell::SpellEffectForgetSpecialization(uint32 i) // Vient de Arcemu adapté par Crash
{
	if (!playerTarget) return;

	uint32 spellid = m_spellInfo->EffectTriggerSpell[i];
	playerTarget->removeSpell( spellid, false, false, 0);

	sLog.outDebug("Le joueur %u a oublié le spell %u du spell %u ( caster: %u)", playerTarget->GetLowGUID(), spellid, m_spellInfo->Id, m_caster->GetLowGUID());
}

void Spell::SpellEffectClearFinishedQuest(uint32 i)
{
	if (playerTarget == NULL)
		return;

	playerTarget->m_finishedQuests.erase(m_spellInfo->EffectMiscValue[i]);
	playerTarget->m_finisheddailyQuests.erase(m_spellInfo->EffectMiscValue[i]);
}

void Spell::SpellEffectPull( uint32 i )
{
	if( unitTarget == NULL && ((Unit *)m_caster) != NULL)
		unitTarget = ((Unit *)m_caster);

	if(unitTarget == NULL)
		return;

	if (!(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION))
		return;

	//check for mechanic immunity (i.e death grip uses clumsy mechanic), stunned, or rooted
	if (unitTarget->MechanicsDispels[m_spellInfo->mechanics] != 0 || unitTarget->IsStunned() || unitTarget->IsRooted())
		return;

	float pullX = 0.0f;
	float pullY = 0.0f;
	float pullZ = 0.0f;
	if(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
	{
		pullX = m_targets.m_destX;
		pullY = m_targets.m_destY;
		pullZ = m_targets.m_destZ;
	}
	else
	{
		pullX = m_caster->GetPositionX();
		pullY = m_caster->GetPositionY();
		pullZ = m_caster->GetPositionZ();
	}

	float arc = 0.0f;
	if(GetSpellProto()->EffectMiscValue[i])
		arc = GetSpellProto()->EffectMiscValue[i]/10;
	else if(GetSpellProto()->EffectMiscValueB[i])
		arc = GetSpellProto()->EffectMiscValueB[i]/10;
	else
		arc = 10.0f;

	uint32 time = uint32((CalculateEffect(i, unitTarget) / arc) * 100);
	unitTarget->GetAIInterface()->StopMovement(time);
	unitTarget->SetPosition(pullX, pullY, pullZ, 0.0f);

	WorldPacket data(SMSG_MONSTER_MOVE, 500);

	data << unitTarget->GetNewGUID();
	data << uint8(0);
	data << unitTarget->GetPositionX() << unitTarget->GetPositionY() << unitTarget->GetPositionZ();
	data << getMSTime();
	data << uint8(0); //flags
	data << uint32(0x108); //moveflags
	data << uint32(500); //movetime, todo: calculate
	data << float(50.0f); //arc height?
	data << uint32(0); //always logged as 0
	data << uint32(1); //point count
	data << m_targets.m_destX << m_targets.m_destY << m_targets.m_destZ;
	unitTarget->SendMessageToSet(&data, true);

	if(playerTarget)
	{
		if( playerTarget->IsPvPFlagged() )
			((Unit *)m_caster)->SetPvPFlag();
		if( playerTarget->IsFFAPvPFlagged() )
			((Unit *)m_caster)->SetFFAPvPFlag();
	}
}

void Spell::SpellEffectApplyGlyph(uint32 i)
{
	if(!m_caster->IsPlayer()) return;

	uint8 glyphslot = m_targets.m_targetMaskExtended >> 8 & 0x0F;

	//remove old glyphs if they exist
	if (((Player *)m_caster)->GetUInt32Value(PLAYER_FIELD_GLYPHS_1 + glyphslot))
	{
		GlyphPropertyEntry* oldglyph = dbcGlyphProperty.LookupEntry(((Player *)m_caster)->GetUInt32Value(PLAYER_FIELD_GLYPHS_1 + glyphslot));
		if (oldglyph != NULL)
			((Player *)m_caster)->RemoveAura(oldglyph->SpellId);
	}
	
	GlyphPropertyEntry* glyph = dbcGlyphProperty.LookupEntry(m_spellInfo->EffectMiscValue[i]);
	if (glyph != NULL)
	{
		Spell *sp = new Spell(((Player *)m_caster), dbcSpell.LookupEntry(glyph->SpellId), true, NULL);
		SpellCastTargets t(((Player *)m_caster));
		sp->prepare(&t);
		((Player *)m_caster)->SetUInt32Value(PLAYER_FIELD_GLYPHS_1 + glyphslot, glyph->Entry);
	}
}

void Spell::SpellEffectJumpMove(uint32 i)
{
	if (unitTarget == NULL)
	{
		if(!m_caster->IsUnit()) return;
		unitTarget = ((Unit *)m_caster);
	}

	if (!(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION))
		return;

	//check for mechanic immunity (i.e death grip uses clumsy mechanic), stunned, or rooted
	if (unitTarget->MechanicsDispels[m_spellInfo->mechanics] != 0 || unitTarget->IsStunned() || unitTarget->IsRooted())
		return;

	WorldPacket data(SMSG_MONSTER_MOVE, 500);

	data << unitTarget->GetNewGUID();
	data << uint8(0);
	data << unitTarget->GetPositionX() << unitTarget->GetPositionY() << unitTarget->GetPositionZ();
	data << getMSTime();
	data << uint8(0); //flags
	data << uint32(0x108); //moveflags
	data << uint32(500); //movetime, todo: calculate
	data << float(50.0f); //arc height?
	data << uint32(0); //always logged as 0
	data << uint32(1); //point count
	data << m_targets.m_destX << m_targets.m_destY << m_targets.m_destZ;
	unitTarget->SendMessageToSet(&data, true);

	float newo = unitTarget->calcRadAngle(unitTarget->GetPositionX(),unitTarget->GetPositionY(), m_targets.m_destX, m_targets.m_destY);
	unitTarget->SetPosition(m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, newo);
	unitTarget->GetAIInterface()->StopMovement(500);

	if (unitTarget->IsPlayer())
		static_cast<Player *>(unitTarget)->DelaySpeedHack(2000);
}
// Hearthstone
void Spell::SpellEffectCreateRandomItem(uint32 i) // Create Random Item
{
	
	if(!((Player *)m_caster))
		return;

	Item* newItem = new(Item);
	newItem = NULL;
	Item* add = NULL;
	uint8 slot;
	uint32 itemid;
	SlotResult slotresult;

	skilllinespell* skill = objmgr.GetSpellSkill(m_spellInfo->Id);

	ItemPrototype *m_itemProto;
	itemid	=	m_spellInfo->EffectItemType[i];
	m_itemProto = ItemPrototypeStorage.LookupEntry( m_spellInfo->EffectItemType[i] );
	if (!m_itemProto)
		 return;

	if(itemid == 0)
		return;

	uint32 item_count = 0;
	// Random Item to Create Jewelcrafting part
	RandomItemCreation * ric = RandomItemCreationStorage.LookupEntry( m_spellInfo->Id );
	if(ric)
	{
		// If we Have Perfect Gem Cutting then we have a chance to create a Perfect Gem, according to comments on wowhead chance is between 20 and 30%
		if (Rand(ric->Chance) && ric->Skill == SKILL_JEWELCRAFTING && ((Player *)m_caster)->HasSpell(55534))
		{
			m_itemProto = ItemPrototypeStorage.LookupEntry( ric->ItemToCreate );
			itemid	=	ric->ItemToCreate;
		}

		//Tarot and Decks from Inscription + Northrend Inscription Research + Minor Inscription Research
		//Northrend Alchemy
		if (ric->Skill == SKILL_INSCRIPTION || ric->Skill == SKILL_ALCHEMY)
		{
			uint32 k;
			RandomCardCreation * rcc = RandomCardCreationStorage.LookupEntry(m_spellInfo->Id);
			if(rcc)
			{
				//Same chance for every card to appear according wowhead and wowwiki info
				k = RandomUInt(rcc->itemcount-1);
				m_itemProto = ItemPrototypeStorage.LookupEntry( rcc->ItemId[k] );
				itemid	=	rcc->ItemId[k];
				item_count = 1;
				switch(m_spellInfo->Id)
				{
					case 61288:
					case 61177:
						{
							item_count = RandomUInt(2);//This 2 can make random scrolls and vellum 1 or 2 according to info
						}break;
					case 60893:
						{
							item_count = RandomUInt(3);//Creates 3 random elixir/potion from alchemy
						}break;
				}
			}
		}
	}
	// Profession Discoveries used in Northrend Alchemy and Inscription Research plus Minor research
	uint32 discovered_recipe = 0;
	std::set<ProfessionDiscovery*>::iterator itr = objmgr.ProfessionDiscoveryTable.begin();
		for ( ; itr != objmgr.ProfessionDiscoveryTable.end(); itr++ )
		{
			ProfessionDiscovery * pf = ( *itr );
			if ( pf != NULL && m_spellInfo->Id == pf->SpellId && ((Player *)m_caster)->_GetSkillLineCurrent( skill->skilline ) >= pf->SkillValue && !((Player *)m_caster)->HasSpell( pf->SpellToDiscover ))
			{
				discovered_recipe = pf->SpellToDiscover;
				break;
			}
		}
		// if something discovered learn p_caster that recipe and broadcast message
		if ( discovered_recipe != 0 )
		{
			SpellEntry * se = dbcSpell.LookupEntry( discovered_recipe );
			if ( se != NULL )
			{
				((Player *)m_caster)->addSpell( discovered_recipe );
				WorldPacket * data;
				char msg[256];
				sprintf( msg, "%sDISCOVERY! %s has discovered how to create %s.|r", MSG_COLOR_GOLD, ((Player *)m_caster)->GetName(), se->Name );
				data = sChatHandler.FillMessageData( CHAT_MSG_SYSTEM, LANG_UNIVERSAL,  msg, ((Player *)m_caster)->GetGUID(), 0 );
				((Player *)m_caster)->GetMapMgr()->SendChatMessageToCellPlayers( ((Player *)m_caster), data, 2, 1, LANG_UNIVERSAL, ((Player *)m_caster)->GetSession() );
				delete data;
			}
		}
	if( m_itemProto == NULL )
		return;

	// item count cannot be more than allowed in a single stack
	if (item_count > m_itemProto->MaxCount)
		item_count = m_itemProto->MaxCount;

	// item count cannot be more than item unique value
	if (m_itemProto->Unique && item_count > m_itemProto->Unique)
		item_count = m_itemProto->Unique;

	if(((Player *)m_caster)->GetItemInterface()->CanReceiveItem(m_itemProto, item_count))
		{
			SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
			return;
		}
	
	slot = 0;
	add = ((Player *)m_caster)->GetItemInterface()->FindItemLessMax(itemid,1, false);
	if (!add)
		{
			slotresult = ((Player *)m_caster)->GetItemInterface()->FindFreeInventorySlot(m_itemProto);
			if(!slotresult.Result)
			{
				  SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
				  return;
			}
			
			newItem =objmgr.CreateItem(itemid,((Player *)m_caster));
			newItem->SetUInt64Value(ITEM_FIELD_CREATOR,m_caster->GetGUID());
			newItem->SetUInt32Value(ITEM_FIELD_STACK_COUNT, item_count);


			if(((Player *)m_caster)->GetItemInterface()->SafeAddItem(newItem,slotresult.ContainerSlot, slotresult.Slot))
			{
				((Player *)m_caster)->GetSession()->SendItemPushResult(newItem,true,false,true,true,slotresult.ContainerSlot,slotresult.Slot,item_count);
			} else {
				delete newItem;
				newItem = NULL;
			}
			if(skill)
				DetermineSkillUp(skill->skilline);
		} 
		else 
		{
			//scale item_count down if total stack will be more than 20
			if(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + item_count > 20)
			{
				uint32 item_count_filled;
				item_count_filled = 20 - add->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
				add->SetCount(20);
				add->m_isDirty = true;

				slotresult = ((Player *)m_caster)->GetItemInterface()->FindFreeInventorySlot(m_itemProto);
				if(!slotresult.Result)
					item_count = item_count_filled;
				else
				{
					newItem =objmgr.CreateItem(itemid,((Player *)m_caster));
					newItem->SetUInt64Value(ITEM_FIELD_CREATOR,m_caster->GetGUID());
					newItem->SetUInt32Value(ITEM_FIELD_STACK_COUNT, item_count - item_count_filled);
					if(!((Player *)m_caster)->GetItemInterface()->SafeAddItem(newItem,slotresult.ContainerSlot, slotresult.Slot))
					{
						delete newItem;
						newItem = NULL;
						item_count = item_count_filled;
					}
					else
						((Player *)m_caster)->GetSession()->SendItemPushResult(newItem, true, false, true, true, slotresult.ContainerSlot, slotresult.Slot, item_count-item_count_filled);
                }
			}
			else
			{
				add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + item_count);
				add->m_isDirty = true;
				((Player *)m_caster)->GetSession()->SendItemPushResult(add, true,false,true,false,((Player *)m_caster)->GetItemInterface()->GetBagSlotByGuid(add->GetGUID()),0xFFFFFFFF,item_count);
			}
			if(skill)
				DetermineSkillUp(skill->skilline);
		}		
}

void Spell::SpellEffectKillCredit( uint32 i )
{
	CreatureProto * cp = CreatureProtoStorage.LookupEntry( m_spellInfo->EffectMiscValue[i] );
	if ( playerTarget != NULL && cp != NULL )
		sQuestMgr._OnPlayerKill( playerTarget, m_spellInfo->EffectMiscValue[i]);
}

void Spell::SpellEffectMilling(uint32 i)
{
	if(((Player *)m_caster) == NULL)
		return;

	if(itemTarget == NULL)
	{
		SendCastResult(SPELL_FAILED_ITEM_GONE);
		return;
	}

	uint32 entry = itemTarget->GetEntry();

	if(((Player *)m_caster)->GetItemInterface()->RemoveItemAmt(entry, 5))
	{
		((Player *)m_caster)->SetLootGUID(((Player *)m_caster)->GetGUID());
		lootmgr.FillItemLoot(&((Player *)m_caster)->m_loot, entry );
		((Player *)m_caster)->SendLoot(((Player *)m_caster)->GetGUID(), LOOT_MILLING);
	}
	else
		SendCastResult(SPELL_FAILED_NEED_MORE_ITEMS);
}

void Spell::SpellEffectLearnSpec(uint32 i)
{
	if(!m_caster->IsPlayer()) return;

	((Player *)m_caster)->m_talentSpecsCount = 2;
	((Player *)m_caster)->smsg_TalentsInfo(false);
}

void Spell::SpellEffectRestoreHealthPct(uint32 i)
{
	if(!m_caster->IsUnit() || !unitTarget || !unitTarget->isAlive())
		return;

	uint32 maxHp = (uint32)unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	uint32 modHp = damage * maxHp / 100;

	((Unit *)m_caster)->Heal(unitTarget, m_spellInfo->Id, modHp);
}

void Spell::SpellEffectRestoreManaPct(uint32 i)
{
	if(!m_caster->IsUnit() || !unitTarget || !unitTarget->isAlive())
		return;

	uint32 maxMana = (uint32)unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1);
	uint32 modMana = damage * maxMana / 100;	

	((Unit *)m_caster)->Energize(unitTarget, pSpellId ? pSpellId : m_spellInfo->Id, modMana, POWER_TYPE_MANA);
}

void Spell::SpellEffectDisengage(uint32 i)
{
	if(unitTarget == NULL || !unitTarget->isAlive())
		return;

	unitTarget->knockback(GetSpellProto()->EffectBasePoints[i]+1, GetSpellProto()->EffectMiscValue[i], true);
}

void Spell::SpellEffectTitanGrip(uint32 i)
{
	
	if (!playerTarget)
		return;
	playerTarget->titanGrip = true;
	
}
// MegaBigBoss, SpellEffect 43 - 
void Spell::SpellEffectTeleportUnitsFaceCaster(uint32 i)
{
	uint32 spellId = m_spellInfo->Id;

	Log.Notice("[Spell::SpellEffectTeleportUnitsFaceCaster]","SpellEffect 43! (SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER)");

	if( unitTarget == NULL ) return;
	else Log.Error("[Spell::SpellEffectTeleportUnitsFaceCaster]","unitTarget == NULL");

	// Try a dummy SpellHandler
	if( sScriptMgr.CallScriptedDummySpell( spellId, i, this ) )
		return;

	float ang;

	if( unitTarget->GetTypeId() != TYPEID_UNIT ) 
	{
		Log.Warning("[Spell::SpellEffectTeleportUnitsFaceCaster]","ECHEC: Cible != TypeUnit");
		return;
	}

	if(m_caster->IsUnit())
	{
	 ang = ((Unit *)m_caster)->GetOrientation() + 3.141592f ; 

	 // avoid teleporting into the model on scaled models
	 const static float teleport_distance = 1.6f * unitTarget->GetFloatValue(OBJECT_FIELD_SCALE_X);
	 float new_x = ((Unit *)m_caster)->GetPositionX() + (teleport_distance * cosf(ang));
	 float new_y = ((Unit *)m_caster)->GetPositionY() + (teleport_distance * sinf(ang));
		
	 /* Send a movement packet to "charge" at this target. Similar to warrior charge. */
	 //unitTarget->z_axisposition = 0.0f; // Test du u_caster pour la position z
	 Player *plr = unitTarget->GetPlayerFrom();
	 if(plr) plr->SafeTeleport(((Unit *)m_caster)->GetMapId(), ((Unit *)m_caster)->GetInstanceID(), 
		                 LocationVector(new_x, new_y, (((Unit *)m_caster)->GetPositionZ() + 0.1f), ang));
	 else Log.Error("[Spell::SpellEffectTeleportUnitsFaceCaster]","ECHEC: Player non trouve!");
	 
	}
	else Log.Error("[Spell::SpellEffectTeleportUnitsFaceCaster]","u_caster == NULL");

	return;
}
// Go destructible
void Spell::SpellEffectWMODamage(uint32 i)
{
	if(gameObjTarget->GetInfo()->Type == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING)
		gameObjTarget->TakeDamage((uint32)damage);
}
void Spell::SpellEffectWMORepair(uint32 i)
{
	if(gameObjTarget->GetInfo()->Type == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING)
		gameObjTarget->Rebuild();
}

void Spell::SpellEffectChangeWMOState(uint32 i)
{
	if(gameObjTarget == NULL)
		return;

	switch(GetSpellProto()->EffectMiscValue[i])
	{
	case 1:
		{
			gameObjTarget->Damage();
		}break;
	case 2:
		{
			gameObjTarget->Destroy();
		}break;
	default:
		{
			gameObjTarget->Rebuild();
		}break;
	}
}

void Spell::SpellEffectActivateRune(uint32 i)
{
	if( !((Player *)m_caster) )
		return;

	if( ((Player *)m_caster)->getClass() != DEATHKNIGHT )
		return;
    uint32 count = damage;
    if (count == 0) 
		count = 1;

	for( uint8 j = 0; j < 6 && count; ++j )
	{
		if( ((Player *)m_caster)->GetRune(j) == RUNE_TYPE_RECHARGING && ((Player *)m_caster)->GetRune(j) == GetSpellProto()->EffectMiscValue[i] )
		{
			((Player *)m_caster)->SetRune(j, ((Player *)m_caster)->GetBaseRune(j));
			((Player *)m_caster)->ClearRuneCooldown(j);
		}
	}
}

void Spell::SpellEffectFailQuest(uint32 i)
{
	Quest *qst = NULL;
	if( !((Player *)m_caster) )
		return;

	qst =  QuestStorage.LookupEntry(GetSpellProto()->EffectMiscValue[i]);

	if(!qst)
		return;

	sQuestMgr.SendQuestFailed(FAILED_REASON_FAILED, qst, ((Player *)m_caster) );
}

void Spell::SpellEffectStartQuest(uint32 i)
{
	if( !playerTarget )
		return;

	playerTarget->HasQuest(GetSpellProto()->EffectMiscValue[i]);
}

// Spells: 46716-46730
void Spell::SpellEffectCreatePet(uint32 i)
{
	CreatureInfo* ci = NULL;
	CreatureProto* proto = NULL; 

	if( !(playerTarget || playerTarget->IsInWorld()) )
		return;

	if( playerTarget->getClass() != HUNTER )
		return;

	if( playerTarget->GetSummon() )
		playerTarget->GetSummon()->Remove( true, true, true );

	ci = CreatureNameStorage.LookupEntry( GetSpellProto()->EffectMiscValue[i] );
	proto = CreatureProtoStorage.LookupEntry(GetSpellProto()->EffectMiscValue[i]);
	if( ci != NULL && proto != NULL )
	{
		Pet *pPet = objmgr.CreatePet();
		if(pPet != NULL)
		{
			pPet->CreateAsSummon(m_spellInfo->EffectMiscValue[i], ci, NULL, 
				playerTarget, m_spellInfo, 1, GetDuration(INVOC_DURATION));
		}
	}
}

void Spell::SpellEffectAddPrismaticSocket(uint32 i)
{
	if( ((Player *)m_caster) == NULL)
		return;

	if(!itemTarget)
		return;

	EnchantEntry* pEnchant = dbcEnchant.LookupEntry(GetSpellProto()->EffectMiscValue[i]);
	if(!pEnchant)
		return;

	bool add_socket = false;
	for(uint8 i = 0; i < 3; i++)
	{
		if(pEnchant->type[i] == 8)
		{
			add_socket = true;
			break;
		}
	}

	if(!add_socket) // Wrong spell.
		return;

	// Item can be in trade slot and have owner diff. from caster
	Player* item_owner = itemTarget->GetOwner();
	if(!item_owner)
		return;

	if(itemTarget->GetSocketsCount() >= 3)
	{
		SendCastResult(SPELL_FAILED_MAX_SOCKETS);
		return;
	}

	itemTarget->RemoveProfessionEnchant();
	itemTarget->AddEnchantment(pEnchant, 0, true, true, false, 6); // 6 is profession slot.
}

void Spell::SpellEffectAllowPetRename( uint32 i )
{
	if( !unitTarget || !unitTarget->IsPet() )
		return;

	unitTarget->SetByte( UNIT_FIELD_BYTES_2, 2, 0x03);
}

void Spell::SpellEffectActivateTalentSpec(uint32 i)
{
	if( ((Player *)m_caster) == NULL)
		return;

	if(((Player *)m_caster)->m_bg)
	{
		SendCastResult(SPELL_FAILED_NOT_IN_BATTLEGROUND);
		return;
	}

	// 1 = primary, 2 = secondary
	((Player *)m_caster)->ApplySpec(uint8(damage - 1), false);

	// Use up all our power.
	switch(((Player *)m_caster)->GetPowerType())
	{
	case POWER_TYPE_MANA:
		((Player *)m_caster)->SetPower(POWER_TYPE_MANA, 0);
		break;
	case POWER_TYPE_RAGE:
		((Player *)m_caster)->SetPower(POWER_TYPE_RAGE, 0);
		break;
	case POWER_TYPE_ENERGY:
		((Player *)m_caster)->SetPower(POWER_TYPE_ENERGY, 0);
		break;
	case POWER_TYPE_RUNES:
		((Player *)m_caster)->SetPower(POWER_TYPE_RUNES, 0);
		break;
	}
}

void Spell::SpellEffectRemoveAura(uint32 i)
{
	if (!unitTarget)
		return;

	unitTarget->RemoveAura(GetSpellProto()->EffectTriggerSpell[i], unitTarget->GetGUID());
}