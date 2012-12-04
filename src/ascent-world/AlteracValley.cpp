/*
 * AscentWaad, AlteracValley Coding ONLY for Open Source Ascent Server
 * Copyright (C) 2008 WaadTeam <http://arbonne.games-rpg.net/>
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
 * MERCHANTAVILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
/*
 * J'inclus le systeme de def du drap ke j'avais codé pour Arathi! c'est beaucoup plus proche de la réalité!
 * Merci a tout ceux ki participeront à completer ce code!! (Merci de ne pas changer l'entete svp!)
 * - WaadTeam - (Branruz) -
 */

/* Go Ids (ce ne sont pas exactement les memes que sur l'offi, les flags des Cim ont un socle en pierre)
 178364 Flag Horde sur poteau avec brasier (declencheur sur HookSlowLockOpen (prise du drap))
 179287 Flag Horde contesté sur poteau avec brasier (declencheur sur HookSlowLockOpen (prise du drap))
 178365 Flag Alli Gros poteau en pierre (declencheur sur HookSlowLockOpen (prise du drap))
 179286 Flag contesté Alli Gros poteau en pierre (declencheur sur HookSlowLockOpen (prise du drap))

 178925 Flag Alli Poteau en bois avec socle pierre (declencheur sur HookSlowLockOpen (prise du drap))
 178940 Flag contesté Alli Poteau en bois avec socle pierre (declencheur sur HookSlowLockOpen (prise du drap))
 178943 Flag Horde sur poteau simple (declencheur sur HookSlowLockOpen (prise du drap))
 179435 Flag Horde contesté sur poteau simple (declencheur sur HookSlowLockOpen (prise du drap))

 180418 Flag Neutre (sans faction, declencheur sur HookSlowLockOpen (prise du drap))

 180100 Aura (bleu) verticale Alliance ( a mettre sur les points de spawns des flags )
 180101 Aura (rouge) verticale H2 ( a mettre sur les points de spawns des flags )
 180102 Aura (blanche) verticale neutre ( a mettre sur les points de spawns des flags )

 180421 (Enorme) Aura (bleu) verticale Alliance ( a mettre sur les points de spawns des tours )
 180422 (Enorme) Aura (rouge) verticale Horde ( a mettre sur les points de spawns des tours ) 
 180423 (Enorme) Aura (blanche) verticale Neutre ( a mettre sur les points de spawns des tours ) 

 180424 Porte du BG (declencheur sur HookSlowLockOpen qui fait remonter la porte toute seule)
*/

/* Pour Memo: Event ajouter sont dans l'enum EventType de l'EventMgr.h

/* Pour Memo: sound
#define SOUND_BATTLEGROUND_BEGIN			0xD6F
#define SOUND_HORDE_SCORES					8213
#define SOUND_ALLIANCE_SCORES				8173
#define SOUND_ALLIANCE_CAPTURE				8174
#define SOUND_HORDE_CAPTURE					8212
#define SOUND_ALLIANCE_RETURNED				8192
#define SOUND_HORDE_RETURNED				8192	// huh?
#define SOUND_HORDEWINS						8454
#define SOUND_ALLIANCEWINS					8455
#define SOUND_HORDE_BGALMOSTEND			 0x2108
#define SOUND_ALLIANCE_BGALMOSTEND		  0x2109
*/
// Marque d'honneur Alterac: 29024
// Flag de la caverne de l'aile de glace 179024


#include "StdAfx.h"

class Creature;

#define BASE_RENFORTS 600
#define RENFORTS_WARNING_THRESHOLD 200
#define RENFORTS_WINVAL 0
#define BASE_BH_GAIN 14
#define TIMER_PRISE_FLAG 300000 // 5min	

#define AV_TEAM_ALLY	0
#define AV_TEAM_H2		1

// Worldstates yeahhh!!!
#define WS_AV_ALLY_SCR		3127
#define WS_AV_H2_SCR		3128
#define WS_AV_ALLY_SCR_DSP  3133
#define WS_AV_HORDE_SCR_DSP	3134


// Les mines
#define WS_AV_MIRONDEEP_ALLY_CNTR	1358
#define WS_AV_MIRONDEEP_H2_CNTR		1359
#define WS_AV_MIRONDEEP_TROGG_CNTR	1360

#define WS_AV_MFROIDEDENT_ALLY_CONTROL	 1355
#define WS_AV_MFROIDEDENT_H2_CONTROL	 1356
#define WS_AV_MFROIDEDENT_KOBOLD_CONTROL 1357

// Cimetieres
#define WS_AV_PSEC_STORM_ALLY_CNTR	1325
#define WS_AV_PSEC_STORM_H2_CNTR	1327
#define WS_AV_PSEC_STORM_ALLY_ASSLT 1326
#define WS_AV_PSEC_STORM_H2_ASSLT	1328

#define WS_AV_CSTORM_ALLI_CONTROL	1333
#define WS_AV_CSTORM_H2_CONTROL		1334
#define WS_AV_CSTORM_ALLI_ASSAULT	1335
#define WS_AV_CSTORM_H2_ASSAULT		1336

#define WS_AV_CSHEARTH_ALLY_CONTR	1302
#define WS_AV_CSHEARTH_H2_CONTR		1301
#define WS_AV_CSHEARTH_ALLY_ASSAULT	1304
#define WS_AV_CSHEARTH_H2_ASSAULT	1303

#define WS_AV_CNEIGES_TROLL_CONTROL	1966
#define WS_AV_CNEIGES_ALLY_CONTROL	1341
#define WS_AV_CNEIGES_H2_CONTROL	1342
#define WS_AV_CNEIGES_ALLY_ASSAULT	1343
#define WS_AV_CNEIGES_H2_ASSAULT	1344

#define WS_AV_CGLACESANG_ALLY_CONTROL	1346
#define WS_AV_CGLACESANG_H2_CONTROL		1347
#define WS_AV_CGLACESANG_ALLY_ASSAULT	1348
#define WS_AV_CGLACESANG_H2_ASSAULT		1349

#define WS_AV_CFWOLF_ALLY_CONTROL	1337
#define WS_AV_CFWOLF_H2_CONTROL		1338
#define WS_AV_CFWOLF_ALLY_ASSAULT	1339
#define WS_AV_CFWOLF_H2_ASSAULT		1340

#define WS_AV_HFROSTWOLF_ALLY_CONTROL	1329
#define WS_AV_HFROSTWOLF_H2_CONTROL		1330
#define WS_AV_HFROSTWOLF_ALLY_ASSAULT	1331
#define WS_AV_HFROSTWOLF_H2_ASSAULT	1332

// Tours et donjon
#define WS_AV_FNORD_DUN_CONTR	1361
#define WS_AV_FNORD_DUN_ASSAULT	1378
#define WS_AV_FNORD_DUN_DESTR	1370

#define WS_AV_FSUD_DUN_CONTR	1362
#define WS_AV_FSUD_DUN_ASSAULT	1379
#define WS_AV_FSUD_DUN_DESTR	1371

#define WS_AV_FAILE_DE_GLACE_CONTROL 1363
#define WS_AV_FAILE_DE_GLACE_ASSAULT 1380
#define WS_AV_FAILE_DE_GLACE_DESTR	 1372

#define WS_AV_FSHEARTH_CONTR	1364
#define WS_AV_FSHEARTH_ASSAULT	1381
#define WS_AV_FSHEARTH_DESTR	1373

#define WS_AV_THALTE_CONTR		1384
#define WS_AV_THALTE_ASSAULT	1389
#define WS_AV_THALTE_DESTR		1367

#define WS_AV_TGLACESANG_CONTR			1385
#define WS_AV_TGLACESANG_DESTR			1368
#define WS_AV_TGLACESANG_ASSAULT		1390

#define WS_AV_TORIENT_FROSTWOLF_CONTROL		1383
#define WS_AV_TORIENT_FROSTWOLF_DESTROY		1366
#define WS_AV_TORIENT_FROSTWOLF_ASSAULT		1388

#define WS_AV_TOCCID_FROSTWOLF_CONTROL		1382
#define WS_AV_TOCCID_FROSTWOLF_DESTROY		1365
#define WS_AV_TOCCID_FROSTWOLF_ASSAULT		1387

static uint32 AVWorldStateMines[2][AV_NUM_SPAWN_TYPES] = 
 //  NEUTRAL , ALLI-ATTACK , H2-ATTACK , ALLI-CONTROLLED , H2_CONTROLLED
{
 { WS_AV_MIRONDEEP_TROGG_CNTR,0, 0,WS_AV_MIRONDEEP_ALLY_CNTR, WS_AV_MIRONDEEP_H2_CNTR }, // Mine FroideDent
 { WS_AV_MFROIDEDENT_KOBOLD_CONTROL,0, 0,WS_AV_MFROIDEDENT_ALLY_CONTROL, WS_AV_MFROIDEDENT_H2_CONTROL }, // Mine FroideDent
};

 static uint32 AVWorldStates[AV_NUM_CONTROL_POINTS][AV_NUM_SPAWN_TYPES] = 
 {
  //  NEUTRAL , ALLI-ATTACK , H2-ATTACK , ALLI-CONTROLLED , H2_CONTROLLED
  // Alliance
  {0, 0, 0, 0, 0 },	// 0 Dun Baldar
  { WS_AV_PSEC_STORM_ALLY_ASSLT,WS_AV_PSEC_STORM_H2_ASSLT,WS_AV_PSEC_STORM_ALLY_CNTR,WS_AV_PSEC_STORM_H2_CNTR },	// 1 Poste de Secours Stormpike		
  {0,0,WS_AV_FSUD_DUN_ASSAULT,WS_AV_FSUD_DUN_CONTR,WS_AV_FSUD_DUN_DESTR },	// 2 Fortin Nord de DunBaldar 	
  {0,0,WS_AV_FSUD_DUN_ASSAULT,WS_AV_FSUD_DUN_CONTR,WS_AV_FSUD_DUN_DESTR  },	// 3 Fortin Sud de DunBaldar     
  {0, WS_AV_CSTORM_ALLI_ASSAULT,WS_AV_CSTORM_H2_ASSAULT,WS_AV_CSTORM_ALLI_CONTROL,WS_AV_CSTORM_H2_CONTROL}, // 4 Flag Cimetiere Stormpike 	    
  {0,0,WS_AV_FAILE_DE_GLACE_ASSAULT,WS_AV_FAILE_DE_GLACE_CONTROL,WS_AV_FAILE_DE_GLACE_DESTR},	// 5 Fortin de l'Aile de Glace   
  {0,WS_AV_CSHEARTH_ALLY_ASSAULT,WS_AV_CSHEARTH_H2_ASSAULT,WS_AV_CSHEARTH_ALLY_CONTR,WS_AV_CSHEARTH_H2_CONTR}, // 6 Flag Cimetiere StoneHearth  	    
  { 0,0,0,0,0 },	// 7 Avant poste de StoneHearth 	  
  { 0,0,WS_AV_FSHEARTH_ASSAULT,WS_AV_FSHEARTH_CONTR,WS_AV_FSHEARTH_DESTR },	// 8 Fortin de StoneHearth 	   
  // Hache-Hiver (les go ne sont pas les bons, je met ceux la en attendant )
  {WS_AV_CNEIGES_TROLL_CONTROL,WS_AV_CNEIGES_ALLY_ASSAULT,WS_AV_CNEIGES_H2_ASSAULT,WS_AV_CNEIGES_ALLY_CONTROL,WS_AV_CNEIGES_H2_CONTROL},	// 9 Flag du cimetiere des neiges 	   
  //Horde
  { 0,0,0,0,0 },	// 10 Garnison GlaceSang	
  { 0,WS_AV_TGLACESANG_ASSAULT,0,WS_AV_TGLACESANG_DESTR,WS_AV_TGLACESANG_CONTR},	// 11 Tour GlaceSang  	   
  { 0,WS_AV_CGLACESANG_ALLY_ASSAULT,WS_AV_CGLACESANG_H2_ASSAULT,WS_AV_CGLACESANG_ALLY_CONTROL,WS_AV_CGLACESANG_H2_CONTROL},	// 12 Flag Cimetiere GlaceSang 	   
  { 0,WS_AV_THALTE_ASSAULT, 0, WS_AV_THALTE_DESTR, WS_AV_THALTE_CONTR },	// 13 Tour de la Halte 	 
  { 0,WS_AV_CFWOLF_ALLY_ASSAULT, WS_AV_CFWOLF_H2_ASSAULT, WS_AV_CFWOLF_ALLY_CONTROL, WS_AV_CFWOLF_H2_CONTROL },	// 14 Flag Cimetiere FrostWolf	
  { 0,WS_AV_TORIENT_FROSTWOLF_ASSAULT,0,WS_AV_TORIENT_FROSTWOLF_DESTROY,WS_AV_TORIENT_FROSTWOLF_CONTROL },	// 15 Tour Orientale FrostWolf    	   
  { 0,WS_AV_TOCCID_FROSTWOLF_ASSAULT ,0,WS_AV_TOCCID_FROSTWOLF_DESTROY ,WS_AV_TOCCID_FROSTWOLF_CONTROL },	// 16 Tour Occidentale FrostWolf 	
  { 0,0,0,0,0 },	// 17 DONJON_FROSTWOLF 	 (en haut de la tour)  
  { 0,   WS_AV_HFROSTWOLF_ALLY_ASSAULT, WS_AV_HFROSTWOLF_H2_ASSAULT, WS_AV_HFROSTWOLF_ALLY_CONTROL, WS_AV_HFROSTWOLF_H2_CONTROL }, 	// 18 Flag cimetiere Hutte FrostWolf 
 };

