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
Mutex m_transportGuidGen;
uint32 m_transportGuidMax = 50;

bool Transporter::CreateAsTransporter(uint32 EntryID, const char* Name, int32 Time)
{
	// Lookup GameobjectInfo
	if(!CreateFromProto(EntryID,0,0,0,0,0)) 
	{
	 sLog.outError("Transporter %u %s GameObject inexistant .",EntryID,Name);
     return false;
	}
	// Override these flags to avoid mistakes in proto
	SetUInt32Value(GAMEOBJECT_FLAGS, (GO_FLAG_NO_DESPAWN | GO_FLAG_TRANSPORT)); // 40);
	SetByte(GAMEOBJECT_BYTES_1, 3, 100); // SetGoAnimProgress(100);
	// SetByte(GAMEOBJECT_BYTES_1, 0, 1);   // SetGoState(1);

	// Set period
	m_period = Time;
	SetUInt32Value(GAMEOBJECT_LEVEL, m_period);
	
	// Generate waypoints
	if(!GenerateWaypoints()) 
	{
	 sLog.outError("Transporter %u %s: Waypoints non créés !",EntryID,Name);
     return false;
	}

	// Set position
	SetMapId(m_WayPoints[0].mapid);
	SetPosition(m_WayPoints[0].x, m_WayPoints[0].y, m_WayPoints[0].z, 0);
	
	// Add to world
	AddToWorld();

	return true;
}

bool FillPathVector(uint32 PathID, TransportPath & Path)
{
	// Store dbc values into current Path array
	Path.Resize(dbcTaxiPathNode.GetNumRows());

	uint32 i = 0;
	for(uint32 j = 0; j < dbcTaxiPathNode.GetNumRows(); j++)
	{
		DBCTaxiPathNode *pathnode = dbcTaxiPathNode.LookupRow(j);
		if(pathnode->PathId == PathID)
		{ // const char* dbctaxipathnodeFormat = "uuuufffuuxx";
			Path[i].mapid	   = pathnode->ContinentID;
			Path[i].x		   = pathnode->LocX;
			Path[i].y		   = pathnode->LocY;
			Path[i].z		   = pathnode->LocZ;
			Path[i].actionFlag  = pathnode->flags;
			Path[i].delay = pathnode->delay;
			++i;
		}
	}

	Path.Resize(i);
	return (i > 0 ? true : false);
}

