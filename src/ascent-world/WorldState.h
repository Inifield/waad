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

#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H

class WorldState
{
public:
	HM_NAMESPACE::hash_map<uint32, uint32> m_states;

	void SetState(uint32 index, uint32 value);

	ASCENT_INLINE void EraseState(uint32 index) { m_states.erase(index); }
};
//-----------------------------------------------------------------------
// Les WorldStates dynamique sont dans la base
// Les WorldStates se terminant par (World) sont independants d'une map
//-----------------------------------------------------------------------
// Saisons d'arenes (World)
#define WS_SEASON_ARENA		3191 // (in_progress/ended)
#define WS_SEASON_NUMBER	3901
//-----------------------------------------------------------------------
// Goulet des Chanteguerres
#define WSG_ALLIANCE_FLAG_CAPTURED		 2338 // 0x0922
#define WSG_HORDE_FLAG_CAPTURED			 2339 // 0x0923
#define WSG_CURRENT_HORDE_SCORE			 1582 // 0x062E
#define WSG_CURRENT_ALLIANCE_SCORE		 1581 // 0x062D
#define WSG_MAX_SCORE					 1601 // 0x0641
//-----------------------------------------------------------------------
// Bassin d'Arathi
#define AB_ALLIANCE_RESOURCES			 1776 // 0x06F0
#define AB_HORDE_RESOURCES				 1777 // 0x06F1
#define AB_HORDE_CAPTUREBASE			 1778 // 0x06F2
#define AB_ALLIANCE_CAPTUREBASE			 1779 // 0x06F3
#define AB_MAX_SCORE					 1780 // 0x06F4
//-----------------------------------------------------------------------
// Vallée d'alterac
#define AV_UNCONTROLED_SNOWFALL_GRAVE			1966 // 0x7AE //1 -> show uncontrolled
#define AV_CONTROLED_ICEBLOOD_TOWER_HORDE		1385 // 0x569 //1 -> horde controled
#define AV_CONTROLED_TOWER_POINT_HORDE			1384 // 0x568 //1 -> horde controled
#define AV_CONTROLED_FROSTWOLF_RELIFHUNT_HORDE	1330 // 0x532 //1 -> horde controled
#define AV_CONTROLED_EAST_FROSTWOLF_TOWER_HORDE 1383 // 0x567 //1 -> horde controled
#define AV_CONTROLED_WEST_FROSTWOLF_TOWER_HORDE 1382 // 0x566 //1 -> horde controled
#define AV_CONTROLED_ICEBLOOD_GRAVE_HORDE		1347 // 0x543 //1 -> horde controled
#define AV_CONTROLED_FROSTWOLF_GRAVE_HORDE		1338 // 0x53A //1 -> horde controled

#define AV_CONTROLED_IRONDEEP_MINE_TROGG		1360 // 0x550 //1 -> trogg controled
#define AV_CONTROLED_COLDTHOOT_MINE_KOBOLT		1357 // 0x54D //1 -> kobolt controled

#define AV_CONTROLED_STORMPIKE_GRAVE_ALLIANCE		 1333 // 0x535 //1 -> alliance controled
#define AV_CONTROLED_STONEHEART_BUNKER_ALLIANCE		 1364 // 0x554 //1 -> alliance controled
#define AV_CONTROLED_ICEWING_BUNKER_ALLIANCE		 1363 // 0x553 //1 -> alliance controled
#define AV_CONTROLED_DUBALDER_NORTH_BUNKER_ALLIANCE  1362 // 0x552 //1 -> alliance controled
#define AV_CONTROLED_DUBALDER_SOUTH_BUNKER_ALLIANCE  1361 // 0x551 //1 -> alliance controled
#define AV_CONTROLED_STORMPIKE_AID_STATION_ALLIANCE  1325 // 0x52D //1 -> alliance controled
#define AV_CONTROLED_STONEHEART_GRAVE_ALLIANCE		 1302 // 0x516 //1 -> alliance controled
//-----------------------------------------------------------------------
// Oeil du cyclone
#define EOTS_WORLDSTATE_DISPLAYON				2718
#define EOTS_WORLDSTATE_DISPLAYVALUE			2719
#define EOTS_WORLDSTATE_ALLIANCE_VICTORYPOINTS  2749
#define EOTS_WORLDSTATE_HORDE_VICTORYPOINTS		2750
#define EOTS_WORLDSTATE_ALLIANCE_BASES			2752
#define EOTS_WORLDSTATE_HORDE_BASES				2753
#define EOTS_NETHERWING_FLAG_SPELL				34976
//-----------------------------------------------------------------------
// Rivage des Anciens
#define SOTA_CAPTURE_BAR_DISPLAY 3536
#define SOTA_CAPTURE_BAR_VALUE	 3537
#define SOTA_ALLIANCE_ATTACKER	 3564
#define SOTA_HORDE_DEFENDER		 3564
#define SOTA_ALLIANCE_DEFENDER	 3565
#define SOTA_HORDE_ATTACKER		 3565
#define SOTA_BONUS_TIME			 3571
#define SOTA_TIMER_1			 3559 //first digit
#define SOTA_TIMER_2			 3560 //second digit
#define SOTA_TIMER_3			 3561 //third digit
//-----------------------------------------------------------------------
#endif
