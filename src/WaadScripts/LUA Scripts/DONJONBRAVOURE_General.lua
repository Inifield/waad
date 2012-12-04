local npcid = 25285 -- Messager
local npcid2 = 25301 -- Conseiller
local npcid3 = 25250 -- Général

local message = {}
local choice = 1

-- Messager
message[1] ="Vous n\'avez qu\'un mot \195\160 dire, g\195\169n\195\169ral. Je peut mettre deux r\195\169giments \195\160 votre disposition en moins d\'un mois."
message[2] ="Tr\195\168s bien, g\195\169n\195\169ral. Si vous changez un jour d\'avis, mes troupes seront plus qu\'heureuses de vous appuyer militairement."

-- Conseiller
message[3] ="Nos troupes, g\195\169n\195\169ral, sont principalement compos\195\169es de villageois et de paysans. De bons gars, mais pas tout \195\160 fait d\195\169barrass\195\169s des pr\195\169jug\195\169s et superstitions de leur \195\169ducation. Ils ne sont pas pr\195\170ts \195\160 se battre aux c\195\180t\195\169s de nos alli\195\169s les plus... exotiques."

-- Général
message[4] ="Nous allons longuement r\195\169fl\195\169chir \195\160 votre offre, messager. En attendant, nous comptons donner \195\160 votre d\195\169l\195\169gation un r\195\180le consultatif."

function Action_Mob_General(Unit, event, player)
Unit:SendChatMessage(12, 0, message[1])
Unit:RegisterEvent("Action2_Mob_General", 30000, 1)
end

function Action2_Mob_General(Unit, event, player)
Unit:SendChatMessage(12, 0, message[2])
Unit:RegisterEvent("Initialisation_Mob_General", 20000, 1)
end

function Action_Mob_General2(Unit, event, player)
Unit:SendChatMessage(12, 0, message[3])
Unit:RegisterEvent("Initialisation_Mob_General2", 40000, 1)
end

function Action_Mob_General3(Unit, event, player)
Unit:SendChatMessage(12, 0, message[4])
Unit:RegisterEvent("Initialisation_Mob_General3", 30000, 1)
end

function Initialisation_Mob_General(Unit, event, player)
Unit:RegisterEvent("Action_Mob_General", 20000, 1)
end
function Initialisation_Mob_General2(Unit, event, player)
Unit:RegisterEvent("Action_Mob_General2", 30000, 1)
end
function Initialisation_Mob_General3(Unit, event, player)
Unit:RegisterEvent("Action_Mob_General3", 40000, 1)
end

RegisterUnitEvent(npcid, 18, "Initialisation_Mob_General")
RegisterUnitEvent(npcid2, 18, "Initialisation_Mob_General2")
RegisterUnitEvent(npcid3, 18, "Initialisation_Mob_General3")