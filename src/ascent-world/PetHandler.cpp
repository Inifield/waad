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

// CMSG_PET_ACTION
//{CLIENT} Packet: (0x0175) CMSG_PET_ACTION PacketSize = 20
//|------------------------------------------------|----------------|
//|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
//|------------------------------------------------|----------------|
//|01 00 00 01 00 00 40 F1 02 00 00 07 05 00 00 21 |......@........!|
//|3D 00 30 F1                                     |=.0.            |
//-------------------------------------------------------------------
//|------------------------------------------------|----------------|
//|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
//|------------------------------------------------|----------------|
//|01 00 00 01 00 00 40 F1 01 00 00 07 00 00 00 00 |......@.........|
//|00 00 00 00                                     |....            |
//-------------------------------------------------------------------
void WorldSession::HandlePetAction(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;

	// Loyauté
	uint32 Loyalty_Ratio = 0;
	uint32 ResultRand    = 0;
    Unit *pOwner = NULL; 
	//WorldPacket data;
	uint64 petGuid = 0;    // F140000001000001
	uint16 misc = 0;       // 0x0001 ( de 2 à 0, Attack-Suis-Reste=Standard 
	uint16 action = 0;     // 0x0700 ( Barre d'action, celle de gauche )  (+ eventuellement 8100/C100/0100 = SpellAction, depend du spell))
	uint64 targetguid = 0; // F130003D21000005 genre ou 0x0000000000000000

	recv_data >> petGuid >> misc >> action;
	//recv_data.hexlike();

	//Log.Warning("CMSG_PET_ACTION","Size: %u, Action: %u [0x%04X], Misc: %u [0x%04X] ",recv_data.size(), action, action, misc, misc);

	//------------- Gestion du Control mental -----------------
	if(GET_TYPE_FROM_GUID(petGuid) == HIGHGUID_TYPE_UNIT)
	{
		Creature *pCharm = GetPlayer()->GetMapMgr()->GetCreature(GET_LOWGUID_PART(petGuid));

		if(!pCharm) return; // le Familier est une Unit mais non controlé (a verifier)

		if(action == PET_ACTION_ACTION) // 0x0700
		{
			recv_data >> targetguid;
			//Log.Warning("CMSG_PET_ACTION","1 - Target LGuid: %u",GET_LOWGUID_PART(targetguid));
			switch(misc)
			{
			 case PET_ACTION_ATTACK:
				{
					if(!sEventMgr.HasEvent(_player, EVENT_PLAYER_CHARM_ATTACK))
					{
						uint32 timer = pCharm->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);
						if(!timer) timer = 2000;
                        //Log.Warning("CMSG_PET_ACTION","Timer Charm Attack: %u",timer); 
						sEventMgr.AddEvent(_player, &Player::_EventCharmAttack, EVENT_PLAYER_CHARM_ATTACK, timer, 0,0);
						_player->_EventCharmAttack();
					}
				}break;
			}
		}
		return;
	}

    //------------- Gestion du familier ----------------- 
	Pet* pPet = _player->GetMapMgr()->GetPet(GET_LOWGUID_PART(petGuid)); 
	if(!pPet || !pPet->isAlive()) return; 


	/* STAND BYE en attendant le debug de la nourriture (Branruz)
    // Verif de la loyauté
	//ListenLoyRatio[7] = { 0, 15, 30, 45, 60, 75, 101 };
	uint8 Loyalty_Level = pPet->GetLoyaltyLevel();

	switch(Loyalty_Level)
	{
	 case REBELIOUS :   // 15% - Rebel
	 case UNRULY :      // 30% - Indiscipliné
	 case SUBMISIVE :   // 45% - Soumis
     case DEPENDABLE :  // 60% - Sûr             
	 case FAITHFUL :    // 75% - Fidele
	 case BEST_FRIEND : // 100% - Meilleur ami    
	                    Loyalty_Ratio = ListenLoyRatio[Loyalty_Level];
						ResultRand    = RandomUInt(100);
						if(ResultRand <= Loyalty_Ratio) break; // Le Familier ecoute le player

						pOwner = pPet->GetOwner();
						if(pOwner)
						{
						 if(pOwner->IsPlayer())
						 {
						     sChatHandler.GreenSystemMessage(((Player *)pOwner)->GetSession(), "%s ne vous ecoute pas.",
									                                                   pPet->GetName().c_str());
						 }
						}
								   
		   			    return;
	 default : Log.Error("HandlePetAction","Loyalty_Level Inconnu <---- Report this to devs.",Loyalty_Level);
		       break; // On continu le code pour l'instant
			   // return;
    }
	*/

	pPet->SetCurrentBarAISpells(NULL);   // Reset de l'AI_Spell courant
	pPet->SetCurrentBarAISpellsCount(0); // Reset du compteur de l'AI_Spell courant

	Unit *pTarget = NULL;

	if(action == PET_ACTION_SPELL || action == DEFAULT_SPELL_STATE || action == AUTOCAST_SPELL_STATE || 
	  (action == PET_ACTION_ACTION && misc == PET_ACTION_ATTACK )) // >> target
	{
		recv_data >> targetguid;
		//Log.Warning("CMSG_PET_ACTION","2 - Target LGuid: %u",GET_LOWGUID_PART(targetguid));
		pTarget = _player->GetMapMgr()->GetUnit(targetguid);

		if(!pTarget) // Spell sur lui meme.
		{
			pTarget = pPet;	// target self, à résoudre, cas du player qui n'a pas de cible. (Branruz)
			//Log.Warning("CMSG_PET_ACTION","%s (%u) se lance le spell sur lui meme",
			//	(pTarget->GetObjectName())?pTarget->GetObjectName():"Inconnu", pTarget->GetEntry());
		}
		//else Log.Warning("CMSG_PET_ACTION","Cible : %s",(pTarget->GetObjectName())?(pTarget->GetObjectName()):"Inconnu");
	}

	switch(action)
	{
     //       [   3      ]   2       1       0
	 // Misc: [ Dismiss  ] Attack - Suis - Reste
	 case PET_ACTION_ACTION: // 0x0700
	 {
	  pPet->SetPetAction(misc);	// set current action
      
      // Action time? Stand up !
	  if( misc!=PET_ACTION_STAY && pPet->GetStandState() == STANDSTATE_SIT ) 
						pPet->SetStandState(STANDSTATE_STAND);

	  switch(misc)
	  {
	    case PET_ACTION_ATTACK:
		{ 
		 // A faire: Un familier de faction ennemie peut etre attaqué
		 // dans ce cas Pet et Player passe JcJ (A Faire)

		 if(!pPet->IsPNJ()) // Pas pour un bot escorteur
		 {
		  // JdR: Ce mode peut etre lourd à suporter pour les joueurs
		  if(pPet->NotListen() && sWorld.m_JdRPetListenning) // L'ecoute du familier = fct(Loyauté)
		  {
		   WorldPacket data(SMSG_SPELL_FAILURE, 20);
		   data << _player->GetNewGUID() << uint32(0) << uint32(0) << uint8(SPELL_FAILED_TRY_AGAIN); // Echec de la tentative
		   SendPacket(&data);

		   return;
		  }
		 }
         
         // make sure the target is attackable
		 bool AttackTarget = isAttackable(pPet, pTarget);
		 if(pTarget == pPet || !AttackTarget )
		 {
		  //Log.Warning("CMSG_PET_ACTION","Echec, pTarget=Pet : %s, Attacable: %s",(pTarget == pPet)?"Oui":"Non",(AttackTarget)?"Oui":"Non");
		  WorldPacket data(SMSG_SPELL_FAILURE, 20);
		  data << _player->GetNewGUID() << uint32(0) << uint32(0) << uint8(SPELL_FAILED_BAD_TARGETS);
		  SendPacket(&data);
		  return;
		 }

		 // Clear the threat
		 pPet->GetAIInterface()->WipeTargetList();
		 pPet->GetAIInterface()->WipeHateList();

		 // Attack target with melee if the owner if we dont have spells - other wise cast. All done by AIInterface.
		 if(pPet->GetAIInterface()->getUnitToFollow() == NULL)
						pPet->GetAIInterface()->SetUnitToFollow(_player);

		 // EVENT_PET_ATTACK
		 //pPet->GetAIInterface()->SetAIState(STATE_ATTACKING); // Test, en commentaire, fait dans l'EVENT_COMBAT (Branruz)
		 pPet->GetAIInterface()->AttackReaction(pTarget, 1, 0);
		}
		break;

		case PET_ACTION_FOLLOW:
		{
		 pPet->GetAIInterface()->SetAIState(STATE_FOLLOWING); // Test (Branruz)
		 //pPet->AttackStop();
		 // Clear the threat
		 pPet->GetAIInterface()->WipeTargetList();
		 pPet->GetAIInterface()->WipeHateList();

		 // Follow the owner... run to him...
		 pPet->GetAIInterface()->SetUnitToFollow(_player);
		 pPet->GetAIInterface()->HandleEvent(EVENT_FOLLOWOWNER, pPet, 0);
		}
		break;

		case PET_ACTION_STAY:
		{
		 pPet->GetAIInterface()->SetAIState(STATE_IDLE); // Test (Branruz)
		 // Clear the threat
		 pPet->GetAIInterface()->WipeTargetList();
		 pPet->GetAIInterface()->WipeHateList();

		 // Stop following the owner, and sit.
		 pPet->GetAIInterface()->SetUnitToFollow(NULL);
		 // A FAIRE: Prendre en compte le type de créature, un oiseau ne peut pas s'asseoir ;) (Branruz)
		 // pPet->SetStandState(STANDSTATE_SIT); 
		}
		break;

		case PET_ACTION_DISMISS:
		{
		 // Bye byte...
		 pPet->Dismiss(true);
		}
		break;
	  } // End Switch(misc)
	 }
	 break; // End Action Barre (Dismiss-Attack-Suis-Reste)

    //     2         1          0 
    // Agressif - Défensif - Passif 
    case PET_ACTION_STATE: // 0x0600
	{
	 //PET_STATE_PASSIVE,   : 2
	 //PET_STATE_DEFENSIVE, : 1 
	 //PET_STATE_AGGRESSIVE : 0
	 pPet->SetPetState(misc);
	}
	break; // End Agressif - Défensif - Passif 

	// Cas d'un Spell
	case PET_ACTION_SPELL:     
	case DEFAULT_SPELL_STATE:  // PET_ACTION_SPELL_1:
	case AUTOCAST_SPELL_STATE: // PET_ACTION_SPELL
	{
	 if(!pPet->IsPNJ()) // Pas pour un Bot escorteur
	 {
      // JdR: Ce mode peut etre lourd à suporter pour les joueurs
	  if(pPet->NotListen() && sWorld.m_JdRPetListenning) // L'ecoute du familier = fct(Loyauté) - Non testé (Branruz)
	  {
	  	  WorldPacket data(SMSG_SPELL_FAILURE, 20);
		  data << _player->GetNewGUID() << uint32(0) << uint32(0) << uint8(SPELL_FAILED_TRY_AGAIN); // Echec de la tentative
		  SendPacket(&data);

		  return;
	  }
	 }
	 // misc == spellid
	 SpellEntry *entry = dbcSpell.LookupEntry(misc);
	 if(!entry) 
	 {
	  Log.Error("CMSG_PET_ACTION","Echec: PetSpell Unknown: %u <---- Check your DBCs, or PetSpell in Database",misc);
	  return;
	 }

	 AI_Spell * sp = pPet->GetAISpellForSpellId(entry->Id);
	 if(sp)
	 {
	  // Check the cooldown
	  if(sp->cooldowntime && (getMSTime() < sp->cooldowntime) )
	  {
	   //SendNotification("That spell is still cooling down.");
	   WorldPacket data(SMSG_SPELL_FAILURE, 20);
	   data << pPet->GetNewGUID();
	   data << uint8(0); //extra_cast_number
	   data << sp->spell->Id;
	   data << uint8(SPELL_FAILED_NOT_READY);
	   SendPacket(&data);
	  }
	  else
	  {
	   if(sp->spellType != STYPE_BUFF)
	   {
	    // make sure the target is attackable
		if(pTarget == pPet || !isAttackable(pPet, pTarget))
		{
		  WorldPacket data(SMSG_SPELL_FAILURE, 20);
		  data << _player->GetNewGUID() << uint32(0) << sp->spell->Id << uint8(SPELL_FAILED_BAD_TARGETS);
		  SendPacket(&data);
		  return;
		}
	   }

	   if(sp->autocast_type != AUTOCAST_EVENT_ATTACK)
	   {
	    if(sp->autocast_type == AUTOCAST_EVENT_OWNER_ATTACKED)
		  	sEventMgr.AddEvent(static_cast<Unit *>( pPet ), &Unit::EventCastSpell, static_cast<Unit *>( _player ), sp->spell, EVENT_AURA_APPLY, 250, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		else
		    sEventMgr.AddEvent(static_cast<Unit *>( _player ), &Unit::EventCastSpell, static_cast<Unit *>( pPet ), sp->spell, EVENT_AURA_APPLY, 250, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	   }
	   else
	   {
		 // Clear the threat
		 pPet->GetAIInterface()->WipeTargetList();
		 pPet->GetAIInterface()->WipeHateList();

		 /*
		 if(entry->Id != sp->spell->Id)
		 {
          Log.Warning("CMSG_PET_ACTION","Spell recue:%u , AI_SPellId:%u different <=== Verifier les AIAgents svp!",entry->Id,sp->spell->Id);
          sp->spell->Id = entry->Id; // Test (Branruz)
		 }
		 else Log.Warning("CMSG_PET_ACTION","Spell recue:%u",entry->Id);
		 */

		 pPet->SetCurrentBarAISpells(sp);
		 pPet->SetCurrentBarAISpellsCount(1); // Le spell se lance une seule fois
	
         pPet->GetAIInterface()->AttackReaction(pTarget, 1, 0); // Pas de SpellId de definie, attaque au cac ??
		 pPet->GetAIInterface()->SetNextSpell(sp);
	   }
	  }
	 }

	 /*// cast spell
	 SpellCastTargets targets;
			
	 //HACK HACK HACK
	 switch (misc)
	 {
	  case 7812:
	  case 19438:
	  case 19440:
	  case 19441:
	  case 19442:
	  case 19443:
		 targets.m_unitTarget = pPet->GetGUID(); // dono maybe it should be NULL;
		 break;
	  default:
		 targets.m_unitTarget = (pTarget ? pTarget->GetGUID() : pPet->GetGUID());
		 break;
	 }
						
	 targets.m_targetMask = TARGET_FLAG_UNIT; // 0x2; // unit

	 pPet->GetAIInterface()->CastSpell(pPet, entry, targets);*/
	} // End case PET_ACTION_SPELL:
	break;
	
	default:
		{
		 Log.Error("CMSG_PET_ACTION","Unknown pet action : Action = %04X, Misc = %04X <--- Reports this to devs.", action, misc);
		}break;
	}

	// Send pet action sound - WHEE THEY TALK 
	//Log.Warning("SMSG_PET_ACTION_SOUND","Send.");
	WorldPacket actionp(SMSG_PET_ACTION_SOUND, 12);
	actionp << pPet->GetGUID();
	actionp << uint32(1); // Pet Talk
	SendPacket(&actionp);
}
//CMSG_REQUEST_PET_INFO
void WorldSession::HandlePetInfo(WorldPacket & recv_data)
{
	//nothing
	sLog.outDebug("CMSG_REQUEST_PET_INFO: HandlePetInfo is called");
}
// CMSG_PET_NAME_QUERY
void WorldSession::HandlePetNameQuery(WorldPacket & recv_data)
{
	//Log.Warning("CMSG_PET_NAME_QUERY","Receive");
	if(!_player->IsInWorld()) return;
	uint32 petNumber = 0;
	uint64 petGuid = 0;

	recv_data >> petNumber >> petGuid;
	Pet* pPet = _player->GetMapMgr()->GetPet(GET_LOWGUID_PART(petGuid));
	if(!pPet) return;

	WorldPacket data(8 + pPet->GetName().size());
	data.SetOpcode(SMSG_PET_NAME_QUERY_RESPONSE);
	data << petNumber;
	data << pPet->GetName();
	data << pPet->GetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP);		// stops packet flood
	data << uint8(0);		// unk
	SendPacket(&data);
}
// CMSG_STABLE_PET
void WorldSession::HandleStablePet(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;

	//Log.Warning("CMSG_STABLE_PET","Receive");

	// remove pet from world and association with player
	Pet *pPet = _player->GetSummon();
	if(pPet && pPet->GetUInt32Value(UNIT_CREATED_BY_SPELL) != 0) 
		return;
	
	PlayerPet *pet = _player->GetPlayerPet(_player->GetUnstabledPetNumber());
	if(!pet) return;
	pet->stablestate = STABLE_STATE_PASSIVE;
	
	if(pPet) pPet->Remove(false, true, true);	// no safedelete needed

	//Log.Warning("SMSG_STABLE_RESULT","Send Success");

	WorldPacket data(1);
	data.SetOpcode(SMSG_STABLE_RESULT);
	data << uint8(0x8);  // success
	SendPacket(&data);
}

