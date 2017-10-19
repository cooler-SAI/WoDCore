////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "World.h"
#include "WorldPacket.h"
#include "ObjectMgr.h"
#include "BattlegroundMgr.hpp"
#include "Battleground.h"
#include "BattlegroundAB.h"
#include "Creature.h"
#include "Language.h"
#include "Object.h"
#include "Player.h"
#include "Util.h"

BattlegroundAB::BattlegroundAB()
{
    m_BuffChange = true;
    BgObjects.resize(BG_AB_OBJECT_MAX);
    BgCreatures.resize(BG_AB_ALL_NODES_COUNT + 5);//+5 for aura triggers

    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_AB_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_AB_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_AB_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_AB_HAS_BEGUN;

    for (int l_Index = 0; l_Index < BG_AB_DYNAMIC_NODES_COUNT; l_Index++)
        m_BannerWorldState[l_Index] = ArathiBannerWorldState::Neutral;
}

BattlegroundAB::~BattlegroundAB()
{
}

void BattlegroundAB::PostUpdateImpl(uint32 diff)
{
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        int team_points[MS::Battlegrounds::TeamsCount::Value] = { 0, 0 };

        for (int node = 0; node < BG_AB_DYNAMIC_NODES_COUNT; ++node)
        {
            // 1-minute to occupy a node from contested state
            if (m_NodeTimers[node])
            {
                if (m_NodeTimers[node] > diff)
                    m_NodeTimers[node] -= diff;
                else
                {
                    m_NodeTimers[node] = 0;
                    // Change from contested to occupied !
                    uint8 teamIndex = m_Nodes[node]-1;
                    m_prevNodes[node] = m_Nodes[node];
                    m_Nodes[node] += 2;

                    // create new occupied banner
                    _ChangeBanner(node, BG_AB_NODE_TYPE_OCCUPIED, teamIndex, true);
                    _SendNodeUpdate(node);
                    _NodeOccupied(node, (teamIndex == 0) ? ALLIANCE : HORDE);

                    // Message to chatlog
                    if (teamIndex == 0)
                    {
                        // FIXME: team and node names not localized
                        SendMessage2ToAll(LANG_BG_AB_NODE_TAKEN, CHAT_MSG_BG_SYSTEM_ALLIANCE, NULL, LANG_BG_AB_ALLY, _GetNodeNameId(node));
                        PlaySoundToAll(BG_AB_SOUND_NODE_CAPTURED_ALLIANCE);
                    }
                    else
                    {
                        // FIXME: team and node names not localized
                        SendMessage2ToAll(LANG_BG_AB_NODE_TAKEN, CHAT_MSG_BG_SYSTEM_HORDE, NULL, LANG_BG_AB_HORDE, _GetNodeNameId(node));
                        PlaySoundToAll(BG_AB_SOUND_NODE_CAPTURED_HORDE);
                    }
                }
            }

            for (int team = 0; team < MS::Battlegrounds::TeamsCount::Value; ++team)
                if (m_Nodes[node] == team + BG_AB_NODE_TYPE_OCCUPIED)
                    ++team_points[team];
        }

        // Accumulate points
        for (int team = 0; team < MS::Battlegrounds::TeamsCount::Value; ++team)
        {
            int points = team_points[team];
            if (!points)
                continue;
            m_lastTick[team] += diff;
            if (m_lastTick[team] > BG_AB_TickIntervals[points])
            {
                m_lastTick[team] -= BG_AB_TickIntervals[points];
                m_TeamScores[team] += BG_AB_TickPoints[points];
                m_HonorScoreTics[team] += BG_AB_TickPoints[points];
                m_ReputationScoreTics[team] += BG_AB_TickPoints[points];
                if (m_ReputationScoreTics[team] >= m_ReputationTics)
                {
                    (team == BG_TEAM_ALLIANCE) ? RewardReputationToTeam(509, 10, ALLIANCE) : RewardReputationToTeam(510, 10, HORDE);
                    m_ReputationScoreTics[team] -= m_ReputationTics;
                }
                if (m_HonorScoreTics[team] >= m_HonorTics)
                {
                    RewardHonorToTeam(GetBonusHonorFromKill(1), (team == BG_TEAM_ALLIANCE) ? ALLIANCE : HORDE, MS::Battlegrounds::RewardCurrencyType::Type::BattlegroundObjectif);
                    m_HonorScoreTics[team] -= m_HonorTics;
                }
                if (!m_IsInformedNearVictory && m_TeamScores[team] > BG_AB_WARNING_NEAR_VICTORY_SCORE)
                {
                    if (team == BG_TEAM_ALLIANCE)
                        SendMessageToAll(LANG_BG_AB_A_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                    else
                        SendMessageToAll(LANG_BG_AB_H_NEAR_VICTORY, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                    PlaySoundToAll(BG_AB_SOUND_NEAR_VICTORY);
                    m_IsInformedNearVictory = true;
                }

                if (m_TeamScores[team] > BG_AB_MAX_TEAM_SCORE)
                    m_TeamScores[team] = BG_AB_MAX_TEAM_SCORE;
                if (team == BG_TEAM_ALLIANCE)
                    UpdateWorldState(BG_AB_OP_RESOURCES_ALLY, m_TeamScores[team]);
                if (team == BG_TEAM_HORDE)
                    UpdateWorldState(BG_AB_OP_RESOURCES_HORDE, m_TeamScores[team]);
                // update achievement flags
                // we increased m_TeamScores[team] so we just need to check if it is 500 more than other teams resources
                uint8 otherTeam = (team + 1) % MS::Battlegrounds::TeamsCount::Value;
                if (m_TeamScores[team] > m_TeamScores[otherTeam] + 500)
                    m_TeamScores500Disadvantage[otherTeam] = true;
            }
        }

        // Test win condition
        if (m_TeamScores[BG_TEAM_ALLIANCE] >= BG_AB_MAX_TEAM_SCORE)
            EndBattleground(ALLIANCE);

        if (m_TeamScores[BG_TEAM_HORDE] >= BG_AB_MAX_TEAM_SCORE)
            EndBattleground(HORDE);
    }

    if (GetStatus() == STATUS_WAIT_JOIN)
    {
        m_CheatersCheckTimer -= diff;
        if (m_CheatersCheckTimer <= 0)
        {
            for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            {
                Player * plr = ObjectAccessor::FindPlayer(itr->first);
                if (!plr || !plr->IsInWorld())
                    continue;
                if (plr->GetPositionZ() < -19)
                {
                    if (plr->GetBGTeam() == HORDE)
                        plr->TeleportTo(529, 685.58f, 683.21f, -12.91f, plr->GetOrientation(), 0);
                    else
                        plr->TeleportTo(529, 1313.90f, 1310.73f, -9.01f, plr->GetOrientation(), 0);
                }
            }
            m_CheatersCheckTimer = 4000;
        }
    }
}

void BattlegroundAB::StartingEventCloseDoors()
{
    // despawn banners, auras and buffs
    for (int obj = BG_AB_OBJECT_BANNER; obj < BG_AB_DYNAMIC_NODES_COUNT; ++obj)
        SpawnBGObject(obj, RESPAWN_ONE_DAY);

    for (int i = 0; i < BG_AB_DYNAMIC_NODES_COUNT * 3; ++i)
        SpawnBGObject(BG_AB_OBJECT_SPEEDBUFF_STABLES + i, RESPAWN_ONE_DAY);

    // Starting doors
    DoorClose(BG_AB_OBJECT_GATE_A);
    DoorClose(BG_AB_OBJECT_GATE_H);
    SpawnBGObject(BG_AB_OBJECT_GATE_A, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_AB_OBJECT_GATE_H, RESPAWN_IMMEDIATELY);

    // Starting base spirit guides
    _NodeOccupied(BG_AB_SPIRIT_ALLIANCE, ALLIANCE);
    _NodeOccupied(BG_AB_SPIRIT_HORDE, HORDE);
}

void BattlegroundAB::StartingEventOpenDoors()
{
    // spawn neutral banners
    for (int banner = BG_AB_OBJECT_BANNER; banner < 5; ++banner)
    {
        SpawnBGObject(banner, RESPAWN_IMMEDIATELY);
        if (GameObject* l_Banner = GetBGObject(banner))
            l_Banner->SetUInt32Value(GAMEOBJECT_FIELD_SPELL_VISUAL_ID, (uint32)ArathiBannerSpellVisual::Neutral);
    }

    for (int i = 0; i < BG_AB_DYNAMIC_NODES_COUNT; ++i)
    {
        //randomly select buff to spawn
        uint8 buff = urand(0, 2);
        SpawnBGObject(BG_AB_OBJECT_SPEEDBUFF_STABLES + buff + i * 3, RESPAWN_IMMEDIATELY);
    }

    DoorOpen(BG_AB_OBJECT_GATE_A);
    DoorOpen(BG_AB_OBJECT_GATE_H);

    // Achievement: Let's Get This Done
    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, AB_EVENT_START_BATTLE);
}

