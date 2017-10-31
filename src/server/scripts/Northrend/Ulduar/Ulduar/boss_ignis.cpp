////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "ulduar.h"
#include "Vehicle.h"

enum Yells
{
    SAY_AGGRO       = 0,
    SAY_SLAY        = 1,
    SAY_DEATH       = 2,
    SAY_SUMMON      = 3,
    SAY_SLAG_POT    = 4,
    SAY_SCORCH      = 5,
    SAY_BERSERK     = 6,

    EMOTE_JETS      = 7
};

enum Spells
{
    // Ignis
    SPELL_FLAME_JETS            = 62680,
    SPELL_SCORCH                = 62546,
    SPELL_SLAG_POT              = 62717,
    SPELL_SLAG_POT_DAMAGE_N     = 65722,
    SPELL_SLAG_POT_DAMAGE_H     = 65723,
    SPELL_SLAG_IMBUED           = 62836,
    SPELL_ACTIVATE_CONSTRUCT    = 62488,
    SPELL_STRENGHT              = 64473,
    SPELL_GRAB                  = 62707,
    SPELL_BERSERK               = 47008,

    // Iron Construct
    SPELL_FREEZE_ANIM           = 63354,
    SPELL_HEAT                  = 65667,
    SPELL_MOLTEN                = 62373,
    SPELL_BRITTLE_N             = 62382,
    SPELL_BRITTLE_H             = 67114,
    SPELL_SHATTER               = 62383,
    SPELL_GROUND                = 62548
};

#define SPELL_BRITTLE   RAID_MODE(SPELL_BRITTLE_N, SPELL_BRITTLE_H)

enum Events
{
    EVENT_FLAME_JETS    = 1,
    EVENT_SCORCH        = 2,
    EVENT_SLAG_POT      = 3,
    EVENT_GRAB_POT      = 4,
    EVENT_CHANGE_POT    = 5,
    EVENT_END_POT       = 6,
    EVENT_CONSTRUCT     = 7,
    EVENT_BERSERK       = 8
};

enum Actions
{
    ACTION_REMOVE_BUFF  = 20
};

enum AchievementData
{
    DATA_SHATTERED                  = 29252926,

    ACHIEVEMENT_IGNIS_START_EVENT   = 20951
};

#define CONSTRUCT_SPAWN_POINTS 20

Position const ConstructSpawnPosition[CONSTRUCT_SPAWN_POINTS] =
{
    {630.366f, 216.772f, 360.891f, 3.001970f},
    {630.594f, 231.846f, 360.891f, 3.124140f},
    {630.435f, 337.246f, 360.886f, 3.211410f},
    {630.493f, 313.349f, 360.886f, 3.054330f},
    {630.444f, 321.406f, 360.886f, 3.124140f},
    {630.366f, 247.307f, 360.888f, 3.211410f},
    {630.698f, 305.311f, 360.886f, 3.001970f},
    {630.500f, 224.559f, 360.891f, 3.054330f},
    {630.668f, 239.840f, 360.890f, 3.159050f},
    {630.384f, 329.585f, 360.886f, 3.159050f},
    {543.220f, 313.451f, 360.886f, 0.104720f},
    {543.356f, 329.408f, 360.886f, 6.248280f},
    {543.076f, 247.458f, 360.888f, 6.213370f},
    {543.117f, 232.082f, 360.891f, 0.069813f},
    {543.161f, 305.956f, 360.886f, 0.157080f},
    {543.277f, 321.482f, 360.886f, 0.052360f},
    {543.316f, 337.468f, 360.886f, 6.195920f},
    {543.280f, 239.674f, 360.890f, 6.265730f},
    {543.265f, 217.147f, 360.891f, 0.174533f},
    {543.256f, 224.831f, 360.891f, 0.122173f}
};

class boss_ignis : public CreatureScript
{
    public:
        boss_ignis() : CreatureScript("boss_ignis") { }

        struct boss_ignis_AI : public BossAI
        {
            boss_ignis_AI(Creature* creature) : BossAI(creature, BOSS_IGNIS)
            {
                ASSERT(me->GetVehicleKit());
            }

