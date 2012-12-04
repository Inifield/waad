local message = {}

-- Messages
message[1] = "On met la goule dans la fosse, ou alors on vas t\195\162ter du fouet !"

function Initialisation_GothikDK(Unit, event)
Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[1])
end
end
Unit:RegisterEvent("Initialisation_GothikDK", 20000, 1);
end

RegisterUnitEvent (28658, 18, "Initialisation_GothikDK")