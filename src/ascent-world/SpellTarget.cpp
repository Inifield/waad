/*
* Ascent MMORPG Server
* Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/
// Coded by Captnoord for ascent (2007), please give feedback to me if your able to.

#include "StdAfx.h"

/* LEFTOVER RESEARCH, this contains note's comments from Captnoord
// these are left here because of the importance's of the info.
// comments and additional information related to this send to Captnoord

// Type 4:
// 4 is related to Diseases fun to give it a try..
// dono related to "Wandering Plague", "Spirit Steal", "Contagion of Rot", "Retching Plague" and "Copy of Wandering Plague"

// Type 7:
// Point Blank Area of Effect
// think its wrong, related to 2 spells, "Firegut Fear Storm" and "Mind Probe"
// FillAllTargetsInArea(tmpMap,m_targets.m_destX,m_targets.m_destY,m_targets.m_destZ,GetRadius(i));

//fear storm is nice
//Score 5.7	 Vote: [-] [+] by plle, 1.5 years ago
//when you set that ogre head on the top of the rock and all orges see it they /yell: FLEE (and) RUN!!
//and then all ogres get fear storm =D 

//this quest
//Score 6.9	 Vote: [-] [+] by ewendim, 1.1 years ago
//happens while doing this quest http://www.thottbot.com/?qu=3825

// Type 11
// this is related to spellID: 4, as I think is a gm spell

// 11 select id,name, EffectImplicitTargetA1 from spell where EffectImplicitTargetA1 = 11;
// +----+----------------------+------------------------+
// | id | name				 | EffectImplicitTargetA1 |
// +----+----------------------+------------------------+
// |  4 | Word of Recall Other |					 11 |
// +----+----------------------+------------------------+

// type 17:
// spells like 17278:Cannon Fire and 21117:Summon Son of Flame A
// A single target at a xyz location or the target is a possition xyz

// select id,name, EffectImplicitTargetA1 from spell where EffectImplicitTargetB1 = 9;
// +-------+----------------------------+------------------------+
// | id	| name					   | EffectImplicitTargetA1 |
// +-------+----------------------------+------------------------+
// |	 1 | Word of Recall (OLD)	   |					  1 |
// |	 3 | Word of Mass Recall (OLD)  |					 20 |
// |   556 | Astral Recall			  |					  1 |
// |  8690 | Hearthstone				|					  1 |
// | 39937 | There's No Place Like Home |					  1 |
// +-------+----------------------------+------------------------+

// type 10: is not used
// type 12: is not used
// type 13: is not used
// type 14: is not used

// type 19: is target zone I think (test spells)

// type 48: is summon wild unit
// type 49: is summon friend unit
// type 50: something related to the pref 2
// type 51: is targeting objects / egg's / possible to use those while flying


// type 55 related to blink and Netherstep... I think this one sets the xyz where you should end...
// type 56 is related to aura holder... Player 1 give's me a aura and that aura has as target me. I wear then the aura / spell and it targeting me

// type 58 proc triggeret target... 
// Apply Aura: Proc Trigger Spell
// Retching Plague
// 10% chance.

// type 59 related to 2 visual spells
// type 60 1 target related. rest is unknown or not clear

// type 62 targets the best player of [class]
// type 63 targets chess player... something like that
// type 64 something related to wharp storm... and the wharpstorm creature..
// type 65 target enemy's weapon... Item target..... "A" target type, "B" target type is target enemy unit...
// type 66 related to summon some unit...
// type 67 related to summon some unit...
// type 68 is not used
// type 69 is totem stuff level 70 totem stuff
// type 70 is not used
// type 71 is not used
// type 72 target enemy unit... (I think it does not matter if its a friendly unit)
// type 73 is serverside scripted.... Mostly summons
// type 74 Target Random Enemy
// type 75 Target location in front of the caster
// type 76 target a area... of target...
// type 77 target single enemy
// type 78 units in front of caster ( test spell )
// type 79 is not used
// type 80 related to summon some unit
// type 81 > N are not handled because they are test spells 

*/

/// Fill the target map with the targets
/// the targets are specified with numbers and handled accordingly

