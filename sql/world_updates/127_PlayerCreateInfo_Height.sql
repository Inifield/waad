DROP TABLE IF EXISTS `playercreateinfo_height`;
CREATE TABLE `playercreateinfo_height` 
(
   `raceid` int(10) unsigned NOT NULL,
   `name` varchar(100) NOT NULL,
   `height` float NOT NULL DEFAULT '1',
   PRIMARY KEY (`raceid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Hauteur des differentes Races';

insert into `playercreateinfo_height` values
(1,"Human",1.75),
(2,"Orc",1.8),
(3,"Dwarf",0.9),
(4,"NightElf",2.1),
(5,"Mort Vivant",1.8),
(6,"Tauren",1.9),
(7,"Gnome",0.8),
(8,"Troll",1.7),
(9,"Goblin",1.5),
(10,"BloodElf",2.1),
(11,"Draenei",1.8),
(12,"Gangr'Orc",1.8),
(13,"Naga",2.3),
(14,"Roué",1.8),
(15,"Skeleton",1.8),
(16,"Vrykul",1.7),
(17,"Tuskarr/Rohart",1.5),
(18,"ForestTroll",1.6),
(19,"Taunka",1.5),
(20,"NorthrendSkeleton",1.5),
(21,"IceTroll",1.6);