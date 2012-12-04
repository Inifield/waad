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

initialiseSingleton( EventMgr );

TimedEvent * TimedEvent::Allocate(void* object, CallbackBase* callback, uint32 flags, time_t time, uint32 repeat)
{
	return new TimedEvent(object, callback, flags, time, repeat, 0);
}
/*
void TimedEvent::IncRef()
{
#ifdef WIN32
	InterlockedIncrement(&ref);
#else
	// atomic patch (ascent r4934)
	__gnu_cxx::__atomic_add((volatile _Atomic_word*)&ref, 1);
#endif
}

void TimedEvent::DecRef()
{
#ifdef WIN32
	InterlockedDecrement(&ref);
#else
	// atomic patch (ascent r4934)
	__gnu_cxx::__atomic_add((volatile _Atomic_word*)&ref, -1);
#endif
	if(ref <= 0)
	{
		EventableObjectHolder* holder = sEventMgr.GetEventHolder(instanceId);
		if (holder == NULL)
		{
			delete cb;
			delete this;
		}
		else
		{
			holder->m_deletePoolLock.Acquire();
			holder->m_deletePool.push_back(this);
			holder->m_deletePoolLock.Release();
		}
	}
}
*/