// Ref pnj: http://fr.wowhead.com/?zone=2597#npcs:0+1-2
#define AV_NBGARDES_DRAP		4
static uint32 AVGuardsFlags[2][AV_NUM_CONTROL_POINTS][AV_NBGARDES_DRAP] = 
{
   { // Gardes de l'Alliance
	{     0,      0,     0,     0 },	// 0 Dun Baldar
	{ 13080,  13552, 13553, 13552 },	// 1 Poste de Secours Stormpike	(Defenseurs)	
	{ 13358,  13358, 13358, 13358 },	// 2 Fortin Nord de DunBaldar 	(Archers)
	{ 13358,  13358, 13358, 13358 },	// 3 Fortin Sud de DunBaldar    (Archers) 
	{ 13080,  13552, 13553, 13552 },    // 4 Flag Cimetiere Stormpike 	(Defenseurs)	    
	{ 13358,  13358, 13358, 13358 },	// 5 Fortin de l'Aile de Glace  (Archers)
	{ 13080,  13552, 13553, 13552 },    // 6 Flag Cimetiere StoneHearth (Defenseurs)	   
	{ 13358,  13358, 13358, 13358 },	// 7 Avant poste de StoneHearth (Archers)	  
	{     0,      0,     0,     0 },	// 8 Fortin de StoneHearth 	       
	{ 13080,  13552, 13553, 13552 },	// 9 Flag du cimetiere des neiges (Defenseurs)		   
	{     0,      0,     0,     0 },	// 10 Garnison GlaceSang	   
	{     0,      0,     0,     0 },	// 11 Tour GlaceSang  	        
	{ 13080,  13552, 13553, 13552 },	// 12 Flag Cimetiere GlaceSang 	(Defenseurs)	   
	{     0,      0,     0,     0 },	// 13 Tour de la Halte 	        
	{ 13080,  13552, 13553, 13552 },	// 14 Flag Cimetiere FrostWolf	(Defenseurs)		    
	{     0,      0,     0,     0 },	// 15 Tour Orientale FrostWolf   
	{     0,      0,     0,     0 },	// 16 Tour Occidentale FrostWolf   
	{     0,      0,     0,     0 },	// 17 DONJON_FROSTWOLF 	 
	{ 13080,  13552, 13553, 13552 }, 	// 18 Flag cimetiere Hutte FrostWolf (Defenseurs)	
	},

    // Gardes de la Horde (Loup de givre)
   {
	{     0,      0,     0,     0 },	// 0 Dun Baldar
	{ 12053,  13332, 13328, 13332 },	// 1 Poste de Secours Stormpike		(Defenseurs)	
	{     0,      0,     0,     0 },	// 2 Fortin Nord de DunBaldar 	    
	{     0,      0,     0,     0 },	// 3 Fortin Sud de DunBaldar        
	{ 12053,  13332, 13328, 13332 },  // 4 Flag Cimetiere Stormpike 	    (Defenseurs)	
	{     0,      0,     0,     0 },	// 5 Fortin de l'Aile de Glace      
	{ 12053,  13332, 13328, 13332 },  // 6 Flag Cimetiere StoneHearth  	    (Defenseurs)	
	{     0,      0,     0,     0 },	// 7 Avant poste de StoneHearth 	
	{     0,      0,     0,     0 },	// 8 Fortin de StoneHearth 	        
	{ 12053,  13332, 13328, 13332 },	// 9 Flag du cimetiere des neiges 	(Defenseurs)	   
	{     0,      0,     0,     0 },	// 10 Garnison GlaceSang	   
	{ 13359,  13359, 13359, 13359 },	// 11 Tour GlaceSang  	           (Archers)   
	{ 12053,  13332, 13328, 13332 },	// 12 Flag Cimetiere GlaceSang 	   (Defenseurs)	
	{ 13359,  13359, 13359, 13359 },	// 13 Tour de la Halte 	            (Archers)
	{ 12053,  13332, 13328, 13332 },	// 14 Flag Cimetiere FrostWolf		(Defenseurs)	    
	{ 13359,  13359, 13359, 13359 },	// 15 Tour Orientale FrostWolf    	(Archers)   
	{ 13359,  13359, 13359, 13359 },	// 16 Tour Occidentale FrostWolf 	(Archers)   
	{     0,      0,     0,     0 },	// 17 DONJON_FROSTWOLF 	 
	{ 12053,  13332, 13328, 13332 }, 	// 18 Flag cimetiere Hutte FrostWolf (Defenseurs)	
   }
};


static uint32 AVResourceUpdateIntervals[6] = {
		0,
		12000,
		9000,
		6000,
		3000,
		1000,
	};
// ATTENTION: Le temps de prise des drapeaux aux cim et ceux des Tours est different!! (A faire)
// Point de spawn de Dun du joueur Alliance s'il declenche son insigne Stormpike
//{ 666.550171f,   -27.814543f, 50.619865f , 3.365431f },
// Point de spawn de Frost du joueur Hordeux s'il declenche son insigne FrostWolf 
// { -1347.051636f,   -291.086670f, 90.782684f , 1.931302f },

   // Position des Draps
	static float AVControlPointCoordinates[AV_NUM_CONTROL_POINTS][4] = {
        // Alliance
        {  702.065002f,  -16.450701f, 50.135300f ,0.293762f},	// 0 Dun Baldar ( Vandar Foudrepique) 
		{  639.449036f,  -31.555246f, 46.230976f ,4.569446f},	// 1 Poste de Secours Stormpike
		{  673.541931f, -143.923157f, 63.553053f ,1.017089f},	// 2 Fortin Nord de DunBaldar 
		{  553.290466f,  -77.490593f, 51.927349f ,5.293582f},	// 3 Fortin Sud de DunBaldar 
		{  667.287720f, -293.458069f, 30.290831f ,2.524269f},	// 4 Flag Cimetiere Stormpike 
		{  202.342712f, -359.479523f, 56.376308f ,5.344635f},	// 5 Fortin de l'Aile de Glace  	    
		{   78.913200f, -405.259888f, 47.262314f ,1.089348f },	// 6 Flag Cimetiere StoneHearth 
		{  -33.163700f, -291.260010f, 15.078600f ,3.001560f},	// 7 Avant poste de StoneHearth	(Capitaine Belinda Gitepierre ) 
		{ -151.682999f, -440.278992f, 40.385300f ,4.286110f},	// 8 Fortin de StoneHearth (Commandant Randdolph ) 
		// Neutre (Entouré de Troll Hache-Hiver hostile ( WinterAxe ) )
		{ -202.469269f, -112.315948f, 78.455085f ,4.757954f},	// 9 Flag du cimetiere des neiges 
		//Horde
		{  -544.133920f, -167.923004f,  57.012199f , 6.281710f},	//  Garnison GlaceSang ( Capitaine Galvangar )
		{  -572.068420f, -262.478577f,  75.008591f , 5.383118f},	//  Tour GlaceSang 	   
		{  -611.875061f, -395.945648f,  60.799133f , 3.077975f},	//  Flag Cimetiere GlaceSang
		{  -769.999756f, -366.278320f,  90.894852f , 1.083852f},	//  Tour de la Halte ( Commandant Louis Philips + Drapeau )
		{ -1078.057129f, -342.744904f,  55.469353f , 1.830767f},	//  Flag Cimetiere FrostWolf		    
		{ -1302.117920f, -319.249176f, 113.867088f , 1.796998f},	//  Tour Orientale FrostWolf
		{ -1294.881470f, -266.878723f, 114.151833f , 3.418845f},	//  Tour Occidentale FrostWolf   
		{ -1368.438232f, -221.880875f,  98.426071f , 5.100384f},	//  DONJON_FROSTWOLF (Drek'Dar)   
		{ -1401.935181f, -306.574158f,  89.476166f , 0.098967f}	    //  Flag cimetiere Hutte FrostWolf		
	};

	// Position des Draps
	static float AVControlTopTowersCoor[AV_NUM_CONTROL_POINTS][4] = {
        // Alliance
        {      0.0f,      0.0f,    0.0f ,   0.0f},	// 0 Dun Baldar ( Vandar Foudrepique) 
		{      0.0f,      0.0f,    0.0f ,   0.0f},	// 1 Poste de Secours Stormpike
		{  677.417f, -138.578f, 76.162f , 2.377f},	// 2 Fortin Nord de DunBaldar 
		{  555.638f,  -84.356f, 64.464f , 0.112f},	// 3 Fortin Sud de DunBaldar 
		{      0.0f,      0.0f,    0.0f ,   0.0f},	// 4 Flag Cimetiere Stormpike 
		{  204.378f, -366.775f, 68.886f , 3.187f},	// 5 Fortin de l'Aile de Glace  	    
		{      0.0f,      0.0f,    0.0f ,   0.0f},	// 6 Flag Cimetiere StoneHearth 
		{ -156.227f, -447.841f, 52.903f , 1.884f},	// 7 Avant poste de StoneHearth	(Capitaine Belinda Gitepierre ) 
		{      0.0f,      0.0f,    0.0f ,   0.0f},	// 8 Fortin de StoneHearth (Commandant Randdolph ) 
		// Neutre (Entouré de Troll Hache-Hiver hostile ( WinterAxe ) )
		{      0.0f,      0.0f,    0.0f ,   0.0f},	// 9 Flag du cimetiere des neiges 
		//Horde
		{       0.0f,      0.0f,     0.0f ,   0.0f},	//  Garnison GlaceSang ( Capitaine Galvangar )
		{  -572.328f, -262.460f,  88.650f , 5.563f},	//  Tour GlaceSang 	   
		{       0.0f,      0.0f,     0.0f ,   0.0f},	//  Flag Cimetiere GlaceSang
		{  -768.145f, -363.096f, 104.534f , 0.340f},	//  Tour de la Halte ( Commandant Louis Philips + Drapeau )
		{       0.0f,      0.0f,     0.0f ,   0.0f},	//  Flag Cimetiere FrostWolf		    
		{ -1302.814f, -316.650f, 127.511f , 2.248f},	//  Tour Orientale FrostWolf
		{ -1297.892f, -266.651f, 127.783f , 5.962f},	//  Tour Occidentale FrostWolf   
		{       0.0f,      0.0f,     0.0f ,   0.0f},	//  DONJON_FROSTWOLF (Drek'Dar)   
		{       0.0f,      0.0f,     0.0f ,   0.0f}	    //  Flag cimetiere Hutte FrostWolf		
	};

	static float AVControlTopTowersRotations[AV_NUM_CONTROL_POINTS][2] = 
	{
		 // Alliance
		{ 0.0f, 0.0f },	// 0 Dun Baldar 
		{ 0.0f, 0.0f },	// 1 Poste de Secours Stormpike		
		{ 0.0f, 0.0f },	// 2 Fortin Nord de DunBaldar 	
		{ 0.0f, 0.0f },	// 3 Fortin Sud de DunBaldar     
		{ 0.0f, 0.0f }, // 4 Flag Cimetiere Stormpike 	    
		{ 0.0f, 0.0f },	// 5 Fortin de l'Aile de Glace   
		{ 0.0f, 0.0f }, // 6 Flag Cimetiere StoneHearth  	    
		{ 0.0f, 0.0f },	// 7 Avant poste de StoneHearth 	  
		{ 0.0f, 0.0f },	// 8 Fortin de StoneHearth 	   
		// Hache-Hiver 
		{ 0.0f, 0.0f },	// 9 Flag du cimetiere des neiges 	   
		//Horde
		{ 0.0f, 0.0f },	// 10 Garnison GlaceSang	   
		{ 0.0f, 0.0f },	// 11 Tour GlaceSang  	   
		{ 0.0f, 0.0f },	// 12 Flag Cimetiere GlaceSang 	   
		{ 0.0f, 0.0f },	// 13 Tour de la Halte 	    
		{ 0.0f, 0.0f },	// 14 Flag Cimetiere FrostWolf		    
		{ 0.0f, 0.0f },	// 15 Tour Orientale FrostWolf    	   
		{ 0.0f, 0.0f },	// 16 Tour Occidentale FrostWolf 	    
		{ 0.0f, 0.0f },	// 17 DONJON_FROSTWOLF 	   
		{ 0.0f, 0.0f } 	// 18 Flag cimetiere Hutte FrostWolf 																

	};

	// Pour avoir les valeurs, il fo spawns les flags ".go spawn xxxx 1" 
	// et recuperer les champs Rotation3 et 4 dans la table gameobject_spawn
	// (ne pas oublier le ".go select" suivi du ".go delete 1")
	// NB: En les laissant a zero, ca marche tres bien qd meme! 
	// (??? Question: A quoi servent ces champs ???)
	// Plus utilisé en 332.11403 (Branruz)



	static float AVControlPointRotations[AV_NUM_CONTROL_POINTS][2] = 
	{
		 // Alliance
		{ 0.0f, 0.0f },	// 0 Dun Baldar 
		{ 0.0f, 0.0f },	// 1 Poste de Secours Stormpike		
		{ 0.0f, 0.0f },	// 2 Fortin Nord de DunBaldar 	
		{ 0.0f, 0.0f },	// 3 Fortin Sud de DunBaldar     
		{ 0.0f, 0.0f }, // 4 Flag Cimetiere Stormpike 	    
		{ 0.0f, 0.0f },	// 5 Fortin de l'Aile de Glace   
		{ 0.0f, 0.0f }, // 6 Flag Cimetiere StoneHearth  	    
		{ 0.0f, 0.0f },	// 7 Avant poste de StoneHearth 	  
		{ 0.0f, 0.0f },	// 8 Fortin de StoneHearth 	   
		// Hache-Hiver (les go ne sont pas les bons, je met ceux la en attendant,  - Branruz - )
		{ 0.0f, 0.0f },	// 9 Flag du cimetiere des neiges 	   
		//Horde
		{ 0.0f, 0.0f },	// 10 Garnison GlaceSang	   
		{ 0.0f, 0.0f },	// 11 Tour GlaceSang  	   
		{ 0.0f, 0.0f },	// 12 Flag Cimetiere GlaceSang 	   
		{ 0.0f, 0.0f },	// 13 Tour de la Halte 	    
		{ 0.0f, 0.0f },	// 14 Flag Cimetiere FrostWolf		    
		{ 0.0f, 0.0f },	// 15 Tour Orientale FrostWolf    	   
		{ 0.0f, 0.0f },	// 16 Tour Occidentale FrostWolf 	    
		{ 0.0f, 0.0f },	// 17 DONJON_FROSTWOLF 	   
		{ 0.0f, 0.0f } 	// 18 Flag cimetiere Hutte FrostWolf 																

	};
    // Position des Draps supplementaires
	static float AVControlPointCoordinatesCN[4][4] = 
	{
        // Drap suplementaire au cim des neiges
        {  -215.250092f,  -91.839043f, 80.983437f ,4.745389f},	// Gauche de l'entrée du pont 
		{  -201.456284f,  -95.468994f, 80.018051f ,4.203465f},	// Droite de l'entrée du pont
		{  -188.690140f, -133.389023f, 79.139816f ,5.070544f},	// Droite en haut de la cote
		{  -203.308136f, -136.170563f, 79.534470f ,5.205633f}	// Gauche en haut de la cote 
	};
	static float AVControlPointRotationsCN[4][2] = 
	{
		 // 
		{ 0.0f, 0.0f },	// Gauche de l'entrée du pont 
		{ 0.0f, 0.0f },	// Droite de l'entrée du pont
		{ 0.0f, 0.0f },	// Droite en haut de la cote
		{ 0.0f, 0.0f }	// Gauche en haut de la cote 
	};

	    // Position des spirit Guides 
    static float AVGraveyardLocationsSG[AV_NUM_CONTROL_POINTS][4] = 
	{
		// Alliance                  
		{        0.0f,         0.0f,       0.0f  , 0.0f}, //  0 Dun
		{ 645.435181f,    30.496916f,  68.286499f, 1.514261f},	//  1 - Poste de Secours Stormpike  
		{        0.0f,         0.0f,       0.0f  , 0.0f}, //  2 Dun Nord
		{        0.0f,         0.0f,       0.0f  , 0.0f}, //  3 Dun Sud
		{ 670.838928f,  -362.755676f,  29.780735f, 5.115312f},	//  4 - Cimetiere Stormpike   
		{        0.0f,         0.0f,       0.0f  , 0.0f}, //  5 Fortin Aile de Glace
		{  73.364616f , -491.262634f,  48.748631f, 4.726540f},	//  6 - Cimetiere StoneHearth     
		{        0.0f,         0.0f,       0.0f  , 0.0f}, //  7 Avant Poste StoneHearth
		{        0.0f,         0.0f,       0.0f  , 0.0f}, //  8 Fortin StoneHearth
		// HacheHiver
		{ -158.413318f ,   28.264214f,  77.037827f, 1.234658f},	//  9 - Cimetiere des Neiges   
		// Horde
		{          0.0f,         0.0f,       0.0f , 0.0f},	//  10 - GARNISON_GLACESANG 	   
		{          0.0f,         0.0f,       0.0f , 0.0f},	//  11 - TOUR_GLACESANG 	   
		{  -529.739502f , -397.457947f,  49.767445f, 4.616569f  },	//  12 - Cimetiere GlaceSang 
		{          0.0f,         0.0f,       0.0f , 0.0f},	//  13 - TOUR_DE_LA_HALTE 	    
		{ -1092.197321f,  -257.362366f,  57.645336f, 1.401933f},	//  14 - Cimetiere FrostWolf     
		{          0.0f,         0.0f,       0.0f , 0.0f},	//  15 - TOUR_ORIENT_FROSTWOLF 	   
		{          0.0f,         0.0f,       0.0f , 0.0f},	//  16 - TOUR_OCCID_FROSTWOLF 	    
		{          0.0f,         0.0f,       0.0f , 0.0f},	//  17 - DONJON_FROSTWOLF 	   
		{ -1486.657471f,  -327.554901f, 100.660393f, 3.574343f} 	//  18 - Hutte FrostWorf 
		
	};
    // Position des cimetieres (point de rez)
	static float AVGraveyardLocations[AV_NUM_CONTROL_POINTS][4] = {
		// Alliance                  
		{        0.0f,         0.0f,       0.0f  , 0.0f}, //  0 Dun
		{ 643.455322f,   34.988720f, 68.801903f  , 4.725740f},	//  1 - Poste de Secours Stormpike  
		{        0.0f,         0.0f,       0.0f  , 0.0f}, //  2 Dun Nord
		{        0.0f,         0.0f,       0.0f  , 0.0f}, //  3 Dun Sud
		{ 675.871399f, -373.446808f, 29.780853f  , 1.895951f},	//  4 - Cimetiere Stormpike   
		{        0.0f,         0.0f,       0.0f  , 0.0f}, //  5 Fortin Aile de Glace
		{  72.932968f, -498.022491f, 48.747192f  , 1.645411f},	//  6 - Cimetiere StoneHearth     
		{        0.0f,         0.0f,       0.0f  , 0.0f}, //  7 Avant Poste StoneHearth
		{        0.0f,         0.0f,       0.0f  , 0.0f}, //  8 Fortin StoneHearth
		// HacheHiver
		{ -156.948471f,  34.124222f, 77.115745f  , 4.241162f},	//  9 - Cimetiere des Neiges   
		// Horde
		{          0.0f,         0.0f,       0.0f , 0.0f},	//  10 - GARNISON_GLACESANG 	   
		{          0.0f,         0.0f,       0.0f , 0.0f},	//  11 - TOUR_GLACESANG 	   
		{  -529.844177f, -404.425934f, 49.473831f , 2.682134f},	//  12 - Cimetiere GlaceSang 
		{          0.0f,         0.0f,       0.0f , 0.0f},	//  13 - TOUR_DE_LA_HALTE 	    
		{ -1092.025146f, -248.078812f, 57.703514f , 4.780723f},	//  14 - Cimetiere FrostWolf     
		{          0.0f,         0.0f,       0.0f , 0.0f},	//  15 - TOUR_ORIENT_FROSTWOLF 	   
		{          0.0f,         0.0f,       0.0f , 0.0f},	//  16 - TOUR_OCCID_FROSTWOLF 	    
		{          0.0f,         0.0f,       0.0f , 0.0f},	//  17 - DONJON_FROSTWOLF 	   
		{ -1499.481201f, -333.295074f, 101.135582f , 0.256047f} 	//  18 - Hutte FrostWorf 
	};
    
                       
	// Bidouille pour eviter un index en plus sur les points de controle
	// contrairement aux autres bg, tts les points de controle ne pointe pas un cimetieres!
	static uint32 IsGraveyard[AV_NUM_CONTROL_POINTS] = 
   // 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 
	{ 0,1,0,0,1,0,1,0,0,1, 0, 0, 1, 0, 1, 0, 0, 0, 1 };

	// Id des GameObjects dans la zone proche
	static unsigned short AVZoneGoId[AV_NUM_CONTROL_POINTS] =
	{ 16766, 29804, 29285, 25441, 24918, 
	  27756, 31077, 16928,  8313, 24919,
	  25697, 24916, 28005, 11902, 11902 };

	static float AVNoBaseGYLocations[2][4] = 
	{    //,873.002,-491.284,96.542,
		{ 866.089417f, -497.179749f, 96.461525f , 3.364645f}, // Point d'entrée Alliance
		{ -1436.499756f, -600.447632f, 51.209682f , 1.033584f}, // Point d'entrée de la horde
		//,-1437.67,-610.089,51.162,
	};

	static const char * AVControlPointNames[AV_NUM_CONTROL_POINTS] = 
	{
		// Alliance
		"Dun Baldar", 
        "Le poste de secours Stormpike",
        "Le fortin Nord de Dun Baldar", 		
        "Le fortin Sud de Dun Baldar", 	
		"Le cimetiere Stormpike", 		
        "Le Fortin de l'Aile de Glace",     
        "Le cimetiere de StoneHearth",  
        "L'avant poste de StoneHearth", 	  
        "Le fortin de StoneHearth", 	
		// Hache-Hiver
        "Le cimetiere des neiges", 	   
        // Horde
        "La garnisson de GlaceSang", 	   
        "La tour de GlaceSang", 	   
        "Le cimetiere de GlaceSang", 	   
        "La tour de la Halte", 	    
        "Le cimetiere FrostWolf",
        "La tour orientale FrostWolf", 	   
        "La tour occidentale FrostWolf", 	    
        "Le donjon FrostWorlf",
        "La hutte de guerrison FrostWolf" 

	};

	
	// GoId des flags
	static uint32 AVControlPointTowerGoIds[AV_NUM_SPAWN_TYPES]=
    // NEUTRAL ALLI-ATTACK H2-ATTACK ALLI-CONTROLLED H2_CONTROLLED
	 { 180418, 178940, 179435, 178925, 178364 }; // En haut des tours
	static uint32 AVControlPointGoIds[AV_NUM_CONTROL_POINTS][AV_NUM_SPAWN_TYPES] = 
	{
// NEUTRAL ALLI-ATTACK H2-ATTACK ALLI-CONTROLLED H2_CONTROLLED
		 // Alliance
		{ 180418, 179286, 179287, 178365, 178364 },	// 0 Dun Baldar (en haut de la tour)
		{ 180418, 179286, 179287, 178365, 178364 },	// 1 Poste de Secours Stormpike		
		{ 180418, 178940, 179435, 178925, 178943 },	// 2 Fortin Nord de DunBaldar 	
		{ 180418, 178940, 179435, 178925, 178943 },	// 3 Fortin Sud de DunBaldar     
		{ 180418, 179286, 179287, 178365, 178364 }, // 4 Flag Cimetiere Stormpike 	    
		{ 180418, 178940, 179435, 178925, 178943 },	// 5 Fortin de l'Aile de Glace   
		{ 180418, 179286, 179287, 178365, 178364 }, // 6 Flag Cimetiere StoneHearth  	    
		{ 180418, 178940, 179435, 178925, 178943 },	// 7 Avant poste de StoneHearth 	  
		{ 180418, 178940, 179435, 178925, 178943 },	// 8 Fortin de StoneHearth 	   
		// Hache-Hiver (les go ne sont pas les bons, je met ceux la en attendant )
		{ 180418, 179286, 179287, 178365, 178364 },	// 9 Flag du cimetiere des neiges 	   
		//Horde
		{ 180418, 178940, 179435, 178925, 178943 },	// 10 Garnison GlaceSang	   
		{ 180418, 178940, 179435, 178925, 178943 },	// 11 Tour GlaceSang  	   
		{ 180418, 179286, 179287, 178365, 178364 },	// 12 Flag Cimetiere GlaceSang 	   
		{ 180418, 178940, 179435, 178925, 178943 },	// 13 Tour de la Halte 	    
		{ 180418, 179286, 179287, 178365, 178364 },	// 14 Flag Cimetiere FrostWolf		    
		{ 180418, 178940, 179435, 178925, 178943 },	// 15 Tour Orientale FrostWolf    	   
		{ 180418, 178940, 179435, 178925, 178943 },	// 16 Tour Occidentale FrostWolf 	    
		{      0,      0,      0,      0,      0 },	// 17 DONJON_FROSTWOLF 	
		{ 180418,   179286, 179287, 178365, 178364 }, 	// 18 Flag cimetiere Hutte FrostWolf 
	};

	// Position des incendies des tours (coord viennent de Summit)
