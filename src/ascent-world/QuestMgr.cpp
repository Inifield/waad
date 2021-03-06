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

uint32 QuestMgr::CalcQuestStatus(Object* quest_giver, Player* plr, QuestRelation* qst)
{
	return CalcQuestStatus(quest_giver, plr, qst->qst, qst->type, false);
}

bool QuestMgr::isRepeatableQuestFinished(Player *plr, Quest *qst)
{
    uint32 i;

	for(i = 0; i < 6; ++i)
	{
		if(qst->required_item[i])
		{
			if(plr->GetItemInterface()->GetItemCount(qst->required_item[i]) < qst->required_itemcount[i])
			{
				return false;
			}
		}
	}

	return true;
}

uint32 QuestMgr::PlayerMeetsReqs(Player* plr, Quest* qst, bool skiplevelcheck)
{
	std::list<uint32>::iterator itr;
	uint32 status=0;

	if (!sQuestMgr.IsQuestRepeatable(qst))
		status = QMGR_QUEST_AVAILABLE;
	else
	{
		status = QMGR_QUEST_REPEATABLE;
		/*if (qst->is_repeatable == REPEATABLE_QUEST)
			return QMGR_QUEST_REPEATABLE_FINISHED;

		if (qst->is_repeatable == REPEATABLE_DAILY)
		{
			if (plr->m_finisheddailyQuests.size() < 25 && plr->m_finisheddailyQuests.find(qst->id) == plr->m_finisheddailyQuests.end())
				return QMGR_QUEST_REPEATABLE;
			else
				return QMGR_QUEST_NOT_AVAILABLE;
		}*/
    }

	if (plr->getLevel() < qst->min_level && !skiplevelcheck)
		return QMGR_QUEST_AVAILABLELOW_LEVEL;

	if(qst->required_class)
		if(!(qst->required_class & plr->getClassMask()))
			return QMGR_QUEST_NOT_AVAILABLE;

	if(qst->required_races)
	{
		if(!(qst->required_races & plr->getRaceMask()))
			return QMGR_QUEST_NOT_AVAILABLE;
	}

	if(qst->required_tradeskill)
	{
		if(!plr->_HasSkillLine(qst->required_tradeskill))
			return QMGR_QUEST_NOT_AVAILABLE;
		if (qst->required_tradeskill_value && plr->_GetSkillLineCurrent(qst->required_tradeskill) < qst->required_tradeskill_value)
			return QMGR_QUEST_NOT_AVAILABLE;
	}

	// Check reputation
	if(qst->required_rep_faction && qst->required_rep_value)
		if(plr->GetStanding(qst->required_rep_faction) < (int32)qst->required_rep_value)
			return QMGR_QUEST_NOT_AVAILABLE;

	if (plr->HasFinishedQuest(qst->id) && (qst->is_repeatable == NO_REPEATABLE_QUEST))
		return QMGR_QUEST_NOT_AVAILABLE;

	for(uint32 i = 0; i < 4; ++i)
	{
		if (qst->required_quests[i] > 0 && !plr->HasFinishedQuest(qst->required_quests[i]))
		{
			return QMGR_QUEST_NOT_AVAILABLE;
		}
	}

	// check quest level
	//if( plr->getLevel() >= ( qst->max_level + 5 ) && ( status != QMGR_QUEST_REPEATABLE_FINISHED ) )
	if( plr->getLevel() >= ( qst->max_level + 5 ) )
		return QMGR_QUEST_CHAT;

	return status;
}

uint32 QuestMgr::CalcQuestStatus(Object* quest_giver, Player* plr, Quest* qst, uint8 type, bool skiplevelcheck)
{
	QuestLogEntry* qle;

	qle = plr->GetQuestLogForEntry(qst->id);

	if (!qle)
	{
		if (type & QUESTGIVER_QUEST_START)
		{
			return PlayerMeetsReqs(plr, qst, skiplevelcheck);
		}
	}
	else
	{		
		if (!qle->CanBeFinished())
		{
			return QMGR_QUEST_NOT_FINISHED;
		}
		else
		{
			if (type & QUESTGIVER_QUEST_END) 
			{
				return QMGR_QUEST_FINISHED;					
			}
			else
			{
				return QMGR_QUEST_NOT_AVAILABLE;
			}
		}
	}

	return QMGR_QUEST_NOT_AVAILABLE;
}

uint32 QuestMgr::CalcStatus(Object* quest_giver, Player* plr)
{
	uint32 status = QMGR_QUEST_NOT_AVAILABLE;
	std::list<QuestRelation *>::const_iterator itr;
	std::list<QuestRelation *>::const_iterator q_begin;
	std::list<QuestRelation *>::const_iterator q_end;
	bool bValid = false;

	if( quest_giver->GetTypeId() == TYPEID_GAMEOBJECT )
	{
        bValid = ((GameObject*)quest_giver)->HasQuests();
        if(bValid)
		{
			q_begin = ((GameObject*)quest_giver)->QuestsBegin();
			q_end = ((GameObject*)quest_giver)->QuestsEnd();
		}
	} 
	else if( quest_giver->GetTypeId() == TYPEID_UNIT )
	{
		bValid = static_cast< Creature* >( quest_giver )->HasQuests();
		if(bValid)
		{
			q_begin = ((Creature*)quest_giver)->QuestsBegin();
			q_end = ((Creature*)quest_giver)->QuestsEnd();
		}
	}
    else if( quest_giver->GetTypeId() == TYPEID_ITEM )
    {
        if( static_cast< Item* >( quest_giver )->GetProto()->QuestId )
            bValid = true;
    }
	//This will be handled at quest share so nothing important as status
	else if(quest_giver->GetTypeId() == TYPEID_PLAYER)
	{
		status = QMGR_QUEST_AVAILABLE;
	}

	if(!bValid)
	{
        //anoying msg that is not needed since all objects dont exactly have quests 
		//sLog.outDebug("QUESTS: Warning, invalid NPC "I64FMT" specified for CalcStatus. TypeId: %d.", quest_giver->GetGUID(), quest_giver->GetTypeId());
		return status;
	}

	if(quest_giver->GetTypeId() == TYPEID_ITEM)
	{
		Quest *pQuest = QuestStorage.LookupEntry( static_cast<Item*>(quest_giver)->GetProto()->QuestId );
		QuestRelation qr;
		qr.qst = pQuest;
		qr.type = 1;

		uint32 tmp_status = CalcQuestStatus(quest_giver,plr, &qr);
		if(tmp_status > status)
			status = tmp_status;
	}

	for(itr = q_begin; itr != q_end; ++itr)
	{
		uint32 tmp_status = CalcQuestStatus(quest_giver, plr, *itr);	// save a call
		if (tmp_status > status)
			status = tmp_status;
	}

	return status;
}

uint32 QuestMgr::ActiveQuestsCount(Object* quest_giver, Player* plr)
{
	std::list<QuestRelation *>::const_iterator itr;
	map<uint32, uint8> tmp_map;
	uint32 questCount = 0;

	std::list<QuestRelation *>::const_iterator q_begin;
	std::list<QuestRelation *>::const_iterator q_end;
	bool bValid = false;

	if(quest_giver->GetTypeId() == TYPEID_GAMEOBJECT)
	{
        bValid = ((GameObject*)quest_giver)->HasQuests();
		if(bValid)
		{
			q_begin = ((GameObject*)quest_giver)->QuestsBegin();
			q_end   = ((GameObject*)quest_giver)->QuestsEnd();
			
		}
	} 
	else if(quest_giver->GetTypeId() == TYPEID_UNIT)
	{
		bValid = ((Creature*)quest_giver)->HasQuests();
		if(bValid)
		{
			q_begin = ((Creature*)quest_giver)->QuestsBegin();
			q_end   = ((Creature*)quest_giver)->QuestsEnd();
		}
	}

	if(!bValid)
	{
		sLog.outDebug("QUESTS: Warning, invalid NPC "I64FMT" specified for ActiveQuestsCount. TypeId: %d.", quest_giver->GetGUID(), quest_giver->GetTypeId());
		return 0;
	}

	for(itr = q_begin; itr != q_end; ++itr)
	{
		if (CalcQuestStatus(quest_giver, plr, *itr) >= QMGR_QUEST_CHAT)
		{
			if (tmp_map.find((*itr)->qst->id) == tmp_map.end())
			{
				tmp_map.insert(std::map<uint32,uint8>::value_type((*itr)->qst->id, 1));
				questCount++;
			}
		}
	}

	return questCount;
}

