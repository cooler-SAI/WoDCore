////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "lost_city_of_the_tolvir.h"
#include "ScriptPCH.h"
#include "AchievementMgr.h"

enum eSpells
{
    SPELL_ABSORB_STORMS              = 83151,
    SPELL_CALL_OF_SKY                = 84956,
    SPELL_DEFLECTING_WINDS           = 84589,
    SPELL_STORM_BOLT_PHASE_DW        = 73564,
    SPELL_STORM_BOLT_PHASE_S         = 91853,
    // Cloud Burst
    SPELL_CLOUD_BURST_SUMMON         = 83790,
    SPELL_CLOUD_BURST                = 83051,
    // Static Shock
    SPELL_STATIC_SHOCK_1             = 84546,
    SPELL_STATIC_SHOCK_2             = 84555,
    SPELL_STATIC_SHOCK_3             = 84556,
    // Wailing Winds
    SPELL_WAILING_WINDS              = 83094,
    SPELL_WAILING_WINDS_AURA         = 83066,
    // Servant of Siamat
    SPELL_THUNDER_CRASH              = 84521,
    SPELL_LIGHTNING_NOVA             = 84544,
    // Siamat Minion
    SPELL_CHAIN_LIGHTNING            = 83455,
    SPELL_TEMPEST_STORM_SUMMON       = 83414,
    SPELL_TEMPEST_STORM_TRANSFORM    = 83170,
    SPELL_LIGHTNING_CHARGE           = 91872,
    SPELL_LIGHTNING_CHARGE_AURA      = 93959
};

enum eCreatures
{
    NPC_TEMPEST_STORM                = 44713,
    NPC_SETVANT_OF_SIAMAT            = 45269
};

enum eActions
{
    ACTION_SERVANT_DEATH
};

enum eTexts
{
    SAY_START_1                        = -1877011,
    SAY_START_2                        = -1877027,
    SAY_WAILING_WINDS_1                = -1877012,
    SAY_WAILING_WINDS_2                = -1877026,
    SAY_DEATH                          = -1877013,
    SAY_KILL_PLAYER                    = -1877025
};

enum ePhases
{
    PHASE_DEFLECTING_WINDS           = 1,
    PHASE_WAILING_WINDS              = 2,
    PHASE_SIAMAT                     = 3,

    PHASE_WAILING_WINDS_MASK         = 1 << PHASE_WAILING_WINDS
};

enum eEvents
{
    // Siamat
    EVENT_STATIC_SHOCK               = 1,
    EVENT_DEFLECTING_WINDS           = 2,
    EVENT_CLOUD_BURST                = 3,
    EVENT_CALL_OF_SKY                = 4,
    EVENT_WAILING_WINDS              = 5,
    EVENT_STORM_BOLT_DW              = 6,
    EVENT_STORM_BOLT_S               = 7,
    EVENT_ABSORB_STORMS              = 8,
    // Servant of Siamat
    EVENT_THUNDER_CRASH              = 1,
    EVENT_LIGHTNING_NOVA             = 2,
    EVENT_SERVANT_DIED               = 3,
    // Siamat Minion
    EVENT_CHAIN_LIGHTNING            = 1,
    // Cloud Burst
    EVENT_PERIODIC_CAST              = 1
};

const uint32 StaticShock[3]=
{
    SPELL_STATIC_SHOCK_1,
    SPELL_STATIC_SHOCK_2,
    SPELL_STATIC_SHOCK_3
};

#define    FLOR_COORD_Z    36.0f

