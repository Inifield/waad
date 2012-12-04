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

#ifndef WORLD_ANNOUNCE_H
#define WORLD_ANNOUNCE_H

/*
 * WorldAnnounce Manager
 */
class SERVER_DECL WorldAnnounceMgr : public Singleton < WorldAnnounceMgr >, public EventableObject
{
public:
	WorldAnnounceMgr();
	~WorldAnnounceMgr();

	bool Start();
	bool Stop();

	bool LoadConfig();

	void HookOnAreaTrigger(WorldSession *m_session, uint32 areatrigger); 

	ASCENT_INLINE bool IsRunnable() { return Runnable; }
	ASCENT_INLINE bool IsRunning() { return Running; }
protected:
	void UpdateHandler();

protected:
	bool Running;
	bool Runnable;	// Runnable only if activate in configuration file
	uint32 Refresh;	// Time to refresh and synchronization between memory and database

private:
	uint32 m_Time;
};

/*
 * WorldAnnounce
 */
class WorldAnnounce
{
	friend class WorldAnnounceMgr;
	friend class WorldAnnounceStorage;
public:
	WorldAnnounce(uint32 id, char type, bool timed, uint32 area, uint32 zone, uint32 time, std::string content);
	~WorldAnnounce() { }

public:
	ASCENT_INLINE uint32 Id() { return m_Id; }
	ASCENT_INLINE char Type() { return m_Type; }
	ASCENT_INLINE bool Timed() { return m_Timed; }
	ASCENT_INLINE uint32 AreaTrigger() { return m_AreaTrigger; }
	ASCENT_INLINE uint32 ZoneID() { return m_ZoneID; }
	ASCENT_INLINE uint32 Time() { return m_Time; }
	ASCENT_INLINE std::string Content() { return m_Content; }

	void assign(WorldAnnounce *wa);

	ASCENT_INLINE bool IsAreaTriggered() { return !m_Timed && m_AreaTrigger && (m_Type == 'C' || m_Type== 'c'); }
	ASCENT_INLINE bool IsTimed() { return m_Timed && m_Time && !m_AreaTrigger && (m_Type == 'C' || m_Type == 'c' || m_Type == 'W' || m_Type == 'w'); }
	ASCENT_INLINE bool IsInvalid() { return !IsTimed() && !IsAreaTriggered(); }
protected:

	// Interface used by WorldAnnounceMgr each seconds.
	ASCENT_INLINE bool Update()
	{ 
		return ++m_CurrentTime == m_Time;
	}
	ASCENT_INLINE void Reset()
	{
		m_CurrentTime = 0;
	}

private:
	uint32 m_Id;
	char m_Type;
	bool m_Timed;
	uint32 m_AreaTrigger;	// Not Yet Implemented.
	uint32 m_ZoneID; // Not Yet Implemented.
	uint32 m_Time; // Time between two raise of this announce. Unit is seconds.
	std::string m_Content; // Content of announce.

	uint32 m_CurrentTime;	// Current time in Manager. If currentTime = Time, announce is raised
};

/*
 * WorldAnnounce Storage system
 */
class SERVER_DECL WorldAnnounceStorage : public Singleton < WorldAnnounceStorage >
{
	friend class WorldAnnounceMgr;

public:
	typedef std::vector<WorldAnnounce*>	AnnounceMap;
	typedef std::vector<WorldAnnounce*>::iterator MapIterator;

public:
	WorldAnnounceStorage();
	~WorldAnnounceStorage();

	void Init();
	void Dealloc();
	void Load();

	uint32 CreateNew(char type, bool timed, uint32 area, uint32 zone, uint32 time, const char* content);
	bool Remove(uint32 id);
	bool ModifyContent(uint32 id, char* arg);
	bool ModifyTime(uint32 id, uint32 arg);
	bool ModifyZone(uint32 id, uint32 arg);
	bool ModifyAreatrigger(uint32 id, uint32 arg);
	bool ModifyTimed(uint32 id, bool arg);
	bool ModifyType(uint32 id, char arg);
	
	AnnounceMap GetMap();	// Return a copy of internal storage mapping for All Announces

protected:
	void Refresh();
	WorldAnnounce* GetById(uint32 id);
	ASCENT_INLINE const char* Escape(const char* str);
	WorldAnnounce* GetByAreaTrigger(uint32 area);

protected:
	AnnounceMap Map;
};

#define sWorldAnnounceMgr WorldAnnounceMgr::getSingleton()
#define sWorldAnnounceStorage WorldAnnounceStorage::getSingleton()

#endif
