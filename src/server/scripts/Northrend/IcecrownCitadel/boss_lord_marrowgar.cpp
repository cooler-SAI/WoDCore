////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuras.h"
#include "Vehicle.h"
#include "MapManager.h"
#include "MoveSplineInit.h"
#include "icecrown_citadel.h"

enum ScriptTexts
{
    SAY_ENTER_ZONE              = 0,
    SAY_AGGRO                   = 1,
    SAY_BONE_STORM              = 2,
    SAY_BONESPIKE               = 3,
    SAY_KILL                    = 4,
    SAY_DEATH                   = 5,
    SAY_BERSERK                 = 6,
    EMOTE_BONE_STORM            = 7
};

enum Spells
{
    // Lord Marrowgar
    SPELL_BONE_SLICE            = 69055,
    SPELL_BONE_STORM            = 69076,
    SPELL_BONE_SPIKE_GRAVEYARD  = 69057,
    SPELL_COLDFLAME_NORMAL      = 69140,
    SPELL_TANK_MARKER           = 71038,
    SPELL_COLDFLAME_BONE_STORM  = 72705,

    // Bone Spike
    SPELL_IMPALED               = 69065,
    SPELL_RIDE_VEHICLE          = 46598,

    // Coldflame
    SPELL_COLDFLAME_PASSIVE     = 69145,
    SPELL_COLDFLAME_SUMMON      = 69147
};

uint32 const BoneSpikeSummonId[3] = {69062, 72669, 72670};

enum Events
{
    EVENT_BONE_SPIKE_GRAVEYARD          = 1,
    EVENT_COLDFLAME                     = 2,
    EVENT_BONE_STORM_BEGIN              = 3,
    EVENT_BONE_STORM_MOVE               = 4,
    EVENT_BONE_STORM_END                = 5,
    EVENT_ENABLE_BONE_SLICE             = 6,
    EVENT_BERSERK                       = 7,
    EVENT_WARN_BONE_STORM               = 8,

    EVENT_COLDFLAME_TRIGGER             = 9,
    EVENT_FAIL_BONED                    = 10,
    EVENT_CHECK_PLAYERS                 = 11,
    EVENT_PREPARE_BONE_SPIKE_GRAVEYARD  = 12,

    EVENT_GROUP_SPECIAL                 = 1
};

enum MovementPoints
{
    POINT_TARGET_BONESTORM_PLAYER   = 36612631,
    POINT_TARGET_COLDFLAME          = 36672631
};

#define DATA_COLDFLAME_GUID 0

class boss_lord_marrowgar : public CreatureScript
{
    public:
        boss_lord_marrowgar() : CreatureScript("boss_lord_marrowgar") { }

        struct boss_lord_marrowgarAI : public BossAI
        {
            boss_lord_marrowgarAI(Creature* creature) : BossAI(creature, DATA_LORD_MARROWGAR)
            {
                _boneStormDuration = RAID_MODE<uint32>(20000, 30000, 20000, 30000);
                _baseSpeed = creature->GetSpeedRate(MOVE_RUN);
                _coldflameLastPos.Relocate(creature);
                _introDone = false;
                _boneSlice = false;
            }

            void Reset()
            {
                _Reset();
                me->SetSpeed(MOVE_RUN, _baseSpeed, true);
                me->RemoveAurasDueToSpell(SPELL_BONE_STORM);
                me->RemoveAurasDueToSpell(SPELL_BERSERK);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, false);
                events.ScheduleEvent(EVENT_CHECK_PLAYERS, 2000);
                events.ScheduleEvent(EVENT_ENABLE_BONE_SLICE, 10000);
                events.ScheduleEvent(EVENT_PREPARE_BONE_SPIKE_GRAVEYARD, 10000, EVENT_GROUP_SPECIAL);
                events.ScheduleEvent(EVENT_BONE_SPIKE_GRAVEYARD, 15000, EVENT_GROUP_SPECIAL);
                events.ScheduleEvent(EVENT_COLDFLAME, 5000, EVENT_GROUP_SPECIAL);
                events.ScheduleEvent(EVENT_WARN_BONE_STORM, 45000);
                events.ScheduleEvent(EVENT_BERSERK, 600000);
                _boneSlice = false;
            }

