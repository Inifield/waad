local cry_delay = 30000
local message = {}
local announcei = 2
local choice = 1

message[1] = "Pourvus que mon but soit vrai."
message[2] = "Aucun ne doit survivre !"

function Action_DameSylvanasCoursevent(Unit, event, player)
local choice = math.random(1, announcei)
if choice == 1 then
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1,0)
elseif choice == 2 then
	Unit:SendChatMessage(12, 0, message[2])
	Unit:Emote(5,0)
end
end

function Initialisation_DameSylvanasCoursevent(Unit, event, player)
	Unit:RegisterEvent("Action_DameSylvanasCoursevent", cry_delay, 1)
end

function Mort_DameSylvanasCoursevent(Unit, event, player)
Unit:RemoveEvents()
end

RegisterUnitEvent(10181, 18, "Initialisation_DameSylvanasCoursevent")
RegisterUnitEvent(10181, 4, "Mort_DameSylvanasCoursevent")