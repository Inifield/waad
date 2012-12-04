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

///////////////////////////////////////////////
//  Waypoint Commands
//

#include "StdAfx.h"

bool ChatHandler::HandleWPAddCommand(const char* args, WorldSession *m_session)
{
	

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		RedSystemMessage(m_session, "No Selection");
		return true;
	}
	AIInterface* ai = NULL;
	Creature * pCreature = NULL;
	Player* p = m_session->GetPlayer();
	if(p->waypointunit != NULL)
	{
		SystemMessage(m_session, "Using Previous Unit.");
		ai = p->waypointunit;
		if(!ai)
		{
			RedSystemMessage(m_session, "Creature sans waypoint!");
			return true;
		}

		pCreature = (Creature*)ai->GetUnit();
		if(!pCreature)
		{
			RedSystemMessage(m_session, "Creature invalide!");
			return true;
		}
	}
	else
	{
		pCreature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(!pCreature)
		{
			RedSystemMessage(m_session, "Vous devez selectionner une creature.");
			return true;
		}
		ai = pCreature->GetAIInterface();
	}
    
	char* next_token;
	char* pWaitTime = strtok_s((char*)args, " ",&next_token);

	if(!pWaitTime)
	{
	  GreenSystemMessage(m_session, "Params: <WaitTime> <Flags> <FwdEmOneShot> <FwdEmId> <BckdEmOneShot> <BckdEmId> <FwdSkinId> <BckdSkinId>");
      return false;
	}
	uint32 WaitTime = (pWaitTime)? atoi(pWaitTime) : 10000;                                  // WaitTime
	char* pFlags = strtok_s(NULL, " ",&next_token);
	uint32 Flags = (pFlags)? atoi(pFlags) : 0 ;                                              // Flags 
	char* pForwardEmoteOneShot = strtok_s(NULL, " ",&next_token);
    uint32 ForwardEmoteOneShot = (pForwardEmoteOneShot)? atoi(pForwardEmoteOneShot) : 0;     // ForwardEmoteOneShot (Bool)
	char* pForwardEmoteId = strtok_s(NULL, " ",&next_token);
	uint32 ForwardEmoteId = (pForwardEmoteId)? atoi(pForwardEmoteId) : 0;                    // ForwardEmoteId
    char* pBackwardEmoteOneShot = strtok_s(NULL, " ",&next_token);
	uint32 BackwardEmoteOneShot = (pBackwardEmoteOneShot)? atoi(pBackwardEmoteOneShot) : 0;  // BackwardEmoteOneShot (Bool)
    char* pBackwardEmoteId = strtok_s(NULL, " ",&next_token);
	uint32 BackwardEmoteId = (pBackwardEmoteId)? atoi(pBackwardEmoteId) : 0;                 // BackwardEmoteId
	char* pForwardSkinId = strtok_s(NULL, " ",&next_token);
	uint32 ForwardSkinId = (pForwardSkinId)? atoi(pForwardSkinId) : pCreature->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID); // ForwardSkinId
	char* pBackwardSkinId = strtok_s(NULL, " ",&next_token);
	uint32 BackwardSkinId = (pBackwardSkinId)? atoi(pBackwardSkinId) : pCreature->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID); // BackwardSkinId


	WayPoint* wp = new WayPoint;
	bool showing = ai->m_WayPointsShowing;
	wp->id = (uint32)ai->GetWayPointsCount()+1;
	wp->x = p->GetPositionX();
	wp->y = p->GetPositionY();
	wp->z = p->GetPositionZ();
	wp->waittime = WaitTime;
	wp->flags = Flags;
	wp->forwardemoteoneshot = (ForwardEmoteOneShot>0)?true:false;
	wp->forwardemoteid = ForwardEmoteId;
	wp->backwardemoteoneshot = (BackwardEmoteOneShot>0)?true:false;
	wp->backwardemoteid = BackwardEmoteId;
	wp->forwardskinid = ForwardSkinId;
	wp->backwardskinid = BackwardSkinId;

	if(showing)
		ai->hideWayPoints(p);

	ai->addWayPoint(wp);
	ai->saveWayPoints();

	if(showing)
		ai->showWayPoints(p,ai->m_WayPointsShowBackwards);

	GreenSystemMessage(m_session, "Waypoint %u added and saved.", wp->id);
	return true;
}