#define MAX_FEU_TOURS	10
struct AVSpawnLocation { float x; float y; float z; float o; };
const AVSpawnLocation AVIncendieLocations[AV_NUM_CONTROL_POINTS][MAX_FEU_TOURS] = 
{
 // 0 Dun Baldar
 {
  { 0.0f,0.0f,0.0f },
 },
 // 1 Poste de Secours Stormpike
 {
    { 0.0f,0.0f,0.0f },
 },
 // 2 Fortin Nord de DunBaldar 
 {
  { 667.917297f, -137.020660f, 63.645058f, 1.094849f },
  { 680.675903f, -125.554802f, 63.666199f, 0.089540f },
  { 685.923828f, -148.394562f, 56.618328f, 1.138047f },
  { 667.368164f, -147.642548f, 56.621113f, 4.523115f },
  { 666.998413f, -143.095612f, 49.673607f, 4.122564f },
  { 676.130615f, -123.653824f, 49.672577f, 2.744190f },
  { 666.790649f, -127.394073f, 49.638577f, 3.348947f },
  { 0.0f, 0.0f, 0.0f },
 },
 // 3 Fortin Sud de DunBaldar
 {	
  { 568.422852f, -83.935478f, 51.942871f, 2.324003f },
  { 547.981018f, -93.095024f, 51.942841f, 0.898504f },
  { 549.647034f, -80.551971f, 44.820171f, 2.858073f },
  { 552.589355f, -96.653267f, 44.819138f, 0.619688f },
  { 562.575439f, -78.773170f, 37.949261f, 1.644633f },
  { 568.352722f, -92.772797f, 37.949261f, 0.761060f },
  { 575.015137f, -78.856613f, 37.921127f, 0.596126f },
  { 0.0f, 0.0f, 0.0f },
 },
 // 4 Flag Cimetiere Stormpike
 {
  { 0.0f, 0.0f, 0.0f }
 },
 // 5 Fortin de l'Aile de Glace
 { 
  { 198.799316f, -361.116241f, 56.391476f, 5.363494f },
  { 217.211273f, -375.348236f, 56.368317f, 0.494023f },
  { 200.838547f, -362.912231f, 49.267738f, 2.708846f },
  { 208.172836f, -379.141357f, 49.267738f, 5.826873f },
  { 209.692886f, -356.993683f, 42.398838f, 2.335778f },
  { 223.358307f, -374.435516f, 42.396271f, 5.288878f },
  { 222.380539f, -357.525726f, 42.361271f, 1.208734f },
  { 0.0f, 0.0f, 0.0f }
 },
 // 6 Flag Cimetiere StoneHearth
 {
  { 0.0f, 0.0f, 0.0f },
 },
 // 7 Avant poste de StoneHearth 
 {	
  { -162.115128f, -458.521393f, 40.389114f, 5.921130f },
  { -146.283035f, -451.985413f, 40.392101f, 1.593585f },
  { -154.911880f, -440.288513f, 33.280674f, 3.392147f },
  { -166.347992f, -458.094086f, 33.279636f, 4.809789f },
  { -146.530441f, -458.876343f, 26.394993f, 5.257466f },
  { -146.420120f, -445.917725f, 26.408756f, 1.020243f },
  { -139.567215f, -456.592163f, 26.380287f, 2.649942f },
  { 0.0f, 0.0f, 0.0f }
 },
 // 8 Fortin de StoneHearth 
 {
  { 0.0f, 0.0f, 0.0f }
 },
 // 9 Flag du cimetiere des neiges 
 {
  { 0.0f, 0.0f, 0.0f }
 },
 // 10 Garnison GlaceSang
 {
  { 0.0f, 0.0f, 0.0f }
 },
 // 11 Tour GlaceSang
 {
  { -569.760010f, -263.288513f, 75.008682f, 2.104091f },
  { -570.742493f, -271.907837f, 74.988449f, 3.238992f },
  { -578.531372f, -253.600327f, 74.959206f, 0.647177f },
  { -561.223206f, -260.727386f, 68.457199f, 3.820190f },
  { -579.971008f, -265.969604f, 68.469582f, 2.068751f },
  { -571.457458f, -256.716736f, 63.293938f, 0.509736f },
  { -567.616211f, -265.129028f, 59.324265f, 4.538830f },
  { -579.480103f, -261.568329f, 52.497894f, 1.821352f },
  { -566.202026f, -260.122009f, 52.728100f, 5.634461f },
  { 0.0f, 0.0f, 0.0f }
 },
 // 12 Cim GlaceSang
 {
  { 0.0f, 0.0f, 0.0f }
 },
 // 13 Tour de la Halte 
  {
		
		{ -769.523010f, -358.549561f, 68.635841f, 3.853180f },
		{ -762.504395f, -362.904480f, 68.543678f, 5.730278f },
		{ -761.258606f, -360.407471f, 72.666672f, 1.618719f },
		{ -772.757813f, -365.472168f, 79.463570f, 5.408264f },
		{ -764.915161f, -358.086975f, 84.355766f, 3.004945f },
		{ -773.573181f, -366.593475f, 84.355766f, 4.414734f },
		{ -777.062927f, -370.895477f, 90.868019f, 5.832375f },
		{ -758.860840f, -358.962280f, 90.825951f, 4.073083f },
		{ -768.840515f, -362.985535f, 90.894974f, 4.304777f },
		{ 0.0f, 0.0f, 0.0f }
	},
  // 14 Flag Cimetiere FrostWolf
  {
		{ 0.0f, 0.0f, 0.0f }
  },
  // 15 Tour Orientale FrostWolf
  {	
		{ -1304.755249f, -322.213409f, 91.419922f, 5.141234f },
		{ -1304.633057f, -310.950684f, 91.677505f, 0.746931f },
		{ -1301.672974f, -309.800598f, 95.747795f, 3.586145f },
		{ -1303.074585f, -321.931915f, 102.658630f, 0.259982f },
		{ -1303.221680f, -310.574463f, 107.328194f, 3.590071f },
		{ -1302.911133f, -323.911835f, 107.328049f, 4.913466f },
		{ -1302.600464f, -328.408600f, 113.846321f, 1.210312f },
		{ -1303.177612f, -307.762390f, 113.797401f, 6.177956f },
		{ -1305.213989f, -315.778412f, 113.867081f, 5.251186f },
		{ 0.0f, 0.0f, 0.0f }
  },
  // 16 Tour Occidentale FrostWolf
  {	
   { -1299.614014f, -263.684998f, 114.151276f, 4.944896f },
   { -1297.918335f, -269.959930f, 114.151276f, 4.772108f },
   { -1304.088379f, -272.878387f, 114.098404f, 5.200147f },
   { -1287.346313f, -267.391785f, 114.115089f, 1.826860f },
   { -1298.288818f, -272.091309f, 107.612137f, 2.883221f },
   { -1298.199463f, -258.249390f, 107.612183f, 3.052081f },
   { -1300.405640f, -273.112244f, 100.317612f, 4.929180f },
   { -1291.555908f, -261.006134f, 91.648979f, 3.244500f },
   { -1299.686279f, -273.323853f, 91.897820f, 3.876745f },
   { 0.0f, 0.0f, 0.0f }
  },
  // 17 DONJON_FROSTWOLF 	
  {
   { 0.0f, 0.0f, 0.0f }
  },
  // 18 Flag cimetiere Hutte FrostWolf
  {
   { 0.0f, 0.0f, 0.0f }
  }
};
// Position des Gardes des drapeaux (coord viennent de Summit)
#define MAX_GARDES_DRAPEAUX	8
const AVSpawnLocation AVGardesLocations[AV_NUM_CONTROL_POINTS][MAX_GARDES_DRAPEAUX] = 
{
 // 0 Dun Baldar
 {
  { 0.0f,0.0f,0.0f }
 },
 // 1 Poste de Secours Stormpike
 {
	{ 639.205f, -36.738f, 45.438f, 4.409f },
    { 633.960f, -35.098f, 45.069f, 4.409f },  
	{ 630.508f, -30.176f, 45.547f, 4.409f },  
    { 647.201f, -33.855f, 46.927f, 4.409f },  
    { 0.0f,0.0f,0.0f }
 },
 // 2 Fortin Nord de DunBaldar
 {	 
	{ 672.141296f, -120.239807f, 64.147659f, 2.379753f },
	{ 664.585083f, -126.076088f, 64.120972f, 2.768524f },
	{ 661.464905f, -137.339371f, 64.216965f, 3.436112f },
	{ 0.0f, 0.0f, 0.0f }
 },
 // 3 Fortin Sud de DunBaldar
 {		
	{ 564.124146f, -71.107430f, 52.488060f, 1.300614f },
	{ 571.863770f, -77.084518f, 52.367657f, 0.295303f },
	{ 574.969543f, -90.801270f, 52.412132f, 5.910901f },
	{ 0.0f, 0.0f, 0.0f }
  },
   // 4 Flag Cimetiere Stormpike
  {
    { 672.000f, -296.523f, 30.393f, 2.746f },
    { 666.708f, -300.979f, 30.127f, 2.746f },  
	{ 672.000f, -295.138f, 30.521f, 2.746f },  
    { 666.708f, -288.979f, 30.094f, 2.746f },  
    { 0.0f,0.0f,0.0f }
  },
  // 5 Fortin de l'Aile de Glace
  {	
   { 222.674530f, -378.605408f, 57.147060f, 5.577111f },
   { 210.093506f, -384.971558f, 56.793076f, 4.595366f },
   { 199.602188f, -380.995575f, 56.864891f, 3.754991f },
   { 192.782074f, -370.546204f, 57.015110f, 3.586132f },
   { 193.377426f, -360.313507f, 57.044708f, 2.891055f },
   { 201.253113f, -351.977631f, 56.802952f, 3.036354f },
   { 0.0f, 0.0f, 0.0f } 
  },
  // 6 Flag Cimetiere StoneHearth
  {
   {  84.464f, -406.247f, 48.097023f, 1.814f },
   {  78.968f, -410.917f, 48.568752f, 1.814f },  
   {  75.395f, -405.791f, 46.703625f, 1.814f },  
   {  77.418f, -401.547f, 46.276844f, 1.814f },  
   { 0.0f,0.0f,0.0f }
  },
  // 7 Avant poste de StoneHearth 
  {
	{ -139.692780f, -445.390533f, 40.982857f, 0.527005f },
	{ -140.723969f, -457.597168f, 40.862610f, 5.643875f },
	{ -148.143784f, -464.959808f, 40.933720f, 5.639948f },
	{ -161.013535f, -466.793640f, 41.118568f, 4.512904f },
	{ -170.595215f, -457.926331f, 40.875153f, 3.385859f },
	{ -172.157013f, -447.972717f, 40.888519f, 2.894984f },
	{ 0.0f, 0.0f, 0.0f }
   },
    // 8 Fortin de StoneHearth 
   {
    { 0.0f, 0.0f, 0.0f }
   },
   // 9 Flag du cimetiere des neiges 
   {
     {  -200.292f, -117.337f, 78.363f, 4.948f },
     {  -199.160f, -112.468f, 78.514f, 4.948f },  
     {  -207.718f, -112.178f, 78.787f, 4.948f },  
     {  -206.300f, -106.391f, 78.892f, 4.948f },  
     { 0.0f,0.0f,0.0f }
   },
   // 10 Garnison GlaceSang
   {
    { 0.0f, 0.0f, 0.0f }
   },
   // 11 Tour GlaceSang
   {
		{ -568.816711f, -255.542038f, 75.008690f, 1.025736f },
		{ -580.106445f, -264.803497f, 74.931145f, 3.931711f },
		{ -572.966248f, -271.499786f, 74.933746f, 4.198746f },
		{ -565.585876f, -268.645294f, 74.914063f, 5.459307f },
		{ 0.0f, 0.0f, 0.0f }
	},
	// 12 Cim GlaceSang
    {
	  {  -614.066f, -395.123f, 60.858f, 2.464f },
      {  -611.611f, -393.205f, 60.844f, 2.277f },  
      {  -615.997f, -399.059f, 60.457f, 3.365f },  
      {  -610.651f, -402.908f, 60.374f, 4.497f },  
      { 0.0f,0.0f,0.0f }
    },
   // 13 Tour de la Halte
	{		
		{ -760.347595f, -358.281586f, 90.885597f, 0.358147f },
		{ -759.719116f, -367.059113f, 90.826775f, 5.722414f },
		{ -768.441956f, -372.756653f, 90.933365f, 4.991992f },
		{ -775.595032f, -365.525177f, 90.894867f, 2.989226f },
		{ -771.311890f, -353.218842f, 90.821220f, 1.795420f },
		{ 0.0f, 0.0f, 0.0f }
	},
	// 14 Flag Cimetiere FrostWolf
    {
	  { -1082.805f, -345.122f, 55.216f, 1.345f },
      { -1073.285f, -343.603f, 55.610f, 1.791f },  
      { -1077.088f, -348.367f, 54.233f, 5.294f },  
      { -1076.193f, -338.160f, 55.246f, 0.041f },  
      { 0.0f,0.0f,0.0f }
    },
    // 15 Tour Orientale FrostWolf
	{	
		{ -1297.069092f, -309.338623f, 113.769043f, 1.002180f },
		{ -1293.462036f, -316.529602f, 113.774048f, 0.067557f },
		{ -1298.062256f, -326.274994f, 113.820679f, 5.015566f },
		{ -1311.626953f, -317.071228f, 113.775551f, 2.977457f },
		{ -1305.169922f, -309.871796f, 113.824280f, 5.192283f },
		{ -1293.822021f, -317.798065f, 113.771339f, 3.103136f },
		{ 0.0f, 0.0f, 0.0f }
	},
	// 16 Tour Occidentale FrostWolf
	{
		{ -1299.964233f, -275.591461f, 114.055862f, 1.241742f },
		{ -1306.900757f, -268.969574f, 114.055481f, 3.464420f },
		{ -1305.109375f, -261.103363f, 114.068550f, 2.298103f },
		{ -1295.956177f, -258.076904f, 114.056999f, 1.277085f },
		{ -1288.039917f, -264.262085f, 114.115341f, 0.224653f },
		{ 0.0f, 0.0f, 0.0f }
	},
      // 17 DONJON_FROSTWOLF 
    {
     { 0.0f, 0.0f, 0.0f }
    },
     // 18 Flag cimetiere Hutte FrostWolf
    {
     { -1407.020f, -304.231f, 89.321f, 0.282f },
     { -1400.707f, -310.185f, 89.373f, 0.555f },  
     { -1406.856f, -307.934f, 89.284f, 0.439f },  
     { -1404.951f, -311.988f, 89.413f, 0.439f },  
     { 0.0f,0.0f,0.0f }
    }
  };
