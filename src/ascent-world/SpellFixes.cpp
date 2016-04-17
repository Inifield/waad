/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
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

/// externals for spell system
extern pSpellEffect SpellEffectsHandler[TOTAL_SPELL_EFFECTS];
extern pSpellAura SpellAuraHandler[TOTAL_SPELL_AURAS];

void CreateDummySpell(uint32 id)
{
	const char * name = "Dummy Trigger";
	SpellEntry * sp = new SpellEntry;
	memset(sp, 0, sizeof(SpellEntry));
	sp->Id = id;
	sp->attributes          = 384;       // 0x00000180  ATTRIBUTES_NO_VISUAL_AURA | ATTRIBUTES_UNK10
	sp->attributesEx        = 268435456; // 0x10000000  ATTRIBUTESEX_UNK30
	sp->attributesExB       = 4;         // 0x00000004  ATTRIBUTESEXB_UNK4 // request pet maybe
	sp->castingTimeIndex=1;
	sp->procChance=75;
	sp->rangeIndex=13;
	sp->EquippedItemClass = uint32(-1);
	sp->Effect[0]         = SPELL_EFFECT_DUMMY; // SPELL_EFFECT_DUMMY
	sp->EffectImplicitTargetA[0] = EFF_TARGET_TO_SELECT; // (Branruz)
	sp->NameHash=crc32((const unsigned char*)name, (unsigned int)strlen(name));
	sp->effectChainAmplitude[0]=1.0f;
	sp->stanceBarOrder=-1;
	dbcSpell.SetRow(id,sp);
	sWorld.dummyspells.push_back(sp);
}

uint32 GetSpellClass(SpellEntry *sp)
{
	switch(sp->skilline)
	{
	case SKILL_ARMS:
	case SKILL_FURY:
	case SKILL_PROTECTION:
		return WARRIOR;
	case SKILL_HOLY2:
	case SKILL_PROTECTION2:
	case SKILL_RETRIBUTION:
		return PALADIN;
	case SKILL_BEAST_MASTERY:
	case SKILL_SURVIVAL:
	case SKILL_MARKSMANSHIP:
		return HUNTER;
	case SKILL_ASSASSINATION:
	case SKILL_COMBAT:
	case SKILL_SUBTLETY:
		return ROGUE;
	case SKILL_DISCIPLINE:
	case SKILL_HOLY:
	case SKILL_SHADOW:
		return PRIEST;
	case SKILL_ENHANCEMENT:
	case SKILL_RESTORATION:
	case SKILL_ELEMENTAL_COMBAT:
		return SHAMAN;
	case SKILL_FROST:
	case SKILL_FIRE:
	case SKILL_ARCANE:
		return MAGE;
	case SKILL_AFFLICTION:
	case SKILL_DEMONOLOGY:
	case SKILL_DESTRUCTION:
		return WARLOCK;
	case SKILL_RESTORATION2:
	case SKILL_BALANCE:
	case SKILL_FERAL_COMBAT:
		return DRUID;
	case SKILL_DK_FROST:
	case SKILL_UNHOLY:
	case SKILL_BLOOD:
		return DEATHKNIGHT;
	}

	return 0;
}

