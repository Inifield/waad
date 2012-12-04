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

#ifndef _OBJECT_H
#define _OBJECT_H

#include "LootMgr.h"

class Unit;
class Spell;
class Aura;
class DynamicObject;

// Quartet 1-3   : 0x000 : Player - 0xF13 : NPC - 0xF14 : Pet - 0xF15 : Vehicle 
// Quartet 4-10  : NPC Id
// Quartet 11-16 : NPC SpawnId
//           1 111111 
// 123 4567890 123456  
// 000 0000000 000000

enum HIGHGUID_TYPE
{
	HIGHGUID_TYPE_VEHICLE			= 0xF1500000,
	HIGHGUID_TYPE_UNIT				= 0xF1300000,
	HIGHGUID_TYPE_PET				= 0xF1400000,
	HIGHGUID_TYPE_GAMEOBJECT		= 0xF1100000,

	HIGHGUID_TYPE_ITEM				= 0x40000000,
	HIGHGUID_TYPE_CONTAINER			= 0x50000000,			
	HIGHGUID_TYPE_PLAYER			= 0x00000000,
	HIGHGUID_TYPE_DYNAMICOBJECT		= 0x60000000,
	HIGHGUID_TYPE_TRANSPORTER		= 0x1FC00000,
	HIGHGUID_TYPE_WAYPOINT			= 0x10000000,
	HIGHGUID_TYPE_CORPSE			= 0x30000000,
//===============================================
	HIGHGUID_TYPE_MASK				= 0xFFF00000,
	LOWGUID_ENTRY_MASK				= 0x00FFFFFF,
};

#define GET_TYPE_FROM_GUID(x) (GUID_HIPART((x)) & HIGHGUID_TYPE_MASK)
#define GET_LOWGUID_PART(x) (GUID_LOPART((x)) & LOWGUID_ENTRY_MASK)


// TODO: fix that type mess
#define _CONST_ID_	0x5A5AA5A5
enum TYPE {
	TYPE_OBJECT		    = 0x0001,
	TYPE_ITEM		    = 0x0002,
	TYPE_CONTAINER	    = 0x0004,
	TYPE_UNIT		    = 0x0008,
	TYPE_PLAYER		    = 0x0010,
	TYPE_GAMEOBJECT	    = 0x0020,
	TYPE_DYNAMICOBJECT  = 0x0040,
	TYPE_CORPSE		    = 0x0080,
	TYPE_AIGROUP		= 0x0100,
	TYPE_AREATRIGGER	= 0x0200
};

enum TYPEID {
	TYPEID_OBJECT		    = 0,
	TYPEID_ITEM		        = 1,
	TYPEID_CONTAINER	    = 2,
	TYPEID_UNIT		        = 3,
	TYPEID_PLAYER		    = 4,
	TYPEID_GAMEOBJECT	    = 5,
	TYPEID_DYNAMICOBJECT    = 6,
	TYPEID_CORPSE		    = 7,
	TYPEID_AIGROUP	        = 8,
	TYPEID_AREATRIGGER      = 9,
	TYPEID_UNUSED			= 10,//used to signal invalid reference (object dealocated but someone is still using it)
};

enum OBJECT_UPDATE_TYPE {
	UPDATETYPE_VALUES = 0,
	//  8 bytes - GUID
	//  Goto Update Block
	UPDATETYPE_MOVEMENT = 1,
	//  8 bytes - GUID
	//  Goto Position Update
	UPDATETYPE_CREATE_OBJECT = 2,
	//  8 bytes - GUID
	//  1 byte - Object Type (*)
	//  Goto Position Update
	//  Goto Update Block
	UPDATETYPE_CREATE_YOURSELF = 3, // looks like 3 & 4 do the same thing
	//  4 bytes - Count
	//  Loop Count Times:
	//  8 bytes - GUID
	UPDATETYPE_OUT_OF_RANGE_OBJECTS = 4 // <- this is correct, not sure about 3
	//  4 bytes - Count
	//  Loop Count Times:
	//  8 bytes - GUID

};

