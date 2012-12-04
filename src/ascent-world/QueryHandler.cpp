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
/// This function handles CMSG_NAME_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleNameQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 guid;
	recv_data >> guid;

	PlayerInfo *pn = objmgr.GetPlayerInfo( (uint32)guid );
	//WoWGuid pguid(guid);

	if(!pn)
		return;

	sLog.outDebug( "Received CMSG_NAME_QUERY for: %s", pn->name );
	
	WoWGuid pguid((uint64)pn->guid); //VLack: The usual new style guid handling on 3.1.2
	uint8 databuffer[5000];
	StackPacket data(SMSG_NAME_QUERY_RESPONSE, databuffer, 5000);
	data << pguid;
	data << uint8(0);
	data << pn->name;
	data << uint8(0);	   // this probably is "different realm" or something flag.
	data << uint8(pn->race);
	data << uint8(pn->gender);
	data << uint8(pn->cl);
	data << uint8(0);
	SendPacket( &data );
	
	/*WorldPacket data(SMSG_NAME_QUERY_RESPONSE, strlen(pn->name) + 35);
	data << pn->guid << uint32(0);	//highguid
	data << pn->name;
	data << uint8(0);	   // this probably is "different realm" or something flag.
	data << pn->race << pn->gender << pn->cl;
	//Removed 3.0.2, Restored in 3.0.8 (Branruz)
	data << uint8(0);	// 2.4.0, why do i get the feeling blizz is adding custom classes or custom titles? (same thing in who list)
	SendPacket( &data );*/
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_QUERY_TIME:
//////////////////////////////////////////////////////////////
void WorldSession::HandleQueryTimeOpcode( WorldPacket & recv_data )
{
	WorldPacket data( SMSG_QUERY_TIME_RESPONSE, 4+4 );
	data << (uint32)UNIXTIME;
	data << (uint32)0; //VLack: 3.1; thanks Stewart for reminding me to have the correct structure even if it seems the old one still works.
	SendPacket( &data );
	
	/*uint32 t = (uint32)UNIXTIME;
#ifdef USING_BIG_ENDIAN
	swap32(&t);
#endif
	OutPacket(SMSG_QUERY_TIME_RESPONSE, 4, &t);*/
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_CREATURE_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleCreatureQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 12);
	WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 250); //VLack: thanks Aspire, this was 146 before
	uint32 entry;
	uint64 guid;
	CreatureInfo *ci;

	recv_data >> entry;
	recv_data >> guid;

	if(entry == 300000)
	{
		data << (uint32)entry;
		data << "WayPoint";
		data << uint8(0) << uint8(0) << uint8(0);
		data << "Level is WayPoint ID";
		for(uint32 i = 0; i < 8;i++)
		{
			data << uint32(0);
		}
		data << uint8(0);  
	}
	else
	{
		ci = CreatureNameStorage.LookupEntry(entry);
		if(ci == NULL)
			return;

		LocalizedCreatureName * lcn = (language>0) ? sLocalizationMgr.GetLocalizedCreatureName(entry, language) : NULL;

		if(lcn == NULL)
		{
			sLog.outDebug("WORLD: CMSG_CREATURE_QUERY '%s'", ci->Name);
			data << (uint32)entry;
			data << ci->Name;
			data << uint8(0) << uint8(0) << uint8(0);
			data << ci->SubName;
		}
		else
		{
			sLog.outDebug("WORLD: CMSG_CREATURE_QUERY '%s' (localized to %s)", ci->Name, lcn->Name);
			data << (uint32)entry;
			data << lcn->Name;
			data << uint8(0) << uint8(0) << uint8(0);
			data << lcn->SubName;
		}
		data << ci->info_str; //!!! this is a string in 2.3.0 Example: stormwind guard has : "Direction"
		data << ci->Flags1;  
		data << ci->Type;
		data << ci->Family;
		data << ci->Rank;
		//data << ci->Unknown1; // En commentaire 3.0.8, je le vois plus ds les log, en test
		data << ci->SpellDataID;
		data << ci->Male_DisplayID;
		data << ci->Female_DisplayID;
		data << ci->Male_DisplayID2;
		data << ci->Female_DisplayID2;
		data << ci->unkfloat1;
		data << ci->unkfloat2;
		data << ci->Leader;
		data << uint32(0);	// unk (Ascent)
		data << uint32(0);	// unk (Ascent)
		data << uint32(0);	// unk (Ascent)
		data << uint32(0);	// unk (Ascent)
		data << uint32(0);	// unk (Ascent)
	}

	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_GAMEOBJECT_QUERY
