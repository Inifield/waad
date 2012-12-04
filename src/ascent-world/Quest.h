/*	QUEST_SHARE_MSG_TOO_FAR		 			= 11,
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

#ifndef WOWSERVER_QUEST_H
#define WOWSERVER_QUEST_H

using namespace std;
/*
2.3.0 research
not available because low level = 1
available but quest low level = 2
question mark = 3 (not got objectives)
blue question mark = 4
blue exclamation mark = 5
yellow exclamation mark = 6
yellow question mark = 7
finished = 8
132 error = 9
*/

enum QUEST_STATUS
{
	QMGR_QUEST_NOT_AVAILABLE					= 0x00,	// There aren't any quests available.		| "No Mark"
	QMGR_QUEST_AVAILABLELOW_LEVEL				= 0x01,	// Quest available, and your level isn't enough.| "Gray Quotation Mark !"
	QMGR_QUEST_CHAT								= 0x02,	// Quest available it shows a talk balloon.	| "No Mark"
	// On 3.1.2 0x03 and 0x04 is some new status, so the old ones are now shifted by 2 (0x03->0x05 and so on). 
	QMGR_QUEST_REPEATABLE_FINISHED_LOWLEVEL		= 0x03, 
	QMGR_QUEST_REPEATABLE_LOWLEVEL				= 0x04, 
	QMGR_QUEST_NOT_FINISHED						= 0x05,	// Quest isn't finished yet.			| "Gray Question ? Mark"
	QMGR_QUEST_REPEATABLE_FINISHED				= 0x06,
	QMGR_QUEST_REPEATABLE						= 0x07,	// Quest repeatable				| "Blue Question ? Mark" 
	QMGR_QUEST_AVAILABLE						= 0x08,	// Quest available, and your level is enough	| "Yellow Quotation ! Mark" 
	QMGR_QUEST_FINISHED							= 0x0A,	// Quest has been finished.			| "Yellow Question  ? Mark" (7 has no minimap icon)
	//QUEST_ITEM_UPDATE				= 0x06	 // Yellow Question "?" Mark. //Unknown
};

enum ICONS_STATUS
{
    ICON_STATUS_NONE           	= 0,
    ICON_STATUS_FAILED       	= 1,
    ICON_STATUS_UNAVAILABLE    	= 2,
    ICON_STATUS_INCOMPLETE     	= 3,
    ICON_STATUS_COMPLETE        = 4,
	ICON_STATUS_AVAILABLE      	= 5,
};

enum QUESTGIVER_QUEST_TYPE
{
	QUESTGIVER_QUEST_START  = 0x01,
	QUESTGIVER_QUEST_END	= 0x02,
};

enum QUEST_TYPE
{
	QUEST_GATHER	= 0x01,
	QUEST_SLAY	  = 0x02,
};

enum QUEST_FLAG
{
	QUEST_FLAG_NONE		    = 0x00, //   0   
	QUEST_FLAG_DELIVER	    = 0x01, //   1   
	QUEST_FLAG_KILL		    = 0x02, //   2  
	QUEST_FLAG_SPEAKTO	    = 0x04, //   4
	QUEST_FLAG_REPEATABLE	= 0x08, //   8  
	QUEST_FLAG_EXPLORATION  = 0x10, //  16
	QUEST_FLAG_TIMED		= 0x20, //  32
	QUEST_FLAG_UNK          = 0x40, //  64
	QUEST_FLAG_REPUTATION	= 0x80, // 128
};

enum FAILED_REASON
{
	FAILED_REASON_FAILED			= 0,
	FAILED_REASON_INV_FULL			= 4,
    FAILED_REASON_DUPE_ITEM_FOUND   = 17,
};

enum INVALID_REASON
{
	INVALID_REASON_DONT_HAVE_REQ			= 0,
	INVALID_REASON_DONT_HAVE_LEVEL			= 1,
	INVALID_REASON_DONT_HAVE_RACE			= 6,
	INVALID_REASON_COMPLETED_QUEST			= 7,
	INVALID_REASON_HAVE_TIMED_QUEST			= 12,
	INVALID_REASON_HAVE_QUEST				= 13,
//	INVALID_REASON_DONT_HAVE_REQ_ITEMS	  = 0x13,
//	INVALID_REASON_DONT_HAVE_REQ_MONEY	  = 0x15,
	INVALID_REASON_DONT_HAVE_EXP_ACCOUNT	= 16,
	INVALID_REASON_DONT_HAVE_REQ_ITEMS		= 21, //changed for 2.1.3
	INVALID_REASON_DONT_HAVE_REQ_MONEY		= 23,
	INVALID_REASON_UNKNOW26					= 26, //"you have completed 10 daily quests today"
	INVALID_REASON_UNKNOW27					= 27,//"You cannot completed quests once you have reached tired time"
};

