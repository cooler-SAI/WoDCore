////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "MoveSplineInit.h"
#include "MoveSpline.h"
#include "Unit.h"
#include "Transport.h"

enum MonsterMoveType
{
    MonsterMoveNormal       = 0,
    MonsterMoveStop         = 1,
    MonsterMoveFacingSpot   = 2,
    MonsterMoveFacingTarget = 3,
    MonsterMoveFacingAngle  = 4
};

struct MonsterSplineFilterKey
{
    int16 Idx = 0;
    int16 Speed = 0;
};

struct MonsterSplineFilter
{
    std::vector<MonsterSplineFilterKey> FilterKeys;
    uint8 FilterFlags = 0;
    float BaseSpeed = 0.0f;
    int16 StartOffset = 0;
    float DistToPrevFilterKey = 0.0f;
    int16 AddedToStart = 0;
};

struct MovementSpline
{
    uint32 Flags = 0;    // Spline flags
    uint8 Face = 0;    // Movement direction (see MonsterMoveType enum)
    uint8 AnimTier = 0;
    uint32 TierTransStartTime = 0;
    uint32 Elapsed = 0;
    uint32 MoveTime = 0;
    float JumpGravity = 0.0f;
    uint32 SpecialTime = 0;
    std::vector<G3D::Vector3> Points;   // Spline path
    uint8 Mode = 0;    // Spline mode - actually always 0 in this packet - Catmullrom mode appears only in SMSG_UPDATE_OBJECT. In this packet it is determined by flags
    uint8 VehicleExitVoluntary = 0;
    uint64 TransportGUID;
    uint8 VehicleSeat = 255;
    std::vector<G3D::Vector3> PackedDeltas;
    MonsterSplineFilter SplineFilter;
    float FaceDirection = 0.0f;
    uint64 FaceGUID;
    G3D::Vector3 FaceSpot;
};

struct MovementMonsterSpline
{
    uint32 ID = 0;
    G3D::Vector3 Destination;
    bool CrzTeleport = false;
    MovementSpline Move;
};

class CommonMovement
{
public:
    static void WriteCreateObjectSplineDataBlock(::Movement::MoveSpline const& moveSpline, ByteBuffer& data);
};

class MonsterMove
{
public:
    MonsterMove()
    {

    }

    void InitializeSplineData(::Movement::MoveSpline const& moveSpline);

    void Write(WorldPacket * data);

    MovementMonsterSpline SplineData;
    uint64 MoverGUID;
    G3D::Vector3 Pos;
};



ByteBuffer& operator<<(ByteBuffer& data, MonsterSplineFilterKey const& monsterSplineFilterKey)
{
    data << monsterSplineFilterKey.Idx;
    data << monsterSplineFilterKey.Speed;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, MonsterSplineFilter const& monsterSplineFilter)
{
    data << uint32(monsterSplineFilter.FilterKeys.size());
    data << monsterSplineFilter.BaseSpeed;
    data << monsterSplineFilter.StartOffset;
    data << monsterSplineFilter.DistToPrevFilterKey;
    data << monsterSplineFilter.AddedToStart;
    for (MonsterSplineFilterKey const& filterKey : monsterSplineFilter.FilterKeys)
        data << filterKey;
    data.WriteBits(monsterSplineFilter.FilterFlags, 2);
    data.FlushBits();

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, MovementSpline const& movementSpline)
{
    data << movementSpline.Flags;
    data << movementSpline.AnimTier;
    data << movementSpline.TierTransStartTime;
    data << movementSpline.Elapsed;
    data << movementSpline.MoveTime;
    data << movementSpline.JumpGravity;
    data << movementSpline.SpecialTime;
    data << int32(movementSpline.Points.size());
    data << movementSpline.Mode;
    data << movementSpline.VehicleExitVoluntary;
    data.appendPackGUID(movementSpline.TransportGUID);
    data << movementSpline.VehicleSeat;
    data << int32(movementSpline.PackedDeltas.size());
    for (G3D::Vector3 const& pos : movementSpline.Points)
    {
        data << pos.x;
        data << pos.y;
        data << pos.z;
    }
    for (G3D::Vector3 const& pos : movementSpline.PackedDeltas)
        data.appendPackXYZ(pos.x, pos.y, pos.z);
    data.WriteBits(movementSpline.Face, 2);
    data.WriteBit(false/*movementSpline.SplineFilter.is_initialized()*/);
    data.FlushBits();

    switch (movementSpline.Face)
    {
    case ::Movement::MONSTER_MOVE_FACING_SPOT:
        data << movementSpline.FaceSpot.x;
        data << movementSpline.FaceSpot.y;
        data << movementSpline.FaceSpot.z;
        break;
    case ::Movement::MONSTER_MOVE_FACING_TARGET:
        data << movementSpline.FaceDirection;
        data.appendPackGUID(movementSpline.FaceGUID);
        break;
    case ::Movement::MONSTER_MOVE_FACING_ANGLE:
        data << movementSpline.FaceDirection;
        break;
    }

  //  if (movementSpline.SplineFilter)
    //    data << *movementSpline.SplineFilter;

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, MovementMonsterSpline const& movementMonsterSpline)
{
    data << movementMonsterSpline.ID;
    data << movementMonsterSpline.Destination.x;
    data << movementMonsterSpline.Destination.y;
    data << movementMonsterSpline.Destination.z;
    data << movementMonsterSpline.Move;
    data.WriteBit(movementMonsterSpline.CrzTeleport);

    // Unk bits. 0 if monster is moving, 1 or 2 if stopped
    if (movementMonsterSpline.Move.Flags)
        data.WriteBits(0, 2);
    else
        data.WriteBits(2, 2);

    data.FlushBits();

    return data;
}