bool Transporter::GenerateWaypoints()
{
	TransportPath path;
	FillPathVector(GetInfo()->moTransport.taxiPathId /*SpellFocus*/, path);

	if(path.Size() == 0) return false;

	vector<keyFrame> keyFrames;
	int mapChange = 0;

	// Pourquoi tout les WayPoints ne sont pas pris ? (Branruz)
	// Ca semble provoquer un blocage entre les maps si le bateau change de map
	//  map a     map b ( les - ne sont pas lu, dans l'exemple )
    //              1111
	// 0123456   7890123
	// -xxxxx-           Ne change pas de map
	// -xxxxx- | -xxxxx- Change de map
	/*
	for (int i = 1; i < (int)path.Size() - 1; i++)
	{
		if (mapChange == 0)
		{
			if ((path[i].mapid == path[i+1].mapid))
			{
				keyFrame k(path[i].x, path[i].y, path[i].z, path[i].mapid, path[i].actionFlag, path[i].delay);
				keyFrames.push_back(k);
			}
			else
			{
				mapChange = 1;
			}
		}
		else
		{
			mapChange--;
		}
	}*/
	// Test: On prend tout les waypoints en compte (Branruz)
	for (int i = 0; i < (int)path.Size(); i++)
	{
	 keyFrame k(path[i].x, path[i].y, path[i].z, path[i].mapid, path[i].actionFlag, path[i].delay);
	 keyFrames.push_back(k);
	}
    //************

	int lastStop = -1;
	int firstStop = -1;

	// first cell is arrived at by teleportation :S
	keyFrames[0].distFromPrev = 0;
	if (keyFrames[0].actionflag == 2)
	{
		lastStop = 0;
	}

	// find the rest of the distances between key points
	for (size_t i = 1; i < keyFrames.size(); i++)
	{
		if ((keyFrames[i-1].actionflag == 1) || (keyFrames[i].mapid != keyFrames[i-1].mapid))
		{
			keyFrames[i].distFromPrev = 0;
		}
		else
		{
			keyFrames[i].distFromPrev =
				sqrt(pow(keyFrames[i].x - keyFrames[i - 1].x, 2) +
				pow(keyFrames[i].y - keyFrames[i - 1].y, 2) +
				pow(keyFrames[i].z - keyFrames[i - 1].z, 2));
		}
		if (keyFrames[i].actionflag == 2) 
		{
            if(firstStop<0) firstStop = (int)i;
			lastStop = (int)i;
		}
	}

	float tmpDist = 0;
	for (int i = 0; i < (int)keyFrames.size(); i++)
	{
		int j = (i + lastStop) % (int)keyFrames.size();

		if (keyFrames[j].actionflag == 2) tmpDist = 0;
		else                              tmpDist += keyFrames[j].distFromPrev;

		keyFrames[j].distSinceStop = tmpDist;
	}

	for (int i = int(keyFrames.size()) - 1; i >= 0; i--)
	{
		int j = (i + (firstStop+1)) % (int)keyFrames.size();
		tmpDist += keyFrames[(j + 1) % keyFrames.size()].distFromPrev;
		keyFrames[j].distUntilStop = tmpDist;

		if (keyFrames[j].actionflag == 2) tmpDist = 0;
	}

	for (size_t i = 0; i < keyFrames.size(); i++)
	{
		if (keyFrames[i].distSinceStop < (30 * 30 * 0.5))
			keyFrames[i].tFrom = sqrt(2 * keyFrames[i].distSinceStop);
		else
			keyFrames[i].tFrom = ((keyFrames[i].distSinceStop - (30 * 30 * 0.5f)) / 30) + 30;

		if (keyFrames[i].distUntilStop < (30 * 30 * 0.5))
			keyFrames[i].tTo = sqrt(2 * keyFrames[i].distUntilStop);
		else
			keyFrames[i].tTo = ((keyFrames[i].distUntilStop - (30 * 30 * 0.5f)) / 30) + 30;

		keyFrames[i].tFrom *= 1000;
		keyFrames[i].tTo *= 1000;
	}

	//	for (int i = 0; i < keyFrames.size(); i++) {
	//		sLog.outString("%f, %f, %f, %f, %f, %f, %f", keyFrames[i].x, keyFrames[i].y, keyFrames[i].distUntilStop, keyFrames[i].distSinceStop, keyFrames[i].distFromPrev, keyFrames[i].tFrom, keyFrames[i].tTo);
	//	}

	// Now we're completely set up; we can move along the length of each waypoint at 100 ms intervals
	// speed = max(30, t) (remember x = 0.5s^2, and when accelerating, a = 1 unit/s^2
	int t = 0;
	bool teleport = false;
	if (keyFrames[keyFrames.size() - 1].mapid != keyFrames[0].mapid)
		teleport = true;

	TWayPoint pos(keyFrames[0].mapid, keyFrames[0].x, keyFrames[0].y, keyFrames[0].z, teleport);
	uint32 last_t = 0;
	m_WayPoints[0] = pos;
	t += keyFrames[0].delay * 1000;

	int cM = keyFrames[0].mapid;
	for (size_t i = 0; i < keyFrames.size() - 1; i++)	   //
	{
		float d = 0;
		float tFrom = keyFrames[i].tFrom;
		float tTo = keyFrames[i].tTo;

		// keep the generation of all these points; we use only a few now, but may need the others later
		if (((d < keyFrames[i + 1].distFromPrev) && (tTo > 0)))
		{
			while ((d < keyFrames[i + 1].distFromPrev) && (tTo > 0))
			{
				tFrom += 100;
				tTo -= 100;

				if (d > 0)
				{
					float newX, newY, newZ;
					newX = keyFrames[i].x + (keyFrames[i + 1].x - keyFrames[i].x) * d / keyFrames[i + 1].distFromPrev;
					newY = keyFrames[i].y + (keyFrames[i + 1].y - keyFrames[i].y) * d / keyFrames[i + 1].distFromPrev;
					newZ = keyFrames[i].z + (keyFrames[i + 1].z - keyFrames[i].z) * d / keyFrames[i + 1].distFromPrev;

					bool teleport = false;
					if ((int)keyFrames[i].mapid != cM)
					{
						teleport = true;
						cM = keyFrames[i].mapid;
					}

					//					sLog.outString("T: %d, D: %f, x: %f, y: %f, z: %f", t, d, newX, newY, newZ);
					TWayPoint pos(keyFrames[i].mapid, newX, newY, newZ, teleport);
					if (teleport || ((t - last_t) >= 1000))
					{
						m_WayPoints[t] = pos;
						last_t = t;
					}
				}

				if (tFrom < tTo)							// caught in tFrom dock's "gravitational pull"
				{
					if (tFrom <= 30000)
					{
						d = 0.5f * (tFrom / 1000) * (tFrom / 1000);
					}
					else
					{
						d = 0.5f * 30 * 30 + 30 * ((tFrom - 30000) / 1000);
					}
					d = d - keyFrames[i].distSinceStop;
				}
				else
				{
					if (tTo <= 30000)
					{
						d = 0.5f * (tTo / 1000) * (tTo / 1000);
					}
					else
					{
						d = 0.5f * 30 * 30 + 30 * ((tTo - 30000) / 1000);
					}
					d = keyFrames[i].distUntilStop - d;
				}
				t += 100;
			}
			t -= 100;
		}

		if (keyFrames[i + 1].tFrom > keyFrames[i + 1].tTo)
			t += 100 - ((long)keyFrames[i + 1].tTo % 100);
		else
			t += (long)keyFrames[i + 1].tTo % 100;

		bool teleport = false;
		if ((keyFrames[i + 1].actionflag == 1) || (keyFrames[i + 1].mapid != keyFrames[i].mapid))
		{
			teleport = true;
			cM = keyFrames[i + 1].mapid;
		}

		TWayPoint pos(keyFrames[i + 1].mapid, keyFrames[i + 1].x, keyFrames[i + 1].y, keyFrames[i + 1].z, teleport);

		//		sLog.outString("T: %d, x: %f, y: %f, z: %f, t:%d", t, pos.x, pos.y, pos.z, teleport);

		//if (teleport)
		//m_WayPoints[t] = pos;
		// Attention avec le flag, normalement on devrait plutot 
		// tester le Flag = 2 (signifie "Arret" au quai)
		//if(keyFrames[i+1].delay > 5) pos.delayed = true;
		if(keyFrames[i+1].actionflag == 2)
			pos.delayed = true;

		m_WayPoints.insert(WaypointMap::value_type(t, pos));
		last_t = t;

		t += keyFrames[i + 1].delay * 1000;
		//		sLog.outString("------");
	}

	uint32 timer = t;

	mCurrentWaypoint = m_WayPoints.begin();
	//mCurrentWaypoint = GetNextWaypoint();
	mNextWaypoint = GetNextWaypoint();
	m_pathTime = timer;
	

	if( this->GetInfo()->ID == sWorld.m_DebugTransporters)
	{
		Log.Notice("[Transporter]","Waypoints Ok %u - %s",this->GetInfo()->ID,this->GetInfo()->Name);
	}

	return true;
}

