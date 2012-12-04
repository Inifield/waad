// De Balthazar

#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305) // warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

#define INFORMATION_NPC 600010

/* VERIF SQL:
INSERT INTO `creature_names` VALUES ('600010', 'Laryllan', 'Pnj Information', '', '1', '7', '0', '0', '0', null, '16683', '0', '0', '0', '2', '1', '1', '0');
INSERT INTO `creature_proto` VALUES ('600010', '85', '85', '35', '100', '100', '10000', '1', '1', '1800', '0', '600', '900', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '360000', '9999', '0', '0', '0', '0', '0', '0', '2', '2', '', '0', '0', '0', '0', '2.5', '8', '14', '0');
*/
class SCRIPT_DECL InformationNPC : public GossipScript
{
    public:
    void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
    void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
    void GossipEnd(Object * pObject, Player* Plr);
    void Destroy()
    {delete this;}
};
  
void InformationNPC::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
    GossipMenu *Menu;
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

    Menu->AddItem(0, "Bienvenue", 1);
    Menu->AddItem(0, "Les R�gles fondamentales du serveur", 2);
    Menu->AddItem(0, "Les Diff�rentes sanctions.", 3);
    Menu->AddItem(0, "Ce que les MJs peuvent faire et ne pas faire pour vous.", 4);
    Menu->AddItem(0, "Le Systeme de vote", 5);
    Menu->AddItem(0, "Le Forum", 6);
    Menu->AddItem(0, "La Boutique", 7);

    if(AutoSend)
    Menu->SendTo(Plr);
}

