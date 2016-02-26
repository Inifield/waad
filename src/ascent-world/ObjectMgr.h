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

#ifndef _OBJECTMGR_H
#define _OBJECTMGR_H

ASCENT_INLINE bool FindXinYString(std::string& x, std::string& y)
{
	return y.find(x) != std::string::npos;
}

struct GM_Ticket
{
	uint64 guid;
	std::string name;
	uint32 level;
	uint32 map;
	float posX;
	float posY;
	float posZ;
	std::string message;
	uint32 timestamp;
};

#pragma pack(push,1)
struct FishingZoneEntry
{
	uint32 ZoneID;
	uint32 MinSkill;
	uint32 MaxSkill;
};

struct AchievementReward
{
	uint32 AchievementID;
	uint32 AllianceTitle;
	uint32 HordeTitle;
	uint32 ItemID;
};

struct ProfessionDiscovery
{
	uint32 SpellId;
	uint32 SpellToDiscover;
	uint32 SkillValue;
	float Chance;
};

struct RandomItemCreation
{
	uint32 SpellId;
	uint32 ItemToCreate;
	uint32 Skill;
	uint32 Chance;
};

struct RandomCardCreation
{
	uint32 SpellId;
	uint32 ItemId[32];
	uint32 itemcount;
};

struct ZoneGuardEntry
{
	uint32 ZoneID;
	uint32 HordeEntry;
	uint32 AllianceEntry;
};

struct ItemPage
{
    uint32 id;
	char * text;
	uint32 next_page;
};
#pragma pack(pop)

struct SpellReplacement
{
	uint32 count;
	uint32 *spells;
};

class Group;

struct GossipMenuItem
{
	uint32 Id;
	uint32 IntId;
	uint8 Icon;
	bool Coded;
	uint32 BoxMoney;
	string Text;
	string BoxMessage;
};

struct GossipMenuAddPOI
{
	uint32 m_IndexPOI;
};

struct PointOfInterest
{
    uint32 entry;
    float x;
    float y;
    uint32 icon;
    uint32 flags;
    uint32 data;
    std::string icon_name;
};

struct QuestPOIPoint
{
	int32 x;
	int32 y;

	QuestPOIPoint() : x(0), y(0) {}
	QuestPOIPoint(int32 _x, int32 _y) : x(_x), y(_y) {}
};

struct QuestPOI
{
	uint32 PoiId;
	int32  ObjectiveIndex;
	uint32 MapId;
	uint32 MapAreaId;
	uint32 FloorId;
	uint32 Unk3;
	uint32 Unk4;
	std::vector<QuestPOIPoint> points;

	QuestPOI() : PoiId(0), ObjectiveIndex(0), MapId(0), MapAreaId(0), FloorId(0), Unk3(0), Unk4(0) {}
	QuestPOI(uint32 poiId, int32 objIndex, uint32 mapId, uint32 mapAreaId, uint32 floorId, uint32 unk3, uint32 unk4) : PoiId(poiId), ObjectiveIndex(objIndex), MapId(mapId), MapAreaId(mapAreaId), FloorId(floorId), Unk3(unk3), Unk4(unk4) {}
};

typedef std::vector<QuestPOI> QuestPOIVector;
typedef HM_NAMESPACE::hash_map<uint32, QuestPOIVector> QuestPOIMap;

struct SpellEntry;
struct TrainerSpell
{
	SpellEntry * pCastSpell;
	SpellEntry * pLearnSpell;
	SpellEntry * pCastRealSpell;
	uint32	DeleteSpell;
	uint32	RequiredSpell;
	uint32	RequiredSkillLine;
	uint32	RequiredSkillLineValue;
	bool	IsProfession;
	uint32	Cost;
	uint32	RequiredLevel;
};

struct Trainer
{
	uint32 SpellCount;
	vector<TrainerSpell> Spells;
	char*	UIMessage;
    uint32 RequiredSkill;
	uint32 RequiredSkillLine;
	uint32 RequiredClass;
	uint32 RequiredRace;
	uint32 RequiredRepFaction;
	uint32 RequiredRepValue;
	uint32 TrainerType;
	uint32 Can_Train_Gossip_TextId;
	uint32 Cannot_Train_GossipTextId;
};

