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
#include "EventableObject.h"

EventableObject::~EventableObject()
{
	/* decrement event count on all events */

	EventMap::iterator itr = m_events.begin();
	for(; itr != m_events.end(); ++itr)
	{
		if( itr->second->eventFlag & EVENT_FLAG_FIRE_ON_DELETE && !itr->second->deleted )
		{
			itr->second->deleted = true;
			itr->second->cb->execute();
		}
		else
			itr->second->deleted = true; // Flag de delete du Callback mais le 'TimedEvent' n'est plus detruit

		itr->second->DecRef();
		if(itr->second->resultDecRef == false)       // Destruction du Callback seulement (plus de 'delete this' ds le DecRef )
		{
			Log.Error("[EventableObject]","Destructor : Suppression Event %d erreur !!",itr->second->eventType);
			Log.Error("[EventableObject]","           : Report this to devs plz.!");
		}
		else
			delete itr->second;          // On detruit le 'TimedEvent' associ� ( seulement dans le destructor de l'EventableObject )
	}

	m_events.clear();
}

EventableObject::EventableObject()
{
	/* commented, these will be allocated when the first event is added. */
	//m_event_Instanceid = event_GetInstanceID();
	//m_holder = sEventMgr.GetEventHolder(m_event_Instanceid);

	m_holder = 0;
	m_event_Instanceid = -1;
	m_events.clear();
}

void EventableObject::event_AddEvent(TimedEvent * ptr)
{
	m_lock.Acquire();

	if(!m_holder)
	{
		m_event_Instanceid = event_GetInstanceID();
		m_holder = sEventMgr.GetEventHolder(m_event_Instanceid);
	}

	if(m_event_Instanceid == WORLD_INSTANCE && (ptr->eventFlag & EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT))
	{
		delete ptr->cb;
		delete ptr;

		m_lock.Release();
		return;
	}

	ptr->IncRef();

	if(ptr->ref >1) //Note Randdrick : On interdit les empilements des events !!
	{
		ptr->DecRef();
		delete ptr->cb;
		delete ptr;

		m_lock.Release();
		return;
	}

	ptr->instanceId = m_event_Instanceid;
	pair<uint32,TimedEvent*> p(ptr->eventType, ptr);
	m_events.insert( p );
	m_lock.Release();

	/* Add to event manager */
	if(!m_holder || ptr->eventFlag & EVENT_FLAG_MOVE_TO_WORLD_CONTEXT)
	{
		/* relocate to -1 eventholder :/ */
		m_event_Instanceid = -1;
		m_holder = sEventMgr.GetEventHolder(m_event_Instanceid);
		ASSERT(m_holder);
	}

	m_holder->AddEvent(ptr);
}

void EventableObject::event_RemoveByPointer(TimedEvent * ev)
{
	m_lock.Acquire();
	EventMap::iterator itr = m_events.find(ev->eventType);
	EventMap::iterator it2;
	if(itr != m_events.end())
	{
		do 
		{
			it2 = itr++;

			if(it2->second == ev)
			{
				if( it2->second->eventFlag & EVENT_FLAG_FIRE_ON_DELETE && !it2->second->deleted )
				{
					it2->second->deleted = true;
					it2->second->cb->execute();
				}
				else
					it2->second->deleted = true;

				it2->second->DecRef();
				if(it2->second->resultDecRef == true) 
					delete it2->second;
				m_events.erase(it2);
				m_lock.Release();
				return;
			}

		} while(itr != m_events.upper_bound(ev->eventType));
	}
	m_lock.Release();
}

void EventableObject::event_PauseEvents(uint32 EventType)
{
	m_lock.Acquire();
	if(!m_events.size())
	{
		m_lock.Release();
		return;
	}

	if(EventType == EVENT_REMOVAL_FLAG_ALL)
	{
		return; //whos pausing all events? LOL
	}
	else
	{
		EventMap::iterator itr = m_events.find(EventType);
		EventMap::iterator it2;
		if(itr != m_events.end())
		{
			do 
			{
				it2 = itr++;

				it2->second->paused++;
			} while(itr != m_events.upper_bound(EventType));
		}
	}

	m_lock.Release();
}

