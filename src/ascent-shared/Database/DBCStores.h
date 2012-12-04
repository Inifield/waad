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

#ifndef __SPELLSTORE_H
#define __SPELLSTORE_H

#include "Common.h"
#include "DataStore.h"
#include "Timer.h"

#pragma pack(push,1)

extern char DBCPath[1024];
extern char RSDBCPath[1024];

struct WorldSafeLocsStoreEntry // Cimetieres
{
	uint32 Id;    // 1
	uint32 MapId; // 2
    float PosX;   // 3
	float PosY;   // 4
	float PosZ;   // 5
	// char *Name;   // 6 enGB
	// Not Used   // 7 
	char *Name;   // 8 Fr
	// Not used   // 9 - 22
};

struct WorldMapOverlayEntry
{
	uint32 AreaReference;  // 1
	// unused              // 2
	uint32 AreaTableID;    // 3
	// unused              // 4 - 8
	// char *InternalName  // 9
	// Witdh               // 10
	// Height              // 11
	// Left                // 12
	// Top                 // 13
	// Y1                  // 14
	// X1                  // 15
	// Y2                  // 16
	// X2                  // 17
};

struct AchievementEntry
{
    uint32 ID;                // 1
    uint32 factionFlag;       // 2 -1=all, 0=horde, 1=alliance
    uint32 mapID;             // 3 -1=none
	uint32 PrevAchievementId; // 4 
    //char* name;               // 5
	// Unk                    // 6
	char* name;               // 7 Fr
	// Unk                    // 8 à 21
	//char *Description;        // 22  enGB
	// description            // 23	
	char *Description;        // 22 Fr
	                          // à 38 
    uint32 categoryId;        // 39
    uint32 points;            // 40 reward points
    uint32 OrderInCategory;   // 41
    uint32 flags;             // 42
	uint32 is_statistic;      // 43
    //char   *RewardName;       // 44 enGB
	// unused                 // 45
	char   *RewardName;       // 46 Fr
	// unused                 // 47 à 60
	uint32 Count;             // 61       
    uint32 refAchievement;    // 62

	uint32 AssociatedCriteria[32]; // Custom stuff
	uint32 AssociatedCriteriaCount;
};
 
struct AchievementCategoryEntry
{
	uint32    ID;                                           // 1
	uint32    parentCategory;                               // 2 -1 for main category
	//char*     name;                                         // 3
	// unused                                               // 4
	char* name;                                             // 5 Fr
	// unused                                               // 6 à 19
	uint32    sortOrder;                                    // 20
};

struct AchievementCriteriaEntry
{
    uint32  ID;                                             // 1
    uint32  referredAchievement;                            // 2
    uint32  requiredType;                                   // 3
	/* A Recoder (Branruz)
    union
    {
		// ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE = 0
        // TODO: also used for player deaths..
        struct
        {
            uint32  creatureID;                             // 4
            uint32  creatureCount;                          // 5
        } kill_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_BG = 1
        // TODO: there are further criterias instead just winning
        struct
        {
            uint32  bgMapID;                                // 4
            uint32  winCount;                               // 5
        } win_bg;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL = 5
        struct
        {
            uint32  unused;                                 // 4
            uint32  level;                                  // 5
        } reach_level;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL = 7
        struct
        {
            uint32  skillID;                                // 3
            uint32  skillLevel;                             // 4
        } reach_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT = 8
        struct
        {
            uint32  linkedAchievement;                      // 4
        } complete_achievement;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT = 9
        struct
        {
            uint32  unused;                                 // 4
            uint32  totalQuestCount;                        // 5
        } complete_quest_count;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY = 10
        struct
        {
            uint32  unused;                                 // 3
            uint32  numberOfDays;                           // 4
        } complete_daily_quest_daily;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11
        struct
        {
            uint32  zoneID;                                 // 4
            uint32  questCount;                             // 5
        } complete_quests_in_zone;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST = 14
        struct
        {
            uint32  unused;                                 // 3
            uint32  questCount;                             // 4
        } complete_daily_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND= 15
        struct
        {
            uint32  mapID;                                  // 3
        } complete_battleground;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP= 16
        struct
        {
            uint32  mapID;                                  // 4
        } death_at_map;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID = 19
        struct
        {
            uint32  groupSize;                              // 3 can be 5, 10 or 25
        } complete_raid;

        // ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE = 20
        struct
        {
            uint32  creatureEntry;                          // 4
        } killed_by_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING = 24
        struct
        {
            uint32  unused;                                 // 3
            uint32  fallHeight;                             // 4
        } fall_without_dying;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST = 27
        struct
        {
            uint32  questID;                                // 3
            uint32  questCount;                             // 4
        } complete_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET = 28
        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2= 69
        struct
        {
            uint32  spellID;                                // 3
            uint32  spellCount;                             // 4
        } be_spell_target;

        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL= 29
        struct
        {
            uint32  spellID;                                // 3
            uint32  castCount;                              // 4
        } cast_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31
        struct
        {
            uint32  areaID;                                 // 3 Reference to AreaTable.dbc
            uint32  killCount;                              // 4
        } honorable_kill_at_area;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA = 32
        struct
        {
            uint32  mapID;                                  // 3 Reference to Map.dbc
        } win_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA = 33
        struct
        {
            uint32  mapID;                                  // 3 Reference to Map.dbc
        } play_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL = 34
        struct
        {
            uint32  spellID;                                // 3 Reference to Map.dbc
        } learn_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM = 36
        struct
        {
            uint32  itemID;                                 // 4
            uint32  itemCount;                              // 5
        } own_item;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA = 37
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                  // 4
            uint32  flag;                                   // 5 4=in a row
        } win_rated_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING = 38
        struct
        {
            uint32  teamtype;                               // 3 {2,3,5}
        } highest_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_TEAM_RATING = 39
        struct
        {
            uint32  teamtype;                               // 3 {2,3,5}
            uint32  teamrating;                             // 4
        } reach_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL = 40
        struct
        {
            uint32  skillID;                                // 3
            uint32  skillLevel;                             // 4 apprentice=1, journeyman=2, expert=3, artisan=4, master=5, grand master=6
        } learn_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM = 41
        struct
        {
            uint32  itemID;                                 // 3
            uint32  itemCount;                              // 4
        } use_item;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM = 42
        struct
        {
            uint32  itemID;                                 // 3
            uint32  itemCount;                              // 4
        } loot_item;

        // ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA = 43
        struct
        {
            // TODO: This rank is _NOT_ the index from AreaTable.dbc
            uint32  areaReference;                          // 4
        } explore_area;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK= 44
        struct
        {
            // TODO: This rank is _NOT_ the index from CharTitles.dbc
            uint32  rank;                                   // 4
        } own_rank;

        // ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT= 45
        struct
        {
            uint32  unused;                                 // 4
            uint32  numberOfSlots;                          // 5
        } buy_bank_slot;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION= 46
        struct
        {
            uint32  factionID;                              // 3
            uint32  reputationAmount;                       // 4 Total reputation amount, so 42000 = exalted
        } gain_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION= 47
        struct
        {
            uint32  unused;                                 // 3
            uint32  numberOfExaltedFactions;                // 4
        } gain_exalted_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM = 49
        // TODO: where is the required itemlevel stored?
        struct
        {
            uint32  itemSlot;                               // 3
        } equip_epic_item;

        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT= 50
        struct
        {
            uint32  rollValue;                              // 3
            uint32  count;                                  // 4
        } roll_need_on_loot;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS = 52
        struct
        {
            uint32  classID;                                // 4
            uint32  count;                                  // 5
        } hk_class;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_RACE = 53
        struct
        {
            uint32  raceID;                                 // 3
            uint32  count;                                  // 4
        } hk_race;

        // ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE = 54
        // TODO: where is the information about the target stored?
        struct
        {
            uint32  emoteID;                                // 4
        } do_emote;

        // ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE = 55
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                  // 4
            uint32  flag;                                   // 5 =3 for battleground healing
            uint32  mapid;                                  // 6
        } healing_done;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM = 57
        struct
        {
            uint32  itemID;                                 // 3
        } equip_item;


        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY = 67
        struct
        {
            uint32  unused;                                 // 3
            uint32  goldInCopper;                           // 4
        } loot_money;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT = 68
        struct
        {
            uint32  goEntry;                                // 3
            uint32  useCount;                               // 4
        } use_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL= 70
        // TODO: are those special criteria stored in the dbc or do we have to add another sql table?
        struct
        {
            uint32  unused;                                 // 3
            uint32  killCount;                              // 4
        } special_pvp_kill;

        // ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT = 72
        struct
        {
            uint32  goEntry;                                // 3
            uint32  lootCount;                              // 4
        } fish_in_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS= 75
        struct
        {
            uint32  skillLine;                              // 3
            uint32  spellCount;                             // 4
        } learn_skilline_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL = 76
        struct
        {
            uint32  unused;                                 // 3
            uint32  duelCount;                              // 4
        } win_duel;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER = 96
        struct
        {
            uint32  powerType;                              // 3 mana=0, 1=rage, 3=energy, 6=runic power
        } highest_power;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT = 97
        struct
        {
            uint32  statType;                               // 3 4=spirit, 3=int, 2=stamina, 1=agi, 0=strength
        } highest_stat;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER = 98
        struct
        {
            uint32  spellSchool;                            // 3
        } highest_spellpower;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING = 100
        struct
        {
            uint32  ratingType;                             // 3
        } highest_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE = 109
        struct
        {
            uint32  lootType;                               // 3 3=fishing, 2=pickpocket, 4=disentchant
            uint32  lootTypeCount;                          // 4
        } loot_type;

        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2 = 110
        struct
        {
            uint32  skillLine;                              // 3
            uint32  spellCount;                             // 4
        } cast_spell2;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE= 112
        struct
        {
            uint32  skillLine;                              // 3
            uint32  spellCount;                             // 4
        } learn_skill_line;

        // ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL = 113
        struct
        {
            uint32  unused;                                 // 4
            uint32  killCount;                              // 5
        } honorable_kill;

		// Structure d'acces générique si le type de haut fait n'est pas connu 
		// (colonne en base 1, pour dbctools)
        struct
        {
            uint32  field3;                                 // 4 main requirement
            uint32  field4;                                 // 5 main requirement count
            uint32  additionalRequirement1_type;            // 6 additional requirement 1 type
            uint32  additionalRequirement1_value;           // 7 additional requirement 1 value
            uint32  additionalRequirement2_type;            // 8 additional requirement 2 type
            uint32  additionalRequirement2_value;           // 9 additional requirement 2 value
        } raw;
    };
	*/ // Sinon 
     // A Coder (Branruz)
	uint32  MainRequirement;                          // 4
    uint32  MainRequirementCount;                     // 5
	uint32  AdditionnalRequirement1_Type;             // 6
    uint32  AdditionnalRequirement1_Value;            // 7
	uint32  AdditionnalRequirement2_Type;             // 8
    uint32  AdditionnalRequirement2_Value;            // 9
    //-----------------
	//char* name;                // 10  enGB
    // unused                  // 11
	char* name;                // 12 Fr
	                           // 13-26 
    uint32  completionFlag;    // 27
	uint32  groupFlag;         // 28
	// char *Descript;         // 29 non c'est uint32 avec 332.11403 (Branruz), Id de quelque chose (335.12340)
    uint32  timeLimit;         // 30 time limit in seconds
    //uint32 unk1;             // 31
};


