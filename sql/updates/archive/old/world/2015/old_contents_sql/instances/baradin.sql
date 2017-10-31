update creature set spawnmask=120 where spawnmask=15 and map=757;
update gameobject set spawnmask=120 where spawnmask=15 and map=757;
update creature set spawnmask=120 where spawnmask=1 and map=757;
update gameobject set spawnmask=120 where spawnmask=1 and map=757;
update creature set spawnmask=120 where spawnmask=3 and map=757;
update gameobject set spawnmask=120 where spawnmask=3 and map=757;

DELETE FROM `creature` WHERE `id`=52368;
DELETE FROM `creature` WHERE `id`=52389;
DELETE FROM `creature` WHERE `id`=52369;

UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=207619;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=208953;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=209849;

UPDATE `creature_template` SET minlevel=87, maxlevel=87, flags_extra=128 WHERE `entry`=52369;
UPDATE `creature_template` SET minlevel=87, maxlevel=87, flags_extra=128 WHERE `entry`=52368;

UPDATE `instance_template` SET `Script`='instance_baradin_hold' WHERE `map`=757;

UPDATE `creature_template` SET `ScriptName`='boss_argaloth' WHERE `entry`=47120;

UPDATE `creature_template` SET `ScriptName`='boss_occuthar' WHERE `entry`=52363;
UPDATE `creature_template` SET `ScriptName`='npc_occuthar_eyestalk' WHERE `entry`=52369;
UPDATE `creature_template` SET `ScriptName`='npc_occuthar_eye_of_occuthar' WHERE `entry`=52389;

DELETE FROM `spell_script_names` WHERE `spell_id`=96931;
INSERT INTO `spell_script_names` VALUES (96931, 'spell_occuthar_eye_of_occuthar');

UPDATE `creature_template` SET `ScriptName`='boss_alizabal' WHERE `entry`=55869;

DELETE FROM `spell_script_names` WHERE `spell_id`=105065;
INSERT INTO `spell_script_names` VALUES (105065,'spell_alizabal_seething_hate');
DELETE FROM `spell_script_names` WHERE `spell_id`=106248;
INSERT INTO `spell_script_names` VALUES (106248,'spell_alizabal_blade_dance');

DELETE FROM `creature_text` WHERE `entry`=55869;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(55869, 0, 0, 'I hate adventurers.', 14, 0, 0, 0, 0, 25777, 'VO_BH_ALIZABAL_AGGRO_01'),
(55869, 1, 0, 'I hate... every one of you...', 14, 0, 0, 0, 0, 25778, 'VO_BH_ALIZABAL_DEATH_01'),
(55869, 2, 0, 'How I HATE this place. My captors may be long-dead, but don\'t think I won\'t take it all out on you miserable treasure-hunters.', 14, 0, 0, 0, 0, 25779, 'VO_BH_ALIZABAL_INTRO_01'),
(55869, 3, 0, 'I hate incompetent raiders.', 14, 0, 0, 0, 0, 25780, 'VO_BH_ALIZABAL_RESET_01'),
(55869, 4, 0, 'I still hate you.', 14, 0, 0, 0, 0, 25781, 'VO_BH_ALIZABAL_SLAY_01'),
(55869, 4, 1, 'Do you hate me? Good.', 14, 0, 0, 0, 0, 25782, 'VO_BH_ALIZABAL_SLAY_02'),
(55869, 4, 2, 'I hate mercy.', 14, 0, 0, 0, 0, 25782, 'VO_BH_ALIZABAL_SLAY_03'),
(55869, 4, 3, 'I didn\'t hate that.', 14, 0, 0, 0, 0, 25784, 'VO_BH_ALIZABAL_SLAY_04'),
(55869, 5, 0, 'I hate armor.', 14, 0, 0, 0, 0, 25785, 'VO_BH_ALIZABAL_SPELL_01'),
(55869, 5, 1, 'I hate martyrs.', 14, 0, 0, 0, 0, 25786, 'VO_BH_ALIZABAL_SPELL_02'),
(55869, 5, 2, 'Feel my hatred!', 14, 0, 0, 0, 0, 25787, 'VO_BH_ALIZABAL_SPELL_03'),
(55869, 5, 3, 'My hatred burns!', 14, 0, 0, 0, 0, 25788, 'VO_BH_ALIZABAL_SPELL_04'),
(55869, 5, 4, 'My hate will consume you!', 14, 0, 0, 0, 0, 25789, 'VO_BH_ALIZABAL_SPELL_05'),
(55869, 6, 0, 'I hate you all!', 14, 0, 0, 0, 0, 25790, 'VO_BH_ALIZABAL_SPELL_06'),
(55869, 6, 1, 'I hate standing still.', 14, 0, 0, 0, 0, 25791, 'VO_BH_ALIZABAL_SPELL_07');

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(104994, 'spell_alizabal_blade_dance_dmg');

UPDATE access_requirement SET level_min=85 WHERE mapId IN (732, 757);

replace into instance_template values (757, 0, 'instance_baradin_hold', 0);

UPDATE `creature_template` SET `dynamicflags` = 0 WHERE `dynamicflags` = 13;
UPDATE `creature` SET `dynamicflags` = 0 WHERE `dynamicflags` = 13;

delete from creature where id=47903;