void EventableObject::event_ResumeEvents(uint32 EventType)
{
	m_lock.Acquire();
	if(!m_events.size())
	{
		m_lock.Release();
		return;
	}

	if(EventType == EVENT_REMOVAL_FLAG_ALL)
	{
		return; //whos pausing all events? LOL
	}
	else
	{
		EventMap::iterator itr = m_events.find(EventType);
		EventMap::iterator it2;
		if(itr != m_events.end())
		{
			do 
			{
				it2 = itr++;
				it2->second->paused--;
			} while(itr != m_events.upper_bound(EventType));
		}
	}

	m_lock.Release();
}
#define WATCHDOG_INSTANCE	30 // en Sec, on affinera plus tard (Branruz)
void EventableObject::event_RemoveEvents(uint32 EventType)
{
	time_t Debut_Shutdown,ShutDownEnCours;
    time(&Debut_Shutdown);


	m_lock.Acquire();
	if(!m_events.size())
	{
		m_lock.Release();
		return;
	}

	if(EventType == EVENT_REMOVAL_FLAG_ALL)
	{
		EventMap::iterator itr = m_events.begin();
		for(; itr != m_events.end(); ++itr)
		{
			if( itr->second->eventFlag & EVENT_FLAG_FIRE_ON_DELETE && !itr->second->deleted )
			{
				itr->second->deleted = true;
				itr->second->cb->execute();
			}
			else
				itr->second->deleted = true;

			itr->second->DecRef();
			if(itr->second->resultDecRef == true) 
				delete itr->second;
		}
		m_events.clear();
	}
	else
	{
		EventMap::iterator itr = m_events.find(EventType);
		EventMap::iterator it2;
		if(itr != m_events.end())
		{
			do 
			{
				it2 = itr++;

				if( it2->second->eventFlag & EVENT_FLAG_FIRE_ON_DELETE && !it2->second->deleted )
				{
					it2->second->deleted = true;
					it2->second->cb->execute();
				}
				else
					it2->second->deleted = true;

				it2->second->DecRef();
				if(it2->second->resultDecRef == true) 
				{
					delete it2->second;
					Log.Debug("[EventableObject]","RemoveEvents it2 : Suppression de l'Event %d",it2->second->eventType);
				}
				m_events.erase(it2);

                time(&ShutDownEnCours);
				if( (ShutDownEnCours - Debut_Shutdown) > WATCHDOG_INSTANCE) 
				{
					Log.Error("Remove Event","L'event n'existe pas!!! (Type: %u)",EventType);
					break; // On sors, le thread aurait largement eu le temps d'etre tu�
				}

			} while(itr != m_events.upper_bound(EventType)); // Mise en place d'un WatchDog (Branruz)
		}
	}

	m_lock.Release();
}

void EventableObject::event_RemoveEvents()
{
	event_RemoveEvents(EVENT_REMOVAL_FLAG_ALL);
}

void EventableObject::event_ModifyTimeLeft(uint32 EventType, uint32 TimeLeft,bool unconditioned)
{
	m_lock.Acquire();
	if(!m_events.size())
	{
		m_lock.Release();
		return;
	}

	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
	{
		do 
		{
			if(unconditioned)
				itr->second->currTime = TimeLeft;
			else itr->second->currTime = ((int32)TimeLeft > itr->second->msTime) ? itr->second->msTime : (int32)TimeLeft;
			++itr;
		} while(itr != m_events.upper_bound(EventType));
	}

	m_lock.Release();
}

bool EventableObject::event_GetTimeLeft(uint32 EventType, uint32 * Time)
{
	m_lock.Acquire();
	if(!m_events.size())
	{
		m_lock.Release();
		return false;
	}

	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
	{
		do 
		{
			if( itr->second->deleted )
			{
				++itr;
				continue;
			}

			*Time = (uint32)itr->second->currTime;
			m_lock.Release();
			return true;

		} while(itr != m_events.upper_bound(EventType));
	}

	m_lock.Release();
	return false;
}

void EventableObject::event_ModifyTime(uint32 EventType, uint32 Time)
{
	m_lock.Acquire();
	if(!m_events.size())
	{
		m_lock.Release();
		return;
	}

	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
	{
		do 
		{
			itr->second->msTime = Time;
			++itr;
		} while(itr != m_events.upper_bound(EventType));
	}

	m_lock.Release();
}

void EventableObject::event_ModifyTimeAndTimeLeft(uint32 EventType, uint32 Time)
{
	m_lock.Acquire();
	if(!m_events.size())
	{
		m_lock.Release();
		return;
	}

	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
	{
		do 
		{
			itr->second->currTime = itr->second->msTime = Time;
			++itr;
		} while(itr != m_events.upper_bound(EventType));
	}

	m_lock.Release();
}


