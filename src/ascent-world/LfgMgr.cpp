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

uint32 LfgDungeonTypes[MAX_DUNGEONS];
initialiseSingleton( LfgMgr );

LfgMgr::LfgMgr()
{
	MaxDungeonID = 0;
	uint32 levelgroup[2];
	levelgroup[0] = LFG_LEVELGROUP_NONE;
	levelgroup[1] = LFG_LEVELGROUP_NONE;
	DBCStorage<LookingForGroup>::iterator itr;
	for(itr = dbcLookingForGroup.begin(); itr != dbcLookingForGroup.end(); ++itr)
	{
		if((*itr)->ID > MaxDungeonID)
			MaxDungeonID = (*itr)->ID;

		levelgroup[0] = GetPlayerLevelGroup((*itr)->minlevel);
		if(levelgroup[0] != LFG_LEVELGROUP_NONE)
			DungeonsByLevel[levelgroup[0]].insert((*itr)->ID);

		levelgroup[1] = GetPlayerLevelGroup((*itr)->maxlevel);
		if(levelgroup[1] != LFG_LEVELGROUP_NONE)
			if(levelgroup[0] != levelgroup[1])
				DungeonsByLevel[levelgroup[1]].insert((*itr)->ID);

		levelgroup[0] = LFG_LEVELGROUP_NONE;
		levelgroup[1] = LFG_LEVELGROUP_NONE;
	}
}

LfgMgr::~LfgMgr()
{

}

void LfgMgr::LoadRandomDungeonRewards()
{
	uint32 count = 0;
	QueryResult* result = WorldDatabase.Query("SELECT * FROM lfd_rewards ORDER BY dungeonid");
	if(result != NULL)
	{
		do
		{
			Field* fields = result->Fetch();
			uint32 dungeonid = fields[0].GetUInt32();
			if(GetLFGReward(dungeonid))
				continue;

			uint32 questid[2], moneyreward[2], XPreward[2];
			questid[0] = questid[1] = moneyreward[0] = moneyreward[1] = XPreward[0] = XPreward[1] = 0;

			// First Reward
			questid[0] = fields[2].GetUInt32();
			moneyreward[0] = fields[3].GetUInt32();
			XPreward[0] = fields[4].GetUInt32();

			// Second reward
			questid[1] = fields[5].GetUInt32();
			moneyreward[1] = fields[6].GetUInt32();
			XPreward[1] = fields[7].GetUInt32();

			LfgReward* reward = new LfgReward(questid[0], moneyreward[0], XPreward[0], questid[1], moneyreward[1], XPreward[1]);
			DungeonRewards[dungeonid] = reward;
			count++;
		}while(result->NextRow());
	}

	Log.Notice("LfgMgr", "%u LFD rewards loaded.", count);
}

bool LfgMgr::AttemptLfgJoin(Player * pl, uint32 LfgDungeonId)
{
	if( pl == NULL )
		return false;

	if( !pl->IsInWorld() )
		return false;

	// if the player has autojoin enabled,
	// search for any groups that have auto add members enabled, also have this dungeon, and add him
	// if one is found.
	/*LfgPlayerList itr;
	Player * plr;

	// make sure the dungeon is usable by autojoin (should never be true)
	if(LfgDungeonTypes[LfgDungeonId] != LFG_INSTANCE && LfgDungeonTypes[LfgDungeonId] != LFG_HEROIC_DUNGEON)
		return false;

	m_lock.Acquire();

	for(itr = m_lookingForGroup[LfgDungeonId].begin(); itr != m_lookingForGroup[LfgDungeonId].end(); ++itr) {
		plr = *itr;
		if(plr->m_AutoAddMem) {
			if(plr->GetGroup() && !plr->GetGroup()->IsFull() && plr->GetGroup()->GetGroupType() == GROUP_TYPE_PARTY) {
				plr->GetGroup()->AddMember(pl->m_playerInfo);
				pl->SendMeetingStoneQueue(LfgDungeonId, 0);
				m_lock.Release();
				return true;
			}
		}
	}

	m_lock.Release();*/
	return false;
}

uint32 LfgMgr::GetPlayerLevelGroup(uint32 level)
{
	if(level > 80)
		return LFG_LEVELGROUP_80_UP;
	else if(level == 80)
		return LFG_LEVELGROUP_80;
	else if(level >= 70)
		return LFG_LEVELGROUP_70_UP;
	else if(level >= 60)
		return LFG_LEVELGROUP_60_UP;
	else if(level >= 50)
		return LFG_LEVELGROUP_50_UP;
	else if(level >= 40)
		return LFG_LEVELGROUP_40_UP;
	else if(level >= 30)
		return LFG_LEVELGROUP_30_UP;
	else if(level >= 20)
		return LFG_LEVELGROUP_20_UP;
	else if(level >= 10)
		return LFG_LEVELGROUP_10_UP;

	return LFG_LEVELGROUP_NONE;
}

