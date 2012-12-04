local cry_delay = 50000
local message = {}

message[1] = "Que pourrais faire les nains pour t\'aider ?"

function Action_RoiMagniBarbedebronze(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(6,0)
end

function Initialisation_RoiMagniBarbedebronze(Unit, event, player)
	Unit:RegisterEvent("Action_RoiMagniBarbedebronze", cry_delay, 1)
end

RegisterUnitEvent(2784, 18, "Initialisation_RoiMagniBarbedebronze")