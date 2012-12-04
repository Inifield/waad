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
// Debug des Spells appartenant aux PNJs/Creatures,
//#define _AI_SPELL_DEBUG	1
//---
// only enable with COLLISION define.
#ifdef COLLISION
#define LOS_CHECKS 1
//#define LOS_ONLY_IN_INSTANCE 1
#endif

#ifdef WIN32
#define HACKY_CRASH_FIXES 1		// SEH stuff
#endif

AIInterface::AIInterface()
{
	m_moveDelayTimer = 0;
	m_backupbytes2 = 0;
	m_neutralGuard = false;
	MindControlOwner = NULL;
	m_canDie = true;
	m_waypoints = NULL;
	m_canMove = true;
	m_destinationX = m_destinationY = m_destinationZ = 0;
	m_nextPosX = m_nextPosY = m_nextPosZ = 0;
	UnitToFollow = NULL;
	FollowDistance = 0.0f;
	m_fallowAngle = float(M_PI/2);
	m_timeToMove = 0;
	m_timeMoved = 0;
	m_moveTimer = 0;
	m_WayPointsShowing = false;
	m_WayPointsShowBackwards = false;
	m_currentWaypoint = 0;
	m_moveBackward = false;
	m_moveType = 0;
	m_moveRun = false;
	m_moveSprint = false;
	m_moveFly = false;
	m_creatureState = STOPPED;
	m_canCallForHelp = false;
	m_hasCalledForHelp = false;
	m_fleeTimer = 0;
	m_FleeDuration = 0;
	m_canFlee = false;
	m_hasFleed = false;
	m_canRangedAttack = false;
	m_FleeHealth = m_CallForHelpHealth = 0.0f;
	m_AIState = STATE_IDLE;

	m_updateAssist = false;
	m_updateTargets = false;
	m_updateAssistTimer = 1;
	m_updateTargetsTimer = TARGET_UPDATE_INTERVAL;

	m_nextSpell = NULL;
	m_nextTarget = NULL;
	totemspell = NULL;
	m_Unit = NULL;
	m_PetOwner = NULL;
	m_aiCurrentAgent = AGENT_NULL;
	m_runSpeed = 0.0f;
	m_flySpeed = 0.0f;
	UnitToFear = NULL;
	firstLeaveCombat = true;
	m_outOfCombatRange = 2500;

	tauntedBy = NULL;
	isTaunted = false;
	soullinkedWith = NULL;
	isSoulLinked = false;
	m_AllowedToEnterCombat = true;
	m_totalMoveTime = 0;
	m_lastFollowX = m_lastFollowY = 0;
	m_FearTimer = 0;
	m_WanderTimer = 0;
	m_totemspelltime = 0;
	m_totemspelltimer = 0;
	m_formationFollowAngle = 0.0f;
	m_formationFollowDistance = 0.0f;
	m_formationLinkTarget = 0;
	m_formationLinkSqlId = 0;
	m_currentHighestThreat = 0;

	disable_combat = false;

	disable_melee = false;
	disable_ranged = false;
	disable_spell = false;

	disable_targeting = false;

	next_spell_time = 0;
	waiting_for_cooldown = false;
	UnitToFollow_backup = NULL;
	m_isGuard = false;
	m_is_in_instance=false;
	skip_reset_hp=false;

	MiscEvent = true; // Note Randdrick : Initialisation de MiscEvent.
}

void AIInterface::Init(Unit *un, AIType at, MovementType mt)
{
	ASSERT(at != AITYPE_PET);

	m_AIType = at;
	m_MovementType = mt;

	m_AIState = STATE_IDLE;
	m_MovementState = MOVEMENTSTATE_STOP;

	m_Unit = un;

	m_walkSpeed = m_Unit->m_walkSpeed*0.001f;//move distance per ms time 
	m_runSpeed = m_Unit->m_runSpeed*0.001f;//move distance per ms time 
	m_flySpeed = m_Unit->m_flySpeed * 0.001f;
	/*if(!m_DefaultMeleeSpell)
	{
		m_DefaultMeleeSpell = new AI_Spell;
		m_DefaultMeleeSpell->entryId = 0;
		m_DefaultMeleeSpell->spellType = 0;
		m_DefaultMeleeSpell->agent = AGENT_MELEE;
		m_DefaultSpell = m_DefaultMeleeSpell;
	}*/
	m_sourceX = un->GetPositionX();
	m_sourceY = un->GetPositionY();
	m_sourceZ = un->GetPositionZ();
	m_guardTimer = getMSTime();

	/*if (un->getLevel() > 50) //default is 50 yards, lets not lower :P
	{
		m_outOfCombatRange = un->getLevel() * un->getLevel();
		if (un->IsCreature() && static_cast<Creature*>(un)->GetCreatureName() != NULL &&static_cast<Creature*>(un)->GetCreatureName()->Rank > 0)
			m_outOfCombatRange = m_outOfCombatRange << 1; //multiply by 2
	}*/	
}

AIInterface::~AIInterface()
{
	//Log.Success("AIInterface","Class Destructor Call");
	//Destruction de la liste des spells pour les pets
	for(list<AI_Spell*>::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
		if((*itr)->custom_pointer)
			delete (*itr);

	//Destruction du tableau des spells pour les npcs
	for(vector<AI_Spell*>::iterator itr = npc_spells.begin(); itr != npc_spells.end(); ++itr)
		if((*itr)->custom_pointer)
			delete (*itr);

	//remove our mind control childs
	for (std::set<uint64>::iterator itr=MindControlChilds.begin(); itr!=MindControlChilds.end(); ++itr)
	{
		Unit* u=m_Unit->GetMapMgr()->GetUnit(*itr);

		if (u == NULL || u->GetAIInterface() == NULL || u->GetAIInterface()->MindControlAura == NULL)
			continue;

		u->GetAIInterface()->MindControlAura->Remove();
		//sEventMgr.AddEvent(u->GetAIInterface()->MindControlAura, &Aura::Remove, true, EVENT_AURA_REMOVE, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}			
}

void AIInterface::Init(Unit *un, AIType at, MovementType mt, Unit *owner)
{
	ASSERT((at == AITYPE_PET) || (at == AITYPE_TOTEM) );

	m_AIType = at;
	m_MovementType = mt;

	m_AIState = STATE_IDLE;
	m_MovementState = MOVEMENTSTATE_STOP;

	m_Unit = un;
	m_PetOwner = owner;

	m_walkSpeed = m_Unit->m_walkSpeed*0.001f;//move distance per ms time 
	m_runSpeed = m_Unit->m_runSpeed*0.001f;//move/ms
	m_flySpeed = m_Unit->m_flySpeed*0.001f;
	m_sourceX = un->GetPositionX();
	m_sourceY = un->GetPositionY();
	m_sourceZ = un->GetPositionZ();
}

void AIInterface::HandleEvent(uint32 event, Unit* pUnit, uint32 misc1)
{
	if( m_Unit == NULL ) return;
	
    // Zone de debug (Branruz)
	/*
	switch(event)
	{
	 case EVENT_ENTERCOMBAT:    Log.Success("AIInterface::HandleEvent","EVENT_ENTERCOMBAT"); break;
	 case EVENT_LEAVECOMBAT:    Log.Success("AIInterface::HandleEvent","EVENT_LEAVECOMBAT"); break;
	 case EVENT_DAMAGETAKEN:    Log.Success("AIInterface::HandleEvent","EVENT_DAMAGETAKEN"); break;
	 case EVENT_DAMAGEDEALT:    Log.Success("AIInterface::HandleEvent","EVENT_DAMAGEDEALT"); break;
	 case EVENT_FEAR:           Log.Success("AIInterface::HandleEvent","EVENT_FEAR"); break;
	 case EVENT_UNFEAR:         Log.Success("AIInterface::HandleEvent","EVENT_UNFEAR"); break;
	 case EVENT_FOLLOWOWNER:    Log.Success("AIInterface::HandleEvent","EVENT_FOLLOWOWNER"); break;
	 case EVENT_WANDER:         Log.Success("AIInterface::HandleEvent","EVENT_WANDER"); break;
	 case EVENT_UNWANDER:       Log.Success("AIInterface::HandleEvent","EVENT_UNWANDER"); break;
	 case EVENT_UNITDIED:       Log.Success("AIInterface::HandleEvent","EVENT_UNITDIED"); break;
	 case EVENT_POSSESS:        Log.Success("AIInterface::HandleEvent","EVENT_POSSESS"); break;
	 case EVENT_UNPOSSESS:      Log.Success("AIInterface::HandleEvent","EVENT_UNPOSSESS"); break;
	 case EVENT_HASPOSSESSED:   Log.Success("AIInterface::HandleEvent","EVENT_HASPOSSESSED"); break; 
	 case EVENT_HASUNPOSSESSED: Log.Success("AIInterface::HandleEvent","EVENT_HASUNPOSSESSED"); break;
	 default : Log.Error("AIInterface::HandleEvent","EVENT INCONNU!"); break;
	}
	*/

	if(m_AIState != STATE_EVADE)
	{
		switch(event)
		{
		case EVENT_ENTERCOMBAT:
			{
				if( pUnit == NULL ) return;

				/* send the message */
				if( m_Unit->GetTypeId() == TYPEID_UNIT )
				{
					if( static_cast< Creature* >( m_Unit )->has_combat_text )
						objmgr.HandleMonsterSayEvent( static_cast< Creature* >( m_Unit ), MONSTER_SAY_EVENT_ENTER_COMBAT );

					CALL_SCRIPT_EVENT(m_Unit, OnCombatStart)(pUnit);

					if( static_cast< Creature* >( m_Unit )->m_spawn && ( static_cast< Creature* >( m_Unit )->m_spawn->channel_target_go || static_cast< Creature* >( m_Unit )->m_spawn->channel_target_creature))
					{
						m_Unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
						m_Unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					}
				}
				
				// Stop the emote
				m_Unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
				m_returnX = m_Unit->GetPositionX();
				m_returnY = m_Unit->GetPositionY();
				m_returnZ = m_Unit->GetPositionZ();

				m_moveRun = true; //run to the target

				// dismount if mounted
				m_Unit->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 0);

				if(m_AIState != STATE_ATTACKING)
					StopMovement(0);

				m_AIState = STATE_ATTACKING;
				firstLeaveCombat = true;
				if(pUnit && pUnit->GetInstanceID() == m_Unit->GetInstanceID())
				{
					m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, pUnit->GetGUID());
				}
				if(m_Unit->GetMapMgr()->GetMapInfo() && m_Unit->GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID)
				{
					if(m_Unit->GetTypeId() == TYPEID_UNIT)
					{
						if(static_cast<Creature*>(m_Unit)->GetCreatureName() && static_cast<Creature*>(m_Unit)->GetCreatureName()->Rank == 3)
						{
							 m_Unit->GetMapMgr()->AddCombatInProgress(m_Unit->GetGUID());
						}
					}
				}
				
				HandleChainAggro(pUnit);

				//give 1 threat to this unit if were not on the threat list
				if (m_aiTargets.find(pUnit) == m_aiTargets.end())
				{
					m_aiTargets.insert(TargetMap::value_type(pUnit, 1));
				}

				for (std::set<uint64>::iterator itr=MindControlChilds.begin(); itr!=MindControlChilds.end(); ++itr)
				{
					if (m_Unit->GetMapMgr()->GetUnit(*itr) == NULL)
						continue;

					m_Unit->GetMapMgr()->GetUnit(*itr)->GetAIInterface()->AttackReaction(pUnit, 1, 0);
				}

				if (!m_Unit->HasFlag(UNIT_FIELD_BYTES_2, 1))
				{
					//set backup to remove on leaving evade
					m_backupbytes2 |= 1;
					m_Unit->SetFlag(UNIT_FIELD_BYTES_2, 1);
				}				
			}break;
		case EVENT_DAMAGEDEALT:
			{
				HandleChainAggro(pUnit);
				if (m_aiTargets.find(pUnit)==m_aiTargets.end())
				{
					m_aiTargets.insert(TargetMap::value_type(pUnit, 1));
				}
			}break;			
		case EVENT_LEAVECOMBAT:
			{
				if( pUnit == NULL ) return;
				
				if( pUnit->IsCreature() )
					pUnit->RemoveNegativeAuras();
				//cancel spells that we are casting. Should remove bug where creatures cast a spell after they died
//				CancelSpellCast();
				// restart emote
				if(m_Unit->GetTypeId() == TYPEID_UNIT)
				{
					if( static_cast< Creature* >( m_Unit )->has_combat_text )
					objmgr.HandleMonsterSayEvent( static_cast< Creature* >( m_Unit ), MONSTER_SAY_EVENT_ON_COMBAT_STOP );

					if(static_cast<Creature*>(m_Unit)->original_emotestate)
						m_Unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, static_cast< Creature* >( m_Unit )->original_emotestate);
					
					if(static_cast<Creature*>(m_Unit)->m_spawn && (static_cast< Creature* >( m_Unit )->m_spawn->channel_target_go || static_cast< Creature* >( m_Unit )->m_spawn->channel_target_creature ) )
					{
						if(static_cast<Creature*>(m_Unit)->m_spawn->channel_target_go)
							sEventMgr.AddEvent( static_cast< Creature* >( m_Unit ), &Creature::ChannelLinkUpGO, static_cast< Creature* >( m_Unit )->m_spawn->channel_target_go, EVENT_CREATURE_CHANNEL_LINKUP, 1000, 5, 0 );

						if(static_cast<Creature*>(m_Unit)->m_spawn->channel_target_creature)
							sEventMgr.AddEvent( static_cast< Creature* >( m_Unit ), &Creature::ChannelLinkUpCreature, static_cast< Creature* >( m_Unit )->m_spawn->channel_target_creature, EVENT_CREATURE_CHANNEL_LINKUP, 1000, 5, 0 );
					}
				}

				//reset ProcCount
				//ResetProcCounts();
				m_moveRun = true;
				m_aiTargets.clear();			
				m_fleeTimer = 0;
				m_hasFleed = false;
				m_hasCalledForHelp = false;
				m_nextSpell = NULL;
				SetNextTarget(NULL);
				m_Unit->CombatStatus.Vanished();

				//bring our mind controlled targets to us!
				for (std::set<uint64>::iterator itr=MindControlChilds.begin(); itr!=MindControlChilds.end(); ++itr)
				{
					if (m_Unit->GetMapMgr()->GetUnit(*itr) == NULL)
						continue;

					//m_Unit->GetMapMgr()->GetUnit(*itr)->GetAIInterface()->MoveTo(m_returnX, m_returnY, m_returnZ, m_Unit->GetSpawnO());

					m_Unit->GetMapMgr()->GetUnit(*itr)->GetAIInterface()->HandleEvent(EVENT_LEAVECOMBAT, m_Unit, 0);
				}

				if (MindControlOwner != NULL)
				{
					if (MindControlOwner->GetAIInterface()->GetAITargets()->size() == 0)
					{
						UnitToFollow=MindControlOwner;
						FollowDistance=3;
						m_AIState = STATE_FOLLOWING;
						return;
					}
				}
				
				if(m_AIType == AITYPE_PET)
				{
					m_AIState = STATE_FOLLOWING;
					UnitToFollow = m_PetOwner;
					FollowDistance = 3.0f;
					m_lastFollowX = m_lastFollowY = 0;
					if(m_Unit->IsPet())
					{
						((Pet*)m_Unit)->SetPetAction(PET_ACTION_FOLLOW);
						if( m_Unit->GetEntry() == 416 && m_Unit->isAlive() && m_Unit->IsInWorld() )
						{
							((Pet*)m_Unit)->HandleAutoCastEvent(AUTOCAST_EVENT_LEAVE_COMBAT);
						}
					}
					HandleEvent(EVENT_FOLLOWOWNER, 0, 0);
				}
				else if (MindControlOwner == NULL)
				{
					m_AIState = STATE_EVADE;

					Unit* SavedFollow = UnitToFollow;
					UnitToFollow = NULL;
					FollowDistance = 0.0f;
					m_lastFollowX = m_lastFollowY = 0;

					if(m_Unit->isAlive() && m_Unit->IsCreature())
					{
						if(m_returnX != 0.0f && m_returnY != 0.0f && m_returnZ != 0.0f && hasWaypoints())
						{
							MoveTo(m_returnX, m_returnY, m_returnZ, 0);
					}
						else
						{
							MoveTo(m_Unit->GetSpawnX(), m_Unit->GetSpawnY(), m_Unit->GetSpawnZ(), m_Unit->GetSpawnO());
						}

						Creature *aiowner = static_cast<Creature*>(m_Unit);
						//clear tagger.
						aiowner->Tagged = false;
						aiowner->TaggerGuid = 0;
						aiowner->SetUInt32Value(UNIT_DYNAMIC_FLAGS,aiowner->GetUInt32Value(UNIT_DYNAMIC_FLAGS) & ~(U_DYN_FLAG_TAGGED_BY_OTHER |U_DYN_FLAG_LOOTABLE));
						aiowner->m_lootMethod = -1;
					}
					CALL_SCRIPT_EVENT(m_Unit, OnCombatStop)(SavedFollow);
				}

				if(m_Unit->IsInWorld() && m_Unit->GetMapMgr()->GetMapInfo() && m_Unit->GetMapMgr()->GetMapInfo()->type == INSTANCE_RAID)
				{
					if(m_Unit->GetTypeId() == TYPEID_UNIT)
					{
						if(static_cast<Creature*>(m_Unit)->GetCreatureName() && static_cast<Creature*>(m_Unit)->GetCreatureName()->Rank == 3)
						{
							  m_Unit->GetMapMgr()->RemoveCombatInProgress(m_Unit->GetGUID());
						}
					}
				}

				// Remount if mounted
				if(m_Unit->GetTypeId() == TYPEID_UNIT)
				{
					if( static_cast< Creature* >( m_Unit )->proto )
						m_Unit->SetUInt32Value( UNIT_FIELD_MOUNTDISPLAYID, static_cast< Creature* >( m_Unit )->proto->MountedDisplayID );
				}
				
			}break;
		case EVENT_DAMAGETAKEN: // m_Unit = Caster, pUnit = Cible
			{
				if( pUnit == NULL ) return;

				//Log.Warning("HandleEvent","EVENT_DAMAGETAKEN,\nm_Unit %s (%u), pUnit %s (%u), Misc: %u",
				//	                      (m_Unit->GetObjectName())?(m_Unit->GetObjectName()):"Inconnu",m_Unit->GetEntry(),
				//		                  (pUnit->GetObjectName())?(pUnit->GetObjectName()):"Inconnu",m_Unit->GetEntry(), misc1); 

				//if(m_Unit->IsPet()) Log.Warning("EVENT_DAMAGETAKEN","Attaque de familier");
				//else                Log.Warning("EVENT_DAMAGETAKEN","Attaque d'une creature");      

				HandleChainAggro(pUnit);

				if( static_cast< Creature* >( m_Unit )->has_combat_text )
				     objmgr.HandleMonsterSayEvent( static_cast< Creature* >( m_Unit ), MONSTER_SAY_EVENT_ON_DAMAGE_TAKEN );

				CALL_SCRIPT_EVENT(m_Unit, OnDamageTaken)(pUnit, float(misc1));
				if(!modThreatByPtr(pUnit, misc1)) // Set de la cible si la cible etait NULL ?
				{
					//Log.Warning("HandleEvent","modThreatByPtr,pUnit %s, Misc: %u",
						                  //(pUnit->GetObjectName())?(pUnit->GetObjectName()):"Inconnu", misc1); 
					m_aiTargets.insert(TargetMap::value_type(pUnit, misc1));
				}
				m_Unit->CombatStatus.OnDamageDealt(pUnit);
			}break;
		case EVENT_FOLLOWOWNER:
			{
				m_AIState = STATE_FOLLOWING;
				if(m_Unit->IsPet())
					((Pet*)m_Unit)->SetPetAction(PET_ACTION_FOLLOW);
				UnitToFollow = m_PetOwner;
				m_lastFollowX = m_lastFollowY = 0;
				FollowDistance = 4.0f;

				m_aiTargets.clear();
				m_fleeTimer = 0;
				m_hasFleed = false;
				m_hasCalledForHelp = false;
				m_nextSpell = NULL;
				SetNextTarget(NULL);
				m_moveRun = true;
			}break;

		case EVENT_FEAR:
			{
				if( pUnit == NULL ) return;

				m_FearTimer = 0;
				SetUnitToFear(pUnit);

				CALL_SCRIPT_EVENT(m_Unit, OnFear)(pUnit, 0);
				m_AIState = STATE_FEAR;
				StopMovement(1);

				UnitToFollow_backup = UnitToFollow;
				UnitToFollow = NULL;
				m_lastFollowX = m_lastFollowY = 0;
				FollowDistance_backup = FollowDistance;
				FollowDistance = 0.0f;

				m_aiTargets.clear(); // we'll get a new target after we are unfeared
				m_fleeTimer = 0;
				m_hasFleed = false;
				m_hasCalledForHelp = false;

				// update speed
				m_moveRun = true;
				getMoveFlags();

				SetNextSpell(NULL);
				SetNextTarget(NULL);
			}break;

		case EVENT_UNFEAR:
			{
				UnitToFollow = UnitToFollow_backup;
				FollowDistance = FollowDistance_backup;
				m_AIState = STATE_IDLE; // we need this to prevent permanent fear, wander, and other problems

				SetUnitToFear(NULL);
				StopMovement(1);
			}break;

		case EVENT_WANDER:
			{
				if( pUnit == NULL ) return;

				m_WanderTimer = 0;

				//CALL_SCRIPT_EVENT(m_Unit, OnWander)(pUnit, 0); FIXME
				m_AIState = STATE_WANDER;
				StopMovement(1);

				UnitToFollow_backup = UnitToFollow;
				UnitToFollow = NULL;
				m_lastFollowX = m_lastFollowY = 0;
				FollowDistance_backup = FollowDistance;
				FollowDistance = 0.0f;

				m_aiTargets.clear(); // we'll get a new target after we are unwandered
				m_fleeTimer = 0;
				m_hasFleed = false;
				m_hasCalledForHelp = false;

				// update speed
				m_moveRun = true;
				getMoveFlags();

				SetNextSpell(NULL);
				SetNextTarget(NULL);
			}break;

		case EVENT_UNWANDER:
			{
				UnitToFollow = UnitToFollow_backup;
				FollowDistance = FollowDistance_backup;
				m_AIState = STATE_IDLE; // we need this to prevent permanent fear, wander, and other problems

				StopMovement(1);
			}break;
 
		case EVENT_POSSESS:
			{
				if( pUnit == NULL ) return;

				StopMovement(0);

				//l
				m_waypoints=NULL;
				m_canMove = true;
				m_destinationX = m_destinationY = m_destinationZ = 0;
				m_nextPosX = m_nextPosY = m_nextPosZ = 0;
				UnitToFollow = NULL;
				FollowDistance = 0.0f;
				m_fallowAngle = float(M_PI/2);
				m_timeToMove = 0;
				m_timeMoved = 0;
				m_moveTimer = 0;
				m_WayPointsShowing = false;
				m_WayPointsShowBackwards = false;
				m_currentWaypoint = 0;
				m_moveBackward = false;
				m_moveType = 0;
				m_moveRun = false;
				m_moveSprint = false;
				m_moveFly = false;
				m_creatureState = STOPPED;
				m_canCallForHelp = false;
				m_hasCalledForHelp = false;
				m_fleeTimer = 0;
				m_FleeDuration = 0;
				m_canFlee = false;
				m_hasFleed = false;
				m_canRangedAttack = false;
				m_FleeHealth = m_CallForHelpHealth = 0.0f;
				m_AIState = STATE_IDLE;

				m_updateAssist = false;
				m_updateTargets = false;
				m_updateAssistTimer = 1;
				m_updateTargetsTimer = TARGET_UPDATE_INTERVAL;

				m_nextSpell = NULL;
				m_nextTarget = NULL;
				totemspell = NULL;
				//m_Unit = NULL;
				m_PetOwner = NULL;
				m_aiCurrentAgent = AGENT_NULL;
				m_runSpeed = 0.0f;
				m_flySpeed = 0.0f;
				UnitToFear = NULL;
				firstLeaveCombat = true;
				m_outOfCombatRange = 2500000;

				tauntedBy = NULL;
				isTaunted = false;
				soullinkedWith = NULL;
				isSoulLinked = false;
				m_AllowedToEnterCombat = true;
				m_totalMoveTime = 0;
				m_lastFollowX = m_lastFollowY = 0;
				m_FearTimer = 0;
				m_WanderTimer = 0;
				m_totemspelltime = 0;
				m_totemspelltimer = 0;
				m_formationFollowAngle = 0.0f;
				m_formationFollowDistance = 0.0f;
				m_formationLinkTarget = 0;
				m_formationLinkSqlId = 0;
				m_currentHighestThreat = 0;
				//el

				m_AIType = AITYPE_PET;

				m_aiTargets.clear();

				// update speed
				m_moveRun = true;
				getMoveFlags();

				SetNextSpell(NULL);
				SetNextTarget(NULL);

				MindControlOwner = pUnit;
				UnitToFollow = pUnit;

				//lets get a random unit to attack from our owner, because we <3 him
				if (MindControlOwner->GetAIInterface()->GetAITargets()->size() > 0)
				{   // cast uint32 (Branruz)
					uint32 index=RandomUInt((uint32)(MindControlOwner->GetAIInterface()->GetAITargets()->size()-1));
					uint32 i=0;
					for (TargetMap::iterator itr=MindControlOwner->GetAIInterface()->GetAITargets()->begin(); itr!=MindControlOwner->GetAIInterface()->GetAITargets()->end(); ++itr, ++i)
					{
						if (i==index)
							AttackReaction(itr->first, 1, 0);
					}
				}
				else
					HandleEvent(EVENT_LEAVECOMBAT, m_Unit, 0); //follow

			}break;

		case EVENT_UNPOSSESS:
			{
				MindControlOwner = NULL;
				UnitToFollow = UnitToFollow_backup;
				FollowDistance = FollowDistance_backup;
				m_AIState = STATE_IDLE; // we need this to prevent permanent fear, wander, and other problems
			}break;

		case EVENT_HASPOSSESSED:
			{
				MindControlChilds.insert(pUnit->GetGUID());
			}break;

		case EVENT_HASUNPOSSESSED:
			{
				MindControlChilds.erase(pUnit->GetGUID());
			}break;
		
		default:
			{
			}break;
		}
	} // End if(m_AIState != STATE_EVADE)

	//Should be able to do this stuff even when evading
	switch(event)
	{
		case EVENT_UNITDIED:
		{
			if( pUnit == NULL || !m_Unit->IsUnit() ) return;
 
			m_Unit->RemoveAllAuras();
            
			if( pUnit->m_CurrentVehicle) pUnit->m_CurrentVehicle->RemovePassenger(pUnit);

            if( pUnit->m_CurrentVehicle )
				pUnit->m_CurrentVehicle->RemovePassenger(pUnit);

			if( static_cast< Creature* >( m_Unit )->has_combat_text )
			objmgr.HandleMonsterSayEvent( static_cast< Creature* >( m_Unit ), MONSTER_SAY_EVENT_ON_DIED );

			CALL_SCRIPT_EVENT(m_Unit, OnDied)(pUnit);
			m_AIState = STATE_IDLE;

			StopMovement(0);
			m_aiTargets.clear();
			UnitToFollow = NULL;
			m_lastFollowX = m_lastFollowY = 0;
			UnitToFear = NULL;
			FollowDistance = 0.0f;
			m_fleeTimer = 0;
			m_hasFleed = false;
			m_hasCalledForHelp = false;
			m_nextSpell = NULL;
			MindControlOwner = NULL;
			
			SetNextTarget(NULL);
			//reset ProcCount
			//ResetProcCounts();
		
			//reset waypoint to 0
			m_currentWaypoint = 0;
			
			// There isn't any need to do any attacker checks here, as
			// they should all be taken care of in DealDamage

			//removed by Zack : why do we need to go to our master if we just died ? On next spawn we will be spawned near him after all
/*			if(m_AIType == AITYPE_PET)
			{
				SetUnitToFollow(m_PetOwner);
				SetFollowDistance(3.0f);
				HandleEvent(EVENT_FOLLOWOWNER, m_Unit, 0);
			}*/

			if(m_Unit->GetMapMgr())
			{
				if(m_Unit->GetTypeId() == TYPEID_UNIT && !m_Unit->IsPet())
				{
					if(m_Unit->GetMapMgr()->pInstance && m_Unit->GetMapMgr()->GetMapInfo()->type != INSTANCE_NONRAID)
					{
						m_Unit->GetMapMgr()->pInstance->m_killedNpcs.insert( static_cast< Creature* >( m_Unit )->GetSQL_id() );
						m_Unit->GetMapMgr()->pInstance->SaveToDB();
					}
				}
			}
		}break;
	}
}

