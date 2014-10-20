/*
* Ascent MMORPG Server
* Copyright (C) 2005-2009 Ascent Team <http://www.ascentcommunity.com/>
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

#define SPELL_CHANNEL_UPDATE_INTERVAL 1000

/// externals for spell system
extern pSpellEffect SpellEffectsHandler[TOTAL_SPELL_EFFECTS];
extern pSpellTarget SpellTargetHandler[TOTAL_SPELL_TARGET];

enum SpellTargetSpecification
{
    TARGET_SPECT_NONE       = 0,
    TARGET_SPEC_INVISIBLE   = 1,
    TARGET_SPEC_DEAD        = 2,
};

void SpellCastTargets::read( WorldPacket & data, Object* caster )
{
	m_target = NULL;
	m_srcX = m_srcY = m_srcZ = m_destX = m_destY = m_destZ = 0;
	m_strTarget = "";

	data >> m_targetMask;
	data >> m_targetMaskExtended;
	WoWGuid guid;

	Log.Warning("SpellCastTargets","targetMask:0x%04X, MaskExtended:0x%04X",m_targetMask,m_targetMaskExtended);

	if( m_targetMask == TARGET_FLAG_SELF || m_targetMask & TARGET_FLAG_GLYPH )
	{
		m_target = caster;
	}
	else // optimisation, TARGET_FLAG_SELF = 0x00000000 (Branruz)
	{
	 if( m_targetMask & TARGET_FLAG_UNIT)
	 {
		data >> guid;
		m_target = caster->GetMapMgr()->GetUnit(guid.GetOldGuid());
	 }

	 if( m_targetMask & TARGET_FLAG_OBJECT)
	 {
		data >> guid;
		uint64 oldguid = guid.GetOldGuid();
		m_target = caster->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(oldguid));
	 }

	 if( m_targetMask & ( TARGET_FLAG_CORPSE | TARGET_FLAG_CORPSE2 ) )
	 {
		data >> guid;
		uint64 oldguid = guid.GetOldGuid();
		m_target = objmgr.GetCorpse(GET_LOWGUID_PART(oldguid));
	 }

	 if( m_targetMask & TARGET_FLAG_ITEM )
	 {
		data >> guid;
		if (caster->IsPlayer())
			m_target = static_cast<Player*>(caster)->GetItemInterface()->GetItemByGUID(guid.GetOldGuid());
	 } 

	 if( m_targetMask & TARGET_FLAG_TRADE_ITEM )
	 {
		data >> guid;
		uint64 oldguid = guid.GetOldGuid();
		if (caster->IsPlayer())
		{
			Player* plr=static_cast<Player*>(caster)->GetTradeTarget();
			if (plr != NULL && plr->getTradeItem(GET_LOWGUID_PART(oldguid)) != NULL)
				m_target = plr->getTradeItem(GET_LOWGUID_PART(oldguid));
		}
	 }

	 if( m_targetMask & TARGET_FLAG_SOURCE_LOCATION )
	 {
		data >> guid >> m_srcX >> m_srcY >> m_srcZ;

		if( !( m_targetMask & TARGET_FLAG_DEST_LOCATION ) )
		{
			m_destX = m_srcX;
			m_destY = m_srcY;
			m_destZ = m_srcZ;
		}
	 }

	 if( m_targetMask & TARGET_FLAG_DEST_LOCATION )
	 {          
		//3.0.8 Andy  // data >> m_destX >> m_destY >> m_destZ;
		data >> guid >> m_destX >> m_destY >> m_destZ;
		if( !( m_targetMask & TARGET_FLAG_SOURCE_LOCATION ) )
		{
			m_srcX = m_destX;
			m_srcY = m_destY;
			m_srcZ = m_destZ;
		}
	 }

	 if( m_targetMask & TARGET_FLAG_STRING )
	 {
		data >> m_strTarget;
	 }
	} // End else m_targetMask == TARGET_FLAG_SELF
}

void SpellCastTargets::write( WorldPacket& data )
{
	data << m_targetMask;
	data << m_targetMaskExtended;

	if( m_targetMask & (TARGET_FLAG_UNIT | TARGET_FLAG_CORPSE | TARGET_FLAG_CORPSE2 | TARGET_FLAG_OBJECT | TARGET_FLAG_GLYPH) )
	{
		if (m_target)
		{
			FastGUIDPack( data, m_target->GetGUID() );
		} //else FastGUIDPack( data, m_unitTarget );
	}
    if( m_targetMask & ( TARGET_FLAG_ITEM | TARGET_FLAG_TRADE_ITEM ) )
	{
		if (m_target)
		{
			FastGUIDPack( data, m_target->GetGUID() );
		} //else FastGUIDPack( data, m_itemTarget );
	}	
	if( m_targetMask & TARGET_FLAG_SOURCE_LOCATION )
	{
		data << m_srcX << m_srcY << m_srcZ;
	}
	if( m_targetMask & TARGET_FLAG_DEST_LOCATION )
	{
		if(m_target)
		{
			// je pense que c'est celui la qui etait la qui plantait le core
			FastGUIDPack( data, m_target->GetGUID() );
			data << m_destX << m_destY << m_destZ;
		} 
		else
			data << uint8(0) << m_destX << m_destY << m_destZ;
	}
	/*if( m_targetMask & TARGET_FLAG_STRING )
		data << m_strTarget;*/
}

void SpellCastTargets::write( StackPacket& data )
{
	data << m_targetMask;
	data << m_targetMaskExtended;

	if( m_targetMask & (TARGET_FLAG_UNIT | TARGET_FLAG_CORPSE | TARGET_FLAG_CORPSE2 | TARGET_FLAG_OBJECT | TARGET_FLAG_GLYPH) )
	{
		if (m_target)
		{
			FastGUIDPack( data, m_target->GetGUID() );
		} //else FastGUIDPack( data, m_unitTarget );
	}
    if( m_targetMask & ( TARGET_FLAG_ITEM | TARGET_FLAG_TRADE_ITEM ) )
	{
		if (m_target)
		{
			FastGUIDPack( data, m_target->GetGUID() );
		} //else FastGUIDPack( data, m_itemTarget );
	}	
	if( m_targetMask & TARGET_FLAG_SOURCE_LOCATION )
	{
		data << m_srcX << m_srcY << m_srcZ;
	}
	if( m_targetMask & TARGET_FLAG_DEST_LOCATION )
	{
		if(m_target && m_target->IsUnit())
		{
			// je pense que c'est celui la qui etait la qui plantait le core
			FastGUIDPack( data, m_target->GetGUID() );
			data << m_destX << m_destY << m_destZ;
		} 
		else
			data << uint8(0) << m_destX << m_destY << m_destZ;
	}
	/*if( m_targetMask & TARGET_FLAG_STRING )
		data << m_strTarget;*/
}

Spell::Spell(Object* Caster, SpellEntry *info, bool triggered, Aura* aur)
{
	// ASSERT( Caster != NULL && info != NULL );
	// Caster et SpellEntry NE PEUVENT ETRE NULL, a tester avant le new sinon leak memoire
	if(Caster == NULL) 
	{
		Log.Error("Spell::Spell","Caster = NULL");
		return;
	}
	if(info == NULL) 
	{
        Log.Error("Spell::Spell","info = NULL");
		return;
	}
	POINTERLOGNEW(this);
	m_objectsHit = 0;
	m_objectsModerated = 0;
	delaycount = 0;
	m_instanceId = Caster->event_GetInstanceID();
	deleted = false;

	m_procdata = 0;	
	amp = 1.0f;
	triggered_id=0;
	forced_modifier=NULL;
	forced_applications=0;
	hasended=false;
	m_spellInfo = info;
	m_caster = Caster;
	m_owner  = NULL; // Init lors de la reception des trames reseaux (Spell_Use/Item_Use/Go_Use)
	duelSpell = false;

	m_spellScript = sScriptMgr.CreateScriptClassForSpell(m_spellInfo->Id, this);

	switch( m_caster->GetTypeId() )
	{
		case TYPEID_PLAYER:
        {
		 //   g_caster = NULL;
		 //   i_caster = NULL;
		 //   u_caster = static_cast< Unit* >( m_caster );   // Un player fait parti de la gestion Unit
		 //   p_caster = static_cast< Player* >( m_caster );
			if( ((Player *)m_caster)->GetDuelState() == DUEL_STATE_STARTED ) duelSpell = true;
        }break;

		case TYPEID_UNIT:
        {
		   // g_caster = NULL;
		   // i_caster = NULL;
		   // p_caster = NULL; // Une Unit ne peut pas faire partie de la gestion Player
		   // u_caster = static_cast< Unit* >( m_caster );
			if( m_caster->IsPet() && ( ((Pet *)m_caster)->GetPetOwner() != NULL     ) 
				                  && ( ((Pet *)m_caster)->GetPetOwner()->IsPlayer() )
								  && ( ((Player *)(((Pet* )m_caster)->GetPetOwner()))->GetDuelState() == DUEL_STATE_STARTED ) )
			{
			    duelSpell = true;
			}
        }break;

		case TYPEID_ITEM:
		case TYPEID_CONTAINER:
        {
		    //g_caster = NULL;
		    //u_caster = NULL;
		    //p_caster = NULL;
		    //i_caster = static_cast< Item* >( Caster );
			if( ((Item *)m_caster)->GetOwner() && 
				(((Item *)m_caster)->GetOwner()->GetDuelState() == DUEL_STATE_STARTED ) )
			{
				duelSpell = true;
			}
        }break;
		
		case TYPEID_GAMEOBJECT:
        {
		    //u_caster = NULL;
		    //p_caster = NULL;
		    //i_caster = NULL;
		    //g_caster = static_cast< GameObject* >( Caster );
        }break;
        default:
			
            sLog.outDebug("[DEBUG][SPELL] Incompatible object type, please report this to the dev's");
        break;
	}

	m_spellState = SPELL_STATE_NULL;

	m_castPositionX = m_castPositionY = m_castPositionZ = 0;
	//TriggerSpellId = 0;
	//TriggerSpellTarget = 0;
	m_triggeredSpell = triggered;
	m_AreaAura = false;
  
	m_triggeredByAura = aur;

	damageToHit = 0;
	castedItemId = 0;
	
	m_usesMana = false;
	m_Spell_Failed = false;
	hadEffect = false;
	bDurSet = false;
	bRadSet[0] = false;
	bRadSet[1] = false;
	bRadSet[2] = false;
	
	cancastresult = SPELL_CANCAST_OK;
	
	m_requiresCP = false;
	unitTarget = NULL;
	itemTarget = NULL;
	gameObjTarget = NULL;
	playerTarget = NULL;
	corpseTarget = NULL;
	m_backupTarget = NULL;
	judgement = false;
	add_damage = 0;
	m_Delayed = false;
	m_ForceConsumption = false;
	pSpellId = 0;
	m_cancelled = false;
	ProcedOnSpell = 0;
	forced_basepoints[0] = forced_basepoints[1] = forced_basepoints[2] = 0;
	extra_cast_number = 0;
	m_reflectedParent = NULL;
	m_isCasting = false;
	//m_hitTargetCount = 0;
	//m_magnetTarget = NULL;
}
void Spell::Init()
{
	m_spellScript = sScriptMgr.CreateScriptClassForSpell(m_spellInfo->Id, this);

	this->GetSpellProto()->EffectBasePointCalculate[0] = 0;
	this->GetSpellProto()->EffectBasePointCalculate[1] = 0;
	this->GetSpellProto()->EffectBasePointCalculate[2] = 0;
}

Spell::~Spell()
{
	//POINTERLOGDELETE(this);

	//delete only if no aura references on it :)
	if (m_spellScript != NULL)
	{
		m_spellScript->_spell = NULL; // Normalement mis à 0 ds TryDelete si diff de 0
		m_spellScript->TryDelete();
	}
	
	if(m_caster)
	{
     if(m_caster->IsUnit() && ((Unit *)m_caster)->GetCurrentSpell() == this)
	                      ((Unit *)m_caster)->SetCurrentSpell(NULL);
	}

	//g_caster = NULL;
	//u_caster = NULL;
	//i_caster = NULL;
	//p_caster = NULL;
	m_caster = NULL;
	m_triggeredByAura = NULL;
	unitTarget = NULL;
	itemTarget = NULL;
	gameObjTarget = NULL;
	playerTarget = NULL;
	corpseTarget = NULL;
	//m_magnetTarget = NULL;
	m_reflectedParent = NULL;

	sEventMgr.RemoveEvents(this);
}

//i might forget conditions here. Feel free to add them
bool Spell::IsStealthSpell()
{
	//check if aura name is some stealth aura
	if( m_spellInfo->EffectApplyAuraName[0] == SPELL_AURA_MOD_STEALTH ||
		m_spellInfo->EffectApplyAuraName[1] == SPELL_AURA_MOD_STEALTH ||
		m_spellInfo->EffectApplyAuraName[2] == SPELL_AURA_MOD_STEALTH )
		return true;
	return false;
}

//i might forget conditions here. Feel free to add them
bool Spell::IsInvisibilitySpell()
{
	//check if aura name is some invisibility aura
	if( m_spellInfo->EffectApplyAuraName[0] == SPELL_AURA_MOD_INVISIBILITY ||
		m_spellInfo->EffectApplyAuraName[1] == SPELL_AURA_MOD_INVISIBILITY ||
		m_spellInfo->EffectApplyAuraName[2] == SPELL_AURA_MOD_INVISIBILITY )
		return true;
	return false;
}

/*
//Note Randdrick : ancienne gestion des spells en 2.4.x
void Spell::FillSpecifiedTargetsInArea( float srcx, float srcy, float srcz, uint32 ind, uint32 specification )
{
    FillSpecifiedTargetsInArea( ind, srcx, srcy, srcz, GetRadius(ind), specification );
}

// for the moment we do invisible targets
void Spell::FillSpecifiedTargetsInArea(uint32 i,float srcx,float srcy,float srcz, float range, uint32 specification)
{
	//TargetsList *tmpMap=&m_targetUnits[i];
    //InStealth()
    float r = range * range;
	//uint8 did_hit_result;
    for( std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
    {
        // don't add objects that are not units and that are dead
        if( !( (*itr)->IsUnit() ) || ! static_cast< Unit* >( *itr )->isAlive())
            continue;
        
        //static_cast< Unit* >(*itr)->InStealth()
        if( m_spellInfo->targetCreatureType )
        {
            if((*itr)->GetTypeId()!= TYPEID_UNIT)
                continue;
            CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureName();
            if(!inf || !(1<<(inf->Type-1) & m_spellInfo->targetCreatureType))
                continue;
        }

        if(IsInrange(srcx,srcy,srcz,(*itr),r))
        {
            if( m_caster->IsUnit() )
            {
                if( isAttackable( ((Unit *)m_caster), static_cast< Unit* >( *itr ),!(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
                {
					_AddTarget((static_cast< Unit* >(*itr)), i);
                }

            }
            else //cast from GO
            {
                if(((GameObject *)m_caster) && ((GameObject *)m_caster)->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && ((GameObject *)m_caster)->m_summoner)
                {
                    //trap, check not to attack owner and friendly
                    if(isAttackable(((GameObject *)m_caster)->m_summoner,static_cast< Unit* >(*itr),!(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
                        _AddTarget((static_cast< Unit* >(*itr)), i);
                }
                else
                    _AddTargetForced((*itr)->GetGUID(), i);
            }
            if( m_spellInfo->MaxTargets)
            {
                if( m_objectsHit >= m_spellInfo->MaxTargets )
                    return;
            }
        }
    }
}
void Spell::FillAllTargetsInArea(LocationVector & location,uint32 ind)
{
    FillAllTargetsInArea(ind,location.x,location.y,location.z,GetRadius(ind));
}

void Spell::FillAllTargetsInArea(float srcx,float srcy,float srcz,uint32 ind)
{
	FillAllTargetsInArea(ind,srcx,srcy,srcz,GetRadius(ind));
}

/// We fill all the targets in the area, including the stealth ed one's
void Spell::FillAllTargetsInArea(uint32 i,float srcx,float srcy,float srcz, float range)
{
	//TargetsList *tmpMap=&m_targetUnits[i];
	float r = range*range;
	//uint8 did_hit_result;
	for( std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
	{
		if( !( (*itr)->IsUnit() ) || ! static_cast< Unit* >(*itr)->isAlive() || ( (*itr)->GetTypeId()==TYPEID_UNIT && static_cast<Creature *>(*itr)->IsTotem() ) || !(*itr)->PhasedCanInteract(m_caster))
			continue;

		if( m_spellInfo->targetCreatureType )
		{
			if( (*itr)->GetTypeId()!= TYPEID_UNIT )
				continue;
			CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureName();
			if( !inf || !( 1 << (inf->Type-1) & m_spellInfo->targetCreatureType ) )
				continue;
		}
		if( IsInrange( srcx, srcy, srcz, (*itr), r ) )
		{
			if( m_caster->IsUnit() )
			{
				if( isAttackable( ((Unit *)m_caster), static_cast< Unit* >(*itr), !(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED) ) )
				{
					_AddTarget((static_cast< Unit* >(*itr)), i);
				}
			}
			else //cast from GO
			{
				if( ((GameObject *)m_caster) != NULL && ((GameObject *)m_caster)->GetUInt32Value( OBJECT_FIELD_CREATED_BY ) && ((GameObject *)m_caster)->m_summoner != NULL )
				{
					//trap, check not to attack owner and friendly
					if( isAttackable( ((GameObject *)m_caster)->m_summoner, static_cast< Unit* >(*itr), !(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED) ) )
						_AddTarget((static_cast< Unit* >(*itr)), i);
				}
				else
					_AddTargetForced((*itr)->GetGUID(), i);
			}			
			if( m_spellInfo->MaxTargets )
				if( m_objectsHit >= m_spellInfo->MaxTargets )
					return;
		}
	}	
}

// We fill all the targets in the area, including the stealth ed one's
void Spell::FillAllFriendlyInArea( uint32 i, float srcx, float srcy, float srcz, float range )
{
	//TargetsList *tmpMap=&m_targetUnits[i];
	float r = range * range;
	//uint8 did_hit_result;
	for( std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
	{
		if( !((*itr)->IsUnit()) || !static_cast< Unit* >(*itr)->isAlive() || !(*itr)->PhasedCanInteract(m_caster))
			continue;

		if( m_spellInfo->targetCreatureType )
		{
			if((*itr)->GetTypeId()!= TYPEID_UNIT)
				continue;
			CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureName();
			if(!inf || !(1<<(inf->Type-1) & m_spellInfo->targetCreatureType))
				continue;
		}

		if( IsInrange( srcx, srcy, srcz, (*itr), r ) )
		{
			if( m_caster->IsUnit() )
			{
				if( isFriendly( ((Unit *)m_caster), static_cast< Unit* >(*itr) ) )
				{
					_AddTarget((static_cast< Unit* >(*itr)), i);
				}
			}
			else //cast from GO
			{
				if( ((GameObject *)m_caster) != NULL && ((GameObject *)m_caster)->GetUInt32Value( OBJECT_FIELD_CREATED_BY ) && ((GameObject *)m_caster)->m_summoner != NULL )
				{
					//trap, check not to attack owner and friendly
					if( isFriendly( ((GameObject *)m_caster)->m_summoner, static_cast< Unit* >(*itr) ) )
						_AddTargetForced((*itr)->GetGUID(), i);
				}
				else
					_AddTargetForced((*itr)->GetGUID(), i);
			}			
			if( m_spellInfo->MaxTargets )
				if( m_objectsHit >= m_spellInfo->MaxTargets )
					return;
		}
	}	
}*/

uint64 Spell::GetSinglePossibleEnemy(uint32 i,float prange)
{
	float r;
	if(prange)
		r = prange;
	else
	{
		r = m_spellInfo->base_range_or_radius_sqr;
		if(m_caster->IsUnit())
		{
		 if( m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2])
		 {
			SM_FFValue(((Unit *)m_caster)->SM_FRadius,&r,m_spellInfo);
			SM_PFValue(((Unit *)m_caster)->SM_PRadius,&r,m_spellInfo);
		 }
		}
	}
	float srcx = m_caster->GetPositionX(), srcy = m_caster->GetPositionY(), srcz = m_caster->GetPositionZ();
	for( std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
	{
		if( !( (*itr)->IsUnit() ) || !static_cast<Unit *>(*itr)->isAlive() || !(*itr)->PhasedCanInteract(m_caster))
			continue;

		if( m_spellInfo->targetCreatureType )
		{
			if( (*itr)->GetTypeId() != TYPEID_UNIT ) continue;

			CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureName();

			if(!inf || !(1<<(inf->Type-1) & m_spellInfo->targetCreatureType)) continue;
		}	

		if(IsInrange(srcx,srcy,srcz,(*itr),r))
		{
			if( m_caster->IsUnit() || 
				(m_caster->IsGO() && ((GameObject *)m_caster)->GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID) == GAMEOBJECT_TYPE_TRAP) )
			{
				if(isAttackable(((Unit*)m_caster), static_cast< Unit* >( *itr ),
					!(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)) && DidHit(i,((Unit*)*itr))==SPELL_DID_HIT_SUCCESS)
					return (*itr)->GetGUID(); 			
			}
			else if(m_caster->IsGO())//cast from GO
			{
				if(((GameObject *)m_caster)->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && ((GameObject *)m_caster)->m_summoner)
				{
					//trap, check not to attack owner and friendly
					if( isAttackable( ((GameObject *)m_caster)->m_summoner, static_cast< Unit* >( *itr ),
						                                       !(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)))
						return (*itr)->GetGUID();
				}
			}			
		}
	}
	return 0;
}

uint64 Spell::GetSinglePossibleFriend(uint32 i,float prange)
{
	float r;
	if(prange)
		r = prange;
	else
	{
		r = m_spellInfo->base_range_or_radius_sqr;

		if(m_caster->IsUnit())
		{
		 if( m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2])
		 {
			SM_FFValue(((Unit *)m_caster)->SM_FRadius,&r,m_spellInfo);
			SM_PFValue(((Unit *)m_caster)->SM_PRadius,&r,m_spellInfo);
		 }
		}
	}
	float srcx=m_caster->GetPositionX(),srcy=m_caster->GetPositionY(),srcz=m_caster->GetPositionZ();
	for(std::set<Object*>::iterator itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd(); itr++ )
	{
		if( !( (*itr)->IsUnit() ) || !static_cast< Unit* >( *itr )->isAlive() )
			continue;
		if( m_spellInfo->targetCreatureType )
		{
			if((*itr)->GetTypeId()!= TYPEID_UNIT)
				continue;
			CreatureInfo *inf = ((Creature*)(*itr))->GetCreatureName();
				if(!inf || !(1<<(inf->Type-1) & m_spellInfo->targetCreatureType))
					continue;
		}	
		if(IsInrange(srcx,srcy,srcz,(*itr),r))
		{
			if( m_caster->IsUnit() )
			{
				if( isFriendly( ((Unit *)m_caster), static_cast< Unit* >( *itr ) ) && DidHit(i, ((Unit*)*itr))==SPELL_DID_HIT_SUCCESS)
					return (*itr)->GetGUID(); 			
			}
			else if(m_caster->IsGO()) //cast from GO
			{
				if(((GameObject *)m_caster)->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && ((GameObject *)m_caster)->m_summoner)
				{
					//trap, check not to attack owner and friendly
					if( isFriendly( ((GameObject *)m_caster)->m_summoner, static_cast< Unit* >( *itr ) ) )
						return (*itr)->GetGUID();
				}
			}			
		}
	}
	return 0;
}

uint8 Spell::DidHit(uint32 effindex,Unit* target)
{
	//note resistchance is vise versa, is full hit chance
	Unit* u_victim = target;
	Player* p_victim = ( target->GetTypeId() == TYPEID_PLAYER ) ? static_cast< Player* >( target ) : NULL;
	uint32 spellschool = GetBestSchoolForSpell(target);

	// 
	float baseresist[3] = { 4.0f, 5.0f, 6.0f };
	int32 lvldiff;
	float resistchance ;
	if( u_victim == NULL )
		return SPELL_DID_HIT_MISS;

	/************************************************************************/
	/* Can't resist non-unit                                                */
	/************************************************************************/
	if(!m_caster->IsUnit()) return SPELL_DID_HIT_SUCCESS;

	/************************************************************************/
	/* Can't reduce your own spells                                         */
	/************************************************************************/
	if(m_caster == u_victim) return SPELL_DID_HIT_SUCCESS;

	/************************************************************************/
	/* Check if the unit is evading                                         */
	/************************************************************************/
	if(u_victim->GetTypeId()==TYPEID_UNIT && u_victim->GetAIInterface()->getAIState()==STATE_EVADE)
		return SPELL_DID_HIT_EVADE;

	/************************************************************************/
	/* Check if the target is immune to this spell school                   */
	/************************************************************************/
	if(u_victim->SchoolImmunityList[spellschool]) return SPELL_DID_HIT_IMMUNE;

	/************************************************************************/
	/* Can we reflect?                                                      */
	/************************************************************************/
	if( Reflect(u_victim) && (!m_triggeredSpell || m_reflectedParent == NULL || (m_reflectedParent->m_spellInfo->EffectApplyAuraName[0] != SPELL_AURA_PERIODIC_TRIGGER_SPELL && m_reflectedParent->m_spellInfo->EffectApplyAuraName[1] != SPELL_AURA_PERIODIC_TRIGGER_SPELL && m_reflectedParent->m_spellInfo->EffectApplyAuraName[2] != SPELL_AURA_PERIODIC_TRIGGER_SPELL)))
		return SPELL_DID_HIT_REFLECT;
	
	/*************************************************************************/
	/* Check if the target is immune to this mechanic                        */
	/*************************************************************************/
 
	if(u_victim->MechanicsDispels[m_spellInfo->mechanics])
	{
		// Immune - IF, and ONLY IF, there is no damage component!
		bool no_damage_component = true;
		for( int x = 0 ; x <= 2 ; x ++ )
		{
			if( m_spellInfo->Effect[x] == SPELL_EFFECT_SCHOOL_DAMAGE
				|| m_spellInfo->Effect[x] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE
				|| m_spellInfo->Effect[x] == SPELL_EFFECT_WEAPON_DAMAGE
				|| m_spellInfo->Effect[x] == SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL
				|| m_spellInfo->Effect[x] == SPELL_EFFECT_DUMMY
				|| ( m_spellInfo->Effect[x] == SPELL_EFFECT_APPLY_AURA &&
					( m_spellInfo->EffectApplyAuraName[x] == SPELL_AURA_PERIODIC_DAMAGE 
					) )
				)
			{
				no_damage_component = false;
				break;
			}
		}
		if( no_damage_component )
			return SPELL_DID_HIT_IMMUNE; // Moved here from Spell::CanCast
	}

	/************************************************************************/
	/* Check if the target has a % resistance to this mechanic              */
	/************************************************************************/
	if( m_spellInfo->mechanics < MECHANIC_COUNT)
	{
		float res;
		if(p_victim) res = p_victim->MechanicsResistancesPCT[Spell::GetMechanic(m_spellInfo)];
		else         res = u_victim->MechanicsResistancesPCT[Spell::GetMechanic(m_spellInfo)];

		if( !(m_spellInfo->c_is_flags & SPELL_FLAG_IS_NOT_RESISTABLE) && Rand(res))
			return SPELL_DID_HIT_RESIST;
	}

	/************************************************************************/
	/* Check if the spell is a melee attack and if it was missed/parried    */
	/************************************************************************/
	uint32 melee_test_result;
	if( m_spellInfo->is_melee_spell || m_spellInfo->is_ranged_spell )
	{
		uint32 _type;
		if( GetType() == DEFENSE_TYPE_RANGED )
			_type = RANGED;
		else
		{         
			if (m_spellInfo->attributesExC & ATTRIBUTESEXC_OFFHAND)
				_type =  OFFHAND;
			else
				_type = MELEE;
		}

		melee_test_result = ((Unit *)m_caster)->GetSpellDidHitResult( u_victim, _type, m_spellInfo, spellschool );
		if(melee_test_result != SPELL_DID_HIT_SUCCESS)
			return (uint8)melee_test_result;
	}

	/************************************************************************/
	/* Check if the spell is resisted.                                      */
	/************************************************************************/
	if( spellschool==0  || m_spellInfo->is_ranged_spell ) // all ranged spells are physical too...
		return SPELL_DID_HIT_SUCCESS;

	bool pvp =(m_caster->IsPlayer() && p_victim);

	if(pvp) lvldiff = p_victim->getLevel() - ((Player *)m_caster)->getLevel();
	else    lvldiff = u_victim->getLevel() - ((Unit *)m_caster)->getLevel();

	if (lvldiff < 0)
	{
		resistchance = baseresist[0] +lvldiff;
	}
	else
	{
		if(lvldiff < 3)
		{ 
				resistchance = baseresist[lvldiff];
		}
		else
		{
			if(pvp)
				resistchance = baseresist[2] + (((float)lvldiff-2.0f)*7.0f);
			else
				resistchance = baseresist[2] + (((float)lvldiff-2.0f)*11.0f);
		}
	}
	/*check mechanical resistance
	//i have no idea what is the best pace for this code
	if( m_spellInfo->mechanics<27)
	{
		if(p_victim)
			resistchance += p_victim->MechanicsResistancesPCT[m_spellInfo->mechanics];
		else 
			resistchance += u_victim->MechanicsResistancesPCT[m_spellInfo->mechanics];
	}

	if (m_spellInfo->dispelType < NUM_DISPELS)
		resistchance += u_victim->DispelResistancesPCT[m_spellInfo->dispelType];
*/
	//rating bonus
	if( m_caster->IsPlayer() )
	{
		resistchance -= ((Player *)m_caster)->CalcRating( PLAYER_RATING_MODIFIER_SPELL_HIT );
		resistchance -= ((Player *)m_caster)->GetHitFromSpell();
	}

	if(p_victim)
		resistchance += p_victim->m_resist_hit[2];

	if( this->m_spellInfo->Effect[effindex] == SPELL_EFFECT_DISPEL 
		&& (m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2]) && ((Unit *)m_caster))
	{
		SM_FFValue(((Unit *)m_caster)->SM_FRezist_dispell,&resistchance,m_spellInfo);
		SM_PFValue(((Unit *)m_caster)->SM_PRezist_dispell,&resistchance,m_spellInfo);
	}

	if((m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2]) && m_caster->IsUnit())
	{
		float hitchance=0;
		/*SM_FFValue(((Unit *)m_caster)->SM_FHitchance,&hitchance,m_spellInfo);
		SM_PFValue(((Unit *)m_caster)->SM_FHitchance,&hitchance,m_spellInfo);*/
		SM_FFValue(((Unit *)m_caster)->SM[SMT_HITCHANCE][0],&hitchance,m_spellInfo);
		SM_PFValue(((Unit *)m_caster)->SM[SMT_HITCHANCE][1],&hitchance,m_spellInfo);
		resistchance -= hitchance;
	}

	if(IsBinary(m_spellInfo))
	{ // need to apply resistance mitigation
		float mitigation = 1.0f - float (((Unit *)m_caster)->GetResistanceReducion(u_victim, m_spellInfo->School, 0.0f));
		resistchance = 100 - (100 - resistchance) * mitigation; // meaning hitchance * mitigation
	}

	if(resistchance < 1.0f)
		resistchance = 1.0f;

	if (m_spellInfo->attributes & ATTRIBUTES_IGNORE_INVULNERABILITY)
		resistchance = 0.0f;

	if(resistchance >= 100.0f)
		return SPELL_DID_HIT_RESIST;
	else
	{
		uint32 res;
		if(resistchance<=1.0)//resist chance >=1
			res =  (Rand(1.0f) ? SPELL_DID_HIT_RESIST : SPELL_DID_HIT_SUCCESS);
		else
			res =  (Rand(resistchance) ? SPELL_DID_HIT_RESIST : SPELL_DID_HIT_SUCCESS);

		if (res == SPELL_DID_HIT_SUCCESS) // proc handling. mb should be moved outside this function
			target->HandleProc(PROC_ON_HARMFULSPELL_LAND_VICTIM, ((Unit *)m_caster), m_spellInfo); // Ascent 5218
			//target->HandleProc(PROC_ON_SPELL_LAND_VICTIM,u_caster,m_spellInfo);	

		return res;
	}
 
}

//generate possible target list for a spell. Use as last resort since it is not acurate
//this function makes a rough estimation for possible target !
//!!!disabled parts that were not tested !!
/*void Spell::GenerateTargets(SpellCastTargets *store_buff)
{
	float r = m_spellInfo->base_range_or_radius_sqr;
	if( m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2] && u_caster)
	{
		SM_FFValue(u_caster->SM[SMT_RADIUS][0],&r,m_spellInfo);
		SM_PFValue(u_caster->SM[SMT_RADIUS][1],&r,m_spellInfo);
	}
	uint32 cur;
	for(uint32 i=0;i<3;i++)
		for(uint32 j=0;j<2;j++)
		{
			if(j==0)
				cur = m_spellInfo->EffectImplicitTargetA[i];
			else // if(j==1)
				cur = m_spellInfo->EffectImplicitTargetB[i];		
			switch(cur)
			{
				case EFF_TARGET_NONE:{
					//this is bad for us :(
					}break;
				case EFF_TARGET_SELF:{
						if(m_caster->IsUnit())
							store_buff->m_unitTarget = m_caster->GetGUID();
					}break;		
					// need more research
				case 4:{ // dono related to "Wandering Plague", "Spirit Steal", "Contagion of Rot", "Retching Plague" and "Copy of Wandering Plague"
					}break;			
				case EFF_TARGET_PET:
					{// Target: Pet
						if(p_caster && p_caster->GetSummon())
							store_buff->m_unitTarget = p_caster->GetSummon()->GetGUID();
					}break;
				case EFF_TARGET_SINGLE_ENEMY:// Single Target Enemy
				case 8: // related to Chess Move (DND), Firecrackers, Spotlight, aedm, Spice Mortar
				case EFF_TARGET_ALL_ENEMY_IN_AREA: // All Enemies in Area of Effect (TEST)
				case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT: // All Enemies in Area of Effect instant (e.g. Flamestrike)
				case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
				case EFF_TARGET_IN_FRONT_OF_CASTER:
				case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:// All Enemies in Area of Effect(Blizzard/Rain of Fire/volley) channeled
				case 31:// related to scripted effects
				case 53:// Target Area by Players CurrentSelection()
				case 54:// Targets in Front of the Caster
					{
						if( p_caster != NULL )
						{
							Unit *selected = p_caster->GetMapMgr()->GetUnit(p_caster->GetSelection());
							if(isAttackable(p_caster,selected,!(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)) && selected != p_caster )
								store_buff->m_unitTarget = p_caster->GetSelection();
						}
						else if( u_caster != NULL )
						{
							if(	u_caster->GetAIInterface()->GetNextTarget() &&
								isAttackable(u_caster,u_caster->GetAIInterface()->GetNextTarget(),!(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED)) &&
								u_caster->GetDistanceSq(u_caster->GetAIInterface()->GetNextTarget()) <= r)
							{
								store_buff->m_unitTarget = u_caster->GetAIInterface()->GetNextTarget()->GetGUID();
							}
							if(u_caster->GetAIInterface()->getAITargetsCount())
							{
								//try to get most hated creature
								TargetMap *m_aiTargets = u_caster->GetAIInterface()->GetAITargets();
								TargetMap::iterator itr;
								for(itr = m_aiTargets->begin(); itr != m_aiTargets->end();itr++)
								{
									if( //m_caster->GetMapMgr()->GetUnit(itr->first->GetGUID()) &&// itr->first->GetMapMgr() == m_caster->GetMapMgr() && 
										itr->first->isAlive() &&
										m_caster->GetDistanceSq(itr->first) <= r &&
										isAttackable(u_caster,itr->first,!(m_spellInfo->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED))
										)
									{
										store_buff->m_unitTarget=itr->first->GetGUID();
										break;
									}
								}
							}
						}
						//try to get a whatever target
						if(!store_buff->m_unitTarget)
						{
							store_buff->m_unitTarget=GetSinglePossibleEnemy(i);
						}
						//if we still couldn't get a target, check maybe we could use 
//						if(!store_buff->m_unitTarget)
//						{
//						}
					}break;
					// spells like 17278:Cannon Fire and 21117:Summon Son of Flame A
				case 17: // A single target at a xyz location or the target is a possition xyz
				case 18:// Land under caster.Maybe not correct
					{
						store_buff->m_srcX=m_caster->GetPositionX();
						store_buff->m_srcY=m_caster->GetPositionY();
						store_buff->m_srcZ=m_caster->GetPositionZ();
						store_buff->m_targetMask |= TARGET_FLAG_SOURCE_LOCATION;
					}break;
				case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
					{// All Party Members around the Caster in given range NOT RAID!			
						Player *p = p_caster;
						if( p == NULL)
						{
							if( static_cast<Creature *>( u_caster )->IsTotem() )
								p = static_cast<Player *>( static_cast<Creature *>( u_caster )->GetSummonOwner() );
						}
						if( p == NULL )
							break;

						if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),p,r))
						{
							store_buff->m_unitTarget = m_caster->GetGUID();
							break;
						}
						SubGroup * subgroup = p->GetGroup() ?
							p->GetGroup()->GetSubGroup(p->GetSubGroup()) : 0;

						if(subgroup)
						{
							p->GetGroup()->Lock();
							for(GroupMembersSet::iterator itr = subgroup->GetGroupMembersBegin(); itr != subgroup->GetGroupMembersEnd(); ++itr)
							{
								if(!(*itr)->m_loggedInPlayer || m_caster == (*itr)->m_loggedInPlayer) 
									continue;
								if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),(*itr)->m_loggedInPlayer,r))
								{
									store_buff->m_unitTarget = (*itr)->m_loggedInPlayer->GetGUID();
									break;
								}
							}
							p->GetGroup()->Unlock();
						}
					}break;
				case EFF_TARGET_SINGLE_FRIEND:
				case 45:// Chain,!!only for healing!! for chain lightning =6 
				case 57:// Targeted Party Member
					{// Single Target Friend
						if( p_caster != NULL )
						{
							if(isFriendly(p_caster,p_caster->GetMapMgr()->GetUnit(p_caster->GetSelection())))
								store_buff->m_unitTarget = p_caster->GetSelection();
							else store_buff->m_unitTarget = p_caster->GetGUID();
						}
						else if( u_caster != NULL )
						{
							if(u_caster->GetUInt64Value(UNIT_FIELD_CREATEDBY))
								store_buff->m_unitTarget = u_caster->GetUInt64Value(UNIT_FIELD_CREATEDBY);
							else store_buff->m_unitTarget = u_caster->GetGUID();
						}
						else store_buff->m_unitTarget=GetSinglePossibleFriend(i,r);			
					}break;
				case EFF_TARGET_GAMEOBJECT:
					{
						if(p_caster && p_caster->GetSelection())
							store_buff->m_unitTarget = p_caster->GetSelection();
					}break;
				case EFF_TARGET_DUEL: 
					{// Single Target Friend Used in Duel
						if(p_caster && p_caster->DuelingWith != NULL && p_caster->DuelingWith->isAlive() && IsInrange(p_caster,p_caster->DuelingWith,r))
							store_buff->m_unitTarget = p_caster->GetSelection();
					}break;
				case EFF_TARGET_GAMEOBJECT_ITEM:{// Gameobject/Item Target
						//shit
					}break;
				case 27:{ // target is owner of pet
					// please correct this if not correct does the caster variablen need a Pet caster variable?
						if(u_caster && u_caster->IsPet())
							store_buff->m_unitTarget = static_cast<Pet *>( u_caster )->GetPetOwner()->GetGUID();
					}break;
				case EFF_TARGET_MINION:
				case 73:
					{// Minion Target
						if(m_caster->GetUInt64Value(UNIT_FIELD_SUMMON) == 0)
							store_buff->m_unitTarget = m_caster->GetGUID();
						else store_buff->m_unitTarget = m_caster->GetUInt64Value(UNIT_FIELD_SUMMON);
					}break;
				case 33://Party members of totem, inside given range
				case EFF_TARGET_SINGLE_PARTY:// Single Target Party Member
				case EFF_TARGET_ALL_PARTY: // all Members of the targets party
					{
						Player *p=NULL;
						if( p_caster != NULL )
								p = p_caster;
						else if( u_caster && u_caster->GetTypeId() == TYPEID_UNIT && static_cast<Creature *>( u_caster )->IsTotem() )
								p = static_cast<Player *>( static_cast<Creature *>( u_caster )->GetSummonOwner() );
						if( p_caster != NULL )
						{
							if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),p,r))
							{
								store_buff->m_unitTarget = p->GetGUID();
								break;
							}
							SubGroup * pGroup = p_caster->GetGroup() ?
								p_caster->GetGroup()->GetSubGroup(p_caster->GetSubGroup()) : 0;

							if( pGroup )
							{
								p_caster->GetGroup()->Lock();
								for(GroupMembersSet::iterator itr = pGroup->GetGroupMembersBegin();
									itr != pGroup->GetGroupMembersEnd(); ++itr)
								{
									if(!(*itr)->m_loggedInPlayer || p == (*itr)->m_loggedInPlayer) 
										continue;
									if(IsInrange(m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),(*itr)->m_loggedInPlayer,r))
									{
										store_buff->m_unitTarget = (*itr)->m_loggedInPlayer->GetGUID();
										break;
									}
								}
								p_caster->GetGroup()->Unlock();
							}
						}
					}break;
				case 38:{//Dummy Target
					//have no idea
					}break;
				case EFF_TARGET_SELF_FISHING://Fishing
				case 46://Unknown Summon Atal'ai Skeleton
				case 47:// Portal
				case 52:	// Lightwells, etc
					{
						store_buff->m_unitTarget = m_caster->GetGUID();
					}break;
				case 40://Activate Object target(probably based on focus)
				case EFF_TARGET_TOTEM_EARTH:
				case EFF_TARGET_TOTEM_WATER:
				case EFF_TARGET_TOTEM_AIR:
				case EFF_TARGET_TOTEM_FIRE:// Totem
					{
						if( p_caster != NULL )
						{
							SummonPropertiesEntry* summonprop = dbcSummonProperties.LookupEntryForced( m_spellInfo->EffectMiscValueB[i] );
							if(!summonprop) return;
							uint32 slot = summonprop->slot;

							if(p_caster->m_summonslot[slot] != 0)
								store_buff->m_unitTarget = p_caster->m_summonslot[slot]->GetGUID();
						}
					}break;
				case 61:{ // targets with the same group/raid and the same class
					//shit again
				}break;
				case EFF_TARGET_ALL_FRIENDLY_IN_AREA:{

				}break;
					
			}//end switch
		}//end for
	if(store_buff->m_unitTarget)
		store_buff->m_targetMask |= TARGET_FLAG_UNIT;
	if(store_buff->m_srcX)
		store_buff->m_targetMask |= TARGET_FLAG_SOURCE_LOCATION;
	if(store_buff->m_destX)
		store_buff->m_targetMask |= TARGET_FLAG_DEST_LOCATION;
}//end function*/

uint8 Spell::prepare( SpellCastTargets * targets )
{
	// Point commun de la preparation au lancement
	// Est appelé apres le filtrage des receptions reseau
	// CMSG_USE_ITEM / CMSG_CAST_SPELL / CMSG_GAMEOBJ_USE

	if(!m_caster->IsInWorld())
	{
		sLog.outDebug("Object "I64FMT" is casting Spell ID %u while not in World", m_caster->GetGUID(), m_spellInfo->Id);
		delete this;
		return SPELL_FAILED_DONT_REPORT;
	}


	uint8 ccr;
	char tErreur[80];

	if(targets->m_target) // y'a t-il une cible en particulier (Debug - Brz)
	{
	 //Log.Warning( "Spell::prepare","Target %u [%s]", targets->m_target->GetEntry(),
	 //	(targets->m_target->GetObjectName())?(targets->m_target->GetObjectName()):"Sans nom");
	}
	else
	{
	 targets->m_target = NULL;
	 GenerateTargets( targets );
	}
	
	chaindamage = 0;
	
	// En commentaire, Test (Branruz)
	//if( p_caster && /*m_spellInfo->Id == __Hex____12 ||*/ // 51514 , etrange "Maléfice" sur wowhead: http://fr.wowhead.com/spell=51514 
	//	(m_spellInfo->NameHash == SPELL_HASH_ARCANE_SHOT || 
	//	m_spellInfo->NameHash == SPELL_HASH_MIND_FLAY) )
	//{
    //		targets->m_unitTarget = 0;
	//	targets->m_target = NULL;
	//	GenerateTargets( targets );
	//}

	m_targets = *targets;

	// Debug (Branruz)
	/*
	if(p_caster)
	  Log.Notice("[Spell::prepare]","(%s) Spell Preparation, Id %u",p_caster->GetName(),m_spellInfo->Id);
	else
      Log.Notice("[Spell::prepare]","Spell Preparation, Id %u ",m_spellInfo->Id);
    */

	if (m_targets.m_targetMask & (TARGET_FLAG_OBJECT | TARGET_FLAG_UNIT | 
		                          TARGET_FLAG_CORPSE | TARGET_FLAG_CORPSE2 | 
								  TARGET_FLAG_ITEM | TARGET_FLAG_TRADE_ITEM) && m_targets.m_target == NULL)
	{
		//Log.Warning("[Spell::prepare]","SPELL_FAILED_BAD_TARGETS");
		finish();
		return SPELL_FAILED_BAD_TARGETS;
	}

	if( !m_triggeredSpell && m_caster->IsPlayer() && ((Player *)m_caster)->CastTimeCheat )
		m_castTime = 0;
	else
	{
		m_castTime = GetCastTime( dbcSpellCastTime.LookupEntry( m_spellInfo->castingTimeIndex ) );

		if(m_caster->IsUnit())
		{
		 if(m_castTime)
		 {
		   if(m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2])
		   {
			SM_FIValue( ((Unit *)m_caster)->SM_FCastTime, (int32*)&m_castTime, m_spellInfo );
			SM_PIValue( ((Unit *)m_caster)->SM_PCastTime, (int32*)&m_castTime, m_spellInfo );
		   }
		   // handle MOD_CAST_TIME
		   m_castTime = float2int32( m_castTime * ((Unit *)m_caster)->GetFloatValue( UNIT_MOD_CAST_SPEED ) );
		 }
		}
	}

	uint8 forced_cancast_failure = 0;
	if( m_caster->IsPlayer() )
	{
		if( GetGameObjectTarget() || GetSpellProto()->Id == 21651) // __Opening____10 (Ouverture: 5m de portée, 8sec d'incantation)
		{
			if( ((Player *)m_caster)->IsStealth() )
			{
				((Player *)m_caster)->RemoveAura( ((Player *)m_caster)->m_stealth );
			}

			if( (GetSpellProto()->Effect[0] == SPELL_EFFECT_OPEN_LOCK ||
				GetSpellProto()->Effect[1] == SPELL_EFFECT_OPEN_LOCK ||
				GetSpellProto()->Effect[2] == SPELL_EFFECT_OPEN_LOCK) && 
				((Player *)m_caster)->m_bgFlagIneligible)
			{
				forced_cancast_failure = SPELL_FAILED_BAD_TARGETS;
			}
		}

		if( ((Player *)m_caster)->cannibalize )
		{
			sEventMgr.RemoveEvents( ((Player *)m_caster), EVENT_CANNIBALIZE );
			((Player *)m_caster)->SetUInt32Value( UNIT_NPC_EMOTESTATE, 0 );
			((Player *)m_caster)->cannibalize = false;
		}
	}

	//let us make sure cast_time is within decent range
	//this is a hax but there is no spell that has more then 10 minutes cast time

	if( m_castTime < 0 ) 	                m_castTime = 0;
	else if( m_castTime > (60 * 10 * 1000)) m_castTime = 60 * 10 * 1000; //we should limit cast time to 10 minutes right ?

	m_timer = m_castTime;

	//if( p_caster != NULL )
	//   m_castTime -= 100;	  // session update time


	if( !m_triggeredSpell && m_caster->IsPlayer() && ((Player *)m_caster)->CooldownCheat )
		((Player *)m_caster)->ClearCooldownForSpell( m_spellInfo->Id );

	m_spellState = SPELL_STATE_PREPARING;

	if( m_triggeredSpell )
	{
		//Log.Notice("[Spell::prepare]","SPELL_CANCAST_OK");
		cancastresult = SPELL_CANCAST_OK;
	}
	else
	{
		//Log.Notice("[Spell::prepare]","CanCast(false)");
		//cancastresult = CanCast(false);
		cancastresult = CanCastSpell();
	}

	if(cancastresult != SPELL_CANCAST_OK)
	{
	 //Log.Warning( "Spell::prepare","CanCast result: %u" , cancastresult );
	 
	 strncpy(tErreur,SpellCastErrorMsg(cancastresult),70);
	 if(tErreur) Log.Warning( "Spell::prepare","SpellFailure.h : %s",tErreur);
	 else        Log.Warning( "Spell::prepare","(Refer to SpellFailure.h to work out why.)");
	}

	ccr = cancastresult;

	if( forced_cancast_failure )
		cancastresult = forced_cancast_failure;

	if( cancastresult != SPELL_CANCAST_OK )
	{
		SendCastResult( cancastresult ); // Envoi du SpellFailure

		if( m_triggeredByAura )
		{
			SendChannelUpdate( 0 ); // Suppression barre d'incantation
			if(m_caster->IsUnit()) 
				((Unit *)m_caster)->RemoveAura( m_triggeredByAura );
		}
		else
		{
			// HACK, real problem is the way spells are handled
			// when a spell is channeling and a new spell is casted
			// that is a channeling spell, but not triggert by a aura
			// the channel bar/spell is bugged
			if( ((Unit *)m_caster)->GetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT) > 0 && ((Unit *)m_caster)->GetCurrentSpell() )
			{
				((Unit *)m_caster)->GetCurrentSpell()->cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
				SendChannelUpdate( 0 ); // Suppression barre d'incantation
				cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
				//Log.Warning( "Spell::prepare","UNIT_FIELD_CHANNEL_OBJECT > 0" ); // Debug (Branruz)
				return ccr;
			}
		}
		finish();
		//Log.Warning( "Spell::prepare","Finish(), CanCast result: %u" , ccr );
		return ccr;
	}
	else if( !m_triggeredSpell )
	{
		//Log.Warning( "Spell::prepare","Not triggered");

		if( !HasPower() )
		{
			SendCastResult(SPELL_FAILED_NO_POWER);
			// in case we're out of sync
			//if( p_caster ) p_caster->SendPowerUpdate(); // Non non et non..... plus valable en 332 .....(Branruz)

			//Log.Warning( "Spell::prepare","SPELL_FAILED_NO_POWER" ); // Debug (Branruz)
			return SPELL_FAILED_NO_POWER;
		}
		SendSpellStart();

		// start cooldown handler
		if( m_caster->IsPlayer() && !((Player *)m_caster)->CastTimeCheat )
		{
			AddStartCooldown();
		}

		if( !m_caster->IsItem() )
		{
			if( m_caster->IsPlayer() && m_timer > 0 )
				((Player *)m_caster)->setAttackTimer(m_timer + 1000, false); // (Branruz)
			//	((Player *)m_caster)->delayAttackTimer( m_timer + 1000 );
			
		}

		// aura state removal
		if( m_spellInfo->casterAuraState && m_spellInfo->casterAuraState != AURASTATE_FLAG_JUDGEMENT )
          ((Unit *)m_caster)->RemoveFlag( UNIT_FIELD_AURASTATE, m_spellInfo->casterAuraState ); 
	}

	m_spellState = SPELL_STATE_PREPARING;

	// Debug (Branruz)
	//Log.Warning("[Spell::prepare]","SPELL_STATE_PREPARING");

	//instant cast(or triggered) 
	if((m_caster->IsUnit()) && ( m_castTime > 0 || m_spellInfo->channelInterruptFlags ) && !m_triggeredSpell )	
	{
		m_castPositionX = m_caster->GetPositionX();
		m_castPositionY = m_caster->GetPositionY();
		m_castPositionZ = m_caster->GetPositionZ();
	

		//Log.Warning("[Spell::prepare]","Lancement du spell %u ( Caster: %d )",this->GetSpellProto()->Id,((Unit *)m_caster)->GetEntry());   // Debug (Branruz)
		((Unit *)m_caster)->castSpell(this);
		//((Unit *)m_caster)->CastSpell(m_targets.m_target,this->GetSpellProto()->Id,false); // Not triggered, Test (Branruz)
	}
	else // and not channeling
	{
	 
	 //if(m_caster) Log.Warning("[Spell::prepare]","Spell Caster : %d",m_caster->GetEntry());
	 //else         Log.Warning("[Spell::prepare]","Spell Caster : NULL");

	 //if(m_targets.m_target) Log.Warning("[Spell::prepare]","Cible        : %d",m_targets.m_target->GetEntry());
	 //else                   Log.Warning("[Spell::prepare]","Cible        : NULL");

	 //Log.Warning("[Spell::prepare]","cast( false )");   // Debug (Branruz)
     

	 cast( false ); // False: Force le SPELL_CANCAST_OK
	}

	//Log.Warning("[Spell::prepare]","return: %d",ccr);   // Debug (Branruz)

	return ccr;
}

void Spell::cancel(uint8 result)
{
	SendInterrupted(0);
	SendCastResult(result); // SPELL_FAILED_INTERRUPTED

	if(m_spellState == SPELL_STATE_CASTING)
	{
		if( m_caster->IsUnit() )
			((Unit *)m_caster)->RemoveAura(m_spellInfo->Id);
	
		if(m_timer > 0 || m_Delayed)
		{
			if(m_caster->IsPlayer() && ((Player *)m_caster)->IsInWorld())
			{
				Unit *pTarget = ((Player *)m_caster)->GetMapMgr()->GetUnit(((Player *)m_caster)->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT));
				if(!pTarget)
					pTarget = ((Player *)m_caster)->GetMapMgr()->GetUnit(((Player *)m_caster)->GetSelection());
				  
				if(pTarget)
				{
					pTarget->RemoveAura(m_spellInfo->Id, m_caster->GetGUID());
				}
				if(m_AreaAura)//remove of blizz and shit like this
				{
					DynamicObject* dynObj = new (m_caster->GetMapMgr()->GetDynamicObject(m_caster->GetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT))) DynamicObject(NULL, NULL);
					if(dynObj)
					{
						dynObj->RemoveFromWorld(true);
						delete dynObj;
						dynObj = NULL;
						//dynObj->m_aliveDuration = 1;
					}
				}

				if(((Player *)m_caster)->GetSummonedObject())
				{
					if(((Player *)m_caster)->GetSummonedObject()->IsInWorld())
						((Player *)m_caster)->GetSummonedObject()->RemoveFromWorld(true);
					// for now..
					//ASSERT(p_caster->GetSummonedObject()->GetTypeId() == TYPEID_GAMEOBJECT);
					if(((Player *)m_caster)->GetSummonedObject()->GetTypeId() == TYPEID_GAMEOBJECT)
					{
					//delete ((GameObject*)(p_caster->GetSummonedObject()));
					 ((Player *)m_caster)->GetSummonedObject()->Delete();
					 ((Player *)m_caster)->SetSummonedObject(NULL);
					}
					else
					{
						Log.Error("[Spell::cancel()]","GetSummonedObject (%u) different de TYPEID_GAMEOBJECT (%u)",
                                ((Player *)m_caster)->GetSummonedObject()->GetTypeId(),TYPEID_GAMEOBJECT);
					}
				}
				if (m_timer > 0 )
					((Player *)m_caster)->setAttackTimer(1000, false);
				    //((Player *)m_caster)->delayAttackTimer(-m_timer);
			 }
		}
		SendChannelUpdate(0); // Suppression Barre d'incantation 
	}
	// Ensure the item gets consumed once the channel has started
	if (m_owner)
	{
		if (m_timer > 0 && m_owner->IsItem())
			m_ForceConsumption = true;
	}

	//m_spellState = SPELL_STATE_FINISHED;

	// prevent memory corruption. free it up later.
	// if this is true it means we are currently in the cast() function somewhere else down the stack
	// (recursive spells) and we don't wanna have this class delete'd when we return to it.
	// at the end of cast() it will get freed anyway.
	if( !m_isCasting ) finish();
}

void Spell::AddCooldown()
{
	// Log.Warning("AddCooldown","Attention: Inoperant....");
	/* Le caster est un item mais on considère que l'on ajoute le cooldown au player*/
	if( m_caster->IsItem())
		((Player *)m_caster)->Cooldown_Add( m_spellInfo, ((Item *)m_caster) );
		
}

void Spell::AddStartCooldown()
{
	if( m_caster->IsPlayer() )
		((Player *)m_caster)->Cooldown_AddStart( m_spellInfo );
}

void Spell::cast(bool check)
{
	if( duelSpell && (
		( m_caster->IsPlayer() && ((Player *)m_caster)->GetDuelState() != DUEL_STATE_STARTED ) ||
		( m_caster->IsUnit() && ((Unit *)m_caster)->IsPet() && 
		  static_cast< Pet* >( m_caster )->GetPetOwner() && 
		  static_cast< Pet* >( m_caster )->GetPetOwner()->IsPlayer() && 
		  static_cast<Player*>(static_cast< Pet* >( m_caster )->GetPetOwner())->GetDuelState() != DUEL_STATE_STARTED ) ) )
	{
		// Can't cast that!
		SendInterrupted( SPELL_FAILED_TARGET_FRIENDLY );
		finish();
		return;
	}

	/*Log.Success("Cast","Check: %s",check?"Oui":"Non");
	if(u_caster != NULL)
	{
		Log.Success("Cast","%s tente de lancer le spell %u",u_caster->GetObjectName(),this->GetSpellProto()->Id);
	}
	else if(p_caster != NULL)
	{
		Log.Success("Cast","%s tente de lancer le spell %u",p_caster->GetObjectName(),this->GetSpellProto()->Id);
	}*/

    if(check) cancastresult = CanCast(true);
	else cancastresult = SPELL_CANCAST_OK;  
    //-----------------------------------

	if(cancastresult == SPELL_CANCAST_OK)
	{
		//if (p_caster && !m_triggeredSpell && p_caster->IsInWorld() && GET_TYPE_FROM_GUID(m_targets.m_target->GetGUID())==HIGHGUID_TYPE_UNIT)
		if (m_caster->IsPlayer() && m_targets.m_target) // => sinon Spell sans cible direct (Area of Effect) exemple Spell 1725 (Distraction)
		{                                               //    La gestion de l'AoE devra etre faite via le SpellEffect (et/ou l'AuraEffect) - Brz
			if(!m_triggeredSpell && ((Player *)m_caster)->IsInWorld() && GET_TYPE_FROM_GUID(m_targets.m_target->GetGUID())==HIGHGUID_TYPE_UNIT)
		    {
			 sQuestMgr.OnPlayerCast(((Player *)m_caster), m_spellInfo->Id, m_targets.m_target->GetGUID());
			}
		}

		//----------------------------------------------------
		// trigger on next attack
		if( m_spellInfo->attributes & ATTRIBUTE_ON_NEXT_ATTACK && !m_triggeredSpell )
		{
			// check power
			if(!HasPower())
			{
				SendInterrupted(SPELL_FAILED_NO_POWER);
				SendCastResult(SPELL_FAILED_NO_POWER);
				finish();
				return; 
			}
			else
			{
				// this is the actual spell cast
				if(!TakePower())	// shouldn't happen
				{
					SendInterrupted(SPELL_FAILED_NO_POWER);
					SendCastResult(SPELL_FAILED_NO_POWER);// Test
					finish();
					return;
				}
			}
			
			m_isCasting = false;
			//SendCastResult(cancastresult); // Ne sert a rien du tout ici (Branruz)

			if( m_caster->IsUnit() ) 
			{
				// we're much better to remove this here, because otherwise spells that change powers etc,
			    // don't get applied.
				((Unit *)m_caster)->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_CAST_SPELL, m_spellInfo->Id);
				((Unit *)m_caster)->SetOnMeleeSpell(m_spellInfo->Id, extra_cast_number);
			}

			finish();
			return;
		}
		else
		{
			if(!TakePower() && !m_triggeredSpell) //not enough mana
			{
				//sLog.outDebug("Spell::Not Enough Mana");
				SendInterrupted(SPELL_FAILED_NO_POWER);
				SendCastResult(SPELL_FAILED_NO_POWER);
				finish();
				return; 
			}
		}
		// End "trigger on next attack"
		//----------------------------------------------------
		
		for(uint32 i=0;i<3;i++)
        {
			if((m_targets.m_targetMask != TARGET_FLAG_DEST_LOCATION) && (m_spellInfo->Effect[i] != 0))
			{
				//Log.Warning("FillTargetMap","Call01");
				//if(m_spellInfo->spell_signature != _SPELL_SIGNATURE_) 
				//	Log.Error("FillTargetMap","FillTargetMap Call01: SpellInfo leak! (Index: %u)",i);

				FillTargetMap(i);
			}
        }

		if (!m_filledTargetMap)
		{
			for(uint32 i=0;i<3;i++)
			{
				if((m_targets.m_targetMask != TARGET_FLAG_DEST_LOCATION) && (m_spellInfo->Effect[i] != 0))
				{
					//Log.Warning("FillTargetMap","Call02");
					FillTargetMap(i);
				}
			}
		}

		SendCastResult(cancastresult);
		if(cancastresult != SPELL_CANCAST_OK)
		{
			finish();
			return;
		}

		m_isCasting = true;		

		// start cooldown handler
		if(!m_triggeredSpell) 
			AddCooldown();

		if( m_caster->IsPlayer() && !((Player *)m_caster)->CastTimeCheat && !m_triggeredSpell )
			AddStartCooldown();
		
		if( !m_caster->IsItem() )
		{
			if( m_caster->IsPlayer() )
			{
				if(m_timer > 0 && !m_triggeredSpell )
			                   ((Player *)m_caster)->setAttackTimer(m_timer + 1000, false);
			}
		}

		// aura state removal
		if( m_spellInfo->casterAuraState )
		{
			if(m_caster->IsUnit()) ((Unit *)m_caster)->RemoveFlag( UNIT_FIELD_AURASTATE, m_spellInfo->casterAuraState );
		}

		

		if (m_caster->IsUnit())
		{
			if (((Unit *)m_caster)->IsStealth() && !(m_spellInfo->attributesEx & ATTRIBUTESEX_NOT_BREAK_STEALTH) && !m_triggeredSpell)
				((Unit *)m_caster)->RemoveStealth();
		}


		if( m_caster->IsPlayer()) //-------------------------------
		{
			if( m_spellInfo->NameHash == SPELL_HASH_SLAM)
			{
				/* slam - reset attack timer */
				((Player *)m_caster)->setAttackTimer( 0, true );
				((Player *)m_caster)->setAttackTimer( 0, false );
			}

			else if( m_spellInfo->NameHash == SPELL_HASH_VICTORY_RUSH ) // Test
			{
				((Player *)m_caster)->RemoveFlag(UNIT_FIELD_AURASTATE,AURASTATE_FLAG_LASTKILLWITHHONOR);
			}            

			// Arathi Basin opening spell, remove stealth, invisibility, etc. 
			// hacky but haven't found a better way that works
			// Note: Same stuff but for picking flags is over AddAura
			if (((Player *)m_caster)->m_bg && m_spellInfo->Id == 21651)
			{
				((Player *)m_caster)->RemoveStealth();
				((Player *)m_caster)->RemoveInvisibility();
				((Player *)m_caster)->RemoveAllAuraByNameHash(SPELL_HASH_ICE_BLOCK);
				((Player *)m_caster)->RemoveAllAuraByNameHash(SPELL_HASH_DIVINE_SHIELD);
				((Player *)m_caster)->RemoveAllAuraByNameHash(SPELL_HASH_BLESSING_OF_PROTECTION);
			}

			
			if( m_spellInfo->NameHash == SPELL_HASH_HOLY_LIGHT || m_spellInfo->NameHash == SPELL_HASH_FLASH_OF_LIGHT)
			{                               
                if( ((Player *)m_caster)->HasAura( __Infusion_of_Light ) ) // 53672 
				    ((Player *)m_caster)->RemoveAura(__Infusion_of_Light); // 53672
                if( ((Player *)m_caster)->HasAura( __Infusion_of_Light_Rank_2____0 ))   // 54149
				    ((Player *)m_caster)->RemoveAura(__Infusion_of_Light_Rank_2____0 ); // 54149
			}


			// Ne devrait-il pas être fait dans la gestion du BG ? (Branruz)
            /*
			if(p_caster->m_bg) // Test (HearthStone)
			{
				// SOTA Gameobject spells
				if (p_caster->m_bg->GetType() == BATTLEGROUND_STRAND_OF_THE_ANCIENTS)
				{
					StrandOfTheAncients * sota = (StrandOfTheAncients *)p_caster->m_bg;
					// Transporter platforms
					//if (m_spellInfo->Id == 54640) // __Teleport___34
					//	sota->OnPlatformTeleport(p_caster); <= A finir (Branruz)
				}
				// warsong gulch & eye of the storm flag pickup check
				// also includes check for trying to cast stealth/etc while you have the flag
				switch(m_spellInfo->Id)
				{
					case __Opening____10 : // 21651
						// Arathi Basin opening spell, remove stealth, invisibility, etc.
						p_caster->RemoveStealth();
						//p_caster->RemoveInvisibility();
						p_caster->RemoveAllAuraByNameHash(SPELL_HASH_DIVINE_SHIELD);
						p_caster->RemoveAllAuraByNameHash(SPELL_HASH_DIVINE_PROTECTION);
						p_caster->RemoveAllAuraByNameHash(SPELL_HASH_BLESSING_OF_PROTECTION);
						break;
					case __Warsong_Flag :    // 23333
					case __Silverwing_Flag:  // 23335
					case __Netherstorm_Flag: // 34976
						// if we're picking up the flag remove the buffs
						p_caster->RemoveStealth();
						//p_caster->RemoveInvisibility();
						p_caster->RemoveAllAuraByNameHash(SPELL_HASH_DIVINE_SHIELD);
						p_caster->RemoveAllAuraByNameHash(SPELL_HASH_DIVINE_PROTECTION);
						p_caster->RemoveAllAuraByNameHash(SPELL_HASH_BLESSING_OF_PROTECTION);
						break;
					// cases for stealth - etc
					// we can cast the spell, but we drop the flag (if we have it)
					// case 20580:		// Unknown spell, n'existe pas dans spell.dbc
					// case 1020:		// Unknown spell, n'existe pas dans spell.dbc
					// case 5573:		// Unknown spell, n'existe pas dans spell.dbc
					case __Stealth        : // 1784 _Rank_1
					case __Stealth_Rank_2 : // 1785 
					case __Stealth_Rank_3 : // 1786 
					case __Stealth_Rank_4 : // 1787 
					case __Prowl_Rank_1   : // 5215 
					case __Prowl_Rank_2   : // 6783 
					case __Prowl_Rank_3   : // 9913 
					case __Divine_Protection : // 498 
					case __Divine_Shield : // 642 , n'existe pas dans spell.dbc
					case __Hand_of_Protection_Rank_1 : // 1022 (ex blessing of protection)
					case __Hand_of_Protection_Rank_2 : // 5599
					case __Hand_of_Protection_Rank_3 : // 10278
					case __Vanish_Rank_1             : // 1856
					case __Vanish_Rank_2             : // 1857
					case __Vanish_Rank_3____0        : // 26889 f*ck blizz
					case __Ice_Block____2            : // 45438 f*ck blizz
					case __Shadowmeld_Racial         : // 58984
					case __Petrification             : // 17624 -> http://www.wowhead.com/?spell=17624
					case __Invisibility              : // 66
						if(p_caster->m_bg->GetType() == BATTLEGROUND_WARSONG_GULCH)
						{
							if(p_caster->GetTeam() == 0)
								p_caster->RemoveAura(__Warsong_Flag); // 23333 ally player drop horde flag if they have it
							else
								p_caster->RemoveAura(__Silverwing_Flag); // 23335 horde player drop ally flag if they have it
						}
						else if(p_caster->m_bg->GetType() == BATTLEGROUND_EYE_OF_THE_STORM)
							p_caster->RemoveAura(__Netherstorm_Flag); // 34976 drop the flag
						break;
				}
			}//-------- BG
			*/
			
		} //-------------------------------

		/*SpellExtraInfo* sp = objmgr.GetSpellExtraData(m_spellInfo->Id);
		if(sp)
		{
			Unit *Target = objmgr.GetUnit(m_targets.m_unitTarget);
			if(Target)
				Target->RemoveBySpecialType(sp->specialtype, p_caster->GetGUID());
		}*/

		if(!(m_spellInfo->attributes & ATTRIBUTE_ON_NEXT_ATTACK  && !m_triggeredSpell))//on next attack
		{
			SendSpellGo();

			//******************** SHOOT SPELLS ***********************
			//* Flags are now 1,4,19,22 (4718610) //0x480012
                                                  //0x008000
			if ((m_spellInfo->attributesExC & ATTRIBUTESEXC_PLAYER_RANGED_SPELLS) && m_caster->IsPlayer() && m_caster->IsInWorld())
			{
                /// Part of this function contains a hack fix
                /// hack fix for shoot spells, should be some other resource for it
                //p_caster->SendSpellCoolDown(m_spellInfo->Id, m_spellInfo->recoveryTime ? m_spellInfo->recoveryTime : 2300);
				WorldPacket data(SMSG_SPELL_COOLDOWN, 14);
				data << m_spellInfo->Id;
				data << ((Player *)m_caster)->GetNewGUID();
				data << uint32(m_spellInfo->recoveryTime ? m_spellInfo->recoveryTime : 2300);
				((Player *)m_caster)->GetSession()->SendPacket(&data);
			}

			//---------------------------------------------------------
			// Barre d'invocation
			// Note: [Warlock] Immolation Aura somehow has these flags, but it's not channeled
			if( (m_spellInfo->channelInterruptFlags != 0) && !m_triggeredSpell && (m_spellInfo->Id != __Immolation_Aura_Demon) ) // 50589
			{
				/*
				Channeled spells are handled a little differently. The five second rule starts when the spell's channeling starts; 
				i.e. when you pay the mana for it.
				The rule continues for at least five seconds, and longer if the spell is channeled for more than five seconds. For example,
				Mind Flay channels for 3 seconds and interrupts your regeneration for 5 seconds, while Tranquility channels for 10 seconds
				and interrupts your regeneration for the full 10 seconds.
				*/

				uint32 channelDuration = GetDuration(INVOC_DURATION);
				m_spellState = SPELL_STATE_CASTING;
				SendChannelStart(channelDuration); // Barre d'incantation

				if( m_caster->IsPlayer() && ((Player *)m_caster)->GetSelection() )
				{
					//Use channel interrupt flags here
					if(m_targets.m_targetMask == TARGET_FLAG_DEST_LOCATION || m_targets.m_targetMask == TARGET_FLAG_SOURCE_LOCATION)
						((Unit *)m_caster)->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT,((Player *)m_caster)->GetSelection());
					else if(((Player *)m_caster)->GetSelection() == m_caster->GetGUID())
					{
						if(((Player *)m_caster)->GetSummon())
							((Unit *)m_caster)->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT,((Player *)m_caster)->GetSummon()->GetGUID());
						else if(m_targets.m_target)
							((Unit *)m_caster)->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, m_targets.m_target->GetGUID());
						else
							((Unit *)m_caster)->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT,((Player *)m_caster)->GetSelection());	
					}
					else
					{
						if(((Player *)m_caster)->GetSelection())
							((Unit *)m_caster)->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT,((Player *)m_caster)->GetSelection());
						else if(((Player *)m_caster)->GetSummon())
							((Unit *)m_caster)->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT,((Player *)m_caster)->GetSummon()->GetGUID());
						else if(m_targets.m_target)
							((Unit *)m_caster)->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, m_targets.m_target->GetGUID());
						else
						{
							m_isCasting = false;
							cancel(SPELL_FAILED_INTERRUPTED);
							return;
						}
					}
				}

				if(((Unit *)m_caster) && ((Unit *)m_caster)->GetPowerType()==POWER_TYPE_MANA)
				{
					if(channelDuration <= 5000)
						((Unit *)m_caster)->DelayPowerRegeneration(5000);
					else
						((Unit *)m_caster)->DelayPowerRegeneration(channelDuration);
				}

				if( ((Unit *)m_caster) != NULL )
					((Unit *)m_caster)->SetCurrentSpell(this);
			}

            // if the spell is not reflected
		    SpellTargetList::iterator itr = m_targetList.begin();
		    uint32 x;
		    bool effects_done[3];
		    effects_done[0]=effects_done[1]=effects_done[2] = false;

		    for(; itr != m_targetList.end(); ++itr)
		    {
				if( itr->HitResult != SPELL_DID_HIT_SUCCESS ) continue;

				// set target pointers
				_SetTargets(itr->Guid);

				// call effect handlers
				for( x = 0; x < 3; ++x ) 
				{
					switch (m_spellInfo->Effect[x])
					{
						//Don't handle effect now
						case SPELL_EFFECT_SUMMON:
			   			{
							effects_done[x] = false;
							continue;
						}break;

						default:
						{
							if(itr->EffectMask & (1 << x))
							{
								//NewHandleEffects(x);
								HandleEffects(NULL, x, false);
								effects_done[x] = true;
							}
						}break;
					}
			 }
			 // handle the rest of shit
			 if( unitTarget != NULL )
			 {
				// aura state
				if( m_spellInfo->targetAuraState )
					unitTarget->RemoveFlag(UNIT_FIELD_AURASTATE, uint32(1) << (m_spellInfo->targetAuraState - 1) );

				// proc!
				if(!m_triggeredSpell)
				{
					if( m_caster->IsUnit() && ((Unit *)m_caster)->IsInWorld() )
					{
							((Unit *)m_caster)->HandleProc(PROC_ON_HARMFULSPELL_LAND | PROC_ON_CAST_SPELL, unitTarget, m_spellInfo);
							((Unit *)m_caster)->m_procCounter = 0; //this is required for to be able to count the depth of procs (though i have no idea where/why we use proc on proc)
					}
				}
				if( unitTarget != NULL && unitTarget->IsInWorld() )
				{
					unitTarget->HandleProc(PROC_ON_HEALSPELL_LAND_VICTIM , ((Unit *)m_caster), m_spellInfo);
					unitTarget->m_procCounter = 0; //this is required for to be able to count the depth of procs (though i have no idea where/why we use proc on proc)
				}
			 }
			}

		    //Handle remaining effects for which we did not find targets.
		    for( x = 0; x < 3; ++x )
			{
				if(m_spellInfo->Effect[x])
				{
					/*if(!CanHandleSpellEffect(x, m_spellInfo->NameHash))
							continue;*/

					if(!effects_done[x])
					{
						switch (m_spellInfo->Effect[x])
						{
							// Target ourself for these effects
							case SPELL_EFFECT_TRIGGER_SPELL:
							case SPELL_EFFECT_JUMP_MOVE:
							case SPELL_EFFECT_SUMMON:
							case SPELL_EFFECT_TRIGGER_MISSILE:
							case SPELL_EFFECT_APPLY_GLYPH:
							{
								/*_SetTargets(m_caster->GetGUID());
								NewHandleEffects(x);*/
								HandleEffects(m_caster, x, false);
							}break;

							// No Target required for these effects
							case SPELL_EFFECT_PERSISTENT_AREA_AURA:
							case SPELL_EFFECT_APPLY_AREA_AURA:
							{
								//NewHandleEffects(x);
								HandleEffects(NULL, x, false);
							}break;
						}
					}
				}	
			}

		    // don't call HandleAddAura unless we actually have auras... - Burlex
			/* Si le player fait parti de la liste, les auras seront lancées une 2ieme fois  - Branruz
			// Note Randdrick : ****** Obsolète suite changement de gestion des Auras *******
			// A ce stade  l'aura a déjà été lancée par un Event. Si une autre aura est lancée, l'event s'en sera chargé.
		    if( m_spellInfo->EffectApplyAuraName[0] != SPELL_AURA_NONE || 
				m_spellInfo->EffectApplyAuraName[1] != SPELL_AURA_NONE ||
		        m_spellInfo->EffectApplyAuraName[2] != SPELL_AURA_NONE)
			{			 
				itr = m_targetList.begin();
				for(; itr != m_targetList.end(); ++itr)
				{
					//if( m_caster->GetGUID() == itr->Guid) continue; 
					if( itr->HitResult != SPELL_DID_HIT_SUCCESS ) continue;
					NewHandleAddAura(itr->Guid);
				}
			}*/

			SpellTargetMap::iterator i;
			
            bool isDuelEffect = false;
			//uint32 spellid = m_spellInfo->Id;

			//call script before doing effects/aura
			if (m_spellScript != NULL) m_spellScript->OnCast();
				
			//procfncs :D
			std::vector<void*> data;
			data.push_back((void*)this);

			if (m_caster->IsUnit()) ((Unit *)m_caster)->HandleProcFnc(SPELLFNC_PROC_ON_CAST_SPELL, &data);

			//----------------------------------------------------
            // cant reflect if its a dest location
			if (m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
			{
				float dist=0.0f;

				//Log.Warning("Cast","TARGET_FLAG_DEST_LOCATION");

				
				if (m_spellInfo->speed <=0) HandleDestLocationHit();
				else // Le pnj se deplace en lancant son spell
				{
					dist = m_caster->CalcDistance(m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ);
					float time = ((dist*1000.0f)/m_spellInfo->speed);
					//Log.Warning("Cast","Declenchement dans %f msec",time);
					if(time <= 100) HandleDestLocationHit();
					else sEventMgr.AddEvent(this, &Spell::HandleDestLocationHit, EVENT_SPELL_HIT, float2int32(time), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				}
			}
			else
			{
				//do we already have 
				for(uint32 x=0;x<3;x++)
				{
					// check if we actualy have a effect
					if( m_spellInfo->Effect[x])
					{
						if (m_orderedObjects.size() == 0) // Pas de cible multiple ?
						{
							if(m_targets.m_target) // Une seule cible existe
							{
								this->_SetTargets(m_targets.m_target->GetGUID()); // On designe la cible, pas le caster (Branruz)
								HandleCastEffects(m_targets.m_target, x);  
							}
							else
							{ // Seulement AoE ou AoF ?? (Branruz)
								Log.Warning("[Spell::cast]","HandleCastEffects: (AoE) Cible = NULL, Pos:%u, Index:%u",x,m_spellInfo->Effect[x]);
								HandleCastEffects(NULL, x);
							}
						}

						isDuelEffect = isDuelEffect ||  (m_spellInfo->Effect[x] == SPELL_EFFECT_DUEL);

                        if(m_orderedObjects.size()) // Cible multiple
						{
							for (std::vector<Object*>::iterator itr=m_orderedObjects.begin(); itr!=m_orderedObjects.end(); ++itr)
							{
								i = m_spellTargets.find(*itr);
								if (i != m_spellTargets.end() && i->second.HasEffect[x])
								{
									//Log.Notice("[Spell::cast]","m_spellTargets envoyé %u - %u);",x,m_spellInfo->Effect[x]);
									HandleCastEffects(i->first, x);
								}
								// Capt: The way this is done is NOT GOOD. Target code should be redone.
								else if (m_spellInfo->Effect[x] == SPELL_EFFECT_TELEPORT_UNITS)
								{
									//Log.Notice("[Spell::cast]","SPELL_EFFECT_TELEPORT_UNITS envoyé %u - %u);",x,m_spellInfo->Effect[x]);
									HandleCastEffects(m_caster,x);
								}
					     		else if (m_spellInfo->Effect[x] == SPELL_EFFECT_SUMMON_WILD)
								{
									//Log.Notice("[Spell::cast]","SPELL_EFFECT_SUMMON_WILD envoyé %u - %u);",x,m_spellInfo->Effect[x]);
									HandleCastEffects(m_caster,x);
								} 
							}
						}

						//Handle reflected stuff
						for (i=m_spellTargets.begin(); i!=m_spellTargets.end(); ++i)
						{
							if (i->second.TargetModType == SPELL_DID_HIT_REFLECT && i->second.ExtendedTargetModType == SPELL_DID_HIT_SUCCESS)
							{
								//Log.Notice("[Spell::cast]","SPELL_DID_HIT_REFLECT envoyé");
								HandleCastEffects(i->first, x, true);
							}
						}
					}
				}

				// spells that proc on spell cast, some talents
				if(m_caster->IsPlayer() && m_caster->IsInWorld())
				{
					for(i=m_spellTargets.begin(); i!=m_spellTargets.end(); ++i)
					{
						if (!i->first->IsUnit() || i->second.TargetModType != 0)
							continue;

						Unit* Target=static_cast<Unit*>(i->first);

						if(!m_triggeredSpell || m_spellInfo->NameHash == SPELL_HASH_DEEP_WOUND)
						{
							((Player *)m_caster)->HandleProc(PROC_ON_HARMFULSPELL_LAND | PROC_ON_CAST_SPELL,Target, m_spellInfo);
							//((Player *)m_caster)->HandleProc(PROC_ON_HEALSPELL_LAND_VICTIM | PROC_ON_CAST_SPELL,Target, m_spellInfo);
							Target->HandleProc(PROC_ON_HEALSPELL_LAND_VICTIM | PROC_ON_CAST_SPELL,((Player *)m_caster),m_spellInfo);
							((Player *)m_caster)->m_procCounter = 0; //this is required for to be able to count the depth of procs (though i have no idea where/why we use proc on proc)
						}
						Target->RemoveFlag(UNIT_FIELD_AURASTATE, m_spellInfo->targetAuraState);
					}
				}
			} //---------------------------------------------

			if( m_caster->IsUnit() ) ((Unit *)m_caster)->SendPowerUpdate();

			// we're much better to remove this here, because otherwise spells that change powers etc,
			// don't get applied.

			if(m_caster->IsUnit() && !m_triggeredSpell && !m_triggeredByAura)
				((Unit *)m_caster)->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_CAST_SPELL, m_spellInfo->Id);
				
			//i don't think that's good idea
			/* En test (en commentaire - Branruz)
			switch( m_spellInfo->NameHash )
			{
			case SPELL_HASH_FIREBALL:
				{
					if( p_caster )
						p_caster->RemoveAura( __Fireball_ ); //57761
				}break;
			case SPELL_HASH_PYROBLAST:
				{
					if( p_caster )
						p_caster->RemoveAura( __Hot_Streak ); //48108
				}break;
			case SPELL_HASH_GLYPH_OF_ICE_BLOCK:
				{
					if( p_caster && p_caster->HasDummyAura(SPELL_HASH_GLYPH_OF_ICE_BLOCK) )
					{
						//frost nova
						p_caster->ClearCooldownForSpell( 122 );
						p_caster->ClearCooldownForSpell( 865 );
						p_caster->ClearCooldownForSpell( 6131 );
						p_caster->ClearCooldownForSpell( 10230 );
						p_caster->ClearCooldownForSpell( 27088 );
						p_caster->ClearCooldownForSpell( 42917 );
					}
				}break;
			case SPELL_HASH_GLYPH_OF_ICY_VEINS:
				{
					if( p_caster && p_caster->HasDummyAura(SPELL_HASH_GLYPH_OF_ICY_VEINS) )
					{
						Aura *pAura;
						for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; ++i)
						{
							pAura = p_caster->m_auras[i];
							if( pAura != NULL && !pAura->IsPositive() )
							{
								for(uint32 j = 0; j < 3; ++j)
								{
									if( pAura->GetSpellProto()->EffectApplyAuraName[j] == SPELL_AURA_MOD_DECREASE_SPEED || 
										pAura->GetSpellProto()->EffectApplyAuraName[j] == SPELL_AURA_MOD_CASTING_SPEED )
									{
										pAura->Remove();
										break;
									}
								}
							}
						}
					}
				}break;
			case SPELL_HASH_HAND_OF_FREEDOM:
				{
					if( p_caster && p_caster->HasDummyAura(SPELL_HASH_DIVINE_PURPOSE) )
					{
						if( Rand( p_caster->GetDummyAura(SPELL_HASH_DIVINE_PURPOSE)->RankNumber * 50 ) )
						{
							Unit *u_target = p_caster->GetMapMgr()->GetUnit(m_targets.m_target->GetGUID());
							if( u_target )
							{
								Aura *pAura;
								for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; ++i)
								{
									pAura = u_target->m_auras[i];
									if( pAura != NULL )
									{
										for(uint32 j = 0; j < 3; ++j)
										{
											if( pAura->GetSpellProto()->EffectApplyAuraName[j] == SPELL_AURA_MOD_STUN )
											{
												pAura->Remove();
												break;
											}
										}
									}
								}
							}
						}
					}
				}break;
			case SPELL_HASH_TIGER_S_FURY:
				{
					if( p_caster && p_caster->HasDummyAura(SPELL_HASH_KING_OF_THE_JUNGLE) )
					{
						SpellEntry * spellInfo = dbcSpell.LookupEntry( 51178 );
						if( spellInfo )
						{
							Spell *spell = new Spell(p_caster, spellInfo ,true, NULL);
							spell->forced_basepoints[0] = p_caster->GetDummyAura(SPELL_HASH_KING_OF_THE_JUNGLE)->RankNumber * 20;
							SpellCastTargets targets(p_caster->GetGUID());
							spell->prepare(&targets);
						}
					}
				}break;
			case SPELL_HASH_EARTHBIND:
			{
				if( m_spellInfo->Id == 3600 && u_caster && u_caster->IsCreature() && static_cast<Creature *>( u_caster )->IsTotem() )
					{
						if( static_cast<Creature *>( u_caster )->GetSummonOwner() && static_cast<Creature *>( u_caster )->GetSummonOwner()->HasDummyAura(SPELL_HASH_EARTHEN_POWER) )
							{
							if( Rand( static_cast<Creature *>( u_caster )->GetSummonOwner()->GetDummyAura(SPELL_HASH_EARTHEN_POWER)->RankNumber * 50 ) )
							{
								SpellEntry* totemSpell = dbcSpell.LookupEntryForced( 59566 );
								Spell *pSpell = new Spell(u_caster, totemSpell, true, NULL);
								SpellCastTargets targets;
								pSpell->GenerateTargets( &targets );
								pSpell->prepare(&targets);
							}
						}
					}
				}break;
			}
			*/

            if( m_caster->IsPlayer() && ((Player *)m_caster)->HasDummyAura(SPELL_HASH_DEADLY_BREW) &&
				m_spellInfo->poison_type && 
				(
				 m_spellInfo->poison_type == POISON_TYPE_WOUND ||
				 m_spellInfo->poison_type == POISON_TYPE_INSTANT ||
				 m_spellInfo->poison_type == POISON_TYPE_MIND_NUMBING 
				))
			{
				Unit *u_target = ((Player *)m_caster)->GetMapMgr()->GetUnit(m_targets.m_target->GetGUID());
				if( u_target && (u_target != (Unit *)m_caster) && isAttackable( u_target, (Unit *)m_caster ) )
				{
					uint32 chance = ((Player *)m_caster)->GetDummyAura(SPELL_HASH_DEADLY_BREW)->RankNumber == 1 ? 50 : 100;
					if( Rand( chance ) )
					{
					 //apply Crippling poison
					 ((Unit *)m_caster)->CastSpell( u_target, 25809, true );
					}
				}
			}
			m_isCasting = false;
			if (m_spellState != SPELL_STATE_CASTING)
				finish();
		} 
		else //this shit has nothing to do with instant, this only means it will be on NEXT melee hit
		{
			// we're much better to remove this here, because otherwise spells that change powers etc,
			// don't get applied.

			if(((Unit *)m_caster) && !m_triggeredSpell && !m_triggeredByAura)
				((Unit *)m_caster)->RemoveAurasByInterruptFlagButSkip(AURA_INTERRUPT_ON_CAST_SPELL, m_spellInfo->Id);
						
			m_isCasting = false;
			SendCastResult(cancastresult);
			if(((Unit *)m_caster) != NULL)
				((Unit *)m_caster)->SetOnMeleeSpell(GetSpellProto()->Id, extra_cast_number);

			finish();
			return;
		}
			/*
             damn fireball! stuff..
			if(p_caster)
			{
		     if(m_spellInfo->NameHash == SPELL_HASH_FIREBALL && p_caster->HasAura(__Fireball_)) // 57761
			            p_caster->RemoveAura(__Fireball_ ); // 57761
			}

		    // mage hot streak
		    if(p_caster && m_spellInfo->NameHash == SPELL_HASH_PYROBLAST &&
		                                   p_caster->HasAura(__Hot_Streak)) // 48108
			           p_caster->RemoveAura(__Hot_Streak); // 48108
			
      		m_isCasting = false;
			SendCastResult(cancastresult);
			if( u_caster != NULL )  
				u_caster->SetOnMeleeSpell(m_spellInfo->Id, extra_cast_number);

			finish();
			}*/
	} //-----------------------------------
	else
	{
		// cancast failed
		Log.Warning("Cast","Cast failed!");
		SendCastResult(cancastresult);
		SendInterrupted(cancastresult);
		finish();
	} //-----------------------------------
}

void Spell::HandleDestLocationHit()
{
	if(m_spellInfo->spell_signature != _SPELL_SIGNATURE_)
	{
		sLog.outDetail("Spell::HandleDestLocationHit: SpellInfo leak!");
		return;
	}

	//Log.Warning("HandleDestLocationHit","Begin");

	SpellTargetMap::iterator i;
	for(uint32 x=0;x<3;x++) // Une Aura par SpellEffect
	{
		//Log.Warning("FillTargetMap","Call03");
		FillTargetMap(x); 
		// check if we actualy have a effect
		if( m_spellInfo->Effect[x])
		{
			// Debug (Branruz)
			//Log.Notice("[Spell::HandleDestLocationHit]","In progress: Index %u, SpellEffect %u",x,m_spellInfo->Effect[x]);
			if (m_orderedObjects.size()>0)
			{
				for (std::vector<Object*>::iterator itr=m_orderedObjects.begin(); itr!=m_orderedObjects.end(); ++itr)
				{
					i = m_spellTargets.find(*itr);
					if (i != m_spellTargets.end() && i->second.HasEffect[x])
					{                     
						HandleEffects(i->first, x, false);
					}
				}
			}
			else if( m_spellInfo->Effect[x] == SPELL_EFFECT_TELEPORT_UNITS)
			{
				//Log.Notice("[Spell::HandleDestLocationHit]","SPELL_EFFECT_TELEPORT_UNITS (%u)",m_spellInfo->Effect[x]);
				HandleEffects(m_caster, x, false);
			}
			else if( m_spellInfo->Effect[x] == SPELL_EFFECT_SUMMON_WILD)
			{
				//Log.Notice("[Spell::HandleDestLocationHit]","SPELL_EFFECT_SUMMON_WILD (%u)",m_spellInfo->Effect[x]);
				HandleEffects(m_caster, x, false);
			}
			else if(m_spellInfo->Effect[x] == SPELL_EFFECT_LEARN_SPELL)
			{
				Log.Notice("[Spell::HandleDestLocationHit]","SPELL_EFFECT_LEARN_SPELL (%u)",m_spellInfo->Effect[x]);
				HandleEffects(m_caster, x, false);
			}
			else
			{
				if((m_caster != NULL) && (m_caster->IsUnit()) && (!m_caster->IsPlayer())) // PNJ seulement
				{
					HandleEffects((Object *)((Unit *)m_caster)->GetAIInterface()->GetCurrentTarget(), x, false); // try with m_target
				}
				else
				{
					Log.Warning("HandleDestLocationHit","%s avec NULL Target",SpellEffectTxt(m_spellInfo->Effect[x]));
					HandleEffects(NULL, x, false);     //try NULL for some effects
				}
			}
		}

		
	}
	//Log.Warning("Spell::HandleDestLocationHit","End");
}

void Spell::AddTime(uint32 type)
{
	//if(m_caster->IsPlayer())
	//{ Deplacer plus bas 

		if( m_spellInfo->interruptFlags) // & CAST_INTERRUPT_ON_DAMAGE_TAKEN)
		{
		 if (delaycount >= 2) return;
		 ++delaycount;
		
			if(m_caster->IsPlayer()) cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
			return;
		}

	if(m_caster->IsPlayer())
	{	
		if( (m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2] ) ) //&& u_caster)
		{
			float ch=0;
			SM_FFValue(((Player *)m_caster)->SM_PNonInterrupt,&ch,m_spellInfo);
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
			float spell_pct_modifers=0;
			SM_FFValue(u_caster->SM_PNonInterrupt,&spell_pct_modifers,m_spellInfo->SpellGroupType[0],m_spellInfo->SpellGroupType[1],m_spellInfo->SpellGroupType[2]);
			if(spell_pct_modifers!=0)
				printf("!!!!!spell interrupt chance mod pct %f , uninterrupt chance %f, spell group %u\n",
				spell_pct_modifers,ch,m_spellInfo->SpellGroupType[0],m_spellInfo->SpellGroupType[1],m_spellInfo->SpellGroupType[2]);
#endif
			if(Rand(ch))
				return;
		}
		if( m_caster->IsPlayer() )
		{
			if(Rand(((Player *)m_caster)->SpellDelayResist[type]))
				return;
		}
		 

		 if(m_spellState==SPELL_STATE_PREPARING)
		 {
			int32 delay = m_castTime/4;
            
			// Ascent 5218 - Randdrick 214
			float reductionmult = 0.0f;
			SM_FFValue(((Unit *)m_caster)->SM_PNonInterrupt, &reductionmult, m_spellInfo);
			delay *= (1 - (reductionmult / 100));
			if(m_caster->IsPlayer())
				delay *= (1 - (((Player *)m_caster)->SpellDelayResist[type] / 100));
			//--------

			m_timer+=delay;
			if(m_timer>m_castTime)
			{
				delay -= (m_timer - m_castTime);
				m_timer=m_castTime;
				if(delay<0)
					delay = 1;
			}

			WorldPacket data(SMSG_SPELL_DELAYED, 13);
			data << ((Unit *)m_caster)->GetNewGUID();
			data << uint32(delay);
			((Unit *)m_caster)->SendMessageToSet(&data, true);

			if(!m_caster->IsPlayer())
			{
				if(m_caster->GetTypeId() == TYPEID_UNIT)
					((Unit *)m_caster)->GetAIInterface()->AddStopTime(delay);
			}
			//in case cast is delayed, make sure we do not exit combat 
			else
			{
//				sEventMgr.ModifyEventTimeLeft(p_caster,EVENT_ATTACK_TIMEOUT,PLAYER_ATTACK_TIMEOUT_INTERVAL,true);
				// also add a new delay to offhand and main hand attacks to avoid cutting the cast short
				((Player *)m_caster)->delayAttackTimer(delay);
			}
		}
		else if( m_spellInfo->channelInterruptFlags != 48140)
		{
			int32 delay = GetDuration(INVOC_DURATION)/4;

			// Ascent 5218 - Randdrick 214
			float reductionmult = 0.0f;
			SM_FFValue(((Unit *)m_caster)->SM_PNonInterrupt, &reductionmult, m_spellInfo);
			delay *= (1 - (reductionmult / 100));
			if (m_caster->IsPlayer())
				delay *= (1 - (((Player *)m_caster)->SpellDelayResist[type] / 100));
			//---------

			m_timer-=delay;
			if(m_timer<0)
				m_timer=0;
			else
				((Player *)m_caster)->delayAttackTimer(-delay);

			m_Delayed = true;
			if(m_timer>0) SendChannelUpdate(m_timer); // Barre d'incantation
		}
	}
}

void Spell::update(uint32 difftime)
{
	// skip cast if we're more than 2/3 of the way through
	// TODO: determine which spells can be casted while moving.
	// Client knows this, so it should be easy once we find the flag.
	// XD, it's already there!
	if( ( m_spellInfo->interruptFlags & CAST_INTERRUPT_ON_MOVEMENT ) &&
		(((float)m_castTime / 1.5f) > (float)m_timer ) && 
//		float(m_castTime)/float(m_timer) >= 2.0f		&&
		(
		m_castPositionX != m_caster->GetPositionX() ||
		m_castPositionY != m_caster->GetPositionY() ||
		m_castPositionZ != m_caster->GetPositionZ()
		)
		)
	{	
		if( m_caster->IsUnit() )
		{
			if(((Unit *)m_caster)->HasNoInterrupt() == 0 && m_spellInfo->EffectMechanic[1] != MECHANIC_INCAPACIPATED) // 14
			{
				//Log.Warning("Spell::update","Spell %d Interrompu! 1",this->GetSpellProto()->Id);
				cancel(SPELL_FAILED_INTERRUPTED); // "Interrompu"
				return;
			}
		}
	}

	if (m_targets.m_target != NULL) 
	{

		// Debug (Branruz)
		/*if(m_targets.m_target->IsPlayer()) Log.Warning("Spell::update","Cible: Player");
		else if(m_targets.m_target->IsUnit()) Log.Warning("Spell::update","Cible: Creature");
		else if(m_targets.m_target->IsItem()) Log.Warning("Spell::update","Cible: Item");
		else if(m_targets.m_target->IsGO()) Log.Warning("Spell::update","Cible: Go");
		else Log.Warning("Spell::update","Cible: Type inconnu");

		if(m_targets.m_target->IsUnit()) 
		{
		 Log.Warning("Spell::update","Cible: %s",
			(static_cast<Unit*>(m_targets.m_target)->isAlive())?"en vie":"mort");
		 Log.Warning("Spell::update","Cible: Nom: %s",
			 (static_cast<Unit*>(m_targets.m_target)->GetObjectName())?(static_cast<Unit*>(m_targets.m_target)->GetObjectName()):"Iconnu");
		}*/

		if(m_targets.m_target->IsUnit() && !static_cast<Unit*>(m_targets.m_target)->isAlive()) // Cible sans vie
	    {
			// Exception pour: Depecage / Guerrison d'esprit / Ceuillette / Minage / Ingenierie
			if(!(m_spellInfo->targets & TARGET_FLAG_INERTE)) // 0x00000400
			{
		     //Log.Warning("Spell::update","Spell %d Interrompu! 2",this->GetSpellProto()->Id);
			 if( m_caster->IsPlayer() ) cancel(SPELL_FAILED_TARGETS_DEAD); // "Votre cible est morte"
		     else                       cancel(SPELL_FAILED_INTERRUPTED);  // "Interrompu"
		     return;
			}
		}
	}

	if(m_cancelled)
	{
		//Log.Warning("Spell::update","Spell %d cancelled! 3",this->GetSpellProto()->Id);
		cancel(SPELL_FAILED_INTERRUPTED); // Interrompu
		return;
	}

	switch(m_spellState)
	{
	case SPELL_STATE_PREPARING:
		{
			//printf("spell::update m_timer %u, difftime %d, newtime %d\n", m_timer, difftime, m_timer-difftime);
			if((int32)difftime >= m_timer)
				cast(true);
			else 
			{
				m_timer -= difftime;
				if((int32)difftime >= m_timer)
				{
					m_timer = 0;
					cast(true);
				}
			}
			
			
		} break;
	case SPELL_STATE_CASTING:
		{
			if(m_timer > 0)
			{
				if((int32)difftime >= m_timer)
					m_timer = 0;
				else
					m_timer -= difftime;
			}
			if(m_timer <= 0)
			{
				SendChannelUpdate(0);
				finish();
			}
		} break;
	}
}

void Spell::finish()
{
	m_spellState = SPELL_STATE_FINISHED;

	//remove all auras created if we channeled :P
	if (m_spellInfo->channelInterruptFlags != 0)
	{
		for (std::set<Aura*>::iterator itr=m_auras.begin(); itr!=m_auras.end(); ++itr)
			sEventMgr.ModifyEventTimeLeft((*itr), EVENT_AURA_REMOVE, 500);
	}
	
	if( m_caster->IsUnit() )
	{
		((Unit *)m_caster)->m_canMove = true;
		// mana           channeled                                                     power type is mana
		if(m_usesMana && (m_spellInfo->channelInterruptFlags == 0 && !m_triggeredSpell) && ((Unit *)m_caster)->GetPowerType()==POWER_TYPE_MANA)
		{
			/*
			Five Second Rule
			After a character expends mana in casting a spell, the effective amount of mana gained per tick from spirit-based regeneration becomes a ratio of the normal 
			listed above, for a period of 5 seconds. During this period mana regeneration is said to be interrupted. This is commonly referred to as the five second rule. 
			By default, your interrupted mana regeneration ratio is 0%, meaning that spirit-based mana regeneration is suspended for 5 seconds after casting.
			Several effects can increase this ratio, including:
			*/

			((Unit *)m_caster)->DelayPowerRegeneration(5000);
		}
	}
	/* Mana Regenerates while in combat but not for 5 seconds after each spell */
	/* Only if the spell uses mana, will it cause a regen delay.
	   is this correct? is there any spell that doesn't use mana that does cause a delay?
	   this is for creatures as effects like chill (when they have frost armor on) prevents regening of mana	*/
	
	//moved to spellhandler.cpp -> remove item when click on it! not when it finishes 

	//enable pvp when attacking another player with spells
	if( m_caster->IsPlayer() )
	{
		if (m_spellInfo->attributes & ATTRIBUTES_STOP_ATTACK)
		{
			((Player *)m_caster)->EventAttackStop();
			((Player *)m_caster)->smsg_AttackStop( unitTarget );
		}

		if(m_requiresCP && !GetSpellFailed())
		{
			if(((Player *)m_caster)->m_spellcomboPoints)
			{
				((Player *)m_caster)->m_comboPoints = ((Player *)m_caster)->m_spellcomboPoints;
				((Player *)m_caster)->UpdateComboPoints(); //this will make sure we do not use any wrong values here
			}
			else
			{
				((Player *)m_caster)->NullComboPoints();
			}
		}
		if(m_Delayed)
		{
			Unit *pTarget = NULL;
			if( m_caster->IsInWorld() )
			{
				pTarget = ((Player *)m_caster)->GetMapMgr()->GetUnit(m_caster->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT));
				if(!pTarget)
					pTarget = ((Player *)m_caster)->GetMapMgr()->GetUnit(((Player *)m_caster)->GetSelection());
			}
			   
			if(pTarget)
			{
				pTarget->RemoveAura(m_spellInfo->Id, m_caster->GetGUID());
			}
		}
	}

	if( m_spellInfo->Effect[0] == SPELL_EFFECT_SUMMON_OBJECT ||
		m_spellInfo->Effect[1] == SPELL_EFFECT_SUMMON_OBJECT ||
		m_spellInfo->Effect[2] == SPELL_EFFECT_SUMMON_OBJECT)
		if( m_caster->IsPlayer() )
			((Player *)m_caster)->SetSummonedObject(NULL);		


	if( m_caster->IsItem())
	{
			 
		/*Set cooldown on item*/	
		if( ((Item *)m_caster)->GetOwner() && (cancastresult == SPELL_CANCAST_OK) && (!GetSpellFailed()) )
	    {
			uint32 x;
		    for(x = 0; x < 5; x++)
		    {
			 if(((Item *)m_caster)->GetProto()->Spells[x].Trigger == USE)
			 {
				if(((Item *)m_caster)->GetProto()->Spells[x].Id)
					break;
			 }
		    }
		    ((Item *)m_caster)->GetOwner()->Cooldown_AddItem( ((Item *)m_caster)->GetProto() , x );
		}

		/* Suppression de la charge de l'Item ou de l'Item utilisé dans le sort */
		if( m_ForceConsumption || ( cancastresult == SPELL_CANCAST_OK && !GetSpellFailed()) )
		{
			Log.Notice("[Spell::finish()]","Suppression de la charge de l'Item ou de l'Item utilise dans le sort");
			RemoveItems((Item *)m_owner);	// Normalement c'est un Item
											// Attention: L'owner n'est pas forcement definie directement (trames reseaux)
											// en tout cas, bien verifier que l'item stack se decompte correctement via un SpellEffect si m_owner est NULL
		}
	}
	/*
	We set current spell only if this spell has cast time or is channeling spell
	otherwise it's instant spell and we delete it right after completion
	*/

	Spell* spl = (this); // feeefeee! <3

	if(  m_caster->IsUnit() )
	{
		if( ((Unit *)m_caster)->GetCurrentSpell() == this )
			((Unit *)m_caster)->SetCurrentSpell(NULL);
	}

	if( m_caster->IsPlayer() && !GetSpellFailed() )
	{
		sHookInterface.OnPostSpellCast( ((Player *)m_caster), GetSpellProto(), unitTarget );
	}
	
	if( m_caster->IsPlayer() ) 
	{
		// En principe, le caster ne devrait pas être player si le m_owner est un item. 
		//Cependant, on considère dans ce cas là, que le caster est le player et le déclencheur, l'item.
		if( (m_owner && m_owner->IsItem()) && (m_ForceConsumption || ( cancastresult == SPELL_CANCAST_OK && !GetSpellFailed()) ) )
		{
			Log.Notice("[Spell::finish()]","Suppression de la charge de l'Item ou de l'Item utilise par le joueur");
			RemoveItems((Item *)m_owner);	// Normalement c'est un Item
											// Attention: L'owner n'est pas forcement definie directement (trames reseaux)
											// en tout cas, bien verifier que l'item stack se decompte correctement via un SpellEffect si m_owner est NULL
		}
	}
	
	/*
	if( u_caster != NULL )
	{
		if(!m_triggeredSpell && (m_spellInfo->channelInterruptFlags || m_castTime>0))
			u_caster->SetCurrentSpell(NULL);
	}*/
	/*
	if( m_caster->IsUnit()) 
	{
		if(((Unit *)m_caster)->GetCurrentSpell() == this ) ((Unit *)m_caster)->SetCurrentSpell(NULL); 
	}
	
	if( m_caster->IsPlayer())
	{
		if( hadEffect || ( cancastresult == SPELL_CANCAST_OK && !GetSpellFailed() ) )
		{
			if(m_owner)
			{
			 if(m_owner->IsItem()) // Normalement c'est un Item
			 {
				RemoveItems((Item *)m_owner); 
			 }
			 else
			 {
				 Log.Error("[Spell::finish()]","Remove: pas un Item <---- Report this to devs.");
			 }
			} // Attention: L'owner n'est pas forcement definie directement (trames reseaux)
			  // en tout cas, bien verifier que l'item stack se decompte correctement via un SpellEffect si m_owner est NULL
			//else Log.Error("[Spell::finish()]","Remove: m_owner NULL <---- Report this to devs."); // N'arrive jamais non plus, sauf via SpellEffect (Brz)
		}
	}

   //procfncs :D
   std::vector<void*> data;
   data.push_back((void*)this);

   if(m_caster) // Ca c'est tres zarb...le caster a été reset avant la fin du Spell::finish(), a voir.... (brz)
   {
    if (m_caster->IsUnit())
		((Unit *)m_caster)->HandleProcFnc(SPELLFNC_PROC_ON_POST_CAST_SPELL, &data);

	//this->Destructor();
	sEventMgr.AddEvent(this, &Spell::EventDelete, EVENT_UNK, 500, 0, 0);
	//sEventMgr.AddEvent(this, &Spell::EventDelete, EVENT_UNK, 1000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
   } 
   else Spell::~Spell(); // Cas extreme, le m_caster devrait tjs exister à ce stade, c'est tres zarb cette histoire (Branruz)
	*/
	
}

void Spell::SendCastResult(uint8 result)
{
	uint32 Extra = 0;
	if(result == SPELL_CANCAST_OK) return;

	SetSpellFailed();

	if(m_caster)
	{
	 if(!m_caster->IsInWorld()) return;
	}

	
	Player * plr =NULL;
    if(m_caster->IsPlayer()) plr = ((Player *)m_caster); 
	if(!plr && m_caster->IsUnit())
        plr = ((Unit *)m_caster)->m_redirectSpellPackets;

	if(!plr) return;

	// reset cooldowns
	if( m_spellState == SPELL_STATE_PREPARING )
		plr->Cooldown_OnCancel(m_spellInfo);

	// for some reason, the result extra is not working for anything, including SPELL_FAILED_REQUIRES_SPELL_FOCUS
	switch( result )
	{
	case SPELL_FAILED_REQUIRES_SPELL_FOCUS: //  "Requiert (%s)"
		Extra = m_spellInfo->requiresSpellFocus; 
		break;

	case SPELL_FAILED_REQUIRES_AREA: // "Vous devez etre dans (%s)"
		if( m_spellInfo->AreaGroupId > 0 )
		{
			uint16 area_id = plr->GetMapMgr()->GetAreaID( plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ() );
			AreaGroup *GroupEntry = dbcAreaGroup.LookupEntry( m_spellInfo->AreaGroupId );

			for( uint8 i = 0; i < 7; i++ )
				if( GroupEntry->AreaId[i] != 0 && GroupEntry->AreaId[i] != area_id )
				{
					Extra = GroupEntry->AreaId[i];
					break;
				}
		}
		break;

	case SPELL_FAILED_TOTEMS: //  (pas de message attente d'un id item comme param) 
		Extra = m_spellInfo->totem[1] ? m_spellInfo->totem[1] : m_spellInfo->totem[0];
		break;
	}

	//plr->SendCastResult(m_spellInfo->Id, result, extra_cast_number, Extra);
	if( Extra )
	{
		packetSMSG_CASTRESULT_EXTRA pe;
		pe.SpellId = m_spellInfo->Id;
		pe.ErrorMessage = result;
		pe.MultiCast = extra_cast_number;
		pe.Extra = Extra;
		plr->GetSession()->OutPacket( SMSG_CAST_FAILED, sizeof( packetSMSG_CASTRESULT_EXTRA ), &pe );
	}
	else
	{
		packetSMSG_CASTRESULT pe;
		pe.SpellId = m_spellInfo->Id;
		pe.ErrorMessage = result;
		pe.MultiCast = extra_cast_number;
		plr->GetSession()->OutPacket( SMSG_CAST_FAILED, sizeof( packetSMSG_CASTRESULT ), &pe );
	}
}

// uint16 0xFFFF
enum SpellStartFlags
{
	SPELL_START_FLAGS_NONE				= 0x00000000,
	SPELL_START_FLAGS_UNKNOWN0			= 0x00000001,	// may be pending spell cast
    SPELL_START_FLAG_DEFAULT 			= 0x00000002,   // atm set as default flag
	SPELL_START_FLAGS_UNKNOWN2			= 0x00000004,
	SPELL_START_FLAGS_UNKNOWN3			= 0x00000008,
	SPELL_START_FLAGS_UNKNOWN4			= 0x00000010,
    SPELL_START_FLAGS_RANGED	    	= 0x00000020,
	SPELL_START_FLAGS_UNKNOWN6			= 0x00000040,
	SPELL_START_FLAGS_UNKNOWN7			= 0x00000080,
	SPELL_START_FLAGS_UNKNOWN8			= 0x00000100,
	SPELL_START_FLAGS_UNKNOWN9			= 0x00000200,
	SPELL_START_FLAGS_UNKNOWN10			= 0x00000400, //TARGET MISSES AND OTHER MESSAGES LIKE "Resist"
	SPELL_START_FLAGS_POWER_UPDATE		= 0x00000800,
	SPELL_START_FLAGS_UNKNOWN12			= 0x00001000,
	SPELL_START_FLAGS_UNKNOWN13			= 0x00002000,
	SPELL_START_FLAGS_UNKNOWN14			= 0x00004000,
	SPELL_START_FLAGS_UNKNOWN15			= 0x00008000,
	SPELL_START_FLAGS_UNKNOWN16			= 0x00010000,
	SPELL_START_FLAGS_UNKNOWN17			= 0x00020000,
	SPELL_START_FLAGS_UNKNOWN18			= 0x00040000,
	SPELL_START_FLAGS_UNKNOWN19			= 0x00080000,
	SPELL_START_FLAGS_UNKNOWN20			= 0x00100000,
	SPELL_START_FLAGS_RUNE_UPDATE		= 0x00200000,
    SPELL_START_FLAGS_UNKNOWN21			= 0x00400000,
};

void Spell::SendSpellStart()
{
	// no need to send this on passive spells
	if(m_caster)
	{
	 if( !m_caster->IsInWorld() || (m_spellInfo->attributes & ATTRIBUTES_PASSIVE) || m_triggeredSpell )
	 {
		// Debug (Branruz)
		//Log.Notice("[SendSpellStart]","Interrompu.... Aura passive ou TriggerSpell ");
		return;
	 }
	}
	else 
	{
		// m_caster DOIT exister si i_caster n'existe pas, sinon paf!!
		Log.Error("[SendSpellStart]","Interrompu.... m_caster NULL <- report this to dev.");
		return;
	}

	WorldPacket data( SMSG_SPELL_START,1024 ); // 150 : BufferOverflow ? (Branruz), je compte 161 octets max et 155 octets minimum
	/*uint8 buf[1000];			// should be more than enough
	StackPacket data(SMSG_SPELL_START, buf, 1000);*/

	uint32 cast_flags = SPELL_START_FLAG_DEFAULT;

	if(m_caster->IsUnit() && !m_triggeredSpell && !(m_spellInfo->attributesEx & ATTRIBUTESEX_AREA_OF_EFFECT))
		cast_flags |= SPELL_START_FLAGS_POWER_UPDATE;

	if(m_caster->IsPlayer() && ((Player *)m_caster)->getClass() == DEATHKNIGHT)
		cast_flags |= SPELL_START_FLAGS_RUNE_UPDATE;

	if( GetType() == DEFENSE_TYPE_RANGED )
		cast_flags |= SPELL_START_FLAGS_RANGED;

    // hacky yeaaaa
    if( m_spellInfo->Id == __Ghost ) // death 8326
        cast_flags = 0x0F;

	// SendSpellStart
	if( m_caster->IsItem() )  
	{
		//Log.Warning("[SendSpellStart]","Item %d, Cible: %d",m_caster->GetEntry(),m_targets.m_target->GetEntry());
		data << ((Item *)m_caster)->GetNewGUID() << m_targets.m_target->GetNewGUID(); // 64 + 64 icaster,ucaster
	}
	else                     
		data << m_caster->GetNewGUID() << m_caster->GetNewGUID(); // 64 + 64

	data << extra_cast_number;   // + 1
	data << m_spellInfo->Id;     // + 4
	data << cast_flags;          // + 4
	data << (uint32)m_castTime;  // + 4
		
	m_targets.write( data );

	if (cast_flags & SPELL_START_FLAGS_POWER_UPDATE) //send new mana
		data << uint32( (m_caster->IsUnit()) ? ((Unit *)m_caster)->GetUInt32Value(UNIT_FIELD_POWER1 + ((Unit *)m_caster)->GetPowerType()) : 0);

	if ((cast_flags & SPELL_START_FLAGS_RUNE_UPDATE) && m_caster->IsPlayer() )//send new runes
	{
		RuneCostEntry * runecost = dbcSpellRuneCost.LookupEntry(m_spellInfo->runeCostID);
		uint8 theoretical = ((Player *)m_caster)->TheoreticalUseRunes(runecost->blood, runecost->frost, runecost->unholy);
		data << ((Player *)m_caster)->m_runemask << theoretical; // + 1 + 1

		for (uint8 i=0; i<6; ++i)                    // +6 (maximum)
		{
			if ((1 << i) & ((Player *)m_caster)->m_runemask)
				if (!((1 << i) & theoretical))
					data << uint8(0); // I love you, Andy--in a gay way.
		}
	}

	if( GetType() == DEFENSE_TYPE_RANGED )
	{
		ItemPrototype* ip = NULL;
        if( m_spellInfo->Id == SPELL_RANGED_THROW ) // throw
        {
			if( m_caster->IsPlayer() )
			{
				Item *itm = ((Player *)m_caster)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
				if( itm != NULL )
				{
					ip = itm->GetProto();
					/* Throwing Weapon Patch by Supalosa
					p_caster->GetItemInterface()->RemoveItemAmt(it->GetEntry(),1);
					(Supalosa: Instead of removing one from the stack, remove one from durability)
					We don't need to check if the durability is 0, because you can't cast the Throw spell if the thrown weapon is broken, because it returns "Requires Throwing Weapon" or something.
					*/
	
					// burlex - added a check here anyway (wpe suckers :P)
					if( itm->GetDurability() > 0 )
					{
						itm->SetDurability( itm->GetDurability() - 1 );
						if( itm->GetDurability() == 0 )
							((Player *)m_caster)->ApplyItemMods( itm, EQUIPMENT_SLOT_RANGED, false, true );
					}
				}
				else
				{
					ip = ItemPrototypeStorage.LookupEntry( 2512 );	/*rough arrow*/
				}
			}
		}
		else if( m_spellInfo->attributesExC & ATTRIBUTESEXC_PLAYER_RANGED_SPELLS )
		{
			if( m_caster->IsPlayer() )
				ip = ItemPrototypeStorage.LookupEntry( ((Player *)m_caster)->GetUInt32Value( PLAYER_AMMO_ID ) );
			else
				ip = ItemPrototypeStorage.LookupEntry( 2512 );	/*rough arrow*/
		}
		
		if( ip != NULL )
			data << ip->DisplayInfoID << ip->InventoryType; // + 4 +4
	}
	// Debug (Branruz)
    //Log.Notice("[Spell::SendSpellStart]","Ok ");
	//Log.Warning("SendSpellStart","Opcode: 0x%04X",data.GetOpcode());

	if(m_caster->IsPlayer()) m_caster->SendMessageToSet( &data, true );
	else if(m_targets.m_target->IsPlayer()) m_targets.m_target->SendMessageToSet( &data, true );
	//else Log.Warning("SendSpellStart","Spell %u : Le player n'est ni caster, ni la cible",m_spellInfo->Id);
}

/************************************************************************/
/* General Spell Go Flags, for documentation reasons                    */
/************************************************************************/
enum SpellGoFlags
{
	SPELL_GO_FLAGS_NONE					= 0x00000000,
	SPELL_GO_FLAGS_UNKNOWN0				= 0x00000001,              // may be pending spell cast
	SPELL_GO_FLAGS_UNKNOWN1				= 0x00000002,
	SPELL_GO_FLAGS_UNKNOWN2				= 0x00000004,
	SPELL_GO_FLAGS_UNKNOWN3				= 0x00000008,
	SPELL_GO_FLAGS_UNKNOWN4				= 0x00000010,
    SPELL_GO_FLAGS_RANGED           	= 0x00000020,
	SPELL_GO_FLAGS_UNKNOWN6				= 0x00000040,
	SPELL_GO_FLAGS_UNKNOWN7				= 0x00000080,
    SPELL_GO_FLAGS_ITEM_CASTER      	= 0x00000100,
	SPELL_GO_FLAGS_UNKNOWN9				= 0x00000200,
    SPELL_GO_FLAGS_EXTRA_MESSAGE    	= 0x00000400, //TARGET MISSES AND OTHER MESSAGES LIKE "Resist"
    SPELL_GO_FLAGS_POWER_UPDATE     	= 0x00000800,
	SPELL_GO_FLAGS_UNKNOWN12			= 0x00001000,
	SPELL_GO_FLAGS_UNKNOWN13			= 0x00002000,
	SPELL_GO_FLAGS_UNKNOWN14			= 0x00004000,
	SPELL_GO_FLAGS_UNKNOWN15			= 0x00008000,
	SPELL_GO_FLAGS_UNKNOWN16			= 0x00010000,
	SPELL_GO_UNK2                   	= 0x00020000,
    SPELL_GO_UNK4                  		= 0x00040000, 
	SPELL_GO_UNK3                  		= 0x00080000,
	SPELL_GO_FLAGS_UNKNOWN20			= 0x00100000,
	SPELL_GO_FLAGS_RUNE_UPDATE			= 0x00200000,
	SPELL_GO_FLAGS_UNKNOWN22			= 0x00400000
};

void Spell::SendSpellGo()
{
    // no need to send this on passive spells
    if(!m_caster->IsInWorld() || m_spellInfo->attributes & ATTRIBUTES_PASSIVE)
        return;

	ItemPrototype* ip = NULL;
	uint32 cast_flags = (m_triggeredSpell && !(m_spellInfo->attributes & ATTRIBUTE_ON_NEXT_ATTACK)) ? 0x0105 : 0x0100;
	if(m_caster->IsUnit() && !m_triggeredSpell && (((Unit *)m_caster)->GetPowerType() != POWER_TYPE_MANA || m_usesMana) &&
		!(m_spellInfo->attributesEx & ATTRIBUTESEX_AREA_OF_EFFECT))	// Hackfix for client bug which displays mana as 0 after receiving update for these spells
		cast_flags |= SPELL_GO_FLAGS_POWER_UPDATE;
	SpellTargetList::iterator itr;
	//uint32 counter;

	if( m_caster->IsItem() )
		cast_flags |= SPELL_GO_FLAGS_ITEM_CASTER; // 0x100 ITEM CASTER

	// hacky..
	if( m_spellInfo->Id == __Ghost ) // death
		cast_flags = SPELL_GO_FLAGS_ITEM_CASTER | 0x0D; // 0x0D ??Inconnu

	if( GetType() == DEFENSE_TYPE_RANGED )
	{
		cast_flags |=SPELL_GO_FLAGS_RANGED;
		if( m_spellInfo->Id == SPELL_RANGED_THROW )
		{
			if( m_caster->IsPlayer() )
			{
				Item *it = ((Player *)m_caster)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_RANGED );
				if( it != NULL )
					ip = it->GetProto();
			}
			else
			{
				ip = ItemPrototypeStorage.LookupEntry(2512);	//rough arrow
			}
		}
		else
		{
			if( m_caster->IsPlayer() )
				ip = ItemPrototypeStorage.LookupEntry(((Player *)m_caster)->GetUInt32Value( PLAYER_AMMO_ID ) );
			else // HACK FIX
				ip = ItemPrototypeStorage.LookupEntry(2512);	//rough arrow
		}
	}

	// Start Spell
	/*WorldPacket data(200);
	//data.SetOpcode(SMSG_SPELL_GO);
	//32 bits in 3.0.2 i think, from packet logs it seems that way
                                                            // 64                            64
	WorldPacket data(SMSG_SPELL_GO, 1 + 4 + 4 + 4 + 1 + (8 * m_objectsHit) + 1 + (8 * m_objectsModerated)); //needs to be var size for assertion clients
	*/
	// stack-based solution
	// this has room for every type of spell target type, 
	// as well as 100 missed and 100 hit targets.
	// if you're doing that, you're NUTS.
	uint8 packet[1882];
	StackPacket data(SMSG_SPELL_GO, packet, 1882);
	
	// IMPOSSIBLE ENSEMBLE
	//if( i_caster != NULL && u_caster != NULL ) // this is needed for correct cooldown on items
	//	data << i_caster->GetNewGUID() << u_caster->GetNewGUID();
	//else 
		data << m_caster->GetNewGUID() << m_caster->GetNewGUID();

	data << extra_cast_number;
	data << m_spellInfo->Id;
	data << cast_flags;
	data << getMSTime();

	/************************************************************************/
	/* spell go targets                                                     */
	/************************************************************************/
	// Make sure we don't hit over 100 targets.
	// It's fine internally, but sending it to the client will REALLY cause it to freak.

    if(m_objectsHit != m_orderedObjects.size()) // Secu, curieux ce probleme (branruz)
	{
		Log.Error("SMSG_SPELL_GO","m_objectsHit != m_orderedObjects.size() ( %u / %u )",m_objectsHit,m_orderedObjects.size());
        m_objectsHit = (int8) m_orderedObjects.size();
	}

	data << (uint8)(m_objectsHit); //number of hits
 
	if (m_objectsHit > 0)
	{
		std::vector<Object*>::iterator oitr = m_orderedObjects.begin();
		for (uint8 i=0; i<m_objectsHit; ++i)
		{
			if (oitr != m_orderedObjects.end())
			{
				
				if(!(*oitr)) 
				{
					Log.Error("SMSG_SPELL_GO","FATAL: (*oitr) NULL");
					data << uint64(0);  //deref will fail, beats client crash
				}
				else 
				{
					uint64 oitr_GUID = (*oitr)->GetGUID();
					data << uint64(oitr_GUID);
				}
				++oitr;
				
			}
			else
 			  data << uint64(0); //deref will fail, beats client crash
		}
	}

	data << m_objectsModerated; //number if misses

	if (m_objectsModerated > 0)
	{
		SpellTargetMap::iterator sitr=m_spellTargets.begin();
		uint8 i=0;
		do
		{
			if (sitr != m_spellTargets.end())
			{
				if (sitr->second.TargetModType != 0)
				{
					data << sitr->first->GetGUID();
					data << sitr->second.TargetModType;
					if (sitr->second.TargetModType == SPELL_DID_HIT_REFLECT)
						data << sitr->second.ExtendedTargetModType;

					if(sitr->second.TargetModType == SPELL_DID_HIT_RESIST && sitr->first->IsUnit() && static_cast<Unit*>(sitr->first)->isAlive())
					{
						Unit* target = static_cast<Unit*>(sitr->first);
						((Unit *)m_caster)->HandleProc(PROC_ON_RESIST_VICTIM, target, m_spellInfo/*,damage*/);		/** Damage is uninitialized at this point - burlex */
						target->CombatStatusHandler_ResetPvPTimeout(); // aaa
						((Unit *)m_caster)->CombatStatusHandler_ResetPvPTimeout(); // bbb
					}
					++i;
				}
				++sitr;
			}
			else
			{
				//this should never be hit
				data << uint64(0) << uint8(1);
				++i;
			}
		}
		while (i < m_objectsModerated);
	}

	m_targets.write(data); // this write is included the target flag

	if (cast_flags & SPELL_GO_FLAGS_POWER_UPDATE) //send new power
		data << uint32( (m_caster->IsUnit()) ? ((Unit *)m_caster)->GetUInt32Value(UNIT_FIELD_POWER1 + ((Unit *)m_caster)->GetPowerType()) : 0);
		
	if( ip != NULL)
		data << ip->DisplayInfoID << ip->InventoryType;

	m_caster->SendMessageToSet( &data, true );
	/*if (cast_flags & SPELL_GO_UNK2) //  0x00020000
		data << float(0) << uint32(0);

	if( ip != NULL)
		data << ip->DisplayInfoID << ip->InventoryType;

	if (cast_flags & SPELL_GO_UNK3) // 0x00080000
		data << uint32(0) << uint32(0);

	if (m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
		data << uint8(0);
	m_caster->SendMessageToSet( &data, true );*/
}
void Spell::writeSpellGoTargets( WorldPacket * data )
{

	for (std::vector<Object*>::iterator itr=m_orderedObjects.begin(); itr!=m_orderedObjects.end(); ++itr)
	{
		SendCastSuccess(*itr);
		*data << (*itr)->GetGUID();
	}
}

void Spell::writeSpellMissedTargets( WorldPacket * data )
{
	/*
	 * The flags at the end known to us so far are.
	 * 1 = Miss
	 * 2 = Resist
	 * 3 = Dodge // mellee only
	 * 4 = Deflect
	 * 5 = Block // mellee only
	 * 6 = Evade
	 * 7 = Immune
	 */
	SpellTargetMap::iterator i;
	if(m_caster->IsUnit() && ((Unit *)m_caster)->isAlive())
	{
		for (i=m_spellTargets.begin(); i!=m_spellTargets.end(); ++i)
		{
			if (i->second.TargetModType == 0)
				continue;

			*data << i->first->GetGUID();       // uint64
			*data << i->second.TargetModType;    // uint8
			if (i->second.TargetModType == SPELL_DID_HIT_REFLECT)
				*data << uint8(i->second.ExtendedTargetModType);
			///handle proc on resist spell
			if(i->second.TargetModType == SPELL_DID_HIT_RESIST && i->first->IsUnit() && static_cast<Unit*>(i->first)->isAlive())
			{
				Unit* target=static_cast<Unit*>(i->first);
				((Unit *)m_caster)->HandleProc(PROC_ON_RESIST_VICTIM, target, m_spellInfo/*,damage*/);		/** Damage is uninitialized at this point - burlex */
				target->CombatStatusHandler_ResetPvPTimeout(); // aaa
				((Unit *)m_caster)->CombatStatusHandler_ResetPvPTimeout(); // bbb
			}
		}
	}
	else
		for (i=m_spellTargets.begin(); i!=m_spellTargets.end(); ++i)
		{
			if (i->second.TargetModType == 0)
				continue;

			if (i->first == NULL)
				continue;

			*data << i->first->GetGUID();       // uint64
			*data << i->second.TargetModType;    // uint8
		}
}

void Spell::SendLogExecute(uint32 damage, uint64 & targetGuid)
{
	WorldPacket data(SMSG_SPELLLOGEXECUTE, 37);
	data << m_caster->GetNewGUID();
	data << m_spellInfo->Id;
	data << uint32(1);
//	data << m_spellInfo->SpellVisual;
//	data << uint32(1);
	if (m_caster->GetGUID() != targetGuid)
		data << targetGuid;
	if (damage)
		data << damage;
	m_caster->SendMessageToSet(&data,true);
}

void Spell::SendInterrupted(uint8 result)
{
	SetSpellFailed();

	if(m_caster == NULL) return;
	if(!m_caster->IsInWorld()) return;

	WorldPacket data(SMSG_SPELL_FAILURE, 20);

	// send the failure to pet owner if we're a pet
	Player *plr = ((Player *)m_caster);
	if(!plr)
	{
		 if(m_caster->IsPet())
		 {
           Unit *Pet_m_caster = static_cast<Pet*>(m_caster)->GetPetOwner();
		   if(Pet_m_caster)
		   {
			   if(Pet_m_caster->IsPlayer())
 		            plr = static_cast<Player*>(Pet_m_caster);
		   }
		 }


	    if(m_caster->IsUnit()) plr = ((Unit *)m_caster)->m_redirectSpellPackets;
	}

	if(plr && plr->IsPlayer())
	{
		data << m_caster->GetNewGUID();
		data << uint8(extra_cast_number);
		data << uint32(m_spellInfo->Id);
		data << uint8(result);
		plr->GetSession()->SendPacket(&data);
	}

	data.Initialize(SMSG_SPELL_FAILED_OTHER);
	data << m_caster->GetNewGUID();
	data << uint8(extra_cast_number);
	data << uint32(m_spellInfo->Id);
	data << uint8(result);
	m_caster->SendMessageToSet(&data, false);
}

void Spell::SendChannelUpdate(uint32 time)
{
	if(time == 0)
	{
		if(m_caster->IsUnit() && ((Unit *)m_caster)->IsInWorld() && ((Unit *)m_caster)->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT))
		{
			DynamicObject* dynObj = new (((Unit *)m_caster)->GetMapMgr()->GetDynamicObject(((Unit *)m_caster)->GetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT))) DynamicObject(NULL, NULL);
			if(dynObj)
			{
				dynObj->RemoveFromWorld(true);
				delete dynObj;
				dynObj = NULL;
				//dynObj->m_aliveDuration = 1;
			}
		}
		if( m_caster->IsPlayer() && ((Player *)m_caster)->IsInWorld() && ((Player *)m_caster)->GetUInt64Value(PLAYER_FARSIGHT) )
		{
			DynamicObject* dynObj = new (((Player *)m_caster)->GetMapMgr()->GetDynamicObject(((Player *)m_caster)->GetUInt32Value(PLAYER_FARSIGHT))) DynamicObject(NULL, NULL);
			if( dynObj )
			{
				dynObj->RemoveFromWorld(true);
				delete dynObj;
				dynObj = NULL;
				((Player *)m_caster)->SetUInt32Value(PLAYER_FARSIGHT, 0);
			}
			((Player *)m_caster)->SetUInt64Value(PLAYER_FARSIGHT, 0);
			((Player *)m_caster)->GetMapMgr()->ChangeFarsightLocation(((Player *)m_caster), ((Player *)m_caster)->GetPositionX(), ((Player *)m_caster)->GetPositionY(), false);
		}

		((Unit *)m_caster)->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT,0);
		((Unit *)m_caster)->SetUInt32Value(UNIT_CHANNEL_SPELL,0);
	}

	if (!m_caster->IsPlayer()) return;

	/*WorldPacket data(MSG_CHANNEL_UPDATE, 18);*/
	uint8 buf[20];
	StackPacket data(MSG_CHANNEL_UPDATE, buf, 20);

	data << ((Player *)m_caster)->GetNewGUID();
	data << time;	
	((Player *)m_caster)->SendMessageToSet(&data, true);	
}

void Spell::SendChannelStart(uint32 duration)
{
	if (m_caster->GetTypeId() != TYPEID_GAMEOBJECT)
	{
		// Send Channel Start
		/*WorldPacket data(MSG_CHANNEL_START, 22);*/
		uint8 buf[30];
		StackPacket data(MSG_CHANNEL_START, buf, 30);
		data << m_caster->GetNewGUID();
		data << m_spellInfo->Id;
		data << duration;
		m_caster->SendMessageToSet(&data, true);
	}

	m_castTime = m_timer = duration;

	if( m_caster->IsUnit() )
		((Unit *)m_caster)->SetUInt32Value(UNIT_CHANNEL_SPELL,m_spellInfo->Id);

	/*
	Unit* target = objmgr.GetCreature( static_cast< Player* >( m_caster )->GetSelection());
	if(!target)
		target = objmgr.GetObject<Player>( static_cast< Player* >( m_caster )->GetSelection());
	if(!target)
		return;
 
	m_caster->SetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT,target->GetGUIDLow());
	m_caster->SetUInt32Value(UNIT_FIELD_CHANNEL_OBJECT+1,target->GetGUIDHigh());
	//disabled it can be not only creature but GO as well
	//and GO is not selectable, so this method will not work
	//these fields must be filled @ place of call
	*/
}

void Spell::SendResurrectRequest(Player* target)
{
	//Log.Warning("SMSG_RESURRECT_REQUEST","Target: %s (Entry: %u - Entry Caster: %u)",target->GetName(),target->GetEntry(),m_caster->GetEntry());

	WorldPacket data(SMSG_RESURRECT_REQUEST, 13);
	data << m_caster->GetGUID();
	data << uint32(0) << uint8(0);
	target->GetSession()->SendPacket(&data);
}

bool Spell::HasPower()
{
	//trainers can always cast
	if( m_caster->IsUnit() && ((Unit *)m_caster)->HasFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_TRAINER) )
		return true;
	//Powercheaters too
	if(m_caster->IsPlayer() && ((Player *)m_caster)->PowerCheat) return true;

	//Seems to be an issue since 3.0.9, as many elixers/potions got powertype 4 
	//Haven't found any items taking power, so guess it's safe to skip them.
	if(m_caster->IsItem())  return(true); 

	int32 powerField;
	//Log.Warning( "Spell::HasPower","Spell %u : Power type = %u", m_spellInfo->Id ,m_spellInfo->powerType); // Debug (Branruz)
	switch(m_spellInfo->powerType)
	{
	case POWER_TYPE_HEALTH:	{ powerField = UNIT_FIELD_HEALTH; }break;
	case POWER_TYPE_MANA:	{ powerField = UNIT_FIELD_POWER1; m_usesMana=true; }break;
	case POWER_TYPE_RAGE:	{ powerField = UNIT_FIELD_POWER2; }break;
	case POWER_TYPE_FOCUS:	{ powerField = UNIT_FIELD_POWER3; }break;
	case POWER_TYPE_ENERGY:	{ powerField = UNIT_FIELD_POWER4; }break;
	case POWER_TYPE_BUFF_RESISTANCE:
							{ powerField = UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE_1; }break; // Randdrick
	case POWER_TYPE_RUNES:
		{
			if(m_spellInfo->runeCostID && m_caster->IsPlayer())
			{
				RuneCostEntry * runecost = dbcSpellRuneCost.LookupEntry(m_spellInfo->runeCostID);
				if( !((Player *)m_caster)->CanUseRunes( runecost->blood, runecost->frost, runecost->unholy) )
					return false;
			}
			return true;
		}
	case POWER_TYPE_RUNIC_POWER:	{ powerField = UNIT_FIELD_POWER7; }break;
	default:{
		sLog.outDebug("Spell","unknown power type %d", m_spellInfo->powerType);
		// we should'nt be here to return
		return false;
			}break;
	}
    //Log.Warning( "Spell::HasPower","Spell %u : Current PowerType Player %u", m_spellInfo->Id ,powerField); // Debug (Branruz)
	
	int32 currentPower = m_caster->GetUInt32Value(powerField);
	//Log.Warning( "Spell::HasPower","Spell %u : Current Power Player     %u", m_spellInfo->Id ,currentPower); // Debug (Branruz)

	int32 cost = m_caster->GetSpellBaseCost(m_spellInfo);
    //Log.Warning( "Spell::HasPower","Spell %u : Current Spell Cost       %d", m_spellInfo->Id ,cost); // Debug (Branruz)
	
	/*if( m_spellInfo->ManaCostPercentage)//Percentage spells cost % of !!!BASE!!! mana
	{
		if( m_spellInfo->powerType==POWER_TYPE_MANA)
			cost = (m_caster->GetUInt32Value(UNIT_FIELD_BASE_MANA)*m_spellInfo->ManaCostPercentage)/100;
		else
			cost = (m_caster->GetUInt32Value(UNIT_FIELD_BASE_HEALTH)*m_spellInfo->ManaCostPercentage)/100;
	}
	else 
	{
		cost = m_spellInfo->manaCost;
	}*/
    if((int32)m_spellInfo->powerType == POWER_TYPE_HEALTH)
		cost -= m_spellInfo->baseLevel;//FIX for life tap	
	else if( m_caster->IsUnit() )
	{
		if( m_spellInfo->powerType == POWER_TYPE_MANA)
		{
			cost += ((Unit *)m_caster)->PowerCostMod[m_spellInfo->School];//this is not percent!
			//Log.Warning( "Spell::HasPower","Spell %u : Current Cost with modifier %d", m_spellInfo->Id ,cost); // Debug (Branruz)
		}
		else
			cost += ((Unit *)m_caster)->PowerCostMod[0];

		cost +=float2int32(cost*((Unit *)m_caster)->GetFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER+m_spellInfo->School));
	}

	//apply modifiers
	if(m_caster->IsUnit())
	{
	 if( m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2])
	 {
		SM_FIValue(((Unit *)m_caster)->SM_FCost,&cost,m_spellInfo);
		SM_PIValue(((Unit *)m_caster)->SM_PCost,&cost,m_spellInfo);
		//Log.Warning( "Spell::HasPower","Spell %u : Current Cost with SpellGrpMod %d", m_spellInfo->Id ,cost); // Debug (Branruz)
	 }
	}

	if (cost <=0)
	{
		m_usesMana = false; // no mana regen interruption for free spells
		return true;
	}

	//Stupid shiv
	if( m_spellInfo->NameHash == SPELL_HASH_SHIV )
	{
		Item *Offhand = ((Player *)m_caster)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND);

		if( Offhand != NULL && Offhand->GetProto() != NULL )
			cost += Offhand->GetProto()->Delay / 100;
	}

	//FIXME:DK:if field value < cost what happens
	if(powerField == UNIT_FIELD_HEALTH)
	{
		return true;
	}
	else
	{
		if(cost <= currentPower) // Unit has enough power (needed for creatures)
		{
			return true;
		}
		else
			return false;	 
	} 
}

bool Spell::TakePower()
{
	//trainers can always cast

	//Powercheaters too
	if(m_caster->IsPlayer() && ((Player *)m_caster)->PowerCheat)
		return true;

	if( m_caster->IsUnit() )
	{
	 if(((Unit *)m_caster)->HasFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_TRAINER))
		return true;
	}
	
	//Seems to be an issue since 3.0.9, as many elixers/potions got powertype 4 
	//Haven't found any items taking power, so guess it's safe to skip them.
	if(m_caster->IsItem()) return true; 

	int32 powerField;
	switch(m_spellInfo->powerType)
	{
		case POWER_TYPE_HEALTH:{ powerField = UNIT_FIELD_HEALTH; } break;
		case POWER_TYPE_MANA  :{ powerField = UNIT_FIELD_POWER1; m_usesMana=true; } break;
		case POWER_TYPE_RAGE  :{ powerField = UNIT_FIELD_POWER2; } break;
		case POWER_TYPE_FOCUS :{ powerField = UNIT_FIELD_POWER3; } break;
		case POWER_TYPE_ENERGY:{ powerField = UNIT_FIELD_POWER4; } break;
		case POWER_TYPE_BUFF_RESISTANCE:{ powerField = UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE_1; } break; //Randdrick
		case POWER_TYPE_RUNES :
			{ 
			 if(m_spellInfo->runeCostID && m_caster->IsPlayer())
			 {
				RuneCostEntry * runecost = dbcSpellRuneCost.LookupEntry(m_spellInfo->runeCostID);
				if( !((Player *)m_caster)->CanUseRunes( runecost->blood, runecost->frost, runecost->unholy) )
					return false;

				((Player *)m_caster)->UseRunes( runecost->blood, runecost->frost, runecost->unholy, m_spellInfo);
				if(runecost->runePowerGain)
					((Player *)m_caster)->SetPower(POWER_TYPE_RUNIC_POWER, runecost->runePowerGain + ((Player *)m_caster)->GetUInt32Value(UNIT_FIELD_POWER7));
			 }
			 return true;
		}break;
		default:
		{
			Log.Error("Spell","Unknown power type %u for spell %u", m_spellInfo->powerType, m_spellInfo->Id);
			// we shouldn't be here to return
			return false;
		}break;
	}

	//FIXME: add handler for UNIT_FIELD_POWER_COST_MODIFIER
	//UNIT_FIELD_POWER_COST_MULTIPLIER

	int32 cost;
	int32 currentPower = m_caster->GetUInt32Value(powerField);

	cost = m_caster->GetSpellBaseCost(m_spellInfo);

	if( m_caster->IsUnit() )
	{
		if( m_spellInfo->attributesEx & ATTRIBUTESEX_DRAIN_WHOLE_MANA ) // Uses %100 mana
		{
			m_caster->SetUInt32Value(powerField, 0);
			return true;
		}
		cost += ((Unit *)m_caster)->PowerCostMod[ m_usesMana ? m_spellInfo->School : 0 ];//this is not percent!
		cost += float2int32(float(cost)* ((Unit *)m_caster)->GetFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER + m_spellInfo->School));
	}
	/*if( m_spellInfo->ManaCostPercentage)//Percentage spells cost % of !!!BASE!!! mana
	{
		if( m_spellInfo->powerType==POWER_TYPE_MANA)
			cost = (m_caster->GetUInt32Value(UNIT_FIELD_BASE_MANA)*m_spellInfo->ManaCostPercentage)/100;
		else
			cost = (m_caster->GetUInt32Value(UNIT_FIELD_BASE_HEALTH)*m_spellInfo->ManaCostPercentage)/100;
	}
	
	else 
	{
		cost = m_spellInfo->manaCost;
	}
	*/

	if( powerField == UNIT_FIELD_HEALTH )
		cost -= m_spellInfo->baseLevel;//FIX for life tap	

	//apply modifiers
	if(m_caster->IsUnit())
	{
	 if( m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2])
	 {
		SM_FIValue(((Unit *)m_caster)->SM_FCost,&cost,m_spellInfo);
		SM_PIValue(((Unit *)m_caster)->SM_PCost,&cost,m_spellInfo);
	 }
	}
		 
	//Stupid shiv
	if( m_spellInfo->NameHash == SPELL_HASH_SHIV )
	{
		Item *Offhand = ((Player *)m_caster)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND);

		if( Offhand != NULL && Offhand->GetProto() != NULL )
			cost += Offhand->GetProto()->Delay / 100;
	}

	if (cost <=0)
	{
		m_usesMana = false; // no mana regen interruption for free spells
		return true;
	}

	if( m_caster->IsUnit() && m_spellInfo->powerType == POWER_TYPE_MANA )
		((Unit *)m_caster)->m_LastSpellManaCost = cost;

	//FIXME:DK:if field value < cost what happens
	if(powerField == UNIT_FIELD_HEALTH)
	{
		m_caster->DealDamage(((Unit *)m_caster), cost, 0, 0, 0,true);
		return true;
	}
	else
	{
		if(cost <= currentPower) // Unit has enough power (needed for creatures)
		{
			if(m_caster->IsUnit())
				((Unit *)m_caster)->SetPower(m_spellInfo->powerType, currentPower - cost);
			else // Is this needed at all Do GO's use mana? 
				m_caster->SetUInt32Value(powerField, currentPower - cost);
			return true;
		}
	}
	return false;
}

void Spell::HandleCastEffects(Object* obj, uint32 i, bool reflected)
{
	Object* target = NULL;
	float dist=0.0f;
	//if(obj == NULL) Log.Notice("[Spell::HandleCastEffects]","Obj == NULL");
	//else            Log.Notice("[Spell::HandleCastEffects]","Obj existe!");
	
	// Pour GameObject CHEST / GOOBER
	if (m_spellInfo->speed <=0)
	{
		HandleEffects(obj, i, reflected);
		return;
	}
	//----------------------------------
		

	if(obj == NULL || obj->GetGUID() == m_caster->GetGUID())
	{
		if (m_targets.m_targetMask & TARGET_FLAG_SOURCE_LOCATION)
			dist = m_caster->CalcDistance(m_targets.m_srcX, m_targets.m_srcY, m_targets.m_srcZ);
	}
	else
	{
		if(!m_caster->IsInWorld() || m_targets.m_targetMask & TARGET_FLAG_TRADE_ITEM )
		{
			HandleEffects(obj, i, reflected);
			return;
		}
		else
		{
			target = obj;

			if (target != NULL)
				dist=m_caster->CalcDistance(target);
		}
	}

	if (dist == 0)
		HandleEffects(obj, i, reflected);
	else
	{
		float time = dist * 1000.0f / m_spellInfo->speed;
		if (reflected) time *= 1.25;

		if(time <= 100)
			HandleEffects(obj, i, reflected);
		else
		 sEventMgr.AddEvent(this, &Spell::HandleEffects, obj, i, reflected, EVENT_SPELL_HIT, float2int32(time), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// sEventMgr.AddEvent(this, &Spell::HandleEffects, obj, i, reflected, EVENT_SPELL_HIT, float2int32(time), 1, 0);
	}
	//Log.Notice("[Spell::HandleCastEffects]","OK");
}


void Spell::HandleEffects(Object* obj, uint32 i, bool reflected)
{	
	//Log.Notice("[Spell:HandleEffects]","In Progress: %5d, Num %d, Effect: %5d",m_spellInfo->Id,i,m_spellInfo->Effect[i]);
	//uh oh, were we deleted? naughty naughty
	if (event_GetInstanceID() == WORLD_INSTANCE) 
	{
		//Log.Notice("[Spell:HandleEffects]","Echec: WORLD_INSTANCE");
		return;
	}

	if(m_caster->m_ConstanteId != _CONST_ID_) 
	{
		Log.Error("HandleEffects","Leak m_caster!! <-- Report this to devs.");
		return;
	}

	/*
	if(!m_caster) Log.Warning("[Spell:HandleEffects]","Caster = NULL");
	else     Log.Warning("[Spell:HandleEffects]","Caster = %d",m_caster->GetEntry());

	if(!obj) Log.Warning("[Spell:HandleEffects]","Obj = NULL");
	else     Log.Warning("[Spell:HandleEffects]","Obj = %d",obj->GetEntry());

	if(!m_targets.m_target) Log.Warning("[Spell:HandleEffects]","m_target = NULL");
	else     Log.Warning("[Spell:HandleEffects]","m_target = %d",m_targets.m_target->GetEntry());
    */


	//duel immunity?
	if (obj != NULL && obj->IsUnit() &&  static_cast<Unit*>(obj)->m_duelImmunity && duelSpell)
		return;

	if (m_spellScript != NULL)
		m_spellScript->OnEffect(i);


	//these effects can be done with a NULL target :P
	if (obj == NULL)
	{
		switch (m_spellInfo->Effect[i])
		{ 
		//case SPELL_EFFECT_APPLY_AURA : // Pour appliquer une Aura fo obligatoirement une cible (Branruz)
		case SPELL_EFFECT_SUMMON:
		case SPELL_EFFECT_PERSISTENT_AREA_AURA:
		case SPELL_EFFECT_TRIGGER_MISSILE:
		case SPELL_EFFECT_APPLY_AREA_AURA:
		case SPELL_EFFECT_JUMP_MOVE:
		case SPELL_EFFECT_APPLY_GLYPH:
			break;

		default:
			// Test (Branruz)
			// Si Obj est NULL et que le Caster = la Cible, unitTarget = Caster (ou Cible)
			if(m_caster && m_targets.m_target) 
			{
				if(m_caster->GetEntry() ==  m_targets.m_target->GetEntry()) 
				{
                 // Log.Warning("[Spell:HandleEffects]","Correction (%u): Obj = Caster (NULL Obj) m_spellInfo->Effect[%u]=%u",
			     //                                                         	m_spellInfo->Id,i,m_spellInfo->Effect[i]);
				   unitTarget = (Unit *)m_caster; // ou unitTarget = m_targets.m_target;
				   if(m_caster->IsPlayer()) playerTarget  = (Player *)m_caster; 
                   
				}
			}
			else 
			{
			  Log.Warning("[Spell:HandleEffects]","Echec (%u): Invalide (NULL Obj) m_spellInfo->Effect[%u]=%u",
			                                                              	m_spellInfo->Id,i,m_spellInfo->Effect[i]);
			  return;
			}
		}
	}


	if(obj != NULL && obj->GetGUID() == m_caster->GetGUID())
	{
		unitTarget    = NULL;
		playerTarget  = NULL;
		itemTarget    = NULL;
		gameObjTarget = NULL;
		corpseTarget  = NULL;

		if(m_caster->IsPlayer())    { playerTarget  = (Player *)m_caster; unitTarget = (Unit * )m_caster; }
		else if(m_caster->IsUnit()) unitTarget    = (Unit * )m_caster;
		else if(m_caster->IsGO())   gameObjTarget = (GameObject *)m_caster;
		else if(m_caster->IsItem()) itemTarget    = (Item *)m_caster;
		else 
		{
			Log.Error("Spell::HandleEffects","=> **** Report this to Devs **** <=");
			Log.Error("                    ","m_caster inconnu => (Entry: %u)",m_caster->GetEntry());

		}
		
	}
	else
	{
		if(!m_caster->IsInWorld())
		{
			unitTarget    = NULL;
			playerTarget  = NULL;
			itemTarget    = NULL;
			gameObjTarget = NULL;
			corpseTarget  = NULL;
		}
		else if (obj != NULL)
		{
			//players changing maps won't be hit by old spell projectiles
			if (obj->event_GetInstanceID() != event_GetInstanceID())
				return;
		
			switch(GET_TYPE_FROM_GUID(obj->GetGUID()))
			{
			case HIGHGUID_TYPE_UNIT:
			case HIGHGUID_TYPE_VEHICLE: // 214
			case HIGHGUID_TYPE_PET:
				unitTarget = static_cast<Unit*>(obj);
				break;
			case HIGHGUID_TYPE_PLAYER:
				unitTarget =  static_cast<Unit*>(obj);
				playerTarget = static_cast< Player* >(unitTarget);
				break;
			case HIGHGUID_TYPE_ITEM:
				itemTarget = static_cast<Item*>(obj);
				break;
			case HIGHGUID_TYPE_GAMEOBJECT:
				gameObjTarget = static_cast<GameObject*>(obj);
				break;
			case HIGHGUID_TYPE_CORPSE:
				corpseTarget = static_cast<Corpse*>(obj);
				break;
			default:
				Log.Error("Spell::HandleEffects","=> **** Report this to Devs **** <=");
				Log.Error("                    ","m_caster inconnu => (Entry: %u)",m_caster->GetEntry());
				return;					
			}
		}
	}	
	
	//reflected spells :P
	if (reflected)
	{
		if(!m_caster->IsUnit()) return;
		if(unitTarget == NULL) 	return;

		m_backupTarget = unitTarget;
		unitTarget = ((Unit *)m_caster);
	}

	
	if( m_spellInfo->Effect[i]<TOTAL_SPELL_EFFECTS)
	{
		//Log.Notice("[Spell:HandleEffects]","ApplyAuraName %u index %u",m_spellInfo->EffectApplyAuraName[i],i);
		
		damage = 0;
		if (m_caster->IsUnit()) 
			((Unit *)m_caster)->HandleProc(m_procdata, unitTarget, m_spellInfo);

		if (unitTarget)
		{
			SpellTargetMap::iterator smitr = m_spellTargets.find(unitTarget);
			if (smitr != m_spellTargets.end())
			{
				unitTarget->HandleProc(smitr->second.ProcData, ((Unit *)m_caster), m_spellInfo);
				damage = smitr->second.EffectPoints[i];
			}
			else
				damage = CalculateEffect(i, unitTarget);
		}
		else
			damage = CalculateEffect(i, NULL);

	    (*this.*SpellEffectsHandler[m_spellInfo->Effect[i]])(i);
		
		if (m_caster->IsUnit())
			((Unit *)m_caster)->m_procCounter = 0;

		if (unitTarget)
			unitTarget->m_procCounter = 0;

		//Log.Notice("[Spell:HandleEffects]","Ok");
	}
	else Log.Error("Spell","Id %u, Effect%u %u Inconnu <-- Report this to devs.", m_spellInfo->Id,i,m_spellInfo->Effect[i]);
}
void Spell::SetSpellTargets(const uint64& guid)
{
	_SetTargets(guid);
}


void Spell::_SetTargets(const uint64& guid)
{
	unitTarget = NULL;
	playerTarget = NULL;
	gameObjTarget = NULL;
	corpseTarget = NULL;
	itemTarget = NULL;

	if(guid == 0)
	{
		if(!m_caster->IsInWorld())
			return;

		if(m_caster->IsPlayer())
		{
			if(m_targets.m_targetMask & TARGET_FLAG_ITEM)
				itemTarget = ((Player *)m_caster)->GetItemInterface()->GetItemByGUID(m_targets.m_target->GetGUID());
			if(m_targets.m_targetMask & TARGET_FLAG_TRADE_ITEM)
			{
				Player* p_trader = ((Player *)m_caster)->GetTradeTarget();
				if(p_trader != NULL)
					itemTarget = p_trader->getTradeItem((uint32)m_targets.m_target->GetGUID());
			}
		}

		if(m_targets.m_targetMask & TARGET_FLAG_UNIT && m_targets.m_target->GetGUID())
		{
			MapMgr* mgr = m_caster->GetMapMgr();
			switch(GET_TYPE_FROM_GUID(m_targets.m_target->GetGUID()))
			{
			case HIGHGUID_TYPE_VEHICLE:
				unitTarget = mgr->GetVehicle(GET_LOWGUID_PART(m_targets.m_target->GetGUID()));
				break;
			case HIGHGUID_TYPE_UNIT:
				unitTarget = mgr->GetCreature(GET_LOWGUID_PART(m_targets.m_target->GetGUID()));
				break;
			case HIGHGUID_TYPE_PET:
				unitTarget = mgr->GetPet(GET_LOWGUID_PART(m_targets.m_target->GetGUID()));
				break;
			case HIGHGUID_TYPE_PLAYER:
				{
					unitTarget = mgr->GetPlayer((uint32)m_targets.m_target->GetGUID());
					playerTarget = static_cast<Player *>(unitTarget);
				}break;
			case HIGHGUID_TYPE_GAMEOBJECT:
				gameObjTarget = mgr->GetGameObject(GET_LOWGUID_PART(m_targets.m_target->GetGUID()));
				break;
			case HIGHGUID_TYPE_CORPSE:
				corpseTarget = objmgr.GetCorpse((uint32)m_targets.m_target->GetGUID());
				break;
			}
		}
	}
	else if(guid == m_caster->GetGUID())
	{
		if(m_caster->IsPlayer())    playerTarget = (Player *)m_caster;      // en 1ier
		if(m_caster->IsUnit())      unitTarget = (Unit *)m_caster;          // en 2ieme , pas de 'else', Player est aussi Unit
		else if(m_caster->IsGO())   gameObjTarget = (GameObject *)m_caster; // en 3ieme
		else if(m_caster->IsItem()) // en 4ieme
		{
			if(m_targets.m_targetMask & TARGET_FLAG_ITEM)
				itemTarget = ((Player *)m_caster)->GetItemInterface()->GetItemByGUID(m_targets.m_target->GetGUID());
			if(m_targets.m_targetMask & TARGET_FLAG_TRADE_ITEM)
			{
				Player* p_trader = ((Player *)m_caster)->GetTradeTarget();
				if(p_trader != NULL)
					itemTarget = p_trader->getTradeItem((uint32)m_targets.m_target->GetGUID());
			} 
		}
	}
	else
	{
		if(!m_caster->IsInWorld())
		{
			return;
		}
		else
		{
  		 switch(GET_TYPE_FROM_GUID(guid))
		 {
			case HIGHGUID_TYPE_VEHICLE:
				unitTarget = m_caster->GetMapMgr()->GetVehicle(GET_LOWGUID_PART(guid));
				break;
			case HIGHGUID_TYPE_UNIT:
				unitTarget = m_caster->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
				break;
			case HIGHGUID_TYPE_PET:
				unitTarget = m_caster->GetMapMgr()->GetPet(GET_LOWGUID_PART(guid));
				break;
			case HIGHGUID_TYPE_PLAYER:
				{
					unitTarget =  m_caster->GetMapMgr()->GetPlayer((uint32)guid);
					playerTarget = static_cast<Player *>(unitTarget);
				}break;
			case HIGHGUID_TYPE_ITEM:
				if( m_caster->IsPlayer() )
					itemTarget = ((Player *)m_caster)->GetItemInterface()->GetItemByGUID(guid);
				break;
			case HIGHGUID_TYPE_GAMEOBJECT:
				gameObjTarget = m_caster->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
				break;
			case HIGHGUID_TYPE_CORPSE:
				corpseTarget = objmgr.GetCorpse((uint32)guid);
				break;
			}
		}
	}
}

/*void Spell::NewHandleEffects(uint32 i)
{   
		
	// Try a dummy SpellHandler
	if(sScriptMgr.CallScriptedDummySpell(m_spellInfo->Id, i, this ))
	{
		sLog.outDebug( "Spell","Redirecting Spell %u Effect id = %u to sScriptMgr", m_spellInfo->Id, m_spellInfo->Effect[i]); 
		return;
	}
	
	//if(!unitTarget && m_caster->IsPlayer())  
	//{
	//  unitTarget = ((Unit *)m_caster);
	//	_SetTargets(   targets->m_target->GetGUID());
	//}

	if(unitTarget) damage = CalculateEffect(i,unitTarget);  
	//Log.Warning( "Spell","Handling Spell %d, New Effect id = %u, damage = %d",m_spellInfo->Id,m_spellInfo->Effect[i], damage); 
	
	if( m_spellInfo->Effect[i]<TOTAL_SPELL_EFFECTS)
	{
	    if(i) // 2ieme ou 3ieme SpellEffect
		{
			if(m_spellInfo->Effect[i] == m_spellInfo->Effect[i-1]) // Le meme que celui d'avant ?
			{
				Log.Warning("Spell::HandleEffects","SpellEffect %d en double, Spell %d Index %d et %d",
					m_spellInfo->Effect[i],m_spellInfo->Id,i-1,i);
				return;
			}
		}
		(*this.*SpellEffectsHandler[m_spellInfo->Effect[i]])(i);
	}
	else
		Log.Error("Spell","Unknown effect %u spellid %u",m_spellInfo->Effect[i], m_spellInfo->Id);
}*/

void Spell::NewHandleAddAura(uint64 guid)
{
	Unit *Target = NULL;
	if(guid == 0)
		return;

	if(m_caster->IsUnit() && ((Unit *)m_caster)->GetGUID() == guid)
		Target = ((Unit *)m_caster);
	else if(m_caster->IsInWorld())
		Target = m_caster->GetMapMgr()->GetUnit(guid);

	if(!Target) return;

	// Applying an aura to a flagged target will cause you to get flagged.
	// self casting doesnt flag himself.
	if(Target->IsPlayer() && m_caster->IsPlayer() && (((Player *)m_caster) != static_cast<Player *>(Target)) )
	{
		if(static_cast<Player *>(Target)->IsPvPFlagged())
		{
			((Player *)m_caster)->SetPvPFlag();
			((Player *)m_caster)->SetPVPCombat();
		}
	}
	
	// remove any auras with same type
	if( m_spellInfo->buffType > 0)
		Target->RemoveAurasByBuffType(m_spellInfo->buffType, m_caster->GetGUID(),m_spellInfo->Id);
		
	uint32 spellid = 0;

	if( m_spellInfo->mechanics == 25 && m_spellInfo->Id != 25771 || m_spellInfo->Id == 31884 ) // Cast spell Forbearance
	{
		if( m_spellInfo->Id != 31884 )
			spellid = 25771;

		if( Target->IsPlayer() )
		{
			sEventMgr.AddEvent(static_cast<Player *>(Target), &Player::AvengingWrath, EVENT_PLAYER_AVENGING_WRATH, 30000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);	
			static_cast<Player *>(Target)->mAvengingWrath = false;
		}
	}
	else if( m_spellInfo->mechanics == 16 && m_spellInfo->Id != 11196) // Cast spell Recently Bandaged
		spellid = 11196;
	else if( m_spellInfo->mechanics == 19 && m_spellInfo->Id != 6788) // Cast spell Weakened Soul
		spellid = 6788;
	else if( m_spellInfo->Id == 45438) // Cast spell Hypothermia
		spellid = 41425;
	else if (m_caster->IsPlayer() && m_spellInfo->Id == 34754 && ((Player *)m_caster)->HasSpell(47549))//Improved Holy Concentration 3.0.9 on 3.1.0 doesn't exist
		spellid = 47894;
	else if (m_caster->IsPlayer() && m_spellInfo->Id == 34754 && ((Player *)m_caster)->HasSpell(47551))//Improved Holy Concentration 3.0.9 on 3.1.0 doesn't exist
		spellid = 47895;
	else if (m_caster->IsPlayer() && m_spellInfo->Id == 34754 && ((Player *)m_caster)->HasSpell(47552))//Improved Holy Concentration 3.0.9 on 3.1.0 doesn't exist
		spellid = 47896;
	else if( m_spellInfo->AdditionalAura )
		spellid = m_spellInfo->AdditionalAura;
	else if( m_spellInfo->NameHash == SPELL_HASH_HEROISM )
		spellid = 57723;
	else if( m_spellInfo->NameHash == SPELL_HASH_BLOODLUST )
		spellid = 57724;
	else if( m_spellInfo->NameHash == SPELL_HASH_STEALTH )
	{
		if( Target->HasDummyAura(SPELL_HASH_MASTER_OF_SUBTLETY) )
			spellid = 31665;
	}
	else if( m_spellInfo->Id == 62124 && m_caster->IsUnit() )
	{
		if( ((Unit *)m_caster)->HasDummyAura(SPELL_HASH_VINDICATION) )
			spellid = ((Unit *)m_caster)->GetDummyAura(SPELL_HASH_VINDICATION)->RankNumber == 2 ? 26017 : 67;
	}
	else if( m_spellInfo->Id == 5229 &&
		m_caster->IsPlayer() && (
		((Player *)m_caster)->GetShapeShift() == FORM_BEAR ||
		((Player *)m_caster)->GetShapeShift() == FORM_DIREBEAR ) &&
		((Player *)m_caster)->HasDummyAura(SPELL_HASH_KING_OF_THE_JUNGLE) )
	{
		SpellEntry *spellInfo = dbcSpell.LookupEntry( 51185 );
		if(!spellInfo) 
			return;

		Spell *spell = new Spell(((Player *)m_caster), spellInfo ,true, NULL);
		spell->forced_basepoints[0] = ((Player *)m_caster)->GetDummyAura(SPELL_HASH_KING_OF_THE_JUNGLE)->RankNumber * 5;
		SpellCastTargets targets(m_caster); // SpellCastTargets targets(p_caster->GetGUID());
		spell->prepare(&targets);
	}
	else if( m_spellInfo->Id == 19574 )
	{
		if( ((Unit *)m_caster)->HasDummyAura(SPELL_HASH_THE_BEAST_WITHIN) )
			((Unit *)m_caster)->CastSpell(((Unit *)m_caster), 34471, true);
	}
	else if( m_spellInfo->NameHash == SPELL_HASH_RAPID_KILLING )
	{
		if( ((Unit *)m_caster)->HasDummyAura(SPELL_HASH_RAPID_RECUPERATION) )
			m_spellInfo->Id = 56654;
	}

	switch( m_spellInfo->NameHash )
	{
	case SPELL_HASH_CLEARCASTING:
	case SPELL_HASH_PRESENCE_OF_MIND:
		{
			if( Target->HasDummyAura(SPELL_HASH_ARCANE_POTENCY) )
				spellid = Target->GetDummyAura(SPELL_HASH_ARCANE_POTENCY)->RankNumber == 1 ? 57529 : 57531;
		}break;
	}

	if( spellid && Target )
	{
		SpellEntry *spellInfo = dbcSpell.LookupEntry( spellid );
		if(!spellInfo)
			return;

		Spell *spell = new Spell(Target, spellInfo ,true, NULL);
		if( spellid == 31665 && Target->HasDummyAura(SPELL_HASH_MASTER_OF_SUBTLETY) )
			spell->forced_basepoints[0] = Target->GetDummyAura(SPELL_HASH_MASTER_OF_SUBTLETY)->EffectBasePoints[0];

		SpellCastTargets targets(Target); // SpellCastTargets targets(Target->GetGUID());
		spell->prepare(&targets);
	}

	if( m_spellInfo->mechanics == 31 )
		Target->SetFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_ENRAGE);

	// avoid map corruption
	if(Target->GetInstanceID() != m_caster->GetInstanceID())
		return;

	std::map<uint32,Aura* >::iterator itr=Target->tmpAura.find(GetSpellProto()->Id);
	if(itr!=Target->tmpAura.end())
	{
		Aura* aura = itr->second;
		if(aura != NULL)
		{
			// did our effects kill the target?
			if( Target->isDead() && !(m_spellInfo->attributesExC & ATTRIBUTESEXC_CAN_PERSIST_AND_CASTED_WHILE_DEAD))
			{
				// free pointer
				aura->m_deletedfromtmp = true;
				aura->Remove();
				itr->second = NULL;
				Target->tmpAura.erase(itr);
				return;
			}

			//make sure bg/arena preparation aura's are positive.
			if(m_spellInfo->Id == 32727 || m_spellInfo->Id == 44521)
				aura->SetPositive(100);

			Target->AddAura(aura, NULL);
			if(!aura->m_deletedfromtmp && !Target->tmpAura.empty())
				Target->tmpAura.erase(itr);
		}
	}
}

void Spell::HandleAddAura(Unit* u, Aura* a)
{
	Log.Notice("[Spell::HandleAddAura]","In progress... ");
	if (u == NULL || a == NULL) return;

	Object* caster = NULL;

	if (m_caster->IsPlayer())
		caster = ((Player *)m_caster);
	else
		caster = ((Unit *)m_caster);

	/*Log.Notice("[Spell::HandleAddAura]","Unit %s Caster %s Aura %u",
		u->GetPlayerFrom()->GetName(),
		a->GetCaster()->GetPlayerFrom()->GetName(),
		a->GetSpellId());*/

	//cant add auras to someone in different instance/map
	if (m_caster->event_GetInstanceID() != u->event_GetInstanceID())
		return;

	// Applying an aura to a flagged target will cause you to get flagged.
    // self casting doesnt flag himself.
	if(u->IsPlayer() && m_caster->IsPlayer() && ((Unit *)m_caster != u) )
	{
		if(static_cast< Player* >(u)->IsPvPFlagged())
		{
			((Player *)m_caster)->SetPvPFlag();
			((Player *)m_caster)->SetPVPCombat();
		}
	}

	// remove any auras with same type
	if( m_spellInfo->buffType > 0)
		u->RemoveAurasByBuffType(m_spellInfo->buffType, m_caster->GetGUID(),m_spellInfo->Id);

	uint32 spellid = 0;
                                    
	if( (m_spellInfo->mechanics == MECHANIC_INVULNARABLE) && (m_spellInfo->Id != __Forbearance)) // Cast spell Forbearance
		spellid = __Forbearance;
	else if( m_spellInfo->NameHash == SPELL_HASH_AVENGING_WRATH )
		spellid = __Forbearance;            
	else if( (m_spellInfo->mechanics == MECHANIC_HEALING) && m_spellInfo->Id != __Recently_Bandaged) // Cast spell Recently Bandaged
		spellid = __Recently_Bandaged;
	else if( m_spellInfo->Id == __Arcane_Blast_Rank_1) // Cast spell Arcane Blast
		spellid = 36032; // __Arcane_Blast___12
	else if( m_spellInfo->Id == __Blood_Fury_Racial || 
		     m_spellInfo->Id == __Blood_Fury_Racial____2 || // 33702
			 m_spellInfo->Id == __Blood_Fury_Racial____1 )  // 33697 // Cast spell Blood Fury
		spellid = __Blood_Fury_Racial____0; // 23230
	else if( m_spellInfo->Id == 45438) // Cast spell Hypothermia
		spellid = 41425;
	else if (m_caster->IsPlayer() && m_spellInfo->Id == 34754 && ((Player *)m_caster)->HasSpell(47549))//Improved Holy Concentration 3.0.9 on 3.1.0 doesn't exist
		spellid = 47894;
	else if (m_caster->IsPlayer() && m_spellInfo->Id == 34754 && ((Player *)m_caster)->HasSpell(47551))//Improved Holy Concentration 3.0.9 on 3.1.0 doesn't exist
		spellid = 47895;
	else if (m_caster->IsPlayer() && m_spellInfo->Id == 34754 && ((Player *)m_caster)->HasSpell(47552))//Improved Holy Concentration 3.0.9 on 3.1.0 doesn't exist
		spellid = 47896;
	else if( m_spellInfo->AdditionalAura )
		spellid = m_spellInfo->AdditionalAura;

	if(spellid && caster)
	{
		SpellEntry *spellInfo = dbcSpell.LookupEntry( spellid );
		if(!spellInfo) return;
		Spell *spell = new Spell(caster, spellInfo ,true, NULL);
		SpellCastTargets targets(u);
		spell->prepare(&targets);	
	}

	if(a->GetSpellProto()->procCharges > 0)
	{
		//do we have an aura?
		Aura* oldaura=u->FindAura(m_spellInfo->Id);

		if (oldaura != NULL)
		{
			oldaura->HandleStackAdd();
			delete a;
			return;
		}

		//if(!(a->GetSpellProto()->procFlags & PROC_REMOVEONUSE))
		//{
			SpellCharge charge;
			charge.count=a->GetSpellProto()->procCharges;
			charge.spellId=a->GetSpellId();
			charge.ProcFlag=a->GetSpellProto()->procFlags;
			charge.lastproc = 0;
			u->m_chargeSpells.insert(make_pair(a->GetSpellId(), charge));
		//}
	}

	if (forced_modifier != NULL)
	{
		for (uint32 i=0; i<3; ++i)
			a->m_modList[i] = *forced_modifier;
	}

	u->AddAura(a, m_spellScript); // the real spell is added last so the modifier is removed last

	if (forced_applications != 0)
	{
		//addstack until were done
		for (uint32 i=a->m_stackcount; i<forced_applications; ++i)
			a->HandleStackAdd();
	}
	Log.Warning("[Spell::HandleAddAura]","Ok... ");
}


/*
void Spell::TriggerSpell()
{
	if(TriggerSpellId != 0)
	{
		// check for spell id
		SpellEntry *spellInfo = sSpellStore.LookupEntry(TriggerSpellId );

		if(!spellInfo)
		{
			sLog.outError("WORLD: unknown spell id %i\n", TriggerSpellId);
			return;
		}

		Spell *spell = new Spell(m_caster, spellInfo,false, NULL);
		WPAssert(spell);

		SpellCastTargets targets;
		if(TriggerSpellTarget)
			targets.m_unitTarget = TriggerSpellTarget;
		else
			targets.m_unitTarget = m_targets.m_unitTarget;

		spell->prepare(&targets);
	}
}*/
/*
void Spell::DetermineSkillUp()
{
	skilllinespell* skill = objmgr.GetSpellSkill(m_spellInfo->Id);
	if (m_caster->GetTypeId() == TYPEID_PLAYER)
	{
		if ((skill) && static_cast< Player* >( m_caster )->HasSkillLine( skill->skilline ) )
		{
			uint32 amt = static_cast< Player* >( m_caster )->GetBaseSkillAmt(skill->skilline);
			uint32 max = static_cast< Player* >( m_caster )->GetSkillMax(skill->skilline);
			if (amt >= skill->grey) //grey
			{
			}
			else if ((amt >= (((skill->grey - skill->green) / 2) + skill->green)) && (amt < max))
			{
				if (rand()%100 < 33) //green
					static_cast< Player* >( m_caster )->AdvanceSkillLine(skill->skilline);	

			}
			else if ((amt >= skill->green) && (amt < max))
			{
				if (rand()%100 < 66) //yellow
					static_cast< Player* >( m_caster )->AdvanceSkillLine(skill->skilline);		

			}
			else 
			{
				if (amt < max) //brown
					static_cast< Player* >( m_caster )->AdvanceSkillLine(skill->skilline);
			}
		}
	}
}
*/

bool Spell::IsAspect()
{
	switch(m_spellInfo->Id)
	{
	 case __Aspect_Mastery_Rank_1:           // 53265
	 case __Aspect_of_Arlokk:                //	24690
	 case __Aspect_of_Jeklik:                //	24687
	 case __Aspect_of_Mar_li:                //	24686
	 case __Aspect_of_Neptulon:              //	21332
	 case __Aspect_of_the_Beast:             //	13161
	 case __Aspect_of_the_Beast____0:         //	61669
     case __Aspect_of_the_Cheetah:           // 5118 
	 case __Aspect_of_the_Dragonhawk:        //	61848
	 case __Aspect_of_the_Dragonhawk_Rank_1: //	61846
	 case __Aspect_of_the_Dragonhawk_Rank_2: //	61847
	 case __Aspect_of_the_Flame:       // 41593
	 case __Aspect_of_the_Hawk_Rank_1: // 13165
	 case __Aspect_of_the_Hawk_Rank_2: // 14318
	 case __Aspect_of_the_Hawk_Rank_3: // 14319
	 case __Aspect_of_the_Hawk_Rank_4: // 14320
	 case __Aspect_of_the_Hawk_Rank_5: // 14321
	 case __Aspect_of_the_Hawk_Rank_6: // 14322
	 case __Aspect_of_the_Hawk_Rank_7: // 25296
	 case __Aspect_of_the_Hawk_Rank_8: // 27044
	 case __Aspect_of_the_Ice:         // 41594
	 case __Aspect_of_the_Monkey:      // 13163
	 case __Aspect_of_the_Pack:        // 13159
	 case __Aspect_of_the_Shadow:      // 41595
	 case __Aspect_of_the_Spirit_Hunter: // 36613
	 case __Aspect_of_the_Viper:         // 34074
	 case __Aspect_of_the_Viper____0:     //	34075
	 case __Aspect_of_the_Wild_Rank_1:   //	20043
	 case __Aspect_of_the_Wild_Rank_2:   //	20190
	 case __Aspect_of_the_Wild_Rank_3:   //	27045
	 case __Aspect_of_the_Wild_Rank_4:   //	49071
	 case __Aspect_of_Thekal:            //	24689
	 case __Aspect_of_Venoxis:           //	24688
                return(true);
	 default: return(false); 

	}
	return (m_spellInfo->buffType == SPELL_TYPE_ASPECT);
}

bool Spell::IsSeal()
{
	switch(m_spellInfo->Id)
	{
	 case __Seal_Cost_Reduced:	       // 27848
	 case __Seal_Cost_Reduced____0:	   // 60804
	 case __Seal_Fate:	               // 14189
	 case __Seal_Fate_Rank_1:	       // 14186
	 case __Seal_Fate_Rank_2:	       // 14190
	 case __Seal_Fate_Rank_3:	       // 14193
	 case __Seal_Fate_Rank_4:	       // 14194
	 case __Seal_Fate_Rank_5:	       // 14195
	 case __Seal_Felvine_Shard:	       // 22949
	 case __Seal_of_Blood:	           // 31892
	 case __Seal_of_Blood____0:	       // 31893
	 case __Seal_of_Blood____1:	       // 32221
	 case __Seal_of_Blood____2:	       // 38008
	 case __Seal_of_Blood____3:	       // 41459
	 case __Seal_of_Command	:	       // 20375
	 case __Seal_of_Command____0:	       // 20424
	 case __Seal_of_Command____1:	       // 29385
	 case __Seal_of_Command____2:	       // 33127
	 case __Seal_of_Command____3:	       // 41469
	 case __Seal_of_Command____4:	       // 42058
	 case __Seal_of_Command____5:	       // 57769
	 case __Seal_of_Command____6:	       // 57770
	 case __Seal_of_Corruption:	       // 53736
	 case __Seal_of_Corruption____0:	       // 53739
	 case __Seal_of_Justice:	       // 20164
	 case __Seal_of_Light:	       // 20165
	 case __Seal_of_Light____0:	       // 20167
	 case __Seal_of_Onslaught:	       // 50907
	 case __Seal_of_Onslaught____0:	       // 50908
	 case __Seal_of_Protection:	       // 17177
	 case __Seal_of_Reckoning:	       // 15277
	 case __Seal_of_Reckoning____0:	       // 15346
	 case __Seal_of_Reckoning_Effect:	       // 15278
	 case __Seal_of_Reckoning_Effect____0:	       // 15347
	 case __Seal_of_Righteousness:	       // 20154
	 case __Seal_of_Righteousness____0:	       // 21084
	 case __Seal_of_Righteousness____1:	       // 25742
	 case __Seal_of_Sacrifice:	       // 13903
	 case __Seal_of_the_Crusader_Judgement_Increase:	       // 23300
	 case __Seal_of_the_Dawn:	       // 23930
	 case __Seal_of_the_Martyr:	       // 53718
	 case __Seal_of_the_Martyr____0:	       // 53719
	 case __Seal_of_the_Martyr____1:	       // 53720
	 case __Seal_of_the_Pantheon:	       // 60214
	 case __Seal_of_Vengeance:	       // 31801
	 case __Seal_of_Vengeance____0:	       // 42463
	 case __Seal_of_Wisdom:	       // 20166
	 case __Seal_of_Wisdom____0:	       // 20168
	 case __Seal_of_Wrath:	       // 44480
	 case __Seal_of_Wrath____0:	       // 44481
	 case __Seal_of_Wrath____1:	       // 45095
	 case __Seal_of_Wrath____2:	       // 45096
	 case __Seal_of_Wrath____3:	       // 46030
	 case __Seal_of_Wrath____4:	       // 46031
	 case __Seal_Sphere:	       // 36849
	 case __Seals_of_the_Pure_Rank_1:	       // 20224
	 case __Seals_of_the_Pure_Rank_2:	       // 20225
	 case __Seals_of_the_Pure_Rank_3:	       // 20330
	 case __Seals_of_the_Pure_Rank_4:	       // 20331
	 case __Seals_of_the_Pure_Rank_5:	       // 20332
               return(true);
	 default: return(false); 

	}
	return (m_spellInfo->buffType == SPELL_TYPE_SEAL);
}

bool Spell::IsBinary(SpellEntry * sp)
{
	// Normally, damage spells are only binary if they have an additional non-damage effect
	// DoTs used to be binary spells, but this was changed. (WoWwiki)
	return !(sp->Effect[0] == SPELL_EFFECT_SCHOOL_DAMAGE ||
		sp->EffectApplyAuraName[0] == SPELL_AURA_PERIODIC_DAMAGE);
}

uint8 Spell::CanCastSpell(void)
{
  switch(m_spellInfo->Effect[0])
  {
   case SPELL_EFFECT_FEED_PET: // Nourrir le familier ---------------------------------------------
   {
    if(!m_targets.m_target->IsItem() || !m_caster->IsPlayer())
	{
     // Impossible que cela arrive sinon prob de codage ou hack reseau (Branruz)
	 Log.Error("CanCastSpell","Fatal Item/Player <--- Report this to devs.");
     return(SPELL_FAILED_TRY_AGAIN);
	}

    Pet *pPet = ((Player *)m_caster)->GetSummon();

	if(!pPet) return(SPELL_FAILED_NO_PET); // Pas de familier invoqué 

	ItemPrototype* proto = ((Item *)m_targets.m_target)->GetProto();  
	if(!proto->FoodType) return(SPELL_FAILED_WRONG_PET_FOOD); // Tout sauf de la nourriture
	
	// Mauvais type de nourriture
	if(!(pPet->GetPetDiet() & (1 << (proto->FoodType - 1)))) return(SPELL_FAILED_WRONG_PET_FOOD);

	// check food level: food should be max 30 lvls below pets level
	//if(pPet->getLevel() > proto->ItemLevel + 30)return(SPELL_FAILED_FOOD_LOWLEVEL); // Waad JdR, géré au niveau du SpellEffectFeedPet 
   } break; // End SPELL_EFFECT_FEED_PET ---------------------------------------------
	
   default: return(SPELL_CANCAST_OK); // Pour test (Branruz) => SPELL_FAILED_TRY_AGAIN
	        
  }


  

  return(SPELL_CANCAST_OK);
}
//--------------------------------------------------------------------------------------------
uint8 Spell::CanCast(bool tolerate)
{
  uint32 i;
  bool skip = false;

  // Table Disable_spell
  if(objmgr.IsSpellDisabled(m_spellInfo->Id))
  {
		Log.Warning("CanCast","Spell %u disable",m_spellInfo->Id);
		return(SPELL_FAILED_SPELL_UNAVAILABLE);
  }

  if(!m_targets.m_target->IsInWorld()) return(SPELL_FAILED_CANT_CAST_ON_TAPPED); // "Cible indisponible"

#ifdef COLLISION
  if (m_spellInfo->mechanics == MECHANIC_MONTURE)
  {
   // Qiraj battletanks work everywhere on map 531
   if ( m_caster->GetMapId() == 531 && ( m_spellInfo->Id == 25953 || m_spellInfo->Id == 26054 || m_spellInfo->Id == 26055 || m_spellInfo->Id == 26056 ) )
   {
    //Log.Warning("CanCast","SPELL_CANCAST_OK");
	return(SPELL_CANCAST_OK);
   }

   if (CollideInterface.IsIndoor( m_caster->GetMapId(), m_caster->GetPositionNC() ))
   {
    //Log.Warning("CanCast","SPELL_FAILED_NO_MOUNTS_ALLOWED");
	return(SPELL_FAILED_NO_MOUNTS_ALLOWED); // "Vous ne pouvez pas utiliser une monture ici"
   }
  }
  else if( m_spellInfo->attributes & ATTRIBUTES_ONLY_OUTDOORS )
  {
   // On test sur la cible
   if( !CollideInterface.IsOutdoor( m_targets.m_target->GetMapId(), m_targets.m_target->GetPositionNC() ) )
   {
    //Log.Warning("CanCast","SPELL_FAILED_ONLY_OUTDOORS");
	return(SPELL_FAILED_ONLY_OUTDOORS); // "A utiliser en exterieur"
   }
  }

  // Check LOS
  float SourceTaille = m_caster->GetSize();
  float TargetTaille = m_targets.m_target->GetSize();
  if(!m_caster->IsGO() && !m_targets.m_target->IsGO())
  {
   if (!CollideInterface.CheckLOS( m_caster->GetMapId(),m_caster->GetPosition().x,m_caster->GetPosition().y,m_caster->GetPosition().z+SourceTaille,
	                              m_targets.m_target->GetPosition().x,m_targets.m_target->GetPosition().y,m_targets.m_target->GetPosition().z+TargetTaille ))
   {
    Log.Warning("CanCast","La cible n'est plus en vue");
	return(SPELL_FAILED_LINE_OF_SIGHT); // "Cible hors du champ de vision"
   }
  }
#endif
// Laissé en dur, pas trouver de meilleur solution grrrrrrrrrrrrr f*ck blizz (Brz)
  switch(m_spellInfo->Id)
  {
    case 603: //curse of doom, can't be casted on players
	case 30910:
	case 47867:
	{
	 if(m_targets.m_target->IsPlayer()) return(SPELL_FAILED_TARGET_IS_PLAYER); // "Impossible de cibler les joueurs"
	}break;

    case 13907:
	{
	 if (!m_targets.m_target || m_targets.m_target->IsPlayer() || ((Player *)m_targets.m_target)->getClass()!=TARGET_TYPE_DEMON )
	 {
	  return(SPELL_FAILED_SPELL_UNAVAILABLE); // "Le sort ne vous est pas disponible"
	 }
	}break;

	// disable spell
	case 25997: // Eye for an Eye
	case 38554: //Absorb Eye of Grillok
	{
	 // do not allow spell to be cast
	 return(SPELL_FAILED_SPELL_UNAVAILABLE); // "Le sort ne vous est pas disponible"
	}break;

	//These spells are NPC only.
	case 25166: //Call Glyphs of Warding
	case 38892: //Shadow Bolt
	case 40536: //Chain Lightning
	case 41078: //Shadow Blast
	{
	 if(m_caster->IsPlayer()) return(SPELL_FAILED_BAD_TARGETS); // "Cible incorrect" "Cet objet n'est pas une cible autorisée"
	}break;

	case 27907: // Disciplinary Rod
	{
	 if(!m_targets.m_target->IsUnit() || 
		( (m_targets.m_target->GetEntry() != 15945) && (m_targets.m_target->GetEntry() != 15941)) ) // 'Apprentice Meledor' and 'Apprentice Ralen'
	 {
	  return(SPELL_FAILED_BAD_TARGETS); // "Cible incorrect" "Cet objet n'est pas une cible autorisée"
	 }
	}break;

	case 982: //Revive Pet
	{
	 Pet *pPet = ((Player *)m_caster)->GetSummon();
	 if(pPet && !pPet->isDead()) return(SPELL_FAILED_TARGET_NOT_DEAD); // // "La cible est en vie"
	}break;
  }



 return SPELL_CANCAST_OK; // Pour Debug (Branruz)
//--------------------------------------------------

	Unit *target = m_caster->GetMapMgr()->GetUnit(m_targets.m_target->GetGUID());

    

	if(m_caster->IsPlayer()) // Player
	{
	 skip = ( (((Player *)m_caster)->m_skipCastCheck[0] & m_spellInfo->SpellGroupType[0]) || 
		      (((Player *)m_caster)->m_skipCastCheck[1] & m_spellInfo->SpellGroupType[1]) || 
		      (((Player *)m_caster)->m_skipCastCheck[2] & m_spellInfo->SpellGroupType[2])) ; // related to aura 262
	}

	

	if( m_caster->IsUnit() ) // Unit (peut etre un player)
	{
		if( (((Unit *)m_caster)->GetCurrentSpell() != NULL) && 
			(((Unit *)m_caster)->GetCurrentSpell() != this) && 
			!m_triggeredSpell )
		{
		 Log.Warning("CanCast","u_caster a le spell %u en cours (%s)",((Unit *)m_caster)->GetCurrentSpell()->GetSpellProto()->Id,
			                                                       ((Unit *)m_caster)->GetCurrentSpell()->GetSpellProto()->Name);
		 return SPELL_FAILED_SPELL_IN_PROGRESS;
		}
	}

	/*
	// Spells for the zombie event 
	if( p_caster && (p_caster->GetShapeShift() == FORM_ZOMBIE) && !( ((uint32)1 << (p_caster->GetShapeShift()-1)) & GetSpellProto()->shapeshiftMask  ))
	{
		Log.Error("CanCast","Invalid shapeshift: %u", GetSpellProto()->shapeshiftMask);
		
		return SPELL_FAILED_SPELL_UNAVAILABLE;
	}*/

	if (m_spellScript != NULL)
	{
		SpellCastError scriptresult=m_spellScript->CanCast(tolerate);

		if (scriptresult != SPELL_CANCAST_OK) // || (m_spellScript->flags & SSCRIPT_FLAG_COMPLETE_CHECK) )
		{
			//Log.Warning("CanCast","scriptresult != SPELL_CANCAST_OK");
			return scriptresult;
		}
	}

/*		Unit* target = NULL;
	if (m_targets.m_target != NULL && m_targets.m_target->IsUnit())
		target = static_cast<Unit*>(m_targets.m_target);
	else
		target = u_caster;

	bool hasrequiredaura = false;
	if (m_spellInfo->casterAuraSpell == 0)
		hasrequiredaura = true;

	if (u_caster != NULL)
	{
		for(uint32 x=0;x<MAX_AURAS+MAX_PASSIVE_AURAS;x++)
		{
			if(u_caster->m_auras[x] != NULL)
			{
				if (m_spellInfo->casterAuraSpell == u_caster->m_auras[x]->m_spellProto->Id)
					hasrequiredaura = true;
				if (m_spellInfo->excludeCasterAuraSpell == u_caster->m_auras[x]->m_spellProto->Id)
					return SPELL_FAILED_BAD_TARGETS;
			}
		}
	}

	//and check target excluded auras
	if (target->FindAura(m_spellInfo->excludeTargetAuraSpell) != NULL)
		return SPELL_FAILED_BAD_TARGETS;

	if (!hasrequiredaura)
		return SPELL_FAILED_BAD_TARGETS;
*/
	if( m_caster->IsPlayer() )
	{
		//blizzard while moving, looks really sexy
		if ( ((Player *)m_caster)->m_isMoving && (m_spellInfo->channelInterruptFlags != 0) )
		{
			//Log.Warning("CanCast","SPELL_FAILED_MOVING");
			return SPELL_FAILED_MOVING;
		}

		if( m_spellInfo->Id == __Dominion_Over_Acherus ) // 51721 sur 335.12340
		{
			if( !(((Player *)m_caster)->GetAreaID() == 4281) )
				return SPELL_FAILED_NOT_HERE;
		}
		// flying auras
		if(m_caster->IsPlayer())
		{
		 if( m_spellInfo->c_is_flags & SPELL_FLAG_IS_FLYING )
		 {
			// can't fly in non-flying zones
			if( ((Player *)m_caster)->GetAreaDBC() != NULL && !(((Player *)m_caster)->GetAreaDBC()->AreaFlags & AREA_FLYING_PERMITTED) )
			{
				//Log.Warning("CanCast","SPELL_FAILED_NOT_HERE");
				return SPELL_FAILED_NOT_HERE;
			}
			else if( ((Player *)m_caster)->GetMapId() == 571 && !((Player *)m_caster)->HasDummyAura(SPELL_HASH_COLD_WEATHER_FLYING) )
			{
				//Log.Warning("CanCast","SPELL_FAILED_TARGET_AURASTATE");
				return SPELL_FAILED_TARGET_AURASTATE ;
			}
		 }
		}
        /*
		if( m_spellInfo->Id == 33076 )			// Prayer of mending
		{
			if( p_caster->GetGroup() != NULL && p_caster->GetGroup()->m_prayerOfMendingCount )
			{
				Player * tmp_plr_pom = p_caster->IsInWorld() ? p_caster->GetMapMgr()->GetPlayer(p_caster->GetGroup()->m_prayerOfMendingTarget) : NULL;
				if( tmp_plr_pom != NULL )		// remove from current target.
					tmp_plr_pom->RemoveAura(41635);

				p_caster->GetGroup()->m_prayerOfMendingTarget = 0;
				p_caster->GetGroup()->m_prayerOfMendingCount = 0;
			}
		}
		*/

		if(((Player *)m_caster)->m_castFilterEnabled) 
		{
			if(!m_spellInfo->SpellGroupType[0] && !m_spellInfo->SpellGroupType[1] && !m_spellInfo->SpellGroupType[2])
			{
				((Player *)m_caster)->m_castFilterEnabled = false;
			}
			else if(!( (m_spellInfo->SpellGroupType[0] & ((Player *)m_caster)->m_castFilter[0]) || 
			           (m_spellInfo->SpellGroupType[1] & ((Player *)m_caster)->m_castFilter[1]) ||
			           (m_spellInfo->SpellGroupType[2] & ((Player *)m_caster)->m_castFilter[2]) ) )
		    {
			 Log.Warning("CanCast","SPELL_FAILED_SPELL_IN_PROGRESS 2 (%u - %s)",m_spellInfo->Id,m_spellInfo->Name);
			 Log.zPvP("       ","m_castFilterEnabled: %s",(((Player *)m_caster)->m_castFilterEnabled) ? "Oui":"Non");
			 Log.zPvP("       ","(SpellGroupType[0] & m_castFilter[0]) 0x%08X & 0x%08X",
				                                          m_spellInfo->SpellGroupType[0],((Player *)m_caster)->m_castFilter[0]);
			 Log.zPvP("       ","(SpellGroupType[1] & m_castFilter[1]) 0x%08X & 0x%08X",
				                                          m_spellInfo->SpellGroupType[1],((Player *)m_caster)->m_castFilter[1]);
			 Log.zPvP("       ","(SpellGroupType[2] & m_castFilter[2]) 0x%08X & 0x%08X",
				                                          m_spellInfo->SpellGroupType[2],((Player *)m_caster)->m_castFilter[2]);
			 Log.Warning("CanCast","------------------------------------------");

			 //return SPELL_FAILED_SPELL_IN_PROGRESS;	// Need to figure the correct message
			 return SPELL_FAILED_TOO_MANY_SOCKETS; // "Vous ne pouvez pas faire ca maintenant"
		    }
		}

		uint32 self_rez = ((Player *)m_caster)->GetUInt32Value(PLAYER_SELF_RES_SPELL);
		if( !((Player *)m_caster)->isAlive() && self_rez != m_spellInfo->Id)
		{
			// spirit of redemption
			if( ((Player *)m_caster)->m_canCastSpellsWhileDead )
			{
				// casting a spell on self
				if( m_targets.m_targetMask & TARGET_FLAG_SELF || m_targets.m_target->GetGUID() == ((Player *)m_caster)->GetGUID() ||
					!IsHealingSpell(m_spellInfo) )		// not a holy spell
				{
					//Log.Warning("CanCast","SPELL_FAILED_SPELL_UNAVAILABLE2");
					return SPELL_FAILED_SPELL_UNAVAILABLE;
				}
			}
			else		// not SOR
			{
				//Log.Warning("CanCast","SPELL_FAILED_CASTER_DEAD");
				return SPELL_FAILED_CASTER_DEAD;
			}
		}

		if( m_spellInfo->NameHash == SPELL_HASH_HUNTER_S_MARK )
		{
			if( GetUnitTarget() && !isHostile( GetUnitTarget(), m_caster ))
			{
				//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS");
				return SPELL_FAILED_BAD_TARGETS;
			}
		}



		//are we in an arena and the spell cooldown is longer then 15mins?
		/*if ( p_caster->m_bg && ( p_caster->m_bg->GetType() >= BATTLEGROUND_ARENA_2V2 && p_caster->m_bg->GetType() <= BATTLEGROUND_ARENA_5V5 ) && ( m_spellInfo->recoveryTime >= 900000 || m_spellInfo->categoryRecoveryTime >= 900000 ) )
			return SPELL_FAILED_SPELL_UNAVAILABLE;*/

		if ( ((Player *)m_caster)->m_bg && 
		   ( ((Player *)m_caster)->m_bg->GetType() >= BATTLEGROUND_ARENA_2V2 && 
		     ((Player *)m_caster)->m_bg->GetType() <= BATTLEGROUND_ARENA_5V5 ) )
		{
			if( ( m_spellInfo->recoveryTime >= 900000 || m_spellInfo->categoryRecoveryTime >= 900000 ) )
			{
				//Log.Warning("CanCast","SPELL_FAILED_NOT_IN_ARENA");
				return SPELL_FAILED_NOT_IN_ARENA;
			}

			if( !((Player *)m_caster)->m_bg->HasStarted() )
			{
				// cannot cast spells in an arena if it hasn't started (blinking through gates, lalala)
				if( m_spellInfo->NameHash == SPELL_HASH_BLINK )
				{
					//Log.Warning("CanCast","SPELL_FAILED_SPELL_UNAVAILABLE3");
					return SPELL_FAILED_SPELL_UNAVAILABLE;
				}
			}
		}

		// What a waste.
		if( ((Player *)m_caster)->m_bg && GetGameObjectTarget())
		{
			if(((Player *)m_caster)->SchoolImmunityList[0]) // Physical is all that really matters.
			{
				//Log.Warning("CanCast","SPELL_FAILED_SPELL_UNAVAILABLE4");
				return SPELL_FAILED_SPELL_UNAVAILABLE;
			}
		}

		// Disarm
		if( m_caster->IsUnit() )
		{
			if( ((Unit *)m_caster)->disarmed )
			{
				if( m_spellInfo->is_melee_spell )
				{
					//Log.Warning("CanCast","SPELL_FAILED_EQUIPPED_ITEM_CLASS");
					return SPELL_FAILED_EQUIPPED_ITEM_CLASS ;
				}
				else if( m_spellInfo->is_ranged_spell )
				{
					//Log.Warning("CanCast","SPELL_FAILED_EQUIPPED_ITEM_CLASS2");
					return SPELL_FAILED_EQUIPPED_ITEM_CLASS ;
				}
			}
			if( ((Unit *)m_caster)->disarmedShield && m_spellInfo->RequiredItemFlags && (m_spellInfo->RequiredItemFlags & (1 << INVTYPE_SHIELD)) )
			{
				//Log.Warning("CanCast","SPELL_FAILED_EQUIPPED_ITEM_CLASS3");
				return SPELL_FAILED_EQUIPPED_ITEM_CLASS ;
			}
		}

		// check for cooldowns
		if(!tolerate && !((Player *)m_caster)->Cooldown_CanCast(m_spellInfo))
			return SPELL_FAILED_NOT_READY;

		if(((Player *)m_caster)->GetDuelState() == DUEL_STATE_REQUESTED)
		{
			for(i = 0; i < 3; ++i)
			{
				if( m_spellInfo->Effect[i] && m_spellInfo->Effect[i] != SPELL_EFFECT_APPLY_AURA && m_spellInfo->Effect[i] != SPELL_EFFECT_APPLY_PET_AURA
					&& m_spellInfo->Effect[i] != SPELL_EFFECT_APPLY_AREA_AURA)
				{
					//Log.Warning("CanCast","SPELL_FAILED_TARGET_DUELING");
					return SPELL_FAILED_TARGET_DUELING;
				}
			}
		}

		if( m_caster->IsPlayer() && playerTarget && ((Player *)m_caster) != playerTarget && 
			playerTarget->GetDuelState() != DUEL_STATE_FINISHED && 
			m_spellInfo->c_is_flags & SPELL_FLAG_CASTED_ON_FRIENDS )
		{
			//Log.Warning("CanCast","SPELL_FAILED_TARGET_DUELING2");
			return SPELL_FAILED_TARGET_DUELING;
		}

		for (i=0; i<3; ++i)
		{
			if(m_spellInfo->Effect[i] == SPELL_EFFECT_APPLY_GLYPH)
			{
				uint8 glyphslot = m_targets.m_targetMaskExtended >> 1 & 0x0F;
				if (!((Player *)m_caster)->CanUseGlyphSlot(glyphslot))
				{
					//Log.Warning("CanCast","SPELL_FAILED_GLYPH_SOCKET_LOCKED");
					return SPELL_FAILED_GLYPH_SOCKET_LOCKED;
				}
				break; //done check now :P
			}
		}

		// check for duel areas
		if(m_caster->IsPlayer() && m_spellInfo->Id == __Duel) //7266
		{
			AreaTable* at = dbcArea.LookupEntry( ((Player *)m_caster)->GetAreaID() );
			if(at->AreaFlags & AREA_CITY_AREA)
			{
				//Log.Warning("CanCast","SPELL_FAILED_NO_DUELING3");
				return SPELL_FAILED_NO_DUELING;
			}

			if( ((Player *)m_caster)->m_bg )
			{
				//Log.Warning("CanCast","SPELL_FAILED_NO_DUELING4");
				return SPELL_FAILED_NO_DUELING;
			}
		}

		// check if spell is allowed while player is on a taxi
		if(((Player *)m_caster)->m_onTaxi)
		{
			// This uses the same flag as ordinary mounts
			if(!(m_spellInfo->attributes & ATTRIBUTES_MOUNT_CASTABLE))
			{
				//Log.Warning("CanCast","SPELL_FAILED_NOT_ON_TAXI");
				return SPELL_FAILED_NOT_ON_TAXI;
			}

			// 33836 = _Dropping_Heavy_Bomb
			if( m_spellInfo->Id != __Dropping_Heavy_Bomb) // 33836 - exception for Area 52 Special
			{
				//Log.Warning("CanCast","SPELL_FAILED_NOT_ON_TAXI2");
				return SPELL_FAILED_NOT_ON_TAXI;
			}
		}

		// check if spell is allowed while player is on a transporter
		if(((Player *)m_caster)->m_CurrentTransporter)
		{
			// no mounts while on transporters
			if( m_spellInfo->EffectApplyAuraName[0] == SPELL_AURA_MOUNTED || m_spellInfo->EffectApplyAuraName[1] == SPELL_AURA_MOUNTED || m_spellInfo->EffectApplyAuraName[2] == SPELL_AURA_MOUNTED)
			{
				//Log.Warning("CanCast","SPELL_FAILED_NOT_ON_TRANSPORT");
				return SPELL_FAILED_NOT_ON_TRANSPORT;
			}
		}

		// check if spell is allowed while not mounted
		if(!((Player *)m_caster)->IsMounted())
		{
			if( m_spellInfo->Id == 25860) // Reindeer Transformation
			{
				//Log.Warning("CanCast","SPELL_FAILED_ONLY_MOUNTED");
				return SPELL_FAILED_ONLY_MOUNTED;
			}
		}
		else
		{
			if (!(m_spellInfo->attributes & ATTRIBUTES_MOUNT_CASTABLE))
			{
				//Log.Warning("CanCast","SPELL_FAILED_ONLY_MOUNTED2");
				return SPELL_FAILED_NOT_MOUNTED;
			}
		}

		// no mana drains on shifted druids :(
		if( GetPlayerTarget() && GetPlayerTarget()->getClass() == DRUID && 
			(m_spellInfo->Effect[0] == SPELL_EFFECT_POWER_DRAIN || m_spellInfo->Effect[1] == SPELL_EFFECT_POWER_DRAIN || 
			 m_spellInfo->Effect[2] == SPELL_EFFECT_POWER_DRAIN))
		{
			if( GetPlayerTarget()->GetShapeShift() == FORM_BEAR ||
				GetPlayerTarget()->GetShapeShift() == FORM_DIREBEAR ||
				GetPlayerTarget()->GetShapeShift() == FORM_CAT)
			{
				//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS");
				return SPELL_FAILED_BAD_TARGETS;
			}
		}

		// check if spell is allowed while shapeshifted
		if(((Player *)m_caster)->GetShapeShift())
		{
			switch(((Player *)m_caster)->GetShapeShift())
			{
				case FORM_TREE:
				case FORM_BATTLESTANCE:
				case FORM_DEFENSIVESTANCE:
				case FORM_BERSERKERSTANCE:
				case FORM_SHADOW:
				case FORM_STEALTH:
				case FORM_MOONKIN:
				{
					break;
				}

				case FORM_SWIFT:
				case FORM_FLIGHT:
				{
					// check if item is allowed (only special items allowed in flight forms)
				//	if(i_caster && !(i_caster->GetProto()->Flags & ITEM_FLAG_SHAPESHIFT_OK))
				//		return SPELL_FAILED_NO_ITEMS_WHILE_SHAPESHIFTED;

					break;
				}

				//case FORM_CAT: 
				//case FORM_TRAVEL:
				//case FORM_AQUA:
				//case FORM_BEAR:
				//case FORM_AMBIENT:
				//case FORM_GHOUL:
				//case FORM_DIREBEAR:
				//case FORM_CREATUREBEAR:
				//case FORM_GHOSTWOLF:
				//case FORM_SPIRITOFREDEMPTION:

				default:
				{
					// check if item is allowed (only special & equipped items allowed in other forms)
					/*if(i_caster && !(i_caster->GetProto()->Flags & ITEM_FLAG_SHAPESHIFT_OK))
					{
						if(i_caster->GetProto()->InventoryType == INVTYPE_NON_EQUIP)
						{
							//Log.Warning("CanCast","SPELL_FAILED_NO_ITEMS_WHILE_SHAPESHIFTED");
							return SPELL_FAILED_NO_ITEMS_WHILE_SHAPESHIFTED;
						}
					}*/
				}
			}
		}

		// item spell checks
		if(m_caster->IsItem())
		{
			if( ((Item *)m_caster)->GetProto()->ZoneNameID && ((Item *)m_caster)->GetProto()->ZoneNameID != ((Item *)m_caster)->GetZoneId() ) 
			{
				//Log.Warning("CanCast","SPELL_FAILED_NOT_HERE");
				return SPELL_FAILED_NOT_HERE;
			}
			if( ((Item *)m_caster)->GetProto()->MapID && ((Item *)m_caster)->GetProto()->MapID != ((Item *)m_caster)->GetMapId() )
			{
				//Log.Warning("CanCast","SPELL_FAILED_NOT_HERE2");
				return SPELL_FAILED_NOT_HERE;
			}

            

			if(((Item *)m_caster)->GetProto()->Spells[0].Charges != 0)
			{
				
				// check if the item has the required charges
				if(((Item *)m_caster)->GetUInt32Value(ITEM_FIELD_SPELL_CHARGES) == 0)
				{
					//Mounts have changed, they should be added to known spells
					if(((Item *)m_caster)->GetProto()->Class != ITEM_CLASS_MISCELLANEOUS && 
					   ((Item *)m_caster)->GetProto()->SubClass != ITEM_SUBCLASS_MISCELLANEOUS_MOUNT )
					{
						//Log.Warning("CanCast","SPELL_FAILED_NO_CHARGES_REMAIN");
						return SPELL_FAILED_NO_CHARGES_REMAIN;
					}
					else
					{
						//Unit *target = (m_caster->IsInWorld()) ? m_caster->GetMapMgr()->GetUnit(m_targets.m_target->GetGUID()) : NULL;
						if(target && target->IsPlayer())
						{
							//Allow spell to be casted if player didn't have this mount yet in pet tab (iow has the spell).
							if(((Item *)m_caster)->GetProto()->Spells[1].Id && 
								static_cast<Player *>(target)->HasSpell(((Item *)m_caster)->GetProto()->Spells[1].Id))
							{
								//Log.Warning("CanCast","SPELL_FAILED_NO_CHARGES_REMAIN2");
								return SPELL_FAILED_NO_CHARGES_REMAIN;
							}
						}
					}

				}
				// for items that combine to create a new item, check if we have the required quantity of the item
				if(((Item *)m_caster)->GetProto()->ItemId == m_spellInfo->reagent[0])
				{
					//Unit *target = m_caster->GetMapMgr()->GetUnit(m_targets.m_target->GetGUID());
					if(target && target->IsPlayer())
					{
					 if(static_cast<Player *>(target)->GetItemInterface()->GetItemCount(m_spellInfo->reagent[0]) < 1 + m_spellInfo->reagentCount[0])
					 {
						//Log.Warning("CanCast","SPELL_FAILED_ITEM_GONE");
						return SPELL_FAILED_ITEM_GONE; // "L'objet n'est plus disponible"
					 }
					}
				}
			}

			// heal checks are only applied to item casters
			if(target && target->IsPlayer())
			{
			 if( m_spellInfo->c_is_flags & SPELL_FLAG_IS_HEALING_SPELL && 
				static_cast<Player *>(target)->GetUInt32Value(UNIT_FIELD_HEALTH) == static_cast<Player *>(target)->GetUInt32Value(UNIT_FIELD_MAXHEALTH) )
			 {
				//Log.Warning("CanCast","SPELL_FAILED_ALREADY_AT_FULL_HEALTH");
				return SPELL_FAILED_ALREADY_AT_FULL_HEALTH;
			 }

			 if( (static_cast<Player *>(target)->GetPowerType() == POWER_TYPE_MANA) && 
				 (m_spellInfo->c_is_flags & SPELL_FLAG_IS_HEALING_MANA_SPELL) && 
				 (static_cast<Player *>(target)->GetUInt32Value(UNIT_FIELD_POWER1) == static_cast<Player *>(target)->GetUInt32Value(UNIT_FIELD_MAXPOWER1)) )
			 {
				//Log.Warning("CanCast","SPELL_FAILED_ALREADY_AT_FULL_MANA");
				return SPELL_FAILED_ALREADY_AT_FULL_MANA;
			 }
			}
		}

		bool CheckReagents = true;
		if( m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2] )
		{
			//lets check if we need regeants
			uint32 AffectedSpellGroupType[3] = {0,0,0};
			for(uint32 x=0;x<3;x++)
			{
				AffectedSpellGroupType[x] |= ((Player *)m_caster)->GetUInt32Value(PLAYER_NO_REAGENT_COST_1+x);
			}
			if( AffectedSpellGroupType )
			{
				for(uint32 x=0;x<3;x++)
				{
					if( AffectedSpellGroupType[x] & m_spellInfo->SpellGroupType[x] )
						CheckReagents = false;
				}
			}
		}

		// check if we have the required reagents
		if( CheckReagents && (!m_caster->IsItem() || (((Item *)m_caster)->GetProto() && 
			((Item *)m_caster)->GetProto()->Flags != 268435520)) ) // ITEM_FLAG_UNKNOWN_27 | ITEM_FLAG_GLYPHES ??
			for(i=0; i<8 ;i++)
			{
				if( m_spellInfo->reagent[i] <= 0 || m_spellInfo->reagentCount[i] <= 0)
					continue;
				if(target && target->IsPlayer())
				{
				 if(static_cast<Player *>(target)->GetItemInterface()->GetItemCount(m_spellInfo->reagent[i]) < m_spellInfo->reagentCount[i])
				 {
					//Log.Warning("CanCast","SPELL_FAILED_ITEM_GONE");
					return SPELL_FAILED_ITEM_GONE; // "L'objet n'est plus disponible"
				 }
				}
			}

		// check if we have the required tools, totems, etc
		if( m_spellInfo->totem[0] != 0)
		{
			if(target && target->IsPlayer())
			{
			 if(!static_cast<Player *>(target)->GetItemInterface()->GetItemCount(m_spellInfo->totem[0]))
			 {
				//Log.Warning("CanCast","SPELL_FAILED_TOTEMS");
				return SPELL_FAILED_TOTEMS;
			 }
			}
		}
		if( m_spellInfo->totem[1] != 0)
		{
		  if(target && target->IsPlayer())
		  {
			if(!static_cast<Player *>(target)->GetItemInterface()->GetItemCount(m_spellInfo->totem[1]))
			{
				//Log.Warning("CanCast","SPELL_FAILED_TOTEMS2");
				return SPELL_FAILED_TOTEMS;
			}
		  }
		}
		// stealth check
		if( m_spellInfo->NameHash == SPELL_HASH_STEALTH || m_spellInfo->NameHash == SPELL_HASH_PROWL )
		{
		  if(target && target->IsPlayer())
		  {
			if( static_cast<Player *>(target)->CombatStatus.IsInCombat() )
			{
				//Log.Warning("CanCast","SPELL_FAILED_TARGET_IN_COMBAT");
				return SPELL_FAILED_TARGET_IN_COMBAT;
			}
		  }
		}

		/*Warrior can 'charge' in combat if he/she has Juggernaut talent
		if( m_spellInfo->NameHash == SPELL_HASH_CHARGE)
		{
			if( p_caster->CombatStatus.IsInCombat() )
				{
					if( p_caster->HasDummyAura( SPELL_HASH_JUGGERNAUT ) )
					p_caster->CastSpell( p_caster, 65219,true);
				}
		}*/

		if( m_caster->IsPlayer() && ( m_spellInfo->NameHash == SPELL_HASH_CANNIBALIZE || m_spellInfo->Id == 46584 )) // __Raise_Dead____15
		{
			bool check = false;
			for(Object::InRangeSet::iterator i = ((Player *)m_caster)->GetInRangeSetBegin(); i != ((Player *)m_caster)->GetInRangeSetEnd(); ++i)
			{
				if(((Player *)m_caster)->GetDistance2dSq((*i)) <= 25)
					if((*i)->GetTypeId() == TYPEID_UNIT || (*i)->GetTypeId() == TYPEID_PLAYER )
						if( static_cast<Unit *>(*i)->isDead() )
							check = true;
			}

			if( !check && m_spellInfo->Id == 46584 ) // __Raise_Dead____15
			{
				if( ((Player *)m_caster)->HasDummyAura( SPELL_HASH_GLYPH_OF_RAISE_DEAD ) )
					check = true;
				else if( ((Player *)m_caster)->GetItemInterface()->GetItemCount( 37201 ) )
				{
					check = true;
					((Player *)m_caster)->GetItemInterface()->RemoveItemAmt( 37201, 1 ); 
				}
			}
			if( !check )
			{
				//Log.Warning("CanCast","SPELL_FAILED_NO_EDIBLE_CORPSES");
				return SPELL_FAILED_NO_EDIBLE_CORPSES; // "Aucun cadavre a devorer a proximité"
			}
		}

		// check if we have the required gameobject focus
		if( m_spellInfo->requiresSpellFocus)
		{
			float focusRange;
			bool found = false;

            for(std::set<Object*>::iterator itr = ((Player *)m_caster)->GetInRangeSetBegin(); itr != ((Player *)m_caster)->GetInRangeSetEnd(); itr++ )
			{
				if((*itr)->GetTypeId() != TYPEID_GAMEOBJECT)
					continue;

				if((*itr)->GetByte(GAMEOBJECT_BYTES_1, 1) != GAMEOBJECT_TYPE_SPELL_FOCUS)
					continue;

				GameObjectInfo *info = ((GameObject*)(*itr))->GetInfo();
				if(!info)
				{
					sLog.outDebug("Warning: could not find info about game object %u",(*itr)->GetEntry());
					continue;
				}

				// professions use rangeIndex 1, which is 0yds, so we will use 5yds, which is standard interaction range.
				if(info->raw.sound1)
					focusRange = float(info->raw.sound1);
				else
					focusRange = GetMaxRange(dbcSpellRange.LookupEntry(m_spellInfo->rangeIndex));

				// check if focus object is close enough
				if(!IsInrange(m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), (*itr), (focusRange * focusRange)))
					continue;

				if(info->raw.SpellFocus == m_spellInfo->requiresSpellFocus)
				{
					found = true;
					break;
				}
			}

			if(!found)
			{
				//Log.Warning("CanCast","SPELL_FAILED_REQUIRES_SPELL_FOCUS");
				return SPELL_FAILED_REQUIRES_SPELL_FOCUS;
			}
		}

		if( m_spellInfo->AreaGroupId > 0)
		{
			bool found = false;
			uint16 area_id = m_caster->GetMapMgr()->GetAreaID( m_caster->GetPositionX(),m_caster->GetPositionY(), m_caster->GetPositionZ() );
			uint32 zone_id = m_caster->GetZoneId();

			AreaGroup const* groupEntry = dbcAreaGroup.LookupEntry( m_spellInfo->AreaGroupId );
			if( groupEntry )
			{
				for ( uint8 i=0; i<7; i++ )
				{
					if( groupEntry->AreaId[i] == zone_id || groupEntry->AreaId[i] == area_id )
					{
						found = true;
						break;
					}
				}
			}
			if(!found)
			{
				//Log.Warning("CanCast","SPELL_FAILED_REQUIRES_AREA2");
				return SPELL_FAILED_REQUIRES_AREA; // "Vous devez etre dans (%s)"
			}
		}

		// aurastate check
		if( m_spellInfo->casterAuraState )
		{
			if( !m_caster->HasFlag( UNIT_FIELD_AURASTATE, m_spellInfo->casterAuraState ) )
			{
				//Log.Warning("CanCast","SPELL_FAILED_CASTER_AURASTATE");
				return SPELL_FAILED_CASTER_AURASTATE; // "Vous ne pouvez pas encore faire cela"
			}
		}
	}

	// Targetted Item Checks
	if(m_targets.m_target && m_targets.m_target->IsItem() && m_caster->IsPlayer())
	{
		Item *i_target = NULL;

		// check if the targeted item is in the trade box
		if( m_targets.m_targetMask & TARGET_FLAG_TRADE_ITEM )
		{
			switch( m_spellInfo->Effect[0] )
			{
				// only lockpicking and enchanting can target items in the trade box
				case SPELL_EFFECT_OPEN_LOCK:
				case SPELL_EFFECT_ENCHANT_ITEM:
				case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY:
				{
					// check for enchants that can only be done on your own items
					if( m_spellInfo->attributesExB & ATTRIBUTESEXB_ENCHANT_OWN_ONLY )
					{
						//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS20");
						return SPELL_FAILED_BAD_TARGETS;
					}
					// get the player we are trading with
					Player* t_player = ((Player *)m_caster)->GetTradeTarget();
					// get the targeted trade item
					if( t_player != NULL && m_targets.m_target != NULL && m_targets.m_target->IsItem() )
						i_target = static_cast<Item*>(m_targets.m_target);
					/*if( t_player != NULL )
						i_target = t_player->getTradeItem((uint32)m_targets.m_itemTarget);*/						
				}
			}
		}
		// targeted item is not in a trade box, so get our own item
		else
		{
			if (m_targets.m_target != NULL && m_targets.m_target->IsItem())
				i_target = static_cast<Item*>(m_targets.m_target);
		}

		// check to make sure we have a targeted item
		if( i_target == NULL )
		{
			//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS21");
			return SPELL_FAILED_BAD_TARGETS;
		}

		ItemPrototype* proto = i_target->GetProto();

		// check to make sure we have it's prototype info
		if(!proto) 
		{
            //Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS22");		
			return SPELL_FAILED_BAD_TARGETS;
		}

		// check to make sure the targeted item is acceptable
		switch(m_spellInfo->Effect[0])
		{
			// Lock Picking Targeted Item Check
			case SPELL_EFFECT_OPEN_LOCK:
			{
				// this is currently being handled in SpellEffects
				break;
			}

			// Enchanting Targeted Item Check
			case SPELL_EFFECT_ENCHANT_ITEM:
			case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY:
			{
				// check for enchants that can only be done on your own items, make sure they are soulbound
				if( m_spellInfo->attributesExB & ATTRIBUTESEXB_ENCHANT_OWN_ONLY && i_target->GetOwner() != ((Player *)m_caster))
				{
					//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS23");
					return SPELL_FAILED_BAD_TARGETS; // "Cible incorrect" "Cet objet n'est pas une cible autorisée"
				}

				// check if we have the correct class, subclass, and inventory type of target item
				if( (m_spellInfo->EquippedItemClass != (int32)proto->Class) && (proto->Class != ITEM_CLASS_TRADEGOODS))
				{
					//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS24");
					return SPELL_FAILED_BAD_TARGETS; // "Cible incorrect" "Cet objet n'est pas une cible autorisée"
				}

				if( m_spellInfo->EquippedItemSubClass && !(m_spellInfo->EquippedItemSubClass & (1 << proto->SubClass)) &&  (m_spellInfo->EffectMiscValueB[0] != (int32)proto->SubClass) )
				{
					//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS25");
					return SPELL_FAILED_BAD_TARGETS; // "Cible incorrect" "Cet objet n'est pas une cible autorisée"
				}

				if( m_spellInfo->RequiredItemFlags && !(m_spellInfo->RequiredItemFlags & (1 << proto->InventoryType)) && proto->InventoryType != 0 )
				{
					//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS26");
					return SPELL_FAILED_BAD_TARGETS; // "Cible incorrect" "Cet objet n'est pas une cible autorisée"
				}

				if (m_spellInfo->Effect[0] == SPELL_EFFECT_ENCHANT_ITEM && 
					m_spellInfo->baseLevel && (m_spellInfo->baseLevel > proto->ItemLevel))
				{
					//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS27");
					return int8(SPELL_FAILED_BAD_TARGETS); // "Cible incorrect" "Cet objet n'est pas une cible autorisée"
				}
				                                                                         //       player created health/mana/poisons
				if( m_caster->IsItem() && ((Item *)m_caster)->GetProto()->Flags == 2097216) // 0x00200040 ITEM_FLAG_UNKNOWN_22 ITEM_FLAG_GLYPHES
					break;

				break;
			}

			// Disenchanting Targeted Item Check
			case SPELL_EFFECT_DISENCHANT:
			{
				// check if item can be disenchanted
				if(proto->DisenchantReqSkill < 1)
				{
					//Log.Warning("CanCast","SPELL_FAILED_CANT_BE_DISENCHANTED");
					return SPELL_FAILED_CANT_BE_DISENCHANTED;
				}

				// check if we have high enough skill
				if((int32)((Player *)m_caster)->_GetSkillLineCurrent(SKILL_ENCHANTING) < proto->DisenchantReqSkill)
				{
					//Log.Warning("CanCast","SPELL_FAILED_CANT_BE_DISENCHANTED_SKILL");
					return SPELL_FAILED_CANT_BE_DISENCHANTED_SKILL;
				}

				break;
			}

			// Feed Pet Targeted Item Check
			case SPELL_EFFECT_FEED_PET:
			{
				Pet *pPet = ((Player *)m_caster)->GetSummon();

				// check if we have a pet
				if(!pPet) 
				{
					//Log.Warning("CanCast","SPELL_FAILED_NO_PET");
					return SPELL_FAILED_NO_PET;
				}

				// check if item is food
				if(!proto->FoodType)
				{
					Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS30");
					return SPELL_FAILED_BAD_TARGETS;
				}

				// check if food type matches pets diet
				if(!(pPet->GetPetDiet() & (1 << (proto->FoodType - 1))))
				{
					Log.Warning("CanCast","SPELL_FAILED_WRONG_PET_FOOD");
					return SPELL_FAILED_WRONG_PET_FOOD;
				}

				// check food level: food should be max 30 lvls below pets level
				// Nouvelle regle Waad JdR: Géré dans void Spell::SpellEffectFeedPet(...) 
				/*if(pPet->getLevel() > proto->ItemLevel + 30) 
				{
					Log.Warning("CanCast","SPELL_FAILED_FOOD_LOWLEVEL");
					return SPELL_FAILED_FOOD_LOWLEVEL;
				}*/

				break;
			}

			// Prospecting Targeted Item Check
			case SPELL_EFFECT_PROSPECTING:
			{
				// check if the item can be prospected
				if(!(proto->Flags & ITEM_FLAG_PROSPECTABLE))
				{
					//Log.Warning("CanCast","SPELL_FAILED_CANT_BE_PROSPECTED");
					return SPELL_FAILED_CANT_BE_PROSPECTED;
				}

				// check if we have at least 5 of the item
				if(((Player *)m_caster)->GetItemInterface()->GetItemCount(proto->ItemId) < 5)
				{
					//Log.Warning("CanCast","SPELL_FAILED_ITEM_GONE");
					return SPELL_FAILED_ITEM_GONE;
				}

				// check if we have high enough skill
				if(((Player *)m_caster)->_GetSkillLineCurrent(SKILL_JEWELCRAFTING) < proto->RequiredSkillRank)
				{
					//Log.Warning("CanCast","SPELL_FAILED_LOW_CASTLEVEL");
					return SPELL_FAILED_LOW_CASTLEVEL;
				}

				break;
			}
			
			// Milling Targeted Item Check
			case SPELL_EFFECT_MILLING:
			{
				// check if the item can be milled
				if(!(proto->Flags & ITEM_FLAG_MILLABLE))
				{
					//Log.Warning("CanCast","SPELL_FAILED_CANT_BE_MILLED");
					return SPELL_FAILED_CANT_BE_MILLED;
				}

				// check if we have at least 5 of the item
				if(((Player *)m_caster)->GetItemInterface()->GetItemCount(proto->ItemId) < 5)
				{
					//Log.Warning("CanCast","SPELL_FAILED_ITEM_GONE");
					return SPELL_FAILED_ITEM_GONE;
				}

				// check if we have high enough skill
				if(((Player *)m_caster)->_GetSkillLineCurrent(SKILL_INSCRIPTION) < proto->RequiredSkillRank)
				{
					//Log.Warning("CanCast","SPELL_FAILED_LOW_CASTLEVEL");
					return SPELL_FAILED_LOW_CASTLEVEL;
				}

				break;
			}
		}
	}

	// set up our max Range
	float maxRange = GetMaxRange( dbcSpellRange.LookupEntry( m_spellInfo->rangeIndex ) );
		// latency compensation!!
		// figure out how much extra distance we need to allow for based on our movespeed and latency.
		if( m_caster->IsUnit() && m_targets.m_target && m_targets.m_target->IsUnit() )
		{
			Unit * utarget = static_cast<Unit*>(m_targets.m_target);
			if(utarget && utarget->IsPlayer() && static_cast< Player* >( utarget )->m_isMoving )
				{
					// this only applies to PvP.
					uint32 lat = static_cast< Player* >( utarget )->GetSession() ? static_cast< Player* >( utarget )->GetSession()->GetLatency() : 0;

					// if we're over 500 get fucked anyway.. your gonna lag! and this stops cheaters too
					lat = ( lat > 500 ) ? 500 : lat;

					// calculate the added distance
					maxRange += ( ((Unit *)m_caster)->m_runSpeed * 0.001f ) * float( lat );
				}
		}
		if( m_caster->IsPlayer() && ((Player *)m_caster)->m_isMoving )
		{
			// this only applies to PvP.
			uint32 lat = ((Player *)m_caster)->GetSession() ? ((Player *)m_caster)->GetSession()->GetLatency() : 0;

			// if we're over 500 get fucked anyway.. your gonna lag! and this stops cheaters too
			lat = ( lat > 500) ? 500 : lat;

			// calculate the added distance
			maxRange += ( ((Player *)m_caster)->m_runSpeed * 0.001f ) * float( lat );
		}
	if( (m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2]) && m_caster->IsUnit() )
	{
		SM_FFValue( ((Unit *)m_caster)->SM_FRange, &maxRange, m_spellInfo );
		SM_PFValue( ((Unit *)m_caster)->SM_PRange, &maxRange, m_spellInfo );
	}

	// Targeted Location Checks (AoE spells)
	if( m_targets.m_targetMask == TARGET_FLAG_DEST_LOCATION )
	{
		if( !IsInrange( m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, m_caster, ( maxRange * maxRange ) ) )
		{
			//Log.Warning("CanCast","SPELL_FAILED_OUT_OF_RANGE");
			return SPELL_FAILED_OUT_OF_RANGE;
		}
	}

	target = NULL;
	if( m_targets.m_targetMask == TARGET_FLAG_SELF )
		target = ((Unit *)m_caster);

	// Targeted Unit Checks
	if(m_targets.m_target)
	{
		if( (m_targets.m_target == m_caster) && m_caster->IsUnit() )
			target = static_cast<Unit *>(m_caster);
		/*else
			target = (m_caster->IsInWorld()) ? m_caster->GetMapMgr()->GetUnit(m_targets.m_unitTarget) : NULL;*/

		if(target)
		{
			if( target != m_caster )
			{
				// Partha: +2.52yds to max range, this matches the range the client is calculating.
				// see extra/supalosa_range_research.txt for more info

				if( tolerate ) // add an extra 33% to range on final check (squared = 1.78x)
				{
					if( !IsInrange( m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), target, ( ( maxRange + 2.52f ) * ( maxRange + 2.52f ) * 1.78f ) ) )
					{
						//Log.Warning("CanCast","SPELL_FAILED_OUT_OF_RANGE2");
						return SPELL_FAILED_OUT_OF_RANGE;
					}
				}
				else
				{
					// Added +2 because there's always someone who forgot to put CombatReach into the DB and latency compensation
					float targetRange = maxRange + target->GetSize() + (m_caster->IsUnit() ? ((Unit *)m_caster)->GetSize() : 0 ) + 2;
					if( !IsInrange(m_caster, target, targetRange * targetRange ) )
					{
						//Log.Warning("CanCast","SPELL_FAILED_OUT_OF_RANGE3");
						return SPELL_FAILED_OUT_OF_RANGE;
					}
				}
			}
			if( m_caster->IsPlayer() )
			{
				if( m_spellInfo->forced_creature_target )
				{
					if( !target->IsCreature() )
					{
						//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS40");
						return SPELL_FAILED_BAD_TARGETS;
					}

					if( static_cast<Creature *>( target )->GetCreatureInfo() != NULL )
						if( m_spellInfo->forced_creature_target != static_cast<Creature *>( target )->GetCreatureInfo()->Id )
						{
							//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS41");
							return SPELL_FAILED_BAD_TARGETS;
						}
				}

				if( m_spellInfo->Id == SPELL_RANGED_THROW)
				{
					Item * itm = ((Player *)m_caster)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
					if(!itm || itm->GetDurability() == 0)
					{
						//Log.Warning("CanCast","SPELL_FAILED_NO_AMMO");
						return SPELL_FAILED_NO_AMMO;
					}
				}

				// check aurastate
				if( m_spellInfo->targetAuraState && !skip)
				{
					if( !target->HasFlag( UNIT_FIELD_AURASTATE, 1<<(m_spellInfo->targetAuraState-1) ) )
					{
						return SPELL_FAILED_TARGET_AURASTATE;
					}
				}

				if(target->IsPlayer())
				{
					// disallow spell casting in sanctuary zones
					// allow attacks in duels
					if( ((Player *)m_caster)->DuelingWith != target && !isFriendly( m_caster, target ) )
					{
						AreaTable* atCaster = dbcArea.LookupEntry( ((Player *)m_caster)->GetAreaID() );
						AreaTable* atTarget = dbcArea.LookupEntry( static_cast< Player* >( target )->GetAreaID() );
						if( atCaster->AreaFlags & 0x800 || atTarget->AreaFlags & 0x800 )
						{
							//Log.Warning("CanCast","SPELL_FAILED_NOT_HERE");
							return SPELL_FAILED_NOT_HERE; // "Inutilisable ici"
						}
					}
				}
				
				if( m_spellInfo->EffectApplyAuraName[0] == SPELL_AURA_MOD_POSSESS)//mind control
				{
					if( m_spellInfo->EffectBasePoints[0])//got level req;
					{
						if((int32)target->getLevel() > m_spellInfo->EffectBasePoints[0]+1 + int32(((Player *)m_caster)->getLevel() - m_spellInfo->spellLevel))
						{
							//Log.Warning("CanCast","SPELL_FAILED_HIGHLEVEL");
							return SPELL_FAILED_HIGHLEVEL;
						}
						else if(target->GetTypeId() == TYPEID_UNIT) 
						{ 
							Creature *c = static_cast<Creature *>(target);
							if (c&&c->GetCreatureName()&&c->GetCreatureName()->Rank >ELITE_ELITE)
							{
								//Log.Warning("CanCast","SPELL_FAILED_HIGHLEVEL2");
								return SPELL_FAILED_HIGHLEVEL;
							}
						} 
					}
				}
				
				// pet training
				if( m_spellInfo->EffectImplicitTargetA[0] == EFF_TARGET_PET &&
					m_spellInfo->Effect[0] == SPELL_EFFECT_LEARN_SPELL )
				{
					Pet *pPet = ((Player *)m_caster)->GetSummon();
					// check if we have a pet
					if( pPet == NULL )
					{
						//Log.Warning("CanCast","SPELL_FAILED_NO_PET10");
						return SPELL_FAILED_NO_PET;
					}

					// other checks
					SpellEntry* trig = dbcSpell.LookupEntry( m_spellInfo->EffectTriggerSpell[0] );
					if( trig == NULL )
					{
						//Log.Warning("CanCast","SPELL_FAILED_SPELL_UNAVAILABLE10");
						return SPELL_FAILED_SPELL_UNAVAILABLE;
					}
				}
			}

			

			// if the target is not the unit caster and not the masters pet
			if((target != (Unit *)m_caster) && !m_caster->IsPet())
			{
				// Dummy spells check
				if( m_spellInfo->Id == __Banish_Burning_Exile)// Banish Burning Exile
				{
					if(target->GetEntry()!= 2760) // target needs to be a Burning Exile
					{
						//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS60");
						return SPELL_FAILED_BAD_TARGETS;
					}
				}
				if( m_spellInfo->Id == __Banish_Cresting_Exile)// Banish Cresting Exile
				{
					if(target->GetEntry()!= 2761) // target needs to be a Cresting Exile
					{
						//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS61");
						return SPELL_FAILED_BAD_TARGETS;
					}
				}
				if( m_spellInfo->Id == __Banish_Thundering_Exile)// Banish Thundering Exile
				{
					if(target->GetEntry()!= 2762) // target needs to be a Thundering Exile
					{
						//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS62");
						return SPELL_FAILED_BAD_TARGETS;
					}
				}
				if( m_spellInfo->Id == __Blackwhelp_Net) //Blackwhelp Net
				{
					// should only affect Wyrmcult Blackwhelps
					if(target->GetEntry()!= 21387)
					{
						//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS63");
						return SPELL_FAILED_BAD_TARGETS;
					}
				}

				/***********************************************************
				* Inface checks, these are checked in 2 ways
				* 1e way is check for damage type, as 3 is always ranged
				* 2e way is trough the data in the extraspell db
				*
				**********************************************************/

				/* burlex: units are always facing the target! */
				if(m_caster->IsPlayer())
				{
					if( m_spellInfo->facingCasterFlags & 1 )
					{
						if(m_spellInfo->in_front_status == 2 && target->isInFront(m_caster))
						{
							//Log.Warning("CanCast","SPELL_FAILED_NOT_BEHIND30");
							return SPELL_FAILED_NOT_BEHIND;
						}
						else if (!((Player *)m_caster)->isInFront(target))
						{
							//Log.Warning("CanCast","SPELL_FAILED_UNIT_NOT_INFRONT30");
							return SPELL_FAILED_UNIT_NOT_INFRONT;
						}
					}
				}
			}

			// if target is already skinned, don't let it be skinned again
			if( m_spellInfo->Effect[0] == SPELL_EFFECT_SKINNING) // skinning
			{
				if(target->IsUnit() && (((Creature*)target)->Skinned) )
				{
					//Log.Warning("CanCast","SPELL_FAILED_TARGET_UNSKINNABLE");
					return SPELL_FAILED_TARGET_UNSKINNABLE;
				}
			}

			// all spells with target 61 need to be in group or raid
			// TODO: need to research this if its not handled by the client!!!
			if(
				m_spellInfo->EffectImplicitTargetA[0] == EFF_TARGET_AREAEFFECT_PARTY_AND_CLASS || 
				m_spellInfo->EffectImplicitTargetA[1] == EFF_TARGET_AREAEFFECT_PARTY_AND_CLASS || 
				m_spellInfo->EffectImplicitTargetA[2] == EFF_TARGET_AREAEFFECT_PARTY_AND_CLASS)
			{
				if( target->IsPlayer() && !static_cast< Player* >( target )->InGroup() )
				{
					//Log.Warning("CanCast","SPELL_FAILED_NOT_READY31");
					return SPELL_FAILED_NOT_READY;//return SPELL_FAILED_TARGET_NOT_IN_PARTY or SPELL_FAILED_TARGET_NOT_IN_PARTY;
				}
			}

			// pet's owner stuff
			/*if (m_spellInfo->EffectImplicitTargetA[0] == EFF_TARGET_PET_MASTER || 
				m_spellInfo->EffectImplicitTargetA[1] == EFF_TARGET_PET_MASTER || 
				m_spellInfo->EffectImplicitTargetA[2] == EFF_TARGET_PET_MASTER)
			{
				if (!target->IsPlayer())
					return SPELL_FAILED_TARGET_NOT_PLAYER; //if you are there something is very wrong
			}*/

			// target 39 is fishing, all fishing spells are handled
			if( (m_spellInfo->EffectImplicitTargetA[0] == EFF_TARGET_SELF_FISHING) || 
			    (m_spellInfo->EffectImplicitTargetA[1] == EFF_TARGET_SELF_FISHING) || 
			    (m_spellInfo->EffectImplicitTargetA[2] == EFF_TARGET_SELF_FISHING) )
			{
				uint32 entry = m_spellInfo->EffectMiscValue[0];
				if(entry == GO_FISHING_BOBBER)
				{
					//uint32 mapid = p_caster->GetMapId();
					float px=m_caster->GetPositionX();
					float py=m_caster->GetPositionY();
					float pz=m_caster->GetPositionZ();
					float orient = m_caster->GetOrientation();
					float posx = 0,posy = 0,posz = 0;
					float co = cos(orient);
					float si = sin(orient);
					MapMgr * map = m_caster->GetMapMgr(); 

					float r;
					for(r=20; r>10; r--)
					{
						posx = px + r * co;
						posy = py + r * si;
						/*if(!(map->GetWaterType(posx,posy) & 1))//water 
							continue;*/
						posz = map->GetWaterHeight(posx,posy);
						if(posz > map->GetLandHeight(posx,posy,pz))//water
							break;
					}
					if(r<=10)
					{
						//Log.Warning("CanCast","SPELL_FAILED_NOT_FISHABLE");
						return SPELL_FAILED_NOT_FISHABLE;
					}

					// if we are already fishing, dont cast it again
					if(((Player *)m_caster)->GetSummonedObject())
					{
						if(((Player *)m_caster)->GetSummonedObject()->GetEntry() == GO_FISHING_BOBBER)
						{
							//Log.Warning("CanCast","SPELL_FAILED_SPELL_IN_PROGRESS70");
							return SPELL_FAILED_SPELL_IN_PROGRESS;
						}
					}
				}
			}

			if( m_caster->IsPlayer() )
			{
				if( m_spellInfo->NameHash == SPELL_HASH_GOUGE )// Gouge
				{
					if(!target->isInFront(m_caster))
					{
						//Log.Warning("CanCast","SPELL_FAILED_NOT_INFRONT80");
						return SPELL_FAILED_NOT_INFRONT;
					}
				}

				if( m_spellInfo->Category==1131)//Hammer of wrath, requires target to have 20- % of hp
				{
					if(target->GetUInt32Value(UNIT_FIELD_HEALTH) == 0)
					{
						//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS90");
						return SPELL_FAILED_BAD_TARGETS;
					}

					if(target->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/target->GetUInt32Value(UNIT_FIELD_HEALTH)<5)
					{
						//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS91");
						 return SPELL_FAILED_BAD_TARGETS;
					}
				}
				else if( m_spellInfo->Category == 672)//Conflagrate, requires immolation spell on victim
				{
					if(!target->HasAuraVisual(46))
					{
						//Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS92");
						return SPELL_FAILED_BAD_TARGETS;
					}
				}

				if( m_spellInfo->NameHash == SPELL_HASH_ENVENOM )
				{
					if( !target->HasAuraVisual(5100) )
					{
						Log.Warning("CanCast","SPELL_FAILED_BAD_TARGETS93");
						return SPELL_FAILED_BAD_TARGETS ;
					}
				}	

				if(target->dispels[m_spellInfo->dispelType])
				{
					//Log.Warning("CanCast","SPELL_FAILED_DAMAGE_IMMUNE");
					return SPELL_FAILED_DAMAGE_IMMUNE; // SPELL_FAILED_PREVENTED_BY_MECHANIC-1;			// hackfix - burlex
				}

			}

			// if we're replacing a higher rank, deny it
			if( m_spellInfo->buffType > 0 && target != m_caster)
			{
				AuraCheckResponse acr = target->AuraCheck(m_spellInfo);
				if( acr.Error == AURA_CHECK_RESULT_HIGHER_BUFF_PRESENT )
				{
					//Log.Warning("CanCast","SPELL_FAILED_AURA_BOUNCED");
					return SPELL_FAILED_AURA_BOUNCED;
				}
			}

			//check if we are trying to stealth or turn invisible but it is not allowed right now
			if( IsStealthSpell() || IsInvisibilitySpell() )
			{
				//if we have Faerie Fire, we cannot stealth or turn invisible
				if( ((Unit *)m_caster)->HasNegativeAuraWithNameHash( SPELL_HASH_FAERIE_FIRE ) || 
					((Unit *)m_caster)->HasNegativeAuraWithNameHash( SPELL_HASH_FAERIE_FIRE__FERAL_ ) )
				{
					//Log.Warning("CanCast","SPELL_FAILED_SPELL_UNAVAILABLE80");
					return SPELL_FAILED_SPELL_UNAVAILABLE;
				}
			}

			if( target->IsPlayer() )
			{
				switch( m_spellInfo->NameHash )
				{
				case SPELL_HASH_DIVINE_PROTECTION:
				case SPELL_HASH_DIVINE_SHIELD:
				case SPELL_HASH_HAND_OF_PROTECTION:
					{
						if( static_cast<Player *>(target)->mForbearance )
						{
							//Log.Warning("CanCast","SPELL_FAILED_DAMAGE_IMMUNE90");
							return SPELL_FAILED_DAMAGE_IMMUNE;
						}

						if( !static_cast<Player *>(target)->mAvengingWrath )
						{
							//Log.Warning("CanCast","SPELL_FAILED_DAMAGE_IMMUNE91");
							return SPELL_FAILED_DAMAGE_IMMUNE;
						}
					}break;

				case SPELL_HASH_AVENGING_WRATH:
					{
						if( !static_cast<Player *>(target)->mAvengingWrath )
						{
							//Log.Warning("CanCast","SPELL_FAILED_DAMAGE_IMMUNE92");
							return SPELL_FAILED_DAMAGE_IMMUNE;
						}
					}break;

				case SPELL_HASH_ICE_BLOCK:
					{
						if( static_cast<Player *>(target)->mHypothermia )
						{
							//Log.Warning("CanCast","SPELL_FAILED_DAMAGE_IMMUNE93");
							return SPELL_FAILED_DAMAGE_IMMUNE;
						}

					}break;

				case SPELL_HASH_POWER_WORD__SHIELD:
					{
						if( static_cast<Player *>(target)->mWeakenedSoul )
						{
							//Log.Warning("CanCast","SPELL_FAILED_DAMAGE_IMMUNE94");
							return SPELL_FAILED_DAMAGE_IMMUNE;
						}
					}break;

				case SPELL_HASH_FIRST_AID:
					{
						if( static_cast<Player *>(target)->mRecentlyBandaged )
						{
							//Log.Warning("CanCast","SPELL_FAILED_DAMAGE_IMMUNE95");
							return SPELL_FAILED_DAMAGE_IMMUNE;
						}
					}break;
				case SPELL_HASH_HEROISM:
					{
						if( static_cast<Player *>(target)->mExhaustion )
						{
							//Log.Warning("CanCast","SPELL_FAILED_DAMAGE_IMMUNE96");
							return SPELL_FAILED_DAMAGE_IMMUNE;
						}
					}break;
				case SPELL_HASH_BLOODLUST:
					{
						if( static_cast<Player *>(target)->mSated )
						{
							//Log.Warning("CanCast","SPELL_FAILED_DAMAGE_IMMUNE97");
							return SPELL_FAILED_DAMAGE_IMMUNE;
						}
					}break;
				}
			}

			if (m_spellInfo->mechanics == 16 && target->mRecentlyBandaged)
			{
				//Log.Warning("CanCast","SPELL_FAILED_DAMAGE_IMMUNE98");
				return SPELL_FAILED_DAMAGE_IMMUNE;
			}
		}
	}	

	// Special State Checks (for creatures & players)
	if( m_caster->IsUnit() )
	{
		if( ((Unit *)m_caster)->SchoolCastPrevent[m_spellInfo->School] )
		{	
			uint32 now_ = getMSTime();
			if( now_ < ((Unit *)m_caster)->SchoolCastPrevent[m_spellInfo->School] )
			{
				//Log.Warning("CanCast","SPELL_FAILED_SILENCED");
				return SPELL_FAILED_SILENCED;
			}
		}

		if( ((Unit *)m_caster)->m_silenced && m_spellInfo->School != NORMAL_DAMAGE )
		{
				// HACK FIX
				switch( m_spellInfo->NameHash )
				{
					case SPELL_HASH_ICE_BLOCK: //Ice Block
					case SPELL_HASH_DIVINE_SHIELD: //Divine Shield 0x9840A1A6
						break;

					case SPELL_HASH_WILL_OF_THE_FORSAKEN: //Will of the Forsaken
						{
							if( ((Unit *)m_caster)->m_special_state & ( UNIT_STATE_FEAR | UNIT_STATE_CHARM | UNIT_STATE_SLEEP ) )
								break;
						}break;

					case SPELL_HASH_DEATH_WISH: //Death Wish
					case SPELL_HASH_FEAR_WARD: //Fear Ward
					case SPELL_HASH_BERSERKER_RAGE: //Berserker Rage
						{
							if( ((Unit *)m_caster)->m_special_state & UNIT_STATE_FEAR )
								break;
						}break;

                    // A Aprofondir (Branruz)
					// {Insignia|Medallion} of the {Horde|Alliance}
					//case 0xC7C45478: // SPELL_HASH_WORD_OF_RECALL__OLD_ (???) Immune Movement Impairment and Loss of Control
					//case 0x048c32f9: // insignia of the alliance/horde (n'existe plus - Branruz)
					case SPELL_HASH_EVERY_MAN_FOR_HIMSELF:
					case SPELL_HASH_DISPERSION:
						{
							break;
						}

					case SPELL_HASH_BARKSKIN: // Barksin
					{ // This spell is usable while stunned, frozen, incapacitated, feared or asleep.  Lasts 12 sec.
						if( ((Unit *)m_caster)->m_special_state & ( UNIT_STATE_STUN | UNIT_STATE_FEAR | UNIT_STATE_SLEEP ) ) // Uh, what unit_state is Frozen? (freezing trap...)
							break;
					}

					default:
						//Log.Warning("CanCast","SPELL_FAILED_SILENCED2");
						return SPELL_FAILED_SILENCED;
				}
		}

		if(target) /* -Supalosa- Shouldn't this be handled on Spell Apply? */
		{
			for( int i = 0; i < 3; i++ ) // if is going to cast a spell that breaks stun remove stun auras, looks a bit hacky but is the best way i can find
			{
				if( m_spellInfo->EffectApplyAuraName[i] == SPELL_AURA_MECHANIC_IMMUNITY )
				{
					target->RemoveAllAurasByMechanic( m_spellInfo->EffectMiscValue[i] , -1 , true );
					// Remove all debuffs of that mechanic type.
					// This is also done in SpellAuras.cpp - wtf?
				}
				/*
				if( m_spellInfo->EffectApplyAuraName[i] == SPELL_AURA_MECHANIC_IMMUNITY && (m_spellInfo->EffectMiscValue[i] == 12 || m_spellInfo->EffectMiscValue[i] == 17))
				{
					for(uint32 x=MAX_POSITIVE_AURAS;x<MAX_AURAS;x++)
						if(target->m_auras[x])
							if(target->m_auras[x]->GetSpellProto()->mechanics == m_spellInfo->EffectMiscValue[i])
								target->m_auras[x]->Remove();
				}
				*/
			}
		}

		if( ((Unit *)m_caster)->IsPacified() && m_spellInfo->School == NORMAL_DAMAGE ) // only affects physical damage
		{
			// HACK FIX
			switch( m_spellInfo->NameHash )
			{
				case SPELL_HASH_ICE_BLOCK:            // Ice Block
				case SPELL_HASH_DIVINE_SHIELD:        // Divine Shield
				case SPELL_HASH_WILL_OF_THE_FORSAKEN: // Will of the Forsaken
				{
					if( ((Unit *)m_caster)->m_special_state & (UNIT_STATE_FEAR | UNIT_STATE_CHARM | UNIT_STATE_SLEEP))
						break;
				}break;
				case SPELL_HASH_PVP_TRINKET: // insignia of the alliance/horde 2.4.3
				case SPELL_HASH_EVERY_MAN_FOR_HIMSELF:
				case SPELL_HASH_DISPERSION:
					break;

				default:
					//Log.Warning("CanCast","SPELL_FAILED_PACIFIED");
					return SPELL_FAILED_PACIFIED;
			}
		}

		if( ((Unit *)m_caster)->IsStunned() || ((Unit *)m_caster)->IsFeared())
		{
			// HACK FIX
			switch( m_spellInfo->NameHash )
			{
				case SPELL_HASH_HAND_OF_FREEDOM:
				{
					if( !((Unit *)m_caster)->HasDummyAura(SPELL_HASH_DIVINE_PURPOSE) )
					{
						//Log.Warning("CanCast","SPELL_FAILED_STUNNED190");
						return SPELL_FAILED_STUNNED;
					}
				}break;
				case SPELL_HASH_ICE_BLOCK: //Ice Block
				case SPELL_HASH_DIVINE_SHIELD: //Divine Shield
				case SPELL_HASH_DIVINE_PROTECTION: //Divine Protection
				case SPELL_HASH_BARKSKIN: //Barkskin
					break;
				/* -Supalosa- For some reason, being charmed or sleep'd is counted as 'Stunned'. 
				Check it: http://www.wowhead.com/?spell=700 */

				case SPELL_HASH_PVP_TRINKET: /* Immune Movement Impairment and Loss of Control (PvP Trinkets) */
					break;

				case SPELL_HASH_WILL_OF_THE_FORSAKEN: /* Will of the Forsaken (Undead Racial) */
					break;

				//case 0x048c32f9:	// insignia of the alliance/horde, n'existe plus (Branruz)
				case SPELL_HASH_EVERY_MAN_FOR_HIMSELF:
					break;

				case SPELL_HASH_BLINK:
					break;

				case SPELL_HASH_DISPERSION:
					break;

				default:
					//Log.Warning("CanCast","SPELL_FAILED_STUNNED191");
					return SPELL_FAILED_STUNNED;
			}
		}

		if(((Unit *)m_caster)->GetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT) > 0)
		{
			SpellEntry * t_spellInfo = (((Unit *)m_caster)->GetCurrentSpell() ? ((Unit *)m_caster)->GetCurrentSpell()->m_spellInfo : NULL);

			if(!t_spellInfo || !m_triggeredSpell)
			{
				 //Log.Warning("CanCast","SPELL_FAILED_SPELL_IN_PROGRESS 3");
				return SPELL_FAILED_SPELL_IN_PROGRESS;
			}
			else if (t_spellInfo)
			{
				if(
					t_spellInfo->EffectTriggerSpell[0] != m_spellInfo->Id &&
					t_spellInfo->EffectTriggerSpell[1] != m_spellInfo->Id &&
					t_spellInfo->EffectTriggerSpell[2] != m_spellInfo->Id)
				{
					 //Log.Warning("CanCast","SPELL_FAILED_SPELL_IN_PROGRESS 4");
					return SPELL_FAILED_SPELL_IN_PROGRESS;
				}
			}
		}
	}

	// no problems found, so we must be ok
	//Log.Success("CanCast","SPELL_CANCAST_OK");
	return SPELL_CANCAST_OK;
}

void Spell::RemoveItems(Item *Item_To_Remove)
{
	// Item Charges & Used Item Removal
	if(Item_To_Remove)
	{
		// Stackable Item -> remove 1 from stack
		if(Item_To_Remove->GetUInt32Value(ITEM_FIELD_STACK_COUNT) > 1)
		{
			Item_To_Remove->ModUnsigned32Value(ITEM_FIELD_STACK_COUNT, -1);
			Item_To_Remove->m_isDirty = true;
		}
		// Expendable Item
		else if(Item_To_Remove->GetProto()->Spells[0].Charges < 0
		     || Item_To_Remove->GetProto()->Spells[1].Charges == -1) // hackfix for healthstones/mana gems/depleted items
		{
			// if item has charges remaining -> remove 1 charge
			if(((int32)Item_To_Remove->GetUInt32Value(ITEM_FIELD_SPELL_CHARGES)) < -1)
			{
				Item_To_Remove->ModUnsigned32Value(ITEM_FIELD_SPELL_CHARGES, 1);
				Item_To_Remove->m_isDirty = true;
			}
			// if item has no charges remaining -> delete item
			else
			{
				Item_To_Remove->GetOwner()->GetItemInterface()->SafeFullRemoveItemByGuid(((Item *)Item_To_Remove)->GetGUID());
				m_caster = NULL;
			}
		}
		// Non-Expendable Item -> remove 1 charge
		else if(Item_To_Remove->GetProto()->Spells[0].Charges > 0)
		{
			Item_To_Remove->ModUnsigned32Value(ITEM_FIELD_SPELL_CHARGES, -1);
			Item_To_Remove->m_isDirty = true;
		}
	} 

	// Ammo Removal
	if( ((Player *)m_caster) && (m_spellInfo->attributesExB == ATTRIBUTESEXB_REQ_RANGED_WEAPON || m_spellInfo->attributesExC & ATTRIBUTESEXC_PLAYER_RANGED_SPELLS))
	{
		Item *_item = (Item *)m_targets.m_target;
		((Player *)m_caster)->GetItemInterface()->RemoveItemAmt_ProtectPointer(((Player *)m_caster)->GetUInt32Value(PLAYER_AMMO_ID), 1, & _item);
	}

	// Reagent Removal
	for(uint32 i=0; i<8 ;i++)
	{
		if( ((Player *)m_caster) && m_spellInfo->reagent[i])
		{
			Item *_item = (Item *)m_targets.m_target;
			((Player *)m_caster)->GetItemInterface()->RemoveItemAmt_ProtectPointer(m_spellInfo->reagent[i], m_spellInfo->reagentCount[i], & _item);
		}
	}
}

int32 Spell::CalculateEffect(uint32 i,Unit *target)
{
	// TODO: Add ARMOR CHECKS; Add npc that have ranged weapons use them;

	// Range checks
 /*   if (m_spellInfo->Id == SPELL_RANGED_GUN) // this includes bow and gun
	{
		if(!u_caster || !unitTarget)
			return 0;

		return ::CalculateDamage( u_caster, unitTarget, RANGED, m_spellInfo->SpellGroupType );
	}
*/
	if( m_spellInfo->Id == 3018 )
		m_spellInfo = dbcSpell.LookupEntry(75);

	int32 value = 0;

	// Test pointeur par Franck
	if ( m_spellInfo == NULL )
	{
		Log.Error("[Spell::CalculateEffect]","m_spellInfo = NULL ?"); 
		return 0;
	}
	
	float basePointsPerLevel    = m_spellInfo->EffectRealPointsPerLevel[i];
	float randomPointsPerLevel  = 0; // m_spellInfo->EffectDicePerLevel[i]; - Disparu 335.12340
	int32 basePoints = m_spellInfo->EffectBasePoints[i] + 1;
	int32 randomPoints = m_spellInfo->EffectDieSides[i];

	//added by Zack : some talents inherit their basepoints from the previously casted spell: see mage - Master of Elements
	if(forced_basepoints[i])
		basePoints = forced_basepoints[i];

	/* Random suffix value calculation */
	/*if(i_caster && (int32(i_caster->GetUInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID)) < 0))
	{
        ItemRandomSuffixEntry * si = dbcItemRandomSuffix.LookupEntry(abs(int32(i_caster->GetUInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID))));
		EnchantEntry * ent;
		uint32 j,k;

		// Test pointeur par Franck
		if ( (si == NULL)) // || (ent == NULL) )(Branruz)
		{
			Log.Error("[Spell::CalculateEffect]","si est NULL ?!?"); 
			return 0;
		}
		
		for(j = 0; j < 3; ++j)
		{
			if(si->enchantments[j] != 0)
			{
				ent = dbcEnchant.LookupEntry(si->enchantments[j]);
				if(ent == NULL) // Normalement ne peut pas arriver (Branruz)
				{
					Log.Error("[Spell::CalculateEffect]","Enchantement non trouvee (%u) <- Report this to dev.",si->enchantments[j]);
					continue; // On essaye le suivant
				}
				for(k = 0; k < 3; ++k)
				{
					if(ent->spell[k] == m_spellInfo->Id)
					{
						if(si->prefixes[k] == 0)
							goto exit;
						
						value = RANDOM_SUFFIX_MAGIC_CALCULATION(si->prefixes[j], i_caster->GetItemRandomSuffixFactor());
						
						if(value == 0)
							goto exit;

						return value;
					}
				}
			}
		}
	}
exit:*/

	if( m_caster && m_caster->IsUnit() )
	{
		int32 diff = -(int32)m_spellInfo->baseLevel;
		if (m_spellInfo->maxLevel && ((Unit *)m_caster)->getLevel()>m_spellInfo->maxLevel)
			diff +=m_spellInfo->maxLevel;
		else
			diff +=((Unit *)m_caster)->getLevel();
		randomPoints += float2int32(diff * randomPointsPerLevel);
		basePoints += float2int32(diff * basePointsPerLevel );
	}

	if(randomPoints <= 1)
		value = basePoints;
	else
		value = basePoints + rand() % randomPoints;

	// Slam	
	if( m_spellInfo->NameHash == SPELL_HASH_SLAM )
	{
		if( m_caster && m_caster->IsPlayer() )
		{
			Item *it;
			if(((Player *)m_caster)->GetItemInterface())
			{
				it = ((Player *)m_caster)->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
				if(it)
				{
					float weapondmg = ( it->GetProto()->Damage[0].Max + it->GetProto()->Damage[0].Min ) / 2;
					value += float2int32( m_spellInfo->EffectBasePoints[0] + weapondmg );
				}
			}
		}
	}
	else if( m_spellInfo->NameHash == SPELL_HASH_RAKE )//rake
	{
		//Rake the target for ${$AP/100+$m1} bleed damage and an additional ${$m2*3+$AP*0.06} damage over $d.
		if( m_caster && m_caster->IsUnit() ) 
		{
			float ap = (float)((Unit *)m_caster)->GetAP();
			if(i==0)
				value+=(uint32)ceilf((ap*0.01f));	// / 100
			else if(i==1)
				value=(int32)ceilf((float(value * 3) + ceilf((ap*0.06f))) / 3.0f);
		}
	}
	else if( m_spellInfo->NameHash == SPELL_HASH_RUPTURE )
	{
		/* 
		1pt = Attack Power * 0.04 + x
		2pt = Attack Power * 0.10 + y
		3pt = Attack Power * 0.18 + z
		4pt = Attack Power * 0.21 + a
		5pt = Attack Power * 0.24 + b
		*/
		if( m_caster && m_caster->IsPlayer() )
		{
			float ap = (float)((Player *)m_caster)->GetAP();
			float cp = (float)static_cast<Player *>(m_caster)->m_comboPoints;
			value += (uint32) ceilf( ( ap * ( 0.04f * cp ) ) / ( ( 6 + ( cp * 2 ) ) / 2 ) );
		}
	}
	else if( m_spellInfo->Id == 60103 && m_caster->IsPlayer() && i == 0) // Lava Lash
	{   // Check if offhand is enchanted with Flametongue
		// TODO less hacky implementation		
		ItemInterface * ii = ((Player *)m_caster)->GetItemInterface();
		if(ii)
		{
			Item *offhandweapon = ii->GetInventoryItem(INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_OFFHAND);
			if(offhandweapon)
			{
				EnchantmentInstance * ei = offhandweapon->GetEnchantment(1);
				if(ei)
				{
					EnchantEntry * e = ei->Enchantment;
					if(e)
					{
						uint32 id = e->Id;
						switch(id)
						{
						case 3:
						case 4:
						case 5:
						case 523:
						case 1665:
						case 1666:
						case 2634:
						case 3779:
						case 3780:
						case 3781:
							uint32 bonus = 125;
							if(m_caster->IsUnit() && ((Unit *)m_caster)->HasAura(55444)) // Glyph of Lava Lash
								bonus += 10;
							value = value * bonus / 100;
						}	
					}
				}
			}
		}
	}
	
	if( m_caster && m_caster->IsPlayer() )
	{
		int32 comboDamage = (int32)m_spellInfo->EffectPointsPerComboPoint[i];
		if(comboDamage)
		{
			value += ( comboDamage * ((Player *)m_caster)->m_comboPoints );
			m_requiresCP = true;
			//this is ugly so i will explain the case maybe someone ha a better idea :
			// while casting a spell talent will trigger uppon the spell prepare faze
			// the effect of the talent is to add 1 combo point but when triggering spell finishes it will clear the extra combo point
			((Player *)m_caster)->m_spellcomboPoints = 0;
		}

		SpellOverrideMap::iterator itr = ((Player *)m_caster)->mSpellOverrideMap.find(m_spellInfo->Id);
		if(itr != ((Player *)m_caster)->mSpellOverrideMap.end())
		{
			ScriptOverrideList::iterator itrSO;
			for(itrSO = itr->second->begin(); itrSO != itr->second->end(); ++itrSO)
			{
				//DK:FIXME->yeni bir map oluþtur
                // Capt: WHAT THE FUCK DOES THIS MEAN....
				// Supa: WHAT THE FUCK DOES THIS MEAN?
				value += RandomUInt((*itrSO)->damage);
			}
		}
	 }

	if (m_spellScript != NULL)
		m_spellScript->CalculateEffect(i, target, &value);

	if( m_caster && m_caster->IsItem() && target && target->GetMapMgr() && ((Item *)m_caster)->GetUInt64Value( ITEM_FIELD_CREATOR ) )
	{	
		//we should inherit the modifiers from the conjured food caster
		Unit *item_creator = target->GetMapMgr()->GetUnit( ((Item *)m_caster)->GetUInt64Value( ITEM_FIELD_CREATOR ) );
		//if( item_creator != NULL ) caster = item_creator;
	}

	if( m_caster && m_caster->IsUnit() )
	{
		int32 spell_flat_modifers=0;
		int32 spell_pct_modifers=0;
		int32 spell_pct_modifers2=0;//separated from the other for debugging purpuses

		SM_FIValue(((Unit *)m_caster)->SM_FSPELL_VALUE,&spell_flat_modifers, m_spellInfo);
		SM_FIValue(((Unit *)m_caster)->SM_PSPELL_VALUE,&spell_pct_modifers, m_spellInfo);

		SM_FIValue(((Unit *)m_caster)->SM_FEffectBonus,&spell_flat_modifers, m_spellInfo);
		SM_FIValue(((Unit *)m_caster)->SM_PEffectBonus,&spell_pct_modifers, m_spellInfo);

		//wee
		switch (i)
		{
		case 0:
			SM_FIValue(((Unit *)m_caster)->SM_FEffect1_Bonus, &spell_flat_modifers, m_spellInfo);
			SM_FIValue(((Unit *)m_caster)->SM_PEffect1_Bonus, &spell_pct_modifers, m_spellInfo);
			break;
		case 1:
			SM_FIValue(((Unit *)m_caster)->SM_FEffect2_Bonus, &spell_flat_modifers, m_spellInfo);
			SM_FIValue(((Unit *)m_caster)->SM_PEffect2_Bonus, &spell_pct_modifers, m_spellInfo);
			break;
		case 2:
			SM_FIValue(((Unit *)m_caster)->SM_FEffect3_Bonus, &spell_flat_modifers, m_spellInfo);
			SM_FIValue(((Unit *)m_caster)->SM_PEffect3_Bonus, &spell_pct_modifers, m_spellInfo);
			break;
		}
		
		value = value + value*(spell_pct_modifers+spell_pct_modifers2)/100 + spell_flat_modifers;

	}
	else if( m_caster && m_caster->IsItem() && target)
	{	
		//we should inherit the modifiers from the conjured food caster
		Unit *item_creator = target->GetMapMgr()->GetUnit( ((Item *)m_caster)->GetUInt64Value( ITEM_FIELD_CREATOR ) );
		if( item_creator != NULL )
		{
			int32 spell_flat_modifers=0;
			int32 spell_pct_modifers=0;
			int32 spell_pct_modifers2=0;//separated from the other for debugging purpuses

			SM_FIValue(item_creator->SM_FSPELL_VALUE,&spell_flat_modifers,m_spellInfo);
			SM_FIValue(item_creator->SM_PSPELL_VALUE,&spell_pct_modifers,m_spellInfo);

			SM_FIValue(item_creator->SM_FEffectBonus,&spell_flat_modifers,m_spellInfo);
			SM_FIValue(item_creator->SM_PEffectBonus,&spell_pct_modifers,m_spellInfo);
/*
#ifdef COLLECTION_OF_UNTESTED_STUFF_AND_TESTERS
			if(spell_flat_modifers!=0 || spell_pct_modifers!=0 || spell_pct_modifers2!=0)
				printf("!!!!ITEMCASTER ! : spell value mod flat %d , spell value mod pct %d, spell value mod pct2 %d , spell dmg %d, spell group %u %u %u\n",spell_flat_modifers,
				spell_pct_modifers,spell_pct_modifers2,value,m_spellInfo->SpellGroupType[0],m_spellInfo->SpellGroupType[1],m_spellInfo->SpellGroupType[2]);
#endif
*/
			value = value + value*(spell_pct_modifers+spell_pct_modifers2)/100 + spell_flat_modifers;
		}
	}

	if (m_spellScript != NULL)
		m_spellScript->CalculatePostEffect(i, target, &value);
	
	return value;
}

void Spell::HandleTeleport(uint32 id, Unit* Target)
{
	if(Target->GetTypeId()!=TYPEID_PLAYER)
		return;

	Player* pTarget = static_cast< Player* >( Target );
   
	float x,y,z;
	uint32 mapid;
	
    // predefined behavior                   // 556 - Astral Recall ; 39937 - Ruby Slippers
	if (m_spellInfo->Id == __Hearthstone || 
		m_spellInfo->Id == __Astral_Recall ||
		//m_spellInfo->Id == 39937) // __There_s_No_Place_Like_Home
		m_spellInfo->Id == __Aurora_Slippers    ||
		m_spellInfo->Id == __Glacial_Slippers   ||
		m_spellInfo->Id == __Soulguard_Slippers ||
		m_spellInfo->Id == __Spider_Silk_Slippers )
	{
		x = pTarget->GetBindPositionX();
		y = pTarget->GetBindPositionY();
		z = pTarget->GetBindPositionZ();
		mapid = pTarget->GetBindMapId();
	}
	else // normal behavior
	{
		TeleportCoords* TC = TeleportCoordStorage.LookupEntry(id);
		if(!TC)
			return;
		 
		x=TC->x;
		y=TC->y;
		z=TC->z;
		mapid=TC->mapId;
	}

	pTarget->EventAttackStop();
	pTarget->SetSelection(0);
	  
	// We use a teleport event on this one. Reason being because of UpdateCellActivity,
	// the game object set of the updater thread WILL Get messed up if we teleport from a gameobject
	// caster.
	if(!sEventMgr.HasEvent(pTarget, EVENT_PLAYER_TELEPORT))
		sEventMgr.AddEvent(pTarget, &Player::EventTeleport, mapid, x, y, z, EVENT_PLAYER_TELEPORT, 1, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Spell::CreateItem(uint32 itemId)
{
    if( !itemId )
        return;

	Player*			pUnit = static_cast< Player* >( m_caster );
	Item*			newItem;
	Item*			add;
	SlotResult		slotresult;
	ItemPrototype*	m_itemProto;

	m_itemProto = ItemPrototypeStorage.LookupEntry( itemId );
	if( m_itemProto == NULL )
	    return;

	if (pUnit->GetItemInterface()->CanReceiveItem(m_itemProto, 1))
	{
		SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
		return;
	}

	add = pUnit->GetItemInterface()->FindItemLessMax(itemId, 1, false);
	if (!add)
	{
		slotresult = pUnit->GetItemInterface()->FindFreeInventorySlot(m_itemProto);
		if(!slotresult.Result)
		{
			 SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
			 return;
		}
		
		newItem = objmgr.CreateItem(itemId, pUnit);
		AddItemResult result = pUnit->GetItemInterface()->SafeAddItem(newItem, slotresult.ContainerSlot, slotresult.Slot);
		if(!result)
		{
			delete newItem;
			return;
		}

		newItem->SetUInt64Value(ITEM_FIELD_CREATOR,m_caster->GetGUID());
		newItem->SetUInt32Value(ITEM_FIELD_STACK_COUNT, damage);

		/*WorldPacket data(45);
		((Player *)m_caster)->GetSession()->BuildItemPushResult(&data, p_caster->GetGUID(), 1, 1, itemId ,0,0xFF,1,0xFFFFFFFF);
		((Player *)m_caster)->SendMessageToSet(&data, true);*/
		((Player *)m_caster)->GetSession()->SendItemPushResult(newItem,true,false,true,true,slotresult.ContainerSlot,slotresult.Slot,1);
		newItem->m_isDirty = true;

	} 
	else 
	{
		add->SetUInt32Value(ITEM_FIELD_STACK_COUNT,add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + damage);
		/*WorldPacket data(45);
		((Player *)m_caster)->GetSession()->BuildItemPushResult(&data, p_caster->GetGUID(), 1, 1, itemId ,0,0xFF,1,0xFFFFFFFF);
		((Player *)m_caster)->SendMessageToSet(&data, true);*/
		((Player *)m_caster)->GetSession()->SendItemPushResult(add,true,false,true,false,
			                                      ((Player *)m_caster)->GetItemInterface()->GetBagSlotByGuid(add->GetGUID()),0xFFFFFFFF,1);
		add->m_isDirty = true;
	}
}

/*void Spell::_DamageRangeUpdate()
{
	if(unitTarget)
	{
		if(unitTarget->isAlive())
		{
			m_caster->SpellNonMeleeDamageLog(unitTarget,m_spellInfo->Id, damageToHit);
		}
		else
		{	if( u_caster != NULL )
			if(u_caster->GetCurrentSpell() != this)
			{
					if(u_caster->GetCurrentSpell() != NULL)
					{
						u_caster->GetCurrentSpell()->SendChannelUpdate(0);
						u_caster->GetCurrentSpell()->cancel();
					}
			}
			SendChannelUpdate(0);
			cancel();
		}
		sEventMgr.RemoveEvents(this, EVENT_SPELL_DAMAGE_HIT);
		delete this;
	}
	else if(gameObjTarget)
	{
		sEventMgr.RemoveEvents(this, EVENT_SPELL_DAMAGE_HIT);
		delete this;
		//Go Support
	}
	else
	{
		sEventMgr.RemoveEvents(this, EVENT_SPELL_DAMAGE_HIT);
		delete this;
	}
}*/

void Spell::SendHealSpellOnPlayer(Object* caster, Object* target, uint32 dmg,bool critical, uint32 overheal, uint32 spellid)
{
	if(!caster || !target || !target->IsPlayer())
		return;

	WorldPacket data(SMSG_SPELLHEALLOG, 100);

	data << target->GetNewGUID();
	data << caster->GetNewGUID();
	data << spellid;
	data << uint32(dmg);	// amt healed
	data << uint32(overheal);
	data << uint8(critical);	 //this is critical message

	caster->SendMessageToSet(&data, true);
}

void Spell::SendHealManaSpellOnPlayer(Object * caster, Object * target, uint32 dmg, uint32 powertype, uint32 spellid)
{
	if(!caster || !target || !target->IsPlayer())
		return;

	WorldPacket data(SMSG_SPELLENERGIZELOG, 100);

	data << target->GetNewGUID();
	data << caster->GetNewGUID();
	data << spellid;
	data << powertype;
	data << dmg;

	caster->SendMessageToSet(&data, true);
}

/*void Spell::Heal(int32 amount, bool ForceCrit)
{
	int32 base_amount = amount; //store base_amount for later use

	if(!unitTarget || !unitTarget->isAlive())
		return;
	
	if( p_caster != NULL )
		p_caster->last_heal_spell=m_spellInfo;

    //self healing shouldn't flag himself
	if(p_caster && playerTarget && p_caster != playerTarget)
	{
		// Healing a flagged target will flag you.
		if(playerTarget->IsPvPFlagged())
		{
			p_caster->SetPvPFlag();
			p_caster->SetPVPCombat();
		}
	}

	//Make it critical
	bool critical = false;
	int32 critchance = 0; 
	int32 bonus = 0;
	float healdoneaffectperc = 1.0f;
	if( u_caster != NULL )
	{
		// All calculations are done in getspellbonusdamage
		amount = u_caster->GetSpellBonusDamage(unitTarget, m_spellInfo, amount, false, true); // 3.0.2 Spellpower change: In order to keep the effective amount healed for a given spell the same, weâ€™d expect the original coefficients to be multiplied by 1/0.532 or 1.88.
		// Healing Way fix
 		if(m_spellInfo->NameHash == SPELL_HASH_HEALING_WAVE)
		{
			if(unitTarget->HasActiveAura(29203))
				amount += amount * 18 / 100;
		}
		else if( m_spellInfo->NameHash == SPELL_HASH_HOLY_LIGHT )
		{
			if( unitTarget->HasDummyAura(SPELL_HASH_GLYPH_OF_HOLY_LIGHT) )
			{
				uint32 GHL = float2int32(amount * 0.1f);
				uint32 targetcnt = 0;
				set<Object*>::iterator itr;
				for( itr = unitTarget->GetInRangeSetBegin(); itr != unitTarget->GetInRangeSetEnd(); itr++ )
				{
					if( !(*itr)->IsUnit() || !static_cast<Unit *>(*itr)->isAlive() || isAttackable(u_caster, (*itr), true) )
						continue;

					if( targetcnt > 4 )
						break;

					if(unitTarget->GetDistanceSq((*itr)) <= 64.0f)
					{
						SpellEntry* HLH = dbcSpell.LookupEntryForced( 54968 );
						Spell *pSpell(new Spell(u_caster, HLH, true, NULL));
						pSpell->forced_basepoints[0] = GHL;
						SpellCastTargets tgt;
						tgt.m_unitTarget = (*itr)->GetGUID();
						pSpell->prepare(&tgt);
						targetcnt++;
					}
				}
			}
		}

		//Downranking
		if(p_caster && p_caster->IsPlayer() && m_spellInfo->baseLevel > 0 && m_spellInfo->maxLevel > 0)
		{
			float downrank1 = 1.0f;
			if (m_spellInfo->baseLevel < 20)
				downrank1 = 1.0f - (20.0f - float (m_spellInfo->baseLevel) ) * 0.0375f;
			float downrank2 = ( float(m_spellInfo->maxLevel + 5.0f) / float(p_caster->getLevel()) );
			if (downrank2 >= 1 || downrank2 < 0)
				downrank2 = 1.0f;
			healdoneaffectperc *= downrank1 * downrank2;
		}

		//Spells Not affected by Bonus Healing
		if(m_spellInfo->NameHash == SPELL_HASH_SEAL_OF_LIGHT) //Seal of Light
			healdoneaffectperc = 0.0f;

		if(m_spellInfo->NameHash == SPELL_HASH_LESSER_HEROISM) //Lesser Heroism
			healdoneaffectperc = 0.0f;

		//Spells affected by Bonus Healing
		if(m_spellInfo->NameHash == SPELL_HASH_EARTH_SHIELD) //Earth Shield
			healdoneaffectperc = 0.3f;

		//Basic bonus
		bonus += u_caster->HealDoneMod[m_spellInfo->School];
		bonus += unitTarget->HealTakenMod[m_spellInfo->School];

		//Spell Coefficient
		if(  m_spellInfo->Dspell_coef_override >= 0 ) //In case we have forced coefficients
			bonus = float2int32( float( bonus ) * m_spellInfo->Dspell_coef_override );
		else
		{
			//Bonus to DH part
			if( m_spellInfo->fixed_dddhcoef >= 0 )
				bonus = float2int32( float( bonus ) * m_spellInfo->fixed_dddhcoef );
		}

		critchance = float2int32(u_caster->spellcritperc + u_caster->SpellCritChanceSchool[m_spellInfo->School]);

		int penalty_pct = 0;
		int penalty_flt = 0;
		SM_FIValue( u_caster->SM_PPenalty, &penalty_pct, m_spellInfo );
		bonus += amount*penalty_pct/100;
		SM_FIValue( u_caster->SM_FPenalty, &penalty_flt, m_spellInfo );
		bonus += penalty_flt;
		SM_FIValue( u_caster->SM_CriticalChance, &critchance, m_spellInfo);
				
		amount += float2int32( float( bonus ) * healdoneaffectperc ); //apply downranking on final value ?
		amount += amount*u_caster->HealDonePctMod[m_spellInfo->School]/100;
		amount += float2int32( float( amount ) * unitTarget->HealTakenPctMod[m_spellInfo->School] );

		if (m_spellInfo->SpellGroupType)
			SM_FIValue(u_caster->SM_PDamageBonus, &amount, m_spellInfo);
		
		if( critical = Rand(critchance) || ForceCrit )
		{
			//int32 critbonus = amount >> 1;
			//if( m_spellInfo->SpellGroupType)
			//		SM_PIValue(static_cast<Unit*>(u_caster)->SM_PCriticalDamage, &critbonus, m_spellInfo->SpellGroupType);
			//amount += critbonus;

			int32 critical_bonus = 100;
			if( m_spellInfo->SpellGroupType )
				SM_FIValue( u_caster->SM_PCriticalDamage, &critical_bonus, m_spellInfo );

			if( critical_bonus > 0 )
			{
				// the bonuses are halved by 50% (funky blizzard math :S)
				float b = ( ( float(critical_bonus) / 2.0f ) / 100.0f );
				amount += float2int32( float(amount) * b );
			}

			//Shady: does it correct> caster casts heal and proc ..._VICTIM ? 
			// Or mb i'm completely wrong? So if true  - just replace with old string. 
			//u_caster->HandleProc(PROC_ON_SPELL_CRIT_HIT_VICTIM, unitTarget, m_spellInfo, amount);
			//Replaced with following one:

			unitTarget->HandleProc(PROC_ON_CRIT_ATTACK_VICTIM | PROC_ON_HEALSPELL_LAND_VICTIM, u_caster, m_spellInfo, amount);
			u_caster->HandleProc(PROC_ON_CRIT_ATTACK | PROC_ON_HEALSPELL_LAND, unitTarget, m_spellInfo, amount);
		}
		
	}

	if(amount < 0) 
		amount = 0;

	SendHealSpellOnPlayer(m_caster, unitTarget, amount, critical);

	if( p_caster != NULL )  
	{
		p_caster->m_bgScore.HealingDone += amount;
		if( p_caster->m_bg != NULL )
			p_caster->m_bg->UpdatePvPData();
	}
	uint32 curHealth = unitTarget->GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 maxHealth = unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	if((curHealth + amount) >= maxHealth)
		unitTarget->SetUInt32Value(UNIT_FIELD_HEALTH, maxHealth);
	else
		unitTarget->ModUnsigned32Value(UNIT_FIELD_HEALTH, amount);


	//send the difference to linked units
	std::set<Unit*>::iterator itr = unitTarget->m_sharedHPUnits.begin();
	std::set<Unit*>::iterator itrend = unitTarget->m_sharedHPUnits.end();

	//were not the master, try and get the master
	if (unitTarget->m_sharedHPUnits.size() == 0 && unitTarget->m_sharedHPMaster != NULL && unitTarget->m_sharedHPMaster->event_GetInstanceID() == unitTarget->event_GetInstanceID())
	{
			itr = unitTarget->m_sharedHPMaster->m_sharedHPUnits.begin();
			itrend = unitTarget->m_sharedHPMaster->m_sharedHPUnits.end();
	}

	//do this with the other guys in the hp share :P
	for (; itr != itrend; ++itr)
	{
		if ((*itr) == NULL || !(*itr)->isAlive() || (*itr) == unitTarget)
			continue;

		if(((*itr)->GetUInt32Value(UNIT_FIELD_HEALTH) + amount) >= (*itr)->GetUInt32Value(UNIT_FIELD_MAXHEALTH))
			(*itr)->SetUInt32Value( UNIT_FIELD_HEALTH, (*itr)->GetUInt32Value(UNIT_FIELD_MAXHEALTH) );
		else
			(*itr)->ModUnsigned32Value(UNIT_FIELD_HEALTH, amount);
	}

	if (p_caster)
	{
		p_caster->m_casted_amount[m_spellInfo->School]=amount;
		//p_caster->HandleProc( PROC_ON_CAST_SPECIFIC_SPELL | PROC_ON_CAST_SPELL, unitTarget, m_spellInfo );
		p_caster->HandleProc( PROC_ON_HEALSPELL_LAND | PROC_ON_CAST_SPELL, unitTarget, m_spellInfo );
	}

	int doneTarget = 0;

	// add threat
	if( u_caster != NULL )
	{
		//preventing overheal ;)
		if( (curHealth + base_amount) >= maxHealth )
			base_amount = maxHealth - curHealth;

		uint32 base_threat=GetBaseThreat(base_amount);
		int count = 0;
		Unit *unit;
		std::vector<Unit*> target_threat;
		if(base_threat)
		{
			// 
			http://www.wowwiki.com/Threat
			Healing threat is global, and is normally .5x of the amount healed.
			Healing effects cause no threat if the target is already at full health.

			Example: Player 1 is involved in combat with 5 mobs. Player 2 (priest) heals Player 1 for 1000 health,
			and has no threat reduction talents. A 1000 heal generates 500 threat,
			however that 500 threat is split amongst the 5 mobs.
			Each of the 5 mobs now has 100 threat towards Player 2.
			//

			target_threat.reserve(u_caster->GetInRangeCount()); // this helps speed

			for(std::set<Object*>::iterator itr = u_caster->GetInRangeSetBegin(); itr != u_caster->GetInRangeSetEnd(); ++itr)
			{
				if((*itr)->GetTypeId() != TYPEID_UNIT)
					continue;
				unit = static_cast<Unit*>((*itr));
				if(unit->GetAIInterface()->GetNextTarget() == unitTarget)
				{
					target_threat.push_back(unit);
					++count;
				}
			}
			count = ( count == 0 ? 1 : count );  // division against 0 protection

			// every unit on threatlist should get 1/2 the threat, divided by size of list
			uint32 threat = base_threat / (count * 2);

			// update threatlist (HealReaction)
			for(std::vector<Unit*>::iterator itr = target_threat.begin(); itr != target_threat.end(); ++itr)
			{
				// for now we'll just use heal amount as threat.. we'll prolly need a formula though
				static_cast< Unit* >( *itr )->GetAIInterface()->HealReaction( u_caster, unitTarget, threat );

				if( (*itr)->GetGUID() == u_caster->CombatStatus.GetPrimaryAttackTarget() )
					doneTarget = 1;
			}
		}
		// remember that we healed (for combat status)
		if(unitTarget->IsInWorld() && u_caster->IsInWorld())
			u_caster->CombatStatus.WeHealed(unitTarget);
	}
}*/
void Spell::Heal(int32 amount, bool ForceCrit)
{
	if( !unitTarget || !unitTarget->isAlive() )
		return;
	
	if( m_caster->IsPlayer() )
		((Player *)m_caster)->last_heal_spell=m_spellInfo;

    //self healing shouldn't flag himself
	if(m_caster->IsPlayer() && playerTarget && (((Player *)m_caster) != playerTarget) )
	{
		// Healing a flagged target will flag you.
		if(playerTarget->IsPvPFlagged())
			((Player *)m_caster)->SetPvPFlag();
	}

	//Make it critical
	bool critical = false;
	float critchance = 0; 
	int32 bonus = 0;
	if( m_caster->IsUnit() )
	{		
		// All calculations are done in getspellbonusdamage
		amount = ((Unit *)m_caster)->GetSpellDmgBonus(unitTarget, m_spellInfo, amount, false, true); // 3.0.2 Spellpower change: In order to keep the effective amount healed for a given spell the same, weâ€™d expect the original coefficients to be multiplied by 1/0.532 or 1.88.

		// Healing Way fix
 		if(m_spellInfo->NameHash == SPELL_HASH_HEALING_WAVE)
		{
			if(unitTarget->HasActiveAura(29203))
				amount += amount * 18 / 100;
		}
		else if( m_spellInfo->NameHash == SPELL_HASH_HOLY_LIGHT )
		{
			if( unitTarget->HasDummyAura(SPELL_HASH_GLYPH_OF_HOLY_LIGHT) )
			{
				uint32 GHL = float2int32(amount * 0.1f);
				uint32 targetcnt = 0;
				std::set<Object*>::iterator itr;
				for( itr = unitTarget->GetInRangeSetBegin(); itr != unitTarget->GetInRangeSetEnd(); itr++ )
				{
					if( !(*itr)->IsUnit() || !static_cast<Unit *>(*itr)->isAlive() || isAttackable(m_caster, (*itr), true) )
						continue;

					if( targetcnt > 4 )
						break;

					if(unitTarget->GetDistanceSq((*itr)) <= 64.0f)
					{
						SpellEntry* HLH = dbcSpell.LookupEntryForced( 54968 );
						Spell *pSpell = new Spell(((Unit *)m_caster), HLH, true, NULL);
						pSpell->forced_basepoints[0] = GHL;
						SpellCastTargets tgt;
						tgt.m_target = (*itr); // tgt.m_unitTarget = (*itr)->GetGUID();
						pSpell->prepare(&tgt);
						targetcnt++;
					}
				}
			}
		}

		if(m_spellInfo->spell_can_crit)
		{
			critchance = ((Unit *)m_caster)->spellcritperc + ((Unit *)m_caster)->SpellCritChanceSchool[m_spellInfo->School];
			if(m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2] )
			{
				SM_FFValue(((Unit *)m_caster)->SM[SMT_CRITICAL][0], &critchance, m_spellInfo);
				SM_PFValue(((Unit *)m_caster)->SM[SMT_CRITICAL][1], &critchance, m_spellInfo);
			}

			// Sacred Shield HOAX
			if( unitTarget->HasDummyAura(SPELL_HASH_SACRED_SHIELD) )
				critchance += 50.0f;
		}
		if(critical = Rand(critchance))
		{
			/*int32 critbonus = amount >> 1;
			if( pSpell->SpellGroupType[0] || pSpell->SpellGroupType[1] || pSpell->SpellGroupType[2])
					SM_PIValue(TO_UNIT(u_caster)->SM[SMT_CRITICAL_DAMAGE][1], &critbonus, m_spellInfo->SpellGroupType);
			amount += critbonus;*/

			int32 critical_bonus = 100;
			if( m_spellInfo->SpellGroupType[0] || m_spellInfo->SpellGroupType[1] || m_spellInfo->SpellGroupType[2] )
			{
				SM_FIValue( ((Unit *)m_caster)->SM[SMT_CRITICAL_DAMAGE][1], &critical_bonus, m_spellInfo );
			}

			if( critical_bonus > 0 )
			{
				// the bonuses are halved by 50% (funky blizzard math :S)
				float b = ( ( float(critical_bonus) / 2.0f ) / 100.0f );
				amount += float2int32( float(amount) * b );
			}

			if( ((Unit *)m_caster)->HasDummyAura(SPELL_HASH_LIVING_SEED) &&
				( m_spellInfo->NameHash == SPELL_HASH_SWIFTMEND ||
				m_spellInfo->NameHash == SPELL_HASH_REGROWTH ||
				m_spellInfo->NameHash == SPELL_HASH_NOURISH ||
				m_spellInfo->NameHash == SPELL_HASH_HEALING_TOUCH) )
			{
				uint32 chance = ( ((Unit *)m_caster)->GetDummyAura(SPELL_HASH_LIVING_SEED)->RankNumber * 33 ) + 1;
				if( Rand( chance ) )
				{
					SpellEntry *spellInfo = dbcSpell.LookupEntry( 48504 );
					Spell *sp = new Spell( m_caster, spellInfo, true, NULL);
					sp->forced_basepoints[0] = float2int32(amount * 0.3f);
					SpellCastTargets tgt;
					tgt.m_target = unitTarget; // tgt.m_unitTarget = unitTarget->GetGUID();
					sp->prepare(&tgt);
				}
			}

			if( playerTarget && ((Unit *)m_caster)->HasDummyAura(SPELL_HASH_DIVINE_AEGIS) )
			{
				SpellEntry * spellInfo = dbcSpell.LookupEntry( 47753 );
				Spell *sp = new Spell( m_caster, spellInfo, true, NULL );
				sp->forced_basepoints[0] = float2int32(amount * ( 0.1f * ((Unit *)m_caster)->GetDummyAura(SPELL_HASH_DIVINE_AEGIS)->RankNumber ));
				SpellCastTargets tgt;
				tgt.m_target = playerTarget; // tgt.m_unitTarget = playerTarget->GetGUID();
				sp->prepare(&tgt);
			}

			if( ((Unit *)m_caster)->HasDummyAura(SPELL_HASH_SHEATH_OF_LIGHT) && unitTarget )
			{
				SpellEntry * spellInfo = dbcSpell.LookupEntry( 54203 );
				Spell *sp = new Spell( m_caster, spellInfo, true, NULL );
				sp->forced_basepoints[0] = float2int32(amount * ( 0.05f * ((Unit *)m_caster)->GetDummyAura(SPELL_HASH_SHEATH_OF_LIGHT)->RankNumber ));
				SpellCastTargets tgt;
				tgt.m_target = unitTarget; // tgt.m_unitTarget = unitTarget->GetGUID();
				sp->prepare(&tgt);
			}
			unitTarget->HandleProc(PROC_ON_HEALSPELL_LAND_VICTIM, ((Unit *)m_caster), m_spellInfo, amount);
			((Unit *)m_caster)->HandleProc(PROC_ON_HEALSPELL_LAND, unitTarget, m_spellInfo, amount);
		}

		if( unitTarget && (m_spellInfo->NameHash == SPELL_HASH_GREATER_HEAL ||
			m_spellInfo->NameHash == SPELL_HASH_FLASH_HEAL ||
			m_spellInfo->NameHash == SPELL_HASH_PENANCE ) &&
			((Unit *)m_caster)->HasDummyAura( SPELL_HASH_RAPTURE ) && 
			(((Unit *)m_caster)->m_CustomTimers[CUSTOM_TIMER_RAPTURE] <= getMSTime()) )
		{
			SpellEntry *spellInfo = dbcSpell.LookupEntry( 47755 );
			Spell *sp = new Spell( ((Unit *)m_caster), spellInfo, true, NULL );
			uint32 maxmana = ((Unit *)m_caster)->GetUInt32Value(UNIT_FIELD_MAXPOWER1);
			float rapture_mod = ((Unit *)m_caster)->GetDummyAura( SPELL_HASH_RAPTURE )->RankNumber * 0.005f;
			sp->forced_basepoints[0] = float2int32( maxmana * rapture_mod );
			SpellCastTargets tgt;
			tgt.m_target = unitTarget; // tgt.m_unitTarget = unitTarget->GetGUID();
			sp->prepare(&tgt);
			((Unit *)m_caster)->m_CustomTimers[CUSTOM_TIMER_RAPTURE] = getMSTime() + 12000;
		}
	}

	if(amount < 0) 
		amount = 0;

	uint32 overheal = 0;
	uint32 curHealth = unitTarget->GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 maxHealth = unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	if((curHealth + amount) >= maxHealth)
	{
		unitTarget->SetUInt32Value(UNIT_FIELD_HEALTH, maxHealth);
		overheal = curHealth + amount - maxHealth;
	} else
		unitTarget->ModUnsigned32Value(UNIT_FIELD_HEALTH, amount);

	if( overheal && m_caster->IsUnit() && ((Unit *)m_caster)->HasDummyAura(SPELL_HASH_SERENDIPITY) && ((Unit *)m_caster)->m_LastSpellManaCost &&
		( m_spellInfo->NameHash == SPELL_HASH_GREATER_HEAL || m_spellInfo->NameHash == SPELL_HASH_FLASH_HEAL ) )
	{
		int32 amt = float2int32( ((Unit *)m_caster)->m_LastSpellManaCost * ( 0.08f * ((Unit *)m_caster)->GetDummyAura(SPELL_HASH_SERENDIPITY)->RankNumber ));
		SpellEntry* SpellInfo = dbcSpell.LookupEntry( 47762 );
		if( SpellInfo )
		{
			Spell *sp = new Spell( ((Unit *)m_caster), SpellInfo, true, NULL );
			sp->forced_basepoints[0] = amt;
			SpellCastTargets tgt;
			tgt.m_target = m_caster; // tgt.m_unitTarget = u_caster->GetGUID();
			sp->prepare( &tgt );
		}
	}
	if( m_caster->IsUnit() && unitTarget && (((Unit *)m_caster) != unitTarget) && unitTarget->HasDummyAura(SPELL_HASH_SPIRITUAL_ATTUNEMENT) && amount)
	{
		int32 amt = float2int32( amount * ( (unitTarget->GetDummyAura(SPELL_HASH_SPIRITUAL_ATTUNEMENT)->EffectBasePoints[0]+1) / 100.0f ));
		SpellEntry* SpellInfo = dbcSpell.LookupEntry( 31786 );
		if( SpellInfo )
		{
			Spell *sp = new Spell( ((Unit *)m_caster), SpellInfo, true, NULL );
			sp->forced_basepoints[0] = amt;
			SpellCastTargets tgt;
			tgt.m_target = unitTarget; // tgt.m_unitTarget = unitTarget->GetGUID();
			sp->prepare( &tgt );
		}
	}

	if( m_caster )
	{
		SendHealSpellOnPlayer( m_caster, unitTarget, amount, critical, overheal, m_spellInfo->logsId ? m_spellInfo->logsId : (pSpellId ? pSpellId : m_spellInfo->Id) );
	}
	if( m_caster->IsPlayer() )  
	{
		((Player *)m_caster)->m_bgScore.HealingDone += amount - overheal;
		if( ((Player *)m_caster)->m_bg != NULL )
			((Player *)m_caster)->m_bg->UpdatePvPData();
	}

	//Beacon of Light
	if( m_spellInfo->Id != 53652 && unitTarget->BeaconCaster && unitTarget->BeaconTarget && m_caster->IsUnit() && (((Unit *)m_caster) == unitTarget->BeaconCaster) )
	{
		SpellEntry *spellInfo = dbcSpell.LookupEntry( 53652 );
		Spell *sp = new Spell( unitTarget->BeaconCaster, spellInfo, true, NULL );
		sp->forced_basepoints[0] = amount;
		SpellCastTargets tgt;
		tgt.m_target = unitTarget->BeaconTarget; // tgt.m_unitTarget = unitTarget->BeaconTarget->GetGUID();
		sp->prepare(&tgt);
	}

	if (m_caster->IsPlayer())
	{
		((Player *)m_caster)->m_casted_amount[m_spellInfo->School]=amount;
		//p_caster->HandleProc( PROC_ON_CAST_SPECIFIC_SPELL | PROC_ON_CAST_SPELL, unitTarget, m_spellInfo );
	}

	// add threat
	if( m_caster->IsUnit() )
	{
		uint32 base_threat=Spell::GetBaseThreat(amount);
		int count = 0;
		Unit *unit;
		std::vector<Unit *> target_threat;
		if(base_threat)
		{
			target_threat.reserve(((Unit *)m_caster)->GetInRangeCount()); // this helps speed

			for(std::set<Object *>::iterator itr = ((Unit *)m_caster)->GetInRangeSetBegin(); itr != ((Unit *)m_caster)->GetInRangeSetEnd(); ++itr)
			{
				if((*itr)->GetTypeId() != TYPEID_UNIT)
					continue;
				unit = static_cast<Unit *>(*itr);
				if(unit->GetAIInterface()->GetNextTarget() == unitTarget)
				{
					target_threat.push_back(unit);
					++count;
				}
			}
			if(count == 0)
				count = 1;  // division against 0 protection
			/* 
			When a tank hold multiple mobs, the threat of a heal on the tank will be split between all the mobs.
			The exact formula is not yet known, but it is more than the Threat/number of mobs.
			So if a tank holds 5 mobs and receives a heal, the threat on each mob will be less than Threat(heal)/5.
			Current speculation is Threat(heal)/(num of mobs *2)
			*/
			uint32 threat = base_threat / (count * 2);
				
			for(std::vector<Unit *>::iterator itr = target_threat.begin(); itr != target_threat.end(); ++itr)
			{
				// for now we'll just use heal amount as threat.. we'll prolly need a formula though
				static_cast<Unit *>(*itr)->GetAIInterface()->HealReaction( ((Unit *)m_caster), unitTarget, threat/*, m_spellInfo*/ );
			}
		}

		if(unitTarget->IsInWorld() && m_caster->IsInWorld())
			((Unit *)m_caster)->CombatStatus.WeHealed(unitTarget);
	}   
}

void Spell::DetermineSkillUp(uint32 skillid,uint32 targetlevel)
{
	if(!m_caster->IsPlayer()) return;

	if(((Player *)m_caster)->GetSkillUpChance(skillid)<0.01)return;//to preven getting higher skill than max

	int32 diff=((Player *)m_caster)->_GetSkillLineCurrent(skillid,false)/5-targetlevel;
	if(diff<0)diff=-diff;
	float chance;
	if(diff<=5)chance=95.0;
	else if(diff<=10)chance=66;
	else if(diff <=15)chance=33;
	else return;
	if(Rand(chance*sWorld.getRate(RATE_SKILLCHANCE)))
		((Player *)m_caster)->_AdvanceSkillLine(skillid, float2int32( 1.0f * sWorld.getRate(RATE_SKILLRATE)));

	uint32 value = ((Player *)m_caster)->_GetSkillLineCurrent( skillid, true );
	uint32 spellid = 0;

	// Lifeblood
		if( skillid == SKILL_HERBALISM )
		{
			switch(value)
			{
				case 75:{	spellid = 55428; }break;// Rank 1
				case 150:{	spellid = 55480; }break;// Rank 2
				case 225:{	spellid = 55500; }break;// Rank 3
				case 300:{	spellid = 55501; }break;// Rank 4
				case 375:{	spellid = 55502; }break;// Rank 5
				case 450:{	spellid = 55503; }break;// Rank 6
			}
		}

		// Toughness
		else if( skillid == SKILL_MINING )
		{
			switch( value )
			{
				case 75:{	spellid = 53120; }break;// Rank 1
				case 150:{	spellid = 53121; }break;// Rank 2
				case 225:{	spellid = 53122; }break;// Rank 3
				case 300:{	spellid = 53123; }break;// Rank 4
				case 375:{	spellid = 53124; }break;// Rank 5
				case 450:{	spellid = 53040; }break;// Rank 6
			}
		}


		// Master of Anatomy
		else if( skillid == SKILL_SKINNING )
		{
			switch( value )
			{
				case 75:{	spellid = 53125;} break;// Rank 1
				case 150:{	spellid = 53662;} break;// Rank 2
				case 225:{	spellid = 53663;} break;// Rank 3
				case 300:{	spellid = 53664;} break;// Rank 4
				case 375:{	spellid = 53665;} break;// Rank 5
				case 450:{	spellid = 53666;} break;// Rank 6
			}
		}
		if( spellid != 0 )
			((Player *)m_caster)->addSpell( spellid );
}

void Spell::DetermineSkillUp(uint32 skillid)
{
	//This code is wrong for creating items and disenchanting. 
	if(!m_caster->IsPlayer()) return;

	float chance = 0.0f;
	skilllinespell* skill = objmgr.GetSpellSkill(m_spellInfo->Id);
	if( skill != NULL && static_cast< Player* >( m_caster )->_HasSkillLine( skill->skilline ) )
	{
		uint32 amt = static_cast< Player* >( m_caster )->_GetSkillLineCurrent( skill->skilline, false );
		uint32 max = static_cast< Player* >( m_caster )->_GetSkillLineMax( skill->skilline );
		if( amt >= max )
			return;
		if( amt >= skill->trivialSkillLineRankHigh ) //grey
			chance = 0.0f;
		else if( ( amt >= ( ( ( skill->trivialSkillLineRankHigh - skill->trivialSkillLineRankLow) / 2 ) + skill->trivialSkillLineRankLow ) ) ) //green
			chance = 33.0f;
		else if( amt >= skill->trivialSkillLineRankLow ) //yellow
			chance = 66.0f;
		else //brown
			chance=100.0f;
	}
	if(Rand(chance*sWorld.getRate(RATE_SKILLCHANCE)))
		((Player *)m_caster)->_AdvanceSkillLine(skillid, float2int32( 1.0f * sWorld.getRate(RATE_SKILLRATE)));
}

bool Spell::Reflect(Unit *refunit)
{
	uint32 refspellid = 0;
	bool canreflect = false;
	bool remove = false;

	SpellEntry * refspell = NULL;

	if( m_reflectedParent != NULL || m_caster == refunit)
		return false;

	// if the spell to reflect is a reflect spell, do nothing.
	for(int i=0; i<3; i++)
    {
		//if (GetTargetType(m_spellInfo->EffectImplicitTargetA[i], i) & (SPELL_TARGET_AOE | SPELL_TARGET_AOE_SELF | SPELL_TARGET_AOE_CURRENT_TARGET | SPELL_TARGET_CONE | SPELL_TARGET_SELF | SPELL_TARGET_CHAINED_OBJECT))
		if (GetTargetType(m_spellInfo->EffectImplicitTargetA[i], i) & (SPELL_TARGET_AREA | SPELL_TARGET_AREA_SELF | SPELL_TARGET_AREA_CURTARGET | SPELL_TARGET_AREA_CONE | SPELL_TARGET_OBJECT_SELF | SPELL_TARGET_AREA_CHAIN))
			return false;

		//if (GetTargetType(m_spellInfo->EffectImplicitTargetB[i], i) & (SPELL_TARGET_AOE | SPELL_TARGET_AOE_SELF | SPELL_TARGET_AOE_CURRENT_TARGET | SPELL_TARGET_CONE | SPELL_TARGET_SELF | SPELL_TARGET_CHAINED_OBJECT))
		if (GetTargetType(m_spellInfo->EffectImplicitTargetB[i], i) & (SPELL_TARGET_AREA | SPELL_TARGET_AREA_SELF | SPELL_TARGET_AREA_CURTARGET | SPELL_TARGET_AREA_CONE | SPELL_TARGET_OBJECT_SELF | SPELL_TARGET_AREA_CHAIN))
			return false;

		if(  m_spellInfo->Effect[i] == SPELL_AURA_MOD_CHARM && 
			(   m_spellInfo->EffectApplyAuraName[i] == SPELL_AURA_REFLECT_SPELLS_SCHOOL 
			 || m_spellInfo->EffectApplyAuraName[i] == SPELL_AURA_REFLECT_SPELLS) 
		   )
			return false;
    }
	for(std::list<struct ReflectSpellSchool*>::iterator i = refunit->m_reflectSpellSchool.begin();i != refunit->m_reflectSpellSchool.end();i++)
	{
		if((*i)->school & m_spellInfo->schoolMask)
		{
			if(Rand((float)(*i)->chance))
			{
				//the god blessed special case : mage - Frost Warding = is an augmentation to frost warding
				if((*i)->require_aura_hash && m_caster->IsUnit() && !((Unit *)m_caster)->HasAurasWithNameHash((*i)->require_aura_hash))
                {
					if( !(m_spellInfo->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
						continue;

					int32 evilforce = 0;
					uint32 effectid = 0;
					for( uint32 loopnr = 0; loopnr < 3; loopnr++ )
					{
						if( m_spellInfo->EffectBasePoints[loopnr] > evilforce )
						{
							evilforce = m_spellInfo->EffectBasePoints[loopnr];
							effectid = loopnr;
						}
					}

					SpellEntry *spellInfo = dbcSpell.LookupEntry( 57776 );
					Spell *spell(new Spell( refunit, spellInfo, true, NULL));
					uint32 manaregenamt = CalculateEffect(effectid, refunit);
					spell->forced_basepoints[0] = manaregenamt;
					SpellCastTargets targets;
					targets.m_target = refunit; // targets.m_unitTarget = refunit->GetGUID();
					spell->prepare( &targets );
                }
				else if( (*i)->infront )
				{
					if( m_caster->isInFront(refunit) )
					{
						canreflect = true;
					}
				}
				else
					canreflect = true;

				refspellid = (*i)->spellId;
				if( !(*i)->infinity )
					refunit->RemoveAura(refspellid);

				break;
			}
		}
	}

	if(!refspell || m_caster == refunit) return false;

	Spell *spell = new Spell(refunit, m_spellInfo, true, NULL);
	SpellCastTargets targets;
	targets.m_target = m_caster; // targets.m_unitTarget = m_caster->GetGUID();
	spell->m_reflectedParent = this;
	spell->prepare(&targets);
	return true;
}

void ApplyDiminishingReturnTimer(int32 * Duration, Unit * Target, SpellEntry * spell)
{
	uint32 status = GetDiminishingGroup(spell->NameHash);
	uint32 Grp = status & 0xFFFF;   // other bytes are if apply to pvp
	uint32 PvE = (status >> 16) & 0xFFFF;

	// Make sure we have a group
	if(Grp == 0xFFFF) return;

	// Check if we don't apply to pve
	if(!PvE && Target->GetTypeId() != TYPEID_PLAYER && !Target->IsPet())
		return;

	// TODO: check for spells that should do this
	float Dur = float(*Duration);

	switch(Target->m_diminishCount[Grp])
	{
	case 0: // Full effect
		if ( ( Target->IsPlayer() || Target->IsPet() ) && Dur > 10000)
		{
			Dur = 10000;
		}
		break;
		
	case 1: // Reduced by 50%
		Dur *= 0.5f;
		if ( ( Target->IsPlayer() || Target->IsPet() ) && Dur > 5000)
		{
			Dur = 5000;
		}
		break;

	case 2: // Reduced by 75%
		Dur *= 0.25f;
		if ( ( Target->IsPlayer() || Target->IsPet() ) && Dur > 2500)
		{
			Dur = 2500;
		}
		break;

	default:// Target immune to spell
		{
			*Duration = 0;
			return;
		}break;
	}

	// Convert back
	*Duration = ((int32)Dur);

	// Reset the diminishing return counter, and add to the aura count (we don't decrease the timer till we
	// have no auras of this type left.
	++Target->m_diminishAuraCount[Grp];
	++Target->m_diminishCount[Grp];
}

void UnapplyDiminishingReturnTimer(Unit * Target, SpellEntry * spell)
{
	uint32 status = GetDiminishingGroup(spell->NameHash);
	uint32 Grp = status & 0xFFFF;   // other bytes are if apply to pvp
	uint32 PvE = (status >> 16) & 0xFFFF;

	// Make sure we have a group
	if(Grp == 0xFFFF) return;

	// Check if we don't apply to pve
	if(!PvE && Target->GetTypeId() != TYPEID_PLAYER && !Target->IsPet())
		return;

	Target->m_diminishAuraCount[Grp]--;

	// start timer decrease
	if(!Target->m_diminishAuraCount[Grp])
	{
		Target->m_diminishActive = true;
		Target->m_diminishTimer[Grp] = 15000;
	}
}

/// Calculate the Diminishing Group. This is based on a name hash.
/// this off course is very hacky, but as its made done in a proper way
/// I leave it here.
uint32 GetDiminishingGroup(uint32 NameHash)
{
	int32 grp = -1;
	bool pve = false;

	switch(NameHash)
	{
	case SPELL_HASH_SAP:					// Sap
	case SPELL_HASH_GOUGE:					// Gouge
		grp = 0;
		break;

	case SPELL_HASH_CHEAP_SHOT:				// Cheap Shot
		{
			grp = 1;
			pve = true;
		}break;

	case SPELL_HASH_KIDNEY_SHOT:			// Kidney Shot
		{
			grp = 2;
			pve = true;
		}break;

	case SPELL_HASH_BASH:					// Bash
		grp = 3;
		break;

	case SPELL_HASH_ENTANGLING_ROOTS:		// Entangling Roots
		grp = 4;
		break;

	case SPELL_HASH_HAMMER_OF_JUSTICE:		// Hammer of Justice
		{
			grp = 5;
			pve = true;
		}break;

	case SPELL_HASH_STUN:					// Stuns (all of them)
		grp = 6;
		break;

	case SPELL_HASH_CHARGE:					// Charge
	case SPELL_HASH_INTERCEPT :				// Intercept
	case SPELL_HASH_CONCUSSION_BLOW:		// Concussion Blow
		{
			grp = 7;
			pve = true;
		}break;

	case SPELL_HASH_FEAR:					// Fear
	case SPELL_HASH_SEDUCTION:				// Seduction
	case SPELL_HASH_HOWL_OF_TERROR:			// Howl of Terror
		grp = 8;
		break;

	case SPELL_HASH_FROST_NOVA:				// Frost Nova
		grp = 9;
		break;

	case SPELL_HASH_POLYMORPH:				// Polymorph
	case SPELL_HASH_POLYMORPH__CHICKEN:		// Chicken
//	case SPELL_HASH_POLYMORPH__PIG:			// Pig    // disparu 3.0.9
//	case SPELL_HASH_POLYMORPH__TURTLE:		// Turtle // disparu 3.0.9
	case SPELL_HASH_POLYMORPH__SHEEP:		// Good ol' sheep
	case SPELL_HASH_POLYMORPH___PENGUIN:    // éhéh en pinguoin 
		{
			grp = 10;
			pve = true;   
		}break;

	case SPELL_HASH_PSYCHIC_SCREAM:			// Psychic Scream
		grp = 11;
		break;

	case SPELL_HASH_MIND_CONTROL:			// Mind Control
		grp = 12;
		break;

	//case SPELL_HASH_FROST_SHOCK:			// Frost Shock
		//grp = 13;
		//break;

	case SPELL_HASH_HIBERNATE:				// Hibernate
		grp = 14;
		break;

	case SPELL_HASH_CYCLONE:				// Cyclone
	case SPELL_HASH_BLIND:					// Blind
		{
			grp = 15;
			pve = true;
		}break;

	case SPELL_HASH_CELESTIAL_FOCUS:		// Celestial Focus
		{
			grp = 16;
			pve = true;
		}break;

	case SPELL_HASH_IMPACT:					// Impact
		{
			grp = 17;
			pve = true;
		}break;

	case SPELL_HASH_BLACKOUT:				// Blackout
		{
			grp = 18;
			pve = true;
		}break;

	case SPELL_HASH_BANISH:					// Banish
		grp = 19;
		break;

	case SPELL_HASH_FREEZING_TRAP_EFFECT:	// Freezing Trap Effect
		grp = 20;
		break;

	case SPELL_HASH_SCARE_BEAST:			// Scare Beast
		grp = 21;
		break;

	case SPELL_HASH_ENSLAVE_DEMON:			// Enslave Demon
		grp = 22;
		break;
	case SPELL_HASH_SLEEP:					// Sleep
	case SPELL_HASH_RECKLESS_CHARGE:		// Reckless Charge
		grp = 23;
		break;
	case SPELL_HASH_RIPOSTE:
		grp = 24;
		break;
	}
	uint32 ret;
	if( pve )
		ret = grp | (1 << 16);
	else
		ret = grp;

	return ret;
}

void Spell::SendCastSuccess(Object * target)
{
/*	
	Player * plr = p_caster;
	if(!plr && u_caster)
		plr = u_caster->m_redirectSpellPackets;
	if(!plr||!plr->IsPlayer())
		return;

	WorldPacket data(SMSG_CLEAR_EXTRA_AURA_INFO_OBSOLETE, 13);
	data << ((target != 0) ? target->GetNewGUID() : uint8(0));
	data << m_spellInfo->Id;
	
	plr->GetSession()->SendPacket(&data);
*/	
}

void Spell::SendCastSuccess(const uint64& guid)
{ /* OBSOLETE (Branruz)
	Player * plr = p_caster;
	if(!plr && u_caster)
		plr = u_caster->m_redirectSpellPackets;
	if(!plr || !plr->IsPlayer())
		return;
    */

    if(!m_caster->IsPlayer()) return;
    Player * plr = ((Player *)m_caster)->m_redirectSpellPackets;

	// fuck bytebuffers
	unsigned char buffer[13];
	uint32 c = FastGUIDPack(guid, buffer, 0);
#ifdef USING_BIG_ENDIAN
	*(uint32*)&buffer[c] = swap32(m_spellInfo->Id);         c += 4;
#else
	*(uint32*)&buffer[c] = m_spellInfo->Id;                 c += 4;
#endif

	//plr->GetSession()->OutPacket(SMSG_CLEAR_EXTRA_AURA_INFO_OBSOLETE, c, buffer);
}
/*
bool IsBeneficSpell(SpellEntry *sp)
{
	uint32 cur;
	for(uint32 i=0;i<3;i++)
		for(uint32 j=0;j<2;j++)
		{
			if(j==0)
				cur = sp->EffectImplicitTargetA[i];
			else // if(j==1)
				cur = sp->EffectImplicitTargetB[i];		
			switch(cur)
			{
				case EFF_TARGET_SELF:
				case EFF_TARGET_PET:
				case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
				case EFF_TARGET_SINGLE_FRIEND:
				case 45:// Chain,!!only for healing!! for chain lightning =6 
				case 57:// Targeted Party Member
				case 27: // target is owner of pet
				case EFF_TARGET_MINION:// Minion Target
				case 33://Party members of totem, inside given range
				case EFF_TARGET_SINGLE_PARTY:// Single Target Party Member
				case EFF_TARGET_ALL_PARTY: // all Members of the targets party
				case EFF_TARGET_SELF_FISHING://Fishing
				case 46://Unknown Summon Atal'ai Skeleton
				case 47:// Portal
				case 52:	// Lightwells, etc
				case 40://Activate Object target(probably based on focus)
				case EFF_TARGET_TOTEM_EARTH:
				case EFF_TARGET_TOTEM_WATER:
				case EFF_TARGET_TOTEM_AIR:
				case EFF_TARGET_TOTEM_FIRE:// Totem
				case 61: // targets with the same group/raid and the same class
				case 32:
				case 73:
					return true;
			}//end switch
		}//end for
	return false;
}

AI_SpellTargetType RecommandAISpellTargetType(SpellEntry *sp)
{
	uint32 cur;
	for(uint32 i=0;i<3;i++)
		for(uint32 j=0;j<2;j++)
		{
			if(j==0)
				cur = sp->EffectImplicitTargetA[i];
			else // if(j==1)
				cur = sp->EffectImplicitTargetB[i];		
			switch(cur)
			{
				case EFF_TARGET_NONE:
				case EFF_TARGET_GAMEOBJECT:
				case EFF_TARGET_GAMEOBJECT_ITEM:// Gameobject/Item Target
				case EFF_TARGET_SELF_FISHING://Fishing
				case 47:// Portal
				case 52:	// Lightwells, etc
				case 40://Activate Object target(probably based on focus)
					return TTYPE_NULL;

				case EFF_TARGET_SELF:
				case 38://Dummy Target
				case 32:
				case 73:
					return TTYPE_CASTER;

				case EFF_TARGET_ALL_ENEMY_IN_AREA: // All Enemies in Area of Effect (TEST)
				case EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT: // All Enemies in Area of Effect instant (e.g. Flamestrike)
				case EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:// All Enemies in Area of Effect(Blizzard/Rain of Fire/volley) channeled
					return TTYPE_DESTINATION;

				case 4: // dono related to "Wandering Plague", "Spirit Steal", "Contagion of Rot", "Retching Plague" and "Copy of Wandering Plague"
				case EFF_TARGET_PET:
				case EFF_TARGET_SINGLE_ENEMY:// Single Target Enemy
				case 77:					// grep: i think this fits
				case 8: // related to Chess Move (DND), Firecrackers, Spotlight, aedm, Spice Mortar
				case EFF_TARGET_IN_FRONT_OF_CASTER:
				case 31:// related to scripted effects
				case 53:// Target Area by Players CurrentSelection()
				case 54:// Targets in Front of the Caster
				case EFF_TARGET_ALL_PARTY_AROUND_CASTER:
				case EFF_TARGET_SINGLE_FRIEND:
				case 45:// Chain,!!only for healing!! for chain lightning =6 
				case 57:// Targeted Party Member
				case EFF_TARGET_DUEL: 
				case 27: // target is owner of pet
				case EFF_TARGET_MINION:// Minion Target
				case 33://Party members of totem, inside given range
				case EFF_TARGET_SINGLE_PARTY:// Single Target Party Member
				case EFF_TARGET_ALL_PARTY: // all Members of the targets party
				case EFF_TARGET_TOTEM_EARTH:
				case EFF_TARGET_TOTEM_WATER:
				case EFF_TARGET_TOTEM_AIR:
				case EFF_TARGET_TOTEM_FIRE:// Totem
				case 61: // targets with the same group/raid and the same class
					return TTYPE_SINGLETARGET;

					// spells like 17278:Cannon Fire and 21117:Summon Son of Flame A
				case 17: // A single target at a xyz location or the target is a possition xyz
				case 18:// Land under caster.Maybe not correct
				case 46://Unknown Summon Atal'ai Skeleton
				case EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
					return TTYPE_SOURCE;

			}//end switch
		}//end for
	return TTYPE_NULL;// this means a new spell :P
}
*/

/*void Spell::EventDelete()
{
	//this will execute until the events are finished, then delete :)
	if (!sEventMgr.HasEvent(this, EVENT_SPELL_HIT) && (m_spellScript == NULL || (m_spellScript->Auras.size() == 0 && m_spellScript->DynamicObjects.size() == 0)))
		return;

	//is unit (NOT PLAYER) not in world? EEEEEEEEEEK
	if (m_caster->IsUnit() && !m_caster->IsPlayer() && !m_caster->IsInWorld())
		return;
}*/

int32 Spell::event_GetInstanceID()
{
 //talents should really go with the caster, for scripts... doh
 if (deleted)
		return -1; //put us in world instance
 if (m_targets.m_target == m_caster)
	return m_caster->event_GetInstanceID();

 return m_instanceId;
}

uint32 Spell::GetBestSchoolForSpell(Unit* pVictim)
{
	int32 school = -1; //invalid school
	uint32 resist = 50000;

	for (uint32 i=0; i<7; ++i)
	{
		if (m_spellInfo->schoolMask & (1 << i) && ((pVictim->GetUInt32Value(UNIT_FIELD_RESISTANCES + i) < resist && !pVictim->SchoolImmunityList[i]) || school == -1))
		{
			school = i;
			resist=pVictim->GetUInt32Value(UNIT_FIELD_RESISTANCES + i);
		}
	}

	return school;
}
/*
//Note Randdrick : ancienne gestion des spells en 2.4.x
void Spell::_AddTarget(Unit* target, uint32 effectid)
{
	SpellTargetList::iterator itr;
	SpellTarget tgt;

	// look for the target in the list already
	for( itr = m_targetList.begin(); itr != m_targetList.end(); ++itr )
	{
		if( itr->Guid == target->GetGUID() )
		{
			// add the effect to it, hit result is already determined
			itr->EffectMask |= (1 << effectid);
			return;
		}
	}

	// setup struct
	tgt.Guid = target->GetGUID();
	tgt.EffectMask = (1 << effectid);

	// work out hit result (always true if we are a GO)
	tgt.HitResult = (((GameObject *)m_caster) || (((GameObject *)m_caster) && ((GameObject *)m_caster)->GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID) != GAMEOBJECT_TYPE_TRAP) ) ? SPELL_DID_HIT_SUCCESS : DidHit(effectid,target);

	// add to the list
	m_targetList.push_back(tgt);

	// add counter
	if( tgt.HitResult == SPELL_DID_HIT_SUCCESS )
		++m_objectsHit;
	else
		++m_objectsModerated;
}

void Spell::_AddTargetForced(const uint64& guid, const uint32 effectid)
{
	SpellTargetList::iterator itr;
	SpellTarget tgt;

	// look for the target in the list already
	for( itr = m_targetList.begin(); itr != m_targetList.end(); ++itr )
	{
		if( itr->Guid == guid )
		{
			// add the effect to it, hit result is already determined
			itr->EffectMask |= (1 << effectid);
			return;
		}
	}

	// setup struct
	tgt.Guid = guid;
	tgt.EffectMask = (1 << effectid);
	tgt.HitResult = SPELL_DID_HIT_SUCCESS;

	// add to the list
	m_targetList.push_back(tgt);

	// add counter
	if( tgt.HitResult == SPELL_DID_HIT_SUCCESS )
		++m_objectsHit;
	else
		++m_objectsModerated;
}
*/
char *SpellCastErrorMsg(uint8 error_result)
{
	switch(error_result)
	{
	 case SPELL_CANCAST_OK                    : return("SPELL_CANCAST_OK");
	 case SPELL_FAILED_AFFECTING_COMBAT       : return("SPELL_FAILED_AFFECTING_COMBAT");
	 case SPELL_FAILED_ALREADY_AT_FULL_HEALTH : return("SPELL_FAILED_ALREADY_AT_FULL_HEALTH");
	 case SPELL_FAILED_ALREADY_AT_FULL_MANA   : return("SPELL_FAILED_ALREADY_AT_FULL_MANA");
	 case SPELL_FAILED_ALREADY_AT_FULL_POWER  : return("SPELL_FAILED_ALREADY_AT_FULL_POWER");
	 case SPELL_FAILED_ALREADY_BEING_TAMED    : return("SPELL_FAILED_ALREADY_BEING_TAMED");
	 case SPELL_FAILED_ALREADY_HAVE_CHARM     : return("SPELL_FAILED_ALREADY_HAVE_CHARM");
	 case SPELL_FAILED_ALREADY_HAVE_SUMMON    : return("SPELL_FAILED_ALREADY_HAVE_SUMMON");
	 case SPELL_FAILED_ALREADY_OPEN           : return("SPELL_FAILED_ALREADY_OPEN");
	 case SPELL_FAILED_AURA_BOUNCED           : return("SPELL_FAILED_AURA_BOUNCED");
	 case SPELL_FAILED_AUTOTRACK_INTERRUPTED  : return("SPELL_FAILED_AUTOTRACK_INTERRUPTED");
	 case SPELL_FAILED_BAD_IMPLICIT_TARGETS   : return("SPELL_FAILED_BAD_IMPLICIT_TARGETS");
	 case SPELL_FAILED_BAD_TARGETS            : return("SPELL_FAILED_BAD_TARGETS");
	 case SPELL_FAILED_CANT_BE_CHARMED        : return("SPELL_FAILED_CANT_BE_CHARMED");
	 case SPELL_FAILED_CANT_BE_DISENCHANTED   : return("SPELL_FAILED_CANT_BE_DISENCHANTED");
	 case SPELL_FAILED_CANT_BE_DISENCHANTED_SKILL : return("SPELL_FAILED_CANT_BE_DISENCHANTED_SKILL");
	 case SPELL_FAILED_CANT_BE_MILLED         : return("SPELL_FAILED_CANT_BE_MILLED");
	 case SPELL_FAILED_CANT_BE_PROSPECTED     : return("SPELL_FAILED_CANT_BE_PROSPECTED");
	 case SPELL_FAILED_CANT_CAST_ON_TAPPED    : return("SPELL_FAILED_CANT_CAST_ON_TAPPED");
	 case SPELL_FAILED_CANT_DUEL_WHILE_INVISIBLE : return("SPELL_FAILED_CANT_DUEL_WHILE_INVISIBLE");
	 case SPELL_FAILED_CANT_DUEL_WHILE_STEALTHED : return("SPELL_FAILED_CANT_DUEL_WHILE_STEALTHED");
	 case SPELL_FAILED_CANT_STEALTH           : return("SPELL_FAILED_CANT_STEALTH");
	 case SPELL_FAILED_CASTER_AURASTATE       : return("SPELL_FAILED_CASTER_AURASTATE");
	 case SPELL_FAILED_CASTER_DEAD            : return("SPELL_FAILED_CASTER_DEAD");
	 case SPELL_FAILED_CHARMED                : return("SPELL_FAILED_CHARMED");
	 case SPELL_FAILED_CHEST_IN_USE           : return("SPELL_FAILED_CHEST_IN_USE");
	 case SPELL_FAILED_CONFUSED               : return("SPELL_FAILED_CONFUSED");
	 case SPELL_FAILED_DONT_REPORT            : return("SPELL_FAILED_DONT_REPORT");
	 case SPELL_FAILED_EQUIPPED_ITEM          : return("SPELL_FAILED_EQUIPPED_ITEM");
	 case SPELL_FAILED_EQUIPPED_ITEM_CLASS    : return("SPELL_FAILED_EQUIPPED_ITEM_CLASS");
	 case SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND : return("SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND");
	 case SPELL_FAILED_EQUIPPED_ITEM_CLASS_OFFHAND  : return("SPELL_FAILED_EQUIPPED_ITEM_CLASS_OFFHAND");
	 case SPELL_FAILED_ERROR                  : return("SPELL_FAILED_ERROR");
	 case SPELL_FAILED_FIZZLE                 : return("SPELL_FAILED_FIZZLE");
	 case SPELL_FAILED_FLEEING                : return("SPELL_FAILED_FLEEING");
	 case SPELL_FAILED_FOOD_LOWLEVEL          : return("SPELL_FAILED_FOOD_LOWLEVEL");
	 case SPELL_FAILED_HIGHLEVEL              : return("SPELL_FAILED_HIGHLEVEL");
	 case SPELL_FAILED_HUNGER_SATIATED        : return("SPELL_FAILED_HUNGER_SATIATED");
	 case SPELL_FAILED_IMMUNE                 : return("SPELL_FAILED_IMMUNE");
	 case SPELL_FAILED_INCORRECT_AREA         : return("SPELL_FAILED_INCORRECT_AREA");
	 case SPELL_FAILED_INTERRUPTED            : return("SPELL_FAILED_INTERRUPTED");
	 case SPELL_FAILED_INTERRUPTED_COMBAT     : return("SPELL_FAILED_INTERRUPTED_COMBAT");
	 case SPELL_FAILED_ITEM_ALREADY_ENCHANTED : return("SPELL_FAILED_ITEM_ALREADY_ENCHANTED");
	 case SPELL_FAILED_ITEM_GONE              : return("SPELL_FAILED_ITEM_GONE");
	 case SPELL_FAILED_ITEM_NOT_FOUND         : return("SPELL_FAILED_ITEM_NOT_FOUND");
	 case SPELL_FAILED_ITEM_NOT_READY         : return("SPELL_FAILED_ITEM_NOT_READY");
	 case SPELL_FAILED_LEVEL_REQUIREMENT      : return("SPELL_FAILED_LEVEL_REQUIREMENT");
	 case SPELL_FAILED_LINE_OF_SIGHT          : return("SPELL_FAILED_LINE_OF_SIGHT");
	 case SPELL_FAILED_LOWLEVEL               : return("SPELL_FAILED_LOWLEVEL");
	 case SPELL_FAILED_LOW_CASTLEVEL          : return("SPELL_FAILED_LOW_CASTLEVEL");
	 case SPELL_FAILED_MAINHAND_EMPTY         : return("SPELL_FAILED_MAINHAND_EMPTY");
	 case SPELL_FAILED_MOVING                 : return("SPELL_FAILED_MOVING");
	 case SPELL_FAILED_NEED_AMMO              : return("SPELL_FAILED_NEED_AMMO");
	 case SPELL_FAILED_NEED_AMMO_POUCH        : return("SPELL_FAILED_NEED_AMMO_POUCH");
	 case SPELL_FAILED_NEED_EXOTIC_AMMO       : return("SPELL_FAILED_NEED_EXOTIC_AMMO");
	 case SPELL_FAILED_NEED_MORE_ITEMS        : return("SPELL_FAILED_NEED_MORE_ITEMS");
	 case SPELL_FAILED_NOPATH                 : return("SPELL_FAILED_NOPATH");
	 case SPELL_FAILED_NOT_BEHIND             : return("SPELL_FAILED_NOT_BEHIND");
	 case SPELL_FAILED_NOT_FISHABLE           : return("SPELL_FAILED_NOT_FISHABLE");
	 case SPELL_FAILED_NOT_FLYING             : return("SPELL_FAILED_NOT_FLYING");
	 case SPELL_FAILED_NOT_HERE               : return("SPELL_FAILED_NOT_HERE");
	 case SPELL_FAILED_NOT_INFRONT            : return("SPELL_FAILED_NOT_INFRONT");
	 case SPELL_FAILED_NOT_IN_CONTROL         : return("SPELL_FAILED_NOT_IN_CONTROL");
	 case SPELL_FAILED_NOT_KNOWN              : return("SPELL_FAILED_NOT_KNOWN");
	 case SPELL_FAILED_NOT_MOUNTED            : return("SPELL_FAILED_NOT_MOUNTED");
	 case SPELL_FAILED_NOT_ON_TAXI            : return("SPELL_FAILED_NOT_ON_TAXI");
	 case SPELL_FAILED_NOT_ON_TRANSPORT       : return("SPELL_FAILED_NOT_ON_TRANSPORT");
	 case SPELL_FAILED_NOT_READY              : return("SPELL_FAILED_NOT_READY");
	 case SPELL_FAILED_NOT_SHAPESHIFT         : return("SPELL_FAILED_NOT_SHAPESHIFT");
	 case SPELL_FAILED_NOT_STANDING           : return("SPELL_FAILED_NOT_STANDING");
	 case SPELL_FAILED_NOT_TRADEABLE          : return("SPELL_FAILED_NOT_TRADEABLE");
	 case SPELL_FAILED_NOT_TRADING            : return("SPELL_FAILED_NOT_TRADING");
	 case SPELL_FAILED_NOT_UNSHEATHED         : return("SPELL_FAILED_NOT_UNSHEATHED");
	 case SPELL_FAILED_NOT_WHILE_GHOST        : return("SPELL_FAILED_NOT_WHILE_GHOST");
	 case SPELL_FAILED_NOT_WHILE_LOOTING      : return("SPELL_FAILED_NOT_WHILE_LOOTING");
	 case SPELL_FAILED_NO_AMMO                : return("SPELL_FAILED_NO_AMMO");
	 case SPELL_FAILED_NO_CHARGES_REMAIN      : return("SPELL_FAILED_NO_CHARGES_REMAIN");
	 case SPELL_FAILED_NO_CHAMPION            : return("SPELL_FAILED_NO_CHAMPION");
	 case SPELL_FAILED_NO_COMBO_POINTS        : return("SPELL_FAILED_NO_COMBO_POINTS");
	 case SPELL_FAILED_NO_DUELING             : return("SPELL_FAILED_NO_DUELING");
	 case SPELL_FAILED_NO_ENDURANCE           : return("SPELL_FAILED_NO_ENDURANCE");
	 case SPELL_FAILED_NO_FISH                : return("SPELL_FAILED_NO_FISH");
	 case SPELL_FAILED_NO_ITEMS_WHILE_SHAPESHIFTED : return("SPELL_FAILED_NO_ITEMS_WHILE_SHAPESHIFTED");
	 case SPELL_FAILED_NO_MOUNTS_ALLOWED      : return("SPELL_FAILED_NO_MOUNTS_ALLOWED");
	 case SPELL_FAILED_NO_PET                 : return("SPELL_FAILED_NO_PET");
	 case SPELL_FAILED_NO_POWER               : return("SPELL_FAILED_NO_POWER");
	 case SPELL_FAILED_NOTHING_TO_DISPEL      : return("SPELL_FAILED_NOTHING_TO_DISPEL");
	 case SPELL_FAILED_NOTHING_TO_STEAL       : return("SPELL_FAILED_NOTHING_TO_STEAL");
	 case SPELL_FAILED_ONLY_ABOVEWATER        : return("SPELL_FAILED_ONLY_ABOVEWATER");
	 case SPELL_FAILED_ONLY_DAYTIME           : return("SPELL_FAILED_ONLY_DAYTIME");
	 case SPELL_FAILED_ONLY_INDOORS           : return("SPELL_FAILED_ONLY_INDOORS");
	 case SPELL_FAILED_ONLY_MOUNTED           : return("SPELL_FAILED_ONLY_MOUNTED");
	 case SPELL_FAILED_ONLY_NIGHTTIME         : return("SPELL_FAILED_ONLY_NIGHTTIME");
	 case SPELL_FAILED_ONLY_OUTDOORS          : return("SPELL_FAILED_ONLY_OUTDOORS");
	 case SPELL_FAILED_ONLY_SHAPESHIFT        : return("SPELL_FAILED_ONLY_SHAPESHIFT");
	 case SPELL_FAILED_ONLY_STEALTHED         : return("SPELL_FAILED_ONLY_STEALTHED");
	 case SPELL_FAILED_ONLY_UNDERWATER        : return("SPELL_FAILED_ONLY_UNDERWATER");
	 case SPELL_FAILED_OUT_OF_RANGE           : return("SPELL_FAILED_OUT_OF_RANGE");
	 case SPELL_FAILED_PACIFIED               : return("SPELL_FAILED_PACIFIED");
	 case SPELL_FAILED_POSSESSED              : return("SPELL_FAILED_POSSESSED");
	 case SPELL_FAILED_REAGENTS               : return("SPELL_FAILED_REAGENTS");
	 case SPELL_FAILED_REQUIRES_AREA          : return("SPELL_FAILED_REQUIRES_AREA");
	 case SPELL_FAILED_REQUIRES_SPELL_FOCUS   : return("SPELL_FAILED_REQUIRES_SPELL_FOCUS");
	 case SPELL_FAILED_ROOTED                 : return("SPELL_FAILED_ROOTED");
	 case SPELL_FAILED_SILENCED               : return("SPELL_FAILED_SILENCED");
	 case SPELL_FAILED_SPELL_IN_PROGRESS      : return("SPELL_FAILED_SPELL_IN_PROGRESS");
	 case SPELL_FAILED_SPELL_LEARNED          : return("SPELL_FAILED_SPELL_LEARNED");
	 case SPELL_FAILED_SPELL_UNAVAILABLE      : return("SPELL_FAILED_SPELL_UNAVAILABLE");
	 case SPELL_FAILED_STUNNED                : return("SPELL_FAILED_STUNNED");
	 case SPELL_FAILED_TARGETS_DEAD           : return("SPELL_FAILED_TARGETS_DEAD");
	 case SPELL_FAILED_TARGET_AFFECTING_COMBAT: return("SPELL_FAILED_TARGET_AFFECTING_COMBAT");
	 case SPELL_FAILED_TARGET_AURASTATE       : return("SPELL_FAILED_TARGET_AURASTATE");
	 case SPELL_FAILED_TARGET_DUELING         : return("SPELL_FAILED_TARGET_DUELING");
	 case SPELL_FAILED_TARGET_ENEMY           : return("SPELL_FAILED_TARGET_ENEMY");
	 case SPELL_FAILED_TARGET_ENRAGED         : return("SPELL_FAILED_TARGET_ENRAGED");
	 case SPELL_FAILED_TARGET_FRIENDLY        : return("SPELL_FAILED_TARGET_FRIENDLY");
	 case SPELL_FAILED_TARGET_IN_COMBAT       : return("SPELL_FAILED_TARGET_IN_COMBAT");
	 case SPELL_FAILED_TARGET_IS_PLAYER       : return("SPELL_FAILED_TARGET_IS_PLAYER");
	 case SPELL_FAILED_TARGET_IS_PLAYER_CONTROLLED : return("SPELL_FAILED_TARGET_IS_PLAYER_CONTROLLED");
	 case SPELL_FAILED_TARGET_NOT_DEAD        : return("SPELL_FAILED_TARGET_NOT_DEAD");
	 case SPELL_FAILED_TARGET_NOT_IN_PARTY    : return("SPELL_FAILED_TARGET_NOT_IN_PARTY");
	 case SPELL_FAILED_TARGET_NOT_LOOTED      : return("SPELL_FAILED_TARGET_NOT_LOOTED");
	 case SPELL_FAILED_TARGET_NOT_PLAYER      : return("SPELL_FAILED_TARGET_NOT_PLAYER");
	 case SPELL_FAILED_TARGET_NO_POCKETS      : return("SPELL_FAILED_TARGET_NO_POCKETS");
	 case SPELL_FAILED_TARGET_NO_WEAPONS      : return("SPELL_FAILED_TARGET_NO_WEAPONS");
	 case SPELL_FAILED_TARGET_NO_RANGED_WEAPONS : return("SPELL_FAILED_TARGET_NO_RANGED_WEAPONS");
	 case SPELL_FAILED_TARGET_UNSKINNABLE	: return("SPELL_FAILED_TARGET_UNSKINNABLE");
	 case SPELL_FAILED_THIRST_SATIATED		: return("SPELL_FAILED_THIRST_SATIATED");
	 case SPELL_FAILED_TOO_CLOSE			: return("SPELL_FAILED_TOO_CLOSE");
	 case SPELL_FAILED_TOO_MANY_OF_ITEM		: return("SPELL_FAILED_TOO_MANY_OF_ITEM");
	 case SPELL_FAILED_TOTEM_CATEGORY		: return("SPELL_FAILED_TOTEM_CATEGORY");
	 case SPELL_FAILED_TOTEMS				: return("SPELL_FAILED_TOTEMS");
	 case SPELL_FAILED_TRY_AGAIN			: return("SPELL_FAILED_TRY_AGAIN");
	 case SPELL_FAILED_UNIT_NOT_BEHIND		: return("SPELL_FAILED_UNIT_NOT_BEHIND");
	 case SPELL_FAILED_UNIT_NOT_INFRONT		: return("SPELL_FAILED_UNIT_NOT_INFRONT");
	 case SPELL_FAILED_WRONG_PET_FOOD		: return("SPELL_FAILED_WRONG_PET_FOOD");
	 case SPELL_FAILED_NOT_WHILE_FATIGUED	: return("SPELL_FAILED_NOT_WHILE_FATIGUED");
	 case SPELL_FAILED_TARGET_NOT_IN_INSTANCE : return("SPELL_FAILED_TARGET_NOT_IN_INSTANCE");
	 case SPELL_FAILED_NOT_WHILE_TRADING	: return("SPELL_FAILED_NOT_WHILE_TRADING");
	 case SPELL_FAILED_TARGET_NOT_IN_RAID	: return("SPELL_FAILED_TARGET_NOT_IN_RAID");
	 case SPELL_FAILED_TARGET_FREEFORALL	: return("SPELL_FAILED_TARGET_FREEFORALL");
	 case SPELL_FAILED_NO_EDIBLE_CORPSES	: return("SPELL_FAILED_NO_EDIBLE_CORPSES");
	 case SPELL_FAILED_ONLY_BATTLEGROUNDS	: return("SPELL_FAILED_ONLY_BATTLEGROUNDS");
	 case SPELL_FAILED_TARGET_NOT_GHOST		: return("SPELL_FAILED_TARGET_NOT_GHOST");
	 case SPELL_FAILED_TRANSFORM_UNUSABLE	:  return("SPELL_FAILED_TRANSFORM_UNUSABLE");
	 case SPELL_FAILED_WRONG_WEATHER		: return("SPELL_FAILED_WRONG_WEATHER");
	 case SPELL_FAILED_DAMAGE_IMMUNE		: return("SPELL_FAILED_DAMAGE_IMMUNE");
	 case SPELL_FAILED_PREVENTED_BY_MECHANIC : return("SPELL_FAILED_PREVENTED_BY_MECHANIC");
	 case SPELL_FAILED_PLAY_TIME			: return("SPELL_FAILED_PLAY_TIME");
	 case SPELL_FAILED_REPUTATION			: return("SPELL_FAILED_REPUTATION");
	 case SPELL_FAILED_MIN_SKILL			: return("SPELL_FAILED_MIN_SKILL");
	 case SPELL_FAILED_NOT_IN_ARENA			: return("SPELL_FAILED_NOT_IN_ARENA");
	 case SPELL_FAILED_NOT_ON_SHAPESHIFT	: return("SPELL_FAILED_NOT_ON_SHAPESHIFT");
	 case SPELL_FAILED_NOT_ON_STEALTHED		: return("SPELL_FAILED_NOT_ON_STEALTHED");
	 case SPELL_FAILED_NOT_ON_DAMAGE_IMMUNE : return("SPELL_FAILED_NOT_ON_DAMAGE_IMMUNE");
	 case SPELL_FAILED_NOT_ON_MOUNTED       : return("SPELL_FAILED_NOT_ON_MOUNTED");
	 case SPELL_FAILED_TOO_SHALLOW          :  return("SPELL_FAILED_TOO_SHALLOW");
	 case SPELL_FAILED_TARGET_NOT_IN_SANCTUARY : return("SPELL_FAILED_TARGET_NOT_IN_SANCTUARY");
	 case SPELL_FAILED_TARGET_IS_TRIVIAL	: return("SPELL_FAILED_TARGET_IS_TRIVIAL");
	 case SPELL_FAILED_BM_OR_INVISGOD		: return("SPELL_FAILED_BM_OR_INVISGOD");
	 case SPELL_FAILED_EXPERT_RIDING_REQUIREMENT	: return("SPELL_FAILED_EXPERT_RIDING_REQUIREMENT");
	 case SPELL_FAILED_ARTISAN_RIDING_REQUIREMENT	: return("SPELL_FAILED_ARTISAN_RIDING_REQUIREMENT");
	 case SPELL_FAILED_NOT_IDLE				: return("SPELL_FAILED_NOT_IDLE");
	 case SPELL_FAILED_NOT_INACTIVE			: return("SPELL_FAILED_NOT_INACTIVE");
	 case SPELL_FAILED_PARTIAL_PLAYTIME		: return("SPELL_FAILED_PARTIAL_PLAYTIME");
	 case SPELL_FAILED_NO_PLAYTIME			: return("SPELL_FAILED_NO_PLAYTIME");
	 case SPELL_FAILED_NOT_IN_BATTLEGROUND	: return("SPELL_FAILED_NOT_IN_BATTLEGROUND");
	 case SPELL_FAILED_NOT_IN_RAID_INSTANCE	: return("SPELL_FAILED_NOT_IN_RAID_INSTANCE");
	 case SPELL_FAILED_ONLY_IN_ARENA		: return("SPELL_FAILED_ONLY_IN_ARENA");
	 case SPELL_FAILED_TARGET_LOCKED_TO_RAID_INSTANCE : return("SPELL_FAILED_TARGET_LOCKED_TO_RAID_INSTANCE");
	 case SPELL_FAILED_ON_USE_ENCHANT		: return("SPELL_FAILED_ON_USE_ENCHANT");
	 case SPELL_FAILED_NOT_ON_GROUND		: return("SPELL_FAILED_NOT_ON_GROUND");
	 case SPELL_FAILED_CUSTOM_ERROR			: return("SPELL_FAILED_CUSTOM_ERROR");
	 case SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW	: return("SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW");
	 case SPELL_FAILED_TOO_MANY_SOCKETS		: return("SPELL_FAILED_TOO_MANY_SOCKETS");
	 case SPELL_FAILED_INVALID_GLYPH		: return("SPELL_FAILED_INVALID_GLYPH");
	 case SPELL_FAILED_UNIQUE_GLYPH			: return("SPELL_FAILED_UNIQUE_GLYPH");
	 case SPELL_FAILED_GLYPH_SOCKET_LOCKED	: return("SPELL_FAILED_GLYPH_SOCKET_LOCKED");
	 case SPELL_FAILED_NO_VALID_TARGETS		: return("SPELL_FAILED_NO_VALID_TARGETS");
	 case SPELL_FAILED_ITEM_AT_MAX_CHARGES	: return("SPELL_FAILED_ITEM_AT_MAX_CHARGES");
	 case SPELL_FAILED_NOT_IN_BARBERSHOP	: return("SPELL_FAILED_NOT_IN_BARBERSHOP");
	 case SPELL_FAILED_FISHING_TOO_LOW		: return("SPELL_FAILED_FISHING_TOO_LOW");
	 case SPELL_FAILED_ITEM_ENCHANT_TRADE_WINDOW : return("SPELL_FAILED_ITEM_ENCHANT_TRADE_WINDOW");
	 case SPELL_FAILED_SUMMON_PENDING       : return("SPELL_FAILED_SUMMON_PENDING");
	 case SPELL_FAILED_MAX_SOCKETS          : return("SPELL_FAILED_MAX_SOCKETS");
	 case SPELL_FAILED_PET_CAN_RENAME       : return("SPELL_FAILED_PET_CAN_RENAME");
	 case SPELL_FAILED_TARGET_CANNOT_BE_RESURRECTED : return("SPELL_FAILED_TARGET_CANNOT_BE_RESURRECTED");
	 //case SPELL_FAILED_UNKNOWN             :  return("SPELL_FAILED_UNKNOWN");

	 default :  return("SPELL_FAILED_UNKNOWN");
	 
	}
}
//------------------------------------------------------------------------
char *SpellEffectTxt(uint8 EffectNum)
{
	switch(EffectNum)
	{
      case SPELL_EFFECT_NULL : return(" SPELL_EFFECT_NULL "); 
      case SPELL_EFFECT_INSTANT_KILL : return(" SPELL_EFFECT_INSTANT_KILL "); //1
      case SPELL_EFFECT_SCHOOL_DAMAGE : return(" SPELL_EFFECT_SCHOOL_DAMAGE "); //2
      case SPELL_EFFECT_DUMMY : return(" SPELL_EFFECT_DUMMY "); //3
      case SPELL_EFFECT_PORTAL_TELEPORT : return(" SPELL_EFFECT_PORTAL_TELEPORT "); //4
      case SPELL_EFFECT_TELEPORT_UNITS : return(" SPELL_EFFECT_TELEPORT_UNITS//5 "); 
      case SPELL_EFFECT_APPLY_AURA : return(" SPELL_EFFECT_APPLY_AURA//6 "); 
      case SPELL_EFFECT_ENVIRONMENTAL_DAMAGE : return(" SPELL_EFFECT_ENVIRONMENTAL_DAMAGE "); //7
      case SPELL_EFFECT_POWER_DRAIN : return(" SPELL_EFFECT_POWER_DRAIN "); //8
      case SPELL_EFFECT_HEALTH_LEECH : return(" SPELL_EFFECT_HEALTH_LEECH "); //9
      case SPELL_EFFECT_HEAL : return(" SPELL_EFFECT_HEAL "); //10
      case SPELL_EFFECT_BIND : return(" SPELL_EFFECT_BIND "); //11
      case SPELL_EFFECT_PORTAL : return(" SPELL_EFFECT_PORTAL//12 "); 
      case SPELL_EFFECT_RITUAL_BASE : return(" SPELL_EFFECT_RITUAL_BASE "); //13
      case SPELL_EFFECT_RITUAL_SPECIALIZE : return(" SPELL_EFFECT_RITUAL_SPECIALIZE "); //14
      case SPELL_EFFECT_RITUAL_ACTIVATE_PORTAL : return(" SPELL_EFFECT_RITUAL_ACTIVATE_PORTAL//15 "); 
      case SPELL_EFFECT_QUEST_COMPLETE         : return(" SPELL_EFFECT_QUEST_COMPLETE//16 "); 
      case SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL : return(" SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL//17 "); 
      case SPELL_EFFECT_RESURRECT : return(" SPELL_EFFECT_RESURRECT "); //18
      case SPELL_EFFECT_ADD_EXTRA_ATTACKS : return(" SPELL_EFFECT_ADD_EXTRA_ATTACKS "); //19
      case SPELL_EFFECT_DODGE : return(" SPELL_EFFECT_DODGE "); //20
      case SPELL_EFFECT_EVADE : return(" SPELL_EFFECT_EVADE "); //21
      case SPELL_EFFECT_PARRY : return(" SPELL_EFFECT_PARRY "); //22
      case SPELL_EFFECT_BLOCK : return(" SPELL_EFFECT_BLOCK "); //23
      case SPELL_EFFECT_CREATE_ITEM : return(" SPELL_EFFECT_CREATE_ITEM "); //24
      case SPELL_EFFECT_WEAPON      : return(" SPELL_EFFECT_WEAPON//25 "); 
      case SPELL_EFFECT_DEFENSE     : return(" SPELL_EFFECT_DEFENSE "); //26
      case SPELL_EFFECT_PERSISTENT_AREA_AURA : return(" SPELL_EFFECT_PERSISTENT_AREA_AURA "); //27
      case SPELL_EFFECT_SUMMON               : return(" SPELL_EFFECT_SUMMON//28 "); 
      case SPELL_EFFECT_LEAP                 : return(" SPELL_EFFECT_LEAP "); //29
      case SPELL_EFFECT_ENERGIZE             : return(" SPELL_EFFECT_ENERGIZE "); //30
      case SPELL_EFFECT_WEAPON_PERCENT_DAMAGE : return(" SPELL_EFFECT_WEAPON_PERCENT_DAMAGE "); //31
      case SPELL_EFFECT_TRIGGER_MISSILE : return(" SPELL_EFFECT_TRIGGER_MISSILE "); //32
      case SPELL_EFFECT_OPEN_LOCK : return(" SPELL_EFFECT_OPEN_LOCK "); //33
      case SPELL_EFFECT_TRANSFORM_ITEM : return(" SPELL_EFFECT_TRANSFORM_ITEM//34 "); 
      case SPELL_EFFECT_APPLY_AREA_AURA : return(" SPELL_EFFECT_APPLY_AREA_AURA "); //35
      case SPELL_EFFECT_LEARN_SPELL : return(" SPELL_EFFECT_LEARN_SPELL "); //36
      case SPELL_EFFECT_SPELL_DEFENSE : return(" SPELL_EFFECT_SPELL_DEFENSE "); //37
      case SPELL_EFFECT_DISPEL : return(" SPELL_EFFECT_DISPEL//38 "); 
      case SPELL_EFFECT_LANGUAGE : return(" SPELL_EFFECT_LANGUAGE "); //39
      case SPELL_EFFECT_DUAL_WIELD : return(" SPELL_EFFECT_DUAL_WIELD//40 "); 
      case SPELL_EFFECT_SUMMON_WILD : return(" SPELL_EFFECT_SUMMON_WILD "); //41
      case SPELL_EFFECT_JUMP_MOVE : return(" SPELL_EFFECT_JUMP_MOVE "); //42
      case SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER : return(" SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER//43 "); 
      case SPELL_EFFECT_SKILL_STEP : return(" SPELL_EFFECT_SKILL_STEP//44 "); 
      case SPELL_EFFECT_UNDEFINED_45 : return(" SPELL_EFFECT_UNDEFINED_45 "); //45
      case SPELL_EFFECT_SPAWN : return(" SPELL_EFFECT_SPAWN "); //46
      case SPELL_EFFECT_TRADE_SKILL : return(" SPELL_EFFECT_TRADE_SKILL "); //47
      case SPELL_EFFECT_STEALTH : return(" SPELL_EFFECT_STEALTH "); //48
      case SPELL_EFFECT_DETECT : return(" SPELL_EFFECT_DETECT//49 "); 
      case SPELL_EFFECT_SUMMON_OBJECT : return(" SPELL_EFFECT_SUMMON_OBJECT "); //50
      //case SPELL_EFFECT_TRANS_DOOR : return(" //SPELL_EFFECT_TRANS_DOOR "); //50
      case SPELL_EFFECT_FORCE_CRITICAL_HIT : return(" SPELL_EFFECT_FORCE_CRITICAL_HIT//51 "); 
      case SPELL_EFFECT_GUARANTEE_HIT : return(" SPELL_EFFECT_GUARANTEE_HIT "); //52
      case SPELL_EFFECT_ENCHANT_ITEM : return(" SPELL_EFFECT_ENCHANT_ITEM "); //53
      case SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY : return(" SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY//54 "); 
      case SPELL_EFFECT_TAMECREATURE : return(" SPELL_EFFECT_TAMECREATURE "); //55
      case SPELL_EFFECT_SUMMON_PET : return(" SPELL_EFFECT_SUMMON_PET//56 "); 
      case SPELL_EFFECT_LEARN_PET_SPELL : return(" SPELL_EFFECT_LEARN_PET_SPELL "); //57
      case SPELL_EFFECT_WEAPON_DAMAGE : return(" SPELL_EFFECT_WEAPON_DAMAGE "); //58
      case SPELL_EFFECT_OPEN_LOCK_ITEM : return(" SPELL_EFFECT_OPEN_LOCK_ITEM//59 "); 
      case SPELL_EFFECT_PROFICIENCY : return(" SPELL_EFFECT_PROFICIENCY "); //60
      case SPELL_EFFECT_SEND_EVENT  : return(" SPELL_EFFECT_SEND_EVENT//61 "); 
      case SPELL_EFFECT_POWER_BURN  : return(" SPELL_EFFECT_POWER_BURN//62 "); 
      case SPELL_EFFECT_THREAT      : return(" SPELL_EFFECT_THREAT//63 "); 
      case SPELL_EFFECT_TRIGGER_SPELL : return(" SPELL_EFFECT_TRIGGER_SPELL "); //64
      case SPELL_EFFECT_RAID_AREA_AURA : return(" SPELL_EFFECT_RAID_AREA_AURA "); //65
      case SPELL_EFFECT_POWER_FUNNEL : return(" SPELL_EFFECT_POWER_FUNNEL "); //66
      case SPELL_EFFECT_HEAL_MAX_HEALTH : return(" SPELL_EFFECT_HEAL_MAX_HEALTH "); //67
      case SPELL_EFFECT_INTERRUPT_CAST  : return(" SPELL_EFFECT_INTERRUPT_CAST//68 "); 
      case SPELL_EFFECT_DISTRACT : return(" SPELL_EFFECT_DISTRACT "); //69
      case SPELL_EFFECT_PULL : return(" SPELL_EFFECT_PULL "); //70
      case SPELL_EFFECT_PICKPOCKET   : return(" SPELL_EFFECT_PICKPOCKET//71 "); 
      case SPELL_EFFECT_ADD_FARSIGHT : return(" SPELL_EFFECT_ADD_FARSIGHT "); //72
      case SPELL_EFFECT_SUMMON_POSSESSED : return(" SPELL_EFFECT_SUMMON_POSSESSED "); //73
      case SPELL_EFFECT_APPLY_GLYPH : return(" SPELL_EFFECT_APPLY_GLYPH "); //74
      case SPELL_EFFECT_HEAL_MECHANICAL : return(" SPELL_EFFECT_HEAL_MECHANICAL "); //75
      case SPELL_EFFECT_SUMMON_OBJECT_WILD : return(" SPELL_EFFECT_SUMMON_OBJECT_WILD//76 "); 
      case SPELL_EFFECT_SCRIPT_EFFECT : return(" SPELL_EFFECT_SCRIPT_EFFECT "); //77
      case SPELL_EFFECT_ATTACK : return(" SPELL_EFFECT_ATTACK//78 "); 
      case SPELL_EFFECT_SANCTUARY : return(" SPELL_EFFECT_SANCTUARY "); //79
      case SPELL_EFFECT_ADD_COMBO_POINTS : return(" SPELL_EFFECT_ADD_COMBO_POINTS "); //80
      case SPELL_EFFECT_CREATE_HOUSE : return(" SPELL_EFFECT_CREATE_HOUSE "); //81
      case SPELL_EFFECT_BIND_SIGHT : return(" SPELL_EFFECT_BIND_SIGHT//82 "); 
      case SPELL_EFFECT_DUEL : return(" SPELL_EFFECT_DUEL "); //83
      case SPELL_EFFECT_STUCK : return(" SPELL_EFFECT_STUCK "); //84
      case SPELL_EFFECT_SUMMON_PLAYER : return(" SPELL_EFFECT_SUMMON_PLAYER "); //85
      case SPELL_EFFECT_ACTIVATE_OBJECT : return(" SPELL_EFFECT_ACTIVATE_OBJECT "); //86
      case SPELL_EFFECT_SUMMON_TOTEM_SLOT1 : return(" SPELL_EFFECT_SUMMON_TOTEM_SLOT1//87 "); 
      case SPELL_EFFECT_SUMMON_TOTEM_SLOT2 : return(" SPELL_EFFECT_SUMMON_TOTEM_SLOT2//88 "); 
      case SPELL_EFFECT_SUMMON_TOTEM_SLOT3 : return(" SPELL_EFFECT_SUMMON_TOTEM_SLOT3//89 "); 
      case SPELL_EFFECT_SUMMON_TOTEM_SLOT4 : return(" SPELL_EFFECT_SUMMON_TOTEM_SLOT4//90 "); 
      case SPELL_EFFECT_THREAT_ALL : return(" SPELL_EFFECT_THREAT_ALL//91 "); 
      case SPELL_EFFECT_ENCHANT_HELD_ITEM : return(" SPELL_EFFECT_ENCHANT_HELD_ITEM "); //92
      case SPELL_EFFECT_SUMMON_PHANTASM : return(" SPELL_EFFECT_SUMMON_PHANTASM "); //93
      case SPELL_EFFECT_SELF_RESURRECT : return(" SPELL_EFFECT_SELF_RESURRECT//94 "); 
      case SPELL_EFFECT_SKINNING : return(" SPELL_EFFECT_SKINNING "); //95
      case SPELL_EFFECT_CHARGE : return(" SPELL_EFFECT_CHARGE//96 "); 
      case SPELL_EFFECT_SUMMON_CRITTER : return(" SPELL_EFFECT_SUMMON_CRITTER//97 "); 
      case SPELL_EFFECT_KNOCK_BACK : return(" SPELL_EFFECT_KNOCK_BACK//98 "); 
      case SPELL_EFFECT_DISENCHANT : return(" SPELL_EFFECT_DISENCHANT//99 "); 
      case SPELL_EFFECT_INEBRIATE : return(" SPELL_EFFECT_INEBRIATE "); //100
      case SPELL_EFFECT_FEED_PET : return(" SPELL_EFFECT_FEED_PET "); //101
      case SPELL_EFFECT_DISMISS_PET : return(" SPELL_EFFECT_DISMISS_PET "); //102
      case SPELL_EFFECT_REPUTATION : return(" SPELL_EFFECT_REPUTATION//103 "); 
      case SPELL_EFFECT_SUMMON_OBJECT_SLOT1 : return(" SPELL_EFFECT_SUMMON_OBJECT_SLOT1 "); //104
      case SPELL_EFFECT_SUMMON_OBJECT_SLOT2 : return(" SPELL_EFFECT_SUMMON_OBJECT_SLOT2 "); //105
      case SPELL_EFFECT_SUMMON_OBJECT_SLOT3 : return(" SPELL_EFFECT_SUMMON_OBJECT_SLOT3 "); //106
      case SPELL_EFFECT_SUMMON_OBJECT_SLOT4 : return(" SPELL_EFFECT_SUMMON_OBJECT_SLOT4 "); //107
      case SPELL_EFFECT_DISPEL_MECHANIC : return(" SPELL_EFFECT_DISPEL_MECHANIC "); //108
      case SPELL_EFFECT_SUMMON_DEAD_PET : return(" SPELL_EFFECT_SUMMON_DEAD_PET "); //109
      case SPELL_EFFECT_DESTROY_ALL_TOTEMS : return(" SPELL_EFFECT_DESTROY_ALL_TOTEMS//110 "); 
      case SPELL_EFFECT_DURABILITY_DAMAGE : return(" SPELL_EFFECT_DURABILITY_DAMAGE "); //111
      case SPELL_EFFECT_SUMMON_DEMON : return(" SPELL_EFFECT_SUMMON_DEMON "); //112
      case SPELL_EFFECT_RESURRECT_FLAT : return(" SPELL_EFFECT_RESURRECT_FLAT//113 "); 
      case SPELL_EFFECT_ATTACK_ME : return(" SPELL_EFFECT_ATTACK_ME "); //114
      case SPELL_EFFECT_DURABILITY_DAMAGE_PCT : return(" SPELL_EFFECT_DURABILITY_DAMAGE_PCT "); //115
      case SPELL_EFFECT_SKIN_PLAYER_CORPSE : return(" SPELL_EFFECT_SKIN_PLAYER_CORPSE//116 "); 
      case SPELL_EFFECT_SPIRIT_HEAL : return(" SPELL_EFFECT_SPIRIT_HEAL "); //117
      case SPELL_EFFECT_SKILL : return(" SPELL_EFFECT_SKILL "); //118
      case SPELL_EFFECT_APPLY_PET_AURA : return(" SPELL_EFFECT_APPLY_PET_AURA//119 "); 
      case SPELL_EFFECT_TELEPORT_GRAVEYARD : return(" SPELL_EFFECT_TELEPORT_GRAVEYARD//120 "); 
      case SPELL_EFFECT_DUMMYMELEE : return(" SPELL_EFFECT_DUMMYMELEE//121 "); 
      case SPELL_EFFECT_UNKNOWN1 : return(" SPELL_EFFECT_UNKNOWN1 "); //122
      case SPELL_EFFECT_UNKNOWN2 : return(" SPELL_EFFECT_UNKNOWN2 "); //123
      case SPELL_EFFECT_PLAYER_PULL : return(" SPELL_EFFECT_PLAYER_PULL "); //124
      case SPELL_EFFECT_UNKNOWN4 : return(" SPELL_EFFECT_UNKNOWN4 "); //125
      case SPELL_EFFECT_UNKNOWN5 : return(" SPELL_EFFECT_UNKNOWN5 "); //126
      case SPELL_EFFECT_PROSPECTING : return(" SPELL_EFFECT_PROSPECTING "); //127
      case SPELL_EFFECT_UNKNOWN7 : return(" SPELL_EFFECT_UNKNOWN7 "); //128
      case SPELL_EFFECT_UNKNOWN8 : return(" SPELL_EFFECT_UNKNOWN8 "); //129
      case SPELL_EFFECT_UNKNOWN9 : return(" SPELL_EFFECT_UNKNOWN9 "); //129
      case SPELL_EFFECT_UNKNOWN10 : return(" SPELL_EFFECT_UNKNOWN10 "); //130
      case SPELL_EFFECT_UNKNOWN11 : return(" SPELL_EFFECT_UNKNOWN11 "); //131
      case SPELL_EFFECT_UNKNOWN12 : return(" SPELL_EFFECT_UNKNOWN12 "); //132
      case SPELL_EFFECT_FORGET_SPECIALIZATION : return(" SPELL_EFFECT_FORGET_SPECIALIZATION "); //133
      case SPELL_EFFECT_UNKNOWN14 : return(" SPELL_EFFECT_UNKNOWN14 "); //134
      case SPELL_EFFECT_UNKNOWN15 : return(" SPELL_EFFECT_UNKNOWN15 "); //135
      case SPELL_EFFECT_UNKNOWN16 : return(" SPELL_EFFECT_UNKNOWN16 "); //136
      case SPELL_EFFECT_UNKNOWN17 : return(" SPELL_EFFECT_UNKNOWN17 "); //137
      case SPELL_EFFECT_UNKNOWN18 : return(" SPELL_EFFECT_UNKNOWN18 "); //138
      case SPELL_EFFECT_UNKNOWN19 : return(" SPELL_EFFECT_UNKNOWN19 "); //139
      case SPELL_EFFECT_UNKNOWN20 : return(" SPELL_EFFECT_UNKNOWN20 "); //140
      case SPELL_EFFECT_UNKNOWN21 : return(" SPELL_EFFECT_UNKNOWN21 "); //141
      case SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE : return(" SPELL_EFFECT_TRIGGER_SPELL_WITH_VALUE "); //142
      case SPELL_EFFECT_UNKNOWN23 : return(" SPELL_EFFECT_UNKNOWN23 "); //143
      case SPELL_EFFECT_MODIFY_GRAVITY : return(" SPELL_EFFECT_MODIFY_GRAVITY//144 "); 
      case SPELL_EFFECT_REFRESH_RUNE : return(" SPELL_EFFECT_REFRESH_RUNE "); 
      case SPELL_EFFECT_FAIL_QUEST : return(" SPELL_EFFECT_FAIL_QUEST "); 
      case SPELL_EFFECT_UNKNOWN147 : return(" SPELL_EFFECT_UNKNOWN147 "); 
      case SPELL_EFFECT_UNKNOWN148 : return(" SPELL_EFFECT_UNKNOWN148 "); //148
      case SPELL_EFFECT_UNKNOWN149 : return(" SPELL_EFFECT_UNKNOWN149 "); //149
      case SPELL_EFFECT_UNKNOWN150 : return(" SPELL_EFFECT_UNKNOWN150 "); //150
      case SPELL_EFFECT_UNKNOWN151 : return(" SPELL_EFFECT_UNKNOWN151 "); //151
      case SPELL_EFFECT_UNKNOWN152 : return(" SPELL_EFFECT_UNKNOWN152 "); //152
      case SPELL_EFFECT_SUMMON_TARGET : return(" SPELL_EFFECT_SUMMON_TARGET "); //153
      case SPELL_EFFECT_SUMMON_REFER_A_FRIEND : return(" SPELL_EFFECT_SUMMON_REFER_A_FRIEND "); //154
      case SPELL_EFFECT_TAME_CREATURE : return(" SPELL_EFFECT_TAME_CREATURE "); 
      case SPELL_EFFECT_UNKNOWN156 : return(" SPELL_EFFECT_UNKNOWN "); //156
      case SPELL_EFFECT_UNKNOWN157 : return(" SPELL_EFFECT_UNKNOWN "); //157
      case SPELL_EFFECT_MILLING : return(" SPELL_EFFECT_MILLING "); //ArcEmu
      default : return(" SPELL_EFFECT_UNKNOWN ");
	}
};