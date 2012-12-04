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
#define SWIMMING_TOLERANCE_LEVEL -0.08f
#define MOVEMENT_PACKET_TIME_DELAY 500

class Vehicle;

#ifdef WIN32

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#define DELTA_EPOCH_IN_USEC  11644473600000000ULL

uint32 TimeStamp()
{
	//return timeGetTime();

	FILETIME ft;
	uint64 t;
	GetSystemTimeAsFileTime(&ft);

	t = (uint64)ft.dwHighDateTime << 32;
	t |= ft.dwLowDateTime;
	t /= 10;
	t -= DELTA_EPOCH_IN_USEC;

	return uint32(((t / 1000000L) * 1000) + ((t % 1000000L) / 1000));
}

ASCENT_INLINE uint32 mTimeStamp()
{
	return timeGetTime();
}

#else

uint32 TimeStamp()
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
}

ASCENT_INLINE uint32 mTimeStamp()
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
}

#endif

void WorldSession::HandleMoveWorldportAckOpcode( WorldPacket & recv_data )
{
	GetPlayer()->SetPlayerStatus(NONE);
	if(_player->IsInWorld())
	{
		// get outta here
		return;
	}
	sLog.outDebug( "WORLD: got MSG_MOVE_WORLDPORT_ACK." );
	
	if(_player->m_CurrentTransporter && _player->GetMapId() != _player->m_CurrentTransporter->GetMapId())
	{
		/* wow, our pc must really suck. */
		Transporter * pTrans = _player->m_CurrentTransporter;
		/*float c_tposx = pTrans->GetPositionX() + _player->m_TransporterX;
		float c_tposy = pTrans->GetPositionY() + _player->m_TransporterY;
		float c_tposz = pTrans->GetPositionZ() + _player->m_TransporterZ;*/
		
		// Prise en compte de la position du joueur sur le moyen de transport par Randdrick
		float c_tposx = pTrans->GetPositionX() - _player->m_positionX;
		float c_tposy = pTrans->GetPositionY() - _player->m_positionY;
		float c_tposz = pTrans->GetPositionZ() - _player->m_positionZ;

		WorldPacket dataw(SMSG_NEW_WORLD, 20);
		dataw << pTrans->GetMapId() << c_tposx << c_tposy << c_tposz << _player->GetOrientation();
		SendPacket(&dataw);

		_player->SetMapId(_player->m_CurrentTransporter->GetMapId());
		_player->SetPosition(c_tposx, c_tposy, c_tposz, 0, false);
	}
	else
	{
		// don't overwrite the loading flag here.
		// reason: talents/passive spells don't get cast on an invalid instance login
		if( _player->m_TeleportState != 1 )
			_player->m_TeleportState = 2;

		_player->AddToWorld();
	}
}

void WorldSession::HandleMoveTeleportAckOpcode( WorldPacket & recv_data )
{
	WoWGuid GuidWoW;
	recv_data >> GuidWoW;
	uint64 guid = GuidWoW.GetOldGuid();
	if(guid == _player->GetGUID())
	{
		if(sWorld.antihack_teleport && !(HasGMPermissions() && sWorld.no_antihack_on_gm) && _player->GetPlayerStatus() != TRANSFER_PENDING)
		{
			/* we're obviously cheating */
			sCheatLog.writefromsession(this, "Used teleport hack, disconnecting.");
			Disconnect();
			return;
		}

		if(sWorld.antihack_teleport && !(HasGMPermissions() && sWorld.no_antihack_on_gm) && _player->m_position.Distance2DSq(_player->m_sentTeleportPosition) > 625.0f)	/* 25.0f*25.0f */
		{
			/* cheating.... :( */
			sCheatLog.writefromsession(this, "Used teleport hack {2}, disconnecting.");
			Disconnect();
			return;
		}

		sLog.outDebug( "WORLD: got MSG_MOVE_TELEPORT_ACK." );
		GetPlayer()->SetPlayerStatus(NONE);
		GetPlayer()->SetMovement(MOVE_UNROOT,5);
		_player->ResetHeartbeatCoords();

		if(GetPlayer()->GetSummon() != NULL)		// move pet too
			GetPlayer()->GetSummon()->SetPosition((GetPlayer()->GetPositionX() + 2), (GetPlayer()->GetPositionY() + 2), GetPlayer()->GetPositionZ(), float(M_PI));
		m_isFalling = false;
		if(_player->m_sentTeleportPosition.x != 999999.0f)
		{
			_player->m_position = _player->m_sentTeleportPosition;
			_player->m_sentTeleportPosition.ChangeCoords(999999.0f,999999.0f,999999.0f);
		}
	}

}

