// De Doowi, trouvé sur zone-Emu
// REBØRN TEAM 2009 ©
//
/*
- Menu d'or [ vous pouvez retirer de 100 a 50000 Pièces d'or ]
- Menu TP Zone pour MJ [ Vieux iF, Ile des MJ, Boite a MJ ]
- Menu du Heal [ le NPC vous remonteras tout vos points de vie ]
- Maxskill [ UP toutes vos competence a 350 (70) modifiable pour level 80 ]
- Menu Materiel des MJ [ a savoir tete, robe et botte de maître de jeux official set ]
- Suppression de mal de rez [ au cas ou Langue ]
- TOTALEMENT en Français

*/

#define ID_NPC_ASSISTANCE	500001

#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305)// warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

class SCRIPT_DECL GMHelper : public GossipScript
{
 public:
   void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
   void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
   void GossipEnd(Object * pObject, Player* Plr);
   void Destroy()
   {
    delete this;
   }
};
void GMHelper::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
 GossipMenu *Menu;
 objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
 if (!Plr->GetSession()->HasGMPermissions())
 {
  Plr->BroadcastMessage("Vous n'etes pas Maitre de jeux.");
  Menu->AddItem(0, "Vous n'etes pas Maitre de jeux vous ne pouvez pas utiliser ce menu!", 0);
 }
 if (Plr->GetSession()->HasGMPermissions())
 {
  Plr->BroadcastMessage("Menu des Maitre de jeux");
  Menu->AddItem(6, "Retirez de l argent"          , 1);
  Menu->AddItem(0, "Menu TP zone pour MJ"         , 100);
  Menu->AddItem(0, "Healez Moi"                   , 5);
  Menu->AddItem(0, "Toute mes competences aux max", 6);
  Menu->AddItem(0, "Donnez-moi le Materiel MJ"    , 7);
  Menu->AddItem(0, "Enlevez le mal de resurection", 8);
 }
 if(AutoSend) Menu->SendTo(Plr);
}

