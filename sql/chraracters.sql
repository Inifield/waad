-- MySQL Administrator dump 1.4
--
-- ------------------------------------------------------
-- Server version	5.1.30-community


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;



DROP TABLE IF EXISTS `_blizzrequirements`;
CREATE TABLE `_blizzrequirements` (
  `acct_id` bigint(20) unsigned NOT NULL DEFAULT '0',
  `AlreadyDK` bigint(20) unsigned NOT NULL DEFAULT '0',
  `MaxLevelGet` int(10) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`acct_id`),
  UNIQUE KEY `a` (`acct_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `_blizzrequirements` DISABLE KEYS */;
/*!40000 ALTER TABLE `_blizzrequirements` ENABLE KEYS */;


DROP TABLE IF EXISTS `account_data`;
CREATE TABLE `account_data` (
  `acct` int(30) NOT NULL,
  `uiconfig0` blob,
  `uiconfig1` blob,
  `uiconfig2` blob,
  `uiconfig3` blob,
  `uiconfig4` blob,
  `uiconfig5` blob,
  `uiconfig6` blob,
  `uiconfig7` blob,
  `uiconfig8` blob,
  PRIMARY KEY (`acct`),
  UNIQUE KEY `a` (`acct`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `account_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_data` ENABLE KEYS */;


DROP TABLE IF EXISTS `account_forced_permissions`;
CREATE TABLE `account_forced_permissions` (
  `login` varchar(50) NOT NULL,
  `permissions` varchar(100) NOT NULL,
  PRIMARY KEY (`login`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `account_forced_permissions` DISABLE KEYS */;
/*!40000 ALTER TABLE `account_forced_permissions` ENABLE KEYS */;


DROP TABLE IF EXISTS `accounts`;
CREATE TABLE `accounts` (
  `acct` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `login` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `password` varchar(28) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `encrypted_password` varchar(42) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `gm` varchar(27) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `banned` int(1) unsigned NOT NULL DEFAULT '0',
  `lastlogin` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `lastip` varchar(15) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `email` varchar(50) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `flags` int(11) unsigned NOT NULL DEFAULT '0',
  `forceLanguage` varchar(5) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'frFR',
  `muted` int(30) NOT NULL DEFAULT '0',
  PRIMARY KEY (`acct`,`login`),
  UNIQUE KEY `acct` (`acct`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*!40000 ALTER TABLE `accounts` DISABLE KEYS */;
/*!40000 ALTER TABLE `accounts` ENABLE KEYS */;


DROP TABLE IF EXISTS `achievements`;
CREATE TABLE `achievements` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `player` int(11) NOT NULL,
  `achievementid` int(11) NOT NULL,
  `progress` varchar(255) NOT NULL,
  `completed` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `c` (`player`,`achievementid`),
  KEY `a` (`player`),
  KEY `b` (`achievementid`)
) ENGINE=MyISAM AUTO_INCREMENT=20214 DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `achievements` DISABLE KEYS */;
/*!40000 ALTER TABLE `achievements` ENABLE KEYS */;


DROP TABLE IF EXISTS `arenateams`;
CREATE TABLE `arenateams` (
  `id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Unique ID',
  `type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Type: 0-2v2 1-3v3 2-5v5',
  `leader` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Leading character guid',
  `name` varchar(24) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Team name',
  `emblemstyle` int(10) unsigned NOT NULL DEFAULT '0',
  `emblemcolour` int(10) unsigned NOT NULL DEFAULT '0',
  `borderstyle` int(10) unsigned NOT NULL DEFAULT '0',
  `bordercolor` int(10) unsigned NOT NULL DEFAULT '0',
  `backgroundcolor` int(10) unsigned NOT NULL DEFAULT '0',
  `rating` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Server Rating',
  `data` varchar(24) COLLATE utf8_unicode_ci NOT NULL COMMENT '(played_week) (won_week) (played_season) (won_season)',
  `ranking` int(10) unsigned NOT NULL COMMENT 'Server Ranking',
  `player_data1` varchar(32) COLLATE utf8_unicode_ci NOT NULL COMMENT '(player_guid) (played_week) (won_week) (played_season) (won_season)',
  `player_data2` varchar(32) COLLATE utf8_unicode_ci NOT NULL COMMENT '(player_guid) (played_week) (won_week) (played_season) (won_season)',
  `player_data3` varchar(32) COLLATE utf8_unicode_ci NOT NULL COMMENT '(player_guid) (played_week) (won_week) (played_season) (won_season)',
  `player_data4` varchar(32) COLLATE utf8_unicode_ci NOT NULL COMMENT '(player_guid) (played_week) (won_week) (played_season) (won_season)',
  `player_data5` varchar(32) COLLATE utf8_unicode_ci NOT NULL COMMENT '(player_guid) (played_week) (won_week) (played_season) (won_season)',
  `player_data6` varchar(32) COLLATE utf8_unicode_ci NOT NULL COMMENT '(player_guid) (played_week) (won_week) (played_season) (won_season)',
  `player_data7` varchar(32) COLLATE utf8_unicode_ci NOT NULL COMMENT '(player_guid) (played_week) (won_week) (played_season) (won_season)',
  `player_data8` varchar(32) COLLATE utf8_unicode_ci NOT NULL COMMENT '(player_guid) (played_week) (won_week) (played_season) (won_season)',
  `player_data9` varchar(32) COLLATE utf8_unicode_ci NOT NULL COMMENT '(player_guid) (played_week) (won_week) (played_season) (won_season)',
  `player_data10` varchar(32) COLLATE utf8_unicode_ci NOT NULL COMMENT '(player_guid) (played_week) (won_week) (played_season) (won_season)',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Arena Teams';

/*!40000 ALTER TABLE `arenateams` DISABLE KEYS */;
/*!40000 ALTER TABLE `arenateams` ENABLE KEYS */;


DROP TABLE IF EXISTS `auctions`;
CREATE TABLE `auctions` (
  `auctionId` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `auctionhouse` int(11) unsigned DEFAULT NULL,
  `item` bigint(10) unsigned DEFAULT NULL,
  `owner` int(11) unsigned DEFAULT NULL,
  `buyout` int(11) unsigned DEFAULT NULL,
  `time` int(11) unsigned DEFAULT NULL,
  `bidder` int(11) unsigned DEFAULT NULL,
  `bid` int(11) unsigned DEFAULT NULL,
  `deposit` int(11) unsigned DEFAULT NULL,
  PRIMARY KEY (`auctionId`)
) ENGINE=MyISAM AUTO_INCREMENT=11 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*!40000 ALTER TABLE `auctions` DISABLE KEYS */;
/*!40000 ALTER TABLE `auctions` ENABLE KEYS */;


DROP TABLE IF EXISTS `banned_names`;
CREATE TABLE `banned_names` (
  `name` varchar(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `banned_names` DISABLE KEYS */;
/*!40000 ALTER TABLE `banned_names` ENABLE KEYS */;


DROP TABLE IF EXISTS `banned_phrases`;
CREATE TABLE `banned_phrases` (
  `phrase` varchar(250) NOT NULL,
  PRIMARY KEY (`phrase`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `banned_phrases` DISABLE KEYS */;
/*!40000 ALTER TABLE `banned_phrases` ENABLE KEYS */;


DROP TABLE IF EXISTS `characters`;
CREATE TABLE `characters` (
  `guid` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `acct` int(10) unsigned NOT NULL,
  `name` varchar(21) NOT NULL DEFAULT '',
  `race` tinyint(3) unsigned NOT NULL,
  `class` tinyint(3) unsigned NOT NULL,
  `gender` tinyint(3) unsigned NOT NULL,
  `custom_faction` int(10) unsigned NOT NULL DEFAULT '0',
  `level` tinyint(3) unsigned NOT NULL,
  `xp` int(10) unsigned NOT NULL,
  `exploration_data` longtext NOT NULL,
  `skills` longtext NOT NULL,
  `watched_faction_index` bigint(30) NOT NULL DEFAULT '0',
  `selected_pvp_title` int(10) unsigned NOT NULL DEFAULT '0',
  `available_pvp_titles1` bigint(30) unsigned NOT NULL DEFAULT '0',
  `available_pvp_titles2` bigint(30) unsigned NOT NULL,
  `gold` int(10) unsigned NOT NULL DEFAULT '0',
  `ammo_id` int(10) unsigned NOT NULL DEFAULT '0',
  `available_prof_points` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `total_talent_points` int(30) NOT NULL,
  `current_hp` int(10) unsigned NOT NULL DEFAULT '0',
  `current_power` int(10) unsigned NOT NULL DEFAULT '0',
  `pvprank` int(10) unsigned NOT NULL DEFAULT '0',
  `bytes` int(10) unsigned NOT NULL DEFAULT '0',
  `bytes2` int(10) unsigned NOT NULL DEFAULT '0',
  `player_flags` int(10) unsigned NOT NULL DEFAULT '0',
  `player_bytes` int(10) unsigned NOT NULL DEFAULT '0',
  `positionX` float NOT NULL DEFAULT '0',
  `positionY` float NOT NULL DEFAULT '0',
  `positionZ` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `mapId` int(10) unsigned NOT NULL DEFAULT '0',
  `zoneId` int(10) unsigned NOT NULL DEFAULT '0',
  `taximask` longtext NOT NULL,
  `banned` int(40) NOT NULL,
  `banReason` varchar(50) NOT NULL,
  `timestamp` int(10) unsigned NOT NULL DEFAULT '0',
  `online` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `bindpositionX` float NOT NULL DEFAULT '0',
  `bindpositionY` float NOT NULL DEFAULT '0',
  `bindpositionZ` float NOT NULL DEFAULT '0',
  `bindmapId` int(10) unsigned NOT NULL DEFAULT '0',
  `bindzoneId` int(10) unsigned NOT NULL DEFAULT '0',
  `isResting` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `restState` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `restTime` int(10) unsigned NOT NULL DEFAULT '0',
  `playedtime` longtext NOT NULL,
  `deathstate` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `TalentResetTimes` int(10) unsigned NOT NULL DEFAULT '0',
  `first_login` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `forced_rename_pending` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `arenaPoints` int(10) unsigned NOT NULL DEFAULT '0',
  `totalstableslots` int(10) unsigned NOT NULL DEFAULT '0',
  `instance_id` int(10) unsigned NOT NULL DEFAULT '0',
  `entrypointmap` int(10) unsigned NOT NULL DEFAULT '0',
  `entrypointx` float NOT NULL DEFAULT '0',
  `entrypointy` float NOT NULL DEFAULT '0',
  `entrypointz` float NOT NULL DEFAULT '0',
  `entrypointo` float NOT NULL DEFAULT '0',
  `entrypointinstance` int(10) unsigned NOT NULL DEFAULT '0',
  `taxi_path` int(10) unsigned NOT NULL DEFAULT '0',
  `taxi_lastnode` int(10) unsigned NOT NULL DEFAULT '0',
  `taxi_mountid` int(10) unsigned NOT NULL DEFAULT '0',
  `transporter` int(10) unsigned NOT NULL DEFAULT '0',
  `transporter_xdiff` float NOT NULL DEFAULT '0',
  `transporter_ydiff` float NOT NULL DEFAULT '0',
  `transporter_zdiff` float NOT NULL DEFAULT '0',
  `deleted_spells` longtext NOT NULL,
  `reputation` longtext NOT NULL,
  `actions` longtext NOT NULL,
  `auras` longtext NOT NULL,
  `finished_quests` longtext NOT NULL,
  `finished_daily_quests` longtext NOT NULL,
  `honorRolloverTime` int(30) NOT NULL DEFAULT '0',
  `killsToday` int(10) unsigned NOT NULL DEFAULT '0',
  `killsYesterday` int(10) unsigned NOT NULL DEFAULT '0',
  `killsLifeTime` int(10) unsigned NOT NULL DEFAULT '0',
  `honorToday` int(10) unsigned NOT NULL DEFAULT '0',
  `honorYesterday` int(10) unsigned NOT NULL DEFAULT '0',
  `honorPoints` int(10) unsigned NOT NULL DEFAULT '0',
  `difficulty` int(10) unsigned NOT NULL DEFAULT '0',
  `height` float NOT NULL DEFAULT '1',
  `active_spec` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `specs_count` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `need_talent_reset` int(3) NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=MyISAM AUTO_INCREMENT=10 DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `characters` DISABLE KEYS */;
/*!40000 ALTER TABLE `characters` ENABLE KEYS */;


DROP TABLE IF EXISTS `characters_insert_queue`;
CREATE TABLE `characters_insert_queue` (
  `guid` int(10) unsigned NOT NULL,
  `acct` int(10) unsigned NOT NULL,
  `name` varchar(21) NOT NULL,
  `race` tinyint(3) unsigned NOT NULL,
  `class` tinyint(3) unsigned NOT NULL,
  `gender` tinyint(3) unsigned NOT NULL,
  `custom_faction` int(10) unsigned NOT NULL DEFAULT '0',
  `level` tinyint(3) unsigned NOT NULL,
  `xp` int(10) unsigned NOT NULL,
  `exploration_data` longtext NOT NULL,
  `skills` longtext NOT NULL,
  `watched_faction_index` int(10) unsigned NOT NULL DEFAULT '0',
  `selected_pvp_title` int(10) unsigned NOT NULL DEFAULT '0',
  `available_pvp_titles` int(10) unsigned NOT NULL DEFAULT '0',
  `gold` int(10) unsigned NOT NULL DEFAULT '0',
  `ammo_id` int(10) unsigned NOT NULL DEFAULT '0',
  `available_prof_points` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `available_talent_points` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `current_hp` int(10) unsigned NOT NULL DEFAULT '0',
  `current_power` int(10) unsigned NOT NULL DEFAULT '0',
  `pvprank` int(10) unsigned NOT NULL DEFAULT '0',
  `bytes` int(10) unsigned NOT NULL DEFAULT '0',
  `bytes2` int(10) unsigned NOT NULL DEFAULT '0',
  `player_flags` int(10) unsigned NOT NULL DEFAULT '0',
  `player_bytes` int(10) unsigned NOT NULL DEFAULT '0',
  `positionX` float NOT NULL DEFAULT '0',
  `positionY` float NOT NULL DEFAULT '0',
  `positionZ` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `mapId` int(10) unsigned NOT NULL DEFAULT '0',
  `zoneId` int(10) unsigned NOT NULL DEFAULT '0',
  `taximask` longtext NOT NULL,
  `banned` int(40) NOT NULL,
  `banReason` varchar(50) NOT NULL,
  `timestamp` int(10) unsigned NOT NULL DEFAULT '0',
  `online` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `bindpositionX` float NOT NULL DEFAULT '0',
  `bindpositionY` float NOT NULL DEFAULT '0',
  `bindpositionZ` float NOT NULL DEFAULT '0',
  `bindmapId` int(10) unsigned NOT NULL DEFAULT '0',
  `bindzoneId` int(10) unsigned NOT NULL DEFAULT '0',
  `isResting` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `restState` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `restTime` int(10) unsigned NOT NULL DEFAULT '0',
  `playedtime` longtext NOT NULL,
  `deathstate` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `TalentResetTimes` int(10) unsigned NOT NULL DEFAULT '0',
  `first_login` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `forced_rename_pending` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `arenaPoints` int(10) unsigned NOT NULL DEFAULT '0',
  `totalstableslots` int(10) unsigned NOT NULL DEFAULT '0',
  `instance_id` int(10) unsigned NOT NULL DEFAULT '0',
  `entrypointmap` int(10) unsigned NOT NULL DEFAULT '0',
  `entrypointx` float NOT NULL DEFAULT '0',
  `entrypointy` float NOT NULL DEFAULT '0',
  `entrypointz` float NOT NULL DEFAULT '0',
  `entrypointo` float NOT NULL DEFAULT '0',
  `entrypointinstance` int(10) unsigned NOT NULL DEFAULT '0',
  `taxi_path` int(10) unsigned NOT NULL DEFAULT '0',
  `taxi_lastnode` int(10) unsigned NOT NULL DEFAULT '0',
  `taxi_mountid` int(10) unsigned NOT NULL DEFAULT '0',
  `transporter` int(10) unsigned NOT NULL DEFAULT '0',
  `transporter_xdiff` float NOT NULL DEFAULT '0',
  `transporter_ydiff` float NOT NULL DEFAULT '0',
  `transporter_zdiff` float NOT NULL DEFAULT '0',
  `spells` longtext NOT NULL,
  `deleted_spells` longtext NOT NULL,
  `reputation` longtext NOT NULL,
  `actions` longtext NOT NULL,
  `auras` longtext NOT NULL,
  `finished_quests` longtext NOT NULL,
  `honorPointsToAdd` int(11) NOT NULL,
  `killsToday` int(10) unsigned NOT NULL DEFAULT '0',
  `killsYesterday` int(10) unsigned NOT NULL DEFAULT '0',
  `killsLifeTime` int(10) unsigned NOT NULL DEFAULT '0',
  `honorToday` int(10) unsigned NOT NULL DEFAULT '0',
  `honorYesterday` int(10) unsigned NOT NULL DEFAULT '0',
  `honorPoints` int(10) unsigned NOT NULL DEFAULT '0',
  `difficulty` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`),
  UNIQUE KEY `name` (`name`),
  KEY `acct` (`acct`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `characters_insert_queue` DISABLE KEYS */;
/*!40000 ALTER TABLE `characters_insert_queue` ENABLE KEYS */;


DROP TABLE IF EXISTS `charters`;
CREATE TABLE `charters` (
  `charterId` int(30) NOT NULL,
  `charterType` int(30) NOT NULL DEFAULT '0',
  `leaderGuid` int(20) unsigned NOT NULL DEFAULT '0',
  `guildName` varchar(32) NOT NULL DEFAULT '',
  `itemGuid` bigint(40) unsigned NOT NULL DEFAULT '0',
  `signer1` int(10) unsigned NOT NULL DEFAULT '0',
  `signer2` int(10) unsigned NOT NULL DEFAULT '0',
  `signer3` int(10) unsigned NOT NULL DEFAULT '0',
  `signer4` int(10) unsigned NOT NULL DEFAULT '0',
  `signer5` int(10) unsigned NOT NULL DEFAULT '0',
  `signer6` int(10) unsigned NOT NULL DEFAULT '0',
  `signer7` int(10) unsigned NOT NULL DEFAULT '0',
  `signer8` int(10) unsigned NOT NULL DEFAULT '0',
  `signer9` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`charterId`),
  UNIQUE KEY `leaderGuid` (`leaderGuid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `charters` DISABLE KEYS */;
/*!40000 ALTER TABLE `charters` ENABLE KEYS */;


DROP TABLE IF EXISTS `corpses`;
CREATE TABLE `corpses` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `positionX` float NOT NULL DEFAULT '0',
  `positionY` float NOT NULL DEFAULT '0',
  `positionZ` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `zoneId` int(10) unsigned NOT NULL DEFAULT '38',
  `mapId` int(10) unsigned NOT NULL DEFAULT '0',
  `instanceId` int(10) unsigned NOT NULL DEFAULT '0',
  `data` longtext COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*!40000 ALTER TABLE `corpses` DISABLE KEYS */;
/*!40000 ALTER TABLE `corpses` ENABLE KEYS */;


DROP TABLE IF EXISTS `gm_groups`;
CREATE TABLE `gm_groups` (
  `rank` int(10) unsigned NOT NULL COMMENT 'Id du groupe',
  `flags` varchar(45) NOT NULL COMMENT 'Flags spécifique',
  `inherit` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Id du groupe dont celui-ci hérite',
  `comment` text,
  PRIMARY KEY (`rank`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `gm_groups` DISABLE KEYS */;
INSERT INTO `gm_groups` VALUES  (1,'rt',0,'Adviseur (téléportation de base)'),
 (2,'bcfghikmuCGPRT',1,'Game Master'),
 (3,'nopqsvwLMSW',2,'GM - Tech DB (lookups)'),
 (4,'aeBEIN',3,'Administrateur'),
 (5,'A',4,'Super-administrateur');
/*!40000 ALTER TABLE `gm_groups` ENABLE KEYS */;


DROP TABLE IF EXISTS `gm_overrides`;
CREATE TABLE `gm_overrides` (
  `command` varchar(100) NOT NULL,
  `access` char(1) NOT NULL,
  PRIMARY KEY (`command`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Commands System';

/*!40000 ALTER TABLE `gm_overrides` DISABLE KEYS */;
/*!40000 ALTER TABLE `gm_overrides` ENABLE KEYS */;


DROP TABLE IF EXISTS `gm_tickets`;
CREATE TABLE `gm_tickets` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `name` varchar(200) COLLATE utf8_unicode_ci NOT NULL,
  `level` int(10) unsigned NOT NULL DEFAULT '0',
  `type` int(10) unsigned NOT NULL DEFAULT '0',
  `posX` float NOT NULL DEFAULT '0',
  `posY` float NOT NULL DEFAULT '0',
  `posZ` float NOT NULL DEFAULT '0',
  `message` text COLLATE utf8_unicode_ci NOT NULL,
  `timestamp` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*!40000 ALTER TABLE `gm_tickets` DISABLE KEYS */;
/*!40000 ALTER TABLE `gm_tickets` ENABLE KEYS */;


DROP TABLE IF EXISTS `groups`;
CREATE TABLE `groups` (
  `group_id` int(30) NOT NULL,
  `group_type` tinyint(2) NOT NULL,
  `subgroup_count` tinyint(2) NOT NULL,
  `loot_method` tinyint(2) NOT NULL,
  `loot_threshold` tinyint(2) NOT NULL,
  `difficulty` int(30) NOT NULL DEFAULT '0',
  `assistant_leader` int(30) NOT NULL DEFAULT '0',
  `main_tank` int(30) NOT NULL DEFAULT '0',
  `main_assist` int(30) NOT NULL DEFAULT '0',
  `group1member1` int(50) NOT NULL,
  `group1member2` int(50) NOT NULL,
  `group1member3` int(50) NOT NULL,
  `group1member4` int(50) NOT NULL,
  `group1member5` int(50) NOT NULL,
  `group2member1` int(50) NOT NULL,
  `group2member2` int(50) NOT NULL,
  `group2member3` int(50) NOT NULL,
  `group2member4` int(50) NOT NULL,
  `group2member5` int(50) NOT NULL,
  `group3member1` int(50) NOT NULL,
  `group3member2` int(50) NOT NULL,
  `group3member3` int(50) NOT NULL,
  `group3member4` int(50) NOT NULL,
  `group3member5` int(50) NOT NULL,
  `group4member1` int(50) NOT NULL,
  `group4member2` int(50) NOT NULL,
  `group4member3` int(50) NOT NULL,
  `group4member4` int(50) NOT NULL,
  `group4member5` int(50) NOT NULL,
  `group5member1` int(50) NOT NULL,
  `group5member2` int(50) NOT NULL,
  `group5member3` int(50) NOT NULL,
  `group5member4` int(50) NOT NULL,
  `group5member5` int(50) NOT NULL,
  `group6member1` int(50) NOT NULL,
  `group6member2` int(50) NOT NULL,
  `group6member3` int(50) NOT NULL,
  `group6member4` int(50) NOT NULL,
  `group6member5` int(50) NOT NULL,
  `group7member1` int(50) NOT NULL,
  `group7member2` int(50) NOT NULL,
  `group7member3` int(50) NOT NULL,
  `group7member4` int(50) NOT NULL,
  `group7member5` int(50) NOT NULL,
  `group8member1` int(50) NOT NULL,
  `group8member2` int(50) NOT NULL,
  `group8member3` int(50) NOT NULL,
  `group8member4` int(50) NOT NULL,
  `group8member5` int(50) NOT NULL,
  `timestamp` int(30) NOT NULL,
  PRIMARY KEY (`group_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `groups` DISABLE KEYS */;
/*!40000 ALTER TABLE `groups` ENABLE KEYS */;


DROP TABLE IF EXISTS `guild_bankitems`;
CREATE TABLE `guild_bankitems` (
  `guildId` int(30) NOT NULL,
  `tabId` int(30) NOT NULL,
  `slotId` int(30) NOT NULL,
  `itemGuid` int(30) NOT NULL,
  PRIMARY KEY (`guildId`,`tabId`,`slotId`),
  KEY `a` (`guildId`),
  KEY `b` (`tabId`),
  KEY `c` (`slotId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `guild_bankitems` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_bankitems` ENABLE KEYS */;


DROP TABLE IF EXISTS `guild_banklogs`;
CREATE TABLE `guild_banklogs` (
  `log_id` int(30) NOT NULL,
  `guildid` int(30) NOT NULL,
  `tabid` int(30) NOT NULL COMMENT 'tab 6 is money logs',
  `action` int(5) NOT NULL,
  `player_guid` int(30) NOT NULL,
  `item_entry` int(30) NOT NULL,
  `stack_count` int(30) NOT NULL,
  `timestamp` int(30) NOT NULL,
  PRIMARY KEY (`log_id`,`guildid`),
  KEY `a` (`guildid`),
  KEY `b` (`tabid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `guild_banklogs` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_banklogs` ENABLE KEYS */;


DROP TABLE IF EXISTS `guild_banktabs`;
CREATE TABLE `guild_banktabs` (
  `guildId` int(30) NOT NULL,
  `tabId` int(30) NOT NULL,
  `tabName` varchar(200) NOT NULL,
  `tabIcon` varchar(200) NOT NULL,
  PRIMARY KEY (`guildId`,`tabId`),
  KEY `a` (`guildId`),
  KEY `b` (`tabId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `guild_banktabs` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_banktabs` ENABLE KEYS */;


DROP TABLE IF EXISTS `guild_checkpoints`;
CREATE TABLE `guild_checkpoints` (
  `guildid` int(30) NOT NULL,
  `checkid` int(30) NOT NULL,
  PRIMARY KEY (`guildid`,`checkid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `guild_checkpoints` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_checkpoints` ENABLE KEYS */;


DROP TABLE IF EXISTS `guild_data`;
CREATE TABLE `guild_data` (
  `guildid` int(30) NOT NULL,
  `playerid` int(30) NOT NULL,
  `guildRank` int(30) NOT NULL,
  `publicNote` varchar(300) NOT NULL,
  `officerNote` varchar(300) NOT NULL,
  `lastWithdrawReset` int(30) NOT NULL DEFAULT '0',
  `withdrawlsSinceLastReset` int(30) NOT NULL DEFAULT '0',
  `lastItemWithdrawReset0` int(30) NOT NULL DEFAULT '0',
  `itemWithdrawlsSinceLastReset0` int(30) NOT NULL DEFAULT '0',
  `lastItemWithdrawReset1` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset1` int(30) NOT NULL,
  `lastItemWithdrawReset2` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset2` int(30) NOT NULL,
  `lastItemWithdrawReset3` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset3` int(30) NOT NULL,
  `lastItemWithdrawReset4` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset4` int(30) NOT NULL,
  `lastItemWithdrawReset5` int(30) NOT NULL,
  `itemWithdrawlsSinceLastReset5` int(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `guild_data` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_data` ENABLE KEYS */;


DROP TABLE IF EXISTS `guild_logs`;
CREATE TABLE `guild_logs` (
  `log_id` int(30) NOT NULL,
  `guildid` int(30) NOT NULL,
  `timestamp` int(30) NOT NULL,
  `event_type` int(30) NOT NULL,
  `misc1` int(30) NOT NULL,
  `misc2` int(30) NOT NULL,
  `misc3` int(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `guild_logs` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_logs` ENABLE KEYS */;


DROP TABLE IF EXISTS `guild_ranks`;
CREATE TABLE `guild_ranks` (
  `guildId` int(6) unsigned NOT NULL DEFAULT '0',
  `rankId` int(1) NOT NULL DEFAULT '0',
  `rankName` varchar(255) NOT NULL DEFAULT '',
  `rankRights` int(3) unsigned NOT NULL DEFAULT '0',
  `goldLimitPerDay` int(30) NOT NULL DEFAULT '0',
  `bankTabFlags0` int(30) NOT NULL DEFAULT '0',
  `itemStacksPerDay0` int(30) NOT NULL DEFAULT '0',
  `bankTabFlags1` int(30) NOT NULL DEFAULT '0',
  `itemStacksPerDay1` int(30) NOT NULL DEFAULT '0',
  `bankTabFlags2` int(30) NOT NULL DEFAULT '0',
  `itemStacksPerDay2` int(30) NOT NULL DEFAULT '0',
  `bankTabFlags3` int(30) NOT NULL DEFAULT '0',
  `itemStacksPerDay3` int(30) NOT NULL DEFAULT '0',
  `bankTabFlags4` int(30) NOT NULL DEFAULT '0',
  `itemStacksPerDay4` int(30) NOT NULL DEFAULT '0',
  `bankTabFlags5` int(30) NOT NULL DEFAULT '0',
  `itemStacksPerDay5` int(30) NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildId`,`rankId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `guild_ranks` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_ranks` ENABLE KEYS */;


DROP TABLE IF EXISTS `guilds`;
CREATE TABLE `guilds` (
  `guildId` bigint(20) NOT NULL AUTO_INCREMENT,
  `guildName` varchar(32) NOT NULL DEFAULT '',
  `leaderGuid` bigint(20) NOT NULL DEFAULT '0',
  `emblemStyle` int(10) NOT NULL DEFAULT '0',
  `emblemColor` int(10) NOT NULL DEFAULT '0',
  `borderStyle` int(10) NOT NULL DEFAULT '0',
  `borderColor` int(10) NOT NULL DEFAULT '0',
  `backgroundColor` int(10) NOT NULL DEFAULT '0',
  `guildInfo` varchar(100) NOT NULL DEFAULT '',
  `motd` varchar(200) NOT NULL,
  `createdate` int(30) NOT NULL DEFAULT '0',
  `bankTabCount` int(30) NOT NULL DEFAULT '0',
  `bankBalance` int(30) NOT NULL,
  PRIMARY KEY (`guildId`),
  UNIQUE KEY `guildId` (`guildId`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `guilds` DISABLE KEYS */;
/*!40000 ALTER TABLE `guilds` ENABLE KEYS */;


DROP TABLE IF EXISTS `instances`;
CREATE TABLE `instances` (
  `id` int(30) NOT NULL,
  `mapid` int(30) NOT NULL,
  `creation` int(30) NOT NULL,
  `expiration` int(30) NOT NULL,
  `killed_npc_guids` text NOT NULL,
  `difficulty` int(30) NOT NULL,
  `creator_group` int(30) NOT NULL,
  `creator_guid` int(30) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `instances` DISABLE KEYS */;
/*!40000 ALTER TABLE `instances` ENABLE KEYS */;


DROP TABLE IF EXISTS `ipbans`;
CREATE TABLE `ipbans` (
  `ip` varchar(15) COLLATE utf8_unicode_ci NOT NULL,
  `expire` int(16) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ip`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*!40000 ALTER TABLE `ipbans` DISABLE KEYS */;
/*!40000 ALTER TABLE `ipbans` ENABLE KEYS */;


DROP TABLE IF EXISTS `mailbox`;
CREATE TABLE `mailbox` (
  `message_id` int(10) unsigned NOT NULL DEFAULT '0',
  `message_type` int(10) unsigned NOT NULL DEFAULT '0',
  `player_guid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `sender_guid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `subject` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `body` longtext COLLATE utf8_unicode_ci NOT NULL,
  `money` int(10) unsigned NOT NULL DEFAULT '0',
  `attached_item_guids` varchar(200) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `cod` int(10) unsigned NOT NULL DEFAULT '0',
  `stationary` int(10) unsigned NOT NULL DEFAULT '0',
  `expiry_time` int(10) unsigned NOT NULL DEFAULT '0',
  `delivery_time` int(10) unsigned NOT NULL DEFAULT '0',
  `copy_made` int(10) unsigned NOT NULL DEFAULT '0',
  `read_flag` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `deleted_flag` tinyint(3) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*!40000 ALTER TABLE `mailbox` DISABLE KEYS */;
/*!40000 ALTER TABLE `mailbox` ENABLE KEYS */;


DROP TABLE IF EXISTS `mailbox_insert_queue`;
CREATE TABLE `mailbox_insert_queue` (
  `sender_guid` int(30) NOT NULL,
  `receiver_guid` int(30) NOT NULL,
  `subject` varchar(200) NOT NULL,
  `body` varchar(500) NOT NULL,
  `stationary` int(30) NOT NULL,
  `money` int(30) NOT NULL,
  `item_id` int(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `mailbox_insert_queue` DISABLE KEYS */;
/*!40000 ALTER TABLE `mailbox_insert_queue` ENABLE KEYS */;


DROP TABLE IF EXISTS `playercooldowns`;
CREATE TABLE `playercooldowns` (
  `player_guid` int(30) NOT NULL,
  `cooldown_type` int(30) NOT NULL COMMENT '0 is spell, 1 is item, 2 is spell category',
  `cooldown_misc` int(30) NOT NULL COMMENT 'spellid/itemid/category',
  `cooldown_expire_time` int(30) NOT NULL COMMENT 'expiring time in unix epoch format',
  `cooldown_spellid` int(30) NOT NULL COMMENT 'spell that cast it',
  `cooldown_itemid` int(30) NOT NULL COMMENT 'item that cast it'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*!40000 ALTER TABLE `playercooldowns` DISABLE KEYS */;
/*!40000 ALTER TABLE `playercooldowns` ENABLE KEYS */;


DROP TABLE IF EXISTS `playerglyphs`;
CREATE TABLE `playerglyphs` (
  `guid` int(10) unsigned NOT NULL,
  `spec` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `glyph1` smallint(5) unsigned DEFAULT NULL,
  `glyph2` smallint(5) unsigned DEFAULT NULL,
  `glyph3` smallint(5) unsigned DEFAULT NULL,
  `glyph4` smallint(5) unsigned DEFAULT NULL,
  `glyph5` smallint(5) unsigned DEFAULT NULL,
  `glyph6` smallint(5) unsigned DEFAULT NULL,
  PRIMARY KEY (`guid`,`spec`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 CHECKSUM=1 DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC;

/*!40000 ALTER TABLE `playerglyphs` DISABLE KEYS */;
/*!40000 ALTER TABLE `playerglyphs` ENABLE KEYS */;


DROP TABLE IF EXISTS `playeritems`;
CREATE TABLE `playeritems` (
  `ownerguid` int(10) unsigned NOT NULL DEFAULT '0',
  `guid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `wrapped_item_id` int(30) NOT NULL DEFAULT '0',
  `wrapped_creator` int(30) NOT NULL DEFAULT '0',
  `creator` int(10) unsigned NOT NULL DEFAULT '0',
  `count` int(10) unsigned NOT NULL DEFAULT '0',
  `charges` int(10) unsigned NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `randomprop` int(10) unsigned NOT NULL DEFAULT '0',
  `randomsuffix` int(30) NOT NULL,
  `itemtext` int(10) unsigned NOT NULL DEFAULT '0',
  `durability` int(10) unsigned NOT NULL DEFAULT '0',
  `containerslot` int(11) DEFAULT '-1' COMMENT 'couldnt find this being used in source',
  `slot` tinyint(4) NOT NULL DEFAULT '0',
  `enchantments` longtext COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`guid`),
  UNIQUE KEY `guid` (`guid`),
  KEY `ownerguid` (`ownerguid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*!40000 ALTER TABLE `playeritems` DISABLE KEYS */;
/*!40000 ALTER TABLE `playeritems` ENABLE KEYS */;


DROP TABLE IF EXISTS `playeritems_insert_queue`;
CREATE TABLE `playeritems_insert_queue` (
  `ownerguid` int(10) unsigned NOT NULL DEFAULT '0',
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `wrapped_item_id` int(30) NOT NULL DEFAULT '0',
  `wrapped_creator` int(30) NOT NULL DEFAULT '0',
  `creator` int(10) unsigned NOT NULL DEFAULT '0',
  `count` int(10) unsigned NOT NULL DEFAULT '0',
  `charges` int(10) unsigned NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `randomprop` int(10) unsigned NOT NULL DEFAULT '0',
  `randomsuffix` int(30) NOT NULL,
  `itemtext` int(10) unsigned NOT NULL DEFAULT '0',
  `durability` int(10) unsigned NOT NULL DEFAULT '0',
  `containerslot` int(11) NOT NULL DEFAULT '-1' COMMENT 'couldnt find this being used in source',
  `slot` tinyint(4) NOT NULL DEFAULT '0',
  `enchantments` longtext COLLATE utf8_unicode_ci NOT NULL,
  KEY `ownerguid` (`ownerguid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*!40000 ALTER TABLE `playeritems_insert_queue` DISABLE KEYS */;
/*!40000 ALTER TABLE `playeritems_insert_queue` ENABLE KEYS */;


DROP TABLE IF EXISTS `playerpetactionbar`;
CREATE TABLE `playerpetactionbar` (
  `ownerguid` bigint(20) NOT NULL DEFAULT '0',
  `petnumber` int(11) NOT NULL DEFAULT '0',
  `spellid_1` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_2` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_3` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_4` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_5` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_6` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_7` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_8` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_9` int(11) unsigned NOT NULL DEFAULT '0',
  `spellid_10` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_1` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_2` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_3` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_4` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_5` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_6` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_7` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_8` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_9` int(11) unsigned NOT NULL DEFAULT '0',
  `spellstate_10` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ownerguid`,`petnumber`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `playerpetactionbar` DISABLE KEYS */;
/*!40000 ALTER TABLE `playerpetactionbar` ENABLE KEYS */;


DROP TABLE IF EXISTS `playerpets`;
CREATE TABLE `playerpets` (
  `ownerguid` bigint(20) NOT NULL DEFAULT '0',
  `petnumber` int(11) NOT NULL DEFAULT '0',
  `name` varchar(21) NOT NULL DEFAULT '',
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `fields` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `xp` int(11) NOT NULL DEFAULT '0',
  `active` tinyint(1) NOT NULL DEFAULT '0',
  `level` int(11) NOT NULL DEFAULT '0',
  `happiness` int(11) NOT NULL DEFAULT '0',
  `happinessupdate` int(11) NOT NULL DEFAULT '0',
  `summon` int(11) NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `playerpets` DISABLE KEYS */;
/*!40000 ALTER TABLE `playerpets` ENABLE KEYS */;


DROP TABLE IF EXISTS `playerpetspells`;
CREATE TABLE `playerpetspells` (
  `ownerguid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `petnumber` int(10) unsigned NOT NULL DEFAULT '0',
  `spellid` int(10) unsigned NOT NULL DEFAULT '0',
  `flags` smallint(5) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*!40000 ALTER TABLE `playerpetspells` DISABLE KEYS */;
/*!40000 ALTER TABLE `playerpetspells` ENABLE KEYS */;


DROP TABLE IF EXISTS `playerpettalents`;
CREATE TABLE `playerpettalents` (
  `ownerguid` bigint(20) NOT NULL DEFAULT '0',
  `petnumber` int(4) NOT NULL DEFAULT '0',
  `talentid` int(11) NOT NULL DEFAULT '0',
  `rank` tinyint(4) NOT NULL DEFAULT '0',
  KEY `a` (`ownerguid`),
  KEY `b` (`petnumber`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `playerpettalents` DISABLE KEYS */;
/*!40000 ALTER TABLE `playerpettalents` ENABLE KEYS */;


DROP TABLE IF EXISTS `playerskills`;
CREATE TABLE `playerskills` (
  `player_guid` int(11) NOT NULL DEFAULT '0',
  `skill_id` int(11) NOT NULL DEFAULT '0',
  `type` int(11) NOT NULL,
  `currentlvl` int(11) NOT NULL DEFAULT '1',
  `maxlvl` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`player_guid`,`skill_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `playerskills` DISABLE KEYS */;
/*!40000 ALTER TABLE `playerskills` ENABLE KEYS */;


DROP TABLE IF EXISTS `playerspells`;
CREATE TABLE `playerspells` (
  `guid` int(10) unsigned NOT NULL,
  `spellid` int(10) unsigned NOT NULL,
  PRIMARY KEY (`guid`,`spellid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `playerspells` DISABLE KEYS */;
/*!40000 ALTER TABLE `playerspells` ENABLE KEYS */;


DROP TABLE IF EXISTS `playersummonspells`;
CREATE TABLE `playersummonspells` (
  `ownerguid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `entryid` int(10) unsigned NOT NULL DEFAULT '0',
  `spellid` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*!40000 ALTER TABLE `playersummonspells` DISABLE KEYS */;
/*!40000 ALTER TABLE `playersummonspells` ENABLE KEYS */;


DROP TABLE IF EXISTS `playertalents`;
CREATE TABLE `playertalents` (
  `guid` int(10) unsigned NOT NULL,
  `spec` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `tid` smallint(5) unsigned NOT NULL,
  `rank` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`guid`,`spec`,`tid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 CHECKSUM=1 DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC;

/*!40000 ALTER TABLE `playertalents` DISABLE KEYS */;
/*!40000 ALTER TABLE `playertalents` ENABLE KEYS */;


DROP TABLE IF EXISTS `questlog`;
CREATE TABLE `questlog` (
  `player_guid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `quest_id` int(10) unsigned NOT NULL DEFAULT '0',
  `slot` int(10) unsigned NOT NULL DEFAULT '0',
  `time_left` int(10) unsigned NOT NULL DEFAULT '0',
  `explored_area1` int(10) unsigned NOT NULL DEFAULT '0',
  `explored_area2` int(10) unsigned NOT NULL DEFAULT '0',
  `explored_area3` int(10) unsigned NOT NULL DEFAULT '0',
  `explored_area4` int(10) unsigned NOT NULL DEFAULT '0',
  `mob_kill1` int(10) unsigned NOT NULL DEFAULT '0',
  `mob_kill2` int(10) unsigned NOT NULL DEFAULT '0',
  `mob_kill3` int(10) unsigned NOT NULL DEFAULT '0',
  `mob_kill4` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`player_guid`,`quest_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*!40000 ALTER TABLE `questlog` DISABLE KEYS */;
/*!40000 ALTER TABLE `questlog` ENABLE KEYS */;


DROP TABLE IF EXISTS `server_settings`;
CREATE TABLE `server_settings` (
  `setting_id` varchar(200) NOT NULL,
  `setting_value` int(50) NOT NULL,
  PRIMARY KEY (`setting_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `server_settings` DISABLE KEYS */;
/*!40000 ALTER TABLE `server_settings` ENABLE KEYS */;


DROP TABLE IF EXISTS `serveurstats`;
CREATE TABLE `serveurstats` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tagStat` int(11) NOT NULL,
  `uptime` text NOT NULL,
  `player` smallint(6) NOT NULL,
  `gm` smallint(6) NOT NULL,
  `alliance` smallint(6) NOT NULL,
  `horde` smallint(6) NOT NULL,
  `accept` smallint(6) NOT NULL,
  `peak` smallint(6) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=369 DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `serveurstats` DISABLE KEYS */;
/*!40000 ALTER TABLE `serveurstats` ENABLE KEYS */;

DROP TABLE IF EXISTS `social_friends`;
CREATE TABLE `social_friends` (
  `character_guid` int(30) NOT NULL,
  `friend_guid` int(30) NOT NULL,
  `note` varchar(100) NOT NULL,
  PRIMARY KEY (`character_guid`,`friend_guid`),
  KEY `a` (`character_guid`),
  KEY `b` (`friend_guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*!40000 ALTER TABLE `social_friends` DISABLE KEYS */;
/*!40000 ALTER TABLE `social_friends` ENABLE KEYS */;


DROP TABLE IF EXISTS `social_ignores`;
CREATE TABLE `social_ignores` (
  `character_guid` int(30) NOT NULL,
  `ignore_guid` int(30) NOT NULL,
  PRIMARY KEY (`character_guid`,`ignore_guid`),
  KEY `a` (`character_guid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*!40000 ALTER TABLE `social_ignores` DISABLE KEYS */;
/*!40000 ALTER TABLE `social_ignores` ENABLE KEYS */;


DROP TABLE IF EXISTS `tutorials`;
CREATE TABLE `tutorials` (
  `playerId` bigint(20) unsigned NOT NULL DEFAULT '0',
  `tut0` int(10) unsigned NOT NULL DEFAULT '0',
  `tut1` int(10) unsigned NOT NULL DEFAULT '0',
  `tut2` int(10) unsigned NOT NULL DEFAULT '0',
  `tut3` int(10) unsigned NOT NULL DEFAULT '0',
  `tut4` int(10) unsigned NOT NULL DEFAULT '0',
  `tut5` int(10) unsigned NOT NULL DEFAULT '0',
  `tut6` int(10) unsigned NOT NULL DEFAULT '0',
  `tut7` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`playerId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;


/*!40000 ALTER TABLE `tutorials` DISABLE KEYS */;
/*!40000 ALTER TABLE `tutorials` ENABLE KEYS */;


/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
