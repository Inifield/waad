local message = {}

message[1] = "Groupe 1, rechargez et prenez positions !"
message[2] = "Feu \195\160 volont\195\169 !"
message[3] = "Soldats, cessez le feu ! Groupe 2, voyons si vous pouvez faire mieux..."
message[4] = "Feu \195\160 volont\195\169 !"
message[5] = "J\'en ai vu assez. Si j\'\195\169tais vous, j\'irais au temple prier pour tirer plus droit au moment de v\195\169rit\195\169 !"

function Action_Veteran(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1,0)
end
function Action_Veteran2(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[2])
	Unit:Emote(25,0)
end
function Action_Veteran3(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[3])
	Unit:Emote(274,0)
end
function Action_Veteran4(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[4])
	Unit:Emote(25,0)
end
function Action_Veteran5(Unit, event, player)
	Unit:SendChatMessage(12, 0, message[5])
	Unit:Emote(274,0)
end

function Initialisation_Veteran(Unit, event, player)
Unit:RegisterEvent("Action_Veteran", 5000, 1)
Unit:RegisterEvent("Action_Veteran2", 10000, 1)
Unit:RegisterEvent("Action_Veteran3", 15000, 1)
Unit:RegisterEvent("Action_Veteran4", 20000, 1)
Unit:RegisterEvent("Action_Veteran5", 25000, 1)
Unit:RegisterEvent("Initialisation_Veteran", 120000, 1)
end

function Mort_Veteran(Unit, event, player)
Unit:RemoveEvents()
end

RegisterUnitEvent(25143, 18, "Initialisation_Veteran")
RegisterUnitEvent(25143, 4, "Mort_Veteran")