//////////////////////////////////////////////////////////////
void WorldSession::HandleGameObjectQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 12);
	WorldPacket data(SMSG_GAMEOBJECT_QUERY_RESPONSE, 900);

	uint32 entryID;
	uint64 guid;
	GameObjectInfo *goinfo;
	

	recv_data >> entryID;
	recv_data >> guid;

	sLog.outDebug("WORLD: CMSG_GAMEOBJECT_QUERY '%u'", entryID);

	goinfo = GameObjectNameStorage.LookupEntry(entryID);
	if(goinfo == NULL) return;

	LocalizedGameObjectName * lgn = (language>0) ? sLocalizationMgr.GetLocalizedGameObjectName(entryID, language) : NULL;
    
	data << entryID;
	data << goinfo->Type;
	data << goinfo->DisplayID;
	if(lgn)
		data << lgn->Name;
	else
		data << goinfo->Name;

	data << uint8(0) << uint8(0) << uint8(0) << uint8(0) << uint8(0) << uint8(0);   // new string in 1.12
	data << goinfo->raw.SpellFocus;
	data << goinfo->raw.sound1;
	data << goinfo->raw.sound2;
	data << goinfo->raw.sound3;
	data << goinfo->raw.sound4;
	data << goinfo->raw.sound5;
	data << goinfo->raw.sound6;
	data << goinfo->raw.sound7;
	data << goinfo->raw.sound8;
	data << goinfo->raw.sound9;
	data << goinfo->raw.Unknown1;
	data << goinfo->raw.Unknown2;
	data << goinfo->raw.Unknown3;
	data << goinfo->raw.Unknown4;
	data << goinfo->raw.Unknown5;
	data << goinfo->raw.Unknown6;
	data << goinfo->raw.Unknown7;
	data << goinfo->raw.Unknown8;
	data << goinfo->raw.Unknown9;
	data << goinfo->raw.Unknown10;
	data << goinfo->raw.Unknown11;
	data << goinfo->raw.Unknown12;
	data << goinfo->raw.Unknown13;
	data << goinfo->raw.Unknown14;
	//data << (uint32)goinfo->Unknown15; // float

	SendPacket( &data );

	if((goinfo->Type == GAMEOBJECT_TYPE_TRANSPORT) || (goinfo->Type == GAMEOBJECT_TYPE_MO_TRANSPORT))
	{
	 // On lance une requete de synchro si le Go est un transporters (Branruz)
	 WorldPacket *data = new WorldPacket(SMSG_TIME_SYNC_REQ, 4);
	 _player->SyncPlayerTickNumber  = 0; 
	 //------
	 //if(_player->m_CurrentTransporter)
	 //{
	 //	 if(_player->SyncCurrentTickNumber != 0)
		  _player->SyncServeurTickNumber = 0; // On remet a 0 pour un nouveau transport
		  _player->SyncCurrentTickNumber = 0;
	 //}
	 //else  _player->SyncCurrentTickNumber++;
	 //--------
	 _player->SyncServeurTickCount  = getMSTime();
	 *data << uint32(_player->SyncServeurTickNumber);
	 SendPacket(data);
	 Log.Notice("[SMSG_TIME_SYNC_REQ]","Demande Synchro pour %s",_player->GetName());

	}
}

