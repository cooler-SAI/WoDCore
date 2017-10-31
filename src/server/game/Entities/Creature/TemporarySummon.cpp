////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Log.h"
#include "ObjectAccessor.h"
#include "CreatureAI.h"
#include "ObjectMgr.h"
#include "TemporarySummon.h"
#include "Totem.h"

TempSummon::TempSummon(SummonPropertiesEntry const* properties, Unit* owner, bool isWorldObject) :
Creature(isWorldObject), m_Properties(properties), m_type(TEMPSUMMON_MANUAL_DESPAWN),
m_timer(0), m_lifetime(0)
{
    m_summonerGUID = owner ? owner->GetGUID() : 0;
    m_unitTypeMask |= UNIT_MASK_SUMMON;
}

Unit* TempSummon::GetSummoner() const
{
    return m_summonerGUID ? ObjectAccessor::GetUnit(*this, m_summonerGUID) : NULL;
}

void TempSummon::Update(uint32 diff)
{
    Creature::Update(diff);

    if (m_deathState == DEAD)
    {
        UnSummon();
        return;
    }
    switch (m_type)
    {
        case TEMPSUMMON_MANUAL_DESPAWN:
            break;
        case TEMPSUMMON_TIMED_DESPAWN:
        {
            if (m_timer <= diff)
            {
                if (IsAIEnabled)
                    AI()->JustDespawned();

                UnSummon();
                return;
            }

            m_timer -= diff;
            break;
        }
        case TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT:
        {
            if (!isInCombat())
            {
                if (m_timer <= diff)
                {
                    UnSummon();
                    return;
                }

                m_timer -= diff;
            }
            else if (m_timer != m_lifetime)
                m_timer = m_lifetime;

            break;
        }

        case TEMPSUMMON_CORPSE_TIMED_DESPAWN:
        {
            if (m_deathState == CORPSE)
            {
                if (m_timer <= diff)
                {
                    UnSummon();
                    return;
                }

                m_timer -= diff;
            }
            break;
        }
        case TEMPSUMMON_CORPSE_DESPAWN:
        {
            // if m_deathState is DEAD, CORPSE was skipped
            if (m_deathState == CORPSE || m_deathState == DEAD)
            {
                UnSummon();
                return;
            }

            break;
        }
        case TEMPSUMMON_DEAD_DESPAWN:
        {
            if (m_deathState == DEAD)
            {
                UnSummon();
                return;
            }
            break;
        }
        case TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN:
        {
            // if m_deathState is DEAD, CORPSE was skipped
            if (m_deathState == CORPSE || m_deathState == DEAD)
            {
                UnSummon();
                return;
            }

            if (!isInCombat())
            {
                if (m_timer <= diff)
                {
                    UnSummon();
                    return;
                }
                else
                    m_timer -= diff;
            }
            else if (m_timer != m_lifetime)
                m_timer = m_lifetime;
            break;
        }
        case TEMPSUMMON_TIMED_OR_DEAD_DESPAWN:
        {
            // if m_deathState is DEAD, CORPSE was skipped
            if (m_deathState == DEAD)
            {
                UnSummon();
                return;
            }

            if (!isInCombat() && isAlive())
            {
                if (m_timer <= diff)
                {
                    UnSummon();
                    return;
                }
                else
                    m_timer -= diff;
            }
            else if (m_timer != m_lifetime)
                m_timer = m_lifetime;
            break;
        }
        default:
            UnSummon();
            sLog->outError(LOG_FILTER_UNITS, "Temporary summoned creature (entry: %u) have unknown type %u of ", GetEntry(), m_type);
            break;
    }
}

