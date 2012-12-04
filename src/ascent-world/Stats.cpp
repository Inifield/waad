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
#define PLAYER_LEVEL_CAP_MAX 250 // MAX MAX MAX (Player Level codé sur un octet)

// Modifié pour integrer le fait qu'un player peut atteindre le niveau 250 (Branruz)
// Au dela du niveau 70, tout n'est que pur spéculation, 
// la modif rend juste le code homogene, rien de plus!

uint32 getConColor(uint16 AttackerLvl, uint16 VictimLvl)
{

	const uint32 grayLevel[PLAYER_LEVEL_CAP_MAX+1] = 
	{ 0  ,0  ,0  ,0  ,0  ,0  ,0  ,1  ,2  ,3 , // lv10 
	  4  ,5  ,6  ,7  ,8  ,9  ,10 ,11 ,12 ,13, // lv20
	  13 ,14 ,15 ,16 ,17 ,18 ,19 ,20 ,21 ,22, // lv30
	  22 ,23 ,24 ,25 ,26 ,27 ,28 ,29 ,30 ,31, // lv40
	  31 ,32 ,33 ,34 ,35 ,35 ,36 ,37 ,38 ,39, // lv50
	  39 ,40 ,41 ,42 ,43 ,43 ,44 ,45 ,46 ,47, // lv60
	  47 ,48 ,49 ,50 ,51 ,51 ,52 ,53 ,54 ,55, // lv70
	  55 ,56 ,57 ,58 ,59 ,60 ,61 ,62 ,63 ,64, // lv80
	  64 ,65 ,66 ,67 ,68 ,69 ,70 ,71 ,72 ,73, // lv90
	  73 ,74 ,75 ,76 ,77 ,78 ,79 ,80 ,81 ,82, // lv100
	  82 ,83 ,84 ,85 ,86 ,87 ,88 ,89 ,90 ,91, // lv110
	  91 ,92 ,93 ,94 ,95 ,96 ,97 ,98 ,99 ,100, // lv120
	  100,101,102,103,104,105,106,107,108,109, // lv130
	  109,110,111,112,113,114,115,116,117,118, // lv140
	  118,119,120,121,122,123,124,125,126,127, // lv150
	  127,128,129,130,131,132,133,134,135,136, // lv160
	  136,137,138,139,140,141,142,143,144,145, // lv170
	  145,146,147,148,149,150,151,152,153,154, // lv180
	  154,155,156,157,158,159,160,161,162,163, // lv190
	  163,164,165,166,167,168,169,170,171,172, // lv200
	  172,173,174,175,176,177,178,179,180,181, // lv210
	  181,192,193,184,185,186,187,188,189,190, // lv220
	  190,191,192,193,194,195,196,197,198,199, // lv230
	  199,200,201,202,203,204,205,206,207,208, // lv240
	  208,209,210,211,212,213,214,215,216,217,217  // lv250
	  };


	if((AttackerLvl + 5) <= VictimLvl)
	{
		if((AttackerLvl + 10) <= VictimLvl)
		{
			return 5;
		}
		return 4;
	}
	else
	{
		switch(VictimLvl - AttackerLvl)
		{
		case 4:
		case 3: return 3; break;
		case 2:
		case 1:
		case 0:
		case -1:
		case -2: return 2; break;
		default:
			// More adv formula for grey/green lvls:
			if(AttackerLvl <= 6)
			{
				return 1; //All others are green.
			}
			else
			{
			 if(AttackerLvl > PLAYER_LEVEL_CAP_MAX) return 1; //gm

			 if((AttackerLvl < PLAYER_LEVEL_CAP_MAX) && (VictimLvl <= grayLevel[AttackerLvl])) return 0;
			 else                                                                            return 1;
			}
		}
	}
}

