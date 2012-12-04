  /*
 * AscentWaad MMORPG Wow Server
 * Copyright (C) 2011 Waad Team <http://arbonne.games-rpg.net>
 * Recherche sur le parler naturel du PNJ, Adapté sur Ascent par Branruz
 * Note: Ce code est Copyright par l'auteur mais plus mis a jour depuis 2006
 *       L'UTILISATION COMMERCIAL DE CE CODE EST INTERDITE
 *       Le fait de posseder un serveur qui echange du contenu lié au jeux par quelque forme 
 *       de payement que ce soit est passible de poursuite judiciare s'il utilise ce code.
//====================================================================================================
// Program Name: FSM Eliza
// Author: Gonzales Cenelia
// Website: http://ai-programming.com/
//
// This was one of my first atempt at writing an eliza like chatterbot, 
// this version of eliza uses an FSM (Finite State Machine) 
// for coding the main routine of the chatterbot, Finite State Machines are very well suitable
// for writing chatterbots in general. The main reason for this would be because that 
// most of the things that are found in the algorithm of chatterbots can be easealy coded by using
// predefine states of a given Finite State Machine.
// As you probably know it already, the first chatterbot was developped in the MIT laboratories
// during the year 1966 by professor Joseph Weizenbaum, the main purpose of chatterbots or chat robots
// is to simulate a conversation that ressemble a real conversation between humans.
// So far no chatterbot have really succeeded in this goal, although "Alice" a program that was
// written by Richard Wallace has won the Loebner Prize for most human chatbot three times.
// How can we explain the fact that chatterbots haven't succeeded yet in emulating a real conversation
// between huamns? Probably one the main reason for this would be simply because chatterbots in general 
// are unable to learn new keywords or even patterns by themselve during a conversation, if this specific
// feature were to ever been invented then it might solve one the biggest problems of chatbots.
// (a static database however big that it might be will never be sufficient to enable a chatbot 
// to have a coherent conversation)
//
// These code is Copyrighted and has limited warranty.
//====================================================================================================
*/

#ifndef _AI_SPEAK_H
#define _AI_SPEAK_H

#include "Tokenizer.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <stack>
#include <list>
#include <string>
#include <cstdio>
#include <ctime>

class Unit;
class Player;
class Creature;
class GameObject;
class Object;

using namespace std;

class AI_Speak
{
 public:
  AI_Speak(Unit *_unit);
  ~AI_Speak(); 

 void Listen(Player *_plr,string _User_texte);
 void Init(void);
 
 private: // ----- Procs et Fonctions ------

  typedef struct  // Table 'npc_poi', les endroits que connait le pnj en dehors de son environnement
  {
	std::string NameFr;
	float pox_x;
	float pox_y;
	float pox_z;
	uint32 Map_id;
    uint32 Area_id;
  } npc_poi_search;

  typedef struct  
  {
	std::string verbPhrase1;
	std::string verbPhrase2;
  } transpos_t;

  typedef struct  
  {
	vstring keyWords;
	vstring response;
  } key_t;

  enum STATE 
  { 
	AI_SPEAK_START, 
	AI_SPEAK_SIGN_ON, 
	AI_SPEAK_GET_INPUT, 
	AI_SPEAK_RESPOND, 
	AI_SPEAK_QUIT, 
	AI_SPEAK_NULL_INPUT, 
	AI_SPEAK_BOT_REPEAT, 
	AI_SPEAK_USER_REPEAT,
	AI_SPEAK_RECALL_MEMORY, 
	AI_SPEAK_SEARCH_KEY, 
	AI_SPEAK_KEY_FOUND, 
	AI_SPEAK_NO_KEY_FOUND, 
	AI_SPEAK_DEFAULT_REPLY, 
	AI_SPEAK_CHANGE_TOPIC, 
	AI_SPEAK_DONT_UNDERSTAND, 
	AI_SPEAK_LEARNING, 
	AI_SPEAK_ASK_QUESTION, 
	AI_SPEAK_VERIFY_RESPONSE, 
	AI_SPEAK_MEMORISING, 
	AI_SPEAK_WRONG_CONTEXT, 
	AI_SPEAK_WRONG_SUBJECT, 
	AI_SPEAK_SELECT_RESPONSE,
	AI_SPEAK_MAX_STATE
  } state;

