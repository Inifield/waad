/*
* Aspire Hearthstone
* Copyright (C) 2008 - 2009 AspireDev <http://www.aspiredev.org/>
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
/*
//---- ArcEmu
bool SendAchievementProgress(const CriteriaProgress* c)
{
	if(c->counter <= 0) // achievement not started yet, don't send progress
		return false;
	AchievementCriteriaEntry const* acEntry = dbcAchivementCriteria.LookupEntry(c->id);

	// Exalted with X faction (don't send 12323/42000 progress, it's not shown anyway)
	if(acEntry->requiredType==ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION) return false;

	// Reach level (don't send 7/80 progress, it's not shown anyway)
	if(acEntry->requiredType==ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL) 	return false;

	return true;
}

bool IsStatisticAchievement(const AchievementEntry* a)
{
	switch(a->categoryId)
	{
		case    21: // Total deaths from other players, Total deaths from opposite faction
		case   122: // Total deaths
		case   125: // Total raid and dungeon deaths (and for each dungeon/raid type)
		case   128: // Total kills, Total kills that grant experience or honor
		case   131: // Total facepalms, etc. (Social)
		case   132: // Weapon skills at maximum skill, Professions learned, ...
		case   133: // Quests completed, Quests abandoned, Daily quests completed, ...
		case   135: // Creatures killed, Critters killed, ...
		case   136: // Total Honorable Kills, World Honorable Kills, Continent with the most Honorable Kills, ...
		case   137: // Total Killing Blows, World Killing Blows, Continent with the most Killing Blows, ...
		case   140: // Total gold acquired, Average gold earned per day, Gold looted, Gold from quest rewards, ...
		case   141: // Total damage done, Total damage received, Total healing done, Total healing received, Largest hit dealt, ...
		case   145: // Beverages consumed, Different foods eaten, ...
		case   147: // Total factions encountered, Most factions at Exalted, Most factions at Revered or higher, ...
		case   152: // Arenas won, Arenas played, Highest 5 man team rating, ...
		case   153: // Battlegrounds played, Battleground won the most, ...
		case   173: // Highest alchemy skill, Tailoring patterns learned, ...
		case   178: // Fishing daily quests completed, Cooking Recipes known, ...
		case   191: // Mounts owned, Vanity pets owned, Extra bank slots purchased, Greed rolls made on loot, ...
		case 14807: // Total 5-player dungeons entered, Total 10-player raids entered, Total 25-player raids entered, ...
		case 14821: // Edwin VanCleef kills (Deadmines), C'Thun kills (Temple of Ahn'Qiraj), ...
			return true;
		default:
			break;
	}
	return false;
}

bool SaveAchievementProgressToDB(const CriteriaProgress* c)
{
	if(c->counter <= 0) // don't save it if it's not started yet
		return false;
	AchievementCriteriaEntry const* acEntry = dbcAchievementCriteria.LookupEntry(c->id);
	switch(acEntry->requiredType)
	{
		// these get updated when character logs on, don't save to character progress db
		case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
		case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
		case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
		case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
		case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
		case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
		case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_MOUNTS:
		case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
		case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
			return false;
		default:
			break;
	}
	return true;
}

//-----
*/
set<uint32> m_completedRealmFirstAchievements;

AchievementInterface::AchievementInterface(Player  *plr)
{
	m_player = plr;
	m_achievementInspectPacket = NULL;
}

AchievementInterface::~AchievementInterface()
{
	m_player = NULL;

	if( m_achivementDataMap.size() > 0 )
	{
		std::map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.begin();
		for(; itr != m_achivementDataMap.end(); ++itr)
		{
			delete itr->second;
		}
	}

	if( m_achievementInspectPacket )
		delete m_achievementInspectPacket;
}

