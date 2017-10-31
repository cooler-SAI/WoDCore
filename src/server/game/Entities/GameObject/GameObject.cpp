////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include <G3D/Quat.h>
#include "GameObjectAI.h"
#include "ObjectMgr.h"
#include "GroupMgr.h"
#include "PoolMgr.h"
#include "SpellMgr.h"
#include "World.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "OutdoorPvPMgr.h"
#include "BattlegroundAV.h"
#include "ScriptMgr.h"
#include "CreatureAISelector.h"
#include "Group.h"
#include "MapManager.h"
#include "GameObjectModel.h"
#include "DynamicTree.h"
#include "SpellAuraEffects.h"
#include "UpdateFieldFlags.h"
#include "Transport.h"

GameObject::GameObject() : WorldObject(false), MapObject(),
m_model(NULL), m_goValue(new GameObjectValue), m_AI(NULL)
{
    m_objectType |= TYPEMASK_GAMEOBJECT;
    m_objectTypeId = TYPEID_GAMEOBJECT;

    m_updateFlag = (UPDATEFLAG_HAS_POSITION | UPDATEFLAG_HAS_ROTATION);

    m_valuesCount = GAMEOBJECT_END;
    _dynamicValuesCount = GAMEOBJECT_DYNAMIC_END;
    m_respawnTime = 0;
    m_respawnDelayTime = 300;
    m_lootState = GO_NOT_READY;
    m_spawnedByDefault = true;
    m_usetimes = 0;
    m_spellId = 0;
    m_cooldownTime = 0;
    m_goInfo = NULL;
    m_ritualOwner = NULL;
    m_goData = NULL;

    m_DBTableGuid = 0;
    m_Rotation = 0;

    m_lootRecipient = 0;
    m_lootRecipientGroup = 0;
    m_groupLootTimer = 0;
    lootingGroupLowGUID = 0;

    m_CustomFlags = eGoBCustomFlags::CustomFlagNone;

    m_AllowAnim = true;

    ResetLootMode(); // restore default loot mode
}

GameObject::~GameObject()
{
    if (m_goInfo != nullptr && m_goInfo->type == GAMEOBJECT_TYPE_TRANSPORT)
        delete m_goValue->Transport.StopFrames;

    delete m_goValue;
    delete m_AI;
    delete m_model;

    if (GetTransport() && !ToTransport())
        GetTransport()->RemovePassenger(this);
}

bool GameObject::AIM_Initialize()
{
    if (m_AI)
        delete m_AI;

    m_AI = FactorySelector::SelectGameObjectAI(this);

    if (!m_AI)
        return false;

    m_AI->InitializeAI();
    return true;
}

std::string GameObject::GetAIName() const
{
    if (GameObjectTemplate const* got = sObjectMgr->GetGameObjectTemplate(GetEntry()))
        return got->AIName;

    return "";
}

void GameObject::CleanupsBeforeDelete(bool /*finalCleanup*/)
{
    if (IsInWorld())
        RemoveFromWorld();

    if (m_uint32Values)                                      // field array can be not exist if GameOBject not loaded
        RemoveFromOwner();

    m_Events.KillAllEvents(false);
}

void GameObject::RemoveFromOwner()
{
    uint64 ownerGUID = GetOwnerGUID();
    if (!ownerGUID)
        return;

    if (Unit* owner = ObjectAccessor::GetUnit(*this, ownerGUID))
    {
        owner->RemoveGameObject(this, false);
        ASSERT(!GetOwnerGUID());
        return;
    }

    const char* ownerType = "creature";
    if (IS_PLAYER_GUID(ownerGUID))
        ownerType = "player";
    else if (IS_PET_GUID(ownerGUID))
        ownerType = "pet";

    sLog->outFatal(LOG_FILTER_GENERAL, "Delete GameObject (GUID: %u Entry: %u SpellId %u LinkedGO %u) that lost references to owner (GUID %u Type '%s') GO list. Crash possible later.",
        GetGUIDLow(), GetGOInfo()->entry, m_spellId, GetGOInfo()->GetLinkedGameObjectEntry(), GUID_LOPART(ownerGUID), ownerType);
    SetOwnerGUID(0);
}

void GameObject::AddToWorld()
{
    ///- Register the gameobject for guid lookup
    if (!IsInWorld())
    {
        if (m_zoneScript)
            m_zoneScript->OnGameObjectCreate(this);

        sObjectAccessor->AddObject(this);

        if (GetGoType() == GAMEOBJECT_TYPE_TRANSPORT)
        {
            GetMap()->AddGameObjectTransport(this);
            SendTransportToOutOfRangePlayers();
        }

        // The state can be changed after GameObject::Create but before GameObject::AddToWorld
        bool toggledState = GetGoType() == GAMEOBJECT_TYPE_CHEST ? getLootState() == GO_READY : GetGoState() == GO_STATE_READY;
        if (m_model)
        {
            if (Transport* trans = ToTransport())
                trans->SetDelayedAddModelToMap();
            else
                GetMap()->InsertGameObjectModel(*m_model);
        }

        EnableCollision(toggledState);
        WorldObject::AddToWorld();
    }
}

void GameObject::RemoveFromWorld()
{
    ///- Remove the gameobject from the accessor
    if (IsInWorld())
    {
        if (m_zoneScript)
            m_zoneScript->OnGameObjectRemove(this);

        RemoveFromOwner();
        if (m_model)
            if (GetMap()->ContainsGameObjectModel(*m_model))
                GetMap()->RemoveGameObjectModel(*m_model);

        GetMap()->DeleteGameObjectTransport(this);

        WorldObject::RemoveFromWorld();
        sObjectAccessor->RemoveObject(this);
    }
}

bool GameObject::Create(uint32 guidlow, uint32 name_id, Map* map, uint32 phaseMask, float x, float y, float z, float ang, float rotation0, float rotation1, float rotation2, float rotation3, uint32 animprogress, GOState go_state, uint32 artKit, uint32 p_GoHealth)
{
    {
        GameObjectTemplate const* l_GameObjectTemplate = sObjectMgr->GetGameObjectTemplate(name_id);

        if (l_GameObjectTemplate && l_GameObjectTemplate->type == GAMEOBJECT_TYPE_MAP_OBJ_TRANSPORT)
        {
            sLog->outAshran("GameObject::Create called with an GAMEOBJECT_TYPE_MAP_OBJ_TRANSPORT template %u", name_id);
            return false;
        }
    }

    ASSERT(map);
    SetMap(map);

    Relocate(x, y, z, ang);
    m_stationaryPosition.Relocate(x, y, z, ang);

    if (!IsPositionValid())
    {
        sLog->outError(LOG_FILTER_GENERAL, "Gameobject (GUID: %u Entry: %u) not created. Suggested coordinates isn't valid (X: %f Y: %f)", guidlow, name_id, x, y);
        return false;
    }

    SetPhaseMask(phaseMask, false);

    SetZoneScript();
    if (m_zoneScript)
    {
        name_id = m_zoneScript->GetGameObjectEntry(guidlow, name_id);
        if (!name_id)
            return false;
    }

    GameObjectTemplate const* goinfo = sObjectMgr->GetGameObjectTemplate(name_id);
    if (!goinfo)
    {
        sLog->outError(LOG_FILTER_SQL, "Gameobject (GUID: %u Entry: %u) not created: non-existing entry in `gameobject_template`. Map: %u (X: %f Y: %f Z: %f)", guidlow, name_id, map->GetId(), x, y, z);
        return false;
    }

    if (goinfo->type == GAMEOBJECT_TYPE_TRANSPORT)
        m_updateFlag |= UPDATEFLAG_HAS_SERVER_TIME;

    if (goinfo->WorldEffectID)
        m_updateFlag |= UPDATEFLAG_HAS_GAMEOBJECT;

    if (goinfo->type == GAMEOBJECT_TYPE_TRANSPORT)
        Object::_Create(guidlow, 0, HIGHGUID_MO_TRANSPORT);
    else
        Object::_Create(guidlow, goinfo->entry, HIGHGUID_GAMEOBJECT);

    m_goInfo = goinfo;

    if (goinfo->type >= MAX_GAMEOBJECT_TYPE)
    {
        sLog->outError(LOG_FILTER_SQL, "Gameobject (GUID: %u Entry: %u) not created: non-existing GO type '%u' in `gameobject_template`. It will crash client if created.", guidlow, name_id, goinfo->type);
        return false;
    }

    if (GetCustomFlags() & eGoBCustomFlags::CustomFlagUseQuaternion)
        SetRotationQuat(rotation0, rotation1, rotation2, rotation3);
    else
    {
        SetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION + 0, rotation0);
        SetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION + 1, rotation1);

        UpdateRotationFields(rotation2, rotation3);              // GAMEOBJECT_FACING, GAMEOBJECT_ROTATION, GAMEOBJECT_PARENTROTATION+2/3
    }

    SetObjectScale(goinfo->size);

    SetUInt32Value(GAMEOBJECT_FIELD_FACTION_TEMPLATE, goinfo->faction);
    SetUInt32Value(GAMEOBJECT_FIELD_FLAGS, goinfo->flags);

    SetEntry(goinfo->entry);

    // set name for logs usage, doesn't affect anything ingame
    SetName(goinfo->name);

    SetDisplayId(goinfo->displayId);
    m_model = GameObjectModel::Create(*this);

    /// Recall this function to update the model
    SetPhaseMask(phaseMask, false);

    loot.SetSource(GetGUID());

    // GAMEOBJECT_FIELD_PERCENT_HEALTH, index at 0, 1, 2 and 3
    SetGoType(GameobjectTypes(goinfo->type));
    SetGoState(go_state);
    SetGoArtKit(artKit);
    SetGoHealth(p_GoHealth);

    LastUsedScriptID = GetGOInfo()->ScriptId;
    AIM_Initialize();

    loot.Context = map->GetLootItemContext();

    switch (goinfo->type)
    {
            // @todo On port intactNumHits is removed
        case GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING:
            m_goValue->Building.Health = goinfo->building.intactNumHits + goinfo->building.InteriorVisible;
            m_goValue->Building.MaxHealth = m_goValue->Building.Health;
            SetGoHealth(255);
            SetUInt32Value(GAMEOBJECT_FIELD_PARENT_ROTATION, m_goInfo->building.DestructibleModelRec);
            break;
        case GAMEOBJECT_TYPE_TRANSPORT:
            m_goValue->Transport.AnimationInfo = sTransportMgr->GetTransportAnimInfo(goinfo->entry);
            m_goValue->Transport.PathProgress = getMSTime();

            if (m_goValue->Transport.AnimationInfo)
                m_goValue->Transport.PathProgress -= m_goValue->Transport.PathProgress % GetTransportPeriod();

            m_goValue->Transport.CurrentSeg           = 0;
            m_goValue->Transport.StateUpdateTimer     = 0;
            m_goValue->Transport.StopFrames           = new std::vector<uint32>();

            if (goinfo->transport.Timeto2ndfloor > 0)
                m_goValue->Transport.StopFrames->push_back(goinfo->transport.Timeto2ndfloor);
            if (goinfo->transport.Timeto3rdfloor > 0)
                m_goValue->Transport.StopFrames->push_back(goinfo->transport.Timeto3rdfloor);
            if (goinfo->transport.Timeto4thfloor > 0)
                m_goValue->Transport.StopFrames->push_back(goinfo->transport.Timeto4thfloor);
            if (goinfo->transport.Timeto5thfloor > 0)
                m_goValue->Transport.StopFrames->push_back(goinfo->transport.Timeto5thfloor);
            if (goinfo->transport.Timeto6thfloor > 0)
                m_goValue->Transport.StopFrames->push_back(goinfo->transport.Timeto6thfloor);
            if (goinfo->transport.Timeto7thfloor > 0)
                m_goValue->Transport.StopFrames->push_back(goinfo->transport.Timeto7thfloor);
            if (goinfo->transport.Timeto8thfloor > 0)
                m_goValue->Transport.StopFrames->push_back(goinfo->transport.Timeto8thfloor);
            if (goinfo->transport.Timeto9thfloor > 0)
                m_goValue->Transport.StopFrames->push_back(goinfo->transport.Timeto9thfloor);
            if (goinfo->transport.Timeto10thfloor > 0)
                m_goValue->Transport.StopFrames->push_back(goinfo->transport.Timeto10thfloor);

            if (goinfo->transport.startOpen && sScriptMgr->OnGameObjectElevatorCheck(this))
                SetTransportState(GO_STATE_TRANSPORT_STOPPED, goinfo->transport.startOpen - 1);
            else
                SetGoState(GO_STATE_TRANSPORT_ACTIVE);

            SetGoAnimProgress(0xFF);
            break;
        case GAMEOBJECT_TYPE_FISHINGNODE:
            SetGoAnimProgress(0);
            break;
        case GAMEOBJECT_TYPE_TRAP:
            if (GetGOInfo()->trap.stealthed)
            {
                m_stealth.AddFlag(STEALTH_TRAP);
                m_stealth.AddValue(STEALTH_TRAP, 70);
            }

            if (GetGOInfo()->trap.stealthAffected)
            {
                m_invisibility.AddFlag(INVISIBILITY_TRAP);
                m_invisibility.AddValue(INVISIBILITY_TRAP, 300);
            }
            break;
        default:
            SetGoAnimProgress(animprogress);
            break;
    }

    return true;
}

