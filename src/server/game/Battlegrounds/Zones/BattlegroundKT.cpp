////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Object.h"
#include "Player.h"
#include "BattlegroundKT.h"
#include "Creature.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "BattlegroundMgr.hpp"
#include "WorldPacket.h"
#include "Language.h"
#include "MapManager.h"

BattlegroundKT::BattlegroundKT()
{
    StartMessageIds[BG_STARTING_EVENT_FIRST]  = 0;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_KT_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_KT_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_KT_HAS_BEGUN;

    m_ReputationCapture = 0;
    m_HonorWinKills = 0;
    m_HonorEndKills = 0;

    m_UpdatePointsTimer = BG_KT_POINTS_UPDATE_TIME;
    m_LastCapturedOrbTeam = TEAM_NONE;

    for (uint32 i = 0; i < MAX_ORBS; ++i)
        m_OrbKeepers[i] = 0;
}

BattlegroundKT::~BattlegroundKT()
{
}

void BattlegroundKT::PostUpdateImpl(uint32 diff)
{
    if (GetStatus() == STATUS_WAIT_JOIN)
    {
        m_CheatersCheckTimer -= diff;
        if (m_CheatersCheckTimer <= 0)
        {
            for (auto itr : GetPlayers())
            {
                Player * plr = ObjectAccessor::FindPlayer(itr.first);
                if (!plr || !plr->IsInWorld())
                    continue;

                if (plr->GetPositionZ() < 24)
                {
                    if (plr->GetBGTeam() == HORDE)
                        plr->TeleportTo(998, 1781.31f, 1597.76f, 33.61f, plr->GetOrientation(), 0);
                    else
                        plr->TeleportTo(998, 1784.42f, 1072.73f, 29.88f, plr->GetOrientation(), 0);
                }
            }
            m_CheatersCheckTimer = 4000;
        }
    }

    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        if (m_UpdatePointsTimer <= diff)
        {
            for (uint8 i = 0; i < MAX_ORBS; ++i)
            {
                if (uint64 guid = m_OrbKeepers[i])
                {
                    if (m_playersZone.find(guid) != m_playersZone.end())
                    {
                        if (Player* player = ObjectAccessor::FindPlayer(guid))
                        {
                            AccumulateScore(player->GetBGTeam() == ALLIANCE ? BG_TEAM_ALLIANCE : BG_TEAM_HORDE, m_playersZone[guid]);
                            UpdatePlayerScore(player, nullptr, SCORE_ORB_SCORE, BG_KT_TickPoints[m_playersZone[guid]]);
                        }
                    }
                }
            }

            m_UpdatePointsTimer = BG_KT_POINTS_UPDATE_TIME;
        }
        else
            m_UpdatePointsTimer -= diff;
    }
}

void BattlegroundKT::StartingEventCloseDoors()
{
    SpawnBGObject(BG_KT_OBJECT_A_DOOR, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_KT_OBJECT_H_DOOR, RESPAWN_IMMEDIATELY);

    DoorClose(BG_KT_OBJECT_A_DOOR);
    DoorClose(BG_KT_OBJECT_H_DOOR);

    for (uint8 i = 0; i < 4; ++i)
        SpawnBGObject(BG_KT_OBJECT_ORB_1 + i, RESPAWN_ONE_DAY);
}

void BattlegroundKT::StartingEventOpenDoors()
{
    DoorOpen(BG_KT_OBJECT_A_DOOR);
    DoorOpen(BG_KT_OBJECT_H_DOOR);

    for (uint8 i = 0; i < 4; ++i)
        SpawnBGObject(BG_KT_OBJECT_ORB_1 + i, RESPAWN_IMMEDIATELY);

    // Players that join battleground after start are not eligible to get achievement.
    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, BG_KT_EVENT_START_BATTLE);
}

void BattlegroundKT::AddPlayer(Player *plr)
{
    Battleground::AddPlayer(plr);
    //create score and add it to map, default values are set in constructor
    BattleGroundKTScore* sc = new BattleGroundKTScore;

    PlayerScores[plr->GetGUID()] = sc;
    m_playersZone[plr->GetGUID()] = KT_ZONE_OUT;
}

std::string GetOrbString(uint8 p_Index)
{
    uint32 l_SpellId = BG_KT_ORBS_SPELLS[p_Index];

    std::ostringstream l_Stream;
    l_Stream << "|c" << s_OrbColor[p_Index] << "|Hspell:" << l_SpellId << "|h[" << sSpellMgr->GetSpellInfo(l_SpellId)->SpellName << "]|h|r";

    return l_Stream.str();
}

