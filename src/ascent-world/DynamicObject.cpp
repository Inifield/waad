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

DynamicObject::DynamicObject(uint32 high, uint32 low)
{
	m_objectTypeId = TYPEID_DYNAMICOBJECT;
	m_valuesCount = DYNAMICOBJECT_END;
	m_uint32Values = _fields;
	memset(m_uint32Values, 0,(DYNAMICOBJECT_END)*sizeof(uint32));
	m_updateMask.SetCount(DYNAMICOBJECT_END);
	m_uint32Values[OBJECT_FIELD_TYPE] = TYPE_DYNAMICOBJECT|TYPE_OBJECT;
	m_uint32Values[OBJECT_FIELD_GUID] = low;
	m_uint32Values[OBJECT_FIELD_GUID+1] = high;
	m_wowGuid.Init(GetGUID());
	m_floatValues[OBJECT_FIELD_SCALE_X] = 1;

	m_spellScript = NULL;


	m_parentSpell = NULL;
	m_aliveDuration = 0;
	m_caster = NULL;
	u_caster = NULL;
	m_spellProto = NULL;
	p_caster = NULL;
}

DynamicObject::~DynamicObject()
{
	// remove aura from all targets
	DynamicObjectList::iterator jtr  = targets.begin();
	DynamicObjectList::iterator jend = targets.end();
	Unit * target;

	while(jtr != jend)
	{
		target = *jtr;
		++jtr;
		target->RemoveAura(m_spellProto->Id);
	}

	if(u_caster != NULL && u_caster->dynObj == this)
		u_caster->dynObj = 0;

	if (m_spellScript != NULL)
		m_spellScript->RemoveRef(this);
}