void _HandleBreathing(MovementInfo &movement_info, Player * _player, WorldSession * pSession)
{

	// no water breathing is required
	if( !sWorld.BreathingEnabled || _player->FlyCheat || _player->m_bUnlimitedBreath || !_player->isAlive() || _player->GodModeCheat )
	{
		// player is flagged as in water
		if( _player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING  )
		{
			_player->m_UnderwaterState &= ~UNDERWATERSTATE_SWIMMING;
			if( _player->FlyCheat )
			{
				if(_player->m_lastMoveType != 2)
				{
					_player->m_lastMoveType = 2; // flying
					_player->ResetHeartbeatCoords();
				}
			}
			else
			{
				if(_player->m_lastMoveType != 1)
				{
					_player->m_lastMoveType = 1; // swimming
					_player->ResetHeartbeatCoords();
					_player->DelaySpeedHack(5000);
				}
			}
		}

		// player is flagged as under water
		if( _player->m_UnderwaterState & UNDERWATERSTATE_UNDERWATER )
		{
			_player->m_UnderwaterState &= ~UNDERWATERSTATE_UNDERWATER;
			WorldPacket data(SMSG_START_MIRROR_TIMER, 20);
			data << uint32(1) << _player->m_UnderwaterTime << _player->m_UnderwaterMaxTime << int32(-1) << uint32(0);
			pSession->SendPacket(&data);
		}

		// player is above water level
		if( pSession->m_bIsWLevelSet )
		{
			if( ( movement_info.z + _player->m_noseLevel ) > pSession->m_wLevel )
			{
				// remove druid aquatic form on land
				if( _player->getClass() == DRUID )
				{
					_player->RemoveAura( __Aquatic_Form_Shapeshift); // 1066 - 335.12340
				}

				// unset swim session water level
				pSession->m_bIsWLevelSet = false;
			}
		}

		return;
	}

	//player is swiming and not flagged as in the water
	if( movement_info.flags & MOVEFLAG_SWIMMING && !( _player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING ) )
	{
		// Changed in 3.1 TODO: dismount for flying mounts only
		// dismount if mounted
		//if( _player->m_MountSpellId )
			// _player->RemoveAura( _player->m_MountSpellId );

		// get water level only if it was not set before
		if( !pSession->m_bIsWLevelSet )
		{
			// water level is somewhere below the nose of the character when entering water
			pSession->m_wLevel = movement_info.z + _player->m_noseLevel * 0.95f;
			pSession->m_bIsWLevelSet = true;
		}

		if( _player->FlyCheat )
		{
			if( _player->m_lastMoveType != 2 )
			{
				_player->m_lastMoveType = 2; // flying
				_player->ResetHeartbeatCoords();
			}
		}
		else
		{
			if( _player->m_lastMoveType != 1 )
			{
				_player->m_lastMoveType = 1; // swimming
				_player->ResetHeartbeatCoords();
			}
		}
		_player->m_UnderwaterState |= UNDERWATERSTATE_SWIMMING;
	}

	// player is not swimming and is not stationary and is flagged as in the water
	if( !( movement_info.flags & MOVEFLAG_SWIMMING ) && !( movement_info.flags & MOVEFLAG_MOVE_STOP ) && _player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING )
	{
		// player is above water level
		if( ( movement_info.z + _player->m_noseLevel ) > pSession->m_wLevel )
		{
			// remove druid aquatic form on land
			if( _player->getClass() == DRUID )
			{
				_player->RemoveAura( __Aquatic_Form_Shapeshift); // 1066 - 335.12340
			}

			// unset swim session water level
			pSession->m_bIsWLevelSet = false;

			if( movement_info.flags & MOVEFLAG_AIR_SWIMMING )
			{
				if( _player->FlyCheat )
				{
					if( _player->m_lastMoveType != 2 )
					{
						_player->m_lastMoveType = 2; // flying
						_player->ResetHeartbeatCoords();
					}
				}
			}
			else
			{
				if( _player->m_lastMoveType != 0 )
				{
					_player->m_lastMoveType = 0; // walk or run on land
					_player->ResetHeartbeatCoords();
				}
			}
			_player->m_UnderwaterState &= ~UNDERWATERSTATE_SWIMMING;
		}
	}

	// player is flagged as in the water and is not flagged as under the water
	if( _player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING && !( _player->m_UnderwaterState & UNDERWATERSTATE_UNDERWATER ) )
	{
		//the player is in the water and has gone under water, requires breath bar.
		if( ( movement_info.z + _player->m_noseLevel ) < pSession->m_wLevel )
		{
			_player->m_UnderwaterState |= UNDERWATERSTATE_UNDERWATER;
			WorldPacket data(SMSG_START_MIRROR_TIMER, 20);
			data << uint32(1) << _player->m_UnderwaterTime << _player->m_UnderwaterMaxTime << int32(-1) << uint32(0);
			pSession->SendPacket(&data);
		}
	}

	// player is flagged as in the water and is flagged as under the water
	if( _player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING && _player->m_UnderwaterState & UNDERWATERSTATE_UNDERWATER )
	{
		//the player is in the water but their face is above water, no breath bar neeeded.
		if( ( movement_info.z + _player->m_noseLevel ) > pSession->m_wLevel )
		{
			_player->m_UnderwaterState &= ~UNDERWATERSTATE_UNDERWATER;
			WorldPacket data(SMSG_START_MIRROR_TIMER, 20);
			data << uint32(1) << _player->m_UnderwaterTime << _player->m_UnderwaterMaxTime << int32(10) << uint32(0);
			pSession->SendPacket(&data);
		}
	}

	// player is flagged as not in the water and is flagged as under the water
	if( !( _player->m_UnderwaterState & UNDERWATERSTATE_SWIMMING ) && _player->m_UnderwaterState & UNDERWATERSTATE_UNDERWATER )
	{
		//the player is out of the water, no breath bar neeeded.
		if( ( movement_info.z + _player->m_noseLevel ) > pSession->m_wLevel )
		{
			_player->m_UnderwaterState &= ~UNDERWATERSTATE_UNDERWATER;
			WorldPacket data(SMSG_START_MIRROR_TIMER, 20);
			data << uint32(1) << _player->m_UnderwaterTime << _player->m_UnderwaterMaxTime << int32(10) << uint32(0);
			pSession->SendPacket(&data);
		}
	}

}
// 0101110000000000 CB9EC200 539F0BC6 357E04C3 9FCBA642 00000000 00000000 MSG_MOVE_START_TURN_LEFT 332
void WorldSession::HandleMovementOpcodes( WorldPacket & recv_data )
{
	if(!_player->IsInWorld() || _player->m_uint32Values[UNIT_FIELD_CHARMEDBY] || 
		(_player->GetPlayerStatus() == TRANSFER_PENDING && _player->m_CurrentVehicle == NULL) || _player->GetTaxiState())
		return;

	// spell cancel on movement, for now only fishing is added
	Object * t_go = _player->m_SummonedObject;
	uint32 mstime = mTimeStamp();
	if (t_go)
	{
		if (t_go->GetEntry() == GO_FISHING_BOBBER)
			((GameObject*)t_go)->EndFishing(GetPlayer(),true);
	}

	/************************************************************************/
	/* Clear standing state to stand.				                        */
	/************************************************************************/
	if( recv_data.GetOpcode() == MSG_MOVE_START_FORWARD )
		_player->SetStandState( STANDSTATE_STAND );

	/************************************************************************/
	/* Make sure the packet is the correct size range.                      */
	/************************************************************************/
	if(recv_data.size() > 80) { Disconnect(); return; }

	/************************************************************************/
	/* Read Movement Data Packet                                            */
	/************************************************************************/
	movement_info.init(recv_data);

	/************************************************************************/
	/* Update player movement state                                         */
	/************************************************************************/
	if( sWorld.antihack_cheatengine && _player->m_lastMovementPacketTimestamp != 0 && (int32)mstime - (int32)_player->m_lastMovementPacketTimestamp != 0)
	{
		int32 server_delta = (int32)mstime - (int32)_player->m_lastMovementPacketTimestamp;
		int32 client_delta = (int32)movement_info.time - (int32)_player->m_lastMoveTime;
		int32 diff = client_delta - server_delta;
		//DEBUG_LOG("WorldSession","HandleMovementOpcodes: server delta=%u, client delta=%u", server_delta, client_delta);
		int32 threshold = int32( World::m_CEThreshold ) + int32( _player->GetSession()->GetLatency() );
		if( diff >= threshold )		// replace with threshold var
		{
			// client cheating with process speedup
			if( _player->m_cheatEngineChances == 1 )
			{
				_player->SetMovement( MOVE_ROOT, 1 );
				_player->BroadcastMessage( "Cheat engine detected. Please contact an admin with the below information if you believe this is a false detection." );
				_player->BroadcastMessage( "You will be disconnected in 10 seconds." );
				_player->BroadcastMessage( MSG_COLOR_WHITE"diff: %d server delta: %u client delta: %u\n", diff, server_delta, client_delta );
				sEventMgr.AddEvent( _player, &Player::_Disconnect, EVENT_PLAYER_KICK, 10000, 1, 0 );
				_player->m_cheatEngineChances = 0;
				sCheatLog.writefromsession(this, "Cheat Engine detected. Diff: %d, Server Delta: %u, Client Delta: %u", diff, server_delta, client_delta );
			}
			else if (_player->m_cheatEngineChances > 0 )
				_player->m_cheatEngineChances--;
		}
	}
/*
	if( recv_data.GetOpcode() == MSG_MOVE_STOP 
		|| recv_data.GetOpcode() == MSG_MOVE_STOP_STRAFE 
		|| recv_data.GetOpcode() == MSG_MOVE_STOP_TURN 
		|| recv_data.GetOpcode() == MSG_MOVE_FALL_LAND
		|| ( recv_data.GetOpcode() == MSG_MOVE_SET_FACING && !(movement_info.flags & MOVEFLAG_MOVING_MASK) ) )
	{
		if( _player->m_isMoving )
		{
#ifdef _DEBUG
//			printf("MOVING: FALSE (Packet %s)\n", LookupName( recv_data.GetOpcode(), g_worldOpcodeNames ) );
#endif
			mstime = getMSTime();
			_player->_SpeedhackCheck(mstime);
			_player->m_isMoving = false;
			_player->_startMoveTime = 0;
		}
	}
	else
	{
		if( !_player->m_isMoving )
		{
#ifdef _DEBUG
//			printf("MOVING: TRUE (Packet %s)\n", LookupName( recv_data.GetOpcode(), g_worldOpcodeNames ) );
#endif
			mstime = getMSTime();
			_player->m_isMoving = true;
			_player->_startMoveTime = mstime;
			_player->_lastHeartbeatPosition = _player->GetPosition();
		}
	}
*/
	_player->m_lastMovementPacketTimestamp = mstime;
	_player->m_lastMoveTime = movement_info.time;


	/************************************************************************/
	/* Remove Emote State                                                   */
	/************************************************************************/
	if(_player->m_uint32Values[UNIT_NPC_EMOTESTATE])
		_player->SetUInt32Value(UNIT_NPC_EMOTESTATE,0);

	/************************************************************************/
	/* Make sure the co-ordinates are valid.                                */
	/************************************************************************/
	if( !((movement_info.y >= _minY) && (movement_info.y <= _maxY)) || !((movement_info.x >= _minX) && (movement_info.x <= _maxX)) )
	{
		Disconnect();
		return;
	}

	/************************************************************************/
	/* Dump movement flags - Wheee!                                         */
	/************************************************************************/
#if 0
	printf("=========================================================\n");
	printf("Full movement flags: 0x%.8X\n", movement_info.flags);
	uint32 z, b;
	for(z = 1, b = 1; b < 32;)
	{
		if(movement_info.flags & z)
			printf("   Bit %u (0x%.8X or %u) is set!\n", b, z, z);

		z <<= 1;
		b+=1;
	}
	printf("=========================================================\n");
#endif

	/************************************************************************/
	/* Orientation dumping                                                  */
	/************************************************************************/
#if 0
	printf("Packet: 0x%03X (%s)\n", recv_data.GetOpcode(), LookupName( recv_data.GetOpcode(), g_worldOpcodeNames ) );
	printf("Orientation: %.10f\n", movement_info.orientation);
#endif

	/************************************************************************/
	/* Anti-Hack Checks                                                     */
	/************************************************************************/
	if( !(HasGMPermissions() && sWorld.no_antihack_on_gm) && !_player->m_uint32Values[UNIT_FIELD_CHARM] && !_player->_heartbeatDisable)
	{
		/************************************************************************/
		/* Anti-Teleport                                                        */
		/************************************************************************/
		if(sWorld.antihack_teleport && _player->m_position.Distance2DSq(movement_info.x, movement_info.y) > 5625.0f
			&& _player->m_runSpeed < 50.0f && !_player->m_TransporterGUID)
		{
			sCheatLog.writefromsession(this, "Used teleport hack {3}, speed was %f", _player->m_runSpeed);
			Disconnect();
			return;
		}
	}

	//Water walk hack
	if (movement_info.flags & MOVEFLAG_WATER_WALK && !GetPlayer()->m_isWaterWalking)
	{
		/* Need to check if aura has just been dispelled recently and client didn't have time to update
		sCheatLog.writefromsession(this, "Used water walk hack");
		Disconnect();*/
	}

	/************************************************************************/
	/* Calculate the timestamp of the packet we have to send out            */
	/************************************************************************/
	size_t pos = (size_t)m_MoverWoWGuid.GetNewGuidLen() + 1;
	//uint32 mstime = mTimeStamp();
	int32 move_time;
	if(m_clientTimeDelay == 0)
		m_clientTimeDelay = mstime - movement_info.time;

	/************************************************************************/
	/* Copy into the output buffer.                                         */
	/************************************************************************/
	if(_player->m_inRangePlayers.size())
	{
		move_time = (movement_info.time - (mstime - m_clientTimeDelay)) + MOVEMENT_PACKET_TIME_DELAY + mstime;
		memcpy(&movement_packet[0], recv_data.contents(), recv_data.size());
		movement_packet[pos+6]= 0;

		/************************************************************************/
		/* Distribute to all inrange players.                                   */
		/************************************************************************/
		for(set<Player*>::iterator itr = _player->m_inRangePlayers.begin(); itr != _player->m_inRangePlayers.end(); ++itr)
		{
			if( (*itr)->GetSession() && (*itr)->IsInWorld() )
			{
				*(uint32*)&movement_packet[pos+6] = uint32(move_time + (*itr)->GetSession()->m_moveDelayTime);

#if defined(ENABLE_COMPRESSED_MOVEMENT) && defined(ENABLE_COMPRESSED_MOVEMENT_FOR_PLAYERS)
				if( _player->GetPositionNC().Distance2DSq((*itr)->GetPosition()) >= World::m_movementCompressThreshold )
					(*itr)->AppendMovementData( recv_data.GetOpcode(), uint16(recv_data.size() + pos), movement_packet );
				else
					(*itr)->GetSession()->OutPacket(recv_data.GetOpcode(), uint16(recv_data.size() + pos), movement_packet);
#else
				(*itr)->GetSession()->OutPacket(recv_data.GetOpcode(), uint16(recv_data.size() + pos), movement_packet);
#endif
			}
		}
	}
	/************************************************************************/
	/* Falling damage checks                                                */
	/************************************************************************/

		if( _player->blinked )
	{
		_player->blinked = false;
		_player->m_fallDisabledUntil = UNIXTIME + 5;
		_player->DelaySpeedHack( 5000 );
	}
	else
	{
		if (movement_info.flags & MOVEFLAG_FALLING)
			m_isFalling = true;
/*
         // si le player saute, fatalement il tombe (meme apres un petit bon ;)
		if (recv_data.GetOpcode() == MSG_MOVE_JUMP)
		{
			if (m_isFalling)
				Disconnect();  // <========= C'est quoi ca!!!
			m_isFalling = true;
		}
*/
		if( recv_data.GetOpcode() == MSG_MOVE_FALL_LAND )
		{
			// player has finished falling
			//if _player->z_axisposition contains no data then set to current position
			if( !_player->z_axisposition )
				_player->z_axisposition = movement_info.z;

			// calculate distance fallen
			int32 falldistance = float2int32( _player->z_axisposition - movement_info.z );
			if(movement_info.z > _player->z_axisposition)
				falldistance = 0;

			/*if player is a rogue or druid(in cat form), then apply -17 modifier to fall distance.
			these checks need improving, low level rogue/druid should not receive this benefit*/
			if( ( _player->getClass() == ROGUE ) || ( _player->GetShapeShift() == FORM_CAT ) )
				falldistance -=17;

			//checks that player has fallen more than 12 units, otherwise no damage will be dealt
			//falltime check is also needed here, otherwise sudden changes in Z axis position, such as using !recall, may result in death
			if( _player->isAlive() && !_player->bInvincible && !_player->GodModeCheat && (falldistance > 12) && ( UNIXTIME >= _player->m_fallDisabledUntil ) /*&& movement_info.FallTime > 1000*/ && !_player->m_noFallDamage )
			{
				// 1.7% damage for each unit fallen on Z axis over 13
				uint32 health_loss = float2int32( float( _player->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) * ( ( falldistance - 12 ) * 0.017 ) ) );

				if( health_loss >= _player->GetUInt32Value( UNIT_FIELD_HEALTH ) )
					health_loss = _player->GetUInt32Value( UNIT_FIELD_HEALTH );

				_player->SendEnvironmentalDamageLog( _player->GetGUID(), DAMAGE_FALL, health_loss );
				_player->DealDamage( _player, health_loss, 0, 0, 0 );

				//_player->RemoveStealth(); // cebernic : why again? lost stealth by AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN already.
			}
			_player->z_axisposition = 0.0f;
		}
		else
			//whilst player is not falling, continuously update Z axis position.
			//once player lands this will be used to determine how far he fell.
			if( !( movement_info.flags & MOVEFLAG_FALLING ) )
				_player->z_axisposition = movement_info.z;
	}

	/************************************************************************/
	/* Transporter Setup                                                    */
	/************************************************************************/
	if(!_player->m_lockTransportVariables)
	{
		if(_player->m_TransporterGUID && !movement_info.transGuid)
		{
			/* we left the transporter we were on */
			if(_player->m_CurrentTransporter)
			{
				_player->m_CurrentTransporter->RemovePlayer(_player);
				_player->m_CurrentTransporter = NULL;
			}

			_player->m_TransporterGUID = 0;
			_player->ResetHeartbeatCoords();
			_player->DelaySpeedHack(5000);
		}
		else if(movement_info.transGuid)
		{
			if( _player->m_CurrentVehicle &&
				_player->m_CurrentVehicle->GetGUID() == movement_info.transGuid )
				_player->m_sentTeleportPosition.ChangeCoords(movement_info.x, movement_info.y, movement_info.z);
			else if(!_player->m_TransporterGUID)
			{
				/* just walked into a transport */
				if(_player->IsMounted())
					_player->RemoveAura(_player->m_MountSpellId);


				// vehicles, meh (Hearstone)
				if( _player->m_CurrentVehicle )
					_player->m_CurrentVehicle->RemovePassenger( _player );

				_player->m_CurrentTransporter = objmgr.GetTransporter(GUID_LOPART(movement_info.transGuid));
				if(_player->m_CurrentTransporter)
				{
					_player->m_CurrentTransporter->AddPlayer(_player);
                    Log.Warning("MouvementHandler","Player est a bord de %u",_player->m_CurrentTransporter->GetEntry());
				}

				/* set variables */
				_player->m_TransporterGUID = movement_info.transGuid; 
				_player->m_TransporterUnk = movement_info.transUnk;
				_player->m_TransporterX = movement_info.transX;
				_player->m_TransporterY = movement_info.transY;
				_player->m_TransporterZ = movement_info.transZ;
				_player->DelaySpeedHack(5000);
			}
			else
			{
				/* no changes */
				_player->m_TransporterUnk = movement_info.transUnk;
				_player->m_TransporterX = movement_info.transX;
				_player->m_TransporterY = movement_info.transY;
				_player->m_TransporterZ = movement_info.transZ;

			}
		}
		// Calclul de la position du joueur par rapport au moyen de transport par Randdrick
		// Modifi� par Branruz ;)
		 _player->m_positionX = movement_info.x - movement_info.transX;
		 _player->m_positionY = movement_info.y - movement_info.transY;
		 _player->m_positionZ = movement_info.z - movement_info.transZ;
		 Transporter* trans = _player->m_CurrentTransporter;
		 if(trans!=NULL)
		 { 
			 if(trans->GetInfo()->ID == sWorld.m_DebugTransporters)
		  {
		   sChatHandler.SystemMessageToPlr(_player, "Client pos: %.3f %.3f\nServer pos: %.3f %.3f\nDiff: %.3f %.3f", 
		    _player->m_positionX,_player->m_positionY, trans->GetPositionX(), trans->GetPositionY(), 
			trans->CalcDistance(_player->m_positionX,_player->m_positionY,_player->m_positionZ), trans->CalcDistance(movement_info.x, movement_info.y, movement_info.z));
		  }
		 }
	}

	/************************************************************************/
	/* Anti-Speed Hack Checks                                               */
	/************************************************************************/

	

	/************************************************************************/
	/* Breathing System                                                     */
	/************************************************************************/
	_HandleBreathing(movement_info, _player, this);

	/************************************************************************/
	/* Remove Spells                                                        */
	/************************************************************************/
	uint32 flags = AURA_INTERRUPT_ON_MOVEMENT;
	if( !( movement_info.flags & MOVEFLAG_SWIMMING || movement_info.flags & MOVEFLAG_FALLING ) )
		flags |= AURA_INTERRUPT_ON_LEAVE_WATER;
	/*if( movement_info.flags & MOVEFLAG_SWIMMING )
		flags |= AURA_INTERRUPT_ON_ENTER_WATER;
	if( movement_info.flags & ( MOVEFLAG_TURN_LEFT | MOVEFLAG_TURN_RIGHT ) )
		flags |= AURA_INTERRUPT_ON_TURNING;*/
	
	_player->RemoveAurasByInterruptFlag( flags );

	/************************************************************************/
	/* Update our position in the server.                                   */
	/************************************************************************/
	if( _player->m_CurrentCharm )
		_player->m_CurrentCharm->SetPosition(movement_info.x, movement_info.y, movement_info.z, movement_info.orientation);
	else if ( _player->m_CurrentVehicle )
		_player->m_CurrentVehicle->MoveVehicle(movement_info.x, movement_info.y, movement_info.z, movement_info.orientation);
	else
	{
		if(!_player->m_CurrentTransporter) 
		{
			if( !_player->SetPosition(movement_info.x, movement_info.y, movement_info.z, movement_info.orientation) )
			{
				// Pourquoi faire ca ? (Branruz)
				//_player->SetUInt32Value(UNIT_FIELD_HEALTH, 0);
				//_player->KillPlayer();
			}
		}
		else
		{
			_player->SetPosition(movement_info.x, movement_info.y, movement_info.z, 
				movement_info.orientation + movement_info.transO, false);
		}
	}	

	if(  !(movement_info.flags & MOVEFLAG_MOTION_MASK) )
	{
		if( _player->m_isMoving )
		{
			//printf("MOVING: FALSE (Packet %s)\n", LookupName( recv_data.GetOpcode(), g_worldOpcodeNames ) );
			mstime = getMSTime();
			_player->_SpeedhackCheck(mstime);
			_player->m_isMoving = false;
			_player->_startMoveTime = 0;
		}
	}
	else
	{
		if( !_player->m_isMoving )
		{
			//printf("MOVING: TRUE (Packet %s)\n", LookupName( recv_data.GetOpcode(), g_worldOpcodeNames ) );
			_player->m_isMoving = true;
			_player->_startMoveTime = movement_info.time;
			_player->_lastHeartbeatPosition.ChangeCoords(movement_info.x, movement_info.y, movement_info.z);
		}
	}

	// reset the period every 5 seconds, for a little more accuracy
	if( _player->m_isMoving && (_player->_lastMoveTime - _player->_startMoveTime) >= 5000 )
	{
		_player->_lastHeartbeatPosition.ChangeCoords(movement_info.x, movement_info.y, movement_info.z);
		_player->_startMoveTime = _player->_lastMoveTime;
		_player->m_cheatEngineChances = 2;
	}


}

