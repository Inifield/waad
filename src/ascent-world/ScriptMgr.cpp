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
#ifndef WIN32
    #include <dlfcn.h>
    #include <unistd.h>
    #include <dirent.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <cstdlib>
    #include <cstring>
#endif

#include <svn_revision.h>
#define SCRIPTLIB_HIPART(x) ((x >> 16))
#define SCRIPTLIB_LOPART(x) ((x & 0x0000ffff))
#define SCRIPTLIB_VERSION_MINOR (BUILD_REVISION % 1000)
#define SCRIPTLIB_VERSION_MAJOR (BUILD_REVISION / 1000)

initialiseSingleton(ScriptMgr);
initialiseSingleton(HookInterface);

ScriptMgr::ScriptMgr()
{
	DefaultGossipScript = new GossipScript();
}

ScriptMgr::~ScriptMgr()
{

}

struct ScriptingEngine
{
#ifdef WIN32
	HMODULE Handle;
#else
	void* Handle;
#endif
	exp_script_register InitializeCall;
	uint32 Type;
};

void ScriptMgr::LoadScripts()
{
	if(!HookInterface::getSingletonPtr())
		new HookInterface;

  	Log.Notice("Server","Loading External Script Libraries...");
	sLog.outString("");

	string start_path = Config.MainConfig.GetStringDefault( "Script", "BinaryLocation", "script_bin" ) + "\\";
	string search_path = start_path + "*.";

	vector< ScriptingEngine > ScriptEngines;

	/* Loading system for win32 */
#ifdef WIN32
	search_path += "dll";

	WIN32_FIND_DATA data;
	uint32 count = 0;
	HANDLE find_handle = FindFirstFile( search_path.c_str(), &data );
	if(find_handle == INVALID_HANDLE_VALUE)
		sLog.outError( "  Aucun script externe trouve! Le serveur continuera a fonctionner avec des fonctionnalites limitees." );
	else
	{
		do
		{
			string full_path = start_path + data.cFileName;
			HMODULE mod = LoadLibrary( full_path.c_str() );
			printf( "  %s : 0x%p : ", data.cFileName, reinterpret_cast< uint32* >( mod ));
			if( mod == 0 )
			{
				printf("Load Library Error !\n");
			}
			else
			{
				// find version import
				exp_get_version vcall = (exp_get_version)GetProcAddress(mod, "_exp_get_version");
				exp_script_register rcall = (exp_script_register)GetProcAddress(mod, "_exp_script_register");
				exp_get_script_type scall = (exp_get_script_type)GetProcAddress(mod, "_exp_get_script_type");
				if(vcall == 0 || rcall == 0 || scall == 0)
				{
					if(!vcall) printf("_exp_get_version error!\n");
					else if(!rcall) printf("_exp_script_register error!\n");
					else if(!scall) printf("_exp_get_script_type error!\n");
					FreeLibrary(mod);
				}
				else
				{
					uint32 version = vcall();
					uint32 stype = scall();
					if(SCRIPTLIB_LOPART(version) == SCRIPTLIB_VERSION_MINOR && SCRIPTLIB_HIPART(version) == SCRIPTLIB_VERSION_MAJOR)
					{
						if( stype & SCRIPT_TYPE_SCRIPT_ENGINE )
						{
							printf("Delayed load: v%u.%u : \n", SCRIPTLIB_HIPART(version), SCRIPTLIB_LOPART(version));
							//printf("delayed load.\n");

							ScriptingEngine se;
							se.Handle = mod;
							se.InitializeCall = rcall;
							se.Type = stype;

							ScriptEngines.push_back( se );
						}
						else
						{
							_handles.push_back(((SCRIPT_MODULE)mod));
							printf("Loaded : v%u.%u :\n ", SCRIPTLIB_HIPART(version), SCRIPTLIB_LOPART(version));
							rcall(this);
							//printf("loaded.\n");						
						}

						++count;
					}
					else
					{
						FreeLibrary(mod);
						printf("version mismatch!\n");						
					}
				}
			}
		}
		while(FindNextFile(find_handle, &data));
		FindClose(find_handle);
		sLog.outString("");
		Log.Notice("Server","Loaded %u external libraries.", count);
		sLog.outString("");

		Log.Notice("Server","Loading optional scripting engines...");
		for(vector<ScriptingEngine>::iterator itr = ScriptEngines.begin(); itr != ScriptEngines.end(); ++itr)
		{
			if( itr->Type & SCRIPT_TYPE_SCRIPT_ENGINE_LUA )
			{
				// lua :O
				if( Config.MainConfig.GetBoolDefault("ScriptBackends", "LUA", false) )
				{
					Log.Notice("Server","Initializing LUA script engine...");
					itr->InitializeCall(this);
					_handles.push_back( (SCRIPT_MODULE)itr->Handle );
				}
				else
				{
					FreeLibrary( itr->Handle );
				}
			}
			else if( itr->Type & SCRIPT_TYPE_SCRIPT_ENGINE_AS )
			{
				if( Config.MainConfig.GetBoolDefault("ScriptBackends", "AS", false) )
				{
					Log.Notice("Server","Initializing AngelScript script engine...");
					itr->InitializeCall(this);
					_handles.push_back( (SCRIPT_MODULE)itr->Handle );
				}
				else
				{
					FreeLibrary( (*itr).Handle );
				}
			}
			else
			{
				Log.Notice("Server","Unknown script engine type: 0x%.2X, please contact developers.", (*itr).Type );
				FreeLibrary( itr->Handle );
			}
		}
		Log.Notice("Server","Done loading script engines...");
	}
#else
	/* Loading system for *nix */
	struct dirent ** list;
	// Frk - 8 Janvier 2009
	// int filecount = scandir(PREFIX "/lib/", &list, 0, 0);
	// en partant du principe que le dossier d origine est le dossier du binair donc PREFIX/bin/ donc ../lib/
	int filecount = scandir("../lib/", &list, 0, 0);
	uint32 count = 0;

	if(!filecount || !list || filecount < 0)
		sLog.outError("  Aucun script externe trouve ! Le serveur continuera a fonctionner avec des fonctionnalites limitees.");
	else
	{
char *ext;
		while(filecount--)
		{
			ext = strrchr(list[filecount]->d_name, '.');
#ifdef HAVE_DARWIN
			if (ext != NULL && strstr(list[filecount]->d_name, ".0.dylib") == NULL && !strcmp(ext, ".dylib")) {
#else
                        if (ext != NULL && !strcmp(ext, ".so")) {
#endif
				string full_path = "../lib/" + string(list[filecount]->d_name);
				SCRIPT_MODULE mod = dlopen(full_path.c_str(), RTLD_NOW);
				printf("  %s : 0x%p : ", list[filecount]->d_name, mod);
				if(mod == 0)
					printf("error! [%s]\n", dlerror());
				else
				{
					// find version import
					exp_get_version vcall = (exp_get_version)dlsym(mod, "_exp_get_version");
					exp_script_register rcall = (exp_script_register)dlsym(mod, "_exp_script_register");
					exp_get_script_type scall = (exp_get_script_type)dlsym(mod, "_exp_get_script_type");
					if(vcall == 0 || rcall == 0 || scall == 0)
					{
						printf("Fonction non trouvee !\n");
						dlclose(mod);
					}
					else
					{
						int32 version = vcall();
						uint32 stype = scall();
						if(SCRIPTLIB_LOPART(version) == SCRIPTLIB_VERSION_MINOR && SCRIPTLIB_HIPART(version) == SCRIPTLIB_VERSION_MAJOR)
						{
							if( stype & SCRIPT_TYPE_SCRIPT_ENGINE )
							{
								printf("v%u.%u : ", SCRIPTLIB_HIPART(version), SCRIPTLIB_LOPART(version));
								printf("delayed load.\n");

								ScriptingEngine se;
								se.Handle = mod;
								se.InitializeCall = rcall;
								se.Type = stype;

								ScriptEngines.push_back( se );
							}
							else
							{
								_handles.push_back(((SCRIPT_MODULE)mod));
								printf("v%u.%u : ", SCRIPTLIB_HIPART(version), SCRIPTLIB_LOPART(version));
								rcall(this);
								printf("loaded.\n");						
							}

							++count;
						}
						else
						{
							dlclose(mod);
							printf("version mismatch!\n");						
						}
					}
				}
			}
			free(list[filecount]);
		}
		free(list);
		sLog.outString("");
		sLog.outString("Loaded %u external libraries.", count);
		sLog.outString("");

		sLog.outString("Loading optional scripting engines...");
		for(vector<ScriptingEngine>::iterator itr = ScriptEngines.begin(); itr != ScriptEngines.end(); ++itr)
		{
			if( itr->Type & SCRIPT_TYPE_SCRIPT_ENGINE_LUA )
			{
				// lua :O
				if( Config.MainConfig.GetBoolDefault("ScriptBackends", "LUA", false) )
				{
					sLog.outString("   Initializing LUA script engine...");
					itr->InitializeCall(this);
					_handles.push_back( (SCRIPT_MODULE)itr->Handle );
				}
				else
				{
					dlclose( itr->Handle );
				}
			}
			else if( itr->Type & SCRIPT_TYPE_SCRIPT_ENGINE_AS )
			{
				if( Config.MainConfig.GetBoolDefault("ScriptBackends", "AS", false) )
				{
					sLog.outString("   Initializing AngelScript script engine...");
					itr->InitializeCall(this);
					_handles.push_back( (SCRIPT_MODULE)itr->Handle );
				}
				else
				{
					dlclose( (*itr).Handle );
				}
			}
			else
			{
				sLog.outString("  Unknown script engine type: 0x%.2X, please contact developers.", (*itr).Type );
				dlclose( itr->Handle );
			}
		}
		sLog.outString("Done loading script engines...");
	}
#endif
}

void ScriptMgr::UnloadScripts()
{
	if(HookInterface::getSingletonPtr())
		delete HookInterface::getSingletonPtr();

	for(CustomGossipScripts::iterator itr = _customgossipscripts.begin(); itr != _customgossipscripts.end(); ++itr)
		(*itr)->Destroy();
	_customgossipscripts.clear();
	delete this->DefaultGossipScript;
	this->DefaultGossipScript=NULL;

	for(QuestScripts::iterator itr = _questscripts.begin(); itr != _questscripts.end(); ++itr)
		delete *itr;
	_questscripts.clear();

	LibraryHandleMap::iterator itr = _handles.begin();
	for(; itr != _handles.end(); ++itr)
	{
#ifdef WIN32
		FreeLibrary(((HMODULE)*itr));
#else
		dlclose(*itr);
#endif
	}
	_handles.clear();
}

void ScriptMgr::register_creature_script(uint32 entry, exp_create_creature_ai callback)
{
	_creatures.insert( CreatureCreateMap::value_type( entry, callback ) );
}

void ScriptMgr::register_gameobject_script(uint32 entry, exp_create_gameobject_ai callback)
{
	_gameobjects.insert( GameObjectCreateMap::value_type( entry, callback ) );
}

void ScriptMgr::register_dummy_aura(uint32 entry, exp_handle_dummy_aura callback)
{
	_auras.insert( HandleDummyAuraMap::value_type( entry, callback ) );
}

void ScriptMgr::register_spell_script(uint32 entry, exp_create_spell callback)
{
	_spellscripts.insert( SpellCreateMap::value_type( entry, callback ) );
}

void ScriptMgr::register_event_script(uint32 entry, exp_create_event callback)
{
	_eventscripts.insert( EventCreateMap::value_type( entry, callback ) );
}

void ScriptMgr::register_dummy_spell(uint32 entry, exp_handle_dummy_spell callback)
{
	_spells.insert( HandleDummySpellMap::value_type( entry, callback ) );
}

void ScriptMgr::register_gossip_script(uint32 entry, GossipScript * gs)
{
	CreatureInfo * ci = CreatureNameStorage.LookupEntry(entry);
	if(ci)
		ci->gossip_script = gs;

	_customgossipscripts.insert(gs);
}

void ScriptMgr::register_go_gossip_script(uint32 entry, GossipScript * gs)
{
	GameObjectInfo * gi = GameObjectNameStorage.LookupEntry(entry);
	if(gi)
		gi->gossip_script = gs;

	_customgossipscripts.insert(gs);
}

void ScriptMgr::register_quest_script(uint32 entry, QuestScript * qs)
{
	Quest * q = QuestStorage.LookupEntry( entry );
	if( q != NULL )
		q->pQuestScript = qs;

	_questscripts.insert( qs );
}

CreatureAIScript* ScriptMgr::CreateAIScriptClassForEntry(Creature* pCreature)
{
	CreatureCreateMap::iterator itr = _creatures.find(pCreature->GetEntry());
	if(itr == _creatures.end())
		return NULL;

	exp_create_creature_ai function_ptr = itr->second;
	return (function_ptr)(pCreature);
}

GameObjectAIScript * ScriptMgr::CreateAIScriptClassForGameObject(uint32 uEntryId, GameObject* pGameObject)
{
	GameObjectCreateMap::iterator itr = _gameobjects.find(pGameObject->GetEntry());
	if(itr == _gameobjects.end())
		return NULL;

	exp_create_gameobject_ai function_ptr = itr->second;
	return (function_ptr)(pGameObject);
}

SpellScript * ScriptMgr::CreateScriptClassForSpell(uint32 uEntryId, Spell* pSpell)
{
	SpellCreateMap::iterator itr = _spellscripts.find(uEntryId);
	if(itr == _spellscripts.end())
		return new SpellScript(pSpell);

	exp_create_spell function_ptr = itr->second;
	return (function_ptr)(pSpell);
}

EventScript * ScriptMgr::CreateScriptClassForEvent(uint32 uEntryId, Object* o, AreaTrigger* a, Spell* s)
{
	EventCreateMap::iterator itr = _eventscripts.find(uEntryId);
	if(itr == _eventscripts.end())
		return NULL;

	exp_create_event function_ptr = itr->second;
	return (function_ptr)(o, a, s);
}

bool ScriptMgr::CallScriptedDummySpell(uint32 uSpellId, uint32 i, Spell* pSpell)
{
	HandleDummySpellMap::iterator itr = _spells.find(uSpellId);
	if(itr == _spells.end())
		return false;

	exp_handle_dummy_spell function_ptr = itr->second;
	return (function_ptr)(i, pSpell);
}

bool ScriptMgr::CallScriptedDummyAura(uint32 uSpellId, uint32 i, Aura* pAura, bool apply)
{
	HandleDummyAuraMap::iterator itr = _auras.find(uSpellId);
	if(itr == _auras.end())
		return false;

	exp_handle_dummy_aura function_ptr = itr->second;
	return (function_ptr)(i, pAura, apply);
}

bool ScriptMgr::CallScriptedItem(Item * pItem, Player * pPlayer)
{
	if(pItem->GetProto()->gossip_script)
	{
		pItem->GetProto()->gossip_script->GossipHello(pItem,pPlayer,true);
		return true;
	}
	
	return false;
}

void ScriptMgr::register_item_gossip_script(uint32 entry, GossipScript * gs)
{
	ItemPrototype * proto = ItemPrototypeStorage.LookupEntry(entry);
	if(proto)
		proto->gossip_script = gs;

	_customgossipscripts.insert(gs);
}

/* CreatureAI Stuff */
CreatureAIScript::CreatureAIScript(Creature* creature) : _unit(creature)
{

}

void CreatureAIScript::RegisterAIUpdateEvent(uint32 frequency)
{
	sEventMgr.AddEvent(_unit, &Creature::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, frequency, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void CreatureAIScript::ModifyAIUpdateEvent(uint32 newfrequency)
{
	sEventMgr.ModifyEventTimeAndTimeLeft(_unit, EVENT_SCRIPT_UPDATE_EVENT, newfrequency);
}

void CreatureAIScript::RemoveAIUpdateEvent()
{
	sEventMgr.RemoveEvents(_unit, EVENT_SCRIPT_UPDATE_EVENT);
}

/* GameObjectAI Stuff */

GameObjectAIScript::GameObjectAIScript(GameObject* goinstance) : _gameobject(goinstance)
{

}

void GameObjectAIScript::ModifyAIUpdateEvent(uint32 newfrequency)
{
	sEventMgr.ModifyEventTimeAndTimeLeft(_gameobject, EVENT_SCRIPT_UPDATE_EVENT, newfrequency);
}
 
void GameObjectAIScript::RemoveAIUpdateEvent()
{
	sEventMgr.RemoveEvents(_gameobject, EVENT_SCRIPT_UPDATE_EVENT);
}

void GameObjectAIScript::RegisterAIUpdateEvent(uint32 frequency)
{
	sEventMgr.AddEvent(_gameobject, &GameObject::CallScriptUpdate, EVENT_SCRIPT_UPDATE_EVENT, frequency, 0,0);
}


/* InstanceAI Stuff */

InstanceScript::InstanceScript(MapMgr *instance) : _instance(instance)
{
}

/* QuestScript Stuff */

/* Gossip Stuff*/

GossipScript::GossipScript()
{
	
}

void GossipScript::GossipEnd(Object* pObject, Player* Plr)
{
	Plr->CleanupGossipMenu();
}

#define TXT_BATTLEMASTER_BG_CLOSE	"Nos troupes ne sont pas encore prêtes à livrer bataille, repassez me voir dans quelques temps!"
bool CanTrainAt(Player * plr, Trainer * trn);
void GossipScript::GossipHello(Object* pObject, Player* Plr, bool AutoSend)
{
	bool BG_OPEN_OK = false;
	GossipMenu *Menu;
	uint32 TextID = 2;
	char NameBG[512];
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?static_cast< Creature* >( pObject ):NULL;
	
	if(!pCreature) return;

	uint32 flags = pCreature->GetUInt32Value(UNIT_NPC_FLAGS);

	sLog.outDetail( "WORLD: GossipHello" );

	Trainer *pTrainer = pCreature->GetTrainer();

	if(flags & UNIT_NPC_FLAG_DIRECTTAXI)
	{
		// Genre le griffon du fort d'ebene
		//Log.Warning("","Griffon du fort d'ebene...");
		Plr->GetSession()->ActivateDirectTaxi(pCreature);
		return;
	}

	uint32 Text = objmgr.GetGossipTextForNpc(pCreature->GetEntry());
	if(Text != 0)
	{
		GossipText * text = NpcTextStorage.LookupEntry(Text);
		if(text != 0)
			TextID = Text;
	}
	else
		TextID = 1;

	objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), TextID, Plr);

	if( (flags & UNIT_NPC_FLAG_TRAINER) || (flags & UNIT_NPC_FLAG_TRAINER_PROF) ) 
	{
	   if(pTrainer == 0)
	   {
		   Log.Error("[GossipHello]","[DB] Erreur de table Trainer: %u n'existe pas (%s)",
			                pCreature->GetCreatureName()->Id,pCreature->GetCreatureName()->Name );
	   }
	   else 
	   {
		string name = pCreature->GetCreatureName()->Name;
		string::size_type pos = name.find(" ");	  // only take first name
		if(pos != string::npos)
			name = name.substr(0, pos);

		if(CanTrainAt(Plr, pTrainer))
			Menu->SetTextID(pTrainer->Can_Train_Gossip_TextId);
		else
			Menu->SetTextID(pTrainer->Cannot_Train_GossipTextId);
        
		string msg = "Je cherche ";
		if(pTrainer->RequiredClass)
		{
			msg += "un entrainement ";
			switch(Plr->getClass())
			{
			case MAGE:
				msg += "de Mage, ";
				break;
			case SHAMAN:
				msg += "de Chaman, ";
				break;
			case WARRIOR:
				msg += "de Guerrier, ";
				break;
			case PALADIN:
				msg += "de Paladin, ";
				break;
			case WARLOCK:
				msg += "de Démoniste, ";
				break;
			case HUNTER:
				msg += "de Chasseur, ";
				break;
			case ROGUE:
				msg += "de Voleur, ";
				break;
			case DRUID:
				msg += "de Druide, ";
				break;
			case PRIEST:
				msg += "de Prêtre, ";
				break;
			case DEATHKNIGHT:
                msg += "de Chevalier de la mort, ";
				break;
			default: 
				msg += "de ce que vous me proposerez, ";
				break;
			}
			msg += name;
			msg += ".";

			Menu->AddItem(GOSSIP_ICON_GOSSIP_TRAINER, msg.c_str(),GOSSIP_OPTION_TRAINER);

		}
		else
		{
			msg += "un entrainement, ";
			msg += name;
			msg += ".";

			Menu->AddItem(GOSSIP_ICON_GOSSIP_TRAINER, msg.c_str(),GOSSIP_OPTION_TRAINER);
		}
	   }
	}
	if(flags & UNIT_NPC_FLAG_VENDOR)
	{
		Menu->AddItem(GOSSIP_ICON_GOSSIP_VENDOR, "Je voudrais voir vos marchandises.",GOSSIP_OPTION_VENDOR);
		Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, "Finalement, je n'ai besoin de rien aujourd'hui. Au revoir",GOSSIP_OPTION_ABORTING_ACTION);
	}

	if(flags & UNIT_NPC_FLAG_TAXIVENDOR)
	{
		Menu->AddItem(GOSSIP_ICON_GOSSIP_FLIGHT, "Donnez moi les destinations.",GOSSIP_OPTION_TAXI);
		Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, "Je préfère voyager par mes propres moyens. Au revoir",GOSSIP_OPTION_ABORTING_ACTION);
	}

	if(flags & UNIT_NPC_FLAG_AUCTIONEER)
		Menu->AddItem(GOSSIP_ICON_GOSSIP_AUCTION, "Je voudrais faire une enchère.",GOSSIP_OPTION_AUCTIONNER);

	if(flags & UNIT_NPC_FLAG_INNKEEPER)
		Menu->AddItem(GOSSIP_ICON_GOSSIP_EXTRA, "Que puis je faire dans cette auberge.", GOSSIP_OPTION_INNKEEPER_HELP_MENU);

	if(flags & UNIT_NPC_FLAG_BANKER)
	{
		Menu->AddItem(GOSSIP_ICON_GOSSIP_COIN, "J'aimerais verifier mon coffre de banque.",GOSSIP_OPTION_BANK);
		Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, "Comment ça il vous faut les papiers pour accèder à mon coffre !!?? Allez donc les demander au dragon qui les a avalés !!  Au revoir !", 99);
	}

	if(flags & UNIT_NPC_FLAG_SPIRITHEALER)
		Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, "Ramenez-moi à la vie.",GOSSIP_OPTION_SPIRIT);

	if(flags & UNIT_NPC_FLAG_ARENACHARTER)
		Menu->AddItem(GOSSIP_ICON_GOSSIP_ARENA, "Comment faire pour créer une équipe de guilde/arène ?",GOSSIP_OPTION_PETITION);

	if(flags & UNIT_NPC_FLAG_TABARDCHANGER)
		Menu->AddItem(GOSSIP_ICON_GOSSIP_TABARD, "J'aimerais acheter un tabard de guilde.",GOSSIP_OPTION_TABARDVENDOR);

    if(flags & UNIT_NPC_FLAG_STABLEMASTER)
	{
		if((Plr->getClass()==HUNTER) || ((Plr->getClass()==WARRIOR) && sWorld.m_WarriorPetAllowed) )
		{
		 //Menu->SetTextID(pTrainer->Can_Train_Gossip_TextId);
		 // Texte Libre
		 // Bienvenue, $Sex?guerriere:guerrier, je peux mettre // WaadFun
		 // Bienvenue, $Sex?chasseresse:chasseur, je peux mettre 
		 // vos familiers à l'écurie ou vous aider à
		 // retrouver un compagnon perdu.
		 Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, "J'aimerais mettre mon familier à l'écurie.",GOSSIP_OPTION_STABLEPET); // Trad Papyjo
		 Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, "Finalement, je n'ai besoin de rien aujourd'hui. Au revoir",GOSSIP_OPTION_ABORTING_ACTION);
		}
	}

	if(flags & UNIT_NPC_FLAG_BATTLEFIELDPERSON)
	{ 
		char BufferBGTxt[256]; 
		 if(strstr(pCreature->GetCreatureName()->Name,"Emissaire") != NULL   // Alliance
			 || strstr(pCreature->GetCreatureName()->Name,"Envoyé") != NULL ) // Horde
		 {  // Le nom du Bg est dans le 'Name' de l'Emissaire ou l'Envoyé
			strcpy(NameBG,pCreature->GetCreatureName()->Name); 
		 }
		 else 
		 {
			 // Le nom du Bg est dans le 'SubName' du maitre de Guerre
			 strcpy(NameBG,pCreature->GetCreatureName()->SubName);
		 }
        // FIX pour eviter la mise en file d'attente si le champ de bataille est fermé! (Branruz)
		if(strstr(NameBG,sWorld.m_ArenaSubnameContent) != NULL) 
		{
			BG_OPEN_OK = true;
			strcpy(BufferBGTxt,"J'aimerais me rendre dans l'arene");
		}
		else if(strstr(NameBG,sWorld.m_ArathiSubnameContent) != NULL) 
		{
			BG_OPEN_OK = isBattleGroundOpen(BATTLEGROUND_ARATHI_BASIN);
			strcpy(BufferBGTxt,"J'aimerais me rendre dans le bassin d'Arathi");
		}
		else if(strstr(NameBG, sWorld.m_WarSongSubnameContent) != NULL) 
		{
			BG_OPEN_OK = isBattleGroundOpen(BATTLEGROUND_WARSONG_GULCH);
			strcpy(BufferBGTxt,"J'aimerais me rendre au goulet des Chanteguerres");
		}
		else if(strstr(NameBG,sWorld.m_AlteracSubnameContent) != NULL) 
		{
			BG_OPEN_OK = isBattleGroundOpen(BATTLEGROUND_ALTERAC_VALLEY);
			strcpy(BufferBGTxt,"J'aimerais me rendre dans la vallee d'Alterac");
		}
		else if(strstr(NameBG, sWorld.m_NetherStormSubnameContent) != NULL) 
		{
			BG_OPEN_OK = isBattleGroundOpen(BATTLEGROUND_EYE_OF_THE_STORM);
			strcpy(BufferBGTxt,"J'aimerais me rendre a l'Oeil du Cyclone");
		}
		else if(strstr(NameBG, sWorld.m_SotaSubnameContent) != NULL) 
		{
			BG_OPEN_OK = isBattleGroundOpen(BATTLEGROUND_STRAND_OF_THE_ANCIENTS);
			strcpy(BufferBGTxt,"J'aimerais me rendre au rivage des Anciens");
		}


		if(BG_OPEN_OK)
		{
			Menu->AddItem(GOSSIP_ICON_GOSSIP_ARENA, BufferBGTxt,GOSSIP_OPTION_BATTLEFIELD); // 0
			Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, 
				"Hmmmm... Livrer batailles vous dites ? Je ne peux pas, je suis pacifique ! Au revoir", GOSSIP_OPTION_ABORTING_ACTION);
		}
		else
		{
			Menu->AddItem(GOSSIP_ICON_GOSSIP_ARENA, TXT_BATTLEMASTER_BG_CLOSE, GOSSIP_OPTION_ABORTING_ACTION);
			Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, 
				"Aucune importance ! Je n'ai pas envie de me faire massacrer aujourd'hui ! Au revoir.", GOSSIP_OPTION_ABORTING_ACTION);
		} // Sort de la boucle
			
	}
		
	if( pTrainer && pTrainer->RequiredClass )
	{
		if( pTrainer->RequiredClass == Plr->getClass() && pCreature->getLevel() > 10 && Plr->getLevel() > 11 )
		{
			Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, "Je souhaiterais oublier mes talents.",GOSSIP_OPTION_TALENTRESET_MENU);
			if( Plr->getLevel() >= 40 && Plr->m_talentSpecsCount < 2)
				Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, 
				        "Au sujet de l'apprentissage d'une double spécialisation.",GOSSIP_OPTION_DUAL_SPE_MENU);
		}
	}
	
	if( pTrainer &&
		pTrainer->TrainerType == TRAINER_TYPE_PET &&	// pet trainer type
		(Plr->getClass() == HUNTER ||					// hunter class
		(Plr->getClass()==WARRIOR && sWorld.m_WarriorPetAllowed) ) && // warrior class - WaadFun
		Plr->GetSummon() != NULL )						// have pet
	{
		Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, "Je souhaiterais faire oublier ses talents a mon familer.", GOSSIP_OPTION_UNTRAIN_MSG); // Branruz
	}

	if(AutoSend)
		Menu->SendTo(Plr);
}

