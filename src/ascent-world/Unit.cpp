/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2009 Ascent Team <http://www.ascentemulator.net/>
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

Unit::Unit()
{
	
	for (uint32 i=0; i<NUM_POWER_TYPES; ++i)
	{
		m_ModInterrMRegenPCT[i] = 0;
		m_ModInterrMRegen[i] = 0;
	}

	for (uint32 i=0; i<NUM_DISPELS; ++i)
		m_dispelscount[i] = 0;

	m_healthfromspell = 0;
	m_manafromspell = 0;

	m_modifiersInherited = false;
	m_chain = NULL;
	m_sharedHPMaster = NULL;
	m_duelImmunity = false;
	m_Summon = NULL;
	m_lastHauntInitialDamage = 0;
	m_attackTimer = 0;
	m_attackTimer_1 = 0;
	m_duelWield = false;
	
	m_ignoreArmorPct = 0.0f;
	m_ignoreArmorPctMaceSpec = 0.0f;

	for(uint32 x=0;x<5;x++)
	{
		TotalStatModPct[x]=0;
		StatModPct[x]=0;
		FlatStatMod[x]=0;
		BaseStats[x]=0;
	}

	m_state = 0;
	m_special_state = 0;
	m_deathState = ALIVE;
	m_currentSpell = NULL;
	m_meleespell = 0;
	m_addDmgOnce = 0;
	
	for (uint32 i=0; i<NUM_SUMMON_SLOTS; ++i)
		m_summonslot[i] = NULL;

	m_ObjectSlots[0] = 0;
	m_ObjectSlots[1] = 0;
	m_ObjectSlots[2] = 0;
	m_ObjectSlots[3] = 0;
	m_silenced = 0;
	disarmed   = false;

	for (uint32 i=0; i<TOTAL_SPELL_AURAS; ++i)
		m_auracount[i] = 0;

	// Pet
	m_isPet = false;
	
	// Pet Talents
	m_PetTalentPointModifier = 0;
	
	//Vehicle
	m_teleportAckCounter = 0;
	m_inVehicleSeatId = 0xFF;
	m_CurrentVehicle = NULL;

	//DK:modifiers
	PctRegenModifier = 1.0;
	for( uint32 x = 0; x < 4; x++ )
	{
		PctPowerRegenModifier[x] = 1;
	}
	mana_reg=0;
	m_speedModifier = 0;
	m_slowdown = 0;
	m_mountedspeedModifier=0;
	m_maxSpeed = 0;
	for(uint32 x=0;x<27;x++)
	{
		MechanicsDispels[x]=0;
		MechanicsResistancesPCT[x]=0;
		ModDamageTakenByMechPCT[x]=0;
	}

	for (uint32 i=0; i<NUM_DISPELS; ++i)
		DispelResistancesPCT[i] = 0;

	//SM
	SM_CriticalChance=0;
	SM_FDur=0;//flat
	SM_PDur=0;//pct
	SM_FRadius=0;
	SM_FRange=0;
	SM_PCastTime=0;
	SM_FCastTime=0;
	SM_PCriticalDamage=0;
	SM_FDOT=0;
	SM_PDOT=0;
	SM_FEffectBonus=0;
	SM_PEffectBonus=0;
	SM_FDamageBonus=0;
	SM_PDamageBonus=0;
	SM_PSPELL_VALUE=0;
	SM_FSPELL_VALUE=0;
	SM_FHitchance=0;
	SM_PRange=0;//pct
	SM_PRadius=0;
	SM_PAPBonus=0;
	SM_PCost=0;
	SM_FCost=0;
	SM_FAdditionalTargets=0;
	SM_PJumpReduce=0;
	SM_FSpeedMod=0;
	SM_PNonInterrupt=0;
	SM_FPenalty=0;
	SM_PPenalty=0;
	SM_FCooldownTime = 0;
	SM_PCooldownTime = 0;
	SM_FChanceOfSuccess = 0;
	SM_FAmptitude = 0;
	SM_PAmptitude = 0;
	SM_FRezist_dispell = 0;
	SM_PRezist_dispell = 0;
	SM_PThreat_Reduction = 0;
	SM_FThreat_Reduction = 0;
	SM_FEffect1_Bonus = 0;
	SM_FEffect2_Bonus = 0;
	SM_FEffect3_Bonus = 0;
	SM_PEffect1_Bonus = 0;
	SM_PEffect2_Bonus = 0;
	SM_PEffect3_Bonus = 0;
	SM_FCharge_Bonus = 0;
	SM_PCharge_Bonus = 0;
	memset(SM, 0, 2*SPELL_MODIFIERS*sizeof(int32 *));

	m_interruptRegen = 0;
	m_resistChance = 0;
	m_powerRegenPCT = 0;
	RAPvModifier=0;
	APvModifier=0;
	stalkedby=0;

	m_extraattacks = 0;
	m_manashieldamt=0;
	m_triggerSpell = 0;
	m_triggerDamage = 0;
	m_canMove = 0;
	m_noInterrupt = 0;
	m_modlanguage = -1;
	
	critterPet = NULL;
	summonPet = NULL;

	for(uint32 x=0;x<10;x++)
	{
		dispels[x]=0;
		CreatureAttackPowerMod[x] = 0;
		CreatureRangedAttackPowerMod[x] = 0;
	}
	//REMIND:Update these if you make any changes
	CreatureAttackPowerMod[UNIT_TYPE_MISC] = 0;
	CreatureRangedAttackPowerMod[UNIT_TYPE_MISC] = 0;
	CreatureAttackPowerMod[11] = 0;
	CreatureRangedAttackPowerMod[11] = 0;

	m_invisFlag = INVIS_FLAG_NORMAL;

	for(int i = 0; i < INVIS_FLAG_TOTAL; i++)
	{
		m_invisDetect[i] = 0;
	}

	m_stealthLevel = 0;
	m_stealthDetectBonus = 0;
	m_stealth = 0;
	m_can_stealth = true;

	for(uint32 x=0;x<5;x++)
		BaseStats[x]=0;

	m_H_regenTimer = 2000;
	m_P_regenTimer = 2000;

	//	if(GetTypeId() == TYPEID_PLAYER) //only player for now
	//		CalculateActualArmor();

	m_aiInterface = new AIInterface();
	m_aiInterface->Init(this, AITYPE_AGRO, MOVEMENTTYPE_NONE);

	m_emoteState = 0;
	m_oldEmote = 0;	
	
	BaseDamage[0]=0;
	BaseOffhandDamage[0]=0;
	BaseRangedDamage[0]=0;
	BaseDamage[1]=0;
	BaseOffhandDamage[1]=0;
	BaseRangedDamage[1]=0;

	m_CombatUpdateTimer = 0;
	for(uint32 x=0;x<7;x++)
	{
		SchoolImmunityList[x] = 0;
		BaseResistance[x] = 0;
		HealDoneMod[x] = 0;
		HealDonePctMod[x] = 1.0f;
		HealTakenMod[x] = 0;
		HealTakenPctMod[x] = 1.0f;
		DamageTakenMod[x] = 0;
		DamageDoneModPCT[x]= 1;
		SchoolCastPrevent[x]=0;
		DamageTakenPctMod[x] = 1;
		SpellCritChanceSchool[x] = 0;
		PowerCostMod[x] = 0;
		Penetration[x] = 0; // armor penetration & spell penetration
		AttackerCritChanceMod[x]=0;
		CritMeleeDamageTakenPctMod[x]=0;
		CritRangedDamageTakenPctMod[x]=0;
	}
	DamageTakenPctModOnHP35 = 1;
	RangedDamageTaken = 0;
	AOEDmgMod = 1.0f;

	for(int i = 0; i < 5; i++)
	{
		m_detectRangeGUID[i] = 0;
		m_detectRangeMOD[i] = 0;
	}

	trackStealth = false;

	m_threatModifyer = 0;
	m_generatedThreatModifyer = 0;
	//memset(m_auras, 0, (MAX_AURAS+MAX_PASSIVE_AURAS)*sizeof(Aura*));
	memset(m_auras, NULL, sizeof(m_auras)); //OpenAscent 3.22
	
	// diminishing return stuff
	memset(m_diminishAuraCount, 0, 23);
	memset(m_diminishCount, 0, 23 * sizeof(uint16));
	memset(m_diminishTimer, 0, 23 * sizeof(uint16));
	m_diminishActive = false;
	dynObj = NULL;
	pLastSpell = 0;
	m_flyspeedModifier = 0;
	bInvincible = false;
	m_redirectSpellPackets = NULL;
	can_parry = false;
	bProcInUse = false;
	spellcritperc = 0;

	polySpell = 0;
	RangedDamageTaken = 0;
	m_procCounter = 0;
	m_extrastriketargets = 0;
	m_damgeShieldsInUse = false;
//	fearSpell = 0;
	m_extraAttackCounter = false;
	mRecentlyBandaged = false;
	
	mThreatRTarget = NULL;
	mThreatRAmount = 0;
	
	m_soulSiphon.amt = 0;
	m_soulSiphon.max = 0;
	
	m_frozenTargetCharges = 0;
	m_frozenTargetId = 0;
	m_incanterAbsorption = 0;
	
	trigger_on_stun = 0;
	trigger_on_stun_chance = 100;
	trigger_on_chill = 0;
	trigger_on_chill_chance = 100;
	
	
	
	CombatStatus.SetUnit(this);
    
	// En commantaire pour l'instant (Branruz)
	// A quoi sert cette var ?? 
	// m_temp_summon=false;

	m_chargeSpellsInUse=false;
	m_spellsbusy=false;
	m_interruptedRegenTime = 0;
	m_hasVampiricEmbrace = m_hasVampiricTouch = 0;

	//lets clear our containers
	m_chargeSpells.clear();
	m_procSpells.clear();
	m_damageSplitTargets.clear();
	m_damageShields.clear();

	BeaconCaster = NULL;
	BeaconTarget = NULL;
	m_LastSpellManaCost = 0;

	CurrentPathFinding = NULL; // PathFinding

    m_speaker = false;         // Le PNJ ne parle pas
	m_AI_Speak = NULL;         // Pas d'interface de parole
	m_useAI = false;

	for (uint32 x = 0;x<NUM_CUSTOM_TIMERS;x++)
	{
		m_CustomTimers[x] = 0;
	}
}

void Unit::Init()
{
	Object::Init();

	if (m_aiInterface != NULL) m_aiInterface->Init(this, AITYPE_AGRO, MOVEMENTTYPE_NONE);
	CombatStatus.SetUnit(this);
	SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER );
	sEventMgr.AddEvent(this, &Unit::SetInitialFactionStuff, EVENT_INITIAL_FACTION_EVENT, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

Unit::~Unit()
{
	RemoveAllAuras();

	if (m_chain)
		m_chain->RemoveUnit(this);

	if (m_sharedHPMaster)
	{
		m_sharedHPMaster->m_sharedHPUnits.erase(this);
		m_sharedHPMaster = NULL;
	}

	if (IsInWorld())
		RemoveFromWorld(true);

	if (!m_modifiersInherited)
	{
		if(SM_CriticalChance != 0) { delete [] SM_CriticalChance; SM_CriticalChance = NULL; }
		if(SM_FDur != 0)           { delete [] SM_FDur ; SM_FDur = NULL; }
		if(SM_PDur != 0)           { delete [] SM_PDur ; SM_PDur = NULL; }
		if(SM_FRadius != 0)        { delete [] SM_FRadius ; SM_FRadius = NULL; }
		if(SM_FRange != 0) { delete [] SM_FRange ; SM_FRange = NULL; }
		if(SM_PCastTime != 0) { delete [] SM_PCastTime ; SM_PCastTime = NULL; }
		if(SM_FCastTime != 0) { delete [] SM_FCastTime ; SM_FCastTime = NULL; }
		if(SM_PCriticalDamage != 0) { delete [] SM_PCriticalDamage ; SM_PCriticalDamage = NULL; }
		if(SM_FDOT != 0) { delete [] SM_FDOT ; SM_FDOT = NULL; }
		if(SM_PDOT != 0) { delete [] SM_PDOT ; SM_PDOT = NULL; }
		if(SM_PEffectBonus != 0) { delete [] SM_PEffectBonus ; SM_PEffectBonus = NULL; }
		if(SM_FEffectBonus != 0) { delete [] SM_FEffectBonus ; SM_FEffectBonus = NULL; }
		if(SM_FDamageBonus != 0) { delete [] SM_FDamageBonus ; SM_FDamageBonus = NULL; }
		if(SM_PDamageBonus != 0) { delete [] SM_PDamageBonus ; SM_PDamageBonus = NULL; } 
		if(SM_PSPELL_VALUE != 0) { delete [] SM_PSPELL_VALUE ; SM_PSPELL_VALUE = NULL; }
		if(SM_FSPELL_VALUE != 0) { delete [] SM_FSPELL_VALUE ; SM_FSPELL_VALUE = NULL; }
		if(SM_FHitchance != 0) { delete [] SM_FHitchance ; SM_FHitchance = NULL; }
		if(SM_PRange != 0) { delete [] SM_PRange ; SM_PRange = NULL; }
		if(SM_PRadius != 0) { delete [] SM_PRadius ; SM_PRadius = NULL; }
		if(SM_PAPBonus != 0) { delete [] SM_PAPBonus ; SM_PAPBonus = NULL; }
		if(SM_PCost != 0) { delete [] SM_PCost ; SM_PCost = NULL; }
		if(SM_FCost != 0) { delete [] SM_FCost ; SM_FCost = NULL; }
		if(SM_FAdditionalTargets != 0) { delete [] SM_FAdditionalTargets ; SM_FAdditionalTargets = NULL; }
		if(SM_PJumpReduce != 0) { delete [] SM_PJumpReduce ; SM_PJumpReduce = NULL; }
		if(SM_FSpeedMod != 0) { delete [] SM_FSpeedMod ; SM_FSpeedMod = NULL; }
		if(SM_PNonInterrupt != 0) { delete [] SM_PNonInterrupt ; SM_PNonInterrupt = NULL; }
		if(SM_FPenalty != 0) { delete [] SM_FPenalty ; SM_FPenalty = NULL; }
		if(SM_PPenalty != 0) { delete [] SM_PPenalty ; SM_PPenalty = NULL; }
		if(SM_FCooldownTime != 0) { delete [] SM_FCooldownTime ; SM_FCooldownTime = NULL; }
		if(SM_PCooldownTime != 0) { delete [] SM_PCooldownTime ; SM_PCooldownTime = NULL; }
		if(SM_FChanceOfSuccess != 0) { delete [] SM_FChanceOfSuccess ; SM_FChanceOfSuccess = NULL; }
		if(SM_FAmptitude != 0) { delete [] SM_FAmptitude ; SM_FAmptitude = NULL; }
		if(SM_PAmptitude != 0) { delete [] SM_PAmptitude ; SM_PAmptitude = NULL; }
		if(SM_FRezist_dispell != 0) { delete[] SM_FRezist_dispell ; SM_FRezist_dispell = NULL; }
		if(SM_PRezist_dispell != 0) { delete[] SM_PRezist_dispell ; SM_PRezist_dispell = NULL; }
		if(SM_PThreat_Reduction != 0) { delete[] SM_PThreat_Reduction; SM_PThreat_Reduction = NULL; }
		if(SM_FThreat_Reduction != 0) { delete[] SM_FThreat_Reduction; SM_FThreat_Reduction = NULL; }
		if(SM_FEffect1_Bonus != 0) { delete[] SM_FEffect1_Bonus; SM_FEffect1_Bonus = NULL; }
		if(SM_FEffect2_Bonus != 0) { delete[] SM_FEffect2_Bonus; SM_FEffect2_Bonus = NULL; }
		if(SM_FEffect2_Bonus != 0) { delete[] SM_FEffect2_Bonus; SM_FEffect2_Bonus = NULL; }
		if(SM_PEffect1_Bonus != 0) { delete[] SM_PEffect1_Bonus; SM_PEffect1_Bonus = NULL; }
		if(SM_PEffect2_Bonus != 0) { delete[] SM_PEffect2_Bonus; SM_PEffect2_Bonus = NULL; }
		if(SM_PEffect3_Bonus != 0) { delete[] SM_PEffect3_Bonus; SM_PEffect3_Bonus = NULL; }
		if(SM_FCharge_Bonus != 0)  { delete[] SM_FCharge_Bonus; SM_FCharge_Bonus = NULL; }
		if(SM_PCharge_Bonus != 0)  { delete[] SM_PCharge_Bonus; SM_PCharge_Bonus = NULL; }
	}

	delete m_aiInterface;

    if(m_speaker)
	{
	 if(m_AI_Speak) delete m_AI_Speak;
	 m_AI_Speak = NULL;
	 m_speaker = false;
	}

	/*for(int i = 0; i < 4; i++)
		if(m_ObjectSlots[i])
			delete m_ObjectSlots[i];*/

	if(m_currentSpell != NULL)
		m_currentSpell->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"

	if(m_Summon)
		m_Summon->ClearPetOwner();

	//remove guardians
	for (std::set<Creature*>::iterator itr=m_summonedCreatures.begin(); itr!=m_summonedCreatures.end(); ++itr)
		(*itr)->SafeDelete();
	m_summonedCreatures.clear();

	for (std::map<uint32, Aura*>::iterator itr=tmpAura.begin(); itr!=tmpAura.end(); ++itr)
	{
		itr->second->m_deletedfromtmp = true;
		itr->second->Remove();
	}
	tmpAura.clear();

	for (std::set<Unit*>::iterator itr=m_sharedHPUnits.begin(); itr!=m_sharedHPUnits.end(); ++itr)
		(*itr)->m_sharedHPMaster = NULL;
	m_sharedHPUnits.clear();

	for (std::multimap<uint32, Aura*>::iterator itr=m_aurascast.begin(); itr!=m_aurascast.end(); ++itr)
	{
		//remove the reference to the caster!
		itr->second->m_caster = NULL;
		if (itr->second->m_spellProto->UniqueTargetBased & 2) //1 target at a time
			sEventMgr.AddEvent(itr->second, &Aura::Remove, EVENT_AURA_REMOVE, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT | EVENT_FLAG_DELETES_OBJECT);
	}

	m_aurascast.clear();

	for (std::list<ReflectSpellSchool*>::iterator itr=m_reflectSpellSchool.begin(); itr!=m_reflectSpellSchool.end(); ++itr)
		delete (*itr);
	m_reflectSpellSchool.clear();

	for (HM_NAMESPACE::hash_multimap<uint32, ProcFnc*>::iterator itr=m_procfuncs.begin(); itr!=m_procfuncs.end(); ++itr)
		delete itr->second;
	m_procfuncs.clear();

	if (m_chain != NULL)
	{
		m_chain->RemoveUnit(static_cast<Unit *>(this));
		m_chain = NULL;
	}

	if (m_sharedHPMaster != NULL)
	{
		m_sharedHPMaster->m_sharedHPUnits.erase(static_cast<Unit *>(this));
		m_sharedHPMaster = NULL;
	}

	for(uint32 x=0; x<NUM_SUMMON_SLOTS; ++x)
	{
		if(m_summonslot[x] != NULL)
			m_summonslot[x]->SafeDelete();
	}

	//vanish to clear refs
	CombatStatus.Vanished();
	CombatStatus.SetUnit(NULL);

	//lets clear our containers
	m_chargeSpells.clear();
	m_procSpells.clear();
	m_damageSplitTargets.clear();
	m_damageShields.clear();
	
	DamageTakenPctModPerCaster.clear();

}

void Unit::Update( uint32 p_time )
{
	UpdateOppFactionSet();
	_UpdateSpells( p_time );

    HM_NAMESPACE::hash_multimap<uint32, ProcFnc*>::iterator itr, itr2 = m_procfuncs.begin();

	for (itr=m_procfuncs.begin(); itr!=m_procfuncs.end(); ++itr)
	{
		itr = itr2++;
		if (itr->second->deleted)
		{
			delete itr->second;
			m_procfuncs.erase(itr);
		}
	}

	if(!isDead())
	{
		//-----------------------POWER & HP REGENERATION-----------------
/* Please dont do temp fixes. Better report to me. Thx. Shady */
        if( p_time >= m_H_regenTimer ) RegenerateHealth();
	    else                           m_H_regenTimer -= p_time;

		if( p_time >= m_P_regenTimer )
		{
			RegeneratePower( false );
			m_interruptedRegenTime=0;
		}
		else
		{
			m_P_regenTimer -= p_time;
			if (m_interruptedRegenTime)
			{
				if(p_time>=m_interruptedRegenTime)
					RegeneratePower( true );
				else
					m_interruptedRegenTime -= p_time;
			}
		}


		if(m_aiInterface != NULL && m_useAI) m_aiInterface->Update(p_time);

		if(m_diminishActive)
		{
			uint32 count = 0;
			for(uint32 x = 0; x < 16; ++x)
			{
				// diminishing return stuff
				if(m_diminishTimer[x] && !m_diminishAuraCount[x])
				{
					if(p_time >= m_diminishTimer[x])
					{
						// resetting after 15 sec
						m_diminishTimer[x] = 0;
						m_diminishCount[x] = 0;
					}
					else
					{
						// reducing, still.
						m_diminishTimer[x] -= p_time;
						++count;
					}
				}
			}
			if(!count)
				m_diminishActive = false;
		}

/*		//if health changed since last time. Would be perfect if it would work for creatures too :)
		if(m_updateMask.GetBit(UNIT_FIELD_HEALTH))
			EventHealthChangeSinceLastUpdate();*/
	}
}


bool Unit::canReachWithAttack(Unit *pVictim)
{
	float targetscale = 0;
	float selfscale = 0;
//	float targetreach = pVictim->GetFloatValue(UNIT_FIELD_COMBATREACH);
	float selfreach = m_floatValues[UNIT_FIELD_COMBATREACH];
	float targetradius = pVictim->m_floatValues[UNIT_FIELD_BOUNDINGRADIUS];
	float selfradius = m_floatValues[UNIT_FIELD_BOUNDINGRADIUS];

	if( GetMapId() != pVictim->GetMapId() ) return false;

	if(pVictim->IsCreature())
	{
		targetscale = (((Creature *)this)->Taille);
		selfscale = (((Player *)this)->Taille);
	}
	else if(pVictim->IsPlayer())
	{
		targetscale = (((Player *)this)->Taille);
		selfscale = (((Creature *)this)->Taille);
	}	

	

	float distance = CalcDistance(pVictim);
	float attackreach = (((targetradius*targetscale) + selfreach) + (((selfradius*selfradius)*selfscale)+1.50f));

	//formula adjustment for player side.
	if( IsPlayer() )
	{
		if( attackreach <= 8 && attackreach >= 5 && targetradius >= 1.80f)
			attackreach = 11; //giant type units

		if( attackreach > 11)
			attackreach = 11; //distance limited to max 11 yards attack range //max attack distance

		if( attackreach < 5 )
			attackreach = 5; //normal units with too small reach.

		//range can not be less than 5 yards - this is normal combat range, SCALE IS NOT SIZE

		// latency compensation!!
		// figure out how much extra distance we need to allow for based on our movespeed and latency.
		if( pVictim->IsPlayer() && static_cast< Player* >( pVictim )->m_isMoving )
		{
			// this only applies to PvP.
			uint32 lat = static_cast< Player* >( pVictim )->GetSession() ? static_cast< Player* >( pVictim )->GetSession()->GetLatency() : 0;

			// if we're over 500 get fucked anyway.. your gonna lag! and this stops cheaters too
			lat = ( lat > 250 ) ? 250 : lat;

			// calculate the added distance
			attackreach += ( m_runSpeed * 0.001f ) * (float( lat ) * 1.5f);
		}
	}
	return ( distance <= attackreach );
}

void Unit::GiveGroupXP(Unit *pVictim, Player *PlayerInGroup)
{
	if(!PlayerInGroup) 
		return;
	if(!pVictim) 
		return;
	if(!PlayerInGroup->InGroup()) 
		return;
	Group *pGroup = PlayerInGroup->GetGroup();
	uint32 xp = 0;
	if(!pGroup) 
		return;

	//Get Highest Level Player, Calc Xp and give it to each group member
	Player *pHighLvlPlayer = NULL;
	Player *pGroupGuy = NULL;
	  int active_player_count=0;
	Player *active_player_list[MAX_GROUP_SIZE_RAID];//since group is small we can afford to do this ratehr then recheck again the whole active player set
	int total_level=0;
	float xp_mod = 1.0f;

/*	if(pGroup->GetGroupType() == GROUP_TYPE_RAID)
	{   //needs to change
		//Calc XP
		xp = CalculateXpToGive(pVictim, PlayerInGroup);
		xp /= pGroup->MemberCount();

		GroupMembersSet::iterator itr;
		for(uint32 i = 0; i < pGroup->GetSubGroupCount(); i++)
		{
			for(itr = pGroup->GetSubGroup(i)->GetGroupMembersBegin(); itr != pGroup->GetSubGroup(i)->GetGroupMembersEnd(); ++itr)
			{
				if((*itr)->getLevel() < sWorld.LevelCap)
					(*itr)->GiveXP(xp, pVictim->GetGUID(), true);
			}
		}
	}
	else if(pGroup->GetGroupType() == GROUP_TYPE_PARTY) */
	//change on 2007 04 22 by Zack
	//we only take into count players that are near us, on same map
	GroupMembersSet::iterator itr;
	pGroup->Lock();
	for(uint32 i = 0; i < pGroup->GetSubGroupCount(); i++) {
		for(itr = pGroup->GetSubGroup(i)->GetGroupMembersBegin(); itr != pGroup->GetSubGroup(i)->GetGroupMembersEnd(); ++itr)
		{
			pGroupGuy = (*itr)->m_loggedInPlayer;
			if( pGroupGuy && 
				pGroupGuy->isAlive() && 
//				PlayerInGroup->GetInstanceID()==pGroupGuy->GetInstanceID() &&
				pVictim->GetMapMgr() == pGroupGuy->GetMapMgr() && 
				pGroupGuy->GetDistanceSq(pVictim)<100*100
				)
			{
				active_player_list[active_player_count]=pGroupGuy;
				active_player_count++;
				total_level += pGroupGuy->getLevel();
				if(pHighLvlPlayer)
				{
					if(pGroupGuy->getLevel() > pHighLvlPlayer->getLevel())
						pHighLvlPlayer = pGroupGuy;
				}
				else 
					pHighLvlPlayer = pGroupGuy;
			}
		}
	}
	pGroup->Unlock();
	if(active_player_count<1) //killer is always close to the victim. This should never execute
	{
		if(PlayerInGroup == 0)
		{
			PlayerInfo * pleaderinfo = pGroup->GetLeader();
			if(!pleaderinfo->m_loggedInPlayer)
				return;

			PlayerInGroup = pleaderinfo->m_loggedInPlayer;
		}

		xp = CalculateXpToGive(pVictim, PlayerInGroup);
		PlayerInGroup->GiveXP(xp, pVictim->GetGUID(), true);
	}
	else
	{
		if( pGroup->GetGroupType() == GROUP_TYPE_PARTY)
		{
			if(active_player_count==3)
				xp_mod=1.1666f;
			else if(active_player_count==4)
				xp_mod=1.3f;
			else if(active_player_count==5)
				xp_mod=1.4f;
			else xp_mod=1;//in case we have only 2 members ;)
		}
		else if(pGroup->GetGroupType() == GROUP_TYPE_RAID)
			xp_mod=0.5f;

		if(pHighLvlPlayer == 0)
		{
			PlayerInfo * pleaderinfo = pGroup->GetLeader();
			if(!pleaderinfo->m_loggedInPlayer)
				return;

			pHighLvlPlayer = pleaderinfo->m_loggedInPlayer;
		}

		xp = CalculateXpToGive(pVictim, pHighLvlPlayer);
		//i'm not sure about this formula is correct or not. Maybe some brackets are wrong placed ?
		for(int i=0;i<active_player_count;i++)
			active_player_list[i]->GiveXP( float2int32(((xp*active_player_list[i]->getLevel()) / total_level)*xp_mod), pVictim->GetGUID(), true );
	}
}

void Unit::HandleProc( uint32 flag, Unit* victim, SpellEntry* CastingSpell, uint32 dmg, uint32 abs , bool mainhand, uint32 weapon_damage_type )
{
	++m_procCounter;
	bool can_delete = !bProcInUse; //if this is a nested proc then we should have this set to TRUE by the father proc
	bProcInUse = true; //locking the proc list

	std::list<uint32> remove;
	std::list<struct ProcTriggerSpell>::iterator itr,itr2;
	for( itr = m_procSpells.begin(); itr != m_procSpells.end(); )  // Proc Trigger Spells for Victim
	{
		itr2 = itr;
		++itr;
		if( itr2->deleted )
		{
			if( can_delete )
				m_procSpells.erase( itr2 );
			continue;
		}

		SpellEntry* sp = dbcSpell.LookupEntry( itr2->spellId );
		SpellEntry* ospinfo = dbcSpell.LookupEntry( itr2->origId );//no need to check if exists or not since we were not able to register this trigger if it would not exist :P

		if (sp == NULL)
			continue;

		//required flags, this is gonna be crap but its required :(
		//dont let crit attacks proc without right flags
		/*if ((itr2->procFlags & PROC_ON_CRIT_ATTACK) && !(flag & PROC_ON_CRIT_ATTACK))
			continue;
		if ((itr2->procFlags & PROC_ON_CRIT_ATTACK_VICTIM) && !(flag & PROC_ON_CRIT_ATTACK_VICTIM))
			continue;*/

			if(itr2->weapon_damage_type > 0 && itr2->weapon_damage_type < 3 &&
				(itr2->procFlags & (PROC_ON_MELEE_HIT | PROC_ON_CRIT_ATTACK)) &&
				itr2->weapon_damage_type != weapon_damage_type)
				continue; // This spell should proc only from other hand attacks

		//do we require honorable or experiance kill?
		if ((itr2->procFlags & PROC_ON_XP_OR_HONOR_KILL ) && !(flag & PROC_ON_XP_OR_HONOR_KILL ))
			continue;

		uint32 origId = itr2->origId;

			//Handle target triggers
			//we need to have a target trigger aura apply name
		if (ospinfo->EffectApplyAuraName[0] == SPELL_AURA_ADD_TARGET_TRIGGER || ospinfo->EffectApplyAuraName[1] == SPELL_AURA_ADD_TARGET_TRIGGER || ospinfo->EffectApplyAuraName[2] == SPELL_AURA_ADD_TARGET_TRIGGER)
		{
			bool invalid = true;

			if (itr2->index < 3) // if (itr2->index != 0xFF)
			{
				if(ospinfo->EffectApplyAuraName[itr2->index] == SPELL_AURA_ADD_TARGET_TRIGGER)
			    {
				 //THIS IS SLOW
				 for (uint32 bit=0; bit<SPELL_GROUPS; ++bit)
				 {
					if (HasSpellMaskBit(bit, itr2->index, ospinfo) && HasGroupBit(bit, CastingSpell))
					{
						invalid = false;
						break;
					}
				 }
				}
			}
			if (invalid)
				continue;
		}
			
		if( CastingSpell != NULL )
		{
			//this is to avoid spell proc on spellcast loop. We use dummy that is same for both spells
			//if( CastingSpell->Id == itr2->spellId )
			if( CastingSpell->Id == itr2->origId || CastingSpell->Id == itr2->spellId )
			{
				//printf("WOULD CRASH HERE ON PROC: CastingId: %u, OrigId: %u, SpellId: %u\n", CastingSpell->Id, itr2->origId, itr2->spellId);
				continue;
			}
		}
		
		//this requires some specific spell check,not yet implemented
		if(itr2->index < 3) // if(itr2->index != 0xFF)
		{
		 if( itr2->procFlags & flag || 
			 ( (itr2->procFlags == 0) && 
			   (itr2->index < 3) && 
			   (ospinfo->EffectApplyAuraName[itr2->index] == SPELL_AURA_ADD_TARGET_TRIGGER) && 
			   (m_procCounter < 2) ) ) // (m_procCounter == 1) ) //only allow 1 proc at a time :P
		 {
			uint32 spellId = itr2->spellId;
			if( itr2->procFlags & PROC_ON_HARMFULSPELL_LAND  )
			{

				if( CastingSpell == NULL )
					continue;
				
				//this is wrong, dummy is too common to be based on this, we should use spellgroup or something
				SpellEntry* sp = dbcSpell.LookupEntry( spellId );
				if( sp->dummy != CastingSpell->dummy )
				{
					if( !ospinfo->School )
						continue;
					if( ospinfo->School != CastingSpell->School )
						continue;
					if( CastingSpell->EffectImplicitTargetA[0] == 1 || 
						CastingSpell->EffectImplicitTargetA[1] == 1 || 
						CastingSpell->EffectImplicitTargetA[2] == 1) //Prevents school based procs affecting caster when self buffing
						continue;
				}
				else
					if( sp->dummy == 1 )
						continue;
			}
			// handle "equipped item class/subclass"
			// kinda hacky.
			if( IsPlayer() )
			{
				if( ospinfo->EquippedItemClass && ospinfo->EquippedItemSubClass )
				{
					Item *itm = static_cast<Player*>(this)->GetItemInterface()->GetInventoryItem( (mainhand) ? EQUIPMENT_SLOT_MAINHAND : EQUIPMENT_SLOT_OFFHAND );
					if(itm != NULL)
					{
						if( (int32)itm->GetProto()->Class != ospinfo->EquippedItemClass || !(( 1 << itm->GetProto()->SubClass ) & ospinfo->EquippedItemSubClass) )
							continue; // not applicable
					}
				}
			}
			uint32 proc_Chance = itr2->procChance;
			SpellEntry* spe  = dbcSpell.LookupEntry( spellId );

			// feral = no procs (need a better way to do this)
			/*if( this->IsPlayer() && static_cast<Player*>(this)->GetShapeShift() )
			{
				if( spe->NameHash == SPELL_HASH_LIGHTNING_SPEED ) // mongoose
					continue;
				else if( spe->NameHash == SPELL_HASH_HOLY_STRENGTH ) //crusader
					continue;
			}*/
			//Custom procchance modifications based on equipped weapon speed.
			if( IsPlayer() && ospinfo != NULL && ospinfo->ProcsPerMinute > 0.0f )
			{
				float ppm = ospinfo->ProcsPerMinute;

				Player *plr = static_cast< Player* >( this );
				Item *weapon = NULL;
				if(plr->GetItemInterface() && weapon_damage_type > 0 && weapon_damage_type < 3)
				{
						weapon = plr->GetItemInterface()->
							GetInventoryItem( EQUIPMENT_SLOT_MAINHAND + weapon_damage_type - 1 );
				}
				if(weapon && weapon->GetProto())
				{
					float speed = float( weapon->GetProto()->Delay );
					proc_Chance = float2int32( speed * 0.001f * ppm / 0.6f );
				}

			}

			if( this->IsPlayer() && spe != NULL && (
				spe->NameHash == SPELL_HASH_MAGTHERIDON_MELEE_TRINKET || 
				spe->NameHash == SPELL_HASH_ROMULO_S_POISON || 
				spe->NameHash == SPELL_HASH_BLACK_TEMPLE_MELEE_TRINKET || spellId == 16870 ) )
			{
				float ppm = 1.0f;
				switch( spe->NameHash )
				{
					case SPELL_HASH_MAGTHERIDON_MELEE_TRINKET:
						ppm = 1.5f;
						break; // dragonspine trophy
					case SPELL_HASH_ROMULO_S_POISON:
						ppm = 1.5f;
						break; // romulo's
					case SPELL_HASH_BLACK_TEMPLE_MELEE_TRINKET:
						ppm = 1.0f;
						break; // madness of the betrayer
				}
				switch( spellId )
				{
					case 16870:
						ppm = 2.0f;
						break; //druid: clearcasting
				}

				Item * mh = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
				Item * of = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
				
				if( mh != NULL && of != NULL )
				{
					float mhs = float( mh->GetProto()->Delay );
					float ohs = float( of->GetProto()->Delay );
					proc_Chance = FL2UINT( ( mhs + ohs ) * 0.001f * ppm / 0.6f );
				}
				else if( mh != NULL )
				{
					float mhs = float( mh->GetProto()->Delay );
					proc_Chance = float2int32( mhs * 0.001f * ppm / 0.6f );
				}
				else
					proc_Chance = 0;

				if( static_cast< Player* >( this )->IsInFeralForm() )
				{
					if( static_cast< Player* >( this )->GetShapeShift() == FORM_CAT )
					{
						proc_Chance = FL2UINT( ppm / 0.6f );
					}
					else if( static_cast< Player* >( this )->GetShapeShift() == FORM_BEAR || static_cast< Player* >( this )->GetShapeShift() == FORM_DIREBEAR )
					{
						proc_Chance = FL2UINT( ppm / 0.24f );
					}
				}
			}

			SM_FIValue( SM_FChanceOfSuccess, (int32*)&proc_Chance, ospinfo );
			if( spellId && Rand( proc_Chance ) )
			{
				SpellEntry* proc_spell = dbcSpell.LookupEntry(spellId);
				SpellCastTargets targets;
				if(proc_spell->EffectImplicitTargetA[0] == EFF_TARGET_SELF)
					targets.m_target = this;
				else 
					targets.m_target = victim;
				/* hmm whats a reasonable value here */
				if( m_procCounter > 40 )
				{
					/* something has proceed over 10 times in a loop :/ dump the spellids to the crashlog, as the crashdump will most likely be useless. */
					// BURLEX FIX ME!
					//OutputCrashLogLine("HandleProc %u SpellId %u (%s) %u", flag, spellId, sSpellStore.LookupString(sSpellStore.LookupEntry(spellId)->Name), m_procCounter);
					return;
				}

				//check if we can trigger due to time limitation
				if( ospinfo->proc_interval )
				{
					uint32 now_in_ms=getMSTime();
					if( itr2->LastTrigger + ospinfo->proc_interval > now_in_ms )
						continue; //we can't trigger it yet.
					itr2->LastTrigger = now_in_ms; // consider it triggered
				}

				int dmg_overwrite = itr2->procValue;

				//these are player talents. Fuckem they pull the emu speed down 
				if( IsPlayer() )
				{
					if( spe && spe->ProcOnNameHash[0] != 0 )
					{
						if( CastingSpell == NULL )
							continue;

						if( CastingSpell->NameHash != spe->ProcOnNameHash[0] &&
							CastingSpell->NameHash != spe->ProcOnNameHash[1] &&
							CastingSpell->NameHash != spe->ProcOnNameHash[2] )
							continue;
					}

					uint32 talentlevel = 0;
					switch( origId )
					{
						//mace specialization
						case __Mace_Specialization_Rank_1____0	 :	{talentlevel = 1;} break; // 12284 
						case __Mace_Specialization_Rank_2		 :	{talentlevel = 2;} break; // 12701 
						case __Mace_Specialization_Rank_3		 :	{talentlevel = 3;} break; // 12702 
						case __Mace_Specialization_Rank_4		 :	{talentlevel = 4;} break; // 12703 
						case __Mace_Specialization_Rank_5		 :	{talentlevel = 5;} break; // 12704 

						//Unbridled Wrath
						case __Unbridled_Wrath_Rank_1____0 :	{talentlevel = 1;}break; // 12964
						case __Unbridled_Wrath_Rank_2      :	{talentlevel = 2;}break; // 12999
						case __Unbridled_Wrath_Rank_3      :	{talentlevel = 3;}break; // 13000 
						case __Unbridled_Wrath_Rank_4      :	{talentlevel = 4;}break; // 13001
						case __Unbridled_Wrath_Rank_5      :	{talentlevel = 5;}break; // 13002 
					}

					switch( spellId )
					{
						/*//Healing Way should only proc from Healing Wave
						case 29203:
						{
							if (CastingSpell == NULL)
								continue;
							if (CastingSpell->NameHash != SPELL_HASH_HEALING_WAVE)
								continue;
						} break;*/
						// Winter's Chill should only proc from frost damage
					case __Winter_s_Chill : // 12579
						{
							if( CastingSpell == NULL )
								continue;
							if( !(CastingSpell->schoolMask & SCHOOL_MASK_FROST) )
								continue;
						}break;

						case __Seal_Fate :           //Seal Fate (14189)
						case __Blood_Frenzy_Rank_1 : // 16952 (16953 = faux) //Blood Frenzy 
						case __Primal_Fury         : // 16953 (16959 = faux) //Primal Fury
						{
							if( !this->IsPlayer() || 
								!CastingSpell || 
								CastingSpell->Id == __Seal_Fate ||
								CastingSpell->Id == __Blood_Frenzy_Rank_1 ||
								CastingSpell->Id == __Primal_Fury )
								continue;
							if( CastingSpell->Effect[0] != 80 &&
								CastingSpell->Effect[1] != 80 &&
								CastingSpell->Effect[2] != 80 &&
								CastingSpell->NameHash != SPELL_HASH_MANGLE__CAT_ )
								continue;
						}break;

						case __Intensity_Rank_1____0: // 17106: //druid intencity
						{
							if( CastingSpell == NULL )
								continue;
							if( CastingSpell->Id != __Enrage )//enrage
								continue;
						}break;

						case __Eclipse__Lunar_:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_WRATH )
									continue;
							}break;
						case __Eclipse__Solar_:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_STARFIRE )
									continue;
							}break;
						// Infected Wounds
						case __Infected_Wounds:
						case __Infected_Wounds____0:
						case __Infected_Wounds____1:
							{
								if (!CastingSpell )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_SHRED &&
									CastingSpell->NameHash != SPELL_HASH_MAUL &&
									CastingSpell->NameHash != SPELL_HASH_MANGLE__CAT_ &&
									CastingSpell->NameHash != SPELL_HASH_MANGLE__BEAR_ )
									continue;
							}break;
						// Focus Magic
						case __Focus_Magic____0:
							{
								Aura* aur = FindAura(54646);
								if( aur )
								{
									Unit* cstr = aur->GetUnitCaster();
									if( cstr )
										victim = cstr;
								}
							}break;
						case __Nature_s_Guardian: // 31616:
						{
							//yep, another special case: Nature's grace
							if( GetHealthPct() > 30 )
								continue;
							//heal value depends on the rank of parent spell
							//maybe we should use CalculateEffect(uint32 i) to gain SM benefits
							int32 value = 0;
							int32 basePoints = ospinfo->EffectBasePoints[0]+1;//+(getLevel()*basePointsPerLevel);
							int32 randomPoints = ospinfo->EffectDieSides[0];
							if(randomPoints <= 1)
								value = basePoints;
							else
								value = basePoints + rand() % randomPoints;
							dmg_overwrite = GetUInt32Value(UNIT_FIELD_MAXHEALTH) * value / 100;
						}break;
						case __Evasive_Maneuvers____0: // Commendation of Kael'thas
						{
							if(GetHealthPct() >= 35)
								continue;
						}break;
						case __Bloodlust_Rank_1: // 37309:
						{
							if( !this->IsPlayer() )
								continue;
							if( static_cast< Player* >( this )->GetShapeShift() != FORM_BEAR ||
								static_cast< Player* >( this )->GetShapeShift() != FORM_DIREBEAR )
								continue;
						}break;

						case __Bloodlust_Rank_1____0: //37310:
						{
							if( !this->IsPlayer() || static_cast< Player* >( this )->GetShapeShift() != FORM_CAT )
								continue;
						}break;

						case __Holy_Concentration_Rank_1: //34754: //holy concentration
						case __Holy_Concentration_Rank_2:
						case __Holy_Concentration_Rank_3:
                        { /*
						  __Holy_Concentration_Rank_1				34753
						  __Holy_Concentration_Rank_2				34859
						  __Holy_Concentration_Rank_3				34860
						  */

							if( CastingSpell == NULL ) continue;

							if( CastingSpell->NameHash != SPELL_HASH_FLASH_HEAL &&
								CastingSpell->NameHash != SPELL_HASH_BINDING_HEAL &&
								CastingSpell->NameHash != SPELL_HASH_GREATER_HEAL )
								continue;
						}break;

						//Desecration
						case __Desecration____0:
							{
								if ( CastingSpell == NULL )
									continue;

								if( CastingSpell->NameHash != SPELL_HASH_PLAGUE_STRIKE &&
									CastingSpell->NameHash != SPELL_HASH_SCOURGE_STRIKE )
									continue;
							}break;
						case __Mace_Specialization_Rank_1: //5530:
						case __Mace_Specialization_Rank_2:
						case __Mace_Specialization_Rank_3:
						case __Mace_Specialization_Rank_4:
						case __Mace_Specialization_Rank_5:

						{
							//warrior/rogue mace specialization can trigger only when using maces
							Item* it;
							if( static_cast< Player* >( this )->GetItemInterface() )
							{
								it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
								if( it != NULL && it->GetProto() )
								{
									uint32 reqskill = GetSkillByProto( it->GetProto()->Class, it->GetProto()->SubClass );
									if( reqskill != SKILL_MACES && reqskill != SKILL_2H_MACES )
										continue;
								}
								else
									continue; //no weapon no joy
							}
							else
								continue; //no weapon no joy
							//let's recalc chance to cast since we have a full 100 all time on this one
							//how lame to get talentpointlevel for this spell :(
							//float chance=it->GetProto()->Delay*100*talentlevel/60000;
							float chance = float( it->GetProto()->Delay ) * float( talentlevel ) / 600.0f;
							if( !Rand( chance ) )
								continue;
						}break;
						case __Blessing_of_Sanctuary____0:
						{
							switch( victim->GetPowerType() )
							{
								case POWER_TYPE_RAGE: spellId = 57320; break;
								case POWER_TYPE_RUNIC_POWER : spellId = 57321; break;
							}
						}break;
						case __Sword_Specialization: // 16459:
						{
							//sword specialization
							if( static_cast< Player* >( this )->GetItemInterface())
							{
								Item* it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
								if( it != NULL && it->GetProto() )
								{
									uint32 reqskill=GetSkillByProto( it->GetProto()->Class, it->GetProto()->SubClass );
									if( reqskill != SKILL_SWORDS && reqskill != SKILL_2H_SWORDS )
										continue;
								}
								else
									continue; //no weapon no joy
							}
							else
								continue; //no weapon no joy
						}break;

						case __Deep_Wounds: //12721:
						{
							//deep wound requires a melee weapon
							Item* it;
							if( static_cast< Player* >( this )->GetItemInterface())
							{
								it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
								if( it != NULL && it->GetProto() )
								{
									//class 2 means weapons ;)
									if( it->GetProto()->Class != 2 )
										continue;
								}
								else continue; //no weapon no joy
							}
							else continue; //no weapon no joy
						}break;
						//Death knight - Chilblains
						case __Chilblains_Rank_1:
						case __Chilblains_Rank_2:
						case __Chilblains_Rank_3:
						case __Icy_Clutch:
						case __Icy_Clutch____0:
						case __Icy_Clutch____1:
								{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_ICY_TOUCH  )
									continue;
								}break;
						//Death Knight - Desecration
						case __Desecration_Rank_1:
						case __Desecration_Rank_2:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_SCOURGE_STRIKE &&
									CastingSpell->NameHash != SPELL_HASH_PLAGUE_STRIKE )
									continue;
							}break;

						//Warrior - Juggernaut
						case __Juggernaut_Rank_1:
						case __Juggernaut:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_CHARGE )
									continue;
							}break;
						//warrior - Blood Frenzy
						case __Blood_Frenzy_Rank_1____1: //30069:
						case __Blood_Frenzy_Rank_2____1: //30070:
						{
							if( CastingSpell == NULL )
								continue;
								if( CastingSpell->NameHash != SPELL_HASH_REND && 
									CastingSpell->NameHash != SPELL_HASH_DEEP_WOUNDS )
									continue;
						}break;
						//warrior - Unbridled Wrath
						case __Unbridled_Wrath_Rank_1: //12964:
						{
							//let's recalc chance to cast since we have a full 100 all time on this one
							Item* it;
							if( static_cast< Player* >( this )->GetItemInterface() )
							{
								it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
								if( !( it != NULL && it->GetProto() ) )
									continue; //no weapon no joy
							}
							else
								continue; //no weapon no joy
							//float chance=float(it->GetProto()->Delay)*float(talentlevel)/600.0f;
							float chance = float( it->GetProto()->Delay ) * float(talentlevel ) / 300.0f; //zack this had a very low proc rate. Kinda liek a waisted talent
							uint32 myroll = RandomUInt( 100 );
							if( myroll > chance )
								continue;
						}break;
						// Sword and Board
						case __Sword_and_Board:
							{
								if (!CastingSpell )
									continue;

								if( CastingSpell->NameHash != SPELL_HASH_DEVASTATE &&
									CastingSpell->NameHash != SPELL_HASH_REVENGE )
									continue;
							}break;
						// Improved Revenge / Glyph of Revenge
						case __Revenge_Stun_Rank_1:
						case __Glyph_of_Revenge____0:
							{
								if (!CastingSpell )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_REVENGE )
									continue;
							}break;
						//warrior - Bloodsurge
						case __Bloodsurge_Rank_1:
						case __Bloodsurge_Rank_2:
						case __Bloodsurge_Rank_3:
						//case __Slam_: // Je pense qu'il s'agit d'une erreur. En commentaire.
						{
							if( CastingSpell == NULL)
								continue;
								if( CastingSpell->NameHash != SPELL_HASH_HEROIC_STRIKE &&
									CastingSpell->NameHash != SPELL_HASH_BLOODTHIRST &&
									CastingSpell->NameHash != SPELL_HASH_WHIRLWIND )
									continue;
						}break;
						//Paladin - Art of War
						case __The_Art_of_War:
						case __The_Art_of_War_Rank_2____0:
							{
								if(CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_JUSTICE &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_LIGHT &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_WISDOM &&
									CastingSpell->NameHash != SPELL_HASH_CRUSADER_STRIKE &&
									CastingSpell->NameHash != SPELL_HASH_DIVINE_STORM )
									continue;
							}break;
						//Rogue - Waylay
						case __Waylay:
							{
								if(CastingSpell == NULL )
									continue;
								if(CastingSpell->NameHash != SPELL_HASH_AMBUSH)
									continue;
							}break;
						//Paladin - Infusion of Light
						case __Infusion_of_Light:
						case __Infusion_of_Light_Rank_2____0:
							{
								if(CastingSpell == NULL)
									continue;
								if(CastingSpell->NameHash != SPELL_HASH_HOLY_SHOCK)
									continue;
							}break;
