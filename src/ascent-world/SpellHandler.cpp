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
// Reponse a CMSG_USE_ITEM
// Client sur Offi
// FF 18 33 6B 040000AB19144601 00 0040000000000000000000
// FF 17 39 D6 6900005CB5051601 00 0040000000000000000000
// FF 17 47 D6 6900005CB5051601 00 0040000000000000000000
// Client sur Waad
// tmp1 slot tmp3 cn   guid            GIndex unk1 unk2 m_targetMask m_targetMaskExtended
// FF    17   8B  B2 010000D9 10000000 00     00   4000 00000000     00000000
// FF    17   8C  B2 010000D9 10000000 00     00   4000 00000000     00000000 
// FF    1E   8D  7F 2E000010 27000000 00     00   4000 00000000     00000000
// 13    04   95  B2 01000049 2B000000 00     00   4000 00000000     00000000 // 19 4 -107 178 ... 4194304(0x00004000)
void WorldSession::HandleUseItemOpcode(WorldPacket& recvPacket)
{
	if(!_player->IsInWorld()) return;
	typedef std::list<Aura*> AuraList;
	
	Player* p_User = GetPlayer();
	sLog.outDetail("WORLD: got use Item packet, data length = %i",recvPacket.size());
	int8 tmp1,slot,tmp3;
	uint32 item_guidL;
	uint32 item_guidH;
	uint8 cn;
	uint32 spellId = 0;
	//uint32 SpellCategorie = 0;
	//uint32 ItemSpellCategorie = 0;
	
	uint8 glyphIndex; 
	uint8 unk1;	
	uint16 unk2; 

	recvPacket >> tmp1 >> slot >> tmp3 >> cn >> item_guidL >> item_guidH >> glyphIndex >> unk1 >> unk2;
    
	sLog.outDetail("WORLD: tmp1:0x%02X (%d) slot:%d tmp3:0x%02X (%d) cn:%u item_guid 0x%08X %08X",
		                         tmp1,tmp1,slot,tmp3,tmp3,cn,item_guidL,item_guidH);

	Item* tmpItem = NULL;
	tmpItem = p_User->GetItemInterface()->GetInventoryItem(tmp1,slot);

	if (!tmpItem) tmpItem = p_User->GetItemInterface()->GetInventoryItem(slot);

	if (!tmpItem) 
	{
		Log.Error("HandleUseItemOpcode","%s veux utiliser un item qu'il ne possede pas!",p_User->GetName());
		return;
	}

	ItemPrototype *itemProto = tmpItem->GetProto();

	if(!itemProto) 
	{
        Log.Error("HandleUseItemOpcode","%s veux utiliser un item qui n'existe pas dans la base!",p_User->GetName());
		return;
	}

	if(p_User->getDeathState()==CORPSE) return;


	if(itemProto->Bonding == ITEM_BIND_ON_USE) tmpItem->SoulBind();

	if(sScriptMgr.CallScriptedItem(tmpItem,_player)) return; // Si script sur l'item on exec et on sort

	// Utilisation: Bonne classe ?
	if( (itemProto->AllowableClass && !(p_User->getClassMask() & itemProto->AllowableClass)) || 
		(itemProto->AllowableRace && !(p_User->getRaceMask() & itemProto->AllowableRace)) )
	{
		p_User->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULL,INV_ERR_YOU_CAN_NEVER_USE_THAT_ITEM);
		return;
	}

	// Utilisation: Bon level ?
	if(itemProto->RequiredLevel)
	{
		if(p_User->getLevel() < itemProto->RequiredLevel)
		{
			p_User->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULL,INV_ERR_ITEM_RANK_NOT_ENOUGH);
			return;
		}
	}
    // Utilisation: Bonne competence ?
	if(itemProto->RequiredSkill)
	{
		if(!p_User->_HasSkillLine(itemProto->RequiredSkill))
		{
			p_User->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULL,INV_ERR_ITEM_RANK_NOT_ENOUGH);
			return;
		}
        // Utilisation: xp suffisant dans la competence ?
		if(itemProto->RequiredSkillRank)
		{
			if(p_User->_GetSkillLineCurrent(itemProto->RequiredSkill, false) < itemProto->RequiredSkillRank)
			{
				p_User->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULL,INV_ERR_ITEM_RANK_NOT_ENOUGH);
				return;
			}
		}
	}
	
    if(itemProto->QuestId) // Item de quest ?
	{
		// Item Starter
		Quest *qst = QuestStorage.LookupEntry(itemProto->QuestId);
		// Player fait la quete ?
		if(!qst) return;

		// Level pour la quete
		if( (sQuestMgr.PlayerMeetsReqs(_player, qst, false) != QMGR_QUEST_AVAILABLE) || (qst->min_level > _player->getLevel()) )
			return;

        WorldPacket data;
        sQuestMgr.BuildQuestDetails(&data, qst, tmpItem, 0, language,p_User);
		SendPacket(&data);
	}

	//Log.Warning("HandleUseItemOpcode","Recherche spell sur l'item");
    //********** Traitement si l'item possede un Spell **************
	if( !itemProto->Spells[0].Id && !itemProto->Spells[1].Id && 
		!itemProto->Spells[2].Id && !itemProto->Spells[3].Id && !itemProto->Spells[4].Id ) return;

	// Maj Caster = player
	SpellCastTargets targets(recvPacket, p_User);

	uint32 spellIndex;
	for(spellIndex = 0; spellIndex < 5; spellIndex++)
	{
		if(itemProto->Spells[spellIndex].Trigger == USE)
		{
			if(itemProto->Spells[spellIndex].Id)
			{
				spellId = itemProto->Spells[spellIndex].Id;
				//SpellCategorie = itemProto->Spells[spellIndex].Category;
				break;
			}
		}
	}
    if(!spellId) 
	{
		Log.Warning("HandleUseItemOpcode","Pas de Spell avec Trigger");
		return;
	}
	
	SpellEntry *spellInfo = dbcSpell.LookupEntryForced( spellId );

    if ( !spellInfo)
	{
		Log.Error("HandleUseItemOpcode","Unknown spell id %i\n", spellId);
		return;
	}

	//if(sHookInterface.OnCastSpell( _player, spellInfo )) return; // A Deliberer plus tard.....(Branruz)
    sHookInterface.OnCastSpell( p_User, spellInfo );

	// Redefinition possible de la cible via les implicitTargets
    // Attention: A ce stade le m_target = Player par defaut, il s'agit ici d'un override eventuel (Brz)
	uint8 _result = p_User->ConfirmPlayerTarget(&targets,p_User,spellInfo->Id,spellInfo->EffectImplicitTargetA[0]);
	if(_result != SPELL_CANCAST_OK)
	{
 	 p_User->SendCastResult(spellInfo->Id, _result, cn, 0); // A Finir: Parametre eventuel du spellFailure à definir (Branruz)
	 return;
	}

	// Le player s'assoie (genre l'item est de la nourriture (Randdrick) )
    // A voir pour le combat sur griffon (Branruz)
	if (spellInfo->auraInterruptFlags & AURA_INTERRUPT_ON_STAND_UP)
	{

		if (!p_User) return; //franck r144 anticrash pierre de foyer

		if (p_User->CombatStatus.IsInCombat() || p_User->IsMounted())
		{
			p_User->GetItemInterface()->BuildInventoryChangeError(tmpItem,NULL,INV_ERR_CANT_DO_IN_COMBAT);
			return;
		}
		if(p_User->GetStandState()!= STANDSTATE_SIT) p_User->SetStandState(STANDSTATE_SIT);
		// loop through the auras and removing existing eating spells
	}

	if( !p_User->Cooldown_CanCast( itemProto, spellIndex ) )
	{
		p_User->SendCastResult(spellInfo->Id, SPELL_FAILED_NOT_READY, cn, 0);
		return;
	}

	if(p_User->m_currentSpell)
	{
		p_User->SendCastResult(spellInfo->Id, SPELL_FAILED_SPELL_IN_PROGRESS, cn, 0);
		return;
	}

	if( itemProto->ForcedPetId >= 0 )
	{
		if( itemProto->ForcedPetId == 0 )
		{
			if( p_User != targets.m_target )
			{
				p_User->SendCastResult(spellInfo->Id, SPELL_FAILED_BAD_TARGETS, cn, 0);
				return;
			}
		}
		else
		{
			if( !p_User->GetSummon() || (p_User->GetSummon()->GetEntry() != (uint32)itemProto->ForcedPetId) )
			{
				p_User->SendCastResult(spellInfo->Id, SPELL_FAILED_SPELL_IN_PROGRESS, cn, 0);
				return;
			}
		}
	}

	Spell * spell = new Spell(p_User, spellInfo, false, NULL);
	
	if (!spell) return; //franck r144 anticrash pierre de foyer
	
	spell->extra_cast_number=cn;
	spell->m_glyphIndex = glyphIndex;
	
	/*Note Randdrick : il faut pouvoir faire la différence entre le owner d'un spell et son caster
	sachant que si le owner (déclencheur du spell) peut être différent du caster (source du spell), 
	le caster peut très bien être le même que le owner.
	C'est d'ailleurs le cas pour les Items qui n'ont pas de CLASSE définies. Ils sont à la fois
	caster et owner. Dans ce cas, il suffit juste de définir le caster */
	switch(itemProto->Class)
	{
		case ITEM_CLASS_MISCELLANEOUS : // 15
			switch (itemProto->SubClass)
			{
				case ITEM_SUBCLASS_MISCELLANEOUS_MOUNT:
					spell->m_caster = tmpItem;					                                      
				if(spell->m_caster) Log.Warning("[HandleCastSpellOpcode]","(%s) Spell %u, Def m_caster %u",
											 GetPlayer()->GetName(),spellInfo->Id,spell->m_caster->GetEntry());
				else Log.Warning("[HandleCastSpellOpcode]","(%s) Spell %u",GetPlayer()->GetName(),spellInfo->Id);
					break;
				default:
					spell->m_owner = tmpItem;
					break;
			}
			break;
		case ITEM_NO_CLASS_USE		: // -1
        case ITEM_CLASS_CONSUMABLE	: // 0
		case ITEM_CLASS_CONTAINER	: // 1	 
		case ITEM_CLASS_WEAPON		: // 2
		case ITEM_CLASS_JEWELRY		: // 3
		case ITEM_CLASS_ARMOR		: // 4
		case ITEM_CLASS_REAGENT		: // 5
        case ITEM_CLASS_PROJECTILE	: // 6	
		case ITEM_CLASS_TRADEGOODS	: // 7
		case ITEM_CLASS_GENERIC		: // 8
		case ITEM_CLASS_RECIPE		: // 9
		case ITEM_CLASS_MONEY		: // 10
		case ITEM_CLASS_QUIVER		: // 11
		case ITEM_CLASS_QUEST		: // 12
		case ITEM_CLASS_KEY			: // 13
		case ITEM_CLASS_PERMANENT	: // 14
		case ITEM_CLASS_GLYPHS	    : // 16	
			spell->m_owner = tmpItem;
            break;

		default : 
			Log.Warning("[HandleCastSpellOpcode]","(%s) Spell %u, Classe Item inconnue %u",
				                            GetPlayer()->GetName(),spellInfo->Id,itemProto->Class);
			break;
	}
	// Le LockMaterial de l'Item donne se qui est deverouillé 
	//spell->g_caster = NULL;
	if(itemProto->LockMaterial == LOCKTYPE_DISARM_TRAP) // Il faut un g_caster (Dé-armement de piège)
	{
		Log.Warning("HandleUseItemOpcode","LOCKTYPE_DISARM_TRAP");
		std::set<Object*>::iterator Itr = p_User->GetInRangeSetBegin();
		for( ; Itr != p_User->GetInRangeSetEnd(); Itr++ )
		{
			if( (*Itr)->GetTypeId() == TYPEID_GAMEOBJECT ) // Recherche du Go le plus pres du player
			{ 
				uint32 ItemLockId = ((GameObject*)(*Itr))->GetInfo()->raw.sound1; //button.lockId, Recherche de la bonne clef
				if(ItemLockId)
				{
					LockEntry *pLock = dbcLock.LookupEntry(ItemLockId); // Recherce de l'Id du dévérouilleur (une clef par exemple) dans le LockId du Go
					if(pLock) 
					{
						// On delock avec un Item (la, forcement en faite, sinon on serait pas dans cette fonction!!)
						//if( pLock->locktype[0] == 1 )
						//{ // Dans le lockmics0 de lock.dbc du LockId du Go, on trouve l'ID de la clef
						if( itemProto->ItemId == pLock->lockmisc[0]) 
						{
							spell->m_caster = (*Itr); // (GameObject*)(*Itr); // Go trouvé
							//spell->_SetTargets(spell->g_caster->GetGUID()); A FINIR, appeler seulement le NewHandleEffects si fonctionnel
							break;
						}
					}
					else Log.Error("HandleUseItemOpcode","Erreur du fichier Lock.dbc, Verifier la version des DBCs svp!");
				}
			}
		}
	} // End if

    //if(spell->g_caster != NULL)	Log.Warning("HandleUseItemOpcode","Go : %u", spell->g_caster->GetEntry());
	//else                        Log.Warning("HandleUseItemOpcode","Go introuvable");
	
	if( spell->prepare(&targets) == SPELL_CANCAST_OK )
	{
		p_User->Cooldown_AddItem( itemProto, spellIndex );
		//Log.Warning("HandleUseItemOpcode","SPELL_CANCAST_OK: %u\n",spellInfo->Id);

		// Lancement du spell Effect de l'item sur la cible
		for(uint32 i=0;i < 3;i++)
		{
			// Validation et recheche de la cible en fonction du spellEffect (Brz)
			if(spellInfo->EffectImplicitTargetA[i])
			{
				uint8 _result = p_User->ConfirmPlayerTarget(&targets,p_User,spellInfo->Id,spellInfo->EffectImplicitTargetA[i]);
				if(_result != SPELL_CANCAST_OK)
				{
 					p_User->SendCastResult(spellInfo->Id, _result, cn, 0); // A Finir: Parametre eventuel du spellFailure à definir (Branruz)
					return; // SpellFailure, on sort
				}
			}
            
			if(spellInfo->EffectImplicitTargetB[i])
			{
				uint8 _result = p_User->ConfirmPlayerTarget(&targets,p_User,spellInfo->Id,spellInfo->EffectImplicitTargetB[i]);
				if(_result != SPELL_CANCAST_OK)
				{
 					p_User->SendCastResult(spellInfo->Id, _result, cn, 0); // A Finir: Parametre eventuel du spellFailure à definir (Branruz)
					return; // SpellFailure, on sort
				} 
			} 

			if(spellInfo->Effect[i]) 
			{
				if(spell->m_caster)
				{
					if(spell->m_caster->IsGO()) 
					{
						spell->HandleEffects(spell->m_caster,i,false);
						return; 
					} 
				}
				/*else
				{
					spell->m_caster = spell->m_owner; // m_caster est l'item lui-meme (ou doit forcement etre le player ?) (Brz)
				    spell->HandleEffects(spell->m_caster,i,false);
					return;
				}*/
				
			}
		}
/*
	 // Event Nourriture ou boisson (NE PAS TOUCHER AU COMMENTAIRE - Branruz)
     if(itemProto->Class==ITEM_CLASS_CONSUMABLE)
	 {	
		SpellDuration *spDuration = dbcSpellDuration.LookupEntryForced(spellInfo->durationIndex);
        if(!spDuration) 
		{
		 Log.Warning("Use Item","Duration de regen non trouvee (Spell %u)",spellInfo->Id);
		 return;
		}
        spNbTick = spDuration->Duration3;
	    if(spNbTick<1000) spNbTick = 1; 
        else              spNbTick = (uint32)((float)spNbTick / 2000.0); // en ms et ttes les 2secs

		switch(spellInfo->powerType)
		{
			case POWER_TYPE_MANA        :
    		case POWER_TYPE_RAGE        :
    		case POWER_TYPE_FOCUS       :
    		case POWER_TYPE_ENERGY      :
    		case POWER_TYPE_HAPPINESS  :
    		case POWER_TYPE_RUNES       :
    		case POWER_TYPE_RUNIC_POWER : break;
			default : Log.Warning("Table 'Item'","SpellCategorie n'existe pas pour les items consomables! (%u, Type: %d)",
						  spellInfo->Id,spellInfo->powerType);
					  return;     
	    }

		int32 ItemSpellCategorie = spellInfo->powerType;

        SpellParse *spParse = new SpellParse;
		switch(itemProto->SubClass)
		{

		 case ITEM_SUBCLASS_CONSUMABLE              : // 0
	     case ITEM_SUBCLASS_CONSUMABLE_FOOD         : // 5
			 // A ce stade il faut parser la formule de la colonne "Description"
			 if(spellInfo->EffectBasePointCalculate[0]) // Deja calculé ?
			 {
              spBasePointPerTick = spellInfo->EffectBasePointCalculate[0]; // Sur Index 0 en attendant
			 }
			 else
			 {
			  spParse->EvaluateFormule(spellInfo,spellInfo->Description,&spBasePointPerTick,1); // 1: premiere formule trouvée
			  // Memo, Data static, le premier du seveur à l'utiliser le calcul pour les autres ;)
			  spellInfo->EffectBasePointCalculate[0] = spBasePointPerTick; 
			 }

			 spBasePointPerTick = (int32)((float)spBasePointPerTick / (float)(spNbTick*2));

             Log.Warning("Regen","Regen Food de %u",spBasePointPerTick*spNbTick*2);

			 sEventMgr.RemoveEvents(_player,EVENT_FOOD_REGEN);
			 sEventMgr.AddEvent((Player *)targets.m_target, &Player::EventFoodRegen,spellInfo,spBasePointPerTick,  
				            EVENT_FOOD_REGEN,2000, spNbTick, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			 break;

		 case ITEM_SUBCLASS_CONSUMABLE_POTION       : // 1
		 case ITEM_SUBCLASS_CONSUMABLE_ELIXIR       : // 2
             // A ce stade il faut parser la formule de la colonne "Description"
			 if(spellInfo->EffectBasePointCalculate[0]) // Deja calculé ?
			 {
              spBasePointPerTick = spellInfo->EffectBasePointCalculate[0]; // Sur Index 0 en attendant
			 }
			 else
			 {
			  spParse->EvaluateFormule(spellInfo,spellInfo->Description,&spBasePointPerTick,1); // 1: premiere formule trouvée
			  // Memo, Data static, le premier du seveur à l'utiliser le calcul pour les autres ;)
			  spellInfo->EffectBasePointCalculate[0] = spBasePointPerTick; 
			 }
			 
			 spBasePointPerTick = (int32)((float)spBasePointPerTick / (float)(spNbTick*2));

             Log.Warning("Regen","Regen Power de %u",spBasePointPerTick*spNbTick*2);

			 sEventMgr.RemoveEvents(_player,EVENT_DRUNK_REGEN);
			 sEventMgr.AddEvent((Player *)targets.m_target, &Player::EventDrunkRegen,spellInfo,ItemSpellCategorie,spBasePointPerTick,  
				 EVENT_DRUNK_REGEN,2000, spNbTick, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			 
			 break;	                      
		 
		 case ITEM_SUBCLASS_CONSUMABLE_FLASK	    : // 3
	                                                 break;
		 case ITEM_SUBCLASS_CONSUMABLE_SCROLL       : // 4
	                                                 break;
		 case ITEM_SUBCLASS_CONSUMABLE_ITEM_ENCHANT : // 6
	                                                 break;
		 case ITEM_SUBCLASS_CONSUMABLE_BANDAGE      : // 7
	                                                 break;
		 case ITEM_SUBCLASS_CONSUMABLE_OTHER	    : // 8
			 	                                     break;
		 default : Log.Warning("Use Item","%s utilise un consomable non reference, verifier la table 'item'!",_player->GetName());
			       Log.Warning("        ","Item : %u (%s)",itemProto->ItemId,itemProto->Name1);
				   break;
		}
		delete spParse;
	 }
	 */
	} // End if( spell->prepare(&targets) == SPELL_CANCAST_OK)
}

