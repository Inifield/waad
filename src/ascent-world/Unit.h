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
#ifndef __UNIT_H
#define __UNIT_H

class AIInterface;
class DynamicObject;

#define MAX_AURAS          96   //56 // 40 buff slots, 16 debuff slots.
#define MAX_POSITIVE_AURAS 40   // 
#define MAX_PASSIVE_AURAS 192   // grep: i mananged to break this.. :p seems we need more

#define UF_TARGET_DIED  1
#define UF_ATTACKING	2 // this unit is attacking it's selection
#define SPELL_MODIFIERS 30
#define NUM_MECHANIC 32
#define SPELL_GROUPS	32 // 96 bits as of 3.0.2 8926, fucking hell, on dirait plutot 2 uint16 en 332.11403 (branruz)
#define MAX_SPELLGROUPTYPE	3

#define UNIT_TYPE_HUMANOID_BIT (1 << (HUMANOID-1)) //should get computed by precompiler ;)

class Aura;
class Spell;
class AIInterface;
class GameObject;
class Vehicle;
class PathFinding;

struct CreatureInfo;
struct FactionTemplateDBC;
struct FactionDBC;

//function procs
typedef std::vector<void*> AuraProcData;
typedef std::vector<Unit*> UnitArray;
typedef struct
{ 
	uint32 school_type;
	int32 full_damage;
	uint32 resisted_damage;
}dealdamage;

struct ReflectSpellSchool
{
	ReflectSpellSchool()
	{
		spellId = 0;
		school = 0;
		chance = 0;
		require_aura_hash = 0;
		infinity = false;
		infront  = false;
	}
	uint32 spellId;
	uint32 school;
	int32 chance;
	int32 require_aura_hash;
	bool infinity;
	bool infront;
};

typedef struct
{
	uint32 spellid;
	uint64 caster;//not yet in use
	int32 amt;
	int32 reflect_pct;
}Absorb;

typedef std::list<Absorb*> SchoolAbsorb;

typedef struct 
{
	uint32 spellid;
	uint32 mindmg;
	uint32 maxdmg;
} OnHitSpell;

struct AreaAura
{
	uint32 auraid;
	Unit* caster;
};

typedef struct
{
	uint32 spell;
	uint32 chance;
	bool self;
	bool deleted;
} onAuraRemove;

enum DeathState
{
	ALIVE = 0,  // Unit is alive and well
	JUST_DIED,  // Unit has JUST died
	CORPSE,	 // Unit has died but remains in the world as a corpse
	DEAD		// Unit is dead and his corpse is gone from the world
};

#define HIGHEST_FACTION = 46 // Ne sert a rien
enum Factions {
	FACTION_BLOODSAIL_BUCCANEERS,    // 0
	FACTION_BOOTY_BAY,               // 1
	FACTION_GELKIS_CLAN_CENTAUR,     // 2 
	FACTION_MAGRAM_CLAN_CENTAUR,     // 3
	FACTION_THORIUM_BROTHERHOOD,     // 4 
	FACTION_RAVENHOLDT,              // 5 
	FACTION_SYNDICATE,               // 6 
	FACTION_GADGETZAN,               // 7
	FACTION_WILDHAMMER_CLAN,         // 8
	FACTION_RATCHET,                 // 9
	FACTION_STEAMWHEEDLE_CARTEL,     // 10
	FACTION_ALLIANCE_,               // 11
	FACTION_HORDE_,                  // 12
	FACTION_ARGENT_DAWN,             // 13
	FACTION_ORGRIMMAR,               // 14
	FACTION_DARKSPEAR_TROLLS,        // 15
	FACTION_THUNDER_BLUFF,           // 16
	FACTION_UNDERCITY,               // 17
	FACTION_GNOMEREGAN_EXILES,       // 18
	FACTION_STORMWIND,               // 19
	FACTION_IRONFORGE,               // 20
	FACTION_DARNASSUS,               // 21
	FACTION_LEATHERWORKING_DRAGON,   // 22
	FACTION_LEATHERWORKING_ELEMENTAL,// 23
	FACTION_LEATHERWORKING_TRIBAL,   // 24
	FACTION_ENGINEERING_GNOME,       // 25
	FACTION_ENGINEERING_GOBLIN,      // 26
	FACTION_WINTERSABER_TRAINERS,    // 27
	FACTION_EVERLOOK,                // 28
	FACTION_BLACKSMITHING_ARMOR,     // 29
	FACTION_BLACKSMITHING_WEAPON,    // 30
	FACTION_BLACKSMITHING_AXE,       // 31
	FACTION_BLACKSMITHING_SWORD,     // 32
	FACTION_BLACKSMITHING_HAMMER,    // 33
	FACTION_CAER_DARROW,             // 34
	//-------------------------
	FACTION_TIMBERMAW_FURBOLGS,     // 35
	FACTION_CENARION_CIRCLE,        // 36
	FACTION_SHATTERSPEAR_TROLLS,    // 37
	FACTION_RAVASAUR_TRAINERS,      // 38
	FACTION_BATTLEGROUND_NEUTRAL,   // 39
	FACTION_STORMPIKE_GUARDS,       // 40
	FACTION_FROSTWOLF_CLAN,         // 41
	FACTION_HYDRAXIAN_WATERLORDS,   // 42
	FACTION_MORO_GAI,               // 43
	FACTION_SHEN_DRALAR,            // 44
	FACTION_SILVERWING_SENTINELS,   // 45
	FACTION_WARSONG_OUTRIDERS       // 46
	//A Voir
/*	FACTION_TIMBERMAW_HOLD,         // 47       
	FACTION_CENARION_CIRCLE,        // 48  
	FACTION_THRALLMAR,              // 49
	FACTION_HONOR_HOLD,             // 50
	FACTION_THE_SHA_TAR,            // 51
	FACTION_STORMPIKE_GUARDS,       // 52
	FACTION_FROSTWOLF_CLAN,         // 53
	FACTION_HYDRAXIAN_WATERLORDS,   // 54
	FACTION_OUTLAND,                // 55
	FACTION_SHEN_DRALAR,            // 56
	FACTION_SILVERWING_SENTINELS,   // 57
	FACTION_WARSONG_OUTRIDERS,      // 58
	FACTION_ALLIANCE_FORCES,        // 59
	FACTION_HORDE_FORCES,           // 60
	FACTION_EXODAR,                 // 61
	FACTION_DARKMOON_FAIRE,         // 62
	FACTION_ZANDALAR_TRIBE,         // 63
	FACTION_THE_DEFILERS,           // 64
	FACTION_THE_LEAGUE_OF_ARATHOR,  // 65
	FACTION_BROOD_OF_NOZDORMU,      // 66
	FACTION_SILVERMOON_CITY,        // 67
	FACTION_TRANQUILLIEN,           // 68
	FACTION_THE_SCALE_OF_THE_SANDS, // 69
	FACTION_THE_ALDOR,              // 70
	FACTION_SHATTRATH_CITY,         // 71
	FACTION_THE_CONSORTIUM,         // 72
	FACTION_THE_MAG_HAR,            // 73
	FACTION_THE_SCRYERS,            // 74
	FACTION_THE_VIOLET_EYE,         // 75
	FACTION_CENARION_EXPEDITION,    // 76
	FACTION_SPOREGGAR,              // 77
	FACTION_KURENAI,                // 78
	FACTION_KEEPERS_OF_TIME,        // 79
	FACTION_FRIENDLY_HIDDEN,        // 80
	FACTION_LOWER_CITY,             // 81
	FACTION_ASHTONGUE_DEATHSWORN,   // 82
	FACTION_NETHERWING,             // 83
	FACTION_SHATARI_SKYGUARD,       // 84
	FACTION_OGRI_LA,                // 85
	FACTION_SHATTERED_SUN_OFFENSIVE = 86
	*/
};
typedef enum
{
	TEXTEMOTE_AGREE			= 1,
	TEXTEMOTE_AMAZE			= 2,
	TEXTEMOTE_ANGRY			= 3,
	TEXTEMOTE_APOLOGIZE		= 4,
	TEXTEMOTE_APPLAUD		  = 5,
	TEXTEMOTE_BASHFUL		  = 6,
	TEXTEMOTE_BECKON		   = 7,
	TEXTEMOTE_BEG			  = 8,
	TEXTEMOTE_BITE			 = 9,
	TEXTEMOTE_BLEED			= 10,
	TEXTEMOTE_BLINK			= 11,
	TEXTEMOTE_BLUSH			= 12,
	TEXTEMOTE_BONK			 = 13,
	TEXTEMOTE_BORED			= 14,
	TEXTEMOTE_BOUNCE		   = 15,
	TEXTEMOTE_BRB			  = 16,
	TEXTEMOTE_BOW			  = 17,
	TEXTEMOTE_BURP			 = 18,
	TEXTEMOTE_BYE			  = 19,
	TEXTEMOTE_CACKLE		   = 20,
	TEXTEMOTE_CHEER			= 21,
	TEXTEMOTE_CHICKEN		  = 22,
	TEXTEMOTE_CHUCKLE		  = 23,
	TEXTEMOTE_CLAP			 = 24,
	TEXTEMOTE_CONFUSED		 = 25,
	TEXTEMOTE_CONGRATULATE	 = 26,
	TEXTEMOTE_COUGH			= 27,
	TEXTEMOTE_COWER			= 28,
	TEXTEMOTE_CRACK			= 29,
	TEXTEMOTE_CRINGE		   = 30,
	TEXTEMOTE_CRY			  = 31,
	TEXTEMOTE_CURIOUS		  = 32,
	TEXTEMOTE_CURTSEY		  = 33,
	TEXTEMOTE_DANCE			= 34,
	TEXTEMOTE_DRINK			= 35,
	TEXTEMOTE_DROOL			= 36,
	TEXTEMOTE_EAT			  = 37,
	TEXTEMOTE_EYE			  = 38,
	TEXTEMOTE_FART			 = 39,
	TEXTEMOTE_FIDGET		   = 40,
	TEXTEMOTE_FLEX			 = 41,
	TEXTEMOTE_FROWN			= 42,
	TEXTEMOTE_GASP			 = 43,
	TEXTEMOTE_GAZE			 = 44,
	TEXTEMOTE_GIGGLE		   = 45,
	TEXTEMOTE_GLARE			= 46,
	TEXTEMOTE_GLOAT			= 47,
	TEXTEMOTE_GREET			= 48,
	TEXTEMOTE_GRIN			 = 49,
	TEXTEMOTE_GROAN			= 50,
	TEXTEMOTE_GROVEL		   = 51,
	TEXTEMOTE_GUFFAW		   = 52,
	TEXTEMOTE_HAIL			 = 53,
	TEXTEMOTE_HAPPY			= 54,
	TEXTEMOTE_HELLO			= 55,
	TEXTEMOTE_HUG			= 56,
	TEXTEMOTE_HUNGRY		= 57,
	TEXTEMOTE_KISS			= 58,
	TEXTEMOTE_KNEEL			= 59,
	TEXTEMOTE_LAUGH			= 60,
	TEXTEMOTE_LAYDOWN		  = 61,
	TEXTEMOTE_MASSAGE		  = 62,
	TEXTEMOTE_MOAN			 = 63,
	TEXTEMOTE_MOON			 = 64,
	TEXTEMOTE_MOURN			= 65,
	TEXTEMOTE_NO			   = 66,
	TEXTEMOTE_NOD			  = 67,
	TEXTEMOTE_NOSEPICK		 = 68,
	TEXTEMOTE_PANIC			= 69,
	TEXTEMOTE_PEER			 = 70,
	TEXTEMOTE_PLEAD			= 71,
	TEXTEMOTE_POINT			= 72,
	TEXTEMOTE_POKE			 = 73,
	TEXTEMOTE_PRAY			 = 74,
	TEXTEMOTE_ROAR			 = 75,
	TEXTEMOTE_ROFL			 = 76,
	TEXTEMOTE_RUDE			 = 77,
	TEXTEMOTE_SALUTE		   = 78,
	TEXTEMOTE_SCRATCH		  = 79,
	TEXTEMOTE_SEXY			 = 80,
	TEXTEMOTE_SHAKE			= 81,
	TEXTEMOTE_SHOUT			= 82,
	TEXTEMOTE_SHRUG			= 83,
	TEXTEMOTE_SHY			  = 84,
	TEXTEMOTE_SIGH			 = 85,
	TEXTEMOTE_SIT			  = 86,
	TEXTEMOTE_SLEEP			= 87,
	TEXTEMOTE_SNARL			= 88,
	TEXTEMOTE_SPIT			 = 89,
	TEXTEMOTE_STARE			= 90,
	TEXTEMOTE_SURPRISED		= 91,
	TEXTEMOTE_SURRENDER		= 92,
	TEXTEMOTE_TALK			 = 93,
	TEXTEMOTE_TALKEX		   = 94,
	TEXTEMOTE_TALKQ			= 95,
	TEXTEMOTE_TAP			  = 96,
	TEXTEMOTE_THANK			= 97,
	TEXTEMOTE_THREATEN		 = 98,
	TEXTEMOTE_TIRED			= 99,
	TEXTEMOTE_VICTORY		  = 100,
	TEXTEMOTE_WAVE			 = 101,
	TEXTEMOTE_WELCOME		  = 102,
	TEXTEMOTE_WHINE			= 103,
	TEXTEMOTE_WHISTLE		  = 104,
	TEXTEMOTE_WORK			 = 105,
	TEXTEMOTE_YAWN			 = 106,
	TEXTEMOTE_BOGGLE		   = 107,
	TEXTEMOTE_CALM			 = 108,
	TEXTEMOTE_COLD			 = 109,
	TEXTEMOTE_COMFORT		  = 110,
	TEXTEMOTE_CUDDLE		   = 111,
	TEXTEMOTE_DUCK			 = 112,
	TEXTEMOTE_INSULT		   = 113,
	TEXTEMOTE_INTRODUCE		= 114,
	TEXTEMOTE_JK			   = 115,
	TEXTEMOTE_LICK			 = 116,
	TEXTEMOTE_LISTEN		   = 117,
	TEXTEMOTE_LOST			 = 118,
	TEXTEMOTE_MOCK			 = 119,
	TEXTEMOTE_PONDER		   = 120,
	TEXTEMOTE_POUNCE		   = 121,
	TEXTEMOTE_PRAISE		   = 122,
	TEXTEMOTE_PURR			 = 123,
	TEXTEMOTE_PUZZLE		   = 124,
	TEXTEMOTE_RAISE			= 125,
	TEXTEMOTE_READY			= 126,
	TEXTEMOTE_SHIMMY		   = 127,
	TEXTEMOTE_SHIVER		   = 128,
	TEXTEMOTE_SHOO			 = 129,
	TEXTEMOTE_SLAP			 = 130,
	TEXTEMOTE_SMIRK			= 131,
	TEXTEMOTE_SNIFF			= 132,
	TEXTEMOTE_SNUB			 = 133,
	TEXTEMOTE_SOOTHE		   = 134,
	TEXTEMOTE_STINK			= 135,
	TEXTEMOTE_TAUNT			= 136,
	TEXTEMOTE_TEASE			= 137,
	TEXTEMOTE_THIRSTY		  = 138,
	TEXTEMOTE_VETO			 = 139,
	TEXTEMOTE_SNICKER		  = 140,
	TEXTEMOTE_STAND			= 141,
	TEXTEMOTE_TICKLE		   = 142,
	TEXTEMOTE_VIOLIN		   = 143,
	TEXTEMOTE_SMILE			= 163,
	TEXTEMOTE_RASP			 = 183,
	TEXTEMOTE_PITY			 = 203,
	TEXTEMOTE_GROWL			= 204,
	TEXTEMOTE_BARK			 = 205,
	TEXTEMOTE_SCARED		   = 223,
	TEXTEMOTE_FLOP			 = 224,
	TEXTEMOTE_LOVE			 = 225,
	TEXTEMOTE_MOO			  = 226,
	TEXTEMOTE_COMMEND		  = 243,
	TEXTEMOTE_JOKE			 = 329
} TextEmoteType;