            void EnterCombat(Unit* /*who*/)
            {
                Talk(SAY_AGGRO);

                me->setActive(true);
                DoZoneInCombat();
                instance->SetBossState(DATA_LORD_MARROWGAR, IN_PROGRESS);
            }

            void JustDied(Unit* /*killer*/)
            {
                Talk(SAY_DEATH);

                _JustDied();
            }

            void JustReachedHome()
            {
                _JustReachedHome();
                instance->SetBossState(DATA_LORD_MARROWGAR, FAIL);
                instance->SetData(DATA_BONED_ACHIEVEMENT, uint32(true));    // reset
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->IsPlayer())
                    Talk(SAY_KILL);
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!_introDone && me->IsWithinDistInMap(who, 70.0f))
                {
                    Talk(SAY_ENTER_ZONE);
                    _introDone = true;
                }

                BossAI::MoveInLineOfSight(who);
            }
            
            void SpellHitTarget(Unit* target, SpellInfo const* spell)
            {
                if (spell->Id == SPELL_BONE_SLICE)
                    me->AddAura(SPELL_TANK_MARKER, target);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || !CheckInRoom())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK_PLAYERS:
                            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TANK_MARKER);
                            events.ScheduleEvent(EVENT_CHECK_PLAYERS, 2000);
                            break;
                        case EVENT_PREPARE_BONE_SPIKE_GRAVEYARD:
                            events.CancelEvent(EVENT_CHECK_PLAYERS);
                            break;
                        case EVENT_BONE_SPIKE_GRAVEYARD:
                            if (IsHeroic() || !me->HasAura(SPELL_BONE_STORM))
                                DoCast(me, SPELL_BONE_SPIKE_GRAVEYARD);
                            events.ScheduleEvent(EVENT_PREPARE_BONE_SPIKE_GRAVEYARD, 12000, EVENT_GROUP_SPECIAL);
                            events.ScheduleEvent(EVENT_BONE_SPIKE_GRAVEYARD, 18000, EVENT_GROUP_SPECIAL);
                            events.ScheduleEvent(EVENT_CHECK_PLAYERS, 4000);
                            break;
                        case EVENT_COLDFLAME:
                            _coldflameLastPos.Relocate(me);
                            _coldflameTarget = 0LL;
                            if (!me->HasAura(SPELL_BONE_STORM))
                                DoCastAOE(SPELL_COLDFLAME_NORMAL);
                            else
                                DoCast(me, SPELL_COLDFLAME_BONE_STORM);
                            events.ScheduleEvent(EVENT_COLDFLAME, 5000, EVENT_GROUP_SPECIAL);
                            break;
                        case EVENT_WARN_BONE_STORM:
                            _boneSlice = false;
                            Talk(EMOTE_BONE_STORM);
                            me->FinishSpell(CURRENT_MELEE_SPELL, false);
                            DoCast(me, SPELL_BONE_STORM);
                            events.DelayEvents(3000, EVENT_GROUP_SPECIAL);
                            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
                            events.ScheduleEvent(EVENT_BONE_STORM_BEGIN, 3050);
                            events.ScheduleEvent(EVENT_WARN_BONE_STORM, 90000);
                            break;
                        case EVENT_BONE_STORM_BEGIN:
                            if (Aura* pStorm = me->GetAura(SPELL_BONE_STORM))
                                pStorm->SetDuration(int32(_boneStormDuration));
                            me->SetSpeed(MOVE_RUN, _baseSpeed*3.0f, true);
                            Talk(SAY_BONE_STORM);
                            events.ScheduleEvent(EVENT_BONE_STORM_END, _boneStormDuration+1);
                            // no break here
                        case EVENT_BONE_STORM_MOVE:
                        {
                            events.ScheduleEvent(EVENT_BONE_STORM_MOVE, IsHeroic() ? _boneStormDuration/5 : _boneStormDuration/4);
                            Unit* unit = SelectTarget(SELECT_TARGET_FARTHEST, 0, 60.0f);
                            if (!unit || unit->isPet() || unit->isTotem() || !me->IsWithinLOSInMap(unit))
                                unit = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);
                            if (unit)
                                if (unit->isPet() || unit->isTotem() || !me->IsWithinLOSInMap(unit))
                                    unit = SelectTarget(SELECT_TARGET_RANDOM, 1);
                                else
                                    me->GetMotionMaster()->MovePoint(POINT_TARGET_BONESTORM_PLAYER, unit->GetPositionX(), unit->GetPositionY(), unit->GetPositionZ());
                            break;
                        }
                        case EVENT_BONE_STORM_END:
                            if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                                me->GetMotionMaster()->MovementExpired();
                            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
                            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, false);
                            DoStartMovement(me->getVictim());
                            me->SetSpeed(MOVE_RUN, _baseSpeed, true);
                            events.CancelEvent(EVENT_BONE_STORM_MOVE);
                            events.ScheduleEvent(EVENT_ENABLE_BONE_SLICE, 10000);
                            if (!IsHeroic())
                                events.RescheduleEvent(EVENT_BONE_SPIKE_GRAVEYARD, 15000, EVENT_GROUP_SPECIAL);
                            break;
                        case EVENT_ENABLE_BONE_SLICE:
                            _boneSlice = true;
                            break;
                        case EVENT_BERSERK:
                            DoCast(me, SPELL_BERSERK, true);
                            Talk(SAY_BERSERK);
                            break;
                    }
                }

                // We should not melee attack when storming
                if (me->HasAura(SPELL_BONE_STORM))
                    return;

                // 10 seconds since encounter start Bone Slice replaces melee attacks
                if (_boneSlice && !me->GetCurrentSpell(CURRENT_MELEE_SPELL))
                    DoCastVictim(SPELL_BONE_SLICE);

                DoMeleeAttackIfReady();
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE || id != POINT_TARGET_BONESTORM_PLAYER)
                    return;

                // lock movement
                DoStartNoMovement(me->getVictim());
            }

            Position const* GetLastColdflamePosition() const
            {
                return &_coldflameLastPos;
            }

            uint64 GetGUID(int32 type/* = 0 */)
            {
                if (type == DATA_COLDFLAME_GUID)
                    return _coldflameTarget;
                return 0LL;
            }

            void SetGUID(uint64 guid, int32 type/* = 0 */)
            {
                if (type != DATA_COLDFLAME_GUID)
                    return;

                _coldflameTarget = guid;
            }

        private:
            Position _coldflameLastPos;
            uint64 _coldflameTarget;
            uint32 _boneStormDuration;
            float _baseSpeed;
            bool _introDone;
            bool _boneSlice;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<boss_lord_marrowgarAI>(creature);
        }
};

