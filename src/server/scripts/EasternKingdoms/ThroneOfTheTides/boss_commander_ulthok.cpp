////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "throne_of_the_tides.h"

enum Spells
{
    SPELL_DARK_FISSURE          = 76047,
    SPELL_SQUEEZE               = 76026,
    SPELL_SQUEEZE_VEHICLE       = 76028,
    SPELL_ENRAGE                = 76100,
    SPELL_CURSE_OF_FATIGUE      = 76094,
    SPELL_DARK_FISSURE_AURA     = 76066,
    SPELL_DARK_FISSURE_AURA_H   = 91371,
    SPELL_DARK_FISSURE_DMG      = 76085,
    SPELL_DARK_FISSURE_DMG_H    = 91375,
    SPELL_ULTHOK_INTRO          = 82960
};

enum Events
{
    EVENT_DARK_FISSURE      = 1,
    EVENT_SQUEEZE           = 2,
    EVENT_CURSE_OF_FATIGUE  = 3,
    EVENT_ENRAGE            = 4
};

enum Actions
{
    ACTION_COMMANDER_ULTHOK_START_EVENT = 2
};

enum Adds
{
    NPC_DARK_FISSURE = 40784
};

class boss_commander_ulthok : public CreatureScript
{
    public:
        boss_commander_ulthok() : CreatureScript("boss_commander_ulthok") { }

        CreatureAI* GetAI(Creature *pCreature) const
        {
            return new boss_commander_ulthokAI (pCreature);
        }

        struct boss_commander_ulthokAI : public BossAI
        {
            boss_commander_ulthokAI(Creature* pCreature) : BossAI(pCreature, DATA_COMMANDER_ULTHOK)
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

            void InitializeAI()
            {
                if (!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != sObjectMgr->GetScriptId(TotTScriptName))
                    me->IsAIEnabled = false;
                else if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                _Reset();
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_COMMANDER_ULTHOK_START_EVENT)
                {
                    me->SetPhaseMask(PHASEMASK_NORMAL, true);
                    DoCast(me, SPELL_ULTHOK_INTRO);
                    if (GameObject* pCorales = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_CORALES)))
                    {
                        pCorales->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
                        pCorales->SetPhaseMask(2, true);
                    }
                }
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_DARK_FISSURE, urand(5000, 8000));
                events.ScheduleEvent(EVENT_ENRAGE, urand(20000, 25000));
                events.ScheduleEvent(EVENT_CURSE_OF_FATIGUE, urand(9000, 15000));
                events.ScheduleEvent(EVENT_SQUEEZE, urand(14000, 20000));
                instance->SetBossState(DATA_COMMANDER_ULTHOK, IN_PROGRESS);
            }

            void JustDied(Unit* /*pKiller*/)
            {
                _JustDied();
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
                    case EVENT_DARK_FISSURE:
                        DoCast(me, SPELL_DARK_FISSURE);
                        events.ScheduleEvent(EVENT_DARK_FISSURE, urand(20000, 22000));
                        break;
                    case EVENT_ENRAGE:
                        DoCast(me, SPELL_ENRAGE);
                        events.ScheduleEvent(EVENT_ENRAGE, urand(20000, 25000));
                        break;
                    case EVENT_SQUEEZE:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                            DoCast(pTarget, SPELL_SQUEEZE);
                        events.ScheduleEvent(EVENT_SQUEEZE, urand(29000, 31000));
                        break;
                    case EVENT_CURSE_OF_FATIGUE:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                            DoCast(pTarget, SPELL_CURSE_OF_FATIGUE);
                        events.ScheduleEvent(EVENT_CURSE_OF_FATIGUE, urand(13000, 15000));
                        break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

};

class npc_ulthok_dark_fissure : public CreatureScript
{
    public:
        npc_ulthok_dark_fissure() : CreatureScript("npc_ulthok_dark_fissure") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_ulthok_dark_fissureAI (pCreature);
        }

        struct npc_ulthok_dark_fissureAI : public Scripted_NoMovementAI
        {
            npc_ulthok_dark_fissureAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                DoCast(me, IsHeroic()? SPELL_DARK_FISSURE_AURA_H: SPELL_DARK_FISSURE_AURA, true);
            }

            void UpdateAI(const uint32 /*diff*/)
            {
            }
        };
};

class at_tott_commander_ulthok : public AreaTriggerScript
{
    public:
        at_tott_commander_ulthok() : AreaTriggerScript("at_tott_commander_ulthok") { }

        bool OnTrigger(Player* pPlayer, const AreaTriggerEntry* /*pAt*/)
        {
            if (InstanceScript* pInstance = pPlayer->GetInstanceScript())
            {
                if (pInstance->GetData(DATA_COMMANDER_ULTHOK_EVENT) != DONE
                    && pInstance->GetBossState(DATA_LADY_NAZJAR) != DONE)
                {
                    pInstance->SetData(DATA_COMMANDER_ULTHOK_EVENT, DONE);
                    if (Creature* pUlthok = ObjectAccessor::GetCreature(*pPlayer, pInstance->GetData64(DATA_COMMANDER_ULTHOK)))
                    {
                        pUlthok->AI()->DoAction(ACTION_COMMANDER_ULTHOK_START_EVENT);
                    }
                }
            }
            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_commander_ulthok()
{
    new boss_commander_ulthok();
    new npc_ulthok_dark_fissure();
    new at_tott_commander_ulthok();
}
#endif
