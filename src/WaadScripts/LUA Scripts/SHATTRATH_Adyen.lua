local message = {}

message[1] = "Jusque-l\195\160, votre pr\195\169sence \195\160 votre poste les gardera sur la br\195\168che."
message[2] = "A vos ordres, monsieur."

function Action_Adyen(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1)
end

function Action_Nemarn(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[2])
	Unit:RegisterEvent("Emote66", 0, 1000)
end

function Action_Nemarn2(Unit, event, player)
	Unit:MoveTo(-1838.000000, 5757.000000, 130.494995, 2.678450)
end

function Action_Nemarn3(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[2])
	Unit:MoveTo(-1774.000000, 5731.000000, 127.293503, 5.923717)
end

function Action_Nemarn4(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[2])
	Unit:MoveTo(-1691.000000, 5705.000000, 130.638138, 5.982622)
end

function Action_Nemarn5(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[2])
	Unit:MoveTo(-1774.000000, 5731.000000, 127.293503, 5.923717)
end

function Initialisation_Adyen(Unit, event, player)
Unit:RegisterEvent("Action_Adyen", 60000, 1)
Unit:RegisterEvent("Initialisation_Adyen", 600000, 1)
end

function Initialisation_Nemarn(Unit, event, player)
Unit:RegisterEvent("Action_Nemarn", 70000, 1)
Unit:RegisterEvent("Action_Nemarn2", 72000, 1)
Unit:RegisterEvent("Action_Nemarn3", 102000, 1)
Unit:RegisterEvent("Action_Nemarn4", 132000, 1)
Unit:RegisterEvent("Action_Nemarn5", 162000, 1)
Unit:RegisterEvent("Initialisation_Nemarn", 600000, 1)
end

RegisterUnitEvent(18537, 18, "Initialisation_Adyen")
RegisterUnitEvent(19377, 18, "Initialisation_Nemarn")