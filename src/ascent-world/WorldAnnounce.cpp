/*
 * Waad-Ascent MMORPG Server
 * Copyright (C) 2005-2008 Waad-Ascent Team <http://arbonne.games-rpg.net/>
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
#ifdef WIN32
#include <string.h>
#include <stdio.h>
#endif
#include "StdAfx.h"

initialiseSingleton( WorldAnnounceMgr );
initialiseSingleton( WorldAnnounceStorage );

/*
 * @WorldAnnounceMgr
 */
WorldAnnounceMgr::WorldAnnounceMgr()
{
	Runnable = Running = false;
	Refresh = 3600;
}

WorldAnnounceMgr::~WorldAnnounceMgr()
{
}

bool WorldAnnounceMgr::Start()
{
	bool cond;
	if ((cond = !Running && Runnable))
	{
		EventMgr::getSingleton().AddEvent(this, &WorldAnnounceMgr::UpdateHandler, EVENT_WORLDANNOUCE_CONTROL, 1000, 0, 0);
		Running = true;
	}
	return cond;
}

bool WorldAnnounceMgr::Stop()
{
	bool cond;
	if ((cond = Running))
	{
		EventMgr::getSingleton().RemoveEvents(this);
		Running = false;
	}
	return cond;
}

bool WorldAnnounceMgr::LoadConfig()
{
	bool old = Runnable;
	Runnable = Config.MainConfig.GetBoolDefault("WorldAnnounce", "Enabled", false);
	bool begun = Config.MainConfig.GetBoolDefault("WorldAnnounce", "Started", false);
	Refresh = Config.MainConfig.GetIntDefault("WorldAnnounce", "Refresh", 3600);
	if (Runnable)
	{
		if (!old) 
		{
			sWorldAnnounceStorage.Init();
			sWorldAnnounceStorage.Load();
		}
		if (begun)
			Start();
	}
	else 
	{
		Stop();
		sWorldAnnounceStorage.Dealloc();
	}
	Log.Notice("WorldAnnounce", "Enabled: %s.", (Runnable)?"Oui":"Non");
	Log.Notice("WorldAnnounce", "Refresh: %d sec.", Refresh);
	Log.Notice("WorldAnnounce", "Started: %s.", (begun)?"Oui":"Non");
	Log.Notice("WorldAnnounce", "Status:  %s", ((Running) ? "actif." : "non actif."));
	return true;
}

// Event handler (each second)
void WorldAnnounceMgr::UpdateHandler()
{
	if (++m_Time == Refresh)
	{
		m_Time = 0;
		sWorldAnnounceStorage.Refresh();
	}

	if (!Running || sWorldAnnounceStorage.Map.empty()) return;

	for (WorldAnnounceStorage::MapIterator itr = sWorldAnnounceStorage.Map.begin();
		 itr != sWorldAnnounceStorage.Map.end();
		 itr++)
	{
		int at = 0;
		WorldAnnounce* wa = *itr;
		if (wa->IsAreaTriggered()) continue;
		if (wa->Update())
		{
			wa->Reset();
			switch (wa->Type())
			{
				case 'c' :
				case 'C' : at = 1; break;
				case 'w' :
				case 'W' : at = 2; break;
			}
			if (wa->ZoneID() && at != 2)
				at = 3;

			switch (at)
			{
				case 3: // Zone
					// NOT YET IMPLEMENTED
					break;
				case 2:
					sWorld.SendWorldWideScreenText(wa->Content().c_str());
					break;
				case 1:
					sWorld.SendWorldText(wa->Content().c_str());
					break;
				default:
					Log.Notice("WorldAnnounceManager", "Format d'annonce automatique inconnu %c.", wa->Type());
					/// This announce cannot be preserved.
					sWorldAnnounceStorage.Map.erase(itr);
					delete wa;
					break;
			}
		}
	}
}

