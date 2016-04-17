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
//CMSG_AREATRIGGER
void WorldSession::HandleAreaTriggerOpcode(WorldPacket & recv_data)
{
	 if(!_player->IsInWorld()) return;
	CHECK_PACKET_SIZE(recv_data, 4);
	uint32 id ;
	recv_data >> id;
	_HandleAreaTriggerOpcode(id);
}

enum AreaTriggerFailures
{
	AREA_TRIGGER_FAILURE_OK				= 0,
	AREA_TRIGGER_FAILURE_UNAVAILABLE	= 1,
	AREA_TRIGGER_FAILURE_NO_BC			= 2,
	AREA_TRIGGER_FAILURE_NO_HEROIC		= 3,
	AREA_TRIGGER_FAILURE_NO_RAID		= 4,
	AREA_TRIGGER_FAILURE_NO_ATTUNE_Q	= 5,
	AREA_TRIGGER_FAILURE_NO_ATTUNE_I	= 6,
	AREA_TRIGGER_FAILURE_LEVEL			= 7,
	AREA_TRIGGER_FAILURE_NO_GROUP		= 8,
	AREA_TRIGGER_FAILURE_NO_KEY			= 9,
	AREA_TRIGGER_FAILURE_LEVEL_HEROIC	= 10,
	AREA_TRIGGER_FAILURE_NO_CHECK		= 11,
	AREA_TRIGGER_FAILURE_NO_WOTLK		= 12,
	AREA_TRIGGER_FAILURE_IN_QUEUE		= 13,
	AREA_TRIGGER_FAILURE_WRONG_GROUP	= 14,
};

const char * AreaTriggerFailureMessages[] = {
	"-",                                                                                        //0
   "Cette instance est actuellement indisponible.",                                             //1
	"Vous devez avoir installé l'extension 'The Burning Crusade' pour avoir accès à ce contenu.", //2
	"Le mode héroïque n'est pas disponible pour cette instance.",                               //3
	"Vous devez être dans un groupe de raid pour pouvoir entrer dans cette instance.",          //4  
	"Vous n'avez pas la quete voulu pour entrer ici.", // TODO questname                        //5
	"Vous n'avez pas l'objet voulu pour entrer ici.",  //TODO: itemname                         //6
	"Vous devez être niveau %u pour entrer ici.",    //7 Ne pas modifier ce message (Branruz)
	"Vous devez être dans dans un groupe pour traverser ici.",                                  //8
	"Vous n'avez pas la clef pour entrer ici.",                                                 //9
	"Vous n'avez pas le niveau requis pour entrer en mode heroique.",                           //10
	"Vous ne pouvez pas entrer.",                                                               //11
	"Vous devez avoir installé l'extension 'Wrath of the Linch King' pour avoir accès à ce contenu.", //12
	"Vous êtes en attente pour entrer dans le groupe de Raid.",                                  //13
	"Vous n'êtes pas dans le bon groupe de raid pour entrer ici",								//14
};