void Spell::FillTargetMap(uint32 i)
{

	if(m_spellInfo->spell_signature != _SPELL_SIGNATURE_)
	{
		Log.Error("FillTargetMap","SpellInfo leak! (Index: %u)",i);
		return;
	}
	
	if(!m_caster->IsInWorld())
		return;
		
	uint32 TargetType = SPELL_TARGET_NONE;

	// Get our info from A regardless of nullity
	TargetType |= GetTargetType(m_spellInfo->EffectImplicitTargetA[i], i);

	//never get info from B if it is 0 :P
	if (m_spellInfo->EffectImplicitTargetB[i] != 0)
		TargetType |= GetTargetType(m_spellInfo->EffectImplicitTargetB[i], i);

	SpellTargetMap* t=&m_spellTargets;

	//always add this guy :P
	if(!(TargetType & (SPELL_TARGET_AREA | SPELL_TARGET_AREA_SELF | SPELL_TARGET_AREA_CURTARGET | SPELL_TARGET_AREA_CONE | SPELL_TARGET_OBJECT_SELF | SPELL_TARGET_OBJECT_PETOWNER)))
		AddTarget(i, TargetType, m_targets.m_target);
	
	if (TargetType & SPELL_TARGET_OBJECT_SELF) 
		AddTarget(i, TargetType, m_caster);

	if (TargetType & (SPELL_TARGET_AREA | SPELL_TARGET_AREA_SELF)) // Sort de zone
		AddAOETargets(i, TargetType, GetRadius(i), m_spellInfo->MaxTargets);

	if (!(TargetType & (SPELL_TARGET_PLAYER_SELECTION | SPELL_TARGET_REQUIRE_ATTACKABLE | SPELL_TARGET_REQUIRE_FRIENDLY))) // Sort de zone type Colonne de feu
		AddAOETargets(i, TargetType, GetRadius(i), m_spellInfo->MaxTargets);

	if ((TargetType & SPELL_TARGET_OBJECT_CURTOTEMS) && (m_caster->IsUnit()))
	{
		for (uint32 i=1; i<5; ++i) //totem slots are 1, 2, 3, 4
		{
			AddTarget(i, TargetType, ((Unit *)m_caster)->m_summonslot[i]);
		}
	}

	if ((TargetType & SPELL_TARGET_OBJECT_CURPET) && (m_caster->IsUnit()))
		AddTarget(i, TargetType, ((Unit *)m_caster)->GetSummon());

	if ((TargetType & SPELL_TARGET_OBJECT_PETOWNER) && (m_targets.m_target->IsPet()))
		AddTarget(i, TargetType, static_cast<Pet*>(m_targets.m_target)->GetOwner());

	//targets party, not raid
	if ((TargetType & SPELL_TARGET_AREA_PARTY) && !(TargetType & SPELL_TARGET_AREA_RAID))
	{
		if (!m_caster->IsPlayer() && !m_caster->IsPet() && (!m_caster->IsCreature() || !static_cast<Creature*>(m_caster)->IsTotem()))
			AddAOETargets(i, TargetType, GetRadius(i), m_spellInfo->MaxTargets); //npcs
		else
			AddPartyTargets(i, TargetType, GetRadius(i), m_spellInfo->MaxTargets); //players/pets/totems
	}
	if (TargetType & SPELL_TARGET_AREA_RAID)
	{
		if (!m_caster->IsPlayer() && !m_caster->IsPet() && (!m_caster->IsCreature() || !static_cast<Creature *>(m_caster)->IsTotem()))
			AddAOETargets(i, TargetType, GetRadius(i), m_spellInfo->MaxTargets); //npcs
		else
			AddRaidTargets(i, TargetType, GetRadius(i), m_spellInfo->MaxTargets, (TargetType & SPELL_TARGET_AREA_PARTY)? true: false); //players/pets/totems
	}

	if (TargetType & SPELL_TARGET_AREA_CHAIN)
		AddChainTargets(i, TargetType, GetRadius(i), m_spellInfo->MaxTargets);

	//target cone
	if (TargetType & SPELL_TARGET_AREA_CONE)
		AddConeTargets(i, TargetType, GetRadius(i), m_spellInfo->MaxTargets);
	
	if(TargetType & SPELL_TARGET_OBJECT_SCRIPTED)
		AddScriptedOrSpellFocusTargets(i, TargetType, GetRadius(i), m_spellInfo->MaxTargets);
		
	if ((TargetType & SPELL_TARGET_AREA_CURTARGET) && (m_targets.m_target != NULL))
	{
		//this just forces dest as the targets location :P
		m_targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;
		m_targets.m_destX = m_targets.m_target->GetPositionX();
		m_targets.m_destY = m_targets.m_target->GetPositionY();
		m_targets.m_destZ = m_targets.m_target->GetPositionZ();
		m_targets.m_target = NULL;
	}
}

void Spell::AddConeTargets(uint32 i, uint32 TargetType, float r, uint32 maxtargets)
{
	SpellTargetMap* t=&m_spellTargets;

	std::set<Object*>::iterator itr;
	for( itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
	{
		if(!((*itr)->IsUnit()) || !((Unit*)(*itr))->isAlive())
			continue;
		//is Creature in range
		if(m_caster->isInRange((Unit*)(*itr),GetRadius(i)))
		{
			if( m_spellInfo->cone_width ? m_caster->isInArc( (Unit*)(*itr) , m_spellInfo->cone_width ) : m_caster->isInFront((Unit*)(*itr)) ) // !!! is the target within our cone ?
			{
				AddTarget(i, TargetType, (*itr));	
			}
		}
		if (maxtargets != 0 && t->size() >= maxtargets)
			return;
	}
}

void Spell::AddChainTargets(uint32 i, uint32 TargetType, float r, uint32 maxtargets)
{
	if(!m_caster->IsInWorld())
		return;

	if (m_targets.m_target == NULL)
		return;

	SpellTargetMap* t=&m_spellTargets;
	//if selected target is party member, then jumps on party
	Unit* firstTarget = NULL;

	if (m_targets.m_target->IsUnit()) firstTarget = static_cast<Unit*>(m_targets.m_target);
	else      		                  firstTarget = (Unit *)m_caster;

	bool RaidOnly = false;
	float range = GetMaxRange(dbcSpellRange.LookupEntry(m_spellInfo->rangeIndex));//this is probably wrong,
	//this is cast distance, not searching distance
	range *= range;

	//is this party only?
	Player* casterFrom =  ((Player *)m_caster)->GetPlayerFrom();
	Player* pfirstTargetFrom=firstTarget->GetPlayerFrom();

	if (casterFrom != NULL && pfirstTargetFrom != NULL && casterFrom->GetGroup() == pfirstTargetFrom->GetGroup())
		RaidOnly = true;

	uint32 jumps=m_spellInfo->EffectChainTarget[i];

	//range
	range /= jumps; //hacky, needs better implementation!

	if((m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2]) && m_caster->IsUnit())
		SM_FIValue(((Unit *)m_caster)->SM_FAdditionalTargets, (int32*)&jumps, m_spellInfo);

	AddTarget(i, TargetType, firstTarget);

	if(jumps <= 1 || m_orderedObjects.size() == 0) //1 because we've added the first target, 0 size if spell is resisted
		return;

	std::set<Object*>::iterator itr;
	for( itr = firstTarget->GetInRangeSetBegin(); itr != firstTarget->GetInRangeSetEnd(); itr++ )
	{
		if( !(*itr)->IsUnit() || !((Unit*)(*itr))->isAlive())
			continue;

		if (RaidOnly && !pfirstTargetFrom->InRaid(static_cast<Unit*>(*itr)))
			continue;

		//healing spell, full health target = NONO
		if (IsHealingSpell(m_spellInfo) && static_cast<Unit*>(*itr)->GetHealthPct() == 100)
			continue;


		if(IsInrange(firstTarget->GetPositionX(), firstTarget->GetPositionY(), firstTarget->GetPositionZ(), (*itr), range))
		{
			AddTarget(i, TargetType, (*itr));
			if(t->size() >= jumps)
				return;
		}
	}
}

