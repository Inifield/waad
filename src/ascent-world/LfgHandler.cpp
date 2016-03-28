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

void WorldSession::HandleSetLookingForGroupComment(WorldPacket& recvPacket)
{
	std::string comment;
		
	recvPacket >> comment;
	
	GetPlayer()->Lfgcomment = comment;	
}

void WorldSession::HandleEnableAutoJoin(WorldPacket& recvPacket)
{
	uint32 lfgRoles;
	uint8 i, dungeonsCount, counter2, GetLFGInfoLocal;
    std::string comment;
    std::vector<uint32> dungeons;

    recvPacket >> lfgRoles; // lfg roles
    recvPacket >> GetLFGInfoLocal; // lua: GetLFGInfoLocal
    recvPacket >> GetLFGInfoLocal; // lua: GetLFGInfoLocal

    recvPacket >> dungeonsCount;

    dungeons.resize(dungeonsCount);

    for (i = 0; i < dungeonsCount; ++i)
        recvPacket >> dungeons[i]; // dungeons id/type

    recvPacket >> counter2; // const count = 3, lua: GetLFGInfoLocal

    for (i = 0; i < counter2; ++i)
        recvPacket >> GetLFGInfoLocal; // lua: GetLFGInfoLocal

    recvPacket >> comment; // lfg comment

    //SendLfgJoinResult(ERR_LFG_OK);
    //SendLfgUpdate(false, LFG_UPDATE_JOIN, dungeons[0]);
/*
	// make sure they're not queued in any invalid cases
	for(i = 0; i < MAX_LFG_QUEUE_ID; ++i)
	{
		if(_player->LfgDungeonId[i] != 0)
		{
			if(LfgDungeonTypes[_player->LfgDungeonId[i]] != LFG_INSTANCE && LfgDungeonTypes[_player->LfgDungeonId[i]] != LFG_HEROIC_DUNGEON)
			{
				return;
			}
		}
	}

	// enable autojoin, join any groups if possible.
	_player->m_Autojoin = true;
	
	for(i = 0; i < MAX_LFG_QUEUE_ID; ++i)
	{
		if(_player->LfgDungeonId[i] != 0)
		{
			_player->SendMeetingStoneQueue(_player->LfgDungeonId[i], 1);
			sLfgMgr.UpdateLfgQueue(_player->LfgDungeonId[i]);
		}
	}
*/
}

void WorldSession::HandleDisableAutoJoin(WorldPacket& recvPacket)
{
	uint32 i;
	_player->m_Autojoin = false;

	for(i = 0; i < MAX_LFG_QUEUE_ID; ++i)
	{
		if(_player->LfgDungeonId[i] != 0)
		{
			if(LfgDungeonTypes[_player->LfgDungeonId[i]] == LFG_DUNGEON || LfgDungeonTypes[_player->LfgDungeonId[i]] == LFG_HEROIC_DUNGEON)
				_player->SendMeetingStoneQueue(_player->LfgDungeonId[i], 0);
		}
	}
}

void WorldSession::HandleEnableAutoAddMembers(WorldPacket& recvPacket)
{
	uint32 i;
	_player->m_AutoAddMem = true;

	for(i = 0; i < MAX_LFG_QUEUE_ID; ++i)
	{
		if(_player->LfgDungeonId[i] != 0)
		{
			sLfgMgr.UpdateLfgQueue(_player->LfgDungeonId[i]);
		}
	}
}

void WorldSession::HandleDisableAutoAddMembers(WorldPacket& recvPacket)
{
	_player->m_AutoAddMem = false;	
}

void WorldSession::HandleMsgLookingForGroup(WorldPacket& recvPacket)
{
	/* this is looking for more */
	uint32 LfgType,LfgDungeonId,unk1;
	recvPacket >> LfgType >> LfgDungeonId >> unk1;
	
	if(LfgDungeonId > MAX_DUNGEONS)
		return;

	if(LfgDungeonId)
		sLfgMgr.SendLfgList(_player, LfgDungeonId);
}

