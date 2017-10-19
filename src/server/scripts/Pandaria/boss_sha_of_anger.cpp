////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"

enum eBosses
{
    BOSS_SHA_OF_ANGER
};

enum eSpells
{
    SPELL_SHADOW_BOLT_ANGER     = 119487,
    SPELL_OVERCOME_BY_ANGER     = 129356,
    SPELL_ENDLESS_RAGE          = 119446,
    SPELL_BITTER_THOUGHTS       = 119601,
    SPELL_BERSERK               = 47008,
    SPELL_DOMINATE_MIND_WARNING = 119622,
    SPELL_DOMINATE_MIND         = 119626,
    SPELL_SEETHE_AURA           = 119487,
    SPELL_SHA_OF_ANGER_BONUS    = 132205
};

enum eEvents
{
    EVENT_GROWING_ANGER_WARNING = 1,
    EVENT_GROWING_ANGER         = 2,
    EVENT_UNLEASHED_WRATH       = 3,
    EVENT_BERSERK               = 4,
    EVENT_DESPAWN               = 5,
    EVENT_SPAWN                 = 6,
    EVENT_UPDATE_RAGE           = 7,
    EVENT_RANGE_ATTACK          = 8
};

enum eCreatures
{
    CREATURE_SHA_OF_ANGER           = 56439
};

enum eTalk
{
    TALK_INTRO = 0,
    TALK_ANGER = 1,
    TALK_SPAWN = 2,
    TALK_RESET = 3,
    TALK_SLAY  = 4,
    TALK_AGGRO = 5
};

