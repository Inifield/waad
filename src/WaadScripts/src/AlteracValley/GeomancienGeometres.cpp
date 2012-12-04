/* Scripts du champ de bataille de la vall� d'Alterac par Randdrick pour la communaut� Waad
Bas� sur le travail de la team Moon++  - Projet Moon -
Soumis � la licence Creative Common - Paternit� - pas d'utilisation commerciale -
Pour en savoir plus : http://creativecommons.org/licenses/by-nc/2.0/fr/
*/

#include "StdAfx.h"
#include "Setup.h"
#include "Base.h"

//Scripts g�omancien et g�om�tres Alterac

#define BLANCHE_MOUSTACHE 13099 //G�omancien Blanche-moustache
#define FIRE_SHIELD 18968 // Bouclier de feu
#define FIREBALL 15228 // Boule de feu

class BLANCHEMOUSTACHEAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(BLANCHEMOUSTACHEAI, MoonScriptCreatureAI);
	BLANCHEMOUSTACHEAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(FIRE_SHIELD, Target_RandomFriendly, 40, 0, 0, 0, 0);
		AddSpell(FIREBALL, Target_RandomPlayer, 75, 3, 0, 0, 0);
	}
};

#define GEOMETRES_GOUFFREFER 13098 //G�om�trede Gouffrefer
#define GEOMETRES_GOUFFREFER_AGUERRI 13555 //G�om�tre  de Gouffrefer aguerri
#define GEOMETRES_GOUFFREFER_VETERAN 13556 //G�om�tre  de Gouffrefer v�t�ran
#define GEOMETRES_FROIDEMINE 13097 // G�om�tre  de Froidemine
#define GEOMETRES_FROIDEMINE_AGUERRI 13537 // G�om�tre  de Froidemine aguerri
#define GEOMETRES_FROIDEMINE_VETERAN 13538 // G�om�tre  de Froidemine v�t�ran

#define HOLY_SMITE 15498 //Ch�timent sacr�
#define SHADOW_WORD 15654 //Mot de l'ombre : Douleur
#define HEAL 15498 //Soins

class GEOMETRESAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(GEOMETRESAI, MoonScriptCreatureAI);
	GEOMETRESAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(HOLY_SMITE, Target_RandomPlayer, 40, 2.5, 0, 0, 40);
		AddSpell(SHADOW_WORD, Target_RandomPlayer, 20, 0, 0, 0, 40);
		AddSpell(HEAL, Target_RandomFriendly, 40, 3, 0, 0, 40);
	}
};

void SetupGeomanciensGeometres(ScriptMgr * pScriptMgr)
{
	pScriptMgr->register_creature_script(BLANCHE_MOUSTACHE, &BLANCHEMOUSTACHEAI::Create);
	pScriptMgr->register_creature_script(GEOMETRES_GOUFFREFER, &GEOMETRESAI::Create) ;
	pScriptMgr->register_creature_script(GEOMETRES_GOUFFREFER_AGUERRI, &GEOMETRESAI::Create) ;
	pScriptMgr->register_creature_script(GEOMETRES_GOUFFREFER_VETERAN, &GEOMETRESAI::Create) ;
	pScriptMgr->register_creature_script(GEOMETRES_FROIDEMINE, &GEOMETRESAI::Create) ;
	pScriptMgr->register_creature_script(GEOMETRES_FROIDEMINE_AGUERRI, &GEOMETRESAI::Create) ;
	pScriptMgr->register_creature_script(GEOMETRES_FROIDEMINE_VETERAN, &GEOMETRESAI::Create) ;
}