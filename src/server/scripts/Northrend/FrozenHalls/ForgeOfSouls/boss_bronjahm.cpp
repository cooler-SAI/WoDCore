////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "SpellAuraEffects.h"
#include "forge_of_souls.h"

#define DATA_SOUL_POWER     1

enum Yells
{
    SAY_AGGRO           = -1632001,
    SAY_SLAY_1          = -1632002,
    SAY_SLAY_2          = -1632003,
    SAY_DEATH           = -1632004,
    SAY_SOUL_STORM      = -1632005,
    SAY_CORRUPT_SOUL    = -1632006
};

enum Spells
{
    SPELL_MAGIC_S_BANE          = 68793,
    SPELL_SHADOW_BOLT           = 70043,
    SPELL_CORRUPT_SOUL          = 68839,
    SPELL_CONSUME_SOUL          = 68861,
    SPELL_TELEPORT              = 68988,
    SPELL_FEAR                  = 68950,
    SPELL_SOULSTORM             = 68872,
    SPELL_SOULSTORM_CHANNEL     = 69008,    // pre-fight
    SPELL_SOULSTORM_VISUAL      = 68870,    // pre-cast soulstorm
    SPELL_PURPLE_BANISH_VISUAL  = 68862     // Used by Soul Fragment (Aura)
};

enum Events
{
    EVENT_MAGIC_BANE    = 1,
    EVENT_SHADOW_BOLT   = 2,
    EVENT_CORRUPT_SOUL  = 3,
    EVENT_SOULSTORM     = 4,
    EVENT_FEAR          = 5
};

enum CombatPhases
{
    PHASE_1 = 1,
    PHASE_2 = 2
};

enum Actions
{
    ACTION_SUMMON_DIE   = 1
};

class boss_bronjahm : public CreatureScript
{
    public:
        boss_bronjahm() : CreatureScript("boss_bronjahm") { }

        struct boss_bronjahmAI : public BossAI
        {
            boss_bronjahmAI(Creature* creature) : BossAI(creature, DATA_BRONJAHM)
            {
                DoCast(me, SPELL_SOULSTORM_CHANNEL, true);
            }

            void InitializeAI()
            {
                if (!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != sObjectMgr->GetScriptId(FoSScriptName))
                    me->IsAIEnabled = false;
                else if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                events.Reset();
                events.SetPhase(PHASE_1);
                events.ScheduleEvent(EVENT_SHADOW_BOLT, 2000);
                events.ScheduleEvent(EVENT_MAGIC_BANE, urand(8000, 20000));
                events.ScheduleEvent(EVENT_CORRUPT_SOUL, urand(25000, 35000), 0, PHASE_1);

                instance->SetBossState(DATA_BRONJAHM, NOT_STARTED);
                soulCount = 0;
            }

           void JustReachedHome()
           {
               DoCast(me, SPELL_SOULSTORM_CHANNEL, true);
           }

            void EnterCombat(Unit* /*who*/)
            {
                DoScriptText(SAY_AGGRO, me);
                me->RemoveAurasDueToSpell(SPELL_SOULSTORM_CHANNEL);

                instance->SetBossState(DATA_BRONJAHM, IN_PROGRESS);
            }

            void JustDied(Unit* /*killer*/)
            {
                DoScriptText(SAY_DEATH, me);

                instance->SetBossState(DATA_BRONJAHM, DONE);
            }

            void KilledUnit(Unit* who)
            {
                if (who->IsPlayer())
                    DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (events.GetPhaseMask() & (1 << PHASE_1) && !HealthAbovePct(30))
                {
                    events.SetPhase(PHASE_2);
                    me->InterruptNonMeleeSpells(true);
                    DoCast(me, SPELL_TELEPORT);
                    events.RescheduleEvent(EVENT_SHADOW_BOLT, 5000);
                    events.ScheduleEvent(EVENT_FEAR, urand(12000, 16000), 0, PHASE_2);
                    events.ScheduleEvent(EVENT_SOULSTORM, 100, 0, PHASE_2);
                }
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
                summon->SetReactState(REACT_PASSIVE);
                summon->GetMotionMaster()->Clear();
                summon->GetMotionMaster()->MoveFollow(me, me->GetObjectSize(), 0.0f);
                summon->CastSpell(summon, SPELL_PURPLE_BANISH_VISUAL, true);
                soulCount++;
            }

