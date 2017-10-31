#include "ScriptPCH.h"
#include "Spell.h"
#include "bastion_of_twilight.h"

enum HalfusScriptTexts
{
    SAY_AGGRO   = 0,
    SAY_KILL    = 1
};

enum ChogallScriptTexts
{
    SAY_INTRO   = 1,
    SAY_DEATH   = 2
};

enum Equipment
{
    EQUIPMENT_ONE   = 59492
};

enum Spells
{
    SPELL_MALEVOLENT_STRIKES_DEBUFF = 83908,
    SPELL_MALEVOLENT_STRIKES        = 39171,
    SPELL_FRENZIED_ASSAULT          = 83693,
    SPELL_FURIOUS_ROAR              = 83710,
    SPELL_FURIOUS_ROAR_25           = 86169,
    SPELL_FURIOUS_ROAR_10H          = 86170,
    SPELL_FURIOUS_ROAR_25H          = 86171,
    SPELL_STONE_GRIP                = 83603,
    SPELL_PARALYSIS                 = 84030,
    SPELL_SHADOW_NOVA               = 83703,
    SPELL_SHADOW_NOVA_25            = 86166,
    SPELL_SHADOW_NOVA_10H           = 86167,
    SPELL_SHADOW_NOVA_25H           = 86168,
    SPELL_SHADOW_WARPED             = 83952,
    SPELL_UNRESPONSIVE_DRAKE        = 86003,
    SPELL_UNRESPONSIVE_WHELP        = 86022,
    SPELL_CYCLONE_WINDS_SUM         = 83612,
    SPELL_CYCLONE_WINDS             = 84092,
    SPELL_ATROPHIC_POISON           = 83609,
    SPELL_BIND_WILL                 = 83432,
    SPELL_DRAGON_VENGEANCE          = 87683,
    SPELL_NETHER_BLINDNESS          = 83611,
    SPELL_BERSERK                   = 26662,
    SPELL_SUPERHEATED_BREATH        = 83956,
    SPELL_SCORCHING_BREATH          = 83707,
    SPELL_SCORCHING_BREATH_DMG      = 83855,
    SPELL_SCORCHING_BREATH_DMG_25   = 86163,
    SPELL_SCORCHING_BREATH_DMG_10H  = 86164,
    SPELL_SCORCHING_BREATH_DMG_25H  = 86165,
    SPELL_DANCING_FLAMES            = 84106,
    SPELL_FIREBALL_BARRAGE          = 83706,
    SPELL_FIREBALL_BARRAGE_T        = 83719,
    SPELL_FIREBALL_BARRAGE_M0       = 83720,
    SPELL_FIREBALL_BARRAGE_M1       = 83733,
    SPELL_FIREBALL_BARRAGE_DMG0     = 83734,
    SPELL_FIREBALL_BARRAGE_DMG0_25  = 86154,
    SPELL_FIREBALL_BARRAGE_DMG0_10H = 86155,
    SPELL_FIREBALL_BARRAGE_DMG0_25H = 86156,
    SPELL_FIREBALL_BARRAGE_DMG1     = 83721,
    SPELL_FIREBALL_BARRAGE_DMG1_25  = 86151,
    SPELL_FIREBALL_BARRAGE_DMG1_10H = 86152,
    SPELL_FIREBALL_BARRAGE_DMG1_25H = 86153,
    SPELL_TIME_DILATION             = 83601,
    SPELL_FIREBALL                  = 83862
};

enum Adds
{
    NPC_SLATE_DRAKE     = 44652,
    NPC_NETHER_SCION    = 44645,
    NPC_STORM_RIDER     = 44650,
    NPC_TIME_WARDEN     = 44797,
    NPC_ORPHANED_WHELP  = 44641,

    NPC_PROTO_BEHEMOTH  = 44687,
    NPC_CYCLON_WIND     = 45026
};

enum Events
{
    EVENT_SHADOW_NOVA       = 1,
    EVENT_FIREBALL          = 2,
    EVENT_BERSERK           = 3,
    EVENT_FURIOUS_ROAR1     = 4,
    EVENT_FURIOUS_ROAR2     = 5,
    EVENT_FURIOUS_ROAR0     = 6,
    EVENT_SCORCHING_BREATH  = 7,
    EVENT_DRAGON_DEATH      = 8
};

