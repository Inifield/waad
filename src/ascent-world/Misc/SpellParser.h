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

#ifndef _SPELL_PARSER_H
#define _SPELL_PARSER_H
//---------------------------------------------------------------------------
// Backstab the target, causing $m2% weapon damage plus ${$m1*1.5} to the target.
// Must be behind the target.  Requires a dagger in the main hand.  Awards $s3 combo $lpoint:points;.

// A Voir spell 53251

// A defensive combat stance.  Decreases damage taken by $7376s1% and damage caused by $7376s2%.  Increases threat generated.
// Trips the enemy humanoid, causing $s2 damage and knocking the opponent down for $d.  Awards $s3 combo point.

// Garrote the enemy, causing ${($m1+$AP*0.07)*6} damage over $d, increased by your attack power.  Must be stealthed and behind the target.  Awards $s2 combo $lpoint:points;.
// BuffDescript: $s1 damage every $t1 seconds.

// Armure de Givre: 168
// Description:
//               Increases Armor by $s1.  If an enemy strikes the caster, they may have their movement slowed by $6136s1% and
//               the time between their attacks increased by $6136s2% for $6136d.  Only one type of Armor spell can be active
//               on the Mage at any time.  Lasts $d.
//
// BuffDescription:  Increases Armor by $s1 and may slow attackers.
/*
Armure de givre	Rang 1
24% du mana de base
Incantation immédiate
Augmente l'Armure de 30. Si un ennemi frappe le lanceur de sorts,
il peut subir les effets suivants : vitesse de déplacement réduite
de 30% et temps entre ses attaques augmenté de 25% pendant 5 sec.
Vous ne pouvez bénéficier que d’un seul type de sort d’armure à la fois. Dure 30 min.
*/



// Spell 66
// $?s54354[Instantly makes the caster invisible, reducing all threat.]
// [Fades the caster to invisibility over $66d, reducing threat each second.]
// The effect is cancelled if you perform any actions.  While invisible, you can
// only see other invisible targets and those who can see invisible.  Lasts $32612d.

// Garrote the enemy, causing ${($m1+$AP*0.07)*6} damage over $d, increased by your attack power.  Must be stealthed and behind the target.  Awards $s2 combo $lpoint:points;.

enum TypeColonneDesc
{
 COL_DESCRIPTION,
 COL_BUFFDESCRIPTION,
};

#define		MAXIDSIZE		256

enum EToken
{
	TEnd,
	TValue,
	TPlus,
	TMinus,
	TMul,
	TDiv,
	TMod,
	TExp,
	TLparen,
	TRparen,
	TAssign,
	TIdent
};

enum EError
{
	ETextExpected,
	EUnknownSymbol,
	EIncompleteExpression,
	EExpectedIdentifierOrNumeric,
	EExpectedParenBalance,
	EExpectedIdentifier,
	EExpcetedNumeric,
	EExpectedLParen,
	EExpectedRParen,
	EIdentifierTooLong
};

#define SIZE_FORMULE 512

class SpellParse
{
 public:
  SpellParse()
  {
   SpellRelay     = 0;
   IndexBasePoint = 0;
   memset(Formule,0x00,SIZE_FORMULE);
  };
 
  ~SpellParse() { };

 int32 ParseDescription(uint32 id,uint32 IndexFormule,bool TypeColonne);
 
 protected:
  uint32 Flag_Parse;
  uint32 SpellRelay;
  uint32 IndexBasePoint;
  char Formule[SIZE_FORMULE];

  int32 EvaluateFormule(SpellEntry *sp,char *pFormule);
  int32  ParamCalc(SpellEntry *sp,char *pParam,int IndexParam);
  int32 ParseShortFormule(SpellEntry *sp,char *pFormule);
  double wStrToD(char *pFormule,int *NbDec);
};

// Algo trouvé sur http://www.devmaster.net/forums/showthread.php?t=14858
// Ecrit par v71
class CExprParser 
{
 public:
  CExprParser()
	{
		Value=0.0f;
		Look=NULL;	
	};

   ~CExprParser() { };

  double Parse( char *text );
  char ErrorString[80];

 protected:
  bool ErrorParser( int err );
  bool EatSymbol( void );
  bool GetNextToken( void );
  bool VerifyParenthesysBalance( void );
  bool Factor ( double &c );
  bool Term ( double &c );
  bool Expr( double &c );

  char	   *Look;
  double   Value;
  EToken   Token;
  char	   Identifier[MAXIDSIZE];
};
//---------------------------------------------------------------------------
#endif