struct ProcFnc
{
	void* m_creator;
	CallbackBase* m_base;
	std::vector<void*>* m_data;
	bool deleted;

	ProcFnc() : m_creator(NULL), m_base(NULL), m_data(NULL), deleted(false) {}
};

enum PHASEMODE
{
	ALL_PHASES = -1,
	GHOST_PHASING = 0 , 
	NORMAL_PHASING  = 1 ,
	PHASE_2,
	PHASE_3, 
	PHASE_4,
	PHASE_5, // Derniere Phase trouvé ( Cf: Spells.dbc) (Branruz)
	// Au cas zou... RFU
	PHASE_6,
	PHASE_7,
	PHASE_8,
	PHASE_9,
	PHASE_10,
	MAX_PHASEMODE

};

class WorldPacket;
class ByteBuffer;
class WorldSession;
class Player;
class MapCell;
class MapMgr;
class Vehicle;

//====================================================================
//  Object
//  Base object for every item, unit, player, corpse, container, etc
//====================================================================
class SERVER_DECL Object : public EventableObject
{
public:
	typedef std::set<Object*> InRangeSet;
	typedef std::map<string, void*> ExtensionSet;
	
	//Object();
	virtual void Init();
	virtual ~Object();

    virtual void OnPositionChange() {};	

	// phasing
	uint32 m_Phase; //phases the object is in
	const uint32 GetPhase() { return m_Phase; }
	void SetPhase(uint32 Phase);
	void EnablePhase(uint32 Phase);
	void DisablePhase(uint32 Phase);
	//void RemovePhase(uint32 p);
	//void SetPhase(uint32 phase, bool notmask = false);

	bool CanActivateCell();
	virtual void Update ( uint32 time ) { }
  //! True if object exists in world

	
	void KillSelf();
 
	void DeleteOperator();
	void HandleDeath(Unit *pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras, Player* plr, bool isCritter);
	
	ASCENT_INLINE bool IsInWorld() { return m_mapMgr != NULL; }
	virtual void AddToWorld();
	virtual void AddToWorld(MapMgr * pMapMgr);
	void PushToWorld(MapMgr*);
	virtual void OnPushToWorld() { }
	virtual void OnPrePushToWorld() { }
	virtual void RemoveFromWorld(bool free_guid);

	// guid always comes first
#ifndef USING_BIG_ENDIAN
	ASCENT_INLINE const uint64& GetGUID() const { return *((uint64*)m_uint32Values); }
#else
	ASCENT_INLINE const uint64 GetGUID() const { return GetUInt64Value(0); }
#endif

	ASCENT_INLINE const WoWGuid& GetNewGUID() const { return m_wowGuid; }
	ASCENT_INLINE uint32 GetEntry(){return m_uint32Values[3];}
	
    ASCENT_INLINE void ObjLock() { m_objlock.Acquire(); }
	ASCENT_INLINE void ObjUnlock() { m_objlock.Release(); }

	ASCENT_INLINE const uint32 GetEntryFromGUID() const
	{
/*		uint64 entry = *(uint64*)m_uint32Values;
		entry >>= 24;
		return (uint32)(entry & 0xFFFFFFFF);*/

		return uint32( (*(uint64*)m_uint32Values >> 24) & 0xFFFFFFFF );
	}

	ASCENT_INLINE const uint32 GetTypeFromGUID() const { return (m_uint32Values[1] & HIGHGUID_TYPE_MASK); }
	ASCENT_INLINE const uint32 GetUIdFromGUID() const { return (m_uint32Values[0] & LOWGUID_ENTRY_MASK); }
	ASCENT_INLINE const uint32 GetLowGUID() const { return (m_uint32Values[0]); }
	ASCENT_INLINE const uint32 GetHighGUID() const { return (m_uint32Values[1]); }

