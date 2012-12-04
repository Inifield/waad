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

initialiseSingleton( ChatHandler );
initialiseSingleton(CommandTableStorage);

ChatCommand * ChatHandler::getCommandTable()
{
	ASSERT(false);
	return 0;
}

ChatCommand * CommandTableStorage::GetSubCommandTable(const char * name)
{
	if(!strcmp(name, "modify"))            return _modifyCommandTable;
	else if(!strcmp(name, "debug")) 	   return _debugCommandTable;
	else if(!strcmp(name, "waypoint"))     return _waypointCommandTable;
	else if(!strcmp(name, "gmticket"))     return _GMTicketCommandTable;
	else if(!strcmp(name, "gobject"))      return _GameObjectCommandTable;
	else if(!strcmp(name, "battleground")) return _BattlegroundCommandTable;
	else if(!strcmp(name, "npc"))          return _NPCCommandTable;
	else if(!strcmp(name, "cheat"))        return _CheatCommandTable;
	else if(!strcmp(name, "account"))      return _accountCommandTable;
	else if(!strcmp(name, "pet"))          return _petCommandTable;
	else if(!strcmp(name, "recall"))       return _recallCommandTable;
	else if(!strcmp(name, "honor"))        return _honorCommandTable;
	else if(!strcmp(name, "guild"))        return _GuildCommandTable;
	else if(!strcmp(name, "quest"))        return _questCommandTable;
	else if(!strcmp(name, "character"))    return _charCommandTable;           // Tazjin, V4593
	else if(!strcmp(name, "worldannounce")) return _worldannounceCommandTable; // Cinemania
	else if(!strcmp(name, "server"))       return _serverCommandTable;         // Cinemania
	else if(!strcmp(name, "lookup"))       return _lookupCommandTable;         // Cinemania
	else if(!strcmp(name, "achievement"))  return _achCommandTable;            // Mizuka
		
	return 0;
}

#define dupe_command_table(ct, dt) this->dt = (ChatCommand*)allocate_and_copy(sizeof(ct)/* / sizeof(ct[0])*/, ct)
ASCENT_INLINE void* allocate_and_copy(uint32 len, void * pointer)
{
	void * data = (void*)malloc(len);
	memcpy(data, pointer, len);
	return data;
}

void CommandTableStorage::Load()
{
	QueryResult * result = CharacterDatabase.Query("SELECT * FROM gm_overrides");
	if(!result) return;

	do 
	{
		const char * name = result->Fetch()[0].GetString();
		const char * level = result->Fetch()[1].GetString();
		Override(name, level);
	} while(result->NextRow());
	delete result;
}
                                        // npc info                    'n'
void CommandTableStorage::Override(const char * command, const char * level)
{
	//ASSERT(level[0] != '\0');

	if(!level[0])
	{
	 Log.Error("CommandTableStorage::Override","Level[0] est NULL!! ( Command: %s)",command);
	 return;
	}

	char * cmd = strdup(command);

	// find the command we're talking about
	char * sp = strchr(cmd, ' ');
	const char * command_name = cmd;
	const char * subcommand_name = 0;

	if(sp != 0)
	{
		// we're dealing with a subcommand.
		*sp = 0;
		subcommand_name = sp + 1;
	}

	size_t len1 = strlen(command_name);
	size_t len2 = subcommand_name ? strlen(subcommand_name) : 0;

	// look for the command.
	ChatCommand * p = &_commandTable[0];
	while(p->Name != 0)
	{
		if(!strnicmp(p->Name, command_name, len1))
		{
			// this is the one we wanna modify
			if(!subcommand_name)
			{
				// no subcommand, we can change it.
				p->CommandGroup = level[0];
				Log.Notice("ChatCommand","Changing command level of command `%s` to %c", p->Name, level[0]);
			}
			else
			{
				// assume this is a subcommand, loop the second set.
				ChatCommand * p2 = p->ChildCommands;
				if(!p2)
				{
					Log.Warning("ChatCommand","Invalid ChatCommand specified for table Gm_Override: %s", command_name);
				}
				else
				{
					while(p2->Name != 0)
					{
						if(!strnicmp("*",subcommand_name,1))
						{
								p2->CommandGroup = level[0];
								printf("Changing command level of command (wildcard) `%s`:`%s` to %c.\n", p->Name, p2->Name, level[0]);
						}else{
							if(!strnicmp(p2->Name, subcommand_name, len2))
							{
								// change the level
								p2->CommandGroup = level[0];
								printf("Changing command level of command `%s`:`%s` to %c.\n", p->Name, p2->Name, level[0]);
								break;
							}
						}
						p2++;
					}
					if(p2->Name == 0)
					{
						if(strnicmp("*",subcommand_name,1)) //Hacky.. meh.. -DGM
						{
						 printf("Invalid subcommand referenced: `%s` under `%s`.\n", subcommand_name, p->Name);
						}
						break;
					}
				}
			}
			break;
		}
		++p;
	}

	if(p->Name == 0)
	{
		printf("Invalid command referenced: `%s`\n", command_name);
	}

	free(cmd);
}

void CommandTableStorage::Dealloc()
{
	free( _modifyCommandTable );
	free( _debugCommandTable );
	free( _waypointCommandTable );
	free( _GMTicketCommandTable );
	free( _GameObjectCommandTable );
	free( _BattlegroundCommandTable );
	free( _NPCCommandTable );
	free( _CheatCommandTable );
	free( _accountCommandTable );
	free( _petCommandTable );
	free( _recallCommandTable );
	free( _honorCommandTable );
	free( _GuildCommandTable);
	free( _questCommandTable );
	free( _worldannounceCommandTable ); // cinemania
	free( _serverCommandTable ); // cinemania
	free( _lookupCommandTable ); // cinemania
	free( _achCommandTable ); // Mizuka
	free( _charCommandTable );
	free( _commandTable );
}

