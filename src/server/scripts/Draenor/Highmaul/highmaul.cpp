////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

/// Gharg positions
Position const g_GhargFirstPos = { 3466.11f, 7577.58f, 15.203f, 0.8954f };
Position const g_GhargSecondPos = { 3483.23f, 7598.67f, 10.65f, 0.8954f };

/// Gharg Teleport position
Position const g_GhargTeleportPos = { 3475.60f, 7590.64f, 55.30f, 4.062f };

/// Mar'gok (Cosmetic) teleport position
Position const g_MargokTeleport = { 3432.25f, 7536.13f, 73.664f, 0.896154f };

/// Kargath Bladefist position
Position const g_KargathPos = { 3444.50f, 7550.76f, 55.39f, 0.90f };

/// Iron Warmaster (Brackenspore cosmetic) move positions
Position const g_IronWarmasterPos = { 4155.636719f, 7817.216309f, 0.253316f, 0.514213f };
Position const g_IronWarmasterJump = { 4182.975098f, 7839.367188f, 7.755508f, 5.603590f };

/// Teleporter to Imperator's Rise
Position const g_TeleporterSpawnPos = { 4186.096f, 8574.492f, 425.353f, 3.851739f };

/// Gharg <Arena Master> - 84971
class npc_highmaul_gharg_arena_master : public CreatureScript
{
    public:
        npc_highmaul_gharg_arena_master() : CreatureScript("npc_highmaul_gharg_arena_master") { }

        enum eMove
        {
            MoveFirstPos,
            MoveSecondPos
        };

        enum eAction
        {
            ActionMove
        };

        struct npc_highmaul_gharg_arena_masterAI : public ScriptedAI
        {
            npc_highmaul_gharg_arena_masterAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                me->SetWalk(true);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SummonGameObject(eHighmaulGameobjects::ArenaElevator, 3466.438f, 7577.974f, 14.94214f, 0.8901166f, 0.0f, 0.0f, 0.4305113f, 0.9025852f, 1000);
                me->GetMotionMaster()->MovePoint(eMove::MoveFirstPos, g_GhargFirstPos);
            }

            bool CanRespawn() override
            {
                return false;
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eAction::ActionMove)
                {
                    if (m_Instance == nullptr)
                        return;

                    me->GetMotionMaster()->MovePoint(eMove::MoveSecondPos, g_GhargSecondPos);
                    m_Instance->SetData(eHighmaulDatas::ElevatorActivated, true);
                }
            }

            void sGossipSelect(Player* p_Player, uint32 /*p_Sender*/, uint32 /*p_Action*/) override
            {
                if (m_Instance == nullptr)
                    return;

                /// Teleport player
                if (m_Instance->GetData(eHighmaulDatas::ElevatorActivated))
                    p_Player->NearTeleportTo(g_GhargTeleportPos);
                else
                {
                    me->GetMotionMaster()->MovePoint(eMove::MoveSecondPos, g_GhargSecondPos);
                    m_Instance->SetData(eHighmaulDatas::ElevatorActivated, true);
                }

                p_Player->PlayerTalkClass->SendCloseGossip();
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (m_Instance == nullptr || p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMove::MoveFirstPos:
                    {
                        me->SetFacingTo(0.8954f);
                        me->SetHomePosition(g_GhargSecondPos);
                        break;
                    }
                    case eMove::MoveSecondPos:
                    {
                        me->SetHomePosition(g_GhargSecondPos);

                        /// Start elevator
                        if (GameObject* l_Elevator = GameObject::GetGameObject(*me, m_Instance->GetData64(eHighmaulGameobjects::ArenaElevator)))
                            l_Elevator->SetTransportState(GOState::GO_STATE_TRANSPORT_STOPPED);

                        /// Active Collision Wall
                        if (GameObject* l_Wall = GameObject::GetGameObject(*me, m_Instance->GetData64(eHighmaulGameobjects::CollisionWall)))
                            l_Wall->SetGoState(GOState::GO_STATE_READY);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_gharg_arena_masterAI(p_Creature);
        }
};

/// Jhorn the Mad - 83377
class npc_highmaul_jhorn_the_mad : public CreatureScript
{
    public:
        npc_highmaul_jhorn_the_mad() : CreatureScript("npc_highmaul_jhorn_the_mad") { }

        enum eTalks
        {
            Intro1,
            Intro2,
            Intro3,
            Intro4,
            Intro5,
            Trash1,
            Trash2,
            Kargath1,
            Kargath2
        };

        enum eActions
        {
            StartIntro,
            ContinueIntro,
            VulgorDied
        };

        struct npc_highmaul_jhorn_the_madAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_jhorn_the_madAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::StartIntro:
                    {
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro1); });
                        AddTimedDelayedOperation(16 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro2); });
                        AddTimedDelayedOperation(38 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro3); });

                        AddTimedDelayedOperation(54 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            if (m_Instance != nullptr)
                            {
                                if (GameObject* l_InnerGate = GameObject::GetGameObject(*me, m_Instance->GetData64(eHighmaulGameobjects::GateArenaInner)))
                                    l_InnerGate->SetGoState(GOState::GO_STATE_ACTIVE);
                            }
                        });

                        AddTimedDelayedOperation(55 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            Talk(eTalks::Intro4);

                            if (m_Instance != nullptr)
                            {
                                if (Creature* l_Vulgor = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Vulgor)))
                                    l_Vulgor->AI()->DoAction(eActions::StartIntro);
                            }
                        });

                        AddTimedDelayedOperation(59 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            if (m_Instance != nullptr)
                            {
                                if (GameObject* l_InnerGate = GameObject::GetGameObject(*me, m_Instance->GetData64(eHighmaulGameobjects::GateArenaInner)))
                                    l_InnerGate->SetGoState(GOState::GO_STATE_READY);
                            }
                        });

                        AddTimedDelayedOperation(70 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro5); });
                        break;
                    }
                    case eActions::ContinueIntro:
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Trash1); });
                        AddTimedDelayedOperation(11 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Trash2); });
                        break;
                    case eActions::VulgorDied:
                    {
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Kargath1); });

                        AddTimedDelayedOperation(11 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            if (Creature* l_Kargath = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::KargathBladefist)))
                                l_Kargath->AI()->DoAction(eActions::VulgorDied);
                        });

                        AddTimedDelayedOperation(20 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Kargath2); });

                        AddTimedDelayedOperation(21 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            if (Creature* l_Kargath = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::KargathBladefist)))
                                l_Kargath->SetFacingTo(4.02f);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_jhorn_the_madAI(p_Creature);
        }
};

/// Thoktar Ironskull - 83378
class npc_highmaul_thoktar_ironskull : public CreatureScript
{
    public:
        npc_highmaul_thoktar_ironskull() : CreatureScript("npc_highmaul_thoktar_ironskull") { }

        enum eTalks
        {
            Intro1,
            Intro2,
            Intro3,
            Trash1,
            Trash2,
            Kargath1
        };

        enum eActions
        {
            StartIntro,
            ContinueIntro,
            VulgorDied
        };

        struct npc_highmaul_thoktar_ironskullAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_thoktar_ironskullAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::StartIntro:
                        AddTimedDelayedOperation(11 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro1); });
                        AddTimedDelayedOperation(31 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro2); });
                        AddTimedDelayedOperation(50 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro3); });
                        break;
                    case eActions::ContinueIntro:
                        AddTimedDelayedOperation(6 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Trash1); });
                        AddTimedDelayedOperation(17 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Trash2); });
                        break;
                    case eActions::VulgorDied:
                        AddTimedDelayedOperation(6 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Kargath1); });
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_thoktar_ironskullAI(p_Creature);
        }
};

/// Imperator Mar'gok <Sorcerer King> (Only cosmetic) - 83268
class npc_highmaul_imperator_margok : public CreatureScript
{
    public:
        npc_highmaul_imperator_margok() : CreatureScript("npc_highmaul_imperator_margok") { }

        enum eTalks
        {
            SorckingEvent12,
            SorckingEvent13
        };

        enum eActions
        {
            VulgorDied = 2,
            KargathLastTalk
        };

        enum eMove
        {
            MoveFrontGate = 1
        };

        enum eSpells
        {
            TeleportIntoArena   = 167048,
            TeleportVisual      = 167050,
            SitThrone           = 88648
        };

        struct npc_highmaul_imperator_margokAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_imperator_margokAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::VulgorDied:
                    {
                        /// Teleport in Coliseum
                        AddTimedDelayedOperation(19 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->CastSpell(me, eSpells::TeleportIntoArena, true);
                            me->NearTeleportTo(g_MargokTeleport);
                            me->CastSpell(me, eSpells::TeleportVisual, true);
                        });

                        AddTimedDelayedOperation(20 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->SetFacingTo(g_MargokTeleport.m_orientation);
                            me->RemoveAura(eSpells::TeleportIntoArena);
                        });

                        AddTimedDelayedOperation(28 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::SorckingEvent12); });

                        AddTimedDelayedOperation(49 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            if (m_Instance == nullptr)
                                return;

                            if (Creature* l_Kargath = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::KargathBladefist)))
                            {
                                l_Kargath->SetWalk(true);
                                l_Kargath->GetMotionMaster()->MovePoint(eMove::MoveFrontGate, g_KargathPos);
                            }
                        });

                        AddTimedDelayedOperation(39 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            if (m_Instance == nullptr)
                                return;

                            if (Creature* l_Kargath = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::KargathBladefist)))
                                l_Kargath->AI()->DoAction(eActions::KargathLastTalk);
                        });

                        AddTimedDelayedOperation(51 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::SorckingEvent13); });
                        AddTimedDelayedOperation(52 * TimeConstants::IN_MILLISECONDS, [this]() -> void { me->CastSpell(me, eSpells::SitThrone, true); });
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_imperator_margokAI(p_Creature);
        }
};

/// Gorian Guardsman - 81270
class npc_highmaul_gorian_guardsman : public CreatureScript
{
    public:
        npc_highmaul_gorian_guardsman() : CreatureScript("npc_highmaul_gorian_guardsman") { }

        enum eSpells
        {
            SpellBloodyCleave   = 166767,

            ChainGripSearcher   = 151990,
            ChainGripAura       = 152024,
            ViciousSlash        = 152043,

            SpellStaggeringBlow = 166779
        };

        enum eEvents
        {
            EventBloodyCleave   = 1,
            EventChainGrip,
            EventStaggeringBlow
        };