struct CurrencyTypesEntry
{
	//uint32    ID;
	uint32    ItemId;	// used as index
	//uint32    Category;
	uint32    BitIndex;
};

struct RuneCostEntry
{
	uint32 Id;       // 1
	uint32 blood;    // 2
	uint32 frost;    // 3
	uint32 unholy;   // 4
	uint32 runePowerGain;  // 5
};

struct AreaTriggerEntry
{
	uint32 Id;
	uint32 continentId;
	float base_x;
	float base_y;
	float base_z;
	float radius;
	float box_length;
	float box_width;
	float box_height;
	float box_yaw;
};

struct CharTitlesEntry
{
    uint32  ID;      // 1
	//uint32  unk1;  // 2
	//char *name;    // 3
	//uint32 unk;    // 4
	char *name;		// 5 335.12340 Fr
	//uint32  unk;   // 6 - 19
    //char*  name2   // 20 ;                              
    //uint32  unk3;  // 21 - 36                
    uint32  bit_index; // 37
};


struct SummonPropertiesEntry
{
	uint32 Id;
	uint32 controltype;
	uint32 factionId;
	uint32 type;
	uint32 slot;
	uint32 flags; // Ascent5218
};

struct BankSlotPrice
{
	uint32 Id;
	uint32 Price;
};

struct ItemSetEntry // 335.12340
{
    uint32 id;                  //1
    //char* name;                 //2 enGB
	                            //3
	char* name;                 //4 Fr
    //uint32 unused_shit[15];   //5 - 17
    uint32 flag;                //18
    uint32 itemid[10];          //19 - 26
    //uint32 more_unused_shit[7]; //29 - 35
    uint32 SpellID[8];          //36 - 43
    uint32 itemscount[8];       //36 - 43
    uint32 RequiredSkillID;     //52
    uint32 RequiredSkillAmt;    //53
};

struct BarberShopStyleEntry
{
    uint32  id;          // 1
    uint32  type;        // 2 
	//char* name;        // 3
    // unused            // 4
	char* name;          // 5 Fr
	// unused            // 6 - 37 ;
    uint32  race;        // 38
    uint32  gender;      // 39
    uint32  hair_id;     // 40
};

struct LockEntry
{
    uint32 Id;          // 1
    uint32 locktype[5]; // 2 - 6 // 0 - no lock, 1 - item needed for lock, 2 - min lockping skill needed
    //uint32 unused     // 7 - 9
    uint32 lockmisc[5]; // 10 - 14 // if type is 1 here is a item to unlock, else is unknow for now
    //uint32 unused     // 15 - 17
    uint32 minlockskill[5]; // 18 - 22 // min skill in lockpiking to unlock.
    //uint32 unused    // 23 - 33
};

struct EmoteTextEntry
{
    uint32 Id;      // 1
    //uint32 name;  // 2
    uint32 textid;  // 3
    uint32 textid2; // 4
    uint32 textid3; // 5
    uint32 textid4; // 6
    //uint32 unk1;  // 7
    uint32 textid5; // 8
    //uint32 unk2;  // 9
    uint32 textid6; // 10
    //uint32 unk3;
    uint32 textid7; // 12
    uint32 textid8; // 13
    //uint32 unk6;
    //uint32 unk7;
    uint32 textid9; // 16
    //uint32 unk9;
    //uint32 unk10;
    //uint32 unk11;
};

struct skilllinespell //SkillLineAbility.dbc
{
    uint32 Id;                       // 1
    uint32 skilline;                 // 2
    uint32 spell;                    // 3 
    uint32 raceMask;                 // 4 // Val = 16 bits
    uint32 classMask;                // 5 // Val = 16 bits
    uint32 excludeRace;              // 6
    uint32 excludeClass;             // 7
    uint32 minSkillLineRank;         // 8
    uint32 supercededBySpell;        // 9  // SpellId ?
    uint32 acquireMethod;            // 10
    uint32 trivialSkillLineRankHigh; // 11
    uint32 trivialSkillLineRankLow;  // 12
    uint32 abandonable;              // 13
    uint32 reqTP;                    // 14
};
//                                   11111111112222222222333333333
//                          12345678901234567890123456789012345678
//                     335 "uxuuuuuuuuuuuuxxsxxxxxxxxxxxxxxuuuuuuu"
struct EnchantEntry 
{
    uint32 Id;                 //  1
	//uint32 Unk2;               //  2
    uint32 type[3];            //  3 -  5 // Valeur = 8 Max en 332.11403 - 335.12340
    int32 min[3];              //  6 -  8 For compat, in practice min==max
    int32 max[3];              //  9 - 11
    uint32 spell[3];           // 12 - 14
    //char* Name;                // 15 enGB
    //uint32 NameAlt;          // 16 
	char* Name;                // 17 Fr
	                           // 18 - 30
    //uint32 NameFlags;        // 31
    uint32 visual;             // 32
    uint32 EnchantGroups;      // 33
    uint32 GemEntry;           // 34
    uint32 unk7;//Gem Related  // 35
	uint32 unk8;//Gem Related  // 36
	uint32 unk9;//Gem Related  // 37
	uint32 LevelMin;           // 38

};