void GossipScript::GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * EnteredCode)
{
	GossipMenu* Menu ;
	GossipMenuAddPOI pMenuPOI ;
	pMenuPOI.m_IndexPOI=IntId;

	Creature* pCreature = static_cast< Creature* >( pObject );
	if( pObject->GetTypeId() != TYPEID_UNIT )
		return;

    switch( IntId )
	{
	 case GOSSIP_OPTION_PATHFINDER: // GOSSIP Gossip/Talk
			if (pMenuPOI.m_IndexPOI)
				Plr->CurrentGossipMenu->SendPointOfInterest(pMenuPOI.m_IndexPOI);
			Plr->Gossip_Complete();
			break;
	 case GOSSIP_OPTION_QUESTGIVER: // Questgiver
			Plr->Gossip_Complete();
			break;
	 case GOSSIP_OPTION_VENDOR: // Vendor
			Plr->GetSession()->SendInventoryList(pCreature);
			break;
	 case GOSSIP_OPTION_TAXI: // taxi
			Plr->GetSession()->SendTaxiList(pCreature);
			break;
	 case GOSSIP_OPTION_TRAINER: // trainer
			Plr->GetSession()->SendTrainerList(pCreature);
			break;
	 case GOSSIP_OPTION_SPIRIT: // spirit
			Plr->GetSession()->SendSpiritHealerRequest(pCreature);
			break;
	 case GOSSIP_OPTION_SPIRITGUIDE: // Spirit Guide ( Coder l'appel dans GossipHello )
			Plr->Gossip_Complete();
			break;
	 case GOSSIP_OPTION_INNKEEPER: // innkeeper
			Plr->GetSession()->SendInnkeeperBind(pCreature);
			break;
	 case GOSSIP_OPTION_BANK: // banker
			Plr->GetSession()->SendBankerList(pCreature);
			break;
	 case GOSSIP_OPTION_PETITION: // petition
			Plr->GetSession()->SendCharterRequest(pCreature);
			break;
	 case GOSSIP_OPTION_TABARDVENDOR: // Tabard Vendor
		{
			Plr->Gossip_Complete();		
			Plr->GetSession()->SendTabardHelp(pCreature);
		}break;
	 case GOSSIP_OPTION_BATTLEFIELD: // battlefield
			Plr->GetSession()->SendBattlegroundList(pCreature, 0);
			break;
	 case GOSSIP_OPTION_AUCTIONNER: // Auctioner - MSG_AUCTION_HELLO: -
			Plr->GetSession()->SendAuctionList(pCreature);
			break;
	 case GOSSIP_OPTION_STABLEPET: // PetList
			Plr->GetSession()->SendStabledPetList(pCreature->GetGUID());
			break;
	 case GOSSIP_OPTION_ARMORER: //  Armorer ( Coder l'appel dans GossipHello )
			Plr->GetSession()->SendInventoryList(pCreature);
			break;
	 case GOSSIP_OPTION_TALENTRESET: // talent reset
		{
			Plr->Gossip_Complete();
			Plr->SendTalentResetConfirm();
		}break;
	 case GOSSIP_OPTION_UNTRAINPET: // untrain pet
		{
			Plr->Gossip_Complete();
			Plr->SendPetUntrainConfirm();
		}break;
	 case GOSSIP_OPTION_DUAL_TALENT: // Dual talent
		{
			Plr->Gossip_Complete();
			Plr->SendDualTalentConfirm();
		}break;
     // Menu Recursif
	 case GOSSIP_OPTION_TALENTRESET_MENU: // Switch Menu talents reset
		{
			//GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 5674, Plr);
			Menu->AddItem(GOSSIP_ICON_GOSSIP_ENGINEER2, "Je comprends, continuez.",GOSSIP_OPTION_TALENTRESET);
			Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, "Non merci, finalement je viens de changer d'avis.",GOSSIP_OPTION_ABORTING_ACTION);
			Menu->SendTo(Plr);		
		}break;
	 case GOSSIP_OPTION_UNTRAIN_MSG: // switch to untrain message
		{
			//GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 7722, Plr);
			Menu->AddItem(GOSSIP_ICON_GOSSIP_ENGINEER2, "Oui, Faite le s'il vous plait !",GOSSIP_OPTION_UNTRAINPET);
			Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, "Non finalement pas maintenant, merci.",GOSSIP_OPTION_ABORTING_ACTION);
			Menu->SendTo(Plr);
		}break;
	 case GOSSIP_OPTION_DUAL_SPE_MENU: // Switch Menu Double Spécialisation
		{
			//GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 30000, Plr);
			Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, "Acheter une double specialisation.",GOSSIP_OPTION_DUAL_TALENT);
			Menu->SendTo(Plr);
		}break;
	 case GOSSIP_OPTION_INNKEEPER_HELP_MENU: // switch Innkeeper help menu
		{
			Plr->bHasBindDialogOpen = false;
			//GossipMenu *Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 1853, Plr);
			Menu->AddItem(GOSSIP_ICON_GOSSIP_ENGINEER2, "Faite de cette Auberge mon foyer.",GOSSIP_OPTION_INNKEEPER);
			Menu->AddItem(GOSSIP_ICON_GOSSIP_NORMAL, "Au revoir.",GOSSIP_OPTION_ABORTING_ACTION);
			Menu->SendTo(Plr);
		}break;		
		
	 case GOSSIP_OPTION_ABORTING_ACTION: // Aborting current action
		{
			Plr->Gossip_Complete();
		}break;
	 default:
		sLog.outError("GossipSelectOption","Unknown menuitem %u on npc %u", IntId, pCreature->GetEntry());
		break;
	}	
}

