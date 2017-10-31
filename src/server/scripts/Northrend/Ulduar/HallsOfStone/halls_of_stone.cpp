////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"
#include "halls_of_stone.h"

enum Texts
{
    SAY_KILL_1                          = -1599016,
    SAY_KILL_2                          = -1599017,
    SAY_KILL_3                          = -1599018,
    SAY_LOW_HEALTH                      = -1599019,
    SAY_DEATH                           = -1599020,
    SAY_PLAYER_DEATH_1                  = -1599021,
    SAY_PLAYER_DEATH_2                  = -1599022,
    SAY_PLAYER_DEATH_3                  = -1599023,
    SAY_ESCORT_START                    = -1599024,

    SAY_SPAWN_DWARF                     = -1599025,
    SAY_SPAWN_TROGG                     = -1599026,
    SAY_SPAWN_OOZE                      = -1599027,
    SAY_SPAWN_EARTHEN                   = -1599028,

    SAY_EVENT_INTRO_1                   = -1599029,
    SAY_EVENT_INTRO_2                   = -1599030,
    SAY_EVENT_INTRO_3_ABED              = -1599031,

    SAY_EVENT_A_1                       = -1599032,
    SAY_EVENT_A_2_KADD                  = -1599033,
    SAY_EVENT_A_3                       = -1599034,

    SAY_EVENT_B_1                       = -1599035,
    SAY_EVENT_B_2_MARN                  = -1599036,
    SAY_EVENT_B_3                       = -1599037,

    SAY_EVENT_C_1                       = -1599038,
    SAY_EVENT_C_2_ABED                  = -1599039,
    SAY_EVENT_C_3                       = -1599040,

    SAY_EVENT_D_1                       = -1599041,
    SAY_EVENT_D_2_ABED                  = -1599042,
    SAY_EVENT_D_3                       = -1599043,
    SAY_EVENT_D_4_ABED                  = -1599044,

    SAY_EVENT_END_01                    = -1599045,
    SAY_EVENT_END_02                    = -1599046,
    SAY_EVENT_END_03_ABED               = -1599047,
    SAY_EVENT_END_04                    = -1599048,
    SAY_EVENT_END_05_ABED               = -1599049,
    SAY_EVENT_END_06                    = -1599050,
    SAY_EVENT_END_07_ABED               = -1599051,
    SAY_EVENT_END_08                    = -1599052,
    SAY_EVENT_END_09_KADD               = -1599053,
    SAY_EVENT_END_10                    = -1599054,
    SAY_EVENT_END_11_KADD               = -1599055,
    SAY_EVENT_END_12                    = -1599056,
    SAY_EVENT_END_13_KADD               = -1599057,
    SAY_EVENT_END_14                    = -1599058,
    SAY_EVENT_END_15_MARN               = -1599059,
    SAY_EVENT_END_16                    = -1599060,
    SAY_EVENT_END_17_MARN               = -1599061,
    SAY_EVENT_END_18                    = -1599062,
    SAY_EVENT_END_19_MARN               = -1599063,
    SAY_EVENT_END_20                    = -1599064,
    SAY_EVENT_END_21_ABED               = -1599065,

    SAY_VICTORY_SJONNIR_1               = -1599066,
    SAY_VICTORY_SJONNIR_2               = -1599067,

    SAY_ENTRANCE_MEET                   = -1599068,

    TEXT_ID_START                       = 13100,
    TEXT_ID_PROGRESS                    = 13101
};

enum BrannCreatures
{
    CREATURE_TRIBUNAL_OF_THE_AGES       = 28234,
    CREATURE_BRANN_BRONZEBEARD          = 28070,
    CREATURE_DARK_MATTER                = 28235,
    CREATURE_DARK_MATTER_TARGET         = 28237,
    CREATURE_SEARING_GAZE_TARGET        = 28265,
    CREATURE_DARK_RUNE_PROTECTOR        = 27983,
    CREATURE_DARK_RUNE_STORMCALLER      = 27984,
    CREATURE_IRON_GOLEM_CUSTODIAN       = 27985
};

