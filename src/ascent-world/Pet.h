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

#ifndef _PET_H
#define _PET_H

/* Taken from ItemPetFood.dbc
 * Each value is equal to a flag
 * so 1 << PET_FOOD_BREAD for example
 * will result in a usable value.
 */
enum PET_FOOD
{
	PET_FOOD_NONE,
	PET_FOOD_MEAT,     // Viande
	PET_FOOD_FISH,     // Poisson
	PET_FOOD_CHEESE,   // Fromage
	PET_FOOD_BREAD,    // Pain
	PET_FOOD_FUNGUS,   // Champignon
	PET_FOOD_FRUIT,    // Fruit
	PET_FOOD_RAW_MEAT, // Viande Cru
	PET_FOOD_RAW_FISH  // Poisson Cru
};

#define MAX_STABLE_SLOTS 4
//***********************
// Loyalty and happiness 
//***********************
// PetPersonality.dbc
#define PET_UNHAPPY_VALUE	   0
#define PET_CONTENT_VALUE 333000
#define PET_HAPPY_VALUE   666000
#define UNHAPPY_RATIO     (float)0.75
#define CONTENT_RATIO     (float)1.0
#define HAPPY_RATIO       (float)1.25
//-----

#define PET_FLEE_UPDATE_TIMER         60000 // 1min 
#define PET_HAPPINESS_UPDATE_TIMER     7500 // 7sec5
#define PET_PARTY_SPELLS_UPDATE_TIMER 10000 // 10sec
// Custom WaadFun
static const uint32 HappinessValue[4] = { 0, PET_UNHAPPY_VALUE, PET_CONTENT_VALUE, PET_HAPPY_VALUE };
static const uint32 ListenLoyRatio[7] = { 0, 15, 30, 45, 60, 80, 101 }; //loyalty ratio level ranges (100% is guessed)
//-----------
//static const uint32 HappinessTicks[6] = { 2783, 2088, 1670, 1392, 1193, 1044 }; //loose_happiness ticks per loyalty lvl for 7.5s timer
// A quoi sert le LoyaltyTicks ?? (Branruz)
//static const char LoyaltyTicks[3] = { -10, 5, 20 };//loyalty_ticks for unhappy, content, happy - PetPersonality.dbc N'existe plus 335 (Branruz)
enum HappinessState
{
	UNHAPPY		=1,
	CONTENT		=2,
	HAPPY		=3
};

static const uint16 LoyLvlRange[7] = { 0, 150, 300, 450, 600, 900, 1200 }; //loyalty level ranges (1200 is guessed)
enum _LoyaltyLevel
{
	REBELIOUS	=1, // Rebel
	UNRULY		=2, // Indiscipliné
	SUBMISIVE	=3, // Soumis
	DEPENDABLE	=4, // Sûr
	FAITHFUL	=5, // Fidele
	BEST_FRIEND	=6  // Meilleur ami
};



enum CREATURE_TYPE
{
	CREATURE_TYPE_CRITTER = 1,
	CREATURE_TYPE_PET     = 2,
	CREATURE_TYPE_VEHICLE = 4
};

enum PET_ACTION // 0x0700
{
	PET_ACTION_STAY,
	PET_ACTION_FOLLOW,
	PET_ACTION_ATTACK,
	PET_ACTION_DISMISS,
	PET_ACTION_CASTING,
};

enum PET_STATE // 0x0600
{
	PET_STATE_PASSIVE,
	PET_STATE_DEFENSIVE,
	PET_STATE_AGGRESSIVE
};

enum PET_SPELL
{
	PET_SPELL_PASSIVE   = 0x06000000,
	PET_SPELL_DEFENSIVE,
	PET_SPELL_AGRESSIVE,
	PET_SPELL_STAY      = 0x07000000,
	PET_SPELL_FOLLOW,
	PET_SPELL_ATTACK
};

enum StableState
{
	STABLE_STATE_ACTIVE		= 1,
	STABLE_STATE_PASSIVE	= 2
};

enum StableResult
{
	STABLERESULT_FAIL_CANT_AFFORD		= 0x01, // 0000 0001
	STABLERESULT_FAIL					= 0x06, // 0000 0110
	STABLERESULT_STABLE_SUCCESS			= 0x08, // 0000 1000
	STABLERESULT_UNSTABLE_SUCCESS		= 0x09, // 0000 1001
	STABLERESULT_BUY_SLOT_SUCCESS		= 0x0A  // 0000 1010
};

enum AutoCastEvents
{
	AUTOCAST_EVENT_NONE					= 0,
	AUTOCAST_EVENT_ATTACK				= 1,
	AUTOCAST_EVENT_ON_SPAWN				= 2,
	AUTOCAST_EVENT_OWNER_ATTACKED		= 3,
	AUTOCAST_EVENT_LEAVE_COMBAT			= 4,
	AUTOCAST_EVENT_COUNT				= 5,
};