void QuestMgr::BuildOfferReward(WorldPacket *data, Quest* qst, Object* qst_giver, uint32 menutype, uint32 language,Player *plr)
{
	LocalizedQuest * lq = (language>0) ? sLocalizationMgr.GetLocalizedQuest(qst->id,language):NULL;
	ItemPrototype * it;

	data->SetOpcode(SMSG_QUESTGIVER_OFFER_REWARD);
	*data << uint64(qst_giver->GetGUID());
	*data << uint32(qst->id);

	if(lq)
	{
		*data << lq->Title;
		*data << lq->CompletionText;
	}
	else
	{
		*data << qst->title;
		*data << qst->completiontext;
	}
	
	//uint32 a = 0, b = 0, c = 1, d = 0, e = 1;

	*data << (qst->next_quest_id ? uint8(1) : uint8(0)); // next quest shit (uint8 3.3.2.11403) (Branruz)
    *data << qst->quest_flags;
    *data << qst->required_players;
	*data << uint32(1);									 // emotes count
	*data << uint32(0);									 // emote delay
	*data << uint32(1);									 // emote type

	*data << qst->count_reward_choiceitem;
	if (qst->count_reward_choiceitem)
	{
		for(uint32 i = 0; i < qst->count_reward_choiceitem; ++i) // 6
		{
		if(!qst->reward_choiceitem[i]) 
			{
				Log.Error("SMSG_QUESTGIVER_OFFER_REWARD",
					"reward_choiceitem NULL, veuillez verifier svp, table Quests (Id:%u)",qst->id);
				
				//continue;
			}
			*data << qst->reward_choiceitem[i];
			*data << qst->reward_choiceitemcount[i];
			it = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[i]);
			*data << (it ? it->DisplayInfoID : uint32(0));
		}
	}

	*data << qst->count_reward_item;
	if(qst->count_reward_item)
	{
		for(uint32 i = 0; i < qst->count_reward_item; ++i) // 4
		{
			if(!qst->reward_item[i]) 
			{
				Log.Error("SMSG_QUESTGIVER_OFFER_REWARD",
					"reward_item NULL, veuillez verifier svp, table Quests (Id:%u)",qst->id);
				//continue;
			}

			*data << qst->reward_item[i];
			*data << qst->reward_itemcount[i];
			it = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
			*data << (it ? it->DisplayInfoID : uint32(0));
		}
	}
	
	//*data << GenerateRewardMoney(plr, qst); // Hearthstone
	//*data << ((plr->getLevel() >= sWorld.m_levelCap) ? uint32(0):uint32(qst->reward_xp)); // 3.3.2.11403 (Branruz)
	//*data << uint32(0);

	//Gestion des r�compenses en Xps, argents, et argent � la place d'Xps par Randdrick
	if (plr->getLevel() >= sWorld.m_levelCap)
	{
		*data << uint32(qst->reward_xp_as_money);
		*data << uint32(0); // Pas d'Xps de gagn�s si le niveau du joueur est sup�rieur au niveau maximum
	}
	else
	{
		uint32 xp = 0;
		xp = float2int32(GenerateQuestXP(plr,qst) * sWorld.getRate(RATE_QUESTXP));
		*data << uint32(GenerateRewardMoney(plr, qst));
		*data << uint32(xp);
	}

	*data << (qst->bonus_honor *10);								// Bonus Honor
	*data << float(0);	         // Test 3.3.2.11403 (Branruz) 
	*data << uint32(0x08);	     // Test 3.3.2.11403 (Branruz) (0x00000008 avec quete 11752)
	*data << qst->reward_spell;
	*data << qst->effect_on_player;	 // Cast Spell
	*data << uint32(0);              // qst->reward_title; 
	*data << qst->reward_talents;	 // 3.0.2 - Reward Talents
	*data << uint32(0); // 3.3 Arena Points.
	*data << uint32(0); // Maybe show reward rep.

	 // ReputationId - Faction.dbc 3.3.2.11403 (Branruz)
	*data << qst->reward_repfaction[0]; // par exemple Darnassus 69
	*data << qst->reward_repfaction[1];
	*data << qst->reward_repfaction[2];
	*data << qst->reward_repfaction[3];
	*data << qst->reward_repfaction[4];

	// QuestFactionReward.dbc 3.3.2.11403 (Branruz)
    // Seulement 2 lignes et 11 colonnes donc attention avec la base
	// ATTENTION: c'est le numero de la colonne qui est pass� et plus la valeur
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

	// Inconnu 3.3.2.11403 (Branruz)
    *data << uint32(0); 
	*data << uint32(0); 
	*data << uint32(0); 
	*data << uint32(0); 
	*data << uint32(0);
    //--------- 
	//Log.Notice( "[QuestMgr]","BuildOfferReward Sent SMSG_QUESTGIVER_OFFER_REWARD." );
}

void QuestMgr::BuildQuestDetails(WorldPacket *data, Quest* qst, Object* qst_giver, uint32 menutype, uint32 language,Player *plr)
{
	ItemPrototype *ip;
	uint32 i;

	LocalizedQuest * lq = (language>0) ? sLocalizationMgr.GetLocalizedQuest(qst->id,language):NULL;
	std::map<uint32, uint8>::const_iterator itr;

	data->SetOpcode( SMSG_QUESTGIVER_QUEST_DETAILS );

	*data << qst_giver->GetGUID();
	*data << uint64(0); //3.0.2
	*data << qst->id;
	if(lq)
	{
		*data << lq->Title;
		*data << lq->Details;
		*data << lq->Objectives;
	}
	else
	{
		*data <<  qst->title;
		*data <<  qst->details;
		*data <<  qst->objectives;
	}

	*data << uint8(1);						// Activate accept
	*data << qst->quest_flags;
	*data << qst->required_players;	// "Suggested players"
	*data << uint8(0);
	/*data << uint8(1);	// Pvp warning?
	*data << uint8(0); //VLack: some 3.1.x thing - if the client answers, this byte will be sent back in the response, so it could be used for a few interesting things...
	*data << uint8(0); //new 3.3 - if you set it to 1 then it'll show the quest panel without the decline button, however the accept button won't work either, just closes the dialog. Probably the client expects the server to auto-accept the quest without client side intervention.*/

	if(qst->count_reward_choiceitem > 5) // 6 maximum, de 0 � 5 
	{
      Log.Error("SMSG_QUESTGIVER_QUEST_DETAILS",
		  "Veuillez verifier 'count_reward_choiceitem' dans la table Quests! (Id:%u)",qst->id);
	  qst->count_reward_choiceitem = 0;
	}

	*data << qst->count_reward_choiceitem; 

	if(qst->count_reward_choiceitem) // optimisation traitement
	{
	 for(i = 0; i < 6; ++i)
	 {
		if(!qst->reward_choiceitem[i]) // Ne devrait pas arriver si 'a la suite'
		{
			continue;
		}

		*data << qst->reward_choiceitem[i];
		*data << qst->reward_choiceitemcount[i];

		ip = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[i]);
		if(ip)
			*data << ip->DisplayInfoID;
		else
			*data << uint32(0);
	 }
	} // End if(qst->count_reward_choiceitem)


	if(qst->count_reward_item > 3) // 4 maximum, de 0 � 3 
	{
      Log.Error("SMSG_QUESTGIVER_QUEST_DETAILS",
		  "Veuillez verifier 'count_reward_item' dans la table Quests! (Id:%u)",qst->id);
	  qst->count_reward_item = 0;
	}

	*data << qst->count_reward_item;

	if(qst->count_reward_item)
	{
	 for(i = 0; i < 4; ++i)
	 {
		if(!qst->reward_item[i]) continue;

		*data << qst->reward_item[i];
		*data << qst->reward_itemcount[i];

		ip = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
		if(ip)
			*data << ip->DisplayInfoID;
		else
			*data << uint32(0);
	 }
	} // End if(qst->count_reward_item)

	/**data << (uint32)qst->reward_money;     // GenerateRewardMoney(_player, qst);
	*data << uint32(qst->reward_xp);        // 3.3.2.11403 Point d'xp*/
	
	//Gestion des r�compenses en Xps, argents, et argent � la place d'Xps par Randdrick
	if (plr->getLevel() >= sWorld.m_levelCap)
	{
		*data << uint32(qst->reward_xp_as_money);
		*data << uint32(0); // Pas d'Xps de gagn�s si le niveau du joueur est sup�rieur au niveau maximum
	}
	else
	{
		uint32 xp = 0;
		xp = float2int32(GenerateQuestXP(plr,qst) * sWorld.getRate(RATE_QUESTXP));
		*data << uint32(GenerateRewardMoney(plr, qst));
		*data << uint32(xp);
	}
	*data << uint32(qst->bonus_honor * 10);	// Bonus Honor
	*data << float(0);                  	// 3.3.2.11403 ??
	*data << (uint32)qst->reward_spell;		// Reward Spell Id
	*data << (uint32)qst->effect_on_player;	// Cast Spell Id
	*data << (uint32)(0);				    // Reward Rank Title ID
	*data << (uint32)qst->reward_talents;	// 3.0.2 - Reward Talents
 	*data << uint32(0);                     // 3.3.2.11403 ?? 
 	*data << uint32(0);                     // 3.3.2.11403 ??

	 // ReputationId - Faction.dbc 3.3.2.11403 (Branruz)
	*data << qst->reward_repfaction[0]; // par exemple Darnassus 69
	*data << qst->reward_repfaction[1];
	*data << qst->reward_repfaction[2];
	*data << qst->reward_repfaction[3];
	*data << qst->reward_repfaction[4];

	// QuestFactionReward.dbc 3.3.2.11403 (Branruz)
    // Seulement 2 lignes et 11 colonnes donc attention avec la base
	// ATTENTION: c'est le numero de la colonne qui est pass� et plus la valeur
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

	// Unconnu 3.3.2.11403 (Branruz)
    *data << int32(0); 
	*data << int32(0); 
	*data << int32(0); 
	*data << int32(0); 
	*data << int32(0);
    //--------- 

	*data << uint32(4);							// Quantity of emotes, always four
	*data << uint32(1);							// Emote id 1
	*data << uint32(0);							// Emote delay/player emote
	*data << uint32(1);							// Emote id 2
	*data << uint32(0);							// Emote delay/player emote
	*data << uint32(0);							// Emote id 3
	*data << uint32(0);							// Emote delay/player emote
	*data << uint32(0);							// Emote id 4
	*data << uint32(0);							// Emote delay/player emote

}

void QuestMgr::BuildRequestItems(WorldPacket *data, Quest* qst, Object* qst_giver, uint32 status, uint32 language)
{
	LocalizedQuest * lq = (language>0) ? sLocalizationMgr.GetLocalizedQuest(qst->id,language):NULL;
	ItemPrototype * it;
	data->SetOpcode( SMSG_QUESTGIVER_REQUEST_ITEMS );

	*data << uint64(qst_giver->GetGUID());
	*data << uint32(qst->id);

	if(lq)
	{
		*data << lq->Title;
		*data << ((lq->IncompleteText[0]) ? lq->IncompleteText : lq->Details);
	}
	else
	{
		*data << qst->title;
		*data << (qst->incompletetext[0] ? qst->incompletetext : qst->details);
	}
	
	*data << uint32(0);
	*data << uint32(1);
	*data << uint32(0);	// Close on cancel?	
    *data << qst->quest_flags;
    *data << qst->required_players;

	*data << qst->required_money;	   // Required Money

	// item count
	*data << qst->count_required_item;
	
	// (loop for each item)
	for(uint32 i = 0; i < qst->count_required_item; ++i)
	{
		if(qst->required_item[i])
		{
 			*data << qst->required_item[i]; 
 			*data << qst->required_itemcount[i]; 
 			it = ItemPrototypeStorage.LookupEntry(qst->required_item[i]); 
 			*data << (it ? it->DisplayInfoID : uint32(0)); 
		}
		else
		{
			*data << uint32(0);
			*data << uint32(0);
			*data << uint32(0);
		}
	}

	// wtf is this?
    if(status == QMGR_QUEST_NOT_FINISHED)
    {
	    *data << uint32(0); //incomplete button
    }
    else
    {
        *data << uint32(3); 
    }
    *data << uint32(4); // Sur offi 3.0.9 (Branruz)
	*data << uint32(8);
	*data << uint32(10);
}