uint32 CalculateXpToGive(Unit *pVictim, Unit *pAttacker)
{
	if(pVictim->IsPlayer()) return 0;

	if(((Creature*)pVictim)->IsTotem()) return 0;

	CreatureInfo *victimI;
	victimI = ((Creature*)pVictim)->GetCreatureName();

	if(victimI)
		if(victimI->Type == CRITTER)
			return 0;

	uint32 VictimLvl = pVictim->getLevel();
	uint32 AttackerLvl = pAttacker->getLevel();

	if( pAttacker->IsPet() && static_cast< Pet* >( pAttacker )->GetPetOwner() )
	{
		// based on: http://www.wowwiki.com/Talk:Formulas:Mob_XP#Hunter.27s_pet_XP (2008/01/12)
		uint32 ownerLvl = static_cast< Pet* >( pAttacker )->GetPetOwner()->getLevel();
		VictimLvl += ownerLvl - AttackerLvl;
		AttackerLvl = ownerLvl;
	}
	else if( (int32)VictimLvl - (int32)AttackerLvl > 10 ) //not wowwikilike but more balanced
		return 0;

	float zd = 5;
	float g = 5;

	// get zero diff - get grey diff
	if(AttackerLvl >= PLAYER_LEVEL_CAP_MAX) { zd = 31; g = 51; }
	else if(AttackerLvl >= 245 ) { zd = 30; g = 50; }
	else if(AttackerLvl >= 240 ) { zd = 29; g = 49; }
	else if(AttackerLvl >= 235 ) { zd = 28; g = 48; }
	else if(AttackerLvl >= 230 ) { zd = 27; g = 47; }
	else if(AttackerLvl >= 225 ) { zd = 26; g = 46; }
	else if(AttackerLvl >= 220 ) { zd = 25; g = 45; }
	else if(AttackerLvl >= 215 ) { zd = 48; g = 44; }
	else if(AttackerLvl >= 210 ) { zd = 47; g = 43; }
	else if(AttackerLvl >= 205 ) { zd = 46; g = 42; }
	else if(AttackerLvl >= 200 ) { zd = 45; g = 41; }
	else if(AttackerLvl >= 195 ) { zd = 44; g = 40; }
	else if(AttackerLvl >= 190 ) { zd = 43; g = 39; }
	else if(AttackerLvl >= 185 ) { zd = 42; g = 38; }
	else if(AttackerLvl >= 180 ) { zd = 41; g = 37; }
	else if(AttackerLvl >= 175 ) { zd = 40; g = 36; }
	else if(AttackerLvl >= 170 ) { zd = 36; g = 35; }
	else if(AttackerLvl >= 165 ) { zd = 38; g = 34; }
	else if(AttackerLvl >= 160 ) { zd = 37; g = 33; }
	else if(AttackerLvl >= 155 ) { zd = 36; g = 32; }
	else if(AttackerLvl >= 150 ) { zd = 35; g = 31; }
	else if(AttackerLvl >= 145 ) { zd = 34; g = 30; }
	else if(AttackerLvl >= 140)  { zd = 33; g = 29; }
	else if(AttackerLvl >= 135 ) { zd = 32; g = 28; }
	else if(AttackerLvl >= 130 ) { zd = 31; g = 27; }
	else if(AttackerLvl >= 125 ) { zd = 30; g = 26; }
	else if(AttackerLvl >= 120 ) { zd = 29; g = 25; }
	else if(AttackerLvl >= 115 ) { zd = 28; g = 24; }
	else if(AttackerLvl >= 110 ) { zd = 27; g = 23; }
	else if(AttackerLvl >= 105 ) { zd = 26; g = 22; }
	else if(AttackerLvl >= 100) { zd = 25; g = 21; }
	else if(AttackerLvl >= 95 ) { zd = 24; g = 20; }
	else if(AttackerLvl >= 90 ) { zd = 23; g = 19; }
	else if(AttackerLvl >= 85 ) { zd = 22; g = 18; }
	else if(AttackerLvl >= 80 ) { zd = 21; g = 17; }
	else if(AttackerLvl >= 75 ) { zd = 20; g = 16; }
	else if(AttackerLvl >= 70 ) { zd = 19; g = 15; }
	else if(AttackerLvl >= 65 ) { zd = 18; g = 14; }
	else if(AttackerLvl >= 60 ) { zd = 17; g = 13; }
	else if(AttackerLvl >= 55 ) { zd = 16; g = 12; }
	else if(AttackerLvl >= 50 ) { zd = 15; g = 11; }
	else if(AttackerLvl >= 45 ) { zd = 14; g = 10; }
	else if(AttackerLvl >= 40 ) { zd = 13; g = 9; }
	else if(AttackerLvl >= 30 ) { zd = 12; g = 8; }
	else if(AttackerLvl >= 20 ) { zd = 11; g = 7; }
	else if(AttackerLvl >= 16 ) { zd = 9; g = 6; }
	else if(AttackerLvl >= 12 ) { zd = 8; g = 6; }
	else if(AttackerLvl >= 10 ) { zd = 7; g = 6; }
	else if(AttackerLvl >= 8)   { zd = 6; g = 5; }
	else                        { zd = 5; g = 5; }

	float xp = 0.0f;
	float fVictim = float(VictimLvl);
	float fAttacker = float(AttackerLvl);

	if(VictimLvl == AttackerLvl)
		xp = float( ((fVictim * 5.0f) + 45.0f) );
	else if(VictimLvl > AttackerLvl)
	{
		float j = 1.0f + (0.25f * (fVictim - fAttacker));
		xp = float( ((AttackerLvl * 5.0f) + 45.0f) * j );
	}
	else
	{
		if((AttackerLvl - VictimLvl) < g)
		{
			float j = (1.0f - float((fAttacker - fVictim) / zd));
			xp = (AttackerLvl * 5.0f + 45.0f) * j;
		}
	}

	// multiply by global XP rate
	if(xp == 0.0f) return 0;

	xp *= sWorld.getRate(RATE_XP);

	// elite boss multiplier
	if(victimI)
	{
		switch(victimI->Rank)
		{
		case 0: // normal mob
			break;
		case 1: // elite
			xp *= 2.0f;
			break;
		case 2: // rare elite
			xp *= 2.0f;
			break;
		case 3: // world boss
			xp *= 2.5f;
			break;
		default:	// rare or higher
			//			xp *= 7.0f;
			break;
		}
	}
	return (uint32)xp;
}

