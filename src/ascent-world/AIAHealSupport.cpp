/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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


//#ifdef USE_SPECIFIC_AIAGENTS

#ifndef MAX_INT
	#define MAX_INT ( 1 << 30 )
#endif 
#ifndef PI
	#define PI ( 3.14f )	//yeah i know this is low precision
#endif 

#define MAX(a,b) (a>b?a:b)

//will get bigger if it is better to cast this spell then some other version
float GetSpellEficiencyFactor(SpellEntry *sp)
{
	/*
	SpellCastTime *sd = dbcSpellCastTime.LookupEntry( sp->CastingTimeIndex );
	return (float)(sp->EffectBasePoints[0]) / ( GetCastTime( sd ) * 2 + sp->manaCost + 1) ;
	*/
	return(0); // pour test (brz)
}

void AiAgentHealSupport::Init(Unit *un, AIType at, MovementType mt, Unit *owner)
{
	//un->SetUInt32Value( UNIT_FIELD_BASE_HEALTH , un->GetUInt32Value( UNIT_FIELD_HEALTH ));
	//un->SetUInt32Value( UNIT_FIELD_BASE_MANA , un->GetUInt32Value( UNIT_FIELD_POWER1 ));

	//AIInterface::Init(un,at,mt,owner);//run the original init and we add our custom stuff too
	Log.zPvP("AiAgentHealSupport","Init");

	// La class AIInterface a un override avec AIHealInterface, on tourne en rond ou un truc m'a echappé (Brz)
    //AIInterface::Init(un,at,mt,owner); 
	m_AIType = at;
	m_MovementType = mt;

	m_AIState = STATE_IDLE;
	m_MovementState = MOVEMENTSTATE_STOP;

	m_Unit = un;
	m_PetOwner = owner;

	m_walkSpeed = m_Unit->m_walkSpeed*0.001f;//move distance per ms time 
	m_runSpeed = m_Unit->m_runSpeed*0.001f;//move/ms
	m_flySpeed = m_Unit->m_flySpeed*0.001f;
	m_sourceX = m_Unit->GetPositionX();
	m_sourceY = m_Unit->GetPositionY();
	m_sourceZ = m_Unit->GetPositionZ();

	m_fallowAngle = 2 * PI - PI / 6;
	FollowDistance = 5 ; //we are support, we stand behind our master 

	DifficultyLevel = m_PetOwner->GetUInt32Value(UNIT_FIELD_LEVEL) / HealSpellLevels;
	if( DifficultyLevel > HealSpellLevels - 2 )
		DifficultyLevel = HealSpellLevels - 2;

	m_Unit->SetIAGuard(true); // Use IA, Test (Brz)

	//scale health and mana
	m_Unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH , m_Unit->GetUInt32Value( UNIT_FIELD_BASE_HEALTH ) * DifficultyLevel * CREATURE_STATS_SCALE_WITH_DIFFICULTY );
	m_Unit->SetUInt32Value( UNIT_FIELD_MAXPOWER1 , m_Unit->GetUInt32Value( UNIT_FIELD_BASE_MANA ) * DifficultyLevel * CREATURE_STATS_SCALE_WITH_DIFFICULTY );
	m_Unit->SetUInt32Value( UNIT_FIELD_HEALTH    , m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
	m_Unit->SetUInt32Value( UNIT_FIELD_POWER1    , m_Unit->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) );

	uint8 race = m_PetOwner->getRace();

	if( race == RACE_HUMAN || race == RACE_DWARF || race == RACE_NIGHTELF || race == RACE_GNOME || race == RACE_DRAENEI )
		Owner_side = OWNER_SIDE_ALIANCE;
	else
		Owner_side = OWNER_SIDE_HORDE;

	last_time_full_health = true; //before we start healing we should try to quick augment our master

	m_defend_self = dbcSpell.LookupEntry( __Divine_Shield ); // Divine Shield 1

	uint32 local_itr,local_itr2;

	memset(m_HealSpells,NULL,HealSpellCount*HealSpellLevels*sizeof(SpellEntry*));
	memset(m_HealSpellsEficiency,NULL,HealSpellCount*HealSpellLevels*sizeof(float));

	//we are using const lists. You don't like it ? Write your own AI :P
	//owner : Lesser Heal, Flash Heal, heal, Healing Touch, Greater Heal, Great Heal, Heal Brethren, Debuff Uber Heal Over Time
	//group : Prayer of Healing, Chain Heal, Healing Aura
	//augment : Uber Heal Over Time, Spiritual Healing,Healing Light, renew
	//augment : renew, power word shield, Dampen Magic, Blessing of Sanctuary 
	local_itr = 0;
	local_itr2 = 0;
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Prayer_of_Healing____1 ); // 30604
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Prayer_of_Healing____2 ); // 33152
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Prayer_of_Healing____3 ); // 35943
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Prayer_of_Healing____4 ); // 59698
	/*
	local_itr++;
	local_itr2 = 0;
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Chain_Heal____1 ); // 16367
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Chain_Heal____2 ); // 33642
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Chain_Heal____3 ); // 41114
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Chain_Heal____4 ); // 42027
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Chain_Heal____5 ); // 42477
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Chain_Heal____6 ); // 43527
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Chain_Heal____7 ); // 48894
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Chain_Heal____8 ); // 54481
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Chain_Heal____9 ); // 59473
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Chain_Heal____10 ); // 69923
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Chain_Heal____11 ); // 70425
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Chain_Heal____12 ); // 71120
	
	local_itr++;
	local_itr2 = 0;
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Healing_Aura____0 ); // 5607
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Healing_Aura____1 ); // 6275
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Healing_Aura____2 ); // 11900
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Healing_Aura____3 ); // 15870
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Healing_Aura____4 ); // 31762
	m_PartySpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Healing_Aura____5 ); // 42374	
    */
	local_itr = 0;
	local_itr2 = 0;
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew_Rank_1 ); // 139
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew_Rank_2 ); // 6074
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew_Rank_3 ); // 6075
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew_Rank_5 ); // 6077
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew_Rank_6 ); // 6078

    /*
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____1 ); // 22168
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____2 ); // 23895
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____3 ); // 25058
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____4 ); // 28807
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____5 ); // 31325
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____6 ); // 34423
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____7 ); // 36679
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____9 ); // 37260
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____10 ); // 37978
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____11 ); // 38210
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____13 ); // 37260
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____14 ); // 45859
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____15 ); // 46192
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____16 ); // 46563
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____17 ); // 47079
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____18 ); // 49263
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____20 ); // 57777
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____21 ); // 60004
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____22 ); // 61967
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____23 ); // 62333
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____24 ); // 62441
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____25 ); // 66177
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____26 ); // 66537
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____27 ); // 67675
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____28 ); // 68035
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____29 ); // 68036
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Renew____30 ); // 68037
    */
    /*
	local_itr++;
	local_itr2 = 0;
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____1 ); // 11974
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____2 ); // 17139
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____3 ); // 20697
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____4 ); // 22187
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____5 ); // 29408
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____6 ); // 32595
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____7 ); // 35944
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____8 ); // 36052
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____9 ); // 41373
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____10 ); // 44175
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____11 ); //44291
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____12 ); //46193
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____13 ); //66099
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____14 ); //68032
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____15 ); //68033
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____16 ); //68034
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____17 ); //71548
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____18 ); //71780
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Power_Word__Shield____19 ); //71781
	*/
	
	/*local_itr++;
	local_itr2 = 0;
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 604 ); // Dampen Magic 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 8450 ); // Dampen Magic 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 8451 ); // Dampen Magic 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10173 ); // Dampen Magic 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 10174 ); // Dampen Magic 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 33944 ); // Dampen Magic 2

	local_itr++;
	local_itr2 = 0;
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 20911 ); // Blessing of Sanctuary 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 20913 ); // Blessing of Sanctuary 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 20914 ); // Blessing of Sanctuary 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 20914 ); // Blessing of Sanctuary 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 27168 ); // Blessing of Sanctuary 1
	m_AugmentSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 27169 ); // Blessing of Sanctuary 2*/

	local_itr = 0;
	local_itr2 = 0;
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Lesser_Heal_Rank_2 ); // Lesser Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Lesser_Heal_Rank_2 ); // Lesser Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Lesser_Heal_Rank_2 ); // Lesser Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Lesser_Heal_Rank_2 ); // Lesser Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Lesser_Heal_Rank_2 ); // Lesser Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Lesser_Heal_Rank_3 ); // Lesser Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Lesser_Heal_Rank_3 ); // Lesser Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Lesser_Heal_Rank_3 ); // Lesser Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Lesser_Heal_Rank_3 ); // Lesser Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Lesser_Heal_Rank_3 ); // Lesser Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
/*
	local_itr++;
	local_itr2 = 0;
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____1 ); // Flash Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____1 ); // Flash Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____2 ); // Flash Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____2 ); // Flash Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____3 ); // Flash Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____3 ); // Flash Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____4 ); // Flash Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____4 ); // Flash Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____4 ); // Flash Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____5); // Flash Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____5 ); // Flash Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____5 ); // Flash Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____6 ); // Flash Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____6 ); // Flash Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____6 ); // Flash Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____8 ); // Flash Heal 8
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____8 ); // Flash Heal 8
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Flash_Heal____8 ); // Flash Heal 8
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	
	local_itr++;
	local_itr2 = 0;
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Heal_Rank_1 ); // Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Heal_Rank_1 ); // Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Heal_Rank_1 ); // Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Heal_Rank_2 ); // Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Heal_Rank_2 ); // Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Heal_Rank_2 ); // Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Heal_Rank_3 ); // Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Heal_Rank_3 ); // Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Heal_Rank_3 ); // Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Heal_Rank_4 ); // Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Heal_Rank_4 ); // Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Heal_Rank_4 ); // Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);

	local_itr++;
	local_itr2 = 0;
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Healing_Touch____1 ); // Healing Touch 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Healing_Touch____2 ); // Healing Touch 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Healing_Touch____5 ); // Healing Touch 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Healing_Touch____6 ); // Healing Touch 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Healing_Touch____7 ); // Healing Touch 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Healing_Touch____8 ); // Healing Touch 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);

	local_itr++;
	local_itr2 = 0;
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____1 ); // Greater Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____2 ); // Greater Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____3 ); // Greater Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____4 ); // Greater Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____4 ); // Greater Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____5 ); // Greater Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____5 ); // Greater Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____6 ); // Greater Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____6 ); // Greater Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____7 ); // Greater Heal 7
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____7 ); // Greater Heal 7
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____8); // Greater Heal 8
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____8 ); // Greater Heal 8
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____9); // Greater Heal 9
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____9); // Greater Heal 9
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____10); // Greater Heal 10
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____10); // Greater Heal 10
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____11); // Greater Heal 11
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____11); // Greater Heal 11
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____12); // Greater Heal 12
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____12); // Greater Heal 12
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____13); // Greater Heal 13
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____13); // Greater Heal 13
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____14); // Greater Heal 14
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Greater_Heal____14); // Greater Heal 14
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);	
	
	local_itr++;
	local_itr2 = HealSpellLevels / 2;

	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____0); // Great Heal 0
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____0 ); // Great Heal 0
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____0 ); // Great Heal 0
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____0); // Great Heal 0
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____0 ); // Great Heal 0
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);	

	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____1); // Great Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____1); // Great Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____1 ); // Great Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____1 ); // Great Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____1 ); // Great Heal 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);

	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____2 ); // Great Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____2 ); // Great Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____2 ); // Great Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____2 ); // Great Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____2 ); // Great Heal 2
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);

	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____3 ); // Great Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____3 ); // Great Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____3 ); // Great Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____3 ); // Great Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____3 ); // Great Heal 3
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);

	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____4 ); // Great Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____4 ); // Great Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____4 ); // Great Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____4 ); // Great Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____4 ); // Great Heal 4
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);	

	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____5 ); // Great Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____5 ); // Great Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____5 ); // Great Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____5 ); // Great Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____5 ); // Great Heal 5
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);	

	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____6 ); // Great Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____6 ); // Great Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____6 ); // Great Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____6 ); // Great Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( __Great_Heal____6 ); // Great Heal 6
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
	
	local_itr++;
	local_itr2 = HealSpellLevels - 2; //kinda uber spells :P
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 26565 ); // Heal Brethren 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);

	local_itr++;
	local_itr2 = HealSpellLevels - 2; //kinda uber spells :P
	m_HealSpells[local_itr2++][local_itr] = dbcSpell.LookupEntry( 30839 ); // Debuff Uber Heal Over Time 1
	m_HealSpellsEficiency[local_itr2][local_itr] = GetSpellEficiencyFactor(m_HealSpells[local_itr2][local_itr]);
*/

