-- Quête du Chariot de mine discret
local  go1 = 190942
local  npc1 = 28821
local  npc2 = 28819
local  go_use_chariot = 0

function Action_Acherus_DeathPortal (pGameObject, event, pMisc)
Unit:RemoveEvents();
player_use_chariot = Unit:GetClosestPlayer()
if (player_use_chariot ~= nil) then
pMisc:Teleport (0, 2358.372070, -5662.258789, 382.258881, 0.561560)
pMisc:SpawnCreature (npc1, 2390.415039, -5901.630371, 109.131508, 3.510444, 35, 360000);
pMisc:SpawnCreature (npc2, 2387.862793, -5901.001953, 108.617508, 6.106476, 2095, 360000);
local player_use_chariot_dist = Unit:GetDistance(player_use_chariot)
if player_use_chariot_dist < 40 then
local go_use_chariot = 1
end
end
end

function MineurEcarlate_Chariot(Unit, event)
Unit:RemoveEvents();
if go_use_chariot == 1 then
Unit:SendChatMessage(15, 0, "D'ou est-ce que ça sort, tout ça ? Je ferais mieux de tout descendre aux bateaux, avant que le contremaitre ne voie ça !")
end
Unit:RegisterEvent("MineurEcarlate_Chariot", 2000, 1);
end

RegisterGameObjectEvent(go1, 4, "Action_Acherus_DeathPortal")
RegisterUnitEvent (npc2, 18, "MineurEcarlate_Chariot")