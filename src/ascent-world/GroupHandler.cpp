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

//////////////////////////////////////////////////////////////
/// This function handles CMSG_GROUP_INVITE
//////////////////////////////////////////////////////////////
void WorldSession::HandleGroupInviteOpcode( WorldPacket & recv_data )
{
	// Memo
    // _Player : Celui qui envoi l'invite
	// player : Celui qui recoie l'invite

	if(!_player->IsInWorld()) return;

	sLog.outDetail( "WORLD: %s envoi CMSG_GROUP_INVITE.",_player->GetName() );

	CHECK_PACKET_SIZE(recv_data, 1);
	WorldPacket data(100);
	std::string membername;
	Player * player = NULL;
	Group *group = NULL;

	recv_data >> membername;

	if(_player->HasBeenInvited()) 
	{
        //Log.Notice("[WorldSession::HandleGroupInviteOpcode]","HasBeenInvited = true!" );
		return;
	}

	player = objmgr.GetPlayer(membername.c_str(), false);

	if ( player == NULL)
	{
		SendPartyCommandResult(_player, 0, membername, ERR_PARTY_CANNOT_FIND);
		//Log.Notice("[WorldSession::HandleGroupInviteOpcode]","WORLD: sent ERR_PARTY_CANNOT_FIND." );
		return;
	}

	//Log.Notice("[WorldSession::HandleGroupInviteOpcode]","%s a recu une invitation de groupe",player->GetName() );
	if (player == _player)
	{
		//Log.Notice("[WorldSession::HandleGroupInviteOpcode]","player == _player" );
		return;
	}

	if ( _player->InGroup() && (!_player->IsGroupLeader() || _player->GetGroup()->HasFlag(GROUP_FLAG_BATTLEGROUND_GROUP) ) )
	{
		SendPartyCommandResult(_player, 0, "", ERR_PARTY_YOU_ARE_NOT_LEADER);
		//Log.Notice("[WorldSession::HandleGroupInviteOpcode]","WORLD: sent ERR_PARTY_YOU_ARE_NOT_LEADER." );
		return;
	}

	group = _player->GetGroup();
	if ( group != NULL )
	{
		if (group->IsFull())
		{
			SendPartyCommandResult(_player, 0, "", ERR_PARTY_IS_FULL);
			//Log.Notice("[WorldSession::HandleGroupInviteOpcode]","WORLD: sent ERR_PARTY_IS_FULL." );
			return;
		}
	}

	if ( player->InGroup() )
	{
		SendPartyCommandResult(_player, player->GetGroup()->GetGroupType(), membername, ERR_PARTY_ALREADY_IN_GROUP);
		//Log.Notice("[WorldSession::HandleGroupInviteOpcode]","WORLD: sent ERR_PARTY_ALREADY_IN_GROUP1." );
		data.SetOpcode(SMSG_GROUP_INVITE);
		data << uint8(0);
		data << GetPlayer()->GetName();
		player->GetSession()->SendPacket(&data);
		return;
	}
	
	if(player->GetTeam()!=_player->GetTeam() && !_player->GetSession()->HasPermissions())
	{
		SendPartyCommandResult(_player, 0, membername, ERR_PARTY_WRONG_FACTION);
		//Log.Notice("[WorldSession::HandleGroupInviteOpcode]","WORLD: sent ERR_PARTY_WRONG_FACTION." );
		return;
	}

	if ( player->HasBeenInvited() )
	{
		SendPartyCommandResult(_player, 0, membername, ERR_PARTY_ALREADY_IN_GROUP);
		//Log.Notice("[WorldSession::HandleGroupInviteOpcode]","WORLD: sent ERR_PARTY_ALREADY_IN_GROUP2." );
		return;
	}

	if( player->Social_IsIgnoring( _player->GetLowGUID() ) )
	{
		SendPartyCommandResult(_player, 0, membername, ERR_PARTY_IS_IGNORING_YOU);
		//Log.Notice("[WorldSession::HandleGroupInviteOpcode]","WORLD: sent ERR_PARTY_IS_IGNORING_YOU." );
		return;
	}

	if( player->bGMTagOn && !_player->GetSession()->HasPermissions())
	{
		SendPartyCommandResult(_player, 0, membername, ERR_PARTY_CANNOT_FIND);
		return;
	}

	// 16/08/06 - change to guid to prevent very unlikely event of a crash in deny, etc
	_player->SetInviter(_player->GetLowGUID());//bugfix if player invtied 2 people-> he can be in 2 parties
	player->SetInviter(_player->GetLowGUID()); // Faut pas l'oublier celui-la (Branruz)
	//_player->SetInviter(0); // Pour test (Branrue)

	//sLog.outDetail( "WORLD: sent SMSG_GROUP_INVITE, GetInviter : %u",_player->GetInviter() );
		
	//snprintf(_buff_,20,"%s",GetPlayer()->GetName());
	uint8 sizename= (uint8)strlen(_player->GetName());
	data.SetOpcode(SMSG_GROUP_INVITE);
	data << (uint8)(sizename); // sizename
	data << _player->GetName(); //_buff_
	player->GetSession()->SendPacket(&data);

	uint32 gtype = 0;
	if(group)
		gtype = group->GetGroupType();

	SendPartyCommandResult(_player, gtype, membername, ERR_PARTY_NO_ERROR);
	//Log.Notice("[WorldSession::HandleGroupInviteOpcode]","WORLD: sent ERR_PARTY_NO_ERROR." );
}

