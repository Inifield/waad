local cry_delay = 50000
local message = {}
local announcei = 4
local choice = 1

message[1] = "Nos vies sont à vous, chevalier de sang."
message[2] = "Gloire au roi-s - euh, Gloire \195\160 Lune-d\'argent !"

function Action_Guet(Unit, event, player)
local choice = math.random(1, announcei)
if choice == 1 then
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1,0)
elseif choice == 2 then
	Unit:SendChatMessage(12, 0, message[2])
	Unit:Emote(5,0)
end
end

function Initialisation_Guet(Unit, event, player)
	Unit:RegisterEvent("Action_Guet", cry_delay, 1)
end

function Mort_Guet(Unit, event, player)
Unit:RemoveEvents()
end

RegisterUnitEvent(16222, 18, "Initialisation_Guet")
RegisterUnitEvent(16222, 4, "Mort_Guet")