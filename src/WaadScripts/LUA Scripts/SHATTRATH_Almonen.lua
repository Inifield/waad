local message = {}
local announcei = 2
local choice = 1

message[1] = "A\'dal a ouvert mon esprit \195\160 des v\195\169rit\195\169s que je dois communiquer \195\160 nos invit\195\169s aujourd\'hui."
message[2] = "Je vous invite \195\160 tous rester parmi nous."
message[3] = "Ce serait un honneur, grand anachor\195\168te."
message[4] = "Comme le veut ma vocation divine, je dois partager les v\195\169rit\195\169s qui m\'ont \195\169t\195\169 r\195\169v\195\169l\195\169es avec tous ceux qui souhaitent les entendre."
message[5] = "Les gentillesses simples, les actes de charit\195\169, l\'aide aux gens dans le besoin. Ce sont l\195\160 les fruits de la Lumi\195\168re."
message[6] = "Malheureusement, d\'aucuns traversent leur existence mortelle dans la m\195\169fiance de la Lumi\195\168re."
message[7] = "Ceux-l\195\160, la Lumi\195\168re les quitte, jusqu\'\195\160 ce qu\'il ne reste que les t\195\169n\195\168bres."
message[8] = "Qu\'il en soit ainsi, grand anachor\195\168te."
message[9] = "Qu\'il en soit ainsi."

function Action_Almonen(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:RegisterEvent("Emote1", 0, 3000)
end

function Action_Almonen2(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[2])
	Unit:Emote(1,0)
	Unit:MoveTo(-1855.000000, 5422.000000, -10.400000, 4.200000)
	Unit:MoveTo(-1853.937134, 5410.574219, -12.428270, 5.224454)
end

function Action_Steele(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[3])
	Unit:RegisterEvent("Emote2", 0, 1000)
	Unit:MoveTo(-1850.151733, 5409.349121, -12.428200, 2.000000)
end

function Action_Almonen3(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[4])
	Unit:RegisterEvent("Emote1", 0, 3000)
end

function Action_Almonen4(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[5])
	Unit:RegisterEvent("Emote1", 0, 3000)
end

function Action_Almonen5(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[6])
	Unit:RegisterEvent("Emote1", 0, 3000)
end

function Action_Almonen6(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[7])
	Unit:RegisterEvent("Emote1", 0, 3000)
end

function Action_Steele2(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[8])
	Unit:RegisterEvent("Emote66", 0, 1000)
end

function Action_Almonen7(Unit, event, player)
	Unit:MoveTo(-1853.51, 5426.11, -10.4634, 2.81561)
end

function Action_Steele3(Unit, event, player)
	Unit:MoveTo(-1852.96, 5423.64, -10.4653, 2.28)
end

function Action_Clerc(Unit, event, player)
local choice = math.random(1, announcei)
if choice == 1 then
	Unit:SendChatMessage(12, 0, message[9])
	Unit:RegisterEvent("Emote20", 0, 1000)
end
end

function Action_Magus(Unit, event, player)
local choice = math.random(1, announcei)
if choice == 1 then
	Unit:SendChatMessage(12, 0, message[9])
	Unit:RegisterEvent("Emote20", 0, 1000)
end
end

function Initialisation_Clerc(Unit, event, player)
Unit:RegisterEvent("Action_Clerc", 965000, 1)
Unit:RegisterEvent("Initialisation_Clerc", 900000, 1)
end

function Initialisation_Magus(Unit, event, player)
Unit:RegisterEvent("Action_Magus", 965000, 1)
Unit:RegisterEvent("Initialisation_Magus", 900000, 1)
end

function Initialisation_Steele(Unit, event, player)
Unit:RegisterEvent("Action_Steele", 916000, 1)
Unit:RegisterEvent("Action_Steele2", 965000, 1)
Unit:RegisterEvent("Action_Steele3", 965000, 1)
Unit:RegisterEvent("Initialisation_Steele", 900000, 1)
end

function Initialisation_Almonen(Unit, event, player)
Unit:RegisterEvent("Action_Almonen", 900000, 1)
Unit:RegisterEvent("Action_Almonen2", 915000, 1)
Unit:RegisterEvent("Action_Almonen3", 925000, 1)
Unit:RegisterEvent("Action_Almonen4", 935000, 1)
Unit:RegisterEvent("Action_Almonen5", 945000, 1)
Unit:RegisterEvent("Action_Almonen6", 955000, 1)
Unit:RegisterEvent("Action_Almonen7", 965000, 1)
Unit:RegisterEvent("Initialisation_Almonen", 900000, 1)
end

RegisterUnitEvent(19216, 18, "Initialisation_Almonen")
RegisterUnitEvent(25141, 18, "Initialisation_Steele")
RegisterUnitEvent(25155, 18, "Initialisation_Clerc")
RegisterUnitEvent(25153, 18, "Initialisation_Magus")