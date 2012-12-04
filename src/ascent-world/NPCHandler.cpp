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

trainertype trainer_types[TRAINER_TYPE_MAX] = 
{
{	"Guerrier",		  0 },
{	"Paladin",		  0 },
{	"Voleur"  ,		  0 },
{	"Demoniste",	  0 },
{	"Mage",			  0 },
{	"Chaman",		  0 },
{	"Pretre",		  0 },
{	"Chasseur",		  0 },
{	"Druide",		  0 },
{	"Travail du cuir",2 },
{	"Depeceur",		  2 },
{	"Peche",		  2 },
{	"Premier soin",	  2 },
{	"Physician",	  2 },
{	"Ingenieur",	  2 },
{	"Maitre d arme",  0 },
};

bool CanTrainAt(Player * plr, Trainer * trn)
{
	if ( (trn->RequiredClass && plr->getClass() != trn->RequiredClass) ||
		 (trn->RequiredSkill && !plr->_HasSkillLine(trn->RequiredSkill)) ||
		 (trn->RequiredSkillLine && plr->_GetSkillLineCurrent(trn->RequiredSkill) < trn->RequiredSkillLine) )
	{
		return false;
	}
	
	return true;
}

//////////////////////////////////////////////////////////////
/// This function handles MSG_TABARDVENDOR_ACTIVATE:
//////////////////////////////////////////////////////////////
void WorldSession::HandleTabardVendorActivateOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	uint64 guid;
	recv_data >> guid;
	Creature *pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature) return;

	SendTabardHelp(pCreature);
}

void WorldSession::SendTabardHelp(Creature* pCreature)
{
	if(!_player->IsInWorld()) return;
	WorldPacket data(8);
	data.Initialize( MSG_TABARDVENDOR_ACTIVATE );
	data << pCreature->GetGUID();
	SendPacket( &data );
}


//////////////////////////////////////////////////////////////
/// This function handles CMSG_BANKER_ACTIVATE:
//////////////////////////////////////////////////////////////
void WorldSession::HandleBankerActivateOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	uint64 guid;
	recv_data >> guid;

	Creature *pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature)
		return;
 
	//cheatenginelolol
	if (pCreature->CalcDistance(_player) > 10.0f)
		return;


	SendBankerList(pCreature);
}

void WorldSession::SendBankerList(Creature* pCreature)
{
	if(!_player->IsInWorld()) return;
	WorldPacket data(8);
	data.Initialize( SMSG_SHOW_BANK );
	data << pCreature->GetGUID();
	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_TRAINER_LIST
//////////////////////////////////////////////////////////////
//NOTE: we select prerequirements for spell that TEACHES you
//not by spell that you learn!
void WorldSession::HandleTrainerListOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	// Inits, grab creature, check.
	sLog.outDebug( "WORLD: Received CMSG_TRAINER_LIST from %s",GetPlayer()->GetName() );
	uint64 guid;
	recv_data >> guid;
	Creature *train = GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!train) return;

	_player->Reputation_OnTalk(train->m_factionDBC);
	SendTrainerList(train);
}

