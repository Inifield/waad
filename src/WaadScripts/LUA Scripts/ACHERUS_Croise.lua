local message = {}

-- Messages
message[1] = "Disparais, odieux d\195\169mon !"
message[2] = "C\'est le signe du Fléau !"
message[3] = "La mort nous poursuit ! D\195\169truisez-la !"

function CroiseEcarlate_InAggro (Unit, Event)
Unit:RemoveEvents();
local randomchoice = math.random(1,3)
if randomchoice == 1 then
Unit:SendChatMessage(12, 0, message[1])
elseif randomchoice == 2 then
Unit:SendChatMessage(12, 0, message[2])
elseif randomchoice == 3 then
Unit:SendChatMessage(12, 0, message[3])
end
end

function spell0_28529(Unit, Event) --@Shoot--
Unit:FullCastSpellOnTarget(6660,Unit:GetMainTank ())
Unit:RegisterEvent('mob28529_Action', 1, 1)
end
function Sorts_28529(Unit, Event)
choix_sorts_28529 = math.random(0, 0)
if choix_sorts_28529 == 0 then
Unit:RegisterEvent('spell0_28529', 1, 1)
end
end
function mob28529_Action(Unit, Event)
Unit:RegisterEvent('Sorts_28529', math.random(2000, 4000), 1)
Unit:RegisterEvent('CroiseEcarlate_InAggro', 1, 1)
end

function spell0_28940(Unit, Event) --@Heroic Strike--
Unit:FullCastSpellOnTarget(52221,Unit:GetMainTank ())
Unit:RegisterEvent('mob28940_Action', 1, 1)
end
function spell1_28940(Unit, Event) --@Shoot--
Unit:FullCastSpellOnTarget(52818,Unit:GetMainTank ())
Unit:RegisterEvent('mob28940_Action', 1, 1)
end
function Sorts_28940(Unit, Event)
choix_sorts_28940 = math.random(0, 1)
if choix_sorts_28940 == 0 then
Unit:RegisterEvent('spell0_28940', 1, 1)
end
if choix_sorts_28940 == 1 then
Unit:RegisterEvent('spell1_28940', 1, 1)
end
end
function mob28940_Action(Unit, Event)
Unit:RegisterEvent('Sorts_28940', math.random(2000, 4000), 1)
Unit:RegisterEvent('CroiseEcarlate_InAggro', 1, 1)
end

-- Citoyen de Havre-Comté
RegisterUnitEvent(28529, 1, "mob28529_Action");
RegisterUnitEvent(28940, 1, "mob28940_Action");