void WorldSession::HandleMoveTimeSkippedOpcode( WorldPacket & recv_data )
{
 /* Debug des trames de synchro (Branruz)
 {CLIENT} Packet: (0x02CE) CMSG_MOVE_TIME_SKIPPED PacketSize = 12
|------------------------------------------------|----------------|
|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
|------------------------------------------------|----------------|
|01 00 00 00 00 00 00 00 12 00 00 00             |............    |
-------------------------------------------------------------------
 */
/* // A quoi sert cette trame, le client r�itere, donc le serveur devrait renvoyer un truc
 uint64 _PlayerGuid; // ??
 uint32 _Unk32;

 recv_data >> _PlayerGuid;
 recv_data >> _Unk32;

 if(_player->GetGUID() == _PlayerGuid)
     Log.Notice("[CMSG_MOVE_TIME_SKIPPED]","PlayerGuid: %u , Unk: %u",_PlayerGuid,_Unk32);
 else
	 Log.Notice("[CMSG_MOVE_TIME_SKIPPED]","PlayerGuid %u, Cheater PlayerGuid: %u , Unk: %u",_player->GetGUID(),_PlayerGuid,_Unk32);

 ----
 */	
 	WoWGuid guid;
	uint32 time_dif;
	uint8 buf[16];
	StackPacket data(MSG_MOVE_TIME_SKIPPED, buf, 16); //0x319

	recv_data >> guid;
	recv_data >> time_dif;

	// ignore updates for not us
	if( guid.GetOldGuid() != _player->GetGUID() )
		return;

	// send to other players
	data << _player->GetNewGUID();
	data << time_dif;
	_player->SendMessageToSet(&data, false);
}

