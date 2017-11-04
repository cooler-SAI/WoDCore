/*
SQLyog Community v12.09 (64 bit)
MySQL - 5.5.9 : Database - characters
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `account_achievement` */

DROP TABLE IF EXISTS `account_achievement`;

CREATE TABLE `account_achievement` (
  `account` int(10) unsigned NOT NULL,
  `first_guid` int(10) unsigned NOT NULL,
  `achievement` smallint(5) unsigned NOT NULL,
  `date` int(10) unsigned NOT NULL,
  PRIMARY KEY (`account`,`achievement`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `account_achievement` */

/*Table structure for table `account_achievement_progress` */

DROP TABLE IF EXISTS `account_achievement_progress`;

CREATE TABLE `account_achievement_progress` (
  `account` int(10) unsigned NOT NULL,
  `criteria` smallint(5) unsigned NOT NULL,
  `counter` int(10) unsigned NOT NULL,
  `date` int(10) unsigned NOT NULL,
  PRIMARY KEY (`account`,`criteria`),
  KEY `Account` (`account`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `account_achievement_progress` */

insert  into `account_achievement_progress`(`account`,`criteria`,`counter`,`date`) values (5,5215,1,1509780301),(5,20677,1,1509780301),(5,20678,1,1509780301),(5,25828,3000,1509780301),(5,28981,3000,1509780301);

/*Table structure for table `account_data` */

DROP TABLE IF EXISTS `account_data`;

CREATE TABLE `account_data` (
  `accountId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Account Identifier',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  `data` blob NOT NULL,
  PRIMARY KEY (`accountId`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `account_data` */

insert  into `account_data`(`accountId`,`type`,`time`,`data`) values (5,0,1509780300,'SET flaggedTutorials \"v##$##E##%##(\"\r\nSET cameraDistanceMaxFactor \"1\"\r\nSET showTutorials \"0\"\r\nSET showNPETutorials \"0\"\r\n');

/*Table structure for table `account_instance_times` */

DROP TABLE IF EXISTS `account_instance_times`;

CREATE TABLE `account_instance_times` (
  `accountId` int(10) unsigned NOT NULL,
  `instanceId` int(10) unsigned NOT NULL DEFAULT '0',
  `releaseTime` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`accountId`,`instanceId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `account_instance_times` */

/*Table structure for table `account_social` */

DROP TABLE IF EXISTS `account_social`;

CREATE TABLE `account_social` (
  `account_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Account Identifier',
  `friend` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Friend Global Unique Identifier',
  `flags` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Friend Flags',
  `note` varchar(48) NOT NULL DEFAULT '' COMMENT 'Friend Note',
  PRIMARY KEY (`account_id`,`friend`,`flags`),
  KEY `friend` (`friend`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `account_social` */

/*Table structure for table `account_toys` */

DROP TABLE IF EXISTS `account_toys`;

CREATE TABLE `account_toys` (
  `account_id` int(10) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `is_favorite` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`account_id`,`item_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `account_toys` */

/*Table structure for table `account_tutorial` */

DROP TABLE IF EXISTS `account_tutorial`;

CREATE TABLE `account_tutorial` (
  `accountId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Account Identifier',
  `tut0` int(10) unsigned NOT NULL DEFAULT '0',
  `tut1` int(10) unsigned NOT NULL DEFAULT '0',
  `tut2` int(10) unsigned NOT NULL DEFAULT '0',
  `tut3` int(10) unsigned NOT NULL DEFAULT '0',
  `tut4` int(10) unsigned NOT NULL DEFAULT '0',
  `tut5` int(10) unsigned NOT NULL DEFAULT '0',
  `tut6` int(10) unsigned NOT NULL DEFAULT '0',
  `tut7` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`accountId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `account_tutorial` */

/*Table structure for table `addons` */

DROP TABLE IF EXISTS `addons`;

CREATE TABLE `addons` (
  `name` varchar(120) NOT NULL DEFAULT '',
  `crc` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Addons';

/*Data for the table `addons` */

insert  into `addons`(`name`,`crc`) values ('Blizzard_AchievementUI',0),('Blizzard_ArchaeologyUI',0),('Blizzard_ArenaUI',0),('Blizzard_AuctionUI',0),('Blizzard_AuthChallengeUI',0),('Blizzard_BarbershopUI',0),('Blizzard_BattlefieldMinimap',0),('Blizzard_BindingUI',0),('Blizzard_BlackMarketUI',0),('Blizzard_Calendar',0),('Blizzard_ChallengesUI',0),('Blizzard_ClientSavedVariables',0),('Blizzard_Collections',0),('Blizzard_CombatLog',0),('Blizzard_CombatText',0),('Blizzard_CompactRaidFrames',0),('Blizzard_CUFProfiles',0),('Blizzard_DeathRecap',0),('Blizzard_DebugTools',0),('Blizzard_EncounterJournal',0),('Blizzard_GarrisonUI',0),('Blizzard_GlyphUI',0),('Blizzard_GMChatUI',0),('Blizzard_GMSurveyUI',0),('Blizzard_GuildBankUI',0),('Blizzard_GuildControlUI',0),('Blizzard_GuildUI',0),('Blizzard_InspectUI',0),('Blizzard_ItemAlterationUI',0),('Blizzard_ItemSocketingUI',0),('Blizzard_ItemUpgradeUI',0),('Blizzard_LookingForGuildUI',0),('Blizzard_MacroUI',0),('Blizzard_MovePad',0),('Blizzard_ObjectiveTracker',0),('Blizzard_PetBattleUI',0),('Blizzard_PVPUI',0),('Blizzard_QuestChoice',0),('Blizzard_RaidUI',0),('Blizzard_SocialUI',0),('Blizzard_StoreUI',0),('Blizzard_TalentUI',0),('Blizzard_TimeManager',0),('Blizzard_TokenUI',0),('Blizzard_TradeSkillUI',0),('Blizzard_TrainerUI',0),('Blizzard_Tutorial',0),('Blizzard_VoidStorageUI',0),('Blizzard_WowTokenUI',0);

/*Table structure for table `arena_team` */

DROP TABLE IF EXISTS `arena_team`;

CREATE TABLE `arena_team` (
  `arenaTeamId` int(10) unsigned NOT NULL DEFAULT '0',
  `name` varchar(24) NOT NULL,
  `captainGuid` int(10) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `rating` smallint(5) unsigned NOT NULL DEFAULT '0',
  `seasonGames` smallint(5) unsigned NOT NULL DEFAULT '0',
  `seasonWins` smallint(5) unsigned NOT NULL DEFAULT '0',
  `weekGames` smallint(5) unsigned NOT NULL DEFAULT '0',
  `weekWins` smallint(5) unsigned NOT NULL DEFAULT '0',
  `rank` int(10) unsigned NOT NULL DEFAULT '0',
  `backgroundColor` int(10) unsigned NOT NULL DEFAULT '0',
  `emblemStyle` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `emblemColor` int(10) unsigned NOT NULL DEFAULT '0',
  `borderStyle` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `borderColor` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`arenaTeamId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `arena_team` */

/*Table structure for table `arena_team_member` */

DROP TABLE IF EXISTS `arena_team_member`;

CREATE TABLE `arena_team_member` (
  `arenaTeamId` int(10) unsigned NOT NULL DEFAULT '0',
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `weekGames` smallint(5) unsigned NOT NULL DEFAULT '0',
  `weekWins` smallint(5) unsigned NOT NULL DEFAULT '0',
  `seasonGames` smallint(5) unsigned NOT NULL DEFAULT '0',
  `seasonWins` smallint(5) unsigned NOT NULL DEFAULT '0',
  `personalRating` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`arenaTeamId`,`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `arena_team_member` */

/*Table structure for table `auctionhouse` */

DROP TABLE IF EXISTS `auctionhouse`;

CREATE TABLE `auctionhouse` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `auctioneerguid` int(10) unsigned NOT NULL DEFAULT '0',
  `itemguid` int(10) unsigned NOT NULL DEFAULT '0',
  `itemowner` int(10) unsigned NOT NULL DEFAULT '0',
  `buyoutprice` bigint(20) unsigned NOT NULL DEFAULT '0',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  `buyguid` int(10) unsigned NOT NULL DEFAULT '0',
  `lastbid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `startbid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `deposit` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `item_guid` (`itemguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `auctionhouse` */

/*Table structure for table `autobroadcast` */

DROP TABLE IF EXISTS `autobroadcast`;

CREATE TABLE `autobroadcast` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `text` longtext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Data for the table `autobroadcast` */

/*Table structure for table `blackmarket` */

DROP TABLE IF EXISTS `blackmarket`;

CREATE TABLE `blackmarket` (
  `id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Identifier',
  `templateId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Reference to world.blackmarket_template',
  `startTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Start time of the bid',
  `bid` bigint(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Amount of the actual bid',
  `bidder` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Player guid (highest bid)',
  `bidderCount` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `blackmarket` */

insert  into `blackmarket`(`id`,`templateId`,`startTime`,`bid`,`bidder`,`bidderCount`) values (1,32,1509779431,100000000,0,0),(2,90,1509779423,150000000,0,0),(3,146,1509779384,125000000,0,0),(4,40,1509779409,100000000,0,0),(5,43,1509779387,100000000,0,0),(6,55,1509779446,100000000,0,0),(7,38,1509779385,100000000,0,0),(8,19,1509779383,100000000,0,0),(9,36,1509779411,100000000,0,0),(10,31,1509779428,100000000,0,0);

/*Table structure for table `bugreport` */

DROP TABLE IF EXISTS `bugreport`;

CREATE TABLE `bugreport` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `type` longtext NOT NULL,
  `content` longtext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Debug System';

/*Data for the table `bugreport` */

/*Table structure for table `calendar_events` */

DROP TABLE IF EXISTS `calendar_events`;

CREATE TABLE `calendar_events` (
  `id` bigint(20) unsigned NOT NULL DEFAULT '0',
  `creator` int(10) unsigned NOT NULL DEFAULT '0',
  `title` varchar(255) NOT NULL DEFAULT '',
  `description` varchar(255) NOT NULL DEFAULT '',
  `type` tinyint(1) unsigned NOT NULL DEFAULT '4',
  `dungeon` int(10) NOT NULL DEFAULT '-1',
  `eventtime` int(10) unsigned NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `time2` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `calendar_events` */

/*Table structure for table `calendar_invites` */

DROP TABLE IF EXISTS `calendar_invites`;

CREATE TABLE `calendar_invites` (
  `id` bigint(20) unsigned NOT NULL DEFAULT '0',
  `event` bigint(20) unsigned NOT NULL DEFAULT '0',
  `invitee` int(10) unsigned NOT NULL DEFAULT '0',
  `sender` int(10) unsigned NOT NULL DEFAULT '0',
  `status` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `statustime` int(10) unsigned NOT NULL DEFAULT '0',
  `rank` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `text` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `calendar_invites` */

/*Table structure for table `channels` */

DROP TABLE IF EXISTS `channels`;

CREATE TABLE `channels` (
  `name` varchar(128) NOT NULL,
  `team` int(10) unsigned NOT NULL,
  `announce` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `ownership` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `password` varchar(32) DEFAULT NULL,
  `bannedList` text,
  `lastUsed` int(10) unsigned NOT NULL,
  PRIMARY KEY (`name`,`team`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Channel System';

/*Data for the table `channels` */

/*Table structure for table `character_account_data` */

DROP TABLE IF EXISTS `character_account_data`;

CREATE TABLE `character_account_data` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  `data` blob NOT NULL,
  PRIMARY KEY (`guid`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_account_data` */

insert  into `character_account_data`(`guid`,`type`,`time`,`data`) values (2,1,1509780300,'SET autoQuestPopUps \"v\"\r\nSET trackedQuests \"v$)l\"\r\nSET hardTrackedQuests \"v\"\r\nSET trackedAchievements \"v\"\r\nSET cameraSavedDistance \"5.550000\"\r\nSET cameraSavedPitch \"10.000000\"\r\nSET currencyCategoriesCollapsed \"v\"\r\nSET minimapTrackedInfov2 \"229384\"\r\nSET minimapShapeshiftTracking \"v\"\r\nSET reputationsCollapsed \"v##$\"\r\nSET activeCUFProfile \"Основной\"\r\nSET EJLootClass \"2\"\r\n'),(2,7,1509780300,'VERSION 5\n\nADDEDVERSION 19\n\nCHANNELS\nEND\n\nZONECHANNELS 2097155\n\nCOLORS\n\nSYSTEM 255 255 0 N\nSAY 255 255 255 N\nPARTY 170 170 255 N\nRAID 255 127 0 N\nGUILD 64 255 64 N\nOFFICER 64 192 64 N\nYELL 255 64 64 N\nWHISPER 255 128 255 N\nWHISPER_FOREIGN 255 128 255 N\nWHISPER_INFORM 255 128 255 N\nEMOTE 255 128 64 N\nTEXT_EMOTE 255 128 64 N\nMONSTER_SAY 255 255 159 N\nMONSTER_PARTY 170 170 255 N\nMONSTER_YELL 255 64 64 N\nMONSTER_WHISPER 255 181 235 N\nMONSTER_EMOTE 255 128 64 N\nCHANNEL 255 192 192 N\nCHANNEL_JOIN 192 128 128 N\nCHANNEL_LEAVE 192 128 128 N\nCHANNEL_LIST 192 128 128 N\nCHANNEL_NOTICE 192 192 192 N\nCHANNEL_NOTICE_USER 192 192 192 N\nAFK 255 128 255 N\nDND 255 128 255 N\nIGNORED 255 0 0 N\nSKILL 85 85 255 N\nLOOT 0 170 0 N\nMONEY 255 255 0 N\nOPENING 128 128 255 N\nTRADESKILLS 255 255 255 N\nPET_INFO 128 128 255 N\nCOMBAT_MISC_INFO 128 128 255 N\nCOMBAT_XP_GAIN 111 111 255 N\nCOMBAT_HONOR_GAIN 224 202 10 N\nCOMBAT_FACTION_CHANGE 128 128 255 N\nBG_SYSTEM_NEUTRAL 255 120 10 N\nBG_SYSTEM_ALLIANCE 0 174 239 N\nBG_SYSTEM_HORDE 255 0 0 N\nRAID_LEADER 255 72 9 N\nRAID_WARNING 255 72 0 N\nRAID_BOSS_EMOTE 255 221 0 N\nRAID_BOSS_WHISPER 255 221 0 N\nFILTERED 255 0 0 N\nRESTRICTED 255 0 0 N\nBATTLENET 255 255 255 N\nACHIEVEMENT 255 255 0 N\nGUILD_ACHIEVEMENT 64 255 64 N\nARENA_POINTS 255 255 255 N\nPARTY_LEADER 118 200 255 N\nTARGETICONS 255 255 0 N\nBN_WHISPER 0 255 246 N\nBN_WHISPER_INFORM 0 255 246 N\nBN_CONVERSATION 0 177 240 N\nBN_CONVERSATION_NOTICE 0 177 240 N\nBN_CONVERSATION_LIST 0 177 240 N\nBN_INLINE_TOAST_ALERT 130 197 255 N\nBN_INLINE_TOAST_BROADCAST 130 197 255 N\nBN_INLINE_TOAST_BROADCAST_INFORM 130 197 255 N\nBN_INLINE_TOAST_CONVERSATION 130 197 255 N\nBN_WHISPER_PLAYER_OFFLINE 255 255 0 N\nCOMBAT_GUILD_XP_GAIN 111 111 255 N\nCURRENCY 0 170 0 N\nQUEST_BOSS_EMOTE 255 128 64 N\nPET_BATTLE_COMBAT_LOG 231 222 171 N\nPET_BATTLE_INFO 225 222 93 N\nINSTANCE_CHAT 255 127 0 N\nINSTANCE_CHAT_LEADER 255 72 9 N\nCHANNEL1 255 192 192 N\nCHANNEL2 255 192 192 N\nCHANNEL3 255 192 192 N\nCHANNEL4 255 192 192 N\nCHANNEL5 255 192 192 N\nCHANNEL6 255 192 192 N\nCHANNEL7 255 192 192 N\nCHANNEL8 255 192 192 N\nCHANNEL9 255 192 192 N\nCHANNEL10 255 192 192 N\nEND\n\nWINDOW 1\nNAME Общий\nSIZE 0\nCOLOR 0 0 0 40\nLOCKED 1\nUNINTERACTABLE 0\nDOCKED 1\nSHOWN 1\nMESSAGES\nSYSTEM\nSYSTEM_NOMENU\nSAY\nEMOTE\nYELL\nWHISPER\nPARTY\nPARTY_LEADER\nRAID\nRAID_LEADER\nRAID_WARNING\nGUILD\nOFFICER\nMONSTER_SAY\nMONSTER_YELL\nMONSTER_EMOTE\nMONSTER_WHISPER\nMONSTER_BOSS_EMOTE\nMONSTER_BOSS_WHISPER\nERRORS\nAFK\nDND\nIGNORED\nBG_HORDE\nBG_ALLIANCE\nBG_NEUTRAL\nCOMBAT_FACTION_CHANGE\nSKILL\nLOOT\nMONEY\nCHANNEL\nACHIEVEMENT\nGUILD_ACHIEVEMENT\nBN_WHISPER\nBN_WHISPER_INFORM\nBN_CONVERSATION\nBN_INLINE_TOAST_ALERT\nCURRENCY\nBN_WHISPER_PLAYER_OFFLINE\nPET_BATTLE_INFO\nINSTANCE_CHAT\nINSTANCE_CHAT_LEADER\nEND\n\nCHANNELS\nEND\n\nZONECHANNELS 2097155\n\nEND\n\nWINDOW 2\nNAME Журнал боя\nSIZE 0\nCOLOR 0 0 0 40\nLOCKED 1\nUNINTERACTABLE 0\nDOCKED 2\nSHOWN 0\nMESSAGES\nOPENING\nTRADESKILLS\nPET_INFO\nCOMBAT_XP_GAIN\nCOMBAT_HONOR_GAIN\nCOMBAT_MISC_INFO\nCOMBAT_GUILD_XP_GAIN\nEND\n\nCHANNELS\nEND\n\nZONECHANNELS 0\n\nEND\n\nWINDOW 3\nSIZE 0\nCOLOR 0 0 0 40\nLOCKED 1\nUNINTERACTABLE 0\nDOCKED 0\nSHOWN 0\nMESSAGES\nEND\n\nCHANNELS\nEND\n\nZONECHANNELS 0\n\nEND\n\nWINDOW 4\nSIZE 0\nCOLOR 0 0 0 40\nLOCKED 1\nUNINTERACTABLE 0\nDOCKED 0\nSHOWN 0\nMESSAGES\nEND\n\nCHANNELS\nEND\n\nZONECHANNELS 0\n\nEND\n\nWINDOW 5\nSIZE 0\nCOLOR 0 0 0 40\nLOCKED 1\nUNINTERACTABLE 0\nDOCKED 0\nSHOWN 0\nMESSAGES\nEND\n\nCHANNELS\nEND\n\nZONECHANNELS 0\n\nEND\n\nWINDOW 6\nSIZE 0\nCOLOR 0 0 0 40\nLOCKED 1\nUNINTERACTABLE 0\nDOCKED 0\nSHOWN 0\nMESSAGES\nEND\n\nCHANNELS\nEND\n\nZONECHANNELS 0\n\nEND\n\nWINDOW 7\nSIZE 0\nCOLOR 0 0 0 40\nLOCKED 1\nUNINTERACTABLE 0\nDOCKED 0\nSHOWN 0\nMESSAGES\nEND\n\nCHANNELS\nEND\n\nZONECHANNELS 0\n\nEND\n\nWINDOW 8\nSIZE 0\nCOLOR 0 0 0 40\nLOCKED 1\nUNINTERACTABLE 0\nDOCKED 0\nSHOWN 0\nMESSAGES\nEND\n\nCHANNELS\nEND\n\nZONECHANNELS 0\n\nEND\n\nWINDOW 9\nSIZE 0\nCOLOR 0 0 0 40\nLOCKED 1\nUNINTERACTABLE 0\nDOCKED 0\nSHOWN 0\nMESSAGES\nEND\n\nCHANNELS\nEND\n\nZONECHANNELS 0\n\nEND\n\nWINDOW 10\nSIZE 0\nCOLOR 0 0 0 40\nLOCKED 1\nUNINTERACTABLE 0\nDOCKED 0\nSHOWN 0\nMESSAGES\nEND\n\nCHANNELS\nEND\n\nZONECHANNELS 0\n\nEND\n\n');

/*Table structure for table `character_achievement` */

DROP TABLE IF EXISTS `character_achievement`;

CREATE TABLE `character_achievement` (
  `guid` int(10) unsigned NOT NULL,
  `achievement` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`guid`,`achievement`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_achievement` */

/*Table structure for table `character_achievement_count` */

DROP TABLE IF EXISTS `character_achievement_count`;

CREATE TABLE `character_achievement_count` (
  `guid` bigint(11) NOT NULL DEFAULT '0',
  `count` int(11) DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `character_achievement_count` */

/*Table structure for table `character_achievement_progress` */

DROP TABLE IF EXISTS `character_achievement_progress`;

CREATE TABLE `character_achievement_progress` (
  `guid` int(10) unsigned NOT NULL,
  `criteria` smallint(5) unsigned NOT NULL,
  `counter` int(10) unsigned NOT NULL,
  `date` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`criteria`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_achievement_progress` */

insert  into `character_achievement_progress`(`guid`,`criteria`,`counter`,`date`) values (2,167,1,1509780301),(2,641,1,1509780301),(2,655,1,1509780301),(2,656,1,1509780301),(2,753,1,1509780301),(2,754,1,1509780301),(2,755,1,1509780301),(2,834,1,1509780301),(2,1552,1,1509780311),(2,2020,200,1509780301),(2,3631,1,1509780349),(2,4946,2,1509780369),(2,4948,2,1509780369),(2,4949,2,1509780369),(2,4950,2,1509780369),(2,4951,2,1509780369),(2,4952,2,1509780369),(2,4953,2,1509780369),(2,4954,2,1509780369),(2,4955,2,1509780369),(2,4956,2,1509780369),(2,4957,2,1509780369),(2,4958,2,1509780369),(2,5231,1,1509780301),(2,5301,8,1509780301),(2,5328,3154,1509780349),(2,5329,3154,1509780349),(2,5330,3154,1509780349),(2,5331,3154,1509780349),(2,5332,4225,1509780349),(2,16190,1,1509780301),(2,16826,54,1509780349),(2,19481,3100,1509780301),(2,25256,1,1509780301),(2,25259,1,1509780301),(2,25264,1,1509780301),(2,28099,1,1509780301);

/*Table structure for table `character_action` */

DROP TABLE IF EXISTS `character_action`;

CREATE TABLE `character_action` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `spec` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `button` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `action` int(10) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spec`,`button`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_action` */

insert  into `character_action`(`guid`,`spec`,`button`,`action`,`type`) values (2,0,0,35395,0),(2,0,9,59542,0),(2,0,83,4540,128);

/*Table structure for table `character_archaeology` */

DROP TABLE IF EXISTS `character_archaeology`;

CREATE TABLE `character_archaeology` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `counts` text,
  `projects` text,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_archaeology` */

/*Table structure for table `character_archaeology_projects` */

DROP TABLE IF EXISTS `character_archaeology_projects`;

CREATE TABLE `character_archaeology_projects` (
  `guid` int(10) unsigned NOT NULL,
  `project` int(8) unsigned NOT NULL,
  `count` int(8) unsigned NOT NULL,
  `first_date` int(8) unsigned NOT NULL,
  PRIMARY KEY (`guid`,`project`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `character_archaeology_projects` */

/*Table structure for table `character_archaeology_sites` */

DROP TABLE IF EXISTS `character_archaeology_sites`;

CREATE TABLE `character_archaeology_sites` (
  `guid` int(11) unsigned NOT NULL,
  `map` int(11) unsigned NOT NULL,
  `sites` text NOT NULL,
  PRIMARY KEY (`guid`,`map`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_archaeology_sites` */

/*Table structure for table `character_arena_data` */

DROP TABLE IF EXISTS `character_arena_data`;

CREATE TABLE `character_arena_data` (
  `guid` int(11) NOT NULL,
  `rating0` int(10) DEFAULT '0',
  `bestRatingOfWeek0` int(10) unsigned NOT NULL DEFAULT '0',
  `bestRatingOfSeason0` int(10) unsigned NOT NULL DEFAULT '0',
  `matchMakerRating0` int(10) DEFAULT '0',
  `weekGames0` int(10) DEFAULT '0',
  `weekWins0` int(10) DEFAULT '0',
  `prevWeekWins0` int(10) unsigned NOT NULL DEFAULT '0',
  `prevWeekGames0` int(10) unsigned NOT NULL DEFAULT '0',
  `seasonGames0` int(10) DEFAULT '0',
  `seasonWins0` int(10) DEFAULT '0',
  `rating1` int(10) DEFAULT '0',
  `bestRatingOfWeek1` int(10) unsigned NOT NULL DEFAULT '0',
  `bestRatingOfSeason1` int(10) unsigned NOT NULL DEFAULT '0',
  `matchMakerRating1` int(10) DEFAULT '0',
  `weekGames1` int(10) DEFAULT '0',
  `weekWins1` int(10) DEFAULT '0',
  `prevWeekWins1` int(10) unsigned NOT NULL DEFAULT '0',
  `prevWeekGames1` int(10) unsigned NOT NULL DEFAULT '0',
  `seasonGames1` int(10) DEFAULT '0',
  `seasonWins1` int(10) DEFAULT '0',
  `rating2` int(10) DEFAULT '0',
  `bestRatingOfWeek2` int(10) unsigned NOT NULL DEFAULT '0',
  `bestRatingOfSeason2` int(10) unsigned NOT NULL DEFAULT '0',
  `matchMakerRating2` int(10) DEFAULT '0',
  `weekGames2` int(10) DEFAULT '0',
  `weekWins2` int(10) DEFAULT '0',
  `prevWeekWins2` int(10) unsigned NOT NULL DEFAULT '0',
  `prevWeekGames2` int(10) unsigned NOT NULL DEFAULT '0',
  `seasonGames2` int(10) DEFAULT '0',
  `seasonWins2` int(10) DEFAULT '0',
  `rating3` int(10) unsigned NOT NULL DEFAULT '0',
  `bestRatingOfWeek3` int(10) unsigned NOT NULL DEFAULT '0',
  `bestRatingOfSeason3` int(10) unsigned NOT NULL DEFAULT '0',
  `matchMakerRating3` int(10) NOT NULL DEFAULT '1500',
  `weekGames3` int(10) unsigned NOT NULL DEFAULT '0',
  `weekWins3` int(10) unsigned NOT NULL DEFAULT '0',
  `prevWeekWins3` int(10) unsigned NOT NULL DEFAULT '0',
  `prevWeekGames3` int(10) unsigned NOT NULL DEFAULT '0',
  `seasonGames3` int(10) unsigned NOT NULL DEFAULT '0',
  `seasonWins3` int(10) unsigned NOT NULL DEFAULT '0',
  `rating4` int(10) unsigned DEFAULT '0',
  `bestRatingOfWeek4` int(10) unsigned NOT NULL DEFAULT '0',
  `bestRatingOfSeason4` int(10) unsigned NOT NULL DEFAULT '0',
  `matchMakerRating4` int(10) DEFAULT '0',
  `weekGames4` int(10) unsigned DEFAULT '0',
  `weekWins4` int(10) unsigned DEFAULT '0',
  `prevWeekWins4` int(10) unsigned NOT NULL DEFAULT '0',
  `prevWeekGames4` int(10) unsigned NOT NULL DEFAULT '0',
  `seasonGames4` int(10) unsigned DEFAULT '0',
  `seasonWins4` int(10) unsigned DEFAULT '0',
  `rating5` int(10) unsigned DEFAULT '0',
  `bestRatingOfWeek5` int(10) unsigned NOT NULL DEFAULT '0',
  `bestRatingOfSeason5` int(10) unsigned NOT NULL DEFAULT '0',
  `matchMakerRating5` int(10) DEFAULT '0',
  `weekGames5` int(10) unsigned DEFAULT '0',
  `weekWins5` int(10) unsigned DEFAULT '0',
  `prevWeekWins5` int(10) unsigned NOT NULL DEFAULT '0',
  `prevWeekGames5` int(10) unsigned NOT NULL DEFAULT '0',
  `seasonGames5` int(10) unsigned DEFAULT '0',
  `seasonWins5` int(10) unsigned DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_arena_data` */

insert  into `character_arena_data`(`guid`,`rating0`,`bestRatingOfWeek0`,`bestRatingOfSeason0`,`matchMakerRating0`,`weekGames0`,`weekWins0`,`prevWeekWins0`,`prevWeekGames0`,`seasonGames0`,`seasonWins0`,`rating1`,`bestRatingOfWeek1`,`bestRatingOfSeason1`,`matchMakerRating1`,`weekGames1`,`weekWins1`,`prevWeekWins1`,`prevWeekGames1`,`seasonGames1`,`seasonWins1`,`rating2`,`bestRatingOfWeek2`,`bestRatingOfSeason2`,`matchMakerRating2`,`weekGames2`,`weekWins2`,`prevWeekWins2`,`prevWeekGames2`,`seasonGames2`,`seasonWins2`,`rating3`,`bestRatingOfWeek3`,`bestRatingOfSeason3`,`matchMakerRating3`,`weekGames3`,`weekWins3`,`prevWeekWins3`,`prevWeekGames3`,`seasonGames3`,`seasonWins3`,`rating4`,`bestRatingOfWeek4`,`bestRatingOfSeason4`,`matchMakerRating4`,`weekGames4`,`weekWins4`,`prevWeekWins4`,`prevWeekGames4`,`seasonGames4`,`seasonWins4`,`rating5`,`bestRatingOfWeek5`,`bestRatingOfSeason5`,`matchMakerRating5`,`weekGames5`,`weekWins5`,`prevWeekWins5`,`prevWeekGames5`,`seasonGames5`,`seasonWins5`) values (1,0,0,0,1500,0,0,0,1868459651,0,0,0,0,0,1500,0,0,0,32763,0,0,0,0,0,1500,0,0,0,4,0,0,0,0,0,1500,0,0,0,0,0,0,0,0,0,1500,0,0,0,0,0,0,0,0,0,1500,0,0,0,0,0,0),(2,0,0,0,1500,0,0,0,1868459651,0,0,0,0,0,1500,0,0,0,32763,0,0,0,0,0,1500,0,0,0,4,0,0,0,0,0,1500,0,0,0,0,0,0,0,0,0,1500,0,0,0,0,0,0,0,0,0,1500,0,0,0,0,0,0);

/*Table structure for table `character_arena_stats` */

DROP TABLE IF EXISTS `character_arena_stats`;

CREATE TABLE `character_arena_stats` (
  `guid` int(10) NOT NULL,
  `slot` tinyint(3) NOT NULL,
  `matchMakerRating` smallint(5) NOT NULL,
  PRIMARY KEY (`guid`,`slot`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_arena_stats` */

/*Table structure for table `character_aura` */

DROP TABLE IF EXISTS `character_aura`;

CREATE TABLE `character_aura` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `slot` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `caster_guid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'Full Global Unique Identifier',
  `item_guid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `effect_mask` int(11) unsigned NOT NULL DEFAULT '0',
  `recalculate_mask` int(11) unsigned NOT NULL DEFAULT '0',
  `stackcount` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `maxduration` int(11) NOT NULL DEFAULT '0',
  `remaintime` int(11) NOT NULL DEFAULT '0',
  `remaincharges` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `castItemLevel` int(11) NOT NULL DEFAULT '-1',
  PRIMARY KEY (`guid`,`caster_guid`,`item_guid`,`spell`,`effect_mask`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `character_aura` */

/*Table structure for table `character_aura_effect` */

DROP TABLE IF EXISTS `character_aura_effect`;

CREATE TABLE `character_aura_effect` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `slot` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `effect` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `baseamount` int(11) NOT NULL DEFAULT '0',
  `amount` int(11) NOT NULL,
  `data` blob,
  PRIMARY KEY (`guid`,`slot`,`effect`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `character_aura_effect` */

/*Table structure for table `character_banned` */

DROP TABLE IF EXISTS `character_banned`;

CREATE TABLE `character_banned` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `bandate` int(10) unsigned NOT NULL DEFAULT '0',
  `unbandate` int(10) unsigned NOT NULL DEFAULT '0',
  `bannedby` varchar(50) NOT NULL,
  `banreason` varchar(255) NOT NULL,
  `active` tinyint(3) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`guid`,`bandate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Ban List';

/*Data for the table `character_banned` */

/*Table structure for table `character_battleground_data` */

DROP TABLE IF EXISTS `character_battleground_data`;

CREATE TABLE `character_battleground_data` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `instanceId` int(10) unsigned NOT NULL COMMENT 'Instance Identifier',
  `team` smallint(5) unsigned NOT NULL,
  `joinX` float NOT NULL DEFAULT '0',
  `joinY` float NOT NULL DEFAULT '0',
  `joinZ` float NOT NULL DEFAULT '0',
  `joinO` float NOT NULL DEFAULT '0',
  `joinMapId` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Map Identifier',
  `taxiStart` int(10) unsigned NOT NULL DEFAULT '0',
  `taxiEnd` int(10) unsigned NOT NULL DEFAULT '0',
  `mountSpell` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `lastActiveSpec` smallint(6) DEFAULT NULL,
  `bgTypeId` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `character_battleground_data` */

insert  into `character_battleground_data`(`guid`,`instanceId`,`team`,`joinX`,`joinY`,`joinZ`,`joinO`,`joinMapId`,`taxiStart`,`taxiEnd`,`mountSpell`,`lastActiveSpec`,`bgTypeId`) values (2,0,0,0,0,0,0,65535,0,0,0,0,0);

/*Table structure for table `character_battleground_random` */

DROP TABLE IF EXISTS `character_battleground_random`;

CREATE TABLE `character_battleground_random` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_battleground_random` */

/*Table structure for table `character_completed_challenges` */

DROP TABLE IF EXISTS `character_completed_challenges`;

CREATE TABLE `character_completed_challenges` (
  `guid` int(10) unsigned NOT NULL,
  `map_id` int(10) unsigned NOT NULL DEFAULT '0',
  `best_time` int(10) unsigned NOT NULL DEFAULT '0',
  `last_time` int(10) unsigned NOT NULL DEFAULT '0',
  `best_medal` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `best_medal_date` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`map_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `character_completed_challenges` */

/*Table structure for table `character_currency` */

DROP TABLE IF EXISTS `character_currency`;

CREATE TABLE `character_currency` (
  `guid` int(11) unsigned NOT NULL,
  `currency` smallint(5) unsigned NOT NULL,
  `total_count` int(11) unsigned NOT NULL,
  `week_count` int(11) unsigned NOT NULL,
  `season_total` int(11) DEFAULT '0',
  `flags` int(10) unsigned DEFAULT NULL,
  `weekCap` int(10) unsigned NOT NULL DEFAULT '0',
  `needResetCap` tinyint(1) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`guid`,`currency`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_currency` */

/*Table structure for table `character_daily_loot_cooldown` */

DROP TABLE IF EXISTS `character_daily_loot_cooldown`;

CREATE TABLE `character_daily_loot_cooldown` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `character_daily_loot_cooldown` */

/*Table structure for table `character_declinedname` */

DROP TABLE IF EXISTS `character_declinedname`;

CREATE TABLE `character_declinedname` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `genitive` varchar(15) NOT NULL DEFAULT '',
  `dative` varchar(15) NOT NULL DEFAULT '',
  `accusative` varchar(15) NOT NULL DEFAULT '',
  `instrumental` varchar(15) NOT NULL DEFAULT '',
  `prepositional` varchar(15) NOT NULL DEFAULT '',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_declinedname` */

insert  into `character_declinedname`(`guid`,`genitive`,`dative`,`accusative`,`instrumental`,`prepositional`) values (2,'Варитиса','Варитису','Варитиса','Варитисом','Варитисе');

/*Table structure for table `character_equipmentsets` */

DROP TABLE IF EXISTS `character_equipmentsets`;

CREATE TABLE `character_equipmentsets` (
  `guid` int(10) NOT NULL DEFAULT '0',
  `setguid` bigint(20) NOT NULL AUTO_INCREMENT,
  `setindex` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `name` varchar(31) NOT NULL,
  `iconname` varchar(100) NOT NULL,
  `ignore_mask` int(11) unsigned NOT NULL DEFAULT '0',
  `item0` int(11) unsigned NOT NULL DEFAULT '0',
  `item1` int(11) unsigned NOT NULL DEFAULT '0',
  `item2` int(11) unsigned NOT NULL DEFAULT '0',
  `item3` int(11) unsigned NOT NULL DEFAULT '0',
  `item4` int(11) unsigned NOT NULL DEFAULT '0',
  `item5` int(11) unsigned NOT NULL DEFAULT '0',
  `item6` int(11) unsigned NOT NULL DEFAULT '0',
  `item7` int(11) unsigned NOT NULL DEFAULT '0',
  `item8` int(11) unsigned NOT NULL DEFAULT '0',
  `item9` int(11) unsigned NOT NULL DEFAULT '0',
  `item10` int(11) unsigned NOT NULL DEFAULT '0',
  `item11` int(11) unsigned NOT NULL DEFAULT '0',
  `item12` int(11) unsigned NOT NULL DEFAULT '0',
  `item13` int(11) unsigned NOT NULL DEFAULT '0',
  `item14` int(11) unsigned NOT NULL DEFAULT '0',
  `item15` int(11) unsigned NOT NULL DEFAULT '0',
  `item16` int(11) unsigned NOT NULL DEFAULT '0',
  `item17` int(11) unsigned NOT NULL DEFAULT '0',
  `item18` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`setguid`),
  UNIQUE KEY `idx_set` (`guid`,`setguid`,`setindex`),
  KEY `Idx_setindex` (`setindex`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_equipmentsets` */

/*Table structure for table `character_garrison` */

DROP TABLE IF EXISTS `character_garrison`;

CREATE TABLE `character_garrison` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `character_guid` int(10) unsigned NOT NULL,
  `level` int(10) NOT NULL DEFAULT '1',
  `blue_recipes` text NOT NULL,
  `specializations` text NOT NULL,
  `num_follower_activation` int(10) unsigned NOT NULL DEFAULT '1',
  `num_follower_activation_regen_timestamp` int(10) unsigned NOT NULL DEFAULT '0',
  `cache_last_usage` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `character_guid` (`character_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `character_garrison` */

/*Table structure for table `character_garrison_building` */

DROP TABLE IF EXISTS `character_garrison_building`;

CREATE TABLE `character_garrison_building` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `garrison_id` int(10) unsigned NOT NULL,
  `plot_instance_id` int(10) unsigned NOT NULL,
  `building_id` int(10) unsigned NOT NULL,
  `spec_id` int(10) unsigned NOT NULL DEFAULT '0',
  `time_built_start` int(10) unsigned NOT NULL DEFAULT '0',
  `time_built_end` int(10) unsigned NOT NULL DEFAULT '0',
  `follower_assigned` int(10) unsigned DEFAULT '0',
  `active` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `gathering_data` text NOT NULL,
  PRIMARY KEY (`id`),
  KEY `garrison_id` (`garrison_id`),
  KEY `id_garrison_id` (`id`,`garrison_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `character_garrison_building` */

/*Table structure for table `character_garrison_daily_tavern_data` */

DROP TABLE IF EXISTS `character_garrison_daily_tavern_data`;

CREATE TABLE `character_garrison_daily_tavern_data` (
  `CharacterGuid` int(10) unsigned NOT NULL DEFAULT '0',
  `NpcEntry` int(8) NOT NULL,
  PRIMARY KEY (`CharacterGuid`,`NpcEntry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `character_garrison_daily_tavern_data` */

/*Table structure for table `character_garrison_follower` */

DROP TABLE IF EXISTS `character_garrison_follower`;

CREATE TABLE `character_garrison_follower` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `garrison_id` int(10) unsigned NOT NULL DEFAULT '0',
  `follower_id` int(10) unsigned NOT NULL DEFAULT '0',
  `level` int(10) unsigned NOT NULL DEFAULT '0',
  `xp` int(10) unsigned NOT NULL DEFAULT '0',
  `quality` int(10) unsigned NOT NULL DEFAULT '0',
  `item_level_armor` int(10) unsigned NOT NULL DEFAULT '0',
  `item_level_weapon` int(10) unsigned NOT NULL DEFAULT '0',
  `current_mission_id` int(10) unsigned NOT NULL DEFAULT '0',
  `current_building_id` int(10) unsigned NOT NULL DEFAULT '0',
  `abilities` text NOT NULL,
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `ship_name` text NOT NULL,
  PRIMARY KEY (`id`),
  KEY `garrison_id_id` (`id`,`garrison_id`),
  KEY `garrison_id` (`garrison_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `character_garrison_follower` */

/*Table structure for table `character_garrison_mission` */

DROP TABLE IF EXISTS `character_garrison_mission`;

CREATE TABLE `character_garrison_mission` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `garrison_id` int(10) unsigned NOT NULL DEFAULT '0',
  `mission_id` int(10) unsigned NOT NULL DEFAULT '0',
  `offer_time` int(10) unsigned NOT NULL DEFAULT '0',
  `offer_max_duration` int(10) unsigned NOT NULL DEFAULT '0',
  `start_time` int(10) unsigned NOT NULL DEFAULT '0',
  `state` int(10) unsigned NOT NULL DEFAULT '10',
  PRIMARY KEY (`id`),
  KEY `garrison_id` (`garrison_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `character_garrison_mission` */

/*Table structure for table `character_garrison_weekly_tavern_data` */

DROP TABLE IF EXISTS `character_garrison_weekly_tavern_data`;

CREATE TABLE `character_garrison_weekly_tavern_data` (
  `CharacterGUID` int(10) DEFAULT NULL,
  `FollowerID` int(10) DEFAULT NULL,
  `Abilities` blob
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `character_garrison_weekly_tavern_data` */

/*Table structure for table `character_garrison_work_order` */

DROP TABLE IF EXISTS `character_garrison_work_order`;

CREATE TABLE `character_garrison_work_order` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `garrison_id` int(10) unsigned NOT NULL,
  `plot_instance_id` int(10) unsigned NOT NULL,
  `shipment_id` int(10) unsigned NOT NULL,
  `creation_time` int(10) unsigned NOT NULL,
  `complete_time` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `garrison_id` (`garrison_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `character_garrison_work_order` */

/*Table structure for table `character_gifts` */

DROP TABLE IF EXISTS `character_gifts`;

CREATE TABLE `character_gifts` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `item_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`item_guid`),
  KEY `idx_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_gifts` */

/*Table structure for table `character_glyphs` */

DROP TABLE IF EXISTS `character_glyphs`;

CREATE TABLE `character_glyphs` (
  `guid` int(10) unsigned NOT NULL,
  `spec` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `glyph1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `glyph2` smallint(5) unsigned DEFAULT '0',
  `glyph3` smallint(5) unsigned DEFAULT '0',
  `glyph4` smallint(5) unsigned DEFAULT '0',
  `glyph5` smallint(5) unsigned DEFAULT '0',
  `glyph6` smallint(5) unsigned DEFAULT '0',
  PRIMARY KEY (`guid`,`spec`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_glyphs` */

/*Table structure for table `character_homebind` */

DROP TABLE IF EXISTS `character_homebind`;

CREATE TABLE `character_homebind` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `mapId` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Map Identifier',
  `zoneId` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Zone Identifier',
  `posX` float NOT NULL DEFAULT '0',
  `posY` float NOT NULL DEFAULT '0',
  `posZ` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `character_homebind` */

insert  into `character_homebind`(`guid`,`mapId`,`zoneId`,`posX`,`posY`,`posZ`) values (2,530,3526,-3961.64,-13931.2,100.615);

/*Table structure for table `character_instance` */

DROP TABLE IF EXISTS `character_instance`;

CREATE TABLE `character_instance` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `instance` int(10) unsigned NOT NULL DEFAULT '0',
  `permanent` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`instance`),
  KEY `instance` (`instance`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_instance` */

/*Table structure for table `character_inventory` */

DROP TABLE IF EXISTS `character_inventory`;

CREATE TABLE `character_inventory` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `bag` int(10) unsigned NOT NULL DEFAULT '0',
  `slot` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `item` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Item Global Unique Identifier',
  PRIMARY KEY (`item`),
  UNIQUE KEY `guid` (`guid`,`bag`,`slot`),
  KEY `idx_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `character_inventory` */

insert  into `character_inventory`(`guid`,`bag`,`slot`,`item`) values (2,0,4,14),(2,0,6,16),(2,0,7,20),(2,0,15,18),(2,0,23,22);

/*Table structure for table `character_lf_guild_params` */

DROP TABLE IF EXISTS `character_lf_guild_params`;

CREATE TABLE `character_lf_guild_params` (
  `guid` int(10) unsigned NOT NULL,
  `guild_interests` smallint(5) unsigned DEFAULT NULL,
  `availability` smallint(5) unsigned DEFAULT NULL,
  `class_roles` smallint(5) unsigned DEFAULT NULL,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `character_lf_guild_params` */

/*Table structure for table `character_pet` */

DROP TABLE IF EXISTS `character_pet`;

CREATE TABLE `character_pet` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `owner` int(10) unsigned NOT NULL DEFAULT '0',
  `modelid` int(10) unsigned DEFAULT '0',
  `CreatedBySpell` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `PetType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `level` smallint(5) unsigned NOT NULL DEFAULT '1',
  `exp` int(10) unsigned NOT NULL DEFAULT '0',
  `Reactstate` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `name` varchar(21) NOT NULL DEFAULT 'Pet',
  `renamed` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `slot` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `curhealth` int(10) unsigned NOT NULL DEFAULT '1',
  `curmana` int(10) unsigned NOT NULL DEFAULT '0',
  `savetime` int(10) unsigned NOT NULL DEFAULT '0',
  `abdata` text,
  `specialization` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `owner` (`owner`),
  KEY `idx_slot` (`slot`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Pet System';

/*Data for the table `character_pet` */

/*Table structure for table `character_pet_declinedname` */

DROP TABLE IF EXISTS `character_pet_declinedname`;

CREATE TABLE `character_pet_declinedname` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `owner` int(10) unsigned NOT NULL DEFAULT '0',
  `genitive` varchar(12) NOT NULL DEFAULT '',
  `dative` varchar(12) NOT NULL DEFAULT '',
  `accusative` varchar(12) NOT NULL DEFAULT '',
  `instrumental` varchar(12) NOT NULL DEFAULT '',
  `prepositional` varchar(12) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  KEY `owner_key` (`owner`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_pet_declinedname` */

/*Table structure for table `character_queststatus` */

DROP TABLE IF EXISTS `character_queststatus`;

CREATE TABLE `character_queststatus` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Quest Identifier',
  `status` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `explored` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `timer` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`quest`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `character_queststatus` */

insert  into `character_queststatus`(`guid`,`quest`,`status`,`explored`,`timer`) values (2,9280,3,0,1509780394);

/*Table structure for table `character_queststatus_daily` */

DROP TABLE IF EXISTS `character_queststatus_daily`;

CREATE TABLE `character_queststatus_daily` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Quest Identifier',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`quest`),
  KEY `idx_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `character_queststatus_daily` */

/*Table structure for table `character_queststatus_monthly` */

DROP TABLE IF EXISTS `character_queststatus_monthly`;

CREATE TABLE `character_queststatus_monthly` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Quest Identifier',
  PRIMARY KEY (`guid`,`quest`),
  KEY `idx_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `character_queststatus_monthly` */

/*Table structure for table `character_queststatus_objective` */

DROP TABLE IF EXISTS `character_queststatus_objective`;

CREATE TABLE `character_queststatus_objective` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `objectiveId` int(10) unsigned NOT NULL DEFAULT '0',
  `amount` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`objectiveId`,`guid`),
  KEY `guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_queststatus_objective` */

insert  into `character_queststatus_objective`(`guid`,`objectiveId`,`amount`) values (2,255848,0);

/*Table structure for table `character_queststatus_rewarded` */

DROP TABLE IF EXISTS `character_queststatus_rewarded`;

CREATE TABLE `character_queststatus_rewarded` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Quest Identifier',
  PRIMARY KEY (`guid`,`quest`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `character_queststatus_rewarded` */

insert  into `character_queststatus_rewarded`(`guid`,`quest`) values (2,9279);

/*Table structure for table `character_queststatus_seasonal` */

DROP TABLE IF EXISTS `character_queststatus_seasonal`;

CREATE TABLE `character_queststatus_seasonal` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Quest Identifier',
  `event` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Event Identifier',
  PRIMARY KEY (`guid`,`quest`),
  KEY `idx_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `character_queststatus_seasonal` */

/*Table structure for table `character_queststatus_weekly` */

DROP TABLE IF EXISTS `character_queststatus_weekly`;

CREATE TABLE `character_queststatus_weekly` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `quest` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Quest Identifier',
  PRIMARY KEY (`guid`,`quest`),
  KEY `idx_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `character_queststatus_weekly` */

/*Table structure for table `character_rates` */

DROP TABLE IF EXISTS `character_rates`;

CREATE TABLE `character_rates` (
  `guid` bigint(20) NOT NULL DEFAULT '0',
  `rate` tinyint(2) DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `character_rates` */

/*Table structure for table `character_reputation` */

DROP TABLE IF EXISTS `character_reputation`;

CREATE TABLE `character_reputation` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `faction` smallint(5) unsigned NOT NULL DEFAULT '0',
  `standing` int(11) NOT NULL DEFAULT '0',
  `flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`faction`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `character_reputation` */

insert  into `character_reputation`(`guid`,`faction`,`standing`,`flags`) values (2,21,0,64),(2,46,0,4),(2,47,54,17),(2,54,54,17),(2,59,0,16),(2,67,0,14),(2,68,0,6),(2,69,54,17),(2,70,0,2),(2,72,54,17),(2,76,0,6),(2,81,0,6),(2,83,0,4),(2,86,0,4),(2,87,0,2),(2,92,0,2),(2,93,0,2),(2,169,0,12),(2,270,0,16),(2,289,0,4),(2,349,0,0),(2,369,0,64),(2,469,0,25),(2,470,0,64),(2,471,0,20),(2,509,0,16),(2,510,0,2),(2,529,0,0),(2,530,0,6),(2,549,0,4),(2,550,0,4),(2,551,0,4),(2,569,0,4),(2,570,0,4),(2,571,0,4),(2,574,0,4),(2,576,0,0),(2,577,0,64),(2,589,0,0),(2,609,0,0),(2,729,0,6),(2,730,0,16),(2,749,0,0),(2,809,0,16),(2,889,0,6),(2,890,0,16),(2,891,0,24),(2,892,0,14),(2,909,0,16),(2,910,0,0),(2,911,0,6),(2,922,0,6),(2,930,225,17),(2,932,0,80),(2,933,0,16),(2,934,0,80),(2,935,0,16),(2,936,0,28),(2,941,0,6),(2,942,0,16),(2,946,0,16),(2,947,0,0),(2,948,0,8),(2,949,0,24),(2,952,0,0),(2,953,0,4),(2,967,0,16),(2,970,0,0),(2,978,0,16),(2,980,0,24),(2,989,0,16),(2,990,0,16),(2,1005,0,20),(2,1011,0,16),(2,1012,0,16),(2,1015,0,2),(2,1031,0,16),(2,1037,0,136),(2,1038,0,16),(2,1050,0,16),(2,1052,0,0),(2,1064,0,6),(2,1067,0,0),(2,1068,0,16),(2,1073,0,16),(2,1077,0,16),(2,1082,0,4),(2,1085,0,6),(2,1090,0,16),(2,1091,0,16),(2,1094,0,16),(2,1097,0,0),(2,1098,0,16),(2,1104,0,16),(2,1105,0,16),(2,1106,0,16),(2,1117,0,12),(2,1118,0,12),(2,1119,0,0),(2,1124,0,6),(2,1126,0,16),(2,1133,0,6),(2,1134,54,17),(2,1135,0,16),(2,1136,0,4),(2,1137,0,4),(2,1138,0,0),(2,1154,0,4),(2,1155,0,4),(2,1156,0,16),(2,1158,0,16),(2,1162,0,8),(2,1163,0,0),(2,1168,0,16),(2,1169,0,28),(2,1171,0,64),(2,1172,0,0),(2,1173,0,16),(2,1174,0,16),(2,1177,0,0),(2,1178,0,6),(2,1204,0,16),(2,1216,0,0),(2,1228,0,2),(2,1242,0,16),(2,1245,0,8),(2,1269,0,16),(2,1270,0,16),(2,1271,0,16),(2,1272,0,152),(2,1273,0,16),(2,1275,0,16),(2,1276,0,16),(2,1277,0,16),(2,1278,0,16),(2,1279,0,16),(2,1280,0,16),(2,1281,0,16),(2,1282,0,16),(2,1283,0,16),(2,1302,0,152),(2,1337,0,16),(2,1341,0,16),(2,1345,0,16),(2,1351,0,0),(2,1352,0,6),(2,1353,0,17),(2,1357,0,4),(2,1358,0,16),(2,1359,0,16),(2,1374,0,0),(2,1375,0,2),(2,1376,0,144),(2,1387,0,144),(2,1388,0,2),(2,1416,0,0),(2,1419,0,16),(2,1433,0,20),(2,1435,0,16),(2,1440,0,16),(2,1444,0,24),(2,1445,0,2),(2,1492,0,16),(2,1515,0,16),(2,1679,0,0),(2,1681,0,2),(2,1682,0,0),(2,1690,0,0),(2,1691,0,16),(2,1708,0,2),(2,1710,0,16),(2,1711,0,16),(2,1712,0,4),(2,1713,0,4),(2,1714,0,4),(2,1715,0,4),(2,1716,0,4),(2,1717,0,4),(2,1718,0,4),(2,1731,0,16),(2,1732,0,4),(2,1733,0,16),(2,1735,0,0),(2,1736,0,16),(2,1737,0,16),(2,1738,0,16),(2,1739,0,0),(2,1740,0,0),(2,1741,0,16),(2,1847,0,16),(2,1848,0,2),(2,1849,0,16),(2,1850,0,16);

/*Table structure for table `character_skills` */

DROP TABLE IF EXISTS `character_skills`;

CREATE TABLE `character_skills` (
  `guid` int(10) unsigned NOT NULL COMMENT 'Global Unique Identifier',
  `skill` smallint(5) unsigned NOT NULL,
  `value` smallint(5) unsigned NOT NULL,
  `max` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`guid`,`skill`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `character_skills` */

insert  into `character_skills`(`guid`,`skill`,`value`,`max`) values (2,43,1,5),(2,44,1,5),(2,54,1,5),(2,55,1,5),(2,95,1,5),(2,98,300,300),(2,160,1,5),(2,162,1,5),(2,172,1,5),(2,188,1,1),(2,189,1,1),(2,204,1,1),(2,205,1,1),(2,206,1,1),(2,207,1,1),(2,229,1,5),(2,270,1,1),(2,293,1,1),(2,413,1,1),(2,414,1,1),(2,415,1,1),(2,433,1,1),(2,759,300,300),(2,761,1,1),(2,782,1,1),(2,800,1,1),(2,805,1,1),(2,927,1,1),(2,962,1,1),(2,963,1,1),(2,1748,1,1),(2,1777,1,1),(2,1981,1,1),(2,1982,1,1);

/*Table structure for table `character_spell` */

DROP TABLE IF EXISTS `character_spell`;

CREATE TABLE `character_spell` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Spell Identifier',
  `active` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `disabled` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `IsMountFavorite` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `character_spell` */

insert  into `character_spell`(`guid`,`spell`,`active`,`disabled`,`IsMountFavorite`) values (2,750,1,0,0),(2,111621,1,0,0),(2,113873,1,0,0),(2,134732,1,0,0),(2,137026,1,0,0);

/*Table structure for table `character_spell_charges` */

DROP TABLE IF EXISTS `character_spell_charges`;

CREATE TABLE `character_spell_charges` (
  `guid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier, Low part',
  `categoryId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'SpellCategory.dbc Identifier',
  `rechargeStart` int(10) unsigned NOT NULL DEFAULT '0',
  `rechargeEnd` int(10) unsigned NOT NULL DEFAULT '0',
  KEY `idx_guid` (`guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_spell_charges` */

/*Table structure for table `character_spell_cooldown` */

DROP TABLE IF EXISTS `character_spell_cooldown`;

CREATE TABLE `character_spell_cooldown` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier, Low part',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Spell Identifier',
  `item` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Item Identifier',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_spell_cooldown` */

insert  into `character_spell_cooldown`(`guid`,`spell`,`item`,`time`) values (2,59542,0,1509780534);

/*Table structure for table `character_stats` */

DROP TABLE IF EXISTS `character_stats`;

CREATE TABLE `character_stats` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier, Low part',
  `maxhealth` int(10) unsigned NOT NULL DEFAULT '0',
  `maxpower1` int(10) unsigned NOT NULL DEFAULT '0',
  `maxpower2` int(10) unsigned NOT NULL DEFAULT '0',
  `maxpower3` int(10) unsigned NOT NULL DEFAULT '0',
  `maxpower4` int(10) unsigned NOT NULL DEFAULT '0',
  `maxpower5` int(10) unsigned NOT NULL DEFAULT '0',
  `maxpower6` int(10) unsigned NOT NULL DEFAULT '0',
  `strength` int(10) unsigned NOT NULL DEFAULT '0',
  `agility` int(10) unsigned NOT NULL DEFAULT '0',
  `stamina` int(10) unsigned NOT NULL DEFAULT '0',
  `intellect` int(10) unsigned NOT NULL DEFAULT '0',
  `spirit` int(10) unsigned NOT NULL DEFAULT '0',
  `armor` int(10) unsigned NOT NULL DEFAULT '0',
  `resHoly` int(10) unsigned NOT NULL DEFAULT '0',
  `resFire` int(10) unsigned NOT NULL DEFAULT '0',
  `resNature` int(10) unsigned NOT NULL DEFAULT '0',
  `resFrost` int(10) unsigned NOT NULL DEFAULT '0',
  `resShadow` int(10) unsigned NOT NULL DEFAULT '0',
  `resArcane` int(10) unsigned NOT NULL DEFAULT '0',
  `blockPct` float unsigned NOT NULL DEFAULT '0',
  `dodgePct` float unsigned NOT NULL DEFAULT '0',
  `parryPct` float unsigned NOT NULL DEFAULT '0',
  `critPct` float unsigned NOT NULL DEFAULT '0',
  `rangedCritPct` float unsigned NOT NULL DEFAULT '0',
  `spellCritPct` float unsigned NOT NULL DEFAULT '0',
  `attackPower` int(10) unsigned NOT NULL DEFAULT '0',
  `rangedAttackPower` int(10) unsigned NOT NULL DEFAULT '0',
  `spellPower` int(10) unsigned NOT NULL DEFAULT '0',
  `resilience` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_stats` */

/*Table structure for table `character_stats_wod` */

DROP TABLE IF EXISTS `character_stats_wod`;

CREATE TABLE `character_stats_wod` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier, Low part',
  `strength` int(10) unsigned NOT NULL DEFAULT '0',
  `agility` int(10) unsigned NOT NULL DEFAULT '0',
  `stamina` int(10) unsigned NOT NULL DEFAULT '0',
  `intellect` int(10) unsigned NOT NULL DEFAULT '0',
  `critPct` float unsigned NOT NULL DEFAULT '0',
  `haste` float unsigned NOT NULL DEFAULT '0',
  `mastery` float unsigned NOT NULL DEFAULT '0',
  `spirit` int(10) unsigned NOT NULL DEFAULT '0',
  `armorBonus` int(10) unsigned NOT NULL DEFAULT '0',
  `multistrike` float unsigned NOT NULL DEFAULT '0',
  `leech` float unsigned NOT NULL DEFAULT '0',
  `versatility` float unsigned NOT NULL DEFAULT '0',
  `avoidance` float unsigned NOT NULL DEFAULT '0',
  `attackDamage` varchar(255) NOT NULL,
  `attackPower` int(10) unsigned NOT NULL DEFAULT '0',
  `attackSpeed` float unsigned NOT NULL DEFAULT '0',
  `spellPower` int(10) unsigned NOT NULL DEFAULT '0',
  `manaRegen` int(10) unsigned NOT NULL DEFAULT '0',
  `armor` int(10) unsigned NOT NULL DEFAULT '0',
  `dodgePct` float unsigned NOT NULL DEFAULT '0',
  `parryPct` float unsigned NOT NULL DEFAULT '0',
  `blockPct` float unsigned NOT NULL DEFAULT '0',
  `ilvl` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_stats_wod` */

insert  into `character_stats_wod`(`guid`,`strength`,`agility`,`stamina`,`intellect`,`critPct`,`haste`,`mastery`,`spirit`,`armorBonus`,`multistrike`,`leech`,`versatility`,`avoidance`,`attackDamage`,`attackPower`,`attackSpeed`,`spellPower`,`manaRegen`,`armor`,`dodgePct`,`parryPct`,`blockPct`,`ilvl`) values (2,19,3,11,13,5,0,0,11,0,0,0,0,0,'16.7429 - 17.7429',19,2.9,13,0,8,3.0064,3.01249,3,0);

/*Table structure for table `character_talent` */

DROP TABLE IF EXISTS `character_talent`;

CREATE TABLE `character_talent` (
  `guid` int(10) unsigned NOT NULL,
  `spell` mediumint(8) unsigned NOT NULL,
  `spec` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spell`,`spec`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_talent` */

/*Table structure for table `character_template` */

DROP TABLE IF EXISTS `character_template`;

CREATE TABLE `character_template` (
  `id` int(10) unsigned NOT NULL DEFAULT '1',
  `class` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `name` varchar(256) NOT NULL,
  `description` varchar(2048) NOT NULL,
  `level` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `money` bigint(20) unsigned NOT NULL DEFAULT '0',
  `alianceX` float DEFAULT '0',
  `alianceY` float NOT NULL DEFAULT '0',
  `alianceZ` float NOT NULL DEFAULT '0',
  `alianceO` float NOT NULL DEFAULT '0',
  `alianceMap` smallint(6) NOT NULL DEFAULT '0',
  `hordeX` float NOT NULL DEFAULT '0',
  `hordeY` float NOT NULL DEFAULT '0',
  `hordeZ` float NOT NULL DEFAULT '0',
  `hordeO` float NOT NULL DEFAULT '0',
  `hordeMap` smallint(6) NOT NULL DEFAULT '0',
  `disabled` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `allianceDefaultRace` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `hordeDefaultRace` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_template` */

/*Table structure for table `character_void_storage` */

DROP TABLE IF EXISTS `character_void_storage`;

CREATE TABLE `character_void_storage` (
  `itemId` bigint(20) unsigned NOT NULL,
  `playerGuid` int(10) unsigned NOT NULL,
  `itemEntry` mediumint(8) unsigned NOT NULL,
  `slot` tinyint(3) unsigned NOT NULL,
  `creatorGuid` int(10) unsigned NOT NULL DEFAULT '0',
  `randomProperty` int(10) unsigned NOT NULL DEFAULT '0',
  `suffixFactor` int(10) unsigned NOT NULL DEFAULT '0',
  `bonuses` text NOT NULL,
  PRIMARY KEY (`itemId`),
  UNIQUE KEY `idx_player_slot` (`playerGuid`,`slot`),
  KEY `idx_player` (`playerGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_void_storage` */

/*Table structure for table `character_worldstates` */

DROP TABLE IF EXISTS `character_worldstates`;

CREATE TABLE `character_worldstates` (
  `guid` int(10) unsigned NOT NULL,
  `worldstate` int(10) unsigned NOT NULL,
  `value` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`guid`,`worldstate`),
  UNIQUE KEY `unique` (`guid`,`worldstate`),
  KEY `guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `character_worldstates` */

/*Table structure for table `characters` */

DROP TABLE IF EXISTS `characters`;

CREATE TABLE `characters` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `account` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Account Identifier',
  `name` varchar(12) NOT NULL DEFAULT '',
  `slot` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `race` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `class` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `gender` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `xp` int(10) unsigned NOT NULL DEFAULT '0',
  `money` bigint(20) unsigned NOT NULL DEFAULT '0',
  `playerBytes` int(10) unsigned NOT NULL DEFAULT '0',
  `playerBytes2` int(10) unsigned NOT NULL DEFAULT '0',
  `playerFlags` int(10) unsigned NOT NULL DEFAULT '0',
  `playerFlagsEx` int(10) unsigned NOT NULL DEFAULT '0',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `map` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Map Identifier',
  `instance_id` int(10) unsigned NOT NULL DEFAULT '0',
  `DungeonDifficulty` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `taximask` text NOT NULL,
  `online` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `cinematic` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `totaltime` int(10) unsigned NOT NULL DEFAULT '0',
  `leveltime` int(10) unsigned NOT NULL DEFAULT '0',
  `logout_time` int(10) NOT NULL DEFAULT '0',
  `is_logout_resting` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `rest_bonus` float NOT NULL DEFAULT '0',
  `resettalents_cost` int(10) unsigned NOT NULL DEFAULT '0',
  `resettalents_time` int(10) unsigned NOT NULL DEFAULT '0',
  `resetspecialization_cost` int(10) NOT NULL DEFAULT '0',
  `resetspecialization_time` int(10) NOT NULL DEFAULT '0',
  `talentTree` varchar(10) NOT NULL DEFAULT '0 0',
  `trans_x` float NOT NULL DEFAULT '0',
  `trans_y` float NOT NULL DEFAULT '0',
  `trans_z` float NOT NULL DEFAULT '0',
  `trans_o` float NOT NULL DEFAULT '0',
  `transguid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `extra_flags` smallint(5) unsigned NOT NULL DEFAULT '0',
  `stable_slots` tinyint(3) unsigned DEFAULT NULL,
  `at_login` smallint(5) unsigned NOT NULL DEFAULT '0',
  `zone` smallint(5) unsigned NOT NULL DEFAULT '0',
  `death_expire_time` int(10) unsigned NOT NULL DEFAULT '0',
  `taxi_path` text,
  `totalKills` int(10) unsigned NOT NULL DEFAULT '0',
  `todayKills` smallint(5) unsigned NOT NULL DEFAULT '0',
  `yesterdayKills` smallint(5) unsigned NOT NULL DEFAULT '0',
  `chosenTitle` int(10) unsigned NOT NULL DEFAULT '0',
  `watchedFaction` int(10) unsigned NOT NULL DEFAULT '0',
  `drunk` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `health` int(10) unsigned NOT NULL DEFAULT '0',
  `power1` int(10) unsigned NOT NULL DEFAULT '0',
  `power2` int(10) unsigned NOT NULL DEFAULT '0',
  `power3` int(10) unsigned NOT NULL DEFAULT '0',
  `power4` int(10) unsigned NOT NULL DEFAULT '0',
  `power5` int(10) unsigned NOT NULL DEFAULT '0',
  `power6` int(10) unsigned NOT NULL DEFAULT '0',
  `latency` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `speccount` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `activespec` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `specialization1` int(11) NOT NULL,
  `specialization2` int(11) NOT NULL,
  `exploredZones` longtext,
  `equipmentCache` longtext,
  `knownTitles` longtext,
  `actionBars` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `currentpetslot` tinyint(3) unsigned DEFAULT NULL,
  `petslotused` int(10) unsigned DEFAULT '0',
  `grantableLevels` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `guildId` int(10) unsigned NOT NULL DEFAULT '0',
  `deleteInfos_Account` int(10) unsigned DEFAULT NULL,
  `deleteInfos_Name` varchar(12) DEFAULT NULL,
  `deleteDate` int(10) unsigned DEFAULT NULL,
  `RaidDifficulty` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `LegacyRaidDifficuly` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `xprate` float unsigned NOT NULL DEFAULT '1',
  `lastbattlepet` int(10) unsigned NOT NULL DEFAULT '0',
  `ilvl` int(10) unsigned NOT NULL DEFAULT '0',
  `end_sales` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`),
  KEY `idx_account` (`account`),
  KEY `idx_online` (`online`),
  KEY `idx_name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `characters` */

insert  into `characters`(`guid`,`account`,`name`,`slot`,`race`,`class`,`gender`,`level`,`xp`,`money`,`playerBytes`,`playerBytes2`,`playerFlags`,`playerFlagsEx`,`position_x`,`position_y`,`position_z`,`map`,`instance_id`,`DungeonDifficulty`,`orientation`,`taximask`,`online`,`cinematic`,`totaltime`,`leveltime`,`logout_time`,`is_logout_resting`,`rest_bonus`,`resettalents_cost`,`resettalents_time`,`resetspecialization_cost`,`resetspecialization_time`,`talentTree`,`trans_x`,`trans_y`,`trans_z`,`trans_o`,`transguid`,`extra_flags`,`stable_slots`,`at_login`,`zone`,`death_expire_time`,`taxi_path`,`totalKills`,`todayKills`,`yesterdayKills`,`chosenTitle`,`watchedFaction`,`drunk`,`health`,`power1`,`power2`,`power3`,`power4`,`power5`,`power6`,`latency`,`speccount`,`activespec`,`specialization1`,`specialization2`,`exploredZones`,`equipmentCache`,`knownTitles`,`actionBars`,`currentpetslot`,`petslotused`,`grantableLevels`,`guildId`,`deleteInfos_Account`,`deleteInfos_Name`,`deleteDate`,`RaidDifficulty`,`LegacyRaidDifficuly`,`xprate`,`lastbattlepet`,`ilvl`,`end_sales`) values (2,5,'Варитис',0,11,2,1,1,160,0,67840,33554433,0,0,-4009.62,-13730.4,68.5324,530,0,1,2.89417,'34 0 0 6 0 0 1 0 0 0 0 32 8 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 128 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 32 16 0 0 0 140 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ',0,1,94,94,1509780394,0,0.01085,0,0,0,0,'0 0 ',0,0,0,0,0,4,0,0,6456,0,'',0,0,0,0,4294967295,0,154,28,0,0,0,0,0,0,1,0,0,0,'0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3221225472 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ','0 0 0 0 0 0 0 0 0 0 0 0 58242 36133 0 0 0 0 23477 36134 0 52533 36135 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2361 2776 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ','0 0 0 0 0 0 0 0 0 0 0 0 ',0,255,0,0,0,NULL,NULL,NULL,14,3,1,0,0,0);

/*Table structure for table `characters_boss_looted` */

DROP TABLE IF EXISTS `characters_boss_looted`;

CREATE TABLE `characters_boss_looted` (
  `guid` int(10) unsigned NOT NULL,
  `boss_entry` int(10) unsigned NOT NULL,
  `boss_model_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`guid`,`boss_entry`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `characters_boss_looted` */

/*Table structure for table `corpse` */

DROP TABLE IF EXISTS `corpse`;

CREATE TABLE `corpse` (
  `corpseGuid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character Global Unique Identifier',
  `posX` float NOT NULL DEFAULT '0',
  `posY` float NOT NULL DEFAULT '0',
  `posZ` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `mapId` smallint(5) unsigned NOT NULL DEFAULT '0' COMMENT 'Map Identifier',
  `phaseMask` smallint(5) unsigned NOT NULL DEFAULT '1',
  `displayId` int(10) unsigned NOT NULL DEFAULT '0',
  `itemCache` text NOT NULL,
  `bytes1` int(10) unsigned NOT NULL DEFAULT '0',
  `bytes2` int(10) unsigned NOT NULL DEFAULT '0',
  `flags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `dynFlags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  `corpseType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `instanceId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Instance Identifier',
  PRIMARY KEY (`corpseGuid`),
  KEY `idx_type` (`corpseType`),
  KEY `idx_instance` (`instanceId`),
  KEY `idx_player` (`guid`),
  KEY `idx_time` (`time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Death System';

/*Data for the table `corpse` */

/*Table structure for table `creature_respawn` */

DROP TABLE IF EXISTS `creature_respawn`;

CREATE TABLE `creature_respawn` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `respawnTime` int(10) unsigned NOT NULL DEFAULT '0',
  `mapId` smallint(10) unsigned NOT NULL DEFAULT '0',
  `instanceId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Instance Identifier',
  PRIMARY KEY (`guid`,`instanceId`),
  KEY `idx_instance` (`instanceId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Grid Loading System';

/*Data for the table `creature_respawn` */

insert  into `creature_respawn`(`guid`,`respawnTime`,`mapId`,`instanceId`) values (30310,1509780720,530,0),(30349,1509780729,530,0);

/*Table structure for table `cuf_profile` */

DROP TABLE IF EXISTS `cuf_profile`;

CREATE TABLE `cuf_profile` (
  `guid` int(10) unsigned NOT NULL,
  `name` varchar(128) NOT NULL,
  `data` tinyblob,
  PRIMARY KEY (`guid`,`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `cuf_profile` */

insert  into `cuf_profile`(`guid`,`name`,`data`) values (2,'Основной','�<\0($\0H');

/*Table structure for table `daily_players_reports` */

DROP TABLE IF EXISTS `daily_players_reports`;

CREATE TABLE `daily_players_reports` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `creation_time` int(10) unsigned NOT NULL DEFAULT '0',
  `average` float NOT NULL DEFAULT '0',
  `total_reports` bigint(20) unsigned NOT NULL DEFAULT '0',
  `speed_reports` bigint(20) unsigned NOT NULL DEFAULT '0',
  `fly_reports` bigint(20) unsigned NOT NULL DEFAULT '0',
  `jump_reports` bigint(20) unsigned NOT NULL DEFAULT '0',
  `waterwalk_reports` bigint(20) unsigned NOT NULL DEFAULT '0',
  `teleportplane_reports` bigint(20) unsigned NOT NULL DEFAULT '0',
  `climb_reports` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `daily_players_reports` */

/*Table structure for table `encounter_damage_log` */

DROP TABLE IF EXISTS `encounter_damage_log`;

CREATE TABLE `encounter_damage_log` (
  `encounterId` int(11) unsigned NOT NULL,
  `encounterStartTime` bigint(20) unsigned NOT NULL,
  `logTime` bigint(20) unsigned NOT NULL,
  `attackerGuid` int(11) unsigned NOT NULL,
  `damage` int(11) unsigned NOT NULL,
  `spellId` int(11) unsigned NOT NULL,
  KEY `encounterId` (`encounterId`,`encounterStartTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `encounter_damage_log` */

/*Table structure for table `encounter_group_dump` */

DROP TABLE IF EXISTS `encounter_group_dump`;

CREATE TABLE `encounter_group_dump` (
  `encounterId` int(11) unsigned NOT NULL,
  `encounterStartTime` bigint(20) unsigned NOT NULL,
  `dumpTime` bigint(20) unsigned NOT NULL,
  `dump` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `encounter_group_dump` */

/*Table structure for table `game_event_condition_save` */

DROP TABLE IF EXISTS `game_event_condition_save`;

CREATE TABLE `game_event_condition_save` (
  `eventEntry` tinyint(3) unsigned NOT NULL,
  `condition_id` int(10) unsigned NOT NULL DEFAULT '0',
  `done` float DEFAULT '0',
  PRIMARY KEY (`eventEntry`,`condition_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `game_event_condition_save` */

/*Table structure for table `game_event_save` */

DROP TABLE IF EXISTS `game_event_save`;

CREATE TABLE `game_event_save` (
  `eventEntry` tinyint(3) unsigned NOT NULL,
  `state` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `next_start` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`eventEntry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `game_event_save` */

/*Table structure for table `gameobject_respawn` */

DROP TABLE IF EXISTS `gameobject_respawn`;

CREATE TABLE `gameobject_respawn` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `respawnTime` int(10) unsigned NOT NULL DEFAULT '0',
  `mapId` smallint(10) unsigned NOT NULL DEFAULT '0',
  `instanceId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Instance Identifier',
  PRIMARY KEY (`guid`,`instanceId`),
  KEY `idx_instance` (`instanceId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Grid Loading System';

/*Data for the table `gameobject_respawn` */

/*Table structure for table `gm_subsurveys` */

DROP TABLE IF EXISTS `gm_subsurveys`;

CREATE TABLE `gm_subsurveys` (
  `surveyId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `subsurveyId` int(10) unsigned NOT NULL DEFAULT '0',
  `rank` int(10) unsigned NOT NULL DEFAULT '0',
  `comment` text NOT NULL,
  PRIMARY KEY (`surveyId`,`subsurveyId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `gm_subsurveys` */

/*Table structure for table `gm_surveys` */

DROP TABLE IF EXISTS `gm_surveys`;

CREATE TABLE `gm_surveys` (
  `surveyId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `mainSurvey` int(10) unsigned NOT NULL DEFAULT '0',
  `overallComment` longtext NOT NULL,
  `createTime` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`surveyId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `gm_surveys` */

/*Table structure for table `gm_tickets` */

DROP TABLE IF EXISTS `gm_tickets`;

CREATE TABLE `gm_tickets` (
  `ticketId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier of ticket creator',
  `name` varchar(12) NOT NULL COMMENT 'Name of ticket creator',
  `message` text NOT NULL,
  `createTime` int(10) unsigned NOT NULL DEFAULT '0',
  `mapId` int(10) unsigned NOT NULL DEFAULT '0',
  `posX` float NOT NULL DEFAULT '0',
  `posY` float NOT NULL DEFAULT '0',
  `posZ` float NOT NULL DEFAULT '0',
  `lastModifiedTime` int(10) unsigned NOT NULL DEFAULT '0',
  `closedBy` int(10) NOT NULL DEFAULT '0',
  `assignedTo` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'GUID of admin to whom ticket is assigned',
  `comment` text NOT NULL,
  `completed` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `escalated` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `viewed` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `haveTicket` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `response` text,
  PRIMARY KEY (`ticketId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `gm_tickets` */

/*Table structure for table `group_completed_challenges` */

DROP TABLE IF EXISTS `group_completed_challenges`;

CREATE TABLE `group_completed_challenges` (
  `map_id` int(10) unsigned NOT NULL,
  `attempt_id` int(10) unsigned NOT NULL DEFAULT '0',
  `completion_time` int(10) unsigned NOT NULL DEFAULT '0',
  `completion_date` int(10) unsigned NOT NULL DEFAULT '0',
  `medal_earned` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `group_members` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `group_1_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `group_1_spec` int(10) unsigned NOT NULL DEFAULT '0',
  `group_2_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `group_2_spec` int(10) unsigned NOT NULL DEFAULT '0',
  `group_3_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `group_3_spec` int(10) unsigned NOT NULL DEFAULT '0',
  `group_4_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `group_4_spec` int(10) unsigned NOT NULL DEFAULT '0',
  `group_5_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `group_5_spec` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`map_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `group_completed_challenges` */

/*Table structure for table `group_instance` */

DROP TABLE IF EXISTS `group_instance`;

CREATE TABLE `group_instance` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `instance` int(10) unsigned NOT NULL DEFAULT '0',
  `permanent` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`instance`),
  KEY `instance` (`instance`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `group_instance` */

/*Table structure for table `group_member` */

DROP TABLE IF EXISTS `group_member`;

CREATE TABLE `group_member` (
  `guid` int(10) unsigned NOT NULL,
  `memberGuid` int(10) unsigned NOT NULL,
  `memberFlags` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `subgroup` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `roles` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `class` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `specId` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`memberGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Groups';

/*Data for the table `group_member` */

/*Table structure for table `groups` */

DROP TABLE IF EXISTS `groups`;

CREATE TABLE `groups` (
  `guid` int(10) unsigned NOT NULL,
  `leaderGuid` int(10) unsigned NOT NULL,
  `lootMethod` tinyint(3) unsigned NOT NULL,
  `looterGuid` int(10) unsigned NOT NULL,
  `lootThreshold` tinyint(3) unsigned NOT NULL,
  `icon1` int(10) unsigned NOT NULL,
  `icon2` int(10) unsigned NOT NULL,
  `icon3` int(10) unsigned NOT NULL,
  `icon4` int(10) unsigned NOT NULL,
  `icon5` int(10) unsigned NOT NULL,
  `icon6` int(10) unsigned NOT NULL,
  `icon7` int(10) unsigned NOT NULL,
  `icon8` int(10) unsigned NOT NULL,
  `groupType` tinyint(3) unsigned NOT NULL,
  `difficulty` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `raiddifficulty` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `legacyraiddifficulty` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`),
  KEY `leaderGuid` (`leaderGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Groups';

/*Data for the table `groups` */

/*Table structure for table `guild` */

DROP TABLE IF EXISTS `guild`;

CREATE TABLE `guild` (
  `guildid` int(10) unsigned NOT NULL DEFAULT '0',
  `name` varchar(24) NOT NULL DEFAULT '',
  `leaderguid` int(10) unsigned NOT NULL DEFAULT '0',
  `EmblemStyle` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `EmblemColor` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BorderStyle` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BorderColor` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BackgroundColor` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `info` text NOT NULL,
  `motd` varchar(128) NOT NULL DEFAULT '',
  `createdate` int(10) unsigned NOT NULL DEFAULT '0',
  `BankMoney` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

/*Data for the table `guild` */

/*Table structure for table `guild_achievement` */

DROP TABLE IF EXISTS `guild_achievement`;

CREATE TABLE `guild_achievement` (
  `guildId` int(10) unsigned NOT NULL,
  `achievement` smallint(5) unsigned NOT NULL,
  `date` int(10) unsigned NOT NULL DEFAULT '0',
  `guids` text NOT NULL,
  PRIMARY KEY (`guildId`,`achievement`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `guild_achievement` */

/*Table structure for table `guild_achievement_progress` */

DROP TABLE IF EXISTS `guild_achievement_progress`;

CREATE TABLE `guild_achievement_progress` (
  `guildId` int(10) unsigned NOT NULL,
  `criteria` smallint(5) unsigned NOT NULL,
  `counter` int(10) unsigned NOT NULL,
  `date` int(10) unsigned NOT NULL DEFAULT '0',
  `completedGuid` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildId`,`criteria`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `guild_achievement_progress` */

/*Table structure for table `guild_bank_eventlog` */

DROP TABLE IF EXISTS `guild_bank_eventlog`;

CREATE TABLE `guild_bank_eventlog` (
  `guildid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Guild Identificator',
  `LogGuid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Log record identificator - auxiliary column',
  `TabId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Guild bank TabId',
  `EventType` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Event type',
  `PlayerGuid` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemOrMoney` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemStackCount` smallint(5) unsigned NOT NULL DEFAULT '0',
  `DestTabId` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Destination Tab Id',
  `TimeStamp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Event UNIX time',
  PRIMARY KEY (`guildid`,`LogGuid`,`TabId`),
  KEY `guildid_key` (`guildid`),
  KEY `Idx_PlayerGuid` (`PlayerGuid`),
  KEY `Idx_LogGuid` (`LogGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `guild_bank_eventlog` */

/*Table structure for table `guild_bank_item` */

DROP TABLE IF EXISTS `guild_bank_item`;

CREATE TABLE `guild_bank_item` (
  `guildid` int(10) unsigned NOT NULL DEFAULT '0',
  `TabId` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SlotId` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `item_guid` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildid`,`TabId`,`SlotId`),
  KEY `guildid_key` (`guildid`),
  KEY `Idx_item_guid` (`item_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `guild_bank_item` */

/*Table structure for table `guild_bank_right` */

DROP TABLE IF EXISTS `guild_bank_right`;

CREATE TABLE `guild_bank_right` (
  `guildid` int(10) unsigned NOT NULL DEFAULT '0',
  `TabId` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `rid` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `gbright` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `SlotPerDay` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildid`,`TabId`,`rid`),
  KEY `guildid_key` (`guildid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `guild_bank_right` */

/*Table structure for table `guild_bank_tab` */

DROP TABLE IF EXISTS `guild_bank_tab`;

CREATE TABLE `guild_bank_tab` (
  `guildid` int(10) unsigned NOT NULL DEFAULT '0',
  `TabId` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `TabName` varchar(16) NOT NULL DEFAULT '',
  `TabIcon` varchar(100) NOT NULL DEFAULT '',
  `TabText` varchar(500) DEFAULT NULL,
  PRIMARY KEY (`guildid`,`TabId`),
  KEY `guildid_key` (`guildid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `guild_bank_tab` */

/*Table structure for table `guild_challenges` */

DROP TABLE IF EXISTS `guild_challenges`;

CREATE TABLE `guild_challenges` (
  `GuildId` int(10) unsigned NOT NULL,
  `ChallengeType` int(1) NOT NULL DEFAULT '0',
  `ChallengeCount` int(1) DEFAULT NULL,
  PRIMARY KEY (`GuildId`,`ChallengeType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `guild_challenges` */

/*Table structure for table `guild_completed_challenges` */

DROP TABLE IF EXISTS `guild_completed_challenges`;

CREATE TABLE `guild_completed_challenges` (
  `map_id` int(10) unsigned NOT NULL,
  `guild_id` int(10) unsigned NOT NULL,
  `attempt_id` int(10) unsigned NOT NULL DEFAULT '0',
  `completion_time` int(10) unsigned NOT NULL DEFAULT '0',
  `completion_date` int(10) unsigned NOT NULL DEFAULT '0',
  `medal_earned` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `guild_members` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `guild_1_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `guild_1_spec` int(10) unsigned NOT NULL DEFAULT '0',
  `guild_2_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `guild_2_spec` int(10) unsigned NOT NULL DEFAULT '0',
  `guild_3_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `guild_3_spec` int(10) unsigned NOT NULL DEFAULT '0',
  `guild_4_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `guild_4_spec` int(10) unsigned NOT NULL DEFAULT '0',
  `guild_5_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `guild_5_spec` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`map_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `guild_completed_challenges` */

/*Table structure for table `guild_eventlog` */

DROP TABLE IF EXISTS `guild_eventlog`;

CREATE TABLE `guild_eventlog` (
  `guildid` int(10) unsigned NOT NULL COMMENT 'Guild Identificator',
  `LogGuid` int(10) unsigned NOT NULL COMMENT 'Log record identificator - auxiliary column',
  `EventType` tinyint(3) unsigned NOT NULL COMMENT 'Event type',
  `PlayerGuid1` int(10) unsigned NOT NULL COMMENT 'Player 1',
  `PlayerGuid2` int(10) unsigned NOT NULL COMMENT 'Player 2',
  `NewRank` tinyint(3) unsigned NOT NULL COMMENT 'New rank(in case promotion/demotion)',
  `TimeStamp` int(10) unsigned NOT NULL COMMENT 'Event UNIX time',
  PRIMARY KEY (`guildid`,`LogGuid`),
  KEY `Idx_PlayerGuid1` (`PlayerGuid1`),
  KEY `Idx_PlayerGuid2` (`PlayerGuid2`),
  KEY `Idx_LogGuid` (`LogGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild Eventlog';

/*Data for the table `guild_eventlog` */

/*Table structure for table `guild_finder_applicant` */

DROP TABLE IF EXISTS `guild_finder_applicant`;

CREATE TABLE `guild_finder_applicant` (
  `guildId` int(10) unsigned DEFAULT NULL,
  `playerGuid` int(10) unsigned DEFAULT NULL,
  `availability` tinyint(3) unsigned DEFAULT '0',
  `classRole` tinyint(3) unsigned DEFAULT '0',
  `interests` tinyint(3) unsigned DEFAULT '0',
  `comment` varchar(255) DEFAULT NULL,
  `submitTime` int(10) unsigned DEFAULT NULL,
  UNIQUE KEY `guildId` (`guildId`,`playerGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `guild_finder_applicant` */

/*Table structure for table `guild_finder_guild_settings` */

DROP TABLE IF EXISTS `guild_finder_guild_settings`;

CREATE TABLE `guild_finder_guild_settings` (
  `guildId` int(10) unsigned NOT NULL,
  `availability` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `classRoles` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `interests` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `level` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `listed` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `comment` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`guildId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `guild_finder_guild_settings` */

/*Table structure for table `guild_member` */

DROP TABLE IF EXISTS `guild_member`;

CREATE TABLE `guild_member` (
  `guildid` int(10) unsigned NOT NULL COMMENT 'Guild Identificator',
  `guid` int(10) unsigned NOT NULL,
  `rank` tinyint(3) unsigned NOT NULL,
  `pnote` varchar(31) NOT NULL DEFAULT '',
  `offnote` varchar(31) NOT NULL DEFAULT '',
  `BankResetTimeMoney` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemMoney` int(10) unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab0` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab0` int(10) unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab1` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab1` int(10) unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab2` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab2` int(10) unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab3` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab3` int(10) unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab4` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab4` int(10) unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab5` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab5` int(10) unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab6` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab6` int(10) unsigned NOT NULL DEFAULT '0',
  `BankResetTimeTab7` int(10) unsigned NOT NULL DEFAULT '0',
  `BankRemSlotsTab7` int(10) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `guid_key` (`guid`),
  KEY `guildid_key` (`guildid`),
  KEY `guildid_rank_key` (`guildid`,`rank`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

/*Data for the table `guild_member` */

/*Table structure for table `guild_news_log` */

DROP TABLE IF EXISTS `guild_news_log`;

CREATE TABLE `guild_news_log` (
  `guild` int(10) unsigned NOT NULL,
  `id` int(10) unsigned NOT NULL,
  `eventType` int(10) unsigned NOT NULL,
  `playerGuid` bigint(20) unsigned NOT NULL,
  `data` int(10) unsigned NOT NULL,
  `flags` int(10) unsigned NOT NULL,
  `date` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guild`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `guild_news_log` */

/*Table structure for table `guild_rank` */

DROP TABLE IF EXISTS `guild_rank`;

CREATE TABLE `guild_rank` (
  `guildid` int(10) unsigned NOT NULL DEFAULT '0',
  `rid` tinyint(3) unsigned NOT NULL,
  `rname` varchar(20) NOT NULL DEFAULT '',
  `rights` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `BankMoneyPerDay` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guildid`,`rid`),
  KEY `Idx_rid` (`rid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

/*Data for the table `guild_rank` */

/*Table structure for table `instance` */

DROP TABLE IF EXISTS `instance`;

CREATE TABLE `instance` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `map` smallint(5) unsigned NOT NULL DEFAULT '0',
  `resettime` int(10) unsigned NOT NULL DEFAULT '0',
  `difficulty` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `completedEncounters` int(10) unsigned NOT NULL DEFAULT '0',
  `data` tinytext NOT NULL,
  PRIMARY KEY (`id`),
  KEY `map` (`map`),
  KEY `resettime` (`resettime`),
  KEY `difficulty` (`difficulty`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `instance` */

/*Table structure for table `instance_reset` */

DROP TABLE IF EXISTS `instance_reset`;

CREATE TABLE `instance_reset` (
  `mapid` smallint(5) unsigned NOT NULL DEFAULT '0',
  `difficulty` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `resettime` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`mapid`,`difficulty`),
  KEY `difficulty` (`difficulty`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `instance_reset` */

insert  into `instance_reset`(`mapid`,`difficulty`,`resettime`) values (33,2,1509854400),(36,2,1509854400),(249,3,1510372800),(249,4,1510372800),(269,2,1509854400),(409,9,1510372800),(469,9,1510372800),(509,3,1510027200),(531,9,1510372800),(532,3,1510372800),(533,3,1510372800),(533,4,1510372800),(534,4,1510372800),(540,2,1509854400),(542,2,1509854400),(543,2,1509854400),(544,4,1510372800),(545,2,1509854400),(546,2,1509854400),(547,2,1509854400),(548,4,1510372800),(550,4,1510372800),(552,2,1509854400),(553,2,1509854400),(554,2,1509854400),(555,2,1509854400),(556,2,1509854400),(557,2,1509854400),(558,2,1509854400),(560,2,1509854400),(564,4,1510372800),(565,4,1510372800),(568,2,1509854400),(574,2,1509854400),(575,2,1509854400),(576,2,1509854400),(578,2,1509854400),(580,4,1510372800),(585,2,1509854400),(595,2,1509854400),(598,2,1509854400),(599,2,1509854400),(600,2,1509854400),(601,2,1509854400),(602,2,1509854400),(603,3,1510372800),(603,4,1510372800),(604,2,1509854400),(608,2,1509854400),(615,3,1510372800),(615,4,1510372800),(616,3,1510372800),(616,4,1510372800),(619,2,1509854400),(624,3,1510372800),(624,4,1510372800),(631,3,1510372800),(631,4,1510372800),(631,5,1510372800),(631,6,1510372800),(632,2,1509854400),(643,2,1509854400),(644,2,1509854400),(645,2,1509854400),(649,3,1510372800),(649,4,1510372800),(649,5,1510372800),(649,6,1510372800),(650,2,1509854400),(657,2,1509854400),(658,2,1509854400),(668,2,1509854400),(669,3,1510372800),(669,4,1510372800),(669,5,1510372800),(669,6,1510372800),(670,2,1509854400),(671,3,1510372800),(671,4,1510372800),(671,5,1510372800),(671,6,1510372800),(720,3,1510372800),(720,4,1510372800),(720,5,1510372800),(720,6,1510372800),(724,3,1510372800),(724,4,1510372800),(724,5,1510372800),(724,6,1510372800),(725,2,1509854400),(754,3,1510372800),(754,4,1510372800),(754,5,1510372800),(754,6,1510372800),(755,2,1509854400),(757,3,1510372800),(757,4,1510372800),(757,5,1510372800),(757,6,1510372800),(859,2,1509854400),(938,2,1509854400),(939,2,1509854400),(940,2,1509854400),(959,2,1509854400),(960,2,1509854400),(961,2,1509854400),(962,2,1509854400),(967,3,1510372800),(967,4,1510372800),(967,5,1510372800),(967,6,1510372800),(994,2,1509854400),(996,3,1510372800),(996,4,1510372800),(996,5,1510372800),(996,6,1510372800),(1001,2,1509854400),(1004,2,1509854400),(1007,2,1509854400),(1008,3,1510372800),(1008,4,1510372800),(1008,5,1510372800),(1008,6,1510372800),(1009,3,1510372800),(1009,4,1510372800),(1009,5,1510372800),(1009,6,1510372800),(1011,2,1509854400),(1098,3,1510372800),(1098,4,1510372800),(1098,5,1510372800),(1098,6,1510372800),(1136,14,1510372800),(1136,15,1510372800),(1136,16,1510372800),(1175,2,1509854400),(1175,23,1510372800),(1176,2,1509854400),(1176,23,1510372800),(1182,2,1509854400),(1182,23,1510372800),(1195,2,1509854400),(1195,23,1510372800),(1205,14,1510372800),(1205,15,1510372800),(1205,16,1510372800),(1208,2,1509854400),(1208,23,1510372800),(1209,2,1509854400),(1209,23,1510372800),(1228,14,1510372800),(1228,15,1510372800),(1228,16,1510372800),(1279,2,1509854400),(1279,23,1510372800),(1358,2,1509854400),(1358,23,1510372800),(1448,14,1510372800),(1448,15,1510372800),(1448,16,1510372800);

/*Table structure for table `item_instance` */

DROP TABLE IF EXISTS `item_instance`;

CREATE TABLE `item_instance` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `itemEntry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `owner_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `creatorGuid` int(10) unsigned NOT NULL DEFAULT '0',
  `giftCreatorGuid` int(10) unsigned NOT NULL DEFAULT '0',
  `count` int(10) unsigned NOT NULL DEFAULT '1',
  `duration` int(10) NOT NULL DEFAULT '0',
  `charges` tinytext,
  `flags` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `enchantments` text NOT NULL,
  `randomPropertyId` smallint(5) NOT NULL DEFAULT '0',
  `transmogrifyId` int(10) NOT NULL DEFAULT '0',
  `bonuses` text,
  `upgradeId` mediumint(8) DEFAULT NULL,
  `durability` smallint(5) unsigned NOT NULL DEFAULT '0',
  `playedTime` int(10) unsigned NOT NULL DEFAULT '0',
  `text` text,
  `custom_flags` int(10) unsigned NOT NULL DEFAULT '0',
  `enchantIllusionId` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`),
  KEY `idx_owner_guid` (`owner_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Item System';

/*Data for the table `item_instance` */

insert  into `item_instance`(`guid`,`itemEntry`,`owner_guid`,`creatorGuid`,`giftCreatorGuid`,`count`,`duration`,`charges`,`flags`,`enchantments`,`randomPropertyId`,`transmogrifyId`,`bonuses`,`upgradeId`,`durability`,`playedTime`,`text`,`custom_flags`,`enchantIllusionId`) values (2,49514,1,0,0,1,0,'0 0 0 0 0 ',0,'0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ',0,0,'',0,55,0,'',0,0),(4,49515,1,0,0,1,0,'0 0 0 0 0 ',0,'0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ',0,0,'',0,40,0,'',0,0),(6,49516,1,0,0,1,0,'0 0 0 0 0 ',0,'0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ',0,0,'',0,25,0,'',0,0),(8,36,1,0,0,1,0,'0 0 0 0 0 ',0,'0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ',0,0,'',0,35,0,'',0,0),(10,6948,1,0,0,1,0,'0 0 0 0 0 ',1,'0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ',0,0,'',0,0,0,'',0,0),(12,50055,1,0,0,1,0,'0 0 0 0 0 ',0,'0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ',0,0,'',0,25,0,'',0,0),(14,58242,2,0,0,1,0,'0 0 0 0 0 ',0,'0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ',0,0,'',0,55,0,'',0,0),(16,23477,2,0,0,1,0,'0 0 0 0 0 ',0,'0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ',0,0,'',0,40,0,'',0,0),(18,2361,2,0,0,1,0,'0 0 0 0 0 ',0,'0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ',0,0,'',0,40,0,'',0,0),(20,52533,2,0,0,1,0,'0 0 0 0 0 ',0,'0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ',0,0,'',0,25,0,'',0,0),(22,6948,2,0,0,1,0,'0 0 0 0 0 ',1,'0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ',0,0,'',0,0,0,'',0,0);

/*Table structure for table `item_refund_instance` */

DROP TABLE IF EXISTS `item_refund_instance`;

CREATE TABLE `item_refund_instance` (
  `item_guid` int(10) unsigned NOT NULL COMMENT 'Item GUID',
  `player_guid` int(10) unsigned NOT NULL COMMENT 'Player GUID',
  `paidMoney` int(10) unsigned NOT NULL DEFAULT '0',
  `paidExtendedCost` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`item_guid`,`player_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Item Refund System';

/*Data for the table `item_refund_instance` */

/*Table structure for table `item_soulbound_trade_data` */

DROP TABLE IF EXISTS `item_soulbound_trade_data`;

CREATE TABLE `item_soulbound_trade_data` (
  `itemGuid` int(10) unsigned NOT NULL COMMENT 'Item GUID',
  `allowedPlayers` text NOT NULL COMMENT 'Space separated GUID list of players who can receive this item in trade',
  PRIMARY KEY (`itemGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Item Refund System';

/*Data for the table `item_soulbound_trade_data` */

/*Table structure for table `item_template` */

DROP TABLE IF EXISTS `item_template`;

CREATE TABLE `item_template` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `class` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `subclass` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `name` varchar(255) NOT NULL DEFAULT '',
  `Quality` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `spellid_1` int(11) unsigned NOT NULL,
  `spellid_2` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`),
  KEY `idx_name` (`name`),
  KEY `items_index` (`class`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Item System';

/*Data for the table `item_template` */

/*Table structure for table `lag_reports` */

DROP TABLE IF EXISTS `lag_reports`;

CREATE TABLE `lag_reports` (
  `reportId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `lagType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `mapId` smallint(5) unsigned NOT NULL DEFAULT '0',
  `posX` float NOT NULL DEFAULT '0',
  `posY` float NOT NULL DEFAULT '0',
  `posZ` float NOT NULL DEFAULT '0',
  `latency` int(10) unsigned NOT NULL DEFAULT '0',
  `createTime` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`reportId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player System';

/*Data for the table `lag_reports` */

/*Table structure for table `lfg_data` */

DROP TABLE IF EXISTS `lfg_data`;

CREATE TABLE `lfg_data` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `dungeon` int(10) unsigned NOT NULL DEFAULT '0',
  `state` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='LFG Data';

/*Data for the table `lfg_data` */

/*Table structure for table `log_arena` */

DROP TABLE IF EXISTS `log_arena`;

CREATE TABLE `log_arena` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `timestamp` int(11) DEFAULT NULL,
  `string` blob,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `log_arena` */

/*Table structure for table `log_faction_change` */

DROP TABLE IF EXISTS `log_faction_change`;

CREATE TABLE `log_faction_change` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `guid` int(16) NOT NULL,
  `account` int(11) NOT NULL,
  `OldRace` int(11) NOT NULL,
  `NewRace` int(11) NOT NULL,
  `date` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `log_faction_change` */

/*Table structure for table `log_gm` */

DROP TABLE IF EXISTS `log_gm`;

CREATE TABLE `log_gm` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `date` datetime NOT NULL,
  `gm_account_id` bigint(20) NOT NULL,
  `gm_account_name` blob NOT NULL,
  `gm_character_id` bigint(20) NOT NULL,
  `gm_character_name` blob NOT NULL,
  `gm_last_ip` tinytext,
  `sc_account_id` bigint(20) NOT NULL,
  `sc_account_name` blob NOT NULL,
  `sc_character_id` bigint(20) NOT NULL,
  `sc_character_name` blob NOT NULL,
  `command` blob NOT NULL,
  PRIMARY KEY (`id`),
  KEY `gm_account_id` (`gm_account_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `log_gm` */

/*Table structure for table `log_gm_chat` */

DROP TABLE IF EXISTS `log_gm_chat`;

CREATE TABLE `log_gm_chat` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `type` int(11) NOT NULL,
  `date` datetime NOT NULL,
  `from_account_id` bigint(20) NOT NULL,
  `from_account_name` blob NOT NULL,
  `from_character_id` bigint(20) NOT NULL,
  `from_character_name` blob NOT NULL,
  `to_account_id` bigint(20) NOT NULL,
  `to_account_name` blob NOT NULL,
  `to_character_id` bigint(20) NOT NULL,
  `to_character_name` blob NOT NULL,
  `message` blob NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `log_gm_chat` */

/*Table structure for table `log_mail` */

DROP TABLE IF EXISTS `log_mail`;

CREATE TABLE `log_mail` (
  `id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Identifier',
  `messageType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `stationery` tinyint(3) NOT NULL DEFAULT '41',
  `mailTemplateId` smallint(5) unsigned NOT NULL DEFAULT '0',
  `sender` int(10) unsigned NOT NULL DEFAULT '0',
  `receiver` int(10) unsigned NOT NULL DEFAULT '0',
  `subject` longtext,
  `body` longtext,
  `has_items` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `expire_time` int(10) unsigned NOT NULL DEFAULT '0',
  `deliver_time` int(10) unsigned NOT NULL DEFAULT '0',
  `money` bigint(20) DEFAULT NULL,
  `cod` bigint(20) unsigned NOT NULL DEFAULT '0',
  `checked` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `log_mail` */

/*Table structure for table `log_rename` */

DROP TABLE IF EXISTS `log_rename`;

CREATE TABLE `log_rename` (
  `guid` int(11) unsigned NOT NULL,
  `date` datetime NOT NULL,
  `oldName` tinytext NOT NULL,
  `newName` tinytext NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `log_rename` */

/*Table structure for table `log_store_gold` */

DROP TABLE IF EXISTS `log_store_gold`;

CREATE TABLE `log_store_gold` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `transaction` int(11) NOT NULL DEFAULT '0',
  `guid` int(11) NOT NULL DEFAULT '0',
  `gold` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `log_store_gold` */

/*Table structure for table `log_store_item` */

DROP TABLE IF EXISTS `log_store_item`;

CREATE TABLE `log_store_item` (
  `id` int(11) NOT NULL DEFAULT '0',
  `transaction` int(11) DEFAULT '0',
  `guid` int(11) DEFAULT '0',
  `itemid` int(11) DEFAULT '0',
  `count` int(5) DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `log_store_item` */

/*Table structure for table `log_store_title` */

DROP TABLE IF EXISTS `log_store_title`;

CREATE TABLE `log_store_title` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `transaction` int(10) unsigned NOT NULL,
  `guid` int(10) unsigned NOT NULL,
  `title` int(10) unsigned NOT NULL,
  `date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  UNIQUE KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `log_store_title` */

/*Table structure for table `log_trade` */

DROP TABLE IF EXISTS `log_trade`;

CREATE TABLE `log_trade` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` datetime NOT NULL,
  `sc_accountid` int(11) DEFAULT NULL,
  `sc_guid` int(11) DEFAULT NULL,
  `sc_name` text,
  `tar_accountid` int(11) DEFAULT NULL,
  `tar_guid` int(11) DEFAULT NULL,
  `tar_name` text,
  `item_name` tinytext,
  `item_entry` int(11) DEFAULT NULL,
  `item_count` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `log_trade` */

/*Table structure for table `lvl100_fr` */

DROP TABLE IF EXISTS `lvl100_fr`;

CREATE TABLE `lvl100_fr` (
  `account` int(11) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `account` (`account`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `lvl100_fr` */

/*Table structure for table `mail` */

DROP TABLE IF EXISTS `mail`;

CREATE TABLE `mail` (
  `id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Identifier',
  `messageType` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `stationery` tinyint(3) NOT NULL DEFAULT '41',
  `mailTemplateId` smallint(5) unsigned NOT NULL DEFAULT '0',
  `sender` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character Global Unique Identifier',
  `receiver` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character Global Unique Identifier',
  `subject` longtext,
  `body` longtext,
  `has_items` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `expire_time` int(10) unsigned NOT NULL DEFAULT '0',
  `deliver_time` int(10) unsigned NOT NULL DEFAULT '0',
  `money` bigint(20) unsigned NOT NULL DEFAULT '0',
  `cod` bigint(20) unsigned NOT NULL DEFAULT '0',
  `checked` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `idx_receiver` (`receiver`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Mail System';

/*Data for the table `mail` */

/*Table structure for table `mail_items` */

DROP TABLE IF EXISTS `mail_items`;

CREATE TABLE `mail_items` (
  `mail_id` int(10) unsigned NOT NULL DEFAULT '0',
  `item_guid` int(10) unsigned NOT NULL DEFAULT '0',
  `receiver` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Character Global Unique Identifier',
  PRIMARY KEY (`item_guid`),
  KEY `idx_receiver` (`receiver`),
  KEY `idx_mail_id` (`mail_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

/*Data for the table `mail_items` */

/*Table structure for table `pet_aura` */

DROP TABLE IF EXISTS `pet_aura`;

CREATE TABLE `pet_aura` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `slot` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `caster_guid` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'Full Global Unique Identifier',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `effect_mask` int(11) unsigned NOT NULL DEFAULT '0',
  `recalculate_mask` int(11) unsigned NOT NULL DEFAULT '0',
  `stackcount` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `maxduration` int(11) NOT NULL DEFAULT '0',
  `remaintime` int(11) NOT NULL DEFAULT '0',
  `remaincharges` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spell`,`slot`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Pet System';

/*Data for the table `pet_aura` */

/*Table structure for table `pet_aura_effect` */

DROP TABLE IF EXISTS `pet_aura_effect`;

CREATE TABLE `pet_aura_effect` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `slot` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `effect` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `amount` int(11) NOT NULL DEFAULT '0',
  `baseamount` int(11) NOT NULL DEFAULT '0',
  `data` blob,
  PRIMARY KEY (`guid`,`slot`,`effect`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `pet_aura_effect` */

/*Table structure for table `pet_spell` */

DROP TABLE IF EXISTS `pet_spell`;

CREATE TABLE `pet_spell` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Spell Identifier',
  `active` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Pet System';

/*Data for the table `pet_spell` */

/*Table structure for table `pet_spell_cooldown` */

DROP TABLE IF EXISTS `pet_spell_cooldown`;

CREATE TABLE `pet_spell_cooldown` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier, Low part',
  `spell` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Spell Identifier',
  `time` int(10) unsigned NOT NULL DEFAULT '0',
  `categoryId` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Spell category Id',
  `categoryEnd` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`spell`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `pet_spell_cooldown` */

/*Table structure for table `petition` */

DROP TABLE IF EXISTS `petition`;

CREATE TABLE `petition` (
  `ownerguid` int(10) unsigned NOT NULL,
  `petitionguid` int(10) unsigned DEFAULT '0',
  `name` varchar(24) NOT NULL,
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ownerguid`,`type`),
  UNIQUE KEY `index_ownerguid_petitionguid` (`ownerguid`,`petitionguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

/*Data for the table `petition` */

/*Table structure for table `petition_sign` */

DROP TABLE IF EXISTS `petition_sign`;

CREATE TABLE `petition_sign` (
  `ownerguid` int(10) unsigned NOT NULL,
  `petitionguid` int(10) unsigned NOT NULL DEFAULT '0',
  `playerguid` int(10) unsigned NOT NULL DEFAULT '0',
  `player_account` int(10) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`petitionguid`,`playerguid`),
  KEY `Idx_playerguid` (`playerguid`),
  KEY `Idx_ownerguid` (`ownerguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Guild System';

/*Data for the table `petition_sign` */

/*Table structure for table `players_reports_status` */

DROP TABLE IF EXISTS `players_reports_status`;

CREATE TABLE `players_reports_status` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `creation_time` int(10) unsigned NOT NULL DEFAULT '0',
  `average` float NOT NULL DEFAULT '0',
  `total_reports` bigint(20) unsigned NOT NULL DEFAULT '0',
  `speed_reports` bigint(20) unsigned NOT NULL DEFAULT '0',
  `fly_reports` bigint(20) unsigned NOT NULL DEFAULT '0',
  `jump_reports` bigint(20) unsigned NOT NULL DEFAULT '0',
  `waterwalk_reports` bigint(20) unsigned NOT NULL DEFAULT '0',
  `teleportplane_reports` bigint(20) unsigned NOT NULL DEFAULT '0',
  `climb_reports` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `players_reports_status` */

insert  into `players_reports_status`(`guid`,`creation_time`,`average`,`total_reports`,`speed_reports`,`fly_reports`,`jump_reports`,`waterwalk_reports`,`teleportplane_reports`,`climb_reports`) values (1,0,0,0,0,0,0,0,0,0);

/*Table structure for table `pool_quest_save` */

DROP TABLE IF EXISTS `pool_quest_save`;

CREATE TABLE `pool_quest_save` (
  `pool_id` int(10) unsigned NOT NULL DEFAULT '0',
  `quest_id` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`pool_id`,`quest_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `pool_quest_save` */

insert  into `pool_quest_save`(`pool_id`,`quest_id`) values (348,24635),(349,14104),(350,13903),(352,11377),(353,11667),(354,13424),(357,11389),(358,25162),(359,25155),(360,29363),(361,26226),(362,29360),(363,26190),(364,29353),(365,29316),(366,26543),(367,29346),(368,29317),(369,26442),(370,29344),(371,29324),(372,28059),(372,28137),(373,27949),(373,27967),(374,27987),(374,27991),(375,27971),(375,28050),(376,28682),(376,28686),(377,28678),(377,28680),(378,28696),(378,28700),(379,28690),(379,28693),(5662,13674),(5663,13764),(5664,13769),(5665,13774),(5666,13778),(5667,13783),(5668,13666),(5669,13616),(5670,13742),(5671,13748),(5672,13759),(5673,13754),(5674,13101),(5675,13114),(5676,13836),(5677,12958),(5678,24584),(30004,37881),(30005,35072),(30006,36510),(30007,37320),(30008,37260),(30011,39253);

/*Table structure for table `reserved_name` */

DROP TABLE IF EXISTS `reserved_name`;

CREATE TABLE `reserved_name` (
  `name` varchar(12) NOT NULL DEFAULT '',
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Player Reserved Names';

/*Data for the table `reserved_name` */

/*Table structure for table `store_gold` */

DROP TABLE IF EXISTS `store_gold`;

CREATE TABLE `store_gold` (
  `guid` int(11) unsigned NOT NULL,
  `gold` int(11) unsigned DEFAULT NULL,
  `transaction` int(11) unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`transaction`),
  UNIQUE KEY `transaction` (`transaction`),
  KEY `guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `store_gold` */

/*Table structure for table `store_item` */

DROP TABLE IF EXISTS `store_item`;

CREATE TABLE `store_item` (
  `itemid` int(11) NOT NULL DEFAULT '0',
  `guid` int(11) unsigned NOT NULL,
  `count` int(11) unsigned NOT NULL,
  `transaction` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`transaction`),
  UNIQUE KEY `transaction` (`transaction`),
  KEY `guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `store_item` */

/*Table structure for table `store_level` */

DROP TABLE IF EXISTS `store_level`;

CREATE TABLE `store_level` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0',
  `level` int(11) unsigned NOT NULL DEFAULT '80',
  `transaction` int(11) unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`transaction`),
  UNIQUE KEY `transaction` (`transaction`),
  KEY `guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `store_level` */

/*Table structure for table `store_metier` */

DROP TABLE IF EXISTS `store_metier`;

CREATE TABLE `store_metier` (
  `guid` int(11) unsigned NOT NULL,
  `skill` int(11) unsigned NOT NULL,
  `value` int(11) unsigned NOT NULL,
  `transaction` int(11) unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`transaction`),
  UNIQUE KEY `NewIndex1` (`skill`,`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `store_metier` */

/*Table structure for table `store_profession` */

DROP TABLE IF EXISTS `store_profession`;

CREATE TABLE `store_profession` (
  `guid` int(11) unsigned NOT NULL,
  `skill` int(11) unsigned NOT NULL,
  `recipe` int(11) unsigned NOT NULL,
  PRIMARY KEY (`guid`,`skill`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `store_profession` */

/*Table structure for table `store_title` */

DROP TABLE IF EXISTS `store_title`;

CREATE TABLE `store_title` (
  `guid` int(10) unsigned NOT NULL,
  `title` int(10) unsigned NOT NULL,
  `transaction` int(10) unsigned NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`guid`),
  KEY `transaction` (`transaction`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `store_title` */

/*Table structure for table `time_diff_log` */

DROP TABLE IF EXISTS `time_diff_log`;

CREATE TABLE `time_diff_log` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `time` int(10) unsigned DEFAULT '0',
  `average` int(10) unsigned DEFAULT '0',
  `max` int(10) unsigned DEFAULT '0',
  `players` int(10) unsigned DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=18 DEFAULT CHARSET=utf8;

/*Data for the table `time_diff_log` */

insert  into `time_diff_log`(`id`,`time`,`average`,`max`,`players`) values (1,1509779202,0,0,0),(2,1509779289,10,640,0),(3,1509779322,10,271,0),(4,1509779382,10,1545,0),(5,1509779442,10,12,0),(6,1509779502,10,220,0),(7,1509780223,0,0,0),(8,1509780283,10,707,0),(9,1509780343,10,1560,1),(10,1509780403,10,314,0),(11,1509780463,10,12,0),(12,1509780523,10,12,0),(13,1509780583,10,2895,0),(14,1509780643,9,12,0),(15,1509780703,10,12,0),(16,1509780763,10,12,0),(17,1509780823,10,12,0);

/*Table structure for table `warden_action` */

DROP TABLE IF EXISTS `warden_action`;

CREATE TABLE `warden_action` (
  `wardenId` smallint(5) unsigned NOT NULL,
  `action` tinyint(3) unsigned DEFAULT NULL,
  PRIMARY KEY (`wardenId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `warden_action` */

/*Table structure for table `webshop_delivery_currency` */

DROP TABLE IF EXISTS `webshop_delivery_currency`;

CREATE TABLE `webshop_delivery_currency` (
  `transaction` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `guid` int(11) unsigned NOT NULL DEFAULT '0',
  `currency` int(11) unsigned NOT NULL DEFAULT '0',
  `amount` int(11) unsigned NOT NULL DEFAULT '0',
  `delivery` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`transaction`),
  KEY `guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `webshop_delivery_currency` */

/*Table structure for table `webshop_delivery_gold` */

DROP TABLE IF EXISTS `webshop_delivery_gold`;

CREATE TABLE `webshop_delivery_gold` (
  `transaction` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `guid` int(11) unsigned NOT NULL DEFAULT '0',
  `gold` int(11) unsigned NOT NULL DEFAULT '0',
  `delivery` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `purchase_id` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`transaction`),
  KEY `guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `webshop_delivery_gold` */

/*Table structure for table `webshop_delivery_item` */

DROP TABLE IF EXISTS `webshop_delivery_item`;

CREATE TABLE `webshop_delivery_item` (
  `transaction` int(11) NOT NULL AUTO_INCREMENT,
  `itemid` int(11) NOT NULL DEFAULT '0',
  `guid` int(11) unsigned NOT NULL,
  `count` int(11) unsigned NOT NULL,
  `delivery` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `ItemBonus` varchar(255) NOT NULL DEFAULT '0',
  `purchase_id` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`transaction`),
  KEY `guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `webshop_delivery_item` */

/*Table structure for table `webshop_delivery_level` */

DROP TABLE IF EXISTS `webshop_delivery_level`;

CREATE TABLE `webshop_delivery_level` (
  `transaction` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `guid` int(11) unsigned NOT NULL DEFAULT '0',
  `level` int(11) unsigned NOT NULL DEFAULT '0',
  `delivery` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `purchase_id` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`transaction`),
  KEY `guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `webshop_delivery_level` */

/*Table structure for table `webshop_delivery_premade` */

DROP TABLE IF EXISTS `webshop_delivery_premade`;

CREATE TABLE `webshop_delivery_premade` (
  `transaction` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `delivery` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `templateId` int(11) unsigned NOT NULL DEFAULT '0',
  `faction` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `account` int(11) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `purchase_id` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`transaction`),
  UNIQUE KEY `transaction` (`transaction`),
  KEY `account` (`account`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `webshop_delivery_premade` */

/*Table structure for table `webshop_delivery_profession` */

DROP TABLE IF EXISTS `webshop_delivery_profession`;

CREATE TABLE `webshop_delivery_profession` (
  `transaction` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `guid` int(11) unsigned NOT NULL DEFAULT '0',
  `skill` int(11) unsigned NOT NULL DEFAULT '0',
  `recipe` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `delivery` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `purchase_id` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`transaction`),
  KEY `transaction` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `webshop_delivery_profession` */

/*Table structure for table `webshop_delivery_title` */

DROP TABLE IF EXISTS `webshop_delivery_title`;

CREATE TABLE `webshop_delivery_title` (
  `transaction` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `guid` int(11) unsigned NOT NULL DEFAULT '0',
  `level` int(11) unsigned NOT NULL DEFAULT '0',
  `delivery` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `purchase_id` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`transaction`),
  KEY `guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `webshop_delivery_title` */

/*Table structure for table `worldstates` */

DROP TABLE IF EXISTS `worldstates`;

CREATE TABLE `worldstates` (
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `value` int(10) unsigned NOT NULL DEFAULT '0',
  `comment` tinytext,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Variable Saves';

/*Data for the table `worldstates` */

insert  into `worldstates`(`entry`,`value`,`comment`) values (3781,9000000,NULL),(3801,0,NULL),(3802,1,NULL),(20002,1510384000,NULL),(20003,1509850800,NULL),(20004,1512075600,NULL),(20005,1509757200,NULL),(20006,17475,NULL),(20007,1429038600,NULL),(20008,17478,'Boss looted reset time'),(30000,17478,NULL);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