void BattlegroundAB::AddPlayer(Player* player)
{
    Battleground::AddPlayer(player);
    //create score and add it to map, default values are set in the constructor
    BattlegroundABScore* sc = new BattlegroundABScore;

    PlayerScores[player->GetGUID()] = sc;
}

void BattlegroundAB::RemovePlayer(Player* /*player*/, uint64 /*guid*/, uint32 /*team*/)
{

}

void BattlegroundAB::HandleAreaTrigger(Player* Source, uint32 Trigger)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    switch (Trigger)
    {
        case 3948:                                          // Arathi Basin Alliance Exit.
            if (Source->GetBGTeam() == ALLIANCE)
                Source->LeaveBattleground();
            break;
        case 3949:                                          // Arathi Basin Horde Exit.
            if (Source->GetBGTeam() == HORDE)
                Source->LeaveBattleground();
            break;
        case 3866:                                          // Stables
        case 3869:                                          // Gold Mine
        case 3867:                                          // Farm
        case 3868:                                          // Lumber Mill
        case 3870:                                          // Black Smith
        case 4020:                                          // Unk1
        case 4021:                                          // Unk2
        case 4674:                                          // Unk3
            //break;
        default:
            break;
    }
}

/*  type: 0-neutral, 1-contested, 3-occupied
    teamIndex: 0-ally, 1-horde                        */
