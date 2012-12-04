plr_use_oeuil = 0
ready_teleport = 1
local spell1 = 51859 --Siphon of Acherus
local spell2 = 51904 --Summon Ghouls On Scarlet Crusade
local spell3 = 51890 --Eye of Acherus Flight
local spell4 = 51852 --The Eye of Acherus
local spell5 = 51892 --Eye of Acherus Visual
local spell6 = 51887 --Recall Eye of Acherus

function OeuilAcherus_onUse(Unit, Event)
plr_use_oeuil = Unit:GetClosestPlayer()
-- Creation de l'Oeil
Unit:FullCastSpellOnTarget(spell3,plr_use_oeuil);
Unit:FullCastSpellOnTarget(spell4,plr_use_oeuil);
Unit:FullCastSpellOnTarget(spell5,plr_use_oeuil);
---
if ready_teleport == 1 then
plr_use_oeuil:Teleport(609, 2374.17, -5648.67, 507.20, 3.47)
ready_teleport = 0
end
end

function OeuilAcherus_onUseMob(Unit, event)
Unit:EnableFlight();
Unit:EnableMoveFly(1);

Unit:ModifyWalkSpeed(30);
Unit:ModifyFlySpeed(30);

if go_use == 1 then
if (plr_use_oeuil ~= nil) then
plr_use_oeuil:Possess(Unit);
Unit:SetPetOwner(plr_use_oeuil);
go_use = 0
end
end

Unit:RegisterEvent("OeuilAcherus_onUseMob", 1000, 1);

end

--[[function OeuilAcherus_onUseMobTeleport(Unit, event)
-- mob_x = Unit:GetX();
-- mob_y = Unit:GetY();
-- mob_z = Unit:GetZ();
-- mob_o = Unit:GetO();
Unit:ModifyWalkSpeed(30);
Unit:ModifyFlySpeed(30);
if ready_teleport == 1 then
plr_use_oeuil:Teleport(609, mob_x, mob_y, mob_z, mob_o)
end 
-- plr_use_oeuil:EnableFlight();
if go_use == 1 then
if (plr_use_oeuil ~= nil) then
plr_use_oeuil:Possess(ArcheusOeuil);
Unit:SetPetOwner(plr_use_oeuil);
local test = Unit:SetPetOwner(plr_use_oeuil);
-- print (test)
go_use = 0
ready_teleport = 1
end
end
Unit:RegisterEvent("OeuilAcherus_onUseMobTeleport", 1, 1);
end
]]
--

RegisterUnitEvent (28511, 18, "OeuilAcherus_onUseMob") --oeil d'Achérus
RegisterGameObjectEvent (191609, 4, "OeuilAcherus_onUse") --Mécanisme de contrôle de l'oeil d'Achérus