void DynamicObject::Create(Unit * caster, Spell * pSpell, float x, float y, float z, uint32 duration, float radius)
{
	Object::_Create(caster->GetMapId(),x, y, z, 0);
	if(pSpell->m_caster->IsGO())
	{
		m_parentSpell = pSpell;
	}

	if (pSpell->m_spellScript != NULL)
		pSpell->m_spellScript->AddRef(this);

	
	//p_caster = caster->IsPlayer()? static_cast<Player*>(caster) : NULL;
	m_caster = caster;
	//u_caster = caster;

	m_spellProto = pSpell->m_spellInfo;
	SetUInt64Value(DYNAMICOBJECT_CASTER, caster->GetGUID());

	m_uint32Values[OBJECT_FIELD_ENTRY] = m_spellProto->Id;
	m_uint32Values[DYNAMICOBJECT_BYTES] = 0x01eeeeee;
	m_uint32Values[DYNAMICOBJECT_SPELLID] = m_spellProto->Id;

	m_floatValues[DYNAMICOBJECT_RADIUS] = radius;
	// disparu 332.11403
//	m_floatValues[DYNAMICOBJECT_POS_X]  = x;
//	m_floatValues[DYNAMICOBJECT_POS_Y]  = y;
//	m_floatValues[DYNAMICOBJECT_POS_Z]  = z;

	m_aliveDuration = duration;
	m_faction = caster->m_faction;
	m_factionDBC = caster->m_factionDBC;
	m_Phase = caster->GetPhase();

	/*if(caster->dynObj != 0)
	{
		// expire next update
		caster->dynObj->m_aliveDuration = 1;
		caster->dynObj->UpdateTargets();
	}*/

	caster->dynObj = this;

	if(pSpell->m_caster->IsGO())
	{
	   PushToWorld(((GameObject *)pSpell->m_caster)->GetMapMgr());
	}
	else 
		PushToWorld(caster->GetMapMgr());
	
	sEventMgr.AddEvent(this, &DynamicObject::UpdateTargets, EVENT_DYNAMICOBJECT_UPDATE, 100, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void DynamicObject::AddInRangeObject( Object* pObj )
{
	if( pObj->IsUnit() )
	{
		bool attackable = isAttackable( m_caster, pObj );
		
		if( attackable )
			m_inRangeOppFactions.insert( static_cast< Unit* >( pObj ) );
	}
	Object::AddInRangeObject( pObj );
}

void DynamicObject::OnRemoveInRangeObject( Object* pObj )
{
	if( pObj->IsUnit() )
	{
		m_inRangeOppFactions.erase( static_cast< Unit* >( pObj ) );
		targets.erase( static_cast< Unit* >( pObj ) );
	}
	Object::OnRemoveInRangeObject( pObj );
}

void DynamicObject::UpdateTargets()
{
	//Log.Notice("[DynamicObject::UpdateTargets]","In progress...");

	if(m_aliveDuration == 0)
		return;

	if(m_aliveDuration >= 100)
	{
//		FactionRangeList::iterator itr  = m_inRangeOppFactions.begin();
//		FactionRangeList::iterator iend = m_inRangeOppFactions.end();
		std::set<Object*>::iterator itr = GetInRangeSetBegin(),itr2;
		std::set<Object*>::iterator iend = GetInRangeSetEnd();
		Unit * target;
		Aura * pAura;
		float radius = m_floatValues[DYNAMICOBJECT_RADIUS]*m_floatValues[DYNAMICOBJECT_RADIUS];

		while(itr != iend)
		{
//			target = *itr;
//			++itr;

			itr2 = itr;
			++itr;

			if( !( (*itr2)->IsUnit() ) || ! static_cast< Unit* >( *itr2 )->isAlive() || ( static_cast< Creature* >( *itr2 )->IsTotem() && !static_cast< Unit* >( *itr2 )->IsPlayer() ) )
				continue;

			target = static_cast< Unit* >( *itr2 );

			if( !isAttackable( m_caster, target, !(m_spellProto->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED) ) )
				continue;

			// skip units already hit, their range will be tested later
			if(targets.find(target) != targets.end())
				continue;

			if(GetDistanceSq(target) <= radius)
			{
				pAura = new Aura(m_spellProto, m_aliveDuration,INVOC_DURATION, u_caster, target);
				for(uint32 i = 0; i < 3; ++i)
				{
					if(m_spellProto->Effect[i] ==  SPELL_EFFECT_PERSISTENT_AREA_AURA)
					{
						pAura->AddMod(m_spellProto->EffectApplyAuraName[i],
							m_spellProto->EffectBasePoints[i]+1, m_spellProto->EffectMiscValue[i], i);
					}
				}
				target->AddAura(pAura, m_spellScript);
				if(p_caster)
				{
					p_caster->HandleProc(PROC_ON_HARMFULSPELL_LAND | PROC_ON_CAST_SPELL,target, m_spellProto);
					p_caster->m_procCounter = 0;
				}

				// add to target list
				targets.insert(target);
			}
		}

		// loop the targets, check the range of all of them
		DynamicObjectList::iterator jtr  = targets.begin();
		DynamicObjectList::iterator jtr2;
		DynamicObjectList::iterator jend = targets.end();
		
		while(jtr != jend)
		{
			target = *jtr;
			jtr2 = jtr;
			++jtr;

			if(GetDistanceSq(target) > radius)
			{
				targets.erase(jtr2);
				target->RemoveAura(m_spellProto->Id);
			}
		}

		m_aliveDuration -= 100;
	}
	else
	{
		m_aliveDuration = 0;
	}

	if(m_aliveDuration == 0)
	{
		DynamicObjectList::iterator jtr  = targets.begin();
		DynamicObjectList::iterator jend = targets.end();
		Unit * target;

		while(jtr != jend)
		{
			target = *jtr;
			++jtr;
			target->RemoveAura(m_spellProto->Id);
		}

		Remove();
	}
}

void DynamicObject::Remove()
{
	if(IsInWorld())
		RemoveFromWorld(true);
}

const char* DynamicObject::GetObjectName()
{
	return m_spellProto->Name;
}