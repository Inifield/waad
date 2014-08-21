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
GameObject::GameObject(uint64 guid)
{
	m_objectTypeId = TYPEID_GAMEOBJECT;
	m_valuesCount = GAMEOBJECT_END;
	m_uint32Values = _fields;
	memset(m_uint32Values, 0,(GAMEOBJECT_END)*sizeof(uint32));
	m_updateMask.SetCount(GAMEOBJECT_END);
	SetUInt32Value( OBJECT_FIELD_TYPE,TYPE_GAMEOBJECT|TYPE_OBJECT);
	SetUInt64Value( OBJECT_FIELD_GUID,guid);
	m_wowGuid.Init(GetGUID());
 
	SetFloatValue( OBJECT_FIELD_SCALE_X, 1);//info->Size  );
	
	SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_ANIMPROGRESS, 100);	

	counter=0;//not needed at all but to prevent errors that var was not inited, can be removed in release

	bannerslot = bannerauraslot = -1;

	m_summonedGo = false;
	invisible = false;
	invisibilityFlag = INVIS_FLAG_NORMAL;
	spell = 0;
	m_summoner = NULL;
	charges = -1;
	m_ritualcaster = 0;
	m_ritualtarget = 0;
	m_ritualmembers = NULL;
	m_ritualspell = 0;
	m_rotation = 0;

	m_quests = NULL;
	pInfo = NULL;
	myScript = NULL;
	m_spawn = 0;
	loot.gold = 0;
	m_deleted = false;
	mines_remaining = 1;
	m_respawnCell=NULL;
	m_battleground = NULL;
}


void GameObject::Init()
{
	Object::Init();
}

GameObject::~GameObject()
{
	sEventMgr.RemoveEvents(this);
	if(m_ritualmembers)
	delete[] m_ritualmembers;

	uint32 guid = GetUInt32Value(OBJECT_FIELD_CREATED_BY);
	if(guid)
	{
		Player *plr = objmgr.GetPlayer(guid);
		if(plr && plr->GetSummonedObject() == this)
			plr->SetSummonedObject(NULL);

		if(plr == m_summoner)
			m_summoner = 0;
	}
	
	if (m_battleground != NULL)
		m_battleground = NULL;
		
	if(m_respawnCell!=NULL)
		m_respawnCell->_respawnObjects.erase(this);

	if (m_summonedGo && m_summoner)
		for(int i = 0; i < 4; i++)
			if (m_summoner->m_ObjectSlots[i] == GetLowGUID())
				m_summoner->m_ObjectSlots[i] = 0;

	if( m_battleground != NULL && m_battleground->GetType() == BATTLEGROUND_ARATHI_BASIN )
	{
		if( bannerslot >= 0 && static_cast<ArathiBasin*>(m_battleground)->m_controlPoints[bannerslot] == this )
			static_cast<ArathiBasin*>(m_battleground)->m_controlPoints[bannerslot] = NULL;

		if( bannerauraslot >= 0 && static_cast<ArathiBasin*>(m_battleground)->m_controlPointAuras[bannerauraslot] == this )
			static_cast<ArathiBasin*>(m_battleground)->m_controlPointAuras[bannerauraslot] = NULL;
	}
	
}