uint32 GameObject::GetTransportPeriod() const
{
    ASSERT(GetGOInfo()->type == GAMEOBJECT_TYPE_TRANSPORT);
    if (m_goValue->Transport.AnimationInfo)
        return m_goValue->Transport.AnimationInfo->TotalTime;

    // return something that will nicely divide for GAMEOBJECT_DYNAMIC value calculation
    return m_goValue->Transport.PathProgress;
}

void GameObject::SetTransportState(GOState state, uint32 stopFrame /*= 0*/)
{
    if (GetGoState() == state)
        return;

    ASSERT(GetGOInfo()->type == GAMEOBJECT_TYPE_TRANSPORT);
    ASSERT(state >= GO_STATE_TRANSPORT_ACTIVE);
    if (state == GO_STATE_TRANSPORT_ACTIVE)
    {
        m_goValue->Transport.StateUpdateTimer = 0;
        m_goValue->Transport.PathProgress =  getMSTime();
        if (GetGoState() >= GO_STATE_TRANSPORT_STOPPED)
            m_goValue->Transport.PathProgress += m_goValue->Transport.StopFrames->at(GetGoState() - GO_STATE_TRANSPORT_STOPPED);
        SetGoState(GO_STATE_TRANSPORT_ACTIVE);
    }
    else
    {
        ASSERT(state < GO_STATE_TRANSPORT_STOPPED + MAX_GO_STATE_TRANSPORT_STOP_FRAMES);
        ASSERT(stopFrame < m_goValue->Transport.StopFrames->size());
        m_goValue->Transport.PathProgress = getMSTime() + m_goValue->Transport.StopFrames->at(stopFrame);
        SetGoState(GOState(GO_STATE_TRANSPORT_STOPPED + stopFrame));
    }
}

void GameObject::Update(uint32 diff)
{
    m_Events.Update(diff);

    if (!AI())
    {
        if (!AIM_Initialize())
            sLog->outError(LOG_FILTER_GENERAL, "Could not initialize GameObjectAI");
    }

    if (AI())
        AI()->UpdateAI(diff);
    else if (!AIM_Initialize())
        sLog->outError(LOG_FILTER_GENERAL, "Could not initialize GameObjectAI");

    if (GetGoType() == GAMEOBJECT_TYPE_TRANSPORT)
    {
        if (m_goValue->Transport.AnimationInfo != nullptr)
        {
            /// Update PathProgress at every tick, and send update to client every 20 sec (blizz value, find in retail sniff 6.0.x)
            if (GetGoState() == GO_STATE_TRANSPORT_ACTIVE)
            {
                m_goValue->Transport.PathProgress += diff;

                // TODO: Fix movement in unloaded grid - currently GO will just disappear
                /*{
                    uint32 timer = m_goValue->Transport.PathProgress % m_goValue->Transport.AnimationInfo->TotalTime;
                    TransportAnimationEntry const* node = m_goValue->Transport.AnimationInfo->GetAnimNode(timer);
                    if (node && m_goValue->Transport.CurrentSeg != node->TimeSeg)
                    {
                        m_goValue->Transport.CurrentSeg = node->TimeSeg;

                        G3D::Quat rotation = m_goValue->Transport.AnimationInfo->GetAnimRotation(timer);
                        G3D::Vector3 pos = rotation.toRotationMatrix()
                            * G3D::Matrix3::fromEulerAnglesZYX(GetOrientation(), 0.0f, 0.0f)
                            * G3D::Vector3(node->X, node->Y, node->Z);

                        pos += G3D::Vector3(GetStationaryX(), GetStationaryY(), GetStationaryZ());

                        G3D::Vector3 src(GetPositionX(), GetPositionY(), GetPositionZ());

                        GetMap()->GameObjectRelocation(this, pos.x, pos.y, pos.z, GetOrientation());
                    }
                }*/

                if (!m_goValue->Transport.StopFrames->empty() && sScriptMgr->OnGameObjectElevatorCheck(this))
                {
                    uint32 visualStateBefore = (m_goValue->Transport.StateUpdateTimer / 20000) & 1;
                    m_goValue->Transport.StateUpdateTimer += diff;
                    uint32 visualStateAfter = (m_goValue->Transport.StateUpdateTimer / 20000) & 1;

                    /// Force send SMSG_UPDATE_OBJECT with updatefield
                    if (visualStateBefore != visualStateAfter)
                    {
                        ForceValuesUpdateAtIndex(GAMEOBJECT_FIELD_LEVEL);
                        ForceValuesUpdateAtIndex(GAMEOBJECT_FIELD_PERCENT_HEALTH);
                    }
                }
            }
        }
    }

    switch (m_lootState)
    {
        case GO_NOT_READY:
        {
            switch (GetGoType())
            {
                case GAMEOBJECT_TYPE_TRAP:
                {
                    // Arming Time for GAMEOBJECT_TYPE_TRAP (6)
                    GameObjectTemplate const* goInfo = GetGOInfo();
                    // Bombs
                    if (goInfo->trap.charges == 2)
                        m_cooldownTime = time(NULL) + 10;   // Hardcoded tooltip value
                    else if (Unit* owner = GetOwner()) ///< owner is unused
                    {
                        m_cooldownTime = time(NULL) + goInfo->trap.startDelay;
                    }
                    m_lootState = GO_READY;
                    break;
                }
                case GAMEOBJECT_TYPE_FISHINGNODE:
                {
                    // fishing code (bobber ready)
                    if (time(NULL) > m_respawnTime - FISHING_BOBBER_READY_TIME)
                    {
                        // splash bobber (bobber ready now)
                        Unit* caster = GetOwner();
                        if (caster && caster->IsPlayer())
                        {
                            SetGoState(GO_STATE_ACTIVE);
                            SetUInt32Value(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NODESPAWN);

                            UpdateData udata(caster->GetMapId());
                            WorldPacket packet;
                            BuildValuesUpdateBlockForPlayer(&udata, caster->ToPlayer());
                            if (udata.BuildPacket(&packet))
                                caster->ToPlayer()->GetSession()->SendPacket(&packet);

                            SendCustomAnim(GetGoAnimProgress());
                        }

                        m_lootState = GO_READY;                 // can be successfully open with some chance
                    }
                    return;
                }
                default:
                    m_lootState = GO_READY;                         // for other GOis same switched without delay to GO_READY
                    break;
            }
            // NO BREAK for switch (m_lootState)
        }
        case GO_READY:
        {
            if (m_respawnTime > 0)                          // timer on
            {
                time_t now = time(NULL);
                if (m_respawnTime <= now)            // timer expired
                {
                    uint64 dbtableHighGuid = MAKE_NEW_GUID(m_DBTableGuid, GetEntry(), HIGHGUID_GAMEOBJECT);
                    time_t linkedRespawntime = GetMap()->GetLinkedRespawnTime(dbtableHighGuid);
                    if (linkedRespawntime)             // Can't respawn, the master is dead
                    {
                        uint64 targetGuid = sObjectMgr->GetLinkedRespawnGuid(dbtableHighGuid);
                        if (targetGuid == dbtableHighGuid) // if linking self, never respawn (check delayed to next day)
                            SetRespawnTime(DAY);
                        else
                            m_respawnTime = (now > linkedRespawntime ? now : linkedRespawntime)+urand(5, MINUTE); // else copy time from master and add a little
                        SaveRespawnTime(); // also save to DB immediately
                        return;
                    }

                    m_respawnTime = 0;
                    m_SkillupList.clear();
                    m_usetimes = 0;

                    switch (GetGoType())
                    {
                        case GAMEOBJECT_TYPE_FISHINGNODE:   //  can't fish now
                        {
                            Unit* caster = GetOwner();
                            if (caster && caster->IsPlayer())
                            {
                                caster->FinishSpell(CURRENT_CHANNELED_SPELL);

                                WorldPacket data(SMSG_FISH_ESCAPED, 0);
                                caster->ToPlayer()->GetSession()->SendPacket(&data);
                            }
                            // can be delete
                            m_lootState = GO_JUST_DEACTIVATED;
                            return;
                        }
                        case GAMEOBJECT_TYPE_DOOR:
                        case GAMEOBJECT_TYPE_BUTTON:
                            //we need to open doors if they are closed (add there another condition if this code breaks some usage, but it need to be here for battlegrounds)
                            if (GetGoState() != GO_STATE_READY)
                                ResetDoorOrButton();
                            //flags in AB are type_button and we need to add them here so no break!
                        default:
                            if (!m_spawnedByDefault)        // despawn timer
                            {
                                                            // can be despawned or destroyed
                                SetLootState(GO_JUST_DEACTIVATED);
                                return;
                            }
                                                            // respawn timer
                            uint32 poolid = GetDBTableGUIDLow() ? sPoolMgr->IsPartOfAPool<GameObject>(GetDBTableGUIDLow()) : 0;
                            if (poolid)
                                sPoolMgr->UpdatePool<GameObject>(poolid, GetDBTableGUIDLow());
                            else
                                GetMap()->AddToMap(this);
                            break;
                    }
                }
            }

            if (isSpawned())
            {
                // traps can have time and can not have
                GameObjectTemplate const* goInfo = GetGOInfo();
                if (goInfo->type == GAMEOBJECT_TYPE_TRAP)
                {
                    if (m_cooldownTime >= time(NULL))
                        return;

                    // Type 2 - Bomb (will go away after casting it's spell)
                    if (goInfo->trap.charges == 2)
                    {
                        if (goInfo->trap.spell)
                            CastSpell(NULL, goInfo->trap.spell);  // @todo FIXME: null target won't work for target type 1
                        SetLootState(GO_JUST_DEACTIVATED);
                        break;
                    }
                    // Type 0 and 1 - trap (type 0 will not get removed after casting a spell)
                    Unit* owner = GetOwner();
                    Unit* ok = NULL;                            // pointer to appropriate target if found any

                    bool IsBattlegroundTrap = false;
                    //FIXME: this is activation radius (in different casting radius that must be selected from spell data)
                    //TODO: move activated state code (cast itself) to GO_ACTIVATED, in this place only check activating and set state
                    float radius = (float)(goInfo->trap.radius)/3*2; // TODO rename radius to diameter (goInfo->trap.radius) should be (goInfo->trap.diameter)
                    if (!radius)
                    {
                        if (goInfo->trap.cooldown != 3)            // cast in other case (at some triggering/linked go/etc explicit call)
                            return;
                        else
                        {
                            if (m_respawnTime > 0)
                                break;

                            radius = (float)goInfo->trap.cooldown;       // battlegrounds gameobjects has data2 == 0 && data5 == 3
                            IsBattlegroundTrap = true;

                            if (!radius)
                                return;
                        }
                    }

                    // Note: this hack with search required until GO casting not implemented
                    // search unfriendly creature
                    if (owner)                    // hunter trap
                    {
                        JadeCore::AnyUnfriendlyNoTotemUnitInObjectRangeCheck checker(this, owner, radius);
                        JadeCore::UnitSearcher<JadeCore::AnyUnfriendlyNoTotemUnitInObjectRangeCheck> searcher(this, ok, checker);
                        VisitNearbyGridObject(radius, searcher);
                        if (!ok) VisitNearbyWorldObject(radius, searcher);
                    }
                    else                                        // environmental trap
                    {
                        // environmental damage spells already have around enemies targeting but this not help in case not existed GO casting support
                        // affect only players
                        Player* player = NULL;
                        JadeCore::AnyPlayerInObjectRangeCheck checker(this, radius);
                        JadeCore::PlayerSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(this, player, checker);
                        VisitNearbyWorldObject(radius, searcher);
                        ok = player;
                    }

                    if (ok)
                    {
                        // some traps do not have spell but should be triggered
                        if (goInfo->trap.spell)
                            CastSpell(ok, goInfo->trap.spell);

                        m_cooldownTime = time(NULL) + (goInfo->trap.cooldown ? goInfo->trap.cooldown :  uint32(4));   // template or 4 seconds

                        if (goInfo->trap.charges == 1)
                            SetLootState(GO_JUST_DEACTIVATED);

                        if (IsBattlegroundTrap && ok->IsPlayer())
                        {
                            //Battleground gameobjects case
                            if (ok->ToPlayer()->InBattleground())
                                if (Battleground* bg = ok->ToPlayer()->GetBattleground())
                                    bg->HandleTriggerBuff(GetGUID());
                        }
                    }
                }
                else if (uint32 max_charges = goInfo->GetCharges())
                {
                    if (m_usetimes >= max_charges)
                    {
                        m_usetimes = 0;
                        SetLootState(GO_JUST_DEACTIVATED);      // can be despawned or destroyed
                    }
                }
            }

            break;
        }
        case GO_ACTIVATED:
        {
            switch (GetGoType())
            {
                case GAMEOBJECT_TYPE_DOOR:
                case GAMEOBJECT_TYPE_BUTTON:
                    if (GetGOInfo()->GetAutoCloseTime() && (m_cooldownTime < time(NULL)))
                        ResetDoorOrButton();
                    break;
                case GAMEOBJECT_TYPE_GOOBER:
                    if (m_cooldownTime < time(NULL))
                    {
                        RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);

                        SetLootState(GO_JUST_DEACTIVATED);
                        m_cooldownTime = 0;
                    }
                    break;
                case GAMEOBJECT_TYPE_CHEST:
                    if (m_groupLootTimer)
                    {
                        if (m_groupLootTimer <= diff)
                        {
                            Group* group = sGroupMgr->GetGroupByGUID(lootingGroupLowGUID);
                            if (group)
                                group->EndRoll(&loot);
                            m_groupLootTimer = 0;
                            lootingGroupLowGUID = 0;
                        }
                        else
                            m_groupLootTimer -= diff;
                    }
                default:
                    break;
            }
            break;
        }
        case GO_JUST_DEACTIVATED:
        {
            //if Gameobject should cast spell, then this, but some GOs (type = 10) should be destroyed
            if (GetGoType() == GAMEOBJECT_TYPE_GOOBER)
            {
                uint32 spellId = GetGOInfo()->goober.spell;

                if (spellId)
                {
                    for (std::set<uint64>::const_iterator it = m_unique_users.begin(); it != m_unique_users.end(); ++it)
                        // m_unique_users can contain only player GUIDs
                        if (Player* owner = ObjectAccessor::GetPlayer(*this, *it))
                            owner->CastSpell(owner, spellId, false);

                    m_unique_users.clear();
                    m_usetimes = 0;
                }

                SetGoState(GO_STATE_READY);

                //any return here in case battleground traps
                if (GetGOInfo()->flags & GO_FLAG_NODESPAWN)
                    return;
            }

            loot.clear();

            //! If this is summoned by a spell with ie. SPELL_EFFECT_SUMMON_OBJECT_WILD, with or without owner, we check respawn criteria based on spell
            //! The GetOwnerGUID() check is mostly for compatibility with hacky scripts - 99% of the time summoning should be done trough spells.
            if (GetSpellId() || GetOwnerGUID())
            {
                SetRespawnTime(0);
                Delete();
                return;
            }

            SetLootState(GO_READY);

            //burning flags in some battlegrounds, if you find better condition, just add it
            if (GetGOInfo()->IsDespawnAtAction() || GetGoAnimProgress() > 0)
            {
                SendObjectDeSpawnAnim(GetGUID());
                //reset flags
                SetUInt32Value(GAMEOBJECT_FIELD_FLAGS, GetGOInfo()->flags);
            }

            if (!m_respawnDelayTime)
                return;

            if (!m_spawnedByDefault)
            {
                m_respawnTime = 0;
                UpdateObjectVisibility();
                return;
            }

            m_respawnTime = time(NULL) + m_respawnDelayTime;

            // if option not set then object will be saved at grid unload
            if (sWorld->getBoolConfig(CONFIG_SAVE_RESPAWN_TIME_IMMEDIATELY))
                SaveRespawnTime();

            UpdateObjectVisibility();

            break;
        }
    }
    sScriptMgr->OnGameObjectUpdate(this, diff);
}