void BattlegroundAB::_ChangeBanner(uint8 node, uint8 type, uint8 teamIndex, bool /*delay*/)
{
    GameObject* l_Banner = GetBGObject(node);
    if (l_Banner == nullptr)
        return;

    ArathiBannerWorldState l_WorldStateValue = ArathiBannerWorldState::Neutral;

    uint32 l_SpellVisualId   = (uint32)ArathiBannerSpellVisual::Neutral;

    if (type == BG_AB_NODE_TYPE_CONTESTED)
    {
        if (teamIndex == 1)
        {
            l_SpellVisualId   = (uint32)ArathiBannerSpellVisual::HordeContested;
            l_WorldStateValue = ArathiBannerWorldState::HordeContested;
        }
        else
        {
            l_SpellVisualId   = (uint32)ArathiBannerSpellVisual::AllianceContested;
            l_WorldStateValue = ArathiBannerWorldState::AllianceContested;
        }

    }

    if (type == BG_AB_NODE_TYPE_OCCUPIED)
    {
        if (teamIndex == 1)
        {
            l_SpellVisualId   = (uint32)ArathiBannerSpellVisual::HordeOccupied;
            l_WorldStateValue = ArathiBannerWorldState::HordeOccupied;
        }
        else
        {
            l_SpellVisualId   = (uint32)ArathiBannerSpellVisual::AllianceOccupied;
            l_WorldStateValue = ArathiBannerWorldState::AllianceOccupied;
        }
    }


    // Update the visual of the banner
    l_Banner->SetUInt32Value(GAMEOBJECT_FIELD_SPELL_VISUAL_ID, l_SpellVisualId);

    // Update the worldstate
    m_BannerWorldState[node] = l_WorldStateValue;
    UpdateWorldState(l_Banner->GetGOInfo()->capturePoint.worldState1, (uint8)l_WorldStateValue);
}

int32 BattlegroundAB::_GetNodeNameId(uint8 node)
{
    switch (node)
    {
        case BG_AB_NODE_STABLES:    return LANG_BG_AB_NODE_STABLES;
        case BG_AB_NODE_BLACKSMITH: return LANG_BG_AB_NODE_BLACKSMITH;
        case BG_AB_NODE_FARM:       return LANG_BG_AB_NODE_FARM;
        case BG_AB_NODE_LUMBER_MILL:return LANG_BG_AB_NODE_LUMBER_MILL;
        case BG_AB_NODE_GOLD_MINE:  return LANG_BG_AB_NODE_GOLD_MINE;
        default:
            ASSERT(false);
    }
    return 0;
}