struct GemPropertyEntry{
    uint32 Entry;
    uint32 EnchantmentID;
    uint32 unk1;//bool
    uint32 unk2;//bool
    uint32 SocketMask;
};

struct GlyphPropertyEntry	//GlyphProperties.dbc
{
    uint32 Entry;
    uint32 SpellId;
    uint32 TypeFlag; // 0 = Major, 1 = Minor  + Flag
    uint32 unk; // some flag
};

struct skilllineentry //SkillLine.dbc ( 56 colonnes 335.12340)
{
    uint32 id;             // 1
    uint32 type;           // 2
    uint32 unk1;           // 3
    //char* Name;            // 4
    //int32 NameAlt1;
	char* Name;           // 6 Name Fr
    //uint32 NameAlt2;
    //uint32 NameAlt3;
    //uint32 NameAlt4;
    //uint32 NameAlt5;
    //uint32 NameAlt6;
    //uint32 NameAlt7;
    //uint32 NameAlt8;
    //uint32 NameAlt9;
    //uint32 NameAlt10;
    //uint32 NameAlt11;
    //uint32 NameAlt12;
    //uint32 NameAlt13;
    //uint32 NameAlt14;
    //uint32 NameAlt15;
    //uint32 NameFlags;
    //uint32 Description;
    //uint32 DescriptionAlt1;
    //uint32 DescriptionAlt2;
    //uint32 DescriptionAlt3;
    //uint32 DescriptionAlt4;
    //uint32 DescriptionAlt5;
    //uint32 DescriptionAlt6;
    //uint32 DescriptionAlt7;
    //uint32 DescriptionAlt8;
    //uint32 DescriptionAlt9;
    //uint32 DescriptionAlt10;
    //uint32 DescriptionAlt11;
    //uint32 DescriptionAlt12;
    //uint32 DescriptionAlt13;
    //uint32 DescriptionAlt14;
    //uint32 DescriptionAlt15;
    //uint32 DescriptionFlags; // 38
    //uint32 unk2;

};

// Struct for the entry in Spell.dbc
#define _SPELL_SIGNATURE_	  		 0x5A5AA5A5
#define _SPELL_EFFECT_DBC_COL1_				71 // 332.11403 - 335.12340
#define _SPELL_EFFECTAPPLYAURA_DBC_COL1_	95 // 335.12340 - 101 // 332.11403




struct SpellEntry // 335.12340 en
{
	uint32 Id;                              //1
	uint32 Category;						//2
	uint32 dispelType;                      //3
	uint32 mechanics;                       //4
	uint32 attributes;                      //5
	uint32 attributesEx;                    //6
	uint32 attributesExB;                   //7
	uint32 attributesExC;                   //8 // Flags to
	uint32 attributesExD;                   //9 // Flags....
	uint32 attributesExE;                   //10 // Flags 2.0.1 unknown one
	uint32 attributesExF;					//11
	// Unk322                               // ** 12
	uint32 shapeshiftMask;					//13 // Flags BitMask for shapeshift spells
	// Unk322                               // ** 14
	uint32 shapeshiftExclude;               //15-> this is wrong // Flags BitMask for which shapeshift forms this spell can NOT be used in.
	// Unk322                               // ** 16
	uint32 targets;                         //17 - N / M
	uint32 targetCreatureType;              //18
	uint32 requiresSpellFocus;              //19
	uint32 facingCasterFlags;				//20
	uint32 casterAuraState;                 //21
	uint32 targetAuraState;                 //22
	uint32 excludeCasterAuraState;          //23 2.0.1 unknown two
	uint32 excludeTargetAuraState;          //24 2.0.1 unknown three
	uint32 casterAuraSpell; //casterAura;	//25 you have to have this aura to cast this spell
	uint32 targetAuraSpell; // unk;			//26 unknown flags, used for homing missiles
	uint32 excludeCasterAuraSpell; //excludeCasterAuraId; //27 you can't cast the spell if the caster has this aura
	uint32 excludeTargetAuraSpell; //excludeTargetAuraId; //28 you can't cast the spell if the target has this aura
	uint32 castingTimeIndex;                //29
	uint32 recoveryTime;                    //30
	uint32 categoryRecoveryTime;            //31 recoverytime
	uint32 interruptFlags;                  //32
	uint32 auraInterruptFlags;              //33
	uint32 channelInterruptFlags;           //34
	uint32 procFlags;                       //35
	uint32 procChance;                      //36
	int32 procCharges;                      //37 0 ou 1
	uint32 maxLevel;                        //38
	uint32 baseLevel;                       //39
	uint32 spellLevel;                      //40
	uint32 durationIndex;                   //41
	int32 powerType;                        //42
	uint32 manaCost;                        //43
	uint32 manaCostPerlevel;                //44
	uint32 manaPerSecond;                   //45
	uint32 manaPerSecondPerLevel;           //46
	uint32 rangeIndex;                      //47
	float  speed;                           //48
	uint32 modalNextSpell;                  //49 no need to load this, not used at all
	uint32 cumulativeAura;                  //50
	uint32 totem[2];                        //51 - 52
	uint32 reagent[8];                      //53 - 60
	uint32 reagentCount[8];                 //61 - 68
	int32 EquippedItemClass;				//69
	uint32 EquippedItemSubClass;            //70
	uint32 RequiredItemFlags;               //71
	uint32 Effect[3];                       //72 - 74 -> _SPELL_EFFECT_DBC_COL1_ = (col-1)
	uint32 EffectDieSides[3];               //75 - 77
	//uint32 EffectBaseDice[3];               //78 - 80 Disparu - 335.12340
	//float  EffectDicePerLevel[3];           //81 - 83 Disparu - 335.12340
	float  EffectRealPointsPerLevel[3];     //78 - 80
	int32  EffectBasePoints[3];             //81 - 83
	int32  EffectMechanic[3];               //84 - 86       Related to SpellMechanic.dbc
	uint32 EffectImplicitTargetA[3];        //87 - 89
	uint32 EffectImplicitTargetB[3];        //90 - 92
	uint32 EffectRadiusIndex[3];            //93 - 95
	uint32 EffectApplyAuraName[3];          //96 - 98 -> _SPELL_EFFECTAPPLYAURA_DBC_COL1_ = (col-1)
	uint32 EffectAmplitude[3];              //99 - 101
	float  EffectMultipleValue[3];          //102 - 104     This value is the $ value from description
	uint32 EffectChainTarget[3];            //105 - 107
	uint32 EffectItemType[3];				//108 - 110     Andy: This isn't the relation field, its the item type!
	uint32 EffectMiscValue[3];              //111 - 113
	uint32 EffectMiscValueB[3];             //114 - 116
	uint32 EffectTriggerSpell[3];           //117 - 119
	/*float*/ uint32  EffectPointsPerComboPoint[3];    //120 - 122 // c'est un uint32, verifié ok 335.12340 (Branruz)
	uint32 SpellEffectMaskA[3];				//123 - 125
	uint32 SpellEffectMaskB[3];				//126 - 128
	uint32 SpellEffectMaskC[3];				//129 - 131
	uint32 SpellVisual;                     //132
	uint32 unused_field;                    //133
	uint32 dummy;                           //134
	uint32 CoSpell;                         //135  activeIconID;
	uint32 spellPriority;                   //136
	char* Name;                             //137 - enGB
	//uint32 unused;                        //138
	//char* Name;                             //139 335.12340 Fr
	//uint32 unused;                        //140 - 153
	char * Rank;                            //154 en GB
	//uint32 unused;                        //155
	//char * Rank;                          //156 Fr
	//uint32 unused;                        //157 - 170
	char * Description;                     //171 enGB
	//uint32 unused;                        //172 
	//char * Description;                     //173 Fr
	//uint32 unused;                        //174 - 187
	char * BuffDescription;                 //188 enGB
	//uint32 unused;                        //189
	//char * BuffDescription;                 //190 Fr
	//uint32 unused;                        //191 - 204
	uint32 ManaCostPercentage;              //205
	uint32 unkflags;                        //206
	uint32 StartrecoveryTime;               //207
	uint32 StartRecoveryCategory;           //208
	uint32 SpellFamilyName;                 //209
	uint32 SpellGroupType[3];				//210 - 212 en fait il s'agirait de 2 uint16 ( genre HighSpellGroupType et LowSpellGroupType ) (Branruz) 332.11403
	uint32 MaxTargets;                      //213
	uint32 defenseType;                     //214   dmg_class Integer      0=None, 1=Magic, 2=Melee, 3=Ranged
	uint32 preventionType;                  //215  0,1,2 related to defenseType I think
	int32 stanceBarOrder;                   //216   related to paladin aura's 
	float effectChainAmplitude[3];          //217 - 225   if the name is correct I dono
	uint32 minFactionID;                    //218   only one spellid:6994 has this value = 369 UNUSED
	uint32 minReputation;                   //219   only one spellid:6994 has this value = 4 UNUSED
	uint32 requiredAuraVision;              //220  3 spells 1 or 2   
	uint32 requiredTotemCategoryID[2];		//221 - 222
	int32 AreaGroupId;				        //223 // signé
	uint32 schoolMask;						//224
	// unused                               						//225 - 228
	uint32 runeCostID;						//229
	// unkfloat[3]                          						//230 - 232
    // unused                               						//233
	uint32 spellDifficultyID;				//234
	

