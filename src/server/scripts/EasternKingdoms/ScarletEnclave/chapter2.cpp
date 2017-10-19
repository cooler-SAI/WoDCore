////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"

//How to win friends and influence enemies
// texts signed for creature 28939 but used for 28939, 28940, 28610
enum win_friends
{
    SAY_PERSUADE1                     = -1609501,
    SAY_PERSUADE2                     = -1609502,
    SAY_PERSUADE3                     = -1609503,
    SAY_PERSUADE4                     = -1609504,
    SAY_PERSUADE5                     = -1609505,
    SAY_PERSUADE6                     = -1609506,
    SAY_PERSUADE7                     = -1609507,
    SAY_CRUSADER1                     = -1609508,
    SAY_CRUSADER2                     = -1609509,
    SAY_CRUSADER3                     = -1609510,
    SAY_CRUSADER4                     = -1609511,
    SAY_CRUSADER5                     = -1609512,
    SAY_CRUSADER6                     = -1609513,
    SAY_PERSUADED1                    = -1609514,
    SAY_PERSUADED2                    = -1609515,
    SAY_PERSUADED3                    = -1609516,
    SAY_PERSUADED4                    = -1609517,
    SAY_PERSUADED5                    = -1609518,
    SAY_PERSUADED6                    = -1609519,
    SPELL_PERSUASIVE_STRIKE           = 52781
};

class npc_crusade_persuaded : public CreatureScript
{
public:
    npc_crusade_persuaded() : CreatureScript("npc_crusade_persuaded") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_crusade_persuadedAI (creature);
    }

    struct npc_crusade_persuadedAI : public ScriptedAI
    {
        npc_crusade_persuadedAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 uiSpeech_timer;
        uint32 uiSpeech_counter;
        uint64 uiPlayerGUID;

        void Reset()
        {
            uiSpeech_timer = 0;
            uiSpeech_counter = 0;
            uiPlayerGUID = 0;
            me->SetReactState(REACT_AGGRESSIVE);
            me->RestoreFaction();
        }

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_PERSUASIVE_STRIKE && caster->IsPlayer() && me->isAlive() && !uiSpeech_counter)
            {
                if (CAST_PLR(caster)->GetQuestStatus(12720) == QUEST_STATUS_INCOMPLETE)
                {
                    uiPlayerGUID = caster->GetGUID();
                    uiSpeech_timer = 1000;
                    uiSpeech_counter = 1;
                    me->setFaction(caster->getFaction());
                    me->CombatStop(true);
                    me->GetMotionMaster()->MoveIdle();
                    me->SetReactState(REACT_PASSIVE);
                    DoCastAOE(58111, true);

                    DoScriptText(RAND(SAY_PERSUADE1, SAY_PERSUADE2, SAY_PERSUADE3,
                                      SAY_PERSUADE4, SAY_PERSUADE5, SAY_PERSUADE6,
                                      SAY_PERSUADE7), caster);

                    DoScriptText(RAND(SAY_CRUSADER1, SAY_CRUSADER2, SAY_CRUSADER3,
                                      SAY_CRUSADER4, SAY_CRUSADER5, SAY_CRUSADER6), me);
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiSpeech_counter)
            {
                if (uiSpeech_timer <= diff)
                {
                    Player* player = Unit::GetPlayer(*me, uiPlayerGUID);
                    if (!player)
                    {
                        EnterEvadeMode();
                        return;
                    }

                    switch (uiSpeech_counter)
                    {
                        case 1:
                            DoScriptText(SAY_PERSUADED1, me);
                            uiSpeech_timer = 8000;
                            break;

                        case 2:
                            DoScriptText(SAY_PERSUADED2, me);
                            uiSpeech_timer = 8000;
                            break;

                        case 3:
                            DoScriptText(SAY_PERSUADED3, me);
                            uiSpeech_timer = 8000;
                            break;

                        case 4:
                            DoScriptText(SAY_PERSUADED4, me);
                            uiSpeech_timer = 8000;
                            break;

                        case 5:
                            DoScriptText(SAY_PERSUADED5, player);
                            uiSpeech_timer = 8000;
                            break;

                        case 6:
                            DoScriptText(SAY_PERSUADED6, me);
                            player->Kill(me);
                            //me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            //me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            uiSpeech_counter = 0;
                            player->GroupEventHappens(12720, me);
                            return;
                    }

                    ++uiSpeech_counter;
                    DoCastAOE(58111, true);
                } else uiSpeech_timer -= diff;

                return;
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

};

/*######
## npc_koltira_deathweaver
######*/

enum Koltira
{
    SAY_BREAKOUT1                   = 1,
    SAY_BREAKOUT2                   = 2,
    SAY_BREAKOUT3                   = 3,
    SAY_BREAKOUT4                   = 4,
    SAY_BREAKOUT5                   = 5,
    SAY_BREAKOUT6                   = 6,
    SAY_BREAKOUT7                   = 7,
    SAY_BREAKOUT8                   = 8,
    SAY_BREAKOUT9                   = 9,
    SAY_BREAKOUT10                  = 10,

    SPELL_KOLTIRA_TRANSFORM         = 52899,
    SPELL_ANTI_MAGIC_ZONE           = 52894,

    QUEST_BREAKOUT                  = 12727,
    NPC_KOLTIRA_SUMMON              = 45112,

    NPC_CRIMSON_ACOLYTE             = 29007,
    NPC_HIGH_INQUISITOR_VALROTH     = 29001,


    //not sure about this id
    //NPC_DEATH_KNIGHT_MOUNT        = 29201,
    MODEL_DEATH_KNIGHT_MOUNT        = 25278
};

/// Passive Scene Object
class playerScript_koltira_deathweaver_escort : public PlayerScript
{
    public:
        playerScript_koltira_deathweaver_escort() : PlayerScript("playerScript_koltira_deathweaver_escort") { }

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Quest->GetQuestId() == QUEST_BREAKOUT)
            {
                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask &= ~0x08;
                l_PhaseMask |= 0x04;
                p_Player->SetPhaseMask(l_PhaseMask, true);
            }
        }

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Quest->GetQuestId() == QUEST_BREAKOUT)
            {
                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask &= ~0x08;
                l_PhaseMask |= 0x04;
                p_Player->SetPhaseMask(l_PhaseMask, true);
            }
        }
};