void GossipScript::Destroy()
{
	delete this;
}

void ScriptMgr::register_hook(ServerHookEvents event, void * function_pointer)
{
	ASSERT(event < NUM_SERVER_HOOKS);
	_hooks[event].push_back(function_pointer);
}

/* Hook Implementations */
#define OUTER_LOOP_BEGIN(type, fptr_type) if(!sScriptMgr._hooks[type].size()) { \
	return; } \
	fptr_type call; \
	for(ServerHookList::iterator itr = sScriptMgr._hooks[type].begin(); itr != sScriptMgr._hooks[type].end(); ++itr) { \
	call = ((fptr_type)*itr);

#define OUTER_LOOP_END }

#define OUTER_LOOP_BEGIN_COND(type, fptr_type) if(!sScriptMgr._hooks[type].size()) { \
	return false; } \
	fptr_type call; \
	bool ret_val = true; \
	for(ServerHookList::iterator itr = sScriptMgr._hooks[type].begin(); itr != sScriptMgr._hooks[type].end(); ++itr) { \
		call = ((fptr_type)*itr);

#define OUTER_LOOP_END_COND } return ret_val;

bool HookInterface::OnNewCharacter(uint32 Race, uint32 Class, WorldSession * Session, const char * Name)
{
	OUTER_LOOP_BEGIN_COND(SERVER_HOOK_EVENT_ON_NEW_CHARACTER, tOnNewCharacter)
		ret_val = (call)(Race, Class, Session, Name);
	OUTER_LOOP_END_COND
}

void HookInterface::OnKillPlayer(Player * pPlayer, Player * pVictim)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_KILL_PLAYER, tOnKillPlayer)
		(call)(pPlayer, pVictim);
	OUTER_LOOP_END
}

