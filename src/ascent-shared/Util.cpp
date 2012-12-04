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

#include "Common.h"

using namespace std;

vector<string> StrSplit(const string &src, const string &sep)
{
	vector<string> r;
	string s;
	for (string::const_iterator i = src.begin(); i != src.end(); i++) {
		if (sep.find(*i) != string::npos) {
			if (s.length()) r.push_back(s);
			s = "";
		} else {
			s += *i;
		}
	}
	if (s.length()) r.push_back(s);
	return r;
}

void SetThreadName(const char* format, ...)
{
	// This isn't supported on nix?
	va_list ap;
	va_start(ap, format);

#ifdef WIN32

	char thread_name[200];
	vsnprintf(thread_name, 200, format, ap);

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.dwThreadID = GetCurrentThreadId();
	info.dwFlags = 0;
	info.szName = thread_name;

	__try
	{
#ifdef _WIN64
		RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (ULONG_PTR*)&info);
#else
		RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info);
#endif
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{

	}

#endif

	va_end(ap);
}

time_t convTimePeriod ( uint32 dLength, char dType )
{
	time_t rawtime = 0;
	if (dLength == 0)
		return rawtime;
	struct tm * ti = localtime( &rawtime );
	switch(dType)
	{
		case 'h':		// hours
			ti->tm_hour += dLength;
			break;
		case 'd':		// days
			ti->tm_mday += dLength;
			break;
		case 'w':		// weeks
			ti->tm_mday += 7 * dLength;
			break;
		case 'm':		// months
			ti->tm_mon += dLength;
			break;
		case 'y':		// years
			// are leap years considered ? do we care ?
			ti->tm_year += dLength;
			break;
		default:		// minutes
			ti->tm_min += dLength;
			break;
	}
	return mktime(ti);
}
int32 GetTimePeriodFromString(const char * str)
{
	uint32 time_to_ban = 0;
	char * p = (char*)str;
	uint32 multiplier;
	string number_temp;
	uint32 multipliee;
	number_temp.reserve(10);

	while(*p != 0)
	{
		// always starts with a number.
		if(!isdigit(*p))
			break;

		number_temp.clear();
		while(isdigit(*p) && *p != 0)
		{
			number_temp += *p;
			++p;
		}

		// try and find a letter
		if(*p == 0)
			break;

		// check the type
		switch(tolower(*p))
		{
		case 'y':
			multiplier = TIME_YEAR;		// eek!
			break;

		case 'm':
			multiplier = TIME_MONTH;
			break;

		case 'd':
			multiplier = TIME_DAY;
			break;

		case 'h':
			multiplier = TIME_HOUR;
			break;

		default:
			return -1;
			break;
		}

		++p;
		multipliee = atoi(number_temp.c_str());
		time_to_ban += (multiplier * multipliee);
	}

	return time_to_ban;
}

