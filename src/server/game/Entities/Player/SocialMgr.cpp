////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "SocialMgr.h"

#include "DatabaseEnv.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "World.h"
#include "Util.h"
#include "AccountMgr.h"

PlayerSocial::PlayerSocial()
{
    m_playerGUID = 0;
}

PlayerSocial::~PlayerSocial()
{
    m_playerSocialMap.clear();
}

uint32 PlayerSocial::GetNumberOfSocialsWithFlag(SocialFlag flag)
{
    uint32 counter = 0;
    for (PlayerSocialMap::const_iterator itr = m_playerSocialMap.begin(); itr != m_playerSocialMap.end(); ++itr)
        if (itr->second.Flags & flag)
            ++counter;

    return counter;
}

bool PlayerSocial::AddToSocialList(uint32 friendGuid, bool ignore)
{
#ifndef CROSS
    // check client limits
    if (ignore)
    {
        if (GetNumberOfSocialsWithFlag(SOCIAL_FLAG_IGNORED) >= SOCIALMGR_IGNORE_LIMIT)
            return false;
    }
    else
    {
        if (GetNumberOfSocialsWithFlag(SOCIAL_FLAG_FRIEND) >= SOCIALMGR_FRIEND_LIMIT)
            return false;
    }

    uint8 flag = SOCIAL_FLAG_FRIEND;
    if (ignore)
        flag = SOCIAL_FLAG_IGNORED;

    PlayerSocialMap::const_iterator itr = m_playerSocialMap.find(friendGuid);

    if (itr != m_playerSocialMap.end())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_CHARACTER_SOCIAL_FLAGS);

        stmt->setUInt8(0, flag);
        stmt->setUInt32(1, m_AccountID);
        stmt->setUInt32(2, friendGuid);

        CharacterDatabase.Execute(stmt);

        m_playerSocialMap[friendGuid].Flags |= flag;
    }
    else
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHARACTER_SOCIAL);

        stmt->setUInt32(0, m_AccountID);
        stmt->setUInt32(1, friendGuid);
        stmt->setUInt8(2, flag);

        CharacterDatabase.Execute(stmt);

        FriendInfo fi;
        fi.Flags |= flag;
        m_playerSocialMap[friendGuid] = fi;
    }

#endif
    return true;
}

void PlayerSocial::RemoveFromSocialList(uint32 friendGuid, bool ignore)
{
#ifndef CROSS
    PlayerSocialMap::iterator itr = m_playerSocialMap.find(friendGuid);
    if (itr == m_playerSocialMap.end())                     // not exist
        return;

    uint8 flag = SOCIAL_FLAG_FRIEND;
    if (ignore)
        flag = SOCIAL_FLAG_IGNORED;

    itr->second.Flags &= ~flag;
    if (itr->second.Flags == 0)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHARACTER_SOCIAL);

        stmt->setUInt32(0, m_AccountID);
        stmt->setUInt32(1, friendGuid);

        CharacterDatabase.Execute(stmt);

        m_playerSocialMap.erase(itr);
    }
    else
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_REM_CHARACTER_SOCIAL_FLAGS);

        stmt->setUInt8(0, flag);
        stmt->setUInt32(1, m_AccountID);
        stmt->setUInt32(2, friendGuid);

        CharacterDatabase.Execute(stmt);
    }
#endif
}

void PlayerSocial::SetFriendNote(uint32 friendGuid, std::string note)
{
#ifndef CROSS
    PlayerSocialMap::const_iterator itr = m_playerSocialMap.find(friendGuid);
    if (itr == m_playerSocialMap.end())                     // not exist
        return;

    utf8truncate(note, 48);                                  // DB and client size limitation

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHARACTER_SOCIAL_NOTE);

    stmt->setString(0, note);
    stmt->setUInt32(1, m_AccountID);
    stmt->setUInt32(2, friendGuid);

    CharacterDatabase.Execute(stmt);

    m_playerSocialMap[friendGuid].Note = note;
#endif
}