void MonsterMove::Write(WorldPacket * _worldPacket)
{
    (*_worldPacket).appendPackGUID(MoverGUID);
    (*_worldPacket) << Pos.x;
    (*_worldPacket) << Pos.y;
    (*_worldPacket) << Pos.z;
    (*_worldPacket) << SplineData;
}



void MonsterMove::InitializeSplineData(::Movement::MoveSpline const& moveSpline)
{
    SplineData.ID = moveSpline.m_Id;
    MovementSpline& movementSpline = SplineData.Move;

    ::Movement::MoveSplineFlag splineFlags = moveSpline.splineflags;
    splineFlags.enter_cycle = moveSpline.isCyclic();
    movementSpline.Flags = uint32(splineFlags & uint32(~::Movement::MoveSplineFlag::Mask_No_Monster_Move));

    switch (moveSpline.splineflags & ::Movement::MoveSplineFlag::Mask_Final_Facing)
    {
    case ::Movement::MoveSplineFlag::Final_Point:
        movementSpline.Face = ::Movement::MONSTER_MOVE_FACING_SPOT;
        movementSpline.FaceSpot = moveSpline.facing.f;
        break;
    case ::Movement::MoveSplineFlag::Final_Target:
        movementSpline.Face = ::Movement::MONSTER_MOVE_FACING_TARGET;
        movementSpline.FaceGUID = moveSpline.facing.target;
        break;
    case ::Movement::MoveSplineFlag::Final_Angle:
        movementSpline.Face = ::Movement::MONSTER_MOVE_FACING_ANGLE;
        movementSpline.FaceDirection = moveSpline.facing.angle;
        break;
    default:
        movementSpline.Face = ::Movement::MONSTER_MOVE_NORMAL;
        break;
    }

    if (splineFlags.animation)
    {
        movementSpline.AnimTier = splineFlags.getAnimationId();
        movementSpline.TierTransStartTime = moveSpline.effect_start_time;
    }

    movementSpline.MoveTime = moveSpline.Duration();

    if (splineFlags.parabolic)
    {
        movementSpline.JumpGravity = moveSpline.vertical_acceleration;
        movementSpline.SpecialTime = moveSpline.effect_start_time;
    }

    ::Movement::Spline<int32> const& spline = moveSpline.spline;
    std::vector<G3D::Vector3> const& array = spline.getPoints();

    if (splineFlags & ::Movement::MoveSplineFlag::UncompressedPath)
    {
        if (!splineFlags.cyclic)
        {
            uint32 count = spline.getPointCount() - 3;
            for (uint32 i = 0; i < count; ++i)
                movementSpline.Points.push_back(G3D::Vector3(array[i + 2].x + (float(i) / 1000.0f), array[i + 2].y, array[i + 2].z));
        }
        else
        {
            uint32 count = spline.getPointCount() - 3;
            movementSpline.Points.push_back(G3D::Vector3(array[1].x + (float(50) / 1000.0f), array[1].y, array[1].z));
            for (uint32 i = 0; i < count; ++i)
                movementSpline.Points.push_back(G3D::Vector3(array[i + 1].x + (float(i) / 1000.0f), array[i + 1].y, array[i + 1].z));
        }
    }
    else
    {
        uint32 lastIdx = spline.getPointCount() - 3;
        G3D::Vector3 const* realPath = &spline.getPoint(1);

        movementSpline.Points.push_back(G3D::Vector3(realPath[lastIdx].x + (float(50) / 1000.0f), realPath[lastIdx].y, realPath[lastIdx].z));

        if (lastIdx > 1)
        {
            G3D::Vector3 middle = (realPath[0] + realPath[lastIdx]) / 2.0f;

            // first and last points already appended
            for (uint32 i = 1; i < lastIdx; ++i)
                movementSpline.PackedDeltas.push_back(middle - realPath[i]);
        }
    }
}


