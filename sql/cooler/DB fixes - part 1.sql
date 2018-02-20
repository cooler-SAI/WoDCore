DELETE FROM `creature_template_addon` WHERE (`entry`=44857);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (44857, 0, 0, 3, 1, 0, 78677);
UPDATE `creature_template` SET `InhabitType` = 1 WHERE `entry` = 40222;
UPDATE `creature` SET `spawndist` = 0, `MovementType` = 0 WHERE `guid` = 82198;

UPDATE `creature` SET `spawndist` = 0, `MovementType` = 0 WHERE `id` = 39251;
SET @ENTRY := 39251;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,4,1,100,1,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Stop Moving on Aggro');


-- Gappy Silvertooth SAI
SET @ENTRY := 35126;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,62,0,100,0,10620,0,0,0,56,47045,1,0,0,0,0,0,0,0,0,0,0,0,0,"Gappy Silvertooth - On Gossip Option 0 Selected - Add Item 'Shiny Bling' 1 Time"),
(@ENTRY,0,1,0,61,0,100,0,10620,0,0,0,72,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"Gappy Silvertooth - On Gossip Option 0 Selected - Close Gossip"),
(@ENTRY,0,2,0,64,0,100,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,"Gappy Silvertooth - On Gossip Hello - Play Random Emote (1, 3, 5, 6)");

-- Szabo SAI
SET @ENTRY := 35128;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,62,0,100,0,10622,0,0,0,56,47046,1,0,0,0,0,0,0,0,0,0,0,0,0,"Add Item on Gossip"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,72,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"Close Gossip on Gossip"),
(@ENTRY,0,2,0,64,0,100,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,"Play Emote on Gossip");


-- Missa Spekkies SAI
SET @ENTRY := 35130;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,62,0,100,0,10624,0,0,0,56,47047,1,0,0,0,0,0,0,0,0,0,0,0,0,"Add Item on Gossip"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,72,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"Close Gossip on Gossip"),
(@ENTRY,0,2,0,64,0,100,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,"Play Emote on Gossip");

