REVOKE ALL PRIVILEGES ON * . * FROM 'msstudio'@'localhost';

REVOKE ALL PRIVILEGES ON `world` . * FROM 'msstudio'@'localhost';
REVOKE GRANT OPTION ON `world` . * FROM 'msstudio'@'localhost';

REVOKE ALL PRIVILEGES ON `characters` . * FROM 'msstudio'@'localhost';
REVOKE GRANT OPTION ON `characters` . * FROM 'msstudio'@'localhost';

REVOKE ALL PRIVILEGES ON `auth` . * FROM 'msstudio'@'localhost';
REVOKE GRANT OPTION ON `auth` . * FROM 'msstudio'@'localhost';

REVOKE ALL PRIVILEGES ON `hotfix` . * FROM 'msstudio'@'localhost';
REVOKE GRANT OPTION ON `hotfix` . * FROM 'msstudio'@'localhost';

REVOKE ALL PRIVILEGES ON `web` . * FROM 'msstudio'@'localhost';
REVOKE GRANT OPTION ON `web` . * FROM 'msstudio'@'localhost';

DROP USER 'msstudio'@'localhost';

DROP DATABASE IF EXISTS `world`;
DROP DATABASE IF EXISTS `characters`;
DROP DATABASE IF EXISTS `auth`;
DROP DATABASE IF EXISTS `hotfix`;
DROP DATABASE IF EXISTS `web`;