/*						//disabled by zack until finished : this needs to get trigered on trap trigger and not trap cast
						// hunter - Entrapment
						case 19185:
						{
							if( CastingSpell == NULL )
								continue;
								//only trigger effect for specified spells
								if( CastingSpell->NameHash!=SPELL_HASH_BACKSTAB && //immolation trap
									CastingSpell->NameHash!=SPELL_HASH_SINISTER_STRIKE && //frost trap
									CastingSpell->NameHash!=SPELL_HASH_SHIV && //shiv
									CastingSpell->NameHash!=SPELL_HASH_GOUGE ) //gouge
									continue;
						}break;*/
						// Mage ignite talent only for fire dmg
						case __Ignite____0: //12654:
						{
							if( CastingSpell == NULL )
								continue;
							if( !(CastingSpell->schoolMask & SCHOOL_MASK_FIRE) )
								continue;
							SpellEntry* spellInfo = dbcSpell.LookupEntry( spellId ); //we already modified this spell on server loading so it must exist
							SpellDuration* sd = dbcSpellDuration.LookupEntryForced( spellInfo->durationIndex );
							uint32 tickcount = GetDuration( sd ,INVOC_DURATION) / spellInfo->EffectAmplitude[0] ;
							dmg_overwrite = ospinfo->EffectBasePoints[0] * dmg / (100  * tickcount );
						}break;
						//druid - Primal Fury
						case __Primal_Fury_Rank_1____1: //37116:
						case __Primal_Fury_Rank_2____0: //37117:
						{
							if (!this->IsPlayer())
								continue;
							Player* mPlayer = (Player*)this;
							if (!mPlayer->IsInFeralForm() || 
								(mPlayer->GetShapeShift() != FORM_CAT &&
								mPlayer->GetShapeShift() != FORM_BEAR &&
								mPlayer->GetShapeShift() != FORM_DIREBEAR))
								continue;
						}break;
						//rogue - blade twisting
						case __Blade_Twisting: //31125:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( CastingSpell->NameHash != SPELL_HASH_BACKSTAB && //backstab
									CastingSpell->NameHash != SPELL_HASH_SINISTER_STRIKE && //sinister strike
									CastingSpell->NameHash != SPELL_HASH_SHIV && //shiv
									CastingSpell->NameHash != SPELL_HASH_GOUGE ) //gouge
									continue;
							}break;
						//warlock - Improved Shadow Bolt
						case __Shadow_Mastery:
						case __Shadow_Mastery____0:
						case __Shadow_Mastery____1:
						case __Shadow_Mastery____2:
						case __Shadow_Mastery____3:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( CastingSpell->NameHash!=SPELL_HASH_SHADOW_BOLT)//shadow bolt								
									continue;
							}break;
						// warlock - Seed of Corruption
						case __Seed_of_Corruption_Rank_1____0:
							{
								bool can_proc_now = false;
								//if we proced on spell tick
								if( flag & PROC_ON_HARMFULSPELL_LAND_VICTIM )
								{
									if( CastingSpell == NULL )
										continue;
									//only trigger effect for specified spells
									if( CastingSpell->NameHash != SPELL_HASH_SEED_OF_CORRUPTION )						
										continue;
									//this spell builds up in time
									(*itr2).procCharges += dmg;
									if( (int32)(*itr2).procCharges >= ospinfo->EffectBasePoints[ 1 ] && //if charge built up
										dmg < this->GetUInt32Value( UNIT_FIELD_HEALTH ) ) //if this is not a killer blow
										can_proc_now = true;
								}
								else can_proc_now = true; //target died
								if( can_proc_now == false )
									continue;
								Unit *new_caster = victim;
								if( new_caster && new_caster->isAlive() )
								{
									SpellEntry *spellInfo = dbcSpell.LookupEntry( spellId ); //we already modified this spell on server loading so it must exist
									Spell *spell = new Spell( new_caster, spellInfo ,true, NULL );
									SpellCastTargets targets;
									targets.m_destX = GetPositionX();
									targets.m_destY = GetPositionY();
									targets.m_destZ = GetPositionZ();
									spell->prepare(&targets);
								}
								(*itr2).deleted = true;
								continue;
							}break;
						// warlock - Improved Drain Soul
						case __Drain_Soul:
							{
								if( !IsPlayer() )
									continue;
								Player* plr = static_cast<Player *>(this);
								if(!plr->m_currentSpell || !plr->m_currentSpell->GetSpellProto() || 
									plr->m_currentSpell->GetSpellProto()->NameHash != SPELL_HASH_DRAIN_SOUL)
										continue;
								dmg_overwrite = ( ospinfo->EffectBasePoints[2] + 1 ) * GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) / 100;
							}break;
						// warlock - Unstable Affliction
						case __Unstable_Affliction:
							{
								//null check was made before like 2 times already :P
								dmg_overwrite = ( ospinfo->EffectBasePoints[0] + 1 ) * 9;
								if( itr2->caster && GetMapMgr()->GetUnit(itr2->caster) )
								{
									Unit* cstr = GetMapMgr()->GetUnit(itr2->caster);
									dmg_overwrite += cstr->GetDamageDoneMod(SCHOOL_SHADOW) * 1.8;
								}
							}break;
						//warlock soul link
						case __Soul_Link____0:
							{
								//we need a pet for this, else we do not trigger it at all
								if( !IsPlayer() )
									continue;
								Unit* new_caster;
								if( GetSummon() )
									new_caster = GetSummon();
								else if( GetUInt64Value( UNIT_FIELD_CHARM ) )
									new_caster = GetMapMgr()->GetUnit( GetUInt64Value( UNIT_FIELD_CHARM ) );
								else
									new_caster = NULL;
								if( new_caster != NULL && new_caster->isAlive() )
								{
									SpellEntry* spellInfo = dbcSpell.LookupEntry( 25228 ); //we already modified this spell on server loading so it must exist
									Spell* spell = new Spell( new_caster, spellInfo, true, NULL );
									spell->forced_basepoints[0] = dmg;
									SpellCastTargets targets;
									targets.m_target = this;
									spell->prepare( &targets );
								}
								continue;
							}break;
						//warlock - Nighfall
						case __Shadow_Trance:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( CastingSpell->NameHash != SPELL_HASH_CORRUPTION && //Corruption
									CastingSpell->NameHash != SPELL_HASH_DRAIN_LIFE )//Drain Life								
									continue;
							}break;