typedef boss_lord_marrowgar::boss_lord_marrowgarAI MarrowgarAI;

class npc_coldflame : public CreatureScript
{
    public:
        npc_coldflame() : CreatureScript("npc_coldflame") { }

        struct npc_coldflameAI : public ScriptedAI
        {
            npc_coldflameAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void IsSummonedBy(Unit* owner)
            {
                if (owner->GetTypeId() != TYPEID_UNIT)
                    return;

                Creature* creOwner = owner->ToCreature();
                Position pos;
                // random target case
                if (owner->HasAura(SPELL_BONE_STORM))
                {
                    if (MarrowgarAI* marrowgarAI = CAST_AI(MarrowgarAI, creOwner->AI()))
                    {
                        Position const* ownerPos = marrowgarAI->GetLastColdflamePosition();
                        float ang = me->GetAngle(ownerPos) - static_cast<float>(M_PI);
                        me->NormalizeOrientation(ang);
                        me->SetOrientation(ang);
                        owner->GetNearPosition(pos, 2.5f, 0.0f);
                    }
                }
                else
                {
                    Player* target = ObjectAccessor::GetPlayer(*owner, owner->GetAI()->GetGUID(DATA_COLDFLAME_GUID));
                    if (!target)
                    {
                        me->DespawnOrUnsummon();
                        return;
                    }

                    me->SetOrientation(owner->GetAngle(target));
                    owner->GetNearPosition(pos, owner->GetObjectSize() / 2.0f, 0.0f);
                }

                me->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                _events.ScheduleEvent(EVENT_COLDFLAME_TRIGGER, 450);
            }

            void UpdateAI(uint32 const diff)
            {
                _events.Update(diff);

                if (_events.ExecuteEvent() == EVENT_COLDFLAME_TRIGGER)
                {
                    Position newPos;
                    me->GetNearPosition(newPos, 5.5f, 0.0f);
                    me->NearTeleportTo(newPos.GetPositionX(), newPos.GetPositionY(), me->GetPositionZ(), me->GetOrientation());
                    //DoCast(SPELL_COLDFLAME_SUMMON);
                    _events.ScheduleEvent(EVENT_COLDFLAME_TRIGGER, 650);
                }
            }

