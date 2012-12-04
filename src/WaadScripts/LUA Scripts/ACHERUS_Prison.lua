--[[ ARCHERUS - ARCHERUS_Prison.lua

********************************
*                              *
*   WaadLUA Scripting Project  *
*                              *
********************************

This software is provided as free and open source by the
staff of WAAD Community, in accordance with the GPL license. 
This means we provide the software we have created freely 
and it has been thoroughly tested to work for the developers, 
but NO GUARANTEE is made it will work for you as well. 
Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

~~End of License Agreement

-- Script by Randdrick for WAAD
-- Staff of WAAD, December 2010 ]]

plr_aggro_prison = 0
local go_aggro = 0
local npcid = 29567
local npcid1 = 29566
local npcid2 = 29565
local npcid3 = 29520
local npcid4 = 29519

function Prison_onUse (Unit, Event)
plr_aggro_prison = Unit:GetClosestPlayer()
if (plr_aggro_prison ~= nil) then
go_aggro = 1
end
end

function Prison_onUseMob(Unit, event)
-- Unit:SetFaction(2093);
if go_aggro == 1 then
if (plr_aggro_prison ~= nil) then
local player_sight_dist = Unit:GetDistance(plr_aggro_prison);
if player_sight_dist < 20 then
Unit:RegisterEvent("Prison_onUseMob1", 1000, 1);
go_aggro = 0
end
end
end
Unit:RegisterEvent("Prison_onUseMob", 1000, 1);
end

function Prison_onUseMob1(Unit, event)
local player_race = plr_aggro_prison:GetPlayerRace();
local race_player = "Chevalier.";
if player_race == 1 then
race_player = "Humain."
end
if player_race == 2 then
race_player = "Orc."
end
if player_race == 3 then
race_player = "Nain."
end
if player_race == 4 then
race_player = "Elfe de la nuit."
end
if player_race == 5 then
race_player = "Mort-vivant."
end
if player_race == 6 then
race_player = "Tauren."
end
if player_race == 7 then
race_player = "Gnome."
end
if player_race == 8 then
race_player = "Troll."
end
if player_race == 10 then
race_player = "Elfe de sang."
end
if player_race == 11 then
race_player = "Draeneï."
end
Unit:SendChatMessage(12, 0, "C\'est sans espoir, vous n\'\195\170tes pas \195\160 la hauteur, " .. race_player);
Unit:RegisterEvent("Prison_onUseMob2", 4000, 1);
end

function Prison_onUseMob2(Unit, event)
Unit:SendChatMessage(12, 0, "HA ! Vous arrivez \195\160 peine \195\160 soulever votre arme ! Vous aurez une mort rapide.");
Unit:RegisterEvent("Prison_onUseMob3", 4000, 1);
end

function Prison_onUseMob3(Unit, event)
local model = Unit:GetModel();
if model == 25354 then
Unit:SetModel(25448)
end
if model == 25355 then
Unit:SetModel(25404)
end
if model == 25356 then
Unit:SetModel(25414)
end
if model == 25357 then
Unit:SetModel(25420)
end
if model == 25358 then
Unit:SetModel(25418)
end
if model == 25359 then
Unit:SetModel(25416)
end
if model == 25360 then
Unit:SetModel(25410)
end
if model == 25361 then
Unit:SetModel(25406)
end
if model == 25362 then
Unit:SetModel(25408)
end
if model == 25363 then
Unit:SetModel(25412)
end
if model == 25364 then
Unit:SetModel(25424)
end
if model == 25365 then
Unit:SetModel(25428)
end
if model == 25366 then
Unit:SetModel(25430)
end
if model == 25367 then
Unit:SetModel(25426)
end
if model == 25368 then
Unit:SetModel(25436)
end
if model == 25369 then
Unit:SetModel(25432)
end
if model == 25370 then
Unit:SetModel(25434)
end
if model == 25371 then
Unit:SetModel(25440)
end
if model == 25372 then
Unit:SetModel(25438)
end
if model == 25373 then
Unit:SetModel(25422)
end
Unit:SetFaction(14);
Unit:SetFlags(32768);
Unit:SetEquipModel(12764, 0, 0);
Unit:SetTauntedBy(plr_aggro_prison);
end