        struct npc_highmaul_gorian_guardsmanAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_gorian_guardsmanAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventBloodyCleave, urand(4000, 7000));
                m_Events.ScheduleEvent(eEvents::EventChainGrip, urand(3000, 8000));
                m_Events.ScheduleEvent(eEvents::EventStaggeringBlow, urand(6000, 10000));
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::ChainGripSearcher:
                    {
                        uint64 l_Guid = p_Target->GetGUID();
                        me->CastSpell(p_Target, eSpells::ChainGripAura, true);

                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this, l_Guid]() -> void
                        {
                            if (Unit* l_Target = Unit::GetUnit(*me, l_Guid))
                                me->CastSpell(l_Target, eSpells::ViciousSlash, true);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventBloodyCleave:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellBloodyCleave, true);
                        m_Events.ScheduleEvent(eEvents::EventBloodyCleave, urand(10000, 15000));
                        break;
                    case eEvents::EventChainGrip:
                        me->CastSpell(me, eSpells::ChainGripSearcher, true);
                        m_Events.ScheduleEvent(eEvents::EventChainGrip, urand(8000, 12000));
                        break;
                    case eEvents::EventStaggeringBlow:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellStaggeringBlow, true);
                        m_Events.ScheduleEvent(eEvents::EventStaggeringBlow, urand(15000, 20000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_gorian_guardsmanAI(p_Creature);
        }
};

/// Night-Twisted Devout - 82698
class npc_highmaul_night_twisted_devout : public CreatureScript
{
    public:
        npc_highmaul_night_twisted_devout() : CreatureScript("npc_highmaul_night_twisted_devout") { }

        enum eSpells
        {
            SpellTaintedClaws   = 175601,

            SpellDevouringLeap  = 175598,
            SpellDevour         = 175599
        };

        enum eEvents
        {
            EventTaintedClaws = 1,
            EventDevour
        };

        struct npc_highmaul_night_twisted_devoutAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_night_twisted_devoutAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                if (me->HasFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC) ||
                    me->HasFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE))
                    me->SetReactState(ReactStates::REACT_PASSIVE);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                if (me->HasReactState(ReactStates::REACT_PASSIVE))
                {
                    EnterEvadeMode();
                    return;
                }

                m_Events.ScheduleEvent(eEvents::EventTaintedClaws, urand(6000, 9000));
                m_Events.ScheduleEvent(eEvents::EventDevour, urand(8000, 10000));

                if (Creature* l_IronGrunt = me->FindNearestCreature(eHighmaulCreatures::IronGrunt, 3.0f))
                {
                    me->Kill(l_IronGrunt);
                    DoZoneInCombat(me, 40.0f);
                }

                me->SetAIAnimKitId(0);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventTaintedClaws:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellTaintedClaws, true);
                        m_Events.ScheduleEvent(eEvents::EventTaintedClaws, urand(8000, 11000));
                        break;
                    }
                    case eEvents::EventDevour:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                        {
                            me->CastSpell(l_Target, eSpells::SpellDevouringLeap, true);

                            uint64 l_Guid = l_Target->GetGUID();
                            AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this, l_Guid]() -> void
                            {
                                if (Unit* l_Target = Unit::GetUnit(*me, l_Guid))
                                    me->CastSpell(l_Target, eSpells::SpellDevour, false);
                            });
                        }

                        m_Events.ScheduleEvent(eEvents::EventDevour, urand(8000, 11000));
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_night_twisted_devoutAI(p_Creature);
        }
};

/// Gorian Runemaster - 81272
class npc_highmaul_gorian_runemaster : public CreatureScript
{
    public:
        npc_highmaul_gorian_runemaster() : CreatureScript("npc_highmaul_gorian_runemaster") { }

        enum eSpells
        {
            SpellRuneOfDestruction      = 175636,

            SpellRuneOfDisintegration   = 175648,

            SpellRuneOfUnmaking         = 175899
        };

        enum eEvents
        {
            EventRuneOfDestruction = 1,
            EventRuneOfDisintegration,
            EventRuneOfUnmaking
        };

        struct npc_highmaul_gorian_runemasterAI : public ScriptedAI
        {
            npc_highmaul_gorian_runemasterAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                me->RemoveAllAreasTrigger();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventRuneOfDestruction, urand(6000, 9000));
                m_Events.ScheduleEvent(eEvents::EventRuneOfDisintegration, urand(9000, 12000));
                m_Events.ScheduleEvent(eEvents::EventRuneOfUnmaking, urand(4000, 6000));
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventRuneOfDestruction:
                        me->CastSpell(me, eSpells::SpellRuneOfDestruction, false);
                        m_Events.ScheduleEvent(eEvents::EventRuneOfDestruction, urand(9000, 12000));
                        break;
                    case eEvents::EventRuneOfDisintegration:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::SpellRuneOfDisintegration, false);
                        m_Events.ScheduleEvent(eEvents::EventRuneOfDisintegration, urand(12000, 15000));
                        break;
                    case eEvents::EventRuneOfUnmaking:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellRuneOfUnmaking, false);
                        m_Events.ScheduleEvent(eEvents::EventRuneOfUnmaking, urand(6000, 9000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_gorian_runemasterAI(p_Creature);
        }
};

/// Gorian Enforcer - 82900
/// Gorian Enforcer - 86275
class npc_highmaul_gorian_enforcer : public CreatureScript
{
    public:
        npc_highmaul_gorian_enforcer() : CreatureScript("npc_highmaul_gorian_enforcer") { }

        enum eSpell
        {
            SpellMeatGrinder = 175665
        };

        enum eEvent
        {
            EventMeatGrinder = 1
        };

        struct npc_highmaul_gorian_enforcerAI : public ScriptedAI
        {
            npc_highmaul_gorian_enforcerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvent::EventMeatGrinder, urand(6000, 8000));
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventMeatGrinder:
                        me->CastSpell(me, eSpell::SpellMeatGrinder, false);
                        m_Events.ScheduleEvent(eEvent::EventMeatGrinder, urand(12000, 15000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_gorian_enforcerAI(p_Creature);
        }
};

/// Underbelly Vagrant - 86290
class npc_highmaul_underbelly_vagrant : public CreatureScript
{
    public:
        npc_highmaul_underbelly_vagrant() : CreatureScript("npc_highmaul_underbelly_vagrant") { }

        enum eSpells
        {
            SpellLooting    = 175673,
            SpellPilfer     = 175715
        };

        enum eEvent
        {
            EventPilfer = 1
        };

        struct npc_highmaul_underbelly_vagrantAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_underbelly_vagrantAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void { me->CastSpell(me, eSpells::SpellLooting, false); });
            }

            void LastOperationCalled() override
            {
                AddTimedDelayedOperation(16 * TimeConstants::IN_MILLISECONDS, [this]() -> void { me->CastSpell(me, eSpells::SpellLooting, false); });
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvent::EventPilfer, urand(2000, 4000));

                m_TimedDelayedOperations.clear();
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventPilfer:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::SpellPilfer, false);
                        m_Events.ScheduleEvent(eEvent::EventPilfer, urand(2000, 4000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_underbelly_vagrantAI(p_Creature);
        }
};

/// Gorian Sorcerer - 85225
class npc_highmaul_gorian_sorcerer : public CreatureScript
{
    public:
        npc_highmaul_gorian_sorcerer() : CreatureScript("npc_highmaul_gorian_sorcerer") { }

        enum eSpells
        {
            SpellArcaneForce    = 175848,

            SpellArcaneBolt     = 175879
        };

        enum eEvents
        {
            EventArcaneForce = 1,
            EventArcaneBolt
        };

        struct npc_highmaul_gorian_sorcererAI : public ScriptedAI
        {
            npc_highmaul_gorian_sorcererAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventArcaneForce, urand(6000, 8000));
                m_Events.ScheduleEvent(eEvents::EventArcaneBolt, urand(7000, 10000));
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventArcaneForce:
                        me->CastSpell(me, eSpells::SpellArcaneForce, false);
                        m_Events.ScheduleEvent(eEvents::EventArcaneForce, urand(20000, 25000));
                        break;
                    case eEvents::EventArcaneBolt:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellArcaneBolt, false);
                        m_Events.ScheduleEvent(eEvents::EventArcaneBolt, urand(7000, 10000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_gorian_sorcererAI(p_Creature);
        }
};

/// Night-Twisted Brute - 85241
class npc_highmaul_night_twisted_brute : public CreatureScript
{
    public:
        npc_highmaul_night_twisted_brute() : CreatureScript("npc_highmaul_night_twisted_brute") { }

        enum eSpell
        {
            SpellSurgeOfDarkness = 175763
        };

        enum eEvent
        {
            EventSurgeOfDarkness = 1
        };

        struct npc_highmaul_night_twisted_bruteAI : public ScriptedAI
        {
            npc_highmaul_night_twisted_bruteAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvent::EventSurgeOfDarkness, urand(8000, 12000));

                std::list<Creature*> l_IronGrunts;
                me->GetCreatureListWithEntryInGrid(l_IronGrunts, eHighmaulCreatures::IronGrunt, 35.0f);

                if (!l_IronGrunts.empty())
                {
                    std::list<Unit*> l_Allies;

                    JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(me, me, 50.0f);
                    JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(me, l_Allies, l_Check);
                    me->VisitNearbyObject(50.0f, l_Searcher);

                    for (Unit* l_Unit : l_Allies)
                    {
                        if (l_Unit->ToCreature() != nullptr && l_Unit->ToCreature()->AI())
                            l_Unit->ToCreature()->AI()->AttackStart(p_Attacker);
                    }
                }

                for (Creature* l_Creature : l_IronGrunts)
                    me->Kill(l_Creature);

                me->SetAIAnimKitId(0);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventSurgeOfDarkness:
                        me->CastSpell(me, eSpell::SpellSurgeOfDarkness, false);
                        m_Events.ScheduleEvent(eEvent::EventSurgeOfDarkness, urand(8000, 11000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_night_twisted_bruteAI(p_Creature);
        }
};

/// Night-Twisted Soothsayer - 85240
class npc_highmaul_night_twisted_soothsayer : public CreatureScript
{
    public:
        npc_highmaul_night_twisted_soothsayer() : CreatureScript("npc_highmaul_night_twisted_soothsayer") { }

        enum eSpells
        {
            SpellVoidStorm  = 167039,

            SpellVoidBolt   = 175876
        };

        enum eEvents
        {
            EventVoidStorm = 1,
            EventVoidBolt
        };

        struct npc_highmaul_night_twisted_soothsayerAI : public ScriptedAI
        {
            npc_highmaul_night_twisted_soothsayerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventVoidStorm, urand(6000, 9000));
                m_Events.ScheduleEvent(eEvents::EventVoidBolt, urand(4000, 7000));
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventVoidStorm:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::SpellVoidStorm, false);
                        m_Events.ScheduleEvent(eEvents::EventVoidStorm, urand(9000, 12000));
                        break;
                    case eEvents::EventVoidBolt:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellVoidBolt, false);
                        m_Events.ScheduleEvent(eEvents::EventVoidBolt, urand(8000, 11000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_night_twisted_soothsayerAI(p_Creature);
        }
};

/// Void Aberration - 85243
class npc_highmaul_void_aberration : public CreatureScript
{
    public:
        npc_highmaul_void_aberration() : CreatureScript("npc_highmaul_void_aberration") { }