            void Reset()
            {
                _Reset();

                if (Unit* slagPotTarget = ObjectAccessor::GetUnit(*me, _slagPotGUID))
                {
                    slagPotTarget->ExitVehicle();
                    slagPotTarget->RemoveAurasDueToSpell(SPELL_SLAG_POT);
                    _slagPotGUID = 0;
                }

                ClearSummons(false);
                for (uint8 i = 0; i < CONSTRUCT_SPAWN_POINTS; ++i)
                    if (Creature* construct = me->SummonCreature(NPC_IRON_CONSTRUCT, ConstructSpawnPosition[i]))
                        _creatureList.push_back(construct->GetGUID());

                instance->DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEVEMENT_IGNIS_START_EVENT);
            }

            void ClearSummons(bool onBossDie)
            {
                while (!_creatureList.empty())
                {
                    Creature* summon = Unit::GetCreature(*me, *(_creatureList.begin()));
                    if (!summon)
                        _creatureList.erase(_creatureList.begin());
                    else
                    {
                        _creatureList.erase(_creatureList.begin());
                        if (!onBossDie)
                            summon->DespawnOrUnsummon();
                        else
                            me->Kill(summon);
                    }
                }
            }

            void EnterCombat(Unit* /*who*/)
            {
                _EnterCombat();
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_FLAME_JETS, 30000);
                events.ScheduleEvent(EVENT_SCORCH, 25000);
                events.ScheduleEvent(EVENT_SLAG_POT, 35000);
                events.ScheduleEvent(EVENT_CONSTRUCT, 15000);
                events.ScheduleEvent(EVENT_END_POT, 40000);
                events.ScheduleEvent(EVENT_BERSERK, 480000);
                _slagPotGUID = 0;
                _shattered = false;
                _firstConstructKill = 0;
                instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEVEMENT_IGNIS_START_EVENT);
            }

            void JustDied(Unit* /*victim*/)
            {
                if (Unit* slagPotTarget = ObjectAccessor::GetUnit(*me, _slagPotGUID))
                {
                    slagPotTarget->ExitVehicle();
                    slagPotTarget->RemoveAurasDueToSpell(SPELL_SLAG_POT);
                    _slagPotGUID = 0;
                }
                
                ClearSummons(true);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FLAME_JETS);

                _JustDied();
                Talk(SAY_DEATH);
            }

            uint32 GetData(uint32 type)
            {
                if (type == DATA_SHATTERED)
                    return _shattered ? 1 : 0;

                return 0;
            }

            void KilledUnit(Unit* who)
            {
                if (who->IsPlayer())
                    Talk(SAY_SLAY);
            }

            void JustSummoned(Creature* summon)
            {
                if (summon->GetEntry() == NPC_GROUND_SCORCH)
                    summons.Summon(summon);
            }

            void DoAction(const int32 action)
            {
                if (action != ACTION_REMOVE_BUFF)
                    return;

                me->RemoveAuraFromStack(SPELL_STRENGHT);
                // Shattered Achievement
                time_t secondKill = sWorld->GetGameTime();
                if ((secondKill - _firstConstructKill) < 5)
                    _shattered = true;
                _firstConstructKill = secondKill;
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
                        case EVENT_FLAME_JETS:
                            Talk(EMOTE_JETS);
                            DoCast(me, SPELL_FLAME_JETS);
                            events.ScheduleEvent(EVENT_FLAME_JETS, urand(35000, 40000));
                            break;
                        case EVENT_SLAG_POT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, NonTankTargetSelector(me)))
                            {
                                Talk(SAY_SLAG_POT);
                                _slagPotGUID = target->GetGUID();
                                DoCast(target, SPELL_GRAB);
                                events.DelayEvents(3000);
                                events.ScheduleEvent(EVENT_GRAB_POT, 500);
                            }
                            events.ScheduleEvent(EVENT_SLAG_POT, RAID_MODE(30000, 15000));
                            break;
                        case EVENT_GRAB_POT:
                            if (Unit* slagPotTarget = ObjectAccessor::GetUnit(*me, _slagPotGUID))
                            {
                                slagPotTarget->EnterVehicle(me, 0, true);
                                events.ScheduleEvent(EVENT_CHANGE_POT, 1000);
                            }
                            break;
                        case EVENT_CHANGE_POT:
                            if (Unit* slagPotTarget = ObjectAccessor::GetUnit(*me, _slagPotGUID))
                            {
                                slagPotTarget->ChangeSeat(1);
                                DoCast(slagPotTarget, SPELL_SLAG_POT);
                                events.ScheduleEvent(EVENT_END_POT, 10000);
                            }
                            break;
                        case EVENT_END_POT:
                            if (Unit* slagPotTarget = ObjectAccessor::GetUnit(*me, _slagPotGUID))
                            {
                                slagPotTarget->ExitVehicle();
                                slagPotTarget = NULL;
                                _slagPotGUID = 0;
                            }
                            break;
                        case EVENT_SCORCH:
                            Talk(SAY_SCORCH);
                            if (Unit* target = me->getVictim())
                                me->SummonCreature(NPC_GROUND_SCORCH, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 45000);
                            DoCast(SPELL_SCORCH);
                            events.ScheduleEvent(EVENT_SCORCH, 25000);
                            break;
                        case EVENT_CONSTRUCT:
                            Talk(SAY_SUMMON);
                            if (!_creatureList.empty())
                            {
                                std::list<uint64>::iterator itr = _creatureList.begin();
                                std::advance(itr, urand(0, _creatureList.size() - 1));
                                if (Creature* construct = Unit::GetCreature(*me, *itr))
                                {
                                    construct->RemoveAurasDueToSpell(SPELL_FREEZE_ANIM);
                                    construct->SetReactState(REACT_AGGRESSIVE);
                                    construct->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_STUNNED | UNIT_FLAG_DISABLE_MOVE);
                                    construct->AI()->AttackStart(me->getVictim());
                                    construct->AI()->DoZoneInCombat();
                                    DoCast(me, SPELL_STRENGHT, true);
                                    _creatureList.erase(itr);
                                    summons.Summon(construct);
                                }
                            }
                            DoCast(me, SPELL_ACTIVATE_CONSTRUCT);
                            events.ScheduleEvent(EVENT_CONSTRUCT, RAID_MODE(40000, 30000));
                            break;
                        case EVENT_BERSERK:
                            Talk(SAY_BERSERK);
                            DoCast(me, SPELL_BERSERK, true);
                            break;
                    }
                }

                DoMeleeAttackIfReady();

                EnterEvadeIfOutOfCombatArea(diff);
            }

        private:
            uint64 _slagPotGUID;
            time_t _firstConstructKill;
            bool _shattered;
            std::list<uint64> _creatureList;

        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetUlduarAI<boss_ignis_AI>(creature);
        }
};

