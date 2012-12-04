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
#include "Unit.h"
using namespace std;

//#define DEG2RAD (M_PI/180.0)
#define M_PI		3.14159265358979323846
#define M_H_PI		1.57079632679489661923
#define M_Q_PI		0.785398163397448309615

Object::Object() : m_position(0,0,0,0), m_spawnLocation(0,0,0,0)
{
	m_Phase = NORMAL_PHASING;
	POINTERLOGNEW(this);
	m_mapId = 0;
	m_zoneId = 0;

	m_uint32Values = 0;
	m_objectUpdated = false;


	m_valuesCount = 0;

	//official Values
	m_walkSpeed = 2.5f;
	m_runSpeed = 8.0f;
	m_base_runSpeed = m_runSpeed;
	m_base_walkSpeed = m_walkSpeed;

	m_flySpeed = 7.0f;
	m_backFlySpeed = 4.5f;

	m_backWalkSpeed = 4.5f;	// this should really be named m_backRunSpeed
	m_swimSpeed = 4.722222f;
	m_backSwimSpeed = 2.5f;
	m_turnRate = 3.141593f;

	m_mapMgr = 0;
	m_mapCell = 0;

	mSemaphoreTeleport = false;


	m_faction = NULL;
	m_factionDBC = NULL;

	m_instanceId = 0;
	Active = false;
	m_inQueue = false;
	m_extensions = NULL;
	m_loadedFromDB = false;

	// HearthStone (Vehicule)
	m_isVehicle = false;

	m_looted = false;
	m_objectsInRange.clear();
	m_inRangePlayers.clear();
	m_oppFactsInRange.clear();
}

void Object::Init()
{
}

Object::~Object()
{
	POINTERLOGDELETE(this);
	if(m_objectTypeId != TYPEID_ITEM)
		ASSERT(!m_inQueue);

	if (this->IsInWorld() && m_objectTypeId != TYPEID_ITEM && m_objectTypeId != TYPEID_CONTAINER)
	{
		this->RemoveFromWorld(false);
	}

	ClearInRangeSet();

	// for linux
	m_instanceId = -1;
	m_objectTypeId=TYPEID_UNUSED;

	if( m_extensions != NULL )
		delete m_extensions;

	sEventMgr.RemoveEvents(this);
}



void Object::_Create( uint32 mapid, float x, float y, float z, float ang )
{
	m_mapId = mapid;
	m_position.ChangeCoords(x, y, z, ang);
	m_spawnLocation.ChangeCoords(x, y, z, ang);
	m_lastMapUpdatePosition.ChangeCoords(x,y,z,ang);
}

uint32 Object::BuildCreateUpdateBlockForPlayer(ByteBuffer *data, Player *target)
{
	uint16 flags = 0;
	uint32 flags2 = 0;

	uint8 updatetype = UPDATETYPE_CREATE_OBJECT;
	if(m_objectTypeId == TYPEID_CORPSE)
	{
		if(m_uint32Values[CORPSE_FIELD_DISPLAY_ID] == 0)
			return 0;
	}

	// any other case
	switch(m_objectTypeId)
	{
		// items + containers: 0x8
	case TYPEID_ITEM:
	case TYPEID_CONTAINER:
		flags = 0x0010;
		break;
		
		// player/unit: 0x68 (except self)
	case TYPEID_UNIT:
		{
		flags  = 0x0070;
		}	
	case TYPEID_PLAYER:
		{
		flags  = 0x0070;
		}	
		break;

		// gameobject/dynamicobject
	case TYPEID_GAMEOBJECT:
		flags = 0x0350;
		break;

	case TYPEID_DYNAMICOBJECT:
		flags = 0x0048;
		break;

	case TYPEID_CORPSE:
		flags = 0x0148;
		break;
	}

	if(GetTypeFromGUID() == HIGHGUID_TYPE_VEHICLE)
		flags |= 0x00E0;

	if(target == this)
	{
		// player creating self
		flags |= 0x0001;
		updatetype = UPDATETYPE_CREATE_YOURSELF;
	}

	// gameobject stuff
	if(m_objectTypeId == TYPEID_GAMEOBJECT)
	{
		switch(GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID))
		{
			case GAMEOBJECT_TYPE_MO_TRANSPORT:  
				{
					if(GetTypeFromGUID() != HIGHGUID_TYPE_TRANSPORTER)
						return 0;   // bad transporter
					else
						flags = 0x0352;
				}break;

			case GAMEOBJECT_TYPE_TRANSPORT:
				{
					/* deeprun tram, etc */
					flags = 0x0352;
				}break;

			case GAMEOBJECT_TYPE_DUEL_ARBITER:
				{
					// duel flags have to stay as updatetype 3, otherwise
					// it won't animate
					updatetype = UPDATETYPE_CREATE_YOURSELF;
				}break;
		}
	}

	// build our actual update
	*data << updatetype;

	// we shouldn't be here, under any cercumstances, unless we have a wowguid..
	ASSERT(m_wowGuid.GetNewGuidLen());
	*data << m_wowGuid;
	
	*data << m_objectTypeId;

	_BuildMovementUpdate(data, flags, flags2, target);

	// we have dirty data, or are creating for ourself.
	UpdateMask updateMask;
	updateMask.SetCount( m_valuesCount );
	_SetCreateBits( &updateMask, target );

	// this will cache automatically if needed
	_BuildValuesUpdate( data, &updateMask, target );

	// update count: 1 ;)
	return 1;
}


//That is dirty fix it actually creates update of 1 field with
//the given value ignoring existing changes in fields and so on
//usefull if we want update this field for certain players
//NOTE: it does not change fields. This is also very fast method
WorldPacket *Object::BuildFieldUpdatePacket( uint32 index,uint32 value)
{
   // uint64 guidfields = GetGUID();
   // uint8 guidmask = 0;
	WorldPacket * packet=new WorldPacket(1500);
	packet->SetOpcode( SMSG_UPDATE_OBJECT );
	
	*packet << (uint32)1;//number of update/create blocks
//	*packet << (uint8)0;//unknown //VLack: removed for 3.1

	*packet << (uint8) UPDATETYPE_VALUES;		// update type == update
	*packet << GetNewGUID();

	uint32 mBlocks = index/32+1;
	*packet << (uint8)mBlocks;
	
	for(uint32 dword_n=mBlocks-1;dword_n;dword_n--)
	*packet <<(uint32)0;

	*packet <<(((uint32)(1))<<(index%32));
	*packet << value;
	
	return packet;
}

void Object::BuildFieldUpdatePacket(Player* Target, uint32 Index, uint32 Value)
{
	ByteBuffer buf(500);

	BuildFieldUpdatePacket(&buf, Index, Value);
	Target->PushUpdateData(&buf, 1);
}

void Object::BuildFieldUpdatePacket(ByteBuffer * buf, uint32 Index, uint32 Value)
{
	*buf << uint8(UPDATETYPE_VALUES);
	*buf << GetNewGUID();

	uint32 mBlocks = Index/32+1;
	*buf << (uint8)mBlocks;

	for(uint32 dword_n=mBlocks-1;dword_n;dword_n--)
		*buf <<(uint32)0;

	*buf <<(((uint32)(1))<<(Index%32));
	*buf << Value;
}

uint32 Object::BuildValuesUpdateBlockForPlayer(ByteBuffer *data, Player *target)
{
	UpdateMask updateMask;
	updateMask.SetCount( m_valuesCount );
	_SetUpdateBits( &updateMask, target );
	for(uint32 x = 0; x < m_valuesCount; ++x)
	{
		if(updateMask.GetBit(x))
		{
			*data << (uint8) UPDATETYPE_VALUES;		// update type == update
			ASSERT(m_wowGuid.GetNewGuidLen());
			*data << m_wowGuid;

			_BuildValuesUpdate( data, &updateMask, target );
			return 1;
		}
	}

	return 0;
}

uint32 Object::BuildValuesUpdateBlockForPlayer(ByteBuffer * buf, UpdateMask * mask )
{
	// returns: update count
	*buf << (uint8) UPDATETYPE_VALUES;		// update type == update

	ASSERT(m_wowGuid.GetNewGuidLen());
	*buf << m_wowGuid;

	_BuildValuesUpdate( buf, mask, NULL );

	// 1 update.
	return 1;
}

void Object::DestroyForPlayer(Player *target) const
{
	if(!target || target->GetSession() == 0) return;

	WorldPacket data(SMSG_DESTROY_OBJECT, 8);
	data << GetGUID();
	data << uint8(0);

	target->GetSession()->SendPacket( &data );
}


///////////////////////////////////////////////////////////////
/// Build the Movement Data portion of the update packet
/// Fills the data with this object's movement/speed info
/// TODO: rewrite this stuff, document unknown fields and flags
uint32 TimeStamp();

void Object::_BuildMovementUpdate(ByteBuffer * data, uint16 flags, uint32 flags2, Player* target )
{
	ByteBuffer *splinebuf = (m_objectTypeId == TYPEID_UNIT) ? target->GetAndRemoveSplinePacket(GetGUID()) : 0;
	uint16 flag16 = 0;	// some other flag
	*data << (uint16)flags;

	Player * pThis = NULL;
	MovementInfo* moveinfo;
	if(m_objectTypeId == TYPEID_PLAYER)
	{
		pThis = static_cast< Player* >( this );
		if(pThis->GetSession())
			moveinfo = pThis->GetSession()->GetMovementInfo();
		if(target == pThis)
		{
			// Updating our last speeds.
			pThis->UpdateLastSpeeds();
		}
	}

	//0x20 else 0x40, client does this, so we have to
	if (flags & 0x20)
	{
		if(pThis && pThis->m_TransporterGUID != 0)
			flags2 |= 0x200;
		else if(m_objectTypeId==TYPEID_UNIT && ((Creature*)this)->m_transportGuid != 0 && ((Creature*)this)->m_transportPosition != NULL)
			flags2 |= 0x200;
		else if (IsUnit() && ((Unit*)this)->m_CurrentVehicle != NULL)
			flags2 |= 0x200;

		if(splinebuf)
		{
			flags2 |= 0x08000001;	   //1=move forward
			if(GetTypeId() == TYPEID_UNIT)
			{
				if(static_cast<Unit*>(this)->GetAIInterface()->m_moveRun == false)
					flags2 |= 0x100;	//100=walk
			}
		}

		if(GetTypeId() == TYPEID_UNIT)
		{
			//		 Don't know what this is, but I've only seen it applied to spirit healers.
			//		 maybe some sort of invisibility flag? :/

			switch(GetEntry())
			{
			case 6491:  // Spirit Healer
			case 13116: // Alliance Spirit Guide
			case 13117: // Horde Spirit Guide
				{
					flags2 |= 0x10000000;
				}break;
			}
		
			if(static_cast<Unit*>(this)->GetAIInterface()->IsFlying())
				flags2 |= 0x400;
			if(static_cast<Creature*>(this)->proto && static_cast<Creature*>(this)->proto->extra_a9_flags)
			{
				if(!(flags2 & 0x0200))
					flags2 |= static_cast<Creature*>(this)->proto->extra_a9_flags;
			}
		}

		*data << (uint32)flags2;

		*data << (uint16)flag16;

		*data << getMSTime(); // this appears to be time in ms but can be any thing

		//wtf are these?
		*data << float(m_position.x);
		*data << float(m_position.y);
		*data << float(m_position.z);
		*data << float(m_position.o);

		if ( flags2 & 0x0200 )	//BYTE1(flags2) & 2
		{
			if (IsUnit() && static_cast<Unit *>(this)->m_CurrentVehicle != NULL)
			{
				Unit *pUnit = static_cast<Unit *>(this);
				Vehicle *vehicle = static_cast<Unit *>(this)->m_CurrentVehicle;

				if (pUnit->m_inVehicleSeatId != 0xFF && vehicle->GetVehicleSeatEntry(pUnit->m_inVehicleSeatId) != NULL)
				{
					*data << pUnit->m_CurrentVehicle->GetNewGUID();
					*data << vehicle->GetVehicleSeatEntry(pUnit->m_inVehicleSeatId)->m_attachmentOffsetX;
					*data << vehicle->GetVehicleSeatEntry(pUnit->m_inVehicleSeatId)->m_attachmentOffsetY;
					*data << vehicle->GetVehicleSeatEntry(pUnit->m_inVehicleSeatId)->m_attachmentOffsetZ;
					*data << float(0.0f);
					*data << uint32(0);
					*data << pUnit->m_inVehicleSeatId;
				}
				else
				{
					*data << uint8(0);
					*data << float(0);
					*data << float(0);
					*data << float(0);
					*data << float(0);
					*data << uint32(0);
					*data << uint8(0);
				}
			}
			else if(pThis)
			{
				WoWGuid wowguid(pThis->m_TransporterGUID);
				*data << wowguid;
				*data << pThis->m_TransporterX;
				*data << pThis->m_TransporterY;
				*data << pThis->m_TransporterZ;
				*data << pThis->m_TransporterO;
				*data << pThis->m_TransporterUnk; //this should be uint32
				*data << uint8(0);
			}
			else if(m_objectTypeId==TYPEID_UNIT && ((Creature*)this)->m_transportPosition != NULL)
			{
				
				*data << static_cast<Creature *>(this)->m_transportNewGuid;
				*data << uint32(HIGHGUID_TYPE_TRANSPORTER);
				*data << ((Creature*)this)->m_transportPosition->x;
				*data << ((Creature*)this)->m_transportPosition->y;
				*data << ((Creature*)this)->m_transportPosition->z;
				*data << ((Creature*)this)->m_transportPosition->o;
				*data << uint32(0.0f);
				*data << uint8(0);
			}
		}

		if(flags2 & 0x2200000 || flag16 & 0x20) //flying/swimming, && unk sth to do with vehicles?
		{
			if(pThis && moveinfo)
			{
				*data << moveinfo->pitch;
			}
			*data << (float)0; //pitch
		}
		if(pThis && moveinfo)
		{
			*data << moveinfo->unklast;
		}
		else
		*data << (uint32)0; //last fall time

		if(flags2 & 0x1000) // BYTE1(flags2) & 0x10
		{
			if(pThis && moveinfo)
			{
				*data << moveinfo->FallTime;
				*data << moveinfo->unk8;
				*data << moveinfo->unk9;
				*data << moveinfo->unk10;
			}
			*data << (float)0;
			*data << (float)1.0;
			*data << (float)0;
			*data << (float)0;
		}
		if( flags2 & 0x4000000 )
		{
			*data << (float)0; //unknown float
		}

		*data << m_walkSpeed;	 // walk speed
		*data << m_runSpeed;	  // run speed
		*data << m_backWalkSpeed; // backwards run speed
		*data << m_swimSpeed;	 // swim speed
		*data << m_backSwimSpeed; // backwards swim speed
		*data << m_flySpeed;		// fly speed
		*data << m_backFlySpeed;	// back fly speed
		*data << m_turnRate;	  // turn rate
		*data << float(7);			//pitch rate

		if(splinebuf)	// client expects that flags2 & 0x8000000 != 0 in this case
		{
			data->append(*splinebuf);
			delete splinebuf;
		}
	}
	else if(flags & 0x100)
    {
			*data << uint8(0);                              // unk PGUID!
			*data << (float)m_position.x;
			*data << (float)m_position.y;
			*data << (float)m_position.z;
			*data << (float)m_position.x;
			*data << (float)m_position.y;
			*data << (float)m_position.z;
			*data << (float)m_position.o;
			*data << float(0);
	}
	else if(flags & 0x40)
	{
		if( (flags & 0x02) && 
			(GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID) == GAMEOBJECT_TYPE_MO_TRANSPORT))
		{
			*data << float(0);
			*data << float(0);
			*data << float(0);
		}
		else
		{
			*data << (float)m_position.x;
			*data << (float)m_position.y;
			*data << (float)m_position.z;
		}
			*data << (float)m_position.o;
	}

	if(flags & 8)
	{
		*data << GetUInt32Value(OBJECT_FIELD_GUID);
	}
	
	if(flags & 0x0010)
		*data << GetUInt32Value(OBJECT_FIELD_GUID+1);

	if(flags & 0x0004)
	{
		*data << uint8(0);// unknown NewGUID
	}

	if(flags & 2)
	{
        *data << (uint32)getMSTime();
	}

	if (flags & 0x80) //if ((_BYTE)flags_ < 0)
	{
			*data << static_cast<Vehicle *>(this)->GetVehicleEntry() << float(0.0f);
	}

	// 0x200
    if(flags & 0x0200)
    {
		uint64 rotation = 0;
		if(IsGO())
			rotation = static_cast<GameObject *>(this)->m_rotation;
        *data << uint64(rotation); //blizz 64bit rotation
    }
}