/*                        //warlock - Demonic Knowledge
                        case 39576:
                            {
                                if( CastingSpell == NULL )
                                    continue;
                                if( CastingSpell->Effect[0] != 56 )
                                    continue;
                                Pet* ps = static_cast< Player* >( this )->GetSummon();
                                if( ps == NULL)
                                    return;//no pet
                                int32 val;
                                SpellEntry *parentproc= dbcSpell.LookupEntry( origId );
                                val = parentproc->EffectBasePoints[0] + 1;
                                val = val * (ps->GetUInt32Value( UNIT_FIELD_STAT2 ) + ps->GetUInt32Value( UNIT_FIELD_STAT3 ) );
                                SpellEntry *spellInfo = dbcSpell.LookupEntry( 39576 );
                                Spell *spell = new Spell( this, spellInfo ,true, NULL );
                                spell->forced_basepoints[0] = ( val / 100 );
                                SpellCastTargets targets;
                                targets.m_unitTarget = GetGUID();
                                spell->prepare( &targets );
                                continue;
                            }break;*/
						//mage - Arcane Blast proc
						case __Arcane_Blast____12:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_ARCANE_BLAST ) //Arcane Blast
									continue;
							}break;
						//warlock - Shadow Embrace
						case __Shadow_Embrace:
						case __Shadow_Embrace____0:
						case __Shadow_Embrace____1:
						case __Shadow_Embrace____2:
						case __Shadow_Embrace____3:
							{
								if( CastingSpell == NULL )
									continue;
								else
								{
									if( CastingSpell->NameHash != SPELL_HASH_FIRE_SHIELD_II && // Corruption
										CastingSpell->NameHash != SPELL_HASH_CURSE_OF_AGONY && //CoA
										CastingSpell->NameHash != SPELL_HASH_SIPHON_LIFE && //Siphon Life
										CastingSpell->NameHash != SPELL_HASH_SEED_OF_CORRUPTION ) //SoC
										continue;
								}
							}break;
						//warlock - Aftermath
						case __Aftermath:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								skilllinespell* skillability = objmgr.GetSpellSkill(CastingSpell->Id);
								if( !skillability )
									continue;
								if( skillability->skilline != SKILL_DESTRUCTION )
									continue;
							}break;
						//warlock - Nether Protection
						case __Nether_Protection:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if(!(CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING))
									continue;
								if( !(CastingSpell->schoolMask & SCHOOL_MASK_FIRE | SCHOOL_MASK_SHADOW))
									continue;
							}break;
						//warlock - Soul Leech
						//this whole spell should get rewriten. Uses bad formulas, bad trigger method, spell is rewriten ...
						case __Soul_Leech:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								uint32 amount;
								switch( CastingSpell->NameHash )
								{
									case SPELL_HASH_SHADOW_BOLT: //Shadow Bolt
									case SPELL_HASH_SOUL_FIRE: //Soul Fire
									case SPELL_HASH_INCINERATE: //Incinerate
									case SPELL_HASH_SEARING_PAIN: //Searing Pain
									case SPELL_HASH_CONFLAGRATE: //Conflagrate
									{
										amount = CastingSpell->EffectBasePoints[0]+1;
									}break;
									case SPELL_HASH_SHADOWBURN: //Shadowburn
									{
										amount = CastingSpell->EffectBasePoints[1]+1;
									}break;
									default:
										amount=0;
								}
								if(!amount)
									continue;
								SpellEntry *spellInfo = dbcSpell.LookupEntry(spellId );
								if(!spellInfo)
									continue;
								Spell *spell = new Spell(this, spellInfo ,true, NULL);
								spell->SetUnitTarget(this);
								spell->Heal(amount*(ospinfo->EffectBasePoints[0]+1)/100);
								continue;
							}break;
						//warlock - pyroclasm
						case __Pyroclasm:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( CastingSpell->NameHash != SPELL_HASH_RAIN_OF_FIRE && //Rain of Fire
									CastingSpell->NameHash != SPELL_HASH_HELLFIRE_EFFECT && //Hellfire
									CastingSpell->NameHash != SPELL_HASH_SOUL_FIRE ) //Soul Fire
									continue;
							}break;
						//mage - Improved Scorch
						case __Improved_Scorch:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( CastingSpell->NameHash != SPELL_HASH_SCORCH ) //Scorch
									continue;
							}break;
						//mage - Combustion
						case __Combustion____0:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING)
									|| !(CastingSpell->schoolMask & SCHOOL_MASK_FIRE) )
									continue;
								if( flag & PROC_ON_SPELL_CRIT_HIT )
								{
									itr2->procCharges++;
									if( itr2->procCharges >= 3 ) //whatch that number cause it depends on original stack count !
									{
										RemoveAllAuraByNameHash( SPELL_HASH_COMBUSTION );
									}
									continue;
								}
							}break;
						//priest - Misery
						case __Misery_Rank_3:
						case __Misery_Rank_2:
						case __Misery_Rank_1:
						case __Misery_Rank_3____0:
						case __Misery_Rank_2____0:
						case __Misery_Rank_1____0:
							{
								if( CastingSpell == NULL )
									continue;
								else
								{
									if( CastingSpell->NameHash != SPELL_HASH_MIND_FLAY && // Mind Flay
										CastingSpell->NameHash != SPELL_HASH_SHADOW_WORD__PAIN && //SW:P
										CastingSpell->NameHash != SPELL_HASH_VAMPIRIC_TOUCH ) //SoC
										continue;
								}
							}break;
						//priest - Shadow Weaving
						case __Shadow_Weaving_Rank_1____0:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								if( CastingSpell->School != SCHOOL_SHADOW || !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ) ) //we need damaging spells for this, so we suppose all shadow spells casted on target are dmging spells = Wrong
									continue;
							}break;
						//priest - Inspiration
						case __Inspiration_Rank_3____0:
						case __Inspiration_Rank_1____0:
						case __Inspiration_Rank_2:
						case __Inspiration_Rank_3:
							{
								if( !CastingSpell || !( CastingSpell->c_is_flags & SPELL_FLAG_IS_HEALING ) )
									continue;
							}
							break;
						//priest - Blessed Recovery
                        case __Blessed_Recovery_Rank_1____0:
                        case __Blessed_Recovery_Rank_2____0:
                        case __Blessed_Recovery_Rank_3____0:
                            {
                                if(!IsPlayer() || !dmg)
                                    continue;
                                SpellEntry *parentproc= dbcSpell.LookupEntry(origId);
                                SpellEntry *spellInfo = dbcSpell.LookupEntry(spellId);
								if (!parentproc || !spellInfo)
									continue;
								int32 val = parentproc->EffectBasePoints[0] + 1;
                                Spell *spell = new Spell(this, spellInfo ,true, NULL);
                                spell->forced_basepoints[0] = (val*dmg)/300; //per tick
                                SpellCastTargets targets(this);
                                spell->prepare(&targets);
                                continue;
                            }break;
						//Druid Living Seed
						case __Living_Seed____0:
							{
								if ( CastingSpell == NULL )
									continue;
								if ( CastingSpell->NameHash != SPELL_HASH_SWIFTMEND &&
									 CastingSpell->NameHash != SPELL_HASH_REGROWTH &&
									 CastingSpell->NameHash != SPELL_HASH_HEALING_TOUCH &&
									 CastingSpell->NameHash != SPELL_HASH_NOURISH )
									continue;
							}break;
						case __Earth_Shield_Rank_1:
						case __Earth_Shield_Rank_2:
						case __Earth_Shield_Rank_3:
						case __Earth_Shield_Rank_4:
						case __Earth_Shield_Rank_5: // Earth Shield
							{
								SpellEntry *spellInfo = dbcSpell.LookupEntry( 379 );
								Unit* caster = NULL;
								if(GET_TYPE_FROM_GUID(itr2->caster) == HIGHGUID_TYPE_PLAYER && IsInWorld())
								{
									caster = GetMapMgr()->GetPlayer( GUID_LOPART(itr2->caster) );
								}
								if(!caster)
									caster = static_cast<Unit *>(this);
								targets.m_target = ((Unit *)GetGUID());
								Spell* spell(new Spell(static_cast<Unit *>(this), spellInfo ,true, NULL));
								spell->forced_basepoints[0] = ospinfo->EffectBasePoints[0] + 1;
								spell->ProcedOnSpell = CastingSpell;
								spell->pSpellId=origId;
								spell->prepare(&targets);
								continue;
							}break;
						case __Improved_Water_Shield_Rank_1:
						case __Improved_Water_Shield_Rank_2:
						case __Improved_Water_Shield_Rank_3: // Improved Water Shield
							{
								// Proc for Lesser Healing Wave is 60% of base one
								if(!CastingSpell || 
									(CastingSpell->NameHash == SPELL_HASH_LESSER_HEALING_WAVE && Rand(40)))
									continue;
								Aura* shield = FindAuraPosByNameHash(SPELL_HASH_WATER_SHIELD);
								
								if(!shield)
									continue;
								int32 procCharge = shield->m_spellProto->procCharges;
								if (procCharge)
								{
									procCharge = procCharge-1;
									shield->m_spellProto->procCharges = procCharge;
								}
								spellId = shield->m_spellProto->EffectTriggerSpell[0];
								
							}break;
						case __Static_Shock_Rank_1:
						case __Static_Shock_Rank_2:
						case __Static_Shock_Rank_3:	// Static Shock
							{
								Aura* shield = FindAuraPosByNameHash(SPELL_HASH_LIGHTNING_SHIELD);
								if(!shield)
									continue;
								int32 procCharge = shield->m_spellProto->procCharges;
								if (procCharge)
								{
									procCharge = procCharge-1;
									shield->m_spellProto->procCharges = procCharge;
								}
								spellId = shield->m_spellProto->EffectTriggerSpell[0];
							}break;
						//shaman - Elemental Devastation
						case __Elemental_Devastation_Rank_2:
						case __Elemental_Devastation_Rank_3:
						case __Elemental_Devastation_Rank_1____0:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//only trigger effect for specified spells
								if( !(CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING)) //healing wave
									continue;
							}break;
						//shaman - windfury weapon
						case __Windfury_Attack:
							{
								if(!IsPlayer() || weapon_damage_type < 1 || weapon_damage_type > 2)
									continue;
								Item* mh = static_cast<Player *>(this)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND + weapon_damage_type -1);
								if(!mh) continue;
								uint32 apBonus = 46; // use rank 1 bonus as default
								EnchantmentInstance * ei = mh->GetEnchantment(1);
								if(!ei) continue;
								EnchantEntry * e = ei->Enchantment;
								if(!e) continue;
								switch(e->Id)
								{
								case 284: apBonus = 119; break;
								case 525: apBonus = 249; break;
								case 1669: apBonus = 333; break;
								case 2636: apBonus = 445; break;
								case 3785: apBonus = 835; break;
								case 3786: apBonus = 1090; break;
								case 3787: apBonus = 1250; break;
								}	
								float mhs = float( mh->GetProto()->Delay );
								// Calculate extra AP bonus damage
								uint32 extra_dmg=float2int32(mhs * apBonus / 14000.0f);
								if(weapon_damage_type == 2)	// offhand gets 50% bonus
									extra_dmg /= 2;
								Strike( victim, weapon_damage_type-1, spe, extra_dmg, 0, 0, false, false );
								Strike( victim, weapon_damage_type-1, spe, extra_dmg, 0, 0, false, false );
								spellId = 33010; // WF animation
							}break;
	
						//shaman - Ancestral Fortitude
						case __Ancestral_Fortitude_Rank_1:
						case __Ancestral_Fortitude_Rank_2:
						case __Ancestral_Fortitude_Rank_3:
							{
								if( CastingSpell == NULL )
									continue;
								if( !(CastingSpell->c_is_flags & SPELL_FLAG_IS_HEALING) ) //healing spell
									continue;
							}break;
						// Flametongue Weapon
						case __Flametongue_Weapon_Proc_Rank_1:
						case __Flametongue_Weapon_Proc_Rank_2:
						case __Flametongue_Weapon_Proc_Rank_3:
						case __Flametongue_Weapon_Proc_Rank_4:
						case __Flametongue_Weapon_Proc_Rank_5:
						case __Flametongue_Weapon_Proc_Rank_6:
						case __Flametongue_Weapon_Proc_Rank_7:
						case __Flametongue_Weapon_Proc_Rank_8:
						case __Flametongue_Weapon_Proc_Rank_9:
						case __Flametongue_Weapon_Proc_Rank_10:
							{
								if(!IsPlayer() || weapon_damage_type < 1 || weapon_damage_type > 2)
									continue;
								spellId = 10444;	// Flametongue Weapon proc
								Item* mh = static_cast<Player *>(this)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND + weapon_damage_type - 1 );

								if( mh != NULL)
								{
									float mhs = float( mh->GetProto()->Delay );
									dmg_overwrite = FL2UINT( mhs * 0.001f * (spe->EffectBasePoints[0] + 1)/88 );
								}
								else
									continue;
							}break;

						//shaman - windfurry weapon
						case __Windfury_Weapon_Rank_1:
						case __Windfury_Weapon_Rank_2:
						case __Windfury_Weapon_Rank_3:
						case __Windfury_Weapon_Rank_4:
						case __Windfury_Weapon_Rank_5:
							{
								if(!IsPlayer())
									continue;
								//!! The wierd thing is that we need the spell thet trigegred this enchant spell in order to output logs ..we are using oldspell info too 
								//we have to recalc the value of this spell
								SpellEntry *spellInfo = dbcSpell.LookupEntry(origId);
								uint32 AP_owerride=GetAP() + spellInfo->EffectBasePoints[0]+1;
								uint32 dmg = static_cast< Player* >( this )->GetMainMeleeDamage(AP_owerride);
								SpellEntry *sp_for_the_logs = dbcSpell.LookupEntry(spellId);
								Strike( victim, MELEE, sp_for_the_logs, dmg, 0, 0, true, false );
								Strike( victim, MELEE, sp_for_the_logs, dmg, 0, 0, true, false );
								//nothing else to be done for this trigger
								continue;
							}break;
						//rogue - Ruthlessness
						case __Ruthlessness_Rank_1____0:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//we need a finishing move for this 
								if(!(CastingSpell->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE) || victim==this)
									continue;

								if(IsPlayer())
								{
									static_cast< Player* >(this)->m_spellcomboPoints = 0;
									//static_cast< Player* >(this)->UpdateComboPoints();
								}
							}break;
						//rogue - Relentless Strikes
						case __Relentless_Strikes_Effect_Rank_1:
							{
								if( CastingSpell == NULL || !IsPlayer() || static_cast< Unit* >(this) != victim)	// to prevent it proccing 2 times
									continue;//this should not ocur unless we made a fuckup somewhere
								int32 proc_Chance;
								//chance is based actually on combopoint count and not 100% always 
								if( CastingSpell->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE && IsPlayer())
									proc_Chance = static_cast< Player* >( this )->m_comboPoints*ospinfo->EffectBasePoints[1];
								else continue;
								if(!Rand(proc_Chance))
									continue;
							}break;
						//rogue - Find Weakness
						case __Find_Weakness_Rank_1:
						case __Find_Weakness_Rank_2:
						case __Find_Weakness_Rank_3:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								if(!(CastingSpell->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE))
									continue;
							}break;
						//rogue - Initiative
						case __Initiative_Rank_1____0:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//we need a Ambush, Garrote, or Cheap Shot
								if( CastingSpell == NULL )
									continue;

								if( CastingSpell->NameHash != SPELL_HASH_CHEAP_SHOT && //Cheap Shot
									CastingSpell->NameHash != SPELL_HASH_AMBUSH && //Ambush
									CastingSpell->NameHash != SPELL_HASH_GARROTE )  //Garrote
									continue;
							}break;
						//warrior - improved berserker rage 
						case __Berserker_Rage_Effect:
						case __Berserker_Rage_Effect____0:
							{
								if( !CastingSpell || CastingSpell->NameHash != SPELL_HASH_BERSERKER_RAGE )
									continue;
							}break;
						//mage - Arcane Concentration 
						/*case 12536:
							{
								//requires damageing spell
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								if( !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
									continue;
							}break;*/
						//mage - Improved Blizzard
						case __Improved_Blizzard_Rank_1:
						case __Improved_Blizzard_Rank_2:
						case __Improved_Blizzard_Rank_3:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_BLIZZARD || victim == this ) //Blizzard
									continue;
							}break;
						//mage - Master of Elements
						case __Master_of_Elements:
							{
								if( CastingSpell == NULL )
									continue;
								if(!(CastingSpell->schoolMask & (SCHOOL_MASK_FROST | SCHOOL_MASK_FIRE))) //fire and frost criticals
									continue;
								dmg_overwrite = CastingSpell->manaCost * ( ospinfo->EffectBasePoints[0] + 1 ) / 100;
							}break;
						// Burnout
						case __Burnout:
							{
								if( CastingSpell == NULL )
									continue;

								int32 addcost = float2int32(GetSpellBaseCost(CastingSpell) * ((ospinfo->EffectBasePoints[1] + 1) / 100));
								if( (GetUInt32Value(UNIT_FIELD_POWER1) - addcost) < 0 )
									SetUInt32Value(UNIT_FIELD_POWER1, 0);
								else
									ModUnsigned32Value(UNIT_FIELD_POWER1, -addcost);
							}break;
						//Hunter - Thrill of the Hunt
						case __Thrill_of_the_Hunt:
							{
								if( CastingSpell == NULL )
									continue;
								dmg_overwrite = CastingSpell->manaCost * 40 / 100;
							}break;
						// Improved Steady Shot
						case __Improved_Steady_Shot____1:
							{
								if (!CastingSpell )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_STEADY_SHOT )
									continue;				
							}break;
						//priest - Reflective Shield 
						case __Reflective_Shield:
							{
								//requires Power Word: Shield active
								int power_word_id = HasAurasWithNameHash( SPELL_HASH_POWER_WORD__SHIELD );
								if( !power_word_id )
									continue;//this should not ocur unless we made a fuckup somewhere
								//make a direct strike then exit rest of handler
								int tdmg = abs * ( ospinfo->EffectBasePoints[0] + 1 ) / 100;
								//somehow we should make this not caused any threat (tobedone)
								SpellNonMeleeDamageLog( victim, power_word_id, tdmg, false, true );
								continue;
							}break;
						//rogue - improved sprint
						case __Improved_Sprint:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_SPRINT || victim != this ) //sprint
									continue;
							}break;
						//rogue - combat potency
						case __Combat_Potency_Rank_1____0:
						case __Combat_Potency_Rank_2:
						case __Combat_Potency_Rank_3:
						case __Combat_Potency_Rank_4:
						case __Combat_Potency_Rank_5:
							{
								if( !IsPlayer() || !dmg )
									continue;
								//this needs offhand weapon
								Item* it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
								if(it == NULL || (it->GetProto()->InventoryType != INVTYPE_WEAPON && it->GetProto()->InventoryType != INVTYPE_WEAPONOFFHAND))
									continue;
							}break;
						//paladin - Seal of Blood
						case __Seal_of_Blood____0:
							{
								//we loose health depending on father of trigger spell when trigering this effect
								int32 healthtoloose = ospinfo->EffectBasePoints[1] * GetUInt32Value( UNIT_FIELD_BASE_HEALTH ) / 100;
								if( healthtoloose > (int32)GetUInt32Value( UNIT_FIELD_HEALTH ) )
									SetUInt32Value( UNIT_FIELD_HEALTH, 1 );
								else
									ModUnsigned32Value( UNIT_FIELD_HEALTH, -healthtoloose );
							}break;
						//paladin - Improved Lay on Hands
						case __Lay_on_Hands_Rank_1____1:
						case __Lay_on_Hands_Rank_2____1:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_LAY_ON_HANDS )
									continue;
							}break;
						//paladin - Spiritual Attunement
						case __Spiritual_Attunement:
							{
								if( CastingSpell == NULL )
									continue;
								//trigger only on heal spell cast by NOT us
								if( !( CastingSpell->c_is_flags & SPELL_FLAG_IS_HEALING ) || this == victim )
									continue; 
							}break;
						//paladin - Light's Grace
						case __Light_s_Grace:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								if( CastingSpell->NameHash != SPELL_HASH_HOLY_LIGHT )
									continue; 
							}break;
						//paladin - Eye for an Eye
						case __Eye_for_an_Eye_Rank_1____0:
							{
								if( victim == this )
									continue; //not self casted crits
								//requires damageing spell
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								if(!(CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING))
									continue;
								dmg_overwrite = ( dmg *  (ospinfo->EffectBasePoints[0] + 1 )) / 100 ; //only half dmg
								int32 half_health = this->GetUInt32Value(UNIT_FIELD_HEALTH) >> 1;
								if( dmg_overwrite > half_health )
									dmg_overwrite = half_health ;
							}break;
						//paladin - Blessed Life
						case __Blessed_Life_Rank_1:
							{
								//we should test is damage is from enviroment or not :S
								ModUnsigned32Value(UNIT_FIELD_HEALTH,dmg/2);
								continue; //there is no visual for this ?
							}break;
							//paladin - sanctified judgement
						case __Judgements_of_the_Wise:
							{
								//!! not working since we use post even hook and seal disapears before event
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								if(	CastingSpell->NameHash != SPELL_HASH_JUDGEMENT )
									continue;
								if( !IsPlayer() )
									continue; //great, we can only make this for players 
								Player *c = static_cast<Player *>(this);
								//printf("is there a seal on the player ? %u \n",c->Seal);
								if( !c->Seal )
									continue; //how the hack did we manage to cast judgement without a seal ?
								SpellEntry *spellInfo = dbcSpell.LookupEntry( c->Seal ); //null pointer check was already made
								if( !spellInfo )
									continue;	//now this is getting freeky, how the hell did we manage to create this bug ?
								dmg_overwrite = spellInfo->manaCost / 2 ; //only half dmg
								//printf("is there a seal on the player ? %u \n",dmg_overwrite);
							}break;
						//paladin  - Sacred Cleansing
						case __Sacred_Cleansing____0:
							{
								if( CastingSpell == NULL )
									continue;
								if(CastingSpell->NameHash != SPELL_HASH_CLEANSE )
									continue;
							}break;						
						// Righteous Vengeance
						case __Righteous_Vengeance:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_DIVINE_STORM &&
									CastingSpell->buffType != SPELL_TYPE_JUDGEMENT )
									continue;
						dmg_overwrite = ( dmg * (ospinfo->EffectBasePoints[0] + 1 )) / 400 ;
							}break;
						// Paladin - Sheat of Light
						case __Sheath_of_Light:
							{
								if( CastingSpell == NULL )
									continue;
								if(!(CastingSpell->c_is_flags & SPELL_FLAG_IS_HEALING))
									continue; 
								dmg_overwrite = dmg * (ospinfo->EffectBasePoints[1] + 1) / 400;
							}break;
						case __Evasive_Maneuvers: // Trinket: Evasive Maneuvers
							{
								if(GetHealthPct() > 35)
									continue;
							}break;
						//Energized
						case __Energized____2:
							{
								if( CastingSpell == NULL )
									continue;
								if(	CastingSpell->NameHash != SPELL_HASH_LIGHTNING_BOLT )
									continue;
							}break;
						//Spell Haste Trinket
						//http://www.wowhead.com/?item=28190 scarab of the infinite cicle
						case __Spell_Haste:
							{
								if( CastingSpell == NULL )
									continue;
								if( !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
									continue;
							}break;
							// Fathom-Brooch of the Tidewalker proc
						case __Revitalize____1:
							{
								if( !CastingSpell )
									continue;
								if( CastingSpell->School != SCHOOL_NATURE )
									continue;
							}break;
						//shaman - Lightning Overload
						case __Lightning_Overload_Rank_1:
							{
								if( CastingSpell == NULL )
									continue;//this should not ocur unless we made a fuckup somewhere
								//trigger on lightning and chain lightning. Spell should be identical , well maybe next time :P
								if(	CastingSpell->NameHash == SPELL_HASH_LIGHTNING_BOLT || CastingSpell->NameHash == SPELL_HASH_CHAIN_LIGHTNING )
								{
									spellId = CastingSpell->Id;
									dmg_overwrite = (CastingSpell->EffectBasePoints[0] + 1) / 2; //only half dmg
									targets.m_target = ((Unit *)victim->GetGUID());
								}
								else continue;
							}break;
						//item - Band of the Eternal Sage
						case __Band_of_the_Eternal_Sage____0:
							{
								if( CastingSpell == NULL )
									continue;
								if( !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ) ) //requires offensive spell. ! might not cover all spells
									continue;
							}break;
						//Fingers of Frost
						case __Fingers_of_Frost:
						//Brain Freeze
						case __Brain_Freeze:
							{
								if( CastingSpell == NULL)
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_FROSTBOLT &&
									CastingSpell->NameHash != SPELL_HASH_CONE_OF_COLD &&
									CastingSpell->NameHash != SPELL_HASH_FROSTFIRE_BOLT )
									continue;
							}break;
						// druid - Celestial Focus
						case __Starfire_Stun:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_STARFIRE )
									continue;
							}break;
						case __Flexibility: //setbonus
							{
								if (!CastingSpell || CastingSpell->NameHash != SPELL_HASH_FLASH_HEAL)
									continue;
							}break;
						//SETBONUSES
						case __Flameshadow:
							{
								if (!CastingSpell || !(CastingSpell->schoolMask & SCHOOL_MASK_SHADOW) || !(CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING))
									continue;
							}break;
						case __Shadowflame____0:
							{
								if (!CastingSpell || !(CastingSpell->schoolMask & SCHOOL_MASK_FIRE) || !(CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING))
									continue;
							}break;
						case __Wave_Trance:
							{
								if (!CastingSpell ||  !(CastingSpell->c_is_flags & SPELL_FLAG_IS_HEALING))
									continue;
							}break;
						case __Charged:
						case __Energized:
						case __Relentlessness:
							{
								if (!CastingSpell ||  !(CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING))
									continue;
							}break;
						case __Lightning_Bolt_Discount:
							{
								if (!CastingSpell || CastingSpell->NameHash != SPELL_HASH_LIGHTNING_BOLT)
									continue;
							}break;
						case __Infused_Shield:
							{
								if (!CastingSpell || CastingSpell->NameHash != SPELL_HASH_HOLY_SHIELD)
									continue;
							}break;
						case __Divine_Light:
						case __Judgement____4:
							{
								if (!CastingSpell)
									continue;
								if (CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_JUSTICE &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_LIGHT &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_WISDOM &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_RIGHTEOUSNESS &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_BLOOD &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_VENGEANCE &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_COMMAND &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_ONSLAUGHT &&
		 							CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_THE_MARTYR &&
									CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_CORRUPTION &&
		 							CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_RECKONING &&
		 							CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_THE_FLAME &&
		 							CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_DARKNESS &&
		 							CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_THE_VAL_KYR &&
		 							CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_THE_VAL_KYR_OVERSEER &&
		 							CastingSpell->NameHash != SPELL_HASH_JUDGEMENT_OF_WRATH)
									continue;
							}break;
						case __Enlightenment____2:
							{
								if (!CastingSpell || (CastingSpell->NameHash != SPELL_HASH_FLASH_OF_LIGHT && CastingSpell->NameHash != SPELL_HASH_HOLY_LIGHT))
									continue;
							}break;
						case __Overpower_Bonus:
							{
								if (!CastingSpell || CastingSpell->NameHash != SPELL_HASH_OVERPOWER)
									continue;
							}break;
						case __Revenge_Bonus:
							{
								if (!CastingSpell || CastingSpell->Id == 37517 || CastingSpell->NameHash != SPELL_HASH_REVENGE)
									continue; 
							}break;
						//SETBONUSES END
						//Pendulum of Telluric Currents
						case __Pendulum_of_Telluric_Currents____0:
							{
								if (CastingSpell == NULL)
									continue;

								if(!( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ))
									continue;
							}break;
							//http://www.wowhead.com/?item=32493 Ashtongue Talisman of Shadows
						case __Power_of_the_Ashtongue:
							{
								if( CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_CORRUPTION )
									continue; 
							}break;

							//http://www.wowhead.com/?item=32496  Memento of Tyrande
						case __Bonus_Mana_Regen: //dont say damaging spell but EACH time spell is casted there is a chance (so can be healing spell)
							{
								if( CastingSpell == NULL )
									continue;
							}break;
						//http://www.wowhead.com/?item=32488 Ashtongue Talisman of Insight
						case __Insight_of_the_Ashtongue:
							{
								if( CastingSpell == NULL )
									continue;
								if( !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
									continue;
							}break;

							//http://www.wowhead.com/?item=32487 Ashtongue Talisman of Swiftness
						case __Hunter_Tier_6_Trinket:
							{
								if( CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_STEADY_SHOT)
									continue; 
							}break;

							//http://www.wowhead.com/?item=32485 Ashtongue Talisman of Valor
						case __Warrior_Tier_6_Trinket:
							{
								if( CastingSpell == NULL || (CastingSpell->NameHash != SPELL_HASH_MORTAL_STRIKE || CastingSpell->NameHash != SPELL_HASH_BLOODTHIRST || CastingSpell->NameHash != SPELL_HASH_SHIELD_SLAM))
									continue; 
							}break;
						case __Epiphany____0://Epiphany :Each spell you cast can trigger an Epiphany, increasing your mana regeneration by 24 for 30 sec.
							{
								if (!CastingSpell)
									continue;
							}break;
						//SETBONUSES END
						//item - Band of the Eternal Restorer 
						case __Band_of_the_Eternal_Restorer____0:
							{
								if( CastingSpell == NULL )
									continue;
								if(!(CastingSpell->c_is_flags & SPELL_FLAG_IS_HEALING)) //requires healing spell.
									continue;
							}break;

							//http://www.wowhead.com/?item=32486 Ashtongue Talisman of Equilibrium
						case __Druid_Tier_6_Trinket: //Mangle has a 40% chance to grant 140 Strength for 8 sec
							{
								if( CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_MANGLE__BEAR_ || CastingSpell->NameHash != SPELL_HASH_MANGLE__CAT_)
									continue; 
							}break;
						case 40445: //Starfire has a 25% chance to grant up to 150 spell damage for 8 sec
							{
								if( CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_STARFIRE)
									continue; 
							}break;
						case 40446: //Rejuvenation has a 25% chance to grant up to 210 healing for 8 sec
							{
								if( CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_REJUVENATION)
									continue; 
							}break;

							//http://www.wowhead.com/?item=32490 Ashtongue Talisman of Acumen
						case __Priest_Tier_6_Trinket: //Each time your Shadow Word: Pain deals damage, it has a 10% chance to grant you 220 spell damage for 10 sec
							{
								if( CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_SHADOW_WORD__PAIN)
									continue; 
							}break;

							//http://www.wowhead.com/?item=32490 Ashtongue Talisman of Acumen
						case 40440: //Each time your Renew heals, it has a 10% chance to grant you 220 healing for 5 sec
							{
								if( CastingSpell == NULL || CastingSpell->NameHash != SPELL_HASH_RENEW)
									continue; 
							}break;

							//http://www.wowhead.com/?item=32492 Ashtongue Talisman of Lethality
						case __Rogue_Tier_6_Trinket:
							{
								if( CastingSpell == NULL )
									continue;
								//we need a finishing move for this 
								if(!(CastingSpell->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE) || victim==this)
									continue;
							}break;
						case __Mana_Gem_Trinket: //using a mana gem grants you 225 spell damage for 15 sec
							{
								if (!CastingSpell || CastingSpell->NameHash != SPELL_HASH_REPLENISH_MANA)
									continue; 
							}break;

						case __Siphon_Essence:
							{

								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_IMMOLATE &&
									CastingSpell->NameHash != SPELL_HASH_CORRUPTION)
									continue;
							}break;
						
						case __Hot_Streak: // [Mage] Hot Streak
							{
								if (!CastingSpell )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_FIREBALL &&
									CastingSpell->NameHash != SPELL_HASH_FIRE_BLAST &&
									CastingSpell->NameHash != SPELL_HASH_SCORCH &&
									CastingSpell->NameHash != SPELL_HASH_LIVING_BOMB &&
									CastingSpell->NameHash != SPELL_HASH_FROSTFIRE_BOLT )
 									continue; 

								m_hotStreakCount++;
								if (m_hotStreakCount >= 2)
									m_hotStreakCount = 0;
								else
									continue;
							}break;
						case __Astral_Shift: // Astral Shift
							{
								if( !CastingSpell || !spe || 
									!(Spell::HasMechanic(CastingSpell, MECHANIC_STUNNED) ||
									Spell::HasMechanic(CastingSpell, MECHANIC_FLEEING) || 
									Spell::HasMechanic(CastingSpell, MECHANIC_SILENCED)) )
									continue;

								SpellDuration *sd = dbcSpellDuration.LookupEntry(CastingSpell->durationIndex);
								if(!sd)
									continue;
								Aura* aura (new Aura(spe, GetDuration(sd,uint32(10000)), INVOC_DURATION, victim, static_cast< Unit* >(this)));								
								aura->AddMod(SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, -30, 127, 0);
								AddAura(aura,NULL);
								continue;
							}break;
						case __Taste_for_Blood: // Taste for Blood
							{
								if(!CastingSpell || CastingSpell->NameHash != SPELL_HASH_REND)
									continue;
							}break;


						case __Crypt_Fever_Rank_1____0: // crypt fever
						case __Crypt_Fever:
						case __Crypt_Fever____0:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_ICY_TOUCH && CastingSpell->NameHash != SPELL_HASH_PLAGUE_STRIKE)
									continue;
							}break;

						case __Desolation:// Desolation
						case __Desolation____0:
						case __Desolation____1:
						case __Desolation____2:
						case __Desolation____3:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_BLOOD_STRIKE )
									continue;
							}break;

						case __Improved_Blink: //improved blink
						case __Improved_Blink____0:
							{
								if ( !CastingSpell || CastingSpell->NameHash != SPELL_HASH_BLINK )
									continue; 
							}break;
						case __Ancestral_Awakening: // Ancestral Awakening
							{
								if((CastingSpell->NameHash != SPELL_HASH_HEALING_WAVE &&
									CastingSpell->NameHash != SPELL_HASH_LESSER_HEALING_WAVE &&
									CastingSpell->NameHash != SPELL_HASH_RIPTIDE) ||
									!IsPlayer())
										continue;
								targets.m_target = ((Player *)Spell::FindLowestHealthRaidMember(static_cast<Player *>(this), 1600)); // within 40 yards
								if(!targets.m_target)	// shouldn't happen
									continue;
								dmg_overwrite = dmg * (ospinfo->EffectBasePoints[0] + 1) / 100;
							}break;
							
						case __Burning_Determination: //Burning Determination
							{
								if( !CastingSpell || !(Spell::HasMechanic(CastingSpell, MECHANIC_SILENCED) || 
										Spell::HasMechanic(CastingSpell, MECHANIC_INTERRUPTED)))
									continue;
							}break;
						case __Blood_Presence____0:
							{
								//heal him :)
								if( dmg )
								{
									int32 toheal = (int32)(dmg * 0.04);
									Heal( static_cast<Unit *>(this), 50475, toheal );
								}
							}break;

						case __Necrosis: // Necrosis - shadow damage
							{
								if ( dmg )
								{
									dmg_overwrite = dmg * (ospinfo->EffectBasePoints[0] + 1) / 200;
								}
							}break;
						case __Seal_of_Righteousness____1: // Seal of righteousness - Holy damage
						{
							if ( dmg )
							{
								dmg_overwrite = dmg * (ospinfo->EffectBasePoints[0] + 1) / 400;
							}
						}break;
						case __Firestarter: //Firestarter
							{
								if (!CastingSpell )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_BLAST_WAVE &&
									CastingSpell->NameHash != SPELL_HASH_DRAGON_S_BREATH )
									continue; 
							}break;
						case __Frostbite: //Frostbite
							{
								if (!CastingSpell )
									continue;
								//me thinks its correct
								if( !( CastingSpell->SpellGroupType[0] & 0x100220 ) || victim == static_cast<Unit *>(this) ) //frost
									continue;
							}break;
						case __Tidal_Waves: //Tidal Waves
							{
								if (!CastingSpell )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_CHAIN_HEAL &&
									CastingSpell->NameHash != SPELL_HASH_RIPTIDE )
									continue; 
							}break;
							//Earthliving
						case __Earthliving_Rank_1:
						case __Earthliving_Rank_2:
						case __Earthliving_Rank_3:
						case __Earthliving_Rank_4:
						case __Earthliving_Rank_5:
						case __Earthliving_Rank_6:
							{
								if (!CastingSpell )
									continue;
								if( !(CastingSpell->c_is_flags & SPELL_FLAG_IS_HEALING) )
									continue;
							}break;
							//Borrowed Time
						case __Borrowed_Time:
						case __Borrowed_Time____0:
						case __Borrowed_Time____1:
						case __Borrowed_Time____2:
						case __Borrowed_Time____3:
							{
								if (!CastingSpell )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_POWER_WORD__SHIELD )
									continue;
							}break;
						case __Now_is_the_time_: // Sundial of the Exiled
							{
								if(!victim || !isAttackable(this, victim, false))	// harmful spells
									continue;
							}break;
						case __Greatness: // Darkmoon Card: Greatness
							{
								if(!CastingSpell || !(CastingSpell->c_is_flags & (SPELL_FLAG_IS_DAMAGING | SPELL_FLAG_IS_HEALING) ))
									continue;
								// find the maximum stat and proc appropriate spell
								static const uint32 greatness[4] = {60229, 60233, 60234, 60235};
								uint32 stats[4], maxstat = 0;
								stats[0] = GetUInt32Value(UNIT_FIELD_STRENGTH);
								stats[1] = GetUInt32Value(UNIT_FIELD_AGILITY);
								stats[2] = GetUInt32Value(UNIT_FIELD_INTELLECT);
								stats[3] = GetUInt32Value(UNIT_FIELD_SPIRIT);
								for(uint32 i=0; i<4; i++)
								{
									if(stats[i] > stats[maxstat])
										maxstat = i;
								}
								spellId = greatness[maxstat];
							}break;
						case __Improved_Spirit_Tap_Rank_1____0://Improved Spirit Tap
						case __Improved_Spirit_Tap_Rank_2____0:
							{
								if (!CastingSpell )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_MIND_BLAST &&
									CastingSpell->NameHash != SPELL_HASH_SHADOW_WORD__DEATH )
									continue; 
							}break;
						case __Molten_Core://molten core
								{
								if( !CastingSpell )
									continue;

								if( !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
									continue;

								if( CastingSpell->School != SCHOOL_SHADOW )
									continue;
							}break;
						case __Damage_Shield____0:
							{
								float shdmg = 0.0f;
								if( HasDummyAura(SPELL_HASH_DAMAGE_SHIELD) )
									GetDummyAura(SPELL_HASH_DAMAGE_SHIELD)->Id == 58874 ? shdmg = 0.2f : shdmg = 0.1f;

								dmg_overwrite = float2int32(GetUInt32Value(PLAYER_SHIELD_BLOCK) * shdmg);
							}break;
						// Aspect of the Viper
						case __Aspect_of_the_Viper:
						case __Aspect_of_the_Viper____0:
							{
								if( !IsPlayer() )
									continue;

								Player* caster = static_cast<Player *>(this);

								Item* it = caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
								if( !it )
									it = caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);

								if( it )
								{
									float mod = it->GetProto()->Delay / 1000.0f;
									dmg_overwrite = float2int32( 0.01f * mod * caster->GetUInt32Value(UNIT_FIELD_BASE_MANA) );
								}
								else 
									dmg_overwrite = 1;
							}break;
						case __Judgements_of_the_Pure_Rank_1: //judgements of the pure
						case __Judgements_of_the_Pure_Rank_2:
						case __Judgements_of_the_Pure_Rank_3:
						case __Judgements_of_the_Pure_Rank_4:
						case __Judgements_of_the_Pure_Rank_5:
							{
								if (!CastingSpell )
									continue;
								if( CastingSpell->buffIndexType != SPELL_TYPE_INDEX_JUDGEMENT )
									continue; 
							}break;
						case __Glyph_of_Remove_Curse: //Glyph of Remove Curse
							{
								if (!CastingSpell )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_REMOVE_CURSE || victim == this )
									continue;
							}break;
						case __Elemental_Focus: // Elemental focus
							{
								if (!CastingSpell )
									continue;
								if( !( CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
									continue;
								if( CastingSpell->School != SCHOOL_FIRE &&
									CastingSpell->School != SCHOOL_NATURE &&
									CastingSpell->School != SCHOOL_FROST )
									continue;
							}break;
						case __Silenced___Improved_Counterspell_Rank_2:
						case __Silenced___Improved_Counterspell_Rank_1:
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_COUNTERSPELL || victim == this )
									continue;
							}break;
						case __Owlkin_Frenzy: // Owlkin Frenzy
							{
								if( !IsPlayer() )
									continue;

								if( static_cast<Player *>( this )->GetShapeShift() != FORM_MOONKIN )
									continue;
							}break;
						case __Earth_and_Moon____1:
						case __Earth_and_Moon____0:
						case __Earth_and_Moon: // Earth and Moon
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_WRATH &&
									CastingSpell->NameHash != SPELL_HASH_STARFIRE )
									continue;
							}break;
						case __Glyph_of_Heroic_Strike____1: // Glyph of heroic strike
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_HEROIC_STRIKE )
									continue;
							}break;
						case __Missile_Barrage: // Missile Barrage
							{
								if( CastingSpell == NULL )
									continue;
								if( CastingSpell->NameHash != SPELL_HASH_ARCANE_BLAST &&
									CastingSpell->NameHash != SPELL_HASH_ARCANE_BARRAGE &&
									CastingSpell->NameHash != SPELL_HASH_FIREBALL &&
									CastingSpell->NameHash != SPELL_HASH_FROSTBOLT && 
									CastingSpell->NameHash != SPELL_HASH_FROSTFIRE_BOLT )
									continue;
							}break;
						case __Nature_s_Grace____0: 
							{
								if( !CastingSpell )
									continue;
								
								if( !(CastingSpell->c_is_flags & SPELL_FLAG_IS_HEALING) )
									continue;
							}break;
						case __Pain_and_Suffering:
							{
								if( CastingSpell == NULL )
									continue;

								if( CastingSpell->NameHash != SPELL_HASH_MIND_FLAY )
									continue;
							}break;
						// Illumination
						case __Illumination:
							{
								if( CastingSpell == NULL )
									continue;

								if( CastingSpell->NameHash != SPELL_HASH_HOLY_LIGHT &&
									CastingSpell->NameHash != SPELL_HASH_FLASH_OF_LIGHT &&
									CastingSpell->NameHash != SPELL_HASH_HOLY_SHOCK )
									continue;

								dmg_overwrite = float2int32(GetSpellBaseCost(CastingSpell) * 0.6f);
							}break;
						//Fiery Payback
						case __Fiery_Payback_Rank_1:
						case __Fiery_Payback_Rank_2:
							{
								if(GetHealthPct() > 35)
									continue;
							}break;
						// Pallas
						case __Crusader_s_Command_Rank_5:
						case __Justice____0:
							{
								if( !HasDummyAura(SPELL_HASH_SEAL_OF_COMMAND) )
									continue;
							}break;
						// Omen of Clarity
						case __Clearcasting____1:
							{
								if( CastingSpell && !(CastingSpell->c_is_flags & SPELL_FLAG_IS_HEALING || CastingSpell->c_is_flags & SPELL_FLAG_IS_DAMAGING) )
									continue;
							}break;

						// Gag order
						case __Silenced___Gag_Order:
							{
								if( !CastingSpell )
									continue;

								if( CastingSpell->NameHash != SPELL_HASH_SHIELD_BASH && CastingSpell->NameHash != SPELL_HASH_HEROIC_THROW )
									continue;
							}break;
						// Replenishment
						case __Replenishment____0:
							{
								if( !CastingSpell )
									continue;

								if( CastingSpell->NameHash != SPELL_HASH_EXPLOSIVE_SHOT && CastingSpell->NameHash != SPELL_HASH_ARCANE_SHOT && CastingSpell->NameHash != SPELL_HASH_STEADY_SHOT )
									continue;

								if( IsPlayer() )
								{
									Player* caster = static_cast<Player *>( this );
									if( caster->GetGroup() )
									{
										
										uint32 TargetCount = 0;
										caster->GetGroup()->Lock();
										for(uint32 x = 0; x < caster->GetGroup()->GetSubGroupCount(); ++x)
										{
											if( TargetCount == 10 )
												break;

											for(GroupMembersSet::iterator itr = caster->GetGroup()->GetSubGroup( x )->GetGroupMembersBegin(); itr != caster->GetGroup()->GetSubGroup( x )->GetGroupMembersEnd(); ++itr)
											{
												if((*itr)->m_loggedInPlayer && TargetCount <= 10)
												{
													Player* p_target = (*itr)->m_loggedInPlayer;
													if( p_target->GetPowerType() != POWER_TYPE_MANA )
														continue;

													SpellEntry* Replinishment = dbcSpell.LookupEntryForced( 57669 );
													Spell* pSpell = NULL;
													pSpell = (new Spell(static_cast<Player *>( this ), Replinishment, true, NULL));
													pSpell->forced_basepoints[0] = float2int32(p_target->GetUInt32Value(UNIT_FIELD_BASE_MANA) * 0.0025f);
													SpellCastTargets tgt;
													tgt.m_target = ((Player *)p_target->GetGUID());
													pSpell->prepare(&tgt);
													TargetCount++;
												}
											}
										}
										caster->GetGroup()->Unlock();
										continue;
									}
								}
							}break;
						case __Freeze____14:// Frozen Power - Freeze
							{
								uint32 minDistance = spe->EffectBasePoints[0] + 1;
								if(!victim || GetDistanceSq(victim) < minDistance * minDistance)
									continue;	// victim not far enough
							}break;
						case __Improved_Stormstrike: // Improved Stormstrike
							{
								dmg_overwrite = (spe->EffectBasePoints[0] + 1) * GetUInt32Value(UNIT_FIELD_BASE_MANA) / 100;
							}break;
						case __Blade_Barrier_Rank_1____0:
							{
								//Must be player, and not have any blood runes (0x01 and 0x02)
								if (!IsPlayer() || static_cast<Player*>(this)->m_runemask & 0x03)
									continue;
							}break;
					}
				}
				else
				{
					switch( spellId )
					{
						case __Seal_Fate: //Seal Fate
						case __Blood_Frenzy_Rank_1: //Blood Frenzy
						case __Primal_Fury: //Primal Fury
						{
							if( !CastingSpell || 
								CastingSpell->Id == __Seal_Fate ||
								CastingSpell->Id == __Blood_Frenzy_Rank_1 ||
								CastingSpell->Id == __Primal_Fury )
								continue;
						}break;
					}
				}
				if(spellId==17364 || spellId==32175 || spellId==32176) // Stormstrike fix
					continue;
				if(spellId == 22858 && isInBack(victim)) //retatliation needs target to be not in front. Can be casted by creatures too
					continue;
				if( CastingSpell && spellId == 54370 )
				{
					uint32 NewSpellID = 0;
					switch( CastingSpell->School )
					{
					case SCHOOL_HOLY:
						NewSpellID = 54370; break;
					case SCHOOL_FIRE:
						NewSpellID = 54371; break;
					case SCHOOL_NATURE:
						NewSpellID = 54375; break;
					case SCHOOL_FROST:
						NewSpellID = 54372; break;
					case SCHOOL_SHADOW:
						NewSpellID = 54374; break;
					case SCHOOL_ARCANE:
						NewSpellID = 54373; break;
					}
					if( NewSpellID )
						spellId = NewSpellID;
				}
				SpellEntry* spellInfo = dbcSpell.LookupEntry(spellId);
				Spell *spell = new Spell(this, spellInfo ,true, NULL);
				spell->forced_basepoints[0] = dmg_overwrite;
				spell->ProcedOnSpell = CastingSpell;
				if(spellId == 974 || spellId == 32593 || spellId == 32594) // Earth Shield handler
				{
					spell->pSpellId = itr2->spellId;
					spell->SpellEffectDummy(0);
					continue;
				}
				spell->pSpellId=origId;
				spell->prepare(&targets);
				
			} // end of if( spellId && Rand( proc_Chance ) ) (not always we have a spell to cast )
			
		} // if( itr2->procFlags & flag ......
	   } // if( itr2->index < 3)
		
	}

	if (m_procCounter == 1)
	{

		m_chargeSpellsInUse=true;
		std::map<uint32,struct SpellCharge>::iterator iter,iter2;
		iter=m_chargeSpells.begin();
		while(iter!= m_chargeSpells.end())
		{
			iter2=iter++;
			if(iter2->second.count)
			{
				if((iter2->second.ProcFlag & flag))
				{
					//Fixes for spells that dont lose charges when dmg is absorbd
					if(iter2->second.ProcFlag==680&&dmg==0) continue;
					if(CastingSpell)
					{

						SpellCastTime *sd = dbcSpellCastTime.LookupEntry(CastingSpell->castingTimeIndex);
						if(!sd) continue; // this shouldnt happen though :P
						//if we did not proc these then we should not remove them
						if( CastingSpell->Id == iter2->second.spellId)
							continue;
						switch(iter2->second.spellId)
						{
						case 12043:
							{
								//Presence of Mind and Nature's Swiftness should only get removed
								//when a non-instant and bellow 10 sec. Also must be nature :>
	//							if(!sd->CastTime||sd->CastTime>10000) continue;
								if(sd->CastTime==0)
									continue;
							}break;
						case 17116: //Shaman - Nature's Swiftness
						case 16188:
							{
	//							if( CastingSpell->School!=SCHOOL_NATURE||(!sd->CastTime||sd->CastTime>10000)) continue;
								if( !(CastingSpell->schoolMask & SCHOOL_MASK_NATURE) || (sd->CastTime==0)) continue;
							}break;
						case 16166:
							{
								if(!(CastingSpell->schoolMask & (SCHOOL_MASK_FIRE | SCHOOL_MASK_FROST | SCHOOL_MASK_NATURE)))
									continue;
							}break;
						case 14177: //cold blood will get removed on offensive spell
							{
								if(victim == this || isFriendly(this, victim))
									continue;
							}break;
						}
					}
					if(iter2->second.lastproc != 0)
					{
						if(iter2->second.procdiff > 3000)
						{
							--iter2->second.count;
							Aura *aur = FindAura(iter2->second.spellId);
							if (aur == NULL)
								iter2->second.count = 0;
							else
								aur->HandleStackRemove();

						}
					}
					else
					{
						--iter2->second.count;
						Aura* aur=FindAura(iter2->second.spellId);
						if (aur == NULL)
							iter2->second.count = 0;
						else
							aur->HandleStackRemove();
					}
				}
			}
			if(!iter2->second.count)
			{
				m_chargeSpells.erase(iter2);
			}
		}
	}

	for(;!m_chargeSpellRemoveQueue.empty();)
	{
		std::map<uint32,struct SpellCharge>::iterator iter, iter2;
		iter = m_chargeSpells.find(m_chargeSpellRemoveQueue.front());
		if(iter != m_chargeSpells.end())
		{
			if(iter->second.count>1)
				--iter->second.count;
			else
				m_chargeSpells.erase(iter);
		}
		m_chargeSpellRemoveQueue.pop_front();
	}

	m_chargeSpellsInUse=false;
	if(can_delete) //are we the upper level of nested procs ? If yes then we can remove the lock
		bProcInUse = false;
}

//damage shield is a triggered spell by owner to atacker
void Unit::HandleProcDmgShield(uint32 flag, Unit* attacker)
{
	//make sure we do not loop dmg procs
	if(this==attacker || !attacker)
		return;
	if(m_damgeShieldsInUse)
		return;
	m_damgeShieldsInUse = true;
	//charges are already removed in handleproc
	WorldPacket data(24);
	std::list<DamageProc>::iterator i;
	std::list<DamageProc>::iterator i2;
	for(i = m_damageShields.begin();i != m_damageShields.end();)     // Deal Damage to Attacker
	{
		i2 = i++; //we should not proc on proc.. not get here again.. not needed.Better safe then sorry.
		if(	(flag & (*i2).m_flags) )
		{
			if(PROC_MISC & (*i2).m_flags)
			{
				data.Initialize(SMSG_SPELLDAMAGESHIELD);
				data << GetGUID();
				data << attacker->GetGUID();
				data << (*i2).m_spellId;
				data << (*i2).m_damage;
				data << uint32(std::max(int32((*i2).m_damage - attacker->GetUInt32Value(UNIT_FIELD_HEALTH)), 0)); //overkill
				data << uint32(1 << (*i2).m_school);
				SendMessageToSet(&data, true);
				this->DealDamage(attacker, (*i2).m_damage, 0, 0, (*i2).m_spellId);
			}
			else
			{
				SpellEntry	*ability=dbcSpell.LookupEntry((*i2).m_spellId);
				Strike(attacker, RANGED, ability, 0, 0, (*i2).m_damage, true, false);
			}
		}
	}
	m_damgeShieldsInUse = false;
}

/*
void Unit::HandleProcSpellOnSpell(Unit* Victim,uint32 damage,bool critical)
{
}
*/
bool Unit::isCasting()
{
	return (m_currentSpell != NULL);
}

bool Unit::IsInInstance()
{
	MapInfo *pMapinfo = WorldMapInfoStorage.LookupEntry(this->GetMapId());
	if(pMapinfo)
		return (pMapinfo->type != INSTANCE_NULL);

	return false;
}

void Unit::RegenerateHealth()
{
	m_H_regenTimer = 2000;//set next regen time 

	if(!isAlive())
		return;

	// player regen
	if(this->IsPlayer())
	{
		// These only NOT in combat
		if(!CombatStatus.IsInCombat())
		{
			static_cast< Player* >( this )->RegenerateHealth(false);
		}
		else
			static_cast< Player* >( this )->RegenerateHealth(true);
	}
	else
	{
		// Only regen health out of combat
		if(!CombatStatus.IsInCombat())
			static_cast<Creature*>(this)->RegenerateHealth();
	}
}

void Unit::RegeneratePower(bool isinterrupted)
{
    // This is only 2000 IF the power is not rage
	if (isinterrupted)
		m_interruptedRegenTime =2000;
	else
		m_P_regenTimer = 2000;//set next regen time 

	if(!isAlive())
		return;

	// player regen
	if(IsPlayer())
	{
		uint32 powertype = GetPowerType();
		switch(powertype)
		{
		case POWER_TYPE_MANA:
			static_cast< Player* >( this )->RegenerateMana(isinterrupted);
			break;
		case POWER_TYPE_ENERGY:
			static_cast< Player* >( this )->RegenerateEnergy();
			break;

		case POWER_TYPE_RAGE:
			{
				// These only NOT in combat
				if(!CombatStatus.IsInCombat())
				{
					m_P_regenTimer = 1000;
					static_cast<Player*>(this)->LooseRage(30-(m_ModInterrMRegen[POWER_TYPE_RAGE] / 5.0f));
				}
				else
				{
					m_P_regenTimer = 1000;
					static_cast<Player*>(this)->LooseRage(-(m_ModInterrMRegen[POWER_TYPE_RAGE] / 5.0f));
				}

			}break;
		case POWER_TYPE_RUNIC_POWER:
			{
				// These only NOT in combat
				if(!CombatStatus.IsInCombat())
				{
					m_P_regenTimer = 1000;
					static_cast<Player*>(this)->LooseRunic(30 - (m_ModInterrMRegen[POWER_TYPE_RUNIC_POWER] / 5.0f));
				}
				else
				{
					m_P_regenTimer = 1000;
					static_cast<Player*>(this)->LooseRunic(-(m_ModInterrMRegen[POWER_TYPE_RUNIC_POWER] / 5.0f));
				}

			}break;	
		}
		
		/*

		There is a problem here for druids.
		Druids when shapeshifted into bear have 2 power with different regen timers
		a) Mana (which regenerates while shapeshifted
		b) Rage

		Mana has a regen timer of 2 seconds
		Rage has a regen timer of 3 seconds

		I think the only viable way of fixing this is to have 2 different timers
		to check each individual power.

		Atm, mana is being regen at 3 seconds while shapeshifted...

		*/


		// druids regen mana when shapeshifted
		if(getClass() == DRUID && powertype != POWER_TYPE_MANA)
			static_cast< Player* >( this )->RegenerateMana(isinterrupted);
	}
	else
	{
		uint32 powertype = GetPowerType();
		switch(powertype)
		{
		case POWER_TYPE_MANA:
			static_cast<Creature*>(this)->RegenerateMana();
			//Log.Warning("RegeneratePower","Id %u, Type Mana, Interrompu : %s",(Creature *)this->GetEntry(),(isinterrupted)?"Oui":"Non");
			break;
		case POWER_TYPE_FOCUS:
			static_cast<Creature*>(this)->RegenerateFocus();
			//Log.Warning("RegeneratePower",",Id %u, Type Focus, Interrompu : %s",(Creature *)this->GetEntry(),(isinterrupted)?"Oui":"Non");
			m_P_regenTimer = 4000;
			break;
		case POWER_TYPE_ENERGY:
			static_cast< Creature* >( this )->RegenerateEnergy();
			break;	
		}
	}
}

void Unit::CalculateOldResistanceReduction(Unit *pVictim,dealdamage * dmg, SpellEntry* ability)
{
	float AverageResistance = 0.0f;
	float ArmorReduce;

	if((*dmg).school_type == 0)//physical
	{		
		if(this->IsPlayer())
			ArmorReduce = Penetration[0];
		else
			ArmorReduce = 0.0f;

		if(ArmorReduce >= pVictim->GetResistance(0))		// fully penetrated :O
			return;

//		double Reduction = double(pVictim->GetResistance(0)) / double(pVictim->GetResistance(0)+400+(85*getLevel()));
		//dmg reduction formula from xinef
		double Reduction = 0;
		if(getLevel() < 60) Reduction = double(pVictim->GetResistance(0) - ArmorReduce) / double(pVictim->GetResistance(0)+400+(85*getLevel()));
		else if(getLevel() > 59 && getLevel() < 70) Reduction = double(pVictim->GetResistance(0) - ArmorReduce) / double(pVictim->GetResistance(0)-22167.5+(467.5*getLevel()));
		//
		else Reduction = double(pVictim->GetResistance(0) - ArmorReduce) / double(pVictim->GetResistance(0)+10557.5);
		if(Reduction > 0.75f) Reduction = 0.75f;
		else if(Reduction < 0) Reduction = 0;
		if(Reduction) dmg[0].full_damage = (uint32)(dmg[0].full_damage*(1-Reduction));	  // no multiply by 0
	}
	else
	{
		// applying resistance to other type of damage 
		int32 RResist = float2int32( float(pVictim->GetResistance( (*dmg).school_type ) + ((pVictim->getLevel() > getLevel()) ? (pVictim->getLevel() - this->getLevel()) * 5 : 0)) - Penetration[(*dmg).school_type] ); 
		if (RResist<0)
			RResist = 0;
		AverageResistance = (float)(RResist) / (float)(getLevel() * 5) * 0.75f;
		  if(AverageResistance > 0.75f)
			AverageResistance = 0.75f;

		  // NOT WOWWIKILIKE but i think it's actual to add some fullresist chance frome resistances
		  if (!ability || !(ability->attributes & ATTRIBUTES_IGNORE_INVULNERABILITY))
		  {
			  float Resistchance=(float)pVictim->GetResistance( (*dmg).school_type)/(float)pVictim->getLevel();
			  Resistchance*=Resistchance;
			  if(Rand(Resistchance))
				  AverageResistance=1.0f;
		  }

		if(AverageResistance>0)
			(*dmg).resisted_damage = (uint32)(((*dmg).full_damage)*AverageResistance);
		else 
			(*dmg).resisted_damage=0; 
	}
}

double Unit::GetResistanceReducion(Unit *pVictim, uint32 school, float armorReducePct)
{
	double reduction = 0.0;
	float resistance = (float) pVictim->GetResistance(school);
	if(school == 0) // physical
	{
		if(IsPlayer()) // apply armor penetration
			resistance -= Penetration[0] + (resistance * (armorReducePct + static_cast<Player *>(this)->CalcRating( PLAYER_RATING_MODIFIER_ARMOR_PENETRATION_RATING )) / 100.0f);
		
		if(getLevel() < 60) reduction = double(resistance) / double(resistance+400+(85*getLevel()));
		else if(getLevel() > 59 && getLevel() < sWorld.m_levelCap) reduction = double(resistance) / double(resistance-22167.5+(467.5*getLevel()));
		else reduction = double(resistance) / double(resistance+15232.5);
	} else { // non-physical
		double RResist = resistance + float((pVictim->getLevel() > getLevel()) ? (pVictim->getLevel() - getLevel()) * 5 : 0) - Penetration[school]; 
		reduction = RResist / (double)(getLevel() * 5) * 0.75f;
	}
	if(reduction > 0.75) reduction = 0.75;
	else if(reduction < 0) reduction = 0;

	return reduction;
}

void Unit::CalculateResistanceReduction(Unit *pVictim,dealdamage * dmg, SpellEntry* ability, float armorReducePct)
{
	if( (dmg->school_type && ability && Spell::IsBinary(ability)) || dmg->school_type == SCHOOL_HOLY )	// damage isn't reduced for binary spells
	{
		(*dmg).resisted_damage = 0;
		return;
	}

	double reduction = GetResistanceReducion(pVictim, dmg->school_type, armorReducePct);

	// only for physical or non binary spells
	if(reduction>0)
		(*dmg).resisted_damage = (uint32)(((*dmg).full_damage)*reduction);
	else 
		(*dmg).resisted_damage = 0; 
}

