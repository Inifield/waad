/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2011 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the Ascent Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the Ascent is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#include "RStdAfx.h"

//////////////////////////////////////////////////////////////
/// This function handles CMSG_CREATURE_QUERY:
//////////////////////////////////////////////////////////////
void Session::HandleCreatureQueryOpcode( WorldPacket & recv_data )
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
		Log.Debug("World", "CMSG_CREATURE_QUERY '%s'", ci->Name);

		data << (uint32)entry;
		data << ci->Name;
		data << uint8(0) << uint8(0) << uint8(0);
		data << ci->SubName;
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
void Session::HandleGameObjectQueryOpcode( WorldPacket & recv_data )
{
	//CHECK_PACKET_SIZE(recv_data, 12);
	WorldPacket data(SMSG_GAMEOBJECT_QUERY_RESPONSE, 900);

	uint32 entryID;
	uint64 guid;
	GameObjectInfo *goinfo;
	

	recv_data >> entryID;
	recv_data >> guid;

	sLog.outDebug("WORLD: CMSG_GAMEOBJECT_QUERY '%u'", entryID);

	goinfo = GameObjectNameStorage.LookupEntry(entryID);
	if(goinfo == NULL) return;

	data << entryID;
	data << goinfo->Type;
	data << goinfo->DisplayID;
	data << goinfo->Name;
	data << uint8(0) << uint8(0) << uint8(0) << uint8(0) << uint8(0) << uint8(0);   // new string in 1.12
	data << goinfo->SpellFocus;
	data << goinfo->sound1;
	data << goinfo->sound2;
	data << goinfo->sound3;
	data << goinfo->sound4;
	data << goinfo->sound5;
	data << goinfo->sound6;
	data << goinfo->sound7;
	data << goinfo->sound8;
	data << goinfo->sound9;
	data << goinfo->Unknown1;
	data << goinfo->Unknown2;
	data << goinfo->Unknown3;
	data << goinfo->Unknown4;
	data << goinfo->Unknown5;
	data << goinfo->Unknown6;
	data << goinfo->Unknown7;
	data << goinfo->Unknown8;
	data << goinfo->Unknown9;
	data << goinfo->Unknown10;
	data << goinfo->Unknown11;
	data << goinfo->Unknown12;
	data << goinfo->Unknown13;
	data << goinfo->Unknown14;
	//data << (uint32)goinfo->Unknown15; // float

	SendPacket( &data );

}

