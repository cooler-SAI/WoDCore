////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "UpdateMask.h"
#include "Opcodes.h"
#include "World.h"
#include "ObjectAccessor.h"
#include "DatabaseEnv.h"
#include "GridNotifiers.h"
#include "CellImpl.h"
#include "GridNotifiersImpl.h"
#include "ScriptMgr.h"

DynamicObject::DynamicObject(bool isWorldObject) : WorldObject(isWorldObject),
    _aura(NULL), _removedAura(NULL), _caster(NULL), _duration(0), _isViewpoint(false)
{
    m_objectType |= TYPEMASK_DYNAMICOBJECT;
    m_objectTypeId = TYPEID_DYNAMICOBJECT;

    m_updateFlag = UPDATEFLAG_HAS_POSITION;

    m_valuesCount = DYNAMICOBJECT_END;
    _dynamicValuesCount = DYNAMICOBJECT_DYNAMIC_END;
}

DynamicObject::~DynamicObject()
{
    // make sure all references were properly removed
    ASSERT(!_aura);
    ASSERT(!_caster);
    ASSERT(!_isViewpoint);
}

void DynamicObject::AddToWorld()
{
    ///- Register the dynamicObject for guid lookup and for caster
    if (!IsInWorld())
    {
        sObjectAccessor->AddObject(this);
        WorldObject::AddToWorld();
        BindToCaster();
    }
}

void DynamicObject::RemoveFromWorld()
{
    ///- Remove the dynamicObject from the accessor and from all lists of objects in world
    if (IsInWorld())
    {
        if (_isViewpoint)
            RemoveCasterViewpoint();

        if (_aura)
            RemoveAura();

        // dynobj could get removed in Aura::RemoveAura
        if (!IsInWorld())
            return;

        UnbindFromCaster();
        WorldObject::RemoveFromWorld();
        sObjectAccessor->RemoveObject(this);
    }
}

bool DynamicObject::CreateDynamicObject(uint32 guidlow, Unit* caster, SpellInfo const* spell, Position const& pos, float radius, DynamicObjectType type)
{
    _spell = spell;
    _type = type;

    SetMap(caster->GetMap());
    Relocate(pos);
    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_GENERAL, "DynamicObject (spell %u) not created. Suggested coordinates isn't valid (X: %f Y: %f)", spell->Id, GetPositionX(), GetPositionY());
        return false;
    }

    WorldObject::_Create(guidlow, HIGHGUID_DYNAMICOBJECT, caster->GetPhaseMask());

    SetEntry(spell->Id);
    SetObjectScale(1.0f);
    SetGuidValue(DYNAMICOBJECT_FIELD_CASTER, caster->GetGUID());

    // The lower word of DYNAMICOBJECT_FIELD_TYPE_AND_VISUAL_ID must be 0x0001. This value means that the visual radius will be overriden
    // by client for most of the "ground patch" visual effect spells and a few "skyfall" ones like Hurricane.
    // If any other value is used, the client will _always_ use the radius provided in DYNAMICOBJECT_RADIUS, but
    // precompensation is necessary (eg radius *= 2) for many spells. Anyway, blizz sends 0x0001 for all the spells
    // I saw sniffed...

    // Blizz set visual spell Id in 3 first byte of DYNAMICOBJECT_FIELD_TYPE_AND_VISUAL_ID after 5.X
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell->Id);
    if (spellInfo)
    {
        uint32 const* l_VisualID = nullptr;
        if (SpellXSpellVisualEntry const* l_VisualEntry = sSpellXSpellVisualStore.LookupEntry(spellInfo->GetSpellXSpellVisualId(caster)))
            l_VisualID = l_VisualEntry->VisualID;

        uint32 visual = l_VisualID != nullptr ? (l_VisualID[0] ? l_VisualID[0] : l_VisualID[1]) : 0;
        if (spellInfo->Id == 104232 || spellInfo->Id == 5740) // Rain of Fire
        {
            if (caster->HasAura(101508)) // green fire spells
                visual = 25761;
        }

        SetUInt32Value(DYNAMICOBJECT_FIELD_TYPE_AND_VISUAL_ID, 0x10000000 | visual);
    }

    SetUInt32Value(DYNAMICOBJECT_FIELD_SPELL_ID, spell->Id);
    SetFloatValue(DYNAMICOBJECT_FIELD_RADIUS, radius);
    SetUInt32Value(DYNAMICOBJECT_FIELD_CAST_TIME, getMSTime());

    if (IsWorldObject())
        setActive(true);    //must before add to map to be put in world container

    if (!GetMap()->AddToMap(this))
        return false;

    return true;
}

void DynamicObject::Update(uint32 p_time)
{
    // caster has to be always available and in the same map
    ASSERT(_caster);
    ASSERT(_caster->GetMap() == GetMap());

    bool expired = false;

    if (_aura)
    {
        if (!_aura->IsRemoved())
            _aura->UpdateOwner(p_time, this);

        // _aura may be set to null in Aura::UpdateOwner call
        if (_aura && (_aura->IsRemoved() || _aura->IsExpired()))
            expired = true;
    }
    else
    {
        if (GetDuration() > int32(p_time))
            _duration -= p_time;
        else
            expired = true;
    }

    if (expired)
        Remove();
    else
        sScriptMgr->OnDynamicObjectUpdate(this, p_time);
}

void DynamicObject::Remove()
{
    if (IsInWorld())
    {
        SendObjectDeSpawnAnim(GetGUID());
        RemoveFromWorld();
        AddObjectToRemoveList();
    }
}

int32 DynamicObject::GetDuration() const
{
    if (!_aura)
        return _duration;
    else
        return _aura->GetDuration();
}

void DynamicObject::SetDuration(int32 newDuration)
{
    if (!_aura)
        _duration = newDuration;
    else
        _aura->SetDuration(newDuration);
}

void DynamicObject::Delay(int32 delaytime)
{
    SetDuration(GetDuration() - delaytime);
}

void DynamicObject::SetAura(Aura* aura)
{
    ASSERT(!_aura && aura);
    _aura = aura;
}

void DynamicObject::RemoveAura()
{
    ASSERT(_aura && !_removedAura);
    _removedAura = _aura;
    _aura = nullptr;
    if (!_removedAura->IsRemoved())
        _removedAura->_Remove(AURA_REMOVE_BY_DEFAULT);
}

void DynamicObject::SetCasterViewpoint()
{
    if (Player* caster = _caster->ToPlayer())
    {
        caster->SetViewpoint(this, true);
        _isViewpoint = true;
    }
}

void DynamicObject::RemoveCasterViewpoint()
{
    if (Player* caster = _caster->ToPlayer())
    {
        caster->SetViewpoint(this, false);
        _isViewpoint = false;
    }
}

void DynamicObject::BindToCaster()
{
    //ASSERT(!_caster);
    _caster = ObjectAccessor::GetUnit(*this, GetCasterGUID());
    //ASSERT(_caster);
    //ASSERT(_caster->GetMap() == GetMap());
    if (_caster)
        _caster->_RegisterDynObject(this);
}

void DynamicObject::UnbindFromCaster()
{
    //ASSERT(_caster);
    _caster->_UnregisterDynObject(this);
    _caster = NULL;
}
