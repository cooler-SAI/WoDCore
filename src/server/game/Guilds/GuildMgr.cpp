////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "GuildMgr.h"

#ifndef CROSS 

GuildMgr::GuildMgr()
{
    NextGuildId = 1;
}

GuildMgr::~GuildMgr()
{
    for (GuildContainer::iterator itr = GuildStore.begin(); itr != GuildStore.end(); ++itr)
        delete itr->second;
}

void GuildMgr::AddGuild(Guild* guild)
{
    GuildStore[guild->GetId()] = guild;
}

void GuildMgr::RemoveGuild(uint32 guildId)
{
    GuildStore.erase(guildId);
}

void GuildMgr::SaveGuilds()
{
    for (GuildContainer::iterator itr = GuildStore.begin(); itr != GuildStore.end(); ++itr)
        itr->second->SaveToDB();
}

uint32 GuildMgr::GenerateGuildId()
{
    if (NextGuildId >= 0xFFFFFFFE)
    {
        sLog->outError(LOG_FILTER_GUILD, "Guild ids overflow!! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }
    return NextGuildId++;
}

// Guild collection
Guild* GuildMgr::GetGuildById(uint32 guildId) const
{
    GuildContainer::const_iterator itr = GuildStore.find(guildId);
    if (itr != GuildStore.end())
        return itr->second;

    return NULL;
}

Guild* GuildMgr::GetGuildByGuid(uint64 guid) const
{
    // Full guids are only used when receiving/sending data to client
    // everywhere else guild id is used
    if (IS_GUILD(guid))
        if (uint32 guildId = GUID_LOPART(guid))
            return GetGuildById(guildId);

    return NULL;
}

Guild* GuildMgr::GetGuildByName(const std::string& guildName) const
{
    std::string search = guildName;
    std::transform(search.begin(), search.end(), search.begin(), ::toupper);
    for (GuildContainer::const_iterator itr = GuildStore.begin(); itr != GuildStore.end(); ++itr)
    {
        std::string gname = itr->second->GetName();
        std::transform(gname.begin(), gname.end(), gname.begin(), ::toupper);
        if (search == gname)
            return itr->second;
    }
    return NULL;
}

std::string GuildMgr::GetGuildNameById(uint32 guildId) const
{
    if (Guild* guild = GetGuildById(guildId))
        return guild->GetName();

    return "";
}

Guild* GuildMgr::GetGuildByLeader(uint64 guid) const
{
    for (GuildContainer::const_iterator itr = GuildStore.begin(); itr != GuildStore.end(); ++itr)
        if (itr->second->GetLeaderGUID() == guid)
            return itr->second;

    return NULL;
}

void GuildMgr::ResetReputationCaps()
{
    /// @TODO: Implement
}


void GuildMgr::LoadGuilds()
{
    // 1. Load all guilds
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading guilds definitions...");
    {
        uint32 oldMSTime = getMSTime();

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD);
        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        if (!result)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 guild definitions. DB table `guild` is empty.");
            return;
        }
        else
        {
            uint32 count = 0;
            do
            {
                Field* fields = result->Fetch();
                Guild* guild = new Guild();

                if (!guild->LoadFromDB(fields))
                {
                    delete guild;
                    continue;
                }
                AddGuild(guild);

                ++count;
            } while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u guild definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // 2. Load all guild ranks
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading guild ranks...");
    {
        uint32 oldMSTime = getMSTime();

        // Delete orphaned guild rank entries before loading the valid ones
        CharacterDatabase.DirectExecute("DELETE gr FROM guild_rank gr LEFT JOIN guild g ON gr.guildId = g.guildId WHERE g.guildId IS NULL");

        //                                                         0    1      2       3                4
        QueryResult result = CharacterDatabase.Query("SELECT guildid, rid, rname, rights, BankMoneyPerDay FROM guild_rank ORDER BY guildid ASC, rid ASC");

        if (!result)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 guild ranks. DB table `guild_rank` is empty.");
        }
        else
        {
            uint32 count = 0;
            do
            {
                Field* fields = result->Fetch();
                uint32 guildId = fields[0].GetUInt32();

                if (Guild* guild = GetGuildById(guildId))
                    guild->LoadRankFromDB(fields);

                ++count;
            } while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u guild ranks in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // 3. Load all guild members
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading guild members...");
    {
        uint32 oldMSTime = getMSTime();

        // Delete orphaned guild member entries before loading the valid ones
        CharacterDatabase.DirectExecute("DELETE gm FROM guild_member gm LEFT JOIN guild g ON gm.guildId = g.guildId WHERE g.guildId IS NULL");

        //          0        1        2     3      4        5                   6
        QueryResult result = CharacterDatabase.Query("SELECT gm.guildid, gm.guid, rank, pnote, offnote, BankResetTimeMoney, BankRemMoney, "
            //   7                  8                 9                  10                11                 12
            "BankResetTimeTab0, BankRemSlotsTab0, BankResetTimeTab1, BankRemSlotsTab1, BankResetTimeTab2, BankRemSlotsTab2, "
            //   13                 14                15                 16                17                 18
            "BankResetTimeTab3, BankRemSlotsTab3, BankResetTimeTab4, BankRemSlotsTab4, BankResetTimeTab5, BankRemSlotsTab5, "
            //   19                 20                21                 22
            "BankResetTimeTab6, BankRemSlotsTab6, BankResetTimeTab7, BankRemSlotsTab7, "
            //   23      24       25       26      27         28
            "c.name, c.level, c.class, c.zone, c.account, c.logout_time "
            "FROM guild_member gm LEFT JOIN characters c ON c.guid = gm.guid ORDER BY guildid ASC");

        if (!result)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 guild members. DB table `guild_member` is empty.");
        }
        else
        {
            uint32 count = 0;

            do
            {
                Field* fields = result->Fetch();
                uint32 guildId = fields[0].GetUInt32();

                if (Guild* guild = GetGuildById(guildId))
                    guild->LoadMemberFromDB(fields);

                ++count;
            } while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u guild members int %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // 4. Load all guild bank tab rights
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading bank tab rights...");
    {
        uint32 oldMSTime = getMSTime();

        // Delete orphaned guild bank right entries before loading the valid ones
        CharacterDatabase.DirectExecute("DELETE gbr FROM guild_bank_right gbr LEFT JOIN guild g ON gbr.guildId = g.guildId WHERE g.guildId IS NULL");

        //       0        1      2    3        4
        QueryResult result = CharacterDatabase.Query("SELECT guildid, TabId, rid, gbright, SlotPerDay FROM guild_bank_right ORDER BY guildid ASC, TabId ASC");

        if (!result)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 guild bank tab rights. DB table `guild_bank_right` is empty.");
        }
        else
        {
            uint32 count = 0;
            do
            {
                Field* fields = result->Fetch();
                uint32 guildId = fields[0].GetUInt32();

                if (Guild* guild = GetGuildById(guildId))
                    guild->LoadBankRightFromDB(fields);

                ++count;
            } while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u bank tab rights in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // 5. Load all event logs
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading guild event logs...");
    {
        uint32 oldMSTime = getMSTime();

        CharacterDatabase.DirectPExecute("DELETE FROM guild_eventlog WHERE LogGuid > %u", sWorld->getIntConfig(CONFIG_GUILD_EVENT_LOG_COUNT));

        //          0        1        2          3            4            5        6
        QueryResult result = CharacterDatabase.Query("SELECT guildid, LogGuid, EventType, PlayerGuid1, PlayerGuid2, NewRank, TimeStamp FROM guild_eventlog ORDER BY TimeStamp DESC, LogGuid DESC");

        if (!result)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 guild event logs. DB table `guild_eventlog` is empty.");
        }
        else
        {
            uint32 count = 0;
            do
            {
                Field* fields = result->Fetch();
                uint32 guildId = fields[0].GetUInt32();

                if (Guild* guild = GetGuildById(guildId))
                    guild->LoadEventLogFromDB(fields);

                ++count;
            } while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u guild event logs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // 6. Load guild achievements
    {
        PreparedQueryResult achievementResult;
        PreparedQueryResult criteriaResult;
        for (GuildContainer::const_iterator itr = GuildStore.begin(); itr != GuildStore.end(); ++itr)
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_ACHIEVEMENT);
            stmt->setUInt32(0, itr->first);
            achievementResult = CharacterDatabase.Query(stmt);
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_ACHIEVEMENT_CRITERIA);
            stmt->setUInt32(0, itr->first);
            criteriaResult = CharacterDatabase.Query(stmt);

            itr->second->GetAchievementMgr().LoadFromDB(NULL, itr->second, achievementResult, criteriaResult);
        }
    }

    // 7. Deleting old Guild News (older than one week)
    sLog->outInfo(LOG_FILTER_GENERAL, "Deleting old Guild News");
    {
        CharacterDatabase.PQuery("DELETE FROM guild_news_log WHERE date < %u;", uint32(time(NULL) - DAY * 7));
    }

    // 8. Loading Guild news
    sLog->outInfo(LOG_FILTER_GENERAL, "Loading Guild News");
    {
        for (GuildContainer::const_iterator itr = GuildStore.begin(); itr != GuildStore.end(); ++itr)
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_LOAD_GUILD_NEWS);
            stmt->setInt32(0, itr->first);
            itr->second->GetNewsLog().LoadFromDB(CharacterDatabase.Query(stmt));
        }
    }

    // 9. Validate loaded guild data
    sLog->outInfo(LOG_FILTER_GENERAL, "Validating data of loaded guilds...");
    {
        uint32 oldMSTime = getMSTime();

        for (GuildContainer::iterator itr = GuildStore.begin(); itr != GuildStore.end();)
        {
            Guild* guild = itr->second;
            ++itr;
            if (guild && !guild->Validate())
                delete guild;
        }

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Validated data of loaded guilds in %u ms", GetMSTimeDiffToNow(oldMSTime));
    }

    // 10. Validate loaded guild challenges
    sLog->outInfo(LOG_FILTER_GENERAL, "Validating guild Challenges...");
    {
        uint32 l_OldMSTime = getMSTime();
        PreparedQueryResult l_Result = CharacterDatabase.Query(CharacterDatabase.GetPreparedStatement(CHAR_LOAD_GUILD_CHALLENGES));

        if (l_Result)
        {
            uint32 l_Count = 0;
            do
            {
                Field* l_Fields = l_Result->Fetch();
                uint32 l_GuildId = l_Fields[0].GetInt32();

                if (Guild* l_Guild = GetGuildById(l_GuildId))
                {
                    l_Guild->LoadGuildChallengesFromDB(l_Fields);
                }
                ++l_Count;

            } while (l_Result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u guild challenges in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
        }
    }

    /// Delete orphaned guild bank tab
    CharacterDatabase.DirectExecute("DELETE gbt FROM guild_bank_tab gbt LEFT JOIN guild g ON gbt.guildId = g.guildId WHERE g.guildId IS NULL");

    /// Delete orphan guild bank items
    CharacterDatabase.DirectExecute("DELETE gbi FROM guild_bank_item gbi LEFT JOIN guild g ON gbi.guildId = g.guildId WHERE g.guildId IS NULL");

    /// Remove log entries that exceed the number of allowed entries per guild
    CharacterDatabase.DirectPExecute("DELETE FROM guild_bank_eventlog WHERE LogGuid > %u", sWorld->getIntConfig(CONFIG_GUILD_BANK_EVENT_LOG_COUNT));
}

void GuildMgr::LoadGuildRewards()
{
    GuildRewards.clear();
    uint32 oldMSTime = getMSTime();

    //                                                  0      1         2        3        4
    QueryResult result = WorldDatabase.Query("SELECT entry, standing, racemask, price, achievement FROM guild_rewards");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 guild reward definitions. DB table `guild_rewards` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        GuildReward reward;
        Field* fields = result->Fetch();
        reward.Entry = fields[0].GetUInt32();
        reward.Standing = fields[1].GetUInt8();
        reward.Racemask = fields[2].GetInt32();
        reward.Price = fields[3].GetUInt64();
        reward.AchievementId = fields[4].GetUInt32();

        if (!sObjectMgr->GetItemTemplate(reward.Entry))
        {
            sLog->outError(LOG_FILTER_SERVER_LOADING, "Guild rewards contains not existing item entry %u", reward.Entry);
            continue;
        }

        if (reward.AchievementId != 0 && (!sAchievementStore.LookupEntry(reward.AchievementId)))
        {
            sLog->outError(LOG_FILTER_SERVER_LOADING, "Guild rewards contains not existing achievement entry %u", reward.AchievementId);
            continue;
        }

        if (reward.Standing >= MAX_REPUTATION_RANK)
        {
            sLog->outError(LOG_FILTER_SERVER_LOADING, "Guild rewards contains wrong reputation standing %u, max is %u", uint32(reward.Standing), MAX_REPUTATION_RANK - 1);
            continue;
        }

        GuildRewards.push_back(reward);
        ++count;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u guild reward definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}
#else
GuildMgr::GuildMgr()
{
}

GuildMgr::~GuildMgr()
{
}

InterRealmGuildEmblem::InterRealmGuildEmblem(uint32 style, uint32 color, uint32 borderStyle, uint32 borderColor, uint32 backgroundColor) :
m_Style(style), m_Color(color), m_BorderStyle(borderStyle), m_BorderColor(borderColor), m_BackgroundColor(backgroundColor)
{

}

InterRealmGuildRank::InterRealmGuildRank(uint32 rankId, std::string& rankName)
{
    m_RankId = rankId;
    m_RankName = rankName;
}

InterRealmGuild::InterRealmGuild(uint64 guildGuid, std::string& guildName, uint32 membersCount, uint32 level, uint64 experience, uint32 achievementPoints, InterRealmGuildEmblem* emblem)
{
    m_GuildGUID = guildGuid;
    m_GuildName = guildName;
    m_MembersCount = membersCount;
    m_GuildLevel = level;
    m_Experience = experience;
    m_AchievementPoints = achievementPoints;

    m_Emblem = emblem;
}

InterRealmGuild::~InterRealmGuild()
{
    if (m_Emblem != NULL)
    {
        delete m_Emblem;
        m_Emblem = NULL;
    }

    for (InterRealmGuildRanks::iterator itr = m_Ranks.begin(); itr != m_Ranks.end(); ++itr)
        delete (*itr);

    m_Ranks.clear();
}

void InterRealmGuild::AddRank(InterRealmGuildRank* rank)
{
    m_Ranks.push_back(rank);
}

InterRealmGuildRanks const* InterRealmGuild::GetRanks()
{
    return &m_Ranks;
}

InterRealmGuildEmblem const* InterRealmGuild::GetEmblem()
{
    return m_Emblem;
}

void GuildMgr::AddInterRealmGuild(uint32 realmId, InterRealmGuild* guild)
{
    InterRealmGuilds& guilds = m_InterRealmGuilds[realmId];

    guilds[guild->GetGUID()] = guild;
}

void GuildMgr::ClearInterRealmGuilds()
{
    for (InterRealmGuildsContainer::iterator itr = m_InterRealmGuilds.begin(); itr != m_InterRealmGuilds.end(); ++itr)
    {
        for (InterRealmGuilds::iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
        {
            delete itr2->second;
        }
    }

    m_InterRealmGuilds.clear();
}

InterRealmGuildsContainer const* GuildMgr::GetInterRealmGuilds()
{
    return &m_InterRealmGuilds;
}

InterRealmGuilds const* GuildMgr::GetInterRealmGuildsByRealm(uint32 realmId)
{
    InterRealmGuildsContainer::const_iterator itr = m_InterRealmGuilds.find(realmId);
    if (itr == m_InterRealmGuilds.end())
        return NULL;

    return &(itr->second);
}

InterRealmGuild* GuildMgr::GetInterRealmGuild(uint32 realmId, uint64 guildGuid)
{
    InterRealmGuildsContainer::const_iterator itr = m_InterRealmGuilds.find(realmId);
    if (itr == m_InterRealmGuilds.end())
        return NULL;

    InterRealmGuilds::const_iterator itr2 = itr->second.find(guildGuid);
    if (itr2 == itr->second.end())
        return NULL;

    return itr2->second;
}

bool GuildMgr::HasInterRealmGuild(uint32 realmId, uint64 guildGuid) const
{
    InterRealmGuildsContainer::const_iterator itr = m_InterRealmGuilds.find(realmId);
    if (itr == m_InterRealmGuilds.end())
        return NULL;

    return itr->second.find(guildGuid) != itr->second.end();
}

bool GuildMgr::HandleInterRealmGuildQuery(Player* p_Player, uint32 p_RealmId, uint64 p_GuildGUID)
{
    if (!IS_GUILD_GUID(p_GuildGUID))
        return false;

    InterRealmGuild* l_Guild = GetInterRealmGuild(p_RealmId, p_GuildGUID);
    if (!l_Guild)
        return false;

    WorldPacket l_Data(SMSG_QUERY_GUILD_INFO_RESPONSE, 500);

    l_Data.appendPackGUID(p_GuildGUID);
    l_Data.WriteBit(1);                 ///< hasData
    l_Data.FlushBits();

    l_Data.appendPackGUID(p_GuildGUID);
    l_Data << uint32(p_RealmId);
    l_Data << uint32(l_Guild->GetRanks()->size());
    l_Data << uint32(l_Guild->GetEmblem()->GetStyle());
    l_Data << uint32(l_Guild->GetEmblem()->GetColor());
    l_Data << uint32(l_Guild->GetEmblem()->GetBorderStyle());
    l_Data << uint32(l_Guild->GetEmblem()->GetBorderColor());
    l_Data << uint32(l_Guild->GetEmblem()->GetBackgroundColor());

    uint32 l_I = 0;

    for (InterRealmGuildRanks::const_iterator itr = l_Guild->GetRanks()->begin(); itr != l_Guild->GetRanks()->end(); ++itr)
    {
        l_Data << uint32((*itr)->GetRankId());
        l_Data << uint32(l_I);

        l_Data.WriteBits((*itr)->GetRankName().size(), 7);
        l_Data.FlushBits();

        l_Data.WriteString((*itr)->GetRankName());

        l_I++;
    }

    l_Data.WriteBits(l_Guild->GetName().size(), 7);
    l_Data.FlushBits();

    l_Data.WriteString(l_Guild->GetName());

    p_Player->GetSession()->SendPacket(&l_Data);

    return true;
}
#endif