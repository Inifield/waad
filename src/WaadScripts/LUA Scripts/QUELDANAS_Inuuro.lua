local cry_delay = 25000
local message = {}

message[1] = "R\195\169veillez-vous ! L\'un de nos meilleurs combattants, est l\195\160 !"

function Action_Inuuro(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(5)
end

RegisterUnitEvent(25061, 10, "Action_Inuuro")