void LfgMgr::SetPlayerInLFGqueue(Player *pl,uint32 LfgDungeonId)
{
	if( pl == NULL )
		return;

	if( LfgDungeonId >= MAX_DUNGEONS )
		return;

	m_lock.Acquire();

	// there are either no groups free or we don't have autojoin enabled, put us in the queue.
	m_lookingForGroup[LfgDungeonId].push_back(pl);

	// release the lock
	m_lock.Release();
}

void LfgMgr::RemovePlayerFromLfgQueues(Player * pl)
{
	if( pl == NULL )
		return;

	m_lock.Acquire();
	for(uint32 i = 0; i < MAX_LFG_QUEUE_ID; ++i)
	{
		if(pl->LfgDungeonId[i] != 0)
		{
			if( pl->LfgDungeonId[i] < MAX_DUNGEONS )
				m_lookingForGroup[pl->LfgDungeonId[i]].remove(pl);

			if(pl->m_Autojoin)
				pl->SendMeetingStoneQueue(pl->LfgDungeonId[i], 0);

			pl->LfgDungeonId[i] = 0;
			pl->LfgType[i] = 0;
		}
	}

	if(pl->LfmDungeonId)
	{
		if( pl->LfmDungeonId < MAX_DUNGEONS )
			m_lookingForMore[pl->LfmDungeonId].remove(pl);

		pl->LfmDungeonId=0;
		pl->LfmType=0;
	}

	m_lock.Release();
}

void LfgMgr::RemovePlayerFromLfgQueue( Player* plr, uint32 LfgDungeonId )
{
	if( plr == NULL )
		return;

	if( LfgDungeonId >= MAX_DUNGEONS )
		return;

	m_lock.Acquire();
	m_lookingForGroup[LfgDungeonId].remove( plr );
	m_lock.Release();

	if( plr->m_Autojoin )
		plr->SendMeetingStoneQueue( LfgDungeonId, 0 );
}

void LfgMgr::UpdateLfgQueue(uint32 LfgDungeonId)
{
	if( LfgDungeonId > MAX_DUNGEONS )
		return;

	LfgPlayerList possibleGroupLeaders;
	LfgPlayerList possibleMembers;
	LfgPlayerList::iterator itr;
	LfgPlayerList::iterator it2;
	LfgPlayerList::iterator it3;
	Player * plr=NULL;
	uint32 i;
	//LfgMatch * pMatch;

	// only update on autojoinable dungeons
	if(LfgDungeonTypes[LfgDungeonId] != LFG_DUNGEON && LfgDungeonTypes[LfgDungeonId] != LFG_HEROIC_DUNGEON)
		return;

	m_lock.Acquire();
	for(itr = m_lookingForGroup[LfgDungeonId].begin(); itr != m_lookingForGroup[LfgDungeonId].end(); ++itr)
	{
        plr = *itr;

		if(plr->m_lfgInviterGuid || plr->m_lfgMatch != NULL)
			continue;

		// possible member?
		if(plr->m_Autojoin)
			possibleMembers.push_back(plr);
	}

	for(itr = m_lookingForMore[LfgDungeonId].begin(); itr != m_lookingForMore[LfgDungeonId].end(); ++itr)
	{
		if(plr->GetGroup())
		{
			// check if this is a suitable candidate for a group for others to join
			if(plr->m_AutoAddMem && plr->IsGroupLeader() && !plr->GetGroup()->IsFull() && plr->GetGroup()->GetGroupType() == GROUP_TYPE_PARTY)
			{
				possibleGroupLeaders.push_back(plr);
				continue;
			}

			if(plr->m_lfgInviterGuid || plr->m_lfgMatch != NULL)
				continue;

			// just a guy in a group
			continue;
		}
	}

	// look through our members, as well as our groups and see if we can add anyone to any groups
	// if not, if we have enough members, create a new group for them.

	if(possibleMembers.size() > 0)
	{
		for(itr = possibleGroupLeaders.begin(); itr != possibleGroupLeaders.end(); ++itr)
		{
			for(it2 = possibleMembers.begin(); it2 != possibleMembers.end();)
			{
				if((*itr)->GetGroup()->IsFull())
				{
					++it2;
					continue;
				}

				// found a group for him, lets insert him.
				if((*itr)->GetGroup()->AddMember((*it2)->m_playerInfo))
				{
					(*it2)->m_lfgInviterGuid = (*itr)->GetLowGUID();

					it3 = it2;
					++it2;

					possibleMembers.erase(it2);
				}
				else
					++it2;
			}
		}
	}

	// do we still have any members left over (enough to form a group)
	while(possibleMembers.size() > 1)
	{
		/*pMatch = new LfgMatch(LfgDungeonId);
		pMatch->lock.Acquire();
		for(i = 0; i < 5, possibleMembers.size() > 0; ++i)
		{
			pMatch->PendingPlayers.insert(possibleMembers.front());
			possibleMembers.front()->SendMeetingStoneQueue(LfgDungeonId, 0);
			possibleMembers.front()->GetSession()->OutPacket(SMSG_LFG_INVITE, 4, &LfgDungeonId);
			possibleMembers.front()->m_lfgMatch = pMatch;
			possibleMembers.pop_front();
		}
		pMatch->lock.Release();*/
		Group * pGroup = new Group(true);
		for(i = 0; i < 5 && possibleMembers.size() > 0; ++i)
		{
			pGroup->AddMember( possibleMembers.front()->m_playerInfo );
			possibleMembers.front()->SendMeetingStoneQueue( LfgDungeonId, 0 );
			m_lookingForGroup[LfgDungeonId].remove( possibleMembers.front() );
			possibleMembers.pop_front();
		}
	}

	m_lock.Release();
}