enum Spells
{
    SPELL_STEALTH                       = 58506,
    //Kadrak
    SPELL_GLARE_OF_THE_TRIBUNAL         = 50988,
    H_SPELL_GLARE_OF_THE_TRIBUNAL       = 59870,
    //Marnak
    SPELL_DARK_MATTER_VISUAL            = 51000,
    SPELL_DARK_MATTER_DUMMY             = 51001,
    SPELL_DARK_MATTER                   = 51012,
    H_SPELL_DARK_MATTER                 = 59868,
    //Abedneum
    SPELL_SEARING_GAZE_DUMMY            = 51136,
    SPELL_SEARING_GAZE                  = 51125,
    H_SPELL_SEARING_GAZE                = 59866,
    SPELL_REWARD_ACHIEVEMENT            = 59046
};

enum Quests
{
    QUEST_HALLS_OF_STONE                = 13207
};

#define GOSSIP_ITEM_START               "Brann, it would be our honor!"
#define GOSSIP_ITEM_PROGRESS            "Let's move Brann, enough of the history lessons!"
#define DATA_BRANN_SPARKLIN_NEWS        1

static Position SpawnLocations[]=
{
    {946.992f, 397.016f, 208.374f, 0.0f},
    {960.748f, 382.944f, 208.374f, 0.0f}
};

class mob_tribuna_controller : public CreatureScript
{
public:
    mob_tribuna_controller() : CreatureScript("mob_tribuna_controller") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_tribuna_controllerAI(creature);
    }

    struct mob_tribuna_controllerAI : public ScriptedAI
    {
        mob_tribuna_controllerAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            SetCombatMovement(false);
        }

        InstanceScript* instance;

        uint64 uiDarkMatterGUID;

        uint32 uiKaddrakEncounterTimer;
        uint32 uiMarnakEncounterTimer;
        uint32 uiAbedneumEncounterTimer;
        uint32 uiLoadingTimer;

        bool bKaddrakActivated;
        bool bMarnakActivated;
        bool bAbedneumActivated;
        bool bLoadingDarkMatter;

        std::list<uint64> KaddrakGUIDList;
        std::list<uint64> AbedneumGUIDList;
        std::list<uint64> MarnakGUIDList;

        void Reset()
        {
            uiKaddrakEncounterTimer = 1500;
            uiMarnakEncounterTimer = 10*IN_MILLISECONDS;
            uiAbedneumEncounterTimer = 10*IN_MILLISECONDS;
            uiLoadingTimer = 5*IN_MILLISECONDS;

            bKaddrakActivated = false;
            bMarnakActivated = false;
            bAbedneumActivated = false;
            bLoadingDarkMatter = false;

            uiDarkMatterGUID = 0;

            if (instance)
            {
                instance->HandleGameObject(instance->GetData64(DATA_GO_KADDRAK), false);
                instance->HandleGameObject(instance->GetData64(DATA_GO_MARNAK), false);
                instance->HandleGameObject(instance->GetData64(DATA_GO_ABEDNEUM), false);
                instance->HandleGameObject(instance->GetData64(DATA_GO_SKY_FLOOR), false);
            }

            KaddrakGUIDList.clear();
            AbedneumGUIDList.clear();
            MarnakGUIDList.clear();
        }

        void UpdateAI(uint32 const diff)
        {
            if (bKaddrakActivated)
            {
                if (uiKaddrakEncounterTimer <= diff)
                {
                    if (!KaddrakGUIDList.empty())
                        for (std::list<uint64>::const_iterator itr = KaddrakGUIDList.begin(); itr != KaddrakGUIDList.end(); ++itr)
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                                if (Creature* kaddrak = Unit::GetCreature(*me, *itr))
                                    if (kaddrak->isAlive())
                                        kaddrak->CastSpell(target, DUNGEON_MODE(SPELL_GLARE_OF_THE_TRIBUNAL, H_SPELL_GLARE_OF_THE_TRIBUNAL), true);
                        }

                    uiKaddrakEncounterTimer = 3*IN_MILLISECONDS;
                } else uiKaddrakEncounterTimer -= diff;
            }

            if (bMarnakActivated)
            {
                if (uiMarnakEncounterTimer <= diff)
                {
                    if (!MarnakGUIDList.empty())
                        if (Creature* matter = me->SummonCreature(CREATURE_DARK_MATTER, 904.07f, 351.363f, 214.77f, 0, TEMPSUMMON_TIMED_DESPAWN, 20*IN_MILLISECONDS))
                        {
                            matter->SetDisplayId(17200);
                            matter->setFaction(16);
                            uiDarkMatterGUID = matter->GetGUID();

                            bLoadingDarkMatter = true;

                            for (std::list<uint64>::const_iterator itr = MarnakGUIDList.begin(); itr != MarnakGUIDList.end(); ++itr)
                            {
                                if (Creature* marnak = Unit::GetCreature(*me, *itr))
                                    if (marnak->isAlive())
                                        marnak->CastSpell(matter, SPELL_DARK_MATTER_DUMMY, true);
                            }
                        }

                    uiMarnakEncounterTimer = urand(20000, 25000);
                } else uiMarnakEncounterTimer -= diff;

                if (bLoadingDarkMatter && uiDarkMatterGUID)
                {
                    if (uiLoadingTimer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            if (Creature* matter = Unit::GetCreature(*me, uiDarkMatterGUID))
                            {
                                float x, y, z;
                                target->GetPosition(x, y, z);

                                me->SummonCreature(CREATURE_DARK_MATTER_TARGET, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN, 20*IN_MILLISECONDS);
                                matter->GetMotionMaster()->MovePoint(0, x, y, z);
                            }

                        bLoadingDarkMatter = false;
                        uiLoadingTimer = 5*IN_MILLISECONDS;
                    } else uiLoadingTimer -= diff;
                }
            }

            if (bAbedneumActivated)
            {
                if (uiAbedneumEncounterTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        if (!AbedneumGUIDList.empty())
                            if (Creature* gaze = me->SummonCreature(CREATURE_SEARING_GAZE_TARGET, target->GetPositionX(), target->GetPositionY(),
                                target->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 11*IN_MILLISECONDS))
                            {
                                gaze->SetDisplayId(11686);
                                gaze->setFaction(16);

                                for (std::list<uint64>::const_iterator itr = AbedneumGUIDList.begin(); itr != AbedneumGUIDList.end(); ++itr)
                                {
                                    if (Creature* abedneum = Unit::GetCreature(*me, *itr))
                                        if (abedneum->isAlive())
                                            abedneum->CastSpell(gaze, SPELL_SEARING_GAZE_DUMMY, true);
                                }
                            }

                    uiAbedneumEncounterTimer = urand(17500, 22500);
                } else uiAbedneumEncounterTimer -= diff;
            }
        }
    };

};