            void SummonedCreatureDies(Creature* /*summon*/, Unit* /*killer*/)
            {
                if (soulCount > 0)
                    soulCount--;
            }

            void DoAction(const int32 param)
            {
                if (!me->isAlive())
                    return;

                if (param == ACTION_SUMMON_DIE)
                    soulCount--;
            }

            uint32 GetData(uint32 type)
            {
                if (type == DATA_SOUL_POWER)
                    return (soulCount >= 4) ? 1 : 0;

                return 0;
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
                        case EVENT_MAGIC_BANE:
                            DoCastVictim(SPELL_MAGIC_S_BANE);
                            events.ScheduleEvent(EVENT_MAGIC_BANE, urand(8000, 20000));
                            break;
                        case EVENT_SHADOW_BOLT:
                            if (!me->IsWithinMeleeRange(me->getVictim()))
                                DoCastVictim(SPELL_SHADOW_BOLT);
                            events.ScheduleEvent(EVENT_SHADOW_BOLT, 2000);
                            break;
                        case EVENT_CORRUPT_SOUL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                            {
                                DoScriptText(SAY_CORRUPT_SOUL, me);
                                DoCast(target, SPELL_CORRUPT_SOUL);
                            }
                            events.ScheduleEvent(EVENT_CORRUPT_SOUL, urand(25000, 35000), 0, PHASE_1);
                            break;
                        case EVENT_SOULSTORM:
                            DoScriptText(SAY_SOUL_STORM, me);
                            me->CastSpell(me, SPELL_SOULSTORM_VISUAL, true);
                            me->CastSpell(me, SPELL_SOULSTORM, false);
                            break;
                        case EVENT_FEAR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                                me->CastCustomSpell(SPELL_FEAR, SPELLVALUE_MAX_TARGETS, 1, target, false);
                            events.ScheduleEvent(EVENT_FEAR, urand(8000, 12000), 0, PHASE_2);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        private:
            uint16 soulCount;
        };
        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_bronjahmAI(creature);
        }
};

class mob_corrupted_soul_fragment : public CreatureScript
{
    public:
        mob_corrupted_soul_fragment() : CreatureScript("mob_corrupted_soul_fragment") { }

        struct mob_corrupted_soul_fragmentAI : public ScriptedAI
        {
            mob_corrupted_soul_fragmentAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = me->GetInstanceScript();
            }

            void MovementInform(uint32 /*type*/, uint32 id)
            {
                if (instance)
                {
                    if (TempSummon* summ = me->ToTempSummon())
                    {
                        uint64 BronjahmGUID = instance->GetData64(DATA_BRONJAHM);
                        if (GUID_LOPART(BronjahmGUID) != id)
                            return;

                        if (Creature* bronjahm = ObjectAccessor::GetCreature(*me, BronjahmGUID))
                        {
                            if (me->GetDistance2d(bronjahm->GetPositionX(), bronjahm->GetPositionY()) <= 2.0f)
                            {
                                me->CastSpell(bronjahm, SPELL_CONSUME_SOUL, true);
                                bronjahm->AI()->DoAction(ACTION_SUMMON_DIE);
                                summ->UnSummon();
                            }
                        }
                    }
                }
            }

        private:
            InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_corrupted_soul_fragmentAI(creature);
        }
};

class spell_bronjahm_magic_bane: public SpellScriptLoader
{
    public:
        spell_bronjahm_magic_bane() :  SpellScriptLoader("spell_bronjahm_magic_bane") { }

        class spell_bronjahm_magic_bane_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_bronjahm_magic_bane_SpellScript)

            void RecalculateDamage()
            {
                if (GetHitUnit()->getPowerType() != POWER_MANA)
                    return;

                const int32 maxDamage = GetCaster()->GetMap()->GetSpawnMode() == 1 ? 15000 : 10000;
                int32 newDamage = GetHitDamage();
                newDamage += GetHitUnit()->GetMaxPower(POWER_MANA)/2;
                newDamage = std::min<int32>(maxDamage, newDamage);

                SetHitDamage(newDamage);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_bronjahm_magic_bane_SpellScript::RecalculateDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_bronjahm_magic_bane_SpellScript();
        }
};