//Taken from WoWWoW Source
/*
Author: pionere

Calculate the stat increase. Using 3rd grade polynome.

Parameter level The level the character reached.
Parameter a3 The factor for x^3.
Parameter a2 The factor for x^2.
Parameter a1 The factor for x^1.
Parameter a0 The constant factor for the polynome.
Return stat gain
*/
uint32 CalculateStat(uint16 level,double a3, double a2, double a1, double a0)
{
	int result1;
	int result2;
	int diff;

	result1 =	(int)(a3*level*level*level +
		a2*level*level +
		a1*level +
		a0);

	result2 =	(int)(a3*(level-1)*(level-1)*(level-1) +
		a2*(level-1)*(level-1) +
		a1*(level-1) +
		a0);

	//get diffrence
	diff = result1-result2;
	return diff;
}
//Partialy taken from WoWWoW Source
uint32 GainStat(uint16 level, uint8 playerclass,uint8 Stat)
{
	uint32 gain = 0;
	switch(playerclass)
	{
     case DEATHKNIGHT: // Je prend les stats du guerrier en attendant
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000039, 0.006902, 1.080040, -1.051701); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000022, 0.004600, 0.655333, -0.600356); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000059, 0.004044, 1.040000, -1.488504); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000002, 0.001003, 0.100890, -0.076055); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000006, 0.002031, 0.278360, -0.340077); }break;
			}

		}break;

	case WARRIOR:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000039, 0.006902, 1.080040, -1.051701); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000022, 0.004600, 0.655333, -0.600356); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000059, 0.004044, 1.040000, -1.488504); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000002, 0.001003, 0.100890, -0.076055); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000006, 0.002031, 0.278360, -0.340077); }break;
			}

		}break;

	case PALADIN:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000037, 0.005455, 0.940039, -1.000090); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000020, 0.003007, 0.505215, -0.500642); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000038, 0.005145, 0.871006, -0.832029); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000023, 0.003345, 0.560050, -0.562058); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000032, 0.003025, 0.615890, -0.640307); }break;
			}
		}break;

	case HUNTER:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000022, 0.001800, 0.407867, -0.550889); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000040, 0.007416, 1.125108, -1.003045); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000031, 0.004480, 0.780040, -0.800471); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000020, 0.003007, 0.505215, -0.500642); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000017, 0.003803, 0.536846, -0.490026); }break;
			}
		}break;

	case ROGUE:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000025, 0.004170, 0.654096, -0.601491); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000038, 0.007834, 1.191028, -1.203940); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000032, 0.003025, 0.615890, -0.640307); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000008, 0.001001, 0.163190, -0.064280); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000024, 0.000981, 0.364935, -0.570900); }break;
			}
		}break;

	case PRIEST:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000008, 0.001001, 0.163190, -0.064280); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000022, 0.000022, 0.260756, -0.494000); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000024, 0.000981, 0.364935, -0.570900); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000039, 0.006981, 1.090090, -1.006070); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000040, 0.007416, 1.125108, -1.003045); }break;
			}
		}break;

	case SHAMAN:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000035, 0.003641, 0.734310, -0.800626); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000022, 0.001800, 0.407867, -0.550889); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000020, 0.006030, 0.809570, -0.809220); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000031, 0.004480, 0.780040, -0.800471); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000038, 0.005145, 0.871006, -0.832029); }break;
			}
		}break;

	case MAGE:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000002, 0.001003, 0.100890, -0.076055); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000008, 0.001001, 0.163190, -0.064280); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000006, 0.002031, 0.278360, -0.340077); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000040, 0.007416, 1.125108, -1.003045); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000039, 0.006981, 1.090090, -1.006070); }break;
			}
		}break;

	case WARLOCK:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000006, 0.002031, 0.278360, -0.340077); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000024, 0.000981, 0.364935, -0.570900); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000021, 0.003009, 0.486493, -0.400003); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000059, 0.004044, 1.040000, -1.488504); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000040, 0.006404, 1.038791, -1.039076); }break;
			}
		}break;

	case DRUID:
		{
			switch(Stat)
			{
			case STAT_STRENGTH:  { gain = CalculateStat(level, 0.000021, 0.003009, 0.486493, -0.400003); }break;
			case STAT_AGILITY:   { gain = CalculateStat(level, 0.000041, 0.000440, 0.512076, -1.000317); }break;
			case STAT_STAMINA:   { gain = CalculateStat(level, 0.000023, 0.003345, 0.560050, -0.562058); }break;
			case STAT_INTELLECT: { gain = CalculateStat(level, 0.000038, 0.005145, 0.871006, -0.832029); }break;
			case STAT_SPIRIT:	{ gain = CalculateStat(level, 0.000059, 0.004044, 1.040000, -1.488504); }break;
			}
		}break;
	}
	return gain;
}

