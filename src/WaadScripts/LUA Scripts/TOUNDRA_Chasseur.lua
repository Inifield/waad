local npcid = 25979
local message = {}
local choice = 1
local cry_delay = 10000

-- OnAggro
message[1] = "Seulement cinquante sabots de plus et \195\160 moi le nouveau flingue !"
message[2] = "Je suis s\195\187r que l\'vieux Hemet trouvera quelque chose \195\160 faire de vos os !"
message[3] = "TUE TUE TUE !!!!!!!"

-- OnSpeak
message[4] = "Il se fait appeler le roi de la fourrure, mais c\'est plut\195\180t le roi des pigeons ! Je r\195\169cup\195\168re une paire de chaussettes d\'hiver toute neuve en peau de rat, rien qu\'en tuant et en d\195\169pe\195\167ant cinquante b\195\169b\195\169s caribous \196\177"
message[5] = "Attendez voir : si je donne quinze trompes de mammouth \195\160 Lane, je re\195\167ois une s\195\169rie de dagues qui peuvent aussi servir de couteaux \195\160 steak \196\177 Mais comment fait-il \196\177 Comment fait-il pour proposer des affaires aussi fantastiques \196\177"

function Mob_Chasseur_OnAggro(Unit, event, player)
local choice = math.random(1, 3)
Unit:RemoveEvents();
if choice == 1 then
Unit:SendChatMessage(12, 0, message[1])
elseif choice == 2 then
Unit:SendChatMessage(12, 0, message[2])
elseif choice == 3 then
Unit:SendChatMessage(12, 0, message[3])
end
end

function Mob_Chasseur_Speak(Unit, event, player)
local choice = math.random(1, 2)
Unit:RemoveEvents();
if choice == 1 then
Unit:SendChatMessage(12, 0, message[4])
elseif choice == 2 then
Unit:SendChatMessage(12, 0, message[5])
end
end

function Mob_Chasseur_OnSpeak(Unit, event, player)
Unit:RegisterEvent("Mob_Chasseur_Speak", cry_delay, 1)
end

function Mob_Chasseur_OnDied(Unit, event, player)
Unit:RemoveEvents()
end

RegisterUnitEvent(npcid, 1, "Mob_Chasseur_OnAggro")
RegisterUnitEvent(npcid, 18, "Mob_Chasseur_OnSpeak")
RegisterUnitEvent(npcid, 4, "Mob_Chasseur_OnDied")