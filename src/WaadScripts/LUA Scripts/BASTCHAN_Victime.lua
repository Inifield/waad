local npcid = 25284
local npcid2 = 31563
local faction = 1981
local message = {}

message[1] = "Ma hache est à vous, héros !\nEnsemble, nous allons détruire ces insectes !"
message[2] = "Je dois m'en retourner au bastion Chanteguerre. Continuez le combat !"
message[3] = "Je dois m'en retourner au bastion Chanteguerre. Puissiez-vous baigner dans le sang de nos ennemis et vous repaître de leur peine !"

function Victime_Mob_OnDie(Unit, event, player)
Unit:RemoveEvents();
Unit:StopMovement(0);
Unit:SetCombatCapable(1);
local Mob_x = Unit:GetX();
local Mob_y = Unit:GetY();
local Mob_z = Unit:GetZ();
local Mob_o = Unit:GetO();
Unit:SpawnCreature(npcid2, Mob_x, Mob_y, Mob_z, Mob_o, faction, 0);
end

RegisterUnitEvent(npcid, 4, "Victime_Mob_OnDie")