//=======================================================================================
//  Creates an update block with the values of this object as
//  determined by the updateMask.
//=======================================================================================
void Object::_BuildValuesUpdate(ByteBuffer * data, UpdateMask *updateMask, Player* target)
{
	bool activate_quest_object = false;
	bool reset = false;
	uint32 oldflags = 0;

	if(updateMask->GetBit(OBJECT_FIELD_GUID) && target)	   // We're creating.
	{
		Creature * pThis = static_cast<Creature*>(this);
		if(GetTypeId() == TYPEID_UNIT && pThis->Tagged && (pThis->loot.gold || pThis->loot.items.size()))
		{
			// Let's see if we're the tagger or not.
			oldflags = m_uint32Values[UNIT_DYNAMIC_FLAGS];
			uint32 Flags = m_uint32Values[UNIT_DYNAMIC_FLAGS];
			uint32 oldFlags = 0;

			if(pThis->TaggerGuid == target->GetGUID())

			{
				// Our target is our tagger.
				oldFlags = U_DYN_FLAG_TAGGED_BY_OTHER;

				if(Flags & U_DYN_FLAG_TAGGED_BY_OTHER)
					Flags &= ~oldFlags;

				if(!(Flags & U_DYN_FLAG_LOOTABLE))
					Flags |= U_DYN_FLAG_LOOTABLE;
			}
			else
			{
				// Target is not the tagger.
				oldFlags = U_DYN_FLAG_LOOTABLE;

				if(!(Flags & U_DYN_FLAG_TAGGED_BY_OTHER))
					Flags |= U_DYN_FLAG_TAGGED_BY_OTHER;

				if(Flags & U_DYN_FLAG_LOOTABLE)
					Flags &= ~oldFlags;
			}

			m_uint32Values[UNIT_DYNAMIC_FLAGS] = Flags;

			updateMask->SetBit(UNIT_DYNAMIC_FLAGS);
			
			reset = true;
		}

		if(target && GetTypeId() == TYPEID_GAMEOBJECT)
		{
			GameObject *go = ((GameObject*)this);
			QuestLogEntry *qle;
			GameObjectInfo *info;
			if( go->HasQuests() )
			{
				activate_quest_object = true;
			}
			else
			{
				info = go->GetInfo();
				if( info && ( info->goMap.size() || info->itemMap.size() ) )
				{
					for( GameObjectGOMap::iterator itr = go->GetInfo()->goMap.begin(); itr != go->GetInfo()->goMap.end(); ++itr )
					{
						qle = target->GetQuestLogForEntry( itr->first->id );
						if( qle != NULL )
						{
							if( qle->GetQuest()->count_required_mob == 0 )
								continue;
							for( uint32 i = 0; i < 4; ++i )
							{
								if( qle->GetQuest()->required_mob[i] == go->GetEntry() && qle->GetMobCount(i) < qle->GetQuest()->required_mobcount[i])
								{
									activate_quest_object = true;
									break;
								}
							}
							if(activate_quest_object)
								break;
						}
					}

					if(!activate_quest_object)
					{
						for(GameObjectItemMap::iterator itr = go->GetInfo()->itemMap.begin();
							itr != go->GetInfo()->itemMap.end();
							++itr)
						{
							for(std::map<uint32, uint32>::iterator it2 = itr->second.begin();
								it2 != itr->second.end(); 
								++it2)
							{
								qle = target->GetQuestLogForEntry(itr->first->id);
								if(qle != NULL)
								{
									if(target->GetItemInterface()->GetItemCount(it2->first) < it2->second)
									{
										activate_quest_object = true;
										break;
									}
								}
							}
							if(activate_quest_object)
								break;
						}
					}
				}
			}
		}
	}


	if(activate_quest_object)
	{
		oldflags = m_uint32Values[GAMEOBJECT_DYNAMIC];
		if(!updateMask->GetBit(GAMEOBJECT_DYNAMIC))
			updateMask->SetBit(GAMEOBJECT_DYNAMIC);
		m_uint32Values[GAMEOBJECT_DYNAMIC] = 1;
		reset = true;
	}

	WPAssert( updateMask && updateMask->GetCount() == m_valuesCount );
	uint32 bc;
	uint32 values_count;
	if( m_valuesCount > ( 2 * 0x20 ) )//if number of blocks > 2->  unit and player+item container
	{
		bc = updateMask->GetUpdateBlockCount();
		values_count = (uint32)min( bc * 32, m_valuesCount );

	}
	else
	{
		bc=updateMask->GetBlockCount();
		values_count=m_valuesCount;
	}

	*data << (uint8)bc;
	data->append( updateMask->GetMask(), bc*4 );
	  
	for( uint32 index = 0; index < values_count; ++index )
	{
		if( updateMask->GetBit( index ) )
		{
			*data << m_uint32Values[ index ];
			updateMask->UnsetBit( index );
		}
	}

	if(reset)
	{
		switch(GetTypeId())
		{
		case TYPEID_UNIT:
			m_uint32Values[UNIT_DYNAMIC_FLAGS] = oldflags;
			break;
		case TYPEID_GAMEOBJECT:
			m_uint32Values[GAMEOBJECT_DYNAMIC] = oldflags;
			break;
		}
	}

}

void Object::BuildHeartBeatMsg(WorldPacket *data) const
{
	data->Initialize(MSG_MOVE_HEARTBEAT);

	*data << GetGUID();

	*data << uint32(0); // flags
	*data << getMSTime(); // mysterious value #1

	*data << m_position;
	*data << m_position.o;
}

WorldPacket * Object::BuildTeleportAckMsg(const LocationVector & v)
{
	///////////////////////////////////////
	//Update player on the client with TELEPORT_ACK
	static_cast< Player* >( this )->SetPlayerStatus( TRANSFER_PENDING );

	WorldPacket * data = new WorldPacket(MSG_MOVE_TELEPORT_ACK, 80);
	*data << GetNewGUID();

	*data << uint32(2); // flags
	*data << getMSTime(); // mysterious value #1
	*data << uint16(0);

	*data << float(0);
	*data << v;
	*data << v.o;
	*data << uint16(2);
	*data << uint8(0);
	return data;
}

void Object::SetSpawnPosition(const LocationVector & v)
{
	SetSpawnPosition(v.x, v.y, v.z, v.o);
}

void Object::SetSpawnPosition(float newX, float newY, float newZ, float newOrientation)
{
	m_spawnLocation.x=newX;
	m_spawnLocation.y=newY;
	m_spawnLocation.z=newZ;
	m_spawnLocation.o=newOrientation;
}


bool Object::SetPosition(const LocationVector & v, bool allowPorting /* = false */)
{
	bool updateMap = false, result = true;

	if (m_position.x != v.x || m_position.y != v.y)
		updateMap = true;

	m_position = const_cast<LocationVector&>(v);

	if (!allowPorting && v.z < -500)
	{
		m_position.z = 500;
		sLog.outError( "setPosition: fell through map; height ported" );

		result = false;
	}

	if (IsInWorld() && updateMap)
	{
		m_mapMgr->ChangeObjectLocation(this);
	}

	return result;
}

bool Object::SetPosition( float newX, float newY, float newZ, float newOrientation, bool allowPorting )
{
	bool updateMap = false, result = true;

	//if (m_position.x != newX || m_position.y != newY)
		//updateMap = true;
	if(m_lastMapUpdatePosition.Distance2DSq(newX, newY) > 4.0f)		/* 2.0f */
		updateMap = true;

	m_position.ChangeCoords(newX, newY, newZ, newOrientation);

	if (!allowPorting && newZ < -500)
	{
		m_position.z = 500;
		sLog.outError( "setPosition: fell through map; height ported" );

		result = false;
	}

	if (IsInWorld() && updateMap)
	{
		m_lastMapUpdatePosition.ChangeCoords(newX,newY,newZ,newOrientation);
		m_mapMgr->ChangeObjectLocation(this);

		if( m_objectTypeId == TYPEID_PLAYER && static_cast< Player* >( this )->GetGroup() && static_cast< Player* >( this )->m_last_group_position.Distance2DSq(m_position) > 25.0f ) // distance of 5.0
		{
            static_cast< Player* >( this )->GetGroup()->HandlePartialChange( PARTY_UPDATE_FLAG_POSITION, static_cast< Player* >( this ) );
		}	
	}

	return result;
}

void Object::SetRotation( uint64 guid )
{
	WorldPacket data(SMSG_AI_REACTION, 12);
	data << guid;
	data << uint32(2);
	SendMessageToSet(&data, false);
}

void Object::OutPacketToSet(uint16 Opcode, uint16 Len, const void * Data, bool self)
{
	if(self && m_objectTypeId == TYPEID_PLAYER)
		static_cast< Player* >( this )->GetSession()->OutPacket(Opcode, Len, Data);

	if(!IsInWorld())
		return;

	std::set<Player*>::iterator itr = m_inRangePlayers.begin();
	std::set<Player*>::iterator it_end = m_inRangePlayers.end();
	int gm = ( m_objectTypeId == TYPEID_PLAYER ? static_cast< Player* >( this )->m_isGmInvisible : 0 );
	for(; itr != it_end; ++itr)
	{
		//ASSERT((*itr)->GetSession());
		if( (*itr)->GetSession() == NULL)
		{
			Log.Error("Object::OutPacketToSet","FATAL: std::set<Player*>::iterator itr");
			break;
		}
		if( gm )
		{
			if( (*itr)->GetSession()->HasPermissions() != NULL )
				(*itr)->GetSession()->OutPacket(Opcode, Len, Data);
		}
		else
		{
			(*itr)->GetSession()->OutPacket(Opcode, Len, Data);
		}
	}
}