void AchievementInterface::LoadFromDB( QueryResult * pResult )
{
	if( !pResult ) // We have no achievements yet. :)
		return;

	do 
	{
		Field * fields = pResult->Fetch();
		uint32 achievementid = fields[2].GetUInt32();
		string criteriaprogress = fields[3].GetString();
		bool completed = (fields[4].GetUInt32() > 0);

		AchievementEntry * ae = dbcAchievement.LookupEntry( achievementid );
		AchievementData * ad = new AchievementData;
		memset(ad, 0, sizeof(AchievementData));

		ad->id = achievementid;
		ad->num_criterias = ae->AssociatedCriteriaCount;
		ad->completed = completed;
		ad->date = fields[4].GetUInt32();
		
		if( ad->completed && string(ae->name).find("Realm First!") != string::npos )
			m_completedRealmFirstAchievements.insert( ae->ID );

		vector<string> Delim = StrSplit( criteriaprogress, "," );
		for( uint32 i = 0; i < 32; ++i)
		{
			if( i >= Delim.size() )
				continue;

			string posValue = Delim[i];
			if( !posValue.size() )
				continue;

			uint32 r = atoi(posValue.c_str());
			ad->counter[i] = r;

			//printf("Loaded achievement: %u, %s\n", ae->ID, ad->completed ? "completed" : "incomplete" );
		}

		m_achivementDataMap.insert( make_pair( achievementid, ad) );
	} while ( pResult->NextRow() );
}

void AchievementInterface::SaveToDB(QueryBuffer * buffer)
{
	bool NewBuffer = false;
	if( !buffer )
	{
		buffer = new QueryBuffer;
		NewBuffer = true;
	}

	map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.begin();
	for(; itr != m_achivementDataMap.end(); ++itr)
	{
		AchievementData * ad = itr->second;
		if( !ad->m_isDirty )
			continue;

		std::stringstream ss;
		ss << "REPLACE INTO achievements (player,achievementid,progress,completed) VALUES (";
		ss << m_player->GetLowGUID() << ",";
		ss << ad->id << ",";
		ss << "'";
		for(uint32 i = 0; i < ad->num_criterias; ++i)
		{
			ss << ad->counter[i] << ",";
		}
		ss << "',";
		ss << ad->date << ")";
		buffer->AddQueryStr( ss.str().c_str() );

		ad->m_isDirty = false;
	}

	if( NewBuffer )
		CharacterDatabase.AddQueryBuffer( buffer );
}

WorldPacket* AchievementInterface::BuildAchievementData(bool forInspect)
{
	if(forInspect && m_achievementInspectPacket)
		return m_achievementInspectPacket;

	WorldPacket * data = new WorldPacket(forInspect ? SMSG_RESPOND_INSPECT_ACHIEVEMENTS : SMSG_ALL_ACHIEVEMENT_DATA, 400);
	if(forInspect)
		*data << m_player->GetNewGUID();

	std::map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.begin();
	for(; itr != m_achivementDataMap.end(); ++itr)
	{
		if( itr->second->completed )
		{
			*data << uint32(itr->first);
			*data << uint32( unixTimeToTimeBitfields(itr->second->date) );
		}
	}

	*data << uint32(0xFFFFFFFF); // (Branruz)
	itr = m_achivementDataMap.begin(); // Re-loop, luls
	for(; itr != m_achivementDataMap.end(); ++itr)
	{
		if( !itr->second->completed )
		{
			AchievementEntry * ae = dbcAchievement.LookupEntry( itr->second->id );
			// Loop over the associated criteria
			for(uint32 i = 0; i < ae->AssociatedCriteriaCount; ++i)
			{
				*data << uint32( ae->AssociatedCriteria[i] );
				uint32 counterVar = itr->second->counter[i];
				FastGUIDPack( *data, counterVar );
				*data << m_player->GetNewGUID();
				*data << uint32(0);
				*data << uint32( unixTimeToTimeBitfields( time(NULL) ) );
				*data << uint32(0);
				*data << uint32(0);
			}
		}
	}
	*data << uint32(0xFFFFFFFF); // (Branruz)

	if(forInspect)
		m_achievementInspectPacket = data;

	return data;
}

