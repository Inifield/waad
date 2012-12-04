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

//
// MapCell.cpp
//
#include "StdAfx.h"

extern bool bServerShutdown;

MapCell::~MapCell()
{
	RemoveObjects();
}

void MapCell::Init(uint32 x, uint32 y, uint32 mapid, MapMgr* mapmgr)
{
	_mapmgr = mapmgr;
	_active = false;
	_loaded = false;
	_activityCount = 0;
	_corpses.clear();
	_x = static_cast<uint16>(x);
	_y = static_cast<uint16>(y);
	_unloadpending=false;
	_objects.clear();
	objects_iterator = _objects.begin();
}

void MapCell::AddObject(Object* obj)
{
	if(obj->IsPlayer())
		++_activityCount;
	else if(obj->IsCorpse())
	{
		_corpses.push_back(obj);
		if(_unloadpending)
			CancelPendingUnload();
	}

	_objects.insert(obj);
}

void MapCell::RemoveObject(Object* obj)
{
	if(obj->IsPlayer())
		--_activityCount;
	else if(obj->IsCorpse())
		_corpses.remove(obj);

	if(objects_iterator != _objects.end() && (*objects_iterator) == obj)
		++objects_iterator;

	_objects.erase(obj);
}

void MapCell::SetActivity(bool state)
{	
	if(!_active && state)
	{
		// Move all objects to active set.
		for(ObjectSet::iterator itr = _objects.begin(); itr != _objects.end(); ++itr)
		{
			if(!(*itr)->Active && (*itr)->CanActivate())
				(*itr)->Activate(_mapmgr);
		}

		if(_unloadpending)
			CancelPendingUnload();

#ifdef COLLISION
		CollideInterface.ActivateTile(_mapmgr->GetMapId(), _x/8, _y/8);
#endif

	} else if(_active && !state)
	{
		// Move all objects from active set.
		for(ObjectSet::iterator itr = _objects.begin(); itr != _objects.end(); ++itr)
		{
			if((*itr)->Active)
				(*itr)->Deactivate(_mapmgr);
		}

		if(sWorld.map_unload_time && !_unloadpending)
			QueueUnloadPending();

#ifdef COLLISION
		CollideInterface.DeactivateTile(_mapmgr->GetMapId(), _x/8, _y/8);
#endif
	}

	_active = state; 

}
void MapCell::RemoveObjects()
{
	ObjectSet::iterator itr;
	uint32 count = 0;
	//uint32 ltime = getMSTime();

	//Zack : we are delaying cell removal so transports can see objects far away. We are waiting for the event to remove us
	if(_unloadpending == true)
		return;

	/* delete objects in pending respawn state */
	for(itr = _respawnObjects.begin(); itr != _respawnObjects.end(); ++itr)
	{
		switch((*itr)->GetTypeId())
		{
		case TYPEID_UNIT:
			{
				if( (*itr)->IsVehicle() )
				{
					_mapmgr->_reusable_guids_vehicle.push_back( (*itr)->GetUIdFromGUID() );
					static_cast< Vehicle* >(*itr)->m_respawnCell=NULL;
					delete static_cast< Vehicle* >(*itr);
				}
				else if( !(*itr)->IsPet() )
				{
					_mapmgr->_reusable_guids_creature.push_back( (*itr)->GetUIdFromGUID() );
					static_cast< Creature* >(*itr)->m_respawnCell=NULL;
					delete static_cast< Creature* >(*itr);
				}
			}break;

		case TYPEID_GAMEOBJECT:
			{
				_mapmgr->_reusable_guids_gameobject.push_back( (*itr)->GetUIdFromGUID() );
				static_cast< GameObject* >(*itr)->m_respawnCell=NULL;
				delete static_cast< GameObject* >(*itr);
			}break;
		}
	}
	_respawnObjects.clear();

	//This time it's simpler! We just remove everything :)
	Object* obj; //do this outside the loop!
	for(itr = _objects.begin(); itr != _objects.end();)
	{
		obj = (*itr);
		itr++;

		if(!obj)
		{
			_objects.erase(itr);
			continue;
		}

		if( _unloadpending )
		{
			if(obj->GetTypeFromGUID() == HIGHGUID_TYPE_TRANSPORTER)
				continue;

			if(obj->GetTypeId()==TYPEID_CORPSE && obj->GetUInt32Value(CORPSE_FIELD_OWNER) != 0)
				continue;

			if(!obj->m_loadedFromDB)
				continue;
		}

		//If MapUnloadTime is non-zero, a transport could get deleted here (when it arrives to a cell that's scheduled to be unloaded because players left from it), so don't delete it! - By: VLack aka. VLsoft
		if( sWorld.map_unload_time && !bServerShutdown && obj->GetTypeId() == TYPEID_GAMEOBJECT && static_cast<GameObject*>(obj)->GetInfo()->Type == GAMEOBJECT_TYPE_TRANSPORT )
			continue;


		if( obj->Active )
			obj->Deactivate( _mapmgr );

		if( obj->IsInWorld())
			obj->RemoveFromWorld( true );

		delete obj;
		obj = NULL;
	}
	_objects.clear();
	_corpses.clear();
	_activityCount = 0;
	_loaded = false;
}