bool EventableObject::event_HasEvent(uint32 EventType)
{
	bool ret = false;
	m_lock.Acquire();
	if(!m_events.size())
	{
		m_lock.Release();
		return false;
	}

	//ret = m_events.find(EventType) == m_events.end() ? false : true;
	// Y'a un probleme de synchro avec l'event, je comprend pas pourquoi on a un plantage ici 
	// Il faut trouver un moyen de tester sur le m_events (Brz)
	EventMap::iterator itr = m_events.find(EventType);
	
	if(itr != m_events.end())
	{
		do 
		{
			if(!itr->second->deleted)
			{
				ret = true;
				break;
			}
			++itr;
		} while(itr != m_events.upper_bound(EventType));
	}

	m_lock.Release();
	return ret;
}

EventableObjectHolder::EventableObjectHolder(int32 instance_id) : mInstanceId(instance_id)
{
	m_insertPool.clear();
	sEventMgr.AddEventHolder(this, instance_id);
}

EventableObjectHolder::~EventableObjectHolder()
{
	sEventMgr.RemoveEventHolder(this);

	/* decrement events reference count */
	m_lock.Acquire();
	EventList::iterator itr = m_events.begin();
	for(; itr != m_events.end(); ++itr)
	{
		(*itr)->DecRef();
		if((*itr)->resultDecRef == true) 
			delete (*itr);
		else 
		{
			Log.Error("[EventableObjectHolder]","Destructor : Suppression Event %d erreur !!",(*itr)->eventType);
			Log.Error("[EventableObjectHolder]","           : Report this to devs plz.!");
		}
	}
	m_lock.Release();
}

void EventableObjectHolder::Update(uint32 time_difference)
{
	m_lock.Acquire();			// <<<<
    //---------------------------------------------------------------
	// Gestion du Spool d'Event (Event en attente d'insertion) 
	//---------------------------------------------------------------
	m_insertPoolLock.Acquire();
	TimedEventQueue::iterator iqi;
	TimedEventQueue::iterator iq2 = m_insertPool.begin();
	while(iq2 != m_insertPool.end())
	{
		iqi = iq2++;
		if((*iqi)->deleted || (*iqi)->instanceId != mInstanceId)
		{
			(*iqi)->DecRef();
			if((*iqi)->resultDecRef == true) 
				delete (*iqi);
		}
		else
			m_events.push_back( (*iqi) );

		m_insertPool.erase(iqi);
	}
	m_insertPoolLock.Release();
    //---------------------------------------------------------------
	// Gestion de la pile des Events courant
	//---------------------------------------------------------------
	/* Now we can proceed normally. */
	EventList::iterator itr = m_events.begin();
	EventList::iterator it2;
	TimedEvent * ev;

	while(itr != m_events.end())
	{
		it2 = itr++;

		if((*it2)->instanceId != mInstanceId || (*it2)->deleted || 
			( mInstanceId == WORLD_INSTANCE && (*it2)->eventFlag & EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT))
		{
			// remove from this list.
			(*it2)->DecRef();
			if((*it2)->resultDecRef == true) 
				delete (*it2);

			m_events.erase(it2);
			continue;
		}

		/* A FINIR: faire la difference entre une class EventObject et une class Player

		void* obj = (*it2)->obj;
		if (obj->event_GetInstanceID() != mInstanceId || 
		   (mInstanceId == WORLD_INSTANCE && ((*it2)->eventFlag & EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT)))
		{
			//either relocate, or continue and try to relocate later!
			uint32 tmpid = obj->event_GetInstanceID();

			if (tmpid != mInstanceId) //cant relocate to same instance, were locked :)
			{
				EventableObjectHolder * nh = sEventMgr.GetEventHolder(tmpid);
				if (nh != NULL)
				{
					nh->AddObject(obj);
					//all events in this context are now invalid
					continue;
				}
			}

			//we can't execute this anymore, won't be safe, so blow it up! rofl
			continue;
		}*/
        
		if ((*it2)->paused > 0) // Event en pause, pas de scan, pas d'execution, on passe � l'event suivant
			continue;
        
		// Event Update Procedure
		ev = *it2;

		if((uint32)ev->currTime <= time_difference)
		{
			// execute the callback
			if(ev->eventFlag & EVENT_FLAG_DELETES_OBJECT)
			{
				ev->deleted = true;
				ev->cb->execute();
				m_events.erase(it2);
				ev->DecRef();
				if(ev->resultDecRef == true) 
				{
					Log.Debug("[EventableObjectHolder]","EVENT_FLAG_DELETES_OBJECT : L'Event %d a bien ete supprime !!",ev->eventType);
					delete ev;
				}
				continue;
			}
			else
			{
				ev->cb->execute(); // Execution du CallBack
			}

			// check if the event is expired now.
			if(ev->repeats && --ev->repeats == 0)
			{
				// Event expired :>

				/* remove the event from here */
				ev->deleted = true;
				ev->DecRef();
				if(ev->resultDecRef == true) 
				{
					Log.Debug("[EventableObjectHolder]","Expiration de l'event : L'Event %d a bien ete supprime !!",ev->eventType);
					delete ev;
				}				
				m_events.erase(it2);

				continue;
			}
			
			else if(ev->deleted)
			{
				// event is now deleted
				ev->DecRef();
				if(ev->resultDecRef == true) // A ce stade, le callback est forcement detruit => ev->Deleted = True => l'Event � expir�
				{
					Log.Debug("[EventableObjectHolder]","Suppresion tardive de l'Event %d!!",ev->eventType);
					delete ev;	
				}

				m_events.erase(it2);
				continue;
			}
			// event has to repeat again, reset the timer
			ev->currTime = ev->msTime;
		}
		else
		{
			// event is still "waiting", subtract time difference
			ev->currTime -= time_difference;
		}
	}

	m_lock.Release();
}

