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

//Pakcet Building
/////////////////

WorldPacket* WorldSession::BuildQuestQueryResponse(Quest *qst)
{
	// 2048 bytes should be more than enough. The fields cost ~200 bytes.
	// better to allocate more at startup than have to realloc the buffer later on.

	WorldPacket *data = new WorldPacket(SMSG_QUEST_QUERY_RESPONSE, 248);
	LocalizedQuest * lci = (language>0) ? sLocalizationMgr.GetLocalizedQuest(qst->id, language) : NULL;

	*data << uint32(qst->id);					 // Quest ID
	*data << uint32(2);							 // Quest Method ? Accepted values: 0, 1 or 2. 0==IsAutoComplete() (skip objectives/details) (mangos)
	*data << int32(qst->max_level);				 // Quest level
	*data << uint32(qst->min_level);             // 3.3.2.11403 (Branruz)

	if(qst->quest_sort > 0)
		*data << int32(-(int32)qst->quest_sort); // Negative if pointing to a sort.
	else 
		*data << uint32(qst->zone_id);			 // Positive if pointing to a zone.


	*data << uint32(qst->type);					 // Info ID / Type
	*data << uint32(qst->required_players);		 // suggested players

	*data << uint32(qst->required_rep_faction);	 // Faction ID
	*data << uint32(qst->required_rep_value);	 // Faction Amount
	
	*data << uint32(0);							 // Opposite Faction ID
	*data << uint32(0);							 // Opposite Faction Amount
	
	*data << uint32(qst->next_quest_id);		 // Next Quest ID
    *data << uint32(0);							 // column index in QuestXP.dbc (row based on quest level) (mangos)
	
	*data << uint32(sQuestMgr.GenerateRewardMoney(GetPlayer(), qst)); // Copper reward
	
	*data << uint32(qst->required_money);        // Money au LevelCap (Branruz)
	*data << uint32(qst->reward_spell);			 // Spell added to spellbook upon completion	
	*data << uint32(qst->effect_on_player); 	 // Test Branruz 3.0.3 // Spell casted on player upon completion	

	*data << uint32(qst->bonus_honor);			 // 2.3.0 - bonus honor
	*data << float(0);							 // new reward honor (multiplied by ~62 at client side) (mangos)
	
	*data << uint32(qst->srcitem);				 // Item given at the start of a quest (srcitem)
	*data << uint32(qst->quest_flags & 0xFFFF ); // Quest Flags
	*data << uint32(qst->reward_title);			 // Reward Title Id - Player is givn this title upon completion
	*data << uint32(qst->required_kill_player);	 // Required Kill Player
	*data << uint32(qst->reward_talents);		 // 3.0.2 Rewarded talents ID
	//*data << uint32(0);							 // bonus arena points
	*data << uint32(0);							 // Arena Points
	*data << uint32(0);							 // Arena Points Multiplicator.

	// (loop 4 times)
	for(uint32 i = 0; i < 4; ++i)
	{
		*data << qst->reward_item[i];				// Forced Reward Item [i]
		*data << qst->reward_itemcount[i];			// Forced Reward Item Count [i]
	}

	// (loop 6 times)
	for(uint32 i = 0; i < 6; ++i)
	{
		*data << qst->reward_choiceitem[i];			// Choice Reward Item [i]
		*data << qst->reward_choiceitemcount[i];	// Choice Reward Item Count [i]
	}

    // ReputationId - Faction.dbc 3.3.2.11403 (Branruz)
	*data << qst->reward_repfaction[0]; // par exemple Darnassus 69
	*data << qst->reward_repfaction[1];
	*data << qst->reward_repfaction[2];
	*data << qst->reward_repfaction[3];
	*data << qst->reward_repfaction[4];

	// QuestFactionReward.dbc 3.3.2.11403 (Branruz)
    // Seulement 2 lignes et 11 colonnes donc attention avec la base
	// ATTENTION: c'est le numero de la colonne qui est passé et plus la valeur
	// d'apres wowhead, on prend 250 a Darna pour la quete 1134, pourtant col = 5
	// http://fr.wowhead.com/?quest=1134
	// Col Base ??       1   2   3    4    5    6    7     8   9 <----------- *data ??
 	// Col Dbc     1 2   3   4   5    6    7    8    9    10  11
	//             1 0  10  25  75  150  250  350  500  1000   5 (Ami)
	//             2 0 -10 -25 -75 -150 -250 -350 -500 -1000  -5 (Ennemi)
	*data << qst->reward_repvalueIndex[0]; // Darna reput = col 5 sur offi 332
	*data << qst->reward_repvalueIndex[1];
	*data << qst->reward_repvalueIndex[2];
	*data << qst->reward_repvalueIndex[3];
	*data << qst->reward_repvalueIndex[4];

    // Inconnu 3.3.2.11403 (Branruz) ! attention a la reput ....
	// Source Mangos : Override Reputation values - Donc il ne faut pas prendre en compte les trames qui suivent sous peine d'avoir la réputation à 0
	*data << uint32(0);
	*data << uint32(0);
	*data << uint32(0);
	*data << uint32(0);
	*data << uint32(0);
	// ---

	*data << qst->point_mapid;	
	*data << qst->point_x;		
	*data << qst->point_y;		
	*data << qst->point_opt;	
	
	if(lci)
	{
		*data << lci->Title;
		*data << lci->Objectives;
		*data << lci->Details;
		*data << lci->EndText;
		*data << lci->CompletionText; // (mangos)
	}
	else
	{
		*data << qst->title;		// Title / name of quest
		*data << qst->objectives;	// Objectives / description
		*data << qst->details;		// Details
		*data << qst->endtext;		// Subdescription
		*data << qst->completiontext; // (mangos)
	}

	// (loop 4 times)
	for(uint32 i = 0; i < 4; ++i)
	{
       if (qst->required_mob[i] < 0)
        {
            // client expected gameobject template id in form (id|0x80000000)
            *data << uint32((qst->required_mob[i]*(-1))|0x80000000);
        }
        else
        {
            *data << uint32(qst->required_mob[i]);
        }
		// *data << qst->required_mob[i];			 // Kill mob entry ID [i]
		*data << qst->required_mobcount[i];			 // Kill mob count [i]
		*data << qst->RecItemDuringQuestId[i];		 // 3.0.3, WaadTeam
		*data << uint32(0);							 // Unknown 3.3.2.11403 (Branruz)
	}

	for(uint32 i = 0; i < 6; ++i)
	{
		*data << qst->required_item[i];				// Collect item [i]
		*data << qst->required_itemcount[i];		// Collect item count [i]
	}

	if(lci)
	{
		*data << lci->ObjectiveText[0];
		*data << lci->ObjectiveText[1];
		*data << lci->ObjectiveText[2];
		*data << lci->ObjectiveText[3];
	}
	else
	{
		*data << qst->objectivetexts[0];				// Objective 1 - Used as text if mob not set
		*data << qst->objectivetexts[1];				// Objective 2 - Used as text if mob not set
		*data << qst->objectivetexts[2];				// Objective 3 - Used as text if mob not set
		*data << qst->objectivetexts[3];				// Objective 4 - Used as text if mob not set
	}

	return data;
}


