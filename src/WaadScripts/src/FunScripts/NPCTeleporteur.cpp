// Source LUA: Stoneharry   
// Traduit en cpp (Branruz)
/*
INSERT INTO creature_proto (entry, minlevel, maxlevel, faction, minhealth, maxhealth, mana, scale, npcflags, attacktime, mindamage, maxdamage, rangedattacktime, rangedmindamage, rangedmaxdamage, mountdisplayid, equipmodel1, equipinfo1, equipslot1, equipmodel2, equipinfo2, equipslot2, equipmodel3, equipinfo3, equipslot3, respawntime, armor, resistance1, resistance2, resistance3, resistance4, resistance5, resistance6, combat_reach, bounding_radius, auras, boss, money, invisibility_type, death_state)
VALUES 
(901314, 70, 70, 35,  100000,  100000,  0,  1, 1, 1800, 150, 250, 1800, 500, 650, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36000, 0, 0, 0, 0, 0, 0, 0, 1, 0, "MEMO", 0, 0, 0);;

INSERT INTO creature_names (entry, `name`, Subname, Flags1, type, Family, Rank, unk4, SpellDataID, male_displayid, female_displayid, unknown_float1, unknown_float2, Civilian, Leader, info_str)
VALUES 
(901314,"Teleporteur","Taxi Service",0,7,0,0,0,NULL,22720,22720,1, 1,1,NULL, '');
*/
#define NPC_TELEPORTEUR 901314

#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305) // warning C4305: 'argument' : truncation from 'double' to 'float'
#endif


class SCRIPT_DECL NPC_Teleporteur : public GossipScript
{
    public:
    void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
    void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
    void GossipEnd(Object * pObject, Player* Plr);
    void Destroy()
    {delete this;}
};

void NPC_Teleporteur::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

void SetupNPC_Teleporteur(ScriptMgr * mgr)
{
    GossipScript * NPC_Tp = (GossipScript*) new NPC_Teleporteur();
	if(NPC_Tp != NULL) mgr->register_gossip_script(NPC_TELEPORTEUR, NPC_Tp);
}

void NPC_Teleporteur::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
 GossipMenu *Menu;
 objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);

 Menu->AddItem(0, "Villes de la Horde"     , 1);
 Menu->AddItem(0, "Villes de l'Alliance"   , 2);
 Menu->AddItem(0, "Locations en Azeroth"   , 3);
 Menu->AddItem(0, "Instances en Azeroth"   , 5);
 Menu->AddItem(0, "Locations en Outreterre", 6);
 Menu->AddItem(0, "Instances en Outreterre", 7);
 Menu->AddItem(0, "Shattrath"              , 8);
 Menu->AddItem(0, "Ile de Quel'Danas"      , 46);
 Menu->AddItem(0, "Arène de Gurubashi"     , 45);
 Menu->AddItem(0, "J'ai besoin d'energie"  , 990);
 Menu->AddItem(0, "Soignez moi"            , 980);
 Menu->AddItem(0, "Options de Morphing"    , 97);
 Menu->AddItem(0, "Retirer Mal de Resurrection", 900);

 if(AutoSend) Menu->SendTo(Plr);
}

