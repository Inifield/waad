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

#include "StdAfx.h"

void SetupPvPZones(MapMgr* mgr)
{
	
	switch(mgr->GetMapId())
	{
	case   1 : Log.zPvP("Server","Initializing PvP Zones: Kalimdor...");
		        // SetupPvPSilithus(mgr);  
				Log.zPvP("Server","PvP Zones: Kalimdor initializing done.");
		        break;
		  
	case 530 : Log.zPvP("Server","Initializing PvP Zones: Outreterre...");
		        SetupPvP_HellfirePeninsula(mgr); 
                SetupPvP_TerokkarForest(mgr);
		        //SetupPvP_Zangarmarsh(mgr);
			    //SetupPvP_Halaa(mgr);
				Log.zPvP("Server","PvP Zones: Outreterre initializing done.");
		       break;
	 default: 

		 break;
	}  
	
}