uint32 CalculateDamage( Unit* pAttacker, Unit* pVictim, uint32 weapon_damage_type, SpellEntry* ability )
{
	//TODO: Some awesome formula to determine how much damage to deal
	//consider this is melee damage
	// weapon_damage_type: 0 = melee, 1 = offhand(dualwield), 2 = ranged

	// Attack Power increases your base damage-per-second (DPS) by 1 for every 14 attack power.
	// (c) wowwiki

	//type of this UNIT_FIELD_ATTACK_POWER_MODS is unknown, not even uint32 disabled for now.

	uint32 offset = 0;
	Item *it = NULL;

	if(pAttacker->disarmed && pAttacker->IsPlayer())
	{
		offset=UNIT_FIELD_MINDAMAGE;
		it = static_cast< Player* >(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
	}
	else if( weapon_damage_type == MELEE )   	offset = UNIT_FIELD_MINDAMAGE;
	else if( weapon_damage_type == OFFHAND )	offset = UNIT_FIELD_MINOFFHANDDAMAGE;
	else  /*weapon_damage_type == RANGED*/	    offset = UNIT_FIELD_MINRANGEDDAMAGE;

	float min_damage = pAttacker->GetFloatValue(offset);
	float max_damage = pAttacker->GetFloatValue(offset+1);
	if(it)
	{
		min_damage -= it->GetProto()->Damage[0].Min;
		max_damage -= it->GetProto()->Damage[0].Max;
	}

	float ap = 0;
	float bonus = 0.0;
	float wspeed = 0.0;

	if(offset == UNIT_FIELD_MINRANGEDDAMAGE)
	{
		//starting from base attack power then we apply mods on it
		//ap += pAttacker->GetRAP();
		ap += pVictim->RAPvModifier;

		if(!pVictim->IsPlayer() && ((Creature*)pVictim)->GetCreatureName())
		{
			uint32 creatType = ((Creature*)pVictim)->GetCreatureName()->Type;
			ap += (float)pAttacker->CreatureRangedAttackPowerMod[creatType];

			if(pAttacker->IsPlayer())
			{
				min_damage = (min_damage + static_cast< Player* >(pAttacker)->IncreaseDamageByType[creatType]) * (1 + static_cast< Player* >(pAttacker)->IncreaseDamageByTypePCT[creatType]);
				max_damage = (max_damage + static_cast< Player* >(pAttacker)->IncreaseDamageByType[creatType]) * (1 + static_cast< Player* >(pAttacker)->IncreaseDamageByTypePCT[creatType]);
			}
		}

		if(pAttacker->IsPlayer())
		{
			if(!pAttacker->disarmed)
			{
				Item *it = static_cast< Player* >(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
				if(it)
					wspeed = (float)it->GetProto()->Delay;
				else
					wspeed = 2000;
			}
			else
				wspeed = (float)pAttacker->GetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME);
		}
		else
		{
			wspeed = (float)pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);
		}

		//ranged weapon normalization.
		if(pAttacker->IsPlayer() && ability)
		{
			if(ability->Effect[0] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[1] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[2] == SPELL_EFFECT_DUMMYMELEE)
			{
				wspeed = 2800;
			}
		}

		//Weapon speed constant in feral forms
		if(pAttacker->IsPlayer())
		{
			if(static_cast< Player* >(pAttacker)->IsInFeralForm())
			{
				uint8 ss = static_cast< Player* >(pAttacker)->GetShapeShift();

				if(ss == FORM_CAT)
					wspeed = 1000.0;
				else if(ss == FORM_DIREBEAR || ss == FORM_BEAR)
					wspeed = 2500.0;
			}
		}

		bonus = ((wspeed-pAttacker->GetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME))/14000.0f)*ap;
		min_damage += bonus;
		max_damage += bonus;
	}
	else
	{
		//MinD = AP(28AS-(WS/7))-MaxD
		//starting from base attack power then we apply mods on it
		//ap += pAttacker->GetAP();
		ap += pVictim->APvModifier;

		if(!pVictim->IsPlayer() && ((Creature*)pVictim)->GetCreatureName())
		{
			uint32 creatType = ((Creature*)pVictim)->GetCreatureName()->Type;
		
			ap += (float)pAttacker->CreatureAttackPowerMod[creatType];

			if(pAttacker->IsPlayer())
			{
				min_damage = (min_damage + static_cast< Player* >(pAttacker)->IncreaseDamageByType[creatType]) * (1 + static_cast< Player* >(pAttacker)->IncreaseDamageByTypePCT[creatType]);
				max_damage = (max_damage + static_cast< Player* >(pAttacker)->IncreaseDamageByType[creatType]) * (1 + static_cast< Player* >(pAttacker)->IncreaseDamageByTypePCT[creatType]);
			}
		}

		if(pAttacker->IsPlayer())
		{
			if(!pAttacker->disarmed)
			{
				Item *it = static_cast< Player* >(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);

				if(it)
					wspeed = (float)it->GetProto()->Delay;
				else
					wspeed = 2000;
			}
			else
				wspeed = (float)pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);


			int32 apall = pAttacker->GetAP();
			int32 apb = 0;
			SM_FIValue(pAttacker->SM_PAPBonus, &apb, ability);

			if(apb) ap += apall*((float)apb/100);
			else    ap = float(apall);
		}
		else
		{
			wspeed = (float)pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);
		}

		//Normalized weapon damage checks.
		if(pAttacker->IsPlayer() && ability)
		{
			if(ability->Effect[0] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[1] == SPELL_EFFECT_DUMMYMELEE || ability->Effect[2] == SPELL_EFFECT_DUMMYMELEE)
			{
				it = static_cast< Player* >(pAttacker)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);

				if(it)
				{
					if(it->GetProto()->Class == 2) //weapon
					{
						if(it->GetProto()->InventoryType == INVTYPE_2HWEAPON) wspeed = 3300;
						else if(it->GetProto()->SubClass == 15) wspeed = 1700;
						else wspeed = 2400;
					}
				}
			}
		}

		//Weapon speed constant in feral forms
		if(pAttacker->IsPlayer())
		{
			if(static_cast< Player* >(pAttacker)->IsInFeralForm())
			{
				uint8 ss = static_cast< Player* >(pAttacker)->GetShapeShift();

				if(ss == FORM_CAT)
					wspeed = 1000.0;
				else if(ss == FORM_DIREBEAR || ss == FORM_BEAR)
					wspeed = 2500.0;
			}
		}

		if (offset == UNIT_FIELD_MINDAMAGE)
			bonus = (wspeed-pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME))/14000.0f*ap;
		else if( offset == UNIT_FIELD_MINOFFHANDDAMAGE )
			bonus = (wspeed-pAttacker->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME + 1)) / 14000.0f*ap;			
		else
			bonus = (wspeed-pAttacker->GetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME))/14000.0f*ap;
		min_damage += bonus;
		max_damage += bonus;
	}
	float diff = fabs(max_damage - min_damage);
	float result = min_damage;

	if(diff >= 1)
		result += float(RandomDouble(diff));

	if(result >= 0)
	{
			if( pAttacker->IsPlayer() && ((Player*)pAttacker)->m_outStealthDamageBonusTimer )
		{
			if( (uint32)UNIXTIME >= ((Player*)pAttacker)->m_outStealthDamageBonusTimer )
				((Player*)pAttacker)->m_outStealthDamageBonusTimer = 0;
			else
				result *= ((float(((Player*)pAttacker)->m_outStealthDamageBonusPct) / 100.0f) + 1.0f);
		}

		return FL2UINT(result);
	}

	return 0;
}

bool isEven (int num)
{
	if ((num%2)==0)
	{
		return true;
	}

	return false;
}