enum QUEST_SHARE
{
	QUEST_SHARE_MSG_SHARING_QUEST			= 0, 
	QUEST_SHARE_MSG_CANT_TAKE_QUEST			= 1, 
	QUEST_SHARE_MSG_ACCEPT_QUEST			= 2,
	QUEST_SHARE_MSG_REFUSE_QUEST			= 3,
	QUEST_SHARE_MSG_TOO_FAR		 			= 4,
	QUEST_SHARE_MSG_BUSY					= 5, 
	QUEST_SHARE_MSG_LOG_FULL				= 6, 
	QUEST_SHARE_MSG_HAVE_QUEST				= 7, 
	QUEST_SHARE_MSG_FINISH_QUEST			= 8, 
	QUEST_SHARE_MSG_CANT_SHARE_TODAY		= 9,
	QUEST_SHARE_MSG_QUEST_TIMER_FINISHED	= 10,
	QUEST_SHARE_MSG_NOT_IN_PARTY			= 11,
};

class QuestScript;
#pragma pack(push,1)
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
	
	QuestScript* pQuestScript;
};
#pragma pack(pop)

enum QUEST_MOB_TYPES
{
	QUEST_MOB_TYPE_CREATURE     = 1,
	QUEST_MOB_TYPE_GAMEOBJECT   = 2,
	QUEST_MOB_TYPE_UNK_CREATURE = 3,
};

enum QuestLogIndex
{
 QUEST_ID_INDEX    = 0,
 QUEST_STATE_INDEX = 1,
 QUEST_FIELD_INDEX = 2,
 QUEST_TIMER_INDEX = 4,
};

enum QuestLogState
{
 PLAYER_QUEST_NONE = 0,
 PLAYER_QUEST_COMPLETE,
 PLAYER_QUEST_FAIL
};


class QuestScript;
#define CALL_QUESTSCRIPT_EVENT(obj, func) if(static_cast<QuestLogEntry*>(obj)->GetQuest()->pQuestScript != NULL) static_cast<QuestLogEntry*>(obj)->GetQuest()->pQuestScript->func

class SERVER_DECL QuestLogEntry : public EventableObject
{
	friend class QuestMgr;

public:

	QuestLogEntry();
	~QuestLogEntry();

	ASCENT_INLINE Quest* GetQuest() { return m_quest; };
	void Init(Quest* quest, Player* plr, uint32 slot);

	bool CanBeFinished();
	void SubtractTime(uint32 value);
	void SaveToDB(QueryBuffer * buf);
	bool LoadFromDB(Field *fields);
	void UpdatePlayerFields();

	void SetTrigger(uint32 i);
	void SetMobCount(uint32 i, uint32 count);
	void SetPlayerSlainCount(uint32 count);
	void SetPlayerKillCount(uint32 count);

	bool IsUnitAffected(Unit* target);
	ASCENT_INLINE bool IsCastQuest() { return iscastquest;}
	void AddAffectedUnit(Unit* target);
	void ClearAffectedUnits();

	void SetSlot(int32 i);
	void Finish();

	void SendQuestComplete();
	void SendUpdateAddKill(uint32 i, uint64 victimGUID); // On passe le Guid du mob (Branruz)
	void SendUpdateAddPlayerKill(uint64 victimGUID);
	ASCENT_INLINE uint32 GetPlayerSlainCount() { return m_player_slain; }
	ASCENT_INLINE uint32 GetMobCount(uint32 i) { return m_mobcount[i]; }
	ASCENT_INLINE uint32 GetPlayerCount() { return m_playercount; } //AspireDev
	ASCENT_INLINE uint32 GetExploredAreas(uint32 i) { return m_explored_areas[i]; }

	ASCENT_INLINE uint32 GetBaseField(uint32 slot)
	{
		return PLAYER_QUEST_LOG_1_1 + (slot * PLAYER_QUEST_SLOT_MAX);
	}

private:
	uint32 completed;

	bool mInitialized;
	bool mDirty;

	Quest *m_quest;
	Player *m_plr;
	
	uint32 m_mobcount[4];
	uint32 m_playercount;
	uint32 m_explored_areas[4];

	uint32 m_player_slain;

	std::set<uint64> m_affected_units;
	bool iscastquest;

	uint32 m_time_left;
	int32 m_slot;
};

#endif