class boss_siamat : public CreatureScript
{
public:
    boss_siamat() : CreatureScript("boss_siamat") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_siamatAI (creature);
    }

    struct boss_siamatAI : public ScriptedAI
    {
        boss_siamatAI(Creature* creature) : ScriptedAI(creature), lSummons(me)
        {
            instance = creature->GetInstanceScript();
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        EventMap events;
        SummonList lSummons;
        InstanceScript* instance;
        uint8 uiStaticShockId;

        void Reset()
        {
            if (instance)
                instance->SetData(DATA_SIAMAT, NOT_STARTED);

            events.Reset();
            lSummons.DespawnAll();
            uiStaticShockId = 0;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
        }
        
        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(RAND(SAY_START_1, SAY_START_1), me);
            events.SetPhase(PHASE_DEFLECTING_WINDS);
            events.ScheduleEvent(EVENT_STATIC_SHOCK, 2000, 0, PHASE_DEFLECTING_WINDS);
            events.ScheduleEvent(EVENT_DEFLECTING_WINDS, 5000, 0, PHASE_DEFLECTING_WINDS);
            events.ScheduleEvent(EVENT_CALL_OF_SKY, 15000);
            events.ScheduleEvent(EVENT_CLOUD_BURST, 7000, 0, PHASE_DEFLECTING_WINDS);
            events.ScheduleEvent(EVENT_STORM_BOLT_DW, 500, 0, PHASE_DEFLECTING_WINDS);

            if (instance)
                instance->SetData(DATA_SIAMAT, IN_PROGRESS);
        }

        void DoAction(const int32 action)
        {
            if (action == ACTION_SERVANT_DEATH)
            {
                DoScriptText(RAND(SAY_WAILING_WINDS_1, SAY_WAILING_WINDS_2), me);
                me->RemoveAura(SPELL_DEFLECTING_WINDS);
                me->CastSpell(me, SPELL_WAILING_WINDS_AURA, false);
                events.SetPhase(PHASE_WAILING_WINDS);
                events.ScheduleEvent(EVENT_WAILING_WINDS, 1000, 0, PHASE_WAILING_WINDS);
                events.ScheduleEvent(EVENT_STORM_BOLT_S, urand(10000, 25000), 0, PHASE_SIAMAT);
            }
        }

        void JustSummoned(Creature* summoned)
        {
            lSummons.Summon(summoned);
        }

        void KilledUnit(Unit* victim)
        {
            if (victim->IsPlayer())
                DoScriptText(SAY_KILL_PLAYER, me);
        }

        void JustDied(Unit* /*killer*/)
        {
            events.Reset();
            lSummons.DespawnAll();
            DoScriptText(SAY_DEATH, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING) && events.GetPhaseMask() != PHASE_WAILING_WINDS_MASK)
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_STATIC_SHOCK:
                        {
                            uint8 dist = urand(5, 30);
                            float angle = frand(0, M_PI);
                            float x, y;
                            me->GetNearPoint2D(x, y, (float)dist, angle);
                            me->CastSpell(x, y, FLOR_COORD_Z, StaticShock[uiStaticShockId], false);
                            ++uiStaticShockId;

                            if (uiStaticShockId < 3)
                                events.ScheduleEvent(EVENT_STATIC_SHOCK, 32000, 0, PHASE_DEFLECTING_WINDS);
                        }
                        break;
                    case EVENT_DEFLECTING_WINDS:
                        me->CastSpell(me, SPELL_DEFLECTING_WINDS, false);
                        break;
                    case EVENT_CALL_OF_SKY:
                        {
                            uint8 dist = urand(5, 30);
                            float angle = frand(0, M_PI);
                            float x, y;
                            me->GetNearPoint2D(x, y, (float)dist, angle);
                            me->CastSpell(x, y, FLOR_COORD_Z, SPELL_CALL_OF_SKY, false);
                            events.ScheduleEvent(EVENT_CALL_OF_SKY, urand(15000, 35000));
                        }
                        break;
                    case EVENT_CLOUD_BURST:
                        {
                            events.ScheduleEvent(EVENT_CLOUD_BURST, urand(10000, 25000), 0, PHASE_DEFLECTING_WINDS);

                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            {
                                float x, y, z;
                                target->GetPosition(x, y, z);
                                me->CastSpell(x, y, z, SPELL_CLOUD_BURST_SUMMON, false);
                            }
                        }
                        break;
                    case EVENT_WAILING_WINDS:
                        {
                            me->CastSpell(me, SPELL_WAILING_WINDS, false);

                            if (Spell* spell = me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                            {
                                if (const SpellInfo* spellInfo = spell->GetSpellInfo())
                                    if (spellInfo->Id == SPELL_WAILING_WINDS_AURA)
                                        events.ScheduleEvent(EVENT_WAILING_WINDS, 1000, 0, PHASE_WAILING_WINDS);
                            }
                            else
                            {
                                events.SetPhase(PHASE_SIAMAT);
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                                me->GetMotionMaster()->MoveChase(me->getVictim());
                                events.ScheduleEvent(EVENT_ABSORB_STORMS, 15000, 0, PHASE_SIAMAT);
                                events.ScheduleEvent(EVENT_STORM_BOLT_S, urand(10000, 25000), 0, PHASE_SIAMAT);
                            }
                        }
                        break;
                    case EVENT_ABSORB_STORMS:
                        me->CastSpell(me, SPELL_ABSORB_STORMS, false);
                        events.ScheduleEvent(EVENT_ABSORB_STORMS, 33000, 0, PHASE_SIAMAT);
                        break;
                    case EVENT_STORM_BOLT_DW:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                me->CastSpell(target, SPELL_STORM_BOLT_PHASE_DW, false);

                            events.ScheduleEvent(EVENT_STORM_BOLT_DW, 2500, 0, PHASE_DEFLECTING_WINDS);
                        }
                        break;
                    case EVENT_STORM_BOLT_S:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                me->CastSpell(target, SPELL_STORM_BOLT_PHASE_S, false);

                            events.ScheduleEvent(EVENT_STORM_BOLT_S, urand(10000, 25000), 0, PHASE_SIAMAT);
                        }
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