//{CLIENT} Packet: (0x012E) CMSG_CAST_SPELL PacketSize = 10
//|------------------------------------------------|----------------|
//|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
//|------------------------------------------------|----------------|
//|07 73 03 00 00 00 00 00 00 00                   |.s........      |
//-------------------------------------------------------------------
//{CLIENT} Packet: (0x012E) CMSG_CAST_SPELL PacketSize = 16
//|------------------------------------------------|----------------|
//|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
//|------------------------------------------------|----------------|
//|01 EB 05 00 00 00 02 00 00 00 D9 08 67 04 30 F1 |............g.0.|
//-------------------------------------------------------------------
//|27 9D 0B 00 00 00 02 00 00 00 D9 11 AA 3B 30 F1 |'............;0.|
// Reponse a CMSG_CAST_SPELL, un player lance un spell (335.12340 Check - Branruz)
void WorldSession::HandleCastSpellOpcode(WorldPacket& recvPacket)
{
	//Log.Notice("[HandleCastSpellOpcode]","CMSG_CAST_SPELL Recue");
	sLog.outDetail("HandleCastSpellOpcode: CMSG_CAST_SPELL Recue");
    Player *plr =NULL;
	Item *item_used = NULL;
	                                 // Packet Size 10              Packet Size 16               Packet Size 17
	uint8 cn;                        // 07                          01                           - 
	uint32 spellId;                  // 73 03 00 00                 EB 05 00 00                  4E 19 00 00
	uint8 unk;                       // 00                          00                           ??
/* Passage sur "void SpellCastTargets::read( WorldPacket & data, Object* caster )" appel via "SpellCastTargets targets(recvPacket, GetPlayer());"
	uint16 m_targetMask=0;           // 00 00  (TARGET_FLAG_SELF)   02 00 (TARGET_FLAG_UNIT)     00 08 (TARGET_FLAG_OBJECT)
	uint16 m_targetMaskExtended=0;   // 00 00                       00 00                        ?? 
	                                                                D9 08 67 04 30 F1 (WoWguid)  ?? (WowGuid)
																	
																	SpellTick : 27 (Crash le core: Morsure de serpent)
																	spellId   : 0x000B9D // 9D 0B 00 00
                                                                    unk       : 00 
                                                                    m_targetMask : 02 00 (TARGET_FLAG_UNIT)
																	m_targetMaskExtended: 00 00
																	WoWLowGuid(zip) : 0xF1303BAA11D9 // D9 11 AA 3B 30 F1
*/
	plr = GetPlayer();
	if(!plr)
	{
		Log.Error("CMSG_CAST_SPELL","FATAL: Player NULL <------ Report this to devs.");
		return;
	}

	if(!plr->IsInWorld()) return;
	if(plr->getDeathState()==CORPSE) return;

    // Andy
	//if unk & 2, its a missile

	recvPacket >> cn >> spellId >> unk;
	// check for spell id
	sLog.outDetail("HandleCastSpellOpcode: CMSG_CAST_SPELL, TickSpell:%u spellId:%u unk:%u (0x%02X)",cn ,spellId,unk,unk);

	SpellEntry *spellInfo = dbcSpell.LookupEntryForced(spellId );

	if(!spellInfo) // Le player lance un spell qui n'existe pas....
	{
		Log.Error("CMSG_CAST_SPELL","%s : Spell inconnu %d => Version spell.dbc <> Version Client <=",plr->GetName(), spellId);
		plr->SendCastResult(spellId, SPELL_FAILED_BAD_TARGETS, cn, 0); // Liberation client
		return;
	}

	if (spellInfo->attributes & ATTRIBUTES_PASSIVE)
	{
		// Pour Verif
		Log.Warning("CMSG_CAST_SPELL","%s tried to cast a passive spell (spell id %u)",plr->GetName(), spellInfo->Id);
		plr->SendCastResult(spellInfo->Id,SPELL_FAILED_SUCCESS, cn, 0);
		return;
	}

	// Debug (Branruz)
	//Log.Notice("[HandleCastSpellOpcode]","(%s) Spell Recu, SpellId - %i (%s)",this->GetPlayer()->GetName(),spellId, spellInfo->Name);
	
	// Cheat Detection only if player and not from an item
	// this could fuck up things but meh it's needed ALOT of the newbs are using WPE now
	// WPE allows them to mod the outgoing packet and basicly choose what ever spell they want :(
    // A ce stade, autant laisser le client coincé.....gniark....
	if( !plr->HasSpell(spellId) )
	{
		Log.Error("CMSG_CAST_SPELL","%s lance le Spell %d qu'il ne possede pas, cheater!",plr->GetName(), spellId);
		return;
	}

	// Si le spellScript existe, on l'execute et on ressort, sinon on passe a la gestion standard
    // if(sHookInterface.OnCastSpell(plr, spellInfo)) return; // A discuter (Branruz)

    sHookInterface.OnCastSpell(plr, spellInfo);

	//if(spellInfo->attributesEx & ATTRIBUTESEX_AREA_OF_EFFECT)
	// EffectImplicitTargetA[0] == EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT
	// EffectImplicitTargetA[1] == EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED

	// Verif si Spell pour apprivoiser alors que le player à déja son familier avec lui
	if((spellInfo->attributesExB & ATTRIBUTESEXB_TAME_X) )// && () )
	{
	  if(plr->GetPlayerPet(plr->GetUnstabledPetNumber() )) // Pas besoin que le familier soit invoqué
	  { // Ex: Invoc le familier = 883 (Dummy = 455)
		// Log.Warning("CMSG_CAST_SPELL","%s est avec son familier",plr->GetName());
		plr->SendCastResult(spellInfo->Id,SPELL_FAILED_ALREADY_HAVE_SUMMON, cn, 0);
		return;
	  }
	}
	//-----------------
	if (plr->GetOnMeleeSpell() != spellId)
	{
		// Debug (Branruz)
       //Log.Warning("[HandleCastSpellOpcode]","(%s) Spell Distant",plr->GetName());

		//autoshot 75
		if((spellInfo->attributesExB & ATTRIBUTESEXB_ACTIVATE_AUTO_SHOT) /*spellInfo->attributes == 327698*/)	// auto shot..
		{
			// Debug (Branruz)
			//Log.Notice("[HandleCastSpellOpcode]","(%s) Spell Auto Shot",this->GetPlayer()->GetName());
			item_used = plr->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
			if(!item_used) 
			{
				// SPELL_FAILED_EQUIPPED_ITEM_CLASS            = 29, // "Doit avoir équiper (%s)"
				// SPELL_FAILED_NEED_AMMO_POUCH                = 53, // "Requiert (%s)"
				plr->SendCastResult(spellInfo->Id,SPELL_FAILED_SUCCESS, cn, 0);
				return;
			}
            // Pourquoi faire un override du spell de tir... (Branruz) ?
			// On fait un override pour l'autoshot car le spell 75
			// n'est pas capable de connaitre l'arme à distance utilisée. (Randdrick)
			switch(item_used->GetProto()->SubClass)
			{
			case ITEM_SUBCLASS_WEAPON_BOW:
			case ITEM_SUBCLASS_WEAPON_GUN:
			case ITEM_SUBCLASS_WEAPON_CROSSBOW:
				Log.Warning("[HandleCastSpellOpcode]","(%s) Spell %u, subclass d'Arme de jet %u",
				                            this->GetPlayer()->GetName(),spellInfo->Id,item_used->GetProto()->SubClass);
				spellId = SPELL_RANGED_GENERAL;
				break;
			 case ITEM_SUBCLASS_WEAPON_THROWN:			// thrown (Arme de Jet)
				spellId = SPELL_RANGED_THROW;
				break;
			 case ITEM_SUBCLASS_WEAPON_WAND:			// wands
				spellId = SPELL_RANGED_WAND;
				break;
			 default:
				spellId = 0;
				break;
			}
		   
			if(!spellId) 
			{
				// Debug (Branruz)
				Log.Error("[HandleCastSpellOpcode]","(%s) Spell %u, subclass d'Arme de jet inconnu %u",
				                            this->GetPlayer()->GetName(),spellInfo->Id,item_used->GetProto()->SubClass);
				spellId = spellInfo->Id;
			}

			if(!plr->m_onAutoShot)
			{
				plr->m_AutoShotTarget = plr->GetSelection();
				uint32 duration = plr->GetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME);
				SpellCastTargets targets(recvPacket, plr);
				if(!targets.m_target)
				{
					//sLog.outString( "Cancelling auto-shot cast because targets.m_target is null!" );
					plr->SendCastResult(spellInfo->Id,SPELL_FAILED_LINE_OF_SIGHT, cn, 0); // "Cible hors du champ de vision"
					return;
				}
				SpellEntry *sp = dbcSpell.LookupEntry(spellId);
				Spell *spell = new Spell(plr, spellInfo, false, NULL);
			
				plr->m_AutoShotSpell = sp;
				plr->m_AutoShotDuration = duration;
				//This will fix fast clicks
				if(plr->m_AutoShotAttackTimer < 500) plr->m_AutoShotAttackTimer = 500;
				plr->m_onAutoShot = true;


			}

			return;
		} // -----------------------------

		/*const char * name = sSpellStore.LookupString(spellInfo->Name);
		if(name)
			sChatHandler.SystemMessageToPlr(_player, "%sSpell Cast:%s %s %s[Group %u, family %u]", MSG_COLOR_LIGHTBLUE,
			MSG_COLOR_SUBWHITE, name, MSG_COLOR_YELLOW, spellInfo->SpellGroupType, spellInfo->SpellFamilyName);*/

        if(plr->m_currentSpell)
        {
			if( plr->m_currentSpell->getState() == SPELL_STATE_CASTING ) // Le spell est deja lancé, canceling...
			{
				plr->m_currentSpell->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
			}
			else
			{   // "Une autre action est en cours"
				Log.Warning("CMSG_CAST_SPELL","Player a deja lancer ce spell: %u",plr->m_currentSpell->GetSpellProto()->Id);
				plr->SendCastResult(spellInfo->Id, SPELL_FAILED_SPELL_IN_PROGRESS, cn, 0);
				return;
			}
        }

		/* Test/Debug (Branruz)
        plr->SendCastResult(spellInfo->Id, SPELL_FAILED_BAD_TARGETS, cn, 0); // "Cet objet n'est pas une cible autorisée"
		return;
		*/

		
		SpellCastTargets targets(recvPacket, plr); // Définition de la cible
		Spell *spell = new Spell(plr, spellInfo, false, NULL);

		// Def du spell->m_owner si le spell utilise un Item Stackable (Brz)
		// Le truc est de savoir comment retrouver l'item
		item_used = plr->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
		if(item_used) // sécurité
		{
			switch(item_used->GetProto()->Class)
			{
			case ITEM_CLASS_PROJECTILE	: // 6		 
			case ITEM_CLASS_WEAPON		: // 2 
				spell->m_owner = item_used; // meme NULL c'est Ok (Brz)
				if(spell->m_owner)
					Log.Warning("[HandleCastSpellOpcode]","(%s) Spell %u, Def m_owner %u",GetPlayer()->GetName(),spellInfo->Id,spell->m_owner->GetEntry());
				else 
					Log.Warning("[HandleCastSpellOpcode]","(%s) Spell %u",GetPlayer()->GetName(),spellInfo->Id);
				break;         
			case ITEM_CLASS_CONSUMABLE : // 0
			case ITEM_CLASS_CONTAINER	: // 1
			case ITEM_CLASS_JEWELRY	: // 3
			case ITEM_CLASS_ARMOR		: // 4
			case ITEM_CLASS_REAGENT	: // 5
			case ITEM_CLASS_TRADEGOODS	: // 7
			case ITEM_CLASS_GENERIC	: // 8
			case ITEM_CLASS_RECIPE		: // 9
			case ITEM_CLASS_MONEY		: // 10
			case ITEM_CLASS_QUIVER		: // 11
			case ITEM_CLASS_QUEST		: // 12
			case ITEM_CLASS_KEY		: // 13
			case ITEM_CLASS_PERMANENT	: // 14
			case ITEM_CLASS_MISCELLANEOUS : // 15
			case ITEM_CLASS_GLYPHS	    : // 16
			case ITEM_NO_CLASS_USE     : // -1
			                          break;
			default : Log.Warning("[HandleCastSpellOpcode]","(%s) Spell %u, Classe d'Item inconnue %u",
				                            GetPlayer()->GetName(),spellInfo->Id,item_used->GetProto()->Class);
			                         break;
			}
		}
		else
			spell->m_owner = item_used; // meme NULL c'est Ok (Brz)

		spell->extra_cast_number=cn;

		//spell->GenerateTargets(&targets); // Définition de la cible, affutage (Test, Branruz)
		spell->prepare(&targets);
	}
}