void WorldSession::SendTrainerList(Creature* pCreature)
{
	Trainer * pTrainer = pCreature->GetTrainer();
	//if(pTrainer == 0 || !CanTrainAt(_player, pTrainer)) return;
	if(pTrainer==0)
		return;

	if(!CanTrainAt(_player,pTrainer))
	{
		GossipMenu * pMenu;
		objmgr.CreateGossipMenuForPlayer(&pMenu,pCreature->GetGUID(),pTrainer->Cannot_Train_GossipTextId,_player);
		pMenu->SendTo(_player);
		return;
	}

	WorldPacket data(SMSG_TRAINER_LIST, 5000);
	TrainerSpell * pSpell;
	uint32 Spacer = 0;
	uint32 Count=0;
	uint8 Status;
	string Text;

	data << pCreature->GetGUID();
	data << pTrainer->TrainerType;
	data << uint32(0); 

	for(vector<TrainerSpell>::iterator itr = pTrainer->Spells.begin(); itr != pTrainer->Spells.end(); ++itr)
	{
		pSpell = &(*itr);
		Status = TrainerGetSpellStatus(pSpell);
		if( pSpell->pCastRealSpell != NULL )
			data << pSpell->pCastRealSpell->Id;
		else if( pSpell->pLearnSpell )
			data << pSpell->pLearnSpell->Id;
		else
			continue;

		data << Status;
		data << pSpell->Cost;
		data << Spacer;
		data << uint32(pSpell->IsProfession);
		data << uint8(pSpell->RequiredLevel);
		data << pSpell->RequiredSkillLine;
		//data << (uint8)0x00;            // Test 3.0.3 WaadTeam
		//data << pSpell->RequiredSpell;  //  Test 3.0.3 waadTeam
		data << pSpell->RequiredSkillLineValue;
		data << pSpell->RequiredSpell;  // Deplacé 3.0.3, WaadTeam
		data << Spacer;	
		data << Spacer; // Test 3.0.3, En Commentaire, WaadTeam
		++Count;
	}

#ifdef USING_BIG_ENDIAN
	*(uint32*)&data.contents()[12] = swap32(Count);
#else
	*(uint32*)&data.contents()[12] = Count;
#endif

	data << pTrainer->UIMessage;
	SendPacket(&data);
}
// CMSG_TRAINER_BUY_SPELL
void WorldSession::HandleTrainerBuySpellOpcode(WorldPacket& recvPacket)
{

	sLog.outDetail("WORLD: CMSG_TRAINER_BUY_SPELL");
	if(!_player->IsInWorld()) return;

	uint64 Guid;
	uint32 TeachingSpellID;

	recvPacket >> Guid >> TeachingSpellID;

	sLog.outDetail("WORLD: CMSG_TRAINER_BUY_SPELL Guid %u pellID %u",GET_LOWGUID_PART(Guid), TeachingSpellID );

	Creature *pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(Guid));
	if(pCreature == 0) return;

	Trainer *pTrainer = pCreature->GetTrainer();
	if(pTrainer == 0 || !CanTrainAt(_player, pTrainer)) return;

	TrainerSpell * pSpell=NULL;
	for(vector<TrainerSpell>::iterator itr = pTrainer->Spells.begin(); itr != pTrainer->Spells.end(); ++itr)
	{
		if( ( itr->pCastRealSpell && itr->pCastRealSpell->Id == TeachingSpellID ) ||
			( itr->pLearnSpell && itr->pLearnSpell->Id == TeachingSpellID ) )
		{
			pSpell = &(*itr);
		}
	}
	
	if(pSpell == NULL)
		return;

	if(TrainerGetSpellStatus(pSpell) > 0) return;
	
	_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -(int32)pSpell->Cost);

	if( pSpell->pCastSpell)
	{
		// Cast teaching spell on player
		pCreature->CastSpell(_player, pSpell->pCastSpell, true);
	}

	if( pSpell->pLearnSpell )
	{
		packetSMSG_PLAY_SPELL_VISUAL pck;
		pck.guid = pCreature->GetGUID();
		pck.visualid = 0x5b3;
		_player->OutPacketToSet( SMSG_PLAY_SPELL_VISUAL, sizeof(packetSMSG_PLAY_SPELL_VISUAL), &pck, true );

		pck.guid = _player->GetGUID();
		pck.visualid = 0x16a;
		_player->OutPacketToSet( 0x1F7, sizeof(packetSMSG_PLAY_SPELL_VISUAL), &pck, true );

		// add the spell
		_player->addSpell( pSpell->pLearnSpell->Id );

		uint8 i;
		for( i = 0; i < 3; ++i)
		{
			if(pSpell->pLearnSpell->Effect[i] == SPELL_EFFECT_PROFICIENCY || pSpell->pLearnSpell->Effect[i] == SPELL_EFFECT_LEARN_SPELL ||
				pSpell->pLearnSpell->Effect[i] == SPELL_EFFECT_WEAPON)
			{
				_player->CastSpell(_player, pSpell->pLearnSpell, true);
				break;
			}
		}

		for( i = 0; i < 3; ++i)
		{
			if( pSpell->pLearnSpell->Effect[i] == SPELL_EFFECT_SKILL )
			{
				uint32 skill = pSpell->pLearnSpell->EffectMiscValue[i];
				uint32 val = (pSpell->pLearnSpell->EffectBasePoints[i]+1) * 75;
				if( val > (sWorld.m_levelCap*5) ) val = (sWorld.m_levelCap*5);

				if( _player->_GetSkillLineMax(skill) > val ) continue; 

				if( skill == SKILL_RIDING )
					_player->_AddSkillLine( skill, val, val );
				else
				{
					if( _player->_HasSkillLine(skill) )
						_player->_ModifySkillMaximum(skill, val);
					else
						_player->_AddSkillLine( skill, 1, val);
				}
			}
		}
	}

	if(pSpell->DeleteSpell)
	{
		// Remove old spell.
		if( pSpell->pLearnSpell )
			_player->removeSpell(pSpell->DeleteSpell, true, true, pSpell->pLearnSpell->Id);
		else if(pSpell->pCastSpell)
			_player->removeSpell(pSpell->DeleteSpell, true, true, pSpell->pCastRealSpell->Id);
		else
			_player->removeSpell(pSpell->DeleteSpell,true,false,0);
	}
}

