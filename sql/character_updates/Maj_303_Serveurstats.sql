DROP TABLE IF EXISTS `serveurstats`;

 CREATE TABLE `serveurstats` (
`id` INT NOT NULL AUTO_INCREMENT ,
`tagStat` INT NOT NULL ,
`uptime` TEXT NOT NULL ,
`player` SMALLINT NOT NULL ,
`gm` SMALLINT NOT NULL ,
`alliance` SMALLINT NOT NULL ,
`horde` SMALLINT NOT NULL ,
`accept` SMALLINT NOT NULL ,
`peak` SMALLINT NOT NULL ,
PRIMARY KEY ( `id` )
) ENGINE = MYISAM ;