const char * szDayNames[7] = {
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

const char * szMonthNames[12] = {
	"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

void MakeIntString(char * buf, int num)
{
	if(num<10)
	{
		buf[0] = '0';
		//itoa(num, &buf[1], 10);
		sprintf(&buf[1], "%u", num);
	}
	else
	{
		//itoa(num,buf,10);
		sprintf(buf,"%u",num);
	}
}

void MakeIntStringNoZero(char * buf, int num)
{
	//itoa(num,buf,10);
	sprintf(buf,"%u",num);
}

string ConvertTimeStampToString(uint32 timestamp)
{
	int seconds = (int)timestamp;
	int mins=0;
	int hours=0;
	int days=0;
	int months=0;
	int years=0;
	if(seconds >= 60)
	{
		mins = seconds / 60;
		if(mins)
		{
			seconds -= mins*60;
			if(mins >= 60)
			{
				hours = mins / 60;
				if(hours)
				{
					mins -= hours*60;
					if(hours >= 24)
					{
						days = hours/24;
						if(days)
						{
							hours -= days*24;
							if(days >= 30)
							{
								months = days / 30;
								if(months)
								{
									days -= months*30;
									if(months >= 12)
									{
										years = months / 12;
										if(years)
										{
											months -= years*12;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	char szTempBuf[100];
	string szResult;

	if(years) {
		MakeIntStringNoZero(szTempBuf, years);
		szResult += szTempBuf;
		szResult += " years, ";
	}

	if(months) {
		MakeIntStringNoZero(szTempBuf, months);
		szResult += szTempBuf;
		szResult += " months, ";
	}

	if(days) {
		MakeIntStringNoZero(szTempBuf, days);
		szResult += szTempBuf;
		szResult += " days, ";
	}

	if(hours) {
		MakeIntStringNoZero(szTempBuf, hours);
		szResult += szTempBuf;
		szResult += " hours, ";
	}

	if(mins) {
		MakeIntStringNoZero(szTempBuf, mins);
		szResult += szTempBuf;
		szResult += " minutes, ";
	}

	if(seconds) {
		MakeIntStringNoZero(szTempBuf, seconds);
		szResult += szTempBuf;
		szResult += " seconds";
	}

	return szResult;
}

string ConvertTimeStampToDataTime(uint32 timestamp)
{
	char szTempBuf[100];
	time_t t = (time_t)timestamp;
	tm * pTM = localtime(&t);

	string szResult;
	szResult += szDayNames[pTM->tm_wday];
	szResult += ", ";

	MakeIntString(szTempBuf, pTM->tm_mday);
	szResult += szTempBuf;
	szResult += " ";

	szResult += szMonthNames[pTM->tm_mon];
	szResult += " ";

	MakeIntString(szTempBuf, pTM->tm_year+1900);
	szResult += szTempBuf;
	szResult += ", ";
	MakeIntString(szTempBuf, pTM->tm_hour);
	szResult += szTempBuf;
	szResult += ":";
	MakeIntString(szTempBuf, pTM->tm_min);
	szResult += szTempBuf;
	szResult += ":";
	MakeIntString(szTempBuf, pTM->tm_sec);
	szResult += szTempBuf;

	return szResult;
}

// returns true if the ip hits the mask, otherwise false
bool ParseCIDRBan(unsigned int IP, unsigned int Mask, unsigned int MaskBits)
{
	// CIDR bans are a compacted form of IP / Submask
	// So 192.168.1.0/255.255.255.0 would be 192.168.1.0/24
	// IP's in the 192.168l.1.x range would be hit, others not.
	unsigned char * source_ip = (unsigned char*)&IP;
	unsigned char * mask = (unsigned char*)&Mask;
	int full_bytes = MaskBits / 8;
	int leftover_bits = MaskBits % 8;
	//int byte;

	// sanity checks for the data first
	if( MaskBits > 32 )
		return false;

	// this is the table for comparing leftover bits
	static const unsigned char leftover_bits_compare[9] = {
		0x00,			// 00000000
		0x80,			// 10000000
		0xC0,			// 11000000
		0xE0,			// 11100000
		0xF0,			// 11110000
		0xF8,			// 11111000
		0xFC,			// 11111100
		0xFE,			// 11111110
		0xFF,			// 11111111 - This one isn't used
	};

	// if we have any full bytes, compare them with memcpy
	if( full_bytes > 0 )
	{
		if( memcmp( source_ip, mask, full_bytes ) != 0 )
			return false;
	}

	// compare the left over bits
	if( leftover_bits > 0 )
	{
		if( ( source_ip[full_bytes] & leftover_bits_compare[leftover_bits] ) !=
			( mask[full_bytes] & leftover_bits_compare[leftover_bits] ) )
		{
			// one of the bits does not match
			return false;
		}
	}

	// all of the bits match that were testable
	return true;
}

unsigned int MakeIP(const char * str)
{
	unsigned int bytes[4];
	unsigned int res;
	if( sscanf(str, "%u.%u.%u.%u", &bytes[0], &bytes[1], &bytes[2], &bytes[3]) != 4 )
		return 0;

	res = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
	return res;
}

string ConvertMSTimeToString(uint32 timestamp)
{
 char szTempBuf[255];
 string szResult;

 uint32 centieme = (uint32)(timestamp % 1000);
 uint32 seconds  = (uint32)((uint32)((float)timestamp/1000.) % 60);
 uint32 reste    = (uint32)(((float)timestamp/1000.) / 60.);
 uint32 mins     = (uint32)(reste % 60);
 reste           = (uint32)((float)reste / 60.);
 uint32 hours    = (uint32)(reste % 24);
 reste           = (uint32)((float)reste / 24.);
 uint32 days     = reste;

  // Formatage de la date.
 sprintf(szTempBuf,"%dJ %02dH %02dmin %02dsec:%03d",days,hours,mins,seconds,centieme);
 szResult = szTempBuf;

 return szResult;
}
// Original d'un certain tiger222, écrit en php, traduit en C (Branruz)
string ConvertUnixTimeToString(uint32 timestamp)
{
 // La fonction entier() indiquera le nombre entier d'unités de temps.
 //
 // Principe :
 // La plus grande unité de temps invariable est le jour ; les mois
 // (28, 29, 30 ou 31 jours) et les années (365 ou 366) sont variables.
 // La première étape consiste à décompter le nombre de périodes
 // quadriennales bisextiles.
 //
 // La date au format Unix étant le nombre de secondes depuis
 // le 1er Janvier 1970, et la première année bisextile étant
 // 1972, nous calculons le nombre de secondes entre
 // le 01/01/1970 et le 29/02/1972.

 // Définitions des variables :
 char szTempBuf[255];
 string szResult;
 uint32 vingtSixMois = 60 * 60 * 24 * 790;
 uint32 quatreAns = 126230400;
 uint32 unAn = 31536000;
 uint32 unJour = 86400;
 uint32 tranche = 0;
 uint32 reste = 0;
 uint32 nbAnnee = 0;
 uint32 nbJours = 0;
 uint32 Mois[12],MoisN[12];

 uint32 seconds = 0;
 uint32 mins=0;
 uint32 hours=0;
 uint32 days=0;
 uint32 months=0;
 uint32 years=0;
 
 if (timestamp > vingtSixMois) 
 {
  // 1er calcul : le nombre de périodes quadriennales bisextiles.
  tranche = (timestamp - vingtSixMois) / quatreAns;

 // Calcul des secondes restantes :
  reste = timestamp - vingtSixMois - (quatreAns * tranche);
 }
 else 
 {
  reste = timestamp;
 }

 // 2ème calcul : le nombre d'années.
 nbAnnee = reste / unAn;

 // Calcul des secondes restantes :
 reste = reste - (nbAnnee * unAn);

 // 3ème calcul : nombre de jours.
 nbJours = reste / unJour;
 days = nbJours + 1;

 // Nombre de jours par mois.
 if (timestamp <= vingtSixMois) 
 {
  Mois[0] = 31; // Janvier
  if (nbAnnee == 2) Mois[1] = 29; // Février
  else              Mois[1] = 28; // Février
  Mois[2] = 31; // Mars
  Mois[3] = 30; // Avril
  Mois[4] = 31; // Mai
  Mois[5] = 30; // Juin
  Mois[6] = 31; // Juillet
  Mois[7] = 31; // Août
  Mois[8] = 30; // Septembre
  Mois[9] = 31; // Octobre
  Mois[10] = 30; // Novembre
  Mois[11] = 31; // Décembre

  // Numéro du mois.
  MoisN[0] = 1; // Janvier
  MoisN[1] = 2; // Février
  MoisN[2] = 3; // Mars
  MoisN[3] = 4; // Avril
  MoisN[4] = 5; // Mai
  MoisN[5] = 6; // Juin
  MoisN[6] = 7; // Juillet
  MoisN[7] = 8; // Août
  MoisN[8] = 9; // Septembre
  MoisN[9] = 10; // Octobre
  MoisN[10] = 11; // Novembre
  MoisN[11] = 12; // Décembre
 }
 else 
 {
  Mois[0] = 31; // Mars
  Mois[1] = 30; // Avril
  Mois[2] = 31; // Mai
  Mois[3] = 30; // Juin
  Mois[4] = 31; // Juillet
  Mois[5] = 31; // Août
  Mois[6] = 30; // Septembre
  Mois[7] = 31; // Octobre
  Mois[8] = 30; // Novembre
  Mois[9] = 31; // Décembre
  Mois[10] = 31; // Janvier
  Mois[11] = 28; // Février

  // Numéro du mois.
  MoisN[0] = 3; // Mars
  MoisN[1] = 4; // Avril
  MoisN[2] = 5; // Mai
  MoisN[3] = 6; // Juin
  MoisN[4] = 7; // Juillet
  MoisN[5] = 8; // Août
  MoisN[6] = 9; // Septembre
  MoisN[7] = 10; // Octobre
  MoisN[8] = 11; // Novembre
  MoisN[9] = 12; // Décembre
  MoisN[10] = 1; // Janvier
  MoisN[11] = 2; // Février
 }

 // Détermination du numéro du mois en cours.
 for (int i = 0; i < 12; ++i) 
 {
  if (days > Mois[i]) days = days - Mois[i];
  else if (days == Mois[i]) 
  {
   months = MoisN[i];
   break;
  }
  else 
  {
   months = MoisN[i];
   break;
  }
 }

 // Détermination de l'année.
 if (timestamp > vingtSixMois) 
 {
  if (nbJours <= 306) 
  {
   if (tranche < 1) years = 1972 + nbAnnee;
   else             years = (tranche * 4) + nbAnnee + 1972;
  }
  else 
  {
   if (tranche < 1) years = 1972 + 1 + nbAnnee;
   else             years = (tranche * 4) + nbAnnee + 1972 + 1;
  }
 }
 else years = 1970 + nbAnnee;
 
 // Calcul des secondes restantes :
 reste = reste - (nbJours * unJour);

 // 4ème calcul : nombre d'heures.
 hours = reste / 3600;

 // Calcul des secondes restantes :
 reste = reste - (hours * 3600);

 // 5ème calcul : nombre de minutes.
 mins = reste / 60;

 // Calcul des secondes restantes :
 seconds = reste - (mins * 60);

 // Formatage de la date.
 if((days == 1) && (months==1) && (years==1970) ) sprintf(szTempBuf,"%02d:%02d:%02d",hours,mins,seconds);
 else                                             sprintf(szTempBuf,"%02d:%02d:%02d (%02d-%02d-%04d)",hours,mins,seconds,days,months,years);
 szResult = szTempBuf;

 return szResult;
}