        enum eSpell
        {
            SpellVoidSoul = 175816
        };

        enum eEvent
        {
            EventVoidSoul = 1
        };

        struct npc_highmaul_void_aberrationAI : public ScriptedAI
        {
            npc_highmaul_void_aberrationAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvent::EventVoidSoul, urand(8000, 12000));
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventVoidSoul:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpell::SpellVoidSoul, true);
                        m_Events.ScheduleEvent(eEvent::EventVoidSoul, urand(8000, 11000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_void_aberrationAI(p_Creature);
        }
};

/// Krush - 82532
class npc_highmaul_krush : public CreatureScript
{
    public:
        npc_highmaul_krush() : CreatureScript("npc_highmaul_krush") { }

        enum eSpells
        {
            SpellBoarsRushSearcher  = 166224,
            SpellBoarsRushMissile   = 166226
        };

        enum eEvent
        {
            EventBoarsRush = 1
        };

        struct npc_highmaul_krushAI : public ScriptedAI
        {
            npc_highmaul_krushAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvent::EventBoarsRush, 5000);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::SpellBoarsRushSearcher:
                        me->SetFacingToObject(p_Target);
                        me->CastSpell(p_Target, eSpells::SpellBoarsRushMissile, true);
                        break;
                    default:
                        break;
                }
            }

            void JustReachedHome() override
            {
                me->GetMotionMaster()->MoveIdle();
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventBoarsRush:
                        me->CastSpell(me, eSpells::SpellBoarsRushSearcher, true);
                        me->AddUnitState(UnitState::UNIT_STATE_ROOT);
                        m_Events.ScheduleEvent(eEvent::EventBoarsRush, 25000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_krushAI(p_Creature);
        }
};

/// Iron Flame Technician - 86607
class npc_highmaul_iron_flame_technician : public CreatureScript
{
    public:
        npc_highmaul_iron_flame_technician() : CreatureScript("npc_highmaul_iron_flame_technician") { }

        enum eSpells
        {
            SpellCorruptedBlood             = 174475,
            SpellFlamethrower               = 173231,

            SpellUnstoppableChargeSearcher  = 174462,
            UnstoppableChargeCharge         = 174461,
            UnstoppableChargeDmg            = 174465
        };

        enum eEvents
        {
            EventCorruptedBlood = 1,
            EventFlamethrower,
            EventUnstoppableCharge
        };

        enum eCreature
        {
            UnstoppableCharge = 87230
        };

        enum eAction
        {
            DoIntro
        };

        struct npc_highmaul_iron_flame_technicianAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_iron_flame_technicianAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_IsCosmetic = false;
            }

            EventMap m_Events;
            EventMap m_CosmeticEvent;

            uint64 m_ChargeTarget;

            /// For Brackenspore event
            bool m_IsCosmetic;

            void Reset() override
            {
                m_Events.Reset();

                m_ChargeTarget = 0;

                AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (Creature* l_Boss = me->FindNearestCreature(eHighmaulCreatures::Brackenspore, 50.0f))
                    {
                        m_IsCosmetic = true;
                        m_CosmeticEvent.ScheduleEvent(eEvents::EventFlamethrower, urand(4000, 7000));
                    }
                });
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                if (m_IsCosmetic)
                    return;

                m_Events.ScheduleEvent(eEvents::EventCorruptedBlood, urand(6000, 9000));
                m_Events.ScheduleEvent(eEvents::EventFlamethrower, urand(4000, 7000));
                m_Events.ScheduleEvent(eEvents::EventUnstoppableCharge, urand(9000, 12000));
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eAction::DoIntro)
                {
                    m_Events.Reset();
                    m_CosmeticEvent.Reset();
                }
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::SpellUnstoppableChargeSearcher:
                    {
                        if (Creature* l_Charge = me->SummonCreature(eCreature::UnstoppableCharge, *p_Target, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 2000))
                        {
                            l_Charge->SetReactState(ReactStates::REACT_PASSIVE);
                            l_Charge->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                            m_ChargeTarget = l_Charge->GetGUID();
                            me->CastSpell(l_Charge, eSpells::UnstoppableChargeCharge, true);
                            me->CastSpell(l_Charge, eSpells::UnstoppableChargeDmg, true);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            uint64 GetGUID(int32 /*p_ID*/) override
            {
                return m_ChargeTarget;
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                if (p_ID == 0)
                    me->DespawnOrUnsummon();
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                {
                    m_CosmeticEvent.Update(p_Diff);

                    if (m_CosmeticEvent.ExecuteEvent() == eEvents::EventFlamethrower)
                    {
                        me->CastSpell(me, eSpells::SpellFlamethrower, false);
                        m_CosmeticEvent.ScheduleEvent(eEvents::EventFlamethrower, urand(4000, 7000));
                    }
                }

                if (!UpdateVictim() || m_IsCosmetic)
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventCorruptedBlood:
                        me->CastSpell(me, eSpells::SpellCorruptedBlood, false);
                        m_Events.ScheduleEvent(eEvents::EventCorruptedBlood, urand(13000, 16000));
                        break;
                    case eEvents::EventFlamethrower:
                        me->CastSpell(me, eSpells::SpellFlamethrower, false);
                        m_Events.ScheduleEvent(eEvents::EventFlamethrower, urand(14000, 17000));
                        break;
                    case eEvents::EventUnstoppableCharge:
                        me->CastSpell(me, eSpells::SpellUnstoppableChargeSearcher, false);
                        m_Events.ScheduleEvent(eEvents::EventUnstoppableCharge, urand(19000, 22000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_iron_flame_technicianAI(p_Creature);
        }
};

/// Iron Warmaster - 86609
class npc_highmaul_iron_warmaster : public CreatureScript
{
    public:
        npc_highmaul_iron_warmaster() : CreatureScript("npc_highmaul_iron_warmaster") { }

        enum eSpells
        {
            SpellIronBattleRage = 173238,
            SpellCorruptedBlood = 174475
        };

        enum eEvents
        {
            EventIronBattleRage = 1,
            EventCorruptedBlood
        };

        enum eTalks
        {
            Intro1,
            Intro2
        };

        enum eAction
        {
            DoIntro
        };

        struct npc_highmaul_iron_warmasterAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_iron_warmasterAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_IsCosmetic = false;
                m_IntroDone = false;
            }

            EventMap m_Events;

            /// For Brackenspore event
            bool m_IsCosmetic;
            bool m_IntroDone;

            void Reset() override
            {
                m_Events.Reset();

                AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (Creature* l_Boss = me->FindNearestCreature(eHighmaulCreatures::Brackenspore, 50.0f))
                        m_IsCosmetic = true;
                });
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                if (m_IsCosmetic)
                    return;

                m_Events.ScheduleEvent(eEvents::EventIronBattleRage, urand(6000, 9000));
                m_Events.ScheduleEvent(eEvents::EventCorruptedBlood, urand(6000, 9000));
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eAction::DoIntro && !m_IntroDone)
                {
                    m_IntroDone = true;
                    AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro1); });

                    AddTimedDelayedOperation(6 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        Talk(eTalks::Intro2);

                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);
                        me->GetMotionMaster()->MovePoint(0, g_IronWarmasterPos);
                    });
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                if (p_ID == 0)
                    me->GetMotionMaster()->MoveJump(g_IronWarmasterJump, 10.0f, 30.0f, 1);
                else
                    me->DespawnOrUnsummon();
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim() || m_IsCosmetic)
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventIronBattleRage:
                        me->CastSpell(me, eSpells::SpellIronBattleRage, true);
                        m_Events.ScheduleEvent(eEvents::EventIronBattleRage, urand(12000, 15000));
                        break;
                    case eEvents::EventCorruptedBlood:
                        me->CastSpell(me, eSpells::SpellCorruptedBlood, false);
                        m_Events.ScheduleEvent(eEvents::EventCorruptedBlood, urand(13000, 16000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_iron_warmasterAI(p_Creature);
        }
};

/// Iron Blood Mage - 87229
class npc_highmaul_iron_blood_mage : public CreatureScript
{
    public:
        npc_highmaul_iron_blood_mage() : CreatureScript("npc_highmaul_iron_blood_mage") { }

        enum eSpells
        {
            SpellCorruptedBlood         = 174475,
            SpellBloodBolt              = 174574,
            SpellCorruptedBloodShield   = 174474
        };

        enum eEvents
        {
            EventCorruptedBlood = 1,
            EventBloodBolt,
            EventCorruptedBloodShield,
            EventCheckForIntro
        };

        enum eCreatures
        {
            IronFlameTechnician = 86607,
            IronWarmaster       = 86609
        };

        enum eAction
        {
            DoIntro
        };

        struct npc_highmaul_iron_blood_mageAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_iron_blood_mageAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    std::list<Creature*> l_CreatureList;
                    me->GetCreatureListInGrid(l_CreatureList, 30.0f);

                    if (l_CreatureList.empty())
                        return;

                    l_CreatureList.remove_if([this](Creature* p_Creature) -> bool
                    {
                        if (p_Creature == nullptr)
                            return true;

                        if (p_Creature->GetEntry() != me->GetEntry() &&
                            p_Creature->GetEntry() != eCreatures::IronFlameTechnician &&
                            p_Creature->GetEntry() != eCreatures::IronWarmaster)
                            return true;

                        return false;
                    });

                    uint8 l_Count = 0;
                    for (Creature* l_Creature : l_CreatureList)
                    {
                        if (Creature* l_Boss = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Brackenspore)))
                        {
                            if (l_Boss->GetAI())
                                l_Boss->AI()->SetGUID(l_Creature->GetGUID(), l_Count);
                        }
                    }
                });
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventCorruptedBlood, urand(6000, 9000));
                m_Events.ScheduleEvent(eEvents::EventBloodBolt, urand(4000, 7000));
                m_Events.ScheduleEvent(eEvents::EventCorruptedBloodShield, urand(8000, 11000));
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventCorruptedBlood:
                        me->CastSpell(me, eSpells::SpellCorruptedBlood, false);
                        m_Events.ScheduleEvent(eEvents::EventCorruptedBlood, urand(13000, 16000));
                        break;
                    case eEvents::EventBloodBolt:
                        me->CastSpell(me, eSpells::SpellBloodBolt, false);
                        m_Events.ScheduleEvent(eEvents::EventBloodBolt, urand(8000, 11000));
                        break;
                    case eEvents::EventCorruptedBloodShield:
                        if (Unit* l_Target = me->SelectNearbyAlly(me, 15.0f, true))
                            me->CastSpell(l_Target, eSpells::SpellCorruptedBloodShield, false);
                        m_Events.ScheduleEvent(eEvents::EventCorruptedBloodShield, urand(15000, 18000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_iron_blood_mageAI(p_Creature);
        }
};

/// Night-Twisted Ritualist - 85245
class npc_highmaul_night_twisted_ritualist : public CreatureScript
{
    public:
        npc_highmaul_night_twisted_ritualist() : CreatureScript("npc_highmaul_night_twisted_ritualist") { }