enum Action
{
    ACTION_WHELP_DIED       = 1,
    ACTION_ACTIVE_GOSSIP    = 2,
    ACTION_WHELPS_RELEASE   = 3
};


const Position halfusdrakePos[13] =
{
    //whelps
    {-340.68f, -721.54f, 888.09f, 0.39f},
    {-342.13f, -715.18f, 888.09f, 0.44f},
    {-339.47f, -717.20f, 888.09f, 0.33f},
    {-336.57f, -713.02f, 888.09f, 0.38f},
    {-334.09f, -718.08f, 888.09f, 0.43f},
    {-341.98f, -723.86f, 890.07f, 0.41f},
    {-344.18f, -719.97f, 890.07f, 0.39f},
    {-346.52f, -714.41f, 890.07f, 0.39f},
    //proto
    {-274.71f, -732.76f, 904.97f, 2.14f},
    //storm rider
    {-313.14f, -723.88f, 888.08f, 1.59f},
    //nether scion
    {-279.17f, -662.48f, 888.09f, 1.74f},
    //time warden
    {-345.21f, -699.77f, 888.10f, 5.17f},
    //slate drake
    {-276.26f, -695.36f, 888.08f, 2.65f}
};

Creature* whelps[8];

class boss_halfus_wyrmbreaker : public CreatureScript
{
    public:
        boss_halfus_wyrmbreaker() : CreatureScript("boss_halfus_wyrmbreaker") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_halfus_wyrmbreakerAI(creature);
        }

        struct boss_halfus_wyrmbreakerAI : public BossAI
        {
            boss_halfus_wyrmbreakerAI(Creature * creature) : BossAI(creature, DATA_HALFUS)
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

            Creature* netherscion;
            Creature* stormrider;
            Creature* slatedrake;
            Creature* timewarden;
            Creature* proto;
            GameObject* whelpcage;
            GameObject* whelpcage2;
            bool bRoar;
            bool bIntro;
            bool bWhelps;
            uint8 whelpcount;

            void InitializeAI()
            {
                if (!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != sObjectMgr->GetScriptId(BTScriptName))
                    me->IsAIEnabled = false;
                else if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                _Reset();

                SetEquipmentSlots(false, EQUIPMENT_ONE, 0, 0);
                me->SetFloatValue(UNIT_FIELD_BOUNDING_RADIUS, 10);
                me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 10);
                bRoar = false;
                bWhelps = false;
                events.Reset();
                summons.DespawnAll();
                if (GameObject* pGo = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_WHELP_CAGE)))
                {
                    pGo->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);
                    pGo->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);
                    pGo->SetGoState(GO_STATE_READY);
                }
                for (uint8 i = 0; i < 8; i++)
                    whelps[i] = me->SummonCreature(NPC_ORPHANED_WHELP, halfusdrakePos[i]);
                proto = me->SummonCreature(NPC_PROTO_BEHEMOTH, halfusdrakePos[8]);
                stormrider = me->SummonCreature(NPC_STORM_RIDER, halfusdrakePos[9]);
                netherscion = me->SummonCreature(NPC_NETHER_SCION, halfusdrakePos[10]);
                timewarden = me->SummonCreature(NPC_TIME_WARDEN, halfusdrakePos[11]);
                slatedrake = me->SummonCreature(NPC_SLATE_DRAKE, halfusdrakePos[12]);
                whelpcount = 0;
                if (IsHeroic())
                {
                    bWhelps = true;
                    DoCast(me, SPELL_MALEVOLENT_STRIKES);
                    DoCast(me, SPELL_SHADOW_WARPED);
                    DoCast(me, SPELL_FRENZIED_ASSAULT);
                    proto->CastSpell(proto, SPELL_DANCING_FLAMES, true);
                    proto->CastSpell(proto, SPELL_SUPERHEATED_BREATH, true);
                }
                else
                {
                    switch (urand(0, 9))
                    {
                    case 0:
                        timewarden->CastSpell(timewarden, SPELL_UNRESPONSIVE_DRAKE, true);
                        for (uint8 i = 0; i < 8; i++)
                            whelps[i]->CastSpell(whelps[i], SPELL_UNRESPONSIVE_WHELP, true);
                        DoCast(me, SPELL_MALEVOLENT_STRIKES);
                        DoCast(me, SPELL_SHADOW_WARPED);
                        DoCast(me, SPELL_FRENZIED_ASSAULT);
                        break;
                    case 1:
                        slatedrake->CastSpell(slatedrake, SPELL_UNRESPONSIVE_DRAKE, true);
                        for (uint8 i = 0; i < 8; i++)
                            whelps[i]->CastSpell(whelps[i], SPELL_UNRESPONSIVE_WHELP, true);
                        proto->CastSpell(proto, SPELL_DANCING_FLAMES, true);
                        DoCast(me, SPELL_SHADOW_WARPED);
                        DoCast(me, SPELL_FRENZIED_ASSAULT);
                        break;
                    case 2:
                        bWhelps = true;
                        timewarden->CastSpell(timewarden, SPELL_UNRESPONSIVE_DRAKE, true);
                        stormrider->CastSpell(stormrider, SPELL_UNRESPONSIVE_DRAKE, true);
                        DoCast(me, SPELL_MALEVOLENT_STRIKES);
                        DoCast(me, SPELL_FRENZIED_ASSAULT);
                        proto->CastSpell(proto, SPELL_SUPERHEATED_BREATH, true);
                        break;
                    case 3:
                        netherscion->CastSpell(netherscion, SPELL_UNRESPONSIVE_DRAKE, true);
                        for (uint8 i = 0; i < 8; i++)
                            whelps[i]->CastSpell(whelps[i], SPELL_UNRESPONSIVE_WHELP, true);
                        proto->CastSpell(proto, SPELL_DANCING_FLAMES, true);
                        DoCast(me, SPELL_MALEVOLENT_STRIKES);
                        DoCast(me, SPELL_SHADOW_WARPED);
                        break;
                    case 4:
                        bWhelps = true;
                        slatedrake->CastSpell(slatedrake, SPELL_UNRESPONSIVE_DRAKE, true);
                        timewarden->CastSpell(timewarden, SPELL_UNRESPONSIVE_DRAKE, true);
                        DoCast(me, SPELL_FRENZIED_ASSAULT);
                        DoCast(me, SPELL_SHADOW_WARPED);
                        proto->CastSpell(proto, SPELL_SUPERHEATED_BREATH, true);
                        break;
                    case 5:
                        stormrider->CastSpell(stormrider, SPELL_UNRESPONSIVE_DRAKE, true);
                        for (uint8 i = 0; i < 8; i++)
                            whelps[i]->CastSpell(whelps[i], SPELL_UNRESPONSIVE_WHELP, true);
                        DoCast(me, SPELL_MALEVOLENT_STRIKES);
                        proto->CastSpell(proto, SPELL_DANCING_FLAMES, true);
                        DoCast(me, SPELL_FRENZIED_ASSAULT);
                        break;
                    case 6:
                        bWhelps = true;
                        slatedrake->CastSpell(slatedrake, SPELL_UNRESPONSIVE_DRAKE, true);
                        netherscion->CastSpell(netherscion, SPELL_UNRESPONSIVE_DRAKE, true);
                        DoCast(me, SPELL_SHADOW_WARPED);
                        proto->CastSpell(proto, SPELL_DANCING_FLAMES, true);
                        proto->CastSpell(proto, SPELL_SUPERHEATED_BREATH, true);
                        break;
                    case 7:
                        bWhelps = true;
                        stormrider->CastSpell(stormrider, SPELL_UNRESPONSIVE_DRAKE, true);
                        netherscion->CastSpell(netherscion, SPELL_UNRESPONSIVE_DRAKE, true);
                        DoCast(me, SPELL_MALEVOLENT_STRIKES);
                        proto->CastSpell(proto, SPELL_DANCING_FLAMES, true);
                        proto->CastSpell(proto, SPELL_SUPERHEATED_BREATH, true);
                        break;
                    case 8:
                        bWhelps = true;
                        slatedrake->CastSpell(slatedrake, SPELL_UNRESPONSIVE_DRAKE, true);
                        stormrider->CastSpell(stormrider, SPELL_UNRESPONSIVE_DRAKE, true);
                        DoCast(me, SPELL_FRENZIED_ASSAULT);
                        proto->CastSpell(proto, SPELL_DANCING_FLAMES, true);
                        proto->CastSpell(proto, SPELL_SUPERHEATED_BREATH, true);
                        break;
                    case 9:
                        bWhelps = true;
                        netherscion->CastSpell(netherscion, SPELL_UNRESPONSIVE_DRAKE, true);
                        timewarden->CastSpell(timewarden, SPELL_UNRESPONSIVE_DRAKE, true);
                        DoCast(me, SPELL_MALEVOLENT_STRIKES);
                        DoCast(me, SPELL_SHADOW_WARPED);
                        proto->CastSpell(proto, SPELL_SUPERHEATED_BREATH, true);
                        break;
                    }
                }
            }

            void EnterCombat(Unit* who)
            {
                if (!slatedrake->HasAura(SPELL_UNRESPONSIVE_DRAKE))
                    slatedrake->GetAI()->DoAction(ACTION_ACTIVE_GOSSIP);
                if (!stormrider->HasAura(SPELL_UNRESPONSIVE_DRAKE))
                    stormrider->GetAI()->DoAction(ACTION_ACTIVE_GOSSIP);
                if (!netherscion->HasAura(SPELL_UNRESPONSIVE_DRAKE))
                    netherscion->GetAI()->DoAction(ACTION_ACTIVE_GOSSIP);
                if (!timewarden->HasAura(SPELL_UNRESPONSIVE_DRAKE))
                    timewarden->GetAI()->DoAction(ACTION_ACTIVE_GOSSIP);

                if (bWhelps)
                    if (GameObject* pGo = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_WHELP_CAGE)))
                        pGo->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);

                if (me->HasAura(SPELL_SHADOW_WARPED))
                    events.ScheduleEvent(EVENT_SHADOW_NOVA, 7000);
                events.ScheduleEvent(EVENT_BERSERK, 6 * MINUTE * IN_MILLISECONDS);
                proto->AddThreat(who, 10.0f);
                proto->SetInCombatWith(who);
                Talk(SAY_AGGRO);
                DoZoneInCombat();
                instance->SetBossState(DATA_HALFUS,IN_PROGRESS);
            }

            void KilledUnit(Unit* who)
            {
                if (who && who->IsPlayer())
                    Talk(SAY_KILL);
            };

            void DoAction(const int32 action)
            {
                switch(action)
                {
                    case ACTION_WHELPS_RELEASE:
                    {
                        if (instance->GetBossState(DATA_HALFUS) != IN_PROGRESS)
                            return;

                        EntryCheckPredicate pred(NPC_ORPHANED_WHELP);
                        summons.DoAction(ACTION_WHELPS_RELEASE, pred, 8);
                        break;
                    }
                    case ACTION_WHELP_DIED:
                        whelpcount++;
                        if (whelpcount == 8)
                        {
                            if (Aura* aura = me->GetAura(SPELL_DRAGON_VENGEANCE))
                                aura->SetStackAmount(aura->GetStackAmount() + 1);
                            else
                                me->AddAura(SPELL_DRAGON_VENGEANCE, me);
                        }
                        break;
                    }
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                if(Creature *Chogall = me->SummonCreature(NPC_CHOGALL_DLG, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 1, TEMPSUMMON_DEAD_DESPAWN, 0))
                    Chogall->AI()->DoAction(ACTION_AT_HALFUS_END);
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell)
            {
                if (me->GetCurrentSpell(CURRENT_GENERIC_SPELL))
                if (me->GetCurrentSpell(CURRENT_GENERIC_SPELL)->m_spellInfo->Id == SPELL_SHADOW_NOVA ||
                    me->GetCurrentSpell(CURRENT_GENERIC_SPELL)->m_spellInfo->Id == SPELL_SHADOW_NOVA_25 ||
                    me->GetCurrentSpell(CURRENT_GENERIC_SPELL)->m_spellInfo->Id == SPELL_SHADOW_NOVA_10H ||
                    me->GetCurrentSpell(CURRENT_GENERIC_SPELL)->m_spellInfo->Id == SPELL_SHADOW_NOVA_25H)
                    for (uint8 i = 0; i < 3; ++i)
                        if (spell->Effects[i].Effect == SPELL_EFFECT_INTERRUPT_CAST)
                            me->InterruptSpell(CURRENT_GENERIC_SPELL);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
                
                if (me->GetDistance(me->GetHomePosition()) >= 100.0f)
                {
                    EnterEvadeMode();
                    return;
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (me->HealthBelowPct(50) && !bRoar)
                {
                    bRoar = true;
                    events.CancelEvent(EVENT_SHADOW_NOVA);
                    events.ScheduleEvent(EVENT_FURIOUS_ROAR0, 1000);
                    if (me->HasAura(SPELL_SHADOW_WARPED))
                        events.ScheduleEvent(EVENT_SHADOW_NOVA, 7000);
                    return;
                }

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHADOW_NOVA:
                            DoCast(me, SPELL_SHADOW_NOVA);
                            events.ScheduleEvent(EVENT_SHADOW_NOVA, 7000);
                            break;
                        case EVENT_FURIOUS_ROAR1:
                        case EVENT_FURIOUS_ROAR2:
                            DoCast(me, SPELL_FURIOUS_ROAR);
                            break;
                        case EVENT_FURIOUS_ROAR0:
                            DoCast(me, SPELL_FURIOUS_ROAR);
                            events.ScheduleEvent(EVENT_FURIOUS_ROAR1, 1800);
                            events.ScheduleEvent(EVENT_FURIOUS_ROAR2, 3200);
                            events.ScheduleEvent(EVENT_FURIOUS_ROAR0, 30000);
                            break;
                        case EVENT_BERSERK:
                            DoCast(me, SPELL_BERSERK);
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };
};

