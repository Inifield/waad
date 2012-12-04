local message = {}

message[1] = "Ne me faite pas perdre mon temps."

function Action_Varimathras(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1,0)
end

function Mort_Varimathras(Unit, event, player)
Unit:RemoveEvents()
end

RegisterUnitEvent(2425, 10, "Action_Varimathras")
RegisterUnitEvent(2425, 4, "Mort_Varimathras")