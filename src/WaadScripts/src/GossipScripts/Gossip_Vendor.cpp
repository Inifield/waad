/*
 * D'apres un Lua de la base de donnée Eiffel, Trunk/lua
 * Posé sur la GossipScript (Waad Team, Branruz)
 * 
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
 */

#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305)		// warning C4305: 'argument' : truncation from 'double' to 'float'
#endif


#define cry_delay  25000

char MsgPain1[] = "Pain frais !";
char MsgPain2[] = "Pain frais, cuit ce matin !";
char MsgPain3[] = "Venez prendre votre pain frais !";
char MsgFruit1[] = "Croquez dans une pomme fraiche et mure.";
char MsgFruit2[] = "Fruits juteux a vendre !";
char MsgFruit3[] = "Les fruits frais en provenance de la foret d'Elwyn sont a votre droite.";

/************************************************************************/
/* Vendeur de pain                                                      */
/************************************************************************/
class VendeurDePain : public CreatureAIScript
{
 public:
    ADD_CREATURE_FACTORY_FUNCTION(VendeurDePain);

	VendeurDePain(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent(cry_delay);
	}

	void AIUpdate()
    {
		if(!_unit) 
		{
			Log.Notice("Vendeur de Pain","Probleme Script, Unit inconnu!");
			return;
		}

		uint32 val = RandomUInt(3);
        switch(val)
		{
   		 default :
		 case 1  : 
			 _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, MsgPain1);
			 _unit->Emote(EMOTE_ONESHOT_EXCLAMATION);
			 
           break;

		 case 2  : 
			 _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, MsgPain2);
			 _unit->Emote(EMOTE_ONESHOT_EXCLAMATION);
           break;

         case 3  : 
			 _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, MsgPain3);
			 _unit->Emote(EMOTE_ONESHOT_EXCLAMATION);
           break;

		}
    }
protected:
};

void SetupVendeurDePain(ScriptMgr * mgr)
{
	mgr->register_creature_script(1670, &VendeurDePain::Create);
	mgr->register_creature_script(5109, &VendeurDePain::Create);
	mgr->register_creature_script(3003, &VendeurDePain::Create);
	mgr->register_creature_script(3480, &VendeurDePain::Create);
	mgr->register_creature_script(3518, &VendeurDePain::Create);
	mgr->register_creature_script(3883, &VendeurDePain::Create);
	mgr->register_creature_script(3884, &VendeurDePain::Create);
	mgr->register_creature_script(3937, &VendeurDePain::Create);
	mgr->register_creature_script(3948, &VendeurDePain::Create);
	mgr->register_creature_script(3959, &VendeurDePain::Create);
	mgr->register_creature_script(4190, &VendeurDePain::Create);
	mgr->register_creature_script(8307, &VendeurDePain::Create);
	mgr->register_creature_script(19664, &VendeurDePain::Create);
	mgr->register_creature_script(23603, &VendeurDePain::Create);
	mgr->register_creature_script(23522, &VendeurDePain::Create);
}
/************************************************************************/
/* Vendeur de pain                                                      */
/************************************************************************/
class VendeurDeFruit : public CreatureAIScript
{
 public:
    ADD_CREATURE_FACTORY_FUNCTION(VendeurDeFruit);

	VendeurDeFruit(Creature* pCreature) : CreatureAIScript(pCreature)
    {
		RegisterAIUpdateEvent(cry_delay);
	}

	void AIUpdate()
    {
		if(!_unit) 
		{
			Log.Notice("Vendeur de Fruit","Probleme Script, Unit inconnu!");
			return;
		}

		uint32 val = RandomUInt(3);
        switch(val)
		{
   		 default :
		 case 1  : 
			 _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, MsgFruit1);
			 _unit->Emote(EMOTE_ONESHOT_TALK);
           break;

		 case 2  : 
			 _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, MsgFruit2);
			 _unit->Emote(EMOTE_ONESHOT_EXCLAMATION);
           break;

         case 3  : 
			 _unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, MsgFruit3);
			 _unit->Emote(EMOTE_ONESHOT_TALK);
           break;

		}
    }
protected:
};

void SetupVendeurDeFruit(ScriptMgr * mgr)
{
	mgr->register_creature_script(7978, &VendeurDeFruit::Create);
	mgr->register_creature_script(3017, &VendeurDeFruit::Create);
	mgr->register_creature_script(3342, &VendeurDeFruit::Create);
	mgr->register_creature_script(7978, &VendeurDeFruit::Create);
	mgr->register_creature_script(19223, &VendeurDeFruit::Create);
	mgr->register_creature_script(29547, &VendeurDeFruit::Create);
}