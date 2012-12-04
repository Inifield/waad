/*
 * Ascent MMORPG Server
 * Copyright (C) 2009 Waad Team <http://arbonne.games-rpg.net>
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
#ifndef _PVPZONES_
#define _PVPZONES_

#include "PvPZones/ZoneHellfirePeninsula.h"
#include "PvPZones/ZoneTerokkarForest.h"

enum PvPZone_BannerStatus
{
	PVPZONE_BANNER_STATUS_NEUTRAL = 0,
	PVPZONE_BANNER_STATUS_ALLIANCE = 1,
	PVPZONE_BANNER_STATUS_HORDE = 2,
};

struct PvPZone_sgodata
{
	uint32 entry;
	float posx;
	float posy;
	float posz;
	float facing;
	float orientation[4];
	uint32 state;
	uint32 flags;
	uint32 faction;
	float scale;
	uint32 is_banner;
};

void SetupPvPZones(MapMgr* mgr);

#endif
