////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "lost_city_of_the_tolvir.h"
#include "Vehicle.h"

enum eSpells
{
    SPELL_FIFTY_LASHINGS                       = 82506,
    SPELL_PLAGUE_OF_AGES                       = 82622,
    SPELL_HEAVENS_FURY                         = 81939,
    // Repentance
    SPELL_REPENTANCE_CLONE_PLAYER              = 82009,
    SPELL_REPENTANCE_COPY_WEAPON               = 81960,
    SPELL_REPENTANCE_START                     = 82320,
    SPELL_REPENTANCE_PLAYER_PULL               = 82168,
    SPELL_REPENTANCE_PLAYER_KNEEL              = 81947,
    SPELL_REPENTANCE_PLAYER_KNOCK_BACK         = 82012,
    SPELL_REPENTANCE_PLAYER_CHANGE_PHASE       = 82139,
    SPELL_REPENTANCE_FINISH                    = 82430,
    // Harbinger of Darkness
    SPELL_WAIL_OF_DARKNESS_SUMMON_WAIL         = 82195,
    SPELL_WAIL_OF_DARKNESS_VISUAL              = 82197,
    SPELL_WAIL_OF_DARKNESS_SUMMON_HARBINGER    = 82203,
    SPELL_BIRTH                                = 26586,
    SPELL_WAIL_OF_DARKNESS_DAMAGE              = 82533,
    SPELL_WAIL_OF_DARKNESS_DEATH               = 82425,
    SPELL_SOUL_SEVER                           = 82255,
    SPELL_SOUL_SEVER_CHANNEL                   = 82224,
    SPELL_MERGED_SOULS                         = 82263,
    SPELL_SOUL_SEVER_CLONE_PLAYER              = 82220,
    // Blaze of the Heavens
    SPELL_SUMMON_BLAZE_OF_THE_HEAVENS_SUMMONER = 91181,
    SPELL_SUMMON_BLAZE_OF_THE_HEAVENS          = 91180,
    SPELL_BLAZE_OF_THE_HEAVENS_VISUAL          = 91179,
    SPELL_BLAZE_OF_THE_HEAVENS_TRANSFORM       = 95276,
    SPELL_BLAZE_OF_THE_HEAVENS_PERIODIC        = 95248,
    SPELL_BLAZE_OF_THE_HEAVENS_SUMMON_FIRE     = 91189
};

enum eCreatures
{
    NPC_BLAZE_OF_THE_HEAVENS_SUMMONER          = 48904,
    NPC_REPENTANCE_MIRROR                      = 43817,
    NPC_WAIL_OF_DARKNESS                       = 43926,
    NPC_SOUL_FRAGMENT                          = 43934
};

enum eActions
{
    ACTION_REPENTANCE_START                    = 1,
    ACTION_REPENTANCE_DONE                     = 2
};

enum eTexts
{
    SAY_START                                  = -1877004,
    SAY_KNEEL_DOWN                             = -1877005,
    SAY_DEATH                                  = -1877006,
    SAY_KILL_PLAYER_1                          = -1877023,
    SAY_KILL_PLAYER_2                          = -1877024
};

enum ePhases
{
    // Barim
    PHASE_BARIM                                = 1,
    PHASE_REPENTANCE                           = 2,
    // Blaze
    PHASE_BLAZE                                = 3,
    PHASE_EGG                                  = 4,
    PHASE_BLAZE_MASK                           = 1 << PHASE_BLAZE,
    PHASE_EGG_MASK                             = 1 << PHASE_EGG
};

enum eEvents
{
    // Barim
    EVENT_REPENTANCE                           = 1,
    EVENT_SUMMON_BLAZE_OF_THE_HEAVENS          = 2,
    EVENT_HEAVENS_FURY                         = 3,
    EVENT_PLAGUE_OF_AGES                       = 4,
    EVENT_FIFTY_LASHINGS                       = 5,
    // Harbinger
    EVENT_SOUL_SEVER                           = 1,
    EVENT_WAIL_OF_DARKNESS                     = 2,
    // Blaze
    EVENT_SUMMON_BLAZE_OF_THE_HEAVENS_GROUND   = 1
};

enum eModels
{
    INVISIBLE_CREATURE_MODEL                   = 11686
};