void WorldSession::HandleCancelCastOpcode(WorldPacket& recvPacket)
{
	uint32 spellId;
	recvPacket >> spellId;

	if(GetPlayer()->m_currentSpell)
		GetPlayer()->m_currentSpell->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
}

void WorldSession::HandleCancelAuraOpcode( WorldPacket& recvPacket)
{
	uint32 spellId;
	recvPacket >> spellId;
	
	for(uint32 x = 0; x < MAX_AURAS+MAX_POSITIVE_AURAS; ++x)
	{
		if(_player->m_auras[x] && _player->m_auras[x]->IsPositive() && _player->m_auras[x]->GetSpellId() == spellId && (!_player->m_auras[x]->m_AreaAura || _player->m_auras[x]->m_casterGuid == _player->GetGUID()))
		{
			_player->m_auras[x]->m_ignoreunapply = true; // prevent abuse
			_player->m_auras[x]->Remove();
		}
	}
}

void WorldSession::HandleCancelChannellingOpcode( WorldPacket& recvPacket)
{
	uint32 spellId;
	recvPacket >> spellId;

	Player *plyr = GetPlayer();
	if(!plyr)
		return;
	if(plyr->m_currentSpell)
	{		
		plyr->m_currentSpell->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
	}
}

void WorldSession::HandleCancelAutoRepeatSpellOpcode(WorldPacket& recv_data)
{
	//sLog.outString("Received CMSG_CANCEL_AUTO_REPEAT_SPELL message.");
	//on original we automatically enter combat when creature got close to us
//	GetPlayer()->GetSession()->OutPacket(SMSG_CANCEL_COMBAT);
	GetPlayer()->m_onAutoShot = false;
}