void WorldSession::HandleSetLookingForGroup(WorldPacket& recvPacket)
{
	CHECK_INWORLD_RETURN;

	DungeonSet::iterator itr;
	DungeonSet randomDungeonSet;
	LookingForGroup* dungeon = NULL;
	uint32 level = _player->getLevel();
	DungeonSet LevelDungeonSet = sLfgMgr.GetLevelSet(level);
	for (itr = LevelDungeonSet.begin(); itr != LevelDungeonSet.end(); itr++)
	{
		dungeon = dbcLookingForGroup.LookupEntry(*itr);
		if (dungeon != NULL && dungeon->type == LFG_RANDOM
			&& dungeon->expansion <= GetHighestExpansion()
			&& dungeon->minlevel <= level && level <= dungeon->maxlevel)
			randomDungeonSet.insert(dungeon->ID);
		dungeon = NULL;
	}

	// Crow: Confirmed structure below
	WorldPacket data(SMSG_LFG_PLAYER_INFO, 400);
	uint8 randomsize = (uint8)randomDungeonSet.size();
	data << randomsize;
	for(itr = randomDungeonSet.begin(); itr != randomDungeonSet.end(); itr++)
	{
		dungeon = dbcLookingForGroup.LookupEntry(*itr);
		data << uint32(dungeon->GetEntry());

		uint8 done = 0;
		Quest* QuestReward = NULL;
		LfgReward* reward = sLfgMgr.GetLFGReward(*itr);
		if(reward)
		{
			QuestReward = sQuestMgr.GetQuestPointer(reward->reward[0].QuestId);
			if(QuestReward)
			{
				done = _player->HasFinishedQuest(reward->reward[0].QuestId);
				if(!done)
					done = _player->HasFinishedDailyQuest(reward->reward[0].QuestId);
				if (done)
					QuestReward = sQuestMgr.GetQuestPointer(reward->reward[1].QuestId);
			}
		}

		if (QuestReward)
		{
			data << uint8(done);
			data << uint32(sQuestMgr.GenerateRewardMoney(_player, QuestReward));
			data << uint32(sQuestMgr.GenerateQuestXP(_player, QuestReward)*sWorld.getRate(RATE_QUESTXP));
			data << uint32(reward->reward[done].MoneyReward);
			data << uint32(reward->reward[done].XPReward);
			if(QuestReward->reward_item == NULL)
			{
				data << uint8(0);
			}
			else
			{
				data << uint8(QuestReward->count_reward_item);
				if (QuestReward->count_reward_item)
				{
					ItemPrototype* proto = NULL;
					for (uint8 i = 0; i < 4; i++)
					{
						if (!QuestReward->reward_item[i])
							continue;

						proto = ItemPrototypeStorage.LookupEntry(QuestReward->reward_item[i]);
						data << uint32(QuestReward->reward_item[i]);
						data << uint32(proto ? proto->DisplayInfoID : 0);
						data << uint32(QuestReward->reward_itemcount[i]);
					}
				}
			}
		}
		else
		{
			data << uint8(0);
			data << uint32(0);
			data << uint32(0);
			data << uint32(0);
			data << uint32(0);
			data << uint8(0);
		}
	}
	SendPacket(&data);
}

void WorldSession::HandleLFDPartyLockOpcode( WorldPacket& recv_data )
{
	// Crow: Confirmed structure below
	WorldPacket data(SMSG_LFG_PARTY_INFO, 400);
	uint8 cnt = 0;
	data << uint8(cnt);
	for(uint8 i = 0; i < cnt; i++)
	{
		data << uint64(0);
		uint32 count = 0;
		data << count;
		for(uint32 i = 0; i < count; i++)
		{
			data << uint32(0);
			data << uint32(0);
		}
	}
	SendPacket(&data);
}
void WorldSession::HandleSetLookingForMore(WorldPacket& recvPacket)
{
	uint16 LfgDungeonId;
	uint8 LfgType,unk1;

	recvPacket >> LfgDungeonId >> unk1 >> LfgType;
	if( LfgDungeonId >= MAX_DUNGEONS )
		return;

	// remove from an existing queue
	if(LfgDungeonId != _player->LfmDungeonId && _player->LfmDungeonId)
		sLfgMgr.RemovePlayerFromLfmList(_player, _player->LfmDungeonId);

	_player->LfmDungeonId = LfgDungeonId;
	_player->LfmType = LfgType;

	if(LfgDungeonId)
		sLfgMgr.SetPlayerInLfmList(_player, LfgDungeonId);
}

void WorldSession::HandleLfgClear(WorldPacket & recvPacket)
{
	sLfgMgr.RemovePlayerFromLfgQueues(_player);
	
}
//ArcEmu
void WorldSession::HandleMeetingStoneInfo(WorldPacket & recvPacket)
{
	_player->SendMeetingStoneQueue(0,6); //values drawn from packet logs, don't appear to change
}