/* End BG Data */

void AlteracValley::SpawnBuff(uint32 x)
{
	// Aucun spawn de buff ici
}


//-------------------------------------------------------------------------------------------------
void AlteracValley::OnCreate()
{
 
    // Grille Alliance
	GameObject *gate = SpawnGameObject(180424,m_mapMgr->GetMapId(), 798.056763f, -493.719696f, 99.695282f, 3.027709f, 32, 114, 3.85f);
	gate->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	// Grille Horde 
	// 
	//gate = SpawnGameObject(180424, 30, -1387.344604f, -550.101013f, 54.992661f, 0.551349f, 32, 114, 3.85f);
	gate = SpawnGameObject(180424, m_mapMgr->GetMapId(), -1387.344604f, -550.101013f, 54.992661f, 0.551349f, 32, 114, 3.85f);
	gate->SetByte(GAMEOBJECT_BYTES_1,GAMEOBJECT_BYTES_ANIMPROGRESS, 100);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	// 3 Spirit-guides chacun au depart du Bg
	// Le spirit guide du cim des neiges apparait sur prise de flag!
	for(uint32 i=0; i < AV_NUM_CONTROL_POINTS;i++)
	{
	 if(!IsGraveyard[i]) continue;
	 switch(i)
	 {
		 // Alliance
	  case AV_CONTROL_POINT_POSTE_STORMPIKE:
	  case AV_CONTROL_POINT_CIM_STORMPIKE:
	  case AV_CONTROL_POINT_CIM_STONEHEARTH:
	   if(IsGraveyard[i]) // En phase avec le tableau de coordonnées ? (secu)
	   {
		m_spiritGuides[i] = SpawnSpiritGuide(AVGraveyardLocationsSG[i][0], AVGraveyardLocationsSG[i][1], AVGraveyardLocationsSG[i][2],AVGraveyardLocationsSG[i][3], 0);
	    if(m_spiritGuides[i]) AddSpiritGuide(m_spiritGuides[i]);
	   }
	   else Log.Debug("AlteracValley","Spirit guide Alliance: %u n'existe pas ou est decalé!",i);
		break;
	  // Horde
	  case AV_CONTROL_POINT_CIM_GLACESANG:
	  case AV_CONTROL_POINT_CIM_FROSTWOLF:
	  case AV_CONTROL_POINT_HUTTE_FROSTWOLF:
       if(IsGraveyard[i]) // En phase avec le tableau de coordonnées ? (secu)
	   {
		m_spiritGuides[i] = SpawnSpiritGuide(AVGraveyardLocationsSG[i][0], AVGraveyardLocationsSG[i][1], AVGraveyardLocationsSG[i][2],AVGraveyardLocationsSG[i][3], 1);
	    if(m_spiritGuides[i]) AddSpiritGuide(m_spiritGuides[i]);
	   }
	    else Log.Debug("AlteracValley","Spirit guide Horde: %u n'existe pas ou est decalé!",i);
	    break;
	  default: break;
	 }
	}


	// spawn (default) control points
    //SpawnControlPoint(AV_CONTROL_POINT_DUNBALDAR             ,	AV_SPAWN_TYPE_ALLIANCE_CONTROLLED);
	SpawnControlPoint(AV_CONTROL_POINT_POSTE_STORMPIKE       ,	AV_SPAWN_TYPE_ALLIANCE_CONTROLLED);
	SpawnControlPoint(AV_CONTROL_POINT_FORTIN_NORD_DUNBALDAR ,	AV_SPAWN_TYPE_ALLIANCE_CONTROLLED); 
	SpawnControlPoint(AV_CONTROL_POINT_FORTIN_SUD_DUNBALDAR  ,	AV_SPAWN_TYPE_ALLIANCE_CONTROLLED); 	 	
	SpawnControlPoint(AV_CONTROL_POINT_CIM_STORMPIKE         ,	AV_SPAWN_TYPE_ALLIANCE_CONTROLLED); 	      	    
	SpawnControlPoint(AV_CONTROL_POINT_AILE_DE_GLACE         ,	AV_SPAWN_TYPE_ALLIANCE_CONTROLLED); 	      
	SpawnControlPoint(AV_CONTROL_POINT_CIM_STONEHEARTH       ,	AV_SPAWN_TYPE_ALLIANCE_CONTROLLED); 	    
	SpawnControlPoint(AV_CONTROL_POINT_AV_POSTE_STONEHEARTH  ,	AV_SPAWN_TYPE_ALLIANCE_CONTROLLED); 	 
	SpawnControlPoint(AV_CONTROL_POINT_FORTIN_STONEHEARTH    ,	AV_SPAWN_TYPE_ALLIANCE_CONTROLLED);    
	SpawnControlPoint(AV_CONTROL_POINT_CIM_DES_NEIGES        ,    AV_SPAWN_TYPE_NEUTRAL            ); 	        
	SpawnControlPoint(AV_CONTROL_POINT_GARNISON_GLACESANG    , 	AV_SPAWN_TYPE_HORDE_CONTROLLED   ); 
	SpawnControlPoint(AV_CONTROL_POINT_TOUR_GLACESANG        , 	AV_SPAWN_TYPE_HORDE_CONTROLLED   );        
	SpawnControlPoint(AV_CONTROL_POINT_CIM_GLACESANG         , 	AV_SPAWN_TYPE_HORDE_CONTROLLED   );      
	SpawnControlPoint(AV_CONTROL_POINT_TOUR_DE_LA_HALTE      , 	AV_SPAWN_TYPE_HORDE_CONTROLLED   );      
	SpawnControlPoint(AV_CONTROL_POINT_CIM_FROSTWOLF         , 	AV_SPAWN_TYPE_HORDE_CONTROLLED   );     
	SpawnControlPoint(AV_CONTROL_POINT_TOUR_ORIENT_FROSTWOLF ,  AV_SPAWN_TYPE_HORDE_CONTROLLED   ); 
	SpawnControlPoint(AV_CONTROL_POINT_TOUR_OCCID_FROSTWOLF  , 	AV_SPAWN_TYPE_HORDE_CONTROLLED   );
	//SpawnControlPoint(AV_CONTROL_POINT_DONJON_FROSTWOLF    , 	AV_SPAWN_TYPE_HORDE_CONTROLLED   );     
	SpawnControlPoint(AV_CONTROL_POINT_HUTTE_FROSTWOLF       ,  AV_SPAWN_TYPE_HORDE_CONTROLLED   ); 

	
    // Drapeaux en haut des tours
	SpawnControlTopTowers(AV_CONTROL_POINT_FORTIN_SUD_DUNBALDAR ,AV_SPAWN_TYPE_ALLIANCE_CONTROLLED);
	SpawnControlTopTowers(AV_CONTROL_POINT_FORTIN_NORD_DUNBALDAR,AV_SPAWN_TYPE_ALLIANCE_CONTROLLED);
	SpawnControlTopTowers(AV_CONTROL_POINT_AILE_DE_GLACE        ,AV_SPAWN_TYPE_ALLIANCE_CONTROLLED);
	SpawnControlTopTowers(AV_CONTROL_POINT_AV_POSTE_STONEHEARTH ,AV_SPAWN_TYPE_ALLIANCE_CONTROLLED);

	SpawnControlTopTowers(AV_CONTROL_POINT_TOUR_GLACESANG       ,AV_SPAWN_TYPE_HORDE_CONTROLLED);
	SpawnControlTopTowers(AV_CONTROL_POINT_TOUR_DE_LA_HALTE     ,AV_SPAWN_TYPE_HORDE_CONTROLLED);
	SpawnControlTopTowers(AV_CONTROL_POINT_TOUR_ORIENT_FROSTWOLF,AV_SPAWN_TYPE_HORDE_CONTROLLED);
	SpawnControlTopTowers(AV_CONTROL_POINT_TOUR_OCCID_FROSTWOLF ,AV_SPAWN_TYPE_HORDE_CONTROLLED);
	

	// Spawns des Archers
	SpawnArchers(AV_CONTROL_POINT_FORTIN_NORD_DUNBALDAR , AV_TEAM_ALLY);
	SpawnArchers(AV_CONTROL_POINT_FORTIN_SUD_DUNBALDAR  , AV_TEAM_ALLY); 	 	
	SpawnArchers(AV_CONTROL_POINT_AILE_DE_GLACE         , AV_TEAM_ALLY); 	      
	SpawnArchers(AV_CONTROL_POINT_AV_POSTE_STONEHEARTH  , AV_TEAM_ALLY); 	 
	SpawnArchers(AV_CONTROL_POINT_TOUR_GLACESANG        , AV_TEAM_H2  );        
	SpawnArchers(AV_CONTROL_POINT_TOUR_DE_LA_HALTE      , AV_TEAM_H2  );      
	SpawnArchers(AV_CONTROL_POINT_TOUR_ORIENT_FROSTWOLF , AV_TEAM_H2  ); 
	SpawnArchers(AV_CONTROL_POINT_TOUR_OCCID_FROSTWOLF  , AV_TEAM_H2  );   

	// Spawns des Defenseurs des Drapeaux des cim
	FireGuards(AV_CONTROL_POINT_POSTE_STORMPIKE , AV_TEAM_ALLY);
	FireGuards(AV_CONTROL_POINT_CIM_STORMPIKE   , AV_TEAM_ALLY); 	      	    
	FireGuards(AV_CONTROL_POINT_CIM_STONEHEARTH , AV_TEAM_ALLY); 	    
	FireGuards(AV_CONTROL_POINT_CIM_GLACESANG   , AV_TEAM_H2  );      
	FireGuards(AV_CONTROL_POINT_CIM_FROSTWOLF   , AV_TEAM_H2  );     
	FireGuards(AV_CONTROL_POINT_HUTTE_FROSTWOLF , AV_TEAM_H2  );

	m_lastRenforts[0]=BASE_RENFORTS; // Renforts Ally
	m_lastRenforts[1]=BASE_RENFORTS; // Renforts H2

	// worldsates scores
	WorldStateManager &wState = m_mapMgr->GetDBStateManager();

	wState.CreateWorldState(WS_AV_ALLY_SCR, m_lastRenforts[0]);
	wState.CreateWorldState(WS_AV_H2_SCR  , m_lastRenforts[1]);
	wState.CreateWorldState(WS_AV_ALLY_SCR_DSP , 1);
	wState.CreateWorldState(WS_AV_HORDE_SCR_DSP, 1);

	// Points de controle sur la Map

	// Les mines
    wState.CreateWorldState(WS_AV_MIRONDEEP_TROGG_CNTR,1);
    wState.CreateWorldState(WS_AV_MFROIDEDENT_KOBOLD_CONTROL,1);

    // Cimetieres
    wState.CreateWorldState(WS_AV_PSEC_STORM_ALLY_CNTR,1);
    wState.CreateWorldState(WS_AV_CSTORM_ALLI_CONTROL,1);
    wState.CreateWorldState(WS_AV_CSHEARTH_ALLY_CONTR,1);
    wState.CreateWorldState(WS_AV_CNEIGES_TROLL_CONTROL,1);
    wState.CreateWorldState(WS_AV_CGLACESANG_H2_CONTROL,1);
    wState.CreateWorldState(WS_AV_CFWOLF_H2_CONTROL,1);
    wState.CreateWorldState(WS_AV_HFROSTWOLF_H2_CONTROL,1);
    // Les Tours
    wState.CreateWorldState(WS_AV_FNORD_DUN_CONTR,1);
    wState.CreateWorldState(WS_AV_FSUD_DUN_CONTR,1);
    wState.CreateWorldState(WS_AV_FAILE_DE_GLACE_CONTROL,1);
    wState.CreateWorldState(WS_AV_FSHEARTH_CONTR,1);
    wState.CreateWorldState(WS_AV_THALTE_CONTR,1);
    wState.CreateWorldState(WS_AV_TGLACESANG_CONTR,1);
    wState.CreateWorldState(WS_AV_TORIENT_FROSTWOLF_CONTROL,1);
    wState.CreateWorldState(WS_AV_TOCCID_FROSTWOLF_CONTROL,1);
}

