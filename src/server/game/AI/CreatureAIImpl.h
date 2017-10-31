////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef CREATUREAIIMPL_H
#define CREATUREAIIMPL_H

#include "Common.h"
#include "Define.h"
#include "CreatureAI.h"
#include "TemporarySummon.h"
#include "SpellMgr.h"
#include <functional>
#include <type_traits>

template<typename First, typename Second, typename... Rest>
static inline First const& RAND(First const& first, Second const& second, Rest const&... rest)
{
    std::reference_wrapper<typename std::add_const<First>::type> const pack[] = { first, second, rest... };
    return pack[urand(0, sizeof...(rest) + 1)].get();
}

class EventMap : private std::map<uint32, uint32>
{
    public:
        EventMap() : _time(0), _phase(0) {}

        // Returns current timer value, does not represent real dates/times
        uint32 GetTimer() const { return _time; }

        // Removes all events and clears phase
        void Reset() { clear(); _time = 0; _phase = 0; }

        void Update(uint32 time) { _time += time; }

        uint32 GetPhaseMask() const { return (_phase >> 24) & 0xFF; }

        bool Empty() const { return empty(); }

        // Sets event phase, must be in range 1 - 8
        void SetPhase(uint32 phase)
        {
            if (phase && phase < 8)
                _phase = (1 << (phase + 24));
            else if (!phase)
                _phase = 0;
        }

        // Creates new event entry in map with given id, time, group if given (1 - 8) and phase if given (1 - 8)
        // 0 for group/phase means it belongs to no group or runs in all phases
        void ScheduleEvent(uint32 eventId, uint32 time, uint32 groupId = 0, uint32 phase = 0)
        {
            time += _time;
            if (groupId && groupId < 9)
                eventId |= (1 << (groupId + 16));
            if (phase && phase < 8)
                eventId |= (1 << (phase + 24));
            const_iterator itr = find(time);
            while (itr != end())
            {
                ++time;
                itr = find(time);
            }

            insert(std::make_pair(time, eventId));
        }

        // Removes event with specified id and creates new entry for it
        void RescheduleEvent(uint32 eventId, uint32 time, uint32 groupId = 0, uint32 phase = 0)
        {
            CancelEvent(eventId);
            ScheduleEvent(eventId, time, groupId, phase);
        }

        // Reschedules closest event
        void RepeatEvent(uint32 time)
        {
            if (empty())
                return;

            uint32 eventId = begin()->second;
            erase(begin());
            time += _time;
            const_iterator itr = find(time);
            while (itr != end())
            {
                ++time;
                itr = find(time);
            }

            insert(std::make_pair(time, eventId));
        }

        // Removes first event
        void PopEvent()
        {
            erase(begin());
        }

        // Gets next event id to execute and removes it from map
        uint32 ExecuteEvent()
        {
            while (!empty())
            {
                if (begin()->first > _time)
                    return 0;
                else if (_phase && (begin()->second & 0xFF000000) && !(begin()->second & _phase))
                    erase(begin());
                else
                {
                    uint32 eventId = (begin()->second & 0x0000FFFF);
                    erase(begin());
                    return eventId;
                }
            }
            return 0;
        }

        // Gets next event id to execute
        uint32 GetEvent()
        {
            while (!empty())
            {
                if (begin()->first > _time)
                    return 0;
                else if (_phase && (begin()->second & 0xFF000000) && !(begin()->second & _phase))
                    erase(begin());
                else
                    return (begin()->second & 0x0000FFFF);
            }

            return 0;
        }

        // Delay all events
        void DelayEvents(uint32 delay)
        {
            if (delay < _time)
                _time -= delay;
            else
                _time = 0;
        }

        // Delay all events having the specified Group
        void DelayEvents(uint32 delay, uint32 groupId)
        {
            uint32 nextTime = _time + delay;
            uint32 groupMask = (1 << (groupId + 16));
            for (iterator itr = begin(); itr != end() && itr->first < nextTime;)
            {
                if (itr->second & groupMask)
                {
                    ScheduleEvent(itr->second, itr->first - _time + delay);
                    erase(itr);
                    itr = begin();
                }
                else
                    ++itr;
            }
        }

        /// Delay specific event
        void DelayEvent(uint32 p_EventID, uint32 p_Delay)
        {
            uint32 l_NextTime = _time + p_Delay;
            for (iterator l_Iter = begin(); l_Iter != end() && l_Iter->first < l_NextTime;)
            {
                if ((l_Iter->second & 0x0000FFFF) == p_EventID)
                {
                    ScheduleEvent(p_EventID, l_Iter->first - _time + p_Delay);
                    erase(l_Iter);
                    l_Iter = begin();
                }
                else
                    ++l_Iter;
            }
        }

        /// Check if specified event is scheduled
        bool HasEvent(uint32 p_EventID) const
        {
            for (const_iterator l_Iter = begin(); l_Iter != end(); ++l_Iter)
            {
                if ((l_Iter->second & 0x0000FFFF) == p_EventID)
                    return true;
            }

            return false;
        }

        uint32 GetEventTime(uint32 p_EventID) const
        {
            for (const_iterator l_Iter = begin(); l_Iter != end(); ++l_Iter)
            {
                if ((l_Iter->second & 0x0000FFFF) == p_EventID)
                    return l_Iter->first;
            }

            return 0;
        }

        // Cancel events with specified id
        void CancelEvent(uint32 eventId)
        {
            for (iterator itr = begin(); itr != end();)
            {
                if (eventId == (itr->second & 0x0000FFFF))
                {
                    erase(itr);
                    itr = begin();
                }
                else
                    ++itr;
            }
        }