void Spell::AddPartyTargets(uint32 i, uint32 TargetType, float r, uint32 maxtargets)
{
	Object* u = m_targets.m_target? m_targets.m_target : m_caster;
	Player* p = u->GetPlayerFrom();	

	if(p == NULL) return;
	if(!m_caster->IsUnit()) return;

	AddTarget(i, TargetType, p);

	std::set<Object*>::iterator itr;
	for(itr = u->GetInRangeSetBegin(); itr != u->GetInRangeSetEnd(); itr++)
	{
		if (!(*itr)->IsUnit() || !static_cast<Unit*>(*itr)->isAlive())
			continue;

		//only affect players and pets
		if (!(*itr)->IsPlayer() && !(*itr)->IsPet())
			continue;

		if (!p->InParty(static_cast<Unit*>(*itr)))
			continue;

		if (u->CalcDistance(*itr) > r)
			continue;

		AddTarget(i, TargetType, (*itr));
	}
}

void Spell::AddRaidTargets(uint32 i, uint32 TargetType, float r, uint32 maxtargets, bool partylimit)
{
	Object* u = m_targets.m_target? m_targets.m_target : m_caster;
	Player* p = u->GetPlayerFrom();

	if(p == NULL ) return;
	if(!m_caster->IsUnit()) return;

	AddTarget(i, TargetType, p);

	std::set<Object*>::iterator itr;
	for(itr = u->GetInRangeSetBegin(); itr != u->GetInRangeSetEnd(); itr++)
	{
		if (!(*itr)->IsUnit() || !static_cast<Unit*>(*itr)->isAlive())
			continue;

		//only affect players and pets
		if (!(*itr)->IsPlayer() && !(*itr)->IsPet())
			continue;

		if (!p->InRaid(static_cast<Unit*>(*itr)))
			continue;

		if (u->CalcDistance(*itr) > r)
			continue;

		AddTarget(i, TargetType, (*itr));
	}
}

void Spell::AddAOETargets(uint32 i, uint32 TargetType, float r, uint32 maxtargets)
{
	if(m_spellInfo->spell_signature != _SPELL_SIGNATURE_)
	{
		sLog.outDetail("AddAOETargets: SpellInfo Leak! (Index: %u, TargetType: 0x%08X, maxtargets: %u)",i,TargetType,maxtargets);
		return;
	}

    if(m_caster->m_ConstanteId != _CONST_ID_) 
    {
     	Log.Error("HandleEffects","Leak m_caster!! <-- Report this to devs.");
    	return;
    }

	SpellTargetMap* t = &m_spellTargets;
	LocationVector source;

	//cant do raid/party stuff here, seperate functions for it
	if (TargetType & (SPELL_TARGET_AREA_PARTY | SPELL_TARGET_AREA_RAID) && !(!m_caster->IsPlayer() && !m_caster->IsPet() && (!m_caster->IsCreature() || !static_cast<Creature *>(m_caster)->IsTotem())))
		return;
	
	if (TargetType & SPELL_TARGET_AREA_SELF)
		source=m_caster->GetPosition();
	else if (TargetType & SPELL_TARGET_AREA_CURTARGET && m_targets.m_target != NULL)
		source=m_targets.m_target->GetPosition();
	else
	{
		m_targets.m_target = NULL; //visual fix
		m_targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;
		source.x=m_targets.m_destX;
		source.y=m_targets.m_destY;
		source.z=m_targets.m_destZ;
	}

	//caster might be in the aoe LOL
	if (m_caster->CalcDistance(source) <= r)
	{
		AddTarget(i, TargetType, m_caster);
	}

	float distclosest = 99999.9f;

	for (ObjectSet::iterator itr = m_caster->GetInRangeSetBegin(); itr!=m_caster->GetInRangeSetEnd(); ++itr)
	{
		if (maxtargets != 0 && t->size() >= maxtargets)
			break;

		float dist = (*itr)->CalcDistance(source);
		if (dist <= r)
		{
			if (AddTarget(i, TargetType, (*itr)) && dist < distclosest)
			{
				distclosest = dist;
				m_closestDestObject[i] = (*itr);
			}
		}
	}
}

