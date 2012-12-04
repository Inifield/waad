/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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

/************************************************************************/
/* GENERAL GUARD SCRIPT                                                 */
/************************************************************************/

// Covers *all* guard types, scripting their texts to guide players around.
// Enable this define to make all gossip texts have a "back" / "I was looking
// for somethinge else" button added.

#define BACK_BUTTON

#ifdef BACK_BUTTON

// Make code neater with this define.
#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
    Menu->SendTo(Plr);

#else

// Make code neater with this define.
#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
    Menu->AddItem(0, "Je cherchais quelque chose d'autre.", 0); \
    Menu->SendTo(Plr);

#endif

/************************************************************************/
/* Stormwind CITY Guards                                                */
/************************************************************************/

class StormwindGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 933, Plr);

        Menu->AddItem(0, "Hôtel des ventes", 1);
        Menu->AddItem(0, "Banque de Hurlevent", 2);
		Menu->AddItem(0, "Port de Hurlevent", 3);
        Menu->AddItem(0, "Le Tram des profondeurs", 4);
        Menu->AddItem(0, "L'auberge", 5);
        Menu->AddItem(0, "Le maître des griffons", 6);    
        Menu->AddItem(0, "Le maître des guildes", 7);
        Menu->AddItem(0, "Boîte aux lettres", 8);
        Menu->AddItem(0, "Maître des écuries", 9);
        Menu->AddItem(0, "Maître d'armes", 10);
        Menu->AddItem(0, "Mess des officiers", 11);
        Menu->AddItem(0, "Maître de guerre", 12);
		Menu->AddItem(0, "Salon de coiffure", 13);
        Menu->AddItem(0, "Maître de classe", 14);
        Menu->AddItem(0, "Maître de profession", 15);
		Menu->AddItem(0, "Lexique de puissance", 16);

        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // Auction House
            Plr->Gossip_SendPOI(-8811.46, 667.46, 7, 6, 0, "Hôtel des ventes de Hurlevent");
			SendQuickMenu(3834);
			break;

        case 2:     // Bank of Stormwind
            Plr->Gossip_SendPOI(-8916.87, 622.87, 7, 6, 0, "Banque de Hurlevent");
            SendQuickMenu(764);
			break;

		case 3:		// Docks
			Plr->Gossip_SendPOI(-8583.87, 1005.10, 7, 6, 0, "Port de Hurlevent");
			SendQuickMenu(12840);
			break;

        case 4:     // Deeprun Tram
            Plr->Gossip_SendPOI(-8378.88, 554.23, 7, 6, 0, "Le tram des profondeurs");
            SendQuickMenu(3813);
			break;

        case 5:     // The Inn
            SendQuickMenu(3860);
            Plr->Gossip_SendPOI(-8869.0, 675.4, 7, 6, 0, "La Rose dorée");
            break;

        case 6:     // Gryphon Master
            Plr->Gossip_SendPOI(-8837.0, 493.5, 7, 6, 0, "Maître des griffons de Hurlevent");
            SendQuickMenu(879);
			break;

        case 7:     // Guild Master
            Plr->Gossip_SendPOI(-8894.0, 611.2, 7, 6, 0, "Centre d'accueil des visiteurs de Hurlevent");
            SendQuickMenu(882);
			break;

        case 8:     // Mailbox
            Plr->Gossip_SendPOI(-8876.48, 649.18, 7, 6, 0, "Boîte aux lettres de Hurlevent");
			SendQuickMenu(3518);
			break;

        case 9:     // Stable Master
            Plr->Gossip_SendPOI(-8433.0, 554.7, 7, 6, 0, "Jenova Targe-pierre");
            SendQuickMenu(5984);
			break;

        case 10:     // Weapons Master
            Plr->Gossip_SendPOI(-8797.0, 612.8, 7, 6, 0, "Woo Ping");
            SendQuickMenu(4516);
			break;

        case 11:    // Officers' Lounge
            Plr->Gossip_SendPOI(-8759.92, 399.69, 7, 6, 0, "Hall des Champions");
            SendQuickMenu(7047);
			break;

        case 12:    // Battlemaster
            Plr->Gossip_SendPOI(-8393.62, 274.21, 7, 6, 0, "Maître de guerre de Hurlevent");
			SendQuickMenu(7499);
			break;

		case 13:	// Barbershop
			Plr->Gossip_SendPOI(-8765.87, 662.01, 7, 6, 0, "Salon de coiffure de Hurlevent");
			SendQuickMenu(12841);
			break;

        case 14:    // Class Trainers
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 898, Plr);
			Menu->AddItem( 0, "Druide"      , 17);
            Menu->AddItem( 0, "Chasseur"     , 18);
            Menu->AddItem( 0, "Mage"       , 19);
            Menu->AddItem( 0, "Paladin"    , 20);
            Menu->AddItem( 0, "Prêtre"     , 21);
            Menu->AddItem( 0, "Voleur"      , 22);
            Menu->AddItem( 0, "Chaman"     , 23);
            Menu->AddItem( 0, "Démoniste"    , 24);
			Menu->AddItem( 0, "Guerrier"    , 25);
            Menu->SendTo(Plr);
            break;

        case 15:    // Profession Trainers
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 918, Plr);
            Menu->AddItem( 0, "Alchimie"           , 26);
            Menu->AddItem( 0, "Forge"     , 27);
            Menu->AddItem( 0, "Cuisine"           , 28);
            Menu->AddItem( 0, "Enchantement"        , 29);
            Menu->AddItem( 0, "Ingénierie"       , 30);
            Menu->AddItem( 0, "Secourisme"         , 31);
            Menu->AddItem( 0, "Pêche"           , 32);
            Menu->AddItem( 0, "Herboristerie"         , 33);
			Menu->AddItem( 0, "Calligraphie"		, 34);
            Menu->AddItem( 0, "Travail du cuir"    , 35);
            Menu->AddItem( 0, "Minage"            , 36);
            Menu->AddItem( 0, "Dépeçage"          , 37);
            Menu->AddItem( 0, "Couture"         , 38);
            Menu->SendTo(Plr);
            break;

		case 16: // Lexicon of power
			Plr->Gossip_SendPOI(-8859.61, 861.73, 7, 6, 0, "Calligraphie de Hurlevent");
			SendQuickMenu(12842);
			break;

            ////////////////
            // Class trainer submenu
            ////////

        case 17: // Druid
			Plr->Gossip_SendPOI(-8751.0, 1124.5, 7, 6, 0, "Le Parc");
            SendQuickMenu(902);
            break;

        case 18: // Hunter
            Plr->Gossip_SendPOI(-8413.0, 541.5, 7, 6, 0, "Pavillon du chasseur");
            SendQuickMenu(905);
            break;

        case 19: // Mage
            Plr->Gossip_SendPOI(-9012.0, 867.6, 7, 6, 0, "Sanctuaire du sorcier");
            SendQuickMenu(899);
            break;

        case 20: // Paladin
            Plr->Gossip_SendPOI(-8577.0, 881.7, 7, 6, 0, "Cathédrale de la Lumière");
            SendQuickMenu(904);
            break;

        case 21: // Priest
            Plr->Gossip_SendPOI(-8512.0, 862.4, 7, 6, 0, "Cathédrale de la Lumière");
            SendQuickMenu(903);
            break;

        case 22: // Rogue
            Plr->Gossip_SendPOI(-8753.0, 367.8, 7, 6, 0, "Hurlevent - Maison des voleurs");
            SendQuickMenu(900);
            break;

        case 23: // Shaman
			Plr->Gossip_SendPOI(-9031.54, 549.87, 7, 6, 0, "Long-voyante Umbrua");
            SendQuickMenu(10106);
            break;

		case 24: // Warlock
            Plr->Gossip_SendPOI(-8948.91, 998.35, 7, 6, 0, "L'Agneau assassiné");
            SendQuickMenu(906);
            break;

        case 25: // Warrior
            Plr->Gossip_SendPOI(-8714.14, 334.96, 7, 6, 0, "Caserne de Hurlevent");
            SendQuickMenu(901);
            break;

        case 26: // Alchemy
            Plr->Gossip_SendPOI(-8988.0, 759.60, 7, 6, 0, "Tout pour l'alchimiste");
            SendQuickMenu(919);
            break;

        case 27: // Blacksmithing
            Plr->Gossip_SendPOI(-8424.0, 616.9, 7, 6, 0, "Therum Forge-profonde");
            SendQuickMenu(920);
            break;

        case 28: // Cooking
            Plr->Gossip_SendPOI(-8611.0, 364.6, 7, 6, 0, "Taverne du Cochon Siffleur");
            SendQuickMenu(921);
            break;

        case 29: // Enchanting
            Plr->Gossip_SendPOI(-8858.0, 803.7, 7, 6, 0, "Lucan Cordell");
            SendQuickMenu(941);
            break;

        case 30: // Engineering
            Plr->Gossip_SendPOI(-8347.0, 644.1, 7, 6, 0, "Lilliam Fusétincelle");
            SendQuickMenu(922);
            break;

        case 31: // First Aid
            Plr->Gossip_SendPOI(-8513.0, 801.8, 7, 6, 0, "Shaina Fuller");
            SendQuickMenu(923);
            break;

        case 32: // Fishing
            Plr->Gossip_SendPOI(-8803.0, 767.5, 7, 6, 0, "Arnold Leland");
            SendQuickMenu(940);
            break;

        case 33: // Herbalism
            Plr->Gossip_SendPOI(-8967.0, 779.5, 7, 6, 0, "Tout pour l'alchimiste");
            SendQuickMenu(924);
            break;

		case 34: // Inscription
			Plr->Gossip_SendPOI(-8859.61, 861.73, 7, 6, 0, "Calligraphie de Hurlevent");
			SendQuickMenu(22002);
			break;

        case 35: // Leatherworking
            Plr->Gossip_SendPOI(-8726.0, 477.4, 7, 6, 0, "Au cuir protecteur");
            SendQuickMenu(925);
            break;

        case 36: // Mining
            Plr->Gossip_SendPOI(-8434.0, 692.8, 7, 6, 0, "Gelman Main-de-Pierre");
            SendQuickMenu(927);
            break;

        case 37: // Skinning
            Plr->Gossip_SendPOI(-8716.0, 469.4, 7, 6, 0, "Au cuir protecteur");
            SendQuickMenu(928);
            break;

        case 38: //Tailoring
            Plr->Gossip_SendPOI(-8938.0, 800.7, 7, 6, 0, "Textiles de Duncan");
            SendQuickMenu(929);
            break;
        }
    }
};

class DarnassusGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3016, Plr);

        Menu->AddItem( 0, "Hôtel des ventes"      , 1);
        Menu->AddItem( 0, "La banque"           , 2);
        Menu->AddItem( 0, "Le maître des hippogriffes"  , 3);
        Menu->AddItem( 0, "Le maître des guildes"       , 4);
        Menu->AddItem( 0, "L'auberge"            , 5);
        Menu->AddItem( 0, "La boîte aux lettres"            , 6);
        Menu->AddItem( 0, "Maître des écuries"      , 7);
        Menu->AddItem( 0, "Maître d'armes"    , 8);
        Menu->AddItem( 0, "Maître de guerre"       , 9);
        Menu->AddItem( 0, "Maître de classe"      , 10);
	 	Menu->AddItem( 0, "Maître de profession" , 11);
		Menu->AddItem( 0, "Lexique de puissance", 12);

        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers (Most/All 'borrowed' from scriptdev)
            /////

        case 1:     // Auction House
			Plr->Gossip_SendPOI(9861.23, 2334.55, 7, 6, 0, "Hôtel des Ventes de Darnassus");
			SendQuickMenu(3833);
			break;
			
		case 2:		// The Bank
			Plr->Gossip_SendPOI(9938.45, 2512.35, 7, 6, 0, "Banque de Darnassus");
			SendQuickMenu(3017);
			break;
		
		case 3:		// Hippogryph Master
			Plr->Gossip_SendPOI(9945.65, 2618.94, 7, 6, 0, "Ruth'theran");
			SendQuickMenu(3018);
			break;
			
		case 4:		// Guild Master
			Plr->Gossip_SendPOI(10076.40, 2199.59, 7, 6, 0, "Maître des guildes de Darnassus");
			SendQuickMenu(3019);
			break;
			
		case 5:		// The Inn
			Plr->Gossip_SendPOI(10133.29, 2222.52, 7, 6, 0, "Auberge de Darnassus");
			SendQuickMenu(3020);
			break;
			
		case 6:		// Mailbox
			Plr->Gossip_SendPOI(9942.17, 2495.48, 7, 6, 0, "Boîte aux lettres de Darnassus");
			SendQuickMenu(3021);
			break;
		
		case 7:		// Stable Master
			Plr->Gossip_SendPOI(10167.20, 2522.66, 7, 6, 0, "Alassin");
			SendQuickMenu(5980);
			break;
			
		case 8:		// Weapons Trainer
			Plr->Gossip_SendPOI(9907.11, 2329.70, 7, 6, 0, "Ilyenia Lunéclat");
			SendQuickMenu(4517);
			break;
			
		case 9:    // Battlemaster
			Plr->Gossip_SendPOI(9981.9, 2325.9, 7, 6, 0, "Maître de guerre de Darnassus");
			SendQuickMenu(7522);            
			break;
			
		case 10:    // Class Trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4264, Plr);
            Menu->AddItem( 0, "Druide"       , 13);
            Menu->AddItem( 0, "Chasseur"      , 14);
            Menu->AddItem( 0, "Prêtre"      , 15);
            Menu->AddItem( 0, "Voleur"       , 16);
            Menu->AddItem( 0, "Guerrier"     , 17);
            Menu->SendTo(Plr);
            break;

        case 11:    // Profession Trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4273, Plr);
            Menu->AddItem( 0, "Alchimie"           , 18);
            Menu->AddItem( 0, "Cuisine"           , 19);
            Menu->AddItem( 0, "Enchantement"        , 20);
            Menu->AddItem( 0, "Secourisme"         , 21);
            Menu->AddItem( 0, "Pêche"           , 22);
            Menu->AddItem( 0, "Herboristerie"         , 23);
			Menu->AddItem( 0, "Calligraphie"		, 24);
            Menu->AddItem( 0, "Travail du cuir"    , 25);
            Menu->AddItem( 0, "Dépeçage"          , 26);
            Menu->AddItem( 0, "Couture"         , 27);
            Menu->SendTo(Plr);
            break;

		
		case 12:	// Lexicon of power
			Plr->Gossip_SendPOI(10131.70, 2321.77, 7, 6, 0, "Calligraphie de Darnassus");
			SendQuickMenu(12843);
			break;
		

		case 13:	// Druid
            Plr->Gossip_SendPOI(10186, 2570.46, 7, 6, 0, "Maître des druides de Darnassus");
			SendQuickMenu(3024);
			break;

		case 14:	// Hunter
            Plr->Gossip_SendPOI(10177.29, 2511.10, 7, 6, 0, "Maître des chasseurs de Darnassus");
			SendQuickMenu(3023);
            break;

        case 15:	// Priest
			Plr->Gossip_SendPOI(9659.12, 2524.88, 7, 6, 0, "Temple de la Lune");
			SendQuickMenu(3025);
			break;
	
        case 16:	// Rogue
            Plr->Gossip_SendPOI(10122, 2599.12, 7, 6, 0, "Maître des voleurs de Darnassus");
			SendQuickMenu(3026);
			break;

        case 17:	// Warrior
            Plr->Gossip_SendPOI(9951.91, 2280.38, 7, 6, 0, "Terrasse des guerriers");
            SendQuickMenu(3033);
			break;
			
		case 18: // Alchemy
            Plr->Gossip_SendPOI(10075.90, 2356.76, 7, 6, 0, "Maître des alchimistes de Darnassus");
            SendQuickMenu(3035);
			break;

        case 19: // Cooking
            Plr->Gossip_SendPOI(10088.59, 2419.21, 7, 6, 0, "Maître des cuisinier de Darnassus");
            SendQuickMenu(3036);
			break;
			
        case 20: // Enchanting
            Plr->Gossip_SendPOI(10146.09, 2313.42, 7, 6, 0, "Maître des enchanteurs de Darnassus");
            SendQuickMenu(3337);
			break;

        case 21: // First Aid
            Plr->Gossip_SendPOI(10150.09, 2390.43, 7, 6, 0, "Maître des secouristes de Darnassus");
            SendQuickMenu(3037);
			break;

        case 22: // Fishing
            Plr->Gossip_SendPOI(9836.20, 2432.17, 7, 6, 0, "Maître des pêcheurs de Darnassus");
            SendQuickMenu(3038);
			break;

        case 23: // Herbalism
            Plr->Gossip_SendPOI(9757.17, 2430.16, 7, 6, 0, "Maître des herboristes de Darnassus");
            SendQuickMenu(3039);
			break;

		case 24: // Inscription
			Plr->Gossip_SendPOI(10131.70, 2321.77, 7, 6, 0, "Calligraphie de Darnassus");
			SendQuickMenu(12844);
			break;

        case 25: // Leatherworking
            Plr->Gossip_SendPOI(10086.59, 2255.77, 7, 6, 0, "Maître des travailleurs du cuir de Darnassus");
            SendQuickMenu(3040);
            break;

        case 26: // Skinning
            Plr->Gossip_SendPOI(10081.40, 2257.18, 7, 6, 0, "Maître des dépeceurs de Darnassus");
            SendQuickMenu(3042);
			break;

        case 27: //Tailoring
            Plr->Gossip_SendPOI(10079.70, 2268.19, 7, 6, 0, "Tailleur de Darnassus");
			SendQuickMenu(3044);
            break;
		}
	}
    void GossipEnd(Object* pObject, Player* Plr)
    {

    }
};

class GoldshireGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
		Menu->AddItem( 0, "Banque"                 , 1);
		Menu->AddItem( 0, "Maître des griffons"       , 2);
		Menu->AddItem( 0, "Maître des guildes"         , 3);
		Menu->AddItem( 0, "Auberge"                  , 4);
		Menu->AddItem( 0, "Maître des écuries"        , 5);
		Menu->AddItem( 0, "Maître de classe"        , 6);
		Menu->AddItem( 0, "Maître de profession"   , 7);
        if(AutoSend)
            Menu->SendTo(Plr);
    }

	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {	
	
		case 0:     // Return to start
			GossipHello(pObject, Plr, true);
			break;

            //////////////////////
            // Main menu handlers
            /////

		case 1:     //Bank
			Plr->Gossip_SendPOI(-8916.87, 622.87, 7, 6, 0, "Banque de Hurlevent");
			SendQuickMenu(4260);
			break;

		case 2:     //Gryphon Master
			Plr->Gossip_SendPOI(-8837.0, 493.5, 7, 6, 0, "Maître des griffons de Hurlevent");
	        SendQuickMenu(4261);
			break;

		case 3:     //Guild Master
			Plr->Gossip_SendPOI(-8894.0, 611.2, 7, 6, 0, "Centre d'accueil des visiteurs de Hurlevent");
			SendQuickMenu(4262);
			break;
	
	   case 4:     //Inn
	       Plr->Gossip_SendPOI(-9459.34, 42.08, 99, 6, 0, "Auberge de la Fierte du Lion");
	       SendQuickMenu(4263);
		   break;

	   case 5:     //Stable Master
	       Plr->Gossip_SendPOI(-9466.62, 45.87, 99, 6, 0, "Erma");
		   SendQuickMenu(5983);
	       break;

	   case 6:     //Class Trainer
		   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4264, Plr);
	       Menu->AddItem( 0, "Druide",                8);
	       Menu->AddItem( 0, "Chasseur",               9);
	       Menu->AddItem( 0, "Mage",                 10);
	       Menu->AddItem( 0, "Paladin",              11);
	       Menu->AddItem( 0, "Prêtre",               12);
	       Menu->AddItem( 0, "Voleur",                13);
		   Menu->AddItem( 0, "Chaman",               14);
	       Menu->AddItem( 0, "Démoniste",              15);
		   Menu->AddItem( 0, "Guerrier",              16);
		   Menu->SendTo(Plr);
		   break;

		case 7:		//Profession Trainer
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4273, Plr);
			Menu->AddItem( 0, "Alchimie"              ,17);
			Menu->AddItem( 0, "Forge"        ,18);
			Menu->AddItem( 0, "Cuisine"              ,19);
			Menu->AddItem( 0, "Enchantement"           ,20);
			Menu->AddItem( 0, "Ingénierie"          ,21);
			Menu->AddItem( 0, "Secourisme"            ,22);
			Menu->AddItem( 0, "Pêche"              ,23);
			Menu->AddItem( 0, "Herboristerie"            ,24);
			Menu->AddItem( 0, "Calligraphie"		,25);
			Menu->AddItem( 0, "Travail du cuir"       ,26);
			Menu->AddItem( 0, "Minage"               ,27);
			Menu->AddItem( 0, "Dépeçage"             ,28);
			Menu->AddItem( 0, "Couture"            ,29);
			Menu->SendTo(Plr);
			break;

		case 8: //Druid
            SendQuickMenu(4265);
            break;

		case 9: //Hunter
            SendQuickMenu(4266);
            break;

		case 10: //Mage
            Plr->Gossip_SendPOI(-9471.12, 33.44, 7, 6, 0, "Zaldimar Wefhellt");
            SendQuickMenu(4268);
            break;

		case 11: //Paladin
            Plr->Gossip_SendPOI(-9469, 108.05, 7, 6, 0, "Frere Wilhelm");
            SendQuickMenu(4269);
            break;

		case 12: //Priest
            Plr->Gossip_SendPOI(-9461.07, 32.6, 7, 6, 0, "Pretresse Josetta");
            SendQuickMenu(4267);
            break;

		case 13: //Rogue
            Plr->Gossip_SendPOI(-9465.13, 13.29, 7, 6, 0, "Keryn Sylvius");
            SendQuickMenu(4270);
            break;

		case 14: //Shaman
            Plr->Gossip_SendPOI(1781, 53, 7, 6, 0, "Maître des chamans");
            SendQuickMenu(3512);
            break;

		case 15: //Warlock
            Plr->Gossip_SendPOI(-9473.21, -4.08, 7, 6, 0, "Maximillian Crowe");
            SendQuickMenu(4272);
            break;

		case 16: //Warrior
            Plr->Gossip_SendPOI(-9461.82, 109.50, 7, 6, 0, "Lyria Du Lac");
            SendQuickMenu(4271);
            break;

		case 17: //Alchemy
            Plr->Gossip_SendPOI(-9057.04, 153.63, 7, 6, 0, "Alchimie");
            SendQuickMenu(4274);
            break;

		case 18: //Blacksmithing
            Plr->Gossip_SendPOI(-9456.58, 87.90, 7, 6, 0, "Argus le Forgeron");
            SendQuickMenu(4275);
            break;

		case 19: //Cooking
            Plr->Gossip_SendPOI(-9467.54, -3.16, 7, 6, 0, "Tomas");
            SendQuickMenu(4276);
            break;

		case 20: //Enchanting
            SendQuickMenu(4277);
            break;

		case 21: //Engineering
            SendQuickMenu(4278);
            break;

		case 22: //First Aid
            Plr->Gossip_SendPOI(-9456.82, 30.49, 7, 6, 0, "Michelle Belle");
            SendQuickMenu(4279);
            break;

		case 23: //Fishing
            Plr->Gossip_SendPOI(-9386.54, -118.73, 7, 6, 0, "Lee Brown");
            SendQuickMenu(4280);
            break;

		case 24: //Herbalism
            Plr->Gossip_SendPOI(-9060.70, 149.23, 7, 6, 0, "Herboriste Pomeroy");
            SendQuickMenu(4281);        
            break;

		case 25: //Inscription
			Plr->Gossip_SendPOI(-8859.61, 861.73, 7, 6, 0, "Calligraphie de Hurlevent");
			SendQuickMenu(22002);
			break;

		case 26: //Leatherworking
            Plr->Gossip_SendPOI(-9376.12, -75.23, 7, 6, 0, "Adele Fielder");
            SendQuickMenu(4282);
            break;

		case 27: //Mining
            SendQuickMenu(4283);
            break;

		case 28: //Skinnin
            Plr->Gossip_SendPOI(-9536.91, -1212.76, 7, 6, 0, "Helene Peltskinner");
            SendQuickMenu(4284);
            break;

		case 29: //Tailoring
            Plr->Gossip_SendPOI(-9376.12, -75.23, 7, 6, 0, "Eldrin");
            SendQuickMenu(4285);
            break;
		}
    }   
};

class UndercityGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3543, Plr);
		
		Menu->AddItem(0, "La banque", 1);
		Menu->AddItem(0, "L'éleveur de chauve-souris", 2);
		Menu->AddItem(0, "Le maître des guildes", 3);
		Menu->AddItem(0, "L'auberge", 4);
		Menu->AddItem(0, "La boite aux lettres", 5);
		Menu->AddItem(0, "L'hôtel des ventes", 6);
		Menu->AddItem(0, "Le maître des zeppelins", 7);
		Menu->AddItem(0, "Le maître d'armes", 8);
		Menu->AddItem(0, "Le maître des écuries", 9);
		Menu->AddItem(0, "Le maître de guerre", 10);
		Menu->AddItem(0, "Le salon de coiffure", 11);
		Menu->AddItem(0, "Les maîtres de classe", 12);
		Menu->AddItem(0, "Les maîtres de profession", 13);
		Menu->AddItem(0, "Lexique de puissance", 14);

        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////
			
		case 1:     // The bank
            Plr->Gossip_SendPOI(1595.64, 232.45, 7, 6, 0, "Banque de Fossoyeuse");
            SendQuickMenu(3514);
			break;

        case 2:     // The bat handler
			Plr->Gossip_SendPOI(1565.9, 271.43, 7, 6, 0, "Eleveur de chauve-souris");
			SendQuickMenu(3515);
            break;

        case 3:     // The guild master
            Plr->Gossip_SendPOI(1594.17, 205.57, 7, 6, 0, "Maître des guildes de Fossoyeuse");
            SendQuickMenu(3516);
            break;

        case 4:     // The inn
            Plr->Gossip_SendPOI(1639.43, 220.99, 7, 6, 0, "Auberge de Fossoyeuse");
            SendQuickMenu(3517);
            break;

        case 5:     // The mailbox
            Plr->Gossip_SendPOI(1632.68, 219.4, 7, 6, 0, "Boite aux lettres de Fossoyeuse");
            SendQuickMenu(3518);
			break;

        case 6:     // The auction house
            Plr->Gossip_SendPOI(1647.9, 258.49, 7, 6, 0, "Hotel des ventes de Fossoyeuse");
            SendQuickMenu(3519);
			break;

        case 7:     // The zeppelin master
            Plr->Gossip_SendPOI(2059, 274.86, 7, 6, 0, "Zeppelin de Fossoyeuse");
            SendQuickMenu(3520);
			break;

        case 8:     // The weapon master
            SendQuickMenu(4521);
            Plr->Gossip_SendPOI(1670.31, 324.66, 7, 6, 0, "Archibald");
            break;

        case 9:     // The stable master
            Plr->Gossip_SendPOI(1634.18, 226.76, 7, 6, 0, "Anya Maulray");
            SendQuickMenu(5979);
			break;

        case 10:    // The battlemaster
			Plr->Gossip_SendPOI(1300.33, 350.92, 7, 6, 0, "Maître de guerre de Fossoyeuse");
			SendQuickMenu(7527);
			break;

		case 11:	// Barbershop
			Plr->Gossip_SendPOI(1577.19,203.82, 7, 6, 0, "Salon de coiffure de Fossoyeuse");
			SendQuickMenu(12845);
			break;

        case 12:    // A class trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3542, Plr);
            Menu->AddItem( 0, "Mage"		 , 15);
            Menu->AddItem( 0, "Paladin"      , 16);
            Menu->AddItem( 0, "Prêtre"       , 17);
            Menu->AddItem( 0, "Voleur"        , 18);
            Menu->AddItem( 0, "Démoniste"      , 19);
            Menu->AddItem( 0, "Guerrier"      , 20);
            Menu->SendTo(Plr);
            break;

        case 13:    // A profession trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3541, Plr);
            Menu->AddItem( 0, "Alchimie"           , 21);
            Menu->AddItem( 0, "Forge"     , 21);
            Menu->AddItem( 0, "Cuisine"           , 23);
            Menu->AddItem( 0, "Enchantement"        , 24);
            Menu->AddItem( 0, "Ingénierie"       , 25);
            Menu->AddItem( 0, "Secourisme"         , 26);
            Menu->AddItem( 0, "Pêche"           , 27);
            Menu->AddItem( 0, "Herboristerie"         , 28);
			Menu->AddItem( 0, "Calligraphie"		, 29);
            Menu->AddItem( 0, "Travail du cuir"    , 30);
            Menu->AddItem( 0, "Minage"            , 31);
            Menu->AddItem( 0, "Dépeçage"          , 32);
            Menu->AddItem( 0, "Couture"         , 33);
            Menu->SendTo(Plr);
            break;
		
		case 14: // Lexicon of power
			Plr->Gossip_SendPOI(1498.86, 293.03, 7, 6, 0, "Calligraphie de Fossoyeuse");
			SendQuickMenu(12847);
			break;

            ////////////////
            // Class trainer submenu
            ////////

        case 15: // Mage
            Plr->Gossip_SendPOI(1781, 53, 7, 6, 0, "Maître des mages de Fossoyeuse");
            SendQuickMenu(3513);
            break;

        case 16: // Paladin
            Plr->Gossip_SendPOI(1298.98, 316.51, 7, 6, 0, "Championne Cyssa Auberose");
            SendQuickMenu(3556);
            break;

        case 17: // Priest
            Plr->Gossip_SendPOI(1758.33, 401.5, 7, 6, 0, "Maître des prêtres de Fossoyeuse");
            SendQuickMenu(3521);
            break;

        case 18: // Rogue
            Plr->Gossip_SendPOI(1418.56, 65, 7, 6, 0, "Maître des voleurs de Fossoyeuse");
            SendQuickMenu(3526);
            break;

        case 19: // Warlock
            Plr->Gossip_SendPOI(1780.92, 53.16, 7, 6, 0, "Maître des démonistes de Fossoyeuse");
            SendQuickMenu(3526);
            break;

        case 20: // Warrior
            Plr->Gossip_SendPOI(1775.59, 418.19, 7, 6, 0, "Maître des guerriers de Fossoyeuse");
            SendQuickMenu(3527);
            break;

        case 21: // Alchemy
            Plr->Gossip_SendPOI(1419.82, 417.19, 7, 6, 0, "L'Apothicarium");
            SendQuickMenu(3528);
            break;

        case 22: // Blacksmithing
            Plr->Gossip_SendPOI(1696, 285, 7, 6, 0, "Maître des forgerons de Fossoyeuse");
            SendQuickMenu(3529);
            break;

        case 23: // Cooking
            Plr->Gossip_SendPOI(1596.34, 274.68, 7, 6, 0, "Maître des cuisiniers de Fossoyeuse");
            SendQuickMenu(3530);
            break;

        case 24: // Enchanting
            Plr->Gossip_SendPOI(1488.54, 280.19, 7, 6, 0, "Maître des enchanteurs de Fossoyeuse");
            SendQuickMenu(3531);
            break;

        case 25: // Engineering
            Plr->Gossip_SendPOI(1408.58, 143.43, 7, 6, 0, "Maître des ingénieurs de Fossoyeuse");
            SendQuickMenu(3532);
            break;

        case 26: //First Aid
            Plr->Gossip_SendPOI(1519.65, 167.19, 7, 6, 0, "Maître des premiers soins de Fossoyeuse");
             SendQuickMenu(3533);
            break;

        case 27: // Fishing
            Plr->Gossip_SendPOI(1679.9, 89, 7, 6, 0, "Maître des pêcheurs de Fossoyeuse");
            SendQuickMenu(3534);
            break;

        case 28: // Herbalism
            Plr->Gossip_SendPOI(1558, 349.36, 7, 6, 0, "Maître des herboristes de Fossoyeuse");
            SendQuickMenu(3535);
            break;

		case 29: // Inscription
			Plr->Gossip_SendPOI(1498.86, 293.03, 7, 6, 0, "Calligraphie de Fossoyeuse");
			SendQuickMenu(12846);
			break;

        case 30: // Leatherworking
            Plr->Gossip_SendPOI(1498.76, 196.43, 7, 6, 0, "Maître des artisans du cuir de Fossoyeuse");
            SendQuickMenu(3536);
            break;

        case 31: // Mining
            Plr->Gossip_SendPOI(1642.88, 335.58, 7, 6, 0, "Maître des mineurs de Fossoyeuse");
            SendQuickMenu(3537);
            break;

        case 32: // Skinning
            Plr->Gossip_SendPOI(1498.6, 196.46, 7, 6, 0, "Maître des dépeceurs de Fossoyeuse");
            SendQuickMenu(3538);
            break;

        case 33: // Tailoring
            Plr->Gossip_SendPOI(1689.55, 193, 7, 6, 0, "Maître des tailleurs de Fossoyeuse");
            SendQuickMenu(3539);
            break;
        }
    }
};

class DolanaarGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4316, Plr);

        Menu->AddItem(0, "La Banque", 1);
		Menu->AddItem(0, "Bac de Rut'Theran", 2);
		Menu->AddItem(0, "Le maître des guildes", 3);
		Menu->AddItem(0, "L'auberge", 4);
		Menu->AddItem(0, "Maître des écuries", 5);
		Menu->AddItem(0, "Maître de classe", 6);
		Menu->AddItem(0, "Maître de profession", 7);

        if(AutoSend)
            Menu->SendTo(Plr);
    }
	
    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // The Bank
            SendQuickMenu(4317);
            break;
		
		case 2:		// Rut'theran erry 
			SendQuickMenu(4318);
			break;
		
		case 3:		// The Guild Master
			SendQuickMenu(4319);
			break;
		
		case 4:		// The Inn
			Plr->Gossip_SendPOI(9821.49, 960.13, 6, 6, 0, "Auberge de Dolanaar");
			SendQuickMenu(4320);
			break;
			
		case 5:		// Stable Master
			Plr->Gossip_SendPOI(9808.37, 931.1, 6, 6, 0, "Seriadne");
			SendQuickMenu(5982);
			break;

        case 6:    // Class Trainers
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4264, Plr);
            Menu->AddItem( 0, "Druide"       , 8);
            Menu->AddItem( 0, "Chasseur"      , 9);
            Menu->AddItem( 0, "Prêtre"     , 10);
            Menu->AddItem( 0, "Voleur"      , 11);
            Menu->AddItem( 0, "Guerrier"    , 12);
            Menu->SendTo(Plr);
            break;

        case 7:    // Profession Trainers
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4273, Plr);
            Menu->AddItem( 0, "Alchimie"           , 13);
            Menu->AddItem( 0, "Cuisine"           , 14);
            Menu->AddItem( 0, "Enchantement"        , 15);
            Menu->AddItem( 0, "Secourisme"         , 16);
            Menu->AddItem( 0, "Pêche"           , 17);
            Menu->AddItem( 0, "Herboristerie"         , 18);
			Menu->AddItem( 0, "Calligraphie"		, 19);
            Menu->AddItem( 0, "Travail du cuir"    , 20);
            Menu->AddItem( 0, "Dépeçage"          , 21);
            Menu->AddItem( 0, "Couture"         , 22);
            Menu->SendTo(Plr);
            break;

            ////////////////
            // Class trainer submenu
            ////////

        case 8: //Druid
            
            Plr->Gossip_SendPOI(9741.58, 963.7, 7, 6, 0, "Kal");
            SendQuickMenu(4323);
            break;
		
		case 9: // Hunter
			Plr->Gossip_SendPOI(9815.12, 926.28, 7, 6, 0, "Dazalar");
			SendQuickMenu(4324);
			break;
		
		case 10: // Priest
			Plr->Gossip_SendPOI(9906.16, 986.63, 7, 6, 0, "Laurna Clairmatin");
			SendQuickMenu(4325);
			break;

		case 11: // Rogue
			
			Plr->Gossip_SendPOI(9789, 942.86, 7, 6, 0, "Jannok Chantebrise");
			SendQuickMenu(4327);
			break;
			
		case 12: // Warrior
			Plr->Gossip_SendPOI(9821.96, 950.61, 7, 6, 0, "Kyra Lamevent");
			SendQuickMenu(4327);
			break;

        case 13: // Alchemy
            Plr->Gossip_SendPOI(9767.59, 878.81, 7, 6, 0, "Cyndra Murmegent");
            SendQuickMenu(4329);
            break;
		
		case 14: // Cooking

			Plr->Gossip_SendPOI(9751.19, 906.13, 7, 6, 0, "Zarrin");
			SendQuickMenu(4330);
			break;
		
		case 15: // Enchanting
			Plr->Gossip_SendPOI(10677.59, 1946.56, 7, 6, 0, "Alanna Raveneye");
			SendQuickMenu(4331);
			break;
			
		case 16: // First Aid
			Plr->Gossip_SendPOI(9903.12, 999, 7, 6, 0, "Byancie");
			SendQuickMenu(4332);
			break;
			
		case 17: // Fishing
			SendQuickMenu(4333);
			break;
			
		case 18: // Herbalism
			Plr->Gossip_SendPOI(9773.78, 875.88, 7, 6, 0, "Malorne FeuilleLame");
			SendQuickMenu(4334);
			break;
		
		case 19: // Inscription
			Plr->Gossip_SendPOI(10131.70, 2321.77, 7, 6, 0, "Calligraphie de Darnassus");
			SendQuickMenu(12844);
			break;

		case 20: // Leatherworking
			Plr->Gossip_SendPOI(10152.59, 1681.46, 7, 6, 0, "Nadyia Tissecrin");
			SendQuickMenu(4335);
			break;
			
		case 21: // Skinning
			Plr->Gossip_SendPOI(10135.59, 1673.18, 7, 6, 0, "Radnaal Tissecrin");
			SendQuickMenu(4336);
			break;
			
		case 22: // Tailoring
			SendQuickMenu(4337);
			break;
        }
    }
};

class SilvermoonGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9316, Plr);

        Menu->AddItem(0, "L'hôtel des ventes", 1);
        Menu->AddItem(0, "La banque", 2);
		Menu->AddItem(0, "Le maître des faucons-dragons", 3);
        Menu->AddItem(0, "Le maître des guildes", 4);
        Menu->AddItem(0, "L'auberge", 5);
        Menu->AddItem(0, "La boite aux lettres", 6);    
        Menu->AddItem(0, "Maître des écuries", 7);
        Menu->AddItem(0, "Le maître d'armes", 8);
        Menu->AddItem(0, "Les maîtres de guerre", 9);
        Menu->AddItem(0, "Maître de classe", 10);
        Menu->AddItem(0, "Maître de profession", 11);
        Menu->AddItem(0, "Métier à tisser le mana", 12);
		Menu->AddItem(0, "Lexique de puissance", 13);
 
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // Auction House
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9316, Plr);
            Menu->AddItem(0, "A l'ouest."  , 14);
            Menu->AddItem(0, "A l'est."    , 15);
            Menu->SendTo(Plr);
            break;

        case 2:     // The Bank
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9322, Plr);
            Menu->AddItem(0, "L'ouest."  , 16);
            Menu->AddItem(0, "L'est."    , 17);
            Menu->SendTo(Plr);
            break;

		case 3:     // Dragonhawk Handler
            Plr->Gossip_SendPOI(9378.45, -7163.94, 7, 6, 0, "Lune-d'argent, maître de vol");
            SendQuickMenu(10160);
			break;

        case 4:     // Guild Master
            Plr->Gossip_SendPOI(9480.75, -7345.587, 7, 6, 0, "Lune-d'argent, maison des guildes");
            SendQuickMenu(10161);
			break;

        case 5:     // The Inn
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9325, Plr);
            Menu->AddItem(0, "L'auberge de Lune-d'argent"  , 17);
            Menu->AddItem(0, "Au Repos du Voyageur"    , 18);
            Menu->SendTo(Plr);
            break;

        case 6:     // Mailbox
            Plr->Gossip_SendPOI(9743.078, -7466.4, 7, 6, 0, "Boite aux lettres de Lune-d'argent");
            SendQuickMenu(10162);
			break;

        case 7:     // Stable Master
            Plr->Gossip_SendPOI(9904.95, -7404.31, 7, 6, 0, "Maître des écuries de Lune-d'argent");
            SendQuickMenu(10163);
			break;

        case 8:     // Weapon Master
            Plr->Gossip_SendPOI(9841.17, -7505.13, 7, 6, 0, "Maître d'armes de Lune-d'argent");
            SendQuickMenu(10164);
			break;

	    case 9:     // Battlemasters
            Plr->Gossip_SendPOI(9850.74, -7563.84, 7, 6, 0, "Maître de guerre de Lune-d'argent");
            SendQuickMenu(10165);
			break;

        case 10:    // Class Trainers
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9316, Plr);
            Menu->AddItem( 0, "Druide"       , 20);
            Menu->AddItem( 0, "Chasseur"      , 21);
            Menu->AddItem( 0, "Mage"        , 22);
            Menu->AddItem( 0, "Paladin"     , 23);
            Menu->AddItem( 0, "Prêtre"      , 24);
            Menu->AddItem( 0, "Voleur"       , 25);
            Menu->AddItem( 0, "Démoniste"     , 26);
            Menu->SendTo(Plr);
            break;

        case 11:    // Profession Trainers
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9338, Plr);
            Menu->AddItem( 0, "Alchimie"           , 27);
            Menu->AddItem( 0, "Forge"     , 28);
            Menu->AddItem( 0, "Cuisine"           , 29);
            Menu->AddItem( 0, "Enchantement"        , 30);
            Menu->AddItem( 0, "Ingénierie"       , 31);
            Menu->AddItem( 0, "Secourisme"         , 32);
            Menu->AddItem( 0, "Pêche"           , 33);
			Menu->AddItem( 0, "Herboristerie"         , 34);
			Menu->AddItem( 0, "Calligraphie"		, 35);
			Menu->AddItem( 0, "Joaillier"     , 36);
            Menu->AddItem( 0, "Travail du cuir"    , 37);
            Menu->AddItem( 0, "Minage"            , 38);
            Menu->AddItem( 0, "Dépeçage"          , 39);
            Menu->AddItem( 0, "Couture"         , 40);
            Menu->SendTo(Plr);
            break;

		case 12: // Mana Loom
            Plr->Gossip_SendPOI(9751.013, -7074.85, 7, 6, 0, "Lune-d'argent, métier à tisser le mana");
            SendQuickMenu(10166);
            break;
		
		case 13: // Lexicon of power
			Plr->Gossip_SendPOI(9962.57, -7246.18, 7, 6, 0, "Calligraphie de Lune-d'argent");
			SendQuickMenu(12843);
			break;

        case 14: // To the west - Auction House no. 1
            Plr->Gossip_SendPOI(9649.429, -7134.027, 7, 6, 0, "Lune-d'argent, hôtel des ventes");
            SendQuickMenu(10154);
            break;

        case 15: // To the east - Auction House no. 2
            Plr->Gossip_SendPOI(9682.864, -7515.786, 7, 6, 0, "Lune-d'argent, hôtel des ventes");
            SendQuickMenu(10155);
            break;

        case 16: // The bank - The west.
            Plr->Gossip_SendPOI(9522.104, -7208.878, 7, 6, 0, "Lune-d'argent, banque");
            SendQuickMenu(10156);
			break;

        case 17: // The bank - The east.
            Plr->Gossip_SendPOI(9791.07, -7488.041, 7, 6, 0, "Lune-d'argent, banque");
            SendQuickMenu(10157);            
			break;

        case 18: // The Silvermoon City Inn
            Plr->Gossip_SendPOI(9677.113, -7367.575, 7, 6, 0, "Lune-d''argent, auberge");
            SendQuickMenu(10158);
            break;

        case 19: // The Wayfarer's Rest tavern
            Plr->Gossip_SendPOI(9562.813, -7218.63, 7, 6, 0, "Lune-d''argent, auberge");
            SendQuickMenu(10159);
            break;

        case 20: // Druid
            Plr->Gossip_SendPOI(9700.55, -7262.57, 7, 6, 0, "Maître des druides de Lune-d'argent");
            SendQuickMenu(11407);
            break;

        case 21: // Hunter
            Plr->Gossip_SendPOI(9930.568, -7412.115, 7, 6, 0, "Maître des chasseurs de Lune-d'argent");
            SendQuickMenu(11408);
            break;

        case 22: // Mage
            Plr->Gossip_SendPOI(9996.914, -7104.803, 7, 6, 0, "Maître des mages de Lune-d'argent");
            SendQuickMenu(11409);
            break;

        case 23: // Paladin
            Plr->Gossip_SendPOI(9850.22, -7516.93, 7, 6, 0, "Maître des paladins de Lune-d'argent");
            SendQuickMenu(11410);
			break;

        case 24: // Priest
            Plr->Gossip_SendPOI(9935.37, -7131.14, 7, 6, 0, "Maître des prêtres de Lune-d'argent");
            SendQuickMenu(11411);
            break;

        case 25: // Rogue
            Plr->Gossip_SendPOI(9739.88, -7374.33, 7, 6, 0, "Maître des voleurs de Lune-d'argent");
            SendQuickMenu(11412);
            break;

        case 26: // Warlock
            Plr->Gossip_SendPOI(9803.052, -7316.967, 7, 6, 0, "Maître des démonistes de Lune-d'argent");
            SendQuickMenu(11413);
            break;

        case 27: // Alchemy
            Plr->Gossip_SendPOI(10000.9, -7216.63, 7, 6, 0, "Lune-d'argent, alchimie");
            SendQuickMenu(11433);
            break;

        case 28: // Blacksmithing
            Plr->Gossip_SendPOI(9841.43, -7361.53, 7, 6, 0, "Lune-d'argent, forge");
            SendQuickMenu(11434);
            break;

        case 29: // Cooking
           Plr->Gossip_SendPOI(9577.26, -7243.6, 7, 6, 0, "Lune-d'argent, cuisine");
		   SendQuickMenu(11435);
           break;

        case 30: // Enchanting
            Plr->Gossip_SendPOI(9962.57, -7246.18, 7, 6, 0, "Lune-d'argent, enchantement");
            SendQuickMenu(11436);
            break;

        case 31: // Engineering
            Plr->Gossip_SendPOI(9808.85, -7287.31, 7, 6, 0, "Lune-d'argent, ingénierie");
            SendQuickMenu(11437);
            break;

        case 32: // First Aid
            Plr->Gossip_SendPOI(9588.61, -7337.526, 7, 6, 0, "Secourisme de Lune-d'argent");
            SendQuickMenu(11438);
            break;

        case 33: // Fishing
            Plr->Gossip_SendPOI(9601.97, -7332.34, 7, 6, 0, "Maître des pêcheurs de Lune-d'argent");
            SendQuickMenu(11439);
            break;

        case 34: // Herbalism
            Plr->Gossip_SendPOI(9996.96, -7208.39, 7, 6, 0, "Maître des herboristes de Lune-d'argent");
            SendQuickMenu(11440);
            break;

		case 35: // Inscription
			Plr->Gossip_SendPOI(9962.57, -7246.18, 7, 6, 0, "Calligraphie de Lune-d'argent");
			SendQuickMenu(12848);
			break;

        case 36: // Jewelcrafting
            Plr->Gossip_SendPOI(9552.8, -7502.12, 7, 6, 0, "Maître des joailliers de Lune-d'argent");
            SendQuickMenu(11441);
            break;

        case 37: // Leatherworking
            Plr->Gossip_SendPOI(9502.486, -7425.51, 7, 6, 0, "Lune-d'argent, artisanat du cuir");
            SendQuickMenu(11442);
            break;

        case 38: // Mining
            Plr->Gossip_SendPOI(9813.73, -7360.19, 7, 6, 0, "Maître des mineurs de Lune-d'argent");
            SendQuickMenu(11443);
            break;

        case 39: // Skinning
            Plr->Gossip_SendPOI(9513.37, -7429.4, 7, 6, 0, "Maître des dépeceurs de Lune-d'argent");
            SendQuickMenu(11444);
            break;

		case 40: // Tailoring
            Plr->Gossip_SendPOI(9727.56, -7086.65, 7, 6, 0, "Lune-d'argent, couture");
            SendQuickMenu(11445);
            break;
        }
    }
};

class ExodarGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9316, Plr);
        Menu->AddItem(0, "L'hôtel des ventes", 1);
        Menu->AddItem(0, "La banque", 2);
        Menu->AddItem(0, "Maître des hippogriffes", 3);
		Menu->AddItem(0, "Le maître des guildes", 4);
        Menu->AddItem(0, "L'auberge", 5);    
        Menu->AddItem(0, "La boite aux lettres", 6);
        Menu->AddItem(0, "Maître des écuries", 7);
        Menu->AddItem(0, "Le maître d'armes", 8);
        Menu->AddItem(0, "Les maîtres de guerre", 9);
        Menu->AddItem(0, "Maître de classe", 10);
        Menu->AddItem(0, "Maître de profession", 11);
		Menu->AddItem(0, "Lexique de puissance", 12);
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // Auction House
            Plr->Gossip_SendPOI(-4013.82, -11729.64, 7, 6, 0, "Hôtel des ventes d'Exodar");
            SendQuickMenu(9528);
			break;

        case 2:     // The Bank
            Plr->Gossip_SendPOI(-3923.89, -11544.5, 7, 6, 0, "Banque d'Exodar");
            SendQuickMenu(9530);
			break;

        case 3:     // Hippogryph Master
            Plr->Gossip_SendPOI(-4058.45, -11789.7, 7, 6, 0, "Maître des hippogriffes d'Exodar");
            SendQuickMenu(9531);
			break;

        case 4:     // Guild Master
            Plr->Gossip_SendPOI(-4093.38, -11630.39, 7, 6, 0, "Maître des guildes d'Exodar");
            SendQuickMenu(9532);            
			break;

        case 5:     // The Inn
            Plr->Gossip_SendPOI(-3765.34, -11695.8, 7, 6, 0, "Auberge d'Exodar");
            SendQuickMenu(9533);
			break;

        case 6:     // Mailbox
            Plr->Gossip_SendPOI(-3913.75, -11606.83, 7, 6, 0, "Boite aux lettres d'Exodar");
            SendQuickMenu(9534);            
			break;

        case 7:     // Stable Master
            Plr->Gossip_SendPOI(-3787.01, -11702.7, 7, 6, 0, "Maître des écuries d'Exodar");
            SendQuickMenu(9535);
			break;

        case 8:     // Weapon Master
            Plr->Gossip_SendPOI(-4215.68, -11628.9, 7, 6, 0, "Maître d'armes d'Exodar");
            SendQuickMenu(9536);
            break;

	    case 9:     // Battlemasters
            Plr->Gossip_SendPOI(-3999.82, -11368.33, 7, 6, 0, "Maîtres de guerre d'Exodar");
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9537, Plr);
            Menu->AddItem(0, "Arene des maîtres de guerre"  , 13);
            Menu->SendTo(Plr);
            break;

        case 10:    // Class Trainers
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2599, Plr);
                Menu->AddItem( 0, "Druide"       , 14);
                Menu->AddItem( 0, "Chasseur"      , 15);
                Menu->AddItem( 0, "Mage"        , 16);
                Menu->AddItem( 0, "Paladin"     , 17);
                Menu->AddItem( 0, "Prêtre"      , 18);
                Menu->AddItem( 0, "Chaman"      , 19);
                Menu->AddItem( 0, "Guerrier"     , 20);
                Menu->SendTo(Plr);
            }break;

        case 11:    // Profession Trainers
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9555, Plr);
                Menu->AddItem( 0, "Alchimie"           , 21);
                Menu->AddItem( 0, "Forge"     , 22);
                Menu->AddItem( 0, "Enchantement"        , 23);
                Menu->AddItem( 0, "Ingénierie"       , 24);
                Menu->AddItem( 0, "Secourisme"         , 25);
                Menu->AddItem( 0, "Pêche"           , 26);
                Menu->AddItem( 0, "Herboristerie"         , 27);
				Menu->AddItem( 0, "Calligraphie"		, 28);
				Menu->AddItem( 0, "Joailler"     , 29);
                Menu->AddItem( 0, "Travail du cuir"    , 30);
                Menu->AddItem( 0, "Minage"            , 31);
                Menu->AddItem( 0, "Dépeçage"          , 32);
                Menu->AddItem( 0, "Couture"         , 33);
                Menu->AddItem( 0, "Cuisine"           , 34);
                Menu->SendTo(Plr);
            }break;

		case 12: // Lexicon of power
			Plr->Gossip_SendPOI(-3889.3, -11495, 7, 6, 0, "Calligraphie de l'Exodar");
			SendQuickMenu(12843);
			break;

            //////////////////
            // Battlemaster submenu
            ////////

		case 13:// Arena Battlemaster Exodar
            Plr->Gossip_SendPOI(-3725.25, -11688.3, 7, 6, 0, "Maître de guerre de l'Arene");
            SendQuickMenu(9538);
            break;

        case 14: // Druid
            Plr->Gossip_SendPOI(-4274.81, -11495.3, 7, 6, 0, "Maître des druides d'Exodar"); 
            SendQuickMenu(9539);
            break;

        case 15: // Hunter
            Plr->Gossip_SendPOI(-4229.36, -11563.36, 7, 6, 0, "Maître des chasseurs d'Exodar");
            SendQuickMenu(9540);
            break;

        case 16: // Mage
            Plr->Gossip_SendPOI(-4048.8, -11559.02, 7, 6, 0, "Maître des mages d'Exodar");
            SendQuickMenu(9541);
            break;

        case 17: // Paladin
            Plr->Gossip_SendPOI(-4176.57, -11476.46, 7, 6, 0, "Maître des paladins d'Exodar");
            SendQuickMenu(9542);
            break;

        case 18: // Priest
            Plr->Gossip_SendPOI(-3972.38, -11483.2, 7, 6, 0, "Maître des prêtres d'Exodar");
            SendQuickMenu(9543);
            break;

        case 19: // Shaman
            Plr->Gossip_SendPOI(-3843.8, -11390.75, 7, 6, 0, "Maître des chamans d'Exodar");
            SendQuickMenu(9544);
            break;

        case 20: // Warrior
            Plr->Gossip_SendPOI(-4191.11, -11650.45, 7, 6, 0, "Maître des guerriers d'Exodar");
            SendQuickMenu(9545);
            break;

        case 21: // Alchemy
            Plr->Gossip_SendPOI(-4042.37, -11366.3, 7, 6, 0, "Maître alchimiste d'Exodar");
            SendQuickMenu(9802);
            break;

        case 22: // Blacksmithing
            Plr->Gossip_SendPOI(-4232.4, -11705.23, 7, 6, 0, "Maître forgerons d'Exodar");
            SendQuickMenu(9803);
            break;

        case 23: // Enchanting
            Plr->Gossip_SendPOI(-3889.3, -11495, 7, 6, 0, "Maître enchanteurs d'Exodar");
            SendQuickMenu(9804);
            break;

        case 24: // Engineering
            Plr->Gossip_SendPOI(-4257.93, -11636.53, 7, 6, 0, "Maître ingénieur d'Exodar");
            SendQuickMenu(9805);
            break;

        case 25: // First Aid
            Plr->Gossip_SendPOI(-3766.05, -11481.8, 7, 6, 0, "Maître secourisme d'Exodar");
            SendQuickMenu(9806);
            break;

        case 26: // Fishing
            Plr->Gossip_SendPOI(-3726.64, -11384.43, 7, 6, 0, "Maître pêcheur d'Exodar");
            SendQuickMenu(9807);
            break;

        case 27: // Herbalism
            Plr->Gossip_SendPOI(-4052.5, -11356.6, 7, 6, 0, "Maître Herboriste d'Exodar");
            SendQuickMenu(9808);
            break;

		case 28: // Inscription
			Plr->Gossip_SendPOI(-3889.3, -11495, 7, 6, 0, "Calligraphie de l'Exodar");
			SendQuickMenu(12849);
			break;

        case 29: // Jewelcrafting
            Plr->Gossip_SendPOI(-3786.27, -11541.33, 7, 6, 0, "Maître joaillier d'Exodar");
            SendQuickMenu(9809);
            break;

        case 30: // Leatherworking
            Plr->Gossip_SendPOI(-4134.42, -11772.93, 7, 6, 0, "Maître artisan du cuir d'Exodar");
            SendQuickMenu(9810);
            break;

        case 31: // Mining
            Plr->Gossip_SendPOI(-4220.31, -11694.29, 7, 6, 0, "Maître mineur d'Exodar");
            SendQuickMenu(9811);
            break;

        case 32: // Skinning
            Plr->Gossip_SendPOI(-4134.97, -11760.5, 7, 6, 0, "Maître dépeceur d'Exodar");
            SendQuickMenu(9812);
            break;

		case 33: // Tailoring
            Plr->Gossip_SendPOI(-4095.78, -11746.9, 7, 6, 0, "Maître tailleur d'Exodar");
            SendQuickMenu(9813);
            break;

        case 34: // Cooking
            Plr->Gossip_SendPOI(-3799.69, -11650.51, 7, 6, 0, "Maître cuisinier d'Exodar");
            SendQuickMenu(9814);
            break;
        }
    }
};

class OrgrimmarGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
        Menu->AddItem(0, "La banque", 1);
        Menu->AddItem(0, "Le maître des coursiers du vent", 2);
        Menu->AddItem(0, "Le maître des guildes", 3);
        Menu->AddItem(0, "L'auberge", 4);
        Menu->AddItem(0, "La boite aux lettres", 5);
        Menu->AddItem(0, "L'hôtel des ventes", 6);    
        Menu->AddItem(0, "Le maître des zeppelins", 7);
        Menu->AddItem(0, "Le maître d'armes", 8);
        Menu->AddItem(0, "Le maître des écuries", 9);
        Menu->AddItem(0, "Mess des officiers", 10);
        Menu->AddItem(0, "Le maître de guerre", 11);
		Menu->AddItem(0, "Salon de coiffure", 12);
        Menu->AddItem(0, "Un maître de classe", 13);
        Menu->AddItem(0, "Un maître de profession", 14);
		Menu->AddItem(0, "Lexique de puissance", 15);

        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // The bank
            Plr->Gossip_SendPOI(1631.51, -4375.33, 7, 6, 0, "Banque d'Orgrimmar");
            SendQuickMenu(2554);
			break;

        case 2:     // The wind rider master
            Plr->Gossip_SendPOI(1676.6, -4332.72, 7, 6, 0, "La Tour des cieux");
            SendQuickMenu(2555);
			break;

        case 3:     // The guild master
            Plr->Gossip_SendPOI(1576.93, -4294.75, 7, 6, 0, "Ambassade de la Horde");
            SendQuickMenu(2556);
			break;

        case 4:     // The inn
            Plr->Gossip_SendPOI(1644.51, -4447.27, 7, 6, 0, "Auberge d'Orgrimmar");
            SendQuickMenu(2557);
			break;

        case 5:     // The mailbox
            Plr->Gossip_SendPOI(1622.53, -4388.79, 7, 6, 0, "Boite aux lettres d'Orgrimmar");
            SendQuickMenu(2558);
			break;

        case 6:     // The auction house
            Plr->Gossip_SendPOI(1679.21, -4450.1, 7, 6, 0, "Hôtel des ventes d'Orgrimmar");
            SendQuickMenu(3075);
			break;

        case 7:     // The zeppelin master
            Plr->Gossip_SendPOI(1337.36, -4632.7, 7, 6, 0, "Tour des zeppelins d'Orgrimmar");
            SendQuickMenu(3173);
			break;

        case 8:     // The weapon master
            Plr->Gossip_SendPOI(2092.56, -4823.95, 7, 6, 0, "Sayoc et Hanashi");
            SendQuickMenu(4519);
			break;

	    case 9:     // The stable master
            Plr->Gossip_SendPOI(2133.12, -4663.93, 7, 6, 0, "Xon'cha");
			SendQuickMenu(5974);
			break;

        case 10:    // The officers' lounge
			Plr->Gossip_SendPOI(1633.56, -4249.37, 7, 6, 0, "Hall des Légendes");
			SendQuickMenu(7046);
			break;

        case 11:    // The battlemaster
			Plr->Gossip_SendPOI(1990.41, -4794.15, 7, 6, 0, "Maître de guerre d'Orgrimmar");
			SendQuickMenu(7531);
			break;

		case 12:	// Barbershop
			Plr->Gossip_SendPOI(1768.30, -4342.55, 7, 6, 0, "Salon de coiffure d'Orgrimmar");
			SendQuickMenu(12854);
			break;

        case 13:    // A class trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2599, Plr);
            Menu->AddItem( 0, "Chasseur"       , 16);
            Menu->AddItem( 0, "Mage"         , 17);
            Menu->AddItem( 0, "Prêtre"       , 18);
            Menu->AddItem( 0, "Chaman"       , 19);
            Menu->AddItem( 0, "Voleur"        , 20);
            Menu->AddItem( 0, "Démoniste"      , 21);
            Menu->AddItem( 0, "Guerrier"      , 22);
            Menu->AddItem( 0, "Paladin"      , 23);
            Menu->SendTo(Plr);
            break;

        case 14:    // A profession trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2594, Plr);
            Menu->AddItem( 0, "Alchimie"           , 24);
            Menu->AddItem( 0, "Forge"     , 25);
            Menu->AddItem( 0, "Cuisine"           , 26);
            Menu->AddItem( 0, "Enchantement"        , 27);
            Menu->AddItem( 0, "Ingénierie"       , 28);
            Menu->AddItem( 0, "Secourisme"         , 29);
            Menu->AddItem( 0, "Pêche"           , 30);
            Menu->AddItem( 0, "Herboristerie"         , 31);
			Menu->AddItem( 0, "Calligraphie"		, 32);
            Menu->AddItem( 0, "Travail du cuir"    , 33);
            Menu->AddItem( 0, "Minage"            , 34);
            Menu->AddItem( 0, "Dépeçage"          , 35);
            Menu->AddItem( 0, "Couture"         , 36);
            Menu->SendTo(Plr);
            break;

		case 15: // Lexicon of power
			Plr->Gossip_SendPOI(1839.56, -4477.24, 7, 6, 0, "Calligraphie d'Orgrimmar");
			SendQuickMenu(12843);
			break;

            ////////////////
            // Class trainer submenu
            ////////

        case 16: // Hunter
            Plr->Gossip_SendPOI(2114.84, -4625.31, 7, 6, 0, "Hall du chasseur d'Orgrimmar");
            SendQuickMenu(2559);
            break;

        case 17: // Mage
            Plr->Gossip_SendPOI(1451.26, -4223.33, 7, 6, 0, "Pavillon de Buissombre");
            SendQuickMenu(2560);
            break;

        case 18: // Priest
			Plr->Gossip_SendPOI(1442.21, -4183.24, 7, 6, 0, "Autel des Esprits");
            SendQuickMenu(2561);
            break;

        case 19: // Shaman
		    Plr->Gossip_SendPOI(1925.34, -4181.89, 7, 6, 0, "Forteresse de Thrall");
            SendQuickMenu(2562);
            break;

        case 20: // Rogue
            Plr->Gossip_SendPOI(1773.39, -4278.97, 7, 6, 0, "Confrerie Ombrevive");
            SendQuickMenu(2563);
            break;

        case 21: // Warlock
            Plr->Gossip_SendPOI(1849.57, -4359.68, 7, 6, 0, "Enclave de Sombrefeu");
            SendQuickMenu(2564);
            break;

        case 22: // Warrior
            Plr->Gossip_SendPOI(1983.92, -4794.2, 7, 6, 0, "Hall des braves");
            SendQuickMenu(2565);
            break;

		case 23: // Paladin
            Plr->Gossip_SendPOI(1937.53, -4141.0, 7, 6, 0, "Forteresse de Thrall");
            SendQuickMenu(2566);
            break;

        case 24: // Alchemy
            Plr->Gossip_SendPOI(1955.17, -4475.79, 7, 6, 0, "Alchimie et potions de Yelmak");
            SendQuickMenu(2497);
            break;

        case 25: // Blacksmithing
            Plr->Gossip_SendPOI(2054.34, -4831.85, 7, 6, 0, "L'enclume ardente");
            SendQuickMenu(2499);
            break;

        case 26: // Cooking
            Plr->Gossip_SendPOI(1780.96, -4481.31, 7, 6, 0, "Fosse à feu de Borstan");
            SendQuickMenu(2500);
            break;

        case 27: // Enchanting
            Plr->Gossip_SendPOI(1917.5, -4434.95, 7, 6, 0, "Atelier des runes de Godan");
            SendQuickMenu(2501);
            break;

        case 28: // Engineering
            Plr->Gossip_SendPOI(2038.45, -4744.75, 7, 6, 0, "Atelier de Nogg");
            SendQuickMenu(2653);
            break;

        case 29: // First Aid
            Plr->Gossip_SendPOI(1485.21, -4160.91, 7, 6, 0, "Survie du plus fort");
            SendQuickMenu(2502);
            break;

        case 30: // Fishing
            Plr->Gossip_SendPOI(1994.15, -4655.7, 7, 6, 0, "Pêche de Lumak");
            SendQuickMenu(2503);
            break;

        case 31: // Herbalism
            Plr->Gossip_SendPOI(1898.61, -4454.93, 7, 6, 0, "Arboretum de Jandi");
            SendQuickMenu(2504);
            break;
		
		case 32: // Inscription
			Plr->Gossip_SendPOI(1839.56, -4477.24, 7, 6, 0, "Calligraphie d'Orgrimmar");
			SendQuickMenu(12850);
			break;

        case 33: // Leatherworking
            Plr->Gossip_SendPOI(1852.82, -4562.31, 7, 6, 0, "Artisan du cuir de Kodo");
            SendQuickMenu(2513);
            break;

        case 34: // Mining
            Plr->Gossip_SendPOI(2029.79, -4704, 7, 6, 0, "Mine du canyon rouge");
            SendQuickMenu(2515);
            break;

        case 35: //Skinning
            Plr->Gossip_SendPOI(1852.82, -4562.31, 7, 6, 0, "Artisan du cuir de Kodo");
            SendQuickMenu(2516);        
            break;

		case 36: //Tailoring
            Plr->Gossip_SendPOI(1802.66, -4560.66, 7, 6, 0, "Les étoffes de Magar");
            SendQuickMenu(2518);
            break;
        }
    }
};

class ThunderbluffGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3543, Plr);
        Menu->AddItem(0, "La banque", 1);
        Menu->AddItem(0, "Le maître des coursiers du vent", 2);
        Menu->AddItem(0, "Le maître des guildes", 3);
        Menu->AddItem(0, "L'auberge", 4);
        Menu->AddItem(0, "Le boite aux lettres", 5);
        Menu->AddItem(0, "L'hôtel des ventes", 6);    
        Menu->AddItem(0, "Le maître d'armes", 7);
        Menu->AddItem(0, "Le maître des écuries", 8);
        Menu->AddItem(0, "Le maître de guerre", 9);
        Menu->AddItem(0, "Un maître de classe", 10);
        Menu->AddItem(0, "Un maître de profession", 11);
		Menu->AddItem(0, "Lexique de puissance", 12);

        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // The bank
            Plr->Gossip_SendPOI(-1257.8, 24.14, 7, 6, 0, "Banque des Pitons du Tonnerre");
            SendQuickMenu(1292);
			break;

        case 2:     // The wind rider master
            Plr->Gossip_SendPOI(-1196.43, 28.26, 7, 6, 0, "Maître des coursiers du vent");
            SendQuickMenu(1293);
			break;

        case 3:     // The guild master
            Plr->Gossip_SendPOI(-1296.5, 127.57, 7, 6, 0, "Informations civiques des Pitons du Tonnerre");
            SendQuickMenu(1291);
			break;

        case 4:     // The inn
            Plr->Gossip_SendPOI(-1296, 39.7, 7, 6, 0, "Auberge des Pitons du Tonnerre");
            SendQuickMenu(3153);
			break;

        case 5:     // The mailbox
            Plr->Gossip_SendPOI(-1263.59, 44.36, 7, 6, 0, "Boite aux lettres des Pitons du Tonnerre");
            SendQuickMenu(3154);
			break;

        case 6:     // The auction house
            Plr->Gossip_SendPOI(-1205.51, 105.74, 7, 6, 0, "Hôtel des ventes des Pitons du Tonnerre");
            SendQuickMenu(3155);
			break;

        case 7:     // The weapon master
            Plr->Gossip_SendPOI(-1282.31, 89.56, 7, 6, 0, "Ansekhwa");
            SendQuickMenu(4520);
			break;

	    case 8:     // The stable master
            Plr->Gossip_SendPOI(-1270.19, 48.84, 7, 6, 0, "Bulrug");
			SendQuickMenu(5977);
			break;

        case 9:    // The battlemaster
			Plr->Gossip_SendPOI(-1391.22, -81.33, 7, 6, 0, "Maître de guerre des Pitons du Tonnerre");
			SendQuickMenu(7535);
			break;

        case 10:    // A class trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3542, Plr);
            Menu->AddItem( 0, "Druide"       , 13);
            Menu->AddItem( 0, "Chasseur"      , 14);
            Menu->AddItem( 0, "Mage"        , 15);
            Menu->AddItem( 0, "Prêtre"      , 16);
            Menu->AddItem( 0, "Chaman"      , 17);
            Menu->AddItem( 0, "Guerrier"     , 18);
            Menu->SendTo(Plr);
            break;

        case 11:    // A profession trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3541, Plr);
            Menu->AddItem( 0, "Alchimie"           , 19);
            Menu->AddItem( 0, "Forge"     , 20);
            Menu->AddItem( 0, "Cuisine"           , 21);
            Menu->AddItem( 0, "Enchantement"        , 22);
            Menu->AddItem( 0, "Secourisme"         , 23);
            Menu->AddItem( 0, "Pêche"           , 24);
            Menu->AddItem( 0, "Herboristerie"         , 25);
			Menu->AddItem( 0, "Calligraphie"		, 26);
            Menu->AddItem( 0, "Travail du cuir"    , 27);
            Menu->AddItem( 0, "Minage"            , 28);
            Menu->AddItem( 0, "Dépeçage"          , 29);
            Menu->AddItem( 0, "Couture"         , 30);
            Menu->SendTo(Plr);
            break;

		case 12: // Lexicon of power
			Plr->Gossip_SendPOI(-997.50, 219.01, 7, 6, 0, "Calligraphie des Pitons du Tonnerre");
			SendQuickMenu(12843);
			break;

            ////////////////
            // Class trainer submenu
            ////////

        case 13: // Druid
            Plr->Gossip_SendPOI(-1054.47, -285, 7, 6, 0, "Hall des Anciens");
            SendQuickMenu(1294);
            break;

        case 14: // Hunter
            Plr->Gossip_SendPOI(-1416.32, -114.28, 7, 6, 0, "Hall du Chasseur");
            SendQuickMenu(1295);
            break;

        case 15: // Mage
            Plr->Gossip_SendPOI(-1061.2, 195.5, 7, 6, 0, "Les Bassins de la vision");
            SendQuickMenu(1296);
            break;

        case 16: // Priest
            Plr->Gossip_SendPOI(-1061.2, 195.5, 7, 6, 0, "Les Bassins de la vision");
            SendQuickMenu(1297);
            break;

        case 17: // Shaman
            Plr->Gossip_SendPOI(-989.54, 278.25, 7, 6, 0, "Hall des Esprits");
            SendQuickMenu(1298);
            break;

        case 18: // Warrior
			Plr->Gossip_SendPOI(-1416.32, -114.28, 6, 6, 0, "Hall du Chasseur");
            SendQuickMenu(1299);
            break;

        case 19: // Alchemy
            Plr->Gossip_SendPOI(-1085.56, 27.29, 7, 6, 0, "Alchimie de Bena");
            SendQuickMenu(1332);
            break;

        case 20: // Blacksmithing
            Plr->Gossip_SendPOI(-1239.75, 104.88, 7, 6, 0, "Forge de Karn");
            SendQuickMenu(1333);
            break;

        case 21: // Cooking
            Plr->Gossip_SendPOI(-1214.5, -21.23, 7, 6, 0, "Cuisine d'Aska");
            SendQuickMenu(1334);
            break;

        case 22: // Enchanting
            Plr->Gossip_SendPOI(-1112.65, 48.26, 7, 6, 0, "Enchanteurs Aube-glorieuse");
            SendQuickMenu(1335);
            break;

        case 23: // First Aid
            Plr->Gossip_SendPOI(-996.58, 200.5, 7, 6, 0, "Soins spirituels");
            SendQuickMenu(1336);
            break;

        case 24: // Fishing
            Plr->Gossip_SendPOI(-1169.35, -68.87, 7, 6, 0, "Appâts et équipements du sommet");
            SendQuickMenu(1337);
            break;

        case 25: // Herbalism
            Plr->Gossip_SendPOI(-1137.7, -1.51, 7, 6, 0, "Herboristerie holistique");
            SendQuickMenu(1338);
            break;

		case 26: // Inscription
			Plr->Gossip_SendPOI(-997.50, 219.01, 7, 6, 0, "Calligraphie des Pitons du Tonnerre");
			SendQuickMenu(12851);
			break;

        case 27: // Leatherworking
            Plr->Gossip_SendPOI(-1156.22, 66.86, 7, 6, 0, "Armurerie des Pitons du Tonnerre");
            SendQuickMenu(1339);
            break;

        case 28: // Mining
            Plr->Gossip_SendPOI(-1249.17, 155, 7, 6, 0, "Géologie de Sabot-de-Pierre");
            SendQuickMenu(1340);
            break;

        case 29: // Skinning
            Plr->Gossip_SendPOI(-1148.56, 51.18, 7, 6, 0, "Mooranta");
            SendQuickMenu(1343); 
            break;

		case 30: // Tailoring
            Plr->Gossip_SendPOI(-1156.22, 66.86, 7, 6, 0, "Armurerie des Pitons du Tonnerre");
            SendQuickMenu(1341);
            break;
        }
    }
};

class BloodhoofGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3543, Plr);
        Menu->AddItem(0, "La banque", 1);
        Menu->AddItem(0, "Le maître des coursiers du vent", 2);
        Menu->AddItem(0, "L'auberge", 3);
        Menu->AddItem(0, "Le maître des écuries", 4);
        Menu->AddItem(0, "Un maître de classe", 5);
        Menu->AddItem(0, "Un maître de profession", 6);

        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // The bank
			Plr->Gossip_SendPOI(-1257.63, 19.56, 7, 6, 0, "Banque des Pitons du Tonnerre");
            SendQuickMenu(4051);
            break;

        case 2:     // The wind rider master
			Plr->Gossip_SendPOI(-1203.29, 34.19, 7, 6, 0, "Perchoir des coursiers du vent");
            SendQuickMenu(4052);
            break;

        case 3:     // The inn
            Plr->Gossip_SendPOI(-2361.38, -349.19, 7, 6, 0, "Auberge du village de Sabot-de-Sang");      
            SendQuickMenu(4053);
			break;

        case 4:     // The stable master
            Plr->Gossip_SendPOI(-2338.86, -357.56, 7, 6, 0, "Seikwa");
            SendQuickMenu(5976);
			break;

        case 5:     // A class trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4069, Plr);
            Menu->AddItem( 0, "Druide"       , 7);
            Menu->AddItem( 0, "Chasseur"      , 8);
            Menu->AddItem( 0, "Chaman"      , 9);
            Menu->AddItem( 0, "Guerrier"    , 10);
            Menu->SendTo(Plr);
			break;

        case 6:     // A profession trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3541, Plr);
            Menu->AddItem( 0, "Alchimie"           , 11);
            Menu->AddItem( 0, "Forge"     , 12);
            Menu->AddItem( 0, "Cuisine"           , 13);
            Menu->AddItem( 0, "Enchantement"        , 14);
            Menu->AddItem( 0, "Secourisme"         , 15);
            Menu->AddItem( 0, "Pêche"           , 16);
            Menu->AddItem( 0, "Herboristerie"         , 17);
			Menu->AddItem( 0, "Calligraphie"		, 18);
            Menu->AddItem( 0, "Travail du cuir"    , 19);
            Menu->AddItem( 0, "Minage"            , 20);
            Menu->AddItem( 0, "Dépeçage"          , 21);
            Menu->AddItem( 0, "Couture"         , 22);
            Menu->SendTo(Plr);
            break;


            ////////////////
            // Class trainer submenu
            ////////

        case 7: // Druid
            Plr->Gossip_SendPOI(-2312.15, -443.69, 7, 6, 0, "Gennia Totem-Runique");
            SendQuickMenu(4054);
            break;

        case 8: // Hunter
            Plr->Gossip_SendPOI(-2178.14, -406.14, 7, 6, 0, "Yaw Crin-Tranchant");
            SendQuickMenu(4055);
            break;

        case 9: // Shaman
            Plr->Gossip_SendPOI(-2301.5, -439.87, 7, 6, 0, "Narm Chasse-le-Ciel");
            SendQuickMenu(4056);
            break;

        case 10: // Warrior
            Plr->Gossip_SendPOI(-2345.43, -494.11, 7, 6, 0, "Krang Sabot-de-Pierre");
            SendQuickMenu(4057);
            break;

        case 11: // Alchemy
			Plr->Gossip_SendPOI(-1080.81, 30.11, 7, 6, 0, "Alchimie de Bena");
            SendQuickMenu(4058);
            break;

        case 12: // Blacksmithing
			Plr->Gossip_SendPOI(-1250.11, 113.80, 7, 6, 0, "Forge de Karn");
            SendQuickMenu(4059);
            break;

        case 13: // Cooking
            Plr->Gossip_SendPOI(-2263.34, -287.91, 7, 6, 0, "Pyall Sabots-feutres");
            SendQuickMenu(4060);
            break;

        case 14: // Enchanting
			Plr->Gossip_SendPOI(-1115.49, 49.14, 7, 6, 0, "Enchanteurs Aube-glorieuse");
			SendQuickMenu(4061);
            break;

        case 15: // First Aid
            Plr->Gossip_SendPOI(-2353.52, -355.82, 7, 6, 0, "Vira Jeune-sabot");
            SendQuickMenu(4062);
            break;

        case 16: // Fishing
            Plr->Gossip_SendPOI(-2349.21, -241.37, 7, 6, 0, "Uthan Eau-Plate");
            SendQuickMenu(4063);
            break;

        case 17: // Herbalism
			Plr->Gossip_SendPOI(-1145.67, 22.57, 7, 6, 0, "Herboristerie holistique");
            SendQuickMenu(4064);
            break;

		case 18: // Inscription
			Plr->Gossip_SendPOI(-997.50, 219.01, 7, 6, 0, "Calligraphie des Pitons du Tonnerre");
			SendQuickMenu(12851);
			break;

        case 19: // Leatherworking
            Plr->Gossip_SendPOI(-2257.12, -288.63, 7, 6, 0, "Chaw Cuir-solide");
            SendQuickMenu(4065);
            break;

        case 20: // Mining
			Plr->Gossip_SendPOI(-1255.88, 161.24, 7, 6, 0, "Géologie Sabot-de-pierre");
            SendQuickMenu(4066);
            break;

        case 21: // Skinning
            Plr->Gossip_SendPOI(-2252.94, -291.32, 7, 6, 0, "Yonn Entaille-profonde");
            SendQuickMenu(4067); 
            break;

		case 22: // Tailoring
			Plr->Gossip_SendPOI(-1161.71, 70.43, 7, 6, 0, "Armuriers de Pitons du Tonnerre");
            SendQuickMenu(4068);
            break;
        }
    }
};

class RazorHillGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4037, Plr);
        Menu->AddItem(0, "La banque", 1);
        Menu->AddItem(0, "Le maître des coursiers du Vent", 2);
        Menu->AddItem(0, "L'auberge", 3);
        Menu->AddItem(0, "Le maître des écuries", 4);
        Menu->AddItem(0, "Un maître de classe", 5);
        Menu->AddItem(0, "Un maître de profession", 6); 

        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // The bank
            SendQuickMenu(4032);
            break;

        case 2:     // The wind rider master
            SendQuickMenu(4033);
            break;

        case 3:     // The inn
            Plr->Gossip_SendPOI(338.7, -4688.87, 7, 6, 0, "Auberge de Tranchecolline");      
            SendQuickMenu(4034);
			break;

        case 4:     // The stable master
            Plr->Gossip_SendPOI(330.31, -4710.66, 7, 6, 0, "Shoja'my");
            SendQuickMenu(5973);
			break;

        case 5:     // A class trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4035, Plr);
            Menu->AddItem( 0, "Chasseur"       , 7);
			Menu->AddItem( 0, "Mage"         , 8);
			Menu->AddItem( 0, "Prêtre"       , 9);
			Menu->AddItem( 0, "Voleur"       , 10);
            Menu->AddItem( 0, "Chaman"      , 11);
			Menu->AddItem( 0, "Démoniste"     , 12);
            Menu->AddItem( 0, "Guerrier"     , 13);
            Menu->SendTo(Plr);
			break;

        case 6:     // A profession trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3541, Plr);
            Menu->AddItem( 0, "Alchimie"           , 14);
            Menu->AddItem( 0, "Forge"     , 15);
            Menu->AddItem( 0, "Cuisine"           , 16);
            Menu->AddItem( 0, "Enchantement"        , 17);
            Menu->AddItem( 0, "Ingénierie"       , 18);
            Menu->AddItem( 0, "Secourisme"         , 19);
            Menu->AddItem( 0, "Pêche"           , 20);
            Menu->AddItem( 0, "Herboristerie"         , 21);
			Menu->AddItem( 0, "Calligraphie"		, 22);
            Menu->AddItem( 0, "Travail du cuir"    , 23);
            Menu->AddItem( 0, "Minage"            , 24);
            Menu->AddItem( 0, "Dépeçage"          , 25);
            Menu->AddItem( 0, "Couture"         , 26);
            Menu->SendTo(Plr);
            break;


            ////////////////
            // Class trainer submenu
            ////////

        case 7: // Hunter
            Plr->Gossip_SendPOI(276, -4706.72, 7, 6, 0, "Thotar");
            SendQuickMenu(4013);
            break;        

        case 8: // Mage
            Plr->Gossip_SendPOI(-839.33, -4935.6, 7, 6, 0, "Un'Thuwa");
            SendQuickMenu(4014);
            break;

        case 9: // Priest
            Plr->Gossip_SendPOI(296.22, -4828.1, 7, 6, 0, "Tai'jin");
            SendQuickMenu(4015);
            break;

        case 10: // Rogue
            Plr->Gossip_SendPOI(265.76, -4709, 7, 6, 0, "Kaplak");
            SendQuickMenu(4016);
            break;

        case 11: // Shaman
            Plr->Gossip_SendPOI(307.79, -4836.97, 7, 6, 0, "Swart");
            SendQuickMenu(4017);
            break;

        case 12: // Warlock
            Plr->Gossip_SendPOI(355.88, -4836.45, 7, 6, 0, "Dhugru Soif-de-Carnage");
            SendQuickMenu(4018);
            break;

        case 13: // Warrior
            Plr->Gossip_SendPOI(312.3, -4824.66, 7, 6, 0, "Tarshaw Salbalaffre");
            SendQuickMenu(4019);
            break;

        case 14: // Alchemy
			Plr->Gossip_SendPOI(-800.25, -4894.33, 7, 6, 0, "Miao'zan");
            SendQuickMenu(4020);
            break;

        case 15: // Blacksmithing
			Plr->Gossip_SendPOI(373.24, -4716.45, 7, 6, 0, "Dwukk");
            SendQuickMenu(4021);
            break;

        case 16: // Cooking
            SendQuickMenu(4022);
            break;

        case 17: // Enchanting
			SendQuickMenu(4023);
            break;
			
        case 18: // Engineering
            Plr->Gossip_SendPOI(368.95, -4723.95, 7, 6, 0, "Mukdrak");
            SendQuickMenu(4024);
            break;

        case 19: // First Aid
            Plr->Gossip_SendPOI(327.17, -4825.62, 7, 6, 0, "Rawrk");
            SendQuickMenu(4025);
            break;

        case 20: // Fishing
            Plr->Gossip_SendPOI(-1065.48, -4777.43, 7, 6, 0, "Lau'Tiki");
            SendQuickMenu(4026);
            break;

        case 21: // Herbalism
			Plr->Gossip_SendPOI(-836.25, -4896.89, 7, 6, 0, "Mishiki");
            SendQuickMenu(4027);
            break;

		case 22: // Inscription
			Plr->Gossip_SendPOI(1839.56, -4477.24, 7, 6, 0, "Calligraphie d'Orgrimmar");
			SendQuickMenu(12850);
			break;

        case 23: // Leatherworking
            SendQuickMenu(4028);
            break;

        case 24: // Mining
			Plr->Gossip_SendPOI(366.94, -4705, 7, 6, 0, "Krunn");
            SendQuickMenu(4029);
            break;

        case 25: // Skinning
            Plr->Gossip_SendPOI(-2252.94, -291.32, 7, 6, 0, "Yonn Entaille-Profonde");
            SendQuickMenu(4030); 
            break;

		case 26: // Tailoring
            SendQuickMenu(4031);
            break;
        }
    }
};

class BrillGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
        Menu->AddItem(0, "La banque", 1);
        Menu->AddItem(0, "L'éleveur de chauve-souris", 2);
        Menu->AddItem(0, "L'auberge", 3);
        Menu->AddItem(0, "Le maître des écuries", 4);
        Menu->AddItem(0, "Un maître de classe", 5);
        Menu->AddItem(0, "Un maître de profession", 6);

        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // The bank
            SendQuickMenu(4074);
            break;

        case 2:     // The bat handler
            SendQuickMenu(4075);
            break;

        case 3:     // The inn
            Plr->Gossip_SendPOI(2246.68, 241.89, 7, 6, 0, "La taverne des Pendus"); 
            SendQuickMenu(4076);
			break;

        case 4:     // The stable master
            Plr->Gossip_SendPOI(2267.66, 319.32, 7, 6, 0, "Morganus");
            SendQuickMenu(5978);
			break;

        case 5:     // A class trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4292, Plr);
            Menu->AddItem( 0, "Mage"         , 7);
			Menu->AddItem( 0, "Paladin"      , 8);
			Menu->AddItem( 0, "Prêtre"       , 9);
			Menu->AddItem( 0, "Voleur"       , 10);
			Menu->AddItem( 0, "Démoniste"     , 11);
            Menu->AddItem( 0, "Guerrier"     , 12);
            Menu->SendTo(Plr);
			break;

        case 6:     // A profession trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4300, Plr);
            Menu->AddItem( 0, "Alchimie"           , 13);
            Menu->AddItem( 0, "Forge"     , 14);
            Menu->AddItem( 0, "Cuisine"           , 15);
            Menu->AddItem( 0, "Enchantement"        , 16);
            Menu->AddItem( 0, "Ingénierie"       , 17);
            Menu->AddItem( 0, "Secourisme"         , 18);
            Menu->AddItem( 0, "Pêche"           , 19);
            Menu->AddItem( 0, "Herboristerie"         , 20);
			Menu->AddItem( 0, "Calligraphie"		, 21);
            Menu->AddItem( 0, "Travail du cuir"    , 22);
            Menu->AddItem( 0, "Minage"            , 23);
            Menu->AddItem( 0, "Dépeçage"          , 24);
            Menu->AddItem( 0, "Couture"         , 25);
            Menu->SendTo(Plr);
            break;


            ////////////////
            // Class trainer submenu
            ////////

        case 7: // Mage
            Plr->Gossip_SendPOI(2259.18, 240.93, 7, 6, 0, "Cain Chantefeu");
            SendQuickMenu(4077);
            break;        

        case 8: // Paladin
            SendQuickMenu(4094);
            break;

        case 9: // Priest
            Plr->Gossip_SendPOI(2259.18, 240.93, 7, 6, 0, "Sombre clerc Beryl");
            SendQuickMenu(4078);
            break;

        case 10: // Rogue
            Plr->Gossip_SendPOI(2259.18, 240.93, 7, 6, 0, "Marion Call");
            SendQuickMenu(4079);
            break;

        case 11: // Warlock
            Plr->Gossip_SendPOI(2259.18, 240.93, 7, 6, 0, "Rupert Boch");
            SendQuickMenu(4080);
            break;

        case 12: // Warrior
            Plr->Gossip_SendPOI(2256.48, 240.32, 7, 6, 0, "Austil de Mon");
            SendQuickMenu(4081);
            break;

        case 13: // Alchemy
			Plr->Gossip_SendPOI(2263.25, 344.23, 7, 6, 0, "Carolai Anise");
            SendQuickMenu(4082);
            break;

        case 14: // Blacksmithing
            SendQuickMenu(4083);
            break;

        case 15: // Cooking
            SendQuickMenu(4084);
            break;

        case 16: // Enchanting
			Plr->Gossip_SendPOI(2250.35, 249.12, 7, 6, 0, "Vance Tristefosse");
			SendQuickMenu(4085);
            break;
			 
        case 17: // Engineering
            SendQuickMenu(4086);
            break;

        case 18: // First Aid
            Plr->Gossip_SendPOI(2246.68, 241.89, 7, 6, 0, "Infirmiere Neela");
            SendQuickMenu(4087);
            break;

        case 19: // Fishing
            Plr->Gossip_SendPOI(2292.37, -10.72, 7, 6, 0, "Clyde Kellen");
            SendQuickMenu(4088);
            break;

        case 20: // Herbalism
			Plr->Gossip_SendPOI(2268.21, 331.69, 7, 6, 0, "Faruza");
            SendQuickMenu(4089);
            break;

		case 21: // Inscription
			Plr->Gossip_SendPOI(1498.86, 293.03, 7, 6, 0, "Calligraphie de Fossoyeuse");
			SendQuickMenu(12846);
			break;

        case 22: // Leatherworking
			Plr->Gossip_SendPOI(2027, 78.72, 7, 6, 0, "Shelene Rhobart");
            SendQuickMenu(4090);
            break;

        case 23: // Mining
            SendQuickMenu(4091);
            break;

        case 24: // Skinning
            Plr->Gossip_SendPOI(2027, 78.72, 7, 6, 0, "Rand Rhobart");
            SendQuickMenu(4092); 
            break;

		case 25: // Tailoring
			Plr->Gossip_SendPOI(2160.45, 659.93, 7, 6, 0, "Bowen Brisboise");
            SendQuickMenu(4093);
            break;
        }
    }
};

class IronforgeGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2760, Plr);
        Menu->AddItem(0, "Hôtel des ventes", 1);
        Menu->AddItem(0, "Banque de Forgefer", 2);
        Menu->AddItem(0, "Le Tram des profondeurs", 3);
        Menu->AddItem(0, "Maître des griffons", 4);
        Menu->AddItem(0, "Maître des guildes", 5);
        Menu->AddItem(0, "L'auberge", 6);    
        Menu->AddItem(0, "Boite aux lettres", 7);
        Menu->AddItem(0, "Maître des écuries", 8);
        Menu->AddItem(0, "Maître d'armes", 9);
        Menu->AddItem(0, "Maître de guerre", 10);
		Menu->AddItem(0, "Salon de coiffure", 11);
        Menu->AddItem(0, "Maître de classe", 12);
        Menu->AddItem(0, "Maître de profession", 13);
		Menu->AddItem(0, "Lexique de puissance", 14);

        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // Auction House
            Plr->Gossip_SendPOI(-4957.39, -911.6, 7, 6, 0, "Hôtel des ventes de Forgefer");
            SendQuickMenu(3014);
			break;

        case 2:     // Bank of Ironforge
            Plr->Gossip_SendPOI(-4891.91, -991.47, 7, 6, 0, "Le Coffre");
            SendQuickMenu(2761);
			break;

        case 3:     // Deeprun Tram
            Plr->Gossip_SendPOI(-4835.27, -1294.69, 7, 6, 0, "Tram des profondeurs");
            SendQuickMenu(3814);
			break;

        case 4:     // Gryphon Master
            Plr->Gossip_SendPOI(-4821.52, -1152.3, 7, 6, 0, "Maître des griffons de Forgefer");
            SendQuickMenu(2762);
			break;

        case 5:     // Guild Master
            Plr->Gossip_SendPOI(-5021, -996.45, 7, 6, 0, "Centre des visiteurs de Forgefer");
            SendQuickMenu(2764);
			break;

        case 6:     // The Inn
            Plr->Gossip_SendPOI(-4850.47, -872.57, 7, 6, 0, "Taverne de Brûlepierre");
            SendQuickMenu(2768);
			break;

        case 7:     // Mailbox
            Plr->Gossip_SendPOI(-4845.7, -880.55, 7, 6, 0, "Boite aux lettres de Forgefer");
            SendQuickMenu(2769);
			break;

	    case 8:     // Stable Master
            Plr->Gossip_SendPOI(-5010.2, -1262, 7, 6, 0, "Ulbrek Brasemain");
			SendQuickMenu(5986);
			break;

        case 9:    // Weapon Trainer
			Plr->Gossip_SendPOI(-5040, -1201.88, 7, 6, 0, "Bixi et Buliwyf");
			SendQuickMenu(4518);
			break;

        case 10:    // Battlemaster
			Plr->Gossip_SendPOI(-5038.54, -1266.44, 7, 6, 0, "Maître de guerre de Forgefer");
			SendQuickMenu(7698);
			break;

		case 11:	// Barbershop
			Plr->Gossip_SendPOI(-4841.25, -912.41, 7, 6, 0, "Salon de coiffure de Forgefer");
			SendQuickMenu(12852);
			break;

        case 12:    // A class trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2766, Plr);
            Menu->AddItem( 0, "Chasseur"      , 15);
            Menu->AddItem( 0, "Mage"        , 16);
            Menu->AddItem( 0, "Paladin"     , 17);
            Menu->AddItem( 0, "Prêtre"      , 18);
            Menu->AddItem( 0, "Voleur"       , 19);
            Menu->AddItem( 0, "Démoniste"     , 20);
            Menu->AddItem( 0, "Guerrier"     , 21);
            Menu->AddItem( 0, "Chaman"      , 22);
            Menu->SendTo(Plr);
            break;

        case 13:    // A profession trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2793, Plr);
            Menu->AddItem( 0, "Alchimie"           , 23);
            Menu->AddItem( 0, "Forge"     , 24);
            Menu->AddItem( 0, "Cuisine"           , 25);
            Menu->AddItem( 0, "Enchantement"        , 26);
			Menu->AddItem( 0, "Ingénierie"       , 27);
            Menu->AddItem( 0, "Secourisme"         , 28);
            Menu->AddItem( 0, "Pêche"           , 29);
            Menu->AddItem( 0, "Herboristerie"         , 30);
			Menu->AddItem( 0, "Calligraphie"		, 31);
            Menu->AddItem( 0, "Travail du cuir"    , 32);
            Menu->AddItem( 0, "Minage"            , 33);
            Menu->AddItem( 0, "Dépeçage"          , 34);
            Menu->AddItem( 0, "Couture"         , 35);
            Menu->SendTo(Plr);
            break;

		case 14: // Lexicon of power
			Plr->Gossip_SendPOI(-4804.38, -1188.36, 7, 6, 0, "Calligraphie de Forgefer");
			SendQuickMenu(12843);
			break;

            ////////////////
            // Class trainer submenu
            ////////

        case 15: // Hunter
            Plr->Gossip_SendPOI(-5023, -1253.68, 7, 6, 0, "Hall des armes");
            SendQuickMenu(2770);
            break;

        case 16: // Mage
            Plr->Gossip_SendPOI(-4627, -926.45, 7, 6, 0, "Hall des mystères");
            SendQuickMenu(2771);
            break;

        case 17: // Paladin
            Plr->Gossip_SendPOI(-4627.02, -926.45, 7, 6, 0, "Hall des mystères");
            SendQuickMenu(2773);
            break;

        case 18: // Priest
            Plr->Gossip_SendPOI(-4627, -926.45, 7, 6, 0, "Hall des mystères");
            SendQuickMenu(2772);
            break;

        case 19: // Rogue
            Plr->Gossip_SendPOI(-4647.83, -1124, 7, 6, 0, "Maître des voleurs de Forgefer");
            SendQuickMenu(2774);
            break;

        case 20: // Warlock
            Plr->Gossip_SendPOI(-4605, -1110.45, 7, 6, 0, "Maître des démonistes de Forgefer");
            SendQuickMenu(2775);
            break;

        case 21: // Warrior
            Plr->Gossip_SendPOI(-5023.08, -1253.68, 7, 6, 0, "Hall des armes");
            SendQuickMenu(2776);
            break;

        case 22: // Shaman
            Plr->Gossip_SendPOI(-4722.02, -1150.66, 7, 6, 0, "Maître des chamans de Forgefer");
            SendQuickMenu(1299);
            break;

        case 23: // Alchemy
            Plr->Gossip_SendPOI(-4858.5, -1241.83, 7, 6, 0, "Potions et mixtures de Baiedesbulles");
            SendQuickMenu(2794);
            break;

        case 24: // Blacksmithing
            Plr->Gossip_SendPOI(-4796.97, -1110.17, 7, 6, 0, "La Grande Forge");
            SendQuickMenu(2795);
            break;

        case 25: // Cooking
            Plr->Gossip_SendPOI(-4767.83, -1184.59, 7, 6, 0, "La bouilloire de bronze");
            SendQuickMenu(2796);
            break;

        case 26: // Enchanting
            Plr->Gossip_SendPOI(-4803.72, -1196.53, 7, 6, 0, "Arcanes de Duvechardon");
            SendQuickMenu(2797);
            break;

		case 27: // Engineering
            Plr->Gossip_SendPOI(-4799.56, -1250.23, 7, 6, 0, "Les gadjets de Fuseressort");
            SendQuickMenu(2798);
            break;

        case 28: // First Aid
            Plr->Gossip_SendPOI(-4881.6, -1153.13, 7, 6, 0, "Médecin de Forgefer");
            SendQuickMenu(2799);
            break;

        case 29: // Fishing
            Plr->Gossip_SendPOI(-4597.91, -1091.93, 7, 6, 0, "Pecheur itinerant");
            SendQuickMenu(2800);
            break;

        case 30: // Herbalism
            Plr->Gossip_SendPOI(-4876.9, -1151.92, 7, 6, 0, "Médecin de Forgefer");
            SendQuickMenu(2801);
            break;

		case 31: // Inscription
			Plr->Gossip_SendPOI(-4804.38, -1188.36, 7, 6, 0, "Calligraphie de Forgefer");
			SendQuickMenu(12853);
			break;

        case 32: // Leatherworking
            Plr->Gossip_SendPOI(-4745, -1027.57, 7, 6, 0, "Cuirs de Finfuseau");
            SendQuickMenu(2802);
            break;

        case 33: // Mining
            Plr->Gossip_SendPOI(-4705.06, -1116.43, 7, 6, 0, "Guilde des mineurs de la Montagne profonde");
            SendQuickMenu(2804);
            break;

        case 34: // Skinning
            Plr->Gossip_SendPOI(-4745, -1027.57, 7, 6, 0, "Cuirs de Finfuseau");
            SendQuickMenu(2805); 
            break;

		case 35: // Tailoring
            Plr->Gossip_SendPOI(-4719.60, -1056.96, 7, 6, 0, "Vêtements de Front-de-Pierre");
            SendQuickMenu(2807);
            break;
        }
    }
};

class KharanosGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
        Menu->AddItem(0, "Banque", 1);
        Menu->AddItem(0, "Maître des griffons", 2);
        Menu->AddItem(0, "Maître des guildes", 3);
        Menu->AddItem(0, "L'auberge", 4);
        Menu->AddItem(0, "Maître des écuries", 5);
        Menu->AddItem(0, "Maître de classe", 6);
        Menu->AddItem(0, "Maître de profession", 7);

        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     //Bank
            SendQuickMenu(4288);
            break;

        case 2:     //Gryphon Master
            SendQuickMenu(4289);
            break;

        case 3:     //Guild Master
            SendQuickMenu(4290);
			break;
			
        case 4:     //The Inn
            Plr->Gossip_SendPOI(-5582.66, -525.89, 7, 6, 0, "Distillerie Tonnebiere");
            SendQuickMenu(4291);
			break;

        case 5:     //Stable Master
            Plr->Gossip_SendPOI(-5604, -509.58, 7, 6, 0, "Shelby Pierre-a-feu");
            SendQuickMenu(5985);
			break;

        case 6:     //Class Trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4292, Plr);
            Menu->AddItem( 0, "Chasseur"       , 8);
			Menu->AddItem( 0, "Mage"         , 9);
			Menu->AddItem( 0, "Paladin"      ,10);
			Menu->AddItem( 0, "Prêtre"      , 11);
            Menu->AddItem( 0, "Voleur"       , 12);
			Menu->AddItem( 0, "Démoniste"     , 13);
            Menu->AddItem( 0, "Guerrier"     , 14);
            Menu->SendTo(Plr);
			break;

        case 7:     // Profession Trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4300, Plr);
            Menu->AddItem( 0, "Alchimie"           , 15);
            Menu->AddItem( 0, "Forge"     , 16);
            Menu->AddItem( 0, "Cuisine"           , 17);
            Menu->AddItem( 0, "Enchantement"        , 18);
            Menu->AddItem( 0, "Ingénierie"       , 19);
            Menu->AddItem( 0, "Secourisme"         , 20);
            Menu->AddItem( 0, "Pêche"           , 21);
            Menu->AddItem( 0, "Herboristerie"         , 22);
			Menu->AddItem( 0, "Calligraphie"		, 23);
            Menu->AddItem( 0, "Travail du cuir"    , 24);
            Menu->AddItem( 0, "Minage"            , 25);
            Menu->AddItem( 0, "Dépeçage"          , 26);
            Menu->AddItem( 0, "Couture"         , 27);
            Menu->SendTo(Plr);
            break;

            ////////////////
            // Class trainer submenu
            ////////

        case 8: // Hunter
            Plr->Gossip_SendPOI(-5618.29, -454.25, 7, 6, 0, "Grif Coeur-sauvage");
            SendQuickMenu(4293);
            break;

        case 9: // Mage
            Plr->Gossip_SendPOI(-5585.6, -539.99, 7, 6, 0, "Magis Mantincelle");
            SendQuickMenu(4294);
            break;

        case 10: // Paladin
            Plr->Gossip_SendPOI(-5585.6, -539.99, 7, 6, 0, "Azar Fortmartel");
            SendQuickMenu(4295);
            break;

        case 11: // Priest
            Plr->Gossip_SendPOI(-5591.74, -525.61, 7, 6, 0, "Maxan Anvol");
            SendQuickMenu(4296);
            break;

        case 12: // Rogue
            Plr->Gossip_SendPOI(-5602.75, -542.4, 7, 6, 0, "Hogral Bakkan");
            SendQuickMenu(4297);
            break;

        case 13: // Warlock
            Plr->Gossip_SendPOI(-5641.97, -523.76, 7, 6, 0, "Gimrizz Sombrouage");
            SendQuickMenu(4298);
            break;

        case 14: // Warrior
            Plr->Gossip_SendPOI(-5604.79, -529.38, 7, 6, 0, "Granis Vivehache");
            SendQuickMenu(4299);
            break;

        case 15: // Alchemy
            SendQuickMenu(4301);
            break;

        case 16: // Blacksmithing
			Plr->Gossip_SendPOI(-5584.72, -428.41, 7, 6, 0, "Tognus Flammesilex");
            SendQuickMenu(4302);
            break;

        case 17: // Cooking
			Plr->Gossip_SendPOI(-5596.85, -541.43, 7, 6, 0, "Gremlock Pilsnor");
            SendQuickMenu(4303);
            break;

        case 18: // Enchanting
			SendQuickMenu(4304);
            break;
			
        case 19: // Engineering
            SendQuickMenu(4305);
            break;

        case 20: // First Aid
            Plr->Gossip_SendPOI(-5603.67, -523.57, 7, 6, 0, "Thamner Pol");
            SendQuickMenu(4306);
            break;

        case 21: // Fishing
            Plr->Gossip_SendPOI(-5202.39, -51.36, 7, 6, 0, "Paxton Ganter");
            SendQuickMenu(4307);
            break;

        case 22: // Herbalism
            SendQuickMenu(4308);
            break;

		case 23: // Inscription
			Plr->Gossip_SendPOI(-4804.38, -1188.36, 7, 6, 0, "Calligraphie de Forgefer");
			SendQuickMenu(12853);
			break;

        case 24: // Leatherworking
            SendQuickMenu(4310);
            break;

        case 25: // Mining
			Plr->Gossip_SendPOI(-5531, -666.53, 7, 6, 0, "Yarr Martelepierre");
            SendQuickMenu(4311);
            break;

        case 26: // Skinning
            SendQuickMenu(4312);
            break;

		case 27: //Tailoring
            SendQuickMenu(4313);
            break;
        }
    }
};

class FalconwingGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
        Menu->AddItem(0, "Le maître des faucons-dragons", 1);
        Menu->AddItem(0, "Le maître des guildes", 2);
        Menu->AddItem(0, "L'auberge", 3);
        Menu->AddItem(0, "Maître des écuries", 4);
        Menu->AddItem(0, "Maître de classe", 5);
        Menu->AddItem(0, "Maître de profession", 6);

        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // Dragonhawk Handler
			Plr->Gossip_SendPOI(9376.4, -7164.92, 7, 6, 0, "Lune-d'argent, maître de vol");
            SendQuickMenu(3695);
			break;

        case 2:     // Guild Master
            SendQuickMenu(3696);
			break;
			
        case 3:     // The Inn
            Plr->Gossip_SendPOI(9476.916, -6859.2, 7, 6, 0, "Place de l'épervier, aubergiste");
            SendQuickMenu(3697);
			break;

        case 4:     // Stable Master
            Plr->Gossip_SendPOI(9487.6, -6830.59, 7, 6, 0, "Place de l'épervier, maître des écuries");
            SendQuickMenu(3698);
			break;

        case 5:     // Class Trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4292, Plr);
            Menu->AddItem( 0, "Druide"        , 7);
			Menu->AddItem( 0, "Chasseur"       , 8);
			Menu->AddItem( 0, "Mage"         , 9);
			Menu->AddItem( 0, "Paladin"     , 10);
            Menu->AddItem( 0, "Prêtre"      , 11);
			Menu->AddItem( 0, "Voleur"       , 12);
            Menu->AddItem( 0, "Démoniste"     , 13);
            Menu->SendTo(Plr);
			break;

        case 6:     // Profession Trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
            Menu->AddItem( 0, "Alchimie"           , 14);
            Menu->AddItem( 0, "Forge"     , 15);
            Menu->AddItem( 0, "Cuisine"           , 16);
            Menu->AddItem( 0, "Enchantement"        , 17);
            Menu->AddItem( 0, "Ingénierie"       , 18);
            Menu->AddItem( 0, "Secourisme"         , 19);
            Menu->AddItem( 0, "Pêche"           , 20);
            Menu->AddItem( 0, "Herboristerie"         , 21);
			Menu->AddItem( 0, "Calligraphie"		, 22);
			Menu->AddItem( 0, "Joaillierie"     , 23);
            Menu->AddItem( 0, "Travail du cuir"    , 24);
            Menu->AddItem( 0, "Minage"            , 25);
            Menu->AddItem( 0, "Dépeçage"          , 26);
            Menu->AddItem( 0, "Couture"         , 27);
            Menu->SendTo(Plr);
            break;

            ////////////////
            // Class trainer submenu
            ////////

        case 7: // Druid
            SendQuickMenu(3699);
            break;

        case 8: // Hunter
            Plr->Gossip_SendPOI(9529.2, -6864.58, 7, 6, 0, "Hannovia");
            SendQuickMenu(3700);
            break;

        case 9: // Mage
            Plr->Gossip_SendPOI(9462.24, -6853.45, 7, 6, 0, "Garridel");
            SendQuickMenu(3701);
            break;

        case 10: // Paladin
            Plr->Gossip_SendPOI(9516.05, -6870.96, 7, 6, 0, "Noellene");
            SendQuickMenu(3702);
            break;

        case 11: // Priest
            Plr->Gossip_SendPOI(9466.62, -6844.23, 7, 6, 0, "Ponaris");
            SendQuickMenu(3703);
            break;

        case 12: // Rogue
            Plr->Gossip_SendPOI(9534.15, -6876.0, 7, 6, 0, "Tannaria");
            SendQuickMenu(3704);
            break;

        case 13: // Warlock
            Plr->Gossip_SendPOI(9467.63, -6862.82, 7, 6, 0, "Celoenus");
            SendQuickMenu(3705);
            break;

        case 14: // Alchemy
			Plr->Gossip_SendPOI(8661.36, -6367.0, 7, 6, 0, "Havre de Saltheril, alchimiste");
            SendQuickMenu(3706);
            break;

        case 15: // Blacksmithing
			Plr->Gossip_SendPOI(8986.43, -7419.07, 7, 6, 0, "La retraite des Pérégrins, forgeron");
            SendQuickMenu(3707);
            break;

        case 16: // Cooking
			Plr->Gossip_SendPOI(9494.86, -6879.45, 7, 6, 0, "Quarelestra");
            SendQuickMenu(3708);
            break;

        case 17: // Enchanting
			Plr->Gossip_SendPOI(8657.6, -6366.7, 7, 6, 0, "Havre de Saltheril, enchanteur");
			SendQuickMenu(3709);
            break;
			
        case 18: // Engineering
            SendQuickMenu(3710);
            break;

        case 19: // First Aid
            Plr->Gossip_SendPOI(9479.53, -6880.07, 7, 6, 0, "Kanaria");
            SendQuickMenu(3711);
            break;

        case 20: // Fishing
            SendQuickMenu(3712);
            break;

        case 21: // Herbalism
            Plr->Gossip_SendPOI(8678.92, -6329.09, 7, 6, 0, "Havre de Saltheril, herboriste");
            SendQuickMenu(3713);
            break;

		case 22: // Inscription
			Plr->Gossip_SendPOI(9962.57, -7246.18, 7, 6, 0, "Calligraphie de Lune-d'argent");
			SendQuickMenu(12848);
			break;

        case 23: // Jewelcrafting
			Plr->Gossip_SendPOI(9484.79, -6876.58, 7, 6, 0, "Aleinia");
            SendQuickMenu(3714);
            break;

        case 24: // Leatherworking
			Plr->Gossip_SendPOI(9363.75, -7130.75, 7, 6, 0, "Bois des chants éternels, travailleur du cuir");
            SendQuickMenu(3715);
            break;

        case 25: // Mining
            SendQuickMenu(3716);
            break;

        case 26: // Skinning
			Plr->Gossip_SendPOI(9362.89, -7134.58, 7, 6, 0, "Bois des chants éternels, dépeceur");
            SendQuickMenu(3717);
            break;

		case 27: // Tailoring
			Plr->Gossip_SendPOI(8680.36, -6327.51, 7, 6, 0, "Havre de Saltheril, tailleur");
            SendQuickMenu(3718);
            break;
        }
    }
};

class AzureWatchGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}

    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
        Menu->AddItem(0, "Banque", 1);
        Menu->AddItem(0, "Maître des hippogriffes", 2);
        Menu->AddItem(0, "Maître des guildes", 3);
        Menu->AddItem(0, "Auberge", 4);
        Menu->AddItem(0, "Maître des écurie", 5);
        Menu->AddItem(0, "Maître de classe", 6);
		Menu->AddItem(0, "Maître de profession", 7);
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     //Bank
            SendQuickMenu(3719);
            break;

        case 2:     //Hippogryph Master
            SendQuickMenu(3720);
			break;
			
        case 3:     //Guild Master
            SendQuickMenu(3721);
			break;

        case 4:     //Inn
            Plr->Gossip_SendPOI(-4127.81, -12467.7, 7, 6, 0, "Guet d'Azur, aubergiste");
            SendQuickMenu(3722);
			break;

        case 5:     // Stable Master
            Plr->Gossip_SendPOI(-4146.42, -12492.7, 7, 6, 0, "Guet d'Azur, maître des écuries");
            SendQuickMenu(3723);
			break;

        case 6:     // Class Trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4292, Plr);
            Menu->AddItem( 0, "Druide"        , 8);
			Menu->AddItem( 0, "Chasseur"       , 9);
			Menu->AddItem( 0, "Mage"        , 10);
			Menu->AddItem( 0, "Paladin"     , 11);
            Menu->AddItem( 0, "Prêtre"      , 12);
			Menu->AddItem( 0, "Chaman"      , 13);
            Menu->AddItem( 0, "Guerrier"     , 14);
            Menu->SendTo(Plr);
			break;

        case 7:     // Profession Trainer
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
            Menu->AddItem( 0, "Alchimie"           , 15);
            Menu->AddItem( 0, "Forge"     , 16);
            Menu->AddItem( 0, "Cuisine"           , 17);
            Menu->AddItem( 0, "Enchantement"        , 18);
            Menu->AddItem( 0, "Ingénierie"       , 19);
            Menu->AddItem( 0, "Secourisme"         , 20);
            Menu->AddItem( 0, "Pêche"           , 21);
            Menu->AddItem( 0, "Herboristerie"         , 22);
			Menu->AddItem( 0, "Calligraphie"		, 23);
			Menu->AddItem( 0, "Joaillierie"     , 24);
            Menu->AddItem( 0, "Travail du cuir"    , 25);
            Menu->AddItem( 0, "Minage"            , 26);
            Menu->AddItem( 0, "Dépeçage"          , 27);
            Menu->AddItem( 0, "Couture"         , 28);
            Menu->SendTo(Plr);
            break;

            ////////////////
            // Class trainer submenu
            ////////

        case 8: // Druid
            SendQuickMenu(3724);
            break;

        case 9: // Hunter
            Plr->Gossip_SendPOI(-4203.65, -12467.7, 7, 6, 0, "Guet d'Azur, maître des chasseurs");
            SendQuickMenu(3725);
            break;

        case 10: // Mage
            Plr->Gossip_SendPOI(-4149.62, -12530.1, 7, 6, 0, "Guet d'Azur, maître des mages");
            SendQuickMenu(3726);
            break;

        case 11: // Paladin
            Plr->Gossip_SendPOI(-4138.98, -12468.5, 7, 6, 0, "Guet d'Azur, maître des paladins");
            SendQuickMenu(3727);
            break;

        case 12: // Priest
            Plr->Gossip_SendPOI(-4131.66, -12478.6, 7, 6, 0, "Guet d'Azur, maître des prêtres");
            SendQuickMenu(3728);
            break;

        case 13: // Shaman
            Plr->Gossip_SendPOI(-4162.33, -12456.1, 7, 6, 0, "Guet d'Azur, maître des chamans");
            SendQuickMenu(3729);
            break;

        case 14: // Warrior
            Plr->Gossip_SendPOI(-4165.05, -12536.4, 7, 6, 0, "Guet d'Azur, maître des guerriers");
            SendQuickMenu(3730);
            break;

        case 15: // Alchemy
            Plr->Gossip_SendPOI(-4191.15, -12470, 7, 6, 0, "Guet d'Azur, alchimiste");
            SendQuickMenu(3731);
            break;

        case 16: // Blacksmithing
			Plr->Gossip_SendPOI(-4726.29, -12387.0, 7, 6, 0, "Point d'accostage d'Odesyus, forgeron");
            SendQuickMenu(3732);
            break;

        case 17: // Cooking
			Plr->Gossip_SendPOI(-4708.59, -12400.3, 7, 6, 0, "Point d'accostage d'Odesyus, cuisinier");
            SendQuickMenu(3733);
            break;

        case 18: // Enchanting
			SendQuickMenu(3734);
            break;
			
        case 19: // Engineering
            Plr->Gossip_SendPOI(-4157.57, -12470.2, 7, 6, 0, "Guet d'Azur, ingénieur");
            SendQuickMenu(3735);
            break;

        case 20: // First Aid
            Plr->Gossip_SendPOI(-4199.1, -12469.9, 7, 6, 0, "Guet d'Azur, maître des secouristes");
            SendQuickMenu(3736);
            break;

        case 21: // Fishing
            Plr->Gossip_SendPOI(-4266.34, -12985.4, 7, 6, 0, "Guet d'Ammen, pêcheuse");
            SendQuickMenu(3737);
            break;

        case 22: // Herbalism
            Plr->Gossip_SendPOI(-4189.43, -12459.4, 7, 6, 0, "Guet d'Azur, herboriste");
            SendQuickMenu(3738);
            break;

		case 23: // Inscription
			Plr->Gossip_SendPOI(-3889.3, -11495, 7, 6, 0, "Calligraphie de l'Exodar");
			SendQuickMenu(12849);
			break;

        case 24: // Jewelcrafting
            SendQuickMenu(3739);
            break;

        case 25: // Leatherworking
            Plr->Gossip_SendPOI(-3442.68, -12322.2, 7, 6, 0, "Repaire des Calmepins, travailleur du cuir");
            SendQuickMenu(3740);
            break;

        case 26: //Mining
			Plr->Gossip_SendPOI(-4179.89, -12493.1, 7, 6, 0, "Guet d'Azur, maître des mineurs");
            SendQuickMenu(3741);
            break;

        case 27: // Skinning
			Plr->Gossip_SendPOI(-3431.17, -12316.5, 7, 6, 0, "Repaire des Calmepins, dépeceur");
            SendQuickMenu(3742);
            break;

		case 28: //Tailoring
			Plr->Gossip_SendPOI(-4711.54, -12386.7, 7, 6, 0, "Point d'accostage d'Odesyus, tailleur");
            SendQuickMenu(3743);
            break;
        }
    }
};