void WorldSession::HandleUnstablePet(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	
	uint64 npcguid = 0;
	uint32 petnumber = 0;

	//Log.Warning("CMSG_UNSTABLE_PET","Receive");

	recv_data >> npcguid >> petnumber;
	PlayerPet *pet = _player->GetPlayerPet(petnumber);
	if(!pet)
	{
		sLog.outError("PET SYSTEM: Player "I64FMT" tried to unstable non-existant pet %d", _player->GetGUID(), petnumber);
		return;
	}
	_player->SpawnPet(petnumber);
	pet->stablestate = STABLE_STATE_ACTIVE;

	WorldPacket data(1);
	data.SetOpcode(SMSG_STABLE_RESULT);
	data << uint8(STABLERESULT_UNSTABLE_SUCCESS); // success?
	SendPacket(&data);
}
void WorldSession::HandleStableSwapPet(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;

    //Log.Warning("CMSG_STABLE_SWAP_PET","Receive");

	uint64 npcguid = 0;
	uint32 petnumber = 0;
	recv_data >> npcguid >> petnumber;

	PlayerPet *pet = _player->GetPlayerPet(petnumber);
	if(!pet)
	{
		sLog.outError("PET SYSTEM: Player %s tried to unstable non-existant pet %d", _player->GetName(), petnumber);
		return;
	}
	Pet *pPet = _player->GetSummon();
	if(pPet && pPet->GetUInt32Value(UNIT_CREATED_BY_SPELL) != 0) return;

	//stable current pet
	PlayerPet *pet2 = _player->GetPlayerPet(_player->GetUnstabledPetNumber());
	if(!pet2) return;
	if(pPet) pPet->Remove(false, true, true);	// no safedelete needed
	pet2->stablestate = STABLE_STATE_PASSIVE;

	//unstable selected pet
	_player->SpawnPet(petnumber);
	pet->stablestate = STABLE_STATE_ACTIVE;

	WorldPacket data;
	data.SetOpcode(SMSG_STABLE_RESULT);
	data << uint8(STABLERESULT_UNSTABLE_SUCCESS);
	SendPacket(&data);
}

