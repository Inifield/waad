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

initialiseSingleton(CBattlegroundManager);
typedef CBattleground*(*CreateBattlegroundFunc)(MapMgr* mgr,uint32 iid,uint32 group, uint32 type);

const static uint32 BGMapIds[BATTLEGROUND_NUM_TYPES] = 
{
	0,		//  0
	30,		//  1 - AV
	489,	//  2 - WSG
	529,	//  3 - AB
	0,		//  4 - 2v2
	0,		//  5 - 3v3
	0,		//  6 - 5v5
	566,	//  7 - Netherstorm BG
	572,    //  8 - Arenes
	607,	//  9 - SOTA (Rivage des anciens)
	617,    // 10 - Arene: Egouts de Dalaran
	618,    // 11 - L'arène des valeureux (Ogrimar)
 	0,      // 12 -
	0,      // 13 -
	0,      // 14 -
	0,      // 15 -
	0,      // 16 -
	0,      // 17 -
	0,      // 18 -
	0,      // 19 -
	0,      // 20 -
	0,      // 21 -
	0,      // 22 -
	0,      // 23 -
	0,      // 24 -
	0,      // 25 -
	0,      // 26 -
	0,      // 27 -
	0,      // 28 -
	0,      // 29 -
 	628,	// 30 - IOC (Ile des conquetes)
	0,      // 31 -
	489,    // 32 - Goulet par defaut sur le Maitre de Guerre sans BG de spécifié
};

static CreateBattlegroundFunc BGCFuncs[BATTLEGROUND_NUM_TYPES] = 
{
	NULL,						 //  0
	&AlteracValley::Create,		 //  1 - AV
	&WarsongGulch::Create,		 //  2 - WSG
	&ArathiBasin::Create,		 //  3 - AB
	NULL,						 //  4 - 2v2
	NULL,						 //  5 - 3v3
	NULL,						 //  6 - 5v5
	&EyeOfTheStorm::Create,		 //  7 - Netherstorm
	NULL,                        //  8 - Arena
	&StrandOfTheAncients::Create, //  9 - Rivage des Anciens
	NULL,						 // 10 - Dalaran Arena
	NULL,						 // 11 - Ogrimar Arena
	NULL,						 // 12 -
	NULL,						 // 13 -
	NULL,						 // 14 -
	NULL,						 // 15 -
	NULL,						 // 16 -
	NULL,						 // 17 -
	NULL,						 // 18 -
	NULL,						 // 19 -
	NULL,						 // 20 -
	NULL,						 // 21 -
	NULL,						 // 22 -
	NULL,						 // 23 -
	NULL,						 // 24 -
	NULL,						 // 25 -
	NULL,						 // 26 -
	NULL,						 // 27 -
	NULL,						 // 28 -
	NULL,						 // 29 -
 	NULL, /*&IsleOfConquest::Create,*/	 // 30 - Ile des conquetes (A FAIRE)
	NULL,						 // 31 -
	&WarsongGulch::Create,		 // 32 -

};

static uint32 BGMinimumPlayers[BATTLEGROUND_NUM_TYPES] = 
{
	0,							//  0
	5,							//  1 - AV
	5,							//  2 - WSG
	5,							//  3 - AB
	4,							//  4 - 2v2
	6,							//  5 - 3v3
	10,							//  6 - 5v5
	5,							//  7 - Netherstorm
	0,							//  8 -
	5,							//  9 - SOTA
    0,							// 10 - Egouts de Dalaran (Arene)
	0,							// 11 - Arene des Valeureux (Ogrimar)
	0,							// 12 - 
	0,							// 13 - 
	0,							// 14 - 
	0,							// 15 - 
	0,							// 16 - 
	0,							// 17 - 
	0,							// 18 - 
	0,							// 19 - 
	0,							// 20 - 
	0,							// 21 - 
	0,							// 22 - 
	0,							// 23 - 
	0,							// 24 - 
	0,							// 25 - 
	0,							// 26 - 
	0,							// 27 - 
	0,							// 28 - 
	0,							// 29 - 
 	5,							// 30 - Ile des conquetes
	0,							// 31 - 
	5,							// 32 - 
};

// Nombre de colonne dans le tableau des scores, (MiscScore[x])
const static uint32 BGPvPDataFieldCount[BATTLEGROUND_NUM_TYPES] = 
{
	0,							//  0 - NONE
	5,							//  1 - AV
	2,							//  2 - WSG
	2,							//  3 - AB
	0,							//  4 - 2v2
	0,							//  5 - 3v3
	0,							//  6 - 5v5
	1,							//  7 - EOTS
	0,							//  8 - 
	0,							//  9 - SOTA
	0,							// 10 - Egouts de Dalaran (Arene)
	0,							// 11 - Arene des Valeureux (Arene - Ogrimar)
	0,							// 12 - 
	0,							// 13 - 
	0,							// 14 - 
	0,							// 15 - 
	0,							// 16 - 
	0,							// 17 - 
	0,							// 18 - 
	0,							// 19 - 
	0,							// 20 - 
	0,							// 21 - 
	0,							// 22 - 
	0,							// 23 - 
	0,							// 24 - 
	0,							// 25 - 
	0,							// 26 - 
	0,							// 27 - 
	0,							// 28 - 
	0,							// 29 - 
	0,							// 30 - IOC
	0,							// 31 - 
	2,							// 32 - Goulet
};
//--------------------------------------------------------------------------------------------
bool isBattleGroundOpen(uint32 _BattleGroundId)
{
	switch(_BattleGroundId)
	{
	 case BATTLEGROUND_WARSONG_GULCH:
     case BATTLEGROUND_ARATHI_BASIN:
     case BATTLEGROUND_ALTERAC_VALLEY:
     case BATTLEGROUND_EYE_OF_THE_STORM:
	 case BATTLEGROUND_STRAND_OF_THE_ANCIENTS:
	        if(BGCFuncs[_BattleGroundId] != NULL) return(true);
			else                                  return(false);
	 default: return(false);
	}
}
//--------------------------------------------------------------------------------------------
void InitPointeurBGs(void)
{
  BGMinimumPlayers[BATTLEGROUND_WARSONG_GULCH]          = sWorld.m_WarsongsBG_Min; 
  BGMinimumPlayers[BATTLEGROUND_ARATHI_BASIN]           = sWorld.m_ArathiBG_Min;
  BGMinimumPlayers[BATTLEGROUND_ALTERAC_VALLEY]         = sWorld.m_AlteracValleyBG_Min;
  BGMinimumPlayers[BATTLEGROUND_EYE_OF_THE_STORM]       = sWorld.m_NetherStormBG_Min;
  BGMinimumPlayers[BATTLEGROUND_STRAND_OF_THE_ANCIENTS] = sWorld.m_SotaBG_Min;

  BGCFuncs[BATTLEGROUND_WARSONG_GULCH]          = (sWorld.m_WarsongsBG_Open) ? &WarsongGulch::Create  : NULL ;
  BGCFuncs[BATTLEGROUND_ARATHI_BASIN]           = (sWorld.m_ArathiBG_Open)   ? &ArathiBasin::Create   : NULL ;
  BGCFuncs[BATTLEGROUND_ALTERAC_VALLEY]         = (sWorld.m_AlteracValleyBG_Open) ? &AlteracValley::Create : NULL ;
  BGCFuncs[BATTLEGROUND_EYE_OF_THE_STORM]       = (sWorld.m_NetherStormBG_Open) ? &EyeOfTheStorm::Create : NULL ;//
  BGCFuncs[BATTLEGROUND_STRAND_OF_THE_ANCIENTS] = (sWorld.m_SotaBG_Open) ? &StrandOfTheAncients::Create : NULL ;//
}
//--------------------------------------------------------------------------------------------
Creature* CBattleground::SpawnCreature(uint32 entry,float x, float y, float z, float o)
{
	CreatureProto *cp = CreatureProtoStorage.LookupEntry(entry);
	CreatureInfo *ci = CreatureNameStorage.LookupEntry(entry);
	Creature* c = NULL;
	if (cp && ci)
	{
		c = m_mapMgr->CreateCreature(entry);
		if (c != NULL)
		{
			c->Load(cp,x, y, z, o);
			c->PushToWorld(m_mapMgr);
		}
	}
	return c;
}
//--------------------------------------------------------------------------------------------
Vehicle* CBattleground::SpawnVehicle(uint32 entry, float x, float y, float z, float o)
{
	CreatureProto *cp = CreatureProtoStorage.LookupEntry(entry);
	CreatureInfo *ci = CreatureNameStorage.LookupEntry(entry);
	Vehicle* v = NULL;
	if (cp && ci)
	{
		v = m_mapMgr->CreateVehicle(entry);
		if (v != NULL)
		{
			v->Load(cp,x, y, z, o);
			v->PushToWorld(m_mapMgr);
		}
	}
	return v;
}
//--------------------------------------------------------------------------------------------
CBattlegroundManager::CBattlegroundManager() : EventableObject()
{
	int i, j;

	m_maxBattlegroundId = 0;

	for (i=0; i<BATTLEGROUND_NUM_TYPES; i++)
	{
		m_instances[i].clear();
		m_queuedGroups[i].clear();

		for (j=0; j<MAX_LEVEL_GROUP; j++)
		{
			m_queuedPlayers[i][j].clear();
		}
	}

	memset(m_averageQueueTimes, 0, sizeof(uint32)*BATTLEGROUND_NUM_TYPES*10);

}
//--------------------------------------------------------------------------------------------
void CBattlegroundManager::Init()
{
	sEventMgr.AddEvent(this, &CBattlegroundManager::EventQueueUpdate, false,EVENT_BATTLEGROUNDMGR_QUEUE_UPDATE, 5000, 0,0);
	Log.Success("CBattlegroundManager","Starting QueueUpdaterManager.");
}
//--------------------------------------------------------------------------------------------
CBattlegroundManager::~CBattlegroundManager()
{ 
	if(sEventMgr.HasEvent(this, EVENT_BATTLEGROUNDMGR_QUEUE_UPDATE)) 
	{
		sEventMgr.RemoveEvents(this,EVENT_BATTLEGROUNDMGR_QUEUE_UPDATE); // on vire s'il en reste un
    }
}
//--------------------------------------------------------------------------------------------
void CBattlegroundManager::HandleBattlegroundListPacket(WorldSession * m_session, uint32 BattlegroundType,bool BattleMaster)
{
	
/*
	if(IsArena(BattlegroundType))
	{
		WorldPacket data(SMSG_BATTLEFIELD_LIST, 18);
		data << m_session->GetPlayer()->GetGUID();
		data << uint32(6);
		data << uint32(0xC);
		data << uint8(0);
		m_session->SendPacket(&data);
		return;
	}
*/	
	if( BattlegroundType >= BATTLEGROUND_NUM_TYPES ) return;

	uint32 LevelGroup = GetLevelGrouping(m_session->GetPlayer()->getLevel());
	uint32 Count = 0;
	WorldPacket data(SMSG_BATTLEFIELD_LIST, 200);
	data << m_session->GetPlayer()->GetGUID();
    data << uint8(!BattleMaster);	// from where are we joining
	data << BattlegroundType;
	data << uint8(0);				//unk 3.3
	data << uint8(0);				//unk 3.3
	//data << uint8(2);
	// Rewards
	data << uint8(0);                                      // 3.3.3 hasWin
	data << uint32(0);                                     // 3.3.3 winHonor
	data << uint32(0);                                     // 3.3.3 winArena
	data << uint32(0);                                     // 3.3.3 lossHonor
	
	uint8 isRandom = 0;
	data << uint8(isRandom);                               // 3.3.3 isRandom
	
	// Random bgs
	if( isRandom == 1 ){
		// rewards
		data << uint8(0);                                  // win random
		data << uint32(0);                                 // Reward honor if won
		data << uint32(0);                                 // Reward arena point if won
		data << uint32(0);                                 // Lost honor if lost
	}
	
	size_t CountPos = data.wpos();
	data << uint32(0);		// Count

	if(!IsArena(BattlegroundType))
	{
	 /* Append the battlegrounds */
	 m_instanceLock.Acquire();
	 for(map<uint32, CBattleground*>::iterator itr = m_instances[BattlegroundType].begin(); itr != m_instances[BattlegroundType].end(); ++itr)
	 {
        if( itr->second->GetLevelGroup() == LevelGroup && itr->second->CanPlayerJoin(m_session->GetPlayer()) && !itr->second->HasEnded() )
		{
			data << itr->first;
			++Count;
		}
	 }
	 m_instanceLock.Release();

	 *(uint32*)&data.contents()[CountPos] = Count;
	}

	 m_session->SendPacket(&data);
}
//--------------------------------------------------------------------------------------------
// Returns the average queue time for a bg type, using last 10 players queued
uint32 CBattlegroundManager::GetAverageQueueTime(uint32 BgType)
{
	uint32 avg;
	int i;

	avg = m_averageQueueTimes[BgType][0];
	for (i=1; i<10; i++)
	{
		if (m_averageQueueTimes[BgType][i] == 0) break;

		avg = (avg + m_averageQueueTimes[BgType][i])/2;
	}

	return avg;
}
//--------------------------------------------------------------------------------------------
void CBattlegroundManager::AddAverageQueueTime(uint32 BgType, uint32 queueTime)
{
	int i;

	assert(BgType < BATTLEGROUND_NUM_TYPES);

	// move the queue times one place in the array
	for (i=0; i<9; i++)
	{
		m_averageQueueTimes[BgType][i+1] = m_averageQueueTimes[BgType][i];
	}

	// add the new queueTime in the first slot
	m_averageQueueTimes[BgType][0] = queueTime;
}
//--------------------------------------------------------------------------------------------
void CBattlegroundManager::HandleBattlegroundJoin(WorldSession * m_session, WorldPacket & pck)
{
	uint64 guid;
	
	uint32 bgtype;
	uint32 instance;
	uint8 joinasgroup; //0x01 = Group and 0x00 = Player

    Player* plr = m_session->GetPlayer(); // Oui
    uint32 pguid = plr->GetLowGUID();
	uint32 lgroup = GetLevelGrouping(plr->getLevel());

	pck >> guid >> bgtype >> instance >> joinasgroup;

	//Log.Warning("HandleBattlegroundJoin","%s: BGType %u, Instance: %u, JoinAsGroup: %u",plr->GetName(), bgtype,instance,joinasgroup);

	if ( ! guid ) //crash fix. /script JoinBattlefield(0,1); ;s
	{
        Log.Error("HandleBattlegroundJoin","(%s) GUID NULL <--- Script JoinBattlefield exist!",plr->GetName(),bgtype);
		return; 
	}

	if(bgtype >= BATTLEGROUND_NUM_TYPES)
	{
		Log.Error("HandleBattlegroundJoin","(%s) Type de BG inconnu: %u <--- Report this to devs!",plr->GetName(),bgtype);
		m_session->Disconnect();
		return;
	}

	if(BGMapIds[bgtype] == 0)
	{
		Log.Error("HandleBattlegroundJoin","(%s) BattleGround est NULL (%u) <--- Report this to devs!",plr->GetName(),bgtype);
		m_session->Disconnect();
		return;
	}

    if ( plr->HasActiveAura(BG_DESERTER) && !m_session->HasGMPermissions() )
	{
		m_session->GetPlayer()->GetSession()->SendNotification("Vous etes considere comme deserteur!");
		return;
	}

	MapInfo * inf = WorldMapInfoStorage.LookupEntry(BGMapIds[bgtype]);
	if(inf->minlevel > plr->getLevel())
	{
		plr->GetSession()->SendNotification("Vous n'avez pas le niveau requis pour entrer dans cette instance: lv%u.",inf->minlevel);
		return;
	}

    if( joinasgroup && plr->GetGroup() == NULL ) 
	{
		//Correct? is there any message here at blizz?
		sChatHandler.RedSystemMessage( m_session, "Vous devez etre dans un groupe." );
		return;
	}

	/* Check the instance id */
	if(instance)
	{
		/* We haven't picked the first instance. This means we've specified an instance to join. */
		m_instanceLock.Acquire();
		map<uint32, CBattleground*>::iterator itr = m_instances[bgtype].find(instance);

		if(itr == m_instances[bgtype].end())
		{
			sChatHandler.SystemMessage(m_session, "Cette instance est invalide.");
			m_instanceLock.Release();
			return;
		}

		m_instanceLock.Release();
	}
    
	/* Queue him! */
	m_queueLock.Acquire();

	if( !plr->HasBattlegroundQueueSlot() )
	{
		plr->BroadcastMessage("Vous avez trop de champ de bataille dans la file d'attente.");
		m_queueLock.Release();
		return;
	}

	if( joinasgroup && m_session->GetPlayer()->GetGroup())
	{
	 plr->GetGroup()->m_isqueued = true;
	 Log.Success("BattlegroundManager", "Le groupe %u est maintenant dans la file d'attente de champ de bataille (%u)", plr->GetGroupID(), instance );
	}
	else
		Log.Success("BattlegroundManager", "Le joueur %s est maintenant dans la file d'attente de champ de bataille (%u)", plr->GetName(), instance );

	

    /* prepare the battleground status packet */
	uint32 queueSlot = plr->GetBGQueueSlot();
	if(queueSlot == 0xFFFFFFFF)
	{
		Log.Error("HandleBattlegroundJoin"," queueSlot is 0xFFFFFFFF in %s\n", __FUNCTION__);
		m_queueLock.Release();
		return;
	} 

    if( plr->HasBGQueueSlotOfType( bgtype ) != 4) // 4 Max dans la file d'attente
		queueSlot = plr->HasBGQueueSlotOfType( bgtype );

    if( queueSlot >= 3 )
	{
		m_queueLock.Release();
		return;
	}

    if( plr->m_pendingBattleground[ queueSlot ] )
		plr->m_pendingBattleground[ queueSlot ]->RemovePendingPlayer(plr);

	if( plr->m_bg && (plr->m_bg->GetType() == bgtype) )
	{
		Log.Warning("BattlegroundManager","%s est deja dans la file d'attente du BG",plr->GetName());
		m_queueLock.Release();
		return;
	}


	//SendBattlefieldStatus(plr, 1, bgtype, instance, 0, BGMapIds[bgtype],0);

	plr->m_bgIsQueued[queueSlot] = true;
	plr->m_bgQueueInstanceId[queueSlot] = instance;
	plr->m_bgQueueType[queueSlot] = bgtype;
	plr->m_bgQueueTime[queueSlot] = (uint32)UNIXTIME;

	/* Set battleground entry point */
	plr->m_bgEntryPointX = plr->GetPositionX();
	plr->m_bgEntryPointY = plr->GetPositionY();
	plr->m_bgEntryPointZ = plr->GetPositionZ();
	plr->m_bgEntryPointMap = plr->GetMapId();
	plr->m_bgEntryPointInstance = plr->GetInstanceID();

	m_queuedPlayers[bgtype][lgroup].push_back(pguid);

	m_queueLock.Release();

	SendBattlegroundQueueStatus( plr, queueSlot);

	/* We will get updated next few seconds =) */
}