bool GameObject::CreateFromProto(uint32 entry,uint32 mapid, float x, float y, float z, float ang)
{
	pInfo= GameObjectNameStorage.LookupEntry(entry);
	if(!pInfo)return false;

	Object::_Create( mapid, x, y, z, ang );
	SetUInt32Value( OBJECT_FIELD_ENTRY, entry );
	// disparu 332.11403
	//SetFloatValue( GAMEOBJECT_POS_X, x );
	//SetFloatValue( GAMEOBJECT_POS_Y, y );
	//SetFloatValue( GAMEOBJECT_POS_Z, z );
	//SetFloatValue( GAMEOBJECT_FACING, ang );
	SetRotation();
	//SetRotation(ang);
	
	// SetUInt32Value( GAMEOBJECT_TIMESTAMP, (uint32)UNIXTIME);
	// SetUInt32Value( GAMEOBJECT_ARTKIT, 0 );		   //these must be from wdb somewhere i guess
    // SetUInt32Value( GAMEOBJECT_ANIMPROGRESS, 0 );

	SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, 1);
	SetUInt32Value( GAMEOBJECT_DISPLAYID, pInfo->DisplayID );
	SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID, pInfo->Type);
   
	InitAI();

	 return true;
	/*
	original_flags = m_uint32Values[GAMEOBJECT_FLAGS];
	original_state = m_uint32Values[GAMEOBJECT_STATE];
	*/
}
/*
void GameObject::Create(uint32 mapid, float x, float y, float z, float ang)
{
	Object::_Create( mapid, x, y, z, ang);

	SetFloatValue( GAMEOBJECT_POS_X, x);
	SetFloatValue( GAMEOBJECT_POS_Y, y );
	SetFloatValue( GAMEOBJECT_POS_Z, z );
	SetFloatValue( GAMEOBJECT_FACING, ang );
	//SetUInt32Value( GAMEOBJECT_TIMESTAMP, (uint32)time(NULL));
}

void GameObject::Create( uint32 guidlow, uint32 guidhigh,uint32 displayid, uint8 state, uint32 entryid, float scale,uint32 typeId, uint32 type,uint32 flags, uint32 mapid, float x, float y, float z, float ang )
{
	Object::_Create( mapid, x, y, z, ang);

	SetUInt32Value( OBJECT_FIELD_ENTRY, entryid );
	SetFloatValue( OBJECT_FIELD_SCALE_X, scale );
	SetUInt32Value( GAMEOBJECT_DISPLAYID, displayid );
	SetUInt32Value( GAMEOBJECT_STATE, state  );
	SetUInt32Value( GAMEOBJECT_TYPE_ID, typeId  );
	SetUInt32Value( GAMEOBJECT_FLAGS, flags );
}*/

void GameObject::EventCastSpell(uint32 guid, uint32 sp, bool triggered)
{
	Spell * spp = new Spell(this,dbcSpell.LookupEntry(sp),false,NULL);
	SpellCastTargets tars(GetMapMgr()->GetUnit(guid));
	spp->prepare(&tars);
}

void GameObject::TrapSearchTarget()
{
	Update(100);
}

void GameObject::Update(uint32 p_time)
{
//	UpdateInRangeSet();
	if(m_event_Instanceid != m_instanceId)
	{
		event_Relocate();
		return;
	}

	if(!IsInWorld())
		return;

	if(m_deleted)
		return;

	if(spell && (GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID) == 1))
	{
		if(checkrate > 1)
		{
			if(counter++%checkrate)
				return;
		}
		ObjectSet::iterator itr = GetInRangeSetBegin();
		ObjectSet::iterator it2 = itr;
		ObjectSet::iterator iend = GetInRangeSetEnd();
		Unit * pUnit;
		float dist;
		for(; it2 != iend;)
		{
			itr = it2;
			++it2;
			dist = GetDistanceSq((*itr));
			if( (*itr) != m_summoner && (*itr)->IsUnit() && dist <= range)
			{
				pUnit = static_cast<Unit*>(*itr);

				if(m_summonedGo)
				{
					if(!m_summoner)
					{
						ExpireAndDelete();
						return;
					}
					if(!isAttackable(m_summoner,pUnit))continue;
				}
				
				Spell * sp=new Spell((Object*)this,spell,true,NULL);
				SpellCastTargets tgt(*itr);
				tgt.m_destX = GetPositionX();
				tgt.m_destY = GetPositionY();
				tgt.m_destZ = GetPositionZ();
				sp->prepare(&tgt);
				if(pInfo->Type == 6)
				{
					if(m_summoner != NULL)
						m_summoner->HandleProc(PROC_ON_TRAP_TRIGGER, pUnit, spell);
				} 
				
				if(m_summonedGo)
				{
					ExpireAndDelete();
					return;
				}

				if(spell->EffectImplicitTargetA[0] == EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT ||
					spell->EffectImplicitTargetB[0] == EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT)
					return;	 // on area dont continue.
			}
		}
	}
}

void GameObject::Spawn(MapMgr * m)
{
	PushToWorld(m);	
	CALL_GO_SCRIPT_EVENT(this, OnSpawn)();
}