	// type
	ASCENT_INLINE const uint8& GetTypeId() const { return m_objectTypeId; }
	ASCENT_INLINE bool IsUnit()	{ return ( (m_objectTypeId == TYPEID_UNIT) || (m_objectTypeId == TYPEID_PLAYER) ); }
	ASCENT_INLINE bool IsPlayer() { return m_objectTypeId == TYPEID_PLAYER; }
	ASCENT_INLINE bool IsCorpse() { return m_objectTypeId == TYPEID_CORPSE; }
	ASCENT_INLINE bool IsCreature() { return m_objectTypeId == TYPEID_UNIT; }
	ASCENT_INLINE bool IsItem() { return m_objectTypeId == TYPEID_ITEM; }
	ASCENT_INLINE bool IsGO() { return m_objectTypeId == TYPEID_GAMEOBJECT; }
	bool m_isVehicle;                                       // (HearthStone)
	ASCENT_INLINE bool IsVehicle() { return m_isVehicle; }  // (HearthStone)
	bool IsPet();
	ASCENT_INLINE bool IsFromPlayer() { return GetPlayerFrom() ? true : false; }
	Player* GetPlayerFrom();
	Player* GetPlayerFromItem();

	//! This includes any nested objects we have, inventory for example.
	virtual uint32 __fastcall BuildCreateUpdateBlockForPlayer( ByteBuffer *data, Player *target );
	uint32 __fastcall BuildValuesUpdateBlockForPlayer( ByteBuffer *buf, Player *target );
	uint32 __fastcall BuildValuesUpdateBlockForPlayer( ByteBuffer * buf, UpdateMask * mask );
	uint32 __fastcall BuildOutOfRangeUpdateBlock( ByteBuffer *buf );

	WorldPacket* BuildFieldUpdatePacket(uint32 index,uint32 value);
	void BuildFieldUpdatePacket(Player* Target, uint32 Index, uint32 Value);
	void BuildFieldUpdatePacket(ByteBuffer * buf, uint32 Index, uint32 Value);

	void DealDamage(Unit *pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras = false);
	

	virtual void DestroyForPlayer( Player *target ) const;

	void BuildHeartBeatMsg( WorldPacket *data ) const;
	WorldPacket * BuildTeleportAckMsg( const LocationVector & v);
	bool IsBeingTeleported() { return mSemaphoreTeleport; }
	void SetSemaphoreTeleport(bool semphsetting) { mSemaphoreTeleport = semphsetting; }

	void SetSpawnPosition(float newX, float newY, float newZ, float newOrientation);
	void SetSpawnPosition(const LocationVector & v);
	bool SetPosition( float newX, float newY, float newZ, float newOrientation, bool allowPorting = false );
	bool SetPosition( const LocationVector & v, bool allowPorting = false);
	void SetRotation( uint64 guid );
	
	//ASCENT_INLINE void EventSetPosition(float x, float y, float z, float o) { SetPosition(x, y, z, o, false); }

	void CastSpell(Object *Target, SpellEntry* Sp, bool triggered);
	void CastSpell(Object *Target, uint32 SpellID, bool triggered);
	void CastSpell(uint64 targetGuid, SpellEntry* Sp, bool triggered);
	void CastSpell(uint64 targetGuid, uint32 SpellID, bool triggered);

	ASCENT_INLINE const float& GetPositionX( ) const { return m_position.x; }
	ASCENT_INLINE const float& GetPositionY( ) const { return m_position.y; }
	ASCENT_INLINE const float& GetPositionZ( ) const { return m_position.z; }
	ASCENT_INLINE const float& GetOrientation( ) const { return m_position.o; }
	ASCENT_INLINE void SetOrientation( float &o ) { m_position.o = o; }

	ASCENT_INLINE const float& GetSpawnX( ) const { return m_spawnLocation.x; }
	ASCENT_INLINE const float& GetSpawnY( ) const { return m_spawnLocation.y; }
	ASCENT_INLINE const float& GetSpawnZ( ) const { return m_spawnLocation.z; }
	ASCENT_INLINE const float& GetSpawnO( ) const { return m_spawnLocation.o; }

	ASCENT_INLINE const LocationVector & GetPosition() { return m_position; }
	ASCENT_INLINE LocationVector & GetPositionNC() { return m_position; }
	ASCENT_INLINE LocationVector * GetPositionV() { return &m_position; }