class npc_koltira_deathweaver_escort : public CreatureScript
{
    public:
        npc_koltira_deathweaver_escort() : CreatureScript("npc_koltira_deathweaver_escort") { }

        struct npc_koltira_deathweaver_escortAI : public npc_escortAI
        {
            npc_koltira_deathweaver_escortAI(Creature* creature) : npc_escortAI(creature)
            {
                me->SetReactState(REACT_DEFENSIVE);
            }

            void Reset()
            {
                if (!HasEscortState(STATE_ESCORT_ESCORTING))
                {
                    wave = 0;
                    waveTimer = 3000;
                    valrothGUID = 0;
                    SetDespawnAtFar(false);
                    SetDespawnAtEnd(false);
                    me->LoadEquipment(0, true);
                    me->RemoveAurasDueToSpell(SPELL_ANTI_MAGIC_ZONE);
                    me->RemoveAurasDueToSpell(SPELL_KOLTIRA_TRANSFORM);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                Start(false, false, p_Summoner->GetGUID());
                SetEscortPaused(false);
            }

            void WaypointReached(uint32 waypointId)
            {
                switch (waypointId)
                {
                    case 0:
                        Talk(SAY_BREAKOUT1);
                        break;
                    case 1:
                        me->SetStandState(UNIT_STAND_STATE_KNEEL);
                        break;
                    case 2:
                        me->SetStandState(UNIT_STAND_STATE_STAND);
                        DoCast(me, SPELL_KOLTIRA_TRANSFORM);
                        me->LoadEquipment();
                        break;
                    case 3:
                        SetEscortPaused(true);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->SetStandState(UNIT_STAND_STATE_KNEEL);
                        Talk(SAY_BREAKOUT2);
                        DoCast(me, SPELL_ANTI_MAGIC_ZONE);
                        break;
                    case 4:
                        SetRun(true);
                        break;
                    case 9:
                        me->Mount(MODEL_DEATH_KNIGHT_MOUNT);
                        break;
                    case 10:
                        me->Dismount();
                        me->DespawnOrUnsummon();
                        break;
                }
            }

            void JustSummoned(Creature* summoned)
            {
                if (Player* player = GetPlayerForEscort())
                    summoned->AI()->AttackStart(player);

                if (summoned->GetEntry() == NPC_HIGH_INQUISITOR_VALROTH)
                    valrothGUID = summoned->GetGUID();

                summoned->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            }

            void SummonAcolyte(uint32 uiAmount)
            {
                for (uint32 i = 0; i < uiAmount; ++i)
                    me->SummonCreature(NPC_CRIMSON_ACOLYTE, 1642.329f, -6045.818f, 127.583f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                npc_escortAI::UpdateAI(uiDiff);

                if (HasEscortState(STATE_ESCORT_PAUSED))
                {
                    if (waveTimer <= uiDiff)
                    {
                        switch (wave)
                        {
                            case 0:
                                Talk(SAY_BREAKOUT3);
                                SummonAcolyte(3);
                                waveTimer = 20000;
                                break;
                            case 1:
                                Talk(SAY_BREAKOUT4);
                                SummonAcolyte(3);
                                waveTimer = 20000;
                                break;
                            case 2:
                                Talk(SAY_BREAKOUT5);
                                SummonAcolyte(4);
                                waveTimer = 20000;
                                break;
                            case 3:
                                Talk(SAY_BREAKOUT6);
                                me->SummonCreature(NPC_HIGH_INQUISITOR_VALROTH, 1642.329f, -6045.818f, 127.583f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000);
                                waveTimer = 1000;
                                break;
                            case 4:
                            {
                                Creature* temp = Unit::GetCreature(*me, valrothGUID);

                                if (!temp || !temp->isAlive())
                                {
                                    Talk(SAY_BREAKOUT8);
                                    waveTimer = 5000;
                                }
                                else
                                {
                                    waveTimer = 2500;
                                    return;
                                }
                                break;
                            }
                            case 5:
                                Talk(SAY_BREAKOUT9);
                                me->RemoveAurasDueToSpell(SPELL_ANTI_MAGIC_ZONE);
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                waveTimer = 2500;
                                break;
                            case 6:
                                Talk(SAY_BREAKOUT10);
                                SetEscortPaused(false);
                                break;
                        }

                        ++wave;
                    }
                    else
                        waveTimer -= uiDiff;
                }
            }
        
        private:
            uint8 wave;
            uint32 waveTimer;
            uint64 valrothGUID;

        };
    
        CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_koltira_deathweaver_escortAI(creature);
    }
};

class npc_koltira_deathweaver : public CreatureScript
{
    public:
        npc_koltira_deathweaver() : CreatureScript("npc_koltira_deathweaver") { }

