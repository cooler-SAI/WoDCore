////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"

#define BOSS_XUEN 0

enum eSpells
{
};

enum eEvents
{
};

enum eCreatures
{
};

enum eSays
{
};

enum eActions
{
};

class boss_xuen : public CreatureScript
{
    public:
        boss_xuen() : CreatureScript("boss_xuen") { }

        struct boss_xuenAI : public BossAI
        {
            boss_xuenAI(Creature* creature) : BossAI(creature, BOSS_XUEN) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                _Reset();
            }

            void KilledUnit(Unit* who)
            {
                if (who->IsPlayer())
                    //Talk(TALK_SLAY);
                        return;
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
            }

            void JustDied(Unit* /*p_Killer*/)
            {
            }

            void DoAction(const int32 /*p_Action*/)
            {
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                /*switch (events.ExecuteEvent())
                {
                    default:
                        break;
                }*/

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_xuenAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_xuen()
{
    new boss_xuen();
}
#endif
