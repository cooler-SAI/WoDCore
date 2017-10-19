////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "deadmines.h"
#include "Vehicle.h"

enum ScriptTexts
{
    SAY_AGGRO    = 5,
    SAY_KILL     = 1,
    SAY_DEATH    = 0,
    SAY_SPELL1   = 3,
    SAY_SPELL2   = 4,
    SAY_OAF_DEAD = 2,

    SAY_OAF1     = 0,
    SAY_OAF2     = 1
};

enum Spells
{
    SPELL_ARMED_STATE           = 88319,
    SPELL_PERIODIC_TRIGGER      = 88329,
    SPELL_EXPLODE               = 88974,
    SPELL_EXPLODE_H             = 91566,

    SPELL_CHEST_BOMB            = 88352,
    SPELL_CHEST_BOMB_DMG        = 88250,

    SPELL_OAF_SMASH             = 88300,
    SPELL_OAF_SMASH_H           = 91568,
    SPELL_CHARGE                = 88288,
    SPELL_FORCE_PLAYER_RIDE_OAF = 88278,
    SPELL_RIDE_OAF              = 88277
};

enum Adds
{
    NPC_STICKY_BOMB = 47314,
    NPC_HELIX_CREW  = 49136
};

enum Events
{
    EVENT_THROW_HELIX   = 5,
    EVENT_PICKUP_HELIX  = 6,
    EVENT_STICKY_BOMB   = 7,
    EVENT_HELIX_CREW    = 8,
    EVENT_CHARGE_OAF0   = 1,
    EVENT_CHARGE_OAF1   = 2,
    EVENT_CHARGE_OAF2   = 3,
    EVENT_CHARGE_OAF3   = 4,
    EVENT_BOMB_READY    = 9,
    EVENT_BOMB_EXPLODE  = 10,
    EVENT_CHEST_BOMB    = 11
};

enum Points
{
    POINT_START = 1,
    POINT_END   = 2
};

enum Actions
{
    ACTION_CHARGE   = 1
};

const Position lumberingoafPos[3] =
{
    {-301.93f, -516.32f, 51.71f, 2.03f},
    {-289.98f, -528.06f, 49.75f, 1.59f},
    {-289.67f, -488.46f, 49.80f, 1.54f}
};

const Position helixcrewPos[4] =
{
    {-295.26f,-503.38f,60.16f, 0.0f},
    {-280.85f,-503.98f,60.16f, 0.0f},
    {-291.45f,-503.37f,60.16f, 0.0f},
    {-285.63f,-504.04f,60.16f, 0.0f}
};

class boss_helix_gearbreaker : public CreatureScript
{
    public:
        boss_helix_gearbreaker() : CreatureScript("boss_helix_gearbreaker") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_helix_gearbreakerAI (pCreature);
        }

        struct boss_helix_gearbreakerAI : public BossAI
        {
            boss_helix_gearbreakerAI(Creature* pCreature) : BossAI(pCreature, DATA_HELIX)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->setActive(true);
            }
            
            void Reset()
            {
                _Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_STICKY_BOMB, 8000);
                if (IsHeroic())
                {
                    events.ScheduleEvent(EVENT_CHEST_BOMB, urand(8000, 12000));
                    events.ScheduleEvent(EVENT_HELIX_CREW, 5000);
                }
                Talk(SAY_AGGRO);
                DoZoneInCombat();
                instance->SetBossState(DATA_HELIX, IN_PROGRESS);
            }

            void KilledUnit(Unit * /*victim*/)
            {
                Talk(SAY_KILL);
            }
            
            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (!me->GetVehicle())
                    if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
                    {
                        Talk(SAY_OAF_DEAD);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        return;
                    }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHEST_BOMB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                Talk(SAY_SPELL1);
                                DoCast(target, SPELL_CHEST_BOMB);
                            }
                            events.ScheduleEvent(EVENT_CHEST_BOMB, urand(15000, 20000));
                            break;
                        case EVENT_STICKY_BOMB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                me->SummonCreature(NPC_STICKY_BOMB,
                                    target->GetPositionX(),
                                    target->GetPositionY(),
                                    target->GetPositionZ(),
                                    0.0f);
                            }
                            events.ScheduleEvent(EVENT_STICKY_BOMB, urand(8000, 10000));
                            break;
                        case EVENT_HELIX_CREW:
                            for (uint8 i = 0; i < 4; i++)
                                me->SummonCreature(NPC_HELIX_CREW, helixcrewPos[i], TEMPSUMMON_CORPSE_DESPAWN);
                            break;
                    }
                }

                if (!me->GetVehicle())
                    DoMeleeAttackIfReady();
            }
        };
};

