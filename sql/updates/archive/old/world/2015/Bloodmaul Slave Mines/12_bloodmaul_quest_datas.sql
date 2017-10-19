DELETE FROM quest_poi  WHERE QuestID=35993;
DELETE FROM quest_poi_points  WHERE questId=35993;
INSERT INTO quest_poi (`QuestID`, `BlobIndex`, `ObjectiveIndex`, `MapID`, `WorldMapAreaId`, `Floor`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `Unk`) VALUES(35993, 0, 32, 1116, 946, 0, 0, 0, 0, 0, 919270) ON DUPLICATE KEY UPDATE `QuestID` = VALUES(`QuestID`), `BlobIndex` = VALUES(`BlobIndex`), `ObjectiveIndex` = VALUES(`ObjectiveIndex`), `MapID` = VALUES(`MapID`), `WorldMapAreaId` = VALUES(`WorldMapAreaId`), `Floor` = VALUES(`Floor`), `Priority` = VALUES(`Priority`), `Flags` = VALUES(`Flags`), `WorldEffectID` = VALUES(`WorldEffectID`), `PlayerConditionID` = VALUES(`PlayerConditionID`), `Unk` = VALUES(`Unk`);
INSERT INTO quest_poi_points (`questId`, `id`, `idx`, `x`, `y`) VALUES(35993, 0, 0, 3168, 754) ON DUPLICATE KEY UPDATE `questId` = VALUES(`questId`), `id` = VALUES(`id`), `idx` = VALUES(`idx`), `x` = VALUES(`x`), `y` = VALUES(`y`);
INSERT INTO quest_poi (`QuestID`, `BlobIndex`, `ObjectiveIndex`, `MapID`, `WorldMapAreaId`, `Floor`, `Priority`, `Flags`, `WorldEffectID`, `PlayerConditionID`, `Unk`) VALUES(35993, 1, 0, 1116, 946, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `QuestID` = VALUES(`QuestID`), `BlobIndex` = VALUES(`BlobIndex`), `ObjectiveIndex` = VALUES(`ObjectiveIndex`), `MapID` = VALUES(`MapID`), `WorldMapAreaId` = VALUES(`WorldMapAreaId`), `Floor` = VALUES(`Floor`), `Priority` = VALUES(`Priority`), `Flags` = VALUES(`Flags`), `WorldEffectID` = VALUES(`WorldEffectID`), `PlayerConditionID` = VALUES(`PlayerConditionID`), `Unk` = VALUES(`Unk`);
INSERT INTO quest_poi_points (`questId`, `id`, `idx`, `x`, `y`) VALUES(35993, 1, 0, 1475, 3039) ON DUPLICATE KEY UPDATE `questId` = VALUES(`questId`), `id` = VALUES(`id`), `idx` = VALUES(`idx`), `x` = VALUES(`x`), `y` = VALUES(`y`);
INSERT INTO quest_template (`Id`, `Method`, `Level`, `ExclusiveGroup`, `MinLevel`, `ZoneOrSort`, `Type`, `SourceItemId`, `SuggestedPlayers`, `NextQuestIdChain`, `RewardXPId`, `RewardMoney`, `RewardMoneyMaxLevel`, `RewardSpell`, `RewardSpellCast`, `RewardHonor`, `RewardHonorMultiplier`, `Flags`, `Flags2`, `RewardTitleId`, `RewardArenaPoints`, `RewardSkillId`, `RewardSkillPoints`, `RewardReputationMask`, `QuestGiverPortrait`, `QuestTurnInPortrait`, `RewardItemId1`, `RewardItemId2`, `RewardItemId3`, `RewardItemId4`, `RewardItemCount1`, `RewardItemCount2`, `RewardItemCount3`, `RewardItemCount4`, `RequiredSourceItemId1`, `RequiredSourceItemId2`, `RequiredSourceItemId3`, `RequiredSourceItemId4`, `RequiredSourceItemCount1`, `RequiredSourceItemCount2`, `RequiredSourceItemCount3`, `RequiredSourceItemCount4`, `RewardChoiceItemId1`, `RewardChoiceItemId2`, `RewardChoiceItemId3`, `RewardChoiceItemId4`, `RewardChoiceItemId5`, `RewardChoiceItemId6`, `RewardChoiceItemCount1`, `RewardChoiceItemCount2`, `RewardChoiceItemCount3`, `RewardChoiceItemCount4`, `RewardChoiceItemCount5`, `RewardChoiceItemCount6`, `RewardFactionId1`, `RewardFactionId2`, `RewardFactionId3`, `RewardFactionId4`, `RewardFactionId5`, `RewardFactionValueId1`, `RewardFactionValueId2`, `RewardFactionValueId3`, `RewardFactionValueId4`, `RewardFactionValueId5`, `RewardFactionValueIdOverride1`, `RewardFactionValueIdOverride2`, `RewardFactionValueIdOverride3`, `RewardFactionValueIdOverride4`, `RewardFactionValueIdOverride5`, `PointMapId`, `PointX`, `PointY`, `PointOption`, `Title`, `Objectives`, `Details`, `EndText`, `CompletedText`, `QuestGiverTextWindow`, `QuestGiverTargetName`, `QuestTurnTextWindow`, `QuestTurnTargetName`, `RewardCurrencyId1`, `RewardCurrencyId2`, `RewardCurrencyId3`, `RewardCurrencyId4`, `RewardCurrencyCount1`, `RewardCurrencyCount2`, `RewardCurrencyCount3`, `RewardCurrencyCount4`, `SoundAccept`, `SoundTurnIn`, `LimitTime`) VALUES(37279, 2, 100, 0, 100, 7307, 85, 0, 0, 0, 8, 462000, 7, 0, 0, 0, 0, 33558536, 0, 0, 0, 0, 0, 0, 0, 0, 119032, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Un défi de taille : le seigneur de guerre Zaela", "Terminer le sommet du pic Rochenoire en mode Défi.", "$n, il faut que vous rassembliez un groupe daventuriers solides et que vous retourniez sur Azeroth, au sommet du pic Rochenoire. Activez lorbe de défi, entrez dans lancienne demeure du clan Rochenoire et triomphez de lavant-garde marchefer et de son nouvel allié, le seigneur de guerre Zaela.$B$BRevenez me voir une fois le défi remporté et je vous récompenserai généreusement.", "", "Retournez voir la Maîtresse des défis Savina à Bouclier-des-Tempêtes à Ashran.", "", "", "", "", 0, 0, 0, 0, 0, 0, 0, 0, 890, 878, 0) ON DUPLICATE KEY UPDATE `Id` = VALUES(`Id`), `Method` = VALUES(`Method`), `Level` = VALUES(`Level`), `ExclusiveGroup` = VALUES(`ExclusiveGroup`), `MinLevel` = VALUES(`MinLevel`), `ZoneOrSort` = VALUES(`ZoneOrSort`), `Type` = VALUES(`Type`), `SourceItemId` = VALUES(`SourceItemId`), `SuggestedPlayers` = VALUES(`SuggestedPlayers`), `NextQuestIdChain` = VALUES(`NextQuestIdChain`), `RewardXPId` = VALUES(`RewardXPId`), `RewardMoney` = VALUES(`RewardMoney`), `RewardMoneyMaxLevel` = VALUES(`RewardMoneyMaxLevel`), `RewardSpell` = VALUES(`RewardSpell`), `RewardSpellCast` = VALUES(`RewardSpellCast`), `RewardHonor` = VALUES(`RewardHonor`), `RewardHonorMultiplier` = VALUES(`RewardHonorMultiplier`), `Flags` = VALUES(`Flags`), `Flags2` = VALUES(`Flags2`), `RewardTitleId` = VALUES(`RewardTitleId`), `RewardArenaPoints` = VALUES(`RewardArenaPoints`), `RewardSkillId` = VALUES(`RewardSkillId`), `RewardSkillPoints` = VALUES(`RewardSkillPoints`), `RewardReputationMask` = VALUES(`RewardReputationMask`), `QuestGiverPortrait` = VALUES(`QuestGiverPortrait`), `QuestTurnInPortrait` = VALUES(`QuestTurnInPortrait`), `RewardItemId1` = VALUES(`RewardItemId1`), `RewardItemId2` = VALUES(`RewardItemId2`), `RewardItemId3` = VALUES(`RewardItemId3`), `RewardItemId4` = VALUES(`RewardItemId4`), `RewardItemCount1` = VALUES(`RewardItemCount1`), `RewardItemCount2` = VALUES(`RewardItemCount2`), `RewardItemCount3` = VALUES(`RewardItemCount3`), `RewardItemCount4` = VALUES(`RewardItemCount4`), `RequiredSourceItemId1` = VALUES(`RequiredSourceItemId1`), `RequiredSourceItemId2` = VALUES(`RequiredSourceItemId2`), `RequiredSourceItemId3` = VALUES(`RequiredSourceItemId3`), `RequiredSourceItemId4` = VALUES(`RequiredSourceItemId4`), `RequiredSourceItemCount1` = VALUES(`RequiredSourceItemCount1`), `RequiredSourceItemCount2` = VALUES(`RequiredSourceItemCount2`), `RequiredSourceItemCount3` = VALUES(`RequiredSourceItemCount3`), `RequiredSourceItemCount4` = VALUES(`RequiredSourceItemCount4`), `RewardChoiceItemId1` = VALUES(`RewardChoiceItemId1`), `RewardChoiceItemId2` = VALUES(`RewardChoiceItemId2`), `RewardChoiceItemId3` = VALUES(`RewardChoiceItemId3`), `RewardChoiceItemId4` = VALUES(`RewardChoiceItemId4`), `RewardChoiceItemId5` = VALUES(`RewardChoiceItemId5`), `RewardChoiceItemId6` = VALUES(`RewardChoiceItemId6`), `RewardChoiceItemCount1` = VALUES(`RewardChoiceItemCount1`), `RewardChoiceItemCount2` = VALUES(`RewardChoiceItemCount2`), `RewardChoiceItemCount3` = VALUES(`RewardChoiceItemCount3`), `RewardChoiceItemCount4` = VALUES(`RewardChoiceItemCount4`), `RewardChoiceItemCount5` = VALUES(`RewardChoiceItemCount5`), `RewardChoiceItemCount6` = VALUES(`RewardChoiceItemCount6`), `RewardFactionId1` = VALUES(`RewardFactionId1`), `RewardFactionId2` = VALUES(`RewardFactionId2`), `RewardFactionId3` = VALUES(`RewardFactionId3`), `RewardFactionId4` = VALUES(`RewardFactionId4`), `RewardFactionId5` = VALUES(`RewardFactionId5`), `RewardFactionValueId1` = VALUES(`RewardFactionValueId1`), `RewardFactionValueId2` = VALUES(`RewardFactionValueId2`), `RewardFactionValueId3` = VALUES(`RewardFactionValueId3`), `RewardFactionValueId4` = VALUES(`RewardFactionValueId4`), `RewardFactionValueId5` = VALUES(`RewardFactionValueId5`), `RewardFactionValueIdOverride1` = VALUES(`RewardFactionValueIdOverride1`), `RewardFactionValueIdOverride2` = VALUES(`RewardFactionValueIdOverride2`), `RewardFactionValueIdOverride3` = VALUES(`RewardFactionValueIdOverride3`), `RewardFactionValueIdOverride4` = VALUES(`RewardFactionValueIdOverride4`), `RewardFactionValueIdOverride5` = VALUES(`RewardFactionValueIdOverride5`), `PointMapId` = VALUES(`PointMapId`), `PointX` = VALUES(`PointX`), `PointY` = VALUES(`PointY`), `PointOption` = VALUES(`PointOption`), `Title` = VALUES(`Title`), `Objectives` = VALUES(`Objectives`), `Details` = VALUES(`Details`), `EndText` = VALUES(`EndText`), `CompletedText` = VALUES(`CompletedText`), `QuestGiverTextWindow` = VALUES(`QuestGiverTextWindow`), `QuestGiverTargetName` = VALUES(`QuestGiverTargetName`), `QuestTurnTextWindow` = VALUES(`QuestTurnTextWindow`), `QuestTurnTargetName` = VALUES(`QuestTurnTargetName`), `RewardCurrencyId1` = VALUES(`RewardCurrencyId1`), `RewardCurrencyId2` = VALUES(`RewardCurrencyId2`), `RewardCurrencyId3` = VALUES(`RewardCurrencyId3`), `RewardCurrencyId4` = VALUES(`RewardCurrencyId4`), `RewardCurrencyCount1` = VALUES(`RewardCurrencyCount1`), `RewardCurrencyCount2` = VALUES(`RewardCurrencyCount2`), `RewardCurrencyCount3` = VALUES(`RewardCurrencyCount3`), `RewardCurrencyCount4` = VALUES(`RewardCurrencyCount4`), `SoundAccept` = VALUES(`SoundAccept`), `SoundTurnIn` = VALUES(`SoundTurnIn`), `LimitTime` = VALUES(`LimitTime`);
DELETE FROM quest_template_objective  WHERE QuestID=37279;
INSERT INTO quest_template_objective (`ID`, `QuestID`, `Type`, `Index`, `ObjectID`, `Amount`, `Flags`, `UnkFloat`, `Description`, `VisualEffects`) VALUES(276066, 37279, 0, 0, 79122, 1, 0, 0, "Terminer le sommet du pic Rochenoire en mode Défi", "") ON DUPLICATE KEY UPDATE `ID` = VALUES(`ID`), `QuestID` = VALUES(`QuestID`), `Type` = VALUES(`Type`), `Index` = VALUES(`Index`), `ObjectID` = VALUES(`ObjectID`), `Amount` = VALUES(`Amount`), `Flags` = VALUES(`Flags`), `UnkFloat` = VALUES(`UnkFloat`), `Description` = VALUES(`Description`), `VisualEffects` = VALUES(`VisualEffects`);
INSERT INTO quest_template (`Id`, `Method`, `Level`, `ExclusiveGroup`, `MinLevel`, `ZoneOrSort`, `Type`, `SourceItemId`, `SuggestedPlayers`, `NextQuestIdChain`, `RewardXPId`, `RewardMoney`, `RewardMoneyMaxLevel`, `RewardSpell`, `RewardSpellCast`, `RewardHonor`, `RewardHonorMultiplier`, `Flags`, `Flags2`, `RewardTitleId`, `RewardArenaPoints`, `RewardSkillId`, `RewardSkillPoints`, `RewardReputationMask`, `QuestGiverPortrait`, `QuestTurnInPortrait`, `RewardItemId1`, `RewardItemId2`, `RewardItemId3`, `RewardItemId4`, `RewardItemCount1`, `RewardItemCount2`, `RewardItemCount3`, `RewardItemCount4`, `RequiredSourceItemId1`, `RequiredSourceItemId2`, `RequiredSourceItemId3`, `RequiredSourceItemId4`, `RequiredSourceItemCount1`, `RequiredSourceItemCount2`, `RequiredSourceItemCount3`, `RequiredSourceItemCount4`, `RewardChoiceItemId1`, `RewardChoiceItemId2`, `RewardChoiceItemId3`, `RewardChoiceItemId4`, `RewardChoiceItemId5`, `RewardChoiceItemId6`, `RewardChoiceItemCount1`, `RewardChoiceItemCount2`, `RewardChoiceItemCount3`, `RewardChoiceItemCount4`, `RewardChoiceItemCount5`, `RewardChoiceItemCount6`, `RewardFactionId1`, `RewardFactionId2`, `RewardFactionId3`, `RewardFactionId4`, `RewardFactionId5`, `RewardFactionValueId1`, `RewardFactionValueId2`, `RewardFactionValueId3`, `RewardFactionValueId4`, `RewardFactionValueId5`, `RewardFactionValueIdOverride1`, `RewardFactionValueIdOverride2`, `RewardFactionValueIdOverride3`, `RewardFactionValueIdOverride4`, `RewardFactionValueIdOverride5`, `PointMapId`, `PointX`, `PointY`, `PointOption`, `Title`, `Objectives`, `Details`, `EndText`, `CompletedText`, `QuestGiverTextWindow`, `QuestGiverTargetName`, `QuestTurnTextWindow`, `QuestTurnTargetName`, `RewardCurrencyId1`, `RewardCurrencyId2`, `RewardCurrencyId3`, `RewardCurrencyId4`, `RewardCurrencyCount1`, `RewardCurrencyCount2`, `RewardCurrencyCount3`, `RewardCurrencyCount4`, `SoundAccept`, `SoundTurnIn`, `LimitTime`) VALUES(33834, 2, 91, 392, 90, 6719, 1, 0, 3, 0, 6, 236000, 6, 0, 0, 0, 0, 167772168, 0, 0, 0, 0, 0, 0, 61634, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1731, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Avis de recherche : le dard de Kliaa", "Tuez Kliaa et apportez son dard à Dyuna, au Village dEmbaari.", "Posté par : Dyuna$B$BUne cruelle reine flammerole sest installée dans les forêts septentrionales dOmbrelune.$B$BMalheureusement, cette saleté toxique sème désormais la terreur dans la forêt entre le village dEmbaari et Shattrath. Tant que les marchands des environs devront risquer leur vie, le prix des marchandises ne cessera de grimper.$B$BToute personne parvenant à tuer la bête est priée de lui arracher son dard et de lapporter comme preuve à Dyuna au village dEmbaari. Les marchands lui offriront une généreuse récompense.", "", "Parlez à Dyuna au Village dEmbaari, dans la Vallée dOmbrelune.", "Récupérez son dard.", "Kliaa", "", "", 0, 0, 0, 0, 0, 0, 0, 0, 890, 878, 0) ON DUPLICATE KEY UPDATE `Id` = VALUES(`Id`), `Method` = VALUES(`Method`), `Level` = VALUES(`Level`), `ExclusiveGroup` = VALUES(`ExclusiveGroup`), `MinLevel` = VALUES(`MinLevel`), `ZoneOrSort` = VALUES(`ZoneOrSort`), `Type` = VALUES(`Type`), `SourceItemId` = VALUES(`SourceItemId`), `SuggestedPlayers` = VALUES(`SuggestedPlayers`), `NextQuestIdChain` = VALUES(`NextQuestIdChain`), `RewardXPId` = VALUES(`RewardXPId`), `RewardMoney` = VALUES(`RewardMoney`), `RewardMoneyMaxLevel` = VALUES(`RewardMoneyMaxLevel`), `RewardSpell` = VALUES(`RewardSpell`), `RewardSpellCast` = VALUES(`RewardSpellCast`), `RewardHonor` = VALUES(`RewardHonor`), `RewardHonorMultiplier` = VALUES(`RewardHonorMultiplier`), `Flags` = VALUES(`Flags`), `Flags2` = VALUES(`Flags2`), `RewardTitleId` = VALUES(`RewardTitleId`), `RewardArenaPoints` = VALUES(`RewardArenaPoints`), `RewardSkillId` = VALUES(`RewardSkillId`), `RewardSkillPoints` = VALUES(`RewardSkillPoints`), `RewardReputationMask` = VALUES(`RewardReputationMask`), `QuestGiverPortrait` = VALUES(`QuestGiverPortrait`), `QuestTurnInPortrait` = VALUES(`QuestTurnInPortrait`), `RewardItemId1` = VALUES(`RewardItemId1`), `RewardItemId2` = VALUES(`RewardItemId2`), `RewardItemId3` = VALUES(`RewardItemId3`), `RewardItemId4` = VALUES(`RewardItemId4`), `RewardItemCount1` = VALUES(`RewardItemCount1`), `RewardItemCount2` = VALUES(`RewardItemCount2`), `RewardItemCount3` = VALUES(`RewardItemCount3`), `RewardItemCount4` = VALUES(`RewardItemCount4`), `RequiredSourceItemId1` = VALUES(`RequiredSourceItemId1`), `RequiredSourceItemId2` = VALUES(`RequiredSourceItemId2`), `RequiredSourceItemId3` = VALUES(`RequiredSourceItemId3`), `RequiredSourceItemId4` = VALUES(`RequiredSourceItemId4`), `RequiredSourceItemCount1` = VALUES(`RequiredSourceItemCount1`), `RequiredSourceItemCount2` = VALUES(`RequiredSourceItemCount2`), `RequiredSourceItemCount3` = VALUES(`RequiredSourceItemCount3`), `RequiredSourceItemCount4` = VALUES(`RequiredSourceItemCount4`), `RewardChoiceItemId1` = VALUES(`RewardChoiceItemId1`), `RewardChoiceItemId2` = VALUES(`RewardChoiceItemId2`), `RewardChoiceItemId3` = VALUES(`RewardChoiceItemId3`), `RewardChoiceItemId4` = VALUES(`RewardChoiceItemId4`), `RewardChoiceItemId5` = VALUES(`RewardChoiceItemId5`), `RewardChoiceItemId6` = VALUES(`RewardChoiceItemId6`), `RewardChoiceItemCount1` = VALUES(`RewardChoiceItemCount1`), `RewardChoiceItemCount2` = VALUES(`RewardChoiceItemCount2`), `RewardChoiceItemCount3` = VALUES(`RewardChoiceItemCount3`), `RewardChoiceItemCount4` = VALUES(`RewardChoiceItemCount4`), `RewardChoiceItemCount5` = VALUES(`RewardChoiceItemCount5`), `RewardChoiceItemCount6` = VALUES(`RewardChoiceItemCount6`), `RewardFactionId1` = VALUES(`RewardFactionId1`), `RewardFactionId2` = VALUES(`RewardFactionId2`), `RewardFactionId3` = VALUES(`RewardFactionId3`), `RewardFactionId4` = VALUES(`RewardFactionId4`), `RewardFactionId5` = VALUES(`RewardFactionId5`), `RewardFactionValueId1` = VALUES(`RewardFactionValueId1`), `RewardFactionValueId2` = VALUES(`RewardFactionValueId2`), `RewardFactionValueId3` = VALUES(`RewardFactionValueId3`), `RewardFactionValueId4` = VALUES(`RewardFactionValueId4`), `RewardFactionValueId5` = VALUES(`RewardFactionValueId5`), `RewardFactionValueIdOverride1` = VALUES(`RewardFactionValueIdOverride1`), `RewardFactionValueIdOverride2` = VALUES(`RewardFactionValueIdOverride2`), `RewardFactionValueIdOverride3` = VALUES(`RewardFactionValueIdOverride3`), `RewardFactionValueIdOverride4` = VALUES(`RewardFactionValueIdOverride4`), `RewardFactionValueIdOverride5` = VALUES(`RewardFactionValueIdOverride5`), `PointMapId` = VALUES(`PointMapId`), `PointX` = VALUES(`PointX`), `PointY` = VALUES(`PointY`), `PointOption` = VALUES(`PointOption`), `Title` = VALUES(`Title`), `Objectives` = VALUES(`Objectives`), `Details` = VALUES(`Details`), `EndText` = VALUES(`EndText`), `CompletedText` = VALUES(`CompletedText`), `QuestGiverTextWindow` = VALUES(`QuestGiverTextWindow`), `QuestGiverTargetName` = VALUES(`QuestGiverTargetName`), `QuestTurnTextWindow` = VALUES(`QuestTurnTextWindow`), `QuestTurnTargetName` = VALUES(`QuestTurnTargetName`), `RewardCurrencyId1` = VALUES(`RewardCurrencyId1`), `RewardCurrencyId2` = VALUES(`RewardCurrencyId2`), `RewardCurrencyId3` = VALUES(`RewardCurrencyId3`), `RewardCurrencyId4` = VALUES(`RewardCurrencyId4`), `RewardCurrencyCount1` = VALUES(`RewardCurrencyCount1`), `RewardCurrencyCount2` = VALUES(`RewardCurrencyCount2`), `RewardCurrencyCount3` = VALUES(`RewardCurrencyCount3`), `RewardCurrencyCount4` = VALUES(`RewardCurrencyCount4`), `SoundAccept` = VALUES(`SoundAccept`), `SoundTurnIn` = VALUES(`SoundTurnIn`), `LimitTime` = VALUES(`LimitTime`);
DELETE FROM quest_template_objective  WHERE QuestID=33834;
INSERT INTO quest_template_objective (`ID`, `QuestID`, `Type`, `Index`, `ObjectID`, `Amount`, `Flags`, `UnkFloat`, `Description`, `VisualEffects`) VALUES(272050, 33834, 1, 0, 108484, 1, 1, 0, "", "") ON DUPLICATE KEY UPDATE `ID` = VALUES(`ID`), `QuestID` = VALUES(`QuestID`), `Type` = VALUES(`Type`), `Index` = VALUES(`Index`), `ObjectID` = VALUES(`ObjectID`), `Amount` = VALUES(`Amount`), `Flags` = VALUES(`Flags`), `UnkFloat` = VALUES(`UnkFloat`), `Description` = VALUES(`Description`), `VisualEffects` = VALUES(`VisualEffects`);