typedef enum // 335.12340 (emotes.dbc)
{
EMOTE_ONESHOT_NONE				=	0	,
EMOTE_ONESHOT_TALK				=	1	, // (DNR)
EMOTE_ONESHOT_BOW				=	2	,
EMOTE_ONESHOT_WAVE				=	3	, // (DNR)
EMOTE_ONESHOT_CHEER				=	4	, // (DNR)
EMOTE_ONESHOT_EXCLAMATION		=	5	, // (DNR)
EMOTE_ONESHOT_QUESTION			=	6	,
EMOTE_ONESHOT_EAT				=	7	,
EMOTE_STATE_DANCE				=	10	,
EMOTE_ONESHOT_LAUGH				=	11	,
EMOTE_STATE_SLEEP				=	12	,
EMOTE_STATE_SIT					=	13	,
EMOTE_ONESHOT_RUDE				=	14	, // (DNR)	
EMOTE_ONESHOT_ROAR				=	15	, // (DNR)
EMOTE_ONESHOT_KNEEL				=	16	,
EMOTE_ONESHOT_KISS				=	17	,
EMOTE_ONESHOT_CRY				=	18	,
EMOTE_ONESHOT_CHICKEN			=	19	,
EMOTE_ONESHOT_BEG				=	20	,
EMOTE_ONESHOT_APPLAUD			=	21	,
EMOTE_ONESHOT_SHOUT				=	22	, // (DNR)
EMOTE_ONESHOT_FLEX				=	23	,
EMOTE_ONESHOT_SHY				=	24	, // (DNR)
EMOTE_ONESHOT_POINT				=	25	, // (DNR)
EMOTE_STATE_STAND				=	26	,
EMOTE_STATE_READYUNARMED		=	27	,
EMOTE_STATE_WORK_SHEATHED		=	28	,
EMOTE_STATE_POINT				=	29	, // (DNR)
EMOTE_STATE_NONE				=	30	,
EMOTE_ONESHOT_WOUND				=	33	,
EMOTE_ONESHOT_WOUNDCRITICAL		=	34	,
EMOTE_ONESHOT_ATTACKUNARMED		=	35	,
EMOTE_ONESHOT_ATTACK1H			=	36	,
EMOTE_ONESHOT_ATTACK2HTIGHT		=	37	,
EMOTE_ONESHOT_ATTACK2HLOOSE		=	38	,
EMOTE_ONESHOT_PARRYUNARMED		=	39	,
EMOTE_ONESHOT_PARRYSHIELD		=	43	,
EMOTE_ONESHOT_READYUNARMED		=	44	,
EMOTE_ONESHOT_READY1H			=	45	,
EMOTE_ONESHOT_READYBOW			=	48	,
EMOTE_ONESHOT_SPELLPRECAST		=	50	,
EMOTE_ONESHOT_SPELLCAST			=	51	,
EMOTE_ONESHOT_BATTLEROAR		=	53	,
EMOTE_ONESHOT_SPECIALATTACK1H	=	54	,
EMOTE_ONESHOT_KICK				=	60	,
EMOTE_ONESHOT_ATTACKTHROWN		=	61	,
EMOTE_STATE_STUN				=	64	,
EMOTE_STATE_DEAD				=	65	,
EMOTE_ONESHOT_SALUTE			=	66	,
EMOTE_STATE_KNEEL				=	68	, 
EMOTE_STATE_USESTANDING			=	69	,
EMOTE_ONESHOT_WAVE_NOSHEATHE	=	70	,
EMOTE_ONESHOT_CHEER_NOSHEATHE	=	71	,
EMOTE_ONESHOT_EAT_NOSHEATHE		=	92	,
EMOTE_STATE_STUN_NOSHEATHE		=	93	,
EMOTE_ONESHOT_DANCE				=	94	,
EMOTE_ONESHOT_SALUTE_NOSHEATH	=	113	,
EMOTE_STATE_USESTANDING_NOSHEATHE	=	133	,
EMOTE_ONESHOT_LAUGH_NOSHEATHE	=	153	,
EMOTE_STATE_WORK			=	173	,
EMOTE_STATE_SPELLPRECAST	=	193	,
EMOTE_ONESHOT_READYRIFLE	=	213	,
EMOTE_STATE_READYRIFLE		=	214	,
EMOTE_STATE_WORK_MINING		=	233	,
EMOTE_STATE_WORK_CHOPWOOD	=	234	,
EMOTE_STATE_APPLAUD			=	253	,
EMOTE_ONESHOT_LIFTOFF		=	254	,
EMOTE_ONESHOT_YES			=	273	, // (DNR)	
EMOTE_ONESHOT_NO			=	274	, // (DNR)	
EMOTE_ONESHOT_TRAIN			=	275	, // (DNR)	
EMOTE_ONESHOT_LAND			=	293	,
EMOTE_STATE_AT_EASE			=	313	,
EMOTE_STATE_READY1H			=	333	,
EMOTE_STATE_SPELLKNEELSTART	=	353	,
EMOTE_STAND_STATE_SUBMERGED	=	373	,
EMOTE_ONESHOT_SUBMERGE		=	374	,
EMOTE_STATE_READY2H			=	375	,
EMOTE_STATE_READYBOW		=	376	,
EMOTE_ONESHOT_MOUNTSPECIAL	=	377	,
EMOTE_STATE_TALK			=	378	,
EMOTE_STATE_FISHING			=	379	,
EMOTE_ONESHOT_FISHING		=	380	,
EMOTE_ONESHOT_LOOT			=	381	,
EMOTE_STATE_WHIRLWIND		=	382	,
EMOTE_STATE_DROWNED			=	383	,
EMOTE_STATE_HOLD_BOW		=	384	,
EMOTE_STATE_HOLD_RIFLE		=	385	,
EMOTE_STATE_HOLD_THROWN		=	386	,
EMOTE_ONESHOT_DROWN			=	387	,
EMOTE_ONESHOT_STOMP			=	388	,
EMOTE_ONESHOT_ATTACKOFF		=	389	,
EMOTE_ONESHOT_ATTACKOFFPIERCE =	390	,
EMOTE_STATE_ROAR			=	391	,
EMOTE_STATE_LAUGH			=	392	,
EMOTE_ONESHOT_CREATURE_SPECIAL =	393	,
EMOTE_ONESHOT_JUMPLANDRUN	=	394	,
EMOTE_ONESHOT_JUMPEND		=	395	,
EMOTE_ONESHOT_TALK_NOSHEATHE =	396	,
EMOTE_ONESHOT_POINT_NOSHEATHE =	397	,
EMOTE_STATE_CANNIBALIZE		=	398	,
EMOTE_ONESHOT_JUMPSTART		=	399	,
EMOTE_STATE_DANCESPECIAL	=	400	,
EMOTE_ONESHOT_DANCESPECIAL	=	401	,
EMOTE_ONESHOT_CUSTOMSPELL01	=	402	,
EMOTE_ONESHOT_CUSTOMSPELL02	=	403	,
EMOTE_ONESHOT_CUSTOMSPELL03	=	404	,
EMOTE_ONESHOT_CUSTOMSPELL04	=	405	,
EMOTE_ONESHOT_CUSTOMSPELL05	=	406	,
EMOTE_ONESHOT_CUSTOMSPELL06	=	407	,
EMOTE_ONESHOT_CUSTOMSPELL07	=	408	,
EMOTE_ONESHOT_CUSTOMSPELL08	=	409	,
EMOTE_ONESHOT_CUSTOMSPELL09	=	410	,
EMOTE_ONESHOT_CUSTOMSPELL10	=	411	,
EMOTE_STATE_EXCLAIM			=	412	,
EMOTE_STATE_DANCE_CUSTOM	=	413	,
EMOTE_STATE_SIT_CHAIR_MED	=	415	,
EMOTE_STATE_CUSTOM_SPELL_01	=	416	,
EMOTE_STATE_CUSTOM_SPELL_02	=	417	,
EMOTE_STATE_EAT				=	418	,
EMOTE_STATE_CUSTOM_SPELL_04	=	419	,
EMOTE_STATE_CUSTOM_SPELL_03	=	420	,
EMOTE_STATE_CUSTOM_SPELL_05	=	421	,
EMOTE_STATE_SPELLEFFECT_HOLD =	422	,
EMOTE_STATE_EAT_NO_SHEATHE	=	423	,
EMOTE_STATE_MOUNT			=	424	,
EMOTE_STATE_READY2HL		=	425	,
EMOTE_STATE_SIT_CHAIR_HIGH	=	426	,
EMOTE_STATE_FALL			=	427	,
EMOTE_STATE_LOOT			=	428	,
EMOTE_STATE_SUBMERGED		=	429	,
EMOTE_ONESHOT_COWER			=	430	, // (DNR)
EMOTE_STATE_COWER			=	431	,
EMOTE_ONESHOT_USESTANDING	=	432	,
EMOTE_STATE_STEALTH_STAND	=	433	,
EMOTE_ONESHOT_OMNICAST_GHOUL =	434	, // (W/SOUND
EMOTE_ONESHOT_ATTACKBOW		=	435	,
EMOTE_ONESHOT_ATTACKRIFLE	=	436	,
EMOTE_STATE_SWIM_IDLE		=	437	,
EMOTE_STATE_ATTACK_UNARMED	=	438	,
EMOTE_ONESHOT_SPELLCAST_W 	=	439	, // (W/SOUND)
EMOTE_ONESHOT_DODGE			=	440	,
EMOTE_ONESHOT_PARRY1H		=	441	,
EMOTE_ONESHOT_PARRY2H		=	442	,
EMOTE_ONESHOT_PARRY2HL		=	443	,
EMOTE_STATE_FLYFALL			=	444	,
EMOTE_ONESHOT_FLYDEATH		=	445	,
EMOTE_STATE_FLY_FALL		=	446	,
EMOTE_ONESHOT_FLY_SIT_GROUND_DOWN	=	447	,
EMOTE_ONESHOT_FLY_SIT_GROUND_UP	=	448	,
EMOTE_ONESHOT_EMERGE		=	449	,
EMOTE_ONESHOT_DRAGONSPIT	=	450	,
EMOTE_STATE_SPECIALUNARMED	=	451	,
EMOTE_ONESHOT_FLYGRAB		=	452	,
EMOTE_STATE_FLYGRABCLOSED	=	453	,
EMOTE_ONESHOT_FLYGRABTHROWN	=	454	,
EMOTE_STATE_FLY_SIT_GROUND	=	455	,
EMOTE_STATE_WALKBACKWARDS	=	456	,
EMOTE_ONESHOT_FLYTALK		=	457	,
EMOTE_ONESHOT_FLYATTACK1H	=	458	,
EMOTE_STATE_CUSTOMSPELL08	=	459	,
EMOTE_ONESHOT_FLY_DRAGONSPIT =	460	,
EMOTE_STATE_SIT_CHAIR_LOW	=	461	,
EMOTE_ONE_SHOT_STUN			=	462	,
EMOTE_ONESHOT_SPELLCAST_OMNI =	463	,
EMOTE_STATE_READYTHROWN		=	465	,
EMOTE_ONESHOT_WORK_CHOPWOOD	=	466	,
EMOTE_ONESHOT_WORK_MINING	=	467	,
EMOTE_STATE_SPELL_CHANNEL_OMNI	=	468	,
EMOTE_STATE_SPELL_CHANNEL_DIRECTED	=	469	,
EMOTE_STAND_STATE_NONE		=	470	,
EMOTE_STATE_READYJOUST		=	471	,
EMOTE_STATE_STRANGULATE		=	473	,
EMOTE_STATE_READYSPELLOMNI	=	474	,
EMOTE_STATE_HOLD_JOUST		=	475	,
EMOTE_ONESHOT_CRY_PROUD		=	476	, // (JAINA PROUDMOORE ONLY)

} EmoteType;