//-------------------------------------------------------------------------------------------------
void AlteracValley::OnStart()
{
 for(uint32 i = 0; i < 2; ++i) 
 {
  for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr) 
  {
	(*itr)->RemoveAura(BG_PREPARATION); // On vire le debuff
  }
 }

    // PlaySoundToAll(SOUND_OPEN_GATE); ??? 
    // open gates
    for(list<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
		(*itr)->SetUInt32Value(GAMEOBJECT_FLAGS, GO_FLAG_TRIGGERED);
		(*itr)->SetByte( GAMEOBJECT_BYTES_1,0, 0); // GAMEOBJECT_STATE
	}
    

	PlaySoundToAll(SOUND_BATTLEGROUND_BEGIN);
	m_started = true;	  
}
//-------------------------------------------------------------------------------------------------
AlteracValley::AlteracValley(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr,id,lgroup,t)
{
 
	m_playerCountPerTeam=40;

     for(int i=0; i < AV_NUM_CONTROL_POINTS;i++)
	 {
		m_controlPointAuras[i] = NULL;
		m_controlPoints[i] = NULL;
		m_controlPointsTopTowers[i] = NULL;
		m_controlPointAurasTopTowers[i] = NULL;
		m_spiritGuides[i] = NULL;
		m_basesAssaultedBy[i] = -1;
		m_basesOwnedBy[i] = -1;
	 }
	 // Drap du cimetieres des neiges
     for(int i=0; i < 4;i++)
	 {
	   m_drapeauCimDesNeiges[i]=NULL;
	 }

    // Le bg est divisé en 2, Alli et H2, 3 cims chacun au depart
    // Le cimetiere des neiges est neutre et appartient aux Hache-Hivers
	// Controlé Alliance
	m_basesOwnedBy[AV_CONTROL_POINT_DUNBALDAR] = 0;           
	m_basesOwnedBy[AV_CONTROL_POINT_POSTE_STORMPIKE] = 0;       
	m_basesOwnedBy[AV_CONTROL_POINT_FORTIN_NORD_DUNBALDAR] = 0; 
	m_basesOwnedBy[AV_CONTROL_POINT_FORTIN_SUD_DUNBALDAR] = 0; 	 
	m_basesOwnedBy[AV_CONTROL_POINT_CIM_STORMPIKE] = 0; 	     
	m_basesOwnedBy[AV_CONTROL_POINT_AILE_DE_GLACE] = 0; 	     
	m_basesOwnedBy[AV_CONTROL_POINT_CIM_STONEHEARTH] = 0; 	     
	m_basesOwnedBy[AV_CONTROL_POINT_AV_POSTE_STONEHEARTH] = 0; 	 
	m_basesOwnedBy[AV_CONTROL_POINT_FORTIN_STONEHEARTH] = 0; 
	// Neutre (Camp de base des Troll Hache-Hiver hostile ( WinterAxe ) )
	m_basesOwnedBy[AV_CONTROL_POINT_CIM_DES_NEIGES] = -1; 
	// Controlé Horde
	m_basesOwnedBy[AV_CONTROL_POINT_GARNISON_GLACESANG] = 1; 	
	m_basesOwnedBy[AV_CONTROL_POINT_TOUR_GLACESANG] = 1; 	    
	m_basesOwnedBy[AV_CONTROL_POINT_CIM_GLACESANG] = 1; 	    
	m_basesOwnedBy[AV_CONTROL_POINT_TOUR_DE_LA_HALTE] = 1; 	    
	m_basesOwnedBy[AV_CONTROL_POINT_CIM_FROSTWOLF] = 1; 	    
	m_basesOwnedBy[AV_CONTROL_POINT_TOUR_ORIENT_FROSTWOLF] = 1; 
	m_basesOwnedBy[AV_CONTROL_POINT_TOUR_OCCID_FROSTWOLF] = 1; 	
	m_basesOwnedBy[AV_CONTROL_POINT_DONJON_FROSTWOLF] = 1; 	    
	m_basesOwnedBy[AV_CONTROL_POINT_HUTTE_FROSTWOLF] = 1;

	// Appartient a l'alliance
	DefFlag[AV_CONTROL_POINT_DUNBALDAR][0]             = false; // Flag attaqué 
	DefFlag[AV_CONTROL_POINT_DUNBALDAR][1]             = false; // Flag neutre au depart 
	DefFlag[AV_CONTROL_POINT_POSTE_STORMPIKE][0]       = false; 
	DefFlag[AV_CONTROL_POINT_POSTE_STORMPIKE][1]       = false;
	DefFlag[AV_CONTROL_POINT_FORTIN_NORD_DUNBALDAR][0] = false; 
	DefFlag[AV_CONTROL_POINT_FORTIN_NORD_DUNBALDAR][1] = false;
	DefFlag[AV_CONTROL_POINT_FORTIN_SUD_DUNBALDAR][0]  = false; 
	DefFlag[AV_CONTROL_POINT_FORTIN_SUD_DUNBALDAR][1]  = false;
	DefFlag[AV_CONTROL_POINT_CIM_STORMPIKE][0]         = false; 
	DefFlag[AV_CONTROL_POINT_CIM_STORMPIKE][1]         = false;
	DefFlag[AV_CONTROL_POINT_AILE_DE_GLACE][0]         = false; 
	DefFlag[AV_CONTROL_POINT_AILE_DE_GLACE][1]         = false;
	DefFlag[AV_CONTROL_POINT_CIM_STONEHEARTH][0]       = false; 
	DefFlag[AV_CONTROL_POINT_CIM_STONEHEARTH][1]       = false;
	DefFlag[AV_CONTROL_POINT_AV_POSTE_STONEHEARTH][0]  = false; 
	DefFlag[AV_CONTROL_POINT_AV_POSTE_STONEHEARTH][1]  = false;
	DefFlag[AV_CONTROL_POINT_FORTIN_STONEHEARTH][0]    = false; 
	DefFlag[AV_CONTROL_POINT_FORTIN_STONEHEARTH][1]    = false;
    // Cim des neiges, appartient aux Hache-Hiver, Drap "neutre" Alli/Horde
	DefFlag[AV_CONTROL_POINT_CIM_DES_NEIGES][0] = false; 
	DefFlag[AV_CONTROL_POINT_CIM_DES_NEIGES][1] = true; // Flag Neutre a l'init du Bg (le seul)
    // Appartient a la Horde 
	DefFlag[AV_CONTROL_POINT_GARNISON_GLACESANG][0] = false; 
	DefFlag[AV_CONTROL_POINT_GARNISON_GLACESANG][1] = false;
	DefFlag[AV_CONTROL_POINT_TOUR_GLACESANG][0]     = false; 
	DefFlag[AV_CONTROL_POINT_TOUR_GLACESANG][1]     = false;
	DefFlag[AV_CONTROL_POINT_CIM_GLACESANG][0]      = false; 
	DefFlag[AV_CONTROL_POINT_CIM_GLACESANG][1]      = false;
	DefFlag[AV_CONTROL_POINT_TOUR_DE_LA_HALTE][0]   = false; 
	DefFlag[AV_CONTROL_POINT_TOUR_DE_LA_HALTE][1]   = false;
	DefFlag[AV_CONTROL_POINT_CIM_FROSTWOLF][0]      = false; 
	DefFlag[AV_CONTROL_POINT_CIM_FROSTWOLF][1]      = false;
	DefFlag[AV_CONTROL_POINT_TOUR_ORIENT_FROSTWOLF][0] = false; 
	DefFlag[AV_CONTROL_POINT_TOUR_ORIENT_FROSTWOLF][1] = false;
	DefFlag[AV_CONTROL_POINT_TOUR_OCCID_FROSTWOLF][0]  = false; 
	DefFlag[AV_CONTROL_POINT_TOUR_OCCID_FROSTWOLF][1]  = false;
	DefFlag[AV_CONTROL_POINT_DONJON_FROSTWOLF][0] = false; 
	DefFlag[AV_CONTROL_POINT_DONJON_FROSTWOLF][1] = false;
	DefFlag[AV_CONTROL_POINT_HUTTE_FROSTWOLF][0]  = false; 
	DefFlag[AV_CONTROL_POINT_HUTTE_FROSTWOLF][1]  = false;

	// Debut du Bg, aucune tour n'est detruite
	 for(int i=0; i < AV_NUM_CONTROL_POINTS;i++)
	 {
	   m_DestroyedTower[i]=false;
	 }

}
//-------------------------------------------------------------------------------------------------
void AlteracValley::Init()
{
	CBattleground::Init();
}
//-------------------------------------------------------------------------------------------------
AlteracValley::~AlteracValley()
{
 //
}
//-------------------------------------------------------------------------------------------------
void AlteracValley::EventUpdateResources(uint32 Team)
{
	Log.Debug("###Vallee d'Alterac: EventUpdateResources(Team: %s)",(Team) ? "Horde" : "Alliance");	

}
//-------------------------------------------------------------------------------------------------
void AlteracValley::HookOnPlayerDeath(Player * plr)
{
	plr->m_bgScore.Deaths++;
	//UpdatePvPData();
}
//-------------------------------------------------------------------------------------------------
void AlteracValley::HookOnMount(Player * plr)
{
	// nothing in this BG
}
//-------------------------------------------------------------------------------------------------
void AlteracValley::HookOnPlayerKill(Player * plr, Unit * pVictim)
{
	if(pVictim->IsPlayer())
	{
		plr->m_bgScore.KillingBlows++;
		//UpdatePvPData();
	}
}
//-------------------------------------------------------------------------------------------------
void AlteracValley::HookOnHK(Player * plr)
{
	plr->m_bgScore.HonorableKills++;
	//UpdatePvPData();
}
//-------------------------------------------------------------------------------------------------
void AlteracValley::OnAddPlayer(Player * plr)
{
	if(!m_started)
		plr->CastSpell(plr, BG_PREPARATION, true);
}
//-------------------------------------------------------------------------------------------------
void AlteracValley::OnRemovePlayer(Player * plr)
{
	plr->RemoveAura(BG_PREPARATION);
}
//-------------------------------------------------------------------------------------------------
void AlteracValley::HookFlagDrop(Player * plr, GameObject * obj)
{
	// nothing?
	Log.Debug("AlteracValley","HookFlagDrop Trig!!");
}
//-------------------------------------------------------------------------------------------------
void AlteracValley::HookFlagStand(Player * plr, GameObject * obj)
{
	// nothing?
	Log.Debug("AlteracValley","HookFlagStand Trig!!");
}
//-------------------------------------------------------------------------------------------------
LocationVector AlteracValley::GetStartingCoords(uint32 Team)
{
 //printf("AlteracValley::GetStartingCoords(Team: %d)",Team);
 if(Team > 1) Team = 1; // Secu
 return LocationVector(AVNoBaseGYLocations[Team][0], AVNoBaseGYLocations[Team][1], AVNoBaseGYLocations[Team][2],AVNoBaseGYLocations[Team][3]);
}

