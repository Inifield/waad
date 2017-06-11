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

bool isHostile(Object* objA, Object* objB)// B is hostile for A?
{
	bool hostile = false;

	// can't attack self.. this causes problems with buffs if we dont have it :p
	if(!objA || !objB || (objA == objB))
		return false;

	// can't attack corpses neither...
	if(objA->GetTypeId() == TYPEID_CORPSE || objB->GetTypeId() == TYPEID_CORPSE)
		return false;

	if( objA->m_faction == NULL || objB->m_faction == NULL || objA->m_factionDBC == NULL || objB->m_factionDBC == NULL )
		return true;

	//What you can't see, can't be hostile!
	if( !(objA->m_Phase & objB->m_Phase) )
		return false;
		
	//neutral guards
	if (objA->IsUnit() && static_cast<Unit*>(objA)->GetAIInterface()->m_neutralGuard && objB->IsPlayer() && static_cast<Player*>(objB)->m_pvpcombat)
		return true;
	if (objB->IsUnit() && static_cast<Unit*>(objB)->GetAIInterface()->m_neutralGuard && objA->IsPlayer() && static_cast<Player*>(objA)->m_pvpcombat)
		return true;

	// Test, la colonne Mask a disparu du FactionTemplate.dbc, (Branruz)
	//uint32 faction = objB->m_faction->Mask;
	uint32 factionG = objA->m_faction->FactionGroup;
	uint32 hostileM  = objB->m_faction->HostileMask;
	uint32 friendlyM = objB->m_faction->FriendlyMask;
    
    if(factionG & hostileM)  hostile = true;
	//if(factionG & friendlyM) hostile = false; // FFA (Arene etc...) (Branruz)

	// check friend/enemy list
	for(uint32 i = 0; i < 4; i++)
	{
		//if(objA->m_faction->EnemyFactions[i] == objB->m_faction->Faction)
		if(objB->m_faction->EnemyFactions[i] == objA->m_faction->Faction)
		{
			hostile = true;
			break;
		}
		if(objB->m_faction->FriendlyFactions[i] == objA->m_faction->Faction)
		{
			hostile = false;
			break;
		}
	}

	// PvP Flag System Checks
	// We check this after the normal isHostile test, that way if we're
	// on the opposite team we'll already know :p

	if( hostile && ( objA->IsPlayer() || objA->IsPet() || ( objA->IsUnit() && !objA->IsPlayer() && static_cast< Creature* >( objA )->IsTotem() && static_cast< Creature* >( objA )->GetSummonOwner()->IsPlayer() && static_cast<Player*>(static_cast< Creature* >( objA )->GetSummonOwner())->IsPvPFlagged() ) ) )
	{
		if( objB->IsPlayer() )
		{
			// Check PvP Flags.
			if( static_cast< Player* >( objB )->IsPvPFlagged() )
				return true;
			else
				return false;
		}
		if( objB->IsPet() )
		{
			// Check PvP Flags.
			if( static_cast< Pet* >( objB )->GetPetOwner() != NULL && 
				static_cast< Pet* >( objB )->GetPetOwner()->IsPlayer() && 
				static_cast< Pet* >( objB )->GetPetOwner()->GetMapMgr() == objB->GetMapMgr() && 
				static_cast<Player*>(static_cast< Pet* >( objB )->GetPetOwner())->IsPvPFlagged() )
				return true;
			else
				return false;
		}
	}

	//ignore rep based when mind controlled
	// Reputation System Checks
	if(objA->IsPlayer() && !objB->IsPlayer() && 
		static_cast<Player*>(objA)->GetAIInterface()->MindControlOwner == NULL)	   // PvE
	{
		if(objB->m_factionDBC->RepListId >= 0)
			hostile = static_cast< Player* >( objA )->IsHostileBasedOnReputation( objB->m_factionDBC );
	}
	
	if(objB->IsPlayer() && !objA->IsPlayer() && static_cast<Player*>(objB)->GetAIInterface()->MindControlOwner == NULL)	   // PvE
	{
		if(objA->m_factionDBC->RepListId >= 0)
			hostile = static_cast< Player* >( objB )->IsHostileBasedOnReputation( objA->m_factionDBC );
	}

	if( objA->IsPlayer() && objB->IsPlayer() && static_cast<Player*>(objA)->m_bg != NULL )
	{
		if( static_cast<Player*>(objA)->m_bgTeam != static_cast<Player*>(objB)->m_bgTeam )
			return true;
	}

	return hostile;
}