    /// CUSTOM: these fields are used for the modifications made in the world.cpp
	int32  EffectBasePointCalculate[3];  // CUSTOM,EffectBasePoint recalculé avec la formule (Branruz)
	uint16 ExtendedSpellGroupType[3];    // CUSTOM,Extended SpellGroupType (Branruz)
	uint16 UniqueGroup;						//Aura: unique group, Remplacement de l'aura
	uint8 UniqueTargetBased;				//Aura: flags, 1=target based, 2=only one aura can be alive from the caster at any time
	uint8 UniqueGroup2;						//Aura: this is used for paladin blessings, flask/elixirs
	uint32 School;							//this is for the school fixes, keep mask for talents and procs, etc
    uint32 DiminishStatus;                  //
    uint32 proc_interval;                   //!!! CUSTOM, <Fill description for variable>
	float ProcsPerMinute;                   //!!! CUSTOM, number of procs per minute
    uint32 c_is_flags;						//!!! CUSTOM, store spell checks in a static way : isdamageind,ishealing
	uint32 buffIndexType;					//!!! CUSTOM, <Fill description for variable>
    uint32 buffType;                        //!!! CUSTOM, these are related to creating a item through a spell
    uint32 RankNumber;                      //!!! CUSTOM, this protects players from having >1 rank of a spell
    uint32 NameHash;                        //!!! CUSTOM, related to custom spells, summon spell quest related spells
    float base_range_or_radius_sqr;         //!!! CUSTOM, needed for aoe spells most of the time
	uint32 talent_tree;						//!!! CUSTOM,
	uint8 in_front_status;					//!!! CUSTOM,
	bool is_melee_spell;					//!!! CUSTOM,
	bool is_ranged_spell;					//!!! CUSTOM,
	bool spell_can_crit;					//!!! CUSTOM,
    //uint32 EffectSpellGroupRelation_high[3];     //!!! this is not contained in client dbc but server must have it
	uint32 ThreatForSpell;

	uint32 ProcOnNameHash[3];

	// love me or hate me, all "In a cone in front of the caster" spells don't necessarily mean "in front"
	float cone_width;
	
	//Spell Coefficient
	float casttime_coef;                                    //!!! CUSTOM, faster spell bonus calculation
	uint8 spell_coef_flags;                                //!!! CUSTOM, store flags for spell coefficient calculations
	float fixed_dddhcoef;                                   //!!! CUSTOM, fixed DD-DH coefficient for some spells
	float fixed_hotdotcoef;                                 //!!! CUSTOM, fixed HOT-DOT coefficient for some spells
	float Dspell_coef_override;                             //!!! CUSTOM, overrides any spell coefficient calculation and use this value in DD&DH
	float OTspell_coef_override;							//!!! CUSTOM, overrides any spell coefficient calculation and use this value in HOT&DOT
	
	bool self_cast_only;
	bool apply_on_shapeshift_change;
	bool always_apply;
	
	bool Unique;

	uint32 logsId;	// SpellId used to send log to client for this spell
	uint32 AdditionalAura;
	uint32 forced_creature_target;
	uint32 AreaAuraTarget;
	uint32 poison_type;     //poisons type...
	uint32 spell_signature; // Fix me "les memory-leak" (Branruz)
};

struct ItemExtendedCostEntry // 322
{
    uint32 costid;   // 1
    uint32 honor;    // 2
    uint32 arena;    // 3
	//  Unk          // 4 
    uint32 item[5];  // 5 - 9
    uint32 count[5]; // 10 - 14
	uint32 costsell; // 15 CostSell ?? 
	// unk           // 16
};

struct TalentEntry
{
    uint32  TalentID;       // 1
    uint32  TalentTree;     // 2
    uint32  Row;            // 3      
    uint32  Col;            // 4
    uint32  RankID[5];      // 5 - 9 
    //uint32  unused;       // 10 - 13
    uint32  DependsOn;      // 14
    //uint32  unused;       // 15 - 16  
    uint32  DependsOnRank;  // 17
    //uint32  unused;       // 18 - 19
    uint32  DependsOnSpell; // 20
	//uint32 unused         // 21 - 23

};

struct TalentTabEntry
{
    uint32	TalentTabID;  // 1 
    //char*	Name;         // 2
	//unit32  unused      // 3 - 19
    uint32	ClassMask;    // 20
	uint32  PetTalentMask; // 21
    //unit32  unused       // 22
    uint32	TabPage;       // 23
    //char*	InternalName; // 24
};

struct Trainerspell
{
    uint32 Id;
    uint32 skilline1;
    uint32 skilline2;
    uint32 skilline3;
    uint32 maxlvl;
    uint32 charclass;
};

struct SpellCastTime
{
    uint32 ID;
    uint32 CastTime; // Lancement
    //uint32 unk1;   // negative ou 0  
    //uint32 unk2;   // inferieur ou egal à CastTime
};

struct SpellRadius
{
    uint32 ID;
    float Radius;
    //float unk1;
    float Radius2;
};

struct SpellRange
{
    uint32 ID;
    float minRange;
	float minRange2;
    float maxRange;
    float maxRange2;
	//uint32 unused; //6
	// char *Name;   // 7
    //uint32 unused; // 8 - 40
};

#define INVOC_DURATION	    0
#define INSTANT_DURATION	1
#define COOLDOWN_DURATION	2

struct SpellDuration
{
    uint32 ID;
    int32 Duration1; // INVOC_DURATION
    int32 Duration2; // INSTANT_DURATION
    int32 Duration3; // COOLDOWN_DURATION
};

struct RandomProps
{
    uint32 ID;
    //uint32 name1;
    uint32 spells[3];
    //uint32 unk1;
    //uint32 unk2;
    //uint32 name2;
    //uint32 RankAlt1;
    //uint32 RankAlt2;
    //uint32 RankAlt3;
    //uint32 RankAlt4;
    //uint32 RankAlt5;
    //uint32 RankAlt6;
    //uint32 RankAlt7;
    //uint32 RankAlt8;
    //uint32 RankAlt9;
    //uint32 RankAlt10;
    //uint32 RankAlt11;
    //uint32 RankAlt12;
    //uint32 RankAlt13;
    //uint32 RankAlt14;
    //uint32 RankAlt15;
    //uint32 RankFlags;

};

