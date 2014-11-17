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

void WorldSession::HandleTaxiNodeStatusQueryOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	sLog.outDebug( "WORLD: Received CMSG_TAXINODE_STATUS_QUERY" );

	uint64 guid;
	uint32 curloc;
	uint8 field;
	uint32 submask;

	recv_data >> guid;

	curloc = sTaxiMgr.GetNearestTaxiNode( GetPlayer( )->GetPositionX( ), GetPlayer( )->GetPositionY( ), GetPlayer( )->GetPositionZ( ), GetPlayer( )->GetMapId( ) );

	field = (uint8)((curloc - 1) / 32);
	submask = 1<<((curloc-1)%32);

	WorldPacket data(9);
	data.Initialize( SMSG_TAXINODE_STATUS );
	data << guid;

	// Check for known nodes
	if ( (GetPlayer( )->GetTaximask(field) & submask) != submask )
	{   
		data << uint8( 0 );	
	}
	else
	{
		data << uint8( 1 );
	}	

	SendPacket( &data );
	sLog.outDebug( "WORLD: Sent SMSG_TAXINODE_STATUS" );
}


void WorldSession::HandleTaxiQueryAvaibleNodesOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	sLog.outDebug( "WORLD: Received CMSG_TAXIQUERYAVAILABLENODES" );
	uint64 guid;
	recv_data >> guid;
	Creature *pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature) return;

	SendTaxiList(pCreature);
}