class npc_proto_behemoth : public CreatureScript
{
    public :
        npc_proto_behemoth() : CreatureScript("npc_proto_behemoth") { }

        struct npc_proto_behemothAI : public ScriptedAI
        {
            npc_proto_behemothAI(Creature * creature) : ScriptedAI(creature)
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
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
            void Reset()
            {
                if (!pInstance)
                    return;
                me->SetCanFly(true);
                SetCombatMovement(false);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }

            void EnterCombat(Unit* /*who*/)
            {
                if (!pInstance)
                    return;
                if (me->HasAura(SPELL_SUPERHEATED_BREATH))
                    events.ScheduleEvent(EVENT_SCORCHING_BREATH, 30000);
                if (me->HasAura(SPELL_DANCING_FLAMES))
                    events.ScheduleEvent(EVENT_FIREBALL, 20000);
            }
            
            void DamageTaken(Unit* /*attacker*/, uint32 &damage, SpellInfo const* /*p_SpellInfo*/)
            {
                if (!pInstance)
                    return;

                damage = 0;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!pInstance || !UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case EVENT_FIREBALL:
                        DoCast(me, SPELL_FIREBALL_BARRAGE);
                        events.ScheduleEvent(EVENT_FIREBALL, urand(25000, 30000));
                        break;
                    case EVENT_SCORCHING_BREATH:
                        DoCast(me, SPELL_SCORCHING_BREATH);
                        events.ScheduleEvent(EVENT_SCORCHING_BREATH, urand(35000, 40000));
                        break;
                    }
                }

            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_proto_behemothAI(creature);
        }
};

