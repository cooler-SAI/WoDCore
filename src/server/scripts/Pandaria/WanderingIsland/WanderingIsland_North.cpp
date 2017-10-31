////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"

#define ACTION_TALK 1

const uint32 g_PandarenQuests[91]
{
    29404,
    29405,
    29406,
    29407,
    29408,
    29409,
    29410,
    29414,
    29417,
    29418,
    29419,
    29420,
    29421,
    29422,
    29423,
    29424,
    29521,
    29522,
    29523,
    29524,
    29661,
    29662,
    29663,
    29664,
    29665,
    29666,
    29676,
    29677,
    29678,
    29679,
    29680,
    29703,
    29705,
    29706,
    29768,
    29769,
    29770,
    29771,
    29772,
    29773,
    29774,
    29775,
    29776,
    29777,
    29778,
    29779,
    29780,
    29781,
    29782,
    29783,
    29784,
    29785,
    29786,
    29787,
    29788,
    29789,
    29790,
    29791,
    29792,
    29793,
    29794,
    29795,
    29796,
    29797,
    29798,
    29799,
    29800,
    30027,
    30033,
    30034,
    30035,
    30036,
    30037,
    30038,
    30039,
    30040,
    30041,
    30042,
    30043,
    30044,
    30045,
    30454,
    30455,
    30589,
    30590,
    30591,
    30767,
    30817,
    30818,
    31450,
    32396
};

class mob_master_shang_xi : public CreatureScript
{
    public:
        mob_master_shang_xi() : CreatureScript("mob_master_shang_xi") { }

        bool OnQuestAccept(Player* /*player*/, Creature* creature, Quest const* quest)
        {
            if (quest->GetQuestId() == 29408) // La lecon du parchemin brulant
            {
                creature->AddAura(114610, creature);
                creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
                creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            }

            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_master_shang_xi_AI(creature);
        }

        struct mob_master_shang_xi_AI : public ScriptedAI
        {
            mob_master_shang_xi_AI(Creature* creature) : ScriptedAI(creature)
            {
                checkPlayersTime = 2000;
                creature->SetStandState(UNIT_STAND_STATE_SIT_MEDIUM_CHAIR);
            }

            uint32 checkPlayersTime;

            void SpellHit(Unit* caster, const SpellInfo* pSpell)
            {
                if (pSpell->Id == 114746) // Attraper la flamme
                {
                    if (caster->IsPlayer())
                    {
                        if (caster->ToPlayer()->GetQuestStatus(29408) == QUEST_STATUS_INCOMPLETE)
                        {
                            me->CastSpell(caster, 114611, true);
                            me->RemoveAurasDueToSpell(114610);
                            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
                            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                        }
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                Position const l_Pos = { 1462.0f, 3465.590f, 181.597f };

                if (me->IsNearPosition(&l_Pos, 0.5f))
                    me->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_STAND_STATE_SIT_MEDIUM_CHAIR);

                if (checkPlayersTime <= diff)
                {
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, me, 5.0f);

                    bool playerWithQuestNear = false;

                    for (auto player: playerList)
                        if (player->GetQuestStatus(29408) == QUEST_STATUS_INCOMPLETE)
                            if (!player->HasItemCount(80212)) // Master's flame
                                playerWithQuestNear = true;

                    if (playerWithQuestNear && !me->HasAura(114610))
                    {
                        me->AddAura(114610, me);
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
                        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    }
                    else if (!playerWithQuestNear && me->HasAura(114610))
                    {
                        me->RemoveAurasDueToSpell(114610);
                        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                    }

                    checkPlayersTime = 2000;
                }
                else
                    checkPlayersTime -= diff;
            }
        };
};

class go_wandering_weapon_rack : public GameObjectScript
{
public:
    go_wandering_weapon_rack() : GameObjectScript("go_wandering_weapon_rack") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
        if (player->GetQuestStatus(30027) == QUEST_STATUS_INCOMPLETE && !player->HasItemCount(73209))
        {
            player->AddItem(73209, 1);
        }
        else if (player->GetQuestStatus(30033) == QUEST_STATUS_INCOMPLETE && (!player->HasItemCount(76392) || !player->HasItemCount(76390)))
        {
            player->AddItem(76392, 1);
            player->AddItem(76390, 1);
        }
        else if (player->GetQuestStatus(30034) == QUEST_STATUS_INCOMPLETE && !player->HasItemCount(73211))
        {
            player->AddItem(73211, 1);
        }
        else if (player->GetQuestStatus(30035) == QUEST_STATUS_INCOMPLETE && (!player->HasItemCount(76393) || !player->HasItemCount(73207)))
        {
            player->AddItem(76393, 1);
            player->AddItem(73207, 1);
        }
        else if (player->GetQuestStatus(30036) == QUEST_STATUS_INCOMPLETE && (!player->HasItemCount(73212) || !player->HasItemCount(73208)))
        {
            player->AddItem(73212, 1);
            player->AddItem(73208, 1);
        }
        else if (player->GetQuestStatus(30037) == QUEST_STATUS_INCOMPLETE && (!player->HasItemCount(73213) || !player->HasItemCount(76391)))
        {
            player->AddItem(73213, 1);
            player->AddItem(76391, 1);
        }
        else if (player->GetQuestStatus(30038) == QUEST_STATUS_INCOMPLETE && !player->HasItemCount(73210))
        {
            player->AddItem(73210, 1);
        }

        return true;
    }
};

class mob_training_target : public CreatureScript
{
public:
    mob_training_target() : CreatureScript("mob_training_target") { }
    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_training_targetAI(creature);
    }
    
    struct mob_training_targetAI : public ScriptedAI
    {
        mob_training_targetAI(Creature* creature) : ScriptedAI(creature) {}
        
        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void EnterCombat(Unit* /*enemy*/)
        {
            return;
        }
    };
};

