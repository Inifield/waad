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
#ifndef _WAAD_IRC_RELAY_
#define _WAAD_IRC_RELAY_

class WaadIrcRelay : public Singleton < WaadIrcRelay >
{

 public:
	WaadIrcRelay()
	{
		m_SockPort  = 0;
		m_SockHost  = "127.0.0.1";
		m_username  = "WaadIrcServerRelay";
		m_nick      = "BotFromMyWowServer";
		m_usertext  = "Waad World Server Relay actif.";
		m_BotChan   = "#chat_to_join";
		m_connected = false;
	};
	~WaadIrcRelay();

	int WaadBotLauncher(void);
	void SendToChatIrc(char *_texte);

	// Methodes
	void SetUserName(char * _username) { m_username = _username; }
	void SetNick(char * _nick) { m_nick = _nick; }
	void SetUserText(char * _usertext) { m_usertext = _usertext; }
	void SetChannel(char * _BotChan) { m_BotChan = _BotChan; }
	void SetPort(int _SockPort) { m_SockPort = _SockPort; }
	void SetHost(char * _SockHost) { m_SockHost = _SockHost; }

	bool IsConnect(void) { return(m_connected);  }

private:
	string m_username;
    string m_nick;
    string m_usertext;
	// canal pour le bot 
    string m_BotChan;
	
	int m_SockPort ;
    string m_SockHost;
	string m_arg[100];

	bool m_connected;

	// HOSTENT* HostName; 
    //Pour le Nom de domaine	
    // HostName = gethostbyname(SockHostName.c_str());	
    // SockHost = inet_ntoa(*((struct in_addr * )HostName->h_addr));

    //var socket 
    #ifdef WIN32
     SOCKET sockfd; // windows
    #else
     int sockfd; // linux
    #endif
    // info interne au bot
	string m_reponse;
    string m_LigneRecu;
    int m_Index;

	//prot flood ...
    string m_flood_m1,m_flood_m2,m_flood_m3;

	string RNick(string addr);
	string RUsername(string addr);	
	string RHost(string addr);
	int EnvMsg (string a_envoyer);
	int explode (string donnees, string separateur);
	string StrUp(string strToConvert);
	string StrLo(string strToConvert);
	char JoinCanal();
	int GestionLigne ( string buffer_serveur );
};

#endif