        bool OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest)
        {
            if (p_Quest->GetQuestId() == QUEST_BREAKOUT)
            {
                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask &= ~p_Creature->GetPhaseMask();
                l_PhaseMask |= 0x08;
                p_Player->SetPhaseMask(l_PhaseMask, true);

                if (Creature* l_Creature = p_Creature->SummonCreature(NPC_KOLTIRA_SUMMON, *p_Creature, TEMPSUMMON_MANUAL_DESPAWN))
                    l_Creature->SetPhaseMask(0x08, true);
            }
            return true;
        }
};

//Scarlet courier
enum ScarletCourierEnum
{
    SAY_TREE1                          = -1609531,
    SAY_TREE2                          = -1609532,
    SPELL_SHOOT                        = 52818,
    GO_INCONSPICUOUS_TREE              = 191144,
    NPC_SCARLET_COURIER                = 29076
};

class mob_scarlet_courier : public CreatureScript
{
public:
    mob_scarlet_courier() : CreatureScript("mob_scarlet_courier") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_scarlet_courierAI (creature);
    }

    struct mob_scarlet_courierAI : public ScriptedAI
    {
        mob_scarlet_courierAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 uiStage;
        uint32 uiStage_timer;

        void Reset()
        {
            me->Mount(14338); // not sure about this id
            uiStage = 1;
            uiStage_timer = 3000;
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(SAY_TREE2, me);
            me->Dismount();
            uiStage = 0;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (id == 1)
                uiStage = 2;
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiStage && !me->isInCombat())
            {
                if (uiStage_timer <= diff)
                {
                    switch (uiStage)
                    {
                    case 1:
                        me->SetWalk(true);
                        if (GameObject* tree = me->FindNearestGameObject(GO_INCONSPICUOUS_TREE, 40.0f))
                        {
                            DoScriptText(SAY_TREE1, me);
                            float x, y, z;
                            tree->GetContactPoint(me, x, y, z);
                            me->GetMotionMaster()->MovePoint(1, x, y, z);
                        }
                        break;
                    case 2:
                        if (GameObject* tree = me->FindNearestGameObject(GO_INCONSPICUOUS_TREE, 40.0f))
                            if (Unit* unit = tree->GetOwner())
                                AttackStart(unit);
                        break;
                    }
                    uiStage_timer = 3000;
                    uiStage = 0;
                } else uiStage_timer -= diff;
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

};

//Koltira & Valroth- Breakout

enum valroth
{
    SAY_VALROTH1                      = -1609581,
    SAY_VALROTH2                      = -1609582,
    SAY_VALROTH3                      = -1609583,
    SAY_VALROTH4                      = -1609584,
    SAY_VALROTH5                      = -1609585,
    SAY_VALROTH6                      = -1609586,
    SPELL_RENEW                       = 38210,
    SPELL_INQUISITOR_PENANCE          = 52922,
    SPELL_VALROTH_SMITE               = 52926,
    SPELL_SUMMON_VALROTH_REMAINS      = 52929
};

class mob_high_inquisitor_valroth : public CreatureScript
{
public:
    mob_high_inquisitor_valroth() : CreatureScript("mob_high_inquisitor_valroth") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_high_inquisitor_valrothAI (creature);
    }

    struct mob_high_inquisitor_valrothAI : public ScriptedAI
    {
        mob_high_inquisitor_valrothAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 uiRenew_timer;
        uint32 uiInquisitor_Penance_timer;
        uint32 uiValroth_Smite_timer;

        void Reset()
        {
            uiRenew_timer = 1000;
            uiInquisitor_Penance_timer = 2000;
            uiValroth_Smite_timer = 1000;
        }

        void EnterCombat(Unit* who)
        {
            DoScriptText(SAY_VALROTH2, me);
            DoCast(who, SPELL_VALROTH_SMITE);
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiRenew_timer <= diff)
            {
                Shout();
                DoCast(me, SPELL_RENEW);
                uiRenew_timer = urand(1000, 6000);
            } else uiRenew_timer -= diff;

            if (uiInquisitor_Penance_timer <= diff)
            {
                Shout();
                DoCast(me->getVictim(), SPELL_INQUISITOR_PENANCE);
                uiInquisitor_Penance_timer = urand(2000, 7000);
            } else uiInquisitor_Penance_timer -= diff;

            if (uiValroth_Smite_timer <= diff)
            {
                Shout();
                DoCast(me->getVictim(), SPELL_VALROTH_SMITE);
                uiValroth_Smite_timer = urand(1000, 6000);
            } else uiValroth_Smite_timer -= diff;

            DoMeleeAttackIfReady();
        }

        void Shout()
        {
            if (rand()%100 < 15)
                DoScriptText(RAND(SAY_VALROTH3, SAY_VALROTH4, SAY_VALROTH5), me);
        }

        void JustDied(Unit* killer)
        {
            DoScriptText(SAY_VALROTH6, me);
            killer->CastSpell(me, SPELL_SUMMON_VALROTH_REMAINS, true);
        }
    };

};