void QuestMgr::BuildQuestComplete(Player*plr, Quest* qst)
{
	uint32 xp ;
	uint32 reward_talents = qst->reward_talents;	
	if(plr->getLevel() >= plr->GetUInt32Value(PLAYER_FIELD_MAX_LEVEL))
	{
		plr->ModUnsigned32Value(PLAYER_FIELD_COINAGE, qst->reward_xp_as_money);
		xp = 0;
	}else
	{
		xp = float2int32(GenerateQuestXP(plr,qst) * sWorld.getRate(RATE_QUESTXP));
		plr->GiveXP(xp, 0, false);
	}
  
	WorldPacket data( SMSG_QUESTGIVER_QUEST_COMPLETE,24 );

	data << uint32(qst->id);
	data << uint32(xp);
	data << uint32(GenerateRewardMoney(plr, qst));
	data << uint32(qst->bonus_honor);		// Honor Points
	data << uint32(qst->reward_talents); 	//Reward Talents
	//data << uint32(0);						// 3.3 Arena points
	data << uint32(qst->count_reward_item); //Reward item count

	for(uint32 i = 0; i < 4; ++i)
	{
		if(qst->reward_item[i])
		{
			data << qst->reward_item[i];
			data << qst->reward_itemcount[i];
		}
	}
	plr->GetSession()->SendPacket(&data);
}

void QuestMgr::BuildQuestList(WorldPacket *data, Object* qst_giver, Player *plr, uint32 language)
{
	uint32 status;
	list<QuestRelation *>::iterator it;
	list<QuestRelation *>::iterator st;
	list<QuestRelation *>::iterator ed;
	map<uint32, uint8> tmp_map;

	data->Initialize( SMSG_QUESTGIVER_QUEST_LIST );

	*data << qst_giver->GetGUID();
	*data << "Comment puis-je vous aider ?"; //Hello line 
	*data << uint32(1);//Emote Delay
	*data << uint32(1);//Emote

	bool bValid = false;
	if(qst_giver->GetTypeId() == TYPEID_GAMEOBJECT)
	{
		bValid = ((GameObject*)qst_giver)->HasQuests();
		if(bValid)
		{
			st = ((GameObject*)qst_giver)->QuestsBegin();
			ed = ((GameObject*)qst_giver)->QuestsEnd();
		}
	} 
	else if(qst_giver->GetTypeId() == TYPEID_UNIT)
	{
		bValid = ((Creature*)qst_giver)->HasQuests();
		if(bValid)
		{
			st = ((Creature*)qst_giver)->QuestsBegin();
			ed = ((Creature*)qst_giver)->QuestsEnd();
		}
	}

	if(!bValid)
	{
		*data << uint8(0);
		return;
	}
	
	*data << uint8(sQuestMgr.ActiveQuestsCount(qst_giver, plr));

	for (it = st; it != ed; ++it)
	{
		status = sQuestMgr.CalcQuestStatus(qst_giver, plr, *it);
		if (status >= QMGR_QUEST_CHAT)
		{
			if (tmp_map.find((*it)->qst->id) == tmp_map.end())
			{
				tmp_map.insert(std::map<uint32,uint8>::value_type((*it)->qst->id, 1));
				LocalizedQuest * lq = (language>0) ? sLocalizationMgr.GetLocalizedQuest((*it)->qst->id,language):NULL;

				*data << (*it)->qst->id;
				/**data << sQuestMgr.CalcQuestStatus(qst_giver, plr, *it);
				*data << uint32(0);*/
				
				switch(status)
				{
				case QMGR_QUEST_NOT_FINISHED:
					*data << uint32(4);
					break;

				case QMGR_QUEST_FINISHED:
					*data << uint32(5);
					break;

				case QMGR_QUEST_CHAT:
					*data << uint32( QMGR_QUEST_AVAILABLE );
					break;

				default:
					*data << status;
				}
				*data << (*it)->qst->max_level;
				*data << uint32( (*it)->qst->quest_flags );
				*data << uint8(0); // According to MANGOS: "changes icon: blue question or yellow exclamation"
				if(lq)
					*data << lq->Title;
				else
					*data << (*it)->qst->title;
			}
		}
	}
}

void QuestMgr::BuildQuestUpdateAddItem(WorldPacket* data, uint32 itemid, uint32 count)
{
	data->Initialize(SMSG_QUESTUPDATE_ADD_ITEM);
	*data << itemid << count;
}

void QuestMgr::SendQuestUpdateAddKill(Player* plr, uint32 questid, uint32 entry, uint32 count, uint32 tcount, uint64 guid)
{
	//Log.Notice("SendQuestUpdateAddKill","Player %s, Mob/Pnj Guid tu� %u",plr->GetName(),guid); // debug (Branruz)

	WorldPacket data(32); 
	data.SetOpcode(SMSG_QUESTUPDATE_ADD_KILL);
	data << (uint32)questid;
	data << (uint32)entry;
	data << (uint32)count;
	data << (uint32)tcount;
	data << (uint64)guid;
	plr->GetSession()->SendPacket(&data);
}
//Mode PVP par AspireDev
void QuestMgr::SendQuestUpdateAddPvpKill(Player* plr, uint32 questid, uint32 count, uint32 tcount, uint64 guid)
{
	sLog.outDebug("WORLD", "Sent SMSG_QUESTUPDATE_ADD_PVP_KILL");
	WorldPacket data(SMSG_QUESTUPDATE_ADD_PVP_KILL, 50);

	data << questid;
	uint32 arg = 409;
	data << count;
	data << arg;
	switch(arg)
	{
	case 406:
	case 407:
	case 408:
		{
			plr->GetSession()->SendPacket(&data);
		}break;
	case 409:
		{
			data << uint32(0);
			data << (uint32)tcount;
			data << uint64(0);  //guid
			plr->GetSession()->SendPacket(&data);
		}break;
	default:
		{
			plr->GetSession()->SendPacket(&data);
		}break;
	}
}

void QuestMgr::BuildQuestUpdateComplete(WorldPacket* data, Quest* qst)
{
	data->Initialize(SMSG_QUESTUPDATE_COMPLETE);

	*data << qst->id;
}

void QuestMgr::SendPushToPartyResponse(Player *plr, Player* pTarget, uint32 response)
{
	WorldPacket data(MSG_QUEST_PUSH_RESULT, 13);
	data << pTarget->GetGUID();
	//data << response;
	data << uint8(response);
	plr->GetSession()->SendPacket(&data);
}

bool QuestMgr::OnGameObjectActivate(Player *plr, GameObject *go)
{
	uint32 i, j;
	QuestLogEntry *qle;
	uint32 entry = go->GetEntry();

	for(i = 0; i < 25; ++i)
	{
		qle = plr->GetQuestLogInSlot( i );
		if( qle != NULL )
		{
			// dont waste time on quests without mobs
			if( qle->GetQuest()->count_required_mob == 0 )
				continue;

			for( j = 0; j < 4; ++j )
			{
				if( qle->GetQuest()->required_mob[j] == entry &&
					qle->GetQuest()->required_mobtype[j] == QUEST_MOB_TYPE_GAMEOBJECT &&
					qle->m_mobcount[j] < qle->GetQuest()->required_mobcount[j] )
				{
					// add another kill.
					// (auto-dirtys it)
					qle->SetMobCount( j, qle->m_mobcount[j] + 1 );
					qle->SendUpdateAddKill( j, go->GetGUID() ); // CreatureGuid (Branruz)
					CALL_QUESTSCRIPT_EVENT( qle, OnGameObjectActivate )( entry, plr, qle );

					if( qle->CanBeFinished() )
					{
						//Log.Warning("OnGameObjectActivate","SendQuestComplete");
						qle->SendQuestComplete();
					}

					qle->UpdatePlayerFields();
					qle->SaveToDB(NULL);
					return true;
				}
			}
		}
	}
	return false;
}

void QuestMgr::OnPlayerKill(Player* plr, Creature* victim)
{
	if(!victim) return;

	uint32 entry = victim->GetEntry();
	_OnPlayerKill( plr, entry);
}