class mob_tushui_trainee : public CreatureScript
{
    public:
        mob_tushui_trainee() : CreatureScript("mob_tushui_trainee") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_tushui_trainee_AI(creature);
        }

        struct mob_tushui_trainee_AI : public ScriptedAI
        {
            mob_tushui_trainee_AI(Creature* creature) : ScriptedAI(creature)
            {
            }

            bool isInCombat;
            uint64 playerGUID;
            uint32 punch1;
            uint32 punch2;
            uint32 punch3;

            void Reset()
            {
                punch1 = 1000;
                punch2 = 3500;
                punch3 = 6000;
                playerGUID = 0;
                isInCombat = false;
                me->SetReactState(REACT_DEFENSIVE);
                me->setFaction(7);
                me->SetFullHealth();
            }

            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const* /*p_SpellInfo*/)
            {
                if (me->HealthBelowPctDamaged(16.67f, damage))
                {
                    me->setFaction(35);

                    if(attacker && attacker->IsPlayer())
                        attacker->ToPlayer()->KilledMonsterCredit(54586, 0);

                    damage = 0;
                    me->CombatStop();
                    isInCombat = false;
                    me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
                    Talk(urand(0, 7));
                    me->GetMotionMaster()->MovePoint(0, 1446.322876f, 3389.027588f, 173.782471f);
                }
            }

            void EnterCombat(Unit* /*unit*/)
            {
                isInCombat = true;
            }

            void JustRespawned()
            {
                Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (isInCombat)
                {
                    DoMeleeAttackIfReady();
                    return;
                }
                else
                {
                    if (punch1 <= diff)
                    {
                        me->HandleEmoteCommand(35);
                        punch1 = 7500;
                    }
                    else
                        punch1 -= diff;

                    if (punch2 <= diff)
                    {
                        me->HandleEmoteCommand(36);
                        punch2 = 7500;
                    }
                    else
                        punch2 -= diff;

                    if (punch3 <= diff)
                    {
                        me->HandleEmoteCommand(37);
                        punch3 = 7500;
                    }
                    else
                        punch3 -= diff;
                }

                if (me->GetPositionX() >= 1446.0f  && me->GetPositionX() <= 1447.0f &&
                    me->GetPositionY() >= 3389.0f && me->GetPositionY() <= 3390.0f)

                me->DespawnOrUnsummon();
            }
        };
};

class boss_jaomin_ro : public CreatureScript
{
public:
    boss_jaomin_ro() : CreatureScript("boss_jaomin_ro") { }
    
    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_jaomin_roAI(creature);
    }
    
    struct boss_jaomin_roAI : public ScriptedAI
    {
        boss_jaomin_roAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        enum eJaominEvents
        {
            EVENT_JAOMIN_JUMP   = 1,
            EVENT_HIT_CIRCLE    = 2,
            EVENT_FALCON        = 3,
            EVENT_FALCON_ATTACK = 4,
            EVENT_RESET         = 5,
            EVENT_CHECK_AREA    = 6,
            EVENT_JAOMIN_BUMP   = 7,
            EVENT_STUNNED       = 8
        };

        EventMap m_Events;
        bool m_HasSaidIntro;
        uint32 spellJumpTimer;
        bool m_HasScheduledFalcon;
        bool m_CombatEngaged;

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            m_CombatEngaged = true;

            m_Events.ScheduleEvent(EVENT_JAOMIN_JUMP, 1000);
            m_Events.ScheduleEvent(EVENT_JAOMIN_BUMP, 3200);
            m_Events.ScheduleEvent(EVENT_HIT_CIRCLE, 12000);
            m_Events.ScheduleEvent(EVENT_CHECK_AREA, 12500);
        }
        
        void Reset() override
        {
            m_Events.Reset();
            me->setFaction(7);
            me->SetReactState(REACT_DEFENSIVE);
            me->CombatStop(true);
            me->SetDisplayId(39755);
            m_HasSaidIntro = false;
            m_HasScheduledFalcon = false;
            m_CombatEngaged = false;
            me->HandleEmoteCommand(EMOTE_STATE_SIT);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->GetMotionMaster()->MovePoint(1, me->GetHomePosition());
        }

        void JustSummoned(Creature* p_Summoned) override
        {
            if (p_Summoned->GetEntry() == 57750) // Script of the falcon attack
            {
                p_Summoned->CastSpell(me->getVictim(), 108935, true);
                p_Summoned->DespawnOrUnsummon();
            }
        }

        void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* /*p_SpellInfo*/) override
        {
            if (me->GetReactState() != REACT_AGGRESSIVE)
                me->SetReactState(REACT_AGGRESSIVE);

            if (Player* l_Player = p_Attacker->ToPlayer())
            {
                if (me->HealthBelowPctDamaged(30, p_Damage))
                {
                    if (!m_HasScheduledFalcon)
                    {
                        m_Events.ScheduleEvent(EVENT_FALCON, 1000);
                        m_Events.ScheduleEvent(EVENT_FALCON_ATTACK, 1800);
                        m_Events.ScheduleEvent(EVENT_STUNNED, 2800);
                        m_HasScheduledFalcon = true;
                    }
                    m_Events.CancelEvent(EVENT_JAOMIN_JUMP);
                    m_Events.CancelEvent(EVENT_JAOMIN_BUMP);
                    m_Events.CancelEvent(EVENT_HIT_CIRCLE);
                }
                else if (me->HealthBelowPctDamaged(5, p_Damage))
                {
                    p_Damage = 0;
                    Talk(urand(1, 6));
                    l_Player->KilledMonsterCredit(me->GetEntry(), 0);
                    l_Player->CombatStop(true);

                    if (me->HasUnitState(UNIT_STATE_ROOT))
                        me->ClearUnitState(UNIT_STATE_ROOT);

                    me->CombatStop();
                    me->setFaction(35);
                    me->SetFullHealth();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);

                    m_Events.ScheduleEvent(EVENT_RESET, 3000);
                }

                if (p_Damage > me->GetHealth())
                    p_Damage = 0;
            }
        }

        void EnterEvadeMode() override
        {
            _EnterEvadeMode();
            m_CombatEngaged = false;
            m_Events.ScheduleEvent(EVENT_RESET, 3000);
        }

        void DoAction(const int32 p_Action) override
        {
            if (p_Action == ACTION_TALK)
            {
                if (m_HasSaidIntro == false)
                {
                    Talk(0);
                    m_HasSaidIntro = true;
                }
            }
        }

        void UpdateAI(const uint32 diff) override
        {
            m_Events.Update(diff);

            if (!UpdateVictim() && !m_CombatEngaged)
            {
                Player* l_Player = me->FindNearestPlayer(15.0f);

                if (l_Player != nullptr && l_Player->GetQuestStatus(29409) == QUEST_STATUS_INCOMPLETE)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->HandleEmoteCommand(EMOTE_STATE_STAND);
                    me->SetOrientation(1.98f);
                    me->SetFacingTo(1.98f);
                    me->setFaction(7);
                    me->SetReactState(REACT_DEFENSIVE);
                    DoAction(ACTION_TALK);
                }

                return;
            }

            while (uint32 l_EventId = m_Events.ExecuteEvent())
            {
                switch (l_EventId)
                {
                    case EVENT_JAOMIN_JUMP: // Jaomin jumps to prepare the knock
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, 108938, true);
                        m_Events.ScheduleEvent(EVENT_JAOMIN_JUMP, 12000);
                        break;
                    case EVENT_JAOMIN_BUMP: // Jaomin inflicts damage, next to EVENT_JAOMIN_JUMP
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, 108937, true);
                        m_Events.ScheduleEvent(EVENT_JAOMIN_BUMP, 12000);
                        break;
                    case EVENT_HIT_CIRCLE: // Circular knock
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, 119301, true);
                        m_Events.ScheduleEvent(EVENT_HIT_CIRCLE, 15000);
                        break;
                    case EVENT_FALCON: // Here summons the falcon
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, 108955, true);
                        m_Events.ScheduleEvent(EVENT_FALCON, 12000);
                        break;
                    case EVENT_FALCON_ATTACK: // Here, attacks w/ the falcon
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, 108935, true);
                        m_Events.ScheduleEvent(EVENT_FALCON_ATTACK, 12000);
                        break;
                    case EVENT_STUNNED: // Here, gets stunned
                            me->CastSpell(me, 108959, true);
                            m_Events.ScheduleEvent(EVENT_STUNNED, 12000);
                        break;
                    case EVENT_RESET: // Comes back to initial pos
                        Reset();
                        break;
                    case EVENT_CHECK_AREA:
                        if (me->GetAreaId() != 5843) // Grotte Paisible (unk ?)
                            Reset();
                        break;
                }
            }
            
            DoMeleeAttackIfReady();
        }
    };
};