void PlayerSocial::SendSocialList(Player * p_Player)
{
    if (!p_Player)
        return;

    WorldPacket l_Data(SMSG_CONTACT_LIST, 4 + 1 + (m_playerSocialMap.size() * (16 + 2 + 16 + 2 + 4 + 4 + 4 + 1 + 4 + 4 + 4 + 2 + 100)));
    l_Data << uint32(7);                                                            ///< 0x1 = Friendlist update. 0x2 = Ignorelist update. 0x4 = Mutelist update.
    l_Data.WriteBits(m_playerSocialMap.size(), 8);                                  ///< Friends count
    l_Data.FlushBits();

    for (PlayerSocialMap::iterator itr = m_playerSocialMap.begin(); itr != m_playerSocialMap.end(); ++itr)
    {
        sSocialMgr->GetFriendInfo(p_Player, itr->first, itr->second);

        l_Data.appendPackGUID(MAKE_NEW_GUID(itr->first, 0, HIGHGUID_PLAYER));
        l_Data.appendPackGUID(MAKE_NEW_GUID(itr->first, 0, HIGHGUID_WOW_ACCOUNT));
        l_Data << uint32(g_RealmID);                                                ///< Virtual Realm Addr
        l_Data << uint32(g_RealmID);                                                ///< Native Realm Addr
        l_Data << uint32(itr->second.Flags);                                        ///< player flag (0x1 = Friend, 0x2 = Ignored, 0x4 = Muted)
        l_Data << uint8(itr->second.Status);                                        ///< online/offline/etc?
        l_Data << uint32(itr->second.Area);                                         ///< player area
        l_Data << uint32(itr->second.Level);                                        ///< player level
        l_Data << uint32(itr->second.Class);                                        ///< player class

        l_Data.WriteBits(itr->second.Note.size(), 10);
        l_Data.FlushBits();

        l_Data.WriteString(itr->second.Note);                                       ///< String note
    }

    p_Player->GetSession()->SendPacket(&l_Data);
}

bool PlayerSocial::HasFriend(uint32 friend_guid)
{
    PlayerSocialMap::const_iterator itr = m_playerSocialMap.find(friend_guid);
    if (itr != m_playerSocialMap.end())
        return itr->second.Flags & SOCIAL_FLAG_FRIEND;
    return false;
}

bool PlayerSocial::HasIgnore(uint32 ignore_guid)
{
    PlayerSocialMap::const_iterator itr = m_playerSocialMap.find(ignore_guid);
    if (itr != m_playerSocialMap.end())
        return itr->second.Flags & SOCIAL_FLAG_IGNORED;
    return false;
}

SocialMgr::SocialMgr()
{
}

SocialMgr::~SocialMgr()
{
}

void SocialMgr::GetFriendInfo(Player* player, uint32 friendGUID, FriendInfo &friendInfo)
{
    if (!player)
        return;

    friendInfo.Status = FRIEND_STATUS_OFFLINE;
    friendInfo.Area = 0;
    friendInfo.Level = 0;
    friendInfo.Class = 0;

    Player* pFriend = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(friendGUID, 0, HIGHGUID_PLAYER));
#ifndef CROSS
    
    /// handle if friend is in interrealm bg
#endif /* not CROSS */
    if (!pFriend)
#ifndef CROSS
    {
        pFriend = ObjectAccessor::FindPlayerInOrOutOfWorld(friendGUID);
        if (!pFriend || !pFriend->GetSession()->GetInterRealmBG())
            return;

        friendInfo.Status = FRIEND_STATUS_ONLINE;
        if (pFriend->isAFK())
            friendInfo.Status = FRIEND_STATUS_AFK;
        if (pFriend->isDND())
            friendInfo.Status = FRIEND_STATUS_DND;

        
        friendInfo.Level = pFriend->getLevel();
        friendInfo.Class = pFriend->getClass();

        uint32 zoneId = pFriend->GetSession()->GetInterRealmBG();
        friendInfo.Area = zoneId > 1 ? zoneId : 0;

#endif /* not CROSS */
        return;
#ifndef CROSS
    }
#endif /* not CROSS */

    uint32 team = player->GetTeam();
    AccountTypes security = player->GetSession()->GetSecurity();
    bool allowTwoSideWhoList = sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_WHO_LIST);
    AccountTypes gmLevelInWhoList = AccountTypes(sWorld->getIntConfig(CONFIG_GM_LEVEL_IN_WHO_LIST));

#ifndef CROSS
    PlayerSocialMap::iterator itr = player->GetSocial()->m_playerSocialMap.find(friendGUID);
    if (itr != player->GetSocial()->m_playerSocialMap.end())
        friendInfo.Note = itr->second.Note;
#else /* CROSS */
    if (player->GetSocial())
    {
        PlayerSocialMap::iterator itr = player->GetSocial()->m_playerSocialMap.find(friendGUID);
        if (itr != player->GetSocial()->m_playerSocialMap.end())
            friendInfo.Note = itr->second.Note;
    }
#endif /* CROSS */

    // PLAYER see his team only and PLAYER can't see MODERATOR, GAME MASTER, ADMINISTRATOR characters
    // MODERATOR, GAME MASTER, ADMINISTRATOR can see all
    if (pFriend && pFriend->GetName() &&
        (!AccountMgr::IsPlayerAccount(security) ||
        ((pFriend->GetTeam() == team || allowTwoSideWhoList) && (pFriend->GetSession()->GetSecurity() <= gmLevelInWhoList))) &&
        pFriend->IsVisibleGloballyFor(player))
    {
        friendInfo.Status = FRIEND_STATUS_ONLINE;
        if (pFriend->isAFK())
            friendInfo.Status = FRIEND_STATUS_AFK;
        if (pFriend->isDND())
            friendInfo.Status = FRIEND_STATUS_DND;
        friendInfo.Area = pFriend->GetZoneId();
        friendInfo.Level = pFriend->getLevel();
        friendInfo.Class = pFriend->getClass();
    }
}

