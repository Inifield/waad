local message = {}

message[1] = "Kael\'thas et ses agents ont grandement fait souffrir nos deux peuples, dame Liadrin. Les v\195\180tres n\'ont pas \195\169t\195\169 les artisans de leur propre destin, mais ils mourront s\'ils ne changent pas de voie."
message[2] = "Quand il nous fut donn\195\169 par le « prince » Kael\'thas, nous pensions que sa puissance nous aiderait \195\160 guider notre peuple vers une nouvelle \195\168re."
message[3] = "Je me suis rendu compte que le chemin que nous suivions \195\169tait le mauvais. Nous avons \195\169t\195\169 trahis par celui que nous appelions notre prince. Pour satisfaire sa soif de puissance, il a envoy\195\169 ses gangresangs nous attaquer et enlever M\'uru pour le ramener au Puits de soleil."
message[4] = "M\'uru a accept\195\169 le r\195\180le qui devait \195\170tre le sien il y a bien longtemps, en toute connaissance de ce qui lui arriverait. Accepterez-vous celui qui doit \195\170tre le v\195\180tre \196\177"
message[5] = "Je... Je ne comprends pas. Vous... et M\'uru... saviez ce qui allait arriver depuis le d\195\169but \196\177"
message[6] = "L\'op\195\169ration Soleil bris\195\169 tirera sans aucun doute grand profit du renfort de vos chevaliers, dame Liadrin."
message[7] = "Nous combattrons \195\160 vos c\195\180t\195\169s, A\'dal."
message[8] = "Comment pouvez-vous supporter la pr\195\169sence de la m\195\169prisable dame Liadrin \196\177 Elle et ses disciples d\195\169naturent la Lumi\195\168re et bafouent toutes les valeurs que nous d\195\169fendons !"

function Action_Adal(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
end

function Action_Liadrin(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[2])
	Unit:Emote(1,0)
end

function Action_Liadrin2(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[3])
	Unit:Emote(1,0)
end

function Action_Tiras(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[8])
	Unit:Emote(5,0)
end

function Action_Adal2(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[4])
end

function Action_Liadrin3(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[5])
	Unit:Emote(6,0)
end

function Action_Adal3(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[6])
end

function Action_Liadrin4(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[7])
	Unit:Emote(16,0)
end

function Initialisation_Adal(Unit, event, player)
Unit:RegisterEvent("Action_Adal", 60000, 1)
Unit:RegisterEvent("Action_Adal2", 90000, 1)
Unit:RegisterEvent("Action_Adal3", 110000, 1)
Unit:RegisterEvent("Initialisation_Adal", 300000, 1)
end

function Initialisation_Liadrin(Unit, event, player)
Unit:RegisterEvent("Action_Liadrin", 70000, 1)
Unit:RegisterEvent("Action_Liadrin2", 80000, 1)
Unit:RegisterEvent("Action_Liadrin3", 100000, 1)
Unit:RegisterEvent("Action_Liadrin4", 120000, 1)
Unit:RegisterEvent("Initialisation_Liadrin", 300000, 1)
end

function Initialisation_Tiras(Unit, event, player)
Unit:RegisterEvent("Action_Tiras", 83000, 1)
Unit:RegisterEvent("Initialisation_Tiras", 300000, 1)
end

RegisterUnitEvent(18481, 18, "Initialisation_Adal")
RegisterUnitEvent(17076, 18, "Initialisation_Liadrin")
RegisterUnitEvent(25167, 18, "Initialisation_Tiras")