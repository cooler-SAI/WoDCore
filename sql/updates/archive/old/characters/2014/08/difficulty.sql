ALTER TABLE `groups`     ADD COLUMN `legacyraiddifficulty` TINYINT(3) UNSIGNED DEFAULT '0' NOT NULL AFTER `raiddifficulty`,    CHANGE `raiddifficulty` `raiddifficulty` TINYINT(3) UNSIGNED DEFAULT '0' NOT NULL;
ALTER TABLE `characters`     ADD COLUMN `RaidDifficulty` TINYINT(3) UNSIGNED DEFAULT '0' NOT NULL AFTER `deleteDate`,     ADD COLUMN `LegacyRaidDifficuly` TINYINT(3) UNSIGNED DEFAULT '0' NOT NULL AFTER `RaidDifficulty`,    CHANGE `instance_mode_mask` `DungeonDifficulty` TINYINT(3) UNSIGNED DEFAULT '0' NOT NULL;