        enum eSpells
        {
            VoidChannel = 170677,
            VoidTouch   = 175581
        };

        enum eEvent
        {
            EventVoidTouch = 1
        };

        enum eCreature
        {
            GreaterAberration = 85246
        };

        enum eAction
        {
            RitualistDied
        };

        struct npc_highmaul_night_twisted_ritualistAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_night_twisted_ritualistAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_Aberration = 0;
            }

            InstanceScript* m_Instance;
            EventMap m_Events;

            uint64 m_Aberration;

            void Reset() override
            {
                m_Events.Reset();

                AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (Creature* l_Aberration = me->FindNearestCreature(eCreature::GreaterAberration, 20.0f))
                        m_Aberration = l_Aberration->GetGUID();
                });

                me->CastSpell(me, eSpells::VoidChannel, true);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->RemoveAura(eSpells::VoidChannel);

                m_Events.ScheduleEvent(eEvent::EventVoidTouch, urand(3000, 6000));
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (Creature* l_Aberration = Creature::GetCreature(*me, m_Aberration))
                    l_Aberration->AI()->DoAction(eAction::RitualistDied);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventVoidTouch:
                        me->CastSpell(me, eSpells::VoidTouch, false);
                        m_Events.ScheduleEvent(eEvent::EventVoidTouch, urand(6000, 9000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_night_twisted_ritualistAI(p_Creature);
        }
};

/// Greater Void Aberration - 85246
class npc_highmaul_greater_void_aberration : public CreatureScript
{
    public:
        npc_highmaul_greater_void_aberration() : CreatureScript("npc_highmaul_greater_void_aberration") { }

        enum eSpells
        {
            VoidCommunion   = 175539,
            CallOfTheVoid   = 175589
        };

        enum eEvent
        {
            EventCallOfTheVoid = 1
        };

        enum eCreature
        {
            NightTwistedRitualist = 85245
        };

        enum eAction
        {
            RitualistDied
        };

        struct npc_highmaul_greater_void_aberrationAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_greater_void_aberrationAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_Ritualists = 0;
            }

            InstanceScript* m_Instance;
            EventMap m_Events;

            uint32 m_Ritualists;

            void Reset() override
            {
                m_Events.Reset();

                AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    std::list<Creature*> l_RitualistList;
                    me->GetCreatureListWithEntryInGrid(l_RitualistList, eCreature::NightTwistedRitualist, 20.0f);
                    m_Ritualists = (uint32)l_RitualistList.size();

                    if (m_Ritualists)
                        me->CastSpell(me, eSpells::VoidCommunion, true);
                });
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvent::EventCallOfTheVoid, urand(6000, 9000));
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eAction::RitualistDied && m_Ritualists)
                {
                    --m_Ritualists;

                    if (!m_Ritualists)
                        me->RemoveAura(eSpells::VoidCommunion);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventCallOfTheVoid:
                        me->CastSpell(me, eSpells::CallOfTheVoid, false);
                        m_Events.ScheduleEvent(eEvent::EventCallOfTheVoid, urand(9000, 12000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_greater_void_aberrationAI(p_Creature);
        }
};

/// Highmaul Conscript - 82519
class npc_highmaul_highmaul_conscript : public CreatureScript
{
    public:
        npc_highmaul_highmaul_conscript() : CreatureScript("npc_highmaul_highmaul_conscript") { }

        enum eSpells
        {
            RendingSlash    = 166185,
            ShieldBlocking  = 166177,
            ShieldCharge    = 166178,
            AtArms          = 157739
        };

        enum eEvents
        {
            EventRendingSlash = 1,
            EventShieldBlocking,
            EventShieldCharge
        };

        struct npc_highmaul_highmaul_conscriptAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_highmaul_conscriptAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;

            uint64 m_ChargeTarget;

            void Reset() override
            {
                m_Events.Reset();

                m_ChargeTarget = 0;

                me->ClearUnitState(UnitState::UNIT_STATE_ROOT);
                me->CastSpell(me, eSpells::AtArms, true);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventRendingSlash, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventShieldBlocking, 13 * TimeConstants::IN_MILLISECONDS);

                me->RemoveAura(eSpells::AtArms);
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_ID) override
            {
                if (p_ID != EVENT_CHARGE)
                    return;

                me->RemoveAura(eSpells::AtArms);
                me->RemoveAura(eSpells::ShieldCharge);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Phemos = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Phemos)))
                {
                    if (l_Phemos->IsAIEnabled)
                    {
                        l_Phemos->AI()->SetGUID(me->GetGUID(), 0);
                        l_Phemos->AI()->DoAction(0);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventRendingSlash:
                    {
                        me->CastSpell(me, eSpells::RendingSlash, false);
                        m_Events.ScheduleEvent(eEvents::EventRendingSlash, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventShieldBlocking:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                        {
                            m_ChargeTarget = l_Target->GetGUID();
                            me->SetFacingTo(me->GetAngle(l_Target));
                            me->CastSpell(l_Target, eSpells::ShieldBlocking, false);
                            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                            me->AddUnitState(UnitState::UNIT_STATE_ROOT);
                        }

                        AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->CastSpell(me, eSpells::AtArms, true);
                        });

                        m_Events.ScheduleEvent(eEvents::EventShieldCharge, 6 * TimeConstants::IN_MILLISECONDS);
                        m_Events.ScheduleEvent(eEvents::EventShieldBlocking, 60 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventShieldCharge:
                    {
                        if (Unit* l_Target = Unit::GetUnit(*me, m_ChargeTarget))
                        {
                            float l_O = me->GetOrientation();
                            Position l_Pos;

                            me->GetContactPoint(l_Target, l_Pos.m_positionX, l_Pos.m_positionY, l_Pos.m_positionZ);
                            l_Target->GetFirstCollisionPosition(l_Pos, l_Target->GetObjectSize(), l_O);
                            me->ClearUnitState(UnitState::UNIT_STATE_ROOT);
                            me->GetMotionMaster()->MoveCharge(l_Pos.m_positionX, l_Pos.m_positionY, l_Pos.m_positionZ + l_Target->GetObjectSize());

                            me->CastSpell(me, eSpells::ShieldCharge, true);
                        }

                        break;
                    }
                    default:
                        break;
                }

                if (!me->HasAura(eSpells::ShieldBlocking))
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_highmaul_conscriptAI(p_Creature);
        }
};

/// Ogron Earthshaker - 82399
class npc_highmaul_ogron_earthshaker : public CreatureScript
{
    public:
        npc_highmaul_ogron_earthshaker() : CreatureScript("npc_highmaul_ogron_earthshaker") { }

        enum eSpells
        {
            IntimidatingRoarJump    = 166170,
            EarthdevastatingSlam    = 166174,
            EarthdevastatingSlamDmg = 166175
        };

        enum eEvents
        {
            EventIntimidatingRoar = 1,
            EventEarthdevastatingSlam
        };

        enum eAction
        {
            ActionSlam
        };

        struct npc_highmaul_ogron_earthshakerAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_ogron_earthshakerAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;

            float m_Orientation;
            uint8 m_SlamCount;

