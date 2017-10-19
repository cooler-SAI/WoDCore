
DELETE FROM `_hotfixs` WHERE `Hash` = 2442913102 AND `Entry` in (43958, 64998, 64999, 79802, 81354, 85262, 85429, 85430, 87774, 87781, 87782, 87783, 87791, 89304, 89305, 89306, 89307, 89362, 89390, 89391, 93168, 93169, 95564, 95565, 116785);
INSERT INTO `_hotfixs` (`Entry`, `Hash`, `Date`, `Comment`) VALUES (43958, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (64998, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (64999, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (79802, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (81354, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (85262, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (85429, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (85430, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (87774, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (87781, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (87782, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (87783, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (87791, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (89304, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (89305, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (89306, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (89307, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (89362, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (89390, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (89391, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (93168, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (93169, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (95564, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (95565, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement'),
																   (116785, '2442913102', UNIX_TIMESTAMP(), 'Mount faction requirement');

UPDATE `item_sparse` SET `RequiredReputationFaction` = 0, `RequiredReputationRank` = 0 WHERE `ID` IN (43958, 64998, 64999, 79802, 81354, 85262, 85429, 85430, 87774, 87781, 87782, 87783, 87791, 89304, 89305, 89306, 89307, 89362, 89390, 89391, 93168, 93169, 95564, 95565, 116785);

DELETE FROM `_hotfixs` WHERE `Entry` in (10000, 10001) AND `Hash` = 3146089301;
INSERT INTO `_hotfixs` (`Entry`, `Hash`, `Date`, `Comment`) VALUES ('10000', '3146089301', UNIX_TIMESTAMP(), 'Fun Exented cost 10000'); 
INSERT INTO `_hotfixs` (`Entry`, `Hash`, `Date`, `Comment`) VALUES ('10001', '3146089301', UNIX_TIMESTAMP(), 'Fun Exented cost 10001'); 


DELETE FROM `item_extended_cost` WHERE `ID` in (10000, 10001);
INSERT INTO `item_extended_cost` (
  `ID`,
  `RequiredArenaSlot`,
  `RequiredItem1`,
  `RequiredItem2`,
  `RequiredItem3`,
  `RequiredItem4`,
  `RequiredItem5`,
  `RequiredItemCount1`,
  `RequiredItemCount2`,
  `RequiredItemCount3`,
  `RequiredItemCount4`,
  `RequiredItemCount5`,
  `RequiredPersonalArenaRating`,
  `ItemPurchaseGroup`,
  `RequiredCurrency1`,
  `RequiredCurrency2`,
  `RequiredCurrency3`,
  `RequiredCurrency4`,
  `RequiredCurrency5`,
  `RequiredCurrencyCount1`,
  `RequiredCurrencyCount2`,
  `RequiredCurrencyCount3`,
  `RequiredCurrencyCount4`,
  `RequiredCurrencyCount5`,
  `RequiredFactionId`,
  `RequiredFactionStanding`,
  `RequirementFlags`,
  `RequiredAchievement`,
  `OverrideBuyPrice`,
  `BuildVerified`
) 
VALUES
  (
    '10000',
    '0',
    '29434',
    '0',
    '0',
    '0',
    '0',
    '20',
    '0',
    '0',
    '0',
    '0',
    '0',
    '0',
    '392',
    '0',
    '0',
    '0',
    '0',
    '250000',
    '0',
    '0',
    '0',
    '0',
    '0',
    '0',
    '0',
    '0',
    '0',
    '20779'
  ),
  (
    '10001',
    '0',
    '29434',
    '0',
    '0',
    '0',
    '0',
    '20',
    '0',
    '0',
    '0',
    '0',
    '0',
    '0',
    '392',
    '0',
    '0',
    '0',
    '0',
    '300000',
    '0',
    '0',
    '0',
    '0',
    '0',
    '0',
    '0',
    '0',
    '0',
    '20779'
  );