class npc_servant_of_siamat : public CreatureScript
{
public:
    npc_servant_of_siamat() : CreatureScript("npc_servant_of_siamat") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_servant_of_siamatAI (creature);
    }

    struct npc_servant_of_siamatAI : public ScriptedAI
    {
        npc_servant_of_siamatAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            me->SetInCombatWithZone();
        }

        EventMap events;
        InstanceScript* instance;
        bool LightningCharge;

        void Reset()
        {
            events.Reset();
            LightningCharge = false;
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_THUNDER_CRASH, 1000);
            events.ScheduleEvent(EVENT_LIGHTNING_NOVA, 5000);
        }

        void DamageTaken(Unit* , uint32 &damage, SpellInfo const*  /*p_SpellInfo*/)
        {
            if (!IsHeroic())
                return;

            if (damage >= me->GetHealth())
            {
                damage = 0;
                me->SetHealth(1);

                if (!LightningCharge)
                {
                    if (instance)
                        me->CastSpell(me, SPELL_LIGHTNING_CHARGE, false, NULL, NULL, instance->GetData64(DATA_SIAMAT));

                    LightningCharge = true;
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    events.Reset();
                    events.ScheduleEvent(EVENT_SERVANT_DIED, 2000);
                }
            }
        }

        void JustDied(Unit* /*killer*/)
        {
            if (me->GetEntry() == NPC_SETVANT_OF_SIAMAT)
                if (instance)
                    if (Creature* siamat = me->FindNearestCreature(BOSS_SIAMAT, 300.0f))
                        siamat->AI()->DoAction(ACTION_SERVANT_DEATH);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SERVANT_DIED:
                        me->Kill(me);
                        break;
                    case EVENT_THUNDER_CRASH:
                        me->CastSpell(me, SPELL_THUNDER_CRASH, false);
                        events.ScheduleEvent(EVENT_THUNDER_CRASH, urand(7000, 15000));
                        break;
                    case EVENT_LIGHTNING_NOVA:
                        me->CastSpell(me->getVictim(), SPELL_LIGHTNING_NOVA, false);
                        events.ScheduleEvent(EVENT_LIGHTNING_NOVA, urand(7000, 15000));
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

class npc_siamat_minion : public CreatureScript
{
public:
    npc_siamat_minion() : CreatureScript("npc_siamat_minion") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_siamat_minionAI (creature);
    }

    struct npc_siamat_minionAI : public ScriptedAI
    {
        npc_siamat_minionAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetInCombatWithZone();
        }

        EventMap events;
        bool TempestStorm;

        void Reset()
        {
            events.Reset();
            events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 1000);
            TempestStorm = false;
            me->AddAura(84550, me);
        }
        
        void JustSummoned(Creature* summoned)
        {
            if (summoned->GetEntry() == NPC_TEMPEST_STORM)
            {
                summoned->SetReactState(REACT_PASSIVE);
                summoned->SetInCombatWithZone();
                summoned->GetMotionMaster()->MoveRandom(25.0f);
                InstanceScript* instance = me->GetInstanceScript();

                if (instance)
                    if (Creature* siamat = Unit::GetCreature(*me, instance->GetData64(DATA_SIAMAT)))
                        siamat->AI()->JustSummoned(summoned);
            }
        }

        void SpellHit(Unit* /*caster*/, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_TEMPEST_STORM_TRANSFORM)
            {
                me->CastSpell(me, SPELL_TEMPEST_STORM_SUMMON, false);
                me->DespawnOrUnsummon(100);
            }
        }

        void DamageTaken(Unit* , uint32 &damage, SpellInfo const*  /*p_SpellInfo*/)
        {
            if (damage >= me->GetHealth())
            {
                damage = 0;
                me->SetHealth(1);

                if (!TempestStorm)
                {
                    events.Reset();
                    me->RemoveAllAuras();
                    TempestStorm = true;
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    me->StopMoving();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    me->CastSpell(me, SPELL_TEMPEST_STORM_TRANSFORM, false);
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (events.ExecuteEvent() == EVENT_CHAIN_LIGHTNING)
            {
                me->CastSpell(me->getVictim(), SPELL_CHAIN_LIGHTNING, false);
                events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 2000);
            }

            DoMeleeAttackIfReady();
        }
    };
};

