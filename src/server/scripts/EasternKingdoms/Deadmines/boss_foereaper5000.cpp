////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "deadmines.h"

enum ScriptTexts
{
    SAY_AGGRO       = 0,
    SAY_DEATH       = 1,
    SAY_KILL        = 2,
    SAY_SPELL1      = 3,
    SAY_SPELL2      = 4,
    SAY_SPELL3      = 5,
    SAY_SPELL4      = 6
};

enum Spells
{
    SPELL_OFF_LINE          = 88348,
    SPELL_ZERO_POWER        = 87239,
    SPELL_RED_EYES          = 24263,
    SPELL_REAPER_STRIKE     = 88490,
    SPELL_REAPER_STRIKE_H   = 91717,
    SPELL_SAFETY            = 88522,
    SPELL_SAFETY_H          = 91720,
    SPELL_HARVEST           = 88495,
    SPELL_HARVEST_AURA      = 88497,
    SPELL_HARVEST_DMG       = 88501,
    SPELL_HARVEST_DMG_H     = 91719,
    SPELL_HARVEST_SWEEP     = 88521,
    SPELL_HARVEST_SWEEP_H   = 91718,
    SPELL_OVERDRIVE         = 88481,
    SPELL_OVERDRIVE_DMG     = 88484,
    SPELL_OVERDRIVE_DMG_H   = 91716,
    SPELL_TARGET_BUNNY      = 71371, //rocket artillery
};

enum Adds
{
    NPC_TARGETING_BUNNY     = 47468,
    NPC_DEFIAS_REAPER       = 47403,
    NPC_MOLTEN_SLAG         = 49229,
    NPC_PROTOTYPE_REAPER    = 49208, // 87239 91731
};

enum Events
{
    EVENT_OVERDRIVE    = 1,
    EVENT_OVERDRIVE1    = 2,
    EVENT_REAPER_STRIKE    = 3,
    EVENT_HARVEST        = 4,
    EVENT_HARVEST1        = 5,

};


class boss_foereaper5000 : public CreatureScript
{
    public:
        boss_foereaper5000() : CreatureScript("boss_foereaper5000") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_foereaper5000AI (pCreature);
        }

        struct boss_foereaper5000AI : public BossAI
        {
            boss_foereaper5000AI(Creature* pCreature) : BossAI(pCreature, DATA_FOEREAPER)
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

            Creature* harvestTarget;
            bool bEnrage;

            void Reset()
            {
                _Reset();

                bEnrage = false;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
                
                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (HealthBelowPct(30) && !bEnrage)
                {
                    Talk(SAY_SPELL4);
                    bEnrage = true;
                    DoCast(me, SPELL_SAFETY);
                    return;
                }

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_OVERDRIVE:
                            DoCast(me, SPELL_OVERDRIVE);
                            Talk(SAY_SPELL3);
                            events.ScheduleEvent(EVENT_OVERDRIVE, urand(25000, 30000));
                            break;
                        case EVENT_REAPER_STRIKE:
                            DoCast(me->getVictim(), SPELL_REAPER_STRIKE);
                            events.ScheduleEvent(EVENT_REAPER_STRIKE, urand(7000, 10000));
                            break;
                        case EVENT_HARVEST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                DoCast(target, SPELL_HARVEST);
                                Talk(SAY_SPELL1);
                            }
                            events.ScheduleEvent(EVENT_HARVEST, 30000, 40000);
                            break;
                        }
                }

                DoMeleeAttackIfReady();
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type == POINT_MOTION_TYPE)
                {
                    switch (id)
                    {
                        case 1001:
                            DoCast(me, DUNGEON_MODE(SPELL_HARVEST_SWEEP, SPELL_HARVEST_SWEEP), true);
                            me->RemoveAurasDueToSpell(SPELL_HARVEST_AURA);
                            if (harvestTarget)
                                harvestTarget->DespawnOrUnsummon();
                            break;
                    }
                }
            }

            void EnterCombat(Unit* /*who*/)
            {
                Talk(SAY_AGGRO);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
                events.ScheduleEvent(EVENT_REAPER_STRIKE, urand(5000, 8000));
                events.ScheduleEvent(EVENT_OVERDRIVE, urand(10000, 15000));
                events.ScheduleEvent(EVENT_HARVEST, urand(25000, 30000));
                DoZoneInCombat();
                instance->SetBossState(DATA_FOEREAPER, IN_PROGRESS);
            }

            void KilledUnit(Unit * /*victim*/)
            {
                Talk(SAY_KILL);
            }

            void JustSummoned(Creature* summon)
            {
                BossAI::JustSummoned(summon);
                if (summon->GetEntry()== NPC_TARGETING_BUNNY)
                {
                    harvestTarget = summon;
                    Talk(SAY_SPELL2);
                    me->GetMotionMaster()->MovePoint(1001, harvestTarget->GetPositionX(), harvestTarget->GetPositionY(), harvestTarget->GetPositionZ());
                }
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                Talk(SAY_DEATH);
            }
        };
};

class npc_foereaper_targeting_bunny : public CreatureScript
{
    public:
        npc_foereaper_targeting_bunny() : CreatureScript("npc_foereaper_targeting_bunny") { }
     
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_foereaper_targeting_bunnyAI (pCreature);
        }
     
        struct npc_foereaper_targeting_bunnyAI : public Scripted_NoMovementAI
        {
            npc_foereaper_targeting_bunnyAI(Creature *c) : Scripted_NoMovementAI(c)
            {
                pInstance = c->GetInstanceScript();
            }
           
            InstanceScript* pInstance;

            void Reset()
            {
                if (!pInstance)
                    return;

                DoCast(SPELL_TARGET_BUNNY);
            }
        };
};

#ifndef __clang_analyzer__
void AddSC_boss_foereaper5000()
{
    new boss_foereaper5000();
    new npc_foereaper_targeting_bunny();
}
#endif
