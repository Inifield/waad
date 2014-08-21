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

#ifndef WOWSERVER_GAMEOBJECT_H
#define WOWSERVER_GAMEOBJECT_H

class Player;
class GameObjectAIScript;
class GameObjectTemplate;

struct GOQuestItem
{
	uint32 itemid;
	uint32 requiredcount;
};

struct GOQuestGameObject
{
	uint32 goid;
	uint32 requiredcount;
};

enum GAMEOBJECT_FLAG_BIT
{
	GAMEOBJECT_UNCLICKABLE = 0x01,
	GAMEOBJECT_CLICKABLE = 0x20,
};

#if ENABLE_SHITTY_STL_HACKS == 1
typedef HM_NAMESPACE::hash_map<Quest*, uint32 > GameObjectGOMap;
#else
namespace HM_NAMESPACE
{
	template <>
	struct hash<Quest*>
	{
		union __vp
		{
			size_t s;
			Quest* p;
		};

		size_t operator()(Quest* __x) const
		{
			__vp vp;
			vp.p = __x;
			return vp.s;
		}
	};
};

typedef HM_NAMESPACE::hash_map<Quest*, uint32, HM_NAMESPACE::hash<Quest*> > GameObjectGOMap;
#endif

typedef HM_NAMESPACE::hash_map<Quest*, std::map<uint32, uint32> > GameObjectItemMap;

#pragma pack(push,1)
struct GameObjectInfo
{
	uint32 ID;
	uint32 Type;
	uint32 DisplayID;
	char * Name;

    union // different GO types have different data field (Analyse vient de Mangos)
    {
        struct //0 GAMEOBJECT_TYPE_DOOR
        {
            uint32 startOpen;                               //0 used client side to determine GO_ACTIVATED means open/closed
            uint32 lockId;                                  //1 -> Lock.dbc
            uint32 autoCloseTime;                           //2 secs till autoclose = autoCloseTime / IN_MILLISECONDS (previous was 0x10000)
            uint32 noDamageImmune;                          //3 break opening whenever you recieve damage?
            uint32 openTextID;                              //4 can be used to replace castBarCaption?
            uint32 closeTextID;                             //5
            uint32 ignoredByPathing;                        //6
        } door;
        
        struct //1 GAMEOBJECT_TYPE_BUTTON
        {
            uint32 startOpen;                               //0
            uint32 lockId;                                  //1 -> Lock.dbc
            uint32 autoCloseTime;                           //2 secs till autoclose = autoCloseTime / IN_MILLISECONDS (previous was 0x10000)
            uint32 linkedTrapId;                            //3
            uint32 noDamageImmune;                          //4 isBattlegroundObject
            uint32 large;                                   //5
            uint32 openTextID;                              //6 can be used to replace castBarCaption?
            uint32 closeTextID;                             //7
            uint32 losOK;                                   //8
        } button;
        
        struct //2 GAMEOBJECT_TYPE_QUESTGIVER
        {
            uint32 lockId;                                  //0 -> Lock.dbc
            uint32 questList;                               //1
            uint32 pageMaterial;                            //2
            uint32 gossipID;                                //3
            uint32 customAnim;                              //4
            uint32 noDamageImmune;                          //5
            uint32 openTextID;                              //6 can be used to replace castBarCaption?
            uint32 losOK;                                   //7
            uint32 allowMounted;                            //8
            uint32 large;                                   //9
        } questgiver;
        
        struct //3 GAMEOBJECT_TYPE_CHEST
        {
            uint32 lockId;                                  //0 -> Lock.dbc
            uint32 lootId;                                  //1
            uint32 chestRestockTime;                        //2
            uint32 consumable;                              //3
            uint32 minSuccessOpens;                         //4
            uint32 maxSuccessOpens;                         //5
            uint32 eventId;                                 //6 lootedEvent
            uint32 linkedTrapId;                            //7
            uint32 questId;                                 //8 not used currently but store quest required for GO activation for player
            uint32 level;                                   //9
            uint32 losOK;                                   //10
            uint32 leaveLoot;                               //11
            uint32 notInCombat;                             //12
            uint32 logLoot;                                 //13
            uint32 openTextID;                              //14 can be used to replace castBarCaption?
            uint32 groupLootRules;                          //15
            uint32 floatingTooltip;                         //16
        } chest;