void EventableObject::event_Relocate()
{
	/* prevent any new stuff from getting added */
	m_lock.Acquire();

	EventableObjectHolder * nh = sEventMgr.GetEventHolder(event_GetInstanceID());
	if(nh != m_holder)
	{
		// whee, we changed event holder :>
		// doing this will change the instanceid on all the events, as well as add to the new holder.
		
		// no need to do this if we don't have any events, though.
		if(!nh)
			nh = sEventMgr.GetEventHolder(-1);

		nh->AddObject(this);

		// reset our m_holder pointer and instance id
		m_event_Instanceid = nh->GetInstanceID();
		m_holder = nh;
	}

	/* safe again to add */
	m_lock.Release();
}

uint32 EventableObject::event_GetEventPeriod(uint32 EventType)
{
	uint32 ret = 0;
	m_lock.Acquire();
	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
		ret = (uint32)itr->second->msTime;
	
	m_lock.Release();
	return ret;
}

void EventableObjectHolder::AddEvent(TimedEvent * ev)
{
	// m_lock NEEDS TO BE A RECURSIVE MUTEX
	ev->IncRef();
	if(!m_lock.AttemptAcquire())
	{
		m_insertPoolLock.Acquire();
		m_insertPool.push_back( ev );
		m_insertPoolLock.Release();
	}
	else
	{
		m_events.push_back( ev );
		m_lock.Release();
	}
}

void EventableObjectHolder::AddObject(EventableObject * obj)
{
	// transfer all of this objects events into our holder
	if(!m_lock.AttemptAcquire())
	{
		// The other thread is obviously occupied. We have to use an insert pool here, otherwise
		// if 2 threads relocate at once we'll hit a deadlock situation.
		m_insertPoolLock.Acquire();
		EventMap::iterator it2;

		for(EventMap::iterator itr = obj->m_events.begin(); itr != obj->m_events.end(); ++itr)
		{
			// ignore deleted events (shouldn't be any in here, actually)
			if(itr->second->deleted)
				continue;

			if(mInstanceId == WORLD_INSTANCE && itr->second->eventFlag & EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT)
				continue;

			itr->second->IncRef();
			itr->second->instanceId = mInstanceId;
			m_insertPool.push_back(itr->second);
		}

		// Release the insert pool.
		m_insertPoolLock.Release();

		// Ignore the rest of this stuff
		return;
	}

	for(EventMap::iterator itr = obj->m_events.begin(); itr != obj->m_events.end(); ++itr)
	{
		// ignore deleted events
		if(itr->second->deleted)
			continue;

		if(mInstanceId == WORLD_INSTANCE && itr->second->eventFlag & EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT)
			continue;

		itr->second->IncRef();
		itr->second->instanceId = mInstanceId;
		m_events.push_back( itr->second );
	}

	m_lock.Release();
}
