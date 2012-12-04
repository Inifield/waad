local npcid = 28605
local npcid2 = 28606
local delay = 500

function Acherus_Chevaux(Unit, event)
Unit:RemoveEvents();
Unit:MarkQuestObjectiveAsComplete(12680, 1)
end

RegisterUnitEvent(npcid, 4, "Acherus_Chevaux")
RegisterUnitEvent(npcid2, 4, "Acherus_Chevaux")