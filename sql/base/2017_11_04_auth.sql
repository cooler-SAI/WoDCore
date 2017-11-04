/*
SQLyog Community v12.09 (64 bit)
MySQL - 5.5.9 : Database - auth
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `account` */

DROP TABLE IF EXISTS `account`;

CREATE TABLE `account` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `username` varchar(250) NOT NULL DEFAULT '',
  `sha_pass_hash` varchar(40) NOT NULL DEFAULT '',
  `sessionkey` varchar(80) NOT NULL DEFAULT '',
  `v` varchar(64) NOT NULL DEFAULT '',
  `s` varchar(64) NOT NULL DEFAULT '',
  `token_key` varchar(100) NOT NULL DEFAULT '',
  `email` varchar(254) NOT NULL DEFAULT '',
  `joindate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `last_ip` varchar(15) NOT NULL DEFAULT '127.0.0.1',
  `failed_logins` int(10) unsigned NOT NULL DEFAULT '0',
  `locked` smallint(3) NOT NULL DEFAULT '0',
  `last_login` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `online` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `expansion` tinyint(3) unsigned NOT NULL DEFAULT '5',
  `mutetime` bigint(20) NOT NULL DEFAULT '0',
  `locale` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `os` varchar(4) NOT NULL DEFAULT '',
  `recruiter` int(10) unsigned NOT NULL DEFAULT '0',
  `bnet2_pass_hash` varchar(256) NOT NULL DEFAULT '',
  `bnet2_salt` varchar(64) NOT NULL DEFAULT '',
  `google_auth` varchar(15) DEFAULT NULL,
  `rsa_clear` blob,
  `admin` tinyint(3) NOT NULL DEFAULT '0',
  `activate` tinyint(4) DEFAULT '0',
  `payment` tinyint(4) DEFAULT '0',
  `service_flags` int(10) unsigned NOT NULL DEFAULT '0',
  `custom_flags` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_username` (`username`),
  KEY `recruiterIndex` (`recruiter`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 COMMENT='Account System';

/*Data for the table `account` */

insert  into `account`(`id`,`username`,`sha_pass_hash`,`sessionkey`,`v`,`s`,`token_key`,`email`,`joindate`,`last_ip`,`failed_logins`,`locked`,`last_login`,`online`,`expansion`,`mutetime`,`locale`,`os`,`recruiter`,`bnet2_pass_hash`,`bnet2_salt`,`google_auth`,`rsa_clear`,`admin`,`activate`,`payment`,`service_flags`,`custom_flags`) values (1,'ANDER@COOL.RU','A844ADB448F8139CF9C0D01EA78C769DDF0FCDD1','','','','','','2017-10-21 18:19:18','127.0.0.1',0,0,'0000-00-00 00:00:00',0,5,0,0,'',0,'ED5AABAEF533A28B8611A035C7AEEEEDB2A3E55DE040ACBFA03D54B5E543DF55C25F2D63A898FF7B854680EBF07F5EBD1E51E6982DF2F65C8E28B44FD20387D59AC3817DDC64B02111525B0CD947453DF637BCB8ED9593E4D660B4582AAABFF66033C7E8FF35AB0D63BB6F2D273A92E3B644891BA1EE60B79C250D95ED02C326','D534A7488A9719DB45F77C7835424BABCFC7806947660242F7647E4E45A09E38',NULL,NULL,0,0,0,0,0),(2,'ANDER','D44CE8DF75C1AE3F7C5CD0CDABB1B3BCD8B960C1','','','','','','2017-10-21 18:31:06','127.0.0.1',0,0,'0000-00-00 00:00:00',0,5,0,0,'',0,'3542918A3D12137A3EABBECFBDCF4CA2914F1A8D63FDDAC99601016382930930EB2D285ED0C15CB0FC3BF0123E20DFF4E97ECBC69A37580215BCE034A180DB4978756D9EFA54991795A1F78B019B8968A177EA705323F892DB74B11612D1D5342A90FD5B4E60995F83E4D5A12FF0DB63CC4E08F58F37B530283272CF29673530','77CFD534A7488A9719DB45F77C7835424BABCFC7806947660242F7647E4E45A0',NULL,NULL,0,0,0,0,0),(3,'1@1.RU','C685AD1029919DCC617C1427F5342CCEDAC64144','','','','','','2017-10-21 21:07:33','127.0.0.1',0,0,'0000-00-00 00:00:00',0,5,0,0,'',0,'5AEE608E0EF05D8B17A53BA4258BD1BCE753BC6B144106BEC5E76199E42609E7E36778FE9A618F8E25093CA0D3C5B343DA6D9A9A66150C08ADCA16BCD055F9B845060935B3A8006EEB825FDCD9E8C0DDED6BD4F3195D2AA31D5F3FFCFBA58CFE30BDFB5C6B7D1AC50CFCB75B202EBD72D35C4A498C05D3D629C7F51267E8E870','77CFD534A7488A9719DB45F77C7835424BABCFC7806947660242F7647E4E45A0',NULL,NULL,0,0,0,0,0),(4,'ADMIN@ADMIN.COM','3D7094301C87120708A2C3F93FE682F15C7D38E7','3EFCB42144EE80190F93240A572A8907A2B9165A41CA272C034EECB76CFB28B5E5D9FF3D67E3916D','','','','','2017-10-21 21:51:04','127.0.0.1',0,0,'2017-10-21 22:20:01',0,5,0,8,'Wn64',0,'89638FAA6A99DC7E4E0CF108853E4BAC77F3183BB8D9005658243830B8DC95EAD5D8B08F18D324065F1E9AA98B1B3A49E6F887B7285125C7EFC57BB74AB95BEBB477E5D2E8966068D8843DCD05B8B5B6460C9E8D5D8FCC295558D3953E4D7C1D111F64915A9A4112EFB5C691A5BA80FA427FEA3409DCE68A7850559269050D7D','77CFD534A7488A9719DB45F77C7835424BABCFC7806947660242F7647E4E45A0',NULL,NULL,0,0,0,0,0),(5,'ANDER@ANDER.COM','59610BA40CE8D3EE5E37DA388479FD1F69E60B4E','5C03F34A5247B56DA44BE5266A34766F2DB4E513685C7FD5FD2711DE2503F9A9F2FC9E3A9A503870','','','','','2017-11-04 01:00:00','127.0.0.1',0,0,'2017-11-04 10:24:00',1,5,0,8,'Wn64',0,'5DDD0013FE745CE523CCD1E0C27BD8E0E80071B9DA94CD9C90694FB774833651B5BBA93840E009FA0E69009AA84FD626FB9AF2A35B94777A0247BFCD15B72930E8CA5170A1D211FBF1255F0B27BAA879519C7316E47F3A9A9A3070CB7738C47ED9197365A904256EAD2ED0434228FB81A59C53E05A36EDD1A97B0BBB8A84F740','77CFD534A7488A9719DB45F77C7835424BABCFC7806947660242F7647E4E45A0',NULL,NULL,0,0,0,0,0);

/*Table structure for table `account_access` */

DROP TABLE IF EXISTS `account_access`;

CREATE TABLE `account_access` (
  `id` int(10) unsigned NOT NULL,
  `gmlevel` tinyint(3) unsigned NOT NULL,
  `RealmID` int(11) NOT NULL DEFAULT '-1',
  `comment` text,
  PRIMARY KEY (`id`,`RealmID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `account_access` */

/*Table structure for table `account_banned` */

DROP TABLE IF EXISTS `account_banned`;

CREATE TABLE `account_banned` (
  `id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'Account id',
  `externalban` int(10) unsigned NOT NULL DEFAULT '0',
  `bandate` int(10) unsigned NOT NULL DEFAULT '0',
  `unbandate` int(10) unsigned NOT NULL DEFAULT '0',
  `bannedby` varchar(50) NOT NULL,
  `banreason` varchar(255) NOT NULL,
  `active` tinyint(3) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`,`bandate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Ban List';

/*Data for the table `account_banned` */

/*Table structure for table `account_battlepay` */

DROP TABLE IF EXISTS `account_battlepay`;

CREATE TABLE `account_battlepay` (
  `accountId` int(11) unsigned NOT NULL,
  `points` int(11) NOT NULL,
  PRIMARY KEY (`accountId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `account_battlepay` */

/*Table structure for table `account_battlepet` */

DROP TABLE IF EXISTS `account_battlepet`;

CREATE TABLE `account_battlepet` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `account` int(10) unsigned NOT NULL DEFAULT '0',
  `slot` int(11) NOT NULL DEFAULT '-1',
  `name` varchar(50) NOT NULL,
  `nameTimeStamp` int(10) unsigned NOT NULL DEFAULT '0',
  `species` int(10) unsigned NOT NULL DEFAULT '0',
  `quality` int(10) unsigned NOT NULL DEFAULT '0',
  `breed` int(10) unsigned NOT NULL DEFAULT '0',
  `level` int(10) unsigned NOT NULL DEFAULT '0',
  `xp` int(10) unsigned NOT NULL DEFAULT '0',
  `display` int(10) unsigned NOT NULL DEFAULT '0',
  `health` int(11) NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `infoPower` int(11) NOT NULL DEFAULT '0',
  `infoMaxHealth` int(11) NOT NULL DEFAULT '0',
  `infoSpeed` int(11) NOT NULL DEFAULT '0',
  `infoGender` int(11) NOT NULL DEFAULT '0',
  `declinedGenitive` varchar(50) NOT NULL,
  `declinedNative` varchar(50) NOT NULL,
  `declinedAccusative` varchar(50) NOT NULL,
  `declinedInstrumental` varchar(50) NOT NULL,
  `declinedPrepositional` varchar(50) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `account` (`account`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `account_battlepet` */

/*Table structure for table `account_heirlooms` */

DROP TABLE IF EXISTS `account_heirlooms`;

CREATE TABLE `account_heirlooms` (
  `account_id` int(10) unsigned NOT NULL DEFAULT '0',
  `heirloom_id` int(10) unsigned NOT NULL DEFAULT '0',
  `upgrade_flags` int(10) unsigned NOT NULL DEFAULT '0',
  `groupRealmMask` int(10) unsigned NOT NULL,
  PRIMARY KEY (`account_id`,`heirloom_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `account_heirlooms` */

/*Table structure for table `account_log_ip` */

DROP TABLE IF EXISTS `account_log_ip`;

CREATE TABLE `account_log_ip` (
  `accountid` int(11) unsigned NOT NULL,
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `ip` varchar(30) NOT NULL DEFAULT '0.0.0.0',
  `date` datetime DEFAULT NULL,
  `error` int(10) unsigned NOT NULL DEFAULT '0',
  `source` int(10) unsigned NOT NULL DEFAULT '2',
  PRIMARY KEY (`accountid`,`ip`,`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `account_log_ip` */

/*Table structure for table `account_loyalty` */

DROP TABLE IF EXISTS `account_loyalty`;

CREATE TABLE `account_loyalty` (
  `AccountID` int(11) unsigned NOT NULL,
  `LastClaim` int(11) NOT NULL DEFAULT '0',
  `LastEventReset` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`AccountID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `account_loyalty` */

/*Table structure for table `account_loyalty_event` */

DROP TABLE IF EXISTS `account_loyalty_event`;

CREATE TABLE `account_loyalty_event` (
  `AccountID` int(11) unsigned NOT NULL,
  `Event` int(11) unsigned NOT NULL DEFAULT '0',
  `Count` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`AccountID`,`Event`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `account_loyalty_event` */

/*Table structure for table `account_note` */

DROP TABLE IF EXISTS `account_note`;

CREATE TABLE `account_note` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `account_id` int(10) unsigned DEFAULT NULL,
  `note` text,
  `date` int(11) DEFAULT NULL,
  `notedby` varchar(255) DEFAULT NULL,
  KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `account_note` */

/*Table structure for table `account_premium` */

DROP TABLE IF EXISTS `account_premium`;

CREATE TABLE `account_premium` (
  `id` int(11) NOT NULL DEFAULT '0' COMMENT 'Account id',
  `setdate` bigint(40) NOT NULL DEFAULT '0',
  `unsetdate` bigint(40) NOT NULL DEFAULT '0',
  `premium_type` tinyint(4) unsigned NOT NULL DEFAULT '1',
  `gm` varchar(12) NOT NULL DEFAULT '0',
  `active` tinyint(4) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`,`setdate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `account_premium` */

/*Table structure for table `account_spell` */

DROP TABLE IF EXISTS `account_spell`;

CREATE TABLE `account_spell` (
  `accountId` int(11) NOT NULL,
  `spell` int(10) NOT NULL,
  `active` tinyint(1) DEFAULT NULL,
  `disabled` tinyint(1) DEFAULT NULL,
  `IsMountFavorite` tinyint(1) NOT NULL DEFAULT '0',
  `groupRealmMask` int(10) unsigned NOT NULL,
  PRIMARY KEY (`accountId`,`spell`),
  KEY `account` (`accountId`) USING HASH,
  KEY `account_spell` (`accountId`,`spell`) USING HASH
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `account_spell` */

/*Table structure for table `account_toys` */

DROP TABLE IF EXISTS `account_toys`;

CREATE TABLE `account_toys` (
  `account_id` int(10) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `is_favorite` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`account_id`,`item_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `account_toys` */

/*Table structure for table `account_vote` */

DROP TABLE IF EXISTS `account_vote`;

CREATE TABLE `account_vote` (
  `account` int(11) unsigned NOT NULL,
  `remainingTime` int(11) unsigned NOT NULL,
  PRIMARY KEY (`account`),
  UNIQUE KEY `account` (`account`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `account_vote` */

/*Table structure for table `autobroadcast` */

DROP TABLE IF EXISTS `autobroadcast`;

CREATE TABLE `autobroadcast` (
  `Expension` int(11) DEFAULT NULL,
  `RealmID` int(11) DEFAULT NULL,
  `Text` blob,
  `TextFR` blob,
  `TextES` blob,
  `textRU` blob
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `autobroadcast` */

/*Table structure for table `battlenet_modules` */

DROP TABLE IF EXISTS `battlenet_modules`;

CREATE TABLE `battlenet_modules` (
  `Hash` varchar(64) NOT NULL,
  `Name` varchar(64) NOT NULL DEFAULT '',
  `Type` varchar(8) NOT NULL,
  `System` varchar(8) NOT NULL,
  `Data` text,
  PRIMARY KEY (`Name`,`System`),
  UNIQUE KEY `uk_name_type_system` (`Name`,`Type`,`System`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `battlenet_modules` */

/*Table structure for table `bnet_allowed_build` */

DROP TABLE IF EXISTS `bnet_allowed_build`;

CREATE TABLE `bnet_allowed_build` (
  `build` int(11) DEFAULT NULL,
  `version` varchar(40) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `bnet_allowed_build` */

insert  into `bnet_allowed_build`(`build`,`version`) values (20779,'6.2.3.20779'),(20726,'6.2.3.20726');

/*Table structure for table `character_renderer_queue` */

DROP TABLE IF EXISTS `character_renderer_queue`;

CREATE TABLE `character_renderer_queue` (
  `guid` int(11) DEFAULT NULL,
  `race` tinyint(3) DEFAULT NULL,
  `gender` tinyint(3) DEFAULT NULL,
  `class` tinyint(3) DEFAULT NULL,
  `skinColor` tinyint(3) DEFAULT NULL,
  `face` tinyint(3) DEFAULT NULL,
  `hairStyle` tinyint(3) DEFAULT NULL,
  `hairColor` tinyint(3) DEFAULT NULL,
  `facialHair` tinyint(3) DEFAULT NULL,
  `equipment` blob
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

/*Data for the table `character_renderer_queue` */

/*Table structure for table `firewall_farms` */

DROP TABLE IF EXISTS `firewall_farms`;

CREATE TABLE `firewall_farms` (
  `ip` tinytext NOT NULL,
  `type` tinyint(1) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `firewall_farms` */

/*Table structure for table `ip_banned` */

DROP TABLE IF EXISTS `ip_banned`;

CREATE TABLE `ip_banned` (
  `ip` varchar(15) NOT NULL DEFAULT '127.0.0.1',
  `bandate` int(10) unsigned NOT NULL,
  `unbandate` int(10) NOT NULL,
  `bannedby` varchar(50) NOT NULL DEFAULT '[Console]',
  `banreason` varchar(255) NOT NULL DEFAULT 'no reason',
  PRIMARY KEY (`ip`,`bandate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Banned IPs';

/*Data for the table `ip_banned` */

/*Table structure for table `ip_to_country` */

DROP TABLE IF EXISTS `ip_to_country`;

CREATE TABLE `ip_to_country` (
  `IP_FROM` bigint(20) unsigned NOT NULL,
  `IP_TO` bigint(20) unsigned NOT NULL,
  `REGISTRY` char(7) NOT NULL,
  `ASSIGNED` bigint(20) NOT NULL,
  `CTRY` char(2) NOT NULL,
  `CNTRY` char(3) NOT NULL,
  `COUNTRY` varchar(100) NOT NULL,
  PRIMARY KEY (`IP_FROM`,`IP_TO`),
  KEY `IP_FROM` (`IP_FROM`),
  KEY `IP_TO` (`IP_TO`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `ip_to_country` */

/*Table structure for table `log_vote` */

DROP TABLE IF EXISTS `log_vote`;

CREATE TABLE `log_vote` (
  `top_name` varchar(15) NOT NULL DEFAULT 'top',
  `ip` varchar(15) NOT NULL DEFAULT '0.0.0.0',
  `date` int(11) NOT NULL,
  PRIMARY KEY (`top_name`,`ip`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `log_vote` */

/*Table structure for table `logs` */

DROP TABLE IF EXISTS `logs`;

CREATE TABLE `logs` (
  `time` int(10) unsigned NOT NULL,
  `realm` int(10) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `string` text CHARACTER SET latin1
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `logs` */

/*Table structure for table `motd` */

DROP TABLE IF EXISTS `motd`;

CREATE TABLE `motd` (
  `RealmID` int(10) unsigned NOT NULL,
  `Text` text NOT NULL,
  `TextFR` text NOT NULL,
  `TextES` text NOT NULL,
  `TextRU` text NOT NULL,
  PRIMARY KEY (`RealmID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `motd` */

insert  into `motd`(`RealmID`,`Text`,`TextFR`,`TextES`,`TextRU`) values (1,'english motd','french motd','spanish motd','russian motd');

/*Table structure for table `realmcharacters` */

DROP TABLE IF EXISTS `realmcharacters`;

CREATE TABLE `realmcharacters` (
  `realmid` int(10) unsigned NOT NULL DEFAULT '0',
  `acctid` int(10) unsigned NOT NULL,
  `numchars` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`realmid`,`acctid`),
  KEY `acctid` (`acctid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Realm Character Tracker';

/*Data for the table `realmcharacters` */

insert  into `realmcharacters`(`realmid`,`acctid`,`numchars`) values (1,1,0),(1,2,0),(1,3,0),(1,4,1),(1,5,1),(2,3,0),(2,4,0);

/*Table structure for table `realmlist` */

DROP TABLE IF EXISTS `realmlist`;

CREATE TABLE `realmlist` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(32) NOT NULL DEFAULT '',
  `address` varchar(255) NOT NULL DEFAULT '127.0.0.1',
  `port` smallint(5) unsigned NOT NULL DEFAULT '8085',
  `icon` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `flag` tinyint(3) unsigned NOT NULL DEFAULT '2',
  `timezone` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `allowedSecurityLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `motd` varchar(255) NOT NULL DEFAULT 'Welcome on our server.',
  `population` float unsigned NOT NULL DEFAULT '0',
  `gamebuild` int(10) unsigned NOT NULL DEFAULT '20779' COMMENT '20779 build 6.2.3',
  `online` int(10) DEFAULT '0',
  `delay` int(10) unsigned NOT NULL DEFAULT '0',
  `queue` int(10) unsigned NOT NULL DEFAULT '0',
  `lastupdate` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COMMENT='Realm System';

/*Data for the table `realmlist` */

insert  into `realmlist`(`id`,`name`,`address`,`port`,`icon`,`flag`,`timezone`,`allowedSecurityLevel`,`motd`,`population`,`gamebuild`,`online`,`delay`,`queue`,`lastupdate`) values (1,'Draenor','127.0.0.1',8085,0,0,1,0,'Welcome to World of Warcraft Warlords of Draenor',0,20779,1,9,0,1509780764),(2,'test','127.0.0.1',8085,0,2,0,0,'Welcome on our server.',0,20779,0,0,0,0);

/*Table structure for table `stat_lifetime_retention` */

DROP TABLE IF EXISTS `stat_lifetime_retention`;

CREATE TABLE `stat_lifetime_retention` (
  `minutes` int(11) NOT NULL,
  `usersPercentage` float(11,8) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `stat_lifetime_retention` */

/*Table structure for table `transferts` */

DROP TABLE IF EXISTS `transferts`;

CREATE TABLE `transferts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `account` int(11) NOT NULL,
  `perso_guid` int(11) NOT NULL,
  `from` int(11) NOT NULL,
  `to` int(11) NOT NULL,
  `revision` blob NOT NULL,
  `dump` longtext NOT NULL,
  `last_error` blob NOT NULL,
  `nb_attempt` int(11) NOT NULL,
  `state` int(10) NOT NULL DEFAULT '0',
  `error` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `transferts` */

/*Table structure for table `transferts_logs` */

DROP TABLE IF EXISTS `transferts_logs`;

CREATE TABLE `transferts_logs` (
  `id` int(11) DEFAULT NULL,
  `account` int(11) DEFAULT NULL,
  `perso_guid` int(11) DEFAULT NULL,
  `from` int(2) DEFAULT NULL,
  `to` int(2) DEFAULT NULL,
  `dump` longtext
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `transferts_logs` */

/*Table structure for table `uptime` */

DROP TABLE IF EXISTS `uptime`;

CREATE TABLE `uptime` (
  `realmid` int(10) unsigned NOT NULL,
  `starttime` int(10) unsigned NOT NULL DEFAULT '0',
  `uptime` int(10) unsigned NOT NULL DEFAULT '0',
  `maxplayers` smallint(5) unsigned NOT NULL DEFAULT '0',
  `revision` varchar(255) NOT NULL DEFAULT 'Trinitycore',
  PRIMARY KEY (`realmid`,`starttime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Uptime system';

/*Data for the table `uptime` */

insert  into `uptime`(`realmid`,`starttime`,`uptime`,`maxplayers`,`revision`) values (1,1508599103,0,0,'Firestorm WoDCore rev. unknown 1970-01-01 00:00:00 +0000 (Archived branch) (Win64, Release)'),(1,1508599669,0,0,'Firestorm WoDCore rev. unknown 1970-01-01 00:00:00 +0000 (Archived branch) (Win64, Release)'),(1,1508606535,2404,0,'Firestorm WoDCore rev. unknown 1970-01-01 00:00:00 +0000 (Archived branch) (Win64, Release)'),(1,1508609627,3003,0,'Firestorm WoDCore rev. unknown 1970-01-01 00:00:00 +0000 (Archived branch) (Win64, Release)'),(1,1508613230,0,0,'Firestorm WoDCore rev. unknown 1970-01-01 00:00:00 +0000 (Archived branch) (Win64, Release)'),(1,1509724086,611,0,'Firestorm WoDCore rev. unknown 1970-01-01 00:00:00 +0000 (Archived branch) (Win64, Release)'),(1,1509739293,1209,0,'Firestorm WoDCore rev. unknown 1970-01-01 00:00:00 +0000 (Archived branch) (Win64, Release)'),(1,1509740703,5411,0,'Firestorm WoDCore rev. unknown 1970-01-01 00:00:00 +0000 (Archived branch) (Win64, Release)'),(1,1509746907,0,0,'Firestorm WoDCore rev. unknown 1970-01-01 00:00:00 +0000 (Archived branch) (Win64, Release)'),(1,1509779190,0,0,'Firestorm WoDCore rev. unknown 1970-01-01 00:00:00 +0000 (Archived branch) (Win64, Release)'),(1,1509780218,0,0,'Firestorm WoDCore rev. unknown 1970-01-01 00:00:00 +0000 (Archived branch) (Win64, Release)');

/*Table structure for table `user_reporting` */

DROP TABLE IF EXISTS `user_reporting`;

CREATE TABLE `user_reporting` (
  `ga` int(10) unsigned NOT NULL,
  `account_id` int(10) unsigned NOT NULL,
  `creation_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `step` int(10) NOT NULL DEFAULT '0',
  `referer` varchar(255) NOT NULL,
  `last_ip` varchar(255) NOT NULL,
  `has_view_creation` tinyint(3) unsigned DEFAULT '0',
  PRIMARY KEY (`ga`,`account_id`),
  KEY `ga_idx` (`ga`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `user_reporting` */

/*Table structure for table `webshop_delivery_interexp_transfer` */

DROP TABLE IF EXISTS `webshop_delivery_interexp_transfer`;

CREATE TABLE `webshop_delivery_interexp_transfer` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `account` int(11) NOT NULL,
  `guid` int(11) NOT NULL,
  `dump` longtext NOT NULL,
  `destrealm` int(11) NOT NULL,
  `state` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `webshop_delivery_interexp_transfer` */

/*Table structure for table `webshop_delivery_interrealm_transfer` */

DROP TABLE IF EXISTS `webshop_delivery_interrealm_transfer`;

CREATE TABLE `webshop_delivery_interrealm_transfer` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `account` int(11) unsigned NOT NULL DEFAULT '0',
  `guid` int(11) unsigned NOT NULL DEFAULT '0',
  `startrealm` int(11) unsigned NOT NULL DEFAULT '0',
  `destrealm` int(11) unsigned NOT NULL DEFAULT '0',
  `revision` blob,
  `dump` longtext,
  `last_error` blob,
  `nb_attempt` int(11) unsigned NOT NULL DEFAULT '0',
  `state` int(10) unsigned NOT NULL DEFAULT '1',
  `error` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `account` (`account`),
  KEY `guid` (`guid`),
  KEY `startrealm` (`startrealm`),
  KEY `destrealm` (`destrealm`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `webshop_delivery_interrealm_transfer` */

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