enum StandState
{
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
    STANDSTATE_FLAG_ALL          = 0xFF000000,
};

enum UnitSpecialStates
{
	UNIT_STATE_NORMAL    = 0x0000,
	UNIT_STATE_DISARMED  = 0X0001,
	UNIT_STATE_CHARM     = 0x0002,
	UNIT_STATE_FEAR      = 0x0004,
	UNIT_STATE_ROOT      = 0x0008,
	UNIT_STATE_SLEEP     = 0x0010,  // never set
	UNIT_STATE_SNARE     = 0x0020,  // never set
	UNIT_STATE_STUN      = 0x0040,
	UNIT_STATE_KNOCKOUT  = 0x0080,  // not used
	UNIT_STATE_BLEED     = 0x0100,  // not used
	UNIT_STATE_POLYMORPH = 0x0200,  // not used
	UNIT_STATE_BANISH    = 0x0400,  // not used
	UNIT_STATE_CONFUSE   = 0x0800,
	UNIT_STATE_PACIFY    = 0x1000,
	UNIT_STATE_SILENCE   = 0x2000,
	UNIT_STATE_FROZEN    = 0x4000,
};

enum UnitFieldBytes1
{
	U_FIELD_BYTES_ANIMATION_FROZEN = 0x01,
};

enum UnitFieldBytes2
{
	U_FIELD_BYTES_FLAG_PVP     = 0x01,
	U_FIELD_BYTES_SPELL_AURAS  = 0x02, // A Voir (Branruz)
    U_FIELD_BYTES_FLAG_FFA_PVP = 0x04,  
	U_FIELD_BYTES_POS_AURAS    = 0x10,
	
};