void WorldSession::HandleStabledPetList(WorldPacket & recv_data)
{
	//Log.Warning("HandleStabledPetList","Recue!");

	if(!_player->IsInWorld()) return;

	uint64 npcguid = 0;
	recv_data >> npcguid;

	SendStabledPetList(npcguid);
}

void WorldSession::SendStabledPetList(uint64 creature_guid)
{
	if(!_player->IsInWorld()) return;
	WorldPacket data(10 + (_player->m_Pets.size() * 25));
	data.SetOpcode(MSG_LIST_STABLED_PETS);

	data << creature_guid;

	data << uint8(_player->m_Pets.size());
	data << uint8(_player->m_StableSlotCount);
	uint8 i=0;
	bool HasActive = false;
	for(std::map<uint32, PlayerPet*>::iterator itr = _player->m_Pets.begin(); itr != _player->m_Pets.end(); ++itr)
	{
		data << uint32(itr->first); // pet no (LowGuid)
		data << uint32(itr->second->entry); // entryid
		data << uint32(itr->second->level); // level
		data << itr->second->name;		  // name
		if(!HasActive && itr->second->stablestate == STABLE_STATE_ACTIVE)
		{
			data << uint8(STABLE_STATE_ACTIVE);
			HasActive = true;
			continue;
		}
		data << uint8(STABLE_STATE_PASSIVE + i);
		itr->second->stablestate = STABLE_STATE_PASSIVE; //if it ain't active it must be passive ;)
		i++;
		if(i>3)//we only have 4 stable slots
			break;
	}

	SendPacket(&data);
	//Log.Warning("SendStabledPetList","Transmis!");
}