#define PET_DELAYED_REMOVAL_TIME 60000  // 1 min


//TODO: grep see the way pet spells contain the same flag? => Oui, ca a l'air (Branruz)
#define PET_ACTION_ACTION         0x0700 // 0000 0[1]11 0000 0000 // 0 - 3
#define PET_ACTION_STATE	      0x0600 // 0000 0[1]10 0000 0000 // 0 - 2
#define PET_ACTION_SPELL          0x0100 // 0000 0 0 01 0000 0000 // 0 - 1
#define PET_UNKNOW_SPELL          0x4100 // 0100 0 0 01 0000 0000 // 1 - 1
#define DEFAULT_SPELL_STATE       0x8100 // 1000 0 0 01 0000 0000 // 2 - 1
#define AUTOCAST_SPELL_STATE      0xC100 // 1100 0 0 01 0000 0000 // 3 - 1
#define PET_SPELL_AUTOCAST_CHANCE 50

typedef map<SpellEntry*, uint16> PetSpellMap;
typedef map<uint32, uint8> PetTalentMap;
struct PlayerPet;

class SERVER_DECL Pet : public Creature
{
	friend class Player;
	friend class Creature;
	friend class WorldSession;

public:
	Pet(uint64 guid);
	~Pet();
	virtual void Init();
		
	void ApplyPassiveAuras();
	ASCENT_INLINE Unit* GetOwner() { return m_Owner; }
	ASCENT_INLINE void SetOwner(Unit* _Owner) { m_Owner = _Owner; }

	void LoadFromDB(Player* owner, PlayerPet * pi);
	void CreateAsSummon(uint32 entry, CreatureInfo* ci, Creature* created_from_creature, Unit* owner, SpellEntry* created_by_spell, uint32 type, uint32 expiretime, Spell* call_back=NULL);

	virtual void Update(uint32 time);
	//void OnPushToWorld();

	ASCENT_INLINE uint32 GetXP(void) { return m_PetXP; }

	void InitializeSpells();
	void ReInitializeSpells();
	void InitializeMe(bool first);
	void SendSpellsToOwner();
	void SendNullSpellsToOwner();

	ASCENT_INLINE void SetPetAction(uint32 act) { m_Action = act; }
	ASCENT_INLINE uint32 GetPetAction(void) { return m_Action; }

	ASCENT_INLINE void SetPetState(uint32 state) { m_State = state; }
	ASCENT_INLINE uint32 GetPetState(void) { return m_State; }

	ASCENT_INLINE void SetPetDiet(uint32 diet) { m_Diet = diet; }
	ASCENT_INLINE void SetPetDiet() { m_Diet = myFamily->petdietflags; }

	ASCENT_INLINE uint32 GetPetDiet(void) { return m_Diet; }
	
	ASCENT_INLINE AI_Spell* GetAISpellForSpellId(uint32 spellid)
	{
		std::map<uint32, AI_Spell*>::iterator itr = m_AISpellStore.find(spellid);
		if(itr != m_AISpellStore.end())
			return itr->second;
		else
			return NULL;
	}

	void UpdatePetInfo(bool bSetToOffline);
	void Remove(bool bSafeDelete, bool bUpdate, bool bSetOffline);
	void Dismiss(bool bSafeDelete = false);

	void DelayedRemove(bool bTime, bool bDeath);

	ASCENT_INLINE Unit* GetPetOwner() { return m_Owner; }
	ASCENT_INLINE void ClearPetOwner() { m_Owner = 0; }
	void GiveXP(uint32 xp);
	uint32 GetNextLevelXP(uint32 currentlevel);
	void UpdateStats();
	void ApplyStatsForLevel();
	void ApplySummonLevelAbilities();
	void ApplyPetLevelAbilities();

	void PetSafeDelete();

	void SetDefaultSpells();
	void SetDefaultActionbar();
	void SetActionBarSlot(uint32 slot, uint32 spell){ ActionBar[ slot ] = spell; };
	ASCENT_INLINE void SetCurrentBarAISpells(AI_Spell *aiSpell) { m_CurrentBarAISpell = aiSpell; };
	ASCENT_INLINE AI_Spell *GetCurrentBarAISpells() { return(m_CurrentBarAISpell); };
	ASCENT_INLINE void SetCurrentBarAISpellsCount(uint32 AISpellCount) { m_CurrentBarAISpellCount = AISpellCount; };
	ASCENT_INLINE uint32 GetCurrentBarAISpellsCount() { return(m_CurrentBarAISpellCount); };

