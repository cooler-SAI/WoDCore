////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include"ScriptPCH.h"
#include"shadowfang_keep.h"

#define SAY_AGGRO "Even here i find myself beset by fools!"
#define SAY_DEATH "My new life was just beginning..."
#define SAY_KILL1 "This childish effort ends now!"
#define SAY_KILL2 "You shall annoy me no longer!"

enum ScriptTexts
{
};

enum Spells
{
    SPELL_ICE_SHARDS                    = 93527,
    SPELL_CONJURE_POISONOUS_MIXTURE     = 93697,
    SPELL_CONJURE_POISONOUS_MIXTURE_H   = 93704,
    SPELL_CONJURE_FROST_MIXTURE         = 93505,
    SPELL_CONJURE_FROST_MIXTURE_H       = 93702,


    SPELL_CONJURE_MYSTERY_TOXIN_1       = 93695,
    SPELL_CONJURE_MYSTERY_TOXIN_2       = 93563,
    

    SPELL_CONJURE_MYSTERY_PERIODIC      = 93562,
                                        
    SPELL_TOXIC_COAGULANT_AURA          = 93572,
    SPELL_TOXIC_COAGULANT_DMG           = 93617,

    SPELL_TOXIC_CATALYST_AURA           = 93573,
    SPELL_TOXIC_CATALYST_DMG            = 93689
};

enum Events
{
    EVENT_ICE_SHARDS                = 1,
    EVENT_CONJURE_MYSTERY_TOXIN     = 2,
    EVENT_CONJURE_POISONOUS_MIXTURE = 3,
    EVENT_CONJURE_FROST_MIXTURE     = 4
};

enum Adds
{
    NPC_MYSTERY_TOXIN_1 = 50522,
    NPC_MYSTERY_TOXIN_2 = 50439
};

class boss_lord_valden : public CreatureScript
{
    public:
        boss_lord_valden() : CreatureScript("boss_lord_valden") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_lord_valdenAI(pCreature);
        }

        struct boss_lord_valdenAI : public BossAI
        {
            boss_lord_valdenAI(Creature* pCreature) : BossAI(pCreature, DATA_VALDEN)
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

            bool bRed;

            void Reset()
            {
                _Reset();

                bRed = false;
            }
            
            void EnterCombat(Unit* /*pWho*/)
            {
                if (IsHeroic())
                {
                    events.ScheduleEvent(EVENT_CONJURE_FROST_MIXTURE, 7500);
                    events.ScheduleEvent(EVENT_CONJURE_MYSTERY_TOXIN, 15000);
                }
                events.ScheduleEvent(EVENT_ICE_SHARDS, 30000);
                events.ScheduleEvent(EVENT_CONJURE_POISONOUS_MIXTURE, 6000);
                instance->SetBossState(DATA_VALDEN, IN_PROGRESS);
                me->MonsterYell(SAY_AGGRO, 0, 0);
                DoZoneInCombat();
            }
            
            void JustDied(Unit* /*pWho*/)
            {
                _JustDied();

                me->MonsterYell(SAY_DEATH, 0, 0);
            }

            void KilledUnit(Unit* /*pWho*/)
            {
                me->MonsterYell(urand(0, 1)? SAY_KILL1: SAY_KILL2, 0, 0);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HealthBelowPct(30) && !bRed)
                {
                    bRed = true;
                    DoCast(me, SPELL_CONJURE_MYSTERY_TOXIN_2);
                    return;
                }

                events.Update(uiDiff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_ICE_SHARDS:
                            DoCast(me, SPELL_ICE_SHARDS);
                            events.ScheduleEvent(EVENT_ICE_SHARDS, urand(20000, 25000));
                            break;
                        case EVENT_CONJURE_POISONOUS_MIXTURE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                                DoCast(target, SPELL_CONJURE_POISONOUS_MIXTURE);
                            events.ScheduleEvent(EVENT_CONJURE_POISONOUS_MIXTURE, urand(8000, 14000));
                            break;
                        case EVENT_CONJURE_FROST_MIXTURE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                                DoCast(target, SPELL_CONJURE_FROST_MIXTURE);
                            events.ScheduleEvent(EVENT_CONJURE_FROST_MIXTURE, urand(8000, 14000));
                            break;
                        case EVENT_CONJURE_MYSTERY_TOXIN:
                            DoCast(me, SPELL_CONJURE_MYSTERY_TOXIN_1);
                            events.ScheduleEvent(EVENT_CONJURE_MYSTERY_TOXIN, 25000);
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };
};

class npc_valden_mystery_toxin : public CreatureScript
{
    public:
        npc_valden_mystery_toxin() : CreatureScript("npc_valden_mystery_toxin") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_valden_mystery_toxinAI(pCreature);
        }
        struct npc_valden_mystery_toxinAI : public Scripted_NoMovementAI
        {
            npc_valden_mystery_toxinAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
            {
            }

            void Reset()
            {
                switch (me->GetEntry())
                {
                    case NPC_MYSTERY_TOXIN_1:
                        DoCast(me, SPELL_TOXIC_COAGULANT_AURA);
                        break;
                    case NPC_MYSTERY_TOXIN_2:
                        DoCast(me, SPELL_TOXIC_CATALYST_DMG);
                        DoCast(me, SPELL_TOXIC_CATALYST_AURA);
                        break;
                }
            }
     };
};

class spell_valden_toxic_coagulant: public SpellScriptLoader
{
    public:
        spell_valden_toxic_coagulant() : SpellScriptLoader("spell_valden_toxic_coagulant") { }

        class spell_valden_toxic_coagulant_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_valden_toxic_coagulant_AuraScript)
            
            void HandleTick(AuraEffect const* /*aurEff*/)
            {
                if (!GetTarget())
                    return;

                if (GetTarget()->IsMoving())
                    GetTarget()->RemoveAuraFromStack(SPELL_TOXIC_COAGULANT_DMG);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_valden_toxic_coagulant_AuraScript::HandleTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_valden_toxic_coagulant_AuraScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_lord_valden()
{
    new boss_lord_valden();
    new npc_valden_mystery_toxin();
    new spell_valden_toxic_coagulant();
}
#endif
