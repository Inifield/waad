local cry_delay = 30000
local message = {}
local announcei = 2
local choice = 1

message[1] = "J\'adore bricoler !"
message[2] = "Des nouvelles de Gnomeregan ?"

function Action_GrandBricoleurMekkanivelle(Unit, event, player)
local choice = math.random(1, announcei)
if choice == 1 then
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1,0)
elseif choice == 2 then
	Unit:SendChatMessage(12, 0, message[2])
	Unit:Emote(6,0)
end
end

function Initialisation_GrandBricoleurMekkanivelle(Unit, event, player)
	Unit:RegisterEvent("Action_GrandBricoleurMekkanivelle", cry_delay, 1)
end

RegisterUnitEvent(7937, 18, "Initialisation_GrandBricoleurMekkanivelle")