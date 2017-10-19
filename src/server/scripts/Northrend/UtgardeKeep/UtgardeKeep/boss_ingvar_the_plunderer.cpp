////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Ingvar_The_Plunderer
SD%Complete: 95
SDComment: Some Problems with Annhylde Movement, Blizzlike Timers (just shadow axe summon needs a new timer)
SDCategory: Udgarde Keep
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "utgarde_keep.h"

enum Yells
{
    //Yells Ingvar
    YELL_AGGRO_1                                = -1574005,
    YELL_AGGRO_2                                = -1574006,

    YELL_DEAD_1                                 = -1574007,
    YELL_DEAD_2                                 = -1574008,

    YELL_KILL_1                                 = -1574009,
    YELL_KILL_2                                 = -1574010
};

enum Creatures
{
    MOB_INGVAR_HUMAN                            = 23954,
    MOB_ANNHYLDE_THE_CALLER                     = 24068,
    MOB_INGVAR_UNDEAD                           = 23980
};

enum Events
{
    EVENT_CLEAVE = 1,
    EVENT_SMASH,
    EVENT_STAGGERING_ROAR,
    EVENT_ENRAGE,

    EVENT_DARK_SMASH,
    EVENT_DREADFUL_ROAR,
    EVENT_WOE_STRIKE,
    EVENT_SHADOW_AXE,
    EVENT_JUST_TRANSFORMED,
    EVENT_SUMMON_BANSHEE
};

enum Phases
{
    PHASE_HUMAN = 1,
    PHASE_UNDEAD,
    PHASE_EVENT
};

enum Spells
{
    //Ingvar Spells human form
    SPELL_CLEAVE                                = 42724,
    SPELL_SMASH                                 = 42669,
    SPELL_STAGGERING_ROAR                       = 42708,
    SPELL_ENRAGE                                = 42705,

    SPELL_INGVAR_FEIGN_DEATH                    = 42795,
    SPELL_SUMMON_BANSHEE                        = 42912,
    SPELL_SCOURG_RESURRECTION                   = 42863, // Spawn resurrect effect around Ingvar

    //Ingvar Spells undead form
    SPELL_DARK_SMASH                            = 42723,
    SPELL_DREADFUL_ROAR                         = 42729,
    SPELL_WOE_STRIKE                            = 42730,

    ENTRY_THROW_TARGET                          = 23996,
    SPELL_SHADOW_AXE_SUMMON                     = 42748
};