class mob_attacker_dimwind : public CreatureScript
{
public:
    mob_attacker_dimwind() : CreatureScript("mob_attacker_dimwind") { }
    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_attacker_dimwindAI(creature);
    }
    
    struct mob_attacker_dimwindAI : public ScriptedAI
    {
        mob_attacker_dimwindAI(Creature* creature) : ScriptedAI(creature) {}
        
        void DamageTaken(Unit* pDoneBy, uint32 &uiDamage, SpellInfo const* /*p_SpellInfo*/)
        {
            if(me->GetHealthPct() < 90 && pDoneBy && pDoneBy->ToCreature() && pDoneBy->ToCreature()->GetEntry() == 54785)
                uiDamage = 0;
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            if (me->GetPositionX() == 1403.440430f && me->GetPositionY() == 3566.382568f)
                me->DespawnOrUnsummon();
        }
    };
};

class mob_min_dimwind : public CreatureScript
{
public:
    mob_min_dimwind() : CreatureScript("mob_min_dimwind") { }
    
    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_min_dimwindAI(creature);
    }
    
    struct mob_min_dimwindAI : public ScriptedAI
    {
        EventMap events;
        uint64 guidMob[4];

        enum eEvents
        {
            EVENT_CHECK_MOBS    = 1,
            EVENT_RESET         = 2
        };
        
        mob_min_dimwindAI(Creature* creature) : ScriptedAI(creature)
        {
            for(int i = 0; i < 4; i++)
                guidMob[i] = 0;

            ResetMobs();
            me->HandleEmoteCommand(EMOTE_STATE_READY2H);
        }

        void Reset() override
        {
            me->SetRespawnDelay(10);
            events.ScheduleEvent(EVENT_RESET, 1000);
        }
        
        void DamageTaken(Unit* pDoneBy, uint32 &uiDamage, SpellInfo const* /*p_SpellInfo*/)
        {
            if(me->GetHealthPct() < 25 && pDoneBy && pDoneBy->ToCreature() && pDoneBy->ToCreature()->GetEntry() == 54130)
                uiDamage = 0;
        }
        
        bool VerifyMobs()
        {
            bool HasRemainingAttacker = false;

            for(int i = 0; i < 4; i++)
            {
                if(guidMob[i])
                {
                    if (Unit* unit = sObjectAccessor->FindUnit(guidMob[i]))
                    {
                        if(unit->isAlive())
                            HasRemainingAttacker = true;
                    }
                    else
                        guidMob[i] = 0;
                }
            }

            return !HasRemainingAttacker;
        }
        
        void ResetMobs()
        {
            events.ScheduleEvent(EVENT_CHECK_MOBS, 1000);
            me->HandleEmoteCommand(EMOTE_STATE_READY2H);

            for(int i = 0; i < 4; i++)
            {
                if(guidMob[i])
                    if (Unit* unit = sObjectAccessor->FindUnit(guidMob[i]))
                        if (unit->ToCreature())
                            unit->ToCreature()->DespawnOrUnsummon();

                guidMob[i] = 0;

                if(TempSummon* temp = me->SummonCreature(54130, me->GetPositionX()-3+rand()%6, me->GetPositionY() + 4 + rand()%4, me->GetPositionZ()+2, 3.3f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                {
                    guidMob[i] = temp->GetGUID();
                    temp->SetFacingToObject(me);
                    temp->HandleEmoteCommand(EMOTE_STATE_READY2H);
                    temp->GetMotionMaster()->Clear(false);
                    temp->GetMotionMaster()->MoveChase(me);
                    temp->Attack(me, true);
                    temp->getThreatManager().addThreat(me, 250.0f);
                }
            }
        }

        void MovementInform(uint32 p_Type, uint32 p_Id) override
        {
            if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                return;

            if (p_Id == 1)
                me->DespawnOrUnsummon();
        }
        
        void UpdateAI(const uint32 diff)
        {
            std::list<Player*> PlayerList;
            GetPlayerListInGrid(PlayerList, me, 20.0f);
            std::list<Creature*> amberleafScampList;
            GetCreatureListWithEntryInGrid(amberleafScampList, me, 54130, 15.0f);
            for (auto player : PlayerList)
            {
                if (player->GetQuestStatus(29419) == QUEST_STATUS_INCOMPLETE)
                {
                    player->KilledMonsterCredit(54855, 0);

                    for (auto amberleafScamp : amberleafScampList)
                        amberleafScamp->GetMotionMaster()->MovePoint(0, 1403.440430f, 3566.382568f, 88.840317f);

                    Talk(0);
                }
            }

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CHECK_MOBS:
                    {
                        if(VerifyMobs()) // No more mobs, objective completed
                        {
                            me->HandleEmoteCommand(EMOTE_STATE_STAND);
                            me->MonsterYell("Thank you!", LANG_UNIVERSAL, 0);
                        
                            std::list<Player*> PlayerList;
                            GetPlayerListInGrid(PlayerList, me, 20.0f);
                            for (auto player: PlayerList)
                                player->KilledMonsterCredit(54855, 0);

                            me->SetWalk(false);
                            me->GetMotionMaster()->MovePoint(1, 1406.08f, 3560.41f, 88.2305f);
                        }
                        else
                            events.ScheduleEvent(EVENT_CHECK_MOBS, 1000);

                        break;
                    }
                    case EVENT_RESET:
                        ResetMobs();
                        break;
                    default:
                        break;
                }
            }
        }
    };
};

