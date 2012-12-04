local message = {}

message[1] = "Profitez du temps que vous aller passer loin d\'ici pour r\195\169fl\195\169chir \195\160 vos actes."

function Redresseur_OnCombat(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(274,0)
	Unit:FullCastSpellOnTarget(36642,Unit:GetClosestPlayer())
	Unit:RegisterEvent("Redresseur_Teleport", 6000, 1)
end

function Redresseur_Teleport(Unit, event, player)
	player=Unit:GetClosestPlayer();
	player:Teleport (530, -1418.000000, 5164.000000, 64.656502)
end

function Redresseur_OnDied(Unit, event, player)
Unit:RemoveEvents()
end

RegisterUnitEvent(18549, 1, "Redresseur_OnCombat")
RegisterUnitEvent(18549, 4, "Redresseur_OnDied")