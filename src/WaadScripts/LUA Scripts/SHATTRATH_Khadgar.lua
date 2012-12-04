local message = {}

message[1] = "Faites visiter Shattrath \195\160 notre invit\195\169, voulez-vous \196\177 Faites attention aux voleurs \195\160 la tire dans la ville basse."
message[2] = "Suivez-moi. Ce ne sera pas long."

function Action_Khadgar(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1,0)
end

function Action_Serviteur(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[2])
	Unit:Emote(1,0)
	Unit:MoveTo(-1863.000000, 5407.000000, -12.427669, 4.700486)
end

function Action_Serviteur2(Unit, event, player)
	Unit:MoveTo(-1850.953613, 5385.117188, -12.427729, 5.196856)
end

function Action_Serviteur3(Unit, event, player)
	Unit:MoveTo(-1830.000000, 5362.000000, -12.427729, 5.377500)
end

function Action_Serviteur4(Unit, event, player)
	Unit:MoveTo(-1850.953613, 5385.117188, -12.427729, 5.196856)
end

function Action_Serviteur5(Unit, event, player)
	Unit:MoveTo(-1863.000000, 5407.000000, -12.427669, 4.700486)
end

function Action_Serviteur6(Unit, event, player)
	Unit:MoveTo(-1861.41, 5418.28, -10.4649, 1.8488)
end

function Initialisation_Khadgar(Unit, event, player)
Unit:RegisterEvent("Action_Khadgar", 600000, 1)
Unit:RegisterEvent("Initialisation_Khadgar", 1800000, 1)
end

function Initialisation_Serviteur(Unit, event, player)
Unit:RegisterEvent("Action_Serviteur", 601000, 1)
Unit:RegisterEvent("Action_Serviteur2", 611000, 1)
Unit:RegisterEvent("Action_Serviteur3", 621000, 1)
Unit:RegisterEvent("Action_Serviteur4", 631000, 1)
Unit:RegisterEvent("Action_Serviteur5", 641000, 1)
Unit:RegisterEvent("Action_Serviteur6", 651000, 1)
Unit:RegisterEvent("Initialisation_Serviteur", 1800000, 1)
end

RegisterUnitEvent(18166, 18, "Initialisation_Khadgar")
RegisterUnitEvent(19685, 18, "Initialisation_Serviteur")