void GameObject::Refresh()
{
    // not refresh despawned not casted GO (despawned casted GO destroyed in all cases anyway)
    if (m_respawnTime > 0 && m_spawnedByDefault)
        return;

    if (isSpawned())
        GetMap()->AddToMap(this);
}

void GameObject::AddUniqueUse(Player* player)
{
    AddUse();
    m_unique_users.insert(player->GetGUID());
}

void GameObject::Delete()
{
    SetLootState(GO_NOT_READY);
    RemoveFromOwner();

    SendObjectDeSpawnAnim(GetGUID());

    SetGoState(GO_STATE_READY);
    SetUInt32Value(GAMEOBJECT_FIELD_FLAGS, GetGOInfo()->flags);

    uint32 poolid = GetDBTableGUIDLow() ? sPoolMgr->IsPartOfAPool<GameObject>(GetDBTableGUIDLow()) : 0;
    if (poolid)
        sPoolMgr->UpdatePool<GameObject>(poolid, GetDBTableGUIDLow());
    else
        AddObjectToRemoveList();
}

void GameObject::getFishLoot(Loot* fishloot, Player* loot_owner)
{
    fishloot->clear();

    uint32 zone, subzone;
    GetZoneAndAreaId(zone, subzone);

    // if subzone loot exist use it
    if (!fishloot->FillLoot(subzone, LootTemplates_Fishing, loot_owner, true, true))
        // else use zone loot (must exist in like case)
        fishloot->FillLoot(zone, LootTemplates_Fishing, loot_owner, true);
}

void GameObject::SaveToDB()
{
    // this should only be used when the gameobject has already been loaded
    // preferably after adding to map, because mapid may not be valid otherwise
    GameObjectData const* data = sObjectMgr->GetGOData(m_DBTableGuid);
    if (!data)
    {
        sLog->outError(LOG_FILTER_GENERAL, "GameObject::SaveToDB failed, cannot get gameobject data!");
        return;
    }

    SaveToDB(GetMapId(), data->spawnMask, data->phaseMask);
}

void GameObject::SaveToDB(uint32 mapid, uint32 spawnMask, uint32 phaseMask)
{
    const GameObjectTemplate* goI = GetGOInfo();

    if (!goI)
        return;

    if (!m_DBTableGuid)
        m_DBTableGuid = GetGUIDLow();
    // update in loaded data (changing data only in this place)
    GameObjectData& data = sObjectMgr->NewGOData(m_DBTableGuid);

    uint32 zoneId = 0;
    uint32 areaId = 0;
    sMapMgr->GetZoneAndAreaId(zoneId, areaId, mapid, GetPositionX(), GetPositionY(), GetPositionZ());

    // data->guid = guid must not be updated at save
    data.id = GetEntry();
    data.mapid = mapid;
    data.zoneId = zoneId;
    data.areaId = areaId;
    data.phaseMask = phaseMask;
    data.posX = GetPositionX();
    data.posY = GetPositionY();
    data.posZ = GetPositionZ();
    data.orientation = GetOrientation();
    data.rotation0 = GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+0);
    data.rotation1 = GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+1);
    data.rotation2 = GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+2);
    data.rotation3 = GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+3);
    data.spawntimesecs = m_spawnedByDefault ? m_respawnDelayTime : -(int32)m_respawnDelayTime;
    data.animprogress = GetGoAnimProgress();
    data.go_state = GetGoState();
    data.spawnMask = spawnMask;
    data.artKit = GetGoArtKit();
    data.isActive = isActiveObject();
    data.CustomFlags = GetCustomFlags();

    // Update in DB
    SQLTransaction trans = WorldDatabase.BeginTransaction();

    uint8 index = 0;

    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_GAMEOBJECT);
    stmt->setUInt32(0, m_DBTableGuid);
    trans->Append(stmt);

    stmt = WorldDatabase.GetPreparedStatement(WORLD_INS_GAMEOBJECT);
    stmt->setUInt32(index++, m_DBTableGuid);
    stmt->setUInt32(index++, GetEntry());
    stmt->setUInt16(index++, uint16(mapid));
    stmt->setUInt16(index++, zoneId);
    stmt->setUInt16(index++, areaId);
    stmt->setUInt32(index++, spawnMask);
    stmt->setUInt32(index++, uint32(GetPhaseMask()));
    stmt->setFloat(index++, GetPositionX());
    stmt->setFloat(index++, GetPositionY());
    stmt->setFloat(index++, GetPositionZ());
    stmt->setFloat(index++, GetOrientation());
    stmt->setFloat(index++, GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION));
    stmt->setFloat(index++, GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+1));
    stmt->setFloat(index++, GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+2));
    stmt->setFloat(index++, GetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+3));
    stmt->setInt32(index++, int32(m_respawnDelayTime));
    stmt->setUInt8(index++, GetGoAnimProgress());
    stmt->setUInt8(index++, uint8(GetGoState()));
    stmt->setUInt8(index++, uint8(isActiveObject()));
    trans->Append(stmt);

    WorldDatabase.CommitTransaction(trans);
}