void HookInterface::OnFirstEnterWorld(Player * pPlayer)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD, tOnFirstEnterWorld)
		(call)(pPlayer);
	OUTER_LOOP_END
}

void HookInterface::OnCharacterCreate(Player * pPlayer)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_CHARACTER_CREATE, tOCharacterCreate)
		(call)(pPlayer);
	OUTER_LOOP_END
}

void HookInterface::OnEnterWorld(Player * pPlayer)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_ENTER_WORLD, tOnEnterWorld)
		(call)(pPlayer);
	OUTER_LOOP_END
}

void HookInterface::OnGuildCreate(Player * pLeader, Guild * pGuild)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_GUILD_CREATE, tOnGuildCreate)
		(call)(pLeader, pGuild);
	OUTER_LOOP_END
}

void HookInterface::OnGuildJoin(Player * pPlayer, Guild * pGuild)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_GUILD_JOIN, tOnGuildJoin)
		(call)(pPlayer, pGuild);
	OUTER_LOOP_END
}

void HookInterface::OnDeath(Player * pPlayer)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_DEATH, tOnDeath)
		(call)(pPlayer);
	OUTER_LOOP_END
}

bool HookInterface::OnRepop(Player * pPlayer)
{
	OUTER_LOOP_BEGIN_COND(SERVER_HOOK_EVENT_ON_REPOP, tOnRepop)
	{
		if(ret_val == true)
			ret_val = (call)(pPlayer);
		else
			(call)(pPlayer);
	}
	OUTER_LOOP_END_COND
}

