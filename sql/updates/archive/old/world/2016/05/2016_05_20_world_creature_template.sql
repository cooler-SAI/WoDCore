DELETE FROM `conditions` WHERE SourceEntry = 77346;
INSERT INTO `conditions`
            (`SourceTypeOrReferenceId`,
             `SourceGroup`,
             `SourceEntry`,
             `SourceId`,
             `ElseGroup`,
             `ConditionTypeOrReference`,
             `ConditionTarget`,
             `ConditionValue1`,
             `ConditionValue2`,
             `ConditionValue3`,
             `NegativeCondition`,
             `ErrorTextId`,
             `ScriptName`,
             `Comment`)
VALUES ('17',
        '0',
        '77346',
        '0',
        '0',
        '29',
        '0',
        '41255',
        '30',
        '0',
        '0',
        '0',
        '',
        'ISignal Rocket only usable on Sethria slain');