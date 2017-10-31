namespace WowPacketParser.Enums
{
    public enum DifficultyChangeType
    {
        PlayerDifficulty1 = 0,
        SpellDuration = 1,
        WorldState = 2,
        Encounter = 3,
        Combat = 4,
        Unknown1 = 5,
        Time = 6,
        Unknown2 = 7,
        MapDifficulty = 8,
        PlayerDifficulty2 = 9,
    }

    public enum DifficultyChangeType434
    {
        Cooldown                        = 0, // ERR_DIFFICULTY_CHANGE_COOLDOWN_S
        WorldState                      = 1,
        Encounter                       = 2,
        Combat                          = 3,
        PlayerBusy                      = 4,
        Time                            = 5,
        AlreadyStarted                  = 6,
        MapDifficultyRequirement        = 7,
        PlayerAlreadyLocked             = 8, // ERR_DIFFICULTY_CHANGE_OTHER_HEROIC_S
        HeroicInstanceAlreadyRunning    = 9,
        DisabledInLFG                   = 10,
        DifficultyChanged               = 11
    }

    public enum DifficultyTypes
    {
        DifficultyNone          = 0,
        DifficultyRegular       = 1,
        DifficultyHeroic        = 2,
        DifficultyMan10         = 3,
        DifficultyMan25         = 4,
        DifficultyMan10Heroic   = 5,
        DifficultyMan25Heroic   = 6,
        DifficultyLFR           = 7,
        DifficultyChallenge     = 8,
        DifficultyMan40         = 9,
        DifficultyUnk           = 10,
        DifficultyScenarHeroic  = 11,
        DifficultyScenar        = 12,
        DifficultyUnk2          = 13,
        DifficultyDynamic       = 14
    }

    public enum GroupTypes
    {
        Normal              = 0,
        Battleground        = 1,
        Raid                = 2,
        BattlegroundRaid    = 3,
        Unk1                = 4,
        LFG                 = 5,
        EveryoneIsAssistant = 40,
    }
}