void QuestMgr::_OnPlayerKill(Player* plr, uint32 creature_entry)
{
	if(!plr) return;

	uint32 i, j;
	QuestLogEntry *qle;

	if (plr->HasQuestMob(creature_entry))
	{
		for(i = 0; i < 25; ++i)
		{
			qle = plr->GetQuestLogInSlot( i );
			if( qle != NULL )
			{
				// dont waste time on quests without mobs
				if( qle->GetQuest()->count_required_mob == 0 )
					continue;

				for( j = 0; j < 4; ++j )
				{
					if( qle->GetQuest()->required_mob[j] == creature_entry &&
						qle->GetQuest()->required_mobtype[j] == QUEST_MOB_TYPE_CREATURE &&
						qle->m_mobcount[j] < qle->GetQuest()->required_mobcount[j] )
					{
						// don't update killcount for these questslogentries
						if ( QuestException( qle->GetQuest()->id) )
							return;

						// add another kill.(auto-dirtys it)
						qle->SetMobCount( j, qle->m_mobcount[j] + 1 );
						qle->SendUpdateAddKill( j,0 );
						CALL_QUESTSCRIPT_EVENT( qle, OnCreatureKill)( creature_entry, plr, qle );
						if( qle->CanBeFinished() )
							qle->SendQuestComplete();
						qle->UpdatePlayerFields();
						qle->SaveToDB(NULL);
						break;
					}
				 }
				
			}
		}
	}

	// Shared kills
	Player *gplr = NULL;

	if(plr->InGroup())
	{
		if(Group* pGroup = plr->GetGroup())
		{
//			removed by Zack How the hell will healers get the kills then ?
//			if(pGroup->GetGroupType() != GROUP_TYPE_PARTY) 
//				return;  // Raid's don't get shared kills.

			GroupMembersSet::iterator gitr;
			pGroup->Lock();
			for(uint32 k = 0; k < pGroup->GetSubGroupCount(); k++)
			{
				for(gitr = pGroup->GetSubGroup(k)->GetGroupMembersBegin(); gitr != pGroup->GetSubGroup(k)->GetGroupMembersEnd(); ++gitr)
				{
					gplr = (*gitr)->m_loggedInPlayer;
					if(gplr && gplr != plr && plr->isInRange(gplr,300) && gplr->HasQuestMob(creature_entry)) // dont double kills also dont give kills to party members at another side of the world
					{
						for( i = 0; i < 25; ++i )
						{
							qle = gplr->GetQuestLogInSlot(i);
							if( qle != NULL )
							{
								// dont waste time on quests without mobs
								if( qle->GetQuest()->count_required_mob == 0 )
									continue;

								for( j = 0; j < 4; ++j )
								{
									if( qle->GetQuest()->required_mob[j] == creature_entry &&
										qle->GetQuest()->required_mobtype[j] == QUEST_MOB_TYPE_CREATURE &&
										qle->m_mobcount[j] < qle->GetQuest()->required_mobcount[j] )
									{
										// don't update killcount for these questslogentries
										if ( QuestException( qle->GetQuest()->id) )
											return;

										// add another kill.
										// (auto-dirtys it)
										qle->SetMobCount(j, qle->m_mobcount[j] + 1);
										qle->SendUpdateAddKill( j ,0); // CreatureGuid (Branruz)
										CALL_QUESTSCRIPT_EVENT( qle, OnCreatureKill )( creature_entry, plr, qle );
										qle->UpdatePlayerFields();

										if( qle->CanBeFinished() )
										{
											//Log.Warning("_OnPlayerKill","SendQuestComplete");
											qle->SendQuestComplete();
										}
										qle->SaveToDB(NULL);
										break;
									}
								}
							}
						}
					}
				}
			}
			pGroup->Unlock();
		}
	}
}

void QuestMgr::OnPlayerSlain(Player* plr, Player* victim)
{
	if(!plr || !victim)
		return;

	QuestLogEntry *qle = NULL;	
	Player* gplr = NULL; // Shared kills

	uint32 i;
	for(i = 0; i < 25; ++i)
	{
		qle = plr->GetQuestLogInSlot(i);
		if(qle && qle->m_quest->required_kill_player)
		{
			qle->SetPlayerSlainCount(qle->m_player_slain + 1);
			if(qle->CanBeFinished())
				qle->SendQuestComplete();
			qle->SaveToDB(NULL);
		}
	}
	
	if(plr->InGroup())
	{
		if(Group* pGroup = plr->GetGroup())
		{
			GroupMembersSet::iterator gitr;
			pGroup->Lock();
			for(uint32 k = 0; k < pGroup->GetSubGroupCount(); k++)
			{
				for(gitr = pGroup->GetSubGroup(k)->GetGroupMembersBegin(); gitr != pGroup->GetSubGroup(k)->GetGroupMembersEnd(); ++gitr)
				{
					gplr = (*gitr)->m_loggedInPlayer;
					if(gplr && gplr != plr && plr->isInRange(gplr,300)) // dont double kills also dont give kills to party members at another side of the world
					{
						for( i = 0; i < 25; ++i )
						{
							qle = gplr->GetQuestLogInSlot(i);
							if( qle != NULL )
							{
								// dont waste time on quests without mobs
								if( qle->GetQuest()->required_kill_player == 0 )
									continue;

								qle->SetPlayerSlainCount(qle->m_player_slain + 1);

								if(qle->CanBeFinished())
									qle->SendQuestComplete();
								qle->SaveToDB(NULL);
							}
						}
					}
				}
			}
			pGroup->Unlock();
		}
	}
}

void QuestMgr::OnPlayerCast(Player* plr, uint32 spellid, uint64 victimguid)
{
	if(!plr || !plr->HasQuestSpell(spellid))
		return;

	Unit * victim = plr->GetMapMgr() ? plr->GetMapMgr()->GetUnit(victimguid) : NULL;
	if(victim==NULL)
		return;

	uint32 i, j;
	uint32 entry = victim->GetEntry();
	QuestLogEntry *qle;
	for(i = 0; i < 25; ++i)
	{
		qle = plr->GetQuestLogInSlot(i);
		if(qle)
		{
			// dont waste time on quests without casts
			if(!qle->IsCastQuest())
				continue;

			for(j = 0; j < 4; ++j)
			{
				if(qle->GetQuest()->required_mob[j] == entry &&
					qle->GetQuest()->required_spell[j] == spellid &&
					qle->m_mobcount[j] < qle->GetQuest()->required_mobcount[j] &&
					!qle->IsUnitAffected(victim))
				{
					// add another kill.(auto-dirtys it)
					qle->AddAffectedUnit(victim);
					qle->SetMobCount(j, qle->m_mobcount[j] + 1);
					qle->SendUpdateAddKill(j, victimguid); // CreatureGuid (Branruz)
					qle->UpdatePlayerFields();
					qle->SaveToDB(NULL);
					break;
				}
			}
		}
	}
}



void QuestMgr::OnPlayerItemPickup(Player* plr, Item* item)
{
	uint32 i, j;
	uint32 pcount;
	uint32 entry = item->GetEntry();
	QuestLogEntry *qle;
	for( i = 0; i < 25; ++i )
	{
		qle = plr->GetQuestLogInSlot(i);
		if(qle)
		{
			if( qle->GetQuest()->count_required_item == 0 )
				continue;

			for( j = 0; j < 6; ++j )
			{
				if( qle->GetQuest()->required_item[j] == entry )
				{
					pcount = plr->GetItemInterface()->GetItemCount(entry, true);
					CALL_QUESTSCRIPT_EVENT(qle, OnPlayerItemPickup)(entry, pcount, plr, qle);
					if(pcount < qle->GetQuest()->required_itemcount[j])
					{
						WorldPacket data(8);
						data.SetOpcode(SMSG_QUESTUPDATE_ADD_ITEM);
						data << qle->GetQuest()->required_item[j] << uint32(1);
						plr->GetSession()->SendPacket(&data);
						if(qle->CanBeFinished())
						{
							plr->UpdateNearbyGameObjects();
							//Log.Warning("OnPlayerItemPickup","SendQuestComplete");
							qle->SendQuestComplete();
						}
						break;
					}
				}
			}
		}
	}
}

void QuestMgr::OnPlayerExploreArea(Player* plr, uint32 AreaID)
{
	uint32 i, j;
	QuestLogEntry *qle;
	for( i = 0; i < 25; ++i )
	{
		qle = plr->GetQuestLogInSlot(i);
		if(qle)
		{
			// dont waste time on quests without triggers
			if( qle->GetQuest()->count_requiredtriggers == 0 )
				continue;

			for( j = 0; j < 4; ++j )
			{
				if(qle->GetQuest()->required_triggers[j] == AreaID &&
					!qle->m_explored_areas[j])
				{
					qle->SetTrigger(j);
					CALL_QUESTSCRIPT_EVENT(qle, OnExploreArea)(qle->m_explored_areas[j], plr, qle);
					qle->UpdatePlayerFields();
					if(qle->CanBeFinished())
					{
						plr->UpdateNearbyGameObjects();
						//Log.Warning("OnPlayerExploreArea","SendQuestComplete");
						qle->SendQuestComplete();
					}
					qle->SaveToDB(NULL);
					break;
				}
			}
		}
	}
}
// ATTENTION: A verif, les reput sont maintenant pass�es dans les trames reseaux
//            vers le client (Les reputs seraient maintenant g�r� par le client ??)
void QuestMgr::GiveQuestRewardReputation(Player* plr, Quest* qst, Object *qst_giver)
{
	
	/*if(sWorld.m_DebugPlayers) Log.Notice("[QuestMgr::GiveQuestRewardReputation]","(%s)",plr->GetName());

    if(!qst->reward_repfaction[0]) return; // Pas de reput pour cette quete

	for(int z = 0; z < 5; z++)
	{
      if(qst->reward_repfaction[z])
	  {
		  uint32 amt = 10; // Au cas ou y'aurait une erreur dans la base
		  uint32 fact = qst->reward_repfaction[z];
  		  if(qst->reward_repvalueIndex[z]) amt = qst->reward_repvalueIndex[z];
		  else                        
		  {
			  Log.Error("[GiveQuestRewardReputation]","Table Quests (%u): RewRepValue%u = 0!!",qst->id,z+1);
		  }
		  amt = float2int32( float( amt ) * sWorld.getRate( RATE_QUESTREPUTATION ) ); // Ascent-World.conf
   		  

		  if(qst->reward_replimit) // Test Reput Max 
		  {
			if(sWorld.m_DebugPlayers) Log.Notice("[QuestMgr::GiveQuestRewardReputation]","(%s) faction %u Reput %d / %d ",
				                                                        plr->GetName(),fact,plr->GetStanding(fact),qst->reward_replimit);
			if(plr->GetStanding(fact) >= (int32)qst->reward_replimit) // Reput Max atteinte ?
				continue;
		  }

		  if(sWorld.m_DebugPlayers) Log.Notice("[QuestMgr::GiveQuestRewardReputation]","(%s) Reputation prise %d ",plr->GetName(),amt);

		  plr->ModStanding(fact, amt); 
	  }
	  else break;
	}
	
	if(sWorld.m_DebugPlayers) Log.Notice("[QuestMgr::GiveQuestRewardReputation]","(%s) Fini!",plr->GetName());
*/	
	
    // Inspir� de Mangos et adapt� pour Waad par Randdrick
    for(int z = 0; z < 5; z++)
    {
        if (!qst->reward_repfaction[z])
            continue;

        if (qst->reward_repvalueIndex[z] && qst->reward_repvalueIndex[z] > 10) // Permet de g�rer la r�putation en tant que valeur si besoin
        {
		  uint32 amt = 0;
		  uint32 fact = qst->reward_repfaction[z];
  		  amt = qst->reward_repvalueIndex[z];
		  amt = float2int32( float( amt ) * sWorld.getRate( RATE_QUESTREPUTATION ) ); // Ascent-World.conf   		  

		  if(sWorld.m_DebugPlayers) Log.Notice("[QuestMgr::GiveQuestRewardReputation]","(%s) Reputation prise %d ",plr->GetName(),amt);

		  plr->ModStanding(fact, amt); 
		}
		else
		{		
          uint32 row = ((qst->reward_repvalueIndex[z] < 0) ? 1 : 0) + 1;
          uint32 field = abs(qst->reward_repvalueIndex[z]);

            if (const QuestFactionRewardEntry *pRow = dbcQuestFactionReward.LookupEntry(row))
            {
                int32 repPoints = pRow->rewardValue[field];

                if (!repPoints)
                    continue;

                repPoints = float2int32( float( repPoints ) * sWorld.getRate( RATE_QUESTREPUTATION ) ); // Ascent-World.conf;

                if (const FactionDBC* factionEntry = dbcFaction.LookupEntry(qst->reward_repfaction[z]))
				{
					uint32 fact = factionEntry->ID;
                    plr->ModStanding(fact, repPoints);
				}
            }
        }
    }	
}
void QuestMgr::OnQuestAccepted(Player* plr, Quest* qst, Object *qst_giver)
{
	
}

