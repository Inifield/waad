-- Informations
local temps_action = 20000
local temps_inter_action = 10000
local message = {}

-- Messages
message[1] = "Des mages squelettes. Voil\195\160 une occasion parfaite de faire la d\195\169monstration de l\'utilit\195\169 de certains de nos pouvoirs."
message[2] = "Your spells will not be as potent as mine, but you will still find anti-magic shells to be entirely invaluable against a large number of magical attacks."
message[3] = "L\'\195\169nergie des sorts que votre carapace absorbe peut \195\170tre utilis\195\169e pour lancer certaines de vos attaques."
message[4] = "Les mages nous craignent... \195\160 raison. Jouez de cette peur, disciples."
message[5] = "La suite..."
message[6] = "Moi... revenu... ma\195\174tre..." -- Goules
message[7] = "Des goules \196\177 Nous en avons d\195\169j\195\160 bien assez."
message[8] = "Je me dis souvent que le probl\195\168me des goules est qu\'elles survivent trop longtemps pour si peu d\'utilit\195\169. Que la mort et la d\195\169composition viennent reprendre celles-ci."
message[9] = "Moi... pas bien \196\177" -- Goules
message[10] = "Parfois, les cadavres se r\195\169v\195\168lent plus utiles que les goules ne l\'ont \195\169t\195\169."
message[11] = "Mmm..."
message[12] = "Regrettable."
message[13] = "Ô cher monde des ombres, accorde-nous la pr\195\169sence de l\'un de tes nombreux habitants."
message[14] = "Wha... where am I\196\177" -- Eclaireur
message[15] = "A druid! How curious... I wonder how it got in there."
message[16] = "Disciples... Avec le temps, vous apprendrez que de nombreux sorts de gu\195\169rison sont vuln\195\169rables \195\160 l\'effet d\195\169g\195\169n\195\169rant de nos coups."
message[17] = "Certains gu\195\169risseurs ne disposent pas de connaissance n\195\169cessaire pour lutter contre les maladies dont nous sommes les ma\195\174tres."
message[18] = "Ce d\195\169faut de connaissance n\'est qu\'une de leurs nombreuses faiblesses. Affronter la mort elle-m\195\170me est un combat sans grand espoir."
message[19] = "Il reste encore bien des le\195\167ons \195\160 recevoir, passons donc \195\160 notre prochain sujet..."

-- Mobs
local alistra = 28471
local squelette = 29231
local goule = 29232
local druide = 29220

-- Function
function Alistra_Phase1(Unit, Event)
Unit:RemoveEvents();
local ok_goule = 0
local finish_goule = 0
Unit:SendChatMessage(12, 0, message[1])
Unit:RegisterEvent("Alistra_Phase2", temps_inter_action, 0)
end
function Alistra_Phase2(Unit, Event)
Unit:RemoveEvents();
Unit:RegisterEvent("Alistra_Phase3", temps_inter_action, 0)
end
function Alistra_Phase3(Unit, Event)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[2])
Unit:RegisterEvent("Alistra_Phase4", temps_inter_action, 0)
end
function Alistra_Phase4(Unit, Event)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[3])
Unit:RegisterEvent("Alistra_Phase5", temps_inter_action, 0)
end
function Alistra_Phase5(Unit, Event)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[4])
Unit:RegisterEvent("Alistra_Phase6", temps_inter_action, 0)
end
function Alistra_Phase6(Unit, Event)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[5])
ok_goule = 1
Unit:RegisterEvent("Alistra_Phase7", temps_inter_action, 0)
end
function Alistra_Phase7(Unit, Event)
Unit:RemoveEvents();
Unit:RegisterEvent("Alistra_Phase8", temps_inter_action, 0)
end
function Goule_Phase1(Unit, Event)
Unit:RemoveEvents();
if ok_goule == 1 then
Unit:SendChatMessage(12, 0, message[6])
Unit:RegisterEvent("Goule_Phase2", 10000, 0)
else
Unit:RegisterEvent("Goule_Phase1", temps_inter_action, 0)
end
end
function Alistra_Phase8(Unit, Event)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[7])
Unit:RegisterEvent("Alistra_Phase9", temps_inter_action, 0)
end
function Alistra_Phase9(Unit, Event)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[8])
finish_goule = 1
Unit:RegisterEvent("Alistra_Phase10", temps_inter_action, 0)
end
function Goule_Phase2(Unit, Event)
Unit:RemoveEvents();
if finish_goule == 1 then
Unit:SendChatMessage(12, 0, message[9])
else
Unit:RegisterEvent("Goule_Phase2", temps_inter_action, 0)
end
end
function Alistra_Phase10(Unit, Event)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[10])
Unit:RegisterEvent("Alistra_Phase11", temps_inter_action, 0)
end
function Alistra_Phase11(Unit, Event)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[11])
Unit:RegisterEvent("Alistra_Phase12", temps_inter_action, 0)
end
function Alistra_Phase12(Unit, Event)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[12])
Unit:RegisterEvent("Alistra_Phase13", temps_inter_action, 0)
end
function Alistra_Phase13(Unit, Event)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[13])
Unit:RegisterEvent("Alistra_Phase14", temps_inter_action, 0)
end
function Alistra_Phase14(Unit, Event)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[16])
Unit:RegisterEvent("Alistra_Phase15", temps_inter_action, 0)
end
function Alistra_Phase15(Unit, Event)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[17])
Unit:RegisterEvent("Alistra_Phase16", temps_inter_action, 0)
end
function Alistra_Phase16(Unit, Event)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[18])
Unit:RegisterEvent("Alistra_Phase17", temps_inter_action, 0)
end
function Alistra_Phase17(Unit, Event)
Unit:RemoveEvents();
Unit:SendChatMessage(12, 0, message[19])
Unit:RegisterEvent("Alistra_Phase1", temps_action, 0)
end

-- Function RegisterUnitEvent
function Alistra_Action(Unit, Event)
Unit:RemoveEvents();
Unit:RegisterEvent("Alistra_Phase1", temps_action, 0)
end
function Goule_Action(Unit, Event)
Unit:RemoveEvents();
Unit:RegisterEvent("Goule_Phase1", 1, 0)
end

-- RegisterUnitEvent
RegisterUnitEvent(alistra, 18, "Alistra_Action")
RegisterUnitEvent(goule, 18, "Goule_Action")