class mob_aysa_lake_escort : public CreatureScript
{
public:
    mob_aysa_lake_escort() : CreatureScript("mob_aysa_lake_escort") { }

    struct mob_aysa_lake_escortAI : public npc_escortAI
    {
        mob_aysa_lake_escortAI(Creature* creature) : npc_escortAI(creature)
        {}

        uint32 IntroTimer;

        void Reset()
        {
            IntroTimer = 2500;
        }

        void MovementInform(uint32 uiType, uint32 uiId)
        {
            npc_escortAI::MovementInform(uiType, uiId);

            if (uiType != POINT_MOTION_TYPE && uiType != EFFECT_MOTION_TYPE)
                return;

            switch (uiId)
            {
                case 1:
                    me->GetMotionMaster()->MoveJump(1227.11f, 3489.73f, 100.37f, 10, 20, 10.0f, 2);
                    break;
                case 2:
                    me->GetMotionMaster()->MoveJump(1236.68f, 3456.68f, 102.58f, 10, 20, 10.0f, 3);
                    break;
                case 3:
                    Start(false, true);
                    break;
                default:
                    break;
            }
        }

        void WaypointReached(uint32 waypointId)
        {
            if (waypointId == 4)
                me->DespawnOrUnsummon(500);
        }

        void UpdateAI(const uint32 diff)
        {
            if (IntroTimer)
            {
                if (IntroTimer <= diff)
                {
                    me->MonsterYell("Follow me!", LANG_UNIVERSAL, 0);
                    IntroTimer = 0;
                    me->GetMotionMaster()->MoveJump(1216.78f, 3499.44f, 91.15f, 10, 20, 10.0f, 1);
                }
                else
                    IntroTimer -= diff;
            }

            npc_escortAI::UpdateAI(diff);
        }
    };
    
    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_aysa_lake_escortAI(creature);
    }
    
};

#define ACTION_TALK_1 2