bool GameObject::LoadGameObjectFromDB(uint32 guid, Map* map, bool addToMap)
{
    GameObjectData const* data = sObjectMgr->GetGOData(guid);

    if (!data)
    {
        sLog->outError(LOG_FILTER_SQL, "Gameobject (GUID: %u) not found in table `gameobject`, can't load. ", guid);
        return false;
    }

    uint32 entry = data->id;
    //uint32 map_id = data->mapid;                          // already used before call
    uint32 phaseMask = data->phaseMask;
    float x = data->posX;
    float y = data->posY;
    float z = data->posZ;
    float ang = data->orientation;

    float rotation0 = data->rotation0;
    float rotation1 = data->rotation1;
    float rotation2 = data->rotation2;
    float rotation3 = data->rotation3;

    uint32 animprogress = data->animprogress;
    GOState go_state = data->go_state;
    uint32 artKit = data->artKit;

    m_DBTableGuid = guid;
    m_CustomFlags = data->CustomFlags;

    if (map->GetInstanceId() != 0)
        guid = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);

    if (!Create(guid, entry, map, phaseMask, x, y, z, ang, rotation0, rotation1, rotation2, rotation3, animprogress, go_state, artKit))
        return false;

    if (data->spawntimesecs >= 0)
    {
        m_spawnedByDefault = true;

        if (!GetGOInfo()->GetDespawnPossibility() && !GetGOInfo()->IsDespawnAtAction())
        {
            SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NODESPAWN);
            m_respawnDelayTime = 0;
            m_respawnTime = 0;
        }
        else
        {
            m_respawnDelayTime = data->spawntimesecs;
            m_respawnTime = GetMap()->GetGORespawnTime(m_DBTableGuid);

            // ready to respawn
            if (m_respawnTime && m_respawnTime <= time(NULL))
            {
                m_respawnTime = 0;
                GetMap()->RemoveGORespawnTime(m_DBTableGuid);
            }
        }
    }
    else
    {
        m_spawnedByDefault = false;
        m_respawnDelayTime = -data->spawntimesecs;
        m_respawnTime = 0;
    }

    m_goData = data;

    setActive(data->isActive);

    if (addToMap && !GetMap()->AddToMap(this))
        return false;

    return true;
}

void GameObject::DeleteFromDB()
{
    GetMap()->RemoveGORespawnTime(m_DBTableGuid);
    sObjectMgr->DeleteGOData(m_DBTableGuid);

    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_GAMEOBJECT);

    stmt->setUInt32(0, m_DBTableGuid);

    WorldDatabase.Execute(stmt);

    stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_EVENT_GAMEOBJECT);

    stmt->setUInt32(0, m_DBTableGuid);

    WorldDatabase.Execute(stmt);
}

GameObject* GameObject::GetGameObject(WorldObject& object, uint64 guid)
{
    return object.GetMap()->GetGameObject(guid);
}

/*********************************************************/
/***                    QUEST SYSTEM                   ***/
/*********************************************************/
bool GameObject::hasQuest(uint32 quest_id) const
{
    QuestRelationBounds qr = sObjectMgr->GetGOQuestRelationBounds(GetEntry());
    for (QuestRelations::const_iterator itr = qr.first; itr != qr.second; ++itr)
    {
        if (itr->second == quest_id)
            return true;
    }
    return false;
}

bool GameObject::hasInvolvedQuest(uint32 quest_id) const
{
    QuestRelationBounds qir = sObjectMgr->GetGOQuestInvolvedRelationBounds(GetEntry());
    for (QuestRelations::const_iterator itr = qir.first; itr != qir.second; ++itr)
    {
        if (itr->second == quest_id)
            return true;
    }
    return false;
}

bool GameObject::IsTransport() const
{
    // If something is marked as a transport, don't transmit an out of range packet for it.
    GameObjectTemplate const* gInfo = GetGOInfo();
    if (!gInfo)
        return false;

    return gInfo->type == GAMEOBJECT_TYPE_TRANSPORT || gInfo->type ==GAMEOBJECT_TYPE_MAP_OBJ_TRANSPORT;
}

// is Dynamic transport = non-stop Transport
bool GameObject::IsDynTransport() const
{
    // If something is marked as a transport, don't transmit an out of range packet for it.
    GameObjectTemplate const* gInfo = GetGOInfo();
    if (!gInfo)
        return false;

    return gInfo->type == GAMEOBJECT_TYPE_MAP_OBJ_TRANSPORT || (gInfo->type == GAMEOBJECT_TYPE_TRANSPORT && m_goValue->Transport.StopFrames->empty());
}

bool GameObject::IsDestructibleBuilding() const
{
    GameObjectTemplate const* gInfo = GetGOInfo();
    if (!gInfo) return false;
    return gInfo->type == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING;
}

Unit* GameObject::GetOwner() const
{
    return ObjectAccessor::GetUnit(*this, GetOwnerGUID());
}

void GameObject::SaveRespawnTime()
{
    if (m_goData && m_goData->dbData && m_respawnTime > time(NULL) && m_spawnedByDefault)
        GetMap()->SaveGORespawnTime(m_DBTableGuid, m_respawnTime);
}

bool GameObject::IsAlwaysVisibleFor(WorldObject const* seer) const
{
    if (WorldObject::IsAlwaysVisibleFor(seer))
        return true;

    if (IsTransport() || IsDestructibleBuilding())
        return true;

    if (!seer)
        return false;

    // Always seen by owner and friendly units
    if (uint64 guid = GetOwnerGUID())
    {
        if (seer->GetGUID() == guid)
            return true;

        Unit* owner = GetOwner();
        if (owner && seer->isType(TYPEMASK_UNIT) && owner->IsFriendlyTo(((Unit*)seer)))
            return true;
    }

    return false;
}

bool GameObject::IsInvisibleDueToDespawn() const
{
    if (WorldObject::IsInvisibleDueToDespawn())
        return true;

    // Despawned
    if (!isSpawned())
        return true;

    return false;
}

void GameObject::Respawn()
{
    if (m_spawnedByDefault && m_respawnTime > 0)
    {
        m_respawnTime = time(NULL);
        GetMap()->RemoveGORespawnTime(m_DBTableGuid);
    }
}

bool GameObject::ActivateToQuest(Player* target) const
{
    if (target->HasQuestForGO(GetEntry()))
        return true;

    if (!sObjectMgr->IsGameObjectForQuests(GetEntry()))
        return false;

    switch (GetGoType())
    {
        // scan GO chest with loot including quest items
        case GAMEOBJECT_TYPE_CHEST:
        {
            if (LootTemplates_Gameobject.HaveQuestLootForPlayer(GetGOInfo()->GetLootId(), target))
            {
                //TODO: fix this hack
                //look for battlegroundAV for some objects which are only activated after mine gots captured by own team
                if (GetEntry() == BG_AV_OBJECTID_MINE_N || GetEntry() == BG_AV_OBJECTID_MINE_S)
                    if (Battleground* bg = target->GetBattleground())
                        if (bg->GetTypeID(true) == BATTLEGROUND_AV && !(((BattlegroundAV*)bg)->PlayerCanDoMineQuest(GetEntry(), target->GetTeam())))
                            return false;
                return true;
            }
            break;
        }
        case GAMEOBJECT_TYPE_GENERIC:
        {
            if (GetGOInfo()->_generic.questID == -1 || target->GetQuestStatus(GetGOInfo()->_generic.questID) == QUEST_STATUS_INCOMPLETE) ///< Comparison of integers of different signs: 'uint32' (aka 'unsigned int') and 'int'

                return true;
            break;
        }
        case GAMEOBJECT_TYPE_GOOBER:
        {
            if (GetGOInfo()->goober.questID == -1 || target->GetQuestStatus(GetGOInfo()->goober.questID) == QUEST_STATUS_INCOMPLETE) ///< Comparison of integers of different signs: 'uint32' (aka 'unsigned int') and 'int'

                return true;
            break;
        }
        default:
            break;
    }

    return false;
}

void GameObject::SetCancelAnim(bool p_Disable)
{
    if (p_Disable)
        m_AllowAnim = false;
    else
        m_AllowAnim = true;
}

void GameObject::TriggeringLinkedGameObject(uint32 trapEntry, Unit* target)
{
    GameObjectTemplate const* trapInfo = sObjectMgr->GetGameObjectTemplate(trapEntry);
    if (!trapInfo || trapInfo->type != GAMEOBJECT_TYPE_TRAP)
        return;

    SpellInfo const* trapSpell = sSpellMgr->GetSpellInfo(trapInfo->trap.spell);
    if (!trapSpell)                                          // checked at load already
        return;

    float range = float(target->GetSpellMaxRangeForTarget(GetOwner(), trapSpell));

    // search nearest linked GO
    GameObject* trapGO = NULL;
    {
        // using original GO distance
        CellCoord p(JadeCore::ComputeCellCoord(GetPositionX(), GetPositionY()));
        Cell cell(p);

        JadeCore::NearestGameObjectEntryInObjectRangeCheck go_check(*target, trapEntry, range);
        JadeCore::GameObjectLastSearcher<JadeCore::NearestGameObjectEntryInObjectRangeCheck> checker(this, trapGO, go_check);

        TypeContainerVisitor<JadeCore::GameObjectLastSearcher<JadeCore::NearestGameObjectEntryInObjectRangeCheck>, GridTypeMapContainer > object_checker(checker);
        cell.Visit(p, object_checker, *GetMap(), *target, range);
    }

    // found correct GO
    if (trapGO)
        trapGO->CastSpell(target, trapInfo->trap.spell);
}

GameObject* GameObject::LookupFishingHoleAround(float range)
{
    GameObject* ok = NULL;

    CellCoord p(JadeCore::ComputeCellCoord(GetPositionX(), GetPositionY()));
    Cell cell(p);
    JadeCore::NearestGameObjectFishingHole u_check(*this, range);
    JadeCore::GameObjectSearcher<JadeCore::NearestGameObjectFishingHole> checker(this, ok, u_check);

    TypeContainerVisitor<JadeCore::GameObjectSearcher<JadeCore::NearestGameObjectFishingHole>, GridTypeMapContainer > grid_object_checker(checker);
    cell.Visit(p, grid_object_checker, *GetMap(), *this, range);

    return ok;
}

void GameObject::ResetDoorOrButton()
{
    if (m_lootState == GO_READY || m_lootState == GO_JUST_DEACTIVATED)
        return;

    SwitchDoorOrButton(false);
    SetLootState(GO_JUST_DEACTIVATED);
    m_cooldownTime = 0;
}

void GameObject::UseDoorOrButton(uint32 time_to_restore, bool alternative /* = false */, Unit* user /*=NULL*/)
{
    if (m_lootState != GO_READY)
        return;

    if (!time_to_restore)
        time_to_restore = GetGOInfo()->GetAutoCloseTime();

    SwitchDoorOrButton(true, alternative);
    SetLootState(GO_ACTIVATED, user);

    m_cooldownTime = time(NULL) + time_to_restore;
}

void GameObject::SetGoArtKit(uint8 kit)
{
    SetByteValue(GAMEOBJECT_FIELD_STATE_SPELL_VISUAL_ID, 1, kit);
    GameObjectData* data = const_cast<GameObjectData*>(sObjectMgr->GetGOData(m_DBTableGuid));
    if (data)
        data->artKit = kit;
}

void GameObject::SetGoArtKit(uint8 artkit, GameObject* go, uint32 lowguid)
{
    const GameObjectData* data = NULL;
    if (go)
    {
        go->SetGoArtKit(artkit);
        data = go->GetGOData();
    }
    else if (lowguid)
        data = sObjectMgr->GetGOData(lowguid);

    if (data)
        const_cast<GameObjectData*>(data)->artKit = artkit;
}

void GameObject::SwitchDoorOrButton(bool activate, bool alternative /* = false */)
{
    if (activate)
        SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);
    else
        RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);

    if (GetGoState() == GO_STATE_READY)                      //if closed -> open
        SetGoState(alternative ? GO_STATE_ACTIVE_ALTERNATIVE : GO_STATE_ACTIVE);
    else                                                    //if open -> close
        SetGoState(GO_STATE_READY);
}