void ApplyExtraDataFixes()
{
	SpellEntry * sp;
	// Spell 53 (Backstab Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(53);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 72 (Shield Bash Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(72);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 75 (Auto Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(75);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 78 (Heroic Strike Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(78);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 116 (Frostbolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(116);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 118 (Polymorph Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(118);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 133 (Fireball Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(133);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 143 (Fireball Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(143);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 145 (Fireball Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(145);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 172 (Corruption Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(172);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 205 (Frostbolt Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(205);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 228 (Polymorph: Chicken Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(228);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 246 (Slow Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(246);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 284 (Heroic Strike Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(284);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 285 (Heroic Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(285);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 348 (Immolate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(348);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 370 (Purge Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(370);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 403 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(403);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 408 (Kidney Shot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(408);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 421 (Chain Lightning Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(421);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 529 (Lightning Bolt Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(529);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 548 (Lightning Bolt Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(548);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 585 (Smite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(585);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 589 (Shadow Word: Pain Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(589);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 591 (Smite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(591);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 594 (Shadow Word: Pain Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(594);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 598 (Smite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(598);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 603 (Curse of Doom Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(603);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 686 (Shadow Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(686);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 689 (Drain Life Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(689);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 694 (Mocking Blow Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(694);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 695 (Shadow Bolt Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(695);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 699 (Drain Life Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(699);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 701 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(701);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 702 (Curse of Weakness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(702);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 703 (Garrote Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(703);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 704 (Curse of Recklessness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(704);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 705 (Shadow Bolt Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(705);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 707 (Immolate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(707);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 709 (Drain Life Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(709);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 710 (Banish Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(710);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 770 (Faerie Fire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(770);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 772 (Rend Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(772);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 778 (Faerie Fire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(778);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 830 (Fearless ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(830);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 837 (Frostbolt Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(837);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 845 (Cleave Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(845);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 851 (Polymorph: Sheep Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(851);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 853 (Hammer of Justice Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(853);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 879 (Exorcism Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(879);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 915 (Lightning Bolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(915);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 921 (Pick Pocket ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(921);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 930 (Chain Lightning Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(930);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 943 (Lightning Bolt Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(943);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 970 (Shadow Word: Pain Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(970);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 980 (Curse of Agony Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(980);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 984 (Smite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(984);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 992 (Shadow Word: Pain Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(992);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1004 (Smite Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1004);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1010 (Curse of Idiocy Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1010);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1014 (Curse of Agony Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1014);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1079 (Rip Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1079);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1082 (Claw Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1082);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1088 (Shadow Bolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1088);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1094 (Immolate Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1094);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1098 (Enslave Demon Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1098);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1106 (Shadow Bolt Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1106);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1108 (Curse of Weakness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1108);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1120 (Drain Soul Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1120);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1130 (Hunter's Mark Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1130);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1329 (Mutilate Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(1329);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 1464 (Slam Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1464);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1490 (Curse of the Elements Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1490);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1495 (Mongoose Bite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1495);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1510 (Volley Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1510);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1608 (Heroic Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1608);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1671 (Shield Bash Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1671);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1672 (Shield Bash Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1672);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1714 (Curse of Tongues Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1714);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1715 (Hamstring Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1715);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1752 (Sinister Strike Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1752);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1757 (Sinister Strike Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1757);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1758 (Sinister Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1758);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1759 (Sinister Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1759);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1760 (Sinister Strike Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1760);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1766 (Kick Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1766);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1767 (Kick Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1767);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1768 (Kick Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1768);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1769 (Kick Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1769);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1776 (Gouge Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1776);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1777 (Gouge Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1777);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1804 (Pick Lock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1804);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1822 (Rake Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1822);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1823 (Rake Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1823);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1824 (Rake Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1824);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1940 (Rocket Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1940);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1943 (Rupture Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1943);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 1949 (Hellfire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(1949);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2070 (Sap Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2070);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2098 (Eviscerate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2098);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2136 (Fire Blast Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2136);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2137 (Fire Blast Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2137);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2138 (Fire Blast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2138);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2589 (Backstab Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(2589);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 2590 (Backstab Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(2590);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 2591 (Backstab Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(2591);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 2596 (Touch of the Black Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2596);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2626 (Lightning Bolt Proc V ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2626);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2637 (Hibernate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2637);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2643 (Multi-Shot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2643);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2652 (Touch of Weakness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2652);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2691 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2691);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2764 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2764);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2767 (Shadow Word: Pain Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2767);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2812 (Holy Wrath Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2812);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2860 (Chain Lightning Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2860);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2908 (Soothe Animal Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2908);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2912 (Starfire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2912);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2941 (Immolate Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2941);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2943 (Touch of Weakness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2943);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2944 (Devouring Plague Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2944);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2948 (Scorch Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2948);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2973 (Raptor Strike Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2973);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 2974 (Wing Clip Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(2974);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3009 (Claw Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3009);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3010 (Claw Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3010);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3018 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3018);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3029 (Claw Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3029);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3034 (Viper Sting Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3034);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3044 (Arcane Shot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3044);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3110 (Firebolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3110);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3130 (Ice Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3130);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3140 (Fireball Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3140);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3232 (Gouge Stun Test ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3232);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3242 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3242);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3252 (Shred ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3252);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3263 (Touch of Ravenclaw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3263);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3446 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3446);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3674 (Black Arrow Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3674);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3720 (Throw Syndicate Bomb ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3720);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 3823 (Throw Proximity Bomb ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(3823);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 4043 (Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(4043);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 4164 (Throw Rock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(4164);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 4165 (Throw Rock II ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(4165);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5010 (Scorch Breath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5010);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5019 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5019);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5138 (Drain Mana Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5138);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5143 (Arcane Missiles Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5143);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5144 (Arcane Missiles Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5144);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5145 (Arcane Missiles Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5145);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5171 (Slice and Dice Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5171);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5176 (Wrath Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5176);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5177 (Wrath Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5177);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5178 (Wrath Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5178);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5179 (Wrath Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5179);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5180 (Wrath Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5180);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5201 (Claw Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5201);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5211 (Bash Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5211);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5221 (Shred Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(5221);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 5256 (Claw Cover ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5256);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5401 (Lizard Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5401);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5422 (Lacerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5422);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5424 (Claw Grasp ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5424);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5480 (Cleave Armor Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5480);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5508 (Cleave Armor Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5508);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5532 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5532);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5570 (Insect Swarm Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5570);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5588 (Hammer of Justice Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5588);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5589 (Hammer of Justice Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5589);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5614 (Exorcism Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5614);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5615 (Exorcism Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5615);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5648 (Stunning Blast Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5648);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5649 (Stunning Blast Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5649);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5676 (Searing Pain Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5676);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5782 (Fear Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5782);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5938 (Shiv Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5938);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 5940 (Shiv Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(5940);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6041 (Lightning Bolt Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6041);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6060 (Smite Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6060);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6136 (Chilled Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6136);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6146 (Slow Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6146);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6205 (Curse of Weakness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6205);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6213 (Fear Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6213);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6215 (Fear Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6215);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6217 (Curse of Agony Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6217);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6222 (Corruption Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6222);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6223 (Corruption Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6223);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6226 (Drain Mana Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6226);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6254 (Chained Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6254);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6255 (Lizard Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6255);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6345 (Polymorphic Explosion ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6345);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6346 (Fear Ward ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6346);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6353 (Soul Fire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6353);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6434 (Slice and Dice Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6434);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6546 (Rend Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6546);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6547 (Rend Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6547);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6548 (Rend Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6548);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6552 (Pummel Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6552);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6554 (Pummel Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6554);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6572 (Revenge Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6572);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6574 (Revenge Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6574);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6660 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6660);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6760 (Eviscerate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6760);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6761 (Eviscerate Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6761);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6762 (Eviscerate Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6762);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6770 (Sap Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6770);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6774 (Slice and Dice Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6774);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6780 (Wrath Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6780);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6785 (Ravage Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(6785);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 6787 (Ravage Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(6787);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 6789 (Death Coil Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6789);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6798 (Bash Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6798);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6800 (Shred Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(6800);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 6949 (Weak Frostbolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6949);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6950 (Faerie Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6950);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 6965 (Polymorphic Ray ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(6965);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7101 (Flame Blast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7101);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7160 (Execute ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7160);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7269 (Arcane Missiles Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7269);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7270 (Arcane Missiles Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7270);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7288 (Immolate Cumulative (TEST) Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7288);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7321 (Chilled Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7321);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7322 (Frostbolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7322);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7369 (Cleave Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7369);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7372 (Hamstring Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7372);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7373 (Hamstring Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7373);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7379 (Revenge Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7379);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7384 (Overpower Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7384);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7386 (Sunder Armor Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7386);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7400 (Mocking Blow Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7400);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7402 (Mocking Blow Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7402);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7405 (Sunder Armor Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7405);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7588 (Void Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7588);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7641 (Shadow Bolt Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7641);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7646 (Curse of Weakness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7646);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7648 (Corruption Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7648);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7651 (Drain Life Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7651);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7656 (Hex of Ravenclaw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7656);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7657 (Hex of Ravenclaw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7657);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7658 (Curse of Recklessness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7658);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7659 (Curse of Recklessness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7659);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7799 (Firebolt Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7799);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7800 (Firebolt Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7800);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7801 (Firebolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7801);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7802 (Firebolt Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7802);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7887 (Overpower Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7887);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 7978 (Throw Dynamite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(7978);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8012 (Purge Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8012);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8042 (Earth Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8042);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8044 (Earth Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8044);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8045 (Earth Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8045);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8046 (Earth Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8046);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8050 (Flame Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8050);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8052 (Flame Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8052);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8053 (Flame Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8053);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8056 (Frost Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8056);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8058 (Frost Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8058);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8092 (Mind Blast Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8092);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8102 (Mind Blast Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8102);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8103 (Mind Blast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8103);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8104 (Mind Blast Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8104);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8105 (Mind Blast Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8105);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8106 (Mind Blast Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8106);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8129 (Mana Burn Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8129);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8131 (Mana Burn Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8131);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8242 (Shield Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8242);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8246 (Lightning Bolt Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8246);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8288 (Drain Soul Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8288);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8289 (Drain Soul Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8289);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8292 (Chain Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8292);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8380 (Sunder Armor Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8380);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8391 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8391);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8398 (Frostbolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8398);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8400 (Fireball Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8400);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8401 (Fireball Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8401);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8402 (Fireball Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8402);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8406 (Frostbolt Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8406);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8407 (Frostbolt Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8407);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8408 (Frostbolt Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8408);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8412 (Fire Blast Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8412);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8413 (Fire Blast Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8413);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8416 (Arcane Missiles Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8416);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8417 (Arcane Missiles Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8417);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8418 (Arcane Missiles Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8418);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8419 (Arcane Missiles Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8419);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8444 (Scorch Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8444);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8445 (Scorch Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8445);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8446 (Scorch Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8446);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8552 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8552);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8598 (Lightning Blast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8598);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8621 (Sinister Strike Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8621);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8623 (Eviscerate Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8623);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8624 (Eviscerate Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8624);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8627 (Backstab (TEST) Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8627);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8629 (Gouge Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8629);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8631 (Garrote Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8631);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8632 (Garrote Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8632);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8633 (Garrote Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8633);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8639 (Rupture Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8639);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8640 (Rupture Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8640);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8643 (Kidney Shot Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8643);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8647 (Expose Armor Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8647);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8649 (Expose Armor Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8649);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8650 (Expose Armor Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8650);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8676 (Ambush Rank 1) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8676);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8721 (Backstab Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8721);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8724 (Ambush Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8724);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8725 (Ambush Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8725);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8818 (Garrote Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8818);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8820 (Slam Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8820);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8905 (Wrath Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8905);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8921 (Moonfire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8921);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8924 (Moonfire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8924);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8925 (Moonfire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8925);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8926 (Moonfire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8926);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8927 (Moonfire Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8927);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8928 (Moonfire Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8928);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8929 (Moonfire Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8929);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8949 (Starfire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8949);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8950 (Starfire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8950);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8951 (Starfire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8951);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8955 (Soothe Animal Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8955);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8983 (Bash Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8983);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8992 (Shred Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(8992);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 8994 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8994);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8995 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8995);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8996 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8996);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 8997 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(8997);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9007 (Pounce Bleed Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9007);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9008 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9008);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9034 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9034);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9035 (Hex of Weakness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9035);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9053 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9053);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9057 (Firebolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9057);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9080 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9080);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9081 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9081);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9275 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9275);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9347 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9347);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9435 (Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9435);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9484 (Shackle Undead Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9484);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9485 (Shackle Undead Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9485);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9487 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9487);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9488 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9488);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9492 (Rip Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9492);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9493 (Rip Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9493);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9532 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9532);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9575 (Self Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9575);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9613 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9613);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9672 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9672);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9739 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9739);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9749 (Faerie Fire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9749);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9752 (Rip Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9752);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9771 (Radiation Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9771);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9824 (Pounce Bleed Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9824);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9826 (Pounce Bleed Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9826);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9829 (Shred Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(9829);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 9830 (Shred Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(9830);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 9833 (Moonfire Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9833);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9834 (Moonfire Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9834);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9835 (Moonfire Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9835);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9849 (Claw Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9849);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9850 (Claw Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9850);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9866 (Ravage Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(9866);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 9867 (Ravage Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(9867);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 9875 (Starfire Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9875);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9876 (Starfire Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9876);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9894 (Rip Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9894);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9896 (Rip Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9896);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9901 (Soothe Animal Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9901);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9904 (Rake Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9904);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9907 (Faerie Fire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9907);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 9912 (Wrath Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(9912);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10148 (Fireball Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10148);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10149 (Fireball Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10149);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10150 (Fireball Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10150);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10151 (Fireball Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10151);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10179 (Frostbolt Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10179);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10180 (Frostbolt Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10180);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10181 (Frostbolt Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10181);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10197 (Fire Blast Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10197);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10199 (Fire Blast Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10199);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10205 (Scorch Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10205);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10206 (Scorch Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10206);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10207 (Scorch Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10207);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10211 (Arcane Missiles Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10211);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10212 (Arcane Missiles Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10212);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10273 (Arcane Missiles Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10273);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10274 (Arcane Missiles Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10274);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10277 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10277);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10308 (Hammer of Justice Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10308);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10312 (Exorcism Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10312);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10313 (Exorcism Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10313);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10314 (Exorcism Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10314);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10318 (Holy Wrath Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10318);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10371 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10371);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10391 (Lightning Bolt Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10391);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10392 (Lightning Bolt Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10392);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10412 (Earth Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10412);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10413 (Earth Shock Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10413);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10414 (Earth Shock Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10414);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10447 (Flame Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10447);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10448 (Flame Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10448);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10472 (Frost Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10472);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10473 (Frost Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10473);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10578 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10578);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10605 (Chain Lightning Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10605);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10617 (Release Rageclaw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10617);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10797 (Starshards Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10797);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10833 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10833);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10874 (Mana Burn Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10874);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10875 (Mana Burn Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10875);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10876 (Mana Burn Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10876);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10887 (Crowd Pummel Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10887);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10892 (Shadow Word: Pain Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10892);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10893 (Shadow Word: Pain Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10893);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10894 (Shadow Word: Pain Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10894);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10933 (Smite Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10933);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10934 (Smite Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10934);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10945 (Mind Blast Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10945);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10946 (Mind Blast Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10946);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10947 (Mind Blast Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10947);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 10955 (Shackle Undead Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(10955);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11085 (Chain Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11085);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11113 (Blast Wave Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11113);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11197 (Expose Armor Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11197);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11198 (Expose Armor Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11198);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11264 (Ice Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11264);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11267 (Ambush Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11267);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11268 (Ambush Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11268);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11269 (Ambush Rank 6) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11269);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11273 (Rupture Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11273);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11274 (Rupture Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11274);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11275 (Rupture Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11275);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11279 (Backstab Rank 6) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11279);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11280 (Backstab Rank 7) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11280);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11281 (Backstab Rank 8) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11281);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11285 (Gouge Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11285);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11286 (Gouge Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11286);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11289 (Garrote Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11289);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11290 (Garrote Rank 6) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(11290);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 11293 (Sinister Strike Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11293);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11294 (Sinister Strike Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11294);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11297 (Sap Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11297);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11299 (Eviscerate Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11299);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11300 (Eviscerate Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11300);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11366 (Pyroblast Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11366);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11427 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11427);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11430 (Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11430);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11436 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11436);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11444 (Shackle Undead ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11444);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11538 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11538);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11564 (Heroic Strike Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11564);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11565 (Heroic Strike Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11565);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11566 (Heroic Strike Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11566);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11567 (Heroic Strike Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11567);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11572 (Rend Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11572);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11573 (Rend Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11573);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11574 (Rend Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11574);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11584 (Overpower Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11584);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11585 (Overpower Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11585);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11596 (Sunder Armor Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11596);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11597 (Sunder Armor Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11597);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11600 (Revenge Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11600);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11601 (Revenge Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11601);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11604 (Slam Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11604);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11605 (Slam Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11605);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11608 (Cleave Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11608);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11609 (Cleave Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11609);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11639 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11639);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11659 (Shadow Bolt Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11659);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11660 (Shadow Bolt Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11660);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11661 (Shadow Bolt Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11661);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11665 (Immolate Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11665);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11667 (Immolate Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11667);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11668 (Immolate Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11668);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11671 (Corruption Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11671);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11672 (Corruption Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11672);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11675 (Drain Soul Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11675);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11683 (Hellfire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11683);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11684 (Hellfire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11684);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11699 (Drain Life Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11699);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11700 (Drain Life Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11700);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11703 (Drain Mana Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11703);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11704 (Drain Mana Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11704);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11707 (Curse of Weakness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11707);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11708 (Curse of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11708);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11711 (Curse of Agony Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11711);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11712 (Curse of Agony Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11712);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11713 (Curse of Agony Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11713);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11717 (Curse of Recklessness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11717);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11719 (Curse of Tongues Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11719);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11721 (Curse of the Elements Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11721);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11722 (Curse of the Elements Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11722);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11725 (Enslave Demon Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11725);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11726 (Enslave Demon Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11726);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11762 (Firebolt Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11762);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11763 (Firebolt Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11763);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11815 (Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11815);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11839 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11839);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11891 (Antu'sul Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11891);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11921 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11921);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11962 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11962);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11971 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11971);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11972 (Shield Bash ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11972);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11977 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11977);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11978 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11978);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11980 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11980);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11981 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11981);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11985 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11985);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11988 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11988);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 11989 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(11989);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12054 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12054);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12058 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12058);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12096 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12096);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12167 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12167);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12170 (Revenge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12170);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12294 (Mortal Strike Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12294);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12331 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12331);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12355 (Impact Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12355);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12466 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12466);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12471 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12471);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12484 (Chilled Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12484);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12485 (Chilled Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12485);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12486 (Chilled Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12486);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12493 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12493);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12494 (Frostbite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12494);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12505 (Pyroblast Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12505);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12522 (Pyroblast Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12522);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12523 (Pyroblast Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12523);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12524 (Pyroblast Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12524);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12525 (Pyroblast Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12525);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12526 (Pyroblast Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12526);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12538 (Ravenous Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12538);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12540 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12540);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12542 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12542);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12548 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12548);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12555 (Pummel ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12555);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12675 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12675);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12693 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12693);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12733 (Fearless ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12733);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12737 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12737);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12739 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12739);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12741 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12741);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12742 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12742);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12745 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12745);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12824 (Polymorph Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12824);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12825 (Polymorph Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12825);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12826 (Polymorph Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12826);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12889 (Curse of Tongues ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12889);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 12964 (Unbridled Wrath Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(12964);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13005 (Hammer of Justice ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13005);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13009 (Amnennar's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13009);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13018 (Blast Wave Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13018);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13019 (Blast Wave Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13019);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13020 (Blast Wave Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13020);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13021 (Blast Wave Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13021);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13140 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13140);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13281 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13281);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13318 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13318);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13321 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13321);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13322 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13322);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13323 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13323);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13338 (Curse of Tongues ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13338);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13339 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13339);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13340 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13340);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13341 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13341);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13342 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13342);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13374 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13374);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13375 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13375);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13398 (Throw Wrench ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13398);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13424 (Faerie Fire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13424);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13438 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13438);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13439 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13439);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13440 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13440);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13441 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13441);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13442 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13442);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13443 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13443);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13444 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13444);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13445 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13445);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13480 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13480);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13482 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13482);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13491 (Pummel Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13491);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13527 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13527);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13530 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13530);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13553 (Serpent Sting Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13553);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13554 (Serpent Sting Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13554);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13555 (Serpent Sting Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13555);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13579 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13579);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13728 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13728);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13729 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13729);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13737 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13737);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13738 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13738);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13747 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13747);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13748 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13748);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13752 (Faerie Fire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13752);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13860 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13860);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13878 (Scorch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13878);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 13901 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(13901);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14032 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14032);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14033 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14033);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14034 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14034);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14087 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14087);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14103 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14103);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14105 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14105);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14106 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14106);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14109 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14109);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14118 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14118);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14119 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14119);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14122 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14122);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14145 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14145);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14200 (Chained Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14200);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14260 (Raptor Strike Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14260);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14261 (Raptor Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14261);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14262 (Raptor Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14262);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14263 (Raptor Strike Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14263);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14264 (Raptor Strike Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14264);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14265 (Raptor Strike Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14265);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14266 (Raptor Strike Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14266);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14267 (Wing Clip Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14267);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14268 (Wing Clip Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14268);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14269 (Mongoose Bite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14269);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14270 (Mongoose Bite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14270);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14271 (Mongoose Bite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14271);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14279 (Viper Sting Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14279);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14280 (Viper Sting Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14280);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14281 (Arcane Shot Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14281);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14282 (Arcane Shot Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14282);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14283 (Arcane Shot Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14283);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14284 (Arcane Shot Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14284);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14285 (Arcane Shot Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14285);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14286 (Arcane Shot Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14286);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14287 (Arcane Shot Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14287);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14288 (Multi-Shot Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14288);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14289 (Multi-Shot Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14289);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14290 (Multi-Shot Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14290);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14294 (Volley Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14294);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14295 (Volley Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14295);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14296 (Black Arrow Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14296);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14323 (Hunter's Mark Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14323);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14324 (Hunter's Mark Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14324);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14325 (Hunter's Mark Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14325);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14443 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14443);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14621 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14621);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14868 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14868);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14871 (Shadow Bolt Misfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14871);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14873 (Sinister Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14873);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14874 (Rupture ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14874);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14875 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14875);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14887 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14887);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14895 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14895);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14903 (Rupture ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14903);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 14914 (Holy Fire Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(14914);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15039 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15039);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15040 (Molten Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15040);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15043 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15043);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15089 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15089);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15091 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15091);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15095 (Molten Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15095);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15096 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15096);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15117 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15117);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15124 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15124);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15207 (Lightning Bolt Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15207);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15208 (Lightning Bolt Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15208);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15228 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15228);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15230 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15230);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15232 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15232);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15234 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15234);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15241 (Scorch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15241);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15242 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15242);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15243 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15243);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15245 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15245);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15254 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15254);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15261 (Holy Fire Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15261);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15262 (Holy Fire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15262);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15263 (Holy Fire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15263);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15264 (Holy Fire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15264);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15265 (Holy Fire Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15265);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15266 (Holy Fire Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15266);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15267 (Holy Fire Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15267);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15280 (Cleave Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15280);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15284 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15284);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15285 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15285);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15305 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15305);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15407 (Mind Flay Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15407);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15451 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15451);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15470 (Curse of Tongues ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15470);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15472 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15472);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15496 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15496);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15497 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15497);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15499 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15499);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15501 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15501);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15502 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15502);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15505 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15505);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15530 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15530);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15534 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15534);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15536 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15536);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15537 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15537);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15547 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15547);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15549 (Chained Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15549);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15570 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15570);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15572 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15572);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15574 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15574);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15579 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15579);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15581 (Sinister Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15581);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15583 (Rupture ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15583);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15584 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15584);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15587 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15587);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15592 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15592);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15598 (Firebolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15598);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15607 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15607);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15608 (Ravenous Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15608);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15610 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15610);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15611 (Lizard Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15611);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15612 (Lizard Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15612);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15613 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15613);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15614 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15614);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15615 (Pummel ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15615);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15616 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15616);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15619 (Throw Wrench ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15619);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15620 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15620);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15622 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15622);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15623 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15623);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15654 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15654);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15655 (Shield Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15655);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15659 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15659);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15661 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15661);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15662 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15662);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15663 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15663);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15665 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15665);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15667 (Sinister Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15667);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15691 (Eviscerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15691);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15692 (Eviscerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15692);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15708 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15708);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15732 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15732);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15735 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15735);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15736 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15736);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15744 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15744);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15754 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15754);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15785 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15785);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15790 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15790);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15791 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15791);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15795 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15795);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15798 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15798);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15800 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15800);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15801 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15801);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15850 (Chilled ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15850);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15878 (Ice Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15878);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15979 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15979);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 15980 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(15980);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16000 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16000);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16006 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16006);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16033 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16033);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16044 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16044);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16046 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16046);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16067 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16067);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16075 (Throw Axe ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16075);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16100 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16100);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16101 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16101);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16144 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16144);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16145 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16145);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16231 (Curse of Recklessness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16231);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16249 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16249);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16250 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16250);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16375 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16375);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16393 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16393);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16402 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16402);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16403 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16403);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16406 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16406);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16407 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16407);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16408 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16408);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16409 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16409);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16410 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16410);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16412 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16412);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16413 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16413);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16414 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16414);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16415 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16415);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16454 (Searing Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16454);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16496 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16496);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16498 (Faerie Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16498);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16509 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16509);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16511 (Hemorrhage Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16511);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16553 (Ravenous Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16553);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16554 (Toxic Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16554);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16559 (Flame Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16559);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16560 (Flame Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16560);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16568 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16568);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16570 (Charged Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16570);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16572 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16572);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16608 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16608);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16767 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16767);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16768 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16768);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16772 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16772);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16775 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16775);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16776 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16776);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16777 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16777);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16778 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16778);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16779 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16779);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16780 (Shoot Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16780);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16782 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16782);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16783 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16783);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16784 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16784);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16788 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16788);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16799 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16799);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16804 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16804);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16827 (Claw Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16827);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16828 (Claw Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16828);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16829 (Claw Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16829);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16830 (Claw Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16830);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16831 (Claw Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16831);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16832 (Claw Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16832);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16856 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16856);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16857 (Faerie Fire (Feral) Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16857);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16921 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16921);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16922 (Starfire Stun ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16922);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 16927 (Chilled ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(16927);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17008 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17008);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17011 (Freezing Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17011);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17140 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17140);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17141 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17141);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17142 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17142);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17144 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17144);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17145 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17145);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17146 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17146);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17147 (Exorcism ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17147);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17149 (Exorcism ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17149);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17153 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17153);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17165 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17165);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17173 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17173);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17194 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17194);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17195 (Scorch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17195);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17198 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17198);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17203 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17203);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17227 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17227);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17228 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17228);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17238 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17238);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17243 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17243);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17253 (Bite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17253);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17255 (Bite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17255);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17256 (Bite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17256);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17257 (Bite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17257);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17258 (Bite Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17258);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17259 (Bite Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17259);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17260 (Bite Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17260);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17261 (Bite Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17261);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17273 (Pyroblast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17273);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17274 (Pyroblast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17274);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17277 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17277);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17287 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17287);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17290 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17290);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17311 (Mind Flay Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17311);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17312 (Mind Flay Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17312);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17313 (Mind Flay Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17313);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17314 (Mind Flay Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17314);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17347 (Hemorrhage Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17347);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17348 (Hemorrhage Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17348);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17353 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17353);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17368 (Egan's Blaster ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17368);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17390 (Faerie Fire (Feral) Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17390);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17391 (Faerie Fire (Feral) Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17391);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17392 (Faerie Fire (Feral) Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17392);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17393 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17393);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17434 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17434);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17435 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17435);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17470 (Ravenous Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17470);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17483 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17483);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17503 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17503);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17504 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17504);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17509 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17509);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17510 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17510);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17547 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17547);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17615 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17615);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17620 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17620);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17630 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17630);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17668 (Ramstein's Lightning Bolts ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17668);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17669 (Ramstein's Lightning Bolts ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17669);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17682 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17682);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17685 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17685);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17748 (Kick Peon ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17748);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17771 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17771);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17862 (Curse of Shadow Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17862);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17883 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17883);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17919 (Searing Pain Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17919);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17920 (Searing Pain Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17920);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17921 (Searing Pain Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17921);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17922 (Searing Pain Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17922);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17923 (Searing Pain Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17923);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17924 (Soul Fire Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17924);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17925 (Death Coil Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17925);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17926 (Death Coil Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17926);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17937 (Curse of Shadow Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17937);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 17962 (Conflagrate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(17962);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18075 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18075);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18078 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18078);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18081 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18081);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18082 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18082);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18083 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18083);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18084 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18084);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18085 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18085);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18086 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18086);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18088 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18088);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18089 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18089);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18091 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18091);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18092 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18092);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18101 (Chilled Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18101);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18104 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18104);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18105 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18105);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18106 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18106);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18108 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18108);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18111 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18111);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18112 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18112);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18137 (Shadowguard Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18137);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18138 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18138);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18164 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18164);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18165 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18165);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18187 (Firebolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18187);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18199 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18199);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18200 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18200);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18202 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18202);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18204 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18204);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18205 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18205);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18211 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18211);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18214 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18214);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18217 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18217);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18265 (Siphon Life Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18265);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18266 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18266);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18267 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18267);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18276 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18276);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18371 (Drain Soul ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18371);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18376 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18376);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18392 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18392);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18396 (Dismounting Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18396);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18398 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18398);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18425 (Kick - Silenced ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18425);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18498 (Shield Bash - Silenced ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18498);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18557 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18557);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18561 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18561);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18647 (Banish Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18647);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18651 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18651);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18656 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18656);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18657 (Hibernate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18657);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18658 (Hibernate Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18658);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18662 (Curse of Doom Effect ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18662);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18671 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18671);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18796 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18796);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18807 (Mind Flay Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18807);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18809 (Pyroblast Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18809);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18817 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18817);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18833 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18833);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18879 (Siphon Life Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18879);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18880 (Siphon Life Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18880);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18881 (Siphon Life Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18881);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18930 (Conflagrate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18930);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18931 (Conflagrate Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18931);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18932 (Conflagrate Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18932);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 18972 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(18972);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19032 (Throw Human Skull ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19032);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19130 (Revenge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19130);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19133 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19133);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19136 (Stormbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19136);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19137 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19137);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19229 (Improved Wing Clip ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19229);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19249 (Touch of Weakness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19249);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19251 (Touch of Weakness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19251);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19252 (Touch of Weakness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19252);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19253 (Touch of Weakness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19253);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19254 (Touch of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19254);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19260 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19260);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19261 (Touch of Weakness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19261);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19262 (Touch of Weakness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19262);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19264 (Touch of Weakness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19264);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19265 (Touch of Weakness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19265);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19266 (Touch of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19266);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19276 (Devouring Plague Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19276);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19277 (Devouring Plague Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19277);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19278 (Devouring Plague Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19278);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19279 (Devouring Plague Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19279);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19280 (Devouring Plague Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19280);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19281 (Hex of Weakness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19281);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19282 (Hex of Weakness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19282);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19283 (Hex of Weakness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19283);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19284 (Hex of Weakness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19284);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19285 (Hex of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19285);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19296 (Starshards Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19296);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19299 (Starshards Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19299);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19302 (Starshards Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19302);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19303 (Starshards Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19303);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19304 (Starshards Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19304);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19305 (Starshards Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19305);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19306 (Counterattack Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19306);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19308 (Shadowguard Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19308);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19309 (Shadowguard Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19309);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19310 (Shadowguard Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19310);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19311 (Shadowguard Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19311);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19312 (Shadowguard Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19312);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19386 (Wyvern Sting Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19386);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19391 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19391);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19397 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19397);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19472 (Sinister Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19472);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19574 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19574);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19597 (Tame Ice Claw Bear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19597);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19632 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19632);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19639 (Pummel ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19639);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19642 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19642);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19643 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19643);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19728 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19728);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19729 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19729);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19776 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19776);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19785 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19785);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19816 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19816);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19820 (Mangle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19820);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19874 (Lightning Bolt Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19874);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19881 (Shoot (TEST) ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19881);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 19983 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(19983);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20005 (Chilled ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20005);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20184 (Judgement of Justice Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20184);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20185 (Judgement of Light Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20185);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20186 (Judgement of Wisdom Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20186);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20187 (Judgement of Righteousness Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20187);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20188 (Judgement of the Crusader Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20188);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20228 (Pyroblast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20228);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20229 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20229);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20243 (Devastate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20243);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20253 (Intercept Stun Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20253);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20267 (Judgement of Light Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20267);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20268 (Judgement of Wisdom Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20268);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20271 (Judgement ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20271);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20280 (Judgement of Righteousness Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20280);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20281 (Judgement of Righteousness Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20281);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20282 (Judgement of Righteousness Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20282);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20283 (Judgement of Righteousness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20283);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20284 (Judgement of Righteousness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20284);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20285 (Judgement of Righteousness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20285);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20286 (Judgement of Righteousness Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20286);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20294 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20294);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20295 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20295);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20297 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20297);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20298 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20298);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20300 (Judgement of the Crusader Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20300);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20301 (Judgement of the Crusader Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20301);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20302 (Judgement of the Crusader Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20302);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20303 (Judgement of the Crusader Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20303);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20341 (Judgement of Light Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20341);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20342 (Judgement of Light Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20342);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20343 (Judgement of Light Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20343);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20344 (Judgement of Light Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20344);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20345 (Judgement of Light Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20345);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20346 (Judgement of Light Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20346);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20352 (Judgement of Wisdom Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20352);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20353 (Judgement of Wisdom Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20353);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20354 (Judgement of Wisdom Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20354);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20355 (Judgement of Wisdom Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20355);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20420 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20420);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20425 (Judgement of Command Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20425);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20463 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20463);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20467 (Judgement of Command Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20467);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20473 (Holy Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20473);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20508 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20508);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20559 (Mocking Blow Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20559);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20560 (Mocking Blow Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20560);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20565 (Magma Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20565);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20566 (Wrath of Ragnaros ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20566);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20569 (Cleave Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20569);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20605 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20605);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20614 (Intercept Stun Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20614);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20615 (Intercept Stun Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20615);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20623 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20623);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20647 (Execute ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20647);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20656 (Faerie Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20656);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20666 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20666);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20677 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20677);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20678 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20678);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20679 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20679);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20684 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20684);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20685 (Storm Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20685);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20690 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20690);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20691 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20691);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20692 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20692);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20698 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20698);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20714 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20714);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20720 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20720);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20733 (Black Arrow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20733);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20735 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20735);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20741 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20741);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20743 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20743);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20787 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20787);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20791 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20791);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20792 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20792);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20793 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20793);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20795 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20795);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20797 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20797);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20800 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20800);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20801 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20801);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20802 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20802);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20805 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20805);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20806 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20806);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20807 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20807);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20808 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20808);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20811 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20811);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20815 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20815);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20816 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20816);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20817 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20817);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20819 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20819);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20822 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20822);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20823 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20823);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20824 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20824);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20825 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20825);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20826 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20826);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20829 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20829);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20830 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20830);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20831 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20831);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20832 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20832);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20869 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20869);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20882 (Enslave Demon ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20882);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20883 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20883);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20909 (Counterattack Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20909);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20910 (Counterattack Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20910);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20929 (Holy Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20929);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20930 (Holy Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20930);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20961 (Judgement of Command Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20961);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20962 (Judgement of Command Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20962);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20963 (Judgement of Command Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20963);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20964 (Judgement of Command Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20964);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20965 (Judgement of Command Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20965);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20966 (Judgement of Command Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20966);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20967 (Judgement of Command Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20967);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 20968 (Judgement of Command Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(20968);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21007 (Curse of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21007);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21008 (Mocking Blow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21008);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21030 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21030);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21052 (Monty Bashes Rats (DND) ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21052);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21066 (Void Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21066);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21067 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21067);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21068 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21068);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21072 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21072);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21077 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21077);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21081 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21081);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21141 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21141);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21159 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21159);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21162 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21162);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21170 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21170);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21179 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21179);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21183 (Judgement of the Crusader Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21183);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21230 (Fireball, Improved DoT ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21230);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21341 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21341);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21369 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21369);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21390 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21390);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21401 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21401);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21402 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21402);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21549 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21549);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21551 (Mortal Strike Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21551);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21552 (Mortal Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21552);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21553 (Mortal Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21553);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21667 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21667);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21668 (Starfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21668);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21669 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21669);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21670 (Faerie Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21670);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21807 (Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21807);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21887 (Warrior's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21887);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21889 (Judgement Smite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21889);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21949 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21949);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21971 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21971);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21977 (Warrior's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(21977);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 21987 (Lash of Pain ) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(21987);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 22088 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22088);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22120 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22120);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22121 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22121);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22189 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22189);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22206 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22206);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22272 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22272);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22273 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22273);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22336 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22336);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22355 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22355);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22356 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22356);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22357 (Icebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22357);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22411 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22411);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22414 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22414);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22423 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22423);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22424 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22424);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22425 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22425);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22438 (Mark of Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22438);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22439 (Mark of Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22439);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22540 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22540);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22568 (Ferocious Bite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22568);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22570 (Maim Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22570);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22582 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22582);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22639 (Eskhandar's Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22639);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22643 (Frostbolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22643);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22665 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22665);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22677 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22677);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22678 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22678);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22689 (Mangle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22689);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22709 (Void Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22709);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22822 (Starshards ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22822);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22823 (Starshards ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22823);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22827 (Ferocious Bite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22827);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22828 (Ferocious Bite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22828);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22829 (Ferocious Bite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22829);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22851 (Ferocious Bite Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22851);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22853 (Ferocious Bite Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22853);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22861 (Ferocious Bite Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22861);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22862 (Ferocious Bite Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22862);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22878 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22878);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22885 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22885);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22887 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22887);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22893 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22893);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22907 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22907);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22908 (Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22908);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22911 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22911);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22919 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22919);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22920 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22920);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22936 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22936);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22937 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22937);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22940 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22940);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 22947 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(22947);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23024 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23024);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23038 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23038);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23039 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23039);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23073 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23073);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23102 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23102);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23106 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23106);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23113 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23113);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23114 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23114);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23115 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23115);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23206 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23206);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23267 (Firebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23267);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23268 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23268);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23269 (Holy Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23269);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23308 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23308);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23309 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23309);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23331 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23331);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23337 (Shoot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23337);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23380 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23380);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23411 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23411);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23412 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23412);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23512 (Fireball Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23512);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23590 (Judgement ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23590);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23592 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23592);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23642 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23642);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23722 (Arcane Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23722);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23860 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23860);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23880 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23880);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23881 (Bloodthirst Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23881);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23885 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23885);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23886 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23886);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23887 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23887);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23888 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23888);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23889 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23889);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23890 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23890);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23891 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23891);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23892 (Bloodthirst Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23892);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23893 (Bloodthirst Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23893);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23894 (Bloodthirst Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23894);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23922 (Shield Slam Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23922);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23923 (Shield Slam Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23923);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23924 (Shield Slam Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23924);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23925 (Shield Slam Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23925);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23952 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23952);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23953 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23953);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23959 (Test Stab R50 Rank 8) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(23959);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 23968 (Throw Liquid Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23968);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23969 (Throw Liquid Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23969);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23970 (Throw Liquid Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23970);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 23979 (Holy Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(23979);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24099 (Poison Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24099);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24131 (Wyvern Sting Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24131);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24132 (Wyvern Sting Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24132);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24133 (Wyvern Sting Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24133);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24134 (Wyvern Sting Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24134);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24135 (Wyvern Sting Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24135);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24187 (Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24187);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24193 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24193);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24212 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24212);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24213 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24213);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24239 (Hammer of Wrath Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24239);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24274 (Hammer of Wrath Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24274);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24275 (Hammer of Wrath Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24275);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24300 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24300);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24317 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24317);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24331 (Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24331);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24332 (Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24332);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24333 (Ravage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24333);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24335 (Wyvern Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24335);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24336 (Wyvern Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24336);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24374 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24374);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24395 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24395);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24396 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24396);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24397 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24397);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24407 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24407);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24408 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24408);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24415 (Slow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24415);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24435 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24435);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24466 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24466);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24530 (Felfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24530);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24573 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24573);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24585 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24585);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24611 (Fireball ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24611);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24618 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24618);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24668 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24668);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24680 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24680);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24685 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24685);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24698 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24698);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24816 (Test Mortal Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24816);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24817 (Test Overpower Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24817);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24820 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24820);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24821 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24821);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24822 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24822);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24823 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24823);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24824 (Test Sinister Strike Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24824);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24825 (Test Backstab Rank 8) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(24825);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 24834 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24834);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24835 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24835);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24836 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24836);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24837 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24837);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24838 (Shadow Bolt Whirl ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24838);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24857 (Arcane Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24857);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24910 (Corruption of the Earth ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24910);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24942 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24942);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24957 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24957);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24974 (Insect Swarm Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24974);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24975 (Insect Swarm Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24975);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24976 (Insect Swarm Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24976);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24977 (Insect Swarm Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24977);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24993 (Acid Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24993);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 24995 (Pyroblast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(24995);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25004 (Throw Nightmare Object ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25004);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25021 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25021);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25025 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25025);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25028 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25028);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25030 (Shoot Rocket ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25030);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25031 (Shoot Missile ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25031);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25049 (Blast Wave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25049);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25051 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25051);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25055 (Arcane Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25055);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25099 (Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25099);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25195 (Curse of Tongues ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25195);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25208 (Rend Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25208);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25212 (Hamstring Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25212);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25225 (Sunder Armor Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25225);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25231 (Cleave Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25231);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25234 (Execute Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25234);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25236 (Execute Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25236);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25241 (Slam Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25241);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25242 (Slam Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25242);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25248 (Mortal Strike Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25248);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25251 (Bloodthirst Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25251);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25252 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25252);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25253 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25253);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25258 (Shield Slam Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25258);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25266 (Mocking Blow Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25266);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25269 (Revenge Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25269);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25273 (Intercept Stun Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25273);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25274 (Intercept Stun Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25274);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25286 (Heroic Strike Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25286);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25288 (Revenge Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25288);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25294 (Multi-Shot Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25294);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25295 (Serpent Sting Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25295);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25298 (Starfire Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25298);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25300 (Backstab Rank 9) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(25300);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 25304 (Frostbolt Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25304);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25306 (Fireball Rank 12) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25306);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25307 (Shadow Bolt Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25307);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25309 (Immolate Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25309);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25311 (Corruption Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25311);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25345 (Arcane Missiles Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25345);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25346 (Arcane Missiles Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25346);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25363 (Smite Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25363);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25364 (Smite Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25364);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25367 (Shadow Word: Pain Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25367);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25368 (Shadow Word: Pain Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25368);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25372 (Mind Blast Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25372);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25375 (Mind Blast Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25375);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25379 (Mana Burn Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25379);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25380 (Mana Burn Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25380);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25384 (Holy Fire Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25384);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25387 (Mind Flay Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25387);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25424 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25424);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25439 (Chain Lightning Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25439);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25442 (Chain Lightning Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25442);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25446 (Starshards Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25446);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25454 (Earth Shock Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25454);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25457 (Flame Shock Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25457);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25460 (Touch of Weakness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25460);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25461 (Touch of Weakness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25461);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25464 (Frost Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25464);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25467 (Devouring Plague Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25467);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25470 (Hex of Weakness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25470);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25477 (Shadowguard Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25477);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25515 (Bash ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25515);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25586 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25586);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25595 (Savage Pummel Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25595);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25671 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25671);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25710 (Heroic Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25710);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25712 (Heroic Strike Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25712);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25755 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25755);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25756 (Purge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25756);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25759 (Video Camera 1 ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25759);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25760 (Video Camera 2 ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25760);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25761 (Video Camera 3 ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25761);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25762 (Video Camera 4 ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25762);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25779 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25779);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25805 (Soul Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25805);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25811 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25811);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25816 (Hex of Weakness Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25816);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25821 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25821);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25902 (Holy Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25902);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25903 (Holy Shock Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25903);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25907 (Spell Blasting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25907);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25911 (Holy Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25911);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25912 (Holy Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25912);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25913 (Holy Shock Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25913);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25914 (Holy Shock Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25914);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 25991 (Poison Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(25991);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26006 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26006);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26044 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26044);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26046 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26046);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26048 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26048);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26049 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26049);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26052 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26052);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26070 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26070);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26098 (Lightning Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26098);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26102 (Sand Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26102);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26121 (Stormcaller's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26121);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26141 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26141);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26143 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26143);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26180 (Wyvern Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26180);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26194 (Earth Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26194);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26211 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26211);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26233 (Wyvern Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26233);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26282 (Shoot Tauren Rifleman ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26282);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26350 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26350);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26385 (Shadowburn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26385);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26458 (Shock Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26458);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26580 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26580);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26592 (Bestial Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26592);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26601 (Poison Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26601);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26639 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26639);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26661 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26661);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26679 (Deadly Throw Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26679);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26693 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26693);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26748 (Wyvern Sting Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26748);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26839 (Garrote Rank 7) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(26839);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 26861 (Sinister Strike Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26861);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26862 (Sinister Strike Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26862);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26863 (Backstab Rank 10) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(26863);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 26864 (Hemorrhage Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26864);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26865 (Eviscerate Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26865);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26866 (Expose Armor Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26866);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26867 (Rupture Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26867);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26884 (Garrote Rank 8) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(26884);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 27001 (Shred Rank 6) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(27001);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 27002 (Shred Rank 7) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(27002);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 27005 (Ravage Rank 5) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(27005);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 27014 (Raptor Strike Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27014);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27016 (Serpent Sting Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27016);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27018 (Viper Sting Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27018);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27019 (Arcane Shot Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27019);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27021 (Multi-Shot Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27021);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27022 (Volley Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27022);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27067 (Counterattack Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27067);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27068 (Wyvern Sting Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27068);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27069 (Wyvern Sting Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27069);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27138 (Exorcism Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27138);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27174 (Holy Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27174);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27175 (Holy Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27175);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27176 (Holy Shock Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27176);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27180 (Hammer of Wrath Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27180);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27210 (Searing Pain Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27210);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27211 (Soul Fire Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27211);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27213 (Hellfire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27213);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27215 (Immolate Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27215);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27216 (Corruption Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27216);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27217 (Drain Soul Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27217);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27218 (Curse of Agony Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27218);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27219 (Drain Life Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27219);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27220 (Drain Life Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27220);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27221 (Drain Mana Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27221);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27223 (Death Coil Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27223);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27224 (Curse of Weakness Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27224);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27226 (Curse of Recklessness Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27226);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27228 (Curse of the Elements Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27228);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27229 (Curse of Shadow Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27229);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27243 (Seed of Corruption Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27243);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27264 (Siphon Life Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27264);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27285 (Seed of Corruption Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27285);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27286 (Shadow Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27286);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27382 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27382);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27383 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27383);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27441 (Ambush Rank 7) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(27441);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 27499 (Crusader's Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27499);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27555 (Shred ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27555);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27556 (Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27556);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27557 (Ferocious Bite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27557);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27565 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27565);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27567 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27567);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27580 (Mortal Strike Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27580);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27584 (Hamstring Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27584);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27605 (Shadow Word: Pain Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27605);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27611 (Eviscerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27611);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27613 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27613);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27615 (Kidney Shot Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27615);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27626 (Purge Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27626);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27633 (Wing Clip Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27633);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27636 (Starshards ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27636);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27638 (Rake ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27638);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27641 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27641);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27646 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27646);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27662 (Throw Cupid's Dart ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27662);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27737 (Moonfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27737);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27760 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27760);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27794 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27794);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27808 (Frost Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27808);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27812 (Void Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27812);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27814 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27814);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27820 (Mana Detonation ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27820);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27826 (Intercept ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27826);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27831 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27831);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27990 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27990);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27991 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27991);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27992 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27992);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 27994 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(27994);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28167 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28167);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28271 (Polymorph Rank 1: Turtle) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28271);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28272 (Polymorph Rank 1: Pig) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28272);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28293 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28293);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28301 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28301);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28310 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28310);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28311 (Slime Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28311);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28377 (Shadowguard Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28377);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28378 (Shadowguard Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28378);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28379 (Shadowguard Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28379);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28380 (Shadowguard Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28380);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28381 (Shadowguard Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28381);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28382 (Shadowguard Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28382);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28385 (Shadowguard Rank 7) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28385);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28407 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28407);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28412 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28412);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28448 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28448);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28456 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28456);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28457 (Dark Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28457);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28478 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28478);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28479 (Frostbolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28479);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28522 (Icebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28522);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28526 (Icebolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28526);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28598 (Touch of Weakness ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28598);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28599 (Shadow Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28599);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28608 (Test Curse of Agony Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28608);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28750 (Blessing of the Claw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28750);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28796 (Poison Bolt Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28796);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28883 (Holy Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28883);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 28900 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(28900);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29058 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29058);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29124 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29124);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29155 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29155);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29168 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29168);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29183 (Polymorph Immunity ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29183);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29214 (Wrath of the Plaguebringer ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29214);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29228 (Flame Shock Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29228);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29310 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29310);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29317 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29317);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29320 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29320);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29321 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29321);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29341 (Shadowburn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29341);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29405 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29405);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29407 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29407);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29425 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29425);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29426 (Heroic Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29426);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29492 (Searing Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29492);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29515 (TEST Scorch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29515);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29522 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29522);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29560 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29560);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29561 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29561);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29563 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29563);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29567 (Heroic Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29567);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29570 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29570);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29572 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29572);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29574 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29574);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29576 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29576);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29578 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29578);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29579 (Throw Dynamite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29579);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29582 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29582);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29586 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29586);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29640 (Shadow Bolt ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29640);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29644 (Fire Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29644);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29665 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29665);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29666 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29666);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29667 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29667);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29684 (Shield Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29684);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29707 (Heroic Strike Rank 10) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29707);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29722 (Incinerate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29722);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29848 (Polymorph ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29848);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29881 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29881);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29928 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29928);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 29930 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(29930);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30002 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30002);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30014 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30014);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30016 (Devastate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30016);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30017 (Devastate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30017);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30022 (Devastate Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30022);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30108 (Unstable Affliction Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30108);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30131 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30131);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30153 (Intercept Stun Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30153);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30195 (Intercept Stun Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30195);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30197 (Intercept Stun Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30197);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30213 (Cleave Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30213);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30219 (Cleave Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30219);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30223 (Cleave Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30223);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30231 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30231);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30324 (Heroic Strike Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30324);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30330 (Mortal Strike Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30330);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30335 (Bloodthirst Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30335);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30339 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30339);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30340 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30340);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30356 (Shield Slam Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30356);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30357 (Revenge Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30357);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30358 (Searing Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30358);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30404 (Unstable Affliction Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30404);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30405 (Unstable Affliction Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30405);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30412 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30412);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30459 (Searing Pain Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30459);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30460 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30460);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30470 (Slice and Dice ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30470);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30475 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30475);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30476 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30476);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30478 (Hemorrhage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30478);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30482 (Molten Armor Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30482);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30493 (Shoot Bow ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30493);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30500 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30500);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30530 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30530);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30545 (Soul Fire Rank 4) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30545);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30584 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30584);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30615 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30615);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30619 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30619);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30621 (Kidney Shot Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30621);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30688 (Shield Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30688);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30741 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30741);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30832 (Kidney Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30832);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30846 (Throw Proximity Bomb ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30846);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30854 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30854);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30859 (Hellfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30859);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30898 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30898);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30901 (Sunder Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30901);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30908 (Drain Mana Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30908);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30909 (Curse of Weakness Rank 8) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30909);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30910 (Curse of Doom Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30910);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30911 (Siphon Life Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30911);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30933 (Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30933);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30938 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30938);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30980 (Sap ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30980);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30989 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30989);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 30990 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(30990);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31016 (Eviscerate Rank 9) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31016);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31018 (Ferocious Bite Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31018);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31043 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31043);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31117 (Unstable Affliction ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31117);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31312 (Drunken Avenger's Shield ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31312);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31330 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31330);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31345 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31345);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31358 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31358);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31378 (Worm Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31378);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31402 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31402);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31405 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31405);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31407 (Viper Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31407);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31516 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31516);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31553 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31553);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31566 (Raptor Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31566);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31615 (Hunter's Mark ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31615);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31717 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31717);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31729 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31729);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31733 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31733);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31751 (Arcane Missiles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31751);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31779 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31779);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31827 (Heroic Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31827);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31911 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31911);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31935 (Avenger's Shield Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31935);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31942 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31942);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31946 (Throw Freezing Trap ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31946);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31970 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31970);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31975 (Serpent Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31975);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31997 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31997);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 31998 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(31998);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32001 (Throw Gordawg's Boulder ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32001);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32063 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32063);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32105 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32105);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32154 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32154);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32197 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32197);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32231 (Incinerate Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32231);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32241 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32241);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32323 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32323);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32337 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32337);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32379 (Shadow Word: Death Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32379);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32409 (Shadow Word: Death Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32409);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32416 (Hammer of Justice ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32416);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32417 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32417);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32418 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32418);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32554 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32554);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32645 (Envenom Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32645);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32674 (Avenger's Shield ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32674);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32684 (Envenom Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32684);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32699 (Avenger's Shield Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32699);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32700 (Avenger's Shield Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32700);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32707 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32707);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32709 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32709);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32736 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32736);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32771 (Holy Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32771);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32772 (Hammer of Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32772);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32774 (Avenger's Shield ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32774);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32784 (Throw Proximity Bomb ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32784);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32861 (Shadowguard ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32861);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32862 (Drain Soul ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32862);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32863 (Seed of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32863);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32864 (Kidney Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32864);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32865 (Seed of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32865);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32908 (Wing Clip ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32908);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32915 (Raptor Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32915);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32967 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32967);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 32996 (Shadow Word: Death Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(32996);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33070 (Cloud of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33070);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33072 (Holy Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33072);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33073 (Holy Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33073);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33074 (Holy Shock Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33074);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33130 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33130);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33385 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33385);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33424 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33424);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33547 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33547);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33625 (Purge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33625);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33632 (Exorcism ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33632);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33643 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33643);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33670 (Throw Explosives ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33670);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33709 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33709);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33745 (Lacerate Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33745);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33805 (Throw Hammer Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33805);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33806 (Throw Hammer Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33806);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33808 (Shoot Gun Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33808);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33822 (Throw Hammer Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33822);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33871 (Shield Bash ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33871);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 33924 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(33924);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34076 (Fear the Worgs ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34076);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34095 (Throw Proximity Bomb ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34095);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34100 (Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34100);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34107 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34107);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34259 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34259);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34353 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34353);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34354 (Flame Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34354);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34389 (Throw Doomhammer ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34389);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34411 (Mutilate Rank 2) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(34411);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 34412 (Mutilate Rank 3) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(34412);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 34413 (Mutilate Rank 4) is in behind from spell extra.
	sp = dbcSpell.LookupEntryForced(34413);
	if( sp != NULL )
		sp->in_front_status = 2;

	// Spell 34428 (Victory Rush Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34428);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34437 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34437);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34438 (Unstable Affliction ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34438);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34439 (Unstable Affliction ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34439);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34441 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34441);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34620 (Slam ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34620);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34659 (Hellfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34659);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34660 (Hellfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34660);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34696 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34696);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34802 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34802);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34806 (Drain Mana Visual ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34806);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34829 (Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34829);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34832 (Hunter's Mark Visual Only Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34832);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34846 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34846);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34879 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34879);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34913 (Molten Armor Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34913);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34914 (Vampiric Touch Rank 1) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34914);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34916 (Vampiric Touch Rank 2) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34916);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34917 (Vampiric Touch Rank 3) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34917);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34919 (Vampiric Touch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34919);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34930 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34930);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34931 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34931);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34940 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34940);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34941 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34941);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34942 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34942);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34974 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34974);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 34995 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(34995);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35054 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35054);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35123 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35123);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35125 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35125);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35139 (Throw Boom's Doom ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35139);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35155 (Smite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35155);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35160 (Holy Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35160);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35178 (Shield Bash ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35178);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35182 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35182);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35183 (Unstable Affliction ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35183);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35195 (Siphon Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35195);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35276 (Throw Dynamite ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35276);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35332 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35332);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35401 (Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35401);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35412 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35412);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35507 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35507);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35511 (Serpent Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35511);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35570 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35570);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35724 (Throw Torch ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35724);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35748 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35748);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35754 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35754);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35839 (Drain Soul ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35839);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35851 (Test Sunder Armor Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35851);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35915 (Molten Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35915);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35916 (Molten Armor ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35916);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35947 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35947);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35948 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35948);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35950 (Volley ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35950);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35954 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35954);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 35963 (Improved Wing Clip ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(35963);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36033 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36033);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36058 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36058);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36088 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36088);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36095 (Drain Mana ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36095);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36123 (Seed of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36123);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36140 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36140);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36224 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36224);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36293 (Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36293);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36340 (Holy Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36340);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36470 (Pummel ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36470);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36484 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36484);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36509 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36509);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36608 (Charged Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36608);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36609 (Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36609);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36623 (Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36623);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36638 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36638);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36645 (Throw Rock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36645);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36651 (Throw Rock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36651);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36655 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36655);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36825 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36825);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36831 (Curse of the Elements ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36831);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36832 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36832);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36862 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36862);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36891 (Devastate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36891);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36894 (Devastate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36894);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36896 (Chain Lightning 02 ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36896);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36916 (Mongoose Bite Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36916);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36947 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36947);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36965 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36965);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36979 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36979);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36984 (Serpent Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36984);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36988 (Shield Bash ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36988);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 36991 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(36991);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37066 (Garrote ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37066);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37074 (Deadly Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37074);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37113 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37113);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37159 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37159);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37176 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37176);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37251 (Hammer of Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37251);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37255 (Hammer of Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37255);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37259 (Hammer of Wrath ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37259);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37275 (Shadow Word: Pain ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37275);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37276 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37276);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37321 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37321);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37330 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37330);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37331 (Hemorrhage ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37331);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37332 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37332);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37334 (Curse of Agony ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37334);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37335 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37335);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37369 (Hammer of Justice ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37369);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37428 (Hellfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37428);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37448 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37448);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37476 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37476);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37511 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37511);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37517 (Revenge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37517);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37527 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37527);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37529 (Overpower ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37529);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37531 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37531);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37546 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37546);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37551 (Viper Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37551);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37554 (Avenger's Shield ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37554);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37598 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37598);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37621 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37621);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37628 (Fel Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37628);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37662 (Rend ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37662);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37668 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37668);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37790 (Spread Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37790);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37826 (Seed of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37826);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37833 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37833);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37865 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37865);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37961 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37961);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 37992 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(37992);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38009 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38009);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38065 (Death Coil ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38065);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38154 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38154);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38219 (Mark of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38219);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38220 (Mark of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38220);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38221 (Mark of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38221);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38222 (Mark of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38222);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38230 (Mark of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38230);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38233 (Shield Bash ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38233);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38234 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38234);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38243 (Mind Flay ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38243);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38252 (Seed of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38252);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38259 (Mind Blast ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38259);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38260 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38260);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38262 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38262);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38310 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38310);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38313 (Pummel ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38313);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38376 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38376);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38379 (Shadowguard ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38379);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38383 (Multi-Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38383);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38401 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38401);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38461 (Charge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38461);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38474 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38474);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38556 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38556);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38557 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38557);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38558 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38558);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38559 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38559);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38560 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38560);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38561 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38561);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38562 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38562);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38563 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38563);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38564 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38564);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38565 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38565);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38566 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38566);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38567 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38567);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38568 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38568);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38569 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38569);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38570 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38570);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38585 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38585);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38595 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38595);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38625 (Kick ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38625);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38631 (Avenger's Shield ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38631);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38660 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38660);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38764 (Gouge Rank 6) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38764);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38768 (Kick Rank 5) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38768);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38791 (Banish ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38791);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38806 (Immolate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38806);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38807 (Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38807);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38808 (Charged Arcane Shot ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38808);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38817 (Drain Life ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38817);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38849 (Devastate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38849);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38859 (Serpent Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38859);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38863 (Gouge ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38863);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38883 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38883);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38884 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38884);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38904 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38904);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38914 (Serpent Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38914);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38918 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38918);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38921 (Holy Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38921);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38959 (Execute ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38959);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38967 (Devastate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38967);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 38995 (Hamstring ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(38995);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39020 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39020);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39043 (Kick Akuno ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39043);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39047 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39047);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39060 (Throw ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39060);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39062 (Frost Shock ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39062);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39066 (Chain Lightning ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39066);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39071 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39071);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39072 (Bloodthirst ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39072);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39077 (Hammer of Justice ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39077);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39083 (Incinerate ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39083);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39119 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39119);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39131 (Hellfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39131);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39132 (Hellfire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39132);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39136 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39136);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39146 (Throw Orange Juice Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39146);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39147 (Throw Tar Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39147);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39148 (Throw Blood Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39148);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39149 (Throw Frosty Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39149);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39150 (Throw Random Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39150);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39151 (Throw 15 Bottles ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39151);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39154 (Throw Acid Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39154);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39155 (Throw Grape Juice Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39155);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39156 (Throw Random Bottle ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39156);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39171 (Mortal Strike ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39171);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39174 (Cleave ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39174);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39176 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39176);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39182 (Serpent Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39182);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39210 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39210);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39212 (Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39212);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39262 (Mana Burn ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39262);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39323 (Holy Fire ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39323);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39367 (Seed of Corruption ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39367);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39413 (Viper Sting ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39413);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39415 (Fear ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39415);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 39416 (Blizzard ) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(39416);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26987 (Moonfire Rank 11) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26987);
	if( sp != NULL )
		sp->in_front_status = 1;

	// Spell 26988 (Moonfire Rank 12) is infront from spell extra.
	sp = dbcSpell.LookupEntryForced(26988);
	if( sp != NULL )
		sp->in_front_status = 1;

		//////////////////////////////////////////////////////////////////
		//AREA AURA TARGETS - START
		//////////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////////////////////////////
		//AREA AURA TARGETS - END
		//////////////////////////////////////////////////////////////////
}