            void Reset() override
            {
                m_Events.Reset();

                m_Orientation = 0.0f;
                m_SlamCount = 0;
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventIntimidatingRoar, 6 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventEarthdevastatingSlam, 17 * TimeConstants::IN_MILLISECONDS);
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eAction::ActionSlam)
                {
                    me->SetFacingTo(m_Orientation);
                    me->CastSpell(me, eSpells::EarthdevastatingSlamDmg, false);

                    m_Orientation += M_PI / 3;
                    ++m_SlamCount;

                    if (m_SlamCount >= 6)
                    {
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                        });
                    }
                }
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Phemos = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Phemos)))
                {
                    if (l_Phemos->IsAIEnabled)
                    {
                        l_Phemos->AI()->SetGUID(me->GetGUID(), 0);
                        l_Phemos->AI()->DoAction(0);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventIntimidatingRoar:
                    {
                        me->CastSpell(me, eSpells::IntimidatingRoarJump, true);
                        m_Events.ScheduleEvent(eEvents::EventIntimidatingRoar, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventEarthdevastatingSlam:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 15.0f))
                        {
                            m_Orientation = me->GetAngle(l_Target);
                            m_SlamCount = 0;
                            me->SetFacingTo(m_Orientation);
                            me->CastSpell(l_Target, eSpells::EarthdevastatingSlam, true);
                            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                        }

                        m_Events.ScheduleEvent(eEvents::EventEarthdevastatingSlam, 60 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                if (!me->HasAura(eSpells::EarthdevastatingSlam))
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_ogron_earthshakerAI(p_Creature);
        }
};

/// Gorian Arcanist - 82528
class npc_highmaul_gorian_arcanist : public CreatureScript
{
    public:
        npc_highmaul_gorian_arcanist() : CreatureScript("npc_highmaul_gorian_arcanist") { }

        enum eSpells
        {
            ArcaneForceCosmetic     = 166289,

            ArcaneBolt              = 166204,
            ArcaneVolatility        = 166199,
            ArcaneVolatilityAura    = 166200,
            ArcaneVolatilityDmg     = 166202,
            ArcaneVolatilityBump    = 166201,
            ArcaneBarrage           = 178023
        };

        enum eEvents
        {
            EventArcaneBolt = 1,
            EventArcaneVolatility,
            EventArcaneBarrage
        };

        enum eCreature
        {
            InvisibleStalker = 15214
        };

        struct npc_highmaul_gorian_arcanistAI : public ScriptedAI
        {
            npc_highmaul_gorian_arcanistAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                if (Creature* l_Stalker = me->FindNearestCreature(eCreature::InvisibleStalker, 20.0f))
                    me->CastSpell(l_Stalker, eSpells::ArcaneForceCosmetic, false);

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISARMED);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventArcaneBolt, 6 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventArcaneVolatility, 10 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventArcaneBarrage, 8 * TimeConstants::IN_MILLISECONDS);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::ArcaneVolatility:
                        me->CastSpell(p_Target, eSpells::ArcaneVolatilityAura, true);
                        break;
                    case eSpells::ArcaneVolatilityDmg:
                        me->CastSpell(p_Target, eSpells::ArcaneVolatilityBump, true);
                        break;
                    default:
                        break;
                }
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Phemos = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Phemos)))
                {
                    if (l_Phemos->IsAIEnabled)
                    {
                        l_Phemos->AI()->SetGUID(me->GetGUID(), 0);
                        l_Phemos->AI()->DoAction(0);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventArcaneBolt:
                        me->CastSpell(me, eSpells::ArcaneBolt, false);
                        m_Events.ScheduleEvent(eEvents::EventArcaneBolt, 28 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventArcaneVolatility:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::ArcaneVolatility, false);
                        m_Events.ScheduleEvent(eEvents::EventArcaneVolatility, 35 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventArcaneBarrage:
                        me->CastSpell(me, eSpells::ArcaneBarrage, false);
                        m_Events.ScheduleEvent(eEvents::EventArcaneBarrage, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_gorian_arcanistAI(p_Creature);
        }
};

/// Ogron Brute - 82400
class npc_highmaul_ogron_brute : public CreatureScript
{
    public:
        npc_highmaul_ogron_brute() : CreatureScript("npc_highmaul_ogron_brute") { }

        enum eSpell
        {
            Decimate = 166189
        };

        enum eEvent
        {
            EventDecimate = 1
        };

        struct npc_highmaul_ogron_bruteAI : public ScriptedAI
        {
            npc_highmaul_ogron_bruteAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvent::EventDecimate, 6 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Phemos = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Phemos)))
                {
                    if (l_Phemos->IsAIEnabled)
                    {
                        l_Phemos->AI()->SetGUID(me->GetGUID(), 0);
                        l_Phemos->AI()->DoAction(0);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventDecimate:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpell::Decimate, false);
                        m_Events.ScheduleEvent(eEvent::EventDecimate, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_ogron_bruteAI(p_Creature);
        }
};

/// Gorian Rune-Mender - 87910
class npc_highmaul_gorian_rune_mender : public CreatureScript
{
    public:
        npc_highmaul_gorian_rune_mender() : CreatureScript("npc_highmaul_gorian_rune_mender") { }

        enum eSpells
        {
            AttackPale  = 175491,
            Ready2HL    = 175155
        };

        struct npc_highmaul_gorian_rune_menderAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_gorian_rune_menderAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            void Reset() override
            {
                me->RemoveAura(eSpells::AttackPale);
                me->CastSpell(me, eSpells::Ready2HL, true);

                ScheduleNextOperation();
            }

            void LastOperationCalled() override
            {
                ScheduleNextOperation();
            }

            void ScheduleNextOperation()
            {
                AddTimedDelayedOperation(urand(2 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS), [this]() -> void
                {
                    if (urand(0, 1))
                    {
                        me->RemoveAura(eSpells::Ready2HL);
                        me->CastSpell(me, eSpells::AttackPale, true);
                    }
                    else
                    {
                        me->RemoveAura(eSpells::AttackPale);
                        me->CastSpell(me, eSpells::Ready2HL, true);
                    }
                });
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_gorian_rune_menderAI(p_Creature);
        }
};

/// Night-Twisted Fanatic - 87768
class npc_highmaul_night_twisted_fanatic : public CreatureScript
{
    public:
        npc_highmaul_night_twisted_fanatic() : CreatureScript("npc_highmaul_night_twisted_fanatic") { }

        enum eSpells
        {
            Sacrifice       = 175430,
            FullHeal        = 17683,
            ClearAllDebuffs = 34098,
            FadeOut         = 166930
        };

        enum eSteps
        {
            StepSacrifice,
            StepHeal,
            StepReturn
        };

        struct npc_highmaul_night_twisted_fanaticAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_night_twisted_fanaticAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_CosmeticStep = eSteps::StepSacrifice;
            }

            uint8 m_CosmeticStep;

            void Reset() override
            {
                ScheduleNextOperation();
            }

            void LastOperationCalled() override
            {
                ScheduleNextOperation();
            }

            void ScheduleNextOperation()
            {
                uint32 l_Time = 0;
                switch (m_CosmeticStep)
                {
                    default:
                    case eSteps::StepSacrifice:
                        l_Time = urand(5 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eSteps::StepHeal:
                    case eSteps::StepReturn:
                        l_Time = 4 * TimeConstants::IN_MILLISECONDS;
                        break;
                }

                AddTimedDelayedOperation(l_Time, [this]() -> void
                {
                    switch (m_CosmeticStep)
                    {
                        default:
                        case eSteps::StepSacrifice:
                        {
                            m_CosmeticStep = eSteps::StepHeal;
                            me->CastSpell(me, eSpells::Sacrifice, false);
                            break;
                        }
                        case eSteps::StepHeal:
                        {
                            m_CosmeticStep = eSteps::StepReturn;
                            me->CastSpell(me, eSpells::FullHeal, true);
                            me->CastSpell(me, eSpells::ClearAllDebuffs, true);
                            me->CastSpell(me, eSpells::FadeOut, true);
                            break;
                        }
                        case eSteps::StepReturn:
                        {
                            m_CosmeticStep = eSteps::StepSacrifice;
                            me->RemoveAura(eSpells::Sacrifice);
                            me->RemoveAura(eSpells::FadeOut);
                            break;
                        }
                    }
                });
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_night_twisted_fanaticAI(p_Creature);
        }
};

/// Gorian Warden - 87619
class npc_highmaul_gorian_warden : public CreatureScript
{
    public:
        npc_highmaul_gorian_warden() : CreatureScript("npc_highmaul_gorian_warden") { }

        enum eSpells
        {
            AttackPale  = 175491,
            Ready1H     = 175387
        };

        struct npc_highmaul_gorian_wardenAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_gorian_wardenAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            void Reset() override
            {
                me->RemoveAura(eSpells::AttackPale);
                me->CastSpell(me, eSpells::Ready1H, true);

                ScheduleNextOperation();
            }

            void LastOperationCalled() override
            {
                ScheduleNextOperation();
            }

            void ScheduleNextOperation()
            {
                AddTimedDelayedOperation(5 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (urand(0, 1))
                    {
                        me->RemoveAura(eSpells::Ready1H);
                        me->CastSpell(me, eSpells::AttackPale, true);
                    }
                    else
                    {
                        me->RemoveAura(eSpells::AttackPale);
                        me->CastSpell(me, eSpells::Ready1H, true);
                    }
                });
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_gorian_wardenAI(p_Creature);
        }
};

/// Ogron Warbringer - 87589
class npc_highmaul_ogron_warbringer : public CreatureScript
{
    public:
        npc_highmaul_ogron_warbringer() : CreatureScript("npc_highmaul_ogron_warbringer") { }

        enum eSpells
        {
            AttackPale      = 175505,
            ReadyUnarmed    = 173366
        };

        struct npc_highmaul_ogron_warbringerAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_ogron_warbringerAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            void Reset() override
            {
                me->RemoveAura(eSpells::AttackPale);
                me->CastSpell(me, eSpells::ReadyUnarmed, true);

                ScheduleNextOperation();
            }

            void LastOperationCalled() override
            {
                ScheduleNextOperation();
            }

            void ScheduleNextOperation()
            {
                AddTimedDelayedOperation(5 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (urand(0, 1))
                    {
                        me->RemoveAura(eSpells::ReadyUnarmed);
                        me->CastSpell(me, eSpells::AttackPale, true);
                    }
                    else
                    {
                        me->RemoveAura(eSpells::AttackPale);
                        me->CastSpell(me, eSpells::ReadyUnarmed, true);
                    }
                });
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_ogron_warbringerAI(p_Creature);
        }
};

/// Warden Thul'tok <Defender of the Rise> - 81269
class npc_highmaul_warden_thultok : public CreatureScript
{
    public:
        npc_highmaul_warden_thultok() : CreatureScript("npc_highmaul_warden_thultok") { }

        enum eSpells
        {
            DestructiveForce    = 175061,
            ArcaneResidue       = 175047
        };

        enum eEvents
        {
            EventDestructiveForce = 1,
            EventArcaneResidue
        };

        struct npc_highmaul_warden_thultokAI : public ScriptedAI
        {
            npc_highmaul_warden_thultokAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                /// Summon portal in case of this mob is dead at the instance creation
                if (!p_Creature->isAlive())
                    me->SummonGameObject(eHighmaulGameobjects::Teleporter, g_TeleporterSpawnPos, 0.0f, 0.0f, 0.0f, 0.0f, 0);
            }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                me->RemoveAllAreasTrigger();
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                me->RemoveAllAreasTrigger();

                me->SummonGameObject(eHighmaulGameobjects::Teleporter, g_TeleporterSpawnPos, 0.0f, 0.0f, 0.0f, 0.0f, 0);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventDestructiveForce, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventArcaneResidue, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventDestructiveForce:
                        me->CastSpell(me, eSpells::DestructiveForce, false);
                        m_Events.ScheduleEvent(eEvents::EventDestructiveForce, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventArcaneResidue:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::ArcaneResidue, true);
                        m_Events.ScheduleEvent(eEvents::EventArcaneResidue, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_warden_thultokAI(p_Creature);
        }
};

/// Gorian Royal Guardsman <King's Guard> - 81806
class npc_highmaul_gorian_royal_guardsman : public CreatureScript
{
    public:
        npc_highmaul_gorian_royal_guardsman() : CreatureScript("npc_highmaul_gorian_royal_guardsman") { }

        enum eSpells
        {
            /// Pulverize
            PulverizeAura   = 174445,
            PulverizeStack  = 174446,
            PulverizedStun  = 174452,
            /// Rampage
            RampageSearcher = 174468,
            RampageAura     = 174469
        };

        enum eEvent
        {
            EventRampage = 1
        };