class boss_sha_of_anger : public CreatureScript
{
    public:
        boss_sha_of_anger() : CreatureScript("boss_sha_of_anger") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_sha_of_anger_AI(creature);
        }

        struct boss_sha_of_anger_AI : public ScriptedAI
        {
            boss_sha_of_anger_AI(Creature* creature) : ScriptedAI(creature) { }

            int _targetCount;
            int _cloudCount;
            int _maxTargetCount;
            uint8 _dominateMindCount;
            uint32 timer;
            bool phase1;
            bool range;

            std::list<uint64> targetedDominationPlayerGuids;
            std::list<uint64> m_LootersGuids;

            void Reset()
            {
                me->setPowerType(POWER_RAGE);
                me->SetPower(POWER_RAGE, 0);

                me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 5.0f);

                summons.DespawnAll();

                phase1 = true;
                range = false;
                _dominateMindCount = 2;
                _cloudCount = 3;
                _targetCount = 0;
                _maxTargetCount = 5;
                timer = 0;

                Talk(TALK_RESET);

                events.Reset();
                events.ScheduleEvent(EVENT_GROWING_ANGER_WARNING, 19000);
                events.ScheduleEvent(EVENT_SPAWN, 5000);
                events.ScheduleEvent(EVENT_UNLEASHED_WRATH, 52000);
                events.ScheduleEvent(EVENT_BERSERK, 900000);
                events.ScheduleEvent(EVENT_UPDATE_RAGE, 1000);

                targetedDominationPlayerGuids.clear();
                m_LootersGuids.clear();

                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, me, 100.0f);

                for (auto itr : playerList)
                    itr->RemoveAura(SPELL_DOMINATE_MIND);
            }

            void KilledUnit(Unit* u)
            {
                if (u->IsPlayer())
                    Talk(TALK_SLAY);
            }

            void EnterCombat(Unit* /*p_Unit*/)
            {
                Talk(TALK_AGGRO);
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);

                // Clouds
                if (summon->GetEntry() == 61523)
                {
                    summon->CastSpell(summon, SPELL_BITTER_THOUGHTS, true);
                    summon->DespawnOrUnsummon(60000);
                }
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (p_Damage >= me->GetHealth())
                {
                    std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                    for (std::list<HostileReference*>::const_iterator l_Itr = l_ThreatList.begin(); l_Itr != l_ThreatList.end(); ++l_Itr)
                    {
                        if (Player* l_Player = Player::GetPlayer(*me, (*l_Itr)->getUnitGuid()))
                            m_LootersGuids.push_back(l_Player->GetGUID());
                    }
                }
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                for (uint64 l_Guid : m_LootersGuids)
                {
                    if (Player* l_Player = Player::GetPlayer(*me, l_Guid))
                        me->CastSpell(l_Player, SPELL_SHA_OF_ANGER_BONUS, true);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_UNLEASHED_WRATH:
                        {
                            phase1 = false;

                            for (uint8 i = 0; i < 5; i++)
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                {
                                    if (target->GetAuraCount(SPELL_SEETHE_AURA) < 3)
                                    {
                                        me->CastSpell(target, SPELL_SHADOW_BOLT_ANGER, false);
                                        me->AddAura(SPELL_SEETHE_AURA, target);
                                    }
                                }
                            }

                            if (_targetCount < _maxTargetCount)
                            {
                                if (_targetCount == 0)
                                    Talk(TALK_INTRO);

                                _targetCount++;
                                events.ScheduleEvent(EVENT_UNLEASHED_WRATH, 2000);
                            }
                            else
                            {
                                events.ScheduleEvent(EVENT_UNLEASHED_WRATH, 50000);
                                phase1 = true;
                                _targetCount = 0;
                            }

                            break;
                        }
                        case EVENT_GROWING_ANGER_WARNING:
                        {
                            Talk(TALK_ANGER);

                            for (uint8 i = 0; i < _dominateMindCount; ++i)
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                {
                                    targetedDominationPlayerGuids.push_back(target->GetGUID());
                                    me->CastSpell(target, SPELL_DOMINATE_MIND_WARNING, true);
                                }

                            events.ScheduleEvent(EVENT_GROWING_ANGER, 6000);
                            break;
                        }
                        case EVENT_GROWING_ANGER:
                        {
                            if (!targetedDominationPlayerGuids.empty())
                                for (auto guid : targetedDominationPlayerGuids)
                                    if (Player* target = ObjectAccessor::GetPlayer(*me, guid))
                                        if (!me->getVictim() || target != me->getVictim())
                                            me->CastSpell(target, SPELL_DOMINATE_MIND, false);

                            events.ScheduleEvent(EVENT_GROWING_ANGER_WARNING, 19000);
                            break;
                        }
                        case EVENT_SPAWN:
                        {
                            Talk(TALK_SPAWN);

                            for (uint8 i = 0; i < _cloudCount; ++i)
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                    me->CastSpell(target, SPELL_ENDLESS_RAGE, false);

                            events.ScheduleEvent(EVENT_SPAWN, 15000);
                            break;
                        }
                        case EVENT_UPDATE_RAGE:
                        {
                            if (phase1)
                                timer = timer + 20;
                            else
                                timer = timer - 20;

                            me->SetPower(POWER_RAGE, timer);
                            events.ScheduleEvent(EVENT_UPDATE_RAGE, 1000);
                            break;
                        }
                        case EVENT_BERSERK:
                        {
                            me->CastSpell(me, SPELL_BERSERK, false);
                            break;
                        }
                        case EVENT_RANGE_ATTACK:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            {
                                me->CastSpell(target, SPELL_SHADOW_BOLT_ANGER, false);
                                me->AddAura(SPELL_SEETHE_AURA, target);
                            }

                            range = false;
                            break;
                        }
                        default:
                            break;
                    }
                }

                if (me->getVictim() && !me->IsWithinMeleeRange(me->getVictim()) && !range)
                {
                    range = true;
                    events.ScheduleEvent(EVENT_RANGE_ATTACK, 2000);
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_sha_of_anger_bunny : public CreatureScript
{
    public:
        mob_sha_of_anger_bunny() : CreatureScript("mob_sha_of_anger_bunny") {}

        struct mob_sha_of_anger_bunnyAI : public ScriptedAI
        {
            mob_sha_of_anger_bunnyAI(Creature* creature) : ScriptedAI(creature)
            {
                creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                creature->GetMotionMaster()->Clear();
            }

            void Reset()
            {
                me->GetMotionMaster()->Clear();
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                if (me->AI())
                    me->AI()->SetCanSeeEvenInPassiveMode(true);
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (who->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (!who->IsWithinDist(me, 30.0f))
                    return;

                if (who->IsWithinDist(me, 20.0f))
                    who->AddAura(SPELL_OVERCOME_BY_ANGER, who);
                else if (who->HasAura(SPELL_OVERCOME_BY_ANGER))
                    who->RemoveAura(SPELL_OVERCOME_BY_ANGER);
            }

            void UpdateAI(uint32 const /*p_Diff*/)
            {
                if (me->AI() && !me->AI()->CanSeeEvenInPassiveMode())
                    me->AI()->SetCanSeeEvenInPassiveMode(true);

                if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE))
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                if (me->IsMoving())
                    me->StopMoving();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sha_of_anger_bunnyAI(creature);
        }
};

class spell_sha_of_anger_aggressive_behaviour: public SpellScriptLoader
{
    public:
        spell_sha_of_anger_aggressive_behaviour() : SpellScriptLoader("spell_sha_of_anger_aggressive_behaviour") { }

        class spell_sha_of_anger_aggressive_behaviour_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_of_anger_aggressive_behaviour_AuraScript);

            void HandlePeriodicTick(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                if (Unit* target = GetTarget())
                    if (target->GetHealthPct() < 50.0f)
                        this->Remove(AURA_REMOVE_BY_DEFAULT);
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                {
                    if (!target->ToPlayer())
                        return;

                    target->SetPvP(true);
                    target->setFaction(16);
                    target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
                }

            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                {
                    target->RestoreFaction();
                    target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_of_anger_aggressive_behaviour_AuraScript::HandlePeriodicTick, EFFECT_5, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectApply += AuraEffectApplyFn(spell_sha_of_anger_aggressive_behaviour_AuraScript::OnApply, EFFECT_5, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_sha_of_anger_aggressive_behaviour_AuraScript::OnRemove, EFFECT_5, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_of_anger_aggressive_behaviour_AuraScript();
        }
};

// Overcome by Anger - 129356
 class spell_sha_of_anger_overcome_by_anger: public SpellScriptLoader
 {
    public:
        spell_sha_of_anger_overcome_by_anger() : SpellScriptLoader("spell_sha_of_anger_overcome_by_anger") { }
 
        class spell_sha_of_anger_overcome_by_anger_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_of_anger_overcome_by_anger_AuraScript);
 
            enum eNpcs
            {
                ShaOfAngerBunny = 35114
            };

            void OnUpdate(uint32 /*p_Diff*/)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Radius = 20.0f;
                float l_ShaOfAngerBunnyPresent = false;

                std::list<Unit*> l_UnitList;
                JadeCore::AnyUnitInObjectRangeCheck u_check(l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck> searcher(l_Caster, l_UnitList, u_check);
                l_Caster->VisitNearbyObject(l_Radius, searcher);

                for (Unit* l_Target : l_UnitList)
                {
                    if (l_Target->GetEntry() == eNpcs::ShaOfAngerBunny)
                        l_ShaOfAngerBunnyPresent = true;
                }

                if (!l_ShaOfAngerBunnyPresent)
                    l_Caster->RemoveAura(SPELL_OVERCOME_BY_ANGER);
            }
 
            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_sha_of_anger_overcome_by_anger_AuraScript::OnUpdate);
            }
        };
 
        AuraScript* GetAuraScript() const override
        {
            return new spell_sha_of_anger_overcome_by_anger_AuraScript();
        }
 };

#ifndef __clang_analyzer__
void AddSC_boss_sha_of_anger()
{
    new boss_sha_of_anger();
    new mob_sha_of_anger_bunny();
    new spell_sha_of_anger_aggressive_behaviour();
    new spell_sha_of_anger_overcome_by_anger();
}
#endif
