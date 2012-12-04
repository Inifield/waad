local cry_delay = 25000
local message = {}
local messmax = 14
local npcid = 25379
local npcid2 = 25426

message[1] = "Ils sont l\195\160 parce qu\'ils n\'ont pas voulu d\195\169fendre leurs foyers... leurs familles."
message[2] = "Ils ont r\195\169nonc\195\169 et ils se sont rendus \195\160 nous. Volontairement."
message[3] = "J\'comprend pas, p\195\180pa."
message[4] = "[En commun approximatif] Entendu le gar\195\167on, les l\195\162ches \196\177 Le gar\195\167on il meurt pour libert\195\169 !"

function Action_GardienNork(Unit, event, player)
Unit:SendChatMessage(12, 0, message[1])
Unit:RegisterEvent("Action_GardienNork2", 5000, 1)
end
function Action_GardienNork2(Unit, event, player)
Unit:SendChatMessage(12, 0, message[2])
Unit:RegisterEvent("Action_GardienNork3", 10000, 1)
end

function Action_Ugthor(Unit, event, player)
Unit:SendChatMessage(12, 0, message[3])
Unit:RegisterEvent("Action_Ugthor", 50000, 1)
end

function Action_GardienNork3(Unit, event, player)
Unit:SendChatMessage(12, 0, message[4])
Unit:RegisterEvent("Action_GardienNork", 60000, 1)
end

function Initialisation_GardienNork(Unit, event, player)
Unit:RegisterEvent("Action_GardienNork", 20000, 1)
end

function Initialisation_Ugthor(Unit, event, player)
Unit:RegisterEvent("Action_Ugthor", 30000, 1)
end

RegisterUnitEvent(npcid, 18, "Initialisation_GardienNork")
RegisterUnitEvent(npcid2, 18, "Initialisation_Ugthor")