/*****************
* QuestLogEntry *
*****************/
QuestLogEntry::QuestLogEntry()
{
	mInitialized = false;
	m_quest = NULL;
	mDirty = false;
	m_slot = -1;
	completed=0;
	m_player_slain = 0;
}

QuestLogEntry::~QuestLogEntry()
{
	m_plr = NULL;
	m_quest = NULL;
}

void QuestLogEntry::Init(Quest* quest, Player* plr, uint32 slot)
{
	//ASSERT(quest);
	//ASSERT(plr);

	if(!plr)
	{
		Log.Error("QuestLogEntry::Init","Player est NULL!");
		return;
	}

	if(!quest)
	{
		Log.Error("QuestLogEntry::Init","Parametre Quest est NULL! (%s)",plr->GetName());
		return;
	}

	if(slot >=25)
	{
      Log.Error("QuestLogEntry::Init","Numero de Slot trop grand (%u >= 25)! (%s)",slot,plr->GetName());
	  return;
	}

	m_quest = quest;
	m_plr = plr;
	m_slot = slot;

	iscastquest = false;
	for (uint32 i=0;i<4;++i)
	{
		if (quest->required_spell[i]!=0)
		{
			iscastquest=true;
			if (!plr->HasQuestSpell(quest->required_spell[i]))
				plr->quest_spells.insert(quest->required_spell[i]);
		}
		else if (quest->required_mob[i]!=0)
		{
			if (!plr->HasQuestMob(quest->required_mob[i]))
				plr->quest_mobs.insert(quest->required_mob[i]);
		}
	}


	// update slot
	plr->SetQuestLogSlot(this, slot);
	
	mDirty = true;

	//m_playercount = 0;
	memset(m_mobcount, 0, 4*4);
	memset(m_explored_areas, 0, 4*4);

	if(m_quest->time)
		m_time_left = m_quest->time * 1000;
	else
		m_time_left = 0;

	CALL_QUESTSCRIPT_EVENT(this, OnQuestStart)(plr, this);
}