void GameObject::Despawn(uint32 time)
{
	if(!IsInWorld())
		return;

	loot.items.clear();

	//This is for go get deleted while looting
	if(m_spawn)
	{
		SetByte(GAMEOBJECT_BYTES_1, 0, m_spawn->state);
		SetUInt32Value(GAMEOBJECT_FLAGS, m_spawn->flags);
	}

	CALL_GO_SCRIPT_EVENT(this, OnDespawn)();

	if(time)
	{
		/* Get our originiating mapcell */
		MapCell * pCell = m_mapCell;
		//ASSERT(pCell);
		if(pCell == NULL)
		{
			Log.Error("GameObject::Despawn","pCell == NULL, Time: %u, Go: %u",this->GetEntry());
		}
		else
		{
		 pCell->_respawnObjects.insert( ((Object*)this) );
		 sEventMgr.RemoveEvents(this);
		 sEventMgr.AddEvent(m_mapMgr, &MapMgr::EventRespawnGameObject, this, pCell, EVENT_GAMEOBJECT_ITEM_SPAWN, time, 1, 0);
		 Object::RemoveFromWorld(false);
		 m_respawnCell=pCell;
		}
	}
	else

		ExpireAndDelete();
}

void GameObject::SaveToDB()
{
	std::stringstream ss;
	ss << "REPLACE INTO gameobject_spawns VALUES("
		<< ((m_spawn == NULL) ? 0 : m_spawn->id) << ","
		<< GetEntry() << ","
		<< GetMapId() << ","
		<< GetPositionX() << ","
		<< GetPositionY() << ","
		<< GetPositionZ() << ","
		<< GetOrientation() << ","
		//<< GetUInt64Value(GAMEOBJECT_ROTATION) << "," // Test (Branruz)
		<< GetFloatValue(GAMEOBJECT_PARENTROTATION) << ","
		<< GetFloatValue(GAMEOBJECT_PARENTROTATION_1) << ","
		<< GetFloatValue(GAMEOBJECT_PARENTROTATION_2) << ","
		<< GetFloatValue(GAMEOBJECT_PARENTROTATION_3) << ","
		<< (uint32)( GetByte(GAMEOBJECT_BYTES_1, 0)? 1 : 0 ) << ","
		<< GetUInt32Value(GAMEOBJECT_FLAGS) << ","
		<< GetUInt32Value(GAMEOBJECT_FACTION) << ","
		<< GetFloatValue(OBJECT_FIELD_SCALE_X) << ","
		<< m_Phase <<")";
	WorldDatabase.Execute(ss.str().c_str());
}

void GameObject::SaveToFile(std::stringstream & name)
{
/*	std::stringstream ss;
	if (!m_sqlid)
		m_sqlid = objmgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT);

	 ss.rdbuf()->str("");
	 ss << "INSERT INTO gameobjects VALUES ( "
		<< m_sqlid << ", "
		<< m_position.x << ", "
		<< m_position.y << ", "
		<< m_position.z << ", "
		<< m_position.o << ", "
		<< GetZoneId() << ", "
		<< GetMapId() << ", '";

	for( uint32 index = 0; index < m_valuesCount; index ++ )
		ss << GetUInt32Value(index) << " ";

	ss << "', ";
	ss << GetEntry() << ", 0, 0)"; 

	FILE * OutFile;

	OutFile = fopen(name.str().c_str(), "wb");
	if (!OutFile) return;
	fwrite(ss.str().c_str(),1,ss.str().size(),OutFile);
	fclose(OutFile);
*/
}

