/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2011 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the Ascent Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the Ascent is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#ifndef __STRUCTURES_H
#define __STRUCTURES_H

#define CHECK_PACKET_SIZE(pckp, ssize) if(ssize && pckp.size() < ssize) { Disconnect(); return; }

struct RPlayerInfo
{
	uint32 Guid;
	uint32 AccountId;
	string Name;
	uint32 Level;
	uint32 GuildId;
	float PositionX;
	float PositionY;
	float PositionZ;
	uint32 ZoneId;
	uint8 Race;
	uint8 Class;
	uint8 Gender;
	uint32 Latency;
	string GMPermissions;
	uint32 Account_Flags;
	uint32 InstanceId;
	uint32 MapId;
	uint32 iInstanceType;
	int32 references;
	uint32 ClientBuild;
	uint32 Team;

	void Pack(ByteBuffer& buf)
	{
		buf << Guid << AccountId << Name << PositionX << PositionY << PositionZ << ZoneId << Race << Class << Gender << Latency << GMPermissions
			<< Account_Flags << InstanceId << Level << GuildId << MapId << iInstanceType << ClientBuild << Team;
	}

	size_t Unpack(ByteBuffer & buf)
	{
		buf >> Guid >> AccountId >> Name >> PositionX >> PositionY >> PositionZ >> ZoneId >> Race >> Class >> Gender >> Latency >> GMPermissions
			>> Account_Flags >> InstanceId >> Level >> GuildId >> MapId >> iInstanceType >> ClientBuild >> Team;
		return buf.rpos();
	}

	uint32 getClassMask() { return 1 << (Class - 1); }
	uint32 getRaceMask() { return 1 << (Race - 1); }

#ifndef _GAME
	/* This stuff is used only by the realm server */
	Session * session;
	Session * GetSession() { return session; }
	void RBroadcastMessage(const char* Format, ...);
	uint32 RecoveryMapId;
	LocationVector RecoveryPosition;

	/************************************************************************/
	/* SOCIAL                                                               */
	/************************************************************************/
private:
	/* we may have multiple threads on this(chat) - burlex */
	Mutex m_socialLock;
	map<uint32, char*> m_friends;
	set<uint32> m_ignores;
	set<uint32> m_hasFriendList;

public:
	//void RBroadcastMessage(const char* Format, ...);

	bool Social_IsIgnoring(RPlayerInfo * m_info)
	{
		bool res;
		m_socialLock.Acquire();
		if( m_ignores.find( m_info->Guid ) == m_ignores.end() )
			res = false;
		else
			res = true;

		m_socialLock.Release();
		return res;
	}

	bool Social_IsIgnoring(uint32 guid)
	{
		bool res;
		m_socialLock.Acquire();
		if( m_ignores.find( guid ) == m_ignores.end() )
			res = false;
		else
			res = true;

		m_socialLock.Release();
		return res;
	}

	/************************************************************************/
	/* end social                                                           */
	/************************************************************************/
#endif //_GAME
};

#ifndef _GAME
/* Copied structures from game */
struct SocketInfo {
	uint32 SocketColor;
	uint32 Unk;
};

struct ItemSpell
{
	uint32 Id;
	uint32 Trigger;
	int32 Charges;
	int32 Cooldown;
	uint32 Category;
	int32 CategoryCooldown;
};

struct ItemDamage
{
	float Min;
	float Max;
	uint32 Type;
};

struct ItemStat
{
	uint32 Type;
	int32 Value;
};