void WorldAnnounceMgr::HookOnAreaTrigger(WorldSession *m_session, uint32 areatrigger)
{
	if (!Running || sWorldAnnounceStorage.Map.empty() || !areatrigger) return;
	WorldAnnounce *wa = sWorldAnnounceStorage.GetByAreaTrigger(areatrigger);
	if (wa != NULL)
		m_session->SystemMessage(wa->Content().c_str());
}

/*
 * @WorldAnnounceStorage
 */
WorldAnnounceStorage::WorldAnnounceStorage()
{
}

WorldAnnounceStorage::~WorldAnnounceStorage()
{
	Dealloc();
}

void WorldAnnounceStorage::Init()
{
}

void WorldAnnounceStorage::Load()
{
	if (!sWorldAnnounceMgr.IsRunnable()) return;
	QueryResult * result = WorldDatabase.Query("SELECT * FROM world_announce ORDER BY id");
	if(result)
		do
		{
			Field *f = result->Fetch();
			// check flags to determine if announce is enabled or "not yet implemented"
			WorldAnnounce *wa = new WorldAnnounce( f[0].GetUInt32(), f[1].GetString()[0], f[2].GetBool(), f[3].GetUInt32(), f[4].GetUInt32(), f[5].GetUInt32(), f[6].GetString() );
			if (wa->IsInvalid())
				delete wa;
			else
				Map.push_back( wa ) ;
		}
		while(result->NextRow());
	delete result;
	Log.Notice("WorldAnnounceManager", "%d Annonces automatiques chargees.", Map.size());
}

void WorldAnnounceStorage::Dealloc()
{
	if (Map.empty()) return;
	for (MapIterator it = Map.begin(); it != Map.end(); it++)
		delete (*it);
	Map.clear();
}

void WorldAnnounceStorage::Refresh()
{
	QueryResult * result = WorldDatabase.Query("SELECT * FROM world_announce ORDER BY id");
	if(result)
		do
		{
			WorldAnnounce *cwa = NULL;
			WorldAnnounce *mwa = NULL;
			Field *f = result->Fetch();
			bool fetch = false;
			cwa = new WorldAnnounce( f[0].GetUInt32(), f[1].GetString()[0], f[2].GetBool(), f[3].GetUInt32(), f[4].GetUInt32(), f[5].GetUInt32(), f[6].GetString() );
		
			// check if announce is already in memory
			for (MapIterator it = Map.begin(); it != Map.end(); it++)
			{
				if ((mwa = *it)->Id() != f[0].GetUInt32()) 
					continue;
				fetch = true;
				// announce is in memory... we check memory status and database status
				if (cwa->IsInvalid())
				{
					delete cwa;
					delete mwa;
					Map.erase(it);
				}
				else 
					mwa->assign( cwa ) ;
				break; 
			}
			if (!fetch)
			{
				if (cwa->IsInvalid())
					delete cwa;
				else
					Map.push_back( cwa ) ;
			}
		}
		while(result->NextRow());
	delete result;
}

WorldAnnounceStorage::AnnounceMap WorldAnnounceStorage::GetMap()
{
	AnnounceMap result;
	result.assign( Map.begin(), Map.end() );
	return result;
}

uint32 WorldAnnounceStorage::CreateNew(char type, bool timed, uint32 area, uint32 zone, uint32 time, const char* content)
{
	uint32 id;

	WorldAnnounce *wa = new WorldAnnounce(0, type, timed, area, zone, time, content);
	if (wa->IsInvalid())
	{
		delete wa;
		return 0;
	}
	delete wa;

	WorldDatabase.Query(
		"INSERT INTO world_announce (`type`,`timed`,`areatrigger`,`zoneid`,`time`,`content`) VALUES ('%c',%u,%u,%u,%u,'%s')", 
		type, timed, area, zone, time, WorldDatabase.EscapeString(content).c_str()
	);
	QueryResult* qr = WorldDatabase.Query("SELECT max(id) FROM world_announce");
	if (qr)
	{
		Field *f = qr->Fetch();
		id = f[0].GetUInt32();
	}
	else
		return 0;

	delete qr;
	Map.push_back( new WorldAnnounce(id, type, timed, area, zone, time, content) );
	return id;
}

