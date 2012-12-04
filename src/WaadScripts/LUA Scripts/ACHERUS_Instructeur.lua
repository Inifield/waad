local npcid = 28394 --Chevalier de la mort Initié
local npcid2 = 28357 --Instructeur
local npcid3 = 28383 --Nécromancien d'Achérus
local npcid4 = 28405 -- Goule
local cry_delay = 160000

local temps_action1 = 6000
local temps_action2 = 12000
local temps_action3 = 18000
local temps_action4 = 24000
local temps_action5 = 30000
local temps_action6 = 36000
local temps_action7 = 42000
local temps_action8 = 48000
local temps_action9 = 54000
local temps_action10 = 60000

local message = {}

-- Messages
message[1] = "Celui-ci..."
message[2] = "J\'ai mal... souffrance insupportable... mettez-y fin... Je vous en supplie !"
message[3] = "Pitoyable..."
message[4] = "Il a \195\169t\195\169 d\195\169cr\195\169t\195\169 que vous n\'\195\170tes pas \195\160 la hauteur..."
message[5] = "D\195\169barrassez-vous-en..."
message[6] = "Pitoyable..."
message[7] = "Je sens la peur..."
message[8] = "Inutile..."
message[9] = "Oui, instructeur."
message[10] = "Tout de suite, instructeur."
message[11] = "Levez-vous, serviteurs. Levez-vous et repaissez-vous de la chair des faibles !"
message[12] = "Je... Je me suis réveill\195\169 dans un cauchemar ?"
message[13] = "Cette voix... quelle splendeur... Arthas... mon roi..."
message[14] = "Un messager de la mort rena\195\174t..."
message[15] = "Dotez-le des attributs qui conviennent \195\160 un messager d\'Arthas."
message[16] = "\195\137coutez, chevalier de la mort... \195\137coutez la voix de votre ma\195\174tre. Il vous appelle."
message[17] = "Levez-vous pour subir le test ! Levez-vous, car votre ma\195\174tre attend votre arriv\195\169e. Allez !"
message[18] = "À vos ordres, instructeur !"

function Instructeur_Razuvious_Message1(Unit, event)
--print "Instructeur_Razuvious_Message1"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[1])
end
end
end
function Instructeur_Razuvious_Message3(Unit, event)
--print "Instructeur_Razuvious_Message3"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[3])
end
end
end
function Instructeur_Razuvious_Message4(Unit, event)
--print "Instructeur_Razuvious_Message4"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[4])
end
end
end
function Instructeur_Razuvious_Message5(Unit, event)
--print "Instructeur_Razuvious_Message5"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[5])
end
end
end
function Instructeur_Razuvious_Message7(Unit, event)
--print "Instructeur_Razuvious_Message7"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[7])
end
end
end
function Instructeur_Razuvious_Message14(Unit, event)
--print "Instructeur_Razuvious_Message14"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[14])
end
end
end
function Instructeur_Razuvious_Message15(Unit, event)
--print "Instructeur_Razuvious_Message15"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[15])
end
end
end
function Instructeur_Razuvious_Message16(Unit, event)
--print "Instructeur_Razuvious_Message16"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[16])
end
end
end
function Instructeur_Razuvious_Message17(Unit, event)
--print "Instructeur_Razuvious_Message17"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[17])
end
end
end
-- ####################################
-- ####################################

function Chevalierdelamort_Message2(Unit, event)
--print "Chevalierdelamort_Message2"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 40 then
Unit:SendChatMessage(12, 0, message[2])
end
end
end
function Chevalierdelamort_Message12(Unit, event)
--print "Chevalierdelamort_Message12"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 40 then
Unit:SendChatMessage(12, 0, message[12])
end
end
end
function Chevalierdelamort_Message13(Unit, event)
--print "Chevalierdelamort_Message13"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[13])
end
end
end
function Chevalierdelamort_Message18(Unit, event)
--print "Chevalierdelamort_Message18"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 40 then
Unit:SendChatMessage(12, 0, message[18])
end
end
end
-- ####################################
-- ####################################

function Necromancien_Message9(Unit, event)
--print "Necromancien_Message9"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[9])
end
end
end
function Necromancien_Message10(Unit, event)
--print "Necromancien_Message10"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[10])
end
end
end
function Necromancien_Message11(Unit, event)
--print "Necromancien_Message11"
--Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[11])
-- kill_chevalierdelamort = 1
end
end
end