void BattlegroundAB::FillInitialWorldStates(ByteBuffer& data)
{
    const uint8 plusArray[] = {0, 2, 3, 0, 1};

    // Node icons
    for (uint8 node = 0; node < BG_AB_DYNAMIC_NODES_COUNT; ++node)
        data << uint32(BG_AB_OP_NODEICONS[node]) << uint32((m_Nodes[node] == 0)?1:0);

    // Node occupied states
    for (uint8 node = 0; node < BG_AB_DYNAMIC_NODES_COUNT; ++node)
        for (uint8 i = 1; i < BG_AB_DYNAMIC_NODES_COUNT; ++i)
            data << uint32(BG_AB_OP_NODESTATES[node] + plusArray[i]) << uint32((m_Nodes[node] == i)?1:0);

    // How many bases each team owns
    uint8 ally = 0, horde = 0;
    for (uint8 node = 0; node < BG_AB_DYNAMIC_NODES_COUNT; ++node)
        if (m_Nodes[node] == BG_AB_NODE_STATUS_ALLY_OCCUPIED)
            ++ally;
        else if (m_Nodes[node] == BG_AB_NODE_STATUS_HORDE_OCCUPIED)
            ++horde;

    data << uint32(BG_AB_OP_OCCUPIED_BASES_ALLY)  << uint32(ally);
    data << uint32(BG_AB_OP_OCCUPIED_BASES_HORDE) << uint32(horde);

    // Team scores
    data << uint32(BG_AB_OP_RESOURCES_MAX)      << uint32(BG_AB_MAX_TEAM_SCORE);
    data << uint32(BG_AB_OP_RESOURCES_WARNING)  << uint32(BG_AB_WARNING_NEAR_VICTORY_SCORE);
    data << uint32(BG_AB_OP_RESOURCES_ALLY)     << uint32(m_TeamScores[BG_TEAM_ALLIANCE]);
    data << uint32(BG_AB_OP_RESOURCES_HORDE)    << uint32(m_TeamScores[BG_TEAM_HORDE]);

    // Banner world states
    for (int obj = BG_AB_OBJECT_BANNER; obj < BG_AB_DYNAMIC_NODES_COUNT; ++obj)
    {
        GameObject* l_Banner = GetBGObject(obj);
        if (l_Banner == nullptr)
            continue;

        data << uint32(l_Banner->GetGOInfo()->capturePoint.worldState1) << uint32(m_BannerWorldState[obj]);
    }

    // other unknown
    data << uint32(0x745) << uint32(0x2);           // 37 1861 unk
}

void BattlegroundAB::_SendNodeUpdate(uint8 node)
{
    // Send node owner state update to refresh map icons on client
    const uint8 plusArray[] = {0, 2, 3, 0, 1};

    if (m_prevNodes[node])
        UpdateWorldState(BG_AB_OP_NODESTATES[node] + plusArray[m_prevNodes[node]], 0);
    else
        UpdateWorldState(BG_AB_OP_NODEICONS[node], 0);

    UpdateWorldState(BG_AB_OP_NODESTATES[node] + plusArray[m_Nodes[node]], 1);

    // How many bases each team owns
    uint8 ally = 0, horde = 0;
    for (uint8 i = 0; i < BG_AB_DYNAMIC_NODES_COUNT; ++i)
        if (m_Nodes[i] == BG_AB_NODE_STATUS_ALLY_OCCUPIED)
            ++ally;
        else if (m_Nodes[i] == BG_AB_NODE_STATUS_HORDE_OCCUPIED)
            ++horde;

    UpdateWorldState(BG_AB_OP_OCCUPIED_BASES_ALLY, ally);
    UpdateWorldState(BG_AB_OP_OCCUPIED_BASES_HORDE, horde);
}

void BattlegroundAB::_NodeOccupied(uint8 node, Team team)
{
    if (node >= BG_AB_DYNAMIC_NODES_COUNT)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundAB::_NodeOccupied node(%u) > GILNEAS_BG_DYNAMIC_NODES_COUNT(%u)", node, BG_AB_DYNAMIC_NODES_COUNT);
        return;
    }

    if (!AddSpiritGuide(node, BG_AB_SpiritGuidePos[node][0], BG_AB_SpiritGuidePos[node][1], BG_AB_SpiritGuidePos[node][2], BG_AB_SpiritGuidePos[node][3], team))
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Failed to spawn spirit guide! point: %u, team: %u, ", node, team);

    uint8 capturedNodes = 0;
    for (uint8 i = 0; i < BG_AB_DYNAMIC_NODES_COUNT; ++i)
    {
        if (m_Nodes[node] == GetTeamIndexByTeamId(team) + BG_AB_NODE_TYPE_OCCUPIED && !m_NodeTimers[i])
            ++capturedNodes;
    }

    if (capturedNodes >= 5)
        CastSpellOnTeam(SPELL_AB_QUEST_REWARD_5_BASES, team);
    if (capturedNodes >= 4)
        CastSpellOnTeam(SPELL_AB_QUEST_REWARD_4_BASES, team);

    if (node >= BG_AB_DYNAMIC_NODES_COUNT)//only dynamic nodes, no start points
        return;

    Creature* trigger = BgCreatures[node+7] ? GetBGCreature(node+7) : NULL;//0-6 spirit guides
    if (!trigger)
        trigger = AddCreature(WORLD_TRIGGER, node+7, team, BG_AB_NodePositions[node][0], BG_AB_NodePositions[node][1], BG_AB_NodePositions[node][2], BG_AB_NodePositions[node][3]);

    //add bonus honor aura trigger creature when node is accupied
    //cast bonus aura (+50% honor in 25yards)
    //aura should only apply to players who have accupied the node, set correct faction for trigger
    if (trigger)
    {
        trigger->setFaction(team == ALLIANCE ? 84 : 83);
        trigger->CastSpell(trigger, SPELL_HONORABLE_DEFENDER_25Y, false);
    }
}