//-------------------------------------------------------------------------------------------------
// A completer au fur et a mesure....
void AlteracValley::HookOnAreaTrigger(Player * plr, uint32 id)
{
	/*Note Randdrick : ne sert à rien
	uint32 spellid=0;
	int32 buffslot = -1;*/

	if(plr->isDead()) return; // dont apply to dead players... :P

	switch(id)
	{
	 //case 2412: // Entrée par les montagnes (Alliance)
	 //	 return;
	 case 2606: // Sortie du bg tunnel Horde 
	 case 2608: // Sortie du bg tunnel Alliance
		RemovePlayer(plr,false);
		return;
	 case 3328: // Tour Occidentale FrostWolf (Coté Horde )
	 case 3326: // Tour de la halte (Coté Horde)
	 case 3327: // Inconnu (Randdrick)
     case 3329: // Fortin de StoneHearth (Coté Alliance)
	 case 3331: // Fortin Nord de dunbaldar (Coté Alliance)
		 Log.Warning("## AlteracValley", "Areatrigger (%u) non traité!", id);
		 return;
		 
	 default:
		 Log.Debug("## AlteracValley", "Areatrigger inconnu (id %u) non traité!", id);
		return;
		
	}
}
//-------------------------------------------------------------------------------------------------
bool AlteracValley::HookHandleRepop(Player * plr)
{
	printf("AlteracValley::HookHandleRepop(Player: %s)",plr->GetName());

	if(plr->m_bgTeam > 1) return(true); // Secu, seulement 2 Teams 1:Alliance 0:H2
	 
	LocationVector dest( AVNoBaseGYLocations[plr->m_bgTeam][0], AVNoBaseGYLocations[plr->m_bgTeam][1], 
		                 AVNoBaseGYLocations[plr->m_bgTeam][2], AVNoBaseGYLocations[plr->m_bgTeam][3] );
	float current_distance = 999999.0f;
	float dist;

	for(uint32 i = 0; i < AV_NUM_CONTROL_POINTS; ++i)
	{
		if((!IsGraveyard[i])
		  || AVGraveyardLocations[i][0]==0.0f || AVGraveyardLocations[i][1]==0.0f 
		  || AVGraveyardLocations[i][2]==0.0f   ) continue; // Secu

		if(m_basesOwnedBy[i] == (int32)plr->m_bgTeam)
		{   
			dist = plr->GetPositionV()->Distance2DSq(AVGraveyardLocations[i][0], AVGraveyardLocations[i][1]);
			if(dist < current_distance)
			{
				current_distance = dist;
				dest.ChangeCoords(AVGraveyardLocations[i][0], AVGraveyardLocations[i][1], 
					              AVGraveyardLocations[i][2], AVGraveyardLocations[i][3]);
			}
		  
		}
	}

	// si aucun cim trouver, retour au point d'entrée du Bg en auto
	plr->SafeTeleport(plr->GetMapId(),plr->GetInstanceID(),dest);
	
	return true;
}
//-------------------------------------------------------------------------------------------------
// Gestion des drapeaux du BG
//-------------------------------------------------------------------------------------------------
void AlteracValley::SpawnControlPoint(uint32 Id, uint32 Type)
{
 GameObjectInfo * gi, * gi_aura;    
	gi = GameObjectNameStorage.LookupEntry(AVControlPointGoIds[Id][Type]);
	if(gi == NULL) return;

	gi_aura = gi->trap.spellId /*sound3*/ ? GameObjectNameStorage.LookupEntry(gi->trap.spellId /*sound3*/) : NULL;

	
	if(m_controlPoints[Id] == NULL) // Init du Bg
	{
		// Drap dans les tours et draps des cimetieres
		m_controlPoints[Id] = SpawnGameObject(gi->ID, m_mapMgr->GetMapId(), 
			AVControlPointCoordinates[Id][0], AVControlPointCoordinates[Id][1],
			AVControlPointCoordinates[Id][2], AVControlPointCoordinates[Id][3], 0, 35, 1.0f);

		//m_controlPoints[Id]->SetFloatValue(GAMEOBJECT_PARENTROTATION_2, AVControlPointRotations[Id][0]); 
		//m_controlPoints[Id]->SetFloatValue(GAMEOBJECT_PARENTROTATION_3, AVControlPointRotations[Id][1]); 
		m_controlPoints[Id]->SetByte(GAMEOBJECT_BYTES_1,0, 1); // GAMEOBJECT_STATE
		m_controlPoints[Id]->SetByte(GAMEOBJECT_BYTES_1, 1, gi->Type); // GAMEOBJECT_TYPE_ID
		m_controlPoints[Id]->SetByte(GAMEOBJECT_BYTES_1, 3, 100); // GAMEOBJECT_ANIMPROGRESS
		m_controlPoints[Id]->SetUInt32Value(GAMEOBJECT_DYNAMIC, 1);
		m_controlPoints[Id]->SetUInt32Value(GAMEOBJECT_DISPLAYID, gi->DisplayID);

		switch(Type)
		{
		case AV_SPAWN_TYPE_ALLIANCE_ASSAULT:
			m_controlPoints[Id]->SetUInt32Value(GAMEOBJECT_FACTION, 2);
			break;

		case AV_SPAWN_TYPE_ALLIANCE_CONTROLLED:
			m_controlPoints[Id]->SetUInt32Value(GAMEOBJECT_FACTION, 2);
			break;

		case AV_SPAWN_TYPE_HORDE_ASSAULT:
			m_controlPoints[Id]->SetUInt32Value(GAMEOBJECT_FACTION, 1);
			break; 

		case AV_SPAWN_TYPE_HORDE_CONTROLLED:
			m_controlPoints[Id]->SetUInt32Value(GAMEOBJECT_FACTION, 1);
			break;

		default:
			m_controlPoints[Id]->SetUInt32Value(GAMEOBJECT_FACTION, 0); //35);		// neutral
			break;
		}
        m_controlPoints[Id]->bannerslot = Id;
		m_controlPoints[Id]->PushToWorld(m_mapMgr);
	}
	else // Bg en cours
	{
		// Drap dans les tours et draps des cimetieres
		if(m_controlPoints[Id]->IsInWorld())
			m_controlPoints[Id]->RemoveFromWorld(false);

		// Calcul d'un nouveau Guid, sinon le joueur verra l'ancien spawn
		m_controlPoints[Id]->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());
		m_controlPoints[Id]->SetUInt32Value(OBJECT_FIELD_ENTRY, gi->ID);
		m_controlPoints[Id]->SetUInt32Value(GAMEOBJECT_DISPLAYID, gi->DisplayID);
		m_controlPoints[Id]->SetByte(GAMEOBJECT_BYTES_1, 1, gi->Type); // GAMEOBJECT_TYPE_ID

		switch(Type)
		{
		case AV_SPAWN_TYPE_ALLIANCE_ASSAULT:
			m_controlPoints[Id]->SetUInt32Value(GAMEOBJECT_FACTION, 2);
			break;

		case AV_SPAWN_TYPE_ALLIANCE_CONTROLLED:
			m_controlPoints[Id]->SetUInt32Value(GAMEOBJECT_FACTION, 2);
			break;

		case AV_SPAWN_TYPE_HORDE_ASSAULT:
			m_controlPoints[Id]->SetUInt32Value(GAMEOBJECT_FACTION, 1);
			break;

		case AV_SPAWN_TYPE_HORDE_CONTROLLED:
		    m_controlPoints[Id]->SetUInt32Value(GAMEOBJECT_FACTION, 1);
			break;

		default:
			m_controlPoints[Id]->SetUInt32Value(GAMEOBJECT_FACTION, 0);		// neutral
			break;
		}
        
		m_controlPoints[Id]->SetInfo(gi);
		m_controlPoints[Id]->PushToWorld(m_mapMgr);
	}

	
    // Le truc curieux c'est ke les auras sont legerement decalé, 
	// pourtant c'est le meme point de spawn ( pareil sur l'offi!! )
	if(gi_aura==NULL) // ya une aura  sur ce GoId?
	{
		// remove it if it exists
		if(m_controlPointAuras[Id]!=NULL && m_controlPointAuras[Id]->IsInWorld())
			m_controlPointAuras[Id]->RemoveFromWorld(false);

    	return;
	}
    // Aura des Draps des cimetieres
	if(m_controlPointAuras[Id] == NULL) // Inti Bg
	{
		m_controlPointAuras[Id] = SpawnGameObject(gi_aura->ID, m_mapMgr->GetMapId(), 
			AVControlPointCoordinates[Id][0], AVControlPointCoordinates[Id][1],
			AVControlPointCoordinates[Id][2], AVControlPointCoordinates[Id][3], 0, 35, 3.5f);

		//m_controlPointAuras[Id]->SetFloatValue(GAMEOBJECT_PARENTROTATION_2, 0.0f); 
		//m_controlPointAuras[Id]->SetFloatValue(GAMEOBJECT_PARENTROTATION_3, 0.0f); 
		m_controlPointAuras[Id]->SetByte(GAMEOBJECT_BYTES_1,0, 1); // GAMEOBJECT_STATE
		m_controlPointAuras[Id]->SetByte(GAMEOBJECT_BYTES_1, 1, 6); // GAMEOBJECT_TYPE_ID
		m_controlPointAuras[Id]->SetByte(GAMEOBJECT_BYTES_1, 3, 100); // GAMEOBJECT_ANIMPROGRESS
		m_controlPointAuras[Id]->bannerauraslot = Id;
		m_controlPointAuras[Id]->PushToWorld(m_mapMgr); 
	}
	else // Bg en cours
	{
		if(m_controlPointAuras[Id]->IsInWorld())
			m_controlPointAuras[Id]->RemoveFromWorld(false);

		// re-spawn the aura
		m_controlPointAuras[Id]->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());
		m_controlPointAuras[Id]->SetUInt32Value(OBJECT_FIELD_ENTRY, gi_aura->ID);
		m_controlPointAuras[Id]->SetUInt32Value(GAMEOBJECT_DISPLAYID, gi_aura->DisplayID);
		m_controlPointAuras[Id]->SetInfo(gi_aura);
		m_controlPointAuras[Id]->PushToWorld(m_mapMgr);
	}	
}
//-----------------------------------------------------------------------------------
void AlteracValley::SpawnControlTopTowers(uint32 Id, uint32 Type)
{  
 GameObjectInfo * gi, * gi_aura;    
	gi = GameObjectNameStorage.LookupEntry(AVControlPointTowerGoIds[Type]);
	if(gi == NULL) return;

	gi_aura = gi->trap.spellId /*sound3*/ ? GameObjectNameStorage.LookupEntry(gi->trap.spellId /*sound3*/) : NULL;

	
	if(m_controlPointsTopTowers[Id] == NULL) // Init du Bg
	{
		// Drapeaux en haut des tours
        if(AVControlTopTowersCoor[Id][0] != 0.0f)
		{
          m_controlPointsTopTowers[Id] = SpawnGameObject(gi->ID, m_mapMgr->GetMapId(), 
			AVControlTopTowersCoor[Id][0], AVControlTopTowersCoor[Id][1],
			AVControlTopTowersCoor[Id][2], AVControlTopTowersCoor[Id][3], 0, 35, 2.0f);;

		  m_controlPointsTopTowers[Id]->SetFloatValue(GAMEOBJECT_PARENTROTATION_2, AVControlPointRotations[Id][0]); 
          m_controlPointsTopTowers[Id]->SetFloatValue(GAMEOBJECT_PARENTROTATION_3, AVControlPointRotations[Id][1]); 
		  m_controlPointsTopTowers[Id]->SetByte(GAMEOBJECT_BYTES_1,0, 0); // GAMEOBJECT_STATE
		  m_controlPointsTopTowers[Id]->SetByte(GAMEOBJECT_BYTES_1, 1, gi->Type); // GAMEOBJECT_TYPE_ID
		  m_controlPointsTopTowers[Id]->SetByte(GAMEOBJECT_BYTES_1, 3 , 100);// GAMEOBJECT_ANIMPROGRESS
		  m_controlPointsTopTowers[Id]->SetUInt32Value(GAMEOBJECT_DYNAMIC, 0);
		  m_controlPointsTopTowers[Id]->SetUInt32Value(GAMEOBJECT_DISPLAYID, gi->DisplayID);
	      m_controlPointsTopTowers[Id]->SetUInt32Value(GAMEOBJECT_FACTION, 0);
		  m_controlPointsTopTowers[Id]->bannerslot = Id;
		  m_controlPointsTopTowers[Id]->PushToWorld(m_mapMgr);
		}
		else { return; }
	}
	else // Bg en cours
	{
	   // Drap en haut des tours
       if(AVControlTopTowersCoor[Id][0] != 0.0f)
	   {
        if(m_controlPointsTopTowers[Id]->IsInWorld())
			m_controlPointsTopTowers[Id]->RemoveFromWorld(false);

		// Calcul d'un nouveau Guid, sinon le joueur verra l'ancien spawn
		m_controlPointsTopTowers[Id]->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());
		m_controlPointsTopTowers[Id]->SetUInt32Value(OBJECT_FIELD_ENTRY, gi->ID);
		m_controlPointsTopTowers[Id]->SetUInt32Value(GAMEOBJECT_DISPLAYID, gi->DisplayID);
		m_controlPointsTopTowers[Id]->SetByte(GAMEOBJECT_BYTES_1, 1, gi->Type); // GAMEOBJECT_TYPE_ID
		m_controlPointsTopTowers[Id]->SetUInt32Value(GAMEOBJECT_FACTION, 0);
        m_controlPointsTopTowers[Id]->SetInfo(gi);
		m_controlPointsTopTowers[Id]->PushToWorld(m_mapMgr);
	   }
	}

	
    // Le truc curieux c'est ke les auras sont legerement decalé, 
	// pourtant c'est le meme point de spawn ( pareil sur l'offi!! )
	if(gi_aura==NULL) // ya une aura  sur ce GoId?
	{
		// remove it if it exists
		if(m_controlPointAurasTopTowers[Id]!=NULL && m_controlPointAurasTopTowers[Id]->IsInWorld())
			m_controlPointAurasTopTowers[Id]->RemoveFromWorld(false);

    	return;
	}
    // Aura des Draps des cimetieres
	if(m_controlPointAurasTopTowers[Id] == NULL) // Inti Bg
	{
		m_controlPointAurasTopTowers[Id] = SpawnGameObject(gi_aura->ID, m_mapMgr->GetMapId(), 
			AVControlTopTowersCoor[Id][0], AVControlTopTowersCoor[Id][1],
			AVControlTopTowersCoor[Id][2], AVControlTopTowersCoor[Id][3], 0, 35, 2.0f);

		m_controlPointAurasTopTowers[Id]->SetFloatValue(GAMEOBJECT_PARENTROTATION_2, 0.0f); 
		m_controlPointAurasTopTowers[Id]->SetFloatValue(GAMEOBJECT_PARENTROTATION_3, 0.0f); 
		m_controlPointAurasTopTowers[Id]->SetByte(GAMEOBJECT_BYTES_1,0, 1); // GAMEOBJECT_STATE
		m_controlPointAurasTopTowers[Id]->SetByte(GAMEOBJECT_BYTES_1, 1, 6); // GAMEOBJECT_TYPE_ID
		m_controlPointAurasTopTowers[Id]->SetByte(GAMEOBJECT_BYTES_1, 3, 100); // GAMEOBJECT_ANIMPROGRESS
		m_controlPointAurasTopTowers[Id]->bannerauraslot = Id;
		m_controlPointAurasTopTowers[Id]->PushToWorld(m_mapMgr);
	}
	else // Bg en cours
	{
		if(m_controlPointAurasTopTowers[Id]->IsInWorld())
			m_controlPointAurasTopTowers[Id]->RemoveFromWorld(false);

		// re-spawn the aura
		m_controlPointAurasTopTowers[Id]->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());
		m_controlPointAurasTopTowers[Id]->SetUInt32Value(OBJECT_FIELD_ENTRY, gi_aura->ID);
		m_controlPointAurasTopTowers[Id]->SetUInt32Value(GAMEOBJECT_DISPLAYID, gi_aura->DisplayID);
		m_controlPointAurasTopTowers[Id]->SetInfo(gi_aura);
		m_controlPointAurasTopTowers[Id]->PushToWorld(m_mapMgr);
	}	
}
//-------------------------------------------------------------------------------------------------
// Gestion des Drapeaux du cimetiere des neiges (4 draps qui marque l'entrée)
//-------------------------------------------------------------------------------------------------
void AlteracValley::SpawnDrapCimDesNeiges(uint32 Type)
{
	
	uint32 Go_Type;
	// A faire: On enleve les drap si le Drap du cim est contesté ou neutre!!
	GameObjectInfo * Flag_cim;

	// NEUTRAL ALLI-ATTACK H2-ATTACK ALLI-CONTROLLED H2_CONTROLLED
	// 180418,   178940,    179435,    178925,        178943 },
    switch(Type)
	{
	 case AV_SPAWN_TYPE_HORDE_CONTROLLED     : Go_Type = 178943; break;
     case AV_SPAWN_TYPE_ALLIANCE_CONTROLLED  : Go_Type = 178925; break;
     case AV_SPAWN_TYPE_HORDE_ASSAULT        : Go_Type = 179435; break;
     case AV_SPAWN_TYPE_ALLIANCE_ASSAULT     : Go_Type = 178940; break;
	 default                                 : return;
	}

	Flag_cim = GameObjectNameStorage.LookupEntry(Go_Type);
	if(Flag_cim == NULL) return;

	if(m_basesOwnedBy[AV_CONTROL_POINT_CIM_DES_NEIGES] == -1) 
	{
		// despawn des flags si le drap est contesté!!!
		//for(int i=0; i<4 ;i++)
		//{
		// if(m_drapeauCimDesNeiges[i]->IsInWorld())
		//	m_drapeauCimDesNeiges[i]->RemoveFromWorld(false);
		//}
		return;
	}

	for(int i=0; i<4;i++) // 4 drapeaux non actif
	{

	 if(m_drapeauCimDesNeiges[i]==NULL) // init, 
	 {
	 
		m_drapeauCimDesNeiges[i] = SpawnGameObject(Flag_cim->ID, m_mapMgr->GetMapId(), 
			AVControlPointCoordinatesCN[i][0], AVControlPointCoordinatesCN[i][1],
			AVControlPointCoordinatesCN[i][2], AVControlPointCoordinatesCN[i][3], 0, 35, 1.0f);

		m_drapeauCimDesNeiges[i]->SetFloatValue(GAMEOBJECT_PARENTROTATION_2, AVControlPointRotationsCN[i][0]); 
		m_drapeauCimDesNeiges[i]->SetFloatValue(GAMEOBJECT_PARENTROTATION_3, AVControlPointRotationsCN[i][1]); 
		m_drapeauCimDesNeiges[i]->SetByte(GAMEOBJECT_BYTES_1,0, 0); // GAMEOBJECT_STATE // Pas actif, juste pour le decor
		m_drapeauCimDesNeiges[i]->SetByte(GAMEOBJECT_BYTES_1, 1 , Flag_cim->Type); // GAMEOBJECT_TYPE_ID
		m_drapeauCimDesNeiges[i]->SetByte(GAMEOBJECT_BYTES_1, 3, 100); // GAMEOBJECT_ANIMPROGRESS
		m_drapeauCimDesNeiges[i]->SetUInt32Value(GAMEOBJECT_DYNAMIC, 0); // Pas actif, juste pour le decor
		m_drapeauCimDesNeiges[i]->SetUInt32Value(GAMEOBJECT_DISPLAYID, Flag_cim->DisplayID);
		m_drapeauCimDesNeiges[i]->SetUInt32Value(GAMEOBJECT_FACTION, 2);
        m_drapeauCimDesNeiges[i]->bannerslot = AV_CONTROL_POINT_CIM_DES_NEIGES;
		m_drapeauCimDesNeiges[i]->PushToWorld(m_mapMgr);
		
	 }
	 else // Bg en cours le drap a été pris au moins une fois!
	 {
	  if(m_drapeauCimDesNeiges[i]->IsInWorld())
			m_drapeauCimDesNeiges[i]->RemoveFromWorld(false);

		// Calcul d'un nouveau Guid, sinon le joueur verra l'ancien spawn
		m_drapeauCimDesNeiges[i]->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());
		m_drapeauCimDesNeiges[i]->SetUInt32Value(OBJECT_FIELD_ENTRY, Flag_cim->ID);
		m_drapeauCimDesNeiges[i]->SetUInt32Value(GAMEOBJECT_DISPLAYID, Flag_cim->DisplayID);
		m_drapeauCimDesNeiges[i]->SetByte(GAMEOBJECT_BYTES_1, 1, Flag_cim->Type); // GAMEOBJECT_TYPE_ID
		m_drapeauCimDesNeiges[i]->SetUInt32Value(GAMEOBJECT_FACTION, 35);
		m_drapeauCimDesNeiges[i]->SetInfo(Flag_cim);
		m_drapeauCimDesNeiges[i]->PushToWorld(m_mapMgr);
	 }
	} // End for(i....)
	
}
//-------------------------------------------------------------------------------------------------
// Gestion des incendies des tours et des gardes defenseur aux cims
// ( NB: Les tours detruites ne sont plus capturables!)
//-------------------------------------------------------------------------------------------------
void AlteracValley::FireGuards(uint32 Id, uint32 Team)
{
 bool Incendie_des_Tours = false;
 bool Spawns_Gardes_Drapeaux = false;
 bool SexeId = true;
 
 int i;
 
 // netoyage du spool d'event du point controlé
 sEventMgr.RemoveEvents(this,EVENT_AV_CONTROL_CP00 + Id);

 switch(Id)
 {
  // Incendie
  case	AV_CONTROL_POINT_FORTIN_NORD_DUNBALDAR : // 2 Fortin Nord de DunBaldar 
  case	AV_CONTROL_POINT_FORTIN_SUD_DUNBALDAR  : // 3 Fortin Sud de DunBaldar 
  case	AV_CONTROL_POINT_AILE_DE_GLACE 	       : // 5 Fortin de l'Aile de Glace  
  case	AV_CONTROL_POINT_FORTIN_STONEHEARTH    : // 8 Fortin de StoneHearth (Commandant Randdolph ) 
  case	AV_CONTROL_POINT_TOUR_GLACESANG 	   : // 11 Tour GlaceSang 	
  case	AV_CONTROL_POINT_TOUR_DE_LA_HALTE 	   : // 13 Tour de la Halte ( Commandant Louis Philips + Drapeau )
  case	AV_CONTROL_POINT_TOUR_ORIENT_FROSTWOLF : // 15 Tour Orientale FrostWolf   
  case	AV_CONTROL_POINT_TOUR_OCCID_FROSTWOLF  : // 16 Tour Occidentale FrostWolf 
	    if(m_DestroyedTower[Id]) return;
		Incendie_des_Tours  = true;
		break;	  

  // Les Gardes qui gardent les drapeaux
  case	AV_CONTROL_POINT_POSTE_STORMPIKE       : // 1 Poste de Secours Stormpike
  case	AV_CONTROL_POINT_CIM_STORMPIKE 	       : // 4 Flag Cimetiere Stormpike 	
  case	AV_CONTROL_POINT_CIM_STONEHEARTH 	   : // 6 Flag Cimetiere StoneHearth 
  case	AV_CONTROL_POINT_CIM_DES_NEIGES 	   : // 9 Flag du cimetiere des neiges 
  case	AV_CONTROL_POINT_CIM_GLACESANG 	       : // 12 Flag Cimetiere GlaceSang
  case	AV_CONTROL_POINT_CIM_FROSTWOLF 	       : // 14 Flag Cimetiere FrostWolf	
  case	AV_CONTROL_POINT_HUTTE_FROSTWOLF       : // 18 Flag cimetiere Hutte FrostWolf
	    Spawns_Gardes_Drapeaux = true;
		break;

  // Ben la ya rien! ('fin pas encore)
  case	AV_CONTROL_POINT_DUNBALDAR             : // 0 Dun Baldar ( Vandar Foudrepique)
  case	AV_CONTROL_POINT_AV_POSTE_STONEHEARTH  : // 7 Avant poste de StoneHearth (Capitaine Belinda Gitepierre ) 
  case	AV_CONTROL_POINT_GARNISON_GLACESANG    : // 10 Garnison GlaceSang ( Capitaine Galvangar ) 
  case	AV_CONTROL_POINT_DONJON_FROSTWOLF 	   : // 17 DONJON_FROSTWOLF (Drek'Dar)   
  default: return;
 }

 if(Incendie_des_Tours)
 { 
  const AVSpawnLocation *pIncendieLocations = AVIncendieLocations[Id];
  GameObject *go;
  while(pIncendieLocations->x != 0.0f)
  {
   go = SpawnGameObject(179065,m_mapMgr->GetMapId(),pIncendieLocations->x, pIncendieLocations->y,
	                                          pIncendieLocations->z, pIncendieLocations->o, 
											  0, 35, 1.35f);
   if(go!=NULL) go->PushToWorld(GetMapMgr());
   pIncendieLocations++;
  }
  m_DestroyedTower[Id] = true; // La tour est detruite!
  if(AVControlPointNames[Id][1]=='a') SexeId=false;
  SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE, 0, "%s a ete detruit%spar %s!",AVControlPointNames[Id],
	                           (SexeId) ? " " : "e ", Team ? "la Horde" : "l'Alliance");

  // Bonnus d'honneur
  m_mainLock.Acquire();
  for(set<Player*>::iterator itr = m_players[Team].begin(); itr != m_players[Team].end(); ++itr)
   	(*itr)->m_bgScore.BonusHonor += 62;
  // HonorHandler::AddHonorPointsToPlayer((*itr), 62); // A voir
  m_mainLock.Release();

  if(Team) 
  { 
    if(AVWorldStates[Id][AV_SPAWN_TYPE_HORDE_CONTROLLED]!= 0)
	{
     if(AVWorldStates[Id][AV_SPAWN_TYPE_HORDE_ASSAULT]!= 0)
             SetWorldState(AVWorldStates[Id][AV_SPAWN_TYPE_HORDE_ASSAULT],0);

     SetWorldState(AVWorldStates[Id][AV_SPAWN_TYPE_HORDE_CONTROLLED],1);
	 //SendChatMessage(CHAT_MSG_BG_EVENT_HORDE,0,
	 //	  "Worldstate %d (CONTROL-DESTROY) Ok!",Id);
	 }
	// else SendChatMessage(CHAT_MSG_BG_EVENT_HORDE, 0,
	// 	  "Worldstate %d (CONTROL-DESTROY) est nulle!",Id);
     
  }
  else
  {
   if(AVWorldStates[Id][AV_SPAWN_TYPE_ALLIANCE_CONTROLLED]!=0)
   {
	if(AVWorldStates[Id][AV_SPAWN_TYPE_ALLIANCE_ASSAULT]!= 0)
             SetWorldState(AVWorldStates[Id][AV_SPAWN_TYPE_ALLIANCE_ASSAULT],0);
    SetWorldState(AVWorldStates[Id][AV_SPAWN_TYPE_ALLIANCE_CONTROLLED],1);
	//	SendChatMessage(CHAT_MSG_BG_EVENT_ALLIANCE,0,
	// 	  "Worldstate %d (CONTROL-DESTROY) Ok!",Id);
   }
   //else SendChatMessage(CHAT_MSG_BG_EVENT_ALLIANCE, 0,
   //	 	  "Worldstate %d (CONTROL-DESTROY) est nulle!",Id);
  }
  //A FAIRE: SetWorldState(Scores_ecran_player[Team], Renforts); 
  //UpdatePvPData();

 }
 else if(Spawns_Gardes_Drapeaux)
 {
   if(AVGuardsFlags[Team][Id][0] == 0) return;

	const AVSpawnLocation *pGardesLocations = AVGardesLocations[Id];

    Creature *cGuards;     
    
	i=0;
	while(pGardesLocations->x != 0.0f)
	{
     if(i>AV_NBGARDES_DRAP) i=0;
	
	 cGuards = GetMapMgr()->GetInterface()->SpawnCreature(AVGuardsFlags[Team][Id][i], 
		 pGardesLocations->x, pGardesLocations->y, 
		 pGardesLocations->z, pGardesLocations->o, false, false, 0, 0);
   
	 if(cGuards) 
	 {
		 cGuards->PushToWorld(GetMapMgr());
	     pGardesLocations++;
	     i++;
	 }
	 else break;
	 
	}
	
 }

}
//-------------------------------------------------------------------------------------------------
void AlteracValley::SpawnArchers(int32 Id, uint32 Team)
{
	if(AVGuardsFlags[Team][Id][0] == 0) return;

	const AVSpawnLocation *pArchersLocations = AVGardesLocations[Id];

    Creature *cArchers;     
    
	int i=0;
	while(pArchersLocations->x != 0.0f)
	{
     if(i>AV_NBGARDES_DRAP) i=0;

	 if(AVGuardsFlags[Team][Id][i] == 0) break;
	 cArchers = GetMapMgr()->GetInterface()->SpawnCreature(AVGuardsFlags[Team][Id][i], 
		 pArchersLocations->x, pArchersLocations->y, 
		 pArchersLocations->z, pArchersLocations->o, false, false, 0, 0);
   
	 if(cArchers) 
	 {
		 cArchers->PushToWorld(GetMapMgr());
	     pArchersLocations++;
	     i++;
	 }
	 else break;
	 
	}
		 
}