void NPC_Teleporteur::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{        
    Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
    if(pObject==NULL) return;

	GossipMenu *Menu;
    switch(IntId)
    {
        case 0: // Retour
        {
            GossipHello(pObject, Plr, true);
        }    
        break;

        case 1: // Villes de la Horde
        {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(1, "Orgrimmar"             , 10);
            Menu->AddItem(1, "Fossoyeuse"            , 11);
            Menu->AddItem(1, "Les Pitons du Tonnerre", 12);
            Menu->AddItem(1, "Lune d'Argent"         , 13);
            Menu->AddItem(0, "[Menu]", 75);
            Menu->SendTo(Plr);
        }    
        break;

        case 2: // Villes de l'Alliance
        {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);            
			Menu->AddItem(1, "Hurlevent", 14);
            Menu->AddItem(1, "Forgefer" , 15);
            Menu->AddItem(1, "Darnassus", 16);
            Menu->AddItem(1, "Exodar"   , 17);
            Menu->AddItem(0, "[Menu]", 75);
            Menu->SendTo(Plr);
        }    
        break;

        case 3: // Locations en Azeroth
        {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(0, "Royaumes de l'est", 40);
            Menu->AddItem(0, "Kalimdor"         , 41);
            Menu->AddItem(0, "[Menu]", 75);
            Menu->SendTo(Plr);
        }
        break;

        case 5: // Instances en Azeroth
        {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(1, "Donjon d'Ombrecroc", 19);
            Menu->AddItem(1, "Zul'Gurub"         , 20);
            Menu->AddItem(1, "Monastère écarlate", 21);
            Menu->AddItem(1, "Stratholme"        , 22);
            Menu->AddItem(1, "Scholomance"       , 23);
            Menu->AddItem(1, "Rochenoire"        , 24);
            Menu->AddItem(1, "Repaire d'Onyxia"  , 25);
            Menu->AddItem(1, "Coeur du Magma"    , 26);
            Menu->AddItem(1, "Karazhan"          , 27);
            Menu->AddItem(1, "Naxxramas"         , 28);
            Menu->AddItem(1, "Grottes du Temps"  , 29);
            Menu->AddItem(0, "[Menu]",75);
            Menu->SendTo(Plr);
        }
        break;

        case 6: // Locations en Outreterre
        {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(1, "Péninsule des flammes infernales", 30);
            Menu->AddItem(1, "Marécage de Zangar"              , 31);
            Menu->AddItem(1, "Nagrand"                         , 32);
            Menu->AddItem(1, "Les Tranchantes"                 , 33);
            Menu->AddItem(1, "Raz-de-Néant"                    , 34);
            Menu->AddItem(1, "Forêt de Terokkar"               , 35);
            Menu->AddItem(1, "Vallée d'Ombrelune"              , 36);
            Menu->AddItem(1, "Shattrath"                       , 37);
            Menu->AddItem(0, "[Menu]", 75);
            Menu->SendTo(Plr);
        }
        break;

        case 7: // Instances en Outreterre
        {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(1, "Remparts des Flammes infernales", 430);
            Menu->AddItem(1, "La Fournaise du sang"           , 431);
            Menu->AddItem(1, "Les Salles brisées"             , 441);
            Menu->AddItem(1, "Tombes-mana"                    , 434);
            Menu->AddItem(1, "Les salles des Sethekk"         , 438);
            Menu->AddItem(1, "Cryptes Auchenaï"               , 435);
            Menu->AddItem(1, "Labyrinthe des ombres"          , 440);
            Menu->AddItem(1, "Grottes du Temps"               , 436);
            Menu->AddItem(1, "Terrasse des Magistères"        , 445);
            Menu->AddItem(0, "-->Seconde Page-->"             , 47);
            Menu->AddItem(0, "[Menu]", 75);;
            Menu->SendTo(Plr);
        }    
        break;

        case 47: // Instances en Outreterre (suite)
        {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(1, "Repaire de Gruul"                , 446);
            Menu->AddItem(1, "Le repaire de Magtheridon"       , 447);
            Menu->AddItem(1, "Zul'Aman"                        , 448);
            Menu->AddItem(1, "Caverne du sanctuaire du Serpent", 449);
            Menu->AddItem(1, "L'Oeil"                          , 450);
            Menu->AddItem(1, "Temple Noir"                     , 451);
            Menu->AddItem(1, "Plateau du Puits de soleil"      , 452);
            Menu->AddItem(0, "[Retour]", 7);
            Menu->AddItem(0, "[Menu]", 75);
            Menu->SendTo(Plr);
        }    
        break;

		case 40: // Royaumes de l'est 
		   {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(1, "Montagnes d'Alterac"     , 208);
            Menu->AddItem(1, "Terres ingrates"         , 214);
            Menu->AddItem(1, "Terres foudroyées"       , 222);
            Menu->AddItem(1, "Steppes ardentes"        , 216);
            Menu->AddItem(1, "Défilé de Deuillevent"   , 221);
            Menu->AddItem(1, "Dun Morogh"              , 212);
            Menu->AddItem(1, "Bois de la Pénombre"     , 220);
            Menu->AddItem(1, "Maleterres de l'est"     , 206);
            Menu->AddItem(1, "Forêt d'Elwynn"          , 217);
            Menu->AddItem(1, "Bois des Chants éternels", 201);
            Menu->AddItem(1, "Les Terres fantômes"     , 202);
            Menu->AddItem(0, "-->Seconde Page-->", 43);
            Menu->AddItem(0, "[Retour]", 3);
            Menu->AddItem(0, "[Menu]", 75);
			Menu->SendTo(Plr);
		   }
			break;

		case 43: // Royaumes de l'est (Suite)
		   {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(1, "Les Serres-Rocheuses", 112);
            Menu->AddItem(1, "Tanaris", 121);
            Menu->AddItem(1, "Teldrassil", 100);
            Menu->AddItem(1, "Les Tarides", 114);
            Menu->AddItem(1, "Mille pointes", 118);
            Menu->AddItem(1, "Cratère d'Un'Goro", 119);
            Menu->AddItem(1, "Berceau-de-l'Hiver", 108);
            Menu->AddItem(0, "[Retour]", 3);
            Menu->AddItem(0, "[Menu]", 75);
			Menu->SendTo(Plr);
		   }
		   break;

		case 41: // Kalimdor 
		   {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(1, "Orneval", 109);
            Menu->AddItem(1, "Ile de Brume-azur", 103);
            Menu->AddItem(1, "Ile de Brume-sang", 102);
            Menu->AddItem(1, "Sombrivage", 105);
            Menu->AddItem(1, "Désolace", 113);
            Menu->AddItem(1, "Durotar", 111);
            Menu->AddItem(1, "Marécage d'Âprefange", 117);
            Menu->AddItem(1, "Gangrebois", 107);
            Menu->AddItem(1, "Féralas", 116);
            Menu->AddItem(1, "Reflet-de-Lune", 106);
            Menu->AddItem(1, "Mulgore", 115);
            Menu->AddItem(1, "Silithus", 120);
            Menu->AddItem(0, "-->Seconde Page-->", 42);
            Menu->AddItem(0, "[Retour]", 3);
            Menu->AddItem(0, "[Menu]", 75);
			Menu->SendTo(Plr);
		   }
		   break;

		case 42: // Kalimdor  (Suite)
		   { 
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(1, "Contreforts de Hautebrande", 210);
            Menu->AddItem(1, "Loch Modan"                , 213);
            Menu->AddItem(1, "Les Carmines"              , 218);
            Menu->AddItem(1, "Gorge des Vents brûlants"  , 215);
            Menu->AddItem(1, "Forêt des Pins argentés"   , 207);
            Menu->AddItem(1, "Vallée de Strangleronce"   , 223);
            Menu->AddItem(1, "Marais des Chagrins"       , 219);
            Menu->AddItem(1, "Les Hinterlands"           , 209);
            Menu->AddItem(1, "Clairières de Tirisfal"    , 205);
            Menu->AddItem(1, "Maleterres de l'ouest"     , 203);
            Menu->AddItem(1, "Les Paluns"                , 211);
            Menu->AddItem(0, "[Retour]", 3);
            Menu->AddItem(0, "[Menu]", 75);
 			Menu->SendTo(Plr);
		   } break;

		case 48: // Teleportation 
		   {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(1, "Le Méchanar", 442);
            Menu->AddItem(1, "La Botanica", 443);
            Menu->AddItem(1, "L'Arcatraz", 444);
            Menu->AddItem(1, "Le Caveau de la vapeur", 439);
            Menu->AddItem(1, "Les enclos aux esclaves", 432);
            Menu->AddItem(1, "La Basse-tourbière", 433);
            Menu->AddItem(0, "[Retour]", 7);
            Menu->AddItem(0, "[Menu]", 75);
 			Menu->SendTo(Plr);
		   } break;

        case 8:   // Shattrath 
        {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(1, "La grande arène", 38);
            Menu->AddItem(0, "[Menu]", 75);
            Menu->SendTo(Plr);
        }    
        break;

		case 97: // Options de Morphing 
		   {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(0, "Arlokk", 500);
            Menu->AddItem(0, "Homme Habillé", 501);
            Menu->AddItem(0, "Poussin démoniaque", 502);
            Menu->AddItem(0, "Fantôme", 503);
            Menu->AddItem(0, "Élémentaire d'eau", 504);
            Menu->AddItem(0, "Ninja", 505);
            Menu->AddItem(0, "Gnome Lepreux", 506);
            Menu->AddItem(0, "Costume de Murloc", 507);
            Menu->AddItem(0, "[Menu]", 75);
			Menu->SendTo(Plr);
		   } break;
        
        case 10: // Teleportation Orgrimmar
			Plr->Gossip_Complete(); 
            Plr->SafeTeleport(1, 0,LocationVector( 1484, -4417, 25));
            break;
		
        case 11: // Teleportation Fossoyeuse
            Plr->SafeTeleport(0, 0,LocationVector( 1831, 238, 60));
            Plr->Gossip_Complete();    
            break;

		case 12: // Teleportation Les Pitons du Tonnerre
          Plr->SafeTeleport(1, 0,LocationVector( -1277, 118, 131));
          Plr->Gossip_Complete();    
          break;

		case 13: // Teleportation Lune d'Argent
          Plr->SafeTeleport(530, 0,LocationVector( 9413, -7277, 14));
          Plr->Gossip_Complete();    
           break;

		case 14: // Teleportation Hurlevent
          Plr->SafeTeleport(0, 0,LocationVector( -8913.23, 554.633, 94.7944));
          Plr->Gossip_Complete();    
         break;

		case 15: // Teleportation Forgefer
          Plr->SafeTeleport(0, 0,LocationVector( -4981.25, -881.542, 502.66));
          Plr->Gossip_Complete();    
 
        break;

		case 16: // Teleportation Darnassus
          Plr->SafeTeleport(1, 0,LocationVector( 9948, 2413, 1327));
          Plr->Gossip_Complete();    
        break;

		case 17: // Teleportation Exodar
          Plr->SafeTeleport(530, 0,LocationVector( -4014.080078, -11895.799805, -1.990842));
          Plr->Gossip_Complete();    
        break;

		case 19: // Teleportation Donjon d'Ombrecroc
         Plr->SafeTeleport(0, 0,LocationVector( -234.495087, 1561.946411, 76.892143));
          Plr->Gossip_Complete();    
        break;

		case 20: // Teleportation Zul'Gurub
          Plr->SafeTeleport(0, 0,LocationVector( -11919.073242, -1202.459374, 92.298744));
          Plr->Gossip_Complete();    
        break;

		case 21: // Teleportation "Monastère écarlate
          Plr->SafeTeleport(0, 0,LocationVector( 2870.442627, -819.985229, 160.331085));
          Plr->Gossip_Complete();    

		case 22: // Teleportation Stratholme
          Plr->SafeTeleport(0, 0,LocationVector( 3359.111572, -3380.8444238, 144.781860));
          Plr->Gossip_Complete();    
        break;

		case 23: // Teleportation Scholomance
          Plr->SafeTeleport(0, 0,LocationVector( 1267.468628, -2556.651367, 94.127983));
          Plr->Gossip_Complete();    
        break;

		case 24: // Teleportation Rochenoire
          Plr->SafeTeleport(0, 0,LocationVector( -7527.129883, -1224.997437, 285.733002));
          Plr->Gossip_Complete();    
        break;

		case 25: // Teleportation Repaire d'Onyxia
          Plr->SafeTeleport(1, 0,LocationVector( -4708.491699, -3727.672363, 54.535076));
          Plr->Gossip_Complete();    
        break;

		case 26: // Teleportation Coeur du Magma
          Plr->SafeTeleport(0, 0,LocationVector( -7515.409668, -1045.369629, 182.301208));
          Plr->Gossip_Complete();    
        break;

		case 27: // Teleportation Karazhan
          Plr->SafeTeleport(0, 0,LocationVector( -11122.913086, -2014.498779, 47.079350));
          Plr->Gossip_Complete();    
        break;

		case 28: // Teleportation Naxxramas
          Plr->SafeTeleport(0, 0,LocationVector( 3132.915283, -3731.012939, 138.658371));
          Plr->Gossip_Complete();    
        break;

		case 29: // Teleportation Grottes du Temps
          Plr->SafeTeleport(1, 0,LocationVector( -8519.718750, -4297.542480, -208.441376));
          Plr->Gossip_Complete();    
        break;

		case 31: // Teleportation Marécage de Zangar
          Plr->SafeTeleport(530, 0,LocationVector( -248, 922, 84));
          Plr->Gossip_Complete();    
        break;
        case 32: // Teleportation Nagrand
          Plr->SafeTeleport(530, 0,LocationVector( -468, 8418, 28));
          Plr->Gossip_Complete();    
        break;

		case 33: // Teleportation Les Tranchantes
          Plr->SafeTeleport(530, 0,LocationVector( 1471, 6831, 107));
          Plr->Gossip_Complete();    
        break;

		case 34: // Teleportation Raz-de-Néant
          Plr->SafeTeleport(530, 0,LocationVector( 3396, 4185, 137));
          Plr->Gossip_Complete();    
        break;

		case 35: // Teleportation Forêt de Terokkar
          Plr->SafeTeleport(530, 0,LocationVector( -2276.82, 5132.03, -8.62994));
          Plr->Gossip_Complete();    
        break;

		case 36: // Teleportation Vallée d'Ombrelune
          Plr->SafeTeleport(530, 0,LocationVector( -1202, 5316, 33));
          Plr->Gossip_Complete();    
        break;

		case 37: // Teleportation Shattrath
          Plr->SafeTeleport(530, 0,LocationVector( -2859, 3182, 10));
          Plr->Gossip_Complete();    
        break;

		case 38: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( -1887, 5359, -12));
          Plr->Gossip_Complete();    
        break;

		case 100: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( 9874.646484, 595.742432, 1303.874023));
          Plr->Gossip_Complete();    
        break;

		case 102: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( -2721.68, -12208.9, 10.0882));
          Plr->Gossip_Complete();    
        break;

		case 45: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -13243.240234, 197.949799, 32.112690));
          Plr->Gossip_Complete();    
        break;

		case 103: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( -4020.48, -13783.3, 74.9001));
          Plr->Gossip_Complete();    
        break;

		case 105: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( 6207.5, -152.833, 80.8185));
          Plr->Gossip_Complete();    
        break;

		case 106: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( 7101.68, -2670.2, 512.2));
          Plr->Gossip_Complete();    
        break;

		case 107: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( 5483.9, -749.881, 335.621));
          Plr->Gossip_Complete();    
        break;

		case 108: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( 6107.62, -4181.6, 853.322));
          Plr->Gossip_Complete();    
        break;

		case 109: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( 2717.1, 5967.91, 107.4)); // (1, 3469.43, 847.62, 6.36476);
          Plr->Gossip_Complete();    
        break;

		case 111: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( 341.42, -4684.7, 31.9493));
          Plr->Gossip_Complete();    
        break;

		case 112: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( 1145.85, 664.812, 143));
          Plr->Gossip_Complete();    
        break;

		case 113: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( -93.1614, 1691.15, 90.0649));
          Plr->Gossip_Complete();    
        break;

		case 114: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( -90.19003, -1943.44, -180.4727));
          Plr->Gossip_Complete();    
        break;

		case 115: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( -1840.75, 5359, -7.845));
          Plr->Gossip_Complete();    
        break;

		case 116: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( -4458.93, 243.415, -65.6136));
          Plr->Gossip_Complete();    
        break;

		case 117: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( -3463.26, -4123.13, 18.1043));
          Plr->Gossip_Complete();    
        break;

		case 118: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( -4932.53, -1596.05, 85.8157));
          Plr->Gossip_Complete();    
        break;

		case 119: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( -7932.61, -2139.61, -229.728));
          Plr->Gossip_Complete();    
        break;

		case 120: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( -7373.69, -2950.2, -11.7598));
          Plr->Gossip_Complete();    
        break;

		case 121: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( -7373.69, -2950.2, -30.7598));
          Plr->Gossip_Complete();    
        break;

		case 201: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( 9449.15, -6782.61, 16.6167));
          Plr->Gossip_Complete();    
        break;

		case 202: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( 7880, -6193, 22));
          Plr->Gossip_Complete();    
        break;

		case 203: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( 1224.36, -1151.97, 61.7327));
          Plr->Gossip_Complete();    
        break;

		case 205: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( 2019.35, 1904.36, 106.144));
          Plr->Gossip_Complete();    
        break;

		case 206: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( 1919.44, -4306.23, 77.838));
          Plr->Gossip_Complete();    
        break;

		case 207: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( 511.536, 1638.63, 121.417));
          Plr->Gossip_Complete();    
        break;

		case 208: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( 272.704, -654.514, 129.609));
          Plr->Gossip_Complete();    
        break;

		case 209: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( 139.375, -1982.79, 134.043));
          Plr->Gossip_Complete();    
        break;

		case 210: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -852.854, -576.712, 21.0293));
          Plr->Gossip_Complete();    
        break;

		case 211: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -4086.36, -2610.95, 47.0143));
          Plr->Gossip_Complete();    
        break;

		case 212: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -5425.924316, -224.271957, 404.984344));
          Plr->Gossip_Complete();    
        break;

		case 213: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -4939.1, -3423.74, 306.595));
          Plr->Gossip_Complete();    
        break;

		case 214: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -6018.138184, -3311.517822, 261.744324));
          Plr->Gossip_Complete();    
        break;

		case 215: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -7176.63, -937.667, 171.206));
          Plr->Gossip_Complete();    
        break;

		case 216: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -7907.41, -1128.66, 192.056));
          Plr->Gossip_Complete();    
        break;

		case 217: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -9621.383789, -371.068207, 57.471478));
          Plr->Gossip_Complete();    
        break;

		case 218: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -9219.37, -2149.94, 71.606));
          Plr->Gossip_Complete();    
        break;

		case 219: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -10264.6, -3059.9, 19.9356));
          Plr->Gossip_Complete();                             
        break;

		case 220: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -11224.254883, -378.471802, 52.764240));
          Plr->Gossip_Complete();    
        break;

		case 221: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -10435.4, -1809.28, 101));
          Plr->Gossip_Complete();    
        break;

		case 222: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -11204.5, -2730.61, 15.8972));
          Plr->Gossip_Complete();    
        break;

		case 223: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -11634.8, -54.0697, 14.4439));
          Plr->Gossip_Complete();    
        break;

		case 430: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( -360.671, 3071.9, -15.0977));
          Plr->Gossip_Complete();    
        break;

		case 431: // Teleportation 
          Plr->SafeTeleport(542,0, LocationVector(-3.9967, 14.6363, -44.8009));
          Plr->Gossip_Complete();    
        break;

		case 432: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( 721.926, 7012.24, -73.065));
          Plr->Gossip_Complete();    
        break;

		case 433: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( 779.802, 6769.33, -71.4282));
          Plr->Gossip_Complete();    
        break;

		case 434: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( -3101.47, 4947.11, -101.177));
          Plr->Gossip_Complete();    
        break;

		case 435: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( -3357.32, 5216.77, -101.049));
          Plr->Gossip_Complete();    
        break;

		case 436: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( -8195.94, -4500.13, 9.60819));
          Plr->Gossip_Complete();    
        break;

		case 438: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( -3360.13, 4667.85, -101.047));
          Plr->Gossip_Complete();    
        break;

		case 439: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector( -11634.8, -54.0697, 14.4439));
          Plr->Gossip_Complete();    
        break;

		case 440: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( -3635.76, 4931.82, -100.034));
          Plr->Gossip_Complete();    
        break;

		case 441: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( -309.83, 3080.08, -3.63538));
          Plr->Gossip_Complete();    
        break;

		case 442: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( 2885.2, 1564.73, 248.874));
          Plr->Gossip_Complete();    
        break;

		case 443: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( 3405.48, 1489.14, 183.838));
          Plr->Gossip_Complete();    
        break;

		case 444: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( 2872, 1555.29, 253.159));
          Plr->Gossip_Complete();    
        break;

		case 445: // Teleportation 
          Plr->SafeTeleport(585,0,LocationVector( 2.19347, -0.123698, -2.8025));
          Plr->Gossip_Complete();    
        break;

		case 446: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( 3606.85, 5260.49, 4.1724));
          Plr->Gossip_Complete();    
        break;

		case 447: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( -319.635, 3102.03, -113.937));
          Plr->Gossip_Complete();    
        break;

		case 448: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( 6850, -7950, 170));
          Plr->Gossip_Complete();    
        break;

		case 449: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( 742.883, 6867.19, -68.8289));
          Plr->Gossip_Complete();    
        break;

		case 450: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( 3087.22, 1380.7, 184.883));
          Plr->Gossip_Complete();    
        break;

		case 451: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( -3604.74, 328.252, 38.3077));
          Plr->Gossip_Complete();    
        break;

		case 452: // Teleportation 
          Plr->SafeTeleport(580,0, LocationVector(1791.17, 926.31, 15.1135));
          Plr->Gossip_Complete();    
        break;

		case 46: // Teleportation 
          Plr->SafeTeleport(530, 0,LocationVector( 12956.072266, -6943.814453, 9.968110));
          Plr->Gossip_Complete();    
        break;

		case 990: // Teleportation 
          Plr->SafeTeleport(1, 0,LocationVector( 2127.671631, -4733.104492, 50.276615));
          Plr->Gossip_Complete();    
        break;

		case 991: // Teleportation 
          Plr->SafeTeleport(0, 0,LocationVector(  -8443.413086, 335.691833, 122.163055));
		Plr->Gossip_Complete();
        break;		

       case 75: //Retour au Depart
        {
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
            Menu->AddItem(0, "Villes de la Horde"     , 1);
            Menu->AddItem(0, "Villes de l'Alliance"   , 2);
            Menu->AddItem(0, "Locations en Azeroth"   , 3);
            Menu->AddItem(0, "Instances en Azeroth"   , 5);
            Menu->AddItem(0, "Locations en Outreterre", 6);
            Menu->AddItem(0, "Instances en Outreterre", 7);
            Menu->AddItem(0, "Shattrath"              , 8);
            Menu->AddItem(0, "Ile de Quel'Danas"      , 46);
            Menu->AddItem(0, "Arène de Gurubashi"     , 45);
            Menu->AddItem(0, "J'ai besoin d'energie"  , 990);
            Menu->AddItem(0, "Soignez moi"            , 980);
            Menu->AddItem(0, "Options de Morph"       , 97);
            Menu->AddItem(0, "Retirer Mal de Resurrection", 900);
            Menu->SendTo(Plr);
        }
        break;
    }
};

