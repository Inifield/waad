-- ----------------------------
-- Table structure for randomitemcreation
-- ----------------------------
DROP TABLE IF EXISTS `randomitemcreation`;
CREATE TABLE `randomitemcreation` (
  `SpellId` int(10) unsigned NOT NULL default '0',
  `ItemToCreate` int(10) unsigned NOT NULL default '0',
  `Skill` int(10) unsigned NOT NULL default '0',
  `Chance` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ItemToCreate`,`SpellId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