bool Spell::AddTarget(uint32 i, uint32 TargetType, Object* obj)
{
	SpellTargetMap* t = &m_spellTargets;

	if (obj == NULL || !obj->IsInWorld())
		return false;

	if (m_spellScript != NULL && !m_spellScript->IsValidSpellTarget(obj))
		return false;

	//GO target, not item
	if ((TargetType & SPELL_TARGET_REQUIRE_GAMEOBJECT) && !(TargetType & SPELL_TARGET_REQUIRE_ITEM) && !obj->IsGO())
		return false;

	//target go, not able to target go
	if (obj->IsGO() && !(TargetType & SPELL_TARGET_REQUIRE_GAMEOBJECT) && !m_triggeredSpell)
		return false;
	//target item, not able to target item
	if (obj->IsItem() && !(TargetType & SPELL_TARGET_REQUIRE_ITEM) && !m_triggeredSpell)
		return false;

	if (m_caster->IsUnit() && (((Unit *)m_caster)->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9))
		                 && ((obj->IsPlayer() || obj->IsPet()) || (m_caster->IsPlayer() || m_caster->IsPet())))
		return false;

	if ((TargetType & SPELL_TARGET_REQUIRE_FRIENDLY) && !isFriendly(m_caster, obj))   	
		return false;
	if ((TargetType & SPELL_TARGET_REQUIRE_ATTACKABLE) && !isAttackable(m_caster, obj))   
		return false;
	if ((TargetType & SPELL_TARGET_OBJECT_SCRIPTED) && (m_spellScript == NULL || !m_spellScript->IsValidTarget(obj))) 
		return false;
	if ((TargetType & SPELL_TARGET_OBJECT_TARCLASS) && m_targets.m_target != NULL && m_targets.m_target->IsPlayer() && obj->IsPlayer() && ((Player *)m_caster)->getClass() != static_cast<Player *>(m_targets.m_target)->getClass())
		return false;
	if ((TargetType & SPELL_TARGET_OBJECT_CURPET) && !obj->IsPet())
		return false;
	if (TargetType & (SPELL_TARGET_AREA | SPELL_TARGET_AREA_SELF | SPELL_TARGET_AREA_CURTARGET | SPELL_TARGET_AREA_CONE | SPELL_TARGET_AREA_PARTY | SPELL_TARGET_AREA_RAID) && ((obj->IsUnit() && !static_cast<Unit *>(obj)->isAlive()) || (obj->IsCreature() && static_cast<Creature *>(obj)->IsTotem())))
		return false;

	//check if it hit on hostiles :P
	if (TargetType & SPELL_TARGET_REQUIRE_ATTACKABLE && obj->IsUnit())
	{
		uint8 hitresult=DidHit(i, static_cast<Unit*>(obj));
		if(hitresult != SPELL_DID_HIT_SUCCESS)
		{
			uint8 extended = 0;
			if (hitresult == SPELL_DID_HIT_REFLECT && m_caster->IsUnit())
			{
				//for checks
				/* OBSOLETE (Branruz)
				Unit* tmp = u_caster;
				u_caster = static_cast<Unit*>(obj);
				extended = DidHit(i, u_caster);
				u_caster = tmp;
				*/
				extended = DidHit(i, (Unit *)obj);
			}

			SpellTargetMap::iterator itr = m_spellTargets.find(obj);
			if (itr != m_spellTargets.end())
			{
				itr->second.HasEffect[i] = false;
			}
			else
			{
				SpellTargetEntry s;
				s.HasEffect[i] = false;
				s.TargetModType = hitresult;
				s.ExtendedTargetModType = extended;
				m_spellTargets.insert(std::pair<Object*, SpellTargetEntry>(obj, s));
				++m_objectsModerated;
			}
			return false;
		}
	}

	//final checks, require line of sight unless range/radius is 50000 yards // 1 yard = 0,9144 mètres
	SpellRange* r=dbcSpellRange.LookupEntry(m_spellInfo->rangeIndex);
	if (r->maxRange < 50000 && GetRadius(i) < 50000 && !obj->IsItem())
	{
		float x=m_caster->GetPositionX(), y=m_caster->GetPositionY(), z=m_caster->GetPositionZ() + 0.5;

		//are we using a different location?
		if (TargetType & SPELL_TARGET_AREA)
		{
			x = m_targets.m_destX;
			y = m_targets.m_destY;
			z = m_targets.m_destZ;
		}
		else if (TargetType & SPELL_TARGET_AREA_CHAIN)
		{
			Object* lasttarget = NULL;
			if (m_orderedObjects.size() > 0)
			{
				lasttarget=m_orderedObjects[m_orderedObjects.size() - 1];
				x = lasttarget->GetPositionX();
				y = lasttarget->GetPositionY();
				z = lasttarget->GetPositionZ();
			}
		}
#ifdef COLLISION
        float SourceTaille = ((Unit *)m_caster)->GetSize();
        float TargetTaille = ((Unit *)obj)->GetSize();
		if (!CollideInterface.CheckLOS(m_caster->GetMapId(), x, y, z + SourceTaille, //2, 
			           obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ() + TargetTaille)) //2))
			return false;
#endif
	}

	//ok, we try and find old entry, if not, put a new one in :)
	SpellTargetMap::iterator itr = m_spellTargets.find(obj);

	if (itr != m_spellTargets.end())
	{
		itr->second.HasEffect[i] = true;
		//make sure this proc data is set
		if (TargetType & SPELL_TARGET_REQUIRE_ATTACKABLE)
		{
			itr->second.ProcData |= PROC_ON_HARMFULSPELL_LAND_VICTIM;
			m_procdata |= PROC_ON_HARMFULSPELL_LAND;
		}
		else if (IsHealingSpell(m_spellInfo))
		{
			itr->second.ProcData |= PROC_ON_HEALSPELL_LAND_VICTIM;
			m_procdata |= PROC_ON_HEALSPELL_LAND;
		}
		if (obj->IsUnit())
			itr->second.EffectPoints[i] = CalculateEffect(i, static_cast<Unit *>(obj));
	}
	else
	{
		SpellTargetEntry s;
		s.HasEffect[i] = true;
		if (TargetType & SPELL_TARGET_REQUIRE_ATTACKABLE)
		{
			s.ProcData |= PROC_ON_HARMFULSPELL_LAND_VICTIM;
			m_procdata |= PROC_ON_HARMFULSPELL_LAND;
		}
		else if (IsHealingSpell(m_spellInfo))
		{
			s.ProcData |= PROC_ON_HEALSPELL_LAND_VICTIM;
			m_procdata |= PROC_ON_HEALSPELL_LAND;
		}
		if (obj->IsUnit())
			s.EffectPoints[i] = CalculateEffect(i, static_cast<Unit *>(obj));
		m_spellTargets.insert(std::pair<Object *, SpellTargetEntry>(obj, s));
		m_orderedObjects.push_back(obj);
		++m_objectsHit;
	}

	return true;
}

