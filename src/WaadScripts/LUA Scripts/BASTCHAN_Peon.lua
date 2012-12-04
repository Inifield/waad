local cry_delay = 25000
local message = {}
local messmax = 8
local npcid = 27432

message[1] = "Moi pas aimer tas de cailloux plein d\'insectes."
message[2] = "A\195\175euh."
message[3] = "Moi sommeil."
message[4] = "Moi froid."
message[5] = "Marrant."
message[6] = "Quoi c\'est, le bruit ?"
message[7] = "Stop !"
message[8] = "Au travail ! Zug Zug !"

function Action_Mob_Peons(Unit, event, player)
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
elseif choice == 5 then
	Unit:SendChatMessage(12, 0, message[5])
	Unit:Emote(1,0)
elseif choice == 6 then
	Unit:SendChatMessage(12, 0, message[6])
	Unit:Emote(1,0)
elseif choice == 7 then
	Unit:SendChatMessage(12, 0, message[7])
	Unit:Emote(1,0)
elseif choice == 8 then
	Unit:SendChatMessage(12, 0, message[8])
	Unit:Emote(1,0)
end
end

function Peons_Speach_Mob(Unit, event, player)
	Unit:RegisterEvent("Action_Mob_Peons", cry_delay, 1)
end

RegisterUnitEvent(npcid, 18, "Peons_Speach_Mob")