////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "hour_of_twilight.h"

enum ScriptTexts
{
    SAY_AGGRO   = 0,
    SAY_DEATH   = 1,
    SAY_EVENT_1 = 2,
    SAY_EVENT_2 = 3,
    SAY_EVENT_3 = 4,
    SAY_PHASE   = 5,
    SAY_EVENT_4 = 6,
    SAY_KILL    = 7,
    SAY_EVENT_5 = 8,
    SAY_TOMB    = 9
};

enum Spells
{
    SPELL_ARCURION_APPEARS          = 104767,
    SPELL_HAND_OF_FROST             = 102593,
    SPELL_CHAINS_OF_FROST           = 102582,
    SPELL_ICY_TOMB                  = 103252,
    SPELL_ICY_TOMB_AURA             = 103252,
    SPELL_ICY_TOMB_SUMMON_TRIGGER   = 103250,
    SPELL_ICY_TOMB_SUMMON           = 103249,
    SPELL_TORRENT_OF_FROST          = 104050,

    // Frozen Servitor
    SPELL_ICY_BOULDER_AOE           = 102480,
    SPELL_ICY_BOULDER               = 102198,
    SPELL_ICY_BOULDER_DMG           = 102199
};

enum Events
{
    EVENT_HAND_OF_FROST     = 1,
    EVENT_CHAINS_OF_FROST   = 2,
    EVENT_ICY_TOMB          = 3,
    EVENT_ICY_BOULDER       = 4,
    EVENT_FROZEN_SERVITOR   = 5
};

enum Adds
{
    NPC_FROZEN_SERVITOR = 54600,
    NPC_ICY_TOMB        = 54995,
    NPC_SPAWN_STALKER   = 57197
};

#define MAX_SERVITOR 6
const Position servitorPos[MAX_SERVITOR] =
{
    {4810.14f, 31.5191f, 104.593f, 2.3692f},
    {4831.26f, 64.6198f, 108.553f, 3.28342f},
    {4827.03f, 50.566f, 108.63f, 3.05247f},
    {4796.06f, 131.432f, 132.468f, 4.60976f},
    {4788.47f, 125.67f, 129.112f, 4.73712f},
    {4777.38f, 30.809f, 92.5167f, 1.58226f},
    /*{4842.17f, 110.13f, 107.272f, 4.03014f},
    {4737.55f, 75.5538f, 105.757f, 5.68455f},
    {4838.94f, 90.1892f, 108.409f, 3.56877f},
    {4771.36f, 110.743f, 121.498f, 5.00128f},
    {4818.98f, 44.75f, 106.324f, 3.22131f},
    {4750.13f, 97.3125f, 112.217f, 5.8437f},
    {4756.07f, 103.248f, 114.95f, 5.74322f},
    {4739.72f, 84.1997f, 107.23f, 5.30915f}*/
};

class boss_arcurion : public CreatureScript
{
    public:
        boss_arcurion() : CreatureScript("boss_arcurion") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_arcurionAI(creature);
        }

        struct boss_arcurionAI : public BossAI
        {
            boss_arcurionAI(Creature* creature) : BossAI(creature, DATA_ARCURION)
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
                phase = 0;
                memset(servitors, 0, sizeof(servitors));
            }

            void InitializeAI()
            {
                if (!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != sObjectMgr->GetScriptId(HoTScriptName))
                    me->IsAIEnabled = false;
                else if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                _Reset();
                phase = 0;
                memset(servitors, 0, sizeof(servitors));
            }

            void EnterCombat(Unit* /*who*/)
            {
                Talk(SAY_AGGRO);

                phase = 0;
                memset(servitors, 0, sizeof(servitors));

                events.ScheduleEvent(EVENT_HAND_OF_FROST, 1000);
                events.ScheduleEvent(EVENT_CHAINS_OF_FROST, 15000);
                events.ScheduleEvent(EVENT_FROZEN_SERVITOR, 1);
                
                instance->SetBossState(DATA_ARCURION, IN_PROGRESS);
                DoZoneInCombat();
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                DeleteServitor(summon->GetGUID());
                BossAI::SummonedCreatureDespawn(summon);
            }

            void KilledUnit(Unit* who)
            {
                if (who && who->IsPlayer())
                    Talk(SAY_KILL);
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (me->HealthBelowPct(30) && phase == 0)
                {
                    phase = 1;
                    Talk(SAY_PHASE);
                    events.Reset();
                    summons.DespawnEntry(NPC_FROZEN_SERVITOR);
                    DoCast(me, SPELL_TORRENT_OF_FROST);
                    return;
                }

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_HAND_OF_FROST:
                            DoCastVictim(SPELL_HAND_OF_FROST);
                            events.ScheduleEvent(EVENT_HAND_OF_FROST, 2000);
                            break;
                        case EVENT_CHAINS_OF_FROST:
                            DoCastAOE(SPELL_CHAINS_OF_FROST);
                            events.ScheduleEvent(EVENT_CHAINS_OF_FROST, urand(15000, 20000));
                            break;
                        case EVENT_ICY_TOMB:
                            Talk(SAY_TOMB);
                            if (Creature* pThrall = me->FindNearestCreature(NPC_THRALL_2, 100.0f))
                                DoCast(pThrall, SPELL_ICY_TOMB);
                            break;
                        case EVENT_FROZEN_SERVITOR:
                            SummonNewServitor();
                            events.ScheduleEvent(EVENT_FROZEN_SERVITOR, urand(10000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                //DoMeleeAttackIfReady();
            }
        private:
            uint8 phase;
            uint64 servitors[MAX_SERVITOR];

            void SummonNewServitor()
            {
                for (uint8 i = 0; i < MAX_SERVITOR; ++i)
                {
                    if (servitors[i] == 0)
                    {
                        if (Creature* pServitor = me->SummonCreature(NPC_FROZEN_SERVITOR, servitorPos[i], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000))
                        {
                            servitors[i] = pServitor->GetGUID();
                            break;
                        }
                    }
                }
            }

            void DeleteServitor(uint64 guid)
            {
                for (uint8 i = 0; i < MAX_SERVITOR; ++i)
                    if (servitors[i] == guid)
                        servitors[i] = 0;
            }

        };
};

class npc_arcurion_frozen_servitor : public CreatureScript
{
    public:
        npc_arcurion_frozen_servitor() : CreatureScript("npc_arcurion_frozen_servitor") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_arcurion_frozen_servitorAI(creature);
        }

        struct npc_arcurion_frozen_servitorAI : public Scripted_NoMovementAI
        {
            npc_arcurion_frozen_servitorAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
            }

            void IsSummonedBy(Unit* /*owner*/)
            {
                events.ScheduleEvent(EVENT_ICY_BOULDER, urand(1000, 15000));
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
                    DoCastAOE(SPELL_ICY_BOULDER_AOE);
                    events.ScheduleEvent(EVENT_ICY_BOULDER, urand(15000, 30000));
                }
            }
        private:
            EventMap events;
        };
};

class spell_arcurion_icy_boulder: public SpellScriptLoader
{
    public:
        spell_arcurion_icy_boulder() : SpellScriptLoader("spell_arcurion_icy_boulder") { }

        class spell_arcurion_icy_boulder_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_arcurion_icy_boulder_SpellScript)

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_ICY_BOULDER, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_arcurion_icy_boulder_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_arcurion_icy_boulder_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_arcurion()
{
    new boss_arcurion();
    new npc_arcurion_frozen_servitor();
    new spell_arcurion_icy_boulder();
}
#endif