bool WorldAnnounceStorage::Remove(uint32 id)
{
	WorldAnnounce* wa;
	for (MapIterator it = Map.begin(); it != Map.end(); it++)
		if ((wa = *it)->Id() == id)
		{
			Map.erase(it);
			WorldDatabase.Query("DELETE FROM world_announce WHERE id=%u", id);
			delete wa;
			return true;
		}
	return false;
}

WorldAnnounce* WorldAnnounceStorage::GetById(uint32 id)
{
	if (!id) return NULL;
	for (MapIterator it = Map.begin(); it != Map.end(); it++)
		if ((*it)->Id() == id) return (WorldAnnounce*)*it;
	return NULL;
}

WorldAnnounce* WorldAnnounceStorage::GetByAreaTrigger(uint32 id)
{
	if (!id) return NULL;
	for (MapIterator it = Map.begin(); it != Map.end(); it++)
		if ((*it)->AreaTrigger() == id) return (WorldAnnounce*)*it;
	return NULL;
}

const char* WorldAnnounceStorage::Escape(const char *str)
{
	return WorldDatabase.EscapeString(str).c_str();
}

bool WorldAnnounceStorage::ModifyContent(uint32 id, char* arg)
{
		WorldAnnounce* wa = GetById(id);
		if(!wa) return false;
		wa->m_Content = (char*)arg;
		WorldDatabase.Query("UPDATE world_announce SET content='%s' WHERE id=%u", Escape(wa->m_Content.c_str()), id);
		return true;
}

bool WorldAnnounceStorage::ModifyTime(uint32 id, uint32 arg)
{
		WorldAnnounce* wa = GetById(id);
		if(!wa) return false;
		wa->m_Time = (uint32)arg;
		WorldDatabase.Query("UPDATE world_announce SET time=%u WHERE id=%u", wa->m_Time, id);
		return true;
}

bool WorldAnnounceStorage::ModifyZone(uint32 id, uint32 arg)
{
		WorldAnnounce* wa = GetById(id);
		if(!wa) return false;
		wa->m_ZoneID = (uint32)arg;
		WorldDatabase.Query("UPDATE world_announce SET zoneid=%u WHERE id=%u", wa->m_ZoneID, id);
		return true;
}

bool WorldAnnounceStorage::ModifyAreatrigger(uint32 id, uint32 arg)
{
		WorldAnnounce* wa = GetById(id);
		if(!wa) return false;
		wa->m_AreaTrigger = (uint32)arg;
		WorldDatabase.Query("UPDATE world_announce SET areatrigger=%u WHERE id=%u", wa->m_AreaTrigger, id);
		return true;
}

bool WorldAnnounceStorage::ModifyTimed(uint32 id, bool arg)
{
		WorldAnnounce* wa = GetById(id);
		if(!wa) return false;
		wa->m_Timed = (bool)arg;
		WorldDatabase.Query("UPDATE world_announce SET timed=%u WHERE id=%u", wa->m_Timed, id);
		return true;
}

bool WorldAnnounceStorage::ModifyType(uint32 id, char arg)
{
		WorldAnnounce* wa = GetById(id);
		if(!wa) return false;
		wa->m_Type = (char)arg;
		WorldDatabase.Query("UPDATE world_announce SET type='%c' WHERE id=%u", wa->m_Type, id);
		return true;
}

/*
 * @WorldAnnounce
 */
WorldAnnounce::WorldAnnounce(uint32 id, char type, bool timed, uint32 area, uint32 zone, uint32 time, std::string content)
{
	m_Id = id;
	m_Type = type;
	m_Timed = timed;
	m_AreaTrigger = area;
	m_ZoneID = zone;
	m_Time = time;
	m_CurrentTime = 0;
	m_Content = content;
}