uint32 CheckTriggerPrerequsites(AreaTrigger * pAreaTrigger, WorldSession * pSession, Player * pPlayer, MapInfo * pMapInfo)
{
//is this map enabled?
	if( pMapInfo  == NULL || !pMapInfo->HasFlag(WMI_INSTANCE_ENABLED))
		return AREA_TRIGGER_FAILURE_UNAVAILABLE;

	//Do we need TBC expansion?
	if(!pSession->HasFlag(ACCOUNT_FLAG_XPACK_01) && pMapInfo->HasFlag(WMI_INSTANCE_XPACK_01))
		return AREA_TRIGGER_FAILURE_NO_BC;

	//Do we need WOTLK expansion?
	if(!pSession->HasFlag(ACCOUNT_FLAG_XPACK_02) && pMapInfo->HasFlag(WMI_INSTANCE_XPACK_02))
		return AREA_TRIGGER_FAILURE_NO_WOTLK;

	//Do we meet the areatrigger level requirements?
	if( pAreaTrigger != NULL && pAreaTrigger->required_level && pPlayer->getLevel() < pAreaTrigger->required_level)
		return AREA_TRIGGER_FAILURE_LEVEL;

	//Do we meet the map level requirements?
	if( pPlayer->getLevel() < pMapInfo->minlevel )
		return AREA_TRIGGER_FAILURE_LEVEL;

	// These can be overridden by cheats/GM
	if(!pPlayer->triggerpass_cheat)
	{
		//Do we need any quests?
		if( ( pMapInfo->required_quest_A1 || pMapInfo->required_quest_H2 )  && !( pPlayer->HasFinishedDailyQuest(pMapInfo->required_quest_A1) || pPlayer->HasFinishedDailyQuest(pMapInfo->required_quest_A1) || pPlayer->HasFinishedDailyQuest(pMapInfo->required_quest_H2) || pPlayer->HasFinishedDailyQuest(pMapInfo->required_quest_H2)))
			return AREA_TRIGGER_FAILURE_NO_ATTUNE_Q;

		//Do we need certain items?
		if( pMapInfo->required_item && !pPlayer->GetItemInterface()->GetItemCount(pMapInfo->required_item, true))
			return AREA_TRIGGER_FAILURE_NO_ATTUNE_I;

		//Do we need to be in a group?
		if((pMapInfo->type == INSTANCE_RAID || pMapInfo->type == INSTANCE_MULTIMODE ) && !pPlayer->GetGroup())
			return AREA_TRIGGER_FAILURE_NO_GROUP;

		//Does the group have to be a raid group?
		if( pMapInfo->type == INSTANCE_RAID && pPlayer->GetGroup()->GetGroupType() != GROUP_TYPE_RAID )
			return AREA_TRIGGER_FAILURE_NO_RAID;

		//Are we trying to enter a non-heroic instance in heroic mode?
		if( pPlayer->iInstanceType >= MODE_HEROIC && pMapInfo->type != INSTANCE_MULTIMODE && pMapInfo->type != INSTANCE_NULL)
			return AREA_TRIGGER_FAILURE_NO_HEROIC;

		//Are we trying to enter a saved raid/heroic instance?
		if( pMapInfo->type == INSTANCE_RAID || pPlayer->iInstanceType >= MODE_HEROIC && pMapInfo->type != INSTANCE_NULL )
		{
			//Raid queue, did we reach our max amt of players?
			if( pPlayer->m_playerInfo && pMapInfo->playerlimit >= 5 && (int32)((pMapInfo->playerlimit - 5)/5) < pPlayer->m_playerInfo->subGroup)
				return AREA_TRIGGER_FAILURE_IN_QUEUE;

			// Need http://www.wowhead.com/?spell=46591 to enter Magisters Terrace
			if( pMapInfo->mapid == 585 && pPlayer->iInstanceType >= MODE_HEROIC && !pPlayer->HasSpell(46591)) // Heroic Countenance
				return AREA_TRIGGER_FAILURE_NO_HEROIC;

			//All Heroic instances are automatically unlocked when reaching lvl 80, no keys needed here.
			if(!(pMapInfo->HasFlag(WMI_INSTANCE_XPACK_02) && pPlayer->getLevel()>= 80))
			{
				//otherwise we still need to be lvl 65 for heroic.
				if( pPlayer->iInstanceType && pPlayer->getLevel() < uint32(pMapInfo->HasFlag(WMI_INSTANCE_XPACK_02) ? 80 : 70))
					return AREA_TRIGGER_FAILURE_LEVEL_HEROIC;

				//and we might need a key too.
				bool reqkey = (pMapInfo->heroic_key_1||pMapInfo->heroic_key_2)?true:false;
				bool haskey = (pPlayer->GetItemInterface()->GetItemCount(pMapInfo->heroic_key_1, false) || pPlayer->GetItemInterface()->GetItemCount(pMapInfo->heroic_key_2, false))?true:false;
				if(reqkey && !haskey)
					return AREA_TRIGGER_FAILURE_NO_KEY;
			}
		}
	}
	//Nothing more to check, should be ok
	return AREA_TRIGGER_FAILURE_OK;
}