// void HookInterface::OnEmote(Player * pPlayer, uint32 Emote)
void HookInterface::OnEmote(Player * pPlayer, uint32 Emote, Unit * pUnit) // Marcelo's Patch for Sun ++ and WaadScripts
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_EMOTE, tOnEmote)
		// (call)(pPlayer, Emote);
		(call)(pPlayer, Emote, pUnit); // Marcelo's Patch for Sun ++ and WaadScripts
	OUTER_LOOP_END
}

void HookInterface::OnEnterCombat(Player * pPlayer, Unit * pTarget)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_ENTER_COMBAT, tOnEnterCombat)
		(call)(pPlayer, pTarget);
	OUTER_LOOP_END
}

bool HookInterface::OnCastSpell(Player * pPlayer, SpellEntry* pSpell)
{
	OUTER_LOOP_BEGIN_COND(SERVER_HOOK_EVENT_ON_CAST_SPELL, tOnCastSpell)
	{
		if(ret_val == true)
			ret_val = (call)(pPlayer, pSpell);
		else
			(call)(pPlayer, pSpell);
	}
	OUTER_LOOP_END_COND
}

bool HookInterface::OnLogoutRequest(Player * pPlayer)
{
	OUTER_LOOP_BEGIN_COND(SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST, tOnLogoutRequest)
	{
		if(ret_val == true)
			ret_val = (call)(pPlayer);
		else
			(call)(pPlayer);
	}
	OUTER_LOOP_END_COND
}