class npc_orphaned_whelp : public CreatureScript
{
    public:
        npc_orphaned_whelp() : CreatureScript("npc_orphaned_whelp") { }

        CreatureAI * GetAI(Creature * creature) const
        {
            return new npc_orphaned_whelpAI(creature);
        }

        struct npc_orphaned_whelpAI : public ScriptedAI
        {
            npc_orphaned_whelpAI(Creature * creature) : ScriptedAI(creature)
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
                pInstance = (InstanceScript*)creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            uint8 count;

            void Reset()
            {
                if (!pInstance)
                    return;

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->setFaction(35);
                me->SetCanFly(false);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_WHELPS_RELEASE)
                {
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                    me->setFaction(16);
                    if (Creature* proto = me->FindNearestCreature(NPC_PROTO_BEHEMOTH, 200.0f))
                        DoCast(proto, SPELL_ATROPHIC_POISON, true);

                    DoZoneInCombat();
                }
            }

            void JustDied(Unit* /*Killer*/)
            {
                 if (Creature* pHalfus = me->FindNearestCreature(NPC_HALFUS_WYRMBREAKER, 200.0f))
                    pHalfus->AI()->DoAction(ACTION_WHELP_DIED);
            }
        };
};

class go_whelp_cage : public GameObjectScript
{
    public:
        go_whelp_cage() : GameObjectScript("go_whelp_cage") {}