uint32 Unit::GetSpellDidHitResult( Unit* pVictim, uint32 weapon_damage_type, SpellEntry* ability, int32 forced_school )
{
	Item * it = NULL;
	float hitchance          = 0.0f;
	float dodge				 = 0.0f;
	float parry				 = 0.0f;
	float block				 = 0.0f;

	float hitmodifier		 = 0;
	int32 self_skill;
	int32 victim_skill;
	uint32 SubClassSkill	 = SKILL_UNARMED;

	bool backAttack			 = !pVictim->isInFront( this ); // isInBack is bugged!
	uint32 vskill            = 0;

	//==========================================================================================
	//==============================Victim Skill Base Calculation===============================
	//==========================================================================================
	if( pVictim->IsPlayer() )
	{
		vskill = static_cast< Player* >( pVictim )->_GetSkillLineCurrent( SKILL_DEFENSE );
		if( weapon_damage_type != RANGED )
		{
			if( !backAttack )
			{
//--------------------------------block chance----------------------------------------------
				if( !pVictim->disarmedShield )
					block = pVictim->GetFloatValue(PLAYER_BLOCK_PERCENTAGE); //shield check already done in Update chances
//--------------------------------dodge chance----------------------------------------------
				if(pVictim->m_auracount[SPELL_AURA_MOD_STUN] <= 0) 
				{
					dodge = pVictim->GetFloatValue( PLAYER_DODGE_PERCENTAGE );
				}
//--------------------------------parry chance----------------------------------------------
				if(pVictim->can_parry && !disarmed)
				{
					parry = pVictim->GetFloatValue( PLAYER_PARRY_PERCENTAGE );
				}
			}
			else if( IsPlayer() )
			{			// you can dodge if anal attacked
//--------------------------------dodge chance----------------------------------------------
				if(pVictim->m_auracount[SPELL_AURA_MOD_STUN] <= 0) 
				{
					dodge = pVictim->GetFloatValue( PLAYER_DODGE_PERCENTAGE );
				}
			}
		}
		victim_skill = float2int32( vskill + static_cast< Player* >( pVictim )->CalcRating( PLAYER_RATING_MODIFIER_DEFENCE ) );
	}
	//--------------------------------mob defensive chances-------------------------------------
	else
	{
		if( weapon_damage_type != RANGED && !backAttack )
			dodge = pVictim->GetUInt32Value(UNIT_FIELD_STAT1) / 14.5f; // what is this value?
		victim_skill = pVictim->getLevel() * 5;
		if(pVictim->m_objectTypeId == TYPEID_UNIT) 
		{ 
			Creature * c = (Creature*)(pVictim);
			if(c&&c->GetCreatureName()&&c->GetCreatureName()->Rank == ELITE_WORLDBOSS)
			{
				victim_skill = std::max(victim_skill,((int32)this->getLevel()+3)*5); //used max to avoid situation when lowlvl hits boss.
			}
		} 
	}
	//==========================================================================================
	//==============================Attacker Skill Base Calculation=============================
	//==========================================================================================
	if(this->IsPlayer())
	{	  
		self_skill = 0;
		Player* pr = static_cast< Player* >( this );
		hitmodifier = pr->GetHitFromMeleeSpell();  

		switch( weapon_damage_type )
		{
		case MELEE:   // melee main hand weapon
			it = disarmed ? NULL : pr->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
			hitmodifier += pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_HIT );
			self_skill = float2int32( pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL ) );
			break;
		case OFFHAND: // melee offhand weapon (dualwield)
			it = disarmed ? NULL : pr->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
			hitmodifier += pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_HIT );
			self_skill = float2int32( pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_OFF_HAND_SKILL ) );
			break;
		case RANGED:  // ranged weapon
			it = disarmed ? NULL : pr->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
			hitmodifier += pr->CalcRating( PLAYER_RATING_MODIFIER_RANGED_HIT );
			self_skill = float2int32( pr->CalcRating( PLAYER_RATING_MODIFIER_RANGED_SKILL ) );
			break;
		}

		// erm. some spells don't use ranged weapon skill but are still a ranged spell and use melee stats instead
		// i.e. hammer of wrath
		if( ability && (ability->NameHash == SPELL_HASH_HAMMER_OF_WRATH || ability->NameHash == SPELL_HASH_AVENGER_S_SHIELD) )
		{
			it = pr->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
			hitmodifier += pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_HIT );
			self_skill = float2int32( pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL ) );
		}
		if(it && it->GetProto())
			SubClassSkill = GetSkillByProto(it->GetProto()->Class,it->GetProto()->SubClass);
		else
			SubClassSkill = SKILL_UNARMED;

		if(SubClassSkill==SKILL_FIST_WEAPONS) 
			SubClassSkill = SKILL_UNARMED;

		//chances in feral form don't depend on weapon skill
		if(static_cast< Player* >( this )->IsInFeralForm()) 
		{
			uint8 form = static_cast< Player* >( this )->GetShapeShift();
			if(form == FORM_CAT || form == FORM_BEAR || form == FORM_DIREBEAR)
			{
				SubClassSkill = SKILL_FERAL_COMBAT;
				// Adjust skill for Level * 5 for Feral Combat
				self_skill += pr->getLevel() * 5;
			}
		}


		self_skill += pr->_GetSkillLineCurrent(SubClassSkill);
	}
	else
	{
		self_skill = this->getLevel() * 5;
		if(m_objectTypeId == TYPEID_UNIT) 
		{ 
			Creature * c = (Creature*)(this);
			if(c != NULL && c->GetCreatureName() != NULL && c->GetCreatureName()->Rank == ELITE_WORLDBOSS)
				self_skill = std::max(self_skill,((int32)pVictim->getLevel()+3)*5);//used max to avoid situation when lowlvl hits boss.
		} 
	}
	//==========================================================================================
	//==============================Special Chances Base Calculation============================
	//==========================================================================================
	//<THE SHIT> to avoid Linux bug. 
	float diffVcapped = (float)self_skill;
	if(int32(pVictim->getLevel()*5)>victim_skill)
		diffVcapped -=(float)victim_skill;
	else
		diffVcapped -=(float)(pVictim->getLevel()*5);

	float diffAcapped = (float)victim_skill;
	if(int32(this->getLevel()*5)>self_skill)
		diffAcapped -=(float)self_skill;
	else
		diffAcapped -=(float)(this->getLevel()*5);
	//<SHIT END>

	//--------------------------------by victim state-------------------------------------------
	if(pVictim->IsPlayer()&&pVictim->GetStandState()) //every not standing state is >0
	{
		hitchance = 100.0f;
	}
	//--------------------------------by damage type and by weapon type-------------------------
	if( weapon_damage_type == RANGED ) 
	{
		dodge=0.0f;
		parry=0.0f;
	}
	else if(this->IsPlayer())
	{
		it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
		if( it != NULL && it->GetProto()->InventoryType == INVTYPE_WEAPON && !ability )//dualwield to-hit penalty
		{
			hitmodifier -= 19.0f;
		}
		else
		{
			it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
			if( it != NULL && it->GetProto()->InventoryType == INVTYPE_2HWEAPON )//2 handed weapon to-hit penalty
				hitmodifier -= 4.0f;
		}
	}

	//--------------------------------by skill difference---------------------------------------
	float vsk = (float)self_skill - (float)victim_skill;
	dodge = std::max( 0.0f, dodge - vsk * 0.04f );
	if( parry )
		parry = std::max( 0.0f, parry - vsk * 0.04f );
	if( block )
		block = std::max( 0.0f, block - vsk * 0.04f );

	if( pVictim->IsPlayer() )
	{
		if( vsk > 0 )
			hitchance = std::max( hitchance, 95.0f + vsk * 0.02f);
		else
			hitchance = std::max( hitchance, 95.0f + vsk * 0.04f);
	} else 
	{
		if(vsk >= -10 && vsk <= 10)
			hitchance = std::max( hitchance, 95.0f + vsk * 0.1f);
		else
			hitchance = std::max( hitchance, 93.0f + (vsk - 10.0f) * 0.4f);
	}
	hitchance += hitmodifier;

	if( ability )
	{
	 if( ability->SpellGroupType[0] || ability->SpellGroupType[1] || ability->SpellGroupType[2] )
	 {
		SM_FFValue( SM_FHitchance, &hitchance, ability );
	 }
	}
	
	// overpower nana
	if( ability != NULL && ability->attributes & ATTRIBUTES_CANT_BE_DPB )
	{
		dodge = 0.0f;
		parry = 0.0f;
		block = 0.0f;
	}

	//==========================================================================================
	//==============================One Roll Processing=========================================
	//==========================================================================================
	//--------------------------------cummulative chances generation----------------------------
	float chances[4];
	chances[0]=std::max(0.0f,100.0f-hitchance);
	chances[1]=chances[0]+dodge;
	chances[2]=chances[1]+parry;
	chances[3]=chances[2]+block;

	//--------------------------------roll------------------------------------------------------
	float Roll = RandomFloat(100.0f);
	uint32 r = 0;
	while (r<4&&Roll>chances[r])
	{
		r++;
	}

	uint32 roll_results[5] = { SPELL_DID_HIT_MISS,SPELL_DID_HIT_DODGE,SPELL_DID_HIT_PARRY,SPELL_DID_HIT_BLOCK,SPELL_DID_HIT_SUCCESS };
	return roll_results[r];
}

void Unit::Strike( Unit* pVictim, uint32 weapon_damage_type, SpellEntry* ability, int32 add_damage, int32 pct_dmg_mod, uint32 exclusive_damage, bool disable_proc, bool skip_hit_check )
{
//==========================================================================================
//==============================Unacceptable Cases Processing===============================
//==========================================================================================
	if(!pVictim->isAlive() || !isAlive()  || IsStunned() || IsPacified() || IsFeared())
		return;
	if(!isInFront(pVictim))
		if(IsPlayer())
		{
			static_cast< Player* >( this )->GetSession()->OutPacket(SMSG_ATTACKSWING_BADFACING);
			return;
		}

	if (IsPlayer())
	{
		AreaTable * at = dbcArea.LookupEntry(static_cast<Player*>(this)->GetAreaID());
		if(at && at->category == AREAC_SANCTUARY || at->AreaFlags & AREA_SANCTUARY)
		{
			Player* pVictimFrom = pVictim->GetPlayerFrom();
			if(pVictimFrom != NULL && static_cast<Player*>(this)->DuelingWith != pVictimFrom)
			{
				static_cast<Player*>(this)->EventAttackStop();
				smsg_AttackStop(pVictim);
				return;
			}
		}
	}

//==========================================================================================
//==============================Variables Initialization====================================
//==========================================================================================
	dealdamage dmg			  = {0,0,0};
	
	Item * it = NULL;
	
	float armorreducepct = 	m_ignoreArmorPct;

	float hitchance          = 0.0f;
	float dodge				 = 0.0f;
	float parry				 = 0.0f;
	float glanc              = 0.0f;
	float block				 = 0.0f;
	float crit				 = 0.0f;
	float crush              = 0.0f;

	uint32 targetEvent		 = 0;
	uint32 hit_status		 = 0;

	uint32 blocked_damage	 = 0;
	int32  realdamage		 = 0;

	uint32 vstate			 = 1;
	uint32 aproc			 = 0;
	uint32 vproc			 = 0;
	   
	float hitmodifier		 = 0;
	int32 self_skill;
	int32 victim_skill;
	uint32 SubClassSkill	 = SKILL_UNARMED;

	bool backAttack			 = !pVictim->isInFront( this );
	uint32 vskill            = 0;
	bool disable_dR			 = false;
	
	if(ability)
		dmg.school_type = ability->School;
	else
	{
		if (GetTypeId() == TYPEID_UNIT)
			dmg.school_type = static_cast< Creature* >( this )->BaseAttackType;
		else
			dmg.school_type = SCHOOL_NORMAL;
	}
//==========================================================================================
//==============================Victim Skill Base Calculation===============================
//==========================================================================================
	if(pVictim->IsPlayer())
	{
		vskill = static_cast<Player *>( pVictim )->_GetSkillLineCurrent( SKILL_DEFENSE );
		if( weapon_damage_type != RANGED )
		{
			if( !backAttack )
			{
//--------------------------------block chance----------------------------------------------
				if( !pVictim->disarmedShield )
					block = pVictim->GetFloatValue(PLAYER_BLOCK_PERCENTAGE); //shield check already done in Update chances
//--------------------------------dodge chance----------------------------------------------
				if(pVictim->m_auracount[SPELL_AURA_MOD_STUN] <= 0) 
				{
					dodge = pVictim->GetFloatValue( PLAYER_DODGE_PERCENTAGE );
				}
//--------------------------------parry chance----------------------------------------------
				if(pVictim->can_parry && !disarmed)
				{
					parry = pVictim->GetFloatValue( PLAYER_PARRY_PERCENTAGE );
				}
			}
			else if( IsPlayer() )
			{			// you can dodge if anal attacked
//--------------------------------dodge chance----------------------------------------------
				if(pVictim->m_auracount[SPELL_AURA_MOD_STUN] <= 0) 
				{
					dodge = pVictim->GetFloatValue( PLAYER_DODGE_PERCENTAGE );
				}
			}
		}
		victim_skill = float2int32( vskill + static_cast< Player* >( pVictim )->CalcRating( PLAYER_RATING_MODIFIER_DEFENCE ) );
	}
//--------------------------------mob defensive chances-------------------------------------
	else
	{
		if( weapon_damage_type != RANGED && !backAttack )
			dodge = pVictim->GetUInt32Value( UNIT_FIELD_STAT1 ) / 14.5f; // what is this value? (Agility)
		victim_skill = pVictim->getLevel() * 5;
		if(pVictim->IsCreature()) 
		{ 
			Creature* c = static_cast< Creature* >( pVictim );
			if(c->GetCreatureName() != NULL && c->GetCreatureName()->Rank == ELITE_WORLDBOSS)
			{
				victim_skill = std::max( victim_skill, ( (int32)getLevel() + 3 ) * 5 ); //used max to avoid situation when lowlvl hits boss.
			}
		} 
	}
//==========================================================================================
//==============================Attacker Skill Base Calculation=============================
//==========================================================================================
	if( this->IsPlayer() )
	{	  
		self_skill = 0;
		Player* pr = static_cast< Player* >( this );
		hitmodifier = pr->GetHitFromMeleeSpell();  
		
		switch( weapon_damage_type )
		{
		case MELEE:   // melee main hand weapon
			it = disarmed ? NULL : pr->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
			self_skill = float2int32( pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL ) );
			if (it && it->GetProto())
			{
				dmg.school_type = it->GetProto()->Damage[0].Type;
				if( it->GetProto()->SubClass == ITEM_SUBCLASS_WEAPON_MACE )
					armorreducepct += m_ignoreArmorPctMaceSpec;
			}
			break;
		case OFFHAND: // melee offhand weapon (dualwield)
			it = disarmed ? NULL : pr->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
			self_skill = float2int32( pr->CalcRating( PLAYER_RATING_MODIFIER_MELEE_OFF_HAND_SKILL ) );
			hit_status |= HITSTATUS_DUALWIELD;//animation
			if (it && it->GetProto())
			{
				dmg.school_type = it->GetProto()->Damage[0].Type;
				if( it->GetProto()->SubClass == ITEM_SUBCLASS_WEAPON_MACE )
					armorreducepct += m_ignoreArmorPctMaceSpec;
			}
			break;
		case RANGED:  // ranged weapon
			it = disarmed ? NULL : pr->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
			self_skill = float2int32( pr->CalcRating( PLAYER_RATING_MODIFIER_RANGED_SKILL ) );
			if (it && it->GetProto())
				dmg.school_type = it->GetProto()->Damage[0].Type;
			break;
		}

		if(it && it->GetProto())
		{
			SubClassSkill = GetSkillByProto(it->GetProto()->Class,it->GetProto()->SubClass);
			if(SubClassSkill==SKILL_FIST_WEAPONS) 
				SubClassSkill = SKILL_UNARMED;
		}
		else
			SubClassSkill = SKILL_UNARMED;


		//chances in feral form don't depend on weapon skill
		if(pr->IsInFeralForm()) 
		{
			uint8 form = pr->GetShapeShift();
			if(form == FORM_CAT || form == FORM_BEAR || form == FORM_DIREBEAR)
			{
				SubClassSkill = SKILL_FERAL_COMBAT;
				self_skill += pr->getLevel() * 5;
			}
		}

		self_skill += pr->_GetSkillLineCurrent(SubClassSkill);
		crit = GetFloatValue(PLAYER_CRIT_PERCENTAGE);
	}
	else
	{
		self_skill = this->getLevel() * 5;
		if(m_objectTypeId == TYPEID_UNIT) 
		{ 
			Creature * c = (Creature*)(this);
			if(c&&c->GetCreatureName()&&c->GetCreatureName()->Rank == ELITE_WORLDBOSS)
				self_skill = std::max(self_skill,((int32)pVictim->getLevel()+3)*5);//used max to avoid situation when lowlvl hits boss.
		} 
		crit = 5.0f; //will be modified later
	}

//==========================================================================================
//==============================Special Chances Base Calculation============================
//==========================================================================================
//<THE SHIT> to avoid Linux bug. 
float diffVcapped = (float)self_skill;
if(int32(pVictim->getLevel()*5)>victim_skill)
	diffVcapped -=(float)victim_skill;
else
	diffVcapped -=(float)(pVictim->getLevel()*5);

float diffAcapped = (float)victim_skill;
if(int32(getLevel()*5)>self_skill)
	diffAcapped -=(float)self_skill;
else
	diffAcapped -=(float)(getLevel()*5);
//<SHIT END>

//--------------------------------crushing blow chance--------------------------------------
	if(pVictim->IsPlayer()&&!IsPlayer()&&!ability && !dmg.school_type)
	{
		int32 LevelDiff = GetUInt32Value(UNIT_FIELD_LEVEL) - pVictim->GetUInt32Value(UNIT_FIELD_LEVEL);
		if(diffVcapped>=15.0f && LevelDiff >= 4)
			crush = -15.0f+2.0f*diffVcapped; 
		else 
			crush = 0.0f;
	}
//--------------------------------glancing blow chance--------------------------------------
	if(IsPlayer()&&!pVictim->IsPlayer()&&!ability)
	{
		glanc = 10.0f + diffAcapped;
		if(glanc<0)
			glanc = 0.0f;
	}
//==========================================================================================
//==============================Advanced Chances Modifications==============================
//==========================================================================================
//--------------------------------by talents------------------------------------------------
	if(pVictim->IsPlayer())
	{
		if( weapon_damage_type != RANGED )
		{
			crit += static_cast< Player* >(pVictim)->res_M_crit_get();
			hitmodifier += static_cast< Player* >(pVictim)->m_resist_hit[0];
		}
		else 
		{
			crit += static_cast< Player* >(pVictim)->res_R_crit_get(); //this could be ability but in that case we overwrite the value
			hitmodifier += static_cast< Player* >(pVictim)->m_resist_hit[1];
		}
	}
	crit += (float)(pVictim->AttackerCritChanceMod[0]);
//--------------------------------by skill difference---------------------------------------

	float vsk = (float)self_skill - (float)victim_skill;
	dodge = std::max( 0.0f, dodge - vsk * 0.04f );
	if( parry )
		parry = std::max( 0.0f, parry - vsk * 0.04f );
	if( block )
		block = std::max( 0.0f, block - vsk * 0.04f );

	crit += pVictim->IsPlayer() ? vsk * 0.04f : min( vsk * 0.2f, 0.0f ); 

	if( pVictim->IsPlayer() )
	{
		if( vsk > 0 )
			hitchance = std::max( hitchance, 95.0f + vsk * 0.02f);
		else
			hitchance = std::max( hitchance, 95.0f + vsk * 0.04f);
	} else 
	{
		if(vsk >= -10 && vsk <= 10)
			hitchance = std::max( hitchance, 95.0f + vsk * 0.1f);
		else
			hitchance = std::max( hitchance, 93.0f + (vsk - 10.0f) * 0.4f);
	}
//--------------------------------by ratings------------------------------------------------
	crit -= pVictim->IsPlayer() ? static_cast< Player* >(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE ) : 0.0f;
	if(crit<0) crit=0.0f;
	if (this->IsPlayer())
	{
		hitmodifier += (weapon_damage_type == RANGED) ? static_cast< Player* >(this)->CalcRating( PLAYER_RATING_MODIFIER_RANGED_HIT ) : static_cast< Player* >(this)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_HIT );
		
		float expertise_bonus = static_cast< Player* >(this)->CalcRating( PLAYER_RATING_MODIFIER_EXPERTISE );
		if(weapon_damage_type == MELEE)
			expertise_bonus += static_cast< Player* >(this)->GetUInt32Value(PLAYER_EXPERTISE);
		else if(weapon_damage_type == OFFHAND)
			expertise_bonus += static_cast< Player* >(this)->GetUInt32Value(PLAYER_OFFHAND_EXPERTISE);
		expertise_bonus *= 0.25f;
		dodge -= expertise_bonus;
		if(dodge<0) dodge=0.0f;
		parry -= expertise_bonus;
		if(parry<0) parry=0.0f;
	}

//--------------------------------by damage type and by weapon type-------------------------
	if( weapon_damage_type == RANGED ) 
	{
		dodge=0.0f;
		parry=0.0f;
		glanc=0.0f;
	}
	else
		if(this->IsPlayer())
		{
			it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND );
			if( it != NULL && it->GetProto()->InventoryType == INVTYPE_WEAPON && !ability )//dualwield to-hit penalty
				hitmodifier -= 19.0f; //titan's grip suffers 15% loss, dual wield suffers 19% loss
			else
			{
				it = static_cast< Player* >( this )->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
				if( it != NULL && it->GetProto()->InventoryType == INVTYPE_2HWEAPON )//2 handed weapon to-hit penalty
					hitmodifier -= 4.0f;
			}
		}

	hitchance += hitmodifier;

	if(ability)
	{
		if(ability->SpellGroupType[0] || ability->SpellGroupType[1] || ability->SpellGroupType[2])
		{
			SM_FFValue(SM_CriticalChance,&crit,ability);
			SM_FFValue(SM_FHitchance,&hitchance,ability);
		}
	}

	//Hackfix for Surprise Attacks
	if(  this->IsPlayer() && ability && static_cast< Player* >( this )->m_finishingmovesdodge && ability->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE)
			dodge = 0.0f;

	if( skip_hit_check )
	{
		hitchance = 100.0f;
		dodge = 0.0f;
		parry = 0.0f;
		block = 0.0f;
	}

	if( ability != NULL && ability->attributes & ATTRIBUTES_CANT_BE_DPB )
	{
		dodge = 0.0f;
		parry = 0.0f;
		block = 0.0f;
	}

//--------------------------------by victim state-------------------------------------------
	if(pVictim->IsPlayer() && pVictim->GetStandState()) //every not standing state is >0
	{
		hitchance = 100.0f;
		dodge = 0.0f;
		parry = 0.0f;
		block = 0.0f;
		crush = 0.0f;
		crit = 100.0f;
	}
//==========================================================================================
//==============================One Roll Processing=========================================
//==========================================================================================
//--------------------------------cummulative chances generation----------------------------
	float chances[7];
	chances[0]=std::max(0.0f,100.0f-hitchance);
	chances[1]=chances[0]+dodge;
	chances[2]=chances[1]+parry;
	chances[3]=chances[2]+glanc;
	chances[4]=chances[3]+block;
	chances[5]=chances[4]+crit;
	chances[6]=chances[5]+crush;
//--------------------------------roll------------------------------------------------------
	float Roll = RandomFloat(100.0f);
	uint32 r = 0;
	while (r < 7 && Roll > chances[r])
	{
		r++;
	}
//--------------------------------postroll processing---------------------------------------
	uint32 abs = 0;

	switch(r)
	{ 
//--------------------------------miss------------------------------------------------------
	case 0:
		hit_status |= HITSTATUS_MISS;
		// dirty ai agro fix
		if(pVictim->GetTypeId() == TYPEID_UNIT && pVictim->GetAIInterface()->GetNextTarget() == NULL)
			pVictim->GetAIInterface()->AttackReaction(this, 1, 0);
		break;
//--------------------------------dodge-----------------------------------------------------
	case 1:
		// dirty ai agro fix
		if(pVictim->GetTypeId() == TYPEID_UNIT && pVictim->GetAIInterface()->GetNextTarget() == NULL)
			pVictim->GetAIInterface()->AttackReaction(this, 1, 0);

		CALL_SCRIPT_EVENT(pVictim, OnTargetDodged)(this);
		CALL_SCRIPT_EVENT(this, OnDodged)(this);
		targetEvent = 1;
		vstate = DODGE;
		vproc |= PROC_ON_DODGE_VICTIM;
		pVictim->Emote(EMOTE_ONESHOT_PARRYUNARMED);			// Animation
		if( pVictim->IsPlayer() )
		{
			//allmighty warrior overpower
			if( this->IsPlayer() && static_cast< Player* >( this )->getClass() == WARRIOR )
			{
				static_cast< Player* >( this )->AddComboPoints( pVictim->GetGUID(), 1 );
				static_cast< Player* >( this )->UpdateComboPoints();
				if( !sEventMgr.HasEvent( static_cast< Player* >( this ), EVENT_COMBO_POINT_CLEAR_FOR_TARGET ) )
					sEventMgr.AddEvent( static_cast< Player* >( this ), &Player::NullComboPoints, (uint32)EVENT_COMBO_POINT_CLEAR_FOR_TARGET, (uint32)5000, (uint32)1, (uint32)0 );
				else
					sEventMgr.ModifyEventTimeLeft( static_cast< Player* >( this ), EVENT_COMBO_POINT_CLEAR_FOR_TARGET, 5000 ,0 );
			}
		}
		if( pVictim->IsPlayer() )
		{
			Player* vplr = static_cast<Player *>( pVictim );
			if( vplr->getClass() == DRUID )
			{
				if( (vplr->GetShapeShift() == FORM_BEAR ||  
					 vplr->GetShapeShift() == FORM_DIREBEAR) &&
					 vplr->HasDummyAura(SPELL_HASH_NATURAL_REACTION) )
				{
					switch(vplr->GetDummyAura(SPELL_HASH_NATURAL_REACTION)->RankNumber)
					{
					case 1: vplr->CastSpell(vplr, 57893, true); break;
					case 2: vplr->CastSpell(vplr, 59071, true); break;
					case 3: vplr->CastSpell(vplr, 59072, true); break;
					}	
				}
			}
			else if( vplr->getClass() == ROGUE )
			{
				if( vplr->HasDummyAura(SPELL_HASH_SETUP) )
				{
					if( Rand((vplr->GetDummyAura(SPELL_HASH_SETUP)->RankNumber * 33) + 1) )
						vplr->CastSpell(vplr, 15250, true);
				}
			}
		}

			pVictim->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_DODGE_BLOCK);	//SB@L: Enables spells requiring dodge
			if(!sEventMgr.HasEvent(pVictim,EVENT_DODGE_BLOCK_FLAG_EXPIRE))
				sEventMgr.AddEvent(pVictim,&Unit::EventAurastateExpire,(uint32)AURASTATE_FLAG_DODGE_BLOCK,EVENT_DODGE_BLOCK_FLAG_EXPIRE,5000,1,0);
			else sEventMgr.ModifyEventTimeLeft(pVictim,EVENT_DODGE_BLOCK_FLAG_EXPIRE,5000,0);
		break;
//--------------------------------parry-----------------------------------------------------
	case 2:
		// dirty ai agro fix
		if(pVictim->GetTypeId() == TYPEID_UNIT && pVictim->GetAIInterface()->GetNextTarget() == NULL)
			pVictim->GetAIInterface()->AttackReaction(this, 1, 0);

		CALL_SCRIPT_EVENT(pVictim, OnTargetParried)(this);
		CALL_SCRIPT_EVENT(this, OnParried)(this);
		targetEvent = 3;
		vstate = PARRY;
		pVictim->Emote(EMOTE_ONESHOT_PARRYUNARMED);			// Animation
		if(pVictim->IsPlayer())
		{
			if( static_cast< Player* >( pVictim )->getClass() == 1 || static_cast< Player* >( pVictim )->getClass() == 4 )//warriors for 'revenge' and rogues for 'riposte'
			{
				//pVictim->SetFlag( UNIT_FIELD_AURASTATE,AURASTATE_FLAG_DODGE_BLOCK );	// Enables spells requiring dodge
				if(!sEventMgr.HasEvent( pVictim, EVENT_DODGE_BLOCK_FLAG_EXPIRE ) )
					sEventMgr.AddEvent( pVictim, &Unit::EventAurastateExpire, (uint32)AURASTATE_FLAG_DODGE_BLOCK, EVENT_DODGE_BLOCK_FLAG_EXPIRE, 5000, 1, 0 );
				else 
					sEventMgr.ModifyEventTimeLeft( pVictim, EVENT_DODGE_BLOCK_FLAG_EXPIRE, 5000 );
			}

			if( pVictim->HasDummyAura(SPELL_HASH_BLESSING_OF_SANCTUARY) || pVictim->HasDummyAura(SPELL_HASH_GREATER_BLESSING_OF_SANCTUARY) )
			{
				switch( pVictim->GetPowerType() )
				{
					case POWER_TYPE_MANA: CastSpell(pVictim, 57319, true); break;
					case POWER_TYPE_RAGE: CastSpell(pVictim, 57320, true); break;
					case POWER_TYPE_RUNIC_POWER: CastSpell(pVictim, 57321, true); break;
				}
			}

			pVictim->SetFlag( UNIT_FIELD_AURASTATE,AURASTATE_FLAG_PARRY );	//SB@L: Enables spells requiring parry
			if(!sEventMgr.HasEvent( pVictim, EVENT_PARRY_FLAG_EXPIRE ) )
				sEventMgr.AddEvent( pVictim, &Unit::EventAurastateExpire, (uint32)AURASTATE_FLAG_PARRY,EVENT_PARRY_FLAG_EXPIRE, 5000, 1, 0 );
			else 
				sEventMgr.ModifyEventTimeLeft( pVictim, EVENT_PARRY_FLAG_EXPIRE, 5000 );
		}
		break;
//--------------------------------not miss,dodge or parry-----------------------------------
	default:
		hit_status |= HITSTATUS_HITANIMATION;//hit animation on victim
		if( pVictim->SchoolImmunityList[0] )
			vstate = IMMUNE;		
		else
		{
//--------------------------------state proc initialization---------------------------------
			vproc |= PROC_ON_ANY_DAMAGE_VICTIM;			
			if( weapon_damage_type != RANGED) 
			{
				aproc |= PROC_ON_MELEE_HIT;
				vproc |= PROC_ON_MELEE_HIT_VICTIM;
			}
			else
			{
				aproc |= PROC_ON_RANGED_HIT;
				vproc |= PROC_ON_RANGED_HIT_VICTIM;
				if( ability && ability->Id == 3018 && IsPlayer() && getClass() == HUNTER )
					aproc |= PROC_ON_AUTO_SHOT_HIT;
			}
//--------------------------------base damage calculation-----------------------------------
			if(exclusive_damage)
				dmg.full_damage = exclusive_damage;
			else
			{
				if( weapon_damage_type == MELEE && ability )
					dmg.full_damage = CalculateDamage( this, pVictim, MELEE, ability );
				else			
					dmg.full_damage = CalculateDamage( this, pVictim, weapon_damage_type, ability );
			}

			if( weapon_damage_type == RANGED )
			{
				dmg.full_damage += pVictim->RangedDamageTaken;
			}
			
			if( ability && ability->mechanics == MECHANIC_BLEEDING )
				disable_dR = true; 
			
			if( pct_dmg_mod > 0 )
				dmg.full_damage = float2int32( dmg.full_damage *  ( float( pct_dmg_mod) / 100.0f ) );

			dmg.full_damage += add_damage;

			// Bonus damage
			if(ability)
			{
				if(ability->SpellGroupType[0] || ability->SpellGroupType[1] || ability->SpellGroupType[2])
				{	
					SM_FIValue(((Unit*)this)->SM_FDamageBonus,&dmg.full_damage,ability);
					SM_PIValue(((Unit*)this)->SM_PDamageBonus,&dmg.full_damage,ability);
				}
			}
			dmg.full_damage += pVictim->DamageTakenMod[dmg.school_type];

			//pet happiness state dmg modifier
			if( IsPet() && !static_cast<Pet*>(this)->IsSummon() )
				dmg.full_damage = ( dmg.full_damage <= 0 ) ? 0 : float2int32( dmg.full_damage * static_cast< Pet* >( this )->GetHappinessDmgMod() );
			
			if( HasDummyAura(SPELL_HASH_REND_AND_TEAR) && ability &&
				( ability->NameHash == SPELL_HASH_MAUL || ability->NameHash == SPELL_HASH_SHRED) && pVictim->ModDamageTakenByMechPCT[MECHANIC_BLEEDING] )
			{
				dmg.full_damage += float2int32(dmg.full_damage * ( ( GetDummyAura(SPELL_HASH_REND_AND_TEAR)->RankNumber * 4 ) / 100.f ) );
			}

			if(dmg.full_damage < 0)
				dmg.full_damage = 0;
//--------------------------------check for special hits------------------------------------
			switch(r)
			{
//--------------------------------glancing blow---------------------------------------------
			case 3:
				{
					float low_dmg_mod = 1.5f - (0.05f * diffAcapped);
					if(this->getClass() == MAGE || this->getClass() == PRIEST || this->getClass() == WARLOCK) //casters = additional penalty.
					{
						low_dmg_mod -= 0.7f;
					}
					if(low_dmg_mod<0.01)
						low_dmg_mod = 0.01f;
					if(low_dmg_mod>0.91)
						low_dmg_mod = 0.91f;
					float high_dmg_mod = 1.2f - (0.03f * diffAcapped);
					if(this->getClass() == MAGE || this->getClass() == PRIEST || this->getClass() == WARLOCK) //casters = additional penalty.
					{
						high_dmg_mod -= 0.3f;
					}
					if(high_dmg_mod>0.99)
						high_dmg_mod = 0.99f;
					if(high_dmg_mod<0.2)
						high_dmg_mod = 0.2f;

					float damage_reduction = (high_dmg_mod + low_dmg_mod) / 2.0f;
					if(damage_reduction > 0)
					{
							dmg.full_damage = float2int32(damage_reduction * float(dmg.full_damage));
					}
					hit_status |= HITSTATUS_GLANCING;
				}
				break;
//--------------------------------block-----------------------------------------------------
			case 4:
				{
					Item* shield = static_cast< Player* >( pVictim )->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
					if( shield != NULL )
					{
						targetEvent = 2;
						pVictim->Emote(EMOTE_ONESHOT_PARRYSHIELD);// Animation

						if( shield->GetProto()->InventoryType == INVTYPE_SHIELD )
						{
							float block_multiplier = ( 100.0f + float( static_cast<Player *>( pVictim )->m_modblockabsorbvalue ) ) / 100.0f;
							if( block_multiplier < 1.0f )block_multiplier = 1.0f;

							blocked_damage = float2int32( float( shield->GetProto()->Block ) + ( float( static_cast<Player *>( pVictim )->m_modblockvaluefromspells + pVictim->GetUInt32Value( PLAYER_RATING_MODIFIER_BLOCK ) ) * block_multiplier ) + ( ( float( pVictim->GetUInt32Value( UNIT_FIELD_STAT0 ) ) / 20.0f ) - 1.0f ) );
						}
						else
						{
							blocked_damage = 0;
						}

						if(dmg.full_damage <= (int32)blocked_damage)
						{
							CALL_SCRIPT_EVENT(pVictim, OnTargetBlocked)(this, blocked_damage);
							CALL_SCRIPT_EVENT(this, OnBlocked)(pVictim, blocked_damage);
							vstate = BLOCK;
							vproc |= PROC_ON_BLOCK_VICTIM;
						}
						if( pVictim->IsPlayer() )//not necessary now but we'll have blocking mobs in future
						{            
							pVictim->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_DODGE_BLOCK);	//SB@L: Enables spells requiring dodge
							if(!sEventMgr.HasEvent(pVictim,EVENT_DODGE_BLOCK_FLAG_EXPIRE))
								sEventMgr.AddEvent(pVictim, &Unit::EventAurastateExpire,(uint32)AURASTATE_FLAG_DODGE_BLOCK,EVENT_DODGE_BLOCK_FLAG_EXPIRE,5000,1,0);
							else 
								sEventMgr.ModifyEventTimeLeft(pVictim,EVENT_DODGE_BLOCK_FLAG_EXPIRE,5000);
						}
						
						// Holy shield fix
						pVictim->HandleProcDmgShield(PROC_ON_BLOCK_VICTIM, (this));
					}
				}
				break;
//--------------------------------critical hit----------------------------------------------
			case 5:
				{
					hit_status |= HITSTATUS_CRICTICAL;
					float dmg_bonus_pct = 100.0f;
					//sLog.outString( "DEBUG: Critical Strike! Full_damage: %u" , dmg.full_damage );
					if(ability)
					{
						if(ability->SpellGroupType[0] || ability->SpellGroupType[1] || ability->SpellGroupType[2])
							SM_FFValue(SM_PCriticalDamage,&dmg_bonus_pct,ability);

					}
					if( IsPlayer() )
					{
						// m_modphyscritdmgPCT
						if( weapon_damage_type != RANGED )
						{
							dmg_bonus_pct += (float)static_cast<Player *>( this )->m_modphyscritdmgPCT;
						}

						// IncreaseCriticalByTypePct
						if( !pVictim->IsPlayer() )
						{
							CreatureInfo *pCreatureName = static_cast<Creature *>(pVictim)->GetCreatureInfo();
							if( pCreatureName != NULL )
								dmg_bonus_pct += static_cast<Player *>( this )->IncreaseCricticalByTypePCT[pCreatureName->Type];
						}

						// UGLY GOUGE HAX
						// too lazy to fix this properly...
						if( this && this->IsPlayer() && static_cast<Player *>(this)->HasDummyAura(SPELL_HASH_SEAL_FATE) && ability && ( ability->NameHash == SPELL_HASH_GOUGE || ability->NameHash == SPELL_HASH_MUTILATE ) )
						{
							if( Rand( static_cast<Player *>(this)->GetDummyAura(SPELL_HASH_SEAL_FATE)->RankNumber * 20.0f ) )
								static_cast<Player *>(this)->AddComboPoints(pVictim->GetGUID(), 1);
						}
						else if( HasDummyAura( SPELL_HASH_PREY_ON_THE_WEAK ) )
						{
							if( pVictim->GetHealthPct() < GetHealthPct() )
								dmg.full_damage += dmg.full_damage * ((GetDummyAura(SPELL_HASH_PREY_ON_THE_WEAK)->RankNumber * 4) / 100);
						}

						this->SetFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_CRITICAL);	//SB@L: Enables spells requiring critical strike
						if(!sEventMgr.HasEvent(this,EVENT_CRIT_FLAG_EXPIRE))
							sEventMgr.AddEvent((Unit*)this,&Unit::EventAurastateExpire,(uint32)AURASTATE_FLAG_CRITICAL,EVENT_CRIT_FLAG_EXPIRE,5000,1,0);
						else 
							sEventMgr.ModifyEventTimeLeft(this,EVENT_CRIT_FLAG_EXPIRE,5000);
					}
					// SpellAuraReduceCritRangedAttackDmg
					if( weapon_damage_type == RANGED )
						dmg.full_damage = dmg.full_damage - float2int32(dmg.full_damage * CritRangedDamageTakenPctMod[dmg.school_type]) / 100;
					else 
						dmg.full_damage = dmg.full_damage - float2int32(dmg.full_damage * CritMeleeDamageTakenPctMod[dmg.school_type]) / 100;

					// actual crit damage?
					if( dmg_bonus_pct > 0 )
						dmg.full_damage += float2int32( float(dmg.full_damage) * (dmg_bonus_pct / 100.0f));

					if(pVictim->IsPlayer())
					{
						//Resilience is a special new rating which was created to reduce the effects of critical hits against your character.
						float dmg_reduction_pct = 2.2f * static_cast< Player* >(pVictim)->CalcRating( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE ) / 100.0f;
						if( dmg_reduction_pct > 0.33f )
							dmg_reduction_pct = 0.33f; // 3.0.3
						dmg.full_damage = float2int32( dmg.full_damage - dmg.full_damage*dmg_reduction_pct );
						//sLog.outString( "DEBUG: After Resilience check: %u" , dmg.full_damage );
					}

					//emote
					pVictim->Emote(EMOTE_ONESHOT_WOUNDCRITICAL);
					vproc |= PROC_ON_CRIT_ATTACK_VICTIM;
					aproc |= PROC_ON_CRIT_ATTACK;
					if( weapon_damage_type == RANGED )
					{
						vproc |= PROC_ON_RANGED_HIT_VICTIM;
						aproc |= PROC_ON_RANGED_HIT;
					}



					CALL_SCRIPT_EVENT(pVictim, OnTargetCritHit)(this, float(dmg.full_damage));
					CALL_SCRIPT_EVENT(this, OnCritHit)(pVictim, float(dmg.full_damage));
				}
				break;