        //4 GAMEOBJECT_TYPE_BINDER - empty
        
        struct //5 GAMEOBJECT_TYPE_GENERIC
        {
            uint32 floatingTooltip;                         //0
            uint32 highlight;                               //1
            uint32 serverOnly;                              //2
            uint32 large;                                   //3
            uint32 floatOnWater;                            //4
            uint32 questID;                                 //5
        } _generic;
        
        struct //6 GAMEOBJECT_TYPE_TRAP
        {
            uint32 lockId;                                  //0 -> Lock.dbc
            uint32 level;                                   //1
            uint32 radius;                                  //2 radius for trap activation
            uint32 spellId;                                 //3
            uint32 charges;                                 //4 need respawn (if > 0)
            uint32 cooldown;                                //5 time in secs
            uint32 autoCloseTime;                           //6 secs till autoclose = autoCloseTime / IN_MILLISECONDS (previous was 0x10000)
            uint32 startDelay;                              //7
            uint32 serverOnly;                              //8
            uint32 stealthed;                               //9
            uint32 large;                                   //10
            uint32 stealthAffected;                         //11
            uint32 openTextID;                              //12 can be used to replace castBarCaption?
            uint32 closeTextID;                             //13
            uint32 ignoreTotems;                            //14
        } trap;
        
        struct //7 GAMEOBJECT_TYPE_CHAIR
        {
            uint32 slots;                                   //0
            uint32 height;                                  //1
            uint32 onlyCreatorUse;                          //2
            uint32 triggeredEvent;                          //3
        } chair;
        
        struct //8 GAMEOBJECT_TYPE_SPELL_FOCUS
        {
            uint32 focusId;                                 //0
            uint32 dist;                                    //1
            uint32 linkedTrapId;                            //2
            uint32 serverOnly;                              //3
            uint32 questID;                                 //4
            uint32 large;                                   //5
            uint32 floatingTooltip;                         //6
        } spellFocus;
        
        struct //9 GAMEOBJECT_TYPE_TEXT
        {
            uint32 pageID;                                  //0
            uint32 language;                                //1
            uint32 pageMaterial;                            //2
            uint32 allowMounted;                            //3
        } text;
        
        struct //10 GAMEOBJECT_TYPE_GOOBER
        {
            uint32 lockId;                                  //0 -> Lock.dbc
            uint32 questId;                                 //1
            uint32 eventId;                                 //2
            uint32 autoCloseTime;                           //3 secs till autoclose = autoCloseTime / IN_MILLISECONDS (previous was 0x10000)
            uint32 customAnim;                              //4
            uint32 consumable;                              //5
            uint32 cooldown;                                //6
            uint32 pageId;                                  //7
            uint32 language;                                //8
            uint32 pageMaterial;                            //9
            uint32 spellId;                                 //10
            uint32 noDamageImmune;                          //11
            uint32 linkedTrapId;                            //12
            uint32 large;                                   //13
            uint32 openTextID;                              //14 can be used to replace castBarCaption?
            uint32 closeTextID;                             //15
            uint32 losOK;                                   //16 isBattlegroundObject
            uint32 allowMounted;                            //17
            uint32 floatingTooltip;                         //18
            uint32 gossipID;                                //19
            uint32 WorldStateSetsState;                     //20
        } goober;
        
        struct //11 GAMEOBJECT_TYPE_TRANSPORT
        {
            uint32 pause;                                   //0
            uint32 startOpen;                               //1
            uint32 autoCloseTime;                           //2 secs till autoclose = autoCloseTime / IN_MILLISECONDS (previous was 0x10000)
            uint32 pause1EventID;                           //3
            uint32 pause2EventID;                           //4
        } transport;
        
        struct //12 GAMEOBJECT_TYPE_AREADAMAGE
        {
            uint32 lockId;                                  //0
            uint32 radius;                                  //1
            uint32 damageMin;                               //2
            uint32 damageMax;                               //3
            uint32 damageSchool;                            //4
            uint32 autoCloseTime;                           //5 secs till autoclose = autoCloseTime / IN_MILLISECONDS (previous was 0x10000)
            uint32 openTextID;                              //6
            uint32 closeTextID;                             //7
        } areadamage;
        
        struct //13 GAMEOBJECT_TYPE_CAMERA
        {
            uint32 lockId;                                  //0 -> Lock.dbc
            uint32 cinematicId;                             //1
            uint32 eventID;                                 //2
            uint32 openTextID;                              //3 can be used to replace castBarCaption?
        } camera;

