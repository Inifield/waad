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

#ifdef POINTER_LOGGER

FILE* plog=fopen("PointerLog.txt", "w");
CStackWalker pwalker;

void PointerLog(void* pointer, bool type)
{
	if (plog != NULL)
	{
		uint64 v1, v2;
		v1 = c_GetTimerValue();
		pwalker.CallBack.clear();
		pwalker.ShowCallstack();
		v2 = c_GetTimerValue();
		fprintf(plog, "%s pointer caught (took %u nanoseconds to catch): %p\nCallStack: %s", type? "Deleted" : "New", c_GetNanoSeconds(v2, v1), pointer, pwalker.CallBack.c_str());
		fflush(plog);
	}
}

#endif