	//Distance Calculation
	float CalcDistance(Object* Ob);
	float CalcDistance(float ObX, float ObY, float ObZ);
	float CalcDistance(Object *Oa, Object *Ob);
	float CalcDistance(Object *Oa, float ObX, float ObY, float ObZ);
	float CalcDistance(float OaX, float OaY, float OaZ, float ObX, float ObY, float ObZ);

	//! Only for MapMgr use
	ASCENT_INLINE MapCell* GetMapCell() const { return m_mapCell; }
	//! Only for MapMgr use
	ASCENT_INLINE void SetMapCell(MapCell* cell) { m_mapCell = cell; }
	//! Only for MapMgr use
	ASCENT_INLINE MapMgr* GetMapMgr() const { return m_mapMgr; }

	ASCENT_INLINE void SetMapId(uint32 newMap) { m_mapId = newMap; }
	void SetZoneId(uint32 newZone);

	ASCENT_INLINE const uint32 GetMapId( ) const { return m_mapId; }
	ASCENT_INLINE const uint32& GetZoneId( ) const { return m_zoneId; }

	//! Get uint32 property
	// franck r145 vire warning sur variable temp sous linux voir si oki sous windows
	//ASCENT_INLINE const uint32& GetUInt32Value( uint32 index ) const
	ASCENT_INLINE uint32 GetUInt32Value( uint32 index )
	{
	
		// Warning si m_valuesCount bug ... (voir modif de m_valuesCount)
		if ( m_valuesCount > 2000 )
		{
			Log.Error("[Object::GetUInt32Value]","Value of m_valuesCount (%u) to high <- Report this to dev.",m_valuesCount);
			m_valuesCount = 0; // Note Randdrick : On Reset m_valuesCount sous peine de devoir redémarrer le CORE. 
			return((uint32)NULL); 
		}
		
		//ASSERT( index < m_valuesCount );
		if(index < m_valuesCount) return m_uint32Values[ index ];
		else
		{
			Log.Error("[Object::GetUInt32Value]","Index (%u) < m_valuesCount (%u) <- Report this to dev.",index,m_valuesCount);
			//return((const uint32 &)NULL); 
			return((uint32)NULL); 
		}
	}

	//! Get uint64 property
#ifdef USING_BIG_ENDIAN
        __inline const uint64 GetUInt64Value( uint32 index ) const
#else
	ASCENT_INLINE const uint64& GetUInt64Value( uint32 index ) const
#endif
	{
		ASSERT( index + uint32(1) < m_valuesCount );
#ifdef USING_BIG_ENDIAN
		/* these have to be swapped here :< */
		return uint64((uint64(m_uint32Values[index+1]) << 32) | m_uint32Values[index]);
#else
		return *((uint64*)&(m_uint32Values[ index ]));
#endif
	}

	//! Get float property
	// franck r145 vire warning sur variable temp sous linux voir si oki sous windows
	//ASCENT_INLINE const float& GetFloatValue( uint32 index ) const
	float GetFloatValue( uint32 index )
	{
		//ASSERT( index < m_valuesCount );
		
		if( index < m_valuesCount )
		{
		 return m_floatValues[ index ];
		}
		else
		{
			Log.Error("[GetFloatValue]","index: %u, m_valuesCount: %u <- Report this to dev.",index , m_valuesCount);
			//return((const float &)NULL);
			return((float)NULL);
		}
	}

	void __fastcall ModFloatValue(const uint32 index, const float value );
	void __fastcall ModFloatValuePct(const uint32 index, const float value );
	void ModSignedInt32Value(uint32 index, int32 value);
	void ModUnsigned32Value(uint32 index, int32 mod);
	uint32 GetModPUInt32Value(const uint32 index, const int32 value);

	//! Set uint32 property
	void SetByte(uint32 index, uint32 index1,uint8 value);

	ASCENT_INLINE uint8 GetByte(uint32 i,uint32 i1)
	{
		ASSERT( i < m_valuesCount);
		ASSERT(i1 < 4);
#ifdef USING_BIG_ENDIAN
		return ((uint8*)m_uint32Values)[(i*4)+(3-i1)];
#else
		return ((uint8*)m_uint32Values)[(i*4)+i1];
#endif
	}
	