//-------------------------------------------------------------------------------------------------
// Appelé par le gestionnaire d'event (Event posé par AssaultControlPoint(...))
//-------------------------------------------------------------------------------------------------
void AlteracValley::CaptureControlPoint(uint32 Id, uint32 Team)
{
	if(m_basesOwnedBy[Id] != -1)
	{
		// there is a very slim chance of this happening, 2 teams events could clash..
		// just in case...
		return;
	}

	// anticheat, not really necessary because this is a server method but anyway
	if(m_basesAssaultedBy[Id] != (int32)Team) return;

	m_basesOwnedBy[Id] = Team;
	m_basesAssaultedBy[Id]=-1;

	
	if(IsGraveyard[Id]) // Y'a t-il un cim a cet endroit ?
	{
	 if(m_spiritGuides[Id] != NULL)
	 {  // On vire l'ancien s'il existe
		RemoveSpiritGuide(m_spiritGuides[Id]);
		m_spiritGuides[Id]->Despawn(0,0);
	 }

	 // spawn du spirit guide qui va bien! (fct de la faction)
	 m_spiritGuides[Id] = SpawnSpiritGuide(AVGraveyardLocationsSG[Id][0], AVGraveyardLocationsSG[Id][1], AVGraveyardLocationsSG[Id][2], AVGraveyardLocationsSG[Id][3], Team);
	 if(m_spiritGuides[Id]) 
	 {
	  AddSpiritGuide(m_spiritGuides[Id]);
	  // Debug, a virer
	 // SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE, 0, "Spirit-Guide Spawn: %s!",AVControlPointNames[Id]);
	 }
	// else // Debug, a virer
	  //SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE, 0, "Spirit-Guide Probleme: %s!",AVControlPointNames[Id]);
    }

	// Zarb, j'ai l'impression que les valeurs sont inversés!
	PlaySoundToAll(Team ? SOUND_HORDE_CAPTURE : SOUND_ALLIANCE_CAPTURE); 

	// Ruse pour corriger le texte entre ( " 'de le' fortin..." en " 'du' fortin..." )
	if(AVControlPointNames[Id][0]=='l' && AVControlPointNames[Id][1]=='e')
	 SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE, 0, "%s a pris le controle du %s!", Team ? "La Horde" : "L'Alliance", &AVControlPointNames[Id][2]);
	else
     SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE, 0, "%s a pris le controle de %s!", Team ? "La Horde" : "L'Alliance", AVControlPointNames[Id]);
 	DefFlag[Id][0] = false;
    DefFlag[Id][1] = false;

	sEventMgr.AddEvent(this, &AlteracValley::FireGuards, Id, Team, EVENT_AV_CONTROL_CP00 + Id, 60000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

    // respawn the control point with the correct aura
	SpawnControlPoint(Id, Team ? AV_SPAWN_TYPE_HORDE_CONTROLLED : AV_SPAWN_TYPE_ALLIANCE_CONTROLLED);
	SpawnControlTopTowers(Id, Team ? AV_SPAWN_TYPE_HORDE_CONTROLLED : AV_SPAWN_TYPE_ALLIANCE_CONTROLLED);
	// NEUTRAL ALLI-ATTACK H2-ATTACK ALLI-CONTROLLED H2_CONTROLLED
	// 180418,   178940,    179435,    178925,        178943 },
	if(Id == AV_CONTROL_POINT_CIM_DES_NEIGES) 
	{
		SpawnDrapCimDesNeiges(Team ? AV_SPAWN_TYPE_HORDE_CONTROLLED : AV_SPAWN_TYPE_ALLIANCE_CONTROLLED);
	}

     // update the map
	 if(Team) 
     { 
      if(AVWorldStates[Id][AV_SPAWN_TYPE_HORDE_CONTROLLED]!= 0)
	  {
		if(AVWorldStates[Id][AV_SPAWN_TYPE_HORDE_ASSAULT]!= 0)
	      SetWorldState(AVWorldStates[Id][AV_SPAWN_TYPE_HORDE_ASSAULT],0);
         SetWorldState(AVWorldStates[Id][AV_SPAWN_TYPE_HORDE_CONTROLLED],1);
        // SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE,0,
	 	//  "Worldstate %d (CONTROL) Ok!",Id);
	  }

	  //else SendChatMessage(CHAT_MSG_BG_EVENT_HORDE, 0,
	  //	  "Worldstate %d (CONTROLLED) est nulle!",Id);
     }
     else
     {
      if(AVWorldStates[Id][AV_SPAWN_TYPE_ALLIANCE_CONTROLLED]!=0)
	  {
		if(AVWorldStates[Id][AV_SPAWN_TYPE_ALLIANCE_ASSAULT]!= 0)
	      SetWorldState(AVWorldStates[Id][AV_SPAWN_TYPE_ALLIANCE_ASSAULT],0);
        SetWorldState(AVWorldStates[Id][AV_SPAWN_TYPE_ALLIANCE_CONTROLLED],1);
	   //SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE,0,
	   //	  "Worldstate %d (CONTROL) Ok!",Id);
	  }
	  //else SendChatMessage(CHAT_MSG_BG_EVENT_ALLIANCE, 0,
	  // 	  "Worldstate %d (CONTROLLED) est nulle!",Id);
      //A FAIRE: SetWorldState(Scores_ecran_player[Team], Renforts); 
     }
  //UpdatePvPData();
}
//-------------------------------------------------------------------------------------------------
// Appelé par HookSlowLockOpen(...)
//-------------------------------------------------------------------------------------------------
void AlteracValley::AssaultControlPoint(Player * pPlayer, uint32 Id)
{
bool SexeId=true; // accord de texte au masculin!
#ifdef ANTI_CHEAT
	if(!m_started)
	{
		SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, pPlayer->GetGUID(), "%s prefere quitter le jeux car il triche!", pPlayer->GetName());
		pPlayer->SoftDisconnect();
		return;
	}