uint32 Spell::GetTargetType(uint32 value, uint32 i)
{
	uint32 type = 0;

	//OK, rewrite start!
	switch (value)
	{
		case EFF_TARGET_NONE: type |= SPELL_TARGET_REQUIRE_ITEM | SPELL_TARGET_REQUIRE_GAMEOBJECT;
			                  if (m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION) type |= SPELL_TARGET_AREA;
				              break;
   
		case EFF_TARGET_SELF              : type |= SPELL_TARGET_OBJECT_SELF; break;

		case EFF_TARGET_TELEPORT_LOCATION : type |= SPELL_TARGET_AREA; break;

        case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED             : // Pas de break (Brz)
		case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS: // Pas de break (Brz)					                          										                          
		
	    case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT               : type |= SPELL_TARGET_REQUIRE_ATTACKABLE; 
										            break;

        case EFF_TARGET_UNK4                      : type |= SPELL_TARGET_AREA_SELF; // Pas de break (Brz)
		case EFF_TARGET_FRIENDLY                  : type |= SPELL_TARGET_REQUIRE_FRIENDLY; break;

		case EFF_TARGET_LOCATION_TO_SUMMON        : // Pas de break (Brz)
		case EFF_TARGET_ALL_ENEMY_IN_AREA         : type |= SPELL_TARGET_AREA_SELF;
			                                        type |= SPELL_TARGET_REQUIRE_ATTACKABLE; break;

        case EFF_TARGET_PET_MASTER                : type |= SPELL_TARGET_OBJECT_PETOWNER; break;
		case EFF_TARGET_PET                       : type |= SPELL_TARGET_OBJECT_CURPET; break;

		
		case EFF_TARGET_TO_SELECT                 : // Pas de break (Brz)
		case EFF_TARGET_SINGLE_ENEMY              : if(m_caster->IsPlayer()) type |= SPELL_TARGET_PLAYER_SELECTION  ; // SPELL_TARGET_AREA_CURTARGET
			                                        else					 type |= SPELL_TARGET_REQUIRE_ATTACKABLE; 
													break;

		case EFF_TARGET_SCRIPTED_TARGET           : type |= SPELL_TARGET_OBJECT_SCRIPTED; break;
     
		case EFF_TARGET_ALL_PARTY_AROUND_CASTER  : type |= SPELL_TARGET_AREA_PARTY; break;

        case EFF_TARGET_SINGLE_FRIEND            : // Pas de break (Brz)
        case EFF_TARGET_ALL_FRIENDLY_IN_AREA     : type |= SPELL_TARGET_REQUIRE_FRIENDLY; break;		

		case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER: type |= SPELL_TARGET_AREA_SELF; break;

		case EFF_TARGET_GAMEOBJECT               : type |= SPELL_TARGET_REQUIRE_GAMEOBJECT; break;

		case EFF_TARGET_IN_FRONT_OF_CASTER       : type |= SPELL_TARGET_AREA_CONE;
			                                       type |= SPELL_TARGET_REQUIRE_ATTACKABLE; break;

		case EFF_TARGET_GAMEOBJECT_ITEM          : type |= SPELL_TARGET_REQUIRE_GAMEOBJECT;
			                                       type |= SPELL_TARGET_REQUIRE_ITEM; break;

		case EFF_TARGET_ALL_PARTY_IN_AREA_CHANNELED: type |= SPELL_TARGET_OBJECT_SELF;
			                                         type |= SPELL_TARGET_AREA_PARTY;
													 type |= SPELL_TARGET_AREA_SELF; break;

		case EFF_TARGET_ALL_TARGETABLE_AROUND_LOCATION_IN_RADIUS_OVER_TIME: type |= SPELL_TARGET_REQUIRE_FRIENDLY;
			                                                                type |= SPELL_TARGET_AREA; break; 
        case EFF_TARGET_MINION     : // Subordonné
        case EFF_TARGET_UNK64      : // Pas cherché de nom, seulement 8 spells (Brz)
		case EFF_TARGET_TOTEM_EARTH:
		case EFF_TARGET_TOTEM_WATER:
		case EFF_TARGET_TOTEM_AIR  :
		case EFF_TARGET_TOTEM_FIRE :	
		case EFF_TARGET_SELF_FISHING:
		case EFF_TARGET_DYNAMIC_OBJECT: // Mauvais nom, curieux (Brz)
		case EFF_TARGET_SUMMON_LOCATION:
		case EFF_TARGET_LOCATION_NEAR_CASTER:
		case EFF_TARGET_BEHIND_TARGET_LOCATION:
        case EFF_TARGET_BEHIND_TARGET_LOCATION2:
		case EFF_TARGET_BEHIND_TARGET_LOCATION3:
		case EFF_TARGET_NATURE_SUMMON_LOCATION:
        case EFF_TARGET_SCIPTED_OBJECT_LOCATION:
		case EFF_TARGET_MULTIPLE_SUMMON_LOCATION: 
		case EFF_TARGET_NETHETDRAKE_SUMMON_LOCATION:
		case EFF_TARGET_MULTIPLE_SUMMON_PET_LOCATION: 
		case EFF_TARGET_MULTIPLE_GUARDIAN_SUMMON_LOCATION: type |= SPELL_TARGET_OBJECT_SELF; break;

		case EFF_TARGET_ALL_PARTY_IN_AREA: type |= SPELL_TARGET_AREA_SELF;
			                               type |= SPELL_TARGET_AREA_PARTY; break;

		case EFF_TARGET_SINGLE_PARTY: type |= SPELL_TARGET_AREA_PARTY; break;

		case EFF_TARGET_PET_SUMMON_LOCATION: type |= SPELL_TARGET_OBJECT_SCRIPTED; break;

		case EFF_TARGET_ALL_PARTY: type |= SPELL_TARGET_AREA_SELF | SPELL_TARGET_AREA_PARTY | SPELL_TARGET_AREA_RAID; break;

		case EFF_TARGET_SCRIPTED_GAMEOBJECT: type |= SPELL_TARGET_OBJECT_SCRIPTED; break;

		case EFF_TARGET_CHAIN: type |= SPELL_TARGET_AREA_CHAIN;
			                   type |= SPELL_TARGET_REQUIRE_FRIENDLY; break;
		
		case EFF_TARGET_CURRENT_SELECTION: type |= SPELL_TARGET_AREA_CURTARGET;
			                               type |= SPELL_TARGET_REQUIRE_ATTACKABLE; break;

		case EFF_TARGET_TARGET_AT_ORIENTATION_TO_CASTER: type |= SPELL_TARGET_AREA_CONE;
			                                             type |= SPELL_TARGET_REQUIRE_ATTACKABLE; break;
        case EFF_TARGET_AoF: // Pas de break (Brz)
		case EFF_TARGET_AOE: type |= SPELL_TARGET_AREA_SELF;
			                 type |= SPELL_TARGET_AREA_RAID; break; //used by commanding shout, targets raid now (not only! - Brz)

		case EFF_TARGET_PARTY_MEMBER: type |= SPELL_TARGET_REQUIRE_FRIENDLY;
			                          type |= SPELL_TARGET_AREA_PARTY; break;

		case EFF_TARGET_AREAEFFECT_PARTY_AND_CLASS: type |= SPELL_TARGET_AREA_SELF;
			                                        type |= SPELL_TARGET_AREA_RAID;
													type |= SPELL_TARGET_OBJECT_TARCLASS;
													type |= SPELL_TARGET_REQUIRE_FRIENDLY; break;
		
		case EFF_TARGET_ENEMYS_IN_ARE_CHANNELED_WITH_EXCEPTIONS: type |= SPELL_TARGET_AREA;
			                                                     type |= SPELL_TARGET_REQUIRE_ATTACKABLE; break;

		case EFF_TARGET_SELECTED_ENEMY_CHANNELED: type |= SPELL_TARGET_REQUIRE_ATTACKABLE; break;

		case EFF_TARGET_SELECTED_ENEMY_DEADLY_POISON: type |= SPELL_TARGET_AREA_RANDOM; break;

		case EFF_TARGET_UNK87: type |= SPELL_TARGET_AREA; break;
		case EFF_TARGET_UNK90: type |= SPELL_TARGET_OBJECT_CURCRITTER; break;

        // Pas encore géré 
		// case EFF_TARGET_SCRIPTED_OR_SINGLE_TARGET
        // case EFF_TARGET_CALIRI_EGS 
		// case EFF_TARGET_LOCATION_INFRONT_CASTER
		// case EFF_TARGET_TARGET_FOR_VISUAL_EFFECT
		// case EFF_TARGET_SCRIPTED_TARGET2
		// case EFF_TARGET_FOOTMAN: pas sur sur la
		// case EFF_TARGET_LOCATION_INFRONT_CASTER_AT_RANGE
		// case EFF_TARGET_EVENTAIL_DE_COUTEAUX                      = 78 // 61739
		// case EFF_TARGET_UNK79  									= 79 // 48757 54994 61740 71077
		// case EFF_TARGET_UNK80										= 80, //targetted aoe summon
		// case EFF_TARGET_UNK81                                     = 81 
		// case EFF_TARGET_EVENTAIL_DE_COUTEAUX2                     = 82; // 61746
		// case EFF_TARGET_EVENTAIL_DE_COUTEAUX3                     = 83; // 61741
		// case EFF_TARGET_EVENTAIL_DE_COUTEAUX4                     = 84; // 61744
		// case EFF_TARGET_EVENTAIL_DE_COUTEAUX5                     = 85; // 61743 + 40967 
		// case EFF_TARGET_PESTE_EMETIQUE
        // case EFF_TARGET_SELECTED_ENEMY_DEADLY_POISON				= 86,
        // case EFF_TARGET_UNK87										= 87, // just seems to be a simple aoe target, with a little circle
        // case EFF_TARGET_UNK88										= 88, // even smaller aoe circle
        // case EFF_TARGET_LANCER_DES_TRUCS							= 89, // Lancer des trucs
        // case EFF_TARGET_UNK90										= 90, // target non-combat pet :P
        // case EFF_TARGET_LACHER_LE_COR                             = 91, // 56890 Lacher le Cor de fureur élémentaire
        // case EFF_TARGET_UNK92										= 92,
        // case EFF_TARGET_UNK94  									= 94,
        // case EFF_TARGET_UNK95  									= 95,
        // case EFF_TARGET_UNK96  									= 96,
        // case EFF_TARGET_UNK97 									= 97,
        // case EFF_TARGET_UNK99  									= 99,
        // case EFF_TARGET_UNK104  									= 104,
        // case EFF_TARGET_BLIZZARD 									= 106, // 70131 70598 Blizzard surpuissant - Fureur de Sindragosa
        // case EFF_TARGET_GGOODMANTEST2								= 108, // 23618 GGOODMANTEST2
        // case EFF_TARGET_BOIRE     								= 110,// 42436 49299 Boire - Racer Slam, find Target
	}

	//CHAIN SPELLS ALWAYS CHAIN!
	uint32 jumps = m_spellInfo->EffectChainTarget[i];
	
	if (m_caster->IsUnit()) SM_FIValue(((Unit *)m_caster)->SM_FAdditionalTargets,(int32*)&jumps,m_spellInfo);
	
	if (jumps != 0) type |= SPELL_TARGET_AREA_CHAIN;

	return type;
}

