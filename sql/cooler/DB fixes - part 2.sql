UPDATE `creature_template` SET `KillCredit1` = 39098, `KillCredit2` = 0 WHERE `entry` = 39097;

UPDATE `creature_template` SET `unit_flags` = 536871814, `dynamicflags` = 32, `type_flags` = 128 WHERE `entry` = 44592;
UPDATE `creature` SET `spawndist` = 0, `MovementType` = 0 WHERE `id` IN (44592);
DELETE FROM `creature_template_addon` WHERE (`entry`=44592);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (44592, 0, 0, 7, 1, 65, '');
UPDATE `creature_template` SET `unit_flags2` = 1 WHERE `entry` = 44592;

UPDATE `creature_template` SET `unit_flags` = 536871814, `dynamicflags` = 32, `type_flags` = 128 WHERE `entry` = 44593;
UPDATE `creature` SET `spawndist` = 0, `MovementType` = 0 WHERE `id` IN (44593);
DELETE FROM `creature_template_addon` WHERE (`entry`=44593);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (44593, 0, 0, 7, 1, 65, '');
UPDATE `creature_template` SET `unit_flags2` = 1 WHERE `entry` = 44593;

UPDATE `creature_template` SET `gossip_menu_id` = 11892 WHERE `entry` = 44825;



UPDATE `creature_template` SET `gossip_menu_id` = 45197, `npcflag` = 1 WHERE `entry` = 45197;
REPLACE INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES('45197','0','0','Take Forsaken Insignia from dead body...','1','1','0','0','0','0','');


SET @ENTRY := 45197;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,62,0,100,0,45197,0,0,0,56,60862,1,0,0,0,0,0,0,0,0,0,0,0,0,"Orgus - On Gossip - Add Item"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,41,1000,0,0,0,0,0,1,0,0,0,0,0,0,0,"on Gossip- Despawn");

UPDATE `quest_template` SET `NextQuestId` = 27181 WHERE `Id` = 27180;


UPDATE `creature_template` SET `flags_extra`=0 WHERE `entry`=45312; 
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=45312; 
UPDATE `creature` SET `phaseMask`=0 WHERE `id`=45312; 
UPDATE `creature_addon` SET `auras`='' WHERE `guid` IN (SELECT `guid` FROM `creature` WHERE `id`=45312);

UPDATE `creature_template` SET `faction` = 2316 WHERE `entry` = 45403;
DELETE FROM `creature_template_addon` WHERE (`entry`=45403);
UPDATE `creature_template` SET `unit_flags` = 32768, `dynamicflags` = 0 WHERE `entry` = 45403;




UPDATE `creature_template` SET `gossip_menu_id` = 44369, `npcflag` = 1 WHERE `entry` = 44369;
REPLACE INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES('44369','0','0','Take dead body...','1','1','0','0','0','0','');


SET @ENTRY := 44369;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,62,0,100,0,44369,0,0,0,33,44369,0,0,0,0,0,7,0,0,0,0,0,0,0,"Orgus - On Gossip - Kill Credit"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"on Gossip- Despawn");

UPDATE `creature_template` SET `KillCredit1` = 45769 WHERE `entry` = 2120;



UPDATE `creature_template` SET `unit_flags` = 33536 WHERE `entry` = 47554;
UPDATE `creature_template` SET `unit_flags` = 33536 WHERE `entry` = 47551;

DELETE FROM `creature_template_addon` WHERE (`entry`=47697);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (47697, 0, 0, 1, 4097, 0, '');

SET @ENTRY := 47697;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,8,0,100,0,88797,0,0,0,33,47697,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,1,2,61,0,100,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"on Spell Hit - Despawn");

UPDATE `creature_template` SET `unit_flags` = 536871814, `dynamicflags` = 32, `type_flags` = 128 WHERE `entry` = 47870;
UPDATE `creature` SET `spawndist` = 0, `MovementType` = 0 WHERE `id` IN (47870);
DELETE FROM `creature_template_addon` WHERE (`entry`=47870);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (47870, 0, 0, 0, 1, 0, 29266);

UPDATE `creature_template` SET `unit_flags2` = 1 WHERE `entry` = 47870;

DELETE FROM `creature_template_addon` WHERE (`entry`=47790);
UPDATE `creature_template` SET `unit_flags` = 32768 WHERE `entry` = 47790;


UPDATE `creature_template` SET `faction` = 189 WHERE `entry` = 48080;

UPDATE `creature_template` SET `KillCredit1` = 48290 WHERE `entry` = 48319;
UPDATE `creature_template` SET `KillCredit1` = 48290 WHERE `entry` = 48136;

-- Dark Ranger SAI
SET @ENTRY := 48269;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,0,0,2300,3900,11,74613,64,0,0,0,0,2,0,0,0,0,0,0,0,"Cast Shoot"),
(@ENTRY,0,1,0,11,0,100,1,0,0,0,0,11,89702,0,0,0,0,0,1,0,0,0,0,0,0,0,"Cast Camouflage on Spawn"),
(@ENTRY,0,2,0,7,0,100,1,0,0,0,0,11,89702,0,0,0,0,0,1,0,0,0,0,0,0,0,"Cast Camouflage on Evade"),
(@ENTRY,0,3,1,8,0,100,0,89821,0,0,0,33,48269,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit");


SET @ENTRY := 48503;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,19,0,100,0,28400,0,0,0,15,28400,0,0,0,0,0,7,0,0,0,0,0,0,0,'Quest Credit on Quest Add');

SET @ENTRY := 48470;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,19,0,100,0,28400,0,0,0,15,28400,0,0,0,0,0,7,0,0,0,0,0,0,0,'Quest Credit on Quest Add');

UPDATE `creature_template` SET `KillCredit1` = 48752 WHERE `entry` = 48742;
UPDATE `creature_template` SET `KillCredit1` = 48752 WHERE `entry` = 48741;






UPDATE `creature` SET `spawndist` = 0, `MovementType` = 0 WHERE `id` IN (44941,45263);





















