/*######
## npc_a_special_surprise
######*/
//used by 29032, 29061, 29065, 29067, 29068, 29070, 29074, 29072, 29073, 29071 but signed for 29032
enum SpecialSurprise
{
    SAY_EXEC_START_1            = -1609025,                 // speech for all
    SAY_EXEC_START_2            = -1609026,
    SAY_EXEC_START_3            = -1609027,
    SAY_EXEC_PROG_1             = -1609028,
    SAY_EXEC_PROG_2             = -1609029,
    SAY_EXEC_PROG_3             = -1609030,
    SAY_EXEC_PROG_4             = -1609031,
    SAY_EXEC_PROG_5             = -1609032,
    SAY_EXEC_PROG_6             = -1609033,
    SAY_EXEC_PROG_7             = -1609034,
    SAY_EXEC_NAME_1             = -1609035,
    SAY_EXEC_NAME_2             = -1609036,
    SAY_EXEC_RECOG_1            = -1609037,
    SAY_EXEC_RECOG_2            = -1609038,
    SAY_EXEC_RECOG_3            = -1609039,
    SAY_EXEC_RECOG_4            = -1609040,
    SAY_EXEC_RECOG_5            = -1609041,
    SAY_EXEC_RECOG_6            = -1609042,
    SAY_EXEC_NOREM_1            = -1609043,
    SAY_EXEC_NOREM_2            = -1609044,
    SAY_EXEC_NOREM_3            = -1609045,
    SAY_EXEC_NOREM_4            = -1609046,
    SAY_EXEC_NOREM_5            = -1609047,
    SAY_EXEC_NOREM_6            = -1609048,
    SAY_EXEC_NOREM_7            = -1609049,
    SAY_EXEC_NOREM_8            = -1609050,
    SAY_EXEC_NOREM_9            = -1609051,
    SAY_EXEC_THINK_1            = -1609052,
    SAY_EXEC_THINK_2            = -1609053,
    SAY_EXEC_THINK_3            = -1609054,
    SAY_EXEC_THINK_4            = -1609055,
    SAY_EXEC_THINK_5            = -1609056,
    SAY_EXEC_THINK_6            = -1609057,
    SAY_EXEC_THINK_7            = -1609058,
    SAY_EXEC_THINK_8            = -1609059,
    SAY_EXEC_THINK_9            = -1609060,
    SAY_EXEC_THINK_10           = -1609061,
    SAY_EXEC_LISTEN_1           = -1609062,
    SAY_EXEC_LISTEN_2           = -1609063,
    SAY_EXEC_LISTEN_3           = -1609064,
    SAY_EXEC_LISTEN_4           = -1609065,
    SAY_PLAGUEFIST              = -1609066,
    SAY_EXEC_TIME_1             = -1609067,
    SAY_EXEC_TIME_2             = -1609068,
    SAY_EXEC_TIME_3             = -1609069,
    SAY_EXEC_TIME_4             = -1609070,
    SAY_EXEC_TIME_5             = -1609071,
    SAY_EXEC_TIME_6             = -1609072,
    SAY_EXEC_TIME_7             = -1609073,
    SAY_EXEC_TIME_8             = -1609074,
    SAY_EXEC_TIME_9             = -1609075,
    SAY_EXEC_TIME_10            = -1609076,
    SAY_EXEC_WAITING            = -1609077,
    EMOTE_DIES                  = -1609078,

    NPC_PLAGUEFIST              = 29053
};