struct AreaGroup // 332
{
	uint32  AreaGroupId;		// 0
	uint32  AreaId[7];			// 1-7
};

struct AreaTable // 335.12340
{
    uint32 AreaId;			// 1
    uint32 mapId;			// 2
    uint32 ZoneId;			// 3
    uint32 explorationFlag;	// 4
    uint32 AreaFlags;		// 5
    //uint32 unk2;			// 6
    //uint32 unk3;			// 7
    //uint32 unk4;			// 8
    uint32 EXP;//not XP		// 9
    //uint32 unk5;			// 10
    uint32 level;			// 11
    //char* name;		    // 12 enGB
    //uint32 nameAlt1;		// 13
    char* name;             // 14 name en dbc Fr
    //uint32 nameAlt3;		// 15
    //uint32 nameAlt4;		// 16
    //uint32 nameAlt5;		// 17
    //uint32 nameAlt6;		// 18
    //uint32 nameAlt7;		// 19
    //uint32 nameAlt8;		// 20
    //uint32 nameAlt9;		// 21
    //uint32 nameAlt10;		// 22
    //uint32 nameAlt11;		// 23
    //uint32 nameAlt12;		// 24
    //uint32 nameAlt13;		// 25
    //uint32 nameAlt14;		// 26
    //uint32 nameAlt15;		// 27
    //uint32 nameFlags;		// 28
    uint32 category;		// 29
    //uint32 unk7;			// 30
    //uint32 unk8;			// 31
    //uint32 unk9;			// 32
    //uint32 unk10;			// 33
    //int32 unk11;			// 34
    //float unk12;			// 35
	//uint32 unk13; 		// 36
};

struct FactionTemplateDBC
{
    uint32 ID;                    
    uint32 Faction;
	uint32 FactionFlags;
    uint32 FactionGroup;
    uint32 FriendlyMask;
    uint32 HostileMask;
    uint32 EnemyFactions[4];
    uint32 FriendlyFactions[4];
};

struct AuctionHouseDBC
{
    uint32 id;           // 1
    uint32 unk;          // 2
    uint32 fee;          // 3 
    uint32 tax;          // 4
    //char* name;        // 5
    //char* nameAlt1;    // 6
    //char* nameAlt2;    // 7
    //char* nameAlt3;    // 8
    //char* nameAlt4;    // 9
    //char* nameAlt5;    // 10
    //char* nameAlt6;    // 11
    //char* nameAlt7;    // 12
    //char* nameAlt8;    // 13
    //char* nameAlt9;    // 14
    //char* nameAlt10;   // 15
    //char* nameAlt11;   // 16
    //char* nameAlt12;   // 17
    //char* nameAlt13;   // 18
    //char* nameAlt14;   // 19
    //char* nameAlt15;   // 20
    //char* nameFlags;   // 21
};

struct FactionDBC
{
    uint32 ID;                // 1
    int32 RepListId;          // 2
    uint32 baseRepMask[4];    // 3 - 6
    //uint32 unk1[4];         // 7 - 10
    int32 baseRepValue[4];    // 11 - 14
    //uint32 unk2[4];         // 15 - 18
    uint32 parentFaction;     // 19
	//uint32 unused           // 20 - 23 
    //char* Name;               // 24  
    //uint32 unused;          // 25
	char* Name;               // 26
	//uint32 unused;          // 27 - 57  
};

struct DBCTaxiNode
{
    uint32 id;
    uint32 mapid;
    float x;
    float y;
    float z;
    //uint32 name;
    //uint32 namealt1;
    uint32 name; // 8 frFR
    //uint32 namealt3;
    //uint32 namealt4;
    //uint32 namealt5;
    //uint32 namealt6;
    //uint32 namealt7;
    //uint32 namealt8;
    //uint32 namealt9;
    //uint32 namealt10;
    //uint32 namealt11;
    //uint32 namealt12;
    //uint32 namealt13;
    //uint32 namealt14;
    //uint32 namealt15;
    //uint32 nameflags;
    uint32 horde_mount;
    uint32 alliance_mount;
};

struct DBCTaxiPath
{
    uint32 id;
    uint32 from;
    uint32 to;
    uint32 price; // Etrange ce champ?
};

struct DBCTaxiPathNode // const char* dbctaxipathnodeFormat = "uuuufffuuxx";
{
    uint32 Id;
    uint32 PathId;
    uint32 NodeIndex;
    uint32 ContinentID;
    float LocX;
    float LocY;
    float LocZ;
    uint32 flags;
    uint32 delay;
    uint32 ArrivalEventId;
    uint32 DepartureEventId;
};

struct CreatureSpellDataEntry
{
    uint32 id;
    uint32 Spells[3];
    uint32 PHSpell;
    uint32 Cooldowns[3];
    uint32 PH;
};

struct CharRaceEntry // 335.12340
{
	uint32 RaceId;       // 1 
	                     // 2 - 12
	uint32 CinematicId;  // 13
	uint32 TeamId;       // 14
	char*  RaceName;     // col 17 frFR
	
};

struct CharClassEntry
{
    uint32 class_id;
    //uint32 unk1;
    uint32 power_type;
    //uint32 unk2;
    //char* name;
	//uint32 namealt1;
    char* name; // 7 en Fr
    //uint32 namealt2;
    //uint32 namealt3;
    //uint32 namealt4;
    //uint32 namealt5;
    //uint32 namealt6;
    //uint32 namealt7;
    //uint32 namealt8;
    //uint32 namealt9;
    //uint32 namealt10;
    //uint32 namealt11;
    //uint32 namealt12;
    //uint32 namealt13;
    //uint32 namealt14;
    //uint32 namealt15;
    //uint32 nameflags;
    //uint32 unk3;
    //uint32 unk4;
    //uint32 unk5;
};

struct CreatureDisplayInfo
{
	uint32 ID;		// 1 - id
	//uint32 unk2;	// 2 - ModelData column2?
	//uint32 unk3;	// 3 - ExtraDisplayInfo column 18?
	//uint32 unk4;  // 4
	float Scale;    // 5 
	//uint32 unk6;  // 6
	//char* DisplayTag1; // 7	
	//char* DisplayTag2; // 8
	//char* DisplayTag3; // 9
	//char* DisplayTag4; // 10
	//uint32 unk11;
	//uint32 unk12;
	//uint32 unk13;
	//uint32 unk14;
	//uint32 unk15;
	//uint32 unk16;
};

struct CreatureFamilyEntry
{
    uint32 ID;            // 1
    float minsize;        // 2
    uint32 minlevel;      // 3
    float maxsize;        // 4
    uint32 maxlevel;      // 5
    uint32 skilline;      // 6
    uint32 tameable;	  // 7 - second skill line - 270 Generic
    uint32 petdietflags;  // 8
	int32 pettalenttype;  // 9
                          // 10 - unsused
    char* name;           // 11
    //uint32 namealt1;
    //uint32 namealt2;
    //uint32 namealt3;
    //uint32 namealt4;
    //uint32 namealt5;
    //uint32 namealt6;
    //uint32 namealt7;
    //uint32 namealt8;
    //uint32 namealt9;
    //uint32 namealt10;
    //uint32 namealt11;
    //uint32 namealt12;
    //uint32 namealt13;
    //uint32 namealt14;
    //uint32 namealt15;
    //uint32 nameflags;
    //uint32 iconID; //interface icon
};