        // Cancel events belonging to specified group
        void CancelEventGroup(uint32 groupId)
        {
            uint32 groupMask = (1 << (groupId + 16));

            for (iterator itr = begin(); itr != end();)
            {
                if (itr->second & groupMask)
                {
                    erase(itr);
                    itr = begin();
                }
                else
                    ++itr;
            }
        }

        // Returns time of next event to execute
        // To get how much time remains substract _time
        uint32 GetNextEventTime(uint32 eventId) const
        {
            for (const_iterator itr = begin(); itr != end(); ++itr)
                if (eventId == (itr->second & 0x0000FFFF))
                    return itr->first;

            return 0;
        }

        /**
        * @name IsInPhase
        * @brief Returns wether event map is in specified phase or not.
        * @param phase Wanted phase.
        * @return True, if phase of event map contains specified phase.
        */
        bool IsInPhase(uint8 phase)
        {
            if (phase > 8)
                return false;

            return (!phase || GetPhaseMask() & (1 << phase));
        }

    private:
        uint32 _time;
        uint32 _phase;
};

enum AITarget
{
    AITARGET_SELF,
    AITARGET_VICTIM,
    AITARGET_ENEMY,
    AITARGET_ALLY,
    AITARGET_BUFF,
    AITARGET_DEBUFF
};

enum AICondition
{
    AICOND_AGGRO,
    AICOND_COMBAT,
    AICOND_DIE
};

#define AI_DEFAULT_COOLDOWN 5000

struct AISpellInfoType
{
    AISpellInfoType() : target(AITARGET_SELF), condition(AICOND_COMBAT)
        , cooldown(AI_DEFAULT_COOLDOWN), realCooldown(0), maxRange(0.0f){}
    AITarget target;
    AICondition condition;
    uint32 cooldown;
    uint32 realCooldown;
    float maxRange;
};

AISpellInfoType* GetAISpellInfo(uint32 i);

inline void CreatureAI::SetGazeOn(Unit* target)
{
    if (me->IsValidAttackTarget(target))
    {
        AttackStart(target);
        me->SetReactState(REACT_PASSIVE);
    }
}

inline bool CreatureAI::UpdateVictimWithGaze()
{
    if (!me->isInCombat())
        return false;

    if (me->HasReactState(REACT_PASSIVE))
    {
        if (me->getVictim())
            return true;
        else
            me->SetReactState(REACT_AGGRESSIVE);
    }

    if (Unit* victim = me->SelectVictim())
        AttackStart(victim);
    return me->getVictim();
}

inline bool CreatureAI::UpdateVictim()
{
    if (!me->isInCombat())
        return false;

    if (!me->HasReactState(REACT_PASSIVE))
    {
        if (Unit* victim = me->SelectVictim())
            AttackStart(victim);
        return me->getVictim();
    }
    else if (me->getThreatManager().isThreatListEmpty())
    {
        EnterEvadeMode();
        return false;
    }

    return true;
}

inline bool CreatureAI::_EnterEvadeMode()
{
    if (!me->isAlive() || me->EvadeModeIsDisable())
        return false;

    // dont remove vehicle auras, passengers arent supposed to drop off the vehicle
    if (me->GetEntry() != 46499 && me->GetEntry() != 62982 && me->GetEntry() != 67236 && me->GetEntry() != 59607) // NPC_HOLY_GUARDIAN or Mindbender
        me->RemoveAllAurasExceptType(SPELL_AURA_CONTROL_VEHICLE);

    // sometimes bosses stuck in combat?
    me->DeleteThreatList();
    me->CombatStop(true);
    me->LoadCreaturesAddon();
    me->SetLootRecipient(NULL);
    me->ResetPlayerDamageReq();

    if (me->IsInEvadeMode())
        return false;

    return true;
}

inline void UnitAI::DoCast(Unit* victim, uint32 spellId, bool triggered)
{
    if (!victim || (me->HasUnitState(UNIT_STATE_CASTING) && !triggered))
        return;

    me->CastSpell(victim, spellId, triggered);
}

inline void UnitAI::DoCastVictim(uint32 spellId, bool triggered)
{
    if (Unit* l_Target = me->getVictim())
        me->CastSpell(l_Target, spellId, triggered);
}

inline void UnitAI::DoCastAOE(uint32 spellId, bool triggered)
{
    if (!triggered && me->HasUnitState(UNIT_STATE_CASTING))
        return;

    me->CastSpell((Unit*)NULL, spellId, triggered);
}

inline Creature* CreatureAI::DoSummon(uint32 entry, const Position& pos, uint32 despawnTime, TempSummonType summonType)
{
    return me->SummonCreature(entry, pos, summonType, despawnTime);
}

inline Creature* CreatureAI::DoSummon(uint32 entry, WorldObject* obj, float radius, uint32 despawnTime, TempSummonType summonType)
{
    Position pos;
    obj->GetRandomNearPosition(pos, radius);
    return me->SummonCreature(entry, pos, summonType, despawnTime);
}

inline Creature* CreatureAI::DoSummonFlyer(uint32 entry, WorldObject* obj, float flightZ, float radius, uint32 despawnTime, TempSummonType summonType)
{
    Position pos;
    obj->GetRandomNearPosition(pos, radius);
    pos.m_positionZ += flightZ;
    return me->SummonCreature(entry, pos, summonType, despawnTime);
}

#endif

