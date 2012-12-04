////////////////////////////////////////
//    PvPTitlesNPC Script Redefined   //
//       Developed By: Nemesis        //
//    © 2008 Arc Evolved Scripting    //
//          Don't Be A Jew!           //
//     Give Credit Where It's Due!    //
////////////////////////////////////////

#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305)
#endif
//////////////////////////
// SCRIPT CONFIGURATION //
//////////////////////////

//////////////////////////
//    HK Requiert   //
//////////////////////////
#define HK_1 100
#define HK_2 300
#define HK_3 600
#define HK_4 900
#define HK_5 1200
#define HK_6 1500
#define HK_7 1800
#define HK_8 2100
#define HK_9 2500
#define HK_10 3000
#define HK_11 6000
#define HK_12 10000
#define HK_13 15000
#define HK_14 20000
///////////////////////////
//   Or Requiert   //
///////////////////////////
#define Or_1 500000
#define Or_2 1000000
#define Or_3 1500000
#define Or_4 2000000
#define Or_5 2500000
#define Or_6 3000000
#define Or_7 3500000
#define Or_8 4000000
#define Or_9 5000000
#define Or_10 6500000
#define Or_11 8000000
#define Or_12 10000000
#define Or_13 15000000
#define Or_14 20000000
////////////////////////////
//    Or Info Setup     //
////////////////////////////
#define GINFO_1 50
#define GINFO_2 100
#define GINFO_3 150
#define GINFO_4 200
#define GINFO_5 250
#define GINFO_6 300
#define GINFO_7 350
#define GINFO_8 400
#define GINFO_9 500
#define GINFO_10 650
#define GINFO_11 800
#define GINFO_12 1000
#define GINFO_13 1500
#define GINFO_14 2000
////////////////////////////
//      Arena Points      //
////////////////////////////
#define ARENA_1 200
#define ARENA_2 400
#define ARENA_3 600
#define ARENA_4 800
#define ARENA_5 1000
#define ARENA_6 1500
#define ARENA_7 2000
////////////////////////////
// Personal Arena Rating  //
////////////////////////////
#define ARATING_1 1550
#define ARATING_2 1600
#define ARATING_3 1650
#define ARATING_4 1700
#define ARATING_5 1800
#define ARATING_6 1900
#define ARATING_7 2000
////////////////////////////
//         NPC ID         //
////////////////////////////
#define NPC_ID 500002

//// DO NOT EDIT ANYTHING BELOW UNLESS YOU KNOW WHAT YOUR DOING! YOU HAVE BEEN WARNED.////
class SCRIPT_DECL PvPTitlesNPC : public GossipScript
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
void PvPTitlesNPC::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
 GossipMenu *Menu;
 objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
 
 uint32 PlayerRace = Plr->getRace();
 switch(PlayerRace)
 {	
  // Ally
  case RACE_HUMAN    : // 1
  case RACE_DWARF    : // 3
  case RACE_NIGHTELF : // 4
  case RACE_GNOME    : // 7
  case RACE_DRAENEI  : // 11
	                  Menu->AddItem(0, "Titres JcJ de l'Alliance", 2);
	                  break;
  // Horde
  case RACE_ORC      : // 2
  case RACE_UNDEAD   : // 5
  case RACE_TAUREN   : // 6
  case RACE_TROLL    : // 8
  case RACE_BLOODELF : // 10
                     Menu->AddItem(0, "Titres JcJ de la Horde", 1);
	                 break;
	
  default: free(Menu); return;
 }
 Menu->AddItem(0, "Titres JcJ des Arenes", 32);
 Menu->AddItem(0, "Quelles sont les pre-requis pour les titres ?", 41);

 if(AutoSend) Menu->SendTo(Plr);
}