void QuestLogEntry::ClearAffectedUnits()
{
	if (m_affected_units.size()>0)
		m_affected_units.clear();
}
void QuestLogEntry::AddAffectedUnit(Unit* target)
{
	if (!target)
		return;
	if (!IsUnitAffected(target))
		m_affected_units.insert(target->GetGUID());
}
bool QuestLogEntry::IsUnitAffected(Unit* target)
{
	if (!target)
		return true;
	if (m_affected_units.find(target->GetGUID()) != m_affected_units.end())
		return true;
	return false;
}

void QuestLogEntry::SaveToDB(QueryBuffer * buf)
{
	ASSERT(m_slot != -1);
	if(!mDirty)
		return;

	//Made this into a replace not an insert
	//CharacterDatabase.Execute("DELETE FROM questlog WHERE player_guid=%u AND quest_id=%u", m_plr->GetGUIDLow(), m_quest->id);
	std::stringstream ss;
	ss << "REPLACE INTO questlog VALUES(";
	ss << m_plr->GetLowGUID() << "," << m_quest->id << "," << m_slot << "," << m_time_left;
	for(int i = 0; i < 4; ++i)
		ss << "," << m_explored_areas[i];
	
	for(int i = 0; i < 4; ++i)
		ss << "," << m_mobcount[i];

	ss << "," << m_player_slain;

	ss << ")";
	
	if( buf == NULL )
		CharacterDatabase.Execute( ss.str().c_str() );
	else
		buf->AddQueryStr(ss.str());
}

bool QuestLogEntry::LoadFromDB(Field *fields)
{
	// playerguid,questid,timeleft,area0,area1,area2,area3,kill0,kill1,kill2,kill3
	int f = 3;
	ASSERT(m_plr && m_quest);
	m_time_left = fields[f].GetUInt32();	f++;
	for(int i = 0; i < 4; ++i)
	{
		m_explored_areas[i] = fields[f].GetUInt32();	f++;
		CALL_QUESTSCRIPT_EVENT(this, OnExploreArea)(m_explored_areas[i], m_plr, this);
	}

	for(int i = 0; i < 4; ++i)
	{
		m_mobcount[i] = fields[f].GetUInt32();	f++;
		if(GetQuest()->required_mobtype[i] == QUEST_MOB_TYPE_CREATURE)
			CALL_QUESTSCRIPT_EVENT(this, OnCreatureKill)(GetQuest()->required_mob[i], m_plr, this);
		else
			CALL_QUESTSCRIPT_EVENT(this, OnGameObjectActivate)(GetQuest()->required_mob[i], m_plr, this);
	}
	mDirty = false;
	return true;
}