void QuestMgr::GiveQuestTitleReward(Player* plr, Quest* qst)
{
	if(!qst->reward_title || qst->reward_title > TITLE_END)
		return;

	plr->HasKnownTitle(qst->reward_title);
}

void QuestMgr::OnQuestFinished(Player* plr, Quest* qst, Object *qst_giver, uint32 reward_slot)
{

	//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s",plr->GetName());
    QuestLogEntry *qle = NULL;
	if(qst->is_repeatable != REPEATABLE_QUEST)
    {
	    qle = plr->GetQuestLogForEntry(qst->id);
	    if(!qle)
		    return;
    }
	/*else
	{
		if (plr->m_finisheddailyQuests.size() >= 25)
		{
			sChatHandler.SystemMessage(plr->GetSession(), "You cannot complete anymore daily quests today.");
			return;
		}

		if (plr->m_finisheddailyQuests.find(qst->id) != plr->m_finisheddailyQuests.end())
		{
			sChatHandler.SystemMessage(plr->GetSession(), "You have already completed this quest.");
			return;
		}
	}*/
    BuildQuestComplete(plr, qst);
	if (qle != NULL) CALL_QUESTSCRIPT_EVENT(qle, OnQuestComplete)(plr, qle);
	if(qst->is_repeatable != REPEATABLE_QUEST) 
	{
		for (uint32 x=0; x<4; x++)
		{
			if (qst->required_spell[x]!=0)
			{
				if (plr->HasQuestSpell(qst->required_spell[x]))
					plr->RemoveQuestSpell(qst->required_spell[x]);
			}
			else if (qst->required_mob[x]!=0)
			{
				if (plr->HasQuestMob(qst->required_mob[x]))
					plr->RemoveQuestMob(qst->required_mob[x]);
			} 
		}
		qle->ClearAffectedUnits();
		qle->Finish();
	}
	
	if(qst_giver->GetTypeId() == TYPEID_UNIT)
	{
		if(!((Creature*)qst_giver)->HasQuest(qst->id, 2))
		{
			//sCheatLog.writefromsession(plr->GetSession(), "tried to finish quest from invalid npc.");
			Log.Error("[QuestMgr::OnQuestFinished]"," plr %s Disconnected",plr->GetName());
			//plr->GetSession()->Disconnect();
			
			return;
		}
	}

    //details: hmm as i can remember, repeatable quests give faction rep still after first completation

    if(IsQuestRepeatable(qst))
    {
		plr->ModUnsigned32Value(PLAYER_FIELD_COINAGE, qst->reward_money);	
		// Reputation reward
		GiveQuestRewardReputation(plr, qst, qst_giver);
        // Static Item reward
	    for(uint32 i = 0; i < 4; ++i)
	    {
		    if(qst->reward_item[i])
		    {
			    ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
			    if(!proto)
			    {
				    Log.Error("[QMgr::OnQuestFinished]","Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_item[i], qst->id);
			    }
			    else
			    {   
					Item *add;
					SlotResult slotresult;
					add = plr->GetItemInterface()->FindItemLessMax(qst->reward_item[i], qst->reward_itemcount[i], false);
					if (!add)
					{
						slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
						if(!slotresult.Result)
						{
							plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
						}
						else
						{
							Item *itm = objmgr.CreateItem(qst->reward_item[i], plr);
							itm->SetUInt32Value(ITEM_FIELD_STACK_COUNT, uint32(qst->reward_itemcount[i]));
							if( !plr->GetItemInterface()->SafeAddItem(itm,slotresult.ContainerSlot, slotresult.Slot) )
								delete itm;
						}
					}
					else
					{
						add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + qst->reward_itemcount[i]);
						add->m_isDirty = true;
					}
			    }
		    }
	    }

	    // Choice Rewards
	    if(qst->reward_choiceitem[reward_slot])
	    {
		    ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[reward_slot]);
		    if(!proto)
		    {
			    sLog.outError("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_choiceitem[reward_slot], qst->id);
		    }
		    else
		    {
				Item *add;
				SlotResult slotresult;
				add = plr->GetItemInterface()->FindItemLessMax(qst->reward_choiceitem[reward_slot], qst->reward_choiceitemcount[reward_slot], false);
				if (!add)
				{
					slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
					if(!slotresult.Result)
					{
						plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
					}
					else
					{
						Item *itm = objmgr.CreateItem(qst->reward_choiceitem[reward_slot], plr);
						itm->SetUInt32Value(ITEM_FIELD_STACK_COUNT, uint32(qst->reward_choiceitemcount[reward_slot]));
						if( !plr->GetItemInterface()->SafeAddItem(itm,slotresult.ContainerSlot, slotresult.Slot) )
							delete itm;

					}
				}
				else
				{
					add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + qst->reward_choiceitemcount[reward_slot]);
					add->m_isDirty = true;
				}
		    }
	    }

	    // Remove items
	    for(uint32 i = 0; i < 4; ++i)
	    {
		    if(qst->required_item[i]) plr->GetItemInterface()->RemoveItemAmt(qst->required_item[i],qst->required_itemcount[i]);
	    }

	    // Remove srcitem   UnkReceiveItemId
//	    if(qst->srcitem && qst->srcitem != qst->receive_items[0])
		if(qst->srcitem && qst->srcitem != qst->RecItemDuringQuestId[0])
		   plr->GetItemInterface()->RemoveItemAmt(qst->srcitem, qst->srcitemcount ? qst->srcitemcount : 1);

        // cast Effect Spell
	    if(qst->effect_on_player)
	    {
		    SpellEntry  * inf =dbcSpell.LookupEntry(qst->effect_on_player);
		    if(inf)
		    {
			    Spell * spe = new Spell(qst_giver,inf,true,NULL);
			    SpellCastTargets tgt;
			    tgt.m_target = plr;
			    spe->prepare(&tgt);
		    }
	    }

		if(qst->bonus_honor)
			HonorHandler::AddHonorPointsToPlayer(plr, qst->bonus_honor);
    }
    else
    {
	    plr->ModUnsigned32Value(PLAYER_FIELD_COINAGE, qst->reward_money);
  	    //Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s reward_money",plr->GetName());
	    // Reputation reward
		
		//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s GiveQuestRewardReputation",plr->GetName());
		GiveQuestRewardReputation(plr, qst, qst_giver);
		
		// Static Item reward
	    for(uint32 i = 0; i < 4; ++i)
	    {
		    if(qst->reward_item[i])
		    {
				//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s reward_item[%u]=%u",plr->GetName(),i,qst->reward_item[i]);
			    ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
			    if(!proto)
			    {
				    Log.Error("[QuestMgr::OnQuestFinished]","Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_item[i], qst->id);
			    }
			    else
			    {   
					Item *add;
					SlotResult slotresult;
					add = plr->GetItemInterface()->FindItemLessMax(qst->reward_item[i], qst->reward_itemcount[i], false);
					if (!add)
					{
						slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
						if(!slotresult.Result)
						{
							plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
						}
						else
						{
							Item *itm = objmgr.CreateItem(qst->reward_item[i], plr);
							itm->SetUInt32Value(ITEM_FIELD_STACK_COUNT, uint32(qst->reward_itemcount[i]));
							if( !plr->GetItemInterface()->SafeAddItem(itm,slotresult.ContainerSlot, slotresult.Slot) )
								delete itm;
						}
					}
					else
					{
						add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + qst->reward_itemcount[i]);
						add->m_isDirty = true;
					}
			    }
		    }
	    }

	    // Choice Rewards
	    if(qst->reward_choiceitem[reward_slot])
	    {
			//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s reward_choiceitem[%u]=%u",plr->GetName(),reward_slot,qst->reward_choiceitem[reward_slot]);
		    ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[reward_slot]);
		    if(!proto)
		    {
			    Log.Error("OnQuestFinished","Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_choiceitem[reward_slot], qst->id);
		    }
		    else
		    {
				Item *add;
				SlotResult slotresult;
				add = plr->GetItemInterface()->FindItemLessMax(qst->reward_choiceitem[reward_slot], qst->reward_choiceitemcount[reward_slot], false);
				if (!add)
				{
					slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
					if(!slotresult.Result)
					{
						plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
					}
					else 
					{
						Item *itm = objmgr.CreateItem(qst->reward_choiceitem[reward_slot], plr);
						itm->SetUInt32Value(ITEM_FIELD_STACK_COUNT, uint32(qst->reward_choiceitemcount[reward_slot]));
						if( !plr->GetItemInterface()->SafeAddItem(itm,slotresult.ContainerSlot, slotresult.Slot) )
							delete itm;
					}
				}
				else
				{
					add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + qst->reward_choiceitemcount[reward_slot]);
					add->m_isDirty = true;
				}
		    }
	    }

	    // Remove items
	    for(uint32 i = 0; i < 4; ++i)
	    {
		    if(qst->required_item[i]) 
			{
				//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s Remove items_needed[%u]=%u",plr->GetName(),i,qst->required_item[i]);
				plr->GetItemInterface()->RemoveItemAmt(qst->required_item[i],qst->required_itemcount[i]);
			}
	    }


	    // Remove srcitem 
	   // if(qst->srcitem && qst->srcitem != qst->receive_items[0])
        if(qst->srcitem && qst->srcitem != qst->RecItemDuringQuestId[0]) 
		{
			//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s Remove items_needed[0]=%u",plr->GetName(),qst->RecItemDuringQuestId[0]);
		    plr->GetItemInterface()->RemoveItemAmt(qst->srcitem, qst->srcitemcount ? qst->srcitemcount : 1);
		}

	    // cast learning spell
	    if(qst->reward_spell)
	    {
			//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s reward_spell",plr->GetName());
		    if(!plr->HasSpell(qst->reward_spell))
		    {
				//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s SMSG_SPELL_START",plr->GetName());
			    // "Teaching" effect
			    WorldPacket data(SMSG_SPELL_START, 42);
			    data << qst_giver->GetNewGUID() << qst_giver->GetNewGUID();
			    data << uint32(7763);
				data << uint8(0);
			    data << uint16(0);
			    data << uint32(0);
			    data << uint16(2);
			    data << plr->GetGUID();
			    plr->GetSession()->SendPacket( &data );

				//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s SMSG_SPELL_GO",plr->GetName());
			    data.Initialize( SMSG_SPELL_GO );
			    data << qst_giver->GetNewGUID() << qst_giver->GetNewGUID();
			    data << uint32(7763);		   // spellID
			    data << uint8(0) << uint8(1);   // flags
			    data << uint8(1);			   // amount of targets
			    data << plr->GetGUID();		 // target
			    data << uint8(0);
			    data << uint16(2);
			    data << plr->GetGUID();
			    plr->GetSession()->SendPacket( &data );

			    // Teach the spell
			    plr->addSpell(qst->reward_spell);
		    }
	    }

	    // cast Effect Spell
	    if(qst->effect_on_player)
	    {
			//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s Effect Spell",plr->GetName());
		    SpellEntry  * inf =dbcSpell.LookupEntry(qst->effect_on_player);
		    if(inf)
		    {
			    Spell * spe = new Spell(qst_giver,inf,true,NULL);
			    SpellCastTargets tgt;
			    tgt.m_target = plr;
			    spe->prepare(&tgt);
		    }
	    }
		//Patch AspireDev
		if(qst->bonus_honor)
		{
			//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s bonus_honor %u",plr->GetName(),qst->bonus_honor);
			HonorHandler::AddHonorPointsToPlayer(plr, qst->bonus_honor);
		}

		if(qst->reward_talents)
		{
			//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s reward_talents %u",plr->GetName(),qst->reward_talents);
			plr->SetUInt32Value(PLAYER_CHARACTER_POINTS1, plr->GetUInt32Value(PLAYER_CHARACTER_POINTS1) + qst->reward_talents);
		}
		//Fin patch AspireDev	

	    //Add to finished quests
		if(qst->is_repeatable == REPEATABLE_DAILY)             
		{
			//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s REPEATABLE_DAILY",plr->GetName());
			plr->AddToFinishedDailyQuests(qst->id);
		}
	    else if(IsQuestRepeatable(qst) == NO_REPEATABLE_QUEST) 
		{
			//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s NO_REPEATABLE_QUEST",plr->GetName());
			plr->AddToFinishedQuests(qst->id);
		}
		

		if(qst->zone_id)
		{
		  //Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s zone_id GetAchievementInterface %u",plr->GetName(),qst->zone_id);
  		  plr->GetAchievementInterface()->HandleAchievementCriteriaCompleteQuestsInZone(qst->zone_id);
		}

	}
		//Log.Notice("[QuestMgr::OnQuestFinished]"," plr %s Fini!",plr->GetName());
		
}

