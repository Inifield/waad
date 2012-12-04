/*
 * AscentWaad MMORPG Server
 * Copyright (C) 2010 Waad Team <http://arbonne.games-rpg.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Recherche de chemin des PNJs/Creatures
 * Recodé d'apres une doc de khayyam90 trouvé sur developpez.com
 * http://www.developpez.com/ - http://kakoukorp.free.fr/
 *
 * d'apres l'algo A* ( AStar )
 */

#ifndef _PATHFINDING_H
#define _PATHFINDING_H

enum PATHFINDING_GOAL_TYPE
{
	PATHFINDING_NO_GOAL	= 0,
	PATHFINDING_FOOD    , // 1
	PATHFINDING_MEET    , // 2
	PATHFINDING_FROM_TO , // 3
	PATHFINDING_TO_TOWN , // 4
	PATHFINDING_ATTACK  , // 5
	MAX_PATHFINDING_GOAL
};

enum PATHFINDING_PROGRESS_TYPE
{
	PATHFINDING_READY	= 0,
	PATHFINDING_PROGRESS, // 1
	PATHFINDING_END     , // 2
	MAX_PATHFINDING_PROGRESS_TYPE
};

class PathFinding
{

 public:
	PathFinding(Unit *_AI_Unit,uint32 Dest_MapId,uint32 Dest_ZoneId,uint32 Dest_AreaId,double Dest_X, double Dest_Y,uint32 _Goal);
	~PathFinding();

	//void PathFinding::chercher_chemin(Player *plr,float destX,float DestY)
	void chercher_chemin();
	uint32 Finding_State;

    Unit *GetAIUnit(void) { return(AI_Unit);}


private:
 

 struct noeud
 {
  float cout_g, cout_h, cout_f;
  std::pair<double,double> parent; // 'adresse' du parent (qui sera toujours dans la map fermée)
 };

 struct point
 {
  float x,y;
 };

 typedef map< pair<double,double>, noeud> l_noeud;
 l_noeud liste_ouverte;
 l_noeud liste_fermee;
 list<point> chemin;

 struct point arrivee;
 struct point intermediaire;
 noeud depart;
 Unit *AI_Unit;
 uint32 DestMapId;
 uint32 DestZoneId;
 uint32 DestAreaId;
 uint32 Goal;
 double Direction;     // Angle en radian de la direction à prendre au plus cours
 double DistanceTotal;

 //SDL_Surface *s = SDL_LoadBMP("carte.bmp");
 //SDL_SaveBMP(s, "resultat.bmp");
 double distance(double x1, double y1, double x2, double y2);
 bool deja_present_dans_liste(pair<double,double> n, l_noeud& l);
 void ajouter_cases_adjacentes(pair <double,double>& n);
 pair<double,double> meilleur_noeud(l_noeud& l);
 void ajouter_liste_fermee(pair<double,double>& p);
 void retrouver_chemin();
 void SavePath();

 // virtual double FindPerson(...) = 0;   // Recherche de quelqu'un, Renvoi une direction
 // virtual double ReadSignPost(...) = 0; // Lit les panneaux, Renvoi une direction
};

#endif		// _PATHFINDING_H
