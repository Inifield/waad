ALTER TABLE worldmap_info
 CHANGE required_quest required_quest_A1 INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Quête requise pour l\'alliance',
 ADD required_quest_H2 INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Quête requise pour la horde' AFTER required_quest_A1;