void Object::SendMessageToSet(WorldPacket *data, bool bToSelf, bool myteam_only, uint64 ignoreguid)
{
	if(bToSelf && m_objectTypeId == TYPEID_PLAYER)
	{
		if (static_cast<Player*>(this)->GetSession() != NULL)
			static_cast< Player* >( this )->GetSession()->SendPacket(data);		
	}

	if(!IsInWorld())
		return;

	std::set<Player*>::iterator itr = m_inRangePlayers.begin();
	std::set<Player*>::iterator it_end = m_inRangePlayers.end();
	bool gminvis = (m_objectTypeId == TYPEID_PLAYER ? static_cast< Player* >( this )->m_isGmInvisible : false);
	//Zehamster: Splitting into if/else allows us to avoid testing "gminvis==true" at each loop...
	//		   saving cpu cycles. Chat messages will be sent to everybody even if player is invisible.
	if(myteam_only)
	{
		uint32 myteam=static_cast< Player* >( this )->GetTeam();
		if(gminvis && data->GetOpcode()!=SMSG_MESSAGECHAT)
		{
			for(; itr != it_end; ++itr)
			{
				if ((*itr)->GetSession() == NULL)
					continue;
				if((*itr)->GetSession()->HasPermissions() != NULL && (*itr)->GetTeam()==myteam && PhasedCanInteract(*itr))
					(*itr)->GetSession()->SendPacket(data);
			}
		}
		else
		{
			for(; itr != it_end; ++itr)
			{
				if ((*itr)->GetSession() == NULL)
					continue;
				if((*itr)->GetTeam()==myteam && PhasedCanInteract(*itr))
					(*itr)->GetSession()->SendPacket(data);
			}
		}
	}
	else
	{
		if(gminvis && data->GetOpcode()!=SMSG_MESSAGECHAT)
		{
			for(; itr != it_end; ++itr)
			{
				if ((*itr)->GetSession() == NULL)
					continue;
				if ((*itr)->GetGUID() == ignoreguid)
					continue;

				if((*itr)->GetSession()->HasPermissions() != NULL && PhasedCanInteract(*itr))
					(*itr)->GetSession()->SendPacket(data);
			}
		}
		else
		{
			for(; itr != it_end; ++itr)
			{
				ASSERT((*itr)->GetSession());
				if ((*itr)->GetGUID() == ignoreguid)
					continue;
				if( PhasedCanInteract(*itr) )
				(*itr)->GetSession()->SendPacket(data);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////
/// Fill the object's Update Values from a space deliminated list of values.
void Object::LoadValues(const char* data)
{
	// thread-safe ;) strtok is not.
	std::string ndata = data;
	std::string::size_type last_pos = 0, pos = 0;
	uint32 index = 0;
	uint32 val;
	do 
	{
		// prevent overflow
		if(index >= m_valuesCount)
		{
			break;
		}
		pos = ndata.find(" ", last_pos);
		val = atol(ndata.substr(last_pos, (pos-last_pos)).c_str());
		if(m_uint32Values[index] == 0)
			m_uint32Values[index] = val;
		last_pos = pos+1;
		++index;
	} while(pos != std::string::npos);
}

void Object::_SetUpdateBits(UpdateMask *updateMask, Player *target) const
{
	*updateMask = m_updateMask;
}


void Object::_SetCreateBits(UpdateMask *updateMask, Player *target) const
{
	/*for( uint16 index = 0; index < m_valuesCount; index++ )
	{
		if(GetUInt32Value(index) != 0)
			updateMask->SetBit(index);
	}*/
	for(uint32 i = 0; i < m_valuesCount; ++i)
		if(m_uint32Values[i] != 0)
			updateMask->SetBit(i);
}

void Object::AddToWorld()
{
	MapMgr *mapMgr = sInstanceMgr.GetInstance(this);
	if(!mapMgr || (this->IsPlayer() && mapMgr->GetMapInfo()->type != INSTANCE_NULL && mapMgr->GetPlayerCount() >= mapMgr->GetMapInfo()->playerlimit))
		return; //instance add failed

	m_mapMgr = mapMgr;
	m_inQueue = true;

	mapMgr->AddObject(this);

	// correct incorrect instance id's
	m_instanceId = m_mapMgr->GetInstanceID();

	mSemaphoreTeleport = false;
}

void Object::AddToWorld(MapMgr * pMapMgr)
{
	if(!pMapMgr || (this->IsPlayer() && pMapMgr->GetMapInfo()->type != INSTANCE_NULL && pMapMgr->GetPlayerCount() >= pMapMgr->GetMapInfo()->playerlimit))
		return; //instance add failed

	m_mapMgr = pMapMgr;
	m_inQueue = true;

	pMapMgr->AddObject(this);

	// correct incorrect instance id's
	m_instanceId = pMapMgr->GetInstanceID();

	mSemaphoreTeleport = false;
}

//Unlike addtoworld it pushes it directly ignoring add pool
//this can only be called from the thread of mapmgr!!!
void Object::PushToWorld(MapMgr*mgr)
{
	if(mgr == NULL)
	{
		sLog.outError("Invalid push to world of Object "I64FMT, GetGUID());
 		return; //instance add failed
	}

	m_mapId=mgr->GetMapId();
	m_instanceId = mgr->GetInstanceID();

	m_mapMgr = mgr;
	OnPrePushToWorld();

	mgr->PushObject(this);

	// correct incorrect instance id's
	mSemaphoreTeleport = false;
	m_inQueue = false;
   
	event_Relocate();
	
	// call virtual function to handle stuff.. :P
	OnPushToWorld();
}

void Object::RemoveFromWorld(bool free_guid)
{
	// clear loot
	ClearLoot();
	
	//ASSERT(m_mapMgr);
	if(m_mapMgr == NULL)
	{
		Log.Error("Object::RemoveFromWorld","FATAL: m_mapMgr == NULL");
		return;
	}

	MapMgr * m = m_mapMgr;
	m_mapMgr = 0;

	mSemaphoreTeleport = true;

	m->RemoveObject(this, free_guid);
	
	// remove any spells / free memory
	sEventMgr.RemoveEvents(this, EVENT_UNIT_SPELL_HIT);

	// update our event holder
	event_Relocate();
}

//! Set uint32 property
void Object::SetUInt32Value( const uint32 index, const uint32 value )
{
	// Franck77
	//ASSERT( index < m_valuesCount );
	if ( index >= m_valuesCount ) // m_valuesCount = GAMEOBJECT_END
	{   // Nickel Franck, il s'agit de l'index du OBJECT_FIELD_CREATED_BY qui pose probleme
		// Index etant (normalement) superieur a cette valeur, je la soustrait pour plus de clarté
		// (Les champs sont definis dans le fichier UpdateField.h) (Branruz)
		//Log.Error("[SetUInt32Value]","Object Field %u hors limite!!! Value: %u (m_valuesCount:%u)",
		//	           (index-OBJECT_FIELD_CREATED_BY),value,m_valuesCount-OBJECT_FIELD_CREATED_BY); 	
		Log.Error("[SetUInt32Value]","index: %u, m_valuesCount: %u <- Report this to dev.",index , m_valuesCount);
		return;
	}
	// save updating when val isn't changing.
	if(m_uint32Values[index] == value)
		return;

	m_uint32Values[ index ] = value;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}

	// Group update handling
	if(m_objectTypeId == TYPEID_PLAYER)
	{
		if(IsInWorld())
		{
			Group* pGroup = static_cast< Player* >( this )->GetGroup();
			if( pGroup != NULL )
				pGroup->HandleUpdateFieldChange( index, static_cast< Player* >( this ) );
		}

#ifdef OPTIMIZED_PLAYER_SAVING
		switch(index)
		{
		case UNIT_FIELD_LEVEL:
		case PLAYER_XP:
			static_cast< Player* >( this )->save_LevelXP();
			break;

		case PLAYER_FIELD_COINAGE:
			static_cast< Player* >( this )->save_Gold();
			break;
		}
#endif
	}
}
/*
//must be in %
void Object::ModPUInt32Value(const uint32 index, const int32 value, bool apply )
{
	ASSERT( index < m_valuesCount );
	int32 basevalue = (int32)m_uint32Values[ index ];
	if(apply)
		m_uint32Values[ index ] += ((basevalue*value)/100);
	else
		m_uint32Values[ index ] = (basevalue*100)/(100+value);

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated )
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}
*/
uint32 Object::GetModPUInt32Value(const uint32 index, const int32 value)
{
	//ASSERT( index < m_valuesCount );
    if(index < m_valuesCount)
	{
	 int32 basevalue = (int32)m_uint32Values[ index ];
	 return ((basevalue*value)/100);
	}
	else 
	{
		Log.Error("[Object::GetModPUInt32Value]","index (%u) > m_valuesCount (%u) <- Report this to dev",index,m_valuesCount);
		return(0);
	}
}

void Object::ModUnsigned32Value(uint32 index, int32 mod)
{
	//ASSERT( index < m_valuesCount );
	if(index >= m_valuesCount)
	{
		Log.Error("[Object::ModUnsigned32Value]","index (%u) >= m_valuesCount (%u) <- Report this to dev.",index,m_valuesCount);
		return;
	}

	if(mod == 0) return;

	m_uint32Values[ index ] += mod;
	if( (int32)m_uint32Values[index] < 0 )
		m_uint32Values[index] = 0;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}

	if(m_objectTypeId == TYPEID_PLAYER)
	{
		// mana and energy regen
		if( index == UNIT_FIELD_POWER1 || index == UNIT_FIELD_POWER4 )
			static_cast<Player *>( this )->SendPowerUpdate();
			
#ifdef OPTIMIZED_PLAYER_SAVING
		switch(index)
		{
		case UNIT_FIELD_LEVEL:
		case PLAYER_XP:
			static_cast< Player* >( this )->save_LevelXP();
			break;

		case PLAYER_FIELD_COINAGE:
			static_cast< Player* >( this )->save_Gold();
			break;
		}
#endif
	}
}

void Object::ModSignedInt32Value(uint32 index, int32 value )
{
	//ASSERT( index < m_valuesCount );
    if(index >= m_valuesCount)
	{
     Log.Error("[Object::ModSignedInt32Value]","index (%u) >= m_valuesCount (%u) <- Report this to dev.",index,m_valuesCount);
	 return;
	}

	if(value == 0) return;

	m_uint32Values[ index ] += value;
	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}

	if(m_objectTypeId == TYPEID_PLAYER)
	{
#ifdef OPTIMIZED_PLAYER_SAVING
		switch(index)
		{
		case UNIT_FIELD_LEVEL:
		case PLAYER_XP:
			static_cast< Player* >( this )->save_LevelXP();
			break;

		case PLAYER_FIELD_COINAGE:
			static_cast< Player* >( this )->save_Gold();
			break;
		}
#endif
	}
}

void Object::ModFloatValue(const uint32 index, const float value )
{
	//ASSERT( index < m_valuesCount );
	if(index >= m_valuesCount)
	{
     Log.Error("[Object::ModFloatValue]","index (%u) >= m_valuesCount (%u) <- Report this to dev.",index,m_valuesCount);
	 return;
	}

	m_floatValues[ index ] += value;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}

//PERCENT, send 1.0f for 100%, not 100 
void Object::ModFloatValuePct(const uint32 index, const float value )
{
	//ASSERT( index < m_valuesCount );
	if(index >= m_valuesCount)
	{
     Log.Error("[Object::ModFloatValuePct]","index (%u) >= m_valuesCount (%u) <- Report this to dev.",index,m_valuesCount);
	 return;
	}

	m_floatValues[index] *= value;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}
//! Set uint64 property
void Object::SetUInt64Value( const uint32 index, const uint64 value )
{
	assert( index + 1 < m_valuesCount );
	if(m_uint32Values[index] == GUID_LOPART(value) && m_uint32Values[index+1] == GUID_HIPART(value))
		return;

	m_uint32Values[ index ] = *((uint32*)&value);
	m_uint32Values[ index + 1 ] = *(((uint32*)&value) + 1);

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );
		m_updateMask.SetBit( index + 1 );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}

//! Set float property
void Object::SetFloatValue( const uint32 index, const float value )
{
	//ASSERT( index < m_valuesCount );
	if(index >= m_valuesCount)
	{
     Log.Error("[Object::SetFloatValue]","index (%u) >= m_valuesCount (%u) <- Report this to dev.",index,m_valuesCount);
	 return;
	}


	if(m_floatValues[index] == value) return;

	m_floatValues[ index ] = value;
	
	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}


void Object::SetFlag( const uint32 index, uint32 newFlag )
{
	//ASSERT( index < m_valuesCount );
    if(index >= m_valuesCount)
	{
	 Log.Error("Object::SetFlag","Fatal: Debordement Flag Player (Index) %u >= %u (m_valuesCount)",index,m_valuesCount);
	 return;
	}
	//no change -> no update
	if((m_uint32Values[ index ] & newFlag)==newFlag)
		return;

	m_uint32Values[ index ] |= newFlag;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}


void Object::RemoveFlag( const uint32 index, uint32 oldFlag )
{
	//ASSERT( index < m_valuesCount );

	if(index >= m_valuesCount)
	{
	 Log.Error("Object::SetFlag","Fatal: Debordement Flag Player (Index) %u >= %u (m_valuesCount)",index,m_valuesCount);
	 return;
	}

	//no change -> no update
	if((m_uint32Values[ index ] & oldFlag)==0) 
		return;

	m_uint32Values[ index ] &= ~oldFlag;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}
//------------
void Object::SetDamageFlag( const uint32 index, uint32 newFlag, bool gUpdate )
{
	//ASSERT( index < m_valuesCount );
    if(index >= m_valuesCount)
	{
	 Log.Error("Object::SetFlag","Fatal: Debordement Flag Player (Index) %u >= %u (m_valuesCount)",index,m_valuesCount);
	 return;
	}
	//no change -> no update
	if((m_uint32Values[ index ] & newFlag) == newFlag) return;

	m_uint32Values[ index ] |= newFlag;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );
		if(gUpdate) m_mapMgr->ObjectUpdated(this);
	}
}


void Object::RemoveDamageFlag( const uint32 index, uint32 oldFlag, bool gUpdate )
{
	//ASSERT( index < m_valuesCount );

	if(index >= m_valuesCount)
	{
	 Log.Error("Object::SetFlag","Fatal: Debordement Flag Player (Index) %u >= %u (m_valuesCount)",index,m_valuesCount);
	 return;
	}

	//no change -> no update
	if((m_uint32Values[ index ] & oldFlag) == 0) return;

	m_uint32Values[ index ] &= ~oldFlag;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );
		if(gUpdate) m_mapMgr->ObjectUpdated(this);
	}
}
////////////////////////////////////////////////////////////

float Object::CalcDistance(Object *Ob)
{
	return CalcDistance(this->GetPositionX(), this->GetPositionY(), this->GetPositionZ(), Ob->GetPositionX(), Ob->GetPositionY(), Ob->GetPositionZ());
}
float Object::CalcDistance(float ObX, float ObY, float ObZ)
{
	return CalcDistance(this->GetPositionX(), this->GetPositionY(), this->GetPositionZ(), ObX, ObY, ObZ);
}
float Object::CalcDistance(Object *Oa, Object *Ob)
{
	return CalcDistance(Oa->GetPositionX(), Oa->GetPositionY(), Oa->GetPositionZ(), Ob->GetPositionX(), Ob->GetPositionY(), Ob->GetPositionZ());
}
float Object::CalcDistance(Object *Oa, float ObX, float ObY, float ObZ)
{
	return CalcDistance(Oa->GetPositionX(), Oa->GetPositionY(), Oa->GetPositionZ(), ObX, ObY, ObZ);
}