class boss_high_prophet_barim : public CreatureScript
{
public:
    boss_high_prophet_barim() : CreatureScript("boss_high_prophet_barim") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_high_prophet_barimAI (creature);
    }

    struct boss_high_prophet_barimAI : public ScriptedAI
    {
        boss_high_prophet_barimAI(Creature* creature) : ScriptedAI(creature), lSummons(me)
        {
            instance = creature->GetInstanceScript();
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }

        EventMap events;
        SummonList lSummons;
        InstanceScript* instance;
        uint8 uiEventPhase;
        bool Repentance;

        void Reset()
        {
            events.Reset();
            lSummons.DespawnAll();
            uiEventPhase = 0;
            Repentance = false;
            me->SetReactState(REACT_AGGRESSIVE);

            if (instance)
            {
                instance->SetData(DATA_HIGH_PROPHET_BARIM, NOT_STARTED);

                if (Creature* blaze = Unit::GetCreature(*me, instance->GetData64(DATA_BLAZE)))
                    blaze->AI()->EnterEvadeMode();
            }
        }

        void JustReachedHome()
        {
            if (me->isInCombat())
            {
                if (instance)
                    if (Creature* blaze = Unit::GetCreature(*me, instance->GetData64(DATA_BLAZE)))
                        blaze->AI()->DoAction(ACTION_REPENTANCE_START);

                DoScriptText(SAY_KNEEL_DOWN, me);
                me->CastSpell(me, SPELL_REPENTANCE_START, false);
                events.ScheduleEvent(EVENT_REPENTANCE, 1500, 0, PHASE_REPENTANCE);
                return;
            }
        }

        void DoAction(const int32 action)
        {
            if (action == ACTION_REPENTANCE_DONE)
            {
                lSummons.DespawnEntry(NPC_SOUL_FRAGMENT);

                for (SummonList::const_iterator itr = lSummons.begin(); itr != lSummons.end(); ++itr)
                    if (Creature* pSummon = Unit::GetCreature(*me, (*itr)))
                        if (pSummon->GetEntry() == NPC_REPENTANCE_MIRROR && pSummon->IsAIEnabled)
                            pSummon->AI()->DoAction(ACTION_REPENTANCE_DONE);
                    
                events.ScheduleEvent(EVENT_REPENTANCE, 1500, 0, PHASE_REPENTANCE);
            }
        }

        void JustSummoned(Creature* summon)
        {
            lSummons.Summon(summon);
        }

        void JustDied(Unit* /*killer*/)
        {
            DoScriptText(SAY_DEATH, me);
            events.Reset();
            lSummons.DespawnAll();

            if (instance)
                instance->SetData(DATA_HIGH_PROPHET_BARIM, DONE);

            if (instance)
                if (Creature* blaze = Unit::GetCreature(*me, instance->GetData64(DATA_BLAZE)))
                    blaze->AI()->EnterEvadeMode();
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (instance)
                instance->SetData(DATA_HIGH_PROPHET_BARIM, IN_PROGRESS);

            if (IsHeroic())
                events.ScheduleEvent(EVENT_SUMMON_BLAZE_OF_THE_HEAVENS, 3000);

            DoScriptText(SAY_START, me);
            events.SetPhase(PHASE_BARIM);
            events.ScheduleEvent(EVENT_FIFTY_LASHINGS, 5000, 0, PHASE_BARIM);
            events.ScheduleEvent(EVENT_HEAVENS_FURY, 5000, 0, PHASE_BARIM);
            events.ScheduleEvent(EVENT_PLAGUE_OF_AGES, 6000, 0, PHASE_BARIM);
        }

        void KilledUnit(Unit* victim)
        {
            if (victim->IsPlayer())
                DoScriptText(RAND(SAY_KILL_PLAYER_1, SAY_KILL_PLAYER_2), me);
        }

        void DamageTaken(Unit* /*done_by*/, uint32 & /*damage*/, SpellInfo const*  /*p_SpellInfo*/)
        {
            if (!Repentance && me->GetHealthPct() <= 50.0f)
            {
                Repentance = true;
                events.Reset();
                events.SetPhase(PHASE_REPENTANCE);
                me->SetReactState(REACT_PASSIVE);
                me->AttackStop();
                me->GetMotionMaster()->MoveTargetedHome();
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_REPENTANCE:
                        {
                            ++uiEventPhase;

                            switch(uiEventPhase)
                            {
                                case 1:
                                    me->CastSpell(me, SPELL_REPENTANCE_PLAYER_PULL, false);
                                    events.ScheduleEvent(EVENT_REPENTANCE, 1000, 0, PHASE_REPENTANCE);
                                    break;
                                case 2:
                                    me->CastSpell(me, SPELL_REPENTANCE_PLAYER_KNEEL, false);
                                    events.ScheduleEvent(EVENT_REPENTANCE, 6000, 0, PHASE_REPENTANCE);
                                    break;
                                case 3:
                                    me->CastSpell(me, SPELL_REPENTANCE_PLAYER_CHANGE_PHASE, false);
                                    me->CastSpell(me, SPELL_REPENTANCE_PLAYER_KNOCK_BACK, false);
                                    me->CastSpell(me, SPELL_WAIL_OF_DARKNESS_SUMMON_WAIL, false);
                                    events.ScheduleEvent(EVENT_REPENTANCE, 1000, 0, PHASE_REPENTANCE);
                                    break;
                                case 4:
                                    {
                                        events.ScheduleEvent(EVENT_REPENTANCE, 3000, 0, PHASE_REPENTANCE);

                                        if (Creature* veil = me->FindNearestCreature(NPC_WAIL_OF_DARKNESS, 30.0f))
                                            veil->CastSpell(veil, SPELL_WAIL_OF_DARKNESS_SUMMON_HARBINGER, false);
                                    }
                                    break;
                                case 5:
                                    {
                                        if (Creature* veil = me->FindNearestCreature(NPC_WAIL_OF_DARKNESS, 30.0f))
                                        {
                                            veil->RemoveAura(SPELL_WAIL_OF_DARKNESS_VISUAL);
                                            veil->DespawnOrUnsummon(3500);
                                        }
                                    }
                                    break;
                                case 6:
                                    {
                                        me->SetReactState(REACT_AGGRESSIVE);
                                        me->RemoveAura(SPELL_REPENTANCE_START);
                                        events.Reset();
                                        events.SetPhase(PHASE_BARIM);
                                        events.ScheduleEvent(EVENT_FIFTY_LASHINGS, 5000, 0, PHASE_BARIM);
                                        events.ScheduleEvent(EVENT_HEAVENS_FURY, 5000, 0, PHASE_BARIM);
                                        events.ScheduleEvent(EVENT_PLAGUE_OF_AGES, 6000, 0, PHASE_BARIM);

                                        if (instance)
                                            if (Creature* blaze = Unit::GetCreature(*me, instance->GetData64(DATA_BLAZE)))
                                                blaze->AI()->DoAction(ACTION_REPENTANCE_DONE);
                                    }
                                    break;
                            }
                        }
                        break;
                    case EVENT_SUMMON_BLAZE_OF_THE_HEAVENS:
                        {
                            ++uiEventPhase;

                            switch (uiEventPhase)
                            {
                                case 1:
                                    me->CastSpell(me, SPELL_SUMMON_BLAZE_OF_THE_HEAVENS_SUMMONER, false);
                                    events.ScheduleEvent(EVENT_SUMMON_BLAZE_OF_THE_HEAVENS, 2000);
                                    break;
                                case 2:
                                    {
                                        events.ScheduleEvent(EVENT_SUMMON_BLAZE_OF_THE_HEAVENS, 3000);

                                        if (Creature* blaze = me->FindNearestCreature(NPC_BLAZE_OF_THE_HEAVENS_SUMMONER, 100.0f))
                                            blaze->CastSpell(blaze, SPELL_SUMMON_BLAZE_OF_THE_HEAVENS, false);
                                    }
                                    break;
                                case 3:
                                    {
                                        uiEventPhase = 0;

                                        if (Creature* blaze = me->FindNearestCreature(NPC_BLAZE_OF_THE_HEAVENS_SUMMONER, 100.0f))
                                        {
                                            blaze->RemoveAura(SPELL_BLAZE_OF_THE_HEAVENS_VISUAL);
                                            blaze->DespawnOrUnsummon(3500);
                                        }
                                    }
                                    break;
                            }
                        }
                        break;
                    case EVENT_HEAVENS_FURY:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                me->CastSpell(target, SPELL_HEAVENS_FURY, false);

                            events.ScheduleEvent(EVENT_HEAVENS_FURY, urand(15000, 30000), 0, PHASE_BARIM);
                        }
                        break;
                    case EVENT_FIFTY_LASHINGS:
                        events.ScheduleEvent(EVENT_FIFTY_LASHINGS, urand(15000, 30000), 0, PHASE_BARIM);
                        me->CastSpell(me, SPELL_FIFTY_LASHINGS, false);
                        break;
                    case EVENT_PLAGUE_OF_AGES:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                me->CastSpell(target, SPELL_PLAGUE_OF_AGES, false, NULL, NULL, target->GetGUID());

                            events.ScheduleEvent(EVENT_PLAGUE_OF_AGES, urand(10000, 25000), 0, PHASE_BARIM);
                        }
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

class npc_blaze_of_the_heavens : public CreatureScript
{
public:
    npc_blaze_of_the_heavens() : CreatureScript("npc_blaze_of_the_heavens") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_blaze_of_the_heavensAI (creature);
    }

    struct npc_blaze_of_the_heavensAI : public ScriptedAI
    {
        npc_blaze_of_the_heavensAI(Creature* creature) : ScriptedAI(creature), lSummons(me)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            instance = me->GetInstanceScript();
            me->SetReactState(REACT_PASSIVE);
            me->SetVisible(false);
            uiBirthTimer = 0;
            uiBirthPhase = 1;
            Birth = true;

            if (instance)
                instance->SetData64(DATA_BLAZE, me->GetGUID());
        }

        InstanceScript* instance;
        EventMap events;
        SummonList lSummons;
        uint32 uiBirthTimer;
        uint8 uiBirthPhase;
        uint8 uiBlazeTimer;
        bool Birth;

        void Reset()
        {
            events.Reset();
            uiBlazeTimer = 5;
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_REPENTANCE_START:
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    events.SetPhase(PHASE_REPENTANCE);
                    me->RemoveAura(SPELL_BLAZE_OF_THE_HEAVENS_PERIODIC);
                    me->CastSpell(me, SPELL_BLAZE_OF_THE_HEAVENS_TRANSFORM, false);
                    break;
                case ACTION_REPENTANCE_DONE:
                    {

                        if (me->GetHealth() < me->GetMaxHealth())
                            events.SetPhase(PHASE_EGG);
                        else
                        {
                            Birth = true;
                            uiBirthTimer = 2500;
                            me->RemoveAura(SPELL_BLAZE_OF_THE_HEAVENS_TRANSFORM);
                            events.SetPhase(PHASE_BLAZE);
                            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        }

                    }
                    break;
            }
        }

        void EnterEvadeMode()
        {
            if (instance)
                instance->SetData64(DATA_BLAZE, 0);

            lSummons.DespawnAll();
            me->DespawnOrUnsummon();
        }

        void JustSummoned(Creature* summoned)
        {
            lSummons.Summon(summoned);
        }

        void SpellHit(Unit* /*caster*/, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_BLAZE_OF_THE_HEAVENS_TRANSFORM)
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        void HealReceived(Unit* /*healer*/, uint32& heal)
        {
            if (me->GetHealth() + heal >= me->GetMaxHealth())
                if (events.GetPhaseMask() & PHASE_EGG_MASK)
                {
                    Birth = true;
                    uiBirthTimer = 2500;
                    me->RemoveAura(SPELL_BLAZE_OF_THE_HEAVENS_TRANSFORM);

                    events.SetPhase(PHASE_BLAZE);

                }
        }

        void DamageTaken(Unit* /*done_by*/, uint32 &damage, SpellInfo const*  /*p_SpellInfo*/)
        {
            if (damage >= me->GetHealth())
            {
                damage = 0;
                me->SetHealth(1);

                if (events.GetPhaseMask() & PHASE_BLAZE_MASK)
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    events.SetPhase(PHASE_EGG);
                    me->RemoveAura(SPELL_BLAZE_OF_THE_HEAVENS_PERIODIC);
                    me->CastSpell(me, SPELL_BLAZE_OF_THE_HEAVENS_TRANSFORM, false);
                }
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.SetPhase(PHASE_BLAZE);
            me->CastSpell(me, SPELL_BLAZE_OF_THE_HEAVENS_PERIODIC, false);
            events.ScheduleEvent(EVENT_SUMMON_BLAZE_OF_THE_HEAVENS_GROUND, 3000, 0, PHASE_BLAZE);
        }

        void UpdateAI(const uint32 diff)
        {
            if (Birth)
            {
                if (uiBirthTimer <= diff)
                {
                    switch(uiBirthPhase)
                    {
                        case 1:
                            ++uiBirthPhase;
                            uiBirthTimer = 2500;
                            me->SetVisible(true);
                            me->CastSpell(me, SPELL_BIRTH, false);
                            break;
                        case 2:
                            ++uiBirthPhase;
                            Birth = false;
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->SetInCombatWithZone();
                            break;
                        case 3:
                            Birth = false;
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->CastSpell(me, SPELL_BLAZE_OF_THE_HEAVENS_PERIODIC, false);
                            events.ScheduleEvent(EVENT_SUMMON_BLAZE_OF_THE_HEAVENS_GROUND, 3000, 0, PHASE_BLAZE);
                            break;
                    }
                }
                else
                    uiBirthTimer -= diff;
            }

            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SUMMON_BLAZE_OF_THE_HEAVENS_GROUND:
                        {
                            if (uiBlazeTimer == 1)
                                uiBlazeTimer = urand(5, 6);
                            else
                                uiBlazeTimer = 1;

                            events.ScheduleEvent(EVENT_SUMMON_BLAZE_OF_THE_HEAVENS_GROUND, uiBlazeTimer * 1000, 0, PHASE_BLAZE);
                            me->CastSpell(me, SPELL_BLAZE_OF_THE_HEAVENS_SUMMON_FIRE, false);
                        }
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

class npc_repentance_mirror : public CreatureScript
{
public:
    npc_repentance_mirror() : CreatureScript("npc_repentance_mirror") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_repentance_mirrorAI (creature);
    }

    struct npc_repentance_mirrorAI : public ScriptedAI
    {
        npc_repentance_mirrorAI(Creature* creature) : ScriptedAI(creature)
        {
            InstanceScript* instance = creature->GetInstanceScript();
            uiBirthTimer = 250;
            uiBirthPhase = 1;
            Birth = true;
            me->SetVisible(false);

            if (instance)
                if (Creature* barim = Unit::GetCreature(*me, instance->GetData64(DATA_HIGH_PROPHET_BARIM)))
                    barim->AI()->JustSummoned(me);
        }

        uint32 uiBirthTimer;
        uint8 uiBirthPhase;
        bool Birth;

        void Reset()
        {
            if (!Birth)
            {
                me->CastSpell(me, SPELL_REPENTANCE_CLONE_PLAYER, false);
                me->CastSpell(me, SPELL_REPENTANCE_COPY_WEAPON, false);
            }
        }

        void DoAction(const int32 action)
        {
            if (action == ACTION_REPENTANCE_DONE)
            {
                me->CastSpell(me, SPELL_REPENTANCE_FINISH, false);
                me->DespawnOrUnsummon(1000);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (Birth)
            {
                if (uiBirthTimer <= diff)
                {
                    switch(uiBirthPhase)
                    {
                        case 1:
                            ++uiBirthPhase;
                            uiBirthTimer = 250;
                            me->CastSpell(me, SPELL_REPENTANCE_CLONE_PLAYER, false);
                            me->CastSpell(me, SPELL_REPENTANCE_COPY_WEAPON, false);
                            break;
                        case 2:
                            Birth = false;
                            me->SetVisible(true);
                            break;
                    }
                }
                else
                    uiBirthTimer -= diff;
            }
        }
    };
};

class npc_harbinger_of_darkness : public CreatureScript
{
public:
    npc_harbinger_of_darkness() : CreatureScript("npc_harbinger_of_darkness") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_harbinger_of_darknessAI (creature);
    }

    struct npc_harbinger_of_darknessAI : public ScriptedAI
    {
        npc_harbinger_of_darknessAI(Creature* creature) : ScriptedAI(creature)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            instance = me->GetInstanceScript();
            me->SetReactState(REACT_PASSIVE);
            me->SetVisible(false);
            uiBirthTimer = 0;
            uiBirthPhase = 0;
            Birth = true;

            if (instance)
            {
                instance->SetData64(DATA_HARBINGER, me->GetGUID());

                if (Creature* barim = Unit::GetCreature(*me, instance->GetData64(DATA_HIGH_PROPHET_BARIM)))
                    barim->AI()->JustSummoned(me);
            }
        }

        InstanceScript* instance;
        EventMap events;
        uint32 uiBirthTimer;
        uint8 uiBirthPhase;
        bool Birth;
        bool Dead;

        void Reset()
        {
            Dead = false;
            events.Reset();
        }

        void EnterEvadeMode()
        {
            instance->SetData64(DATA_HARBINGER, 0);

            if (Creature* barim = Unit::GetCreature(*me, instance->GetData64(DATA_HIGH_PROPHET_BARIM)))
                barim->AI()->EnterEvadeMode();
        }

        void DamageTaken(Unit* /*done_by*/, uint32 &damage, SpellInfo const*  /*p_SpellInfo*/)
        {
            if (damage >= me->GetHealth())
            {
                damage = 0;

                if (!Dead)
                {
                    if (instance)
                    {
                        instance->SetData64(DATA_HARBINGER, 0);

                        if (Creature* barim = me->FindNearestCreature(BOSS_HIGH_PROPHET_BARIM, 300.0f))
                            barim->AI()->DoAction(ACTION_REPENTANCE_DONE);
                    }

                    Dead = true;
                    events.Reset();
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    me->SetHealth(1);
                    me->CastSpell(me, SPELL_WAIL_OF_DARKNESS_DEATH, false);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->SetDisplayId(INVISIBLE_CREATURE_MODEL);
                    me->DespawnOrUnsummon(2000);
                }
            }
        }

        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_SOUL_SEVER, 1000);
            events.ScheduleEvent(EVENT_WAIL_OF_DARKNESS, urand(1000, 4000));
            me->CastSpell(who, SPELL_SOUL_SEVER, false);
        }

        void UpdateAI(const uint32 diff)
        {
            if (Birth)
            {
                if (uiBirthTimer <= diff)
                {
                    ++uiBirthPhase;

                    switch(uiBirthPhase)
                    {
                        case 1:
                            uiBirthTimer = 2500;
                            me->SetVisible(true);
                            me->CastSpell(me, SPELL_BIRTH, false);
                            break;
                        case 2:
                            Birth = false;
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->SetInCombatWithZone();
                            break;
                    }
                }
                else
                    uiBirthTimer -= diff;
            }

            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SOUL_SEVER:
                        {
                            events.ScheduleEvent(EVENT_SOUL_SEVER, 11000);

                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                me->CastSpell(target, SPELL_SOUL_SEVER, false);
                        }
                        break;
                    case EVENT_WAIL_OF_DARKNESS:
                        me->CastSpell(me, SPELL_WAIL_OF_DARKNESS_DAMAGE, false);
                        events.ScheduleEvent(EVENT_WAIL_OF_DARKNESS, urand(1000, 4000));
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

class npc_soul_fragment : public CreatureScript
{
public:
    npc_soul_fragment() : CreatureScript("npc_soul_fragment") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_soul_fragmentAI (creature);
    }

    struct npc_soul_fragmentAI : public Scripted_NoMovementAI
    {
        npc_soul_fragmentAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            instance = creature->GetInstanceScript();
            me->SetReactState(REACT_PASSIVE);

            if (instance)
                if (Creature* barim = Unit::GetCreature(*me, instance->GetData64(DATA_HIGH_PROPHET_BARIM)))
                    barim->AI()->JustSummoned(me);
        }

        InstanceScript* instance;
        bool Cast;
        //DestinationHolder< Traveller<Creature> > i_destinationHolder;
        float i_targetX, i_targetY;
        bool ReloadMoving;

        void Reset()
        {
            if (instance)
                if (!instance->GetData64(DATA_HARBINGER))
                {
                    me->DespawnOrUnsummon();
                    return;
                }

            Cast = false;
            me->CastSpell(me, SPELL_REPENTANCE_COPY_WEAPON, false);
            me->CastSpell(me, SPELL_SOUL_SEVER_CLONE_PLAYER, false);
            me->CastSpell(me, SPELL_SOUL_SEVER_CHANNEL, false);
        }

        void SpellHit(Unit* /*unit*/, const SpellInfo* spell)
        {
            if (spell->Id == 91277)
            {
                if (Aura* aura = me->GetAura(62214))
                {
                    aura->SetDuration(5000);
                    return;
                }

                float x, y, z;
                me->GetPosition(x, y, z);

                if (Creature* soul = me->SummonCreature(49219, x, y, z, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 5000))
                    if (Aura* aura = soul->AddAura(62214, me))
                        aura->SetDuration(5000);
            }
        }

        /*bool _setTargetLocation(Unit* target)
        {
            if (me->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED | UNIT_STATE_DISTRACTED))
                return true;

            float x, y, z;
            Traveller<Creature> traveller(*me);

            if (i_destinationHolder.HasDestination() && !ReloadMoving)
            {
                if (i_destinationHolder.HasArrived())
                {
                    if (target->IsWithinMeleeRange(me))
                        return false;
                }
                else
                {
                    bool stop = false;

                    if (me->GetExactDist2d(target->getX(), target->getY()) < 3.0f)
                        stop = true;

                    if (stop)
                    {
                        me->GetPosition(x, y, z);
                        i_destinationHolder.SetDestination(traveller, x, y, z);
                        i_destinationHolder.StartTravel(traveller, false);
                        me->StopMoving();
                        return false;
                    }
                }

                if (target->GetExactDist2d(i_targetX, i_targetY) < 0.01f)
                    return false;
            }
            else
            {
                ReloadMoving = false;
                me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
                target->GetPosition(i_targetX, i_targetY);
                i_destinationHolder.ResetTravelTime();
            }

            target->GetPosition(x, y, z);
            i_destinationHolder.SetDestination(traveller, x, y, z);
            me->AddUnitState(UNIT_STATE_CHASE);
            return true;
        }

        bool DoUpdate(const uint32 time_diff)
        {
            Unit* target = NULL;

            if (instance)
                target = Unit::GetCreature(*me, instance->GetData64(DATA_HARBINGER));

            if (!target || !target->isAlive())
                return false;

            if (me->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED | UNIT_STATE_DISTRACTED))
                return true;

            Traveller<Creature> traveller(*me);

            if (!i_destinationHolder.HasDestination() || ReloadMoving)
            {
                if (!_setTargetLocation(target))
                    Finish();
            }
            else if (me->IsStopped() && !i_destinationHolder.HasArrived())
            {
                me->AddUnitState(UNIT_STATE_CHASE);
                i_destinationHolder.StartTravel(traveller);
                return true;
            }
            else if (i_destinationHolder.HasArrived())
                Finish();

            if (i_destinationHolder.UpdateTraveller(traveller, time_diff))
            {
                if (i_targetX != target->getX() || i_targetY != target->getY())
                {
                    if (!_setTargetLocation(target))
                        Finish();

                    target->GetPosition(i_targetX, i_targetY);
                }

                if ((me->IsStopped() && !i_destinationHolder.HasArrived()))
                {
                    me->SetInFront(target);
                    me->StopMoving();
                }
            }

            return true;
        }

        void Finish()
        {
            if (Cast)
                return;

            if (instance)
                if (Creature* harbinger = Unit::GetCreature(*me, instance->GetData64(DATA_HARBINGER)))
                {
                    Cast = true;
                    me->CastSpell(harbinger, SPELL_MERGED_SOULS, false, NULL, NULL, harbinger->GetGUID());
                    me->RemoveAllAuras();
                    me->StopMoving();
                    me->ForcedDespawn(1000);
                }
        }*/

        void UpdateAI(const uint32 /*p_Diff*/)
        {
            //DoUpdate(diff);
        }
    };
};

