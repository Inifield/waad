local cry_delay = 25000
local message = {}

message[1] = "O195\185 est l\'eau, Emma ? Va chercher l\'eau, Emma ! Si je m\'en chargeais pas, y\'en aurait beaucoup qui n\'sauraient pas \195\160  quoi r\'semble l\'eau."

function Action_LavieilleEmma(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1,0)
	Unit:RegisterEvent("Emote274", 1100, 1000)
end

function Initialisation_LavieilleEmma(Unit, event, player)
	Unit:RegisterEvent("Action_LavieilleEmma", cry_delay, 1)
end

function Mort_LavieilleEmma(Unit, event, player)
Unit:RemoveEvents()
end

RegisterUnitEvent(3520, 18, "Initialisation_LavieilleEmma")
RegisterUnitEvent(3520, 4, "Mort_LavieilleEmma")