        private:
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_coldflameAI>(creature);
        }
};

class npc_bone_spike : public CreatureScript
{
    public:
        npc_bone_spike() : CreatureScript("npc_bone_spike") { }

        struct npc_bone_spikeAI : public Scripted_NoMovementAI
        {
            npc_bone_spikeAI(Creature* creature) : Scripted_NoMovementAI(creature), _hasTrappedUnit(false)
            {
                ASSERT(creature->GetVehicleKit());
            }

            void JustDied(Unit* /*killer*/)
            {
                if (TempSummon* summ = me->ToTempSummon())
                    if (Unit* trapped = summ->GetSummoner())
                        trapped->RemoveAurasDueToSpell(SPELL_IMPALED);

                me->DespawnOrUnsummon();
            }

            void KilledUnit(Unit* victim)
            {
                me->DespawnOrUnsummon();
                victim->RemoveAurasDueToSpell(SPELL_IMPALED);
            }

            void IsSummonedBy(Unit* summoner)
            {
                DoCast(summoner, SPELL_IMPALED);
                summoner->CastSpell(me, SPELL_RIDE_VEHICLE, true);
                _events.ScheduleEvent(EVENT_FAIL_BONED, 8000);

                _hasTrappedUnit = true;
            }

            void PassengerBoarded(Unit* passenger, int8 /*seat*/, bool apply)
            {
                if (!apply)
                    return;

                /// @HACK - Change passenger offset to the one taken directly from sniffs
                /// Remove this when proper calculations are implemented.
                /// This fixes healing spiked people
                Movement::MoveSplineInit init(passenger);
                init.DisableTransportPathTransformations();
                init.MoveTo(-0.02206125f, -0.02132235f, 5.514783f);
                init.Launch();

                /// @WORKAROUND - Clear ON VEHICLE state to allow healing (Invalid target errors)
                /// Current rule for applying this state is questionable (seatFlags & VEHICLE_SEAT_FLAG_ALLOW_TURNING ???)
                passenger->ClearUnitState(UNIT_STATE_ONVEHICLE);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!_hasTrappedUnit)
                    return;

                _events.Update(diff);

                if (_events.ExecuteEvent() == EVENT_FAIL_BONED)
                    if (InstanceScript* instance = me->GetInstanceScript())
                        instance->SetData(DATA_BONED_ACHIEVEMENT, uint32(false));
            }

        private:
            EventMap _events;
            bool _hasTrappedUnit;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<npc_bone_spikeAI>(creature);
        }
};

class spell_marrowgar_coldflame: public SpellScriptLoader
{
    public:
        spell_marrowgar_coldflame() : SpellScriptLoader("spell_marrowgar_coldflame") { }

        class spell_marrowgar_coldflame_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_marrowgar_coldflame_SpellScript)

            void SelectTarget(std::list<WorldObject*>& targets)
            {
                targets.clear();
                // select any unit but not the tank (by owners threatlist)
                Unit* target = GetCaster()->GetAI()->SelectTarget(SELECT_TARGET_RANDOM, 1, -GetCaster()->GetObjectSize(), true, -SPELL_IMPALED);
                if (!target)
                    target = GetCaster()->GetAI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true); // or the tank if its solo
                if (!target)
                    return;

                GetCaster()->GetAI()->SetGUID(target->GetGUID(), DATA_COLDFLAME_GUID);
                targets.push_back(target);
            }

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetCaster()->CastSpell(GetHitUnit(), uint32(GetEffectValue()), true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_marrowgar_coldflame_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_marrowgar_coldflame_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_marrowgar_coldflame_SpellScript();
        }
};

class spell_marrowgar_coldflame_bonestorm: public SpellScriptLoader
{
    public:
        spell_marrowgar_coldflame_bonestorm() : SpellScriptLoader("spell_marrowgar_coldflame_bonestorm") { }

        class spell_marrowgar_coldflame_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_marrowgar_coldflame_SpellScript)

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                for (uint8 i = 0; i < 4; ++i)
                    GetCaster()->CastSpell(GetHitUnit(), uint32(GetEffectValue() + i), true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_marrowgar_coldflame_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_marrowgar_coldflame_SpellScript();
        }
};