enum UnitFieldFlags // UNIT_FIELD_FLAGS #46 - these are client flags
{	//                                            Hex    Bit     Decimal  Comments
	UNIT_FLAG_UNKNOWN_1                  = 0x00000001, // 1            1
	UNIT_FLAG_NOT_ATTACKABLE_2           = 0x00000002, // 2            2  client won't let you attack them
	UNIT_FLAG_LOCK_PLAYER                = 0x00000004, // 3            4  ? does nothing to client (probably wrong) - only taxi code checks this
	UNIT_FLAG_PLAYER_CONTROLLED          = 0x00000008, // 4            8  makes players and NPCs attackable / not attackable
	UNIT_FLAG_UNKNOWN_5                  = 0x00000010, // 5           16  ? some NPCs have this
	UNIT_FLAG_UNKNOWN_6                  = 0x00000020, // 6           32
	UNIT_FLAG_PLUS_MOB                   = 0x00000040, // 7           64  ? some NPCs have this (Rare/Elite/Boss?)
	UNIT_FLAG_UNKNOWN_8                  = 0x00000080, // 8          128  ? can change attackable status 
	UNIT_FLAG_NOT_ATTACKABLE_9           = 0x00000100, // 9          256  changes attackable status
	UNIT_FLAG_UNKNOWN_10                 = 0x00000200, // 10         512  ? some NPCs have this
	UNIT_FLAG_LOOTING                    = 0x00000400, // 11        1024
	UNIT_FLAG_SELF_RES                   = 0x00000800, // 12        2048  ? some NPCs have this
	UNIT_FLAG_PVP                        = 0x00001000, // 13        4096  sets PvP flag
	UNIT_FLAG_SILENCED                   = 0x00002000, // 14        8192
	UNIT_FLAG_DEAD                       = 0x00004000, // 15       16384  used for special "dead" NPCs like Withered Corpses
	UNIT_FLAG_UNKNOWN_16                 = 0x00008000, // 16       32768  ? some NPCs have this
	UNIT_FLAG_ALIVE                      = 0x00010000, // 17       65536  ?
	UNIT_FLAG_PACIFIED                   = 0x00020000, // 18      131072
	UNIT_FLAG_STUNNED                    = 0x00040000, // 19      262144
	UNIT_FLAG_COMBAT                     = 0x00080000, // 20      524288  sets combat flag
	UNIT_FLAG_MOUNTED_TAXI               = 0x00100000, // 21     1048576  mounted on a taxi
	UNIT_FLAG_DISARMED                   = 0x00200000, // 22     2097152
	UNIT_FLAG_CONFUSED                   = 0x00400000, // 23     4194304
	UNIT_FLAG_FLEEING                    = 0x00800000, // 24     8388608  fear
	UNIT_FLAG_PLAYER_CONTROLLED_CREATURE = 0x01000000, // 25    16777216
	UNIT_FLAG_NOT_SELECTABLE             = 0x02000000, // 26    33554432  cannot select the unit
	UNIT_FLAG_SKINNABLE                  = 0x04000000, // 27    67108864
	UNIT_FLAG_MOUNT                      = 0x08000000, // 28   134217728  ? was MAKE_CHAR_UNTOUCHABLE (probably wrong), nothing ever set it
	UNIT_FLAG_UNKNOWN_29                 = 0x10000000, // 29   268435456
	UNIT_FLAG_FEIGN_DEATH                = 0x20000000, // 30   536870912
	UNIT_FLAG_SHEATHE                    = 0x40000000, // 31  1073741824  ? was WEAPON_OFF and being used for disarm
	UNIT_FLAG_UNKNOWN_32                 = 0x80000000, // 32  2147483648
};

enum UnitFlags2
{
	UNIT_FLAG2_FEIGN_DEATH				= 0x00000001,
	UNIT_FLAG2_UNK1						= 0x00000002, // Hide unit model (show only player equip)
	UNIT_FLAG2_COMPREHEND_LANG			= 0x00000008,
	UNIT_FLAG2_FORCE_MOVE				= 0x00000040,
	UNIT_FLAG2_REGENERATE_POWER			= 0x00000800
};

enum UnitDynamicFlags
{
	U_DYN_FLAG_LOOTABLE				 = 0x01,
	U_DYN_FLAG_UNIT_TRACKABLE		   = 0x02,
	U_DYN_FLAG_TAGGED_BY_OTHER		  = 0x04,
	U_DYN_FLAG_TAPPED_BY_PLAYER		 = 0x08,
	U_DYN_FLAG_PLAYER_INFO			  = 0x10,
	U_DYN_FLAG_DEAD					 = 0x20,
};

enum DamageFlags
{
	DAMAGE_FLAG_MELEE   = 0x01,
	DAMAGE_FLAG_HOLY	= 0x02,
	DAMAGE_FLAG_FIRE	= 0x04,
	DAMAGE_FLAG_NATURE  = 0x08,
	DAMAGE_FLAG_FROST   = 0x10,
	DAMAGE_FLAG_SHADOW  = 0x20,
	DAMAGE_FLAG_ARCANE  = 0x40
};

enum WeaponDamageType // this is NOT the same as SPELL_ENTRY_defenseType, or Spell::GetType(), or SPELL_ENTRY_School !!
{
	MELEE   = 0,
	OFFHAND = 1,
	RANGED  = 2,
};

enum VisualState
{
	ATTACK = 1,
	DODGE,
	PARRY,
	INTERRUPT,
	BLOCK,
	EVADE,
	IMMUNE,
	DEFLECT
};

enum HitStatus
{
	HITSTATUS_unk			= 0x01,
	HITSTATUS_HITANIMATION  = 0x02,
	HITSTATUS_DUALWIELD     = 0x04,
	HITSTATUS_MISS          = 0x10,
	HITSTATUS_ABSORBED      = 0x20,
	HITSTATUS_RESIST        = 0x40,
	HITSTATUS_CRICTICAL     = 0x80,
	HITSTATUS_BLOCK         = 0x800,
	HITSTATUS_GLANCING      = 0x4000,
	HITSTATUS_CRUSHINGBLOW  = 0x8000,
	HITSTATUS_NOACTION      = 0x10000,
	HITSTATUS_SWINGNOHITSOUND = 0x80000 // as in miss?
};

enum INVIS_FLAG
{
	INVIS_FLAG_NORMAL, // players and units with no special invisibility flags
	INVIS_FLAG_SPIRIT1,
	INVIS_FLAG_SPIRIT2,
	INVIS_FLAG_TRAP,
	INVIS_FLAG_QUEST,
	INVIS_FLAG_GHOST,
	INVIS_FLAG_UNKNOWN6,
	INVIS_FLAG_UNKNOWN7,
	INVIS_FLAG_SHADOWMOON,
	INVIS_FLAG_NETHERSTORM,
	INVIS_FLAG_TOTAL
};

enum FIELD_PADDING//Since this field isnt used you can expand it for you needs
{
	PADDING_NONE
};

struct AuraCheckResponse
{
	uint32 Error;
	uint32 Misc;
};

enum AURA_CHECK_RESULT
{
	AURA_CHECK_RESULT_NONE				  = 1,
	AURA_CHECK_RESULT_HIGHER_BUFF_PRESENT   = 2,
	AURA_CHECK_RESULT_LOWER_BUFF_PRESENT	= 3,
};

enum CUSTOM_TIMERS
{
	CUSTOM_TIMER_CHIMERA_SCORPID,
	CUSTOM_TIMER_ECLIPSE,
	CUSTOM_TIMER_ERADICATION,
	CUSTOM_TIMER_CHEATDEATH,
	CUSTOM_TIMER_RAPTURE,
	NUM_CUSTOM_TIMERS
};
enum Target_Filter // Note Randdrick : �num�ration des diff�rentes trajectoires possibles
{
	TARGET_FILTER_NONE,
	TARGET_FILTER_CLOSEST,
	TARGET_FILTER_FRIENDLY,
	TARGET_FILTER_NOT_CURRENT,
	TARGET_FILTER_WOUNDED,
	TARGET_FILTER_SECOND_MOSTED_HASTED
};
	
typedef std::list<struct ProcTriggerSpellOnSpell> ProcTriggerSpellOnSpellList;

/************************************************************************/
/* "In-Combat" Handler                                                  */
/************************************************************************/

class Unit;
class CombatStatusHandler
{
	typedef set<uint64> AttackerMap;
    //typedef map<uint64, uint32> AttackTMap; 
	typedef set<uint32> HealedSet;		    // Must Be Players!
	HealedSet m_healers;
	HealedSet m_healed;
	Unit* m_Unit;
	bool m_lastStatus;
	//AttackTMap m_attackTargets; 
	AttackerMap m_attackTargets;
	uint64 m_primaryAttackTarget;
	map<uint64,uint32> DamageMap;

public:

	CombatStatusHandler() : m_lastStatus(false), m_primaryAttackTarget(0)
	{
		m_attackers.clear();
		m_healers.clear();
		m_healed.clear();
		m_attackTargets.clear();
		DamageMap.clear();
	}

	AttackerMap m_attackers;
	void AddAttackTarget(const uint64& guid);						// this means we clicked attack, not actually striked yet, so they shouldnt be in combat.
	void ClearPrimaryAttackTarget();								// means we deselected the unit, stopped attacking it.

	void OnDamageDealt(Unit * pTarget);// , uint32 damage);              // this is what puts the other person in combat.
	void WeHealed(Unit * pHealTarget);								// called when a player heals another player, regardless of combat state.

	void RemoveAttacker(Unit * pAttacker, const uint64& guid);		// this means we stopped attacking them totally. could be because of deagro, etc.
	void RemoveAttackTarget(Unit * pTarget);						// means our DoT expired.

	void UpdateFlag();												// detects if we have changed combat state (in/out), and applies the flag.

	ASCENT_INLINE bool IsInCombat() { return m_lastStatus; }				// checks if we are in combat or not.

	void OnRemoveFromWorld();										// called when we are removed from world, kills all references to us.
	
	ASCENT_INLINE void Vanished()
	{
		ClearAttackers();
		ClearHealers();
		DamageMap.clear();
	}

	ASCENT_INLINE const uint64& GetPrimaryAttackTarget() { return m_primaryAttackTarget; }
	ASCENT_INLINE void SetUnit(Unit * p) { m_Unit = p; }
	void UpdateTargets();
	void TryToClearAttackTargets();									// for pvp timeout
	void AttackersForgetHate();										// used right now for Feign Death so attackers go home

protected:
	bool InternalIsInCombat();										// called by UpdateFlag, do not call from anything else!
	bool IsAttacking(Unit * pTarget);								// internal function used to determine if we are still attacking target x.
	void AddAttacker(const uint64& guid);							// internal function to add an attacker
	void RemoveHealed(Unit * pHealTarget);							// usually called only by updateflag
	void ClearHealers();											// this is called on instance change.
	void ClearAttackers();											// means we vanished, or died.
	void ClearMyHealers();
};