//--------------------------------crushing blow---------------------------------------------
			case 6:
				hit_status |= HITSTATUS_CRUSHINGBLOW;
				dmg.full_damage = (dmg.full_damage * 3) >> 1;
				break;
//--------------------------------regular hit-----------------------------------------------
			default:
				break;	
			}
//==========================================================================================
//==============================Post Roll Damage Processing=================================
//==========================================================================================
//--------------------------absorption------------------------------------------------------
			uint32 dm = dmg.full_damage;
			abs = pVictim->AbsorbDamage(this, dmg.school_type,(uint32*)&dm, ability);
		
			if(dmg.full_damage > (int32)blocked_damage)
			{
				uint32 sh = pVictim->ManaShieldAbsorb(dmg.full_damage, ability);
//--------------------------armor reducing--------------------------------------------------
				if(sh)
				{
					dmg.full_damage -= sh;
					if(dmg.full_damage && !disable_dR)
						CalculateOldResistanceReduction(pVictim, &dmg, ability);
					dmg.full_damage += sh;
					abs+=sh;
				}
				else if(!disable_dR)
					CalculateOldResistanceReduction(pVictim,&dmg, ability);	
			}

			/*if(abs)
				vproc |= PROC_ON_ABSORB;*/

			if (dmg.school_type == SCHOOL_NORMAL)
			{
				abs+=dmg.resisted_damage;
				dmg.resisted_damage=0;
			}

			realdamage = dmg.full_damage-abs-dmg.resisted_damage-blocked_damage;
			if(realdamage < 0)
			{
				realdamage = 0;
				vstate = IMMUNE;
				if (!(hit_status & HITSTATUS_BLOCK))
					hit_status |= HITSTATUS_ABSORBED;
			}
		}
		break;
	}

//==========================================================================================
//==============================Post Roll Special Cases Processing==========================
//==========================================================================================
//------------------------------- Special Effects Processing
// Paladin: Blessing of Sacrifice, and Warlock: Soul Link
		if( !pVictim->m_damageSplitTargets.empty() )
		{
			std::list< DamageSplitTarget >::iterator itr;
			Unit * splittarget = NULL;
			uint32 splitdamage = 0, tmpsplit = 0;
			for( itr = pVictim->m_damageSplitTargets.begin() ; itr != pVictim->m_damageSplitTargets.end() ; itr ++ )
			{
				// TODO: Separate damage based on school.
				splittarget = pVictim->GetMapMgr() ? pVictim->GetMapMgr()->GetUnit( itr->m_target ) : NULL;
				if( splittarget && dmg.full_damage > 0 )
				{
					// calculate damage
					tmpsplit = itr->m_flatDamageSplit;
					if( tmpsplit > (uint32)dmg.full_damage )
						tmpsplit = dmg.full_damage; // prevent < 0 damage
					splitdamage = tmpsplit;
					dmg.full_damage -= tmpsplit;
					tmpsplit = float2int32(itr->m_pctDamageSplit * dmg.full_damage);
					if( tmpsplit > (uint32)dmg.full_damage )
						tmpsplit = dmg.full_damage;
					splitdamage += tmpsplit;
					dmg.full_damage -= tmpsplit;
				}
			}
			// TODO: pct damage
			if( splitdamage )
			{
				pVictim->DealDamage( splittarget , splitdamage , 0 , 0 , 0 , false );
				// Send damage log
				pVictim->SendSpellNonMeleeDamageLog( pVictim , splittarget , 27148 , splitdamage , SCHOOL_HOLY , 0 , 0 , true , 0 , 0 , true );
			}
					realdamage = dmg.full_damage;
		}
//--------------------------special states processing---------------------------------------
	if(pVictim->GetTypeId() == TYPEID_UNIT)
	{
		if(pVictim->GetAIInterface() && (pVictim->GetAIInterface()->getAIState()== STATE_EVADE ||
										(pVictim->GetAIInterface()->GetIsSoulLinked() && pVictim->GetAIInterface()->getSoullinkedWith() != this)))
		{
			vstate = EVADE;
			realdamage = 0;
			dmg.full_damage = 0;
			dmg.resisted_damage = 0;
		}
	}
	if(pVictim->GetTypeId() == TYPEID_PLAYER && static_cast< Player* >(pVictim)->GodModeCheat == true)
	{
		dmg.resisted_damage = dmg.full_damage; //godmode
	}
//--------------------------dirty fixes-----------------------------------------------------
	//vstate=1-wound,2-dodge,3-parry,4-interrupt,5-block,6-evade,7-immune,8-deflect	
	// the above code was remade it for reasons : damage shield needs moslty same flags as handleproc + dual wield should proc too ?
	if( !disable_proc && weapon_damage_type != OFFHAND )
    {
		this->HandleProc(aproc,pVictim, ability,realdamage,abs,(weapon_damage_type == OFFHAND) ? false : true ); //maybe using dmg.resisted_damage is better sometimes but then if using godmode dmg is resisted instead of absorbed....bad
		m_procCounter = 0;

		pVictim->HandleProc(vproc,this, ability,realdamage,abs,(weapon_damage_type == OFFHAND) ? false : true);
		pVictim->m_procCounter = 0;

		if(realdamage > 0)
		{
			pVictim->HandleProcDmgShield(vproc,this);
			HandleProcDmgShield(aproc,pVictim);
		}
	}
//--------------------------spells triggering-----------------------------------------------
	if(realdamage > 0 && ability == 0)
	{
		//handle daze
		if (!IsFromPlayer())
		{
			float our_facing = calcRadAngle(GetPositionX(), GetPositionY(), pVictim->GetPositionX(), pVictim->GetPositionY());
			float his_facing = pVictim->GetOrientation();

			if (Rand(get_chance_to_daze(pVictim) && fabs(our_facing-his_facing)<CREATURE_DAZE_TRIGGER_ANGLE))
			{
				Spell* s=new Spell(this, dbcSpell.LookupEntry(CREATURE_SPELL_TO_DAZE), true, NULL);
				SpellCastTargets t(pVictim);
				s->prepare(&t);

				//remove mount auras if possible
				if (pVictim->IsPlayer())
				{
					Player* ppVictim = static_cast<Player*>(pVictim);
					if (ppVictim->m_MountSpellId != 0)
						ppVictim->RemoveAura(ppVictim->m_MountSpellId);
					if (ppVictim->flying_aura != 0 && ppVictim->GetShapeShift() == 0)
						ppVictim->RemoveAura(ppVictim->flying_aura);
				}
			}
		}

		if( IsPlayer() && static_cast< Player* >( this )->m_onStrikeSpells.size() )
		{
			SpellCastTargets targets;
			targets.m_target = ((Player *)pVictim->GetGUID());
			targets.m_targetMask = TARGET_FLAG_UNIT; // 0x2;
			Spell* cspell;

			// Loop on hit spells, and strike with those.
			for( map< SpellEntry*, pair< uint32, uint32 > >::iterator itr = static_cast<Player *>(this)->m_onStrikeSpells.begin();
				itr != static_cast< Player* >( this )->m_onStrikeSpells.end(); ++itr )
			{
				if( itr->second.first )
				{
					// We have a *periodic* delayed spell.
					uint32 t = getMSTime();
					if( t > itr->second.second )  // Time expired
					{
						// Set new time
						itr->second.second = t + itr->second.first;
					}

					// Cast.
					cspell = new Spell(this, itr->first, true, NULL);
					cspell->prepare(&targets);
				}
				else
				{
					cspell = new Spell(this, itr->first, true, NULL);
					cspell->prepare(&targets);
				}			
			}
		}

		if( IsPlayer() && static_cast< Player* >( this )->m_onStrikeSpellDmg.size() )
		{
			map< uint32, OnHitSpell >::iterator it2 = static_cast< Player* >( this )->m_onStrikeSpellDmg.begin();
			map< uint32, OnHitSpell >::iterator itr;
			uint32 min_dmg, max_dmg, range, dmg;
			for(; it2 != static_cast< Player* >( this )->m_onStrikeSpellDmg.end(); )
			{
				itr = it2;
				++it2;

				min_dmg = itr->second.mindmg;
				max_dmg = itr->second.maxdmg;
				range = min_dmg - max_dmg;
				dmg = min_dmg;
				if(range) range += RandomUInt(range);

				SpellNonMeleeDamageLog(pVictim, itr->second.spellid, dmg, true);
			}
		}

		//ugly hack for shadowfiend restoring mana
		if( GetUInt64Value(UNIT_FIELD_SUMMONEDBY) != 0 && GetUInt32Value(OBJECT_FIELD_ENTRY) == 19668 )
		{
			Player* owner = GetMapMgr()->GetPlayer((uint32)GetUInt64Value(UNIT_FIELD_SUMMONEDBY));
			if ( owner != NULL )	// restore 4% of max mana on each hit
				Energize(owner, 34433, owner->GetUInt32Value(UNIT_FIELD_MAXPOWER1) / 25, POWER_TYPE_MANA );
		}

		// refresh judgements
		// TODO: find the opcode to refresh the aura or just remove it and re add it
		// rather than fuck with duration
		// DONE: Remove + readded it :P
		for( uint32 x = MAX_POSITIVE_AURAS; x <= MAX_AURAS; x++ )
		{
			if( pVictim->m_auras[x] != NULL && pVictim->m_auras[x]->GetUnitCaster() != NULL && pVictim->m_auras[x]->GetUnitCaster()->GetGUID() == GetGUID() && Spell::IsJudgement(pVictim->m_auras[x]->GetSpellProto()) )
			{
				pVictim->m_auras[x]->HandleStackAdd();
			}
		}
	}
	
//==========================================================================================
//==============================Data Sending================================================
//==========================================================================================
	WorldPacket data(SMSG_ATTACKERSTATEUPDATE, 70);
	//0x4--dualwield,0x10 miss,0x20 absorbed,0x80 crit,0x4000 -glancing,0x8000-crushing
	//only for melee!

	if( !ability )
	{
		if( dmg.full_damage > 0 )
		{
			if( dmg.full_damage == (int32)abs )
				hit_status |= HITSTATUS_ABSORBED;
			else if (dmg.full_damage <= (int32)dmg.resisted_damage)
			{
				hit_status |= HITSTATUS_RESIST;
				dmg.resisted_damage = dmg.full_damage;
			}
		}

		if( dmg.full_damage < 0 )
			dmg.full_damage = 0;

		if( realdamage < 0 )
			realdamage = 0;

		hit_status |= 0x100;

		data << (uint32)hit_status;   
		data << GetNewGUID();
		data << pVictim->GetNewGUID();
		
		data << (uint32)realdamage;		 // Realdamage;
		data << uint32(0);					//new 3.0.2 unknown

		uint8 counter = 1; //we always need to send once for some reason :)
		if (hit_status & HITSTATUS_ABSORBED && counter < 1) //need 1 for this
			counter = 1;
		if (hit_status & HITSTATUS_RESIST && counter < 2) //need 2 for this
			counter = 2;

		data << counter;				   // Damage type counter / swing type

		for (uint8 i=0; i<counter; ++i)
		{
			data << (uint32)g_spellSchoolConversionTable[dmg.school_type];				  // Damage school
			data << (float)dmg.full_damage;	 // Damage float
			data << (uint32)dmg.full_damage;	// Damage amount
		}

		if (hit_status & (HITSTATUS_ABSORBED | HITSTATUS_RESIST))
		{
			for (uint8 i=0; i<counter; ++i)
			{
				if (i == 0)
					data << (abs? abs: dmg.resisted_damage);
				else if (i == 1)
					data << (abs? dmg.resisted_damage : uint32(0));
				else
					data << uint32(0);
			}
		}

		//need to check for 0x180, client checks for them, 0x100 is unknown atm
		if (hit_status & (HITSTATUS_CRICTICAL | 0x100))
		{
			for (uint8 i=0; i<counter; ++i)
			{
				//wtf is this? client expects this data
				data << uint32(0);
			}
		}


		data << (uint8)vstate;			 // new victim state
		//data << (uint32)0x03e8;					// can be 0,1000 or -1
		data << (uint32)0;				  // unknown
		data << (uint32)blocked_damage;	 // Damage amount blocked
		//data << (uint32) 0;

		SendMessageToSet(&data, this->IsPlayer());
	}
	else
	{
		if( realdamage > 0 )//FIXME: add log for miss,block etc for ability and ranged
		{
			// here we send "dmg.resisted_damage" for "AbsorbedDamage", "0" for "ResistedDamage", and "false" for "PhysicalDamage" even though "School" is "SCHOOL_NORMAL"   o_O
			SendSpellNonMeleeDamageLog( this, pVictim, ability->Id, realdamage, dmg.school_type, dmg.resisted_damage, 0, false, blocked_damage, ( ( hit_status & HITSTATUS_CRICTICAL ) != 0 ), true );
		}
		//FIXME: add log for miss,block etc for ability and ranged
		//example how it works
		//SendSpellLog(this,pVictim,ability->Id,SPELL_LOG_MISS);
	}

	if(ability && realdamage == 0)
	{	
		SendSpellLog(this,pVictim,ability->Id,SPELL_LOG_RESIST);
	}
//==========================================================================================
//==============================Damage Dealing==============================================
//==========================================================================================

	if(this->IsPlayer() && ability)
		static_cast< Player* >( this )->m_casted_amount[dmg.school_type]=(uint32)(realdamage+abs);
	if(realdamage)
	{
		DealDamage(pVictim, realdamage, 0, targetEvent, 0);

		if(pVictim->GetCurrentSpell())
			pVictim->GetCurrentSpell()->AddTime(0);
	}
	else
	{		
		// have to set attack target here otherwise it wont be set
		// because dealdamage is not called.
		//setAttackTarget(pVictim);
		pVictim->CombatStatus.OnDamageDealt( this );
	}
//==========================================================================================
//==============================Post Damage Dealing Processing==============================
//==========================================================================================
//--------------------------durability processing-------------------------------------------
	if(pVictim->IsPlayer())
	{
		static_cast< Player* >( pVictim )->GetItemInterface()->ReduceItemDurability();
		if( !this->IsPlayer() )
		{
			Player *pr = static_cast< Player* >( pVictim );
			if( Rand( pr->GetSkillUpChance( SKILL_DEFENSE ) * sWorld.getRate( RATE_SKILLCHANCE ) ) )
			{
				pr->_AdvanceSkillLine( SKILL_DEFENSE, float2int32( 1.0f * sWorld.getRate(RATE_SKILLRATE)));
				pr->UpdateChances();
			}
		}
		else
		{
			 static_cast< Player* >( this )->GetItemInterface()->ReduceItemDurability();
		}
	}
	else
	{
		if(this->IsPlayer())//not pvp
		{
			static_cast< Player* >( this )->GetItemInterface()->ReduceItemDurability();
			Player* pr = static_cast< Player* >( this );
			if( Rand( pr->GetSkillUpChance( SubClassSkill) * sWorld.getRate( RATE_SKILLCHANCE ) ) )
			{
				pr->_AdvanceSkillLine( SubClassSkill, float2int32( 1.0f * sWorld.getRate(RATE_SKILLRATE)));
				//pr->UpdateChances();
			}
		}
	}
	//--------------------------rage processing-------------------------------------------------
	//http://www.wowwiki.com/Formulas:Rage_generation

	if( dmg.full_damage && IsPlayer() && GetPowerType() == POWER_TYPE_RAGE && !ability)
	{
		float val;
		float level = (float)getLevel();

		// Conversion Value
		float c = 0.0091107836f * level * level + 3.225598133f * level + 4.2652911f;

		// Hit Factor
		float f = ( weapon_damage_type == OFFHAND ) ? 1.75f : 3.5f;

		if( hit_status & HITSTATUS_CRICTICAL )
			f *= 2.0f;

		float s = 1.0f;

		// Weapon speed (normal)
		Item* weapon = ( static_cast< Player* >( this )->GetItemInterface())->GetInventoryItem( INVENTORY_SLOT_NOT_SET, ( weapon_damage_type == OFFHAND ? EQUIPMENT_SLOT_OFFHAND : EQUIPMENT_SLOT_MAINHAND ) );
		if( weapon == NULL )
		{
			if( weapon_damage_type == OFFHAND )
				s = GetUInt32Value( UNIT_FIELD_RANGEDATTACKTIME ) / 1000.0f;
			else
				s = GetUInt32Value( UNIT_FIELD_BASEATTACKTIME ) / 1000.0f;
		}
		else
		{
			uint32 entry = weapon->GetEntry();
			ItemPrototype* pProto = ItemPrototypeStorage.LookupEntry( entry );
			if( pProto != NULL )
			{
				s = pProto->Delay / 1000.0f;
			}
		}

		val = ( 7.5f * dmg.full_damage / c + f * s ) / 2.0f;;
		val *= ( 1 + ( static_cast< Player* >( this )->rageFromDamageDealt / 100.0f ) );
		val *= 10;

		//float r = ( 7.5f * dmg.full_damage / c + f * s ) / 2.0f;
		//float p = ( 1 + ( static_cast< Player* >( this )->rageFromDamageDealt / 100.0f ) );
		//sLog.outDebug( "Rd(%i) d(%i) c(%f) f(%f) s(%f) p(%f) r(%f) rage = %f", realdamage, dmg.full_damage, c, f, s, p, r, val );

		if (val > 1000)
			val = 1000 - GetUInt32Value(UNIT_FIELD_POWER2);

		static_cast<Player*>(this)->SetRage(GetUInt32Value(UNIT_FIELD_POWER2) + val);
	}

	// I am receiving damage!
	if( dmg.full_damage && pVictim->IsPlayer() && pVictim->GetPowerType() == POWER_TYPE_RAGE && pVictim->CombatStatus.IsInCombat() )
	{
		float val;
		float level = (float)getLevel();

		// Conversion Value
		float c = 0.0091107836f * level * level + 3.225598133f * level + 4.2652911f;

		val = 2.5f * dmg.full_damage / c;
		val *= 10;

		//sLog.outDebug( "Rd(%i) d(%i) c(%f) rage = %f", realdamage, dmg.full_damage, c, val );

		uint32 rage = pVictim->GetUInt32Value(UNIT_FIELD_POWER2);
		uint32 maxrage = pVictim->GetUInt32Value(UNIT_FIELD_MAXPOWER2);
		rage += val;
		if (rage > maxrage)
			rage = maxrage;


		static_cast<Player*>(pVictim)->SetRage(rage);
	}
		
	RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_START_ATTACK);
//--------------------------extra strikes processing----------------------------------------
	if(!m_extraAttackCounter)
	{
		int32 extra_attacks = m_extraattacks;
		m_extraAttackCounter = true;
		m_extraattacks = 0;

		while(extra_attacks > 0)
		{
			extra_attacks--;
			Strike( pVictim, weapon_damage_type, NULL, 0, 0, 0, true, false );
		}

		m_extraAttackCounter = false;
	}

	if(m_extrastriketargets)
	{
		int32 m_extra = m_extrastriketargets;
		int32 m_temp = m_extrastriketargets;
		m_extrastriketargets = 0;

		for(set<Object*>::iterator itr = m_objectsInRange.begin(); itr != m_objectsInRange.end() && m_extra; ++itr)
		{
			if(m_extra == 0)
				break;
			if (!(*itr) || (*itr) == pVictim || !(*itr)->IsUnit())
				continue;


			if(CalcDistance(*itr) < 10.0f && isAttackable(this, (*itr)) && (*itr)->isInFront(this) && !((Unit*)(*itr))->IsPacified())
			{
				Strike( static_cast< Unit* >( *itr ), weapon_damage_type, ability, add_damage, pct_dmg_mod, exclusive_damage, false ,false );
				--m_extra;
			}
		}
		m_extrastriketargets = m_temp;
	}
}	

void Unit::smsg_AttackStop(Unit* pVictim)
{
	if(!pVictim)
		return;

	WorldPacket data(SMSG_ATTACKSTOP, 24);
	if(m_objectTypeId==TYPEID_PLAYER)
	{
		data << pVictim->GetNewGUID();
		data << uint8(0);
		data << uint32(0);
		static_cast< Player* >( this )->GetSession()->SendPacket( &data );
		data.clear();
	}

	data << GetNewGUID();
	data << pVictim->GetNewGUID();
	data << uint32(0);
	SendMessageToSet(&data, true );
	// stop swinging, reset pvp timeout

	if( pVictim->IsPlayer() )
	{
		pVictim->CombatStatusHandler_ResetPvPTimeout();
		CombatStatusHandler_ResetPvPTimeout();
		pVictim->CombatStatus.RemoveAttacker(this, GetGUID());
		CombatStatus.RemoveAttackTarget(pVictim);
	}
	else
	{
		/*if(!IsPlayer() || getClass() == ROGUE)
		{
			m_cTimer = getMSTime() + 5000;
			sEventMgr.RemoveEvents(this, EVENT_COMBAT_TIMER); 
			sEventMgr.AddEvent(this, &Unit::EventUpdateFlag, EVENT_COMBAT_TIMER, 5000, 1, 0);
			sEventMgr.AddEvent(pVictim, &Unit::EventUpdateFlag, EVENT_COMBAT_TIMER, 5000, 1, 0);
		}
		else
		{*/
			pVictim->CombatStatus.RemoveAttacker(this, GetGUID());
			CombatStatus.RemoveAttackTarget(pVictim);
		//}
	}
}

void Unit::smsg_AttackStop(uint64 victimGuid)
{
	WorldPacket data(20);
	data.Initialize( SMSG_ATTACKSTOP );
	data << GetNewGUID();
	FastGUIDPack(data, victimGuid);
	data << uint32( 0 );
	SendMessageToSet(&data, IsPlayer());
}

void Unit::smsg_AttackStart(Unit* pVictim)
{
	if(GetTypeId() != TYPEID_PLAYER) 
		return;

	Player* pThis = static_cast<Player *>(this);

	// Prevent user from ignoring attack speed and stopping and start combat really really fast
	/*if(!isAttackReady())
		setAttackTimer(uint32(0));
	else if(!canReachWithAttack(pVictim))
	{
		setAttackTimer(uint32(500));
		//pThis->GetSession()->OutPacket(SMSG_ATTACKSWING_NOTINRANGE);
	}
	else if(!isInFront(pVictim))
	{
		setAttackTimer(uint32(500));
		//pThis->GetSession()->OutPacket(SMSG_ATTACKSWING_NOTINRANGE);
	}*/

	// Send out ATTACKSTART
	WorldPacket data(SMSG_ATTACKSTART, 16);
	data << GetGUID();
	data << pVictim->GetGUID();
	SendMessageToSet(&data, true);
	sLog.outDebug("WORLD: Sent SMSG_ATTACKSTART");

	// FLAGS changed so other players see attack animation
	//	addUnitFlag(UNIT_FLAG_COMBAT);
	//	setUpdateMaskBit(UNIT_FIELD_FLAGS );
	if(pThis != NULL && pThis->cannibalize)
	{
		sEventMgr.RemoveEvents(pThis, EVENT_CANNIBALIZE);
		pThis->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
		pThis->cannibalize = false;
	}
}

void Unit::AddAura(Aura *aur, SpellScript* script)
{
	//Log.Notice("[Unit::AddAura]","Add Aura progress...");

	if(aur->m_wasremoved) return; // Sécu en attendant le recodage des spells de GOs (Brz) 

	if(!aur->GetSpellProto()) return; // Ne devrait jamais arriver, Sécu en attendant le recodage des spells de GOs (Brz) 

	if (IsPlayer()) // Peut-on voler partout ? à discuter ? (Branruz)
	{
		for (uint32 i=0; i<3; ++i)
		{
			if(   aur->GetSpellProto()->EffectApplyAuraName[i] == SPELL_AURA_FLYING_SPEED 
			   || aur->GetSpellProto()->EffectApplyAuraName[i] == SPELL_AURA_FLYING_MOUNT)
			{
				AreaTable * at = dbcArea.LookupEntry(static_cast<Player*>(this)->GetAreaID());

				if (at && !(at->AreaFlags & AREA_ALLOW_FLYING))
					return; // Ne devrait-on pas detruire le spell en cours ? (et commencant par l'aura ? )
			}
		}
	}

	if( aur->GetSpellProto()->School && SchoolImmunityList[aur->GetSpellProto()->School] )
		return;

	if( !aur->IsPassive() )
	{
		//uint32 aurName = aur->GetSpellProto()->Name;
		//uint32 aurRank = aur->GetSpellProto()->Rank;
		uint32 maxStack = aur->GetSpellProto()->cumulativeAura;
		if( aur->GetSpellProto()->procCharges > 0 )
			maxStack=aur->GetSpellProto()->procCharges;

		if( IsPlayer() && static_cast< Player* >( this )->stack_cheat )
			maxStack = 999;

		SpellEntry * info = aur->GetSpellProto();
		//uint32 flag3 = aur->GetSpellProto()->Flags3;

		AuraCheckResponse acr;
		WorldPacket data( 21 );
		bool deleteAur = false;

		//check if we already have this aura by this caster -> update duration
		// Nasty check for Blood Fury debuff (spell system based on namehashes is bs anyways)
		if( !info->always_apply )
		{
			uint32 f = 0;
			for( uint32 x = 0; x < MAX_AURAS; x++ )
			{
				if( m_auras[x] )
				{
					if(	m_auras[x]->GetSpellProto()->Id != aur->GetSpellId() && 
						( aur->pSpellId != m_auras[x]->GetSpellProto()->Id )) //if this is a proc spell then it should not remove it's mother : test with combustion later
					{
						if( info->buffType > 0 && m_auras[x]->GetSpellProto()->buffType > 0 && (info->buffType & m_auras[x]->GetSpellProto()->buffType) )
						{
							if( m_auras[x]->GetSpellProto()->buffType & SPELL_TYPE_BLESSING )
							{
								// stupid blessings
								// if you have better idea correct
								bool ispair = false;
								switch( info->NameHash )
								{
								case SPELL_HASH_BLESSING_OF_MIGHT:
								case SPELL_HASH_GREATER_BLESSING_OF_MIGHT:
									{
										if( m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_BLESSING_OF_MIGHT ||
											m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_GREATER_BLESSING_OF_MIGHT )
											ispair = true;
									}break;
								case SPELL_HASH_BLESSING_OF_WISDOM:
								case SPELL_HASH_GREATER_BLESSING_OF_WISDOM:
									{
										if( m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_BLESSING_OF_WISDOM ||
											m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_GREATER_BLESSING_OF_WISDOM )
											ispair = true;
									}break;
								case SPELL_HASH_BLESSING_OF_KINGS:
								case SPELL_HASH_GREATER_BLESSING_OF_KINGS:
									{
										if( m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_BLESSING_OF_KINGS ||
											m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_GREATER_BLESSING_OF_KINGS )
											ispair = true;
									}break;
								case SPELL_HASH_BLESSING_OF_SANCTUARY:
								case SPELL_HASH_GREATER_BLESSING_OF_SANCTUARY:
									{
										if( m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_BLESSING_OF_SANCTUARY ||
											m_auras[x]->GetSpellProto()->NameHash == SPELL_HASH_GREATER_BLESSING_OF_SANCTUARY )
											ispair = true;
									}break;

								}
								if( m_auras[x]->GetUnitCaster() == aur->GetUnitCaster() || ispair )
								{
									m_auras[x]->Remove();
									continue;
								}							
							}
							else if( m_auras[x]->GetSpellProto()->buffType & SPELL_TYPE_AURA )
							{
								if( m_auras[x]->GetUnitCaster() == aur->GetUnitCaster() || m_auras[x]->GetSpellProto()->NameHash == info->NameHash )
								{
									m_auras[x]->Remove();
									continue;
								}
							}
							else
							{
								m_auras[x]->Remove();
								continue;
							}
						}
						else if( info->poison_type > 0 && m_auras[x]->GetSpellProto()->poison_type == info->poison_type )
						{
							if( m_auras[x]->GetSpellProto()->RankNumber < info->RankNumber || maxStack == 0)
							{
								m_auras[x]->Remove();
								continue;
							}
							else if( m_auras[x]->GetSpellProto()->RankNumber > info->RankNumber )
							{
								m_auras[x]->Remove();
								break;
							}
						}
						// Check for auras by specific type.
						// Check for auras with the same name and a different rank.						
						else if(info->buffType > 0 && m_auras[x]->GetSpellProto()->buffType & info->buffType && maxStack == 0)
							deleteAur = HasAurasOfBuffType(info->buffType, aur->m_casterGuid,0);
						else
						{
							acr = AuraCheck(info->NameHash, info->RankNumber, m_auras[x],aur->GetCaster());
							if(acr.Error == AURA_CHECK_RESULT_HIGHER_BUFF_PRESENT)
								deleteAur = true;
							else if(acr.Error == AURA_CHECK_RESULT_LOWER_BUFF_PRESENT)
							{
								// remove the lower aura
								m_auras[x]->Remove();

								// no more checks on bad ptr
								continue;
							}
						}					   
					}

					else if( m_auras[x]->GetSpellId() == aur->GetSpellId() ) // not the best formula to test this I know, but it works until we find a solution
					{
						if( !aur->IsPositive() && m_auras[x]->m_casterGuid != aur->m_casterGuid )
							continue;
						f++;
						//if(maxStack > 1)
						{
							//update duration,the same aura (update the whole stack whenever we cast a new one)
							m_auras[x]->SetDuration(aur->GetDuration(COOLDOWN_DURATION),COOLDOWN_DURATION);
							m_auras[x]->SetTimeLeft(aur->GetDuration(COOLDOWN_DURATION),COOLDOWN_DURATION);
							if(maxStack <= 1)
							{
								if(this->IsPlayer())
								{
									//data.Initialize(SMSG_AURA_UPDATE_DURATION);
									//data << (uint8)m_auras[x]->m_visualSlot <<(uint32) aur->GetTimeLeft();
									//((Player*)this)->GetSession()->SendPacket(&data);
								}
							}
						}
						if(maxStack <= f)
						{
							deleteAur = true;
							break;
						}
					}
				}
			}
		}

		if(deleteAur)
		{
			sEventMgr.AddEvent(aur, &Aura::Remove, EVENT_AURA_REMOVE, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			return;
		}
	}
			
	////////////////////////////////////////////////////////
	if((aur->GetSpellProto()->SpellGroupType[0] || aur->GetSpellProto()->SpellGroupType[1] || aur->GetSpellProto()->SpellGroupType[2]) && (m_objectTypeId == TYPEID_PLAYER) )
	{
		int32 speedmod=0;
		SM_FIValue(SM_FSpeedMod, &speedmod, aur->m_spellProto);
		if(speedmod)
		{
			m_speedModifier += speedmod;
			UpdateSpeed();
		}
	}
	////////////////////////////////////////////////////////

	if( aur->m_auraSlot != 0xffffffff )
		m_auras[aur->m_auraSlot] = NULL;
	
	aur->m_auraSlot=255;
	aur->ApplyModifiers(true);

	Unit* target = aur->GetTarget();
	if (target != NULL)
	{
		//send the aura log
		WorldPacket data(SMSG_AURACASTLOG, 28);

		data << aur->m_casterGuid;
		data << aur->GetTarget()->GetGUID();
		data << aur->m_spellProto->Id;
		data << uint64(0);

		target->SendMessageToSet(&data, true);
	}

	
	
	if(!aur->IsPassive())
	{	
		aur->AddAuraVisual();
		if(aur->m_auraSlot==255)
		{
			//add to invisible slot
			for(uint32 x=MAX_AURAS;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
			{
				if(!m_auras[x])
				{
					m_auras[x]=aur;
					aur->m_auraSlot=x;
					break;
				}
			}
			if(aur->m_auraSlot == 255)
			{
				Log.Error("[Unit::AddAura]","Aura error in active aura. ");
				// for next loop.. lets kill the fucker
				aur->Remove();
				return;
			} 

			// add visual
			AddAuraVisual(aur->GetSpellId(), 1, aur->IsPositive());
		}
		else
		{
			m_auras[aur->m_auraSlot]=aur;
		}
	}
	else
	{
        if((aur->m_spellProto->attributesEx & ATTRIBUTESEX_VISUAL_AURA)) // 1024 / 0x0400
        {
            aur->AddAuraVisual();
        }

		for(uint32 x=MAX_AURAS;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
		{
			if(!m_auras[x])
			{
				m_auras[x]=aur;
				aur->m_auraSlot=x;
				break;
			}
		}
		if(aur->m_auraSlot==255)
		{
			Log.Error("[Unit::AddAura]","Aura error in passive aura. removing. SpellId: %u", aur->GetSpellProto()->Id);
			// for next loop.. lets kill the fucker
			aur->Remove();
			return;
		}
	}
 
	AuraProcData data;
	data.push_back((void*)aur);

	HandleProcFnc(SPELLFNC_PROC_ON_AURA_ADD, &data);

	aur->SetTimeLeft(aur->GetDuration(INVOC_DURATION),INVOC_DURATION);

	aur->RelocateEvents();

	// Reaction from enemy AI
	Unit * pCaster = aur->GetUnitCaster();
	if(!aur->IsPositive() && aur->IsCombatStateAffecting())	  // Creature
	{		
		if(pCaster && isAlive())
		{
			pCaster->CombatStatus.OnDamageDealt(this);

			if(m_objectTypeId == TYPEID_UNIT)
				m_aiInterface->AttackReaction(pCaster, 1, aur->GetSpellId());
		}
	}

	if (aur->GetSpellProto()->auraInterruptFlags & AURA_INTERRUPT_ON_INVINCIBLE)
	{
		if( pCaster != NULL )
		{
			pCaster->RemoveStealth();
			pCaster->RemoveInvisibility();
			pCaster->RemoveAllAuraByNameHash(SPELL_HASH_ICE_BLOCK);
			pCaster->RemoveAllAuraByNameHash(SPELL_HASH_DIVINE_SHIELD);
			pCaster->RemoveAllAuraByNameHash(SPELL_HASH_BLESSING_OF_PROTECTION);
		}
	}
}

bool Unit::RemoveAura(Aura *aur)
{
	aur->m_wasremoved = true;
	aur->Remove();
	return true;
}

bool Unit::RemovePositiveAura(uint32 spellId)
{
	for(uint32 x=0;x<MAX_POSITIVE_AURAS;x++)
	{
			if(m_auras[x] && m_auras[x]->GetSpellId()==spellId)
			{
				m_auras[x]->Remove();
				return true;
			}
	}
	return false;
}

bool Unit::RemoveAura(uint32 spellId)
{//this can be speed up, if we know passive \pos neg
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellId()==spellId)
			{
				m_auras[x]->m_wasremoved = true;
				m_auras[x]->Remove();
				return true;
			}
		}
	}
	return false;
}

bool Unit::RemoveAuras(uint32 amount, ...)
{
	std::set<uint32> auraids;
	va_list v;
	va_start(v, amount);

	for (uint32 i=0; i<amount; ++i)
		auraids.insert(va_arg(v, uint32));

	for(uint32 x=0; x<MAX_AURAS+MAX_PASSIVE_AURAS; ++x)
	{
		if(m_auras[x] != NULL)
		{
			if(auraids.find(m_auras[x]->m_spellProto->Id) != auraids.end())
			   m_auras[x]->Remove();
		}
	}
	return true;
}

bool Unit::RemoveAuras(uint32 * SpellIds)
{
	if(!SpellIds || *SpellIds == 0)
		return false;

	uint32 x=0,y;
	bool res = false;
	for(;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			for(y=0;SpellIds[y] != 0;++y)
			{
				if(m_auras[x]->GetSpellId()==SpellIds[y])
				{
					m_auras[x]->m_wasremoved = true;
					m_auras[x]->Remove();
					res = true;
				}
			}
		}
	}
	return res;
}

bool Unit::RemoveAura(uint32 spellId, uint64 guid)
{   
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellId()==spellId && m_auras[x]->m_casterGuid == guid)
			{
				m_auras[x]->m_wasremoved = true;
				m_auras[x]->Remove();
				return true;
			}
		}
	}
	return false;
}

bool Unit::RemoveAuraByNameHash(uint32 namehash)
{
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->NameHash==namehash)
			{
				m_auras[x]->m_wasremoved = true;
				m_auras[x]->Remove();
				return true;
			}
		}
	}
	return false;
}

bool Unit::RemoveAuraPosByNameHash(uint32 namehash)
{
	for(uint32 x=0;x<MAX_POSITIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->NameHash==namehash)
			{
				m_auras[x]->m_wasremoved = true;
				m_auras[x]->Remove();
				return true;
			}
		}
	}
	return false;
}

bool Unit::RemoveAuraNegByNameHash(uint32 namehash)
{
	for(uint32 x=MAX_POSITIVE_AURAS;x<MAX_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->NameHash==namehash)
			{
				m_auras[x]->m_wasremoved = true;
				m_auras[x]->Remove();
				return true;
			}
		}
	}
	return false;
}

bool Unit::RemoveAllAuras(uint32 spellId, uint64 guid, Aura* ignore)
{   
	bool res = false;
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x] && (ignore == NULL || ignore != m_auras[x]))
		{
			if(m_auras[x]->GetSpellId()==spellId)
			{
				if (!guid || m_auras[x]->m_casterGuid == guid)
				{
					m_auras[x]->m_wasremoved = true;
					m_auras[x]->Remove();
					res = true;
				}
			}
		}
	}
	return res;
}

bool Unit::RemoveAllAuraByNameHash(uint32 namehash)
{
	bool res = false;
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->m_spellProto->NameHash==namehash)
			{
				m_auras[x]->m_wasremoved = true;
				m_auras[x]->Remove();
				res=true;
			}
		}
	}
	return res;
}

bool Unit::RemoveAllPosAuraByNameHash(uint32 namehash)
{
	bool res = false;
	for(uint32 x=0;x<MAX_POSITIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->NameHash==namehash)
			{
				m_auras[x]->m_wasremoved = true;
				m_auras[x]->Remove();
				res=true;
			}
		}
	}
	return res;
}

bool Unit::RemoveAllNegAuraByNameHash(uint32 namehash)
{
	bool res = false;
	for(uint32 x=MAX_POSITIVE_AURAS;x<MAX_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->NameHash==namehash)
			{
				m_auras[x]->m_wasremoved = true;
				m_auras[x]->Remove();
				res=true;
			}
		}
	}
	return res;
}

void Unit::RemoveNegativeAuras()
{
	for(uint32 x=MAX_POSITIVE_AURAS;x<MAX_AURAS;x++)
	{
		if(m_auras[x])
		{
            if(m_auras[x]->GetSpellProto()->attributesExC & ATTRIBUTESEXC_CAN_PERSIST_AND_CASTED_WHILE_DEAD)
                continue;
            else
            {
				m_auras[x]->m_wasremoved = true;
			    m_auras[x]->Remove();
            }
		}
	}
}