class spell_marrowgar_coldflame_damage: public SpellScriptLoader
{
    public:
        spell_marrowgar_coldflame_damage() : SpellScriptLoader("spell_marrowgar_coldflame_damage") { }

        class spell_marrowgar_coldflame_damage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_marrowgar_coldflame_damage_AuraScript)

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                if (DynamicObject* owner = GetDynobjOwner())
                    if (GetTarget()->GetExactDist2d(owner) >= owner->GetRadius() || GetTarget()->HasAura(SPELL_IMPALED) || (GetTarget()->IsPlayer() && GetTarget()->ToPlayer()->HasSpellCooldown(69146)))
                        PreventDefaultAction();
                    else if (GetTarget()->IsPlayer())
                        GetTarget()->ToPlayer()->AddSpellCooldown(69146, 0, 1100);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_marrowgar_coldflame_damage_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_marrowgar_coldflame_damage_AuraScript();
        }
};

class spell_marrowgar_bone_spike_graveyard: public SpellScriptLoader
{
    public:
        spell_marrowgar_bone_spike_graveyard() : SpellScriptLoader("spell_marrowgar_bone_spike_graveyard") { }

        class spell_marrowgar_bone_spike_graveyard_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_marrowgar_bone_spike_graveyard_SpellScript)

            bool Validate(SpellInfo const* /*spell*/)
            {
                for (uint32 i = 0; i < 3; ++i)
                    if (!sSpellMgr->GetSpellInfo(BoneSpikeSummonId[i]))
                        return false;

                return true;
            }

            SpellCastResult CheckCast()
            {
                if (Unit* caster = GetCaster())
                    return GetCaster()->GetAI()->SelectTarget(SELECT_TARGET_TOPAGGRO, 1, 0.0f, true, -SPELL_IMPALED) ? SPELL_CAST_OK : SPELL_FAILED_NO_VALID_TARGETS;
                else
                    return SPELL_FAILED_NO_VALID_TARGETS;
            }

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_UNIT && GetCaster()->IsAIEnabled;
            }

            void HandleSpikes(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Creature* marrowgar = GetCaster()->ToCreature())
                {
                    bool didHit = false;
                    CreatureAI* marrowgarAI = marrowgar->AI();
                    uint8 boneSpikeCount = uint8(GetCaster()->GetMap()->GetSpawnMode() & 1 ? 3 : 1);
                    for (uint8 i = 0; i < boneSpikeCount; ++i)
                    {
                        // select any unit but not the tank
                        Unit* target = marrowgarAI->SelectTarget(SELECT_TARGET_RANDOM, 1, 150.0f, true, -SPELL_IMPALED);
                        if (!target)
                            break;

                        if (target->HasAura(SPELL_TANK_MARKER))
                            target = marrowgarAI->SelectTarget(SELECT_TARGET_RANDOM, 1, 150.0f, true, -SPELL_IMPALED);
                        else
                        {
                            didHit = true;
                            target->CastCustomSpell(BoneSpikeSummonId[i], SPELLVALUE_BASE_POINT0, 0, target, true);
                        }
                    }

                    if (didHit)
                        marrowgarAI->Talk(SAY_BONESPIKE);
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_marrowgar_bone_spike_graveyard_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_marrowgar_bone_spike_graveyard_SpellScript::HandleSpikes, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_marrowgar_bone_spike_graveyard_SpellScript();
        }
};

class spell_marrowgar_bone_storm: public SpellScriptLoader
{
    public:
        spell_marrowgar_bone_storm() : SpellScriptLoader("spell_marrowgar_bone_storm") { }

        class spell_marrowgar_bone_storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_marrowgar_bone_storm_SpellScript)

            void RecalculateDamage()
            {
                SetHitDamage(int32(GetHitDamage() / std::max(sqrtf(GetHitUnit()->GetExactDist2d(GetCaster())), 1.0f)));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_marrowgar_bone_storm_SpellScript::RecalculateDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_marrowgar_bone_storm_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_lord_marrowgar()
{
    new boss_lord_marrowgar();
    new npc_coldflame();
    new npc_bone_spike();
    new spell_marrowgar_coldflame();
    new spell_marrowgar_coldflame_bonestorm();
    new spell_marrowgar_coldflame_damage();
    new spell_marrowgar_bone_spike_graveyard();
    new spell_marrowgar_bone_storm();
}
#endif