        //14 GAMEOBJECT_TYPE_MAPOBJECT - empty
       
        struct  //15 GAMEOBJECT_TYPE_MO_TRANSPORT
        {
            uint32 taxiPathId;                              //0
            uint32 moveSpeed;                               //1
            uint32 accelRate;                               //2
            uint32 startEventID;                            //3
            uint32 stopEventID;                             //4
            uint32 transportPhysics;                        //5
            uint32 mapID;                                   //6
            uint32 worldState1;                             //7
        } moTransport;

        //16 GAMEOBJECT_TYPE_DUELFLAG - empty
        //17 GAMEOBJECT_TYPE_FISHINGNODE - empty
        
        struct //18 GAMEOBJECT_TYPE_SUMMONING_RITUAL
        {
            uint32 reqParticipants;                         //0
            uint32 spellId;                                 //1
            uint32 animSpell;                               //2
            uint32 ritualPersistent;                        //3
            uint32 casterTargetSpell;                       //4
            uint32 casterTargetSpellTargets;                //5
            uint32 castersGrouped;                          //6
            uint32 ritualNoTargetCheck;                     //7
        } summoningRitual;

        //19 GAMEOBJECT_TYPE_MAILBOX - empty
        //20 GAMEOBJECT_TYPE_DONOTUSE - empty
        
        struct //21 GAMEOBJECT_TYPE_GUARDPOST
        {
            uint32 creatureID;                              //0
            uint32 charges;                                 //1
        } guardpost;
        
        struct //22 GAMEOBJECT_TYPE_SPELLCASTER
        {
            uint32 spellId;                                 //0
            uint32 charges;                                 //1
            uint32 partyOnly;                               //2
            uint32 allowMounted;                            //3
            uint32 large;                                   //4
        } spellcaster;
       
        struct  //23 GAMEOBJECT_TYPE_MEETINGSTONE
        {
            uint32 minLevel;                                //0
            uint32 maxLevel;                                //1
            uint32 areaID;                                  //2
        } meetingstone;
        
        struct //24 GAMEOBJECT_TYPE_FLAGSTAND
        {
            uint32 lockId;                                  //0
            uint32 pickupSpell;                             //1
            uint32 radius;                                  //2
            uint32 returnAura;                              //3
            uint32 returnSpell;                             //4
            uint32 noDamageImmune;                          //5
            uint32 openTextID;                              //6
            uint32 losOK;                                   //7
        } flagstand;
        
        struct //25 GAMEOBJECT_TYPE_FISHINGHOLE   
        {
            uint32 radius;                                  //0 how close bobber must land for sending loot
            uint32 lootId;                                  //1
            uint32 minSuccessOpens;                         //2
            uint32 maxSuccessOpens;                         //3
            uint32 lockId;                                  //4 -> Lock.dbc; possibly 1628 for all?
        } fishinghole;
        
        struct //26 GAMEOBJECT_TYPE_FLAGDROP
        {
            uint32 lockId;                                  //0
            uint32 eventID;                                 //1
            uint32 pickupSpell;                             //2
            uint32 noDamageImmune;                          //3
            uint32 openTextID;                              //4
        } flagdrop;
        
        struct //27 GAMEOBJECT_TYPE_MINI_GAME
        {
            uint32 gameType;                                //0
        } miniGame;
        
        struct //29 GAMEOBJECT_TYPE_CAPTURE_POINT
        {
            uint32 radius;                                  //0
            uint32 spell;                                   //1
            uint32 worldState1;                             //2
            uint32 worldstate2;                             //3
            uint32 winEventID1;                             //4
            uint32 winEventID2;                             //5
            uint32 contestedEventID1;                       //6
            uint32 contestedEventID2;                       //7
            uint32 progressEventID1;                        //8
            uint32 progressEventID2;                        //9
            uint32 neutralEventID1;                         //10
            uint32 neutralEventID2;                         //11
            uint32 neutralPercent;                          //12
            uint32 worldstate3;                             //13
            uint32 minSuperiority;                          //14
            uint32 maxSuperiority;                          //15
            uint32 minTime;                                 //16
            uint32 maxTime;                                 //17
            uint32 large;                                   //18
            uint32 highlight;                               //19
            uint32 startingValue;                           //20
            uint32 unidirectional;                          //21
        } capturePoint;
        