void AchievementInterface::GiveRewardsForAchievement(AchievementEntry * ae)
{
	AchievementReward * ar = AchievementRewardStorage.LookupEntry( ae->ID );
	if(!ar) return;

	// Reward: Item
	if( ar->ItemID )
	{
		// Just use the in-game mail system and mail it to him.
		MailMessage msg;
		memset(&msg, 0, sizeof(MailMessage));
		Item *pItem = objmgr.CreateItem( ar->ItemID, NULL );
		if(!pItem) return;
		pItem->SaveToDB( INVENTORY_SLOT_NOT_SET, 0, true, NULL );
		msg.items.push_back(pItem->GetUInt64Value(OBJECT_FIELD_ENTRY));
		msg.body = "Votre recherche pour accomplir un haut-fait est decrit en dessous.";
		msg.subject = string(ae->name);
		msg.sender_guid = m_player->GetGUID();
		msg.player_guid = m_player->m_playerInfo->guid;
		msg.delivery_time = (uint32)UNIXTIME;
		msg.expire_time = 0; // This message NEVER expires.
		sMailSystem.DeliverMessage(m_player->m_playerInfo->guid, &msg);
		delete pItem;
		pItem = NULL;
	}

	// Reward: Title. We don't yet support titles due to a lack of uint128.
}

void AchievementInterface::EventAchievementEarned(AchievementData * pData)
{
	pData->completed = true;
	pData->date = (uint32)time(NULL);

	AchievementEntry * ae = dbcAchievement.LookupEntry(pData->id);

	GiveRewardsForAchievement(ae);

	WorldPacket * data = BuildAchievementEarned(pData);

	if( m_player->IsInWorld() )
		m_player->GetSession()->SendPacket( data );
	else
		m_player->CopyAndSendDelayedPacket( data );

	delete data;
	HandleAchievementCriteriaRequiresAchievement(pData->id);

	// Realm First Achievements
	if( string(ae->name).find("Realm First") != string::npos  ) // flags are wrong lol
	{
		m_completedRealmFirstAchievements.insert( ae->ID );

		// Send to my team
		WorldPacket data(SMSG_SERVER_FIRST_ACHIEVEMENT, 60);
		data << m_player->GetName();
		data << m_player->GetGUID();
		data << ae->ID;
		data << uint32(1);
		sWorld.SendFactionMessage(&data, m_player->GetTeam());

		// Send to the other team (no clickable link)
		WorldPacket data2(SMSG_SERVER_FIRST_ACHIEVEMENT, 60);
		data2 << m_player->GetName();
		data2 << m_player->GetGUID();
		data2 << ae->ID;
		data2 << uint32(0);
		sWorld.SendFactionMessage(&data2, m_player->GetTeam() ? 0 : 1);
	}
}