/// Where we check if we object A can attack object B. This is used in many feature's
/// Including the spell class and the player class.
bool isAttackable(Object* objA, Object* objB, bool CheckStealth)// A can attack B?
{
/*
#ifdef COLLISION
	// Test si la cible est visible
	
	if(!CollideInterface.CheckLOS(objA->GetMapId(),objA->GetPositionX(),objA->GetPositionY(),objA->GetPositionZ(),
		                                       objB->GetPositionX(),objB->GetPositionY(),objB->GetPositionZ()))
       return(false);
#endif
*/

	// can't attack self.. this causes problems with buffs if we don't have it :p
	if( !objA || !objB || objA == objB )
		return false;   

	// can't attack corpses neither...
	if( objA->GetTypeId() == TYPEID_CORPSE || objB->GetTypeId() == TYPEID_CORPSE )
		return false;

	// We do need all factiondata for this
	if( objB->m_factionDBC == NULL || objA->m_factionDBC == NULL || objB->m_faction == NULL || objA->m_faction == NULL )
		return false;

	// Checks for untouchable, unattackable
	if( objA->IsUnit() && objA->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_MOUNTED_TAXI | UNIT_FLAG_NOT_SELECTABLE))
		return false;
	if( objB->IsUnit() && objB->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_MOUNTED_TAXI | UNIT_FLAG_NOT_SELECTABLE))
		return false;

	// we cannot attack sheathed units. Maybe checked in other places too ?
	// !! warning, this presumes that objA is attacking ObjB
	if( CheckStealth && objB->IsUnit() && static_cast<Unit*>(objB)->IsStealth() )
		return false;

	//What you can't see, you can't attack either...
	if( !(objA->m_Phase & objB->m_Phase) )
		return false;

	//neutral guards
	if (objA->IsUnit() && static_cast<Unit*>(objA)->GetAIInterface()->m_neutralGuard && objB->IsPlayer() && static_cast<Player*>(objB)->m_pvpcombat)
		return true;
	if (objB->IsUnit() && static_cast<Unit*>(objB)->GetAIInterface()->m_neutralGuard && objA->IsPlayer() && static_cast<Player*>(objA)->m_pvpcombat)
		return true;

	//Get players (or owners of pets/totems)
	Player* pFromA = objA->GetPlayerFrom();
	Player* pFromB = objB->GetPlayerFrom();

	if(objA->IsPlayer() && objB->IsPlayer())
	{
		//Handle duels
		if (pFromA != NULL && pFromB != NULL && pFromA->DuelingWith == pFromB && pFromA->GetDuelState() == DUEL_STATE_STARTED)
			return true;

		// do not let people attack each other in sanctuary
		// Dueling is already catered for
		if (pFromA != NULL && pFromB != NULL)
		{
			AreaTable *atA = dbcArea.LookupEntry(pFromA->GetAreaID());
			AreaTable *atB = dbcArea.LookupEntry(pFromB->GetAreaID());

			if (atA != NULL && atB != NULL)
			{
				if (atA->category == AREAC_SANCTUARY || atA->AreaFlags & AREA_SANCTUARY || atB->category == AREAC_SANCTUARY || atB->AreaFlags & AREA_SANCTUARY)
					return false;
			}
		}

		// Players with feign death flags can't be attacked
		// But they can be attacked by another players. -- Dvlpr
		// WARNING: This presumes, that objA attacks objb!!!
		if( (objA->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || objB->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH)) && !objA->IsPlayer() )
			return false;

		//Handle BG's
		if( pFromA->m_bg != NULL )
		{
			//Handle ffa_PVP
			if(objA->HasFlag(PLAYER_FLAGS,PLAYER_FLAG_FREE_FOR_ALL_PVP) && objB->HasFlag(PLAYER_FLAGS,PLAYER_FLAG_FREE_FOR_ALL_PVP))
			{
				if( pFromA->GetGroup() == pFromB->GetGroup() )
					return false;
				else
					return true;
			}
			//Handle Arenas
			if( pFromA->GetTeam() != pFromB->GetTeam() )
				return true;
		}

		if(pFromA->IsFFAPvPFlagged() && pFromB->IsFFAPvPFlagged())
		{
			if( pFromA->GetGroup() && pFromA->GetGroup() == pFromB->GetGroup() )
				return false;

			if( pFromA == pFromB) // Totems...
				return false;

			return true;		// can hurt each other in FFA pvp
		}

		if( pFromA->GetAreaDBC() != NULL )
		{
			if( pFromA->GetAreaDBC()->AreaFlags & 0x800 )
				return false;
		}
	}	

	// same faction can't kill each other.
	if(objA->m_faction == objB->m_faction)  
		return false;

	//moved this from IsHostile(); 
	// by doing so we skip a hell of a lot redundant checks, which we already passed in this routine.
	//uint32 faction = objB->m_faction->Mask;
	uint32 factionG = objA->m_faction->FactionGroup;
	uint32 hostileM  = objB->m_faction->HostileMask;
	uint32 friendlyM = objB->m_faction->FriendlyMask;
    bool hostile = false;
    if(factionG & hostileM)  hostile = true;
	//if(factionG & friendlyM) hostile = false; // FFA (Arene etc...) (Branruz)

	// check friend/enemy list
	for(uint32 i = 0; i < 4; i++)
	{
		//if(objA->m_faction->EnemyFactions[i] == objB->m_faction->Faction)
		if(objB->m_faction->EnemyFactions[i] == objA->m_faction->Faction)
		{
			hostile = true;
			break;
		}
		if(objB->m_faction->FriendlyFactions[i] == objA->m_faction->Faction)
		{
			hostile = false;
			break;
		}
	}

	// Reputation System Checks
	if(pFromA && !pFromB)	   // PvE
	{
		if(objB->m_factionDBC->RepListId >= 0)
			hostile = pFromA->IsHostileBasedOnReputation( objB->m_factionDBC );
	}
	
	if(pFromB && !pFromA)	   // PvE
	{
		if(objA->m_factionDBC->RepListId >= 0)
			hostile = pFromB->IsHostileBasedOnReputation( objA->m_factionDBC );
	}

	// Neutral Creature Check
	if(pFromA != NULL)
	{
		if(objB->m_factionDBC->RepListId == -1 && objB->m_faction->HostileMask == 0 && objB->m_faction->FriendlyMask == 0)
		{
			hostile = true;
		}
	}
	else if(pFromB != NULL)
	{
		if(objA->m_factionDBC->RepListId == -1 && objA->m_faction->HostileMask == 0 && objA->m_faction->FriendlyMask == 0)
		{
			hostile = true;
		}
	}

	return hostile;
}