class npc_brann_hos : public CreatureScript
{
public:
    npc_brann_hos() : CreatureScript("npc_brann_hos") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_brann_hosAI(creature);
    }

    struct npc_brann_hosAI : public npc_escortAI
    {
        npc_brann_hosAI(Creature* c) : npc_escortAI(c)
        {
            instance = c->GetInstanceScript();
        }

        uint32 uiStep;
        uint32 uiPhaseTimer;

        uint64 uiControllerGUID;
        std::list<uint64> lSummonGUIDList;

        InstanceScript* instance;

        bool bIsBattle;
        bool bIsLowHP;
        bool brannSparklinNews;

        void Reset()
        {
            if (!HasEscortState(STATE_ESCORT_ESCORTING))
            {
                bIsLowHP = false;
                bIsBattle = false;
                brannSparklinNews = true;
                uiStep = 0;
                uiPhaseTimer = 0;
                uiControllerGUID = 0;

                DespawnSummon();

                if (instance)
                    if (instance->GetData(DATA_BRANN_EVENT) != DONE) // dont reset if already done
                        instance->SetData(DATA_BRANN_EVENT, NOT_STARTED);
            }
        }

        void DespawnSummon()
        {
            if (lSummonGUIDList.empty() || !instance)
                return;

            for (std::list<uint64>::const_iterator itr = lSummonGUIDList.begin(); itr != lSummonGUIDList.end(); ++itr)
            {
                Creature* summon = Unit::GetCreature(*me, *itr);
                if (summon && summon->isAlive())
                {
                    if (instance->GetData(DATA_BRANN_EVENT) == DONE && (summon->GetEntry() == CREATURE_KADDRAK || summon->GetEntry() == CREATURE_MARNAK
                        || summon->GetEntry() == CREATURE_ABEDNEUM))
                        continue;

                    summon->DespawnOrUnsummon();
                }
            }
            lSummonGUIDList.clear();
        }

        void WaypointReached(uint32 uiPointId)
        {
            switch (uiPointId)
            {
                case 7:
                    SpawnSummon(4);
                    break;
                case 13:
                    DoScriptText(SAY_EVENT_INTRO_1, me);
                    SetEscortPaused(true);
                    JumpToNextStep(20000);
                    break;
                case 17:
                    DoScriptText(SAY_EVENT_INTRO_2, me);
                    if (instance)
                        instance->HandleGameObject(instance->GetData64(DATA_GO_TRIBUNAL_CONSOLE), true);
                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                    SetEscortPaused(true);
                    JumpToNextStep(8500);
                    break;
                case 18:
                    SetEscortPaused(true);
                    break;
            }
        }

        void SpawnSummon(uint32 uiType)
        {
            uint32 rnd = urand(0, 1);

            switch (uiType)
            {
                case 1:
                {
                    uint32 uiSpawnNumber = DUNGEON_MODE(2,3);
                    for (uint8 i = 0; i < uiSpawnNumber; ++i)
                        me->SummonCreature(CREATURE_DARK_RUNE_PROTECTOR, SpawnLocations[rnd], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                    me->SummonCreature(CREATURE_DARK_RUNE_STORMCALLER, SpawnLocations[rnd], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                    break;
                }
                case 2:
                    for (uint8 i = 0; i < 2; ++i)
                        me->SummonCreature(CREATURE_DARK_RUNE_STORMCALLER, SpawnLocations[rnd], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                    break;
                case 3:
                    me->SummonCreature(CREATURE_IRON_GOLEM_CUSTODIAN, SpawnLocations[rnd], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                    break;
                case 4:
                    if (Creature* creature = me->SummonCreature(CREATURE_TRIBUNAL_OF_THE_AGES, 910.126f, 345.795f, 237.928f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                    {
                        uiControllerGUID = creature->GetGUID();
                        creature->SetVisible(false);
                    }
                    break;
                case 5:
                {
                    if (Creature* controller = Unit::GetCreature(*me, uiControllerGUID))
                    {
                        uint32 uiPositionCounter = 0;
                        for (uint8 i = 0; i < 2; ++i)
                        {
                            Creature* kaddrak = 0;

                            if (uiPositionCounter == 0)
                                kaddrak = me->SummonCreature(CREATURE_KADDRAK, 927.265f, 333.200f, 218.780f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                            else
                                kaddrak = me->SummonCreature(CREATURE_KADDRAK, 921.745f, 328.076f, 218.780f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);

                            if (kaddrak)
                                CAST_AI(mob_tribuna_controller::mob_tribuna_controllerAI, controller->AI())->KaddrakGUIDList.push_back(kaddrak->GetGUID());

                            ++uiPositionCounter;
                        }

                        controller->AI()->DoZoneInCombat(controller, 100.0f);
                        CAST_AI(mob_tribuna_controller::mob_tribuna_controllerAI, controller->AI())->bKaddrakActivated = true;
                    }
                    break;
                }
                case 6:
                {
                    if (Creature* controller = Unit::GetCreature(*me, uiControllerGUID))
                    {
                        uint32 uiPositionCounter = 0;
                        for (uint8 i = 0; i < 2; ++i)
                        {
                            Creature* marnak = 0;

                            if (uiPositionCounter == 0)
                                marnak = me->SummonCreature(CREATURE_MARNAK, 891.309f, 359.382f, 217.422f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                            else
                                marnak = me->SummonCreature(CREATURE_MARNAK, 895.834f, 363.436f, 217.422f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);

                            if (marnak)
                                CAST_AI(mob_tribuna_controller::mob_tribuna_controllerAI, controller->AI())->MarnakGUIDList.push_back(marnak->GetGUID());

                            ++uiPositionCounter;
                        }

                        controller->AI()->DoZoneInCombat(controller, 100.0f);
                        CAST_AI(mob_tribuna_controller::mob_tribuna_controllerAI, controller->AI())->bMarnakActivated = true;
                    }
                    break;
                }
                case 7:
                {
                    if (Creature* controller = Unit::GetCreature(*me, uiControllerGUID))
                    {
                        uint32 uiPositionCounter = 0;
                        for (uint8 i = 0; i < 2; ++i)
                        {
                            Creature* abedneum = 0;

                            if (uiPositionCounter == 0)
                                abedneum = me->SummonCreature(CREATURE_ABEDNEUM, 897.865f, 328.341f, 223.84f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
                            else
                                abedneum = me->SummonCreature(CREATURE_ABEDNEUM, 893.012f, 332.804f, 223.545f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);

                            if (abedneum)
                                CAST_AI(mob_tribuna_controller::mob_tribuna_controllerAI, controller->AI())->AbedneumGUIDList.push_back(abedneum->GetGUID());

                            ++uiPositionCounter;
                        }

                        controller->AI()->DoZoneInCombat(controller, 100.0f);
                        CAST_AI(mob_tribuna_controller::mob_tribuna_controllerAI, controller->AI())->bAbedneumActivated = true;
                    }
                    break;
                }
            }
        }

        void JustSummoned(Creature* summoned)
        {
            lSummonGUIDList.push_back(summoned->GetGUID());

            switch (summoned->GetEntry())
            {
                case CREATURE_DARK_RUNE_PROTECTOR:
                case CREATURE_DARK_RUNE_STORMCALLER:
                case CREATURE_IRON_GOLEM_CUSTODIAN:
                    summoned->AddThreat(me, 1.0f);
                    summoned->AI()->AttackStart(me);
                    break;
            }
        }

        void JumpToNextStep(uint32 uiTimer)
        {
            uiPhaseTimer = uiTimer;
            ++uiStep;
        }

        void JustDied(Unit* /*victim*/)
        {
            me->Respawn();
        }

        void StartWP()
        {
            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            me->setFaction(113);
            SetEscortPaused(false);
            uiStep = 1;
            Start();
        }

        void DamageTaken(Unit* /*done_by*/, uint32 & /*damage*/, SpellInfo const*  /*p_SpellInfo*/)
        {
            if (brannSparklinNews)
                brannSparklinNews = false;
        }

        uint32 GetData(uint32 type)
        {
            if (type == DATA_BRANN_SPARKLIN_NEWS)
                return brannSparklinNews ? 1 : 0;

            return 0;
        }

        void UpdateEscortAI(const uint32 uiDiff)
        {
            if (uiPhaseTimer <= uiDiff)
            {
                if (!instance)
                    return;

                switch (uiStep)
                {
                    case 1:
                        if (instance->GetData(DATA_BRANN_EVENT) != NOT_STARTED)
                            return;
                        instance->SetData(DATA_BRANN_EVENT, IN_PROGRESS);
                        bIsBattle = false;
                        DoScriptText(SAY_ESCORT_START, me);
                        SetRun(true);
                        JumpToNextStep(0);
                        break;
                    case 3:
                        SetEscortPaused(false);
                        JumpToNextStep(0);
                        break;
                    case 5:
                        if (Creature* pTemp = (Unit::GetCreature(*me, instance->GetData64(DATA_ABEDNEUM))))
                            DoScriptText(SAY_EVENT_INTRO_3_ABED, pTemp);
                        JumpToNextStep(8500);
                        break;
                    case 6:
                        DoScriptText(SAY_EVENT_A_1, me);
                        JumpToNextStep(6500);
                        break;
                    case 7:
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_KADDRAK)))
                            DoScriptText(SAY_EVENT_A_2_KADD, pTemp);
                        JumpToNextStep(12500);
                        break;
                    case 8:
                        DoScriptText(SAY_EVENT_A_3, me);
                        SpawnSummon(5);
                        JumpToNextStep(5000);
                        break;
                    case 9:
                        me->SetReactState(REACT_PASSIVE);
                        SpawnSummon(1);
                        JumpToNextStep(20000);
                        break;
                    case 10:
                        DoScriptText(SAY_EVENT_B_1, me);
                        JumpToNextStep(6000);
                        break;
                    case 11:
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_MARNAK)))
                            DoScriptText(SAY_EVENT_B_2_MARN, pTemp);
                        SpawnSummon(1);
                        JumpToNextStep(20000);
                        break;
                    case 12:
                        DoScriptText(SAY_EVENT_B_3, me);
                        SpawnSummon(6);
                        JumpToNextStep(10000);
                        break;
                    case 13:
                        SpawnSummon(1);
                        JumpToNextStep(10000);
                        break;
                    case 14:
                        SpawnSummon(2);
                        JumpToNextStep(20000);
                        break;
                    case 15:
                        DoScriptText(SAY_EVENT_C_1, me);
                        SpawnSummon(1);
                        JumpToNextStep(10000);
                        break;
                    case 16:
                        SpawnSummon(2);
                        JumpToNextStep(20000);
                        break;
                    case 17:
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_ABEDNEUM)))
                            DoScriptText(SAY_EVENT_C_2_ABED, pTemp);
                        SpawnSummon(1);
                        JumpToNextStep(20000);
                        break;
                    case 18:
                        DoScriptText(SAY_EVENT_C_3, me);
                        SpawnSummon(7);
                        JumpToNextStep(5000);
                        break;
                    case 19:
                        SpawnSummon(2);
                        JumpToNextStep(10000);
                        break;
                    case 20:
                        SpawnSummon(1);
                        JumpToNextStep(15000);
                        break;
                    case 21:
                        DoScriptText(SAY_EVENT_D_1, me);
                        SpawnSummon(3);
                        JumpToNextStep(20000);
                        break;
                    case 22:
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_ABEDNEUM)))
                            DoScriptText(SAY_EVENT_D_2_ABED, pTemp);
                        SpawnSummon(1);
                        JumpToNextStep(5000);
                        break;
                    case 23:
                        SpawnSummon(2);
                        JumpToNextStep(15000);
                        break;
                    case 24:
                        DoScriptText(SAY_EVENT_D_3, me);
                        SpawnSummon(3);
                        JumpToNextStep(5000);
                        break;
                    case 25:
                        SpawnSummon(1);
                        JumpToNextStep(5000);
                        break;
                    case 26:
                        SpawnSummon(2);
                        JumpToNextStep(10000);
                        break;
                    case 27:
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_ABEDNEUM)))
                            DoScriptText(SAY_EVENT_D_4_ABED, pTemp);
                        SpawnSummon(1);
                        JumpToNextStep(10000);
                        break;
                    case 28:
                        me->SetReactState(REACT_DEFENSIVE);
                        DoScriptText(SAY_EVENT_END_01, me);
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        instance->HandleGameObject(instance->GetData64(DATA_GO_SKY_FLOOR), true);
                        if (Creature* pTemp = Unit::GetCreature(*me, uiControllerGUID))
                            pTemp->DealDamage(pTemp, pTemp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        bIsBattle = true;
                        SetEscortPaused(false);
                        JumpToNextStep(6500);
                        break;
                    case 29:
                        DoScriptText(SAY_EVENT_END_02, me);
                        me->CastSpell(me, SPELL_REWARD_ACHIEVEMENT, true);

                        if (GameObject* go = me->SummonGameObject(IsHeroic() ? GO_TRIBUNAL_CHEST_HERO : GO_TRIBUNAL_CHEST, 880.406f, 345.164f, 203.706f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, DAY))
                            go->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);

                        instance->HandleGameObject(instance->GetData64(DATA_GO_ABEDNEUM), true);
                        instance->SetData(DATA_BRANN_EVENT, DONE);
                        DespawnSummon();

                        // Achievement criteria is with spell 59046 which does not exist.
                        // There is thus no way it can be given by casting the spell on the players.
                        instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, 59046);

                        JumpToNextStep(5500);
                        break;
                    case 30:
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_ABEDNEUM)))
                            DoScriptText(SAY_EVENT_END_03_ABED, pTemp);
                        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        JumpToNextStep(8500);
                        break;
                    case 31:
                        DoScriptText(SAY_EVENT_END_04, me);
                        JumpToNextStep(11500);
                        break;
                    case 32:
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_ABEDNEUM)))
                            DoScriptText(SAY_EVENT_END_05_ABED, pTemp);
                        JumpToNextStep(11500);
                        break;
                    case 33:
                        DoScriptText(SAY_EVENT_END_06, me);
                        JumpToNextStep(4500);
                        break;
                    case 34:
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_ABEDNEUM)))
                            DoScriptText(SAY_EVENT_END_07_ABED, pTemp);
                        JumpToNextStep(22500);
                        break;
                    case 35:
                        DoScriptText(SAY_EVENT_END_08, me);
                        JumpToNextStep(7500);
                        break;
                    case 36:
                        instance->HandleGameObject(instance->GetData64(DATA_GO_KADDRAK), true);
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_KADDRAK)))
                            DoScriptText(SAY_EVENT_END_09_KADD, pTemp);
                        JumpToNextStep(18500);
                        break;
                    case 37:
                        DoScriptText(SAY_EVENT_END_10, me);
                        JumpToNextStep(5500);
                        break;
                    case 38:
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_KADDRAK)))
                            DoScriptText(SAY_EVENT_END_11_KADD, pTemp);
                        JumpToNextStep(20500);
                        break;
                    case 39:
                        DoScriptText(SAY_EVENT_END_12, me);
                        JumpToNextStep(2500);
                        break;
                    case 40:
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_KADDRAK)))
                            DoScriptText(SAY_EVENT_END_13_KADD, pTemp);
                        JumpToNextStep(19500);
                        break;
                    case 41:
                        DoScriptText(SAY_EVENT_END_14, me);
                        JumpToNextStep(10500);
                        break;
                    case 42:
                        instance->HandleGameObject(instance->GetData64(DATA_GO_MARNAK), true);
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_MARNAK)))
                            DoScriptText(SAY_EVENT_END_15_MARN, pTemp);
                        JumpToNextStep(6500);
                        break;
                    case 43:
                        DoScriptText(SAY_EVENT_END_16, me);
                        JumpToNextStep(6500);
                        break;
                    case 44:
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_MARNAK)))
                            DoScriptText(SAY_EVENT_END_17_MARN, pTemp);
                        JumpToNextStep(25500);
                        break;
                    case 45:
                        DoScriptText(SAY_EVENT_END_18, me);
                        JumpToNextStep(23500);
                        break;
                    case 46:
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_MARNAK)))
                            DoScriptText(SAY_EVENT_END_19_MARN, pTemp);
                        JumpToNextStep(3500);
                        break;
                    case 47:
                        DoScriptText(SAY_EVENT_END_20, me);
                        JumpToNextStep(8500);
                        break;
                    case 48:
                        if (Creature* pTemp = Unit::GetCreature(*me, instance->GetData64(DATA_ABEDNEUM)))
                            DoScriptText(SAY_EVENT_END_21_ABED, pTemp);
                        JumpToNextStep(5500);
                        break;
                    case 49:
                    {
                        instance->HandleGameObject(instance->GetData64(DATA_GO_KADDRAK), false);
                        instance->HandleGameObject(instance->GetData64(DATA_GO_MARNAK), false);
                        instance->HandleGameObject(instance->GetData64(DATA_GO_ABEDNEUM), false);
                        instance->HandleGameObject(instance->GetData64(DATA_GO_SKY_FLOOR), false);

                        Player* pPlayer = GetPlayerForEscort();
                        if (pPlayer)
                            pPlayer->GroupEventHappens(QUEST_HALLS_OF_STONE, me);

                        JumpToNextStep(10000); // (180000);
                        break;
                    }
                    case 50:
                        SetEscortPaused(false);
                        break;
                }
            } else uiPhaseTimer -= uiDiff;

            if (!bIsLowHP && HealthBelowPct(30))
            {
                DoScriptText(SAY_LOW_HEALTH, me);
                bIsLowHP = true;
            }
            else if (bIsLowHP && !HealthBelowPct(30))
                bIsLowHP = false;

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                CAST_AI(npc_brann_hos::npc_brann_hosAI, pCreature->AI())->StartWP();
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                CAST_AI(npc_brann_hos::npc_brann_hosAI, pCreature->AI())->uiStep = 49;
                CAST_AI(npc_brann_hos::npc_brann_hosAI, pCreature->AI())->uiPhaseTimer = 1000;
                break;
        }

        pPlayer->CLOSE_GOSSIP_MENU();

        return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        InstanceScript* instance = pCreature->GetInstanceScript();

        uint32 brann_event_data = instance->GetData(DATA_BRANN_EVENT);
        uint32 uiStepCount = CAST_AI(npc_brann_hos::npc_brann_hosAI, pCreature->AI())->uiStep;

        if (brann_event_data == NOT_STARTED && uiStepCount == 0)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_START, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        // skip history lessons
        if (uiStepCount > 29 && uiStepCount < 49)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_PROGRESS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

        pPlayer->SEND_GOSSIP_MENU(TEXT_ID_START, pCreature->GetGUID());

        return true;
    }

};

