/*
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef WOWSERVER_NGLOG_H
#define WOWSERVER_NGLOG_H

#include "Common.h"
#include "Singleton.h"

#include <iostream>
using namespace std; 

class WorldPacket;
class WorldSession;

// code couleur encore utile autre part ?
#ifdef WIN32

#define TRED FOREGROUND_RED | FOREGROUND_INTENSITY
#define TGREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define TYELLOW FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
#define TNORMAL FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE
#define TWHITE TNORMAL | FOREGROUND_INTENSITY
#define TBLUE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define TPURPLE FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY

#else

#define TRED 1
#define TGREEN 2
#define TYELLOW 3
#define TNORMAL 4
#define TWHITE 5
#define TBLUE 6
#define TPURPLE 7

#endif

#define LONGEURLOG 512

extern SERVER_DECL time_t UNIXTIME;		/* update this every loop to avoid the time() syscall! */
extern SERVER_DECL tm g_localTime;
#define LOG_USE_MUTEX

class SERVER_DECL CLog : public Singleton< CLog >
{
#ifdef LOG_USE_MUTEX
	Mutex mutex;
#define LOCK_LOG mutex.Acquire()
#define UNLOCK_LOG mutex.Release();
#else
#define LOCK_LOG 
#define UNLOCK_LOG 
#endif

public:
#ifdef WIN32
	HANDLE stdout_handle, stderr_handle;
#endif  
	int32 log_level;

	CLog()
	{
		log_level = 3;
#ifdef WIN32
		stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
		stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	}

	void Color(unsigned int color)
	{
#ifndef WIN32
		string couleur; // string better that *char >> comme ca pas de warning

		switch (color) 
		{
			case TRED : couleur = "31;1"; break;
			case TGREEN : couleur = "32;1"; break;
			case TYELLOW : couleur = "32;1"; break;
			case TNORMAL : couleur = "30;0"; break;
			case TWHITE : couleur = "37;1"; break;
			case TBLUE : couleur = "34;1"; break;
			default : couleur = "30;0";
		}

		cout << "\E[" << couleur << "m";
#else
		SetConsoleTextAttribute(stdout_handle, (WORD)color);
#endif
 
	}

	/*
	ASCENT_INLINE void Time()
	{
		printf("%02u:%02u ", g_localTime.tm_hour, g_localTime.tm_min);
	}
*/

	void Notice(const char * source, const char * format, ...)
	{

		va_list ap;
		char buffer[LONGEURLOG];
		va_start(ap, format);
		SetConsoleOutputCP(1252);
		vsprintf(buffer,format,ap);
		va_end(ap);
		
		LOCK_LOG;
		cout << "[" << g_localTime.tm_hour << ":" << g_localTime.tm_min << "]";
		cout << " N ";
		if(*source)
		{
			Color(TWHITE);
			cout << source << ": ";
			Color(TNORMAL);
		}

		if(strlen(format) > 300)
		{
			Color(TRED);
			cout << "(Message trop long > 300 caracteres!)" << endl;

			UNLOCK_LOG;
			Color(TNORMAL);
			return;
		}

		cout << buffer << endl; 
		Color(TNORMAL);
		UNLOCK_LOG;
	}

	void Warning(const char * source, const char * format, ...)
	{
		if(log_level < 2) return;

		va_list ap;
		char buffer[LONGEURLOG];
		va_start(ap, format);
		SetConsoleOutputCP(1252);
		vsprintf(buffer,format,ap);
		va_end(ap);
	
		LOCK_LOG;
		cout << "[" << g_localTime.tm_hour << ":" << g_localTime.tm_min << "]";
		Color(TYELLOW);
		cout << " W ";
		if(*source)
		{
			Color(TWHITE);
			cout << source << ": ";
			Color(TYELLOW);
		}

		cout << buffer << endl; 
		Color(TNORMAL);
		UNLOCK_LOG;
	}

