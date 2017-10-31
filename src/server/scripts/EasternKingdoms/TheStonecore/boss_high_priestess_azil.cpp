////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "the_stonecore.h"

enum ScriptTexts
{
    SAY_AGGRO    = 3,
    SAY_DEATH    = 2,
    SAY_SKILL    = 1
};
enum Spells
{
    SPELL_CURSE_OF_BLOOD            = 79345,
    SPELL_CURSE_OF_BLOOD_H          = 92663,
    SPELL_ARCANE_SHIELD             = 79050,
    SPELL_ARCANE_SHIELD_H           = 92667,
    SPELL_SUMMON_GRAVITY_WELL       = 79340,
    SPELL_GRAVITY_WELL_AURA_0       = 79245,
    SPELL_GRAVITY_WELL_AURA_1       = 79244,
    SPELL_GRAVITY_WELL_AURA_1_T     = 79251,
    SPELL_GRAVITY_WELL_AURA_MOD     = 92475,
    SPELL_GRAVITY_WELL_AURA_DMG     = 79249
};

enum Events
{
    EVENT_CURSE_OF_BLOOD    = 1,
    EVENT_SHIELD            = 2,
    EVENT_GRAVITY_WELL      = 3,
    EVENT_GRAVITY_WELL_1    = 4,
    EVENT_SUMMON_ADDS       = 5,
    EVENT_MOVE              = 6,
    EVENT_MOVE_2            = 7
};

enum Adds
{
    NPC_ADVOUT_FOLLOWER     = 42428,
    NPC_GRAVITY_WELL        = 42499
};

Position highpriestessazilAddsPos[2] =
{
    {1263.20f, 966.03f, 205.81f, 0.0f},
    {1278.51f, 1021.72f, 209.08f, 0.0f}
};

Position highpriestessazilStandPos = {1337.79f, 963.39f, 214.18f, 1.8f};

class boss_high_priestess_azil : public CreatureScript
{
    public:
        boss_high_priestess_azil() : CreatureScript("boss_high_priestess_azil") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_high_priestess_azilAI (pCreature);
        }

        struct boss_high_priestess_azilAI : public BossAI
        {
            boss_high_priestess_azilAI(Creature* pCreature) : BossAI(pCreature, DATA_HIGH_PRIESTESS_AZIL), summons(me)
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
            }

            EventMap events;
            SummonList summons;

            void InitializeAI()
            {
                if (!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != sObjectMgr->GetScriptId(TSScriptName))
                    me->IsAIEnabled = false;
                else if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                _Reset();
                    
                summons.DespawnAll();
                events.Reset();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
                switch (summon->GetEntry())
                {
                case NPC_ADVOUT_FOLLOWER:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                    {
                        summon->AddThreat(target, 10.0f);
                        summon->Attack(target, true);
                        summon->GetMotionMaster()->MoveChase(target);
                    }
                    break;
                }
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void EnterCombat(Unit* /*who*/)
            {
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_SHIELD, 35000);
                events.ScheduleEvent(EVENT_CURSE_OF_BLOOD, 3000);
                events.ScheduleEvent(EVENT_SUMMON_ADDS, urand(5000, 7000));
                events.ScheduleEvent(EVENT_GRAVITY_WELL, 10000);
                instance->SetBossState(DATA_HIGH_PRIESTESS_AZIL, IN_PROGRESS);
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                summons.DespawnAll();
                Talk(SAY_DEATH);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type == POINT_MOTION_TYPE)
                {
                    switch (id)
                    {
                    case 1:
                        events.ScheduleEvent(EVENT_MOVE_2, 20000);
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
                    case EVENT_SUMMON_ADDS:
                        for (uint8 i = 0; i < 3; i++)
                            me->SummonCreature(NPC_ADVOUT_FOLLOWER, highpriestessazilAddsPos[urand(0,1)]);
                        events.ScheduleEvent(EVENT_SUMMON_ADDS, urand(5000, 7000));
                        break;
                    case EVENT_CURSE_OF_BLOOD:
                        DoCast(DUNGEON_MODE(SPELL_CURSE_OF_BLOOD, SPELL_CURSE_OF_BLOOD_H));
                        events.ScheduleEvent(EVENT_CURSE_OF_BLOOD, 15000);
                        break;
                    case EVENT_GRAVITY_WELL:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_SUMMON_GRAVITY_WELL);
                        events.ScheduleEvent(EVENT_GRAVITY_WELL, 15000);
                        break;
                    case EVENT_SHIELD:
                        Talk(SAY_SKILL);
                        SetCombatMovement(false);
                        events.CancelEvent(EVENT_CURSE_OF_BLOOD);
                        DoCast(me, DUNGEON_MODE(SPELL_ARCANE_SHIELD, SPELL_ARCANE_SHIELD_H));
                        events.ScheduleEvent(EVENT_MOVE, 2000);
                        break;
                    case EVENT_MOVE:
                        me->GetMotionMaster()->MovePoint(1, highpriestessazilStandPos);
                        break;
                    case EVENT_MOVE_2:
                        me->RemoveAurasDueToSpell(DUNGEON_MODE(SPELL_ARCANE_SHIELD, SPELL_ARCANE_SHIELD_H));
                        SetCombatMovement(true);
                        if (me->getVictim())
                            me->GetMotionMaster()->MoveChase(me->getVictim());
                        events.ScheduleEvent(EVENT_CURSE_OF_BLOOD, 3000);
                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_gravity_well : public CreatureScript
{
    public:
        npc_gravity_well() : CreatureScript("npc_gravity_well") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_gravity_wellAI(pCreature);
        }

        struct npc_gravity_wellAI : public ScriptedAI
        {
            npc_gravity_wellAI(Creature *c) : ScriptedAI(c)
            {
            }

            EventMap events;
            uint32 uidespawnTimer;

            void Reset()
            {
                uidespawnTimer = 20000;
                events.ScheduleEvent(EVENT_GRAVITY_WELL_1, 3000);
                DoCast(me, SPELL_GRAVITY_WELL_AURA_0);
            }

            void UpdateAI(const uint32 diff)
            {
                if (uidespawnTimer <= diff)
                    me->DespawnOrUnsummon();
                else
                    uidespawnTimer -= diff;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                    case EVENT_GRAVITY_WELL_1:
                        me->RemoveAurasDueToSpell(SPELL_GRAVITY_WELL_AURA_0);
                        DoCast(me, SPELL_GRAVITY_WELL_AURA_1);
                        break;
                    }
                }
            }
        };
};

class spell_high_priestess_azil_gravity_well_script: public SpellScriptLoader
{
    public:
        spell_high_priestess_azil_gravity_well_script() : SpellScriptLoader("spell_high_priestess_azil_gravity_well_script") { }


        class spell_high_priestess_azil_gravity_well_script_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_high_priestess_azil_gravity_well_script_SpellScript)


            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (GetCaster() && GetHitUnit())
                {
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_GRAVITY_WELL_AURA_DMG, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_high_priestess_azil_gravity_well_script_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_high_priestess_azil_gravity_well_script_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_high_priestess_azil()
{
    new boss_high_priestess_azil();
    new npc_gravity_well();
    new spell_high_priestess_azil_gravity_well_script();
}
#endif
