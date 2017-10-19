////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Battleground.h"
#include "BattlegroundTV.h"
#include "Language.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "WorldPacket.h"

BattlegroundTV::BattlegroundTV()
{
    BgObjects.resize(BG_TV_OBJECT_MAX);

    StartDelayTimes[BG_STARTING_EVENT_FIRST]  = BG_START_DELAY_1M;
    StartDelayTimes[BG_STARTING_EVENT_SECOND] = BG_START_DELAY_30S;
    StartDelayTimes[BG_STARTING_EVENT_THIRD]  = BG_START_DELAY_15S;
    StartDelayTimes[BG_STARTING_EVENT_FOURTH] = BG_START_DELAY_NONE;
    //we must set messageIds
    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_ARENA_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_ARENA_THIRTY_SECONDS;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_ARENA_FIFTEEN_SECONDS;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_ARENA_HAS_BEGUN;
}

BattlegroundTV::~BattlegroundTV()
{

}

void BattlegroundTV::StartingEventCloseDoors()
{
    for (uint32 i = BG_TV_OBJECT_DOOR_1; i <= BG_TV_OBJECT_DOOR_2; ++i)
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
}

void BattlegroundTV::StartingEventOpenDoors()
{
    for (uint32 i = BG_TV_OBJECT_DOOR_1; i <= BG_TV_OBJECT_DOOR_2; ++i)
        DoorOpen(i);

    for (uint32 i = BG_TV_OBJECT_BUFF_1; i <= BG_TV_OBJECT_BUFF_2; ++i)
        SpawnBGObject(i, 60);
}

void BattlegroundTV::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    //create score and add it to map, default values are set in constructor
    BattlegroundTVScore* sc = new BattlegroundTVScore;

    PlayerScores[player->GetGUID()] = sc;

    UpdateArenaWorldState();
}

void BattlegroundTV::RemovePlayer(Player* /*player*/, uint64 /*guid*/, uint32 /*team*/)
{
    if (GetStatus() == STATUS_WAIT_LEAVE)
        return;

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

void BattlegroundTV::HandleKillPlayer(Player* player, Player* killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (!killer)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundTV: Killer player not found");
        return;
    }

    Battleground::HandleKillPlayer(player, killer);

    UpdateArenaWorldState();
    CheckArenaWinConditions();
}

bool BattlegroundTV::HandlePlayerUnderMap(Player* player)
{
    player->TeleportTo(GetMapId(), -10716.234375f, 429.321808f, 24.411819f, player->GetOrientation(), false);
    return true;
}

void BattlegroundTV::HandleAreaTrigger(Player* /*Source*/, uint32 Trigger)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    switch (Trigger)
    {
        case 4536:                                          // buff trigger?
        case 4537:                                          // buff trigger?
            break;
        default:
            break;
    }
}

void BattlegroundTV::FillInitialWorldStates(ByteBuffer &data)
{
    data << uint32(0xE1A) << uint32(1);
    UpdateArenaWorldState();
}

void BattlegroundTV::Reset()
{
    // Call parent's class reset
    Battleground::Reset();
}

bool BattlegroundTV::SetupBattleground()
{
    // Gates
    if (!AddObject(BG_TV_OBJECT_DOOR_1, BG_TV_OBJECT_TYPE_DOOR_1, -10774.6f, 430.992f, 24.41076f, 0.0156f, 0.0f, 0.0f, 0.0078f, RESPAWN_IMMEDIATELY)
        || !AddObject(BG_TV_OBJECT_DOOR_2, BG_TV_OBJECT_TYPE_DOOR_2, -10655.0f, 428.117f, 24.416f, 3.148f, 0.0f, 0.0f, 1.0f, RESPAWN_IMMEDIATELY)
    // Buffs
        || !AddObject(BG_TV_OBJECT_BUFF_1, BG_TV_OBJECT_TYPE_BUFF_1, -10717.63f, 383.8223f, 24.412825f, 1.555f, 0.0f, 0.0f, 0.70154f, 120)
        || !AddObject(BG_TV_OBJECT_BUFF_2, BG_TV_OBJECT_TYPE_BUFF_2, -10716.6f, 475.364f, 24.4131f, 0.0f, 0.0f, 0.70068f, -0.713476f, 120))
    {
        sLog->outError(LOG_FILTER_SQL, "BattlegroundTV: Failed to spawn some object!");
        return false;
    }

    return true;
}