void BattlegroundKT::EventPlayerClickedOnOrb(Player* source, GameObject* target_obj)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    if (!source->IsWithinDistInMap(target_obj, 10))
        return;

    uint32 index = target_obj->GetEntry() - BG_KT_OBJECT_ORB_1_ENTRY;

    // If this orb is already keeped by a player, there is a problem
    if (index >= MAX_ORBS || m_OrbKeepers[index] != 0)
        return;

    // Check if the player already have an orb
    for (uint8 i = 0; i < MAX_ORBS; ++i)
        if (m_OrbKeepers[i] == source->GetGUID())
            return;

    PlaySoundToAll(source->GetBGTeam() == ALLIANCE ? BG_KT_SOUND_A_ORB_PICKED_UP: BG_KT_SOUND_H_ORB_PICKED_UP);
    source->CastSpell(source, BG_KT_ORBS_SPELLS[index], true);
    source->CastSpell(source, source->GetBGTeam() == ALLIANCE ? BG_KT_ALLIANCE_INSIGNIA: BG_KT_HORDE_INSIGNIA, true);

    UpdatePlayerScore(source, nullptr, SCORE_ORB_HANDLES, 1);

    m_OrbKeepers[index] = source->GetGUID();
    UpdateWorldState(s_OrbsWorldStates[index], 0);
    UpdateWorldState(BG_KT_ICON_A, 1);
    SpawnBGObject(BG_KT_OBJECT_ORB_1 + index, RESPAWN_ONE_DAY);

    if (Creature* aura = GetBGCreature(BG_KT_CREATURE_ORB_AURA_1 + index))
        aura->RemoveAllAuras();

    PSendMessageToAll(LANG_BG_KT_PICKEDUP, source->GetBGTeam() == ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE, source, GetOrbString(index).c_str());
    source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
}

void BattlegroundKT::EventPlayerDroppedOrb(Player* source)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint8 index = 0;

    for (; index <= MAX_ORBS; ++index)
    {
        if (index == MAX_ORBS)
            return;

        if (m_OrbKeepers[index] == source->GetGUID())
            break;
    }

    PlaySoundToAll(source->GetBGTeam() == ALLIANCE ? BG_KT_SOUND_A_ORB_PICKED_UP: BG_KT_SOUND_H_ORB_PICKED_UP);
    source->RemoveAurasDueToSpell(BG_KT_ORBS_SPELLS[index]);
    source->RemoveAurasDueToSpell(BG_KT_ALLIANCE_INSIGNIA);
    source->RemoveAurasDueToSpell(BG_KT_HORDE_INSIGNIA);

    m_OrbKeepers[index] = 0;
    UpdateWorldState(s_OrbsWorldStates[index], 1);

    SpawnBGObject(BG_KT_OBJECT_ORB_1 + index, RESPAWN_IMMEDIATELY);

    if (Creature* aura = GetBGCreature(BG_KT_CREATURE_ORB_AURA_1 + index))
        aura->AddAura(BG_KT_ORBS_AURA[index], aura);

    UpdateWorldState(BG_KT_ICON_A, 0);

    PSendMessageToAll(LANG_BG_KT_DROPPED, source->GetBGTeam() == ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE, source, GetOrbString(index).c_str());
    source->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
}

void BattlegroundKT::RemovePlayer(Player* plr, uint64 /*guid*/, uint32 /*team*/)
{
    if (plr == nullptr)
        return;

    EventPlayerDroppedOrb(plr);
    m_playersZone.erase(plr->GetGUID());
}

void BattlegroundKT::UpdateOrbState(Team team, uint32 value)
{
    if (team == ALLIANCE)
        UpdateWorldState(BG_KT_ICON_A, value);
    else
        UpdateWorldState(BG_KT_ICON_H, value);
}

void BattlegroundKT::UpdateTeamScore(Team team)
{
    if (team == ALLIANCE)
        UpdateWorldState(BG_KT_ORB_POINTS_A, GetTeamScore(team));
    else
        UpdateWorldState(BG_KT_ORB_POINTS_H, GetTeamScore(team));
}

