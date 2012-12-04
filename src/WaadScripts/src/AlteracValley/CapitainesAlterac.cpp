/* Scripts du champ de bataille de la vallé d'Alterac par Randdrick pour la communauté Waad
Basé sur le travail de la team Moon++  - Projet Moon -
Soumis à la licence Creative Common - Paternité - pas d'utilisation commerciale -
Pour en savoir plus : http://creativecommons.org/licenses/by-nc/2.0/fr/
*/

#include "StdAfx.h"
#include "Setup.h"
#include "Base.h"

//Scripts capitaines Alterac

#define BOSS_BALINDA_STONEHEARTH 11949 // Capitaine Balinda Gîtepierre
#define BALINDA_STONEHEARTH_FIREBALL 46988 // Boule de feu
#define BALINDA_STONEHEARTH_CONEOFCOLD 38384 // Cone de froid
#define BALINDA_STONEHEARTH_FROSTBOLT 46987 // Eclair de givre
#define BALINDA_STONEHEARTH_ARCANESEXPLOSION 46608 // Explosion des Arcanes
#define BALINDA_STONEHEARTH_SUMMONWATER 45067 // Invocation élémentaire d'eau
#define BALINDA_STONEHEARTH_POLYMORPH 12826 // Polymorphie - Je transforme tout le monde en sheep !!!
#define BALINDA_STONEHEARTH_SLOW 31589 //Lenteur

class BalindaStonehearthAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(BalindaStonehearthAI, MoonScriptBossAI);
	BalindaStonehearthAI(Creature* pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(BALINDA_STONEHEARTH_FIREBALL, Target_RandomPlayer, 100, 2.5, 0, 40, 40);
		AddSpell(BALINDA_STONEHEARTH_CONEOFCOLD, Target_Self, 20, 0, 0, 0, 0);
		AddSpell(BALINDA_STONEHEARTH_FROSTBOLT, Target_RandomPlayer, 20, 2.5, 0, 0, 40); 
		AddSpell(BALINDA_STONEHEARTH_ARCANESEXPLOSION, Target_Self, 40, 2, 0, 0, 40);
		AddSpell(BALINDA_STONEHEARTH_SUMMONWATER, Target_Self, 20, 0, 0, 0, 0);
		AddSpell(BALINDA_STONEHEARTH_POLYMORPH, Target_RandomPlayerNotCurrent, 40, 1.5, 0, 0, 30);
		AddSpell(BALINDA_STONEHEARTH_SLOW, Target_RandomPlayer, 10, 0, 0, 0, 30);
	}
};

#define BOSS_CAPTAIN_GALVANGAR 11947 // Capitaine Galvangar - Capitaine Loup-de-givre
#define GALVANGAR_FIGHTENINGSHOUT 19134 // Cri effrayant
#define GALVANGAR_CLEAVE 15284 // Enchainemment
#define GALVANGAR_MORTALSTRIKE 16856 // Frappe Mortelle
#define GALVANGAR_WHIRLWIND 15589 // Tourbillon
#define GALVANGAR_WHIRLWIND_EX 13736 // Tourbillon

class CaptainGalvangarAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CaptainGalvangarAI, MoonScriptBossAI);
	CaptainGalvangarAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(GALVANGAR_FIGHTENINGSHOUT, Target_RandomPlayer, 20, 0, 0, 0, 10);
		AddSpell(GALVANGAR_CLEAVE, Target_RandomPlayer, 20, 0, 0, 0, 5);
		AddSpell(GALVANGAR_MORTALSTRIKE, Target_RandomPlayer, 20, 0, 0, 0, 5);
		AddSpell(GALVANGAR_WHIRLWIND, Target_Self, 20, 0, 0, 0, 0);
		AddSpell(GALVANGAR_WHIRLWIND_EX, Target_Self, 20, 0, 0, 0, 0);
	}
};

void SetupCapitainesAlterac(ScriptMgr * pScriptMgr)
{
	pScriptMgr->register_creature_script(BOSS_BALINDA_STONEHEARTH, &BalindaStonehearthAI::Create);
	pScriptMgr->register_creature_script(BOSS_CAPTAIN_GALVANGAR, &CaptainGalvangarAI::Create) ;
}