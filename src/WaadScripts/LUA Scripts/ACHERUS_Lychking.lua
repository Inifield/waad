local message = {}

-- Messages
message[1] = "Je renais, mon roi, tout entier \195\160 votre service."

function LichKing_Whisper(Unit, Event)
Unit:RemoveEvents();
local chance = math.random(1, 16)
if chance == 1 then
Unit:PlaySoundToSet(14771)
Unit:SendChatMessage(15, 0, "Pas de piti\195\169...")
elseif chance == 2 then
Unit:PlaySoundToSet(14772)
Unit:SendChatMessage(15, 0, "Tue les tous...")
elseif chance == 3 then
Unit:PlaySoundToSet(14773)
Unit:SendChatMessage(15, 0, "La piti\195\169 c\'est pour les faibles...")
elseif chance == 4 then
Unit:PlaySoundToSet(14774)
Unit:SendChatMessage(15, 0, "Ach\195\168ve moi ça...")
elseif chance == 5 then
Unit:PlaySoundToSet(14775)
Unit:SendChatMessage(15, 0, "Fini ça...")
elseif chance == 6 then
Unit:PlaySoundToSet(14776)
Unit:SendChatMessage(15, 0, "Pas de survivants...")
elseif chance == 7 then
Unit:PlaySoundToSet(14777)
Unit:SendChatMessage(15, 0, "Tue ou soit tu\195\169...")
elseif chance == 8 then
Unit:PlaySoundToSet(14778)
Unit:SendChatMessage(15, 0, "N\'h\195\169site pas...")
elseif chance == 9 then
Unit:PlaySoundToSet(14779)
Unit:SendChatMessage(15, 0, "Tue de ta rage et concentre toi...")
elseif chance == 10 then
Unit:PlaySoundToSet(14780)
Unit:SendChatMessage(15, 0, "Met les \195\160 terre...")
elseif chance == 11 then
Unit:PlaySoundToSet(14781)
Unit:SendChatMessage(15, 0, "Douleur aux vaincues...")
elseif chance == 12 then
Unit:PlaySoundToSet(14782)
Unit:SendChatMessage(15, 0, "Montre lui ce qu\'est la terreur...")
elseif chance == 13 then
Unit:PlaySoundToSet(14783)
Unit:SendChatMessage(15, 0, "Met fin \195\160 sa mis\195\169rable existence...")
elseif chance == 14 then
Unit:PlaySoundToSet(14784)
Unit:SendChatMessage(15, 0, "Accepte les t\195\169n\195\168bres, chevalier de la mort...")
elseif chance == 15 then
Unit:PlaySoundToSet(14785)
Unit:SendChatMessage(15, 0, "Tu pense qu\'ils h\195\169siteraient \195\160 te tuer ? Un monstre comme toi...")
elseif chance == 16 then
Unit:PlaySoundToSet(14786)
Unit:SendChatMessage(15, 0, "Tu me servira dans la vie ou dans la mort...")
end
Unit:RegisterEvent("LichKing_Whisper", math.random(15000,30000), 0);
end

function Initialisation_DKLych(Unit, event)
Unit:RemoveEvents();
local player_sight = Unit:GetClosestPlayer()
if (player_sight ~= nil) then
local player_sight_dist = Unit:GetDistance(player_sight)
if player_sight_dist < 220 then
Unit:SendChatMessage(12, 0, message[1])
end
end
Unit:RegisterEvent("Initialisation_DKLych", 20000, 1);
end

RegisterUnitEvent (25462, 18, "LichKing_Whisper")
RegisterUnitEvent (28390, 18, "Initialisation_DKLych")