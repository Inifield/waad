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

#include "RStdAfx.h"
#include "ChannelMgr.h"

initialiseSingleton( ChannelMgr );

void Session::HandleChannelJoin(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname, pass;
	uint32 dbc_id = 0;
	uint16 crap;		// crap = some sort of channel type?
	Channel * chn;

	recvPacket >> dbc_id >> crap;
	recvPacket >> channelname;
	recvPacket >> pass;

	//if(!stricmp(channelname.c_str(), sRMaster.lookingforchannel.c_str()) && !sRMaster.m_lfgForNonLfg)
	if( (dbc_id == CHANNEL_RECHERCHE_GROUPE) && !sRMaster.m_lfgForNonLfg )
	{
		WorldPacket data(ISMSG_CHANNEL_LFG_DUNGEON_STATUS_REQUEST, 4+4+2+channelname.size()+pass.size());
		data << m_currentPlayer->Guid;
		data << dbc_id;
		data << crap;
		data << channelname;
		data << pass;
		GetServer()->SendPacket(&data);
		return;
	}

	if( sRMaster.GmClientChannel.size() && !stricmp(sRMaster.GmClientChannel.c_str(), channelname.c_str()) && !m_GMPermissions.size())
		return;
	
	chn = channelmgr.GetCreateChannel(channelname.c_str(), m_currentPlayer, dbc_id);
	if(chn == NULL)
		return;

	chn->AttemptJoin(m_currentPlayer, pass.c_str());
	Log.Debug("ChannelJoin", "%s, canal numero : %u", channelname.c_str(),chn->m_channelId);
}

void Session::HandleChannelLeave(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	uint32 code = 0;
	Channel * chn;

	recvPacket >> code;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	if(chn == NULL)
		return;

	chn->Part(m_currentPlayer,false);
	Log.Debug("ChannelLeft", "%s, canal numero : %u", channelname.c_str(),chn->m_channelId);
}

void Session::HandleChannelList(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;

	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	if( chn != NULL )
		chn->List(m_currentPlayer);
}

void Session::HandleChannelPassword(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,pass;
	Channel * chn;

	recvPacket >> channelname;
	recvPacket >> pass;
	chn = channelmgr.GetChannel(channelname.c_str(),m_currentPlayer);
	if(chn)
		chn->Password(m_currentPlayer, pass.c_str());
}

void Session::HandleChannelSetOwner(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp.c_str());
	if(chn && plr)
		chn->SetOwner(m_currentPlayer, plr);
}

void Session::HandleChannelOwner(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,pass;
	Channel * chn;

	recvPacket >> channelname;
	chn = channelmgr.GetChannel(channelname.c_str(),m_currentPlayer);
	if(chn)
		chn->GetOwner(m_currentPlayer);
}

void Session::HandleChannelModerator(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp.c_str());
	if(chn && plr)
		chn->GiveModerator(m_currentPlayer, plr);
}

void Session::HandleChannelUnmoderator(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp.c_str());
	if(chn && plr)
		chn->TakeModerator(m_currentPlayer, plr);
}

void Session::HandleChannelMute(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp.c_str());
	if(chn && plr)
		chn->Mute(m_currentPlayer, plr);
}

void Session::HandleChannelUnmute(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp.c_str());
	if(chn && plr)
		chn->Unmute(m_currentPlayer, plr);
}

void Session::HandleChannelInvite(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp.c_str());
	if(chn && plr)
		chn->Invite(m_currentPlayer, plr);
}
void Session::HandleChannelKick(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp.c_str());
	if(chn && plr)
		chn->Kick(m_currentPlayer, plr, false);
}

void Session::HandleChannelBan(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp.c_str());
	if(chn && plr)
		chn->Kick(m_currentPlayer, plr, true);
}

void Session::HandleChannelUnban(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	RPlayerInfo * plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	plr = sClientMgr.GetRPlayer(newp.c_str());
	if(chn && plr)
		chn->Unban(m_currentPlayer, plr);
}

void Session::HandleChannelAnnounce(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	if(chn)
		chn->Announce(m_currentPlayer);
}

void Session::HandleChannelModerate(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	if(chn)
		chn->Moderate(m_currentPlayer);
}

void Session::HandleChannelRosterQuery(WorldPacket & recvPacket)
{
	string channelname;
	Channel * chn;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), m_currentPlayer);
	if(chn)
		chn->List(m_currentPlayer);
}

void Session::HandleChannelNumMembersQuery(WorldPacket & recvPacket)
{
	string channel_name;
	WorldPacket data(SMSG_CHANNEL_MEMBER_COUNT, recvPacket.size() + 4);
	Channel *chn;
	recvPacket >> channel_name;
	chn = channelmgr.GetChannel(channel_name.c_str(), m_currentPlayer);
	if(chn)
	{
		data << channel_name;
		data << uint8(chn->m_flags);
		data << uint32(chn->GetNumMembers());
		SendPacket(&data);
	}
}