	ASCENT_INLINE void SetNewGuid(uint32 Guid)
	{
		uint64 new_full_guid = Guid;
		if( m_objectTypeId == TYPEID_GAMEOBJECT )
			new_full_guid |= ((uint64)GetEntry() << 24) | ((uint64)HIGHGUID_TYPE_GAMEOBJECT << 32);

		if( m_objectTypeId == TYPEID_UNIT )
			new_full_guid |= ((uint64)GetEntry() << 24) | ((uint64)HIGHGUID_TYPE_UNIT << 32);

		SetUInt64Value(OBJECT_FIELD_GUID, new_full_guid);
		m_wowGuid.Init(GetGUID());
	}

	void EventSetUInt32Value(uint32 index, uint32 value);
	void __fastcall SetUInt32Value( const uint32 index, const uint32 value );

	//! Set uint64 property
	void __fastcall SetUInt64Value( const uint32 index, const uint64 value );

	//! Set float property
	void __fastcall SetFloatValue( const uint32 index, const float value );

	void __fastcall SetFlag( const uint32 index, uint32 newFlag );
	void __fastcall RemoveFlag( const uint32 index, uint32 oldFlag );

	void __fastcall SetDamageFlag( const uint32 index, uint32 newFlag, bool gUpdate );
	void __fastcall RemoveDamageFlag( const uint32 index, uint32 oldFlag, bool gUpdate );

	ASCENT_INLINE bool HasFlag( const uint32 index, uint32 flag ) const
	{
		//ASSERT( index < m_valuesCount );
		if( index < m_valuesCount )
		{
			return( (m_uint32Values[ index ] & flag) != 0);
		}
		else
		{
			Log.Error("[Object::HasFlag]","index (%u) >= m_valuesCount (%u) (flag = %X) <- Report this to dev.",index,m_valuesCount,flag);
			return(false);
		}
		
	}

    bool HasFlagDamage( const uint32 index, uint32 flag ) const
	{
		if( index < m_valuesCount )
		{
			return( (m_uint32Values[ index ] ^ flag) == 0);
		}
		else
		{
			Log.Error("[Object::HasFlagDamage]","index (%u) >= m_valuesCount (%u) (flag = %X) <- Report this to dev.",index,m_valuesCount,flag);
			return(false);
		}
		
	}

	void __fastcall SetByteFlag( const uint32 index, const uint32 flag, uint8 newFlag);
	bool __fastcall HasByteFlag( const uint32 index, const uint32 flag, uint8 checkFlag);
	void __fastcall RemoveByteFlag(const uint32 index, const uint32 flag, uint8 checkFlag);
	void __fastcall SetFlag64( const uint32 index, uint64 newFlag );
	void __fastcall RemoveFlag64( const uint32 index, uint64 newFlag );

	
	////////////////////////////////////////
	void ClearUpdateMask( )
	{
		m_updateMask.Clear();
		m_objectUpdated = false;
	}

	bool HasUpdateField(uint32 index)
	{
		//ASSERT( index < m_valuesCount);
        if( index < m_valuesCount)
		{
		 return m_updateMask.GetBit(index);
		}
		else
		{
			Log.Error("[Object::HasUpdateField]","index (%u) >= m_valuesCount (%u) <- Report this to dev.",index,m_valuesCount);
			return(false);
		}
	}

	//use it to check if a object is in range of another
	bool isInRange(Object* target, float range);

	// Use it to Check if a object is in front of another one
	bool isInFront(Object* target);
	bool isInBack(Object* target);
	// Check to see if an object is in front of a target in a specified arc (in degrees)
	bool isInArc(Object* target , float degrees); 

	/* Calculates the angle between two Positions */
	float calcAngle( float Position1X, float Position1Y, float Position2X, float Position2Y );
	float calcRadAngle( float Position1X, float Position1Y, float Position2X, float Position2Y );

	/* converts to 360 > x > 0 */
	float getEasyAngle( float angle );

	ASCENT_INLINE const float GetDistanceSq(Object* obj)
	{
		if(obj->GetMapId() != m_mapId) return 40000.0f; //enough for out of range
		return m_position.DistanceSq(obj->GetPosition());
	}

