ALTER TABLE `creature_proto`
 ADD COLUMN `vehicle_entry` INT(11) NOT NULL DEFAULT '-1' AFTER `extra_a9_flags`;