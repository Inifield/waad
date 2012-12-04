local cry_delay = 30000
local message = {}

message[1] = "Soit en paix."

function Action_ArchevequeBenedictus(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(2,0)
end

function Mort_ArchevequeBenedictus(Unit, event, player)
Unit:RemoveEvents()
end

RegisterUnitEvent(1284, 10, "Action_ArchevequeBenedictus")
RegisterUnitEvent(1284, 4, "Mort_ArchevequeBenedictus")