	void Success(const char * source, const char * format, ...)
	{
		if(log_level < 2) return;

		va_list ap;
		char buffer[LONGEURLOG];
		va_start(ap, format);
		SetConsoleOutputCP(1252);
        vsprintf(buffer,format,ap);
		va_end(ap);
		
		LOCK_LOG;
		cout << "[" << g_localTime.tm_hour << ":" << g_localTime.tm_min << "]";
		Color(TGREEN);
		cout << " S ";
		if(*source)
		{
			Color(TWHITE);
			cout << source << ": ";
			Color(TGREEN);
		}
		
		cout << buffer << endl; 
		Color(TNORMAL);
		UNLOCK_LOG;
	}

	void zPvP(const char * source, const char * format, ...)
	{
		if(log_level < 1) return;

		va_list ap;
		char buffer[LONGEURLOG];
		va_start(ap, format);
		SetConsoleOutputCP(1252);
        vsprintf(buffer,format,ap);
		va_end(ap);
	
		LOCK_LOG;
		cout << "[" << g_localTime.tm_hour << ":" << g_localTime.tm_min << "]";
		Color(TBLUE);
		cout << " S ";
		if(*source)
		{
			Color(TWHITE);
			cout << source << ": ";
			Color(TBLUE);
		}
		
		cout << buffer << endl; 
		Color(TNORMAL);
		UNLOCK_LOG;
	}

	void Error(const char * source, const char * format, ...)
	{
		if(log_level < 1) return;

		va_list ap;
		char buffer[LONGEURLOG];
		va_start(ap, format);
		SetConsoleOutputCP(1252);
        vsprintf(buffer,format,ap);
		va_end(ap);
		
		LOCK_LOG;
		cout << "[" << g_localTime.tm_hour << ":" << g_localTime.tm_min << "]";
		Color(TRED);
		cout << " E ";
		if(*source)
		{
			Color(TWHITE);
			cout << source << ": ";
			Color(TRED);
		}
		
		cout << buffer << endl; 
		Color(TNORMAL);
		UNLOCK_LOG;
	}

	void Debug(const char * source, const char * format, ...)
	{
		if(log_level < 3) return;

		va_list ap;
		char buffer[LONGEURLOG];
		va_start(ap, format);
		SetConsoleOutputCP(1252);
        vsprintf(buffer,format,ap);
		va_end(ap);
		
		LOCK_LOG;
		cout << "[" << g_localTime.tm_hour << ":" << g_localTime.tm_min << "]";
		Color(TBLUE);
		cout << " D ";
		if(*source)
		{
			Color(TWHITE);
			cout << source << ": ";
			Color(TBLUE);
		}
		
		cout << buffer << endl; 
		Color(TNORMAL);
		UNLOCK_LOG;
	}
	
	void Line()
	{
		LOCK_LOG;
		putchar('\n');
		UNLOCK_LOG;
	}

#define LARGERRORMESSAGE_ERROR 1
#define LARGERRORMESSAGE_WARNING 2

	void LargeErrorMessage(uint32 Colour, ...)
	{
		std::vector<char*> lines;
		char * pointer;
		va_list ap;
		va_start(ap, Colour);
		
		size_t i,j,k;
		pointer = va_arg(ap, char*);
		while( pointer != NULL )
		{
			lines.push_back( pointer );
			pointer = va_arg(ap, char*);
		}

		LOCK_LOG;

		if( Colour == LARGERRORMESSAGE_ERROR )
			Color(TRED);
		else
			Color(TYELLOW);

		printf("*********************************************************************\n");
		printf("*                        MAJOR ERROR/WARNING                        *\n");
		printf("*                        ===================                        *\n");

		for(std::vector<char*>::iterator itr = lines.begin(); itr != lines.end(); ++itr)
		{
			i = strlen(*itr);
			j = (i<=65) ? 65 - i : 0;

			printf("* %s", *itr);
			for( k = 0; k < j; ++k )
			{
				printf(" ");
			}

			printf(" *\n");
		}

		printf("*********************************************************************\n");

#ifdef WIN32
		std::string str = "MAJOR ERROR/WARNING:\n";
		for(std::vector<char*>::iterator itr = lines.begin(); itr != lines.end(); ++itr)
		{
			str += *itr;
			str += "\n";
		}

		MessageBox(0, str.c_str(), "Error", MB_OK);
#else
		printf("Sleeping for 5 seconds.\n");
		Sleep(5000);
#endif

		Color(TNORMAL);
		UNLOCK_LOG;
	}
};

#define Log CLog::getSingleton()

#endif