void GameObject::InitAI()
{
	//Log.Notice("[GameObject::InitAI]","Init ");
	if(!pInfo)
		return;
	
	// this fixes those fuckers in booty bay
	if(pInfo->raw.SpellFocus == 0 &&
		pInfo->raw.sound1 == 0 &&
		pInfo->raw.sound2 == 0 &&
		pInfo->raw.sound3 != 0 &&
		pInfo->raw.sound5 != 3 &&
		pInfo->raw.sound9 == 1)
		return;

	uint32 spellid = 0;
	if(pInfo->Type==GAMEOBJECT_TYPE_TRAP)
	{	
		spellid = pInfo->trap.spellId; // sound3
	}
	else if(pInfo->Type == GAMEOBJECT_TYPE_SPELL_FOCUS)//redirect to properties of another go
	{
		uint32 new_entry = pInfo->trap.radius; // sound2
		if(!new_entry)
			return;
		pInfo = GameObjectNameStorage.LookupEntry( new_entry );
		if(!pInfo)
			return;
		spellid = pInfo->trap.spellId; // sound3
	}
	else if(pInfo->Type == GAMEOBJECT_TYPE_RITUAL)
	{	
		m_ritualmembers = new uint32[pInfo->summoningRitual.reqParticipants]; // SpellFocus
		memset(m_ritualmembers,0,sizeof(uint32)*pInfo->summoningRitual.reqParticipants); // SpellFocus
	}
    else if(pInfo->Type == GAMEOBJECT_TYPE_CHEST)
    {
		LockEntry *pLock = dbcLock.LookupEntry(GetInfo()->chest.lockId); // SpellFocus
        if(pLock)
        {
            for(uint32 i=0; i < 5; i++)
            {
                if(pLock->locktype[i])
                {
                    if(pLock->locktype[i] == 2) //locktype;
                    {
                        //herbalism and mining;
                        if(pLock->lockmisc[i] == LOCKTYPE_MINING || pLock->lockmisc[i] == LOCKTYPE_HERBALISM)
                        {
							CalcMineRemaining(true);
                        }
                    }
                }
            }
        }

    }
	else if(pInfo->Type == GAMEOBJECT_TYPE_MO_TRANSPORT)
    {
     // Pas de spellId pour un transport
     spellid = 0;
	 return; 
	}
	myScript = sScriptMgr.CreateAIScriptClassForGameObject(GetEntry(), this);

	// hackfix for bad spell in BWL
	if(!spellid || (spellid == __Suppression_Aura) ) // 22247
		return;

	SpellEntry *sp= dbcSpell.LookupEntry(spellid);
	if(!sp)
	{
		spell = NULL;
		return;
	}
	else
	{
		spell = sp;
	}
	//ok got valid spell that will be casted on target when it comes close enough
	//get the range for that 
	
	float r = 0;

	for(uint32 i=0;i<3;i++)
	{
		if(sp->Effect[i])
		{
			float t = GetRadius(dbcSpellRadius.LookupEntry(sp->EffectRadiusIndex[i]));
			if(t > r)
				r = t;
		}
	}

	if(r < 0.1)//no range
		r = GetMaxRange(dbcSpellRange.LookupEntry(sp->rangeIndex));

	range = r*r;//square to make code faster
	checkrate = 20;//once in 2 seconds
	
}

bool GameObject::Load(GOSpawn *spawn)
{
	if(!CreateFromProto(spawn->entry,0,spawn->x,spawn->y,spawn->z,spawn->facing))
		return false;

	m_Phase = spawn->phase; // Note Randdrick - 3.22 - : indispensable pour que les npc se chargent correctement
	m_spawn = spawn;
	//SetRotation(spawn->facing);	

	//SetFloatValue(GAMEOBJECT_PARENTROTATION   ,spawn->o);
	//SetFloatValue(GAMEOBJECT_PARENTROTATION_1 ,spawn->o1);
	//SetFloatValue(GAMEOBJECT_PARENTROTATION_2 ,spawn->o2);
	//SetFloatValue(GAMEOBJECT_PARENTROTATION_3 ,spawn->o3);

	SetUInt32Value(GAMEOBJECT_FLAGS,spawn->flags);
//	SetUInt32Value(GAMEOBJECT_LEVEL,spawn->level);
	SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, spawn->state);	
	if(spawn->faction)
	{
		SetUInt32Value(GAMEOBJECT_FACTION,spawn->faction);
		m_faction = dbcFactionTemplate.LookupEntry(spawn->faction);
		if(m_faction)
			m_factionDBC = dbcFaction.LookupEntry(m_faction->Faction);
	}
	SetFloatValue(OBJECT_FIELD_SCALE_X,spawn->scale);
	
	//if( spawn->flags & GO_FLAG_IN_USE || spawn->flags & GO_FLAG_LOCKED )
	//	SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
		
	_LoadQuests();
	CALL_GO_SCRIPT_EVENT(this, OnCreate)();
	CALL_GO_SCRIPT_EVENT(this, OnSpawn)();
	
	InitAI();

	_LoadQuests();
	return true;
}