        struct //30 GAMEOBJECT_TYPE_AURA_GENERATOR
        {
            uint32 startOpen;                               //0
            uint32 radius;                                  //1
            uint32 auraID1;                                 //2
            uint32 conditionID1;                            //3
            uint32 auraID2;                                 //4
            uint32 conditionID2;                            //5
            uint32 serverOnly;                              //6
        } auraGenerator;
       
        struct  //31 GAMEOBJECT_TYPE_DUNGEON_DIFFICULTY
        {
            uint32 mapID;                                   //0
            uint32 difficulty;                              //1
        } dungeonDifficulty;
        
        struct //32 GAMEOBJECT_TYPE_BARBER_CHAIR
        {
            uint32 chairheight;                             //0
            uint32 heightOffset;                            //1
        } barberChair;
        
        struct //33 GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING
        {
            uint32 NumHits_100PCT;      //0 - DisplayId Go intact
            uint32 unk1;                //1 - 0: Rempart, 1: Tower
            uint32 empty1;              //2
            uint32 Event_100PCT;        //3
            uint32 DisplayId_50PCT;     //4 - DisplayId Go à 50% detruit
            uint32 NumHits_50PCT;       //5
            uint32 empty3;              //6 - 1: Rempart, 2: Tower
            uint32 empty4;              //7 - 0: Rempart, 1:Tower
            uint32 empty5;              //8
            uint32 DamagedEvent;        //9
            uint32 DisplayId_Detruit;   //10 - DisplayId Go detruit
            uint32 Used;                //11 - 0: Not Use, 1: Used
            uint32 empty8;              //12
            uint32 empty9;              //13
            uint32 DestroyedEvent;      //14
            uint32 empty10;             //15
            uint32 DebuildingTimeSecs;  //16 - en Sec (300Sec, 5min ou 0)
            uint32 empty11;             //17
            uint32 DestructibleData;    //18
            uint32 RebuildingEvent;     //19
            uint32 empty12;             //20
            uint32 empty13;             //21
            uint32 DamageEvent;         //22
            uint32 empty14;             //23
        } destructibleBuilding;

        //34 GAMEOBJECT_TYPE_GUILDBANK - empty
       
        struct  //35 GAMEOBJECT_TYPE_TRAPDOOR
        {
            uint32 whenToPause;                             // 0
            uint32 startOpen;                               // 1
            uint32 autoClose;                               // 2
        } trapDoor;

        struct // Base de donnée
        {
	     uint32 SpellFocus;  // 0
	     uint32 sound1;      // 1
	     uint32 sound2;      // 2
	     uint32 sound3;      // 3
	     uint32 sound4;      // 4
	     uint32 sound5;      // 5
	     uint32 sound6;      // 6
	     uint32 sound7;      // 7
	     uint32 sound8;      // 8
	     uint32 sound9;      // 9
	     uint32 Unknown1;    // 10 
	     uint32 Unknown2;    // 11
	     uint32 Unknown3;    // 12
	     uint32 Unknown4;    // 13
	     uint32 Unknown5;    // 14
	     uint32 Unknown6;    // 15
	     uint32 Unknown7;    // 16
	     uint32 Unknown8;    // 17
	     uint32 Unknown9;    // 18
	     uint32 Unknown10;   // 19
	     uint32 Unknown11;   // 20 
	     uint32 Unknown12;   // 21
	     uint32 Unknown13;   // 22
	     uint32 Unknown14;   // 23
	     float  Unknown15;   // 24 - 3.0.8 (Branruz)
        } raw;
	};
	// Quests
	GameObjectGOMap goMap;
	GameObjectItemMap itemMap;
	GossipScript * gossip_script;
};
#pragma pack(pop)


enum GAMEOBJECT_BYTES // Emprunté a HearthStone, ca facilitera la vie (Branruz) 
{
	GAMEOBJECT_BYTES_STATE			= 0,
	GAMEOBJECT_BYTES_TYPE_ID		= 1,
	GAMEOBJECT_BYTES_UNK			= 2, // todo: unknown atm
	GAMEOBJECT_BYTES_ANIMPROGRESS	= 3,
};

