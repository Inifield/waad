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
 * Derivée du TimedEvent, le probleme de leak est lié directement à la destruction de la ref de l'aura.
 * Dans certain cas on ne detruit pas la ref car le player possede toujours l'aura visible mais qui n'agit plus
 * Dans d'autre cas (Aura à cooldown unique on va dire) l'aura devrait etre totalement detruite.
 * - Branruz - 2012 - 
 */

#include "StdAfx.h"

initialiseSingleton( EventOneTimeAuraMgr );

TimedEventOneAura * TimedEventOneAura::Allocate(void **object, CallbackBase* callback, uint32 flags, time_t time, uint32 repeat)
{
	return new TimedEventOneAura(object, callback, flags, time, repeat, 0);
}