WorldPacket* AchievementInterface::BuildAchievementEarned(AchievementData * pData)
{
	pData->m_isDirty = true;
/*
//-- ArcEmu
	const char *msg = "|HJoueur:$N|h[$N]|h a acompli le Haut-Fait $a!";

	if(GetPlayer()->IsInGuild())
	{
		//Guild* guild = objmgr.GetGuildByGuildName(GetPlayer()->m_playerInfo->guild->GetGuildName());
		//Guild * guild = GetPlayer()->getPlayerInfo()->guild;
		WorldPacket data(SMSG_MESSAGECHAT, 200);
		data << uint8(CHAT_MSG_ACHIEVEMENT);
		data << uint8(CHAT_MSG_GUILD_ACHIEVEMENT);
		data << uint32(LANG_UNIVERSAL);
		data << GetPlayer()->GetNewGUID();
		data << uint32(5);
		data << GetPlayer()->GetNewGUID();
		data << uint32(strlen(msg)+1);
		data << msg;
		data << uint8(0);
		data << uint32(achievement->ID);
		GetPlayer()->GetSession()->SendPacket(&data);   
	}
	if(achievement->flags & (ACHIEVEMENT_FLAG_REALM_FIRST_KILL|ACHIEVEMENT_FLAG_REALM_FIRST_REACH))
	{
		WorldPacket data(SMSG_SERVER_FIRST_ACHIEVEMENT, strlen(GetPlayer()->GetName())+1+8+4+4);
		data << GetPlayer()->GetName();
		data << GetPlayer()->GetNewGUID();
		data << uint32(achievement->ID);
		data << uint32(0);
		GetPlayer()->GetSession()->SendPacket(&data);
	}
	else
	{
		WorldPacket data(SMSG_MESSAGECHAT, 200);
		data << uint8(CHAT_MSG_ACHIEVEMENT);
		data << uint32(LANG_UNIVERSAL);
		data << GetPlayer()->GetNewGUID();
		data << uint32(5 );
		data << GetPlayer()->GetNewGUID();
		data << uint32(strlen(msg)+1);
		data << msg;
		data << uint8(0);
		data << uint32(achievement->ID);
		GetPlayer()->GetSession()->SendPacket(&data);
 	}

//---------
*/


	WorldPacket * data = new WorldPacket(SMSG_ACHIEVEMENT_EARNED, 40);
	*data << m_player->GetNewGUID();
	*data << pData->id;
	*data << uint32( unixTimeToTimeBitfields(time(NULL)) );
	*data << uint32(0);

	if( m_achievementInspectPacket )
	{
		delete m_achievementInspectPacket;
		m_achievementInspectPacket = NULL;
	}

	return data;
}

AchievementData* AchievementInterface::CreateAchievementDataEntryForAchievement(AchievementEntry * ae)
{
	AchievementData * ad = new AchievementData;
	memset(ad, 0, sizeof(AchievementData));
	ad->id = ae->ID;
	ad->num_criterias = ae->AssociatedCriteriaCount;
	m_achivementDataMap.insert( make_pair( ad->id, ad ) );
	return ad;
}

bool AchievementInterface::CanCompleteAchievement(AchievementData * ad)
{
	/* don't allow GMs to complete achievements
	if( m_player->GetSession()->HasGMPermissions() )
		return false;
	*/
	if( ad->completed ) return false;

	bool hasCompleted = false;
	AchievementEntry * ach = dbcAchievement.LookupEntry(ad->id);
	if( ach->categoryId == 1 ) // We cannot complete statistics
		return false;

	// realm first achievements
	if( m_completedRealmFirstAchievements.find(ad->id) != m_completedRealmFirstAchievements.end() )
		return false;

	bool failedOne = false;
	for(uint32 i = 0; i < ad->num_criterias; ++i)
	{
		bool thisFail = false;
		AchievementCriteriaEntry * ace = dbcAchivementCriteria.LookupEntry(ach->AssociatedCriteria[i]);
		uint32 ReqCount = ace->MainRequirementCount ? ace->MainRequirementCount : 1;

		if( ace->groupFlag & ACHIEVEMENT_CRITERIA_GROUP_NOT_IN_GROUP && m_player->GetGroup() )
			return false;

		if( ace->timeLimit && ace->timeLimit < ad->completionTimeLast )
			thisFail = true;

		if( ad->counter[i] < ReqCount )
			thisFail = true;

		if( ach->factionFlag == ACHIEVEMENT_FACTION_ALLIANCE && m_player->GetTeam() == 1 )
			thisFail = true;

		if( ach->factionFlag == ACHIEVEMENT_FACTION_HORDE && m_player->GetTeam() == 0 )
			thisFail = true;

		if( thisFail && ace->completionFlag & ACHIEVEMENT_CRITERIA_COMPLETE_ONE_FLAG )
			failedOne = true;
		else if(thisFail)
			return false;

		if( !thisFail && ace->completionFlag & ACHIEVEMENT_CRITERIA_COMPLETE_ONE_FLAG )
			hasCompleted = true;
	}

	if( failedOne && !hasCompleted )
		return false;

	return true;
}