struct MapEntry // 3.3.5.12340 Brz
{
    uint32 id;						// 1
    char* name_internal;			// 2
    uint32 map_type;				// 3
									// 4 - unused
	uint32 IsPvPZone;               // 5 - 3.3.2
	//char* real_name;	            // 6 - 3.0.9 enGB
	                                // 7 - unused
    char* real_name;	            // 8 - 335.12340 Fr
	                                // 9 - 22 unused
    uint32 linked_zone;				// 23
	//char* hordeIntro;               // 24 - 3.0.9 enGB
	                                // 25 - 
	char* hordeIntro;               // 26 - 3.3.5.12340 Fr
	                                // 27 - 40 unused
	//char* allianceIntro;            // 41 - 3.0.9 enGB
	                                // 42
	char* allianceIntro;            // 43 - 3.3.5 Fr
	                                // 44 - 57 unused
    uint32 multimap_id;				// 58 
									// 59 - unused
    uint32 parent_map;				// 60 map_id of parent map
    float start_x;					// 61 enter x coord (if single entry)
    float start_y;					// 62 enter y coord (if single entry)
                         			// 63 - unused
	uint32 addon;					// 64 (0-WoW original, 1-tbc expansion -2 WowTLK )
	                                // 65 - unused
	uint32 MaxPlayer;               // 66

};

struct ItemRandomSuffixEntry
{
	uint32 id;                   // 1
    // Suffix                    // 2
	// unused                    // 3 - 18
	// Name                      // 19
	uint32 enchantments[5];      // 20 - 24
	uint32 prefixes[5];          // 25 - 29
};

struct gtFloat
{
	float val;
};

struct CombatRatingDBC
{
	float val;
};

struct ChatChannelDBC
{
	uint32 id;    // 1
	uint32 flags; // 2
	//unused          //3
	//unused          //4
	//unused          //5
	const char* pattern;//6
};

struct DurabilityQualityEntry
{
    uint32 id;
    float quality_modifier;
};

struct DurabilityCostsEntry
{
    uint32 itemlevel;
    uint32 modifier[29];
};

struct SpellShapeshiftForm
{
    uint32 id;         // 1
    // unused          // 2
	// char *Name      // 3
	// unused          // 4
	// char *Name      // 5 Name Fr
	// unused          // 6 - 28
	uint32 spells[7];  // 29 - 35
};

struct VehicleEntry
{
    uint32 m_ID;                     // 1
    uint32 m_flags;                  // 2
    float m_turnSpeed;               // 3
    float m_pitchSpeed;              // 4
    float m_pitchMin;                // 5
    float m_pitchMax;                // 6
    uint32 m_seatID[8];              // 7-14
    float m_mouseLookOffsetPitch;    // 15
    float m_cameraFadeDistScalarMin; // 16
    float m_cameraFadeDistScalarMax; // 17
    float m_cameraPitchOffset;       // 18
    //int m_powerType[3]; // 18-20            // disparu 322 - 332
    //int m_powerToken[3]; // 21-23           // disparu 322 - 332
    float m_facingLimitRight;        // 19
    float m_facingLimitLeft;         // 20
    float m_msslTrgtTurnLingering;   // 21
    float m_msslTrgtPitchLingering;  // 22
	float m_msslTrgtMouseLingering;  // 23
    float m_msslTrgtEndOpacity;      // 24
    float m_msslTrgtArcSpeed;        // 25
    float m_msslTrgtArcRepeat;       // 26
    float m_msslTrgtArcWidth;        // 27
    float m_msslTrgtImpactRadius[2]; // 28-29
    char* m_msslTrgtArcTexture;      // 30
    char* m_msslTrgtImpactTexture;   // 31
    char* m_msslTrgtImpactModel[2];  // 32-33
    float m_cameraYawOffset;         // 34
    uint32 m_uiLocomotionType;       // 35
    float m_msslTrgtImpactTexRadius; // 36
    uint32 m_uiSeatIndicatorType;    // 37
	int m_powerType[3];              // 38-39-40
};
 
struct VehicleSeatEntry // 322
{
    uint32 m_ID; // 0
    uint32 m_flags; // 1
    int32 m_attachmentID; // 2
    float m_attachmentOffsetX; // 3
    float m_attachmentOffsetY; // 4
    float m_attachmentOffsetZ; // 5
    float m_enterPreDelay; // 6
    float m_enterSpeed; // 7
    float m_enterGravity; // 8
    float m_enterMinDuration; // 9
    float m_enterMaxDuration; // 10
    float m_enterMinArcHeight; // 11
    float m_enterMaxArcHeight; // 12
    int32 m_enterAnimStart; // 13
    int32 m_enterAnimLoop; // 14
    int32 m_rideAnimStart; // 15
    int32 m_rideAnimLoop; // 16
    int32 m_rideUpperAnimStart; // 17
    int32 m_rideUpperAnimLoop; // 18
    float m_exitPreDelay; // 19
    float m_exitSpeed; // 20
    float m_exitGravity; // 21
    float m_exitMinDuration; // 22
    float m_exitMaxDuration; // 23
    float m_exitMinArcHeight; // 24
    float m_exitMaxArcHeight; // 25
    int32 m_exitAnimStart; // 26
    int32 m_exitAnimLoop; // 27
    int32 m_exitAnimEnd; // 28
    float m_passengerYaw; // 29
    float m_passengerPitch; // 30
    float m_passengerRoll; // 31
    int32 m_passengerAttachmentID; // 32
    int32 m_vehicleEnterAnim; // 33
    int32 m_vehicleExitAnim; // 34
    int32 m_vehicleRideAnimLoop; // 35
    int32 m_vehicleEnterAnimBone; // 36
    int32 m_vehicleExitAnimBone; // 37
    int32 m_vehicleRideAnimLoopBone; // 38
    float m_vehicleEnterAnimDelay; // 39
    float m_vehicleExitAnimDelay; // 40
    uint32 m_vehicleAbilityDisplay; // 41
    uint32 m_enterUISoundID; // 42
    uint32 m_exitUISoundID; // 43
    int32 m_uiSkin; // 44
    uint32 m_flagsB; // 45

	bool IsUsable() const { return (m_flags & 0x2000000) != 0; }
};

// From Mangos
struct QuestXPLevel
{
    uint32      questLevel;                                 // 0
    uint32      xpIndex[8];                                 // 1-9
    //unk                                                   // 10
};

struct QuestFactionRewardEntry
{
    uint32      id;                                         // 0
    int32       rewardValue[10];                            // 1-10
};
// ----------

// From Arcemu
struct AreaTableEntry
{
	uint32 id; // 0
	int32 rootId; // 1
	int32 adtId; // 2
	int32 groupId; // 3
	//uint32 field4;
	//uint32 field5;
	//uint32 field6;
	//uint32 field7;
	//uint32 field8;
	uint32 flags; // 9
	uint32 areaId; // 10  ref -> AreaTableEntry
	//char Name[16];
	//uint32 nameflags;
};

#pragma pack(pop)

ASCENT_INLINE float GetScale(CreatureDisplayInfo *Scale)
{
	return Scale->Scale;
}

ASCENT_INLINE float GetRadius(SpellRadius *radius)
{
    return radius->Radius;
}
ASCENT_INLINE uint32 GetCastTime(SpellCastTime *time)
{
    return time->CastTime;
}
ASCENT_INLINE float GetMaxRange(SpellRange *range)
{
    return range->maxRange;
}
ASCENT_INLINE float GetMinRange(SpellRange *range)
{
    return range->minRange;
}

ASCENT_INLINE int32 GetDuration(SpellDuration *dur, uint32 _type)
{
	//return dur->Duration1;
    // Test (Branruz)
	switch(_type)
	{
	 case INVOC_DURATION    : return(dur->Duration1);
	 case INSTANT_DURATION  : return(dur->Duration2);
	 case COOLDOWN_DURATION : return(dur->Duration3);
	 default:
		 printf("GetDuration: Type inconnu %u <- Report this to devs.\n",_type);
		 return(2000);
	}
	/*
	int32 maxduration;

	if(dur->Duration1 > dur->Duration2 ) maxduration = dur->Duration1;
	else                                 maxduration = dur->Duration2;

	if(maxduration < dur->Duration3 ) maxduration = dur->Duration3;

	return maxduration;
	*/
}

#define SAFE_DBC_CODE_RETURNS			/* undefine this to make out of range/nulls return null. */

template<class T>
class SERVER_DECL DBCStorage
{
	T * m_heapBlock;
	T * m_firstEntry;