bool ChatHandler::HandleWPMoveTypeCommand(const char* args, WorldSession *m_session)
{
	if(!*args)
		return false;

	uint32 option = atoi((char*)args);

	if (option != 0 && option != 1 && option != 2 && option != 3 && option != 4 && option != 10 && option != 11)
	{
		std::stringstream ss;
		ss << "Incorrect value." << endl;
		ss << "0 is Move from WP 1 ->  10 then 10 -> 1." << endl;
		ss << "1 is Move from WP to a random WP." << endl;
		ss << "2 is Move from WP 1 -> 10 then 1 -> 10." << endl;
		ss << "3 is Move from WP to a wanted WP." << endl;
		ss << "4 is Move from WP to a dontmove WP." << endl;
		ss << "10 is Move from WP to quest." << endl;
		ss << "11 is Move from WP to forwardthanstop." << endl;
		SendMultilineMessage(m_session, ss.str().c_str());
		return true;
	}

	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	Creature * pCreature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}

	char sql[512];
	snprintf(sql, 512, "UPDATE creature_spawns SET movetype = '%u' WHERE id = '%u'", (unsigned int)option, (unsigned int)GUID_LOPART(guid));
	WorldDatabase.Execute( sql );

	pCreature->GetAIInterface()->setMoveType(option);

	SystemMessage(m_session, "Value saved to database.");
	return true;
}


bool ChatHandler::HandleWPShowCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	Creature * pCreature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature)
	{
		SystemMessage(m_session, "Vous devez sélectionner une créature.");
		return true;
	}

	char* next_token;
	char* pBackwards = strtok_s((char*)args, " ",&next_token);
	bool Backwards = (pBackwards)? ((atoi(pBackwards)>0)?true:false) : false;

	AIInterface* ai = pCreature->GetAIInterface();
	Player* pPlayer = m_session->GetPlayer();


	if(pPlayer->waypointunit != ai)
	{
		if(ai->m_WayPointsShowing == true) 
		{
			SystemMessage(m_session, "Vous avez affiché les WayPoints d'une autre créature.");
			SystemMessage(m_session, "Voir 2 chemins differents peut causer des résultats indésirable.");
			return true;
		}

		if(pPlayer->waypointunit != NULL)
		{
			pPlayer->waypointunit->hideWayPoints(pPlayer);
		}
		pPlayer->waypointunit = ai;
		ai->showWayPoints(pPlayer,Backwards);
		ai->m_WayPointsShowBackwards = Backwards;
	}
	else
	{
		if(ai->m_WayPointsShowing == true) 
		{
			SystemMessage(m_session, "Waypoints déja en visualisation.");
		}
		else
			ai->showWayPoints(m_session->GetPlayer(),Backwards);
	}

	SystemMessage(m_session, "Les waypoints sont maintenant visible pour %u", pCreature->GetSQL_id());
	return true;
}

bool ChatHandler::HandleWPDeleteCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
	{
		SystemMessage(m_session, "You should select a Waypoint.");
		return true;
	}

	Player* pPlayer = m_session->GetPlayer();
	AIInterface* ai = pPlayer->waypointunit;
	if(!ai || !ai->GetUnit())
	{
		SystemMessage(m_session, "Invalid Creature, please select another one.");
		return true;
	}
	std::stringstream ss;

	uint32 wpid = GUID_LOPART(guid);
	if(wpid)
	{
		//Refresh client
		//Hide all
		bool show = ai->m_WayPointsShowing;
		if(show == true)
			ai->hideWayPoints(pPlayer);

		ai->deleteWayPoint(wpid);
		//Show All again after delete
		if(show == true)
			ai->showWayPoints(pPlayer,ai->m_WayPointsShowBackwards);

		SystemMessage(m_session, "Waypoint %u deleted.", wpid);
	}
	else
	{
		SystemMessage(m_session, "Invalid Waypoint.");
		return true;
	}
	return true;
}