-- Kezan Partygoer SAI
SET @ENTRY := 35186;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,8,0,100,0,75046,0,15000,15000,5,10,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Spellhit Play Emote"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Spellhit Say Text"),
(@ENTRY,0,2,3,8,0,100,0,75044,0,15000,15000,5,17,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Spellhit Play Emote"),
(@ENTRY,0,3,0,61,0,100,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Spellhit Say Text"),
(@ENTRY,0,4,5,8,0,100,0,75042,0,12000,12000,5,7,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Spellhit Play Emote"),
(@ENTRY,0,5,0,61,0,100,0,0,0,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Spellhit Say Text"),
(@ENTRY,0,6,7,8,0,100,0,75048,0,12000,12000,5,7,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Spellhit Play Emote"),
(@ENTRY,0,7,0,61,0,100,0,0,0,0,0,1,3,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Spellhit Say Text"),
(@ENTRY,0,8,9,8,0,100,0,75050,0,10000,10000,5,7,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Spellhit Play Emote"),
(@ENTRY,0,9,0,61,0,100,0,0,0,0,0,1,4,0,0,0,0,0,1,0,0,0,0,0,0,0,"On Spellhit Say Text"),
(@ENTRY,0,10,0,1,0,60,0,2000,8000,10000,20000,1,5,0,0,0,0,0,1,0,0,0,0,0,0,0,"Say Text OOC"),
(@ENTRY,0,11,0,64,0,100,0,0,0,0,0,10,1,3,5,6,0,0,1,0,0,0,0,0,0,0,"Play Emote on Gossip"),
(@ENTRY,0,12,0,8,0,100,0,75046,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,13,0,8,0,100,0,75044,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,14,0,8,0,100,0,75042,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,15,0,8,0,100,0,75048,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,16,0,8,0,100,0,75050,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,17,0,8,0,100,0,75046,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,18,0,8,0,100,0,75044,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,19,0,8,0,100,0,75042,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,20,0,8,0,100,0,75048,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,21,0,8,0,100,0,75050,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,22,1,62,0,100,0,179319,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"Orgus - On Gossip - Kill Credit"),
(@ENTRY,0,23,0,61,0,100,0,0,0,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,"on Gossip- Despawn");



SET @ENTRY := 35175;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,12,0,8,0,100,0,75046,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,13,0,8,0,100,0,75044,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,14,0,8,0,100,0,75042,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,15,0,8,0,100,0,75048,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,16,0,8,0,100,0,75050,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,17,0,8,0,100,0,75046,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,18,0,8,0,100,0,75044,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,19,0,8,0,100,0,75042,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,20,0,8,0,100,0,75048,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,21,0,8,0,100,0,75050,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit");


UPDATE `creature_template` SET `gossip_menu_id` = 179319, `npcflag` = 1 WHERE `entry` = 35186;
UPDATE `creature_template` SET `gossip_menu_id` = 179319, `npcflag` = 1 WHERE `entry` = 35175;

REPLACE INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES('179319','0','0','Hi!!Party!!!','1','1','0','0','0','0','');

SET @ENTRY := 35175;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,62,0,100,0,179319,0,0,0,33,35175,0,0,0,0,0,7,0,0,0,0,0,0,0,"Orgus - On Gossip - Kill Credit"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,41,2000,0,0,0,0,0,1,0,0,0,0,0,0,0,"on Gossip- Despawn");


-- Trade Prince Gallywix SAI
SET @ENTRY := 35222;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,20,0,100,0,14126,0,0,0,62,648,0,0,0,0,0,0,8,0,0,533.77,3274.62,0.198,4.9,"Teleport to Lost Isle"),
(@ENTRY,0,1,0,20,0,100,0,14126,0,0,0,68,22,0,0,0,0,0,7,0,0,0,0,0,0,0,"Play movie on quest reward"),
(@ENTRY,0,2,3,20,0,100,0,14126,0,0,0,28,59087,0,0,0,0,0,7,0,0,0,0,0,0,0,"Remove on quest reward");



UPDATE `creature_template` SET `unit_flags` = 536871814, `dynamicflags` = 32, `type_flags` = 128 WHERE `entry` = 50047;
UPDATE `creature` SET `spawndist` = 0, `MovementType` = 0 WHERE `id` IN (50047);
DELETE FROM `creature_template_addon` WHERE (`entry`=50047);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (50047, 0, 0, 7, 1, 65, '');
UPDATE `creature_template` SET `unit_flags2` = 1 WHERE `entry` = 50047;


DELETE FROM `creature_template_addon` WHERE (`entry`=49044);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (49044, 0, 0, 50331648, 1, 0, '');


UPDATE `creature` SET `spawndist` = 8, `MovementType` = 1 WHERE `id` IN (41258,39259,43325,43089,37507,37218,37105,44098,503,522,45785,44087,45740,203,898,43084,429,432,431,568,43535,43533,345,580,4463,43532,578,424,1216,127,517,458,831,42677,456,117,46,732,1922,2442,285,735,40,524,822,4357,4356,4334,4328,4323,4331,4329,4324,4415,4393,4394,4345,4348,23637,4344,4412,39138,23594,4388,4378,4379,4376,23979,5184,4401,4389,23679,23589,4397,4359,4362,23554,23585,23555,23593,23714,4351,4352,23592,4385,4341,4346,4414,4834,4347,36673,36914,36850,36849,36013,36015,36012,36611,36592,6202,36640,36636,36639,36637,8761,8764,37741,6350,7885,6377,4343,36868,6352,6195,6190,36384,36989,37002,36147,35111,39376,39449,39042,39193,38845,38360,38359,2950,39632,4166,6370,2957,2959,2951,2949,36942,36943,2955,2952,36816,2966,36696,36708,36825,2958,2956,2970,2969,3100,3278,3244,3035,3273,3415,2974,37740,34763,3272,3269,3198,4127,3242,3248,3125,42859,39272,39603,3106,2972,2979,2973,2963,2977,2962,36827,3126,3114,3126,3122,3115,3255,39452,3279,36828,3276,2976,3245,3241,3277,2971,3566,2964,2965,34640,3267,3102,3255,44166,38141,36832,37178,37175,3232,36845,3126,35831,3114,3425,3111,3112,3197,39595,3380,3256,34647,39596,3426,36719,38359,36740,43331,3280,39385,46491,3123,3113,3114,34503,3195,3196,3199,3116,3127,3265,34545,34503,3246,3266,3268,3269,3379,3285,3284,3282,3274,3461,3397,3283,3286,3110,38360,6375,4316,3234,9523,36103,5629,34707,35833,3386,35832,36385,3416,36061,3275,36681,35096,35111,35177,35466,35245,35759,6201,36593,36304,35149,36815,36131,35312,36936,4382);

UPDATE `creature` SET `spawndist` = 8, `MovementType` = 1 WHERE `id` IN (46404,47490,45570,45358,45359,45395,45334,47728,47724,48476,46158,49024,45187,45987,43182,44229,44364,44358,42824,44077,44079,43123,44363,16878,45346,14361,7342,7345,7346,7344,7347,11781,11785,11782,46260,46252,46250,47801,45190,45202,46920,46617,46384,45949,51193,48625,47801,48629,51753,47283,47803,47201,51193,51217,47283,46317,51676,46315,47738,45765,51674,46590,47801,46311,47720,45905,46871,51712,51713,46868,39342,41029,39437,40123,39964,40065,40219,40223,40987,40275,41925,41926,41747,41118,41108,40989,41105,44423,27834,41780,41107,40911,40200,42341,41996,41042,40223,40370,40677,40855,39313,41746,41798,40753,41808,2923,43891,2657,42913,2927,2924,2928,2925,2639,4466,4467,2659,42879,2647,2646,2644,2645,2642,2643,42689,2641,2640,2680,4465,2655,2656,2929,42518,42554,42536,42555,2926,2505,2573,2776,2558,2590,2256,2253,2256,2287,2251,54509,2250,2370,2368,45728,16030,45734,48628,48742,2373,48482,48484,48136,48319,47859,47789,47759,2375,47205,45280,45855,46053,45195,45270,1530,1541,1547,16326,15654,15274,15372,42154,42155,11324,11322,11323,11319,11318,11321,11320,42043,42042,41449,41424,41453,41483,41401,41496,41390,1032,41409,41427,41428,42140,41425,44116,44206,41272,1161,41394,41147,41080,41145,1007,41072,44576,44635,44616,1162,1197,1164);


UPDATE `creature_template` SET `npcflag` = 16777217 WHERE `entry` = 49340;

REPLACE INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES('49340','0','0','Take body...','1','1','0','0','0','0','');


SET @ENTRY := 49340;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,62,0,100,0,49340,0,0,0,33,49340,0,0,0,0,0,7,0,0,0,0,0,0,0,"Orgus - On Gossip - Kill Credit"),
(@ENTRY,0,1,0,61,0,100,0,0,0,0,0,41,1000,0,0,0,0,0,1,0,0,0,0,0,0,0,"on Gossip- Despawn");

UPDATE `creature_template` SET `flags_extra`=0 WHERE `entry`=49340; 
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=49340; 
UPDATE `creature` SET `phaseMask`=0 WHERE `id`=49340; 
UPDATE `creature_addon` SET `auras`='' WHERE `guid` IN (SELECT `guid` FROM `creature` WHERE `id`=49340);

UPDATE `creature_template` SET `npcflag` = 1, `unit_flags` = 536904192, `unit_flags2` = 133120 WHERE `entry` = 49340;

UPDATE `creature_template` SET `unit_flags` = 536871814, `dynamicflags` = 32, `type_flags` = 128 WHERE `entry` = 49340;
UPDATE `creature` SET `spawndist` = 0, `MovementType` = 0 WHERE `id` IN (49340);
DELETE FROM `creature_template_addon` WHERE (`entry`=49340);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (49340, 0, 0, 7, 1, 65, '');
UPDATE `creature_template` SET `unit_flags2` = 1 WHERE `entry` = 49340;
UPDATE `creature_template` SET `scale` = 3.5 WHERE `entry` = 1553;
UPDATE `creature_template` SET `scale` = 3 WHERE `entry` = 1513;
UPDATE `creature_template` SET `scale` = 3 WHERE `entry` = 1512;
UPDATE `creature_template` SET `scale` = 1.5 WHERE `entry` = 1508;

UPDATE `quest_template` SET `PrevQuestId` = 24971 WHERE `Id` = 24972;
UPDATE `quest_template` SET `RequiredRaces` = 2098253, `Flags` = 524288 WHERE `Id` = 2159;

UPDATE `quest_template` SET `Flags` = 3735552 WHERE `Id` = 2159;
UPDATE `quest_template` SET `NextQuestId` = 24972, `ExclusiveGroup` = 0 WHERE `Id` = 24971;
UPDATE `quest_template` SET `Flags` = 0 WHERE `Id` = 2159;

UPDATE `quest_template` SET `RequiredRaces` = 0, `Flags` = 8 WHERE `Id` = 2159;

UPDATE `creature_template` SET `InhabitType` = 1 WHERE `entry` = 1995;

UPDATE `quest_template` SET `SpecialFlags` = 0 WHERE `SpecialFlags` = 4;
UPDATE `creature_template` SET `faction` = 189 WHERE `entry` = 37956;
UPDATE `creature_template` SET `faction` = 189 WHERE `entry` = 38046;

UPDATE `creature_template` SET `npcflag` = 0 WHERE `entry` = 42645;
UPDATE `creature_template` SET `minlevel` = 1, `maxlevel` = 1 WHERE `entry` = 42839;
SET @ENTRY := 43325;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,0,0,2300,3900,11,80944,64,0,0,0,0,2,0,0,0,0,0,0,0,'Cast Fling Fungus'),
(@ENTRY,0,1,0,9,0,100,0,0,20,15000,25000,11,87347,0,0,0,0,0,2,0,0,0,0,0,0,0,'Cast Giftpilz on Close');
UPDATE `creature_template` SET `faction` = 189 WHERE `entry` = 34511;
UPDATE `creature_template` SET `faction` = 189 WHERE `entry` = 34884;
UPDATE `creature_template` SET `faction` = 189 WHERE `entry` = 35627;
SET @ENTRY := 37960;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,8,0,100,0,70874,0,0,0,33,39157,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,1,2,61,0,100,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"on Spell Hit - Despawn");

DELETE FROM `gameobject` WHERE `id`=235883;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`custom_flags`,`protect_anti_doublet`) VALUES
(8489641, 235883, 1116, 6941, 7334, 1, 1, 3734.65, -4042.98, 44.8187, 0.995794, 0, 0, 0, 0, 120, 255, 1, 0, 0, 0),
(8489640, 235883, 1190, 4, 5077, 1, 1, -11266.7, -3624.88, 10.9119, 0, 0, 0, 0, 0, 0, 255, 1, 0, 0, 0),
(22164838, 235883, 870, 6134, 6369, 1, 4294967295, -1021.78, -1071.57, 1.11939, 5.7793, 0, 0, 0.249284, -0.96843, 300, 0, 1, 0, 0, 0);


DELETE FROM `creature_template_addon` WHERE (`entry`=47091);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (47091, 0, 0, 0, 1, 0, 84063);


DELETE FROM `creature_template_addon` WHERE (`entry`=35912);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (35912, 0, 0, 0, 1, 0, 84063);


DELETE FROM `gameobject` WHERE `id`=190960;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`custom_flags`,`protect_anti_doublet`) VALUES
(31831, 190960, 571, 4395, 4740, 1, 1, 5719.19, 719.681, 641.728, 0.837757, 0, 0, 0, 1, 300, 100, 1, 0, 0, 0),
(59339, 190960, 1, 616, 4990, 1, 8, 4845.42, -2812.35, 1440.17, 5.88565, 0, 0, 0.197463, -0.98031, 300, 0, 1, 0, 0, 0),
(59369, 190960, 1, 616, 4990, 1, 8, 4878.77, -2795.11, 1435.34, 2.76525, 0, 0, 0.982348, 0.187064, 300, 0, 1, 0, 0, 0);