void LfgMgr::SendLfgList( Player* plr, uint32 Dungeon )
{
	if( plr == NULL )
		return;

	if( Dungeon >= MAX_DUNGEONS )
		return;

	LfgPlayerList::iterator itr;
	GroupMembersSet::iterator it2;
	uint32 count = 0;
	Player * pl;
	uint32 i;
	uint64 tguid;
	SubGroup * sgrp;

	m_lock.Acquire();

	WorldPacket data(CMSG_LFD_PLAYER_LOCK_INFO_REQUEST, ((m_lookingForGroup[Dungeon].size() + m_lookingForMore[Dungeon].size()) * 20) + 20);
	data << LfgDungeonTypes[Dungeon];
	data << Dungeon;
	data << uint32(m_lookingForGroup[Dungeon].size());
	data << uint32(m_lookingForGroup[Dungeon].size());

	for(itr = m_lookingForGroup[Dungeon].begin(); itr != m_lookingForGroup[Dungeon].end(); ++itr)
	{
		pl = *itr;
		if(pl->GetTeam() != plr->GetTeam() || pl == plr)
			continue;

        ++count;
		data << pl->GetNewGUID();
		data << pl->getLevel();
		data << pl->GetZoneId();
		data << uint8(0);		// 1=LFG?

        for(i = 0; i < MAX_LFG_QUEUE_ID; ++i)
			data << pl->LfgDungeonId[i] << uint8(0) << pl->LfgType[i];

		data << pl->Lfgcomment;

		// LFG members are never in parties.
		data << uint32(0);
	}

	for(itr = m_lookingForMore[Dungeon].begin(); itr != m_lookingForMore[Dungeon].end(); ++itr)
	{
		pl = *itr;
		if(pl->GetTeam() != plr->GetTeam() || pl == plr)
			continue;

		++count;
		data << pl->GetNewGUID();
		data << pl->getLevel();
		data << pl->GetZoneId();
		data << uint8(1);		// 1=LFM?

		for(i = 0; i < MAX_LFG_QUEUE_ID; ++i)
			data << pl->LfgDungeonId[i] << uint8(0) << pl->LfgType[i];

		data << pl->Lfgcomment;

		if(pl->GetGroup() && pl->GetGroup()->GetGroupType() == GROUP_TYPE_PARTY)
		{
			pl->GetGroup()->Lock();
			sgrp = pl->GetGroup()->GetSubGroup(0);
			data << uint32(sgrp->GetMemberCount() - 1);
			for(it2 = sgrp->GetGroupMembersBegin(); it2 != sgrp->GetGroupMembersEnd(); ++it2)
			{
				if((*it2)->m_loggedInPlayer)
					data << (*it2)->m_loggedInPlayer->GetNewGUID();
				else
				{
					tguid = (*it2)->guid;
					FastGUIDPack(data, tguid);
				}
			}

			pl->GetGroup()->Unlock();
		}
		else
			data << uint32(0);
	}

	m_lock.Release();

	//*(uint32*)(data.contents()[8]) = count;
	//*(uint32*)(data.contents()[12]) = count;
	data.put(8, count);
	data.put(12, count);

    plr->GetSession()->SendPacket(&data);
}

void LfgMgr::SetPlayerInLfmList(Player * pl, uint32 LfgDungeonId)
{
	if( pl == NULL )
		return;

	if( !pl->IsInWorld() )
		return;

	if( LfgDungeonId >= MAX_DUNGEONS )
		return;

	m_lock.Acquire();
	m_lookingForMore[LfgDungeonId].push_back(pl);
	m_lock.Release();
}

void LfgMgr::RemovePlayerFromLfmList(Player * pl, uint32 LfmDungeonId)
{
	if( pl == NULL )
		return;

	if( !pl->IsInWorld() )
		return;

	if( LfmDungeonId >= MAX_DUNGEONS )
		return;

	m_lock.Acquire();
	m_lookingForMore[LfmDungeonId].remove(pl);
	m_lock.Release();
}