float Object::CalcDistance(float OaX, float OaY, float OaZ, float ObX, float ObY, float ObZ)
{
	float xdest = OaX - ObX;
	float ydest = OaY - ObY;
	float zdest = OaZ - ObZ;
	return sqrtf(zdest*zdest + ydest*ydest + xdest*xdest);
}

float Object::calcAngle( float Position1X, float Position1Y, float Position2X, float Position2Y )
{
	double angle=0.0f;

	// Calculate angle
	angle = calcRadAngle(Position1X,Position1Y,Position2X,Position2Y);
	// Convert to degrees
	angle = angle * float(180 / M_PI);

	// Return
	return float(angle);
}

float Object::calcRadAngle( float Position1X, float Position1Y, float Position2X, float Position2Y )
{
	double dx = double(Position2X-Position1X);
	double dy = double(Position2Y-Position1Y);
	double angle=0.0;

	// Calculate angle
	if (dx == 0.0) 
	{
		if (dy == 0.0) 	   angle = 0.0;
		else if (dy > 0.0) angle = M_PI * 0.5/*/ 2.0*/;
		else               angle = M_PI * 3.0 * 0.5/*/ 2.0*/;
	}
	else if (dy == 0.0)
	{
		if (dx > 0.0) angle = 0.0;
		else          angle = M_PI;
	}
	else
	{
		if (dx < 0.0) 		angle = atan(dy/dx) + M_PI;
		else if (dy < 0.0)  angle = atan(dy/dx) + (2*M_PI);
		else                angle = atan(dy/dx);
	}

	// Return
	return float(angle);
}

float Object::getEasyAngle( float angle )
{
	while ( angle < 0 ) 
	{
		angle = angle + 360;
	}
	while ( angle >= 360 ) 
	{
		angle = angle - 360;
	}
	return angle;
}

bool Object::inArc(float Position1X, float Position1Y, float FOV, float Orientation, float Position2X, float Position2Y )
{
	float angle = calcAngle( Position1X, Position1Y, Position2X, Position2Y );
	float lborder = getEasyAngle( ( Orientation - (FOV*0.5f/*/2*/) ) );
	float rborder = getEasyAngle( ( Orientation + (FOV*0.5f/*/2*/) ) );
	//sLog.outDebug("Orientation: %f Angle: %f LeftBorder: %f RightBorder %f",Orientation,angle,lborder,rborder);
	if(((angle >= lborder) && (angle <= rborder)) || ((lborder > rborder) && ((angle < rborder) || (angle > lborder))))
	{
		return true;
	}
	else
	{
		return false;
	}
} 

bool Object::isInFront(Object* target)
{
	// check if we facing something ( is the object within a 180 degree slice of our positive y axis )

    double x = target->GetPositionX() - m_position.x;
    double y = target->GetPositionY() - m_position.y;

    double angle = atan2( y, x );
    angle = ( angle >= 0.0 ) ? angle : 2.0 * M_PI + angle;
	angle -= m_position.o;

    while( angle > M_PI)
        angle -= 2.0 * M_PI;

    while(angle < -M_PI)
        angle += 2.0 * M_PI;

	// replace M_PI in the two lines below to reduce or increase angle

    double left = -1.0 * ( M_PI / 2.0 );
    double right = ( M_PI / 2.0 );

    return( ( angle >= left ) && ( angle <= right ) );
}

bool Object::isInBack(Object* target)
{
	// check if we are behind something ( is the object within a 180 degree slice of our negative y axis )

    double x = m_position.x - target->GetPositionX();
    double y = m_position.y - target->GetPositionY();

    double angle = atan2( y, x );
    angle = ( angle >= 0.0 ) ? angle : 2.0 * M_PI + angle;

	// if we are a unit and have a UNIT_FIELD_TARGET then we are always facing them
	if( m_objectTypeId == TYPEID_UNIT && m_uint32Values[UNIT_FIELD_TARGET] != 0 && static_cast< Unit* >( this )->GetAIInterface()->GetNextTarget() )
	{
		Unit* pTarget = static_cast< Unit* >( this )->GetAIInterface()->GetNextTarget();
		angle -= double( Object::calcRadAngle( target->m_position.x, target->m_position.y, pTarget->m_position.x, pTarget->m_position.y ) );
	}
	else
		angle -= target->GetOrientation();

    while( angle > M_PI)
        angle -= 2.0 * M_PI;

    while(angle < -M_PI)
        angle += 2.0 * M_PI;

	// replace M_H_PI in the two lines below to reduce or increase angle

    double left = -1.0 * ( M_H_PI / 2.0 );
    double right = ( M_H_PI / 2.0 );

    return( ( angle <= left ) && ( angle >= right ) );
}
bool Object::isInArc(Object* target , float angle) // angle in degrees
{
    return inArc( GetPositionX() , GetPositionY() , angle , GetOrientation() , target->GetPositionX() , target->GetPositionY() );
}

bool Object::isInRange(Object* target, float range)
{
	float dist = CalcDistance( target );
	return( dist <= range );
}

bool Object::IsPet()
{
	if( this->GetTypeId() != TYPEID_UNIT ) return false; 

	if( (static_cast< Unit* >( this )->m_isPet) && (m_uint32Values[UNIT_FIELD_CREATEDBY] != 0) && (m_uint32Values[UNIT_FIELD_SUMMONEDBY] != 0) )
		return true;

	return false;
}

Player* Object::GetPlayerFrom()
{
	if (IsPlayer())
		return static_cast<Player*>(this);

	if (IsPet() && static_cast<Pet*>(this)->GetOwner() != NULL && static_cast<Pet*>(this)->GetOwner()->IsPlayer())
		return static_cast<Player*>(static_cast<Pet*>(this)->GetOwner());

	if (IsCreature() && static_cast<Creature*>(this)->IsTotem() && static_cast<Creature*>(this)->GetSummonOwner() != NULL  && static_cast<Creature*>(this)->GetSummonOwner()->IsPlayer())
		return static_cast<Player*>(static_cast<Creature*>(this)->GetSummonOwner());

	if (IsUnit())
	{
		//maybe createdby? elemental totem, for example
		if (GetUInt32Value(UNIT_FIELD_CREATEDBY) != 0)
		{
			Unit* tmp=GetMapMgr()->GetUnit(GetUInt32Value(UNIT_FIELD_CREATEDBY));
			if (tmp != NULL)
				return tmp->GetPlayerFrom();
		}
		//maybe summonedby? guardians, for example
		if (GetUInt32Value(UNIT_FIELD_SUMMONEDBY) != 0)
		{
			Unit* tmp=GetMapMgr()->GetUnit(GetUInt32Value(UNIT_FIELD_SUMMONEDBY));
			if (tmp != NULL)
				return tmp->GetPlayerFrom();
		}
	}
	
	return NULL;
}

Player* Object::GetPlayerFromItem()
{
	if (IsPlayer())
		return static_cast<Player*>(this);

	if (IsItem() && static_cast<Item*>(this)->GetOwner() != NULL && static_cast<Item*>(this)->GetOwner()->IsPlayer())
		return static_cast<Player*>(static_cast<Item*>(this)->GetOwner());

	return NULL;
}