void TempSummon::InitStats(uint32 duration)
{
    ASSERT(!isPet());

    m_timer = duration;
    m_lifetime = duration;

    if (m_type == TEMPSUMMON_MANUAL_DESPAWN)
        m_type = (duration == 0) ? TEMPSUMMON_DEAD_DESPAWN : TEMPSUMMON_TIMED_DESPAWN;

    Unit* owner = GetSummoner();

    if (owner && isTrigger() && m_spells[0])
    {
        setFaction(owner->getFaction());
        SetLevel(owner->getLevel());
        if (owner->IsPlayer())
            m_ControlledByPlayer = true;
    }

    if (!m_Properties)
        return;

    if (owner)
    {
        if (uint32 slot = m_Properties->Slot)
        {
            // Totemic Persistence
            if (slot != SUMMON_SLOT_TOTEM && isTotem() && owner->HasAura(108284) && owner->m_SummonSlot[slot] && !owner->m_SummonSlot[slot + MAX_TOTEM_SLOT - SUMMON_SLOT_TOTEM])
                slot += MAX_TOTEM_SLOT - SUMMON_SLOT_TOTEM;

            if (owner->m_SummonSlot[slot] && owner->m_SummonSlot[slot] != GetGUID())
            {
                Creature* oldSummon = GetMap()->GetCreature(owner->m_SummonSlot[slot]);
                if (oldSummon && oldSummon->isSummon())
                {
                    if (oldSummon->isTotem())
                        oldSummon->ToTotem()->UnSummon();
                    else
                        oldSummon->ToTempSummon()->UnSummon();
                }
            }
            owner->m_SummonSlot[slot] = GetGUID();
        }
    }

    if (m_Properties->Faction)
        setFaction(m_Properties->Faction);
    else if (IsVehicle() && owner) // properties should be vehicle
        setFaction(owner->getFaction());
}

void TempSummon::InitSummon()
{
    Unit* owner = GetSummoner();
    if (owner)
    {
        if (owner->GetTypeId() == TYPEID_UNIT && owner->ToCreature()->IsAIEnabled)
            owner->ToCreature()->AI()->JustSummoned(this);
        if (IsAIEnabled)
            AI()->IsSummonedBy(owner);
    }
}

void TempSummon::SetTempSummonType(TempSummonType type)
{
    m_type = type;
}

void TempSummon::UnSummon(uint32 msTime)
{
    if (msTime)
    {
        ForcedUnsummonDelayEvent* pEvent = new ForcedUnsummonDelayEvent(*this);

        m_Events.AddEvent(pEvent, m_Events.CalculateTime(msTime));
        return;
    }

    //ASSERT(!isPet());
    if (isPet())
    {
        if (ToPet()->getPetType() == HUNTER_PET)
            ToPet()->Remove(PET_SLOT_ACTUAL_PET_SLOT, false, ToPet()->m_Stampeded);
        else
            ToPet()->Remove(PET_SLOT_OTHER_PET, false, ToPet()->m_Stampeded);
        ASSERT(!IsInWorld());
        return;
    }

    Unit* owner = GetSummoner();
    if (owner && owner->GetTypeId() == TYPEID_UNIT && owner->ToCreature()->IsAIEnabled)
        owner->ToCreature()->AI()->SummonedCreatureDespawn(this);

    AddObjectToRemoveList();
}

bool ForcedUnsummonDelayEvent::Execute(uint64 /*e_time*/, uint32 /*p_time*/)
{
    m_owner.UnSummon();
    return true;
}

void TempSummon::RemoveFromWorld()
{
    if (!IsInWorld())
        return;

    if (m_Properties)
        if (uint32 slot = m_Properties->Slot)
            if (Unit* owner = GetSummoner())
                if (owner->m_SummonSlot[slot] == GetGUID())
                    owner->m_SummonSlot[slot] = 0;

    //if (GetOwnerGUID())
    //    sLog->outError(LOG_FILTER_UNITS, "Unit %u has owner guid when removed from world", GetEntry());

    Creature::RemoveFromWorld();
}

Minion::Minion(SummonPropertiesEntry const* properties, Unit* owner, bool isWorldObject) : TempSummon(properties, owner, isWorldObject)
{
    m_unitTypeMask |= UNIT_MASK_MINION;
    m_followAngle = PET_FOLLOW_ANGLE;
}