UPDATE `creature` SET `phaseMask`="1" WHERE `phaseMask`="1024" AND `map`="654" AND `zoneId`="4714"; 

SET @ENTRY := 36231;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,8,0,100,0,69094,0,0,0,33,36233,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,1,2,61,0,100,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"on Spell Hit - Despawn");


-- Ashenvale Bowman
SET @ENTRY := 33195;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,0,0,2300,3900,11,6660,64,0,0,0,0,2,0,0,0,0,0,0,0,'Cast Shoot'),
(@ENTRY,0,1,0,2,0,100,1,0,15,0,0,25,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Flee at 15% HP'),
(@ENTRY,0,2,0,0,0,100,0,5000,8000,12000,15000,11,78754,0,0,0,0,0,2,0,0,0,0,0,0,0,'Cast Arcane Shot'),
(@ENTRY,0,3,0,0,0,100,0,4000,10000,10000,22000,11,78841,0,0,0,0,0,2,0,0,0,0,0,0,0,'Cast Aimed Shot');

-- Forest Spider
SET @ENTRY := 30;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,0,5000,8000,12000,15000,11,11918,0,0,0,0,0,2,0,0,0,0,0,0,0,'Cast Poison');

UPDATE creature_template SET ScriptName = "grimrail_depot_mob_gromkar_gunner", AIName = "" WHERE entry = 77483;
UPDATE `creature_template` SET ScriptName='spell_q24747_custom_kill_credit' WHERE entry=38250;
UPDATE creature_template SET ScriptName = "npc_lurking_tempest", AIName = "" WHERE entry = 45704;

 
 
 
  
   
 
  
REPLACE INTO spell_script_names VALUES
(6229, 'spell_warl_twilight_ward_s12'),
(104048, 'spell_warl_twilight_ward_s12'); 
REPLACE INTO `spell_script_names` VALUES (175217, 'spell_gen_draenor_feast');
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (88685, 'spell_pri_holy_word_sanctuary');
REPLACE INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES (161399,'spell_item_Swapblaster'); 