void InformationNPC::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{        
    Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
    if(pObject==NULL)
    return;

    switch(IntId)
    {
        case 0: // Retour
        {
            GossipHello(pObject, Plr, true);
        }    
        break;

        case 1: // Bienvenue
        {
            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

            Menu->AddItem(0, "Bienvenue sur ce serveur compos� d'une database et d'un core d�velopper par la team Ax0ne pour un plus grand confort et plaisir de jeu.", 8);
            Menu->AddItem(0, "Niveau maximum 150", 8);
            Menu->AddItem(0, "Instances personalis�es", 8);
            Menu->AddItem(0, "Deux zones d'achats", 8);
            Menu->AddItem(0, "Zone d'XP personalis�es jusqu'au niveaux 250", 8);
            Menu->AddItem(0, "Items personalis�s jusqu'au 150++", 8);
            Menu->AddItem(0, "Et plein d'autre choses a d�couvrir.", 8);
            Menu->AddItem(0, "Le serveur est en constante �volution, toute l'�quipes vous souhaite un bon jeux.", 8);
            Menu->AddItem(0, "[Retour].", 99);
            Menu->SendTo(Plr);
        }    
        break;

        case 2: // R�gles
        {
            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
                
            Menu->AddItem(0, "Il est interdit d'insulter un joueur ou un MJ", 9);
            Menu->AddItem(0, "Vous devez toujours adopter un comportement mature.", 9);
            Menu->AddItem(0, "Ne pas faire l'enfant lors d'une �ventuelle situation de d�bat.", 9);
            Menu->AddItem(0, "Tout actes de triche sur le serveur est passible d'un bannissement d�finitif", 9);
            Menu->AddItem(0, "Toute utilisation d'un addon sur le serveur est interdite, sinon des sanctions seront prises.", 9);
            Menu->AddItem(0, "Toute exploitation de bugs est passible d'un avertisement et si non �coute de graves sanctions", 9);
            Menu->AddItem(0, "Tout bugs rencontrer doit �tre signaler sur le forum", 9);
            Menu->AddItem(0, "PVP interdit dans les zones d'achats est les zones d'XP", 9);
            Menu->AddItem(0, "PVP autoris� dans les Ar�nes", 9);
            Menu->AddItem(0, "[Retour].", 99);
            Menu->SendTo(Plr);
        }    
        break;

        case 3: // Sanction
        {
            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

            Menu->AddItem(0, "Bannissement d�finitif d'un joueur (Voir suppression du compte).", 10);
            Menu->AddItem(0, "Bannissement d'un joueur a dur� d�termin�.", 10);
            Menu->AddItem(0, "Suppression de personnages.", 10);
            Menu->AddItem(0, "Bannissement de quelques heures ou d'un jour.", 10);
            Menu->AddItem(0, "Avertissement avant sanctions.", 10);
            Menu->AddItem(0, "[Retour].", 99);
            Menu->SendTo(Plr);
        }
        break;

        case 4: // MJ
        {
            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

            Menu->AddItem(0, "Ils ne peuvent pas vous donner de niveau.", 11);
            Menu->AddItem(0, "Ils ne peuvent pas vous donner d'or.", 11);
            Menu->AddItem(0, "Ils ne peuvent pas vous donner d'item (sauf par la Boutique).", 11);
            Menu->AddItem(0, "Les Admin et les MJs sont la pour vous aider en cas de besoin dans le jeu", 11);
            Menu->AddItem(0, "Ils peuvent vous r�ssuciter en cas de bug de r�surrection. Ou pendant un �vent organis�", 11);
            Menu->AddItem(0, "Ils peuvent vous t�l�porter uniquement si vous �tes bloqu�. (Sous la map ou autre.)", 11);
            Menu->AddItem(0, "Ils peuvent r�soudre un conflit entre plusieurs joueurs.", 11);
            Menu->AddItem(0, "[Retour].", 99);
            Menu->SendTo(Plr);
        }    
        break;

        case 5: // vote
        {
            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

            Menu->AddItem(0, "Un syst�me de vote par nom de compte est pr�sent sur le site, permet d'augmenter la population du serveur et r�compence les meilleurs voteurs", 12);
            Menu->AddItem(0, "[Retour].", 99);
            Menu->SendTo(Plr);
        }
        break;

        case 6: // Forum
        {
            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

			Menu->AddItem(0, "URL du forum: http://", 13);
			Menu->AddItem(0, "Un forum est a votre disposition.", 13);
            Menu->AddItem(0, "Pour mettre vos screens en cas de probl�mes rencontr�s en jeu.", 13);
            Menu->AddItem(0, "Pour remonter les informations de bugs.", 13);
            Menu->AddItem(0, "Pour proposer des am�liorations en jeu", 13);
            Menu->AddItem(0, "[Retour].", 99);
            Menu->SendTo(Plr);
        }
        break;

        case 7: // Boutique
        {
            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

            Menu->AddItem(0, "Une section sur le forum est r�server a la boutique, qui vous permet de avoir des items et des armes sp�ciale VIP pour am�liorer votre personnage et contribuer au bon maintient du serveur.", 14);
            Menu->AddItem(0, "[Retour].", 99);
            Menu->SendTo(Plr);
        }    
        break;

        // Message bienvenue
        case 8:    
        {
            pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Bon jeu!" );
            Plr->Gossip_Complete();    
        }    
        break;

        case 9:
        {
            pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Merci de respecter les r�gles, si vous ne voulez pas avoir affaire au sanctions!" );
            Plr->Gossip_Complete();    
        }    
        break;

        // Message Sanctions
        case 10:
        {
            pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Vous �tes au courant des mesures prise sur le serveur!" );
            Plr->Gossip_Complete();    
        }    
        break;

        // Message MJ
        case 11:
        {
            pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Merci de ne pas d�ranger les MJ pour rien!" );
            Plr->Gossip_Complete();    
        }    
        break;

        // Message vote
        case 12:
        {
            pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Merci d'allez voter!" );
            Plr->Gossip_Complete();    
        }    
        break;

        //Message Forum
        case 13:
        {
            pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "N'oubliez pas de vous inscrire!" );
            Plr->Gossip_Complete();    
        }    
        break;

        // Message Boutique
        case 14:
        {
            pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "N'oubliez pas de venir jetez un coup d'oeil!" );
            Plr->Gossip_Complete();    
        }    
        break;

        case 99: //Retour au Depart
        {
            GossipMenu *Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

            Menu->AddItem(0, "Bienvenue", 1);
            Menu->AddItem(0, "Les R�gles fondamentales du serveur", 2);
            Menu->AddItem(0, "Les Diff�rentes sanctions.", 3);
            Menu->AddItem(0, "Ce que les MJs peuvent faire et ne pas faire pour vous.", 4);
            Menu->AddItem(0, "Le Systeme de vote", 5);
            Menu->AddItem(0, "Le Forum", 6);
            Menu->AddItem(0, "La Boutique", 7);
            Menu->SendTo(Plr);
        }
        break;
    }
};

void InformationNPC::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

void SetupInformationNPC(ScriptMgr * mgr)
{
    GossipScript * gs = (GossipScript*) new InformationNPC();
    if(gs != NULL) mgr->register_gossip_script(INFORMATION_NPC, gs);
}