void Minion::InitStats(uint32 duration)
{
    TempSummon::InitStats(duration);

    SetReactState(REACT_PASSIVE);

    if (Unit* l_Owner = GetSummoner())
    {
        SetCreatorGUID(l_Owner->GetGUID());

        setFaction(l_Owner->getFaction());
        l_Owner->SetMinion(this, true, PET_SLOT_UNK_SLOT, ToPet() ? ToPet()->m_Stampeded : false);
    }
}

void Minion::RemoveFromWorld()
{
    if (!IsInWorld())
        return;

    if (Unit* l_Owner = GetSummoner())
        l_Owner->SetMinion(this, false, PET_SLOT_UNK_SLOT, ToPet() ? ToPet()->m_Stampeded : false);

    TempSummon::RemoveFromWorld();
}

bool Minion::IsGuardianPet() const
{
    return isPet() || (m_Properties && m_Properties->Category == SUMMON_CATEGORY_PET);
}

Guardian::Guardian(SummonPropertiesEntry const* properties, Unit* owner, bool isWorldObject) : Minion(properties, owner, isWorldObject)
, m_bonusSpellDamage(0)
{
    memset(m_statFromOwner, 0, sizeof(float)*MAX_STATS);
    m_unitTypeMask |= UNIT_MASK_GUARDIAN;
    if (properties && properties->Type == SUMMON_TYPE_PET)
    {
        m_unitTypeMask |= UNIT_MASK_CONTROLABLE_GUARDIAN;
        InitCharmInfo();
    }
}

void Guardian::InitStats(uint32 duration)
{
    Minion::InitStats(duration);

    if (Unit* l_Owner = GetSummoner())
    {
        InitStatsForLevel(l_Owner->getLevel());

        if (l_Owner->IsPlayer() && HasUnitTypeMask(UNIT_MASK_CONTROLABLE_GUARDIAN))
            m_charmInfo->InitCharmCreateSpells();
    }

    SetReactState(REACT_AGGRESSIVE);
}

void Guardian::InitSummon()
{
    TempSummon::InitSummon();

    Unit* l_Owner = GetSummoner();
    if (l_Owner == nullptr)
        return;

    if (l_Owner->IsPlayer()
        && l_Owner->GetMinionGUID() == GetGUID()
        && !l_Owner->GetCharmGUID())
        l_Owner->ToPlayer()->CharmSpellInitialize();
}

PetStatInfo const* Guardian::GetPetStat(bool p_Force) const
{
    uint32 l_PetStatEntry = GetEntry();
    if (isPet() && ToPet()->getPetType() == HUNTER_PET)
        l_PetStatEntry = 1;

    PetStatInfo const* l_PetStat = sObjectMgr->GetPetStatInfo(l_PetStatEntry);
    if (l_PetStat == nullptr && (p_Force || isPet()))
        l_PetStat = sObjectMgr->GetPetStatInfo(0);

    return l_PetStat;
}

Puppet::Puppet(SummonPropertiesEntry const* properties, Unit* owner) : Minion(properties, owner, false) //maybe true?
{
    ASSERT(owner->IsPlayer());
    m_owner = (Player*)owner;
    m_unitTypeMask |= UNIT_MASK_PUPPET;
}

void Puppet::InitStats(uint32 duration)
{
    Minion::InitStats(duration);
    SetLevel(m_owner->getLevel());
    SetReactState(REACT_PASSIVE);
}

void Puppet::InitSummon()
{
    Minion::InitSummon();
    if (!SetCharmedBy(m_owner, CHARM_TYPE_POSSESS))
        ASSERT(false);
}

void Puppet::Update(uint32 time)
{
    Minion::Update(time);
    //check if caster is channelling?
    if (IsInWorld())
    {
        if (!isAlive())
        {
            UnSummon();
            // TODO: why long distance .die does not remove it
        }
    }
}

void Puppet::RemoveFromWorld()
{
    if (!IsInWorld())
        return;

    RemoveCharmedBy(NULL);
    Minion::RemoveFromWorld();
}