bool ChatHandler::HandleWPChangeNoCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
	{
		SystemMessage(m_session, "You should select a Waypoint.");
		return true;
	}

	Player* pPlayer = m_session->GetPlayer();
	AIInterface* ai = pPlayer->waypointunit;
	if(!ai || !ai->GetUnit())
	{
		SystemMessage(m_session, "Invalid Creature, please select another one.");
		return true;
	}
	std::stringstream ss;

	//get newid
	char* next_token;
	char* pNewID = strtok_s((char*)args, " ",&next_token);
	uint32 NewID = (pNewID)? atoi(pNewID) : 0;

	uint32 wpid = GUID_LOPART(guid);
	if(NewID == wpid) return false;
	if(wpid)
	{
		//Refresh client
		//Hide all

		bool show = ai->m_WayPointsShowing;
		if(show == true)
			ai->hideWayPoints(pPlayer);

		//update to new id
		ai->changeWayPointID(wpid,NewID);

		//Show All again after update
		if(show == true)
			ai->showWayPoints(pPlayer,ai->m_WayPointsShowBackwards);

		ss << "Waypoint " << wpid << " changed to Waypoint " << NewID << ".";
		SystemMessage(m_session, ss.str().c_str());
	}
	else
	{
		SystemMessage(m_session, "Invalid Waypoint.");
		return true;
	}
	return true;
}

bool ChatHandler::HandleWPFlagsCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
	{
		SystemMessage(m_session, "Vous devez sélectionner un waypoint.");
		return true;
	}

	Player* pPlayer = m_session->GetPlayer();
	AIInterface* ai = pPlayer->waypointunit;
	if(!ai || !ai->GetUnit())
	{
		SystemMessage(m_session, "Creature Invalid, veuillez en sélectionner une autre.");
		return true;
	}
	std::stringstream ss;

	uint32 wpid = GUID_LOPART(guid);
	if(wpid)
	{
		WayPoint* wp = ai->getWayPoint(wpid);
		uint32 flags = wp->flags;

		char* next_token;
		char* pNewFlags = strtok_s((char*)args, " ",&next_token);
		uint32 NewFlags = (pNewFlags)? atoi(pNewFlags) : 0;

		wp->flags = NewFlags;

		//save wp
		ai->saveWayPoints();

		ss << "Waypoint de " << wpid << " a changé son flags de " << flags << " à " << NewFlags;
		SystemMessage(m_session, ss.str().c_str());
	}
	else
	{
		SystemMessage(m_session, "Waypoint Invalide.");
		return true;
	}
	return true;
}

bool ChatHandler::HandleWPMoveHereCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
	{
		SystemMessage(m_session, "Vous devez selectionner un waypoint.");
		return true;
	}

	Player* pPlayer = m_session->GetPlayer();
	AIInterface* ai = pPlayer->waypointunit;
	if(!ai || !ai->GetUnit())
	{
		SystemMessage(m_session, "Creature Invalide, veuillez en sélectionner une autre.");
		return true;
	}
	std::stringstream ss;

	uint32 wpid = GUID_LOPART(guid);
	if(wpid)
	{
		WayPoint* wp = ai->getWayPoint(wpid);
		if(wp)
		{
			wp->x = pPlayer->GetPositionX();
			wp->y = pPlayer->GetPositionY();
			wp->z = pPlayer->GetPositionZ();

			//save wp
			ai->saveWayPoints();
		}
		//Refresh client
		if(ai->m_WayPointsShowing == true)
		{
			ai->hideWayPoints(pPlayer);
			ai->showWayPoints(pPlayer,ai->m_WayPointsShowBackwards);
		}

		ss << "Waypoint de " << wpid << " a été changé.";
		SystemMessage(m_session, ss.str().c_str());
	}
	else
	{
		SystemMessage(m_session, "Invalid Waypoint.");
		return true;
	}
	return true;
}