class npc_a_special_surprise : public CreatureScript
{
public:
    npc_a_special_surprise() : CreatureScript("npc_a_special_surprise") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_a_special_surpriseAI(creature);
    }

    struct npc_a_special_surpriseAI : public ScriptedAI
    {
        npc_a_special_surpriseAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 ExecuteSpeech_Timer;
        uint32 ExecuteSpeech_Counter;
        uint64 PlayerGUID;

        void Reset()
        {
            ExecuteSpeech_Timer = 0;
            ExecuteSpeech_Counter = 0;
            PlayerGUID = 0;

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
        }

        bool MeetQuestCondition(Player* player)
        {
            switch (me->GetEntry())
            {
                case 29061:                                     // Ellen Stanbridge
                    if (player->GetQuestStatus(12742) == QUEST_STATUS_INCOMPLETE)
                        return true;
                    break;
                case 29072:                                     // Kug Ironjaw
                    if (player->GetQuestStatus(12748) == QUEST_STATUS_INCOMPLETE)
                        return true;
                    break;
                case 29067:                                     // Donovan Pulfrost
                    if (player->GetQuestStatus(12744) == QUEST_STATUS_INCOMPLETE)
                        return true;
                    break;
                case 29065:                                     // Yazmina Oakenthorn
                    if (player->GetQuestStatus(12743) == QUEST_STATUS_INCOMPLETE)
                        return true;
                    break;
                case 29071:                                     // Antoine Brack
                    if (player->GetQuestStatus(12750) == QUEST_STATUS_INCOMPLETE)
                        return true;
                    break;
                case 29032:                                     // Malar Bravehorn
                    if (player->GetQuestStatus(12739) == QUEST_STATUS_INCOMPLETE)
                        return true;
                    break;
                case 29068:                                     // Goby Blastenheimer
                    if (player->GetQuestStatus(12745) == QUEST_STATUS_INCOMPLETE)
                        return true;
                    break;
                case 29073:                                     // Iggy Darktusk
                    if (player->GetQuestStatus(12749) == QUEST_STATUS_INCOMPLETE)
                        return true;
                    break;
                case 29074:                                     // Lady Eonys
                    if (player->GetQuestStatus(12747) == QUEST_STATUS_INCOMPLETE)
                        return true;
                    break;
                case 29070:                                     // Valok the Righteous
                    if (player->GetQuestStatus(12746) == QUEST_STATUS_INCOMPLETE)
                        return true;
                    break;
                case 49355:                                     // Seigneur Haford
                    if (player->GetQuestStatus(28649) == QUEST_STATUS_INCOMPLETE)
                        return true;
                    break;
                case 49356:                                     // Gally Grossetache
                    if (player->GetQuestStatus(28650) == QUEST_STATUS_INCOMPLETE)
                        return true;
                    break;
                    
            }

            return false;
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (PlayerGUID || who->GetTypeId() != TYPEID_PLAYER || !who->IsWithinDist(me, INTERACTION_DISTANCE))
                return;

            if (MeetQuestCondition(who->ToPlayer()))
                PlayerGUID = who->GetGUID();
        }

        void UpdateAI(const uint32 diff)
        {
            if (PlayerGUID && !me->getVictim() && me->isAlive())
            {
                if (ExecuteSpeech_Timer <= diff)
                {
                    Player* player = Unit::GetPlayer(*me, PlayerGUID);

                    if (!player)
                    {
                        Reset();
                        return;
                    }

                    //TODO: simplify text's selection

                    switch (player->getRace())
                    {
                        // TODO : exactly text for goblin & worgen
                        case RACE_GOBLIN:
                            switch (ExecuteSpeech_Counter)
                            {
                                case 0: DoScriptText(SAY_EXEC_START_1, me, player); break;
                                case 1: me->SetStandState(UNIT_STAND_STATE_STAND); break;
                                case 2: DoScriptText(SAY_EXEC_PROG_5, me, player); break;
                                case 3: DoScriptText(SAY_EXEC_NAME_1, me, player); break;
                                case 4: DoScriptText(SAY_EXEC_RECOG_1, me, player); break;
                                case 5: DoScriptText(SAY_EXEC_NOREM_5, me, player); break;
                                case 6: DoScriptText(SAY_EXEC_THINK_7, me, player); break;
                                case 7: DoScriptText(SAY_EXEC_LISTEN_1, me, player); break;
                                case 8:
                                    if (Creature* Plaguefist = GetClosestCreatureWithEntry(me, NPC_PLAGUEFIST, 85.0f))
                                        DoScriptText(SAY_PLAGUEFIST, Plaguefist, player);
                                    break;
                                case 9:
                                    DoScriptText(SAY_EXEC_TIME_6, me, player);
                                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                    break;
                                case 10:
                                    DoScriptText(SAY_EXEC_WAITING, me, player);
                                    break;
                                case 11:
                                    DoScriptText(EMOTE_DIES, me);
                                    me->setDeathState(JUST_DIED);
                                    me->SetHealth(0);
                                    return;
                            }
                            break;
                        case RACE_WORGEN:
                            switch (ExecuteSpeech_Counter)
                            {
                                case 0: DoScriptText(SAY_EXEC_START_1, me, player); break;
                                case 1: me->SetStandState(UNIT_STAND_STATE_STAND); break;
                                case 2: DoScriptText(SAY_EXEC_PROG_5, me, player); break;
                                case 3: DoScriptText(SAY_EXEC_NAME_1, me, player); break;
                                case 4: DoScriptText(SAY_EXEC_RECOG_1, me, player); break;
                                case 5: DoScriptText(SAY_EXEC_NOREM_5, me, player); break;
                                case 6: DoScriptText(SAY_EXEC_THINK_7, me, player); break;
                                case 7: DoScriptText(SAY_EXEC_LISTEN_1, me, player); break;
                                case 8:
                                    if (Creature* Plaguefist = GetClosestCreatureWithEntry(me, NPC_PLAGUEFIST, 85.0f))
                                        DoScriptText(SAY_PLAGUEFIST, Plaguefist, player);
                                    break;
                                case 9:
                                    DoScriptText(SAY_EXEC_TIME_6, me, player);
                                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                    break;
                                case 10:
                                    DoScriptText(SAY_EXEC_WAITING, me, player);
                                    break;
                                case 11:
                                    DoScriptText(EMOTE_DIES, me);
                                    me->setDeathState(JUST_DIED);
                                    me->SetHealth(0);
                                    return;
                            }
                            break;
                        case RACE_HUMAN:
                            switch (ExecuteSpeech_Counter)
                            {
                                case 0: DoScriptText(SAY_EXEC_START_1, me, player); break;
                                case 1: me->SetStandState(UNIT_STAND_STATE_STAND); break;
                                case 2: DoScriptText(SAY_EXEC_PROG_5, me, player); break;
                                case 3: DoScriptText(SAY_EXEC_NAME_1, me, player); break;
                                case 4: DoScriptText(SAY_EXEC_RECOG_1, me, player); break;
                                case 5: DoScriptText(SAY_EXEC_NOREM_5, me, player); break;
                                case 6: DoScriptText(SAY_EXEC_THINK_7, me, player); break;
                                case 7: DoScriptText(SAY_EXEC_LISTEN_1, me, player); break;
                                case 8:
                                    if (Creature* Plaguefist = GetClosestCreatureWithEntry(me, NPC_PLAGUEFIST, 85.0f))
                                        DoScriptText(SAY_PLAGUEFIST, Plaguefist, player);
                                    break;
                                case 9:
                                    DoScriptText(SAY_EXEC_TIME_6, me, player);
                                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                    break;
                                case 10:
                                    DoScriptText(SAY_EXEC_WAITING, me, player);
                                    break;
                                case 11:
                                    DoScriptText(EMOTE_DIES, me);
                                    me->setDeathState(JUST_DIED);
                                    me->SetHealth(0);
                                    return;
                            }
                            break;
                        case RACE_ORC:
                            switch (ExecuteSpeech_Counter)
                            {
                                case 0: DoScriptText(SAY_EXEC_START_1, me, player); break;
                                case 1: me->SetStandState(UNIT_STAND_STATE_STAND); break;
                                case 2: DoScriptText(SAY_EXEC_PROG_6, me, player); break;
                                case 3: DoScriptText(SAY_EXEC_NAME_1, me, player); break;
                                case 4: DoScriptText(SAY_EXEC_RECOG_1, me, player); break;
                                case 5: DoScriptText(SAY_EXEC_NOREM_7, me, player); break;
                                case 6: DoScriptText(SAY_EXEC_THINK_8, me, player); break;
                                case 7: DoScriptText(SAY_EXEC_LISTEN_1, me, player); break;
                                case 8:
                                    if (Creature* Plaguefist = GetClosestCreatureWithEntry(me, NPC_PLAGUEFIST, 85.0f))
                                        DoScriptText(SAY_PLAGUEFIST, Plaguefist, player);
                                    break;
                                case 9:
                                    DoScriptText(SAY_EXEC_TIME_8, me, player);
                                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                    break;
                                case 10:
                                    DoScriptText(SAY_EXEC_WAITING, me, player);
                                    break;
                                case 11:
                                    DoScriptText(EMOTE_DIES, me);
                                    me->setDeathState(JUST_DIED);
                                    me->SetHealth(0);
                                    return;
                            }
                            break;
                        case RACE_DWARF:
                            switch (ExecuteSpeech_Counter)
                            {
                                case 0: DoScriptText(SAY_EXEC_START_2, me, player); break;
                                case 1: me->SetStandState(UNIT_STAND_STATE_STAND); break;
                                case 2: DoScriptText(SAY_EXEC_PROG_2, me, player); break;
                                case 3: DoScriptText(SAY_EXEC_NAME_1, me, player); break;
                                case 4: DoScriptText(SAY_EXEC_RECOG_3, me, player); break;
                                case 5: DoScriptText(SAY_EXEC_NOREM_2, me, player); break;
                                case 6: DoScriptText(SAY_EXEC_THINK_5, me, player); break;
                                case 7: DoScriptText(SAY_EXEC_LISTEN_2, me, player); break;
                                case 8:
                                    if (Creature* Plaguefist = GetClosestCreatureWithEntry(me, NPC_PLAGUEFIST, 85.0f))
                                        DoScriptText(SAY_PLAGUEFIST, Plaguefist, player);
                                    break;
                                case 9:
                                    DoScriptText(SAY_EXEC_TIME_3, me, player);
                                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                    break;
                                case 10:
                                    DoScriptText(SAY_EXEC_WAITING, me, player);
                                    break;
                                case 11:
                                    DoScriptText(EMOTE_DIES, me);
                                    me->setDeathState(JUST_DIED);
                                    me->SetHealth(0);
                                    return;
                            }
                            break;
                        case RACE_NIGHTELF:
                            switch (ExecuteSpeech_Counter)
                            {
                                case 0: DoScriptText(SAY_EXEC_START_1, me, player); break;
                                case 1: me->SetStandState(UNIT_STAND_STATE_STAND); break;
                                case 2: DoScriptText(SAY_EXEC_PROG_5, me, player); break;
                                case 3: DoScriptText(SAY_EXEC_NAME_1, me, player); break;
                                case 4: DoScriptText(SAY_EXEC_RECOG_1, me, player); break;
                                case 5: DoScriptText(SAY_EXEC_NOREM_6, me, player); break;
                                case 6: DoScriptText(SAY_EXEC_THINK_2, me, player); break;
                                case 7: DoScriptText(SAY_EXEC_LISTEN_1, me, player); break;
                                case 8:
                                    if (Creature* Plaguefist = GetClosestCreatureWithEntry(me, NPC_PLAGUEFIST, 85.0f))
                                        DoScriptText(SAY_PLAGUEFIST, Plaguefist, player);
                                    break;
                                case 9:
                                    DoScriptText(SAY_EXEC_TIME_7, me, player);
                                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                    break;
                                case 10:
                                    DoScriptText(SAY_EXEC_WAITING, me, player);
                                    break;
                                case 11:
                                    DoScriptText(EMOTE_DIES, me);
                                    me->setDeathState(JUST_DIED);
                                    me->SetHealth(0);
                                    return;
                            }
                            break;
                        case RACE_UNDEAD_PLAYER:
                            switch (ExecuteSpeech_Counter)
                            {
                                case 0: DoScriptText(SAY_EXEC_START_1, me, player); break;
                                case 1: me->SetStandState(UNIT_STAND_STATE_STAND); break;
                                case 2: DoScriptText(SAY_EXEC_PROG_3, me, player); break;
                                case 3: DoScriptText(SAY_EXEC_NAME_1, me, player); break;
                                case 4: DoScriptText(SAY_EXEC_RECOG_4, me, player); break;
                                case 5: DoScriptText(SAY_EXEC_NOREM_3, me, player); break;
                                case 6: DoScriptText(SAY_EXEC_THINK_1, me, player); break;
                                case 7: DoScriptText(SAY_EXEC_LISTEN_3, me, player); break;
                                case 8:
                                    if (Creature* Plaguefist = GetClosestCreatureWithEntry(me, NPC_PLAGUEFIST, 85.0f))
                                        DoScriptText(SAY_PLAGUEFIST, Plaguefist, player);
                                    break;
                                case 9:
                                    DoScriptText(SAY_EXEC_TIME_4, me, player);
                                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                    break;
                                case 10:
                                    DoScriptText(SAY_EXEC_WAITING, me, player);
                                    break;
                                case 11:
                                    DoScriptText(EMOTE_DIES, me);
                                    me->setDeathState(JUST_DIED);
                                    me->SetHealth(0);
                                    return;
                            }
                            break;
                        case RACE_TAUREN:
                            switch (ExecuteSpeech_Counter)
                            {
                                case 0: DoScriptText(SAY_EXEC_START_1, me, player); break;
                                case 1: me->SetStandState(UNIT_STAND_STATE_STAND); break;
                                case 2: DoScriptText(SAY_EXEC_PROG_1, me, player); break;
                                case 3: DoScriptText(SAY_EXEC_NAME_1, me, player); break;
                                case 4: DoScriptText(SAY_EXEC_RECOG_5, me, player); break;
                                case 5: DoScriptText(SAY_EXEC_NOREM_8, me, player); break;
                                case 6: DoScriptText(SAY_EXEC_THINK_9, me, player); break;
                                case 7: DoScriptText(SAY_EXEC_LISTEN_1, me, player); break;
                                case 8:
                                    if (Creature* Plaguefist = GetClosestCreatureWithEntry(me, NPC_PLAGUEFIST, 85.0f))
                                        DoScriptText(SAY_PLAGUEFIST, Plaguefist, player);
                                    break;
                                case 9:
                                    DoScriptText(SAY_EXEC_TIME_9, me, player);
                                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                    break;
                                case 10:
                                    DoScriptText(SAY_EXEC_WAITING, me, player);
                                    break;
                                case 11:
                                    DoScriptText(EMOTE_DIES, me);
                                    me->setDeathState(JUST_DIED);
                                    me->SetHealth(0);
                                    return;
                            }
                            break;
                        case RACE_GNOME:
                            switch (ExecuteSpeech_Counter)
                            {
                                case 0: DoScriptText(SAY_EXEC_START_1, me, player); break;
                                case 1: me->SetStandState(UNIT_STAND_STATE_STAND); break;
                                case 2: DoScriptText(SAY_EXEC_PROG_4, me, player); break;
                                case 3: DoScriptText(SAY_EXEC_NAME_1, me, player); break;
                                case 4: DoScriptText(SAY_EXEC_RECOG_1, me, player); break;
                                case 5: DoScriptText(SAY_EXEC_NOREM_4, me, player); break;
                                case 6: DoScriptText(SAY_EXEC_THINK_6, me, player); break;
                                case 7: DoScriptText(SAY_EXEC_LISTEN_1, me, player); break;
                                case 8:
                                    if (Creature* Plaguefist = GetClosestCreatureWithEntry(me, NPC_PLAGUEFIST, 85.0f))
                                        DoScriptText(SAY_PLAGUEFIST, Plaguefist, player);
                                    break;
                                case 9:
                                    DoScriptText(SAY_EXEC_TIME_5, me, player);
                                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                    break;
                                case 10:
                                    DoScriptText(SAY_EXEC_WAITING, me, player);
                                    break;
                                case 11:
                                    DoScriptText(EMOTE_DIES, me);
                                    me->setDeathState(JUST_DIED);
                                    me->SetHealth(0);
                                    return;
                            }
                            break;
                        case RACE_TROLL:
                            switch (ExecuteSpeech_Counter)
                            {
                                case 0: DoScriptText(SAY_EXEC_START_3, me, player); break;
                                case 1: me->SetStandState(UNIT_STAND_STATE_STAND); break;
                                case 2: DoScriptText(SAY_EXEC_PROG_7, me, player); break;
                                case 3: DoScriptText(SAY_EXEC_NAME_2, me, player); break;
                                case 4: DoScriptText(SAY_EXEC_RECOG_6, me, player); break;
                                case 5: DoScriptText(SAY_EXEC_NOREM_9, me, player); break;
                                case 6: DoScriptText(SAY_EXEC_THINK_10, me, player); break;
                                case 7: DoScriptText(SAY_EXEC_LISTEN_4, me, player); break;
                                case 8:
                                    if (Creature* Plaguefist = GetClosestCreatureWithEntry(me, NPC_PLAGUEFIST, 85.0f))
                                        DoScriptText(SAY_PLAGUEFIST, Plaguefist, player);
                                    break;
                                case 9:
                                    DoScriptText(SAY_EXEC_TIME_10, me, player);
                                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                    break;
                                case 10:
                                    DoScriptText(SAY_EXEC_WAITING, me, player);
                                    break;
                                case 11:
                                    DoScriptText(EMOTE_DIES, me);
                                    me->setDeathState(JUST_DIED);
                                    me->SetHealth(0);
                                    return;
                            }
                            break;
                        case RACE_BLOODELF:
                            switch (ExecuteSpeech_Counter)
                            {
                                case 0: DoScriptText(SAY_EXEC_START_1, me, player); break;
                                case 1: me->SetStandState(UNIT_STAND_STATE_STAND); break;
                                case 2: DoScriptText(SAY_EXEC_PROG_1, me, player); break;
                                case 3: DoScriptText(SAY_EXEC_NAME_1, me, player); break;
                                case 4: DoScriptText(SAY_EXEC_RECOG_1, me, player); break;
                                //case 5: //unknown
                                case 6: DoScriptText(SAY_EXEC_THINK_3, me, player); break;
                                case 7: DoScriptText(SAY_EXEC_LISTEN_1, me, player); break;
                                case 8:
                                    if (Creature* Plaguefist = GetClosestCreatureWithEntry(me, NPC_PLAGUEFIST, 85.0f))
                                        DoScriptText(SAY_PLAGUEFIST, Plaguefist, player);
                                    break;
                                case 9:
                                    DoScriptText(SAY_EXEC_TIME_1, me, player);
                                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                    break;
                                case 10:
                                    DoScriptText(SAY_EXEC_WAITING, me, player);
                                    break;
                                case 11:
                                    DoScriptText(EMOTE_DIES, me);
                                    me->setDeathState(JUST_DIED);
                                    me->SetHealth(0);
                                    return;
                            }
                            break;
                        case RACE_DRAENEI:
                            switch (ExecuteSpeech_Counter)
                            {
                                case 0: DoScriptText(SAY_EXEC_START_1, me, player); break;
                                case 1: me->SetStandState(UNIT_STAND_STATE_STAND); break;
                                case 2: DoScriptText(SAY_EXEC_PROG_1, me, player); break;
                                case 3: DoScriptText(SAY_EXEC_NAME_1, me, player); break;
                                case 4: DoScriptText(SAY_EXEC_RECOG_2, me, player); break;
                                case 5: DoScriptText(SAY_EXEC_NOREM_1, me, player); break;
                                case 6: DoScriptText(SAY_EXEC_THINK_4, me, player); break;
                                case 7: DoScriptText(SAY_EXEC_LISTEN_1, me, player); break;
                                case 8:
                                    if (Creature* Plaguefist = GetClosestCreatureWithEntry(me, NPC_PLAGUEFIST, 85.0f))
                                        DoScriptText(SAY_PLAGUEFIST, Plaguefist, player);
                                    break;
                                case 9:
                                    DoScriptText(SAY_EXEC_TIME_2, me, player);
                                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                    break;
                                case 10:
                                    DoScriptText(SAY_EXEC_WAITING, me, player);
                                    break;
                                case 11:
                                    DoScriptText(EMOTE_DIES, me);
                                    me->setDeathState(JUST_DIED);
                                    me->SetHealth(0);
                                    return;
                            }
                            break;
                    }

                    if (ExecuteSpeech_Counter >= 9)
                        ExecuteSpeech_Timer = 15000;
                    else
                        ExecuteSpeech_Timer = 7000;

                    ++ExecuteSpeech_Counter;
                }
                else
                    ExecuteSpeech_Timer -= diff;
            }
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_the_scarlet_enclave_c2()
{
    new npc_crusade_persuaded();
    new mob_scarlet_courier();
    new npc_koltira_deathweaver();
    new npc_koltira_deathweaver_escort();
    new mob_high_inquisitor_valroth();
    new npc_a_special_surprise();

    /// Playerscript
    new playerScript_koltira_deathweaver_escort();
}
#endif