/////////////////////////////////////
//		Quest Management		 //
/////////////////////////////////////

void QuestMgr::LoadNPCQuests(Creature *qst_giver)
{
	qst_giver->SetQuestList(GetCreatureQuestList(qst_giver->GetEntry()));
}

void QuestMgr::LoadGOQuests(GameObject *go)
{
	go->SetQuestList(GetGOQuestList(go->GetEntry()));
}

QuestRelationList* QuestMgr::GetGOQuestList(uint32 entryid)
{
	HM_NAMESPACE::hash_map<uint32, QuestRelationList* > &olist = _GetList<GameObject>();
	HM_NAMESPACE::hash_map<uint32, QuestRelationList* >::iterator itr = olist.find(entryid);
	return (itr == olist.end()) ? 0 : itr->second;
}

QuestRelationList* QuestMgr::GetCreatureQuestList(uint32 entryid)
{
	HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* > &olist = _GetList<Creature>();
	HM_NAMESPACE::hash_map<uint32, QuestRelationList* >::iterator itr = olist.find(entryid);
	return (itr == olist.end()) ? 0 : itr->second;
}

template <class T> void QuestMgr::_AddQuest(uint32 entryid, Quest *qst, uint8 type)
{
	HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* > &olist = _GetList<T>();
	std::list<QuestRelation *>* nlist;
	QuestRelation *ptr = NULL;

	if (olist.find(entryid) == olist.end())
	{
		nlist = new std::list<QuestRelation *>;

		olist.insert(HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* >::value_type(entryid, nlist));
	}
	else
	{
		nlist = olist.find(entryid)->second;
	}

	list<QuestRelation *>::iterator it;
	for (it = nlist->begin(); it != nlist->end(); ++it)
	{
		if ((*it)->qst == qst)
		{
			ptr = (*it);
			break;
		}
	}

	if (ptr == NULL)
	{
		ptr = new QuestRelation;
		ptr->qst = qst;
		ptr->type = type;

		nlist->push_back(ptr);
	}
	else
	{
		ptr->type |= type;
	}
}



void QuestMgr::_CleanLine(std::string *str) 
{
	_RemoveChar((char*)"\r", str);
	_RemoveChar((char*)"\n", str);

	while (str->c_str()[0] == 32) 
	{
		str->erase(0,1);
	}
}

void QuestMgr::_RemoveChar(char *c, std::string *str) 
{
	string::size_type pos = str->find(c,0);

	while (pos != string::npos)
	{
		str->erase(pos, 1);
		pos = str->find(c, 0);
	}	
}

uint32 QuestMgr::GenerateRewardMoney( Player *plr, Quest * qst )
{
	if ( qst->reward_money < 0 ) return(0);

	if(!plr) return(0);

	/*if ( (plr->getLevel() == plr->GetUInt32Value(PLAYER_FIELD_MAX_LEVEL)) && (qst->is_repeatable == 0) )
		return qst->reward_money + float2int32( GenerateQuestXP( plr, qst ) * sWorld.getRate( RATE_QUESTXP ) ) * 6;*/
	else
		return qst->reward_money;
}

uint32 QuestMgr::GenerateQuestXP(Player *plr, Quest *qst)	
{	
 if(qst->is_repeatable) return (uint32)(0);	

 if(plr->getLevel() >= sWorld.m_levelCap) return((uint32)0); // Evite l'xp si levelmax
 
	if(qst->reward_xp > 10)
	{
		if( plr->getLevel() <= qst->max_level +  5 ) return (uint32) qst->reward_xp;	
		if( plr->getLevel() == qst->max_level +  6 ) return (uint32)(qst->reward_xp * 0.8);
		if( plr->getLevel() == qst->max_level +  7 ) return (uint32)(qst->reward_xp * 0.6);
		if( plr->getLevel() == qst->max_level +  8 ) return (uint32)(qst->reward_xp * 0.4);
		if( plr->getLevel() == qst->max_level +  9 ) return (uint32)(qst->reward_xp * 0.2);
	}
	
	// gestion du nouveau syst�me d'XP en provenance de Mangos, adapt�e pour WAAD
	else if (plr)
    {
        uint32 realXP = 0;
        uint32 xpMultiplier = 0;
        int32 baseLevel = 0;
        int32 playerLevel = plr->getLevel();
		int32 QuestLevel = qst->max_level;
		
        // formula can possibly be organized better, using less if's and simplify some.

        if (QuestLevel != -1)
            baseLevel = QuestLevel;

        if (((baseLevel - playerLevel) + 10)*2 > 10)
        {
            baseLevel = playerLevel;

            if (QuestLevel != -1)
                baseLevel = QuestLevel;

            if (((baseLevel - playerLevel) + 10)*2 <= 10)
            {
                if (QuestLevel == -1)
                    baseLevel = playerLevel;

                xpMultiplier = 2 * (baseLevel - playerLevel) + 20;
            }
            else
            {
                xpMultiplier = 10;
            }
        }
        else
        {
            baseLevel = playerLevel;

            if (QuestLevel != -1)
                baseLevel = QuestLevel;

            if (((baseLevel - playerLevel) + 10)*2 >= 1)
            {
                baseLevel = playerLevel;

                if (QuestLevel != -1)
                    baseLevel = QuestLevel;

                if (((baseLevel - playerLevel) + 10)*2 <= 10)
                {
                    if (QuestLevel == -1)
                        baseLevel = playerLevel;

                    xpMultiplier = 2 * (baseLevel - playerLevel) + 20;
                }
                else
                {
                    xpMultiplier = 10;
                }
            }
            else
            {
                xpMultiplier = 1;
            }
        }

        // not possible to reward XP when baseLevel does not exist in dbc
        if (const QuestXPLevel* pXPData = dbcQuestXPLevel.LookupEntry(baseLevel))
        {
            uint32 rawXP = xpMultiplier * pXPData->xpIndex[qst->reward_xp] / 10;

            // round values
            if (rawXP > 1000)
                realXP = ((rawXP + 25) / 50 * 50);
            else if (rawXP > 500)
                realXP = ((rawXP + 12) / 25 * 25);
            else if (rawXP > 100)
                realXP = ((rawXP + 5) / 10 * 10);
            else
                realXP = ((rawXP + 2) / 5 * 5);
        }

        return realXP;
    }

    return 0;	
}
/*
#define XP_INC 50
#define XP_DEC 10
#define XP_INC100 15
#define XP_DEC100 5
	double xp, pxp, mxp, mmx;

	// hack fix
	xp  = qst->max_level * XP_INC;
	if(xp <= 0)
		xp = 1;

	pxp  = xp + (xp / 100) * XP_INC100;

	xp   = XP_DEC;

	mxp  = xp + (xp / 100) * XP_DEC100;

	mmx = (pxp - mxp);

	if(qst->quest_flags & QUEST_FLAG_SPEAKTO)
		mmx *= 0.6;
	if(qst->quest_flags & QUEST_FLAG_TIMED)
		mmx *= 1.1;
	if(qst->quest_flags & QUEST_FLAG_EXPLORATION)
		mmx *= 1.2;

	if(mmx < 0)
		return 1;

	mmx *= sWorld.getRate(RATE_QUESTXP);
	return (int)mmx;*/