/*	//group : Prayer of Healing, Chain Heal, Healing Aura, Powerful Healing Ward
	//augment : Uber Heal Over Time, Spiritual Healing,Healing Light, renew
	sp = dbcSpell.LookupEntry( 122 ); // frost nova 1
	m_DefendSpells.push_back( sp ); */

	revive_spell = dbcSpell.LookupEntry( 2006 ); // resurrection 1
}

bool AiAgentHealSupport::CheckCanCast(SpellEntry *sp, Unit *target)
{
	uint32 Time_Now = getMSTime();

	if ( !sp ) return false;

	if ( !target ) return false;

	if( target->GetUInt32Value(UNIT_FIELD_POWER1) < sp->manaCost )
		return false; //we do not have enough juice

	CooldownMap::iterator itr = spell_cooldown_map.find( sp->Id );
	if( itr != spell_cooldown_map.end() && itr->second >= Time_Now )
		return false; // this spell is still cooling down

	return true;
}

SpellEntry*	AiAgentHealSupport::Get_Best_Heal_Spell(Unit *for_unit)
{
	char Pnj_Say[255];

	uint32 now_health = for_unit->GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 max_health = for_unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	uint32 health_to_heal = max_health - now_health;
	//get best spell to match our needs :D
	float best_match= 1 << 30 ;
	uint32 selected_spell_index = MAX_INT;
	for(uint32 i=0;i<HealSpellCount-1;i++)
#ifdef SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY
		if( m_HealSpells[DifficultyLevel][i]
			&& SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY * DifficultyLevel * m_HealSpells[DifficultyLevel][i]->EffectBasePoints[0] <= health_to_heal
			&& SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY * DifficultyLevel * m_HealSpells[DifficultyLevel][i]->EffectBasePoints[0] >= health_to_heal)
#else
		if( m_HealSpells[DifficultyLevel][spell_slector]
			&& m_HealSpells[DifficultyLevel][i]->EffectBasePoints[0] <= health_to_heal
			&& m_HealSpells[DifficultyLevel][i+1]->EffectBasePoints[0] >= health_to_heal
#endif
		{
			uint32 spell_slector = ( i + 1 ) % HealSpellCount;
			if(	CheckCanCast ( m_HealSpells[DifficultyLevel][spell_slector] , for_unit) )
			{ 
#ifdef SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY
				float cur_efficiency = ( SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY * DifficultyLevel * m_HealSpellsEficiency[DifficultyLevel][spell_slector] * MAX( 1.0f,( health_to_heal - m_HealSpells[DifficultyLevel][spell_slector]->EffectBasePoints[0] ) ) );
#else
				float cur_efficiency = ( m_HealSpellsEficiency[DifficultyLevel][spell_slector] * max(1,( health_to_heal - m_HealSpells[DifficultyLevel][spell_slector]->EffectBasePoints[0] ) ) );
#endif
				if( cur_efficiency <  best_match)
				{
                 sprintf(Pnj_Say,"Je vais soigner quelqu'un");
                 for_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, Pnj_Say);
				 for_unit->Emote(EMOTE_ONESHOT_TALK);

				 Log.zPvP("Pnj Healer","Va soigner id %u , ancien %f nouveau %f",
	               m_HealSpells[DifficultyLevel][spell_slector]->Id,best_match,cur_efficiency);
					best_match = cur_efficiency;
					selected_spell_index = spell_slector;
				}
			}
		}
	if( selected_spell_index != MAX_INT )
	{
		sprintf(Pnj_Say,"C'est le numero %u de mon grimoire",m_HealSpells[DifficultyLevel][selected_spell_index]->Id);
        for_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, Pnj_Say);
		for_unit->Emote(EMOTE_ONESHOT_TALK);

		Log.zPvP("Pnj Healer","sélection du sort avec l'id %u et le prix %f",m_HealSpells[ DifficultyLevel ][ selected_spell_index ]->Id,best_match);
		return m_HealSpells[ DifficultyLevel ][ selected_spell_index ];
	}
	return NULL ;
}

