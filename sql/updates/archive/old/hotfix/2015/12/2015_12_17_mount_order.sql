ALTER TABLE `mount` CHANGE `SpellID` `SpellID` INT(10) UNSIGNED NOT NULL AFTER `ID`, CHANGE `Category` `Category` INT(10) UNSIGNED NOT NULL AFTER `SpellID`, CHANGE `PlayerConditionID` `PlayerConditionID` INT(10) UNSIGNED NOT NULL AFTER `FilterCategory`;