        bool OnGossipHello(Player* /*pPlayer*/, GameObject* pGo)
        {
            InstanceScript* pInstance = pGo ? pGo->GetInstanceScript() : NULL;
            if (!pInstance)
                return true;

            if (Creature* halfus = ObjectAccessor::GetCreature(*pGo, pInstance->GetData64(DATA_HALFUS)))
            {
                halfus->AI()->DoAction(ACTION_WHELPS_RELEASE);
                pGo->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);
            }
            return false;
        }
};

class npc_halfus_dragon : public CreatureScript
{
    public:
        npc_halfus_dragon() : CreatureScript("npc_halfus_dragon") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_halfus_dragonAI(creature);
        }

        bool OnGossipHello(Player* pPlayer, Creature* creature)
        {
            if (creature->HasAura(SPELL_UNRESPONSIVE_DRAKE))
                return false;
            char const* _message = "Release drake!";
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,_message,GOSSIP_SENDER_MAIN ,GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(68,creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* pPlayer, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
        {
            pPlayer->PlayerTalkClass->ClearMenus();
            InstanceScript* pInstance;
            pInstance = (InstanceScript*)creature->GetInstanceScript();
            if (!pInstance)
                return false;
            pPlayer->CLOSE_GOSSIP_MENU();
            switch (uiAction)
            {
            case GOSSIP_ACTION_INFO_DEF+1:
                if (Creature * Halfus = Unit::GetCreature(*creature,pInstance->GetData64(DATA_HALFUS)))
                {
                    switch (creature->GetEntry())
                    {
                        case NPC_SLATE_DRAKE:
                            creature->CastSpell(Halfus, SPELL_STONE_GRIP, false);
                            break;
                        case NPC_NETHER_SCION:
                            creature->CastSpell(Halfus, SPELL_NETHER_BLINDNESS, false);
                            break;
                        case NPC_STORM_RIDER:
                            //creature->CastSpell(Halfus, SPELL_CYCLONE_WINDS_SUM, true);
                            creature->CastSpell(Halfus, SPELL_CYCLONE_WINDS, false);
                            break;
                        case NPC_TIME_WARDEN:
                            if (Creature* proto = creature->FindNearestCreature(NPC_PROTO_BEHEMOTH, 100.0f, true))
                                creature->CastSpell(proto, SPELL_TIME_DILATION, false);
                            break;
                    }

                    Halfus->CastSpell(creature, SPELL_BIND_WILL, true);
                    creature->SetReactState(REACT_AGGRESSIVE);
                    if (Unit* target = Halfus->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                        creature->AI()->AttackStart(target);
                }
                break;
            }
            return true;
        }

        struct npc_halfus_dragonAI : public ScriptedAI
        {
            npc_halfus_dragonAI(Creature * creature) : ScriptedAI(creature)
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

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->setFaction(35);
                me->SetCanFly(false);
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_FLYING);
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_ACTIVE_GOSSIP)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void JustDied(Unit* /*killer*/)
            {
                DoCast(me, SPELL_DRAGON_VENGEANCE, true);
            }
        };
};