void Spell::GenerateTargets(SpellCastTargets* t)
{
	bool CheckInLOS = false;

	if(!m_caster->IsUnit()) return;

	// Le pnj n'a pas d'AIInterface (Brz)
	if(!m_caster->IsPlayer() && ((Unit *)m_caster)->GetAIInterface() == NULL) return;

	for (uint32 i=0; i<3; ++i)
	{
		if(m_spellInfo->Effect[i] == 0)
			continue;
		uint32 TargetType = 0;
		TargetType |= GetTargetType(m_spellInfo->EffectImplicitTargetA[i], i);

		//never get info from B if it is 0 :P
		if (m_spellInfo->EffectImplicitTargetB[i] != 0)
			TargetType |= GetTargetType(m_spellInfo->EffectImplicitTargetB[i], i);

		if (TargetType & (SPELL_TARGET_OBJECT_SELF | SPELL_TARGET_AREA_PARTY | SPELL_TARGET_AREA_RAID))
		{
			t->m_targetMask |= TARGET_FLAG_UNIT;
			t->m_target = m_caster;
		}
		
		if (TargetType & SPELL_TARGET_NO_OBJECT)
		{
			t->m_targetMask = TARGET_FLAG_SELF;
		}
		
		if(!(TargetType & (SPELL_TARGET_AREA | SPELL_TARGET_AREA_SELF | SPELL_TARGET_AREA_CURTARGET | SPELL_TARGET_AREA_CONE)))
		{
			if (TargetType & SPELL_TARGET_ANY_OBJECT)
			{
				if(m_caster->GetUInt64Value(UNIT_FIELD_TARGET))
				{
					//generate targets for things like arcane missiles trigger, tame pet, etc
					Object* target = m_caster->GetMapMgr()->_GetObject(m_caster->GetUInt64Value(UNIT_FIELD_TARGET));
					if(target != NULL)
					{
						if(target->IsUnit())
						{
							t->m_targetMask |= TARGET_FLAG_UNIT;
							t->m_target = target;
						}
						else if(target->IsGO())
						{
							t->m_targetMask |= TARGET_FLAG_OBJECT;
							t->m_target = target;
						}
					}
				}
			}
			
			if (TargetType & SPELL_TARGET_REQUIRE_ATTACKABLE)
			{		
				if (m_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT))
				{
					//generate targets for things like arcane missiles trigger, tame pet, etc
					Object *target = m_caster->GetMapMgr()->_GetObject(m_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT));
					if (target != NULL)
					{
						if (target->IsUnit())
						{
							t->m_targetMask |= TARGET_FLAG_UNIT;
							t->m_target = target;
						}
						else if (target->IsGO())
						{
							t->m_targetMask |= TARGET_FLAG_OBJECT;
							t->m_target = target;
						}
					}
				}
				else if (!m_caster->IsPlayer() && ((Unit *)m_caster)->GetAIInterface()->GetNextTarget() != NULL)
				{
					t->m_targetMask |= TARGET_FLAG_UNIT;
					t->m_target = ((Unit *)m_caster)->GetAIInterface()->GetNextTarget();
				}
				else if(m_caster->GetUInt64Value(UNIT_FIELD_TARGET))
				{
					//generate targets for things like arcane missiles trigger, tame pet, etc
					Object* target = m_caster->GetMapMgr()->_GetObject(m_caster->GetUInt64Value(UNIT_FIELD_TARGET));
					if(target != NULL)
					{
						if(target->IsUnit())
						{
							t->m_targetMask |= TARGET_FLAG_UNIT;
							t->m_target = target;
						}
						else if(target->IsGO())
						{
							t->m_targetMask |= TARGET_FLAG_OBJECT;
							t->m_target = target;
						}
					}
				}
				else
				{
					if (TargetType & SPELL_TARGET_PLAYER_SELECTION) // Player
					{ // Seul un player peut avoir ce flag. (Cf: GenerateTarget(...))
						Creature *target = ((Player *)m_caster)->GetMapMgr()->GetCreature( GET_LOWGUID_PART(((Player *)m_caster)->GetSelection()) );
						if (target != NULL)
						{
							t->m_targetMask |= TARGET_FLAG_DEST_LOCATION; // TARGET_FLAG_UNIT
							t->m_target = target;
							t->m_destX = target->GetPositionX();
							t->m_destY = target->GetPositionY();
							t->m_destZ = target->GetPositionZ()+target->GetSize();
						}
					}
					else
					{
						Unit *target = m_caster->GetMapMgr()->GetUnit(GetSinglePossibleEnemy(i));
						if (target != NULL)
						{
							t->m_targetMask |= TARGET_FLAG_UNIT;
							t->m_target = target;
						}
					}				
				}
			}
			
			if (TargetType & SPELL_TARGET_REQUIRE_FRIENDLY && !(TargetType & (SPELL_TARGET_AREA | SPELL_TARGET_AREA_SELF | SPELL_TARGET_AREA_CURTARGET | SPELL_TARGET_AREA_CONE)))
			{
				Unit *target = m_caster->GetMapMgr()->GetUnit(GetSinglePossibleFriend(i));
				if (target != NULL)
				{
					t->m_targetMask |= TARGET_FLAG_UNIT;
					t->m_target = target;
				}
				else
				{
					t->m_targetMask |= TARGET_FLAG_UNIT;
					t->m_target = m_caster;
				}
			}
		}
		
		if (TargetType & SPELL_TARGET_AREA_RANDOM)
		{
			//we always use radius(0) for some reason
			uint8 attempts = 0;
			do
			{
				//prevent deadlock
				++attempts;
				if (attempts > 10)
					break;

				float r = RandomFloat(GetRadius(0));
				float ang = RandomFloat(float(M_PI) * 2);
				t->m_destX = m_caster->GetPositionX() + (cosf(ang) * r);
				t->m_destY = m_caster->GetPositionY() + (sinf(ang) * r);
				t->m_destZ = CollideInterface.GetHeight(m_caster->GetMapId(), t->m_destX, t->m_destY, m_caster->GetPositionZ() + 2.0f);
				if (t->m_destZ == NO_WMO_HEIGHT)
					t->m_destZ = m_caster->GetMapMgr()->GetLandHeight(t->m_destX, t->m_destY, m_caster->GetPositionZ());
				t->m_targetMask |= TARGET_FLAG_DEST_LOCATION;

#ifdef COLLISION
                float SourceTaille = ((Unit *)m_caster)->GetSize();
                float TargetTaille = ((Unit *)t)->GetSize();
                CheckInLOS = CollideInterface.CheckLOS(m_caster->GetMapId(), 
					              m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ()+SourceTaille, 
								  t->m_destX, t->m_destY, t->m_destZ+TargetTaille);
#else
                CheckInLOS = false;
#endif
			} while (!CheckInLOS);
		}		
		else if (TargetType & SPELL_TARGET_AREA) //targetted aoe
		{
			//spells like blizzard, rain of fire
			if (m_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT))
			{
				Object *target = m_caster->GetMapMgr()->_GetObject(m_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT));
				t->m_targetMask |= TARGET_FLAG_DEST_LOCATION;
				t->m_destX = target->GetPositionX();
				t->m_destY = target->GetPositionY();
				t->m_destZ = target->GetPositionZ();
			}
			else
			{
				if (((Unit *)m_caster)->GetAIInterface()->GetNextTarget() != NULL)
				{
					t->m_targetMask |= TARGET_FLAG_DEST_LOCATION;
					t->m_destX = ((Unit *)m_caster)->GetAIInterface()->GetNextTarget()->GetPositionX();
					t->m_destY = ((Unit *)m_caster)->GetAIInterface()->GetNextTarget()->GetPositionY();
					t->m_destZ = ((Unit *)m_caster)->GetAIInterface()->GetNextTarget()->GetPositionZ();
				}
			}
		}
		if (TargetType & SPELL_TARGET_AREA_SELF)
		{
			t->m_targetMask |= TARGET_FLAG_SOURCE_LOCATION | TARGET_FLAG_DEST_LOCATION;
			t->m_target = m_caster;
			t->m_srcX = ((Unit *)m_caster)->GetPositionX();
			t->m_srcY = ((Unit *)m_caster)->GetPositionY();
			t->m_srcZ = ((Unit *)m_caster)->GetPositionZ();
			t->m_destX = ((Unit *)m_caster)->GetPositionX();
			t->m_destY = ((Unit *)m_caster)->GetPositionY();
			t->m_destZ = ((Unit *)m_caster)->GetPositionZ();

		}

		if (TargetType & SPELL_TARGET_AREA_CHAIN)
		{
			if (TargetType & SPELL_TARGET_REQUIRE_ATTACKABLE)
			{
				if (((Unit *)m_caster)->GetAIInterface()->GetNextTarget() != NULL)
				{
					t->m_targetMask |= TARGET_FLAG_UNIT;
					t->m_target = ((Unit *)m_caster)->GetAIInterface()->GetNextTarget();
				}
			}
			else
			{
				t->m_targetMask |= TARGET_FLAG_UNIT;
				t->m_target = m_caster;
			}
		}
		//target cone
		if (TargetType & SPELL_TARGET_AREA_CURTARGET)
		{
			if (((Unit *)m_caster)->GetAIInterface()->GetNextTarget() != NULL)
			{
				t->m_targetMask |= TARGET_FLAG_DEST_LOCATION;
				t->m_destX = ((Unit *)m_caster)->GetAIInterface()->GetNextTarget()->GetPositionX();
				t->m_destY = ((Unit *)m_caster)->GetAIInterface()->GetNextTarget()->GetPositionY();
				t->m_destZ = ((Unit *)m_caster)->GetAIInterface()->GetNextTarget()->GetPositionZ();
			}
		}

		if(TargetType & SPELL_TARGET_PLAYER_SELECTION) // Player
		{ 
			// Seul un player peut avoir ce flag. (Cf: GenerateTarget(...))
			Creature *target = ((Player *)m_caster)->GetMapMgr()->GetCreature( GET_LOWGUID_PART(((Player *)m_caster)->GetSelection()) );
			if (target != NULL)
			{
				t->m_targetMask |= TARGET_FLAG_DEST_LOCATION; // TARGET_FLAG_UNIT;
				t->m_target = target;
				t->m_destX = target->GetPositionX();
				t->m_destY = target->GetPositionY();
				t->m_destZ = target->GetPositionZ()+target->GetSize();
		   }
		}	
	}
}