class SERVER_DECL UnitChain
{
public:
	UnitChain(bool persist = false, float chainrange = 0.0f) : m_chainrange(chainrange), m_persist(persist) {}
	
	float m_chainrange;
	bool m_persist;
	std::set<Unit*> m_units;
	void AddUnit(Unit* u);
	void RemoveUnit(Unit* u);
};

//====================================================================
//  Unit
//  Base object for Players and Creatures
//====================================================================

class SpellScript;
class Pet;
class AI_Speak;
class SERVER_DECL Unit : public Object
{
public:
	/************************************************************************/
	/* LUA Stuff                                                            */
	/************************************************************************/
/*	typedef struct { const char *name; int(*mfunc)(lua_State*,shared_ptr<Unit>); } RegType;
	static const char className[];
	static RegType methods[];
	
	// a lua script cannot create a unit.
	Unit(lua_State * L) { ASSERT(false); }*/

	virtual void Init();
	 virtual ~Unit ( );

	virtual void OnPositionChange();

	void ReapplyPassiveAuras();

	uint64 m_redirectedThreat;

	void UpdateStats();
	void SetInitialFactionStuff();

	//par sebbu #programmation reseau irc idapnet 1 jav 09
	HM_NAMESPACE::hash_multimap<uint32, ProcFnc*>::iterator upper_bound( HM_NAMESPACE::hash_multimap<uint32, ProcFnc*> hmm, uint32 key);
	
	//new callback hooks, experimental
	HM_NAMESPACE::hash_multimap<uint32, ProcFnc*> m_procfuncs;

	// void FuzzTest();


	

	ASCENT_INLINE void AddProcFnc(uint32 type, ProcFnc* p) { m_procfuncs.insert(std::pair<uint32, ProcFnc*>(type, p)); }
	/*
	all optional, no creator or proctype will erase all,
	creator will erase by creator, proctype will erase by proctype,
	no creator and proctype will erase all of that type,
	creator and no proctype will erase by creator, but will be slower
	please use all arguments if possible, find/upper_bounds
	is quicker then iterating the entire set
	*/
	void DeleteProcFnc(void* creator = NULL, uint32 proctype = 0xFFFFFFFF);

	//Handle a proc function
	void HandleProcFnc(uint32 proctype, std::vector<void*>* data);

	std::multimap<uint32, Aura*> m_aurascast;
	std::set<Creature *> m_summonedCreatures; //needed to get angle for new ones
	bool m_duelImmunity;

	UnitChain* m_chain;

	Unit* m_sharedHPMaster;
	std::set<Unit*> m_sharedHPUnits;
	void ShareHealthWithUnit(Unit* u);

	ASCENT_INLINE void RemoveDuelImmunity() { m_duelImmunity = false; }

	bool InParty(Unit* u);
	bool InRaid(Unit* u);

	ASCENT_INLINE void	SetSummon(Pet *pet) { m_Summon = pet; }
	ASCENT_INLINE Pet*	GetSummon() { return m_Summon; }
	Pet* m_Summon;

	void CombatStatusHandler_UpdatePvPTimeout();
	void CombatStatusHandler_ResetPvPTimeout();

	friend class AIInterface;
	friend class Aura;

	virtual void Update( uint32 time );
	virtual void RemoveFromWorld(bool free_guid);
	virtual void OnPushToWorld();

    void setAttackTimer(int32 time, bool offhand);
	bool isAttackReady(bool offhand);

	ASCENT_INLINE void SetDuelWield(bool enabled) { m_duelWield = enabled; }

	bool __fastcall canReachWithAttack(Unit *pVictim);

  //void StrikeWithAbility( Unit* pVictim, Spell* spell, uint32 addspelldmg, uint32 weapon_damage_type );

	/// State flags are server-only flags to help me know when to do stuff, like die, or attack
	ASCENT_INLINE void addStateFlag(uint32 f) { m_state |= f; };
	ASCENT_INLINE bool hasStateFlag(uint32 f) { return (m_state & f ? true : false); }
	ASCENT_INLINE void clearStateFlag(uint32 f) { m_state &= ~f; };

	/// Stats
	ASCENT_INLINE uint32 getLevel() { return m_uint32Values[ UNIT_FIELD_LEVEL ]; };
	ASCENT_INLINE uint8 getRace() { return GetByte(UNIT_FIELD_BYTES_0,0); }
	ASCENT_INLINE uint8 getClass() { return GetByte(UNIT_FIELD_BYTES_0,1); }
	ASCENT_INLINE void setRace(uint8 race) { SetByte(UNIT_FIELD_BYTES_0,0,race); }
	ASCENT_INLINE void setClass(uint8 class_) { SetByte(UNIT_FIELD_BYTES_0,1, class_ ); }
	ASCENT_INLINE uint32 getClassMask() { return 1 << (getClass() - 1); }
	ASCENT_INLINE uint32 getRaceMask() { return 1 << (getRace() - 1); }
	ASCENT_INLINE uint8 getGender() { return GetByte(UNIT_FIELD_BYTES_0,2); }
	ASCENT_INLINE void setGender(uint8 gender) { SetByte(UNIT_FIELD_BYTES_0,2,gender); }
	ASCENT_INLINE uint8 getStandState() { return GetByte(UNIT_FIELD_BYTES_1,0); }

	// Bonuses
	void ModifyBonuses(uint32 type,int32 val);
	void CalcStat(uint32 index);
	uint32 FlatStatMod[5];
	uint32 StatModPct[5];
	uint32 TotalStatModPct[5];
	int32 m_feralAP;
	uint32 m_ModInterrMRegenPCT[NUM_POWER_TYPES];
	int32 m_ModInterrMRegen[NUM_POWER_TYPES];


 
	//// Combat
   // void DealDamage(Unit *pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId = 0);   // to stop from falling, etc
	//void AttackerStateUpdate( Unit* pVictim, uint32 weapon_damage_type ); // weapon_damage_type: 0 = melee, 1 = offhand(dualwield), 2 = ranged
	uint32 GetSpellDidHitResult( Unit* pVictim, uint32 weapon_damage_type, SpellEntry* ability, int32 forced_school = -1 );
	void Strike( Unit* pVictim, uint32 weapon_damage_type, SpellEntry* ability, int32 add_damage, int32 pct_dmg_mod, uint32 exclusive_damage, bool disable_proc, bool skip_hit_check );
//	void PeriodicAuraLog(Unit *pVictim, SpellEntry* spellID, uint32 damage, uint32 damageType);
	//void SpellNonMeleeDamageLog(Unit *pVictim, uint32 spellID, uint32 damage);
	uint32 m_procCounter;
	void HandleProc(uint32 flag, Unit* Victim, SpellEntry* CastingSpell,uint32 dmg=-1,uint32 abs=0,bool mainhand=true,uint32 weapon_damage_type=0);
	//uint32 HandleProc(uint32 flag, Unit* Victim, SpellEntry* CastingSpell,uint32 dmg=-1,uint32 abs=0,uint32 weapon_damage_type=0);
	void HandleProcDmgShield(uint32 flag, Unit* attacker);//almost the same as handleproc :P
//	void HandleProcSpellOnSpell(Unit* Victim,uint32 damage,bool critical);//nasty, some spells proc other spells

	int32 GetAP();
	int32 GetRAP();

	//float GetSize();
	
	void CastSpell(Object *Target, uint32 SpellID, bool triggered);
	void CastSpell(Object *Target, SpellEntry* Sp, bool triggered);

	void CastSpellOld(Unit *Target, uint32 SpellID, bool triggered);
	void CastSpellOld(Unit *Target, SpellEntry* Sp, bool triggered);
	void CastSpellOld(GameObject *Target, uint32 SpellID, bool triggered);
	void CastSpellOld(GameObject *Target, SpellEntry* Sp, bool triggered);

	/*void CastSpell(Player *Target, uint32 SpellID, bool triggered) { CastSpell(static_cast<Unit *>(Target), SpellID, triggered); }
	void CastSpell(Creature *Target, uint32 SpellID, bool triggered) { CastSpell(static_cast<Unit *>(Target), SpellID, triggered); }
	void CastSpell(Player *Target, SpellEntry* Sp, bool triggered) { CastSpell(static_cast<Unit *>(Target), Sp, triggered); }
	void CastSpell(Creature *Target, SpellEntry* Sp, bool triggered) { CastSpell(static_cast<Unit *>(Target), Sp, triggered); }*/

	void CastSpell(uint64 targetGuid, uint32 SpellID, bool triggered);
	void CastSpell(uint64 targetGuid, SpellEntry* Sp, bool triggered);
	void CastSpellAoF(float x,float y,float z,SpellEntry* Sp, bool triggered);
	void EventCastSpell(Unit *Target, SpellEntry * Sp);
	
	//----- Waad - Gestion des nouvelles trajectoire des sorts lanc�s par les NPC - Fortement inspir� des MoonScripts - Par Randdrick
	float GetDetectRange(Unit *Target); // distance entre la source et la cible.
	bool IsUnitValidTarget(Object* pObject, Target_Filter pFilter); // Validit� de la trajectoire ?
	Unit* GetBestTargetPlayer(Target_Filter pFilter=TARGET_FILTER_NONE); // Meilleur Trajectoire trouv�e vers le joueur aggro.
	Unit* GetBestTargetUnit(Target_Filter pFilter=TARGET_FILTER_NONE); // Meilleur Trajectoire trouv�e pour l'unit� (Player, pets, NPCs, totem, etc.)	
	Unit* ChoiceOfBestTarget(UnitArray& pTargetArray, Target_Filter pFilter); // Choix de la meilleur trajectoire sur l'unit� vis�e( S'applique aussi bien au joueur qu'au NPC).
	Unit* ChoiceOfNearestTarget(UnitArray& pTargetArray); // Choix de la trajectoire la plus proche.
	Unit* ChoiceOfSecondMostHatedTarget(UnitArray& pTargetArray); // Choix de la deuxi�me cible la plus d�test�e.