void AiAgentHealSupport::_UpdateCombat(uint32 p_time)
{
	char Pnj_Say[255];

	m_nextTarget = m_PetOwner;

	Log.zPvP("Pnj Healer","Updatecombat");

	if( !m_nextTarget )
		return; //oh noez, our master has abandoned us ! Where is te luv ?

   Log.zPvP("Pnj Healer","Trouve une cible");

	if( m_Unit->isCasting() )
		return; // we are already supporting someone ...get back later

	//for fun : mimic master standstate. Note that this might give strange results
	/*
	STANDSTATE_STAND			 = 0x00000000,
	STANDSTATE_SIT				 = 0x00000001,
	STANDSTATE_SIT_CHAIR		 = 0x00000002,
	STANDSTATE_SLEEP			 = 0x00000003,
	STANDSTATE_SIT_LOW_CHAIR	 = 0x00000004,
	STANDSTATE_SIT_MEDIUM_CHAIR  = 0x00000005,
	STANDSTATE_SIT_HIGH_CHAIR    = 0x00000006,
	STANDSTATE_DEAD				 = 0x00000007,
	STANDSTATE_KNEEL			 = 0x00000008, // En fuite
	STANDSTATE_FORM_ALL          = 0x00FF0000,
    STANDSTATE_FLAG_ALWAYS_STAND = 0x01000000,
    STANDSTATE_FLAG_CREEP        = 0x02000000,
    STANDSTATE_FLAG_UNTRACKABLE  = 0x04000000,
    STANDSTATE_FLAG_ALL          = 0xFF00000
	*/
	if( m_PetOwner->GetStandState() != m_Unit->GetStandState() )
		m_Unit->SetStandState( m_PetOwner->GetStandState() );
	//----------

	//we should be at same level at owner so we profit of fighting formulas same as owner
	if(	m_Unit->GetUInt32Value( UNIT_FIELD_LEVEL ) != m_PetOwner->GetUInt32Value( UNIT_FIELD_LEVEL ) )
		m_Unit->SetUInt32Value( UNIT_FIELD_LEVEL, m_PetOwner->GetUInt32Value( UNIT_FIELD_LEVEL ) );

    // Log.zPvP("Pnj Healer","nous ne jetons pas de sort");

	/* C'est quoi ce truc ? A Voir (Brz)
	uint32 new_DifficultyLevel = m_PetOwner->GetUInt32Value(UNIT_FIELD_LEVEL) / HealSpellLevels;
	if( new_DifficultyLevel != new_DifficultyLevel)
	{
		if( new_DifficultyLevel > HealSpellLevels - 2 )
			new_DifficultyLevel = HealSpellLevels - 2;
		DifficultyLevel = new_DifficultyLevel;
		//scale health and mana.when we level we max out our stats
		m_Unit->SetUInt32Value( UNIT_FIELD_MAXHEALTH , m_Unit->GetUInt32Value( UNIT_FIELD_BASE_HEALTH ) * new_DifficultyLevel * CREATURE_STATS_SCALE_WITH_DIFFICULTY );
		m_Unit->SetUInt32Value( UNIT_FIELD_MAXPOWER1 , m_Unit->GetUInt32Value( UNIT_FIELD_BASE_MANA ) * new_DifficultyLevel * CREATURE_STATS_SCALE_WITH_DIFFICULTY );
		m_Unit->SetUInt32Value( UNIT_FIELD_HEALTH , m_Unit->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) );
		m_Unit->SetUInt32Value( UNIT_FIELD_POWER1 , m_Unit->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) );
	}*/

	//if owner is mounted then we mount too. Speed is not set though
	if( m_PetOwner->GetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID ) && m_Unit->GetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID ) == 0 )
	{
		if( Owner_side == OWNER_SIDE_ALIANCE )
			m_Unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, HELPER_MOUNT_A_DISPLAY );
		else
			m_Unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, OWNER_SIDE_HORDE );
		m_moveSprint =  true;
	}
	else if( m_PetOwner->GetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID ) == 0 && m_Unit->GetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID ) != 0 )
	{
		m_Unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, 0 );
		m_moveSprint = false;
	}

	if ( m_PetOwner->GetHealthPct() == 100 )
		last_time_full_health = true;
	
	uint32 Time_Now = getMSTime();

	SpellCastTargets targets;
	m_castingSpell = NULL;

	//poor thing died. Res him
	if( !m_nextTarget->isAlive() 
		&& m_Unit->GetUInt32Value(UNIT_FIELD_POWER1) >= revive_spell->manaCost
		&& spell_cooldown_map[ revive_spell->Id ] < Time_Now
		)
	{
		m_castingSpell = revive_spell;
		setSpellTargets(m_castingSpell, m_PetOwner);

		sprintf(Pnj_Say,"Pas de panique, je vais te ressuciter %s",((Player *)m_PetOwner)->GetName());
        m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, Pnj_Say);
		m_Unit->Emote(EMOTE_ONESHOT_TALK);
        //Log.zPvP("Pnj Healer","mon ami est mort, je vais le ressuciter");
	}

	if ( last_time_full_health == true && m_PetOwner->GetHealthPct() != 100 && m_castingSpell == NULL )
	{
		uint32 augment_DifficultyLevel = m_PetOwner->GetUInt32Value(UNIT_FIELD_LEVEL) / AugmentSpellCount;
		//pick 1 random augment spell
		uint32 augment_spell = RandomUInt( AugmentSpellLevels );
		if( m_AugmentSpells [ augment_DifficultyLevel ] [ augment_spell ] )
		{
			m_castingSpell = m_AugmentSpells [ augment_DifficultyLevel ] [ augment_spell ];
			setSpellTargets( m_castingSpell, m_PetOwner );

			sprintf(Pnj_Say,"J'ai choisi le sort %u",m_castingSpell->Id);
            m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, Pnj_Say);
		    m_Unit->Emote(EMOTE_ONESHOT_TALK);
		}
		else
		{
			sprintf(Pnj_Say,"Aie aie, je n'ai pas trouver de sort [%u][%u]",augment_DifficultyLevel,augment_spell);
            m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, Pnj_Say);
		    m_Unit->Emote(EMOTE_ONESHOT_TALK);
		}
	}

	// Surveillance du player
	if ( m_PetOwner->GetHealthPct() < 100 && m_castingSpell == NULL )  // 100% pour test (Brz)
	{
		m_castingSpell = Get_Best_Heal_Spell( m_PetOwner );
		if ( m_castingSpell ) 
		{
		 sprintf(Pnj_Say,"Pas de panique, je vais te soigner %s",((Player *)m_PetOwner)->GetName());
         m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, Pnj_Say);
		 m_Unit->Emote(EMOTE_ONESHOT_TALK);

		 setSpellTargets( m_castingSpell, m_PetOwner );
		}
        else 
		{
		 sprintf(Pnj_Say,"Attend un peu %s, je suis un peu fatigue",((Player *)m_PetOwner)->GetName());
         m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, Pnj_Say);
		 m_Unit->Emote(EMOTE_ONESHOT_TALK);
		 //Log.zPvP("Pnj Healer","nous ne sommes pas capable de choisir un sort de soin en raison d'une question de puissance %u ou de temps de recharge",
		 //                     m_Unit->GetUInt32Value(UNIT_FIELD_POWER1));
		}
	}

	// Auto surveillance
	if ( m_Unit->GetHealthPct() < 100 && m_castingSpell == NULL ) // 100% pour test (Brz)
	{
		if(	!Protect_self() ) //first we try to escape combat
		{
			sprintf(Pnj_Say,"J'ai un probleme %s, je dois me soigner",((Player *)m_PetOwner)->GetName());
            m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, Pnj_Say);
		    m_Unit->Emote(EMOTE_ONESHOT_TALK);
            //Log.zPvP("Pnj Healer","nous sommes blesse, essayons de nous soigner");

			m_castingSpell = Get_Best_Heal_Spell( m_Unit );
			if ( m_castingSpell ) 
				setSpellTargets( m_castingSpell, m_Unit );
		}
        else 
		{
			sprintf(Pnj_Say,"Il y a un probleme %s, je dois reprendre mon souffle",((Player *)m_PetOwner)->GetName());
            m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, Pnj_Say);
		    m_Unit->Emote(EMOTE_ONESHOT_TALK);

			//Log.zPvP("Pnj Healer","nous ne sommes pas capable de choisir un sort de soin en raison de la puissance %u ou de temps de recharge",
			//  m_Unit->GetUInt32Value(UNIT_FIELD_POWER1));
		}
	}

	if ( m_PetOwner->GetHealthPct() == 100 && m_castingSpell == NULL && m_PetOwner->IsPlayer() && static_cast< Player*>( m_PetOwner )->InGroup())
	{
		uint32 party_DifficultyLevel = m_PetOwner->GetUInt32Value(UNIT_FIELD_LEVEL) / PartySpellCount;
		//pick 1 random augment spell
		uint32 party_spell = RandomUInt( PartySpellLevels );
		if( m_PartySpells [ party_DifficultyLevel ] [ party_spell ] )
		{
			m_castingSpell = m_PartySpells [ party_DifficultyLevel ] [ party_spell ];
			setSpellTargets( m_castingSpell, m_PetOwner );
		}
	}

	if( !m_castingSpell )
		return; //sorry but we are out of juice

      Log.zPvP("Pnj Healer","Lancement d'un sort");