AchievementData* AchievementInterface::GetAchievementDataByAchievementID(uint32 ID)
{
	map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.find( ID );
	if( itr != m_achivementDataMap.end() )
		return itr->second;
	else
	{
		AchievementEntry * ae = dbcAchievement.LookupEntryForced(ID);
		if(!ae)
		{
			printf("No achievement for %u!\n", ID);
		}
		return CreateAchievementDataEntryForAchievement(ae);
	}
}

void AchievementInterface::SendCriteriaUpdate(AchievementData * ad, uint32 idx)
{
	// A voir, la trame n'est pas bonne (branruz). 
/*
SMSG_CRITERIA_UPDATE : 
AssocCrit[idx]  Player Guid ?      unk      time     time1    time2
  A2000000      03D5190F3AD27501 00000000 A89D2209 F0557900 EF557900
  A2000000      03C31A0F3AD27501 00000000 A89D2209 F3557900 F2557900
  FC140000      03C60F0F3AD27501 00000000 A89D2209 F4557900 F3557900
  F3020000      030F010F3AD27501 00000000 A89D2209 9A010000 9A010000
  C9150000      030F010F3AD27501 00000000 A89D2209 9A010000 9A010000
  A2000000      03B21B0F3AD27501 00000000 A89D2209 F6557900 F5557900
  4F130000      0001760F3AD27501 00000000 A89D2209 F6557900 F6557900
  FF140000      0001760F3AD27501 00000000 A89D2209 F6557900 F6557900
  A2000000      03D21D0F3AD27501 00000000 A89D2209 FA557900 F9557900
  FD140000      03C3010F3AD27501 00000000 A89D2209 FA557900 02000000

	ad->m_isDirty = true;
	ad->date = (uint32)time(NULL);
	WorldPacket data(SMSG_CRITERIA_UPDATE, 32);
	AchievementEntry * ae = dbcAchievement.LookupEntry(ad->id);
	data << uint32(ae->AssociatedCriteria[idx]);
	FastGUIDPack( data, (uint64)ad->counter[idx] ); // ?? Disparu 
	data << m_player->GetNewGUID();   
	data << uint32(0);
	data << uint32(unixTimeToTimeBitfields(time(NULL)));
	data << uint32(0);
	data << uint32(0); 

	if( !m_player->IsInWorld() )
		m_player->CopyAndSendDelayedPacket(&data);
	else
		m_player->GetSession()->SendPacket(&data);
		
	if( m_achievementInspectPacket )
	{
		delete m_achievementInspectPacket;
		m_achievementInspectPacket = NULL;
	}
	*/
}

void AchievementInterface::HandleAchievementCriteriaConditionDeath()
{
	// We died, so reset all our achievements that have ACHIEVEMENT_CRITERIA_CONDITION_NO_DEATH
	if( !m_achivementDataMap.size() )
		return;

	map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.begin();
	for(; itr != m_achivementDataMap.end(); ++itr)
	{
		AchievementData * ad = itr->second;
		if(ad->completed) continue;
		AchievementEntry * ae = dbcAchievement.LookupEntry( ad->id );
		for(uint32 i = 0; i < ad->num_criterias; ++i)
		{
			uint32 CriteriaID = ae->AssociatedCriteria[i];
			AchievementCriteriaEntry * ace = dbcAchivementCriteria.LookupEntry( CriteriaID );
			if( ad->counter[i] && ace->AdditionnalRequirement1_Type & ACHIEVEMENT_CRITERIA_CONDITION_NO_DEATH )
			{
				ad->counter[i] = 0;
				SendCriteriaUpdate(ad, i); break;
			}
		}
	}
}