class spell_repentance_pull_player: public SpellScriptLoader
{
    public:
        spell_repentance_pull_player() : SpellScriptLoader("spell_repentance_pull_player") { }

        class spell_repentance_pull_player_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_repentance_pull_player_SpellScript)

            void PullPlayer(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!(caster && target))
                    return;

                float x, y, _x, _y;
                target->GetPosition(_x, _y);

                if (caster->GetDistance2d(_x, _y) < 9.0f)
                    return;

                caster->GetNearPoint2D(x, y, frand(5.0f, 9.0f), caster->GetAngle(_x, _y));
                float z = caster->GetBaseMap()->GetHeight(x, y, MAX_HEIGHT);
                float speedZ = (float)(GetSpellInfo()->Effects[effIndex].CalcValue() / 10);
                float speedXY = (float)(GetSpellInfo()->Effects[effIndex].MiscValue / 10);
                target->GetMotionMaster()->MoveJump(x, y, z, speedXY, speedZ);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_repentance_pull_player_SpellScript::PullPlayer, EFFECT_0, SPELL_EFFECT_PULL_TOWARDS);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_repentance_pull_player_SpellScript();
        }
};

class spell_repentance_trigger_clone_spell: public SpellScriptLoader
{
    public:
        spell_repentance_trigger_clone_spell() : SpellScriptLoader("spell_repentance_trigger_clone_spell") { }