bool isCombatSupport(Object* objA, Object* objB)// B combat supports A?
{
	if( !objA || !objB )
		return false;   

	// can't support corpses...
	if( objA->GetTypeId() == TYPEID_CORPSE || objB->GetTypeId() == TYPEID_CORPSE )
		return false;

	// We do need all factiondata for this
	if( objB->m_factionDBC == NULL || objA->m_factionDBC == NULL || objB->m_faction == NULL || objA->m_faction == NULL )
		 return false;

	// fixes an issue where horde pets would chain aggro horde guards and vice versa for alliance.
	if( objA->IsPet() || objB->IsPet() ) return false;

 	//What you can't see, you can't support either...	
	if( !(objA->m_Phase & objB->m_Phase) )
		return false;

	bool combatSupport = false;

	// Test, la colonne Mask a disparu du FactionTemplate.dbc, (Branruz)
	uint32 fSupport   = objB->m_faction->FriendlyMask;
	//uint32 hNoSupport = objB->m_faction->HostileMask; // Pourquoi pas...
	uint32 myFaction = objA->m_faction->FactionGroup;
	if(myFaction & fSupport) combatSupport = true;
	//if(myFaction & hNoSupport) combatSupport = false;

	// check friend/enemy list 
	for(uint32 i = 0; i < 4; i++)
	{
		if(objB->m_faction->EnemyFactions[i] == objA->m_faction->Faction)
		{
			combatSupport = false;
			break;
		}
		if(objB->m_faction->FriendlyFactions[i] == objA->m_faction->Faction)
		{
			combatSupport = true;
			break;
		}
	}
	return combatSupport;
}

bool isAlliance(Object* objA)// A is alliance?
{
	if(!objA || objA->m_factionDBC == NULL || objA->m_faction == NULL)
		return true;

	//Get stormwind faction frm dbc (11/72)
	FactionTemplateDBC * m_sw_faction = dbcFactionTemplate.LookupEntry(11);
	FactionDBC * m_sw_factionDBC = dbcFaction.LookupEntry(72);

	if(m_sw_faction == objA->m_faction || m_sw_factionDBC == objA->m_factionDBC)
		return true;

	//Is StormWind hostile to ObjectA?
	uint32 faction = m_sw_faction->Faction;
	uint32 host = objA->m_faction->HostileMask;

	if(faction & host)
		return false;

	// check friend/enemy list
	for(uint32 i = 0; i < 4; i++)
	{
		if(objA->m_faction->EnemyFactions[i] == faction)
			return false;
	}

	faction = objA->m_faction->Faction;
	host = m_sw_faction->HostileMask;

	if(faction & host)
		return false;

	// check friend/enemy list
	for(uint32 i = 0; i < 4; i++)
	{
		if(objA->m_faction->EnemyFactions[i] == faction)
			return false;
	}

	//We're not hostile towards SW, so we are allied
	return true;
}