	ASCENT_INLINE float GetDistanceSq(LocationVector & comp)
	{
		return comp.DistanceSq(m_position);
	}

	ASCENT_INLINE float CalcDistance(LocationVector & comp)
	{
		return comp.Distance(m_position);
	}

	ASCENT_INLINE const float GetDistanceSq(float x, float y, float z)
	{
		return m_position.DistanceSq(x, y, z);
	}

	ASCENT_INLINE const float GetDistance2dSq( Object* obj )
	{
		if( obj->GetMapId() != m_mapId )
			return 40000.0f; //enough for out of range
		return m_position.Distance2DSq( obj->m_position );
	}

	ASCENT_INLINE float GetDistance2dSq(float x, float y)
	{
		return m_position.Distance2DSq( x, y );
	}


	// In-range object management, not sure if we need it

	//! Set of Objects in range.
	//! TODO: that functionality should be moved into WorldServer.
	std::set<Object*> m_objectsPendingUpdate;
	std::set<Object*> m_objectsInRange;
	std::set<Player*> m_inRangePlayers;
	std::set<Object*> m_oppFactsInRange;


	ASCENT_INLINE bool IsInRangeSet( Object* pObj )
	{
		return !( m_objectsInRange.find( pObj ) == m_objectsInRange.end() );
	}
	
	virtual void AddInRangeObject(Object* pObj)
	{
		if( pObj == NULL )
			return;

		m_objectsInRange.insert( pObj );

		// NOTES: Since someone will come along and try and change it.
		// Don't reinrepret_cast has to be used static_cast will not work when we are
		// inside the class we are casting from if we want to cast up the inheritance
		// chain, as Object has no concept of Player.

		if( pObj->GetTypeId() == TYPEID_PLAYER )
			m_inRangePlayers.insert( reinterpret_cast< Player* >( pObj ) );
	}

	ASCENT_INLINE void RemoveInRangeObject( Object* pObj )
	{
		if( pObj == NULL )
			return;

		OnRemoveInRangeObject( pObj );
		m_objectsInRange.erase( pObj );
	}

	ASCENT_INLINE bool HasInRangeObjects()
	{
		return ( m_objectsInRange.size() > 0 );
	}

	virtual void OnRemoveInRangeObject( Object * pObj )
	{
		if( pObj->GetTypeId() == TYPEID_PLAYER )
			 m_inRangePlayers.erase( reinterpret_cast< Player* >( pObj ) );
	}

	virtual void ClearInRangeSet()
	{
		m_objectsInRange.clear();
		m_inRangePlayers.clear();
		m_oppFactsInRange.clear();
	}

	ASCENT_INLINE size_t GetInRangeCount() { return m_objectsInRange.size(); }
	ASCENT_INLINE size_t GetInRangePlayersCount() { return m_inRangePlayers.size();}
	ASCENT_INLINE InRangeSet::iterator GetInRangeSetBegin() { return m_objectsInRange.begin(); }
	ASCENT_INLINE InRangeSet::iterator GetInRangeSetEnd() { return m_objectsInRange.end(); }
	ASCENT_INLINE InRangeSet::iterator FindInRangeSet(Object * obj) { return m_objectsInRange.find(obj); }

	void RemoveInRangeObject(InRangeSet::iterator itr)
	{ 
		OnRemoveInRangeObject(*itr);
		m_objectsInRange.erase(itr);
	}

	ASCENT_INLINE bool RemoveIfInRange( Object * obj )
	{
		InRangeSet::iterator itr = m_objectsInRange.find(obj);
		if( obj->GetTypeId() == TYPEID_PLAYER )
			m_inRangePlayers.erase( reinterpret_cast< Player* >( obj ) );

		if( itr == m_objectsInRange.end() )
			return false;
		
		m_objectsInRange.erase( itr );
		return true;
	}

	ASCENT_INLINE void AddInRangePlayer( Object * obj )
	{
		m_inRangePlayers.insert( reinterpret_cast< Player* >( obj ) );
	}