class mob_dark_matter : public CreatureScript
{
public:
    mob_dark_matter() : CreatureScript("mob_dark_matter") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_dark_matterAI(pCreature);
    }

    struct mob_dark_matterAI : public ScriptedAI
    {
        mob_dark_matterAI(Creature *pCreature) : ScriptedAI(pCreature) { }

        bool bAura;
        bool bCasted;
        uint32 uiCheckTimer;

        void Reset()
        {
            bAura = false;
            bCasted = false;
            uiCheckTimer = 5500;
            me->SetReactState(REACT_PASSIVE);
            me->SetSpeed(MOVE_FLIGHT, 0.7f, true);
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiCheckTimer <= diff)
            {
                if (!bAura)
                {
                    me->RemoveAllAuras();
                    me->AddAura(SPELL_DARK_MATTER_VISUAL, me);
                    bAura = true;
                }

                if (bCasted)
                    me->DisappearAndDie();

                if (Creature* pTarget = me->FindNearestCreature(CREATURE_DARK_MATTER_TARGET, 1.0f, true))
                {
                    if (!bCasted) // prevent double cast
                        DoCast(DUNGEON_MODE(SPELL_DARK_MATTER, H_SPELL_DARK_MATTER));
                    bCasted = true;
                }

                uiCheckTimer = 1*IN_MILLISECONDS;
            }
            else uiCheckTimer -= diff;
        }
    };
};

