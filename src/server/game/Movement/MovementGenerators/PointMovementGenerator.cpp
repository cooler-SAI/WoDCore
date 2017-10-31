////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "PointMovementGenerator.h"
#include "Errors.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "World.h"
#include "MoveSplineInit.h"
#include "MoveSpline.h"
#include "Player.h"
#include "CreatureGroups.h"
#include "ObjectAccessor.h"
#include <stack>

//----- Point Movement Generator
template<class T>
void PointMovementGenerator<T>::DoInitialize(T* unit)
{
    if (!unit->IsStopped())
        unit->StopMoving();

    unit->AddUnitState(UNIT_STATE_ROAMING|UNIT_STATE_ROAMING_MOVE);

    if (id == EVENT_CHARGE_PREPATH)
        return;

    Movement::MoveSplineInit init(unit);
    init.MoveTo(i_x, i_y, i_z, m_generatePath);
    if (m_Angle != -1000.0f)
        init.SetFacing(m_Angle);

    if (speed > 0.0f)
        init.SetVelocity(speed);
    init.Launch();

    // Call for creature group update
    if (Creature* creature = unit->ToCreature())
        if (creature->GetFormation() && creature->GetFormation()->getLeader() == creature)
            creature->GetFormation()->LeaderMoveTo(i_x, i_y, i_z);
}

template<class T>
bool PointMovementGenerator<T>::DoUpdate(T* unit, uint32 /*diff*/)
{
    if (!unit)
        return false;

    if (unit->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED))
    {
        unit->ClearUnitState(UNIT_STATE_ROAMING_MOVE);
        return true;
    }

    unit->AddUnitState(UNIT_STATE_ROAMING_MOVE);

    if (id != EVENT_CHARGE_PREPATH && i_recalculateSpeed && !unit->movespline->Finalized())
    {
        i_recalculateSpeed = false;
        Movement::MoveSplineInit init(unit);
        init.MoveTo(i_x, i_y, i_z, m_generatePath);
        if (speed > 0.0f) // Default value for point motion type is 0.0, if 0.0 spline will use GetSpeed on unit
            init.SetVelocity(speed);
        init.Launch();

        // Call for creature group update
        if (Creature* creature = unit->ToCreature())
            if (creature->GetFormation() && creature->GetFormation()->getLeader() == creature)
                creature->GetFormation()->LeaderMoveTo(i_x, i_y, i_z);
    }

    return !unit->movespline->Finalized();
}

template<class T>
void PointMovementGenerator<T>::DoFinalize(T* unit)
{
    if (unit->HasUnitState(UNIT_STATE_CHARGING))
        unit->ClearUnitState(UNIT_STATE_ROAMING | UNIT_STATE_ROAMING_MOVE);

    /// Update all passengers after updating vehicle position
    /// This will prevent some base positioning if vehicles are updated in the wrong order
    if (unit->GetVehicleBase())
    {
        std::stack<Unit*> l_RecursivesVehicles;
        Unit* l_Current = unit;

        do
        {
            l_Current = l_Current->GetVehicleBase();

            if (l_Current != nullptr)
                l_RecursivesVehicles.push(l_Current);
        }
        while (l_Current);

        while (!l_RecursivesVehicles.empty())
        {
            Unit* l_Vehicle = l_RecursivesVehicles.top();

            l_Vehicle->UpdateSplinePosition();
            l_RecursivesVehicles.pop();
        }

        unit->UpdateSplinePosition();
    }

    if (unit->movespline->Finalized())
        MovementInform(unit);
}

template<class T>
void PointMovementGenerator<T>::DoReset(T* unit)
{
    if (!unit->IsStopped())
        unit->StopMoving();

    unit->AddUnitState(UNIT_STATE_ROAMING|UNIT_STATE_ROAMING_MOVE);
}

template<class T>
void PointMovementGenerator<T>::MovementInform(T* /*unit*/) { }

enum specialSpells
{
    BABY_ELEPHANT_TAKES_A_BATH          = 108938,
    BABY_ELEPHANT_TAKES_A_BATH_2        = 108937,
    MONK_CLASH                          = 126452,
    MONK_CLASH_IMPACT                   = 126451,
    TOT_RETRIEVE_SPEAR                  = 137070,
    TOT_RETRIEVE_SPEAR_DAMAGE           = 137072,

    // Jin'Rokh the Breaker - ToT
    SPELL_THUNDERING_THROW_JUMP_DEST    = 137173,
    SPELL_THUNDERING_THROW_VEHICLE      = 137161,
    SPELL_THUNDERING_THROW_AOE          = 137167,
    SPELL_THUNDERING_THROW_STUN_PLAYER  = 137371,

    // Horridon - ToT
    SPELL_HORRIDON_CHARGE               = 136769,
    SPELL_DOUBLE_SWIPE                  = 136741,

    /// Kargath Bladefist - Highmaul
    ChainHurlJumpDest                   = 160061,
    ChainHurlKnockBack                  = 160062,
    ChainHurlJumpDestPlayer             = 159995,
    ChainHurlAoEStunPlayer              = 160904,
    ChainHurl                           = 159947,
    AnimChainHurl                       = 5917,