	void LoadSpells();
	void AddSpell(SpellEntry * sp, bool learning);
	void LearnSpell(uint32 spellid);
	void LearnLevelupSpells();
	void RemoveSpell(SpellEntry * sp);
	void WipeSpells();
	uint32 GetUntrainCost();
	void SetSpellState(SpellEntry * sp, uint16 State);
	uint16 GetSpellState(SpellEntry * sp);
	ASCENT_INLINE void RemoveSpell(uint32 SpellID)
	{
		SpellEntry * sp = dbcSpell.LookupEntry(SpellID);
		if(sp) RemoveSpell(sp);
	}
	ASCENT_INLINE void SetSpellState(uint32 SpellID, uint16 State)
	{
		SpellEntry * sp = dbcSpell.LookupEntry(SpellID);
		if(sp) SetSpellState(sp, State);
	}
	ASCENT_INLINE uint16 GetSpellState(uint32 SpellID)
	{
		if(SpellID == 0)
			return DEFAULT_SPELL_STATE;

		SpellEntry * sp = dbcSpell.LookupEntry(SpellID);
		if(sp)
			return GetSpellState(sp);
		return DEFAULT_SPELL_STATE;
	}
	
	AI_Spell * CreateAISpell(SpellEntry * info);
	ASCENT_INLINE PetSpellMap* GetSpells() { return &mSpells; }
	ASCENT_INLINE bool IsSummon() { return Summon; }
	ASCENT_INLINE bool IsPNJ() { return(m_Pet_is_Pnj); }
	ASCENT_INLINE void SetIsPNJ(bool _Pet_is_Pnj) { m_Pet_is_Pnj = _Pet_is_Pnj; }

	void __fastcall SetAutoCastSpell(AI_Spell * sp);
	void Rename(string NewName);
	ASCENT_INLINE string& GetName() { return m_name; }
	void AddPetSpellToOwner(uint32 spellId);
	
	void HandleAutoCastEvent(uint32 Type);
	AI_Spell * HandleAutoCastEvent();
	void SetPetSpellState(uint32 spell, uint16 state);
	void SetAutoCast(AI_Spell * sp, bool on);
	float GetHappinessDmgMod() { return 0.25f * GetHappinessState() + 0.5f; };

	/* Pet Talents! */
	ASCENT_INLINE uint8 GetUnspentPetTalentPoints() { return GetByte( UNIT_FIELD_BYTES_1, 1); }
	ASCENT_INLINE void SetUnspentPetTalentPoints(uint8 points) { SetByte(UNIT_FIELD_BYTES_1, 1, points);}
	ASCENT_INLINE uint8 GetSpentPetTalentPoints() { return GetPetTalentPointsAtLevel() - GetUnspentPetTalentPoints(); }
	void InitTalentsForLevel(bool creating = false);
	bool ResetTalents(bool costs);
	void InitializeTalents();
	std::map<uint32, uint8> m_talents;

	void SetLoyaltyPts(uint32 loyalty_value) { m_LoyaltyPts = loyalty_value; }
	uint32 GetLoyaltyPts(void) { return(m_LoyaltyPts); }

	uint8 GetLoyaltyLevel(){ return (m_LoyaltyLevel) ; }
	void SetLoyaltyLevel(uint32 Loyalty_Level) { m_LoyaltyLevel = Loyalty_Level; }
	bool NotListen(void);
	
protected:
	bool bHasLoyalty;
	bool m_Pet_is_Pnj;
	Unit* m_Owner;
	uint32 m_PetXP;
	PetSpellMap mSpells;
	PlayerPet * m_PlayerPetInfo;
	uint32 ActionBar[10];   // 10 slots
	
	std::map<uint32, AI_Spell*> m_AISpellStore;

	uint32 m_AutoCombatSpell;

	uint32 m_PartySpellsUpdateTimer;
	uint32 m_HappinessTimer;
	uint32 m_PetNumber;

	uint32 m_Action;
	uint32 m_State;
	uint32 m_ExpireTime;
	uint32 m_Diet;
	uint64 m_OwnerGuid;
	AI_Spell *m_CurrentBarAISpell;
	uint32 m_CurrentBarAISpellCount;

	int16 TP;
	uint32 m_LoyaltyXP;
	uint32 m_LoyaltyPts;
	uint32 m_LoyaltyLevel;
	bool bExpires;
	bool Summon;
	string m_name;

	HappinessState GetHappinessState();
	uint32 GetHighestRankSpell(uint32 spellId);
	uint8 GetPetTalentPointsAtLevel();

	list<AI_Spell*> m_autoCastSpells[AUTOCAST_EVENT_COUNT];
	bool m_dismissed;
};


#endif