	bool isCasting();
	bool IsInInstance();
	double GetResistanceReducion(Unit *pVictim, uint32 type, float armorReducePct);
	void CalculateResistanceReduction(Unit *pVictim,dealdamage *dmg,SpellEntry* ability, float armorreducepct) ;
    void CalculateOldResistanceReduction(Unit *pVictim,dealdamage *dmg,SpellEntry* ability) ;
	void RegenerateHealth();
	void RegeneratePower(bool isinterrupted);
	void SendPowerUpdate();
	ASCENT_INLINE void setHRegenTimer(uint32 time) {m_H_regenTimer = time; }
	ASCENT_INLINE void setPRegenTimer(uint32 time) {m_P_regenTimer = time; }
	ASCENT_INLINE void DelayPowerRegeneration(uint32 time) { m_P_regenTimer = time; if (!m_interruptedRegenTime) m_interruptedRegenTime = 2000; }
	void Morph(uint32 displayid);
	void DeMorph();
	uint32 ManaShieldAbsorb(uint32 dmg, SpellEntry* sp);
	void smsg_AttackStart(Unit* pVictim);
	void smsg_AttackStop(Unit* pVictim);
	void smsg_AttackStop(uint64 victimGuid);
	
	ASCENT_INLINE bool IsDazed() { return m_mechanicscount[MECHANIC_DAZED] > 0; }
	//this function is used for creatures to get chance to daze for another unit
	float get_chance_to_daze(Unit *target);

	// Stealth  
	ASCENT_INLINE int32 GetStealthLevel() { return m_stealthLevel; }
	ASCENT_INLINE int32 GetStealthDetectBonus() { return m_stealthDetectBonus; }
	ASCENT_INLINE void SetStealth(uint32 id) { m_stealth = id; }
	ASCENT_INLINE bool IsStealth() { return (m_stealth!=0 ? true : false); }
	float detectRange;

	// Invisibility
	ASCENT_INLINE void SetInvisibility(uint32 id) { m_invisibility = id; }
	ASCENT_INLINE bool IsInvisible() { return (m_invisible!=0 ? true : false); }
	uint32 m_invisibility;
	bool m_invisible;
	uint8 m_invisFlag;
	int32 m_invisDetect[INVIS_FLAG_TOTAL];
	
    /************************************************************************/
    /* Stun Immobilize                                                      */
    /************************************************************************/
	uint32 trigger_on_stun;				// second wind warrior talent
	uint32 trigger_on_stun_chance;

	void SetTriggerStunOrImmobilize(uint32 newtrigger,uint32 new_chance);
    void EventStunOrImmobilize();

    /************************************************************************/
    /* Chill                                                                */
    /************************************************************************/
	uint32 trigger_on_chill;			//mage "Frostbite" talent chill
	uint32 trigger_on_chill_chance;

	void SetTriggerChill(uint32 newtrigger, uint32 new_chance);
    void EventChill(Unit *proc_target);
	
	bool HasAura(uint32 spellid);
	bool HasAura(Aura* a);
	bool HasAuraVisual(uint32 visualid);  //not spell id!!!
	bool HasActiveAura(uint32 spelllid);
	bool HasActiveAura(uint32 spelllid,uint64);

	bool HasAuraWithMechanic(uint32 mechanic);
	bool HasPosAuraWithMechanic(uint32 mechanic);
	bool HasNegAuraWithMechanic(uint32 mechanic);

	void GiveGroupXP(Unit *pVictim, Player *PlayerInGroup);

	/// Combat / Death Status
	ASCENT_INLINE bool isAlive() { return m_deathState == ALIVE; };
	ASCENT_INLINE bool isDead() { return  m_deathState !=ALIVE; };
	virtual void setDeathState(DeathState s) {
		m_deathState = s;
	};
	DeathState getDeathState() { return m_deathState; }
	void OnDamageTaken();

	//! Add Aura to unit
	void AddAura(Aura *aur, SpellScript* script);
	//! Remove aura from unit
	bool RemoveAura(Aura *aur);
	bool RemoveAura(uint32 spellId);
	bool RemoveAura(uint32 spellId,uint64 guid);
	bool RemovePositiveAura(uint32 spellId);
	bool RemoveAuraByNameHash(uint32 namehash);//required to remove weaker instances of a spell
	bool RemoveAuraPosByNameHash(uint32 namehash);//required to remove weaker instances of a spell
	bool RemoveAuraNegByNameHash(uint32 namehash);//required to remove weaker instances of a spell
	bool RemoveAuras(uint32 * SpellIds);
	bool RemoveAuras(uint32 amount, ...);

	void EventRemoveAura(uint32 SpellId)
	{
		RemoveAura(SpellId);
	}

	//! Remove all auras
	void RemoveAllAuras();
	bool RemoveAllAuras(uint32 spellId, uint64 guid, Aura* ignore = NULL); //remove stacked auras but only if they come from the same caster. Shaman purge If GUID = 0 then removes all auras with this spellid
    void RemoveAllAuraType(uint32 auratype);//ex:to remove morph spells
	bool RemoveAllAuraByNameHash(uint32 namehash);//required to remove weaker instances of a spell
	bool RemoveAllPosAuraByNameHash(uint32 namehash);//required to remove weaker instances of a spell
	bool RemoveAllNegAuraByNameHash(uint32 namehash);//required to remove weaker instances of a spell
	bool RemoveAllAurasByMechanic( uint32 MechanicType , uint32 MaxDispel , bool HostileOnly ); // Removes all (de)buffs on unit of a specific mechanic type.
	
	void RemoveNegativeAuras();
	void RemoveAllAreaAuras();
	// Temporary remove all auras
	   // Find auras
	Aura *FindAuraByUniqueGroup(uint64 group, uint64 caster_guid = 0, uint32 name_hash = 0);
	Aura *FindAuraPosByNameHash(uint32 namehash);
	Aura* FindAura(uint32 spellId);
	Aura* FindAura(uint32 spellId, uint64 guid);
	bool SetAurDuration(uint32 spellId,Unit* caster,uint32 duration);
	bool SetAurDuration(uint32 spellId,uint32 duration);
	   void DropAurasOnDeath();
	   
	void castSpell(Spell * pSpell);
	void InterruptSpell();

	//caller is the caster
	int32 GetSpellDmgBonus(Unit *pVictim, Spell* spell, int32 base_dmg, bool isdot, bool healing);
	int32 GetSpellDmgBonus(Unit *pVictim, Spell* spell, int32 base_dmg, bool isdot, float duration = 0.0f);
	int32 GetSpellDmgBonus(Unit *pVictim, SpellEntry* spellInfo, int32 base_dmg, bool isdot, int32 forced_school = -1, float duration = 1.0f);
   
	Unit* create_guardian(uint32 guardian_entry,uint32 duration,float angle, uint32 lvl = 0);//guardians are temporary spawn that will inherit master faction and will folow them. Apart from that they have their own mind

	uint32 m_addDmgOnce;
	Creature* m_summonslot[NUM_SUMMON_SLOTS];
	uint32 m_ObjectSlots[4];
	uint32 m_triggerSpell;
	uint32 m_triggerDamage;
	uint32 m_canMove;
	
	uint32 m_lastHauntInitialDamage;	
	
	// Spell Effect Variables
	int32 m_silenced;
	bool m_damgeShieldsInUse;
	std::list<struct DamageProc> m_damageShields;
	std::list<struct ReflectSpellSchool*> m_reflectSpellSchool;
	std::list<struct DamageSplitTarget> m_damageSplitTargets;
 
	std::list<struct ProcTriggerSpell> m_procSpells;
//	std::map<uint32,ProcTriggerSpellOnSpellList> m_procSpellonSpell; //index is namehash
	std::map<uint32,struct SpellCharge> m_chargeSpells;
	deque<uint32> m_chargeSpellRemoveQueue;
	bool m_chargeSpellsInUse;
	ASCENT_INLINE void SetOnMeleeSpell(uint32 spell, uint8 ecn) { m_meleespell = spell; m_meleespellecn = ecn; }
	ASCENT_INLINE uint32 GetOnMeleeSpell() { return m_meleespell; }
	ASCENT_INLINE uint32 GetOnMeleeSpellExtraCN() { return m_meleespellecn; }

	// On Aura Remove Procs
	map_t m_onAuraRemoveSpells;

	void AddOnAuraRemoveSpell(uint32 NameHash, uint32 procSpell, uint32 procChance, bool procSelf);
	void RemoveOnAuraRemoveSpell(uint32 NameHash);
	void OnAuraRemove(uint32 NameHash, Unit *m_target);
	
	// Spell Crit
	float spellcritperc;

	//dummy auras, spell stuff
	void AddDummyAura( SpellEntry* sp )
	{
		m_DummyAuras[sp->NameHash] = sp;
	}
	bool HasDummyAura( uint32 namehash )
	{
		return m_DummyAuras[namehash] != NULL;
	}
	SpellEntry* GetDummyAura( uint32 namehash )
	{
		return m_DummyAuras[namehash];
	}
	void RemoveDummyAura( uint32 namehash )//we removes only this shit, not aura
	{
		m_DummyAuras[namehash] = NULL;
	}
	
	//Custom timers
	uint32 m_CustomTimers[NUM_CUSTOM_TIMERS];

	//Beacon of Light
	Unit *BeaconCaster;//if we receive heal from him
	Unit *BeaconTarget;//heal him for this same value
	void RemoveBeacons();

	// PathFinding
	PathFinding *CurrentPathFinding;
	
	// AIInterface
	AIInterface *GetAIInterface() { return m_aiInterface; }
	void ReplaceAIInterface(AIInterface *new_interface) ;
	void ClearHateList();
	void WipeHateList();
	void WipeTargetList();
	ASCENT_INLINE void setAItoUse(bool value){m_useAI = value;}

	int32 GetThreatModifyer() { return m_threatModifyer; }
	void ModThreatModifyer(int32 mod) { m_threatModifyer += mod; }
	int32 GetGeneratedThreatModifyer() { return m_generatedThreatModifyer; }
	void ModGeneratedThreatModifyer(int32 mod) { m_generatedThreatModifyer += mod; }

