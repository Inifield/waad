/****************************************************************************
 *
 * SpellHandler Plugin
 * Copyright (c) 2007 Team Ascent
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons,
 * 543 Howard Street, 5th Floor, San Francisco, California, 94105, USA.
 *
 * EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW, IN NO EVENT WILL LICENSOR BE LIABLE TO YOU
 * ON ANY LEGAL THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES
 * ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK, EVEN IF LICENSOR HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 */

#include "StdAfx.h"
#include "Setup.h"

/*
	How to add a new item spell to the dummy spell handler:

		1) Add a new function to handle the spell at the end of this file but before the
		   SetupItemSpells_1() function. SetupItemSpells_1() must always be the last function.

		2) Register the dummy spell by adding a new line to the end of the list in the
		   SetupItemSpells_1() function.

	Please look at how the other spells are handled and try to use the
	same variable names and formatting style in your new spell handler.
*/

// *****************************************************************************

bool GnomishTransporter(uint32 i, Spell * pSpell)
{
	if(!pSpell->m_caster->IsPlayer()) return true;

	((Player *)pSpell->m_caster)->EventAttackStop();
	((Player *)pSpell->m_caster)->SafeTeleport(1, 0, LocationVector(-7169.41f, -3838.63f, 8.72f));
	return true;
}

// -----------------------------------------------------------------------------

bool NoggenFoggerElixr(uint32 i, Spell * pSpell)
{
	if(!pSpell->m_caster->IsPlayer()) return true;

	SpellEntry *spInfo = NULL;
	uint32 chance = RandomUInt(2);

	switch(chance)
	{
	case 0:
		spInfo = dbcSpell.LookupEntry(__Noggenfogger_Elixir____0); // 16591
		break;
	case 1:
		spInfo = dbcSpell.LookupEntry(__Noggenfogger_Elixir____1); // 16593
		break;
	case 2:
		spInfo = dbcSpell.LookupEntry(__Noggenfogger_Elixir____2); // 16595
		break;
	}

	if(!spInfo) return true;

	((Player *)pSpell->m_caster)->CastSpell(((Player *)pSpell->m_caster), spInfo, true);
	return true;
}

// -----------------------------------------------------------------------------

bool HallowsEndCandy(uint32 i, Spell * pSpell)
{
/*
#define __Hallow_s_End_Candy				24924
#define __Hallow_s_End_Candy___0			24925
#define __Hallow_s_End_Candy___1			24926
#define __Hallow_s_End_Candy___2			24927
#define __Hallow_s_End_Candy___3			24930
*/
 SpellEntry *spInfo = NULL;
 uint32 chance = RandomUInt(3);

 if(!pSpell->m_caster->IsPlayer()) return true;

 switch(chance)
	{
	 default:
	 case 0: spInfo = dbcSpell.LookupEntry(__Hallow_s_End_Candy____0); break;
	 case 1: spInfo = dbcSpell.LookupEntry(__Hallow_s_End_Candy____1); break;
	 case 2: spInfo = dbcSpell.LookupEntry(__Hallow_s_End_Candy____2); break;
	 case 3: spInfo = dbcSpell.LookupEntry(__Hallow_s_End_Candy____3); break;
	}

	if(!spInfo) return true;

	((Player *)pSpell->m_caster)->CastSpell(((Player *)pSpell->m_caster), spInfo, true);
	return true;

	/* Obsolete (Branruz)
	int newspell = 24924 + RandomUInt(3);

	SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
	if(!spInfo) return true;

	pSpell->p_caster->CastSpell(pSpell->p_caster, spInfo, true);
	return true;
*/
	
}

// -----------------------------------------------------------------------------

bool DeviateFish(uint32 i, Spell * pSpell)
{
	if(!pSpell->m_caster->IsPlayer()) return true;

	int newspell = 8064 + RandomUInt(4);

	SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
	if(!spInfo) return true;

	((Player *)pSpell->m_caster)->CastSpell(((Player *)pSpell->m_caster), spInfo, true);
	return true;
}

// -----------------------------------------------------------------------------

