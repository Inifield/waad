local message = {}

message[1] = "Nous sommes encercl\195\169s. Nos ennemis nous pressent de toutes parts, jeune Hurlenfer."
message[2] = "Du nord, le Fl\195\169au s\'abat sur nous comme une nu\195\169e de sauterelles."
message[3] = "L\'Alliance tient la seul route maritime s\195\187re de la r\195\169gion, et m\195\170me celle-l\195\160 risque de disparaitre \195\160 tout moment dans ces terrible brumes."
message[4] = "Le seul port viable que nous puissions utiliser pour nous r\195\169approvisionner est aux mains des R\195\169prouv\195\169s et se trouve de l\'autre c\195\180t\195\169 de ce maudit continent !"
message[5] = "Tout ce qui est trop lourd pour nos zeppelins doit \195\170tre achemin\195\169 par mer et traverser le Norfendre dans toute sa largeur avat de nous atteindre."

message[6] = "Les routes maritimes... Les soucis d\'intendance... Tout cela m\'ennuie \195\160 mourir ! Saurcroc, la seule chose dont nous ayons besoin, c\'est de l\'esprit guerrier de la Horde ! Maintenant que nous tenons une position solide dans ce d\195\169sert glac\195\169, rien ne nous arr\195\170tera plus !"
message[7] = "Parce que tu as l\'intention d\'abattre les murailles de la Couronne de glace sans machine de si\195\168ge, sans munitions, sans armures lourdes \196\177"

message[8] = "Mes intentions \196\177 Les voil\195\160, mes intentions !"
message[9] = "L\195\160... Maintenant, nous avons une route maritime."
message[10] = "Et pour faire bonne mesure..."
message[11] = "Le fils prodigue \195\160 parl\195\169 !"
message[12] = "C\'est le sang de ton p\195\168re qui parle en toi, Hurlenfer. Toujours impatient... impatient et t\195\169m\195\169raire."
message[13] = "Tu te jettes t\195\170te baiss\195\169 dans la guerre sans souci des cons\195\169quences."
message[14] = "Ne me parlez pas de cons\195\169quences l\'ancien !"
message[15] = "J\'ai bu le m\195\170me sang que ton p\195\168re, Garrosh. Le venin maudit de Mannoroth a aussi coul\195\169 dans mes veines."
message[16] = "J\'ai plong\195\169 mes armes dans les corps et les esprits de mes ennemis."
message[17] = "Et m\195\170me si Grom est mort glorieusement en nous lib\195\169rant de la mal\195\169diction du sang... De nos souvenirs, m\195\170me lui n\'a pas pu nous lib\195\169rer."
message[18] = "Sont geste n\'a pas \195\169ffac\195\169 les horreurs dont nous nous sommes rendus coupables."
message[19] = "Dans l\'hiver qui a suivi la fin de la mal\195\169diction, des milliers de v\195\169t\195\169rans orcs comme moi ont sombr\195\169 dans le d\195\169sespoir."
message[20] = "Nos esprits \195\169taient enfin libres, oui... Libres de revivre tous les actes indicibles que nous avions commis sous l\'influence de la L\195\169gion."
message[21] = "Je crois que c\'\195\169taient les cris des enfants draene\195\175 qui hantaient la pluspart d\'entre nous... On ne les oublie jamais..."
message[22] = "As-tu d\195\169j\195\160 \195\169t\195\169 \195\160 la ferme R\195\170che-pourceau \196\177 Lorsque les porcs sont en \195\162ge d\'\195\170tre \195\169gorg\195\169s... C\'est le m\195\170me son. Le cri du cochon que l\'on tue. Il r\195\169sonne tr\195\168s fort. Nous autres, les v\195\169t\195\169rans les plus vieux, vivons un cauchemar."
message[23] = "Mais enfin, vous ne pouvez quand m\195\170me pas croire que ces enfants \195\169taient n\195\169s innoncents \196\177 Ils auraient grandi. Ils auraient pris les armes contre nous !"

message[24] = "Je ne parle pas que des enfants de nos ennemis."
message[25] = "Je ne te laisserai pas nous faire reprendre ce terrible chemin, jeune Hurlevent. Je te tuerai de mes propres mains avant de laisser ce jour venir..."
message[26] = "Comment avez-vous pu survivre si longtemps sans \195\170tre d\195\169vor\195\169 par vos propres souvenirs, Saurcroc."
message[27] = "Je ne mange pas de porc..."

function Action_Saurcroc(Unit, event, player)
Unit:SendChatMessage(12, 0, message[1])
Unit:RegisterEvent("Action_Saurcroc2", 5000, 1)
end
function Action_Saurcroc2(Unit, event, player)
Unit:SendChatMessage(12, 0, message[2])
Unit:RegisterEvent("Action_Saurcroc3", 5000, 1)
end
function Action_Saurcroc3(Unit, event, player)
Unit:SendChatMessage(12, 0, message[3])
Unit:RegisterEvent("Action_Saurcroc4", 5000, 1)
end
function Action_Saurcroc4(Unit, event, player)
Unit:SendChatMessage(12, 0, message[4])
Unit:RegisterEvent("Action_Saurcroc5", 5000, 1)
end
function Action_Saurcroc5(Unit, event, player)
Unit:SendChatMessage(12, 0, message[5])
Unit:RegisterEvent("Action_Saurcroc6", 10000, 1)
end