void BattlegroundAB::_NodeDeOccupied(uint8 node)
{
    if (node >= BG_AB_DYNAMIC_NODES_COUNT)
        return;

    //remove bonus honor aura trigger creature when node is lost
    if (node < BG_AB_DYNAMIC_NODES_COUNT)//only dynamic nodes, no start points
        DelCreature(node+7);//NULL checks are in DelCreature! 0-6 spirit guides

    RelocateDeadPlayers(BgCreatures[node]);

    if (BgCreatures[node])
        DelCreature(node);

    // buff object isn't despawned
}

/* Invoked if a player used a banner as a gameobject */
void BattlegroundAB::EventPlayerClickedOnFlag(Player* source, GameObject* /*target_obj*/)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint8 node = BG_AB_NODE_STABLES;
    GameObject* obj = GetBgMap()->GetGameObject(BgObjects[node]);

    while ((node < BG_AB_DYNAMIC_NODES_COUNT) && ((!obj) || (!source->IsWithinDistInMap(obj, 10))))
    {
        ++node;
        obj = GetBgMap()->GetGameObject(BgObjects[node]);
    }

    if (node == BG_AB_DYNAMIC_NODES_COUNT)
    {
        // this means our player isn't close to any of banners - maybe cheater ??
        return;
    }

    BattlegroundTeamId teamIndex = GetTeamIndexByTeamId(source->GetBGTeam());

    // Check if player really could use this banner, not cheated
    if (!(m_Nodes[node] == 0 || teamIndex == m_Nodes[node] % 2))
        return;

    source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
    uint32 sound = 0;

    // If node is neutral, change to contested
    if (m_Nodes[node] == BG_AB_NODE_TYPE_NEUTRAL)
    {
        UpdatePlayerScore(source, nullptr, SCORE_BASES_ASSAULTED, 1);
        m_prevNodes[node] = m_Nodes[node];
        m_Nodes[node] = teamIndex + 1;

        // create new contested banner
        _ChangeBanner(node, BG_AB_NODE_TYPE_CONTESTED, teamIndex, true);
        _SendNodeUpdate(node);
        m_NodeTimers[node] = BG_AB_FLAG_CAPTURING_TIME;

        // FIXME: team and node names not localized
        if (teamIndex == 0)
            SendMessage2ToAll(LANG_BG_AB_NODE_CLAIMED, CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node), LANG_BG_AB_ALLY);
        else
            SendMessage2ToAll(LANG_BG_AB_NODE_CLAIMED, CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node), LANG_BG_AB_HORDE);

        sound = BG_AB_SOUND_NODE_CLAIMED;
    }
    // If node is contested
    else if ((m_Nodes[node] == BG_AB_NODE_STATUS_ALLY_CONTESTED) || (m_Nodes[node] == BG_AB_NODE_STATUS_HORDE_CONTESTED))
    {
        // If last state is NOT occupied, change node to enemy-contested
        if (m_prevNodes[node] < BG_AB_NODE_TYPE_OCCUPIED)
        {
            UpdatePlayerScore(source, nullptr, SCORE_BASES_ASSAULTED, 1);
            m_prevNodes[node] = m_Nodes[node];
            m_Nodes[node] = teamIndex + BG_AB_NODE_TYPE_CONTESTED;
            // create new contested banner
            _ChangeBanner(node, BG_AB_NODE_TYPE_CONTESTED, teamIndex, true);
            _SendNodeUpdate(node);
            m_NodeTimers[node] = BG_AB_FLAG_CAPTURING_TIME;

            // FIXME: node names not localized
            if (teamIndex == BG_TEAM_ALLIANCE)
                SendMessage2ToAll(LANG_BG_AB_NODE_ASSAULTED, CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node));
            else
                SendMessage2ToAll(LANG_BG_AB_NODE_ASSAULTED, CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node));
        }
        // If contested, change back to occupied
        else
        {
            UpdatePlayerScore(source, nullptr, SCORE_BASES_DEFENDED, 1);
            m_prevNodes[node] = m_Nodes[node];
            m_Nodes[node] = teamIndex + BG_AB_NODE_TYPE_OCCUPIED;
            // create new occupied banner
            _ChangeBanner(node, BG_AB_NODE_TYPE_OCCUPIED, teamIndex, true);
            _SendNodeUpdate(node);
            m_NodeTimers[node] = 0;
            _NodeOccupied(node, (teamIndex == BG_TEAM_ALLIANCE) ? ALLIANCE:HORDE);

            // FIXME: node names not localized
            if (teamIndex == BG_TEAM_ALLIANCE)
                SendMessage2ToAll(LANG_BG_AB_NODE_DEFENDED, CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node));
            else
                SendMessage2ToAll(LANG_BG_AB_NODE_DEFENDED, CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node));
        }
        sound = (teamIndex == BG_TEAM_ALLIANCE) ? BG_AB_SOUND_NODE_ASSAULTED_ALLIANCE : BG_AB_SOUND_NODE_ASSAULTED_HORDE;
    }
    // If node is occupied, change to enemy-contested
    else
    {
        UpdatePlayerScore(source, nullptr, SCORE_BASES_ASSAULTED, 1);
        m_prevNodes[node] = m_Nodes[node];
        m_Nodes[node] = teamIndex + BG_AB_NODE_TYPE_CONTESTED;
        // create new contested banner
        _ChangeBanner(node, BG_AB_NODE_TYPE_CONTESTED, teamIndex, true);
        _SendNodeUpdate(node);
        _NodeDeOccupied(node);
        m_NodeTimers[node] = BG_AB_FLAG_CAPTURING_TIME;

        // FIXME: node names not localized
        if (teamIndex == BG_TEAM_ALLIANCE)
            SendMessage2ToAll(LANG_BG_AB_NODE_ASSAULTED, CHAT_MSG_BG_SYSTEM_ALLIANCE, source, _GetNodeNameId(node));
        else
            SendMessage2ToAll(LANG_BG_AB_NODE_ASSAULTED, CHAT_MSG_BG_SYSTEM_HORDE, source, _GetNodeNameId(node));

        sound = (teamIndex == BG_TEAM_ALLIANCE) ? BG_AB_SOUND_NODE_ASSAULTED_ALLIANCE : BG_AB_SOUND_NODE_ASSAULTED_HORDE;
    }

    // If node is occupied again, send "X has taken the Y" msg.
    if (m_Nodes[node] >= BG_AB_NODE_TYPE_OCCUPIED)
    {
        // FIXME: team and node names not localized
        if (teamIndex == BG_TEAM_ALLIANCE)
            SendMessage2ToAll(LANG_BG_AB_NODE_TAKEN, CHAT_MSG_BG_SYSTEM_ALLIANCE, NULL, LANG_BG_AB_ALLY, _GetNodeNameId(node));
        else
            SendMessage2ToAll(LANG_BG_AB_NODE_TAKEN, CHAT_MSG_BG_SYSTEM_HORDE, NULL, LANG_BG_AB_HORDE, _GetNodeNameId(node));
    }
    PlaySoundToAll(sound);
}

