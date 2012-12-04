--[[ HURLEVENT - TORPPER_McNabb.lua

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
-- Staff of WAAD, January 2011 ]]

local messagemax = 8
-- Messages
local message1 = "C\'est enti\195\168rement leur faute \195\160 ces imb\195\169ciles de soldats de l\'Alliance. Il fallait qu\'ils aillent construire leurs tours juste derri\195\168re ma ferme."
local message2 = "Je te paierai volontiers mardi pour avoir un bon steak aujourd\'hui. "
local message3 = "Quelques pi\195\168ces pour un pauvre aveugle ?... Qu\'est-ce que ça veut dire, je ne suis pas aveugle ?... JE NE SUIS PAS AVEUGLE ! JE VOIS ! C\'est un miracle !"
local message4 = "C\'est enti\195\168rement leur faute \195\160 ces imb\195\169ciles d'orcs. Il fallait qu\'ils r\195\169duisent ma ferme en cendres !"
local message5 = "Je ferai reluire votre armure pour une simple pi\195\168ce de cuivre."
local message6 = "La charit\195\169 ?"
local message7 = "Vous n\'auriez pas une ou deux pi\195\168ces ?"
local message8 = "Un coup de main \195\160 un pauvre malheureux ?"

function TORPPERMcNabb_message (Unit, event)
local RandChoice = math.random(1, messagemax)

if RandChoice == 1 then
Unit:SendChatMessage(12, 0, message1)
end
if RandChoice == 2 then
Unit:SendChatMessage(12, 0, message2)
end
if RandChoice == 3 then
Unit:SendChatMessage(12, 0, message3)
end
if RandChoice == 4 then
Unit:SendChatMessage(12, 0, message4)
end
if RandChoice == 5 then
Unit:SendChatMessage(12, 0, message5)
end
if RandChoice == 6 then
Unit:SendChatMessage(12, 0, message6)
end
if RandChoice == 7 then
Unit:SendChatMessage(12, 0, message7)
end
if RandChoice == 8 then
Unit:SendChatMessage(12, 0, message8)
end

end

RegisterUnitGossipEvent(1402, 1, "TORPPERMcNabb_message")