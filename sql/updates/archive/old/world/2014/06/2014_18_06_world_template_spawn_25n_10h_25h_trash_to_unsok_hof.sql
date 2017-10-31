-- Trash - Un'sok - Heart of Fear - Alternatives templates for 25N, 10H, 25H modes

-- 63568 - Amber-ridden Mushan
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('80568','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43456','0','0','0','Amber-Ridden Mushan (4)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','4','2000','2000','1','32768','2099200','0','0','0','0','0','0','0','0','0','1','2097224','0','63568','0','63568','0','0','0','0','0','0','125069','125081','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','60','1','1','1','0','0','0','0','0','0','0','161','1','0','0','0','mob_amberridden_mushan','16048'),
('81568','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43456','0','0','0','Amber-Ridden Mushan (5)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','6','2000','2000','1','32768','2099200','0','0','0','0','0','0','0','0','0','1','2097224','0','63568','0','63568','0','0','0','0','0','0','125069','125081','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','20','1','1','1','0','0','0','0','0','0','0','161','1','0','0','0','mob_amberridden_mushan','16048'),
('82568','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43456','0','0','0','Amber-Ridden Mushan (6)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','9','2000','2000','1','32768','2099200','0','0','0','0','0','0','0','0','0','1','2097224','0','63568','0','63568','0','0','0','0','0','0','125069','125081','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','60','1','1','1','0','0','0','0','0','0','0','161','1','0','0','0','mob_amberridden_mushan','16048');

UPDATE creature_template SET difficulty_entry_4 = 80568, difficulty_entry_5 = 81568, difficulty_entry_6 = 82568 WHERE entry = 63568;
UPDATE creature SET spawnMask = 760 WHERE id = 63568;

-- 63569 - Amber Searsting
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('80569','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43143','0','0','0','Amber Searsting (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','4','2000','2000','1','32768','2099200','0','0','0','0','0','0','0','0','0','1','2097224','0','63569','0','63569','0','0','0','0','0','0','125490','125480','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','120','1','1','1','0','0','0','0','0','0','0','701','1','0','0','0','mob_amber_searsting','16048'),
('81569','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43143','0','0','0','Amber Searsting (5)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','6','2000','2000','1','32768','2099200','0','0','0','0','0','0','0','0','0','1','2097224','0','63569','0','63569','0','0','0','0','0','0','125490','125480','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','0','1','1','1','0','0','0','0','0','0','0','701','1','0','0','0','mob_amber_searsting','16048'),
('82569','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43143','0','0','0','Amber Searsting (6)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','9','2000','2000','1','32768','2099200','0','0','0','0','0','0','0','0','0','1','2097224','0','63569','0','63569','0','0','0','0','0','0','125490','125480','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','120','1','1','1','0','0','0','0','0','0','0','701','1','0','0','0','mob_amber_searsting','16048');

UPDATE creature_template SET difficulty_entry_4 = 80569, difficulty_entry_5 = 81569, difficulty_entry_6 = 82569 WHERE entry = 63569;
UPDATE creature SET spawnMask = 760 WHERE id = 63569;

-- 63570 - Sra'thik Pool-tender
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('80570','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','42640','0','0','0','Sra\'thik Pool-Tender (4)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','4','2000','2000','1','32768','2099200','0','0','0','0','0','0','0','0','0','7','2097224','0','63570','0','0','0','0','0','0','0','0','125426','125432','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','30','10','1','1','0','0','0','0','0','0','0','0','1','0','0','0','mob_srathik_pooltender','16048'),
('81570','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','42640','0','0','0','Sra\'thik Pool-Tender (5)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','6','2000','2000','1','32768','2099200','0','0','0','0','0','0','0','0','0','7','2097224','0','63570','0','0','0','0','0','0','0','0','125426','125432','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','10','10','1','1','0','0','0','0','0','0','0','0','1','0','0','0','mob_srathik_pooltender','16048'),
('82570','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','42640','0','0','0','Sra\'thik Pool-Tender (6)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','9','2000','2000','1','32768','2099200','0','0','0','0','0','0','0','0','0','7','2097224','0','63570','0','0','0','0','0','0','0','0','125426','125432','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','30','10','1','1','0','0','0','0','0','0','0','0','1','0','0','0','mob_srathik_pooltender','16048');

UPDATE creature_template SET difficulty_entry_4 = 80570, difficulty_entry_5 = 81570, difficulty_entry_6 = 82570 WHERE entry = 63570;
UPDATE creature SET spawnMask = 760 WHERE id = 63570;

-- 63594/63597 - Coagulated Ambers
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('80594','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','33921','0','0','0','Coagulated Amber (4)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','4','1000','1000','1','32768','2099200','0','0','0','0','0','0','0','0','0','10','2097224','0','0','0','0','0','0','0','0','0','0','125054','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','9','1','1','1','0','0','0','0','0','0','0','152','1','0','0','0','mob_coagulated_amber','16048'),
('81594','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','33921','0','0','0','Coagulated Amber (5)','','','0','90','90','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','6','1000','1000','1','32768','2099200','0','0','0','0','0','0','0','0','0','10','2097224','0','0','0','0','0','0','0','0','0','0','125054','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','3','1','1','1','0','0','0','0','0','0','0','152','1','0','0','0','mob_coagulated_amber','16048'),
('82594','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','33921','0','0','0','Coagulated Amber (6)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','9','1000','1000','1','32768','2099200','0','0','0','0','0','0','0','0','0','10','2097224','0','0','0','0','0','0','0','0','0','0','125054','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','10','1','1','1','0','0','0','0','0','0','0','152','1','0','0','0','mob_coagulated_amber','16048');