void ErasePlayerFromList(uint32 guid, list<uint32>* l)
{
	for(list<uint32>::iterator itr = l->begin(); itr != l->end(); ++itr)
	{
		if((*itr) == guid)
		{
			l->erase(itr);
			return;
		}
	}
}
//--------------------------------------------------------------------------------------------
uint32 CBattlegroundManager::GetArenaGroupQInfo(Group * group, int type, uint32 *avgRating)
{
	ArenaTeam *team;
	ArenaTeamMember *atm;
	Player* plr;
	uint32 count=0;
	uint32 rating=0;

	if (group == NULL || group->GetLeader() == NULL) return 0;

	plr = group->GetLeader()->m_loggedInPlayer;
	if (plr == NULL || plr->m_playerInfo == NULL) return 0;

	team = plr->m_playerInfo->arenaTeam[type-BATTLEGROUND_ARENA_2V2];
	if (team == NULL) return 0;

	GroupMembersSet::iterator itx;
	for(itx = group->GetSubGroup(0)->GetGroupMembersBegin(); itx != group->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
	{
		plr = (*itx)->m_loggedInPlayer;
		if(plr && plr->m_playerInfo)
		{
			if (team == plr->m_playerInfo->arenaTeam[type-BATTLEGROUND_ARENA_2V2])
			{
				atm = team->GetMemberByGuid(plr->GetLowGUID());
				if (atm)
				{
					rating+= atm->PersonalRating;
					count++;
				}
			}
		}
	}

	*avgRating = count > 0 ? rating/count : 0;

	return team->m_id;
}
//--------------------------------------------------------------------------------------------
void CBattlegroundManager::AddGroupToArena(CBattleground* bg, Group * group, int nteam)
{
	ArenaTeam *team;
	Player* plr;

	if (group == NULL || group->GetLeader() == NULL) return;

	plr = group->GetLeader()->m_loggedInPlayer;
	if (plr == NULL || plr->m_playerInfo == NULL) return;

	team = plr->m_playerInfo->arenaTeam[bg->GetType()-BATTLEGROUND_ARENA_2V2];
	if (team == NULL) return;

	GroupMembersSet::iterator itx;
	for(itx = group->GetSubGroup(0)->GetGroupMembersBegin(); itx != group->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
	{
		plr = (*itx)->m_loggedInPlayer;
		if(plr && plr->m_playerInfo && team == plr->m_playerInfo->arenaTeam[bg->GetType()-BATTLEGROUND_ARENA_2V2])
		{
			if( bg->HasFreeSlots(nteam) )
			{
				bg->AddPlayer(plr, nteam);
				plr->SetTeam(nteam);
				ArenaTeamMember * atm = team->GetMember(plr->m_playerInfo);
				if(atm)
				{
					atm->Played_ThisSeason++;
					atm->Played_ThisWeek++;
				}
			}
		}
	}

	team->m_stat_gamesplayedseason++;
	team->m_stat_gamesplayedweek++;

	team->SaveToDB();
}
//--------------------------------------------------------------------------------------------
int CBattlegroundManager::CreateArenaType(int type, Group * group1, Group * group2)
{
	Arena* ar = (Arena *)(CreateInstance(type, LEVEL_GROUP_RATED_ARENA));
	if (ar == NULL)
	{
		Log.Error("BattlegroundMgr", "%s (%u): Impossible de creer l'arene", __FILE__, __LINE__);
		m_queueLock.Release();
		m_instanceLock.Release();
		return -1;
	}
	ar->rated_match=true;

	AddGroupToArena(ar, group1, 0);
	AddGroupToArena(ar, group2, 1);

	return 0;
}
//--------------------------------------------------------------------------------------------
void CBattlegroundManager::AddPlayerToBg(CBattleground* bg, deque<Player*  > *playerVec, uint32 i, uint32 j)
{
	Player* plr = *playerVec->begin();
	playerVec->pop_front();
	if(bg->CanPlayerJoin(plr))
	{
		bg->AddPlayer(plr, plr->GetTeam());
		ErasePlayerFromList(plr->GetLowGUID(), &m_queuedPlayers[i][j]);
	}
	else 
	{
		// Put again the player in the queue
		playerVec->push_back(plr);
	}
}
//--------------------------------------------------------------------------------------------
void CBattlegroundManager::AddPlayerToBgTeam(CBattleground* bg, deque<Player*  > *playerVec, uint32 i, uint32 j, int Team)
{
	if (bg->HasFreeSlots(Team))
	{
		Player* plr = *playerVec->begin();
		playerVec->pop_front();
		plr->m_bgTeam=Team;
		bg->AddPlayer(plr, Team);
		ErasePlayerFromList(plr->GetLowGUID(), &m_queuedPlayers[i][j]);
	}
}
//--------------------------------------------------------------------------------------------
void CBattlegroundManager::EventQueueUpdate(bool forceStart)
{
	deque<Player*> tempPlayerVec[2];
	uint32 i,j,k;
	Player * plr;
	CBattleground * bg;
	list<uint32>::iterator it3, it4;
	//vector<Player*>::iterator it6;
	map<uint32, CBattleground*>::iterator iitr;
	Arena * arena;
	int32 team;
	m_queueLock.Acquire();
	m_instanceLock.Acquire();

	for(i = 0; i < BATTLEGROUND_NUM_TYPES; ++i)
	{
		for(j = 0; j < MAX_LEVEL_GROUP; ++j)
		{
			if(!m_queuedPlayers[i][j].size())
				continue;

			tempPlayerVec[0].clear();
			tempPlayerVec[1].clear();

			for(it3 = m_queuedPlayers[i][j].begin(); it3 != m_queuedPlayers[i][j].end();)
			{
				it4 = it3++;
                plr = objmgr.GetPlayer(*it4);
				
				if(!plr || GetLevelGrouping(plr->getLevel()) != j)
				{
                    m_queuedPlayers[i][j].erase(it4);
					continue;
				}

				uint32 queueSlot = plr->GetBGQueueSlotByBGType(i);
				if(queueSlot >= 3) continue;

				if( !plr->m_bgIsQueued[queueSlot] )
				{
					// We've since cancelled our queue
					m_queuedPlayers[i][j].erase(it4);
					continue;
				}

				// queued to a specific instance id?
				if((plr->m_bgQueueInstanceId[queueSlot] != 0) && plr->m_bgIsQueued[queueSlot])
				{
					if( m_instances[i].empty() ) continue; // Pas de Break??, apres c'est normalement vide (Branruz)

					iitr = m_instances[i].find(plr->m_bgQueueInstanceId[queueSlot]);
					if(iitr == m_instances[i].end())
					{
						// queue no longer valid
						plr->GetSession()->SystemMessage("Le champ de bataille %u n'existe plus.", plr->m_bgQueueInstanceId);
						//plr->m_bgIsQueued = false;
						//plr->m_bgQueueType = 0;
						//plr->m_bgQueueInstanceId = 0;
						//m_queuedPlayers[i][j].erase(it4);
						plr->RemoveFromBattlegroundQueue(queueSlot);
						SendBattlegroundQueueStatus(plr, queueSlot);
						continue;
					}

					// can we join?
					bg = iitr->second;
					if(bg->CanPlayerJoin(plr))
					{
						bg->AddPlayer(plr, plr->GetTeam());
						m_queuedPlayers[i][j].erase(it4);
					}
				}
				else
				{
					if(IsArena(i)) tempPlayerVec[0].push_back(plr);
					else           tempPlayerVec[plr->GetTeam()].push_back(plr);
				}
			}

			// try to join existing instances
			for(iitr = m_instances[i].begin(); iitr != m_instances[i].end(); ++iitr)
			{
				if( iitr->second->HasEnded() )
					continue;

				if(IsArena(i))
				{
                    arena = ((Arena*)iitr->second);
					if(arena->Rated()) continue;

					team = arena->GetFreeTeam();
					while((team >= FACTION_ALLY) && tempPlayerVec[0].size())
					{
						plr = *tempPlayerVec[0].begin();
						tempPlayerVec[0].pop_front();
						plr->m_bgTeam = team;
						arena->AddPlayer(plr, team);
						ErasePlayerFromList(plr->GetLowGUID(), &m_queuedPlayers[i][j]);
						team = arena->GetFreeTeam();
					}
				}
				else
				{
					bg = iitr->second;
					/* OBSOLETE (Branruz)
					for(k = 0; k < 2; ++k)
					{
						while(tempPlayerVec[k].size() && bg->HasFreeSlots(k))
						{
							plr = *tempPlayerVec[k].begin();
							tempPlayerVec[k].pop_front();
							bg->AddPlayer(plr, plr->GetTeam());
							ErasePlayerFromList(plr->GetLowGUID(), &m_queuedPlayers[i][j]);
						}
					}*/ // ou l'un ou l'autre mais pas les 2 (Branruz)
					if(bg)
					{
						int size = (int)min(tempPlayerVec[0].size(),tempPlayerVec[1].size());
						for(int counter = 0; (counter < size) && (bg->IsFull() == false); counter++)
						{
							AddPlayerToBgTeam(bg, &tempPlayerVec[0], i, j, 0);
							AddPlayerToBgTeam(bg, &tempPlayerVec[1], i, j, 1);
						}

						while (tempPlayerVec[0].size() > 0 && bg->HasFreeSlots(0))
						{
							AddPlayerToBgTeam(bg, &tempPlayerVec[0], i, j, 0);
						}
						while (tempPlayerVec[1].size() > 0 && bg->HasFreeSlots(1))
						{
							AddPlayerToBgTeam(bg, &tempPlayerVec[1], i, j, 1);
						}
					}
				}
			}

			if(IsArena(i))
			{
				// enough players to start a round?
				if(tempPlayerVec[0].size() < BGMinimumPlayers[i])
					continue;

				if(CanCreateInstance(i,j))
				{
					arena = ((Arena*)CreateInstance(i, j));
					team = arena->GetFreeTeam();
					while(!arena->IsFull() && tempPlayerVec[0].size() && team >= 0)
					{
						plr = *tempPlayerVec[0].begin();
						tempPlayerVec[0].pop_front();

						plr->m_bgTeam=team;
						arena->AddPlayer(plr, team);
						team = arena->GetFreeTeam();

						// remove from the main queue (painful!)
						ErasePlayerFromList(plr->GetLowGUID(), &m_queuedPlayers[i][j]);
					}
				}
			}
			else
			{
				uint32 Nb_Player_min = BGMinimumPlayers[i];
            
				if( 
					( Nb_Player_min>0 && (tempPlayerVec[0].size()>=Nb_Player_min && tempPlayerVec[1].size()>=Nb_Player_min) )
					||
					( Nb_Player_min==0 && (tempPlayerVec[0].size()>=1 || tempPlayerVec[1].size()>=1) )
					)
				{
					if(CanCreateInstance(i,j))
					{
						bg = CreateInstance(i,j);
						if(!bg) 
						{
							// creation failed
							for(k = 0; k < 2; ++k)
							{
								while(tempPlayerVec[k].size())
								{
									plr = *tempPlayerVec[k].begin();
									tempPlayerVec[k].pop_front();
									ErasePlayerFromList(plr->GetLowGUID(), &m_queuedPlayers[i][j]);
								}
							}
							Log.Error("CBattlegroundManager::EventQueueUpdate","FATAL: Creation du BG impossible!");
							//return; // Evite le crash assuré du serveur  (Branruz)
						}
						else
						{
						 // push as many as possible in
							/* OBSOLETE (Branruz)
						 for(k = 0; k < 2; ++k)
						 {
							 
							while(tempPlayerVec[k].size() && bg->HasFreeSlots(k))
							{
								plr = *tempPlayerVec[k].begin();
								tempPlayerVec[k].pop_front();
								plr->m_bgTeam=k;
								bg->AddPlayer(plr, k);
								ErasePlayerFromList(plr->GetLowGUID(), &m_queuedPlayers[i][j]);
							}
						 }
						 */
                            if (forceStart)
							{
								for(k = 0; k < 2; ++k)
								{
									while(tempPlayerVec[k].size() && bg->HasFreeSlots(k))
									{
										AddPlayerToBgTeam(bg, &tempPlayerVec[k], i, j, k);
									}
								}
							}
							else
							{
								int size = (int)min(tempPlayerVec[0].size(),tempPlayerVec[1].size());
								for(int counter = 0; (counter < size) && (bg->IsFull() == false); counter++)
								{
									AddPlayerToBgTeam(bg, &tempPlayerVec[0], i, j, 0);
									AddPlayerToBgTeam(bg, &tempPlayerVec[1], i, j, 1);
								}
							}
						}
					}
				}
			}
		}
	}

	/* Handle paired arena team joining */
	Group * group1, *group2;
	uint32 n;
	uint32 teamids[2] = {0,0};
	uint32 avgRating[2] = {0,0};

	list<uint32>::iterator itz;
	for(i = BATTLEGROUND_ARENA_2V2; i < BATTLEGROUND_ARENA_5V5+1; ++i)
	{
		if(!forceStart && m_queuedGroups[i].size() < 2)		/* got enough to have an arena battle ;P */
		{
			continue;
		} 

		for (j=0; j<(uint32)m_queuedGroups[i].size(); j++)
		{
            n =	RandomUInt((uint32)m_queuedGroups[i].size()) - 1;
			for(itz = m_queuedGroups[i].begin(); itz != m_queuedGroups[i].end() && n>0; ++itz)
				--n;

			if(itz == m_queuedGroups[i].end())
				itz = m_queuedGroups[i].begin();
            
			if(itz == m_queuedGroups[i].end()) // Begin = End ?? (Branruz)
			{
				Log.Error("BattlegroundMgr", "Internal error at %s:%u", __FILE__, __LINE__);
				m_queueLock.Release();
				m_instanceLock.Release();
				return;
			}

			group1 = group2 = NULL;
			group1 = objmgr.GetGroupById(*itz);

			if (group1 == NULL) continue; // Au moins un groupe de créé pour une arene

			
			if (forceStart && m_queuedGroups[i].size() == 1)
			{
				if (CreateArenaType(i, group1, NULL) == -1) return;
				m_queuedGroups[i].remove(group1->GetID());
				group1->m_isqueued = false;
				continue;
			}

			teamids[0] = GetArenaGroupQInfo(group1, i, &avgRating[0]);

			list<uint32> possibleGroups;

			possibleGroups.clear();
			for(itz = m_queuedGroups[i].begin(); itz != m_queuedGroups[i].end(); ++itz)
			{
				group2 = objmgr.GetGroupById(*itz);
				if (group2)
				{
					teamids[1] = GetArenaGroupQInfo(group2, i, &avgRating[1]);
					uint32 delta = abs((int32)avgRating[0] - (int32)avgRating[1]);
					if (teamids[0] != teamids[1] && delta <= 150)
					{
						possibleGroups.push_back(group2->GetID());
					}
				}
			}

			if (possibleGroups.size() > 0)
			{
				n = RandomUInt((uint32)possibleGroups.size()) - 1;
				for(itz = possibleGroups.begin(); itz != possibleGroups.end() && n>0; ++itz)
					--n;

				if(itz == possibleGroups.end())
					itz=possibleGroups.begin();

				if(itz == possibleGroups.end())
				{
					Log.Error("BattlegroundMgr", "Internal error at %s:%u", __FILE__, __LINE__);
					m_queueLock.Release();
					m_instanceLock.Release();
					return;
				}

				group2 = objmgr.GetGroupById(*itz);
				if (group2)
				{
					if (CreateArenaType(i, group1, group2) == -1) return;
					m_queuedGroups[i].remove(group1->GetID());
					group1->m_isqueued = false;
					m_queuedGroups[i].remove(group2->GetID());
					group2->m_isqueued = false;
				}
			}

			/* OBSOLETE (Branruz)
			while(group1 == NULL)
			{
				n = RandomUInt((uint32)m_queuedGroups[i].size()) - 1;
				for(itz = m_queuedGroups[i].begin(); itz != m_queuedGroups[i].end() && n>0; ++itz)
					--n;

				if(itz == m_queuedGroups[i].end())
					itz=m_queuedGroups[i].begin();

				if(itz == m_queuedGroups[i].end())
				{
					Log.Error("BattlegroundMgr", "Erreur Interne à %s:%u", __FILE__, __LINE__);
					m_queueLock.Release();
					m_instanceLock.Release();
					return;
				}

				group1 = objmgr.GetGroupById(*itz);
				m_queuedGroups[i].erase(itz);
			}

			while(group2 == NULL)
			{
				n = RandomUInt((uint32)m_queuedGroups[i].size()) - 1;
				for(itz = m_queuedGroups[i].begin(); itz != m_queuedGroups[i].end() && n>0; ++itz)
					--n;

				if(itz == m_queuedGroups[i].end())
					itz=m_queuedGroups[i].begin();

				if(itz == m_queuedGroups[i].end())
				{
					Log.Error("BattlegroundMgr", "Erreur Interne à %s:%u", __FILE__, __LINE__);
					m_queueLock.Release();
					m_instanceLock.Release();
					return;
				}

				group2 = objmgr.GetGroupById(*itz);
				m_queuedGroups[i].erase(itz);
			}

			Arena * ar = ((Arena*)CreateInstance(i,LEVEL_GROUP_ARENA));
			GroupMembersSet::iterator itx;
			ar->rated_match=true;

			for(itx = group1->GetSubGroup(0)->GetGroupMembersBegin(); itx != group1->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
			{
				if((*itx)->m_loggedInPlayer)
				{
					if( ar->HasFreeSlots(0) )
						ar->AddPlayer((*itx)->m_loggedInPlayer, 0);
				}
			}

			for(itx = group2->GetSubGroup(0)->GetGroupMembersBegin(); itx != group2->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
			{
				if((*itx)->m_loggedInPlayer)
				{
					if( ar->HasFreeSlots(1) )
						ar->AddPlayer((*itx)->m_loggedInPlayer, 1);
				}
			}
			*/


		}
	}

	m_queueLock.Release();
	m_instanceLock.Release();
}
//--------------------------------------------------------------------------------------------
void CBattlegroundManager::RemovePlayerFromQueues(Player * plr)
{
	m_queueLock.Acquire();

	//ASSERT(plr->m_bgQueueType < BATTLEGROUND_NUM_TYPES);
    if((plr->m_bgQueueType[0] >= BATTLEGROUND_NUM_TYPES) || 
	   (plr->m_bgQueueType[1] >= BATTLEGROUND_NUM_TYPES) ||
	   (plr->m_bgQueueType[2] >= BATTLEGROUND_NUM_TYPES) )
	{
	 Log.Error("RemovePlayerFromQueues","(%s) m_bgQueueType ERROR <--- Report this to devs.",plr->GetName());
	 Log.Error("                      ","Queue1: %u, Queue2: %u, Queue3: %u",
		                                   plr->m_bgQueueType[0],plr->m_bgQueueType[1],plr->m_bgQueueType[2]);
	 m_queueLock.Release();
	 return;
	}


	uint32 lgroup = GetLevelGrouping(plr->getLevel());

	uint32 i;
	for(i = 0; i < 3; ++i)
	{
	 list<uint32>::iterator itr = m_queuedPlayers[plr->m_bgQueueType[i]][lgroup].begin();
	
	 while(itr != m_queuedPlayers[plr->m_bgQueueType[i]][lgroup].end())
	 {
		if((*itr) == plr->GetLowGUID())
		{
			Log.Debug("BattlegroundManager", "Suppression du joueur %s de la file d'attente de l'instance %u de type %u", 
				                                                 plr->GetName(), plr->m_bgQueueInstanceId[i], plr->m_bgQueueType[i]);
			m_queuedPlayers[plr->m_bgQueueType[i]][lgroup].erase(itr);
			break;
		}

		++itr;
	 }
	}
	plr->m_bgIsQueued[i] = false;
	plr->m_bgTeam=plr->GetTeam();
	plr->m_pendingBattleground[i]=0;
	plr->m_bgQueueType[i]        = 0;
	plr->m_bgQueueInstanceId[i]  = 0;
	plr->m_bgQueueTime[i]        = 0;
	//SendBattlefieldStatus(plr,0,0,0,0,0,0);
    m_queueLock.Release();
}
//--------------------------------------------------------------------------------------------
void CBattlegroundManager::RemoveGroupFromQueues(Group * grp)
{
	m_queueLock.Acquire();
	for(uint32 i = BATTLEGROUND_ARENA_2V2; i < BATTLEGROUND_ARENA_5V5+1; ++i)
	{
		for(list<uint32>::iterator itr = m_queuedGroups[i].begin(); itr != m_queuedGroups[i].end(); )
		{
			if((*itr) == grp->GetID())
				itr = m_queuedGroups[i].erase(itr);
			else
				++itr;
		}
	}

	for(GroupMembersSet::iterator itr = grp->GetSubGroup(0)->GetGroupMembersBegin(); itr != grp->GetSubGroup(0)->GetGroupMembersEnd(); ++itr)
	{
		if((*itr)->m_loggedInPlayer)
		{
			for(uint32 i = 0; i < 2; ++i)
			{
				(*itr)->m_loggedInPlayer->m_bgIsQueued[i] = false;
				SendBattlegroundQueueStatus((*itr)->m_loggedInPlayer, 0);
			}
			//SendBattlefieldStatus((*itr)->m_loggedInPlayer, 0, 0, 0, 0, 0, 0); // OBSOLETE
		}
	}

	grp->m_isqueued = false;
	m_queueLock.Release();
}

//--------------------------------------------------------------------------------------------
bool CBattlegroundManager::CanCreateInstance(uint32 Type, uint32 LevelGroup)
{
	/*uint32 lc = 0;
	for(map<uint32, CBattleground*>::iterator itr = m_instances[Type].begin(); itr != m_instances[Type].end(); ++itr)
	{
		if(itr->second->GetLevelGroup() == LevelGroup)
		{
			lc++;
			if(lc >= MAXIMUM_BATTLEGROUNDS_PER_LEVEL_GROUP)
				return false;
		}
	}*/

	return true;
}
//--------------------------------------------------------------------------------------------
/*
void CBattleground::SendWorldStates(Player * plr)
{
	if(!m_worldStates.size())
		return;

	uint32 bflag = 0;
	uint32 bflag2 = 0;

	switch(m_mapMgr->GetMapId())
	{
	case  489: bflag = 0x0CCD; bflag2 = 0x0CF9; break;
	case  529: bflag = 0x0D1E; break;
	case   30: bflag = 0x0A25; break;
	case  559: bflag = 3698; break;
	case  566: bflag = 0x0eec; bflag2 = 0; break;			// EOTS
	
	default:		// arenas 
		bflag  = 0x0E76;
		bflag2 = 0;
		break;
	}

	WorldPacket data(SMSG_INIT_WORLD_STATES, 10 + (m_worldStates.size() * 8));
	data << m_mapMgr->GetMapId();
	data << bflag;
	data << bflag2;
	data << uint16(m_worldStates.size());

	for(map<uint32, uint32>::iterator itr = m_worldStates.begin(); itr != m_worldStates.end(); ++itr)
		data << itr->first << itr->second;
	plr->GetSession()->SendPacket(&data);
}
*/
//--------------------------------------------------------------------------------------------
CBattleground::CBattleground(MapMgr * mgr, uint32 id, uint32 levelgroup, uint32 type) : m_mapMgr(mgr), m_id(id), m_type(type), m_levelGroup(levelgroup)
{
	m_nextPvPUpdateTime = 0;
	m_countdownStage = 0;
	m_ended = false;
	m_started = false;
	m_winningteam = 0;
	m_startTime = (uint32)UNIXTIME;
	m_lastResurrect = (uint32)UNIXTIME;
	
	/* create raid groups */
	for(uint32 i = 0; i < 2; ++i)
	{
		m_groups[i] = new Group(true);
		m_groups[i]->m_disbandOnNoMembers = false;
		m_groups[i]->SetFlag(GROUP_FLAG_DONT_DISBAND_WITH_NO_MEMBERS | GROUP_FLAG_BATTLEGROUND_GROUP);
		m_groups[i]->ExpandToRaid();
	}

}
//--------------------------------------------------------------------------------------------
void CBattleground::Init()
{
	Log.Notice("CBattleground","Init!");
	sEventMgr.AddEvent(this, &CBattleground::EventResurrectPlayers, EVENT_BATTLEGROUNDMGR_QUEUE_UPDATE, 30000, 0,0);
}
//--------------------------------------------------------------------------------------------
CBattleground::~CBattleground()
{
	sEventMgr.RemoveEvents(this);
	for(uint32 i = 0; i < 2; ++i)
	{
		PlayerInfo *inf;
		for(uint32 j = 0; j < m_groups[i]->GetSubGroupCount(); ++j) 
		{
			for(GroupMembersSet::iterator itr = m_groups[i]->GetSubGroup(j)->GetGroupMembersBegin(); itr != m_groups[i]->GetSubGroup(j)->GetGroupMembersEnd();) {
				inf = (*itr);
				++itr;
				m_groups[i]->RemovePlayer(inf);
			}
		}
		delete m_groups[i];
	}
}
//--------------------------------------------------------------------------------------------
void CBattleground::UpdatePvPData()
{
	
	if(this->IsArena())
	{
		if(!m_ended)
		{
			return;
		}
	}

	if(UNIXTIME >= m_nextPvPUpdateTime)
	{
		if(!m_nextPvPUpdateTime) m_nextPvPUpdateTime = UNIXTIME; // Premier passage ou force la maj
		string TimerUpdate = ConvertUnixTimeToString(m_nextPvPUpdateTime);
		Log.Success("Update des Scores","%s : %s",this->GetName(),TimerUpdate.c_str());
              
		m_mainLock.Acquire();
		WorldPacket data(10*(m_players[0].size()+m_players[1].size())+50);
		BuildPvPUpdateDataPacket(&data);
		DistributePacketToAll(&data);
		m_mainLock.Release();

		m_nextPvPUpdateTime = UNIXTIME + 2; // +2sec, temps mini avant l'update
		TimerUpdate = ConvertUnixTimeToString(m_nextPvPUpdateTime);
		Log.Success("Prochain Update  ","%s : %s",this->GetName(),TimerUpdate.c_str());
	}
}
//--------------------------------------------------------------------------------------------
void CBattleground::BuildPvPUpdateDataPacket(WorldPacket * data)
{
	bool PlrIsInArena = this->IsArena();

 	if(PlrIsInArena && !m_ended) return;

	data->Initialize(MSG_PVP_LOG_DATA);
	data->reserve(10*(m_players[0].size()+m_players[1].size())+50);

	BGScore * bs;
	*data << uint8(PlrIsInArena); // 0 BG, 1 Arene
	if(PlrIsInArena)
	{	// send arena teams info
		Arena* arena= (Arena *)(this);
		ArenaTeam * teams[2] = {NULL,NULL};
		int32 ratingNegativeChange[2] = {0,0}, ratingPositiveChange[2] = {0,0}; // Value in ratingNegativeChange is displayed with minus sign in the client
		if(Rated())
		{
			teams[0] = objmgr.GetArenaTeamById(arena->m_teams[0]);
			teams[1] = objmgr.GetArenaTeamById(arena->m_teams[1]);
		}

		for(uint32 i=0; i<2; i++)
		{
			if(m_deltaRating[i] >= 0)
				ratingPositiveChange[i] = m_deltaRating[i];
			else
				ratingNegativeChange[i] = m_deltaRating[i]*(-1);
		}		
		*data << ratingNegativeChange[0];
		*data << ratingPositiveChange[0];
		*data << ratingNegativeChange[1];
		*data << ratingPositiveChange[1];

		if(teams[0])
			*data << teams[0]->m_name;
		else
			*data << uint8(0);

		if(teams[1])
			*data << teams[1]->m_name;
		else
			*data << uint8(0);
	} // End if(PlrIsArena)

	//*data << uint64(0); // unknown Test Branruz (Pas vu sous Mangos)
	*data << uint8(m_ended);
	if(m_ended)
	{
		if(PlrIsInArena)  *data << uint8(m_winningteam);                                  // on envoi la team qui gagne en Arene
		else              *data << uint8((m_winningteam) ? FACTION_ALLY : FACTION_HORDE); // on envoi la team qui perd en BG
	}

	size_t pos = data->wpos();
	*data << uint32(0); //will be set to correct number later //uint32(m_players[0].size() + m_players[1].size());

	uint32 count = 0;
	uint32 fcount = BGPvPDataFieldCount[GetType()]; // Nombre de champ MiscScore
	for(uint32 i = 0; i < 2; ++i) // 2 Equipes
	{
		for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
		{
			if( (*itr)->m_isGmInvisible ) continue;
			*data << (*itr)->GetGUID();
			bs = &(*itr)->m_bgScore;
			*data << bs->KillingBlows;

			if(PlrIsInArena)
				*data << uint8((*itr)->m_bgTeam);
			else
			{
				*data << bs->HonorableKills;
				*data << bs->Deaths;
				*data << bs->BonusHonor;
			}
			*data << bs->DamageDone;
			*data << bs->HealingDone;

			*data << fcount;	// count of values after this
			for(uint32 j = 0; j < fcount; ++j)
				*data << bs->MiscScore[j];

			count++;
		}
	}
	// Have to set correct number of players sent in log since we skip invisible GMs
	*(uint32*)&data->contents()[pos] = count;
	//*data << uint32(0); // unknown - Test (Branruz)

}
//--------------------------------------------------------------------------------------------
/* OBSOLETE (Branruz)
void CBattleground::BuildPvPUpdateDataPacket(WorldPacket * data)
{
	data->Initialize(MSG_PVP_LOG_DATA);
	data->reserve(10*(m_players[0].size()+m_players[1].size())+50);

	BGScore * bs;

	if(m_type >= BATTLEGROUND_ARENA_2V2 && m_type <= BATTLEGROUND_ARENA_5V5)
	{
		if(!m_ended)
		{
			return;
		}

		*data << uint8(1); // 0 = BG, 1 = Arene

		if(!Rated())
		{
			*data << uint32(0x61272A5C);
			*data << uint8(0);
			*data << uint32(m_players[0].size() + m_players[1].size());
			*data << uint8(0);
		}
		else
		{
			// Grab some arena teams 
			ArenaTeam * teams[2] = {NULL,NULL};
			for(uint32 i = 0; i < 2; ++i)
			{
				for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
				{
					teams[i] = (*itr)->m_arenaTeams[ ((Arena*)this)->GetArenaTeamType() ];
					if(teams[i])
						break;
				}
			}

			if(teams[0])
			{
				*data << uint32(teams[0]->m_id);
				*data << uint32(0);
				*data << teams[0]->m_name;
			}
			else
			{
				*data << uint32(0x61272A5C);
				*data << uint32(0);
				*data << uint8(0);
			}
			
			if(teams[1])
			{
				*data << uint32(teams[1]->m_id);
				*data << uint32(0);
				*data << teams[1]->m_name;
			}
			else
			{
				*data << uint32(m_players[0].size() + m_players[1].size());
				*data << uint32(0);
				*data << uint8(0);
			}
		}

		*data << uint8(1);
		*data << uint8(m_winningteam);

		*data << uint32(m_players[0].size() + m_players[1].size());
		for(uint32 i = 0; i < 2; ++i)
		{
			for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
			{
				*data << (*itr)->GetGUID();
				bs = &(*itr)->m_bgScore;
				*data << bs->KillingBlows;

				// would this be correct?
				if( Rated() )
				{
					*data << uint8((*itr)->m_bgTeam);
				}
				else
				{
					*data << uint32(0);		// w
					*data << uint32(0);		// t
					*data << uint32(0);		// f
				}

				*data << uint32(1);			// count of values after this
				*data << uint32(bs->Misc1);	// rating change
			}
		}
	}
	else
	{
		*data << uint8(0);
		if(m_ended)
		{
			*data << uint8(1);
            // Grrrrr Modif Branruz, Fix tableau des scores BGs
			switch(m_type)
			{
			//case BATTLEGROUND_ARATHI_BASIN  :
			case BATTLEGROUND_EYE_OF_THE_STORM : 
				m_winningteam = (m_winningteam) ? 0 : 1;
				break;

			case BATTLEGROUND_WARSONG_GULCH :
			case BATTLEGROUND_ALTERAC_VALLEY:
			case BATTLEGROUND_STRAND_OF_THE_ANCIENTS:
			default:  break;
			}

			*data << uint8(m_winningteam);
		}
		else
			*data << uint8(0);		// If the game has ended - this will be 1

		*data << uint32(m_players[0].size() + m_players[1].size());
		for(uint32 i = 0; i < 2; ++i)
		{
			for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
			{
				*data << (*itr)->GetGUID();
				bs = &(*itr)->m_bgScore;

				*data << bs->KillingBlows;
				*data << bs->HonorableKills;
				*data << bs->Deaths;
				*data << bs->BonusHonor;
				*data << bs->DamageDone;
				*data << bs->HealingDone;
				*data << uint32(0x2); // Compteur des Datas de Scores
				*data << bs->Misc1;
				*data << bs->Misc2;
			}
		}
	}

}
*/
//--------------------------------------------------------------------------------------------
void CBattleground::AddPlayer(Player * plr, uint32 team)
{
	m_mainLock.Acquire();

	/* This is called when the player is added, not when they port. So, they're essentially still queued, but not inside the bg yet */
	m_pendPlayers[team].insert(plr->GetLowGUID());

	// Test des files d'attente
	uint32 queueSlot = plr->GetBGQueueSlotByBGType(GetType());
	if(queueSlot >= 3)
	{
		m_mainLock.Release();
		return;
	}

	/* Send a packet telling them that they can enter */
	//BattlegroundManager.SendBattlefieldStatus(plr, 2, m_type, m_id, 120000, m_mapMgr->GetMapId(),Rated());	// You will be removed from the queue in 2 minutes.

	// Add an event to remove them in 2 minutes time. 
	sEventMgr.AddEvent(plr, &Player::RemoveFromBattlegroundQueue,queueSlot,true, EVENT_BATTLEGROUND_QUEUE_UPDATE_SLOT_1 + queueSlot, 120000, 1,0);
	plr->m_pendingBattleground[queueSlot] = this;
	plr->m_bgIsQueued[queueSlot] = false;


	// Send a packet telling them that they can enter 
	BattlegroundManager.SendBattlegroundQueueStatus(plr, queueSlot);

	// Add the queue time 
	BattlegroundManager.AddAverageQueueTime(GetType(), (uint32)(UNIXTIME-plr->m_bgQueueTime[queueSlot]));

	m_mainLock.Release();
}
//--------------------------------------------------------------------------------------------
void CBattleground::RemovePendingPlayer(Player * plr)
{
	m_mainLock.Acquire();
	m_pendPlayers[plr->m_bgTeam].erase(plr->GetLowGUID());

	/* send a null bg update (so they don't join) */

	/* OBSOLETE (Branruz)
	BattlegroundManager.SendBattlefieldStatus(plr, 0, 0, 0, 0, 0,0);
	plr->m_pendingBattleground =0;
	*/

	for(uint32 i = 0; i < 3; ++i)
	{
		if( plr->m_pendingBattleground[i] && plr->m_pendingBattleground[i] == this )
		{
			if( plr->m_pendingBattleground[i]->IsArena() )
				plr->m_bgRatedQueue = false;

			plr->m_pendingBattleground[i] = NULL;
			BattlegroundManager.SendBattlegroundQueueStatus(plr, i);
			break;
		}
	}

	plr->m_bgTeam=plr->GetTeam();
	

	m_mainLock.Release();
}
//--------------------------------------------------------------------------------------------
void CBattleground::OnPlayerPushed(Player * plr)
{
	if( plr->GetGroup() && !Rated() )
		plr->GetGroup()->RemovePlayer(plr->m_playerInfo);

	plr->ProcessPendingUpdates(&plr->GetMapMgr()->m_updateBuildBuffer, &plr->GetMapMgr()->m_compressionBuffer);
	
	if( plr->GetGroup() == NULL )
	{
		if ( plr->m_isGmInvisible == false ) //do not join invisible gm's into bg groups.
			m_groups[plr->m_bgTeam]->AddMember( plr->m_playerInfo );
	}
}
//--------------------------------------------------------------------------------------------
void CBattleground::PortPlayer(Player * plr, bool skip_teleport /* = false*/)
{
	m_mainLock.Acquire();
	if(m_ended)
	{
        for(uint32 i = 0; i < 3; ++i)
		{
			if( plr->m_pendingBattleground[i] == this )
			{
				plr->m_pendingBattleground[i] = NULL;
				plr->m_bgIsQueued[i] = false;
			}
		}

		sChatHandler.SystemMessage(plr->GetSession(), "Vous ne pouvez pas joindre ce champ de bataille car il est déjà fini.");

		/* OBSOLETE (Branruz)
		BattlegroundManager.SendBattlefieldStatus(plr, 0, 0, 0, 0, 0,0);
		plr->m_pendingBattleground = 0;
		*/
		BattlegroundManager.SendBattlegroundQueueStatus(plr, 0);
		BattlegroundManager.SendBattlegroundQueueStatus(plr, 1);
		BattlegroundManager.SendBattlegroundQueueStatus(plr, 2);
		m_mainLock.Release();
		return;
	}

	m_pendPlayers[plr->m_bgTeam].erase(plr->GetLowGUID());
	if(m_players[plr->m_bgTeam].find(plr) != m_players[plr->m_bgTeam].end())
	{
		m_mainLock.Release();
		return;
	}

    if( plr->m_bg != NULL )
	{
		plr->m_bg->RemovePlayer(plr, true);		// don't bother porting them out, we're about to port them anyway
		plr->m_bg = NULL;
	}

    plr->FullHPMP();

	plr->SetTeam(plr->m_bgTeam);
	if ( plr->m_isGmInvisible == false )
	{
		//Do not let everyone know an invisible gm has joined.
		WorldPacket data(SMSG_BATTLEGROUND_PLAYER_JOINED, 8);
		data << plr->GetGUID();
		DistributePacketToAll(&data);
	}
	m_players[plr->m_bgTeam].insert(plr);

	if( !skip_teleport )
	{
		if( plr->IsInWorld() )
			plr->RemoveFromWorld();
	}

	for(uint32 i = 0; i < 3; ++i)
	{
		if( plr->m_pendingBattleground[i] == this )
		{
			plr->m_pendingBattleground[i] = NULL;
			plr->m_bgSlot = i;
			BattlegroundManager.SendBattlegroundQueueStatus(plr, i);
		}
	}

	/* remove from any auto queue remove events */
	sEventMgr.RemoveEvents(plr, EVENT_BATTLEGROUND_QUEUE_UPDATE_SLOT_1 + plr->m_bgSlot);

	if( !skip_teleport )
	{
		if( plr->IsInWorld() )
			plr->RemoveFromWorld();
	}

	/* OBSOLETE (Branruz)
	plr->m_pendingBattleground = 0;
	plr->m_bg = this;
	*/

	if(!plr->IsPvPFlagged())
		plr->SetPvPFlag();

	/* Reset the score */
	memset(&plr->m_bgScore, 0, sizeof(BGScore));

	/* send him the world states */
	//SendWorldStates(plr); // OBSOLETE (Branruz), fait sur ZoneUpdate
	
	/* update pvp data */
	UpdatePvPData();

	/* add the player to the group */
	if(plr->GetGroup() && !Rated())
	{
		// remove them from their group
		plr->GetGroup()->RemovePlayer( plr->m_playerInfo );
	}

	if(!m_countdownStage)
	{
		m_countdownStage = 1;
		sEventMgr.AddEvent(this, &CBattleground::EventCountdown, EVENT_BATTLEGROUND_COUNTDOWN, 30000, 0,0);
		sEventMgr.ModifyEventTimeLeft(this, EVENT_BATTLEGROUND_COUNTDOWN, 10000);
	}

	plr->m_bg = this;

	sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);

	if(!skip_teleport)
	{
		/* This is where we actually teleport the player to the battleground. */	
		plr->SafeTeleport(m_mapMgr,GetStartingCoords(plr->m_bgTeam));
		//BattlegroundManager.SendBattlefieldStatus(plr, 3, m_type, m_id, (uint32)UNIXTIME - m_startTime, m_mapMgr->GetMapId(),Rated());	// Elapsed time is the last argument
	}
	
	//OnAddPlayer(plr);

	m_mainLock.Release();
}
//--------------------------------------------------------------------------------------------
CBattleground * CBattlegroundManager::CreateInstance(uint32 Type, uint32 LevelGroup)
{
	CreateBattlegroundFunc cfunc = BGCFuncs[Type];
	MapMgr * mgr = 0;
	CBattleground * bg = NULL;
	uint32 iid;
	bool isWeekend = false;
	//struct tm tm;
	//time_t t;
	//int n;

	if(Type == BATTLEGROUND_ARENA_2V2 || Type == BATTLEGROUND_ARENA_3V3 || Type == BATTLEGROUND_ARENA_5V5)
	{
		/* arenas follow a different procedure. */
		static const uint32 arena_map_ids[5] = { 559, 562, 572, 617, 618 };
		static char arena_map_name[80];
		uint32 mapid = arena_map_ids[RandomUInt(4)];
		switch(mapid)
		{
		 case 559: strcpy(arena_map_name,"de Nagrand"); break;
		 case 562: strcpy(arena_map_name,"des Tranchantes"); break;
		 case 572: strcpy(arena_map_name,"des ruines de Lordaeron"); break;
		 case 617: strcpy(arena_map_name,"des egouts de Dalaran"); break;
		 case 618: strcpy(arena_map_name,"des Valeureux"); break;
		}

		uint32 players_per_side;
		mgr = sInstanceMgr.CreateBattlegroundInstance(mapid);
		if(mgr == NULL)
		{
			mapid=562;
            mgr = sInstanceMgr.CreateBattlegroundInstance(mapid);
            if(mgr == NULL)
			{
				Log.Error("BattlegroundManager", "La map de l'Arene %s n'existe pas (MapId: %u)!",arena_map_name,mapid);
			 return NULL;		// Shouldn't happen
			}
		}

		switch(Type)
		{
		case BATTLEGROUND_ARENA_2V2:
			players_per_side = 2;
			break;

		case BATTLEGROUND_ARENA_3V3:
			players_per_side = 3;
			break;

		case BATTLEGROUND_ARENA_5V5:
			players_per_side = 5;
			break;
        default:
            players_per_side = 0;
            break;
		}

		iid = ++m_maxBattlegroundId;
        bg = new Arena(mgr, iid, LevelGroup, Type, players_per_side);
		bg->Init();
		mgr->m_battleground = bg;
		Log.Success("BattlegroundManager", "Création de l'arène %s (type %u Level %u, %u vs %u).",
			                      arena_map_name, Type, LevelGroup,players_per_side,players_per_side);
		sEventMgr.AddEvent(bg, &CBattleground::EventCreate, EVENT_BATTLEGROUNDMGR_QUEUE_UPDATE, 1, 1,0);
		m_instanceLock.Acquire();
		m_instances[Type].insert( make_pair(iid, bg) );
		m_instanceLock.Release();
		return bg;
	}


	if(cfunc == NULL)
	{
		Log.Error("BattlegroundManager", "Fonction de gestion du BG ou de l'Arene est introuvable (MapId: %u)",BGMapIds[Type]);
		return NULL;
	}

	/* Create Map Manager */
	mgr = sInstanceMgr.CreateBattlegroundInstance(BGMapIds[Type]);
	if(mgr == NULL)
	{
		Log.Error("BattlegroundManager", "Impossible de creer le BG (mapid=%u, type=%u))" , BGMapIds[Type], Type);
		return NULL;		// Shouldn't happen
	}


	// Gestion WeekEnd, A VOIR (Branruz)
	isWeekend = false;
	/*
	t = time(NULL);
#ifdef WIN32
	tm = *localtime(&t);
#else
	localtime_r(&t, &tm);
#endif

	switch (Type)
	{
	    default :
		case BATTLEGROUND_WARSONG_GULCH          : n = 0; break;
		case BATTLEGROUND_ARATHI_BASIN           : n = 1; break;
		case BATTLEGROUND_EYE_OF_THE_STORM       : n = 2; break;
		case BATTLEGROUND_ALTERAC_VALLEY         : n = 3; break;
		case BATTLEGROUND_STRAND_OF_THE_ANCIENTS : n = 4; break;
		case BATTLEGROUND_ISLE_OF_CONQUEST       : n = 5; break;
	}
	if (((tm.tm_yday / 7) % 5) == n)
	{
		// Set weekend from thursday night at midnight until tuesday morning 
		isWeekend = tm.tm_wday >= 5 || tm.tm_wday < 2;
	}
	*/

	/* Call the create function */
	iid = ++m_maxBattlegroundId;
	bg = cfunc(mgr, iid, LevelGroup, Type);	
	mgr->m_battleground = bg;
	bg->Init();
	bg->SetIsWeekend(isWeekend);
	sEventMgr.AddEvent(bg, &CBattleground::EventCreate, EVENT_BATTLEGROUNDMGR_QUEUE_UPDATE, 1, 1,0);
	Log.Success("BattlegroundManager", "Création de champ de bataille (type %u, level %u).", Type, LevelGroup);

	m_instanceLock.Acquire();
	m_instances[Type].insert( make_pair(iid, bg) );
	m_instanceLock.Release();

	return bg;
}
//--------------------------------------------------------------------------------------------
void CBattlegroundManager::DeleteBattleground(CBattleground * bg)
{
	uint32 i = bg->GetType();
	uint32 j = bg->GetLevelGroup();
	Player * plr;

	m_instanceLock.Acquire();
	m_queueLock.Acquire();
	m_instances[i].erase(bg->GetId());
	
	/* erase any queued players */
	list<uint32>::iterator itr = m_queuedPlayers[i][j].begin();
	list<uint32>::iterator it2;
	for(; itr != m_queuedPlayers[i][j].end();)
	{
		it2 = itr++;
		plr = objmgr.GetPlayer(*it2);
		if(!plr)
		{
			m_queuedPlayers[i][j].erase(it2);
			continue;
		}

		for(uint32 z= 0; z < 3; ++z)
		{
		 if(plr && plr->m_bgQueueInstanceId[z] == bg->GetId())
		 {
			sChatHandler.SystemMessageToPlr(plr, "Le champ de bataille %u est terminée.", bg->GetId());
			SendBattlegroundQueueStatus(plr, z);
			plr->m_bgIsQueued[z] = false;
			m_queuedPlayers[i][j].erase(it2);
		 }
		}
	}

	m_queueLock.Release();
	m_instanceLock.Release();

	bg = NULL;
	delete bg;
}
//--------------------------------------------------------------------------------------------
GameObject * CBattleground::SpawnGameObject(uint32 entry,uint32 MapId , float x, float y, float z, float o, uint32 flags, uint32 faction, float scale)
{
	GameObject *go = m_mapMgr->CreateGameObject(entry);

    if(!go || !go->CreateFromProto(entry, m_mapMgr->GetMapId(), x, y, z, 0.0f))
	{
		Log.Error("SpawnGameObject","GameObject %u n'existe pas!",entry);
		return(NULL);
	}

	go->CreateFromProto(entry, MapId, x, y, z, o);

	go->SetUInt32Value(GAMEOBJECT_FACTION,faction);
	go->SetFloatValue(OBJECT_FIELD_SCALE_X,scale);	
	go->SetUInt32Value(GAMEOBJECT_FLAGS, flags);
	// disparu 332.11403
	//go->SetFloatValue(GAMEOBJECT_POS_X, x);
	//go->SetFloatValue(GAMEOBJECT_POS_Y, y);
	//go->SetFloatValue(GAMEOBJECT_POS_Z, z);
	//go->SetFloatValue(GAMEOBJECT_FACING, o);
	go->SetInstanceID(m_mapMgr->GetInstanceID());
	go->m_battleground = this;

	return go;
}
//--------------------------------------------------------------------------------------------
void CBattleground::SendChatMessage(uint32 Type, uint64 Guid, const char * Format, ...)
{
	char msg[500];
	va_list ap;
	va_start(ap, Format);
	vsnprintf(msg, 500, Format, ap);
	va_end(ap);
	WorldPacket * data = sChatHandler.FillMessageData(Type, 0, msg, Guid, 0);
	DistributePacketToAll(data);
	delete data;
}
//--------------------------------------------------------------------------------------------
void CBattleground::DistributePacketToAll(WorldPacket * packet)
{
	m_mainLock.Acquire();
	for(int i = 0; i < 2; ++i)
	{
		if( !m_players[i].size() )	continue;

		for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
			(*itr)->GetSession()->SendPacket(packet);
	}
	m_mainLock.Release();
}
//--------------------------------------------------------------------------------------------
void CBattleground::DistributePacketToTeam(WorldPacket * packet, uint32 Team)
{
	m_mainLock.Acquire();

	if( !m_players[Team].size() )
	{
		m_mainLock.Release();
		return;
	}

	for(set<Player*>::iterator itr = m_players[Team].begin(); itr != m_players[Team].end(); ++itr)
		(*itr)->GetSession()->SendPacket(packet);
	m_mainLock.Release();
}
//--------------------------------------------------------------------------------------------
void CBattleground::PlaySoundToAll(uint32 Sound)
{
	WorldPacket data(SMSG_PLAY_SOUND, 4);
	data << Sound;
	DistributePacketToAll(&data);
}
//--------------------------------------------------------------------------------------------
void CBattleground::PlaySoundToTeam(uint32 Team, uint32 Sound)
{
	WorldPacket data(SMSG_PLAY_SOUND, 4);
	data << Sound;
	DistributePacketToTeam(&data, Team);
}
//--------------------------------------------------------------------------------------------
void CBattlegroundManager::SendBattlegroundQueueStatus(Player* plr, uint32 queueSlot)
{
	if( queueSlot > 2 ) return;

	//Log.Warning("SendBattlegroundQueueStatus", "Sending updated Battleground queues for %u.", queueSlot);

	WorldPacket data(SMSG_BATTLEFIELD_STATUS, 32);
	if( plr->m_bg && plr->m_bgSlot == queueSlot)
	{
		// Perform a manual update: this BG
		data << uint16(0); // 3.3
		data << uint32(queueSlot);
		data << uint8(0) << uint8(2);
		data << plr->m_bg->GetType();
		data << uint16(0x1F90);
		data << plr->m_bg->GetMapMgr()->GetInstanceID();
		data << uint8(plr->m_bg->IsArena() ? 1 : 0);
		data << uint32(3);
		data << plr->m_bg->GetMapMgr()->GetMapId();
		data << uint32(0);
		data << uint32(60);
		data << uint8(1); // Normal BG queue, 0 = arena?
		plr->GetSession()->SendPacket(&data);
		return;
	}
	// We're no longer queued!
	if( !plr->m_bgIsQueued[queueSlot] && !plr->m_pendingBattleground[queueSlot])
	{
		//Log.Warning("SendBattlegroundQueueStatus", "No queue slot active for %u.", queueSlot);
		data << uint32(queueSlot) << uint64(0);
		plr->GetSession()->SendPacket(&data);
		return;
	}

	uint32 Type = plr->m_bgQueueType[ queueSlot ];

    if( Type >= BATTLEGROUND_NUM_TYPES ) 
	{
		Log.Error("SendBattlegroundQueueStatus","BattlegroundType Hors limite.... (%u) <--- Reports this to devs.",Type);
		return; 
	}

	// Arena queuesss
	if( IsArena(Type) )
	{
		data << plr->m_bgTeam;
		switch(Type)
		{
		case BATTLEGROUND_ARENA_2V2:
			data << uint8(2);
			break;

		case BATTLEGROUND_ARENA_3V3:
			data << uint8(3);
			break;

		case BATTLEGROUND_ARENA_5V5:
			data << uint8(5);
			break;
		}
		data << uint8(0xA);
		data << uint32(6);
		data << uint16(0x1F90);
		data << uint32(11);
		data << uint8(plr->m_bgRatedQueue ? 1 : 0); // Rated?
	}
	else
	{
		data << uint32(queueSlot);
		data << uint8(0) << uint8(2);
		data << Type;
		data << uint16(0x1F90);
		data << plr->m_bgQueueInstanceId[queueSlot];
		data << uint8(0);
	}
	
	// Im in a BG
	if( plr->m_bg )
	{
		// Should've been handled already :P
		//Log.Warning("SendBattlegroundQueueStatus", "Player %s deja en BG.",plr->GetName());
		return;
	}

	data << uint16(0); // 3.3

	// We're clear to join!
	if( plr->m_pendingBattleground[queueSlot] )
	{
		data << uint32(2);
		data << plr->m_pendingBattleground[queueSlot]->GetMapMgr()->GetMapId();
		data << uint32(0); // Time
		plr->GetSession()->SendPacket(&data);
		return;
	}

	data << uint32(1);
	// And we're waiting...
	data << uint32(GetAverageQueueTime(Type)*1000);		// average time in msec
	data << uint32(0);
	plr->GetSession()->SendPacket(&data);

	//Log.Warning("SendBattlegroundQueueStatus", "Send packet to Player %s.",plr->GetName());
	
}
//--------------------------------------------------------------------------------------------
/*
void CBattlegroundManager::SendBattlefieldStatus(Player * plr, uint32 Status, uint32 Type, uint32 InstanceID, uint32 Time, uint32 MapId, uint8 RatedMatch)
{
	WorldPacket data(SMSG_BATTLEFIELD_STATUS, 30);
	if(Status == 0)
		data << uint64(0) << uint32(0);
	else
	{
		if(IsArena(Type))
		{
			data << uint32(plr->m_bgTeam);
			switch(Type)
			{
			 case BATTLEGROUND_ARENA_2V2: data << uint8(2); break;
			 case BATTLEGROUND_ARENA_3V3: data << uint8(3); break;
			 case BATTLEGROUND_ARENA_5V5: data << uint8(5); break;
			}
			data << uint8(0x0A); // Ascent 332 (data << uint8(0xC); OBSOLETE ?? )
			data << uint32(6);
			data << uint16(0x1F90);
			data << uint32(11);
			data << uint8(RatedMatch);	// 1 = rated match
		}
		else
		{
			data << uint32(0); // data << uint32(queueSlot);
			data << uint8(0) << uint8(2);
			data << Type;
			data << uint16(0x1F90);
			data << InstanceID;
			data << uint8(plr->m_bgTeam);
		}
		
		data << Status;

		switch(Status)
		{                    // 1 Heure max d'attente
		case 1:	data << uint32(60);
			    data << uint32(0); // 1 Arene, 0 BG
				break; // Waiting in queue
		case 2:	data << MapId << Time; break;           // Ready to join!
		case 3:
			if(IsArena(Type)) data << MapId << uint32(0) << Time << uint8(0);
			else              data << MapId << uint32(0) << Time << uint8(1);
			break;
		}
	}

	plr->GetSession()->SendPacket(&data);
}
*/
//--------------------------------------------------------------------------------------------
void CBattleground::RemovePlayer(Player * plr, bool logout)
{
    if(!plr->IsPlayer()) return;

	WorldPacket data(SMSG_BATTLEGROUND_PLAYER_LEFT, 30);
	data << plr->GetGUID();

	m_mainLock.Acquire();

	m_players[plr->m_bgTeam].erase(plr);
	if ( plr->m_isGmInvisible == false )
	{
		//Dont show invisble gm's leaving the game.
		DistributePacketToAll(&data);
	}

	memset(&plr->m_bgScore, 0, sizeof(BGScore));
	OnRemovePlayer(plr);
	plr->m_bg = 0;

	plr->FullHPMP();

	/* are we in the group? */
	if(plr->GetGroup() == m_groups[plr->m_bgTeam])
		plr->GetGroup()->RemovePlayer( plr->m_playerInfo );

	// reset team
	plr->ResetTeam();

	/* revive the player if he is dead */
	if(!plr->isAlive() && !logout)
	{
		plr->SetUInt32Value(UNIT_FIELD_HEALTH, plr->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
		plr->ResurrectPlayer(NULL);
	}
	
	/* remove buffs */ // Fix ArcEmuV53
	plr->RemoveAura(32727); // Arena preparation
	plr->RemoveAura(44521); // BG preparation

	/* teleport out */
	if(!logout)
	{
		if ( !m_ended )
		{
			SpellEntry *spellInfo = dbcSpell.LookupEntryForced( BG_DESERTER );// escape from BG
			if ( spellInfo )
			{
				SpellCastTargets targets;
				targets.m_target = plr; // targets.m_unitTarget = plr->GetGUID();
				Spell* sp(new Spell(plr,spellInfo,true,NULL));
				if ( sp != NULL ) 
				{
					sp->prepare(&targets);
					plr->SetAurDuration(BG_DESERTER,60000*15);
				}
			}
		}

		if(!IS_INSTANCE(plr->m_bgEntryPointMap))
		{
			LocationVector vec(plr->m_bgEntryPointX, plr->m_bgEntryPointY, plr->m_bgEntryPointZ, plr->m_bgEntryPointO);
			plr->SafeTeleport(plr->m_bgEntryPointMap, plr->m_bgEntryPointInstance, vec);
		}
		else
		{
			LocationVector vec(plr->GetBindPositionX(), plr->GetBindPositionY(), plr->GetBindPositionZ());
			plr->SafeTeleport(plr->GetBindMapId(), 0, vec);
		}

		//BattlegroundManager.SendBattlefieldStatus(plr, 0, 0, 0, 0, 0,0);
		BattlegroundManager.SendBattlegroundQueueStatus(plr, 0);
		BattlegroundManager.SendBattlegroundQueueStatus(plr, 1);
		BattlegroundManager.SendBattlegroundQueueStatus(plr, 2);

		/* send some null world states */
		data.Initialize(SMSG_INIT_WORLD_STATES);
		data << uint32(plr->GetMapId()) << uint32(0) << uint32(0);
		plr->GetSession()->SendPacket(&data);
	}

	if(!m_ended && m_players[0].size() == 0 && m_players[1].size() == 0)
	{
		/* create an inactive event */
		sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);						// 10mins
		sEventMgr.AddEvent(this, &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 600000, 1,0);
	}

	plr->m_bgTeam=plr->GetTeam();
	m_mainLock.Release();
}
//--------------------------------------------------------------------------------------------
void CBattleground::SendPVPData(Player * plr)
{
	m_mainLock.Acquire();

	if( m_players[FACTION_ALLY].size() == 0 && m_players[FACTION_HORDE].size() == 0 )
	{
		m_mainLock.Release();
		return;
	}

	/*if(m_type >= BATTLEGROUND_ARENA_2V2 && m_type <= BATTLEGROUND_ARENA_5V5)
	{
		m_mainLock.Release();
		return;
	}
	else
	{*/
		WorldPacket data(10*(m_players[FACTION_ALLY].size()+m_players[FACTION_HORDE].size())+50);
		BuildPvPUpdateDataPacket(&data);
		plr->GetSession()->SendPacket(&data);
	/*}*/
	
	m_mainLock.Release();
}
//--------------------------------------------------------------------------------------------
void CBattleground::EventCreate()
{
	OnCreate();
}
//--------------------------------------------------------------------------------------------
int32 CBattleground::event_GetInstanceID()
{
	return m_mapMgr->GetInstanceID();
}
//--------------------------------------------------------------------------------------------
void CBattleground::EventCountdown()
{
	if(m_countdownStage == 1)
	{
		m_countdownStage = 2;
		SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "Une minute avant la bataille pour %s !", GetName() );
	}
	else if(m_countdownStage == 2)
	{
		m_countdownStage = 3;
		SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "Trente secondes avant la bataille pour %s !", GetName() );
	}
	else if(m_countdownStage == 3)
	{
		m_countdownStage = 4;
		SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "Quinze secondes avant la bataille pour %s !", GetName() );
		sEventMgr.ModifyEventTime(this, EVENT_BATTLEGROUND_COUNTDOWN, 15000);
		sEventMgr.ModifyEventTimeLeft(this, EVENT_BATTLEGROUND_COUNTDOWN, 15000);
	}
	else
	{
		SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "La bataille pour %s commence!", GetName() );
		sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_COUNTDOWN);
		Start();
	}
}
//--------------------------------------------------------------------------------------------
void CBattleground::Start()
{
	OnStart();
	m_startTime = (uint32)UNIXTIME;
}
//--------------------------------------------------------------------------------------------