void HookInterface::OnLogout(Player * pPlayer)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_LOGOUT, tOnLogout)
		(call)(pPlayer);
	OUTER_LOOP_END
}

// void HookInterface::OnQuestAccept(Player * pPlayer, Quest * pQuest)
void HookInterface::OnQuestAccept(Player * pPlayer, Quest * pQuest, Object * pQuestGiver) // Marcelo's Patch for Sun ++ and WaadScripts
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_QUEST_ACCEPT, tOnQuestAccept)
		// (call)(pPlayer, pQuest);
		(call)(pPlayer, pQuest, pQuestGiver); // Marcelo's Patch for Sun ++ and WaadScripts
	OUTER_LOOP_END
}

void HookInterface::OnZone(Player * pPlayer, uint32 Zone)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_ZONE, tOnZone)
		(call)(pPlayer, Zone);
	OUTER_LOOP_END
}

bool HookInterface::OnChat(Player * pPlayer, uint32 Type, uint32 Lang, const char * Message, const char * Misc)
{
	OUTER_LOOP_BEGIN_COND(SERVER_HOOK_EVENT_ON_CHAT, tOnChat)
	{
		if(ret_val == true)
			ret_val = (call)(pPlayer, Type, Lang, Message, Misc);
		else
			(call)(pPlayer, Type, Lang, Message, Misc);
	}
	OUTER_LOOP_END_COND
}