void Object::_setFaction()
{
	FactionTemplateDBC* factT = NULL;

	if(GetTypeId() == TYPEID_UNIT || GetTypeId() == TYPEID_PLAYER)
	{
		factT = dbcFactionTemplate.LookupEntry(GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	}
	else if(GetTypeId() == TYPEID_GAMEOBJECT)
	{
		factT = dbcFactionTemplate.LookupEntry(GetUInt32Value(GAMEOBJECT_FACTION));
	}

	if(!factT)
	{
   	    Log.Warning("Object::_setFaction()","Pas de gestion de Faction pour le type %d",GetTypeId());
		return;
	}
	m_faction = factT;
	m_factionDBC = dbcFaction.LookupEntry(factT->Faction);

	//remove threat on all npcs inrange that are now friendly, also cancel spells cast on them
	if (IsUnit() && !IsPlayer())
	{
		Unit* u=static_cast<Unit*>(this);
		for (ObjectSet::iterator i=GetInRangeSetBegin(); i!=GetInRangeSetEnd(); ++i)
			(*i)->m_objectsPendingUpdate.insert(this);
	}
}

void Object::UpdateOppFactionSet()
{
	m_oppFactsInRange.clear();
	for(Object::InRangeSet::iterator i = GetInRangeSetBegin(); i != GetInRangeSetEnd(); ++i)
	{
		if (((*i)->GetTypeId() == TYPEID_UNIT) || ((*i)->GetTypeId() == TYPEID_PLAYER) || ((*i)->GetTypeId() == TYPEID_GAMEOBJECT))
		{
			if (isHostile(this, (*i)))
			{
				if(!(*i)->IsInRangeOppFactSet(this))
					(*i)->m_oppFactsInRange.insert(this);
				if (!IsInRangeOppFactSet((*i)))
					m_oppFactsInRange.insert((*i));
				
			}
			else
			{
				if((*i)->IsInRangeOppFactSet(this))
					(*i)->m_oppFactsInRange.erase(this);
				if (IsInRangeOppFactSet((*i)))
					m_oppFactsInRange.erase((*i));
			}
		}
	}
}


void Object::EventSetUInt32Value(uint32 index, uint32 value)
{
	SetUInt32Value(index,value);
}

void Object::DealDamage(Unit *pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras)
{
	Player* plr = 0;

	if( !pVictim || !pVictim->isAlive() || !pVictim->IsInWorld() || !IsInWorld() )
		return;
	if( pVictim->GetTypeId() == TYPEID_PLAYER && static_cast< Player* >( pVictim )->GodModeCheat == true )
		return;
	if( pVictim->IsSpiritHealer() )
		return;

	//handle function procs, not if your hitting yourself :)
	if (this != pVictim)
	{
		std::vector<void*> data;
		data.push_back((void*)this);
		data.push_back((void*)pVictim);
		data.push_back((void*)&damage);

		if (IsUnit())
			static_cast<Unit*>(this)->HandleProcFnc(SPELLFNC_PROC_ON_DEAL_DAMAGE, &data);
		if (pVictim->IsUnit())
			static_cast<Unit*>(pVictim)->HandleProcFnc(SPELLFNC_PROC_ON_TAKE_DAMAGE, &data);
	}

	if( this->IsUnit() && pVictim->IsUnit() && pVictim != this )
	{
		// Set our attack target to the victim.
		static_cast< Unit* >( this )->CombatStatus.OnDamageDealt( pVictim ); // , damage );

	}
	
	if( pVictim->GetStandState() )//not standing-> standup
	{
		pVictim->SetStandState( STANDSTATE_STAND );//probably mobs also must standup
	}

	// This one is easy. If we're attacking a hostile target, and we're not flagged, flag us.
	// Also, you WONT get flagged if you are dueling that person - FiShBaIt
	Player* pFrom=GetPlayerFrom();
	Player* pVictimFrom=pVictim->GetPlayerFrom();

	if (isHostile(this, pVictim) && pFrom != NULL && pVictimFrom != NULL && pFrom->DuelingWith != pVictimFrom)
	{
		pFrom->SetPvPFlag();
		pFrom->SetPVPCombat();
	}

	// Andy: You get flagged if you deal damage to a PVP flagged unit
	if (pFrom != NULL && pVictim->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP))
	{
		pFrom->SetPvPFlag();
		pFrom->SetPVPCombat();
	}

	if(!no_remove_auras)
	{
		//zack 2007 04 24 : root should not remove self (and also other unknown spells)
		if(spellId)
		{
			pVictim->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN,spellId);
			if(Rand(35.0f))
				pVictim->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_UNUSED2,spellId);
		}
		else
		{
			pVictim->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN);
			if(Rand(35.0f))
				pVictim->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_UNUSED2);
		}
	}

	if(this->IsUnit())
	{
/*		if(!pVictim->isInCombat() && pVictim->IsPlayer())
			sHookInterface.OnEnterCombat( static_cast< Player* >( pVictim ), static_cast< Unit* >( this ) );

		if(IsPlayer() && ! static_cast< Player* >( this )->isInCombat())
			sHookInterface.OnEnterCombat( static_cast< Player* >( this ), static_cast< Player* >( this ) );*/

		//the black sheep , no actually it is paladin : Ardent Defender
		if(static_cast<Unit*>(this)->DamageTakenPctModOnHP35 && HasFlag(UNIT_FIELD_AURASTATE , AURASTATE_FLAG_HEALTH35) )
			damage = damage - float2int32(damage * static_cast<Unit*>(this)->DamageTakenPctModOnHP35) / 100 ;

		Player* plr = this->IsPlayer()? static_cast<Player*>(this) : NULL;

		if(pVictim->IsCreature() && plr != NULL && plr->GetTypeId() == TYPEID_PLAYER) // Units can't tag..
		{
			// Tagging
			Creature* victim = static_cast<Creature*>(pVictim);
			bool taggable;
			if(victim->GetCreatureName() && victim->GetCreatureName()->Type == CRITTER || victim->IsPet())
				taggable = false;
			else
				taggable = true;

			if(!victim->Tagged && taggable)
			{
				victim->Tagged = true;
				victim->TaggerGuid = plr->GetGUID();

				/* set loot method */
				if( plr->GetGroup() != NULL )
					victim->m_lootMethod = plr->GetGroup()->GetMethod();

				// For new players who get a create object
				uint32 Flags = pVictim->m_uint32Values[UNIT_DYNAMIC_FLAGS];
				Flags |= U_DYN_FLAG_TAPPED_BY_PLAYER;

				pVictim->m_uint32Values[UNIT_DYNAMIC_FLAGS] |= U_DYN_FLAG_TAGGED_BY_OTHER;

				// Update existing players.
				ByteBuffer buf(500);
				ByteBuffer buf1(500);

				pVictim->BuildFieldUpdatePacket(&buf1, UNIT_DYNAMIC_FLAGS, Flags);
				pVictim->BuildFieldUpdatePacket(&buf, UNIT_DYNAMIC_FLAGS, pVictim->m_uint32Values[UNIT_DYNAMIC_FLAGS]);

				// Loop inrange set, append to their update data.
				for(std::set<Player*>::iterator itr = m_inRangePlayers.begin(); itr != m_inRangePlayers.end(); ++itr)
				{
					if (static_cast< Player* >(plr)->InGroup())
					{
						if (static_cast< Player* >(*itr)->GetGroup() && static_cast< Player* >(plr)->GetGroup()->GetID() == static_cast< Player* >(*itr)->GetGroup()->GetID())
						{
							(*itr)->PushUpdateData(&buf1, 1);
						} 
						else
						{
							(*itr)->PushUpdateData(&buf, 1);
						}
						
					} 
					else
					{
						(*itr)->PushUpdateData(&buf, 1);
					}
				}

				// Update ourselves
				plr->PushUpdateData(&buf1, 1);

			}
		}		
	}

        ///Rage
        float val;

		if( pVictim->GetPowerType() == POWER_TYPE_RAGE 
			//&& !spellId //zack : general opinion is that spells should generate rage. I share the feeling
			&& pVictim != this
			&& pVictim->IsPlayer())
		{
			float level = (float)pVictim->getLevel();
			float c = 0.0091107836f * level * level + 3.225598133f * level + 4.2652911f;
			val = 2.5f * damage / c;
			uint32 rage = pVictim->GetUInt32Value( UNIT_FIELD_POWER2 );
			if( rage + float2int32( val ) > 1000 )
			  val = 1000.0f - (float)pVictim->GetUInt32Value( UNIT_FIELD_POWER2 );

			static_cast<Player*>(pVictim)->SetRage(rage + float2int32(val));
		}

	if( pVictim->IsPlayer() )
	{
		Player *pThis = static_cast< Player* >(pVictim);
		if(pThis->cannibalize)
		{
			sEventMgr.RemoveEvents(pVictim, EVENT_CANNIBALIZE);
			pThis->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			pThis->cannibalize = false;
		}
	}

	//* BATTLEGROUND DAMAGE COUNTER *//
	if( pVictim != this )
	{
		plr = GetPlayerFrom();

		if( plr != NULL && plr->m_bg != NULL && plr->GetMapMgr() == GetMapMgr() )
		{
			plr->m_bgScore.DamageDone += damage;
			plr->m_bg->UpdatePvPData();
		}
	}
   
	uint32 health = pVictim->GetUInt32Value(UNIT_FIELD_HEALTH );
	
	if(health <= damage && pVictim->IsPlayer() && pVictim->getClass() == ROGUE && pVictim->m_CustomTimers[CUSTOM_TIMER_CHEATDEATH] <= getMSTime() )
	{
		Player *plrVictim = static_cast<Player *>(pVictim);
		uint32 rank = plrVictim->m_cheatDeathRank;

		uint32 chance = (rank == 3) ? 100 : rank * 33;
		if(Rand(chance))
		{
			// Proc that cheating death!
			SpellEntry *spellInfo = dbcSpell.LookupEntry(45182);
			Spell *spell = new Spell(pVictim, spellInfo, true, NULL);
			SpellCastTargets targets;
			targets.m_target = (Object *)pVictim; // targets.m_unitTarget = pVictim->GetGUID();
			spell->prepare(&targets);
			static_cast<Player *>(pVictim)->m_CustomTimers[CUSTOM_TIMER_CHEATDEATH] = getMSTime() + 60000;

			// Why return? So this damage isn't counted. ;)
			// On official, it seems Blizzard applies it's Cheating Death school absorb aura for 1 msec, but it's too late
			// for us by now.
			return;
		}
	}
	/*------------------------------------ DUEL HANDLERS --------------------------*/
	if (IsPlayer() && (static_cast< Player* >(this)->DuelingWith == pVictim->GetPlayerFrom()) )
	{
		if((health <= damage) && (static_cast< Player* >(this)->DuelingWith != NULL) )
		{
			//Player in Duel and Player Victim has lost
			uint32 NewHP = pVictim->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/100;

			if(NewHP < 5) 
				NewHP = 5;

			//Set there health to 1% or 5 if 1% is lower then 5
			pVictim->SetUInt32Value(UNIT_FIELD_HEALTH, NewHP);
			//End Duel
			static_cast< Player* >(this)->EndDuel(DUEL_WINNER_KNOCKOUT);
			static_cast<Player *>(this)->GetAchievementInterface()->HandleAchievementCriteriaWinDuel();
			static_cast<Player *>(pVictim)->GetAchievementInterface()->HandleAchievementCriteriaLoseDuel();

			// surrender emote
			pVictim->Emote(EMOTE_ONESHOT_BEG);			// Animation

			return;		  
		}
	}
	/*------------------------------------ DUEL HANDLERS END--------------------------*/

	bool isCritter = false;
	if(pVictim->GetTypeId() == TYPEID_UNIT && ((Creature*)pVictim)->GetCreatureName())
	{
			if(((Creature*)pVictim)->GetCreatureName()->Type == CRITTER)
				isCritter = true;
	}
	/* -------------------------- HIT THAT CAUSES VICTIM TO DIE ---------------------------*/
	if ((isCritter || health <= damage) && (!pVictim->IsCreature() || pVictim->GetAIInterface()->m_canDie))
	{
		HandleDeath(pVictim, damage, targetEvent, unitEvent, spellId, no_remove_auras, plr, isCritter);

		std::set<Unit*>::iterator itr = pVictim->m_sharedHPUnits.begin();
		std::set<Unit*>::iterator itrend = pVictim->m_sharedHPUnits.end();

		//were not the master, try and get the master
		if (pVictim->m_sharedHPUnits.size() == 0 && pVictim->m_sharedHPMaster != NULL && pVictim->m_sharedHPMaster->event_GetInstanceID() == pVictim->event_GetInstanceID())
		{
				itr = pVictim->m_sharedHPMaster->m_sharedHPUnits.begin();
				itrend = pVictim->m_sharedHPMaster->m_sharedHPUnits.end();
		}

		//do this with the other guys in the hp share :P
		for (; itr != itrend; ++itr)
		{
			if ((*itr) == NULL || !(*itr)->isAlive() || (*itr) == pVictim)
				continue;

			HandleDeath((*itr), damage, targetEvent, unitEvent, spellId, no_remove_auras, plr, isCritter);
		}

	}
	else /* ---------- NOT DEAD YET --------- */
	{
		if(pVictim != this /* && updateskill */)
		{
			// Send AI Victim Reaction
			if( this->IsPlayer() || this->GetTypeId() == TYPEID_UNIT )
			{
				if( pVictim->GetTypeId() != TYPEID_PLAYER )
				{
					static_cast< Creature* >( pVictim )->GetAIInterface()->AttackReaction( static_cast< Unit* >( this ), damage, spellId );
				}
			}
		}
		
		// TODO: Mark victim as a HK
		/*if( static_cast< Player* >( pVictim )->GetCurrentBattleground() != NULL && static_cast< Player* >( this )->GetCurrentBattleground() != NULL)
		{
			
		}*/	
		if (health > damage) pVictim->SetUInt32Value( UNIT_FIELD_HEALTH, health - damage );
		else                 pVictim->SetUInt32Value( UNIT_FIELD_HEALTH, 1 );

		std::set<Unit*>::iterator itr = pVictim->m_sharedHPUnits.begin();
		std::set<Unit*>::iterator itrend = pVictim->m_sharedHPUnits.end();

		//were not the master, try and get the master
		if (pVictim->m_sharedHPUnits.size() == 0 && pVictim->m_sharedHPMaster != NULL && pVictim->m_sharedHPMaster->event_GetInstanceID() == pVictim->event_GetInstanceID())
		{
				itr = pVictim->m_sharedHPMaster->m_sharedHPUnits.begin();
				itrend = pVictim->m_sharedHPMaster->m_sharedHPUnits.end();
		}

		//do this with the other guys in the hp share :P
		for (; itr != itrend; ++itr)
		{
			if ((*itr) == NULL || !(*itr)->isAlive() || (*itr) == pVictim)
				continue;

			if (health > damage) (*itr)->SetUInt32Value( UNIT_FIELD_HEALTH, health - damage );
			else                 (*itr)->SetUInt32Value( UNIT_FIELD_HEALTH, 1 );

		}

		/*
		WorldPacket data(SMSG_HEALTH_UPDATE, 12);
		data << pVictim->GetGUID() << GetUInt32Value(UNIT_FIELD_HEALTH);

		pVictim->SendMessageToSet(&data, true);

		if (pVictim->IsPlayer() && static_cast<Player*>(pVictim)->GetGroup() != NULL)
			static_cast<Player*>(pVictim)->GetGroup()->SendPacketToAllButOne(&data, pVictim); //dont send to self, we did that in sendmessagetoset
	

		if (IsCreature() && !IsPet())
			static_cast<Unit*>(this)->GetAIInterface()->HandleEvent(EVENT_DAMAGEDEALT, pVictim, damage);
		*/
	}
}

void Object::SpellNonMeleeDamageLog(Unit *pVictim, uint32 spellID, uint32 damage, bool allowProc, bool static_damage, bool no_remove_auras, int32 forced_school)
{
 //Log.Warning("SpellNonMeleeDamageLog","Call");
//==========================================================================================
//==============================Unacceptable Cases Processing===============================
//==========================================================================================
	if(!pVictim || !pVictim->isAlive())
		return;

	SpellEntry *spellInfo = dbcSpell.LookupEntry( spellID );
	if(!spellInfo)
        return;

	if (this->IsPlayer() && !static_cast< Player* >( this )->canCast(spellInfo))
		return;

	//Log.Warning("SpellNonMeleeDamageLog","Begin (%u)",this->GetEntry());
//==========================================================================================
//==============================Variables Initialization====================================
//========================================================================================== 
	uint32 school = forced_school != -1? forced_school : spellInfo->School;
	float res = float(damage);
	uint32 aproc = PROC_ON_ANY_HOSTILE_ACTION;
	uint32 vproc = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_ANY_DAMAGE_VICTIM;
	bool critical = false;
	//float dmg_reduction_pct;

	float res_after_spelldmg;
	
	Unit *caster = NULL;
	if( IsUnit() )
		caster = static_cast<Unit *>(this);
		
//==========================================================================================
//==============================+Spell Damage Bonus Calculations============================
//==========================================================================================
//------------------------------by stats----------------------------------------------------
	if( this->IsUnit() && !static_damage )
	{
		Unit* caster = static_cast< Unit* >( this );
		caster->RemoveAurasByInterruptFlag( AURA_INTERRUPT_ON_START_ATTACK );
		res += caster->GetSpellDmgBonus( pVictim, spellInfo, ( int )res, false );

		//Aura 271 - Mods Damage for particular casters spells
		Unit::DamageTakenPctModPerCasterType::iterator it = 
			pVictim->DamageTakenPctModPerCaster.find(GetGUID());
		while(it != pVictim->DamageTakenPctModPerCaster.end() && 
			it->first == GetGUID())
		{
			if(spellInfo->SpellGroupType[0] & it->second.first ||
				spellInfo->SpellGroupType[1] & it->second.first ||
				spellInfo->SpellGroupType[2] & it->second.first )
			{
				res *= float(100 + it->second.second) / 100;
			}
			it++;
		}

		res_after_spelldmg = res;
//==========================================================================================
//==============================Post +SpellDamage Bonus Modifications=======================
//==========================================================================================
		if( res < 0 )
			res = 0;
		else if( spellInfo->spell_can_crit == true )
		{
//------------------------------critical strike chance--------------------------------------	
			// lol ranged spells were using spell crit chance
			float CritChance;
			if( spellInfo->is_ranged_spell )
			{

				if( IsPlayer() )
				{
					CritChance = GetFloatValue( PLAYER_RANGED_CRIT_PERCENTAGE );
					if( pVictim->IsPlayer() )
						CritChance += static_cast< Player* >(pVictim)->res_R_crit_get();

					CritChance += (float)(pVictim->AttackerCritChanceMod[spellInfo->School]);
				}
				else
				{
					CritChance = 5.0f; // static value for mobs.. not blizzlike, but an unfinished formula is not fatal :)
				}
				if( pVictim->IsPlayer() )
				CritChance -= static_cast< Player* >(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_RANGED_CRIT_RESILIENCE );
			}
			else if( spellInfo->is_melee_spell )
			{
				// Same shit with the melee spells, such as Judgement/Seal of Command
				if( IsPlayer() )
				{
					CritChance = GetFloatValue(PLAYER_CRIT_PERCENTAGE);
				}
				if( pVictim->IsPlayer() )
				{
					CritChance += static_cast< Player* >(pVictim)->res_R_crit_get(); //this could be ability but in that case we overwrite the value
				}
				// Resilience
				CritChance -= pVictim->IsPlayer() ? static_cast< Player* >(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE ) : 0.0f;
				// Victim's (!) crit chance mod for physical attacks?
				CritChance += (float)(pVictim->AttackerCritChanceMod[0]);

			}
			else
			{
				CritChance = caster->spellcritperc + caster->SpellCritChanceSchool[school] + pVictim->AttackerCritChanceMod[school];
				if(caster->IsPlayer() && pVictim->HasFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_FROZEN))	
					CritChance += static_cast< Player* >( caster )->m_RootedCritChanceBonus;

				SM_FFValue(caster->SM_CriticalChance, &CritChance, spellInfo);

				if( pVictim->IsPlayer() )
				CritChance -= static_cast< Player* >(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE );
			}
			if( CritChance < 0 ) CritChance = 0;
			if( CritChance > 95 ) CritChance = 95;
			critical = Rand(CritChance);
			//sLog.outString( "SpellNonMeleeDamageLog: Crit Chance %f%%, WasCrit = %s" , CritChance , critical ? "Yes" : "No" );
//==========================================================================================
//==============================Spell Critical Hit==========================================
//==========================================================================================
			if (critical)
			{		
				int32 critical_bonus = 100;
				SM_FIValue(caster->SM_PCriticalDamage, &critical_bonus, spellInfo);

				if( critical_bonus > 0 )
				{
					// the bonuses are halved by 50% (funky blizzard math :S)
					float b;
					if( spellInfo->School == 0 || spellInfo->is_melee_spell || spellInfo->is_ranged_spell )		// physical || hackfix SoCommand/JoCommand
						b = ( ( float(critical_bonus) ) / 100.0f ) + 1.0f;
					else
						b = ( ( float(critical_bonus) / 2.0f ) / 100.0f ) + 1.0f;
			
					res *= b;
				}

				if( pVictim->IsPlayer() )
				{
					//res = res*(1.0f-2.0f*static_cast< Player* >(pVictim)->CalcRating(PLAYER_RATING_MODIFIER_MELEE_CRIT_RESISTANCE));
					//Resilience is a special new rating which was created to reduce the effects of critical hits against your character.
					//It has two components; it reduces the chance you will be critically hit by x%, 
					//and it reduces the damage dealt to you by critical hits by 2x%. x is the percentage resilience granted by a given resilience rating. 
					//It is believed that resilience also functions against spell crits, 
					//though it's worth noting that NPC mobs cannot get critical hits with spells.
					float dmg_reduction_pct = 2 * static_cast< Player* >(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE ) / 100.0f;
					if( dmg_reduction_pct > 1.0f )
						dmg_reduction_pct = 1.0f; //we cannot resist more then he is criticalling us, there is no point of the critical then :P
					res = res - res * dmg_reduction_pct;
				}

				pVictim->Emote( EMOTE_ONESHOT_WOUNDCRITICAL );
				aproc |= PROC_ON_CRIT_ATTACK;
				vproc |= PROC_ON_CRIT_ATTACK_VICTIM;
			}
		}
	}