//////////////////////////////////////////////////////////////
/// This function handles MSG_CORPSE_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleCorpseQueryOpcode(WorldPacket &recv_data)
{
	sLog.outDetail("WORLD: Received MSG_CORPSE_QUERY");

	Corpse *pCorpse;
	WorldPacket data(MSG_CORPSE_QUERY, 25);
	MapInfo *pMapinfo=NULL;

	pCorpse = objmgr.GetCorpseByOwner(GetPlayer()->GetLowGUID());

	if(pCorpse) Log.Warning("HandleCorpseQueryOpcode","%s, pCorpse: %u",GetPlayer()->GetName(),pCorpse->GetEntry());
	else        Log.Error("HandleCorpseQueryOpcode","%s, pCorpse NULL!",GetPlayer()->GetName());

	if(pCorpse)
	{
		pMapinfo = WorldMapInfoStorage.LookupEntry(pCorpse->GetMapId());
		if(pMapinfo)
		{
			if(pMapinfo->type == INSTANCE_NULL || pMapinfo->type == INSTANCE_PVP)
			{
				sLog.outDetail("WORLD: Send MSG_CORPSE_QUERY (Mort en Campagne)(LGuid: %u)",GetPlayer()->GetLowGUID());
				data << uint8(0x01); //show ?
				data << pCorpse->GetMapId(); // mapid (that tombstones shown on)
				data << pCorpse->GetPositionX();
				data << pCorpse->GetPositionY();
				data << pCorpse->GetPositionZ();
				data << pCorpse->GetMapId(); //instance mapid (needs to be same as mapid to be able to recover corpse)
				data << uint32(0);
				SendPacket(&data);
			}
			else
			{
				sLog.outDetail("WORLD: Send MSG_CORPSE_QUERY (Mort en Instance)(LGuid: %u)",GetPlayer()->GetLowGUID());
				data << uint8(0x01); //show ?
				data << pMapinfo->repopmapid; // mapid (that tombstones shown on)
				data << pMapinfo->repopx;
				data << pMapinfo->repopy;
				data << pMapinfo->repopz;
				data << pCorpse->GetMapId(); //instance mapid (needs to be same as mapid to be able to recover corpse)
				data << uint32(0);
				SendPacket(&data);
			}
		}
		else
		{
            sLog.outDetail("WORLD: Send MSG_CORPSE_QUERY (Mort par defaut)(LGuid: %u)",GetPlayer()->GetLowGUID());
			data.Initialize(MSG_CORPSE_QUERY);
			data << uint8(0x01); //show ?
			data << pCorpse->GetMapId(); // mapid (that tombstones shown on)
			data << pCorpse->GetPositionX();
			data << pCorpse->GetPositionY();
			data << pCorpse->GetPositionZ();
			data << pCorpse->GetMapId(); //instance mapid (needs to be same as mapid to be able to recover corpse)
			data << uint32(0);
			SendPacket(&data);

		}
	}
	else Log.Error("MSG_CORPSE_QUERY","%s, Corps player non trouvee!",GetPlayer()->GetName());
}

void WorldSession::HandlePageTextQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 4);
	uint32 pageid = 0;
	recv_data >> pageid;

	while(pageid)
	{
		ItemPage * page = ItemPageStorage.LookupEntry(pageid);
		if(!page)
			return;

		LocalizedItemPage * lpi = (language>0) ? sLocalizationMgr.GetLocalizedItemPage(pageid,language):NULL;
		WorldPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE, 1000);
		data << pageid;
		if(lpi)
			data << lpi->Text;
		else
			data << page->text;

		data << page->next_page;
		pageid = page->next_page;
		SendPacket(&data);
	}

}
//////////////////////////////////////////////////////////////
/// This function handles CMSG_ITEM_NAME_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleItemNameQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 4);
	WorldPacket reply(SMSG_ITEM_NAME_QUERY_RESPONSE, 100);
	uint32 itemid;
	recv_data >> itemid;
	reply << itemid;
	ItemPrototype *proto=ItemPrototypeStorage.LookupEntry(itemid);
	if(!proto)
		reply << "Unknown Item";
	else
	{
		LocalizedItem * li = (language>0) ? sLocalizationMgr.GetLocalizedItem(itemid, language) : NULL;
		if(li)
			reply << li->Name;
		else
			reply << proto->Name1;
	}
	SendPacket(&reply);	
}