DELETE FROM `creature_loot_template` WHERE (`entry`=55869);
INSERT INTO `creature_loot_template` VALUES 
(55869, 73479, 0, 1, 1, 1, 1),
(55869, 73481, 0, 1, 2, 1, 1),
(55869, 73485, 0, 1, 1, 1, 1),
(55869, 73487, 0, 1, 2, 1, 1),
(55869, 73488, 0, 1, 1, 1, 1),
(55869, 73489, 0, 1, 2, 1, 1),
(55869, 73491, 0, 1, 1, 1, 1),
(55869, 73492, 0, 1, 2, 1, 1),
(55869, 73493, 0, 1, 1, 1, 1),
(55869, 73494, 0, 1, 2, 1, 1),
(55869, 73495, 0, 1, 1, 1, 1),
(55869, 73496, 0, 1, 2, 1, 1),
(55869, 73497, 0, 1, 1, 1, 1),
(55869, 73498, 0, 1, 2, 1, 1),
(55869, 73503, 0, 1, 1, 1, 1),
(55869, 73505, 0, 1, 2, 1, 1),
(55869, 73507, 0, 1, 1, 1, 1),
(55869, 73509, 0, 1, 2, 1, 1),
(55869, 73511, 0, 1, 1, 1, 1),
(55869, 73514, 0, 1, 2, 1, 1),
(55869, 73516, 0, 1, 1, 1, 1),
(55869, 73518, 0, 1, 2, 1, 1),
(55869, 73519, 0, 1, 1, 1, 1),
(55869, 73520, 0, 1, 2, 1, 1),
(55869, 73521, 0, 1, 1, 1, 1),
(55869, 73522, 0, 1, 2, 1, 1),
(55869, 73524, 0, 1, 1, 1, 1),
(55869, 73526, 0, 1, 2, 1, 1),
(55869, 73528, 0, 1, 1, 1, 1),
(55869, 73529, 0, 1, 2, 1, 1),
(55869, 73530, 0, 1, 1, 1, 1),
(55869, 73531, 0, 1, 2, 1, 1),
(55869, 73532, 0, 1, 1, 1, 1),
(55869, 73533, 0, 1, 2, 1, 1),
(55869, 73534, 1, 1, 0, 1, 1),
(55869, 73535, 1, 1, 0, 1, 1),
(55869, 73536, 1, 1, 0, 1, 1),
(55869, 73537, 1, 1, 0, 1, 1),
(55869, 73538, 1, 1, 0, 1, 1),
(55869, 73539, 1, 1, 0, 1, 1),
(55869, 73542, 0, 1, 1, 1, 1),
(55869, 73544, 0, 1, 2, 1, 1),
(55869, 73547, 0, 1, 1, 1, 1),
(55869, 73549, 0, 1, 2, 1, 1),
(55869, 73550, 0, 1, 1, 1, 1),
(55869, 73551, 0, 1, 2, 1, 1),
(55869, 73552, 0, 1, 1, 1, 1),
(55869, 73553, 0, 1, 2, 1, 1),
(55869, 73554, 0, 1, 1, 1, 1),
(55869, 73555, 0, 1, 2, 1, 1),
(55869, 73557, 0, 1, 1, 1, 1),
(55869, 73559, 0, 1, 2, 1, 1),
(55869, 73561, 0, 1, 1, 1, 1),
(55869, 73562, 0, 1, 2, 1, 1),
(55869, 73563, 0, 1, 1, 1, 1),
(55869, 73564, 0, 1, 2, 1, 1),
(55869, 73565, 0, 1, 1, 1, 1),
(55869, 73566, 0, 1, 2, 1, 1),
(55869, 73568, 0, 1, 1, 1, 1),
(55869, 73570, 0, 1, 2, 1, 1),
(55869, 73574, 0, 1, 1, 1, 1),
(55869, 73576, 0, 1, 2, 1, 1),
(55869, 73581, 0, 1, 1, 1, 1),
(55869, 73583, 0, 1, 2, 1, 1),
(55869, 73585, 0, 1, 1, 1, 1),
(55869, 73586, 0, 1, 2, 1, 1),
(55869, 73587, 0, 1, 1, 1, 1),
(55869, 73588, 0, 1, 2, 1, 1),
(55869, 73589, 0, 1, 1, 1, 1),
(55869, 73590, 0, 1, 2, 1, 1),
(55869, 73591, 0, 1, 1, 1, 1),
(55869, 73592, 0, 1, 2, 1, 1),
(55869, 73593, 0, 1, 1, 1, 1),
(55869, 73597, 0, 1, 2, 1, 1),
(55869, 73599, 0, 1, 1, 1, 1),
(55869, 73600, 0, 1, 2, 1, 1),
(55869, 73601, 0, 1, 1, 1, 1),
(55869, 73602, 0, 1, 2, 1, 1),
(55869, 73605, 0, 1, 1, 1, 1),
(55869, 73607, 0, 1, 2, 1, 1),
(55869, 73608, 0, 1, 1, 1, 1),
(55869, 73609, 0, 1, 2, 1, 1),
(55869, 73610, 0, 1, 1, 1, 1),
(55869, 73613, 0, 1, 2, 1, 1),
(55869, 73615, 0, 1, 1, 1, 1),
(55869, 73617, 0, 1, 2, 1, 1),
(55869, 73619, 0, 1, 1, 1, 1),
(55869, 73621, 0, 1, 2, 1, 1),
(55869, 73622, 0, 1, 1, 1, 1),
(55869, 73623, 0, 1, 2, 1, 1),
(55869, 73625, 0, 1, 1, 1, 1),
(55869, 73626, 0, 1, 2, 1, 1),
(55869, 73627, 0, 1, 1, 1, 1),
(55869, 73628, 0, 1, 2, 1, 1),
(55869, 73629, 0, 1, 1, 1, 1),
(55869, 73630, 0, 1, 2, 1, 1),
(55869, 73631, 0, 1, 1, 1, 1),
(55869, 73632, 0, 1, 2, 1, 1),
(55869, 73633, 0, 1, 1, 1, 1),
(55869, 73634, 0, 1, 2, 1, 1),
(55869, 73635, 0, 1, 1, 1, 1),
(55869, 73636, 0, 1, 2, 1, 1),
(55869, 73637, 0, 1, 1, 1, 1),
(55869, 73638, 0, 1, 2, 1, 1),
(55869, 73639, 0, 1, 1, 1, 1),
(55869, 73640, 0, 1, 2, 1, 1),
(55869, 73641, 0, 1, 1, 1, 1),
(55869, 73643, 0, 1, 2, 1, 1),
(55869, 73644, 0, 1, 1, 1, 1),
(55869, 73645, 0, 1, 2, 1, 1),
(55869, 73646, 0, 1, 1, 1, 1),
(55869, 73647, 0, 1, 2, 1, 1),
(55869, 73648, 0, 1, 1, 1, 1),
(55869, 76212, 0, 1, 2, 1, 1),
(55869, 76214, 0, 1, 1, 1, 1),
(55869, 76341, 0, 1, 2, 1, 1),
(55869, 76343, 0, 1, 1, 1, 1),
(55869, 76346, 0, 1, 2, 1, 1),
(55869, 76348, 0, 1, 1, 1, 1),
(55869, 76357, 0, 1, 2, 1, 1),
(55869, 76359, 0, 1, 1, 1, 1),
(55869, 76749, 0, 1, 2, 1, 1),
(55869, 76751, 0, 1, 1, 1, 1),
(55869, 76757, 0, 1, 2, 1, 1),
(55869, 76759, 0, 1, 1, 1, 1),
(55869, 76766, 0, 1, 2, 1, 1),
(55869, 76768, 0, 1, 1, 1, 1),
(55869, 76875, 0, 1, 2, 1, 1),
(55869, 76877, 0, 1, 1, 1, 1),
(55869, 76975, 0, 1, 2, 1, 1),
(55869, 76977, 0, 1, 1, 1, 1),
(55869, 76985, 0, 1, 2, 1, 1),
(55869, 76986, 0, 1, 1, 1, 1),
(55869, 76989, 0, 1, 2, 1, 1),
(55869, 76991, 0, 1, 1, 1, 1),
(55869, 77004, 0, 1, 2, 1, 1),
(55869, 77006, 0, 1, 1, 1, 1),
(55869, 77009, 0, 1, 2, 1, 1),
(55869, 77011, 0, 1, 1, 1, 1),
(55869, 77014, 0, 1, 2, 1, 1),
(55869, 77016, 0, 1, 1, 1, 1),
(55869, 77018, 0, 1, 2, 1, 1),
(55869, 77020, 0, 1, 1, 1, 1),
(55869, 77024, 0, 1, 2, 1, 1),
(55869, 77026, 0, 1, 1, 1, 1),
(55869, 77029, 0, 1, 2, 1, 1),
(55869, 77031, 0, 1, 1, 1, 1),
(55869, 77036, 0, 1, 2, 1, 1),
(55869, 77038, 0, 1, 1, 1, 1),
(55869, 77041, 0, 1, 2, 1, 1),
(55869, 77043, 0, 1, 1, 1, 1);