    /// Krush - Highmaul
    BoarsRuchJump                       = 166225,
    Winded                              = 166227,

    /// Ogron Earthshaker - Highmaul
    IntimidatingRoarJump                = 166170,
    IntimidatingRoarFear                = 166171,
    Squash                              = 166172
};

template <> void PointMovementGenerator<Creature>::MovementInform(Creature* unit)
{
    if (unit->AI())
        unit->AddMovementInform(POINT_MOTION_TYPE, id);

    switch (id)
    {
        case BABY_ELEPHANT_TAKES_A_BATH:
            unit->CastSpell(unit, BABY_ELEPHANT_TAKES_A_BATH_2, true);
            break;
        case SPELL_HORRIDON_CHARGE:
            unit->CastSpell(unit, SPELL_DOUBLE_SWIPE, false);
            break;
        default:
            break;
    }
}

template <> void PointMovementGenerator<Player>::MovementInform(Player* unit)
{
    sScriptMgr->OnPlayerMovementInform(unit, POINT_MOTION_TYPE, id);

    switch (id)
    {
        case MONK_CLASH:
            unit->CastSpell(unit, MONK_CLASH_IMPACT, true);
            break;
        default:
            break;
    }
}

template void PointMovementGenerator<Player>::DoInitialize(Player*);
template void PointMovementGenerator<Creature>::DoInitialize(Creature*);
template void PointMovementGenerator<Player>::DoFinalize(Player*);
template void PointMovementGenerator<Creature>::DoFinalize(Creature*);
template void PointMovementGenerator<Player>::DoReset(Player*);
template void PointMovementGenerator<Creature>::DoReset(Creature*);
template bool PointMovementGenerator<Player>::DoUpdate(Player*, uint32);
template bool PointMovementGenerator<Creature>::DoUpdate(Creature*, uint32);

void AssistanceMovementGenerator::Finalize(Unit* unit)
{
    unit->ToCreature()->SetNoCallAssistance(false);
    unit->ToCreature()->CallAssistance();
    if (unit->isAlive())
        unit->GetMotionMaster()->MoveSeekAssistanceDistract(sWorld->getIntConfig(CONFIG_CREATURE_FAMILY_ASSISTANCE_DELAY));
}

bool EffectMovementGenerator::Update(Unit* p_Unit, uint32)
{
    if (p_Unit->movespline->Finalized())
    {
        if (Player* l_Player = p_Unit->ToPlayer())
            sScriptMgr->OnFinishMovement(l_Player, _id, _arrivalSpellTargetGuid);
    }
    return !p_Unit->movespline->Finalized();
}

void EffectMovementGenerator::Finalize(Unit* unit)
{
    if (_arrivalSpellId)
        unit->CastSpell(ObjectAccessor::GetUnit(*unit, _arrivalSpellTargetGuid), _arrivalSpellId, true);

    if (unit->GetTypeId() != TYPEID_UNIT)
        return;

    // Need restore previous movement since we have no proper states system
    if (unit->isAlive() && !unit->HasUnitState(UNIT_STATE_CONFUSED | UNIT_STATE_FLEEING))
    {
        if (Unit* victim = unit->getVictim())
            unit->GetMotionMaster()->MoveChase(victim);
        else
            unit->GetMotionMaster()->Initialize();
    }

    MovementInform(unit);
}

void EffectMovementGenerator::MovementInform(Unit* unit)
{
    if (unit->GetTypeId() == TYPEID_UNIT)
    {
        Creature* creature = unit->ToCreature();

        if (creature->AI())
            creature->AddMovementInform(EFFECT_MOTION_TYPE, _id);
    }

    switch (_id)
    {
        case TOT_RETRIEVE_SPEAR:
            unit->CastSpell(unit, TOT_RETRIEVE_SPEAR_DAMAGE, true);
            break;
        case SPELL_THUNDERING_THROW_JUMP_DEST:
            if (unit->HasAura(SPELL_THUNDERING_THROW_VEHICLE))
                break;
            unit->CastSpell(unit, SPELL_THUNDERING_THROW_AOE, true);
            unit->CastSpell(unit, SPELL_THUNDERING_THROW_STUN_PLAYER, true);
            break;
        case specialSpells::ChainHurlJumpDest:
            unit->CastSpell(unit, specialSpells::ChainHurlKnockBack, true);
            unit->CastSpell(unit, specialSpells::ChainHurl, false);
            unit->PlayOneShotAnimKit(specialSpells::AnimChainHurl);
            unit->SetControlled(true, UnitState::UNIT_STATE_ROOT);
            break;
        case specialSpells::ChainHurlJumpDestPlayer:
            unit->CastSpell(unit, specialSpells::ChainHurlAoEStunPlayer, true);
            break;
        case specialSpells::BoarsRuchJump:
            unit->CastSpell(unit, specialSpells::Winded, true);
            unit->ClearUnitState(UnitState::UNIT_STATE_ROOT);
            break;
        case specialSpells::IntimidatingRoarJump:
            unit->CastSpell(unit, specialSpells::Squash, true);
            unit->CastSpell(unit, specialSpells::IntimidatingRoarFear, false);
            break;
        default:
            break;
    }
}
