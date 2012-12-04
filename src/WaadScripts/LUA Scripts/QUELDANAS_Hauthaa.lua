local cry_delay = 30000
local message = {}
local announcei = 2
local choice = 1

message[1] = "Je peux vous dire que sans vous toute cette op\195\169ration aurait \195\169t\195\169 un \195\169chec."
message[2] = "C\'est note h\195\169ros qui est l\195\160 ! Comment allez-vous \196\177"

function Action_Hauthaa(Unit, event, player)
local choice = math.random(1, announcei)
local plr = Unit:GetClosestPlayer()
if (plr ~= nil) then
	mob:SetTauntedBy(plr)
end
if choice == 1 then
	Unit:SendChatMessage(12, 0, message[1])
	Unit:Emote(1)
elseif choice == 2 then
	Unit:SendChatMessage(12, 0, message[2])
	Unit:Emote(6)
end
end

RegisterUnitEvent(25046, 10, "Action_Hauthaa")