void WorldSession::HandleBuyStableSlot(WorldPacket &recv_data)
{
	if(!_player->IsInWorld() || _player->GetStableSlotCount() == MAX_STABLE_SLOTS)
		return;
	uint8 scount = _player->GetStableSlotCount();
	BankSlotPrice* bsp = dbcStableSlotPrices.LookupEntry(scount+1);
	int32 cost = (bsp != NULL) ? bsp->Price : 99999999;
	if(cost > (int32)_player->GetUInt32Value(PLAYER_FIELD_COINAGE))
	{
		WorldPacket data(1);
		data.SetOpcode(SMSG_STABLE_RESULT);
		data << uint8(STABLERESULT_FAIL_CANT_AFFORD);
		SendPacket(&data);
		return;
	}

	_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -cost);
	
	WorldPacket data(1);
	data.SetOpcode(SMSG_STABLE_RESULT);
	data << uint8(STABLERESULT_BUY_SLOT_SUCCESS);
	SendPacket(&data);
	if(_player->GetStableSlotCount() > MAX_STABLE_SLOTS)
		_player->m_StableSlotCount = MAX_STABLE_SLOTS;
	else
		_player->m_StableSlotCount++;
#ifdef OPTIMIZED_PLAYER_SAVING
	_player->save_Misc();