void GameObject::Use(Unit* p_User)
{
    // by default spell caster is user
    Unit* spellCaster = p_User;
    uint32 spellId = 0;
    bool triggered = false;

    if (Player* playerUser = p_User->ToPlayer())
    {
        if (playerUser->GetEmoteState())
            playerUser->SetEmoteState(0);

        if (sScriptMgr->OnGossipHello(playerUser, this))
            return;

        if (AI() && AI()->GossipHello(playerUser))
            return;
    }

    // If cooldown data present in template
    if (uint32 cooldown = GetGOInfo()->GetCooldown())
    {
        if (m_cooldownTime > sWorld->GetGameTime())
            return;

        m_cooldownTime = sWorld->GetGameTime() + cooldown;
    }

    if (GetEntry() == 192819)
    {
        if (Creature* c = p_User->FindNearestCreature(23472, 50.00f))
            if (Player* plr = p_User->ToPlayer())
                plr->TeleportTo(p_User->GetMapId(), c->GetPositionX(), c->GetPositionY(), c->GetPositionZ(), 3.14f);
        return;
    }

    if (GetGoType() != GAMEOBJECT_TYPE_FISHINGNODE && GetGoType() != GAMEOBJECT_TYPE_FISHINGHOLE)
        p_User->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_USE);

    switch (GetGoType())
    {
        case GAMEOBJECT_TYPE_DOOR:                          //0
        case GAMEOBJECT_TYPE_BUTTON:                        //1
            //doors/buttons never really despawn, only reset to default state/flags
            UseDoorOrButton(0, false, p_User);

            if (Player* player = p_User->ToPlayer())
                if (Battleground* bg = player->GetBattleground())
                    bg->EventPlayerUsedGO(player, this);

            // activate script
            GetMap()->ScriptsStart(sGameObjectScripts, GetDBTableGUIDLow(), spellCaster, this);
            return;

        case GAMEOBJECT_TYPE_QUESTGIVER:                    //2
        {
            if (p_User->GetTypeId() != TYPEID_PLAYER)
                return;

            if (Player* player = p_User->ToPlayer())
            {
                player->PrepareGossipMenu(this, GetGOInfo()->questgiver.gossipID, true);
                player->SendPreparedGossip(this);
            }
            return;
        }
        case GAMEOBJECT_TYPE_TRAP:                          //6
        {
            if (GameObjectTemplate const* goInfo = GetGOInfo())
            {
                if (goInfo->trap.spell)
                    CastSpell(p_User, goInfo->trap.spell);

                m_cooldownTime = time(NULL) + (goInfo->trap.cooldown ? goInfo->trap.cooldown :  uint32(4));   // template or 4 seconds

                if (goInfo->trap.charges == 1)         // Desactivate after trigger
                    SetLootState(GO_JUST_DEACTIVATED);
            }

            return;
        }
        //Sitting: Wooden bench, chairs enzz
        case GAMEOBJECT_TYPE_CHAIR:                         //7
        {
            GameObjectTemplate const* info = GetGOInfo();
            if (!info)
                return;

            if (p_User->GetTypeId() != TYPEID_PLAYER)
                return;

            if (ChairListSlots.empty())        // this is called once at first chair use to make list of available slots
            {
                if (info->chair.chairslots > 0)     // sometimes chairs in DB have error in fields and we dont know number of slots
                    for (uint32 i = 0; i < info->chair.chairslots; ++i)
                        ChairListSlots[i] = 0; // Last user of current slot set to 0 (none sit here yet)
                else
                    ChairListSlots[0] = 0;     // error in DB, make one default slot
            }

            Player* player = p_User->ToPlayer();
            if (!player)
                break;

            // a chair may have n slots. we have to calculate their positions and teleport the player to the nearest one

            float lowestDist = DEFAULT_VISIBILITY_DISTANCE;

            uint32 nearest_slot = 0;
            float x_lowest = GetPositionX();
            float y_lowest = GetPositionY();

            // the object orientation + 1/2 pi
            // every slot will be on that straight line
            float orthogonalOrientation = GetOrientation()+M_PI*0.5f;
            // find nearest slot
            bool found_free_slot = false;
            for (ChairSlotAndUser::iterator itr = ChairListSlots.begin(); itr != ChairListSlots.end(); ++itr)
            {
                // the distance between this slot and the center of the go - imagine a 1D space
                float relativeDistance = (info->size*itr->first) - (info->size*(info->chair.chairslots - 1) / 2.0f);

                float x_i = GetPositionX() + relativeDistance * std::cos(orthogonalOrientation);
                float y_i = GetPositionY() + relativeDistance * std::sin(orthogonalOrientation);

                if (itr->second)
                {
                    if (Player* ChairUser = ObjectAccessor::FindPlayer(itr->second))
                        if (ChairUser->IsSitState() && ChairUser->getStandState() != UNIT_STAND_STATE_SIT && ChairUser->GetExactDist2d(x_i, y_i) < 0.1f)
                            continue;        // This seat is already occupied by ChairUser. NOTE: Not sure if the ChairUser->getStandState() != UNIT_STAND_STATE_SIT check is required.
                        else
                            itr->second = 0; // This seat is unoccupied.
                    else
                        itr->second = 0;     // The seat may of had an occupant, but they're offline.
                }

                found_free_slot = true;

                // calculate the distance between the player and this slot
                float thisDistance = player->GetDistance2d(x_i, y_i);

                /* debug code. It will spawn a npc on each slot to visualize them.
                Creature* helper = player->SummonCreature(14496, x_i, y_i, GetPositionZ(), GetOrientation(), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10000);
                std::ostringstream output;
                output << i << ": thisDist: " << thisDistance;
                helper->MonsterSay(output.str().c_str(), LANG_UNIVERSAL, 0);
                */

                if (thisDistance <= lowestDist)
                {
                    nearest_slot = itr->first;
                    lowestDist = thisDistance;
                    x_lowest = x_i;
                    y_lowest = y_i;
                }
            }

            if (found_free_slot)
            {
                ChairSlotAndUser::iterator itr = ChairListSlots.find(nearest_slot);
                if (itr != ChairListSlots.end())
                {
                    itr->second = player->GetGUID(); //this slot in now used by player
                    player->TeleportTo(GetMapId(), x_lowest, y_lowest, GetPositionZ(), GetOrientation(), TELE_TO_NOT_LEAVE_TRANSPORT | TELE_TO_NOT_LEAVE_COMBAT | TELE_TO_NOT_UNSUMMON_PET);
                    player->SetStandState(UNIT_STAND_STATE_SIT_LOW_CHAIR+info->chair.chairheight);
                    return;
                }
            }
            //else
                //player->GetSession()->SendNotification("There's nowhere left for you to sit.");

            return;
        }
        //big gun, its a spell/aura
        case GAMEOBJECT_TYPE_GOOBER:                        //10
        {
            const GameObjectTemplate* l_Info = GetGOInfo();
            if (!l_Info)
                break;

            if (p_User->IsPlayer())
            {
                Player* l_Player = p_User->ToPlayer();
                if (!l_Player)
                    break;

                if (l_Info->goober.pageID)                    // show page...
                {
                    WorldPacket l_Data(SMSG_PAGE_TEXT, 2 + 16);
                    l_Data.appendPackGUID(GetGUID());               ///< Game Object GUID

                    l_Player->GetSession()->SendPacket(&l_Data);
                }
                else if (l_Info->goober.gossipID)
                {
                    l_Player->PrepareGossipMenu(this, l_Info->goober.gossipID);
                    l_Player->SendPreparedGossip(this);
                }

                if (l_Info->goober.eventID)
                {
                    sLog->outDebug(LOG_FILTER_MAPSCRIPTS, "Goober ScriptStart id %u for GO entry %u (GUID %u).", l_Info->goober.eventID, GetEntry(), GetDBTableGUIDLow());
                    GetMap()->ScriptsStart(sEventScripts, l_Info->goober.eventID, l_Player, this);
                    EventInform(l_Info->goober.eventID);
                }

                // possible quest objective for active quests
                if (l_Info->goober.questID && sObjectMgr->GetQuestTemplate(l_Info->goober.questID))
                {
                    //Quest require to be active for GO using
                    if (l_Player->GetQuestStatus(l_Info->goober.questID) != QUEST_STATUS_INCOMPLETE)
                        break;
                }

                if (Battleground* bg = l_Player->GetBattleground())
                    bg->EventPlayerUsedGO(l_Player, this);

                l_Player->QuestObjectiveSatisfy(GetEntry(), 1, QUEST_OBJECTIVE_TYPE_GO, GetGUID());

                GetMap()->ScriptsStart(sGameObjectScripts, GetDBTableGUIDLow(), l_Player, this);
            }

            if (uint32 trapEntry = l_Info->goober.linkedTrap)
                TriggeringLinkedGameObject(trapEntry, p_User);

            SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);
            SetLootState(GO_ACTIVATED, p_User);

            if (m_AllowAnim)
            {
                // this appear to be ok, however others exist in addition to this that should have custom (ex: 190510, 188692, 187389)
                if (l_Info->goober.customAnim)
                    SendCustomAnim(GetGoAnimProgress());
                else
                    SetGoState(GO_STATE_ACTIVE);
            }

            m_cooldownTime = time(NULL) + l_Info->GetAutoCloseTime();

            // cast this spell later if provided
            spellId = l_Info->goober.spell;
            spellCaster = NULL;
            
            break;
        }
        case GAMEOBJECT_TYPE_CAMERA:                        //13
        {
            GameObjectTemplate const* info = GetGOInfo();
            if (!info)
                return;

            if (p_User->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = p_User->ToPlayer();
            if (!player)
                break;

            if (info->camera.camera)
                player->SendCinematicStart(info->camera.camera);

            if (info->camera.eventID)
                GetMap()->ScriptsStart(sEventScripts, info->camera.eventID, player, this);

            return;
        }
        //fishing bobber
        case GAMEOBJECT_TYPE_FISHINGNODE:                   //17
        {
            Player* player = p_User->ToPlayer();
            if (!player)
                return;

            uint64 l_PlayerGUID = player->GetGUID();
            uint64 l_OwnerGUID = GetOwnerGUID();

            if (l_PlayerGUID != l_OwnerGUID)
                return;

            switch (getLootState())
            {
                case GO_READY:                              // ready for loot
                {
                    uint32 zone, subzone;
                    GetZoneAndAreaId(zone, subzone);

                    int32 zone_skill = sObjectMgr->GetFishingBaseSkillLevel(subzone);
                    if (!zone_skill)
                        zone_skill = sObjectMgr->GetFishingBaseSkillLevel(zone);

                    //provide error, no fishable zone or area should be 0
                    if (!zone_skill)
                        sLog->outError(LOG_FILTER_SQL, "Fishable areaId %u are not properly defined in `skill_fishing_base_level`.", subzone);

                    int32 skill = player->GetSkillValue(SKILL_FISHING);

                    int32 chance;
                    if (skill < zone_skill)
                    {
                        chance = int32(pow((double)skill/zone_skill, 2) * 100);
                        if (chance < 1)
                            chance = 1;
                    }
                    else
                        chance = 100;

                    int32 roll = irand(1, 100);

                    sLog->outDebug(LOG_FILTER_GENERAL, "Fishing check (skill: %i zone min skill: %i chance %i roll: %i", skill, zone_skill, chance, roll);

                    // but you will likely cause junk in areas that require a high fishing skill (not yet implemented)
                    if (chance >= roll)
                    {
                        player->UpdateFishingSkill();

                        //TODO: I do not understand this hack. Need some explanation.
                        // prevent removing GO at spell cancel
                        RemoveFromOwner();
                        SetOwnerGUID(player->GetGUID());

                        //TODO: find reasonable value for fishing hole search
                        GameObject* ok = LookupFishingHoleAround(20.0f + CONTACT_DISTANCE);
                        if (ok)
                        {
                            ok->Use(player);
                            SetLootState(GO_JUST_DEACTIVATED);
                        }
                        else
                            player->SendLoot(GetGUID(), LOOT_FISHING);
                    }
                    // TODO: else: junk

                    break;
                }
                case GO_JUST_DEACTIVATED:                   // nothing to do, will be deleted at next update
                    break;
                default:
                {
                    SetLootState(GO_JUST_DEACTIVATED);

                    WorldPacket data(SMSG_FISH_NOT_HOOKED, 0);
                    player->GetSession()->SendPacket(&data);
                    break;
                }
            }

            player->FinishSpell(CURRENT_CHANNELED_SPELL);
            return;
        }

        case GAMEOBJECT_TYPE_RITUAL:              //18
        {
            if (p_User->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = p_User->ToPlayer();
            if (!player)
                break;

            Unit* owner = GetOwner();

            GameObjectTemplate const* info = GetGOInfo();

            // ritual owner is set for GO's without owner (not summoned)
            if (!m_ritualOwner && !owner)
                m_ritualOwner = player;

            if (owner)
            {
                if (owner->GetTypeId() != TYPEID_PLAYER)
                    return;

                // accept only use by player from same group as owner, excluding owner itself (unique use already added in spell effect)
                if (player == owner->ToPlayer() || (info->summoningRitual.castersGrouped && !player->IsInSameRaidWith(owner->ToPlayer())))
                    return;

                // expect owner to already be channeling, so if not...
                if (!owner->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                    return;

                // in case summoning ritual caster is GO creator
                spellCaster = owner;
            }
            else
            {
                if (player != m_ritualOwner && (info->summoningRitual.castersGrouped && !player->IsInSameRaidWith(m_ritualOwner)))
                    return;

                spellCaster = player;
            }

            AddUniqueUse(player);

            if (info->summoningRitual.animSpell)
            {
                player->CastSpell(player, info->summoningRitual.animSpell, true);

                // for this case, summoningRitual.spellId is always triggered
                triggered = true;
            }

            // full amount unique participants including original summoner
            if (GetUniqueUseCount() == info->summoningRitual.casters)
            {
                spellCaster = m_ritualOwner ? m_ritualOwner : spellCaster;
                if (m_ritualOwner)
                    spellCaster = m_ritualOwner;

                spellId = info->summoningRitual.spell;

                if (spellId == 62330)                       // GO store nonexistent spell, replace by expected
                {
                    // spell have reagent and mana cost but it not expected use its
                    // it triggered spell in fact casted at currently channeled GO
                    spellId = 61993;
                    triggered = true;
                }

                // Cast casterTargetSpell at a random GO user
                // on the current DB there is only one gameobject that uses this (Ritual of Doom)
                // and its required target number is 1 (outter for loop will run once)
                if (info->summoningRitual.casterTargetSpell && info->summoningRitual.casterTargetSpell != 1) // No idea why this field is a bool in some cases
                    for (uint32 i = 0; i < info->summoningRitual.casterTargetSpellTargets; i++)
                        // m_unique_users can contain only player GUIDs
                        if (Player* target = ObjectAccessor::GetPlayer(*this, JadeCore::Containers::SelectRandomContainerElement(m_unique_users)))
                            spellCaster->CastSpell(target, info->summoningRitual.casterTargetSpell, true);

                // finish owners spell
                if (owner)
                    owner->FinishSpell(CURRENT_CHANNELED_SPELL);

                // can be deleted now, if
                if (!info->summoningRitual.ritualPersistent)
                    SetLootState(GO_JUST_DEACTIVATED);
                else
                {
                    // reset ritual for this GO
                    m_ritualOwner = NULL;
                    m_unique_users.clear();
                    m_usetimes = 0;
                }
            }
            else
                return;

            // go to end function to spell casting
            break;
        }
        case GAMEOBJECT_TYPE_SPELLCASTER:                   //22
        {
            GameObjectTemplate const* info = GetGOInfo();
            if (!info)
                return;

            if (info->spellcaster.partyOnly)
            {
                Unit* caster = GetOwner();
                if (!caster || caster->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (p_User->GetTypeId() != TYPEID_PLAYER || !p_User->ToPlayer()->IsInSameRaidWith(caster->ToPlayer()))
                    return;
            }

            if (!sScriptMgr->OnGameObjectSpellCasterUse(this, p_User->ToPlayer()))
                return;

            p_User->RemoveAurasByType(SPELL_AURA_MOUNTED);
            spellId = info->spellcaster.spell;

            AddUse();
            break;
        }
        case GAMEOBJECT_TYPE_MEETINGSTONE:                  //23
        {
            GameObjectTemplate const* info = GetGOInfo();

            if (p_User->GetTypeId() != TYPEID_PLAYER || !info)
                return;

            Player* player = p_User->ToPlayer();
            if (!player)
                break;

            Player* targetPlayer = ObjectAccessor::FindPlayer(player->GetSelection());

            // accept only use by player from same raid as caster, except caster itself
            if (!targetPlayer || targetPlayer == player || !targetPlayer->IsInSameRaidWith(player))
                return;

            //required lvl checks!
            uint8 level = player->getLevel();
            if (level < info->meetingstone.minLevel)
                return;
            level = targetPlayer->getLevel();
            if (level < info->meetingstone.minLevel)
                return;

            if (info->entry == 194097)
                spellId = 61994;                            // Ritual of Summoning
            else
                spellId = 59782;                            // Summoning Stone Effect

            break;
        }

        case GAMEOBJECT_TYPE_CAPTURE_POINT:                 // 42
        case GAMEOBJECT_TYPE_FLAGSTAND:                     // 24
        {
            if (p_User->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = p_User->ToPlayer();
            if (!player)
                break;

            if (player->CanUseBattlegroundObject())
            {
                // in battleground check
                Battleground* bg = player->GetBattleground();
                if (!bg)
                    return;
                if (player->GetVehicle())
                    return;

                player->RemoveAurasByType(SPELL_AURA_MOD_STEALTH);
                player->RemoveAurasByType(SPELL_AURA_MOD_INVISIBILITY);
                player->RemoveAurasByType(SPELL_AURA_MOD_CAMOUFLAGE);
                // BG flag click
                // AB:
                // 15001
                // 15002
                // 15003
                // 15004
                // 15005
                bg->EventPlayerClickedOnFlag(player, this);
                return;                                     //we don;t need to delete flag ... it is despawned!
            }
            break;
        }

        case GAMEOBJECT_TYPE_FISHINGHOLE:                   // 25
        {
            if (p_User->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = p_User->ToPlayer();
            if (!player)
                break;

            loot.clear();
            loot.FillLoot(GetGOInfo()->GetLootId(), LootTemplates_Gameobject, player, true);

            player->SendLoot(GetGUID(), LOOT_FISHINGHOLE);
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT, GetGOInfo()->entry);
            return;
        }

        case GAMEOBJECT_TYPE_FLAGDROP:                      // 26
        {
            if (p_User->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = p_User->ToPlayer();

            if (player->CanUseBattlegroundObject())
            {
                // in battleground check
                Battleground* bg = player->GetBattleground();
                if (!bg)
                    return;
                if (player->GetVehicle())
                    return;
                // BG flag dropped
                // WS:
                // 179785 - Silverwing Flag
                // 179786 - Warsong Flag
                // EotS:
                // 184142 - Netherstorm Flag
                GameObjectTemplate const* info = GetGOInfo();
                if (info)
                {
                    switch (info->entry)
                    {
                        case 179785:                        // Silverwing Flag
                        case 179786:                        // Warsong Flag
                            if (bg->GetTypeID(true) == BATTLEGROUND_WS || bg->GetTypeID(true) == BATTLEGROUND_TP)
                                bg->EventPlayerClickedOnFlag(player, this);
                            break;
                        case 184142:                        // Netherstorm Flag
                            if (bg->GetTypeID(true) == BATTLEGROUND_EY || bg->GetTypeID(true) == BATTLEGROUND_EYR)
                                bg->EventPlayerClickedOnFlag(player, this);
                            break;
                    }
                }
                //this cause to call return, all flags must be deleted here!!
                spellId = 0;
                Delete();
            }
            break;
        }
        case GAMEOBJECT_TYPE_BARBER_CHAIR:                  //32
        {
            GameObjectTemplate const* info = GetGOInfo();
            if (!info)
                return;

            if (p_User->GetTypeId() != TYPEID_PLAYER)
                return;

            Player* player = p_User->ToPlayer();

            // fallback, will always work
            player->TeleportTo(GetMapId(), GetPositionX(), GetPositionY(), GetPositionZ(), GetOrientation(), TELE_TO_NOT_LEAVE_TRANSPORT | TELE_TO_NOT_LEAVE_COMBAT | TELE_TO_NOT_UNSUMMON_PET);

            WorldPacket data(SMSG_ENABLE_BARBER_SHOP, 0);
            player->GetSession()->SendPacket(&data);

            player->SetStandState(UNIT_STAND_STATE_SIT_LOW_CHAIR+info->barberChair.chairheight);
            return;
        }
        case GAMEOBJECT_TYPE_NEW_FLAG:
        case GAMEOBJECT_TYPE_NEW_FLAG_DROP:
        {
            if (p_User->IsPlayer())
            {
                if (Battleground* bg = p_User->ToPlayer()->GetBattleground())
                    bg->EventPlayerClickedOnFlag(p_User->ToPlayer(), this);
            }
            break;
        }
        default:
            if (GetGoType() >= MAX_GAMEOBJECT_TYPE)
                sLog->outError(LOG_FILTER_GENERAL, "GameObject::Use(): unit (type: %u, guid: %u, name: %s) tries to use object (guid: %u, entry: %u, name: %s) of unknown type (%u)",
                    p_User->GetTypeId(), p_User->GetGUIDLow(), p_User->GetName(), GetGUIDLow(), GetEntry(), GetGOInfo()->name.c_str(), GetGoType());
            break;
    }

    if (!spellId)
        return;

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
    {
        if (p_User->GetTypeId() != TYPEID_PLAYER || !sOutdoorPvPMgr->HandleCustomSpell(p_User->ToPlayer(), spellId, this))
            sLog->outError(LOG_FILTER_GENERAL, "WORLD: unknown spell id %u at use action for gameobject (Entry: %u GoType: %u)", spellId, GetEntry(), GetGoType());
        else
            sLog->outDebug(LOG_FILTER_OUTDOORPVP, "WORLD: %u non-dbc spell was handled by OutdoorPvP", spellId);
        return;
    }

    if (spellCaster)
        spellCaster->CastSpell(p_User, spellInfo, triggered);
    else
        CastSpell(p_User, spellId);
}

void GameObject::CastSpell(Unit* target, uint32 spellId)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return;

    bool self = false;
    for (uint8 i = 0; i < spellInfo->EffectCount; ++i)
    {
        if (spellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_CASTER)
        {
            self = true;
            break;
        }
    }

    if (self)
    {
        if (target)
            target->CastSpell(target, spellInfo, true);
        return;
    }

    //summon world trigger
    Creature* trigger = SummonTrigger(GetPositionX(), GetPositionY(), GetPositionZ(), 0, spellInfo->CalcCastTime() + 100);
    if (!trigger)
        return;

    if (Unit* owner = GetOwner())
    {
        trigger->setFaction(owner->getFaction());
        // needed for GO casts for proper target validation checks
        trigger->SetGuidValue(UNIT_FIELD_SUMMONED_BY, owner->GetGUID());
        trigger->CastSpell(target ? target : trigger, spellInfo, true, 0, nullptr, owner->GetGUID());
    }
    else
    {
        trigger->setFaction(14);
        // Set owner guid for target if no owner available - needed by trigger auras
        // - trigger gets despawned and there's no caster avalible (see AuraEffect::TriggerSpell())
        trigger->CastSpell(target ? target : trigger, spellInfo, true, 0, nullptr, target ? target->GetGUID() : 0);
    }
}

void GameObject::SendCustomAnim(uint32 p_Anim)
{
    WorldPacket l_Data(SMSG_GAMEOBJECT_CUSTOM_ANIM, 16 + 2 + 4 + 1);
    l_Data.appendPackGUID(GetGUID());
    l_Data << uint32(p_Anim);
    l_Data.WriteBit(p_Anim == 0);
    l_Data.FlushBits();
    SendMessageToSet(&l_Data, true);
}

bool GameObject::IsInRange(float x, float y, float z, float radius) const
{
    GameObjectDisplayInfoEntry const* info = sGameObjectDisplayInfoStore.LookupEntry(m_goInfo->displayId);
    if (!info)
        return IsWithinDist3d(x, y, z, radius);

    float sinA = std::sin(GetOrientation());
    float cosA = std::cos(GetOrientation());
    float dx = x - GetPositionX();
    float dy = y - GetPositionY();
    float dz = z - GetPositionZ();
    float dist = sqrt(dx*dx + dy*dy);
    //! Check if the distance between the 2 objects is 0, can happen if both objects are on the same position.
    //! The code below this check wont crash if dist is 0 because 0/0 in float operations is valid, and returns infinite
    if (G3D::fuzzyEq(dist, 0.0f))
        return true;

    float sinB = dx / dist;
    float cosB = dy / dist;
    dx = dist * (cosA * cosB + sinA * sinB);
    dy = dist * (cosA * sinB - sinA * cosB);
    return dx < info->maxX + radius && dx > info->minX - radius
        && dy < info->maxY + radius && dy > info->minY - radius
        && dz < info->maxZ + radius && dz > info->minZ - radius;
}

void GameObject::EventInform(uint32 eventId)
{
    if (!eventId)
        return;

    if (AI())
        AI()->EventInform(eventId);

    if (m_zoneScript)
        m_zoneScript->ProcessEvent(this, eventId);
}

// overwrite WorldObject function for proper name localization
const char* GameObject::GetNameForLocaleIdx(LocaleConstant loc_idx) const
{
    if (loc_idx != DEFAULT_LOCALE)
    {
        uint8 uloc_idx = uint8(loc_idx);
        if (GameObjectLocale const* cl = sObjectMgr->GetGameObjectLocale(GetEntry()))
            if (cl->Name.size() > uloc_idx && !cl->Name[uloc_idx].empty())
                return cl->Name[uloc_idx].c_str();
    }

    return GetName();
}

struct QuatCompressed
{
    QuatCompressed() : m_Raw(0) { };
    QuatCompressed(int64 p_Val) : m_Raw(p_Val) { };

    enum : int32
    {
        PackYZ = 1 << 20,
        PackX  = 1 << 21
    };

    QuatCompressed(G3D::Quat const& p_Quaternion) : m_Raw(0)
    {
        int8 l_WSign = (p_Quaternion.w >= 0 ? 1 : -1);
        int64 l_X = int32(p_Quaternion.x * PackX) * l_WSign & ((1 << 22) - 1);
        int64 l_Y = int32(p_Quaternion.y * PackYZ) * l_WSign & ((1 << 21) - 1);
        int64 l_Z = int32(p_Quaternion.z * PackYZ) * l_WSign & ((1 << 21) - 1);
        m_Raw = l_Z | (l_Y << 21) | (l_X << 42);
    };

    int64 GetComp() const { return m_Raw; };

    private:
        int64 m_Raw;
};

void GameObject::SetRotationQuat(float p_QX, float p_QY, float p_QZ, float p_QW)
{
    G3D::Quat l_Quat(p_QX, p_QY, p_QZ, p_QW);

    // Temporary solution for gameobjects that has no rotation data in DB:
    if (p_QZ == 0 && p_QW == 0)
        l_Quat = G3D::Quat::fromAxisAngleRotation(G3D::Vector3::unitZ(), GetOrientation());

    m_Rotation = QuatCompressed(l_Quat).GetComp();
    SetFloatValue(EGameObjectFields::GAMEOBJECT_FIELD_PARENT_ROTATION + 0, l_Quat.x);
    SetFloatValue(EGameObjectFields::GAMEOBJECT_FIELD_PARENT_ROTATION + 1, l_Quat.y);
    SetFloatValue(EGameObjectFields::GAMEOBJECT_FIELD_PARENT_ROTATION + 2, l_Quat.z);
    SetFloatValue(EGameObjectFields::GAMEOBJECT_FIELD_PARENT_ROTATION + 3, l_Quat.w);
}

void GameObject::SetRotationAngles(float p_ZRot, float p_YRot, float p_XRot)
{
    G3D::Quat quat(G3D::Matrix3::fromEulerAnglesZYX(p_ZRot, p_YRot, p_XRot));
    SetRotationQuat(quat.x, quat.y, quat.z, quat.w);
}

void GameObject::UpdateRotationFields(float rotation2 /*=0.0f*/, float rotation3 /*=0.0f*/)
{
    static double const atan_pow = atan(pow(2.0f, -20.0f));

    double f_rot1 = std::sin(GetOrientation() / 2.0f);
    double f_rot2 = std::cos(GetOrientation() / 2.0f);

    int64 i_rot1 = int64(f_rot1 / atan_pow *(f_rot2 >= 0 ? 1.0f : -1.0f));
    int64 rotation = (i_rot1 << 43 >> 43) & 0x00000000001FFFFF;

    //float f_rot2 = std::sin(0.0f / 2.0f);
    //int64 i_rot2 = f_rot2 / atan(pow(2.0f, -20.0f));
    //rotation |= (((i_rot2 << 22) >> 32) >> 11) & 0x000003FFFFE00000;

    //float f_rot3 = std::sin(0.0f / 2.0f);
    //int64 i_rot3 = f_rot3 / atan(pow(2.0f, -21.0f));
    //rotation |= (i_rot3 >> 42) & 0x7FFFFC0000000000;

    m_Rotation = rotation;

    if (rotation2 == 0.0f && rotation3 == 0.0f)
    {
        rotation2 = (float)f_rot1;
        rotation3 = (float)f_rot2;
    }

    SetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+2, rotation2);
    SetFloatValue(GAMEOBJECT_FIELD_PARENT_ROTATION+3, rotation3);
}

void GameObject::ModifyHealth(int32 change, Unit* attackerOrHealer /*= NULL*/, uint32 spellId /*= 0*/)
{
    if (!GetGOValue()->Building.MaxHealth || !change)
        return;

    // prevent double destructions of the same object
    if (change < 0 && !GetGOValue()->Building.Health)
        return;

    if (int32(GetGOValue()->Building.Health) + change <= 0)
        GetGOValue()->Building.Health = 0;
    else if (int32(GetGOValue()->Building.Health) + change >= int32(GetGOValue()->Building.MaxHealth))
        GetGOValue()->Building.Health = GetGOValue()->Building.MaxHealth;
    else
        GetGOValue()->Building.Health += change;

    // Set the health bar, value = 255 * healthPct;
    SetGoHealth(GetGOValue()->Building.Health * 255 / GetGOValue()->Building.MaxHealth);

    Player* player = attackerOrHealer ? attackerOrHealer->GetCharmerOrOwnerPlayerOrPlayerItself(): NULL;

    // dealing damage, send packet
    // TODO: is there any packet for healing?
    if (change < 0 && player)
    {
        WorldPacket data(SMSG_DESTRUCTIBLE_BUILDING_DAMAGE, (3 * (16 + 2)) + 4 + 4);
        data.appendPackGUID(GetGUID());
        data.appendPackGUID(player->GetGUID());
        data.appendPackGUID(attackerOrHealer->GetGUID());
        data << int32(-change);
        data << int32(spellId);
        player->GetSession()->SendPacket(&data);
    }

    GameObjectDestructibleState newState = GetDestructibleState();

    if (!GetGOValue()->Building.Health)
        newState = GO_DESTRUCTIBLE_DESTROYED;
    else if (GetGOValue()->Building.Health <= GetGOInfo()->building.InteriorVisible)
        newState = GO_DESTRUCTIBLE_DAMAGED;
    else if (GetGOValue()->Building.Health == GetGOValue()->Building.MaxHealth)
        newState = GO_DESTRUCTIBLE_INTACT;

    if (newState == GetDestructibleState())
        return;

    SetDestructibleState(newState, player, false);
}

void GameObject::SetDestructibleState(GameObjectDestructibleState state, Player* eventInvoker /*= NULL*/, bool setHealth /*= false*/)
{
    // the user calling this must know he is already operating on destructible gameobject
    ASSERT(GetGoType() == GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING);

    switch (state)
    {
        case GO_DESTRUCTIBLE_INTACT:
            RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_DAMAGED | GO_FLAG_DESTROYED);
            SetDisplayId(m_goInfo->displayId);
            if (setHealth)
            {
                m_goValue->Building.Health = m_goValue->Building.MaxHealth;
                SetGoHealth(255);
            }
            EnableCollision(true);
            break;
        case GO_DESTRUCTIBLE_DAMAGED:
        {
            EventInform(m_goInfo->building.DamagedEvent);
            sScriptMgr->OnGameObjectDamaged(this, eventInvoker);
            if (eventInvoker)
                if (Battleground* bg = eventInvoker->GetBattleground())
                    bg->EventPlayerDamagedGO(eventInvoker, this, m_goInfo->building.DamagedEvent);

            RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_DESTROYED);
            SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_DAMAGED);

            uint32 modelId = m_goInfo->displayId;
            if (DestructibleModelDataEntry const* modelData = sDestructibleModelDataStore.LookupEntry(m_goInfo->building.DestructibleModelRec))
                if (modelData->DamagedDisplayId)
                    modelId = modelData->DamagedDisplayId;
            SetDisplayId(modelId);

            if (setHealth)
            {
                m_goValue->Building.Health = m_goInfo->building.InteriorVisible;
                uint32 maxHealth = m_goValue->Building.MaxHealth;
                // in this case current health is 0 anyway so just prevent crashing here
                if (!maxHealth)
                    maxHealth = 1;
                SetGoHealth(m_goValue->Building.Health * 255 / maxHealth);
            }
            break;
        }
        case GO_DESTRUCTIBLE_DESTROYED:
        {
            sScriptMgr->OnGameObjectDestroyed(this, eventInvoker);
            EventInform(m_goInfo->building.DestroyedEvent);
            if (eventInvoker)
            {
                if (Battleground* bg = eventInvoker->GetBattleground())
                {
                    bg->EventPlayerDamagedGO(eventInvoker, this, m_goInfo->building.DestroyedEvent);
                    bg->DestroyGate(eventInvoker, this);
                }
            }

            RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_DAMAGED);
            SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_DESTROYED);

            uint32 modelId = m_goInfo->displayId;
            if (DestructibleModelDataEntry const* modelData = sDestructibleModelDataStore.LookupEntry(m_goInfo->building.DestructibleModelRec))
                if (modelData->DestroyedDisplayId)
                    modelId = modelData->DestroyedDisplayId;
            SetDisplayId(modelId);

            if (setHealth)
            {
                m_goValue->Building.Health = 0;
                SetGoHealth(0);
            }
            EnableCollision(false);
            break;
        }
        case GO_DESTRUCTIBLE_REBUILDING:
        {
            EventInform(m_goInfo->building.RebuildingEvent);
            RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_DAMAGED | GO_FLAG_DESTROYED);

            uint32 modelId = m_goInfo->displayId;
            if (DestructibleModelDataEntry const* modelData = sDestructibleModelDataStore.LookupEntry(m_goInfo->building.DestructibleModelRec))
                if (modelData->RebuildingDisplayId)
                    modelId = modelData->RebuildingDisplayId;
            SetDisplayId(modelId);

            // restores to full health
            if (setHealth)
            {
                m_goValue->Building.Health = m_goValue->Building.MaxHealth;
                SetGoHealth(255);
            }
            EnableCollision(true);
            break;
        }
    }

    UpdateObjectVisibility();
}