void WorldSession::HandleMoveNotActiveMoverOpcode( WorldPacket & recv_data )
{

}


void WorldSession::HandleSetActiveMoverOpcode( WorldPacket & recv_data )
{
	// set current movement object
	uint64 guid;
	recv_data >> guid;

	if(guid != m_MoverWoWGuid.GetOldGuid())
	{
		// make sure the guid is valid and we aren't cheating
		if( !(_player->m_CurrentCharm && _player->m_CurrentCharm->GetGUID() == guid) &&
			!(_player->GetGUID() == guid) )
		{
			// cheater!
			return;
		}

		// generate wowguid
		if(guid != 0)
			m_MoverWoWGuid.Init(guid);
		else
			m_MoverWoWGuid.Init(_player->GetGUID());

		// set up to the movement packet
		movement_packet[0] = m_MoverWoWGuid.GetNewGuidMask();
		memcpy(&movement_packet[1], m_MoverWoWGuid.GetNewGuid(), m_MoverWoWGuid.GetNewGuidLen());
	}
}

void WorldSession::HandleMoveSplineCompleteOpcode(WorldPacket &recvPacket)
{

}

void WorldSession::HandleMountSpecialAnimOpcode(WorldPacket &recvdata)
{
	WorldPacket data(SMSG_MOUNTSPECIAL_ANIM,8);
	data << _player->GetGUID();
	_player->SendMessageToSet(&data, true);
}