void QuestMgr::SendQuestInvalid(INVALID_REASON reason, Player *plyr)
{
	if(!plyr)
		return;

	plyr->GetSession()->OutPacket(SMSG_QUESTGIVER_QUEST_INVALID, 4, &reason);

	sLog.outDebug("WORLD:Sent SMSG_QUESTGIVER_QUEST_INVALID");
}

void QuestMgr::SendQuestFailed(FAILED_REASON failed, Quest * qst, Player *plyr)
{
	if(!plyr)
		return;

    WorldPacket data(8);
    data.Initialize(SMSG_QUESTGIVER_QUEST_FAILED);
    data << uint32(qst->id);
    data << failed;
    plyr->GetSession()->SendPacket(&data);
	sLog.outDebug("WORLD:Sent SMSG_QUESTGIVER_QUEST_FAILED");
}

void QuestMgr::SendQuestUpdateFailedTimer(Quest *pQuest, Player *plyr)
{
	if(!plyr)
		return;

	plyr->GetSession()->OutPacket(SMSG_QUESTUPDATE_FAILEDTIMER, 4, &pQuest->id);
	sLog.outDebug("WORLD:Sent SMSG_QUESTUPDATE_FAILEDTIMER");
}

void QuestMgr::SendQuestUpdateFailed(Quest *pQuest, Player *plyr)
{
	if(!plyr)
		return;

	plyr->GetSession()->OutPacket(SMSG_QUESTUPDATE_FAILED, 4, &pQuest->id);
	sLog.outDebug("WORLD:Sent SMSG_QUESTUPDATE_FAILED");
}

void QuestMgr::SendQuestLogFull(Player *plyr)
{
	if(!plyr)
		return;

	plyr->GetSession()->OutPacket(SMSG_QUESTLOG_FULL);
	sLog.outDebug("WORLD:Sent QUEST_LOG_FULL_MESSAGE");
}

uint32 QuestMgr::GetGameObjectLootQuest(uint32 GO_Entry)
{
	HM_NAMESPACE::hash_map<uint32, uint32>::iterator itr = m_ObjectLootQuestList.find(GO_Entry);
	if(itr == m_ObjectLootQuestList.end()) return 0;
	
	return itr->second;
}

void QuestMgr::SetGameObjectLootQuest(uint32 GO_Entry, uint32 Item_Entry)
{
	if(m_ObjectLootQuestList.find(GO_Entry) != m_ObjectLootQuestList.end())
	{
		//sLog.outError("WARNING: Gameobject %d has more than 1 quest item allocated in it's loot template!", GO_Entry);
	}

	// Find the quest that has that item
	uint32 QuestID = 0;
	uint32 i;
	StorageContainerIterator<Quest> * itr = QuestStorage.MakeIterator();
	while(!itr->AtEnd())
	{
		Quest *qst = itr->Get();
		for(i = 0; i < 4; ++i)
		{
			if(qst->required_item[i] == Item_Entry)
			{
				QuestID = qst->id;
				m_ObjectLootQuestList[GO_Entry] = QuestID;
				itr->Destruct();
				return;
			}
		}
		if(!itr->Inc())
			break;
	}
	itr->Destruct();

	//sLog.outError("WARNING: No coresponding quest was found for quest item %d", Item_Entry);
}

void QuestMgr::BuildQuestFailed(WorldPacket* data, uint32 questid)
{
	data->Initialize(SMSG_QUESTUPDATE_FAILEDTIMER);
	*data << questid;
}

bool QuestMgr::OnActivateQuestGiver(Object *qst_giver, Player *plr)
{
	uint32 status = 0;

	if(qst_giver->GetTypeId() == TYPEID_GAMEOBJECT && !((GameObject*)qst_giver)->HasQuests())
		return false;

	uint32 questCount = sQuestMgr.ActiveQuestsCount(qst_giver, plr);
	WorldPacket data(1000);	

	if (questCount == 0) 
	{
		sLog.outDebug("WORLD: Invalid NPC for CMSG_QUESTGIVER_HELLO.");
		return false;
	}
	else if (questCount == 1)
	{
		std::list<QuestRelation *>::const_iterator itr;
		std::list<QuestRelation *>::const_iterator q_begin;
		std::list<QuestRelation *>::const_iterator q_end;

		bool bValid = false;

		if(qst_giver->GetTypeId() == TYPEID_GAMEOBJECT)
		{
            bValid = ((GameObject*)qst_giver)->HasQuests();
            if(bValid)
            {
				q_begin = ((GameObject*)qst_giver)->QuestsBegin();
				q_end   = ((GameObject*)qst_giver)->QuestsEnd();
			}
		} 
		else if(qst_giver->GetTypeId() == TYPEID_UNIT)
		{
			bValid = ((Creature*)qst_giver)->HasQuests();
			if(bValid)
			{
				q_begin = ((Creature*)qst_giver)->QuestsBegin();
				q_end   = ((Creature*)qst_giver)->QuestsEnd();
			}
		}

		if(!bValid)
		{
			sLog.outDebug("QUESTS: Warning, invalid NPC "I64FMT" specified for OnActivateQuestGiver. TypeId: %d.", qst_giver->GetGUID(), qst_giver->GetTypeId());
			return false;
		}
		
		for(itr = q_begin; itr != q_end; ++itr) 
			if (sQuestMgr.CalcQuestStatus(qst_giver, plr, *itr) >= QMGR_QUEST_CHAT)
				break;

		if (sQuestMgr.CalcStatus(qst_giver, plr) < QMGR_QUEST_CHAT)
			return false; 

		ASSERT(itr != q_end);

		status = sQuestMgr.CalcStatus(qst_giver, plr);

		if (status == QMGR_QUEST_AVAILABLE || status == QMGR_QUEST_REPEATABLE || status == QMGR_QUEST_CHAT)
		{
			sQuestMgr.BuildQuestDetails(&data, (*itr)->qst, qst_giver, 1, plr->GetSession()->language, plr->GetSession()->GetPlayer());		// 1 because we have 1 quest, and we want goodbye to function
			plr->GetSession()->SendPacket(&data);
			sLog.outDebug( "WORLD: Sent SMSG_QUESTGIVER_QUEST_DETAILS." );
		}
		else if (status == QMGR_QUEST_FINISHED)
		{
			sQuestMgr.BuildOfferReward(&data, (*itr)->qst, qst_giver, 1, plr->GetSession()->language,plr);
			plr->GetSession()->SendPacket(&data);
			//ss
			sLog.outDebug( "WORLD: Sent SMSG_QUESTGIVER_OFFER_REWARD." );
		}
		else if (status == QMGR_QUEST_NOT_FINISHED)
		{
			sQuestMgr.BuildRequestItems(&data, (*itr)->qst, qst_giver, status, plr->GetSession()->language);
			plr->GetSession()->SendPacket(&data);
			sLog.outDebug( "WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS." );
		}
	}
	else 
	{
		sQuestMgr.BuildQuestList(&data, qst_giver ,plr, plr->GetSession()->language);
		plr->GetSession()->SendPacket(&data);
		sLog.outDebug( "WORLD: Sent SMSG_QUESTGIVER_QUEST_LIST." );
	}
	return true;
}

QuestMgr::~QuestMgr()
{
	HM_NAMESPACE::hash_map<uint32, Quest*>::iterator itr1;
	HM_NAMESPACE::hash_map<uint32, list<QuestRelation *>* >::iterator itr2;
	list<QuestRelation*>::iterator itr3;
	HM_NAMESPACE::hash_map<uint32, list<QuestAssociation *>* >::iterator itr4;
	std::list<QuestAssociation*>::iterator itr5;

	// clear relations
	for(itr2 = m_obj_quests.begin(); itr2 != m_obj_quests.end(); ++itr2)
	{
		if(!itr2->second)
			continue;

		itr3 = itr2->second->begin();
		for(; itr3 != itr2->second->end(); ++itr3)
		{
			delete (*itr3);
		}
		itr2->second->clear();
		delete itr2->second;
	}
	m_obj_quests.clear();

	for(itr2 = m_npc_quests.begin(); itr2 != m_npc_quests.end(); ++itr2)
	{
		if(!itr2->second)
			continue;

		itr3 = itr2->second->begin();
		for(; itr3 != itr2->second->end(); ++itr3)
		{
			delete (*itr3);
		}
		itr2->second->clear();
		delete itr2->second;
	}
	m_npc_quests.clear();

	for(itr2 = m_itm_quests.begin(); itr2 != m_itm_quests.end(); ++itr2)
	{
		if(!itr2->second)
			continue;

		itr3 = itr2->second->begin();
		for(; itr3 != itr2->second->end(); ++itr3)
		{
			delete (*itr3);
		}
		itr2->second->clear();
		delete itr2->second;
	}
	m_itm_quests.clear();

	for(itr4 = m_quest_associations.begin(); itr4 != m_quest_associations.end(); ++itr4)
	{
		if(!itr4->second)
			continue;

		itr5 = itr4->second->begin();
		for(; itr5 != itr4->second->end(); ++itr5)
		{
			delete (*itr5);
		}
		itr4->second->clear();
		delete itr4->second;
	}
	m_quest_associations.clear();
}