uint8 WorldSession::TrainerGetSpellStatus(TrainerSpell* pSpell)
{
	if(!pSpell->pCastSpell && !pSpell->pLearnSpell)
		return TRAINER_STATUS_NOT_LEARNABLE;

	if( pSpell->pCastRealSpell && (_player->HasSpell(pSpell->pCastRealSpell->Id) || _player->HasDeletedSpell(pSpell->pCastRealSpell->Id)) )
		return TRAINER_STATUS_ALREADY_HAVE;

	if( pSpell->pLearnSpell && (_player->HasSpell(pSpell->pLearnSpell->Id) || _player->HasDeletedSpell(pSpell->pLearnSpell->Id)) )
		return TRAINER_STATUS_ALREADY_HAVE;

	if(pSpell->DeleteSpell && _player->HasDeletedSpell(pSpell->DeleteSpell))
		return TRAINER_STATUS_ALREADY_HAVE;

	if(	(pSpell->RequiredLevel && _player->getLevel()<pSpell->RequiredLevel)
		|| (pSpell->RequiredSpell && !_player->HasSpell(pSpell->RequiredSpell))
		|| (pSpell->Cost && _player->GetUInt32Value(PLAYER_FIELD_COINAGE) < pSpell->Cost)
		|| (pSpell->RequiredSkillLine && _player->_GetSkillLineCurrent(pSpell->RequiredSkillLine,true) < pSpell->RequiredSkillLineValue)
		|| (pSpell->IsProfession && pSpell->RequiredSkillLine==0 && _player->GetUInt32Value(PLAYER_CHARACTER_POINTS2) == 0)//check level 1 professions if we can learn a new proffesion
		)
		return TRAINER_STATUS_NOT_LEARNABLE;
	return TRAINER_STATUS_LEARNABLE;
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_PETITION_SHOWLIST:
//////////////////////////////////////////////////////////////
void WorldSession::HandleCharterShowListOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	uint64 guid;
	recv_data >> guid;

	Creature *pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature) return;

	SendCharterRequest(pCreature);
}