// Reponse à CMSG_QUESTGIVER_STATUS_MULTIPLE_QUERY 
void WorldSession::HandleInrangeQuestgiverQuery(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;

	WorldPacket data(SMSG_QUESTGIVER_STATUS_MULTIPLE, 1000);
	Object::InRangeSet::iterator itr;
	Creature * pCreature;
	uint32 count = 0;
	data << (uint32)count;

	// 32 count
	// <foreach count>
	//    64 guid
	//    8 status

	for( itr = _player->m_objectsInRange.begin(); itr != _player->m_objectsInRange.end(); ++itr )
	{
		pCreature = static_cast<Creature*>(*itr);
		if( pCreature->GetTypeId() != TYPEID_UNIT )
			continue;

		if( pCreature->isQuestGiver() )
		{
			data << (uint64)pCreature->GetGUID();
			data << uint8(sQuestMgr.CalcStatus( pCreature, _player ));
			++count;
		}
	}

	*(uint32*)(data.contents()) = (uint32)count;
	SendPacket(&data);
}

// Reponse à  CMSG_QUEST_POI_QUERY - Source Mangos -
void WorldSession::HandleQuestPOIQuery(WorldPacket& recv_data)
{
	//Log.Warning("CMSG_QUEST_POI_QUERY","Recue");

	uint32 quest_count;
	recv_data >> quest_count;                                  // quest count, max=25

    if(quest_count > PLAYER_QUEST_LOG_MAX)
    {
        recv_data.rpos(recv_data.wpos());                   // set to end to avoid warnings spam
        return;
    }

    WorldPacket data(SMSG_QUEST_POI_QUERY_RESPONSE, 4+(4+4)*quest_count);
    data << uint32(quest_count);                                  // count
	
	QuestLogEntry *qle;
	
    for(uint32 i = 0; i < quest_count; ++i)
    {
        uint32 questId;
        recv_data >> questId;                               // quest id

		qle = _player->GetQuestLogInSlot( i );

        if( qle != NULL )
        {
            QuestPOIVector const * POI = objmgr.GetQuestPOIVector(questId);

            if(POI)
            {
                data << uint32(questId);                    // quest ID
                data << uint32(POI->size());                // POI count

                for(QuestPOIVector::const_iterator itr = POI->begin(); itr != POI->end(); ++itr)
                {
                    data << uint32(itr->PoiId);             // POI index
                    data << int32(itr->ObjectiveIndex);     // objective index
                    data << uint32(itr->MapId);             // mapid
                    data << uint32(itr->MapAreaId);         // world map area id
                    data << uint32(itr->FloorId);           // floor id
                    data << uint32(itr->Unk3);              // unknown
                    data << uint32(itr->Unk4);              // unknown
                    data << uint32(itr->points.size());     // POI points count

                    for(std::vector<QuestPOIPoint>::const_iterator itr2 = itr->points.begin(); itr2 != itr->points.end(); ++itr2)
                    {
                        data << int32(itr2->x);             // POI point x
                        data << int32(itr2->y);             // POI point y
                    }
                }
            }
            else
            {
                data << uint32(questId);                    // quest ID
                data << uint32(0);                          // POI count
            }
        }
        else
        {
            data << uint32(questId);                        // quest ID
            data << uint32(0);                              // POI count
        }
    }

    SendPacket(&data);
}