DELETE FROM `creature_loot_template` WHERE (`entry`=57270);
INSERT INTO `creature_loot_template` VALUES 
(57270, 73479, 0, 1, 1, 1, 1),
(57270, 73481, 0, 1, 2, 1, 1),
(57270, 73485, 0, 1, 3, 1, 1),
(57270, 73487, 0, 1, 4, 1, 1),
(57270, 73488, 0, 1, 1, 1, 1),
(57270, 73489, 0, 1, 2, 1, 1),
(57270, 73491, 0, 1, 3, 1, 1),
(57270, 73492, 0, 1, 4, 1, 1),
(57270, 73493, 0, 1, 2, 1, 1),
(57270, 73494, 0, 1, 3, 1, 1),
(57270, 73495, 0, 1, 4, 1, 1),
(57270, 73496, 0, 1, 1, 1, 1),
(57270, 73497, 0, 1, 2, 1, 1),
(57270, 73498, 0, 1, 3, 1, 1),
(57270, 73503, 0, 1, 4, 1, 1),
(57270, 73505, 0, 1, 1, 1, 1),
(57270, 73507, 0, 1, 2, 1, 1),
(57270, 73509, 0, 1, 3, 1, 1),
(57270, 73511, 0, 1, 4, 1, 1),
(57270, 73514, 0, 1, 1, 1, 1),
(57270, 73516, 0, 1, 2, 1, 1),
(57270, 73518, 0, 1, 3, 1, 1),
(57270, 73519, 0, 1, 4, 1, 1),
(57270, 73520, 0, 1, 1, 1, 1),
(57270, 73521, 0, 1, 2, 1, 1),
(57270, 73522, 0, 1, 3, 1, 1),
(57270, 73524, 0, 1, 4, 1, 1),
(57270, 73526, 0, 1, 1, 1, 1),
(57270, 73528, 0, 1, 2, 1, 1),
(57270, 73529, 0, 1, 3, 1, 1),
(57270, 73530, 0, 1, 4, 1, 1),
(57270, 73531, 0, 1, 1, 1, 1),
(57270, 73532, 0, 1, 2, 1, 1),
(57270, 73533, 0, 1, 3, 1, 1),
(57270, 73534, 1, 1, 0, 1, 1),
(57270, 73535, 1, 1, 0, 1, 1),
(57270, 73536, 1, 1, 0, 1, 1),
(57270, 73537, 1, 1, 0, 1, 1),
(57270, 73538, 1, 1, 0, 1, 1),
(57270, 73539, 1, 1, 0, 1, 1),
(57270, 73542, 0, 1, 4, 1, 1),
(57270, 73544, 0, 1, 1, 1, 1),
(57270, 73547, 0, 1, 2, 1, 1),
(57270, 73549, 0, 1, 3, 1, 1),
(57270, 73550, 0, 1, 4, 1, 1),
(57270, 73551, 0, 1, 1, 1, 1),
(57270, 73552, 0, 1, 2, 1, 1),
(57270, 73553, 0, 1, 3, 1, 1),
(57270, 73554, 0, 1, 4, 1, 1),
(57270, 73555, 0, 1, 1, 1, 1),
(57270, 73557, 0, 1, 2, 1, 1),
(57270, 73559, 0, 1, 3, 1, 1),
(57270, 73561, 0, 1, 4, 1, 1),
(57270, 73562, 0, 1, 1, 1, 1),
(57270, 73563, 0, 1, 2, 1, 1),
(57270, 73564, 0, 1, 3, 1, 1),
(57270, 73565, 0, 1, 4, 1, 1),
(57270, 73566, 0, 1, 1, 1, 1),
(57270, 73568, 0, 1, 2, 1, 1),
(57270, 73570, 0, 1, 3, 1, 1),
(57270, 73574, 0, 1, 4, 1, 1),
(57270, 73576, 0, 1, 1, 1, 1),
(57270, 73581, 0, 1, 2, 1, 1),
(57270, 73583, 0, 1, 3, 1, 1),
(57270, 73585, 0, 1, 4, 1, 1),
(57270, 73586, 0, 1, 1, 1, 1),
(57270, 73587, 0, 1, 2, 1, 1),
(57270, 73588, 0, 1, 3, 1, 1),
(57270, 73589, 0, 1, 4, 1, 1),
(57270, 73590, 0, 1, 1, 1, 1),
(57270, 73591, 0, 1, 2, 1, 1),
(57270, 73592, 0, 1, 3, 1, 1),
(57270, 73593, 0, 1, 4, 1, 1),
(57270, 73597, 0, 1, 1, 1, 1),
(57270, 73599, 0, 1, 2, 1, 1),
(57270, 73600, 0, 1, 3, 1, 1),
(57270, 73601, 0, 1, 4, 1, 1),
(57270, 73602, 0, 1, 1, 1, 1),
(57270, 73605, 0, 1, 2, 1, 1),
(57270, 73607, 0, 1, 3, 1, 1),
(57270, 73608, 0, 1, 4, 1, 1),
(57270, 73609, 0, 1, 1, 1, 1),
(57270, 73610, 0, 1, 2, 1, 1),
(57270, 73613, 0, 1, 3, 1, 1),
(57270, 73615, 0, 1, 4, 1, 1),
(57270, 73617, 0, 1, 1, 1, 1),
(57270, 73619, 0, 1, 2, 1, 1),
(57270, 73621, 0, 1, 3, 1, 1),
(57270, 73622, 0, 1, 4, 1, 1),
(57270, 73623, 0, 1, 1, 1, 1),
(57270, 73625, 0, 1, 2, 1, 1),
(57270, 73626, 0, 1, 3, 1, 1),
(57270, 73627, 0, 1, 4, 1, 1),
(57270, 73628, 0, 1, 1, 1, 1),
(57270, 73629, 0, 1, 2, 1, 1),
(57270, 73630, 0, 1, 3, 1, 1),
(57270, 73631, 0, 1, 4, 1, 1),
(57270, 73632, 0, 1, 1, 1, 1),
(57270, 73633, 0, 1, 2, 1, 1),
(57270, 73634, 0, 1, 3, 1, 1),
(57270, 73635, 0, 1, 4, 1, 1),
(57270, 73636, 0, 1, 1, 1, 1),
(57270, 73637, 0, 1, 2, 1, 1),
(57270, 73638, 0, 1, 3, 1, 1),
(57270, 73639, 0, 1, 4, 1, 1),
(57270, 73640, 0, 1, 1, 1, 1),
(57270, 73641, 0, 1, 2, 1, 1),
(57270, 73643, 0, 1, 3, 1, 1),
(57270, 73644, 0, 1, 4, 1, 1),
(57270, 73645, 0, 1, 1, 1, 1),
(57270, 73646, 0, 1, 2, 1, 1),
(57270, 73647, 0, 1, 3, 1, 1),
(57270, 73648, 0, 1, 4, 1, 1),
(57270, 76212, 0, 1, 1, 1, 1),
(57270, 76214, 0, 1, 2, 1, 1),
(57270, 76341, 0, 1, 3, 1, 1),
(57270, 76343, 0, 1, 4, 1, 1),
(57270, 76346, 0, 1, 1, 1, 1),
(57270, 76348, 0, 1, 2, 1, 1),
(57270, 76357, 0, 1, 3, 1, 1),
(57270, 76359, 0, 1, 4, 1, 1),
(57270, 76749, 0, 1, 1, 1, 1),
(57270, 76751, 0, 1, 2, 1, 1),
(57270, 76757, 0, 1, 3, 1, 1),
(57270, 76759, 0, 1, 4, 1, 1),
(57270, 76766, 0, 1, 1, 1, 1),
(57270, 76768, 0, 1, 2, 1, 1),
(57270, 76875, 0, 1, 3, 1, 1),
(57270, 76877, 0, 1, 4, 1, 1),
(57270, 76975, 0, 1, 1, 1, 1),
(57270, 76977, 0, 1, 2, 1, 1),
(57270, 76985, 0, 1, 3, 1, 1),
(57270, 76986, 0, 1, 4, 1, 1),
(57270, 76989, 0, 1, 1, 1, 1),
(57270, 76991, 0, 1, 2, 1, 1),
(57270, 77004, 0, 1, 3, 1, 1),
(57270, 77006, 0, 1, 4, 1, 1),
(57270, 77009, 0, 1, 1, 1, 1),
(57270, 77011, 0, 1, 2, 1, 1),
(57270, 77014, 0, 1, 3, 1, 1),
(57270, 77016, 0, 1, 4, 1, 1),
(57270, 77018, 0, 1, 1, 1, 1),
(57270, 77020, 0, 1, 2, 1, 1),
(57270, 77024, 0, 1, 3, 1, 1),
(57270, 77026, 0, 1, 4, 1, 1),
(57270, 77029, 0, 1, 1, 1, 1),
(57270, 77031, 0, 1, 2, 1, 1),
(57270, 77036, 0, 1, 3, 1, 1),
(57270, 77038, 0, 1, 4, 1, 1),
(57270, 77041, 0, 1, 1, 1, 1),
(57270, 77043, 0, 1, 2, 1, 1);