void WorldAnnounce::assign(WorldAnnounce *wa)
{
	m_Type = wa->m_Type;
	m_Timed = wa->m_Timed;
	m_AreaTrigger = wa->m_AreaTrigger;
	m_ZoneID = wa->m_ZoneID;
	m_Content = wa->m_Content;

	m_Time = wa->m_Time;
	if (m_CurrentTime >= m_Time) m_CurrentTime = m_Time - 1;

	delete wa;
}

/*
 * @ChatHandler
 */
#define ERET(ses,msg)		{ RedSystemMessage(ses,msg); return true; }
#define ERETP(ses,msg,par)	{ RedSystemMessage(ses,msg,par); return true; }

bool ChatHandler::HandleWorldAnnounceList(const char * args, WorldSession * m_session)
{
	WorldAnnounceStorage::AnnounceMap list = sWorldAnnounceStorage.GetMap();

	SystemMessage(m_session, "Liste des annonces automatiques gerees...");
	if (list.empty())
		ERET(m_session, " pas d'annonce active.");

	for (WorldAnnounceStorage::MapIterator it = list.begin(); it != list.end(); it++)
		SystemMessage(m_session, " %d - <%c/%d/%u/%u/%u> %s", (*it)->Id(), (*it)->Type(), (*it)->Timed(), (*it)->AreaTrigger(), (*it)->ZoneID(), (*it)->Time(), (*it)->Content().c_str());

	return true;
}

bool ChatHandler::HandleWorldAnnounceStart(const char * args, WorldSession * m_session)
{
	if (!sWorldAnnounceMgr.IsRunnable())
		ERET(m_session, "Gestionnaire d'Annonces automatiques desactive.");

	if (sWorldAnnounceMgr.Start())
	{
		GreenSystemMessage(m_session, "Gestionnaire d'Annonces automatiques demarre.");
		sGMLog.writefromsession(m_session, "Gestionnaire d'annonces demarre par %s", m_session->GetPlayer()->GetName());
	}
	else
		RedSystemMessage(m_session, "Gestionnaire d'Annonces automatiques est deja demarre.");
	return true;
}

bool ChatHandler::HandleWorldAnnounceStop(const char * args, WorldSession * m_session)
{
	if (!sWorldAnnounceMgr.IsRunnable())
		ERET(m_session, "Gestionnaire d'Annonces automatiques desactive.");

	if (sWorldAnnounceMgr.Stop())
	{
		GreenSystemMessage(m_session, "Gestionnaire d'Annonces automatiques stoppe.");
		sGMLog.writefromsession(m_session, "Gestionnaire d'annonces stoppe par %s", m_session->GetPlayer()->GetName());
	}
	else
		RedSystemMessage(m_session, "Gestionnaire d'Annonces automatiques n'est pas demarre.");
	return true;
}