struct LevelInfo
{
	uint32 HP;
	uint32 Mana;
	uint32 Stat[5];
	uint32 XPToNextLevel;
};

struct ReputationMod
{
	uint32 faction[2];
	int32 value;
	uint32 replimit;
};

struct InstanceReputationMod
{
	uint32 mapid;
	uint32 mob_rep_reward;
	uint32 mob_rep_limit;
	uint32 boss_rep_reward;
	uint32 boss_rep_limit;
	uint32 faction[2];
};

struct ReputationModifier
{
	uint32 entry;
	vector<ReputationMod> mods;
};

struct InstanceReputationModifier
{
	uint32 mapid;
	vector<InstanceReputationMod> mods;
};

struct NpcMonsterSay
{
	float Chance;
	uint32 Language;
	uint32 Type;
	const char * MonsterName;

	uint32 TextCount;
	const char ** Texts;
};

enum MONSTER_SAY_EVENTS
{
	MONSTER_SAY_EVENT_ENTER_COMBAT		= 0,
	MONSTER_SAY_EVENT_RANDOM_WAYPOINT	= 1,
	MONSTER_SAY_EVENT_CALL_HELP			= 2,
	MONSTER_SAY_EVENT_ON_COMBAT_STOP	= 3,
	MONSTER_SAY_EVENT_ON_DAMAGE_TAKEN	= 4,
	MONSTER_SAY_EVENT_ON_DIED			= 5,
	NUM_MONSTER_SAY_EVENTS,
};

enum AREATABLE_FLAGS
{
	/*
    AREA_CITY_AREA          = 0x0020,
    AREA_NEUTRAL_AREA       = 0x0040,
    AREA_PVP_ARENA          = 0x0080,
    AREA_CITY               = 0x0200,
    AREA_SANCTUARY          = 0x0800,
    AREA_ISLAND             = 0x1000,
	AREA_ALLOW_FLYING		= 0x4000,
    AREA_PVP_OBJECTIVE_AREA = 0x8000,
	*/
	/* 3.0.9 (Branruz)
    AREA_00000001           = 0x00000001, // ?? Dun Morogh / The Storm Peaks 
    AREA_CITY_CAPITAL       = 0x00000008, // Ville/Capitale 
	AREA_CITY_CAPITAL2      = 0x00000010, // Ville/Capitale 
	AREA_CITY_AREA          = 0x00000020, // Ok capitales seulement 
    AREA_NEUTRAL_AREA       = 0x00000040, // Ok (Tout sauf instance, en gros)
    AREA_PVP_ARENA          = 0x00000080, // Ok Battle Ring
	AREA_CITY_CAPITAL3      = 0x00000100, // Ville/Capitale 
//	AREA_CITY               = 0x00000200, // 
//	AREA_0x00000400         = 0x00000400, // ?? Programmer Isle / Dragonblight
	AREA_FLYING_PERMITTED	= 0x00000400, // Note randdrick : Zone de vol autorisée - Ascent 313
    AREA_SANCTUARY          = 0x00000800, // Shattrath / Dalaran
    AREA_ISLAND             = 0x00001000, // pas verif
	AREA_ALLOW_FLYING		= 0x00004000, // NOk, entre autre "Warsong Gulch" ou double sens
    AREA_PVP_OBJECTIVE_AREA = 0x00008000, // Ok , genre "Tour a prendre" 
	AREA_FFA                = 0x00010000, // ?? Blade's Edge Arena 
	AREA_CITY               = 0x00200000, // Ville/Capitale/Village 
	AREA_CITY_ARENA         = 0x04000000, // ?? Undercity / Dalaran Arena*/
	
