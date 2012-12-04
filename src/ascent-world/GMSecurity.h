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

#ifndef _GM_SECURITY_H
#define _GM_SECURITY_H

// Forward
class GMSecurityMgr;
typedef WDFK::set<char> GMFlags;

class GMGroup
{
	friend class GMSecurityMgr;
public:
	ASCENT_INLINE uint8 Id() { return m_Id; }
	ASCENT_INLINE GMFlags& Flags() { return m_Inherits; }
	ASCENT_INLINE bool IsGM() { return (m_Id != 0 && !m_Inherits.empty() && m_Inherits.find('m')) || (m_Id == 255); }
	ASCENT_INLINE bool IsAdmin() { return (m_Id != 0 && !m_Inherits.empty() && m_Inherits.find('a')) || (m_Id == 255); }
	ASCENT_INLINE bool IsSuperAdmin() { return (m_Id != 0 && !m_Inherits.empty() && m_Inherits.find('A')) || (m_Id == 255); }

protected:
	GMGroup(uint8 id): m_Id(id) {}

private:
	uint8 m_Id;
	GMFlags m_Inherits;
};

/*
 * GM Authorizations flags
 */
class GMAuthorizations
{
	friend class GMSecurityMgr;
public:
	GMAuthorizations();
	GMAuthorizations(GMAuthorizations& arg);
	~GMAuthorizations();

	ASCENT_INLINE GMGroup* Group() const { return m_Group; }
	/// Flags inherited by group and personal flags
	GMFlags& Flags() { return m_Flags; }
	ASCENT_INLINE bool IsGM() { return m_Group->IsGM() || m_Flags.find('m'); }
	ASCENT_INLINE bool IsAdmin() { return m_Group->IsAdmin() || m_Flags.find('a'); }
	ASCENT_INLINE bool IsSuperAdmin() { return m_Group->IsSuperAdmin() || m_Flags.find('A'); }
	ASCENT_INLINE bool CanUse(char cmd) { return (cmd) ? m_Flags.find(cmd) : m_Group->Id() != 0 || !m_Flags.empty(); } 

protected:
	GMGroup* m_Group;
	GMFlags m_Flags;
};

/*
 * GM-Security Manager
 *
 * offer many functions to handle GM Authorizations.
 */
class SERVER_DECL GMSecurityMgr : public Singleton < GMSecurityMgr >
{
	typedef HM_NAMESPACE::hash_map<uint8, GMGroup*> GroupMap;
	typedef HM_NAMESPACE::hash_map<char, GMFlags*> FlagsMap;
public:
	GMSecurityMgr();
	~GMSecurityMgr();

	void LoadConf();
	void Init();
	void Load();
	void Reload();
	void Dealloc();

	GMAuthorizations* Create(std::string oldsecurity);
	GMGroup* GetGroup(const uint8 arg) const;
	GMFlags* GetInheritedFlags(char k) const;
	std::string Serialize(GMAuthorizations* auth);
	std::string Serialize(GMAuthorizations* auth, bool todb);

	ASCENT_INLINE bool UseUniversalLang(GMAuthorizations* auth) { return auth && auth->CanUse('c'); }
	ASCENT_INLINE bool UseGamemasterAnnounce(GMAuthorizations* auth) { return auth && auth->CanUse('u'); }

private:
	void BuildFlagsMap();
	void BuildFlagsMap(char m, ChatCommand* childs);
	GMFlags* DefineInheritedFlags(char k);
	GMGroup* DefineGroup(const uint8 arg);
	std::string Serialize(GMFlags flags);

private:
	GroupMap m_Groups;
	GMFlags m_Flags;
	FlagsMap* m_FlagsMap;
	char m_UniversalChatFlag;
	char m_GamemasterAnnounce;
};

#define sGMSecurityMgr GMSecurityMgr::getSingleton()

#endif