void WorldSession::SendCharterRequest(Creature* pCreature)
{
	if(!_player->IsInWorld()) return;
	if(pCreature && pCreature->GetEntry()==19861 ||
		pCreature->GetEntry()==18897 || pCreature->GetEntry()==19856)
	{
		WorldPacket data(SMSG_PETITION_SHOWLIST, 81);
		static const uint8 tdata[73] = { 0x03, 0x01, 0x00, 0x00, 0x00, 0x08, 0x5C, 0x00, 0x00, 0x21, 0x3F, 0x00, 0x00, 0x00, 0x35, 0x0C, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x09, 0x5C, 0x00, 0x00, 0x21, 0x3F, 0x00, 0x00, 0x80, 0x4F, 0x12, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0A, 0x5C, 0x00, 0x00, 0x21, 0x3F, 0x00, 0x00, 0x80, 0x84, 0x1E, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00 };
		data << pCreature->GetGUID();
		data.append(tdata,73);
		SendPacket(&data);
	}
	else
	{
		WorldPacket data(29);
		data.Initialize( SMSG_PETITION_SHOWLIST );
		data << pCreature->GetGUID();
		data << uint8(1);		   // BOOL SHOW_COST = 1
		data << uint32(1);		  // unknown
		if(pCreature && pCreature->GetEntry()==19861 ||
			pCreature->GetEntry()==18897 || pCreature->GetEntry()==19856)
		{
			data << uint16(ARENA_TEAM_CHARTER_2v2);	 // ItemId of the guild charter
		}
		else
		{
			data << uint16(0x16E7);	 // ItemId of the guild charter
		}
		
		data << float(0.62890625);  // strange floating point
		data << uint16(0);		  // unknown
	//	data << uint32(0x3F21);	 // unknown

		data << uint32(1000);	   // charter prise
		data << uint32(0);		  // unknown, maybe charter type
		data << uint32(9);		  // amount of unique players needed to sign the charter
		SendPacket( &data );
	}
}

//////////////////////////////////////////////////////////////
/// This function handles MSG_AUCTION_HELLO:
//////////////////////////////////////////////////////////////
void WorldSession::HandleAuctionHelloOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	uint64 guid;
	recv_data >> guid;
	Creature* auctioneer = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!auctioneer)
		return;

	SendAuctionList(auctioneer);
}

