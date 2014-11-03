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

/** Table formats converted to strings
 */
const char * gItemPrototypeFormat						= "uuuussssuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuffuffuuuuuuuuuufuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuusuuuuuuuuuuuuuuuuuuuuuuuuuuuuu";
const char * gCreatureNameFormat						= "usssuuuuuuuuuuffcc";
const char * gGameObjectNameFormat						= "uuusuuuuuuuuuuuuuuuuuuuuuuuuf";
const char * gCreatureProtoFormat						= "uuuuuuufuuuffuffuuuuuuuuuuuuffsuuuufffuu";

const char * gGraveyardFormat							= "uffffuuuux";
const char * gItemPageFormat							= "usu";

const char * gNpcTextFormat								= "ufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuufssuuuuuuu";
const char * gQuestFormat								= "uuuuuuuuuuuuuuuuuuuussssssssssuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu";
const char * gTeleportCoordFormat						= "uxufffx";
const char * gRealmMapInfoFormat						= "uuuuufffusuuuuuuufuu";
const char * gZoneGuardsFormat							= "uuu";

/** SQLStorage symbols
 */
SERVER_DECL SQLStorage<ItemPrototype, ArrayStorageContainer<ItemPrototype> >				ItemPrototypeStorage;
SERVER_DECL SQLStorage<CreatureInfo, HashMapStorageContainer<CreatureInfo> >				CreatureNameStorage;
SERVER_DECL SQLStorage<GameObjectInfo, HashMapStorageContainer<GameObjectInfo> >			GameObjectNameStorage;
SERVER_DECL SQLStorage<ItemPage, HashMapStorageContainer<ItemPage> >						ItemPageStorage;
SERVER_DECL SQLStorage<Quest, HashMapStorageContainer<Quest> >								QuestStorage;
SERVER_DECL SQLStorage<GossipText, HashMapStorageContainer<GossipText> >					NpcTextStorage;
SERVER_DECL SQLStorage<MapInfo, HashMapStorageContainer<MapInfo> >							WorldMapInfoStorage;

#define make_task(storage, itype, storagetype, tablename, format) tl.AddTask( new Task( \
	new CallbackP2< SQLStorage< itype, storagetype< itype > >, const char *, const char *> \
	(&storage, &SQLStorage< itype, storagetype< itype > >::Load, tablename, format) ) )

void Storage_FillTaskList(TaskList & tl)
{
	make_task(ItemPrototypeStorage, ItemPrototype, ArrayStorageContainer, "items", gItemPrototypeFormat);
	make_task(CreatureNameStorage, CreatureInfo, HashMapStorageContainer, "creature_names", gCreatureNameFormat);
	make_task(GameObjectNameStorage, GameObjectInfo, HashMapStorageContainer, "gameobject_names", gGameObjectNameFormat);
	make_task(ItemPageStorage, ItemPage, HashMapStorageContainer, "itempages", gItemPageFormat);
	make_task(QuestStorage, Quest, HashMapStorageContainer, "quests", gQuestFormat);
	make_task(WorldMapInfoStorage, MapInfo, HashMapStorageContainer, "realmmap_info", gRealmMapInfoFormat);
	make_task(NpcTextStorage, GossipText, HashMapStorageContainer, "npc_text", gNpcTextFormat);
}

/*void Storage_Load()
{
	ItemPrototypeStorage.Load("items", gItemPrototypeFormat);
	CreatureNameStorage.Load("creature_names", gCreatureNameFormat);
	GameObjectNameStorage.Load("gameobject_names", gGameObjectNameFormat);
	ItemPageStorage.Load("itempages", gItemPageFormat);
	QuestStorage.Load("quests", gQuestFormat);
	WorldMapInfoStorage.Load("realmmap_info", gRealmMapInfoFormat);
}*/

void Storage_Cleanup()
{
	ItemPrototypeStorage.Cleanup();
	CreatureNameStorage.Cleanup();
	GameObjectNameStorage.Cleanup();
	ItemPageStorage.Cleanup();
	QuestStorage.Cleanup();
	WorldMapInfoStorage.Cleanup();
}

bool Storage_ReloadTable(const char * TableName)
{
	// bur: mah god this is ugly :P
	if(!stricmp(TableName, "items"))					// Items
		ItemPrototypeStorage.Reload();
	else if(!stricmp(TableName, "creature_names"))		// Creature Names
		CreatureNameStorage.Reload();
	else if(!stricmp(TableName, "gameobject_names"))	// GO Names
		GameObjectNameStorage.Reload();
	else if(!stricmp(TableName, "itempages"))			// Item Pages
		ItemPageStorage.Reload();
	else if(!stricmp(TableName, "quests"))				// Quests
		QuestStorage.Reload();
	else if(!stricmp(TableName, "npc_text"))			// NPC Text Storage
		NpcTextStorage.Reload();
	else if(!stricmp(TableName, "realmmap_info"))		// WorldMapInfo
		WorldMapInfoStorage.Reload();
	else
		return false;
	return true;
}
