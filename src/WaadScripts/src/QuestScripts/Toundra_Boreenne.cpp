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
#include "Base.h"

// ( A VOIR ) Quête : Relever les compteurs par Crash pour WAAD

// Bury Those Cockroaches!
class SeaforiumDepthCharge : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(SeaforiumDepthCharge, MoonScriptCreatureAI);
		SeaforiumDepthCharge(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			SetCanMove(false);
			SetCanEnterCombat(false);
			_unit->_setFaction();
		}

		void OnLoad()
		{
			if(!_unit->GetSummon())
				return;

			Unit* summoner = static_cast<Pet *>(_unit)->GetOwner();

			if(summoner != NULL)
			{
				if(summoner->IsPlayer())
				{
					Player* p = static_cast<Player *>(summoner);
					if(p->HasQuest(11608))
					{
						GameObject* pSinkhole = p->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(p->GetPositionX(), p->GetPositionY(), p->GetPositionZ(), 300171);
						if(pSinkhole != NULL)
						{
							_unit->CastSpell(_unit, 45502, true);

							float posX = pSinkhole->GetPositionX();
							QuestLogEntry *qle = p->GetQuestLogForEntry( 11602 );
							if( (qle && posX == 2657.13f) || (qle && posX == 2716.02f) || (qle && posX == 2877.96f) || (qle && posX == 2962.16f) )
							{
								qle->SetMobCount( 0, 1 );
								qle->SendUpdateAddKill(0,0);
								qle->UpdatePlayerFields();
							}
						}
					}
				}
			}
			_unit->Despawn(500, 0);
		}
};

// Neutralizing the Cauldrons
#define CN_PURIFYING_TOTEM	25494

class PurifyingTotemAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(PurifyingTotemAI, MoonScriptCreatureAI);
		PurifyingTotemAI(Creature* pCreature) : MoonScriptCreatureAI(pCreature)
		{
			SetCanEnterCombat(false);
			SetCanMove(false);
			Despawn(8000, 0);
		}

};

// Appel aux armes !
class BellRope : public GameObjectAIScript
{
	public:
		BellRope(GameObject* goinstance) : GameObjectAIScript(goinstance) {};
		static GameObjectAIScript *Create(GameObject * GO) { return new BellRope(GO); }

		void OnActivate(Player* pPlayer)
		{
			QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( 11965 );
			if(qle)
				qle->SetMobCount( 0, 1 );
				qle->SendUpdateAddKill(0,0);
				qle->UpdatePlayerFields();
		};

};

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

// Cutting Off the Source
class NerubarEggSac : public GameObjectAIScript
{
	public:
		NerubarEggSac(GameObject* goinstance) : GameObjectAIScript(goinstance) {};
		static GameObjectAIScript *Create(GameObject * GO) { return new NerubarEggSac(GO); }

		void OnActivate(Player* pPlayer)
		{
			QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( 11602 );
			if(qle)
			{
				qle->SetMobCount( 0, 1 );
				qle->SendUpdateAddKill(0,0);
				qle->UpdatePlayerFields();
			}

			_gameobject->SetState(1);
			_gameobject->SetState(0);
			_gameobject->Despawn(500);
		};
};



// Hatching a Plan
class BlueDragonEgg : public GameObjectAIScript
{
	public:
		BlueDragonEgg(GameObject* goinstance) : GameObjectAIScript(goinstance) {};
		static GameObjectAIScript *Create(GameObject * GO) { return new BlueDragonEgg(GO); }

		void OnActivate(Player* pPlayer)
		{
			QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( 11936 );
			if(qle)
			{
				qle->SetMobCount( 0, 1 );
				qle->SendUpdateAddKill(0,0);
				qle->UpdatePlayerFields();
			}

			_gameobject->SetState(1);
			_gameobject->SetState(0);
			_gameobject->Despawn(500);
		}
};
enum eFizzcrank
{
    NPC_FIZZCRANK               = 25590,

    GOSSIP_TEXTID_FIZZCRANK1    = 12456,
    GOSSIP_TEXTID_FIZZCRANK2    = 12457,
    GOSSIP_TEXTID_FIZZCRANK3    = 12458,
    GOSSIP_TEXTID_FIZZCRANK4    = 12459,
    GOSSIP_TEXTID_FIZZCRANK5    = 12460,
    GOSSIP_TEXTID_FIZZCRANK6    = 12461,
    GOSSIP_TEXTID_FIZZCRANK7    = 12462,
    GOSSIP_TEXTID_FIZZCRANK8    = 12463,
    GOSSIP_TEXTID_FIZZCRANK9    = 12464,