void WorldSession::HandleAddDynamicTargetOpcode(WorldPacket & recvPacket)
{
	uint64 guid;
	uint32 spellid;
	uint32 flags;
	recvPacket >> guid >> spellid >> flags;
	
	SpellEntry * sp = dbcSpell.LookupEntry(spellid);
	// Summoned Elemental's Freeze
    if (spellid == 33395)
    {
        if (!_player->m_Summon)
            return;
    }
    else if (!_player->m_CurrentCharm || guid != _player->m_CurrentCharm->GetGUID())
    {
        return;
    }
	
	/* burlex: this is.. strange */
	SpellCastTargets targets;
	targets.m_targetMask = flags;

	if(flags == 0)
		targets.m_target = GetPlayer()->GetMapMgr()->GetUnit(guid);
	else if(flags & 0x02)
	{
		WoWGuid guid;
		recvPacket >> guid;
		targets.m_target = GetPlayer()->GetMapMgr()->GetUnit(guid.GetOldGuid());
	}
	else if(flags & 0x20)
	{
		recvPacket >> targets.m_srcX >> targets.m_srcY >> targets.m_srcZ;
	}
	else if(flags & 0x40)
	{
		recvPacket >> targets.m_destX >> targets.m_destY >> targets.m_destZ;
	}
	else if (flags & 0x2000)
	{
		recvPacket >> targets.m_strTarget;
	}
	if(spellid == 33395)	// Summoned Water Elemental's freeze
	{
		Spell * pSpell = new Spell(_player->m_Summon, sp, false, 0);
		pSpell->prepare(&targets);
	}
	else			// trinket?
	{
		Spell * pSpell = new Spell(_player->m_CurrentCharm, sp, false, 0);
		pSpell->prepare(&targets);
	}
}
