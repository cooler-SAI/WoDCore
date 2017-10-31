#include "grim_batol.h"
#include "ScriptPCH.h"

enum ScriptTexts
{
    SAY_AGGRO    = 0,
    SAY_KILL    = 1,
    SAY_DEATH    = 2,
    SAY_SHIELD    = 3,
    SAY_MACE    = 5,
    SAY_DUAL    = 4
};

enum Equipment
{
    EQUIPMENT_ID_SHIELD    = 40400,
    EQUIPMENT_ID_SWORD    = 65094,
    EQUIPMENT_ID_MACE    = 65090
};

enum Spells
{
    SPELL_MIGHTY_STOMP            = 74984,
    SPELL_PICK_WEAPON            = 75000,
    SPELL_DUAL_BLADES            = 74981,
    SPELL_DUAL_BLADES_H            = 90738,
    SPELL_ENCUMBERED            = 75007,
    SPELL_ENCUMBERED_H            = 90729,
    SPELL_IMPALING_SLAM_0        = 75057,
    SPELL_IMPALING_SLAM            = 75056,
    SPELL_IMPALING_SLAM_H        = 90756,
    SPELL_DISORIENTING_ROAR        = 74976,
    SPELL_DISORIENTING_ROAR_H    = 90737,
    SPELL_BURNING_SHIELD        = 90819,
    SPELL_PERSONAL_PHALANX        = 74908,
    SPELL_FLAMING_ARROW            = 45101,
    SPELL_FLAMING_ARROW_VISUAL    = 74944,
    SPELL_CAVE_IN_DUMMY            = 74987,
    SPELL_CAVE_IN                = 74990,
    SPELL_CAVE_IN_DMG            = 74986,
    SPELL_CAVE_IN_DMG_H            = 90722,
    SPELL_FIRE_PATCH_AURA        = 90752,
    SPELL_LAVA_PATCH_DMG        = 90754
};

enum Adds
{
    NPC_CAVE_IN_STALKER    = 40228,
    NPC_TWILIGHT_ARCHER = 40197,
    NPC_FIRE_PATCH        = 48711
};

enum Events
{
    EVENT_PICK_WEAPON        = 1,
    EVENT_DISORIENTING_ROAR    = 2,
    EVENT_IMPALING_SLAM        = 3,
    EVENT_FLAMING_ARROW        = 4,
    EVENT_CAVE_IN_DUMMY        = 8,
    EVENT_CAVE_IN_DMG        = 9
};

enum Others
{
    ACTION_MACE            = 1,
    ACTION_DUAL_BLADES    = 2,
    ACTION_SHIELD        = 3
};

class boss_forgemaster_throngus : public CreatureScript
{
    public:
        boss_forgemaster_throngus() : CreatureScript("boss_forgemaster_throngus") { }

        struct boss_forgemaster_throngusAI : public ScriptedAI
        {
            boss_forgemaster_throngusAI(Creature* creature) : ScriptedAI(creature)
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
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
            uint32 uiLastWeapon;

            void Reset()
            {
                if (!pInstance)
                    return;

                SetEquipmentSlots(false, 0, 0, 0);
                events.Reset();
                if (pInstance)
                    pInstance->SetData(DATA_FORGEMASTER_THRONGUS, NOT_STARTED);
            }

            void EnterCombat(Unit* /*who*/)
            {
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_PICK_WEAPON, 10000);
                if (pInstance)
                    pInstance->SetData(DATA_FORGEMASTER_THRONGUS, IN_PROGRESS);
            }
            
            void JustDied(Unit* /*killer*/)
            {
                Talk(SAY_DEATH);
                if (pInstance)
                    pInstance->SetData(DATA_FORGEMASTER_THRONGUS, DONE);
            }

            void KilledUnit(Unit* /*victim*/)
            {
                Talk(SAY_KILL);
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
                    case EVENT_PICK_WEAPON:
                        events.DelayEvents(3000);
                        DoCast(me, SPELL_PICK_WEAPON);
                        events.ScheduleEvent(EVENT_PICK_WEAPON, 32500);
                        break;
                    case EVENT_IMPALING_SLAM:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                        {
                            DoCast(target, SPELL_IMPALING_SLAM);
                            DoCast(target, SPELL_IMPALING_SLAM_0);
                        }
                        events.ScheduleEvent(EVENT_IMPALING_SLAM, 12000);
                        break;
                    case EVENT_DISORIENTING_ROAR:
                        DoCast(me, SPELL_DISORIENTING_ROAR);
                        events.ScheduleEvent(EVENT_DISORIENTING_ROAR, 12000);
                        break;
                    }
                }
                DoMeleeAttackIfReady();
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                case ACTION_MACE:
                    DoCast(me, SPELL_ENCUMBERED, true);
                    Talk(SAY_MACE);
                    SetEquipmentSlots(false, EQUIPMENT_ID_MACE, 0, 0);
                    events.ScheduleEvent(EVENT_IMPALING_SLAM, 7000);
                    break;
                case ACTION_DUAL_BLADES:
                    DoCast(me, SPELL_DUAL_BLADES, true);
                    Talk(SAY_DUAL);
                    SetEquipmentSlots(false, EQUIPMENT_ID_SWORD, EQUIPMENT_ID_SWORD, 0);
                    events.ScheduleEvent(EVENT_DISORIENTING_ROAR, 7000);
                    break;
                case ACTION_SHIELD:
                    DoCast(me, SPELL_PERSONAL_PHALANX, true);
                    if (DUNGEON_MODE(0, 1))
                        DoCast(me, SPELL_BURNING_SHIELD);
                    Talk(SAY_SHIELD);
                    SetEquipmentSlots(false, 0, EQUIPMENT_ID_SHIELD, 0);
                    break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_forgemaster_throngusAI(creature);
        }
};



class npc_throngus_fire_patch : public CreatureScript
{
    public:

        npc_throngus_fire_patch() : CreatureScript("npc_throngus_fire_patch"){}

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_throngus_fire_patchAI(pCreature);
        }

        struct npc_throngus_fire_patchAI : public Scripted_NoMovementAI
        {
            npc_throngus_fire_patchAI(Creature *c) : Scripted_NoMovementAI(c)
            {
            }
            

            void Reset()
            {
                DoCast(me, SPELL_FIRE_PATCH_AURA);
            }

            void UpdateAI(const uint32 /*uiDiff*/)
            {
                
            }
        };
};

class spell_throngus_pick_weapon: public SpellScriptLoader
{
    public:
        spell_throngus_pick_weapon() : SpellScriptLoader("spell_throngus_pick_weapon") { }

        class spell_throngus_pick_weapon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_throngus_pick_weapon_SpellScript)

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                if (!GetCaster())
                    return;

                switch (urand(0, 2))
                {
                case 0:
                    GetCaster()->GetAI()->DoAction(ACTION_DUAL_BLADES);
                    break;
                case 1:
                    GetCaster()->GetAI()->DoAction(ACTION_MACE);
                    break;
                case 2:
                    GetCaster()->GetAI()->DoAction(ACTION_SHIELD);
                    break;
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_throngus_pick_weapon_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_throngus_pick_weapon_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_forgemaster_throngus()
{
    new boss_forgemaster_throngus();
    new npc_throngus_fire_patch();
    new spell_throngus_pick_weapon();
}
#endif