class spell_halfus_stone_grip: public SpellScriptLoader
{
    public:
        spell_halfus_stone_grip() : SpellScriptLoader("spell_halfus_stone_grip") { }


        class spell_halfus_stone_grip_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_halfus_stone_grip_AuraScript)

            void OnPereodic(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();

                if (GetTarget())
                    GetTarget()->CastSpell(GetTarget(), SPELL_PARALYSIS, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_halfus_stone_grip_AuraScript::OnPereodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_halfus_stone_grip_AuraScript();
        }
};

class spell_halfus_fireball_barrage: public SpellScriptLoader
{
    public:
        spell_halfus_fireball_barrage() : SpellScriptLoader("spell_halfus_fireball_barrage") { }

        class spell_halfus_fireball_barrage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_halfus_fireball_barrage_SpellScript)

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                InstanceScript* pInstance = NULL;
                pInstance = GetCaster()->GetInstanceScript();
                if (!pInstance)
                    return;

                uint32 _spell = (GetCaster()->HasAura(SPELL_TIME_DILATION) ? SPELL_FIREBALL_BARRAGE_M0 : SPELL_FIREBALL_BARRAGE_M1);
                
                GetCaster()->CastSpell(GetHitUnit(), _spell, true);

            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_halfus_fireball_barrage_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_halfus_fireball_barrage_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_halfus_wyrmbreaker()
{
    new boss_halfus_wyrmbreaker();
    new npc_halfus_dragon();
    new npc_orphaned_whelp();
    new npc_proto_behemoth();
    new go_whelp_cage();
    new spell_halfus_stone_grip();
    new spell_halfus_fireball_barrage();
}
#endif