void MapCell::LoadObjects(CellSpawns* sp)
{
	//we still have mobs loaded on cell. There is no point of loading them again
	if(_loaded == true)
		return;

	_loaded = true;
	Instance * pInstance = _mapmgr->pInstance;

	if(sp->CreatureSpawns.size())//got creatures
	{
		Vehicle* v = new Vehicle(NULL);
		Creature* c = new Creature(NULL);

		for(CreatureSpawnList::iterator i=sp->CreatureSpawns.begin();i!=sp->CreatureSpawns.end();i++)
		{
			if(pInstance)
			{
				if(pInstance->m_killedNpcs.find((*i)->id) != pInstance->m_killedNpcs.end())
					continue;

/*				if((*i)->respawnNpcLink && pInstance->m_killedNpcs.find((*i)->respawnNpcLink) != pInstance->m_killedNpcs.end())
					continue;*/
			}
			if((*i)->vehicle != 0)
			{
				v =_mapmgr->CreateVehicle((*i)->entry);
				if(v == NULL)
					continue;

				v->SetMapId(_mapmgr->GetMapId());
				v->SetInstanceID(_mapmgr->GetInstanceID());
				v->m_loadedFromDB = true;

			if(v->Load(*i, _mapmgr->iInstanceMode, _mapmgr->GetMapInfo()))
				{
					if(!v->CanAddToWorld())
					{
						delete v;
						v = NULL;
						continue;
					}

					v->PushToWorld(_mapmgr);
				}
				else
				{
					Log.Warning("[MapCell::LoadObjects]","Le vehicule %u n'existe pas! (CreatureSpawn Id: %u)",
						           (*i)->vehicle,(*i)->entry);
					delete v;
					v = NULL;
				}
			}
			else
			{
				c=_mapmgr->CreateCreature((*i)->entry);
				if(c == NULL)
					continue;
	
				c->SetMapId(_mapmgr->GetMapId());
				c->SetInstanceID(_mapmgr->GetInstanceID());
				c->m_loadedFromDB = true;

				if(c->Load(*i, _mapmgr->iInstanceMode, _mapmgr->GetMapInfo()))
				{
					if(!c->CanAddToWorld())
					{
						delete c;
						c = NULL;
						continue;
					}

					c->PushToWorld(_mapmgr);
				}
				else
				{
					delete c;
					c = NULL;
				}
			}
		}
	}

	if(sp->GOSpawns.size())//got GOs
	{
		GameObject* go = new GameObject(NULL);
		for(GOSpawnList::iterator i=sp->GOSpawns.begin();i!=sp->GOSpawns.end();i++)
		{
			go = _mapmgr->CreateGameObject((*i)->entry);
			if(go == NULL)
				continue;
				
			if(go->Load(*i))
			{
				go->m_loadedFromDB = true;
				go->PushToWorld(_mapmgr);
				CALL_GO_SCRIPT_EVENT(go, OnSpawn)();
			}
			else
			{
				delete go;
				go = NULL;
			}
		}
	}
}


void MapCell::QueueUnloadPending()
{
	if(_unloadpending)
		return;

	_unloadpending = true;
	//Log.Debug("MapCell", "Queueing pending unload of cell %u %u", _x, _y);
	sEventMgr.AddEvent(_mapmgr, &MapMgr::UnloadCell, (uint32)_x, (uint32)_y, MAKE_CELL_EVENT(_x, _y), sWorld.map_unload_time * 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void MapCell::CancelPendingUnload()
{
	//Log.Debug("MapCell", "Cancelling pending unload of cell %u %u", _x, _y);
	if(!_unloadpending)
		return;

	sEventMgr.RemoveEvents(_mapmgr, MAKE_CELL_EVENT(_x, _y));
	_unloadpending = false;
}

void MapCell::Unload()
{
	Log.Debug("MapCell", "Unloading cell %u %u", _x, _y);
	ASSERT(_unloadpending);
	if(_active)
	{
		_unloadpending = false;
		return;
	}

	_unloadpending = false;

	//in ~MapCell RemoveObjects() can delete an Object without removing it from the MapCell.cpp
	//Example:
	//Creature A has guardian B. MapCell is unloaded, _mapmgr->Remove(_x, _y) is called, nullifying the reference to the cell
	//in CellHandler. ~MapCell is called, RemoveObjects() is called and despawns A which despawns B, calling Object::RemoveFromWorld()
	//which calls MapMgr::RemoveObject(B) which calls cell->RemoveObject(obj) ONLY if cell is not NULL, but in this case is NULL, leaving
	//a reference to a deleted Object in MapCell::_objects, iterated in RemoveObjects(). Calling it here fixes this issue.
	//Note: RemoveObjects() is still called in ~MapCell, due to fancy ArcEmu behaviors, like the in-game command ".mapcell delete <x> <y>

	RemoveObjects();

	_mapmgr->Remove(_x, _y);
}

void MapCell::CorpseGoneIdle(Object* corpse)
{
	_corpses.remove(corpse);
	CheckUnload();
}

void MapCell::CheckUnload()
{
	if(!_active && !_unloadpending && CanUnload())
		QueueUnloadPending();
}

bool MapCell::CanUnload()
{
	if(_corpses.size() == 0 && _mapmgr->m_battleground == NULL)
		return true;
	else
		return false;
}