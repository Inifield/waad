local cry_delay = 50000
local message = {}
local announcei = 4

message[1] = "L\'op\195\169ration Soleil bris\195\169 a bien de la chance de vous avoir comme ali\195\169."

function Action_GuerrierSoleil(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1)
end

RegisterUnitEvent(25115, 10, "Action_GuerrierSoleil")