void GameObject::DeleteFromDB()
{
	if( m_spawn != NULL )
		WorldDatabase.Execute("DELETE FROM gameobject_spawns WHERE id=%u", m_spawn->id);
}

void GameObject::EventCloseDoor()
{
	SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, 0);
}

void GameObject::UseFishingNode(Player *player)
{
	sEventMgr.RemoveEvents( this );
	if( GetUInt32Value( GAMEOBJECT_FLAGS ) != GO_FLAG_NO_DESPAWN ) // Clicking on the bobber before something is hooked
	{
		player->GetSession()->OutPacket( SMSG_FISH_NOT_HOOKED );
		EndFishing( player, true );
		return;
	}
	
	/* Unused code: sAreaStore.LookupEntry(GetMapMgr()->GetAreaID(GetPositionX(),GetPositionY()))->ZoneId*/
	uint32 zone = player->GetAreaID();
	if( zone == 0 ) // If the player's area ID is 0, use the zone ID instead
		zone = player->GetZoneId();

	FishingZoneEntry *entry = FishingZoneStorage.LookupEntry( zone );
	if( entry == NULL ) // No fishing information found for area or zone, log an error, and end fishing
	{
		sLog.outError( "ERROR: Fishing zone information for zone %d not found!", zone );
		EndFishing( player, true );
		return;
	}
	uint32 maxskill = entry->MaxSkill;
//	uint32 minskill = entry->MaxSkill;
	uint32 minskill = entry->MinSkill;

	if( player->_GetSkillLineCurrent( SKILL_FISHING, false ) < maxskill )	
		player->_AdvanceSkillLine( SKILL_FISHING, float2int32( 1.0f * sWorld.getRate( RATE_SKILLRATE ) ) );

	// Open loot on success, otherwise FISH_ESCAPED.
	if( Rand(((player->_GetSkillLineCurrent( SKILL_FISHING, true ) - minskill) * 100) / maxskill) )
	{
		lootmgr.FillFishingLoot( &loot, zone );
		player->SendLoot( GetGUID(), LOOT_FISHING );
		EndFishing( player, false );
	}
	else // Failed
	{
		player->GetSession()->OutPacket( SMSG_FISH_ESCAPED );
		EndFishing( player, true );
	}

}