	// DK:Affect
	ASCENT_INLINE uint32 IsPacified() { return m_auracount[SPELL_AURA_MOD_PACIFY]; }
	ASCENT_INLINE uint32 IsStunned() { return m_auracount[SPELL_AURA_MOD_STUN]; }
	ASCENT_INLINE uint32 IsFeared() { return m_auracount[SPELL_AURA_MOD_FEAR]; }
	ASCENT_INLINE uint32 IsConfused() { return m_auracount[SPELL_AURA_MOD_CONFUSE]; }
	ASCENT_INLINE uint32 IsRooted() { return m_auracount[SPELL_AURA_MOD_ROOT]; }
	ASCENT_INLINE bool CanAct() 
	{
		if (!IsFeared() && !IsStunned() && !IsPacified ()&& !IsConfused() && isAlive())
			return true;
		return false;
	}
	ASCENT_INLINE uint32 GetResistChanceMod() { return m_resistChance; }
	ASCENT_INLINE void SetResistChanceMod(uint32 amount) { m_resistChance=amount; }
	
	ASCENT_INLINE uint16 HasNoInterrupt() { return m_noInterrupt; }
	bool setDetectRangeMod(uint64 guid, int32 amount);
	void unsetDetectRangeMod(uint64 guid);
	int32 getDetectRangeMod(uint64 guid);
	void Heal(Unit* target,uint32 SpellId, uint32 amount);
	void Energize(Unit* target,uint32 SpellId, uint32 amount, uint32 type);

	Loot loot;
	ASCENT_INLINE void ClearLoot() { loot.items.clear(); loot.gold = 0; loot.looters.clear(); }
	ASCENT_INLINE Loot* GetLoot() { return &loot; }
	uint32 SchoolCastPrevent[7];
	float MechanicDurationPctMod[NUM_MECHANIC];
	int32 GetDamageDoneMod(uint32 school);
	float GetDamageDonePctMod(uint32 school);
	float DamageDoneModPCT[7];
	int32 DamageTakenMod[7];
	float DamageTakenPctMod[7];
	float DamageTakenPctModOnHP35;
	float CritMeleeDamageTakenPctMod[7];
	float CritRangedDamageTakenPctMod[7];
	int32 RangedDamageTaken;
	void CalcDamage();
	float BaseDamage[2];
	float BaseOffhandDamage[2];
	float BaseRangedDamage[2];
	SchoolAbsorb Absorbs[7];
	uint32 AbsorbDamage(Object *Attacker, uint32 School,uint32 * dmg, SpellEntry * pSpell);//returns amt of absorbed dmg, decreases dmg by absorbed value
	int32 RAPvModifier;
	int32 APvModifier;
	uint64 stalkedby;
	uint32 dispels[10];
	bool trackStealth;
	uint32 MechanicsDispels[27];
	float MechanicsResistancesPCT[27];
	float DispelResistancesPCT[NUM_DISPELS];
	float ModDamageTakenByMechPCT[27];
	//int32 RangedDamageTakenPct; 

	bool m_modifiersInherited;
	//SM
	int32 * SM_CriticalChance;//flat
	int32 * SM_FDur;//flat
	int32 * SM_PDur;//pct
	int32 * SM_PRadius;//pct
	int32 * SM_FRadius;//flat
	int32 * SM_PRange;//pct
	int32 * SM_FRange;//flat
	int32 * SM_PCastTime;//pct
	int32 * SM_FCastTime;//flat
	int32 * SM_PCriticalDamage;
	int32 * SM_PDOT;//pct
	int32 * SM_FDOT;//flat
	int32 * SM_FEffectBonus;//flat
	int32 * SM_PEffectBonus;//pct
	int32 * SM_FDamageBonus;//flat
	int32 * SM_PDamageBonus;//pct
	int32 * SM_PSPELL_VALUE;//pct
	int32 * SM_FSPELL_VALUE;//flat
	int32 * SM_FHitchance;//flat
	int32 * SM_PAPBonus;//pct
	int32 * SM_PCost;
	int32 * SM_FCost;
	int32 * SM_PNonInterrupt;
	int32 * SM_PJumpReduce;
	int32 * SM_FSpeedMod;
	int32 * SM_FAdditionalTargets;
	int32 * SM_FPenalty;//flat
	int32 * SM_PPenalty;//Pct
	int32 * SM_PCooldownTime;
	int32 * SM_FCooldownTime;
	int32 * SM_FChanceOfSuccess;
	int32 * SM_FAmptitude;
	int32 * SM_PAmptitude;
	int32 * SM_FRezist_dispell;
	int32 * SM_PRezist_dispell;
	int32 * SM_PThreat_Reduction; //pct
	int32 * SM_FThreat_Reduction; //flat
	int32 * SM_FEffect1_Bonus;
	int32 * SM_FEffect2_Bonus;
	int32 * SM_FEffect3_Bonus;
	int32 * SM_PEffect1_Bonus;
	int32 * SM_PEffect2_Bonus;
	int32 * SM_PEffect3_Bonus;
	int32 * SM_FCharge_Bonus;
	int32 * SM_PCharge_Bonus;
	int32 * SM[SPELL_MODIFIERS][2]; // 0 = flat, 1 = percent
	void InheritSMMods(Unit *inherited_from);
	
	// Multimap used to handle aura 271
	// key is caster GUID and value is a pair of SpellMask pointer and mod value
	//typedef std::multimap<uint64, pair<uint32*, int32> > DamageTakenPctModPerCasterType;
	typedef std::multimap<uint64, pair<uint32, uint32> > DamageTakenPctModPerCasterType;
	DamageTakenPctModPerCasterType DamageTakenPctModPerCaster;

	//Events
	void Emote (EmoteType emote);
	void EventAddEmote(EmoteType emote, uint32 time);
	void EmoteExpire();
	ASCENT_INLINE void setEmoteState(uint8 emote) { m_emoteState = emote; };
	ASCENT_INLINE uint32 GetOldEmote() { return m_oldEmote; }
	void EventSummonPetExpire();
	void EventAurastateExpire(uint32 aurastateflag){RemoveFlag(UNIT_FIELD_AURASTATE,aurastateflag);} //hmm this looks like so not necesary :S
	void EventHealthChangeSinceLastUpdate();

	void SetStandState (uint8 standstate);

	ASCENT_INLINE StandState GetStandState()
	{
		//uint32 bytes1 = GetUInt32Value (UNIT_FIELD_BYTES_1);
		//return StandState (uint8 (bytes1));

		return StandState (GetByte( UNIT_FIELD_BYTES_1, 0)); // C'est mieux comme ca (Branruz)
	}

	void SendChatMessage(uint8 type, uint32 lang, const char *msg);
	void SendChatMessageToPlayer(uint8 type, uint32 lang, const char *msg, Player *plr);
	void SendChatMessageAlternateEntry(uint32 entry, uint8 type, uint32 lang, const char * msg);
	void RegisterPeriodicChatMessage(uint32 delay, uint32 msgid, std::string message, bool sendnotify);

	ASCENT_INLINE void SetHealthPct(uint32 val) 
	{ 
		if (val>0) SetUInt32Value(UNIT_FIELD_HEALTH,float2int32(val*0.01f*GetUInt32Value(UNIT_FIELD_MAXHEALTH))); 
	};

	ASCENT_INLINE int GetHealthPct() 
	{ 
		if (GetUInt32Value(UNIT_FIELD_MAXHEALTH)==0)  return 0; 
		return (int)(GetUInt32Value(UNIT_FIELD_HEALTH) * 100 / GetUInt32Value(UNIT_FIELD_MAXHEALTH)); };
    
	ASCENT_INLINE int GetManaPct() 
	{ 
		if (GetUInt32Value(UNIT_FIELD_MAXPOWER1)==0)  return 0; 
		return (int)(GetUInt32Value(UNIT_FIELD_POWER1) * 100 / GetUInt32Value(UNIT_FIELD_MAXPOWER1)); 
	};
		
	uint32 GetResistance(uint32 type);	
	    
	uint32 m_teleportAckCounter;
	//Vehicle
	Player* pVehicle;
	uint8 m_inVehicleSeatId;
	Vehicle *m_CurrentVehicle;
	
	ASCENT_INLINE Unit* GetVehicle(bool forcevehicle = false)
	{
		if(m_CurrentVehicle)
			return ((Unit *)m_CurrentVehicle);
		if(pVehicle && !forcevehicle)
			return ((Unit *)pVehicle);
		return NULL;
	}

	//Pet
	void smsg_TalentsInfo(bool pet);
	uint8 m_PetTalentPointModifier;
	ASCENT_INLINE void SetIsPet(bool chck) { m_isPet = chck; }
	
	//In-Range
	virtual void AddInRangeObject(Object* pObj);
	virtual void OnRemoveInRangeObject(Object* pObj);
	void ClearInRangeSet();

	ASCENT_INLINE Spell * GetCurrentSpell(){return m_currentSpell;}
	ASCENT_INLINE void SetCurrentSpell(Spell* cSpell) { m_currentSpell = cSpell; }

	uint32 m_CombatUpdateTimer;

	ASCENT_INLINE void setcanperry(bool newstatus){can_parry=newstatus;}

	std::map<uint32, Aura*> tmpAura; // First = Spell Id

	//stats mods
	uint32 m_healthfromspell;
	uint32 m_manafromspell;

	uint32 BaseResistance[7]; //there are resistances for silence, fear, mechanics ....
	uint32 BaseStats[5];
	int32 HealDoneMod[7];
	int32 HealDonePctMod[7];
	int32 HealTakenMod[7];
	float HealTakenPctMod[7];
	uint32 SchoolImmunityList[7];
	float SpellCritChanceSchool[7];
	int32 PowerCostMod[7];
	float Penetration[7]; // armor penetration & spell penetration
	int32 AttackerCritChanceMod[7];
	uint32 SpellDelayResist[7];
	int32 CreatureAttackPowerMod[12];
	int32 CreatureRangedAttackPowerMod[12];
	float AOEDmgMod;

	float PctRegenModifier;//1.0 by default
	float mana_reg;
	float PctPowerRegenModifier[4];
	void RemoveSoloAura(uint32 type);

