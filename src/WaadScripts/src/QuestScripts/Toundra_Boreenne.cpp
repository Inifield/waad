/*
 * WaadAscent Scripts for Ascent MMORPG Server by Crash
 * Copyright (C) 2010 WaadAscent Team <http://arbonne.games-rpg.net/>
 *
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 *
 */

#include "StdAfx.h"
#include "Setup.h"

// ( A VOIR ) Quête : Relever les compteurs par Crash pour WAAD

class SismographedeFrimarraNexus : public GameObjectAIScript
{
public:
	SismographedeFrimarraNexus(GameObject* goinstance) : GameObjectAIScript(goinstance) {};
	static GameObjectAIScript *Create(GameObject * GO) { return new SismographedeFrimarraNexus(GO); }

	void OnActivate(Player *pPlayer)
	{
		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( 11900 );
		if(qle)
			qle->SetMobCount( 0, 1 );
			qle->SendUpdateAddKill(0,0);
			qle->UpdatePlayerFields();
	};

};

class SismographedeFrimarraSud : public GameObjectAIScript
{
public:
	SismographedeFrimarraSud(GameObject* goinstance) : GameObjectAIScript(goinstance) {};
	static GameObjectAIScript *Create(GameObject * GO) { return new SismographedeFrimarraSud(GO); }

	void OnActivate(Player *pPlayer)
	{
		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( 11900 );
		if(qle)
			qle->SetMobCount( 0, 1 );
			qle->SendUpdateAddKill(0,0);
			qle->UpdatePlayerFields();
	};

};

class SismographedeFrimarraNord : public GameObjectAIScript
{
public:
	SismographedeFrimarraNord(GameObject* goinstance) : GameObjectAIScript(goinstance) {};
	static GameObjectAIScript *Create(GameObject * GO) { return new SismographedeFrimarraNord(GO); }

	void OnActivate(Player *pPlayer)
	{
		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( 11900 );
		if(qle)
			qle->SetMobCount( 0, 1 );
			qle->SendUpdateAddKill(0,0);
			qle->UpdatePlayerFields();
	};

};

class SismographedeFrimarraOuest : public GameObjectAIScript
{
public:
	SismographedeFrimarraOuest(GameObject* goinstance) : GameObjectAIScript(goinstance) {};
	static GameObjectAIScript *Create(GameObject * GO) { return new SismographedeFrimarraOuest(GO); }

	void OnActivate(Player *pPlayer)
	{
		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( 11900 );
		if(qle)
			qle->SetMobCount( 0, 1 );
			qle->SendUpdateAddKill(0,0);
			qle->UpdatePlayerFields();
	};

};

void SetupToundraBoreenne(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(188100, &SismographedeFrimarraNexus::Create);
	mgr->register_gameobject_script(188101, &SismographedeFrimarraSud::Create);
	mgr->register_gameobject_script(188102, &SismographedeFrimarraNord::Create);
	mgr->register_gameobject_script(188103, &SismographedeFrimarraOuest::Create);
}