class spell_bronjahm_consume_soul: public SpellScriptLoader
{
    public:
        spell_bronjahm_consume_soul() :  SpellScriptLoader("spell_bronjahm_consume_soul") { }

        class spell_bronjahm_consume_soul_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_bronjahm_consume_soul_SpellScript)

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetHitUnit()->CastSpell(GetHitUnit(), GetEffectValue(), true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_bronjahm_consume_soul_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_bronjahm_consume_soul_SpellScript();
        }
};

class spell_bronjahm_soulstorm_channel: public SpellScriptLoader
{
    public:
        spell_bronjahm_soulstorm_channel() : SpellScriptLoader("spell_bronjahm_soulstorm_channel") { }

        class spell_bronjahm_soulstorm_channel_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_bronjahm_soulstorm_channel_AuraScript)

            void HandlePeriodicTick(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                for (uint32 i = 68904; i <= 68907; ++i)
                    GetTarget()->CastSpell(GetTarget(), i, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_bronjahm_soulstorm_channel_AuraScript::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_bronjahm_soulstorm_channel_AuraScript();
        }
};

class spell_bronjahm_soulstorm_visual: public SpellScriptLoader
{
    public:
        spell_bronjahm_soulstorm_visual() : SpellScriptLoader("spell_bronjahm_soulstorm_visual") { }

        class spell_bronjahm_soulstorm_visual_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_bronjahm_soulstorm_visual_AuraScript)

            void HandlePeriodicTick(AuraEffect const* aurEff)
            {
                PreventDefaultAction();
                if (aurEff->GetTickNumber()%5)
                    return;
                GetTarget()->CastSpell(GetTarget(), 68886, true);
                for (uint32 i = 68896; i <= 68898; ++i)
                    GetTarget()->CastSpell(GetTarget(), i, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_bronjahm_soulstorm_visual_AuraScript::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_bronjahm_soulstorm_visual_AuraScript();
        }
};

class DistanceCheck
{
    public:
        explicit DistanceCheck(Unit* _caster) : caster(_caster) { }

        bool operator() (WorldObject* unit) const
        {
            if (caster->GetExactDist2d(unit) <= 10.0f)
                return true;
            return false;
        }

        Unit* caster;
};

class spell_bronjahm_soulstorm_targeting: public SpellScriptLoader
{
    public:
        spell_bronjahm_soulstorm_targeting() : SpellScriptLoader("spell_bronjahm_soulstorm_targeting") { }

        class spell_bronjahm_soulstorm_targeting_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_bronjahm_soulstorm_targeting_SpellScript)

            void FilterTargetsInitial(std::list<WorldObject*>& targets)
            {
                targets.remove_if (DistanceCheck(GetCaster()));
                sharedTargets = targets;
            }

            // use the same target for first and second effect
            void FilterTargetsSubsequent(std::list<WorldObject*>& targets)
            {
                targets = sharedTargets;
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_bronjahm_soulstorm_targeting_SpellScript::FilterTargetsInitial, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_bronjahm_soulstorm_targeting_SpellScript::FilterTargetsSubsequent, EFFECT_2, TARGET_UNIT_DEST_AREA_ENEMY);
            }

            std::list<WorldObject*> sharedTargets;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_bronjahm_soulstorm_targeting_SpellScript();
        }
};

class achievement_soul_power : public AchievementCriteriaScript
{
    public:
        achievement_soul_power() : AchievementCriteriaScript("achievement_soul_power") { }

        bool OnCheck(Player* /*player*/, Unit* target)
        {
            if (!target)
                return false;

            if (Creature* bronjahm = target->ToCreature())
                if (bronjahm->AI()->GetData(DATA_SOUL_POWER))
                    return true;

            return false;
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_bronjahm()
{
    new boss_bronjahm();
    new mob_corrupted_soul_fragment();
    new spell_bronjahm_magic_bane();
    new spell_bronjahm_consume_soul();
    new spell_bronjahm_soulstorm_channel();
    new spell_bronjahm_soulstorm_visual();
    new spell_bronjahm_soulstorm_targeting();
    new achievement_soul_power();
}
#endif