/*	SpellCastTime *sd = dbcSpellCastTime.LookupEntry(m_castingSpell->CastingTimeIndex);
	//do not stop for instant casts
	if(GetCastTime(sd) != 0)
	{
		StopMovement(0);
printf("le sort n'est pas instantané alors nous allons arrêter le mouvement \n");
	} (brz) */ 

	float distance = m_Unit->GetDistanceSq(m_nextTarget);
	if(	distance <= m_castingSpell->base_range_or_radius_sqr || m_castingSpell->base_range_or_radius_sqr == 0 )
	{

		sprintf(Pnj_Say,"Ca y est je suis la %s",((Player *)m_PetOwner)->GetName());
        m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, Pnj_Say);
		m_Unit->Emote(EMOTE_ONESHOT_TALK);
        //Log.zPvP("Pnj Healer","nous sommes a portee et allons jeter le sort");
		m_AIState = STATE_CASTING;
		
		Spell *nspell = new Spell(m_Unit, m_castingSpell, false, NULL);

#ifdef SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY
		nspell->forced_basepoints[ 0 ] = (uint32)( m_castingSpell->EffectBasePoints[0] * SPELL_EFF_PCT_SCALE_WITH_DIFFICULTY * DifficultyLevel );
#endif

		if(nspell->prepare( &targets )!= SPELL_FAILED_SUCCESS)
		{
			sprintf(Pnj_Say,"%s, j'arrive pas a lancer le sort...",((Player *)m_PetOwner)->GetName());
            m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, Pnj_Say);
		    m_Unit->Emote(EMOTE_ONESHOT_TALK);
		}
		else
		{
		 CastSpell( m_Unit, m_castingSpell, targets );
         SetSpellDuration( m_castingSpell );
		}

	}
	else // Target out of Range -> Run to it
	{
		sprintf(Pnj_Say,"Je suis trop loin %s, j'arrive...",((Player *)m_PetOwner)->GetName());
        m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, Pnj_Say);
		m_Unit->Emote(EMOTE_ONESHOT_TALK);

        //Log.zPvP("Pnj Healer","nous allons nous rapprocher");

		m_moveRun = true;
		_CalcDestinationAndMove(m_nextTarget, sqrt( m_castingSpell->base_range_or_radius_sqr ) );
	}