bool ChatHandler::HandleWPWaitCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
	{
		SystemMessage(m_session, "You should select a Waypoint.");
		return true;
	}

	Player* pPlayer = m_session->GetPlayer();
	AIInterface* ai = pPlayer->waypointunit;
	if(!ai || !ai->GetUnit())
	{
		SystemMessage(m_session, "Invalid Creature, please select another one.");
		return true;
	}

	uint32 Wait = 10000;
	std::stringstream ss;

	uint32 wpid = GUID_LOPART(guid);
	if(wpid)
	{
		WayPoint* wp = ai->getWayPoint(wpid);
		if(wp)
		{
			char* next_token;
			char* pWait = strtok_s((char*)args, " ",&next_token);
			Wait = (pWait)? atoi(pWait) : 10000;

			if(Wait < 5000)
			{
				SystemMessage(m_session, "A Wait Time of less then 5000ms can cause lag, consider extending it.");
			}
			wp->waittime = Wait;

			//save wp
			ai->saveWayPoints();
		}

		ss << "Wait Time for Waypoint " << wpid << " is now " << Wait << "ms.";
		SystemMessage(m_session, ss.str().c_str());
	}
	else
	{
		SystemMessage(m_session, "Invalid Waypoint.");
		return true;
	}
	return true;
}

bool ChatHandler::HandleWPEmoteCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
	{
		SystemMessage(m_session, "You should select a Waypoint.");
		return true;
	}

	Player* pPlayer = m_session->GetPlayer();
	AIInterface* ai = pPlayer->waypointunit;
	if(!ai || !ai->GetUnit())
	{
		SystemMessage(m_session, "Creature Invalide, veuillez en sélectionner une autre.");
		return true;
	}
	uint32 EmoteId = 0;
	bool OneShot = true;
	bool Backwards = false;

	std::stringstream ss;

	uint32 wpid = GUID_LOPART(guid);
	if(wpid)
	{
		WayPoint* wp = ai->getWayPoint(wpid);
		if(wp)
		{
			char* next_token;

            char* pEmoteId = strtok_s((char*)args, " ",&next_token);
			EmoteId = (pEmoteId)? atoi(pEmoteId) : 0;
			if(pEmoteId == NULL) return(false);

			char* pBackwards = strtok_s(NULL, " ",&next_token);
    		Backwards = (pBackwards)? ((atoi(pBackwards)>0)?true:false) : 0;

			char* pOneShot = strtok_s(NULL, " ",&next_token);
			OneShot = (pOneShot)? ((atoi(pOneShot)>0)?true:false) : true;

			if(Backwards)
			{
				wp->backwardemoteid = EmoteId;
				wp->backwardemoteoneshot = OneShot;
			}
			else
			{
				wp->forwardemoteid = EmoteId;
				wp->forwardemoteoneshot = OneShot;
			}

			//save wp
			ai->saveWayPoints();
		}

		ss << "Ce waypoint de " << wpid << " a maintenant l'emote " << EmoteId << ", Type:" << ((Backwards == true)? "Backwards" : "Forwards") << "OneShot:" << ((OneShot == true)? "Oui" : "Non");
		SystemMessage(m_session,  ss.str().c_str());
	}
	else
	{
		SystemMessage(m_session, "Invalid Waypoint.");
		return true;
	}
	return true;
}

bool ChatHandler::HandleWPSkinCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session,  "No selection.");
		return true;
	}

	if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
	{
		SystemMessage(m_session,  "You should select a Waypoint.");
		return true;
	}

	Player* pPlayer = m_session->GetPlayer();
	AIInterface* ai = pPlayer->waypointunit;
	if(!ai || !ai->GetUnit())
	{
		SystemMessage(m_session, "Invalid Creature, please select another one.");
		return true;
	}
	uint32 SkinId = 0;
	std::stringstream ss;

	uint32 wpid = GUID_LOPART(guid);
	if(wpid)
	{
		WayPoint* wp = ai->getWayPoint(wpid);
		if(wp)
		{
			char* next_token;
			char* pBackwards = strtok_s((char*)args, " ",&next_token);
			uint32 Backwards = (pBackwards)? atoi(pBackwards) : 0;
			char* pSkinId = strtok_s(NULL, " ",&next_token);
			SkinId = (pSkinId)? atoi(pSkinId) : 0;
			if(Backwards)
			{
				wp->backwardskinid = SkinId;
			}
			else
			{
				wp->forwardskinid = SkinId;
			}

			//save wp
			ai->saveWayPoints();
		}

		ss << "SkinID for Waypoint " << wpid << " is now " << SkinId;
		SystemMessage(m_session, ss.str().c_str());
	}
	else
	{
		SystemMessage(m_session, "Invalid Waypoint.");
		return true;
	}
	return true;
}

