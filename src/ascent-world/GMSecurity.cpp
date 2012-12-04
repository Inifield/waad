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

#include "StdAfx.h"

initialiseSingleton( GMSecurityMgr );

/*
 *@GMAuthorizations
 */
GMAuthorizations::~GMAuthorizations()
{
}

GMAuthorizations::GMAuthorizations()
{
	m_Group = sGMSecurityMgr.GetGroup((uint8)0);
}

GMAuthorizations::GMAuthorizations(GMAuthorizations& arg)
{
	m_Group = arg.m_Group;
	m_Flags = arg.m_Flags;
}

/*
 *@GMSecurityMgr
 */
GMSecurityMgr::GMSecurityMgr()
{
	m_UniversalChatFlag = 'c';
	m_GamemasterAnnounce = 'u';
}

GMSecurityMgr::~GMSecurityMgr()
{
	Dealloc();
}

void GMSecurityMgr::Init()
{
	BuildFlagsMap();

	GroupMap::iterator itr = m_Groups.find(0);
	if (itr == m_Groups.end())
		m_Groups.insert(make_pair<uint8,GMGroup*>(0, new GMGroup(0)));
	itr = m_Groups.find(255);
	if (itr == m_Groups.end())
	{
		GMGroup* grp = NULL;
		m_Groups.insert(make_pair<uint8,GMGroup*>(255, grp = new GMGroup(255)));
		grp->m_Inherits = m_Flags;
	}
	else
		itr->second->m_Inherits = m_Flags;
	Load();
}

#define sCTS CommandTableStorage::getSingleton()

void GMSecurityMgr::BuildFlagsMap()
{
	m_FlagsMap = new FlagsMap();
	ChatCommand* master = sCTS.Get();

	while ((master != NULL) &&  master->Name) 
	{
		char f = master->CommandGroup;
		m_Flags << f;
		if (master->ChildCommands != NULL)
			BuildFlagsMap(f, master->ChildCommands);
		++master;
	}
}

void GMSecurityMgr::BuildFlagsMap(char m, ChatCommand *childs)
{
	while ((childs != NULL) && childs->Name)
	{
		char f = childs->CommandGroup;
		if (f != m) 
		{
			m_Flags << f;
			DefineInheritedFlags(f)->insert(m);
		}
		++childs;
	}
}

GMFlags* GMSecurityMgr::GetInheritedFlags(const char k) const
{
	FlagsMap::iterator itr = m_FlagsMap->find(k);
	return (itr != m_FlagsMap->end()) ? itr->second : NULL;
}

GMFlags* GMSecurityMgr::DefineInheritedFlags(char k)
{
	GMFlags* set = GetInheritedFlags(k);
	if (set == NULL)
		m_FlagsMap->insert(make_pair<char, GMFlags*>(k, set = new GMFlags()));
	return set;
}

void GMSecurityMgr::Reload()
{
	m_Flags.clear();
	Dealloc();
	Init();
}

void GMSecurityMgr::LoadConf()
{
	std::string lang = Config.MainConfig.GetStringDefault("GMSecurityMgr","UniversalLang","c");
	std::string annc = Config.MainConfig.GetStringDefault("GMSecurityMgr","GamemasterAnnounce","u");

	if (!lang.empty()) m_UniversalChatFlag = lang[0];
	if (!annc.empty()) m_GamemasterAnnounce = annc[0];
}

void GMSecurityMgr::Dealloc()
{
	if (m_FlagsMap != NULL)
	{
		for (FlagsMap::iterator itr = m_FlagsMap->begin(); itr != m_FlagsMap->end(); itr++)
			delete itr->second;
		m_FlagsMap->clear();
		delete m_FlagsMap;
	}
}

