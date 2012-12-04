--[[ UNIT - Elwynn Forest - Hogger.lua

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

function HoggerCharge(Unit, Event)
	if Unit:GetClosestPlayer() then
		Unit:SendChatMessage(12, 0, "A l'attaaaaque !!!!")
		Unit:FullCastSpellOnTarget(6268, Unit:GetClosestPlayer());
	end
end

RegisterUnitEvent(448, 1, "HoggerCharge");