#define FTOK(str,seq,nxttoken)	strtok_s((str),(seq),(nxttoken))
#define NTOK(seq,nxttoken)		strtok_s((NULL),(seq),(nxttoken))
bool ChatHandler::HandleWorldAnnounceCreate(const char *args, WorldSession * m_session)
{
	if (!sWorldAnnounceMgr.IsRunnable())
		RedSystemMessage(m_session,"Gestionnaire d'Annonces automatiques desactive.");

	if (!*args) 
	{
		BlueSystemMessage(m_session, "usage: create <time,area,zone> <parameters...>");
		SystemMessage(m_session, " time <type C:W> <time> <message...> : Creation d'une annonce temporisee générale.");
		SystemMessage(m_session, " area <type C:W> <areatrigger> <zoneid> <message...> : Creation d'une annonce sur un areatrigger.");
		SystemMessage(m_session, " zone <type C:W> <time> <zoneid> <message...> : Creation d'une annonce temporisee de zone.");
		return true;
	}

	char *next_token;
	char *comm = FTOK((char *)args," ",&next_token);

	char *type = NTOK(" ",&next_token);
	if ((type[0]!='C') && (type[0]!='W') && (type[0]!='c') && (type[0]!='w')) ERET(m_session, "Type manquant ou errone!");

	if(!strcmp(comm, "time")) // Format: "time C 60 Test annonce"
	{
		// Recup et separation des parametres restant
		char *temps = NTOK(" ",&next_token);
		uint32 time_announce = atoi(temps);
		if (!time_announce)
			ERET(m_session, "Les temps doivent etre exprimes en secondes et different de 0."); 

		char *message = NTOK("\0",&next_token);
		if(!message || !*message)
			ERET(m_session, "Le message de l'annonce n'existe pas."); 
		if(strlen(message) > 254) 
			ERET(m_session, "Le message de l'annonce est trop long! ( > 255 caracteres )"); 

		uint32 id = sWorldAnnounceStorage.CreateNew(type[0], true, 0, 0, time_announce, message);
		if(id) 
			GreenSystemMessage(m_session, "Annonce %u creee.", id);
		else
			ERET(m_session, "L'annonce n'a pas ete cree."); 
	}
	else if(!strcmp(comm, "area")) // "area C 4231 Test annonce"
	{
		// Recup et separation des parametres restant
		char *areatrig = NTOK(" ",&next_token);
		uint32 num_areatrig = atoi(areatrig);
		if(!num_areatrig) 
			ERET(m_session, "Areatrigger nul!");

		char *message = NTOK("\0",&next_token);
		if(!message || !*message) ERET(m_session, "Le message de l'annonce n'existe pas."); 
		if(strlen(message) > 254) 
			ERET(m_session, "Le message de l'annonce est trop long! ( > 255 caracteres )"); 

		uint32 id = sWorldAnnounceStorage.CreateNew(type[0], false, num_areatrig, 0, 0, message);
		if(id) GreenSystemMessage(m_session, "Annonce %u creee.", id);
		else   ERET(m_session, "L'Annonce n'a pas ete cree."); 
	}
	else if(!strcmp(comm, "zone")) // "zone C 60 255 Test annonce"
	{
		RedSystemMessage(m_session, "Cette fonctionnalité n'est pas encore prise en charge...");
		/*
		// Recup et separation des parametres restant
		char *temps = strtok_s(NULL," ",&next_token);
		uint32 time_annouce = atoi(temps);
		if (!time_annouce)
		ERET(m_session, "Les temps doivent etre exprimes en secondes et different de 0."); 

		char *zone_id = strtok_s(NULL," ",&next_token);
		uint32 Num_zone = atoi(zone_id);
		if (!Num_zone) ERET(m_session, "La zone 0 n'est pas une zone valide.");

		char *message = strtok_s(NULL,"\0",&next_token);
		if(!message) ERET(m_session, "Le message de l'annonce n'existe pas."); 
		if(strlen(message) > 254) 
		ERET(m_session, "Le message de l'annonce est trop long! ( > 255 caracteres )"); 

		uint32 id = sWorldAnnounceStorage.CreateNew(type[0], true, 0, zone_id, time_annouce, message);
		if (!id) ERET(m_session, "Echec à la création de l'annonce.");
		else     GreenSystemMessage(m_session, "Annonce %u creee.", id);
		*/
	}
	else RedSystemMessage(m_session, "Type d'annonce non supporte.");

	return true;
}