#endif
}

// CMSG_PET_SET_ACTION
void WorldSession::HandlePetSetActionOpcode(WorldPacket& recv_data)
{
	//Log.Warning("CMSG_PET_SET_ACTION","Recue!");

	if(!_player->IsInWorld()) return;

	uint32 unk1; // LowGuid  
	uint32 unk2; // HighGuid  
	uint32 slot;   
	uint16 spell;  
	uint16 state; // = Action du CMSG_PET_ACTION sur un spell 
	              // PET_ACTION_SPELL(0x0100) DEFAULT_SPELL_STATE(0x8100) AUTOCAST_SPELL_STATE(0xC100)
	recv_data >> unk1 >> unk2 >> slot >> spell >> state;

	//Log.Warning("CMSG_PET_SET_ACTION","Size:%u, Guid: 0x%08X%08X, slot %u, spell %u, Misc 0x%08X",
	//	             recv_data.size(),unk2,unk1,slot,spell,state);

	if(!_player->GetSummon())
	{
		Log.Warning("CMSG_PET_SET_ACTION","Le player %s n'a pas de familier!",_player->GetName()); 
		return;
	}

	Pet * pet = _player->GetSummon();
	SpellEntry * spe = dbcSpell.LookupEntryForced( spell );
	if( spe == NULL )
	{
		Log.Error("CMSG_PET_SET_ACTION","Le spell %u n'existe pas, DBC out of time!",spell); 
		return;
	}

	// do we have the spell? if not don't set it (exploit fix)
	PetSpellMap::iterator itr = pet->GetSpells()->find( spe );
	if( itr == pet->GetSpells()->end( ) )
	{
        Log.Error("CMSG_PET_SET_ACTION","Le familer ne possede pas le spell %u, SpellHack par %s",spell,_player->GetName()); 
		return;
	}

	pet->ActionBar[slot] = spell;
	pet->SetSpellState(spell, state);

	//Log.Warning("CMSG_PET_SET_ACTION","End.");
}