bool ChatHandler::HandleWPInfoCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	if(GET_TYPE_FROM_GUID(guid) != HIGHGUID_TYPE_WAYPOINT)
	{
		SystemMessage(m_session, "You should select a Waypoint.");
		return true;
	}

	Player* pPlayer = m_session->GetPlayer();
	AIInterface* ai = pPlayer->waypointunit;
	if(!ai || !ai->GetUnit())
	{
		SystemMessage(m_session, "Invalid Creature, please select another one.");
		return true;
	}
	std::stringstream ss;

	uint32 wpid = GUID_LOPART(guid);
	if((wpid > 0) && (wpid <= ai->GetWayPointsCount()))
	{
		WayPoint* wp = ai->getWayPoint(wpid);
		if(wp)
		{
			ss << "Waypoint Number " << wp->id << ":\n";
			ss << "WaitTime: " << wp->waittime << "\n";
			ss << "Flags: " << wp->flags;
			if((wp->flags & (MOVEFLAG_WALK | MOVEFLAG_TAXI)) || (wp->flags & MOVEFLAG_FLYING) )
				ss << " (Fly)\n";
			else if(wp->flags & MOVEFLAG_WALK)
				ss << " (Run)\n";
			else
				ss << " (Walk)\n";
			ss << "Backwards\n";
			ss << "   emoteid: " << wp->backwardemoteid << "\n";
			ss << "   oneshot: " << ((wp->backwardemoteoneshot == 1)? "Yes" : "No") << "\n";
			ss << "   skinid: " << wp->backwardskinid << "\n";
			ss << "Forwards\n";
			ss << "   emoteid: " << wp->forwardemoteid << "\n";
			ss << "   oneshot: " << ((wp->forwardemoteoneshot == 1)? "Yes" : "No") << "\n";
			ss << "   skinid: " << wp->forwardskinid << "\n";
			SendMultilineMessage(m_session, ss.str().c_str());
		}
	}
	else
	{
	   SystemMessage(m_session,  "Invalid Waypoint.");
		return true;
	}
	return true;
}

bool ChatHandler::HandleWPHideCommand(const char* args, WorldSession *m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No selection.");
		return true;
	}

	Creature * pCreature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature)
	{
		SystemMessage(m_session, "Vous devez sélectionner une Creature.");
		return true;
	}

	AIInterface* ai = pCreature->GetAIInterface();
	Player* pPlayer = m_session->GetPlayer();


	if(pPlayer->waypointunit == ai)
	{
		if(ai->m_WayPointsShowing == true)
			pPlayer->waypointunit->hideWayPoints(pPlayer);
			
		pPlayer->waypointunit = NULL;
	}
	else
	{
		SystemMessage(m_session, "Les waypoints pour cette créature ne sont pas visible.");
		return true;
	}

	std::stringstream ss;
	ss << "Les waypoints pour " << pCreature->GetSQL_id() << " sont maintenant cachés.";
	SystemMessage(m_session, ss.str().c_str());

	return true;
}