bool ChatHandler::HandleWorldAnnounceModify(const char * args, WorldSession * m_session)
{
	if (!sWorldAnnounceMgr.IsRunnable())
		ERET(m_session, "Gestionnaire d'Annonces automatiques desactive.");

	if (!*args)
	{
		BlueSystemMessage(m_session, "usage: modify EntryId <type,timed,area,zone,time,content> <parameter>");
		SystemMessage(m_session, " type : Modifie le type de l'annonce (C/W), ex: modify 4 type C"); 
		SystemMessage(m_session, " timed : Modifie le flag 'temporisee' de l'annonce (1/0), ex: modify 4 timed 0"); 
		SystemMessage(m_session, " area : Modifie l'areatrigger de l'annonce (0: pas d'areatrigger), ex: modify 4 area 3341"); 
		SystemMessage(m_session, " zone : Modifie la zone d'emission de l'annonce (0: pour une annonce globale sauf si areatriggered), ex: modify 4 zone 50"); 
		SystemMessage(m_session, " time : Modifie le temps en seconde de recurrence de l'annonce (0: pour desactiver la recurrence temporisee), ex: modify 4 time 10"); 
		SystemMessage(m_session, " msg  : Modifie le texte de l'annonce., ex: modify 4 msg new texte annonce"); 
		return true;
	}
	// entryId param1 param2"); 
	char *next_token;
	char *entryId = FTOK((char *)args," ",&next_token);
	if (!entryId || !*entryId)
		ERET(m_session, "Veuillez specifier l'EntryId de l'annonce.");
	uint32 id = atoi(entryId);
	if (!id)
		ERET(m_session, "Id d'annonce invalide... Consultez la liste pour plus ample details.");

	char *comm = NTOK(" ",&next_token); // param1
	if(!comm || !*comm) 
		ERET(m_session, "Type de modification manquant!");

	char *param2 = NTOK("\0",&next_token);
	if(!param2 || !*param2) 
		ERET(m_session, "Valeur de modification manquante!");

	if(!strcmp(comm, "time"))
	{
		uint32 time = atoi(param2);
		if(!time)
			BlueSystemMessage(m_session, "attention: le temps defini est null (0).");
		if(!sWorldAnnounceStorage.ModifyTime(id, time))
			ERETP(m_session, "Echec de la modification de l'annonce %u.", id);
	}
	else if(!strcmp(comm, "zone"))
	{
		ERET(m_session, "Cette fonctionnalite n'est pas encore implementee.");
		/*
		uint32 zone = atoi(param2);
		if(!zone)
			BlueSystemMessage(m_session, "attention: la zone definie est invalie (0).");
		if(!sWorldAnnounceStorage.ModifyZone(id, zone))
			ERETP(m_session, "Echec de la modification de l'annonce %u.", id);
		 */
	}
	else if(!strcmp(comm, "msg"))
	{
		if(!param2 || !*param2)
			BlueSystemMessage(m_session, "attention: le message defini est vide.");
		if(strlen(param2)>254)
			ERET(m_session, "Le message de l'annonce est trop long ( > 255).");

		if(!sWorldAnnounceStorage.ModifyContent(id, param2))
			ERETP(m_session, "Echec de la modification de l'annonce %u.", id);
	}
	else if(!strcmp(comm, "timed"))
	{
		bool timed = (atoi(param2)) ? true : false;

		if(!timed)
			BlueSystemMessage(m_session, "attention: le flag timed est defini à 0 (false).");
		if(!sWorldAnnounceStorage.ModifyTimed(id, timed))
			ERETP(m_session, "Echec de la modification de l'annonce %u.", id);
	}
	else if(!strcmp(comm, "type"))
	{
		if(!param2 || !*param2) ERET(m_session, "Type d'annonce manquant.");

		if(!sWorldAnnounceStorage.ModifyType(id, *param2))
			ERETP(m_session, "Echec de la modification de l'annonce %u.", id);
	}
	else ERET(m_session, "Type de modification non supporte.");

	GreenSystemMessage(m_session, "Modification de l'annonce %u effectuee.");
	return true;
}

bool ChatHandler::HandleWorldAnnounceRemove(const char * args, WorldSession * m_session)
{
	if (!sWorldAnnounceMgr.IsRunnable())
		ERET(m_session, "Gestionnaire d'Annonces automatiques desactive.");

	if(!*args) ERET(m_session, "Parametre manquant.");

	uint32 id = atoi(args);
	if(!id)
		ERET(m_session, "L'id de l'annonce doit differer de 0.");

	if(sWorldAnnounceStorage.Remove(id))
		GreenSystemMessage(m_session, "Annonce %u correctement supprimee.", id);
	else
		RedSystemMessage(m_session, "Echec a la suppression de l'annonce %u.", id);
	return true;
}

#undef FTOK
#undef NTOK
#undef ETOK
#undef ERET
#undef ERETP