void Unit::RemoveAllAuras()
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			m_auras[x]->m_wasremoved = true;
			m_auras[x]->Remove();
		}
	}

	//we must cleanup tmpauras
	for (std::map<uint32, Aura *>::iterator itr=tmpAura.begin(); itr!=tmpAura.end(); ++itr)
	{
		itr->second->m_deletedfromtmp = true;
		itr->second->Remove();
	}
	tmpAura.clear();
}

//ex:to remove morph spells
void Unit::RemoveAllAuraType(uint32 auratype)
{
    for(uint32 x=0;x<MAX_AURAS;x++)
    {
		if(m_auras[x])
		{
			SpellEntry *proto=m_auras[x]->GetSpellProto();
			if(proto->EffectApplyAuraName[0]==auratype || proto->EffectApplyAuraName[1]==auratype || proto->EffectApplyAuraName[2]==auratype)
				RemoveAura(m_auras[x]->GetSpellId());//remove all morph auras containig to this spell (like wolf motph also gives speed)
		}
    }
}

bool Unit::SetAurDuration(uint32 spellId,Unit* caster,uint32 duration)
{
	return true;
}

bool Unit::SetAurDuration(uint32 spellId,uint32 duration)
{
	return true;
}

Aura* Unit::FindAuraByUniqueGroup(uint64 group, uint64 caster_guid, uint32 name_hash)
{
	for(uint32 x=0; x<MAX_AURAS+MAX_PASSIVE_AURAS; x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->UniqueGroup==group && (caster_guid == 0 || caster_guid == m_auras[x]->m_casterGuid) && (name_hash == 0 || name_hash == m_auras[x]->GetSpellProto()->NameHash))
			{
				return m_auras[x];
			}
		}
	}
	return NULL;
}

Aura* Unit::FindAuraPosByNameHash(uint32 namehash)
{
	for(uint32 x=0;x<MAX_POSITIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellProto()->NameHash==namehash)
			{
				return m_auras[x];
			}
		}
	}
	return NULL;
}

Aura* Unit::FindAura(uint32 spellId)
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellId()==spellId)
			{
				return m_auras[x];
			}
		}
	}
	return NULL;
}

Aura* Unit::FindAura(uint32 spellId, uint64 guid)
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	{
		if(m_auras[x])
		{
			if(m_auras[x]->GetSpellId() == spellId && m_auras[x]->m_casterGuid == guid)
			{
				return m_auras[x];
			}
		}
	}
	return NULL;
}

void Unit::_UpdateSpells( uint32 time )
{
	/* to avoid deleting the current spell */
	if(m_currentSpell != NULL)
	{
		m_spellsbusy=true;
		m_currentSpell->update(time);
		m_spellsbusy=false;
	}
}

void Unit::castSpell( Spell * pSpell )
{
	// check if we have a spell already casting etc
	if(m_currentSpell && pSpell != m_currentSpell)
	{
		if(m_spellsbusy)
		{
			//Log.Warning("castSpell","Spell Busy: %d",pSpell->GetSpellProto()->Id); 
			// shouldn't really happen. but due to spell sytem bugs there are some cases where this can happen. Latency: 100ms
			sEventMgr.AddEvent(this,&Unit::CancelSpell,m_currentSpell,EVENT_UNK,100,1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
		else
		{
			//Log.Warning("castSpell","Spell Cancel: %d",pSpell->GetSpellProto()->Id); 
			m_currentSpell->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
		}
	}

	//Log.Warning("castSpell","m_currentSpell: %d",pSpell->GetSpellProto()->Id); 
	m_currentSpell = pSpell;
	pLastSpell = pSpell->m_spellInfo;
}

int32 Unit::GetSpellDmgBonus(Unit *pVictim, Spell* spell, int32 base_dmg, bool isdot, float divider)
{
	return GetSpellDmgBonus(pVictim, spell->m_spellInfo, base_dmg, isdot, spell->GetBestSchoolForSpell(pVictim), divider);
}


int32 Unit::GetSpellDmgBonus(Unit *pVictim, SpellEntry* spellInfo, int32 base_dmg, bool isdot, int32 forced_school, float divider)
{
	int32 plus_damage = 0;
	Unit* caster = this;
	uint32 school = forced_school != -1? forced_school : spellInfo->School;

	if( spellInfo->c_is_flags & SPELL_FLAG_IS_NOT_USING_DMG_BONUS )
		return 0;

//------------------------------by school---------------------------------------------------
	plus_damage += caster->GetDamageDoneMod(school);
	plus_damage += pVictim->DamageTakenMod[school];
//------------------------------by victim type----------------------------------------------
	if(((Creature*)pVictim)->GetCreatureName() && caster->IsPlayer()&& !pVictim->IsPlayer())
		plus_damage += static_cast< Player* >(caster)->IncreaseDamageByType[((Creature*)pVictim)->GetCreatureName()->Type];
//==========================================================================================
//==============================+Spell Damage Bonus Modifications===========================
//==========================================================================================
//------------------------------by cast duration--------------------------------------------
	float dmgdoneaffectperc = 1.0f;
	if( spellInfo->Dspell_coef_override >= 0 && !isdot )
		plus_damage = float2int32( float( plus_damage ) * spellInfo->Dspell_coef_override );
	else if( spellInfo->OTspell_coef_override >= 0 && isdot )
		plus_damage = float2int32( float( plus_damage ) * spellInfo->OTspell_coef_override / divider );
	else
	{
		//Bonus to DD part
		if( spellInfo->fixed_dddhcoef >= 0 && !isdot )
			plus_damage = float2int32( float( plus_damage ) * spellInfo->fixed_dddhcoef );
		//Bonus to DoT part
		else if( spellInfo->fixed_hotdotcoef >= 0 && isdot )
		{
			plus_damage = float2int32( float( plus_damage ) * spellInfo->fixed_hotdotcoef / divider );
			if( caster->IsPlayer() )
			{
				int durmod = 0;
				SM_FIValue(caster->SM_FDur, &durmod, spellInfo);
				plus_damage += float2int32( float( plus_damage * durmod ) / 15000.0f );
			}
		}
		//In case we dont fit in previous cases do old thing
		else
		{
			plus_damage = float2int32( float( plus_damage ) * spellInfo->casttime_coef );
			float td = float( GetDuration( dbcSpellDuration.LookupEntry( spellInfo->durationIndex ),INVOC_DURATION) );
			if( spellInfo->NameHash == SPELL_HASH_MOONFIRE || spellInfo->NameHash == SPELL_HASH_IMMOLATE || spellInfo->NameHash == SPELL_HASH_ICE_LANCE || spellInfo->NameHash == SPELL_HASH_PYROBLAST )
				plus_damage = float2int32( float( plus_damage ) * float( 1.0f - ( ( td / 15000.0f ) / ( ( td / 15000.0f ) + dmgdoneaffectperc ) ) ) );
		}
	}

	//------------------------------by downranking----------------------------------------------
	//DOT-DD (Moonfire-Immolate-IceLance-Pyroblast)(Hack Fix)

	if(spellInfo->baseLevel > 0 && spellInfo->maxLevel > 0)
	{
		float downrank1 = 1.0f;
		if(spellInfo->baseLevel < 20)
		    downrank1 = 1.0f - (20.0f - float (spellInfo->baseLevel) ) * 0.0375f;
		float downrank2 = ( float(spellInfo->maxLevel + 5.0f) / float(static_cast< Player* >(caster)->getLevel()) );
		if(downrank2 >= 1 || downrank2 < 0)
		        downrank2 = 1.0f;
		dmgdoneaffectperc *= downrank1 * downrank2;
	}
//==========================================================================================
//==============================Bonus Adding To Main Damage=================================
//==========================================================================================
	int32 bonus_damage = float2int32(plus_damage * dmgdoneaffectperc);

	//bonus_damage +=pVictim->DamageTakenMod[school]; Bad copy-past i guess :P
	if(spellInfo->SpellGroupType[0] || spellInfo->SpellGroupType[1] || spellInfo->SpellGroupType[2])
	{
		SM_FIValue(caster->SM_FPenalty, &bonus_damage, spellInfo);

		SM_FIValue(caster->SM_FDamageBonus, &bonus_damage, spellInfo);
		int dmg_bonus_pct=0;
		SM_FIValue(caster->SM_PPenalty, &dmg_bonus_pct, spellInfo);		
		SM_FIValue(caster->SM_PDamageBonus,&dmg_bonus_pct, spellInfo);
		bonus_damage += base_dmg*dmg_bonus_pct/100;

	}
//------------------------------by school----------------------------------------------
	//Andy: These need to be multiplacative, and not give 500% bonus for speccinc arctic winds.
	float summaryPCTmod = 1.0f;
	summaryPCTmod *= caster->GetDamageDonePctMod(school); //value is initialized with 1 ANDY FIXME
	summaryPCTmod *= pVictim->DamageTakenPctMod[school];//value is initialized with 1 ANDY FIXME
	summaryPCTmod *= caster->DamageDoneModPCT[school];	// BURLEX FIXME, ANDY FIXEDME
	summaryPCTmod += pVictim->ModDamageTakenByMechPCT[spellInfo->mechanics];
	int32 res = (int32)((base_dmg + bonus_damage) * summaryPCTmod); // 1.x*(base_dmg+bonus_damage) == 1.0*base_dmg + 1.0*bonus_damage + 0.x*(base_dmg+bonus_damage) -> we add the returned value to base damage so we do not add it here (function returns bonus only)
	res -= base_dmg; //the function does add the base dmg into res, so remove it :P
return res;
}

void Unit::InterruptSpell()
{
	if(m_currentSpell)
	{
		//ok wtf is this
		//m_currentSpell->SendInterrupted(SPELL_FAILED_INTERRUPTED);
		//m_currentSpell->cancel();
		if(m_spellsbusy)
		{
			// shouldn't really happen. but due to spell sytem bugs there are some cases where this can happen.
			sEventMgr.AddEvent(this,&Unit::CancelSpell,m_currentSpell,EVENT_UNK,1,1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			m_currentSpell=NULL;
		}
		else
			m_currentSpell->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
	}
}

void Unit::Morph(uint32 displayid)
{
	this->SetUInt32Value(UNIT_FIELD_DISPLAYID, displayid);
}

void Unit::DeMorph()
{
	// hope it solves it :)
	uint32 displayid = this->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID);
	this->SetUInt32Value(UNIT_FIELD_DISPLAYID, displayid);
}

void Unit::Emote(EmoteType emote)
{
	if(emote == 0)
		return;

	WorldPacket data(SMSG_EMOTE, 12);
	data << uint32(emote);
	data << this->GetGUID();
	SendMessageToSet (&data, true);
}

void Unit::SendChatMessageToPlayer(uint8 type, uint32 lang, const char *msg, Player *plr)
{
	size_t UnitNameLength = 0, MessageLength = 0;
	CreatureInfo *ci = (m_objectTypeId == TYPEID_UNIT) ? ((Creature*)this)->creature_info : NULL;

	if(ci == NULL || plr == NULL)
		return;

	UnitNameLength = strlen((char*)ci->Name) + 1;
	MessageLength = strlen((char*)msg) + 1;

	WorldPacket data(SMSG_MESSAGECHAT, 35 + UnitNameLength + MessageLength);
	data << type;
	data << lang;
	data << GetGUID();
	data << uint32(0);			// new in 2.1.0
	data << uint32(UnitNameLength);
	data << ci->Name;
	data << uint64(0);
	data << uint32(MessageLength);
	data << msg;
	data << uint8(0x00);
	plr->GetSession()->SendPacket(&data);
}

void Unit::SendChatMessageAlternateEntry(uint32 entry, uint8 type, uint32 lang, const char * msg)
{
	size_t UnitNameLength = 0, MessageLength = 0;
	CreatureInfo *ci;

	ci = CreatureNameStorage.LookupEntry(entry);
	if(!ci)
		return;

	UnitNameLength = strlen((char*)ci->Name) + 1;
	MessageLength = strlen((char*)msg) + 1;

	WorldPacket data(SMSG_MESSAGECHAT, 35 + UnitNameLength + MessageLength);
	data << type;
	data << lang;
	data << GetGUID();
	data << uint32(0);			// new in 2.1.0
	data << uint32(UnitNameLength);
	data << ci->Name;
	data << uint64(0);
	data << uint32(MessageLength);
	data << msg;
	data << uint8(0x00);
	SendMessageToSet(&data, true);
}

void Unit::SendChatMessage(uint8 type, uint32 lang, const char *msg)
{
	char buffer_text[512];

	size_t UnitNameLength = 0, MessageLength = 0;
	CreatureInfo *ci = (m_objectTypeId == TYPEID_UNIT) ? ((Creature*)this)->creature_info : NULL;

	if(ci == NULL)
		return;

	strncpy(buffer_text,sWorld.ChangeLatin2UTF8(msg),500);

//	uint32 messageLength = (uint32)(strlen(buffer_text) + 1);

	UnitNameLength = strlen((char*)ci->Name) + 1;
	//MessageLength = strlen((char*)msg) + 1;
	MessageLength = strlen((char*)buffer_text) + 1;

	WorldPacket data(SMSG_MESSAGECHAT, 35 + UnitNameLength + MessageLength);
	data << type;
	data << lang;
	data << GetGUID();
	data << uint32(0);			// new in 2.1.0
	data << uint32(UnitNameLength);
	data << ci->Name;
	data << uint64(0);
	data << uint32(MessageLength);
	data << buffer_text; // msg;
	data << uint8(0x00); // 0 terminal
	SendMessageToSet(&data, true);
}

void Unit::WipeHateList()
{ 
	GetAIInterface()->WipeHateList(); 
}
void Unit::ClearHateList()
{
	GetAIInterface()->ClearHateList();
}

void Unit::WipeTargetList() 
{ 
	GetAIInterface()->WipeTargetList(); 
}

void Unit::AddInRangeObject(Object* pObj)
{
	if((pObj->GetTypeId() == TYPEID_UNIT) || (pObj->GetTypeId() == TYPEID_PLAYER))
	{
		if( isHostile( this, (Unit*)pObj ) )
			m_oppFactsInRange.insert(pObj);

		// commented - this code won't work anyway due to objects getting added in range before they are created - burlex
		/*if( GetTypeId() == TYPEID_PLAYER )
		{
			if( static_cast< Player* >( this )->InGroup() )
			{
				if( pObj->GetTypeId() == TYPEID_UNIT )
				{
					if(((Creature*)pObj)->Tagged)
					{
						if( static_cast< Player* >( this )->GetGroup()->HasMember( pObj->GetMapMgr()->GetPlayer( (uint32)((Creature*)pObj)->TaggerGuid ) ) )
						{
							uint32 Flags;
							Flags = ((Creature*)pObj)->m_uint32Values[UNIT_DYNAMIC_FLAGS];
							Flags |= U_DYN_FLAG_TAPPED_BY_PLAYER;
							ByteBuffer buf1(500);
							((Creature*)pObj)->BuildFieldUpdatePacket(&buf1, UNIT_DYNAMIC_FLAGS, Flags);
							static_cast< Player* >( this )->PushUpdateData( &buf1, 1 );
						}
					}
				}
			}
		}		*/
	}

	Object::AddInRangeObject(pObj);
}//427

void Unit::OnRemoveInRangeObject(Object* pObj)
{
	m_oppFactsInRange.erase(pObj);

	if (GetAIInterface() != NULL)
		GetAIInterface()->WipeReferences(pObj);

	if(pObj->GetTypeId() == TYPEID_UNIT || pObj->GetTypeId() == TYPEID_PLAYER)
	{
		/*if(m_useAI)*/

		Unit *pUnit = static_cast<Unit*>(pObj);
		GetAIInterface()->CheckTarget(pUnit);

		if(GetUInt64Value(UNIT_FIELD_CHARM) == pObj->GetGUID())
			if(m_currentSpell)
				m_currentSpell->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"

        Object::OnRemoveInRangeObject(pObj);
        if(critterPet == pObj)
		{
			critterPet->SafeDelete();
			critterPet = NULL;
		}
	}
    else
    {
        Object::OnRemoveInRangeObject(pObj);
    }
}

void Unit::ClearInRangeSet()
{
	Object::ClearInRangeSet();
}

//Events
void Unit::EventAddEmote(EmoteType emote, uint32 time)
{
	m_oldEmote = GetUInt32Value(UNIT_NPC_EMOTESTATE);
	SetUInt32Value(UNIT_NPC_EMOTESTATE,emote);
	sEventMgr.AddEvent(this, &Creature::EmoteExpire, EVENT_UNIT_EMOTE, time, 1,0);
}

void Unit::EmoteExpire()
{
	SetUInt32Value(UNIT_NPC_EMOTESTATE, m_oldEmote);
	sEventMgr.RemoveEvents(this, EVENT_UNIT_EMOTE);
}

uint32 Unit::GetResistance(uint32 type)
{
	return GetUInt32Value(UNIT_FIELD_RESISTANCES+type);
}

void Unit::MoveToWaypoint(uint32 wp_id)
{
	if(this->m_useAI && this->GetAIInterface() != NULL)
	{
		AIInterface *ai = GetAIInterface();
		WayPoint *wp = ai->getWayPoint(wp_id);
		if(!wp)
		{
			sLog.outString("WARNING: Invalid WP specified in MoveToWaypoint.");
			return;
		}

		ai->m_currentWaypoint = wp_id;
		if(wp->flags!=0)
			ai->m_moveRun = true;
		ai->MoveTo(wp->x, wp->y, wp->z, 0);
	}
}

int32 Unit::GetDamageDoneMod(uint32 school)
{
	if( this->IsPlayer() )
	   return (int32)GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_POS + school ) - (int32)GetUInt32Value( PLAYER_FIELD_MOD_DAMAGE_DONE_NEG + school );
	else
	   return static_cast< Creature* >( this )->ModDamageDone[school];
}
	
float Unit::GetDamageDonePctMod(uint32 school)
{
   if(this->IsPlayer())
	   return m_floatValues[PLAYER_FIELD_MOD_DAMAGE_DONE_PCT+school];
	else
	   return ((Creature*)this)->ModDamageDonePct[school];
}

void Unit::CalcDamage()
{
	if( IsPlayer() )
		static_cast< Player* >( this )->CalcDamage();
	else
	{
	float r;
	float delta;
	float mult;
	
	float ap_bonus = float(GetAP())/14000.0f;

		float bonus = ap_bonus*GetUInt32Value(UNIT_FIELD_BASEATTACKTIME);
	
		delta = float(((Creature*)this)->ModDamageDone[0]);
		mult = float(static_cast<Creature *>(this)->ModDamageDonePct[0]);
		r = BaseDamage[0]*mult+delta+bonus;
		// give some diversity to pet damage instead of having a 77-78 damage range (as an example)
		SetFloatValue(UNIT_FIELD_MINDAMAGE,r > 0 ? ( IsPet() ? r * 0.9f : r ) : 0 );
		r = BaseDamage[1]*mult+delta+bonus;
		SetFloatValue(UNIT_FIELD_MAXDAMAGE, r > 0 ? ( IsPet() ? r * 1.1f : r ) : 0 );
	
	//	SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE,BaseRangedDamage[0]*mult+delta);
	//	SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE,BaseRangedDamage[1]*mult+delta);
  
	}	
}

//returns absorbed dmg
uint32 Unit::ManaShieldAbsorb(uint32 dmg, SpellEntry* sp)
{
	if(!m_manashieldamt)
		return 0;
	//mana shield group->16. the only

	uint32 mana = GetUInt32Value(UNIT_FIELD_POWER1);
	int32 effectbonus = SM_PEffectBonus ? SM_PEffectBonus[16] : 0;

	int32 potential = (mana*50)/((100+effectbonus));
	if(potential>m_manashieldamt)
		potential = m_manashieldamt;

	if((int32)dmg<potential)
		potential = dmg;

	uint32 cost = (potential*(100+effectbonus))/50;

	SetUInt32Value(UNIT_FIELD_POWER1,mana-cost);
	m_manashieldamt -= potential;
	if(!m_manashieldamt)
		RemoveAura(m_manaShieldId);
		
	if (potential > 0 && m_incanterAbsorption > 0)
	{
		SpellEntry *spInfo = dbcSpell.LookupEntry(44413);
		if(spInfo)
		{
			Spell *sp = new Spell(static_cast<Unit *>(this),spInfo,true,NULL);
			SpellCastTargets tgt;
			int spamount = std::min(float2int32(this->GetUInt32Value(UNIT_FIELD_HEALTH) * 0.05f), float2int32((potential * m_incanterAbsorption) / 100));
			sp->forced_basepoints[0] = spamount;
			tgt.m_target = (Object *)this; //tgt.m_unitTarget=GetGUID();
			sp->prepare(&tgt);
		}
	}

	return potential;
}

// grep: Remove any AA spells that aren't owned by this player.
//		 Otherwise, if he logs out and in and joins another group,
//		 he'll apply it to them.
/*
void Unit::RemoveAllAreaAuras()
{
	AuraList::iterator itr,it1;
	for(itr = m_auras.begin();itr!=m_auras.end();)
	{
		it1 = itr++;
		if(((*it1)->m_spellProto->Effect[0] == SPELL_EFFECT_APPLY_AREA_AURA ||
			(*it1)->m_spellProto->Effect[1] == SPELL_EFFECT_APPLY_AREA_AURA ||
			(*it1)->m_spellProto->Effect[2] == SPELL_EFFECT_APPLY_AREA_AURA) && (*it1)->GetCaster() != this)
		{
			(*it1)->Remove();
			m_auras.erase(it1);
		} 
	}
}
*/
uint32 Unit::AbsorbDamage( Object *Attacker, uint32 School, uint32* dmg, SpellEntry * pSpell )
{
	if( dmg == NULL )
		return 0;

	if( School > 6 )
		return 0;

	SchoolAbsorb::iterator i, j;
	uint32 abs = 0;
	int32 reflect_pct = 0;
	
	for( i = Absorbs[School].begin(); i != Absorbs[School].end(); )
	{
		if( (int32)(*dmg) >= (*i)->amt)//remove this absorb
		{
			(*dmg) -= (*i)->amt;
			abs += (*i)->amt;
			reflect_pct += (*i)->reflect_pct;			
			j = i++;

			if( i != Absorbs[School].end() )
			{
				while( (*i)->spellid == (*j)->spellid )
				{
					++i;
					if( i == Absorbs[School].end() )
						break;
				}
			}
			
			this->RemoveAura((*j)->spellid); //,(*j)->caster);
			if(!*dmg)//absorbed all dmg
				break;		
		}
		else //absorb full dmg
		{
			abs += *dmg;
			reflect_pct += (*i)->reflect_pct;
			(*i)->amt -= *dmg;
			*dmg=0;
			break;
		}
	}
	
	if( abs > 0 )
	{
		if(reflect_pct > 0 && Attacker && Attacker->IsUnit() )
		{
			int32 amt = float2int32(abs * (reflect_pct / 100.0f ));
			DealDamage( static_cast<Unit *>( Attacker ), amt, 0, 0, 0 );
		}
		if(m_incanterAbsorption > 0)
		{
			SpellEntry *spInfo = dbcSpell.LookupEntry(44413);
			if(spInfo)
			{
				Spell *sp = new Spell(this, spInfo, true, NULL);
				SpellCastTargets tgt;
				int spamount = std::min(float2int32(GetUInt32Value(UNIT_FIELD_HEALTH) * 0.05f), float2int32((abs * m_incanterAbsorption) / 100));
				sp->forced_basepoints[0] = spamount;
				tgt.m_target = (Object *)this; //tgt.m_unitTarget=this->GetGUID();
				sp->prepare(&tgt);
			}
		}
	}
	
	return abs;
}

bool Unit::setDetectRangeMod(uint64 guid, int32 amount)
{
	int next_free_slot = -1;
	for(int i = 0; i < 5; i++)
	{
		if(m_detectRangeGUID[i] == 0 && next_free_slot == -1)
		{
			next_free_slot = i;
		}
		if(m_detectRangeGUID[i] == guid)
		{
			m_detectRangeMOD[i] = amount;
			return true;
		}
	}
	if(next_free_slot != -1)
	{
		m_detectRangeGUID[next_free_slot] = guid;
		m_detectRangeMOD[next_free_slot] = amount;
		return true;
	}
	return false;
}

void Unit::unsetDetectRangeMod(uint64 guid)
{
	for(int i = 0; i < 5; i++)
	{
		if(m_detectRangeGUID[i] == guid)
		{
			m_detectRangeGUID[i] = 0;
			m_detectRangeMOD[i] = 0;
		}
	}
}

int32 Unit::getDetectRangeMod(uint64 guid)
{
	for(int i = 0; i < 5; i++)
	{
		if(m_detectRangeGUID[i] == guid)
		{
			return m_detectRangeMOD[i];
		}
	}
	return 0;
}

void Unit::SetStandState(uint8 standstate)
{
	StandState TestStandStateEgal = this->GetStandState();
    if(TestStandStateEgal == standstate) return; // Optimisation, Test, (Branruz)

	SetByte( UNIT_FIELD_BYTES_1, 0, standstate );
	if( standstate == STANDSTATE_STAND )//standup
		RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_STAND_UP);

	if( m_objectTypeId == TYPEID_PLAYER )
		static_cast< Player* >( this )->GetSession()->OutPacket( SMSG_STANDSTATE_UPDATE, 1, &standstate );
}

void Unit::RemoveAurasByInterruptFlag(uint32 flag)
{
	Aura * a;
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		a = m_auras[x];
		if(a == NULL)
			continue;
		
		//some spells do not get removed all the time only at specific intervals
		if(a->m_spellProto->auraInterruptFlags & flag)
		{
			a->Remove();
			m_auras[x] = NULL;
		}
	}
}

bool Unit::HasAuraVisual(uint32 visualid)
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
	if(m_auras[x] && m_auras[x]->GetSpellProto()->SpellVisual==visualid)
	{
		return true;
	}

	return false;
}

bool Unit::HasAura(uint32 spellid)
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
		if(m_auras[x] && m_auras[x]->GetSpellId()== spellid)
		{
			return true;
		}

		return false;
}

bool Unit::HasAura(Aura* a)
{
	for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
		if(m_auras[x] && m_auras[x] == a)
		{
			return true;
		}

		return false;
}


void Unit::DropAurasOnDeath()
{
	for(uint32 x=0;x<MAX_AURAS;x++)
    {
        if(m_auras[x])
        {
            if(m_auras[x] && m_auras[x]->GetSpellProto()->attributesExC & ATTRIBUTESEXC_CAN_PERSIST_AND_CASTED_WHILE_DEAD)
                continue;
            else
	        {
		        m_auras[x]->Remove();
	        }
        }
    }
}

void Unit::UpdateSpeed()
{
	if(GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID) == 0)
	{
		if(IsPlayer())
			m_runSpeed = m_base_runSpeed*(1.0f + ((float)m_speedModifier)/100.0f);
		else
			m_runSpeed = m_base_runSpeed*(1.0f + ((float)m_speedModifier)/100.0f);
	}
	else
	{
		if(IsPlayer())
		{
			m_runSpeed = m_base_runSpeed*(1.0f + ((float)m_mountedspeedModifier)/100.0f);
			m_runSpeed += (m_speedModifier<0) ? (m_base_runSpeed*((float)m_speedModifier)/100.0f) : 0;
		}
		else
		{
			m_runSpeed = m_base_runSpeed*(1.0f + ((float)m_mountedspeedModifier)/100.0f);
			m_runSpeed += (m_speedModifier<0) ? (m_base_runSpeed*((float)m_speedModifier)/100.0f) : 0;
		}
	}

	m_flySpeed = PLAYER_NORMAL_FLIGHT_SPEED*(1.0f + ((float)m_flyspeedModifier)/100.0f);

	// Limit speed due to effects such as http://www.wowhead.com/?spell=31896 [Judgement of Justice]
	if( m_maxSpeed && m_runSpeed > m_maxSpeed )
	{
		m_runSpeed = m_maxSpeed;
	}
	
	if(IsVehicle() && ((Vehicle *)this)->GetControllingUnit())
	{
		Unit* pUnit = ((Vehicle *)this)->GetControllingUnit();
		pUnit->m_runSpeed = m_runSpeed;
		pUnit->m_flySpeed = m_flySpeed;
		((Vehicle *)this)->SetSpeed(RUN, m_runSpeed);
		((Vehicle *)this)->SetSpeed(FLY, m_flySpeed);

		if(pUnit->IsPlayer())
		{
			if(((Player*)pUnit)->m_changingMaps)
				static_cast< Player* >(pUnit)->resend_speed = true;
			else
			{
				static_cast< Player* >(pUnit)->SetPlayerSpeed(RUN, m_runSpeed);
				static_cast< Player* >(pUnit)->SetPlayerSpeed(FLY, m_flySpeed);
			}
		}
	}
	
	if(IsPlayer())
	{
		if(((Player*)this)->m_changingMaps)
			static_cast< Player* >( this )->resend_speed = true;
		else
		{
			static_cast< Player* >( this )->SetPlayerSpeed(RUN, m_runSpeed);
			static_cast< Player* >( this )->SetPlayerSpeed(FLY, m_flySpeed);
		}
	}
}

bool Unit::HasActiveAura(uint32 spellid)
{
	for(uint32 x=0;x<MAX_AURAS;x++)
	if(m_auras[x] && m_auras[x]->GetSpellId()==spellid)
	{
		return true;
	}

	return false;
}

bool Unit::HasActiveAura(uint32 spellid,uint64 guid)
{
	for(uint32 x=0;x<MAX_AURAS;x++)
	if(m_auras[x] && m_auras[x]->GetSpellId()==spellid && m_auras[x]->m_casterGuid==guid)
	{
		return true;
	}

	return false;
}

void Unit::EventSummonPetExpire()
{
	if(summonPet)
	{
		if(summonPet->GetEntry() == 7915)//Goblin Bomb
		{
			SpellEntry *spInfo = dbcSpell.LookupEntry(13259);
			if(!spInfo)
				return;

			Spell*sp=new Spell(summonPet,spInfo,true,NULL);
			SpellCastTargets tgt;
			tgt.m_target=summonPet;
			sp->prepare(&tgt);
		}
		else
		{
			summonPet->RemoveFromWorld(false, true);
			summonPet = NULL;
		}
	}
	sEventMgr.RemoveEvents(this, EVENT_SUMMON_PET_EXPIRE);
}

void Unit::CastSpellOld(Unit* Target, SpellEntry* Sp, bool triggered)
{
	if( Sp == NULL )
		return;

	Spell *newSpell = new Spell(this, Sp, triggered, 0);
	SpellCastTargets targets;
	if(Target)
	{
		targets.m_targetMask |= TARGET_FLAG_UNIT;
		targets.m_target = Target;
	}
	else
	{
		newSpell->GenerateTargets(&targets);
	}
	newSpell->prepare(&targets);
}

void Unit::CastSpellOld(Unit *Target, uint32 SpellID, bool triggered)
{
	SpellEntry * ent = dbcSpell.LookupEntry(SpellID);
	if(ent == 0) return;

	CastSpell(Target, ent, triggered);
}

void Unit::CastSpell(Object *Target, uint32 SpellID, bool triggered)
{
	SpellEntry * ent = dbcSpell.LookupEntry(SpellID);
	if(ent == 0) return;

	CastSpell(Target, ent, triggered);
}

void Unit::CastSpell(Object *Target, SpellEntry* Sp, bool triggered)
{
    if( Sp == NULL )
	{
		Log.Error("[Unit::CastSpell]","(%s) Spell NULL", this->GetPlayerFrom()->GetName());
        return;
    }

    if(Target == NULL)
    {
         Log.Error("[Unit::CastSpell]","(%s) Target NULL, spellId %u, trigg %s",this->GetPlayerFrom()->GetName(),
			                                                                    Sp->Id,(triggered) ? "Oui" : "Non");
         return;
    } 

	if (Target->IsGO())
		CastSpellOld(static_cast<GameObject *>(Target), Sp, triggered);
	else if (Target->IsUnit())
		CastSpellOld(static_cast<Unit *>(Target), Sp, triggered);
}

void Unit::CastSpellOld(GameObject *Target, SpellEntry* Sp, bool triggered)
{
	if( Sp == NULL )
		return;

	Spell *newSpell = new Spell(this, Sp, triggered, NULL);
	SpellCastTargets targets;
	if(Target)
	{
		targets.m_targetMask |= TARGET_FLAG_OBJECT;
		targets.m_target = Target;
	}
	else
	{
		newSpell->GenerateTargets(&targets);
	}
	newSpell->prepare(&targets);
}

void Unit::CastSpellOld(GameObject *Target, uint32 SpellID, bool triggered)
{
	SpellEntry * ent = dbcSpell.LookupEntry(SpellID);
	if(ent == 0) return;

	CastSpell(Target, ent, triggered);
}

void Unit::CastSpell(uint64 targetGuid, SpellEntry* Sp, bool triggered)
{
	if( Sp == NULL )
		return;

	SpellCastTargets targets(GetMapMgr()->GetUnit(targetGuid));
	Spell *newSpell = new Spell(this, Sp, triggered, 0);
	newSpell->prepare(&targets);
}

void Unit::CastSpell(uint64 targetGuid, uint32 SpellID, bool triggered)
{
	SpellEntry * ent = dbcSpell.LookupEntry(SpellID);
	if(ent == 0) return;

	CastSpell(targetGuid, ent, triggered);
}
void Unit::CastSpellAoF(float x,float y,float z,SpellEntry* Sp, bool triggered)
{
	if( Sp == NULL )
		return;

	SpellCastTargets targets;
	// Normalement les coordonnées sont deja init par l'appel de l'AIAgent
	targets.m_destX = x;
	targets.m_destY = y;
	targets.m_destZ = z;
	targets.m_targetMask=TARGET_FLAG_DEST_LOCATION;
	Spell *newSpell = new Spell(this, Sp, triggered, 0);
	newSpell->prepare(&targets);
}

void Unit::PlaySpellVisual(uint64 target, uint32 spellVisual)
{
	WorldPacket data(SMSG_PLAY_SPELL_VISUAL, 12);
	data << target << spellVisual;
	SendMessageToSet(&data, true);
}

void Unit::Root()
{
	this->m_special_state |= UNIT_STATE_ROOT;

	if(m_objectTypeId == TYPEID_PLAYER)
	{
		static_cast< Player* >( this )->SetMovement(MOVE_ROOT, 1);
	}
	else
	{
		m_aiInterface->m_canMove = false;
		m_aiInterface->StopMovement(1);
	}
}

void Unit::Unroot()
{
	this->m_special_state &= ~UNIT_STATE_ROOT;

	if(m_objectTypeId == TYPEID_PLAYER)
	{
		static_cast< Player* >( this )->SetMovement(MOVE_UNROOT, 5);
	}
	else
	{
		m_aiInterface->m_canMove = true;
	}
}

void Unit::RemoveAurasByBuffType(uint32 buff_type, const uint64 &guid, uint32 skip)
{
	uint64 sguid = buff_type >= SPELL_TYPE_BLESSING ? guid : 0;
	
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		if(m_auras[x] != NULL && m_auras[x]->GetSpellProto()->buffType & buff_type && m_auras[x]->GetSpellId()!=skip)
		{
			if(!sguid || m_auras[x]->GetCasterGUID() == sguid)
				m_auras[x]->Remove();
		}
	}
}

void Unit::RemoveAurasByBuffIndexType(uint32 buff_index_type, const uint64 &guid)
{
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		if(m_auras[x] != NULL && m_auras[x]->GetSpellProto()->buffIndexType == buff_index_type)
		{
			if(!guid || (guid && m_auras[x]->m_casterGuid == guid))
				m_auras[x]->Remove();
		}
	}
}

bool Unit::HasAurasOfBuffType(uint32 buff_type, const uint64 &guid,uint32 skip)
{
	uint64 sguid = (buff_type == SPELL_TYPE_BLESSING || buff_type == SPELL_TYPE_WARRIOR_SHOUT) ? guid : 0;

	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		if(m_auras[x] != NULL && m_auras[x]->GetSpellProto()->buffType & buff_type && m_auras[x]->GetSpellId()!=skip)
			if(!sguid || (sguid && m_auras[x]->GetCasterGUID() == sguid))
				return true;
	}

	return false;
}

AuraCheckResponse Unit::AuraCheck(uint32 name_hash, uint32 rank, Object *caster)
{
	AuraCheckResponse resp;

	// no error for now
	resp.Error = AURA_CHECK_RESULT_NONE;
	resp.Misc  = 0;

	// look for spells with same namehash
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
//		if(m_auras[x] && m_auras[x]->GetSpellProto()->NameHash == name_hash && m_auras[x]->GetCaster()==caster)
		if(m_auras[x] && m_auras[x]->GetSpellProto()->NameHash == name_hash)
		{
			// we've got an aura with the same name as the one we're trying to apply
			resp.Misc = m_auras[x]->GetSpellProto()->Id;

			// compare the rank to our applying spell
			if(m_auras[x]->GetSpellProto()->RankNumber > rank)
				resp.Error = AURA_CHECK_RESULT_HIGHER_BUFF_PRESENT;
			else
				resp.Error = AURA_CHECK_RESULT_LOWER_BUFF_PRESENT;

			// we found something, save some loops and exit
			break;
		}
	}

	// return it back to our caller
	return resp;
}

AuraCheckResponse Unit::AuraCheck(uint32 name_hash, uint32 rank, Aura* aur, Object *caster)
{
	AuraCheckResponse resp;

	// no error for now
	resp.Error = AURA_CHECK_RESULT_NONE;
	resp.Misc  = 0;

	// look for spells with same namehash
//	if(aur->GetSpellProto()->NameHash == name_hash && aur->GetCaster()==caster)
	if(aur->GetSpellProto()->NameHash == name_hash && rank != 0)
	{
		// we've got an aura with the same name as the one we're trying to apply
		resp.Misc = aur->GetSpellProto()->Id;

		// compare the rank to our applying spell
		if(aur->GetSpellProto()->RankNumber > rank)
			resp.Error = AURA_CHECK_RESULT_HIGHER_BUFF_PRESENT;
		else
			resp.Error = AURA_CHECK_RESULT_LOWER_BUFF_PRESENT;
	}

	// return it back to our caller
	return resp;
}

AuraCheckResponse Unit::AuraCheck(SpellEntry *info)
{
	AuraCheckResponse resp;

	// no error for now
	resp.Error = AURA_CHECK_RESULT_NONE;
	resp.Misc  = 0;

	// look for spells with same namehash
	bool stronger = false;
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		if( m_auras[x] == NULL )
			continue;

		for( uint32 loop = 0; loop < 3; loop++ )
		{
			if( m_auras[x]->GetSpellProto()->Effect[loop] == info->Effect[loop] && info->Effect[loop] > 0 )
			{
				if( info->EffectBasePoints[loop] < 0 )
				{
					if( info->EffectBasePoints[loop] <= m_auras[x]->GetSpellProto()->EffectBasePoints[loop] )
					{
						stronger = true;
						break;
					}
				}
				else if( info->EffectBasePoints[loop] > 0 )
				{
					if( info->EffectBasePoints[loop] >= m_auras[x]->GetSpellProto()->EffectBasePoints[loop] )
					{
						stronger = true;
						break;
					}
				}
			}
		}

		if( stronger )
		{
			resp.Error = AURA_CHECK_RESULT_HIGHER_BUFF_PRESENT;
			break;
		}
	}

	resp.Error = AURA_CHECK_RESULT_LOWER_BUFF_PRESENT;
	// return it back to our caller
	return resp;
}