namespace Movement
{
    UnitMoveType SelectSpeedType(uint32 moveFlags)
    {
        if (moveFlags & MOVEMENTFLAG_FLYING)
        {
            if (moveFlags & MOVEMENTFLAG_BACKWARD /*&& speed_obj.flight >= speed_obj.flight_back*/)
                return MOVE_FLIGHT_BACK;
            else
                return MOVE_FLIGHT;
        }
        else if (moveFlags & MOVEMENTFLAG_SWIMMING)
        {
            if (moveFlags & MOVEMENTFLAG_BACKWARD /*&& speed_obj.swim >= speed_obj.swim_back*/)
                return MOVE_SWIM_BACK;
            else
                return MOVE_SWIM;
        }
        else if (moveFlags & MOVEMENTFLAG_WALKING)
        {
            //if (speed_obj.run > speed_obj.walk)
            return MOVE_WALK;
        }
        else if (moveFlags & MOVEMENTFLAG_BACKWARD /*&& speed_obj.run >= speed_obj.run_back*/)
            return MOVE_RUN_BACK;

        // Flying creatures use MOVEMENTFLAG_CAN_FLY or MOVEMENTFLAG_DISABLE_GRAVITY
        // Run speed is their default flight speed.
        return MOVE_RUN;
    }

    int32 MoveSplineInit::Launch()
    {
        MoveSpline& l_MoveSpline = *unit->movespline;

        bool transport = unit->GetTransGUID() != 0;
        Location real_position;
        // there is a big chance that current position is unknown if current state is not finalized, need compute it
        // this also allows calculate spline position and update map position in much greater intervals
        // Don't compute for transport movement if the unit is in a motion between two transports
        if (!l_MoveSpline.Finalized() && l_MoveSpline.onTransport == transport && !(l_MoveSpline.splineflags.transportEnter && args.flags.transportExit))
            real_position = l_MoveSpline.ComputePosition();
        else
        {
            Position const* pos;
            if (!transport)
                pos = unit;
            else
                pos = &unit->m_movementInfo.t_pos;

            real_position.x = pos->GetPositionX();
            real_position.y = pos->GetPositionY();
            real_position.z = pos->GetPositionZ();
            real_position.orientation = unit->GetOrientation();
        }

        // should i do the things that user should do? - no.
        if (args.path.empty())
            return 0;

        // correct first vertex
        args.path[0] = real_position;
        args.initialOrientation = real_position.orientation;
        l_MoveSpline.onTransport = unit->GetTransGUID() != 0;

        uint32 moveFlags = unit->m_movementInfo.GetMovementFlags();
        moveFlags |= MOVEMENTFLAG_FORWARD;

        if (moveFlags & MOVEMENTFLAG_ROOT)
            moveFlags &= ~MOVEMENTFLAG_MASK_MOVING;

        if (!args.HasVelocity)
        {
            // If spline is initialized with SetWalk method it only means we need to select
            // walk move speed for it but not add walk flag to unit
            uint32 moveFlagsForSpeed = moveFlags;
            if (args.flags.walkmode)
                moveFlagsForSpeed |= MOVEMENTFLAG_WALKING;
            else
                moveFlagsForSpeed &= ~MOVEMENTFLAG_WALKING;

            args.velocity = unit->GetSpeed(SelectSpeedType(moveFlagsForSpeed));
        }

        if (!args.Validate(unit))
            return 0;

        unit->m_movementInfo.SetMovementFlags(moveFlags);
        l_MoveSpline.Initialize(args);

        MonsterMove packet;
        packet.MoverGUID = unit->GetGUID();
        packet.Pos = real_position;
        packet.InitializeSplineData(l_MoveSpline);
        if (transport)
        {
            packet.SplineData.Move.TransportGUID = unit->GetTransGUID();
            packet.SplineData.Move.VehicleSeat = unit->GetTransSeat();
        }

        WorldPacket l_Data(SMSG_MONSTER_MOVE);
        packet.Write(&l_Data);

        unit->SendMessageToSet(&l_Data, true);

        return l_MoveSpline.Duration();
    }