	ASCENT_INLINE void RemoveInRangePlayer( Object * obj )
	{
		m_inRangePlayers.erase( reinterpret_cast< Player* >( obj ) );
	}

	bool IsInRangeOppFactSet(Object* pObj) { return (m_oppFactsInRange.count(pObj) > 0); }
	void UpdateOppFactionSet();
	ASCENT_INLINE std::set<Object*>::iterator GetInRangeOppFactsSetBegin() { return m_oppFactsInRange.begin(); }
	ASCENT_INLINE std::set<Object*>::iterator GetInRangeOppFactsSetEnd() { return m_oppFactsInRange.end(); }
	ASCENT_INLINE std::set<Player*>::iterator GetInRangePlayerSetBegin() { return m_inRangePlayers.begin(); }
	ASCENT_INLINE std::set<Player*>::iterator GetInRangePlayerSetEnd() { return m_inRangePlayers.end(); }
	ASCENT_INLINE std::set<Player*> * GetInRangePlayerSet() { return &m_inRangePlayers; };

	void __fastcall SendMessageToSet(WorldPacket *data, bool self,bool myteam_only=false, uint64 ignoreguid=0);
	ASCENT_INLINE void SendMessageToSet(StackPacket * data, bool self) { OutPacketToSet(data->GetOpcode(), (uint16)data->GetSize(), data->GetBufferPointer(), self); }
	void OutPacketToSet(uint16 Opcode, uint16 Len, const void * Data, bool self);

	//! Fill values with data from a space seperated string of uint32s.
	void LoadValues(const char* data);

	ASCENT_INLINE uint16 GetValuesCount() const { return m_valuesCount; }

	//! Blizzard seem to send those for all object types. weird.
	float m_walkSpeed;
	float m_runSpeed;
	float m_backWalkSpeed;
	float m_swimSpeed;
	float m_backSwimSpeed;
	float m_turnRate;
	float m_flySpeed;
	float m_backFlySpeed;

	float m_base_runSpeed;
	float m_base_walkSpeed;

	void EventSpellDamage(uint64 Victim, uint32 SpellID, uint32 Damage);
	void SpellNonMeleeDamageLog(Unit *pVictim, uint32 spellID, uint32 damage, bool allowProc, bool static_damage = false, bool no_remove_auras = false, int32 forced_school = -1);
	
	//*****************************************************************************************
	//* SpellLog packets just to keep the code cleaner and better to read
	//*****************************************************************************************
	void SendSpellLog(Object *Caster, Object *Target,uint32 Ability, uint8 SpellLogType);
	void SendSpellNonMeleeDamageLog( Object* Caster, Object* Target, uint32 SpellID, uint32 Damage, uint8 School, uint32 AbsorbedDamage, uint32 ResistedDamage, bool PhysicalDamage, uint32 BlockedDamage, bool CriticalHit, bool bToSet );

	//object faction
	void _setFaction();
	uint32 _getFaction(){return m_faction->Faction;}
	
	FactionTemplateDBC *m_faction;
	FactionDBC *m_factionDBC;

	ASCENT_INLINE void SetInstanceID(int32 instance) { m_instanceId = instance; }
	ASCENT_INLINE int32 GetInstanceID() { return m_instanceId; }

	int32 event_GetInstanceID();

	bool Active;
	bool CanActivate();
	void Activate(MapMgr * mgr);
	void Deactivate(MapMgr * mgr);
	bool m_inQueue;
	ASCENT_INLINE void SetMapMgr(MapMgr * mgr) { m_mapMgr = mgr; }

	void Delete()
	{
		if(IsInWorld())
			RemoveFromWorld(true);
		// delete this;
	}

	void GMScriptEvent(void * function, uint32 argc, uint32 * argv, uint32 * argt);
	ASCENT_INLINE size_t GetInRangeOppFactCount() { return m_oppFactsInRange.size(); }
	void PlaySoundToSet(uint32 sound_entry);

	bool PhasedCanInteract(Object *pObj);
	//bool HasPhase() { return m_phaseMode != 0; }

protected:
	Object (  );