void CommandTableStorage::Init()
{
	static ChatCommand modifyCommandTable[] =
	{  //  Name  ,CommandGroup , ChatHandler::*Handler                     ,Help                                 ,               ChildCommands , NormalValueField , MaxValueField       ,  ValueType;	// 0 = nothing, 1 = uint, 2 = float          
		{ "hp",				'm', NULL,										".modify hp <count> - Points de vie.",						   NULL, UNIT_FIELD_HEALTH, UNIT_FIELD_MAXHEALTH, 1 },
		{ "gender",			'm', &ChatHandler::HandleGenderChanger,			".modify gender <0=Homme/1=Femme> - Genre.",				   NULL, 0, 0, 0},
		{ "mana",			'm', NULL,										".modify mana <count> - Points de mana.",					   NULL, UNIT_FIELD_POWER1, UNIT_FIELD_MAXPOWER1, 1 },
		{ "rage",			'm', NULL,										".modify rage <count> - Points de rage.",					   NULL, UNIT_FIELD_POWER2, UNIT_FIELD_MAXPOWER2, 1 },
		{ "energy",			'm', NULL,										".modify energy <count> - Points d'energie.",				   NULL, UNIT_FIELD_POWER4, UNIT_FIELD_MAXPOWER4, 1 },
		{ "level",			'm', &ChatHandler::HandleModifyLevelCommand,	".modify level <level> - Niveau.",							   NULL, 0, 0, 0 },
		{ "armor",			'm', NULL,										".modify armor <count> - Armure.",							   NULL, UNIT_FIELD_STAT1,	0, 1 },
		{ "holy",			'm', NULL,										".modify holy <val> - Resistance au sacre.",				   NULL, UNIT_FIELD_RESISTANCES, 0,	1 },
		{ "fire",			'm', NULL,										".modify fire <val> - Resistance au feu.",					   NULL, UNIT_FIELD_RESISTANCES_2, 0,	1 },
		{ "nature",			'm', NULL,										".modify nature <val> - Resistance a la nature.",			   NULL, UNIT_FIELD_RESISTANCES_3, 0,	1 },
		{ "frost",			'm', NULL,										".modify frost <val> - Resistance au givre.",				   NULL, UNIT_FIELD_RESISTANCES_4, 0,	1 },
		{ "shadow",			'm', NULL,										".modify shadow <val> - Resistance a l'ombre.",				   NULL, UNIT_FIELD_RESISTANCES_5, 0,	1 },
		{ "arcane",			'm', NULL,										".modify arcane <val> - Resistance aux arcanes.",			   NULL, UNIT_FIELD_RESISTANCES_6, 0,	1 },
		{ "damage",			'm', NULL,										".modify damage <min> <max> - Degats de l'unite.",			   NULL, UNIT_FIELD_MINDAMAGE, UNIT_FIELD_MAXDAMAGE, 2 },
		{ "scale",			'm', NULL,										".modify scale <[0:1]> - Taille/Echelle.",					   NULL, OBJECT_FIELD_SCALE_X, 0, 2 },
		{ "gold",			'm', &ChatHandler::HandleModifyGoldCommand,		".modify gold <count> - Monnaie (en cuivre).",				   NULL, 0, 0, 0 },
		{ "speed",			'm', &ChatHandler::HandleModifySpeedCommand,	".modify speed <speed> - Vitesse de deplacement.",			   NULL, 0, 0, 0 },
		{ "nativedisplayid",'M', NULL,										".modify nativedisplayid <id> - Display ID natif.",			   NULL, UNIT_FIELD_NATIVEDISPLAYID, 0, 1 },
		{ "displayid",		'M', NULL,										".modify displayid <id> - Display ID.",						   NULL, UNIT_FIELD_DISPLAYID,	0, 1 },
		{ "flags",			'M', NULL,										".modify flags <id> - Flags de l'unite.",					   NULL, UNIT_FIELD_FLAGS,	0, 1 },
		{ "faction",		'M', NULL,										".modify faction <id> - Faction.",							   NULL, UNIT_FIELD_FACTIONTEMPLATE, 0, 1 },
		{ "dynamicflags",	'M', NULL,										".modify dynamicflags <id> - Flags dynamiques.",			   NULL, UNIT_DYNAMIC_FLAGS, 0, 1 },
		{ "talentpoints",	'm', NULL,										".modify talentpoints <val> - Points de talent.",			   NULL, PLAYER_CHARACTER_POINTS1, 0, 1 },
		{ "happiness",		'm', &ChatHandler::HandleModifyHappinessCommand,".modify hapiness <val> [<val> = Loyalty]",                    NULL, 0, 0, 0 },// UNIT_FIELD_POWER5, UNIT_FIELD_MAXPOWER5, 1 },
		{ "strength",		'm', NULL,										".modify strength <val> <maxval>(facultatif) - Force.",		   NULL, UNIT_FIELD_STAT0,	0, 1 },
		{ "agility",		'm', NULL,										".modify agility <val> <maxval>(facultatif) - Agilite.",	   NULL, UNIT_FIELD_STAT1,	0, 1 },
		{ "stamina",		'm', NULL,										".modify stamina <val> <maxval>(facultatif) - Endurance.",	   NULL, UNIT_FIELD_STAT2,	0, 1 },
		{ "intellect",		'm', NULL,										".modify intellect <val> <maxval>(facultatif) - Intelligence.",NULL, UNIT_FIELD_STAT3,	0, 1 },
		{ "spirit",			'm', NULL,										".modify spirit <val> <maxval>(facultatif) - Esprit.",		   NULL, UNIT_FIELD_STAT4,	0, 1 },
		{ "boundingradius",	'm', NULL,										".modify boundingradius <val> - Champ de vision.",			   NULL, UNIT_FIELD_BOUNDINGRADIUS, 0,	2 },
		{ "combatreach",	'm', NULL,										".modify combatreach <val> - Portee de combat.",			   NULL, UNIT_FIELD_COMBATREACH, 0, 2 },
		{ "emotestate",		'm', NULL,										".modify emote <id> - Emote.",								   NULL, UNIT_NPC_EMOTESTATE, 0, 1 },
		{ "bytes",			'M', NULL,										".modify bytes <id> - Bytes.",								   NULL, UNIT_FIELD_BYTES_0, 0, 1 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0  }
	};
	dupe_command_table(modifyCommandTable, _modifyCommandTable);

	static ChatCommand debugCommandTable[] =
	{
		{ "sync",             'd', &ChatHandler::HandleDebugSynchroClient, ".debug sync - Force une demande de synchro",NULL, 0, 0, 0},
		{ "retroactivequest", 'd', &ChatHandler::HandleDebugRetroactiveQuestAchievements, "",		NULL, 0, 0, 0},
		{ "infront",		'd', &ChatHandler::HandleDebugInFrontCommand,	"",						NULL, 0, 0, 0},
		{ "showreact",		'd', &ChatHandler::HandleShowReactionCommand,	"",						NULL, 0, 0, 0},
		{ "aimove",			'd', &ChatHandler::HandleAIMoveCommand,			"",						NULL, 0, 0, 0},
		{ "dist",			'd', &ChatHandler::HandleDistanceCommand,		"",						NULL, 0, 0, 0},
		{ "face",			'd', &ChatHandler::HandleFaceCommand,			"",						NULL, 0, 0, 0},
		{ "moveinfo",		'd', &ChatHandler::HandleMoveInfoCommand,		"",						NULL, 0, 0, 0},
		{ "setbytes",		'd', &ChatHandler::HandleSetBytesCommand,		"",						NULL, 0, 0, 0},
		{ "getbytes",		'd', &ChatHandler::HandleGetBytesCommand,		"",						NULL, 0, 0, 0},
		{ "unroot",			'd', &ChatHandler::HandleDebugUnroot,			"",						NULL, 0, 0, 0},
		{ "root",			'd', &ChatHandler::HandleDebugRoot,				"",						NULL, 0, 0, 0},
		{ "landwalk",		'd', &ChatHandler::HandleDebugLandWalk,			"",						NULL, 0, 0, 0},
		{ "waterwalk",		'd', &ChatHandler::HandleDebugWaterWalk,		"",						NULL, 0, 0, 0},
		{ "castspellne",	'd', &ChatHandler::HandleCastSpellNECommand,	".debug castspellne <spellid> - Casts spell on target (only plays animations, doesnt handle effects or range/facing/etc.", NULL, 0, 0, 0 },
		{ "aggrorange",		'd', &ChatHandler::HandleAggroRangeCommand,		".debug aggrorange - Shows aggro Range of the selected Creature.", NULL, 0, 0, 0 },
		{ "knockback",		'd', &ChatHandler::HandleKnockBackCommand,		".debug knockback <value> - Knocks you back.",				NULL, 0, 0, 0 },
		{ "fade",			'd', &ChatHandler::HandleFadeCommand,			".debug fade <value> - calls ModThreatModifyer().",			NULL, 0, 0, 0 },
		{ "threatmod",		'd', &ChatHandler::HandleThreatModCommand,		".debug threatMod <value> - calls ModGeneratedThreatModifyer().", NULL, 0, 0, 0 },
		{ "calcthreat",		'd', &ChatHandler::HandleCalcThreatCommand,		".debug calcThreat <dmg> <spellId> - calculates threat.",		NULL, 0, 0, 0 },
		{ "threatList",		'd', &ChatHandler::HandleThreatListCommand,		".debug threatList  - returns all AI_Targets of the selected Creature.", NULL, 0, 0, 0 },
		{ "gettptime",		'd', &ChatHandler::HandleGetTransporterTime,	".debug grabs transporter travel time",						NULL, 0, 0, 0 },
		{ "itempushresult",	'd',&ChatHandler::HandleSendItemPushResult,		".debug sends item push result", 								NULL, 0, 0, 0 },
		{ "setbit",			'd',  &ChatHandler::HandleModifyBitCommand,		"",														NULL, 0, 0, 0},
		{ "setvalue",		'd', &ChatHandler::HandleModifyValueCommand,	"",														NULL, 0, 0, 0},
		{ "aispelltestbegin",'d', &ChatHandler::HandleAIAgentDebugBegin,	"",														NULL, 0, 0, 0 },
		{ "aispelltestcontinue",'d', &ChatHandler::HandleAIAgentDebugContinue,"",													NULL, 0, 0, 0 },
		{ "aispelltestskip",'d', &ChatHandler::HandleAIAgentDebugSkip,		"",														NULL, 0, 0, 0 },
		{ "dumpcoords",		'd', &ChatHandler::HandleDebugDumpCoordsCommmand,"",													NULL, 0, 0, 0 },
        { "sendpacket",		'd', &ChatHandler::HandleSendpacket,			"<opcode ID>, <data>",									NULL, 0, 0, 0 },
		{ "sqlquery",		'd', &ChatHandler::HandleSQLQueryCommand,		"<sql query>",											NULL, 0, 0, 0 },
		{ "rangecheck",		'd', &ChatHandler::HandleRangeCheckCommand,		"Checks the 'yard' range and internal range between the player and the target.", NULL, 0, 0, 0 },
		{ "setallratings",	'd', &ChatHandler::HandleRatingsCommand,		"Sets rating values to incremental numbers based on their index.", NULL, 0, 0, 0 },
		{ "pvp",	        'd', &ChatHandler::HandlePvPCommand,		    "Change PvP setting.", NULL, 0, 0, 0 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0  }
	};
	dupe_command_table(debugCommandTable, _debugCommandTable);

	static ChatCommand waypointCommandTable[] =
	{
		{ "add",			'w', &ChatHandler::HandleWPAddCommand,			".waypoint add <Params...> - Ajoute un WP sur votre position actuelle.",								NULL, 0, 0, 0},
		{ "show",			'w', &ChatHandler::HandleWPShowCommand,			".waypoint show - Montre les WP de la creature.",								NULL, 0, 0, 0},
		{ "hide",			'w', &ChatHandler::HandleWPHideCommand,			".waypoint hide - Masque les WP de la creature.",								NULL, 0, 0, 0},
		{ "delete",			'w', &ChatHandler::HandleWPDeleteCommand,		".waypoint delete - Supprime le WP selectionne.",									NULL, 0, 0, 0},
		{ "movehere",		'w', &ChatHandler::HandleWPMoveHereCommand,		".waypoint movehere - Bouge vers ce WP.",										NULL, 0, 0, 0},
		{ "flags",			'w', &ChatHandler::HandleWPFlagsCommand,		".waypoint flags <id> - Flags du WP.",												NULL, 0, 0, 0},
		{ "waittime",		'w', &ChatHandler::HandleWPWaitCommand,			".waypoint waittime <time> - Temps d'attente sur ce WP.",									NULL, 0, 0, 0},
		{ "emote",			'w', &ChatHandler::HandleWPEmoteCommand,		".waypoint emote <emote_id> [<backward=0 0:1>  [OneShot=1 0:1]] - Emote joué sur ce WP.",NULL, 0, 0, 0},
		{ "skin",			'w', &ChatHandler::HandleWPSkinCommand,			".waypoint skin <id> - Modele applique a ce WP.",										NULL, 0, 0, 0},
		{ "change",			'w', &ChatHandler::HandleWPChangeNoCommand,		".waypoint change - Change a ce WP.",									NULL, 0, 0, 0},
		{ "info",			'w', &ChatHandler::HandleWPInfoCommand,			".waypoint info - Montre les infos du WP.",										NULL, 0, 0, 0},
		{ "movetype",		'w', &ChatHandler::HandleWPMoveTypeCommand,		".waypoint movetype <id> - Type de mouvement sur ce WP.",									NULL, 0, 0, 0},
		{ "generate",		'w', &ChatHandler::HandleGenerateWaypoints,		".waypoint generate <Rayon> <Nbre> - Genere aleatoirement des WP.",								NULL, 0, 0, 0},
		{ "save",			'w', &ChatHandler::HandleSaveWaypoints,			".waypoint save - Sauvegarde tous les WP.",									NULL, 0, 0, 0},
		{ "deleteall",		'w', &ChatHandler::HandleDeleteWaypoints,		".waypoint deleteall - Supprime tous les WP.",									NULL, 0, 0, 0},
		{ "addfly",			'w', &ChatHandler::HandleWaypointAddFlyCommand, ".waypoint addfly - Ajoute un WP en vol.",								NULL, 0, 0, 0 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0  }
	};
	dupe_command_table(waypointCommandTable, _waypointCommandTable);

	static ChatCommand GMTicketCommandTable[] =
	{
		{ "get",			'G', &ChatHandler::HandleGMTicketGetAllCommand,	".gmticket get - Obtient le GM Ticket.",										NULL, 0, 0, 0 },
		{ "getId",			'G', &ChatHandler::HandleGMTicketGetByIdCommand,".gmticket getId <id> - Obtient le GM Ticket par ID.",									NULL, 0, 0, 0 },
		{ "delId",			'G', &ChatHandler::HandleGMTicketDelByIdCommand,".gmticket delId <id> - Supprime le GM Ticket par ID.",								NULL, 0, 0, 0 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0  }
	};
	dupe_command_table(GMTicketCommandTable, _GMTicketCommandTable);

	static ChatCommand GuildCommandTable[] =
	{
		{ "create",			'g', &ChatHandler::CreateGuildCommand,			".guild create <name> - Cree une guilde.",										NULL, 0, 0, 0 },
		{ "rename",			'g', &ChatHandler::HandleRenameGuildCommand,	".guild rename - Renomme un guilde.",										NULL, 0, 0, 0 },
		{ "members",		'g', &ChatHandler::HandleGuildMembersCommand,	".guild members - Liste les membres de la guilde ainsi que leur rangs.",					NULL, 0, 0, 0 },
		{ "removeplayer",	'g', &ChatHandler::HandleGuildRemovePlayerCommand,".guild removeplayer - Retire un joueur de la guilde.",						NULL, 0, 0, 0 },
		{ "disband",		'g', &ChatHandler::HandleGuildDisbandCommand,	".guild disband - Dissout la guilde du joueur selectionne.",					NULL, 0, 0, 0 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0  }
	};
	dupe_command_table(GuildCommandTable, _GuildCommandTable);

	static ChatCommand GameObjectCommandTable[] =
	{
		{ "select",		 'o', &ChatHandler::HandleGOSelect,		".gobject select - Selectionne le GO le plus proche de vous.",		NULL, 0, 0, 0 },
		{ "delete",		 'o', &ChatHandler::HandleGODelete,		".gobject delete - Supprime le GO selectionne.",					NULL, 0, 0, 0 },
		{ "spawn",		 'o', &ChatHandler::HandleGOSpawn,		".gobject spawn <Id> [save <0:1>] - Fait apparaitre un GO par ID.",	NULL, 0, 0, 0 },
		{ "disp",		 'o', &ChatHandler::HandleGODisplay,	".gobject disp <DisplayId> - Change le displayId d'un Go.",			NULL, 0, 0, 0 },
		{ "damage",		 'o', &ChatHandler::HandleGODamage,	    ".gobject damage <Damage> , -1 affiche Health/MaxHealth, 0 restore le Go.",NULL, 0, 0, 0 },
		{ "info",		 'o', &ChatHandler::HandleGOInfo,		".gobject info - Affiche les information du GO selectionne.",		NULL, 0, 0, 0 },
		{ "activate",	 'o', &ChatHandler::HandleGOActivate,	".gobject activate - Active/Ouvre le GO selectionne.",				NULL, 0, 0, 0 },
		{ "enable",		 'o', &ChatHandler::HandleGOEnable,		".gobject enable - Active le GO selectionne pour utilisation.",		NULL, 0, 0, 0 },
		{ "scale",		 'o', &ChatHandler::HandleGOScale,		".gobject scale <[0:1]> (float) - Defini la taille.",				NULL, 0, 0, 0 },
		{ "animprogress",'o', &ChatHandler::HandleGOAnimProgress,".gobject setanimprogress <val> - Defini la progression de l'animation.",									NULL, 0, 0, 0 },
		{ "export",		 'o', &ChatHandler::HandleGOExport,		".gobject export - Exporte le GO selectionne.",						NULL, 0, 0, 0 },
		{ "move",		 'o', &ChatHandler::HandleGOMove,		".gobject move <x> <y> <z> - Bouge le GO vers un joueur situe aux coordonnees mentionnees.",						NULL, 0, 0, 0 },
		{ "rotate",		 'o', &ChatHandler::HandleGORotate,		".gobject rotate <axe> <x> <y> <o> <valeur_degre [0:1]> - Effectue une rotation.",NULL, 0, 0, 0 },
		{ NULL,0, NULL,"",NULL, 0, 0  } 
	};
	dupe_command_table(GameObjectCommandTable, _GameObjectCommandTable);

	static ChatCommand BattlegroundCommandTable[] = 
	{
		{ "setbgscore",		'e', &ChatHandler::HandleSetBGScoreCommand,		".battleground setbgscore <TeamID> <Score> - Defini le score du champ de bataille.",			NULL, 0, 0, 0 },
		{ "startbg",		'e', &ChatHandler::HandleStartBGCommand,		".battleground startbg - Demarre le champ de bataille actuel.",					NULL, 0, 0, 0 },
		{ "pausebg",		'e', &ChatHandler::HandlePauseBGCommand,		".battleground pausebg - Mets en pause le champ de bataille actuel.",					NULL, 0, 0, 0 },
		{ "bginfo",			'e', &ChatHandler::HandleBGInfoCommnad,			".battleground bginfo - Affiche les information du champ de bataille actuel.",		NULL, 0, 0, 0 },
		{ "battleground",	'e', &ChatHandler::HandleBattlegroundCommand,	".battleground battleground - Affiche le menu du champ de bataille.",										NULL, 0, 0, 0 },
		{ "setworldstate",	'e', &ChatHandler::HandleSetWorldStateCommand,	".battleground setworldstate <var> <val> - var peut etre une valeur Hexadecimale.",			NULL, 0, 0, 0 },
		{ "playsound",		'e', &ChatHandler::HandlePlaySoundCommand,		".battleground playsound <val> - val peut etre une valeur Hexadecimale.",							NULL, 0, 0, 0 },
		//{ "setbfstatus",	'e', &ChatHandler::HandleSetBattlefieldStatusCommand,".battleground setbfstatus -  Defini le status du BG - Pas encore implante.",								NULL, 0, 0, 0 },
		{ "leave",			'e', &ChatHandler::HandleBattlegroundExitCommand,".battleground leave - Quitte le champ de bataille actuel.",					NULL, 0, 0, 0 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0  }
	};
	dupe_command_table(BattlegroundCommandTable, _BattlegroundCommandTable);

	static ChatCommand NPCCommandTable[] =
	{
		{ "vendoradditem",	'n', &ChatHandler::HandleItemCommand,			".npc vendoradditem - Ajoute un item au vendeur selectionne.",										NULL, 0, 0, 0 },
		{ "vendorremoveitem",'n', &ChatHandler::HandleItemRemoveCommand,	".npc vendorremoveitem - Retire un objet du vendeur selectionne.",									NULL, 0, 0, 0 },
		{ "repop",			'n', &ChatHandler::HandleRespawnCommand,			".npc repop - Respawn le pnj selectionne.",										NULL, 0, 0, 0 },
		{ "flags",			'n', &ChatHandler::HandleNPCFlagCommand,		".npc flags <id> - Change les Flags de la creature selectionne.",									NULL, 0, 0, 0 },
		{ "emote",			'n', &ChatHandler::HandleEmoteCommand,			".npc emote <id> - Defini l'emote de la creature selectionne.",							NULL, 0, 0, 0 },
		{ "delete",			'n', &ChatHandler::HandleDeleteCommand,		".npc delete - Supprime la creature selectionnee du monde et de la BDD.",						NULL, 0, 0, 0 },
		{ "info",			'n', &ChatHandler::HandleNpcInfoCommand,		".npc info - Affiche les information de la creature selectionne.",								NULL, 0, 0, 0 },
		{ "addagent",		'n', &ChatHandler::HandleAddAIAgentCommand,		".npc addAgent <Type> <Event> <Chance> <MaxCount> <Spell> <SpellType> <TargetType> <CoolDown> <FloatMisc1> <Misc2> [Record : <0/1>]", NULL, 0, 0, 0},
		{ "listagent",		'n', &ChatHandler::HandleListAIAgentCommand,	".npc listagent - Etablit la liste ai_agent du PNJ selectionne.",NULL, 0, 0, 0 },
		{ "listagentbase",  'n', &ChatHandler::HandleListAIAgentBaseCommand,".npc listagentbase - Etablit la liste ai_agent du PNJ selectionne lu dans la base.",NULL, 0, 0, 0 },
		{ "targettype",		'n', &ChatHandler::HandleTargetTypeAIAgentCommand,".npc targettype <AISpell> <TargetType> - Change le type de cible pour un AISpell du pnj",NULL, 0, 0, 0 },
		{ "say",			'n', &ChatHandler::HandleMonsterSayCommand,		".npc say <text> - Fait parler la creature selectionnee.",NULL, 0, 0, 0 },
		{ "yell",			'n', &ChatHandler::HandleMonsterYellCommand,	".npc yell <text> - Fait crier la creature selectionnee.", NULL, 0, 0, 0},
		{ "come",			'n', &ChatHandler::HandleNpcComeCommand,		".npc come - Fait bouger le PNJ selectionne vers votre position.",			NULL, 0, 0, 0 },
		{ "castself",		'n', &ChatHandler::HandleNpcCastSelfCommand,	".npc castself <SpellId> - Fait incanter la creature un sort sur elle meme.", NULL, 0, 0, 0 },
		{ "castme",			'n', &ChatHandler::HandleNpcCastMeCommand,		".npc castme <SpellId> - Fait incanter a la creature selectionnee, un sort sur vous.", NULL, 0, 0, 0 },
		{ "return",			'n', &ChatHandler::HandleNpcReturnCommand,		".npc return - Fait retourner la creature selectionnee a sa position d'origine.",				NULL, 0, 0, 0 },
		{ "spawn",			'n', &ChatHandler::HandleCreatureSpawnCommand,	".npc spawn <id> - Fait apparaître une creature sur votre position.",				NULL, 0, 0, 0 },
		{ "spawnlink",		'n', &ChatHandler::HandleNpcSpawnLinkCommand,	".npc spawnlink <sqlentry> - Pas de description.",									NULL, 0, 0, 0 },
		{ "speaker",		'n', &ChatHandler::HandleNpcSpeakCommand,	    ".npc speaker <1:0> - Le pnj peut parler en langage naturel (ou pas)", NULL, 0, 0, 0 },
		{ "possess",		'n', &ChatHandler::HandleNpcPossessCommand,		".npc possess - Controle la creature selectionnee.",			NULL, 0, 0, 0 },
		{ "unpossess",		'n', &ChatHandler::HandleNpcUnPossessCommand,	".npc unpossess - Arrete le controle en cours sur la creature.", NULL, 0, 0, 0 },
		{ "select",			'n', &ChatHandler::HandleNpcSelectCommand,		".npc select - Selectionne la creature la plus proche.",					NULL, 0, 0, 0 },
		{ "setphase",       'n',&ChatHandler::HandleCreatureSetPhasing,    	".npc setphase <phasing> [0:1]",NULL, 0, 0, 0 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0  }
	};
	dupe_command_table(NPCCommandTable, _NPCCommandTable);

	static ChatCommand CheatCommandTable[] =
	{
		{ "status",			'C', &ChatHandler::HandleShowCheatsCommand,		".cheat status - Montre les triches actives.",									NULL, 0, 0, 0 },
		{ "taxi",			'C', &ChatHandler::HandleTaxiCheatCommand,		".cheat taxi - Active tous les points de vol.",								NULL, 0, 0, 0 },
		{ "cooldown",		'C', &ChatHandler::HandleCooldownCheatCommand,	".cheat cooldown - Active le mode : aucun temps de recharge.",							NULL, 0, 0, 0 },
		{ "casttime",		'C', &ChatHandler::HandleCastTimeCheatCommand,	".cheat casttime - Active le mode : aucun temps d'incantation.",							NULL, 0, 0, 0 },
		{ "power",			'C', &ChatHandler::HandlePowerCheatCommand,		".cheat power - Active le mode : aucun cout des sorts et techniques.",						NULL, 0, 0, 0 },
		{ "god",			'C', &ChatHandler::HandleGodModeCommand,		".cheat god - Active le mode dieu.",		NULL, 0, 0, 0 },
		{ "fly",			'C', &ChatHandler::HandleFlyCommand,			".cheat fly - Active le mode vol.",										NULL, 0, 0, 0 },
		{ "land",			'C', &ChatHandler::HandleLandCommand,			".cheat land - Desactive le mode vol.",										NULL, 0, 0, 0 },
		{ "explore",		'C', &ChatHandler::HandleExploreCheatCommand,	".cheat explore - Revele tous les endroits inexplores de la carte.",				NULL, 0, 0, 0 },
		{ "flyspeed",		'C', &ChatHandler::HandleFlySpeedCheatCommand,	".cheat flyspeed <val> - Modifie la vitesse en vol.",									NULL, 0, 0, 0 },
		{ "stack",			'C', &ChatHandler::HandleStackCheatCommand,		".cheat stack - Autorise le cumul des auras.",							NULL, 0, 0, 0 },
		{ "triggerpass",	'C', &ChatHandler::HandleTriggerpassCheatCommand,".cheat triggerpass - Ignore les prerequis des areatriggers.",					NULL, 0, 0, 0 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0, 0 },
	};
	dupe_command_table(CheatCommandTable, _CheatCommandTable);

	static ChatCommand accountCommandTable[] =
	{
		{ "ban",			'a', &ChatHandler::HandleAccountBannedCommand,	".account ban <id> <timeperiod> - Bannit le compte pour un temps determine.",			NULL, 0, 0, 0 },
		{ "unban",			'A', &ChatHandler::HandleAccountUnbanCommand,	".account unban <id> - Debannit le compte.",									NULL, 0, 0, 0 },
		{ "level",			'A', &ChatHandler::HandleAccountLevelCommand,	".acccount level <val> - Defini le niveau du compte.",							NULL, 0, 0, 0 },
		{ "mute",			'a', &ChatHandler::HandleAccountMuteCommand,	".acccount mute <id> <timeperiod> - Active le mode mute sur un compte pour un temps determine.",						NULL, 0, 0, 0 },
		{ "unmute",			'a', &ChatHandler::HandleAccountUnmuteCommand,	".account unmute <id> - Desactive le mode mute du compte.",									NULL, 0, 0, 0 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0, 0},
	};
	dupe_command_table(accountCommandTable, _accountCommandTable);

	static ChatCommand honorCommandTable[] =
	{
		{ "addpoints",		'h', &ChatHandler::HandleAddHonorCommand,		".honor addpoints <val> - Ajoute des points d'honneur au joueur selectionne.",				NULL, 0, 0, 0 },
		{ "addkills",		'h', &ChatHandler::HandleAddKillCommand,		".honor addkills <val> - Ajoute un montant de tues au joueur selectionne.",							NULL, 0, 0, 0 },
		{ "globaldailyupdate",'h', &ChatHandler::HandleGlobalHonorDailyMaintenanceCommand,".honor globaldailyupdate - Valide les points d'honneur gagne dans la journee de tous les joueurs.",				NULL, 0, 0, 0 },
		{ "singledailyupdate",'h', &ChatHandler::HandleNextDayCommand,		".honor singledailyupdate - Valide les points d'honneur gagne dans la journee du joueur selectionne.",		NULL, 0, 0, 0 },
		{ "pvpcredit",		'h', &ChatHandler::HandlePVPCreditCommand,		".honor pvpcredit - Sends PVP credit packet, with rank and points.",		NULL, 0, 0, 0 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0, 0 },
	};
	dupe_command_table(honorCommandTable, _honorCommandTable);

	static ChatCommand petCommandTable[] = 
	{
		{ "create",			'P', &ChatHandler::HandleCreatePetCommand,		".pet create <entry> - Cree un pet avec une ID.",		NULL, 0, 0, 0 },
		{ "rename",			'P', &ChatHandler::HandleRenamePetCommand,		".pet rename <name> - Renomme le pet.",					NULL, 0, 0, 0 },
		{ "addspell",		'P', &ChatHandler::HandleAddPetSpellCommand,	".pet addspell <spell> - Apprend un sort au pet.",		NULL, 0, 0, 0 },
		{ "removespell",	'P', &ChatHandler::HandleRemovePetSpellCommand,	".pet removespell <spell> - Desapprend un sort au pet.",NULL, 0, 0, 0 },
		{ "spawnbot",		'P', &ChatHandler::HandlePetSpawnAIBot,			".pet spawnbot <type> - Fait apparaitre un bot pour vous aider.",NULL, 0, 0, 0 },

		{ NULL,				0, NULL,										"",														NULL, 0, 0, 0},
	};
	dupe_command_table(petCommandTable, _petCommandTable);

	static ChatCommand recallCommandTable[] =
	{
		{ "list",			't', &ChatHandler::HandleRecallListCommand,		".recall list - Liste les destinations possibles.",								NULL, 0, 0, 0},
		{ "port",			't', &ChatHandler::HandleRecallGoCommand,		".recall port <name> - Teleporte a la destination mentionne.",							NULL, 0, 0, 0},
		{ "add",			'T', &ChatHandler::HandleRecallAddCommand,		".recall add <name> - Ajoute une destination a partir de votre position. ",									NULL, 0, 0, 0},
		{ "del",			'T', &ChatHandler::HandleRecallDelCommand,		".recall del <name> - Retire une destination de la liste.",								NULL, 0, 0, 0},
		{ "portplayer",		'T', &ChatHandler::HandleRecallPortPlayerCommand,".recall portplayer <name> <dest> - Teleporte un joueur a la destination mentionnee.",									NULL, 0, 0, 0},
		{ NULL,				0, NULL,										"",														NULL, 0, 0, 0},
	};
	dupe_command_table(recallCommandTable, _recallCommandTable);

	static ChatCommand questCommandTable[] =
	{
		{ "addboth",		'q', &ChatHandler::HandleQuestAddBothCommand,	".quest add <entry> - Ajoute une quete au PNJ selectionne en tant que debuteur et finisseur.",	NULL, 0, 0, 0},
		{ "addfinish",		'q', &ChatHandler::HandleQuestAddFinishCommand,	".quest addfinish <entry> - Ajoute une quete au PNJ selectionne en tant que finisseur.",		NULL, 0, 0, 0},
		{ "addstart",		'q', &ChatHandler::HandleQuestAddStartCommand,	".quest addstart <entry> - Add quest <id> to the targeted NPC as starter",		NULL, 0, 0, 0},
		{ "delboth",		'q', &ChatHandler::HandleQuestDelBothCommand,	".quest delboth <entry> - Supprime la quete du PNJ selectionne en tant que debuteur et finisseur.",	NULL, 0, 0, 0},
		{ "delfinish",		'q', &ChatHandler::HandleQuestDelFinishCommand,	".quest delfinish <entry> - Supprime la quete du PNJ selectionne en tant que finisseur.",	NULL, 0, 0, 0},
		{ "delstart",		'q', &ChatHandler::HandleQuestDelStartCommand,	".quest delstart <entry> - Supprime la quete du PNJ selectionne en tant que debuteur.",	NULL, 0, 0, 0},
		{ "complete",		'q', &ChatHandler::HandleQuestFinishCommand,	".quest complete <entry> - Termine la quete.",							NULL, 0, 0, 0},
		{ "finisher",		'q', &ChatHandler::HandleQuestFinisherCommand,	".quest finisher <entry> - Affiche le finisseur de la quete.",					NULL, 0, 0, 0},
		{ "item",			'q', &ChatHandler::HandleQuestItemCommand,		".quest item <entry> - Affiche l'objet necessaire pour finir la quete.",				NULL, 0, 0, 0},
		{ "list",			'q', &ChatHandler::HandleQuestListCommand,		".quest list <entry> - Liste les quetes disponible sur le PNJ selectionne.",					NULL, 0, 0, 0},
		{ "load",			'q', &ChatHandler::HandleQuestLoadCommand,		".quest load - Charge les quetes de la base de donnee.",							NULL, 0, 0, 0},
		{ "giver",			'q', &ChatHandler::HandleQuestGiverCommand,		".quest giver <entry> - Affiche le donneur de cette quete.",					NULL, 0, 0, 0},
		{ "remove",			'q', &ChatHandler::HandleQuestRemoveCommand,	".quest remove <entry> - Retire la quete du journal au joueur selectionne.",		NULL, 0, 0, 0},
		{ "reward",			'q', &ChatHandler::HandleQuestRewardCommand,	".quest reward <entry> - Montre les recompenses de la quete.",							NULL, 0, 0, 0},
		{ "status",			'q', &ChatHandler::HandleQuestStatusCommand,	".quest status <entry> - Affiche le status de la quete.",						NULL, 0, 0, 0},
		{ "spawn",			'q', &ChatHandler::HandleQuestSpawnCommand,		".quest spawn <entry> - Teleporte a l'endroit concerne par la quete.",				NULL, 0, 0, 0},
		{ "start",			'q', &ChatHandler::HandleQuestStartCommand,		".quest start <entry> - Demarre la quete.",									NULL, 0, 0, 0},
		{ NULL,				0,  NULL,										"",														NULL, 0, 0, 0},
	};
	dupe_command_table(questCommandTable, _questCommandTable);

    static ChatCommand charCommandTable[] =
	{
		{ "levelup",		'm', &ChatHandler::HandleLevelUpCommand,		".character levelup - Passe le player au level suivant.",										NULL, 0, 0, 0 },
		{ "renamechar",		'M', &ChatHandler::HandleRenameCommand,			".character renamechar <name1> <name2> - Rennomme le joueur.",							NULL, 0, 0, 0 },
		{ "forcerenamechar",'M', &ChatHandler::HandleForceRenameCommand,	".character forcerenamechar <name> - Force le joueur a se renommer a sa prochaine authentification.",		NULL, 0, 0, 0 },
		{ "removeitem",		'm', &ChatHandler::HandleRemoveItemCommand,		".character removeitem <entry> <count> - Retire un item avec son montant dans l'inventaire du joueur selectionne.",							NULL, 0, 0, 0 },
		{ "resetreputation",'m', &ChatHandler::HandleResetReputationCommand,".character resetreputation - Reinitialise la reputation du joueur tels qu'ils etaient a sa creation.", NULL, 0, 0, 0},
		{ "resetspells",	'm', &ChatHandler::HandleResetSpellsCommand,	".character resetspells - Reinitialise tous les sorts du joueur tels qu'ils etaient a sa creation.", NULL, 0, 0, 0 },
		{ "resettalents",	'm', &ChatHandler::HandleResetTalentsCommand,	".character resettalents - Reinitialise les talents du joueur et lui reattribue ses points.", NULL, 0, 0, 0 },
		{ "resetskills",	'm', &ChatHandler::HandleResetSkillsCommand ,	".character resetskills - Reinitialise toutes les competences du joueur.",					NULL, 0, 0, 0 },
		{ "learn",			'm', &ChatHandler::HandleLearnCommand,			".character learn <spellID> - Apprend un sort au joueur selectionne.",											NULL, 0, 0, 0 },
		{ "unlearn",		'm', &ChatHandler::HandleUnlearnCommand,		".character unlearn <spellID> - Desapprend un sort au joueur selectionne.",										NULL, 0, 0, 0 },
		{ "getskilllevel",	'm', &ChatHandler::HandleGetSkillLevelCommand,	".character getskilllevel <skillID> - Montre le niveau de la competence du joueur selectionne.",					NULL, 0, 0, 0 }, //DGM (maybe add to playerinfo?)
        { "getskillinfo",	'm', &ChatHandler::HandleGetSkillsInfoCommand,	".character getskillinfo <skillID> - Liste toutes les competences du joueur selectionne.",					NULL, 0, 0, 0 },
		{ "learnskill",		'm', &ChatHandler::HandleLearnSkillCommand,		".character learnskill <skillID> (optionnel) <value> <maxvalue> - Apprend la competence au joueur selectionne.", NULL, 0, 0, 0},
		{ "advanceskill",	'm', &ChatHandler::HandleModifySkillCommand,	".character advanceskill <skillID> <count> (optionnel, par defaut : 1) - Avance le niveau de la competence d'un montant mentionne.", NULL, 0, 0, 0},
		{ "removeskill",	'm', &ChatHandler::HandleRemoveSkillCommand,	".character removeskill <skillID> - Supprime la competence du joueur selectionne.",				NULL, 0, 0, 0 },
		{ "increaseweaponskill",'m', &ChatHandler::HandleIncreaseWeaponSkill,".character increaseweaponskill <count> - Augmente le niveau de competence de l'arme actuelle equipee du joueur d'un montant mentionne.", NULL, 0, 0, 0},
		{ "additem",		'm', &ChatHandler::HandleAddInvItemCommand,		"",														NULL, 0, 0, 0 },
		{ "additemset",		'm', &ChatHandler::HandleAddItemSetCommand,		".character additemset <entry> - Ajoute un ensemble d'objet dans l'inventaire du joueur selectionne.",								NULL, 0, 0, 0 },
		{ "addfriend",		'm', &ChatHandler::HandleAddFriendCommand,		".character addfriend <name1> <name2> - Ajoute un joueur dans la liste d'ami du joueur.",					NULL, 0, 0, 0 },	
		{ "addaura",        'd', &ChatHandler::HandleAddPlayerAura,         ".character addaura <auraID> - Ajoute l'aura sur un joueur selectionne.",NULL,0,0,0 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0  }
	};
	dupe_command_table(charCommandTable, _charCommandTable);

	static ChatCommand worldannounceCommandTable[] =
	{
		{ "list",			'W', &ChatHandler::HandleWorldAnnounceList,		".worldannounce list - Liste les annonces automatiques.",						NULL, 0, 0, 0 },
		{ "start",			'W', &ChatHandler::HandleWorldAnnounceStart,	".worldannounce start - Redemarre le systeme d'annonces.",						NULL, 0, 0, 0 },
		{ "stop",			'W', &ChatHandler::HandleWorldAnnounceStop,		".worldannounce stop - Arrete le systeme d'annonces.",						NULL, 0, 0, 0 },
		{ "create",			'a', &ChatHandler::HandleWorldAnnounceCreate,	".worldannounce create - Creation d'une annonce.",								NULL, 0, 0, 0 },
		{ "modify",			'a', &ChatHandler::HandleWorldAnnounceModify,	".worldannounce modify - Modification d'une annonce.",							NULL, 0, 0, 0 },
		{ "remove",			'a', &ChatHandler::HandleWorldAnnounceRemove,	".worldannounce remove - Suppression d'une annonce.",							NULL, 0, 0, 0 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0  }
	};
	dupe_command_table(worldannounceCommandTable, _worldannounceCommandTable);

	static ChatCommand serverCommandTable[] =
	{
		{ "motd",			'S', &ChatHandler::HandleSetMotdCommand,		".motd <content> - Defini le message du jour.",							NULL, 0, 0, 0 },
		{ "reloadtable",	'S', &ChatHandler::HandleDBReloadCommand,		".reloadtable <table> - Recharge quelques tables SQL.",							NULL, 0, 0, 0 },
		{ "rehash",			'A', &ChatHandler::HandleRehashCommand,			".rehash - Recharge les fichiers de configuration.",				NULL, 0, 0, 0 },
		{ "shutdown",		'A', &ChatHandler::HandleShutdownCommand,		".shutdown <time> (en secondes) - Lance l'arret du serveur.",						NULL, 0, 0, 0 },
		{ "restart",		'A', &ChatHandler::HandleShutdownRestartCommand,".restart <time> (en secondes) - Lance le redemarrage du serveur.",				NULL, 0, 0, 0 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0  }
	};
	dupe_command_table(serverCommandTable, _serverCommandTable);

	static ChatCommand lookupCommandTable[] =
	{
		{ "item",			'L', &ChatHandler::HandleLookupItemCommand,		".lookup item <itemID or name> - Affiche les objets comprennant ce nom ou cette ID.",								NULL, 0, 0, 0 },
		{ "quest",			'L', &ChatHandler::HandleQuestLookupCommand,	".lookup quest <questID or name> - Affiche les quetes comprennant ce nom ou cette ID.",								NULL, 0, 0, 0 },
		{ "creature",		'L', &ChatHandler::HandleLookupCreatureCommand,	".lookup creature <creatureID or name> - Affiche les creatures comprennant ce nom ou cette ID.",							NULL, 0, 0, 0 },
		{ "spell",			'L', &ChatHandler::HandleLookupSpellCommand,	".lookup spell <spellID or name> - Affiche les sorts comprennant ce nom ou cette ID.",					NULL, 0, 0, 0 },
		{ "achievement",	'L', &ChatHandler::HandleLookupAchievementCommand,".lookup achievement <achID or name> - Affiche les sorts comprennant ce nom ou cette ID.",					NULL, 0, 0, 0 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0  }
	}; 
	dupe_command_table(lookupCommandTable, _lookupCommandTable);

	static ChatCommand achCommandTable[] =
	{
		{ "complete",'G', &ChatHandler::HandleAchievementComplete,".achievement complete <achID> - Termine le haut-fait du joueur selectionne.",					NULL, 0, 0, 0 },
		{ "name",    'G', &ChatHandler::HandleAchievementGetName,".achievement name <achID> - Donne le nom du haut fait.",NULL, 0, 0, 0 },
		{ NULL,				0, NULL,										"",														NULL, 0, 0  }
	}; 
	dupe_command_table(achCommandTable, _achCommandTable);

	static ChatCommand commandTable[] = {
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Commandes de bases accessibles a tout personne avec au moins 1 flag d'accès.
		{ "commands",		'\0', &ChatHandler::HandleCommandsCommand,		".commands - Liste des commandes.",									NULL, 0, 0, 0 },
		{ "help",			'\0', &ChatHandler::HandleHelpCommand,			".help - Montre l'aide des commandes.",							NULL, 0, 0, 0 },
		{ "gm",				'\0', &ChatHandler::HandleGMListCommand,		".gm - Montre les GMs.",										NULL, 0, 0, 0 },
		{ "gmlist",			'\0', &ChatHandler::HandleGMListCommand,		".gmlist - Montre les GMs.",										NULL, 0, 0, 0 },
		{ "gps",			'\0', &ChatHandler::HandleGPSCommand,			".gps - Affiche votre position ou celle d'un joueur/creature selectionne.",									NULL, 0, 0, 0 },
		{ "info",			'\0', &ChatHandler::HandleInfoCommand,			".info - Montre les infos serveur.",								NULL, 0, 0, 0 },
		{ "logcomment" ,	'\0', &ChatHandler::HandleGmLogCommentCommand,	".logcomment <content> - Ajoute une notification aux logs Admin pour etre lus.",NULL, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Deplacements et teleportations
		{ "appear",			't', &ChatHandler::HandleAppearCommand,			".appear <playername> - Vous teleporte a la position du joueur.",					NULL, 0, 0, 0 },
		{ "summon",			'T', &ChatHandler::HandleSummonCommand,			".summon <playername> - Invoque un joueur sur votre position.",							NULL, 0, 0, 0 },
		{ "start",			'T', &ChatHandler::HandleStartCommand,			".start <id> - Vous teleporte a la position de depart.",				NULL, 0, 0, 0 },
		{ "masssummon",		'A', &ChatHandler::HandleMassSummonCommand,		".massummon - Invoque tous les joueurs vers votre position.",		NULL, 0, 0, 0 },
		{ "gotrig",			'v', &ChatHandler::HandleTriggerCommand,		".gotrig <areatriggerId> - Enchaine l'effet de l'areatrigger.",	     		NULL, 0, 0, 0 },
		{ "worldport",		'v', &ChatHandler::HandleWorldPortCommand,		".worldport <MapId> <x> <y> <z> - Vous teleporte aux coordonnees mentionnees.",NULL, 0, 0, 0 },
		{ "exitinstance",	'T', &ChatHandler::HandleExitInstanceCommand,	".exitinstance - Sortir de l'instance.",								NULL, 0, 0, 0 },
		{ "recall",			't', NULL,										"",														recallCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des annonces
		{ "announce",		'u', &ChatHandler::HandleAnnounceCommand,		".announce <content> - Envoie une annonce en chat.",							NULL, 0, 0, 0 },
		{ "wannounce",		'u', &ChatHandler::HandleWAnnounceCommand,		".wannounce <content> - Envoie une annonce en plein ecran.",					NULL, 0, 0, 0 },
		{ "gmannounce",		'u', &ChatHandler::HandleGMAnnounceCommand,		".gmannounce <content> - Envoie une annonce a tous les GMs en ligne.",			NULL, 0, 0, 0 },
		{ "worldannounce",	'W', NULL,										"",	worldannounceCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des tags GM pour les ayants droits et des whispers
		{ "gmoff",			'c', &ChatHandler::HandleGMOffCommand,			".gmoff - Retire le tag <Mj>.",										NULL, 0, 0, 0 },
		{ "gmon",			'c', &ChatHandler::HandleGMOnCommand,			".gmon - Active le tag <Mj>.",										NULL, 0, 0, 0 },
		{ "allowwhispers",	'c', &ChatHandler::HandleAllowWhispersCommand,	".allowwhispers <playername> - Autorise les chuchotements du joueur en mode <Mj>.",				NULL, 0, 0, 0 },
		{ "blockwhispers",	'c', &ChatHandler::HandleBlockWhispersCommand,	".blockwhispers <playername> - Bloque les chuchotements du joueur en mode <Mj>.",				NULL, 0, 0, 0 },
		{ "whisperblock",	'A', &ChatHandler::HandleWhisperBlockCommand,	".whisperblock - Bloque tout le monde sauf les GMs.",					NULL, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des punitions pour les GM
		{ "revive",			'r', &ChatHandler::HandleReviveCommand,			".revive <target> (optionnel) - Ressucite (joueur/NPC/creature).",										NULL, 0, 0, 0 },
		{ "reviveplr",		'r', &ChatHandler::HandleReviveStringcommand,	".reviveplr <playername> - Resurrection du joueur specifie.",						NULL, 0, 0, 0 },
		{ "removesickness",	'R', &ChatHandler::HandleRemoveRessurectionSickessAuraCommand,	".removesickness <target> - Supprime le mal de resurrection de la cible.",	NULL, 0, 0, 0 },
		{ "kill",			'R', &ChatHandler::HandleKillCommand,			".kill - Tue l'unite selectionnee (joueur/NPC/creature).",		NULL, 0, 0, 0 },
		{ "killplr" ,		'R', &ChatHandler::HandleKillByPlrCommand,		".killplr <name> - Tue le joueur specifie.",			NULL, 0, 0, 0 },
		{ "paralyze",		'R', &ChatHandler::HandleParalyzeCommand,		".paralyze - Immobilise la cible.",							NULL, 0, 0, 0 },
		{ "unparalyze",		'R', &ChatHandler::HandleUnParalyzeCommand,		".unparalyze - Retire l'immobilisation sur la cible.",						NULL, 0, 0, 0 },
		{ "kick",			'k', &ChatHandler::HandleKickCommand,			".kick - Kicke le joueur du serveur.",								NULL, 0, 0, 0 },
		{ "killbyplayer",	'k', &ChatHandler::HandleKillByPlayerCommand,	".killbyplayer <playername> - Deconnecte le joueur.",				NULL, 0, 0, 0 },
		{ "killbyaccount",	'k', &ChatHandler::HandleKillBySessionCommand,	".killbyaccount <accountname> - Deconnecte la session du compte.",		NULL, 0, 0, 0 },
		{ "killbyip",		'B', &ChatHandler::HandleKillByIPCommand,		".killbyip <ipadress> - Deconnecte le joueur a partir de son adresse ip.",				NULL, 0, 0, 0 },
		{ "banchar",		'b', &ChatHandler::HandleBanCharacterCommand,	".banchar <name> <reason> - Bannit le personnage avec ou sans raison.",				NULL, 0, 0, 0 },
		{ "unbanchar",		'b', &ChatHandler::HandleUnBanCharacterCommand,	".unbanchar <name> - Debannit le personnage.",									NULL, 0, 0, 0 },
		{ "addipban",		'B', &ChatHandler::HandleIPBanCommand,			".addipban <ipadress> <duration> : h=heures, d=jours, w=semaines, m=mois, y=annees, par defaut : minutes> - Ajoute une adresse IP dans la table des bannissements. Si aucune mention du temps est faite, l'IP sera bannie definitivement.", NULL, 0, 0, 0 },
		{ "delipban",		'B', &ChatHandler::HandleIPUnBanCommand,		".delipban <ipadress> - Supprime l'adresse IP de la table des bannissements.",	NULL, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des morphings et montures HandleMorphCommand
		{ "morph",		    'm', &ChatHandler::HandleMorphCommand,	 ".morph - Change la forme la cible ou le joueur.",						NULL, 0, 0, 0 },
		{ "demorph",		'm', &ChatHandler::HandleDeMorphCommand, ".demorph - Retire le changeforme precedement applique sur le joueur.",NULL, 0, 0, 0 },
		{ "mount",			'm', &ChatHandler::HandleMountCommand,	 ".mount <modelID> - Monte le joueur sur une creature.",				NULL, 0, 0, 0 },
		{ "dismount",		'm', &ChatHandler::HandleDismountCommand,".dismount - Descend le joueur de sa monture actuelle.",				NULL, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des sauvegardes
		{ "save",			's', &ChatHandler::HandleSaveCommand,			".save - Sauvegarde dans la base de donnee toutes les informations relatives a votre personnage.",								NULL, 0, 0, 0 },
		{ "saveall",		's', &ChatHandler::HandleSaveAllCommand,		".saveall - Sauvegarde dans la base de donnee toutes les informations relatives aux joueurs actuellement connectes.",						NULL, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Cheating
		{ "invincible",		'i', &ChatHandler::HandleInvincibleCommand,		".invincible - Rend insensible a tous les degats.", NULL, 0, 0, 0 },
		{ "invisible",		'I', &ChatHandler::HandleInvisibleCommand,		".invisible - Red invisible et insensible a tous les degats.)", NULL, 0, 0, 0 },
		{ "cheat",			'C', NULL,										"",														CheatCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Informations et comptes joueurs
		{ "playerinfo",		'p', &ChatHandler::HandlePlayerInfo,			".playerinfo <playername> - Affiche les informations du joueur.", NULL, 0, 0, 0 },
		{ "account",		'a', NULL,										"",														accountCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Modification sur le joueur et ajout d'item
		{ "additem",		'm', &ChatHandler::HandleAddInvItemCommand,		".additem <itemID> <count> - Ajoute un objet avec son montant dans l'inventaire du joueur selectionne.",														NULL, 0, 0, 0 },
		{ "showitems",		'm', &ChatHandler::HandleShowItems,				".showitems - Test pour ItemIterator.",								NULL, 0, 0, 0 },
		{ "advanceallskills",'m', &ChatHandler::HandleAdvanceAllSkillsCommand, ".advanceallskills <count> - Avance toutes les competences d'un montant de niveaux.",					NULL, 0, 0, 0 },
		{ "modify",			'm', NULL,										"",														modifyCommandTable, 0, 0, 0 },
		{ "character",		'm', NULL,										"",														charCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion de l'honneur
		{ "honor",			'h', NULL,										"",														honorCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des reputations
		{ "getstanding",	'f', &ChatHandler::HandleGetStandingCommand,	"Gets standing of faction %u.",							NULL, 0, 0, 0 },
		{ "setstanding",	'f', &ChatHandler::HandleSetStandingCommand,	"Sets stanging of faction %u.",							NULL, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des waypoints
		{ "waypoint",		'w', NULL,										"",														waypointCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Commandes de debogage... A utiliser avec extreme precaution.
		{ "getpos",			'd', &ChatHandler::HandleGetPosCommand,			"",														NULL, 0, 0, 0 },
		{ "testlos",		'd', &ChatHandler::HandleCollisionTestLOS,		"tests los",											NULL, 0, 0, 0 },
		{ "testindoor",		'd', &ChatHandler::HandleCollisionTestIndoor,	"tests indoor",											NULL, 0, 0, 0 },
		{ "castspell",		'd', &ChatHandler::HandleCastSpellCommand,		".castspell <spellID> - Incante un sort sur la cible.",		NULL, 0, 0, 0 },
		{ "debug",			'd', NULL,"",debugCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Tickets GM
		{ "gmticket",		'G', NULL,"",GMTicketCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des Battle Ground
		{ "createarenateam",'E', &ChatHandler::HandleCreateArenaTeamCommands,".createarenateam - Cree une equipe d'arene.",									NULL, 0, 0, 0 },
		{ "battleground",	'e', NULL,"",BattlegroundCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des Game Objects
		{ "gobject",		'o', NULL,"",GameObjectCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des NPC
		{ "npc",			'n', NULL,"",NPCCommandTable, 0, 0, 0 }, // a def avant 'npcfollow' (Branruz)
		{ "npcfollow",		'N', &ChatHandler::HandleNpcFollowCommand,		".npcfollow - Ordonne a la creature de vous suivre.",								NULL, 0, 0, 0 },
		{ "nullfollow",		'N', &ChatHandler::HandleNullFollowCommand,		".npcunfollow - Ordonne a la creature de ne plus rien suivre.",						NULL, 0, 0, 0 },
		{ "formationlink1",	'N', &ChatHandler::HandleFormationLink1Command,	".formationlink1 - Defini le maître de la formation.",								NULL, 0, 0, 0 },
		{ "formationlink2",	'N', &ChatHandler::HandleFormationLink2Command, ".formationlink2 - Defini les esclaves de la formation avec distance et angle.",			NULL, 0, 0, 0 },
		{ "formationclear",	'N', &ChatHandler::HandleFormationClearCommand, ".formationclear - Dissout la formation.",						NULL, 0, 0, 0 },
		{ "addtrainerspell",'n', &ChatHandler::HandleAddTrainerSpellCommand,".addtrainerspell <spellID> - Ajoute un sort au maître selectionne.",														NULL, 0, 0, 0 },
		
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des quêtes
		{ "quest",			'q', NULL,"",questCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des pets pour démonistes et chasseurs
		{ "pet",			'P', NULL,"",petCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des guildes
		{ "guild",			'g', NULL,"",GuildCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion du serveur
		{ "server",			'S', NULL,"",serverCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Lookup de base de données (opérateur DB)
		{ "lookup",			'L', NULL,"",lookupCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des spells & spellcast
		{ "clearcooldowns",	'm', &ChatHandler::HandleClearCooldownsCommand, ".clearcooldowns - Reinitialise tous les temps de recharge de votre classe.",					NULL, 0, 0, 0 },
		{ "removeauras",	'm', &ChatHandler::HandleRemoveAurasCommand,	".removeauras - Retire tous les auras actifs sur la cible.",						NULL, 0, 0, 0 },
		{ "castall",		'A', &ChatHandler::HandleCastAllCommand,		".castall <spellID> - Fait incanter un sort a tous les joueurs actuellement connectes.",				NULL, 0, 0, 0 },
		{ "dispelall",		'A', &ChatHandler::HandleDispelAllCommand,		".dispellall - Desactive tous les effets negatifs et benefiques de tous les joueurs actuellement connectes.",	NULL, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Gestion des achievement
		{ "achievement",	'G', NULL,										"",														achCommandTable, 0, 0, 0 },
		/////////////////////////////////////////////////////////////////////////////////////////
		/// Commandes diverses
		{ "modperiod",		'm', &ChatHandler::HandleModPeriodCommand,		".modperiod - Change le temps de navette du transporteur actuel.",				NULL, 0, 0, 0 },
		{ "playall",		'A', &ChatHandler::HandleGlobalPlaySoundCommand,".playall <soundID> - Joue un son audible par tout le serveur.",					NULL, 0, 0, 0 },
		{ "getheight",		'm', &ChatHandler::HandleCollisionGetHeight,	".getheight - Obtient la hauteur.",											NULL, 0, 0, 0 },
		{ "renameallinvalidchars", 'A', &ChatHandler::HandleRenameAllCharacter,".renameallinvalidchars - Renomme tous les joueurs ayant des noms invalides.",				NULL, 0, 0, 0 },
		{ "fixscale",		'm', &ChatHandler::HandleFixScaleCommand,		"",														NULL, 0, 0, 0 },
		{ "ws",				'A', &ChatHandler::HandleWSCommand,				".ws <x> <y> - Defini le worldstate.",					NULL, 0, 0, 0 },		
		{ "ChatIrc",		'A', &ChatHandler::HandleChatIrc,				".ChatIrc <Text> - Envoi le texte sur le channel irc s'il existe.",NULL, 0, 0, 0 },		
		{ NULL,				0, NULL,										"",														NULL, 0, 0 }
	};
	dupe_command_table(commandTable, _commandTable);

	/* set the correct pointers */
	ChatCommand * p = &_commandTable[0];
	while(p->Name != 0)
	{
		if(p->ChildCommands != 0)
		{
			// Set the correct pointer.
			ChatCommand * np = GetSubCommandTable(p->Name);
			ASSERT(np);
			p->ChildCommands = np;
		}
		++p;
	}
}

ChatHandler::ChatHandler()
{
	new CommandTableStorage;
	CommandTableStorage::getSingleton().Init();
	SkillNameManager = new SkillNameMgr;
}

ChatHandler::~ChatHandler()
{
	CommandTableStorage::getSingleton().Dealloc();
	delete CommandTableStorage::getSingletonPtr();
	delete SkillNameManager;
}

bool ChatHandler::hasStringAbbr(const char* s1, const char* s2)
{
	for(;;)
	{
		if( !*s2 )
			return true;
		else if( !*s1 )
			return false;
		else if( tolower( *s1 ) != tolower( *s2 ) )
			return false;
		s1++; s2++;
	}
}

int ChatHandler::statStringAbbr(const char *s1, const char *s2)
{
	int score = 0;
	int l1 = (int)strlen(s1);
	int l2 = (int)strlen(s2);

	/// Si la taille de la commande entrée est trop grande, ou qu'elle est inférieure à 2 caractères on à trop peu de précision...
	if(l1 < l2 || l2 < 2 || !l1) return 0;

	int ratio = 100 / l1;

	for (int k = 0; k < l2; k++)
	{
		if (tolower( s2[k] ) != tolower( s1[k] )) return 0;
		score += ratio;
	}

	if (l2 == l1) return 100;
	return score;
}

void ChatHandler::SendMultilineMessage(WorldSession *m_session, const char *str)
{
	char * start = (char*)str, *end;
	for(;;)
	{
        end = strchr(start, '\n');
		if(!end)
			break;

		*end = '\0';
		SystemMessage(m_session, start);
		start = end + 1;
	}
	if(*start != '\0')
		SystemMessage(m_session, start);
}

bool ChatHandler::ExecuteCommandInTable(ChatCommand *table, const char* text, WorldSession *m_session, bool cache)
{
	std::string cmd = "";

	// get command
	while (*text != ' ' && *text != '\0')
	{
		cmd += *text;
		text++;
	}

	while (*text == ' ') text++; // skip whitespace

	if(!cmd.length())
		return false;

	// printf("Command: %s\n", cmd.c_str());

	int _stat = 0;
	ChatCommand *_command = NULL;
	int _ambig = 0;

#ifdef SMARTCACHE
	bool _incache = false;
	if (cache)
	{
		smart_iterator cache_com = m_Cache.find(cmd);
		if (cache_com != m_Cache.end())
		{
			_incache = true;
			_command = (*cache_com).value;
		}
	}
#endif

	if (!_command)
		for(uint32 i = 0; table[i].Name != NULL; i++)
		{
			int stat = statStringAbbr(table[i].Name, cmd.c_str());

			if(table[i].Name[0] != cmd.c_str()[0]) continue;

			if(!stat) continue;

			if(table[i].CommandGroup != '\0' && !m_session->CanUseCommand(table[i].CommandGroup)) continue;

			if (stat > _stat)
			{
				// printf(" - command candidate '%s' with %d percent.\n", table[i].Name, stat);
				_stat =		stat;
				_command =	&table[i];
				_ambig = 0;
				if (stat == 100) break; // pas la peine de continuer la recherche si on trouve la commande exacte.
			}
			else if (stat == _stat)
				++_ambig;
		}

	if (_command != NULL && _stat) 
	{
		if (_ambig)
		{
			RedSystemMessage(m_session, "Nom de commande ambigue. Le nom doit etre plus detaille.");
			return true;
		}

		if(_command->ChildCommands != NULL)
		{
			// printf(" subcommands detected ...\n");
			if(!ExecuteCommandInTable(_command->ChildCommands, text, m_session))
			{
				if(_command->Help != "")
					SendMultilineMessage(m_session, _command->Help.c_str());
				else
				{
					GreenSystemMessage(m_session, "Subcommandes disponible");
					for(uint32 k=0; _command->ChildCommands[k].Name;k++)
					{
						if(_command->ChildCommands[k].CommandGroup != '\0' && m_session->CanUseCommand(_command->ChildCommands[k].CommandGroup))
							SystemMessage(m_session, " |cffffff00%s|r |cffff0000-|r |cff00ccff%s|r", _command->ChildCommands[k].Name, _command->ChildCommands[k].Help.size() ? _command->ChildCommands[k].Help.c_str() : "Aucune aide disponible");
					}
				}
			}
#ifdef SMARTCACHE
			else if (cache && !_incache)
				m_Cache.insert(cmd, _command);
#endif
			
			return true;
		}
		
		// Check for field-based commands
		if(_command->Handler == NULL && (_command->MaxValueField || _command->NormalValueField))
		{
			bool result = false;
			if(strlen(text) == 0)
			{
				RedSystemMessage(m_session, "Aucune valeur specifiee.");
			}
			if(_command->ValueType == 2)
				result = CmdSetFloatField(m_session, _command->NormalValueField, _command->MaxValueField, _command->Name, text);
			else
				result = CmdSetValueField(m_session, _command->NormalValueField, _command->MaxValueField, _command->Name, text);
			if(!result)
				RedSystemMessage(m_session, "Doit etre de la forme (command) <value>, ou, (command) <value> <maxvalue>");
#ifdef SMARTCACHE
			else if (cache && !_incache)
				m_Cache.insert(cmd, _command);
#endif
		}
		else
		{
			if(!(this->*(_command->Handler))(text, m_session))
			{
				if(_command->Help != "")
					SendMultilineMessage(m_session, _command->Help.c_str());
				else
					RedSystemMessage(m_session, "Erreur de syntax.(.help %s)", _command->Name);
			}
#ifdef SMARTCACHE
			else if (cache && !_incache)
				m_Cache.insert(cmd, _command);
#endif
		}

		return true;
	}

	return false;
}

int ChatHandler::ParseCommands(const char* text, WorldSession *session)
{
	if (!session)
		return 0;

	if(!*text)
		return 0;

	if(text[0] != '!' && text[0] != '.') // let's not confuse users
		return 0;

	if(!session->HasPermissions() && sWorld.m_reqGmForCommands)
	{
		RedSystemMessage(session, "Tentative d'access a une commande sans autorisation...");
		return 1;
	}

	/* skip '..' :P that pisses me off */
	if(text[1] == '.')
		return 0;

	text++;

	if(!ExecuteCommandInTable(CommandTableStorage::getSingleton().Get(), text, session, true))
	{
		SystemMessage(session, "Commande inexistante, ou vous n'y avez pas acces.");
	}

	return 1;
}

WorldPacket * ChatHandler::FillMessageData( uint32 type, uint32 language, const char *message,uint64 guid , uint8 flag) const
{
	//Packet    structure
	//uint8	    type;
	//uint32	language;
	//uint64	guid;
	//uint64	guid;
	//uint32	len_of_text;
	//char	    text[];		 // not sure ? i think is null terminated .. not null terminated
	//uint8	    afk_state;


    char buffer_text[512];
	
	ASSERT(type != CHAT_MSG_CHANNEL); // Au dev de savoir ce qu'il fait....

	   //channels are handled in channel handler and so on
	//this stuff is actually done by the client on the default UI
	//unfortunatly, idiots have to make addons to get around this
	std::string strmessage=message;

	replace(strmessage, "||", "|", 1);
	replace(strmessage, "|", "||", 1);

	message=strmessage.c_str();
	
	strncpy(buffer_text,sWorld.ChangeLatin2UTF8(message),500);

	uint32 messageLength = (uint32)(strlen(buffer_text) + 1);

	WorldPacket *data = new WorldPacket(SMSG_MESSAGECHAT, messageLength + 30);

	*data << (uint8)type;
	*data << language;

	*data << guid;
	*data << uint32(0);

	*data << guid;

	*data << messageLength;
	*data << &buffer_text[0];

	*data << uint8(flag);
	return data;
}

WorldPacket* ChatHandler::FillSystemMessageData(const char *message) const
{
	char buffer_text[512];
	strncpy(buffer_text,sWorld.ChangeLatin2UTF8(message),500);
	//Log.Warning("FillSystemMessageData","%s",buffer_text);

	uint32 messageLength = (uint32)(strlen(buffer_text) + 1);
	

	WorldPacket * data = new WorldPacket(SMSG_MESSAGECHAT, 30 + messageLength);
	*data << (uint8)CHAT_MSG_SYSTEM;
	*data << (uint32)LANG_UNIVERSAL;
	
	*data << (uint64)0; // Who cares about guid when there's no nickname displayed heh ?
	*data << (uint32)0;
	*data << (uint64)0;

	*data << messageLength;
	*data << &buffer_text[0];

	*data << uint8(0);

	return data;
}

Player * ChatHandler::getSelectedChar(WorldSession *m_session, bool showerror)
{
	uint64 guid = 0;
	Player *chr=NULL;
	
	guid = m_session->GetPlayer()->GetSelection();
	
	if (guid == 0)
	{
		if(showerror) GreenSystemMessage(m_session, "Auto-sélection.");
		chr = m_session->GetPlayer(); // autoselect
	}
	else
		chr = m_session->GetPlayer()->GetMapMgr()->GetPlayer((uint32)guid);
	
	if(chr == NULL)
	{
		if(showerror) RedSystemMessage(m_session, "Vous devez sélectionner un joueur.");
		return NULL;
	}

	return chr;
}

Creature * ChatHandler::getSelectedCreature(WorldSession *m_session, bool showerror)
{
	uint64 guid;
	Creature *creature = NULL;

	guid = m_session->GetPlayer()->GetSelection();
	if(GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_PET)
		creature = m_session->GetPlayer()->GetMapMgr()->GetPet( GET_LOWGUID_PART(guid) );
	else if(GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_UNIT)
		creature = m_session->GetPlayer()->GetMapMgr()->GetCreature( GET_LOWGUID_PART(guid) );
	
	if(creature != NULL)
		return creature;
	else
	{
		if(showerror) 
			RedSystemMessage(m_session, "Vous devez sélectionner une créature.");
		return NULL;
	}
}

void ChatHandler::SystemMessage(WorldSession *m_session, const char* message, ...)
{
	if( !message ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024, message,ap);
	WorldPacket * data = FillSystemMessageData(msg1);
	if(m_session != NULL) 
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::ColorSystemMessage(WorldSession *m_session, const char* colorcode, const char *message, ...)
{
	if( !message ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024, message,ap);
	char msg[1024];
	snprintf(msg, 1024, "%s%s|r", colorcode, msg1);
	WorldPacket * data = FillSystemMessageData(msg);
	if(m_session != NULL) 
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::RedSystemMessage(WorldSession *m_session, const char *message, ...)
{
	if( !message ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024,message,ap);
	char msg[1024];
	snprintf(msg, 1024,"%s%s|r", MSG_COLOR_LIGHTRED/*MSG_COLOR_RED*/, msg1);
	WorldPacket * data = FillSystemMessageData(msg);
	if(m_session != NULL) 
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::GreenSystemMessage(WorldSession *m_session, const char *message, ...)
{
	if( !message ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024, message,ap);
	char msg[1024];
	snprintf(msg, 1024, "%s%s|r", MSG_COLOR_GREEN, msg1);
	WorldPacket * data = FillSystemMessageData(msg);
	if(m_session != NULL) 
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::BlueSystemMessage(WorldSession *m_session, const char *message, ...)
{
	if( !message ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024, message,ap);
	char msg[1024];
	snprintf(msg, 1024,"%s%s|r", MSG_COLOR_LIGHTBLUE, msg1);
	WorldPacket * data = FillSystemMessageData(msg);
	if(m_session != NULL) 
		m_session->SendPacket(data);
	delete data;
}

void ChatHandler::RedSystemMessageToPlr(Player* plr, const char *message, ...)
{
	if( !message || !plr->GetSession() ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024,message,ap);
	RedSystemMessage(plr->GetSession(), (const char*)msg1);
}

void ChatHandler::GreenSystemMessageToPlr(Player* plr, const char *message, ...)
{
	if( !message || !plr->GetSession() ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024,message,ap);
	GreenSystemMessage(plr->GetSession(), (const char*)msg1);
}

void ChatHandler::BlueSystemMessageToPlr(Player* plr, const char *message, ...)
{
	if( !message || !plr->GetSession() ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024,message,ap);
	BlueSystemMessage(plr->GetSession(), (const char*)msg1);
}

void ChatHandler::SystemMessageToPlr(Player *plr, const char* message, ...)
{
	if( !message || !plr->GetSession() ) return;
	va_list ap;
	va_start(ap, message);
	char msg1[1024];
	vsnprintf(msg1,1024,message,ap);
	SystemMessage(plr->GetSession(), msg1);
}

bool ChatHandler::CmdSetValueField(WorldSession *m_session, uint32 field, uint32 fieldmax, const char *fieldname, const char *args)
{
	if(!args) return false;
	char *next_token;
	char *pvalue = strtok_s((char*)args, " ",&next_token);
	uint32 mv, av;

	if (!pvalue)
		return false;
	else
		av = atol(pvalue);

	if(fieldmax)
	{
		char* pvaluemax = strtok_s(NULL, " ",&next_token);   
		if (!pvaluemax)
			return false;  
		else
			mv = atol(pvaluemax);
	}
	else
	{
		mv = 0;
	}

	if (av <= 0 && mv > 0)
	{  
		RedSystemMessage(m_session, "Values are invalid. Value must be < max (if max exists), and both must be > 0.");
		return true;   
	}
	if(fieldmax)
	{
		if(mv < av || mv <= 0)
		{
			RedSystemMessage(m_session, "Values are invalid. Value must be < max (if max exists), and both must be > 0.");
			return true;  
		}
	}

	Player *plr = getSelectedChar(m_session, false);
	if(plr)
	{  
		sGMLog.writefromsession(m_session, "used modify field value: %s, %u on %s", fieldname, av, plr->GetName());
		if(fieldmax)
		{
			BlueSystemMessage(m_session, "You set the %s of %s to %d/%d.", fieldname, plr->GetName(), av, mv);
			GreenSystemMessageToPlr(plr, "%s set your %s to %d/%d.", m_session->GetPlayer()->GetName(), fieldname, av, mv);
		}
		else
		{
			BlueSystemMessage(m_session, "You set the %s of %s to %d.", fieldname, plr->GetName(), av);
			GreenSystemMessageToPlr(plr, "%s set your %s to %d.", m_session->GetPlayer()->GetName(), fieldname, av);
		}

		if(field == UNIT_FIELD_STAT1) av /= 2;
		if(field == UNIT_FIELD_BASE_HEALTH) 
		{
			plr->SetUInt32Value(UNIT_FIELD_HEALTH, av);
		}

		plr->SetUInt32Value(field, av);

		if(fieldmax) {
			plr->SetUInt32Value(fieldmax, mv);
		}
	}
	else
	{
		Creature *cr = getSelectedCreature(m_session, false);
		if(cr)
		{
			if(!(field < UNIT_END && fieldmax < UNIT_END)) return false;
			std::string creaturename = "Unknown Being";
			if(cr->GetCreatureName())
				creaturename = cr->GetCreatureName()->Name;
			if(fieldmax)
				BlueSystemMessage(m_session, "Setting %s of %s to %d/%d.", fieldname, creaturename.c_str(), av, mv);
			else
				BlueSystemMessage(m_session, "Setting %s of %s to %d.", fieldname, creaturename.c_str(), av);
			sGMLog.writefromsession(m_session, "used modify field value: [creature]%s, %u on %s", fieldname, av, creaturename.c_str());
			if(field == UNIT_FIELD_STAT1) av /= 2;
			if(field == UNIT_FIELD_BASE_HEALTH) 
				cr->SetUInt32Value(UNIT_FIELD_HEALTH, av);

			switch(field)
			{
			case UNIT_FIELD_FACTIONTEMPLATE:
				{
					if(cr->m_spawn)
						WorldDatabase.Execute("UPDATE creature_spawns SET faction = %u WHERE entry = %u", av, cr->m_spawn->entry);
				}break;
			case UNIT_NPC_FLAGS:
				{
					if(cr->proto)
						WorldDatabase.Execute("UPDATE creature_proto SET npcflags = %u WHERE entry = %u", av, cr->proto->Id);
				}break;
			}

			cr->SetUInt32Value(field, av);
			BlueSystemMessage(m_session, "Changment de %s : Nouvelle valeur: %d.", fieldname, av);

			if(fieldmax) 
			{
				BlueSystemMessage(m_session, "Changment de %s : Nouvelle valeur: %d.", fieldname, mv);
				cr->SetUInt32Value(fieldmax, mv);
			}
			// reset faction
			if(field == UNIT_FIELD_FACTIONTEMPLATE)
			{
				cr->_setFaction();
				BlueSystemMessage(m_session, "Faction mis à jour Ok");
			}

			cr->SaveToDB();
		}
		else
		{
			RedSystemMessage(m_session, "Invalid Selection.");
		}
	}
	return true;
}

bool ChatHandler::CmdSetFloatField(WorldSession *m_session, uint32 field, uint32 fieldmax, const char *fieldname, const char *args)
{
	char* next_token;
	char* pvalue = strtok_s((char*)args, " ",&next_token);
	float mv, av;

	if (!pvalue)
		return false;
	else
		av = (float)atof(pvalue);

	if(fieldmax)
	{
		char* pvaluemax = strtok_s(NULL, " ",&next_token);   
		if (!pvaluemax)
			return false;  
		else
			mv = (float)atof(pvaluemax);
	}
	else
	{
		mv = 0;
	}

	if (av <= 0)
	{  
		RedSystemMessage(m_session, "Values are invalid. Value must be < max (if max exists), and both must be > 0.");
		return true;   
	}
	if(fieldmax)
	{
		if(mv < av || mv <= 0)
		{
			RedSystemMessage(m_session, "Values are invalid. Value must be < max (if max exists), and both must be > 0.");
			return true;  
		}
	}

	Player *plr = getSelectedChar(m_session, false);
	if(plr)
	{  
		sGMLog.writefromsession(m_session, "used modify field value: %s, %f on %s", fieldname, av, plr->GetName());
		if(fieldmax)
		{
			BlueSystemMessage(m_session, "You set the %s of %s to %.1f/%.1f.", fieldname, plr->GetName(), av, mv);
			GreenSystemMessageToPlr(plr, "%s set your %s to %.1f/%.1f.", m_session->GetPlayer()->GetName(), fieldname, av, mv);
		}
		else
		{
			BlueSystemMessage(m_session, "You set the %s of %s to %.1f.", fieldname, plr->GetName(), av);
			GreenSystemMessageToPlr(plr, "%s set your %s to %.1f.", m_session->GetPlayer()->GetName(), fieldname, av);
		}
		plr->SetFloatValue(field, av);
		if(fieldmax) plr->SetFloatValue(fieldmax, mv);
	}
	else
	{
		Creature *cr = getSelectedCreature(m_session, false);
		if(cr)
		{
			if(!(field < UNIT_END && fieldmax < UNIT_END)) return false;
			std::string creaturename = "Unknown Being";
			if(cr->GetCreatureName())
				creaturename = cr->GetCreatureName()->Name;
			if(fieldmax)
				BlueSystemMessage(m_session, "Setting %s of %s to %.1f/%.1f.", fieldname, creaturename.c_str(), av, mv);
			else
				BlueSystemMessage(m_session, "Setting %s of %s to %.1f.", fieldname, creaturename.c_str(), av);
			cr->SetFloatValue(field, av);
			sGMLog.writefromsession(m_session, "used modify field value: [creature]%s, %f on %s", fieldname, av, creaturename.c_str());
			if(fieldmax) {
				cr->SetFloatValue(fieldmax, mv);
			}
			//cr->SaveToDB();
		}
		else
		{
			RedSystemMessage(m_session, "Invalid Selection.");
		}
	}
	return true;
}

bool ChatHandler::HandleGetPosCommand(const char* args, WorldSession *m_session)
{
	/*if(m_session->GetPlayer()->GetSelection() == 0) return false;
	Creature *creature = objmgr.GetCreature(m_session->GetPlayer()->GetSelection());

	if(!creature) return false;
	BlueSystemMessage(m_session, "Creature Position: \nX: %f\nY: %f\nZ: %f\n", creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ());
	return true;*/
/*
	uint32 spell = atol(args);
	SpellEntry *se = dbcSpell.LookupEntry(spell);
	if(se)
		BlueSystemMessage(m_session, "SpellIcon for %d is %d", se->Id, se->field114);
		*/
	return true;
}
// HearthStone
bool ChatHandler::HandleDebugRetroactiveQuestAchievements(const char *args, WorldSession *m_session)
{
	Player *pTarget = getSelectedChar(m_session, true );
	if(!pTarget) return true;

	pTarget->RetroactiveCompleteQuests();
	m_session->GetPlayer()->BroadcastMessage("Done.");
	return true;
} 
//------------------------- 
// Pour test Synchro Client/Serveur (Branruz)
bool ChatHandler::HandleDebugSynchroClient(const char *args, WorldSession *m_session)
{
  Player *plr = m_session->GetPlayer();	
  plr->OnTimeSyncRequest();
  GreenSystemMessage(m_session, "SMSG_TIME_SYNC_REQ envoyé!");
  return(true);
}