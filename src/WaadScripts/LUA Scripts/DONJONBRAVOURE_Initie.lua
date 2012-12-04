local npcid = 25317 -- Initié
local message = {}
local cry_delay = 8000

message[1] = "Mmm..."
message[2] = "Pourriez-vous essayer de faire un peu moins de bruit \196\177"
message[3] = "Je commence \195\160 avoir mal au bras."
message[4] = "Vous n\'allez pas me laissez dormir, hein \196\177"
message[5] = "C\'est juste que je suis nerveux. D\195\169sol\195\169."

function Mob_Initie_Speak(Unit, event, player)
local choice = math.random(1, 10)
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

RegisterUnitEvent(npcid, 18, "Mob_Initie_OnSpeak")