/*
 * AscentWaad MMORPG Server
 * Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
 * Copyright (C) 2009-2012 WaadAscent Team <http://arbonne.games-rpg.net>
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
 * Derivée du TimedEvent, le probleme de leak est lié directement a la destruction de la ref de l'aura.
 * Dans certain cas on ne detruit pas la ref car le player possede toujours l'aura visible mais qui n'agit plus
 * Dans d'autre cas (Aura à cooldown unique on va dire) l'aura devrait etre totalement detruite.
 * - Branruz - 2012 - 
 */

#ifndef __EVENTONETIMEAURAMGR_H
#define __EVENTONETIMEAURAMGR_H

//#define EVENT_REMOVAL_FLAG_ALL 0xFFFFFFFF // Je deplace le define de l'eventobject.h ici (Branruz)

class EventableObject;

struct SERVER_DECL TimedEventOneAura
{
	TimedEventOneAura(void **object, CallbackBase* callback, uint32 type, time_t time, uint32 flags) :
		obj(object), cb(callback), eventType(type), eventFlag(flags), currTime(time),deleted(false),paused(false),instanceId(0), ref(0) { }
		
	void **obj;
	CallbackBase *cb;
	uint32 eventType;
	uint16 eventFlag;
    time_t msTime;
	time_t currTime;
	bool deleted;
	uint32 paused;
	int instanceId;
	volatile long ref;

	static TimedEventOneAura * Allocate(void **object, CallbackBase* callback, uint32 flags, time_t time);

#ifdef WIN32
	void DecRef()
	{
		InterlockedDecrement(&ref); 
		if(ref <= 0)
		{
			delete cb; 
		    delete (*obj);  
			(*obj)=NULL;
			delete this;
		}
	}

	void IncRef() { InterlockedIncrement(&ref); } 
#else

	/* burlex: if anyone knows how to do the equivilent of InterlockedIncrement/Decrement on linux feel free
	   to change this, I couldn't find the atomic functions anymore though :*( */

	void IncRef() { ++ref; }
    
	void DecRef()
	{
		--ref;
		if(ref <= 0)
		{
			 delete cb; 
			 delete (*obj);  
			 (*obj)=NULL;
			 delete this; 
		}
	}

#endif

};

class EventMgr;
class EventableObjectHolder;
//typedef map<int32, EventableObjectHolder*> HolderOneAuraMap;

class SERVER_DECL EventOneTimeAuraMgr : public Singleton < EventOneTimeAuraMgr >
{
	friend class MiniEventMgr;
public:
	template <class Class>
		void PauseEvents(Class *obj, uint32 type)
	{
		obj->event_PauseEvents(type);
	}

	template <class Class>
		void ResumeEvents(Class *obj, uint32 type)
	{
		obj->event_ResumeEvents(type);
	}

	template <class Class>
		void AddEvent(Class **obj, void (Class::*method)(), uint32 type, time_t time, uint32 flags)
	{
		// create a timed event
		TimedEventOneAura * pEvent = new TimedEventOneAura(obj, new CallbackP0<Class>(obj, method), type, time, flags);

		// add this to the object's list, updating will all be done later on...
		*obj->event_AddEvent(pEvent);
	}

	template <class Class, typename P1>
		void AddEvent(Class **obj, void (Class::*method)(P1), P1 p1, uint32 type, time_t time, uint32 flags)
	{
		// create a timed event
		TimedEventOneAura * pEvent = new TimedEventOneAura(obj, new CallbackP1<Class, P1>(obj, method, p1), type, time, flags);

		// add this to the object's list, updating will all be done later on...
		*obj->event_AddEvent(pEvent);
	}

	template <class Class, typename P1, typename P2>
	void AddEvent(Class **obj, void (Class::*method)(P1,P2), P1 p1, P2 p2, uint32 type, time_t time, uint32 flags)
	{
		// create a timed event
		TimedEventOneAura * pEvent = new TimedEventOneAura(obj, new CallbackP2<Class, P1, P2>(obj, method, p1, p2), type, time,  flags);

		// add this to the object's list, updating will all be done later on...
		*obj->event_AddEvent(pEvent);
	}

	template <class Class, typename P1, typename P2, typename P3>
		void AddEvent(Class **obj,void (Class::*method)(P1,P2,P3), P1 p1, P2 p2, P3 p3, uint32 type, time_t time, uint32 flags)
	{
		// create a timed event
		TimedEventOneAura * pEvent = new TimedEventOneAura(obj, new CallbackP3<Class, P1, P2, P3>(obj, method, p1, p2, p3), type, time, flags);

		// add this to the object's list, updating will all be done later on...
		*obj->event_AddEvent(pEvent);
	}

	template <class Class, typename P1, typename P2, typename P3, typename P4>
		void AddEvent(Class **obj, void (Class::*method)(P1,P2,P3,P4), P1 p1, P2 p2, P3 p3, P4 p4, uint32 type, time_t time, uint32 flags)
	{
		// create a timed event
		TimedEventOneAura * pEvent = new TimedEventOneAura(obj, new CallbackP4<Class, P1, P2, P3, P4>(obj, method, p1, p2, p3, p4), type, time, flags);

		// add this to the object's list, updating will all be done later on...
		*obj->event_AddEvent(pEvent);
	}

	template <class Class, typename P1, typename P2, typename P3, typename P4, typename P5>
		void AddEvent(Class **obj, void (Class::*method)(P1,P2,P3,P4,P5), P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, uint32 type, time_t time,  uint32 flags)
	{
		// create a timed event
		TimedEventOneAura * pEvent = new TimedEventOneAura(obj, new CallbackP5<Class, P1, P2, P3, P4, P5>(obj, method, p1, p2, p3, p4, p5), type, time,flags);

		// add this to the object's list, updating will all be done later on...
		*obj->event_AddEvent(pEvent);
	}

	template <class Class> void ModifyEventTimeLeft(Class *obj, uint32 type, uint32 time,bool unconditioned=true)
	{
		obj->event_ModifyTimeLeft(type, time,unconditioned);
	}

	template <class Class> void ModifyEventTimeAndTimeLeft(Class *obj, uint32 type, uint32 time)
	{
		obj->event_ModifyTimeAndTimeLeft(type, time);
	}

	template <class Class> void ModifyEventTime(Class *obj, uint32 type, uint32 time)
	{
		obj->event_ModifyTime(type, time);
	}

	template <class Class> bool HasEvent(Class *obj, uint32 type)
	{
		return (obj->event_HasEvent(type));
	}

	EventableObjectHolder * GetEventHolder(int32 InstanceId)
	{
		HolderMap::iterator itr = mHolders.find(InstanceId);
		if(itr == mHolders.end()) return 0;
		return itr->second;
	}

	void AddEventHolder(EventableObjectHolder * holder, int32 InstanceId)
	{
		holderLock.Acquire();
		mHolders.insert( HolderMap::value_type( InstanceId, holder) );
		holderLock.Release();
	}

	void RemoveEventHolder(int32 InstanceId)
	{
		holderLock.Acquire();
		mHolders.erase(InstanceId);
		holderLock.Release();
	}

	void RemoveEventHolder(EventableObjectHolder * holder)
	{
		holderLock.Acquire();
		HolderMap::iterator itr = mHolders.begin();
		for(; itr != mHolders.end(); ++itr)
		{
			if(itr->second == holder)
			{
				mHolders.erase(itr);
				holderLock.Release();
				return;
			}
		}
		holderLock.Release();
	}

protected:
	HolderMap mHolders;
	Mutex holderLock;
};

#define sEventOneTimeAuraMgr EventOneTimeAuraMgr::getSingleton()

#endif