bool QuestLogEntry::CanBeFinished()
{
	uint32 i;

	for(i = 0; i < 4; ++i)
	{
		if(m_quest->required_mob[i])
		{
			if(m_mobcount[i] < m_quest->required_mobcount[i])
			{
				return false;
			}
		}
	}

	for(i = 0; i < 6; ++i)
	{
		if(m_quest->required_item[i])
		{
			if(m_plr->GetItemInterface()->GetItemCount(m_quest->required_item[i]) < m_quest->required_itemcount[i])
			{
				return false;
			}
		}
	}

//Check for Gold & AreaTrigger Requirement s
	if(m_quest->required_money && (m_plr->GetUInt32Value(PLAYER_FIELD_COINAGE) < m_quest->required_money)) 
			return false;

	for(i = 0; i < 4; ++i)
	{
		if(m_quest->required_triggers[i])
		{
			if(m_explored_areas[i] == 0) return false;
		}
	}


	//if(m_quest->required_kill_player != m_player_slain) return false;

	if(m_plr->GetFinishedDailiesCount() >= 25) return false;


	// Appel QuestScript, Debug de Crash sur les Quetes DK ( http://arbonne.games-rpg.net/index.php/topic,3180.0.html )
	if(static_cast<QuestLogEntry*>(this)->GetQuest()->pQuestScript != NULL) 
	{
		 return(static_cast<QuestLogEntry*>(this)->GetQuest()->pQuestScript->OnQuestCanBeFinished(m_plr,this));
	}
	
	return(true);
}

void QuestLogEntry::SubtractTime(uint32 value)
{
	if(this->m_time_left  <=value)
		m_time_left = 0;
	else
		m_time_left-=value;
}

void QuestLogEntry::SetPlayerSlainCount(uint32 count)
{
	m_player_slain = count;
	mDirty = true;
}

void QuestLogEntry::SetMobCount(uint32 i, uint32 count)
{
	//ASSERT(i<4);
	if(i>=4)
	{
		Log.Error("QuestLogEntry::SetMobCount","MobCount > 3 (%s)",m_plr->GetName());
	}
	m_mobcount[i] = count;
	mDirty = true;
}

void QuestLogEntry::SetPlayerKillCount(uint32 count)
{
	m_playercount = count;
	mDirty = true;
}

void QuestLogEntry::SetTrigger(uint32 i)
{
	//ASSERT(i<4);
	if(i>=4)
	{
		Log.Error("QuestLogEntry::SetTrigger","Index m_explored_areas > 3 (%s)",m_plr->GetName());
	}
	m_explored_areas[i] = 1;
	mDirty = true;
}

void QuestLogEntry::SetSlot(int32 i)
{
	//ASSERT(i!=-1);
	if(i<0)
	{
		Log.Error("QuestLogEntry::SetSlot","Index SetSlot est negatif (%s)",m_plr->GetName());
	}

	m_slot = i;
}

void QuestLogEntry::Finish()
{
	if(this->m_plr)
	{
 	 uint32 base = GetBaseField(m_slot);

	 if(m_plr->GetUInt32Value(base + QUEST_TIMER_INDEX) > 0)
	 {   // Faut surtout pas l'oublier celui-la sinon gros leak quand le timer tombe^^ (Branruz)
		 sEventMgr.RemoveEvents(m_plr,EVENT_TIMED_QUEST_EXPIRE);
	 }

	 m_plr->SetUInt32Value(base + QUEST_ID_INDEX   , 0);
	 m_plr->SetUInt32Value(base + QUEST_STATE_INDEX, 0);
	 m_plr->SetUInt32Value(base + QUEST_FIELD_INDEX, 0); 
	 m_plr->SetUInt32Value(base + QUEST_TIMER_INDEX, 0); 

	 // clear from player log
	 m_plr->SetQuestLogSlot(NULL, m_slot);
	 m_plr->PushToRemovedQuests(m_quest->id);

	 // delete ourselves
	 delete this;
	 
	}
}

