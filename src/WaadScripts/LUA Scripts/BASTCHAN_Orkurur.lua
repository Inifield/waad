local cry_delay = 25000
local message = {}
local messmax = 2
local npcid = 25274

message[1] = "Si tu continues \195\160 faire de la merde, je vais te renvoyer chez ta m\195\168re dans une bo\195\174te, avec une pancarte sur le ventre o\195\185 sera \195\169crit: RATE RATE."
message[2] = "Vous appelez \195\167a un coup de marteau \196\177\\nQui est-ce qui m\'\195\160 coll\195\169 un pareil ramassis d\'amateurs \196\177"

function Action_Mob_Orkurur(Unit, event, player)
local choice = math.random(1, messmax)
if choice == 1 then
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1,0)
elseif choice == 2 then
	Unit:SendChatMessage(12, 0, message[2])
	Unit:Emote(1,0)
end
end

function Orkurur_Speach_Mob(Unit, event, player)
	Unit:RegisterEvent("Action_Mob_Orkurur", cry_delay, 1)
end

RegisterUnitEvent(npcid, 18, "Orkurur_Speach_Mob")