bool BattlegroundAB::SetupBattleground()
{
    if (!AddObject(BG_AB_OBJECT_BANNER, BG_AB_OBJECTID_NODE_BANNER_0, BG_AB_NodePositions[0][0], BG_AB_NodePositions[0][1], BG_AB_NodePositions[0][2], BG_AB_NodePositions[0][3], 0, 0, std::sin(BG_AB_NodePositions[0][3]/2), std::cos(BG_AB_NodePositions[0][3]/2), RESPAWN_ONE_DAY)
        || !AddObject(BG_AB_OBJECT_BANNER + 1, BG_AB_OBJECTID_NODE_BANNER_1, BG_AB_NodePositions[1][0], BG_AB_NodePositions[1][1], BG_AB_NodePositions[1][2], BG_AB_NodePositions[1][3], 0, 0, std::sin(BG_AB_NodePositions[1][3] / 2), std::cos(BG_AB_NodePositions[1][3] / 2), RESPAWN_ONE_DAY)
        || !AddObject(BG_AB_OBJECT_BANNER + 2, BG_AB_OBJECTID_NODE_BANNER_2, BG_AB_NodePositions[2][0], BG_AB_NodePositions[2][1], BG_AB_NodePositions[2][2], BG_AB_NodePositions[2][3], 0, 0, std::sin(BG_AB_NodePositions[2][3] / 2), std::cos(BG_AB_NodePositions[2][3] / 2), RESPAWN_ONE_DAY)
        || !AddObject(BG_AB_OBJECT_BANNER + 3, BG_AB_OBJECTID_NODE_BANNER_3, BG_AB_NodePositions[3][0], BG_AB_NodePositions[3][1], BG_AB_NodePositions[3][2], BG_AB_NodePositions[3][3], 0, 0, std::sin(BG_AB_NodePositions[3][3] / 2), std::cos(BG_AB_NodePositions[3][3] / 2), RESPAWN_ONE_DAY)
        || !AddObject(BG_AB_OBJECT_BANNER + 4, BG_AB_OBJECTID_NODE_BANNER_4, BG_AB_NodePositions[4][0], BG_AB_NodePositions[4][1], BG_AB_NodePositions[4][2], BG_AB_NodePositions[4][3], 0, 0, std::sin(BG_AB_NodePositions[4][3] / 2), std::cos(BG_AB_NodePositions[4][3] / 2), RESPAWN_ONE_DAY))
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundAB: Failed to spawn some object Battleground not created!");
        return false;
    }

    if (!AddObject(BG_AB_OBJECT_GATE_A, BG_AB_OBJECTID_GATE_A, BG_AB_DoorPositions[0][0], BG_AB_DoorPositions[0][1], BG_AB_DoorPositions[0][2], BG_AB_DoorPositions[0][3], BG_AB_DoorPositions[0][4], BG_AB_DoorPositions[0][5], BG_AB_DoorPositions[0][6], BG_AB_DoorPositions[0][7], RESPAWN_IMMEDIATELY)
        || !AddObject(BG_AB_OBJECT_GATE_H, BG_AB_OBJECTID_GATE_H, BG_AB_DoorPositions[1][0], BG_AB_DoorPositions[1][1], BG_AB_DoorPositions[1][2], BG_AB_DoorPositions[1][3], BG_AB_DoorPositions[1][4], BG_AB_DoorPositions[1][5], BG_AB_DoorPositions[1][6], BG_AB_DoorPositions[1][7], RESPAWN_IMMEDIATELY))
    {
        sLog->outError(LOG_FILTER_SQL, "BatteGroundAB: Failed to spawn door object Battleground not created!");
        return false;
    }
    //buffs
    for (int i = 0; i < BG_AB_DYNAMIC_NODES_COUNT; ++i)
    {
        if (!AddObject(BG_AB_OBJECT_SPEEDBUFF_STABLES + 3 * i, Buff_Entries[0], BG_AB_BuffPositions[i][0], BG_AB_BuffPositions[i][1], BG_AB_BuffPositions[i][2], BG_AB_BuffPositions[i][3], 0, 0, std::sin(BG_AB_BuffPositions[i][3] / 2), std::cos(BG_AB_BuffPositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(BG_AB_OBJECT_SPEEDBUFF_STABLES + 3 * i + 1, Buff_Entries[1], BG_AB_BuffPositions[i][0], BG_AB_BuffPositions[i][1], BG_AB_BuffPositions[i][2], BG_AB_BuffPositions[i][3], 0, 0, std::sin(BG_AB_BuffPositions[i][3] / 2), std::cos(BG_AB_BuffPositions[i][3] / 2), RESPAWN_ONE_DAY)
            || !AddObject(BG_AB_OBJECT_SPEEDBUFF_STABLES + 3 * i + 2, Buff_Entries[2], BG_AB_BuffPositions[i][0], BG_AB_BuffPositions[i][1], BG_AB_BuffPositions[i][2], BG_AB_BuffPositions[i][3], 0, 0, std::sin(BG_AB_BuffPositions[i][3] / 2), std::cos(BG_AB_BuffPositions[i][3] / 2), RESPAWN_ONE_DAY))
        {
            sLog->outError(LOG_FILTER_SQL, "BatteGroundAB: Failed to spawn buff object!");
            return false;
        }
    }

    for (uint8 l_I = BG_AB_SPIRIT_ALLIANCE; l_I <= BG_AB_SPIRIT_HORDE; ++l_I)
    {
        if (!AddSpiritGuide(l_I, BG_AB_SpiritGuidePos[l_I][0], BG_AB_SpiritGuidePos[l_I][1], BG_AB_SpiritGuidePos[l_I][2], BG_AB_SpiritGuidePos[l_I][3], l_I == BG_AB_SPIRIT_ALLIANCE ? Team::ALLIANCE : Team::HORDE))
        {
            sLog->outError(LOG_FILTER_SQL, "BatteGroundAB: Failed to spawn spirit guide, Battleground not created!");
            return false;
        }
    }

    return true;
}

void BattlegroundAB::Reset()
{
    //call parent's class reset
    Battleground::Reset();

    m_TeamScores[BG_TEAM_ALLIANCE]          = 0;
    m_TeamScores[BG_TEAM_HORDE]             = 0;
    m_lastTick[BG_TEAM_ALLIANCE]            = 0;
    m_lastTick[BG_TEAM_HORDE]               = 0;
    m_HonorScoreTics[BG_TEAM_ALLIANCE]      = 0;
    m_HonorScoreTics[BG_TEAM_HORDE]         = 0;
    m_ReputationScoreTics[BG_TEAM_ALLIANCE] = 0;
    m_ReputationScoreTics[BG_TEAM_HORDE]    = 0;
    m_CheatersCheckTimer = 0;
    m_IsInformedNearVictory                 = false;
    bool isBGWeekend = sBattlegroundMgr->IsBGWeekend(GetTypeID());
    m_HonorTics = (isBGWeekend) ? BG_AB_ABBGWeekendHonorTicks : BG_AB_NotABBGWeekendHonorTicks;
    m_ReputationTics = (isBGWeekend) ? BG_AB_ABBGWeekendReputationTicks : BG_AB_NotABBGWeekendReputationTicks;
    m_TeamScores500Disadvantage[BG_TEAM_ALLIANCE] = false;
    m_TeamScores500Disadvantage[BG_TEAM_HORDE]    = false;

    for (uint8 i = 0; i < BG_AB_DYNAMIC_NODES_COUNT; ++i)
    {
        m_Nodes[i] = 0;
        m_prevNodes[i] = 0;
        m_NodeTimers[i] = 0;
    }

    for (uint8 i = 0; i < BG_AB_ALL_NODES_COUNT + 5; ++i)//+5 for aura triggers
        if (BgCreatures[i])
            DelCreature(i);
}

void BattlegroundAB::EndBattleground(uint32 winner)
{
    //win reward
    if (winner == ALLIANCE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);
    if (winner == HORDE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);
    //complete map_end rewards (even if no team wins)
    RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);
    RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);

    Battleground::EndBattleground(winner);
}

