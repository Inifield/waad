local npcid = 25467
local message = {}

message[1] = "Pas taper le donneur de vie !"

function Moissonneur_Mob_OnAggro(Unit, event, player)
Unit:SendChatMessage(12, 0, message[1])
end

RegisterUnitEvent(npcid, 1, "Moissonneur_Mob_OnAggro")