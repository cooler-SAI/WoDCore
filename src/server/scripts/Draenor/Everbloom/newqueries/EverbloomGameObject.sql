/*
SQLyog Community v11.51 (64 bit)
MySQL - 5.5.44 
*********************************************************************
*/
/*!40101 SET NAMES utf8 */;

create table `gameobject` (
	`id` mediumint (8),
	`map` smallint (5),
	`zoneId` smallint (8),
	`areaId` smallint (8),
	`spawnMask` int (8),
	`phaseMask` int (10),
	`position_x` float ,
	`position_y` float ,
	`position_z` float ,
	`orientation` float ,
	`rotation0` float ,
	`rotation1` float ,
	`rotation2` float ,
	`rotation3` float ,
	`spawntimesecs` int (11),
	`animprogress` tinyint (3),
	`state` tinyint (3),
	`isActive` tinyint (1),
	`custom_flags` int (10),
	`protect_anti_doublet` int (255)
); 
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('231970','1279','7109','7384','2','4294967295','616.84','1663.34','128.961','4.71718','0','0','0.705411','-0.708798','300','0','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237483','1279','7109','7109','6','1','746.571','1567.3','108.295','0.0687468','0','0','0','0','0','255','1','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237473','1279','7109','7109','6','1','326.981','1490.24','102.041','0.623439','0','0','0','0','0','255','1','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237472','1279','7109','7383','6','1','762.92','1442.21','88.4421','5.07356','0','0','0','0','0','255','1','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237472','1279','7109','7109','6','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('236462','1279','7109','7109','6','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('235355','1279','7109','7109','6','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('234121','1279','7109','7109','6','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('234113','1279','7109','7109','6','4294967295','913.547','1446.23','64.8723','5.58572','0','0','0.341709','-0.939806','300','0','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('234113','1279','7109','7109','6','4294967295','942.827','1450.04','65.2701','5.44513','0','0','0.406874','-0.913484','300','0','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('234025','1279','7109','7384','6','1','623.432','1734.33','144.16','4.75602','0','0','0','0','0','255','1','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('234025','1279','7109','7109','6','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('232552','1279','7109','7109','6','1','420.661','1308.97','125.413','1.07078','0','0','0','0','0','255','1','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('232552','1279','7109','7109','6','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('231970','1279','7109','7384','2','4294967295','617.977','1659.75','128.449','4.48393','0','0','0.783095','-0.621902','300','0','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('232552','1279','7109','7109','6','1','420.661','1308.97','125.413','1.07078','0','0','0','0','0','255','1','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('234025','1279','7109','7109','6','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('234025','1279','7109','7384','6','1','623.432','1734.33','144.16','4.75602','0','0','0','0','0','255','1','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('234113','1279','7109','7109','6','4294967295','926.61','1415.67','66.1778','5.02023','0','0','0.59034','-0.807155','300','0','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('231970','1279','7109','7384','2','4294967295','645.338','1657.4','128.797','1.67689','0','0','0.743605','0.66862','300','0','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('234121','1279','7109','7109','6','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('235355','1279','7109','7109','6','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('236462','1279','7109','7109','6','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237472','1279','7109','7109','6','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237472','1279','7109','7383','6','1','762.92','1442.21','88.4421','5.07356','0','0','0','0','0','255','1','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('231970','1279','7109','7384','2','4294967295','666.651','1659.52','131.439','1.64469','0','0','0.732743','0.680505','300','0','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237473','1279','7109','7109','6','1','326.981','1490.24','102.041','0.623439','0','0','0','0','0','255','1','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237483','1279','7109','7109','6','1','746.571','1567.3','108.295','0.0687468','0','0','0','0','0','255','1','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('232552','1279','7109','7109','6','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('201517','1279','7109','7109','6','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('201517','1279','7109','7109','6','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('234025','1279','7109','7330','6','1','874.013','-1221.43','197.278','3.25828','0','0','0.998299','-0.0583095','300','0','1','0','0','0');
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('195485','1279','7109','7109','6','1','899.355','1472.92','68.0272','5.37887','0','0','0.436906','-0.899507','300','0','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('231970','1279','7109','7384','2','4294967295','641.826','1664.44','130.373','4.58366','0','0','0.751123','-0.660162','300','0','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('231970','1279','7109','7384','2','4294967295','637.326','1664.02','129.051','4.61664','0','0','0.740136','-0.672457','300','0','1','0','0',NULL);
