// 
// Programmeur: AzolexX 

#include "StdAfx.h"
#include "Setup.h"

#define STONEID 60000
#define MOUNTDISPLAYID 23647

class SCRIPT_DECL MountStone : public GossipScript
{
    public:
        void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
        void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
        void GossipEnd(Object * pObject, Player* Plr);
        void Destroy()
        {
            delete this;
        }
};

void MountStone::GossipHello(Object* pObject, Player * Plr, bool AutoSend)
{
    if(Plr->CombatStatus.IsInCombat())
    {
        Plr->BroadcastMessage("Impossible de monter sur la monture!");
        return;
    }

    GossipMenu *Menu;
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);

    Menu->AddItem(0, "Je veux monter!", 1);
    Menu->AddItem(0, "Je veux descendre!", 2);

    if(AutoSend)
        Menu->SendTo(Plr);
}

void MountStone::GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
    switch(IntId)
    {
        case 1:
        {
            Plr->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, MOUNTDISPLAYID);
            Plr->CastSpell(Plr, 27879, true);
        }break;
    
        case 2:
        {
            Plr->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 0);
            Plr->RemoveAura( 27879 );
        }break;
    }
    
    Plr->Gossip_Complete();
}
void MountStone::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

void SetupMountStone(ScriptMgr * mgr)
{
    GossipScript * gs = (GossipScript*) new MountStone();
    mgr->register_item_gossip_script(STONEID, gs);
}