        class spell_repentance_trigger_clone_spell_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_repentance_trigger_clone_spell_SpellScript)

            void TriggerCast(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!(caster && target))
                    return;

                target->CastSpell(caster, uint32(GetEffectValue()), true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_repentance_trigger_clone_spell_SpellScript::TriggerCast, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_repentance_trigger_clone_spell_SpellScript();
        }
};

class spell_copy_melee_weapon: public SpellScriptLoader
{
    public:
        spell_copy_melee_weapon() : SpellScriptLoader("spell_copy_melee_weapon") { }

        class spell_copy_melee_weapon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_copy_melee_weapon_AuraScript)

            uint32 uiTargetDefaultWeapon;

            bool Load()
            {
                uiTargetDefaultWeapon = 0;
                return true;
            }

            void ApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                PreventDefaultAction();
                Unit* caster = GetCaster();
                Unit* target = GetTarget();

                if (!(caster && target))
                    return;

                uiTargetDefaultWeapon = target->GetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS);

                if (Player* player = caster->ToPlayer())
                {
                    if (Item* main_weapon = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
                        target->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS, main_weapon->GetEntry());
                }
                else
                    target->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS, caster->GetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS));
            }

            void RemoveEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                PreventDefaultAction();
                Unit* target = GetTarget();

                if (!target)
                    return;

                target->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS, uiTargetDefaultWeapon);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_copy_melee_weapon_AuraScript::ApplyEffect, EFFECT_0, SPELL_AURA_MOD_DISARM, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_copy_melee_weapon_AuraScript::RemoveEffect, EFFECT_0, SPELL_AURA_MOD_DISARM, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_copy_melee_weapon_AuraScript();
        }
};

