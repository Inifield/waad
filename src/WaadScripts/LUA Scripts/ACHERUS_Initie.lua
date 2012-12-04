local npcid = 28406
local delay = 1500
local delay2 = 1500
local player_duel_acherus = 0

function Initie_Duel(Unit, event)
Unit:SetNPCFlags(1);
end

function Initie_Duel_OnGossip(Unit, Event, Player)
Unit:GossipCreateMenu(100, Player, 0);
Unit:GossipMenuAddItem(0, "Je vous d\195\169fie, chevalier de la mort !", 1, 0);
Unit:GossipSendMenu(Player);
end

function Initie_Duel_OnSelect(Unit, Event, Player, MenuId, id, Code)
if (id == 1) then
player_duel_acherus = Unit:GetClosestPlayer();
Unit:RegisterEvent("Start_Duel1", 1, 1);
local randomtext_initie = math.random(1,5);
if randomtext_initie == 1 then
Unit:SendChatMessage(12, 0, "Le roi-liche va voir qui est son vrai champion aujourd\'hui !")
elseif randomtext_initie == 2 then
Unit:SendChatMessage(12, 0, "Souvenez-vous bien de ce jour. Car en ce jour je vais vous la mettre grave.")
elseif randomtext_initie == 3 then
Unit:SendChatMessage(12, 0, "Envoyez les larmes...")
elseif randomtext_initie == 4 then
Unit:SendChatMessage(12, 0, "Vous avez d\195\169fi\195\169 la mort elle-m\195\170me !")
elseif randomtext_initie == 5 then
Unit:SendChatMessage(12, 0, "Vous n\'avez aucune chance.")
end
Unit:GossipComplete(Player);
end
end

function Start_Duel1(Unit, Event)
local mst_x = Unit:GetSpawnX();
local mst_y = Unit:GetSpawnY();
local mst_z = Unit:GetSpawnZ();
local mst_o = Unit:GetSpawnO();
local rand = math.random(1,3);
Unit:SpawnGameObject(191126, mst_x + rand, mst_y + rand, mst_z -0.4, mst_o + rand, 30000);
player_duel_acherus:SendBroadcastMessage("Le duel va commencer dans...");
Unit:RegisterEvent("Start_Duel", delay2, 1);
end
function Start_Duel(Unit, Event)
player_duel_acherus:SendBroadcastMessage("3 ...");
Unit:RegisterEvent("Start_Duel2", delay2, 1);
end
function Start_Duel2(Unit, Event)
player_duel_acherus:SendBroadcastMessage("2 ...");
Unit:RegisterEvent("Start_Duel3", delay2, 1);
end
function Start_Duel3(Unit, Event)
player_duel_acherus:SendBroadcastMessage("1 ...");
Unit:RegisterEvent("Start_Duel4", delay2, 1);
end
function Start_Duel4(Unit, Event)
Unit:SetFaction(14);
Unit:SetTauntedBy(player_duel_acherus);
end

RegisterUnitEvent(npcid, 18, "Initie_Duel")
RegisterUnitGossipEvent(npcid, 1, "Initie_Duel_OnGossip")
RegisterUnitGossipEvent(npcid, 2, "Initie_Duel_OnSelect")