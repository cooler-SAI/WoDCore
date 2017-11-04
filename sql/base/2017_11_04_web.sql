/*
SQLyog Community v12.09 (64 bit)
MySQL - 5.5.9 : Database - web
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `api_account_validate` */

DROP TABLE IF EXISTS `api_account_validate`;

CREATE TABLE `api_account_validate` (
  `AccountID` int(10) unsigned NOT NULL,
  `IsValidated` tinyint(3) unsigned NOT NULL,
  KEY `AccountID` (`AccountID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `api_account_validate` */

/*Table structure for table `api_points` */

DROP TABLE IF EXISTS `api_points`;

CREATE TABLE `api_points` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `AccountID` int(10) unsigned NOT NULL,
  `Points` int(10) unsigned NOT NULL,
  `TypePoints` int(10) unsigned NOT NULL,
  `Data` text NOT NULL,
  PRIMARY KEY (`id`),
  KEY `AccountID` (`AccountID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `api_points` */

/*Table structure for table `api_purchase` */

DROP TABLE IF EXISTS `api_purchase`;

CREATE TABLE `api_purchase` (
  `AccountID` int(10) unsigned NOT NULL,
  `Expansion` tinyint(3) unsigned NOT NULL,
  `Realm` int(11) NOT NULL,
  `CharacterGUID` int(10) unsigned NOT NULL,
  `ShopEntryData` text NOT NULL,
  `Count` int(10) unsigned NOT NULL,
  `TypePrice` int(10) unsigned NOT NULL,
  `Price` int(10) unsigned NOT NULL,
  `IP` text NOT NULL,
  KEY `AccountID` (`AccountID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `api_purchase` */

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