bool QuestMgr::CanStoreReward(Player *plyr, Quest *qst, uint32 reward_slot)
{
    uint32 available_slots = 0;
    uint32 slotsrequired = 0;
    available_slots = plyr->GetItemInterface()->CalculateFreeSlots(NULL);
    // Static Item reward
    for(uint32 i = 0; i < 4; ++i)
    {
        if(qst->reward_item[i])
        {
            slotsrequired++;
            ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(qst->reward_item[i]);
            if(!proto)
                sLog.outError("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_item[i], qst->id);
            else if(plyr->GetItemInterface()->CanReceiveItem(proto, qst->reward_itemcount[i]))
				return false;
        }
    }

    // Choice Rewards
    if(qst->reward_choiceitem[reward_slot])
    {
        slotsrequired++;
        ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(qst->reward_choiceitem[reward_slot]);
        if(!proto)
            sLog.outError("Invalid item prototype in quest reward! ID %d, quest %d", qst->reward_choiceitem[reward_slot], qst->id);
        else if(plyr->GetItemInterface()->CanReceiveItem(proto, qst->reward_choiceitemcount[reward_slot]))
			return false;
    }
    if(available_slots < slotsrequired)
	{
        return false;
	}

	return true;
}

void QuestMgr::LoadExtraQuestStuff()
{
	StorageContainerIterator<Quest> * it = QuestStorage.MakeIterator();
	Quest * qst;
	while(!it->AtEnd())
	{
		qst = it->Get();

		// 0 them out
		qst->count_required_item = 0;
		qst->count_required_mob = 0;
		qst->count_requiredtriggers = 0;
		qst->count_receiveitems = 0;
		qst->count_reward_item = 0;
		qst->count_reward_choiceitem = 0;
		qst->reward_xp_as_money = 0;

		qst->required_mobtype[0] = 0;
		qst->required_mobtype[1] = 0;
		qst->required_mobtype[2] = 0;
		qst->required_mobtype[3] = 0;

		qst->count_requiredquests = 0;

		for(int i = 0 ; i < 4; ++i)
		{
			if(qst->required_mob[i])
			{
				GameObjectInfo *go_info = GameObjectNameStorage.LookupEntry(qst->required_mob[i]);
				CreatureInfo   *c_info  = CreatureNameStorage.LookupEntry(qst->required_mob[i]);
				                                         // 10 = GOOBER                 0x10 EXPLORATION
				if(go_info && (go_info->Type == GAMEOBJECT_TYPE_GOOBER || qst->quest_flags == QUEST_FLAG_EXPLORATION || !c_info) && qst->quest_flags != QUEST_FLAG_KILL )
					qst->required_mobtype[i] = QUEST_MOB_TYPE_GAMEOBJECT;    
				else
					qst->required_mobtype[i] = QUEST_MOB_TYPE_CREATURE;

				qst->count_required_mob++;
			}

			if(qst->required_item[i])
				qst->count_required_item++;

			if(qst->reward_item[i])
				qst->count_reward_item++;

			if(qst->required_triggers[i])
				qst->count_requiredtriggers++;

			//if(qst->receive_items[i]) // OBSOLETE
			//qst->count_receiveitems++;
			if(qst->RecItemDuringQuestId[i])
				qst->count_receiveitems++;

			if(qst->required_quests[i])
				qst->count_requiredquests++;
		}

		for(int i = 0; i < 6; ++i)
		{
			if(qst->reward_choiceitem[i])
				qst->count_reward_choiceitem++;
		}

		if(!it->Inc())
			break;
	}

	it->Destruct();

	// load creature starters
	uint32 creature, quest;

	QueryResult * pResult = WorldDatabase.Query("SELECT * FROM creature_quest_starter");
	uint32 pos = 0;
	uint32 total;
	if(pResult)
	{
		total = pResult->GetRowCount();
		do 
		{
			Field *data = pResult->Fetch();
			creature = data[0].GetUInt32();
			quest = data[1].GetUInt32();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
			{
				//printf("Tried to add starter to npc %d for non-existant quest %d.\n", creature, quest);
			}
			else 
			{
				_AddQuest<Creature>(creature, qst, 1);  // 1 = starter
			}
		} while(pResult->NextRow());
		delete pResult;
	}

	pResult = WorldDatabase.Query("SELECT * FROM creature_quest_finisher");
	pos = 0;
	if(pResult)
	{
		total = pResult->GetRowCount();
		do 
		{
			Field *data = pResult->Fetch();
			creature = data[0].GetUInt32();
			quest = data[1].GetUInt32();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
			{
				//printf("Tried to add finisher to npc %d for non-existant quest %d.\n", creature, quest);
			} 
			else 
			{
				_AddQuest<Creature>(creature, qst, 2);  // 1 = starter
			}
		} while(pResult->NextRow());
		delete pResult;
	}

	pResult = WorldDatabase.Query("SELECT * FROM gameobject_quest_starter");
	pos = 0;
	if(pResult)
	{
		total = pResult->GetRowCount();
		do 
		{
			Field *data = pResult->Fetch();
			creature = data[0].GetUInt32();
			quest = data[1].GetUInt32();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
			{
				//printf("Tried to add starter to go %d for non-existant quest %d.\n", creature, quest);
			} 
			else
			{
				_AddQuest<GameObject>(creature, qst, 1);  // 1 = starter
			}
		} while(pResult->NextRow());
		delete pResult;
	}

	pResult = WorldDatabase.Query("SELECT * FROM gameobject_quest_finisher");
	pos = 0;
	if(pResult)
	{
		total = pResult->GetRowCount();
		do 
		{
			Field *data = pResult->Fetch();
			creature = data[0].GetUInt32();
			quest = data[1].GetUInt32();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
			{
				//printf("Tried to add finisher to go %d for non-existant quest %d.\n", creature, quest);
			} 
			else 
			{
				_AddQuest<GameObject>(creature, qst, 2);  // 2 = finish
			}
		} while(pResult->NextRow());
		delete pResult;
	}
	objmgr.ProcessGameobjectQuests();

	//load item quest associations
	uint32 item;
	uint8 item_count;

	pResult = WorldDatabase.Query("SELECT * FROM item_quest_association");
	pos = 0;
	if( pResult != NULL)
	{
		total = pResult->GetRowCount();
		do 
		{
			Field *data = pResult->Fetch();
			item = data[0].GetUInt32();
			quest = data[1].GetUInt32();
			item_count = data[2].GetUInt8();

			qst = QuestStorage.LookupEntry(quest);
			if(!qst)
			{
				//printf("Tried to add association to item %d for non-existant quest %d.\n", item, quest);
			} 
			else 
			{
				AddItemQuestAssociation( item, qst, item_count );
			}
		} while( pResult->NextRow() );
		delete pResult;
	}
}

void QuestMgr::AddItemQuestAssociation( uint32 itemId, Quest *qst, uint8 item_count)
{
	HM_NAMESPACE::hash_map<uint32, list<QuestAssociation *>* > &associationList = GetQuestAssociationList();
	std::list<QuestAssociation *>* tempList;
	QuestAssociation *ptr = NULL;
	
	// look for the item in the associationList
	if (associationList.find( itemId ) == associationList.end() )
	{
		// not found. Create a new entry and QuestAssociationList
		tempList = new std::list<QuestAssociation *>;

		associationList.insert(HM_NAMESPACE::hash_map<uint32, list<QuestAssociation *>* >::value_type(itemId, tempList));
	}
	else
	{
		// item found, now we'll search through its QuestAssociationList
		tempList = associationList.find( itemId )->second;
	}
	
	// look through this item's QuestAssociationList for a matching quest entry
	list<QuestAssociation *>::iterator it;
	for (it = tempList->begin(); it != tempList->end(); ++it)
	{
		if ((*it)->qst == qst)
		{
			// matching quest found
			ptr = (*it);
			break;
		}
	}

	// did we find a matching quest?
	if (ptr == NULL)
	{
		// nope, create a new QuestAssociation for this item and quest
		ptr = new QuestAssociation;
		ptr->qst = qst;
		ptr->item_count = item_count;

		tempList->push_back( ptr );
	}
	else
	{
		// yep, update the QuestAssociation with the new item_count information 
		ptr->item_count = item_count;
		sLog.outDebug( "WARNING: Duplicate entries found in item_quest_association, updating item #%d with new item_count: %d.", itemId, item_count );
	}
}

QuestAssociationList* QuestMgr::GetQuestAssociationListForItemId (uint32 itemId)
{
	HM_NAMESPACE::hash_map<uint32, QuestAssociationList* > &associationList = GetQuestAssociationList();
	HM_NAMESPACE::hash_map<uint32, QuestAssociationList* >::iterator itr = associationList.find( itemId );
	if( itr == associationList.end() )
	{
		return 0;
	} else {
		return itr->second;
	}
}
// G�n�ralisation d'apres une id�e d'HearthStone (Branruz)
// Ce principe est excelent, evitera surement le crash sur certaines quetes
bool QuestMgr::QuestException( uint32 QuestID )
{
	// Mis en DB (Branruz)
	QueryResult *pResult = WorldDatabase.Query("SELECT * FROM quests_exception WHERE Quest_Id=%u", QuestID);
	if(pResult) return(true);
	return(false);
/*
	switch(QuestID)
	{
		case 6061:
		case 6062:
		case 6063:
		case 6064:
		case 6082:
		case 6083:
		case 6084:
		case 6085:
		case 6087:
		case 6088:
		case 6101:
		case 6102:
		case 9484:
		case 9485:
		case 9486:
		case 9591:
		case 9592:
		case 9593:
			return true;
	}
	return false;
*/

}