void GameObject::EndFishing(Player* player, bool abort )
{
	Spell * spell = player->GetCurrentSpell();
	
	if(spell)
	{
		if(abort)   // abort becouse of a reason
		{
			//FIXME: here 'failed' should appear over progress bar
			spell->SendChannelUpdate(0);
			//spell->cancel();
			spell->finish();
		}
		else		// spell ended
		{
			spell->SendChannelUpdate(0);
			spell->finish();
		}
	}

	if(!abort)
		sEventMgr.AddEvent(this, &GameObject::ExpireAndDelete, EVENT_GAMEOBJECT_EXPIRE, 10000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	else
		ExpireAndDelete();
}

void GameObject::FishHooked(Player * player)
{
	WorldPacket  data(12);
	data.Initialize(SMSG_GAMEOBJECT_CUSTOM_ANIM); 
	data << GetGUID();
	data << (uint32)(0); // value < 4
	player->GetSession()->SendPacket(&data);
	//SetUInt32Value(GAMEOBJECT_STATE, 0);
	//BuildFieldUpdatePacket(player, GAMEOBJECT_FLAGS, GO_FLAG_NODESPAWN);
	SetUInt32Value(GAMEOBJECT_FLAGS, GO_FLAG_NO_DESPAWN);
 }

/////////////
/// Quests

void GameObject::AddQuest(QuestRelation *Q)
{
	m_quests->push_back(Q);
}

void GameObject::DeleteQuest(QuestRelation *Q)
{
	list<QuestRelation *>::iterator it;
	for( it = m_quests->begin(); it != m_quests->end(); ++it )
	{
		if( ( (*it)->type == Q->type ) && ( (*it)->qst == Q->qst ) )
		{
			delete (*it);
			m_quests->erase(it);
			break;
		}
	}
}

Quest* GameObject::FindQuest(uint32 quest_id, uint8 quest_relation)
{   
	list< QuestRelation* >::iterator it;
	for( it = m_quests->begin(); it != m_quests->end(); ++it )
	{
		QuestRelation* ptr = (*it);
		if( ( ptr->qst->id == quest_id ) && ( ptr->type & quest_relation ) )
		{
			return ptr->qst;
		}
	}
	return NULL;
}

uint16 GameObject::GetQuestRelation(uint32 quest_id)
{
	uint16 quest_relation = 0;
	list< QuestRelation* >::iterator it;
	for( it = m_quests->begin(); it != m_quests->end(); ++it )
	{
		if( (*it) != NULL && (*it)->qst->id == quest_id )
		{
			quest_relation |= (*it)->type;
		}
	}
	return quest_relation;
}

uint32 GameObject::NumOfQuests()
{
	return (uint32)m_quests->size();
}

void GameObject::_LoadQuests()
{
	sQuestMgr.LoadGOQuests(this);
}

/////////////////
// Summoned Go's

void GameObject::_Expire()
{
	sEventMgr.RemoveEvents(this);
	if(IsInWorld())
		RemoveFromWorld(true);

	//sEventMgr.AddEvent(World::getSingletonPtr(), &World::DeleteObject, ((Object*)this), EVENT_DELETE_TIMER, 1000, 1);
}

void GameObject::ExpireAndDelete()
{
	if(m_deleted)
		return;

	m_deleted = true;
	
	/* remove any events */
	sEventMgr.RemoveEvents(this);
	sEventMgr.AddEvent(this, &GameObject::_Expire, EVENT_GAMEOBJECT_EXPIRE, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void GameObject::Deactivate()
{
	SetUInt32Value(GAMEOBJECT_DYNAMIC, 0);
}

void GameObject::CallScriptUpdate()
{
	ASSERT(myScript);
	myScript->AIUpdate();
}

void GameObject::OnPushToWorld()
{
	Object::OnPushToWorld();
}

void GameObject::OnRemoveInRangeObject(Object* pObj)
{
	Object::OnRemoveInRangeObject(pObj);
	if(m_summonedGo && m_summoner == pObj)
	{
		for(int i = 0; i < 4; i++)
			if (m_summoner->m_ObjectSlots[i] == GetLowGUID())
				m_summoner->m_ObjectSlots[i] = 0;

		m_summoner = 0;
		ExpireAndDelete();
	}
}

void GameObject::RemoveFromWorld(bool free_guid)
{
	WorldPacket data(SMSG_GAMEOBJECT_DESPAWN_ANIM, 8);
	data << GetGUID();
	SendMessageToSet(&data,true);

	sEventMgr.RemoveEvents(this, EVENT_GAMEOBJECT_TRAP_SEARCH_TARGET);
	Object::RemoveFromWorld(free_guid);
}

bool GameObject::HasLoot()
{
    int count=0;
    for(vector<__LootItem>::iterator itr = loot.items.begin(); itr != loot.items.end(); ++itr)
	{
		if( itr->item.itemproto->Bonding == ITEM_BIND_QUEST || itr->item.itemproto->Bonding == ITEM_BIND_QUEST2 )
			continue;

		count += (itr)->iItemsCount;
	}

    return (count>0);

}

uint32 GameObject::GetGOReqSkill()  
{
	if(GetEntry() == 180215) return 300;

	if(GetInfo() == NULL)
		return 0;

	LockEntry *lock = dbcLock.LookupEntry( GetInfo()->raw.SpellFocus ); // lockId
	if(!lock) return 0;
	for(uint32 i=0;i<5;i++)
		if(lock->locktype[i] == 2 && lock->minlockskill[i])
		{
			return lock->minlockskill[i];
		}
	return 0;
}

/*const char* GameObject::GetObjectName()
{
	if (GetInfo() == NULL)
		return "Unknown";
	return GetInfo()->Name;
}*/

// Wotlk GameObject Rotation Stuff
// void GameObject::SetRotation(float rad)
void GameObject::SetRotation()
{
	static double const atan_pow = atan(pow(2.0f, -20.0f));

	double f_rot1 = sin(GetOrientation() / 2.0f);
	double f_rot2 = cos(GetOrientation() / 2.0f);

	int64 i_rot1 = int64(f_rot1 / atan_pow *(f_rot2 >= 0 ? 1.0f : -1.0f));
	int64 rotation = (i_rot1 << 43 >> 43) & 0x00000000001FFFFF;

	m_rotation = rotation;

	float r2=GetFloatValue(GAMEOBJECT_PARENTROTATION_2);
	float r3=GetFloatValue(GAMEOBJECT_PARENTROTATION_3);

	if(r2==0.0f && r3==0.0f)
	{
		r2 = f_rot1;
		r3 = f_rot2;
	}
		SetFloatValue(GAMEOBJECT_PARENTROTATION_2, r2);
		SetFloatValue(GAMEOBJECT_PARENTROTATION_3, r3);

	/*if (rad < 0)
	{
		rad = float((360.0f * (M_PI / 180.0f)) - (-rad));
		m_rotation = rad;
	}
	float deg = (float)(rad * (180.0f / (float)M_PI));
	if (deg > 180.0f)
	{
		rad = (1.0f - sinf(rad/2.0f)) / 8.0f + 1.125f;
		m_rotation = rad;
	}
	else
	{
		rad = sinf(rad/2.0f) / 8.0f + 1.0f;
		m_rotation = rad;
	}
	SetFloatValue(GAMEOBJECT_ROTATION, rad);*/
}
//Destructable Buildings
void GameObject::TakeDamage(uint32 ammount)
{
	if(GetInfo()->Type != GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING) 
	{
		Log.Warning("TakeDamage","%u n'est pas un objet destructible.",pInfo->ID);
		return;
	}

	if(!GetInfo()->destructibleBuilding.Used) // Unknown2
	{
		Log.Warning("TakeDamage","%u ne peut pas etre detruit.",pInfo->ID);
		return; // Destructible, mais pas utilisé, normalement, tt est a 0
	}

	if(Health >= ammount)      Health -= ammount;
	else if(Health < ammount) Health = 0;

	//if(Health == 0) return;

	bool AlreadyDestroy = (GetInfo()->destructibleBuilding.DisplayId_Detruit /*Unknown1*/) ? false:true;

	// Detruction en %
	uint32 HealthPct    = (uint32)((((float)(HealthMax - Health))/(float)HealthMax)*100);
	//uint32 HealthMidPct = (uint32)((((float)(HealthMax - HealthMid))/(float)HealthMax)*100);

	uint32 goFlag = this->GetUInt32Value(GAMEOBJECT_FLAGS);

	goFlag = (goFlag & _GAMEOBJECT_DESTRUCT_MASK);

	switch(goFlag)
	{
	 default: if(HealthPct >= 10) // 10% de destruction atteint
				   SetDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_10,true);  
		      break;

     case GO_FLAG_DAMAGED_10 : if(HealthPct >= 20) // 20% ou plus atteint, etc...
							   {
		                        RemoveDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_10,false);
			                    SetDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_20,true); 
							   }
		                       else break; 

     case GO_FLAG_DAMAGED_20 : // GO_FLAG_DAMAGED (Ascent)
		                       if(HealthPct >= 30)
							   {
		                        RemoveDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_20,false);
			                    SetDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_30,true); 
							   }
		                       else break;

     case GO_FLAG_DAMAGED_30 : if(HealthPct >= 40)
							   { 
		                        RemoveDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_30,false);
			                    SetDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_40,true);
							   }
		                       else break;

    case GO_FLAG_DAMAGED_40 : // GO_FLAG_DESTROYED (Ascent)
		                      if(HealthPct >= 50)
							  {
								// Pour l'instant le SpellInfo est egal à Sound5, chgt a 50% de destruction
								SetUInt32Value(GAMEOBJECT_DISPLAYID,GetInfo()->destructibleBuilding.DisplayId_50PCT); // sound4
								Log.Warning("TakeDamage","%u : DispalyId: %u.",GetInfo()->ID,GetUInt32Value(GAMEOBJECT_DISPLAYID));
							    if(AlreadyDestroy) HealthPct = 0;

								RemoveDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_40,false);
			                    SetDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_50,true);
							  }
		                      else break;

	case GO_FLAG_DAMAGED_50 : if(HealthPct >= 60)
							  {
		                        RemoveDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_50,false);
			                    SetDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_60,true);
							  }
		                       else break;

     case GO_FLAG_DAMAGED_60 : if(HealthPct >= 70)
							   {
		                        RemoveDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_60,false);
			                    SetDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_70,true);
							   }
		                       else break;

     case GO_FLAG_DAMAGED_70 : if(HealthPct >= 80)
							   {
		                        RemoveDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_70,false);
			                    SetDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_80,true);
							   }
		                       else break;

     case GO_FLAG_DAMAGED_80 : if(HealthPct >= 90)
							   {
		                        RemoveDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_80,false);
			                    SetDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_90,true);  
							   }
		                       else break;

      case GO_FLAG_DAMAGED_90 :if(HealthPct >= 100) // 100% detruit
							   {
		                        if(!AlreadyDestroy) 
								{
									SetUInt32Value(GAMEOBJECT_DISPLAYID,GetInfo()->destructibleBuilding.DisplayId_Detruit); // Unknown1
									Log.Warning("TakeDamage","%u : DispalyId: %u.",GetInfo()->ID,GetUInt32Value(GAMEOBJECT_DISPLAYID));
								}
								RemoveDamageFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_90,true);
								sHookInterface.OnDestroyBuilding(this);
							   }
							   
							   // _/!\_ => Break <= (else break Ok)
							   sHookInterface.OnDamageBuilding(this);
		                       break;
	}

	Log.Warning("TakeDamage","%u : Health: %u / %u (Pct: %u%% detruit, Flag: 0x%08X).",pInfo->ID,
		Health,HealthMax,HealthPct,GetUInt32Value(GAMEOBJECT_FLAGS));
	
	/* Ascent
	if(HasFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED) && !HasFlag(GAMEOBJECT_FLAGS,GO_FLAG_DESTROYED))
	{
		if(Health <= 0)
		{
			RemoveFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED);
			SetFlag(GAMEOBJECT_FLAGS,GO_FLAG_DESTROYED);
			SetUInt32Value(GAMEOBJECT_DISPLAYID,pInfo->Unknown1);

			sHookInterface.OnDestroyBuilding(this);
		}
	}
	else if(!HasFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED) && !HasFlag(GAMEOBJECT_FLAGS,GO_FLAG_DESTROYED))
	{
		if(Health <= pInfo->sound5)
		{
			if(pInfo->Unknown1 == 0)
				Health = 0;
			else if(Health == 0)
				Health = 1;

			SetFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED);
			SetUInt32Value(GAMEOBJECT_DISPLAYID,pInfo->sound4);
			sHookInterface.OnDamageBuilding(this);
		}
	}
	*/
}

