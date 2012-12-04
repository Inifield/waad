local cry_delay = 25000
local message = {}
local announcei = 6
local choice = 1

message[1] = "Nous ne sommes pas l\195\160 pour nous promener. Nos coeurs, nos \195\162mes, l\'ultime d\195\169livrance des addictions qui nous hantent, la cl\195\169 de tout cela se trouve ici sur Quel\'Danas."
message[2] = "Vous voulez finir comme des D\195\169sh\195\169rit\195\169s \196\177 Non \196\177! Alors mettez-y du coeur cette fois !"
message[3] = "Kael\'thas ne tol\195\169rera pas de nouvel \195\169chec !"
message[4] = "A la charge !"

function Action_Chevalier(Unit, event, player)
local choice = math.random(1, announcei)
if choice == 1 then
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1,0)
elseif choice == 2 then
	Unit:SendChatMessage(12, 0, message[2])
	Unit:Emote(1,0)
elseif choice == 3 then
	Unit:SendChatMessage(12, 0, message[3])
	Unit:Emote(1,0)
end
end

function ActionAggro_Chevalier(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[4])
end

function Initialisation_Chevalier(Unit, event, player)
	Unit:RegisterEvent("Action_Chevalier", cry_delay, 1)
end

function Aggro_Chevalier(Unit, event, player)
	Unit:RegisterEvent("ActionAggro_Chevalier", 0, 1)
end

RegisterUnitEvent(24976, 18, "Initialisation_Chevalier")
RegisterUnitEvent(24976, 1, "Aggro_Chevalier")