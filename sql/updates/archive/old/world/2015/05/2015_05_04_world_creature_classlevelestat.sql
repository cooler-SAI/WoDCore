/*
SQLyog Ultimate v11.11 (64 bit)
MySQL - 5.7.5-m15-log : Database - world
*********************************************************************
*/


/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

/*Table structure for table `creature_classlevelstats` */

DROP TABLE IF EXISTS `creature_classlevelstats`;

CREATE TABLE `creature_classlevelstats` (
  `level` tinyint(4) NOT NULL,
  `class` tinyint(4) NOT NULL,
  `basehp0` mediumint(8) unsigned NOT NULL DEFAULT '1',
  `basehp1` mediumint(8) unsigned NOT NULL DEFAULT '1',
  `basehp2` mediumint(8) unsigned NOT NULL DEFAULT '1',
  `basehp3` mediumint(8) unsigned NOT NULL DEFAULT '1',
  `basehp4` mediumint(8) unsigned NOT NULL DEFAULT '1',
  `basehp5` mediumint(8) unsigned NOT NULL DEFAULT '1',
  `basemana` mediumint(8) unsigned NOT NULL DEFAULT '1',
  `basearmor` mediumint(8) unsigned NOT NULL DEFAULT '1',
  `attackpower` smallint(6) NOT NULL DEFAULT '0',
  `rangedattackpower` smallint(6) NOT NULL DEFAULT '0',
  `damage_base` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`level`,`class`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `creature_classlevelstats` */

insert  into `creature_classlevelstats`(`level`,`class`,`basehp0`,`basehp1`,`basehp2`,`basehp3`,`basehp4`,`basehp5`,`basemana`,`basearmor`,`attackpower`,`rangedattackpower`,`damage_base`) values
(1,1,40,40,40,40,40,40,0,0,10,1,0.5),
(2,1,50,50,50,50,50,50,0,0,10,1,1),
(3,1,55,55,55,55,55,55,0,0,11,1,1),
(4,1,60,60,60,60,60,60,0,0,13,1,2),
(5,1,75,75,75,75,75,75,0,25,17,1,3),
(6,1,90,90,90,90,90,90,0,35,19,1,4),
(7,1,100,100,100,100,100,100,0,48,21,1,4.5),
(8,1,110,110,110,110,110,110,0,62,24,1,5),
(9,1,120,120,120,120,120,120,0,87,28,1,6),
(10,1,141,141,141,141,141,141,0,97,32,1,6.5),
(11,1,261,261,261,261,261,261,0,109,38,1,7),
(12,1,312,312,312,312,312,312,0,114,40,1,7.5),
(13,1,321,321,321,321,321,321,0,118,44,1,8),
(14,1,375,375,375,375,375,375,0,122,50,1,8.5),
(15,1,384,384,384,384,384,384,0,126,54,2,10),
(16,1,432,432,432,432,432,432,0,131,56,2,12),
(17,1,441,441,441,441,441,441,0,136,60,2,13),
(18,1,488,488,488,488,488,488,0,139,64,3,13.5),
(19,1,533,533,533,533,533,533,0,143,68,3,14),
(20,1,547,547,547,547,547,547,0,148,70,4,17),
(21,1,593,593,593,593,593,593,0,152,74,4,19),
(22,1,603,603,603,603,603,603,0,156,78,4,25),
(23,1,672,672,672,672,672,672,0,160,80,5,30),
(24,1,695,695,695,695,695,695,0,165,84,6,33),
(25,1,788,788,788,788,788,788,0,169,86,6,34),
(26,1,804,804,804,804,804,804,0,173,90,7,36),
(27,1,841,841,841,841,841,841,0,177,92,7,37.5),
(28,1,867,867,867,867,867,867,0,182,96,8,38),
(29,1,928,928,928,928,928,928,0,186,100,8,39),
(30,1,950,950,950,950,950,950,0,190,102,9,45),
(31,1,1049,1049,1049,1049,1049,1049,0,194,106,9,52),
(32,1,1067,1067,1067,1067,1067,1067,0,199,108,10,53),
(33,1,1145,1145,1145,1145,1145,1145,0,203,112,10,55),
(34,1,1183,1183,1183,1183,1183,1183,0,207,114,11,60),
(35,1,1245,1245,1245,1245,1245,1245,0,211,118,11,65),
(36,1,1270,1270,1270,1270,1270,1270,0,216,120,12,66),
(37,1,1333,1333,1333,1333,1333,1333,0,220,124,12,67),
(38,1,1358,1358,1358,1358,1358,1358,0,224,128,13,68),
(39,1,1447,1447,1447,1447,1447,1447,0,228,132,13,69),
(40,1,1474,1474,1474,1474,1474,1474,0,233,136,14,80),
(41,1,1589,1589,1589,1589,1589,1589,0,237,142,15,82),
(42,1,1612,1612,1612,1612,1612,1612,0,241,152,15,84),
(43,1,1660,1660,1660,1660,1660,1660,0,245,162,16,88),
(44,1,1688,1688,1688,1688,1688,1688,0,250,174,16,90),
(45,1,1793,1793,1793,1793,1793,1793,0,254,184,17,92),
(46,1,1833,1833,1833,1833,1833,1833,0,258,188,17,96),
(47,1,1950,1950,1950,1950,1950,1950,0,263,192,18,98),
(48,1,1976,1976,1976,1976,1976,1976,0,267,196,19,100),
(49,1,2044,2044,2044,2044,2044,2044,0,271,200,19,115),
(50,1,2070,2070,2070,2070,2070,2070,0,275,206,20,120),
(51,1,2171,2171,2171,2171,2171,2171,0,279,210,20,125),
(52,1,2208,2208,2208,2208,2208,2208,0,283,214,21,130),
(53,1,2300,2300,2300,2300,2300,2300,0,287,218,22,135),
(54,1,2334,2334,2334,2334,2334,2334,0,291,224,22,140),
(55,1,2422,2422,2422,2422,2422,2422,0,297,228,23,145),
(56,1,2467,2467,2467,2467,2467,2467,0,301,234,23,148),
(57,1,2563,2563,2563,2563,2563,2563,0,305,238,24,150),
(58,1,2593,2593,2593,2593,2593,2593,0,309,242,25,152),
(59,1,2747,2747,2747,2747,2747,2747,0,314,248,25,155),
(60,1,2937,2937,2937,2937,2937,2937,0,318,252,26,160),
(61,1,3037,3037,3037,3037,3037,3037,0,322,258,28,180),
(62,1,3122,3122,3122,3122,3122,3122,0,326,262,30,184),
(63,1,3207,3207,3207,3207,3207,3207,0,331,268,32,188),
(64,1,3352,3352,3352,3352,3352,3352,0,335,272,33,190),
(65,1,3516,3516,3516,3516,3516,3516,0,339,278,35,202),
(66,1,3635,3635,3635,3635,3635,3635,0,343,282,37,209),
(67,1,3820,3820,3820,3820,3820,3820,0,348,288,39,219),
(68,1,3858,3858,3858,3858,3858,3858,0,352,292,41,220),
(69,1,3974,3974,3974,3974,3974,3974,0,356,298,43,227),
(70,1,4299,4299,4299,4299,4299,4299,0,360,304,44,245),
(71,1,4443,4443,4443,4443,4443,4443,0,365,308,48,254),
(72,1,4608,4608,4608,4608,4608,4608,0,369,314,53,264),
(73,1,4886,4886,4886,4886,4886,4886,0,373,320,58,276),
(74,1,4973,4973,4973,4973,4973,4973,0,377,354,63,282),
(75,1,5118,5118,5118,5118,5118,5118,0,381,392,68,290),
(76,1,5239,5239,5239,5239,5239,5239,0,385,432,74,306),
(77,1,5298,5298,5298,5298,5298,5298,0,390,478,81,324),
(78,1,5408,5408,5408,5408,5408,5408,0,394,528,88,334),
(79,1,5461,5461,5461,5461,5461,5461,0,399,582,95,336),
(80,1,5534,5534,5534,5534,5534,5534,0,403,642,103,338),
(81,1,6013,6013,6013,6013,6013,6013,0,405,708,112,406),
(82,1,6465,6465,6465,6465,6465,6465,0,411,782,121,412),
(83,1,7096,7096,7096,7096,7096,7096,0,415,805,135,419),
(84,1,7500,7500,7500,7500,7500,7500,0,420,832,148,420),
(85,1,7987,7987,7987,7987,7987,7987,0,424,867,156,432),
(86,1,8840,8840,8840,8840,8840,8840,0,428,879,169,438),
(87,1,9585,9585,9585,9585,9585,9585,0,432,896,184,445),
(88,1,10174,10174,10174,10174,10174,10174,0,440,910,193,459),
(89,1,10883,10883,10883,10883,10883,10883,0,444,945,208,478),
(90,1,11494,11494,11494,11494,11494,17503,0,455,967,229,525),
(91,1,11891,11891,11891,11891,11891,20475,0,504,983,243,797.5),
(92,1,12301,12301,12301,12301,12301,22939,0,571,997,265,1482.5),
(93,1,12725,12725,12725,12725,12725,26964,0,646,1024,279,1638.5),
(94,1,20481,20481,20481,20481,20481,31613,0,731,1059,302,1649.5),
(95,1,20481,20481,20481,20481,20481,37219,0,827,1085,312,1743.5),
(96,1,20481,20481,20481,20481,20481,45530,0,936,1135,325,1799.5),
(97,1,20481,20481,20481,20481,20481,55035,0,1059,1168,356,1802.5),
(98,1,20481,20481,20481,20481,20481,65312,0,1199,1203,379,2466.5),
(99,1,20481,20481,20481,20481,20481,79501,0,1275,1242,410,2639.5),
(100,1,20481,20481,20481,20481,20481,97854,0,1536,1350,440,2892.5),
(101,1,20481,20481,20481,20481,20481,101230,0,1670,1460,480,3122.5),
(102,1,20481,20481,20481,20481,20481,104718,0,1804,1520,510,3382.5),
(103,1,20481,20481,20481,20481,20481,108328,0,1938,1820,710,3738.5),
(105,1,20481,20481,20481,20481,20481,108328,12082,1938,1950,910,4000),
(1,2,40,40,40,40,40,40,60,0,1,1,0.5),
(2,2,50,50,50,50,50,50,69,0,3,1,1),
(3,2,55,55,55,55,55,55,79,0,7,1,1),
(4,2,60,60,60,60,60,60,104,0,18,1,2),
(5,2,75,75,75,75,75,75,115,25,19,1,3),
(6,2,90,90,90,90,90,90,126,35,24,1,4),
(7,2,100,100,100,100,100,100,138,48,28,1,4.5),
(8,2,110,110,110,110,110,110,165,62,32,1,5),
(9,2,120,120,120,120,120,120,178,87,36,1,6),
(10,2,141,141,141,141,141,141,191,97,38,1,6.5),
(11,2,261,261,261,261,261,261,205,109,40,1,7),
(12,2,312,312,312,312,312,312,249,114,42,1,7.5),
(13,2,321,321,321,321,321,321,264,118,44,1,8),
(14,2,375,375,375,375,375,375,295,122,46,1,8.5),
(15,2,384,384,384,384,384,384,326,126,48,2,10),
(16,2,432,432,432,432,432,432,357,131,52,2,12),
(17,2,441,441,441,441,441,441,390,136,56,2,13),
(18,2,488,488,488,488,488,488,408,139,58,3,13.5),
(19,2,533,533,533,533,533,533,456,143,60,3,14),
(20,2,547,547,547,547,547,547,490,148,64,4,17),
(21,2,593,593,593,593,593,593,510,152,68,4,19),
(22,2,603,603,603,603,603,603,545,156,72,5,25),
(23,2,672,672,672,672,672,672,581,160,74,5,30),
(24,2,695,695,695,695,695,695,618,165,78,6,33),
(25,2,788,788,788,788,788,788,655,169,80,6,34),
(26,2,804,804,804,804,804,804,693,173,84,6,36),
(27,2,841,841,841,841,841,841,732,177,86,7,37.5),
(28,2,867,867,867,867,867,867,756,182,90,7,38),
(29,2,928,928,928,928,928,928,811,186,94,8,39),
(30,2,950,950,950,950,950,950,852,190,94,8,45),
(31,2,1049,1049,1049,1049,1049,1049,878,194,98,9,52),
(32,2,1067,1067,1067,1067,1067,1067,935,199,100,9,53),
(33,2,1145,1145,1145,1145,1145,1145,963,203,104,10,55),
(34,2,1183,1183,1183,1183,1183,1183,1007,207,106,10,60),
(35,2,1245,1245,1245,1245,1245,1245,1067,211,110,11,65),
(36,2,1270,1270,1270,1270,1270,1270,1097,216,112,11,66),
(37,2,1333,1333,1093,1333,1333,1333,1142,220,116,11,67),
(38,2,1358,1358,1358,1358,1358,1358,1189,224,120,12,68),
(39,2,1447,1447,1447,1447,1447,1447,1236,228,124,12,69),
(40,2,1474,1474,1474,1474,1474,1474,1283,233,128,13,80),
(41,2,1589,1589,1589,1589,1589,1589,1332,237,132,13,82),
(42,2,1612,1612,1612,1612,1612,1612,1381,241,142,15,84),
(43,2,1660,1660,1660,1660,1660,1660,1432,245,152,14,88),
(44,2,1688,1688,1688,1688,1688,1688,1483,250,166,15,90),
(45,2,1793,1793,1793,1793,1793,1793,1534,254,172,15,92),
(46,2,1833,1833,1833,1833,1833,1833,1587,258,176,16,96),
(47,2,1950,1950,1950,1950,1950,1950,1640,263,180,16,98),
(48,2,1976,1976,1976,1976,1976,1976,1695,267,184,17,100),
(49,2,2044,2044,2044,2044,2044,2044,1750,271,188,17,115),
(50,2,2070,2070,2070,2070,2070,2070,1807,275,194,18,120),
(51,2,2171,2171,2171,2171,2171,2171,1864,279,198,19,125),
(52,2,2208,2208,2208,2208,2208,2208,1923,283,202,19,130),
(53,2,2300,2300,2300,2300,2300,2300,1982,287,206,20,135),
(54,2,2334,2334,2334,2334,2334,2334,2041,291,210,20,140),
(55,2,2422,2422,2422,2422,2422,2422,2117,297,214,21,145),
(56,2,2467,2467,2467,2467,2467,2467,2163,301,220,21,148),
(57,2,2563,2563,2563,2563,2563,2563,2241,305,224,22,150),
(58,2,2593,2593,2593,2593,2593,2593,2289,309,228,23,152),
(59,2,2747,2747,2747,2747,2747,2747,2369,314,234,23,155),
(60,2,2937,2937,2937,2937,2937,2937,2434,318,238,24,160),
(61,2,3037,3037,3037,3037,3037,3037,2486,322,244,25,180),
(62,2,3122,3122,3122,3122,3122,3122,2568,326,246,26,184),
(63,2,3207,3207,3207,3207,3207,3207,2620,331,252,26,188),
(64,2,3352,3352,3352,3352,3352,3352,2705,335,256,27,190),
(65,2,3516,3516,3516,3516,3516,3516,2790,339,262,29,202),
(66,2,3635,3635,3635,3635,3635,3635,2846,343,266,29,209),
(67,2,3820,3820,3820,3820,3820,3820,2933,348,272,31,219),
(68,2,3858,3858,3858,3858,3858,3858,2991,352,276,31,220),
(69,2,3974,3974,3974,3974,3974,3974,3080,356,282,32,227),
(70,2,4299,4299,4299,4299,4299,4299,3155,360,286,33,245),
(71,2,4443,4443,4443,4443,4443,4443,3231,365,290,37,254),
(72,2,4608,4608,4608,4608,4608,4608,3309,369,296,40,264),
(73,2,4886,4886,4886,4886,4886,4886,3387,373,302,44,276),
(74,2,4973,4973,4973,4973,4973,4973,3466,377,334,48,282),
(75,2,5118,5118,5118,5118,5118,5118,3561,381,370,53,290),
(76,2,5239,5239,5239,5239,5239,5239,3643,385,408,57,306),
(77,2,5298,5298,5298,5298,5298,5298,3725,390,452,62,324),
(78,2,5408,5408,5408,5408,5408,5408,3809,394,500,68,334),
(79,2,5461,5461,5461,5461,5461,5461,3893,399,550,74,336),
(80,2,5534,5534,5534,5534,5534,5534,8726,403,608,80,338),
(81,2,6013,6013,6013,6013,6013,6013,4081,405,670,86,406),
(82,2,6465,6465,6465,6465,6465,6465,6465,411,726,91,412),
(83,2,7096,7096,7096,7096,7096,7096,7096,415,783,98,419),
(84,2,7500,7500,7500,7500,7500,7500,8726,420,832,148,420),
(85,2,7987,7987,7987,7987,7987,7987,8905,424,867,156,432),
(86,2,8840,8840,8840,8840,8840,8840,9094,428,879,169,438),
(87,2,9585,9585,9585,9585,9585,9585,9310,432,896,184,445),
(88,2,10174,10174,10174,10174,10174,10174,9470,440,910,193,459),
(89,2,10883,10883,10883,10883,10883,10883,9692,444,945,208,478),
(90,2,11494,11494,11494,11494,11494,17503,9916,455,967,229,525),
(91,2,11891,11891,11891,11891,11891,20475,10084,504,983,243,797.5),
(92,2,12301,12301,12301,12301,12301,22939,10245,571,997,265,1482.5),
(93,2,12725,12725,12725,12725,12725,26964,10514,646,1024,279,1638.5),
(94,2,20481,20481,20481,20481,20481,31613,10748,731,1059,302,1649.5),
(95,2,20481,20481,20481,20481,20481,37219,10954,827,1085,312,1743.5),
(96,2,20481,20481,20481,20481,20481,45530,11164,936,1135,325,1799.5),
(97,2,20481,20481,20481,20481,20481,55035,11404,1059,1168,356,1802.5),
(98,2,20481,20481,20481,20481,20481,65312,11618,1199,1203,379,2466.5),
(99,2,20481,20481,20481,20481,20481,79501,11864,1275,1242,410,2639.5),
(100,2,20481,20481,20481,20481,20481,97854,12082,1536,1350,440,2892.5),
(101,2,20481,20481,20481,20481,20481,101230,12082,1670,1460,480,3122.5),
(102,2,20481,20481,20481,20481,20481,104718,12082,1804,1520,510,3382.5),
(103,2,20481,20481,20481,20481,20481,108328,12082,1938,1820,710,3738.5),
(1,4,40,40,40,40,40,40,0,0,24,1,0.5),
(2,4,50,50,50,50,50,50,0,0,26,1,1),
(3,4,55,55,55,55,55,55,0,0,28,1,1),
(4,4,60,60,60,60,60,60,0,0,30,1,2),
(5,4,75,75,75,75,75,75,0,25,32,1,3),
(6,4,90,90,90,90,90,90,0,35,34,1,4),
(7,4,100,100,100,100,100,100,0,48,36,1,4.5),
(8,4,110,110,110,110,110,110,0,62,38,1,5),
(9,4,120,120,120,120,120,120,0,87,40,1,6),
(10,4,141,141,141,141,141,141,0,97,42,1,6.5),
(11,4,261,261,261,261,261,261,0,109,44,1,7),
(12,4,312,312,312,312,312,312,0,114,46,1,7.5),
(13,4,321,321,321,321,321,321,0,118,48,1,8),
(14,4,375,375,375,375,375,375,0,122,50,1,8.5),
(15,4,384,384,384,384,384,384,0,126,54,2,10),
(16,4,432,432,432,432,432,432,0,131,56,2,12),
(17,4,441,441,441,441,441,441,0,136,60,2,13),
(18,4,488,488,488,488,488,488,0,139,64,3,13.5),
(19,4,533,533,533,533,533,533,0,143,68,3,14),
(20,4,547,547,547,547,547,547,0,148,70,4,17),
(21,4,593,593,593,593,593,593,0,152,74,4,19),
(22,4,603,603,603,603,603,603,0,156,78,4,25),
(23,4,672,672,672,672,672,672,0,160,80,5,30),
(24,4,695,695,695,695,695,695,0,165,84,6,33),
(25,4,788,788,788,788,788,788,0,169,86,6,34),
(26,4,804,804,804,804,804,804,0,173,90,7,36),
(27,4,841,841,841,841,841,841,0,177,92,7,37.5),
(28,4,867,867,867,867,867,867,0,182,96,8,38),
(29,4,928,928,928,928,928,928,0,186,100,8,39),
(30,4,950,950,950,950,950,950,0,190,102,9,45),
(31,4,1049,1049,1006,1006,1006,950,0,194,106,9,52),
(32,4,1067,1067,1067,1067,1067,1067,0,199,108,10,53),
(33,4,1145,1145,1145,1145,1145,1145,0,203,112,10,55),
(34,4,1183,1183,1183,1183,1183,1183,0,207,114,11,60),
(35,4,1245,1245,1245,1245,1245,1245,0,211,118,11,65),
(36,4,1270,1270,1270,1270,1270,1270,0,216,120,12,66),
(37,4,1333,1333,1333,1333,1333,1333,0,220,124,12,67),
(38,4,1358,1358,1358,1358,1358,1358,0,224,128,13,68),
(39,4,1447,1447,1447,1447,1447,1447,0,228,132,13,69),
(40,4,1474,1474,1474,1474,1474,1474,0,233,136,14,80),
(41,4,1589,1589,1589,1589,1589,1589,0,237,142,15,82),
(42,4,1612,1612,1612,1612,1612,1612,0,241,152,15,84),
(43,4,1660,1660,1660,1660,1660,1660,0,245,162,16,88),
(44,4,1688,1668,1668,1668,1668,1782,0,250,174,16,90),
(45,4,1793,1793,1793,1793,1793,1793,0,254,184,17,92),
(46,4,1833,1833,1833,1833,1833,1833,0,258,188,17,96),
(47,4,1950,1950,1950,1950,1950,1950,0,263,192,18,98),
(48,4,1976,1976,1976,1976,1976,1976,0,267,196,19,100),
(49,4,2044,2044,2044,2044,2044,2044,0,271,200,19,115),
(50,4,2070,2070,2070,2070,2070,2070,0,275,206,20,120),
(51,4,2171,2171,2171,2171,2171,2171,0,279,210,20,125),
(52,4,2208,2208,2208,2208,2208,2208,0,283,214,21,130),
(53,4,2300,2300,2300,2300,2300,2300,0,287,218,22,135),
(54,4,2334,2334,2334,2334,2334,2334,0,291,224,22,140),
(55,4,2422,2422,2422,2422,2422,2422,0,297,228,23,145),
(56,4,2467,2467,2467,2467,2467,2467,0,301,234,23,148),
(57,4,2563,2563,2563,2563,2563,2563,0,305,238,24,150),
(58,4,2593,2593,2593,2593,2593,2593,0,309,242,25,152),
(59,4,2747,2747,2747,2747,2747,2747,0,314,248,25,155),
(60,4,2937,2937,2937,2937,2937,2937,0,318,252,26,160),
(61,4,3037,3037,3037,3037,3037,3037,0,322,258,28,180),
(62,4,3122,3122,3122,3122,3122,3122,0,326,262,30,184),
(63,4,3207,3207,3207,3207,3207,3207,0,331,268,32,188),
(64,4,3352,3352,3352,3352,3352,3352,0,335,272,33,190),
(65,4,3516,3516,3516,3516,3516,3516,0,339,278,35,202),
(66,4,3635,3635,3635,3635,3635,3635,0,343,282,37,209),
(67,4,3820,3820,3820,3820,3820,3820,0,348,288,39,219),
(68,4,3858,3858,3858,3858,3858,3858,0,352,292,41,220),
(69,4,3974,3974,3974,3974,3974,3974,0,356,298,43,227),
(70,4,4299,4299,4299,4299,4299,4299,0,360,304,44,245),
(71,4,4443,4443,4443,4443,4443,4443,0,365,308,48,254),
(72,4,4608,4608,4608,4608,4608,4608,0,369,314,53,264),
(73,4,4973,4973,4973,4973,4973,4973,0,373,320,58,276),
(74,4,5118,5118,5118,5118,5118,5118,0,377,354,63,282),
(75,4,5239,5239,5239,5239,5239,5239,0,381,392,68,290),
(76,4,5298,5298,5298,5298,5298,5298,0,385,432,74,306),
(77,4,5408,5408,5408,5408,5408,5408,0,390,478,81,324),
(78,4,5461,5461,5461,5461,5461,5461,0,394,528,88,334),
(79,4,5534,5534,5534,5534,5534,5534,0,399,582,95,336),
(80,4,6013,6013,6013,6013,6013,6013,0,403,642,103,338),
(81,4,6465,6465,6465,6465,6465,6465,0,405,708,112,406),
(82,4,7096,7096,7096,7096,7096,7096,0,411,782,121,412),
(83,4,7500,7500,7500,7500,7500,7500,0,415,805,135,419),
(84,4,7987,7987,7987,7987,7987,7987,0,420,832,148,420),
(85,4,8840,8840,8840,8840,8840,8840,0,424,867,156,432),
(86,4,9585,9585,9585,9585,9585,9585,0,428,879,169,438),
(87,4,10174,10174,10174,10174,10174,10174,0,432,896,184,445),
(88,4,10883,10883,10883,10883,10883,10883,0,440,910,193,459),
(89,4,11494,11494,11494,11494,11494,17503,0,444,945,208,478),
(90,4,11891,11891,11891,11891,11891,20475,0,455,967,229,525),
(91,4,12301,12301,12301,12301,12301,22939,0,504,983,243,797.5),
(92,4,12725,12725,12725,12725,12725,26964,0,571,997,265,1482.5),
(93,4,20481,20481,20481,20481,20481,31613,0,646,1024,279,1638.5),
(94,4,20481,20481,20481,20481,20481,37219,0,731,1059,302,1649.5),
(95,4,20481,20481,20481,20481,20481,45530,0,827,1085,312,1743.5),
(96,4,20481,20481,20481,20481,20481,55035,0,936,1135,325,1799.5),
(97,4,20481,20481,20481,20481,20481,65312,0,1059,1168,356,1802.5),
(98,4,20481,20481,20481,20481,20481,79501,0,1199,1203,379,2466.5),
(99,4,20481,20481,20481,20481,20481,97854,0,1275,1242,410,2639.5),
(100,4,20481,20481,20481,20481,20481,101230,0,1536,1350,440,2892.5),
(101,4,20481,20481,20481,20481,20481,104718,0,1670,1460,480,3122.5),
(102,4,20481,20481,20481,20481,20481,108328,0,1804,1520,510,3382.5),
(103,4,20481,20481,20481,20481,20481,108328,0,1938,1820,710,3738.5),
(1,8,32,32,32,32,32,32,120,0,1,10,0.5),
(2,8,40,40,40,40,40,40,147,0,3,10,1),
(3,8,44,44,44,44,44,44,174,0,7,10,1),
(4,8,48,48,48,48,48,48,202,0,8,11,2),
(5,8,60,60,60,60,60,60,230,25,9,11,3),
(6,8,72,72,72,72,72,72,259,35,10,11,4),
(7,8,80,80,80,80,80,80,289,48,13,11,4.5),
(8,8,88,88,88,88,88,88,319,62,15,11,5),
(9,8,96,96,96,96,96,96,350,87,16,11,6),
(10,8,113,113,113,113,113,113,382,97,17,12,6.5),
(11,8,209,209,209,209,209,209,459,109,17,12,7),
(12,8,250,250,250,250,250,250,537,114,18,12,7.5),
(13,8,257,257,257,257,257,257,601,118,19,12,8),
(14,8,300,300,300,300,300,300,710,122,21,12,8.5),
(15,8,307,307,307,307,307,307,790,126,22,13,10),
(16,8,346,346,346,346,346,346,856,131,24,13,12),
(17,8,353,353,353,353,353,353,938,136,26,13,13),
(18,8,390,390,390,390,390,390,1020,139,27,13,13.5),
(19,8,426,426,426,426,426,426,1118,143,28,13,14),
(20,8,438,438,438,438,438,438,1202,148,30,14,17),
(21,8,474,474,474,474,474,474,1272,152,32,14,19),
(22,8,482,482,482,482,482,482,1357,156,34,14,25),
(23,8,538,538,538,538,538,538,1443,160,35,14,30),
(24,8,556,556,556,556,556,556,1545,165,36,15,33),
(25,8,630,630,630,630,630,630,1633,169,37,15,34),
(26,8,643,643,643,643,643,643,1707,173,39,15,36),
(27,8,673,673,673,673,673,673,1812,177,40,15,37.5),
(28,8,694,694,694,694,694,694,1977,182,42,15,38),
(29,8,742,742,742,742,742,742,2068,186,44,16,39),
(30,8,760,760,760,760,760,760,2175,190,44,16,45),
(31,8,839,839,839,839,839,839,2253,194,46,16,52),
(32,8,854,854,854,854,854,854,2362,199,47,16,53),
(33,8,916,916,916,916,916,916,2457,203,49,17,55),
(34,8,946,946,946,946,946,946,2553,207,50,17,60),
(35,8,996,996,996,996,996,996,2680,211,52,17,65),
(36,8,1016,1016,1016,1016,1016,1016,2763,216,53,18,66),
(37,8,1066,1066,1066,1066,1066,1066,2861,220,54,18,67),
(38,8,1086,1086,1086,1086,1086,1086,2975,224,56,18,68),
(39,8,1158,1158,1158,1158,1158,1158,3075,228,58,18,69),
(40,8,1179,1179,1179,1179,1179,1179,3191,233,60,19,80),
(41,8,1271,1271,1271,1271,1271,1271,3293,237,62,19,82),
(42,8,1290,1290,1290,1290,1290,1290,3471,241,67,19,84),
(43,8,1328,1328,1328,1328,1328,1328,3575,245,71,19,88),
(44,8,1350,1350,1350,1350,1350,1350,3680,250,78,20,90),
(45,8,1434,1434,1434,1434,1434,1434,3801,254,81,20,92),
(46,8,1466,1466,1466,1466,1466,1466,3923,258,83,20,96),
(47,8,1560,1560,1560,1560,1560,1560,4031,263,85,21,98),
(48,8,1581,1581,1581,1581,1581,1581,4140,267,87,21,100),
(49,8,1635,1635,1635,1635,1635,1635,4281,271,89,21,115),
(50,8,1656,1656,1656,1656,1656,1656,4393,275,91,22,120),
(51,8,1737,1737,1737,1737,1737,1737,4506,279,93,22,125),
(52,8,1766,1766,1766,1766,1766,1766,4650,283,95,22,130),
(53,8,1840,1840,1840,1840,1840,1840,4765,287,97,23,135),
(54,8,1867,1867,1867,1867,1773,1867,4896,291,99,23,140),
(55,8,1938,1938,1938,1938,1938,1938,5013,297,101,23,145),
(56,8,1974,1974,1974,1974,1974,1974,5206,301,104,24,148),
(57,8,2050,2050,2050,2050,2050,2050,5340,305,106,24,150),
(58,8,2074,2074,2074,2074,2074,2074,5461,309,107,24,152),
(59,8,2198,2198,2198,2198,2198,2198,5598,314,110,25,155),
(60,8,2350,2350,2350,2350,2350,2350,5751,318,112,25,160),
(61,8,2430,2430,2430,2430,2430,2430,5875,322,115,25,180),
(62,8,2498,2498,2498,2498,2498,2498,6015,326,116,25,184),
(63,8,2566,2566,2566,2566,2566,2566,6156,331,119,26,188),
(64,8,2682,2682,2682,2682,2682,2682,6299,335,121,26,190),
(65,8,2813,2813,2813,2813,2813,2813,6443,339,124,26,202),
(66,8,2908,2908,2908,2908,2908,2908,6588,343,125,27,209),
(67,8,3056,3056,3056,3056,3056,3056,6749,348,128,27,219),
(68,8,3086,3086,3086,3086,3086,3086,6882,352,130,27,220),
(69,8,3179,3179,3179,3179,3179,3179,7031,356,133,28,227),
(70,8,3439,3439,3439,3439,3439,3439,7196,360,135,28,245),
(71,8,3554,3554,3554,3554,3554,3554,7332,365,137,31,254),
(72,8,3686,3686,3686,3686,3686,3686,7500,369,140,34,264),
(73,8,3909,3909,3909,3909,3909,3909,7654,373,143,37,276),
(74,8,3978,3978,3978,3978,3978,3978,7809,377,158,41,282),
(75,8,4094,4094,4094,4094,4094,4094,7981,381,175,45,290),
(76,8,4191,4191,4191,4191,4191,4191,8139,385,193,49,306),
(77,8,4238,4238,4238,4238,4238,4238,8313,390,214,54,324),
(78,8,4326,4326,4326,4326,4326,4326,8459,394,236,58,334),
(79,8,4369,4369,4369,4369,4369,4369,8636,399,260,64,336),
(80,8,4427,4427,4427,4427,4427,4427,8726,403,289,69,338),
(81,8,4810,4810,4810,4810,4810,4810,8979,405,304,74,406),
(82,8,5172,5172,5172,5172,5172,5172,9160,411,326,79,412),
(83,8,5677,5677,5677,5677,5677,5677,9328,415,343,84,419),
(84,8,6000,6000,6000,6000,6000,6000,9512,420,832,148,420),
(85,8,6390,6390,6390,6390,6390,6390,19394,424,867,156,432),
(86,8,7072,7072,7072,7072,7072,7072,19738,428,879,169,438),
(87,8,7668,7668,7668,7668,7668,7668,20114,432,896,184,445),
(88,8,8139,8139,8139,8139,8139,8139,20464,440,910,193,459),
(89,8,8706,8706,8706,8706,8706,8706,20846,444,945,208,478),
(90,8,9195,9195,9195,9195,9195,14002,21262,356,967,229,525),
(91,8,9513,9513,9513,9513,9513,16380,21620,403,983,243,797.5),
(92,8,9841,9841,9841,9841,9841,18351,22012,457,997,265,1482.5),
(93,8,10180,10180,10180,10180,10180,21571,22406,517,1024,279,1638.5),
(94,8,16385,16385,16385,16385,16385,25290,25290,585,1059,302,1649.5),
(95,8,16385,16385,16385,16385,16385,29775,23204,662,1085,312,1743.5),
(96,8,16385,16385,16385,16385,16385,36424,23578,749,1135,325,1799.5),
(97,8,16385,16385,16385,16385,16385,44028,24394,847,1168,356,1802.5),
(98,8,16385,16385,16385,16385,16385,52250,24394,959,1203,379,2466.5),
(99,8,16385,16385,16385,16385,16385,63601,24836,1086,1242,410,2639.5),
(100,8,16385,16385,16385,16385,16385,16385,25282,1229,1350,440,2892.5),
(101,8,16385,16385,20481,20481,20481,78283,12082,1336,1460,480,3122.5),
(102,8,16385,16385,16385,16385,16385,80984,12082,1443,1520,510,3382.5),
(103,8,16385,16385,16385,16385,16385,83774,12082,1550,1820,710,3738.5);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