void AchievementInterface::HandleAchievementCriteriaKillCreature(uint32 killedMonster)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
	{
		return;
	}
	
	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqKill = ace->MainRequirement;   // kill_creature.creatureID;
		uint32 ReqCount = ace->MainRequirementCount; // kill_creature.creatureCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;


		// Wrong monster, continue on, kids.
		if( ReqKill != killedMonster )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaWinBattleground(uint32 bgMapId, uint32 scoreMargin, uint32 time, CBattleground *bg)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_WIN_BG );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqBGMap = ace->MainRequirement;  // win_bg.bgMapID;
		uint32 ReqCount = ace->MainRequirementCount; // win_bg.winCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;
		// Wrong BG, continue on, kids.
		if( ReqBGMap != bgMapId )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		ad->completionTimeLast = time;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				if( compareCriteria->AdditionnalRequirement1_Type && (scoreMargin < compareCriteria->AdditionnalRequirement1_Type) ) // BG Score Requirement.
					continue;

				// AV stuff :P
				if( bg->GetType() == BATTLEGROUND_ALTERAC_VALLEY )
				{
					
					AlteracValley *pAV(((AlteracValley *)bg));
					if( pAchievementEntry->ID == 225 ||  pAchievementEntry->ID == 1164) // AV: Everything Counts
					{
						continue; // We do not support mines yet in AV
					}

					if( pAchievementEntry->ID == 220 ) // AV: Stormpike Perfection
					{
						/* A FINIR (Branruz)
						bool failure = false;
						// We must control all Alliance nodes and Horde nodes (towers only)
						for(uint32 i = 0; i < AV_NUM_CONTROL_POINTS; ++i)
						{
							if( pAV->GetNode(i)->IsGraveyard() )
								continue;

							if( pAV->GetNode(i)->GetState() != AV_NODE_STATE_ALLIANCE_CONTROLLED )
								failure = true;
						}
						if( failure )*/ continue;
					}

					if( pAchievementEntry->ID == 873 ) // AV: Frostwolf Perfection
					{
						/* A FINIR (Branruz)
						bool failure = false;
						// We must control all Alliance nodes and Horde nodes (towers only)
						for(uint32 i = 0; i < AV_NUM_CONTROL_POINTS; ++i)
						{
							if( pAV->GetNode(i)->IsGraveyard() )
								continue;

							if( pAV->GetNode(i)->GetState() != AV_NODE_STATE_HORDE_CONTROLLED )
								failure = true;
						}
						if( failure ) */ continue;
					}
				}

				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaRequiresAchievement(uint32 achievementId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqAchievement = ace->MainRequirement; // complete_achievement.linkedAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if( ReqAchievement != achievementId )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaLevelUp(uint32 level)
{

	// Il y a un probleme avec le level 80, crash du serveur sans Dump, A Voir plus tard
	// => Report: http://arbonne.games-rpg.net/index.php/topic,3391.0.html <=
    if(level >=80) return;
	//------------------------------

	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqLevel = ace->MainRequirementCount; // reach_level.level;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if( level < ReqLevel )
			continue;

		/*
		// Realm first to 80 stuff has race and class requirements. Let the hacking begin.
		if( string(pAchievementEntry->name).find("Prem's") != string::npos ) // Machin <<Prem's>> au niveau 80 sur le royaume
		{                                                                    // Premier machin du royaume à atteindre le level80
			// enGB
			//static const char* classNames[] = { "", "Warrior", "Paladin", "Hunter", "Rogue", "Priest", "Death Knight", "Shaman", "Mage", "Warlock", "", "Druid" };
			//static const char* raceNames[] = { "", "Human", "Orc", "Dwarf", "Night Elf", "Forsaken", "Tauren", "Gnome", "Troll", "", "Blood Elf", "Draenei" };
			// Fr (Ne pas modifier, il n'y a pas d'erreur dans les tables - Branruz)
			static const char* classNames[] = { "", "Guerrier", "Paladin", "Chasseur", "Voleur", "PrÃªtre", "Chevalier de la mort", "Chaman", "Mage", "DÃ©moniste", "", "Druide" };
			static const char* raceNames[] = { "", "Humain", "Orc", "Nain", "Elfe de la nuit", "RÃ©prouvÃ©", "Tauren", "Gnome", "Troll", "", "Elfe de sang", "Draene" };
                                                                                   
			uint32 ReqClass = 0;
			uint32 ReqRace = 0;
			for(uint32 i = 0; i < 12; ++i)
			{
				if(strlen(classNames[i]) > 0 && string(pAchievementEntry->name).find(classNames[i]) != string::npos )
				{
					// We require this class
					ReqClass = i;
					break;
				}
			}
			for(uint32 i = 0; i < 12; ++i)
			{
				if(strlen(raceNames[i]) > 0 && string(pAchievementEntry->name).find(raceNames[i]) != string::npos )
				{
					// We require this race
					ReqRace = i;
					break;
				}
			}

			if( ReqClass && m_player->getClass() != ReqClass )
				continue;

			if( ReqRace && m_player->getRace() != ReqRace )
				continue;
		}*/

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = m_player->getLevel() > ReqLevel ? ReqLevel : m_player->getLevel();
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaOwnItem(uint32 itemId, uint32 stack)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqItemId = ace->MainRequirement; // own_item.itemID;
		uint32 ReqItemCount = ace->MainRequirementCount; // own_item.itemCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if( itemId != ReqItemId )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if( ad->completed )
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + stack;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}

	HandleAchievementCriteriaLootItem(itemId, stack);
}