void PvPTitlesNPC::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
	uint32 m_killsLifeTime,currentOr;

    Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
    if(pCreature==NULL) return;

        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:
          GossipHello(pObject, Plr, true);
        break;

        case 1:    ////Horde Titles////
            {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(5, "Eclaireur"               , 4);
            Menu->AddItem(5, "Grunt"                   , 5);
            Menu->AddItem(5, "Sergent"                 , 6);
            Menu->AddItem(5, "Sergent Chef"            , 7);
            Menu->AddItem(5, "Adjudant"                , 8);
            Menu->AddItem(5, "Garde de pierre"         , 9);
            Menu->AddItem(5, "Garde de sang"           , 10);
            Menu->AddItem(5, "Legionnaire"             , 11);
            Menu->AddItem(5, "Centurion"               , 12);
            Menu->AddItem(5, "Champion"                , 13);
            Menu->AddItem(5, "Lieutenant General"      , 14);
            Menu->AddItem(5, "General"                 , 15);
            Menu->AddItem(5, "Seigneur de guerre"      , 16);
            Menu->AddItem(5, "Grand Seigneur de guerre", 17);
            Menu->SendTo(Plr);
            }break;

        case 2:     ////Alliance Titles////
            {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(5, "Soldat"               , 18);
            Menu->AddItem(5, "Caporal"              , 19);
            Menu->AddItem(5, "Sergent"              , 20);
            Menu->AddItem(5, "Sergent Chef"         , 21);
            Menu->AddItem(5, "Sergent Major"        , 22);
            Menu->AddItem(5, "Chevalier"            , 23);
            Menu->AddItem(5, "Chevalier lieutenant" , 24);
            Menu->AddItem(5, "Chevalier capitaine"  , 25);
            Menu->AddItem(5, "Chevalier champion"   , 26);
            Menu->AddItem(5, "Lieutenant commandant", 27);
            Menu->AddItem(5, "Commandant"           , 28);
            Menu->AddItem(5, "Marechal"             , 29);
            Menu->AddItem(5, "Grand Marechal"       , 30);
            Menu->AddItem(5, "Connetable"           , 31);
            Menu->SendTo(Plr);
            }break;

        case 4: ////Horde////Scout////
            m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_1)
			{
                if(Plr->HasKnownTitle(PVPTITLE_GRUNT) || Plr->HasKnownTitle(PVPTITLE_HSERGEANT)||Plr->HasKnownTitle(PVPTITLE_SENIOR_SERGEANT)||Plr->HasKnownTitle(PVPTITLE_FIRST_SERGEANT)||Plr->HasKnownTitle(PVPTITLE_STONE_GUARD)||Plr->HasKnownTitle(PVPTITLE_BLOOD_GUARD)||Plr->HasKnownTitle(PVPTITLE_LEGIONNAIRE)||Plr->HasKnownTitle(PVPTITLE_CENTURION)||Plr->HasKnownTitle(PVPTITLE_CHAMPION)||Plr->HasKnownTitle(PVPTITLE_LIEUTENANT_GENERAL)||Plr->HasKnownTitle(PVPTITLE_GENERAL)||Plr->HasKnownTitle(PVPTITLE_WARLORD)||Plr->HasKnownTitle(PVPTITLE_HIGH_WARLORD))
				{
                 Plr->BroadcastMessage("etes vous grand ou juste stupide?!");
                 Plr->Gossip_Complete();
                }
				else
				{
                 if(currentOr >= Or_1)
				 {
                  int32 newOr = currentOr - Or_1;
                  Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                  Plr->SetKnownTitle(PVPTITLE_SCOUT,true);
                  Plr->BroadcastMessage("Je vous nomme, %s Eclaireur de la Horde", Plr->GetName());
                  Plr->BroadcastMessage("Reconnectez vous pour activer votre Titre.");
                 }
				 else
				 {
                  Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                  Plr->Gossip_Complete();
                 }
				 break;
                }
				break;
            }
			else
			{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }
			break;
            

        case 5: ////Horde////Grunt////
            m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_2)
			{
                if(Plr->HasKnownTitle(PVPTITLE_SCOUT))
				{
                    if(currentOr>=Or_2)
					{
                        int32 newOr = currentOr - Or_2;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_SCOUT,false);
                        Plr->SetKnownTitle(PVPTITLE_GRUNT,true);
                        Plr->BroadcastMessage("Je vous nomme, Grunt %s de la Horde", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }
					else
					{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }
					break;
                }
				else
				{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }
				break;
            }
			else
			{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }
			break;
            
        case 6: ////Horde////Sergeant////
            m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_3)
			{
                if (Plr->HasKnownTitle(PVPTITLE_GRUNT))
				{
                    if(currentOr>=Or_3)
					{
                        int32 newOr = currentOr - Or_3;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_GRUNT,false);
                        Plr->SetKnownTitle(PVPTITLE_HSERGEANT,true);
                        Plr->BroadcastMessage("Je vous nomme, Sergent %s de la Horde", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }
					else
					{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }
					break;
                }
				else
				{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'acheter celui ci.");
                    Plr->Gossip_Complete();
                }
				break;
            }
			else
			{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }
			break;
            
        case 7: ////Horde////Senior Sergeant////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_4){
                if(Plr->HasKnownTitle(PVPTITLE_HSERGEANT)){
                    if(currentOr>=Or_4){
                        int32 newOr = currentOr - Or_4;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_HSERGEANT,false);
                        Plr->SetKnownTitle(PVPTITLE_SENIOR_SERGEANT,true);
                        Plr->BroadcastMessage("Je vous nomme, Sergent Chef %s de la Horde", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 8: ////Horde////First Sergeant////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_5){
                if(Plr->HasKnownTitle(PVPTITLE_SENIOR_SERGEANT)){
                    if(currentOr>=Or_5){
                        int32 newOr = currentOr - Or_5;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_SENIOR_SERGEANT,false);
                        Plr->SetKnownTitle(PVPTITLE_FIRST_SERGEANT,true);
                        Plr->BroadcastMessage("Je vous nomme, Adjudant %s de la Horde", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 9: ////Horde////Stone Guard////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_6){
                if(Plr->HasKnownTitle(PVPTITLE_FIRST_SERGEANT)){
                    if(currentOr>=Or_6){
                        int32 newOr = currentOr - Or_6;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_FIRST_SERGEANT,false);
                        Plr->SetKnownTitle(PVPTITLE_STONE_GUARD,true);
                        Plr->BroadcastMessage("Je vous nomme, Garde de pierre %s de la Horde", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 10: ////Horde////Blood Guard////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_7){
                if(Plr->HasKnownTitle(PVPTITLE_STONE_GUARD)){
                    if(currentOr>=Or_7){
                        int32 newOr = currentOr - Or_7;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_STONE_GUARD,false);
                        Plr->SetKnownTitle(PVPTITLE_BLOOD_GUARD,true);
                        Plr->BroadcastMessage("Je vous nomme, Garde de sang %s de la Horde", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 11: ////Horde////Legionaire////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_8){
                if(Plr->HasKnownTitle(PVPTITLE_BLOOD_GUARD)){
                    if(currentOr>=Or_8){
                        int32 newOr = currentOr - Or_8;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_BLOOD_GUARD,false);
                        Plr->SetKnownTitle(PVPTITLE_LEGIONNAIRE,true);
                        Plr->BroadcastMessage("Je vous nomme, Legionnaire %s de la Horde", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 12: ////Horde////Centurion////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_9){
                if(Plr->HasKnownTitle(PVPTITLE_LEGIONNAIRE)){
                    if(currentOr>=Or_9){
                        int32 newOr = currentOr - Or_9;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_LEGIONNAIRE,false);
                        Plr->SetKnownTitle(PVPTITLE_CENTURION,true);
                        Plr->BroadcastMessage("Je vous nomme, Centurion %s de la Horde", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 13: ////Horde////Champion////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_10){
                if(Plr->HasKnownTitle(PVPTITLE_CENTURION)){
                    if(currentOr>=Or_10){
                        int32 newOr = currentOr - Or_10;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_CENTURION,false);
                        Plr->SetKnownTitle(PVPTITLE_CHAMPION,true);
                        Plr->BroadcastMessage("Je vous nomme, Champion %s de la Horde", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 14: ////Horde////Lieutenant General////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_11){
                if(Plr->HasKnownTitle(PVPTITLE_CHAMPION)){
                    if(currentOr>=Or_11){
                        int32 newOr = currentOr - Or_11;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_CHAMPION,false);
                        Plr->SetKnownTitle(PVPTITLE_LIEUTENANT_GENERAL,true);
                        Plr->BroadcastMessage("Je vous nomme, Lieutenant General %s de la Horde", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 15: ////Horde////General////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_12){
                if(Plr->HasKnownTitle(PVPTITLE_LIEUTENANT_GENERAL)){
                    if(currentOr>=Or_12){
                        int32 newOr = currentOr - Or_12;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_LIEUTENANT_GENERAL,false);
                        Plr->SetKnownTitle(PVPTITLE_GENERAL,true);
                        Plr->BroadcastMessage("Je vous nomme, General %s de la Horde", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 16: ////Horde////Warlord////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_13){
                if(Plr->HasKnownTitle(PVPTITLE_GENERAL)){
                    if(currentOr>=Or_13){
                        int32 newOr = currentOr - Or_13;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_GENERAL,false);
                        Plr->SetKnownTitle(PVPTITLE_WARLORD,true);
                        Plr->BroadcastMessage("Je vous nomme, Seigneur de guerre %s de la Horde", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 17: ////Horde////High Warlord////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_14){
                if(Plr->HasKnownTitle(PVPTITLE_WARLORD)){
                    if(currentOr>=Or_14){
                        int32 newOr = currentOr - Or_14;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_WARLORD,false);
                        Plr->SetKnownTitle(PVPTITLE_HIGH_WARLORD,true);
                        Plr->BroadcastMessage("Je vous nomme, Grand seigneur de guerre %s de la Horde", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 18: ////Alliance////Private////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_1){
                if(Plr->HasKnownTitle(PVPTITLE_CORPORAL)||Plr->HasKnownTitle(PVPTITLE_SERGEANT)||Plr->HasKnownTitle(PVPTITLE_MASTER_SERGEANT)||Plr->HasKnownTitle(PVPTITLE_SERGEANT_MAJOR)||Plr->HasKnownTitle(PVPTITLE_KNIGHT)||Plr->HasKnownTitle(PVPTITLE_KNIGHT_LIEUTENANT)||Plr->HasKnownTitle(PVPTITLE_KNIGHT_CAPTAIN)||Plr->HasKnownTitle(PVPTITLE_KNIGHT_CHAMPION)||Plr->HasKnownTitle(PVPTITLE_LIEUTENANT_COMMANDER)||Plr->HasKnownTitle(PVPTITLE_COMMANDER)||Plr->HasKnownTitle(PVPTITLE_MARSHAL)||Plr->HasKnownTitle(PVPTITLE_FIELD_MARSHAL)||Plr->HasKnownTitle(PVPTITLE_GRAND_MARSHAL)){
                    Plr->BroadcastMessage("Are you high or just stupid?!");
                    Plr->Gossip_Complete();
                }else{
                    if(currentOr>=Or_1){
                        int32 newOr = currentOr - Or_1;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_PRIVATE,true);
                        Plr->BroadcastMessage("Je vous nomme, Soldat %s de l'Alliance.", Plr->GetName());
                        Plr->BroadcastMessage("Reconnectez Vous Pour Activer Votre Titre.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 19: ////Alliance////Corporal////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_2){
                if(Plr->HasKnownTitle(PVPTITLE_PRIVATE)){
                    if(currentOr>=Or_2){
                        int32 newOr = currentOr - Or_2;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_PRIVATE,false);
                        Plr->SetKnownTitle(PVPTITLE_CORPORAL,true);
                        Plr->BroadcastMessage("Je vous nomme, Caporal %s de l'Alliance.", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 20: ////Alliance////Sergeant////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_3){
                if(Plr->HasKnownTitle(PVPTITLE_CORPORAL)){
                    if(currentOr>=Or_3){
                        int32 newOr = currentOr - Or_3;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_CORPORAL,false);
                        Plr->SetKnownTitle(PVPTITLE_SERGEANT,true);
                        Plr->BroadcastMessage("Je vous nomme, Sergent %s de l'Alliance.", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 21: ////Alliance////Master Sergeant////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_4){
                if(Plr->HasKnownTitle(PVPTITLE_SERGEANT)){
                    if(currentOr>=Or_4){
                        int32 newOr = currentOr - Or_4;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_SERGEANT,false);
                        Plr->SetKnownTitle(PVPTITLE_MASTER_SERGEANT,true);
                        Plr->BroadcastMessage("Je vous nomme, Sergent Chef %s de l'Alliance.", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 22: ////Alliance////Sergeant Major////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_5){
                if(Plr->HasKnownTitle(PVPTITLE_MASTER_SERGEANT)){
                    if(currentOr>=Or_5){
                        int32 newOr = currentOr - Or_5;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_MASTER_SERGEANT,false);
                        Plr->SetKnownTitle(PVPTITLE_SERGEANT_MAJOR,true);
                        Plr->BroadcastMessage("Je vous nomme, Sergent Major %s de l'Alliance.", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 23: ////Alliance////Chevalier////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_6){
                if(Plr->HasKnownTitle(PVPTITLE_SERGEANT_MAJOR)){
                    if(currentOr>=Or_6){
                        int32 newOr = currentOr - Or_6;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);


                        Plr->SetKnownTitle(PVPTITLE_SERGEANT_MAJOR,false);
                        Plr->SetKnownTitle(PVPTITLE_KNIGHT,true);
                        Plr->BroadcastMessage("Je vous nomme, Chevalier %s de l'Alliance.", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 24: ////Alliance////Chevalier-Lieutenant////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_7){
                if(Plr->HasKnownTitle(PVPTITLE_KNIGHT)){
                    if(currentOr>=Or_7){
                        int32 newOr = currentOr - Or_7;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_KNIGHT,false);
                        Plr->SetKnownTitle(PVPTITLE_KNIGHT_LIEUTENANT,true);
                        Plr->BroadcastMessage("Je vous nomme, Chevalier Lieutenant %s de l'Alliance.", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 25: ////Alliance////Chevalier-Captain////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_8){
                if(Plr->HasKnownTitle(PVPTITLE_KNIGHT_LIEUTENANT)){
                    if(currentOr>=Or_8){
                        int32 newOr = currentOr - Or_8;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_KNIGHT_LIEUTENANT,false);
                        Plr->SetKnownTitle(PVPTITLE_KNIGHT_CAPTAIN,true);
                        Plr->BroadcastMessage("Je vous nomme, Chevalier Capitaine %s de l'Alliance.", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 26: ////Alliance////Chevalier-Champion////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_9){
                if(Plr->HasKnownTitle(PVPTITLE_KNIGHT_CAPTAIN)){
                    if(currentOr>=Or_9){
                        int32 newOr = currentOr - Or_9;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_KNIGHT_CAPTAIN,false);
                        Plr->SetKnownTitle(PVPTITLE_KNIGHT_CHAMPION,true);
                        Plr->BroadcastMessage("Je vous nomme, Chevalier Champion %s de l'Alliance.", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 27: ////Alliance////Lieutenant Commander////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_10){
                if(Plr->HasKnownTitle(PVPTITLE_KNIGHT_CHAMPION)){
                    if(currentOr>=Or_10){
                        int32 newOr = currentOr - Or_10;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_KNIGHT_CHAMPION,false);
                        Plr->SetKnownTitle(PVPTITLE_LIEUTENANT_COMMANDER,true);
                        Plr->BroadcastMessage("Je vous nomme, Lieutenant Commandant %s de l'Alliance.", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 28: ////Alliance////Commander////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_11){
                if(Plr->HasKnownTitle(PVPTITLE_LIEUTENANT_COMMANDER)){
                    if(currentOr>=Or_11){
                        int32 newOr = currentOr - Or_11;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_LIEUTENANT_COMMANDER,false);
                        Plr->SetKnownTitle(PVPTITLE_COMMANDER,true);
                        Plr->BroadcastMessage("Je vous nomme, Commandant %s de l'Alliance.", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 29: ////Alliance////Marshal////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_12){
                if(Plr->HasKnownTitle(PVPTITLE_COMMANDER)){
                    if(currentOr>=Or_12){
                        int32 newOr = currentOr - Or_12;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_COMMANDER,false);
                        Plr->SetKnownTitle(PVPTITLE_MARSHAL,true);
                        Plr->BroadcastMessage("Je vous nomme, Marechal %s de l'Alliance.", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 30: ////Alliance////Field Marshal////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_13){
                if(Plr->HasKnownTitle(PVPTITLE_MARSHAL)){
                    if(currentOr>=Or_13){
                        int32 newOr = currentOr - Or_13;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_MARSHAL,false);
                        Plr->SetKnownTitle(PVPTITLE_FIELD_MARSHAL,true);
                        Plr->BroadcastMessage("Je vous nomme, Grand Marechal %s de l'Alliance.", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 31: ////Alliance////Grand Marshal////
            {
            uint32 m_killsLifeTime = Plr->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORBALE_KILLS);
            uint32 currentOr = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
            if (m_killsLifeTime>=HK_14){
                if(Plr->HasKnownTitle(PVPTITLE_FIELD_MARSHAL)){
                    if(currentOr>=Or_14){
                        int32 newOr = currentOr - Or_14;
                        Plr->SetUInt32Value(PLAYER_FIELD_COINAGE,newOr);
                        Plr->SetKnownTitle(PVPTITLE_FIELD_MARSHAL,false);
                        Plr->SetKnownTitle(PVPTITLE_GRAND_MARSHAL,true);
                        Plr->BroadcastMessage("Je vous nomme, Connetable %s de l'Alliance.", Plr->GetName());
                        Plr->BroadcastMessage("Selectionnez votre nouveau titre dans la liste déroulante dans votre onglet personnage.");
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas assez d'argent pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous n'avez pas le nombre d'Honneur tuer requis pour ce titre.");
                Plr->Gossip_Complete();
            }break;
            }

        case 32:     ////World PvP Titles////
            {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(5, "Duelist", 33);
            Menu->AddItem(5, "Rival", 34);
            Menu->AddItem(5, "Challenger", 35);
            if(Plr->getRace()== 10||Plr->getRace()== 2||Plr->getRace()== 6||Plr->getRace()== 8||Plr->getRace()== 5)
            {Menu->AddItem(5, "Conqueror", 36);}else{Menu->AddItem(5, "Justicar", 37);}
            Menu->AddItem(5, "Gladiator", 38);
            Menu->AddItem(5, "Merciless Gladiator", 39);
            Menu->AddItem(5, "Vengeful Gladiator", 40);
            Menu->SendTo(Plr);
            }break;

        case 33: ////Duelist////
            {
            uint32 m_arenaPoints = Plr->GetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY);
            uint32 maxrating = Plr->GetMaxPersonalRating();
            if(Plr->HasKnownTitle(PVPTITLE_RIVAL)||Plr->HasKnownTitle(PVPTITLE_CHALLENGER)||Plr->HasKnownTitle(PVPTITLE_CONQUEROR)||Plr->HasKnownTitle(PVPTITLE_JUSTICAR)||Plr->HasKnownTitle(PVPTITLE_GLADIATOR)||Plr->HasKnownTitle(PVPTITLE_MERCILESS_GLADIATOR)||Plr->HasKnownTitle(PVPTITLE_VENGEFUL_GLADIATOR)){
                Plr->BroadcastMessage("Vous etes grand ou stupide?!");
                Plr->Gossip_Complete();
            }else{
                if(maxrating>=ARATING_1){
                    if(m_arenaPoints>=ARENA_1){
                        int32 newpoints = m_arenaPoints - ARENA_1;
                        Plr->SetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY,newpoints);
                        Plr->SetKnownTitle(PVPTITLE_DUELIST,true);
                        Plr->BroadcastMessage("Je vous nomme, Duelliste %s", Plr->GetName());
                        Plr->BroadcastMessage("Reconnectez Vous Pour Activer Votre Titre");
                        Plr->Gossip_Complete();
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas de points d'arene pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous n'avez pas la cote personelle d'arene requis pour ce titre.");
                    Plr->Gossip_Complete();
                }break;
            }break;
            }

        case 34: ////Rival////
            {
            uint32 m_arenaPoints = Plr->GetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY);
            uint32 maxrating = Plr->GetMaxPersonalRating();
            if(Plr->HasKnownTitle(PVPTITLE_DUELIST)){
                if(maxrating>=ARATING_2){
                    if(m_arenaPoints>=ARENA_2){
                        int32 newpoints = m_arenaPoints - ARENA_2;
                        Plr->SetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY,newpoints);
                        Plr->SetKnownTitle(PVPTITLE_DUELIST,false);
                        Plr->SetKnownTitle(PVPTITLE_RIVAL,true);
                        Plr->BroadcastMessage("Je vous nomme, Rival %s", Plr->GetName());
                        Plr->BroadcastMessage("Reconnectez Vous Pour Activer Votre Titre");
                        Plr->Gossip_Complete();
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas de points d'arene pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous n'avez pas la cote personelle d'arene requis pour ce titre.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                Plr->Gossip_Complete();
            }break;
            }

        case 35: ////Challenger////
            {
            uint32 m_arenaPoints = Plr->GetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY);
            uint32 maxrating = Plr->GetMaxPersonalRating();
            if(Plr->HasKnownTitle(PVPTITLE_RIVAL)){
                if(maxrating>=ARATING_3){
                    if(m_arenaPoints>=ARENA_3){
                        int32 newpoints = m_arenaPoints - ARENA_3;
                        Plr->SetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY,newpoints);
                        Plr->SetKnownTitle(PVPTITLE_RIVAL,false);
                        Plr->SetKnownTitle(PVPTITLE_CHALLENGER,true);
                        Plr->BroadcastMessage("Je vous nomme, Competiteur %s", Plr->GetName());
                        Plr->BroadcastMessage("Reconnectez Vous Pour Activer Votre Titre");
                        Plr->Gossip_Complete();
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas de points d'arene pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous n'avez pas la cote personelle d'arene requis pour ce titre.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                Plr->Gossip_Complete();
            }break;
            }

        case 36: ////Conqueror////
            {
            uint32 m_arenaPoints = Plr->GetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY);
            uint32 maxrating = Plr->GetMaxPersonalRating();
            if(Plr->HasKnownTitle(PVPTITLE_CHALLENGER)){
                if(maxrating>=ARATING_4){
                    if(m_arenaPoints>=ARENA_4){
                        int32 newpoints = m_arenaPoints - ARENA_4;
                        Plr->SetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY,newpoints);
                        Plr->SetKnownTitle(PVPTITLE_CHALLENGER,false);
                        Plr->SetKnownTitle(PVPTITLE_CONQUEROR,true);
                        Plr->BroadcastMessage("Je vous nomme, Conquerant %s", Plr->GetName());
                        Plr->BroadcastMessage("Reconnectez Vous Pour Activer Votre Titre");
                        Plr->Gossip_Complete();
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas de points d'arene pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous n'avez pas la cote personelle d'arene requis pour ce titre.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                Plr->Gossip_Complete();
            }break;
            }

        case 37: ////Justicar////
            {
            uint32 m_arenaPoints = Plr->GetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY);
            uint32 maxrating = Plr->GetMaxPersonalRating();
            if(Plr->HasKnownTitle(PVPTITLE_CHALLENGER)){
                if(maxrating>=ARATING_4){
                    if(m_arenaPoints>=ARENA_4){
                        int32 newpoints = m_arenaPoints - ARENA_4;
                        Plr->SetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY,newpoints);
                        Plr->SetKnownTitle(PVPTITLE_CHALLENGER,false);
                        Plr->SetKnownTitle(PVPTITLE_JUSTICAR,true);
                        Plr->BroadcastMessage("Je vous nomme, Justicier %s", Plr->GetName());
                        Plr->BroadcastMessage("Reconnectez Vous Pour Activer Votre Titre");
                        Plr->Gossip_Complete();
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas de points d'arene pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous n'avez pas la cote personelle d'arene requis pour ce titre.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                Plr->Gossip_Complete();
            }break;
            }

        case 38: ////Gladiator////
            {
            uint32 m_arenaPoints = Plr->GetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY);
            uint32 maxrating = Plr->GetMaxPersonalRating();
            if(Plr->HasKnownTitle(PVPTITLE_CONQUEROR)||Plr->HasKnownTitle(PVPTITLE_JUSTICAR)){
                if(maxrating>=ARATING_5){
                    if(m_arenaPoints>=ARENA_5){
                        int32 newpoints = m_arenaPoints - ARENA_5;
                        Plr->SetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY,newpoints);
                        Plr->SetKnownTitle(PVPTITLE_CONQUEROR,false);
                        Plr->SetKnownTitle(PVPTITLE_JUSTICAR,false);
                        Plr->SetKnownTitle(PVPTITLE_GLADIATOR,true);
                        Plr->BroadcastMessage("Je vous nomme, Gladiateur %s", Plr->GetName());
                        Plr->BroadcastMessage("Reconnectez Vous Pour Activer Votre Titre");
                        Plr->Gossip_Complete();
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas de points d'arene pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous n'avez pas la cote personelle d'arene requis pour ce titre.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                Plr->Gossip_Complete();
            }break;
            }

        case 39: ////Merciless Gladiator////
            {
            uint32 m_arenaPoints = Plr->GetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY);
            uint32 maxrating = Plr->GetMaxPersonalRating();
            if(Plr->HasKnownTitle(PVPTITLE_GLADIATOR)){
                if(maxrating>=ARATING_6){
                    if(m_arenaPoints>=ARENA_6){
                        int32 newpoints = m_arenaPoints - ARENA_6;
                        Plr->SetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY,newpoints);
                        Plr->SetKnownTitle(PVPTITLE_GLADIATOR,false);
                        Plr->SetKnownTitle(PVPTITLE_MERCILESS_GLADIATOR,true);
                        Plr->BroadcastMessage("Je vous nomme, Gladiateur impitoyable %s", Plr->GetName());
                        Plr->BroadcastMessage("Reconnectez Vous Pour Activer Votre Titre");
                        Plr->Gossip_Complete();
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas de points d'arene pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous n'avez pas la cote personelle d'arene requis pour ce titre.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                Plr->Gossip_Complete();
            }break;
            }

        case 40: ////Vengeful Gladiator////
            {
            uint32 m_arenaPoints = Plr->GetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY);
            uint32 maxrating = Plr->GetMaxPersonalRating();
            if(Plr->HasKnownTitle(PVPTITLE_GLADIATOR)){
                if(maxrating>=ARATING_7){
                    if(m_arenaPoints>=ARENA_7){
                        int32 newpoints = m_arenaPoints - ARENA_7;
                        Plr->SetUInt32Value(PLAYER_FIELD_ARENA_CURRENCY,newpoints);
                        Plr->SetKnownTitle(PVPTITLE_MERCILESS_GLADIATOR,false);
                        Plr->SetKnownTitle(PVPTITLE_VENGEFUL_GLADIATOR,true);
                        Plr->BroadcastMessage("Je vous nomme, Gladiateur Vengeur %s", Plr->GetName());
                        Plr->BroadcastMessage("Reconnectez Vous Pour Activer Votre Titre");
                        Plr->Gossip_Complete();
                    }else{
                        Plr->BroadcastMessage("Vous n'avez pas de points d'arene pour ce titre.");
                        Plr->Gossip_Complete();
                    }break;
                }else{
                    Plr->BroadcastMessage("Vous n'avez pas la cote personelle d'arene requis pour ce titre.");
                    Plr->Gossip_Complete();
                }break;
            }else{
                Plr->BroadcastMessage("Vous devez acquerir le precedent titre avant d'achetter celui ci.");
                Plr->Gossip_Complete();
            }break;
            }

        case 41: ////Title Requiert Menu////
            {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            if(Plr->getRace()== 10||Plr->getRace()== 2||Plr->getRace()== 6||Plr->getRace()== 8||Plr->getRace()== 5)
            {Menu->AddItem(0, "Titres PvP de la Horde", 42);}else{Menu->AddItem(0, "Titres PvP de l'Alliance", 43);}
            Menu->AddItem(0, "Titres PvP d'Arene", 44);
            Menu->SendTo(Plr);

            }break;


        case 42: ////Horde Titles////
            {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(0, "Eclaireur", 45);
            Menu->AddItem(0, "Grunt", 46);
            Menu->AddItem(0, "Sergent", 47);
            Menu->AddItem(0, "Sergent Chef", 48);
            Menu->AddItem(0, "Adjudant", 49);
            Menu->AddItem(0, "Garde de pierre", 50);
            Menu->AddItem(0, "Garde de sang", 51);
            Menu->AddItem(0, "Legionnaire", 52);
            Menu->AddItem(0, "Centurion", 53);
            Menu->AddItem(0, "Champion", 54);
            Menu->AddItem(0, "Lieutenant General", 55);
            Menu->AddItem(0, "General", 56);
            Menu->AddItem(0, "Seigneur de guerre", 57);
            Menu->AddItem(0, "Grand Seigneur de guerre", 58);
            Menu->SendTo(Plr);
            }break;

        case 43: ////Alliance Titles////
            {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(0, "Soldat", 59);
            Menu->AddItem(0, "Caporal", 60);
            Menu->AddItem(0, "Sergent", 61);
            Menu->AddItem(0, "Sergent Chef", 62);
            Menu->AddItem(0, "Sergent Major", 63);
            Menu->AddItem(0, "Chevalier", 64);
            Menu->AddItem(0, "Chevalier-Lieutenant", 65);
            Menu->AddItem(0, "Chevalier-Capitaine", 66);
            Menu->AddItem(0, "Chevalier-Champion", 67);
            Menu->AddItem(0, "Lieutenant Commandant", 68);
            Menu->AddItem(0, "Commandant", 69);
            Menu->AddItem(0, "Marechal", 70);
            Menu->AddItem(0, "Grand Marechal", 71);
            Menu->AddItem(0, "Connetable", 72);
            Menu->SendTo(Plr);
            }break;

        case 44: ////World Titles////
            {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(0, "Duellist", 73);
            Menu->AddItem(0, "Rival", 74);
            Menu->AddItem(0, "Competiteur", 75);
            if(Plr->getRace()== 10||Plr->getRace()== 2||Plr->getRace()== 6||Plr->getRace()== 8||Plr->getRace()== 5)
            {Menu->AddItem(0, "Conquerant", 76);}else{Menu->AddItem(0, "Justicier", 77);}
            Menu->AddItem(0, "Gladiateur", 78);
            Menu->AddItem(0, "Gladiateur Impitoyable", 79);
            Menu->AddItem(0, "Gladiateur Vengeur", 80);
            Menu->SendTo(Plr);
            }break;    
////////////////////////---DO NOT EDIT ANYTHING BELOW UNLESS YOU SERIOUSLY KNOW WHAT YOUR DOING---//////////////////////////////
        case 45:
            {
                Plr->BroadcastMessage("|cff00ff00Scout Requirements:|r %u Honorable Kills, %u Gold.", HK_1, GINFO_1);
                Plr->Gossip_Complete();
            }break;

        case 46:
            {
                Plr->BroadcastMessage("|cff00ff00Grunt Requirements:|r %u Honorable Kills, %u Gold.", HK_2, GINFO_2);
                Plr->Gossip_Complete();
            }break;

        case 47:
            {
                Plr->BroadcastMessage("|cff00ff00Sergeant Requirements:|r %u Honorable Kills, %u Gold.", HK_3, GINFO_3);
                Plr->Gossip_Complete();
            }break;

        case 48:
            {
                Plr->BroadcastMessage("|cff00ff00Senior Sergeant Requirements:|r %u Honorable Kills, %u Gold.", HK_4, GINFO_4);
                Plr->Gossip_Complete();
            }break;

        case 49:
            {
                Plr->BroadcastMessage("|cff00ff00First Sergeant Requirements:|r %u Honorable Kills, %u Gold.", HK_5, GINFO_5);
                Plr->Gossip_Complete();
            }break;

        case 50:
            {
                Plr->BroadcastMessage("|cff00ff00Stone Guard Requirements:|r %u Honorable Kills, %u Gold.", HK_6, GINFO_6);
                Plr->Gossip_Complete();
            }break;

        case 51:
            {
                Plr->BroadcastMessage("|cff00ff00Blood Guard Requirements:|r %u Honorable Kills, %u Gold.", HK_7, GINFO_7);
                Plr->Gossip_Complete();
            }break;

        case 52:
            {
                Plr->BroadcastMessage("|cff00ff00Legionnaire Requirements:|r %u Honorable Kills, %u Gold.", HK_8, GINFO_8);
                Plr->Gossip_Complete();
            }break;

        case 53:
            {
                Plr->BroadcastMessage("|cff00ff00Centurion Requirements:|r %u Honorable Kills, %u Gold.", HK_9, GINFO_9);
                Plr->Gossip_Complete();
            }break;

        case 54:
            {
                Plr->BroadcastMessage("|cff00ff00Champion Requirements:|r %u Honorable Kills, %u Gold.", HK_10, GINFO_10);
                Plr->Gossip_Complete();
            }break;

        case 55:
            {
                Plr->BroadcastMessage("|cff00ff00Lieutenant General Requirements:|r %u Honorable Kills, %u Gold.", HK_11, GINFO_11);
                Plr->Gossip_Complete();
            }break;

        case 56:
            {
                Plr->BroadcastMessage("|cff00ff00General Requirements:|r %u Honorable Kills, %u Gold.", HK_12, GINFO_12);
                Plr->Gossip_Complete();
            }break;

        case 57:
            {
                Plr->BroadcastMessage("|cff00ff00Warlord Requirements:|r %u Honorable Kills, %u Gold.", HK_13, GINFO_13);
                Plr->Gossip_Complete();
            }break;

        case 58:
            {
                Plr->BroadcastMessage("|cff00ff00High Warlord Requirements:|r %u Honorable Kills, %u Gold.", HK_14, GINFO_14);
                Plr->Gossip_Complete();
            }break;

        case 59:
            {
                Plr->BroadcastMessage("|cff00ff00Private Requirements:|r %u Honorable Kills, %u Gold.", HK_1, GINFO_1);
                Plr->Gossip_Complete();
            }break;

        case 60:
            {
                Plr->BroadcastMessage("|cff00ff00Corporal Requirements:|r %u Honorable Kills, %u Gold.", HK_2, GINFO_2);
                Plr->Gossip_Complete();
            }break;

        case 61:
            {
                Plr->BroadcastMessage("|cff00ff00Sergeant Requirements:|r %u Honorable Kills, %u Gold.", HK_3, GINFO_3);
                Plr->Gossip_Complete();
            }break;

        case 62:
            {
                Plr->BroadcastMessage("|cff00ff00Master Sergeant Requirements:|r %u Honorable Kills, %u Gold.", HK_4, GINFO_4);
                Plr->Gossip_Complete();
            }break;

        case 63:
            {
                Plr->BroadcastMessage("|cff00ff00Sergeant Major Requirements:|r %u Honorable Kills, %u Gold.", HK_5, GINFO_5);
                Plr->Gossip_Complete();
            }break;

        case 64:
            {
                Plr->BroadcastMessage("|cff00ff00Knight Requirements:|r %u Honorable Kills, %u Gold.", HK_6, GINFO_6);
                Plr->Gossip_Complete();
            }break;

        case 65:
            {
                Plr->BroadcastMessage("|cff00ff00Knight-Lieutenant Requirements:|r %u Honorable Kills, %u Gold.", HK_7, GINFO_7);
                Plr->Gossip_Complete();
            }break;

        case 66:
            {
                Plr->BroadcastMessage("|cff00ff00Knight-Captain Requirements:|r %u Honorable Kills, %u Gold.", HK_8, GINFO_8);
                Plr->Gossip_Complete();
            }break;

        case 67:
            {
                Plr->BroadcastMessage("|cff00ff00Knight-Champion Requirements:|r %u Honorable Kills, %u Gold.", HK_9, GINFO_9);
                Plr->Gossip_Complete();
            }break;

        case 68:
            {
                Plr->BroadcastMessage("|cff00ff00Lieutenant Commander Requirements:|r %u Honorable Kills, %u Gold.", HK_10, GINFO_10);
                Plr->Gossip_Complete();
            }break;

        case 69:
            {
                Plr->BroadcastMessage("|cff00ff00Commander Requirements:|r %u Honorable Kills, %u Gold.", HK_11, GINFO_11);
                Plr->Gossip_Complete();
            }break;

        case 70:
            {
                Plr->BroadcastMessage("|cff00ff00Marshal Requirements:|r %u Honorable Kills, %u Gold.", HK_12, GINFO_12);
                Plr->Gossip_Complete();
            }break;

        case 71:
            {
                Plr->BroadcastMessage("|cff00ff00Field Marshal Requirements:|r %u Honorable Kills, %u Gold.", HK_13, GINFO_13);
                Plr->Gossip_Complete();
            }break;

        case 72:
            {
                Plr->BroadcastMessage("|cff00ff00Grand Marshal Requirements:|r %u Honorable Kills, %u Gold.", HK_14, GINFO_14);
                Plr->Gossip_Complete();
            }break;

        case 73:
            {
                Plr->BroadcastMessage("|cff00ff00Duelist Requirements:|r %u ArenaPoints, Personal Arena Rating of %u.", ARENA_1, ARATING_1);
                Plr->Gossip_Complete();
            }break;

        case 74:
            {
                Plr->BroadcastMessage("|cff00ff00Rival Requirements:|r %u ArenaPoints, Personal Arena Rating of %u.", ARENA_2, ARATING_2);
                Plr->Gossip_Complete();
            }break;

        case 75:
            {
                Plr->BroadcastMessage("|cff00ff00Challenger Requirements:|r %u ArenaPoints, Personal Arena Rating of %u.", ARENA_3, ARATING_3);
                Plr->Gossip_Complete();
            }break;

        case 76:
            {
                Plr->BroadcastMessage("|cff00ff00Conqueror Requirements:|r %u ArenaPoints, Personal Arena Rating of %u.", ARENA_4, ARATING_4);
                Plr->Gossip_Complete();
            }break;

        case 77:
            {
                Plr->BroadcastMessage("|cff00ff00Justicar Requirements:|r %u ArenaPoints, Personal Arena Rating of %u.", ARENA_4, ARATING_4);
                Plr->Gossip_Complete();
            }break;

        case 78:
            {
                Plr->BroadcastMessage("|cff00ff00Gladiator Requirements:|r %u ArenaPoints, Personal Arena Rating of %u.", ARENA_5, ARATING_5);
                Plr->Gossip_Complete();
            }break;

        case 79:
            {
                Plr->BroadcastMessage("|cff00ff00Merciless Gladiator Requirements:|r %u ArenaPoints, Personal Arena Rating of %u.", ARENA_6, ARATING_6);
                Plr->Gossip_Complete();
            }break;

        case 80:
            {
                Plr->BroadcastMessage("|cff00ff00Vengeful Gladiator Requirements:|r %u ArenaPoints, Personal Arena Rating of %u.", ARENA_7, ARATING_7);
                Plr->Gossip_Complete();
            }break;
   }
}

void PvPTitlesNPC::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

void SetupPvPTitlesNPC(ScriptMgr * mgr)
{
    GossipScript * gs = (GossipScript*) new PvPTitlesNPC();
    mgr->register_gossip_script(NPC_ID, gs);
}