function Action_Hurlenfer(Unit, event, player)
Unit:SendChatMessage(12, 0, message[6])
Unit:RegisterEvent("Action_Hurlenfer2", 10000, 1)
end

function Action_Saurcroc6(Unit, event, player)
Unit:SendChatMessage(12, 0, message[7])
Unit:RegisterEvent("Action_Saurcroc7", 20000, 1)
end

function Action_Hurlenfer2(Unit, event, player)
Unit:SendChatMessage(12, 0, message[8])
Unit:RegisterEvent("Action_Hurlenfer3", 5000, 1)
end
function Action_Hurlenfer3(Unit, event, player)
Unit:SendChatMessage(12, 0, message[9])
Unit:MoveTo(2837.959961, 6186.740234, 84.683197, 3.723390)
Unit:RegisterEvent("Action_Hurlenfer4", 5000, 1)
end
function Action_Hurlenfer4(Unit, event, player)
Unit:SendChatMessage(12, 0, message[10])
Unit:RegisterEvent("Action_Hurlenfer5", 20000, 1)
end

function Action_Saurcroc7(Unit, event, player)
Unit:SendChatMessage(12, 0, message[11])
Unit:RegisterEvent("Action_Saurcroc8", 5000, 1)
end
function Action_Saurcroc8(Unit, event, player)
Unit:SendChatMessage(12, 0, message[12])
Unit:RegisterEvent("Action_Saurcroc9", 5000, 1)
end
function Action_Saurcroc9(Unit, event, player)
Unit:SendChatMessage(12, 0, message[13])
Unit:RegisterEvent("Action_Saurcroc10", 10000, 1)
end

function Action_Hurlenfer5(Unit, event, player)
Unit:SendChatMessage(12, 0, message[14])
Unit:MoveTo(2838.100098, 6186.850098, 84.683296, 3.834910)
Unit:RegisterEvent("Action_Hurlenfer6", 45000, 1)
end

function Action_Saurcroc10(Unit, event, player)
Unit:SendChatMessage(12, 0, message[15])
Unit:RegisterEvent("Action_Saurcroc11", 5000, 1)
end
function Action_Saurcroc11(Unit, event, player)
Unit:SendChatMessage(12, 0, message[16])
Unit:RegisterEvent("Action_Saurcroc12", 5000, 1)
end
function Action_Saurcroc12(Unit, event, player)
Unit:SendChatMessage(12, 0, message[17])
Unit:RegisterEvent("Action_Saurcroc13", 5000, 1)
end
function Action_Saurcroc13(Unit, event, player)
Unit:SendChatMessage(12, 0, message[18])
Unit:RegisterEvent("Action_Saurcroc14", 5000, 1)
end
function Action_Saurcroc14(Unit, event, player)
Unit:SendChatMessage(12, 0, message[19])
Unit:RegisterEvent("Action_Saurcroc15", 5000, 1)
end
function Action_Saurcroc15(Unit, event, player)
Unit:SendChatMessage(12, 0, message[20])
Unit:RegisterEvent("Action_Saurcroc16", 5000, 1)
end
function Action_Saurcroc16(Unit, event, player)
Unit:SendChatMessage(12, 0, message[21])
Unit:RegisterEvent("Action_Saurcroc17", 5000, 1)
end
function Action_Saurcroc17(Unit, event, player)
Unit:SendChatMessage(12, 0, message[22])
Unit:RegisterEvent("Action_Saurcroc18", 10000, 1)
end

function Action_Hurlenfer6(Unit, event, player)
Unit:SendChatMessage(12, 0, message[23])
Unit:RegisterEvent("Action_Hurlenfer7", 15000, 1)
end

function Action_Saurcroc18(Unit, event, player)
Unit:SendChatMessage(12, 0, message[24])
Unit:RegisterEvent("Action_Saurcroc19", 5000, 1)
end
function Action_Saurcroc19(Unit, event, player)
Unit:SendChatMessage(12, 0, message[25])
Unit:RegisterEvent("Action_Saurcroc20", 5000, 1)
end

function Action_Hurlenfer7(Unit, event, player)
Unit:SendChatMessage(12, 0, message[26])
Unit:RegisterEvent("Action_Hurlenfer", 70000, 1)
end

function Action_Saurcroc20(Unit, event, player)
Unit:SendChatMessage(12, 0, message[27])
Unit:RegisterEvent("Action_Saurcroc", 45000, 1)
end

function Initialisation_Saurcroc(Unit, event, player)
Unit:RegisterEvent("Action_Saurcroc", 20000, 1)
end

function Initialisation_Hurlenfer(Unit, event, player)
Unit:RegisterEvent("Action_Hurlenfer", 45000, 1)
end

RegisterUnitEvent(25256, 18, "Initialisation_Saurcroc")
RegisterUnitEvent(25237, 18, "Initialisation_Hurlenfer")