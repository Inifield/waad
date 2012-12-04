local npcid = 27300 -- Vernon
local npcid2 = 27299 -- Greer
local message = {}
local choice = 1
local cry_delay = 10000

-- Vernon
message[6] = "Il vous faut plus de mordant. Ne montrez aucune piti\195\169 envers vos ennemis."

-- Greer
message[1] = "C\'est vrai. Mais jusqu\'ou pouvons-nous aller \196\177 À partir de quel point ne sommes-nous plus meilleurs que nos ennemis \196\177"
message[2] = "Ce coup-ci, je crois que je tiens le bon bout..."
message[3] = "Dites-moi si \195\167a fait mal."
message[4] = "Il est vrai que nos ennemis n\'auraient aucune piti\195\169, eux."
message[5] = "Pensez-vous qu\'on en tirera quelques r\195\169ponses \196\177"

function Mob2_Initie_Speak(Unit, event, player)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[6])
Unit:RegisterEvent("Mob2_Initie_Speak", cry_delay, 1)
end

function Mob_Initie_Speak(Unit, event, player)
local choice = math.random(1, 5)
Unit:RemoveEvents();
if choice == 1 then
Unit:SendChatMessage(12, 0, message[1])
elseif choice == 2 then
Unit:SendChatMessage(12, 0, message[2])
elseif choice == 3 then
Unit:SendChatMessage(12, 0, message[3])
elseif choice == 4 then
Unit:SendChatMessage(12, 0, message[4])
elseif choice == 5 then
Unit:SendChatMessage(12, 0, message[5])
end
Unit:RegisterEvent("Mob_Initie_Speak", cry_delay, 1)
end

function Mob_Initie_OnSpeak(Unit, event, player)
Unit:RegisterEvent("Mob_Initie_Speak", cry_delay, 1)
end
function Mob2_Initie_OnSpeak(Unit, event, player)
Unit:RegisterEvent("Mob2_Initie_Speak", cry_delay, 1)
end
function Mob_Initie_OnDied(Unit, event, player)
Unit:RemoveEvents()
end
function Mob2_Initie_OnDied(Unit, event, player)
Unit:RemoveEvents()
end

RegisterUnitEvent(npcid, 18, "Mob_Initie_OnSpeak")
RegisterUnitEvent(npcid2, 18, "Mob2_Initie_OnSpeak")
RegisterUnitEvent(npcid, 4, "Mob_Initie_OnDied")
RegisterUnitEvent(npcid2, 4, "Mob2_Initie_OnDied")