void WorldSession::HandlePetRename(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	uint64 guid;
	string name;
	recv_data >> guid >> name;

	if(!_player->GetSummon() || _player->GetSummon()->GetGUID() != guid)
	{
		sChatHandler.SystemMessage(this, "That pet is not your current pet, or you do not have a pet.");
		return;
	}

	Pet * pet = _player->GetSummon();
	pet->Rename(name);

	// Disable pet rename.
	pet->SetUInt32Value(UNIT_FIELD_BYTES_2, 1 | (0x2 << 16));
}

void WorldSession::HandlePetAbandon(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	Pet * pet = _player->GetSummon();
	if(!pet) return;

	pet->Dismiss(false);
}
void WorldSession::HandlePetUnlearn(WorldPacket & recv_data)
{
	if( !_player->IsInWorld() )
		return;

	uint64 guid;
	recv_data >> guid;

	Pet* pPet = _player->GetSummon();
	if( pPet == NULL || pPet->GetGUID() != guid )
	{
		sChatHandler.SystemMessage(this, "That pet is not your current pet, or you do not have a pet.");
		return;
	}

	int32 cost = pPet->GetUntrainCost();
	if( cost > ( int32 )_player->GetUInt32Value( PLAYER_FIELD_COINAGE ) )
	{
		WorldPacket data(SMSG_BUY_FAILED, 12);
		data << uint64( _player->GetGUID() );
		data << uint32( 0 );
		data << uint8( 2 );		//not enough money
		return;	
	}
	_player->ModUnsigned32Value( PLAYER_FIELD_COINAGE, -cost );
	pPet->WipeSpells();
	_player->smsg_TalentsInfo(true);
}