class mob_aysa : public CreatureScript
{
public:
    mob_aysa() : CreatureScript("mob_aysa") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == 29414) // La voie des tushui
            if (Creature* tempSummon = creature->SummonCreature(56661, creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID()))
                tempSummon->SetPhaseMask(65535, true);

        return true;
    }
    
    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_aysaAI(creature);
    }
    
    struct mob_aysaAI : public ScriptedAI
    {
        EventMap events;
        std::list<Player*> playersInvolved;
        uint64 lifeiGUID;
        bool inCombat;
        uint64 timer;
        bool hasSaidIntro;
        
        mob_aysaAI(Creature* creature) : ScriptedAI(creature)
        {
            events.ScheduleEvent(1, 600); //Begin script
            inCombat = false;
            hasSaidIntro = false;
            timer = 0;
            lifeiGUID = 0;
            me->SetReactState(REACT_DEFENSIVE);
            me->setFaction(2263);
        }

        enum eEvents
        {
            EVENT_START         = 1,
            EVENT_SPAWN_MOBS    = 2,
            EVENT_PROGRESS      = 3,
            EVENT_END           = 4,
        };
        
        void DamageTaken(Unit* /*pDoneBy*/, uint32 &uiDamage, SpellInfo const* /*p_SpellInfo*/)
        {
            if(me->HealthBelowPctDamaged(5, uiDamage))
            {
                if(lifeiGUID)
                {
                    if (Creature* lifei = me->GetCreature(*me, lifeiGUID))
                        if (TempSummon* summon = lifei->ToTempSummon())
                            summon->UnSummon();
                    lifeiGUID = 0;
                }
                
                uiDamage = 0;
                me->MonsterSay("I can't meditate!", LANG_UNIVERSAL, 0);
                me->SetFullHealth();
                me->SetReactState(REACT_DEFENSIVE);
                
                std::list<Creature*> unitlist;
                GetCreatureListWithEntryInGrid(unitlist, me, 59637, 50.0f);
                for (auto creature: unitlist)
                    me->Kill(creature);
                    
                events.ScheduleEvent(EVENT_START, 20000);
                events.CancelEvent(EVENT_SPAWN_MOBS);
                events.CancelEvent(EVENT_PROGRESS);
                events.CancelEvent(EVENT_END);
            }
        }
        
        void updatePlayerList()
        {
            playersInvolved.clear();
            
            std::list<Player*> PlayerList;
            GetPlayerListInGrid(PlayerList, me, 20.0f);

            for (auto player: PlayerList)
            {
                if (player->GetQuestStatus(29414) == QUEST_STATUS_INCOMPLETE)
                    playersInvolved.push_back(player);

                if (player->GetQuestStatus(29419) == QUEST_STATUS_COMPLETE && player->GetQuestStatus(29424) == QUEST_STATUS_COMPLETE
                    && me->GetPositionX() == 1206.310059f && me->GetPositionY() == 3507.459961f)
                {
                    DoAction(ACTION_TALK_1);
                }
                else if (player->GetQuestStatus(29410) != QUEST_STATUS_INCOMPLETE)
                    me->SetReactState(REACT_PASSIVE);
            }
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
            case ACTION_TALK_1:
                if (!hasSaidIntro)
                {
                    Talk(0);
                    hasSaidIntro = true;
                }
                break;
            }
        }
        
        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_START: //Begin script if playersInvolved is not empty
                    {
                        updatePlayerList();
                        if(playersInvolved.empty())
                            events.ScheduleEvent(1, 600);
                        else
                        {
                            me->MonsterSay("Keep those creatures at bay while I meditate. We'll soon have the answers we seek...", LANG_UNIVERSAL, 0);
                            me->SetReactState(REACT_PASSIVE);
                            timer = 0;
                            events.ScheduleEvent(EVENT_SPAWN_MOBS, 5000); //spawn mobs
                            events.ScheduleEvent(EVENT_PROGRESS, 1000); //update time
                            events.ScheduleEvent(EVENT_END, 90000); //end quest
                        }
                        break;
                    }
                    case EVENT_SPAWN_MOBS: //Spawn 3 mobs
                    {
                        updatePlayerList();
                        for(int i = 0; i < std::max((int)playersInvolved.size()*3,3); i++)
                        {
                            if(TempSummon* temp = me->SummonCreature(59637, 1144.55f, 3435.65f, 104.97f, 3.3f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                            {
                                if (temp->AI())
                                    temp->AI()->AttackStart(me);

                                temp->AddThreat(me, 250.0f);
                                temp->GetMotionMaster()->Clear();
                                temp->GetMotionMaster()->MoveChase(me);
                            }
                        }
                        events.ScheduleEvent(EVENT_SPAWN_MOBS, 20000); //spawn mobs
                        break;
                    }
                    case EVENT_PROGRESS: //update energy
                    {
                        timer++;

                        TempSummon* lifei = NULL;
                        if (Creature* c = me->GetCreature(*me, lifeiGUID))
                            if (TempSummon* summon = c->ToTempSummon())
                                lifei = summon;

                        if(timer == 25 && !lifeiGUID)
                        {
                            if ((lifei = me->SummonCreature(54856, 1130.162231f, 3435.905518f, 105.496597f, 0.0f,TEMPSUMMON_MANUAL_DESPAWN)))
                            {
                                lifeiGUID = lifei->GetGUID();
                                lifei->MonsterSay("The way of the Tushui... enlightenment through patience and mediation... the principled life", LANG_UNIVERSAL, 0);
                            }
                        }
                        
                        if(timer == 30)
                            if (lifei)
                                lifei->MonsterSay("It is good to see you again, Aysa. You've come with respect, and so I shall give you the answers you seek.", LANG_UNIVERSAL, 0);
                        
                        if(timer == 42)
                            if (lifei)
                                lifei->MonsterSay("Huo, the spirit of fire, is known for his hunger. He wants for tinder to eat. He needs the caress of the wind to rouse him.", LANG_UNIVERSAL, 0);
                        
                        if(timer == 54)
                            if (lifei)
                                lifei->MonsterSay("If you find these things and bring them to his cave, on the far side of Wu-Song Village, you will face a challenge within.", LANG_UNIVERSAL, 0);
                        
                        if(timer == 66)
                            if (lifei)
                                lifei->MonsterSay("Overcome that challenge, and you shall be graced by Huo's presence. Rekindle his flame, and if your spirit is pure, he shall follow you.", LANG_UNIVERSAL, 0);
                        
                        if(timer == 78)
                            if (lifei)
                                lifei->MonsterSay("Go, children. We shall meet again very soon.", LANG_UNIVERSAL, 0);
                        
                        if(timer == 85)
                        {
                            if (lifei)
                                lifei->UnSummon();

                            lifei = NULL;
                            lifeiGUID = 0;
                        }
                        
                        updatePlayerList();
                        for (auto player: playersInvolved)
                        {
                            if(!player->HasAura(116421))
                                player->CastSpell(player, 116421);

                            player->ModifyPower(POWER_ALTERNATE_POWER, timer/25);
                            player->SetMaxPower(POWER_ALTERNATE_POWER, 90);
                        }

                        events.ScheduleEvent(EVENT_PROGRESS, 1000);
                        break;
                    }
                    case EVENT_END: //script end
                    {
                        TempSummon* lifei = NULL;

                        if (Creature* c = me->GetCreature(*me, lifeiGUID))
                            if (TempSummon* summon = c->ToTempSummon())
                                lifei = summon;

                        if(lifei)
                        {
                            lifei->UnSummon();
                            lifei = NULL;
                            lifeiGUID = 0;
                        }

                        events.ScheduleEvent(EVENT_START, 10000);
                        events.CancelEvent(EVENT_SPAWN_MOBS);
                        events.CancelEvent(EVENT_PROGRESS);
                        me->MonsterSay("And so our path lays before us. Speak to Master Shang Xi, he will tell you what comes next.", LANG_UNIVERSAL, 0);
                        updatePlayerList();
                        me->SetReactState(REACT_DEFENSIVE);
                        for(auto player: playersInvolved)
                        {
                            player->KilledMonsterCredit(54856, 0);
                            player->RemoveAura(116421);
                        }
                        break;
                    }
                }
            }
        }
    };
};

class boss_living_air : public CreatureScript
{
public:
    boss_living_air() : CreatureScript("boss_living_air") { }
    
    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_living_airAI(creature);
    }
    
    struct boss_living_airAI : public ScriptedAI
    {
        boss_living_airAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetReactState(REACT_AGGRESSIVE);
        }
        
        EventMap events;
        
        void EnterCombat(Unit* /*unit*/)
        {
            events.ScheduleEvent(1, 3000);
            events.ScheduleEvent(2, 7000);
        }
        
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
            
            events.Update(diff);
            
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case 1:
                        me->CastSpell(me->getVictim(), 108693);
                        break;
                    case 2:
                        me->CastSpell(me->getVictim(), 73212);
                        events.ScheduleEvent(2, 5000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

#define QUEST_ONLY_THE_WORTHY_SHALL_PASS  29421

class npc_li_fei : public CreatureScript
{
public:
    npc_li_fei() : CreatureScript("npc_li_fei") {}

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_ONLY_THE_WORTHY_SHALL_PASS) // Only the Worthy Shall Pass
        {
            Creature* tempSummon = creature->SummonCreature(54856, creature->GetPositionX() + 19, creature->GetPositionY() + 4, creature->GetPositionZ() - 1, creature->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
            if (tempSummon)
            {
                tempSummon->SetPhaseMask(1024, true);
                tempSummon->AI()->SetGUID(player->GetGUID());
                tempSummon->AI()->Reset();
                tempSummon->AI()->AttackStart(player);
            }
        }

        return true;
    }
};

