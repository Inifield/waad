local cry_delay = 35000
local message = {}

message[1] = "L\'op\195\169ration Soleil bris\195\169 a bien de la chance de vous avoir comme ali\195\169."

function Action_Xayann(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1)
end

RegisterUnitEvent(24965, 10, "Action_Xayann")