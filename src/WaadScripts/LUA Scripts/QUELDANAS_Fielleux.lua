function WretchedFielleux(Unit, Event)
     Unit:RegisterEvent("Fielleux_Bitter", 23000, 5)
     Unit:RegisterEvent("Fielleux_Sunded", 8000, 1)
     Unit:RegisterEvent("Fielleux_Dort", 1000, 1)
     Unit:RegisterEvent("Fielleux_Parle", 1, 1)
end

function Fielleux_Parle(Unit, Event)
Unit:RemoveEvents();
	spin = math.random (1, 3)
	if spin == 1 then
		Unit:SendChatMessage(11, 0, "D\195\169gagez, cet endroit est \195\160 nous !")
	elseif spin == 2 then
		Unit:SendChatMessage(11, 0, "A moi ! Vous ne prendrez pas cet endroit !")
	elseif spin == 3 then
		Unit:SendChatMessage(11, 0, "Il ne vous est pas destin\195\169 ! Partez d\'ici !")
	end
end

function Fielleux_Bitter(Unit, Event)
       Unit:FullCastSpellOnTarget(29098,Unit:GetClosestPlayer())
end

function Fielleux_Sunded(Unit, Event)
       Unit:FullCastSpellOnTarget(11971,Unit:GetClosestPlayer())
end

function Fielleux_Dort(Unit, Event)
   if Unit:GetHealthPct() < 5 then
       Unit:FullCastSpell(42678)
end
end

RegisterUnitEvent(24966, 1, "WretchedFielleux")