WaypointIterator Transporter::GetNextWaypoint()
{
	WaypointIterator iter = mCurrentWaypoint;
	iter++;
	if (iter == m_WayPoints.end())
		iter = m_WayPoints.begin();
	return iter;
}

uint32 TimeStamp();
void Transporter::UpdatePosition()
{
	if (!m_canmove)
		return;

	if (m_WayPoints.size() <= 1)
		return;

	m_timer = getMSTime() % m_period;
	
	while (((m_timer - mCurrentWaypoint->first) % m_pathTime) >= ((mNextWaypoint->first - mCurrentWaypoint->first) % m_pathTime))
	{
		/*printf("%s from %u %f %f %f to %u %f %f %f\n", this->GetInfo()->Name,
			mCurrentWaypoint->second.mapid, mCurrentWaypoint->second.x,mCurrentWaypoint->second.y,mCurrentWaypoint->second.z,
			mNextWaypoint->second.mapid, mNextWaypoint->second.x,mNextWaypoint->second.y,mNextWaypoint->second.z);*/

		mCurrentWaypoint = mNextWaypoint;
		mNextWaypoint = GetNextWaypoint();
        // m_pathTime = timer; 

		// Debug mouvement / Ascent-world.conf, DebugArea (Branruz)
		if( this->GetInfo()->ID == sWorld.m_DebugTransporters)
		{
          Log.Notice("Transporter","%s ",this->GetInfo()->Name);
		  Log.Notice("Transporter","Periode: %s ",ConvertMSTimeToString(m_period).c_str());

		  Log.Notice("Transporter","m_timer %u , m_pathTime: %u ",m_timer,m_pathTime);
		  Log.Notice("Transporter","mNextWaypoint: %u ,mCurrentWaypoint: %u ",mNextWaypoint->first,mCurrentWaypoint->first);
          Log.Notice("Transporter","((m_timer - mCurrentWaypoint) mod m_pathTime) %u >",
			                     ((m_timer - mCurrentWaypoint->first) % m_pathTime) );
		  Log.Notice("Transporter","((mNextWaypoint - mCurrentWaypoint) mod m_pathTime) %u",
			                  ((mNextWaypoint->first - mCurrentWaypoint->first) % m_pathTime) );
		 Log.Notice("Transporter","from %u %f %f",mCurrentWaypoint->second.mapid, 
			  mCurrentWaypoint->second.x,mCurrentWaypoint->second.y);
		 Log.Notice("Transporter","to   %u %f %f",mNextWaypoint->second.mapid, 
			  mNextWaypoint->second.x,mNextWaypoint->second.y);	
		 if(mNextWaypoint->second.delayed) Log.Notice("Transporter","Ponton du quai en vue!");
		 if(mCurrentWaypoint->second.delayed) Log.Notice("Transporter","Arrivee a Quai! gling gling! gling gling!");
		 Log.Notice("Transporter","-----------------------");

		} // 

		if (mNextWaypoint->second.mapid != GetMapId() || mCurrentWaypoint->second.teleport) 
		{
			//mCurrentWaypoint = mNextWaypoint;
			//mNextWaypoint = GetNextWaypoint();
			TransportPassengers(mNextWaypoint->second.mapid, GetMapId(),
				mNextWaypoint->second.x, mNextWaypoint->second.y, mNextWaypoint->second.z);
			break;
		} 
		else 
		{
			SetPosition(mNextWaypoint->second.x, mNextWaypoint->second.y,
				mNextWaypoint->second.z, m_position.o, false);
		}

		if(mCurrentWaypoint->second.delayed) // Arrivée a Quai glink glink! glink glink! ( BlizzLike )
		{
			//Transprter Script = sScriptMgr.CreateAIScriptClassForGameObject(GetEntry(), this);
			switch( GetInfo()->DisplayID )
			{
			    case 7446: // La Lance du Nord / La Fierté de Hurlevent
				case 3015: // La Bravoure
				case 7087: // L'Ecume de lune / La Bénédiction d'Elune / Le Bac de Pennelune
                	{
						PlaySoundToSet(5495); // Test
						//PlaySoundToSet(5154);		// ShipDocked LightHouseFogHorn.wav
					} break;

				case 7546: // Vaisseau de l'Exodar / Zeppelin, Horde (Le Grand vent) / Zeppelin, Horde (The Mighty Wind) (Icecrown Raid)
				case 3031: // Zeppelin (La Princesse violette) / Zeppelin (L'Aigle de fer) / Zeppelin (Le Mande-tonnerre) / The Zephyr
					{
						PlaySoundToSet(5495); // Test
						//PlaySoundToSet(11804);		// ZeppelinDocked	ZeppelinHorn.wav
					} break;

				default :
					{
						PlaySoundToSet(5495);		// BoatDockingWarning	BoatDockedWarning.wav
					} break;
			}
			TransportGossip(GetInfo()->DisplayID);
		} // end of if(mCurrentWaypoint->second.delayed)

	 
	} // End of while (((m_timer ... )
	
}
void Transporter::TransportGossip(uint32 route)
{/*
	if( route == 241)
	{
		if ( mCurrentWaypoint->second.mapid )
		{
			Log.Debug("Transporter","Arrived in Ratched at %u", m_timer);
		}
		else
		{
			Log.Debug("Transporter","Arrived in Booty at %u", m_timer);
		}
	}*/
}
void Transporter::TransportPassengers(uint32 mapid, uint32 oldmap, float x, float y, float z)
{
	sEventMgr.RemoveEvents(this, EVENT_TRANSPORTER_NEXT_WAYPOINT);

	if(mPassengers.size() > 0)
	{
		PassengerIterator itr = mPassengers.begin();
		PassengerIterator it2;

#ifdef CLUSTERING
		WorldPacket data(ICMSG_TRANSPORTER_MAP_CHANGE, 24);
		data << GetEntry() << mapid << oldmap << x << y << z;
		sClusterInterface.SendPacket(&data);
#else
		WorldPacket Pending(SMSG_TRANSFER_PENDING, 12);
		Pending << mapid << GetEntry() << oldmap;
#endif
		WorldPacket NewWorld;
		LocationVector v;

		for(; itr != mPassengers.end();)
		{
			it2 = itr;
			++itr;

			Player * plr = objmgr.GetPlayer(it2->first);
			if(!plr)
			{
				// remove from map
				mPassengers.erase(it2);
				continue;
			}
			if(!plr->GetSession() || !plr->IsInWorld()) 
				continue;

			v.x = x + plr->m_TransporterX;
			v.y = y + plr->m_TransporterY;
			v.z = z + plr->m_TransporterZ;
			v.o = plr->GetOrientation();

			if(mapid == 530 && !plr->GetSession()->HasFlag(ACCOUNT_FLAG_XPACK_01))
			{
				// player does not have BC content, repop at graveyard
				plr->RepopAtGraveyard(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), plr->GetMapId());
				continue;
			}

			if(mapid == 571 && !plr->GetSession()->HasFlag(ACCOUNT_FLAG_XPACK_02))
			{
				// player does not have WOTLK content, repop at graveyard
				plr->RepopAtGraveyard(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), plr->GetMapId());
				continue;
			}

			// Lucky bitch. Do it like on official.
			if(plr->isDead())
			{
				// plr->ResurrectPlayer(NULL);
				// plr->SetUInt32Value(UNIT_FIELD_HEALTH, plr->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				// plr->SetUInt32Value(UNIT_FIELD_POWER1, plr->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
				plr->RemoteRevive();
			}

			if( plr->m_CurrentVehicle )
				plr->m_CurrentVehicle->RemovePassenger( plr );

			plr->m_lockTransportVariables = true;
#ifndef CLUSTERING
			plr->GetSession()->SendPacket(&Pending);	
			plr->_Relocate(mapid, v, false, true, 0);
#else
			plr->GetSession()->SendPacket(&data);
			plr->EventClusterMapChange(mapid, 0, v);
#endif	
		}
	}

	// Set our position
	if (IsInWorld()) RemoveFromWorld(false);
	SetPosition(x,y,z,m_position.o,false);
	SetMapId(mapid);
	AddToWorld();
}