void CBattleground::SetWorldState(uint32 Index, uint32 Value)
{
	//Log.Warning("CBattleground::SetWorldState","Update BattleGround Worldstate, Index: %u, Value: %u",Index,Value);
	m_mapMgr->GetDBStateManager().UpdateWorldState(Index,Value);
}
//--------------------------------------------------------------------------------------------
void CBattleground::Close()
{
	/* remove all players from the battleground */
	m_mainLock.Acquire();
	m_ended = true;
	for(uint32 i = 0; i < 2; ++i)
	{
		set<Player*>::iterator itr;
		set<uint32>::iterator it2;
		uint32 guid;
		Player * plr;
		for(itr = m_players[i].begin(); itr != m_players[i].end();)
		{
			plr = *itr;
			++itr;
			RemovePlayer(plr, false);
		}
        
		for(it2 = m_pendPlayers[i].begin(); it2 != m_pendPlayers[i].end();)
		{
			guid = *it2;
			++it2;
			plr = objmgr.GetPlayer(guid);

			if(plr)
				RemovePendingPlayer(plr);
			else
				m_pendPlayers[i].erase(guid);
		}
	}

	/* call the virtual onclose for cleanup etc */
	OnClose();

	/* shut down the map thread. this will delete the battleground from the corrent context. */
	m_mapMgr->SetThreadState(THREADSTATE_TERMINATE);
	//m_mapMgr->Terminate();

	m_mainLock.Release();
}
//--------------------------------------------------------------------------------------------
Creature * CBattleground::SpawnSpiritGuide(float x, float y, float z, float o, uint32 horde)
{
	if(horde > 1)
		horde = FACTION_HORDE;

	CreatureInfo * pInfo = CreatureNameStorage.LookupEntry(13116 + horde);
	if(pInfo == 0)
	{
		return NULL;
	}

	Creature * pCreature = m_mapMgr->CreateCreature(pInfo->Id);

	pCreature->Create(pInfo->Name, m_mapMgr->GetMapId(), x, y, z, o);

	pCreature->SetInstanceID(m_mapMgr->GetInstanceID());
	pCreature->SetUInt32Value(OBJECT_FIELD_ENTRY, 13116 + horde);
	pCreature->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);

	pCreature->SetUInt32Value(UNIT_FIELD_HEALTH, 100000);
	pCreature->SetUInt32Value(UNIT_FIELD_POWER1, 4868);     // Mana
	pCreature->SetUInt32Value(UNIT_FIELD_POWER3, 200);      // Focus ??
	pCreature->SetUInt32Value(UNIT_FIELD_POWER5, PET_HAPPY_VALUE); // 2000000);  // Pet Hapiness ??

	pCreature->SetUInt32Value(UNIT_FIELD_LEVEL, sWorld.m_levelCap); // 60); // Test (Branruz)
	pCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 84 - horde);
	//pCreature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0 | (2 << 8) | (1 << 16));
	pCreature->SetByte(UNIT_FIELD_BYTES_0,0,0); 
	pCreature->SetByte(UNIT_FIELD_BYTES_0,1,PALADIN); // Classe ??
	pCreature->SetByte(UNIT_FIELD_BYTES_0,2,1);
	pCreature->SetByte(UNIT_FIELD_BYTES_0,3,0); // Type Mana

	pCreature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 22802);

	pCreature->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLUS_MOB | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_UNKNOWN_10 | UNIT_FLAG_PVP); // 4928