bool ChatHandler::HandleGenerateWaypoints(const char* args, WorldSession * m_session)
{
	Creature * cr = 
		m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(m_session->GetPlayer()->GetSelection()));
	if(!cr)
	{
		SystemMessage(m_session, "You should select a creature.");
		return true;
	}
	if(cr->GetAIInterface()->GetWayPointsCount())//ALREADY HAVE WAYPOINTS
	{	
		RedSystemMessage(m_session, "La creature a déja des waypoints de définie");
		return false;
	}
	if(m_session->GetPlayer()->waypointunit != NULL)
	{
		RedSystemMessage(m_session, "Vous visualisez déja des waypoints, cachez les d'abord.");
		return true;
	}

	if(!cr->GetSQL_id()) return false;

	char* next_token;
	char* pR = strtok_s((char*)args, " ",&next_token);
	if(!pR)
	{
		RedSystemMessage(m_session, "Rayon du waypoint manquant.");
		return false;
	}

	int r = atoi(pR);
    if( (r<1) || (r>1000)) 
	{
      RedSystemMessage(m_session, "Rayon max non valide ( 1 < Rayon < 1000)");
	  return true;
	}

	char *pC=strtok_s(NULL, " ",&next_token);
	if(!pC)
	{
		RedSystemMessage(m_session, "Nombre de Waypoint manquant.");
		return false;
	}
	int n = atol(pC);

	for(int i=0;i<n;i++) 
	{
		//float ang = rand()/100.0f;           // RAND_MAX = 0x7FFF (32767)
        float ang = rand()/1000.0f;           // l'angle est en radian (Branruz) / 3.2767 # 3.14159 on va dire.
		float ran = ((rand()%(r*10))/10.0f)+1;

		// ran est le vecteur de la position
		float x = cr->GetPositionX()+ran*sin(ang); 
		float y = cr->GetPositionY()+ran*cos(ang); 

		float z = cr->GetMapMgr()->GetBaseMap()->GetLandHeight(x,y,cr->GetPositionZ());
		 
		WayPoint* wp = new WayPoint;
		wp->id = (uint32)cr->GetAIInterface()->GetWayPointsCount()+1;
		wp->x = x;
		wp->y = y;
		wp->z = z;
		wp->waittime = 5000;
		wp->flags = 0;
		wp->forwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->backwardemoteid = 0;
		wp->forwardskinid = 0;
		wp->backwardskinid = 0;
		
		cr->GetAIInterface()->addWayPoint(wp);
	}

	if(cr->m_spawn && cr->m_spawn->movetype != 1)		/* move random */
	{
		cr->m_spawn->movetype = 1;
		cr->GetAIInterface()->m_moveType = 1;
		WorldDatabase.Execute("UPDATE creature_spawns SET movetype = 1 WHERE id = %u", cr->GetSQL_id());
	}
	m_session->GetPlayer()->waypointunit = cr->GetAIInterface();
	cr->GetAIInterface()->showWayPoints(m_session->GetPlayer(),cr->GetAIInterface()->m_WayPointsShowBackwards);
	
	//cr->GetAIInterface()->saveWayPoints();
    GreenSystemMessage(m_session, "%u Waypoints ont ete genere.",n);
	return true;
}

bool ChatHandler::HandleSaveWaypoints(const char* args, WorldSession * m_session)
{
	Creature * cr = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(m_session->GetPlayer()->GetSelection()));

	if(!cr)
	{
		RedSystemMessage(m_session, "Vous devez selectionner un Pnj ou une Creature!");
		return false;
	}

	if(!cr->GetSQL_id()) 
	{
		RedSystemMessage(m_session, "SQLEntry invalide!");
		return false;
	}
	
	cr->GetAIInterface()->saveWayPoints();
	GreenSystemMessage(m_session, "Sauvegarde Ok.");
	return true;
}


bool ChatHandler::HandleDeleteWaypoints(const char* args, WorldSession * m_session)
{
	Creature * cr = 
		m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(m_session->GetPlayer()->GetSelection()));
	if(!cr)return false;
	if(!cr->GetSQL_id())
		return false;

	if(cr->GetAIInterface()->m_WayPointsShowing)
	{
		SystemMessage(m_session, "Waypoints are showing, hide them first.");
		return true;
	}
	
	WorldDatabase.Execute("DELETE FROM creature_waypoints WHERE spawnid=%u",cr->GetSQL_id());

	cr->GetAIInterface()->deleteWaypoints();
	SystemMessage(m_session, "Deleted waypoints for %u", cr->GetSQL_id());
	return true;
}