void GameObject::Damage()
{
	SetFlags(GO_FLAG_DAMAGED_20);
	if(pInfo->destructibleBuilding.DestructibleData != 0)
	{
		DestructibleModelDataEntry * display = dbcDestructibleModelDataEntry.LookupEntry( pInfo->destructibleBuilding.DestructibleData );
		SetDisplayId(display->GetDisplayId(1));
	}
	else
		SetDisplayId(pInfo->destructibleBuilding.DisplayId_50PCT);
}

void GameObject::Destroy()
{
	RemoveFlag(GAMEOBJECT_FLAGS,GO_FLAG_DAMAGED_20);
	SetFlags(GO_FLAG_DAMAGED_40);
	if(pInfo->destructibleBuilding.DestructibleData != 0)
	{
		DestructibleModelDataEntry * display = dbcDestructibleModelDataEntry.LookupEntry( pInfo->destructibleBuilding.DestructibleData );
		SetDisplayId(display->GetDisplayId(3));
	}
	else
		SetDisplayId(pInfo->destructibleBuilding.DisplayId_Detruit);
}

void GameObject::Rebuild()
{
	SetUInt32Value(GAMEOBJECT_DISPLAYID, pInfo->DisplayID);
	SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_DESPAWN); // au cas ou, et update du Go
	RemoveDamageFlag(GAMEOBJECT_FLAGS,_GAMEOBJECT_DESTRUCT_MASK,true);
	Health = HealthMax = pInfo->destructibleBuilding.NumHits_100PCT /*SpellFocus*/ + pInfo->destructibleBuilding.NumHits_50PCT; // sound5
	HealthMid = pInfo->destructibleBuilding.NumHits_50PCT; // sound5
}