void AchievementInterface::HandleAchievementCriteriaLootItem(uint32 itemId, uint32 stack)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqItemId = ace->MainRequirement; // loot_item.itemID;
		uint32 ReqItemCount = ace->MainRequirementCount; //loot_item.itemCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if( itemId != ReqItemId )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + stack;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaQuestCount(uint32 questCount)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqQuestCount = ace->MainRequirementCount; // complete_quest_count.totalQuestCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = questCount;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaHonorableKillClass(uint32 classId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqClass = ace->MainRequirement; // hk_class.classID;
		uint32 ReqCount = ace->MainRequirementCount; // hk_class.count;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if( ReqClass != classId )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaHonorableKillRace(uint32 raceId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_HK_RACE );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqRace = ace->MainRequirement; // hk_race.raceID;
		uint32 ReqCount = ace->MainRequirementCount; // hk_race.count;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if( ReqRace != raceId )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaTalentResetCostTotal(uint32 cost)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + cost;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaTalentResetCount()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaBuyBankSlot(bool retroactive)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqSlots = ace->MainRequirementCount; // buy_bank_slot.numberOfSlots; // We don't actually use this. :P

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				if( retroactive )
				{
					uint32 bytes = m_player->GetUInt32Value(PLAYER_BYTES_2);
					uint32 slots = (uint8)(bytes >> 16);
					ad->counter[i] = slots > ReqSlots ? ReqSlots : slots;
				}
				else
					ad->counter[i] = ad->counter[i] + 1;

				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaFlightPathsTaken()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaExploreArea(uint32 areaId, uint32 explorationFlags)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqFlags = ace->MainRequirement; // explore_area.areaReference;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		WorldMapOverlayEntry * wmoe = dbcWorldMapOverlay.LookupEntryForced( ReqFlags );
		if(!wmoe) continue;

		AreaTable * at = dbcArea.LookupEntryForced(wmoe->AreaTableID);
		if(!at || !(ReqFlags & at->explorationFlag) )
			continue;

		uint32 offset = at->explorationFlag / 8; //32; // c'est un Byte pas un DWORD (Branruz)
		//offset += PLAYER_EXPLORED_ZONES_1;  // Recup de la zone (de 0 à 512), 346 max trouvées ds Dbc

		uint32 val = (uint32)(1 << (at->explorationFlag % 8)); //32)); // c'est un Byte pas un DWORD (Branruz)
		uint32 currFields = m_player->GetByte(PLAYER_EXPLORED_ZONES_1 + (offset / 4),(offset % 4)); //GetUInt32Value(offset);

		// Not explored /sadface
		if( !(currFields & val) )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaHonorableKill()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqKills = ace->MainRequirementCount; // honorable_kill.killCount;

		if( m_player->m_killsLifetime < ReqKills )
			continue;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

