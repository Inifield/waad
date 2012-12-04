/*
--[[
*
*KappaScripts
*(c) Kappawow 2008, 2009, (http://kwow.puretrance.fr/) (Adresse temporaire)
*
--]]

--[[

WB-Zelpou.lua
World boss

Zelpou
<Le Poulet>

Par Skizot

Notes :
Ce script a été codé par Skizot pour le serveur KappaWoW


--]]

print "---"
print "KappaWoW : Zelpou.lua a ete lance"
print "(c) 2008, 2009 Kwow"
print "Écrit par skizot"
print "---"

--[[Defines]]--

local Zelpou_ID = 200002
local Poulet_Spawns_ID = 200003
local Zelpou_Dragon_Model = 13992
local Dragon_Spawns_Ground_ID = 200004
local Dragon_Spawns_Flying_ID = 200005
local Faction = 7 -- hostile à tous

--:GetRandomPlayer(flag)
   local TOUS             = 0
   local AVEC_MANA         = 4
   local AVEC_RAGE         = 5
   local AVEC_ENERGIE     = 6
   local TOUS_SAUF_TANK    = 7
  
--:SendChatMessage
    local TALK_MSG = 12
    local YELL_MSG = 14
  
RegisterUnitEvent(Zelpou_ID, 1, "Zelpou_OnEnterCombat")
RegisterUnitEvent(Zelpou_ID, 2, "Zelpou_OnLeaveCombat")
RegisterUnitEvent(Zelpou_ID, 4, "Zelpou_Death")
function Zelpou_OnLeaveCombat(pUnit,Event)
    pUnit:RemoveEvents()
end
function Zelpou_Death(pUnit,Event)
    pUnit:RemoveEvents()
end

--[[Phase 1  jusqu'à 98% de sa vie il reste idle]]--
function Zelpou_OnEnterCombat(pUnit,Event)
    pUnit:SetCombatMeleeCapable(1)
    pUnit:SendChatMessage(TALK_MSG, 0, "Couack-k?  Cott-Cott-Cott...")
    pUnit:RegisterEvent("Zelpou_Phase_2", 10000, 0)-- Toutes les 10 secondes il vérifie s'il peut entrer en phase 2
    
end


--[[Phase 2]]--
function Zelpou_Phase_2(pUnit,Event)
    if pUnit:GetHealthPct() < 98 then
        pUnit:RemoveEvents()
        pUnit:SetCombatMeleeCapable(1)
        pUnit:SendChatMessage(TALK_MSG, 0, "Cott...  Zelpou...  Cott...  a... Couack!...  Mal!")
        pUnit:RegisterEvent("Zelpou_Phase_2", 10000, 0)
            if pUnit:GetHealthPct() > 75 then
                pUnit:RegisterEvent("Zelpou_Summon_Poulets_4", 90000, 0) --  Il va sumonner 4 poulets à toutes les 1.5 minutes
            end
            if pUnit:GetHealthPct() < 74 then
                pUnit:SetScale(2)
                pUnit:RegisterEvent("Zelpou_Summon_Poulets_6", 90000, 0) --  Il va sumonner 6 poulets à toutes les 1.5 minutes
            end

        pUnit:RegisterEvent("Zelpou_Phase_3", 10000, 0) --  toutes les 10 sec on vérifie pour la phase 3
    end
end


--[[Phase 3]]--
function Zelpou_Phase_3(pUnit,Event) -- Plutôt une transition qu'une phase ^^
    if pUnit:GetHealthPct() < 20 then
        pUnit:RemoveEvents()
        pUnit:SetCombatMeleeCapable(1)
        pUnit:SetModel(Zelpou_Dragon_Model)
        pUnit:SetScale(3)
        pUnit:SendChatMessage(TALK_MSG, 0, "Vous croyez quoi?  Que je ne suis qu'un poulet insignifiant?")
        pUnit:SendChatMessage(TALK_MSG, 0, "Je suis le grand Zelpoulatou!")
        pUnit:SetHealthPct(80)
        pUnit:RegisterEvent("Zelpou_Phase_4", 1000, 0)
    end
end

--Phase 4
function Zelpou_Phase_4(pUnit,Event)
    pUnit:RemoveEvents()
    pUnit:SetCombatMeleeCapable(0) -- Il devient capable de combattre !
    pUnit:RegisterEvent("Summon_Flying_Whelps", 198000, 0) -- Toutes les 3.3 minutes il en invoque 2
    pUnit:RegisterEvent("Summon_Ground_Whelps", 138000, 0) -- Toutes les 2.3 minutes il en invoque 2
    pUnit:RegisterEvent("Zelpou_Volatile_Infection", 90000, 0)--24928 / Random tous
    pUnit:RegisterEvent("Zelpou_Time_Lapse", 87000, 0)--31467 / Random mana ou energie 87 secondes
    pUnit:RegisterEvent("Zelpou_Phase_5", 10000, 1) -- vérifie toutes les 10 secondes pour la phase 5
end

--Phase 5
function Zelpou_Phase_5(pUnit,Event)
    if pUnit:GetHealthPct() < 35 then
        pUnit:RemoveEvents()
        pUnit:SetCombatMeleeCapable(1)
        pUnit:SetFlying() -- il lui est possible de monter en z
        pUnit:SetMovementType(768)  -- Il a le mouvement d'ailes pour voler
        pUnit:RegisterEvent("Zelpou_Vol", 1000, 1) -- et il va s'envoler!
        pUnit:RegisterEvent("Summon_Ground_Whelps_Phase_5", 138000, 0)
        pUnit:RegisterEvent("Zelpou_Fire", 35000, 0)--35 secondes
        pUnit:RegisterEvent("Zelpou_Ice", 75000, 0)--75 secondes
        pUnit:RegisterEvent("Zelpou_Volatile_Infection", 90000, 0)--24928 / Random tous
        pUnit:RegisterEvent("Zelpou_Time_Lapse", 87000, 0)--31467 / Random mana ou energie 87 secondes
        pUnit:RegisterEvent("Zelpou_Phase_6", 10000, 1) -- vérifie toutes les 10 secondes pour la phase 6
    end
end

--Phase 6
function Zelpou_Phase_6(pUnit,Event)
    if pUnit:GetHealthPct() < 15 then
        pUnit:RegisterEvent("Zelpou_Descend", 1000, 1) -- et il va redescendre
        pUnit:RemoveEvents()
        pUnit:SetScale(2)
        pUnit:SetCombatMeleeCapable(0)
        pUnit:RegisterEvent("Summon_Flying_Whelps_Phase_6", 138000, 0)
        pUnit:RegisterEvent("Zelpou_Fire", 25000, 0)--25 secondes
        pUnit:RegisterEvent("Zelpou_Ice", 45000, 0)--45 secondes
        pUnit:RegisterEvent("Zelpou_Volatile_Infection", 90000, 0)--24928 / Random tous
        pUnit:RegisterEvent("Zelpou_Time_Lapse", 87000, 0)--31467 / Random mana ou energie 87 secondes
    end
end
--[[
function (pUnit,Event)
end
]]--

------------------------------------------------------------------------------------------------------------
--Les spawns
RegisterUnitEvent(Poulet_Spawns_ID, 1, "Poulet_Spawn_AI")
RegisterUnitEvent(Dragon_Spawns_Ground_ID, 1, "Dragon_Spawn_AI_Ground")
RegisterUnitEvent(Dragon_Spawns_Flying_ID, 1, "Dragon_Spawn_AI_Flying")


function Poulet_Spawn_AI(pUnit,Event)
    pUnit:RegisterEvent("Poulet_Spawn_Attaque", 20000, 0) -- Attaque aux 20 sec
end
function Dragon_Spawn_AI_Ground(pUnit,Event)
    pUnit:RegisterEvent("Dragon_Spawn_Attaque", 20000, 0) -- Attaque aux 20 sec
    pUnit:RegisterEvent("Dragon_Spawn_Berserk", 1000, 1) -- Istant 1 fois lorsquil spawn
    pUnit:RegisterEvent("Dragon_Spawn_Berserk", 300000, 0) -- aux 5 min par la suite
end
--
function Dragon_Spawn_AI_Flying(pUnit,Event)
    pUnit:SetCombatMeleeCapable(1)
    pUnit:SetFlying() -- il lui est possible de monter en z
    pUnit:SetMovementType(768)  -- Il a le mouvement d'ailes pour voler
    pUnit:RegisterEvent("Dragon_Spawn_Attaque", 20000, 0) -- Attaque aux 20 sec
    pUnit:RegisterEvent("Dragon_Spawn_Berserk", 1000, 1) -- Istant 1 fois lorsquil spawn
    pUnit:RegisterEvent("Dragon_Spawn_Berserk", 300000, 0) -- aux 5 min par la suite
end

function Poulet_Spawn_Attaque(pUnit,Event)
    Choice=math.random(1, 2)
        if Choice==1 then
        pUnit:FullCastSpellOnTarget(39068, pUnit:GetRandomPlayer(0))--Feather Burst
        end
        --
        if Choice==2 then
        pUnit:FullCastSpellOnTarget(24193, pUnit:GetRandomPlayer(0))--CHARGE
        end
    end
end

function Dragon_Spawn_Berserk(pUnit,Event)
    pUnit:CastSpell(26662)--Berserk --026662
end

function Dragon_Spawn_Attaque(pUnit,Event)
    Choice=math.random(1, 3)
        --
        if Choice==1 then
        pUnit:FullCastSpellOnTarget(28531, pUnit:GetClosestPlayer())--28531 - frost aura
        end
        --
        if Choice==2 then
        pUnit:FullCastSpellOnTarget(28522, pUnit:GetClosestPlayer())--icebolt
        end
        --
        if Choice==3 then
        pUnit:FullCastSpellOnTarget(45108, pUnit:GetClosestPlayer())--FireBall
        end
    end
end

--------------------------------------------------------------------------------------------------------------

function Zelpou_Summon_Poulets_4(pUnit,Event)-- Summon de 4 poulets
local x = pUnit:GetX()
local y = pUnit:GetY()
local z = pUnit:GetZ()
pUnit:SpawnCreature(Poulet_Spawns_ID,x,y-3,z,Faction,180000)--3 minutes
pUnit:SpawnCreature(Poulet_Spawns_ID,x,y+3,z,Faction,180000)
pUnit:SpawnCreature(Poulet_Spawns_ID,x-3,y,z,Faction,180000)
pUnit:SpawnCreature(Poulet_Spawns_ID,x+3,y,z,Faction,180000)
end
--
function Zelpou_Summon_Poulets_6(pUnit,Event)-- Summon de 6 poulets
local x = pUnit:GetX()
local y = pUnit:GetY()
local z = pUnit:GetZ()
pUnit:SpawnCreature(Poulet_Spawns_ID,x,y-3,z,Faction,180000)--3 minutes
pUnit:SpawnCreature(Poulet_Spawns_ID,x,y+3,z,Faction,180000)
pUnit:SpawnCreature(Poulet_Spawns_ID,x-3,y,z,Faction,180000)
pUnit:SpawnCreature(Poulet_Spawns_ID,x+3,y,z,Faction,180000)
pUnit:SpawnCreature(Poulet_Spawns_ID,x+1,y+1,z,Faction,180000)
pUnit:SpawnCreature(Poulet_Spawns_ID,x-1,y-1,z,Faction,180000)
end
--
function Summon_Ground_Whelps(pUnit,Event)-- Summon de 2 dragons au sol
local x = pUnit:GetX()
local y = pUnit:GetY()
local z = pUnit:GetZ()
pUnit:SpawnCreature(Dragon_Spawns_Ground_ID,x+3,y,z,Faction,360000)--6 minutes
pUnit:SpawnCreature(Dragon_Spawns_Ground_ID,x-3,y,z,Faction,360000)
end
--
function Summon_Flying_Whelps(pUnit,Event)-- Summon de 2 dragons volants
local x = pUnit:GetX()
local y = pUnit:GetY()
local z = pUnit:GetZ()
pUnit:SpawnCreature(Dragon_Spawns_Flying_ID,x+3,y,z+6,Faction,360000)--6 minutes
pUnit:SpawnCreature(Dragon_Spawns_Flying_ID,x-3,y,z+6,Faction,360000)
end
--    
function Zelpou_Volatile_Infection(pUnit,Event)
        pUnit:FullCastSpellOnTarget(24928, pUnit:GetRandomPlayer(0))--Volatile infection
end
--
function Zelpou_Time_Lapse(pUnit,Event)
        pUnit:FullCastSpellOnTarget(31467, pUnit:GetRandomPlayer(math.random(4, 6)))-- Time lapse
end
--
function Zelpou_Fire(pUnit,Event)
        pUnit:CastSpellOnTarget(math.random(43245, 33816), pUnit:GetClosestPlayer())-- FireBlast:43245 OU : 33816 - Increate fire damage 86
end
--
function Zelpou_Ice(pUnit,Event)
        pUnit:FullCastSpellOnTarget(math.random(27808, 38234), pUnit:GetRandomPlayer())-- 27808 - frost blast ou Frost Shock 38234
end
*/