enum eLiFeiAuras
{
    AURA_GHOST_VISUAL                  =  22650,
    AURA_BLESSING_OF_THE_PURPLE_FLAME  = 102510,
    AURA_BLESSING_OF_THE_BLUE_FLAME    = 102509,
    AURA_BLESSING_OF_THE_RED_FLAME     = 102508
};

class boss_li_fei_fight : public CreatureScript
{
public:
    boss_li_fei_fight() : CreatureScript("boss_li_fei_fight") { }
    
    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_li_fei_fightAI(creature);
    }
    
    struct boss_li_fei_fightAI : public ScriptedAI
    {
        EventMap events;
        uint64 playerGuid;

        boss_li_fei_fightAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        enum eLiFeiEvents
        {
            EVENT_CHECK_PLAYER      = 1,
            EVENT_FEET_OF_FURY      = 2,
            EVENT_SHADOW_KICK       = 3,
            EVENT_SHADOW_KICK_STUN  = 4,
        };

        void Reset()
        {
            std::list<Player*> playerList;
            GetPlayerListInGrid(playerList, me, 30.0f);

            // This particular entry is also spawned on an other event
            if (me->GetAreaId() != 5825) // Cavern areaid
                return;

            playerGuid = 0;

            for (auto player: playerList)
            {
                if (player->GetQuestStatus(29421) == QUEST_STATUS_INCOMPLETE)
                {
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->setFaction(7);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                    me->AddAura(AURA_GHOST_VISUAL, me);
                    me->AddAura(AURA_BLESSING_OF_THE_PURPLE_FLAME, me);
                    me->AddAura(AURA_BLESSING_OF_THE_BLUE_FLAME, me);
                    me->AddAura(AURA_BLESSING_OF_THE_RED_FLAME, me);
                    player->GetMotionMaster()->MoveJump(me->GetPositionX() + 1, me->GetPositionY(), me->GetPositionZ(), 15.0f, 15.0f);
                    player->AddAura(AURA_BLESSING_OF_THE_PURPLE_FLAME, player);
                    player->AddAura(AURA_BLESSING_OF_THE_BLUE_FLAME, player);
                    player->AddAura(AURA_BLESSING_OF_THE_RED_FLAME, player);

                    events.ScheduleEvent(EVENT_CHECK_PLAYER, 2500);
                    events.ScheduleEvent(EVENT_FEET_OF_FURY, 5000);
                    events.ScheduleEvent(EVENT_SHADOW_KICK,  1000);
                    return;
                }
            }
        }

        void SetGUID(uint64 guid, int32 /*type*/)
        {
            playerGuid = guid;
        }
        
        void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const* /*p_SpellInfo*/)
        {
            if (me->HealthBelowPctDamaged(10, damage))
            {
                damage = 0;
                me->setFaction(35);
                me->CombatStop();
                me->RemoveAura(AURA_BLESSING_OF_THE_PURPLE_FLAME);
                me->RemoveAura(AURA_BLESSING_OF_THE_BLUE_FLAME);
                me->RemoveAura(AURA_BLESSING_OF_THE_RED_FLAME);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
                me->DespawnOrUnsummon(2000);

                if (attacker->IsPlayer())
                    if (Player* player = attacker->ToPlayer())
                    {
                            player->KilledMonsterCredit(54734, 0);
                            player->RemoveAura(AURA_BLESSING_OF_THE_PURPLE_FLAME);
                            player->RemoveAura(AURA_BLESSING_OF_THE_BLUE_FLAME);
                            player->RemoveAura(AURA_BLESSING_OF_THE_RED_FLAME);
                    }
            }
        }

        void KilledUnit(Unit* victim)
        {
            if (victim->IsPlayer())
            {
                victim->ToPlayer()->SetQuestStatus(QUEST_ONLY_THE_WORTHY_SHALL_PASS, QUEST_STATUS_FAILED);

                if (victim->GetGUID() == playerGuid)
                    me->DespawnOrUnsummon(3000);
            }
        }
        
        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);
            
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_CHECK_PLAYER:
                    {
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, me, 30.0f);
                        for (auto player: playerList)
                        {
                            if (!player->isAlive())
                            {
                                me->DespawnOrUnsummon(1000);
                                playerGuid = 0;
                                break;
                            }

                            if (player->GetQuestStatus(QUEST_ONLY_THE_WORTHY_SHALL_PASS) != QUEST_STATUS_INCOMPLETE)
                            {
                                me->DespawnOrUnsummon(1000);
                                playerGuid = 0;
                                break;
                            }
                        }

                        events.ScheduleEvent(EVENT_CHECK_PLAYER, 2500);
                        break;
                    }
                    case EVENT_FEET_OF_FURY:
                        if(me->getVictim())
                            me->CastSpell(me->getVictim(), 108958);

                        events.ScheduleEvent(EVENT_FEET_OF_FURY, 5000);
                        break;
                    case EVENT_SHADOW_KICK:
                        if(me->getVictim())
                            me->CastSpell(me->getVictim(), 108936);

                        events.ScheduleEvent(EVENT_SHADOW_KICK_STUN, 2500);
                        events.ScheduleEvent(EVENT_SHADOW_KICK, 30000);
                        break;
                    case EVENT_SHADOW_KICK_STUN:
                        if(me->getVictim())
                            me->CastSpell(me->getVictim(), 108944);
                        break;
                }
            }
            
            DoMeleeAttackIfReady();
        }
    };
};

// Huo Benediction - 102630
class spell_huo_benediction: public SpellScriptLoader
{
    public:
        spell_huo_benediction() : SpellScriptLoader("spell_huo_benediction") { }

