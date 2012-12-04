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

//----------------------------------------------------------------------------------------
// CMSG_BATTLEFIELD_PORT
//----------------------------------------------------------------------------------------
void WorldSession::HandleBattlefieldPortOpcode(WorldPacket &recv_data)
{
	uint16 mapinfo, unk;
	uint8 action;
	uint32 bgtype;

	if(!_player->IsInWorld()) return;
	recv_data >> unk >> bgtype >> mapinfo >> action;

	if(action == 0)
	{
		//BattlegroundManager.RemovePlayerFromQueues(_player);
		uint32 BGQueueSlot = _player->GetBGQueueSlotByBGType(bgtype);
		_player->RemoveFromBattlegroundQueue(BGQueueSlot);
	}
	else
	{
		/* Usually the fields in the packet would've been used to check what instance we're porting into, however since we're not
		 * doing "queue multiple battleground types at once" we can just use our cached pointer in the player class. - Burlex
		 */

		//if(_player->m_pendingBattleground) 	_player->m_pendingBattleground->PortPlayer(_player); OBSOLETE (Branruz)

		for(uint32 i = 0; i < 3; ++i)
		{
			if( GetPlayer()->m_pendingBattleground[i] && 
				(GetPlayer()->m_pendingBattleground[i]->GetType() == bgtype || bgtype == BATTLEGROUND_ARENA_5V5))
			{
				if( GetPlayer()->m_bg )
				{
					GetPlayer()->m_bg->RemovePlayer(GetPlayer(), true); // Send Logout = true so we can TP him now.
				}

				GetPlayer()->m_pendingBattleground[i]->PortPlayer(GetPlayer(),false);
				return;
			}
		}
	}
}
//----------------------------------------------------------------------------------------
// CMSG_BATTLEFIELD_STATUS
//----------------------------------------------------------------------------------------
void WorldSession::HandleBattlefieldStatusOpcode(WorldPacket &recv_data)
{
	/* This is done based on whether we are queued, inside, or not in a battleground.
	 */
	/*if(_player->m_pendingBattleground)		// Ready to port
		BattlegroundManager.SendBattlefieldStatus(_player, 2, _player->m_pendingBattleground->GetType(), _player->m_pendingBattleground->GetId(), 120000, 0, _player->m_pendingBattleground->Rated());
	else if(_player->m_bg)					// Inside a bg
		BattlegroundManager.SendBattlefieldStatus(_player, 3, _player->m_bg->GetType(), _player->m_bg->GetId(), (uint32)UNIXTIME - _player->m_bg->GetStartTime(), _player->GetMapId(), _player->m_bg->Rated());
	else									// None
		BattlegroundManager.SendBattlefieldStatus(_player, 0, 0, 0, 0, 0, 0);	
    */

	BattlegroundManager.SendBattlegroundQueueStatus(GetPlayer(), 0);
	BattlegroundManager.SendBattlegroundQueueStatus(GetPlayer(), 1);
	BattlegroundManager.SendBattlegroundQueueStatus(GetPlayer(), 2);
}
//----------------------------------------------------------------------------------------
// CMSG_BATTLEFIELD_LIST
//----------------------------------------------------------------------------------------
void WorldSession::HandleBattlefieldListOpcode(WorldPacket &recv_data)
{
	/*
	uint64 guid;
	recv_data >> guid;

	CHECK_INWORLD_RETURN;
	Creature * pCreature = _player->GetMapMgr()->GetCreature( GET_LOWGUID_PART(guid) );
	if( pCreature == NULL )
		return;
    */

    uint32 battlegroundType;
	uint8 requestType, unk1; // 0 = ShowBattlefieldList, 1 = RequestBattlegroundInstanceInfo

	recv_data >> battlegroundType >> requestType >> unk1;

	if( battlegroundType == BATTLEGROUND_ALL_BATTLEGROUND) return; // Systeme du BG aléatoire out, a coder plus tard (Branruz)

	//SendBattlegroundList( pCreature, 0 );
	BattlegroundManager.HandleBattlegroundListPacket(this, battlegroundType, false);
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void WorldSession::SendBattlegroundList(Creature* pCreature, uint32 mapid)
{
   char NameBG[255];

	if(!pCreature) return;

	/* we should have a bg id selection here. */
	int32 BattleGround_Id = -1; // Par Defaut

	 switch (pCreature->m_factionDBC->ID)
	 {
		case 509: // League of Arathor
		case 510: // The Defilers
		{
			BattleGround_Id = BATTLEGROUND_ARATHI_BASIN;
		}break;

		case 729: // The Frostwolf
		case 730: // Stormpike Guard
		{
			BattleGround_Id = BATTLEGROUND_ALTERAC_VALLEY;
		}break;

		case 889: // Warsong Outriders
		case 890: // Silverwing Sentinels
		{
			BattleGround_Id = BATTLEGROUND_WARSONG_GULCH;
		}break;

		default:
              if(pCreature->GetCreatureName())
	           { // A FAIRE: Géré les Ligues (Branruz)
	            if(strstr(pCreature->GetCreatureName()->Name,"Emissaire") != NULL    // Alliance
		            || strstr(pCreature->GetCreatureName()->Name,"Envoyé") != NULL ) // Horde
	             {  // Le nom du Bg est dans le 'Name' de l'Emissaire ou de l'Envoyé
		            strcpy(NameBG,pCreature->GetCreatureName()->Name); 
	             }
	             else 
	             {
		          // Le nom du Bg est dans le 'SubName' du maitre de Guerre
		          strcpy(NameBG,pCreature->GetCreatureName()->SubName);
	             }

			     if(strstr(NameBG,sWorld.m_ArenaSubnameContent) != NULL)       BattleGround_Id = BATTLEGROUND_ARENA_2V2;
	             else if(strstr(NameBG, sWorld.m_ArathiSubnameContent) != NULL)     BattleGround_Id = BATTLEGROUND_ARATHI_BASIN;
	             else if(strstr(NameBG, sWorld.m_WarSongSubnameContent) != NULL)    BattleGround_Id = BATTLEGROUND_WARSONG_GULCH;
	             else if(strstr(NameBG, sWorld.m_AlteracSubnameContent) != NULL)    BattleGround_Id = BATTLEGROUND_ALTERAC_VALLEY;
	             else if(strstr(NameBG,sWorld.m_NetherStormSubnameContent) != NULL) BattleGround_Id = BATTLEGROUND_EYE_OF_THE_STORM;
	             else if(strstr(NameBG,sWorld.m_SotaSubnameContent) != NULL)        BattleGround_Id = BATTLEGROUND_STRAND_OF_THE_ANCIENTS;
			   }
			break;
	 }


    //BattlegroundManager.HandleBattlegroundListPacket(this, t);
	if( BattleGround_Id == -1 )
		SystemMessage("Ce maitre de Guerre ne peut pas vous conduire sur un champ de bataille.");
	else
		BattlegroundManager.HandleBattlegroundListPacket(this, BattleGround_Id, true);
}
//----------------------------------------------------------------------------------------
// CMSG_BATTLEMASTER_HELLO
//----------------------------------------------------------------------------------------
void WorldSession::HandleBattleMasterHelloOpcode(WorldPacket &recv_data)
{
	sLog.outDetail("CMSG_BATTLEMASTER_HELLO Recue!");

	uint64 guid;
	recv_data >> guid;

	CHECK_INWORLD_RETURN;
	Creature* pCreature = _player->GetMapMgr()->GetCreature( GET_LOWGUID_PART(guid) );
	if( pCreature == NULL )
		return;

	SendBattlegroundList( pCreature, 0 );
}
//----------------------------------------------------------------------------------------
// CMSG_LEAVE_BATTLEFIELD
//----------------------------------------------------------------------------------------
void WorldSession::HandleLeaveBattlefieldOpcode(WorldPacket &recv_data)
{
	if (_player->m_bg == NULL || !_player->IsInWorld())
		return;

	if (!_player->m_bg->HasEnded())
	{
		//cast deserter
		SpellEntry* sp = dbcSpell.LookupEntry(__Deserter); // 26013

		if (sp != NULL)
		{
			Spell* s=new Spell(_player, sp, true, NULL);
			SpellCastTargets t(_player);
			s->prepare(&t);
		}
	}
	
	_player->m_bg->RemovePlayer(_player, false);
}
//----------------------------------------------------------------------------------------
// CMSG_AREA_SPIRIT_HEALER_QUERY
//----------------------------------------------------------------------------------------
void WorldSession::HandleAreaSpiritHealerQueryOpcode(WorldPacket &recv_data)
{
	if(!_player->IsInWorld() || !_player->m_bg) return;
	uint64 guid;
	recv_data >> guid;

	Creature * psg = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(psg == NULL)
		return;
	
	uint32 restime = _player->m_bg->GetLastResurrect() + 30;
	if((uint32)UNIXTIME > restime)
		restime = 1000;
	else
		restime = (restime - (uint32)UNIXTIME) * 1000;

	WorldPacket data(SMSG_AREA_SPIRIT_HEALER_TIME, 12);
	data << guid << restime;
	SendPacket(&data);
}
//----------------------------------------------------------------------------------------
// CMSG_AREA_SPIRIT_HEALER_QUEUE
//----------------------------------------------------------------------------------------
void WorldSession::HandleAreaSpiritHealerQueueOpcode(WorldPacket &recv_data)
{
	if(!_player->IsInWorld() || !_player->m_bg) return;
	uint64 guid;
	recv_data >> guid;
	Creature * psg = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(psg == NULL)
		return;

	_player->m_bg->QueuePlayerForResurrect(_player, psg);
	_player->CastSpell(_player,__Waiting_to_Resurrect,true); // 2584
}
//----------------------------------------------------------------------------------------
// MSG_BATTLEGROUND_PLAYER_POSITIONS
//----------------------------------------------------------------------------------------
void WorldSession::HandleBattlegroundPlayerPositionsOpcode(WorldPacket &recv_data)
{
	CBattleground* bg = _player->m_bg;
	if(!_player->IsInWorld() || !bg)
		return;

	uint8 buf[100];
	StackPacket data( MSG_BATTLEGROUND_PLAYER_POSITIONS, buf, 100 );

	if(bg->GetType() == BATTLEGROUND_WARSONG_GULCH)
	{
		uint32 count1 = 0;
		uint32 count2 = 0;

		Player* ap = objmgr.GetPlayer(((static_cast<WarsongGulch *>(bg))->GetAllianceFlagHolderGUID()));
		if(ap) ++count2;

		Player* hp = objmgr.GetPlayer(((static_cast<WarsongGulch *>(bg))->GetHordeFlagHolderGUID()));
		if(hp) ++count2;

		data << count1;
		data << count2;
		if(ap)
		{
			data << (uint64)ap->GetGUID();
			data << (float)ap->GetPositionX();
			data << (float)ap->GetPositionY();
		}
		if(hp)
		{
			data << (uint64)hp->GetGUID();
			data << (float)hp->GetPositionX();
			data << (float)hp->GetPositionY();
		}

		SendPacket(&data);
	}
	else if(bg->GetType() == BATTLEGROUND_EYE_OF_THE_STORM)
	{
		uint32 count1 = 0;
		uint32 count2 = 0;

		Player* ap = objmgr.GetPlayer(((static_cast<EyeOfTheStorm *>(bg))->GetFlagHolderGUID()));
		if(ap) ++count2;

		data << count1;
		data << count2;
		if(ap)
		{
			data << (uint64)ap->GetGUID();
			data << (float)ap->GetPositionX();
			data << (float)ap->GetPositionY();
		}

		SendPacket(&data);
	}
}
//----------------------------------------------------------------------------------------
// CMSG_BATTLEMASTER_JOIN
//----------------------------------------------------------------------------------------
void WorldSession::HandleBattleMasterJoinOpcode(WorldPacket &recv_data)
{
	CHECK_INWORLD_RETURN
	if(_player->GetGroup() && _player->GetGroup()->m_isqueued)
	{
		SystemMessage("Vous êtes dans un groupe qui est en file d'attente pour un champ de bataille ou à l'intérieur d'un champ de bataille.");
		return;
	}

	/* are we already in a queue? */
	if(_player->m_bgIsQueued)
		BattlegroundManager.RemovePlayerFromQueues(_player);

	if(_player->IsInWorld())
		BattlegroundManager.HandleBattlegroundJoin(this, recv_data);
}
//----------------------------------------------------------------------------------------
// CMSG_BATTLEMASTER_JOIN_ARENA
//----------------------------------------------------------------------------------------
void WorldSession::HandleArenaJoinOpcode(WorldPacket &recv_data)
{
	CHECK_INWORLD_RETURN
	if(_player->GetGroup() && _player->GetGroup()->m_isqueued)
	{
		SystemMessage("Vous êtes dans un groupe qui est en file d'attente pour une arene ou à l'intérieur d'une arene.");
		return;
	}

	/* are we already in a queue? */
    if(_player->m_bgIsQueued)
		BattlegroundManager.RemovePlayerFromQueues(_player);

	uint32 bgtype=0;
	uint64 guid;
	uint8 arenacategory;
	uint8 as_group;
	uint8 rated_match;
    recv_data >> guid >> arenacategory >> as_group >> rated_match;
	switch(arenacategory)
	{
	case 0:		// 2v2
		bgtype = BATTLEGROUND_ARENA_2V2;
		break;

	case 1:		// 3v3
		bgtype = BATTLEGROUND_ARENA_3V3;
		break;

	case 2:		// 5v5
		bgtype = BATTLEGROUND_ARENA_5V5;
		break;
	}

	if(bgtype != 0)
		BattlegroundManager.HandleArenaJoin(this, bgtype, as_group, rated_match);
}
//----------------------------------------------------------------------------------------
// MSG_INSPECT_HONOR_STATS
//----------------------------------------------------------------------------------------
void WorldSession::HandleInspectHonorStatsOpcode( WorldPacket &recv_data )
{
    CHECK_PACKET_SIZE( recv_data, 8 );
	CHECK_INWORLD_RETURN

    uint64 guid;
    recv_data >> guid;

  	if( _player == NULL )
	{
		sLog.outError( "HandleInspectHonorStatsOpcode : _player est nul" );
		return;
	}

	if( _player->GetMapMgr() == NULL )
	{
		sLog.outError( "HandleInspectHonorStatsOpcode : _player map mgr est nul" );
		return;
	}

	if( _player->GetMapMgr()->GetPlayer( (uint32)guid ) == NULL )
	{
		sLog.outError( "HandleInspectHonorStatsOpcode : guid est nul" );
		return;
	}

    Player* player =  _player->GetMapMgr()->GetPlayer( (uint32)guid );

    WorldPacket data( MSG_INSPECT_HONOR_STATS, 13 );

    data << player->GetGUID() << (uint8)player->GetUInt32Value( PLAYER_FIELD_HONOR_CURRENCY );
    data << player->GetUInt32Value( PLAYER_FIELD_KILLS );
    data << player->GetUInt32Value( PLAYER_FIELD_TODAY_CONTRIBUTION );
    data << player->GetUInt32Value( PLAYER_FIELD_YESTERDAY_CONTRIBUTION );
    data << player->GetUInt32Value( PLAYER_FIELD_LIFETIME_HONORBALE_KILLS );

    SendPacket( &data );
}
//----------------------------------------------------------------------------------------
// MSG_INSPECT_ARENA_TEAMS
//----------------------------------------------------------------------------------------
void WorldSession::HandleInspectArenaStatsOpcode( WorldPacket & recv_data )
{
    CHECK_PACKET_SIZE( recv_data, 8 );
	CHECK_INWORLD_RETURN

    uint64 guid;
    recv_data >> guid;

    Player* player =  _player->GetMapMgr()->GetPlayer( (uint32)guid );
	if( player == NULL )
	{
		sLog.outError( "HandleInspectHonorStatsOpcode : guid est nul" );
		return;
	}

	ArenaTeam *team;
	uint8 i;
    for( i = 0; i < 3; i++ )
    {
		team = player->m_playerInfo->arenaTeam[i]; // player->GetUInt32Value( PLAYER_FIELD_ARENA_TEAM_INFO_1_1 + ( i * 7 ) );
        if( team )
        {
            ArenaTeamMember *tp = team->GetMember(player->m_playerInfo);
			WorldPacket data( MSG_INSPECT_ARENA_TEAMS, 8 + 1 + 4 * 5 );
			data << player->GetGUID();
			data << uint8(team->m_type);
			data << team->m_id;
			data << team->m_stat_rating;
			if(tp) // send personal stats
			{
				data << tp->Played_ThisSeason;
				data << tp->Won_ThisSeason;
				data << tp->Played_ThisWeek;
				data << tp->PersonalRating;
			} 
			else // send team stats
			{
			 data << team->m_stat_gamesplayedweek;
			 data << team->m_stat_gameswonweek;
			 data << team->m_stat_gamesplayedseason;
			 data << uint32(0);
			}
			SendPacket( &data );


        }
    }
}
//----------------------------------------------------------------------------------------
// MSG_PVP_LOG_DATA
//----------------------------------------------------------------------------------------
void WorldSession::HandlePVPLogDataOpcode(WorldPacket &recv_data)
{
	CHECK_INWORLD_RETURN
	if(_player->m_bg)
		_player->m_bg->SendPVPData(_player);
}