Transporter::Transporter(uint64 guid) : GameObject(guid)
{
	m_canmove = true;
}


Transporter::~Transporter()
{
	sEventMgr.RemoveEvents(this);
	for(TransportNPCMap::iterator itr = m_npcs.begin(); itr != m_npcs.end(); ++itr)
	{
		if(itr->second->GetTypeId()==TYPEID_UNIT)
			delete static_cast< Creature* >( itr->second )->m_transportPosition;

		itr->second = NULL;
	}

	mPassengers.clear();
}

void Transporter::Init()
{
	GameObject::Init();
}

void ObjectMgr::LoadTransporters()
{
	Log.Notice("ObjectMgr", "Loading Transports...");
	QueryResult * QR = WorldDatabase.Query("SELECT * FROM transport_data", GAMEOBJECT_TYPE_MO_TRANSPORT);
	if(!QR) return;

	int64 total = QR->GetRowCount();
	TransportersCount = total;
	do 
	{
		uint32 entry = QR->Fetch()[0].GetUInt32();
		int32 period = QR->Fetch()[2].GetInt32();
		
		QueryResult *QR_disable = WorldDatabase.Query("SELECT * FROM transport_disable where entry = %u",entry);
		if(QR_disable)
		{
		Log.Warning("LoadTransporters","Tranport %u (%s) disable",QR_disable->Fetch()[0].GetUInt32(),QR_disable->Fetch()[1].GetString());
			delete QR_disable;
		continue;
		}

		Transporter * pTransporter = new Transporter((uint64)HIGHGUID_TYPE_TRANSPORTER<<32 |entry);
		pTransporter->Init();
		if(!pTransporter->CreateAsTransporter(entry, "", period))
		{
			sLog.outError("Transporter loading %u %s failed.",entry,QR->Fetch()[1].GetString());
			delete pTransporter;
			pTransporter = NULL;
		}
		else
		{
            AddTransport(pTransporter);

			QueryResult * result2 = WorldDatabase.Query("SELECT * FROM transport_creatures WHERE transport_entry = %u", entry);
			if(result2)
			{
				do 
				{
					pTransporter->AddNPC(result2->Fetch()[1].GetUInt32(), result2->Fetch()[2].GetFloat(),
						result2->Fetch()[3].GetFloat(), result2->Fetch()[4].GetFloat(),
						result2->Fetch()[5].GetFloat());

				} while (result2->NextRow());
				delete result2;
			}
		}

	} while(QR->NextRow());
	delete QR;
}