        class spell_huo_benediction_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_huo_benediction_AuraScript);
            
            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();

                if (!target)
                    return;

                std::list<Creature*> huoList;
                GetCreatureListWithEntryInGrid(huoList, target, 54958, 20.0f);

                for (auto huo: huoList)
                    if (huo->ToTempSummon())
                        if (huo->ToTempSummon()->GetOwnerGUID() == target->GetGUID())
                            return;

                // A partir d'ici on sait que le joueur n'a pas encore de Huo
                if (TempSummon* tempHuo = target->SummonCreature(54958, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0.0f, TEMPSUMMON_MANUAL_DESPAWN, 0, target->GetGUID()))
                {
                    tempHuo->SetOwnerGUID(target->GetGUID());
                    tempHuo->GetMotionMaster()->MoveFollow(target, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();

                if (!target)
                    return;

                std::list<Creature*> huoList;
                GetCreatureListWithEntryInGrid(huoList, target, 54958, 20.0f);

                for (auto huo: huoList)
                    if (huo->ToTempSummon())
                        if (huo->ToTempSummon()->GetOwnerGUID() == target->GetGUID())
                            huo->DespawnOrUnsummon();
            }

            void Register()
            {
                OnEffectApply  += AuraEffectApplyFn (spell_huo_benediction_AuraScript::OnApply,  EFFECT_0, SPELL_AURA_MOD_INCREASE_SPEED, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                OnEffectRemove += AuraEffectRemoveFn(spell_huo_benediction_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_INCREASE_SPEED, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_huo_benediction_AuraScript();
        }
};

class AreaTrigger_at_temple_entrance : public AreaTriggerScript
{
    public:
        AreaTrigger_at_temple_entrance() : AreaTriggerScript("AreaTrigger_at_temple_entrance")
        {}

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            if (player->GetQuestStatus(29423) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(61128, 0);

                std::list<Creature*> huoList;
                GetCreatureListWithEntryInGrid(huoList, player, 54958, 20.0f);

                for (auto huo: huoList)
                {
                    if (huo->ToTempSummon())
                    {
                        if (huo->ToTempSummon()->GetOwnerGUID() == player->GetGUID())
                        {
                            huo->GetMotionMaster()->Clear();
                            huo->GetMotionMaster()->MovePoint(1, 950.0f, 3601.0f, 203.0f);
                            huo->DespawnOrUnsummon(5000);
                        }
                    }
                }
            }

            return true;
        }
};


class mob_trainee_nim : public CreatureScript
{
public:
    mob_trainee_nim() : CreatureScript("mob_trainee_nim") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_trainee_nimAI(creature);
    }

    struct mob_trainee_nimAI : public ScriptedAI
    {
        bool hasSaidIntro;

        mob_trainee_nimAI(Creature* creature) : ScriptedAI(creature)
        {
            hasSaidIntro = false;
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
            case ACTION_TALK:
                Talk(0);
                hasSaidIntro = true;
                break;
            default:
                break;
            }
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            std::list<Player*> playerList;
            GetPlayerListInGrid(playerList, me, 15.0f);

            for (auto player: playerList)
                if (!hasSaidIntro && player->GetQuestStatus(29409) == QUEST_STATUS_INCOMPLETE)
                    DoAction(ACTION_TALK);
        }
    };
};

enum eEvents
{
    EVENT_PUNCH     = 1,
    EVENT_COPY_ANIM = 2
};

#define TRAINEE 65469

enum eActions
{
    ACTION_PUNCH = 0
};

class mob_instructors : public CreatureScript
{
public:
    mob_instructors() : CreatureScript("mob_instructors") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_instructorsAI(creature);
    }

    struct mob_instructorsAI : public ScriptedAI
    {
        EventMap events;
        uint8 rand;

        mob_instructorsAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 punch1;
        uint32 punch2;
        uint32 punch3;

        void Reset()
        {
            punch1 = 300;
            punch2 = 2800;
            punch3 = 5300;
        }

        void UpdateAI(const uint32 diff)
        {
            if (punch1 <= diff)
            {
                me->HandleEmoteCommand(35);
                punch1 = 7500;
            }
            else
                punch1 -= diff;

            if (punch2 <= diff)
            {
                me->HandleEmoteCommand(36);
                punch2 = 7500;
            }
            else
                punch2 -= diff;

            if (punch3 <= diff)
            {
                me->HandleEmoteCommand(37);
                punch3 = 7500;
            }
            else
                punch3 -= diff;
        }
    };
};

class mob_aspiring_trainee : public CreatureScript
{
public:
    mob_aspiring_trainee() : CreatureScript("mob_aspiring_trainee") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_aspiring_traineeAI(creature);
    }

    struct mob_aspiring_traineeAI : public ScriptedAI
    {
        mob_aspiring_traineeAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 punch1;
        uint32 punch2;
        uint32 punch3;

        void Reset()
        {
            punch1 = 1000;
            punch2 = 3500;
            punch3 = 6000;
        }

        void UpdateAI(const uint32 diff)
        {
            if (punch1 <= diff)
            {
                me->HandleEmoteCommand(35);
                punch1 = 7500;
            }
            else
                punch1 -= diff;

            if (punch2 <= diff)
            {
                me->HandleEmoteCommand(36);
                punch2 = 7500;
            }
            else
                punch2 -= diff;

            if (punch3 <= diff)
            {
                me->HandleEmoteCommand(37);
                punch3 = 7500;
            }
            else
                punch3 -= diff;
        }
    };
};

class mob_huojin_trainee : public CreatureScript
{
    public:
        mob_huojin_trainee() : CreatureScript("mob_huojin_trainee") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_huojin_traineeAI(creature);
        }

        struct mob_huojin_traineeAI : public ScriptedAI
        {
            mob_huojin_traineeAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            uint8 punch;
            bool isInCombat;

            void Reset()
            {
                punch = urand(500, 3000);
                me->SetReactState(REACT_DEFENSIVE);
                me->SetFullHealth();
                me->setFaction(7);
                isInCombat = false;
            }

            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const* /*p_SpellInfo*/)
            {
                if (me->HealthBelowPctDamaged(16.67f, damage))
                {
                    damage = 0;
                    me->CombatStop();
                    me->setFaction(35);
                    isInCombat = false;
                    me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
                    Talk(urand(0, 7));
                    me->GetMotionMaster()->MovePoint(0, 1446.322876f, 3389.027588f, 173.782471f);

                    if (attacker && attacker->IsPlayer())
                        attacker->ToPlayer()->KilledMonsterCredit(54586, 0);
                }
            }

            void EnterCombat(Unit* /*unit*/)
            {
                isInCombat = true;
            }

            void JustRespawned()
            {
                Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (isInCombat)
                {
                    DoMeleeAttackIfReady();
                    return;
                }

                else
                {
                    if (punch <= diff)
                    {
                        me->HandleEmoteCommand(35);
                        punch = urand(500, 3000);
                    }
                    else
                        punch -= diff;
                }

                if (me->GetPositionX() >= 1446.0f  && me->GetPositionX() <= 1447.0f &&
                    me->GetPositionY() >= 3389.0f && me->GetPositionY() <= 3390.0f)
                    me->DespawnOrUnsummon();
            }
        };
};