    void MoveSplineInit::Stop(bool /*force*/)
    {
        MoveSpline& move_spline = *unit->movespline;

        // No need to stop if we are not moving
        if (move_spline.Finalized())
            return;

        bool transport = unit->GetTransGUID() != 0;
        Location loc;
        if (move_spline.onTransport == transport)
            loc = move_spline.ComputePosition();
        else
        {
            Position const* pos;
            if (!transport)
                pos = unit;
            else
                pos = &unit->m_movementInfo.t_pos;

            loc.x = pos->GetPositionX();
            loc.y = pos->GetPositionY();
            loc.z = pos->GetPositionZ();
            loc.orientation = unit->GetOrientation();
        }

        args.flags = MoveSplineFlag::Done;
        unit->m_movementInfo.RemoveMovementFlag(MOVEMENTFLAG_FORWARD);
        move_spline.onTransport = transport;
        move_spline.Initialize(args);

        MonsterMove packet;
        packet.MoverGUID = unit->GetGUID();
        packet.Pos = loc;
        packet.SplineData.ID = move_spline.GetId();

        if (transport)
        {
            packet.SplineData.Move.TransportGUID = unit->GetTransGUID();
            packet.SplineData.Move.VehicleSeat = unit->GetTransSeat();
        }

        WorldPacket l_Data(SMSG_MONSTER_MOVE);
        packet.Write(&l_Data);

        unit->SendMessageToSet(&l_Data, true);
    }

    MoveSplineInit::MoveSplineInit(Unit* m) : unit(m)
    {
        args.splineId = splineIdGen.NewId();
        // Elevators also use MOVEMENTFLAG_ONTRANSPORT but we do not keep track of their position changes
        args.TransformForTransport = unit->GetTransGUID() != 0;
        // mix existing state into new
        args.flags.walkmode = unit->m_movementInfo.HasMovementFlag(MOVEMENTFLAG_WALKING);
        args.flags.flying = unit->m_movementInfo.HasMovementFlag(MovementFlags(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_DISABLE_GRAVITY));
        args.flags.smoothGroundPath = true; // enabled by default, CatmullRom mode or client config "pathSmoothing" will disable this
    }

    void MoveSplineInit::SetFacing(const Unit* target)
    {
        args.flags.EnableFacingTarget();
        args.facing.target = target->GetGUID();
    }

    void MoveSplineInit::SetFacing(float angle)
    {
        if (args.TransformForTransport)
        {
            if (Unit* vehicle = unit->GetVehicleBase())
                angle -= vehicle->GetOrientation();
            else if (Transport* transport = unit->GetTransport())
                angle -= transport->GetOrientation();
        }

        args.facing.angle = G3D::wrap(angle, 0.0f, (float)G3D::twoPi());
        args.flags.EnableFacingAngle();
    }

    void MoveSplineInit::MoveTo(const Vector3& dest, bool generatePath, bool forceDestination)
    {
        if (generatePath)
        {
            PathGenerator path(unit);
            bool result = path.CalculatePath(dest.x, dest.y, dest.z, forceDestination);
            if (result && !(path.GetPathType() & PATHFIND_NOPATH))
            {
                MovebyPath(path.GetPath());
                return;
            }
        }

        args.path_Idx_offset = 0;
        args.path.resize(2);
        TransportPathTransform transform(unit, args.TransformForTransport);
        args.path[1] = transform(dest);
    }

    void MoveSplineInit::SetFall()
    {
        args.flags.EnableFalling();
        args.flags.fallingSlow = unit->HasUnitMovementFlag(MOVEMENTFLAG_FALLING_SLOW);
    }

    Vector3 TransportPathTransform::operator()(Vector3 input)
    {
        if (_transformForTransport)
        {
            if (TransportBase* transport = _owner->GetDirectTransport())
            {
                float l_O = 0;
                transport->CalculatePassengerOffset(input.x, input.y, input.z, l_O);
            }
        }

        return input;
    }
}