/*****************************************************************************************/
/* Shattrath Guards   original structure by AeThIs. Translated, updated and  by Pepsi1x1 */
/*****************************************************************************************/

class ShattrathGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);
        Menu->AddItem(0, "Taverne de la Fin du monde", 1);
        Menu->AddItem(0, "Banque", 2);
        Menu->AddItem(0, "Auberge", 3);
        Menu->AddItem(0, "Maître de vol", 4);
        Menu->AddItem(0, "Boite aux lettres", 5);    
        Menu->AddItem(0, "Maître des écuries", 6);
        Menu->AddItem(0, "Maître de guerre", 7);
        Menu->AddItem(0, "Maître de profession", 8);
        Menu->AddItem(0, "Métier à tisser le mana", 9);
        Menu->AddItem(0, "Laboratoire d'alchimie", 10);
        Menu->AddItem(0, "Marchand de gemmes", 11);

        if(AutoSend)
            Menu->SendTo(Plr);
    }

   void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Menus
            /////

        case 1:     // World's End Tavern
            Plr->Gossip_SendPOI(-1760.4, 5166.9, 7, 6, 0, "Shattrath, la Cantina");
            SendQuickMenu(10394);
			break;

        case 2:     // Shattrath Banks
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10395, Plr);
            Menu->AddItem( 0, "Banque de l'Aldor"          , 12);
            Menu->AddItem( 0, "Banque des Clairvoyants"         , 13);
            Menu->SendTo(Plr);
            break;

        case 3:     // Inn's
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10398, Plr);
            Menu->AddItem( 0, "Auberge de l'Aldor"          , 14);
            Menu->AddItem( 0, "Auberge des Clairvoyants"         , 15);
            Menu->SendTo(Plr);
            break;

        case 4:     // Gryphon Master
            Plr->Gossip_SendPOI(-1831.9, 5298.2, 7, 6, 0, "Transports de Shattrath");
            SendQuickMenu(10402);
			break;

        case 5:     // Mailboxes
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10403, Plr);
            Menu->AddItem( 0, "Auberge de l'Aldor"          , 16);
            Menu->AddItem( 0, "Auberge des Clairvoyants"         , 17);
			Menu->AddItem( 0, "Banque de l'Aldor"          , 18);
            Menu->AddItem( 0, "Banque des Clairvoyants"         , 19);
            Menu->SendTo(Plr);
            break;

        case 6:     // Stable Masters
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10404, Plr);
            Menu->AddItem( 0, "Etable de l'Aldor"          , 20);
            Menu->AddItem( 0, "Etable des Clairvoyants"         , 21);
            Menu->SendTo(Plr);
            break;

        case 7:     // Battlemasters
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10405, Plr);
            Menu->AddItem( 0, "Maître de guerre de l'Alliance"          , 22);
            Menu->AddItem( 0, "Maître de guerre de la Horde"         , 23);
			Menu->AddItem( 0, "Maître des arènes"		, 24);
            Menu->SendTo(Plr);
            break;

        case 8:     // Profession Trainers
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10391, Plr);
            Menu->AddItem( 0, "Alchimie"          , 25);
            Menu->AddItem( 0, "Forge"         , 26);
			Menu->AddItem( 0, "Cuisine"          , 27);
            Menu->AddItem( 0, "Enchantement"         , 28);
            Menu->AddItem( 0, "Secourisme"          , 29);
            Menu->AddItem( 0, "Joaillierie"         , 30);
			Menu->AddItem( 0, "Travail du cuir"          , 31);
            Menu->AddItem( 0, "Dépeçage"         , 32);
            Menu->SendTo(Plr);
            break;

        case 9:     // Mana Loom
            Plr->Gossip_SendPOI(-2073.9, 5265.7, 7, 6, 0, "Shattrath, métier à tisser le mana");
            SendQuickMenu(10408);
			break;

        case 10:    // Alchemy Lab
            Plr->Gossip_SendPOI(-1648.1, 5537.3, 7, 6, 0, "Laboratoire d'alchimie de Shattrath");
            SendQuickMenu(10409);
			break;

        case 11:    // Gem Merchants
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10410, Plr);
            Menu->AddItem( 0, "Marchand de gemmes de l'Aldor"          , 33);
            Menu->AddItem( 0, "Marchand de gemmes des Clairvoyants"         , 34);
            Menu->SendTo(Plr);
            break;


            //////////////////////
            // Banks
            /////


        case 12: // Aldor Bank
            Plr->Gossip_SendPOI(-1730.8, 5496.2, 7, 6, 0, "Shattrath, banque de l'Aldor");
            SendQuickMenu(10396);
            break;

        case 13: // Clairvoyants Bank
            Plr->Gossip_SendPOI(-1999.6, 5362.0, 7, 6, 0, "Shattrath, banque des Clairvoyants");
            SendQuickMenu(10397);
            break;


            //////////////////////
            // Inns
            /////


        case 14: // Aldor Inn
            Plr->Gossip_SendPOI(-1897.5, 5767.6, 7, 6, 0, "Auberge de l'Aldor de Shattrath");
            SendQuickMenu(10399);
            break;

        case 15: // Clairvoyants Inn
            Plr->Gossip_SendPOI(-2180.6, 5403.9, 7, 6, 0, "Auberge des Clairvoyants de Shattrath");
            SendQuickMenu(10401);
            break;


            //////////////////////
            // Mailboxes
            /////


        case 16: // Aldor Inn
			Plr->Gossip_SendPOI(-1886.9, 5761.5, 7, 6, 0, "Auberge de l'Aldor de Shattrath");
            SendQuickMenu(10399);
            break;

        case 17: // Clairvoyants Bank
            Plr->Gossip_SendPOI(-2175.0, 5411.7, 7, 6, 0, "Shattrath, banque des Clairvoyants");
            SendQuickMenu(10397);
            break;

        case 18: // Aldor Bank
            Plr->Gossip_SendPOI(-1695.5, 5521.8, 7, 6, 0, "Shattrath, banque de l'Aldor");
            SendQuickMenu(10396);
            break;

        case 19: // Clairvoyants Inn
            Plr->Gossip_SendPOI(-2033.0, 5336.1, 7, 6, 0, "Auberge des Clairvoyants de Shattrath");
            SendQuickMenu(10401);
            break;


            //////////////////////
            // Stable Masters
            /////


        case 20: // Aldor Stable Master
            Plr->Gossip_SendPOI(-1889.6, 5761.5, 7, 6, 0, "Ecuries de Shattrath, Aldor");
            SendQuickMenu(10399);
            break;

        case 21: // Clairvoyants Stable Master
            Plr->Gossip_SendPOI(-2170.0, 5404.6, 7, 6, 0, "Ecuries de Shattrath, Clairvoyants");
            SendQuickMenu(10401);
            break;


            //////////////////////
            // Battlemasters
            /////


        case 22: // Alliance Battlemaster
            Plr->Gossip_SendPOI(-1831.9, 5298.2, 7, 6, 0, "Maître de guerre de l'Alliance de Shattrath");
            SendQuickMenu(10406);
            break;

        case 23: // Horde Battle Master 
            Plr->Gossip_SendPOI(-1968.7, 5262.2, 7, 6, 0, "Maître de guerre de la Horde de Shattrath");
            SendQuickMenu(10407);
            break;

		case 24: // Arena Battle Master
			Plr->Gossip_SendPOI(-1961.82, 5177.22, 7, 6, 0, "Maître de guerre de l'arène de Shattrath");
			SendQuickMenu(10420);


            //////////////////////
            // Profession Trainers
            /////


        case 25: // Alchemy
            Plr->Gossip_SendPOI(-1661.0, 5538, 7, 6, 0, "Alchimiste de Shattrath");
            SendQuickMenu(10413);
            break;

        case 26: // Blacksmithing
            Plr->Gossip_SendPOI(-1847.7, 5230.3, 7, 6, 0, "Fabricant d'armes et armures de Shattrath");
            SendQuickMenu(10400);
            break;

        case 27: // Cooking
            Plr->Gossip_SendPOI(-2067.4, 5316.5, 7, 6, 0, "Cuisinier de Shattrath");
            SendQuickMenu(10414);
            break;

        case 28: // Enchanting
            Plr->Gossip_SendPOI(-2278.5, 5567.7, 7, 6, 0, "Enchanteurs de Shattrath");
            SendQuickMenu(10415);
            break;

        case 29: // First Aid
            Plr->Gossip_SendPOI(-1592.0, 5263.7, 7, 6, 0, "Secourisme de Shattrath");
            SendQuickMenu(10416);
            break;

        case 30: // Jewelcrafting
			Plr->Gossip_SendPOI(-1653.3, 5665.1, 7, 6, 0, "Joaillier de Shattrath");
            SendQuickMenu(10417);
            break;

        case 31: // Leatherworking
            Plr->Gossip_SendPOI(-2060.9, 5256.6, 7, 6, 0, "Travailleur du cuir de Shattrath");
            SendQuickMenu(10418);
            break;

        case 32: // Skinning
            Plr->Gossip_SendPOI(-2047.9, 5299.6, 7, 6, 0, "Dépeceur de Shattrath");
            SendQuickMenu(10419);
            break;


            //////////////////////
            // Gem Merchants
            /////


        case 33: //Aldor gem merchant
            Plr->Gossip_SendPOI(-1649.3, 5668.6, 7, 6, 0, "Vendeur de gemmes de l'Aldor");
            SendQuickMenu(10411);
            break;

        case 34: //Clairvoyants gem merchant
            Plr->Gossip_SendPOI(-2193.9, 5422.1, 7, 6, 0, "Vendeur de gemmes des Clairvoyants");
            SendQuickMenu(10412);
            break;
        }
    }
};
void SetupGuardGossip(ScriptMgr * mgr)
{
	GossipScript * gold = (GossipScript*) new GoldshireGuard();
	GossipScript * sw = (GossipScript*) new StormwindGuard();
	GossipScript * darn = (GossipScript*) new DarnassusGuard();
	GossipScript * dol = (GossipScript*) new DolanaarGuard();
	GossipScript * blood = (GossipScript*) new BloodhoofGuard();
	GossipScript * razor = (GossipScript*) new RazorHillGuard();
	GossipScript * brill = (GossipScript*) new BrillGuard();
	GossipScript * irf = (GossipScript*) new IronforgeGuard();
	GossipScript * khar = (GossipScript*) new KharanosGuard();
	GossipScript * falcon = (GossipScript*) new FalconwingGuard();
	GossipScript * azure = (AzureWatchGuard*) new AzureWatchGuard();
	GossipScript * under = (GossipScript*) new UndercityGuard();
	GossipScript * silver = (SilvermoonGuard*) new SilvermoonGuard();
	GossipScript * exodar = (ExodarGuard*) new ExodarGuard();
	GossipScript * ogri = (OrgrimmarGuard*) new OrgrimmarGuard();
	GossipScript * thun = (ThunderbluffGuard*) new ThunderbluffGuard();
	GossipScript * shattr = (ShattrathGuard*) new ShattrathGuard();

    /* Guard List */
    mgr->register_gossip_script(1423,  gold);        // Stormwind Guard 
    mgr->register_gossip_script(68,    sw);        // Stormwind City Guard
    mgr->register_gossip_script(1976,  sw);        // Stormwind City Patroller
    mgr->register_gossip_script(4262,  darn);        // Darnassus Sentinel
    mgr->register_gossip_script(5624,  under);		// Undercity Guardian
    mgr->register_gossip_script(3571,  dol);			// Teldrassil Sentinel
	mgr->register_gossip_script(16222, silver);	    // Silvermoon City Guardian
	mgr->register_gossip_script(16733, exodar);	        // Exodar Peacekeeper
	mgr->register_gossip_script(20674, exodar);	        // Shield of Velen
	mgr->register_gossip_script(3296, ogri);	        // Orgrimmar Grunt
	mgr->register_gossip_script(3084, thun);	    // Bluffwatcher
	mgr->register_gossip_script(3222, blood);			// Brave Wildrunner
	mgr->register_gossip_script(3224, blood);			// Brave Cloudmane
	mgr->register_gossip_script(3220, blood);			// Brave Darksky
	mgr->register_gossip_script(3219, blood);			// Brave Leaping Deer
	mgr->register_gossip_script(3217, blood);			// Brave Dawneagle
	mgr->register_gossip_script(3215, blood);			// Brave Strongbash
	mgr->register_gossip_script(3218, blood);			// Brave Swiftwind
	mgr->register_gossip_script(3221, blood);			// Brave Rockhorn
	mgr->register_gossip_script(3223, blood);			// Brave Rainchaser
	mgr->register_gossip_script(3212, blood);			// Brave Ironhorn
	mgr->register_gossip_script(5953, razor);			// Razor Hill Grunt
	mgr->register_gossip_script(5725, brill);				// Deathguard Lundmark
	mgr->register_gossip_script(1738, brill);				// Deathguard Terrence
	mgr->register_gossip_script(1652, brill);				// Deathguard Burgess
	mgr->register_gossip_script(1746, brill);				// Deathguard Cyrus
	mgr->register_gossip_script(1745, brill);				// Deathguard Morris
	mgr->register_gossip_script(1743, brill);				// Deathguard Lawrence
	mgr->register_gossip_script(1744, brill);				// Deathguard Mort
	mgr->register_gossip_script(1496, brill);				// Deathguard Dillinger
	mgr->register_gossip_script(1742, brill);				// Deathguard Bartholomew
	mgr->register_gossip_script(5595, irf);			// Ironforge Guard
	mgr->register_gossip_script(727,  khar);			// Ironforge Mountaineer
	mgr->register_gossip_script(16221,falcon);		// Silvermoon Guardian
	mgr->register_gossip_script(18038,azure);		// Azuremyst Peacekeeper
	mgr->register_gossip_script(19687,shattr);         // Shattrath City Guard -by AeThIs
    mgr->register_gossip_script(18568,shattr);      // Shattrath City Guard Aruspice -by AeThIs
    mgr->register_gossip_script(18549,shattr);      // Shattrath City Guard -by AeThIs
}	

	// To Bloodhoof Guards - I don't know if those are all guards with dialog menu,
	// but they were all I could find. Same to Deathguards.

	// To do:
	//	- Add (eventually) missing guards which should use one of those guard menus.
	//	- Check all scripts + add guard text to DB and connect them with correct scripts.