enum GAMEOBJECT_TYPES
{
	GAMEOBJECT_TYPE_DOOR			    = 0, // Item de type Clef ?
	GAMEOBJECT_TYPE_BUTTON			    = 1, // Envoie CMSG_GAMEOBJECT_USE
	GAMEOBJECT_TYPE_QUESTGIVER		    = 2,
	GAMEOBJECT_TYPE_CHEST			    = 3,
	GAMEOBJECT_TYPE_BINDER			    = 4,
	GAMEOBJECT_TYPE_GENERIC			    = 5,
	GAMEOBJECT_TYPE_TRAP			    = 6,
	GAMEOBJECT_TYPE_CHAIR			    = 7,
	GAMEOBJECT_TYPE_SPELL_FOCUS		    = 8,
	GAMEOBJECT_TYPE_TEXT			    = 9,
	GAMEOBJECT_TYPE_GOOBER			    = 10,
	GAMEOBJECT_TYPE_TRANSPORT		    = 11,
	GAMEOBJECT_TYPE_AREADAMAGE		    = 12,
	GAMEOBJECT_TYPE_CAMERA			    = 13,
	GAMEOBJECT_TYPE_MAP_OBJECT		    = 14,
	GAMEOBJECT_TYPE_MO_TRANSPORT	    = 15,
	GAMEOBJECT_TYPE_DUEL_ARBITER	    = 16,
	GAMEOBJECT_TYPE_FISHINGNODE		    = 17,
	GAMEOBJECT_TYPE_RITUAL			    = 18,
	GAMEOBJECT_TYPE_MAILBOX			    = 19,
	GAMEOBJECT_TYPE_AUCTIONHOUSE	    = 20,
	GAMEOBJECT_TYPE_GUARDPOST		    = 21,
	GAMEOBJECT_TYPE_SPELLCASTER		    = 22,
	GAMEOBJECT_TYPE_MEETINGSTONE	    = 23,
	GAMEOBJECT_TYPE_FLAGSTAND		    = 24,
	GAMEOBJECT_TYPE_FISHINGHOLE		    = 25,
	GAMEOBJECT_TYPE_FLAGDROP		    = 26,
	GAMEOBJECT_TYPE_MINI_GAME	        = 27,
    GAMEOBJECT_TYPE_LOTTERY_KIOSK	    = 28,
    GAMEOBJECT_TYPE_CAPTURE_POINT	    = 29,
    GAMEOBJECT_TYPE_AURA_GENERATOR	    = 30,
    GAMEOBJECT_TYPE_DUNGEON_DIFFICULTY	= 31,
    GAMEOBJECT_TYPE_BARBER_CHAIR	    = 32,
    GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING	= 33,
    GAMEOBJECT_TYPE_GUILD_BANK	        = 34,
    GAMEOBJECT_TYPE_TRAPDOOR	        = 35,	
	GAMEOBJECT_TYPE_SPECIALGO	        = 36,
};

#define _GAMEOBJECT_DESTRUCT_MASK	0x00000F00
enum _GAMEOBJECT_FLAGS // Ascent
{
	GO_FLAG_NOT_IN_USE      = 0x00000000, //  0 -
	GO_FLAG_IN_USE          = 0x00000001, //  1 - disables interaction while animated
	GO_FLAG_LOCKED          = 0x00000002, //  2 - require key, spell, event, etc to be opened. Makes "Locked" appear in tooltip
	GO_FLAG_INTERACT_COND   = 0x00000004, //  4 - cannot interact (condition to interact)
	GO_FLAG_TRANSPORT       = 0x00000008, //  8 - any kind of transport? Object can transport (elevator, boat, car)
	GO_FLAG_UNK1            = 0x00000010, // 16 -
	GO_FLAG_NO_DESPAWN      = 0x00000020, // 32 - never despawn, typically for doors, they just change state
	GO_FLAG_TRIGGERED       = 0x00000040, // 64 - typically, summoned objects. Triggered by spell or other events

	GO_FLAG_DAMAGED_10      = 0x00000100, // 0001
	GO_FLAG_DAMAGED_20      = 0x00000200, // 0010 GO_FLAG_DAMAGED	
    GO_FLAG_DAMAGED_30      = 0x00000300, // 0011 xxxx
	GO_FLAG_DAMAGED_40      = 0x00000400, // 0100 GO_FLAG_DESTROYED	
	GO_FLAG_DAMAGED_50      = 0x00000500, // 0101 xxxx
	GO_FLAG_DAMAGED_60      = 0x00000600, // 0110 xxxx
	GO_FLAG_DAMAGED_70      = 0x00000700, // 0111 xxxx
	GO_FLAG_DAMAGED_80      = 0x00000800, // 1000
	GO_FLAG_DAMAGED_90      = 0x00000900, // 1001 xxxx
};