void Unit::OnPushToWorld()
{
	for(uint32 x = 0; x < MAX_AURAS+MAX_PASSIVE_AURAS; ++x)
	{
		if(m_auras[x] != 0)
			m_auras[x]->RelocateEvents();
	}
}

void Unit::RemoveFromWorld(bool free_guid)
{
	CombatStatus.OnRemoveFromWorld();

	if (m_CurrentVehicle != NULL)
	{
		m_CurrentVehicle->RemovePassenger(static_cast<Unit *>(this));
		m_CurrentVehicle = NULL;
	}

	//remove guardians
	for (std::set<Creature *>::iterator itr=m_summonedCreatures.begin(); itr!=m_summonedCreatures.end(); ++itr)
		(*itr)->SafeDelete();
	m_summonedCreatures.clear();

	for(uint32 x = 0; x < MAX_AURAS+MAX_PASSIVE_AURAS; ++x)
	{
		if(m_auras[x] != 0)
		{
			if( m_auras[x]->m_deleted )
			{
				m_auras[x] = NULL;
				continue;
			}
			m_auras[x]->RelocateEvents();
		}
	}

	if(critterPet != 0)
	{
		critterPet->RemoveFromWorld(false, true);
		delete critterPet;
		critterPet = 0;
	}

	if(dynObj != NULL)
		dynObj->Remove();

	for(uint32 i = 0; i < 4; ++i)
	{
		if(m_ObjectSlots[i] != 0)
		{
			GameObject * obj = m_mapMgr->GetGameObject(m_ObjectSlots[i]);
			if(obj)
				obj->ExpireAndDelete();

			m_ObjectSlots[i] = 0;
		}
	}

	Object::RemoveFromWorld(free_guid);

	for (std::set<Object*>::iterator itr=m_objectsInRange.begin(); itr!=m_objectsInRange.end(); ++itr)
	{
		(*itr)->RemoveInRangeObject(this);
		RemoveInRangeObject(*itr);
	}

	m_aiInterface->WipeReferences();
}

void Unit::RemoveAurasByInterruptFlagButSkip(uint32 flag, uint32 skip)
{
	Aura* a = NULL;
	for(uint32 x=0;x<MAX_AURAS;x++)
	{
		a = m_auras[x];
		if(a == NULL)
			continue;

		if( a->GetDuration(INVOC_DURATION ) > 0 && (int32)(a->GetTimeLeft(INVOC_DURATION )+500) > a->GetDuration(INVOC_DURATION ) )
			continue;//pretty new aura, don't remove

		//some spells do not get removed all the time only at specific intervals
		if((a->m_spellProto->auraInterruptFlags & flag) && (a->m_spellProto->Id != skip) && a->m_spellProto->proc_interval==0)
		{
			//the black sheeps of sociaty
			if(a->m_spellProto->auraInterruptFlags & AURA_INTERRUPT_ON_CAST_SPELL)
			{
				switch(a->GetSpellProto()->Id)
				{
					//priest - surge of light
					case 33151:
						{
							//our luck. it got trigered on smite..we do not remove it just yet
							if( m_currentSpell && m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_SMITE )
								continue;

							//this spell gets removed only when casting smite
						    SpellEntry *spi = dbcSpell.LookupEntry( skip );
							if( spi && spi->NameHash != SPELL_HASH_SMITE )
								continue;
						}break;
					case 34936:	// Backlash
						{
							if( m_currentSpell && m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_SHADOW_BOLT )
								continue;
							if( m_currentSpell && m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_INCINERATE )
								continue;
							SpellEntry *spi = dbcSpell.LookupEntry( skip );
							if( spi && spi->NameHash != SPELL_HASH_SHADOW_BOLT && spi->NameHash != SPELL_HASH_INCINERATE )
								continue;
						}break;
					// priest - holy conc
				case 34754:
					{
						if( m_currentSpell!=NULL && 
							!(m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_FLASH_HEAL ||
							m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_BINDING_HEAL ||
							m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_GREATER_HEAL))
							continue;
						SpellEntry *spi = NULL;
						spi = dbcSpell.LookupEntry( skip );
						if( spi!=NULL  && spi->NameHash != SPELL_HASH_FLASH_HEAL && spi->NameHash != SPELL_HASH_BINDING_HEAL && spi->NameHash != SPELL_HASH_GREATER_HEAL)
							continue;
					}break;
					case 17941: //Shadow Trance
						{
							if( m_currentSpell && m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_SHADOW_BOLT)
								continue;
							SpellEntry *spi = dbcSpell.LookupEntry( skip );
							if( spi && spi->NameHash != SPELL_HASH_SHADOW_BOLT )
								continue;
						}break;
					//Arcane Potency
				case 57529:
				case 57531:
					{
						if( m_currentSpell != NULL && !(m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_PRESENCE_OF_MIND ||
							m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_CLEARCASTING ))
							continue;

						SpellEntry *spi = NULL;
						spi = dbcSpell.LookupEntry( skip );
						if( spi != NULL || !(spi->c_is_flags & SPELL_FLAG_IS_DAMAGING) )
							continue;

					}break;
					//paladin - Art of war
				case 53489:
				case 59578:
					{
						if( m_currentSpell != NULL && m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_FLASH_OF_LIGHT )
							continue;
						SpellEntry *spi = NULL;
						spi = dbcSpell.LookupEntry( skip );
						if( spi != NULL && spi->NameHash != SPELL_HASH_FLASH_OF_LIGHT )
							continue;
					}break;
					//paladin - Infusion of light
				case 53672:
				case 54149:
					{
						if( m_currentSpell != NULL && !(m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_FLASH_OF_LIGHT ||
							m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_HOLY_LIGHT))
							continue;
						SpellEntry *spi = NULL;
						spi = dbcSpell.LookupEntry( skip );
						if( spi != NULL && spi->NameHash != SPELL_HASH_FLASH_OF_LIGHT && spi->NameHash != SPELL_HASH_HOLY_LIGHT)
							continue;
					}break;
					//Mage - Firestarter
				case 54741:
					{
						if( m_currentSpell != NULL && m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_FLAMESTRIKE )
							continue;
						SpellEntry *spi = NULL;
						spi = dbcSpell.LookupEntry( skip );
						if( spi != NULL && spi->NameHash != SPELL_HASH_FLAMESTRIKE )
							continue;
					}break;
					// Glyph of Revenge Proc
				case 58363:
					{
						if( m_currentSpell != NULL && m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_HEROIC_STRIKE )
							continue;
						SpellEntry *spi = NULL;
						spi = dbcSpell.LookupEntry( skip );
						if( spi != NULL && spi->NameHash != SPELL_HASH_HEROIC_STRIKE )
							continue;
					}break;
				case 18708: //Fel Domination
					{
						SpellEntry *spi = NULL;
						spi = dbcSpell.LookupEntry( skip );
						if( spi != NULL && spi->NameHash != SPELL_HASH_SUMMON_IMP &&
							spi->NameHash != SPELL_HASH_SUMMON_VOIDWALKER &&
							spi->NameHash != SPELL_HASH_SUMMON_SUCCUBUS &&
							spi->NameHash != SPELL_HASH_SUMMON_FELHUNTER &&
							spi->NameHash != SPELL_HASH_SUMMON_FELGUARD )
							continue;
					}break;
				case 46916: // Bloodsurge
					{
						SpellEntry *spi = NULL;
						spi = dbcSpell.LookupEntry( skip );
						if( spi != NULL && spi->NameHash != SPELL_HASH_SLAM )
							continue;
					}break;
				case 14177: // Cold Blood
					{
						SpellEntry *spi = NULL;
						spi = dbcSpell.LookupEntry( skip );
						if( spi != NULL && !(spi->c_is_flags & SPELL_FLAG_IS_DAMAGING) && spi->NameHash != SPELL_HASH_MUTILATE )
							continue;
						else
							a->Remove();
					}break;
				case 31834: // Light's Grace
					{
						if( m_currentSpell != NULL && m_currentSpell->m_spellInfo->NameHash == SPELL_HASH_HOLY_LIGHT )
							continue;

						SpellEntry *spi = NULL;
						spi = dbcSpell.LookupEntry( skip );
						if( spi != NULL && spi->NameHash != SPELL_HASH_HOLY_LIGHT )
							continue;
					}break;
					// Shadowstep
				case 44373:
				case 36563:
					{
						SpellEntry *spi = NULL;
						spi = dbcSpell.LookupEntry( skip );
						if( spi != NULL && !(spi->c_is_flags & SPELL_FLAG_IS_DAMAGING) )
							continue;
					}break;
				}
			}
			a->Remove();
		}
	}
}

int Unit::HasAurasWithNameHash(uint32 name_hash)
{
	for(uint32 x = 0; x < MAX_AURAS; ++x)
	{
		if(m_auras[x] && m_auras[x]->GetSpellProto()->NameHash == name_hash)
			return m_auras[x]->m_spellProto->Id;
	}

	return 0;
}

bool Unit::HasNegativeAuraWithNameHash(uint32 name_hash)
{
	for(uint32 x = MAX_POSITIVE_AURAS; x < MAX_AURAS; ++x)
	{
		if(m_auras[x] && m_auras[x]->GetSpellProto()->NameHash == name_hash)
			return true;
	}

	return false;
}

bool Unit::HasNegativeAura(uint32 spell_id)
{
	for(uint32 x = MAX_POSITIVE_AURAS; x < MAX_AURAS; ++x)
	{
		if(m_auras[x] && m_auras[x]->GetSpellProto()->Id == spell_id)
			return true;
	}

	return false;
}

uint32 Unit::AddAuraVisual(uint32 spellid, uint32 count, bool positive)
{
	int32 free = -1;
	uint32 start = positive ? 0 : MAX_POSITIVE_AURAS;
	uint32 end_  = positive ? MAX_POSITIVE_AURAS : MAX_AURAS;

	for(uint32 x = start; x < end_; ++x)
	{
		if(free == -1 && m_auras[x] == 0)
			free = x;
	}

	if(free == -1)
		return 0xFF;

	return free;

	/*uint8 flagslot = (free / 4);
	uint32 value = GetUInt32Value((uint16)(UNIT_FIELD_AURAFLAGS + flagslot));

	uint8 aurapos = (free%4)*8;
	value &= ~(0xFF<<aurapos);
	if(positive)
		value |= (0x1F<<aurapos);
	else
		value |= (0x09<<aurapos);

	SetUInt32Value((uint16)(UNIT_FIELD_AURAFLAGS + flagslot), value);
	SetUInt32Value(UNIT_FIELD_AURA + free, spellid);
	ModAuraStackCount(free, count);
	SetAuraSlotLevel(free, positive);
	
	return free;*/
}

void Unit::SetAuraSlotLevel(uint32 slot, bool positive)
{
	/*uint32 index = slot / 4;
	uint32 val = m_uint32Values[UNIT_FIELD_AURALEVELS + index];
	uint32 bit = (slot % 4) * 8;
	val &= ~(0xFF << bit);
	if(positive)
		val |= (0x46 << bit);
	else
		val |= (0x19 << bit);
	
	SetUInt32Value(UNIT_FIELD_AURALEVELS + index, val);*/
}

void Unit::RemoveAuraVisual(uint32 slot)
{
	WorldPacket data(SMSG_AURA_UPDATE, 20);
	FastGUIDPack(data, GetGUID());
	data << uint8(slot);
	data << uint32(0);
	SendMessageToSet(&data, true);
}

void Unit::ModifyAuraApplications(uint32 spellid, uint32 count)
{
	/*for(uint32 x = 0; x < MAX_AURAS; ++x)
	{
		if(m_uint32Values[UNIT_FIELD_AURA+x] == spellid)
		{
			// Decrease count of this aura.
			int diff=count - GetAuraApplications(spellid);
			ModAuraStackCount(x, diff);
		}
	}*/
}

uint32 Unit::GetAuraApplications(uint32 spellid)
{
	return 0;
	/*for(uint32 x = 0; x < MAX_AURAS; ++x)
	{
		if(m_uint32Values[UNIT_FIELD_AURA+x] == spellid)
		{
			return m_auraStackCount[x];
		}
	}
	return 0;*/
}

uint32 Unit::ModAuraStackCount(uint32 slot, int32 count)
{
	return 0;
	/*uint32 index = (slot >> 2);
	uint32 byte  = (slot % 4);
	uint32 val   = m_uint32Values[UNIT_FIELD_AURAAPPLICATIONS+index];

	// shouldn't happen
	uint32 ac;

	if(count < 0 && m_auraStackCount[slot] < abs(count))
	{
		m_auraStackCount[slot] = ac = 0;
	}
	else
	{
		m_auraStackCount[slot] += count;
		ac = (m_auraStackCount[slot] > 0) ? m_auraStackCount[slot] - 1 : 0;
	}

	val &= ~(0xFF << byte * 8);
	val |= (ac << byte * 8);

	SetUInt32Value(UNIT_FIELD_AURAAPPLICATIONS+index, val);

	if (m_auraStackCount[slot]==0)
	{
		// Aura has been removed completely.
		uint8 flagslot = (slot/4);
		uint32 value = GetUInt32Value((uint16)(UNIT_FIELD_AURAFLAGS + flagslot));
		uint8 aurapos = (slot%4)*8;
		value &= ~(0xFF<<aurapos);
		SetUInt32Value(UNIT_FIELD_AURAFLAGS + flagslot,value);
		SetUInt32Value(UNIT_FIELD_AURA + index, 0);
		SetAuraSlotLevel(index, false);
	}	

	return m_auraStackCount[slot];*/
}

void Unit::RemoveAurasOfSchool(uint32 School, bool Positive, bool Immune)
{
	for(uint32 x = 0; x < MAX_AURAS; ++x)
	{
		if(m_auras[x]  && m_auras[x]->GetSpellProto()->School == School && (!m_auras[x]->IsPositive() || Positive) && (!Immune && m_auras[x]->GetSpellProto()->attributes & ATTRIBUTES_IGNORE_INVULNERABILITY))
			m_auras[x]->Remove();
	}
}

void Unit::EnableFlight()
{
	if(m_objectTypeId != TYPEID_PLAYER || ((Player*)this)->m_changingMaps)
	{
		WorldPacket data(SMSG_MOVE_SET_CAN_FLY, 13);
		data << GetNewGUID();
		data << uint32(2);
		SendMessageToSet(&data, true);

		if( IsPlayer() )
		{
			static_cast< Player* >( this )->m_setflycheat = true;
		}
	}
	else
	{
		WorldPacket * data = new WorldPacket(SMSG_MOVE_SET_CAN_FLY, 13);
		*data << GetNewGUID();
		*data << uint32(2);
		SendMessageToSet(data, false);
		static_cast< Player* >( this )->z_axisposition = 0.0f;
		static_cast< Player* >( this )->delayedPackets.add( data );
		static_cast< Player* >( this )->m_setflycheat = true;
	}
}

void Unit::DisableFlight()
{
	if(m_objectTypeId != TYPEID_PLAYER || ((Player*)this)->m_changingMaps)
	{
		WorldPacket data(SMSG_MOVE_UNSET_CAN_FLY, 13);
		data << GetNewGUID();
		data << uint32(5);
		SendMessageToSet(&data, true);

		if( IsPlayer() )
			static_cast< Player* >( this )->m_setflycheat = false;
	}
	else
	{
		WorldPacket * data = new WorldPacket( SMSG_MOVE_UNSET_CAN_FLY, 13 );
		*data << GetNewGUID();
		*data << uint32(5);
		SendMessageToSet(data, false);
		static_cast< Player* >( this )->z_axisposition = 0.0f;
		static_cast< Player* >( this )->delayedPackets.add( data );
		static_cast< Player* >( this )->m_setflycheat = false;
	}
}

void Unit::UpdateVisibility()
{
	ByteBuffer buf(2500);
	InRangeSet::iterator itr, it3;
	uint32 count;
	bool can_see;
	bool is_visible;
	Player *pl;
	Object * pObj;
	Player * plr;

	if( m_objectTypeId == TYPEID_PLAYER )
	{
		plr = static_cast< Player* >( this );
		for( Object::InRangeSet::iterator itr = m_objectsInRange.begin(); itr != m_objectsInRange.end();)
		{
			pObj = (*itr);
			++itr;

			can_see = plr->CanSee(pObj);
			is_visible = plr->GetVisibility(pObj, &it3);
			if(can_see)
			{
				if(!is_visible)
				{
					buf.clear();
					count = pObj->BuildCreateUpdateBlockForPlayer( &buf, plr );
					plr->PushCreationData(&buf, count);
					plr->AddVisibleObject(pObj);
				}
			}
			else
			{
				if(is_visible)
				{
					pObj->DestroyForPlayer(plr);
					plr->RemoveVisibleObject(it3);
				}
			}

			if( pObj->GetTypeId() == TYPEID_PLAYER )
			{
				pl = static_cast< Player* >( pObj );
				can_see = pl->CanSee( plr );
				is_visible = pl->GetVisibility( plr, &it3 );
				if( can_see )
				{
					if(!is_visible)
					{
						buf.clear();
						count = plr->BuildCreateUpdateBlockForPlayer( &buf, pl );
						pl->PushCreationData(&buf, count);
						pl->AddVisibleObject(plr);
					}
				}
				else
				{
					if(is_visible)
					{
						plr->DestroyForPlayer(pl);
						pl->RemoveVisibleObject(it3);
					}
				}
			}
		}
	}
	else			// For units we can save a lot of work
	{
		for(set<Player*>::iterator it2 = GetInRangePlayerSetBegin(); it2 != GetInRangePlayerSetEnd(); ++it2)
		{
			can_see = (*it2)->CanSee(this);
			is_visible = (*it2)->GetVisibility(this, &itr);
			if(!can_see)
			{
				if(is_visible)
				{
					DestroyForPlayer(*it2);
					(*it2)->RemoveVisibleObject(itr);
				}
			}
			else
			{
				if(!is_visible)
				{
					buf.clear();
					count = BuildCreateUpdateBlockForPlayer(&buf, *it2);
					(*it2)->PushCreationData(&buf, count);
					(*it2)->AddVisibleObject(this);
				}
			}
		}
	}
}

void Unit::RemoveSoloAura(uint32 type)
{
	switch(type)
	{
		case 1:// Polymorph
		{
			if(!polySpell) return;
			if(HasActiveAura(polySpell))
				RemoveAura(polySpell);
		}break;
/*		case 2:// Sap
		{
			if(!sapSpell) return;
			if(HasActiveAura(sapSpell))
				RemoveAura(sapSpell);
		}break;
		case 3:// Fear (Warlock)
		{
			if(!fearSpell) return;
			if(HasActiveAura(fearSpell))
				RemoveAura(fearSpell);
		}break;*/
		default:
			{
			sLog.outDebug("Warning: we are trying to remove a soloauratype that has no handle");
			}break;
	}
}

void Unit::EventHealthChangeSinceLastUpdate()
{
	int pct = GetHealthPct();

	uint32 toSet = 0;
	uint32 toRemove = 0;
	if( pct <= 35 && !HasFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_HEALTH35) )
		toSet |= AURASTATE_FLAG_HEALTH35;
	else if( pct > 35 && HasFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_HEALTH35) )
		toRemove |= AURASTATE_FLAG_HEALTH35;

	if( pct <= 20 && !HasFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_HEALTH20) )
		toSet |= AURASTATE_FLAG_HEALTH20;
	else if(pct > 20 && HasFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_HEALTH20))
		toRemove |= AURASTATE_FLAG_HEALTH20;

	if( pct >= 75 && !HasFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_HEALTHABOVE75) )
		toSet |= AURASTATE_FLAG_HEALTHABOVE75;
	else if(pct < 75 && HasFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_HEALTHABOVE75))
		toRemove |= AURASTATE_FLAG_HEALTHABOVE75;

	if(toSet)
		SetFlag(UNIT_FIELD_AURASTATE, toSet);

	if(toRemove)
		RemoveFlag(UNIT_FIELD_AURASTATE, toRemove);
}

int32 Unit::GetAP()
{
    int32 baseap = GetUInt32Value(UNIT_FIELD_ATTACK_POWER) + GetUInt32Value(UNIT_FIELD_ATTACK_POWER_MODS);
    float totalap = float(baseap)*(GetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER)+1);
	if(totalap>=0)
		return float2int32(totalap);
	return	0;
}

int32 Unit::GetRAP()
{
    int32 baseap = GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER) + GetUInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS);
    float totalap = float(baseap)*(GetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER)+1);
	if(totalap>=0)
		return float2int32(totalap);
	return	0;
}

bool Unit::GetSpeedDecrease()
{
	float before = m_speedModifier;
	m_speedModifier -= m_slowdown;
	m_slowdown = 0;
	uint32 spellId = 0;
	map< uint32, float >::iterator itr = speedReductionMap.begin(), itr2, saveditr;
	for(; itr != speedReductionMap.end(); ++itr)
	{
		if (itr->second < m_slowdown)
		{
			m_slowdown = itr->second;
			spellId = itr->first;
			saveditr = itr;
		}
	}

	//now apply slow/speed from this spell Id (for scripted effects)
	for (itr2=speedReductionMap.find(spellId); itr2!=speedReductionMap.upper_bound(spellId); ++itr2)
	{
		if (saveditr != itr2)
			m_slowdown += itr->second;
	}

	if(m_slowdown < -100)
		m_slowdown = -100; //do not walk backwards !

	m_speedModifier += m_slowdown;
	//save bandwidth :P
	if(m_speedModifier!=before)
		return true;
	return false;
}

void Unit::EventCastSpell(Unit * Target, SpellEntry * Sp)
{
	Spell * pSpell = new Spell(Target, Sp, true, NULL);
	SpellCastTargets targets(Target);
	pSpell->prepare(&targets);
}

void Unit::SetFacing(float newo)
{
	SetOrientation(newo);
	/*WorldPacket data(40);
	data.SetOpcode(MSG_MOVE_SET_FACING);
	data << GetNewGUID();
	data << (uint32)0; //flags
	data << (uint32)0; //time
	data << GetPositionX() << GetPositionY() << GetPositionZ() << newo;
	data << (uint32)0; //unk
	SendMessageToSet( &data, false );*/

	/*WorldPacket data(SMSG_MONSTER_MOVE, 60);
	data << GetNewGUID();
	data << m_position << getMSTime();
	data << uint8(4) << newo;
	data << uint32(0x00000000);		// flags
	data << uint32(0);				// time
	data << m_position;				// position
	SendMessageToSet(&data, true);*/
	m_aiInterface->SendMoveToPacket(m_position.x,m_position.y,m_position.z,m_position.o,1,MONSTER_MOVE_FLAG_RUN); // MoveFlags = 0x100 (run)
}

//guardians are temporary spawn that will inherit master faction and will folow them. Apart from that they have their own mind
Unit* Unit::create_guardian(uint32 guardian_entry,uint32 duration,float angle, uint32 lvl)
{
	CreatureProto * proto = CreatureProtoStorage.LookupEntry(guardian_entry);
	CreatureInfo * info = CreatureNameStorage.LookupEntry(guardian_entry);
	if(!proto || !info)
	{
		sLog.outError("Warning : Missing summon creature template %u !", guardian_entry);
		return NULL;
	}
	float m_fallowAngle=angle;
	float x = GetPositionX()+(3*(cosf(m_fallowAngle+GetOrientation())));
	float y = GetPositionY()+(3*(sinf(m_fallowAngle+GetOrientation())));
	float z = GetPositionZ();

	Creature* p = NULL;
	p = GetMapMgr()->CreateCreature(guardian_entry);
	if(p == NULL)
		return NULL;

	p->SetInstanceID(GetMapMgr()->GetInstanceID());
	p->Load(proto, x, y, z,GetOrientation());

	if (lvl != 0)
	{
		/* MANA */
		p->SetPowerType(POWER_TYPE_MANA);
		p->SetUInt32Value(UNIT_FIELD_MAXPOWER1,p->GetUInt32Value(UNIT_FIELD_MAXPOWER1)+28+10*lvl);
		p->SetUInt32Value(UNIT_FIELD_POWER1,p->GetUInt32Value(UNIT_FIELD_POWER1)+28+10*lvl);
		/* HEALTH */
		p->SetUInt32Value(UNIT_FIELD_MAXHEALTH,p->GetUInt32Value(UNIT_FIELD_MAXHEALTH)+28+30*lvl);
		p->SetUInt32Value(UNIT_FIELD_HEALTH,p->GetUInt32Value(UNIT_FIELD_HEALTH)+28+30*lvl);
		/* LEVEL */
		p->ModUnsigned32Value(UNIT_FIELD_LEVEL, lvl);
	}

	p->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, GetGUID());
    p->SetUInt64Value(UNIT_FIELD_CREATEDBY, GetGUID());
    p->SetZoneId(GetZoneId());
	p->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	p->_setFaction();

	p->GetAIInterface()->Init(p,AITYPE_PET,MOVEMENTTYPE_NONE,this);
	p->GetAIInterface()->SetUnitToFollow(this);
	p->GetAIInterface()->SetUnitToFollowAngle(m_fallowAngle);
	p->GetAIInterface()->SetFollowDistance(3.0f);

	p->PushToWorld(GetMapMgr());

	if(duration)
		sEventMgr.AddEvent(p, &Creature::SummonExpire, EVENT_SUMMON_EXPIRE, duration, 1,0);

	m_summonedCreatures.insert(p);

	return p;//lol, will compiler make the pointer conversion ?

}

float Unit::get_chance_to_daze(Unit *target)
{
//	if(GetTypeId()!=TYPEID_UNIT)
//		return 0.0f;
	float attack_skill = float( getLevel() ) * 5.0f;
	float defense_skill = 0.0f;
	if( target->IsPlayer() )
		defense_skill = float( static_cast< Player* >( target )->_GetSkillLineCurrent( SKILL_DEFENSE, false ) );
	else
		defense_skill = float( target->getLevel() * 5 );
	if( defense_skill == 0.0f )
		defense_skill = 1;
	float chance_to_daze = attack_skill * 20 / defense_skill;//if level is equal then we get a 20% chance to daze
	chance_to_daze = chance_to_daze * std::min(target->getLevel() / 30.0f, 1.0f );//for targets below level 30 the chance decreses
	if( chance_to_daze > 40 )
		return 40.0f;
	else
		return chance_to_daze;
}

void CombatStatusHandler::ClearMyHealers()
{
	// this is where we check all our healers
	HealedSet::iterator i;
	Player * pt;
	for(i = m_healers.begin(); i != m_healers.end(); ++i)
	{
		pt = m_Unit->GetMapMgr()->GetPlayer(*i);
		if(pt != NULL)
			pt->CombatStatus.RemoveHealed(m_Unit);
	}

	m_healers.clear();
}

void CombatStatusHandler::WeHealed(Unit * pHealTarget)
{
	if(pHealTarget->GetTypeId() != TYPEID_PLAYER || m_Unit->GetTypeId() != TYPEID_PLAYER || pHealTarget == m_Unit)
		return;

	if(pHealTarget->CombatStatus.IsInCombat())
	{
		m_healed.insert(pHealTarget->GetLowGUID());
		pHealTarget->CombatStatus.m_healers.insert(m_Unit->GetLowGUID());
	}

	UpdateFlag();
}

void CombatStatusHandler::RemoveHealed(Unit * pHealTarget)
{
	m_healed.erase(pHealTarget->GetLowGUID());
	UpdateFlag();
}

void CombatStatusHandler::UpdateFlag()
{
	bool n_status = InternalIsInCombat();
	if(n_status != m_lastStatus)
	{
		m_lastStatus = n_status;
		if(n_status)
		{
			//Log.Warning("CombatStatusHandler::UpdateFlag","est en combat");
			if (m_Unit != NULL)
			{
				m_Unit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_COMBAT);
				if(!m_Unit->hasStateFlag(UF_ATTACKING)) m_Unit->addStateFlag(UF_ATTACKING);
			}
		}
		else
		{
			//Log.Warning("CombatStatusHandler::UpdateFlag","n'est pas en combat", m_Unit->GetGUID());
			if (m_Unit != NULL)
			{
				m_Unit->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_COMBAT);
				if(m_Unit->hasStateFlag(UF_ATTACKING)) m_Unit->clearStateFlag(UF_ATTACKING);
			}

			// remove any of our healers from combat too, if they are able to be.
			ClearMyHealers();
		}
	}
}

bool CombatStatusHandler::InternalIsInCombat()
{
	if(m_healed.size() > 0)
		return true;

	if(m_attackTargets.size() > 0)
		return true;

	if(m_attackers.size() > 0)
		return true;

	return false;
}

void CombatStatusHandler::AddAttackTarget(const uint64& guid)
{
	if(guid == m_Unit->GetGUID())
		return;

	m_attackTargets.insert(guid);
	//printf("Adding attack target "I64FMT" to "I64FMT"\n", guid, m_Unit->GetGUID());
	if(m_Unit->GetTypeId() == TYPEID_PLAYER &&
		m_primaryAttackTarget != guid)			// players can only have one attack target.
	{
		if(m_primaryAttackTarget)
			ClearPrimaryAttackTarget();

		m_primaryAttackTarget = guid;
	}

	UpdateFlag();
}

void CombatStatusHandler::ClearPrimaryAttackTarget()
{
	//printf("ClearPrimaryAttackTarget for "I64FMT"\n", m_Unit->GetGUID());
	if(m_primaryAttackTarget != 0)
	{
		Unit * pt = m_Unit->GetMapMgr()->GetUnit(m_primaryAttackTarget);
		if(pt != NULL)
		{
			// remove from their attacker set. (if we have no longer got any DoT's, etc)
			if(!IsAttacking(pt))
			{
				pt->CombatStatus.RemoveAttacker(m_Unit, m_Unit->GetGUID());
				m_attackTargets.erase(m_primaryAttackTarget);
			}
			
			m_primaryAttackTarget = 0;
		}
		else
		{
			m_attackTargets.erase(m_primaryAttackTarget);
			m_primaryAttackTarget = 0;
		}
	}

	UpdateFlag();
}

bool CombatStatusHandler::IsAttacking(Unit * pTarget)
{
	// dead targets - no combat
	if( pTarget == NULL || m_Unit->isDead() || pTarget->isDead() )
		return false;

	// check the target for any of our DoT's.
	for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; ++i)
	{
		if(pTarget->m_auras[i] != NULL)
		{
			if(m_Unit->GetGUID() == pTarget->m_auras[i]->m_casterGuid && pTarget->m_auras[i]->IsCombatStateAffecting())
				return true;
		}
	}

	// place any additional checks here
	return false;
}

void CombatStatusHandler::RemoveAttackTarget(Unit * pTarget)
{
	// called on aura remove, etc.
	AttackerMap::iterator itr = m_attackTargets.find(pTarget->GetGUID());
	if(itr == m_attackTargets.end())
		return;

	if(!IsAttacking(pTarget))
	{
		//printf("Removing attack target "I64FMT" on "I64FMT"\n", pTarget->GetGUID(), m_Unit->GetGUID());
		m_attackTargets.erase(itr);
		if(m_primaryAttackTarget == pTarget->GetGUID())
			m_primaryAttackTarget = 0;

		UpdateFlag();
	}
	/*else
		printf("Cannot remove attack target "I64FMT" from "I64FMT"\n", pTarget->GetGUID(), m_Unit->GetGUID());*/
}

void CombatStatusHandler::RemoveAttacker(Unit * pAttacker, const uint64& guid)
{
	AttackerMap::iterator itr = m_attackers.find(guid);
	if(itr == m_attackers.end())
		return;

	if( (!pAttacker) || (!pAttacker->CombatStatus.IsAttacking(m_Unit)) )
	{
		//printf("Removing attacker "I64FMT" from "I64FMT"\n", guid, m_Unit->GetGUID());
		m_attackers.erase(itr);
		UpdateFlag();
	}
	/*else
	{
		printf("Cannot remove attacker "I64FMT" from "I64FMT"\n", guid, m_Unit->GetGUID());
	}*/
}

void CombatStatusHandler::OnDamageDealt(Unit * pTarget)
{
	// we added an aura, or dealt some damage to a target. they need to have us as an attacker, and they need to be our attack target if not.
	//printf("OnDamageDealt to "I64FMT" from "I64FMT"\n", pTarget->GetGUID(), m_Unit->GetGUID());
	if(pTarget == m_Unit)
		return;

	if(!pTarget->isAlive())
		return;

	AttackerMap::iterator itr = m_attackTargets.find(pTarget->GetGUID());
	if(itr == m_attackTargets.end())
		AddAttackTarget(pTarget->GetGUID());

	itr = pTarget->CombatStatus.m_attackers.find(m_Unit->GetGUID());
	if(itr == pTarget->CombatStatus.m_attackers.end())
		pTarget->CombatStatus.AddAttacker(m_Unit->GetGUID());

	// update the timeout
	m_Unit->CombatStatusHandler_ResetPvPTimeout();
}

void CombatStatusHandler::AddAttacker(const uint64& guid)
{
	m_attackers.insert(guid);
	UpdateFlag();
}

void CombatStatusHandler::ClearAttackers()
{
	// this is a FORCED function, only use when the reference will be destroyed.
	AttackerMap::iterator itr = m_attackTargets.begin();
	Unit * pt;
	for(; itr != m_attackTargets.end(); ++itr)
	{
		pt = m_Unit->GetMapMgr()->GetUnit(*itr);
		if(pt)
		{
			pt->CombatStatus.m_attackers.erase(m_Unit->GetGUID());
			pt->CombatStatus.UpdateFlag();
		}
	}

	for(itr = m_attackers.begin(); itr != m_attackers.end(); ++itr)
	{
		pt = m_Unit->GetMapMgr()->GetUnit(*itr);
		if(pt)
		{
			pt->CombatStatus.m_attackTargets.erase(m_Unit->GetGUID());
			pt->CombatStatus.UpdateFlag();
		}
	}

	m_attackers.clear();
	m_attackTargets.clear();
	m_primaryAttackTarget = 0;
	UpdateFlag();
}

void CombatStatusHandler::ClearHealers()
{
	HealedSet::iterator itr = m_healed.begin();
	Player * pt;
	for(; itr != m_healed.end(); ++itr)
	{
		pt = m_Unit->GetMapMgr()->GetPlayer(*itr);
		if(pt)
		{
			pt->CombatStatus.m_healers.erase(m_Unit->GetLowGUID());
			pt->CombatStatus.UpdateFlag();
		}
	}

	for(itr = m_healers.begin(); itr != m_healers.end(); ++itr)
	{
		pt = m_Unit->GetMapMgr()->GetPlayer(*itr);
		if(pt)
		{
			pt->CombatStatus.m_healed.erase(m_Unit->GetLowGUID());
			pt->CombatStatus.UpdateFlag();
		}
	}

	m_healed.clear();
	m_healers.clear();
	UpdateFlag();
}

void CombatStatusHandler::OnRemoveFromWorld()
{
	ClearAttackers();
	ClearHealers();
	DamageMap.clear();
}

void Unit::CombatStatusHandler_ResetPvPTimeout()
{
	if(!IsPlayer())
		return;

	m_lock.Acquire();
	EventMap::iterator itr = m_events.find(EVENT_ATTACK_TIMEOUT);
	if(itr != m_events.end())
	{
		for(; itr != m_events.upper_bound(EVENT_ATTACK_TIMEOUT); ++itr)
		{
			if(!itr->second->deleted)
			{
				itr->second->currTime = 5000;
				m_lock.Release();
				return;
			}
		}
	}
	// TESTING GROUND TESTING
	//this->SendChatMessage( CHAT_MSG_MONSTER_SAY , LANG_UNIVERSAL , "Eye of C'thun AI Initialised" );
	//this->SendChatMessage(
	//	this->Emote( EMOTE_ONESHOT_SPELLCAST ); 
	// yay for recursive mutexes
	sEventMgr.AddEvent(this, &Unit::CombatStatusHandler_UpdatePvPTimeout, EVENT_ATTACK_TIMEOUT, 5000, 1, 0);
	m_lock.Release();
}

void Unit::CombatStatusHandler_UpdatePvPTimeout()
{
	CombatStatus.TryToClearAttackTargets();	
}

void Unit::Heal(Unit *target, uint32 SpellId, uint32 amount)
{//Static heal
	if(!target || !SpellId || !amount || !target->isAlive() )
		return;
	
	uint32 overheal = 0;
	uint32 ch=target->GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 mh=target->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	if(mh!=ch)
	{
		ch += amount;
		if(ch > mh)
		{
			target->SetUInt32Value(UNIT_FIELD_HEALTH, mh);
			amount += mh-ch;
		}
		else 
			target->SetUInt32Value(UNIT_FIELD_HEALTH, ch);

		Spell::SendHealSpellOnPlayer(this, target, amount, false, overheal, SpellId);

		/*WorldPacket data(SMSG_SPELLHEALLOG,25);
		data << target->GetNewGUID();
		data << this->GetNewGUID();
		data << uint32(SpellId);  
		data << uint32(amount);   
		data << uint8(0);
		this->SendMessageToSet(&data,true);*/
	}
}
void Unit::Energize(Unit* target,uint32 SpellId, uint32 amount,uint32 type)
{//Static energize
	if(!target || !SpellId || !amount)
		return;
	uint32 cm=target->GetUInt32Value(UNIT_FIELD_POWER1+type);
	uint32 mm=target->GetUInt32Value(UNIT_FIELD_MAXPOWER1+type);
	if(mm!=cm)
	{
		cm += amount;
		if(cm > mm)
		{
			target->SetUInt32Value(UNIT_FIELD_POWER1+type, mm);
			amount += mm-cm;
		}
		else 
			target->SetUInt32Value(UNIT_FIELD_POWER1+type, cm);

		WorldPacket datamr(SMSG_SPELLENERGIZELOG, 30);
		datamr << target->GetNewGUID();
		datamr << this->GetNewGUID();
		datamr << uint32(SpellId);
		datamr << uint32(0);
		datamr << uint32(amount);
		this->SendMessageToSet(&datamr,true);
	}
}

void Unit::InheritSMMods(Unit* inherited_from)
{
	m_modifiersInherited = true;
	SM_CriticalChance = inherited_from->SM_CriticalChance;
	SM_FDur = inherited_from->SM_FDur;
	SM_PDur = inherited_from->SM_PDur;
	SM_PRadius = inherited_from->SM_PRadius;
	SM_FRadius = inherited_from->SM_FRadius;
	SM_PRange = inherited_from->SM_PRange;
	SM_FRange = inherited_from->SM_FRange;
	SM_PCastTime = inherited_from->SM_PCastTime;
	SM_FCastTime = inherited_from->SM_FCastTime;
	SM_PCriticalDamage = inherited_from->SM_PCriticalDamage;
	SM_PDOT = inherited_from->SM_PDOT;
	SM_FDOT = inherited_from->SM_FDOT;
	SM_FEffectBonus = inherited_from->SM_FEffectBonus;
	SM_PEffectBonus = inherited_from->SM_PEffectBonus;
	SM_FDamageBonus = inherited_from->SM_PDamageBonus;
	SM_PDamageBonus = inherited_from->SM_PDamageBonus;
	SM_PSPELL_VALUE = inherited_from->SM_PSPELL_VALUE;
	SM_FSPELL_VALUE = inherited_from->SM_FSPELL_VALUE;
	SM_FHitchance = inherited_from->SM_FHitchance;
	SM_PAPBonus = inherited_from->SM_PAPBonus;
	SM_PCost = inherited_from->SM_PCost;
	SM_FCost = inherited_from->SM_FCost;
	SM_PNonInterrupt = inherited_from->SM_PNonInterrupt;
	SM_PJumpReduce = inherited_from->SM_PJumpReduce;
	SM_FSpeedMod = inherited_from->SM_FSpeedMod;
	SM_FAdditionalTargets = inherited_from->SM_FAdditionalTargets;
	SM_FPenalty = inherited_from->SM_FPenalty;
	SM_PPenalty = inherited_from->SM_PPenalty;
	SM_PCooldownTime = inherited_from->SM_PCooldownTime;
	SM_FCooldownTime = inherited_from->SM_FCooldownTime;
	SM_FChanceOfSuccess = inherited_from->SM_FChanceOfSuccess;
	SM_FAmptitude = inherited_from->SM_FAmptitude;
	SM_PAmptitude = inherited_from->SM_PAmptitude;
	SM_FRezist_dispell = inherited_from->SM_FRezist_dispell;
	SM_PRezist_dispell = inherited_from->SM_PRezist_dispell;
	SM_PThreat_Reduction = inherited_from->SM_PThreat_Reduction;
	SM_FThreat_Reduction = inherited_from->SM_PThreat_Reduction;
	SM_FEffect1_Bonus = inherited_from->SM_FEffect1_Bonus;
	SM_FEffect2_Bonus = inherited_from->SM_FEffect2_Bonus;
	SM_FEffect3_Bonus = inherited_from->SM_FEffect3_Bonus;
	SM_PEffect1_Bonus = inherited_from->SM_PEffect1_Bonus;
	SM_PEffect2_Bonus = inherited_from->SM_PEffect2_Bonus;
	SM_PEffect3_Bonus = inherited_from->SM_PEffect3_Bonus;
	SM_FCharge_Bonus = inherited_from->SM_FCharge_Bonus;
	SM_PCharge_Bonus = inherited_from->SM_PCharge_Bonus;
}

