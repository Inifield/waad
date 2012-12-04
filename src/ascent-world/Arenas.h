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
/**
 * Arena World States
 */
#define WORLDSTATE_ARENA_BLADESEDGE_GREEN_PLAYER_COUNT			2544
#define WORLDSTATE_ARENA_BLADESEDGE_GOLD_PLAYER_COUNT			2545
#define WORLDSTATE_ARENA_BLADESEDGE_SCORE_SHOW					2547

#define WORLDSTATE_ARENA_NAGRAND_GREEN_PLAYER_COUNT				2576
#define WORLDSTATE_ARENA_NAGRAND_GOLD_PLAYER_COUNT				2575
#define WORLDSTATE_ARENA_NAGRAND_SCORE_SHOW						2577

#define WORLDSTATE_ARENA_LORDAERON_GREEN_PLAYER_COUNT			3001
#define WORLDSTATE_ARENA_LORDAERON_GOLD_PLAYER_COUNT			3000
#define WORLDSTATE_ARENA_LORDAERON_SCORE_SHOW					3002

#define WORLDSTATE_ARENA_WOTLK_GREEN_PLAYER_COUNT				3600
#define WORLDSTATE_ARENA_WOTLK_GOLD_PLAYER_COUNT				3601
#define WORLDSTATE_ARENA_WOTLK_SCORE_SHOW						3610


class ArenaTeam;

class Arena : public CBattleground
{
	set<GameObject*> m_gates;
	GameObject * m_buffs[2];
	uint32 m_pcWorldStates[2];
	//set<ArenaTeam*> doneteams;
	bool m_started;
	uint32 m_arenateamtype;
	uint32 m_playersCount[2];
	set<uint32> m_players2[2];
	set<uint32> m_playersAlive;
public:
	bool rated_match;
	Arena(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t, uint32 players_per_side);
	virtual ~Arena();
	virtual void Init();

	bool HookHandleRepop(Player * plr);
	void OnAddPlayer(Player * plr);
	void OnRemovePlayer(Player * plr);
	void OnCreate();
	void HookOnPlayerDeath(Player * plr);
	void HookOnPlayerKill(Player * plr, Unit * pVictim);
	void HookOnHK(Player * plr);
	void HookOnShadowSight();
	void UpdatePlayerCounts();
	LocationVector GetStartingCoords(uint32 Team);
	virtual const char * GetName() { return "Arene"; }
	void OnStart();
	bool CanPlayerJoin(Player * plr)
	{
		if(m_started)
			return false;
		else
			return CBattleground::CanPlayerJoin(plr);
	}

	bool CreateCorpse(Player * plr) { return false; }

	/* dummy stuff */
	void HookOnMount(Player * plr) {}
	void HookFlagDrop(Player * plr, GameObject * obj) {}
	void HookFlagStand(Player * plr, GameObject * obj) {}
	void HookOnAreaTrigger(Player * plr, uint32 id);

	int32 GetFreeTeam()
	{
		size_t c0 = m_players[0].size()  + m_pendPlayers[0].size();
		size_t c1 = m_players[1].size() + m_pendPlayers[1].size();
		if(m_started) return -1;

		// Check if there is free room, if yes, return team with less members
		return ((c0 + c1 >= m_playerCountPerTeam * 2) ? -1 : (c0 > c1));

		/* We shouldn't reach here. */
	}

	void Finish();
	uint8 Rated() { return rated_match; }
	ASCENT_INLINE uint32 GetArenaTeamType() { return m_arenateamtype; }
	uint32 CalcDeltaRating(uint32 oldRating, uint32 opponentRating, bool outcome);
	int32 m_teams[2]; // Ids of teams fighting in rated match
};
