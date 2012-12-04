/*
 * AscentWaad, WorldIrcRelay Coding ONLY for Open Source Ascent Server
 * Copyright (C) 2008-2011 WaadTeam <http://arbonne.games-rpg.net/>
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
 * Codeur: Franck77 (95%), Branruz (5%)
 *
 */
#include "StdAfx.h"

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>  
#else
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#include <fstream>
#include <string>
 
// inc pour soclet non bloquant
#include <fcntl.h>
 
using namespace std; 

#define MAXLINE 4096

// Config du bot
#define localhost "x"
#define serveurhost "x"
#define MessAccueil "Bienvenue sur le Tchat de Wow Ascent Arbonne DB!!! http://arbonne.games-rpg.net/"
#define BotVersion "0.3"

// un singleton c'est le mieux, une seule instance du Relay Irc
// Appel => WaadIrcRelay::getSingleton().WaadBotLauncher();
initialiseSingleton( WaadIrcRelay );

string WaadIrcRelay::RNick(string addr)
{
  int fin_du_nick = (int)addr.find('!');
  string addr_copie = addr;
  addr = addr.substr ( 1 , fin_du_nick - 1 ) ;
  return addr;
}

string WaadIrcRelay::RUsername(string addr)
{
  int debut_username = (int)addr.find ( '!' )+1;
  int fin_username = (int)addr.find ( '@' );
  addr = addr.substr ( debut_username , fin_username-debut_username );
  return addr;
}

string WaadIrcRelay::RHost(string addr)
{
  int debut_host = (int)addr.find('@')+1;
  int taille = (int)addr.length();
  addr = addr.substr ( debut_host , taille-debut_host );
  return addr;
}


int WaadIrcRelay::EnvMsg (string a_envoyer)
{
	//printf("Emsg Debug: %s\n",a_envoyer.c_str());
	size_t size_env= a_envoyer.size() +1;
	char * buffer_env = new char[ size_env ];
	strncpy( buffer_env, a_envoyer.c_str(), size_env );
	
	int length_env = (int)a_envoyer.length();
	//printf("Emsg Debug: (%s) %u\n",buffer_env, length_env);
	
	#ifdef WIN32

	if (send(sockfd, buffer_env, length_env,0) < 0)
	{
		printf ("Erreur d'envoi : %d.\n\n", WSAGetLastError());		
	} 
	else 
	{
		//printf ("Envoi vers le client ok.\n\n");	
	}

	#else

	if (write(sockfd, buffer_env, length_env) < 0) { perror("Erreur d'envoi"); exit(1); } // envoie sur le socket linux

	#endif

	free(buffer_env);
	return 0;
}

int WaadIrcRelay::explode (string donnees, string separateur)
{
	int taille_donnees;
	int taille_sep;
	int pos;
	int a = 0;
	int fin;
	string argument;

	for (int nb=0;nb < 100; nb++)
	{
		m_arg[nb] = "";
	}
	
	taille_sep = (int)separateur.size();

	recherche_arg:
	
	taille_donnees = (int)donnees.size();
	pos = (int)donnees.find(separateur);
	
	if ( (pos != string::npos) )
	{
		argument = donnees.substr(0,pos);
		fin = taille_donnees - pos - taille_sep;
		donnees = donnees.substr(pos+taille_sep,fin);
		m_arg[a] = argument;
		a++;
		goto recherche_arg;

	} else {
		m_arg[a] = donnees;
		a++;
	}
	return a;
}

string WaadIrcRelay::StrUp(string strToConvert)
{
	for(unsigned int i=0;i<strToConvert.length();i++)
	{
		strToConvert[i] = toupper(strToConvert[i]);
	}
	return strToConvert;
}

string WaadIrcRelay::StrLo(string strToConvert)
{
	for(unsigned int i=0;i<strToConvert.length();i++)
	{
		strToConvert[i] = tolower(strToConvert[i]);
	}
	return strToConvert;
} 

char WaadIrcRelay::JoinCanal()
{
	m_connected = true;
	m_reponse = "JOIN " + (string)m_BotChan + "\n";
	EnvMsg(m_reponse);
	return 0;
}