void AIInterface::Update(uint32 p_time)
{
	float tdist;
	Object *RandTarget=0; 

	if(m_AIType == AITYPE_TOTEM)
	{
		assert(totemspell != 0);
		if(p_time >= m_totemspelltimer)
		{
			Spell *pSpell = new Spell(m_Unit, totemspell, true, 0);
            
			SpellCastTargets targets(RandTarget); 
			if(!m_nextTarget ||
				(m_nextTarget && 
					(!m_Unit->GetMapMgr()->GetUnit(m_nextTarget->GetGUID()) || 
					!m_nextTarget->isAlive() ||
					(m_nextTarget->GetTypeId() == TYPEID_UNIT && static_cast<Creature *>(m_nextTarget)->IsTotem()) ||
					!IsInrange(m_Unit,m_nextTarget,pSpell->m_spellInfo->base_range_or_radius_sqr) ||
					!isAttackable(m_Unit, m_nextTarget,!(pSpell->m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED))
					)
				)
				)
			{
				//we set no target and see if we managed to fid a new one
				m_nextTarget=NULL;
				//something happend to our target, pick another one
				pSpell->GenerateTargets(&targets);
				if(targets.m_targetMask & TARGET_FLAG_UNIT)
					m_nextTarget = static_cast<Unit*>(targets.m_target);
			}
			if(m_nextTarget)
			{
				SpellCastTargets targets(m_nextTarget);
				pSpell->prepare(&targets);
				// need proper cooldown time!
				m_totemspelltimer = m_totemspelltime;
			}
			else delete pSpell;

			// these will *almost always* be AoE, so no need to find a target here.
//			SpellCastTargets targets(m_Unit->GetGUID());
//			Spell * pSpell = new Spell(m_Unit, totemspell, true, 0);
//			pSpell->prepare(&targets);
			// need proper cooldown time!
//			m_totemspelltimer = m_totemspelltime;
		}
		else
		{
			m_totemspelltimer -= p_time;
		}
		return;
	} // End if(m_AIType == AITYPE_TOTEM)

	_UpdateTimer(p_time);
	_UpdateTargets();
	if(m_Unit->isAlive() && m_AIState != STATE_IDLE 
		&& m_AIState != STATE_FOLLOWING && m_AIState != STATE_FEAR 
		&& m_AIState != STATE_WANDER && m_AIState != STATE_SCRIPTMOVE)
	{
		if(m_AIType == AITYPE_PET )
		{
			if(!m_Unit->bInvincible && m_Unit->IsPet()) 
			{
				Pet * pPet = static_cast<Pet*>(m_Unit);
	
				if(pPet->GetPetAction() == PET_ACTION_ATTACK || pPet->GetPetState() != PET_STATE_PASSIVE)
				{
					_UpdateCombat(p_time);
				}
			}
			//we just use any creature as a pet guardian
			else if(!m_Unit->IsPet())
			{
				_UpdateCombat(p_time);
			}
		}
		else
		{
			_UpdateCombat(p_time);
		}
	}

	_UpdateMovement(p_time);
	if(m_AIState==STATE_EVADE)
	{
		tdist = m_Unit->GetDistanceSq(m_returnX,m_returnY,m_returnZ);
		if(tdist <= 4.0f/*2.0*/)
		{
			m_AIState = STATE_IDLE;
			m_returnX = m_returnY = m_returnZ = 0.0f;
			m_moveRun = false;
			//remowed by zack : in scripted events if we keep reducing this it will bug the world out !
			//On Blizz it will return to previous wp but we can accept the fact that it will move on to next one
			/*
			if(hasWaypoints())
			{
				if(m_moveBackward)
				{
					if(m_currentWaypoint != GetWayPointsCount()-1)
						m_currentWaypoint++;
				}
				else
				{
					if(m_currentWaypoint != 0)
						m_currentWaypoint--;
				}
			}
			*/
			// Set health to full if they at there last location before attacking
			if(m_AIType != AITYPE_PET&& !skip_reset_hp)
				m_Unit->SetUInt32Value(UNIT_FIELD_HEALTH,m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));

			//unwield weapon if we had to wield it
			if (m_backupbytes2 & 1)
			{
				m_backupbytes2 &= ~1;
				m_Unit->RemoveFlag(UNIT_FIELD_BYTES_2, 1);
			}		
		}
		else
		{
			if( m_creatureState == STOPPED )
			{
				// return to the home
				if( m_returnX == 0.0f && m_returnY == 0.0f )
				{
					m_returnX = m_Unit->GetSpawnX();
					m_returnY = m_Unit->GetSpawnY();
					m_returnZ = m_Unit->GetSpawnZ();
				}

				MoveTo(m_returnX, m_returnY, m_returnZ, m_Unit->GetSpawnO());
			}
		}
	}

	if(m_fleeTimer)
	{
		if(m_fleeTimer > p_time)
		{
			m_fleeTimer -= p_time;
			if(!m_nextTarget) //something happened to our target, lets find another one
				SetNextTarget(FindTargetForSpell(m_nextSpell));
			if(m_nextTarget)
				_CalcDestinationAndMove(m_nextTarget, 5.0f);
		}
		else
		{
			m_fleeTimer = 0;
			SetNextTarget(FindTargetForSpell(m_nextSpell));
		}
	}


	//Pet Dismiss after a certian ditance away
	if(m_AIType == AITYPE_PET && m_PetOwner != NULL)
	{
		float dist = (m_Unit->GetInstanceID() == m_PetOwner->GetInstanceID()) ? 
			m_Unit->GetDistanceSq(m_PetOwner) : 99999.0f;

		if(dist > 8100.0f) //90 yard away we Dismissed
		{
			Log.Warning("AIInterface::Update","Player trop loin de son familier, Dismiss");
			DismissPet();
			return;
		}
	}
}

void AIInterface::_UpdateTimer(uint32 p_time)
{
	if(m_updateAssistTimer > p_time)
	{
		m_updateAssistTimer -= p_time;
	}else
	{
		m_updateAssist = true;
		m_updateAssistTimer = TARGET_UPDATE_INTERVAL * 2 - m_updateAssistTimer - p_time;
	}

	if(m_updateTargetsTimer > p_time)
	{
		m_updateTargetsTimer -= p_time;
	}else
	{
		m_updateTargets = true;
		m_updateTargetsTimer = TARGET_UPDATE_INTERVAL * 2 - m_updateTargetsTimer - p_time;
	}
}

void AIInterface::_UpdateTargets()
{
	if( m_Unit->IsPlayer() || (m_AIType != AITYPE_PET && disable_targeting )) return;

	if( ( ( Creature* )m_Unit )->GetCreatureName() && ( ( Creature* )m_Unit )->GetCreatureName()->Type == CRITTER )
		return;

	AssistTargetSet::iterator i, i2;
	TargetMap::iterator itr, it2;

	// Find new Assist Targets and remove old ones
	if(m_AIState == STATE_FLEEING)
	{
		FindFriends(100.0f/*10.0*/);
	}
	else if(m_AIState != STATE_IDLE && m_AIState != STATE_SCRIPTIDLE)
	{
		FindFriends(16.0f/*4.0f*/);
	}

	if( m_updateAssist )
	{
		m_updateAssist = false;
	/*	deque<Unit*> tokill;

		//modified for vs2005 compatibility
		for(i = m_assistTargets.begin(); i != m_assistTargets.end(); ++i)
		{
			if(m_Unit->GetDistanceSq((*i)) > 2500.0f|| !(*i)->isAlive() || !(*i)->CombatStatus.IsInCombat())
			{
				tokill.push_back(*i);
			}
		}

		for(deque<Unit*>::iterator i2 = tokill.begin(); i2 != tokill.end(); ++i2)
			m_assistTargets.erase(*i2);*/

		for(i = m_assistTargets.begin(); i != m_assistTargets.end();)
		{
			i2 = i++;
			if((*i2) == NULL || (*i2)->event_GetCurrentInstanceId() != m_Unit->event_GetCurrentInstanceId() ||
				!(*i2)->isAlive() || m_Unit->GetDistanceSq((*i2)) >= 2500.0f || !(*i2)->CombatStatus.IsInCombat() || !((*i2)->m_Phase & m_Unit->m_Phase))
			{
				m_assistTargets.erase( i2 );
			}
		}
	}

	if( m_updateTargets )
	{
		m_updateTargets = false;
		/*deque<Unit*> tokill;

		//modified for vs2005 compatibility
		for(itr = m_aiTargets.begin(); itr != m_aiTargets.end();++itr)
		{
			if(!itr->first->isAlive() || m_Unit->GetDistanceSq(itr->first) >= 6400.0f)
			{
				tokill.push_back(itr->first);
			}
		}
		for(deque<Unit*>::iterator itr = tokill.begin(); itr != tokill.end(); ++itr)
			m_aiTargets.erase((*itr));
		tokill.clear();*/
		bool new_target = false;

		for(itr = m_aiTargets.begin(); itr != m_aiTargets.end();)
		{
			it2 = itr++;
		
 			if( !TargetUpdateCheck( it2->first) )
			{
 				m_aiTargets.erase( it2 );
				new_target = true;
			}

			//if new_target is true the iterator is invalid
			if (!new_target && !isAttackable(m_Unit, it2->first))
			{
				m_aiTargets.erase(it2);
				new_target = true;
			}
		}
		
		if( (m_aiTargets.size() == 0) 
			&& (m_AIState != STATE_IDLE)   && (m_AIState != STATE_FOLLOWING) 
			&& (m_AIState != STATE_EVADE)  && (m_AIState != STATE_FEAR) 
			&& (m_AIState != STATE_WANDER) && (m_AIState != STATE_SCRIPTIDLE))
		{
			if(firstLeaveCombat)
			{
				Unit* target = FindTarget();
				if(target)
				{
					AttackReaction(target, 1, 0);
				}else
				{
					firstLeaveCombat = false;
				}
			}

		}
		else if( (m_aiTargets.size() == 0) 
			&& ( (m_AIType == AITYPE_PET)                // Le familier attaque si ennemi dans son rayon d'action
			&& ( m_Unit->IsPet() && static_cast<Pet *>(m_Unit)->GetPetState() == PET_STATE_AGGRESSIVE) 
			|| (!m_Unit->IsPet() && disable_melee == false ) ) )
		{
			 Unit* target = FindTarget();
			 if( target )
			 {
				 AttackReaction(target, 1, 0);
			 }

		}
		else if( (m_aiTargets.size() == 0) 
			&& ( (m_AIType == AITYPE_PET)              // Le familier attaque seulement si le player est attaqué
			&& ( m_Unit->IsPet() && static_cast<Pet *>(m_Unit)->GetPetState() == PET_STATE_DEFENSIVE) 
			|| (!m_Unit->IsPet() && disable_melee == false ) ) )
		{
			Unit *plrUnit = (static_cast<Pet *>(m_Unit))->GetOwner();
			if(plrUnit)
			{
			 if(plrUnit->IsPlayer())
			 {
			  //if( ((Player *)plrUnit)->IsAttacking())
			  if( plrUnit->CombatStatus.IsInCombat() )
			  {
			   //Log.Warning("Familier","%s se fait attaquer",((Player *)plrUnit)->GetName());
			   Unit *target = FindTarget();
			   if( target ) AttackReaction(target, 1, 0);
			  }
			 }
			}
		}
	}
	// Find new Targets when we are ooc
	if((m_AIState == STATE_IDLE || m_AIState == STATE_SCRIPTIDLE) && m_assistTargets.size() == 0)
	{
		Unit* target = FindTarget();
		if(target)
		{
			AttackReaction(target, 1, 0);
		}
	}
}

