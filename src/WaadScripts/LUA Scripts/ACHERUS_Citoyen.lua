local message = {}
-- Messages
message[1] = "Ne me tuez pas ! Si j\'ai pris ce boulot, c\'est juste pour les primes !"
message[2] = "Je vous en pris, non ! Épargnez-moi !"
message[3] = "C\'est pas une bonne semaine pour arr\195\168ter de boire !"
message[4] = "Laissez-moi vivre ! Je ferai ce que vous voulez !"
message[5] = "J'ai cinq enfant ! Sans moi, ils n\'ont plus rien pour vivre !"
message[6] = "Euuhhnng... Je... Je crois que je me suis fait dessus..."
message[7] = "Ayez piti\195\169 !"
message[8] = "Épargnez-mois ! Je jure de partir d'ici ! Je vous en prie !"
message[9] = "Je ne vais pas me laisser tuer si facilement !"
message[10] = "Tu peux me prendre ma vie, mais tu n\'auras jamais ma libert\195\169 !"
message[11] = "MEURS !"
message[12] = "Si... S\'il vous pla\195\174t... Non..."

function Citoyen_Effraye_InAggro (Unit, Event)
Unit:RemoveEvents();
Unit:Emote(431,0);
local randomchoice = math.random(1,6)
if randomchoice == 1 then
Unit:SendChatMessage(12, 0, message[1])
elseif randomchoice == 2 then
Unit:SendChatMessage(12, 0, message[2])
elseif randomchoice == 3 then
Unit:SendChatMessage(12, 0, message[3])
elseif randomchoice == 4 then
Unit:SendChatMessage(12, 0, message[4])
elseif randomchoice == 5 then
Unit:SendChatMessage(12, 0, message[5])
elseif randomchoice == 6 then
Unit:SendChatMessage(12, 0, message[6])
elseif randomchoice == 7 then
Unit:SendChatMessage(12, 0, message[6])
elseif randomchoice == 8 then
Unit:SendChatMessage(12, 0, message[6])
elseif randomchoice == 9 then
Unit:SendChatMessage(12, 0, message[6])
elseif randomchoice == 10 then
Unit:SendChatMessage(12, 0, message[6])
elseif randomchoice == 11 then
Unit:SendChatMessage(12, 0, message[6])
elseif randomchoice == 12 then
Unit:SendChatMessage(12, 0, message[6])
end
end

function spell0_28576(Unit, Event) --@Cornered and Enraged!--
Unit:FullCastSpellOnTarget(52262,Unit:GetMainTank ())
Unit:RegisterEvent('mob28576_Action', 1, 1)
end
function Sorts_28576(Unit, Event)
choix_sorts_28576 = math.random(0, 0)
if choix_sorts_28576 == 0 then
Unit:RegisterEvent('spell0_28576', 1, 1)
end
end
function mob28576_Action(Unit, Event)
Unit:RegisterEvent('Sorts_28576', math.random(2000, 4000), 1)
Unit:RegisterEvent('Citoyen_Effraye_InAggro', 1, 1)
end

function spell0_28577(Unit, Event) --@Cornered and Enraged!--
Unit:FullCastSpellOnTarget(52262,Unit:GetMainTank ())
Unit:RegisterEvent('mob28577_Action', 1, 1)
end
function Sorts_28577(Unit, Event)
choix_sorts_28577 = math.random(0, 0)
if choix_sorts_28577 == 0 then
Unit:RegisterEvent('spell0_28577', 1, 1)
end
end
function mob28577_Action(Unit, Event)
Unit:RegisterEvent('Sorts_28577', math.random(2000, 4000), 1)
Unit:RegisterEvent('Citoyen_Effraye_InAggro', 1, 1)
end

function spell0_28575(Unit, Event) --@Throw--
Unit:FullCastSpellOnTarget(38556,Unit:GetMainTank ())
Unit:RegisterEvent('mob28575_Action', 1, 1)
end
function spell1_28575(Unit, Event) --@Torment--
Unit:FullCastSpellOnTarget(54526,Unit:GetMainTank ())
Unit:RegisterEvent('mob28575_Action', 1, 1)
end
function Sorts_28575(Unit, Event)
choix_sorts_28575 = math.random(0, 1)
if choix_sorts_28575 == 0 then
Unit:RegisterEvent('spell0_28575', 1, 1)
end
if choix_sorts_28575 == 1 then
Unit:RegisterEvent('spell1_28575', 1, 1)
end
end
function mob28575_Action(Unit, Event)
Unit:RegisterEvent('Sorts_28575', math.random(2000, 4000), 1)
Unit:RegisterEvent('Citoyen_Effraye_InAggro', 1, 1)
end

-- Citoyen de Havre-Comté
RegisterUnitEvent(28575, 1, "mob28575_Action");
RegisterUnitEvent(28576, 1, "mob28576_Action");
RegisterUnitEvent(28577, 1, "mob28577_Action");
RegisterUnitEvent(28557, 1, "Citoyen_Effraye_InAggro");