	// 3.3.2 Hearstone
	AREA_SNOW					= 0x00000001, 	// Snow (only Dun Morogh, Naxxramas, Razorfen Downs and Winterspring)
	AREA_CAPITAL_SUB			= 0x00000008, 	// City and city subsones
	AREA_CITY_AREA				= 0x00000020, 	// Slave capital city flag?
	AREA_NEUTRAL_AREA			= 0x00000040, 	// Many zones have this flag
	AREA_PVP_ARENA				= 0x00000080, 	// Arena, both instanced and world arenas
	AREA_CAPITAL				= 0x00000100, 	// Main capital city flag
	AREA_CITY					= 0x00000200, 	// Only for one zone named "City" (where it located?)
	AREA_FLYING_PERMITTED		= 0x00000400, 	// Expansion zones? (only Eye of the Storm not have this flag, but have 0x00004000 flag)
	AREA_SANCTUARY				= 0x00000800, 	// Sanctuary area (PvP disabled)
	AREA_ISLAND					= 0x00001000, 	// Only Netherwing Ledge, Socrethar's Seat, Tempest Keep, The Arcatraz, The Botanica, The Mechanar, Sorrow Wing Point, Dragonspine Ridge, Netherwing Mines, Dragonmaw Base Camp, Dragonmaw Skyway
	AREA_OUTLAND2				= 0x00004000, 	// Expansion zones? (only Circle of Blood Arena not have this flag, but have 0x00000400 flag)
	AREA_PVP_OBJECTIVE_AREA		= 0x00008000, 	// Pvp objective area? (Death's Door also has this flag although it's no pvp object area)
	AREA_ARENA_INSTANCE			= 0x00010000, 	// Used by instanced arenas only
	AREA_LOWLEVEL				= 0x00100000, 	// Used for some starting areas with area_level <=15
	AREA_TOWN					= 0x00200000, 	// Small towns with Inn
	AREA_OUTDOOR_PVP			= 0x01000000, 	// Wintergrasp and it's subzones
	AREA_INSIDE					= 0x02000000, 	// Used for determinating spell related inside/outside questions in Map::IsOutdoors
	AREA_OUTSIDE				= 0x04000000, 	// Used for determinating spell related inside/outside questions in Map::IsOutdoors
	AREA_CAN_HEARTH_AND_RES		= 0x08000000, 	// Wintergrasp and it's subzones
	AREA_CANNOT_FLY				= 0x20000000	// Not allowed to fly, only used in Dalaran areas (zone 4395)
};
/*
enum AREATABLE_CATEGORY
{
    AREAC_CONTESTED          = 0,
    AREAC_ALLIANCE_TERRITORY = 2,
    AREAC_HORDE_TERRITORY    = 4,
    AREAC_SANCTUARY          = 6,
};
*/
// WaadTeam, redefinition plus clair
#define   AREAC_CONTESTED				0x00
#define   AREAC_ALLIANCE_TERRITORY      0x02
#define   AREAC_HORDE_TERRITORY         0x04
#define   AREAC_SANCTUARY               (AREAC_ALLIANCE_TERRITORY | AREAC_HORDE_TERRITORY)

// franck PetLevelUp
struct PetLevelUp {
	uint32 creatureFamily;
	uint32 spellLevel;
	uint32 spellId;
};

#define MAX_PREDEFINED_NEXTLEVELXP 250 // Correction de la table, 
                                      //  bcp plus raisonable +5500 au lv70 (Branruz)