/* Standby, a faire plus tard
	pCreature->SetUInt32Value(UNIT_FIELD_AURA, 22011);
	pCreature->SetUInt32Value(UNIT_FIELD_AURAFLAGS, 9);
	pCreature->SetUInt32Value(UNIT_FIELD_AURALEVELS, 0x3C);
	pCreature->SetUInt32Value(UNIT_FIELD_AURAAPPLICATIONS, 0xFF);
*/
	pCreature->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, 2000);
	pCreature->SetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME, 2000);
	pCreature->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 0.208f);
	pCreature->SetFloatValue(UNIT_FIELD_COMBATREACH, 1.5f);

	pCreature->SetUInt32Value(UNIT_FIELD_DISPLAYID, 13337 + horde);
	pCreature->SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, 13337 + horde);

	pCreature->SetUInt32Value(UNIT_CHANNEL_SPELL, 22011);
	pCreature->SetUInt32Value(UNIT_MOD_CAST_SPEED, 1065353216);

	pCreature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPIRITHEALER);
	pCreature->SetUInt32Value(UNIT_FIELD_BYTES_2, 1 | (0x10 << 8));

	pCreature->SetPvPFlag();
	pCreature->SetPhase(1);

	pCreature->DisableAI(); // A voir (Branruz)

	pCreature->PushToWorld(m_mapMgr);
	return pCreature;
}
//--------------------------------------------------------------------------------------------
void CBattleground::QueuePlayerForResurrect(Player * plr, Creature * spirit_healer)
{
	m_mainLock.Acquire();
	map<Creature*,set<uint32> >::iterator itr = m_resurrectMap.find(spirit_healer);
	if(itr != m_resurrectMap.end())
	{
		itr->second.insert(plr->GetLowGUID());
	    plr->m_areaSpiritHealer_guid=spirit_healer->GetGUID();
        plr->CastSpell(plr,__Waiting_to_Resurrect,true); // 2584 (332.11403)
	}
	m_mainLock.Release();
}
//--------------------------------------------------------------------------------------------
void CBattleground::RemovePlayerFromResurrect(Player * plr, Creature * spirit_healer)
{
	m_mainLock.Acquire();
	map<Creature*,set<uint32> >::iterator itr = m_resurrectMap.find(spirit_healer);
	if(itr != m_resurrectMap.end())
		itr->second.erase(plr->GetLowGUID());
	plr->m_areaSpiritHealer_guid=0;
	m_mainLock.Release();
}
//--------------------------------------------------------------------------------------------
void CBattleground::AddSpiritGuide(Creature * pCreature)
{
	if (pCreature == NULL)
		return;
	m_mainLock.Acquire();
	map<Creature*,set<uint32> >::iterator itr = m_resurrectMap.find(pCreature);
	if(itr == m_resurrectMap.end())
	{
		set<uint32> ti;
		m_resurrectMap.insert(make_pair(pCreature,ti));
	}
	m_mainLock.Release();
}
//--------------------------------------------------------------------------------------------
void CBattleground::RemoveSpiritGuide(Creature * pCreature)
{
	m_mainLock.Acquire();
	m_resurrectMap.erase(pCreature);
	m_mainLock.Release();
}
//--------------------------------------------------------------------------------------------
void CBattleground::EventResurrectPlayers()
{
	m_mainLock.Acquire();
	Player * plr;
	set<uint32>::iterator itr;
	map<Creature*,set<uint32> >::iterator i;
	WorldPacket data(50);
	for(i = m_resurrectMap.begin(); i != m_resurrectMap.end(); ++i)
	{
		for(itr = i->second.begin(); itr != i->second.end(); ++itr)
		{
			plr = m_mapMgr->GetPlayer(*itr);
			if(plr && plr->isDead())
			{
                data.Initialize(SMSG_SPELL_START);                   
				data << plr->GetNewGUID() ;
				data << plr->GetNewGUID() ;
				data << uint32(RESURRECT_SPELL); // __Test_NPC_Resurrection_Rank_1
				data << uint8(0);
				data << uint16(0);
				data << uint32(0);
				data << uint16(2);
				data << plr->GetGUID();
				plr->SendMessageToSet(&data, true);

				data.Initialize(SMSG_SPELL_GO);                    
				data << plr->GetNewGUID(); 
				data << plr->GetNewGUID();
				data << uint32(RESURRECT_SPELL);  // __Test_NPC_Resurrection_Rank_1
				data << uint8(0);
				data << uint8(1);
				data << uint8(1);
				data << plr->GetGUID();
				data << uint8(0);
				data << uint16(2);
				data << plr->GetGUID();
				plr->SendMessageToSet(&data, true);

				plr->ResurrectPlayer(NULL);
				plr->SpawnCorpseBones();
				plr->SetUInt32Value(UNIT_FIELD_HEALTH, plr->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				plr->SetUInt32Value(UNIT_FIELD_POWER1, plr->GetUInt32Value(UNIT_FIELD_MAXPOWER1)); // Mana
				plr->SetUInt32Value(UNIT_FIELD_POWER4, plr->GetUInt32Value(UNIT_FIELD_MAXPOWER4)); // Energie
				plr->CastSpell(plr, BG_REVIVE_PREPARATION, true);
			}
		}
		i->second.clear();
	}
	m_lastResurrect = (uint32)UNIXTIME;
	m_mainLock.Release();
}
//--------------------------------------------------------------------------------------------
void CBattlegroundManager::HandleArenaJoin(WorldSession * m_session, uint32 BattlegroundType, uint8 as_group, uint8 rated_match)
{
	Player* plr = m_session->GetPlayer();
	uint32 pguid = plr->GetLowGUID();
	uint32 lgroup = GetLevelGrouping(plr->getLevel());
	if(as_group && plr->GetGroup() == NULL)
		return;

	// Remove from all existing queues
	BattlegroundManager.RemovePlayerFromQueues(plr);

	Group * pGroup = plr->GetGroup();
	if(as_group)
	{
		if(pGroup->GetSubGroupCount() != 1)
		{
			m_session->SystemMessage("Désolé, les groupes de raid joignant des champs de bataille sont actuellement non supportés.");
			return;
		}
		if(pGroup->GetLeader() != plr->m_playerInfo)
		{
			m_session->SystemMessage("Vous devez être le capitaine de l'équipe pour ajouter un groupe à une arène.");
			return;
		}

		bool isQueued = false;
		GroupMembersSet::iterator itx;
		if(!rated_match)
		{
			/* add all players normally.. bleh ;P */
			pGroup->Lock();
			for(itx = pGroup->GetSubGroup(0)->GetGroupMembersBegin(); itx != pGroup->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
			{
				if((*itx)->m_loggedInPlayer)
				{
                  if((*itx)->m_loggedInPlayer->m_bgIsQueued[0] ||
					 (*itx)->m_loggedInPlayer->m_bgIsQueued[1] || 
					 (*itx)->m_loggedInPlayer->m_bgIsQueued[2] )  isQueued = true;

                if(isQueued && !(*itx)->m_loggedInPlayer->m_bg)
					HandleArenaJoin((*itx)->m_loggedInPlayer->GetSession(), BattlegroundType, 0, 0);
				}
			}
			pGroup->Unlock();
			return;
		}
		else
		{
			uint32 teamType = 0;
			/* make sure all players are 70 */
			uint32 maxplayers;
			switch(BattlegroundType)
			{
			 default:
			 case BATTLEGROUND_ARENA_2V2:
				teamType   = ARENA_TEAM_TYPE_2V2;
				maxplayers = 2;
				break;

			 case BATTLEGROUND_ARENA_3V3:
				teamType   = ARENA_TEAM_TYPE_3V3;
				maxplayers = 3;
				break;

			 case BATTLEGROUND_ARENA:
			 case BATTLEGROUND_OGRIMAR_ARENA:
			 case BATTLEGROUND_DALARAN_ARENA:
			 case BATTLEGROUND_ARENA_5V5:
                teamType   = ARENA_TEAM_TYPE_5V5;
				maxplayers = 5;
				break;

			}

            ArenaTeam * pTargetTeam = NULL;
			pTargetTeam = plr->m_playerInfo->arenaTeam[teamType];
			if(!pTargetTeam)
			{
				m_session->SystemMessage("Vous devez etre membre d'une equipe d'arene!");
				return;
			}

			pGroup->Lock();
			for(itx = pGroup->GetSubGroup(0)->GetGroupMembersBegin(); itx != pGroup->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
			{
				if(maxplayers==0)
				{
					m_session->SystemMessage("Vous avez trop de joueurs dans votre groupe pour joindre ce type d'arène.");
					pGroup->Unlock();
					return;
				}

				if((*itx)->lastLevel < 70) // A FINIR, level max 250, etablir une regle... (Branruz)
				{
					m_session->SystemMessage("Désolé, certains membres de votre groupe ne sont pas de niveau 70.");
					pGroup->Unlock();
					return;
				}

				if((*itx)->m_loggedInPlayer)
				{
					if( !(*itx)->m_loggedInPlayer->IsInWorld() )
					{
						m_session->SystemMessage("Certains de vos membres ont deconnectes.");
						pGroup->Unlock();
						return;
					}

					bool isQueued = false;
					if((*itx)->m_loggedInPlayer->m_bgIsQueued[0] || 
					   (*itx)->m_loggedInPlayer->m_bgIsQueued[1] || 
					   (*itx)->m_loggedInPlayer->m_bgIsQueued[2] ) isQueued = true;

					if((*itx)->m_loggedInPlayer->m_bg || isQueued)
					{
						m_session->SystemMessage("Un ou plusieurs membres de votre groupe sont déjà en file d'attente ou à l'intérieur d'un champ de bataille.");
						pGroup->Unlock();
						return;
					}

					--maxplayers;
				}
				if(pTargetTeam && (*itx)->arenaTeam[teamType] != pTargetTeam)
				{
					m_session->SystemMessage("Un ou plusieurs membres de votre groupe ne sont pas dans une equipe d'arene.");
					pGroup->Unlock();
					return;
				}
			}
			WorldPacket data(SMSG_GROUP_JOINED_BATTLEGROUND, 4);
			data << uint32(6);	// all arenas, BATTLEGROUND_ARENA_5V5

			for(itx = pGroup->GetSubGroup(0)->GetGroupMembersBegin(); itx != pGroup->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
			{
				if((*itx)->m_loggedInPlayer)
				{
					//SendBattlefieldStatus((*itx)->m_loggedInPlayer, 1, BattlegroundType, 0 , 0, 0,1);
					(*itx)->m_loggedInPlayer->m_bgIsQueued[0] = true;
					(*itx)->m_loggedInPlayer->m_bgQueueInstanceId[0] = 0;
					(*itx)->m_loggedInPlayer->m_bgQueueType[0] = BattlegroundType;
					(*itx)->m_loggedInPlayer->m_bgQueueTime[0] = (uint32)UNIXTIME;
					(*itx)->m_loggedInPlayer->GetSession()->SendPacket(&data);
					(*itx)->m_loggedInPlayer->m_bgEntryPointX=(*itx)->m_loggedInPlayer->GetPositionX();
					(*itx)->m_loggedInPlayer->m_bgEntryPointY=(*itx)->m_loggedInPlayer->GetPositionY();
					(*itx)->m_loggedInPlayer->m_bgEntryPointZ=(*itx)->m_loggedInPlayer->GetPositionZ();
					(*itx)->m_loggedInPlayer->m_bgEntryPointMap=(*itx)->m_loggedInPlayer->GetMapId();
					(*itx)->m_loggedInPlayer->m_bgRatedQueue = rated_match ? true : false;
					SendBattlegroundQueueStatus( (*itx)->m_loggedInPlayer, 0);
				}
			}

			pGroup->Unlock();
			pGroup->m_isqueued = true;

			m_queueLock.Acquire();
			m_queuedGroups[BattlegroundType].push_back(pGroup->GetID());
			m_queueLock.Release();
			Log.Success("BattlegroundMgr", "Le groupe %u est maintenant en file d'attente du champ de bataille pour l'arène de type %u", pGroup->GetID(), BattlegroundType);

			/* send the battleground status packet */

			return;
		}
	}
	

	/* Queue him! */
	m_queueLock.Acquire();
	m_queuedPlayers[BattlegroundType][lgroup].push_back(pguid);
	Log.Success("BattlegroundMgr", "Le joueur %u est maintenant dans la file d'attente du champ de bataille pour {L'arène %u}", m_session->GetPlayer()->GetLowGUID(), BattlegroundType );

	/* send the battleground status packet */
	//SendBattlefieldStatus(m_session->GetPlayer(), 1, BattlegroundType, 0 , 0, 0,0);
	plr->m_bgIsQueued[0] = true;
	plr->m_bgQueueInstanceId[0] = 0;
	plr->m_bgQueueType[0] = BattlegroundType;
	plr->m_bgRatedQueue = false;
	plr->m_bgQueueTime[0] = (uint32)UNIXTIME;
	SendBattlegroundQueueStatus( plr, 0);

	/* Set battleground entry point */
	plr->m_bgEntryPointX = plr->GetPositionX();
	plr->m_bgEntryPointY = plr->GetPositionY();
	plr->m_bgEntryPointZ = plr->GetPositionZ();
	plr->m_bgEntryPointMap = plr->GetMapId();
	plr->m_bgEntryPointInstance = plr->GetInstanceID();

	m_queueLock.Release();
}
//--------------------------------------------------------------------------------------------
bool CBattleground::CanPlayerJoin(Player * plr)
{
	//return HasFreeSlots(plr->m_bgTeam);

	return ( plr->bGMTagOn || 
		     (HasFreeSlots(plr->m_bgTeam) && (GetLevelGrouping(plr->getLevel()) == GetLevelGroup()) && (!plr->HasAura(BG_DESERTER))) );

}
//--------------------------------------------------------------------------------------------
void CBattleground::GiveHonorToTeam(uint32 team, uint32 amt)
{
	for(set<Player *>::iterator itx = m_players[team].begin(); itx != m_players[team].end(); ++itx)
	{
		Player *plr = (*itx);
		if(!plr) continue;

		plr->m_bgScore.BonusHonor += amt;
		HonorHandler::AddHonorPointsToPlayer( plr, amt);
	}
}
//--------------------------------------------------------------------------------------------
bool CBattleground::HookSlowLockOpen(GameObject *pGo, Player *pPlayer, Spell *pSpell)
{
	if( pPlayer->m_CurrentVehicle )
		pPlayer->m_CurrentVehicle->RemovePassenger(pPlayer);

	if( pPlayer->m_stealth )
		pPlayer->RemoveAura( pPlayer->m_stealth );

	return false;
}
//--------------------------------------------------------------------------------------------
void CBattleground::QueueAtNearestSpiritGuide(Player* plr, Creature* old)
{
	float dd;
	float dist = 999999.0f;
	Creature* cl = NULL;
	set<uint32> *closest = NULL;
	m_lock.Acquire();
	map<Creature*, set<uint32> >::iterator itr = m_resurrectMap.begin();
	for(; itr != m_resurrectMap.end(); ++itr)
	{
		if( itr->first == old )
			continue;

		dd = plr->GetDistance2dSq(itr->first) < dist;
		if( dd < dist )
		{
			cl = itr->first;
			closest = &itr->second;
			dist = dd;
		}
	}

	if( closest != NULL )
	{
		closest->insert(plr->GetLowGUID());
		plr->m_areaSpiritHealer_guid=cl->GetGUID();
		plr->CastSpell(plr,2584,true);
	}

	m_lock.Release();
}