class npc_cloud_burst : public CreatureScript
{
public:
    npc_cloud_burst() : CreatureScript("npc_cloud_burst") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_cloud_burstAI (creature);
    }

    struct npc_cloud_burstAI : public ScriptedAI
    {
        npc_cloud_burstAI(Creature* creature) : ScriptedAI(creature)
        {
            uiTickCount = 0;
            me->SetInCombatWithZone();
            events.ScheduleEvent(EVENT_PERIODIC_CAST, 1000);
        }

        EventMap events;
        uint8 uiTickCount;

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            if (events.ExecuteEvent() == EVENT_PERIODIC_CAST)
            {
                ++uiTickCount;
                me->CastSpell(me, SPELL_CLOUD_BURST, false);

                if (uiTickCount < 3)
                    events.ScheduleEvent(EVENT_PERIODIC_CAST, 1000);
                else
                    me->DespawnOrUnsummon(500);
            }
        }
    };
};

class spell_wailing_winds: public SpellScriptLoader
{
    public:
        spell_wailing_winds() : SpellScriptLoader("spell_wailing_winds") { }

        class spell_wailing_winds_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_wailing_winds_SpellScript)

            void RandomJump(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Unit* target = GetHitUnit();

                if (!target)
                    return;

                uint8 roll = urand(0, 1);
                float angle = frand(0, M_PI);
                float SpeedXY = frand(10.0f, 30.0f);
                float SpeedZ = frand(10.0f, 15.0f);
                float x, y;

                if (!roll)
                {
                    target->GetPosition(x, y);
                    target->SetOrientation(angle);
                    target->KnockbackFrom(x, y, SpeedXY, SpeedZ);
                }
                else
                {
                    float dist = frand(10.0f, 30.0f);
                    target->GetNearPoint2D(x, y, dist, angle);
                    target->GetMotionMaster()->MoveJump(x, y, FLOR_COORD_Z, SpeedXY, SpeedZ);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_wailing_winds_SpellScript::RandomJump, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_wailing_winds_SpellScript();
        }
};

class spell_gathered_storms: public SpellScriptLoader
{
    public:
        spell_gathered_storms() : SpellScriptLoader("spell_gathered_storms") { }

        class spell_gathered_storms_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gathered_storms_SpellScript)

            void ApplyAura(SpellEffIndex effIndex)
            {
                Unit* target = GetHitUnit();

                if (!target)
                    return;

                if (Creature* storm = target->ToCreature())
                    if (storm->GetEntry() == NPC_TEMPEST_STORM)
                    {
                        storm->RemoveAllAuras();
                        storm->DespawnOrUnsummon(2000);
                        PreventHitDefaultEffect(effIndex);
                        return;
                    }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gathered_storms_SpellScript::ApplyAura, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_gathered_storms_SpellScript();
        }
};

class achievement_headed_south : public AchievementCriteriaScript
{
    public:
        achievement_headed_south() : AchievementCriteriaScript("achievement_headed_south") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            if (Aura* aura = source->GetAura(SPELL_LIGHTNING_CHARGE_AURA))
                if (aura->GetStackAmount() == 3)
                    return true;

            return false;
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_siamat()
{
    new boss_siamat();
    new npc_servant_of_siamat();
    new npc_siamat_minion();
    new npc_cloud_burst();

    new spell_wailing_winds();
    new spell_gathered_storms();

    new achievement_headed_south();
}
#endif