static const uint32 NextLevelXp[MAX_PREDEFINED_NEXTLEVELXP]= 
{
	400,	900,	1400,	2100,	2800,	3600,	4500,	5400,	6500,	7600,    //   1 a 10
	8700,	9800,	11000,	12300,	13600,	15000,	16400,	17800,	19300,	20800,   //  11 a 20
	22400,	24000,	25500,	27200,	28900,	30500,	32200,	33900,	36300,	38800,   //  21 a 30
	41600,	44600,	48000,	51400,	55000,	58700,	62400,	66200,	70200,	74300,   //  31 a 40
	78500,	82800,	87100,	91600,	95300,	101000,	105800,	110700,	115700,	120900,  //  41 a 50
	126100,	131500,	137000,	142500,	148200,	154000,	159900,	165800,	172000,	177500,  //  51 a 60
	177500,	183000,	188500,	194000,	199500,	205000,	210500,	216000,	221500,	227000,  //  61 a 70
    232500,	238000,	243500,	249000,	254500,	260000,	265500,	271000,	276500,	282000,  //  71 a 80
    287500,	293000,	298500,	304000,	309500,	315000,	320500,	326000,	331500,	337000,  //  81 a 90
	342500,	348000,	353500,	359000,	364500,	370000,	375500,	381000,	386500,	392000,  //  91 a 100
	397500,	403000,	408500,	414000,	419500,	425000,	430500,	436000,	441500,	447000,  // 101 a 110
	452500,	458000,	463500,	469000,	474500,	480000,	485500,	491000,	496500,	502000,  // 111 a 120 
	507500,	513000,	518500,	524000,	529500,	535000,	540500,	546000,	551500,	557000,  // 121 a 130
	562500,	568000,	573500,	579000,	584500,	590000,	595500,	601000,	606500,	612000,  // 131 a 140
	617500,	623000,	628500,	634000,	639500,	645000,	650500,	656000,	661500,	667000,  // 141 a 150
	672500,	678000,	683500,	689000,	694500,	700000,	705500,	711000,	716500,	722000,  // 151 a 160
	727500,	733000,	738500,	744000,	749500,	755000,	760500,	766000,	771500,	777000,  // 161 a 170
	782500,	788000,	793500,	799000,	804500,	810000,	815500,	821000,	826500,	832000,  // 171 a 180
	837500,	843000,	848500,	854000,	859500,	865000,	870500,	876000,	881500,	887000,  // 181 a 190
	892500,	898000,	903500,	909000,	914500,	920000,	925500,	931000,	936500,	942000,  // 191 a 200
	947500,	953000,	958500,	964000,	969500,	975000,	980500,	986000,	991500,	997000,  // 201 a 210
	1002500,1008000,1013500,1019000,1024500,1030000,1035500,1041000,1046500,1052000, // 211 a 220
	1057500,1063000,1068500,1074000,1079500,1085000,1090500,1096000,1101500,1107000, // 221 a 230
	1112500,1118000,1123500,1129000,1134500,1140000,1145500,1151000,1156500,1162000, // 231 a 240
	1167500,1173000,1178500,1184000,1189500,1195000,1200500,1206000,1211500,1217000  // 241 a 250
};

class SERVER_DECL GossipMenu
{
private:
    WorldSession* pSession;
	
public:
	GossipMenu(uint64 Creature_Guid, uint32 Text_Id);
	void AddItem(GossipMenuItem* GossipItem);
	void AddItem(uint8 Icon, const char* Text, int32 Id = -1, bool Coded = false, uint32 BoxMoney = 0, const char* BoxMessage = "");
	void AddItem(uint32 IndexPOI);
	void BuildPacket(WorldPacket& Packet);
	void SendTo(Player* Plr);
	void SendPointOfInterest( uint32 poi_id );
	GossipMenuItem GetItem(uint32 Id);
	ASCENT_INLINE void SetTextID(uint32 TID) { TextId = TID; }

protected:
	uint32 TextId;
	uint64 CreatureGuid;
	std::vector<GossipMenuItem> Menu;
	std::vector<GossipMenuAddPOI> POIMenu;
};

class Charter
{
public:
	uint32 GetNumberOfSlotsByType()
	{
		switch(CharterType)
		{
		case CHARTER_TYPE_GUILD:
			return 9;

		case CHARTER_TYPE_ARENA_2V2:
			return 1;

		case CHARTER_TYPE_ARENA_3V3:
			return 2;

		case CHARTER_TYPE_ARENA_5V5:
			return 4;

		default:
			return 9;
		}
	}

	uint32 SignatureCount;
	uint32 * Signatures;
	uint32 CharterType;
	uint32 Slots;
	uint32 LeaderGuid;
	uint64 ItemGuid;
	uint32 CharterId;
	string GuildName;

	Charter(Field * fields);
	Charter(uint32 id, uint32 leader, uint32 type) : CharterType(type), LeaderGuid(leader), CharterId(id)
	{
		SignatureCount = 0;
		ItemGuid = 0;
		Slots = GetNumberOfSlotsByType();
		Signatures = new uint32[Slots];
		memset(Signatures, 0, sizeof(uint32)*Slots);
	}

	~Charter()
	{
		delete [] Signatures;
	}
	
	void SaveToDB();
	void Destroy();		 // When item is deleted.

	void AddSignature(uint32 PlayerGuid);
	void RemoveSignature(uint32 PlayerGuid);

	ASCENT_INLINE uint32 GetLeader() { return LeaderGuid; }
	ASCENT_INLINE uint32 GetID() { return CharterId; }