void Apply112SpellFixes()
{
	// 
}

void ApplySpellUniques()
{
	QueryResult* result = WorldDatabase.Query("select `SpellId`, `AuraGroup`, `AuraGroup2`, `UniqueType` from `spell_unique_auras`;");

	if (result == NULL)
		return;

	do
	{
		Field* fields = result->Fetch();
		SpellEntry* sp = dbcSpell.LookupEntryForced(fields[0].GetUInt32());
		
		if (sp != NULL)
		{
			sp->UniqueGroup = fields[1].GetUInt16();
			sp->UniqueGroup2 = fields[2].GetUInt16();
			sp->UniqueTargetBased = fields[3].GetUInt8();
		}
	}
	while(result->NextRow());

	delete result;
}

void ApplyNormalFixes()
{
	//Updating spell.dbc--this is slow like hell due to we cant read string fields
	//dbc method will be changed in future
	DBCFile dbc;
	uint32 aura;
	
    char _bufftemp_[1024];
	sprintf(_bufftemp_,"%s/Spell.dbc",DBCPath);
	if( !dbc.open( _bufftemp_ ) )
	{
		Log.Error("World", "Cannot find file %s",_bufftemp_ );
		return;
	}
    Log.Notice("DBC:","Loading %s",_bufftemp_);
	Log.Notice("World", "Processing %u spells...", dbc.getRecordCount());
	//Apply112SpellFixes(); // Plus necessaire (Branruz)

	uint32 cnt = (uint32)dbc.getRecordCount();
	uint32 effect;
	uint32 All_Seal_Groups_Combined=0;

	map<uint32, uint32> talentSpells;
	map<uint32,uint32>::iterator talentSpellIterator;
	unsigned int i,j;
	for(i = 0; i < dbcTalent.GetNumRows(); ++i)
	{
		TalentEntry * tal = dbcTalent.LookupRow(i);
		for(j = 0; j < 5; ++j)
			if(tal->RankID[j] != 0)
				talentSpells.insert(make_pair(tal->RankID[j], tal->TalentTree));
	}

	//log spell errors to a file
	FILE* fspellerrors = fopen("SpellErrors.txt", "w");

	for(uint32 x=0; x < cnt; x++)
	{
		// SpellID
		uint32 spellid = dbc.getRecord(x).getInt(0);

		// get spellentry
		SpellEntry * sp = dbcSpell.LookupEntry(spellid);

		if(!sp)
		{   // A ce stade, autant tout laisser se planter, ca changera rien (Branruz)
			Log.Error("ApplyNormalFixes","FATAL: Leak Memory ou Memory System Overflow");
			Log.Error("                ","=======> Server will go crashed <===========");
		}

	    // Rangement des SpellGroup en separant le HighSpellGroupType du SpellGroupType (Branruz)
		for(uint32 i=0;i<MAX_SPELLGROUPTYPE;i++)  
		{
			sp->ExtendedSpellGroupType[i] = (sp->SpellGroupType[i] & 0xFFFF0000) >> 16;
            sp->SpellGroupType[i]         = (sp->SpellGroupType[i] & 0x0000FFFF)      ;
		}
	    //----
		sp->UniqueGroup = 0;
		sp->UniqueGroup2 = 0;
		sp->UniqueTargetBased = 0;
		sp->apply_on_shapeshift_change = false;
		sp->always_apply = false;
		sp->spell_signature = _SPELL_SIGNATURE_;
		uint32 result = 0;

		// Description field
		char* desc = sp->Description;
		const char* ranktext = sp->Rank;
		const char* nametext = sp->Name;

		uint32 rank = 0;
		uint32 type = 0;
		uint32 namehash = 0;

		//test if its a null spell effect
		for (uint8 i=0; i<3; ++i)
		{
			if (sp->Effect[i] == SPELL_EFFECT_NULL) continue;

			if (sp->Effect[i] < TOTAL_SPELL_EFFECTS) // Test s'il reste des SpellEffects a coder (Tous)
			{
				if (SpellEffectsHandler[sp->Effect[i]] == &Spell::SpellEffectNULL)
				{
					fprintf(fspellerrors, "\nSpell %u (%s): Unhandled SpellEffect %u on index %u", sp->Id, sp->Name, sp->Effect[i], i);
				}
			}
			else
			{
				fprintf(fspellerrors, "\nSpell %u (%s): Out of range SpellEffect %u on index %u", sp->Id, sp->Name, sp->Effect[i], i);
			}

			//do auras too!
			if (sp->EffectApplyAuraName[i] == SPELL_AURA_NONE) continue;

			if (sp->EffectApplyAuraName[i] < TOTAL_SPELL_AURAS) // Test s'il reste des SpellAuras a coder (Tous)
			{
				if (SpellAuraHandler[sp->EffectApplyAuraName[i]] == &Aura::SpellAuraNULL)
				{
					fprintf(fspellerrors, "\nSpell %u (%s): Unhandled SpellAura %u on index %u", sp->Id, sp->Name, sp->EffectApplyAuraName[i], i);
				}
			}
			else
			{
				fprintf(fspellerrors, "\nSpell %u (%s): Out of range SpellAura %u on index %u", sp->Id, sp->Name, sp->EffectApplyAuraName[i], i);
			}
		}

		// hash the name
		//!!!!!!! representing all strings on 32 bits is dangerous. There is a chance to get same hash for a lot of strings ;)
		//-- Adaptation Fr (Branruz)
		/*
        for(j=0;j<strlen(nametext);j++)
        {
         if(*(nametext+j) == ' ') *(nametext+j))='_';
        }*/
		//----------------
        namehash = crc32((const unsigned char*)nametext, (unsigned int)strlen(nametext));
		sp->NameHash   = namehash; //need these set before we start processing spells

		float radius=std::max(::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[0])),::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[1])));
		radius=std::max(::GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[2])),radius);
		radius=std::max(GetMaxRange(dbcSpellRange.LookupEntry(sp->rangeIndex)),radius);
		sp->base_range_or_radius_sqr = radius*radius;

		// NEW SCHOOLS AS OF 2.4.0:
		/* (bitwise)
		SCHOOL_NORMAL = 1,
		SCHOOL_HOLY   = 2,
		SCHOOL_FIRE   = 4,
		SCHOOL_NATURE = 8,
		SCHOOL_FROST  = 16,
		SCHOOL_SHADOW = 32,
		SCHOOL_ARCANE = 64
		*/