WorldSafeLocsEntry const* BattlegroundAB::GetClosestGraveYard(Player* player)
{
    BattlegroundTeamId teamIndex = GetTeamIndexByTeamId(player->GetBGTeam());

    // Is there any occupied node for this team?
    std::vector<uint8> nodes;
    for (uint8 i = 0; i < BG_AB_DYNAMIC_NODES_COUNT; ++i)
        if (m_Nodes[i] == teamIndex + 3)
            nodes.push_back(i);

    WorldSafeLocsEntry const* good_entry = NULL;
    // If so, select the closest node to place ghost on
    if (!nodes.empty())
    {
        float plr_x = player->GetPositionX();
        float plr_y = player->GetPositionY();

        float mindist = 999999.0f;
        for (uint8 i = 0; i < nodes.size(); ++i)
        {
            WorldSafeLocsEntry const*entry = sWorldSafeLocsStore.LookupEntry(BG_AB_GraveyardIds[nodes[i]]);
            if (!entry)
                continue;
            float dist = (entry->x - plr_x)*(entry->x - plr_x)+(entry->y - plr_y)*(entry->y - plr_y);
            if (mindist > dist)
            {
                mindist = dist;
                good_entry = entry;
            }
        }
        nodes.clear();
    }
    // If not, place ghost on starting location
    if (!good_entry)
        good_entry = sWorldSafeLocsStore.LookupEntry(BG_AB_GraveyardIds[teamIndex+5]);

    return good_entry;
}

