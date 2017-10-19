////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
Name: Boss_Shirrak_the_dead_watcher
%Complete: 80
Comment: InhibitMagic should stack slower far from the boss, proper Visual for Focus Fire, heroic implemented
Category: Auchindoun, Auchenai Crypts
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

#define SPELL_INHIBITMAGIC          32264
#define SPELL_ATTRACTMAGIC          32265
#define N_SPELL_CARNIVOROUSBITE     36383
#define H_SPELL_CARNIVOROUSBITE     39382
#define SPELL_CARNIVOROUSBITE       DUNGEON_MODE(N_SPELL_CARNIVOROUSBITE, H_SPELL_CARNIVOROUSBITE)

#define ENTRY_FOCUS_FIRE            18374

#define N_SPELL_FIERY_BLAST         32302
#define H_SPELL_FIERY_BLAST         38382
#define SPELL_FIERY_BLAST           DUNGEON_MODE(N_SPELL_FIERY_BLAST, H_SPELL_FIERY_BLAST)
#define SPELL_FOCUS_FIRE_VISUAL     42075 //need to find better visual

#define EMOTE_FOCUSES_ON            "focuses on "

// 18371
class boss_shirrak_the_dead_watcher : public CreatureScript
{
public:
    boss_shirrak_the_dead_watcher() : CreatureScript("boss_shirrak_the_dead_watcher") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_shirrak_the_dead_watcherAI (creature);
    }

    struct boss_shirrak_the_dead_watcherAI : public ScriptedAI
    {
        boss_shirrak_the_dead_watcherAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 Inhibitmagic_Timer;
        uint32 Attractmagic_Timer;
        uint32 Carnivorousbite_Timer;
        uint32 FocusFire_Timer;

        uint64 FocusedTargetGUID;

        void Reset()
        {
            Inhibitmagic_Timer = 0;
            Attractmagic_Timer = 28000;
            Carnivorousbite_Timer = 10000;
            FocusFire_Timer = 17000;
            FocusedTargetGUID = 0;
        }

        void EnterCombat(Unit* /*who*/)
        { }

        void JustSummoned(Creature* summoned)
        {
            if (summoned && summoned->GetEntry() == ENTRY_FOCUS_FIRE)
            {
                summoned->CastSpell(summoned, SPELL_FOCUS_FIRE_VISUAL, false);
                summoned->setFaction(me->getFaction());
                summoned->SetLevel(me->getLevel());
                summoned->AddUnitState(UNIT_STATE_ROOT);

                if (Unit* pFocusedTarget = Unit::GetUnit(*me, FocusedTargetGUID))
                    summoned->AI()->AttackStart(pFocusedTarget);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            //Inhibitmagic_Timer
            if (Inhibitmagic_Timer <= diff)
            {
                float dist;
                Map* map = me->GetMap();
                Map::PlayerList const &PlayerList = map->GetPlayers();
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    if (Player* i_pl = i->getSource())
                        if (i_pl->isAlive() && (dist = i_pl->IsWithinDist(me, 45)))
                        {
                            i_pl->RemoveAurasDueToSpell(SPELL_INHIBITMAGIC);
                            me->AddAura(SPELL_INHIBITMAGIC, i_pl);
                            if (dist < 35)
                                me->AddAura(SPELL_INHIBITMAGIC, i_pl);
                            if (dist < 25)
                                me->AddAura(SPELL_INHIBITMAGIC, i_pl);
                            if (dist < 15)
                                me->AddAura(SPELL_INHIBITMAGIC, i_pl);
                        }
                Inhibitmagic_Timer = 3000+(rand()%1000);
            } else Inhibitmagic_Timer -= diff;

            //Return since we have no target
            if (!UpdateVictim())
                return;

            //Attractmagic_Timer
            if (Attractmagic_Timer <= diff)
            {
                DoCast(me, SPELL_ATTRACTMAGIC);
                Attractmagic_Timer = 30000;
                Carnivorousbite_Timer = 1500;
            } else Attractmagic_Timer -= diff;

            //Carnivorousbite_Timer
            if (Carnivorousbite_Timer <= diff)
            {
                DoCast(me, SPELL_CARNIVOROUSBITE);
                Carnivorousbite_Timer = 10000;
            } else Carnivorousbite_Timer -= diff;

            //FocusFire_Timer
            if (FocusFire_Timer <= diff)
            {
                // Summon Focus Fire & Emote
                Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1);
                if (target && target->IsPlayer() && target->isAlive())
                {
                    FocusedTargetGUID = target->GetGUID();
                    me->SummonCreature(ENTRY_FOCUS_FIRE, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 5500);

                    // TODO: Find better way to handle emote
                    // Emote
                    std::string emote(EMOTE_FOCUSES_ON);
                    emote.append(target->GetName());
                    emote.push_back('!');
                    me->MonsterTextEmote(emote.c_str(), 0, true);
                }
                FocusFire_Timer = 15000+(rand()%5000);
            } else FocusFire_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

class mob_focus_fire : public CreatureScript
{
public:
    mob_focus_fire() : CreatureScript("mob_focus_fire") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_focus_fireAI (creature);
    }

    struct mob_focus_fireAI : public ScriptedAI
    {
        mob_focus_fireAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 FieryBlast_Timer;
        bool fiery1, fiery2;

        void Reset()
        {
            FieryBlast_Timer = 3000+(rand()%1000);
            fiery1 = fiery2 = true;
        }

        void EnterCombat(Unit* /*who*/)
        { }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //FieryBlast_Timer
            if (fiery2 && FieryBlast_Timer <= diff)
            {
                DoCast(me, SPELL_FIERY_BLAST);

                if (fiery1) fiery1 = false;
                else if (fiery2) fiery2 = false;

                FieryBlast_Timer = 1000;
            } else FieryBlast_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_shirrak_the_dead_watcher()
{
    new boss_shirrak_the_dead_watcher();
    new mob_focus_fire();
}
#endif