void BattlegroundKT::HandleAreaTrigger(Player* source, uint32 trigger)
{
    // this is wrong way to implement these things. On official it done by gameobject spell cast.
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint64 sourceGuid = source->GetGUID();
    switch(trigger)
    {
        case 7734: // Out-In trigger
        {
            if (m_playersZone.find(sourceGuid) == m_playersZone.end())
                return;

            if (m_playersZone[sourceGuid] == KT_ZONE_OUT)
                m_playersZone[sourceGuid] = KT_ZONE_IN;
            else
                m_playersZone[sourceGuid] = KT_ZONE_OUT;
            break;
        }
        case 7735: // Middle-In trigger
        {
            if (m_playersZone.find(sourceGuid) == m_playersZone.end())
                return;

            if (m_playersZone[sourceGuid] == KT_ZONE_IN)
                m_playersZone[sourceGuid] = KT_ZONE_MIDDLE;
            else
                m_playersZone[sourceGuid] = KT_ZONE_IN;
            break;
        }
        default:
            break;
    }
}

bool BattlegroundKT::SetupBattleground()
{
    // Doors
    if (   !AddObject(BG_KT_OBJECT_A_DOOR, BG_KT_OBJECT_DOOR_ENTRY, BG_KT_DoorPositions[0][0], BG_KT_DoorPositions[0][1], BG_KT_DoorPositions[0][2], BG_KT_DoorPositions[0][3], 0, 0, sin(BG_KT_DoorPositions[0][3]/2), cos(BG_KT_DoorPositions[0][3]/2), RESPAWN_IMMEDIATELY)
        || !AddObject(BG_KT_OBJECT_H_DOOR, BG_KT_OBJECT_DOOR_ENTRY, BG_KT_DoorPositions[1][0], BG_KT_DoorPositions[1][1], BG_KT_DoorPositions[1][2], BG_KT_DoorPositions[1][3], 0, 0, sin(BG_KT_DoorPositions[1][3]/2), cos(BG_KT_DoorPositions[1][3]/2), RESPAWN_IMMEDIATELY))
        return false;

    if (   !AddSpiritGuide(BG_KT_CREATURE_SPIRIT_1, BG_KT_SpiritPositions[0][0], BG_KT_SpiritPositions[0][1], BG_KT_SpiritPositions[0][2], BG_KT_SpiritPositions[0][3], ALLIANCE)
        || !AddSpiritGuide(BG_KT_CREATURE_SPIRIT_2, BG_KT_SpiritPositions[1][0], BG_KT_SpiritPositions[1][1], BG_KT_SpiritPositions[1][2], BG_KT_SpiritPositions[1][3], HORDE))
        return false;

    // Orbs
    for (uint8 i = 0; i < MAX_ORBS; ++i)
    {
        if (!AddObject(BG_KT_OBJECT_ORB_1 + i, BG_KT_OBJECT_ORB_1_ENTRY + i, BG_KT_OrbPositions[i][0], BG_KT_OrbPositions[i][1], BG_KT_OrbPositions[i][2], BG_KT_OrbPositions[i][3], 0, 0, sin(BG_KT_OrbPositions[i][3]/2), cos(BG_KT_OrbPositions[i][3]/2), RESPAWN_ONE_DAY))
            return false;

        if (Creature* trigger = AddCreature(WORLD_TRIGGER, BG_KT_CREATURE_ORB_AURA_1 + i, TEAM_NEUTRAL, BG_KT_OrbPositions[i][0], BG_KT_OrbPositions[i][1], BG_KT_OrbPositions[i][2], BG_KT_OrbPositions[i][3], RESPAWN_IMMEDIATELY))
            trigger->AddAura(BG_KT_ORBS_AURA[i], trigger);
    }

    return true;
}

void BattlegroundKT::Reset()
{
    //call parent's class reset
    Battleground::Reset();
    BgObjects.resize(BG_KT_OBJECT_MAX);
    BgCreatures.resize(BG_KT_CREATURE_MAX);

    for (uint32 i = 0; i < MAX_ORBS; ++i)
        m_OrbKeepers[i] = 0;

    bool isBGWeekend = MS::Battlegrounds::BattlegroundMgr::IsBGWeekend(GetTypeID());
    m_ReputationCapture = (isBGWeekend) ? 45 : 35;
    m_HonorWinKills = (isBGWeekend) ? 3 : 1;
    m_HonorEndKills = (isBGWeekend) ? 4 : 2;

    m_LastCapturedOrbTeam = TEAM_NONE;

    m_CheatersCheckTimer = 0;
}

