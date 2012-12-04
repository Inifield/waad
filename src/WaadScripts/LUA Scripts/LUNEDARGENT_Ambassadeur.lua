local message = {}

message[1] = "Admirez les merveilles de notre splendide cit\195\169."

function Action_Ambassadeur(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1,0)
	Unit:Emote(29,0)
end

function Initialisation_Ambassadeur(Unit, event, player)
Unit:RegisterEvent("Action_Ambassadeur", 2000, 1)
Unit:RegisterEvent("Initialisation_Ambassadeur", 45000, 1)
end

function Mort_Ambassadeur(Unit, event, player)
Unit:RemoveEvents()
end

RegisterUnitEvent(18191, 18, "Initialisation_Ambassadeur")
RegisterUnitEvent(18191, 4, "Mort_Ambassadeur")