bool CookedDeviateFish(uint32 i, Spell * pSpell)
{
	if(!pSpell->m_caster->IsPlayer()) return true;

	int chance = 0;
	int newspell = 0;

	chance = RandomUInt(1);

	switch(chance)
	{

	/*
	#define __Flip_Out				8219
    #define __Flip_Out___0			8220
	*/
	default:
	case 0:
		newspell = __Flip_Out; // Flip Out (60 min) (turns you into a ninja)
		break;

		/*
		#define __Yaaarrrr				8221
		#define __Yaaarrrr___0			8222
		*/
	case 1:
		newspell = __Yaaarrrr; // Yaaarrrr (60 min) (turns you into a pirate)
		break;
	}

	if(newspell)
	{
		SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
		if(!spInfo) return true;

		((Player *)pSpell->m_caster)->CastSpell(((Player *)pSpell->m_caster), spInfo, true);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool HolidayCheer(uint32 i, Spell * pSpell)
{
	if(!pSpell->m_caster) return true;

	Unit *target;
	float dist = pSpell->GetRadius(i);

	for(ObjectSet::iterator itr = pSpell->m_caster->GetInRangeSetBegin(); itr != pSpell->m_caster->GetInRangeSetEnd(); ++itr)
	{
		if((*itr)->IsUnit())
			target = (Unit*)*itr;
		else
			continue;

		if(pSpell->m_caster->CalcDistance(target) > dist || isAttackable(pSpell->m_caster, target))
			continue;

		target->Emote(EMOTE_ONESHOT_LAUGH);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool NetOMatic(uint32 i, Spell * pSpell)
{
	/*
	#define __Net_o_Matic				13099
	#define __Net_o_Matic___0			13119
	#define __Net_o_Matic___1			13120
	#define __Net_o_Matic___2			13138
	#define __Net_o_Matic___3			13139
	#define __Net_o_Matic___4			16566
	*/
	Unit *target = pSpell->GetUnitTarget();
	if(!pSpell->m_caster->IsPlayer() || !target) return true;

	SpellEntry *spInfo = dbcSpell.LookupEntry(__Net_o_Matic); // 13099
	if(!spInfo) return true;

	int chance = RandomUInt(99)+1;

	if(chance < 51) // nets target: 50%
		((Player *)pSpell->m_caster)->CastSpell(target, spInfo, true);

	else if(chance < 76) // nets you instead: 25%
		((Player *)pSpell->m_caster)->CastSpell(((Player *)pSpell->m_caster), spInfo, true);

	else // nets you and target: 25%
	{
		((Player *)pSpell->m_caster)->CastSpell(((Player *)pSpell->m_caster), spInfo, true);
		((Player *)pSpell->m_caster)->CastSpell(target, spInfo, true);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool BanishExile(uint32 i, Spell * pSpell)
{
	Unit* target = pSpell->GetUnitTarget();
	if(!pSpell->m_caster->IsPlayer() || !target) return true;

	((Player *)pSpell->m_caster)->SpellNonMeleeDamageLog(target, pSpell->m_spellInfo->Id, target->GetUInt32Value(UNIT_FIELD_HEALTH), true);
	return true;
}

// -----------------------------------------------------------------------------

bool ForemansBlackjack(uint32 i, Spell *pSpell)
{
	Unit* target = pSpell->GetUnitTarget();
	if(!pSpell->m_caster->IsPlayer() || !target || target->GetTypeId() != TYPEID_UNIT) return true;

	// play sound and animation
	WorldPacket data(SMSG_PLAY_OBJECT_SOUND, 12);
	data << uint32(6197) << target->GetGUID();
	((Player *)pSpell->m_caster)->SendMessageToSet(&data, true);

	// send chat message
	char msg[100];
	sprintf(msg, "Ow! Ok, Je reviens travailler, %s", ((Player *)pSpell->m_caster)->GetName());
	target->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg);

	Creature* c_target = (Creature*)target;
	if(c_target->GetEntry() != 10556 || !c_target->HasAura(17743))
		return true;

	// Le pnj peut se déplacer
	if(target->GetAIInterface())
		target->GetAIInterface()->StopMovement(0);

	// On enlève les auras du pnj
	c_target->RemoveAllAuras();

	// Ajout d'un timer sur les péons qui se rendorment après 5-10 minutes
	SpellEntry* pSpellEntry = dbcSpell.LookupEntry(17743);
	sEventMgr.AddEvent(target , &Unit::EventCastSpell , target , pSpellEntry , EVENT_UNK, 300000 + RandomUInt(300000) , 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

	return true;
}

// -----------------------------------------------------------------------------

bool NetherWraithBeacon(uint32 i, Spell *pSpell)
{
	if(!pSpell->m_caster->IsPlayer()) return true;

	float SSX = ((Player *)pSpell->m_caster)->GetPositionX();
	float SSY = ((Player *)pSpell->m_caster)->GetPositionY();
	float SSZ = ((Player *)pSpell->m_caster)->GetPositionZ();
	float SSO = ((Player *)pSpell->m_caster)->GetOrientation();

	((Player *)pSpell->m_caster)->GetMapMgr()->GetInterface()->SpawnCreature(22408, SSX, SSY, SSZ, SSO, true, false, 0, 0);
	return true;
}

// -----------------------------------------------------------------------------

bool SymbolOfLife(uint32 i, Spell *pSpell)
{
	/*  // commented out until the SpellTargetDummy (38) targeting is fixed
	Unit* target = pSpell->GetUnitTarget();
	if(!pSpell->p_caster || !target || target->GetTypeId() != TYPEID_UNIT) return true;

	Creature* c_target = (Creature*)target;
	if(!c_target) return true;

	uint32 creatureID = c_target->GetEntry();

  // check to see we that have the correct creature and increment the quest log
	if(creatureID == 17542 || creatureID == 6172)
		sQuestMgr.OnPlayerKill(pSpell->p_caster, c_target);
	*/
	return true;
}

// -----------------------------------------------------------------------------

bool NighInvulnBelt(uint32 i, Spell *pSpell)
{
	if(!pSpell->m_caster->IsPlayer()) return true;

	int chance = RandomUInt(99)+1;
	/*
	#define __Nigh_Invulnerability				30456
	#define __Nigh_Invulnerability0				30458
	*/
	if(chance > 10) // Buff - Nigh-Invulnerability - 30456 
		((Player *)pSpell->m_caster)->CastSpell(((Player *)pSpell->m_caster), dbcSpell.LookupEntry(__Nigh_Invulnerability), true); // 30456
	else // Malfunction - Complete Vulnerability - 30457
		((Player *)pSpell->m_caster)->CastSpell(((Player *)pSpell->m_caster), dbcSpell.LookupEntry(__Complete_Vulnerability), true); // 15902

	return true;
}

// -----------------------------------------------------------------------------

bool ReindeerTransformation(uint32 i, Spell *pSpell)
{
	if(!pSpell->m_caster->IsPlayer()) return true;

	if(((Player *)pSpell->m_caster)->GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID) != 0)
	{
		if(((Player *)pSpell->m_caster)->m_setflycheat)
			((Player *)pSpell->m_caster)->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 22724);
		else
			((Player *)pSpell->m_caster)->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 15902);
	}
	return true;
}

// -----------------------------------------------------------------------------

bool SummonCritterDummy(uint32 i, Spell *pSpell)
{
	// the reason these spells have to be scripted is because they require a
	// reagent to summon the critter pet, but don't require one to dismiss it

	if(!pSpell->m_caster->IsPlayer()) return true;

	uint32 currentCritterID = 0;

	if(((Player *)pSpell->m_caster)->critterPet && ((Player *)pSpell->m_caster)->critterPet->GetCreatureName())
		currentCritterID = ((Player *)pSpell->m_caster)->critterPet->GetCreatureName()->Id;

	uint32 newspell = 0;

	switch(pSpell->m_spellInfo->Id)
	{
		case 26469: // Snowman Kit
		{
			if(currentCritterID == 15710) // do we already have this critter summoned?
				newspell = 26468; // if so, dismiss it
			else
				newspell = 26045; // otherwise summon it
		}	break;

		case 26528: // Jingling Bell
		{
			if(currentCritterID == 15706) // do we already have this critter summoned?
				newspell = 26530; // if so, dismiss it
			else
				newspell = 26529; // otherwise summon it
		}	break;

		case 26532: // Green Helper Box
		{
			if(currentCritterID == 15698) // do we already have this critter summoned?
				newspell = 26534; // if so, dismiss it
			else
				newspell = 26533; // otherwise summon it
		}	break;

		case 26541: // Red Helper Box
		{
			if(currentCritterID == 15705) // do we already have this critter summoned?
				newspell = 26537; // if so, dismiss it
			else
				newspell = 26536; // otherwise summon it
		}	break;
	}

	SpellEntry *spInfo = dbcSpell.LookupEntry(newspell);
	if(!spInfo) return true;

	((Player *)pSpell->m_caster)->CastSpell(((Player *)pSpell->m_caster), spInfo, false); // these spells have to check items, so "triggeredspell" must be false
	return true;
}

// -----------------------------------------------------------------------------

bool WinterWondervolt(uint32 i, Spell * pSpell)
{
	Unit* target = pSpell->GetUnitTarget();

	if(!target || target->GetTypeId() != TYPEID_PLAYER) return true;

	/*
	#define __PX_238_Winter_Wondervolt				26157
	#define __PX_238_Winter_Wondervolt___0			26272
	#define __PX_238_Winter_Wondervolt___1			26273
	#define __PX_238_Winter_Wondervolt___2			26274
	*/
	SpellEntry *spInfo = dbcSpell.LookupEntry(__PX_238_Winter_Wondervolt____2);
	if(!spInfo) return true;
	target->CastSpell(target,spInfo, true); // 26274

	return true;
}

// -----------------------------------------------------------------------------

bool WinterWondervoltAura(uint32 i, Aura* pAura, bool apply)
{
	Unit *u_caster = pAura->GetUnitCaster();

	if(!u_caster || !u_caster->IsPlayer()) return true;

	if(apply)
	{
		uint32 displayId;
		uint32 chance = RandomUInt(7);

		if(u_caster->getGender() == 1) displayId = 15795 + chance; // female 0-7
		else if(chance == 0)           displayId = 15687;          // male   0
		else                           displayId = 15802 + chance; // male   1-7

		u_caster->SetUInt32Value(UNIT_FIELD_DISPLAYID, displayId);
	}
	else
		u_caster->SetUInt32Value(UNIT_FIELD_DISPLAYID, u_caster->GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID));

	return true;
}

// -----------------------------------------------------------------------------

bool DisciplinaryRod(uint32 i, Spell * pSpell)
{
	Unit* target = pSpell->GetUnitTarget();
	if(!pSpell->m_caster->IsPlayer() || !target || target->GetTypeId() != TYPEID_UNIT) return true;

	Creature* c_target = (Creature*)target;
	if(!c_target) return true;

	uint32 creatureID = c_target->GetEntry();
	
	if(creatureID == 15945 || creatureID == 15941)
		sQuestMgr.OnPlayerKill(((Player *)pSpell->m_caster), c_target);

	return true;
}


// ADD NEW FUNCTIONS ABOVE THIS LINE
// *****************************************************************************

void SetupItemSpells_1(ScriptMgr * mgr)
{
	mgr->register_dummy_spell(__Gnomish_Transporter , &GnomishTransporter);      // Gnomish Transporter 23453
	mgr->register_dummy_spell(__Noggenfogger_Elixir , &NoggenFoggerElixr);       // Noggenfogger 16589
	/*
	#define __Hallow_s_End_Candy				24924
	#define __Hallow_s_End_Candy___0			24925
	#define __Hallow_s_End_Candy___1			24926
	#define __Hallow_s_End_Candy___2			24927
	#define __Hallow_s_End_Candy___3			24930
	*/
	mgr->register_dummy_spell(__Hallow_s_End_Candy , &HallowsEndCandy);         // Hallows End Candy 24430
	mgr->register_dummy_spell(__Deviate_Fish , &DeviateFish);             // Deviate Fish 8063
	mgr->register_dummy_spell(__Cooked_Deviate_Fish , &CookedDeviateFish);       // Savory Deviate Delight 8213
	mgr->register_dummy_spell(__Holiday_Cheer , &HolidayCheer);            // Holiday Cheer 26074
	/*
	#define __Net_o_Matic				13099
	#define __Net_o_Matic___0			13119
	#define __Net_o_Matic___1			13120
	#define __Net_o_Matic___2			13138
	#define __Net_o_Matic___3			13139
	#define __Net_o_Matic___4			16566
	*/
	mgr->register_dummy_spell(__Net_o_Matic , &NetOMatic);               // Net-o-Matic 13120
	/*
	#define __Banish_Burning_Exile				4130
	#define __Banish_Cresting_Exile				4131
	#define __Banish_Dragon_Helper				31550
	#define __Banish_Scourge_Crystal				58658
	#define __Banish_Scourge_Crystal_Trigger				58662
	#define __Banished_from_Shattrath_City				36642
	#define __Banished_from_Shattrath_City___0				36671
	*/
	mgr->register_dummy_spell(__Banish_Burning_Exile , &BanishExile);     // 4130 Essence of the Exile Quest 
	mgr->register_dummy_spell(__Banish_Cresting_Exile , &BanishExile);    // 4131 Essence of the Exile Quest 
	mgr->register_dummy_spell(__Banish_Thundering_Exile , &BanishExile);  // 4132 Essence of the Exile Quest 
	mgr->register_dummy_spell(__Awaken_Peon , &ForemansBlackjack);        // 19938 Lazy Peons Quest
	mgr->register_dummy_spell(__Activate_Nether_wraith_Beacon , &NetherWraithBeacon);      // 39105 Spellfire Tailor Quest
	mgr->register_dummy_spell(__Symbol_of_Life_Rank_1 , &SymbolOfLife);            // 8593 Paladin's Redemption QuestLine
	/*
	#define __Nigh_Invulnerability				30456
	//#define __Nigh_Invulnerability_I			30458
	#define __Nigh_Invulnerability2  			30458 // 332.11403
	*/
	mgr->register_dummy_spell(__Nigh_Invulnerability_0 , &NighInvulnBelt);          // 30458 Nigh Invulnerability Belt
	/*
	#define __Reindeer_Transformation				5154
	#define __Reindeer_Transformation___0			25860
	*/
	mgr->register_dummy_spell(__Reindeer_Transformation____0 , &ReindeerTransformation);  // 25860 Fresh Holly & Preserved Holly
	// mgr->register_dummy_spell( 26469, &SummonCritterDummy);      // Snowman Kit
	//mgr->register_dummy_spell( 26528, &SummonCritterDummy);      // Jingling Bell
	//mgr->register_dummy_spell( 26532, &SummonCritterDummy);      // Green Helper Box
	mgr->register_dummy_spell(__Winter_s_Little_Helper , &SummonCritterDummy);      // 26541 Red Helper Box
	mgr->register_dummy_spell(__PX_238_Winter_Wondervolt_TRAP , &WinterWondervolt);        // 26275 PX-238 Winter Wondervolt Trap
	/*
	#define __PX_238_Winter_Wondervolt				26157
	#define __PX_238_Winter_Wondervolt___0			26272
	#define __PX_238_Winter_Wondervolt___1			26273
	#define __PX_238_Winter_Wondervolt___2			26274
	*/
	mgr->register_dummy_aura(__PX_238_Winter_Wondervolt____2  , &WinterWondervoltAura);    // 26274 PX-238 Winter Wondervolt Transform Aura
	
	mgr->register_dummy_spell(27907, &DisciplinaryRod);    // 27907 Disciplinary Rod Transform Aura


// REGISTER NEW DUMMY SPELLS ABOVE THIS LINE
// *****************************************************************************
}