///////////////////////////////////////////////////////////////
///This function handles CMSG_GROUP_CANCEL:
///////////////////////////////////////////////////////////////
void WorldSession::HandleGroupCancelOpcode( WorldPacket & recv_data )
{
	sLog.outDetail( "WORLD: got CMSG_GROUP_CANCEL. <- Nothing" ); 
	
	//if(!_player) return;
	//if(!_player->IsInWorld()) return;
	
}

////////////////////////////////////////////////////////////////
///This function handles CMSG_GROUP_ACCEPT:
////////////////////////////////////////////////////////////////
void WorldSession::HandleGroupAcceptOpcode( WorldPacket & recv_data )
{
	// Marche en Question/Reponse avec CMSG_GROUP_INVITE
	// Memo
	// _player : celui qui repond "Accepter" a l'invitation
	// player : celui qui a envoy� l'invitation
	// Attention: data player de GROUP_INVITE devient data _player de GROUP_ACCEPT ;) (branruz)
   
	if(!_player) return;
	if(!_player->IsInWorld()) return;

    sLog.outDetail( "WORLD: %s got CMSG_GROUP_ACCEPT." , _player->GetName());

	Player *player = objmgr.GetPlayer(_player->GetInviter());

	if ( !player ) 
	{
		Log.Error( "[CMSG_GROUP_ACCEPT]","(%s) GetInviter erronee %u" ,_player->GetName() , _player->GetInviter());
		return;
	}
	
	player->SetInviter(0);
	_player->SetInviter(0);
	
	Group *grp = player->GetGroup();

	if(grp)
	{
		grp->AddMember(_player->m_playerInfo);
		if(grp->GetLeader()->m_loggedInPlayer)
			_player->iInstanceType = grp->GetLeader()->m_loggedInPlayer->iInstanceType;
       
		_player->GetSession()->OutPacket(MSG_SET_DUNGEON_DIFFICULTY, 4, &_player->iInstanceType);
        sLog.outDetail( "WORLD: sent MSG_SET_DUNGEON_DIFFICULTY." );
		//sInstanceSavingManager.ResetSavedInstancesForPlayer(_player);
		return;
	}
	
	// If we're this far, it means we have no existing group, and have to make one.
	grp = new Group(true);
	grp->AddMember(player->m_playerInfo);		// add the inviter first, therefore he is the leader
	grp->AddMember(_player->m_playerInfo);	   // add us.
    _player->iInstanceType = player->iInstanceType;

    _player->GetSession()->OutPacket(MSG_SET_DUNGEON_DIFFICULTY, 4, &player->iInstanceType);
	
	sLog.outDetail( "WORLD: sent MSG_SET_DUNGEON_DIFFICULTY." );
	
	//sInstanceSavingManager.ResetSavedInstancesForPlayer(_player);

	// Currentgroup and all that shit are set by addmember.
}