void GameObject::SetLootState(LootState state, Unit* unit)
{
    m_lootState = state;

    if (AI() != nullptr)
        AI()->OnLootStateChanged(state, unit);

    sScriptMgr->OnGameObjectLootStateChanged(this, state, unit);
    if (m_model)
    {
        bool collision = false;

        // Use the current go state
        if ((GetGoState() != GO_STATE_READY && (state == GO_ACTIVATED || state == GO_JUST_DEACTIVATED)) || state == GO_READY)
            collision = !collision;

        EnableCollision(collision);
    }
}

void GameObject::SetGoState(GOState state)
{
    SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 0, state);

    if (AI() != nullptr)
        AI()->OnStateChanged(state);

    sScriptMgr->OnGameObjectStateChanged(this, state);
    if (m_model)
    {
        if (!IsInWorld())
            return;

        // startOpen determines whether we are going to add or remove the LoS on activation
        bool collision = false;
        if (state == GO_STATE_READY)
            collision = !collision;

        EnableCollision(collision);
    }
}

void GameObject::SetDisplayId(uint32 displayid)
{
    SetUInt32Value(GAMEOBJECT_FIELD_DISPLAY_ID, displayid);
    UpdateModel();
}

void GameObject::SetPhaseMask(uint32 newPhaseMask, bool update)
{
    WorldObject::SetPhaseMask(newPhaseMask, update);
    if (m_model && m_model->isEnabled())
        EnableCollision(true);
}