UPDATE creature_template SET difficulty_entry_4 = 80594, difficulty_entry_5 = 81594, difficulty_entry_6 = 82594 WHERE entry = 63594;
UPDATE creature SET spawnMask = 760 WHERE id = 63594;

INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('80597','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','33921','0','0','0','Coagulated Amber (4)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','4','1000','1000','1','32768','2099200','0','0','0','0','0','0','0','0','0','10','2097224','0','0','0','0','0','0','0','0','0','0','125054','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','10','1','1','1','0','0','0','0','0','0','0','152','1','0','0','0','mob_coagulated_amber','16048'),
('81597','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','33921','0','0','0','Coagulated Amber (5)','','','0','90','90','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','6','1000','1000','1','32768','2099200','0','0','0','0','0','0','0','0','0','10','2097224','0','0','0','0','0','0','0','0','0','0','125054','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','3','1','1','1','0','0','0','0','0','0','0','152','1','0','0','0','mob_coagulated_amber','16048'),
('82597','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','33921','0','0','0','Coagulated Amber (6)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','9','1000','1000','1','32768','2099200','0','0','0','0','0','0','0','0','0','10','2097224','0','0','0','0','0','0','0','0','0','0','125054','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','10','1','1','1','0','0','0','0','0','0','0','152','1','0','0','0','mob_coagulated_amber','16048');

UPDATE creature_template SET difficulty_entry_4 = 80597, difficulty_entry_5 = 81597, difficulty_entry_6 = 82597 WHERE entry = 63597;
UPDATE creature SET spawnMask = 760 WHERE id = 63597;

-- 64902 - Kor'thik Fleshrender
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('80902','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43674','0','0','0','Kor\'thik Fleshrender (4)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','4','2000','2000','1','2099200','0','0','0','0','0','0','0','0','0','0','7','2097256','0','64902','0','0','0','0','0','0','0','0','126912','126907','126901','0','0','0','0','0','0','0','11000','12000','','0','3','1','75','1','1','1','0','0','0','0','0','0','0','152','1','0','0','0','mob_korthik_fleshrender','16048'),
('81902','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43674','0','0','0','Kor\'thik Fleshrender (5)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','6','2000','2000','1','2099200','0','0','0','0','0','0','0','0','0','0','7','2097256','0','64902','0','0','0','0','0','0','0','0','126912','126907','126901','0','0','0','0','0','0','0','11000','12000','','0','3','1','25','1','1','1','0','0','0','0','0','0','0','152','1','0','0','0','mob_korthik_fleshrender','16048'),
('82902','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43674','0','0','0','Kor\'thik Fleshrender (6)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','9','2000','2000','1','2099200','0','0','0','0','0','0','0','0','0','0','7','2097256','0','64902','0','0','0','0','0','0','0','0','126912','126907','126901','0','0','0','0','0','0','0','11000','12000','','0','3','1','75','1','1','1','0','0','0','0','0','0','0','152','1','0','0','0','mob_korthik_fleshrender','16048');

UPDATE creature_template SET difficulty_entry_4 = 80902, difficulty_entry_5 = 81902, difficulty_entry_6 = 82902 WHERE entry = 64902;
UPDATE creature SET spawnMask = 760 WHERE id = 64902;

-- 64916 - Kor'thik Swarmguard
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('80916','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43675','0','0','0','Kor\'thik Swarmguard (4)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','4','2000','2000','1','32768','2099200','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','126928','126933','126926','0','0','0','0','0','0','0','11000','12000','','0','3','1','40.5','1','1','1','0','0','0','0','0','0','0','144','1','0','0','0','mob_korthik_swarmguard','16048'),
('81916','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43675','0','0','0','Kor\'thik Swarmguard (5)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','6','2000','2000','1','32768','2099200','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','126928','126933','126926','0','0','0','0','0','0','0','11000','12000','','0','3','1','15','1','1','1','0','0','0','0','0','0','0','144','1','0','0','0','mob_korthik_swarmguard','16048'),
('82916','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43675','0','0','0','Kor\'thik Swarmguard (6)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','9','2000','2000','1','32768','2099200','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','126928','126933','126926','0','0','0','0','0','0','0','11000','12000','','0','3','1','45','1','1','1','0','0','0','0','0','0','0','144','1','0','0','0','mob_korthik_swarmguard','16048');

UPDATE creature_template SET difficulty_entry_4 = 80916, difficulty_entry_5 = 81916, difficulty_entry_6 = 82916 WHERE entry = 64916;
UPDATE creature SET spawnMask = 760 WHERE id = 64916;

-- 64917 - Sra'thik Ambercaller
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('80917','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43676','0','0','0','Sra\'thik Ambercaller (4)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','4','2000','2000','1','2099200','0','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','126937','0','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','30','15','1','1','0','0','0','0','0','0','0','144','1','0','0','0','mob_srathik_ambercaller','16048'),
('81917','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43676','0','0','0','Sra\'thik Ambercaller (5)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','6','2000','2000','1','2099200','0','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','126937','0','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','10','15','1','1','0','0','0','0','0','0','0','144','1','0','0','0','mob_srathik_ambercaller','16048'),
('82917','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43676','0','0','0','Sra\'thik Ambercaller (6)','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','4750','11118','0','55538','9','2000','2000','1','2099200','0','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','126937','0','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','30','15','1','1','0','0','0','0','0','0','0','144','1','0','0','0','mob_srathik_ambercaller','16048');

UPDATE creature_template SET difficulty_entry_4 = 80917, difficulty_entry_5 = 81917, difficulty_entry_6 = 82917 WHERE entry = 64917;
UPDATE creature SET spawnMask = 760 WHERE id = 64917;