void Spell::AddScriptedOrSpellFocusTargets(uint32 i, uint32 TargetType, float r, uint32 maxtargets)
{
	for(ObjectSet::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); ++itr)
	{
		Object* o = *itr;

		if(!o->IsGO())
			continue;

		GameObject* go = static_cast<GameObject*>(o);

		if(go->GetInfo()->spellFocus.focusId == m_spellInfo->requiresSpellFocus)
		{
			if(!m_caster->isInRange(go, r))
				continue;

			bool success = AddTarget(i, TargetType, go);
			if(success)
				return;
		}
	}
}

// returns Guid of lowest percentage health friendly party or raid target within sqrt('dist') yards
uint64 Spell::FindLowestHealthRaidMember(Player *Target, uint32 dist)
{

	if(!Target || !Target->IsInWorld())
		return 0;

	uint64 lowestHealthTarget = Target->GetGUID();
	uint32 lowestHealthPct = Target->GetHealthPct();
	Group *group = Target->GetGroup();
	if(group)
	{
		group->Lock();
		for(uint32 j = 0; j < group->GetSubGroupCount(); ++j) {
			for(GroupMembersSet::iterator itr = group->GetSubGroup(j)->GetGroupMembersBegin(); itr != group->GetSubGroup(j)->GetGroupMembersEnd(); ++itr)
			{
				if((*itr)->m_loggedInPlayer && Target->GetDistance2dSq((*itr)->m_loggedInPlayer) <= dist)
				{
					uint32 healthPct = (*itr)->m_loggedInPlayer->GetHealthPct();
					if(healthPct < lowestHealthPct)
					{
						lowestHealthPct = healthPct;
						lowestHealthTarget = (*itr)->m_loggedInPlayer->GetGUID();
					}
				}
			}
		}
		group->Unlock();
	}
	return lowestHealthTarget;
}
