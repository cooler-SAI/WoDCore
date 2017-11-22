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



SET @ENTRY := 37960;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,8,0,100,0,70874,0,0,0,33,39157,0,0,0,0,0,7,0,0,0,0,0,0,0,"on Spell Hit - Give Credit"),
(@ENTRY,0,1,2,61,0,100,0,0,0,0,0,41,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"on Spell Hit - Despawn");




UPDATE `creature` SET `spawndist` = 8, `MovementType` = 1 WHERE `id` IN (50039,37070,46363,2031,706,37073,15294,15271,3101,3102,15273,35200,34865,1501);


UPDATE `creature` SET `spawndist` = 0, `MovementType` = 0 WHERE `id` IN (14545,36697,106,44617,37173,49477,49478,37218,62,12349,14543,14544,12346,12350,39014);
















