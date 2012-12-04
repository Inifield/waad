local cry_delay = 25000
local message = {}
local messmax = 4
local npcid = 25276

message[1] = "Et si t\'es un tel h\195\169ros et que t\'as tellement d'or, pourquoi tu viens ici perdre ton temps avec des rat\195\169s comme nous ?"
message[2] = "How do they expect us to craft with these things!"
message[3] = "Cette réunion, c\'est pour quoi ?"
message[4] = "Ces outils sont bons pour la poubelle !"

function Ingenieur_Action_Mob(Unit, event, player)
local choice = math.random(1, messmax)
if choice == 1 then
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1,0)
elseif choice == 2 then
	Unit:SendChatMessage(12, 0, message[2])
	Unit:Emote(1,0)
elseif choice == 3 then
	Unit:SendChatMessage(12, 0, message[3])
	Unit:Emote(1,0)
elseif choice == 4 then
	Unit:SendChatMessage(12, 0, message[4])
	Unit:Emote(1,0)
end
end

function Ingenieur_Speach_Mob(Unit, event, player)
	Unit:RegisterEvent("Ingenieur_Action_Mob", cry_delay, 1)
end

RegisterUnitEvent(npcid, 18, "Ingenieur_Speach_Mob")