class boss_ingvar_the_plunderer : public CreatureScript
{
public:
    boss_ingvar_the_plunderer() : CreatureScript("boss_ingvar_the_plunderer") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ingvar_the_plundererAI(creature);
    }

    struct boss_ingvar_the_plundererAI : public ScriptedAI
    {
        boss_ingvar_the_plundererAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            bIsUndead = false;
        }

        InstanceScript* instance;

        bool bIsUndead;
        bool bIsSummonBanshee;

        void Reset()
        {
            if (bIsUndead)
                me->UpdateEntry(MOB_INGVAR_HUMAN);

            bIsUndead = false;
            bIsSummonBanshee = false;

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            me->SetStandState(UNIT_STAND_STATE_STAND);

            events.Reset();
            events.SetPhase(PHASE_HUMAN);

            events.ScheduleEvent(EVENT_CLEAVE, urand(6,12)*IN_MILLISECONDS, 0, PHASE_HUMAN);
            events.ScheduleEvent(EVENT_STAGGERING_ROAR, urand(18,21)*IN_MILLISECONDS, 0, PHASE_HUMAN);
            events.ScheduleEvent(EVENT_ENRAGE, urand(7,14)*IN_MILLISECONDS, 0, PHASE_HUMAN);
            events.ScheduleEvent(EVENT_SMASH, urand(12,17)*IN_MILLISECONDS, 0, PHASE_HUMAN);

            if (instance)
                instance->SetData(DATA_INGVAR_EVENT, NOT_STARTED);
        }

        void DamageTaken(Unit* /*done_by*/, uint32 &damage, SpellInfo const*  /*p_SpellInfo*/)
        {
            if (damage >= me->GetHealth() && !bIsUndead && !bIsSummonBanshee)
            {
                //DoCast(me, SPELL_INGVAR_FEIGN_DEATH, true);  // Dont work ???
                // visuel hack
                bIsSummonBanshee = true;
                me->SetHealth(0);
                me->InterruptNonMeleeSpells(true);
                me->RemoveAllAuras();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->GetMotionMaster()->MovementExpired(false);
                me->GetMotionMaster()->MoveIdle();
                me->SetStandState(UNIT_STAND_STATE_DEAD);
                // visuel hack end

                events.SetPhase(PHASE_EVENT);
                events.ScheduleEvent(EVENT_SUMMON_BANSHEE, 3 * IN_MILLISECONDS, 0, PHASE_EVENT);

                DoScriptText(YELL_DEAD_1, me);
            }

            if (events.IsInPhase(PHASE_EVENT))
                damage = 0;
        }

        void StartZombiePhase()
        {
            bIsUndead = true;
            me->UpdateEntry(MOB_INGVAR_UNDEAD);
            events.ScheduleEvent(EVENT_JUST_TRANSFORMED, 2 * IN_MILLISECONDS, 0, PHASE_EVENT);

            DoScriptText(YELL_AGGRO_2, me);
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (!bIsUndead)
                DoScriptText(YELL_AGGRO_1, me);

            if (instance)
                instance->SetData(DATA_INGVAR_EVENT, IN_PROGRESS);

            me->SetInCombatWithZone();
        }

        void JustDied(Unit* /*killer*/)
        {
            DoScriptText(YELL_DEAD_2, me);

            if (instance)
            {
                // Ingvar has MOB_INGVAR_UNDEAD id in this moment, so we have to update encounter state for his original id
                instance->UpdateEncounterState(ENCOUNTER_CREDIT_KILL_CREATURE, MOB_INGVAR_HUMAN, me);
                instance->SetData(DATA_INGVAR_EVENT, DONE);
            }
        }

        void ScheduleSecondPhase()
        {
            events.SetPhase(PHASE_UNDEAD);
            events.ScheduleEvent(EVENT_DARK_SMASH, urand(14,18)*IN_MILLISECONDS, 0, PHASE_UNDEAD);
            events.ScheduleEvent(EVENT_DREADFUL_ROAR, urand(18,22)*IN_MILLISECONDS, 0, PHASE_UNDEAD);
            events.ScheduleEvent(EVENT_WOE_STRIKE, urand(10,14)*IN_MILLISECONDS, 0, PHASE_UNDEAD);
            events.ScheduleEvent(EVENT_SHADOW_AXE, 30*IN_MILLISECONDS, 0, PHASE_UNDEAD);
        }

        void KilledUnit(Unit* /*victim*/)
        {
            if (bIsUndead)
                DoScriptText(YELL_KILL_1, me);
            else
                DoScriptText(YELL_KILL_2, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() && !events.IsInPhase(PHASE_EVENT))
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    // PHASE ONE
                    case EVENT_CLEAVE:
                        DoCastVictim(SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, urand(6,12)*IN_MILLISECONDS, 0, PHASE_HUMAN);
                        break;
                    case EVENT_STAGGERING_ROAR:
                        DoCast(me, SPELL_STAGGERING_ROAR);
                        events.ScheduleEvent(EVENT_STAGGERING_ROAR, urand(18,22)*IN_MILLISECONDS, 0, PHASE_HUMAN);
                        break;
                    case EVENT_ENRAGE:
                        DoCast(me, SPELL_ENRAGE);
                        events.ScheduleEvent(EVENT_ENRAGE, urand(7,14)*IN_MILLISECONDS, 0, PHASE_HUMAN);
                        break;
                    case EVENT_SMASH:
                        DoCastAOE(SPELL_SMASH);
                        events.ScheduleEvent(EVENT_SMASH, urand(12,16)*IN_MILLISECONDS, 0, PHASE_HUMAN);
                        break;
                    case EVENT_JUST_TRANSFORMED:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->SetInCombatWithZone();
                        me->GetMotionMaster()->MoveChase(me->getVictim());
                        ScheduleSecondPhase();
                        return;
                    case EVENT_SUMMON_BANSHEE:
                        DoCast(me, SPELL_SUMMON_BANSHEE);
                        return;
                    // PHASE TWO
                    case EVENT_DARK_SMASH:
                        DoCastVictim(SPELL_DARK_SMASH);
                        events.ScheduleEvent(EVENT_DARK_SMASH, urand(12,16)*IN_MILLISECONDS, 0, PHASE_UNDEAD);
                        break;
                    case EVENT_DREADFUL_ROAR:
                        DoCast(me, SPELL_DREADFUL_ROAR);
                        events.ScheduleEvent(EVENT_DREADFUL_ROAR, urand(18,22)*IN_MILLISECONDS, 0, PHASE_UNDEAD);
                        break;
                    case EVENT_WOE_STRIKE:
                        DoCastVictim(SPELL_WOE_STRIKE);
                        events.ScheduleEvent(EVENT_WOE_STRIKE, urand(10,14)*IN_MILLISECONDS, 0, PHASE_UNDEAD);
                        break;
                    case EVENT_SHADOW_AXE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 1))
                        {
                            DoCast(target, SPELL_SHADOW_AXE_SUMMON);
                        }
                        events.ScheduleEvent(EVENT_SHADOW_AXE, 30*IN_MILLISECONDS, 0, PHASE_UNDEAD);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
    };

};

enum eSpells
{
//we don't have that text in db so comment it until we get this text
//    YELL_RESSURECT                      = -1574025,

//Spells for Annhylde
    SPELL_SCOURG_RESURRECTION_HEAL              = 42704, //Heal Max + DummyAura
    SPELL_SCOURG_RESURRECTION_BEAM              = 42857, //Channeling Beam of Annhylde
    SPELL_SCOURG_RESURRECTION_DUMMY             = 42862, //Some Emote Dummy?
    SPELL_INGVAR_TRANSFORM                      = 42796
};