class npc_lumbering_oaf : public CreatureScript
{
    public:
        npc_lumbering_oaf() : CreatureScript("npc_lumbering_oaf") { }
     
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_lumbering_oafAI (pCreature);
        }
     
        struct npc_lumbering_oafAI : public ScriptedAI
        {
            npc_lumbering_oafAI(Creature *c) : ScriptedAI(c)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->setActive(true);
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }
     
            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_CHARGE:
                        events.ScheduleEvent(EVENT_CHARGE_OAF1, 500);
                        break;
                }
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_CHARGE_OAF0, 10000);
            }

            void EnterEvadeMode()
            {
                me->DespawnOrUnsummon();
            }

            void DamageTaken(Unit* /*attacker*/, uint32 &damage, SpellInfo const* /*p_SpellInfo*/)
            {
                if (me->GetHealth() <= damage)
                    me->GetVehicleKit()->RemoveAllPassengers();
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type == POINT_MOTION_TYPE)
                {
                    switch (id)
                    {
                        case POINT_START:
                            events.ScheduleEvent(EVENT_CHARGE_OAF2, 500);
                            break;
                        case POINT_END:
                            events.ScheduleEvent(EVENT_CHARGE_OAF3, 500);
                            break;
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHARGE_OAF0:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                me->SetReactState(REACT_PASSIVE);
                                DoCast(target, SPELL_CHARGE, true);
                            }
                            break;
                        case EVENT_CHARGE_OAF1:
                            me->GetMotionMaster()->MovePoint(POINT_START, lumberingoafPos[1]);
                            break;
                        case EVENT_CHARGE_OAF2:
                            me->GetMotionMaster()->MoveCharge(
                                lumberingoafPos[2].GetPositionX(),
                                lumberingoafPos[2].GetPositionY(),
                                lumberingoafPos[2].GetPositionZ(),
                                42.0f, POINT_END);
                            break;
                        case EVENT_CHARGE_OAF3:
                            DoCastAOE(SPELL_OAF_SMASH);
                            me->SetReactState(REACT_AGGRESSIVE);
                            if (me->getVictim())
                                me->GetMotionMaster()->MoveChase(me->getVictim());
                            me->RemoveAurasDueToSpell(SPELL_RIDE_OAF);
                            events.ScheduleEvent(EVENT_CHARGE_OAF0, 30000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_sticky_bomb : public CreatureScript
{
    public:
        npc_sticky_bomb() : CreatureScript("npc_sticky_bomb") { }
     
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_sticky_bombAI (pCreature);
        }
     
        struct npc_sticky_bombAI : public Scripted_NoMovementAI
        {
            npc_sticky_bombAI(Creature *c) : Scripted_NoMovementAI(c)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                pInstance = c->GetInstanceScript();
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
           
            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }
     
            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_BOMB_READY, 6000);
                events.ScheduleEvent(EVENT_BOMB_EXPLODE, 18000);
            }

            void JustDied(Unit* /*killer*/)
            {
                me->DespawnOrUnsummon();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BOMB_READY:
                            DoCast(me, SPELL_ARMED_STATE);
                            break;
                        case EVENT_BOMB_EXPLODE:
                            DoCast(me, SPELL_EXPLODE);
                            me->DespawnOrUnsummon(500);
                            break;
                    }
                }
            }
        };
};

class npc_helix_crew : public CreatureScript
{
    public:
        npc_helix_crew() : CreatureScript("npc_helix_crew") { }
     
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_helix_crewAI (pCreature);
        }
     
        struct npc_helix_crewAI : public Scripted_NoMovementAI
        {
            npc_helix_crewAI(Creature *c) : Scripted_NoMovementAI(c), summons(me)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                pInstance = c->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
            SummonList summons;

            void Reset()
            {
                if (!pInstance)
                    return;
                
                summons.DespawnAll();
                SetCombatMovement(false);
            }
     
            void JustSummoned(Creature* summon)
            {
                if (!pInstance)
                    return;

                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                if (!pInstance)
                    return;

                summons.Despawn(summon);
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_STICKY_BOMB, 8000);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (!pInstance)
                    return;

                summons.DespawnAll();
                me->DespawnOrUnsummon();
            }

            void UpdateAI(const uint32 diff)
            {

                if (!pInstance)
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STICKY_BOMB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                me->SummonCreature(NPC_STICKY_BOMB, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 30000);
                            events.ScheduleEvent(EVENT_STICKY_BOMB, urand(15000, 20000));
                            break;
                    }
                }
                  
            }
        };
};

class spell_helix_chest_bomb: public SpellScriptLoader
{
    public:
        spell_helix_chest_bomb() : SpellScriptLoader("spell_helix_chest_bomb") { }


        class spell_helix_chest_bomb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_helix_chest_bomb_AuraScript)

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = aurEff->GetBase()->GetUnitOwner())
                    target->CastSpell(target, SPELL_CHEST_BOMB_DMG, true);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_helix_chest_bomb_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
             }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_helix_chest_bomb_AuraScript();
        }
};

class spell_helix_force_player_to_ride_oaf: public SpellScriptLoader
{
    public:
        spell_helix_force_player_to_ride_oaf() : SpellScriptLoader("spell_helix_force_player_to_ride_oaf") { }


        class spell_helix_force_player_to_ride_oaf_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_helix_force_player_to_ride_oaf_SpellScript)


            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetHitUnit()->CastSpell(GetCaster(), SPELL_RIDE_OAF, true);
                GetCaster()->ToCreature()->AI()->DoAction(ACTION_CHARGE);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_helix_force_player_to_ride_oaf_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_helix_force_player_to_ride_oaf_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_helix_gearbreaker()
{
    new boss_helix_gearbreaker();
    new npc_lumbering_oaf();
    new npc_helix_crew();
    new npc_sticky_bomb();
    new spell_helix_force_player_to_ride_oaf();
    new spell_helix_chest_bomb();
}
#endif
