local message = {};
local scan_delay = 3100
local scan_delay2 = 7000
local vierge_die_spawn = 0
local npcid = 28487
local player = 0
local inphase2 = 0

message[1] = "Ton heure n\'est pas venue, champion. L\195\168ve toi, et repars au combat !";

function Vierge_Phase1(Unit, Event)
Unit:RemoveEvents();
if inphase2 == 0 then
local player = Unit:GetClosestPlayer()
if (player ~= nil) then
local player_dist = Unit:GetDistance(player)
if player:IsDead() then
if player_dist < 800 then
player_x = player:GetX();
player_y = player:GetY();
player_z = player:GetZ();
player_o = player:GetO();
Unit:ModifyWalkSpeed(30);
Unit:MoveTo(player_x,player_y,player_z +3,player_o);
Unit:SendChatMessage(12, 0, message[1]);
else
Unit:ReturnToSpawnPoint();
end
Unit:RegisterEvent("Vierge_Phase2", scan_delay2, 1);
end
end
end
Unit:RegisterEvent("Vierge_Phase1", scan_delay, 1);
end

function Vierge_Phase2(Unit, Event)
Unit:RemoveEvents();
local inphase2 = 1
player:ResurrectPlayer();
local max_hp = player:GetMaxHealth();
player:SetHealth(max_hp);
Unit:ReturnToSpawnPoint();
Unit:RegisterEvent("Vierge_Phase1", scan_delay, 1);
local inphase2 = 0
end

RegisterUnitEvent (npcid, 18, "Vierge_Phase1")