void WorldSession::HandlePetCancelAura(WorldPacket & recv_data)
{
	uint64 guid;
	uint32 spellId;
	recv_data >> guid >> spellId;

	if (_player->GetSummon() == NULL)
		return;

	Pet* p=_player->GetSummon();
	
	for(uint32 x = 0; x < MAX_AURAS+MAX_POSITIVE_AURAS; ++x)
	{
		if(p->m_auras[x] && p->m_auras[x]->IsPositive() && p->m_auras[x]->GetSpellId() == spellId && (!p->m_auras[x]->m_AreaAura || p->m_auras[x]->m_casterGuid == p->GetGUID()))
		{
			p->m_auras[x]->m_ignoreunapply = true; // prevent abuse
			p->m_auras[x]->Remove();
		}
	}
}
void WorldSession::HandlePetLearnTalent( WorldPacket & recvPacket )  
{  
	if( !_player->IsInWorld() )
		return;

	uint64 guid = 0;
	uint32 talentid, rank = 0;

	recvPacket >> guid >> talentid >> rank;

	// get a pointer to our current pet
	Pet * pPet = _player->GetSummon();
	if( pPet == NULL )
		return;

	// check the guid to add the talent to is the same as our pets
	// we don't want any cheating now do we
	if( pPet->GetGUID() != guid )
		return;

	// check we have talent points to spend
	uint8 talentPoints = pPet->GetByte( UNIT_FIELD_BYTES_1, 1 );
	if(!talentPoints)
		return;
	else
	{
		// deduct a point now from our stored value
		// we will set this later in case one of our checks fails
		talentPoints--;
	}

	// find our talent
	TalentEntry *talentEntry = dbcTalent.LookupEntry( talentid );
	if( talentEntry == NULL )
		return;

	PetTalentMap::iterator itr;
	// check if we require another talent first to be able to learn this one
	if( talentEntry->DependsOn )
	{
		// find the talent that we require
		// to be able to add this one
		TalentEntry *requiredTalent = dbcTalent.LookupEntryForced( talentEntry->DependsOn );
		if( requiredTalent == NULL )
			return;

		// get the rank of the talent that we require
		// to be able to add this one
		uint32 requiredRank = talentEntry->DependsOnRank;

		// do we have the required talent in our map
		itr = pPet->m_talents.find(talentEntry->DependsOn);
		if (itr != pPet->m_talents.end())
		{
			// is the rank less that the rank we require?
			if(itr->second < requiredRank)
				return; // if so abort

			// if we didn't hit the if case then we have the talent
			// do nothing and continue adding the talent
		}
		else
			return; // we don't have the talent in our map, abort!
	}

	itr = pPet->m_talents.find(talentid);
	// do we have a lower rank of this talent?
	if (itr != pPet->m_talents.end())
	{
		// Remove the lower rank spell from our pet
		pPet->RemoveSpell( talentEntry->RankID[ rank - 1 ] );
		// Replace the rank in our map with the new one, we'll add the spell later
		itr->second = rank;
	}
	else
		pPet->m_talents.insert( make_pair( talentid, rank ) );

	// find spell
	SpellEntry* sp = dbcSpell.LookupEntry( talentEntry->RankID[ rank ] );
	if( sp )
	{
		// set the new talent points, remember we deducted a point earlier ;)
		//sLog.outDebug("Pet","Setting available talent points to %u", talentPoints);
		//pPet->SetUInt32Value(UNIT_FIELD_BYTES_1, 1 | (talentPoints << 24));
		pPet->SetByte( UNIT_FIELD_BYTES_1, 1, talentPoints ); 

		// add the talent spell to our pet
		pPet->AddSpell( sp, true );

		// send the packet to the client saying we've learned it :D
		OutPacket( SMSG_PET_LEARNED_SPELL, 4, &sp->Id );

		// finally re-send our actionbar/spells to the player xD
		pPet->SendSpellsToOwner();
	}
	_player->smsg_TalentsInfo(true);
}

void WorldSession::HandleTotemDestroyed(WorldPacket & recv_data)
{
	// This code can handle destroying totem when it was right clicked in client. 
	// But now this packet for some reason is also sent when server destroys the totem which leads to a bug.
	// So leave it commented for now
	/*if( !_player->IsInWorld() )
		return;
	uint8 slot;
	recv_data >> slot;
	slot++;
	if(slot == 0 || slot > 4 || _player->m_SummonSlots[slot] == NULL)
		return;
	_player->SummonExpireSlot(slot);*/
}