void WorldSession::HandleWorldportOpcode(WorldPacket & recv_data)
{
	uint32 mapid;
	float x,y,z,o;
	recv_data >> mapid >> x >> y >> z >> o;
	
	CHECK_INWORLD_RETURN;

	if(!HasGMPermissions())
	{
		SendNotification("You do not have permission to use this function.");
		return;
	}

	LocationVector vec(x,y,z,o);
	_player->SafeTeleport(mapid,0,vec);
}

void WorldSession::HandleTeleportToUnitOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;

	uint8 unk;
	Unit * target;
	recv_data >> unk;

	if(!HasGMPermissions())
	{
		SendNotification("You do not have permission to use this function.");
		return;
	}

	if( (target = _player->GetMapMgr()->GetUnit(_player->GetSelection())) == NULL )
		return;

	_player->SafeTeleport(_player->GetMapId(), _player->GetInstanceID(), target->GetPosition());
}

void WorldSession::HandleTeleportCheatOpcode(WorldPacket & recv_data)
{
	float x,y,z,o;
	LocationVector vec;

	if(!HasGMPermissions())
	{
		SendNotification("You do not have permission to use this function.");
		return;
	}

	recv_data >> x >> y >> z >> o;
	vec.ChangeCoords(x,y,z,o);
	_player->SafeTeleport(_player->GetMapId(),_player->GetInstanceID(),vec);
}