void WorldSession::SendTaxiList(Creature* pCreature)
{
	uint32 curloc;
	uint8 field;
	uint32 TaxiMask[NUM_TAXI_FIELDS];
	uint32 submask;
	uint64 guid = pCreature->GetGUID();

	curloc = pCreature->m_TaxiNode;
	if (!curloc)
	{
		//Log.Warning("","Pas de TaxiNode de definie");
		return;
	}

	//Log.Warning("","TaxiNode = %u",curloc);

	field = (uint8)((curloc - 1) / 32);
	submask = 1<<((curloc-1)%32);

	// Check for known nodes
	if (!(GetPlayer( )->GetTaximask(field) & submask))
	{
		GetPlayer()->SetTaximask(field, (submask | GetPlayer( )->GetTaximask(field)) );

		OutPacket(SMSG_NEW_TAXI_PATH);

		//Send packet
		WorldPacket update(SMSG_TAXINODE_STATUS, 9);
		update << guid << uint8( 1 );
		SendPacket( &update );
	}

	//Set Mask
	for(uint8 i = 0; i < 12; i++)
	{
		TaxiMask[i] = 0;
	}
	sTaxiMgr.GetGlobalTaxiNodeMask(curloc, TaxiMask);
	TaxiMask[field] |= 1 << ((curloc-1)%32);

	//Remove nodes unknown to player
	for(uint8 i = 0; i < 12; i++)
	{
		TaxiMask[i] &= GetPlayer( )->GetTaximask(i);
	}

	WorldPacket data(48);
	data.Initialize( SMSG_SHOWTAXINODES );
	data << uint32( 1 ) << guid;
	data << uint32( curloc );
	for(int i = 0; i < 12; i++)
	{
		data << TaxiMask[i];
	}
	SendPacket( &data );

	sLog.outDebug( "WORLD: Sent SMSG_SHOWTAXINODES" );
}
// CMSG_ACTIVATETAXI: Reponse a SMSG_SHOWTAXINODES
void WorldSession::HandleActivateTaxiOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;

	uint8 ss = _player->GetShapeShift();
	if( ss && (ss != FORM_BATTLESTANCE) && (ss != FORM_BERSERKERSTANCE) && (ss != FORM_DEFENSIVESTANCE) && (ss != FORM_STEALTH))
	{
		WorldPacket data(SMSG_ACTIVATETAXIREPLY, 4);
		data << uint32(9); //shape shifted
		SendPacket(&data);
		return;
	}

	sLog.outDebug( "WORLD: Received CMSG_ACTIVATETAXI" );

	uint64 guid;
	uint32 sourcenode, destinationnode;
	int32 newmoney;
	uint32 curloc;
	uint8 field;
	uint32 submask;
	uint32 MountId;

	WorldPacket data(SMSG_ACTIVATETAXIREPLY, 4);

	recv_data >> guid >> sourcenode >> destinationnode;

	if(GetPlayer()->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER))
		return;

	TaxiPath* taxipath = sTaxiMgr.GetTaxiPath(sourcenode, destinationnode);
	TaxiNode* taxinode = sTaxiMgr.GetTaxiNode(sourcenode);

	//Log.Warning("TaxiHandle","sourcenode: %u, destinationnode: %u",sourcenode, destinationnode);
	//Log.Warning("TaxiHandle","taxipath Id %u",taxipath->GetID());
	//Log.Warning("TaxiHandle","taxinode Id %u",taxinode->id);

	if( !taxinode || !taxipath )
	{
		if(!taxinode ) Log.Error("HandleActivateTaxiOpcode","taxinode n'existe pas!");
		else           Log.Error("                        ","taxinode : %u",taxinode->id);
        if(!taxipath ) Log.Error("HandleActivateTaxiOpcode","taxipath n'existe pas!");
		else           Log.Error("                        ","taxipath : %u",taxipath->GetID());
		return;
	}

	curloc = taxinode->id;

	field = (uint8)((curloc - 1) / 32);
	submask = 1<<((curloc-1)%32);

	// Check for known nodes
	if ( (GetPlayer( )->GetTaximask(field) & submask) != submask )
	{   
		data << uint32( 1 );
		SendPacket( &data );
		return;
	}

	// Check for valid node
	if (!taxinode)
	{
		data << uint32( 1 );
		SendPacket( &data );
		return;
	}

	if (!taxipath || !taxipath->GetNodeCount())
	{
		data << uint32( 2 );
		SendPacket( &data );
		return;
	}

	// Check for gold
	newmoney = ((GetPlayer()->GetUInt32Value(PLAYER_FIELD_COINAGE)) - taxipath->GetPrice());
	if(newmoney < 0 )
	{
		data << uint32( 3 );
		SendPacket( &data );
		return;
	}

	// MOUNTDISPLAYID
	// bat: 1566
	// gryph: 1147
	// wyvern: 295
	// hippogryph: 479
	// Griffon du Fléau: 26308

	uint32 modelid =0; 
	if( _player->GetTeam()== FACTION_HORDE ) MountId = taxinode->horde_mount;
	else                                     MountId = taxinode->alliance_mount; 
         
	modelid = GetCreatureModelId(MountId,_player->GetTeam());
	/*
	if( _player->GetTeam() )
	{
		if( taxinode->horde_mount == 2224 )
			modelid =295; // In case it's a wyvern
		else
			modelid =1566; // In case it's a bat or a bad id
	}
	else
	{
		if( taxinode->alliance_mount == 3837 )
			modelid =479; // In case it's an hippogryph
		else
			modelid =1147; // In case it's a gryphon or a bad id
	}*/

	//GetPlayer( )->setDismountCost( newmoney );

	data << uint32( 0 );
	// 0 Ok
	// 1 Unspecified Server Taxi Error
	// 2.There is no direct path to that direction
	// 3 Not enough Money
	SendPacket( &data );
	sLog.outDebug( "WORLD: Sent SMSG_ACTIVATETAXIREPLY" );

	// 0x001000 seems to make a mount visible
	// 0x002000 seems to make you sit on the mount, and the mount move with you
	// 0x000004 locks you so you can't move, no msg_move updates are sent to the server
	// 0x000008 seems to enable detailed collision checking

	// check for a summon -> if we do, remove.
	if(_player->GetSummon() != NULL)
	{
		_player->GetSummon()->Dismiss(false);
		/*if(_player->GetSummon()->GetUInt32Value(UNIT_CREATED_BY_SPELL) > 0)
			_player->GetSummon()->Dismiss(false);						   // warlock summon -> dismiss
		else
			_player->GetSummon()->Remove(false, true, true);					  // hunter pet -> just remove for later re-call*/
	}

	_player->taxi_model_id = modelid;
	if (_player->IsStealth()) _player->RemoveAura(_player->m_stealth); //check for stealth flight
	GetPlayer()->TaxiStart(taxipath, modelid, 0);
	GetPlayer()->GetAchievementInterface()->HandleAchievementCriteriaFlightPathsTaken();
	
	//sLog.outString("TAXI: Starting taxi trip. Next update in %d msec.", first_node_time);
}