void Session::HandleItemQuerySingleOpcode( WorldPacket & recv_data )
{
	//CHECK_PACKET_SIZE(recv_data, 4);
 
	unsigned int i;
	uint32 itemid=0;
	recv_data >> itemid;

	Log.Debug("World", "CMSG_ITEM_QUERY_SINGLE for item id %d",	itemid );

	ItemPrototype *itemProto = ItemPrototypeStorage.LookupEntry(itemid);
	if(!itemProto)
	{
		sLog.outError( "WORLD: Unknown item id 0x%.8X", itemid );
		return;
	} 

	WorldPacket data(SMSG_ITEM_QUERY_SINGLE_RESPONSE, 600 + strlen(itemProto->Name1) + strlen(itemProto->Description) );
	data << itemProto->ItemId;
	data << itemProto->Class;
	data << itemProto->SubClass;
	data << itemProto->unknown_bc;
	data << itemProto->Name1;

	/*data << itemProto->Name2;
	data << itemProto->Name3;
	data << itemProto->Name4;*/
	data << uint8(0) << uint8(0) << uint8(0); // name 2,3,4
	data << itemProto->DisplayInfoID;
	data << itemProto->Quality;
	data << itemProto->Flags;
	data << itemProto->Faction;	//Added in 3.2 (Ascent)
	data << itemProto->BuyPrice;
	data << itemProto->SellPrice;
	data << itemProto->InventoryType;
	data << itemProto->AllowableClass;
	data << itemProto->AllowableRace;
	data << itemProto->ItemLevel;
	data << itemProto->RequiredLevel;
	data << itemProto->RequiredSkill;
	data << itemProto->RequiredSkillRank;
	data << itemProto->RequiredSkillSubRank;
	data << itemProto->RequiredPlayerRank1;
	data << itemProto->RequiredPlayerRank2; // ?? obsolete 3.0.3 ?? (branruz)
	data << itemProto->RequiredFaction;
	data << itemProto->RequiredFactionStanding;
	data << itemProto->Unique;
	data << itemProto->MaxCount;
	data << itemProto->ContainerSlots;
	if(itemProto->StatsCount > 10) itemProto->StatsCount = 10; // Secu 
	data << itemProto->StatsCount; //data << uint32(10);
	 for(i = 0; i < (itemProto->StatsCount); i++)
	 {
		data << itemProto->Stats[i].Type;
		data << itemProto->Stats[i].Value;
	 }
	 
	// Sur l'item 2092, on voit clairement
	// que les +1600 protect d'ombre sont decalés de 2 DWORD
	// car ils prennent la valeur du champ 'Delay'
	// idem sur aspiredev ou arcemu
	data << itemProto->ScalingStatsEntry; //(uint32)(0); // Test Branruz
	data << itemProto->ScalingStatsFlag;; //(uint32)(0); // Test Branruz
	//------------------------------------------
	for(i = 0; i < 2; i++) // 5 en 309, 2 en 332 ( items.wbd 332 )
	{
		data << itemProto->Damage[i].Min;
		data << itemProto->Damage[i].Max;
		data << itemProto->Damage[i].Type;
	}
	// 7 resistances
	data << itemProto->Armor;
	data << itemProto->HolyRes;
	data << itemProto->FireRes;
	data << itemProto->NatureRes;
	data << itemProto->FrostRes;
	data << itemProto->ShadowRes;
	data << itemProto->ArcaneRes;
	data << itemProto->Delay;
	data << itemProto->AmmoType;
	data << itemProto->Range;
	for(i = 0; i < 5; i++) 
	{
		data << itemProto->Spells[i].Id;
		data << itemProto->Spells[i].Trigger;
		data << itemProto->Spells[i].Charges;
		data << itemProto->Spells[i].Cooldown;
		data << itemProto->Spells[i].Category;
		data << itemProto->Spells[i].CategoryCooldown;
	}
	data << itemProto->Bonding;
	data << itemProto->Description;

	data << itemProto->PageId;
	data << itemProto->PageLanguage;
	data << itemProto->PageMaterial;
	data << itemProto->QuestId;
	data << itemProto->LockId;
	data << itemProto->LockMaterial;
	data << itemProto->Field108;
	data << itemProto->RandomPropId;
	data << itemProto->RandomSuffixId;
	data << itemProto->Block;
	data << itemProto->ItemSet;
	data << itemProto->MaxDurability;
	data << itemProto->ZoneNameID;
	data << itemProto->MapID;
	data << itemProto->BagFamily;
	data << itemProto->TotemCategory;
	// 3 sockets
	data << itemProto->Sockets[0].SocketColor ;
	data << itemProto->Sockets[0].Unk;
	data << itemProto->Sockets[1].SocketColor ;
	data << itemProto->Sockets[1].Unk ;
	data << itemProto->Sockets[2].SocketColor ;
	data << itemProto->Sockets[2].Unk ;
	data << itemProto->SocketBonus;
	data << itemProto->GemProperties;
	data << itemProto->DisenchantReqSkill;
	data << itemProto->ArmorDamageModifier;
	//-- ArcEmu
	data << itemProto->ExistingDuration;	// 2.4.2 Item duration in seconds
	data << itemProto->ItemLimitCategory;
	data << itemProto->HolidayId; //MesoX: HolidayId - points to HolidayNames.dbc
    // ---

	SendPacket( &data );
	
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_ITEM_NAME_QUERY:
//////////////////////////////////////////////////////////////
void Session::HandleItemNameQueryOpcode( WorldPacket & recv_data )
{
	//CHECK_PACKET_SIZE(recv_data, 4);
	uint8 databuffer[1000];
	StackPacket reply(SMSG_ITEM_NAME_QUERY_RESPONSE, databuffer, 1000);

	uint32 itemid;
	recv_data >> itemid;
	reply << itemid;
	ItemPrototype *proto=ItemPrototypeStorage.LookupEntry(itemid);
	if(!proto)
		reply << "Unknown Item";
	else
	{
		reply << proto->Name1;
	}
	SendPacket(&reply);	
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_PAGE_TEXT_QUERY
//////////////////////////////////////////////////////////////
void Session::HandlePageTextQueryOpcode( WorldPacket & recv_data )
{
	//CHECK_PACKET_SIZE(recv_data, 4);
	uint32 pageid = 0;
	uint8 buffer[10000];
	StackPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE,buffer, 10000);
	recv_data >> pageid;

	while(pageid)
	{
		ItemPage * page = ItemPageStorage.LookupEntry(pageid);
		if(page == NULL)
			return;

		data.Clear();
		data << pageid;
		data.Write((uint8*)page->text, strlen(page->text) + 1);

		data << page->next_page;
		pageid = page->next_page;
		SendPacket(&data);
	}
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_QUERY_TIME:
//////////////////////////////////////////////////////////////
void Session::HandleQueryTimeOpcode( WorldPacket & recv_data )
{
	uint32 t = (uint32)UNIXTIME;
	OutPacket(SMSG_QUERY_TIME_RESPONSE, 4, &t);
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_NAME_QUERY:
//////////////////////////////////////////////////////////////
void Session::HandleNameQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 guid;
	recv_data >> guid;

	RPlayerInfo *pn = sClientMgr.GetRPlayer( (uint32)guid );
	//WoWGuid pguid(guid);

	if(!pn)
		return;

	sLog.outDebug( "[Session] Reception de CMSG_NAME_QUERY pour: %s", pn->Name );
	
	WoWGuid pguid((uint64)pn->Guid); //VLack: The usual new style guid handling on 3.1.2
	uint8 databuffer[5000];
	StackPacket data(SMSG_NAME_QUERY_RESPONSE, databuffer, 5000);
	data << pguid;
	data << uint8(0);
	data << pn->Name;
	data << uint8(0);	   // this probably is "different realm" or something flag.
	data << uint8(pn->Race);
	data << uint8(pn->Gender);
	data << uint8(pn->Class);
	data << uint8(0);
	SendPacket( &data );
}