    QUEST_THE_MECHAGNOMES       = 11708
};

#define GOSSIP_ITEM_GO_ON   "En avant."
#define GOSSIP_ITEM_TELL_ME "Dites-moi ce qui se passe ici, Fizzcrank."

class FizzcrankGossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* pPlayer)
		{
			GossipMenu* Menu;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, pPlayer);

			QuestLogEntry *qle = pPlayer->GetQuestLogForEntry( QUEST_THE_MECHAGNOMES );
			if(qle)
				Menu->AddItem(0, GOSSIP_ITEM_TELL_ME, 1);

			Menu->SendTo(pPlayer);
		}

		void GossipSelectOption(Object* pObject, Player*  pPlayer, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 1:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK1, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 2);
					Menu->SendTo(pPlayer);
					break;
				case 2:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK2, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 3);
					Menu->SendTo(pPlayer);
					break;
				case 3:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK3, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 4);
					Menu->SendTo(pPlayer);
					break;
				case 4:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK4, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 5);
					Menu->SendTo(pPlayer);
					break;
				case 5:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK5, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 6);
					Menu->SendTo(pPlayer);
					break;
				case 6:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK6, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 7);
					Menu->SendTo(pPlayer);
					break;
				case 7:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK7, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 8);
					Menu->SendTo(pPlayer);
					break;
				case 8:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK8, pPlayer);
					Menu->AddItem(0, GOSSIP_ITEM_GO_ON, 9);
					Menu->SendTo(pPlayer);
					break;
				case 9:
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), GOSSIP_TEXTID_FIZZCRANK9, pPlayer);
					Menu->SendTo(pPlayer);
					break;
			}
		}

};

#define GOSSIP_ITEM_FREE_FLIGHT "J'aimerais utliser le passage vers le Bouclier de Transitus."
#define GOSSIP_ITEM_FLIGHT      "Puis-je utiliser un Drake pour m'envoler ailleurs "

enum eSurristrasz
{
    NPC_SURRISTRASZ             = 24795,

    SPELL_ABMER_TO_COLDARRA     = 46064
};
class SurristraszGossip : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* pPlayer)
		{
			GossipMenu* Menu;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, pPlayer);

			Menu->AddItem(0, GOSSIP_ITEM_FREE_FLIGHT, 1);
			Menu->AddItem(3, GOSSIP_ITEM_FLIGHT, 2);

			Menu->SendTo(pPlayer);
		};

		void GossipSelectOption(Object* pObject, Player*  pPlayer, uint32 Id, uint32 IntId, const char* Code)
		{
			if(!pObject->IsCreature())
				return;

			switch(IntId)
			{
				case 1:
					pPlayer->Gossip_Complete();
					pPlayer->CastSpell(pPlayer, SPELL_ABMER_TO_COLDARRA, true);
					break;
				case 2:
					pPlayer->GetSession()->SendTaxiList(static_cast <Creature *>(pObject));
					break;
			};
		};
};

void SetupToundraBoreenne(ScriptMgr * mgr)
{
	mgr->register_creature_script  (25494,  &PurifyingTotemAI::Create);
	mgr->register_creature_script(25401, &SeaforiumDepthCharge::Create);

	mgr->register_gameobject_script(188163, &BellRope::Create);
	mgr->register_gameobject_script(188100, &SismographedeFrimarraNexus::Create);
	mgr->register_gameobject_script(188101, &SismographedeFrimarraSud::Create);
	mgr->register_gameobject_script(188102, &SismographedeFrimarraNord::Create);
	mgr->register_gameobject_script(188103, &SismographedeFrimarraOuest::Create);
	mgr->register_gameobject_script(187655, &NerubarEggSac::Create);	
	mgr->register_gameobject_script(187655, &BlueDragonEgg::Create);

	GossipScript* fGossip = new FizzcrankGossip;
	mgr->register_gossip_script(NPC_FIZZCRANK, fGossip);
	GossipScript* sGossip = new SurristraszGossip;
	mgr->register_gossip_script(NPC_SURRISTRASZ, sGossip);
}