function WretchedDevourer(Unit, Event)
   Unit:RegisterEvent("Devourer_Mana", 19000, 5)
   Unit:RegisterEvent("Devourer_Nether", 9000, 1)
end

function Devourer_Mana(Unit, Event)
   Unit:FullCastSpell(33483)
end

function Devourer_Nether(Unit, Event)
   Unit:FullCastSpell(35334)
end

RegisterUnitEvent(24960, 1, "WretchedDevourer")