void HookInterface::OnLoot(Player * pPlayer, Unit * pTarget, uint32 Money, uint32 ItemId)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_LOOT, tOnLoot)
		(call)(pPlayer, pTarget, Money, ItemId);
	OUTER_LOOP_END
}

void HookInterface::OnObjectLoot(Player * pPlayer, Object * pTarget, uint32 Money, uint32 ItemId)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_OBJECTLOOT, tOnObjectLoot)
		(call)(pPlayer, pTarget, Money, ItemId);
	OUTER_LOOP_END
}

void HookInterface::OnEnterWorld2(Player * pPlayer)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_ENTER_WORLD_2, tOnEnterWorld)
		(call)(pPlayer);
	OUTER_LOOP_END
}

void HookInterface::OnQuestCancelled(Player * pPlayer, Quest * pQuest)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_QUEST_CANCELLED, tOnQuestCancel)
		(call)(pPlayer, pQuest);
	OUTER_LOOP_END
}

// void HookInterface::OnQuestFinished(Player * pPlayer, Quest * pQuest) // Marcelo's Patch for Sun ++ and WaadScripts
void HookInterface::OnQuestFinished(Player * pPlayer, Quest * pQuest, Object * pQuestGiver)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_QUEST_FINISHED, tOnQuestFinished)
		//(call)(pPlayer, pQuest);
	if(call) (call)(pPlayer, pQuest, pQuestGiver); // Marcelo's Patch for Sun ++ and WaadScripts
	OUTER_LOOP_END
}