	ASCENT_INLINE bool IsFull() { return (SignatureCount == Slots); }
};

typedef std::map<uint32, std::list<SpellEntry*>* >                  OverrideIdMap;
typedef HM_NAMESPACE::hash_map<uint32, Player*>                     PlayerStorageMap;
typedef std::list<GM_Ticket*>                                       GmTicketList;

#ifndef WIN32
#define ASCENT_USE_MAP_PLAYER_INDEX
#ifdef ASCENT_USE_MAP_PLAYER_INDEX

// you can use the string map (slower)
typedef map<string, PlayerInfo*> PlayerNameStringIndexMap;


#else			// or

// gcc has no default hash for string type,
// so we have to make an explicit hash template here
template<>
struct __gnu_cxx::hash<string>
{
	size_t operator()(string& tbh) const
	{
		// simple crc32 hash for now, we may need to change this later however
		return size_t( crc32( (const unsigned char*)tbh.c_str(), tbh.length() ) );
	}
}

typedef HM_NAMESPACE::hash_map<string, PlayerInfo*> PlayerNameStringIndexMap;

#endif
#else

// vc++ has the type for a string hash already, so we don't need to do anything special
typedef HM_NAMESPACE::hash_map<string, PlayerInfo*> PlayerNameStringIndexMap;

/*
// (franck) modif r60 leveluppet 
typedef std::map<uint32, uint32> PetLevelupSpellSet;
typedef std::map<uint32, PetLevelupSpellSet> PetLevelupSpellMap;
*/

#endif

class SERVER_DECL ObjectMgr : public Singleton < ObjectMgr >, public EventableObject
{
public:
	ObjectMgr();
	~ObjectMgr();
	void LoadCreatureWaypoints();

	// other objects
    
    // Set typedef's
	typedef HM_NAMESPACE::hash_map<uint32, Group*>						GroupMap;
	
    // HashMap typedef's
    typedef HM_NAMESPACE::hash_map<uint64, Item*>                       ItemMap;
	typedef HM_NAMESPACE::hash_map<uint32, CorpseData*>                 CorpseCollectorMap;
	typedef HM_NAMESPACE::hash_map<uint32, PlayerInfo*>                 PlayerNameMap;
	typedef HM_NAMESPACE::hash_map<uint32, PlayerCreateInfo*>           PlayerCreateInfoMap;
	typedef HM_NAMESPACE::hash_map<uint32, Guild*>                      GuildMap;
	typedef HM_NAMESPACE::hash_map<uint32, skilllinespell*>             SLMap;
	typedef HM_NAMESPACE::hash_map<uint32, std::vector<CreatureItem>*>  VendorMap;
    typedef HM_NAMESPACE::hash_map<uint32, Transporter*>                TransportMap;
	typedef HM_NAMESPACE::hash_map<uint32, Trainer*>                    TrainerMap;
	typedef HM_NAMESPACE::hash_map<uint32, std::vector<TrainerSpell*> > TrainerSpellMap;
    typedef HM_NAMESPACE::hash_map<uint32, ReputationModifier*>         ReputationModMap;
    typedef HM_NAMESPACE::hash_map<uint32, Corpse*>                     CorpseMap;
	typedef HM_NAMESPACE::hash_map<uint32, PointOfInterest>				PointOfInterestMap;
    
    // Map typedef's
    typedef std::map<uint32, LevelInfo*>                                LevelMap;
	typedef std::map<pair<uint32, uint32>, LevelMap* >                  LevelInfoMap;
    typedef std::map<uint32, std::list<ItemPrototype*>* >               ItemSetContentMap;
	typedef std::map<uint32, uint32>                                    NpcToGossipTextMap;
	typedef std::map<uint32, set<SpellEntry*> >                         PetDefaultSpellMap;
	typedef std::map<uint32, uint32>                                    PetSpellCooldownMap;
	typedef std::map<uint32, SpellEntry*>                               TotemSpellMap;

    // object holders
	GmTicketList        GM_TicketList;
	TotemSpellMap       m_totemSpells;
	OverrideIdMap       mOverrideIdMap;

	Player* GetPlayer(const char* name, bool caseSensitive = true);
	Player* GetPlayer(uint32 guid);
	