class mob_searing_gaze : public CreatureScript
{
public:
    mob_searing_gaze() : CreatureScript("mob_searing_gaze") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_searing_gazeAI(pCreature);
    }

    struct mob_searing_gazeAI : public ScriptedAI
    {
        mob_searing_gazeAI(Creature *pCreature) : ScriptedAI(pCreature) { }

        uint32 uiCheckTimer;

        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);
            uiCheckTimer = 1*IN_MILLISECONDS;
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiCheckTimer <= diff)
            {
                DoCast(DUNGEON_MODE(SPELL_SEARING_GAZE, H_SPELL_SEARING_GAZE));
                uiCheckTimer = 1*IN_MILLISECONDS;
            } else uiCheckTimer -= diff;
        }
    };
};

class achievement_brann_spankin_new : public AchievementCriteriaScript
{
    public:
        achievement_brann_spankin_new() : AchievementCriteriaScript("achievement_brann_spankin_new")
        {
        }

        bool OnCheck(Player* /*player*/, Unit* target)
        {
            if (!target)
                return false;

            if (Creature* Brann = target->ToCreature())
                if (Brann->AI()->GetData(DATA_BRANN_SPARKLIN_NEWS))
                    return true;

            return false;
        }
};

#ifndef __clang_analyzer__
void AddSC_halls_of_stone()
{
    new npc_brann_hos();
    new mob_tribuna_controller();
    new mob_dark_matter();
    new mob_searing_gaze();
    new achievement_brann_spankin_new();
}
#endif