class spell_repentance_player_kneel: public SpellScriptLoader
{
    public:
        spell_repentance_player_kneel() : SpellScriptLoader("spell_repentance_player_kneel") { }

        class spell_repentance_player_kneel_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_repentance_player_kneel_AuraScript)

            void DecayPeriodicTimer(AuraEffect* aurEff)
            {
                aurEff->SetPeriodic(false);
            }

            void Register()
            {
                OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_repentance_player_kneel_AuraScript::DecayPeriodicTimer, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript *GetAuraScript() const
        {
            return new spell_repentance_player_kneel_AuraScript();
        }
};

class spell_repentance_player_summon_mirror: public SpellScriptLoader
{
    public:
        spell_repentance_player_summon_mirror() : SpellScriptLoader("spell_repentance_player_summon_mirror") { }

        class spell_repentance_player_summon_mirror_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_repentance_player_summon_mirror_SpellScript)

            void SummonMirror(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Unit* caster = GetCaster();

                if (!caster)
                    return;

                uint32 uiEntry = GetSpellInfo()->Effects[effIndex].MiscValue;

                if (!uiEntry)
                    return;

                float x, y, z, o;
                caster->GetPosition(x, y, z, o);
                caster->SummonCreature(uiEntry, x, y, z, o);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_repentance_player_summon_mirror_SpellScript::SummonMirror, EFFECT_0, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_repentance_player_summon_mirror_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_high_prophet_barim()
{
    new boss_high_prophet_barim();
    new npc_blaze_of_the_heavens();
    new npc_repentance_mirror();
    new npc_harbinger_of_darkness();
    new npc_soul_fragment();

    new spell_repentance_pull_player();
    new spell_repentance_trigger_clone_spell();
    new spell_copy_melee_weapon();
    new spell_repentance_player_kneel();
    new spell_repentance_player_summon_mirror();
}
#endif