class npc_iron_construct : public CreatureScript
{
    public:
        npc_iron_construct() : CreatureScript("npc_iron_construct") { }

        struct npc_iron_constructAI : public ScriptedAI
        {
            npc_iron_constructAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript())
            {
                me->AddAura(SPELL_FREEZE_ANIM, me);
                creature->SetReactState(REACT_PASSIVE);
                needDamage = Is25ManRaid() ? 3000 : 5000;
                shattered = false;
            }

            void Reset()
            {
                _brittled = false;
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (me->HasAura(SPELL_BRITTLE) && damage >= needDamage && !shattered)
                {
                    shattered = true;
                    DoCastAOE(SPELL_SHATTER, true);
                    if (Creature* ignis = ObjectAccessor::GetCreature(*me, _instance->GetData64(BOSS_IGNIS)))
                        if (ignis->AI())
                            ignis->AI()->DoAction(ACTION_REMOVE_BUFF);

                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->DespawnOrUnsummon(1000);
                }
            }
            
            void JustReachedHome()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_STUNNED | UNIT_FLAG_DISABLE_MOVE);
                DoCast(me, SPELL_FREEZE_ANIM, true);
            }

            void UpdateAI(const uint32 /*uiDiff*/)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasAura(SPELL_MOLTEN) && me->HasAura(SPELL_HEAT))
                    me->RemoveAura(SPELL_HEAT);

                if (Aura* aur = me->GetAura(SPELL_HEAT))
                {
                    if (aur->GetStackAmount() >= 10)
                    {
                        me->RemoveAura(SPELL_HEAT);
                        DoCast(SPELL_MOLTEN);
                        _brittled = false;
                    }
                }

                // Water pools
                if (me->IsInWater() && !_brittled && me->HasAura(SPELL_MOLTEN))
                {
                    DoCast(SPELL_BRITTLE);
                    me->RemoveAura(SPELL_MOLTEN);
                    _brittled = true;
                }

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* _instance;
            bool _brittled;
            uint32 needDamage;
            bool shattered;

        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetUlduarAI<npc_iron_constructAI>(creature);
        }
};