///////////////////////////////////////////////////////////////////////////////////////
///This function handles CMSG_GROUP_DECLINE:
//////////////////////////////////////////////////////////////////////////////////////
void WorldSession::HandleGroupDeclineOpcode( WorldPacket & recv_data )
{
	sLog.outDetail( "WORLD: got CMSG_GROUP_DECLINE." );
	
	if(!_player) return;
	if(!_player->IsInWorld()) return;
	WorldPacket data(SMSG_GROUP_DECLINE, 100);

	Player *player = objmgr.GetPlayer(_player->GetInviter());
	if(!player) return;

	data << GetPlayer()->GetName();

	player->GetSession()->SendPacket( &data );
	player->SetInviter(0);
	_player->SetInviter(0);

	sLog.outDetail( "WORLD: sent SMSG_GROUP_DECLINE." );
}

//////////////////////////////////////////////////////////////////////////////////////////
///This function handles CMSG_GROUP_UNINVITE:
//////////////////////////////////////////////////////////////////////////////////////////
void WorldSession::HandleGroupUninviteOpcode( WorldPacket & recv_data )
{
	sLog.outDetail( "WORLD: got CMSG_GROUP_UNINVITE." );
    
	if(!_player) return;
	if(!_player->IsInWorld()) return;
	CHECK_PACKET_SIZE(recv_data, 1);
	std::string membername;
	
	Player * player;
	PlayerInfo * info;

	recv_data >> membername;

	player = objmgr.GetPlayer(membername.c_str(), false);
	info = objmgr.GetPlayerInfoByName(membername.c_str());

	if ( player == NULL && info == NULL )
	{
		SendPartyCommandResult(_player, 0, membername, ERR_PARTY_CANNOT_FIND);
		return;
	}

	_player->GroupUninvite(player, info); // 
}

//////////////////////////////////////////////////////////////////////////////////////////
///This function handles CMSG_GROUP_UNINVITE_GUID: (par Ascent 313 rev5 )
//////////////////////////////////////////////////////////////////////////////////////////
void WorldSession::HandleGroupUninviteGuildOpcode( WorldPacket & recv_data )
{
	sLog.outDetail( "WORLD: got CMSG_GROUP_UNINVITE_GUID." );
	
    if(!_player) return;
    if(!_player->IsInWorld()) return;
	CHECK_PACKET_SIZE(recv_data, 1);
	std::string membername;
	Player *player;
	PlayerInfo * info;
	uint64 guid;

	recv_data >> guid;

	player = objmgr.GetPlayer(guid);
	info = objmgr.GetPlayerInfo(guid);
	if ( player == NULL && info == NULL )
	{
		SendPartyCommandResult(_player, 0, membername, ERR_PARTY_CANNOT_FIND);
		return;
	}
	_player->GroupUninvite(player, info);
}

//////////////////////////////////////////////////////////////////////////////////////////
///This function handles CMSG_GROUP_SET_LEADER:
//////////////////////////////////////////////////////////////////////////////////////////
void WorldSession::HandleGroupSetLeaderOpcode( WorldPacket & recv_data )
{
	sLog.outDetail( "WORLD: got CMSG_GROUP_SET_LEADER." );
    
	if(!_player) return;
	if(!_player->IsInWorld()) return;
	// important note _player->GetName() can be wrong.
	CHECK_PACKET_SIZE(recv_data, 1);
	WorldPacket data;
	uint64 MemberGuid;
	Player * player;

	recv_data >> MemberGuid;
	
	player = objmgr.GetPlayer((uint32)MemberGuid);

	if ( player == NULL )
	{
		//SendPartyCommandResult(_player, 0, membername, ERR_PARTY_CANNOT_FIND);
		SendPartyCommandResult(_player, 0, _player->GetName(), ERR_PARTY_CANNOT_FIND);
		return;
	}

	if(!_player->IsGroupLeader())
	{
		SendPartyCommandResult(_player, 0, "", ERR_PARTY_YOU_ARE_NOT_LEADER);
		return;
	}
	
	if(player->GetGroup() != _player->GetGroup())
	{
		//SendPartyCommandResult(_player, 0, membername, ERR_PARTY_IS_NOT_IN_YOUR_PARTY);
		SendPartyCommandResult(_player, 0, _player->GetName(), ERR_PARTY_IS_NOT_IN_YOUR_PARTY);
		return;
	}

	Group *pGroup = _player->GetGroup();
	if(pGroup)
		pGroup->SetLeader(player,false);
}