struct ItemPrototype
{
	uint32 ItemId;          // 0
	uint32 Class;           // 1
	uint32 SubClass;        // 2 
	uint32 unknown_bc;      // 3 
	char * Name1;           // 4
	char * Name2;           // 5
	char * Name3;           // 6
	char * Name4;           // 7
	uint32 DisplayInfoID;   // 8
	uint32 Quality;         // 9
	uint32 Flags;           // 10
    uint32 Faction;         // 11
	uint32 BuyPrice;        // 12
	uint32 SellPrice;       // 13
	uint32 InventoryType;   // 14
	uint32 AllowableClass;  // 15
	uint32 AllowableRace;   // 16
	uint32 ItemLevel;       // 17
	uint32 RequiredLevel;   // 18
	uint32 RequiredSkill;   // 19
	uint32 RequiredSkillRank; // 20
	uint32 RequiredSkillSubRank; // 21
	uint32 RequiredPlayerRank1;  // 22
	uint32 RequiredPlayerRank2;  // 23
	uint32 RequiredFaction;      // 24
	uint32 RequiredFactionStanding; // 25
	uint32 Unique;           // 26
	uint32 MaxCount;         // 27
	uint32 ContainerSlots;   // 28
	uint32 StatsCount;       // 29
	ItemStat Stats[10];      // 30 - 49
	uint32 ScalingStatsEntry; // 50
	uint32 ScalingStatsFlag; // 51
	ItemDamage Damage[2]; // 52 - 57
	uint32 Armor;         // 58
	uint32 HolyRes;       // 59
	uint32 FireRes;       // 60
	uint32 NatureRes;     // 61 
	uint32 FrostRes;      // 62
	uint32 ShadowRes;     // 63
	uint32 ArcaneRes;     // 64
	uint32 Delay;         // 65 
	uint32 AmmoType;      // 99
	float  Range;         // 100
	ItemSpell Spells[5];  // 101 - 130
	uint32 Bonding;       // 131
	char * Description;   // 132 
	uint32 PageId;        // 133 
	uint32 PageLanguage;  // 134
	uint32 PageMaterial;  // 135
	uint32 QuestId;       // 136
	uint32 LockId;        // 137
	uint32 LockMaterial;  // 138
	uint32 Field108;      // 139
	uint32 RandomPropId;  // 140
	uint32 RandomSuffixId; // 141
	uint32 Block;          // 142
	uint32 ItemSet;        // 143
	uint32 MaxDurability;  // 144
	uint32 ZoneNameID;     // 145 
	uint32 MapID;          // 146
	uint32 BagFamily;      // 147
	uint32 TotemCategory;  // 148
	SocketInfo Sockets[3]; // 149 - 154
	uint32 SocketBonus;    // 155
	uint32 GemProperties;  // 156
	int32 DisenchantReqSkill; // 157
	uint32 ArmorDamageModifier; // 158
	//-- ArcEmu
	uint32 ExistingDuration;   // 159
	uint32 ItemLimitCategory;  // 160
	uint32 HolidayId;          // 161
    // --- End Item.wdb
};

struct CreatureInfo
{
	uint32 Id;
	char * Name;
	char * SubName;
	char * info_str;
	uint32 Flags1;
	uint32 Type;
	uint32 Family;
	uint32 Rank;
	uint32 Unknown1;
	uint32 SpellDataID;
	uint32 Male_DisplayID;
	uint32 Female_DisplayID;
	uint32 Male_DisplayID2;
	uint32 Female_DisplayID2;
	float unkfloat1;
	float unkfloat2;
	uint8  Civilian;
	uint8  Leader;
};


struct GameObjectInfo
{
	uint32 ID;
	uint32 Type;
	uint32 DisplayID;
	char * Name;
	uint32 SpellFocus;
	uint32 sound1;
	uint32 sound2;
	uint32 sound3;
	uint32 sound4;
	uint32 sound5;
	uint32 sound6;
	uint32 sound7;
	uint32 sound8;
	uint32 sound9;
	uint32 Unknown1;
	uint32 Unknown2;
	uint32 Unknown3;
	uint32 Unknown4;
	uint32 Unknown5;
	uint32 Unknown6;
	uint32 Unknown7;
	uint32 Unknown8;
	uint32 Unknown9;
	uint32 Unknown10;
	uint32 Unknown11;
	uint32 Unknown12;
	uint32 Unknown13;
	uint32 Unknown14;
	float  Unknown15;
};

struct ItemPage
{
	uint32 id;
	char * text;
	uint32 next_page;
};


struct Quest
{
	uint32 id;                           // entry           u
	uint32 zone_id;                      // ZoneId          u
	uint32 quest_sort;                   // sort            u
	uint32 quest_flags;                  // flags           u
	uint32 min_level;                    // MinLevel        u
	uint32 max_level;                    // questlevel      u
	uint32 type;                         // Type            u
	uint32 required_races;               // RequiredRaces   u
	uint32 required_class;               // RequiredClass   u
	uint32 required_tradeskill;          // RequiredTradeskill       u
	uint32 required_tradeskill_value;    // RequiredTradeskillValue  u
	uint32 required_rep_faction;         // RequiredRepFaction       u
	uint32 required_rep_value;           // RequiredRepValue         u

	uint32 required_players;          	 // RequiredPlayer           u