	//void _Create (uint32 guidlow, uint32 guidhigh);
	void _Create( uint32 mapid, float x, float y, float z, float ang);

	//! Mark values that need updating for specified player.
	virtual void _SetUpdateBits(UpdateMask *updateMask, Player *target) const;
	//! Mark values that player should get when he/she/it sees object for first time.
	virtual void _SetCreateBits(UpdateMask *updateMask, Player *target) const;

	void _BuildMovementUpdate( ByteBuffer *data, uint16 flags, uint32 flags2, Player* target );
	void _BuildValuesUpdate( ByteBuffer *data, UpdateMask *updateMask, Player* target );

	Mutex m_objlock;

	//! WoWGuid class
	WoWGuid m_wowGuid;

	 //! Type id.
	uint8 m_objectTypeId;

	//! Zone id.
	uint32 m_zoneId;
	//! Continent/map id.
	uint32 m_mapId;
	//! Map manager
	MapMgr *m_mapMgr;
	//! Current map cell
	MapCell *m_mapCell;

	/* Main Function called by isInFront(); */
	bool inArc(float Position1X, float Position1Y, float FOV, float Orientation, float Position2X, float Position2Y );
	
	// int32 m_phaseMode;

	LocationVector m_position;
	LocationVector m_lastMapUpdatePosition;
	LocationVector m_spawnLocation;

	// Semaphores - needed to forbid two operations on the same object at the same very time (may cause crashing\lack of data)
	bool mSemaphoreTeleport;

	//! Object properties.
	union {
		uint32* m_uint32Values;
		float* m_floatValues;
	};

	//! Number of properties
	uint32 m_valuesCount;

	//! List of object properties that need updating.
	UpdateMask m_updateMask;

	//! True if object was updated
	bool m_objectUpdated;

	//! Remove object from map
	void _RemoveFromMap();

	int32 m_instanceId;
	
	// Collision
	float m_Taille; // Taille "physique"

	ExtensionSet * m_extensions;
	void _SetExtension(const string& name, void* ptr);		// so we can set from scripts. :)

public:

	template<typename T>
		void SetExtension(const string& name, T ptr)
	{
		_SetExtension(name, ((void*)ptr));
	}

	template<typename T>
		T GetExtension(const string& name)
	{
		if( m_extensions == NULL )
			return ((T)NULL);
		else
		{
			ExtensionSet::iterator itr = m_extensions->find( name );
			if( itr == m_extensions->end() )
				return ((T)NULL);
			else
				return ((T)itr->second);
		}
	}

	bool m_loadedFromDB;

	virtual const char* GetObjectName() { return "Unknown"; }; 

	/************************************************************************/
	/* ACCESSOR FUNCTIONS                                                   */
	/************************************************************************/
	// Stats
	ASCENT_INLINE uint32 GetStrength() { return m_uint32Values[UNIT_FIELD_STRENGTH]; }
	ASCENT_INLINE uint32 GetAgility() { return m_uint32Values[UNIT_FIELD_AGILITY]; }
	ASCENT_INLINE uint32 GetStamina() { return m_uint32Values[UNIT_FIELD_STAMINA]; }
	ASCENT_INLINE uint32 GetIntellect() { return m_uint32Values[UNIT_FIELD_INTELLECT]; }
	ASCENT_INLINE uint32 GetSpirit() { return m_uint32Values[UNIT_FIELD_SPIRIT]; }

	// Health
	ASCENT_INLINE uint32 GetHealth() { return m_uint32Values[UNIT_FIELD_HEALTH]; }
	ASCENT_INLINE uint32 GetMaxHealth() { return m_uint32Values[UNIT_FIELD_MAXHEALTH]; }

	bool IsInLineOfSight(Object* pObj);
	int32 GetSpellBaseCost(SpellEntry *sp);

	/************************************************************************/
	/* END ACCESSOR FUNCTIONS                                               */
	/************************************************************************/
	
public:
	// AntiLeak
	static const uint32 m_ConstanteId = _CONST_ID_;
	// loooooot
	Loot m_loot;
	bool m_looted;

	// empties loot vector
	void ClearLoot();

	float GetSize(); 
};

#endif


