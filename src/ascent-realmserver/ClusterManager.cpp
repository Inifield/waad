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

#include "RStdAfx.h"

initialiseSingleton(ClusterMgr);
ClusterMgr::ClusterMgr()
{
	memset(SingleInstanceMaps, 0, sizeof(WServer*) * MAX_SINGLE_MAPID);
	memset(WorkerServers, 0, sizeof(WServer*) * MAX_WORKER_SERVERS);
	m_maxInstanceId = 0;
	m_maxWorkerServer = 0;	

	WServer::InitHandlers();
}

WServer * ClusterMgr::GetServerByInstanceId(uint32 InstanceId)
{
	InstanceMap::iterator itr = Instances.find(InstanceId);
	return (itr == Instances.end()) ? 0 : itr->second->Server;
}

WServer * ClusterMgr::GetServerByMapId(uint32 MapId)
{
	ASSERT(IS_MAIN_MAP(MapId));
	return SingleInstanceMaps[MapId]->Server;
}

Instance * ClusterMgr::GetInstanceByInstanceId(uint32 InstanceId)
{
	InstanceMap::iterator itr = Instances.find(InstanceId);
	return (itr == Instances.end()) ? 0 : itr->second;
}

Instance * ClusterMgr::GetInstanceByMapId(uint32 MapId)
{
	m_lock.AcquireReadLock();
	Instance* s = SingleInstanceMaps[MapId];
	m_lock.ReleaseReadLock();
	return s;
}

Instance* ClusterMgr::GetAnyInstance()
{
	//
	m_lock.AcquireReadLock();
	for (uint32 i=0; i<MAX_SINGLE_MAPID; ++i)
	{
		if (SingleInstanceMaps[i] != NULL)
		{
			m_lock.ReleaseReadLock();
			return SingleInstanceMaps[i];
		}
	}
	m_lock.ReleaseReadLock();
	return NULL;

}

Instance * ClusterMgr::GetPrototypeInstanceByMapId(uint32 MapId)
{
	m_lock.AcquireReadLock();
	//lets go through all the instances of this map and find the one with the least instances :P
	std::multimap<uint32, Instance*>::iterator itr = InstancedMaps.find(MapId);

	if (itr == InstancedMaps.end())
	{
		m_lock.ReleaseReadLock();
		return NULL;
	}

	Instance* i = NULL;
	uint32 min = 500000;
	for (; itr != InstancedMaps.upper_bound(MapId); ++itr)
	{
		if (itr->second->MapCount < min)
		{
			min = itr->second->MapCount;
			i = itr->second;
		}
	}

	m_lock.ReleaseReadLock();
	return i;
}


WServer * ClusterMgr::CreateWorkerServer(WSSocket * s)
{
	/* find an id */
	m_lock.AcquireWriteLock();
	uint32 i;
	for(i = 1; i < MAX_WORKER_SERVERS; ++i)
	{
		if(WorkerServers[i] == 0)
			break;
	}

	if(i == MAX_WORKER_SERVERS)
	{
		m_lock.ReleaseWriteLock();
		return 0;		// No spaces
	}

	Log.Success("ClusterMgr", "Allocation du serveur de 'Monde' %u à l'adresse:port %s:%u", i, s->GetRemoteIP().c_str(), s->GetRemotePort());
	WorkerServers[i] = new WServer(i, s);
	if(m_maxWorkerServer <= i)
		m_maxWorkerServer = i+1;
	m_lock.ReleaseWriteLock();
	return WorkerServers[i];
}

void ClusterMgr::AllocateInitialInstances(WServer * server, vector<uint32>& preferred)
{
	m_lock.AcquireReadLock();
	vector<uint32> result;
	result.reserve(10);

	for(vector<uint32>::iterator itr = preferred.begin(); itr != preferred.end(); ++itr)
	{
		if(SingleInstanceMaps[*itr] == 0)
		{
			result.push_back(*itr);
		}
	}
	m_lock.ReleaseReadLock();

	for(vector<uint32>::iterator itr = result.begin(); itr != result.end(); ++itr)
	{
		CreateInstance(*itr, server);
	}
}

Instance * ClusterMgr::CreateInstance(uint32 MapId, WServer * server)
{
	MapInfo * info = WorldMapInfoStorage.LookupEntry(MapId);
	if(!info)
		return NULL;

	Instance * pInstance = new Instance;
	pInstance->InstanceId = ++m_maxInstanceId;
	pInstance->MapId = MapId;
	pInstance->Server = server;

	m_lock.AcquireWriteLock();
	Instances.insert( make_pair( pInstance->InstanceId, pInstance ) );

	if(IS_MAIN_MAP(MapId))
		SingleInstanceMaps[MapId] = pInstance;
	m_lock.ReleaseWriteLock();

	/* tell the actual server to create the instance */
	WorldPacket data(ISMSG_CREATE_INSTANCE, 8);
	data << MapId << pInstance->InstanceId;
	server->SendPacket(&data);
	server->AddInstance(pInstance);
	Log.Success("ClusterMgr", "Allocation de l'instance %u sur la map %u pour le serveur %u", pInstance->InstanceId, pInstance->MapId, server->GetID());
	return pInstance;
}