enum _BUILDING_TYPE
{
 BUILDING_TYPE_REMPART = 0,
 BUILDING_TYPE_TOWER   = 1,
 
};

enum _GAMEOBJECT_DYNFLAGS
{
	GO_DYNFLAG_QUEST		= 0x00000009,
};

#define CALL_GO_SCRIPT_EVENT(obj, func) if(obj->GetTypeId() == TYPEID_GAMEOBJECT && static_cast<GameObject*>(obj)->GetScript() != NULL) static_cast<GameObject*>(obj)->GetScript()->func
#define CALL_GO_EVENT_SCRIPT_POS(obj, func) if(obj->GetTypeId() == TYPEID_GAMEOBJECT && static_cast<GameObject*>(obj)->GetScript() != NULL) static_cast<GameObject*>(obj)->GetScript()->func

class SERVER_DECL GameObject : public Object
{
public:
	/************************************************************************/
	/* LUA Stuff                                                            */
	/************************************************************************/
/*	typedef struct { const char *name; int(*mfunc)(lua_State*,GameObject*); } RegType;
	static const char className[];
	static RegType methods[];

	// a lua script cannot create a unit.
	GameObject(lua_State * L) { ASSERT(false); }*/

	GameObject(uint64 guid);
	
	virtual void Init();
	 virtual ~GameObject();

	ASCENT_INLINE GameObjectInfo* GetInfo() { return pInfo; }
	ASCENT_INLINE void SetInfo(GameObjectInfo * goi) { pInfo = goi; }

	//void Create ( uint32 display_id, uint8 state, uint32 obj_field_entry, float scale, uint16 type, uint16 faction, uint32 mapid, float x, float y, float z, float ang );
   // void Create ( uint32 mapid, float x, float y, float z, float ang);
	bool CreateFromProto(uint32 entry,uint32 mapid, float x, float y, float z, float ang);
   
	bool Load(GOSpawn *spawn);

	virtual void Update(uint32 p_time);

	void Spawn(MapMgr * m);
	void Despawn(uint32 time);
	Loot loot;
	//void _EnvironmentalDamageUpdate();
	void UpdateTrapState();
	// Serialization
	void SaveToDB();
	void SaveToFile(std::stringstream & name);
	//bool LoadFromDB(uint32 guid);
	//void LoadFromDB(GameObjectTemplate *t);
	void DeleteFromDB();
	void EventCloseDoor();
	uint64 m_rotation;
	void EventCastSpell(uint32 guid, uint32 sp, bool triggered);
	void SetRotation();
	// void SetRotation(float rad);

	//Fishing stuff
	void UseFishingNode(Player *player);
	void EndFishing(Player* player,bool abort);
	void FishHooked(Player * player);
	
	// Quests
	void _LoadQuests();
	bool HasQuests() { return m_quests != NULL; };
	void AddQuest(QuestRelation *Q);
	void DeleteQuest(QuestRelation *Q);
	Quest *FindQuest(uint32 quest_id, uint8 quest_relation);
	uint16 GetQuestRelation(uint32 quest_id);
	uint32 NumOfQuests();
	std::list<QuestRelation *>::iterator QuestsBegin() { return m_quests->begin(); };
	std::list<QuestRelation *>::iterator QuestsEnd() { return m_quests->end(); };
	void SetQuestList(std::list<QuestRelation *>* qst_lst) { m_quests = qst_lst; };

	void SetSummoned(Unit *mob)
	{
		m_summoner = mob;
		m_summonedGo = true;
	}
	void _Expire();
	
	void ExpireAndDelete();

	void Deactivate();