	/*ASCENT_INLINE*/ uint32 GetPowerType(){ return (GetByte(UNIT_FIELD_BYTES_0,3)); } //GetUInt32Value(UNIT_FIELD_BYTES_0)>> 24);} // Octet 3

	void RemoveAurasByInterruptFlag(uint32 flag);
	void RemoveAurasByInterruptFlagButSkip(uint32 flag, uint32 skip);
	// Auras Modifiers
	int32 m_interruptRegen;
	int32 m_resistChance;
	int32 m_powerRegenPCT;
	int32 m_extraattacks;   
	int32 m_extrastriketargets;
	//std::set<SpellEntry*> m_onStrikeSpells;

	uint16 m_auracount[TOTAL_SPELL_AURAS];
	uint16 m_mechanicscount[MECHANIC_COUNT];
	uint16 m_dispelscount[NUM_DISPELS];

	int32 m_noInterrupt;
	bool disarmed;
	bool disarmedShield;
	uint64 m_detectRangeGUID[5];
	int32  m_detectRangeMOD[5];
	// Affect Speed
	float m_speedModifier;
	float m_slowdown;
	float m_maxSpeed;
	map< uint32, float > speedReductionMap;
	bool GetSpeedDecrease();
	float m_mountedspeedModifier;
	float m_flyspeedModifier;
	void UpdateSpeed();
	void EnableFlight();
	void DisableFlight();

	// Escort Quests
	//uint32 m_escortquestid;
	//uint32 m_escortupdatetimer;
	//bool bHasEscortQuest;
	//bool bEscortActive;
	//bool bStopAtEndOfWaypoints;
	//bool bReturnOnDie;
	//Player *q_AttachedPlayer;
	//uint16 m_escortStartWP;
	//uint16 m_escortEndWP;
	/*void InitializeEscortQuest(uint32 questid, bool stopatend, bool returnondie);
	void EscortSetStartWP(uint32 wp);
	void EscortSetEndWP(uint32 wp);
	void StartEscortQuest();
	void PauseEscortQuest();
	void EndEscortQuest();*/
	void MoveToWaypoint(uint32 wp_id);	
	void PlaySpellVisual(uint64 target, uint32 spellVisual);
	
	void SetPower(uint32 type, int32 value)
	{
		uint32 maxpower = GetUInt32Value(UNIT_FIELD_MAXPOWER1 + type);
		value = (value < 0) ? 0 : ( (value > (int32)maxpower) ? maxpower : value ); 
		SetUInt32Value(UNIT_FIELD_POWER1 + type, value);
		SendPowerUpdate();
		
	}
	
	void RemoveStealth()
	{
		if( m_stealth != 0 )
		{
			RemoveAura( m_stealth );
			m_stealth = 0;
		}
	}
	
	void RemoveInvisibility()
	{
		if( m_invisibility != 0 )
		{
		RemoveAura( m_invisibility );
		m_invisibility = 0;
		}
	}
	
	void ChangePetTalentPointModifier(bool Increment) { Increment ? m_PetTalentPointModifier++ : m_PetTalentPointModifier-- ; };
	bool m_isPet;
	uint32 m_stealth;
	bool m_can_stealth;

	Aura* m_auras[MAX_AURAS+MAX_PASSIVE_AURAS];   

	int32 m_modlanguage;
	
	Creature *critterPet;
	Creature *summonPet;

	ASCENT_INLINE uint32 GetCharmTempVal() { return m_charmtemp; }
	ASCENT_INLINE void SetCharmTempVal(uint32 val) { m_charmtemp = val; }
	set<uint32> m_SpellList;

	ASCENT_INLINE void DisableAI() { m_useAI = false; }
	ASCENT_INLINE void EnableAI() { m_useAI = true; }

	void SetPowerType(uint8 type);

	ASCENT_INLINE bool IsSpiritHealer()
	{
		switch(GetEntry())
		{
		case 6491:  // Spirit Healer
		case 13116: // Alliance Spirit Guide
		case 13117: // Horde Spirit Guide
			{
				return true;
			}break;
		}
		return false;
	}

	void Root();
	void Unroot();

	void SetFacing(float newo);//only working if creature is idle

	void RemoveAurasByBuffIndexType(uint32 buff_index_type, const uint64 &guid);
	void RemoveAurasByBuffType(uint32 buff_type, const uint64 &guid,uint32 skip);
	bool HasAurasOfBuffType(uint32 buff_type, const uint64 &guid,uint32 skip);
	int	 HasAurasWithNameHash(uint32 name_hash);
	bool HasNegativeAuraWithNameHash(uint32 name_hash); //just to reduce search range in some cases
	bool HasNegativeAura(uint32 spell_id); //just to reduce search range in some cases
	ASCENT_INLINE bool IsPoisoned() { return (m_dispelscount[DISPEL_POISON] == 0); }

	AuraCheckResponse AuraCheck(SpellEntry *info);
	AuraCheckResponse AuraCheck(uint32 name_hash, uint32 rank, Object *caster=NULL);
	AuraCheckResponse AuraCheck(uint32 name_hash, uint32 rank, Aura* aur, Object *caster);

	uint16 m_diminishCount[23];
	uint8  m_diminishAuraCount[23];
	uint16 m_diminishTimer[23];
	bool   m_diminishActive;

	void SetDiminishTimer(uint32 index)
	{
		m_diminishTimer[index] = 15000;
	}

	DynamicObject * dynObj;

	uint32 AddAuraVisual(uint32 spellid, uint32 count, bool positive);
	void SetAuraSlotLevel(uint32 slot, bool positive);

	void RemoveAuraVisual(uint32 slot);
	void ModifyAuraApplications(uint32 spellid, uint32 count);
	uint32 GetAuraApplications(uint32 spellid);
	bool HasVisibleAura(uint32 spellid);

	//! returns: aura stack count
	uint32 ModAuraStackCount(uint32 slot, int32 count);
	//uint8 m_auraStackCount[MAX_AURAS];

	void RemoveAurasOfSchool(uint32 School, bool Positive, bool Immune);
	SpellEntry * pLastSpell;
	bool bProcInUse;
	bool bInvincible;
	Player * m_redirectSpellPackets;
	void UpdateVisibility();
	
	struct {
		uint32 amt;
		uint32 max;
	} m_soulSiphon;
	
	//solo target auras
	uint32 m_hotStreakCount;
	uint32 m_incanterAbsorption;
	uint32 m_frozenTargetCharges;
	uint32 m_frozenTargetId;	
	uint32 polySpell;
	uint32 m_special_state; //flags for special states (stunned,rooted etc)
	
//	uint32 fearSpell;
	uint32 m_cTimer;
	void EventUpdateFlag();
	CombatStatusHandler CombatStatus;

	// En commantaire pour l'instant (Branruz)
	// A quoi sert cette var ?? 
	// bool m_temp_summon;

	// Redirect Threat shit
	Unit * mThreatRTarget;
	float mThreatRAmount;
	
	void CancelSpell(Spell * ptr);
	void EventStrikeWithAbility(uint64 guid, SpellEntry * sp, uint32 damage);
	bool m_spellsbusy;
	void DispelAll(bool positive);

	//void SetPower(uint32 type, int32 value);
	
	bool mRecentlyBandaged;
	
	float m_ignoreArmorPctMaceSpec;
	float m_ignoreArmorPct;

	int32 m_LastSpellManaCost;

	bool HasAurasOfNameHashWithCaster(uint32 namehash, Unit * caster);
	int8 m_hasVampiricTouch;
	int8 m_hasVampiricEmbrace;

	//! Is PVP flagged?
	bool IsPvPFlagged();
	void SetPvPFlag();
	bool IsFFAPvPFlagged();
	void SetFFAPvPFlag();
	//! Removal
	void RemovePvPFlag();
	void RemoveFFAPvPFlag();

	void SetWeaponDisplayId(uint8 slot, uint32 displayId);

	// Pour les bots escorteurs
	void SetIAGuard(bool _m_useAI) { m_useAI = _m_useAI; };

	// Yeah, le pnj peut parlerrrrrrrrrrrrr, banzaiiiiiiiiiii.... (brz)
	AI_Speak *m_AI_Speak;
	void SetPNJIsSpeaker(bool _m_speaker) { m_speaker = _m_speaker; }
	bool IsSpeaker(void) { return(m_speaker); }
	void SearchBestResponse(Player* plr,string texte_player);
	void EventRegainFlight();
	void SetRedirectThreat(Unit * target, float amount, uint32 Duaration);
	void EventResetRedirectThreat();
	void knockback(int32 basepoint, uint32 miscvalue, bool disengage = false );

protected:
	Unit ();

	uint32 m_meleespell;
	uint8 m_meleespellecn;
	void _UpdateSpells(uint32 time);

	uint32 m_H_regenTimer;
	uint32 m_P_regenTimer;
	uint32 m_interruptedRegenTime; //PowerInterruptedegenTimer.
	uint32 m_state;		 // flags for keeping track of some states
	uint32 m_attackTimer;   // timer for attack
	uint32 m_attackTimer_1;
	bool m_duelWield;

	/// Combat
	DeathState m_deathState;

	// Stealth
	uint32 m_stealthLevel;
	uint32 m_stealthDetectBonus;	
	
	// DK:pet
	//uint32 m_pet_state;
	//uint32 m_pet_action;
	//uint8 m_PetTalentPointModifier;

	// Spell currently casting
	Spell * m_currentSpell;

	// AI
	AIInterface *m_aiInterface;
	bool m_useAI;
	bool m_speaker; // Le pnj peut parler avec le player (Traitement langage naturel, Fr seulement - Brz)
	bool can_parry;//will be enabled by block spell
	int32 m_threatModifyer;
	int32 m_generatedThreatModifyer;

	//	float getDistance( float Position1X, float Position1Y, float Position2X, float Position2Y );	

	int32 m_manashieldamt;
	uint32 m_manaShieldId;

	// Quest emote
	uint8 m_emoteState;
	uint32 m_oldEmote;

	uint32 m_charmtemp;

	bool m_extraAttackCounter;

	std::map<uint32, SpellEntry*> m_DummyAuras;

};


#endif