DELETE FROM `creature_loot_template` WHERE (`entry`=47120);
INSERT INTO `creature_loot_template` VALUES 
(47120, 60245, 0, 1, 1, 1, 1),
(47120, 60247, 0, 1, 2, 1, 1),
(47120, 60248, 0, 1, 1, 1, 1),
(47120, 60250, 0, 1, 2, 1, 1),
(47120, 60255, 0, 1, 1, 1, 1),
(47120, 60257, 0, 1, 2, 1, 1),
(47120, 60261, 0, 1, 1, 1, 1),
(47120, 60275, 0, 1, 2, 1, 1),
(47120, 60278, 0, 1, 1, 1, 1),
(47120, 60280, 0, 1, 2, 1, 1),
(47120, 60283, 0, 1, 1, 1, 1),
(47120, 60285, 0, 1, 2, 1, 1),
(47120, 60288, 0, 1, 1, 1, 1),
(47120, 60290, 0, 1, 2, 1, 1),
(47120, 60298, 0, 1, 1, 1, 1),
(47120, 60300, 0, 1, 2, 1, 1),
(47120, 60305, 0, 1, 1, 1, 1),
(47120, 60307, 0, 1, 2, 1, 1),
(47120, 60310, 0, 1, 1, 1, 1),
(47120, 60312, 0, 1, 2, 1, 1),
(47120, 60314, 0, 1, 1, 1, 1),
(47120, 60316, 0, 1, 2, 1, 1),
(47120, 60319, 0, 1, 1, 1, 1),
(47120, 60321, 0, 1, 2, 1, 1),
(47120, 60324, 0, 1, 1, 1, 1),
(47120, 60326, 0, 1, 2, 1, 1),
(47120, 60330, 0, 1, 1, 1, 1),
(47120, 60332, 0, 1, 2, 1, 1),
(47120, 60340, 0, 1, 1, 1, 1),
(47120, 60342, 0, 1, 2, 1, 1),
(47120, 60345, 0, 1, 1, 1, 1),
(47120, 60347, 0, 1, 2, 1, 1),
(47120, 60350, 0, 1, 1, 1, 1),
(47120, 60352, 0, 1, 2, 1, 1),
(47120, 60355, 0, 1, 1, 1, 1),
(47120, 60357, 0, 1, 2, 1, 1),
(47120, 60361, 0, 1, 1, 1, 1),
(47120, 60363, 0, 1, 2, 1, 1),
(47120, 60409, 0, 1, 1, 1, 1),
(47120, 60411, 0, 1, 2, 1, 1),
(47120, 60414, 0, 1, 1, 1, 1),
(47120, 60416, 0, 1, 2, 1, 1),
(47120, 60419, 0, 1, 1, 1, 1),
(47120, 60421, 0, 1, 2, 1, 1),
(47120, 60424, 0, 1, 1, 1, 1),
(47120, 60426, 0, 1, 2, 1, 1),
(47120, 60429, 0, 1, 1, 1, 1),
(47120, 60431, 0, 1, 2, 1, 1),
(47120, 60434, 0, 1, 1, 1, 1),
(47120, 60436, 0, 1, 2, 1, 1),
(47120, 60439, 0, 1, 1, 1, 1),
(47120, 60441, 0, 1, 2, 1, 1),
(47120, 60443, 0, 1, 1, 1, 1),
(47120, 60445, 0, 1, 2, 1, 1),
(47120, 60448, 0, 1, 1, 1, 1),
(47120, 60450, 0, 1, 2, 1, 1),
(47120, 60453, 0, 1, 1, 1, 1),
(47120, 60455, 0, 1, 2, 1, 1),
(47120, 60459, 0, 1, 1, 1, 1),
(47120, 60461, 0, 1, 2, 1, 1),
(47120, 60463, 0, 1, 1, 1, 1),
(47120, 60465, 0, 1, 2, 1, 1),
(47120, 60468, 0, 1, 1, 1, 1),
(47120, 60470, 0, 1, 2, 1, 1),
(47120, 60473, 0, 1, 1, 1, 1),
(47120, 60475, 0, 1, 2, 1, 1),
(47120, 60478, 0, 1, 1, 1, 1),
(47120, 60480, 0, 1, 2, 1, 1),
(47120, 60505, 0, 1, 1, 1, 1),
(47120, 60508, 0, 1, 2, 1, 1),
(47120, 60509, 0, 1, 1, 1, 1),
(47120, 60513, 0, 1, 2, 1, 1),
(47120, 60516, 0, 1, 1, 1, 1),
(47120, 60521, 0, 1, 2, 1, 1),
(47120, 60533, 0, 1, 1, 1, 1),
(47120, 60534, 0, 1, 2, 1, 1),
(47120, 60536, 0, 1, 1, 1, 1),
(47120, 60539, 0, 1, 2, 1, 1),
(47120, 60540, 0, 1, 1, 1, 1),
(47120, 60554, 0, 1, 2, 1, 1),
(47120, 60555, 0, 1, 1, 1, 1),
(47120, 60557, 0, 1, 2, 1, 1),
(47120, 60564, 0, 1, 1, 1, 1),
(47120, 60567, 0, 1, 2, 1, 1),
(47120, 60580, 0, 1, 1, 1, 1),
(47120, 60581, 0, 1, 2, 1, 1),
(47120, 60583, 0, 1, 1, 1, 1),
(47120, 60586, 0, 1, 2, 1, 1),
(47120, 60587, 0, 1, 1, 1, 1),
(47120, 60589, 0, 1, 2, 1, 1),
(47120, 60593, 0, 1, 1, 1, 1),
(47120, 60602, 0, 1, 2, 1, 1),
(47120, 60604, 0, 1, 1, 1, 1),
(47120, 60607, 0, 1, 2, 1, 1),
(47120, 60612, 0, 1, 1, 1, 1),
(47120, 60613, 0, 1, 2, 1, 1),
(47120, 60626, 0, 1, 1, 1, 1),
(47120, 60628, 0, 1, 2, 1, 1),
(47120, 60630, 0, 1, 1, 1, 1),
(47120, 60634, 0, 1, 2, 1, 1),
(47120, 60636, 0, 1, 1, 1, 1),
(47120, 60637, 0, 1, 2, 1, 1),
(47120, 60645, 0, 1, 1, 1, 1),
(47120, 60647, 0, 1, 2, 1, 1),
(47120, 60649, 0, 1, 1, 1, 1),
(47120, 60650, 0, 1, 2, 1, 1),
(47120, 60651, 0, 1, 1, 1, 1),
(47120, 60658, 0, 1, 2, 1, 1),
(47120, 60659, 0, 1, 1, 1, 1),
(47120, 60661, 0, 1, 2, 1, 1),
(47120, 60662, 0, 1, 1, 1, 1),
(47120, 60664, 0, 1, 2, 1, 1),
(47120, 60668, 0, 1, 1, 1, 1),
(47120, 60669, 0, 1, 2, 1, 1),
(47120, 60670, 0, 1, 1, 1, 1),
(47120, 60673, 0, 1, 2, 1, 1),
(47120, 60776, 0, 1, 1, 1, 1),
(47120, 60778, 0, 1, 2, 1, 1),
(47120, 60779, 0, 1, 1, 1, 1),
(47120, 60783, 0, 1, 2, 1, 1),
(47120, 60786, 0, 1, 1, 1, 1),
(47120, 60787, 0, 1, 2, 1, 1),
(47120, 60788, 0, 1, 1, 1, 1),
(47120, 60794, 1, 1, 0, 1, 1),
(47120, 60799, 1, 1, 0, 1, 1),
(47120, 60800, 1, 1, 0, 1, 1),
(47120, 60801, 1, 1, 0, 1, 1),
(47120, 60806, 1, 1, 0, 1, 1),
(47120, 60807, 1, 1, 0, 1, 1),
(47120, 61026, 0, 1, 2, 1, 1),
(47120, 61031, 0, 1, 1, 1, 1),
(47120, 61032, 0, 1, 2, 1, 1),
(47120, 61033, 0, 1, 1, 1, 1),
(47120, 61047, 0, 1, 2, 1, 1);