        struct npc_highmaul_gorian_royal_guardsmanAI : public ScriptedAI
        {
            npc_highmaul_gorian_royal_guardsmanAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                me->CastSpell(me, eSpells::PulverizeAura, true);

                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvent::EventRampage, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::PulverizeStack:
                    {
                        if (Aura* l_Aura = p_Target->GetAura(p_SpellInfo->Id, me->GetGUID()))
                        {
                            if (l_Aura->GetStackAmount() >= 8)
                            {
                                p_Target->CastSpell(p_Target, eSpells::PulverizedStun, true);
                                p_Target->RemoveAura(p_SpellInfo->Id);
                            }
                        }

                        break;
                    }
                    case eSpells::RampageSearcher:
                    {
                        me->CastSpell(me, eSpells::RampageAura, true);
                        me->SetSpeed(UnitMoveType::MOVE_WALK, 2.0f);
                        me->SetSpeed(UnitMoveType::MOVE_RUN, 2.0f);
                        me->GetMotionMaster()->MovePoint(eSpells::RampageAura, *p_Target);
                        break;
                    }
                    default:
                        break;
                }
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_ID) override
            {
                if (p_ID == eSpells::RampageAura)
                {
                    me->RemoveAura(eSpells::RampageAura);

                    me->SetSpeed(UnitMoveType::MOVE_WALK, 1.0f);
                    me->SetSpeed(UnitMoveType::MOVE_RUN, 1.0f);

                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        me->GetMotionMaster()->MoveChase(l_Target);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventRampage:
                        me->CastSpell(me, eSpells::RampageSearcher, false);
                        m_Events.ScheduleEvent(eEvent::EventRampage, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_gorian_royal_guardsmanAI(p_Creature);
        }
};

/// Gorian High Sorcerer <King's Guard> - 86256
class npc_highmaul_gorian_high_sorcerer : public CreatureScript
{
    public:
        npc_highmaul_gorian_high_sorcerer() : CreatureScript("npc_highmaul_gorian_high_sorcerer") { }

        enum eSpells
        {
            /// Arcane Blast
            ArcaneBlast         = 174442,
            /// Nether Font
            NetherFontSearcher  = 174434,
            NetherFontMissile   = 174435,
            /// Celerity
            Celerity            = 174440
        };

        enum eEvents
        {
            EventArcaneBlast = 1,
            EventNetherFont,
            EventCelerity
        };

        struct npc_highmaul_gorian_high_sorcererAI : public ScriptedAI
        {
            npc_highmaul_gorian_high_sorcererAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventArcaneBlast, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventNetherFont, 10 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventCelerity, 15 * TimeConstants::IN_MILLISECONDS);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::NetherFontSearcher:
                        me->CastSpell(p_Target, eSpells::NetherFontMissile, true);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventArcaneBlast:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::ArcaneBlast, false);
                        m_Events.ScheduleEvent(eEvents::EventArcaneBlast, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventNetherFont:
                        me->CastSpell(me, eSpells::NetherFontSearcher, false);
                        m_Events.ScheduleEvent(eEvents::EventNetherFont, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventCelerity:
                        if (Unit* l_Target = me->SelectNearbyAlly(me, 50.0f, true))
                            me->CastSpell(l_Target, eSpells::Celerity, false);
                        m_Events.ScheduleEvent(eEvents::EventCelerity, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_gorian_high_sorcererAI(p_Creature);
        }
};

/// Ogron Mauler - 81779
class npc_highmaul_ogron_mauler : public CreatureScript
{
    public:
        npc_highmaul_ogron_mauler() : CreatureScript("npc_highmaul_ogron_mauler") { }

        enum eSpell
        {
            DeafeningRoar = 174477
        };

        enum eEvent
        {
            EventDeafeningRoar = 1
        };

        struct npc_highmaul_ogron_maulerAI : public ScriptedAI
        {
            npc_highmaul_ogron_maulerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvent::EventDeafeningRoar, 5 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventDeafeningRoar:
                        me->CastSpell(me, eSpell::DeafeningRoar, false);
                        m_Events.ScheduleEvent(eEvent::EventDeafeningRoar, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_ogron_maulerAI(p_Creature);
        }
};

/// Guard Captain Thag <King's Guard> - 81780
class npc_highmaul_guard_captain_thag : public CreatureScript
{
    public:
        npc_highmaul_guard_captain_thag() : CreatureScript("npc_highmaul_guard_captain_thag") { }

        enum eSpells
        {
            /// Brutal Cleave
            BrutalCleave    = 174491,
            /// Ground Stomp
            GroundStomp     = 174495,
            /// Rending Throw
            RendingThrow    = 174500
        };

        enum eEvents
        {
            EventBrutalCleave = 1,
            EventGroundStomp,
            EventRendingThrow
        };

        enum eAction
        {
            ActionIntro
        };

        struct npc_highmaul_guard_captain_thagAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_guard_captain_thagAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_DoorGuid = 0;
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            uint64 m_DoorGuid;

            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();

                AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (GameObject* l_Door = me->FindNearestGameObject(eHighmaulGameobjects::ThroneRoomDoor, 50.0f))
                    {
                        l_Door->SetGoState(GOState::GO_STATE_READY);
                        m_DoorGuid = l_Door->GetGUID();
                    }
                });

                /// Second equip is a shield
                me->SetCanDualWield(false);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventBrutalCleave, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventGroundStomp, 7 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventRendingThrow, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (GameObject* l_Door = GameObject::GetGameObject(*me, m_DoorGuid))
                    l_Door->SetGoState(GOState::GO_STATE_ACTIVE);

                if (m_Instance != nullptr)
                {
                    Creature* l_CouncilorMalgris = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::HighCouncilorMalgris));
                    Creature* l_ImperatorMargok = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::ImperatorMargok));

                    if (l_CouncilorMalgris && l_ImperatorMargok && l_CouncilorMalgris->IsAIEnabled && l_ImperatorMargok->IsAIEnabled)
                    {
                        l_CouncilorMalgris->AI()->DoAction(eAction::ActionIntro);
                        l_ImperatorMargok->AI()->DoAction(eAction::ActionIntro);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventBrutalCleave:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::BrutalCleave, false);
                        m_Events.ScheduleEvent(eEvents::EventBrutalCleave, 7 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventGroundStomp:
                        me->CastSpell(me, eSpells::GroundStomp, true);
                        m_Events.ScheduleEvent(eEvents::EventGroundStomp, 6 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventRendingThrow:
                        me->CastSpell(me, eSpells::RendingThrow, true);
                        m_Events.ScheduleEvent(eEvents::EventRendingThrow, 6 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_guard_captain_thagAI(p_Creature);
        }
};

/// Councilor Daglat <Gorian War Council> - 81810
class npc_highmaul_councilor_daglat : public CreatureScript
{
    public:
        npc_highmaul_councilor_daglat() : CreatureScript("npc_highmaul_councilor_daglat") { }

        enum eSpells
        {
            KneelCosmeticForced = 130491,
            /// Teleport + Arcane Destruction
            TeleportSearcher    = 174536,
            TeleportMove        = 174538,
            ArcaneDestruction   = 174541
        };

        enum eEvent
        {
            EventArcaneDestruction = 1
        };

        struct npc_highmaul_councilor_daglatAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_councilor_daglatAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                me->CastSpell(me, eSpells::KneelCosmeticForced, true);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvent::EventArcaneDestruction, 1 * TimeConstants::IN_MILLISECONDS);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::TeleportSearcher:
                    {
                        me->CastSpell(*p_Target, eSpells::TeleportMove, true);

                        AddTimedDelayedOperation(200, [this]() -> void
                        {
                            me->CastSpell(me, eSpells::ArcaneDestruction, false);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventArcaneDestruction:
                        me->CastSpell(me, eSpells::TeleportSearcher, true);
                        m_Events.ScheduleEvent(eEvent::EventArcaneDestruction, 6 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_councilor_daglatAI(p_Creature);
        }
};

/// Councilor Magknor <Gorian War Council> - 81808
class npc_highmaul_councilor_magknor : public CreatureScript
{
    public:
        npc_highmaul_councilor_magknor() : CreatureScript("npc_highmaul_councilor_magknor") { }

        enum eSpells
        {
            KneelCosmeticForced     = 130491,
            /// Arcane Torrent
            ArcaneTorrentSummon     = 174549,
            ArcaneTorrentAura       = 174558
        };

        enum eEvent
        {
            EventArcaneTorrent = 1
        };

        struct npc_highmaul_councilor_magknorAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_councilor_magknorAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature), m_Summons(p_Creature) { }

            EventMap m_Events;
            SummonList m_Summons;

            void Reset() override
            {
                m_Events.Reset();

                me->CastSpell(me, eSpells::KneelCosmeticForced, true);

                m_Summons.DespawnAll();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->CastSpell(me, eSpells::ArcaneTorrentSummon, true);

                m_Events.ScheduleEvent(eEvent::EventArcaneTorrent, 1 * TimeConstants::IN_MILLISECONDS);
            }

            void JustSummoned(Creature* p_Summon) override
            {
                m_Summons.Summon(p_Summon);
            }

            void SummonedCreatureDespawn(Creature* p_Summon) override
            {
                m_Summons.Despawn(p_Summon);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventArcaneTorrent:
                        me->CastSpell(me, eSpells::ArcaneTorrentAura, false);
                        m_Events.ScheduleEvent(eEvent::EventArcaneTorrent, 60 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_councilor_magknorAI(p_Creature);
        }
};

/// Arcane Torrent - 87256
class npc_highmaul_arcane_torrent : public CreatureScript
{
    public:
        npc_highmaul_arcane_torrent() : CreatureScript("npc_highmaul_arcane_torrent") { }

        enum eSpell
        {
            ArcaneTorrentMoveSearcher = 174581
        };

        struct npc_highmaul_arcane_torrentAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_arcane_torrentAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            void Reset() override
            {
                SetCanSeeEvenInPassiveMode(true);

                me->CastSpell(me, eSpell::ArcaneTorrentMoveSearcher, true);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpell::ArcaneTorrentMoveSearcher:
                        me->GetMotionMaster()->MovePoint(eSpell::ArcaneTorrentMoveSearcher, *p_Target);
                        break;
                    default:
                        break;
                }
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_ID) override
            {
                if (p_ID == eSpell::ArcaneTorrentMoveSearcher)
                    me->CastSpell(me, eSpell::ArcaneTorrentMoveSearcher, true);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_arcane_torrentAI(p_Creature);
        }
};

/// Councilor Gorluk <Gorian War Council> - 81809
class npc_highmaul_councilor_gorluk : public CreatureScript
{
    public:
        npc_highmaul_councilor_gorluk() : CreatureScript("npc_highmaul_councilor_gorluk") { }

        enum eSpells
        {
            KneelCosmeticForced     = 130491,

            ConjurePhantasmalWeapon = 174608
        };

        enum eEvent
        {
            EventPhantasmalWeapon = 1
        };

        struct npc_highmaul_councilor_gorlukAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_councilor_gorlukAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                me->CastSpell(me, eSpells::KneelCosmeticForced, true);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvent::EventPhantasmalWeapon, 7 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventPhantasmalWeapon:
                        me->CastSpell(me, eSpells::ConjurePhantasmalWeapon, false);
                        m_Events.ScheduleEvent(eEvent::EventPhantasmalWeapon, 7 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_councilor_gorlukAI(p_Creature);
        }
};

/// Phantasmal Weapon - 87293
class npc_highmaul_phantasmal_weapon : public CreatureScript
{
    public:
        npc_highmaul_phantasmal_weapon() : CreatureScript("npc_highmaul_phantasmal_weapon") { }

        enum eSpells
        {
            PhantasmalWeaponsSpawn  = 174605,
            Focused                 = 174719,
            Fixated                 = 174627
        };

        struct npc_highmaul_phantasmal_weaponAI : public ScriptedAI
        {
            npc_highmaul_phantasmal_weaponAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->CastSpell(me, eSpells::PhantasmalWeaponsSpawn, true);
                me->CastSpell(me, eSpells::Focused, true);
                me->CastSpell(me, eSpells::Fixated, true);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::Fixated:
                        AttackStart(p_Target);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_phantasmal_weaponAI(p_Creature);
        }
};

/// Councilor Nouk <Gorian War Council> - 81807
class npc_highmaul_councilor_nouk : public CreatureScript
{
    public:
        npc_highmaul_councilor_nouk() : CreatureScript("npc_highmaul_councilor_nouk") { }

        enum eSpells
        {
            KneelCosmeticForced = 130491,

            TimeStop            = 174939
        };

        enum eEvent
        {
            EventTimeStop = 1
        };

        struct npc_highmaul_councilor_noukAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_councilor_noukAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                me->CastSpell(me, eSpells::KneelCosmeticForced, true);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->CastSpell(me, eSpells::TimeStop, false);

                m_Events.ScheduleEvent(eEvent::EventTimeStop, 4 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventTimeStop:
                        me->CastSpell(me, eSpells::TimeStop, false);
                        m_Events.ScheduleEvent(eEvent::EventTimeStop, 4 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_councilor_noukAI(p_Creature);
        }
};

/// High Councilor Mal'gris <Gorian War Council> - 81811
class npc_highmaul_high_councilor_malgris : public CreatureScript
{
    public:
        npc_highmaul_high_councilor_malgris() : CreatureScript("npc_highmaul_high_councilor_malgris") { }

        enum eSpells
        {
            UnstableTempest     = 174981,
            UnstableTempestAura = 174976
        };

        enum eEvent
        {
            EventUnstableTempest = 1
        };

        enum eTalks
        {
            Intro1,
            Intro2
        };

        enum eActions
        {
            ActionIntro,
            ActionFinishIntro
        };

        struct npc_highmaul_high_councilor_malgrisAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_high_councilor_malgrisAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;

            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvent::EventUnstableTempest, 50);
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::ActionIntro:
                    {
                        Talk(eTalks::Intro1);

                        AddTimedDelayedOperation(15 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            Talk(eTalks::Intro2);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Margok = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::ImperatorMargok)))
                {
                    if (l_Margok->IsAIEnabled)
                        l_Margok->AI()->DoAction(eActions::ActionFinishIntro);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventUnstableTempest:
                    {
                        CustomSpellValues l_Values;
                        if (Aura* l_Aura = me->GetAura(eSpells::UnstableTempestAura))
                            l_Values.AddSpellMod(SpellValueMod::SPELLVALUE_MAX_TARGETS, l_Aura->GetStackAmount());
                        else
                            l_Values.AddSpellMod(SpellValueMod::SPELLVALUE_MAX_TARGETS, 1);

                        me->CastCustomSpell(eSpells::UnstableTempest, l_Values, me, false);
                        m_Events.ScheduleEvent(eEvent::EventUnstableTempest, 50);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_high_councilor_malgrisAI(p_Creature);
        }
};

/// Arena Elevator - 233098
class go_highmaul_arena_elevator : public GameObjectScript
{
    public:
        go_highmaul_arena_elevator() : GameObjectScript("go_highmaul_arena_elevator") { }

        bool OnGameObjectElevatorCheck(GameObject const* /*p_GameObject*/) const override
        {
            return false;
        }
};

/// Instance Portal (Raid: Normal, Heroic, Mythic) - 231770
class go_highmaul_instance_portal : public GameObjectScript
{
    public:
        go_highmaul_instance_portal() : GameObjectScript("go_highmaul_instance_portal") { }

        struct go_highmaul_instance_portalAI : public GameObjectAI
        {
            go_highmaul_instance_portalAI(GameObject* p_GameObject) : GameObjectAI(p_GameObject)
            {
                m_CheckTimer = 1000;
            }

            uint32 m_CheckTimer;

            void UpdateAI(uint32 p_Diff) override
            {
                if (m_CheckTimer)
                {
                    if (m_CheckTimer <= p_Diff)
                    {
                        m_CheckTimer = 1000;

                        std::list<Player*> l_PlayerList;
                        go->GetPlayerListInGrid(l_PlayerList, 5.0f);

                        for (Player* l_Player : l_PlayerList)
                            l_Player->TeleportTo(eHighmaulLocs::ExitTarget);
                    }
                    else
                        m_CheckTimer -= p_Diff;
                }
            }
        };

        GameObjectAI* GetAI(GameObject* p_GameObject) const override
        {
            return new go_highmaul_instance_portalAI(p_GameObject);
        }
};

/// Portal (teleporter to upper/lower city) - 231776
class go_highmaul_portal : public GameObjectScript
{
    public:
        go_highmaul_portal() : GameObjectScript("go_highmaul_portal") { }

        struct go_highmaul_portalAI : public GameObjectAI
        {
            go_highmaul_portalAI(GameObject* p_GameObject) : GameObjectAI(p_GameObject)
            {
                m_CheckTimer = 500;
                m_IsUp = p_GameObject->GetPositionZ() < 300.0f;
                m_ImperatorsRise = p_GameObject->GetPositionZ() > 400.0f;
            }

            uint32 m_CheckTimer;
            bool m_IsUp;
            bool m_ImperatorsRise;

            enum eSpell
            {
                Teleport = 160595 ///< Cosmetic effect
            };

            void UpdateAI(uint32 p_Diff) override
            {
                if (m_CheckTimer)
                {
                    if (m_CheckTimer <= p_Diff)
                    {
                        m_CheckTimer = 500;

                        std::list<Player*> l_PlayerList;
                        go->GetPlayerListInGrid(l_PlayerList, 5.0f);

                        for (Player* l_Player : l_PlayerList)
                        {
                            l_Player->CastSpell(l_Player, eSpell::Teleport, true);

                            if (m_ImperatorsRise)
                                l_Player->NearTeleportTo(eHighmaulLocs::ImperatorsRise);
                            else
                            {
                                if (m_IsUp)
                                    l_Player->NearTeleportTo(eHighmaulLocs::PalaceFrontGate);
                                else
                                    l_Player->NearTeleportTo(eHighmaulLocs::CityBaseTeleporter);
                            }
                        }
                    }
                    else
                        m_CheckTimer -= p_Diff;
                }
            }
        };

        GameObjectAI* GetAI(GameObject* p_GameObject) const override
        {
            return new go_highmaul_portalAI(p_GameObject);
        }
};

/// Chain Grip - 151990
class spell_highmaul_chain_grip : public SpellScriptLoader
{
    public:
        spell_highmaul_chain_grip() : SpellScriptLoader("spell_highmaul_chain_grip") { }

        class spell_highmaul_chain_grip_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_chain_grip_SpellScript)

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.sort(JadeCore::ObjectDistanceOrderPred(GetCaster(), false));

                WorldObject* l_Object = p_Targets.front();
                p_Targets.clear();
                p_Targets.push_back(l_Object);
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_chain_grip_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_chain_grip_SpellScript();
        }
};