bool ChatHandler::HandleWaypointAddFlyCommand(const char * args, WorldSession * m_session)
{
	uint64 guid = m_session->GetPlayer()->GetSelection();
	if (guid == 0)
	{
		SystemMessage(m_session, "No Selection");
		return true;
	}
	AIInterface* ai = NULL;
	Creature * pCreature = NULL;
	Player* p = m_session->GetPlayer();
	if(p->waypointunit != NULL)
	{
		SystemMessage(m_session, "Using Previous Unit.");
		ai = p->waypointunit;
		if(!ai)
		{
			SystemMessage(m_session, "Invalid Creature, please select another one.");
			return true;
		}

		pCreature = (Creature*)ai->GetUnit();
		if(!pCreature)
		{
			SystemMessage(m_session, "Invalid Creature, please select another one.");
			return true;
		}
	}
	else
	{
		pCreature = m_session->GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(!pCreature)
		{
			SystemMessage(m_session, "You should select a creature.");
			return true;
		}
		ai = pCreature->GetAIInterface();
	}

	char* next_token;
	char* pWaitTime = strtok_s((char*)args, " ",&next_token);
	uint32 WaitTime = (pWaitTime)? atoi(pWaitTime) : 10000;
	char* pForwardEmoteId = strtok_s(NULL, " ",&next_token);
	uint32 ForwardEmoteId = (pForwardEmoteId)? atoi(pForwardEmoteId) : 0;
	char* pBackwardEmoteId = strtok_s(NULL, " ",&next_token);
	uint32 BackwardEmoteId = (pBackwardEmoteId)? atoi(pBackwardEmoteId) : 0;
	char* pForwardSkinId = strtok_s(NULL, " ",&next_token);
	uint32 ForwardSkinId = (pForwardSkinId)? atoi(pForwardSkinId) : pCreature->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID);
	char* pBackwardSkinId = strtok_s(NULL, " ",&next_token);
	uint32 BackwardSkinId = (pBackwardSkinId)? atoi(pBackwardSkinId) : pCreature->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID);
	char* pForwardEmoteOneShot = strtok_s(NULL, " ",&next_token);
	uint32 ForwardEmoteOneShot = (pForwardEmoteOneShot)? atoi(pForwardEmoteOneShot) : 1;
	char* pBackwardEmoteOneShot = strtok_s(NULL, " ",&next_token);
	uint32 BackwardEmoteOneShot = (pBackwardEmoteOneShot)? atoi(pBackwardEmoteOneShot) : 1;

	WayPoint* wp = new WayPoint;
	bool showing = ai->m_WayPointsShowing;
	wp->id = (uint32)ai->GetWayPointsCount()+1;
	wp->x = p->GetPositionX();
	wp->y = p->GetPositionY();
	wp->z = p->GetPositionZ();
	wp->waittime = WaitTime;
	wp->flags = 768;
	wp->forwardemoteoneshot = (ForwardEmoteOneShot>0)?true:false;
	wp->forwardemoteid = ForwardEmoteId;
	wp->backwardemoteoneshot = (BackwardEmoteOneShot>0)?true:false;
	wp->backwardemoteid = BackwardEmoteId;
	wp->forwardskinid = ForwardSkinId;
	wp->backwardskinid = BackwardSkinId;

	if(showing)
		ai->hideWayPoints(p);

	ai->addWayPoint(wp);
	ai->saveWayPoints();

	if(showing)
		ai->showWayPoints(p,ai->m_WayPointsShowBackwards);

	SystemMessage(m_session, "Waypoint %u added.", wp->id);
	return true;
}

bool ChatHandler::HandleNpcSelectCommand(const char * args, WorldSession * m_session)
{
	Creature * un = NULL;
	float dist = 999999.0f;
	float dist2;
	Player * plr = m_session->GetPlayer();
	set<Object*>::iterator itr;
	for(itr = plr->GetInRangeSetBegin(); itr != plr->GetInRangeSetEnd(); ++itr)
	{
		if( (dist2 = plr->GetDistance2dSq(*itr)) < dist && (*itr)->GetTypeId() == TYPEID_UNIT )
		{
			un = ((Creature*)*itr);
			dist = dist2;
		}
	}

	if(!un)
	{
		SystemMessage(m_session, "No inrange creatures found.");
		return true;
	}

	plr->SetSelection(un->GetGUID());
	SystemMessage(m_session, "Set selection to "I64FMT" (%s)", un->GetGUID(), un->GetCreatureName() ? un->GetCreatureName()->Name : "Unknown");
	return true;
}
