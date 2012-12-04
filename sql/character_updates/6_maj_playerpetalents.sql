-- ----------------------------
-- Table structure for `playerpettalents`
-- ----------------------------
DROP TABLE IF EXISTS `playerpettalents`;
CREATE TABLE `playerpettalents` (
  `ownerguid` bigint(20) NOT NULL DEFAULT '0',
  `petnumber` int(4) NOT NULL DEFAULT '0',
  `talentid` int(11) NOT NULL DEFAULT '0',
  `rank` tinyint(4) NOT NULL DEFAULT '0',
  KEY `a` (`ownerguid`),
  KEY `b` (`petnumber`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of playerpettalents
-- ----------------------------