void SocialMgr::SendFriendStatus(Player* p_Player, FriendsResult p_Result, uint32 p_FriendLowGuid, bool p_Broadcast)
{
    FriendInfo l_FriendInfo;

    Player * l_Friend = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(p_FriendLowGuid, 0, HIGHGUID_PLAYER));

    GetFriendInfo(p_Player, p_FriendLowGuid, l_FriendInfo);

    WorldPacket l_Response(SMSG_FRIEND_STATUS, 50);

    l_Response << uint8(p_Result);
    l_Response.appendPackGUID(MAKE_NEW_GUID(p_FriendLowGuid, 0, HIGHGUID_PLAYER));
    l_Response.appendPackGUID(l_Friend ? l_Friend->GetSession()->GetWoWAccountGUID() : 0);
    l_Response << uint32(g_RealmID);
    
    switch (p_Result)
    {
        case FRIEND_ADDED_ONLINE:
        case FRIEND_ONLINE:
            l_Response << uint8(l_FriendInfo.Status);
            l_Response << uint32(l_FriendInfo.Area);
            l_Response << uint32(l_FriendInfo.Level);
            l_Response << uint32(l_FriendInfo.Class);
            break;

        default:
            l_Response << uint8(0);
            l_Response << uint32(0);
            l_Response << uint32(0);
            l_Response << uint32(0);
            break;

    }

    switch (p_Result)
    {
        case FRIEND_ADDED_OFFLINE:
        case FRIEND_ADDED_ONLINE:
            l_Response.WriteBits(l_FriendInfo.Note.size(), 10);
            l_Response.FlushBits();

            l_Response.WriteString(l_FriendInfo.Note);
            break;

        default:
            l_Response.WriteBits(0, 10);
            l_Response.FlushBits();
            break;

    }
    
    if (p_Broadcast)
        BroadcastToFriendListers(p_Player, &l_Response);
    else
        p_Player->GetSession()->SendPacket(&l_Response);
}

void SocialMgr::BroadcastToFriendListers(Player* player, WorldPacket* packet)
{
    if (!player)
        return;

    uint32 team = player->GetTeam();
    AccountTypes security = player->GetSession()->GetSecurity();
    uint32 guid = player->GetGUIDLow();
    AccountTypes gmLevelInWhoList = AccountTypes(sWorld->getIntConfig(CONFIG_GM_LEVEL_IN_WHO_LIST));
    bool allowTwoSideWhoList = sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_WHO_LIST);

    for (SocialMap::const_iterator itr = m_socialMap.begin(); itr != m_socialMap.end(); ++itr)
    {
        PlayerSocialMap::const_iterator itr2 = itr->second.m_playerSocialMap.find(guid);
        if (itr2 != itr->second.m_playerSocialMap.end() && (itr2->second.Flags & SOCIAL_FLAG_FRIEND))
        {
            Player* pFriend = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(itr->first, 0, HIGHGUID_PLAYER));

            // PLAYER see his team only and PLAYER can't see MODERATOR, GAME MASTER, ADMINISTRATOR characters
            // MODERATOR, GAME MASTER, ADMINISTRATOR can see all
            if (pFriend && pFriend->IsInWorld() &&
                (!AccountMgr::IsPlayerAccount(pFriend->GetSession()->GetSecurity()) ||
                ((pFriend->GetTeam() == team || allowTwoSideWhoList) && security <= gmLevelInWhoList)) &&
                player->IsVisibleGloballyFor(pFriend))
            {
                pFriend->GetSession()->SendPacket(packet);
            }
        }
    }
}
#ifndef CROSS

PlayerSocial* SocialMgr::LoadFromDB(PreparedQueryResult result, uint32 guid, uint32 account_id)
{
    PlayerSocial *social = &m_socialMap[guid];
    social->SetPlayerGUID(guid, account_id);

    if (!result)
        return social;

    uint32 friend_guid = 0;
    uint8 flags = 0;
    std::string note = "";

    do
    {
        Field* fields = result->Fetch();

        friend_guid = fields[0].GetUInt32();
        flags = fields[1].GetUInt8();
        note = fields[2].GetString();

        social->m_playerSocialMap[friend_guid] = FriendInfo(flags, note);

        // client's friends list and ignore list limit
        if (social->m_playerSocialMap.size() >= (SOCIALMGR_FRIEND_LIMIT + SOCIALMGR_IGNORE_LIMIT))
            break;
    }
    while (result->NextRow());

    return social;
}

#endif /* not CROSS */