REPLACE INTO spell_script_names VALUES
(50051, "spell_ethereal_pet_aura"),
(50101, "spell_ethereal_pet_onkill_steal_essence");

REPLACE INTO spell_script_names VALUE (162407, 'grimrail_depot_rocketspark_spell_x21_01a_missile_barrage');
REPLACE INTO spell_script_names VALUE (162500, 'grimrail_depot_rocketspark_spell_vx18_target_eliminator');
REPLACE INTO spell_script_names VALUE (162059, 'grimrail_depot_skylord_torva_at_freezing_snare');
REPLACE INTO spell_script_names VALUE (166565, 'grimrail_depot_nitrogg_thundertower_slag_blast');
REPLACE INTO spell_script_names VALUE (166571, 'grimrail_depot_nitrogg_thundertower_at_slagblast');
REPLACE INTO spell_script_names VALUE (161589, 'grimrail_depot_skylord_tovra_at_diffused_energy');
REPLACE INTO spell_script_names VALUE (166346, 'grimrail_depot_skylord_tovra_spell_thunder_zone');
REPLACE INTO spell_script_names VALUE (162058, 'grimrail_depot_skylord_tovra_at_spinning_spear');
REPLACE INTO spell_script_names VALUE (166341, 'grimrail_depot_at_thunder_zone');
REPLACE INTO spell_script_names VALUE (153690, 'shadowmoon_burial_grounds_bonemaw_necrotic_pitch');
REPLACE INTO spell_script_names VALUE (93720, 'spell_echo_of_sylvanas_wracking_pain_dmg');
REPLACE INTO spell_script_names VALUE (74805, 'spell_halion_summon_exit_portals');
REPLACE INTO spell_script_names VALUE (163966, 'grimrail_depot_spell_activating');
REPLACE INTO spell_script_names VALUE (176033, 'grimrail_depot_at_flametongue');
REPLACE INTO spell_script_names VALUE (166387, 'grimrail_depot_at_healing_rain');
REPLACE INTO spell_script_names VALUE (166397, 'grimrail_depot_spell_arcane_blitz');
REPLACE INTO spell_script_names VALUE (166676, 'grimrail_depot_spell_sharpnel_blast');
REPLACE INTO spell_script_names VALUE (164168, 'grimrail_depot_spell_dash_dummy');