void QuestLogEntry::UpdatePlayerFields()
{
	if(!m_plr) return;
    if(m_slot < 0) return;

	uint32 base = GetBaseField(m_slot);

	m_plr->SetUInt32Value(base + QUEST_ID_INDEX, m_quest->id);

	// next field is kills and shit like that
	uint64 field1 = 0;
	int i;
	
	// explored areas
	if(m_quest->count_requiredtriggers)
	{
		uint32 count = 0;
		for(i = 0; i < 4; ++i)
		{
			if(m_quest->required_triggers[i])
			{
				if(m_explored_areas[i] == 1)
				{
					count++;
				}
			}
		}

		if(count == m_quest->count_requiredtriggers)
		{
			field1 |= 0x01000000;
		}
	}

	// mob hunting
	if(m_quest->count_required_mob)
	{
		// optimized this - burlex
		uint8 * p = (uint8*)&field1;
		for(int i = 0; i < 4; ++i)
		{
			if( m_quest->required_mob[i] && m_mobcount[i] > 0 )
				p[2*i] |= (uint8)m_mobcount[i];
		}
	}

		// player hunting - Patch AspireDev
	if(m_quest->required_players)
	{
		if( m_quest->required_players && m_playercount > 0 )
				field1 |= (uint8)m_playercount;
	}
		
	m_plr->SetUInt32Value(base + QUEST_STATE_INDEX, 0);
	m_plr->SetUInt32Value(base + QUEST_FIELD_INDEX, field1);
	if(m_time_left > 0) // init dans QuestLogEntry::Init
	{   
		//Start Quest Timer Event Here
		sEventMgr.AddEvent(m_plr, &Player::EventTimedQuestExpire, m_quest, this, static_cast<uint32>(m_slot), EVENT_TIMED_QUEST_EXPIRE, m_time_left,1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		uint32 qtime = static_cast<uint32>(time(NULL) + m_quest->time);
		m_plr->SetUInt32Value((PLAYER_QUEST_LOG_1_1 + (m_slot * PLAYER_QUEST_SLOT_MAX))+QUEST_TIMER_INDEX, qtime);
		m_plr->timed_quest_slot = m_slot;
		sLog.outDetail("Quest %u ,Temps restant: %u sec (Slot %u)",m_quest->id,m_quest->time,m_slot);
	}
    else m_plr->SetUInt32Value(base + QUEST_TIMER_INDEX, 0);

	// Timed quest handler.
	if(m_time_left && !sEventMgr.HasEvent( m_plr,EVENT_TIMED_QUEST_EXPIRE ))
		m_plr->EventTimedQuestExpire(m_quest, this , m_slot);
	    
}

void QuestLogEntry::SendQuestComplete()
{
	WorldPacket data(4);
	data.SetOpcode(SMSG_QUESTUPDATE_COMPLETE);
	data << m_quest->id;
	m_plr->GetSession()->SendPacket(&data);
	CALL_QUESTSCRIPT_EVENT(this, OnQuestComplete)(m_plr, this); 

}

// Test Branruz
// Normalement le Guid devrait etre passé s'il s'agit d'un pnj ou d'un mod
void QuestLogEntry::SendUpdateAddKill(uint32 i, uint64 victimGUID)
{
	sQuestMgr.SendQuestUpdateAddKill(m_plr, m_quest->id, m_quest->required_mob[i], m_mobcount[i], m_quest->required_mobcount[i], victimGUID);
}

// Patch AspireDev
void QuestLogEntry::SendUpdateAddPlayerKill(uint64 victimGUID)
{
	sQuestMgr.SendQuestUpdateAddPvpKill(m_plr, m_quest->id, m_playercount, m_quest->required_players, victimGUID);
}