	CorpseMap m_corpses;
	Mutex _corpseslock;
	Mutex m_corpseguidlock;
    Mutex _TransportLock;
	uint32 m_hiCorpseGuid;
	uint32 m_hiGuildId;
	
    Mutex m_achievementLock; // Mutex ou Volatile ? (Branruz)
	AchievementCriteriaMap m_achievementCriteriaMap;

	Item * CreateItem(uint32 entry,Player * owner);
	Item * LoadItem(uint64 guid);
	Item * LoadExternalItem(uint64 guid);
  
	// Groups
	Group * GetGroupByLeader(Player *pPlayer);
	Group * GetGroupById(uint32 id);
	uint32 GenerateGroupId()
	{
		uint32 r;
		m_guidGenMutex.Acquire();
		r = ++m_hiGroupId;
		m_guidGenMutex.Release();
		return r;
	}

	uint32 GenerateGuildId()
	{
		uint32 r;
		m_guidGenMutex.Acquire();
		r = ++m_hiGuildId;
		m_guidGenMutex.Release();
		return r;
	}

	void AddGroup(Group* group)
	{
		m_groupLock.AcquireWriteLock();
		m_groups.insert(make_pair(group->GetID(), group));
		m_groupLock.ReleaseWriteLock();
	}

	void RemoveGroup(Group* group)
	{
		m_groupLock.AcquireWriteLock();
		m_groups.erase(group->GetID());
		m_groupLock.ReleaseWriteLock();
	}

	void GroupVoiceReconnected();

	void LoadGroups();

	// player names
	void AddPlayerInfo(PlayerInfo *pn);
	PlayerInfo *GetPlayerInfo(uint32 guid );
	PlayerInfo *GetPlayerInfoByName(const char * name);
	void RenamePlayerInfo(PlayerInfo * pn, const char * oldname, const char * newname);
	void DeletePlayerInfo(uint32 guid);
	PlayerCreateInfo* GetPlayerCreateInfo(uint8 race, uint8 class_) const;

    map<uint32,set<Quest*>*> ZoneToQuestMap;
	void LoadAchievements();

	// Guild
	void AddGuild(Guild *pGuild);
	uint32 GetTotalGuildCount();
	bool RemoveGuild(uint32 guildId);
	Guild* GetGuild(uint32 guildId);  
	Guild* GetGuildByLeaderGuid(uint64 leaderGuid);  
	Guild* GetGuildByGuildName(std::string guildName);

	//Corpse Stuff
	Corpse *GetCorpseByOwner(uint32 ownerguid);
	void CorpseCollectorUnload();
	void DespawnCorpse(uint64 Guid);
	void CorpseAddEventDespawn(Corpse *pCorpse);
	void DelinkPlayerCorpses(Player *pOwner);
	Corpse * CreateCorpse();
	void AddCorpse(Corpse*);
	void RemoveCorpse(Corpse*);
	Corpse * GetCorpse(uint32 corpseguid);

	uint32 GetGossipTextForNpc(uint32 ID);
	
	// POI - Quetes POI - Provenance Mangos -
	void LoadPointsOfInterest();
    PointOfInterest const* GetPointOfInterest(uint32 id) const
		{
           PointOfInterestMap::const_iterator itr = mPointsOfInterest.find(id);
           if(itr != mPointsOfInterest.end())
               return &itr->second;
           return NULL;
		}

	void LoadQuestPOI();
	QuestPOIVector const* GetQuestPOIVector(uint32 questId) const
		{
			QuestPOIMap::const_iterator itr = mQuestPOIMap.find(questId);
			if(itr != mQuestPOIMap.end())
				return &itr->second;
			return NULL;
		}
	
	// Gm Tickets
	void AddGMTicket(GM_Ticket *ticket,bool startup);
	void remGMTicket(uint64 guid);
	GM_Ticket* GetGMTicket(uint64 guid);

	skilllinespell* GetSpellSkill(uint32 id);

	//Vendors
	std::vector<CreatureItem> *GetVendorList(uint32 entry);
	void SetVendorList(uint32 Entry, std::vector<CreatureItem>* list_);

	//Totem
	SpellEntry* GetTotemSpell(uint32 spellId);

	std::list<ItemPrototype*>* GetListForItemSet(uint32 setid);