	// Misc, recup sur Mangos, tres pratique (Branruz)
	ASCENT_INLINE uint8 GetGoState()                  { return GetByte(GAMEOBJECT_BYTES_1, 0); }
    ASCENT_INLINE void SetGoState(uint8 state)               { SetByte(GAMEOBJECT_BYTES_1, 0, state); }
	ASCENT_INLINE uint8 GetGoType()                   { return(GetByte(GAMEOBJECT_BYTES_1, 1)); }
	ASCENT_INLINE void SetGoType(uint8 type)                 { SetByte(GAMEOBJECT_BYTES_1, 1, type); }
    ASCENT_INLINE uint8 GetGoArtKit()                 { return GetByte(GAMEOBJECT_BYTES_1, 2); }
    ASCENT_INLINE void SetGoArtKit(uint8 artkit)             { SetByte(GAMEOBJECT_BYTES_1, 2, artkit); }
    ASCENT_INLINE uint8 GetGoAnimProgress()           { return GetByte(GAMEOBJECT_BYTES_1, 3); }
    ASCENT_INLINE void SetGoAnimProgress(uint8 animprogress) { SetByte(GAMEOBJECT_BYTES_1, 3, animprogress); }

	ASCENT_INLINE bool isQuestGiver()
	{
		//if(m_uint32Values[GAMEOBJECT_BYTES_1] == 2)
		if(GetGoType() == GAMEOBJECT_TYPE_QUESTGIVER) return true;
		else                                          return false;
	};

	/// Quest data
	std::list<QuestRelation *>* m_quests;
   
	uint32 *m_ritualmembers;
	uint32 m_ritualcaster,m_ritualtarget;
	uint16 m_ritualspell;

	void InitAI();
	SpellEntry* spell;
	
	float range;
	uint8 checkrate;
	uint16 counter;
	int32 charges;//used for type==22,to limit number of usages.
	bool invisible;//invisible
	uint8 invisibilityFlag;
	Unit* m_summoner;
	int8 bannerslot;
	int8 bannerauraslot;
	CBattleground * m_battleground;

	void CallScriptUpdate();
   
	ASCENT_INLINE GameObjectAIScript* GetScript() { return myScript; }
    ASCENT_INLINE void SetScript(GameObjectAIScript *pScript) { myScript = pScript; }

	void TrapSearchTarget();	// Traps need to find targets faster :P

	ASCENT_INLINE bool HasAI() { return spell != 0; }
	GOSpawn * m_spawn;
	void OnPushToWorld();
	void OnRemoveInRangeObject(Object* pObj);
	void RemoveFromWorld(bool free_guid);

	ASCENT_INLINE bool CanMine(){return (mines_remaining > 0);}
	ASCENT_INLINE void UseMine(){ if(mines_remaining) mines_remaining--;}
	void CalcMineRemaining(bool force)
	{
		if(force || !mines_remaining)
			mines_remaining = GetInfo()->raw.sound4 + RandomUInt(GetInfo()->raw.sound5 - GetInfo()->raw.sound4) - 1;
	}
	bool HasLoot();
	uint32 GetGOReqSkill();
	MapCell * m_respawnCell;

//	custom functions for scripting
	void SetState(uint8 state) { SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, state); }
	uint8 GetState() { return GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE); }
	void SetAnimProgress(uint32 animprogress) { SetByte( GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_ANIMPROGRESS, animprogress ); }
	uint32 GetAnimProgress() { return GetByte( GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_ANIMPROGRESS ); }
	void SetFlags(uint32 flags) { SetUInt32Value(GAMEOBJECT_FLAGS, flags ); }
	uint32 GetFlags() { return GetUInt32Value( GAMEOBJECT_FLAGS ); }
	void SetType(uint8 type) { SetByte( GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID, type ); }
	uint8 GetType() { return GetByte( GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID ); }
	void SetLevel(uint32 level) { SetUInt32Value( GAMEOBJECT_LEVEL, level ); }
	uint32 GetLevel() { return GetUInt32Value( GAMEOBJECT_LEVEL ); }
	void SetDisplayId(uint32 id) { SetUInt32Value( GAMEOBJECT_DISPLAYID, id ); }
	uint32 GetDisplayId() { return GetUInt32Value( GAMEOBJECT_DISPLAYID ); }

	uint32 Health;
	uint32 HealthMax;
	uint32 HealthMid;
	void TakeDamage(uint32 ammount);
	void Damage();
	void Destroy();
	void Rebuild();

protected:

	bool m_summonedGo;
	bool m_deleted;
	GameObjectInfo *pInfo;
	GameObjectAIScript * myScript;
	uint32 _fields[GAMEOBJECT_END];
	uint32 mines_remaining; //used for mining to mark times it can be mined

};

#endif

