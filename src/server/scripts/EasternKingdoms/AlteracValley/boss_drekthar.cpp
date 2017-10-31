////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_WHIRLWIND                               = 15589,
    SPELL_WHIRLWIND2                              = 13736,
    SPELL_KNOCKDOWN                               = 19128,
    SPELL_FRENZY                                  = 8269,
    SPELL_SWEEPING_STRIKES                        = 18765, // not sure
    SPELL_CLEAVE                                  = 20677, // not sure
    SPELL_WINDFURY                                = 35886, // not sure
    SPELL_STORMPIKE                               = 51876  // not sure
};

enum Texts
{
    SAY_AGGRO                                    = 0,
    SAY_EVADE                                    = 1,
    SAY_RESPAWN                                  = 2,
    SAY_RANDOM                                   = 3
};

enum Events
{
    EVENT_WHIRLWIND = 1,
    EVENT_WHIRLWIND2,
    EVENT_KNOCKDOWN,
    EVENT_FRENZY,
    EVENT_RANDOM_YELL
};

class boss_drekthar : public CreatureScript
{
public:
    boss_drekthar() : CreatureScript("boss_drekthar") { }

    struct boss_drektharAI : public ScriptedAI
    {
        boss_drektharAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/) override
        {
            Talk(SAY_AGGRO);
            events.ScheduleEvent(EVENT_WHIRLWIND, urand(1 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
            events.ScheduleEvent(EVENT_WHIRLWIND2, urand(1 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
            events.ScheduleEvent(EVENT_KNOCKDOWN, 12 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_FRENZY, 6 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_RANDOM_YELL, urand(20 * IN_MILLISECONDS, 30 * IN_MILLISECONDS)); //20 to 30 seconds
        }

        void JustRespawned() override
        {
            Reset();
            Talk(SAY_RESPAWN);
        }

        bool CheckInRoom()
        {
            if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 50)
            {
                EnterEvadeMode();
                Talk(SAY_EVADE);
                return false;
            }

            return true;
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim() || !CheckInRoom())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_WHIRLWIND:
                        DoCastVictim(SPELL_WHIRLWIND);
                        events.ScheduleEvent(EVENT_WHIRLWIND, urand(8 * IN_MILLISECONDS, 18 * IN_MILLISECONDS));
                        break;
                    case EVENT_WHIRLWIND2:
                        DoCastVictim(SPELL_WHIRLWIND2);
                        events.ScheduleEvent(EVENT_WHIRLWIND2, urand(7 * IN_MILLISECONDS, 25 * IN_MILLISECONDS));
                        break;
                    case EVENT_KNOCKDOWN:
                        DoCastVictim(SPELL_KNOCKDOWN);
                        events.ScheduleEvent(EVENT_KNOCKDOWN, urand(10 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
                        break;
                    case EVENT_FRENZY:
                        DoCastVictim(SPELL_FRENZY);
                        events.ScheduleEvent(EVENT_FRENZY, urand(20 * IN_MILLISECONDS, 30 * IN_MILLISECONDS));
                        break;
                    case EVENT_RANDOM_YELL:
                        Talk(SAY_RANDOM);
                        events.ScheduleEvent(EVENT_RANDOM_YELL, urand(20 * IN_MILLISECONDS, 30 * IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

        private:
            EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_drektharAI(creature);
    }
};

#ifndef __clang_analyzer__
void AddSC_boss_drekthar()
{
    new boss_drekthar;
}
#endif