function Prison_onLeaveMob(Unit, event)
local model = Unit:GetModel();
if model == 25448 then
Unit:SetModel(25354, 0, 0)
end
if model == 25404 then
Unit:SetModel(25355)
end
if model == 25414 then
Unit:SetModel(25356)
end
if model == 25420 then
Unit:SetModel(25357)
end
if model == 25418 then
Unit:SetModel(25358)
end
if model == 25416 then
Unit:SetModel(25359)
end
if model == 25410 then
Unit:SetModel(25360)
end
if model == 25406 then
Unit:SetModel(25361)
end
if model == 25408 then
Unit:SetModel(25362)
end
if model == 25412 then
Unit:SetModel(25363)
end
if model == 25424 then
Unit:SetModel(25364)
end
if model == 25428 then
Unit:SetModel(25365)
end
if model == 25430 then
Unit:SetModel(25366)
end
if model == 25426 then
Unit:SetModel(25367)
end
if model == 25436 then
Unit:SetModel(25368)
end
if model == 25432 then
Unit:SetModel(25369)
end
if model == 25434 then
Unit:SetModel(25370)
end
if model == 25440 then
Unit:SetModel(25371)
end
if model == 25438 then
Unit:SetModel(25372)
end
if model == 25422 then
Unit:SetModel(25373)
end
Unit:SetFaction(2093);
Unit:SetFlags(33024);
Unit:SetEquipModel(0, 0, 0);
Unit:RegisterEvent("Prison_onKillMob", 1000, 1);
end

function Prison_onKillMob(Unit, event, player)
Unit:SendChatMessage(12, 0, "Arrrrggghhhhhhh...");
player:MarkQuestKillObjectiveAsComplete(12848, 29579, 0);
Unit:RemoveEvents();
end

--
RegisterUnitEvent (29567, 18, "Prison_onUseMob")
RegisterUnitEvent (29566, 18, "Prison_onUseMob")
RegisterUnitEvent (29565, 18, "Prison_onUseMob")
RegisterUnitEvent (29520, 18, "Prison_onUseMob")
RegisterUnitEvent (29519, 18, "Prison_onUseMob")
RegisterUnitEvent (29567, 2, "Prison_onLeaveMob")
RegisterUnitEvent (29566, 2, "Prison_onLeaveMob")
RegisterUnitEvent (29565, 2, "Prison_onLeaveMob")
RegisterUnitEvent (29520, 2, "Prison_onLeaveMob")
RegisterUnitEvent (29519, 2, "Prison_onLeaveMob")
--
RegisterUnitEvent (29567, 4, "Prison_onKillMob")
RegisterUnitEvent (29566, 4, "Prison_onKillMob")
RegisterUnitEvent (29565, 4, "Prison_onKillMob")
RegisterUnitEvent (29520, 4, "Prison_onKillMob")
RegisterUnitEvent (29519, 4, "Prison_onKillMob")
--
RegisterGameObjectEvent (191590, 4, "Prison_onUse")
RegisterGameObjectEvent (191589, 4, "Prison_onUse")
RegisterGameObjectEvent (191588, 4, "Prison_onUse")
RegisterGameObjectEvent (191587, 4, "Prison_onUse")
RegisterGameObjectEvent (191586, 4, "Prison_onUse")
RegisterGameObjectEvent (191585, 4, "Prison_onUse")
RegisterGameObjectEvent (191584, 4, "Prison_onUse")
RegisterGameObjectEvent (191583, 4, "Prison_onUse")
RegisterGameObjectEvent (191582, 4, "Prison_onUse")
RegisterGameObjectEvent (191581, 4, "Prison_onUse")
RegisterGameObjectEvent (191580, 4, "Prison_onUse")
RegisterGameObjectEvent (191577, 4, "Prison_onUse")