/// Chain Grip - 152024
class spell_highmaul_chain_grip_aura : public SpellScriptLoader
{
    public:
        spell_highmaul_chain_grip_aura() : SpellScriptLoader("spell_highmaul_chain_grip_aura") { }

        class spell_highmaul_chain_grip_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_chain_grip_aura_AuraScript)

            enum eSpell
            {
                ChainGripJumpDest = 151991
            };

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                        l_Target->CastSpell(l_Caster, eSpell::ChainGripJumpDest, true);
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_chain_grip_aura_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_chain_grip_aura_AuraScript();
        }
};

/// Boar's Rush - 166225
class spell_highmaul_boars_rush : public SpellScriptLoader
{
    public:
        spell_highmaul_boars_rush() : SpellScriptLoader("spell_highmaul_boars_rush") { }

        class spell_highmaul_boars_rush_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_boars_rush_SpellScript)

            enum eSpell
            {
                TargetRestrict = 21373
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eSpell::TargetRestrict);
                if (l_Restriction == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Radius = GetSpellInfo()->Effects[0].CalcRadius(l_Caster);
                p_Targets.remove_if([l_Radius, l_Caster, l_Restriction](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!p_Object->IsInAxe(l_Caster, l_Restriction->Width, l_Radius))
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_boars_rush_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_CONE_ENEMY_129);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_boars_rush_SpellScript::CorrectTargets, EFFECT_3, TARGET_UNIT_CONE_ENEMY_129);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_boars_rush_SpellScript();
        }
};

/// Unstoppable Charge - 174465
class spell_highmaul_unstoppable_charge : public SpellScriptLoader
{
    public:
        spell_highmaul_unstoppable_charge() : SpellScriptLoader("spell_highmaul_unstoppable_charge") { }

        class spell_highmaul_unstoppable_charge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_unstoppable_charge_SpellScript)

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                if (Creature* l_IronFlame = l_Caster->ToCreature())
                {
                    Unit* l_Target = Unit::GetUnit(*l_IronFlame, l_IronFlame->AI()->GetGUID(0));
                    if (l_Target == nullptr)
                        return;

                    float l_Radius = GetSpellInfo()->Effects[0].CalcRadius(l_Caster);
                    p_Targets.remove_if([l_Radius, l_Caster, l_Target](WorldObject* p_Object) -> bool
                    {
                        if (p_Object == nullptr)
                            return true;

                        if (!p_Object->IsInBetween(l_Caster, l_Target, 3.0f))
                            return true;

                        return false;
                    });
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_unstoppable_charge_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_unstoppable_charge_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_unstoppable_charge_SpellScript();
        }
};

/// Corrupted Blood Shield - 174474
class spell_highmaul_corrupted_blood_shield : public SpellScriptLoader
{
    public:
        spell_highmaul_corrupted_blood_shield() : SpellScriptLoader("spell_highmaul_corrupted_blood_shield") { }

        class spell_highmaul_corrupted_blood_shield_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_corrupted_blood_shield_AuraScript)

            enum eSpell
            {
                CorruptedBlood = 174473
            };

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode == AuraRemoveMode::AURA_REMOVE_BY_EXPIRE)
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                        l_Target->CastSpell(l_Caster, eSpell::CorruptedBlood, true, nullptr, nullptr, l_Caster->GetGUID());
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_corrupted_blood_shield_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_corrupted_blood_shield_AuraScript();
        }
};

/// Rending Slash - 166185
class spell_highmaul_rending_slash : public SpellScriptLoader
{
    public:
        spell_highmaul_rending_slash() : SpellScriptLoader("spell_highmaul_rending_slash") { }

        class spell_highmaul_rending_slash_SpellScript: public SpellScript
        {
            PrepareSpellScript(spell_highmaul_rending_slash_SpellScript)

            enum eSpell
            {
                TargetRestrict = 22561
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eSpell::TargetRestrict);
                if (l_Restriction == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Angle = 2 * M_PI / 360 * l_Restriction->ConeAngle;
                p_Targets.remove_if([l_Caster, l_Angle](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!p_Object->isInFront(l_Caster, l_Angle))
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_rending_slash_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_54);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_rending_slash_SpellScript();
        }
};

/// Shield Charge - 166178
class spell_highmaul_shield_charge : public SpellScriptLoader
{
    public:
        spell_highmaul_shield_charge() : SpellScriptLoader("spell_highmaul_shield_charge") { }

        class spell_highmaul_shield_charge_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_shield_charge_AuraScript)

            enum eSpells
            {
                ShieldChargeDamage  = 166180,
                ShieldChargeBump    = 166181
            };

            uint32 m_DamageTimer;

            bool Load() override
            {
                m_DamageTimer = 500;
                return true;
            }