void GameObject::EnableCollision(bool enable)
{
    if (!m_model)
        return;

    if (enable && !GetMap()->ContainsGameObjectModel(*m_model))
        GetMap()->InsertGameObjectModel(*m_model);

    m_model->enable(enable ? GetPhaseMask() : 0);
}

void GameObject::UpdateModelPosition()
{
    if (!m_model)
        return;

    if (GetMap()->ContainsGameObjectModel(*m_model))
    {
        GetMap()->RemoveGameObjectModel(*m_model);
        m_model->Relocate(*this);
        GetMap()->InsertGameObjectModel(*m_model);
    }
}

void GameObject::UpdateModel()
{
    if (!IsInWorld())
        return;

    if (m_model)
        if (GetMap()->ContainsGameObjectModel(*m_model))
            GetMap()->RemoveGameObjectModel(*m_model);

    delete m_model;

    m_model = GameObjectModel::Create(*this);
    if (m_model)
        GetMap()->InsertGameObjectModel(*m_model);
}

Player* GameObject::GetLootRecipient() const
{
    if (!m_lootRecipient)
        return NULL;
    return ObjectAccessor::FindPlayer(m_lootRecipient);
}

Group* GameObject::GetLootRecipientGroup() const
{
    if (!m_lootRecipientGroup)
        return NULL;
    return sGroupMgr->GetGroupByGUID(m_lootRecipientGroup);
}

