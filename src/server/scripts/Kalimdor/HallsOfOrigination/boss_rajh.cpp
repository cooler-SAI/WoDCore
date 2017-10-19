////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "halls_of_origination.h"

// Sun-Touched Spriteling 39370
// Sun-Touched Sprite 39369

enum Spells
{
    //Rajh
    SPELL_SUN_STRIKE                    = 73872,
    SPELL_SUN_STRIKE_H                  = 89887,
    SPELL_SUN_STRIKE_DMG                = 73874,
    SPELL_SUN_STRIKE_DMG_H              = 90009,
    //SPELL_INFERNO_LEAP                = 87647, //Value: 32375 to 37625
    //SPELL_INFERNO_LEAP_2              = 89876, //Value: 120250 to 139750
    //SPELL_SUMMON_SUN_ORB              = 80352,
    SPELL_BLESSING_OF_THE_SUN           = 76352,
    SPELL_BLESSING_OF_THE_SUN_AURA      = 76355,
    SPELL_BLESSING_OF_THE_SUN_AURA_H    = 89879,
    SPELL_ZERO_POWER                    = 72242,
    SPELL_SUMMON_SOLAR_WINDS            = 74104,
    SPELL_SOLAR_WINDS_AURA              = 74107,
    SPELL_SOLAR_WINDS_AURA_H            = 90917,
    SPELL_SOLAR_WINDS_DMG               = 74108,
    SPELL_SOLAR_WINDS_DMG_H             = 89130,
    SPELL_SOLAR_WINDS_VISUAL            = 74109,
    SPELL_SOLAR_FIRE                    = 89131,
    SPELL_SOLAR_FIRE_DMG                = 89133,
    SPELL_SOLAR_FIRE_DMG_H              = 89878
};

enum Texts
{
    SAY_DEATH    = 0,
    SAY_AGGRO    = 1,
    SAY_ENERGIZE = 2,
    SAY_KILL     = 3
};

const Position pointcenter = {-319.68f, 193.42f, 343.94f, 4.76f};

enum Events
{
    EVENT_SUN_STRIKE           = 1,
    EVENT_SUMMON_SOLAR_WINDS   = 2
};

enum Adds
{
    NPC_SOLAR_WINDS1            = 39634,
    NPC_SOLAR_WINDS2            = 39635, // 74109
    NPC_SOLAR_WINDS3            = 47922,
    NPC_SUN_TOUCHED_SPRITELING  = 39370, // 76159
    NPC_SUN_TOUCHED_SPRITE      = 39369, // 76158,
    NPC_ORB_OF_SUN              = 40835, // 73658
    NPC_INFERNO_LEAP            = 47040,
    NPC_BLAZING_INFERNO         = 40927
};

class boss_rajh : public CreatureScript
{
    public:
        boss_rajh() : CreatureScript("boss_rajh") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_rajhAI(pCreature);
        }

        struct boss_rajhAI : public BossAI
        {
            boss_rajhAI(Creature* pCreature) : BossAI(pCreature, DATA_RAJH)
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

            uint8 phase;
            bool achieve;

            void InitializeAI()
            {
                if (!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != sObjectMgr->GetScriptId(HOScriptName))
                    me->IsAIEnabled = false;
                else if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                _Reset();

                bool achieve = false;

                phase = 0;

                SetEquipmentSlots(false, 1728, 0, 0);

                DoCast(me, SPELL_ZERO_POWER, true);
                me->setPowerType(POWER_ENERGY);
                me->SetMaxPower(POWER_ENERGY, 100);
                me->SetPower(POWER_ENERGY, 100);
            }

            void KilledUnit(Unit* /*Killed*/)
            {
                Talk(SAY_KILL);
            }

            void JustDied(Unit* /*Kill*/)
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            bool HasAchieved()
            {
                return achieve;
            }

            void EnterCombat(Unit* /*Ent*/)
            {
                Talk(SAY_AGGRO);
                
                events.ScheduleEvent(EVENT_SUN_STRIKE, urand(5000, 10000));
                events.ScheduleEvent(EVENT_SUMMON_SOLAR_WINDS, urand(10000, 15000));

                achieve = true;
                DoZoneInCombat();
                instance->SetBossState(DATA_RAJH, IN_PROGRESS);
            }

            void JustReachedHome()
            {
                _JustReachedHome();
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type == POINT_MOTION_TYPE)
                {
                    switch (id)
                    {
                        case 1:
                            Talk(SAY_ENERGIZE);
                            DoCastAOE(SPELL_BLESSING_OF_THE_SUN);
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

                if (me->GetPower(POWER_ENERGY) < 1 && phase == 0)
                {
                    phase = 1;
                    events.Reset();
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    me->GetMotionMaster()->MovePoint(1, pointcenter);
                    return;
                }
                else if (me->GetPower(POWER_ENERGY) > 99 && phase == 1)
                {
                    phase = 0;
                    achieve = false;
                    me->SetReactState(REACT_AGGRESSIVE);
                    events.ScheduleEvent(EVENT_SUN_STRIKE, urand(5000, 10000));
                    events.ScheduleEvent(EVENT_SUMMON_SOLAR_WINDS, urand(6000, 11000));
                    me->GetMotionMaster()->MoveChase(me->getVictim());
                    return;
                }

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SUN_STRIKE:
                            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(pTarget, SPELL_SUN_STRIKE);
                            events.ScheduleEvent(EVENT_SUN_STRIKE, urand(20000, 25000));
                            break;
                        case EVENT_SUMMON_SOLAR_WINDS:
                            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(pTarget, SPELL_SUMMON_SOLAR_WINDS);
                            events.ScheduleEvent(EVENT_SUMMON_SOLAR_WINDS, urand(20000, 25000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_rajh_solar_wind : public CreatureScript
{
    public:
        npc_rajh_solar_wind() : CreatureScript("npc_rajh_solar_wind") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_rajh_solar_windAI(creature);
        }

        struct npc_rajh_solar_windAI : public ScriptedAI
        {
            npc_rajh_solar_windAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                DoCast(me, SPELL_SOLAR_WINDS_VISUAL, true);
                DoCast(me, SPELL_SOLAR_WINDS_AURA, true);
            }
        };
};

typedef boss_rajh::boss_rajhAI RajhAI;

class achievement_sun_of_a : public AchievementCriteriaScript
{
    public:
        achievement_sun_of_a() : AchievementCriteriaScript("achievement_sun_of_a") { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (!target)
                return false;
            if (RajhAI* rajhAI = CAST_AI(RajhAI, target->GetAI()))
                return rajhAI->HasAchieved();

            return false;
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_rajh()
{
    new boss_rajh();
    new npc_rajh_solar_wind();
    new achievement_sun_of_a();
}
#endif
