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

#ifndef WOW_TIMER_H
#define WOW_TIMER_H
/*


Le getMSTime renvoi le nombre de mSec depuis que windows a demarré (limite: 49.7 jours)
*/

ASCENT_INLINE uint32 getMSTimeDiff(uint32 oldMSTime, uint32 newMSTime) // Mangos source
{
    // getMSTime() have limited data range and this is case when it overflow in this tick
    if (oldMSTime > newMSTime)
        return (0xFFFFFFFF - oldMSTime) + newMSTime;
    else
        return newMSTime - oldMSTime;
}

#ifdef WIN32
ASCENT_INLINE uint32 getMSTime() { return GetTickCount(); }

// Ajout/Test et remplacement (mais pas partout) par celui la si ca marche mieux (Branruz)

ASCENT_INLINE uint32 getMSTime2() 
{
	uint32 GetTickMs   = GetTickCount(); // en millseconde depuis le demarrage de windows
	time_t GetNormalTime = time(NULL); // en seconde depuis le 1 janvier 70 à 00:00:00
	return(((uint32)(GetNormalTime*1000))+(GetTickMs & 0x000003FF)); // bon c'est une bidouille...
}
#else
/* Memo
struct timeval (depuis le 1 janvier 70 a 00:00:00)
{
    time_t      tv_sec;     // seconds 
    suseconds_t tv_usec;    // microseconds 
};*/
ASCENT_INLINE uint32 getMSTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	// conversion en ms      1sec = 1000 msec et 1ms = 1000uS
	return ((tv.tv_sec * 1000) + ((uint32)((float)tv.tv_usec / 1000.)));
}

#endif

#endif


