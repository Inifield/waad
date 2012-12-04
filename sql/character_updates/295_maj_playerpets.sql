ALTER TABLE `playerpets` ADD COLUMN `happinesslvl` INTEGER UNSIGNED NOT NULL DEFAULT 0 AFTER `happiness`;
ALTER TABLE `playerpets` CHANGE COLUMN `happiness` `currentloyalty` INTEGER NOT NULL DEFAULT 0;