void HookInterface::OnHonorableKill(Player * pPlayer, Player * pKilled)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_HONORABLE_KILL, tOnHonorableKill)
		(call)(pPlayer, pKilled);
	OUTER_LOOP_END
}

void HookInterface::OnArenaFinish(Player* pPlayer, uint32 type, ArenaTeam* pTeam, bool victory, bool rated)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_ARENA_FINISH, tOnArenaFinish)
		(call)(pPlayer, type, pTeam, victory, rated);
	OUTER_LOOP_END
}

void HookInterface::OnAreaTrigger(Player * pPlayer, uint32 areaTrigger)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_AREATRIGGER, tOnAreaTrigger)
		(call)(pPlayer, areaTrigger);
	OUTER_LOOP_END
}

void HookInterface::OnPostLevelUp(Player * pPlayer)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_POST_LEVELUP, tOnPostLevelUp)
		(call)(pPlayer);
	OUTER_LOOP_END
}

void HookInterface::OnPlayerSaveToDB(Player * pPlayer, QueryBuffer* buf)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_PLAYER_SAVE_TO_DB, tOnPlayerSaveToDB)
		(call)(pPlayer, buf);
	OUTER_LOOP_END
}

void HookInterface::OnContinentCreate(MapMgr *pMgr)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_CONTINENT_CREATE, tOnContinentCreate)
		(call)(pMgr);
	OUTER_LOOP_END
}

void HookInterface::OnPostSpellCast(Player *pPlayer, SpellEntry * pSpell, Unit *pTarget)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_POST_SPELL_CAST, tOnPostSpellCast)
		(call)(pPlayer, pSpell, pTarget);
	OUTER_LOOP_END
}

void HookInterface::OnDestroyBuilding(GameObject* go)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_DESTROY_BUILDING,tOnDestroyBuilding)
		(call)(go);
	OUTER_LOOP_END
}

void HookInterface::OnDamageBuilding(GameObject* go)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_DAMAGE_BUILDING,tOnDamageBuilding)
		(call)(go);
	OUTER_LOOP_END
}


SpellScript::SpellScript(Spell *pSpell)
{
	deleted = false;
	_spell = pSpell;
}



SpellScript::~SpellScript()
{  	
	// Suppression du commentaire pour permettre la libération de la mémoire.
	_spell->m_spellScript = NULL;
	_spell = NULL;
	sEventMgr.RemoveEvents(this);	
}

void SpellScript::CallSpellUpdate()
{
	SpellUpdate();
}

void SpellScript::RegisterSpellUpdate(uint32 time)
{
	sEventMgr.AddEvent(this, &SpellScript::SpellUpdate, EVENT_SCRIPT_UPDATE_EVENT, time, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void SpellScript::RemoveSpellUpdate()
{
	sEventMgr.RemoveEvents(this, EVENT_SCRIPT_UPDATE_EVENT);
}

void SpellScript::ModifySpellUpdate(uint32 newtime)
{
	sEventMgr.ModifyEventTimeAndTimeLeft(this, EVENT_SCRIPT_UPDATE_EVENT, newtime);
}

void SpellScript::AddRef(Aura* obj)
{
	Auras.insert(obj);
	obj->m_spellScript=this;
	OnAuraAdd(obj);	
}

void SpellScript::AddRef(DynamicObject* obj)
{
	DynamicObjects.insert(obj);
	obj->m_spellScript=this;
}

void SpellScript::RemoveRef(Aura* obj)
{
	//SpellScript *sthis = this;
	if (Auras.find(obj) != Auras.end())
	{
		obj->m_spellScript = NULL;
		Auras.erase(obj);
	}

	//all auras removed and spell removed?
	TryDelete();
}

void SpellScript::RemoveRef(DynamicObject* obj)
{
	//SpellScript *sthis = this;
	if (DynamicObjects.find(obj) != DynamicObjects.end())
	{
		obj->m_spellScript = NULL;
		DynamicObjects.erase(obj);
	}

	//all auras removed and spell removed?
	TryDelete();
}

void SpellScript::TryDelete()
{
   	if (deleted) return;

	if (Auras.size() == 0 && DynamicObjects.size() == 0)
	{
		deleted = true;
		//SpellScript *sthis = this;
		if (_spell != NULL)
		{
			_spell->m_spellScript = NULL;
		}
	}
}

void EventScript::RegisterEventUpdate(uint32 time)
{
	sEventMgr.AddEvent(this, &EventScript::Update, EVENT_SCRIPT_UPDATE_EVENT, time, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void EventScript::RemoveEventUpdate()
{
	sEventMgr.RemoveEvents(this, EVENT_SCRIPT_UPDATE_EVENT);
}

void EventScript::ModifyEventUpdate(uint32 newtime)
{
	sEventMgr.ModifyEventTimeAndTimeLeft(this, EVENT_SCRIPT_UPDATE_EVENT, newtime);
}

EventScript::~EventScript()
{
	sEventMgr.RemoveEvents(this);
}

int32 SpellScript::event_GetInstanceID()
{
	if (_spell != NULL) return _spell->event_GetInstanceID();
	return -1;	
}

void HookInterface::OnAuraRemove(Player * pPlayer, uint32 spellID)
{
	OUTER_LOOP_BEGIN(SERVER_HOOK_EVENT_ON_AURA_REMOVE, tOnAuraRemove)
		(call)(pPlayer, spellID);
	OUTER_LOOP_END
}