	Pet * CreatePet();
	uint32 m_hiPetGuid;
	uint32 m_hiArenaTeamId;
	uint32 GenerateArenaTeamId()
	{
		uint32 ret;
		m_arenaTeamLock.Acquire();
		ret = ++m_hiArenaTeamId;
		m_arenaTeamLock.Release();
		return ret;
	}

	Mutex m_petlock;

	Player * CreatePlayer();
	Mutex m_playerguidlock;
	PlayerStorageMap _players;
	RWLock _playerslock;
	uint32 m_hiPlayerGuid;
	
	void AddPlayer(Player * p);//add it to global storage
	void RemovePlayer(Player *p);


	// Serialization

	void LoadQuests();
	void LoadPlayersInfo();
	void LoadPlayerCreateInfo();
	void LoadGuilds();
	Corpse* LoadCorpse(uint32 guid);
	void LoadCorpses(MapMgr * mgr);
	void LoadGMTickets();
	void SaveGMTicket(uint64 guid, QueryBuffer * buf);
	void LoadAuctions();
	void LoadAuctionItems();
	void LoadSpellSkills();
	void LoadVendors();
	void ReloadVendors();
	void LoadTotemSpells();
	void LoadAIThreatToSpellId();
	//void LoadSpellFixes();
	void LoadSpellProcs();
	void LoadSpellEffectsOverride();
	void LoadReputationModifierTable(const char * tablename, ReputationModMap * dmap);
	void LoadReputationModifiers();
	ReputationModifier * GetReputationModifier(uint32 entry_id, uint32 faction_id);

	void SetHighestGuids();
	uint32 GenerateLowGuid(uint32 guidhigh);
	uint32 GenerateMailID();
	
	void LoadTransporters();
	void ProcessGameobjectQuests();
    void AddTransport(Transporter * pTransporter);
   
	void LoadTrainers();
	Trainer* GetTrainer(uint32 Entry);

	void LoadExtraItemStuff();
	void LoadExtraCreatureProtoStuff();
	void CreateGossipMenuForPlayer(GossipMenu** Location, uint64 Guid, uint32 TextID, Player* Plr); 

	LevelInfo * GetLevelInfo(uint32 Race, uint32 Class, uint32 Level);
	void GenerateLevelUpInfo();
	void LoadDefaultPetSpells();
	set<SpellEntry*>* GetDefaultPetSpells(uint32 Entry);
	uint32 GetPetSpellCooldown(uint32 SpellId);
	void LoadPetSpellCooldowns();
	WayPointMap * GetWayPointMap(uint32 spawnid);
	void LoadSpellOverride();	

    // (franck) modif leveluppet
	void LoadPetLevelupSpellMap();
	/*
	PetLevelupSpellSet const* GetPetLevelupSpellList(uint32 petFamily) const;
	*/
	vector<PetLevelUp> PetLevelUpList;
	uint32 WaadPetLevelUp (uint32 Family, uint32 Level);

	uint32 GenerateCreatureSpawnID()
	{
		m_CreatureSpawnIdMutex.Acquire();
		uint32 r = ++m_hiCreatureSpawnId;
		m_CreatureSpawnIdMutex.Release();
		return r;
	}

	uint32 GenerateGameObjectSpawnID()
	{
		m_GOSpawnIdMutex.Acquire();
		uint32 r = ++m_hiGameObjectSpawnId;
		m_GOSpawnIdMutex.Release();
		return r;
	}

	Transporter * GetTransporter(uint32 guid);
	Transporter * GetTransporterByEntry(uint32 entry);

	Charter * CreateCharter(uint32 LeaderGuid, CharterTypes Type);
	Charter * GetCharter(uint32 CharterId, CharterTypes Type);
	void RemoveCharter(Charter *);
	void LoadGuildCharters();
	Charter * GetCharterByName(string &charter_name, CharterTypes Type);
	Charter * GetCharterByItemGuid(uint64 guid);
	Charter * GetCharterByGuid(uint64 playerguid, CharterTypes type);

