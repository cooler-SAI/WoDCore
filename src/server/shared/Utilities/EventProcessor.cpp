////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "EventProcessor.h"

EventProcessor::EventProcessor()
{
    m_time = 0;
    m_aborting = false;
}

EventProcessor::~EventProcessor()
{
    KillAllEvents(true);
}

void EventProcessor::Update(uint32 p_time)
{
    auto d = [this](BasicEvent* Event) {
        Event->Abort(m_time);
        delete Event;
    };
    auto c = [this](BasicEvent* Event, uint32 p_time) {
        if (Event->Execute(m_time, p_time))
        {
            // completely destroy event if it is not re-added
            delete Event;
        }
    };
    auto b = [this, &c, &d](EventList::iterator i, uint32 p_time) {
        // get and remove event from queue
        BasicEvent* Event = i->second;
        m_events.erase(i);

        if (!Event->to_Abort)
        {
            c(Event, p_time);
        }
        else
        {
            d(Event);
        }
    };
    auto a = [this, &b](uint32 p_time) {
        // main event loop
        EventList::iterator i;
        while (((i = m_events.begin()) != m_events.end()) && i->first <= m_time)
        {
            b(i, p_time);
        }
    };

    // update time
    m_time += p_time;

    a(p_time);
}

void EventProcessor::KillAllEvents(bool force)
{
    // prevent event insertions
    m_aborting = true;

    // first, abort all existing events
    for (EventList::iterator i = m_events.begin(); i != m_events.end();)
    {
        EventList::iterator i_old = i;
        ++i;

        i_old->second->to_Abort = true;
        i_old->second->Abort(m_time);
        if (force || i_old->second->IsDeletable())
        {
            delete i_old->second;

            if (!force)                                      // need per-element cleanup
                m_events.erase (i_old);
        }
    }

    // fast clear event list (in force case)
    if (force)
        m_events.clear();
}

void EventProcessor::AddEvent(BasicEvent* Event, uint64 e_time, bool set_addtime)
{
    if (set_addtime) Event->m_addTime = m_time;
    Event->m_execTime = e_time;
    m_events.insert(std::pair<uint64, BasicEvent*>(e_time, Event));
}

uint64 EventProcessor::CalculateTime(uint64 t_offset) const
{
    return(m_time + t_offset);
}