/*


		case 99: // Teleportation 
unit:FullCastSpellOnTarget(33077, player)
unit:FullCastSpellOnTarget(33078, player)
unit:FullCastSpellOnTarget(33079, player)
unit:FullCastSpellOnTarget(33080, player)
unit:FullCastSpellOnTarget(33081, player)
unit:FullCastSpellOnTarget(33082, player)
            Menu->AddItem(0, "[Menu]", 75);
unit:GossipS          Plr->Gossip_Complete();    Menu(player)
          Plr->Gossip_Complete();    

		case 98: // Teleportation 
unit:FullCastSpellOnTarget(26565, player)
            Menu->AddItem(0, "[Menu]", 75);
unit:GossipS          Plr->Gossip_Complete();    Menu(player)
          Plr->Gossip_Complete();    

		case 900: // Teleportation 
unit:GossipCreateMenu(62, player);
Plr->LearnSpell(15007)
Plr->UnlearnSpell(15007)
            Menu->AddItem(0, "[Menu]", 75);
unit:GossipS          Plr->Gossip_Complete();    Menu(player)
          Plr->Gossip_Complete();    



		case 500: // Teleportation 
unit:GossipCreateMenu(500, player);
unit:FullCastSpellOnTarget(24190, player)
            Menu->AddItem(0, "[Retour]", 97);
            Menu->AddItem(0, "[Menu]", 75);
unit:GossipS          Plr->Gossip_Complete();    Menu(player)
          Plr->Gossip_Complete();    

		case 501: // Teleportation 
unit:GossipCreateMenu(501, player);
unit:FullCastSpellOnTarget(32616, player)
            Menu->AddItem(0, "[Retour]", 97);
            Menu->AddItem(0, "[Menu]", 75);
unit:GossipS          Plr->Gossip_Complete();    Menu(player)
          Plr->Gossip_Complete();    

		case 502: // Teleportation 
unit:GossipCreateMenu(502, player);
unit:FullCastSpellOnTarget(29489, player)
            Menu->AddItem(0, "[Retour]", 97);
            Menu->AddItem(0, "[Menu]", 75);
unit:GossipS          Plr->Gossip_Complete();    Menu(player)
          Plr->Gossip_Complete();    

		case 503: // Teleportation 
unit:GossipCreateMenu(503, player);
unit:FullCastSpellOnTarget(40155, player)
            Menu->AddItem(0, "[Retour]", 97);
            Menu->AddItem(0, "[Menu]", 75);
unit:GossipS          Plr->Gossip_Complete();    Menu(player)
          Plr->Gossip_Complete();    

		case 504: // Teleportation 
unit:GossipCreateMenu(504, player);
unit:FullCastSpellOnTarget(39354, player)
            Menu->AddItem(0, "[Retour]", 97);
            Menu->AddItem(0, "[Menu]", 75);
unit:GossipS          Plr->Gossip_Complete();    Menu(player)
          Plr->Gossip_Complete();    

		case 505: // Teleportation 
unit:GossipCreateMenu(505, player);
unit:FullCastSpellOnTarget(24718, player)
            Menu->AddItem(0, "[Retour]", 97);
            Menu->AddItem(0, "[Menu]", 75);
unit:GossipS          Plr->Gossip_Complete();    Menu(player)
          Plr->Gossip_Complete();    

		case 506: // Teleportation 
unit:GossipCreateMenu(506, player);
unit:FullCastSpellOnTarget(24719, player)
            Menu->AddItem(0, "[Retour]", 97);
            Menu->AddItem(0, "[Menu]", 75);
unit:GossipS          Plr->Gossip_Complete();    Menu(player)
          Plr->Gossip_Complete();    

		case 507: // Teleportation 
unit:GossipCreateMenu(507, player);
unit:FullCastSpellOnTarget(42365, player)
            Menu->AddItem(0, "[Retour]", 97);
            Menu->AddItem(0, "[Menu]", 75);
unit:GossipS          Plr->Gossip_Complete();    Menu(player)
          Plr->Gossip_Complete();    

*/