//==========================================================================================
//==============================Post Roll Calculations======================================
//==========================================================================================
// Special cases - (Randdrick, rev398)

	if( spellInfo->NameHash == SPELL_HASH_ICE_LANCE &&
	    (( pVictim->HasFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_FROZEN)) ||
		(caster && caster->m_frozenTargetCharges > 0))) 
	{
		res *= 3; //Ice Lance deals 3x damage if target is frozen
	}

	if (caster)
	{
		if (caster->m_frozenTargetCharges > 0 && spellInfo->School == SCHOOL_FROST)
		{
			caster->m_frozenTargetCharges--;
			if (caster->m_frozenTargetCharges <= 0)
				caster->RemoveAura(caster->m_frozenTargetId);
		}

		// [Mage] Hot Streak
		if (!(aproc & PROC_ON_SPELL_CRIT_HIT))
			caster->m_hotStreakCount = 0;

		if( aproc & PROC_ON_SPELL_CRIT_HIT && caster->HasDummyAura(SPELL_HASH_ECLIPSE))
		{
			if( caster->m_CustomTimers[CUSTOM_TIMER_ECLIPSE] <= getMSTime() )
			{
				caster->m_CustomTimers[CUSTOM_TIMER_ECLIPSE] = getMSTime() + MSTIME_SECOND*30;
				if( spellInfo->NameHash == SPELL_HASH_STARFIRE )
				{
					caster->CastSpell( caster, 48517, true );
				}else if( spellInfo->NameHash == SPELL_HASH_WRATH )
				{
					caster->CastSpell( caster, 48518, true );
				}
			}
		}
	}
//------------------------------absorption--------------------------------------------------	
	uint32 ress=(uint32)res;
	uint32 abs_dmg = pVictim->AbsorbDamage(this, school, &ress, dbcSpell.LookupEntryForced(spellID));
	uint32 ms_abs_dmg= pVictim->ManaShieldAbsorb(ress, dbcSpell.LookupEntryForced(spellID));
	if (ms_abs_dmg)
	{
		if(ms_abs_dmg > ress)
			ress = 0;
		else
			ress-=ms_abs_dmg;

		abs_dmg += ms_abs_dmg;
	}

	if(ress < 0) ress = 0;

	res=(float)ress;
	dealdamage dmg;
	dmg.school_type = school;
	dmg.full_damage = ress;
	dmg.resisted_damage = 0;
	
	if(res <= 0) 
		dmg.resisted_damage = dmg.full_damage;

	//------------------------------resistance reducing-----------------------------------------	
	if(res > 0 && this->IsUnit())
	{
		static_cast<Unit*>(this)->CalculateOldResistanceReduction(pVictim,&dmg,spellInfo);
		if((int32)dmg.resisted_damage > dmg.full_damage)
			res = 0;
		else
			res = float(dmg.full_damage - dmg.resisted_damage);
	}
	//------------------------------special states----------------------------------------------
	if(pVictim->GetTypeId() == TYPEID_PLAYER && static_cast< Player* >(pVictim)->GodModeCheat == true)
	{
		res = float(dmg.full_damage);
		dmg.resisted_damage = dmg.full_damage;
	}
	//DK:FIXME->SplitDamage
	// Completed (Supa)
	// Paladin: Blessing of Sacrifice, and Warlock: Soul Link
		if( !pVictim->m_damageSplitTargets.empty() )
		{
			std::list< DamageSplitTarget >::iterator itr;
			Unit * splittarget;
			uint32 splitdamage, tmpsplit;
			for( itr = pVictim->m_damageSplitTargets.begin() ; itr != pVictim->m_damageSplitTargets.end() ; itr ++ )
			{
				// TODO: Separate damage based on school.
				splittarget = pVictim->GetMapMgr() ? pVictim->GetMapMgr()->GetUnit( itr->m_target ) : NULL;
				if( splittarget && res > 0 )
				{
					// calculate damage
					tmpsplit = itr->m_flatDamageSplit;
					if( tmpsplit > (uint32)float2int32( res ))
						tmpsplit = float2int32( res ); // prevent < 0 damage
					splitdamage = tmpsplit;
					res -= (float)tmpsplit;
					tmpsplit = float2int32(itr->m_pctDamageSplit * res);
					if( tmpsplit > (uint32)float2int32( res ) )
						tmpsplit = float2int32( res );
					splitdamage += tmpsplit;
					res -= (float)tmpsplit;
					// TODO: pct damage

					if( splitdamage )
					{
						pVictim->DealDamage( splittarget , splitdamage , 0 , 0 , 0 , false );
						// Send damage log
						pVictim->SendSpellNonMeleeDamageLog( pVictim , splittarget , 27148 , splitdamage , SCHOOL_HOLY , 0 , 0 , true , 0 , 0 , true );
					}
				}
			}
		}
//==========================================================================================
//==============================Data Sending ProcHandling===================================
//==========================================================================================
	SendSpellNonMeleeDamageLog(this, pVictim, spellID, float2int32(res), school, abs_dmg, dmg.resisted_damage, false, 0, critical, IsPlayer());
	DealDamage( pVictim, float2int32( res ), 2, 0, spellID );
	
	if( this->IsUnit() && allowProc && spellInfo->Id != 25501 )
	{
		pVictim->HandleProc( vproc, static_cast< Unit* >( this ), spellInfo, float2int32( res ) );
		pVictim->m_procCounter = 0;
		static_cast< Unit* >( this )->HandleProc( aproc, pVictim, spellInfo, float2int32( res ) );
		static_cast< Unit* >( this )->m_procCounter = 0;
	}
	if( this->IsPlayer() )
	{
			static_cast< Player* >( this )->m_casted_amount[school] = ( uint32 )res;
	}

	

	if( pVictim->GetCurrentSpell() )
		pVictim->GetCurrentSpell()->AddTime( school );

//==========================================================================================
//==============================Post Damage Processing======================================
//==========================================================================================
	if( (int32)dmg.resisted_damage == dmg.full_damage && !abs_dmg )
	{
		//Magic Absorption
		if( pVictim->IsPlayer() )
		{
			if( static_cast< Player* >( pVictim )->m_RegenManaOnSpellResist )
			{
				Player* pl = static_cast< Player* >( pVictim );
				uint32 maxmana = pl->GetUInt32Value( UNIT_FIELD_MAXPOWER1 );

				//TODO: wtf is this ugly mess of casting bullshit
				uint32 amount = uint32(float( float(maxmana)*pl->m_RegenManaOnSpellResist));

				pVictim->Energize( pVictim, 29442, amount, POWER_TYPE_MANA );
			}
			// we still stay in combat dude
			static_cast< Player* >(pVictim)->CombatStatusHandler_ResetPvPTimeout();
		}
		if( IsPlayer() )
			static_cast< Player* >(this)->CombatStatusHandler_ResetPvPTimeout();
	}
	
	if( caster && (int32)dmg.resisted_damage == dmg.full_damage && !abs_dmg )
		caster->HandleProc(PROC_MISC, pVictim, spellInfo);
		
	if( school == SHADOW_DAMAGE )
	{
		if( IsPlayer() && ((Unit*)this)->isAlive() && ((Player*)this)->getClass() == PRIEST )
			((Player*)this)->VampiricSpell(float2int32(res), pVictim);

		if( pVictim->isAlive() && this->IsUnit() )
		{
			//Shadow Word:Death
			if( spellID == 32379 || spellID == 32996 || spellID == 48157 || spellID == 48158 ) 
			{
				uint32 damage = (uint32)( res + abs_dmg );
				uint32 absorbed = static_cast<Unit *>(this)->AbsorbDamage(this, school, &damage, dbcSpell.LookupEntryForced(spellID) );
				DealDamage( static_cast< Unit* >( this ), damage, 2, 0, spellID );
				SendSpellNonMeleeDamageLog( this, this, spellID, damage, school, absorbed, 0, false, 0, false, this->IsPlayer() );
			}
			else if( spellInfo->NameHash == SPELL_HASH_HAUNT )
				caster->m_lastHauntInitialDamage = res;
		}
	}

	//Log.Warning("SpellNonMeleeDamageLog","End");
}

//*****************************************************************************************
//* SpellLog packets just to keep the code cleaner and better to read
//*****************************************************************************************

void Object::SendSpellLog(Object *Caster, Object *Target,uint32 Ability, uint8 SpellLogType)
{
	if ((!Caster || !Target) && Ability)
		return;

	WorldPacket data(SMSG_SPELLLOGMISS,28);
	data << Ability;										// spellid
	data << Caster->GetGUID();							  // caster / player
	data << (uint8)1;									   // unknown but I think they are const
	data << (uint32)1;									  // unknown but I think they are const
	data << Target->GetGUID();							  // target
	data << SpellLogType;								   // spelllogtype
	Caster->SendMessageToSet(&data, true);
}


void Object::SendSpellNonMeleeDamageLog( Object* Caster, Object* Target, uint32 SpellID, uint32 Damage, uint8 School, uint32 AbsorbedDamage, uint32 ResistedDamage, bool PhysicalDamage, uint32 BlockedDamage, bool CriticalHit, bool bToset )
{
	if ((!Caster || !Target) && SpellID)
		return;

	WorldPacket data(SMSG_SPELLNONMELEEDAMAGELOG,40);
	data << Target->GetNewGUID();
	data << Caster->GetNewGUID();
	data << SpellID;                    // SpellID / AbilityID
	data << Damage;                     // All Damage
	data << uint32(0); //overkill
	data << uint8(g_spellSchoolConversionTable[School]);                     // School
	data << AbsorbedDamage;             // Absorbed Damage
	data << ResistedDamage;             // Resisted Damage
	data << uint8(PhysicalDamage);      // Physical Damage (true/false)
	data << uint8(0);                   // unknown or it binds with Physical Damage
	data << BlockedDamage;		     // Physical Damage (true/false)
	data << uint8(CriticalHit ? 7 : 5);                   // unknown const
	data << uint32(0);

	Caster->SendMessageToSet( &data, bToset );
}

int32 Object::event_GetInstanceID()
{
	// return -1 for non-inworld.. so we get our shit moved to the right thread
	if(!IsInWorld())
		return -1;
	else
		return m_instanceId;
}

void Object::EventSpellDamage(uint64 Victim, uint32 SpellID, uint32 Damage)
{
	if(!IsInWorld())
		return;

	Unit * pUnit = GetMapMgr()->GetUnit(Victim);
	if(pUnit == 0) return;

	SpellNonMeleeDamageLog(pUnit, SpellID, Damage, true);
}

// CanActivate se declenche lorsque qu'un player entre dans une cell
bool Object::CanActivate()
{
	switch(m_objectTypeId)
	{
	case TYPEID_UNIT:
		{
			if(!IsPet())
				return true;
		}break;

	case TYPEID_GAMEOBJECT:
		{
			if(static_cast<GameObject*>(this)->HasAI() && (GetByte(GAMEOBJECT_BYTES_1, 1) != GAMEOBJECT_TYPE_TRAP))
				return true;
		}break;
	}

	return false;
}

void Object::Activate(MapMgr * mgr)
{
	switch(m_objectTypeId)
	{
	case TYPEID_UNIT:
		if(IsVehicle())
			mgr->activeVehicles.insert((Vehicle*)this);
		else
			mgr->activeCreatures.insert((Creature*)this);
		break;

	case TYPEID_GAMEOBJECT:
		//Log.Notice("[Object::Activate]","Objet Actif: %s",this->GetObjectName());
		mgr->activeGameObjects.insert((GameObject*)this);
		break;
	}

	Active = true;
}

void Object::Deactivate(MapMgr * mgr)
{
	switch(m_objectTypeId)
	{
	case TYPEID_UNIT:
	if(IsVehicle())
		{
			// check iterator
			if( mgr->__vehicle_iterator != mgr->activeVehicles.end() && (*mgr->__vehicle_iterator) == (Vehicle*)this )
				++mgr->__vehicle_iterator;

			mgr->activeVehicles.erase((Vehicle*)this);
		}
		else
		{
			// check iterator
			if( mgr->__creature_iterator != mgr->activeCreatures.end() && (*mgr->__creature_iterator) == (Creature*)this )
				++mgr->__creature_iterator;
				
			mgr->activeCreatures.erase((Creature*)this);
		}
		break;

	case TYPEID_GAMEOBJECT:
		//Log.Notice("[Object::Activate]","Objet Inactif: %s",this->GetObjectName());
		// check iterator
		if( mgr->__gameobject_iterator != mgr->activeGameObjects.end() && (*mgr->__gameobject_iterator) == (GameObject*)this )
			++mgr->__gameobject_iterator;
			
		mgr->activeGameObjects.erase((GameObject*)this);
		break;
	}
	Active = false;
}