void WorldSession::SendAuctionList(Creature* auctioneer)
{
	AuctionHouse* AH = sAuctionMgr.GetAuctionHouse(auctioneer->GetEntry());
	if(!AH)
	{
		sChatHandler.BlueSystemMessage(this, "Report to devs: Unbound auction house npc %u.", auctioneer->GetEntry());
		return;
	}

	WorldPacket data(MSG_AUCTION_HELLO, 12);
	data << uint64(auctioneer->GetGUID());
	data << uint32(AH->GetID());
	data << uint8(AH->enabled ? 1 : 0); // Alleycat - Need to correct this properly.
	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_GOSSIP_HELLO:
//////////////////////////////////////////////////////////////
void WorldSession::HandleGossipHelloOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	uint32 status=0;
	uint64 guid;
	list<QuestRelation *>::iterator it;
	std::set<uint32> ql;

	recv_data >> guid;
	Creature *qst_giver = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!qst_giver) 
		return;

	//Rev 85 par Franck > decom et temps à 2min
	if(qst_giver->GetAIInterface())
		qst_giver->GetAIInterface()->StopMovement(120000);
 
	// unstealth meh
	if( _player->IsStealth() )
		_player->RemoveAllAuraType( SPELL_AURA_MOD_STEALTH );

	// reputation
	_player->Reputation_OnTalk(qst_giver->m_factionDBC);

	//sLog.outDetail( "WORLD: Received CMSG_GOSSIP_HELLO from %u",GUID_LOPART(guid) );
	sLog.outDetail( "WORLD: CMSG_GOSSIP_HELLO de %s pour %s (id: %u)",GetPlayer()->GetName(),qst_giver->GetCreatureName()->Name,qst_giver->GetCreatureName()->Id );

	GossipScript * Script = qst_giver->GetCreatureName() ? qst_giver->GetCreatureName()->gossip_script : NULL;
	if(!Script)
		return;

	if (qst_giver->isQuestGiver() && qst_giver->HasQuests())
	{
		WorldPacket data;
		data.SetOpcode(SMSG_GOSSIP_MESSAGE);
		Script->GossipHello(qst_giver, _player, false);
		if(!_player->CurrentGossipMenu)
			return;

		_player->CurrentGossipMenu->BuildPacket(data);
		uint32 count=0;//sQuestMgr.ActiveQuestsCount(qst_giver, GetPlayer());
		size_t pos=data.wpos();
		data << uint32(count);
		for (it = qst_giver->QuestsBegin(); it != qst_giver->QuestsEnd(); ++it)
		{
			status = sQuestMgr.CalcQuestStatus(qst_giver, GetPlayer(), *it);
			if (status >= QMGR_QUEST_CHAT)
			{
				if (!ql.count((*it)->qst->id) )
				{	
					ql.insert((*it)->qst->id);
					count++;
					data << (*it)->qst->id;

					uint32 icon;

					switch(status)
					{
					case QMGR_QUEST_NOT_FINISHED:
						icon = ICON_STATUS_INCOMPLETE ;
						break;

					case QMGR_QUEST_FINISHED:
						icon = ICON_STATUS_COMPLETE;
						break;

					case QMGR_QUEST_CHAT:
						icon = QMGR_QUEST_AVAILABLE;
						break;

					default:
						icon = status;
						break;
					}

					data << uint32( icon );
					data << int32( (*it)->qst->min_level );
					data << uint32( (*it)->qst->quest_flags );
					data << uint8(0);   // 3.3.3 According to Mangos: "changes icon: blue question or yellow exclamation"
					
					LocalizedQuest * lq = (language>0) ? sLocalizationMgr.GetLocalizedQuest((*it)->qst->id,language):NULL;
					if(lq)
						data << lq->Title;
					else
						data << (*it)->qst->title;
				}
			}
		}
		data.wpos(pos);
		data << count;
		SendPacket(&data);
		sLog.outDetail( "WORLD: Sent SMSG_GOSSIP_MESSAGE" );
	}
	else
	{
		Script->GossipHello(qst_giver, _player, true);
	}
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_GOSSIP_SELECT_OPTION:
//////////////////////////////////////////////////////////////
void WorldSession::HandleGossipSelectOptionOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	//WorldPacket data;
	uint32 option;
	uint32 unk24;
	uint64 guid;
	bool Coded = false;
	uint32 BoxMoney = 0;
	std::string BoxMessage;
	//int8 extra=0;

	recv_data >> guid >> unk24 >> option;

	//sLog.outDetail("WORLD: CMSG_GOSSIP_SELECT_OPTION Option %i Guid %.8X", option, guid );
	sLog.outDetail("WORLD: CMSG_GOSSIP_SELECT_OPTION Option %i Guid %u", option, GET_LOWGUID_PART(guid) );
	
	GossipScript * Script=NULL;
	Object * qst_giver=NULL;
	uint32 guidtype = GET_TYPE_FROM_GUID(guid);

	if(guidtype==HIGHGUID_TYPE_UNIT)
	{
		Creature *crt = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
		if(!crt)
			return;

		qst_giver=crt;
		Script=crt->GetCreatureName()?crt->GetCreatureName()->gossip_script:NULL;
	}
	else if(guidtype==HIGHGUID_TYPE_ITEM)
	{
		Item * pitem = _player->GetItemInterface()->GetItemByGUID(guid);
		if(pitem==NULL)
			return;

		qst_giver=pitem;
		Script=pitem->GetProto()->gossip_script;
	}
	// Tazjin, V4613
    else if(guidtype==HIGHGUID_TYPE_GAMEOBJECT)
	{
        GameObject *gobj = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
		if(!gobj)
			return;
        
		qst_giver=gobj;
        Script=gobj->GetInfo()->gossip_script;
    } //--------------------------

	if(!Script||!qst_giver)
		return;

	uint32 IntId = 1;
	if(_player->CurrentGossipMenu)
	{
		GossipMenuItem item = _player->CurrentGossipMenu->GetItem(option);
		IntId = item.IntId;
		Coded = item.Coded;
		//extra = item.Extra;
	}

	if(Coded)
	{
		if(recv_data.rpos()!=recv_data.wpos())
			recv_data >> BoxMessage;

		Script->GossipSelectOption(qst_giver, _player, option, IntId, BoxMessage.c_str());
	}
	else
		Script->GossipSelectOption(qst_giver, _player, option, IntId, NULL);
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_SPIRIT_HEALER_ACTIVATE:
//////////////////////////////////////////////////////////////
void WorldSession::HandleSpiritHealerActivateOpcode( WorldPacket & recv_data )
{
	if(!GetPlayer()->IsInWorld() ||!GetPlayer()->isDead()) return;
	
	//25% duralbility loss
	GetPlayer()->DeathDurabilityLoss(0.25);

	//When revived by spirit healer, set health/mana at 50%
	GetPlayer()->m_resurrectHealth = _player->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/2;
	GetPlayer()->m_resurrectMana = _player->GetUInt32Value(UNIT_FIELD_MAXPOWER1)/2;

	GetPlayer()->ResurrectPlayer(NULL);

	if(GetPlayer()->getLevel() > 10)
	{
		Aura *aur = GetPlayer()->FindAura(15007);
		
		if(aur) // If the player already have the aura, just extend it.
		{
			GetPlayer()->SetAurDuration(15007,aur->GetDuration(INVOC_DURATION));
		}
		else // else add him one, that fucker, he think he will get away!?
		{
			SpellEntry *spellInfo = dbcSpell.LookupEntry( 15007 );//resurrection sickness
			SpellCastTargets targets;
			targets.m_target = GetPlayer();
			Spell*sp=new Spell(_player,spellInfo,true,NULL);
			sp->prepare(&targets);
		}
	}
	
	//When revived by spirit healer, set health/mana at 50%
	GetPlayer()->SetUInt32Value(UNIT_FIELD_HEALTH, (uint32)((float)GetPlayer()->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/2.0) );
	GetPlayer()->SetUInt32Value(UNIT_FIELD_POWER1, (uint32)((float)GetPlayer()->GetUInt32Value(UNIT_FIELD_MAXPOWER1)/2.0) );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_NPC_TEXT_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleNpcTextQueryOpcode( WorldPacket & recv_data )
{
	WorldPacket data;
	uint32 textID;
	uint64 targetGuid;
	GossipText *pGossip;

	recv_data >> textID;
	sLog.outDetail("WORLD: CMSG_NPC_TEXT_QUERY ID '%u'", textID );

	recv_data >> targetGuid;
	GetPlayer()->SetUInt64Value(UNIT_FIELD_TARGET, targetGuid);

	/* Test (Branruz), en commentaire
	Player *plr = GetPlayer();
	if(plr)
	{
		Creature *pnj = plr->GetMapMgr()->GetCreature(GET_LOWGUID_PART(targetGuid));
		if(pnj)
		{
			CreatureInfo * PnjInfo = pnj->GetCreatureName();
			pnj->GetTrainer()
		}
		else 
		{
			Log.Error("[CMSG_NPC_TEXT_QUERY]","Le pnj n'est plus selectionné");
			return;
		}
	//Script->GossipHello(qst_giver, _player, true);
	}*/

    // 5E040000  03000087150030F1
	pGossip = NpcTextStorage.LookupEntry(textID);
	LocalizedNpcText * lnc = (language>0) ? sLocalizationMgr.GetLocalizedNpcText(textID,language) : NULL;

	data.Initialize( SMSG_NPC_TEXT_UPDATE );
	data << textID;
	
	if(pGossip)
	{
		data << float(1.0f);		// Unknown
		for(uint8 i=0;i<8;i++)
		{
			if(lnc)
			{
				data << lnc->Texts[i][0];
				data << lnc->Texts[i][1];
			}
			else
			{
				data << pGossip->Texts[i].Text[0];
				data << pGossip->Texts[i].Text[1];
			}

			data << pGossip->Texts[i].Lang;
			data << uint32(0x00);		// Was prob.. but if you set it to 0 emotes work ;)
			for(uint32 e=0;e<6;e++)
				data << uint32(pGossip->Texts[i].Emote[e]);

			// if(i!=7) data << uint32(0x00);	// don't append to last
		}
	} 
	else 
	{
		data << float(1.0f);		// unknown
		data << "Bonjour, $N. Comment puis je vous aider?";
		data << "Bonjour, $N. Comment puis je vous aider?";
		data << uint32(0);	// lang
		for(uint32 e=0;e<6;e++)		// emotes
			data << uint32(0x00);

		for(int i=0;i<7;i++)
		{
			data << float(0.0f);
			data << uint8(0x00) << uint8(0x00);
			data << uint32(0x00);	// lang
			for(uint32 e=0;e<6;e++) // emotes
				data << uint32(0x00);
		}
	}

	SendPacket(&data);
	return;
}

void WorldSession::HandleBinderActivateOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	uint64 guid;
	recv_data >> guid;

	Creature *pC = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pC) return;

	SendInnkeeperBind(pC);
}


