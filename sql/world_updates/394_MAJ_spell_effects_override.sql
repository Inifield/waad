ALTER TABLE spell_effects_override
 ADD interruptFlags INT(30) NOT NULL DEFAULT '0' AFTER spellId;