local message = {}

message[1] = "Alors, messager, comment vont les recherches \196\177 Si les r\195\169sultats arrivent vite, vous y gagnerez peut-\195\170tre un Moule \195\160 gaufres ultrakin\195\169tique !"
message[2] = "\195\135a avance, Grillegaufre. Pourquoi n\'iriez-vous pas voir ailleurs s\'il n\'y a pas une pr\195\169sentation de produit \195\160 faire \196\177"

function Action_Grillegaufre(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(5,0)
end

function Action_Haronem(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[2])
	Unit:Emote(1,1100)
end

function Initialisation_Grillegaufre(Unit, event, player)
Unit:RegisterEvent("Action_Grillegaufre", 120000, 1)
Unit:RegisterEvent("Initialisation_Grillegaufre", 600000, 1)
end

function Initialisation_Haronem(Unit, event, player)
Unit:RegisterEvent("Action_Haronem", 120000, 1)
Unit:RegisterEvent("Initialisation_Haronem", 600000, 1)
end

RegisterUnitEvent(25885, 18, "Initialisation_Grillegaufre")
RegisterUnitEvent(10475, 18, "Initialisation_Haronem")