#define SET_SCHOOL(x) sp->School = x
		     if(!sp->schoolMask)                     SET_SCHOOL(SCHOOL_NORMAL); // Test (Branruz)
	    else if(sp->schoolMask & SCHOOL_MASK_NORMAL) SET_SCHOOL(SCHOOL_NORMAL);
		else if(sp->schoolMask & SCHOOL_MASK_HOLY)   SET_SCHOOL(SCHOOL_HOLY);
		else if(sp->schoolMask & SCHOOL_MASK_FIRE)   SET_SCHOOL(SCHOOL_FIRE);
		else if(sp->schoolMask & SCHOOL_MASK_NATURE) SET_SCHOOL(SCHOOL_NATURE);
		else if(sp->schoolMask & SCHOOL_MASK_FROST)  SET_SCHOOL(SCHOOL_FROST);
		else if(sp->schoolMask & SCHOOL_MASK_SHADOW) SET_SCHOOL(SCHOOL_SHADOW);
		else if(sp->schoolMask & SCHOOL_MASK_ARCANE) SET_SCHOOL(SCHOOL_ARCANE);
		else
			Log.Error("SpellFixes", "Spell %u has unknown school %u", sp->Id, sp->schoolMask);
#undef SET_SCHOOL

		/*
		AURASTATE_FLAG_DODGE_BLOCK			= 1,        //1
		AURASTATE_FLAG_HEALTH20             = 2,        //2
		AURASTATE_FLAG_BERSERK              = 4,        //3
		AURASTATE_FLAG_JUDGEMENT            = 16,       //5
		AURASTATE_FLAG_PARRY                = 64,       //7
		AURASTATE_FLAG_LASTKILLWITHHONOR    = 512,      //10
		AURASTATE_FLAG_CRITICAL             = 1024,     //11
		AURASTATE_FLAG_HEALTH35             = 4096,     //13
		AURASTATE_FLAG_IMMOLATE             = 8192,     //14
		AURASTATE_FLAG_REJUVENATE           = 16384,    //15 //where do i use this ?
		AURASTATE_FLAG_POISON               = 32768,    //16
		*/

		// correct caster aura state
		if( sp->casterAuraState != 0 )
		{
			switch( sp->casterAuraState )
			{
			case 1:
				sp->casterAuraState = AURASTATE_FLAG_DODGE_BLOCK;
				break;

			case 2:
				sp->casterAuraState = AURASTATE_FLAG_HEALTH20;
				break;

			case 3:
				sp->casterAuraState = AURASTATE_FLAG_BERSERK;
				break;

			case 4:
				sp->casterAuraState = AURASTATE_FLAG_FROZEN;
				break;

			case 5:
				sp->casterAuraState = AURASTATE_FLAG_JUDGEMENT;
				break;

			case 7:
				sp->casterAuraState = AURASTATE_FLAG_PARRY;
				break;

			case 10:
				sp->casterAuraState = AURASTATE_FLAG_LASTKILLWITHHONOR;
				break;

			case 11:
				sp->casterAuraState = AURASTATE_FLAG_CRITICAL;
				break;

			case 13:
				sp->casterAuraState = AURASTATE_FLAG_HEALTH35;
				break;

			case 14:
				sp->casterAuraState = AURASTATE_FLAG_IMMOLATE;
				break;

			case 15:
				sp->casterAuraState = AURASTATE_FLAG_REJUVENATE;
				break;

			case 16:
				sp->casterAuraState = AURASTATE_FLAG_POISON;
				break;

			case 17:
				sp->casterAuraState = AURASTATE_FLAG_ENRAGE;
				break;

			case 23:
				sp->casterAuraState = AURASTATE_FLAG_HEALTHABOVE75;
				break;

			default:
				Log.Error("AuraState", "Spell %u (%s) has unknown caster aura state %u\n", sp->Id, sp->Name, sp->casterAuraState);
				sp->casterAuraState = 0;
				break;
			}
		}

		if( sp->targetAuraState != 0 )
		{
			switch( sp->targetAuraState )
			{
			case 1:
				sp->targetAuraState = AURASTATE_FLAG_DODGE_BLOCK;
				break;

			case 2:
				sp->targetAuraState = AURASTATE_FLAG_HEALTH20;
				break;

			case 3:
				sp->targetAuraState = AURASTATE_FLAG_BERSERK;
				break;

			case 4:
				sp->targetAuraState = AURASTATE_FLAG_FROZEN;
				break;

			case 5:
				sp->targetAuraState = AURASTATE_FLAG_JUDGEMENT;
				break;

			case 7:
				sp->targetAuraState = AURASTATE_FLAG_PARRY;
				break;

			case 10:
				sp->targetAuraState = AURASTATE_FLAG_LASTKILLWITHHONOR;
				break;

			case 11:
				sp->targetAuraState = AURASTATE_FLAG_CRITICAL;
				break;

			case 13:
				sp->targetAuraState = AURASTATE_FLAG_HEALTH35;
				break;

			case 14:
				sp->targetAuraState = AURASTATE_FLAG_IMMOLATE;
				break;

			case 15:
				sp->targetAuraState = AURASTATE_FLAG_REJUVENATE;
				break;

			case 16:
				sp->targetAuraState = AURASTATE_FLAG_POISON;
				break;

			case 17:
				sp->targetAuraState = AURASTATE_FLAG_ENRAGE;
				break;

			case 23:
				sp->targetAuraState = AURASTATE_FLAG_HEALTHABOVE75;
				break;

			default:
				Log.Error("AuraState", "Spell %u (%s) has unknown target aura state %u\n", sp->Id, sp->Name, sp->targetAuraState);
				sp->targetAuraState = 0;
				break;
			}
		}

		// apply on shapeshift change
		if( sp->NameHash == SPELL_HASH_TRACK_HUMANOIDS )
			sp->apply_on_shapeshift_change = true;

		if( sp->NameHash == SPELL_HASH_BLOOD_FURY || sp->NameHash == SPELL_HASH_SHADOWSTEP )
			sp->always_apply = true;

		
		for(uint32 b=0;b<3;++b)
		{
			if(sp->EffectTriggerSpell[b] != 0 && dbcSpell.LookupEntryForced(sp->EffectTriggerSpell[b]) == NULL)
			{
				/* proc spell referencing non-existant spell. create a dummy spell for use w/ it. */
				CreateDummySpell(sp->EffectTriggerSpell[b]);
			}
			/** Load teaching spells (used for hunters when learning pets wild abilities) */
			if(sp->Effect[b]==SPELL_EFFECT_LEARN_SPELL && sp->EffectImplicitTargetA[b]==EFF_TARGET_PET)
			{
				std::map<uint32,uint32>::iterator itr = sWorld.TeachingSpellMap.find(sp->EffectTriggerSpell[b]);
				if(itr == sWorld.TeachingSpellMap.end())
					sWorld.TeachingSpellMap.insert(make_pair(sp->EffectTriggerSpell[b],sp->Id));
			}

			if( sp->attributes & ATTRIBUTES_ONLY_OUTDOORS && sp->EffectApplyAuraName[b] == SPELL_AURA_MOUNTED )
			{
				sp->attributes &= ~ATTRIBUTES_ONLY_OUTDOORS;
			}
		}

		/*if(sp->self_cast_only && !(sp->attributes&64))
			printf("SPELL SELF CAST ONLY: %s %u\n", sp->Name, sp->Id);*/

		if(!strcmp(sp->Name, "Hearthstone") || !strcmp(sp->Name, "Stuck") || !strcmp(sp->Name, "Astral Recall"))
		{
			for (uint32 i=0; i<3; ++i)
			{
				sp->EffectImplicitTargetA[i]=EFF_TARGET_SELF;
				sp->EffectImplicitTargetB[i]=EFF_TARGET_NONE;
			}
		}
		
		sp->proc_interval         = 0; //trigger at each event
		sp->c_is_flags            = 0;
		sp->spell_coef_flags      = 0;
		sp->Dspell_coef_override  = -1;
		sp->OTspell_coef_override = -1;
		sp->casttime_coef         = 0;
		sp->fixed_dddhcoef        = -1;
		sp->fixed_hotdotcoef      = -1;

		talentSpellIterator = talentSpells.find(sp->Id);
		if(talentSpellIterator == talentSpells.end())
			sp->talent_tree = 0;
		else
			sp->talent_tree = talentSpellIterator->second;

		// parse rank text
		if( !sscanf( ranktext, "Rank %d", (unsigned int*)&rank) )
			rank = 0;

	    if( strstr( desc, "Battle Elixir"))                   type |= SPELL_TYPE_ELIXIR_BATTLE;
        else if( strstr( desc, "Guardian Elixir"))            type |= SPELL_TYPE_ELIXIR_GUARDIAN;
        else if( strstr( desc, "Battle and Guardian elixir")) type |= SPELL_TYPE_ELIXIR_FLASK;
		else if( strstr( desc, "Finishing move")==desc)         
			sp->c_is_flags |= SPELL_FLAG_IS_FINISHING_MOVE;

		if( IsDamagingSpell( sp ) )       sp->c_is_flags |= SPELL_FLAG_IS_DAMAGING;
		if( IsHealingSpell( sp ) )        sp->c_is_flags |= SPELL_FLAG_IS_HEALING;
		if( IsTargetingStealthed( sp ) )  sp->c_is_flags |= SPELL_FLAG_IS_TARGETINGSTEALTHED;
		

		//stupid spell ranking problem, A voir (Branruz)
		if(sp->spellLevel==0)
		{
			uint32 new_level=0;
			if( strstr( nametext, "Apprentice "))      new_level = 1;
			else if( strstr( nametext, "Journeyman ")) new_level = 2;
			else if( strstr( nametext, "Expert "))     new_level = 3;
			else if( strstr( nametext, "Artisan "))    new_level = 4;
			else if( strstr( nametext, "Master "))     new_level = 5;
			if(new_level!=0)
			{
				uint32 teachspell=0;
				if(sp->Effect[0]==SPELL_EFFECT_LEARN_SPELL) 		teachspell = sp->EffectTriggerSpell[0];
				else if(sp->Effect[1]==SPELL_EFFECT_LEARN_SPELL)    teachspell = sp->EffectTriggerSpell[1];
				else if(sp->Effect[2]==SPELL_EFFECT_LEARN_SPELL)    teachspell = sp->EffectTriggerSpell[2];

				if(teachspell)
				{
					SpellEntry *spellInfo;
					spellInfo = dbcSpell.LookupEntryForced(teachspell);
					spellInfo->spellLevel = new_level;
					sp->spellLevel = new_level;
				}
			}
		}

		/*FILE * f = fopen("C:\\spells.txt", "a");
		fprintf(f, "case 0x%08X:		// %s\n", namehash, nametext);
		fclose(f);*/

		// find diminishing status
		sp->DiminishStatus = GetDiminishingGroup(namehash);

		// HACK FIX: Break roots/fear on damage.. this needs to be fixed properly!
		
	
		if(!(sp->auraInterruptFlags & AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN))
		{
			for(uint32 z = 0; z < 3; ++z) {
				if(sp->EffectApplyAuraName[z] == SPELL_AURA_MOD_FEAR ||
					sp->EffectApplyAuraName[z] == SPELL_AURA_MOD_ROOT)
				{
					//sp->auraInterruptFlags |= AURA_INTERRUPT_ON_UNUSED2; // En commentaire : Recodage des Spells, plus d'override sur le dbc (Branruz)
					break;
				}

				if( ( sp->Effect[z] == SPELL_EFFECT_SCHOOL_DAMAGE && sp->defenseType == DEFENSE_TYPE_MELEE ) || sp->Effect[z] == SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL || sp->Effect[z] == SPELL_EFFECT_WEAPON_DAMAGE || sp->Effect[z] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE || sp->Effect[z] == SPELL_EFFECT_DUMMYMELEE )
					sp->is_melee_spell = true;
				if( ( sp->Effect[z] == SPELL_EFFECT_SCHOOL_DAMAGE && sp->defenseType == DEFENSE_TYPE_RANGED ) )
				{
					//Log.Notice( "SpellFixes" , "Ranged Spell: %u [%s]" , sp->Id , sp->Name );
					sp->is_ranged_spell = true;
				}
			}
		}

		// set extra properties
		sp->buffType   = type;
		sp->RankNumber = rank;

		uint32 pr=sp->procFlags;


		for(uint32 y=0;y < 3; y++)
		{
			// get the effect number from the spell
			effect = sp->Effect[y];
			//effect = dbc.getRecord(x).getUInt(_SPELL_EFFECT_DBC_COL1_ + y); // 3.3.2.11403 (Branruz)
			
			if(effect==SPELL_EFFECT_APPLY_AURA) // || SPELL_EFFECT_APPLY_AREA_AURA)
			{
			 //aura = dbc.getRecord(x).getUInt(_SPELL_EFFECTAPPLYAURA_DBC_COL1_+y); // (Branruz)
			 aura = sp->EffectApplyAuraName[y];

			 if( aura == SPELL_AURA_PROC_TRIGGER_SPELL || aura == SPELL_AURA_PROC_TRIGGER_DAMAGE )//search for spellid in description
			 {
					const char *p=desc;
					while((p=strstr(p,"$"))) // hmmm A voir (Branruz), 
					{          // le spellId serait codé dans le texte de description, 
						       // juste apres le 1ier '$' trouvé
						       // ...stunt for $4562..." par exemple
						       // Le SpellParser peut etre utilisé si on sait ce qu'on cherche
						p++;
						//got $  -> check if spell
						if(*p>='0' && *p <='9')
						{//woot this is spell id
						
							result=atoi(p);
						}					
					}
					pr=0;

					uint32 len = (uint32)strlen(desc);
					for(i = 0; i < len; ++i)
						desc[i] = tolower(desc[i]);
					//dirty code for procs, if any1 got any better idea-> u are welcome
					//139944 --- some magic number, it will trigger on all hits etc
						//for seems to be smth like custom check

			 }//end "if procspellaura"
			}//end "if aura"
		}//end for(uint32 y=0;y < 3; y++)

		sp->procFlags = pr;

		if( strstr( desc, "Must remain seated"))
		{
			sp->recoveryTime = 1000;
			sp->categoryRecoveryTime = 1000;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// procintervals
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//omg lighning shield trigger spell id's are all wrong ?
		//if you are bored you could make thiese by hand but i guess we might find other spells with this problem..and this way it's safe
	
//junk code to get me has :P 
//if(sp->Id==11267 || sp->Id==11289 || sp->Id==6409)
//	printf("!!!!!!! name %s , id %u , hash %u \n",nametext,sp->Id, namehash);
	} // End for(uint32 x=0; x < cnt; x++)


	/////////////////////////////////////////////////////////////////
	//SPELL COEFFICIENT SETTINGS START
	//////////////////////////////////////////////////////////////////

	for(uint32 x=0; x < cnt; x++)
	{
		// SpellID
		uint32 spellid = dbc.getRecord(x).getUInt(0);
		// get spellentry
		SpellEntry * sp = dbcSpell.LookupEntry(spellid);

		//Setting Cast Time Coefficient
		SpellCastTime *sd = dbcSpellCastTime.LookupEntry(sp->castingTimeIndex);
		float castaff = float(GetCastTime(sd));
		if(castaff < 1500) castaff = 1500;
		else
			if(castaff > 7000) castaff = 7000;

		sp->casttime_coef = castaff / 3500;		 

		SpellEntry * spz;
		bool spcheck = false;

		//Flag for DoT and HoT
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if (sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_DAMAGE ||
				sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_HEAL ||
				sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_LEECH )
			{
				sp->spell_coef_flags |= SPELL_FLAG_IS_DOT_OR_HOT_SPELL;
				break;
			}
		}

		//Flag for DD or DH
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if ( sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_TRIGGER_SPELL && sp->EffectTriggerSpell[i] )
			{
				spz = dbcSpell.LookupEntry( sp->EffectTriggerSpell[i] );
				if( spz &&
					(spz->Effect[i] == SPELL_EFFECT_SCHOOL_DAMAGE ||
					spz->Effect[i] == SPELL_EFFECT_HEAL
					)) 
					spcheck = true;
			}
			if (sp->Effect[i] == SPELL_EFFECT_SCHOOL_DAMAGE ||
				sp->Effect[i] == SPELL_EFFECT_HEAL ||
				spcheck
				)
			{
				sp->spell_coef_flags |= SPELL_FLAG_IS_DD_OR_DH_SPELL;
				break;
			}
		}

		for(uint8 i = 0 ; i < 3; i++)
		{
			switch (sp->EffectImplicitTargetA[i])
			{
				//AoE
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS:
			case EFF_TARGET_ALL_ENEMY_IN_AREA:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT:
			case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
			case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
			case EFF_TARGET_IN_FRONT_OF_CASTER:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_PARTY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_FRIENDLY_IN_AREA:
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS_OVER_TIME:
			case EFF_TARGET_ALL_PARTY:
			case EFF_TARGET_BEHIND_TARGET_LOCATION:
			case EFF_TARGET_LOCATION_INFRONT_CASTER_AT_RANGE:
				{
					sp->spell_coef_flags |= SPELL_FLAG_AOE_SPELL;
					break;
				}
			}	
		}

		for(uint8 i = 0 ; i < 3 ; i++)
		{
			switch (sp->EffectImplicitTargetB[i])
			{
				//AoE
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS:
			case EFF_TARGET_ALL_ENEMY_IN_AREA:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT:
			case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
			case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
			case EFF_TARGET_IN_FRONT_OF_CASTER:
			case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_PARTY_IN_AREA_CHANNELED:
			case EFF_TARGET_ALL_FRIENDLY_IN_AREA:
			case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS_OVER_TIME:
			case EFF_TARGET_ALL_PARTY:
			case EFF_TARGET_BEHIND_TARGET_LOCATION:
			case EFF_TARGET_LOCATION_INFRONT_CASTER_AT_RANGE:
				{
					sp->spell_coef_flags |= SPELL_FLAG_AOE_SPELL;
					break;
				}
			}	
		}

		//Special Cases
		//Holy Light & Flash of Light
		if(sp->NameHash == SPELL_HASH_HOLY_LIGHT ||
			sp->NameHash == SPELL_HASH_FLASH_OF_LIGHT)
			sp->spell_coef_flags |= SPELL_FLAG_IS_DD_OR_DH_SPELL;

		//Additional Effect (not healing or damaging)
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if(sp->Effect[i] == SPELL_EFFECT_NULL) continue;

			switch (sp->Effect[i])
			{
			 case SPELL_EFFECT_SCHOOL_DAMAGE:
			 case SPELL_EFFECT_ENVIRONMENTAL_DAMAGE:
			 case SPELL_EFFECT_HEALTH_LEECH:
			 case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL:
			 case SPELL_EFFECT_ADD_EXTRA_ATTACKS:
			 case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE:
			 case SPELL_EFFECT_POWER_BURN:
			 case SPELL_EFFECT_ATTACK:
			 case SPELL_EFFECT_HEAL:
			 case SPELL_EFFECT_HEAL_MAX_HEALTH:
			 case SPELL_EFFECT_DUMMY:
				continue;
			}

			switch (sp->EffectApplyAuraName[i])
			{
			 case SPELL_AURA_PERIODIC_DAMAGE:
			 case SPELL_AURA_PROC_TRIGGER_DAMAGE:
			 case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
			 case SPELL_AURA_POWER_BURN:
			 case SPELL_AURA_PERIODIC_HEAL:
			 case SPELL_AURA_MOD_INCREASE_HEALTH:
			 case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
			 case SPELL_AURA_DUMMY:
				continue;
			}

			sp->spell_coef_flags |= SPELL_FLAG_ADITIONAL_EFFECT;
			break;

		}

		//Calculating fixed coeficients
		//Channeled spells
		if( sp->channelInterruptFlags != 0 )
		{
			float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->durationIndex ),INVOC_DURATION ));
			if(Duration < 1500) Duration = 1500;
			else if(Duration > 7000) Duration = 7000;
			sp->fixed_hotdotcoef = (Duration / 3500.0f);

			if( sp->spell_coef_flags & SPELL_FLAG_ADITIONAL_EFFECT )
				sp->fixed_hotdotcoef *= 0.95f;
			if( sp->spell_coef_flags & SPELL_FLAG_AOE_SPELL )
				sp->fixed_hotdotcoef *= 0.5f;
		}

		//Standard spells
		else if( (sp->spell_coef_flags & SPELL_FLAG_IS_DD_OR_DH_SPELL) && !(sp->spell_coef_flags & SPELL_FLAG_IS_DOT_OR_HOT_SPELL) )
		{
			sp->fixed_dddhcoef = sp->casttime_coef;
			if( sp->spell_coef_flags & SPELL_FLAG_ADITIONAL_EFFECT )
				sp->fixed_dddhcoef *= 0.95f;
			if( sp->spell_coef_flags & SPELL_FLAG_AOE_SPELL )
				sp->fixed_dddhcoef *= 0.5f;
		}

		//Over-time spells
		else if( !(sp->spell_coef_flags & SPELL_FLAG_IS_DD_OR_DH_SPELL) && (sp->spell_coef_flags & SPELL_FLAG_IS_DOT_OR_HOT_SPELL) )
		{
			float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->durationIndex ),INVOC_DURATION ));
			sp->fixed_hotdotcoef = (Duration / 15000.0f);

			if( sp->spell_coef_flags & SPELL_FLAG_ADITIONAL_EFFECT )
				sp->fixed_hotdotcoef *= 0.95f;
			if( sp->spell_coef_flags & SPELL_FLAG_AOE_SPELL )
				sp->fixed_hotdotcoef *= 0.5f;

		}

		//Combined standard and over-time spells
		else if( sp->spell_coef_flags & SPELL_FLAG_IS_DD_DH_DOT_SPELL )
		{
			float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->durationIndex ),INVOC_DURATION ));
			float Portion_to_Over_Time = (Duration / 15000.0f) / ((Duration / 15000.0f) + sp->casttime_coef );
			float Portion_to_Standard = 1.0f - Portion_to_Over_Time;

			sp->fixed_dddhcoef = sp->casttime_coef * Portion_to_Standard;
			sp->fixed_hotdotcoef = (Duration / 15000.0f) * Portion_to_Over_Time;

			if( sp->spell_coef_flags & SPELL_FLAG_ADITIONAL_EFFECT )
			{
				sp->fixed_dddhcoef *= 0.95f;
				sp->fixed_hotdotcoef *= 0.95f;
			}
			if( sp->spell_coef_flags & SPELL_FLAG_AOE_SPELL )
			{
				sp->fixed_dddhcoef *= 0.5f;
				sp->fixed_hotdotcoef *= 0.5f;
			}		
		}

		///	SPELLS CAN CRIT ///
		sp->spell_can_crit = true; // - except in special cases noted in this section
 	} // End for(uint32 x=0; x < cnt; x++)
     

	//Settings for special cases
	QueryResult * resultx = WorldDatabase.Query("SELECT * FROM spell_coef_override");
	if( resultx != NULL )
	{
		do 
		{
			Field * f;
			f = resultx->Fetch();
			SpellEntry * sp = dbcSpell.LookupEntryForced( f[0].GetUInt32() );
			if( sp != NULL )
			{
				sp->Dspell_coef_override = f[2].GetFloat();
				sp->OTspell_coef_override = f[3].GetFloat();
			}
			else
				Log.Warning("SpellCoefOverride", "Has nonexistant spell %u.", f[0].GetUInt32());
		} while( resultx->NextRow() );
		delete resultx;
	}

	//Fully loaded coefficients, we must share channeled coefficient to its triggered spells
	for(uint32 x=0; x < cnt; x++)
	{
		// SpellID
		uint32 spellid = dbc.getRecord(x).getUInt(0);
		// get spellentry
		SpellEntry * sp = dbcSpell.LookupEntry(spellid);
		SpellEntry * spz;

		//Case SPELL_AURA_PERIODIC_TRIGGER_SPELL
		for( uint8 i = 0 ; i < 3 ; i++ )
		{
			if ( sp->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_TRIGGER_SPELL )
			{
				spz = dbcSpell.LookupEntry( sp->EffectTriggerSpell[i] );
				if( spz != NULL ) 
				{
					if( sp->Dspell_coef_override >= 0 )
						spz->Dspell_coef_override = sp->Dspell_coef_override;
					else
					{
						//we must set bonus per tick on triggered spells now (i.e. Arcane Missiles)
						if( sp->channelInterruptFlags != 0 )
						{
							float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->durationIndex ),INVOC_DURATION ));
							float amp = float(sp->EffectAmplitude[i]);
							sp->fixed_dddhcoef = sp->fixed_hotdotcoef * amp / Duration;
						}			
						spz->fixed_dddhcoef = sp->fixed_dddhcoef;
					}

					if( sp->OTspell_coef_override >= 0 )
						spz->OTspell_coef_override = sp->OTspell_coef_override;
					else
					{
						//we must set bonus per tick on triggered spells now (i.e. Arcane Missiles)
						if( sp->channelInterruptFlags != 0 )
						{
							float Duration = float( GetDuration( dbcSpellDuration.LookupEntry( sp->durationIndex ),INVOC_DURATION ));
							float amp = float(sp->EffectAmplitude[i]);
							sp->fixed_hotdotcoef *= amp / Duration;
						}
						spz->fixed_hotdotcoef = sp->fixed_hotdotcoef;
					}
					break;
				}
			}
		}
	}	

	ApplySpellUniques();

	fclose(fspellerrors);
}