void GameObject::SetLootRecipient(Unit* unit)
{
    // set the player whose group should receive the right
    // to loot the creature after it dies
    // should be set to NULL after the loot disappears

    if (!unit)
    {
        m_lootRecipient = 0;
        m_lootRecipientGroup = 0;
        return;
    }

    if (unit->GetTypeId() != TYPEID_PLAYER && !unit->IsVehicle())
        return;

    Player* player = unit->GetCharmerOrOwnerPlayerOrPlayerItself();
    if (!player)                                             // normal creature, no player involved
        return;

    m_lootRecipient = player->GetGUID();
    if (Group* group = player->GetGroup())
        m_lootRecipientGroup = group->GetLowGUID();
}

bool GameObject::IsLootAllowedFor(Player const* p_Player) const
{
    /// If creature is quest tracked and player have the quest, player isn't allowed to loot
    auto l_TrackingQuestId = GetGOInfo()->GetTrackingQuestId();
    auto l_QuestBit = GetQuestUniqueBitFlag(l_TrackingQuestId);

    if (l_TrackingQuestId && p_Player->IsQuestBitFlaged(l_QuestBit))
        return false;

    if (!m_lootRecipient && !m_lootRecipientGroup)
        return true;

    if (p_Player->GetGUID() == m_lootRecipient)
        return true;

    Group const* playerGroup = p_Player->GetGroup();
    if (!playerGroup || playerGroup != GetLootRecipientGroup()) // if we dont have a group we arent the recipient
        return false;                                           // if go doesnt have group bound it means it was solo killed by someone else

    return true;
}

void GameObject::BuildValuesUpdate(uint8 updateType, ByteBuffer* data, Player* target) const
{
    if (!target)
        return;

    bool forcedFlags = GetGoType() == GAMEOBJECT_TYPE_CHEST && GetGOInfo()->chest.usegrouplootrules && HasLootRecipient();
    bool targetIsGM = target->isGameMaster();
    bool isStoppableTransport = GetGoType() == GAMEOBJECT_TYPE_TRANSPORT && !m_goValue->Transport.StopFrames->empty();

    ByteBuffer fieldBuffer;

    UpdateMask updateMask;
    updateMask.SetCount(m_valuesCount);

    uint32* flags = GameObjectUpdateFieldFlags;
    uint32 visibleFlag = UF_FLAG_PUBLIC | UF_FLAG_VIEWER_DEPENDENT;
    if (GetOwnerGUID() == target->GetGUID())
        visibleFlag |= UF_FLAG_OWNER;

    for (uint16 index = 0; index < m_valuesCount; ++index)
    {
        if (_fieldNotifyFlags & flags[index] ||
            ((updateType == UPDATETYPE_VALUES ? _changesMask.GetBit(index) : m_uint32Values[index]) && (flags[index] & visibleFlag)) ||
            (index == GAMEOBJECT_FIELD_FLAGS && forcedFlags) || index == OBJECT_FIELD_DYNAMIC_FLAGS || index == GAMEOBJECT_FIELD_PERCENT_HEALTH || (index == GAMEOBJECT_FIELD_LEVEL && IsTransport()))
        {
            updateMask.SetBit(index);

            if (index == OBJECT_FIELD_DYNAMIC_FLAGS)
            {
                uint16 dynFlags = 0;
                int16 pathProgress = -1;
                switch (GetGoType())
                {
                    case GAMEOBJECT_TYPE_CHEST:
                    case GAMEOBJECT_TYPE_GOOBER:
                        if (ActivateToQuest(target))
                            dynFlags |= GO_DYNFLAG_LO_ACTIVATE | GO_DYNFLAG_LO_SPARKLE;
                        else if (targetIsGM)
                            dynFlags |= GO_DYNFLAG_LO_ACTIVATE;
                        break;
                    case GAMEOBJECT_TYPE_GENERIC:
                        if (ActivateToQuest(target))
                            dynFlags |= GO_DYNFLAG_LO_SPARKLE;
                        break;
                    case GAMEOBJECT_TYPE_TRANSPORT:
                    {
                        float timer = float(m_goValue->Transport.PathProgress % GetTransportPeriod());
                        pathProgress = int16(timer / float(GetTransportPeriod()) * 65535.0f);
                        break;
                    }
                    case GAMEOBJECT_TYPE_MAP_OBJ_TRANSPORT:
                        pathProgress = int16(float(m_goValue->Transport.PathProgress) / float(GetUInt32Value(GAMEOBJECT_FIELD_LEVEL)) * 65535.0f);
                        break;
                }

                fieldBuffer << uint16(dynFlags);
                fieldBuffer << int16(pathProgress);
            }
            else if (index == GAMEOBJECT_FIELD_FLAGS)
            {
                uint32 flags = m_uint32Values[GAMEOBJECT_FIELD_FLAGS];
                if (GetGoType() == GAMEOBJECT_TYPE_CHEST)
                    if ((GetGOInfo()->chest.usegrouplootrules || GetGOInfo()->GetTrackingQuestId()) && !IsLootAllowedFor(target))
                        flags |= GO_FLAG_LOCKED | GO_FLAG_NOT_SELECTABLE;

                fieldBuffer << flags;
            }
            else if (index == GAMEOBJECT_FIELD_LEVEL)
            {
                if (isStoppableTransport)
                    fieldBuffer << uint32(m_goValue->Transport.PathProgress);
                else
                    fieldBuffer << m_uint32Values[index];
            }
            else if (index == GAMEOBJECT_FIELD_PERCENT_HEALTH)
            {
                uint32 bytes1 = m_uint32Values[index];
                if (isStoppableTransport
                    && GetGoState() == GO_STATE_TRANSPORT_ACTIVE
                    && sScriptMgr->OnGameObjectElevatorCheck(this))
                {
                    if ((m_goValue->Transport.StateUpdateTimer / 20000) & 1)
                    {
                        bytes1 &= 0xFFFFFF00;
                        bytes1 |= GO_STATE_TRANSPORT_STOPPED;
                    }
                }
                fieldBuffer << bytes1;
            }
            else
                fieldBuffer << m_uint32Values[index]; // other cases
        }
    }

    *data << uint8(updateMask.GetBlockCount());
    updateMask.AppendToPacket(data);
    data->append(fieldBuffer);
}

void GameObject::GetRespawnPosition(float &x, float &y, float &z, float* ori /* = NULL*/) const
{
    if (m_DBTableGuid)
    {
        if (GameObjectData const* data = sObjectMgr->GetGOData(GetDBTableGUIDLow()))
        {
            x = data->posX;
            y = data->posY;
            z = data->posZ;
            if (ori)
                *ori = data->orientation;
            return;
        }
    }

    x = GetPositionX();
    y = GetPositionY();
    z = GetPositionZ();
    if (ori)
        *ori = GetOrientation();
}

Transport* GameObject::ToTransport()
{
    return dynamic_cast<Transport*>(this);
}

Transport const* GameObject::ToTransport() const
{
    return dynamic_cast<Transport const*>(this);
}

void GameObject::SendTransportToOutOfRangePlayers() const
{
    Map::PlayerList const& players = GetMap()->GetPlayers();
    if (players.isEmpty())
        return;

    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
    {
        if (GetDistance(*itr->getSource()) < GetVisibilityRange())
            continue;

        UpdateData data(GetMapId());
        WorldPacket pkt;
        BuildCreateUpdateBlockForPlayer(&data, itr->getSource());
        if (data.BuildPacket(&pkt))
            itr->getSource()->GetSession()->SendPacket(&pkt);
    }
}

void GameObject::SendGameObjectActivateAnimKit(uint32 p_AnimKitID, bool p_Maintain /*= true*/, Player* p_Target /*= nullptr*/)
{
    /// It'll be sent in CreateObject
    SetAIAnimKitId(p_AnimKitID, false);

    WorldPacket l_Data(Opcodes::SMSG_GAME_OBJECT_ACTIVATE_ANIM_KIT, 16 + 4 + 1);
    l_Data.appendPackGUID(GetGUID());
    l_Data << uint32(p_AnimKitID);
    l_Data.WriteBit(p_Maintain);
    l_Data.FlushBits();

    if (p_Target != nullptr)
        p_Target->GetSession()->SendPacket(&l_Data);
    else
        GetMap()->SendToPlayers(&l_Data);
}
