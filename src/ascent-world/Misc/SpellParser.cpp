/* AscentWaad MMORPG Server
 * Copyright (C) 2010 WaadTeam <http://arbonne.games-rpg.net>
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
 */
#include <stdlib.h>
#include <stdio.h>
#include "StdAfx.h"
#include "SpellParser.h"
using namespace std;
//---------------------------------------------------------------------------
// Obligatoire pour avoir les montants correct lors des Heal ou Damage (Branruz)
// Un grand merci à PappyJo pour ses precieux conseils, programmeur hors pair en algorythmie et C++
//
#define MaxSize_spFormule	128
#define MaxSize_spDescript	500

int32 SpellParse::ParseDescription(uint32 id,uint32 IndexFormule,bool TypeColonne)
{
 char spDescript[MaxSize_spDescript],spFormule[MaxSize_spFormule];
 char *ptr,*ptr2;
 uint32 PosFormule = 0;
 double spBasePoint = 0;

 SpellEntry * sp;
 sp = dbcSpell.LookupEntry(id);

 if(!sp) // Impossible, le spell vient du player
 {
  Log.Error("SpellParse","ParseDescription: Id erreur : %u",id);
  Log.Error("          ","Verifier la version de connection Client!");
  return(0);
 }

 Flag_Parse = TypeColonne;
 if(TypeColonne == COL_DESCRIPTION) strncpy(spDescript,sp->Description,MaxSize_spDescript-1); 
 else                               strncpy(spDescript,sp->BuffDescription,MaxSize_spDescript-1); 

 ptr = &spDescript[0];

 if(!ptr)
 {
  //Log.Error("ParseDescription","Pas de formule trouvee.");
  return(0); // Pas de formule
 }

 while(*ptr!='\0')
 {

   switch(*ptr)
   {
    case '$' : // Debut d'une formule
		       PosFormule++;
		       memset(spFormule,0x00,MaxSize_spFormule);
               ptr2 = &spFormule[0];
               *ptr2++ = *ptr++;
               while( (*ptr!=' ') && (*ptr!='\0')) *(ptr2++) = *(ptr++); // Fin de la formule
			   if(PosFormule == IndexFormule) 
			   {
                sprintf(Formule,"%s",spFormule);
                if(spFormule[1] == '{')
                {
                 ptr2 = &Formule[2];
                 sprintf(spFormule,"%s",ptr2);
                 ptr2 = &spFormule[0];
                 while( *ptr2!='}') ptr2++;
                 *ptr2 = 0x00;
                }
                Log.Warning("ParseDescription","%s",spFormule);
			    spBasePoint = EvaluateFormule(sp,spFormule);
			   }
               ptr++;
               break;
    default : ptr++;
              break;
   }
   if(PosFormule == IndexFormule)
   {
	   return(spBasePoint);
   }
 }
 return(spBasePoint);
}
//---------------------------------------------------------------------------
#define spPARAM_SIZE	80
#define FORMULENUM_SIZE	256
int32 SpellParse::EvaluateFormule(SpellEntry *sp,char *pFormule)
{
 char spParam[spPARAM_SIZE],FormuleNum[FORMULENUM_SIZE];
 char *ptr,*ptr2,*ptr3;
 uint32 IndexParam = 0;
 int32 result_param =0;
 double spBasePoint = 0;

 ptr  = pFormule; // Memo
 memset(FormuleNum,0x00,FORMULENUM_SIZE);
 ptr3 = &FormuleNum[0];

 while(*ptr!='\0')
 {
  switch(*ptr)
  {
   case '$' : ptr++;
              if((*ptr=='/') || (*ptr=='*') || (*ptr=='+') || (*ptr=='-'))
              {
               result_param = ParseShortFormule(sp,ptr); // ShortFormule
               return(result_param);
              }
              else
              {
               memset(spParam,0x00,spPARAM_SIZE);
               ptr2 = &spParam[0];
               while((*ptr!='/') && (*ptr!='*') && (*ptr!='+') && (*ptr!='-') && (*ptr!='\0'))
               {
                *(ptr2++) = *(ptr++);
               }
               *ptr2 = 0x00;
               IndexParam++;
               result_param = ParamCalc(sp,spParam,IndexParam);
               if(result_param)
               {
                sprintf(&FormuleNum[strlen(FormuleNum)],"%d",result_param);
                ptr3 = &FormuleNum[strlen(FormuleNum)];
               }
               else
               {
                return(0); // Pas géré ou erreur
               }
              }
              break;
   default : if(*ptr != '\0') *(ptr3++) = *(ptr++);
             break;
  }

 } // End of while(*ptr!='\0')

 Log.Warning("EvaluateFormule","(%u) BasePoint Str : %s",sp->Id,FormuleNum);

 CExprParser *pParse = new CExprParser;
 if((spBasePoint = pParse->Parse(FormuleNum))==0)
 {
  Log.Error("EvaluateFormule","%s",pParse->ErrorString);
 }
 delete pParse;

 Log.Warning("EvaluateFormule","(%u) BasePoint Calc: %f",sp->Id,spBasePoint);
 
 return(float2int32(spBasePoint));
}
//---------------------------------------------------------------------------
int32 SpellParse::ParamCalc(SpellEntry *sp,char *pParam,int IndexParam)
{
 char buff_tmp[128];
 char *ptr;
 int32 spBasePoint=0;
 SpellEntry *sp_use;

 ptr = &pParam[0];

 SpellRelay = atoi(ptr);

 if(SpellRelay)
 {
  sprintf(buff_tmp,"%d",SpellRelay);
  ptr += strlen(buff_tmp);
  
  sp_use = dbcSpell.LookupEntry(SpellRelay);
  if(!sp_use)
  {
	  Log.Error("ParamCalc","Spell %u , le SpellRelay n'existe pas: %u",sp->Id,SpellRelay);
	  Log.Error("         ","Veuillez verifier la version de connection Client");
	  sp_use = sp; // Secu
  }

  Log.Warning("ParamCalc","SpellRelay: %d",SpellRelay);
 }
 else sp_use = sp;

 switch(*ptr)
 {
  case 'o' : // Suite formule dans BuffDescrition
             // ( le comptage des $o donne les pos des formules dans BuffDescrition )
             if(Flag_Parse == COL_DESCRIPTION) // Une seule recursivité
             {
			  spBasePoint = ParseDescription(sp_use->Id,1,COL_BUFFDESCRIPTION);
              return(spBasePoint);
             }
             // Pas de 'break', on parse la colonne 'BuffDescrition'

  // EffectBasePoint[]
  case 'm' : // Mana ou Mana min
  case 'M' : // Mana max
  case 's' : // Spell de dommages ou absorption de dommage
             ptr++;
             IndexBasePoint = (uint32) atoi(ptr);
             if((IndexBasePoint>=1) && (IndexBasePoint<=3))
             {
				// Note randdrick - Initialisation du tableau EffectBasePoints: de 0 à 2 ( et non de 1 à 3 !! ) -
  			    spBasePoint = sp_use->EffectBasePoints[IndexBasePoint-1]+1; // +1 OBLIGATOIRE pour les calculs
				Log.Warning("ParamCalc","Spell Use(%u), IndexBase %d, valeur: %d",
				                      sp_use->Id,IndexBasePoint,sp_use->EffectBasePoints[IndexBasePoint-1]);
				return(spBasePoint); 
             }
             else
             {
			  Log.Error("ParamCalc","(%u) IndexBase Error: %d",sp_use->Id,IndexBasePoint);
             }
             return(0);

  // DurationIndex
  case 't' : // Time, Temps de duree du spell
  case 'd' : // Duration, DurationIndex
             if(IndexParam > 1) // La durée fait partie du calcul
             {
              // Colonne DurationIndex
              SpellDuration* sd = dbcSpellDuration.LookupEntry(sp_use->durationIndex);
			  if(!sd)
			  {
				  Log.Error("ParamCalc","(%u) DurationIndex Erreur: %d",sp_use->Id,sp_use->durationIndex);
				  return(0);
			  }
			  int32 DurationTotal = (int32)((float)sd->Duration3/1000.);
			  if(DurationTotal) return(sd->Duration3);
			  else              return(1); // Secu
             }
             else return(0);

  case 'a' : // Area, Distance en Yards (Sort de zone)
  case 'r' : // Range, Distance en Yards (Sort de zone) (comme $a )
             return(0);

  // ProcChance
  case 'h' : return(0);

  // ProcCharge
  case 'n' : return(0);

  // EffectChainTarget[]
  case 'x' : return(0);

  case 'i' : // Enumération des PNJs/Players
             return(0);

  case '?' : // Conditionnel: equiv (toto)?titi:tata
             // avec 's' -> Test si le player possede le spell
             // avec 'a' -> Test si le player est dans la zone du spell
             // Exemple1: $?s12296[${$29131m1*$29131d/10+$m2}][$/10;29131o1] 
			 // Exemple2: $?(s25306|!((!a48165)|a66109))[...][...] 
             return(0);

  default : // Log.Warning("SpellParser","Param inconnu ou non géré: %s",ptr);
	        Log.Error("ParamCalc","(%u) Param inconnu ou non géré: %s",sp_use->Id,ptr);
            return(0);
 }
}
//---------------------------------------------------------------------------
int32 SpellParse::ParseShortFormule(SpellEntry *sp,char *pFormule)
{
 // Genre '/10;29131o1'
 char *ptr,*ptr2;
 char buff_tmp[128],buff_ope[128];
 int32 BasePointBD = 0;
 double cBasePointBD = 0;
 SpellEntry *sp_use;

 ptr = &pFormule[0];
 memset(buff_ope,0x00,128);
 ptr2 = &buff_ope[0];
 while( (*ptr != ';') && (*ptr != '\0')) { *(ptr2++) = *(ptr++); }

 if(*ptr != ';')
 {
  Log.Error("ParseShortFormule","(%d) Formule incomplete ou tronquee (%s) <--- Report this to devs.",pFormule);
  return(0);
 }
 ptr++;
 SpellRelay = atoi(ptr);
 if(SpellRelay) // SpellRelay
 {
  // Le basePoint est recup sur le SpellRelay
  // La colonne Description suffit => EffectBasePoint de l'index en cours
  sprintf(buff_tmp,"%d",SpellRelay);
  ptr += strlen(buff_tmp);
  
  sp_use = dbcSpell.LookupEntry(SpellRelay);
  if(!sp_use)
  {
	  Log.Error("ParamCalc","Spell %u , le SpellRelay n'existe pas: %u",sp->Id,SpellRelay);
	  Log.Error("         ","Veuillez verifier la version de connection Client");
	  sp_use = sp; // Secu
  }
  
  Log.Warning("ParseShortFormule","(%u) SpellRelay: %d",sp->Id,SpellRelay);
 }
 else sp_use = sp;

 switch(*ptr)
 {
  case 'o': // Buffdescription ou Descrition normale sur une ShortFormule
  case 'm' : //
  case 'M' :
  case 's' : ptr++;
             IndexBasePoint = (uint32)atoi(ptr);
             if((IndexBasePoint>=1) && (IndexBasePoint<=3))
             {
				 // Note randdrick - Initialisation du tableau EffectBasePoints: de 0 à 2 ( et non de 1 à 3 !! ) -
				 BasePointBD = sp_use->EffectBasePoints[IndexBasePoint-1]+1; // +1 OBLIGATOIRE pour le calcul

				 Log.Warning("ParseShortFormule","Spell (%u), IndexBase %d, valeur: %d",
				                      sp_use->Id,IndexBasePoint,sp_use->EffectBasePoints[IndexBasePoint-1]);

				 sprintf(buff_tmp,"%d%s",BasePointBD,buff_ope);
				 Log.Warning("ParseShortFormule","%s",buff_tmp);

             break;
             }
             else
             {
				 Log.Error("ParseShortFormule","Erreur IndexBasePoint (%d)",sp_use->Id,IndexBasePoint);
				 return(0);
			 }
  default :  // Le reste n'est pas géré
             return(0);
 }

 CExprParser *pParse = new CExprParser;
 if((cBasePointBD = pParse->Parse(buff_tmp)) == 0)
 {
  Log.Error("ParseShortFormule","%s",pParse->ErrorString);
 }
 delete pParse;

 // Repositionnement de la durée total sur BuffDescription
 /*
 SpellDuration* sd = dbcSpellDuration.LookupEntry(sp_use->durationIndex);
 if(!sd)
 {
	 Log.Error("ParseShortFormule","(%u) Duration non trouvee! Index: %u",sp_use->Id,sp_use->durationIndex);
 }
 else
 {
  int32 DureeTotal = (int32)((float)sd->Duration3/1000.); // Duration est en ms
  if(DureeTotal) cBasePointBD *= DureeTotal; // Secu
 }*/

 Log.Warning("ParseShortFormule","(%u) BasePoint Calc: %f",sp_use->Id,cBasePointBD);

 return(float2int32(cBasePointBD));
}
//---------------------------------------------------------------------------
double SpellParse::wStrToD(char *pFormule,int *NbDec)
{
 char *buff;
 char Nombre[80];
 int cmpt=0;
 int NbDecimal=0;
 bool gotcha = false;

 buff = pFormule;
 memset(Nombre,0x00,80);

 while(1)
 {
  if(((*buff < '0') || (*buff > '9')) && (*buff != '.')) break;
  if(gotcha) NbDecimal++;
  if(*buff == '.') gotcha = true;
  Nombre[cmpt] = *buff;
  buff++;
  cmpt++;
 }
 *NbDec = NbDecimal;
 return(atof(Nombre));
}
//---------------------------------------------------------------------------
// Ecrit par v71
// Algo trouvé sur http://www.devmaster.net/forums/showthread.php?t=14858
//---------------------------------------------------------------------------
bool CExprParser::EatSymbol( void )
{
 int Lenght=0;
 		
 memset( Identifier,0,sizeof(char)*MAXIDSIZE );

 if ( isalpha( *Look ) || *Look=='_' )
 {
  Token=TIdent;
  do
  {
   Identifier[ Lenght++ ]=*Look;
   if ( Lenght>=MAXIDSIZE)  return( ErrorParser( EIdentifierTooLong ));
   Look++;
  } while ( isalnum( *Look ) || *Look =='_' );
 }
 else
 {
  return false;
 }
 return true;
};
//---------------------------------------------------------------------------
bool CExprParser::GetNextToken( void )
{
 if ( *Look==NULL )
 {
  Token=TEnd; 
  return true; 
 }

 // Skip over white space.

 while ( isspace( *Look   ) ) Look++;

 switch ( *Look )
 {
  // check for delimiter

  case '+'  : *Look++ ; Token= TPlus;   break;
  case '-'  : *Look++ ; Token= TMinus;  break;
  case '*'  : *Look++ ; Token= TMul;    break;
  case '/'  : *Look++ ; Token= TDiv;	  break;
  case '='  : *Look++ ; Token= TAssign; break;
  case '('  : *Look++ ; Token= TLparen; break;
  case ')'  : *Look++ ; Token= TRparen; break;
  case '%'  : *Look++ ; Token= TMod;	  break;
  case '^'  : *Look++ ; Token= TExp;	  break;

  // check for numeric
  case '0'  :
  case '1'  :
  case '2'  :
  case '3'  :
  case '4'  :
  case '5'  :
  case '6'  :
  case '7'  :
  case '8'  :
  case '9'  : 
  case '.'  : /////////////////////////////////////////
			  // eat numeric value
              char *p;
			  Value=strtod( Look,&p );
			  Look=p;
			  Token=TValue;
			  break;

			// end of text

  case '\0' : Token=TEnd;	   break;
	
  // check for symbolic
  default   : if ( !EatSymbol() ) return ErrorParser( EUnknownSymbol );
              Token=TIdent ;
              break;
 }
 return true;
}
//---------------------------------------------------------------------------
bool CExprParser::ErrorParser( int err )
{
 switch ( err )
 {
  case ETextExpected                : strcpy(ErrorString,"Error: No Text");				    break;
  case EIncompleteExpression        : strcpy(ErrorString,"Error: Syntax error");			break;	
  case EUnknownSymbol               : strcpy(ErrorString,"Error: Unknown identifier");	    break;
  case EExpectedIdentifierOrNumeric : strcpy(ErrorString,"Expected Identifier or numeric"); break;
  case EExpectedIdentifier          : strcpy(ErrorString,"Expected Identifier");			break;
  case EExpcetedNumeric             : strcpy(ErrorString,"Expected numeric");				break;
  case EExpectedLParen              : strcpy(ErrorString,"Error: expected '('");			break;
  case EExpectedRParen              : strcpy(ErrorString,"Error: expected ')'");			break;
  case EExpectedParenBalance        : strcpy(ErrorString,"Error: Parenthesys unbalanced");  break;
  case EIdentifierTooLong	        : strcpy(ErrorString,"Error: Identifier too long");	    break;	
 };
 return false;
}
//---------------------------------------------------------------------------	
bool CExprParser::VerifyParenthesysBalance( void )
{
 int count=0;
 char *ptr;
		
 ptr=Look;

 do
 {
	if ( *ptr=='(' ) count++;
	else if ( *ptr==')' ) count--;
		
	ptr++;
 }
 while ( *ptr!=0 ) ;

 return ( count==0 ) ;
}
//---------------------------------------------------------------------------	
bool CExprParser::Factor ( double &c )
{
 if ( Token==TValue )
 {
  c=Value;
 }
 else if ( Token==TMinus )
 {
  if ( GetNextToken()==false ) return false;

  if ( Token==TValue ) c=-Value;
  else if ( Token==TLparen )
	   {
	    double b;
		
		if ( GetNextToken()==false ) return false;
				
		Expr( b  );

		c=-b;

		if ( Token!=TRparen && Token!=TEnd ) return ErrorParser( EExpectedRParen  );

	   }
	   else
	   {
	    return ErrorParser( EIncompleteExpression );
	   }
 }
 else if ( Token==TPlus )
 {
  if ( GetNextToken()==false ) return false;
		
  if ( Token==TValue ) c=Value;
  else if ( Token==TLparen )
  {
   double b;
  				
   if ( GetNextToken()==false ) return false;

   Expr( b  );

   c=b;

   if ( Token!=TRparen && Token!=TEnd ) return ErrorParser( EExpectedRParen  );

  }
  else
  {
   return ErrorParser( EIncompleteExpression );
  }
 }
 else if ( Token==TLparen )
 {
  double b;

  if ( GetNextToken()==false ) return false;
			
  Expr( b  );

  c=b;
			
  if ( Token!=TRparen && Token!=TEnd ) return ErrorParser( EExpectedRParen  );
 }
 else if ( Token==TRparen )
 {
  // () condition, no value returned
  c=0.0f;
  return true;
 }
 else if ( Token==TEnd )
 {
  return ErrorParser( EIncompleteExpression );
 }
 else 
 {
  return ErrorParser( EUnknownSymbol );
 }
 return true;
}
//---------------------------------------------------------------------------
bool CExprParser::Term ( double &c )
{
 double b;

 if ( Factor( c )==false ) return false;
 if ( GetNextToken()==false ) return false;

 while ( Token==TMul || Token==TDiv )
 {
  if ( Token==TMul )
  {
   if ( GetNextToken()==false ) return false;
   if ( Factor ( b )==false )   return false;

   c*=b;
  }
  else if ( Token==TDiv )
  {
   if ( GetNextToken()==false ) return false;
   if ( Factor ( b )==false )   return false;

   c/=b;
  }
  if ( GetNextToken()==false ) return false;
 }
 return true;
}
//---------------------------------------------------------------------------        
bool CExprParser::Expr( double &c )
{
 double b;

 if ( Term ( c )==false ) return false;

 while ( Token==TPlus || Token==TMinus )
 {
  if ( Token == TPlus )
  {
   if ( GetNextToken()==false ) return false;
   if ( Term( b )==false )      return false;
   c+=b;
  }
  else if ( Token == TMinus )
  {
   if ( GetNextToken()==false ) return false;
   if ( Term( b )==false )      return false;
   c-=b;
  }
			
  if ( Token!=TPlus && Token!=TMinus && 
	   Token!=TRparen && Token!=TEnd )
  {
   return ErrorParser( EIncompleteExpression );
  }		
 }
 return true;
}
//---------------------------------------------------------------------------
// Exemple: string text=" 2 + 3 - 2*( 3+2 )*4 ";
double CExprParser::Parse( char *text )
{
 if ( text==NULL ) return(ErrorParser( ETextExpected ));

 double result_tmp;
 Look=text;
 Token=TEnd;
 Value=0.0f;

 if ( VerifyParenthesysBalance()==false ) return( ErrorParser(EExpectedParenBalance));
	
 GetNextToken();

 Expr( result_tmp  );
 return(result_tmp);
}
//---------------------------------------------------------------------------