	ArenaTeam * GetArenaTeamByName(string & name, uint32 Type);
	ArenaTeam * GetArenaTeamById(uint32 id);
	ArenaTeam * GetArenaTeamByGuid(uint32 guid, uint32 Type);
	void UpdateArenaTeamRankings();
	void UpdateArenaTeamWeekly();
	void LoadArenaTeams();
	void ResetDailies();
	HM_NAMESPACE::hash_map<uint32, ArenaTeam*> m_arenaTeamMap[3];
	HM_NAMESPACE::hash_map<uint32, ArenaTeam*> m_arenaTeams;
	void RemoveArenaTeam(ArenaTeam * team);
	void AddArenaTeam(ArenaTeam * team);
	Mutex m_arenaTeamLock;

	typedef HM_NAMESPACE::hash_map<uint32, NpcMonsterSay*> MonsterSayMap;
	MonsterSayMap mMonsterSays[NUM_MONSTER_SAY_EVENTS];

	void HandleMonsterSayEvent(Creature * pCreature, MONSTER_SAY_EVENTS Event);
	bool HasMonsterSay(uint32 Entry, MONSTER_SAY_EVENTS Event);
	void LoadMonsterSay();

	bool HandleInstanceReputationModifiers(Player * pPlayer, Unit * pVictim);
	void LoadInstanceReputationModifiers();

	ASCENT_INLINE bool IsSpellDisabled(uint32 spellid)
	{
		if(m_disabled_spells.find(spellid) != m_disabled_spells.end())
			return true;
		return false;
	}

	void LoadDisabledSpells();
	ASCENT_INLINE GuildMap::iterator GetGuildsBegin() { return mGuild.begin(); }
	ASCENT_INLINE GuildMap::iterator GetGuildsEnd() { return mGuild.end(); }

	std::set<ProfessionDiscovery*> ProfessionDiscoveryTable;

protected:
	RWLock playernamelock;
	uint32 m_mailid;
	// highest GUIDs, used for creating new objects
	Mutex m_guidGenMutex;
    union
    {
	    uint32 m_hiItemGuid;
	    uint32 m_hiContainerGuid;
    };
	uint32 m_hiGroupId;
	uint32 m_hiCharterId;
	RWLock m_charterLock;

	ReputationModMap m_reputation_faction;
	ReputationModMap m_reputation_creature;
	HM_NAMESPACE::hash_map<uint32, InstanceReputationModifier*> m_reputation_instance;

	HM_NAMESPACE::hash_map<uint32, Charter*> m_charters[NUM_CHARTER_TYPES];
	
	set<uint32> m_disabled_spells;

	uint64 TransportersCount;
	HM_NAMESPACE::hash_map<uint32,PlayerInfo*> m_playersinfo;
	PlayerNameStringIndexMap m_playersInfoByName;
	
	HM_NAMESPACE::hash_map<uint32,WayPointMap*> m_waypoints;//stored by spawnid
	uint32 m_hiCreatureSpawnId;
	
	Mutex m_CreatureSpawnIdMutex;
	Mutex m_GOSpawnIdMutex;

	uint32 m_hiGameObjectSpawnId;
	
	///// Object Tables ////
	// These tables are modified as creatures are created and destroyed in the world

	// Group List
	RWLock m_groupLock;
	GroupMap m_groups;

	// Map of all starting infos needed for player creation
	PlayerCreateInfoMap mPlayerCreateInfo;

	// DK: Map of all Guild's
	GuildMap mGuild;

	// Map of all vendor goods
	VendorMap mVendors;

	// Maps for Gossip stuff
	NpcToGossipTextMap  mNpcToGossipText;

	SLMap				mSpellSkills;

	//Corpse Collector
	CorpseCollectorMap mCorpseCollector;

    TransportMap mTransports;

	ItemSetContentMap mItemSets;

	TrainerMap mTrainers;
	LevelInfoMap mLevelInfo;
	PetDefaultSpellMap mDefaultPetSpells;
	PetSpellCooldownMap mPetSpellCooldowns;
	
	//POI - Quests POI - Mangos -
	PointOfInterestMap  mPointsOfInterest;	
	QuestPOIMap         mQuestPOIMap;
	
	// modif (franck) petlevelup
	//PetLevelupSpellMap  mPetLevelupSpellMap;
};


#define objmgr ObjectMgr::getSingleton()

//void SetProgressBar(int, int, const char*);
//void ClearProgressBar();

#endif
