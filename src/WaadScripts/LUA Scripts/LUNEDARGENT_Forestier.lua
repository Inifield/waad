local cry_delay = 30000
local message = {}
local announcei = 8
local choice = 1

message[1] = "Droit au but, voil\195\160 le truc."
message[2] = "Ha ! En plein dans le mille !"
message[3] = "Agh... on vient de marcher sur ma tombe."
message[4] = "Quel joli coup !"

function Action_Forestier(Unit, event, player)
local choice = math.random(1, announcei)
if choice == 1 then
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(25,0)
elseif choice == 2 then
	Unit:SendChatMessage(12, 0, message[2])
	Unit:Emote(4,0)
elseif choice == 3 then
	Unit:SendChatMessage(12, 0, message[3])
	Unit:Emote(1,0)
elseif choice == 4 then
	Unit:SendChatMessage(12, 0, message[4])
	Unit:Emote(4,0)
end
end

function Initialisation_Forestier(Unit, event, player)
	Unit:RegisterEvent("Action_Forestier", cry_delay, 1)
end

function Mort_Forestier(Unit, event, player)
Unit:RemoveEvents()
end

RegisterUnitEvent(18147, 18, "Initialisation_Forestier")
RegisterUnitEvent(18147, 4, "Mort_Forestier")