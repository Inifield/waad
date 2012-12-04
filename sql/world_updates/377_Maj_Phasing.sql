-- Creature_spawns
UPDATE creature_spawns SET phasemask=1 WHERE phasemask=0;

-- Gameobject_spawns
ALTER TABLE `gameobject_spawns` MODIFY COLUMN `phaseMask` INTEGER UNSIGNED NOT NULL DEFAULT 1;
UPDATE gameobject_spawns SET phaseMask=1 WHERE phaseMask=0;