DELETE FROM `creature_loot_template` WHERE (`entry`=51350);
INSERT INTO `creature_loot_template` VALUES 
(51350, 60245, 0, 1, 1, 1, 1),
(51350, 60247, 0, 1, 2, 1, 1),
(51350, 60248, 0, 1, 3, 1, 1),
(51350, 60250, 0, 1, 4, 1, 1),
(51350, 60255, 0, 1, 1, 1, 1),
(51350, 60257, 0, 1, 2, 1, 1),
(51350, 60261, 0, 1, 3, 1, 1),
(51350, 60275, 0, 1, 4, 1, 1),
(51350, 60278, 0, 1, 1, 1, 1),
(51350, 60280, 0, 1, 2, 1, 1),
(51350, 60283, 0, 1, 3, 1, 1),
(51350, 60285, 0, 1, 4, 1, 1),
(51350, 60288, 0, 1, 1, 1, 1),
(51350, 60290, 0, 1, 2, 1, 1),
(51350, 60298, 0, 1, 3, 1, 1),
(51350, 60300, 0, 1, 4, 1, 1),
(51350, 60305, 0, 1, 1, 1, 1),
(51350, 60307, 0, 1, 2, 1, 1),
(51350, 60310, 0, 1, 3, 1, 1),
(51350, 60312, 0, 1, 4, 1, 1),
(51350, 60314, 0, 1, 1, 1, 1),
(51350, 60316, 0, 1, 2, 1, 1),
(51350, 60319, 0, 1, 3, 1, 1),
(51350, 60321, 0, 1, 4, 1, 1),
(51350, 60324, 0, 1, 1, 1, 1),
(51350, 60326, 0, 1, 2, 1, 1),
(51350, 60330, 0, 1, 3, 1, 1),
(51350, 60332, 0, 1, 4, 1, 1),
(51350, 60340, 0, 1, 1, 1, 1),
(51350, 60342, 0, 1, 2, 1, 1),
(51350, 60345, 0, 1, 3, 1, 1),
(51350, 60347, 0, 1, 4, 1, 1),
(51350, 60350, 0, 1, 1, 1, 1),
(51350, 60352, 0, 1, 2, 1, 1),
(51350, 60355, 0, 1, 3, 1, 1),
(51350, 60357, 0, 1, 4, 1, 1),
(51350, 60361, 0, 1, 1, 1, 1),
(51350, 60363, 0, 1, 2, 1, 1),
(51350, 60409, 0, 1, 3, 1, 1),
(51350, 60411, 0, 1, 4, 1, 1),
(51350, 60414, 0, 1, 1, 1, 1),
(51350, 60416, 0, 1, 2, 1, 1),
(51350, 60419, 0, 1, 3, 1, 1),
(51350, 60421, 0, 1, 4, 1, 1),
(51350, 60424, 0, 1, 1, 1, 1),
(51350, 60426, 0, 1, 2, 1, 1),
(51350, 60429, 0, 1, 3, 1, 1),
(51350, 60431, 0, 1, 4, 1, 1),
(51350, 60434, 0, 1, 1, 1, 1),
(51350, 60436, 0, 1, 2, 1, 1),
(51350, 60439, 0, 1, 3, 1, 1),
(51350, 60441, 0, 1, 4, 1, 1),
(51350, 60443, 0, 1, 1, 1, 1),
(51350, 60445, 0, 1, 2, 1, 1),
(51350, 60448, 0, 1, 3, 1, 1),
(51350, 60450, 0, 1, 4, 1, 1),
(51350, 60453, 0, 1, 1, 1, 1),
(51350, 60455, 0, 1, 2, 1, 1),
(51350, 60459, 0, 1, 3, 1, 1),
(51350, 60461, 0, 1, 4, 1, 1),
(51350, 60463, 0, 1, 1, 1, 1),
(51350, 60465, 0, 1, 2, 1, 1),
(51350, 60468, 0, 1, 3, 1, 1),
(51350, 60470, 0, 1, 4, 1, 1),
(51350, 60473, 0, 1, 1, 1, 1),
(51350, 60475, 0, 1, 2, 1, 1),
(51350, 60478, 0, 1, 3, 1, 1),
(51350, 60480, 0, 1, 4, 1, 1),
(51350, 60505, 0, 1, 1, 1, 1),
(51350, 60508, 0, 1, 2, 1, 1),
(51350, 60509, 0, 1, 3, 1, 1),
(51350, 60513, 0, 1, 4, 1, 1),
(51350, 60516, 0, 1, 1, 1, 1),
(51350, 60521, 0, 1, 2, 1, 1),
(51350, 60533, 0, 1, 3, 1, 1),
(51350, 60534, 0, 1, 4, 1, 1),
(51350, 60536, 0, 1, 1, 1, 1),
(51350, 60539, 0, 1, 2, 1, 1),
(51350, 60540, 0, 1, 3, 1, 1),
(51350, 60554, 0, 1, 4, 1, 1),
(51350, 60555, 0, 1, 1, 1, 1),
(51350, 60557, 0, 1, 2, 1, 1),
(51350, 60564, 0, 1, 3, 1, 1),
(51350, 60567, 0, 1, 4, 1, 1),
(51350, 60580, 0, 1, 1, 1, 1),
(51350, 60581, 0, 1, 2, 1, 1),
(51350, 60583, 0, 1, 3, 1, 1),
(51350, 60586, 0, 1, 4, 1, 1),
(51350, 60587, 0, 1, 1, 1, 1),
(51350, 60589, 0, 1, 2, 1, 1),
(51350, 60593, 0, 1, 3, 1, 1),
(51350, 60602, 0, 1, 4, 1, 1),
(51350, 60604, 0, 1, 1, 1, 1),
(51350, 60607, 0, 1, 2, 1, 1),
(51350, 60612, 0, 1, 3, 1, 1),
(51350, 60613, 0, 1, 4, 1, 1),
(51350, 60626, 0, 1, 1, 1, 1),
(51350, 60628, 0, 1, 2, 1, 1),
(51350, 60630, 0, 1, 3, 1, 1),
(51350, 60634, 0, 1, 4, 1, 1),
(51350, 60636, 0, 1, 1, 1, 1),
(51350, 60637, 0, 1, 2, 1, 1),
(51350, 60645, 0, 1, 3, 1, 1),
(51350, 60647, 0, 1, 4, 1, 1),
(51350, 60649, 0, 1, 1, 1, 1),
(51350, 60650, 0, 1, 2, 1, 1),
(51350, 60651, 0, 1, 3, 1, 1),
(51350, 60658, 0, 1, 4, 1, 1),
(51350, 60659, 0, 1, 1, 1, 1),
(51350, 60661, 0, 1, 2, 1, 1),
(51350, 60662, 0, 1, 3, 1, 1),
(51350, 60664, 0, 1, 4, 1, 1),
(51350, 60668, 0, 1, 1, 1, 1),
(51350, 60669, 0, 1, 2, 1, 1),
(51350, 60670, 0, 1, 3, 1, 1),
(51350, 60673, 0, 1, 4, 1, 1),
(51350, 60776, 0, 1, 1, 1, 1),
(51350, 60778, 0, 1, 2, 1, 1),
(51350, 60779, 0, 1, 3, 1, 1),
(51350, 60783, 0, 1, 4, 1, 1),
(51350, 60786, 0, 1, 1, 1, 1),
(51350, 60787, 0, 1, 2, 1, 1),
(51350, 60788, 0, 1, 3, 1, 1),
(51350, 60794, 1, 1, 0, 1, 1),
(51350, 60799, 1, 1, 0, 1, 1),
(51350, 60800, 1, 1, 0, 1, 1),
(51350, 60801, 1, 1, 0, 1, 1),
(51350, 60806, 1, 1, 0, 1, 1),
(51350, 60807, 1, 1, 0, 1, 1),
(51350, 61026, 0, 1, 4, 1, 1),
(51350, 61031, 0, 1, 1, 1, 1),
(51350, 61032, 0, 1, 2, 1, 1),
(51350, 61033, 0, 1, 3, 1, 1),
(51350, 61047, 0, 1, 4, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (51350, 0, 0, 0, 0, 0, 35426, 0, 0, 0, 'Argaloth (1)', '', '', 0, 88, 88, 3, 16, 16, 0, 1.2, 1.71429, 1, 1, 548, 822, 0, 205, 125, 2000, 2000, 1, 0, 134219776, 0, 0, 0, 0, 0, 0, 438, 658, 164, 3, 108, 51350, 0, 0, 0, 0, 0, 0, 0, 0, 47008, 88954, 95173, 88972, 0, 0, 0, 0, 0, 0, 2961856, 2961856, '', 0, 3, 1, 750, 1, 1, 0, 0, 0, 0, 0, 0, 0, 164, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=52363);
INSERT INTO `creature_loot_template` VALUES 
(52363, 70245, 0, 1, 1, 1, 1),
(52363, 70247, 0, 1, 2, 1, 1),
(52363, 70250, 0, 1, 1, 1, 1),
(52363, 70252, 0, 1, 2, 1, 1),
(52363, 70255, 0, 1, 1, 1, 1),
(52363, 70257, 0, 1, 2, 1, 1),
(52363, 70260, 0, 1, 1, 1, 1),
(52363, 70262, 0, 1, 2, 1, 1),
(52363, 70265, 0, 1, 1, 1, 1),
(52363, 70267, 0, 1, 2, 1, 1),
(52363, 70270, 0, 1, 1, 1, 1),
(52363, 70272, 0, 1, 2, 1, 1),
(52363, 70275, 0, 1, 1, 1, 1),
(52363, 70277, 0, 1, 2, 1, 1),
(52363, 70279, 0, 1, 1, 1, 1),
(52363, 70281, 0, 1, 2, 1, 1),
(52363, 70284, 0, 1, 1, 1, 1),
(52363, 70286, 0, 1, 2, 1, 1),
(52363, 70289, 0, 1, 1, 1, 1),
(52363, 70291, 0, 1, 2, 1, 1),
(52363, 70295, 0, 1, 1, 1, 1),
(52363, 70297, 0, 1, 2, 1, 1),
(52363, 70299, 0, 1, 1, 1, 1),
(52363, 70301, 0, 1, 2, 1, 1),
(52363, 70304, 0, 1, 1, 1, 1),
(52363, 70306, 0, 1, 2, 1, 1),
(52363, 70309, 0, 1, 1, 1, 1),
(52363, 70311, 0, 1, 2, 1, 1),
(52363, 70314, 0, 1, 1, 1, 1),
(52363, 70316, 0, 1, 2, 1, 1),
(52363, 70319, 0, 1, 1, 1, 1),
(52363, 70320, 0, 1, 2, 1, 1),
(52363, 70321, 0, 1, 1, 1, 1),
(52363, 70322, 0, 1, 2, 1, 1),
(52363, 70323, 0, 1, 1, 1, 1),
(52363, 70324, 0, 1, 2, 1, 1),
(52363, 70325, 0, 1, 1, 1, 1),
(52363, 70326, 0, 1, 2, 1, 1),
(52363, 70327, 0, 1, 1, 1, 1),
(52363, 70328, 0, 1, 2, 1, 1),
(52363, 70329, 0, 1, 1, 1, 1),
(52363, 70330, 0, 1, 2, 1, 1),
(52363, 70331, 0, 1, 1, 1, 1),
(52363, 70332, 0, 1, 2, 1, 1),
(52363, 70333, 0, 1, 1, 1, 1),
(52363, 70334, 0, 1, 2, 1, 1),
(52363, 70335, 0, 1, 1, 1, 1),
(52363, 70336, 0, 1, 2, 1, 1),
(52363, 70337, 0, 1, 1, 1, 1),
(52363, 70338, 0, 1, 2, 1, 1),
(52363, 70339, 0, 1, 1, 1, 1),
(52363, 70340, 0, 1, 2, 1, 1),
(52363, 70341, 0, 1, 1, 1, 1),
(52363, 70342, 0, 1, 2, 1, 1),
(52363, 70343, 0, 1, 1, 1, 1),
(52363, 70344, 0, 1, 2, 1, 1),
(52363, 70345, 0, 1, 1, 1, 1),
(52363, 70346, 0, 1, 2, 1, 1),
(52363, 70347, 0, 1, 1, 1, 1),
(52363, 70348, 0, 1, 2, 1, 1),
(52363, 70349, 0, 1, 1, 1, 1),
(52363, 70350, 0, 1, 2, 1, 1),
(52363, 70351, 0, 1, 1, 1, 1),
(52363, 70352, 0, 1, 2, 1, 1),
(52363, 70354, 0, 1, 1, 1, 1),
(52363, 70356, 0, 1, 2, 1, 1),
(52363, 70358, 0, 1, 1, 1, 1),
(52363, 70359, 0, 1, 2, 1, 1),
(52363, 70360, 0, 1, 1, 1, 1),
(52363, 70361, 0, 1, 2, 1, 1),
(52363, 70362, 0, 1, 1, 1, 1),
(52363, 70363, 0, 1, 2, 1, 1),
(52363, 70364, 0, 1, 1, 1, 1),
(52363, 70365, 0, 1, 2, 1, 1),
(52363, 70366, 0, 1, 1, 1, 1),
(52363, 70367, 0, 1, 2, 1, 1),
(52363, 70368, 0, 1, 1, 1, 1),
(52363, 70369, 0, 1, 2, 1, 1),
(52363, 70370, 0, 1, 1, 1, 1),
(52363, 70371, 0, 1, 2, 1, 1),
(52363, 70372, 0, 1, 1, 1, 1),
(52363, 70373, 0, 1, 2, 1, 1),
(52363, 70374, 0, 1, 1, 1, 1),
(52363, 70375, 0, 1, 2, 1, 1),
(52363, 70376, 0, 1, 1, 1, 1),
(52363, 70377, 0, 1, 2, 1, 1),
(52363, 70378, 0, 1, 1, 1, 1),
(52363, 70379, 0, 1, 2, 1, 1),
(52363, 70380, 0, 1, 1, 1, 1),
(52363, 70381, 0, 1, 2, 1, 1),
(52363, 70382, 0, 1, 1, 1, 1),
(52363, 70383, 0, 1, 2, 1, 1),
(52363, 70384, 0, 1, 1, 1, 1),
(52363, 70385, 0, 1, 2, 1, 1),
(52363, 70386, 0, 1, 1, 1, 1),
(52363, 70387, 0, 1, 2, 1, 1),
(52363, 70388, 0, 1, 1, 1, 1),
(52363, 70389, 0, 1, 2, 1, 1),
(52363, 70390, 1, 1, 0, 1, 1),
(52363, 70391, 1, 1, 0, 1, 1),
(52363, 70392, 1, 1, 0, 1, 1),
(52363, 70393, 1, 1, 0, 1, 1),
(52363, 70394, 1, 1, 0, 1, 1),
(52363, 70395, 1, 1, 0, 1, 1),
(52363, 70396, 0, 1, 1, 1, 1),
(52363, 70397, 0, 1, 2, 1, 1),
(52363, 70398, 0, 1, 1, 1, 1),
(52363, 70399, 0, 1, 2, 1, 1),
(52363, 70400, 0, 1, 1, 1, 1),
(52363, 70401, 0, 1, 2, 1, 1),
(52363, 70402, 0, 1, 1, 1, 1),
(52363, 70403, 0, 1, 2, 1, 1),
(52363, 70404, 0, 1, 1, 1, 1),
(52363, 70942, 0, 1, 2, 1, 1),
(52363, 70943, 0, 1, 1, 1, 1),
(52363, 70947, 0, 1, 2, 1, 1),
(52363, 70949, 0, 1, 1, 1, 1),
(52363, 70952, 0, 1, 2, 1, 1),
(52363, 70953, 0, 1, 1, 1, 1),
(52363, 71046, 0, 1, 2, 1, 1),
(52363, 71048, 0, 1, 1, 1, 1),
(52363, 71050, 0, 1, 2, 1, 1),
(52363, 71052, 0, 1, 1, 1, 1),
(52363, 71059, 0, 1, 2, 1, 1),
(52363, 71061, 0, 1, 1, 1, 1),
(52363, 71064, 0, 1, 2, 1, 1),
(52363, 71066, 0, 1, 1, 1, 1),
(52363, 71069, 0, 1, 2, 1, 1),
(52363, 71071, 0, 1, 1, 1, 1),
(52363, 71092, 0, 1, 2, 1, 1),
(52363, 71094, 0, 1, 1, 1, 1),
(52363, 71097, 0, 1, 2, 1, 1),
(52363, 71099, 0, 1, 1, 1, 1),
(52363, 71102, 0, 1, 2, 1, 1),
(52363, 71104, 0, 1, 1, 1, 1),
(52363, 71107, 0, 1, 2, 1, 1),
(52363, 71109, 0, 1, 1, 1, 1),
(52363, 71271, 0, 1, 2, 1, 1),
(52363, 71273, 0, 1, 1, 1, 1),
(52363, 71276, 0, 1, 2, 1, 1),
(52363, 71278, 0, 1, 1, 1, 1),
(52363, 71281, 0, 1, 2, 1, 1),
(52363, 71283, 0, 1, 1, 1, 1),
(52363, 71286, 0, 1, 2, 1, 1),
(52363, 71288, 0, 1, 1, 1, 1),
(52363, 71292, 0, 1, 2, 1, 1),
(52363, 71294, 0, 1, 1, 1, 1),
(52363, 71297, 0, 1, 2, 1, 1),
(52363, 71299, 0, 1, 1, 1, 1),
(52363, 71302, 0, 1, 2, 1, 1),
(52363, 71304, 0, 1, 1, 1, 1);

DELETE FROM `skinning_loot_template` WHERE (`entry`=52363);
INSERT INTO `skinning_loot_template` VALUES 
(52363, 52976, 100, 1, 0, 1, 1);

DELETE FROM `creature_loot_template` WHERE (`entry`=54229);
INSERT INTO `creature_loot_template` VALUES 
(54229, 70245, 0, 1, 1, 1, 1),
(54229, 70247, 0, 1, 2, 1, 1),
(54229, 70250, 0, 1, 3, 1, 1),
(54229, 70252, 0, 1, 4, 1, 1),
(54229, 70255, 0, 1, 1, 1, 1),
(54229, 70257, 0, 1, 2, 1, 1),
(54229, 70260, 0, 1, 3, 1, 1),
(54229, 70262, 0, 1, 4, 1, 1),
(54229, 70265, 0, 1, 1, 1, 1),
(54229, 70267, 0, 1, 2, 1, 1),
(54229, 70270, 0, 1, 3, 1, 1),
(54229, 70272, 0, 1, 4, 1, 1),
(54229, 70275, 0, 1, 1, 1, 1),
(54229, 70277, 0, 1, 2, 1, 1),
(54229, 70279, 0, 1, 3, 1, 1),
(54229, 70281, 0, 1, 4, 1, 1),
(54229, 70284, 0, 1, 1, 1, 1),
(54229, 70286, 0, 1, 2, 1, 1),
(54229, 70289, 0, 1, 3, 1, 1),
(54229, 70291, 0, 1, 4, 1, 1),
(54229, 70295, 0, 1, 1, 1, 1),
(54229, 70297, 0, 1, 2, 1, 1),
(54229, 70299, 0, 1, 3, 1, 1),
(54229, 70301, 0, 1, 4, 1, 1),
(54229, 70304, 0, 1, 1, 1, 1),
(54229, 70306, 0, 1, 2, 1, 1),
(54229, 70309, 0, 1, 3, 1, 1),
(54229, 70311, 0, 1, 4, 1, 1),
(54229, 70314, 0, 1, 1, 1, 1),
(54229, 70316, 0, 1, 2, 1, 1),
(54229, 70319, 0, 1, 3, 1, 1),
(54229, 70320, 0, 1, 4, 1, 1),
(54229, 70321, 0, 1, 1, 1, 1),
(54229, 70322, 0, 1, 2, 1, 1),
(54229, 70323, 0, 1, 3, 1, 1),
(54229, 70324, 0, 1, 4, 1, 1),
(54229, 70325, 0, 1, 1, 1, 1),
(54229, 70326, 0, 1, 2, 1, 1),
(54229, 70327, 0, 1, 3, 1, 1),
(54229, 70328, 0, 1, 4, 1, 1),
(54229, 70329, 0, 1, 1, 1, 1),
(54229, 70330, 0, 1, 2, 1, 1),
(54229, 70331, 0, 1, 3, 1, 1),
(54229, 70332, 0, 1, 4, 1, 1),
(54229, 70333, 0, 1, 1, 1, 1),
(54229, 70334, 0, 1, 2, 1, 1),
(54229, 70335, 0, 1, 3, 1, 1),
(54229, 70336, 0, 1, 4, 1, 1),
(54229, 70337, 0, 1, 1, 1, 1),
(54229, 70338, 0, 1, 2, 1, 1),
(54229, 70339, 0, 1, 3, 1, 1),
(54229, 70340, 0, 1, 4, 1, 1),
(54229, 70341, 0, 1, 1, 1, 1),
(54229, 70342, 0, 1, 2, 1, 1),
(54229, 70343, 0, 1, 3, 1, 1),
(54229, 70344, 0, 1, 4, 1, 1),
(54229, 70345, 0, 1, 1, 1, 1),
(54229, 70346, 0, 1, 2, 1, 1),
(54229, 70347, 0, 1, 3, 1, 1),
(54229, 70348, 0, 1, 4, 1, 1),
(54229, 70349, 0, 1, 1, 1, 1),
(54229, 70350, 0, 1, 2, 1, 1),
(54229, 70351, 0, 1, 3, 1, 1),
(54229, 70352, 0, 1, 4, 1, 1),
(54229, 70354, 0, 1, 1, 1, 1),
(54229, 70356, 0, 1, 2, 1, 1),
(54229, 70358, 0, 1, 3, 1, 1),
(54229, 70359, 0, 1, 4, 1, 1),
(54229, 70360, 0, 1, 2, 1, 1),
(54229, 70361, 0, 1, 3, 1, 1),
(54229, 70362, 0, 1, 4, 1, 1),
(54229, 70363, 0, 1, 1, 1, 1),
(54229, 70364, 0, 1, 2, 1, 1),
(54229, 70365, 0, 1, 3, 1, 1),
(54229, 70366, 0, 1, 4, 1, 1),
(54229, 70367, 0, 1, 1, 1, 1),
(54229, 70368, 0, 1, 2, 1, 1),
(54229, 70369, 0, 1, 3, 1, 1),
(54229, 70370, 0, 1, 4, 1, 1),
(54229, 70371, 0, 1, 1, 1, 1),
(54229, 70372, 0, 1, 2, 1, 1),
(54229, 70373, 0, 1, 3, 1, 1),
(54229, 70374, 0, 1, 4, 1, 1),
(54229, 70375, 0, 1, 1, 1, 1),
(54229, 70376, 0, 1, 2, 1, 1),
(54229, 70377, 0, 1, 3, 1, 1),
(54229, 70378, 0, 1, 4, 1, 1),
(54229, 70379, 0, 1, 1, 1, 1),
(54229, 70380, 0, 1, 2, 1, 1),
(54229, 70381, 0, 1, 3, 1, 1),
(54229, 70382, 0, 1, 4, 1, 1),
(54229, 70383, 0, 1, 1, 1, 1),
(54229, 70384, 0, 1, 2, 1, 1),
(54229, 70385, 0, 1, 3, 1, 1),
(54229, 70386, 0, 1, 4, 1, 1),
(54229, 70387, 0, 1, 1, 1, 1),
(54229, 70388, 0, 1, 2, 1, 1),
(54229, 70389, 0, 1, 3, 1, 1),
(54229, 70390, 1, 1, 0, 1, 1),
(54229, 70391, 1, 1, 0, 1, 1),
(54229, 70392, 1, 1, 0, 1, 1),
(54229, 70393, 1, 1, 0, 1, 1),
(54229, 70394, 1, 1, 0, 1, 1),
(54229, 70395, 1, 1, 0, 1, 1),
(54229, 70396, 0, 1, 4, 1, 1),
(54229, 70397, 0, 1, 1, 1, 1),
(54229, 70398, 0, 1, 2, 1, 1),
(54229, 70399, 0, 1, 3, 1, 1),
(54229, 70400, 0, 1, 4, 1, 1),
(54229, 70401, 0, 1, 1, 1, 1),
(54229, 70402, 0, 1, 2, 1, 1),
(54229, 70403, 0, 1, 3, 1, 1),
(54229, 70404, 0, 1, 4, 1, 1),
(54229, 70942, 0, 1, 1, 1, 1),
(54229, 70943, 0, 1, 2, 1, 1),
(54229, 70947, 0, 1, 3, 1, 1),
(54229, 70949, 0, 1, 4, 1, 1),
(54229, 70952, 0, 1, 1, 1, 1),
(54229, 70953, 0, 1, 2, 1, 1),
(54229, 71046, 0, 1, 3, 1, 1),
(54229, 71048, 0, 1, 4, 1, 1),
(54229, 71050, 0, 1, 1, 1, 1),
(54229, 71052, 0, 1, 2, 1, 1),
(54229, 71059, 0, 1, 3, 1, 1),
(54229, 71061, 0, 1, 4, 1, 1),
(54229, 71064, 0, 1, 1, 1, 1),
(54229, 71066, 0, 1, 2, 1, 1),
(54229, 71069, 0, 1, 3, 1, 1),
(54229, 71071, 0, 1, 4, 1, 1),
(54229, 71092, 0, 1, 1, 1, 1),
(54229, 71094, 0, 1, 2, 1, 1),
(54229, 71097, 0, 1, 3, 1, 1),
(54229, 71099, 0, 1, 4, 1, 1),
(54229, 71102, 0, 1, 1, 1, 1),
(54229, 71104, 0, 1, 2, 1, 1),
(54229, 71107, 0, 1, 3, 1, 1),
(54229, 71109, 0, 1, 4, 1, 1),
(54229, 71271, 0, 1, 1, 1, 1),
(54229, 71273, 0, 1, 2, 1, 1),
(54229, 71276, 0, 1, 3, 1, 1),
(54229, 71278, 0, 1, 4, 1, 1),
(54229, 71281, 0, 1, 1, 1, 1),
(54229, 71283, 0, 1, 2, 1, 1),
(54229, 71286, 0, 1, 3, 1, 1),
(54229, 71288, 0, 1, 4, 1, 1),
(54229, 71292, 0, 1, 1, 1, 1),
(54229, 71294, 0, 1, 2, 1, 1),
(54229, 71297, 0, 1, 3, 1, 1),
(54229, 71299, 0, 1, 4, 1, 1),
(54229, 71302, 0, 1, 1, 1, 1),
(54229, 71304, 0, 1, 2, 1, 1);
