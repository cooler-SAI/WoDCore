////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include"ScriptPCH.h"
#include"the_vortex_pinnacle.h"

enum Spells
{
    SPELL_CALL_OF_WIND            = 88244,
    SPELL_CALL_OF_WIND_DUMMY_1    = 88276,
    SPELL_CALL_OF_WIND_DUMMY_2    = 88772,
    SPELL_DOWNWIND_OF_ALTAIRUS    = 88286,
    SPELL_UPWIND_OF_ALTAIRUS    = 88282,
    SPELL_CHILLING_BREATH        = 88308,
    SPELL_CHILLING_BREATH_DUMMY    = 88322,
    SPELL_LIGHTNING_BLAST        = 88357,
    SPELL_LIGHTNING_BLAST_DUMMY    = 88332,
    SPELL_TWISTER_AURA            = 88313,
    SPELL_TWISTER_AURA_DMG        = 88314,
    
};

enum Events
{
    EVENT_CALL_OF_WIND            = 1,
    EVENT_CHILLING_BREATH        = 2,
    EVENT_LIGHTNING_BLAST        = 3,
    EVENT_CHECK_FACING            = 4,
    EVENT_RESET_WIND            = 5
};

enum Adds
{
    NPC_TWISTER        = 47342,
    NPC_AIR_CURRENT    = 47305
};

const float orientations[4] = {5.70f, 2.54f, 0.84f, 4.44f};

class boss_altairus : public CreatureScript
{
    public:
        boss_altairus() : CreatureScript("boss_altairus") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_altairusAI(pCreature);
        }
        struct boss_altairusAI : public BossAI
        {
            boss_altairusAI(Creature* pCreature) : BossAI(pCreature, DATA_ALTAIRUS)
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
            }

            uint64 _aircurrent;
            uint8 _twisternum;

            void InitializeAI()
            {
                if (!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != sObjectMgr->GetScriptId(VPScriptName))
                    me->IsAIEnabled = false;
                else if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                _Reset();
                _twisternum = 0;
            }
    
            void EnterCombat(Unit* /*pWho*/)
            {
                events.ScheduleEvent(EVENT_CHILLING_BREATH, urand(5000, 10000));
                events.ScheduleEvent(EVENT_CALL_OF_WIND, 2000);
                events.ScheduleEvent(EVENT_CHECK_FACING, 2500);
                DoZoneInCombat();
                instance->SetBossState(DATA_ALTAIRUS, IN_PROGRESS);
            }    

            void JustDied(Unit* /*p_Who*/)
            {
                _JustDied();
            }
            
            bool CheckOrientation(float player, float creature)
            {
                float _cur, _up, _down;
                
                if (creature > M_PI)
                    _cur = creature - M_PI;
                else
                    _cur = creature + M_PI;
                

                _up = _cur + 1.0f;
                _down = _cur - 1.0f;

                if (player > _down && player < _up)
                    return true;
                else
                    return false;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (me->getVictim())
                    if (me->getVictim()->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 55.0f)
                    {
                        DoCast(me->getVictim(), SPELL_LIGHTNING_BLAST);
                        return;
                    }

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHILLING_BREATH:
                            if (Unit* target  = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_CHILLING_BREATH);
                            events.ScheduleEvent(EVENT_CHILLING_BREATH, urand(10000, 16000));
                            break;
                        case EVENT_RESET_WIND:
                            if (Creature*c = me->GetMap()->GetCreature(_aircurrent))
                                c->DespawnOrUnsummon();
                            events.DelayEvents(1000);
                            events.ScheduleEvent(EVENT_CALL_OF_WIND, 800);
                            break;
                        case EVENT_CALL_OF_WIND:
                            if (Creature* c = me->SummonCreature(NPC_AIR_CURRENT,
                                me->GetPositionX(),
                                me->GetPositionY(),
                                me->GetPositionZ(),
                                orientations[urand(0, 3)]))
                                _aircurrent = c->GetGUID();
                            events.ScheduleEvent(EVENT_RESET_WIND, 18000);
                            break;
                        case EVENT_CHECK_FACING:
                        {
                            if (me->GetMap()->GetPlayers().isEmpty())
                                break;
                            
                            Creature* c =me->GetMap()->GetCreature(_aircurrent);
                            if (!c)
                                break;

                            for (Map::PlayerList::const_iterator itr = me->GetMap()->GetPlayers().begin(); itr != me->GetMap()->GetPlayers().end(); ++itr)
                            {
                                if (CheckOrientation(itr->getSource()->GetOrientation(), c->GetOrientation()))
                                {
                                    itr->getSource()->RemoveAurasDueToSpell(SPELL_DOWNWIND_OF_ALTAIRUS);
                                    me->AddAura(SPELL_UPWIND_OF_ALTAIRUS, itr->getSource());
                                }
                                else
                                {
                                    itr->getSource()->RemoveAurasDueToSpell(SPELL_UPWIND_OF_ALTAIRUS);
                                    me->AddAura(SPELL_DOWNWIND_OF_ALTAIRUS, itr->getSource());
                                }
                            }
                            events.ScheduleEvent(EVENT_CHECK_FACING, 3000);
                            break;
                        }
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_air_current : public CreatureScript
{
    public:
        npc_air_current() : CreatureScript("npc_air_current") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_air_currentAI(pCreature);
        }
        struct npc_air_currentAI : public Scripted_NoMovementAI
        {
            npc_air_currentAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                
                //DoCast(me, SPELL_CALL_OF_WIND_DUMMY_1);
                DoCast(me, SPELL_CALL_OF_WIND_DUMMY_2);
                //DoCast(me, SPELL_CALL_OF_WIND);
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
            }
     };
};

/*class npc_altairus_twister : public CreatureScript
{
    public:
        npc_altairus_twister() : CreatureScript("npc_altairus_twister") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_altairus_twisterAI(pCreature);
        }
        struct npc_altairus_twisterAI : public Scripted_NoMovementAI
        {
            npc_altairus_twisterAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
            {
            }

            void Reset()
            {
                DoCast(me, SPELL_TWISTER_AURA);
            }

            void UpdateAI(const uint32 diff)
            {
            }
     };
};*/

#ifndef __clang_analyzer__
void AddSC_boss_altairus()
{
    new boss_altairus();
    new npc_air_current();
    //new npc_altairus_twister();
}
#endif