class npc_merchant_lorvo : public CreatureScript
{
    public:
        npc_merchant_lorvo() : CreatureScript("npc_merchant_lorvo") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_merchant_lorvoAI(creature);
        }

        struct npc_merchant_lorvoAI : public ScriptedAI
        {
            npc_merchant_lorvoAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            uint8 punch;
            bool hasSaid1;

            void Reset()
            {
                punch = urand(500, 3000);
                hasSaid1 = false;
            }

            void DoAction (int32 const action)
            {
                switch (action)
                {
                case ACTION_TALK:
                    if (!hasSaid1)
                    {
                        Talk(0);
                        hasSaid1 = true;
                    }
                    break;
                }
            }

            void UpdateAI(const uint32 /*diff*/)
            {

                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, me, 10.0f);

                for (auto player: playerList)
                {
                    if (player->GetQuestStatus(29410) == QUEST_STATUS_COMPLETE)
                    {
                        DoAction(ACTION_TALK);
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_ji_firepaw : public CreatureScript
{
    public:
        mob_ji_firepaw() : CreatureScript("mob_ji_firepaw") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_ji_firepawAI(creature);
        }

        struct mob_ji_firepawAI : public ScriptedAI
        {
            mob_ji_firepawAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            bool hasSaid1;

            void Reset()
            {
                hasSaid1 = false;
            }

            void DoAction (int32 const action)
            {
                switch (action)
                {
                case ACTION_TALK:
                    if (!hasSaid1)
                    {
                        Talk(0);
                        hasSaid1 = true;
                    }
                    break;
                }
            }

            void UpdateAI(const uint32 /*diff*/)
            {
                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, me, 3.0f);
                for (auto player: playerList)
                {
                    if (player->GetQuestStatus(29522) == QUEST_STATUS_COMPLETE)
                    {
                        DoAction(ACTION_TALK);
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_huojin_monk : public CreatureScript
{
    public:
        mob_huojin_monk() : CreatureScript("mob_huojin_monk") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_huojin_monkAI(creature);
        }

        struct mob_huojin_monkAI : public ScriptedAI
        {
            mob_huojin_monkAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            bool hasSaid1;

            void Reset()
            {
                hasSaid1 = false;
            }

            void DoAction (int32 const action)
            {
                if (action == ACTION_TALK)
                {
                    if (!hasSaid1)
                    {
                        Talk(0);
                        hasSaid1 = true;
                    }
                }
            }

            void UpdateAI(const uint32 /*diff*/)
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 10.0f);
                for (Player* l_Player : l_PlayerList)
                {
                    if (l_Player->GetQuestStatus(29420) == QUEST_STATUS_COMPLETE)
                        DoAction(ACTION_TALK);
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_chia_hui_autumnleaf : public CreatureScript
{
    public:
        mob_chia_hui_autumnleaf() : CreatureScript("mob_chia_hui_autumnleaf") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_chia_hui_autumnleafAI(creature);
        }

        struct mob_chia_hui_autumnleafAI : public ScriptedAI
        {
            mob_chia_hui_autumnleafAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            bool hasSaid1;

            void Reset()
            {
                hasSaid1 = false;
            }

            void DoAction (int32 const action)
            {
                switch (action)
                {
                case ACTION_TALK:
                    if (!hasSaid1)
                    {
                        Talk(0);
                        hasSaid1 = true;
                    }
                    break;
                }
            }

            void UpdateAI(const uint32 /*diff*/)
            {
                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, me, 12.0f);
                for (auto player: playerList)
                {
                    if (player->GetQuestStatus(29423) == QUEST_STATUS_INCOMPLETE)
                    {
                        DoAction(ACTION_TALK);
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_brewer_lin : public CreatureScript
{
    public:
        mob_brewer_lin() : CreatureScript("mob_brewer_lin") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_brewer_linAI(creature);
        }

        struct mob_brewer_linAI : public ScriptedAI
        {
            mob_brewer_linAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            bool hasSaid1;

            void Reset()
            {
                hasSaid1 = false;
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                case ACTION_TALK:
                    if (!hasSaid1)
                    {
                        Talk(0);
                        hasSaid1 = true;
                    }
                    break;
                }
            }

            void UpdateAI(const uint32 /*diff*/)
            {
                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, me, 4.0f);
                for (auto player: playerList)
                {
                    if (player->GetQuestStatus(29423) == QUEST_STATUS_INCOMPLETE)
                    {
                        DoAction(ACTION_TALK);
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class playerScript_pandaren_powerlevel : public PlayerScript
{
    public:
        playerScript_pandaren_powerlevel() : PlayerScript("playerScript_pandaren_powerlevel") { }

        void OnLogin(Player* p_Player)
        {
            if (p_Player->getRace() == RACE_PANDAREN_NEUTRAL && p_Player->getLevel() >= 75)
                p_Player->ShowNeutralPlayerFactionSelectUI();
        }

        void OnFactionChanged(Player* p_Player)
        {
            if (p_Player->getRace() == RACE_PANDAREN_ALLI || p_Player->getRace() == RACE_PANDAREN_HORDE)
            {
                for (int l_Itr = 0; l_Itr < MAX_QUEST_LOG_SIZE; l_Itr++)
                {
                    uint16 l_Slot = p_Player->FindQuestSlot(l_Itr);

                    p_Player->RemoveActiveQuest(g_PandarenQuests[l_Itr]);
                    p_Player->SetQuestSlot(l_Slot, 0);
                }
            }
        }
};

#ifndef __clang_analyzer__
void AddSC_WanderingIsland_North()
{
    new mob_master_shang_xi();
    new go_wandering_weapon_rack();
    new mob_training_target();
    new mob_tushui_trainee();
    new mob_huojin_trainee();
    new boss_jaomin_ro();
    new mob_attacker_dimwind();
    new mob_min_dimwind();
    new mob_aysa_lake_escort();
    new mob_aysa();
    new mob_trainee_nim();
    new mob_instructors();
    new mob_aspiring_trainee();
    new mob_ji_firepaw();
    new npc_merchant_lorvo();
    new mob_huojin_monk();
    new boss_living_air();
    new npc_li_fei();
    new boss_li_fei_fight();
    new mob_chia_hui_autumnleaf();
    new mob_brewer_lin();
    new spell_huo_benediction();
    new AreaTrigger_at_temple_entrance();
    new playerScript_pandaren_powerlevel();
}
#endif