REPLACE INTO spell_script_names VALUE (163550, 'grimrail_depot_nitrogg_thundertower_spell_blackrock_bomb');
REPLACE INTO spell_script_names VALUE (160681, 'grimrail_depot_nitrogg_thundertower_spell_suppressive_fire');
REPLACE INTO spell_script_names VALUE (153068, 'shadowmoon_burial_grounds_nhalish_spell_void_devestation');
REPLACE INTO spell_script_names VALUE (153072, 'shadowmoon_burial_grounds_nhallish_at_vd');
REPLACE INTO spell_script_names VALUE (233920, 'shadowmoon_burial_grounds_gameobject_nerzul_door');
REPLACE INTO spell_script_names VALUE (75875, 'spell_halion_mark_damage_aura_heroic');
REPLACE INTO spell_script_names VALUE (178795, 'spell_ashran_blade_twister');
REPLACE INTO spell_script_names VALUE (176670, 'spell_ashran_emberfall_living_bomb');
REPLACE INTO spell_script_names VALUE (161091, 'grimrail_depot_rocketspark_spell_new_plan');

REPLACE INTO spell_script_names VALUE (134758, 'at_quilen_spear');
REPLACE INTO spell_script_names VALUE (136421, 'at_quilen_spear');
REPLACE INTO spell_script_names VALUE (136451, 'at_quilen_spear');
REPLACE INTO spell_script_names VALUE (137416, 'at_flames_of_passion');
REPLACE INTO spell_script_names VALUE (134628, 'spell_unleashed_flame');
REPLACE INTO spell_script_names VALUE (122385, 'spell_ook_ook_barrel');
REPLACE INTO spell_script_names VALUE (122949, 'spell_unseen_strike');
REPLACE INTO spell_script_names VALUE (138435, 'spell_arcing_lightning');
REPLACE INTO spell_script_names VALUE (137499, 'at_lightning_whip');
REPLACE INTO spell_script_names VALUE (135150, 'at_crashing_thunder');
REPLACE INTO spell_script_names VALUE (137232, 'spell_dead_zones');
REPLACE INTO spell_script_names VALUE (135145, 'spell_frozen');