int WaadIrcRelay::GestionLigne ( string buffer_serveur )
{
	//printf ("Buffer : %s\n",buffer_serveur.c_str());

	int taille_buffer = (int)buffer_serveur.size();

	int posi;
	int nbr_arg;
	string recherche;
	string Parse;
	
	// On fait survivre le bot en repondant o ping serveur 
	recherche = "PING";
	posi = (int)buffer_serveur.find(recherche);
	if ( (posi !=string::npos) && (posi == 0) ) 
	{
		string recup_ping = buffer_serveur.substr( posi+5 , taille_buffer-posi+5 );
		printf ("Ping -> Pong\n");
		m_reponse = "PONG " + recup_ping + "\n";
		EnvMsg(m_reponse);
	} 
	else 
	{
		
		nbr_arg = explode(buffer_serveur," ");
		Parse = m_arg[1];

// PARSE GESTION DU BOT 		
		if ( (Parse == "422") || (Parse == "376") )
		{
			JoinCanal();
		}
		if ( (Parse == "KICK") && (m_arg[3] == m_nick) )
		{
			JoinCanal();
		}
		if ( Parse == "433" )
		{
			m_nick = (string)m_nick + "to";
			m_reponse = "NICK " + m_nick + "\n";
			EnvMsg(m_reponse);
		}			
		
// PARSE GESTION DES USERS
		if ( Parse == "JOIN" )
		{
			m_reponse = "NOTICE " + (string)RNick(m_arg[0]) + " :" + (string)MessAccueil + "\n";
			EnvMsg(m_reponse);
		}
		if ( (Parse == "PRIVMSG") || (Parse == "NOTICE") )
		{

			string msg_qui = m_arg[0];
			string msg_ou = m_arg[2];
			string msg_quoi;

			for(int i=3;m_arg[i]!="";i++)
			{
				msg_quoi += m_arg[i]+" ";
			}
			
			// protection antiflood du canal 
			if ( RNick(m_arg[0]) == m_nick ) 
			{
				printf ("Le bot ne flood pas\n");
				return 0;
			}
			m_flood_m1 = msg_quoi;
			if ( m_flood_m1 == m_flood_m2 )
			{
				if ( m_flood_m2 == m_flood_m3 )
				{
					// on kick ?
					m_reponse = "mode " + (string)m_BotChan + " +m\n";
					EnvMsg(m_reponse);
					m_reponse = "NOTICE " + (string)m_BotChan + " :Flood detecté ... je passe le canal en +m\n";		
					EnvMsg(m_reponse);
				} else {
					m_flood_m3 = m_flood_m2;
				}
			} 
			else 
			{
				m_flood_m2 = m_flood_m1;
			}

			// message o bot :)
			/*
			if (m_arg[2] != m_BotChan) 
			{
				if ( strstr(StrLo(m_arg[3]).c_str(),":!o" ) )
				{
					m_reponse =  m_arg[4] + " " +  m_arg[5] + " " + m_arg[6] + " " + m_arg[7] + " " + m_arg[8] + " " + m_arg[9] + "\n";		
					printf ("DEBUG reponse %s\n",m_reponse.c_str());		
					EnvMsg(m_reponse);
					return(0);
				}
			}*/
			
	        if ( strstr(m_arg[3].c_str(),":!WaadBot") )
			{
				SendToChatIrc("Relay Operationnel");		
			  
			} 
			
			//** Envoi de la ligne au serveur Wow pour affichage client
			printf("<ChatIrc> %s\n",m_arg[3].c_str());
			//**
			


		}
		
	}

	return 0;
}

void WaadIrcRelay::SendToChatIrc(char *_texte)
{
	string _texte_player;

	if(!m_connected) return;

	_texte_player = _texte;
	m_reponse = "PRIVMSG " + (string)m_BotChan + " :" + _texte_player + "\n";		
	EnvMsg(m_reponse);
}

int WaadIrcRelay::WaadBotLauncher(void)
{
	int erreur;
//	char Clavier;
//	char BuffSend[MAXLINE+1];

	printf ("WaadBot %s par Franck\n",BotVersion);

	/*
	uint32 m_IrcUseChatIrc;     // 0 
    uint32 m_IrcServPort;       // 6667
    string m_IrcServHostIp;     // "127.0.0.1"
    string m_IrcChan;           // "#chat_to_join"
    string m_IrcBotNick;        // "BotFromMyWowServer"
    string m_IrcBotName;        // "WaadIrcServerRelay"
    string m_IrcBotText;        // "Waad World Server Relay actif."
	uint32 m_IrcPlayerConnect;  // 1
	uint32 m_IrcChannelWisp;    // 1
	uint32 m_IrcChannelGuilde;  // 1
	uint32 m_IrcChannelGeneral; // 1
	*/
   
	int n;
	char recvline[MAXLINE + 1];

#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2,0), &WSAData);
	//SOCKET sockfd;
	SOCKADDR_IN sin;
	sin.sin_addr.s_addr			= inet_addr(m_SockHost.c_str());
	sin.sin_family				= AF_INET;
	sin.sin_port				= htons(m_SockPort);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//bind(sockfd, (SOCKADDR *)&sin, sizeof(sin));
	erreur = connect(sockfd, (SOCKADDR *)&sin, sizeof(sin));
	if (erreur!=0)
      printf("Session TCP : %d %d\n",erreur,WSAGetLastError());
    else
      printf("setsockopt  : OK\n");
#else
	struct sockaddr_in servaddr; // linux

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // creer socket
	{ 
		perror("Erreur creation socket\n"); 
		exit(1); 
	} 

	bzero(&servaddr, sizeof(servaddr)); // on met la structure du socket a 0 ( ip - port )

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(m_SockPort); //htons: pour convertir le numero de port

	//inet_pton: convertit l'argument ASCII (Ex: 127.0.0.1) dans le bon format
	if (inet_pton(AF_INET, m_SockHost.c_str(), &servaddr.sin_addr) <= 0) 
	{   
		perror("Erreur inet_pton"); 
		exit(1); 
	}

	// etablit connection
	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) 
	{ 
		perror("Erreur connect"); 
		exit(1); 
	} 
#endif

	// Init du client
	m_reponse = "NICK " + m_nick +"\n"; 
	//printf("NICK Reponse: %s",reponse.c_str());
	EnvMsg(m_reponse);
	m_reponse = "USER " + (string)m_username + " " + (string)localhost + " " + (string)serveurhost + " :" + (string)m_usertext+"\n"; 
	EnvMsg(m_reponse);
	//printf("USER Reponse: %s",reponse.c_str());
	
#ifdef WIN32
	while ( (n = recv(sockfd, recvline, MAXLINE,0)) > 0) // windows
#else
	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) // linux
#endif
	{
		recvline[n] = 0; //null terminate

		m_Index = 0;
		m_LigneRecu = "";
		
		while ( m_Index < n )
		{
			if ( recvline[m_Index] == '\n' ) 
			{
				GestionLigne (m_LigneRecu);				
				m_LigneRecu = "";
			} else {
				if ( recvline[m_Index] != '\r')
				{
					m_LigneRecu += recvline[m_Index];
				}
			}
			m_Index++;
		}
	}	

#ifdef WIN32
	closesocket(sockfd); //windows
	WSACleanup(); //windows
#endif

	return(0);

}