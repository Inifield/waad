/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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

#ifndef _COLLIDEINTERFACE_H
#define _COLLIDEINTERFACE_H

/* imports */
#define NO_WMO_HEIGHT -100000.0f
#define WMO_MAX_HEIGHT 100000.0f

extern VMAP::VMapManager2* CollisionMgr;

class NavMeshData;
class NavMeshData
{
public:
	dtNavMesh* mesh;
	dtNavMeshQuery* query;
	CallbackBase *cb;
	volatile long refs;

	// Arcemu::Threading::AtomicCounter refs;

	FastMutex tilelock;
	std::map<uint32, dtTileRef> tilerefs; //key by tile, x | y <<  16

	~NavMeshData()
	{
		dtFreeNavMesh(mesh);
		dtFreeNavMeshQuery(query);
	}

#ifdef WIN32
	bool DecRef()
	{
		InterlockedDecrement(&refs);
		if(refs <= 0)
		{
			delete cb;
			//delete this;
			return(true);
		}
		else return(false);
	}
	void AddRef() { InterlockedIncrement(&refs); }
#else

	/* burlex: if anyone knows how to do the equivilent of InterlockedIncrement/Decrement on linux feel free
	   to change this, I couldn't find the atomic functions anymore though :*( */
	
	void AddRef() { ++refs; }

 	bool DecRef()
	{
		--refs;
		if(refs <= 0)
		{
			 delete cb;
			 //delete this;
			 return(true);
		}
		else return(false);
	}

#endif

};

class SERVER_DECL CCollideInterface
{
public:
	void Init();
	void DeInit();

	//Key: mapid
	FastMutex m_navmaplock;
	std::map<uint32, NavMeshData*> m_navdata;

	bool ActivateTile(uint32 mapId, uint32 tileX, uint32 tileY);
	void DeactivateTile(uint32 mapId, uint32 tileX, uint32 tileY);
	bool IsActiveTile(uint32 mapId, uint32 tileX, uint32 tileY);
	void ActivateMap(uint32 mapId);
	void DeactivateMap(uint32 mapId);

	NavMeshData* GetNavMesh(uint32 mapId);
	bool CheckLOS(uint32 mapId, float x1, float y1, float z1, float x2, float y2, float z2);
	bool GetFirstPoint(uint32 mapId, float x1, float y1, float z1, float x2, float y2, float z2, float & outx, float & outy, float & outz, float distmod);
	bool IsIndoor(uint32 mapId, float x, float y, float z);
	bool IsIncity(uint32 mapid, float x, float y, float z);
	uint32 GetVmapAreaFlags(uint32 mapId, float x, float y, float z);
	float GetHeight(uint32 mapId, float x, float y, float z);
	ASCENT_INLINE bool GetFirstPoint(uint32 mapId, LocationVector & pos1, LocationVector & pos2, LocationVector & outvec, float distmod)
	{
		return GetFirstPoint(mapId, pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z, outvec.x, outvec.y, outvec.z, distmod);
	}
};

extern SERVER_DECL CCollideInterface CollideInterface;
#endif
