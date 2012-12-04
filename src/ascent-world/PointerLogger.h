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

#ifndef __POINTERLOGGER_H
#define __POINTERLOGGER_H

//#define POINTER_LOGGER

#ifdef POINTER_LOGGER

void PointerLog(void* pointer, bool type);


#define POINTERLOGNEW(x) PointerLog(x, 0);

#define POINTERLOGDELETE(x) PointerLog(x, 1);

#else

#define POINTERLOGNEW(x)
#define POINTERLOGDELETE(x)

#endif

#endif
