#ifndef SKILL_NAME_MGR_H
#define SKILL_NAME_MGR_H

#include "StdAfx.h"

class SkillNameMgr
{
public:
	char **SkillNames;
	uint32 maxskill;

	SkillNameMgr()
	{
		DBCFile SkillDBC;
		
		char _bufftemp_[1024];

		sprintf(_bufftemp_,"%s/SkillLine.dbc",DBCPath);
		if( !SkillDBC.open( _bufftemp_ ) )
		{
			Log.Error( "SkillNameMgr", "Cannot find file ./DBC/SkillLine.dbc" );
			return;
		}
        Log.Notice("DBC:","Loading %s",_bufftemp_); 
		//This will become the size of the skill name lookup table
		maxskill = SkillDBC.getRecord(SkillDBC.getRecordCount()-1).getUInt(0);

		//SkillNames = (char **) malloc(maxskill * sizeof(char *));
		SkillNames = new char * [maxskill+1]; //(+1, arrays count from 0.. not 1.)
		memset(SkillNames,0,(maxskill+1) * sizeof(char *));

		for(uint32 i = 0; i < SkillDBC.getRecordCount(); ++i)
		{
			unsigned int SkillID = SkillDBC.getRecord(i).getUInt(0);
			const char *SkillName = SkillDBC.getRecord(i).getString(3);

			SkillNames[SkillID] = new char [strlen(SkillName)+1];
			//When the DBCFile gets cleaned up, so does the record data, so make a copy of it..
			memcpy(SkillNames[SkillID],SkillName,strlen(SkillName)+1);
		}
	}
	~SkillNameMgr()
	{
		for(uint32 i = 0;i<=maxskill;i++)
		{
			if(SkillNames[i] != 0)
				delete SkillNames[i];
		}
		delete SkillNames;
	}
};
// forward
class GossipScript;
struct SkillNames
{
	uint32 Entry;
	char * Name;
	uint32 Maxvalue;

	std::string lowercase_name;
	GossipScript * gossip_script;
};

#endif