// check if pets regenrate mana, If not then we should implement that too
}

void AiAgentHealSupport::SetSpellDuration(SpellEntry *sp)
{
	if ( !sp )
		return ;

	uint32 Time_Now = getMSTime();
	uint32 cooldowntime;

#ifdef SPELL_COOLD_PCT_SCALE_WITH_DIFFICULTY
	cooldowntime = 100; // (Brz) (uint32) (m_castingSpell->RecoveryTime / ( SPELL_COOLD_PCT_SCALE_WITH_DIFFICULTY * DifficultyLevel ) );
#else
	cooldowntime = m_castingSpell->RecoveryTime;
#endif

   /* if(sp->DurationIndex)
    {
        SpellDuration *sd=dbcSpellDuration.LookupEntry(sp->DurationIndex);
		uint32 maxduration;
		maxduration = MAX ( sd->Duration1, sd->Duration2 ) ;
		maxduration = MAX ( maxduration , sd->Duration3 );

		if( maxduration > cooldowntime )
			cooldowntime = maxduration; //do not recast aura or heal over time spells on target
	} (brz) */

	if( cooldowntime )
		spell_cooldown_map[ m_castingSpell->Id ] = Time_Now + cooldowntime;
}

bool AiAgentHealSupport::Protect_self()
{
	if ( CheckCanCast ( m_defend_self , m_Unit ) )
		return true;
	return false;
}

//#endif