	uint32 time;                         // LimitTime                u
	uint32 special_flags;                // SpecialFlags             u
	
	uint32 previous_quest_id;            // PrevQuestId              u
	uint32 next_quest_id;                // NextQuestId              u

	uint32 srcitem;                      // srcItem                  u
	uint32 srcitemcount;                 // SrcItemCount             u

	char * title;                        // Title                    s
	char * details;                      // Details                  s
	char * objectives;                   // Objectives               s
	char * completiontext;               // CompletionText           s
	char * incompletetext;               // IncompleteText           s
	char * endtext;                      // EndText                  s

	char * objectivetexts[4];            // ObjectiveText1 ObjectiveText2 ObjectiveText3 ObjectiveText4 ssss

	uint32 required_mob[4];           // ReqKillMobOrGOId     uuuu
	uint32 required_mobcount[4];      // ReqKillMobOrGOCount  uuuu

	uint32 required_kill_player;	  // RequiredKillPlayer   u 

	uint32 required_item[6];          // ReqOrRecItemId       uuuuuu
	uint32 required_itemcount[6];     // ReqOrRecItemIdCount  uuuuuu     

	uint32 RecItemDuringQuestId[4];   // 3.0.3, WaadTeam      uuuu

	uint32 required_spell[4];         // ReqCastSpellId       uuuu  

	uint32 reward_choiceitem[6];      // RewChoiceItemId1     uuuuuu
	uint32 reward_choiceitemcount[6]; // RewChoiceItemCount1  uuuuuu

	uint32 reward_item[4];            // RewItemId1           uuuu
	uint32 reward_itemcount[4];       // RewItemCount1        uuuu
	
	// A FAIRE: Reduire ou pas a 5, dans les trames reseaux de l'offi
	//          les reps factions ne sont que de 5 max
	// Note: "reward_repvalue" devient "reward_repvalueIndex"
	uint32 reward_repfaction[5];      // RewRepFaction1 // modifié par Randdrick                         uuuuu
	int32  reward_repvalueIndex[5];   // RewRepValue1   // gain de réputation pour 6 factions possibles  uuuuu
	uint32 reward_title;              // RewTitle        u
	uint32 reward_xp_as_money;        // RewMoneyLimit   u

	uint32 reward_money; // RewMoney        u 
	uint32 bonus_honor;  // Obsolete ?      u     
	uint32 reward_xp;        // RewXP       u      
	uint32 reward_spell;     // RewSpell    u
	uint32 reward_talents;	 // RewTalents  u
	uint32 effect_on_player; // CastSpell   u
	
	uint32 point_mapid;      // PointMapId  u
	uint32 point_x;          // PointX      u
	uint32 point_y;          // PointY      u
	uint32 point_opt;        // PointOpt    u

	uint32 required_money;         // RequiredMoney      u
	uint32 required_triggers[4];   // ExploreTrigger1    uuuu
	uint32 required_quests[4];     // RequiredQuest1     uuuu         
	//uint32 receive_items[4]; 
	//uint32 receive_itemcount[4];
	int is_repeatable;             // IsRepeatable       u

    // Personalisation 
	uint32 count_required_mob;
	uint32 count_requiredquests;
	uint32 count_requiredtriggers;
	uint32 count_receiveitems;
	uint32 count_reward_choiceitem;
	uint32 count_required_item;
	uint32 required_mobtype[4];
	uint32 count_reward_item;
	uint32 reward_replimit;
	
};


struct GossipText_Text
{
	float Prob;
	char * Text[2];
	uint32 Lang;
	uint32 Emote[6];
};

struct GossipText
{
	uint32 ID;
	GossipText_Text Texts[8];
};

struct MapInfo
{
	uint32 mapid;
	uint32 screenid;
	uint32 type;
	uint32 playerlimit;
	uint32 minlevel;
	float repopx;
	float repopy;
	float repopz;
	uint32 repopmapid;
	char * name;
	uint32 flags;
	uint32 cooldown;
    uint32 lvl_mod_a;
	uint32 required_quest_A1;
	uint32 required_quest_H2;
	uint32 required_item;
	uint32 heroic_key_1;
	uint32 heroic_key_2;
	float update_distance;
	uint32 checkpoint_id;
	uint32 collision;
	uint32 clustering_handled;

	bool HasFlag(uint32 flag)
	{
		return (flags & flag) != 0;
	}

};

#endif //_GAME

#endif //__STRUCTURES_H