void WorldSession::HandleMultipleActivateTaxiOpcode(WorldPacket & recvPacket)
{
	if(!_player->IsInWorld()) return;
	sLog.outDebug( "WORLD: Received CMSG_ACTIVATETAXI" );

	uint64 guid;
	uint32 moocost;
	uint32 nodecount;
	vector<uint32> pathes;
	int32 newmoney;
	uint32 curloc;
	uint8 field;
	uint32 submask;
	uint32 MountId;

	WorldPacket data(SMSG_ACTIVATETAXIREPLY, 4);

	recvPacket >> guid >> moocost >> nodecount;
	if(nodecount < 2)
		return;

	if(nodecount>12)
	{
		Log.Error("HandleMultipleActivateTaxiOpcode","NodeCount > 10 (%u)",nodecount);
		Disconnect();
		return;
	}

	for(uint32 i = 0; i < nodecount; ++i)
		pathes.push_back( recvPacket.read<uint32>() );

	if(GetPlayer()->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER))
		return;

	// get first trip
	TaxiPath* taxipath = sTaxiMgr.GetTaxiPath(pathes[0], pathes[1]);
	TaxiNode* taxinode = sTaxiMgr.GetTaxiNode(pathes[0]);

	if( !taxinode || !taxipath )
	{
		if(!taxinode ) Log.Error("HandleMultipleActivateTaxiOpcode","taxinode n'existe pas!");
		else           Log.Error("                                ","taxinode : %u",taxinode->id);
        if(!taxipath ) Log.Error("HandleMultipleActivateTaxiOpcode","taxipath n'existe pas!");
		else           Log.Error("                                ","taxipath : %u",taxipath->GetID());
		return;
	}

	curloc = taxinode->id;
	field = (uint8)((curloc - 1) / 32);
	submask = 1<<((curloc-1)%32);

	// Check for known nodes
	if ( (GetPlayer( )->GetTaximask(field) & submask) != submask )
	{   
		data << uint32( 1 );
		SendPacket( &data );
		return;
	}

	// Check for valid node
	if (!taxinode)
	{
		data << uint32( 1 );
		SendPacket( &data );
		return;
	}

	if (!taxipath || !taxipath->GetNodeCount())
	{
		data << uint32( 2 );
		SendPacket( &data );
		return;
	}

	if (taxipath->GetID() == 766 || taxipath->GetID() == 767 || taxipath->GetID() == 771 || taxipath->GetID() == 772)
	{
		data << uint32( 2 );
		SendPacket( &data );
		return;
	}

	uint32 totalcost = taxipath->GetPrice();
	for(uint32 i = 2; i < nodecount; ++i)
	{
		TaxiPath * np = sTaxiMgr.GetTaxiPath(pathes[i-1], pathes[i]);
		if(!np) return;
		totalcost += np->GetPrice();
	}

	// Check for gold
	newmoney = ((GetPlayer()->GetUInt32Value(PLAYER_FIELD_COINAGE)) - totalcost);
	if(newmoney < 0 )
	{
		data << uint32( 3 );
		SendPacket( &data );
		return;
	}

	// MOUNTDISPLAYID
	// bat: 1566
	// gryph: 1147
	// wyvern: 295
	// hippogryph: 479

	uint32 modelid =0;
	if( _player->GetTeam()== FACTION_HORDE ) MountId = taxinode->horde_mount;
	else                                     MountId = taxinode->alliance_mount; 
         
	modelid = GetCreatureModelId(MountId,_player->GetTeam());
	/*
	 if( _player->GetTeam() )
	 {
		 switch(taxinode->horde_mount) // pnj entry
		 {
		  case 2224  : modelid =   295; break; // In case it's a wyvern
		  case 29488 :                         // Griffon du fleau 
	      case 29501 : modelid = 26308; break; // Griffon du fleau 
		  default    : modelid =  1566; break; // In case it's a bat or a bad id
		 }
	 }
	 else
	 {
         switch(taxinode->alliance_mount) // pnj entry
		 {
		  case 3837  : modelid =   479; break; // Hippogryph
		  case 29488 :                         // Griffon du fleau 
		  case 29501 : modelid = 26308; break; // Griffon du fleau 
		  default    : modelid =  1147; break; // Gryphon
		 } 
	 }*/

	//GetPlayer( )->setDismountCost( newmoney );

	data << uint32( 0 );
	// 0 Ok
	// 1 Unspecified Server Taxi Error
	// 2.There is no direct path to that direction
	// 3 Not enough Money
	SendPacket( &data );
	sLog.outDebug( "WORLD: Sent SMSG_ACTIVATETAXIREPLY" );

	// 0x001000 seems to make a mount visible
	// 0x002000 seems to make you sit on the mount, and the mount move with you
	// 0x000004 locks you so you can't move, no msg_move updates are sent to the server
	// 0x000008 seems to enable detailed collision checking

	// check for a summon -> if we do, remove.
	if(_player->GetSummon() != NULL)
	{
		_player->GetSummon()->Dismiss(false);
		/*if(_player->GetSummon()->GetUInt32Value(UNIT_CREATED_BY_SPELL) > 0)
			_player->GetSummon()->Dismiss(false);						   // warlock summon -> dismiss
		else
			_player->GetSummon()->Remove(false, true, true);					  // hunter pet -> just remove for later re-call*/
	}

	_player->taxi_model_id = modelid;
	
	// build the rest of the path list
	for(uint32 i = 2; i < nodecount; ++i)
	{
		TaxiPath * np = sTaxiMgr.GetTaxiPath(pathes[i-1], pathes[i]);
		if(!np) return;

/*		if (np->GetID() == 766 || np->GetID() == 767 || np->GetID() == 771 || np->GetID() == 772)
		{
			_player->m_taxiPaths.clear();
			return;
		}
*/
		// add to the list.. :)
		_player->m_taxiPaths.push_back(np);
	}

	// start the first trip :)
	GetPlayer()->TaxiStart(taxipath, modelid, 0);
}

