-- ** **  ***  *    * *   **
-- * * *   *   *    **   *  *
-- *   *   *   *    **   ****
-- *   *  ***  ***  * *  *  *

-- [*]CE SCRIPT À ETE DEVELOPPE PAR VALKIRIE.
-- [*]IL RESTE LA PROPRIETEE INTELECTUEL DE BLIZZARD.

function DALARAN_Teleport (pGameObject, event, pMisc)
pMisc:Teleport (571, 5802.430176, 843.317993, 680.249023)
end
function DALARAN_Teleport2 (pGameObject, event, pMisc)
pMisc:Teleport (571, 5820.751465, 835.761841, 680.353699)
end

RegisterGameObjectEvent(194011, 4, "DALARAN_Teleport")
RegisterGameObjectEvent(194012, 4, "DALARAN_Teleport2")