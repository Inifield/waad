ALTER TABLE quests
 ADD RequiredPlayer INT(10) NOT NULL DEFAULT '0' AFTER RequiredRepValue,
 CHANGE RequiredPlayer RequiredKillPlayer INT(10) NOT NULL DEFAULT '0' AFTER ReqKillMobOrGOCount4,
 ADD RewTitle INT(11) NOT NULL DEFAULT '0' AFTER RewRepValue5;

UPDATE quests SET RequiredPlayer=RequiredKillPlayer ;
UPDATE quests SET RequiredKillPlayer=0 ;