void CombatStatusHandler::TryToClearAttackTargets()
{
	AttackerMap::iterator i, i2;
	Unit * pt;

	for(i = m_attackTargets.begin(); i != m_attackTargets.end();)
	{
		i2 = i++;
		pt = m_Unit->GetMapMgr()->GetUnit(*i2);
		if(pt == NULL)
		{
			m_attackTargets.erase(i2);
			continue;
		}

		RemoveAttackTarget(pt);
		pt->CombatStatus.RemoveAttacker(m_Unit,m_Unit->GetGUID());
	}
}

void CombatStatusHandler::AttackersForgetHate()
{
	AttackerMap::iterator i, i2;
	Unit * pt;

	for(i = m_attackTargets.begin(); i != m_attackTargets.end();)
	{
		i2 = i++;
		pt = m_Unit->GetMapMgr()->GetUnit(*i2);
		if(pt == NULL)
		{
			m_attackTargets.erase(i2);
			continue;
		}

		if(pt->GetAIInterface())
			pt->GetAIInterface()->RemoveThreatByPtr(m_Unit);
	}
}

void Unit::CancelSpell(Spell * ptr)
{
	if(ptr != NULL)
		ptr->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
	else if(m_currentSpell != NULL)
		m_currentSpell->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
}

void Unit::EventStrikeWithAbility(uint64 guid, SpellEntry * sp, uint32 damage)
{
	if (!IsInWorld())
		return;
	Unit *victim = GetMapMgr()->GetUnit(guid);
	if(victim)
		Strike(victim, RANGED, sp, 0, 0, 0, false, true);
}

void Unit::DispelAll(bool positive)
{
	for(uint32 i = 0; i < MAX_AURAS; ++i)
	{
		if(m_auras[i]!=NULL)
			if((m_auras[i]->IsPositive()&&positive)||!m_auras[i]->IsPositive())
				m_auras[i]->Remove();
	}
}

/* bool Unit::RemoveAllAurasByMechanic (renamed from MechanicImmunityMassDispel)
- Removes all auras on this unit that are of a specific mechanic.
- Useful for things like.. Apply Aura: Immune Mechanic, where existing (de)buffs are *always supposed* to be removed.
- I'm not sure if this goes here under unit.
* Arguments:
	- uint32 MechanicType
		*

* Returns;
	- False if no buffs were dispelled, true if more than 0 were dispelled.
*/
bool Unit::RemoveAllAurasByMechanic( uint32 MechanicType , uint32 MaxDispel = -1 , bool HostileOnly = true )
{
	//sLog.outString( "Unit::MechanicImmunityMassDispel called, mechanic: %u" , MechanicType );
	uint32 DispelCount = 0;
	for(uint32 x = ( HostileOnly ? MAX_POSITIVE_AURAS : 0 ) ; x < MAX_AURAS ; x++ ) // If HostileOnly = 1, then we use aura slots 40-56 (hostile). Otherwise, we use 0-56 (all)
		{
			if( DispelCount >= MaxDispel && MaxDispel > 0 )
			return true;

			if( m_auras[x] )
			{
					if( m_auras[x]->GetSpellProto()->mechanics == MechanicType ) // Remove all mechanics of type MechanicType (my english goen boom)
					{
						m_auras[x]->Remove(); // EZ-Remove
						DispelCount ++;
					}
					else if( MechanicType == MECHANIC_DECELERATION ) // if got immunity for slow, remove some that are not in the mechanics
					{
						for( int i=0 ; i<3 ; i++ )
						{
							// SNARE + ROOT
							if( m_auras[x]->GetSpellProto()->EffectApplyAuraName[i] == SPELL_AURA_MOD_DECREASE_SPEED || 
								m_auras[x]->GetSpellProto()->EffectApplyAuraName[i] == SPELL_AURA_MOD_ROOT )
							{
								m_auras[x]->Remove();
								break;
							}
						}
					}
			}
		}
	return ( DispelCount == 0 );
}

void Unit::setAttackTimer(int32 time, bool offhand)
{
	if(!time)
		time = offhand ? m_uint32Values[UNIT_FIELD_RANGEDATTACKTIME] : m_uint32Values[UNIT_FIELD_BASEATTACKTIME];

	time = std::max(1000,float2int32(float(time)*GetFloatValue(UNIT_MOD_CAST_SPEED)));
	if(time>300000)		// just in case.. shouldn't happen though
		time=offhand ? m_uint32Values[UNIT_FIELD_RANGEDATTACKTIME] : m_uint32Values[UNIT_FIELD_BASEATTACKTIME];

	if(offhand)
		m_attackTimer_1 = getMSTime() + time;
	else
		m_attackTimer = getMSTime() + time;
}

bool Unit::isAttackReady(bool offhand)
{
	if(offhand)
		return (getMSTime() >= m_attackTimer_1) ? true : false;
	else
		return (getMSTime() >= m_attackTimer) ? true : false;
}

void Unit::ReplaceAIInterface(AIInterface *new_interface) 
{ 
	delete m_aiInterface;	//be carefull when you do this. Might screw unit states !
	m_aiInterface = new_interface; 
}

void Unit::EventUpdateFlag()  
{  
static_cast< Player * >( this )->CombatStatus.UpdateFlag(); 
}

bool Unit::HasAurasOfNameHashWithCaster(uint32 namehash, Unit * caster)
{
	if( !namehash )
		return false;

	if(caster!=NULL) 
	{ 
		for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; ++i) 
			if( m_auras[i] != NULL&& m_auras[i]->GetSpellProto()->NameHash == namehash && m_auras[i]->GetCasterGUID() == caster->GetGUID() ) 
				return true; 
	} 
	else 
	{ 
		for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; ++i) 
			if( m_auras[i] != NULL&& m_auras[i]->GetSpellProto()->NameHash == namehash ) 
				return true; 
	} 

	return false;
}


bool Unit::InParty(Unit* u)
{
	Player* p=GetPlayerFrom();
	Player* uFrom=u->GetPlayerFrom();

	if (p == NULL || uFrom == NULL)
		return false;

	if (p == uFrom)
		return true;

	if (p->GetGroup() != NULL && uFrom->GetGroup() != NULL && p->GetGroup() == uFrom->GetGroup() && p->GetSubGroup() == uFrom->GetSubGroup())
		return true;

	return false;
}

bool Unit::InRaid(Unit* u)
{
	Player *p=GetPlayerFrom();
	Player *uFrom=u->GetPlayerFrom();

	if (p == NULL || uFrom == NULL)
		return false;

	if (p == uFrom)
		return true;

	if (p->GetGroup() != NULL && uFrom->GetGroup() != NULL && p->GetGroup() == uFrom->GetGroup())
		return true;

	return false;
}

//what is an Immobilize spell ? Have to add it later to spell effect handler
void Unit::EventStunOrImmobilize()
{
	if( trigger_on_stun )
	{
		if( trigger_on_stun_chance < 100 && !Rand( trigger_on_stun_chance ) )
			return;

		CastSpell(static_cast<Unit *>(this), trigger_on_stun, true);
	}
}

// Proc on chill effects (such as frostbolt slow effect)
void Unit::EventChill(Unit *proc_target)
{
	if ( !proc_target || static_cast<Unit *>(this) == proc_target )
		return; //how and why would we chill ourselfs

	if( trigger_on_chill )
	{
		if( trigger_on_chill_chance < 100 && !Rand( trigger_on_chill_chance ) )
			return;

		CastSpell(proc_target, trigger_on_chill, true);
	}
}

void Unit::SetTriggerStunOrImmobilize(uint32 newtrigger,uint32 new_chance)
{
	trigger_on_stun = newtrigger;
	trigger_on_stun_chance = new_chance;
}

void Unit::SetTriggerChill(uint32 newtrigger,uint32 new_chance)
{
	trigger_on_chill = newtrigger;
	trigger_on_chill_chance = new_chance;
}

void Unit::SendPowerUpdate()
{
	//uint32 amount = GetUInt32Value(UNIT_FIELD_POWER1 + GetPowerType()); //save the amount, so we send the same to the player and everyone else

	//Log.Warning("Unit::SendPowerUpdate"," PowerType: %u, Amount: %u",(uint8)GetPowerType(),GetUInt32Value(UNIT_FIELD_POWER1 + GetPowerType()));

	WorldPacket data( SMSG_POWER_UPDATE, 14 );
	FastGUIDPack( data, GetGUID() );
	data << (uint8)GetPowerType();
	data << GetUInt32Value(UNIT_FIELD_POWER1 + GetPowerType()); // amount;
	SendMessageToSet( &data, GetTypeId() == TYPEID_PLAYER ? true : false );

	//VLack: On 3.1.3, create and send a field update packet to everyone else, as this is the only way to update their GUI with the power values.
	//WorldPacket * pkt=BuildFieldUpdatePacket(UNIT_FIELD_POWER1 + GetPowerType(), amount);
	//SendMessageToSet(pkt, false);
	//delete pkt;
	
	/*WorldPacket data(SMSG_POWER_UPDATE, 14);
	FastGUIDPack(data, GetGUID());
	data << (uint8)GetPowerType();
	data << GetUInt32Value(UNIT_FIELD_POWER1 + GetPowerType());
	SendMessageToSet(&data, GetTypeId() == TYPEID_PLAYER ? true : false);*/
}

void Unit::ShareHealthWithUnit(Unit *u)
{
	u->m_sharedHPMaster = this;
	m_sharedHPUnits.insert(u);
	m_sharedHPUnits.insert(this);

	//set the HP of the unit to our HP!!!
	u->SetUInt32Value(UNIT_FIELD_HEALTH, GetUInt32Value(UNIT_FIELD_HEALTH));
	u->SetUInt32Value(UNIT_FIELD_MAXHEALTH, GetUInt32Value(UNIT_FIELD_MAXHEALTH));
}

void UnitChain::AddUnit(Unit* u)
{
	m_units.insert(u);
	u->m_chain = this;
}

void UnitChain::RemoveUnit(Unit* u)
{
	m_units.erase(u);
	u->m_chain = NULL;

	if (m_units.size() == 0 && !m_persist)
		delete this;
}

void Unit::CalcStat(uint32 index)
{
	if (IsPlayer())
		static_cast<Player*>(this)->CalcStat(index);
	if (IsCreature())
		static_cast<Creature*>(this)->CalcStat(index);
}

void Unit::ModifyBonuses(uint32 type,int32 val)
{
	// Added some updateXXXX calls so when an item modifies a stat they get updated
	// also since this is used by auras now it will handle it for those
	switch (type) 
		{
		case POWER:
			ModUnsigned32Value( UNIT_FIELD_MAXPOWER1, val );
			if (IsPlayer()) static_cast<Player*>(this)->m_manafromitems += val;
			break;

		case HEALTH:
			ModUnsigned32Value( UNIT_FIELD_MAXHEALTH, val );
			if (IsPlayer()) static_cast<Player*>(this)->m_healthfromitems += val;
			break;

		case AGILITY: // modify agility				
			FlatStatMod[STAT_AGILITY] += val;
			CalcStat( STAT_AGILITY );
			break;

		case STRENGTH: //modify strength
			FlatStatMod[STAT_STRENGTH] += val;
			CalcStat( STAT_STRENGTH );
			break;

		case INTELLECT: //modify intellect 
			FlatStatMod[STAT_INTELLECT] += val;
			CalcStat( STAT_INTELLECT );
			break;

		 case SPIRIT: //modify spirit
			FlatStatMod[STAT_SPIRIT] += val;
			CalcStat( STAT_SPIRIT );
			break;

		case STAMINA: //modify stamina
			FlatStatMod[STAT_STAMINA] += val;
			CalcStat( STAT_STAMINA );
			break;

		case WEAPON_SKILL_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_SKILL, val ); // ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_MAIN_HAND_SKILL, val ); // melee main hand
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_OFF_HAND_SKILL, val ); // melee off hand
			}break;

		case DEFENSE_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_DEFENCE, val );
			}break;

		case DODGE_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_DODGE, val );
			}break;

		case PARRY_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_PARRY, val );
			}break;

		case SHIELD_BLOCK_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_BLOCK, val );
			}break;

		case MELEE_HIT_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT, val );
			}break;

		case RANGED_HIT_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT, val );
			}break;

		case SPELL_HIT_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT, val );
			}break;

		case MELEE_CRITICAL_STRIKE_RATING:
			{
				if (!IsPlayer())
					return;

				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_CRIT, val );
			}break;
		case RANGED_CRITICAL_STRIKE_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_CRIT, val );
			}break;

		case SPELL_CRITICAL_STRIKE_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_CRIT, val );
			}break;

		case MELEE_HIT_AVOIDANCE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT_AVOIDANCE, val );
			}break;
		case RANGED_HIT_AVOIDANCE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT_AVOIDANCE, val );
			}break;
		case SPELL_HIT_AVOIDANCE_RATING:
			{
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT_AVOIDANCE, val );

			}break;
		case MELEE_CRITICAL_AVOIDANCE_RATING:
			{

			}break;

		case RANGED_CRITICAL_AVOIDANCE_RATING:
			{

			}break;

		case SPELL_CRITICAL_AVOIDANCE_RATING:
			{

			}break;
		case MELEE_HASTE_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HASTE, val );//melee
			}break;

		case RANGED_HASTE_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HASTE, val );//ranged
			}break;

		case SPELL_HASTE_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HASTE, val );//spell
			}break;

		case HIT_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT, val );
			}break;

		case CRITICAL_STRIKE_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_CRIT, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_CRIT, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_CRIT, val );
			}break;

		case HIT_AVOIDANCE_RATING:// this is guessed based on layout of other fields
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HIT_AVOIDANCE, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HIT_AVOIDANCE, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HIT_AVOIDANCE, val );//spell
			}break;

		case EXPERTISE_RATING:
		case EXPERTISE_RATING_2:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_EXPERTISE, val );
				ModUnsigned32Value( PLAYER_EXPERTISE, val );
			}break;

		case RESILIENCE_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_CRIT_RESILIENCE, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_CRIT_RESILIENCE, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE, val );//spell
			}break;

		case HASTE_RATING:
			{
				if (!IsPlayer()) return;
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_MELEE_HASTE, val );//melee
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_RANGED_HASTE, val );//ranged
				ModUnsigned32Value( PLAYER_RATING_MODIFIER_SPELL_HASTE, val );//spell
				// maybe should do spell here? (19)
			}break;

		case ATTACK_POWER:
			{
				ModUnsigned32Value(UNIT_FIELD_ATTACK_POWER_MODS, val);
				CalcDamage();
			}break;

		case RANGED_ATTACK_POWER:
			{
				ModUnsigned32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS, val);
				CalcDamage();
			}break;

		case SPELL_DAMAGE_DONE:
			{
				 // de PLAYER_FIELD_MOD_DAMAGE_DONE_POS à PLAYER_FIELD_MOD_DAMAGE_DONE_POS_6
				for (uint32 i=0; i<7; ++i)
				{
					ModUnsigned32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + i, val);
				}
				CalcDamage();
			}break;

		case SPELL_HEALING_DONE:
			{
				/* de PLAYER_FIELD_MOD_DAMAGE_DONE_POS à PLAYER_FIELD_MOD_DAMAGE_DONE_POS_6
				//for (uint32 i=0; i<7; ++i) // Test, A voir (Branruz)
				//{
				//	ModUnsigned32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + i, val);
				//}
				ModUnsigned32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, val);
				CalcDamage();*/
				for(uint32 school=1;school < 7; ++school)
				{
					HealDoneMod[school] += val;
				}
				ModUnsigned32Value( PLAYER_FIELD_MOD_HEALING_DONE_POS, val );
			}break;

		case MANA_REGENERATION:
			{
				m_ModInterrMRegen[POWER_TYPE_MANA] += val;
			}break;

		case ARMOR_PENETRATION_RATING:
			{
				ModUnsigned32Value(PLAYER_RATING_MODIFIER_ARMOR_PENETRATION_RATING, val);
			}break;

		case SPELL_POWER:
			{
				// de PLAYER_FIELD_MOD_DAMAGE_DONE_POS à PLAYER_FIELD_MOD_DAMAGE_DONE_POS_6
				for (uint32 i=0; i<7; ++i)
				{
					ModUnsigned32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + i, val);
				}
				//ModUnsigned32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, val); // ??
				CalcDamage();
			};
			break;

		default: Log.Warning("[Unit::ModifyBonuses]","Type inconnu: %u, Val: %d",type,val);
			     break;
		}

    
	if(m_aurascast.size()) // Secu.... (Branruz)
	{
	 //recalc all auras
	 for (std::multimap<uint32, Aura*>::iterator itr=m_aurascast.begin(); itr!=m_aurascast.end(); ++itr)
	 {
		 if (!sEventMgr.HasEvent(itr->second, EVENT_AURA_UPDATE))
			sEventMgr.AddEvent(itr->second, &Aura::HandleChange, EVENT_AURA_UPDATE, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	 }

	 Pet* p=GetSummon();
	 if (p != NULL && p->isAlive() && p->IsInWorld())
	 {
		for (std::multimap<uint32, Aura*>::iterator itr=p->m_aurascast.begin(); itr!=p->m_aurascast.end(); ++itr)
		{
			 if (!sEventMgr.HasEvent(itr->second, EVENT_AURA_UPDATE))
				sEventMgr.AddEvent(itr->second, &Aura::HandleChange, EVENT_AURA_UPDATE, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
	 }
	}
}

//par sebbu #programmation reseau irc idapnet 1 jav 09
HM_NAMESPACE::hash_multimap<uint32, ProcFnc*>::iterator Unit::upper_bound( HM_NAMESPACE::hash_multimap<uint32, ProcFnc*> hmm, uint32 key) {
	HM_NAMESPACE::hash_multimap<uint32, ProcFnc*>::iterator it1;
	for(it1=hmm.begin(); it1!=hmm.end(); it1++) {
		if( it1->first > key ) break;
	}
	return it1;
}

void Unit::DeleteProcFnc(void* creator, uint32 proctype)
{
	HM_NAMESPACE::hash_multimap<uint32, ProcFnc*>::iterator itr1, itr2;
	if (proctype == 0xFFFFFFFF)
	{
		itr1 = m_procfuncs.begin();
		itr2 = m_procfuncs.end();
	}
	else
	{
		itr1 = m_procfuncs.find(proctype);

		//no entry
		if (itr1 == m_procfuncs.end())
			return;

		//par sebbu #programmation reseau irc idapnet 1 jav 09
		//itr2 = m_procfuncs.upper_bound(proctype);
		itr2 = upper_bound( m_procfuncs, proctype );

	}

	for (; itr1 != itr2; ++itr1)
	{
		if (itr1->second->m_creator == creator || creator == NULL)
			itr1->second->deleted = true;
	}
}

void Unit::HandleProcFnc(uint32 proctype, std::vector<void*>* data)
{
	HM_NAMESPACE::hash_multimap<uint32, ProcFnc*>::iterator itr1, itr2;
	itr1 = m_procfuncs.find(proctype);

	//no procs
	if (itr1 == m_procfuncs.end())
		return;

	//par sebbu #programmation reseau irc idapnet 1 jav 09
	//itr2 = m_procfuncs.upper_bound(proctype);
	itr2 = upper_bound( m_procfuncs, proctype );

	for (; itr1 != itr2; ++itr1)
	{
		if (itr1->second->deleted)
			continue;

		itr1->second->m_data = data;
		itr1->second->m_base->execute();
		itr1->second->m_data = NULL; //clear data for next proc :P
	}
}

void Unit::UpdateStats()
{
	if (IsPlayer())
		static_cast<Player*>(this)->UpdateStats();
	if (IsPet())
		static_cast<Pet*>(this)->UpdateStats();
}

void Unit::SetInitialFactionStuff()
{
	if (m_faction == NULL)
		return;

	if (m_faction->FactionFlags & 0x1000)
	{
		Log.Debug("Unit", "Marked unit "I64FMT" as a contested guard", GetGUID());
		m_aiInterface->m_neutralGuard = true;
	}

	sEventMgr.RemoveEvents(this, EVENT_INITIAL_FACTION_EVENT);
}

/*void Unit::FuzzTest()
{
	//1 = balinda, 2 = galvangar
	uint32 npc1 = 11949, npc2 = 11947;
	std::vector<uint32> spells;

	if (!isAlive())
	{
		//recreate us!
		CreatureInfo * ci = CreatureNameStorage.LookupEntry(GetEntry());
		CreatureProto * cp = CreatureProtoStorage.LookupEntry(GetEntry());


		Creature *c = GetMapMgr()->CreateCreature(GetEntry());
		c->Load(cp, GetPositionX(), GetPositionX(), GetPositionZ());

		c->AddToWorld(GetMapMgr());
		
		sEventMgr.AddEvent(static_cast<Unit *>(c), &Unit::FuzzTest, EVENT_UNK, 1000, 0, 1);
		static_cast<Unit *>(this)->SafeDelete();
		return;
	}
	
	if (GetEntry() == npc1)
	{
		spells.push_back(45067);
		spells.push_back(46987);
		spells.push_back(46988);
		spells.push_back(38384);
		spells.push_back(46608);
	}
	else
	{
		spells.push_back(15284);
		//spells.push_back(19134); eek its freezing my client
		spells.push_back(16856);
		//spells.push_back(13736);
	}

	if (GetCurrentSpell() != NULL)
		return;

	uint32 spellid = spells.at(RandomUInt(spells.size() -1));

	Spell *s = new Spell(this, dbcSpell.LookupEntry(spellid), false, NULL);
	SpellCastTargets t;
	s->GenerateTargets(&t);

	if (t.m_destX == 0.0f && t.m_destY == 0.0f && t.m_destZ == 0.0f && t.m_target == 0)
	{
		s = NULL;
		return;
	}

	s->m_targets = t;

	if (s->CanCast(false))
	{
		s->prepare(&t);
		GetAIInterface()->StopMovement(0);
	}
}*/

void Unit::AddOnAuraRemoveSpell(uint32 NameHash, uint32 procSpell, uint32 procChance, bool procSelf)
{
	onAuraRemove *proc;

	if (m_onAuraRemoveSpells == NULL)
		m_onAuraRemoveSpells = hashmap_new();

	if (hashmap_get(m_onAuraRemoveSpells, NameHash, (any_t*)&proc) == MAP_OK)
	{
		proc->spell = procSpell;
		proc->chance = procChance;
		proc->self = procSelf;
		proc->deleted = false;
	}
	else
	{
		proc = (onAuraRemove*)malloc(sizeof(onAuraRemove));
		proc->spell = procSpell;
		proc->chance = procChance;
		proc->self = procSelf;
		proc->deleted = false;

		hashmap_put(m_onAuraRemoveSpells, NameHash, (any_t)proc);
	}

}

void Unit::RemoveOnAuraRemoveSpell(uint32 NameHash)
{
	onAuraRemove *proc;

	if (m_onAuraRemoveSpells == NULL) return;

	if (hashmap_get(m_onAuraRemoveSpells, NameHash, (any_t*)&proc) == MAP_OK)
	{
		proc->deleted = true;
	}
}

// Aura by NameHash has been removed
void Unit::OnAuraRemove(uint32 NameHash, Unit * m_target)
{
	onAuraRemove *proc;

	if (m_onAuraRemoveSpells == NULL) return;

	if (hashmap_get(m_onAuraRemoveSpells, NameHash, (any_t*)&proc) == MAP_OK)
	{
		bool apply;

		if (proc->deleted == true) return;

		if (proc->chance != 100)
			apply = RandomUInt(100) < proc->chance;
		else
			apply = true;

		if (apply)
		{
			if (proc->self)
				CastSpell(static_cast<Unit *>(this), proc->spell, true);
			else if (m_target)
				m_target->CastSpell(m_target, proc->spell, true);
		}
	}
}

void Unit::OnPositionChange()
{
	// Note Randdrick : controle qui conduit le vehicule
	if (m_CurrentVehicle != NULL && m_CurrentVehicle->GetControllingUnit() == (Unit *)this && !(m_position == m_CurrentVehicle->GetPosition() || GetOrientation() == m_CurrentVehicle->GetOrientation())) //check orientation too since == operator of locationvector doesnt
	{
		m_CurrentVehicle->MoveVehicle(GetPositionX(), GetPositionY(), GetPositionZ(), GetOrientation());
	}
}

void Unit::ReapplyPassiveAuras()
{
	for (uint32 i=MAX_AURAS; i< MAX_AURAS + MAX_PASSIVE_AURAS; ++i)
	{
		if (m_auras[i] == NULL || !m_auras[i]->IsPassive())
			continue;

		sEventMgr.AddEvent(m_auras[i], &Aura::ReapplyModifiers, EVENT_AURA_UPDATE, 1, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}
void Unit::RemoveBeacons()
{
	//reset this fucker
	BeaconCaster = NULL;
	BeaconTarget = NULL;
}


void Unit::smsg_TalentsInfo(bool pet)
{
	if(this->IsPlayer()==false) return;

	WorldPacket data(SMSG_TALENTS_INFO, 1000);
	data << uint8(pet?1:0);
	if(pet)
		static_cast< Player* >(this)->BuildPetTalentsInfo(&data);
	else	// initialize sending all info
		static_cast< Player* >(this)->BuildFullTalentsInfo(&data, true);

	static_cast< Player* >( this )->GetSession()->SendPacket(&data);
}

//	custom functions for scripting
void Unit::SetWeaponDisplayId(uint8 slot, uint32 displayId)
{
	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+slot, displayId);
}

bool Unit::IsFFAPvPFlagged()
{ 
	return HasByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_FFA_PVP);
}


void Unit::SetFFAPvPFlag()
{
	if(IsFFAPvPFlagged()) return;

	SetByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_FFA_PVP);
}

void Unit::RemoveFFAPvPFlag()
{
	if(!IsFFAPvPFlagged()) return;

	RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_FFA_PVP);
}

//! Is PVP flagged?
bool Unit::IsPvPFlagged()
{
	return HasByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_PVP);
}

void Unit::SetPvPFlag()
{
	// reset the timer as well..
	if(IsPlayer()) ((Player *)(this))->StopPvPTimer();

	SetByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_PVP);
}

//! Removal
void Unit::RemovePvPFlag()
{
	if(IsPlayer()) ((Player *)(this))->StopPvPTimer();
	RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, U_FIELD_BYTES_FLAG_PVP);
}

void Unit::SetPowerType(uint8 type)
{
	SetByte(UNIT_FIELD_BYTES_0,3,type);
	if (type == POWER_TYPE_RUNIC_POWER && IsPlayer())
	{
		SetFloatValue(PLAYER_RUNE_REGEN_1, 0.100000f);
		SetFloatValue(PLAYER_RUNE_REGEN_1 + 1, 0.100000f);
		SetFloatValue(PLAYER_RUNE_REGEN_1 + 2, 0.100000f);
		SetFloatValue(PLAYER_RUNE_REGEN_1 + 3, 0.100000f);
	}

	//SendPowerUpdate();
}

float Unit::GetDetectRange(Unit *Target)
{
	return this->CalcDistance(Target);
}
// --- Nouvelle gestion des trajets / cible par Randdrick pour Waad
Unit* Unit::GetBestTargetPlayer(Target_Filter pFilter)
{
	//Construction de liste potentielle des trajets
	UnitArray TargetArray;
	for( set<Player*>::iterator PlayerIter = this->GetInRangePlayerSetBegin(); PlayerIter != this->GetInRangePlayerSetEnd(); ++PlayerIter ) 
	{ 
		if( IsUnitValidTarget(*PlayerIter, pFilter) ) TargetArray.push_back(static_cast<Unit*>(*PlayerIter));
	}

	return ChoiceOfBestTarget(TargetArray, pFilter);
}

Unit* Unit::GetBestTargetUnit(Target_Filter pFilter)
{
	//Construction de liste potentielle des trajets
	UnitArray TargetArray;
	if( pFilter & TARGET_FILTER_FRIENDLY )
	{
		for( std::set<Object*>::iterator ObjectIter = this->GetInRangeSetBegin(); ObjectIter != this->GetInRangeSetEnd(); ++ObjectIter )
		{
			if( IsUnitValidTarget(*ObjectIter, pFilter) ) TargetArray.push_back(static_cast<Unit*>(*ObjectIter));
		}
		if( IsUnitValidTarget(this, pFilter) ) TargetArray.push_back(this);
	}
	else
	{
		for( std::set<Object*>::iterator ObjectIter = this->GetInRangeOppFactsSetBegin(); ObjectIter != this->GetInRangeOppFactsSetEnd(); ++ObjectIter )
		{
			if( IsUnitValidTarget(*ObjectIter, pFilter) ) TargetArray.push_back(static_cast<Unit*>(*ObjectIter));
		}
	}

	return ChoiceOfBestTarget(TargetArray, pFilter);
}

Unit* Unit::ChoiceOfBestTarget(UnitArray& pTargetArray, Target_Filter pFilter)
{
	//Si une seule trajectoire possible, retour de cette dernière
	if( pTargetArray.size() == 1 ) return pTargetArray[0];

	//Trouve l'unité la plus proche si requis
	if( pFilter & TARGET_FILTER_CLOSEST ) return ChoiceOfNearestTarget(pTargetArray);

	//Trouve la seconde cible la plus détestée si requis 
	if( pFilter & TARGET_FILTER_SECOND_MOSTED_HASTED ) return ChoiceOfSecondMostHatedTarget(pTargetArray);

	//Choisi aléatoirement une cible dans le tableau
	return ( pTargetArray.size() > 1 ) ? pTargetArray[RandomUInt((uint32)pTargetArray.size()-1)] : NULL;
}

Unit* Unit::ChoiceOfNearestTarget(UnitArray& pTargetArray)
{
	Unit* NearestUnit = NULL;
	float Distance, NearestDistance = 99999;
	for( UnitArray::iterator UnitIter = pTargetArray.begin(); UnitIter != pTargetArray.end(); ++UnitIter )
	{
		Distance = GetDetectRange(*UnitIter);
		if( Distance < NearestDistance )
		{
			NearestDistance = Distance;
			NearestUnit = (*UnitIter);
		}
	}
	return NearestUnit;
}

Unit* Unit::ChoiceOfSecondMostHatedTarget(UnitArray& pTargetArray)
{
	Unit* TargetUnit = NULL;
	Unit* MostHatedUnit = NULL;
	Unit* CurrentTarget = this->GetAIInterface()->GetNextTarget();
	uint32 Threat = 0, HighestThreat = 0;
	for( UnitArray::iterator UnitIter = pTargetArray.begin(); UnitIter != pTargetArray.end(); ++UnitIter )
	{
		TargetUnit = (*UnitIter);
		if( TargetUnit != CurrentTarget )
		{
			Threat = TargetUnit->GetAIInterface()->getThreatByPtr(this);
			if( Threat > HighestThreat )
			{
				MostHatedUnit = TargetUnit;
				HighestThreat = Threat;
			}
		}
	}
	return MostHatedUnit;
}

bool Unit::IsUnitValidTarget(Object* pObject, Target_Filter pFilter)
{
	//Contrôle la validité de l'unité
	if( !pObject->IsUnit() ) return false;
	if( pObject->GetInstanceID() != this->GetInstanceID() ) return false;
	Unit* UnitTarget = static_cast<Unit*>(pObject);

	//Ne prend pas en compte les unité morte, supposée morte, ou invisibles
	if( !UnitTarget->isAlive() ) return false;
	if( UnitTarget->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH) ) return false;
	if( UnitTarget->m_invisFlag ) return false;

	//Ne prend pas en compte les unités qui ne sont pas dans la liste de menace
	if( this->GetAIInterface()->getThreatByPtr(UnitTarget) == 0 ) return false;

	//Contrôle si un filtre doit être appliqué
	if( pFilter != TARGET_FILTER_NONE )
	{
		//Ne prend pas en compte la cible actuelle, si requis
		if( (pFilter & TARGET_FILTER_NOT_CURRENT) && UnitTarget == this->GetAIInterface()->GetNextTarget() ) return false;

		//Garde seulement les cibles blessées, si requis
		if( (pFilter & TARGET_FILTER_WOUNDED) && UnitTarget->GetHealthPct() >= 99 ) return false;

		//Status hostile / amicale
		if( pFilter & TARGET_FILTER_FRIENDLY ) 
		{
			if( !UnitTarget->CombatStatus.IsInCombat() ) return false; //Ne prend pas en compte les cibles hors combat, if amicale
			if( isHostile(this, UnitTarget) || this->GetAIInterface()->getThreatByPtr(UnitTarget) > 0 ) return false;
		}
	}

	return true; //C'est une cible correcte
}
///--------------------------------------------------------------------
// A n'appeler que si et seulement si 'm_speaker' est vrai (Branruz)
// Pour init la class AI_Speaker, il suffit de faire SetPNJIsSpeaker(true); (false pour dé-activer)
// if(Unit->IsSpeaker()) etc....
void Unit::SearchBestResponse(Player* plr,string texte_player)
{
 char Pnj_Say[512];

 if(!m_AI_Speak) // Pas encore eu de discution
 {
	m_AI_Speak = new AI_Speak(this);

	if(!m_AI_Speak)
	{
     snprintf(Pnj_Say,500,"Hmmmmmmm Hmmmmmmm Hmmmmmm, j'ai un chat dans la gorge %s",plr->GetName());
     this->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, Pnj_Say);
	 this->Emote(EMOTE_ONESHOT_TALK);
	 return;
	}
 }

 m_AI_Speak->Listen(plr,texte_player); // Allez on parle^^
 //this->Emote(EMOTE_ONESHOT_TALK);
}
//--------------------------------------------------------
/* Global sur Object*
float Unit::GetSize() 
{ 
        float uTaille = 0;

		if(this->IsPlayer())
		{
			uTaille = (((Player *)this)->Taille) * GetFloatValue(OBJECT_FIELD_SCALE_X);
			uTaille = uTaille*GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS);
		}
		else if(this->IsCreature())
		{
			uTaille = (((Creature *)this)->Taille) * GetFloatValue(OBJECT_FIELD_SCALE_X);
			uTaille = uTaille*GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS);
		}
		else uTaille = 1;

		return ( uTaille );
		//return (GetFloatValue(OBJECT_FIELD_SCALE_X) * GetFloatValue(UNIT_FIELD_BOUNDINGRADIUS)); 
}
*/
void Object::ClearLoot()
{
	// better cancel any rolls just in case.
	for(vector<__LootItem>::iterator itr = m_loot.items.begin(); itr != m_loot.items.end(); ++itr)
	{
		if( itr->roll != NULL )
		{
			sEventMgr.RemoveEvents(itr->roll);
			itr->roll = NULL; // buh-bye!
		}
	}

	m_loot.gold = 0;
	m_loot.items.clear();
	m_loot.looters.clear();
}

void Unit::EventRegainFlight()
{
	if(!IsPlayer())
	{
		if(sEventMgr.HasEvent(this,EVENT_REGAIN_FLIGHT))
			sEventMgr.RemoveEvents(this,EVENT_REGAIN_FLIGHT);
		return;
	}
	Player * plr = static_cast<Player *>(this);

	if(!plr->flying_aura)
	{
		if(sEventMgr.HasEvent(this,EVENT_REGAIN_FLIGHT))
			sEventMgr.RemoveEvents(this,EVENT_REGAIN_FLIGHT);
		return;
	}

	EnableFlight();
	if(sEventMgr.HasEvent(this,EVENT_REGAIN_FLIGHT))
		sEventMgr.RemoveEvents(this,EVENT_REGAIN_FLIGHT);
}

void Unit::SetRedirectThreat(Unit * target, float amount, uint32 Duration)
{
	mThreatRTarget = target;
	mThreatRAmount = amount;
	if(Duration)
		sEventMgr.AddEvent( this, &Unit::EventResetRedirectThreat, EVENT_RESET_REDIRECT_THREAT, Duration, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Unit::EventResetRedirectThreat()
{
	mThreatRTarget = NULL;
	mThreatRAmount = 0.0f;
	sEventMgr.RemoveEvents(this,EVENT_RESET_REDIRECT_THREAT);
}

void Unit::knockback(int32 basepoint, uint32 miscvalue, bool disengage )
{
	float dx, dy;
	float value1 = float( basepoint );
	float value2 = float( miscvalue );
	float proportion;
	float multiplier;

	if( disengage )
		multiplier = -1.0f;
	else
		multiplier = 1.0f;

	if( value2 != 0 )
		proportion = value1 / value2;
	else
		proportion = 0;

	if(proportion)
	{
		value1 = value1 / (10 * proportion);
		value2 = value2 / 10 * proportion;
	}
	else
	{
		value2 = value1 / 10;
		value1 = 0.1f;
	}

	dx = sinf( GetOrientation() );
	dy = cosf( GetOrientation() );

	if( IsCreature() )
	{
		float x = GetPositionX() + (value1 * dx);
		float y = GetPositionY() + (value1 * dy);
		float z = GetPositionZ();
		float dist = CalcDistance(x, y, z);
		uint32 movetime = GetAIInterface()->GetMoveTime(dist);
		GetAIInterface()->SendMoveToPacket( x, y, z, 0.0f, movetime, MONSTER_MOVE_FLAG_JUMP );
		SetPosition(x, y, z, 0.0f);
		GetAIInterface()->StopMovement(movetime);

		if (GetCurrentSpell() != NULL)
			GetCurrentSpell()->cancel(SPELL_FAILED_INTERRUPTED);
	}
	else if(IsPlayer())
	{
		WorldPacket data(SMSG_MOVE_KNOCK_BACK, 50);
		data << GetNewGUID();
		data << uint32( getMSTime() );
		data << float( multiplier * dy );
		data << float( multiplier * dx );
		data << float( value1 );
		data << float( -value2 );
		Player *plr = static_cast<Player *>(this);
		plr->GetSession()->SendPacket( &data );
		plr->ResetHeartbeatCoords();
		plr->DelaySpeedHack(5000);
		plr->GetSession()->m_isKnockedback = true;
		if(plr->flying_aura)
		{
			plr->DisableFlight();
			sEventMgr.AddEvent( this, &Unit::EventRegainFlight, EVENT_REGAIN_FLIGHT, 5000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
	}
}