class mob_annhylde_the_caller : public CreatureScript
{
public:
    mob_annhylde_the_caller() : CreatureScript("mob_annhylde_the_caller") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_annhylde_the_callerAI (creature);
    }

    struct mob_annhylde_the_callerAI : public ScriptedAI
    {
        mob_annhylde_the_callerAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        float x, y, z;
        InstanceScript* instance;
        uint32 uiResurectTimer;
        uint32 uiResurectPhase;

        void Reset()
        {
            //! HACK: Creature's can't have MOVEMENTFLAG_FLYING
            me->AddUnitMovementFlag(MOVEMENTFLAG_FLYING | MOVEMENTFLAG_HOVER);
            me->SetSpeed(MOVE_SWIM, 1.0f);
            me->SetSpeed(MOVE_RUN, 1.0f);
            me->SetSpeed(MOVE_WALK, 1.0f);
            //me->SetSpeed(MOVE_FLIGHT, 1.0f);

            me->GetPosition(x, y, z);
            DoTeleportTo(x+1, y, z+30);

            Unit* ingvar = Unit::GetUnit(*me, instance ? instance->GetData64(DATA_INGVAR) : 0);
            if (ingvar)
            {
                me->GetMotionMaster()->MovePoint(1, x, y, z+15);

    //            DoScriptText(YELL_RESSURECT, me);
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;
            Unit* ingvar = Unit::GetUnit(*me, instance ? instance->GetData64(DATA_INGVAR) : 0);
            if (ingvar)
            {
                switch (id)
                {
                case 1:
                    ingvar->RemoveAura(SPELL_SUMMON_BANSHEE);
                    ingvar->CastSpell(ingvar, SPELL_SCOURG_RESURRECTION_DUMMY, true);
                    DoCast(ingvar, SPELL_SCOURG_RESURRECTION_BEAM);
                    uiResurectTimer = 8000;
                    uiResurectPhase = 1;
                    break;
                case 2:
                    me->SetVisible(false);
                    me->DealDamage(me, me->GetHealth());
                    me->RemoveCorpse();
                    break;
                }
            }
        }

        void AttackStart(Unit* /*who*/) {}
        void MoveInLineOfSight(Unit* /*who*/) {}
        void EnterCombat(Unit* /*who*/) {}
        void UpdateAI(const uint32 diff)
        {
            if (uiResurectTimer)
            {
                if (uiResurectTimer <= diff)
                {
                    if (uiResurectPhase == 1)
                    {
                        Unit* ingvar = Unit::GetUnit(*me, instance ? instance->GetData64(DATA_INGVAR) : 0);
                        if (ingvar)
                        {
                            ingvar->SetStandState(UNIT_STAND_STATE_STAND);
                            ingvar->CastSpell(ingvar, SPELL_SCOURG_RESURRECTION_HEAL, false);
                        }
                        uiResurectTimer = 3000;
                        uiResurectPhase = 2;
                    }
                    else if (uiResurectPhase == 2)
                    {
                        if (Creature* ingvar = Unit::GetCreature(*me, instance ? instance->GetData64(DATA_INGVAR) : 0))
                        {
                            ingvar->RemoveAurasDueToSpell(SPELL_SCOURG_RESURRECTION_DUMMY);

                            if (ingvar->getVictim())
                                if (boss_ingvar_the_plunderer::boss_ingvar_the_plundererAI* ai = CAST_AI(boss_ingvar_the_plunderer::boss_ingvar_the_plundererAI, ingvar->AI()))
                                    ai->StartZombiePhase();

                            me->GetMotionMaster()->MovePoint(2, x+1, y, z+30);
                            ++uiResurectPhase;
                            uiResurectTimer = 0;
                        }
                    }
                } else uiResurectTimer -= diff;
            }
        }
    };
};

enum eShadowAxe
{
    SPELL_SHADOW_AXE_DAMAGE                     = 42750,
    H_SPELL_SHADOW_AXE_DAMAGE                   = 59719,
    POINT_TARGET                                = 28
};

class mob_ingvar_throw_dummy : public CreatureScript
{
public:
    mob_ingvar_throw_dummy() : CreatureScript("mob_ingvar_throw_dummy") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_ingvar_throw_dummyAI (creature);
    }

    struct mob_ingvar_throw_dummyAI : public ScriptedAI
    {
        mob_ingvar_throw_dummyAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        void Reset()
        {
            if (Creature* target = me->FindNearestCreature(ENTRY_THROW_TARGET, 50.0f))
            {
                float x, y, z;
                target->GetPosition(x, y, z);
                me->GetMotionMaster()->MoveCharge(x, y, z, 42.0f, POINT_TARGET);
                target->DisappearAndDie();
            }
            else
            {
                me->DisappearAndDie();
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == EFFECT_MOTION_TYPE && id == POINT_TARGET)
            {
                DoCast(me, SPELL_SHADOW_AXE_DAMAGE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                if (TempSummon* summon = me->ToTempSummon())
                {
                    summon->UnSummon(10000);
                }
                else
                    me->DisappearAndDie();
            }
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_boss_ingvar_the_plunderer()
{
    new boss_ingvar_the_plunderer();
    new mob_annhylde_the_caller();
    new mob_ingvar_throw_dummy();
}
#endif