            void OnUpdate(uint32 p_Diff)
            {
                if (m_DamageTimer)
                {
                    if (m_DamageTimer <= p_Diff)
                    {
                        if (Unit* l_Caster = GetCaster())
                        {
                            std::list<Unit*> l_TargetList;
                            float l_Radius = 1.0f;

                            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(l_Caster, l_Caster, l_Radius);
                            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(l_Caster, l_TargetList, l_Check);
                            l_Caster->VisitNearbyObject(l_Radius, l_Searcher);

                            for (Unit* l_Iter : l_TargetList)
                            {
                                l_Caster->CastSpell(l_Iter, eSpells::ShieldChargeDamage, true);
                                l_Iter->CastSpell(l_Iter, eSpells::ShieldChargeBump, true);
                            }
                        }

                        m_DamageTimer = 500;
                    }
                    else
                        m_DamageTimer -= p_Diff;
                }
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_highmaul_shield_charge_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_shield_charge_AuraScript();
        }
};

/// Earthdevastating Slam - 166174
class spell_highmaul_earthdevastating_slam : public SpellScriptLoader
{
    public:
        spell_highmaul_earthdevastating_slam() : SpellScriptLoader("spell_highmaul_earthdevastating_slam") { }

        class spell_highmaul_earthdevastating_slam_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_earthdevastating_slam_AuraScript)

            enum eAction
            {
                ActionSlam
            };

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (GetTarget() == nullptr)
                    return;

                if (Creature* l_Trash = GetTarget()->ToCreature())
                {
                    if (l_Trash->IsAIEnabled)
                        l_Trash->AI()->DoAction(eAction::ActionSlam);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_earthdevastating_slam_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_earthdevastating_slam_AuraScript();
        }
};

/// Earthdevastating Slam (damage) - 166175
class spell_highmaul_earthdevastating_slam_dmg : public SpellScriptLoader
{
    public:
        spell_highmaul_earthdevastating_slam_dmg() : SpellScriptLoader("spell_highmaul_earthdevastating_slam_dmg") { }

        class spell_highmaul_earthdevastating_slam_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_earthdevastating_slam_dmg_SpellScript)

            enum eSpell
            {
                TargetRestrict = 21362
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eSpell::TargetRestrict);
                if (l_Restriction == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Radius = GetSpellInfo()->Effects[0].CalcRadius(l_Caster);
                p_Targets.remove_if([l_Radius, l_Caster, l_Restriction](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!p_Object->IsInAxe(l_Caster, l_Restriction->Width, l_Radius))
                        return true;

                    if (!p_Object->isInFront(l_Caster))
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_earthdevastating_slam_dmg_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_129);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_earthdevastating_slam_dmg_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_CONE_ENEMY_129);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_earthdevastating_slam_dmg_SpellScript();
        }
};

/// Arcane Barrage - 178023
class spell_highmaul_arcane_barrage : public SpellScriptLoader
{
    public:
        spell_highmaul_arcane_barrage() : SpellScriptLoader("spell_highmaul_arcane_barrage") { }

        enum eSpells
        {
            ArcaneBarrageFirst = 178025,
            ArcaneBarrageSecond = 178026
        };

        class spell_highmaul_arcane_barrage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_arcane_barrage_SpellScript)

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, eSpells::ArcaneBarrageFirst, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_highmaul_arcane_barrage_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_arcane_barrage_SpellScript();
        }

        class spell_highmaul_arcane_barrage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_arcane_barrage_AuraScript)

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, eSpells::ArcaneBarrageSecond, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_arcane_barrage_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_arcane_barrage_AuraScript();
        }
};

/// Decimate - 166189
class spell_highmaul_decimate : public SpellScriptLoader
{
    public:
        spell_highmaul_decimate() : SpellScriptLoader("spell_highmaul_decimate") { }

        enum eSpells
        {
            DecimateMissile = 166187
        };

        class spell_highmaul_decimate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_decimate_SpellScript)

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                        l_Caster->CastSpell(l_Target, eSpells::DecimateMissile, true);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_highmaul_decimate_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_decimate_SpellScript();
        }

        class spell_highmaul_decimate_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_decimate_AuraScript)

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                        l_Caster->CastSpell(l_Target, eSpells::DecimateMissile, true);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_decimate_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_decimate_AuraScript();
        }
};

/// Unstable Tempest - 174981
class spell_highmaul_unstable_tempest : public SpellScriptLoader
{
    public:
        spell_highmaul_unstable_tempest() : SpellScriptLoader("spell_highmaul_unstable_tempest") { }

        class spell_highmaul_unstable_tempest_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_unstable_tempest_SpellScript)

            enum eSpell
            {
                UnstableTempestStack = 174976
            };

            void HandleOnCast()
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, eSpell::UnstableTempestStack, true);
            }

            void Register() override
            {
                OnCast += SpellCastFn(spell_highmaul_unstable_tempest_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_unstable_tempest_SpellScript();
        }
};

/// Time Stop - 174939
class spell_highmaul_time_stop : public SpellScriptLoader
{
    public:
        spell_highmaul_time_stop() : SpellScriptLoader("spell_highmaul_time_stop") { }

        enum eSpells
        {
            TimeStopStun = 174961
        };

        class spell_highmaul_time_stop_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_time_stop_AuraScript)

            void OnTick(AuraEffect const* p_AurEff)
            {
                if (p_AurEff->GetTickNumber() % 3)
                    return;

                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, eSpells::TimeStopStun, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_time_stop_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_time_stop_AuraScript();
        }
};

/// Rune of Disintegration - 175648
class areatrigger_highmaul_rune_of_disintegration : public AreaTriggerEntityScript
{
    public:
        areatrigger_highmaul_rune_of_disintegration() : AreaTriggerEntityScript("areatrigger_highmaul_rune_of_disintegration") { }

        enum eSpell
        {
            RuneOfDisintegration = 175654
        };

        std::set<uint64> m_AffectedPlayers;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 4.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                std::set<uint64> l_Targets;

                for (Unit* l_Iter : l_TargetList)
                {
                    l_Targets.insert(l_Iter->GetGUID());

                    if (!l_Iter->HasAura(eSpell::RuneOfDisintegration))
                    {
                        m_AffectedPlayers.insert(l_Iter->GetGUID());
                        l_Iter->CastSpell(l_Iter, eSpell::RuneOfDisintegration, true);
                    }
                }

                for (std::set<uint64>::iterator l_Iter = m_AffectedPlayers.begin(); l_Iter != m_AffectedPlayers.end();)
                {
                    if (l_Targets.find((*l_Iter)) != l_Targets.end())
                    {
                        ++l_Iter;
                        continue;
                    }

                    if (Unit* l_Unit = Unit::GetUnit(*l_Caster, (*l_Iter)))
                    {
                        l_Iter = m_AffectedPlayers.erase(l_Iter);
                        l_Unit->RemoveAura(eSpell::RuneOfDisintegration);

                        continue;
                    }

                    ++l_Iter;
                }
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                for (uint64 l_Guid : m_AffectedPlayers)
                {
                    if (Unit* l_Unit = Unit::GetUnit(*l_Caster, l_Guid))
                        l_Unit->RemoveAura(eSpell::RuneOfDisintegration);
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_highmaul_rune_of_disintegration();
        }
};

/// Arcane Residue - 175047
class areatrigger_highmaul_arcane_residue : public AreaTriggerEntityScript
{
    public:
        areatrigger_highmaul_arcane_residue() : AreaTriggerEntityScript("areatrigger_highmaul_arcane_residue") { }

        enum eSpell
        {
            ArcaneResidue = 175056
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 8.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                {
                    if (l_Unit->GetDistance(l_Caster) > 4.0f)
                    {
                        if (l_Unit->HasAura(eSpell::ArcaneResidue))
                            l_Unit->RemoveAura(eSpell::ArcaneResidue);
                    }
                    else
                    {
                        if (!l_Unit->HasAura(eSpell::ArcaneResidue))
                            l_Caster->CastSpell(l_Unit, eSpell::ArcaneResidue, true);
                    }
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_highmaul_arcane_residue();
        }
};

#ifndef __clang_analyzer__
void AddSC_highmaul()
{
    /// NPCs
    new npc_highmaul_gharg_arena_master();
    new npc_highmaul_jhorn_the_mad();
    new npc_highmaul_thoktar_ironskull();
    new npc_highmaul_imperator_margok();
    new npc_highmaul_gorian_guardsman();
    new npc_highmaul_night_twisted_devout();
    new npc_highmaul_gorian_runemaster();
    new npc_highmaul_gorian_enforcer();
    new npc_highmaul_underbelly_vagrant();
    new npc_highmaul_gorian_sorcerer();
    new npc_highmaul_night_twisted_brute();
    new npc_highmaul_night_twisted_soothsayer();
    new npc_highmaul_void_aberration();
    new npc_highmaul_krush();
    new npc_highmaul_iron_flame_technician();
    new npc_highmaul_iron_warmaster();
    new npc_highmaul_iron_blood_mage();
    new npc_highmaul_night_twisted_ritualist();
    new npc_highmaul_greater_void_aberration();
    new npc_highmaul_highmaul_conscript();
    new npc_highmaul_ogron_earthshaker();
    new npc_highmaul_gorian_arcanist();
    new npc_highmaul_ogron_brute();
    new npc_highmaul_gorian_rune_mender();
    new npc_highmaul_night_twisted_fanatic();
    new npc_highmaul_gorian_warden();
    new npc_highmaul_ogron_warbringer();
    new npc_highmaul_warden_thultok();
    new npc_highmaul_gorian_royal_guardsman();
    new npc_highmaul_gorian_high_sorcerer();
    new npc_highmaul_ogron_mauler();
    new npc_highmaul_guard_captain_thag();
    new npc_highmaul_councilor_daglat();
    new npc_highmaul_councilor_magknor();
    new npc_highmaul_arcane_torrent();
    new npc_highmaul_councilor_gorluk();
    new npc_highmaul_phantasmal_weapon();
    new npc_highmaul_councilor_nouk();
    new npc_highmaul_high_councilor_malgris();

    /// GameObjects
    new go_highmaul_arena_elevator();
    new go_highmaul_instance_portal();
    new go_highmaul_portal();

    /// Spells
    new spell_highmaul_chain_grip();
    new spell_highmaul_chain_grip_aura();
    new spell_highmaul_boars_rush();
    new spell_highmaul_unstoppable_charge();
    new spell_highmaul_corrupted_blood_shield();
    new spell_highmaul_rending_slash();
    new spell_highmaul_shield_charge();
    new spell_highmaul_earthdevastating_slam();
    new spell_highmaul_earthdevastating_slam_dmg();
    new spell_highmaul_arcane_barrage();
    new spell_highmaul_decimate();
    new spell_highmaul_unstable_tempest();
    new spell_highmaul_time_stop();

    /// AreaTriggers
    new areatrigger_highmaul_rune_of_disintegration();
    new areatrigger_highmaul_arcane_residue();
}
#endif