class npc_scorch_ground : public CreatureScript
{
    public:
        npc_scorch_ground() : CreatureScript("npc_scorch_ground") { }

        struct npc_scorch_groundAI : public ScriptedAI
        {
            npc_scorch_groundAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE |UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
                creature->SetDisplayId(16925); //model 2 in db cannot overwrite wdb fields
            }

            void MoveInLineOfSight(Unit* unit)
            {
                if (!_heat)
                {
                    if (unit->GetEntry() == NPC_IRON_CONSTRUCT)
                    {
                        if (!unit->HasAura(SPELL_HEAT) || !unit->HasAura(SPELL_MOLTEN))
                        {
                            _constructGUID = unit->GetGUID();
                            _heat = true;
                        }
                    }
                }
            }

            void Reset()
            {
                _heat = false;
                DoCast(me, SPELL_GROUND);
                _constructGUID = 0;
                _heatTimer = 0;
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (_heat)
                {
                    if (_heatTimer <= uiDiff)
                    {
                        Creature* construct = me->GetCreature(*me, _constructGUID);
                        if (construct && !construct->HasAura(SPELL_MOLTEN))
                        {
                            me->AddAura(SPELL_HEAT, construct);
                            _heatTimer = 1000;
                        }
                    }
                    else
                        _heatTimer -= uiDiff;
                }
            }

        private:
            uint64 _constructGUID;
            uint32 _heatTimer;
            bool _heat;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetUlduarAI<npc_scorch_groundAI>(creature);
        }
};

class spell_ignis_slag_pot: public SpellScriptLoader
{
    public:
        spell_ignis_slag_pot() : SpellScriptLoader("spell_ignis_slag_pot") { }

        class spell_ignis_slag_pot_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ignis_slag_pot_AuraScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SLAG_POT_DAMAGE_N))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_SLAG_POT_DAMAGE_H))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_SLAG_IMBUED))
                    return false;
                return true;
            }

            void HandleEffectPeriodic(AuraEffect const* aurEff)
            {
                Unit* aurEffCaster = aurEff->GetCaster();
                if (!aurEffCaster)
                    return;

                uint32 spellId = GetSpellInfo()->Id;
                if (Unit* target = GetTarget())
                    aurEffCaster->CastSpell(target, spellId == SPELL_SLAG_POT ? SPELL_SLAG_POT_DAMAGE_N : SPELL_SLAG_POT_DAMAGE_H, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget()->isAlive())
                    GetTarget()->CastSpell(GetTarget(), SPELL_SLAG_IMBUED, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_ignis_slag_pot_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                AfterEffectRemove += AuraEffectRemoveFn(spell_ignis_slag_pot_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_ignis_slag_pot_AuraScript();
        }
};

class achievement_ignis_shattered : public AchievementCriteriaScript
{
    public:
        achievement_ignis_shattered() : AchievementCriteriaScript("achievement_ignis_shattered") { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (target && target->IsAIEnabled)
                return target->GetAI()->GetData(DATA_SHATTERED);

            return false;
        }
};

class achievement_ignis_hot_pocket : public AchievementCriteriaScript
{
    public:
        achievement_ignis_hot_pocket() : AchievementCriteriaScript("achievement_ignis_hot_pocket") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            return (source && source->isAlive());
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_ignis()
{
    new boss_ignis();
    new npc_iron_construct();
    new npc_scorch_ground();
    new spell_ignis_slag_pot();
    new achievement_ignis_shattered();
    new achievement_ignis_hot_pocket();

    if (VehicleSeatEntry* vehSeat = const_cast<VehicleSeatEntry*>(sVehicleSeatStore.LookupEntry(3206)))
        vehSeat->m_flags |= 0x400;
}
#endif
#undef SPELL_BRITTLE