-- ####################################
-- ####################################
function Initialisation_Instructeur_Razuvious(Unit, event)
--Unit:RemoveEvents();
combinaison = math.random(1,3)
--print "=========================="
--print (combinaison)
--print "=========================="
if combinaison == 1 then
--print "combinaison 1 - Instructeur"
Unit:RegisterEvent("Initialisation_Instructeur_Razuvious", cry_delay, 1)
Unit:RegisterEvent("Instructeur_Razuvious_Message1", temps_action1, 1)
Unit:RegisterEvent("Instructeur_Razuvious_Message3", temps_action3, 1)
Unit:RegisterEvent("Instructeur_Razuvious_Message4", temps_action4, 1)
Unit:RegisterEvent("Instructeur_Razuvious_Message5", temps_action5, 1)
end
if combinaison == 2 then
--print "combinaison 2 - Instructeur"
Unit:RegisterEvent("Initialisation_Instructeur_Razuvious", cry_delay, 1)
Unit:RegisterEvent("Instructeur_Razuvious_Message1", temps_action1, 1)
Unit:RegisterEvent("Instructeur_Razuvious_Message7", temps_action2, 1)
Unit:RegisterEvent("Instructeur_Razuvious_Message4", temps_action3, 1)
Unit:RegisterEvent("Instructeur_Razuvious_Message5", temps_action4, 1)
end
if combinaison == 3 then
--print "combinaison 3 - Instructeur"
Unit:RegisterEvent("Initialisation_Instructeur_Razuvious", cry_delay, 1)
Unit:RegisterEvent("Instructeur_Razuvious_Message1", temps_action1, 1)
Unit:RegisterEvent("Instructeur_Razuvious_Message14", temps_action4, 1)
Unit:RegisterEvent("Instructeur_Razuvious_Message15", temps_action5, 1)
Unit:RegisterEvent("Instructeur_Razuvious_Message16", temps_action7, 1)
Unit:RegisterEvent("Instructeur_Razuvious_Message17", temps_action8, 1)
end
end
-- ####################################
function Initialisation_Chevalierdelamort(Unit, event)
Unit:SetCombatCapable(1);
--Unit:RemoveEvents();
if combinaison == 1 then
--print "combinaison 1 - Chevalier"
Unit:RegisterEvent("Initialisation_Chevalierdelamort", cry_delay, 1)
Unit:RegisterEvent("Chevalierdelamort_Message2", temps_action2, 1)
end
if combinaison == 2 then
--print "combinaison 2 - Chevalier"
Unit:RegisterEvent("Initialisation_Chevalierdelamort", cry_delay, 1)
end
if combinaison == 3 then
--print "combinaison 3 - Chevalier"
Unit:RegisterEvent("Initialisation_Chevalierdelamort", cry_delay, 1)
Unit:RegisterEvent("Chevalierdelamort_Message12", temps_action2, 1)
Unit:RegisterEvent("Chevalierdelamort_Message13", temps_action3, 1)
Unit:RegisterEvent("Chevalierdelamort_Message18", temps_action9, 1)
end
end
-- ####################################
function Initialisation_Necromancien(Unit, event)
local kill_chevalierdelamort = 0
--Unit:RemoveEvents();
if combinaison == 1 then
--print "combinaison 1 - Necro"
Unit:RegisterEvent("Initialisation_Necromancien", cry_delay, 1)
Unit:RegisterEvent("Necromancien_Message9", temps_action6, 1)
Unit:RegisterEvent("Necromancien_Message11", temps_action7, 1)
end
if combinaison == 2 then
--print "combinaison 2 - Necro"
Unit:RegisterEvent("Initialisation_Necromancien", cry_delay, 1)
Unit:RegisterEvent("Necromancien_Message9", temps_action5, 1)
Unit:RegisterEvent("Necromancien_Message11", temps_action6, 1)
end
if combinaison == 3 then
--print "combinaison 3 - Necro"
Unit:RegisterEvent("Initialisation_Necromancien", cry_delay, 1)
Unit:RegisterEvent("Necromancien_Message10", temps_action6, 1)
end
end
-- ####################################
function Initialisation_Goule(Unit, event)
--Unit:RemoveEvents();
if kill_chevalierdelamort == 1 then
Unit:SetCombatCapable(0);
else
Unit:SetCombatCapable(1);
end
Unit:RegisterEvent("Initialisation_Goule", 2000, 1)
end
RegisterUnitEvent(npcid, 18, "Initialisation_Chevalierdelamort")
RegisterUnitEvent(npcid2, 18, "Initialisation_Instructeur_Razuvious")
RegisterUnitEvent(npcid3, 18, "Initialisation_Necromancien")
RegisterUnitEvent(npcid4, 18, "Initialisation_Goule")