void Object::SetByte(uint32 index, uint32 index1,uint8 value)
{
	//ASSERT( index < m_valuesCount );
	if(index >= m_valuesCount)
	{
     Log.Error("[Object::SetByte]","index (%u) >= m_valuesCount (%u) <- Report this to dev.",index,m_valuesCount);
	 return;
	}
	// save updating when val isn't changing.
	uint8 * v =&((uint8*)m_uint32Values)[index*4+index1];
	
	if(*v == value)
		return;

	*v = value;

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}

}

void Object::SetZoneId(uint32 newZone)
{
	m_zoneId = newZone;
	if( m_objectTypeId == TYPEID_PLAYER && static_cast< Player* >( this )->GetGroup() )
		static_cast< Player* >( this )->GetGroup()->HandlePartialChange( PARTY_UPDATE_FLAG_ZONEID, static_cast< Player* >( this ) );
}

void Object::PlaySoundToSet(uint32 sound_entry)
{
	WorldPacket data(SMSG_PLAY_SOUND, 4);
	data << sound_entry;
	SendMessageToSet(&data, true);
}

void Object::_SetExtension(const string& name, void* ptr)
{
	if( m_extensions == NULL )
		m_extensions = new ExtensionSet;

	m_extensions->insert( make_pair( name, ptr ) );
}

void Object::DeleteOperator()
{
	delete this;
}

void Object::HandleDeath(Unit *pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras, Player* plr, bool isCritter)
{
	if (pVictim->IsCreature() && static_cast<Creature*>(pVictim)->IsTotem())
	{
		static_cast<Creature*>(pVictim)->SafeDelete();
		return;
	}

		if( IsUnit() )
		{
			SpellEntry *killerspell;
			if( spellId )
				killerspell = dbcSpell.LookupEntry( spellId );
			else killerspell = NULL;
			pVictim->HandleProc( PROC_ON_DIE, static_cast< Unit* >( this ), killerspell );
			pVictim->m_procCounter = 0;
		}
		// check if pets owner is combat participant
		bool owner_participe = false;
		if( IsPet() && static_cast<Pet*>(this)->GetPetOwner() != NULL && static_cast<Pet*>(this)->GetPetOwner()->IsPlayer())
		{
			Player* owner = static_cast<Player*>(static_cast<Pet*>( this )->GetPetOwner());
			if( owner != NULL && pVictim->GetAIInterface()->getThreatByPtr( owner ) > 0 )
				owner_participe = true;
		}
		/* victim died! */
		if( pVictim->IsPlayer() )
			static_cast< Player* >( pVictim )->KillPlayer();
		else
		{
			pVictim->setDeathState( JUST_DIED );
			pVictim->GetAIInterface()->HandleEvent( EVENT_LEAVECOMBAT, static_cast< Unit* >( this ), 0);
		}

		if( pVictim->IsPlayer() && (!IsPlayer() || pVictim == this ) )
		{
			static_cast< Player* >( pVictim )->DeathDurabilityLoss(0.10);
		}

		/* Zone Under Attack */
        MapInfo * pMapInfo = WorldMapInfoStorage.LookupEntry(GetMapId());
        if( pMapInfo && pMapInfo->type == INSTANCE_NULL && !pVictim->IsPlayer() && !pVictim->IsPet() && ( IsPlayer() || IsPet() ) )
		{
			// Only NPCs that bear the PvP flag can be truly representing their faction.
			if( ((Creature*)pVictim)->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_PVP ) )
			{
				Player * pAttacker = NULL;
				if( IsPet() && static_cast< Pet* >(this)->GetOwner() != NULL && static_cast< Pet* >(this)->GetOwner()->IsPlayer())
					pAttacker = static_cast<Player*>(static_cast< Pet* >( this )->GetPetOwner());
				else if(IsPlayer())
					pAttacker = static_cast< Player* >( this );

				if( pAttacker != NULL)
                {
				    uint8 teamId = (uint8)pAttacker->GetTeam();
				    if(teamId == 0) // Swap it.
					    teamId = 1;
				    else
					    teamId = 0;
				    uint32 AreaID = pVictim->GetMapMgr()->GetAreaID(pVictim->GetPositionX(), pVictim->GetPositionY(), pVictim->GetPositionZ());
				    if(!AreaID)
					    AreaID = pAttacker->GetZoneId(); // Failsafe for a shitty TerrainMgr

				    if(AreaID)
				    {
					    WorldPacket data(SMSG_ZONE_UNDER_ATTACK, 4);
					    data << AreaID;
					    sWorld.SendFactionMessage(&data, teamId);
				    }
                }
			}
		}
		
		if(pVictim->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT) > 0)
		{
			if(pVictim->GetCurrentSpell())
			{
				Spell *spl = pVictim->GetCurrentSpell();
				for(int i = 0; i < 3; i++)
				{
					if(spl->m_spellInfo->Effect[i] == SPELL_EFFECT_PERSISTENT_AREA_AURA)
					{
						DynamicObject *dObj = GetMapMgr()->GetDynamicObject(pVictim->GetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT));
						if(!dObj)
							return;
						WorldPacket data(SMSG_GAMEOBJECT_DESPAWN_ANIM, 8);
						data << dObj->GetGUID();
						dObj->SendMessageToSet(&data, false);
						dObj->RemoveFromWorld(true);
						delete dObj;
					}
				}

				//                        Apprivoise un familier
				if(spl->m_spellInfo->channelInterruptFlags == 0xBC0C) spl->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
			}
		}
		
		/* Remove all Auras */
		pVictim->DropAurasOnDeath();

		/* Stop players from casting */
		std::set<Player*>::iterator itr;
		for( itr = pVictim->GetInRangePlayerSetBegin() ; itr != pVictim->GetInRangePlayerSetEnd() ; itr ++ )
		{
			//if player has selection on us
			if( (*itr)->GetSelection()==pVictim->GetGUID())							
			{
				if( (*itr)->isCasting() )
					(*itr)->CancelSpell( NULL ); //cancel current casting spell
			}
		}
		/* Stop victim from attacking */
		if( this->IsUnit() )
			pVictim->smsg_AttackStop( static_cast< Unit* >( this ) );

		if( pVictim->GetTypeId() == TYPEID_PLAYER )
			static_cast< Player* >( pVictim )->EventAttackStop();

		/* Set victim health to 0 */
		pVictim->SetUInt32Value(UNIT_FIELD_HEALTH, 0);
		if(pVictim->IsPlayer())
		{
			uint32 self_res_spell = static_cast< Player* >( pVictim )->SoulStone;
			static_cast< Player* >( pVictim )->SoulStone = static_cast< Player* >( pVictim )->SoulStoneReceiver = 0;

			if( !self_res_spell && static_cast< Player* >( pVictim )->bReincarnation )
			{
				SpellEntry* m_reincarnSpellInfo = dbcSpell.LookupEntry( 20608 );
				if( static_cast< Player* >( pVictim )->Cooldown_CanCast( m_reincarnSpellInfo ) )
				{
					uint32 ankh_count = static_cast< Player* >( pVictim )->GetItemInterface()->GetItemCount( 17030 );
					if( ankh_count )
						self_res_spell = 21169;
				}
			}
			pVictim->SetUInt32Value( PLAYER_SELF_RES_SPELL, self_res_spell );
			pVictim->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID , 0 );
			//pVictim->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
		}

		/* -------------------------------- HONOR + BATTLEGROUND CHECKS ------------------------ */
		plr = NULL;
		if( IsPlayer() )
			plr = static_cast< Player* >( this );
		else if(IsPet() && static_cast< Pet* >(this)->GetOwner() != NULL && static_cast< Pet* >(this)->GetOwner()->IsPlayer())
			plr = static_cast<Player*>(static_cast< Pet* >( this )->GetPetOwner());

		if( plr != NULL)
		{
			if( plr->m_bg != 0 )
				plr->m_bg->HookOnPlayerKill( plr, pVictim );

			if( pVictim->IsPlayer() )
			{
				sHookInterface.OnKillPlayer( plr, static_cast< Player* >( pVictim ) );
				if(plr->getLevel() > pVictim->getLevel())
				{
					unsigned int diff = plr->getLevel() - pVictim->getLevel();
					if( diff <= 8 )
					{
						HonorHandler::OnPlayerKilledUnit(plr, pVictim);
						SetFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_LASTKILLWITHHONOR );
					}
					else
						RemoveFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_LASTKILLWITHHONOR );
				}
				else
				{
					HonorHandler::OnPlayerKilledUnit( plr, pVictim );
					SetFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_LASTKILLWITHHONOR );
				}
			}
			else
			{
				if (!isCritter) // REPUTATION
				{
					plr->Reputation_OnKilledUnit( pVictim, false );
					RemoveFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_LASTKILLWITHHONOR );
				}
			}
		}
		/* -------------------------------- HONOR + BATTLEGROUND CHECKS END------------------------ */

		// Wipe our attacker set on death
		pVictim->CombatStatus.Vanished();

		//		 sent to set. don't send it to the party, becuase if they're out of
		//		 range they won't know this guid exists -> possible 132.

		/*if (this->IsPlayer())
			if( static_cast< Player* >( this )->InGroup() )
				static_cast< Player* >( this )->GetGroup()->SendPartyKillLog( this, pVictim );*/

		/* Stop Unit from attacking */
		if( this->IsPlayer() )
			static_cast< Player* >( this )->EventAttackStop();
	   
		if( this->IsUnit() )
		{
			CALL_SCRIPT_EVENT( this, OnTargetDied )( pVictim );
			static_cast< Unit* >( this )->smsg_AttackStop( pVictim );
		
			/* Tell Unit that it's target has Died */
			static_cast< Unit* >( this )->addStateFlag( UF_TARGET_DIED );

			// We will no longer be attacking this target, as it's dead.
			//static_cast<Unit*>(this)->setAttackTarget(NULL);
		}
		//so now we are completely dead
		//lets see if we have spirit of redemption
		if( pVictim->IsPlayer() )
		{
			if( static_cast< Player* >( pVictim)->HasSpell(20711)) //check for spirit of Redemption
			{

				Spell* sor = new Spell(pVictim, dbcSpell.LookupEntry(27827), true, NULL);
				Spell* sor2 = new Spell(pVictim, dbcSpell.LookupEntry(27792), true, NULL);
				Spell* sor3 = new Spell(pVictim, dbcSpell.LookupEntry(27795), true, NULL);
				SpellCastTargets targets(pVictim);
				sor->prepare(&targets);
				sor2->prepare(&targets);
				sor3->prepare(&targets);
			}
		}
		uint64 victimGuid = pVictim->GetGUID();

		if(pVictim->GetTypeId() == TYPEID_UNIT)
		{
			pVictim->GetAIInterface()->OnDeath(this);
			if(GetTypeId() == TYPEID_PLAYER)
			{
				WorldPacket data(SMSG_PARTYKILLLOG, 16);
				data << GetGUID() << pVictim->GetGUID();
				SendMessageToSet(&data, true);
			}			

			// it Seems that pets some how dont get a name and cause a crash here
			//bool isCritter = (pVictim->GetCreatureName() != NULL)? pVictim->GetCreatureName()->Type : 0;

			//-----------------------------------LOOOT--------------------------------------------
			if ((!pVictim->IsPet())&& ( !isCritter ) && pVictim->GetPlayerFrom() == NULL)
			{
				Creature * victim = static_cast<Creature*>(pVictim);
				// fill loot vector.
				victim->generateLoot();

				Player *owner = 0;
				if(victim->TaggerGuid)
					owner = GetMapMgr()->GetPlayer( (uint32)victim->TaggerGuid );

				if(owner == 0)  // no owner
				{
					// donno why this would happen, but anyway.. anyone can loot ;p
					// no owner no loot
					//victim->SetFlag(UNIT_DYNAMIC_FLAGS, U_DYN_FLAG_LOOTABLE);
				}
				else if (pVictim->loot.items.size() > 0 || pVictim->loot.gold > 0)
				{
					// Build the actual update.
					ByteBuffer buf( 500 );

					uint32 Flags = victim->m_uint32Values[ UNIT_DYNAMIC_FLAGS ];
					Flags |= U_DYN_FLAG_LOOTABLE;
					Flags |= U_DYN_FLAG_TAPPED_BY_PLAYER;

					victim->BuildFieldUpdatePacket( &buf, UNIT_DYNAMIC_FLAGS, Flags );

					// Check for owner's group.
					Group * pGroup = owner->GetGroup();
					if( pGroup != NULL )
					{
						// Owner was in a party.
						// Check loot method.
						switch( pGroup->GetMethod() )
						{
						case PARTY_LOOT_RR:
/*						//this commented code is not used because it was never tested and finished !
						{
								//get new tagger for creature
								Player *tp = pGroup->GetnextRRlooter();
								if(tp)
								{
									//we force on creature a new tagger
									victim->TaggerGuid = tp->GetGUID();
									victim->Tagged = true;
									if(tp->IsVisible(victim))  // Save updates for non-existant creatures
										tp->PushUpdateData(&buf, 1);
								}
							}break;*/
						case PARTY_LOOT_FFA:
						case PARTY_LOOT_GROUP:
						case PARTY_LOOT_NBG:
							{
								// Loop party players and push update data.
								GroupMembersSet::iterator itr;
								SubGroup * sGrp;
								pGroup->Lock();
								for( uint32 Index = 0; Index < pGroup->GetSubGroupCount(); ++Index )
								{
									sGrp = pGroup->GetSubGroup( Index );
									itr = sGrp->GetGroupMembersBegin();
									for( ; itr != sGrp->GetGroupMembersEnd(); ++itr )
									{
										if( (*itr)->m_loggedInPlayer && (*itr)->m_loggedInPlayer->IsVisible( victim ) )	   // Save updates for non-existant creatures
											(*itr)->m_loggedInPlayer->PushUpdateData( &buf, 1 );
									}
								}
								pGroup->Unlock();
							}break;
						case PARTY_LOOT_MASTER:
							{
								// Master loot: only the loot master gets the update.
								Player * pLooter = pGroup->GetLooter() ? pGroup->GetLooter()->m_loggedInPlayer : NULL;
								if( pLooter == NULL )
									pLooter = pGroup->GetLeader()->m_loggedInPlayer;

								if( pLooter->IsVisible( victim ) )  // Save updates for non-existant creatures
									pLooter->PushUpdateData( &buf, 1 );

							}break;
						}
					}
					else
					{
						// Owner killed the mob solo.
						if( owner->IsVisible( victim ) )
							owner->PushUpdateData( &buf, 1 );
					}
				}
			}
			//---------------------------------looot-----------------------------------------  
			
			if( GetTypeId() == TYPEID_PLAYER && 
				pVictim->GetUInt64Value( UNIT_FIELD_CREATEDBY ) == 0 && 
				pVictim->GetUInt64Value( OBJECT_FIELD_CREATED_BY ) == 0 &&
				!pVictim->IsPet() )
			{
				// TODO: lots of casts are bad make a temp member pointer to use for batches like this
				// that way no local loadhitstore and its just one assignment 

				// Is this player part of a group
				if( static_cast< Player* >( this)->InGroup() )
				{
					//Calc Group XP
					static_cast< Player* >( this )->GiveGroupXP( pVictim, static_cast< Player* >( this ) );
					//TODO: pet xp if player in group
				}
				else
				{
					uint32 xp = CalculateXpToGive( pVictim, static_cast< Unit* >( this ) );
					if( xp > 0 )
					{
						static_cast< Player* >( this )->GiveXP( xp, victimGuid, true );
						if( static_cast< Player* >( this )->GetSummon() && static_cast< Player* >( this )->GetSummon()->GetUInt32Value( UNIT_CREATED_BY_SPELL ) == 0 )
						{
							xp = CalculateXpToGive( pVictim, static_cast< Player* >( this )->GetSummon() );
							if( xp > 0 )
								static_cast< Player* >( this )->GetSummon()->GiveXP( xp );
						}
					}
				}

				if( pVictim->GetTypeId() != TYPEID_PLAYER )
					sQuestMgr.OnPlayerKill( static_cast< Player* >( this ), static_cast< Creature* >( pVictim ) );
			}
			else /* is Creature or GameObject*/
			{
				/* ----------------------------- PET XP HANDLING -------------- */
				if( owner_participe && IsPet() && !pVictim->IsPet() && static_cast< Pet* >(this)->GetPetOwner() && static_cast< Pet* >(this)->GetPetOwner()->IsPlayer() )
				{
					Player* petOwner = static_cast<Player*>(static_cast< Pet* >(this)->GetPetOwner());
					if( petOwner != NULL && petOwner->GetTypeId() == TYPEID_PLAYER )
					{
						if( petOwner->InGroup() )
						{
							//Calc Group XP
							static_cast< Unit* >( this )->GiveGroupXP( pVictim, petOwner );
							//TODO: pet xp if player in group
						}
						else
						{
							uint32 xp = CalculateXpToGive( pVictim, petOwner );
							if( xp > 0 )
							{
								petOwner->GiveXP( xp, victimGuid, true );
								if( !static_cast< Pet* >( this )->IsSummon() )
								{
									xp = CalculateXpToGive( pVictim, static_cast< Pet* >( this ) );
									if( xp > 0 )
										static_cast< Pet* >( this )->GiveXP( xp );
								}
							}
						}
					}
					if( petOwner != NULL && pVictim->GetTypeId() != TYPEID_PLAYER && 
						pVictim->GetTypeId() == TYPEID_UNIT )
						sQuestMgr.OnPlayerKill( petOwner, static_cast< Creature* >( pVictim ) );
				}
				/* ----------------------------- PET XP HANDLING END-------------- */

				/* ----------------------------- PET DEATH HANDLING -------------- */
				if( pVictim->IsPet() )
				{
					// dying pet looses 1 happiness level
					if( !static_cast< Pet* >( pVictim )->IsSummon() )
					{
						int32 happy_value = static_cast< Pet* >( pVictim )->GetLoyaltyPts();

						happy_value = happy_value - 350; // Le familier meurt = -350 sur la loyauté
						if(happy_value < 0) happy_value = 0;
						static_cast< Pet* >( pVictim )->SetLoyaltyPts(happy_value);
					}
					
					static_cast< Pet* >( pVictim )->DelayedRemove( false, true );
					
					//remove owner warlock soul link from caster
					if (static_cast<Pet*>( pVictim )->GetPetOwner() != NULL && static_cast<Pet*>( pVictim )->GetPetOwner()->IsPlayer())
					{
						Player* owner = static_cast<Player*>(static_cast<Pet*>( pVictim )->GetPetOwner());
						if( owner != NULL )
							owner->EventDismissPet();
						
					}
				}
				/* ----------------------------- PET DEATH HANDLING END -------------- */
				else if( pVictim->GetUInt64Value( UNIT_FIELD_CHARMEDBY ) )
				{
					//remove owner warlock soul link from caster
					Unit *owner=pVictim->GetMapMgr()->GetUnit( pVictim->GetUInt64Value( UNIT_FIELD_CHARMEDBY ) );
					if( owner != NULL && owner->IsPlayer())
						static_cast< Player* >( owner )->EventDismissPet();
				}
			}
		}
		else if( pVictim->GetTypeId() == TYPEID_PLAYER )
		{
			
			/* -------------------- RESET BREATH STATE ON DEATH -------------- */
			static_cast< Player* >( pVictim )->m_UnderwaterTime = 0;
			static_cast< Player* >( pVictim )->m_UnderwaterState = 0;
			static_cast< Player* >( pVictim )->m_BreathDamageTimer = 0;
			static_cast< Player* >( pVictim )->m_SwimmingTime = 0;

			/* -------------------- KILL PET WHEN PLAYER DIES ---------------*/
			if( static_cast< Player* >( pVictim )->GetSummon() != NULL )
			{
				if( pVictim->GetUInt32Value( UNIT_CREATED_BY_SPELL ) > 0 )
					static_cast< Player* >( pVictim )->GetSummon()->Dismiss( true );
				else
					static_cast< Player* >( pVictim )->GetSummon()->Remove( true, true, true );
			}
			/* -------------------- KILL PET WHEN PLAYER DIES END---------------*/
		}
		else sLog.outError("DealDamage for Unknown Object.");
}