  enum AI_SPEAK_ACTION 
  { 
	AI_SPEAK_NULL_ACTION=0,
	AI_SPEAK_SPELL_ID,
	AI_SPEAK_AOE, 
	AI_SPEAK_GOB_NAME,
	AI_SPEAK_SELECTION,
	AI_SPEAK_ACTION_START, 
	AI_SPEAK_ACTION_SUIVRE, 
	AI_SPEAK_ACTION_RESTE, 
	AI_SPEAK_GIVE_ITEM,
	AI_SPEAK_ACTION_SE_DEPLACE,
	AI_SPEAK_MAX_ACTION
  } action;

  void memorise(void);
  void dump_data(void);
  void saveData(void);
  void saveSignOn(void);
  void saveTransposTable(void);
  void saveNullResponse(void);
  void saveNoKeyWord(void);
  void saveTopicChanger(void);
  void saveSubjectRecall(void);
  void saveKeyWordList(void);
  int Random( const int ival );
  void extractSubject();
  void getInput( std::string &str );
  bool testRepetition(void);
  bool parseScriptFile(void);
  void Transition(void);
  void PrintResponse(void);
  void PrintData(void);
  void trimPuncs( std::string &str );
  void UpperCase( std::string &str );
  int searchKeyWord(std::string str, std::vector<key_t> keyList);
  bool transpose( std::string &str );
  bool replace( std::string &str, std::string oldsubstr, std::string newsubstr );
  bool remove( std::string &str, std::string substr );
  void transpose_sentence( std::string &str );
  bool print_keyword_list(void);
  void print_response_list(void);
  void saveUnknownSentences(void);
  void trimLeft(std::string &str, char c);
  void trimRight(std::string &str, char c);
  void trimLR(std::string &str, std::string characters);
  void select_response(void);
  void handleRepetition(void);
  void handleWrongContext(void);
  void handleWrongSubject(void);

  // Interaction avec le player
  bool IsActionOrMove(void);
  bool ReplaceResponseInContextAction(void);

 private: // ----- Variables ------
  typedef std::vector<std::string> vstring;

  Unit *m_UnitOwner;
 // std::list<Player *,bool > m_PlayerKnown; // _NAME_ : true=Nom; false=Class
  Player *m_CurrentPlayer;
  bool m_PlayerNameknown;
  std::string m_CurrentPlayerNameStr;
  Object *m_AISpeakTarget; 
  bool m_StandByForTarget;
  std::vector<npc_poi_search> npc_SamePoiListKnown; 

  std::string keyWord;
  std::string sInput;
  std::string sPreviousInput;
  std::string sResponse;
  std::string sSubject;
  std::string sPreviousResponse;

  std::string puncs ;
  std::stack<std::string> memory;
  int32 nRepeatCount; // Static

  vstring signOn;
  vstring nullResponse;
  vstring noKeyWord;
  vstring topicChanger;
  vstring subjectRecall;
  vstring unknownSentences;
  vstring responseList;
  std::list<std::string> extendedResponseList;
  std::vector<transpos_t> transposList; 
  transpos_t thisTranspos;

  key_t thisKey, _thisKey;
  std::vector<key_t> keyWordList;
  std::vector<key_t> keyForLearning;
  std::vector<int32> pos_list;

 std::string NEXT_SYMBOL;
 FILE *scriptfile; // std::fstream scriptfile;
 Tokenizer tok;
 bool bNewData;
 int32 nFoundPos;

 uint32 nSignOnSize;
 uint32 nNullSize;
 uint32 nNoKeySize;
 uint32 nTopicChangerSize;
 uint32 nSubjectRecallSize;
 uint32 nKeyWordsSize;
 uint32 nTransPosNum;
 uint32 nNokeyFound;
 uint32 nKeyForLearning;
 uint32 nKeyPos;
 uint32 nSelection;
 uint32 nKeyNum;
};
//---------------------------------------------------------------------------
#endif

