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



--
-- Definition of table `playerpetactionbar`
--

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

--
-- Dumping data for table `playerpetactionbar`
--

/*!40000 ALTER TABLE `playerpetactionbar` DISABLE KEYS */;
SET AUTOCOMMIT=0;
COMMIT;
/*!40000 ALTER TABLE `playerpetactionbar` ENABLE KEYS */;




/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