// Gestion des Taxis en clickant directement sur le griffon
void WorldSession::ActivateDirectTaxi(Creature* pMonture)
{
	// TaxiNodes.dbc    => Le NodeId donne la description du point de depart
	//                     Les coordonnées servent a l'assigné au pnj le plus proche  
	//                     Le NpcFlag devra etre soit TAXI_VENDOR soit DIRECT_TAXI ( et avec le GOSSIP_HELLO )
	// TaxiPath.dbc     => PathId du chemin IdTaxiNode1 vers IdTaxiNode2
	// TaxiPathNote.dbc => PathId du parcours exact du griff (coordonnées x y z de deplacement pt a pt)
	
	int32  newmoney;
	uint32 curloc;
	uint8  field;
	uint32 submask;
    uint32 sourcenode, destinationnode; 
	uint32 MountId;
	
	if(!_player->IsInWorld()) return;

	sourcenode = 0;
	destinationnode = 0;

	// Recup des Nodes du parcours
	// Note: Les griffons du fort d'ebene ont une gestion bizarre (entre les transports et les taxis)
	//       Essai de gestion en auto avec les taxis (ca peut etre utile)  

	// Correction du bug de trajet dans le dbc (curieux) (branruz)
	// Probleme de coordonnée du Node 316 et du node 318 dans le dbc ( = 0 sur x y et z )
	if(pMonture->m_TaxiNode == 319) // Depart Fort d'ebene (Fort d'ebene/Breche de mort)
	{
     sourcenode      = 316; // 1053 316 317
	 destinationnode = 317;
	}
	else if(pMonture->m_TaxiNode == 317) // Depart Breche de mort (Breche de mort/Fort d'ebene)
	{
     sourcenode      = 318; // 1054 318 319
	 destinationnode = 319;
	}
	else sTaxiMgr.GetTaxiNodeFromTo(pMonture->m_TaxiNode,&sourcenode,&destinationnode);

	if(!sourcenode || !destinationnode)
	{
		Log.Error("ActivateDirectTaxi","TaxiNodes.dbc Erreur ( %u - From %u To %u )",
			       pMonture->m_TaxiNode,sourcenode,destinationnode); 
		return;
	}
	/*else
	{
		Log.Warning("ActivateDirectTaxi","Taxi (%u) From %u To %u",pMonture->m_TaxiNode,sourcenode,destinationnode);
	}*/


	uint8 ss = _player->GetShapeShift();
	if( ss && (ss != FORM_BATTLESTANCE) && (ss != FORM_BERSERKERSTANCE) && (ss != FORM_DEFENSIVESTANCE) && (ss != FORM_STEALTH))
	{
		WorldPacket data(SMSG_ACTIVATETAXIREPLY, 4);
		data << uint32(9); //shape shifted
		SendPacket(&data);
		return;
	}

	WorldPacket data(SMSG_ACTIVATETAXIREPLY, 4);

	if(GetPlayer()->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER))
		return;

	TaxiPath* taxipath = sTaxiMgr.GetTaxiPath(sourcenode, destinationnode);
	
	if(!taxipath)
	{
		Log.Error("ActivateDirectTaxi","TaxiPath introuvable: From %u To %u",sourcenode,destinationnode);
		return;
	}
 
	TaxiNode* taxinode = sTaxiMgr.GetTaxiNode(sourcenode);

	if(!taxinode)
	{
		Log.Error("ActivateDirectTaxi","Taxinode inconnu: Id %u",sourcenode);
		return;
	}

	curloc = taxinode->id;
	field = (uint8)((curloc - 1) / 32);
	submask = 1<<((curloc-1)%32);

	/*
	// Check for known nodes
	if ( (GetPlayer()->GetTaximask(field) & submask) != submask )
	{   
		data << uint32( 1 );
		SendPacket( &data );
		return;
	}*/

	// Check for valid node
	if (!taxinode)
	{
		data << uint32( 1 ); // Unspecified Server Taxi Error
		SendPacket( &data );
		return;
	}

	if (!taxipath || !taxipath->GetNodeCount())
	{
		data << uint32( 2 ); // There is no direct path to that direction
		SendPacket( &data );
		return;
	}

	// Check for gold
	newmoney = ((GetPlayer()->GetUInt32Value(PLAYER_FIELD_COINAGE)) - taxipath->GetPrice());
	if(newmoney < 0 )
	{
		data << uint32( 3 ); // "Vous n'avez pas les moyens"
		SendPacket( &data );
		return;
	}

	uint32 modelid = 0; 
    if( _player->GetTeam()== FACTION_HORDE ) MountId = taxinode->horde_mount;
	else                                     MountId = taxinode->alliance_mount; 
         
	modelid = GetCreatureModelId(MountId,_player->GetTeam());

	//GetPlayer( )->setDismountCost( newmoney );

	data << uint32( 0 );
	// 0 Ok
	// 1 Unspecified Server Taxi Error
	// 2.There is no direct path to that direction
	// 3 Not enough Money
	SendPacket( &data );
	sLog.outDebug( "WORLD: Sent SMSG_ACTIVATETAXIREPLY" );

	// 0x001000 seems to make a mount visible
	// 0x002000 seems to make you sit on the mount, and the mount move with you
	// 0x000004 locks you so you can't move, no msg_move updates are sent to the server
	// 0x000008 seems to enable detailed collision checking

	// check for a summon -> if we do, remove.
	if(_player->GetSummon() != NULL)
	{		
		/*if(_player->GetSummon()->GetUInt32Value(UNIT_CREATED_BY_SPELL) > 0)
			_player->GetSummon()->Dismiss(false);						   // warlock summon -> dismiss
		else
			_player->GetSummon()->Remove(false, true, true);					  // hunter pet -> just remove for later re-call*/
		_player->GetSummon()->Dismiss(false);
	}

	_player->taxi_model_id = modelid;
	if (_player->IsStealth()) _player->RemoveAura(_player->m_stealth); //check for stealth flight
	GetPlayer()->TaxiStart(taxipath, modelid, 0);
	GetPlayer()->GetAchievementInterface()->HandleAchievementCriteriaFlightPathsTaken();
	
	//sLog.outString("TAXI: Starting taxi trip. Next update in %d msec.", first_node_time);
}
// Recup du modelId pour avoir la bonne monture du taxi

