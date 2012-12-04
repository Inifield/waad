local scan_delay = 1500
local npcid = 29581 --Téléportation - Coeur -> Hall
local npcid2 = 29580 --Téléportation - Hall -> Coeur
local npcid3 = 29589 --Téléportation - Coeur -> Hall (EPL)
local npcid4 = 29588 --Téléportation - Hall -> Coeur (EPL)
local go1 = 190942

function Action_Acherus_DeathPortal (pGameObject, event, pMisc)
Unit:RemoveEvents();
pMisc:Teleport (0, 2358.372070, -5662.258789, 382.258881, 0.561560)
end

RegisterGameObjectEvent(go1, 4, "Action_Acherus_DeathPortal")

function Action_Acherus_Portal(Unit, event)
Unit:RemoveEvents();
local player = Unit:GetClosestPlayer()
if (player ~= nil) then
local player_dist = Unit:GetDistance(player)
if player_dist < 6 then
if player:GetZ() < 400 then
player:Teleport(609, 2419.491699, -5621.224121, 420.643707, 3.732219)
end
end
end
Unit:RegisterEvent("Action_Acherus_Portal", scan_delay, 1)
end

RegisterUnitEvent(npcid2, 18, "Action_Acherus_Portal")

function Action_Acherus2_Portal(Unit, event)
Unit:RemoveEvents();
local player = Unit:GetClosestPlayer()
if (player ~= nil) then
local player_dist = Unit:GetDistance(player)
if player_dist < 6 then
if player:GetZ() > 400 then
player:Teleport(609, 2401.895508, -5633.643555, 377.021362, 3.689022)
end
end
end
Unit:RegisterEvent("Action_Acherus2_Portal", scan_delay, 1)
end

RegisterUnitEvent(npcid, 18, "Action_Acherus2_Portal")

function Action_Acherus3_Portal(Unit, event)
Unit:RemoveEvents();
local player = Unit:GetClosestPlayer()
if (player ~= nil) then
local player_dist = Unit:GetDistance(player)
if player_dist < 6 then
if player:GetZ() > 400 then
player:Teleport(0, 2401.895508, -5633.643555, 377.021362, 3.689022)
end
end
end
Unit:RegisterEvent("Action_Acherus3_Portal", scan_delay, 1)
end

RegisterUnitEvent(npcid3, 18, "Action_Acherus3_Portal")

function Action_Acherus4_Portal(Unit, event)
Unit:RemoveEvents();
local player = Unit:GetClosestPlayer()
if (player ~= nil) then
local player_dist = Unit:GetDistance(player)
if player_dist < 6 then
if player:GetZ() < 400 then
player:Teleport(0, 2419.491699, -5621.224121, 420.643707, 3.732219)
end
end
end
Unit:RegisterEvent("Action_Acherus4_Portal", scan_delay, 1)
end

RegisterUnitEvent(npcid4, 18, "Action_Acherus4_Portal")