//////////////////////////////////////////////////////////////////////////////////////////
///This function handles CMSG_GROUP_DISBAND:
//////////////////////////////////////////////////////////////////////////////////////////
void WorldSession::HandleGroupDisbandOpcode( WorldPacket & recv_data )
{
	sLog.outDetail( "WORLD: got CMSG_GROUP_DISBAND." );
   
	if(!_player) return;
	if(!_player->IsInWorld()) return;
	Group* pGroup = _player->GetGroup();
	if(!pGroup) return;

	if(pGroup->HasFlag(GROUP_FLAG_BATTLEGROUND_GROUP)) return;

	//Update the group list (Ascent 313 rev5)
	pGroup->SendNullUpdate( _player );
	//pGroup->Disband();
	pGroup->RemovePlayer(_player->m_playerInfo);
}

//////////////////////////////////////////////////////////////////////////////////////////
///This function handles CMSG_LOOT_METHOD:
//////////////////////////////////////////////////////////////////////////////////////////
void WorldSession::HandleLootMethodOpcode( WorldPacket & recv_data )
{
	sLog.outDetail( "WORLD: got CMSG_LOOT_METHOD." );

	if(!_player) return;
	if(!_player->IsInWorld()) return;
	CHECK_PACKET_SIZE(recv_data, 16);
	uint32 lootMethod;
	uint64 lootMaster;
	uint32 threshold;

	recv_data >> lootMethod >> lootMaster >>threshold;
  
	if(!_player->IsGroupLeader())
	{
		SendPartyCommandResult(_player, 0, "", ERR_PARTY_YOU_ARE_NOT_LEADER);
		return;
	}
	
	// TODO: fix me burlex 
	//Player *plyr = objmgr.GetPlayer((uint32)lootMaster);
	//if(!plyr)return;
	Group* pGroup = _player->GetGroup();
	if( pGroup != NULL)
		pGroup->SetLooter( _player, lootMethod, threshold );
}

void WorldSession::HandleMinimapPingOpcode( WorldPacket & recv_data )
{
	if(!_player) return;
	if(!_player->IsInWorld()) return;
	CHECK_PACKET_SIZE(recv_data, 8);
	if(!_player->InGroup())
	return;
	Group * party= _player->GetGroup();
	if(!party)return;

	float x,y;
	recv_data >> x >>y;
	WorldPacket data;
	data.SetOpcode(MSG_MINIMAP_PING);
	data << _player->GetGUID();
	data << x << y;
	party->SendPacketToAllButOne(&data, _player);
}

void WorldSession::HandleSetPlayerIconOpcode(WorldPacket& recv_data)
{
	uint64 guid;
	uint8 icon;
	Group * pGroup = _player->GetGroup();
	if(!_player->IsInWorld() || !pGroup) return;

	recv_data >> icon;
	if(icon == 0xFF)
	{
		// client request
		WorldPacket data(MSG_RAID_TARGET_UPDATE, 73);
		data << uint8(1);
		for(uint8 i = 0; i < 8; ++i)
			data << i << pGroup->m_targetIcons[i];

		SendPacket(&data);
	}
	else if(_player->IsGroupLeader())
	{
		recv_data >> guid;
		if(icon > 7)
			return;			// whhopes,buffer overflow :p

		// setting icon
		WorldPacket data(MSG_RAID_TARGET_UPDATE, 10);
		data << uint8(0) << icon << guid;
		pGroup->SendPacketToAll(&data);

		pGroup->m_targetIcons[icon] = guid;
	}
}

void WorldSession::SendPartyCommandResult(Player *pPlayer, uint32 p1, std::string name, uint32 err)
{
	if(!_player) return;
	if(!_player->IsInWorld()) return;
	// if error message do not work, please sniff it and leave me a message
	if(pPlayer)
	{
		WorldPacket data;
		data.Initialize(SMSG_PARTY_COMMAND_RESULT);
		data << p1;
		if(!name.length())
			data << uint8(0);
		else
			data << name.c_str();

		data << err;
		pPlayer->GetSession()->SendPacket(&data);
	}
}
