////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include"ScriptPCH.h"
#include"shadowfang_keep.h"


#define SAY_AGGRO "Leave this accursed place at once!"
#define SAY_DEATH "This death is only a temporary respite from my curse."
#define SAY_KILL1 "I hope your spirit finds solace."
#define SAY_KILL2 "May your soul rest in peace."

enum ScriptTexts
{
};

enum Events
{
    EVENT_VEIL_OF_SHADOW    = 1,
    EVENT_CURSED_VEIL_H     = 3
};

enum Spells
{
    SPELL_VEIL_OF_SHADOW        = 23224,
    SPELL_CURSED_VEIL_H         = 93956,
    SPELL_SUMMON_WORGEN_SPIRIT  = 93857,

    SPELL_NANDOS_T              = 93899,
    SPELL_ODO_T                 = 93864,
    SPELL_RETHILGORE_T          = 93927,
    SPELL_RAZORCLAW_T           = 93924
};

enum Adds
{
    NPC_RETHILGORE_DUMMY = 50085,
    NPC_RAZORCLAW_DUMMY  = 51080,
    NPC_ODO_DUMMY        = 50934,
    NPC_NANDOS_DUMMY     = 51047,
    NPC_ODO              = 50857,
    NPC_RAZORCLAW        = 50869,
    NPC_RETHILGORE       = 50834,
    NPC_NANDOS           = 50851
};

class boss_baron_silverlaine : public CreatureScript
{
    public:
        boss_baron_silverlaine() : CreatureScript("boss_baron_silverlaine") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_baron_silverlaineAI(pCreature);
        }
        struct boss_baron_silverlaineAI : public BossAI
        {
            boss_baron_silverlaineAI(Creature* pCreature) : BossAI(pCreature, DATA_SILVERLAINE)
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
            }

            uint8 phase;

            void Reset()
            {
                _Reset();
                phase = 0;
            }

            void EnterCombat(Unit* /*pWho*/)
            {
                me->MonsterYell(SAY_AGGRO, 0, 0);
                events.ScheduleEvent(EVENT_VEIL_OF_SHADOW, 12000);
                DoZoneInCombat();
                instance->SetBossState(DATA_SILVERLAINE, IN_PROGRESS);
            }

            void KilledUnit(Unit* /*who*/)
            {
                me->MonsterYell(urand(0, 1)? SAY_KILL1: SAY_KILL2, 0, 0);
            }

            void JustDied(Unit* /*pWho*/)
            {
                _JustDied();
                me->MonsterYell(SAY_DEATH, 0, 0);
            }
                                 
            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;

                if (!HealthAbovePct(50) && phase == 0)
                {
                    phase = 1;
                    DoCast(SPELL_SUMMON_WORGEN_SPIRIT);
                    return;
                }
                if (!HealthAbovePct(25) && phase == 1)
                {
                    phase = 2;
                    DoCast(SPELL_SUMMON_WORGEN_SPIRIT);
                    return;
                }

                events.Update(uiDiff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
                    
                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_VEIL_OF_SHADOW:
                            DoCast(me, SPELL_VEIL_OF_SHADOW);
                            events.ScheduleEvent(EVENT_VEIL_OF_SHADOW, 12000);
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
         };
};

class npc_silverlaine_worgen : public CreatureScript
{
    public:
        npc_silverlaine_worgen() : CreatureScript("npc_silverlaine_worgen") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_silverlaine_worgenAI(pCreature);
        }
         struct npc_silverlaine_worgenAI : public ScriptedAI
         {
            npc_silverlaine_worgenAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
            }

            InstanceScript *pInstance;

            void IsSummonedBy(Unit* /*summoner*/)
            {
                if (Creature* _silverlaine = me->FindNearestCreature(NPC_SILVERLAINE, 200.0f))
                    if (Unit* target = _silverlaine->AI()->SelectTarget(SELECT_TARGET_RANDOM))
                        AttackStart(target);
            }

            void UpdateAI(const uint32 /*diff*/)
            {
                if (pInstance && pInstance->GetBossState(DATA_SILVERLAINE) != IN_PROGRESS)
                    me->DespawnOrUnsummon();

                DoMeleeAttackIfReady();
            }
         };
};

class npc_silverlaine_worgen_spirit : public CreatureScript
{
    public:
        npc_silverlaine_worgen_spirit() : CreatureScript("npc_silverlaine_worgen_spirit") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_silverlaine_worgen_spiritAI(pCreature);
        }
         struct npc_silverlaine_worgen_spiritAI : public Scripted_NoMovementAI
         {
            npc_silverlaine_worgen_spiritAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
            {
            }

            void IsSummonedBy(Unit* /*summoner*/)
            {
                switch (me->GetEntry())
                {
                    case NPC_ODO_DUMMY:
                        DoCast(me, SPELL_ODO_T);
                        break;
                    case NPC_RETHILGORE_DUMMY:
                        DoCast(me, SPELL_RETHILGORE_T);
                        break;
                    case NPC_NANDOS_DUMMY:
                        DoCast(me, SPELL_NANDOS_T);
                        break;
                    case NPC_RAZORCLAW_DUMMY:
                        DoCast(me, SPELL_RAZORCLAW_T);
                        break;
                }
            }
         };
};

class spell_silverlaine_summon_worgen_spirit: public SpellScriptLoader
{
    public:
        spell_silverlaine_summon_worgen_spirit() : SpellScriptLoader("spell_silverlaine_summon_worgen_spirit") { }


        class spell_silverlaine_summon_worgen_spirit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_silverlaine_summon_worgen_spirit_SpellScript)


            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster())
                    return;

                switch (urand(0, 3))
                {
                case 0:
                    GetCaster()->CastSpell(GetCaster(), 93925, true);
                    break;
                case 1:
                    GetCaster()->CastSpell(GetCaster(), 93921, true);
                    break;
                case 2:
                    GetCaster()->CastSpell(GetCaster(), 93859, true);
                    break;
                case 3:
                    GetCaster()->CastSpell(GetCaster(), 93896, true);
                    break;
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_silverlaine_summon_worgen_spirit_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_silverlaine_summon_worgen_spirit_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_baron_silverlaine()
{
    new boss_baron_silverlaine();
    new npc_silverlaine_worgen();
    new npc_silverlaine_worgen_spirit();
    new spell_silverlaine_summon_worgen_spirit();
}
#endif