void Object::KillSelf()
{
	if (!IsUnit())
		return;
	DealDamage(static_cast<Unit*>(this), 99999999, 0, 0, 0);
}

void Object::SetPhase(uint32 Phase)
{
	//if (notmask)
		m_Phase = Phase;
	/*else
		m_phaseMask |= phase;

	for (InRangeSet::iterator itr=m_objectsInRange.begin(); itr!=m_objectsInRange.end(); ++itr)
	{
		if ((*itr)->IsUnit())
			static_cast<Unit*>(*itr)->UpdateVisibility();
	}

	if (IsUnit())
		static_cast<Unit*>(this)->UpdateVisibility();
*/
}
void Object::EnablePhase(uint32 Phase)
{
 m_Phase |= Phase;
}
void Object::DisablePhase(uint32 Phase)
{
	m_Phase &= ~Phase;
/*
	for (InRangeSet::iterator itr=m_objectsInRange.begin(); itr!=m_objectsInRange.end(); ++itr)
	{
		if ((*itr)->IsUnit())
			static_cast<Unit*>(*itr)->UpdateVisibility();
	}

	if (IsUnit())
		static_cast<Unit*>(this)->UpdateVisibility();
		*/
}

bool Object::CanActivateCell() //expand this
{
	if (IsPlayer()) return true;

	return false;
}
//----------------------- Misc ---------------
void Object::SetByteFlag(const uint32 index, const uint32 flag, uint8 newFlag)
{
	if( HasByteFlag(index,flag,newFlag))
		return;

	SetByte(index, flag, GetByte(index,flag)|newFlag);

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}

void Object::RemoveByteFlag(const uint32 index, const uint32 flag, uint8 checkFlag)
{
	if( !HasByteFlag(index,flag,checkFlag))
		return;

	SetByte(index,flag, GetByte(index,flag) & ~checkFlag );

	if(IsInWorld())
	{
		m_updateMask.SetBit( index );

		if(!m_objectUpdated)
		{
			m_mapMgr->ObjectUpdated(this);
			m_objectUpdated = true;
		}
	}
}

bool Object::HasByteFlag(const uint32 index, const uint32 flag, uint8 checkFlag)
{
	if( GetByte(index,flag) & checkFlag )
		return true;
	else
		return false;
}

void Object::SetFlag64( const uint32 index, uint64 newFlag )
{
	//ASSERT( index < m_valuesCount );
	uint32 oldval = GetUInt64Value(index);
	uint32 newval = oldval | newFlag;
	SetUInt64Value(index, newval);
}

void Object::RemoveFlag64( const uint32 index, uint64 oldFlag )
{
	//ASSERT( index < m_valuesCount );
	uint32 oldval = GetUInt64Value(index);
	uint64 newval = oldval & ~oldFlag;
	SetUInt64Value(index, newval);
}

bool Object::IsInLineOfSight(Object* pObj) // NOT USED (Branruz)
{
#ifdef COLLISION
     float SourceTaille = ((Unit *)this)->GetSize();
     float TargetTaille = ((Unit *)pObj)->GetSize();
	 return (CollideInterface.CheckLOS( GetMapId(), GetPositionX(), GetPositionY(), GetPositionZ() + SourceTaille, // 2.0f, 
		                                pObj->GetPositionX(), pObj->GetPositionY(), pObj->GetPositionZ() + TargetTaille) ); // 2.0f) );
#else
		return true;
#endif
}

// _/!\_ Interaction avec la Base de donnée _/!\_
bool Object::PhasedCanInteract(Object* pObj)
{
	if((m_Phase == GHOST_PHASING) || (pObj->m_Phase == GHOST_PHASING) ) return(true);

	// -1: All phases. But perhaps this should be limited to GameObjects?
	if( (m_Phase == ALL_PHASES) || (pObj->m_Phase == ALL_PHASES) ) return(true);

    // Phasing de l'objet correspond avec la phase en court
	if( (pObj->m_Phase & m_Phase) || (pObj->m_Phase == m_Phase) ) return(true);

	/* Pourquoi ca ?? A debatre.... (Branruz)
	Player *pObjI = IsPlayer() ? static_cast<Player *>(this) : NULL;
	Player *pObjII = pObj->IsPlayer() ? static_cast<Player *>(pObj) : NULL;

	if( IsPet() )        pObjI  = static_cast<Player*>(static_cast<Pet*>(this)->GetOwner());
	if( pObj->IsPet() )  pObjII = static_cast<Player*>(static_cast<Pet*>(pObj)->GetPetOwner());

	// Hack for Acherus: Horde/Alliance can't see each other!
	if( pObjI && pObjII)
	{
	 if( (GetMapId() == 609) && (pObjI->GetTeam() == pObjII->GetTeam()) )
	 {
		return(true);
	 }
	}
	*/

	return(false);
}

// Returns the base cost of a spell
int32 Object::GetSpellBaseCost(SpellEntry *sp)
{
	float cost;
	if( sp->ManaCostPercentage && IsUnit() )//Percentage spells cost % of !!!BASE!!! mana
	{
		if( sp->powerType==POWER_TYPE_MANA)
			cost = GetUInt32Value(UNIT_FIELD_BASE_MANA) * (sp->ManaCostPercentage / 100.0f);
		else
			cost = GetUInt32Value(UNIT_FIELD_BASE_HEALTH) * (sp->ManaCostPercentage / 100.0f);
	}
	else 
	{
		cost = (float)sp->manaCost;
	}

	return float2int32(cost); // Truncate zeh decimals!
}


void Object::CastSpell( Object *Target, SpellEntry* Sp, bool triggered )
{
	//uint64 RandVal = 0;
	if( Sp == NULL ) return;

	Spell *newSpell = new Spell(this, Sp, triggered, NULL);
	SpellCastTargets targets; // SpellCastTargets targets(RandVal); // Test (Branruz)

	if(Target)
	{
		if(Target->IsUnit())
		{
			targets.m_targetMask = TARGET_FLAG_UNIT;
		}
		else
		{
			targets.m_targetMask = TARGET_FLAG_OBJECT;
		}
		targets.m_target = Target; // targets.m_unitTarget = Target->GetGUID();
	}
	else
	{
		newSpell->GenerateTargets(&targets);
	}
	newSpell->prepare(&targets);
}

void Object::CastSpell( Object *Target, uint32 SpellID, bool triggered )
{
	SpellEntry * ent = dbcSpell.LookupEntry(SpellID);
	if(ent == 0) return;

	CastSpell(Target, ent, triggered);
}

void Object::CastSpell( uint64 targetGuid, SpellEntry* Sp, bool triggered )
{
	if( Sp == NULL )
		return;

	SpellCastTargets targets(GetMapMgr()->GetUnit(targetGuid));
	Spell *newSpell = new Spell(this, Sp, triggered, NULL);
	newSpell->prepare(&targets);
}

void Object::CastSpell( uint64 targetGuid, uint32 SpellID, bool triggered )
{
	SpellEntry * ent = dbcSpell.LookupEntry(SpellID);
	if(ent == 0) return;

	CastSpell(targetGuid, ent, triggered);

}
float Object::GetSize() 
{ 
        float uTaille = 0;

		if(this->IsPlayer())
		{
			uTaille = (((Player *)this)->Taille) * GetFloatValue(OBJECT_FIELD_SCALE_X);
			uTaille = uTaille*GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS);
		}
		else if(this->IsCreature())
		{
			uTaille = (((Creature *)this)->Taille) * GetFloatValue(OBJECT_FIELD_SCALE_X);
			uTaille = uTaille*GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS);
		}
		else if(this->IsItem()) 
		{
			uTaille = 0;
		}
		else if(this->IsGO())
		{
           uTaille = 1;
		}
		else uTaille = 1;

		return ( uTaille );
		//return (GetFloatValue(OBJECT_FIELD_SCALE_X) * GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS)); 
}