///====================================================================
///  Desc: Updates Combat Status of m_Unit
///====================================================================
void AIInterface::_UpdateCombat(uint32 p_time)
{
	float SourceTaille = 0;
	float TargetTaille = 0;

	//Log.Success("AIInterface","_UpdateCombat : %s",ConvertUnixTimeToString(p_time).c_str()); // Attention au flood, debug devs....

    if( m_AIType != AITYPE_PET && disable_combat )	
	{
        //Log.Warning("AIInterface","Le familier ne peut pas combattre");
		return;
	}

	//just make sure we are not hitting self. This was reported as an exploit.Should never ocure anyway
	if( m_nextTarget == m_Unit ) m_nextTarget = GetMostHated();

	uint16 agent = m_aiCurrentAgent;

	if(	m_AIType != AITYPE_PET 
			&& (m_outOfCombatRange && m_Unit->GetDistanceSq(m_returnX,m_returnY,m_returnZ) > m_outOfCombatRange) 
			&& m_AIState != STATE_EVADE
			&& m_AIState != STATE_SCRIPTMOVE
			&& !m_is_in_instance)
	{
		HandleEvent( EVENT_LEAVECOMBAT, m_Unit, 0 );
	}                                                                                                // Le combat doit s'arreter quelque soit le phasing
	else if( m_nextTarget == NULL && m_AIState != STATE_FOLLOWING && m_AIState != STATE_SCRIPTMOVE ) // && !(/*m_nextTarget->m_Phase & */ m_Unit->m_Phase))
	{
		m_nextTarget = GetMostHated();
		if( m_nextTarget == NULL )
			HandleEvent( EVENT_LEAVECOMBAT, m_Unit, 0 );
	}
     
	
	if( m_nextTarget != NULL && m_nextTarget->isAlive() && m_AIState != STATE_EVADE && !m_Unit->isCasting() )
	{
		//Log.Success("AIInterface","m_nextTarget (Agent: %u)",agent);

		if( agent == AGENT_NULL || ( (m_AIType == AITYPE_PET) && !m_nextSpell ) ) // allow pets autocast
		{
			//Log.Success("AIInterface","Next Spell at : %s",ConvertUnixTimeToString(next_spell_time).c_str()); 
			//Log.Success("AIInterface","Actuel        : %s",ConvertUnixTimeToString((uint32)UNIXTIME).c_str());

			if(m_canFlee && !m_hasFleed && ( m_FleeHealth ? float(m_Unit->GetUInt32Value(UNIT_FIELD_HEALTH) / m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH)) < m_FleeHealth : 1))
				agent = AGENT_FLEE;
			else if(m_canCallForHelp && !m_hasCalledForHelp )
				agent = AGENT_CALLFORHELP;
			else //default to melee if no spells found
			{
				m_nextSpell = getSpell();
				if((m_nextSpell != NULL) && (m_nextSpell->agent != AGENT_NULL) )
					agent = m_nextSpell->agent;
				else
					agent = AGENT_MELEE;
			}
		}
		//check if we can do range attacks
		if(agent == AGENT_RANGED || agent == AGENT_MELEE)
		{
			if(m_canRangedAttack)
			{
				float dist = m_Unit->GetDistanceSq(m_nextTarget);
				if(m_nextTarget->GetTypeId() == TYPEID_PLAYER)
				{
					if( static_cast<Player *>( m_nextTarget )->m_currentMovement == MOVE_ROOT || dist >= 32.0f )
						agent = AGENT_RANGED;
				}
				else if( m_nextTarget->m_canMove == false || dist >= 32.0f )
				   agent = AGENT_RANGED;
			}
			else
			{
				agent = AGENT_MELEE;
			}
		}
        //Log.Warning("AIInterface","Agent: %u",agent);
		//Log.Warning("AIInterface","disable_melee: %s disable_ranged: %s disable_spell: %s",
		//	(disable_melee)?"Oui":"Non",(disable_ranged)?"Oui":"Non",(disable_spell)?"Oui":"Non");
        
		if(this->disable_melee  && agent == AGENT_MELEE)  agent = AGENT_NULL;
		if(this->disable_ranged && agent == AGENT_RANGED) agent = AGENT_NULL;
		if(this->disable_spell  && agent == AGENT_SPELL)  agent = AGENT_NULL;

		//Log.Warning("AIInterface","Agent: %u",agent);

		switch(agent)
		{
		case AGENT_MELEE:
			{
				if( m_Unit->GetTypeId() == TYPEID_UNIT )
					static_cast<Creature *>(m_Unit)->SetSheatheForAttackType( 1 );
					
				float combatReach[2]; // Calculate Combat Reach
				float distance = m_Unit->CalcDistance(m_nextTarget);

				combatReach[0] = PLAYER_SIZE;
				combatReach[1] = _CalcCombatRange(m_nextTarget, false);

				if(	
					distance >= combatReach[0] && 
					distance <= combatReach[1] + DISTANCE_TO_SMALL_TO_WALK) // Target is in Range -> Attack
				{
					if(UnitToFollow != NULL)
					{
						UnitToFollow = NULL; //we shouldn't be following any one
						m_lastFollowX = m_lastFollowY = 0;
						//m_Unit->setAttackTarget(NULL);  // remove ourselves from any target that might have been followed
					}
					
					FollowDistance = 0.0f;
//					m_moveRun = false;
					//FIXME: offhand shit
					if(m_Unit->isAttackReady(false) && !m_fleeTimer)
					{
						m_creatureState = ATTACKING;
						bool infront = m_Unit->isInFront(m_nextTarget);

						if(!infront) // set InFront
						{
							//prevent mob from rotating while stunned
							if(!m_Unit->IsStunned ())
							{
								setInFront(m_nextTarget);
								infront = true;
							}							
						}
						if(infront)
						{
							m_Unit->setAttackTimer(0, false);

#ifdef ENABLE_CREATURE_DAZE
							//we require to know if strike was succesfull. If there was no dmg then target cannot be dazed by it
							uint32 health_before_strike=m_nextTarget->GetUInt32Value(UNIT_FIELD_HEALTH);
#endif
							m_Unit->Strike( m_nextTarget, ( agent == AGENT_MELEE ? MELEE : RANGED ), NULL, 0, 0, 0, false, false );
#ifdef ENABLE_CREATURE_DAZE
							//now if the target is facing his back to us then we could just cast dazed on him :P
							//as far as i know dazed is casted by most of the creatures but feel free to remove this code if you think otherwise
							if(m_nextTarget &&
								!(m_Unit->m_factionDBC->RepListId == -1 && m_Unit->m_faction->FriendlyMask==0 && m_Unit->m_faction->HostileMask==0) /* neutral creature */
								&& m_nextTarget->IsPlayer() && !m_Unit->IsPet() && health_before_strike>m_nextTarget->GetUInt32Value(UNIT_FIELD_HEALTH)
								&& Rand(m_Unit->get_chance_to_daze(m_nextTarget)))
							{
								float our_facing=m_Unit->calcRadAngle(m_Unit->GetPositionX(),m_Unit->GetPositionY(),m_nextTarget->GetPositionX(),m_nextTarget->GetPositionY());
								float his_facing=m_nextTarget->GetOrientation();
								if(fabs(our_facing-his_facing)<CREATURE_DAZE_TRIGGER_ANGLE && !m_nextTarget->HasNegativeAura(CREATURE_SPELL_TO_DAZE))
								{
									SpellEntry *info = dbcSpell.LookupEntry(CREATURE_SPELL_TO_DAZE);
									Spell *sp = new Spell(m_Unit, info, false, NULL);
									SpellCastTargets targets;
									targets.m_target = m_nextTarget; // targets.m_unitTarget = m_nextTarget->GetGUID();
									sp->prepare(&targets);
								}
							}
#endif
							if (m_nextSpell)
							{                                                    // Secu: Le type Player peut etre Unit, Pas l'inverse
							 if( (m_Unit->GetTypeId() == TYPEID_UNIT) && (m_Unit->GetTypeId() != TYPEID_PLAYER) ) // ça devient un spell, on change de mode d'attaque
									static_cast<Creature *>(m_Unit)->SetSheatheForAttackType( 0 );

							 Player *ModePlayer;
							 SpellEntry *spellHeroicMeleeInfo;
							 SpellEntry *spellMeleeInfo;
							 // --- Gestion du mode Héroïque par Randdrick
							 // Certains sorts doivent se lancer qu'en mode Heroïque, d'autres qu'en mode non-héroïque
							 // et la plupart des sorts peuvent se lancer dans tout les modes.
							 // Valeur de spellheroïque :
							 // 0->tous les modes, 1->standard 10 joueurs, 2-> standard 25 joueurs 
							 // ( ou mode non-heroique uniquement), 3-> mode héroïque 10 joueurs, 4-> mode héroïque 25 joueurs

							 // Mode héroïque.
							 //On sélectionne le joueur le plus proche pour déterminer son mode de jeu.
							 ModePlayer = m_Unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ());

							 if (ModePlayer) 
							 {
								if (ModePlayer->iInstanceType >= MODE_HEROIC && m_nextSpell->spellheroicmode == 0 || m_nextSpell->spellheroicmode == 3 || m_nextSpell->spellheroicmode == 4)
								{
									spellHeroicMeleeInfo = m_nextSpell->spell;
									spellMeleeInfo = spellHeroicMeleeInfo;
									//Log.Notice("AIAgent", "Joueur detecte en mode heroique: la compétence utilisee est %u.", spellMeleeInfo->Id);
								}
									// Mode non-héroïque.
								else if ( m_nextSpell->spellheroicmode == 0 || m_nextSpell->spellheroicmode == 1 || m_nextSpell->spellheroicmode == 2)
								{
									spellHeroicMeleeInfo = NULL;	 
									spellMeleeInfo = m_nextSpell->spell;
									//Log.Notice("AIAgent", "Joueur detecte: la compétence utilisee est %u.", spellMeleeInfo->Id);
								}
								else // probleme ModeHero, on sort, a resoudre qd meme avec le spell standard (spellMeleeInfo = m_nextSpell->spell;)
								{
									Log.Error("AIAgent","Echec: Erreur d'init de l'AISpell %u. InstType:%u HeroMode:%u (0 à 3) <---- Check your database",
                                           m_nextSpell->spell->Id,ModePlayer->iInstanceType,m_nextSpell->spellheroicmode);
                    
									return; 
								}
							 }
							 else spellMeleeInfo = m_nextSpell->spell;

							 SpellCastTargets MeleeTargets = setSpellTargets(spellMeleeInfo, m_nextTarget);
							 uint32 meleeTargetType = m_nextSpell->spelltargetType;

                             //Log.Warning("AIInterface","meleeTargetType: %u",meleeTargetType);

							 // On va gérer certaines tajectoires, celles concernant le player ou l'unité ennemie uniquement
							 switch(meleeTargetType)
							 {
							  case TTYPE_HIMSELF: // TTYPE_CASTER:
							  case TTYPE_SINGLETARGET:
							  {
									CastSpell(m_Unit, spellMeleeInfo, MeleeTargets);
									break;
							  }
							  case TTYPE_LAST_HIT_FROM_TARGET:
							  {
									m_Unit->CastSpellAoF(MeleeTargets.m_srcX,MeleeTargets.m_srcY,MeleeTargets.m_srcZ, spellMeleeInfo, true);
									break;
							  }
							  case TTYPE_DESTINATION:
							  {
									m_Unit->CastSpellAoF(MeleeTargets.m_destX,MeleeTargets.m_destY,MeleeTargets.m_destZ, spellMeleeInfo, true);
									break;
							  }
							  case TTYPE_RANDOM_ENNEMY_UNIT:
							  {
									m_Unit->GetBestTargetUnit(); // Envoie de la trajectoire hostile à l'unité pour le spell
									CastSpell(m_Unit, spellMeleeInfo, MeleeTargets); // Appel du spell qui va être utilisé
									break;
							  }
							  case TTYPE_RANDOM_ENNEMY_PLAYER:
							  {
									m_Unit->GetBestTargetPlayer(); // Envoie de la trajectoire pour le spell
									CastSpell(m_Unit, spellMeleeInfo, MeleeTargets); // Appel du spell qui va être utilisé
							 		break;
							  }
							  case TTYPE_ENNEMY_PLAYER_IN_CLOSEST_RANGE:
							  {
									m_Unit->GetBestTargetPlayer(TARGET_FILTER_CLOSEST); // Envoie de la trajectoire la plus proche pour le spell
									CastSpell(m_Unit, spellMeleeInfo, MeleeTargets); // Appel du spell qui va être utilisé
									break;
							  }
							  case TTYPE_ENNEMY_UNIT_IN_CLOSEST_RANGE:
							  {
									m_Unit->GetBestTargetUnit(TARGET_FILTER_CLOSEST); // Envoie de la trajectoire la plus proche à l'unité, pour le spell
									CastSpell(m_Unit, spellMeleeInfo, MeleeTargets); // Appel du spell qui va être utilisé
									break;
							  }
							  case TTYPE_RANDOM_ENNEMY_UNIT_NOT_CURRENT:
							  {
									m_Unit->GetBestTargetUnit(TARGET_FILTER_NOT_CURRENT); // Envoie de la trajectoire la plus proche pour le spell, à l'unité qui ne génère pas le plus de menace.
									CastSpell(m_Unit, spellMeleeInfo, MeleeTargets); // Appel du spell qui va être utilisé
									break;
							  }							
							  case TTYPE_RANDOM_ENNEMY_PLAYER_NOT_CURRENT:
							  {
									m_Unit->GetBestTargetPlayer(TARGET_FILTER_NOT_CURRENT); // Envoie de la trajectoire la plus proche pour le spell, pour le player qui ne génère pas le plus de menace.
									CastSpell(m_Unit, spellMeleeInfo, MeleeTargets); // Appel du spell qui va être utilisé
									break;
							  }
							  case TTYPE_ENNEMY_IN_CLOSEST_RANGE_NOT_CURRENT:
							  {
									m_Unit->GetBestTargetUnit(TARGET_FILTER_CLOSEST); // Envoie de la trajectoire la plus proche pour le spell, pour l'unité qui ne génère pas le plus de menace.
									CastSpell(m_Unit, spellMeleeInfo, MeleeTargets); // Appel du spell qui va être utilisé
									break;
							  }
							  case TTYPE_RANDOM_ENNEMY_WOUNDED_UNIT:
							  {
									m_Unit->GetBestTargetUnit(TARGET_FILTER_WOUNDED); // Envoie de la trajectoire la plus proche pour le spell, pour l'unité qui est blessée.
									CastSpell(m_Unit, spellMeleeInfo, MeleeTargets); // Appel du spell qui va être utilisé
									break;
							  }
							  case TTYPE_RANDOM_ENNEMY_WOUNDED_PLAYER:
							  {
									m_Unit->GetBestTargetPlayer(TARGET_FILTER_WOUNDED); // Envoie de la trajectoire la plus proche pour le spell, pour le player qui est blessé.
									CastSpell(m_Unit, spellMeleeInfo, MeleeTargets); // Appel du spell qui va être utilisé
									break;
							  }
							  case TTYPE_SECOND_MOST_HASTED:
							  {
									m_Unit->GetBestTargetPlayer(TARGET_FILTER_SECOND_MOSTED_HASTED); // Envoie de la trajectoire la plus proche pour le spell, pour le player qui génère en second, le plus de menace.
									CastSpell(m_Unit, spellMeleeInfo, MeleeTargets); // Appel du spell qui va être utilisé
									break;
							  }
							  default:
							  {
							 		Log.Warning("AIAgent", "La trajectoire %u dans ce mode n'existe pas.", meleeTargetType);
									break;
							  }
							 } // End of switch(meleeTargetType)

							 //Did we give it a sound ID?	
							 if( m_nextSpell && m_nextSpell->Misc2 )
								m_Unit->PlaySoundToSet(m_nextSpell->Misc2);

							 // CastSpell(m_Unit, spellInfo, targets);
							 if(m_nextSpell&&m_nextSpell->cooldown)
								m_nextSpell->cooldowntime = getMSTime() + m_nextSpell->cooldown;

							 //add pet spell after use to pet owner with some chance
							 if(m_Unit->IsPet() && m_PetOwner->IsPlayer())
							 {	
								Pet * pPet = static_cast<Pet*>(m_Unit);
									if(pPet && Rand(10))
									pPet->AddPetSpellToOwner(spellMeleeInfo->Id);
							 }
							 //increase procCounter if we're counting
							 if(m_nextSpell && m_nextSpell->procCount)
							          m_nextSpell->procCounter++;

							 m_nextSpell = NULL;

						     // Null out the agent, so we force the lookup of the next spell.
						     // m_aiCurrentAgent = AGENT_NULL;
						   } // End of if (m_nextSpell)

						 // Randdrick: Celui-la il est de trop, Le premier: #1162 (Branruz)
						 //m_Unit->Strike( m_nextTarget, ( agent == AGENT_MELEE ? MELEE : RANGED ), NULL, 0, 0, 0, false, false );

						} // End of if(infront)
					}
				}
				else // Target out of Range -> Run to it
				{
					//calculate next move
					/*float dist = combatReach[1]-PLAYER_SIZE;

					if(dist < PLAYER_SIZE)
						dist = PLAYER_SIZE; //unbelievable how this could happen
					if (distance<combatReach[0])
						dist = -(distance+combatReach[0]*0.6666f);*/

					m_moveRun = true;
					_CalcDestinationAndMove(m_nextTarget, _CalcCombatRange(m_nextTarget, false));
				}
			}break;
		case AGENT_RANGED:
			{
				//if( m_Unit->GetTypeId() == TYPEID_UNIT )
					static_cast<Creature *>(m_Unit)->SetSheatheForAttackType( 3 );

				float combatReach[4]; // Used Shooting Ranges
				float distance = m_Unit->CalcDistance(m_nextTarget);

			    combatReach[0] = GetUnit()->GetFloatValue(UNIT_FIELD_COMBATREACH); //normal combat reach
				combatReach[1] = combatReach[0] + 8.0f; //  if distance <  combatReach[1], run towards target, and enter melee.
				combatReach[2] = combatReach[0] + 30.0f; // When shooting and distance > combatReach[2], 
				combatReach[3] = combatReach[0] + 26.0f; //        close in to combatReach[3] again, and continue shooting.

				if(distance >= combatReach[1] && distance <= combatReach[2]) // Target is in Range -> Shoot!!
				{
					if(UnitToFollow != NULL)
					{
						UnitToFollow = NULL; //we shouldn't be following any one
						m_lastFollowX = m_lastFollowY = 0;
						//m_Unit->setAttackTarget(NULL);  // remove ourselves from any target that might have been followed
					}
					
					FollowDistance = 0.0f;
//					m_moveRun = false;
					//FIXME: offhand shit
					if(m_Unit->isAttackReady(false) && !m_fleeTimer)
					{
						m_creatureState = ATTACKING;
						bool infront = m_Unit->isInFront(m_nextTarget);

						if(!infront) // set InFront
						{
							//prevent mob from rotating while stunned
							if(!m_Unit->IsStunned ())
							{
								setInFront(m_nextTarget);
								infront = true;
							}							
						}

						if(infront)
						{
							m_Unit->setAttackTimer(0, false);
							/* SpellEntry *info = dbcSpell.LookupEntry(SPELL_RANGED_GENERAL); //Note randdrick : Non satisfaisant car il existe plusieurs modes de tir
							if(info)
							{
								Spell *sp = new Spell(m_Unit, info, false, NULL);
								SpellCastTargets targets;
								targets.m_target = m_nextTarget;
								sp->prepare(&targets);
								//Lets make spell handle this
								//m_Unit->Strike( m_nextTarget, ( agent == AGENT_MELEE ? MELEE : RANGED ), NULL, 0, 0, 0 );
							}*/
							if (m_nextSpell)
							{
								Player *ModePlayer;
								SpellEntry *spellHeroicRangeInfo;
								SpellEntry *spellRangeInfo;
								// --- Gestion du mode Héroïque par Randdrick
								// Certains sorts doivent se lancer qu'en mode Heroïque, d'autres qu'en mode non-héroïque
								// et la plupart des sorts peuvent se lancer dans tout les modes.
								// Valeur de spellheroïque :
								// 0->tous les modes, 1->standard 10 joueurs, 2-> standard 25 joueurs 
								// ( ou mode non-heroique uniquement), 3-> mode héroïque 10 joueurs, 4-> mode héroïque 25 joueurs

								// Mode héroïque.
								//On sélectionne le joueur le plus proche pour déterminer son mode de jeu.
								ModePlayer = m_Unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ());

								if (ModePlayer) 
								{
									if (ModePlayer->iInstanceType >= MODE_HEROIC && (m_nextSpell->spellheroicmode == 0 || m_nextSpell->spellheroicmode == 3 || m_nextSpell->spellheroicmode == 4))
									{
										spellHeroicRangeInfo = m_nextSpell->spell;
										spellRangeInfo = spellHeroicRangeInfo;
										Log.Notice("AIAgent", "Joueur detecte en mode heroique: la compétence de tir utilisee est %u.", spellRangeInfo->Id);
									}
									// Mode non-héroïque.
									else if ( m_nextSpell->spellheroicmode == 0 || m_nextSpell->spellheroicmode == 1 || m_nextSpell->spellheroicmode == 2)
									{
										spellHeroicRangeInfo = NULL;	 
										spellRangeInfo = m_nextSpell->spell;
										Log.Notice("AIAgent", "Joueur detecte: la compétence de tir utilisee est %u.", spellRangeInfo->Id);
									}
									else // probleme ModeHero, on sort, a resoudre qd meme avec le spell standard (spellMeleeInfo = m_nextSpell->spell;)
								    {
									   Log.Error("AIAgent","Echec: Erreur d'init de l'AISpell %u. InstType:%u HeroMode:%u (0 à 3) <---- Check your database",
                                           m_nextSpell->spell->Id,ModePlayer->iInstanceType,m_nextSpell->spellheroicmode);
                    
									   return; 
									}
								}
								else spellRangeInfo = m_nextSpell->spell;

								SpellCastTargets rangeTargets = setSpellTargets(spellRangeInfo, m_nextTarget);
								uint32 rangeTargetType = m_nextSpell->spelltargetType;

                                //Log.Warning("AIInterface","rangeTargetType: %u",rangeTargetType); 

								// On va gérer certaines tajectoires, celles concernant le player ou l'unité ennemie uniquement
								switch(rangeTargetType)
								{
								 case TTYPE_LAST_HIT_FROM_TARGET:
									{
										m_Unit->CastSpellAoF(rangeTargets.m_srcX,rangeTargets.m_srcY,rangeTargets.m_srcZ, spellRangeInfo, true);
										break;
									}
								 case TTYPE_DESTINATION:
									{
										m_Unit->CastSpellAoF(rangeTargets.m_destX,rangeTargets.m_destY,rangeTargets.m_destZ, spellRangeInfo, true);
										break;
									}
								 case TTYPE_RANDOM_ENNEMY_UNIT:
									{
										m_Unit->GetBestTargetUnit(); // Envoie de la trajectoire hostile à l'unité pour le spell
										CastSpell(m_Unit, spellRangeInfo, rangeTargets); // Appel du spell qui va être utilisé
										break;
									}
								 case TTYPE_RANDOM_ENNEMY_PLAYER:
									{
									m_Unit->GetBestTargetPlayer(); // Envoie de la trajectoire pour le spell
									CastSpell(m_Unit, spellRangeInfo, rangeTargets); // Appel du spell qui va être utilisé
									break;
									}
								 case TTYPE_ENNEMY_PLAYER_IN_CLOSEST_RANGE:
									{
										m_Unit->GetBestTargetPlayer(TARGET_FILTER_CLOSEST); // Envoie de la trajectoire la plus proche pour le spell
										CastSpell(m_Unit, spellRangeInfo, rangeTargets); // Appel du spell qui va être utilisé
										break;
									}
								 case TTYPE_ENNEMY_UNIT_IN_CLOSEST_RANGE:
									{
										m_Unit->GetBestTargetUnit(TARGET_FILTER_CLOSEST); // Envoie de la trajectoire la plus proche à l'unité, pour le spell
										CastSpell(m_Unit, spellRangeInfo, rangeTargets); // Appel du spell qui va être utilisé
										break;
									}
								 case TTYPE_RANDOM_ENNEMY_UNIT_NOT_CURRENT:
									{
										m_Unit->GetBestTargetUnit(TARGET_FILTER_NOT_CURRENT); // Envoie de la trajectoire la plus proche pour le spell, à l'unité qui ne génère pas le plus de menace.
										CastSpell(m_Unit, spellRangeInfo, rangeTargets); // Appel du spell qui va être utilisé
										break;
									}							
								 case TTYPE_RANDOM_ENNEMY_PLAYER_NOT_CURRENT:
									{
										m_Unit->GetBestTargetPlayer(TARGET_FILTER_NOT_CURRENT); // Envoie de la trajectoire la plus proche pour le spell, pour le player qui ne génère pas le plus de menace.
										CastSpell(m_Unit, spellRangeInfo, rangeTargets); // Appel du spell qui va être utilisé
										break;
									}
								 case TTYPE_ENNEMY_IN_CLOSEST_RANGE_NOT_CURRENT:
									{
										m_Unit->GetBestTargetUnit(TARGET_FILTER_CLOSEST); // Envoie de la trajectoire la plus proche pour le spell, pour l'unité qui ne génère pas le plus de menace.
										CastSpell(m_Unit, spellRangeInfo, rangeTargets); // Appel du spell qui va être utilisé
										break;
									}
								 case TTYPE_RANDOM_ENNEMY_WOUNDED_UNIT:
									{
										m_Unit->GetBestTargetUnit(TARGET_FILTER_WOUNDED); // Envoie de la trajectoire la plus proche pour le spell, pour l'unité qui est blessée.
										CastSpell(m_Unit, spellRangeInfo, rangeTargets); // Appel du spell qui va être utilisé
										break;
									}
								 case TTYPE_RANDOM_ENNEMY_WOUNDED_PLAYER:
									{
										m_Unit->GetBestTargetPlayer(TARGET_FILTER_WOUNDED); // Envoie de la trajectoire la plus proche pour le spell, pour le player qui est blessé.
										CastSpell(m_Unit, spellRangeInfo, rangeTargets); // Appel du spell qui va être utilisé
										break;
									}
								 case TTYPE_SECOND_MOST_HASTED:
									{
										m_Unit->GetBestTargetPlayer(TARGET_FILTER_SECOND_MOSTED_HASTED); // Envoie de la trajectoire la plus proche pour le spell, pour le player qui génère en second, le plus de menace.
										CastSpell(m_Unit, spellRangeInfo, rangeTargets); // Appel du spell qui va être utilisé
										break;
									}
								 default:
									{
										Log.Error("AIAgent", "La trajectoire %u dans ce mode n'existe pas.", rangeTargetType);
										break;
									}
								}
							}
						}
					}
				}
				else // Target out of Range -> Run to it
				{
					//calculate next move
					/*float dist;

					if(distance < combatReach[0])// Target is too near
						dist = 9.0f;
					else
						dist = 20.0f;*/

					m_moveRun = true;
					_CalcDestinationAndMove(m_nextTarget, _CalcCombatRange(m_nextTarget, true));
				}
			}break;
		case AGENT_SPELL:
			{
				if(!m_nextSpell || !m_nextTarget)
					return;  // this shouldnt happen
					
				if( m_Unit->GetTypeId() == TYPEID_UNIT )
					static_cast<Creature *>(m_Unit)->SetSheatheForAttackType( 0 );
				/* stop moving so we don't interrupt the spell */
				//this the way justly suggested
//				if(m_nextSpell->spell->castingTimeIndex != 1)
				//do not stop for instant spells
				SpellCastTime *sd = dbcSpellCastTime.LookupEntry(m_nextSpell->spell->castingTimeIndex);
				if(GetCastTime(sd) != 0) StopMovement(0);

				bool los = true;

#ifdef COLLISION
				const LocationVector &loc1 = m_Unit->GetPosition();
				const LocationVector &loc2 = m_nextTarget->GetPosition();
	            
				SourceTaille = m_Unit->GetSize();
				TargetTaille = m_nextTarget->GetSize();

				los = CollideInterface.CheckLOS(m_Unit->GetMapId(),loc1.x,loc1.y,loc1.z+SourceTaille,loc2.x,loc2.y,loc2.z+TargetTaille);

				//if(los) Log.Success("AIAgent", "La cible est en vue");
				//else    Log.Success("AIAgent", "La cible n'est pas en vue");
				
#endif
				float distance = m_Unit->CalcDistance(m_nextTarget); // Renvoi le RootMeanSquare de la somme des carrés
				//Log.Success("AIAgent", "La distance avec la cible est de %.03f.", distance);

				if(los && ( ( (distance <= (/*m_nextSpell->maxrange**/m_nextSpell->maxrange))  &&  // Pas de carré, cf CalcDistance
					          (distance >= (/*m_nextSpell->minrange**/m_nextSpell->minrange))  )
					                                   || m_nextSpell->maxrange == 0)) // Target is in Range -> Attack
				{
					//Log.Success("AIAgent", "Le Pnj %u va utiliser le spell %u.",m_nextSpell->entryId,m_nextSpell->spell->Id);

					Player *ModePlayer;
					SpellEntry *spellHeroicInfo;
					SpellEntry *spellInfo;
					// --- Gestion du mode Héroïque par Randdrick
					// Certains sorts doivent se lancer qu'en mode Heroïque, d'autres qu'en mode non-héroïque
					// et la plupart des sorts peuvent se lancer dans tout les modes.
					// Valeur de spellheroïque :
					// 0->tous les modes, 1->standard 10 joueurs, 2-> standard 25 joueurs 
					// ( ou mode non-heroique uniquement), 3-> mode héroïque 10 joueurs, 4-> mode héroïque 25 joueurs
					// Mode héroïque.

					//On sélectionne le joueur le plus proche pour déterminer son mode de jeu.
					ModePlayer = m_Unit->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(m_Unit->GetPositionX(), 
						                                                                     m_Unit->GetPositionY(), 
																							 m_Unit->GetPositionZ()+SourceTaille);
					if(ModePlayer) 
					{
						//Mode héroïque                                                                                  25 joueurs
						if( (ModePlayer->iInstanceType >= MODE_HEROIC) && ( m_nextSpell->spellheroicmode == 0  || m_nextSpell->spellheroicmode == 3 || m_nextSpell->spellheroicmode == 4))
						{
						spellHeroicInfo = m_nextSpell->spell;
						spellInfo = spellHeroicInfo;
						//Log.Success("AIAgent", "Joueur detecte en mode heroique: le spell lance est %u.", spellInfo->Id);
						}
						// Mode non-héroïque.                                       // 10 joueurs
						else if ( m_nextSpell->spellheroicmode == 0  || m_nextSpell->spellheroicmode == 1 || m_nextSpell->spellheroicmode == 2)
						{
						 spellHeroicInfo = NULL;	 
						 spellInfo = m_nextSpell->spell;
						//Log.Success("AIAgent", "Joueur detecte: le spell lance est %u.", spellInfo->Id);
						}
						else // probleme ModeHero, on sort, a resoudre qd meme avec le spell standard (spellMeleeInfo = m_nextSpell->spell;)
					    {
						 Log.Error("AIAgent","Echec: Erreur d'init de l'AISpell %u. InstType:%u HeroMode:%u (0 à 3) <---- Check your database",
                                           m_nextSpell->spell->Id,ModePlayer->iInstanceType,m_nextSpell->spellheroicmode);
                    
						 return;
						}
					}
					else 
					{
						spellInfo = m_nextSpell->spell;
						//Log.Success("AIAgent", "Pas de joueur trouve, spell par defaut (%u).",spellInfo->Id);
					}

					SpellCastTargets targets = setSpellTargets(spellInfo, m_nextTarget);
					uint32 targettype = m_nextSpell->spelltargetType;
					//Log.Success("AIAgent", "Target Type : %u",targettype);
					switch(targettype)
					{
					case TTYPE_HIMSELF: // TTYPE_CASTER:
					case TTYPE_SINGLETARGET:
					case TTYPE_MAX_THREATEN_FROM_TARGET:
						{
							CastSpell(m_Unit, spellInfo, targets);
							break;
						}
					case TTYPE_SOURCE:
					case TTYPE_LAST_HIT_FROM_TARGET:
						{
							m_Unit->CastSpellAoF(targets.m_srcX,targets.m_srcY,targets.m_srcZ, spellInfo, true);
							break;
						}
					case TTYPE_DESTINATION:
						{
							m_Unit->CastSpellAoF(targets.m_destX,targets.m_destY,targets.m_destZ, spellInfo, true);
							break;
						}
					case TTYPE_RANDOM_FRIENDLY_UNIT:
						{
							m_Unit->GetBestTargetUnit(TARGET_FILTER_FRIENDLY); // Envoie de la trajectoire à l'unité pour le spell
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_RANDOM_ENNEMY_UNIT:
						{
							m_Unit->GetBestTargetUnit(); // Envoie de la trajectoire ennemie à l'unité pour le spell
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_RANDOM_ENNEMY_PLAYER:
						{
							m_Unit->GetBestTargetPlayer(); // Envoie de la trajectoire pour le spell
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_RANDOM_FRIENDLY_PLAYER:
						{
							m_Unit->GetBestTargetPlayer(TARGET_FILTER_FRIENDLY); // Envoie de la trajectoire amicale pour le spell
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_FRIENDLY_PLAYER_IN_CLOSEST_RANGE:
						{
							m_Unit->GetBestTargetPlayer(Target_Filter(TARGET_FILTER_CLOSEST | TARGET_FILTER_FRIENDLY)); // Envoie de la trajectoire amicale la plus proche pour le spell
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_ENNEMY_PLAYER_IN_CLOSEST_RANGE:
						{
							m_Unit->GetBestTargetPlayer(TARGET_FILTER_CLOSEST); // Envoie de la trajectoire la plus proche pour le spell
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_FRIENDLY_UNIT_IN_CLOSEST_RANGE:
						{
							m_Unit->GetBestTargetUnit(Target_Filter(TARGET_FILTER_CLOSEST | TARGET_FILTER_FRIENDLY)); // Envoie de la trajectoire amicale la plus proche à l'unité pour le spell
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_ENNEMY_UNIT_IN_CLOSEST_RANGE:
						{
							m_Unit->GetBestTargetUnit(TARGET_FILTER_CLOSEST); // Envoie de la trajectoire la plus proche à l'unité, pour le spell
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_RANDOM_FRIENDLY_UNIT_NOT_CURRENT:
						{
							m_Unit->GetBestTargetUnit(Target_Filter(TARGET_FILTER_FRIENDLY | TARGET_FILTER_NOT_CURRENT)); // Envoie de la trajectoire amicale la plus proche pour le spell, à l'unité qui ne génère pas le plus de menace.
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_RANDOM_ENNEMY_UNIT_NOT_CURRENT:
						{
							m_Unit->GetBestTargetUnit(TARGET_FILTER_NOT_CURRENT); // Envoie de la trajectoire la plus proche pour le spell, à l'unité qui ne génère pas le plus de menace.
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}							
					case TTYPE_RANDOM_FRIENDLY_PLAYER_NOT_CURRENT:
						{
							m_Unit->GetBestTargetPlayer(Target_Filter(TARGET_FILTER_FRIENDLY | TARGET_FILTER_NOT_CURRENT)); // Envoie de la trajectoire amicale la plus proche pour le spell, pour le player qui ne génère pas le plus de menace.
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_RANDOM_ENNEMY_PLAYER_NOT_CURRENT:
						{
							m_Unit->GetBestTargetPlayer(TARGET_FILTER_NOT_CURRENT); // Envoie de la trajectoire la plus proche pour le spell, pour le player qui ne génère pas le plus de menace.
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_FRIENDLY_IN_CLOSEST_RANGE_NOT_CURRENT:
						{
							m_Unit->GetBestTargetUnit(Target_Filter(TARGET_FILTER_CLOSEST | TARGET_FILTER_FRIENDLY)); // Envoie de la trajectoire amicale la plus proche pour le spell, pour l'unité qui ne génère pas le plus de menace.
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_ENNEMY_IN_CLOSEST_RANGE_NOT_CURRENT:
						{
							m_Unit->GetBestTargetUnit(TARGET_FILTER_CLOSEST); // Envoie de la trajectoire la plus proche pour le spell, pour l'unité qui ne génère pas le plus de menace.
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_RANDOM_FRIENDLY_WOUNDED_UNIT:
						{
							m_Unit->GetBestTargetUnit(Target_Filter(TARGET_FILTER_FRIENDLY | TARGET_FILTER_WOUNDED)); // Envoie de la trajectoire amicale la plus proche pour le spell, pour l'unité qui est blessée.
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_RANDOM_ENNEMY_WOUNDED_UNIT:
						{
							m_Unit->GetBestTargetUnit(TARGET_FILTER_WOUNDED); // Envoie de la trajectoire la plus proche pour le spell, pour l'unité qui est blessée.
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_RANDOM_FRIENDLY_WOUNDED_PLAYER:
						{
							m_Unit->GetBestTargetPlayer(Target_Filter(TARGET_FILTER_FRIENDLY | TARGET_FILTER_WOUNDED)); // Envoie de la trajectoire amicale la plus proche pour le spell, pour le player qui est blessé.
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_RANDOM_ENNEMY_WOUNDED_PLAYER:
						{
							m_Unit->GetBestTargetPlayer(TARGET_FILTER_WOUNDED); // Envoie de la trajectoire la plus proche pour le spell, pour le player qui est blessé.
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					case TTYPE_SECOND_MOST_HASTED:
						{
							m_Unit->GetBestTargetPlayer(TARGET_FILTER_SECOND_MOSTED_HASTED); // Envoie de la trajectoire la plus proche pour le spell, pour le player qui génère en second, le plus de menace.
							CastSpell(m_Unit, spellInfo, targets); // Appel du spell qui va être utilisé
							break;
						}
					default:
						{
							Log.Error("AIAgent", "TargetType inconnu : %u <--  Verifier la DB. (AI_Agents)", targettype);
							return;
						}
					}

					//Did we give it a sound ID?	
					if( m_nextSpell && m_nextSpell->Misc2 )  m_Unit->PlaySoundToSet(m_nextSpell->Misc2);

					if(m_nextSpell && m_nextSpell->cooldown) m_nextSpell->cooldowntime = getMSTime() + m_nextSpell->cooldown;

					//add pet spell after use to pet owner with some chance
					if(m_Unit->IsPet() && m_PetOwner->IsPlayer())
					{	
						Pet * pPet = static_cast<Pet*>(m_Unit);
						if(pPet && Rand(10))
							pPet->AddPetSpellToOwner(spellInfo->Id);
					}
					//increase procCounter if we're counting
					if(m_nextSpell && m_nextSpell->procCount) m_nextSpell->procCounter++;

				}
				else // Target out of Range -> Run to it
				{
					//Log.Success("AIAgent", "Joueur hors de portee.");
					//calculate next move
					m_moveRun = true;
					float combrange=_CalcCombatRange(m_nextTarget, false);
					if(m_nextSpell->maxrange <= 5.0f || m_nextSpell->maxrange < combrange)
						_CalcDestinationAndMove(m_nextTarget, combrange);
					else
						_CalcDestinationAndMove(m_nextTarget, m_nextSpell->maxrange - 5.0f);
				}
				//Null out the agent, so we force the lookup of the next spell.
				// m_aiCurrentAgent = AGENT_NULL;
			}break;
		case AGENT_FLEE:
			{
				//float dist = 5.0f;

				m_moveRun = false;
				if(m_fleeTimer == 0)
					m_fleeTimer = m_FleeDuration;

				/*Destination* dst = _CalcDestination(m_nextTarget, dist);
				MoveTo(dst->x, dst->y, dst->z,0);
				delete dst;*/
				_CalcDestinationAndMove(m_nextTarget, 5.0f);
				if(!m_hasFleed)
					CALL_SCRIPT_EVENT(m_Unit, OnFlee)(m_nextTarget);

				m_AIState = STATE_FLEEING;
				//removed by Zack : somehow creature starts to attack sefl. Just making sure it is not this one
//				m_nextTarget = m_Unit;
//				m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, 0);
				SetNextTarget(NULL);

				WorldPacket data( SMSG_MESSAGECHAT, 100 );
				string msg = "%s tente de s'enfuir!";
				data << (uint8)CHAT_MSG_CHANNEL;
				data << (uint32)LANG_UNIVERSAL;
				data << (uint32)( strlen( static_cast< Creature* >( m_Unit )->GetCreatureName()->Name ) + 1 );
				data << static_cast< Creature* >( m_Unit )->GetCreatureName()->Name;
				data << (uint64)0;
				data << (uint32)(msg.size() + 1);
				data << msg;
				data << uint8(0);

				m_Unit->SendMessageToSet(&data, false);

				//m_Unit->SendChatMessage(CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, msg);
				//sChatHandler.FillMessageData(&data, CHAT_MSG_MONSTER_EMOTE, LANG_UNIVERSAL, msg, m_Unit->GetGUID());			   
			   
				m_hasFleed = true;
			}break;
		case AGENT_CALLFORHELP:
			{
				FindFriends( 50.0f /*7.0f*/ );
				m_hasCalledForHelp = true; // We only want to call for Help once in a Fight.
				if( m_Unit->GetTypeId() == TYPEID_UNIT )
						objmgr.HandleMonsterSayEvent( static_cast< Creature* >( m_Unit ), MONSTER_SAY_EVENT_CALL_HELP );
				CALL_SCRIPT_EVENT( m_Unit, OnCallForHelp )();
			}break;

		default : Log.Warning("AIAgent", "AGENT_NULL."); 
			      break;
		}
	}
	else if( !m_nextTarget || !m_Unit->isCasting() ||m_nextTarget->GetInstanceID() != m_Unit->GetInstanceID() || !m_nextTarget->isAlive() || !m_nextTarget->IsInWorld() )
	{
		// no more target
		SetNextTarget( NULL );
	}
}

void AIInterface::DismissPet()
{
	/*
	if(m_AIType != AITYPE_PET)
		return;

	if(!m_PetOwner)
		return;
	
	if(m_PetOwner->GetTypeId() != TYPEID_PLAYER)
		return;

	if(m_Unit->GetUInt32Value(UNIT_CREATED_BY_SPELL) == 0)
		static_cast< Player* >( m_PetOwner )->SetFreePetNo(false, (int)m_Unit->GetUInt32Value(UNIT_FIELD_PET_LGUID));
	static_cast< Player* >( m_PetOwner )->SetPet(NULL);
	static_cast< Player* >( m_PetOwner )->SetPetName("");
	
	//FIXME:Check hunter pet or not
	//FIXME:Check enslaved creature
	m_PetOwner->SetUInt64Value(UNIT_FIELD_SUMMON, 0);
	
	WorldPacket data;
	data.Initialize(SMSG_PET_SPELLS);
	data << (uint64)0;
	static_cast< Player* >( m_PetOwner )->GetSession()->SendPacket(&data);
	
	sEventMgr.RemoveEvents(((Creature*)m_Unit));
	if(m_Unit->IsInWorld())
	{
		m_Unit->RemoveFromWorld();
	}
	//setup an event to delete the Creature
	sEventMgr.AddEvent(((Creature*)this->m_Unit), &Creature::DeleteMe, EVENT_DELETE_TIMER, 1, 1);*/
}

void AIInterface::AttackReaction(Unit* pUnit, uint32 damage_dealt, uint32 spellId)
{
	if( m_AIState == STATE_EVADE || m_fleeTimer != 0 || !pUnit || !pUnit->isAlive() || m_Unit->IsPacified() || m_Unit->IsFeared() || m_Unit->IsStunned() || !m_Unit->isAlive() || !m_AllowedToEnterCombat )
	{
		//Log.Warning("AttackReaction","Echec, %s, Spell %u",(pUnit->GetObjectName())?(pUnit->GetObjectName()):"Inconnu",spellId);
		return;
	}

	//soul link deals damage to pet
	if (m_Unit->IsPet() && m_Unit->GetPlayerFrom() == pUnit)
	{
		//Log.Warning("AttackReaction","Echec2: %s, Spell: %u",(pUnit->GetObjectName())?(pUnit->GetObjectName()):"Inconnu",spellId);
		return;
	}

	if(m_Unit == pUnit)
	{
		//Log.Warning("AttackReaction","Echec3: m_Unit == pUnit, %s, Spell: %u",(pUnit->GetObjectName())?(pUnit->GetObjectName()):"Inconnu",spellId);
		return;
	}

	if( m_AIState == STATE_IDLE || m_AIState == STATE_FOLLOWING )
	{
		WipeTargetList(); // Raz m_nexttarget
		//Log.Warning("AttackReaction","EVENT_ENTERCOMBAT, %s, Spell: %u",(pUnit->GetObjectName())?(pUnit->GetObjectName()):"Inconnu",spellId);
		HandleEvent(EVENT_ENTERCOMBAT, pUnit, 0);
	}
	//else // Manque pas le else ?? Test (Branruz) => Fonctionnement de l'attaque à eclaircir
	{
	 //Log.Warning("AttackReaction","EVENT_DAMAGETAKEN, %s, Spell: %u",(pUnit->GetObjectName())?(pUnit->GetObjectName()):"Inconnu",spellId);
	 HandleEvent(EVENT_DAMAGETAKEN, pUnit, _CalcThreat(damage_dealt, spellId ? dbcSpell.LookupEntryForced(spellId) : NULL, pUnit));
	}
	
	//add threat to the controlled childs, works with players because aiinterface is enabled when they are controlled
	for (std::set<uint64>::iterator itr=MindControlChilds.begin(); itr!=MindControlChilds.end(); ++itr)
	{
		if (m_Unit->GetMapMgr()->GetUnit(*itr) == NULL)
			continue;

		m_Unit->GetMapMgr()->GetUnit(*itr)->GetAIInterface()->AttackReaction(pUnit, 1, 0);
	}

	//Log.Warning("AttackReaction","En cours, %s, Spell: %u, AIState: %u",
	//	 (pUnit->GetObjectName())?(pUnit->GetObjectName()):"Inconnu",spellId,m_AIState);
}

bool AIInterface::HealReaction(Unit* caster, Unit* victim, uint32 amount)
{
	if(!caster || !victim)
	{
		Log.Error("HealReaction","!!!MAUVAIS POINTEUR DANS AIInterface::HealReaction!!!");
		return false;
	}

	int casterInList = 0, victimInList = 0;

	if(m_aiTargets.find(caster) != m_aiTargets.end())
		casterInList = 1;

	if(m_aiTargets.find(victim) != m_aiTargets.end())
		victimInList = 1;

	/*for(i = m_aiTargets.begin(); i != m_aiTargets.end(); i++)
	{
		if(casterInList && victimInList)
		{ // no need to check the rest, just break that
			break;
		}
		if(i->target == victim)
		{
			victimInList = true;
		}
		if(i->target == caster)
		{
			casterInList = true;
		}
	}*/
	if(!victimInList && !casterInList) // none of the Casters is in the Creatures Threat list
	{
		return false;
	}
	if(!casterInList && victimInList) // caster is not yet in Combat but victim is
	{
		// get caster into combat if he's hostile
		if(isHostile(m_Unit, caster))
		{
			//AI_Target trgt;
			//trgt.target = caster;
			//trgt.threat = amount;
			//m_aiTargets.push_back(trgt);
			m_aiTargets.insert(TargetMap::value_type(caster, amount));
			return true;
		}
		return false;
	}
	else if(casterInList && victimInList) // both are in combat already
	{
		// mod threat for caster
		modThreatByPtr(caster, amount);
		return true;
	}
	else // caster is in Combat already but victim is not
	{
		modThreatByPtr(caster, amount);
		// both are players so they might be in the same group
		if( caster->GetTypeId() == TYPEID_PLAYER && victim->GetTypeId() == TYPEID_PLAYER )
		{
			if( static_cast< Player* >( caster )->GetGroup() == static_cast< Player* >( victim )->GetGroup() )
			{
				// get victim into combat since they are both
				// in the same party
				if( isHostile( m_Unit, victim ) )
				{
					m_aiTargets.insert( TargetMap::value_type( victim, 1 ) );
					return true;
				}
				return false;
			}
		}
	}

	return false;
}

void AIInterface::OnDeath(Object* pKiller)
{
	if(pKiller->GetTypeId() == TYPEID_PLAYER || pKiller->GetTypeId() == TYPEID_UNIT)
		HandleEvent(EVENT_UNITDIED, static_cast<Unit*>(pKiller), 0);
	else
		HandleEvent(EVENT_UNITDIED, m_Unit, 0);
}

Unit* AIInterface::FindTarget()
{// find nearest hostile Target to attack
	if( !m_AllowedToEnterCombat ) 
		return NULL;

	Unit* target = NULL;
	Unit* critterTarget = NULL;
	float distance = 999999.0f; // that should do it.. :p
	float crange;
	float z_diff;
#ifdef LOS_CHECKS
#ifdef LOS_ONLY_IN_INSTANCE
	bool los = true;
	bool check_los = true;
	if( m_Unit->GetMapMgr()->GetMapInfo()->type == INSTANCE_NULL )
		check_los = false;
#endif
#endif

	std::set<Object*>::iterator itr, it2;
	Object *pObj;
	Unit *pUnit;
	float dist;
	bool pvp=true;
	if(m_Unit->GetTypeId()==TYPEID_UNIT&&((Creature*)m_Unit)->GetCreatureName()&&((Creature*)m_Unit)->GetCreatureName()->Civilian)
		pvp=false;

	//target is immune to all form of attacks, cant attack either.
	if(m_Unit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
	{
		return 0;
	}
	
	//these npc flags never aggro until attacked
	if (m_Unit->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR | UNIT_NPC_FLAG_TRAINER | UNIT_NPC_FLAG_ARMORER | UNIT_NPC_FLAG_TAXIVENDOR | UNIT_NPC_FLAG_INNKEEPER | UNIT_NPC_FLAG_BANKER | UNIT_NPC_FLAG_AUCTIONEER | UNIT_NPC_FLAG_STABLEMASTER))
		return NULL;

	for( itr = m_Unit->GetInRangeOppFactsSetBegin(); itr != m_Unit->GetInRangeOppFactsSetEnd(); )
	{
		it2 = itr;
		++itr;

		pObj = (*it2);

		if( pObj->GetTypeId() == TYPEID_PLAYER )
		{
			if(static_cast< Player* >( pObj )->GetTaxiState() )	  // skip players on taxi
				continue;
		}
		else if( pObj->GetTypeId() != TYPEID_UNIT )
				continue;

		pUnit = static_cast< Unit* >( pObj );
		if( pUnit->bInvincible )
			continue;

		// don't agro players on flying mounts
		/*if(pUnit->GetTypeId() == TYPEID_PLAYER && static_cast< Player* >(pUnit)->FlyCheat)
			continue;*/

		//do not agro units that are faking death. Should this be based on chance ?
		if( pUnit->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH ) )
			continue;

		//target is immune to unit attacks however can be targeted
		//as a part of AI we allow this mode to attack creatures as seen many times on oficial.
		if( m_Unit->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 ) || pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9) )
		{
			if (m_Unit->IsFromPlayer() || pUnit->IsFromPlayer())
				continue;			
		}

		/* is it a player? we have to check for our pvp flag. */
//		if(m_U)
		crange = _CalcCombatRange(pUnit,false);
		if(m_isGuard)
			crange *= 4;

		z_diff = fabs(m_Unit->GetPositionZ() - pUnit->GetPositionZ());
		if(z_diff > crange)
		{
			continue;
		}
		
		if(!pUnit->isAlive()
			|| m_Unit == pUnit /* wtf? */
			|| m_Unit->GetUInt64Value(UNIT_FIELD_CREATEDBY) == pUnit->GetGUID())
			continue;

		dist = m_Unit->GetDistanceSq(pUnit);
		if(!pUnit->m_faction || !pUnit->m_factionDBC)
			continue;

		if(pUnit->m_faction->Faction == 28)// only Attack a critter if there is no other Enemy in range
		{
			if(dist < 225.0f)	// was 10
				critterTarget = pUnit;
			continue;
		}

		if(dist > distance)	 // we want to find the CLOSEST target
			continue;
		
		if(dist <= _CalcAggroRange(pUnit) )
		{
#ifdef LOS_CHECKS // Define si Collision existe

		const LocationVector &loc1 = m_Unit->GetPositionNC();
		const LocationVector &loc2 = pUnit->GetPositionNC();
	            
		float SourceTaille = m_Unit->GetSize();
		float TargetTaille = pUnit->GetSize();

		// if( CollideInterface.CheckLOS( m_Unit->GetMapId( ), m_Unit->GetPositionNC( ), pUnit->GetPositionNC( ) ) )
		if(CollideInterface.CheckLOS(m_Unit->GetMapId(),loc1.x,loc1.y,loc1.z+SourceTaille,loc2.x,loc2.y,loc2.z+TargetTaille))
		{
			distance = dist;
			target = pUnit;
		}
#else
		distance = dist;
		target = pUnit;
#endif		// LOS_CHECKS

		}
	}

	if( !target )
	{
		target = critterTarget;
	}

	if( target )
	{
/*		if(m_isGuard)
		{
			m_Unit->m_runSpeed = m_Unit->m_base_runSpeed * 2.0f;
			m_fastMove = true;
		}*/

		AttackReaction(target, 1, 0);
		if(target->IsPlayer())
		{
			WorldPacket data(SMSG_AI_REACTION, 12);
			data << m_Unit->GetGUID() << uint32(2);		// Aggro sound
			static_cast< Player* >( target )->GetSession()->SendPacket( &data );
		}
		if(target->GetUInt32Value(UNIT_FIELD_CREATEDBY) != 0)
		{
			Unit* target2 = m_Unit->GetMapMgr()->GetPlayer(target->GetUInt32Value(UNIT_FIELD_CREATEDBY));
			/*if(!target2)
			{
				target2 = sObjHolder.GetObject<Player>(target->GetUInt32Value(UNIT_FIELD_CREATEDBY));
			}*/
			if(target2)
			{
				AttackReaction(target2, 1, 0);
			}
		}
	}
	return target;
}

Unit* AIInterface::FindTargetForSpell(AI_Spell *sp)
{
	/*if(!m_Unit) return NULL;*/

	/*if(!sp)
	{
		m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, 0);
		return NULL;
	}*/

	TargetMap::iterator itr, itr2;

	if(sp)
	{
		if(sp->spellType == STYPE_HEAL)
		{
			uint32 cur = m_Unit->GetUInt32Value(UNIT_FIELD_HEALTH) + 1;
			uint32 max = m_Unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH) + 1;
			float healthPercent = float(cur) / float(max);
			if(healthPercent <= sp->floatMisc1) // Heal ourselves cause we got too low HP
			{
				m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, 0);
				return m_Unit;
			}
			for(AssistTargetSet::iterator i = m_assistTargets.begin(); i != m_assistTargets.end(); i++)
			{
				if(!(*i)->isAlive())
				{
					continue;
				}
				cur = (*i)->GetUInt32Value(UNIT_FIELD_HEALTH);
				max = (*i)->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
				healthPercent = float(cur) / float(max);
				if(healthPercent <= sp->floatMisc1) // Heal ourselves cause we got too low HP
				{
					m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, (*i)->GetGUID());
					return (*i); // heal Assist Target which has low HP
				}
			}
		}
		if(sp->spellType == STYPE_BUFF)
		{
			m_Unit->SetUInt64Value(UNIT_FIELD_TARGET, 0);
			return m_Unit;
		}
	}

	return GetMostHated();
}

bool AIInterface::FindFriends(float dist)
{
	bool result = false;
	TargetMap::iterator it;
	uint32 guardid;

	std::set<Object*>::iterator itr;
	Unit *pUnit;

	

	for( itr = m_Unit->GetInRangeSetBegin(); itr != m_Unit->GetInRangeSetEnd(); itr++ )
	{
		if(!(*itr) || (*itr)->GetTypeId() != TYPEID_UNIT)
			continue;

		pUnit = static_cast<Unit*>((*itr));
		if(!pUnit->isAlive())
			continue;

		if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
		{
			continue;
		}
		if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9))
		{
			continue;
		}
		
		if( !(pUnit->m_Phase & m_Unit->m_Phase) ) //We can't help a friendly unit if it is not in our phase
			continue;
			
		if( isCombatSupport( m_Unit, pUnit ) && pUnit->GetAIInterface()->getAIState() != STATE_EVADE)//Not sure
		{
			if( m_Unit->GetDistanceSq(pUnit) < dist)
			{
				if( m_assistTargets.count( pUnit ) > 0 ) // already have him
					break;

				result = true;
				m_assistTargets.insert(pUnit);
					
				for(it = m_aiTargets.begin(); it != m_aiTargets.end(); ++it)
				{
					static_cast< Unit* >( *itr )->GetAIInterface()->AttackReaction( it->first, 1, 0 );
				}
			}
		}
	}

	// check if we're a civillan, in which case summon guards on a despawn timer
	uint8 civilian = (((Creature*)m_Unit)->GetCreatureName()) ? (((Creature*)m_Unit)->GetCreatureName()->Civilian) : 0;
	uint32 family = (((Creature*)m_Unit)->GetCreatureName()) ? (((Creature*)m_Unit)->GetCreatureName()->Type) : 0;
	if(family == HUMANOID && civilian && getMSTime() > m_guardTimer && !IS_INSTANCE(m_Unit->GetMapId()))
	{
		m_guardTimer = getMSTime() + 15000;
		uint16 AreaId = m_Unit->GetMapMgr()->GetAreaID(m_Unit->GetPositionX(),m_Unit->GetPositionY(),m_Unit->GetPositionZ());
		AreaTable * at = dbcArea.LookupEntry(AreaId);
		if(!at)
			return result;

		ZoneGuardEntry * zoneSpawn = ZoneGuardStorage.LookupEntry(at->ZoneId);
		if(!zoneSpawn) return result;

		uint32 team = 1; // horde default
		if(isAlliance(m_Unit))
			team = 0;

		if(team == 1) guardid = zoneSpawn->HordeEntry;
		else          guardid = zoneSpawn->AllianceEntry;
		if(!guardid) return result;

		CreatureInfo * ci = CreatureNameStorage.LookupEntry(guardid);
		if(!ci)
		{
			Log.Error("ZoneGuards","Pas de CreatureName correspondant pour GuardId %u",guardid);
			return result;
		}

		float x = m_Unit->GetPositionX() + (float)( (float)(rand() % 150 + 100) / 1000.0f );
		float y = m_Unit->GetPositionY() + (float)( (float)(rand() % 150 + 100) / 1000.0f );
#ifdef COLLISION
		float z = CollideInterface.GetHeight(m_Unit->GetMapId(), x, y, m_Unit->GetPositionZ() + m_Unit->GetSize() ); //2.0f);
		if( z == NO_WMO_HEIGHT )
			z = m_Unit->GetMapMgr()->GetLandHeight(x, y,m_Unit->GetPositionZ() + m_Unit->GetSize());

		if( fabs( z - m_Unit->GetPositionZ() ) > 10.0f )
			z = m_Unit->GetPositionZ();
#else
		float z = m_Unit->GetPositionZ();
		float adt_z = m_Unit->GetMapMgr()->GetLandHeight(x, y,m_Unit->GetPositionZ() + m_Unit->GetSize());
		if(fabs(z - adt_z) < 3)
			z = adt_z;
#endif
		float o = m_Unit->GetOrientation();

		CreatureProto * cp = CreatureProtoStorage.LookupEntry(guardid);
		if(!cp) return result;

		uint32 languageid = (team == 0) ? LANG_COMMON : LANG_ORCISH;
		m_Unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, languageid, "Gardes!");

		uint8 spawned = 0;
	
		std::set<Player*>::iterator hostileItr = m_Unit->GetInRangePlayerSetBegin();
		for(; hostileItr != m_Unit->GetInRangePlayerSetEnd(); hostileItr++)
		{
			if(spawned >= 3)
				break;

			if(!isHostile(*hostileItr, m_Unit))
				continue;

			Creature * guard = m_Unit->GetMapMgr()->CreateCreature(guardid);
			guard->Load(cp, x, y, z, o);
			guard->SetInstanceID(m_Unit->GetInstanceID());
			guard->SetZoneId(m_Unit->GetZoneId());
			guard->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP); /* shitty DBs */
			guard->m_noRespawn=true;
		
			if(guard->CanAddToWorld())
			{
				uint32 t = RandomUInt(8)*1000;
				if(t==0)
					guard->PushToWorld(m_Unit->GetMapMgr());
				else
					sEventMgr.AddEvent(guard,&Creature::AddToWorld, m_Unit->GetMapMgr(), EVENT_UNK, t, 1, 0);
			}
			else
			{
				guard->SafeDelete();
				return result;
			}
			
			sEventMgr.AddEvent(guard, &Creature::SetGuardWaypoints, EVENT_UNK, 10000, 1,0);
			sEventMgr.AddEvent(guard, &Creature::SafeDelete, EVENT_CREATURE_SAFE_DELETE, 60*5*1000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			spawned++;
		}
	}

	return result;
}

float AIInterface::_CalcAggroRange(Unit* target)
{
	//float baseAR = 15.0f; // Base Aggro Range
					  //   -8	  -7	 -6	    -5	   -4	  -3	 -2	    -1	    0	  +1	 +2	    +3	   +4	  +5	 +6	    +7	  +8
	//float baseAR[17] = {29.0f, 27.5f, 26.0f, 24.5f, 23.0f, 21.5f, 20.0f, 18.5f, 17.0f, 15.5f, 14.0f, 12.5f, 11.0f,  9.5f,  8.0f,  6.5f, 5.0f};
	float baseAR[17] = {19.0f, 18.5f, 18.0f, 17.5f, 17.0f, 16.5f, 16.0f, 15.5f, 15.0f, 14.5f, 12.0f, 10.5f, 8.5f,  7.5f,  6.5f,  6.5f, 5.0f};
	// Lvl Diff -8 -7 -6 -5 -4 -3 -2 -1 +0 +1 +2  +3  +4  +5  +6  +7  +8
	// Arr Pos   0  1  2  3  4  5  6  7  8  9 10  11  12  13  14  15  16
	int8 lvlDiff = target->getLevel() - m_Unit->getLevel();
	uint8 realLvlDiff = lvlDiff;
	if(lvlDiff > 8)
	{
		lvlDiff = 8;
	}
	if(lvlDiff < -8)
	{
		lvlDiff = -8;
	}
	if(!((Creature*)m_Unit)->CanSee(target))
		return 0;
	
	float AggroRange = baseAR[lvlDiff + 8];
	if(realLvlDiff > 8)
	{
		AggroRange += AggroRange * ((lvlDiff - 8) * 5 / 100);
	}
	
	// Spécial aggro par Randdrick. - Tient compte d'une arme longue distante du NPC - Valeur du Champ Combat_Reach > 30
	float selfreach = m_Unit->GetFloatValue(UNIT_FIELD_COMBATREACH);
	if(AggroRange < selfreach && selfreach > 30 ) AggroRange = (selfreach + lvlDiff) / 2 ;
	
	// Multiply by elite value
	if(((Creature*)m_Unit)->GetCreatureName() && ((Creature*)m_Unit)->GetCreatureName()->Rank > 0)
		AggroRange *= (((Creature*)m_Unit)->GetCreatureName()->Rank) * 1.50f;

	if(AggroRange > 40.0f) // cap at 40.0f
	{
		AggroRange = 40.0f;
	}	
  /*  //printf("aggro range: %f , stealthlvl: %d , detectlvl: %d\n",AggroRange,target->GetStealthLevel(),m_Unit->m_stealthDetectBonus);
	if(! ((Creature*)m_Unit)->CanSee(target))
	{
		AggroRange =0;
	//	AggroRange *= ( 100.0f - (target->m_stealthLevel - m_Unit->m_stealthDetectBonus)* 20.0f ) / 100.0f;
	}
*/
	// SPELL_AURA_MOD_DETECT_RANGE
	int32 modDetectRange = target->getDetectRangeMod(m_Unit->GetGUID());
	AggroRange += modDetectRange;
	if(target->IsPlayer())
		AggroRange += static_cast< Player* >( target )->DetectedRange;
	if(AggroRange < 3.0f)
	{
		AggroRange = 3.0f;
	}
	if(AggroRange > 40.0f) // cap at 40.0f
	{
		AggroRange = 40.0f;
	}
	
	return (AggroRange*AggroRange);
}

void AIInterface::_CalcDestinationAndMove(Unit *target, float dist)
{
	if(!m_canMove || m_Unit->IsStunned())
	{
		StopMovement(0); //Just Stop
		return;
	}
	
	if(target && (target->GetTypeId() == TYPEID_UNIT || target->GetTypeId() == TYPEID_PLAYER))
	{
		float ResX = target->GetPositionX();
		float ResY = target->GetPositionY();
		float ResZ = target->GetPositionZ();

		float angle = m_Unit->calcAngle(m_Unit->GetPositionX(), m_Unit->GetPositionY(), ResX, ResY) * float(M_PI) / 180.0f;
		float x = dist * cosf(angle);
		float y = dist * sinf(angle);

		if(target->GetTypeId() == TYPEID_PLAYER && static_cast< Player* >( target )->m_isMoving )
		{
			// cater for moving player vector based on orientation
			x -= cosf(target->GetOrientation());
			y -= sinf(target->GetOrientation());
		}

		m_nextPosX = ResX - x;
		m_nextPosY = ResY - y;
		//m_nextPosZ = ((Unit *)target)->GetMapMgr()->GetLandHeight(m_nextPosX, m_nextPosY); // bof, PB: Sur l'eau et Transports
		m_nextPosZ = ResZ;
	}
	else
	{
		target = NULL;
		m_nextPosX = m_Unit->GetPositionX();
		m_nextPosY = m_Unit->GetPositionY();
		m_nextPosZ = m_Unit->GetPositionZ();
	}

	float dx = m_nextPosX - m_Unit->GetPositionX();
	float dy = m_nextPosY - m_Unit->GetPositionY();
	if(dy != 0.0f)
	{
		float angle = atan2(dx, dy);
		m_Unit->SetOrientation(angle);
	}

	if(m_creatureState != MOVING)
		UpdateMove();
}

float AIInterface::_CalcCombatRange(Unit* target, bool ranged)
{
	if(!target) return(0); // Secu, ne devrait pas arriver
	if(!target->IsUnit()) return(0);
	if(!m_Unit->IsUnit()) return(0);

	float range = 0.0f;
	float rang = PLAYER_SIZE;
	if(ranged)
	{
		rang = 5.0f;
	}

	float selfreach = m_Unit->GetFloatValue(UNIT_FIELD_COMBATREACH);
	float targetradius = target->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS);
	float selfradius = m_Unit->GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS);
	float targetscale = target->GetFloatValue(OBJECT_FIELD_SCALE_X);
	float selfscale = m_Unit->GetFloatValue(OBJECT_FIELD_SCALE_X);

	range = (targetradius * targetscale) + selfreach + (selfradius * selfscale) + rang;

	if(range < PLAYER_SIZE)
		range = PLAYER_SIZE;	
	return range;
}

float AIInterface::_CalcDistanceFromHome()
{
	if (m_AIType == AITYPE_PET)
	{
		return m_Unit->GetDistanceSq(m_PetOwner);
	}
	else if(m_Unit->GetTypeId() == TYPEID_UNIT)
	{

		if(m_returnX !=0.0f && m_returnY != 0.0f)
		{
			return m_Unit->GetDistanceSq(m_returnX,m_returnY,m_returnZ);
		}
	}

	return 0.0f;
}

/************************************************************************************************************
SendMoveToPacket:
Comments: Some comments on the SMSG_MONSTER_MOVE packet: 
	the uint8 field:
		0: Default															known
		1: Don't move														known
		2: there is an extra 3 floats, also known as a vector				unknown
		3: there is an extra uint64 most likely a guid.						unknown
		4: there is an extra float that causes the orientation to be set.	known
		
		note:	when this field is 1. 
			there is no need to send  the next 3 uint32's as they are'nt used by the client
	
	the MoveFlags:
		0x00000000 - Walk
		0x00000100 - Run
		0x00000200 - Fly
		some comments on that 0x00000300 - Fly = 0x00000100 | 0x00000200

	waypoints:
		TODO.... as they somehow seemed to be changed long time ago..
		
*************************************************************************************************************/

void AIInterface::SendMoveToPacket(float toX, float toY, float toZ, float toO, uint32 time, uint32 MoveFlags)
{
	//this should NEVER be called directly !!!!!!
	//use MoveTo()
	//WorldPacket data(SMSG_MONSTER_MOVE, 60);
	uint8 buffer[100];
	StackPacket data(SMSG_MONSTER_MOVE, buffer, 100);
	data << m_Unit->GetNewGUID();
	data << uint8(0);
	data << m_Unit->GetPositionX() << m_Unit->GetPositionY() << m_Unit->GetPositionZ();
	data << getMSTime();
	
	// Check if we have an orientation
	if(toO != 0.0f)
	{
		data << uint8(4);
		data << toO;
	}
	else
	{
		data << uint8(0);
	}

	data << MoveFlags;
	if(MoveFlags & 0x200000)
	{
		data << uint8(0);
		data << uint32(0);
	}
	if(MoveFlags & 0x800)
	{
		data << float(0);
		data << uint32(0);
	}

	data << time;
	data << uint32(1);	  // 1 waypoint
	data << toX << toY << toZ;

#ifndef ENABLE_COMPRESSED_MOVEMENT_FOR_CREATURES
	bool self = m_Unit->GetTypeId() == TYPEID_PLAYER;
	m_Unit->SendMessageToSet( &data, self );
#else
	if( m_Unit->GetTypeId() == TYPEID_PLAYER )
		static_cast<Player*>(m_Unit)->GetSession()->SendPacket(&data);

	for(set<Player*>::iterator itr = m_Unit->GetInRangePlayerSetBegin(); itr != m_Unit->GetInRangePlayerSetEnd(); ++itr)
	{
		if( (*itr)->GetPositionNC().Distance2DSq( m_Unit->GetPosition() ) >= World::m_movementCompressThresholdCreatures )
			(*itr)->AppendMovementData( SMSG_MONSTER_MOVE, data.GetSize(), (const uint8*)data.GetBufferPointer() );
		else
			(*itr)->GetSession()->SendPacket(&data);
	}
#endif
/*
	//this should NEVER be called directly !!!!!!
	//use MoveTo()
	//WorldPacket data(SMSG_MONSTER_MOVE, 60);
	uint8 buffer[100];
	StackPacket data(SMSG_MONSTER_MOVE, buffer, 100);
	data << uint8( 0 ); //VLack: for 3.1.x support; I've discovered this in Mangos code while doing research on how to fix invisible mobs on 3.0.9
	data << m_Unit->GetNewGUID();
	data << m_Unit->GetPositionX() << m_Unit->GetPositionY() << m_Unit->GetPositionZ();
	data << getMSTime();
	
	// Check if we have an orientation
	if(toO != 0.0f)
	{
		data << uint8(4);
		data << toO;
	} else {
		data << uint8(0);
	}
	data << MoveFlags;
	data << time;
	data << uint32(1);	  // 1 waypoint
	data << toX << toY << toZ;

#ifndef ENABLE_COMPRESSED_MOVEMENT_FOR_CREATURES
	bool self = m_Unit->GetTypeId() == TYPEID_PLAYER;
	m_Unit->SendMessageToSet( &data, self );
#else
	if( m_Unit->GetTypeId() == TYPEID_PLAYER )
		static_cast<Player*>(m_Unit)->GetSession()->SendPacket(&data);

	for(set<Player*>::iterator itr = m_Unit->GetInRangePlayerSetBegin(); itr != m_Unit->GetInRangePlayerSetEnd(); ++itr)
	{
		if( (*itr)->GetPositionNC().Distance2DSq( m_Unit->GetPosition() ) >= World::m_movementCompressThresholdCreatures )
			(*itr)->AppendMovementData( SMSG_MONSTER_MOVE, data.GetSize(), (const uint8*)data.GetBufferPointer() );
		else
			(*itr)->GetSession()->SendPacket(&data);
	}
#endif
*/
}

/*
void AIInterface::SendMoveToSplinesPacket(std::list<Waypoint> wp, bool run)
{
	if(!m_canMove)
	{
		return;
	}

	WorldPacket data;

	uint8 DontMove = 0;
	uint32 travelTime = 0;
	for(std::list<Waypoint>::iterator i = wp.begin(); i != wp.end(); i++)
	{
		travelTime += i->time;
	}

	data.Initialize( SMSG_MONSTER_MOVE );
	data << m_Unit->GetNewGUID();
	data << m_Unit->GetPositionX() << m_Unit->GetPositionY() << m_Unit->GetPositionZ();
	data << getMSTime();
	data << uint8(DontMove);
	data << uint32(run ? 0x00000100 : 0x00000000);
	data << travelTime;
	data << (uint32)wp.size();
	for(std::list<Waypoint>::iterator i = wp.begin(); i != wp.end(); i++)
	{
		data << i->x;
		data << i->y;
		data << i->z;
	}

	m_Unit->SendMessageToSet( &data, false );
}
*/
bool AIInterface::StopMovement(uint32 time)
{
	m_moveTimer = time; //set pause after stopping
	m_creatureState = STOPPED;

	m_destinationX = m_destinationY = m_destinationZ = 0;
	m_nextPosX = m_nextPosY = m_nextPosZ = 0;
	m_timeMoved = 0;
	m_timeToMove = 0;

	WorldPacket data(26);
	data.SetOpcode(SMSG_MONSTER_MOVE);
	data << m_Unit->GetNewGUID();
	data << uint8(0); //VLack: 3.1 SMSG_MONSTER_MOVE change...
	data << m_Unit->GetPositionX() << m_Unit->GetPositionY() << m_Unit->GetPositionZ();
	data << getMSTime();
	data << uint8(1);   // "DontMove = 1"

	m_Unit->SendMessageToSet( &data, false );
	return true;
}

void AIInterface::MoveTo(float x, float y, float z, float o)
{
	m_sourceX = m_Unit->GetPositionX();
	m_sourceY = m_Unit->GetPositionY();
	m_sourceZ = m_Unit->GetPositionZ();

	if(!m_canMove || m_Unit->IsStunned())
	{
		StopMovement(0); //Just Stop
		return;
	}
	
	//if were casting, start moving when were finished casting
	if (m_Unit->GetCurrentSpell() != NULL)
	{
		// Note Randdrick : A quoi ça sert de définir la position suivante dans ce cas là ?
		// Pour le prochain passage ?? (Branruz)
		m_nextPosX = x;
		m_nextPosY = y;
		m_nextPosZ = z;
		return;
	}

	m_nextPosX = x;
	m_nextPosY = y;
	m_nextPosZ = z;

	if(m_creatureState != MOVING)
		UpdateMove();
}

bool AIInterface::IsFlying()
{
	if(m_moveFly)
		return true;
	
	/*float z = m_Unit->GetMapMgr()->GetLandHeight(m_Unit->GetPositionX(), m_Unit->GetPositionY());
	if(z)
	{
		if(m_Unit->GetPositionZ() >= (z + 1.0f)) //not on ground? Oo
		{
			return true;
		}
	}
	return false;*/
	if( m_Unit->GetTypeId() == TYPEID_PLAYER )
		return static_cast< Player* >( m_Unit )->FlyCheat;

	return false;
}

uint32 AIInterface::getMoveFlags()
{
	uint32 MoveFlags = 0;
	if(m_moveFly == true) //Fly
	{
		m_flySpeed = m_Unit->m_flySpeed*0.001f;
		MoveFlags = MONSTER_MOVE_FLAG_FLY; // 0x3000 VLack: flight flag changed on 3.1.1
	}
	else if(m_moveSprint == true) //Sprint
	{
		m_runSpeed = (m_Unit->m_runSpeed+5.0f)*0.001f;
		MoveFlags = MONSTER_MOVE_FLAG_RUN; // 0x1000; //VLack: flight flag changed on 3.1.1
	}
	else if(m_moveRun == true) //Run
	{
		m_runSpeed = m_Unit->m_runSpeed*0.001f;
		MoveFlags = MONSTER_MOVE_FLAG_RUN; // 0x1000; //VLack: flight flag changed on 3.1.1
	}
/*	else //Walk
	{
		m_runSpeed = m_Unit->m_walkSpeed*0.001f;
		MoveFlags = 0x000;
	}*/
	m_walkSpeed = m_Unit->m_walkSpeed*0.001f;//move distance per ms time 
	return MoveFlags;
}

void AIInterface::UpdateMove()
{
	//this should NEVER be called directly !!!!!!
	//use MoveTo()
	float distance = m_Unit->CalcDistance(m_nextPosX,m_nextPosY,m_nextPosZ);
	
	if(distance < DISTANCE_TO_SMALL_TO_WALK) return; //we don't want little movements here and there

	m_destinationX = m_nextPosX;
	m_destinationY = m_nextPosY;
	m_destinationZ = m_nextPosZ;
	
	/*if(m_moveFly != true)
	{
		if(m_Unit->GetMapMgr())
		{
			float adt_Z = m_Unit->GetMapMgr()->GetLandHeight(m_destinationX, m_destinationY);
			if(fabsf(adt_Z - m_destinationZ) < 3.0f)
				m_destinationZ = adt_Z;
		}
	}*/
	m_nextPosX = m_nextPosY = m_nextPosZ = 0;

	uint32 moveTime = GetMoveTime(distance);

	m_totalMoveTime = moveTime;

	if(m_Unit->GetTypeId() == TYPEID_UNIT)
	{
		Creature *creature = static_cast<Creature*>(m_Unit);
		// check if we're returning to our respawn location. if so, reset back to default
		// orientation
		if(creature->GetSpawnX() == m_destinationX &&
			creature->GetSpawnY() == m_destinationY)
		{
			float o = creature->GetSpawnO();
			creature->SetOrientation(o);
		} else {
			// Calculate the angle to our next position

			float dx = (float)m_destinationX - m_Unit->GetPositionX();
			float dy = (float)m_destinationY - m_Unit->GetPositionY();
			if(dy != 0.0f)
			{
				float angle = atan2(dy, dx);
				m_Unit->SetOrientation(angle);
			}
		}
	}
	SendMoveToPacket(m_destinationX, m_destinationY, m_destinationZ, m_Unit->GetOrientation(), moveTime, getMoveFlags());

	m_timeToMove = moveTime;
	m_timeMoved = 0;
	if(m_moveTimer == 0)
	{
		m_moveTimer =  UNIT_MOVEMENT_INTERPOLATE_INTERVAL; // update every few msecs
	}

	m_creatureState = MOVING;
}

void AIInterface::SendCurrentMove(Player* plyr/*uint64 guid*/)
{
	if(m_destinationX == 0.0f && m_destinationY == 0.0f && m_destinationZ == 0.0f) return; //invalid move 

	uint32 numpoints = 4;
	float pointDiffX = m_sourceX - m_destinationX / (float)numpoints;
	float pointDiffY = m_sourceY - m_destinationY / (float)numpoints;
	float pointDiffZ = m_sourceZ - m_destinationZ / (float)numpoints;

	if(pointDiffX < 1.0f || pointDiffY < 1.0f)
	{
		SendMoveToPacket(m_destinationX, m_destinationY, m_destinationZ, 0.0f, 0, getMoveFlags());
		return;
	}

	ByteBuffer *splineBuf = new ByteBuffer(34*4);
	uint32 flags = 0;
	*splineBuf << uint32(flags); // spline flags
	/*
	else if(flags & 0x8000)
		; // 3 unk floats
	if(flags & 0x10000)
		; // unknown int64
	if(flags & 0x20000)
		; // unknown float*/
	*splineBuf << uint32(0); //dont know if this is an uint32 but client seems to be trying to read 4 - 24 + 8 + 4
	*splineBuf << uint32((m_totalMoveTime - m_timeToMove)+m_moveTimer); //Time Passed (start Position) //should be generated/save 
	*splineBuf << uint32(m_totalMoveTime); //Total Time //should be generated/save
	*splineBuf << m_sourceX << m_sourceY << m_sourceZ;
	*splineBuf << uint32(0);

	*splineBuf << uint32(numpoints); //Spline Count	// lets try this

	for(uint8 i = 0 ; i < numpoints; i++)
	{
		*splineBuf << m_sourceX + (pointDiffX * i);
		*splineBuf << m_sourceY + (pointDiffY * i);
		*splineBuf << m_sourceZ + (pointDiffZ * i);
	}
	/*
	*splineBuf << m_sourceX << m_sourceY << m_sourceZ;
	*splineBuf << m_Unit->GetPositionX() << m_Unit->GetPositionY() << m_Unit->GetPositionZ();
	*splineBuf << m_destinationX << m_destinationY << m_destinationZ;
	*splineBuf << m_destinationX << m_destinationY << m_destinationZ;
	*/
	*splineBuf << uint8(0); // Pguid?
	
	*splineBuf << m_destinationX << m_destinationY << m_destinationZ;	// 3 floats after all the spline points

	plyr->AddSplinePacket(m_Unit->GetGUID(), splineBuf);

	//This should only be called by Players AddInRangeObject() ONLY
	//using guid cuz when i atempted to use pointer the player was deleted when this event was called some times
	//PlayerPointer plyr = World::GetPlayer(guid);
	//if(!plyr) return;

	/*if(m_destinationX == 0.0f && m_destinationY == 0.0f && m_destinationZ == 0.0f) return; //invalid move 
	uint32 moveTime = m_timeToMove-m_timeMoved;
	//uint32 moveTime = (m_timeToMove-m_timeMoved)+m_moveTimer;
	WorldPacket data(50);
	data.SetOpcode( SMSG_MONSTER_MOVE );
	data << m_Unit->GetNewGUID();
	data << m_Unit->GetPositionX() << m_Unit->GetPositionY() << m_Unit->GetPositionZ();
	data << getMSTime();
	data << uint8(0);
	data << getMoveFlags();

	//float distance = m_Unit->CalcDistance(m_destinationX, m_destinationY, m_destinationZ);
	//uint32 moveTime = (uint32) (distance / m_runSpeed);

	data << moveTime;
	data << uint32(1); //Number of Waypoints
	data << m_destinationX << m_destinationY << m_destinationZ;
	plyr->GetSession()->SendPacket(&data);*/
/*	
	if(m_destinationX == 0.0f && m_destinationY == 0.0f && m_destinationZ == 0.0f) return; //invalid move 
	ByteBuffer *splineBuf = new ByteBuffer(16 + (12 * 4) + 12);
	*splineBuf << uint32(0); // spline flags
	*splineBuf << uint32((m_totalMoveTime - m_timeToMove)+m_moveTimer); //Time Passed (start Position) //should be generated/save 
	*splineBuf << uint32(m_totalMoveTime); //Total Time //should be generated/save
	*splineBuf << uint32(0);

	*splineBuf << uint32(4); //4 points
	
	*splineBuf << m_sourceX << m_sourceY << m_sourceZ;
	*splineBuf << m_Unit->GetPositionX() << m_Unit->GetPositionY() << m_Unit->GetPositionZ();
	*splineBuf << m_destinationX << m_destinationY << m_destinationZ + 0.1f; //stop client crashing on f_notequal_
	*splineBuf << m_destinationX << m_destinationY << m_destinationZ + 0.05f;

	*splineBuf << uint8(0); // Andy 3.0.8

	//destination	
	*splineBuf << m_destinationX << m_destinationY << m_destinationZ;
    
	plyr->AddSplinePacket(m_Unit->GetGUID(), splineBuf);

	//This should only be called by Players AddInRangeObject() ONLY
	//using guid cuz when i atempted to use pointer the player was deleted when this event was called some times
	//Player* plyr = World::GetPlayer(guid);
	//if(!plyr) return;

	/*if(m_destinationX == 0.0f && m_destinationY == 0.0f && m_destinationZ == 0.0f) return; //invalid move 
	uint32 moveTime = m_timeToMove-m_timeMoved;
	//uint32 moveTime = (m_timeToMove-m_timeMoved)+m_moveTimer;
	WorldPacket data(50);
	data.SetOpcode( SMSG_MONSTER_MOVE );
	data << m_Unit->GetNewGUID();
	data << m_Unit->GetPositionX() << m_Unit->GetPositionY() << m_Unit->GetPositionZ();
	data << getMSTime();
	data << uint8(0);
	data << getMoveFlags();

	//float distance = m_Unit->CalcDistance(m_destinationX, m_destinationY, m_destinationZ);
	//uint32 moveTime = (uint32) (distance / m_runSpeed);

	data << moveTime;
	data << uint32(1); //Number of Waypoints
	data << m_destinationX << m_destinationY << m_destinationZ;
	plyr->GetSession()->SendPacket(&data);*/

}

bool AIInterface::setInFront(Unit* target) // not the best way to do it, though
{
	//angle the object has to face
	float angle = m_Unit->calcAngle(m_Unit->GetPositionX(), m_Unit->GetPositionY(), target->GetPositionX(), target->GetPositionY() ); 
	//Change angle slowly 2000ms to turn 180 deg around
	if(angle > 180) angle += 90;
	else angle -= 90; //angle < 180
	m_Unit->getEasyAngle(angle);
	//Convert from degrees to radians (180 deg = PI rad)
	float orientation = angle / float(180 / M_PI);
	//Update Orentation Server Side
	m_Unit->SetPosition(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ(), orientation);
	
	return m_Unit->isInFront(target);
}

bool AIInterface::addWayPoint(WayPoint* wp)
{
	if(!m_waypoints) m_waypoints = new WayPointMap ;

	if(!wp) return false;

	if(wp->id <= 0) return false; //not valid id

	if(m_waypoints->size() <= wp->id) m_waypoints->resize(wp->id+1);

	if((*m_waypoints)[wp->id] == NULL)
	{
		(*m_waypoints)[wp->id] = wp;
		return true;
	}
	return false;
}

void AIInterface::changeWayPointID(uint32 oldwpid, uint32 newwpid)
{
	if(!m_waypoints)return;
	if(newwpid <= 0) 
		return; //not valid id
	if(newwpid > m_waypoints->size()) 
		return; //not valid id
	if(oldwpid > m_waypoints->size())
		return;

	if(newwpid == oldwpid) 
		return; //same spot

	//already wp with that id ?
	WayPoint* originalwp = getWayPoint(newwpid);
	if(!originalwp) 
		return;
	WayPoint* oldwp = getWayPoint(oldwpid);
	if(!oldwp) 
		return;

	oldwp->id = newwpid;
	originalwp->id = oldwpid;
	(*m_waypoints)[oldwp->id] = oldwp;
	(*m_waypoints)[originalwp->id] = originalwp;

	//SaveAll to db
	saveWayPoints();
}

void AIInterface::deleteWayPoint(uint32 wpid)
{
	if(!m_waypoints)return;
	if(wpid <= 0) 
		return; //not valid id
	if(wpid > m_waypoints->size()) 
		return; //not valid id

	WayPointMap new_waypoints;
	uint32 newpid = 1;
	for(WayPointMap::iterator itr = m_waypoints->begin(); itr != m_waypoints->end(); ++itr)
	{
		if((*itr) == NULL || (*itr)->id == wpid)
		{
			if((*itr) != NULL)
				delete ((*itr));

			continue;
		}

		new_waypoints.push_back(*itr);
	}

	m_waypoints->clear();
	m_waypoints->push_back(NULL);		// waypoint 0
	for(WayPointMap::iterator itr = new_waypoints.begin(); itr != new_waypoints.end(); ++itr)
	{
		(*itr)->id = newpid++;
		m_waypoints->push_back(*itr);
	}

	saveWayPoints();
}

bool AIInterface::showWayPoints(Player* pPlayer, bool Backwards)
{
	if(!m_waypoints)
		return false;

	//wpid of 0 == all
	WayPointMap::const_iterator itr;
	if(m_WayPointsShowing == true) 
		return false;

	m_WayPointsShowing = true;

	WayPoint* wp = NULL;
	for (itr = m_waypoints->begin(); itr != m_waypoints->end(); itr++)
	{
		if( (*itr) != NULL )
		{
			wp = *itr;

			//Create
			Creature* pWayPoint = new Creature((uint64)HIGHGUID_TYPE_WAYPOINT << 32 | wp->id);
			pWayPoint->CreateWayPoint(wp->id,pPlayer->GetMapId(),wp->x,wp->y,wp->z,0);
			pWayPoint->SetUInt32Value(OBJECT_FIELD_ENTRY, 300000);
			pWayPoint->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
			if(Backwards)
			{
				uint32 DisplayID = (wp->backwardskinid == 0)? GetUnit()->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID) : wp->backwardskinid;
				pWayPoint->SetUInt32Value(UNIT_FIELD_DISPLAYID, DisplayID);
				pWayPoint->SetUInt32Value(UNIT_NPC_EMOTESTATE, wp->backwardemoteid);
			}
			else
			{
				uint32 DisplayID = (wp->forwardskinid == 0)? GetUnit()->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID) : wp->forwardskinid;
				pWayPoint->SetUInt32Value(UNIT_FIELD_DISPLAYID, DisplayID);
				pWayPoint->SetUInt32Value(UNIT_NPC_EMOTESTATE, wp->forwardemoteid);
			}
			pWayPoint->SetUInt32Value(UNIT_FIELD_LEVEL, wp->id);
			pWayPoint->SetUInt32Value(UNIT_NPC_FLAGS, 0);
			//pWayPoint->SetUInt32Value(UNIT_FIELD_AURA+32, 8326); //invisable & deathworld look
			pWayPoint->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE , pPlayer->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
			pWayPoint->SetUInt32Value(UNIT_FIELD_HEALTH, 1);
			pWayPoint->SetUInt32Value(UNIT_FIELD_MAXHEALTH, 1);
			pWayPoint->SetUInt32Value(UNIT_FIELD_STAT0, wp->flags);

			//Create on client
			ByteBuffer buf(3000);
			uint32 count = pWayPoint->BuildCreateUpdateBlockForPlayer(&buf, pPlayer);
			pPlayer->PushCreationData(&buf, count);

			//root the object
			WorldPacket data1;
			data1.Initialize(SMSG_FORCE_MOVE_ROOT);
			data1 << pWayPoint->GetNewGUID();
			pPlayer->GetSession()->SendPacket( &data1 );

			//Cleanup
			delete pWayPoint;
		}
	}
	return true;
}

bool AIInterface::hideWayPoints(Player* pPlayer)
{
	if(!m_waypoints)
		return false;

	//wpid of 0 == all
	if(m_WayPointsShowing != true) return false;
	m_WayPointsShowing = false;
	WayPointMap::const_iterator itr;

	// slightly better way to do this
	uint64 guid;

	for (itr = m_waypoints->begin(); itr != m_waypoints->end(); itr++)
	{
		if( (*itr) != NULL )
		{
			// avoid C4293
			guid = ((uint64)HIGHGUID_TYPE_WAYPOINT << 32) | (*itr)->id;
			WoWGuid wowguid(guid);
			pPlayer->PushOutOfRange(wowguid);
		}
	}
	return true;
}

bool AIInterface::saveWayPoints()
{
	uint32 nb_way=0;

	if(!GetUnit()) 
	{
		Log.Warning("AIInterface::saveWayPoints","Pas de Pnj definie!");
		return false;
	}

	if(!m_waypoints) 
	{
		Log.Warning("AIInterface::saveWayPoints","Pas de waypoint definie! (Entry: %u)",this->GetUnit()->GetEntry());
		return false;
	}


	if(GetUnit()->GetTypeId() != TYPEID_UNIT) 
	{
        Log.Warning("AIInterface::saveWayPoints","La selection n'est pas un PNJ/Creature!");
		return false;
	}

	WorldDatabase.Execute("DELETE FROM creature_waypoints WHERE spawnid = %u", ((Creature*)GetUnit())->GetSQL_id());
	WayPointMap::const_iterator itr;
	WayPoint* wp = NULL;
	std::stringstream ss;

	for (itr = m_waypoints->begin(); itr != m_waypoints->end(); itr++)
	{
		if((*itr) == NULL) continue;

        nb_way++;

		wp = (*itr);

		//Save
		ss.str("");
		ss << "INSERT INTO creature_waypoints ";
		ss << "(spawnid,waypointid,position_x,position_y,position_z,waittime,flags,forwardemoteoneshot,forwardemoteid,backwardemoteoneshot,backwardemoteid,forwardskinid,backwardskinid) VALUES (";
		ss << ((Creature*)GetUnit())->GetSQL_id() << ", ";
		ss << wp->id << ", ";
		ss << wp->x << ", ";
		ss << wp->y << ", ";
		ss << wp->z << ", ";
		ss << wp->waittime << ", ";
		ss << wp->flags << ", ";
		ss << wp->forwardemoteoneshot << ", ";
		ss << wp->forwardemoteid << ", ";
		ss << wp->backwardemoteoneshot << ", ";
		ss << wp->backwardemoteid << ", ";
		ss << wp->forwardskinid << ", ";
		ss << wp->backwardskinid << ")\0";
		WorldDatabase.Query( ss.str().c_str() );
	}
	Log.Notice("AIInterface::saveWayPoints","%u Waypoints sauvegardes (Entry: %u, SQLEntry %u)",nb_way,this->GetUnit()->GetEntry(),((Creature*)GetUnit())->GetSQL_id());
	return true;
}

void AIInterface::deleteWaypoints()
{
	if(!m_waypoints)
		return;

	for(WayPointMap::iterator itr = m_waypoints->begin(); itr != m_waypoints->end(); ++itr)
	{
		if((*itr) != NULL)
			delete (*itr);
	}
	m_waypoints->clear();
	delete m_waypoints;
	m_waypoints = NULL;	
}

WayPoint* AIInterface::getWayPoint(uint32 wpid)
{
	if(!m_waypoints)return NULL;
	if(wpid >= m_waypoints->size()) 
		return NULL; //not valid id

	/*WayPointMap::const_iterator itr = m_waypoints->find( wpid );
	if( itr != m_waypoints->end( ) )
		return itr->second;*/
	return m_waypoints->at(wpid);
}

void AIInterface::_UpdateMovement(uint32 p_time)
{
	float TailleCreature; 

	if(!m_Unit->isAlive() || m_Unit->GetCurrentSpell() != NULL)
	{
		StopMovement(0);
		return;
	}

	uint32 timediff = 0;

	if (m_moveDelayTimer > 0)
	{
		if (p_time >= m_moveDelayTimer)
			m_moveDelayTimer = 0;
		else
		{
			m_moveDelayTimer -= p_time;
			return;
		}
	}
	
	if(m_moveTimer > 0)
	{
		if(p_time >= m_moveTimer)
		{
			timediff = p_time - m_moveTimer;
			m_moveTimer = 0;
		}
		else
			m_moveTimer -= p_time;
	}

	if(m_timeToMove > 0)
	{
		m_timeMoved = m_timeToMove <= p_time + m_timeMoved ? m_timeToMove : p_time + m_timeMoved;
	}

	TailleCreature = m_Unit->GetSize();

	if(m_creatureState == MOVING)
	{
		if(!m_moveTimer)
		{
#ifdef STICKY_NPC_MOVEMENT
			if (m_timeMoved == m_timeToMove || m_Unit->CalcDistance(m_destinationX, m_destinationY, m_Unit->GetPositionZ()) == 0) //use unit's position X so its 2d
#else
 			if(m_timeMoved == m_timeToMove) //reached destination
#endif
			{
/*				if(m_fastMove)
				{
					m_Unit->UpdateSpeed();
					m_fastMove = false;
				}*/

				if(m_moveType == MOVEMENTTYPE_WANTEDWP)//We reached wanted wp stop now
					m_moveType = MOVEMENTTYPE_DONTMOVEWP;

				float wayO = 0.0f;

				if((GetWayPointsCount() != 0) && (m_AIState == STATE_IDLE || m_AIState == STATE_SCRIPTMOVE)) //if we attacking don't use wps
				{
					WayPoint* wp = getWayPoint(getCurrentWaypoint());
					if(wp)
					{
						CALL_SCRIPT_EVENT(m_Unit, OnReachWP)(wp->id, !m_moveBackward);
						if(((Creature*)m_Unit)->has_waypoint_text)
							objmgr.HandleMonsterSayEvent(((Creature*)m_Unit), MONSTER_SAY_EVENT_RANDOM_WAYPOINT);

						//Lets face to correct orientation
						wayO = wp->o;
						m_moveTimer = wp->waittime; //wait before next move
						if(!m_moveBackward)
						{
							if(wp->forwardemoteoneshot)
							{
								GetUnit()->Emote(EmoteType(wp->forwardemoteid));
							}
							else
							{
								if(GetUnit()->GetUInt32Value(UNIT_NPC_EMOTESTATE) != wp->forwardemoteid)
								{
									GetUnit()->SetUInt32Value(UNIT_NPC_EMOTESTATE, wp->forwardemoteid);
								}
							}
						}
						else
						{
							if(wp->backwardemoteoneshot)
							{
								GetUnit()->Emote(EmoteType(wp->backwardemoteid));
							}
							else
							{
								if(GetUnit()->GetUInt32Value(UNIT_NPC_EMOTESTATE) != wp->backwardemoteid)
								{
									GetUnit()->SetUInt32Value(UNIT_NPC_EMOTESTATE, wp->backwardemoteid);
								}
							}
						}
					}
					else
						m_moveTimer = RandomUInt(m_moveRun ? 5000 : 10000); // wait before next move
				}

				m_creatureState = STOPPED;
				m_moveSprint = false;

				if(m_MovementType == MOVEMENTTYPE_DONTMOVEWP)
					m_Unit->SetPosition(m_destinationX, m_destinationY, m_destinationZ, wayO, true);
				else
					m_Unit->SetPosition(m_destinationX, m_destinationY, m_destinationZ, m_Unit->GetOrientation(), true);

				m_destinationX = m_destinationY = m_destinationZ = 0;
				m_timeMoved = 0;
				m_timeToMove = 0;
			}
			else
			{
				//Move Server Side Update
				float q = (float)m_timeMoved / (float)m_timeToMove;
				float x = m_Unit->GetPositionX() + (m_destinationX - m_Unit->GetPositionX()) * q;
				float y = m_Unit->GetPositionY() + (m_destinationY - m_Unit->GetPositionY()) * q;
				float z = m_Unit->GetPositionZ() + (m_destinationZ - m_Unit->GetPositionZ()) * q;

#ifdef STICKY_NPC_MOVEMENT
				//lets try and fix Z :P
				float land_h = CollideInterface.GetHeight(m_Unit->GetMapId(), x, y, z + 2.0f);
				if (land_h == NO_WMO_HEIGHT)
					land_h = m_Unit->GetMapMgr()->GetLandHeight(x, y);
				if (land_h != NO_WMO_HEIGHT && land_h != 999999.0f)
				{
					uint32 time = GetMoveTime(m_Unit->CalcDistance(x, y, land_h));
					m_timeMoved = time;
					z = land_h;

					//delay movement by new distance :)
					if (m_timeMoved < time) //really wtf would cause it to be bigger?
					{
						m_moveDelayTimer = time - m_timeMoved;
						m_timeToMove += m_moveDelayTimer;
					}
				}
#endif				
				/*if(m_moveFly != true)
				{
					if(m_Unit->GetMapMgr())
					{
						float adt_Z = m_Unit->GetMapMgr()->GetLandHeight(x, y);
						if(fabsf(adt_Z - z) < 1.5)
							z = adt_Z;
					}
				}*/

				if (m_Unit->IsPlayer())
				{
					float dx = (float)m_destinationX - m_Unit->GetPositionX();
					float dy = (float)m_destinationY - m_Unit->GetPositionY();
					if(dy != 0.0f)
					{
						float angle = atan2(dy, dx);
						m_Unit->SetOrientation(angle);
					}
				}
				
				m_Unit->SetPosition(x, y, z, m_Unit->GetOrientation());
				
				m_timeToMove -= m_timeMoved;
				m_timeMoved = 0;
				m_moveTimer = (UNIT_MOVEMENT_INTERPOLATE_INTERVAL < m_timeToMove) ? UNIT_MOVEMENT_INTERPOLATE_INTERVAL : m_timeToMove;
			}
			//**** Movement related stuff that should be done after a move update (Keeps Client and Server Synced) ****//
			//**** Process the Pending Move ****//
			if( (m_nextPosX != 0.0f) && (m_nextPosY != 0.0f) )
			{
				UpdateMove();
			}
		}
	}
	else if(m_creatureState == STOPPED && (m_AIState == STATE_IDLE || m_AIState == STATE_SCRIPTMOVE) 
		   && !m_moveTimer && !m_timeToMove && UnitToFollow == NULL) //creature is stopped and out of Combat
	{
		if(sWorld.getAllowMovement() == false) //is creature movement enabled?
			return;

		if(m_Unit->GetUInt32Value(UNIT_FIELD_DISPLAYID) == 5233) //if Spirit Healer don't move
			return;

		// do we have a formation?
		if(m_formationLinkSqlId != 0)
		{
			if(!m_formationLinkTarget)
			{
				// haven't found our target yet
				Creature * c = static_cast<Creature*>(m_Unit);
				if(!c->haslinkupevent)
				{
					// register linkup event
					c->haslinkupevent = true;
					sEventMgr.AddEvent(c, &Creature::FormationLinkUp, m_formationLinkSqlId, 
						EVENT_CREATURE_FORMATION_LINKUP, 1000, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				}
			}
			else
			{
				// we've got a formation target, set unittofollow to this
				UnitToFollow = m_formationLinkTarget;
				FollowDistance = m_formationFollowDistance;
				m_fallowAngle = m_formationFollowAngle;
			}
		}
		if(UnitToFollow == 0)
		{
			// no formation, use waypoints
			int destpoint = -1;

			// If creature has no waypoints just wander aimlessly around spawnpoint
			if(GetWayPointsCount()==0) //no waypoints
			{
				/*	if(m_moveRandom)
				{
				if((rand()%10)==0)																																	
				{																																								  
				float wanderDistance = rand()%4 + 2;
				float wanderX = ((wanderDistance*rand()) / RAND_MAX) - wanderDistance / 2;																											   
				float wanderY = ((wanderDistance*rand()) / RAND_MAX) - wanderDistance / 2;																											   
				float wanderZ = 0; // FIX ME ( i dont know how to get apropriate Z coord, maybe use client height map data)																											 

				if(m_Unit->CalcDistance(m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ(), ((Creature*)m_Unit)->respawn_cord[0], ((Creature*)m_Unit)->respawn_cord[1], ((Creature*)m_Unit)->respawn_cord[2])>15)																		   
				{   
				//return home																																				 
				MoveTo(((Creature*)m_Unit)->respawn_cord[0],((Creature*)m_Unit)->respawn_cord[1],((Creature*)m_Unit)->respawn_cord[2],false);
				}   
				else
				{
				MoveTo(m_Unit->GetPositionX() + wanderX, m_Unit->GetPositionY() + wanderY, m_Unit->GetPositionZ() + wanderZ,false);
				}	
				}	
				}
				*/	
				return;																																				   
			}																																						  
			else //we do have waypoints
			{
				if(m_moveType == MOVEMENTTYPE_RANDOMWP) //is random move on if so move to a random waypoint
				{
					if(GetWayPointsCount() > 1)
						destpoint = RandomUInt((uint32)GetWayPointsCount());
				}
				else if (m_moveType == MOVEMENTTYPE_CIRCLEWP) //random move is not on lets follow the path in circles
				{
					// 1 -> 10 then 1 -> 10
					m_currentWaypoint++;
					if (m_currentWaypoint > GetWayPointsCount()) m_currentWaypoint = 1; //Happens when you delete last wp seems to continue ticking
					destpoint = m_currentWaypoint;
					m_moveBackward = false;
				}
				else if(m_moveType == MOVEMENTTYPE_WANTEDWP)//Move to wanted wp
				{
					if(m_currentWaypoint)
					{
						if(GetWayPointsCount() > 0)
						{
							destpoint = m_currentWaypoint;
						}
						else
							destpoint = -1;
					}
				}
				else if(m_moveType == MOVEMENTTYPE_FORWARDTHANSTOP)// move to end, then stop
				{
					++m_currentWaypoint;
					if(m_currentWaypoint > GetWayPointsCount())
					{
						//hmm maybe we should stop being path walker since we are waiting here anyway
						destpoint = -1;
					}
					else
						destpoint = m_currentWaypoint;
				}
				else if(m_moveType != MOVEMENTTYPE_QUEST && m_moveType != MOVEMENTTYPE_DONTMOVEWP)//4 Unused
				{
					// 1 -> 10 then 10 -> 1
					if (m_currentWaypoint > GetWayPointsCount()) m_currentWaypoint = 1; //Happens when you delete last wp seems to continue ticking
					if (m_currentWaypoint == GetWayPointsCount()) // Are we on the last waypoint? if so walk back
						m_moveBackward = true;
					if (m_currentWaypoint == 1) // Are we on the first waypoint? if so lets goto the second waypoint
						m_moveBackward = false;
					if (!m_moveBackward) // going 1..n
						destpoint = ++m_currentWaypoint;
					else				// going n..1
						destpoint = --m_currentWaypoint;
				}

				if(destpoint != -1)
				{
					WayPoint* wp = getWayPoint(destpoint);
					if(wp)
					{
						if(!m_moveBackward)
						{
							if((wp->forwardskinid != 0) && (GetUnit()->GetUInt32Value(UNIT_FIELD_DISPLAYID) != wp->forwardskinid))
							{
								GetUnit()->SetUInt32Value(UNIT_FIELD_DISPLAYID, wp->forwardskinid);
							}
						}
						else
						{
							if((wp->backwardskinid != 0) && (GetUnit()->GetUInt32Value(UNIT_FIELD_DISPLAYID) != wp->backwardskinid))
							{
								GetUnit()->SetUInt32Value(UNIT_FIELD_DISPLAYID, wp->backwardskinid);
							}
						}
						m_moveFly = (wp->flags == 768) ? 1 : 0;
						m_moveRun = (wp->flags == 256) ? 1 : 0;
						MoveTo(wp->x, wp->y, wp->z, 0);
					}
				}
			}
		}
	}

	//Fear Code
	if(m_AIState == STATE_FEAR && UnitToFear != NULL && m_creatureState == STOPPED)
	{
		if(getMSTime() > m_FearTimer)   // Wait at point for x ms ;)
		{
			float Fx;
			float Fy;
			float Fz;
			// Calculate new angle to target.
			float Fo = m_Unit->calcRadAngle(UnitToFear->GetPositionX(), UnitToFear->GetPositionY(), m_Unit->GetPositionX(), m_Unit->GetPositionY());
			double fAngleAdd = RandomDouble(((M_PI/2) * 2)) - (M_PI/2);
			Fo += (float)fAngleAdd;
			
			float dist = m_Unit->CalcDistance(UnitToFear);
			if(dist > 30.0f || (Rand(25) && dist > 10.0f))	// not too far or too close
			{
				Fx = m_Unit->GetPositionX() - (RandomFloat(15.f)+5.0f)*cosf(Fo);
				Fy = m_Unit->GetPositionY() - (RandomFloat(15.f)+5.0f)*sinf(Fo);
			}
			else
			{
				Fx = m_Unit->GetPositionX() + (RandomFloat(20.f)+5.0f)*cosf(Fo);
				Fy = m_Unit->GetPositionY() + (RandomFloat(20.f)+5.0f)*sinf(Fo);
			}
			

#ifdef COLLISION
			MapInfo* mapinfo = WorldMapInfoStorage.LookupEntry(m_Unit->GetMapId());
			if (mapinfo != NULL && mapinfo->collision != 0)
			{
				Fz = CollideInterface.GetHeight(m_Unit->GetMapId(), Fx, Fy, m_Unit->GetPositionZ()); //2.0f);

				if( Fz == NO_WMO_HEIGHT ) Fz = m_Unit->GetMapMgr()->GetLandHeight(Fx, Fy, m_Unit->GetPositionZ());

				/* OBSOLETE (Branruz)
				else				
				{
					if( CollideInterface.GetFirstPoint(m_Unit->GetMapId(), 
						m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ() + TailleCreature,
						Fx, Fy, Fz + TailleCreature, Fx, Fy, Fz, -1.0f) )
					{
						//Fz = CollideInterface.GetHeight(m_Unit->GetMapId(), Fx, Fy, m_Unit->GetPositionZ() + TailleCreature); // 2.0f);
					}				
				}
				*/
			}
			else 
#endif
			// Check if this point is in water.
			// float wl = m_Unit->GetMapMgr()->GetWaterHeight(Fx, Fy);
//			uint8 wt = m_Unit->GetMapMgr()->GetWaterType(Fx, Fy);
			//Fz = m_Unit->GetPositionZ();
			Fz = m_Unit->GetMapMgr()->GetLandHeight(Fx, Fy,m_Unit->GetPositionZ()); // + wl sauf lave 
			
#ifdef COLLISION
			if (CollideInterface.CheckLOS(m_Unit->GetMapId(), 
				                           m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ()+TailleCreature, 
				                           Fx, Fy, Fz + TailleCreature))
#endif
			{
				MoveTo(Fx, Fy, Fz, Fo);
				m_FearTimer = m_totalMoveTime + getMSTime() + 400;
			}


		}
	}

	// Wander AI movement code
	if(m_AIState == STATE_WANDER && m_creatureState == STOPPED)
	{
		if(getMSTime() < m_WanderTimer) // is it time to move again?
			return;

		// calculate a random distance and angle to move
		float wanderD = RandomFloat(2.0f) + 2.0f;
		float wanderO = RandomFloat(6.283f);
		float wanderX = m_Unit->GetPositionX() + wanderD * cosf(wanderO);
		float wanderY = m_Unit->GetPositionY() + wanderD * sinf(wanderO);
		float wanderZ = m_Unit->GetMapMgr()->GetLandHeight(wanderX, wanderY,m_Unit->GetPositionZ()); // m_Unit->GetPositionZ();

		MapInfo* mapinfo = WorldMapInfoStorage.LookupEntry(m_Unit->GetMapId());
		if (mapinfo != NULL && mapinfo->collision != 0)
		{
			//wanderZ = CollideInterface.GetHeight(m_Unit->GetMapId(), wanderX, wanderY, m_Unit->GetPositionZ() + 2.0f);
			float wanderZ2 = wanderZ;
			if( wanderZ == NO_WMO_HEIGHT )
				wanderZ = m_Unit->GetMapMgr()->GetLandHeight(wanderX, wanderY,m_Unit->GetPositionZ());
			else
			{
				/*
				if( CollideInterface.GetFirstPoint(m_Unit->GetMapId(), m_Unit->GetPositionX(), m_Unit->GetPositionY(), m_Unit->GetPositionZ() + 2.0f,
					wanderX, wanderY, wanderZ + 2.0f, wanderX, wanderY, wanderZ, -1.0f) )
				{
					wanderZ = CollideInterface.GetHeight(m_Unit->GetMapId(), wanderX, wanderY, m_Unit->GetPositionZ() + 2.0f);
				}
				else*/
					wanderZ = wanderZ2;
			}
		}
		if( fabs( m_Unit->GetPositionZ() - wanderZ ) > 10.0f )
		{
			m_WanderTimer=getMSTime() + 1000;
		}
		else
		{
			m_Unit->SetOrientation(wanderO);
			MoveTo(wanderX, wanderY, wanderZ, wanderO);
			m_WanderTimer = getMSTime() + m_totalMoveTime + 300; // time till next move (+ pause)

		}
	}

	//Unit Follow Code
	if(UnitToFollow != NULL)
	{
		if( UnitToFollow->event_GetCurrentInstanceId() != m_Unit->event_GetCurrentInstanceId() || !UnitToFollow->IsInWorld() )
			UnitToFollow = NULL;
		else
		{
				if(m_AIState == STATE_IDLE || m_AIState == STATE_FOLLOWING)
				{
					float dist = m_Unit->GetDistanceSq(UnitToFollow);

					// re-calculate orientation based on target's movement
					if(m_lastFollowX != UnitToFollow->GetPositionX() ||
						m_lastFollowY != UnitToFollow->GetPositionY())
					{
						float dx = UnitToFollow->GetPositionX() - m_Unit->GetPositionX();
						float dy = UnitToFollow->GetPositionY() - m_Unit->GetPositionY();
						if(dy != 0.0f)
						{
							float angle = atan2(dx,dy);
							m_Unit->SetOrientation(angle);
						}
						m_lastFollowX = UnitToFollow->GetPositionX();
						m_lastFollowY = UnitToFollow->GetPositionY();
					}

					if (dist > (FollowDistance*FollowDistance)) //if out of range
					{
						m_AIState = STATE_FOLLOWING;
						
						if(dist > 25.0f) //25 yard away lets run else we will loose the them
							m_moveRun = true;
						else 
							m_moveRun = false;

						if(m_AIType == AITYPE_PET || UnitToFollow == m_formationLinkTarget) //Unit is Pet/formation
						{
							if(dist > 900.0f/*30*/)
								m_moveSprint = true;

							float delta_x = UnitToFollow->GetPositionX();
							float delta_y = UnitToFollow->GetPositionY();
							float d = 3;
							if(m_formationLinkTarget)
								d = m_formationFollowDistance;

							MoveTo(delta_x+(d*(cosf(m_fallowAngle+UnitToFollow->GetOrientation()))),
								delta_y+(d*(sinf(m_fallowAngle+UnitToFollow->GetOrientation()))),
								UnitToFollow->GetPositionZ(),UnitToFollow->GetOrientation());				
						}
						else
						{
							_CalcDestinationAndMove(UnitToFollow, FollowDistance);
						}
					}
				}
			}
	}
}

void AIInterface::CastSpell(Unit* caster, SpellEntry *spellInfo, SpellCastTargets targets)
{
	if( (m_AIType != AITYPE_PET) && disable_spell ) return;

	// Stop movement while casting.
	m_AIState = STATE_CASTING;

#ifdef _AI_SPELL_DEBUG
	Log.Notice("AIInterface","Unite %u jette le sort %s", caster->GetEntry(),spellInfo->Name);
#endif

	//i wonder if this will lead to a memory leak :S
	Spell *nspell = new Spell(caster, spellInfo, false, NULL);
	nspell->prepare(&targets);
}

SpellEntry *AIInterface::getSpellEntry(uint32 spellId)
{
	SpellEntry *spellInfo = dbcSpell.LookupEntry(spellId );

	if(!spellInfo)
	{
		sLog.outError("WORLD: sort inconnu id %i\n", spellId);
		return NULL;
	}

	return spellInfo;
}
// La corection du 'z' est faite par cette fonction et ne doit pas etre fait
// ailleurs sous peine de disfonctionnement des spells à distance. (Branruz)
SpellCastTargets AIInterface::setSpellTargets(SpellEntry *spellInfo, Unit* target)
{
	SpellCastTargets targets;
	targets.m_target = target;
	targets.m_srcX = 0;
	targets.m_srcY = 0;
	targets.m_srcZ = 0;
	targets.m_destX = 0;
	targets.m_destY = 0;
	targets.m_destZ = 0;

	if(m_nextSpell->spelltargetType == TTYPE_SINGLETARGET)
	{
		targets.m_targetMask = TARGET_FLAG_UNIT; // 2;
		targets.m_target = target;
	}
	else if(m_nextSpell->spelltargetType == TTYPE_SOURCE)
	{
		targets.m_targetMask = TARGET_FLAG_SOURCE_LOCATION; // 32;
		targets.m_srcX = m_Unit->GetPositionX();
		targets.m_srcY = m_Unit->GetPositionY();
		targets.m_srcZ = m_Unit->GetPositionZ() + m_Unit->GetSize(); 
	}
	else if(m_nextSpell->spelltargetType == TTYPE_DESTINATION) 
	{
		targets.m_target = target;
		targets.m_targetMask = TARGET_FLAG_DEST_LOCATION; // 64;
		targets.m_destX = target->GetPositionX();
		targets.m_destY = target->GetPositionY();
		targets.m_destZ = target->GetPositionZ() + target->GetSize();
	}
	else if(m_nextSpell->spelltargetType == TTYPE_HIMSELF) // TTYPE_CASTER)
	{
		targets.m_targetMask = TARGET_FLAG_UNIT; //2;
		targets.m_target = m_Unit;
	}

	return targets;
}

AI_Spell *AIInterface::getSpell()
{
	// pas encore le moment de lancer le prochain sort 
	if(next_spell_time > (uint32)UNIXTIME) 
	{
		//sLog.outString("AI DEBUG (%u) : Pas encore le moment de lancer le prochain sort", m_Unit->GetEntry());
		return NULL;
	}

#ifdef _AI_SPELL_DEBUG    
	Log.Notice("AIAgent", "GetSpell() begin.");
#endif

	waiting_for_cooldown = false;

	// look at our spells
	AI_Spell *  sp = NULL;
	AI_Spell *  def_spell = NULL;
	// AI_Spell *  dist_spell = NULL; // Note Randdrick : Test si la cible est a portée de lancement du spell
	uint32 CurVal = 0;	
	bool MiscEvent3 = false; // Note Randdrick : Initialise le troisième Miscaellenous Event
//	uint32 cool_time;
	uint32 nowtime = getMSTime(); // Nombre de msec depuis le demarrage de windows.... UNIXTIME = DateTime depuis 01/01/1970
	
	if(m_Unit->IsPet()) // si mob Pet
	{
		def_spell = ((Pet*)m_Unit)->HandleAutoCastEvent();

		// Pas d'autocast, seulement un compteur
		if(def_spell == NULL) 
		{
			uint32 CountAISpell = ((Pet*)m_Unit)->GetCurrentBarAISpellsCount();
			if(CountAISpell) 
			{
			 ((Pet*)m_Unit)->SetCurrentBarAISpellsCount(--CountAISpell);
			 def_spell = ((Pet*)m_Unit)->GetCurrentBarAISpells();
			}
		}


		//sLog.outString("AI DEBUG (%u) : Mob est un Pet, Spell en cours: %u", m_Unit->GetEntry(),(def_spell)?(def_spell->spell->Id):(0));

		waiting_for_cooldown = false;
#ifdef _AI_SPELL_DEBUG    
	Log.Notice("AIAgent", "GetSpell() IsPet!, End.");
#endif
		return def_spell;

	} 
	else 
	{ // si pas Pet

			//Log.Notice("AIAgent", "Entree dans l'iterator");
		    if(npc_spells.size() == 0) 
			{
#ifdef _AI_SPELL_DEBUG    
	            Log.Notice("AIAgent", "GetSpell() Pas de Spell!, End.");
#endif
				return(NULL);
			}
			
            uint32 Result = RandomUInt((uint32)npc_spells.size()-1);
			sp = npc_spells.at(Result);  // Nouvelle gestion (Papyjo)
			if(sp)
			{
#ifdef _AI_SPELL_DEBUG            
				Log.Notice("AIAgent", "Spell aleatoire %u ( %u, %u au total).",sp->spell->Id,Result+1,npc_spells.size());
#endif
			 // si le cooldown est pas fini
			 if ( nowtime < sp->cooldowntime )
			 {
				waiting_for_cooldown = true;
#ifdef _AI_SPELL_DEBUG         
				Log.Notice("AIAgent", "CoolDownTime: %s.", ConvertMSTimeToString(sp->cooldowntime - nowtime).c_str());
#endif
				return 0;
			 }
			 /* ****** Gestion des Miscellenous Events par Randdrick **** */
			 // Remise à 0 du procCount et procChance principal au cas ou, si un MiscEvent existe.
			 if(sp->MiscEvent1 || sp->MiscEvent2 || sp->MiscEvent3 )
			 {
				 sp->procCount =0;
				 sp->procChance = 0;
			 }
			 
			 // Gestion du HEAL - ACTION_TYPE_HEAL
			if (sp->MiscEvent1 == ACTION_TYPE_HEAL || sp->MiscEvent2 == ACTION_TYPE_HEAL || sp->MiscEvent3 == ACTION_TYPE_HEAL)
				CurVal = m_Unit->GetUInt32Value(UNIT_FIELD_HEALTH);

			// Gestion du MANA - ACTION_TYPE_MANA
			if (sp->MiscEvent1 == ACTION_TYPE_MANA || sp->MiscEvent2 == ACTION_TYPE_MANA || sp->MiscEvent3 == ACTION_TYPE_MANA)
				CurVal = m_Unit->GetUInt32Value(UNIT_FIELD_POWER1);

			// Gestion de la RAGE - ACTION_TYPE_RAGE
			if (sp->MiscEvent1 == ACTION_TYPE_RAGE || sp->MiscEvent2 == ACTION_TYPE_RAGE || sp->MiscEvent3 == ACTION_TYPE_RAGE)
				CurVal = m_Unit->GetUInt32Value(UNIT_FIELD_POWER2);

			// Gestion du FOCUS - ACTION_TYPE_FOCUS
			if (sp->MiscEvent1 == ACTION_TYPE_FOCUS || sp->MiscEvent2 == ACTION_TYPE_FOCUS || sp->MiscEvent3 == ACTION_TYPE_FOCUS)
				CurVal = m_Unit->GetUInt32Value(UNIT_FIELD_POWER3);
			
			// Gestion de l'ENERGIE - ACTION_TYPE_ENERGIE
			if (sp->MiscEvent1 == ACTION_TYPE_ENERGIE || sp->MiscEvent2 == ACTION_TYPE_ENERGIE || sp->MiscEvent3 == ACTION_TYPE_ENERGIE)
				CurVal = m_Unit->GetUInt32Value(UNIT_FIELD_POWER4);


			// Gestion du PET_HAPINESS - ACTION_TYPE_PET_HAPINESS
			if (sp->MiscEvent1 == ACTION_TYPE_PET_HAPINESS || sp->MiscEvent2 == ACTION_TYPE_PET_HAPINESS || sp->MiscEvent3 == ACTION_TYPE_PET_HAPINESS)
				CurVal = m_Unit->GetUInt32Value(UNIT_FIELD_POWER5);

			// Gestion du POWER6 - ACTION_TYPE_POWER6
			if (sp->MiscEvent1 == ACTION_TYPE_POWER6 || sp->MiscEvent2 == ACTION_TYPE_POWER6 || sp->MiscEvent3 == ACTION_TYPE_POWER6)
				CurVal = m_Unit->GetUInt32Value(UNIT_FIELD_POWER6);

			// Gestion de la Puissance Runic  - ACTION_TYPE_RUNICPOWER
			if (sp->MiscEvent1 == ACTION_TYPE_RUNICPOWER || sp->MiscEvent2 == ACTION_TYPE_RUNICPOWER || sp->MiscEvent3 == ACTION_TYPE_RUNICPOWER)
				CurVal = m_Unit->GetUInt32Value(UNIT_FIELD_POWER7);

			if(sp->MiscEvent1)
			{
				uint32 ValMiscEvent = sp->MiscEvent1;
				switch (ValMiscEvent)
				{
					case ACTION_TYPE_HEAL:
					case ACTION_TYPE_MANA:
					case ACTION_TYPE_FOCUS:					
					case ACTION_TYPE_ENERGIE:
					case ACTION_TYPE_PET_HAPINESS:
						{
							bool MiscEvent1 = MiscEvent;		
							if (sp->MiscEvent1_Val >= CurVal && MiscEvent1 != false) 
							{
								sp->procCount = sp->MiscEvent1_MaxCount; // On override le proCount existant
								sp->procChance = 100; // La chance est alors à 100 % puisque la condition est remplie. Cela permet une gestion BlizLike
							}
							else 
							{
								MiscEvent= true; // permet de lancer le deuxième event à la fin du premier
								sp->MiscEvent1_MaxCount = -1; // On ne peut plus lancer cet event puisque celui-ci est "épuisé"
							}
						}break;
					case ACTION_TYPE_RAGE:
					case ACTION_TYPE_POWER6:
					case ACTION_TYPE_RUNICPOWER:
						{
							bool MiscEvent1 = MiscEvent;		
							if (sp->MiscEvent1_Val <= CurVal && MiscEvent1 != false) 
							{
								sp->procCount = sp->MiscEvent1_MaxCount; // On override le proCount existant
								sp->procChance = 100; // La chance est alors à 100 % puisque la condition est remplie. Cela permet une gestion BlizLike
							}
							else 
							{
								MiscEvent= true; // permet de lancer le deuxième event à la fin du premier
								sp->MiscEvent1_MaxCount = -1; // On ne peut plus lancer cet event puisque celui-ci est "épuisé"
							}
						}break;
					default:
						Log.Warning("AIAgent", "Cet event n'existe pas."); 
						break;
				}
			}
			
			if(sp->MiscEvent2)
			{
				uint32 ValMiscEvent = sp->MiscEvent2;
				switch (ValMiscEvent)
				{
					case ACTION_TYPE_HEAL:
					case ACTION_TYPE_MANA:
					case ACTION_TYPE_FOCUS:					
					case ACTION_TYPE_ENERGIE:
					case ACTION_TYPE_PET_HAPINESS:
						{
							bool MiscEvent2 = MiscEvent;
							if (sp->MiscEvent2_Val >= CurVal && MiscEvent2 != false) 
							{
								sp->procCount = sp->MiscEvent2_MaxCount;
								sp->procChance = 100;
							}
							else 
							{
								MiscEvent3= true; // permet de lancer le troisième event à la fin du deuxième
								sp->MiscEvent2_MaxCount = -1; // On ne peut plus lancer cet event puisque celui-ci est "épuisé"
							}								
						}break;
					case ACTION_TYPE_RAGE:
					case ACTION_TYPE_POWER6:
					case ACTION_TYPE_RUNICPOWER:
						{
							bool MiscEvent2 = MiscEvent;
							if (sp->MiscEvent2_Val <= CurVal && MiscEvent2 != false) 
							{
								sp->procCount = sp->MiscEvent2_MaxCount;
								sp->procChance = 100;
							}
							else 
							{
								MiscEvent3= true; // permet de lancer le troisième event à la fin du deuxième
								sp->MiscEvent2_MaxCount = -1; // On ne peut plus lancer cet event puisque celui-ci est "épuisé"
							}								
						}break;
					default:
						Log.Warning("AIAgent", "Cet event n'existe pas."); 
						break;
				}
			}
			
			if(sp->MiscEvent3)
			{
				uint32 ValMiscEvent = sp->MiscEvent3;
				switch (ValMiscEvent)
				{
					case ACTION_TYPE_HEAL:
					case ACTION_TYPE_MANA:
					case ACTION_TYPE_FOCUS:					
					case ACTION_TYPE_ENERGIE:
					case ACTION_TYPE_PET_HAPINESS:
						{
							if (sp->MiscEvent3_Val >= CurVal && MiscEvent3 != false) 
							{
								sp->procCount = sp->MiscEvent3_MaxCount;
								sp->procChance = 100;
							}
							else
							{
								MiscEvent = false;				
								sp->MiscEvent3_MaxCount = -1; // On ne peut plus lancer cet event puisque celui-ci est "épuisé"
							}						
						}break;
					case ACTION_TYPE_RAGE:
					case ACTION_TYPE_POWER6:
					case ACTION_TYPE_RUNICPOWER:
						{
							if (sp->MiscEvent3_Val <= CurVal && MiscEvent3 != false) 
							{
								sp->procCount = sp->MiscEvent3_MaxCount;
								sp->procChance = 100;
							}
							else
							{
								MiscEvent = false;				
								sp->MiscEvent3_MaxCount = -1; // On ne peut plus lancer cet event puisque celui-ci est "épuisé"
							}							
						}break;
					default:
						Log.Warning("AIAgent", "Cet event n'existe pas."); 
						break;
				}
			}
			
			/*bool MiscEvent1 = MiscEvent;		
			if (sp->MiscEvent1_Val >= CurVal && MiscEvent1 != false) 
			{
				sp->procCount = sp->MiscEvent1_MaxCount; // On override le proCount existant
				sp->procChance = 100; // La chance est alors à 100 % puisque la condition est remplie. Cela permet une gestion BlizLike
			} 
			else 
			{
				MiscEvent= true; // permet de lancer le deuxième event à la fin du premier
				sp->MiscEvent1_MaxCount = -1; // On ne peut plus lancer cet event puisque celui-ci est "épuisé"
			}
			bool MiscEvent2 = MiscEvent;
			if (sp->MiscEvent2_Val >= CurVal && MiscEvent2 != false) 
			{
				sp->procCount = sp->MiscEvent2_MaxCount;
				sp->procChance = 100;
			}
			else 
			{
				MiscEvent3= true; // permet de lancer le troisième event à la fin du deuxième
				sp->MiscEvent2_MaxCount = -1; // On ne peut plus lancer cet event puisque celui-ci est "épuisé"
			}				
			if (sp->MiscEvent3_Val >= CurVal && MiscEvent3 != false) 
			{
				sp->procCount = sp->MiscEvent3_MaxCount;
				sp->procChance = 100;
			}
			else
			{
				MiscEvent = false;				
				sp->MiscEvent3_MaxCount = -1; // On ne peut plus lancer cet event puisque celui-ci est "épuisé"
			}*/
				
			/* ****** Fin de la gestion des Miscellenous Events par Randdrick **** */

			 // si le compteur est superieur ou egale a maxcount
			 if(sp->procCount) 
			 {
				if(sp->procCounter >= sp->procCount)
				{
#ifdef _AI_SPELL_DEBUG
				 Log.Notice("AIAgent", "MaxCount : GetSpell() end. %u / %u (%u)",sp->procCounter,sp->procCount,sp->entryId);
#endif
				 MiscEvent = false;
				 if (MiscEvent3 == true) MiscEvent3 = false;
				 next_spell_time = (uint32)UNIXTIME + 1; // (cool_time/1000); ttes les secs
				 return 0;
				}
				else MiscEvent = true;
			 }
			
			 if((sp->agent == AGENT_MELEE) || (sp->agent == AGENT_RANGED) || (sp->agent == AGENT_SPELL))
			 {
				if(sp->procChance >= 100 || Rand(sp->procChance))
				{
					if (sp->spellType == STYPE_BUFF)
					{
						// cast the buff at requested percent only if we don't have it already
						if(!m_Unit->HasActiveAura(sp->spell->Id)) 
						{
#ifdef _AI_SPELL_DEBUG
				         Log.Notice("AIAgent", "GetSpell() end, Aura Active . (%u)",sp->spell->Id);
#endif
						 return sp;
						}
					} 
					else 
					{
                     if(def_spell!=0)  
					 {
#ifdef _AI_SPELL_DEBUG
				         Log.Notice("AIAgent", "GetSpell() def_spell existe deja. (%u)",def_spell->spell->Id);
#endif
						 next_spell_time = (uint32)UNIXTIME + 1; // (cool_time/1000); ttes les secs
						 return 0;
					 }
							
                     //focus/mana/energie requirement
					 switch(sp->spell->powerType)
					 {
					  case POWER_TYPE_MANA: if(m_Unit->GetUInt32Value(UNIT_FIELD_POWER1) < sp->spell->manaCost) 
											{
#ifdef _AI_SPELL_DEBUG
												Log.Notice("AIAgent", "GetSpell() Mana: Pas assez de mana, End.(%u)",sp->spell->Id);
#endif
												next_spell_time = (uint32)UNIXTIME + 1; // (cool_time/1000); ttes les secs
											    return 0;
											}
                                            break;

 					  case POWER_TYPE_FOCUS: if (m_Unit->GetUInt32Value(UNIT_FIELD_POWER3) < sp->spell->manaCost)
											 {
#ifdef _AI_SPELL_DEBUG
												 Log.Notice("AIAgent", "GetSpell() Focus: Pas assez de focus, End.(%u)",sp->spell->Id);
#endif
												 next_spell_time = (uint32)UNIXTIME + 1; // (cool_time/1000); ttes les secs
								                 return 0;
											 }
							                break;
					 
					 
 					  case POWER_TYPE_ENERGY: if (m_Unit->GetUInt32Value(UNIT_FIELD_POWER4) < sp->spell->manaCost)
											 {
#ifdef _AI_SPELL_DEBUG
												 Log.Notice("AIAgent", "GetSpell() Energie: Pas assez d'energie, End.(%u)",sp->spell->Id);
#endif
												 next_spell_time = (uint32)UNIXTIME + 1; // (cool_time/1000); ttes les secs
								                 return 0;
											 }
							                break;
					 }
					 
					 def_spell = sp;	
								
					} // End "else if (sp->spellType == STYPE_BUFF)"	
				} // End if(sp->procChance >= 100 || Rand(sp->procChance))		
			 } // End if(sp->agent == AGENT_MELEE || AGENT_RANGED || AGENT_SPELL)
			 else
			 {
				 Log.Warning("AIAgent","Bad Agent in action! (Spell:%u, Agent:%u",sp->entryId,sp->agent);
				 return(0);
			 }
			} // End if(sp) 
			else
			{
			 Log.Error("AIAgent","Spell not found!! <--- Report this to Devs.");
			 return(0);
			}
	} // End "else if(m_Unit->IsPet())"
	
	if(def_spell)
	{
		if(def_spell->procCount) def_spell->procCounter++;

		if(def_spell->cooldown) def_spell->cooldowntime = nowtime + def_spell->cooldown;

		waiting_for_cooldown = false; // Premier passage (Branruz)

#ifdef _AI_SPELL_DEBUG
		Log.Notice("AIAgent", "GetSpell() = %u, End normally.",def_spell->spell->Id);
#endif
		return def_spell;
		
	}

#ifdef _AI_SPELL_DEBUG
	Log.Notice("AIAgent", "GetSpell() end. Spell Chance Missed! ");
#endif

	next_spell_time = (uint32)UNIXTIME + 1;
	return 0;
}

void AIInterface::addSpellToList(AI_Spell *sp)
{
	if(!sp->spell)
		return;

	if(m_Unit->IsPet())
	{		
		if (sp->procCount || sp->cooldown)
		{
			AI_Spell * sp1 = new AI_Spell;
			memcpy(sp1, sp, sizeof(AI_Spell));
			sp1->procCounter=0;
			sp1->cooldowntime=0;
			sp1->custom_pointer = true;
			m_spells.push_back(sp1);
		}
		else
		{
			m_spells.push_back(sp);
		}
	}

	// creation de la liste pour les NPCs
	if(sp->procCount || sp->cooldown)
	{
		AI_Spell * sp2 = new AI_Spell;
		memcpy(sp2, sp, sizeof(AI_Spell));
		sp2->procCounter=0;
		sp2->cooldowntime=0;
		sp2->custom_pointer = true;
		npc_spells.push_back(sp2);
	}
	else
		npc_spells.push_back(sp);

	m_Unit->m_SpellList.insert(sp->spell->Id); // add to list

}

uint32 AIInterface::getThreatByGUID(uint64 guid)
{
	Unit *obj = m_Unit->GetMapMgr()->GetUnit(guid);
	if(obj)
		return getThreatByPtr(obj);

	return 0;
}

uint32 AIInterface::getThreatByPtr(Unit* obj)
{
	TargetMap::iterator it = m_aiTargets.find(obj);
	if(it != m_aiTargets.end())
	{
		return it->second;
	}
	return 0;
}

//should return a valid target
Unit *AIInterface::GetMostHated()
{
	Unit *ResultUnit=NULL;

	//override mosthated with taunted target. Basic combat checks are made for it. 
	//What happens if we can't see tauntedby unit ?
	ResultUnit = getTauntedBy();
	if(ResultUnit)
		return ResultUnit;

	pair<Unit*, int32> currentTarget;
	currentTarget.first = 0;
	currentTarget.second = -1;

	TargetMap::iterator it2 = m_aiTargets.begin();
	TargetMap::iterator itr;
	for(; it2 != m_aiTargets.end();)
	{
		itr = it2;
		++it2;

		/* check the target is valid */
		if(itr->first->event_GetCurrentInstanceId() != m_Unit->event_GetCurrentInstanceId() || 
			!itr->first->isAlive() || !isAttackable(m_Unit, itr->first))
		{
			m_aiTargets.erase(itr);
			continue;
		}


		if (itr->first->m_auracount[SPELL_AURA_IGNORE_ENEMY] > 0 && m_aiTargets.size() > 1)
			continue;

		if((itr->second + itr->first->GetThreatModifyer()) > currentTarget.second)
		{
			/* new target */
			currentTarget.first = itr->first;
			currentTarget.second = itr->second + itr->first->GetThreatModifyer();
			m_currentHighestThreat = currentTarget.second;
		}

		/* there are no more checks needed here... the needed checks are done by CheckTarget() */
	}

	return currentTarget.first;
}
Unit *AIInterface::GetSecondHated()
{
	Unit *ResultUnit=GetMostHated();

	pair<Unit*, int32> currentTarget;
	currentTarget.first = 0;
	currentTarget.second = -1;

	TargetMap::iterator it2 = m_aiTargets.begin();
	TargetMap::iterator itr;
	for(; it2 != m_aiTargets.end();)
	{
		itr = it2;
		++it2;

		/* check the target is valid */
		if(itr->first->GetInstanceID() != m_Unit->GetInstanceID() || !itr->first->isAlive() || !isAttackable(m_Unit, itr->first))
		{
			m_aiTargets.erase(itr);
			continue;
		}

		if((itr->second + itr->first->GetThreatModifyer()) > currentTarget.second &&
			itr->first != ResultUnit)
		{
			/* new target */
			currentTarget.first = itr->first;
			currentTarget.second = itr->second + itr->first->GetThreatModifyer();
			m_currentHighestThreat = currentTarget.second;
		}
	}

	return currentTarget.first;
}
bool AIInterface::modThreatByGUID(uint64 guid, int32 mod)
{
	if (!m_aiTargets.size())
		return false;

	Unit *obj = m_Unit->GetMapMgr()->GetUnit(guid);
	if(obj)
		return modThreatByPtr(obj, mod);

	return false;
}

bool AIInterface::modThreatByPtr(Unit* obj, int32 mod)
{
	if(obj == NULL)
		return false;
	int32 newthreat = 0;
		
	TargetMap::iterator it = m_aiTargets.find(obj);
	if(it != m_aiTargets.end())
	{
		it->second += mod;
		//mod threat cant remove from combat, remove threat can
		if (it->second < 1)
			it->second = 1;

		newthreat = it->second;
	}
	else
	{
		m_aiTargets.insert(TargetMap::value_type(obj, mod >= 1? mod : 1)); //threat is minimum 1
		newthreat = mod >= 1? mod : 1;
	}

	//dist threat mod :P
	float distmod = 1.0f;
	if (m_Unit->CalcDistance(obj) > _CalcCombatRange(obj, false) + 5.0f)
		distmod = 1.1f;
	else
		distmod = 1.25f;

	if((newthreat + obj->GetThreatModifyer()) > m_currentHighestThreat * distmod)
	{
		// new target!
		if(!isTaunted)
		{
			m_currentHighestThreat = newthreat + obj->GetThreatModifyer();
			SetNextTarget(obj);
		}
	}

	if(obj == m_nextTarget && !isTaunted)
	{
		// check for a possible decrease in threat.
		if(mod < 0)
		{
			m_nextTarget = GetMostHated();
			//if there is no more new targets then we can walk back home ?
			if(!m_nextTarget)
				HandleEvent(EVENT_LEAVECOMBAT, m_Unit, 0);
			SetNextTarget(m_nextTarget);
		}
	}

	if (isTaunted && tauntedBy == obj && m_nextTarget != obj)
		SetNextTarget(obj);
	
	return true;
}

void AIInterface::RemoveThreatByPtr(Unit* obj)
{
	if(!obj)
		return;
	TargetMap::iterator it = m_aiTargets.find(obj);
	if(it != m_aiTargets.end())
		m_aiTargets.erase(it);

	//no aitarget entry
	if(obj==m_nextTarget)
	{
		m_nextTarget = GetMostHated();
		//if there is no more new targets then we can walk back home ?
		if(!m_nextTarget)
			HandleEvent(EVENT_LEAVECOMBAT, m_Unit, 0);
		SetNextTarget(m_nextTarget);
	}
}

void AIInterface::addAssistTargets(Unit* Friend)
{
	// stop adding stuff that gives errors on linux!
	m_assistTargets.insert(Friend);
}

void AIInterface::WipeHateList()
{
	for(TargetMap::iterator itr = m_aiTargets.begin(); itr != m_aiTargets.end(); ++itr)
		itr->second = 0;
	m_currentHighestThreat = 0;
}
void AIInterface::ClearHateList() //without leaving combat
{
	for(TargetMap::iterator itr = m_aiTargets.begin(); itr != m_aiTargets.end(); ++itr)
		itr->second = 1;
	m_currentHighestThreat = 1;
}

void AIInterface::WipeTargetList()
{
	SetNextTarget(NULL);

	m_nextSpell = NULL;
	m_currentHighestThreat = 0;
	m_aiTargets.clear();
	m_Unit->CombatStatus.Vanished(); // Disparu
}

bool AIInterface::taunt(Unit* caster, bool apply)
{
	if(apply)
	{
		//wowwiki says that we cannot owerride this spell
		if(GetIsTaunted())
			return false;

		if(!caster)
		{
			isTaunted = false;
			return false;
		}
		
		//Andy: use isattackable here, arenas, duels etc, taunt effects work on pets
		//check if we can attack taunter. Maybe it's a hack or a bug if we fail this test
		if(isAttackable(m_Unit, caster))
		{
			isTaunted = true;
			tauntedBy = caster;

			int32 oldthreat = getThreatByPtr(caster);
			//make sure we rush the target anyway. Since we are not tauted yet, this will also set our target
			modThreatByPtr(caster, abs(m_currentHighestThreat-oldthreat)); //we need to be the most hated at this moment
		}
	}
	else
	{
		isTaunted = false;
		tauntedBy = NULL;
		//taunt is over, we should get a new target based on most hated list
		SetNextTarget(GetMostHated());
	}

	return true;
}

Unit* AIInterface::getTauntedBy()
{
	if(GetIsTaunted())
	{
		return tauntedBy;
	}
	else
	{
		return NULL;
	}
}

bool AIInterface::GetIsTaunted()
{
	if(isTaunted)
	{
		if(!tauntedBy || !tauntedBy->isAlive())
		{
			isTaunted = false;
			tauntedBy = NULL;
		}
	}
	return isTaunted;
}

void AIInterface::SetSoulLinkedWith(Unit* target)
{
	if (!target)
		return;
	soullinkedWith = target;
	isSoulLinked = true;
}

Unit* AIInterface::getSoullinkedWith()
{
	if(GetIsTaunted())
	{
		return soullinkedWith;
	}
	else
	{
		return NULL;
	}
}

bool AIInterface::GetIsSoulLinked()
{
	if(isSoulLinked)
	{
		if(!soullinkedWith || !soullinkedWith->isAlive())
		{
			isSoulLinked = false;
			soullinkedWith = NULL;
		}
	}
	return isSoulLinked;
}

void AIInterface::CheckTarget(Unit* target)
{
	if( target == NULL )
		return;

	if( target == UnitToFollow )		  // fix for crash here
	{
		UnitToFollow = NULL;
		m_lastFollowX = m_lastFollowY = 0;
		FollowDistance = 0;
	}

	if( target == UnitToFollow_backup )
	{
		UnitToFollow_backup = NULL;
	}

	AssistTargetSet::iterator  itr = m_assistTargets.find(target);
	if(itr != m_assistTargets.end())
		m_assistTargets.erase(itr);

	TargetMap::iterator it2 = m_aiTargets.find( target );
	if( it2 != m_aiTargets.end() || target == m_nextTarget )
	{
		target->CombatStatus.RemoveAttacker( m_Unit, m_Unit->GetGUID() );
		m_Unit->CombatStatus.RemoveAttackTarget( target );

		if(it2 != m_aiTargets.end())
		{
			m_aiTargets.erase(it2);
		}

		if (target == m_nextTarget)	 // no need to cast on these.. mem addresses are still the same
		{
			SetNextTarget(NULL);
			m_nextSpell = NULL;

			// find the one with the next highest threat
			GetMostHated();
		}
	}

	if( target->GetTypeId() == TYPEID_UNIT && target->IsInWorld() )
	{
		it2 = target->GetAIInterface()->m_aiTargets.find( m_Unit );
		if( it2 != target->GetAIInterface()->m_aiTargets.end() )
			target->GetAIInterface()->m_aiTargets.erase( it2 );
        
		if( target->GetAIInterface()->m_nextTarget == m_Unit )
		{
			target->GetAIInterface()->m_nextTarget = NULL;
			target->GetAIInterface()->m_nextSpell = NULL;
			target->GetAIInterface()->GetMostHated();
		}

		if( target->GetAIInterface()->UnitToFollow == m_Unit )
			target->GetAIInterface()->UnitToFollow = NULL;
	}

	if(target == UnitToFear)
		UnitToFear = NULL;

	if(tauntedBy == target)
		tauntedBy = NULL;
}

uint32 AIInterface::_CalcThreat(uint32 damage, SpellEntry * sp, Unit* Attacker)
{
	if (isSameFaction(m_Unit,Attacker))
		return 0;

	int32 mod = 0;
	if( sp != NULL && sp->ThreatForSpell != 0 )
	{
		mod = sp->ThreatForSpell;
	}
	else
	{
		mod = damage;
	}

	// modify mod by Affects
	mod += (mod * Attacker->GetGeneratedThreatModifyer() / 100);
 
	if (sp != NULL)
		SM_PIValue(Attacker->SM_PThreat_Reduction, &mod, sp);

	return mod;
}

void AIInterface::WipeReferences()
{
	m_nextSpell = 0;
	m_currentHighestThreat = 0;
	m_aiTargets.clear();
	SetNextTarget(NULL);
	UnitToFear = 0;
	UnitToFollow = 0;
	tauntedBy = 0;
}

void AIInterface::WipeReferences(Object *obj)
{
	if (obj->IsUnit())
	{
		Unit *uobj = static_cast<Unit *>(obj);

		if (m_nextTarget == obj)
			RemoveThreatByPtr(uobj);

		if (m_Unit->IsCreature() && m_PetOwner == obj)
		{
			static_cast<Creature *>(m_Unit)->SafeDelete();
			return;
		}

		if (tauntedBy == obj)
		{
			tauntedBy = NULL;
			isTaunted = false;
		}

		if (soullinkedWith == obj)
			SetSoulLinkedWith(NULL);

		if (UnitToFollow == obj)
			UnitToFollow = NULL;

		if (UnitToFollow_backup == obj)
			UnitToFollow_backup = NULL;

		if (UnitToFear == obj)
			UnitToFear = NULL;

		if (MindControlOwner == obj)
		{
			if (MindControlAura != NULL)
				MindControlAura->Remove();
			MindControlOwner = NULL;
		}

		std::set<uint64>::iterator itr = MindControlChilds.find(obj->GetGUID());
		if (itr != MindControlChilds.end() && m_Unit->IsInWorld())
		{
			Unit *child = m_Unit->GetMapMgr()->GetUnit(*itr);
			if (child != NULL && child->GetAIInterface() != NULL && child->GetAIInterface()->MindControlAura != NULL)
				child->GetAIInterface()->MindControlAura->Remove();
		}
	}
}
void AIInterface::ResetProcCounts()
{
	for(list<AI_Spell*>::iterator itr = m_spells.begin(); itr != m_spells.end(); ++itr)
		if((*itr)->procCount)
			(*itr)->procCounter=0;
}

/*
void AIInterface::CancelSpellCast()
{
	//hmm unit spell casting is not the same as Ai spell casting ? Have to test this
	if(m_Unit->isCasting())
		m_Unit->m_currentSpell->safe_cancel();
	//i can see this crashing already :P.
	m_AIState = STATE_IDLE;
}
*/

bool isGuard(uint32 id)
{
	switch(id)
	{
		/* stormwind guards */
	case 68:
	case 1423:
	case 1756:
	case 15858:
	case 15859:
	case 16864:
	case 20556:
	case 18948:
	case 18949:
	case 1642:
		/* ogrimmar guards */
	case 3296:
	case 15852:
	case 15853:
	case 15854:
	case 18950:
		/* undercity guards */
	case 5624:
	case 18971:
	case 16432:
		/* exodar */
	case 16733:
	case 18815:
		/* thunder bluff */
	case 3084:
		/* silvermoon */
	case 16221:
	case 17029:
	case 16222:
		/* ironforge */
	case 727:
	case 5595:
	case 12996:
		/* darnassus? */
		{
			return true;
		}break;
	}
	return false;
}

void AIInterface::WipeCurrentTarget()
{
	TargetMap::iterator itr = m_aiTargets.find( m_nextTarget );
	if( itr != m_aiTargets.end() )
		m_aiTargets.erase( itr );

	//m_nextTarget = NULL; // alors ca c tres zarb??? 

	if( m_nextTarget == UnitToFollow )
		UnitToFollow = NULL;

	if( m_nextTarget == UnitToFollow_backup )
		UnitToFollow_backup = NULL;

	m_nextTarget = NULL;
}

bool AIInterface::CheckCurrentTarget()
{
	bool cansee = false;

	if(m_nextTarget && m_Unit)
	{
	 if(m_nextTarget->GetInstanceID() == m_Unit->GetInstanceID())
	 {
		if( m_Unit->GetTypeId() == TYPEID_UNIT )
			cansee = static_cast< Creature* >( m_Unit )->CanSee( m_nextTarget );
		else
			cansee = static_cast< Player* >( m_Unit )->CanSee( m_nextTarget );
	 }
	}
	else 
	{
		WipeCurrentTarget();
	}


	return cansee;
}

bool AIInterface::TargetUpdateCheck(Unit * ptr)
{
	
	if( ptr->event_GetCurrentInstanceId() != m_Unit->event_GetCurrentInstanceId() ||
		!ptr->isAlive() || m_Unit->GetDistanceSq(ptr) >= 6400.0f )
	{
		return false;
	}
	
	return true;
}

void AIInterface::HandleChainAggro(Unit* u)
{
	if (!m_Unit->IsInWorld() || !m_Unit->isAlive() || m_Unit->m_chain == NULL)
		return;

	for (std::set<Unit*>::iterator itr=m_Unit->m_chain->m_units.begin(); itr!=m_Unit->m_chain->m_units.end(); ++itr)
	{
		if (!(*itr)->IsInWorld() || !(*itr)->isAlive() || (m_Unit->m_chain->m_chainrange != 0 && m_Unit->CalcDistance(*itr) > m_Unit->m_chain->m_chainrange))
			continue;

		if ((*itr)->GetAIInterface()->GetAITargets()->find(u) == (*itr)->GetAIInterface()->GetAITargets()->end())
		{
			if((*itr)->GetAIInterface()->getAIState() == STATE_IDLE || (*itr)->GetAIInterface()->getAIState() == STATE_FOLLOWING)
				(*itr)->GetAIInterface()->HandleEvent(EVENT_ENTERCOMBAT, u, 0);
			else
				(*itr)->GetAIInterface()->GetAITargets()->insert(TargetMap::value_type(u, 1));
		}
	}
}

uint32 AIInterface::GetMoveTime(float distance)
{
	if (m_moveFly)
		return float2int32(distance / m_flySpeed);
	else if (m_moveRun)
		return float2int32(distance / m_runSpeed);
	else
		return float2int32(distance / m_walkSpeed);	
}