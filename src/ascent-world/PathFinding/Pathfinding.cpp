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
#include "StdAfx.h"

#define _PATHFINDING_DEBUG_
//----------------------------------------------------------------------------
PathFinding::PathFinding(Unit *_AI_Unit,uint32 Dest_MapId,uint32 Dest_ZoneId,uint32 Dest_AreaId,double Dest_X, double Dest_Y,uint32 _Goal)
{
 AI_Unit = _AI_Unit;

 depart.parent.first  = AI_Unit->GetPositionX();
 depart.parent.second = AI_Unit->GetPositionY();

 intermediaire.x = 0;
 intermediaire.y = 0;

 arrivee.x = Dest_X;
 arrivee.y = Dest_Y;

 DestMapId  = Dest_MapId;
 DestZoneId = Dest_ZoneId;
 DestAreaId = Dest_AreaId;
 
 Goal = _Goal;

 DistanceTotal = distance(depart.parent.first,depart.parent.second,arrivee.x,arrivee.y);
 Direction     =  0; // A CALCULER arcSin( (arrivee.y - depart.parent.second)/DistanceTotal);

 Finding_State = PATHFINDING_READY;
}
//----------------------------------------------------------------------------
PathFinding::~PathFinding()
{
#ifdef _PATHFINDING_DEBUG_
 Log.Success("PathFinding","Destructor");
#endif

 AI_Unit->CurrentPathFinding = NULL;
}
//----------------------------------------------------------------------------
// calcule la distance entre les points (x1,y1) et (x2,y2) 
double PathFinding::distance(double x1, double y1, double x2, double y2)
{
 // distance euclidienne 
 return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
 // carré de la distance euclidienne 
 // return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2); 
}
//----------------------------------------------------------------------------
bool PathFinding::deja_present_dans_liste(pair<double,double> n, l_noeud& l)
{
 l_noeud::iterator i = l.find(n);
 if (i==l.end()) return false;
 else            return true;
}
//----------------------------------------------------------------------------
#define _2PI_		(M_PI*2)
#define DISTANCE_DE_VUE	55 // 55yards
#define ANGLE_DE_VUE	(double)5 // 5° Gauche et droite
#define ANGLE_DE_VUE_RAD	(double)((M_PI*ANGLE_DE_VUE)/180)
enum ROSACE
{
 NORD_EST	= 1,
 SUD_EST	= 2,
 SUD_OUEST	= 3,
 NORD_OUEST	= 4,
 MAX_ROSACE
};
void PathFinding::ajouter_cases_adjacentes(pair <double,double>& n)
{
 bool result_LOS;
 double xDest,yDest;
 noeud tmp;
 // obstace, terrain non franchissable, on oublie 
 // On lance un rayon autour du pnj, 
 // Le PNJ regarde a droite et a gauche (5° pour l'instant...)
 // On doit recalculer x y z en fonction de l'orientation (en radian)
 // float SourceTaille = GetHeight() + AI_Unit->GetSize();
 // float TargetTaille = GetHeight() + AI_Unit->GetSize(); Le pnj regarde devant lui

 double Orientation = (Direction*180) / M_PI;

 double X_FACE   = AI_Unit->GetPositionX() + ( DISTANCE_DE_VUE*cos((M_PI/2) - Direction));
 double X_GAUCHE = AI_Unit->GetPositionX() + ( DISTANCE_DE_VUE*cos((M_PI/2) - (Direction + ANGLE_DE_VUE_RAD)));
 double X_DROITE = AI_Unit->GetPositionX() + ( DISTANCE_DE_VUE*cos((M_PI/2) - (Direction - ANGLE_DE_VUE_RAD)));
 double Y_FACE   = AI_Unit->GetPositionY() + ( DISTANCE_DE_VUE*sin((M_PI/2) - Direction));
 double Y_GAUCHE = AI_Unit->GetPositionY() + ( DISTANCE_DE_VUE*sin((M_PI/2) - (Direction + ANGLE_DE_VUE_RAD)));
 double Y_DROITE = AI_Unit->GetPositionY() + ( DISTANCE_DE_VUE*sin((M_PI/2) - (Direction - ANGLE_DE_VUE_RAD)));
  
 // Recherche les constantes de la droite y = ax + b (on se limite à la corde de l'arc de cercle)
 double a = (Y_GAUCHE-Y_DROITE)/(X_GAUCHE-X_DROITE);
 double b = Y_GAUCHE - (a*X_GAUCHE); // ou b = Y_DROITE - (a*X_DROITE);
 
 // ( +/-5° donne une variation de ANGLE_DE_VUE*55 = 550 yards) 
 // Du Nord-Ouest vers le Nord-Est
 //for (float i=n.first-(X_OUEST-X_FACE); i<=n.first+(X_EST-X_FACE); i++) 
 xDest = X_FACE;
 yDest = Y_FACE;
 for(uint32 i=0;i < (ANGLE_DE_VUE_RAD*DISTANCE_DE_VUE);i++) // en suivant la droite
 {             
  
  if(i == 0) // En face
  {
   result_LOS = CollideInterface.CheckLOS(AI_Unit->GetMapId(), AI_Unit->GetPositionX(), AI_Unit->GetPositionY(), 
                       AI_Unit->GetPositionZ()+AI_Unit->GetSize(), xDest, yDest, AI_Unit->GetPositionZ()+AI_Unit->GetSize());
   if(!result_LOS) continue; // CheckLOS a echoué, on ne peut pas passer par la
   
  }
  else // Gauche et Droite
  {
   if(i % 2) // Impair ( a droite )
   {
	xDest = xDest + i;
    yDest = (a*xDest) + b;
   }
   else // Pair ( a gauche )
   {
	xDest = xDest - i;
    yDest = (a*xDest) + b;
   } 

   if((xDest >= _maxX) || (xDest <= _minX)) continue; // _maxX,_minX : largeur de la map 
   if((yDest >= _maxY) || (yDest <= _minY)) continue; // _maxY,_minY : largeur de la map 

   if ((xDest==n.first) && (yDest==n.second)) continue; // coord actuelle n, on oublie 
  
   result_LOS = CollideInterface.CheckLOS(AI_Unit->GetMapId(), AI_Unit->GetPositionX(), AI_Unit->GetPositionY(), 
                       AI_Unit->GetPositionZ()+AI_Unit->GetSize(), xDest, yDest, AI_Unit->GetPositionZ()+AI_Unit->GetSize());

   if(!result_LOS) continue; // CheckLOS a echoué, on ne peut pas passer par la
  }
    
  pair<double,double> it(xDest,yDest); // Ok on passe

   if (!deja_present_dans_liste(it, liste_fermee))
   {
    // le noeud n'est pas déjà présent dans la liste fermée 
    // calcul du cout G du noeud en cours d'étude : cout du parent + distance jusqu'au parent 
    tmp.cout_g = liste_fermee[n].cout_g + distance(xDest,yDest,n.first,n.second);
    // calcul du cout H du noeud à la destination 
    tmp.cout_h = distance(xDest,yDest,arrivee.x,arrivee.y);
    tmp.cout_f = tmp.cout_g + tmp.cout_h;
    tmp.parent = n;
    if (deja_present_dans_liste(it, liste_ouverte))
	{
     // le noeud est déjà présent dans la liste ouverte, il faut comparer les couts
     if (tmp.cout_f < liste_ouverte[it].cout_f)
	 {
      // si le nouveau chemin est meilleur, on met à jour 
      liste_ouverte[it]=tmp;
     }
      // le noeud courant a un moins bon chemin, on ne change rien 
    }
	else // le noeud n'est pas présent dans la liste ouverte, on l'y ajoute 
	{
     liste_ouverte[pair<double,double>(xDest,yDest)]=tmp;
	}
   } // End of if(!deja_present_dans_liste...

 }
 
}
//----------------------------------------------------------------------------
pair<double,double> PathFinding::meilleur_noeud(l_noeud& l)
{
 double m_coutf = l.begin()->second.cout_f;
 pair<double,double> m_noeud = l.begin()->first;
 for (l_noeud::iterator i = l.begin(); i != l.end(); i++)
 {
  if (i->second.cout_f < m_coutf)
  {
   m_coutf = i->second.cout_f;
   m_noeud = i->first;
  }
 }
 return m_noeud;
}
//----------------------------------------------------------------------------
void PathFinding::ajouter_liste_fermee(pair<double,double>& p)
{
 noeud& n = liste_ouverte[p];
 liste_fermee[p] = n;
 // il faut le supprimer de la liste ouverte, ce n'est plus une solution explorable 
 if (liste_ouverte.erase(p) == 0)
	 Log.Error("PathFinding","Erreur, le noeud n'est pas dans la liste ouverte, impossible à supprimer");

 return;
}
//----------------------------------------------------------------------------
void PathFinding::retrouver_chemin()
{
 // l'arrivée est le dernier élément de la liste fermée 
 noeud& tmp = liste_fermee[std::pair<double, double>(arrivee.x,arrivee.y)];
 struct point n;
 pair<double,double> prec;
 n.x = arrivee.x;
 n.y = arrivee.y;
 prec.first = tmp.parent.first;
 prec.second = tmp.parent.second;
 chemin.push_front(n);
 while (prec != pair<double,double>(depart.parent.first,depart.parent.first))
 {
  n.x = prec.first;
  n.y = prec.second;
  chemin.push_front(n);
  tmp = liste_fermee[tmp.parent];
  prec.first = tmp.parent.first;
  prec.second = tmp.parent.second;
 }
}
//----------------------------------------------------------------------------
void PathFinding::SavePath()
{
    list<point>::iterator j;
/*
    for (j=chemin.begin(); j!=chemin.end(); j++)
    {
      QueryResult * result = WorldDatabase.Query( "UPDATE ai_pathfinding SET spawn_id creature_entry index ordering mapid areaid zoneid x y z comment" );
    }
*/
#ifdef _PATHFINDING_DEBUG_
	Log.Success("PathFinding","Chemin final a %u waypoints",chemin.size());
    Log.Success("PathFinding","Derniere liste ouverte : %u elements",liste_ouverte.size());
    Log.Success("PathFinding","Derniere liste fermee  : %u elements",liste_fermee.size());
#endif
}
//----------------------------------------------------------------------------
void PathFinding::chercher_chemin()
{
 pair <double,double> courant;
 // déroulement de l'algo A* 
 // initialisation du noeud courant 
 courant.first = 0;
 courant.second = 0;
 // ajout de courant dans la liste ouverte 
 liste_ouverte[courant] = depart;
 ajouter_liste_fermee(courant);
 ajouter_cases_adjacentes(courant);
 // tant que la destination n'a pas été atteinte et 
 // qu'il reste des noeuds à explorer dans la liste ouverte 
 while( !((courant.first == arrivee.x) && (courant.second == arrivee.y))
             && (!liste_ouverte.empty()) )
 {
  // on cherche le meilleur noeud de la liste ouverte, 
  // on sait qu'elle n'est pas vide donc il existe 
  courant = meilleur_noeud(liste_ouverte);
  // on le passe dans la liste fermee, il ne peut pas déjà y être 
  ajouter_liste_fermee(courant);
  // on recommence la recherche des noeuds adjacents 
  ajouter_cases_adjacentes(courant);
 }
 // si la destination est atteinte, on remonte le chemin 
 if ((courant.first == arrivee.x) && (courant.second == arrivee.y))
 {
   if(this->GetAIUnit()->IsUnit())
  {
    this->GetAIUnit()->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Bon, j'y vais!");
	this->GetAIUnit()->Emote(EMOTE_ONESHOT_EXCLAMATION);
  }
  retrouver_chemin();
  SavePath();
 }
 else
 {
  // pas de solution 
  if(this->GetAIUnit()->IsUnit())
  {
    this->GetAIUnit()->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Je ne sais pas par ou aller!");
	this->GetAIUnit()->Emote(EMOTE_ONESHOT_EXCLAMATION);
  }
 }
}
//----------------------------------------------------------------------------