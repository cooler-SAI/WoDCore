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
    SPELL_CLEAVE                                  = 15284,
    SPELL_FRIGHTENING_SHOUT                       = 19134,
    SPELL_WHIRLWIND1                              = 15589,
    SPELL_WHIRLWIND2                              = 13736,
    SPELL_MORTAL_STRIKE                           = 16856
};

enum Texts
{
    SAY_AGGRO                                    = 0,
    SAY_EVADE                                    = 1,
    SAY_BUFF                                     = 2
};

enum Events
{
    EVENT_CLEAVE = 1,
    EVENT_FRIGHTENING_SHOUT,
    EVENT_WHIRLWIND1,
    EVENT_WHIRLWIND2,
    EVENT_MORTAL_STRIKE
};

enum Action
{
    ACTION_BUFF_YELL                              = -30001 // shared from Battleground
};

class boss_galvangar : public CreatureScript
{
public:
    boss_galvangar() : CreatureScript("boss_galvangar") { }

    struct boss_galvangarAI : public ScriptedAI
    {
        boss_galvangarAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/) override
        {
            Talk(SAY_AGGRO);
            events.ScheduleEvent(EVENT_CLEAVE, urand(1 * IN_MILLISECONDS, 9 * IN_MILLISECONDS));
            events.ScheduleEvent(EVENT_FRIGHTENING_SHOUT, urand(2 * IN_MILLISECONDS, 19 * IN_MILLISECONDS));
            events.ScheduleEvent(EVENT_WHIRLWIND1, urand(1 * IN_MILLISECONDS, 13 * IN_MILLISECONDS));
            events.ScheduleEvent(EVENT_WHIRLWIND2, urand(5 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
            events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(5 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
        }

        void DoAction(int32 const actionId) override
        {
            if (actionId == ACTION_BUFF_YELL)
                Talk(SAY_BUFF);
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

        void UpdateAI(uint32 const diff) override
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
                    case EVENT_CLEAVE:
                        DoCastVictim(SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, urand(10 * IN_MILLISECONDS, 16 * IN_MILLISECONDS));
                        break;
                    case EVENT_FRIGHTENING_SHOUT:
                        DoCastVictim(SPELL_FRIGHTENING_SHOUT);
                        events.ScheduleEvent(EVENT_FRIGHTENING_SHOUT, urand(10 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
                        break;
                    case EVENT_WHIRLWIND1:
                        DoCastVictim(SPELL_WHIRLWIND1);
                        events.ScheduleEvent(EVENT_WHIRLWIND1, urand(6 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
                        break;
                    case EVENT_WHIRLWIND2:
                        DoCastVictim(SPELL_WHIRLWIND2);
                        events.ScheduleEvent(EVENT_WHIRLWIND2, urand(10 * IN_MILLISECONDS, 25 * IN_MILLISECONDS));
                        break;
                    case EVENT_MORTAL_STRIKE:
                        DoCastVictim(SPELL_MORTAL_STRIKE);
                        events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(10 * IN_MILLISECONDS, 30 * IN_MILLISECONDS));
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
        return new boss_galvangarAI(creature);
    }
};

#ifndef __clang_analyzer__
void AddSC_boss_galvangar()
{
    new boss_galvangar;
}
#endif
