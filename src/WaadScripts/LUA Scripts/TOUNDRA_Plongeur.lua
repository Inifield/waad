local npcid = 25836
local message = {}

message[1] = "Chemise en peau de palourde, bient\195\180t tu seras mienne !"

function Mob_Plongeur_OnAggro(Unit, event, player)
Unit:SendChatMessage(12, 0, message[1])
end

function Mob_Plongeur_OnDied(Unit, event, player)
Unit:RemoveEvents()
end

RegisterUnitEvent(npcid, 1, "Mob_Plongeur_OnAggro")
RegisterUnitEvent(npcid, 4, "Mob_Plongeur_OnDied")