void Transporter::OnPushToWorld()
{
	// Create waypoint event                                                     Test: 200ms (Branruz) 
	sEventMgr.AddEvent(this, &Transporter::UpdatePosition, EVENT_TRANSPORTER_NEXT_WAYPOINT, 100, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Transporter::AddNPC(uint32 Entry, float offsetX, float offsetY, float offsetZ, float offsetO)
{
	uint32 guid;
	m_transportGuidGen.Acquire();
	guid = ++m_transportGuidMax;
	m_transportGuidGen.Release();

	CreatureInfo * inf = CreatureNameStorage.LookupEntry(Entry);
	CreatureProto * proto = CreatureProtoStorage.LookupEntry(Entry);
	if(inf==NULL||proto==NULL)
		return;

	Creature * pCreature = new Creature((uint64)HIGHGUID_TYPE_TRANSPORTER<<32 | guid);
	pCreature->Init();
	pCreature->Load(proto, m_position.x, m_position.y, m_position.z,this->GetOrientation());
	pCreature->m_transportPosition = new LocationVector(offsetX, offsetY, offsetZ, offsetO);
	pCreature->m_transportGuid = GetUIdFromGUID();
	pCreature->m_transportNewGuid = GetNewGUID();
	m_npcs.insert(make_pair(guid, pCreature));
}

Creature * Transporter::GetCreature(uint32 Guid)
{
	TransportNPCMap::iterator itr = m_npcs.find(Guid);
	if(itr==m_npcs.end())
		return NULL;
	if(itr->second->GetTypeId()==TYPEID_UNIT)
		return static_cast< Creature* >( itr->second );
	else
		return NULL;
}

uint32 Transporter::BuildCreateUpdateBlockForPlayer(ByteBuffer *data, Player *target )
{
uint32 cnt = Object::BuildCreateUpdateBlockForPlayer(data, target);

	// add all the npcs to the packet
	for(TransportNPCMap::iterator itr = m_npcs.begin(); itr != m_npcs.end(); ++itr)
	{
		LocationVector v_offset = GetPosition();
		v_offset.x = v_offset.x + static_cast<Creature *>(itr->second)->m_transportPosition->x;
		v_offset.y = v_offset.y + static_cast<Creature *>(itr->second)->m_transportPosition->y;
		v_offset.z = v_offset.z + static_cast<Creature *>(itr->second)->m_transportPosition->z;
		itr->second->SetPosition(GetPosition(), false);
		cnt += itr->second->BuildCreateUpdateBlockForPlayer(data, target);
	}

	return cnt;
}

void Transporter::EventClusterMapChange(uint32 mapid, LocationVector l)
{
	m_WayPoints.clear();
	
	if (!GenerateWaypoints())
		return;

	SetPosition(l.x, l.y, l.z, 0);

	//hmmm, ok
	for (WaypointMap::iterator itr=m_WayPoints.begin(); itr!=m_WayPoints.end(); ++itr)
	{
		if (itr->second.x == l.x && itr->second.y == l.y && itr->second.z == l.z)
		{
			mCurrentWaypoint = itr;
			break;
		}
	}

	mNextWaypoint = GetNextWaypoint();

	//m_canmove = true;
}