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
* Note: Le moteur d'etat avance au fur et a mesure de la discution et n'est pas géré par timer
*       cependant, il avance jusqu'a ce que la reponse soit trouvée.
*/

#pragma warning(disable: 4786)

#include "StdAfx.h"

class Unit;
class Player;
class Creature;
class GameObject;
class Object;
//----------------------------------------------------------------------------------------
AI_Speak::AI_Speak(Unit *_unit)
{
 nRepeatCount = 1;
 NEXT_SYMBOL = "";
 bNewData = false;
 puncs = ".,;!?";
 keyWord = "";
 sInput = "";
 sPreviousInput = "";
 sResponse = "";
 sSubject = "";
 sPreviousResponse = "";
 
 // AscentWaad
 m_UnitOwner = _unit;
 m_CurrentPlayer = NULL;
 m_PlayerNameknown = false;
 m_CurrentPlayerNameStr = "Inconnu";
 
 m_AISpeakTarget = NULL; 
 m_StandByForTarget = false;
 //-------------------------
 Init();
 
}
//----------------------------------------------------------------------------------------
AI_Speak::~AI_Speak()
{
 if(bNewData) 
 {
	saveData();
 }

 // Vidange du vocabulaire
 if(extendedResponseList.size()) extendedResponseList.clear();
 if(signOn.size())               signOn.clear();
 if(nullResponse.size())         nullResponse.clear();
 if(noKeyWord.size())            noKeyWord.clear();
 if(topicChanger.size())         topicChanger.clear();
 if(subjectRecall.size())        subjectRecall.clear();
 if(keyForLearning.size())       keyForLearning.clear();
 if(keyWordList.size())          keyWordList.clear();
 if(transposList.size())         transposList.clear();

}
//----------------------------------------------------------------------------------------
void AI_Speak::Init(void)
{
	//std::cout << "Eliza 2006: Smarter Eliza,Copyright(c) Gonzales Cenelia.\n" << std::endl;
    //m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Je suis maintenant doué de la parole.");

	parseScriptFile();
	nNokeyFound = 0;
	nSignOnSize = signOn.size();               // Sxxxxxx    - DEFINE SIGNON MESSAGE
	nNullSize = nullResponse.size();           // Nxxxxxx    - RESPONSE FOR NULL ENTRY
	nNoKeySize = noKeyWord.size();             // Xxxxxxx    - DEFINE RESPONSE FOR NO KEYWORD FOUND
	nTopicChangerSize = topicChanger.size();   // Zxxxxxx    - DEFINE CONVERSATION TOPICS
	nSubjectRecallSize = subjectRecall.size();
	nKeyForLearning = keyForLearning.size();
	nKeyWordsSize = keyWordList.size();
	nTransPosNum = transposList.size();
	//srand( ( unsigned )time( NULL ) );

	state = AI_SPEAK_START;
	
}
//----------------------------------------------------------------------------------------
void AI_Speak::Listen(Player *_plr,string _User_texte)
{
 //char _buff_temp[255];
 //sprintf(_buff_temp,"Je suis la %s...",_plr->GetName());
 //m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,_buff_temp);
 if(m_CurrentPlayer)
 {
	 if(m_PlayerNameknown) m_CurrentPlayerNameStr = m_CurrentPlayer->GetName();
     else 		           m_CurrentPlayerNameStr = m_CurrentPlayer->GetClassPlayer();
 }
 else
 {
  m_CurrentPlayer = _plr;
  m_CurrentPlayer->SetClassString();
  m_CurrentPlayerNameStr = m_CurrentPlayer->GetClassPlayer();
  m_PlayerNameknown = false;
  state = AI_SPEAK_START;
 }

 sInput = _User_texte;

 // Le systeme est un moteur d'etat (Transition())
 // il fonctionne nickel si et seulement si State revient au final à AI_SPEAK_GET_INPUT
 // Bien faire attention à l'event sinon overload serveur.....
 do
 {
   Transition();
 } while((state != AI_SPEAK_GET_INPUT) ); 

}
//----------------------------------------------------------------------------------------
void AI_Speak::Transition(void) 
{
	switch(state)
	{
	  case AI_SPEAK_START: //-------------------------------
		state = AI_SPEAK_SIGN_ON; 
		break;

	  case AI_SPEAK_SIGN_ON: //-------------------------------
		responseList = signOn;
		state = AI_SPEAK_SELECT_RESPONSE;
		break;

	  case AI_SPEAK_GET_INPUT: //-------------------------------
		sPreviousInput = sInput;
		getInput(sInput);
		NEXT_SYMBOL = sInput;
		if(NEXT_SYMBOL.length() == 0) 
		{
			state = AI_SPEAK_NULL_INPUT;
		} 
		else if(NEXT_SYMBOL == " QUIT " || NEXT_SYMBOL == " BYE ") 
		{
			
			state = AI_SPEAK_RESPOND;
		} 
		else if(testRepetition() == 1) 
		{
			if(nRepeatCount  < 2) 
			{
				state = AI_SPEAK_SEARCH_KEY;
			} 
			else 
			{
				state = AI_SPEAK_USER_REPEAT;
			}
		} 
		/*else if(NEXT_SYMBOL.find("MON ") != std::string::npos) // MA
		{
			keyWord = "MON "; // MA
			extractSubject();
			memory.push(sInput);
			nRepeatCount = 0;
			state = AI_SPEAK_SEARCH_KEY;
		} */
		else // Recherche normal
		{
			nRepeatCount = 0;
			state = AI_SPEAK_SEARCH_KEY;
		}
		break;

	 case AI_SPEAK_SEARCH_KEY: //-------------------------------
		{
			nFoundPos = searchKeyWord(NEXT_SYMBOL, keyWordList);
			if(nFoundPos == -1 && pos_list.size() > 0) 
			{
				int index = rand() % pos_list.size();
				nFoundPos = pos_list[index];
			}
			if(nFoundPos == -1) 
			{
				unknownSentences.push_back(NEXT_SYMBOL);
				state = AI_SPEAK_NO_KEY_FOUND;
			} 
			else 
			{
				state = AI_SPEAK_KEY_FOUND;
			}
		}
		break;

	 case AI_SPEAK_RESPOND: //-------------------------------
		
		if(IsActionOrMove())
		{
			if(!ReplaceResponseInContextAction()) PrintResponse();
			state = AI_SPEAK_GET_INPUT;
		}
		else if(NEXT_SYMBOL == " QUIT " || NEXT_SYMBOL == " BYE ") 
		{
			PrintResponse();
			saveUnknownSentences();
			state = AI_SPEAK_QUIT;
		} 
		else 
		{
			size_t found_tag = sResponse.find("_MY_NAME_"); // En premier
            if (found_tag!=string::npos) 
		    {
			   sResponse.erase(found_tag,9); // 9 = strlen("_MY_NAME_")
			   sResponse.insert(found_tag,((Unit *)m_UnitOwner)->GetObjectName());
			}

			found_tag = sResponse.find("_NAME_"); // Tjs en dernier ;)
            if (found_tag!=string::npos) 
		    {
			   sResponse.erase(found_tag,6); // 6 = strlen("_NAME_")
			   sResponse.insert(found_tag,m_CurrentPlayerNameStr);
			}

			
			PrintResponse();
			extendedResponseList.push_back(sResponse);
			state = AI_SPEAK_GET_INPUT;
		}
		break;

	 case AI_SPEAK_NULL_INPUT: //-------------------------------
		responseList = nullResponse;
		state = AI_SPEAK_SELECT_RESPONSE;
		break;

	 case AI_SPEAK_BOT_REPEAT: //-------------------------------
		handleRepetition();
		state = AI_SPEAK_RESPOND;
		break;

	 case AI_SPEAK_USER_REPEAT: //-------------------------------
		state = AI_SPEAK_RESPOND;
		break;

	 case AI_SPEAK_KEY_FOUND: //-------------------------------
		responseList = keyWordList[nFoundPos].response;
		state = AI_SPEAK_SELECT_RESPONSE;
		break;

	 case AI_SPEAK_NO_KEY_FOUND: //-------------------------------
		if(memory.size() == 0) 
		{
			++nNokeyFound;
			if(rand() % 3) 
			{
				state = AI_SPEAK_DEFAULT_REPLY;
			} 
			else 
			{
				state = AI_SPEAK_CHANGE_TOPIC;
			}
		} 
		else 
		{
			state = AI_SPEAK_RECALL_MEMORY;
		}
		break;

	 case AI_SPEAK_SELECT_RESPONSE: //-------------------------------
		sPreviousResponse = sResponse;
		nKeyNum = responseList.size();
		nSelection = Random(nKeyNum);
		sResponse = responseList[nSelection];
		if(sResponse == sPreviousResponse) 
		{
			state = AI_SPEAK_BOT_REPEAT;
		}
		else 
		{
			extractSubject();
			transpose_sentence(sSubject);
			replace(sResponse, "*", " " + sSubject);
			state = AI_SPEAK_RESPOND;
		}
		break;

	 case AI_SPEAK_DEFAULT_REPLY: //-------------------------------
		responseList = noKeyWord;
		state = AI_SPEAK_SELECT_RESPONSE;
		break;

	 case AI_SPEAK_RECALL_MEMORY: //-------------------------------
		nSelection = Random(nSubjectRecallSize);
		sResponse = subjectRecall[nSelection];
		trimLR(memory.top(), " ");
		transpose_sentence(memory.top());
		replace(sResponse, "*", " " + memory.top());
		memory.pop();
		state = AI_SPEAK_RESPOND;
		break;

	 case AI_SPEAK_CHANGE_TOPIC: //-------------------------------
		nSelection = Random(nTopicChangerSize);
		sResponse = topicChanger[nSelection];
		nNokeyFound = 0;
		state = AI_SPEAK_RESPOND;
		break;

	 case AI_SPEAK_MEMORISING: //-------------------------------
		memorise();
		state = AI_SPEAK_KEY_FOUND;
		break;

	 case AI_SPEAK_QUIT : //-------------------------------
		 // A FAIRE: Scanner l'equipement externe du player (du slot 0 au slot 12)
         // Si le player n'a pas de vetement: Race a utiliser si le nom n'est pas connu
    	 sResponse = "Au revoir "+m_CurrentPlayerNameStr+", On se voit la prochaine fois.";
         m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,sResponse.c_str());
		 m_CurrentPlayer = NULL; // 
		 m_PlayerNameknown = false;
		 m_CurrentPlayerNameStr = "Inconnu";
		 NEXT_SYMBOL = "";
		 state = AI_SPEAK_GET_INPUT;
		 break;
	 default: //-------------------------------
		 sResponse = "Ca craint la, "+m_CurrentPlayerNameStr+", On se voit la prochaine fois.";
         m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,sResponse.c_str());
		 state = AI_SPEAK_GET_INPUT;
		 break;
	}
}
//----------------------------------------------------------------------------------------
int AI_Speak::searchKeyWord(std::string str, std::vector<key_t> keyList) 
{
	std::string thisKey = "";
	std::string bestKey = "";
	int nKeyNum, nFoundPos = -1;
	int nBestPos = -1, nPos;
	int nkeyListSize = keyList.size();
	pos_list.clear();
	for(int i = 0; i < nkeyListSize; ++i) 
	{
		nKeyNum = keyList[i].keyWords.size();
		for(int j = 0; j < nKeyNum; ++j) 
		{
			thisKey = keyList[i].keyWords[j];
			thisKey.insert(0, " ");
			nPos = str.find(thisKey);
			if(nPos != std::string::npos) 
			{
				if(thisKey.length() > bestKey.length() ||
					(nPos != nBestPos && thisKey.length() == bestKey.length())) 
				{
					if(thisKey.length() > bestKey.length()) 
					{
						nBestPos = nPos;
						bestKey = thisKey;
						nFoundPos = i;
					} else 
					{
						nFoundPos = -1;
						pos_list.push_back(i);
					}
				}
			}
		}
	}
	keyWord = bestKey;
	nKeyPos = nFoundPos;
	return nFoundPos;
}
//----------------------------------------------------------------------------------------
void AI_Speak::PrintResponse(void) 
{
	m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,sResponse.c_str());
}
//----------------------------------------------------------------------------------------
int AI_Speak::Random( const int ival ) 
{
	if( ival == 0 ) 
	{
		return 0;
	} else 
	{ 
		return rand()%ival;
	}
}
//----------------------------------------------------------------------------------------
void AI_Speak::UpperCase( std::string &str ) 
{
	int len = str.length();
	for(int i = 0; i < len; ++i) 
	{
		if ( str[i] >= 'a' && str[i] <= 'z' ) 
		{
			str[i] -= 'a' - 'A';
		}
	}
}
//----------------------------------------------------------------------------------------
void AI_Speak::trimPuncs( std::string &str ) 
{
	std::string temp = "";
	int nStrLen = str.length();
	for(int i = 0; i < nStrLen; ++i) 
	{
		if(puncs.find(str[i]) == std::string::npos) 
		{
			temp += str[i];
		} 
		else if(str[i + 1] != ' ') 
		{
			temp += " ";
		}
	}
	trimRight(temp, ' ');
	str = temp;
}
//----------------------------------------------------------------------------------------
void AI_Speak::getInput( std::string &str ) 
{
	if(str.length() > 0) 
	{
		trimPuncs(str);
		UpperCase(str);
		str.insert(0, " ");
		str.append(" ");
	}
}
//----------------------------------------------------------------------------------------
bool AI_Speak::testRepetition(void) 
{
	bool bRetVal = false;
	if((sPreviousInput.length() != 0) && (sInput == sPreviousInput)) 
	{
		++nRepeatCount;
		if(nRepeatCount == 2) 
		{
			sResponse =  "Tu te repete.";
		} 
		else if(nRepeatCount == 3) 
		{
			sResponse = "Espece de Troll, tu trouve ca drole!?";
		} 
		else if(nRepeatCount == 4) 
		{
			sResponse = "Allez,tu peux faire mieux que ca.";
		} 
		else if(nRepeatCount >= 5) 
		{
			sResponse = "Ok, j'en ai marre de parler avec toi.";
			NEXT_SYMBOL = "QUIT";
		}
		bRetVal = true;
	}
	return bRetVal;
}
//----------------------------------------------------------------------------------------
// removes a substring from a given string
bool AI_Speak::remove(std::string &str, std::string substr) 
{
	int pos = str.find(substr);
	if(pos != std::string::npos) 
	{
		str.erase( pos, substr.length());
	}
	return (pos != std::string::npos);
}
//----------------------------------------------------------------------------------------
// trim the left character of a given string
void AI_Speak::trimLeft(std::string &str, char c) 
{
	int pos = 0;
	for(; str[pos] == c; ++pos);
	str.erase( 0, pos );
}
//----------------------------------------------------------------------------------------
// trim the right character of a given string
void AI_Speak::trimRight(std::string &str, char c) 
{
	int pos = str.length();
	if( pos == 0 ) return;
	for(--pos; str[pos] == c; --pos);
	str.erase(++pos, str.length() - pos);
}
//----------------------------------------------------------------------------------------
// trim all left and right characters that is specified by
// the string "characters"
void AI_Speak::trimLR(std::string &str, std::string characters) 
{
	int len = characters.length();
	for(int i = 0; i < len; ++i) 
	{
		trimLeft( str, characters[i] );
		trimRight( str, characters[i] );
	}
}
//----------------------------------------------------------------------------------------
// replace a substring by another substring
bool AI_Speak::replace(std::string &str, std::string oldsubstr, std::string newsubstr) 
{
	int pos = str.find(oldsubstr);
	if( pos != std::string::npos ) 
	{
		remove( str, oldsubstr );
		str.insert( pos, newsubstr );
	}
	return (pos != std::string::npos);
}
//----------------------------------------------------------------------------------------
// transposes a word
bool AI_Speak::transpose( std::string &str ) 
{
	std::string _str = " " + str + " ";
	std::string backup = str;
	std::string verbP1 = "";
	std::string verbP2 = "";
	for(int i = 0; i < (int)nTransPosNum; ++i) 
	{
		verbP1 = transposList[i].verbPhrase1;
		verbP2 = transposList[i].verbPhrase2;
		if(replace(_str, verbP1, verbP2)) 
		{
			break;
		} 
		else if(replace(_str, verbP2, verbP1)) 
		{
			break;
		} 
	}
	str = _str.substr(1, _str.length() - 2);
	return backup != str;
}
//----------------------------------------------------------------------------------------
// transpose a complete sentence
void AI_Speak::transpose_sentence( std::string &str ) 
{
	std::string thisWord = "";
	std::string tempStr = "";
	vstring stringToken;
	tok.setDelim(" !?,-");
	trimRight(str, '.');
	tok.tokenize(str, stringToken);
	int nTokNum = stringToken.size();
	for(int i = 0; i < nTokNum; ++i) 
	{
		thisWord = stringToken[i];
		transpose(thisWord);
		tempStr += thisWord + " ";
	}
	str = tempStr;
	trimRight(str, ' ');
}
//----------------------------------------------------------------------------------------
void AI_Speak::extractSubject() 
{
	sSubject = "";
	int pos = sInput.find(keyWord);
	if(pos != std::string::npos) 
	{
		pos += keyWord.length();
		sSubject = sInput.substr(pos, sInput.length() - pos);
	}
}
//----------------------------------------------------------------------------------------
void AI_Speak::saveUnknownSentences() 
{
	/*
	int nUnknownSentencesSize = unknownSentences.size();
	std::fstream outfile;
	outfile.open("unkown.txt", std::ios::out | std::ios::app);
	for(int i = 0; i < nUnknownSentencesSize; ++i) 
	{
		outfile << unknownSentences[i] << std::endl;
	}
	outfile.flush();
	outfile.close();
	*/
	m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Sauvegarde du vocabulaire inconnu.");
}
//----------------------------------------------------------------------------------------
// parses the scripts file associated with the bot
bool AI_Speak::parseScriptFile(void) 
{
	std::fstream fin("script.txt", std::ios::in);
	if(fin.fail()) 
	{
		m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Je n'ai pas de vocabulaire.");
		return(false);
	}
	std::string buffer = "";
	int counter = 0;
	char cSymbol;
	//bool good_symbol = true;
	while(std::getline(fin, buffer, '\n')) 
	{
		cSymbol = buffer[0];
		buffer.erase(0, 1);
		switch(cSymbol) 
		{
		 case ';': dump_data();break;
		 case 'S': signOn.push_back(buffer); break;
		 case 'T':
			++counter;
			buffer.erase(buffer.length() - 1, 1);
			if(counter % 2 == 1) 
			{
				thisTranspos.verbPhrase1 = buffer;
			}
			else 
			{
				thisTranspos.verbPhrase2 = buffer;
				transposList.push_back(thisTranspos);
			}
			break;
		case 'K':
			buffer.erase(buffer.length() - 1, 1);
			thisKey.keyWords.push_back(buffer);
			break;
		case 'R':
			thisKey.response.push_back(buffer);
			break;
		case 'M':
			subjectRecall.push_back(buffer);
			break;
		case 'N':
			nullResponse.push_back(buffer);
			break;
		case 'X':
			noKeyWord.push_back(buffer);
			break;
		case 'Z':
			topicChanger.push_back(buffer);
			break;
		case 'L':
			buffer.erase(buffer.length() - 1, 1);
			_thisKey.keyWords.push_back(buffer);
			break;
		}
	}
	fin.close();
	return(true);
}
//----------------------------------------------------------------------------------------
void AI_Speak::dump_data(void) 
{
	if(thisKey.keyWords.size() > 0) 
	{
		keyWordList.push_back(thisKey);
		thisKey.keyWords.clear();
		thisKey.response.clear();
	}
	if(_thisKey.keyWords.size() > 0) 
	{
		_thisKey.response = thisKey.response;
		keyForLearning.push_back(_thisKey);
		_thisKey.keyWords.clear();
		_thisKey.response.clear();
		thisKey.response.clear();
	}
}
//----------------------------------------------------------------------------------------
bool AI_Speak::print_keyword_list(void) 
{
	m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Liste de mots.");
	/*
	std::fstream fout("test.txt", std::ios::out);
	if(fout.fail()) 
	{
		//std::cerr << "Error while opening \"test.txt\"" << std::endl;
		//assert(fout.good());
		m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Je ne peux pas sauvegarder de liste de mots.");
		return(false);
	}
	vstring keyWords;
	vstring responses;
	int numOfKeyword, numOfResponse;
	fout << "Keyword List:\n";
	//std::cout << "Keyword List:\n";
	for(int i = 0; i < nKeyWordsSize; ++i) 
	{
		keyWords = keyWordList[i].keyWords;
		responses = keyWordList[i].response;
		numOfKeyword = keyWords.size();
		numOfResponse = responses.size();
		for(int j = 0; j < numOfKeyword; ++j) 
		{
			fout << "  " << keyWords[j] << std::endl;
			//std::cout << "  " << keyWords[j] << std::endl;
			
		}
		for(int k = 0; k < numOfResponse; ++k) {
			fout << "     " << responses[k] << std::endl;
			//std::cout << "     " << responses[k] << std::endl;
		}
		fout << std::endl;
		//std::cout << std::endl;
	}
	fout.flush();
	fout.close();
	*/
}
//----------------------------------------------------------------------------------------
void AI_Speak::print_response_list(void) 
{
	m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Affichage de la liste de mots.");
	/*
	vstring keyWords;
	vstring responses;
	int numOfKeyword;
	int numOfResponse;
	keyWords = keyWordList[nKeyPos].keyWords;
	responses = keyWordList[nKeyPos].response;
	numOfKeyword = keyWords.size();
	numOfResponse = responses.size();
	for(int j = 0; j < numOfKeyword; ++j) 
	{
		std::cout << "  " << keyWords[j] << std::endl;
	}
	for(int k = 0; k < numOfResponse; ++k) 
	{
		std::cout << "     " << responses[k] << std::endl;
	}
	*/
}
//----------------------------------------------------------------------------------------
void AI_Speak::memorise(void) 
{
	if(sPreviousInput.length() > 0) 
	{
		bNewData = true;
		thisKey.keyWords.clear();
		thisKey.response.clear();
		sResponse = sInput + ".";
		transpose_sentence(sResponse);
		keyWord = sPreviousInput;
		thisKey.keyWords.push_back(keyWord);
		thisKey.response.push_back(sResponse);
		keyWordList.push_back(thisKey);
		nKeyWordsSize++;
	}
}
//----------------------------------------------------------------------------------------
void AI_Speak::saveData(void) 
{
	m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Je memorise les mots.");
	/*
	scriptfile.open("script.txt", std::ios::out);
	if(scriptfile.fail()) {
		std::cerr << "Error while opening \"script.txt\"";
		assert(scriptfile.good());
	}
	scriptfile << ";" << std::endl;
	scriptfile << "; ELIZA SCRIPT FILE" << std::endl;
	scriptfile << ";" << std::endl;
	scriptfile << "; Copyright 2006 Gonzales Cenelia" << std::endl;
	scriptfile << ";" << std::endl;
	scriptfile << "; FORMAT:" << std::endl;
	scriptfile << "; Sxxxxxx    - DEFINE SIGNON MESSAGE" << std::endl;
	scriptfile << "; Txxxxx" << std::endl;
	scriptfile << "; Txxxxx     - DEFINE PAIR OF WORDS TO TRANSPOSE" << std::endl;
	scriptfile << "; Nxxxxxx    - RESPONSE FOR NULL ENTRY" << std::endl;
	scriptfile << "; Mxxxxxx    - DEFINE RESPONSE FOR LATER USE OF \"MY\"" << std::endl;
	scriptfile << "; Xxxxxxx    - DEFINE RESPONSE FOR NO KEYWORD FOUND" << std::endl;
	scriptfile << "; Zxxxxxx    - DEFINE CONVERSATION TOPICS" << std::endl;
	scriptfile << "; Kxxxxxx    - DEFINE KEYWORD" << std::endl;
	scriptfile << "; Rxxxxxx    - DEFINE RESPONSE FOR PREVIOUS KEYWORD" << std::endl;
	scriptfile << "; Lxxxxxx    - DEFINES KEYWORD FOR TEATCHING NEW CONCEPT" << std::endl;
	scriptfile << "; Dxxxxxx    - DEFINES KEYWORD FOR WHEN THE PROGRAM DOESN'T UNDERSTAND" << std::endl;
	scriptfile << "; Cxxxxxx    - DEFINES KEYWORD FOR REPLYING BY USING CONTEXT" << std::endl;
	scriptfile << ";" << std::endl;
	scriptfile << "; NOTE:  FOR \"T\" AND \"K\", THE LAST CHARACTER IS DISCARDED TO MAKE IT EASY" << std::endl;
	scriptfile << ";        TO ENTER TRAILING SPACES." << std::endl;
	scriptfile << ";" << std::endl;
	scriptfile << ";        FOR \"R\" AND \"M\" THE STRING FOLLOWING THE KEYWORD IN THE USER'S" << std::endl;
	scriptfile << ";        INPUT IS INSERTED AT THE *" << std::endl;
	scriptfile << ";" << std::endl;
	scriptfile << ";        IF A KEYWORD HAS NO RESPONSES, THEN IT WILL USE THE RESPONSES OF" << std::endl;
	scriptfile << ";        THE NEXT KEYWORD WITH RESPONSES" << std::endl;
	scriptfile << ";" << std::endl;
	scriptfile << "; GOAL: THE PROGRAM SHOULD HANDLE EVERY SUBTILITIES OF THE ENGLISH LANGUAGE" << std::endl;
	saveSignOn();
	saveTransposTable();
	saveNullResponse();
	saveSubjectRecall();
	saveNoKeyWord();
	saveTopicChanger();
	saveKeyWordList();
	scriptfile.flush();
	scriptfile.close();
	*/
}
//----------------------------------------------------------------------------------------
void AI_Speak::saveSignOn(void) 
{
	m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"saveSignOn.");
  /*
	scriptfile << ";" << std::endl;
	scriptfile << "; SIGNON MESSAGE" << std::endl;
	scriptfile << ";" << std::endl;
	for(int i = 0; i < nSignOnSize; ++i) 
	{
		scriptfile << "S" << signOn[i] << std::endl;
	}
	*/
}
//----------------------------------------------------------------------------------------
void AI_Speak::saveTransposTable(void) 
{
	m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"saveTransposTable.");
	/*
	scriptfile << ";" << std::endl;
	scriptfile << "; TRANSPOSITION TABLE" << std::endl;
	scriptfile << ";" << std::endl;
	for(int i = 0; i < nTransPosNum; ++i) {
		scriptfile << "T" << transposList[i].verbPhrase1 << "\"" << std::endl;
		scriptfile << "T" << transposList[i].verbPhrase2 << "\"" << std::endl;
	}
	*/
}
//----------------------------------------------------------------------------------------
void AI_Speak::saveNullResponse(void) 
{
	m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"saveNullResponse.");
	/*
	scriptfile << ";" << std::endl;
	scriptfile << "; NULL RESPONSES" << std::endl;
	scriptfile << ";" << std::endl;
	for(int i = 0; i < nNullSize; ++i) 
	{
		scriptfile << "N" << nullResponse[i] << std::endl;
	}
	*/
}
//----------------------------------------------------------------------------------------
void AI_Speak::saveNoKeyWord(void) 
{
	m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"saveNoKeyWord.");
	/*
	scriptfile << ";" << std::endl;
	scriptfile << "; NO KEYWORD FOUND" << std::endl;
	scriptfile << ";" << std::endl;
	for(int i = 0; i < nNoKeySize; ++i) {
		scriptfile << "X" << noKeyWord[i] << std::endl;
	}
	*/
}
//----------------------------------------------------------------------------------------
void AI_Speak::saveTopicChanger(void) 
{
	m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"saveTopicChanger.");
	/*
	scriptfile << ";" << std::endl;
	scriptfile << "; THE FOLLOWING RESPONSES WILL HOPEFULLY TRY TO 'STEER'" << std::endl;
	scriptfile << "; THE CONVERSATION TO A TOPIC THAT MAY POSSIBLY PRODUCE" << std::endl;
	scriptfile << "; MORE RECOGNIZABLE WORDS (KEYWORDS)." << std::endl;
	scriptfile << ";" << std::endl;
	scriptfile << ";" << std::endl;
	for(int i = 0; i < nTopicChangerSize; ++i) {
		scriptfile << "Z" << topicChanger[i] << std::endl;
	}
	*/
}
//----------------------------------------------------------------------------------------
void AI_Speak::saveSubjectRecall(void) 
{
	m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"saveSubjectRecall.");
	/*
	scriptfile << ";" << std::endl;
	scriptfile << "; RESPONSES FOR \"MY\"" << std::endl;
	scriptfile << "; TO USE IF NO KEYWORD WAS FOUND." << std::endl;
	scriptfile << ";" << std::endl;
	for(int i = 0; i < nTopicChangerSize; ++i) {
		scriptfile << "M" << topicChanger[i] << std::endl;
	}
	*/
}
//----------------------------------------------------------------------------------------
void AI_Speak::saveKeyWordList(void) 
{
	m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"saveKeyWordList.");
	/*
	scriptfile << ";" << std::endl;
	scriptfile << "; DEFINE KEYWORDS AND RESPONSES" << std::endl;
	scriptfile << ";" << std::endl;
	scriptfile << ";" << std::endl;
	scriptfile << "; BOT KNOWLEDGE" << std::endl;
	scriptfile << ";" << std::endl;
	vstring keyWords;
	vstring responses;
	int numOfKeyword;
	int numOfResponse;
	for(int i = 0; i < nKeyWordsSize; ++i) 
	{
		keyWords = keyWordList[i].keyWords;
		responses = keyWordList[i].response;
		numOfKeyword = keyWords.size();
		numOfResponse = responses.size();
		for(int j = 0; j < numOfKeyword; ++j) 
		{
			scriptfile << "K" << keyWords[j] << "\"" << std::endl;
		}
		for(int k = 0; k < numOfResponse; ++k) 
		{
			scriptfile << "R" << responses[k] << std::endl;
		}
		scriptfile << ";" << std::endl;
	}
	*/
}
//----------------------------------------------------------------------------------------
void AI_Speak::select_response(void) 
{
	std::string sTempResponse = sResponse;
	for(;;) // Ca c pas cool du tout, verif sur les possibilités de sortie (Brz)
	{
		nSelection = Random(nKeyNum);
		sResponse = responseList[nSelection];
		if(sResponse != sTempResponse) 
		{
			break;
		}
		else if(nKeyNum > 1) 
		{
			responseList.erase(responseList.begin() + nSelection);
			nKeyNum = responseList.size();
		}
	}
}
//----------------------------------------------------------------------------------------
void AI_Speak::handleRepetition(void) 
{
	std::list<std::string>::iterator iter;
	if(nKeyNum > 1) 
	{
		responseList.erase(responseList.begin() + nSelection);
	}
	else if(nKeyNum == 1) 
	{
		responseList = topicChanger;
		nKeyNum = nTopicChangerSize;
	}
	select_response();
	iter = std::find(extendedResponseList.begin(), extendedResponseList.end(), sResponse);
	if(iter != extendedResponseList.end() && nKeyNum > 1) 
	{
		responseList.erase(responseList.begin() + nSelection);
		select_response();
	}
}
//****************************************************************************************
// Ajouté pour context AscentWaad (Branruz)
//****************************************************************************************
bool AI_Speak::IsActionOrMove(void)
{
	if(strstr(sResponse.c_str(),"_ACTION_SE_DEPLACE_"))
	{
		action = AI_SPEAK_ACTION_SE_DEPLACE;
	    return(true);
	}
    else if(strstr(sResponse.c_str(),"_ACTION_SUIVRE_"))
	{
		action = AI_SPEAK_ACTION_SUIVRE;
	    return(true);
	}
    else if(strstr(sResponse.c_str(),"_ACTION_RESTE_"))
	{
		action = AI_SPEAK_ACTION_RESTE;
	    return(true);
	}
	else if(strstr(sResponse.c_str(),"_GIVE_ITEM_"))
	{
		action = AI_SPEAK_GIVE_ITEM;
	    return(true);
	}
	else if(strstr(sResponse.c_str(),"_SPELL_ID_"))
	{
	    action = AI_SPEAK_SPELL_ID;
	    return(true);
	}
	else if(strstr(sResponse.c_str(),"_GOB_NAME_"))
	{
		action = AI_SPEAK_GOB_NAME;
		return(true);
	}
	else if(strstr(sResponse.c_str(),"_SELECTION_"))
	{
		action = AI_SPEAK_SELECTION;
		return(true);
	}
	else if(strstr(sResponse.c_str(),"_ACTION_START_"))
	{
		action = AI_SPEAK_ACTION_START;
		return(true);
	}
	else if(strstr(sResponse.c_str(),"_AOE_"))
	{
		action = AI_SPEAK_AOE;
		return(true);
	}

	action = AI_SPEAK_NULL_ACTION;
	return(false);
}
//----------------------------------------------------------------------------------------
bool AI_Speak::ReplaceResponseInContextAction(void)
{
 char *ptr = NULL;
 uint64 tmp_guid = 0;
 string _tmpstr,_tmpstr2;
 Creature *tmp_creature = NULL;
 size_t found_tag = 0;
 int gotcha = 0;
 float cDist = 9999.0f;
 float nDist = 0.0f;

#define _tmpbuff_size	255
 char _tmpbuff[_tmpbuff_size];

 switch(action)
 {
  case AI_SPEAK_ACTION_SE_DEPLACE :
	  gotcha = 0;                  // m_CurrentPlayer
	  
	  /*
	  //-----------------------------------------------------------------
	  // Recuperation de l'endroit seulement pour recherche dans la base.
	  _tmpstr2 = sInput;
	  remove(_tmpstr2,"VA VERS "); // A faire autrement, juste pour trouver l'endroit facilement (Brz)

      found_tag = _tmpstr2.find("L'"); 
	  if (found_tag!=string::npos) remove(_tmpstr2,"L'");
	  found_tag = _tmpstr2.find("LA"); 
	  if (found_tag!=string::npos) remove(_tmpstr2,"LA ");
	  found_tag = _tmpstr2.find("LE"); 
	  if (found_tag!=string::npos) remove(_tmpstr2,"LE ");
	  found_tag = _tmpstr2.find("LES"); 
	  if (found_tag!=string::npos) remove(_tmpstr2,"LES ");

      QueryResult *result = WorldDatabase.Query( "SELECT * FROM `npc_poi` WHERE `NameFr` LIKE %c%%%s%%%c",'"',_tmpstr2.c_str(),'"' );
	  if(result)
	  {
        do
	    {
		 npc_poi_search *NpcPoi = new npc_poi_search;
		 if(NpcPoi)
		 {
		  Field *fields = result->Fetch();
		  // fields[0] = entry, auto increment
		  NpcPoi->NameFr  = fields[1].GetString();
		  NpcPoi->pox_x   = fields[2].GetFloat();
		  NpcPoi->pox_y   = fields[3].GetFloat();
		  NpcPoi->pox_z   = fields[4].GetFloat();
		  NpcPoi->Map_id  = fields[5].GetUInt32();
		  NpcPoi->Area_id = fields[6].GetUInt32();
		  npc_SamePoiListKnown.pushback(NpcPoi);
		  gotcha = true;
		 }
		 else 
		 {
		  m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Je n'ai pas assez de memoire pour me rappeler ou s'est); 
		  break;
		 }
	    } while (result->NextRow());
		// if(npc_SamePoiListKnown->size() > 0)
		// (...)
		// m_AISpeakTarget = 
		// (...)
		// delete npc_SamePoiListKnown[index] // Suppression des new NpcPoi du "do...while"
		npc_SamePoiListKnown.clear();
	  }
      else 
	  {
		m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Je ne connais que le coin, attend je cherche ou c'est!); 
	  }
	  */
	  //-----------------------------------------------------------------
	  // Enumeration des Gob connu par le pnj
	  for(set<Object*>::iterator itr = m_UnitOwner->m_objectsInRange.begin(); itr != m_UnitOwner->m_objectsInRange.end(); ++itr)
	  {
	   if((*itr)->IsGO()) // On cherche ou aller
	   {
         _tmpstr = GameObjectNameStorage.LookupEntry(((GameObject *)(*itr))->GetEntry())->Name; 
		 _tmpstr2 = sInput;
		 UpperCase(_tmpstr);

		 found_tag = _tmpstr2.find(_tmpstr);
		 if (found_tag!=string::npos) // L'endroit est trouvé, mais y'en a peut-etre plusieurs
	     {                            // On prend le plus pres (paresseux le Pnj, lol)
		  
		  if( (nDist = m_UnitOwner->CalcDistance( *itr )) < cDist ) // Le gob le plus proche si rien de precis est connu
		  {
		   ++gotcha;
		   cDist = nDist;
		   nDist = 0.0f;
		   m_AISpeakTarget = (*itr); 
           m_StandByForTarget = false; // La cible est connu
		   //snprintf(_tmpbuff,_tmpbuff_size-5,"Ya %s a %.3f m",_tmpstr.c_str(),cDist*0.98 );
		   //m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,_tmpbuff); 
		  }
		 }
			 
	   }
	  }
	  //-----------------------------------------------------------------
	  if(gotcha) 
	  {
		  nDist = m_UnitOwner->CalcDistance(m_AISpeakTarget);
		  _tmpstr = GameObjectNameStorage.LookupEntry(((GameObject *)(m_AISpeakTarget))->GetEntry())->Name; 
		  memset(_tmpbuff,0x00,_tmpbuff_size); 
		  if(gotcha == 1) // Une seule cible trouvée, goooood......
		  {
			  snprintf(_tmpbuff,_tmpbuff_size-5,"A environ %4.f m je suppose ? (%.3f %.3f %.3f)",nDist*0.98);
		  }
		  else // Plusieurs cible, on choisi la plus pres (pour l'instant)
		  {
		     snprintf(_tmpbuff,_tmpbuff_size-5,"Il y a %d %s(s) pas tres loin, je vais au plus pres, a environ %4.f m",
			                                  gotcha,_tmpstr.c_str(),nDist*0.98 );
		  }
		  m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,_tmpbuff); 
          
		  float coordX_go = ((GameObject *)m_AISpeakTarget)->GetPositionX();
		  float coordY_go = ((GameObject *)m_AISpeakTarget)->GetPositionY();
		  float coordZ_go = ((GameObject *)m_AISpeakTarget)->GetPositionZ();
		  float coordO_go = ((GameObject *)m_AISpeakTarget)->GetOrientation();
		  AIInterface *ai_int = ((Unit *)m_UnitOwner)->GetAIInterface();
		  if(ai_int) 
		  {
			  m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Bon, j'y vais..."); 
			  ai_int->MoveTo(coordX_go,coordY_go,coordZ_go,coordO_go);

			  snprintf(_tmpbuff,_tmpbuff_size-1,"Le gps est %.3f %.3f %.3f",coordX_go,coordY_go,coordZ_go);
			  m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,_tmpbuff); 
		  }
		  else
		  {
             m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"J'le sens pas, je reste la..."); 
		  }


		  return(true); // juste la pour le debug

		//found_tag = sResponse.find("_ACTION_SE_DEPLACE_");
        //if (found_tag!=string::npos) // Ce tag est optionnel, a ce stade c'est une reponse "toute faite"
	    //{
	    // sResponse.erase(found_tag,19); // 19 = strlen("_ACTION_SE_DEPLACE_")
	    //}
	    //return(false);
	  }
	  else
	  {
		  m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Hmmm, nan desole je ne connais pas cet endroit!"); 
		  return(true);
	  }
      break;
	  

  case AI_SPEAK_GIVE_ITEM : 
      ptr = (char *)strstr(sInput.c_str()," L'ITEM "); // L'Id suis derriere (normalement)
	  if(ptr)
	  {
	   uint32 ItemId = atoi((ptr+8)); // Apres ' L'ITEM ' -> 'Donne moi l'item 12345 stp.' ou 'Peux tu me donner l'item 12345 stp.' // par exemple
	   if(ItemId)
	   {
		   found_tag = sResponse.find("_ITEM_NAME_");
		   if (found_tag!=string::npos) // Secu, impossible que le tag ne soit pas trouvé
		   {
            // Remplacement de '_ITEM_NAME_' par l'id de l'item (pour l'instant)
	        //sResponse.erase(found_tag,11); // 11 = strlen("_ITEM_NAME_")
		    char _tmp[20];
		    sprintf(_tmp,"%d",ItemId);
			string _tmpstr = _tmp;
			replace(sResponse, "_ITEM_NAME_", _tmpstr);
		   // sResponse.insert(found_tag,_tmp);

			//size_t found_tag2 = sResponse.find("_GIVE_ITEM_");
            //if (found_tag2!=string::npos) sResponse.erase(found_tag2,11); // 11 = strlen("_GIVE_ITEM_")
			return(false); // 
		   }
		   else 
		   { // _ITEM_NAME_ non trouvé dans la reponse, carrement impossible
            m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Pas possible ca, mais ou ai-je mis ce foutu item"); // Impossible
		   }
	   }
	   // le numero de l'item n'a pas été trouvé, la demande est simplement mal formulée
	   else m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"De quel item tu parle ?\nTu peux repeter plus distinctement ?"); // Possible
	  }
	  else
	  { // 'l'item' n'a pas été trouvé, impossible lorsque qu'on arrive a ce stade du code
       m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Mais de quel item parles tu ? J'ai rien compris du tout."); // Impossible
	  }
	  return(true);

  case AI_SPEAK_SPELL_ID     : 
	  ptr = (char *)strstr(sInput.c_str()," SPELL "); // L'Id suis derriere (normalement)
	  if(ptr)
	  {
	   uint32 SpellId = atoi((ptr+7)); // Apres ' SPELL ' -> 'Lance le spell 12345 stp.' ou 'Tu peux lancer le spell 12345 stp.' // par exemple
	   if(SpellId)
	   {
	    found_tag = sResponse.find("_SPELL_ID_");
        if (found_tag!=string::npos) // Secu, impossible que le tag ne soit pas trouvé
		{
         // Remplacement de '_SPELL_ID_' par l'id du Spell
	     sResponse.erase(found_tag,10); // 10 = strlen("_SPELL_ID_")
		 char _tmp[20];
		 sprintf(_tmp,"%d",SpellId);
		 sResponse.insert(found_tag,_tmp);
		
		 // Attention, a ce stade on DOIT poser un flag de selection de cible si elle n'existe pas
		 // Dans ce cas : c'est le pnj qui va la demander (la cible)
		 if(m_AISpeakTarget == NULL) 
		 {
		   m_StandByForTarget = true;
		   m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,sResponse.c_str());  
		 }
		 else
		 {
            char _tmp[20]; // Grrrrrrrrr
		    sprintf(_tmp,"%d",SpellId);
			std::string _spellStr = _tmp;
			std::string _NameStr = ((m_AISpeakTarget->GetObjectName()!=NULL)?(m_AISpeakTarget->GetObjectName()):"un inconnu");
			sResponse = "C'est parti pour le spell "+_spellStr+" sur "+_NameStr;
			m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,sResponse.c_str());  
			//m_UnitOwner->CastSpell(SpellId,m_AISpeakTarget);
			m_AISpeakTarget = NULL; // C'est la cible du pnj, plus celle de la demande
			m_StandByForTarget = false;
		 }
		}
		else 
		{ // _SPELL_ID_ non trouvé dans la reponse, carrement impossible
         m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Pas possible ca, ou est le foutu marque page du spell"); // Impossible
		}
	   } // le numero du spell n'a pas été trouvé, la demande est simplement mal formulée
	   else m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"De quel spell tu parle ?\nTu peux repeter plus distinctement ?"); // Possible
	  }
	  else
	  { // le mot 'spell' n'a pas été trouvé, impossible lorsque qu'on arrive a ce stade du code
       m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Mais de quel spell parles tu ? J'ai rien compris du tout."); // Impossible
	  }
	  return(true);

  case AI_SPEAK_SELECTION    : 
	  tmp_creature = NULL;
	  tmp_guid     = m_CurrentPlayer->GetSelection();
	  if(tmp_guid)
	  {      
		  if(GET_TYPE_FROM_GUID(tmp_guid) == HIGHGUID_TYPE_PET)
		   tmp_creature = m_CurrentPlayer->GetMapMgr()->GetPet( GET_LOWGUID_PART(tmp_guid) );
	      else if(GET_TYPE_FROM_GUID(tmp_guid) == HIGHGUID_TYPE_UNIT)
		   tmp_creature = m_CurrentPlayer->GetMapMgr()->GetCreature( GET_LOWGUID_PART(tmp_guid) );

		  if(tmp_creature == NULL)
		  {
			  m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Euh, tu es sur ?"); 
		  }
		  else
		  {
			  found_tag = sResponse.find("_SELECTION_");
              if (found_tag!=string::npos) // Secu, impossible que le tag ne soit pas trouvé
		      {
               // Remplacement de '_SELECTION_' par le nom du pnj
	           sResponse.erase(found_tag,11); // 11 = strlen("_SELECTION_")
			   sResponse.insert(found_tag,tmp_creature->GetCreatureName()->Name);
               m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,sResponse.c_str()); 
			  }
			  else m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Bah, je perd la memoire, sur qui deja je lance le spell?"); 
		  }
	  }
	  else m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,"Mais de qui parles tu ?\nMontre moi..."); 

	  return(true);

  case AI_SPEAK_AOE          : 
  case AI_SPEAK_GOB_NAME     :
  case AI_SPEAK_ACTION_START : 
  case AI_SPEAK_ACTION_RESTE :
  case AI_SPEAK_ACTION_SUIVRE: 
	  m_UnitOwner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL,sInput.c_str());
	  return(false);

  default : 
      sResponse = "J'ai un probleme, je ne sais plus quoi faire la!.";
	  m_UnitOwner->Emote(EMOTE_STATE_SUBMERGED);
	  return(false);
 }
 return(false);
 
}
//----------------------------------------------------------------------------------------



	