WServer * ClusterMgr::GetWorkerServerForNewInstance()
{
    WServer * lowest = 0;
	int32 lowest_load = -1;

	/* for now we'll just work with the instance count. in the future we might want to change this to
	   use cpu load instead. */

	Slave_lock.Acquire();
	for(uint32 i = 0; i < MAX_WORKER_SERVERS; ++i) {
		if(WorkerServers[i] != 0) {
			if((int32)WorkerServers[i]->GetInstanceCount() < lowest_load)
			{
				lowest = WorkerServers[i];
				lowest_load = int32(WorkerServers[i]->GetInstanceCount());
			}
		}
	}
	Slave_lock.Release();

	return lowest;
}

/* create new instance based on template, or a saved instance */
Instance * ClusterMgr::CreateInstance(uint32 InstanceId, uint32 MapId)
{

	MapInfo * info = WorldMapInfoStorage.LookupEntry(MapId);
	if(!info)
		return NULL;

	/* pick a server for us :) */
	WServer * server = GetWorkerServerForNewInstance();
	if(!server)
		return NULL;

	if(!GetInstance(InstanceId))
		return NULL;

	/* bump up the max id if necessary */
	if(m_maxInstanceId <= InstanceId)
		m_maxInstanceId = InstanceId + 1;

    Instance * pInstance = new Instance;
	pInstance->InstanceId = InstanceId;
	pInstance->MapId = MapId;
	pInstance->Server = server;

	m_lock.AcquireWriteLock();
	Instances.insert( make_pair( InstanceId, pInstance ) );

	if(info->type == 0 /*INSTANCE_NULL*/)
		SingleInstanceMaps[MapId] = pInstance;
	m_lock.ReleaseWriteLock();

	/* tell the actual server to create the instance */
	WorldPacket data(ISMSG_CREATE_INSTANCE, 8);
	data << MapId << InstanceId;
	server->SendPacket(&data);
	server->AddInstance(pInstance);
	Log.Success("ClusterMgr", "Allocation de l'instance %u sur la map %u pour le serveur %u", pInstance->InstanceId, pInstance->MapId, server->GetID());
	return pInstance;
}

void ClusterMgr::Update()
{
	Slave_lock.Acquire();
	for(uint32 i = 1; i < m_maxWorkerServer; i++)
		if(WorkerServers[i])
			WorkerServers[i]->Update();
	Slave_lock.Release();
}

void ClusterMgr::DistributePacketToAll(WorldPacket * data, WServer * exclude)
{
	Slave_lock.Acquire();
	for(uint32 i = 0; i < m_maxWorkerServer; i++)
		if(WorkerServers[i] && WorkerServers[i] != exclude)
			WorkerServers[i]->SendPacket(data);
	Slave_lock.Release();
}

void ClusterMgr::OnServerDisconnect(WServer* s)
{	
	m_lock.AcquireWriteLock();

	if (Instances.size())
	{
		InstanceMap::iterator itr=Instances.begin();
		while ( itr!=Instances.end() )
		{
			if (itr->second->Server == s)
			{
				Log.Warning("ClusterMgr", "Suppression de l'instance %u sur la Map %u dûe à la déconnexion du serveur 'Monde'", itr->first, itr->second->MapId);
				delete itr->second;
				itr = Instances.erase(itr);
			}
			else ++itr;
		}
		Instances.clear();
	}
	
	if (InstancedMaps.size())
	{
		std::multimap<uint32, Instance*>::iterator itr1=InstancedMaps.begin();
		while ( itr1!=InstancedMaps.end() )
		{
			if (itr1->second->Server == s)
			{
				Log.Warning("ClusterMgr", "Suppression de l'instance 'Prototype Map' %u dûe à la déconnexion du serveur 'Monde'", itr1->first);
				InstancedMaps.erase(itr1++);
			}
			else ++itr1;
		}
		InstancedMaps.clear();
	}

	for (uint32 i=0; i<MAX_SINGLE_MAPID; ++i)
	{
		if (SingleInstanceMaps[i])
		{ 
			if (SingleInstanceMaps[i]->Server == s)
			{
				Log.Warning("ClusterMgr", "Suppression de la Map %u dûe à la déconnexion du serveur 'Monde'", i);
				SingleInstanceMaps[i] = NULL;
			}
		}
	}

	for(uint32 i = 0; i  < m_maxWorkerServer; i++)
	{
		if (WorkerServers[i] == s)
		{
			Log.Warning("ClusterMgr", "Suppression du serveur 'Monde' %u dûe à la fermeture du socket", i);
			WorkerServers[i] = NULL;
		}
	}

	delete s;

	m_lock.ReleaseWriteLock();
}