void BattlegroundAB::UpdatePlayerScore(Player* p_Source, Player* p_Victim, uint32 p_Type, uint32 p_Value, bool p_DoAddHonor, MS::Battlegrounds::RewardCurrencyType::Type p_RewardType)
{
    BattlegroundScoreMap::iterator itr = PlayerScores.find(p_Source->GetGUID());
    if (itr == PlayerScores.end())                         // player not found...
        return;

    switch (p_Type)
    {
        case SCORE_BASES_ASSAULTED:
            ((BattlegroundABScore*)itr->second)->BasesAssaulted += p_Value;
            p_Source->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, AB_OBJECTIVE_ASSAULT_BASE);
            break;
        case SCORE_BASES_DEFENDED:
            ((BattlegroundABScore*)itr->second)->BasesDefended += p_Value;
            p_Source->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, AB_OBJECTIVE_DEFEND_BASE);
            break;
        default:
            Battleground::UpdatePlayerScore(p_Source, p_Victim, p_Type, p_Value, p_DoAddHonor, p_RewardType);
            break;
    }
}

bool BattlegroundAB::IsAllNodesControlledByTeam(uint32 team) const
{
    uint32 count = 0;
    for (int i = 0; i < BG_AB_DYNAMIC_NODES_COUNT; ++i)
        if ((team == ALLIANCE && m_Nodes[i] == BG_AB_NODE_STATUS_ALLY_OCCUPIED) ||
            (team == HORDE    && m_Nodes[i] == BG_AB_NODE_STATUS_HORDE_OCCUPIED))
            ++count;

    return count == BG_AB_DYNAMIC_NODES_COUNT;
}