void GMSecurityMgr::Load()
{
	QueryResult* result = CharacterDatabase.Query("SELECT * FROM gm_groups");
	if(result)
		do
		{
			Field *f = result->Fetch();
			uint8 gi = 0;
			if (!(gi = f[0].GetUInt8()))
			{
				Log.Notice("GMSecurityMgr","Modification du groupe 0 interdite!");
				continue;
			}
			else if(gi == 255)
			{
				Log.Notice("GMSecurityMgr","Modification du groupe 255 interdite!");
				continue;
			}
			char* flg = (char*)f[1].GetString();
			uint8 inherit = f[2].GetUInt8();
			GMGroup* group = DefineGroup(gi);
			group->m_Inherits.clear();
			GMGroup* igroup = GetGroup(inherit);
			if (igroup)
				group->m_Inherits = igroup->m_Inherits;
			while (*flg) 
			{
				if (m_Flags.find(*flg))
				{
					group->m_Inherits += *flg;
					/// On récupère la liste des flags hérités par le flag en cours.
					GMFlags* igf = GetInheritedFlags(*flg);
					if (igf)
						group->m_Inherits |= *igf;
				}
				++flg;
			}
			Log.Notice("GMSecurityMgr", "- Groupe %d : %s", group->m_Id, Serialize(group->m_Inherits).c_str());
		}
		while(result->NextRow());
	delete result;
	// On ne supprime pas les groupes non redéfinis pendant l'exécution.
	// car on ne peut pas retracer l'utilisation des groupes... les détruires risquerait de provoquer des
	// erreurs système grave (SEGFAULT/ACCESS VIOLATION)
	Log.Notice("GMSecurityMgr", "%d groupes charges.", m_Groups.size());
}

namespace WDFK { uint32 hash_mapper[] = { 0x44614157, 0x45637341, 0x0000744E }; }

GMGroup* GMSecurityMgr::DefineGroup(const uint8 arg)
{
	GMGroup* grp = GetGroup(arg);
	if (!grp)
		m_Groups.insert(make_pair<uint8,GMGroup*>(arg, grp = new GMGroup(arg)));
	return grp;
}

#undef sCTS

GMGroup* GMSecurityMgr::GetGroup(const uint8 arg) const
{
	GroupMap::const_iterator item = m_Groups.find(arg);
	return (item != m_Groups.end()) ? item->second : NULL;
}

GMAuthorizations* GMSecurityMgr::Create(std::string oldsecurity)
{
	GMAuthorizations* gm = new GMAuthorizations();

	// On traite flag par flag oldsecurity string
	size_t size = (int)oldsecurity.length();

	if (size) 
	{
		char *str = (char*)oldsecurity.c_str();
		char *buffer = new char[oldsecurity.length()+1];
		memset(buffer, 0, oldsecurity.length()+1);
		int i = 0;
		while (*str)
		{
			if (*str != ' ') buffer[i++] = *str;
			++str;
		}
		size = strlen(buffer);
		size_t pos = strcspn(buffer, ":");

		char* next_token;
		char* sgrp = strtok_s(buffer,	":",&next_token);
		char* sflg = strtok_s(NULL,	"\0",&next_token);

		if (pos == size)
		{
			Log.Notice("GMSecurityMgr","Ancien format utilise: %s", sgrp);
			sflg = sgrp;
		}
		else 
		{
			gm->m_Group = GetGroup((uint8)atoi(sgrp));
			if (gm->m_Group != NULL) gm->m_Flags = gm->m_Group->m_Inherits;
		}
		
		while (sflg && *sflg) 
		{
			gm->m_Flags << *sflg;
			++sflg;
		}
		delete[] buffer;
	}
	Log.Notice("GMSecurityMgr", "Connexion dans le groupe %d avec les flags { %s }", gm->m_Group->m_Id, Serialize(gm).c_str());
	return gm;
}

std::string GMSecurityMgr::Serialize(GMAuthorizations *auth)
{
	return Serialize(auth, false);
}

std::string GMSecurityMgr::Serialize(GMAuthorizations *auth, bool todb)
{
	if (!auth || !auth->m_Group->m_Id) return string("0:");
	else if (auth->m_Group->m_Id == 255) return string("255:");

	char buffer[1024];
	memset(buffer, 0, 1024);
	
	GMFlags flg = auth->m_Flags;
	if (todb)
		flg ^= auth->m_Group->m_Inherits;
	std::string str = string();
	flg >> str;
	sprintf(buffer, "%d:%s", auth->m_Group->m_Id, str.c_str());
	return string(buffer);
}

std::string GMSecurityMgr::Serialize(GMFlags flags)
{
	std::string strf = string();
	flags >> strf;
	return strf;
}