void WorldSession::SendInnkeeperBind(Creature* pCreature)
{
	#define ITEM_ID_HEARTH_STONE 6948
	#define BIND_SPELL_ID 3286

	if(!_player->IsInWorld()) return; // 

	WorldPacket data(45);

	if(!_player->bHasBindDialogOpen)
	{
        OutPacket(SMSG_GOSSIP_COMPLETE, 0, NULL);

		data.Initialize(SMSG_BINDER_CONFIRM);
		data << pCreature->GetGUID() << _player->GetZoneId();
		SendPacket(&data);

		_player->bHasBindDialogOpen = true;
		return;
	}
/*
	// Note Randdrick : Ne dois pas être géré ici, mais dans le SpellEffects.
	// Add a hearthstone if they don't have one
	if(!_player->GetItemInterface()->GetItemCount(ITEM_ID_HEARTH_STONE, true))
	{
		// We don't have a hearthstone. Add one.
		if(_player->GetItemInterface()->CalculateFreeSlots(NULL) > 0)
		{
			Item *item = objmgr.CreateItem( ITEM_ID_HEARTH_STONE, _player);
			if( _player->GetItemInterface()->AddItemToFreeSlot(item) )
			{
				SlotResult * lr = _player->GetItemInterface()->LastSearchResult();
				SendItemPushResult(item,false,true,false,true,lr->ContainerSlot,lr->Slot,1);
			}
			else
				delete item;
		}
	}

	_player->bHasBindDialogOpen = false;

	_player->SetBindPoint(_player->GetPositionX(),_player->GetPositionY(),_player->GetPositionZ(),_player->GetMapId(),_player->GetZoneId());

	data.Initialize(SMSG_BINDPOINTUPDATE);
	data << _player->GetBindPositionX() << _player->GetBindPositionY() << _player->GetBindPositionZ() << _player->GetBindMapId() << _player->GetBindZoneId();
	SendPacket( &data );

	data.Initialize(SMSG_PLAYERBOUND);
	data << pCreature->GetGUID() << _player->GetBindZoneId();
	SendPacket(&data);
*/
    OutPacket(SMSG_GOSSIP_COMPLETE, 0, NULL);

	Spell* BindSpell = (new Spell(pCreature, dbcSpell.LookupEntry( BIND_SPELL_ID ), false, NULL));
	SpellCastTargets targets;
	targets.m_target = GetPlayer(); //targets.m_unitTarget = GetPlayer()->GetGUID();
	BindSpell->prepare(&targets);



	#undef ITEM_ID_HEARTH_STONE
	#undef BIND_SPELL_ID
}



void WorldSession::SendSpiritHealerRequest(Creature* pCreature)
{
	WorldPacket data(SMSG_SPIRIT_HEALER_CONFIRM, 8);
	data << pCreature->GetGUID();
	SendPacket(&data);
}
