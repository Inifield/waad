/*
 * made by jajcer
 * Adapté AscentWaad par Branruz
 */

#include "StdAfx.h"

#define HORDE 0
#define ALLIANCE 1
#define ROUND_LENGTH 600 //in secs
//--------------------------------------------------------------------------
StrandOfTheAncients::StrandOfTheAncients(MapMgr *mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr,id,lgroup,t)
{
	Attackers = RandomUInt(2)-1;
	BattleRound = 1;
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::Init()
{
 CBattleground::Init();
}
//--------------------------------------------------------------------------
StrandOfTheAncients::~StrandOfTheAncients()
{
}
//--------------------------------------------------------------------------
bool StrandOfTheAncients::HookHandleRepop(Player *plr)
{
	return false;
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::HookOnAreaTrigger(Player *plr, uint32 id)
{
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::HookOnPlayerDeath(Player *plr)
{
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::HookFlagDrop(Player *plr, GameObject *obj)
{
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::HookFlagStand(Player *plr, GameObject *obj)
{
}
//--------------------------------------------------------------------------
bool StrandOfTheAncients::HookSlowLockOpen(GameObject *pGo, Player *pPlayer, Spell *pSpell)
{
	return true;
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::HookOnMount(Player *plr)
{
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::OnAddPlayer(Player *plr)
{
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::OnRemovePlayer(Player *plr)
{
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::OnCreate()
{
	WorldStateManager &WState = m_mapMgr->GetDBStateManager();
	WState.CreateWorldState( SOTA_CAPTURE_BAR_DISPLAY,0 );
	WState.CreateWorldState( SOTA_CAPTURE_BAR_VALUE  ,0 );
	PrepareRound(WState);
	WState.CreateWorldState( SOTA_BONUS_TIME, 0 );
	WState.CreateWorldState( SOTA_TIMER_1, 0 );
	WState.CreateWorldState( SOTA_TIMER_2, 0 );
	WState.CreateWorldState( SOTA_TIMER_3, 0 );
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::PrepareRound(WorldStateManager &WSmgr)
{
	WSmgr.CreateWorldState( SOTA_ALLIANCE_DEFENDER, Attackers == HORDE ? 1 : 0 );
	WSmgr.CreateWorldState( SOTA_ALLIANCE_ATTACKER, Attackers == HORDE ? 0 : 1 );
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::HookOnPlayerKill(Player *plr, Unit *pVictim)
{
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::HookOnHK(Player *plr)
{
}
//--------------------------------------------------------------------------
LocationVector StrandOfTheAncients::GetStartingCoords(uint32 Team)
{
	uint32 sTeam = ( Team == Attackers ? 1 : 0 );
	return LocationVector( SOTAStartLocations[sTeam][0], 
		SOTAStartLocations[sTeam][1],
		SOTAStartLocations[sTeam][2] );
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::OnStart()
{
	SetTime(ROUND_LENGTH, 0);
	sEventMgr.AddEvent(this, &StrandOfTheAncients::TimeTick, EVENT_SOTA_TIMER, MSTIME_SECOND * 5, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::HookGenerateLoot(Player *plr, Corpse *pCorpse)
{
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::HookOnShadowSight() 
{
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::SetIsWeekend(bool isweekend) 
{
}
//--------------------------------------------------------------------------
// time in seconds
void StrandOfTheAncients::SetTime(uint32 secs, uint32 WorldState)
{
	uint32 minutes = secs / TIME_MINUTE;
	uint32 seconds = secs % TIME_MINUTE;
	uint32 digits[3];
	digits[0] = minutes;
	digits[1] = seconds / 10;
	digits[2] = seconds % 10;

	SetWorldState( SOTA_TIMER_1, digits[0] );
	SetWorldState( SOTA_TIMER_2, digits[1] );
	SetWorldState( SOTA_TIMER_3, digits[2] );
	SetRoundTime( secs );
}
//--------------------------------------------------------------------------
void StrandOfTheAncients::TimeTick()
{
	SetTime(GetRoundTime() - 5,0);
	if(GetRoundTime() == 0)
	{
		sEventMgr.RemoveEvents(this, EVENT_SOTA_TIMER);
	}
}
//--------------------------------------------------------------------------