void GMHelper::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
 uint32 currentgold,newgold;

 Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
 if(pCreature==NULL) return;

 GossipMenu * Menu;
 switch(IntId)
 {
  case 0: GossipHello(pObject, Plr, true); 
	      break;

  case 1: // MJ PO MENU
   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
   Menu->AddItem(6, "10 piece d or"   ,  9);
   Menu->AddItem(6, "100 piece d or"  , 10);
   Menu->AddItem(6, "1000 piece d or" , 11);
   Menu->AddItem(6, "5000 piece d or" , 12);
   Menu->AddItem(6, "10000 piece d or", 13);
   Menu->SendTo(Plr);
   break;

  case 100: // Teleportation Menu
   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
   Menu->AddItem(0, "Boite des MJ"     , 101); //7
   Menu->AddItem(0, "Ile des MJ"       , 102); //7
   Menu->AddItem(0, "Vieux Ironforge"  , 105); //7
   Menu->SendTo(Plr);
   break;

  case 5: // FULL VIE
   pCreature->CastSpell(Plr, 25840, 0);
   Plr->BroadcastMessage("Vous etes maintenant au maximum de vos points de vie!");
   Plr->Gossip_Complete();
   break;

  case 6: // MAXSKILL => A corriger en fonction du level Max (Branruz)
   if (Plr->_HasSkillLine(43))  { Plr->_AdvanceSkillLine(43, 350); }
   if (Plr->_HasSkillLine(55))  { Plr->_AdvanceSkillLine(55, 350); }
   if (Plr->_HasSkillLine(44))  { Plr->_AdvanceSkillLine(44, 350); }
   if (Plr->_HasSkillLine(95))  { Plr->_AdvanceSkillLine(95, 350); }
   if (Plr->_HasSkillLine(54))  { Plr->_AdvanceSkillLine(54, 350); }
   if (Plr->_HasSkillLine(45))  { Plr->_AdvanceSkillLine(45, 350); }
   if (Plr->_HasSkillLine(46))  { Plr->_AdvanceSkillLine(46, 350); }
   if (Plr->_HasSkillLine(136)) { Plr->_AdvanceSkillLine(136, 350); }
   if (Plr->_HasSkillLine(160)) { Plr->_AdvanceSkillLine(160, 350); }
   if (Plr->_HasSkillLine(162)) { Plr->_AdvanceSkillLine(162, 350); }
   if (Plr->_HasSkillLine(172)) { Plr->_AdvanceSkillLine(172, 350); }
   if (Plr->_HasSkillLine(173)) { Plr->_AdvanceSkillLine(173, 350); }
   if (Plr->_HasSkillLine(176)) { Plr->_AdvanceSkillLine(176, 350); }
   if (Plr->_HasSkillLine(226)) { Plr->_AdvanceSkillLine(226, 350); }
   if (Plr->_HasSkillLine(228)) { Plr->_AdvanceSkillLine(228, 350); }
   if (Plr->_HasSkillLine(229)) { Plr->_AdvanceSkillLine(229, 350); }
   if (Plr->_HasSkillLine(473)) { Plr->_AdvanceSkillLine(473, 350); }
   Plr->BroadcastMessage("Toutes vos competences sont maintenant avancees a 350");
   Plr->Gossip_Complete();
   break;

  case 7: // MJ SET
	  {
       Item *pItem1 = objmgr.CreateItem(2586,Plr);
	   if(pItem1 != NULL) Plr->GetItemInterface()->AddItemToFreeSlot(pItem1);
       Item *pItem2 = objmgr.CreateItem(11508,Plr);
       if(pItem2 != NULL) Plr->GetItemInterface()->AddItemToFreeSlot(pItem2);
       Item *pItem3 = objmgr.CreateItem(12064,Plr);
       if(pItem3 != NULL) Plr->GetItemInterface()->AddItemToFreeSlot(pItem3);
       Plr->Gossip_Complete();
	  }
      break;

  case 8: // Suppresion Mal De Rez
   Plr->addSpell(15007);
   Plr->removeSpell(15007,0,0,0);
   Plr->Gossip_Complete();
   break;

   // Correction des montants: 100pb = 1pa, 100pa = 1po
  case 9: // 10 pièces d'or
   currentgold = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
   newgold = currentgold + 100000;
   Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newgold);
   Plr->Gossip_Complete();
   break;

  case 10: //100 pièces d'or
   currentgold = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
   newgold = currentgold + 1000000;
   Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newgold);
   Plr->Gossip_Complete();
   break;

  case 11: //1000 pièces d'or
   currentgold = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
   newgold = currentgold + 10000000;
   Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newgold);
   Plr->Gossip_Complete();
   break;

 case 12: //5000 pièces d'or
  currentgold = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
  newgold = currentgold + 50000000;
  Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newgold);
  Plr->Gossip_Complete();
  break;

 case 13: //10000 pièce d'or
  currentgold = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
  newgold = currentgold + 100000000;
  Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newgold);
  Plr->Gossip_Complete();
  break;

 case 101: //Teleport MJ BOITE
  Plr->EventTeleport(1, 16221, 16416, -64);
  break;

 case 102: //Teleport Ile Des MJ
  Plr->EventTeleport(1, 16222.1, 16252.1, 12.5872);
  break;

 case 105: //Teleport  Vieux iF
  Plr->EventTeleport(0, -4843.0874, -1066.9552, 500.9893);
  break;
 }
}

void GMHelper::GossipEnd(Object * pObject, Player* Plr)
{
 GossipScript::GossipEnd(pObject, Plr);
}

void SetupGMHelper(ScriptMgr * mgr)
{
 GossipScript * gs = (GossipScript*) new GMHelper();
 if( gs != NULL) mgr->register_gossip_script(ID_NPC_ASSISTANCE, gs);
}