void WorldSession::HandleMoveFallResetOpcode(WorldPacket & recvPacket)
{
	_player->z_axisposition = 0.0f;
}
//     Guid            Unk       X        Y        Z        O
// 0101000000000000 64DCAD00 331D2146 A21D5044 0BCDA544 00000000 10000000 
void MovementInfo::init(WorldPacket & data)
{
	uint16 wOpcode;
	uint32 wSize;

	WoWGuid TransportWoWGuid,PlayerWoWGUID;
	unk13 = 0;
	data >> PlayerWoWGUID;
	//PlayerGUID = PlayerWoWGUID.GetOldGuid();
	data >> flags >> unk_230 >> time;
	data >> x >> y >> z >> orientation;

	//Log.Warning("MSG_MOVE"," 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X",unk,x,y,z,orientation);

	wOpcode = data.GetOpcode();
	wSize   = (uint32)data.size();
	

	if (flags & MOVEFLAG_TAXI)
	{                                                             //          time         seat
		data >> TransportWoWGuid >> transX >> transY >> transZ >> transO >> transUnk >> transUnk2;
		transGuid = TransportWoWGuid.GetOldGuid();
	}
	if (flags & (MOVEFLAG_SWIMMING | MOVEFLAG_AIR_SWIMMING) || unk_230 & 0x20)
	{
		data >> pitch; 
	}

	data >> unklast;

	if (flags & MOVEFLAG_FALLING || flags & MOVEFLAG_REDIRECTED)
	{   // data >> FallTime >> jump_sinAngle >> jump_cosAngle >> jump_xySpeed;  
		data >> FallTime >> unk8 >> unk9 >> unk10;
	}
	if (flags & MOVEFLAG_SPLINE_MOVER)
	{
		data >> unk12;
	}

	if(data.rpos() != data.wpos())
	{
		if(data.rpos() + 4 == data.wpos())
			data >> unk13;
		else
		{
			Log.Warning("MovementInfo","Extra bits of movement packet left");
			Log.Warning("            ","Init : wOpcode %u, wSize %u",wOpcode,wSize);
		}
	}
}

void MovementInfo::write(WorldPacket & data)
{
	data << flags << unk_230 << getMSTime();

	data << x << y << z << orientation;

	if (flags & MOVEFLAG_TAXI)
	{
		data << transGuid << transX << transY << transZ << transO << transUnk << transUnk2;
	}
	if (flags & (MOVEFLAG_SWIMMING | MOVEFLAG_AIR_SWIMMING) || unk_230 & 0x20)
	{
		data << pitch;
	}

	data << unklast;

	if (flags & MOVEFLAG_FALLING || flags & MOVEFLAG_REDIRECTED)
	{
		data << FallTime << unk8 << unk9 << unk10;
	}
	if (flags & MOVEFLAG_SPLINE_MOVER)
	{
		data << unk12;
	}
	
	if(unk13)
		data << unk13;
}