void WorldSession::_HandleAreaTriggerOpcode(uint32 id)
{		
	sLog.outDebug("AreaTrigger: %u", id);

	// Are we REALLY here?
	if( !_player->IsInWorld() ) return;
		
	AreaTriggerEntry* entry = dbcAreaTrigger.LookupEntry(id);
    // Normalement ne doit pas arriver mais avec les patchs, les DBCs sont pas forcement a jour.... (Branruz)
    if( entry == NULL )
	{
	 //sLog.outDebug("AreaTrigger Manquant: %u", id);
	 //Log.Error("[HandleAreaTriggerOpcode]","AreaTrigger %u manquant dans AreaTrigger.dbc <- Fix this with correct DBCs", id);
	 if( _player->GetSession()->HasSuperAdminPermissions() )
		sChatHandler.BlueSystemMessage( this, "[%sSysteme%s] |rAreatrigger Manquant: %s%u, Dbc non a jour .", MSG_COLOR_WHITE, MSG_COLOR_RED,MSG_COLOR_LIGHTBLUE, id );
    
	 return;
	}


    AreaTrigger* pAreaTrigger = AreaTriggerStorage.LookupEntry( id );

	// Affichage des AreaTrigger meme s'il n'existe pas dans la table (Branruz)
	// Ca permettra de completer, le truc louche c'est ke normalement ils sont dans les DBCs!!
	// donc la table devrait normalement etre complete!
    if( pAreaTrigger == NULL )
	{
	 //sLog.outDebug("AreaTrigger Manquant: %u", id);
	 //Log.Error("[HandleAreaTriggerOpcode]","AreaTrigger Manquant: %u <- Complete this in your DB", id);
	 if( _player->GetSession()->HasSuperAdminPermissions() )
		sChatHandler.BlueSystemMessage( this, "[%sSysteme%s] |rAreatrigger Manquant: %s%u .", MSG_COLOR_WHITE, MSG_COLOR_RED,MSG_COLOR_LIGHTBLUE, id );

		return;
	}
    // Search quest log, find any exploration quests
	sQuestMgr.OnPlayerExploreArea(GetPlayer(),id);

	
	sHookInterface.OnAreaTrigger(GetPlayer(), id); // Script
 
	if( _player->GetSession()->HasSuperAdminPermissions() )
		sChatHandler.BlueSystemMessage( this, "[%sSysteme%s] |rAreatrigger: %s%u (%s).", MSG_COLOR_WHITE, MSG_COLOR_LIGHTBLUE, MSG_COLOR_SUBWHITE, id, pAreaTrigger->Name );

	
	// if in BG handle is triggers
	if( _player->m_bg )
	{
		_player->m_bg->HookOnAreaTrigger(_player, id);
		return;
	}

	// Hook for Scripted Areatriggers
	_player->GetMapMgr()->HookOnAreaTrigger(_player, id);

	_player->SetLastAreaTrigger(pAreaTrigger);
	switch(pAreaTrigger->Type)
	{
	case ATTYPE_INSTANCE:
		{
			if(GetPlayer()->GetPlayerStatus() != TRANSFER_PENDING) //only ports if player is out of pendings
			{
				MapInfo * pMi = WorldMapInfoStorage.LookupEntry(pAreaTrigger->Mapid);
				if (!pMi)
					return; // Sécurité - Randdrick

				uint32 reason = CheckTriggerPrerequsites(pAreaTrigger, this, _player, WorldMapInfoStorage.LookupEntry(pAreaTrigger->Mapid));
				if(reason != AREA_TRIGGER_FAILURE_OK)
				{
					const char * pReason = AreaTriggerFailureMessages[reason];
					char msg[200];
					WorldPacket data(SMSG_AREA_TRIGGER_MESSAGE, 50);
					data << uint32(0);
                    
					switch (reason)
					{
					case AREA_TRIGGER_FAILURE_LEVEL:
						snprintf(msg,200,pReason,pAreaTrigger->required_level);
						data << msg;
						break;
					case AREA_TRIGGER_FAILURE_NO_ATTUNE_I:
						{
							ItemPrototype * pItem = ItemPrototypeStorage.LookupEntry(pMi->required_item);
							if(pItem)
								snprintf(msg,200,"Vous devez avoir l'object, `%s` pour passer par ici.",pItem->Name1);
							else
								snprintf(msg,200,"Vous devez avoir l'object, INCONNU pour passer par ici.");

							data << msg;
						}break;
					case AREA_TRIGGER_FAILURE_NO_ATTUNE_Q:
						{
							Quest * pQuestA1 = QuestStorage.LookupEntry(pMi->required_quest_A1);
							Quest * pQuestH2 = QuestStorage.LookupEntry(pMi->required_quest_H2);							
							if(pQuestA1)
								snprintf(msg,200,"Vous devez avoir fini la quête pour l'Alliance, `%s` pour passer par ici.",pQuestA1->title);
							else							
							if(pQuestH2)
								snprintf(msg,200,"Vous devez avoir fini la quête pour la Horde, `%s` pour passer par ici.",pQuestH2->title);

							data << msg;
						}break;
					case AREA_TRIGGER_FAILURE_NO_KEY:
						{
							string temp_msg[2];
							string tmp_msg;

								if ((pMi->heroic_key_1 && _player->GetItemInterface()->GetItemCount(pMi->heroic_key_1, false)==0))
								{
									ItemPrototype * pKey = ItemPrototypeStorage.LookupEntry(pMi->heroic_key_1);
									if(pKey)
										temp_msg[0] += pKey->Name1;
									else
										temp_msg[0] += "UNKNOWN";
								}

								if ((pMi->heroic_key_2 && _player->GetItemInterface()->GetItemCount(pMi->heroic_key_2, false)==0))
								{
									ItemPrototype * pKey = ItemPrototypeStorage.LookupEntry(pMi->heroic_key_2);
									if(pKey)
										temp_msg[1] += pKey->Name1;
									else
										temp_msg[0] += "UNKNOWN";
								}

							tmp_msg += temp_msg[0];
							if(temp_msg[0].size() && temp_msg[1].size())
								tmp_msg += "\" and \"";
							tmp_msg += temp_msg[1];

							snprintf(msg, 200, pReason, tmp_msg.c_str());
							data << msg;
						}break;
					case AREA_TRIGGER_FAILURE_LEVEL_HEROIC:
						{
							snprintf(msg, 200, pReason, pMi->HasFlag(WMI_INSTANCE_XPACK_02) ? 80 : 70);
							data << msg;
						}break;
					default:
						data << pReason;
						break;
					}

					data << uint8(0);
					SendPacket(&data);
					return;
				}

			if( _player->IsMounted())
				_player->RemoveAura(_player->m_MountSpellId); // Note Randdrick : A l'entrée d'une Instance, on déscend aussi de sa monture !
			_player->m_MountSpellId = 0;
				uint32 InstanceID = 0;
				// Try to find a saved instance and
				// do not handle Hyjal Inn (trigger 4319), since we need a unique mapid when generating our instance_id.

				if( id != 4319 && pMi && ( pMi->type == INSTANCE_RAID || _player->iInstanceType >= MODE_HEROIC && pMi->type == INSTANCE_MULTIMODE ) )
				{
					//Do we have a saved instance we should use?
					Instance * in = NULL;
					in = sInstanceMgr.GetSavedInstance( pMi->mapid,_player->GetLowGUID() );
					if( in != NULL  && in->m_instanceId )
					{
						//If we are the first to enter this instance, also set our current group id.
						if( in->m_mapMgr == NULL || (!in->m_mapMgr->HasPlayers() && _player->GetGroupID() != in->m_creatorGroup))
							in->m_creatorGroup =_player->GetGroupID();
						InstanceID = in->m_instanceId;
					}
				}

				//Save our entry point and try to teleport to our instance
				GetPlayer()->SaveEntryPoint(pAreaTrigger->Mapid);
				GetPlayer()->SafeTeleport(pAreaTrigger->Mapid, InstanceID, LocationVector(pAreaTrigger->x, pAreaTrigger->y, pAreaTrigger->z, pAreaTrigger->o));
			}
		}break;
	case ATTYPE_QUESTTRIGGER:
		{

		}break;
	case ATTYPE_INN:
		{
			if( _player->IsMounted())
			{
				_player->RemoveAura(_player->m_MountSpellId);
				_player->m_MountSpellId = 0;
			}

			// Inn
			if (!GetPlayer()->m_isResting) GetPlayer()->ApplyPlayerRestState(true); // zzz

		}break;
	case ATTYPE_TELEPORT:
		{
			if(GetPlayer()->GetPlayerStatus() != TRANSFER_PENDING) //only ports if player is out of pendings
			{
				if( _player->IsMounted())
					_player->RemoveAura(_player->m_MountSpellId);

				GetPlayer()->SaveEntryPoint(pAreaTrigger->Mapid);
				GetPlayer()->SafeTeleport(pAreaTrigger->Mapid, 0, LocationVector(pAreaTrigger->x, pAreaTrigger->y, pAreaTrigger->z, pAreaTrigger->o));
			}
		}break;
	default:break;
	}
}