REPLACE INTO `achievement_criteria_data` (`criteria_id`,`type`,`value1`,`value2`,`ScriptName`) VALUES
(10568,11,0,0,'achievement_he_feeds_on_your_tears'), -- He Feeds On Your Tears (10 player)
(10570,11,0,0,'achievement_he_feeds_on_your_tears'); -- He Feeds On Your Tears (25 player)

REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `AIName`, `ScriptName`, `BuildVerified`) VALUES('900000','5','19153','Duel Area','','','','0','0','1.5','0','0','0','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','go_duel_area_flag','1');
delete from `gameobject_template` where entry = 237738;
insert into `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `AIName`, `ScriptName`, `BuildVerified`) values('237738','22','20809','Portal to Ashran','','','','0','0','1','0','0','0','0','0','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','go_ashran_portal','19116');
REPLACE INTO `achievement_criteria_data` VALUES
(7703, 6, 4197, 0, ''),
(7703, 11, 0, 0, 'achievement_wg_didnt_stand_a_chance');


UPDATE gameobject_template SET ScriptName = "auchindoun_teronogor_gameobject_soul_transporter_04" WHERE entry = 345368;

UPDATE creature_template SET ScriptName = "the_everbloom_mob_dreadpital_toxin", AIName = "" WHERE entry = 81864;

UPDATE creature_template SET ScriptName = "the_everbloom_xeritac_mob_venom_sprayer", AIName = "" WHERE entry = 86547;
UPDATE creature_template SET ScriptName = "the_everbloom_yalnu_mob_vicious_mandragora", AIName = "" WHERE entry = 84399;
UPDATE creature_template SET ScriptName = "grimrail_depot_mob_grimrail_scout", AIName = "" WHERE entry = 82590;
UPDATE creature_template SET ScriptName = "grimrail_depot_mob_grimrail_technician", AIName = "" WHERE entry = 81236;

REPLACE INTO `instance_template` (`map`, `parent`, `script`, `allowMount`) VALUES
(1208, 1116, 'instance_grimrail_depot', 0),  
(1112, 1116, 'instance_scenario_black_temple', 0);

UPDATE creature_template SET ScriptName = "grimrail_depot_mob_gromkar_capitan", AIName = "" WHERE entry = 82597;
UPDATE creature_template SET ScriptName = "grimrail_depot_mob_gromkar_farseer", AIName = "" WHERE entry = 82579;
UPDATE creature_template SET ScriptName = "grimrail_depot_skylord_tovra_creature_rkuna", AIName = "" WHERE entry = 80004;
UPDATE creature_template SET ScriptName = "grimrail_depot_mob_grimrail_technician", AIName = "" WHERE entry = 81236;
UPDATE creature_template SET ScriptName = "grimrail_depot_mob_grimrail_technician", AIName = "" WHERE entry = 81236;
UPDATE creature_template SET ScriptName = "grimrail_depot_mob_grimrail_technician", AIName = "" WHERE entry = 81236;
UPDATE creature_template SET ScriptName = "grimrail_depot_mob_grimrail_technician", AIName = "" WHERE entry = 81236;


UPDATE creature_template SET ScriptName = "grimrail_depot_mob_grimrail_bombadier", AIName = "" WHERE entry = 81407;
UPDATE creature_template SET ScriptName = "grimrail_depot_mob_assault_cannon", AIName = "" WHERE entry = 79548;
UPDATE creature_template SET ScriptName = "grimrail_depot_mob_grimrail_laborer", AIName = "" WHERE entry = 81235;
UPDATE creature_template SET ScriptName = "grimrail_depot_mob_grimrail_overseer", AIName = "" WHERE entry = 81212;
UPDATE creature_template SET ScriptName = "grimrail_depot_mob_gromkar_cinderseer", AIName = "" WHERE entry = 88163;
UPDATE creature_template SET ScriptName = "grimrail_depot_mob_gromkar_hulk", AIName = "" WHERE entry = 80938;
UPDATE creature_template SET ScriptName = "grimrail_depot_rocketspark_mob_borka", AIName = "" WHERE entry = 86226;
UPDATE creature_template SET ScriptName = "grimrail_depot_nitrogg_thundertower_mob_gromkar_grenadier", AIName = "" WHERE entry = 79739;
UPDATE creature_template SET ScriptName = "grimrail_depot_nitrogg_thundertower_mob_gromkar_boomer", AIName = "" WHERE entry = 79720;
UPDATE creature_template SET ScriptName = "grimrail_depot_nitrogg_thundertower_mob_iron_turret", AIName = "" WHERE entry = 85216;
UPDATE `creature_template` SET `speed_run`=1.28571426868439, `npcflag`=0, `VehicleId`=494, `minlevel`=4, `maxlevel`=4, `Unit_Flags`=8, `Modelid1`=236, `Resistance2`=90, `Resistance4`=1, `unit_class`=2, `scriptname`='npc_greymane_horse', `mechanic_immune_mask`=2147483647 WHERE `entry`=35905;
UPDATE `creature_template` SET `ScriptName` = 'mob_general_purpose_bunnyJMF' WHERE `entry` = 55091;
UPDATE creature_template SET ScriptName = "boss_skylord_torva", AIName = "" WHERE entry = 80005;
UPDATE creature_template SET ScriptName = "boss_nitrogg_thundertower", AIName = "" WHERE entry = 79545;
UPDATE creature_template SET ScriptName = "boss_rocketspark", AIName = "" WHERE entry = 77803;
UPDATE creature_template SET ScriptName = "the_everbloom_archmage_sol_mob_noxious_explosion", AIName = "" WHERE entry = 213151;
UPDATE `creature_template` SET `ScriptName` = 'npc_gunship_cannon' WHERE `entry` = 36838;
UPDATE `creature_template` SET `ScriptName`='npc_image_belgaristrasz' WHERE `entry`=28012;
UPDATE areatrigger_template SET ScriptName='at_down_draft' WHERE spell_id=134370;
UPDATE `creature_template` SET `ScriptName` = 'grimrail_depot_nitrogg_thundertower_mob_assault_turret' WHERE `entry` = 82721;
UPDATE `creature_template` SET `ScriptName` = 'grimrail_depot_nitrogg_thundertower_mob_iron_infantry' WHERE `entry` = 79888;
UPDATE `creature_template` SET `ScriptName` = 'auchindoun_kaathar_mob_teleport_players' WHERE `entry` = 3242352;
UPDATE `creature_template` SET `ScriptName` = 'npc_PetBattleTrainer' WHERE `entry` IN (86056,64572,64582,63073,63070,63596,87427,63626,63086,63067,63080,63014,63061,63075,63083,63077);
UPDATE `creature_template` SET `ScriptName` = 'npc_treant_balance' WHERE `entry` = 1964;
UPDATE `creature_template` SET `ScriptName` = 'npc_pvp_training_dummy' WHERE `entry` = 2673;
UPDATE `creature_template` SET `ScriptName` = 'npc_crowley_horse' WHERE `entry` = 35231;

DROP TABLE IF EXISTS `fun_tarrenmill_reward`;

CREATE TABLE `fun_tarrenmill_reward` 
(
  `kills` int(10) unsigned NOT NULL,
  `title_h` int(10) unsigned NOT NULL,
  `title_a` int(10) unsigned NOT NULL,
  `item_h` int(10) unsigned NOT NULL,
  `item_a` int(10) unsigned NOT NULL,
  `aura_a` int(10) unsigned NOT NULL,
  `aura_h` int(10) unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


UPDATE `creature` SET `spawndist` = 8, `MovementType` = 1 WHERE `id` IN (50039,57205,53704,56730,36231,34511,16521,37070,46363,2031,706,37073,15294,15271,3101,3102,15273,35200,34865,1501);


UPDATE `creature` SET `spawndist` = 0, `MovementType` = 0 WHERE `id` IN (16518,14545,36697,106,44617,37173,49477,49478,37218,62,12349,14543,14544,12346,12350,39014);
















