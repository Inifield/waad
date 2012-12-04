local message = {}

message[1] = "Tout est si incertain, maintenant que cette salle en bas est vide..."
message[2] = "Est-ce... Est-ce vrai. ce qu\'on raconte \196\177 Que c\'est le roi-soleil lui-m\195\170me qui a fait \195\167a \196\177"
message[3] = "Mais quoi qu\'il arrive les chevaliers de sang survivront \195\160 cette perte. Nous n\'avons pas le choix... Il nous faut trouver une autre source."

function Action_Emeline(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1,0)
end

function Action_Colin(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[2])
	Unit:Emote(6,0)
end

function Action_Emeline2(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1,0)
end

function Action_Colin2(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[3])
	Unit:Emote(1,0)
end

function Initialisation_Emeline(Unit, event, player)
Unit:RegisterEvent("Action_Emeline", 60000, 1)
Unit:RegisterEvent("Action_Emeline2", 80000, 1)
Unit:RegisterEvent("Initialisation_Emeline", 120000, 1)
end

function Initialisation_Colin(Unit, event, player)
Unit:RegisterEvent("Action_Colin", 70000, 1)
Unit:RegisterEvent("Action_Colin2", 90000, 1)
Unit:RegisterEvent("Initialisation_Colin", 120000, 1)
end

function Mort_Emeline(Unit, event, player)
Unit:RemoveEvents()
end

function Mort_Colin(Unit, event, player)
Unit:RemoveEvents()
end

RegisterUnitEvent(18175, 18, "Initialisation_Emeline")
RegisterUnitEvent(18174, 18, "Initialisation_Colin")
RegisterUnitEvent(18175, 4, "Mort_Emeline")
RegisterUnitEvent(18174, 4, "Mort_Colin")