void BattlegroundKT::EndBattleground(uint32 winner)
{
    //win reward
    if (winner == ALLIANCE)
        RewardHonorToTeam(GetBonusHonorFromKill(m_HonorWinKills), ALLIANCE);
    if (winner == HORDE)
        RewardHonorToTeam(GetBonusHonorFromKill(m_HonorWinKills), HORDE);
    //complete map_end rewards (even if no team wins)
    RewardHonorToTeam(GetBonusHonorFromKill(m_HonorEndKills), ALLIANCE);
    RewardHonorToTeam(GetBonusHonorFromKill(m_HonorEndKills), HORDE);

    Battleground::EndBattleground(winner);
}

void BattlegroundKT::HandleKillPlayer(Player *player, Player *killer)
{
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    EventPlayerDroppedOrb(player);

    Battleground::HandleKillPlayer(player, killer);
}

void BattlegroundKT::UpdatePlayerScore(Player* p_Source, Player* p_Victim, uint32 p_Type, uint32 p_Value, bool p_DoAddHonor, MS::Battlegrounds::RewardCurrencyType::Type p_RewardType)
{
    BattlegroundScoreMap::iterator itr = PlayerScores.find(p_Source->GetObjectGuid());
    if (itr == PlayerScores.end())                         // player not found
        return;

    switch (p_Type)
    {
        case SCORE_ORB_HANDLES:                           // orb handles
            ((BattleGroundKTScore*)itr->second)->OrbHandles += p_Value;
            break;
        case SCORE_ORB_SCORE:
            ((BattleGroundKTScore*)itr->second)->Score += p_Value;
            break;
        default:
            Battleground::UpdatePlayerScore(p_Source, p_Victim, p_Type, p_Value, p_DoAddHonor, p_RewardType);
            break;
    }
}

WorldSafeLocsEntry const* BattlegroundKT::GetClosestGraveYard(Player* player)
{
    //if status in progress, it returns main graveyards with spiritguides
    //else it will return the graveyard in the flagroom - this is especially good
    //if a player dies in preparation phase - then the player can't cheat
    //and teleport to the graveyard outside the flagroom
    //and start running around, while the doors are still closed
    if (player->GetBGTeam() == ALLIANCE)
    {
        if (GetStatus() == STATUS_IN_PROGRESS)
            return sWorldSafeLocsStore.LookupEntry(KT_GRAVEYARD_RECTANGLEA1);
        else
            return sWorldSafeLocsStore.LookupEntry(KT_GRAVEYARD_RECTANGLEA2);
    }
    else
    {
        if (GetStatus() == STATUS_IN_PROGRESS)
            return sWorldSafeLocsStore.LookupEntry(KT_GRAVEYARD_RECTANGLEH1);
        else
            return sWorldSafeLocsStore.LookupEntry(KT_GRAVEYARD_RECTANGLEH2);
    }
}

void BattlegroundKT::AccumulateScore(uint32 team, BG_KT_ZONE zone)
{
    if (zone > KT_ZONE_MAX)
        return;

    if (team >= TEAM_NEUTRAL)
        return;

    if (GetStatus() == STATUS_WAIT_LEAVE)
        return;

    m_TeamScores[team] += BG_KT_TickPoints[zone];

    if (m_TeamScores[team] > BG_KT_MAX_TEAM_SCORE)
        m_TeamScores[team] = BG_KT_MAX_TEAM_SCORE;

    if (team == BG_TEAM_ALLIANCE)
        UpdateWorldState(BG_KT_ORB_POINTS_A, m_TeamScores[team]);
    if (team == BG_TEAM_HORDE)
        UpdateWorldState(BG_KT_ORB_POINTS_H, m_TeamScores[team]);

    // Test win condition
    if (m_TeamScores[BG_TEAM_ALLIANCE] >= BG_KT_MAX_TEAM_SCORE)
        EndBattleground(ALLIANCE);
    else if (m_TeamScores[BG_TEAM_HORDE] >= BG_KT_MAX_TEAM_SCORE)
        EndBattleground(HORDE);
}

void BattlegroundKT::FillInitialWorldStates(ByteBuffer& p_Data)
{
    p_Data << uint32(BG_KT_ORB_POINTS_A)    << uint32(GetTeamScore(ALLIANCE));
    p_Data << uint32(BG_KT_ORB_POINTS_H)    << uint32(GetTeamScore(HORDE));
    p_Data << uint32(BG_KT_ORB_POINTS_MAX)  << uint32(BG_KT_MAX_TEAM_SCORE);

    for (int l_I = 0; l_I < MAX_ORBS; l_I++)
        p_Data << uint32(s_OrbsWorldStates[l_I])    << uint32(m_OrbKeepers[l_I] != 0 ? 0 : 1);
}