#define SCRIPTOK_FALSE { scriptOk = false; break; }
void AchievementInterface::HandleAchievementCriteriaDoEmote(uint32 emoteId, Unit *pTarget)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqEmote = ace->MainRequirement; // do_emote.emoteID;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if(ReqEmote != emoteId)
			continue; // Wrong emote, newb!

		// Target information is not stored, so we'll have to do this one by one...
		// --unless the target's name is the description of the criteria! Bahahaha
		bool scriptOk = false;
		if( pTarget && pTarget->IsCreature() )
		{
			Creature *pCreature = ((Creature *)pTarget);
			if( !(!ace->name || strlen(ace->name) == 0 || !pCreature->GetCreatureName() || stricmp(pCreature->GetCreatureName()->Name, ace->name) != 0) )
			{
				scriptOk = true;
			}
		}

		string name = string(pAchievementEntry->name);
		if( name.find("Total") != string::npos )
		{
			// It's a statistic, like: "Total Times /Lol'd"
			scriptOk = true;
		}

		// Script individual ones here...
		if( ace->ID == 2379 ) // Make Love, Not Warcraft
		{
			if( !pTarget || !pTarget->IsPlayer() || !pTarget->isDead() || !isHostile(pTarget, m_player) )
				SCRIPTOK_FALSE

			scriptOk = true;
		}
		else if( ace->ID == 6261 ) // Winter Veil: A Frosty Shake 
		{
			if( m_player->GetZoneId() != 4395 ) // Not in Dalaran
				SCRIPTOK_FALSE
			
			if( !pTarget || !pTarget->HasAura(21848) ) // Not a Snowman
				SCRIPTOK_FALSE

			scriptOk = true;
		}

		if( !scriptOk ) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaCompleteQuestsInZone(uint32 zoneId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqZone = ace->MainRequirement; // complete_quests_in_zone.zoneID;
		uint32 ReqCount = ace->MainRequirementCount; // complete_quests_in_zone.questCount;

		if( ReqZone != zoneId )
			continue;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaReachSkillLevel(uint32 skillId, uint32 skillLevel)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqSkill = ace->MainRequirement; // reach_skill_level.skillID;
		uint32 ReqLevel = ace->MainRequirementCount; // reach_skill_level.skillLevel;

		if( ReqSkill != skillId )
			continue;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = skillLevel;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaWinDuel()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		//uint32 ReqDuels = ace->win_duel.duelCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaLoseDuel()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		//uint32 ReqDuels = ace->win_duel.duelCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaKilledByCreature(uint32 killedMonster)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE );
	if( itr == objmgr.m_achievementCriteriaMap.end() )
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqCreature = ace->MainRequirement; // killed_by_creature.creatureEntry;

		if( ReqCreature != killedMonster )
			continue;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i]++;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaKilledByPlayer()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i]++;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaDeath()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_DEATH );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i]++;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}

	HandleAchievementCriteriaDeathAtMap(m_player->GetMapId());
}

void AchievementInterface::HandleAchievementCriteriaDeathAtMap(uint32 mapId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 MapID = ace->MainRequirement; // death_at_map.mapID;

		if( mapId != MapID )
			continue;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchivementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria[i] );			
			if( compareCriteria == ace )
			{
				ad->counter[i]++;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}