	T ** m_entries;
	uint32 m_max;
	uint32 m_numrows;
	uint32 m_stringlength;
	char * m_stringData;

public:

	class iterator{
    private:
        T *p;
    public:
        iterator(T* ip = 0) : p(ip){ }
        iterator& operator++(){ ++p; return *this; }
		iterator& operator--(){ --p; return *this; }
        bool operator!=(const iterator &i){ return (p != i.p); }
        T* operator*(){ return p; }
    };

    iterator begin(){ 
        return iterator(&m_heapBlock[0]);
    }
    iterator end(){ 
        return iterator(&m_heapBlock[m_numrows]); 
    }

	DBCStorage()
	{
		m_heapBlock = NULL;
		m_entries = NULL;
		m_firstEntry = NULL;
		m_max = 0;
		m_numrows = 0;
		m_stringlength=0;
		m_stringData = NULL;
	}

	~DBCStorage()
	{
		if(m_heapBlock)
			free(m_heapBlock);
		if(m_entries)
			free(m_entries);
		if( m_stringData != NULL )
			free(m_stringData);
	}

	bool Load(const char * filename, const char * format, bool load_indexed, bool load_strings)
	{
		uint32 rows;
		uint32 cols;
		uint32 useless_shit;
		uint32 string_length;
		uint32 header;
		uint32 i;
		long pos;

		FILE * f = fopen(filename, "rb");
		if(f == NULL)
			return false;

		/* read the number of rows, and allocate our block on the heap */
		fread(&header,4,1,f);
		fread(&rows, 4, 1, f);
		fread(&cols, 4, 1, f);
		fread(&useless_shit, 4, 1, f);
		fread(&string_length, 4, 1, f);
		pos = ftell(f);

#ifdef USING_BIG_ENDIAN
		swap32(&rows); swap32(&cols); swap32(&useless_shit); swap32(&string_length);
#endif

		if( load_strings )
		{
			fseek( f, 20 + ( rows * cols * 4 ), SEEK_SET );
			m_stringData = (char*)malloc(string_length);
			m_stringlength = string_length;
			fread( m_stringData, string_length, 1, f );
		}

		fseek(f, pos, SEEK_SET);

		m_heapBlock = (T*)malloc(rows * sizeof(T));
		ASSERT(m_heapBlock);

		/* read the data for each row */
		for(i = 0; i < rows; ++i)
		{
			memset(&m_heapBlock[i], 0, sizeof(T));
			ReadEntry(f, &m_heapBlock[i], format, cols, filename);

			if(load_indexed)
			{
				/* all the time the first field in the dbc is our unique entry */
				if(*(uint32*)&m_heapBlock[i] > m_max)
					m_max = *(uint32*)&m_heapBlock[i];
			}
		}

		if(load_indexed)
		{
			m_entries = (T**)malloc(sizeof(T*) * (m_max+1));
			ASSERT(m_entries);

			memset(m_entries, 0, (sizeof(T*) * (m_max+1)));
			for(i = 0; i < rows; ++i)
			{
				if(m_firstEntry == NULL)
					m_firstEntry = &m_heapBlock[i];

				m_entries[*(uint32*)&m_heapBlock[i]] = &m_heapBlock[i];
			}
		}

		m_numrows = rows;

		fclose(f);
		return true;
	}

	void ReadEntry(FILE * f, T * dest, const char * format, uint32 cols, const char * filename)
	{
		const char * t = format;
		uint32 * dest_ptr = (uint32*)dest;
		uint32 c = 0;
		uint32 val;
		size_t len = strlen(format);
		if(len!= cols)
			printf("!!! possible invalid format in file %s (us: %u, them: %u)\n", filename, len, cols);

		while(*t != 0)
		{
			if((++c) > cols)
			{
				++t;
				printf("!!! Read buffer overflow in DBC reading of file %s\n", filename);
				continue;
			}

			fread(&val, 4, 1, f);
			if(*t == 'x')
			{
				++t;
				continue;		// skip!
			}
#ifdef USING_BIG_ENDIAN
			swap32(&val);
#endif
			if(*t == 's')
			{
				char ** new_ptr = (char**)dest_ptr;
				static const char * null_str = "";
				char * ptr;
				if( val < m_stringlength )
				{
					ptr = m_stringData + val;
					// Filtre Unicode vers utf8 (Lecture zone de texte des DBCs Fr)
                    // ChangeUnicode2ExtAscii(ptr); 
					//------------
				}
				else
					ptr = (char*)null_str;

				*new_ptr = ptr;
				new_ptr++;
				dest_ptr = (uint32*)new_ptr;
			}
			else
			{
				*dest_ptr = val;
				dest_ptr++;
			}

			++t;
		}
	}

	//-----------------------------------------------------------------------------------------
	// Changement de la zone de texte en UTF8 (prob de lecture des zones de texte sur les DBCs Fr)
    void ChangeUnicode2ExtAscii(char *zone_text_dbc)
    {
     uint32 _ind_,size_text;

	 size_text = (uint32)strlen(zone_text_dbc);

	 if(size_text < 2) return; // Secu: mauvais alignement en lecture (Patch+Old DBCs)

     for(_ind_=0;_ind_ < (size_text - 1) ;_ind_++)
     {
	  if(*(zone_text_dbc+_ind_)==0x00) break;

      if(*(zone_text_dbc+_ind_)=='Ã') // Unicode detecté
      {
       switch(*(zone_text_dbc+_ind_+1)) // 2em carac
       {
        // Imprimable
        case '¨' : *(zone_text_dbc+_ind_) = 'è' ; break; //
        case '©' : *(zone_text_dbc+_ind_) = 'é' ; break; //
        case 'ª' : *(zone_text_dbc+_ind_) = 'ê' ; break; //
        case '´' : *(zone_text_dbc+_ind_) = 'ô' ; break; //
        case '¢' : *(zone_text_dbc+_ind_) = 'â' ; break;
        case '€' :
        case '‚' : *(zone_text_dbc+_ind_) = 'A' ; break;
        case '®' : *(zone_text_dbc+_ind_) = 'i' ; break;
        case '§' : *(zone_text_dbc+_ind_) = 'ç' ; break;
        case '»' : *(zone_text_dbc+_ind_) = 'û' ; break;
        case '¯' : *(zone_text_dbc+_ind_) = 'ï' ; break;  //
        case '¼' : *(zone_text_dbc+_ind_) = 'ü' ; break;  //
        case '¹' : *(zone_text_dbc+_ind_) = 'ù' ; break;  //
        // non imprimable ( Pas d'accent sur les majuscules! )
		case 0xFFFFFFA0 : // Signed comparaison
        case 0xA0       : *(zone_text_dbc+_ind_) = 'à' ; break;
        case 0xFFFFFF87 : 
		case 0x87       : *(zone_text_dbc+_ind_) = 'C' ; break;
        case 0xFFFFFF8A : 
	    case 0x8A       : *(zone_text_dbc+_ind_) = 'E' ; break;
        case 0xFFFFFF89 : 
        case 0x89       : *(zone_text_dbc+_ind_) = 'E' ; break;

        default: break;
	   } // End of switch
	   // On remplace 2 caracteres par 1, il faut decaler à gauche le reste de 1 caractere
	   for(uint32 i = (_ind_+1); i < (size_text-2);i++) *(zone_text_dbc+_ind_+1) = *(zone_text_dbc+_ind_+2);
	  } // End of if('Ã')
      else if(*(zone_text_dbc+_ind_)=='Å') // 1er caract Unicode
      {
       switch(*(zone_text_dbc+_ind_+1)) // 2eme caract Unicode
       {
        // Imprimable
	    case 'î' :
        case '’' :
        case '“' : *(zone_text_dbc+_ind_)  = 'o'; // remplacement 1er caract ( genre CÅ“ur pour Coeur )
			       *(zone_text_dbc+_ind_+1)= 'e'; // remplacement 2em caract (       Åîl pour oeil
                   _ind_++; // Caract suivant+1
                   break;

        default : break;
	   }
	  } // end of if('Å')
      else if(*(zone_text_dbc+_ind_)=='Â') // Celui la il est tt seul, on le vire.
      {
       for(uint32 i = _ind_; i < (size_text-1);i++) *(zone_text_dbc+_ind_) = *(zone_text_dbc+_ind_+1);
      }
      else if(*(zone_text_dbc+_ind_) == 'â') // Etrange, la combinaison â€™ doit donner un '
      {                          
       if((*(zone_text_dbc+_ind_+1) == '€') && (*(zone_text_dbc+_ind_+2) == '™') )
       {
         *(zone_text_dbc+_ind_)='\'';
		 for(uint32 i = (_ind_+1); i < (size_text-3);i++) *(zone_text_dbc+_ind_+1) = *(zone_text_dbc+_ind_+3);
	   }
	  } // End else if(buff_wdb[i] == 'â')

	 } // End for(_ind_=
	} // End Procedure ChangeUnicode2ExtAscii
    //-----------------------------------------------------------------------------------------