uint32 WorldSession::GetCreatureModelId(uint32 CreatureId,uint32 Player_Faction)
{

	uint32 DisplayId;

	if(CreatureId)
	{
     CreatureInfo *pCreatureTaxi = CreatureNameStorage.LookupEntry(CreatureId);
	 if(pCreatureTaxi)
	 {
	  DisplayId = pCreatureTaxi->Male_DisplayID;
	  if(DisplayId) return(DisplayId);
	  DisplayId = pCreatureTaxi->Male_DisplayID2;
      if(DisplayId) return(DisplayId);
	  DisplayId = pCreatureTaxi->Female_DisplayID;
      if(DisplayId) return(DisplayId);
	  DisplayId = pCreatureTaxi->Female_DisplayID2;
	  if(DisplayId) return(DisplayId);
	 }
	 else
	  Log.Error("GetCreatureModelId","Table 'creature_names' incomplete (%u manquant) <- Report this to AdminServer.",CreatureId);
	}

	// Pas d'Id de monture definie dans le TaxiPath.dbc, on en prend un par defaut en fonction de la faction
    if(Player_Faction == FACTION_ALLY)       return(1147); // Griffon
	else if(Player_Faction == FACTION_HORDE) return(1566); // Bat
	else 
	{
		Log.Error("GetCreatureModelId","Faction inconnu: %u <- Report this to devs.",Player_Faction);
		return(1147); // Griffon
	}
}