#endif

	uint32 Team = pPlayer->m_bgTeam;
	uint32 Owner;

	if(m_DestroyedTower[Id]) return; // Si c'est une tour, elle peut-etre deja detruite!

	if(m_basesOwnedBy[Id] != -1) // Perte de la base par la faction
	{
	  Owner = m_basesOwnedBy[Id]; // memo de l'ancienne faction
      m_basesOwnedBy[Id] = -1;    // passage en base contesté/neutre

	 // Plus de rez possible a cet endroit si drap contesté
	 if(IsGraveyard[Id]) // Y'a t-il un cim a cet endroit ?
	 {
	  if( m_spiritGuides[Id] != NULL )
	  {
	   map<Creature*,set<uint32> >::iterator itr = m_resurrectMap.find(m_spiritGuides[Id]);
	   if( itr != m_resurrectMap.end() )
	   {
	    for( set<uint32>::iterator it2 = itr->second.begin(); it2 != itr->second.end(); ++it2 )
	    {
	     Player* r_plr = m_mapMgr->GetPlayer( *it2 );
		 if( r_plr != NULL && r_plr->isDead() ) 
		 {
			 HookHandleRepop( r_plr );
			 QueueAtNearestSpiritGuide(r_plr, m_spiritGuides[Id]);
		 }
	    }
		itr->second.clear();
	   }
	   //m_resurrectMap.erase( itr );
	   RemoveSpiritGuide(m_spiritGuides[Id]);
	   m_spiritGuides[Id]->Despawn( 0, 0 );
	   m_spiritGuides[Id] = NULL;
	   //	sChatHandler.BlueSystemMessage( pPlayer->GetSession(), "[%sSysteme%s] |rSpirit-Guide despawn: %s%u (%s).", 
	   //	MSG_COLOR_WHITE, MSG_COLOR_LIGHTBLUE, MSG_COLOR_SUBWHITE,Id,(Owner) ? "Horde" : "Alliance");

	  }                
     }
     
	// detract one from the teams controlled points
	 m_capturedBases[Owner] -= 1; // Decompte, perte de la base (devient contesté)
	 // Gestionnaire de l'event: modify the resource update time period
	 //if(m_capturedBases[Owner]==0) this->event_RemoveEvents(EVENT_AV_RESOURCES_UPDATE_TEAM_0+Owner);
	 //else this->event_ModifyTime(EVENT_AV_RESOURCES_UPDATE_TEAM_0 + Owner, ResourceUpdateIntervals[m_capturedBases[Owner]]);
	}

	// Prise du flag
	if(m_basesAssaultedBy[Id] != -1)
	{
	 Owner = m_basesAssaultedBy[Id]; // a qui etait le flag ?
     m_basesAssaultedBy[Id] = -1;
	 // On vire l'ancien event
	 	sEventMgr.RemoveEvents(this, EVENT_AV_CONTROL_CP00 + Id);
	}

	m_basesAssaultedBy[Id] = Team;
       
	// spawn du/des drap(s) contesté(s) ;)
	SpawnControlPoint(Id, Team ? AV_SPAWN_TYPE_HORDE_ASSAULT : AV_SPAWN_TYPE_ALLIANCE_ASSAULT);
	SpawnControlTopTowers(Id, Team ? AV_SPAWN_TYPE_HORDE_ASSAULT : AV_SPAWN_TYPE_ALLIANCE_ASSAULT);
	// NEUTRAL ALLI-ATTACK H2-ATTACK ALLI-CONTROLLED H2_CONTROLLED
	// 180418,   178940,    179435,    178925,        178943 },
	if(Id == AV_CONTROL_POINT_CIM_DES_NEIGES) 
		SpawnDrapCimDesNeiges(Team ? AV_SPAWN_TYPE_HORDE_ASSAULT : AV_SPAWN_TYPE_ALLIANCE_ASSAULT);

	PlaySoundToAll(Team ? SOUND_HORDE_CAPTURE : SOUND_ALLIANCE_CAPTURE);

    // Verif Assault/Def, le temps de spawn est different pour la capture ;) (Branruz)
    if(DefFlag[Id][0] && !DefFlag[Id][1] ) // Si le drap est neutre pas de defense mais prise
	{
	 DefFlag[Id][0] = false; // Flag a été defendu!! (Def)
     SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE, pPlayer->GetGUID(), "$N a defendu %s", AVControlPointNames[Id]);
	 // L'event est dans ce cas en prise direct (1sec)....(Blizzlike!!)
     sEventMgr.AddEvent(this, &AlteracValley::CaptureControlPoint, Id, Team, EVENT_AV_CONTROL_CP00 + Id, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	else
	{
     DefFlag[Id][0] = true; // En prise de Flag	(Assault) 
     switch(Id)
     {
      // Event Incendie
      case	AV_CONTROL_POINT_FORTIN_NORD_DUNBALDAR : // 2 Fortin Nord de DunBaldar 
      case	AV_CONTROL_POINT_FORTIN_SUD_DUNBALDAR  : // 3 Fortin Sud de DunBaldar 
      case	AV_CONTROL_POINT_AILE_DE_GLACE 	       : // 5 Fortin de l'Aile de Glace  
      case	AV_CONTROL_POINT_FORTIN_STONEHEARTH    : // 8 Fortin de StoneHearth (Commandant Randdolph ) 
      case	AV_CONTROL_POINT_TOUR_GLACESANG 	   : // 11 Tour GlaceSang 	
      case	AV_CONTROL_POINT_TOUR_DE_LA_HALTE 	   : // 13 Tour de la Halte ( Commandant Louis Philips + Drapeau )
      case	AV_CONTROL_POINT_TOUR_ORIENT_FROSTWOLF : // 15 Tour Orientale FrostWolf   
      case	AV_CONTROL_POINT_TOUR_OCCID_FROSTWOLF  : // 16 Tour Occidentale FrostWolf  
        if(AVControlPointNames[Id][1]=='a') SexeId=false;
        SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE, pPlayer->GetGUID(),
	 	  "%s est attaque%s! Si personne n'intervient, %s l%c detruira!", 
		  AVControlPointNames[Id],(SexeId) ? " ":"e ",Team ? "la Horde" : "l'Alliance",(SexeId) ? 'e':'a');
		sEventMgr.AddEvent(this, &AlteracValley::FireGuards, Id, Team, EVENT_AV_CONTROL_CP00 + Id, /*TIMER_PRISE_FLAG*/ 60000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		// marquage du worldstate
		if(AVWorldStates[Id][Team ? AV_SPAWN_TYPE_HORDE_ASSAULT : AV_SPAWN_TYPE_ALLIANCE_ASSAULT] != 0)
		{
	      SetWorldState(AVWorldStates[Id][Team ? AV_SPAWN_TYPE_HORDE_ASSAULT : AV_SPAWN_TYPE_ALLIANCE_ASSAULT],1);
		  //SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE,0,
	 	  //"Worldstate %d (ASSAULT) Ok!",Id);
		}
		//else SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE,0,
	 	//  "Worldstate %d ASSAULT est nulle!",Id);
		break;	  

       // Event des Draps, Prochain Event: le spawn des Gardes
      case	AV_CONTROL_POINT_POSTE_STORMPIKE       : // 1 Poste de Secours Stormpike
      case	AV_CONTROL_POINT_CIM_STORMPIKE 	       : // 4 Flag Cimetiere Stormpike 	
      case	AV_CONTROL_POINT_CIM_STONEHEARTH 	   : // 6 Flag Cimetiere StoneHearth 
      case	AV_CONTROL_POINT_CIM_DES_NEIGES 	   : // 9 Flag du cimetiere des neiges 
      case	AV_CONTROL_POINT_CIM_GLACESANG 	       : // 12 Flag Cimetiere GlaceSang
      case	AV_CONTROL_POINT_CIM_FROSTWOLF 	       : // 14 Flag Cimetiere FrostWolf	
      case	AV_CONTROL_POINT_HUTTE_FROSTWOLF       : // 18 Flag cimetiere Hutte FrostWolf
		 if(AVControlPointNames[Id][1]=='a') SexeId=false;
	     SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE, pPlayer->GetGUID(),
	 	   "%s est attaque%s! Si personne n'intervient, %s va s'en emparer!", 
		   AVControlPointNames[Id],(SexeId) ? " " :"e ",Team ? "la Horde" : "l'Alliance");
	     sEventMgr.AddEvent(this, &AlteracValley::CaptureControlPoint, Id, Team, EVENT_AV_CONTROL_CP00 + Id, /*TIMER_PRISE_FLAG*/ 60000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		 
		 if(AVWorldStates[Id][Team ? AV_SPAWN_TYPE_HORDE_ASSAULT : AV_SPAWN_TYPE_ALLIANCE_ASSAULT] != 0)
		 {
           if(AVWorldStates[Id][AV_SPAWN_TYPE_NEUTRAL] != 0)
             SetWorldState(AVWorldStates[Id][AV_SPAWN_TYPE_NEUTRAL],0);
	       SetWorldState(AVWorldStates[Id][Team ? AV_SPAWN_TYPE_HORDE_ASSAULT : AV_SPAWN_TYPE_ALLIANCE_ASSAULT],1);

		   // Debug
		   //SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE,0,
	 	   //"Worldstate %d (ASSAULT) Ok!",Id);
		 }
		 //else SendChatMessage(Team ? CHAT_MSG_BG_EVENT_HORDE : CHAT_MSG_BG_EVENT_ALLIANCE,0,
	 	 // "Worldstate %d (ASSAULT) est nulle!",Id);
    	break;

      // Ben la ya rien! ('fin pas encore)
      case	AV_CONTROL_POINT_DUNBALDAR             : // 0 Dun Baldar ( Vandar Foudrepique)
      case	AV_CONTROL_POINT_AV_POSTE_STONEHEARTH  : // 7 Avant poste de StoneHearth (Capitaine Belinda Gitepierre ) 
      case	AV_CONTROL_POINT_GARNISON_GLACESANG    : // 10 Garnison GlaceSang ( Capitaine Galvangar ) 
      case	AV_CONTROL_POINT_DONJON_FROSTWOLF 	   : // 17 DONJON_FROSTWOLF (Drek'Dar)   
      default: return;
     }
	}
}
//-------------------------------------------------------------------------------------------------
bool AlteracValley::HookSlowLockOpen(GameObject * pGo, Player * pPlayer, Spell * pSpell)
{
	if(pGo->bannerslot >= 0 && pGo->bannerslot < AV_NUM_CONTROL_POINTS)
	{
		// TODO: anticheat here
		AssaultControlPoint(pPlayer,pGo->bannerslot);
		//CBattleground::HookSlowLockOpen(pGo, pPlayer, pSpell);
		return true;
	}
    //CBattleground::HookSlowLockOpen(pGo, pPlayer, pSpell);
	return false;
}
//-------------------------------------------------------------------------------------------------
void AlteracValley::HookOnShadowSight()
{
}
//-------------------------------------------------------------------------------------------------
void AlteracValley::SetIsWeekend(bool isweekend) 
{
	m_isWeekend = isweekend;
}