	ASCENT_INLINE uint32 GetNumRows()
	{
		return m_numrows;
	}

	T * LookupEntryForced(uint32 i)
	{
#if 0
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == NULL)
			{
				printf("LookupEntryForced failed for entry %u\n", i);
				return NULL;
			}
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return NULL;
			else
				return &m_heapBlock[i];
		}
#else
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == NULL)
				return NULL;
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return NULL;
			else
				return &m_heapBlock[i];
		}
#endif
	}

	void SetRow(uint32 i, T * t)
	{
		if(i < m_max && m_entries)
			m_entries[i] = t;
	}

#ifdef SAFE_DBC_CODE_RETURNS
	T * LookupEntry(uint32 i)
	{
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == NULL)
				return m_firstEntry;
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return &m_heapBlock[0];
			else
				return &m_heapBlock[i];
		}
	}

	T * LookupRow(uint32 i)
	{
		if(i >= m_numrows)
			return &m_heapBlock[0];
		else
			return &m_heapBlock[i];
	}

#else

	T * LookupEntry(uint32 i)
	{
		if(m_entries)
		{
			if(i > m_max || m_entries[i] == NULL)
				return NULL;
			else
				return m_entries[i];
		}
		else
		{
			if(i >= m_numrows)
				return NULL;
			else
				return m_heapBlock[i];
		}
	}

	T * LookupRow(uint32 i)
	{
		if(i >= m_numrows)
			return NULL;
		else
			return m_heapBlock[i];
	}


#endif
};

extern SERVER_DECL DBCStorage<AreaTriggerEntry> dbcAreaTrigger;
extern SERVER_DECL DBCStorage<GemPropertyEntry> dbcGemProperty;
extern SERVER_DECL DBCStorage<GlyphPropertyEntry> dbcGlyphProperty;
extern SERVER_DECL DBCStorage<ItemSetEntry> dbcItemSet;
extern SERVER_DECL DBCStorage<LockEntry> dbcLock;
extern SERVER_DECL DBCStorage<SpellEntry> dbcSpell;
extern SERVER_DECL DBCStorage<SpellDuration> dbcSpellDuration;
extern SERVER_DECL DBCStorage<SpellRange> dbcSpellRange;
extern SERVER_DECL DBCStorage<SpellShapeshiftForm> dbcSpellShapeshiftForm;
extern SERVER_DECL DBCStorage<EmoteTextEntry> dbcEmoteEntry;
extern SERVER_DECL DBCStorage<SpellRadius> dbcSpellRadius;
extern SERVER_DECL DBCStorage<SpellCastTime> dbcSpellCastTime;
extern SERVER_DECL DBCStorage<AreaGroup> dbcAreaGroup;
extern SERVER_DECL DBCStorage<AreaTable> dbcArea;
extern SERVER_DECL DBCStorage<FactionTemplateDBC> dbcFactionTemplate;
extern SERVER_DECL DBCStorage<FactionDBC> dbcFaction;
extern SERVER_DECL DBCStorage<EnchantEntry> dbcEnchant;
extern SERVER_DECL DBCStorage<RandomProps> dbcRandomProps;
extern SERVER_DECL DBCStorage<skilllinespell> dbcSkillLineSpell;
extern SERVER_DECL DBCStorage<skilllineentry> dbcSkillLine;
extern SERVER_DECL DBCStorage<DBCTaxiNode> dbcTaxiNode;
extern SERVER_DECL DBCStorage<DBCTaxiPath> dbcTaxiPath;
extern SERVER_DECL DBCStorage<DBCTaxiPathNode> dbcTaxiPathNode;
extern SERVER_DECL DBCStorage<AuctionHouseDBC> dbcAuctionHouse;
extern SERVER_DECL DBCStorage<TalentEntry> dbcTalent;
extern SERVER_DECL DBCStorage<TalentTabEntry> dbcTalentTab;
extern SERVER_DECL DBCStorage<CreatureDisplayInfo> dbcCreatureDisplayInfo;
extern SERVER_DECL DBCStorage<CreatureSpellDataEntry> dbcCreatureSpellData;
extern SERVER_DECL DBCStorage<CreatureFamilyEntry> dbcCreatureFamily;
extern SERVER_DECL DBCStorage<CharClassEntry> dbcCharClass;
extern SERVER_DECL DBCStorage<CharRaceEntry> dbcCharRace;
extern SERVER_DECL DBCStorage<MapEntry> dbcMap;
extern SERVER_DECL DBCStorage<ItemExtendedCostEntry> dbcItemExtendedCost;
extern SERVER_DECL DBCStorage<ItemRandomSuffixEntry> dbcItemRandomSuffix;
extern SERVER_DECL DBCStorage<CombatRatingDBC> dbcCombatRating;
extern SERVER_DECL DBCStorage<ChatChannelDBC> dbcChatChannels;
extern SERVER_DECL DBCStorage<DurabilityCostsEntry> dbcDurabilityCosts;
extern SERVER_DECL DBCStorage<DurabilityQualityEntry> dbcDurabilityQuality;
extern SERVER_DECL DBCStorage<BankSlotPrice> dbcBankSlotPrices;
extern SERVER_DECL DBCStorage<BankSlotPrice> dbcStableSlotPrices; //uses same structure as Bank
extern SERVER_DECL DBCStorage<BarberShopStyleEntry> dbcBarberShopStyle;
extern SERVER_DECL DBCStorage<gtFloat> dbcBarberShopPrices;
extern SERVER_DECL DBCStorage<gtFloat> dbcMeleeCrit;
extern SERVER_DECL DBCStorage<gtFloat> dbcMeleeCritBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcSpellCrit;
extern SERVER_DECL DBCStorage<gtFloat> dbcSpellCritBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcManaRegen;
extern SERVER_DECL DBCStorage<gtFloat> dbcManaRegenBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcHPRegen;
extern SERVER_DECL DBCStorage<gtFloat> dbcHPRegenBase;
extern SERVER_DECL DBCStorage<SummonPropertiesEntry> dbcSummonProperties;
extern SERVER_DECL DBCStorage<RuneCostEntry> dbcSpellRuneCost;
extern SERVER_DECL DBCStorage<CurrencyTypesEntry> dbcCurrencyTypes;

// HearthStone
extern SERVER_DECL DBCStorage<AchievementEntry> dbcAchievement;
extern SERVER_DECL DBCStorage<AchievementCriteriaEntry> dbcAchivementCriteria;
extern SERVER_DECL DBCStorage<VehicleEntry> dbcVehicle;
extern SERVER_DECL DBCStorage<VehicleSeatEntry> dbcVehicleSeat;
extern SERVER_DECL DBCStorage<WorldMapOverlayEntry> dbcWorldMapOverlay;
//-----------

// Mangos
extern SERVER_DECL DBCStorage<QuestXPLevel> dbcQuestXPLevel;
extern SERVER_DECL DBCStorage<QuestFactionRewardEntry> dbcQuestFactionReward;

//Arcemu
extern SERVER_DECL DBCStorage<AreaTableEntry> dbcWMOAreaTable;

bool LoadDBCs();
bool LoadRSDBCs();

#endif
