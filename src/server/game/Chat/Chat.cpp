////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "ObjectMgr.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"

#include "AccountMgr.h"
#include "CellImpl.h"
#include "Chat.h"
#include "GridNotifiersImpl.h"
#include "Language.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include "Group.h"
#include "UpdateMask.h"
#include "SpellMgr.h"
#include "ScriptMgr.h"
#include "ChatLink.h"
#ifndef CROSS
#include "Guild.h"
#endif /* not CROSS */
#include "Threading.h"
#include "MSSignalHandler.h"

bool ChatHandler::load_command_table = true;

// get number of commands in table
static size_t getCommandTableSize(const ChatCommand* commands)
{
    if (!commands)
        return 0;
    size_t count = 0;
    while (commands[count].Name != NULL)
        count++;
    return count;
}

// append source command table to target, return number of appended commands
static size_t appendCommandTable(ChatCommand* target, const ChatCommand* source)
{
    const size_t count = getCommandTableSize(source);
    if (count)
        memcpy(target, source, count * sizeof(ChatCommand));
    return count;
}

ChatCommand* ChatHandler::getCommandTable()
{
    // cache for commands, needed because some commands are loaded dynamically through ScriptMgr
    // cache is never freed and will show as a memory leak in diagnostic tools
    // can't use vector as vector storage is implementation-dependent, eg, there can be alignment gaps between elements
    static ChatCommand* commandTableCache = 0;

    if (LoadCommandTable())
    {
        SetLoadCommandTable(false);

        {
            // count total number of top-level commands
            size_t total = 0;
            std::vector<ChatCommand*> const& dynamic = sScriptMgr->GetChatCommands();
            for (std::vector<ChatCommand*>::const_iterator it = dynamic.begin(); it != dynamic.end(); ++it)
                total += getCommandTableSize(*it);
            total += 1; // ending zero

            // cache top-level commands
            size_t added = 0;
            commandTableCache = (ChatCommand*)malloc(sizeof(ChatCommand) * total);
            ACE_ASSERT(commandTableCache);
            memset(commandTableCache, 0, sizeof(ChatCommand) * total);
            for (std::vector<ChatCommand*>::const_iterator it = dynamic.begin(); it != dynamic.end(); ++it)
                added += appendCommandTable(commandTableCache + added, *it);
        }

        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_COMMANDS);
        PreparedQueryResult result = WorldDatabase.Query(stmt);
        if (result)
        {
            do
            {
                Field* fields = result->Fetch();
                std::string name = fields[0].GetString();

                SetDataForCommandInTable(commandTableCache, name.c_str(), fields[1].GetUInt8(), fields[2].GetString(), name);

            }
            while (result->NextRow());
        }
    }

    return commandTableCache;
}

std::string ChatHandler::PGetParseString(int32 entry, ...) const
{
    const char *format = GetTrinityString(entry);
    char str[1024];
    va_list ap;
    va_start(ap, entry);
    vsnprintf(str, 1024, format, ap);
    va_end(ap);
    return std::string(str);
}

const char *ChatHandler::GetTrinityString(int32 entry) const
{
    return m_session->GetTrinityString(entry);
}

bool ChatHandler::isAvailable(ChatCommand const& cmd) const
{
    // Command have been disable because it make crash
    if (cmd.SecurityLevel == (uint32)SpecificSecurityLevel::DisableByFailure)
        return false;

    // check security level only for simple  command (without child commands)
    return m_session->GetSecurity() >= AccountTypes(cmd.SecurityLevel);
}

bool ChatHandler::HasLowerSecurity(Player* target, uint64 guid, bool strong)
{
    WorldSession* target_session = NULL;
    uint32 target_account = 0;

    if (target)
        target_session = target->GetSession();
    else if (guid)
        target_account = sObjectMgr->GetPlayerAccountIdByGUID(guid);

    if (!target_session && !target_account)
    {
        SendSysMessage(LANG_PLAYER_NOT_FOUND);
        SetSentErrorMessage(true);
        return true;
    }

    return HasLowerSecurityAccount(target_session, target_account, strong);
}

bool ChatHandler::HasLowerSecurityAccount(WorldSession* target, uint32 target_account, bool strong)
{
    uint32 target_sec;

    // allow everything from console and RA console
    if (!m_session)
        return false;

    // ignore only for non-players for non strong checks (when allow apply command at least to same sec level)
    if (!AccountMgr::IsPlayerAccount(m_session->GetSecurity()) && !strong && !sWorld->getBoolConfig(CONFIG_GM_LOWER_SECURITY))
        return false;

    if (target)
        target_sec = target->GetSecurity();
    else if (target_account)
        target_sec = AccountMgr::GetSecurity(target_account, g_RealmID);
    else
        return true;                                        // caller must report error for (target == NULL && target_account == 0)

    AccountTypes target_ac_sec = AccountTypes(target_sec);
    if (m_session->GetSecurity() < target_ac_sec || (strong && m_session->GetSecurity() <= target_ac_sec))
    {
        SendSysMessage(LANG_YOURS_SECURITY_IS_LOW);
        SetSentErrorMessage(true);
        return true;
    }

    return false;
}

bool ChatHandler::hasStringAbbr(const char* name, const char* part)
{
    // non "" command
    if (*name)
    {
        // "" part from non-"" command
        if (!*part)
            return false;

        for (;;)
        {
            if (!*part)
                return true;
            else if (!*name)
                return false;
            else if (tolower(*name) != tolower(*part))
                return false;
            ++name; ++part;
        }
    }
    // allow with any for ""

    return true;
}

void ChatHandler::SendSysMessage(const char *str)
{
    WorldPacket data;

    // need copy to prevent corruption by strtok call in LineFromMessage original string
    char* buf = strdup(str);
    char* pos = buf;

    while (char* line = LineFromMessage(pos))
    {
        FillSystemMessageData(&data, line);
        m_session->SendPacket(&data);
    }

    free(buf);
}

void ChatHandler::SendGlobalSysMessage(const char *str)
{
    // Chat output
    WorldPacket data;

    // need copy to prevent corruption by strtok call in LineFromMessage original string
    char* buf = strdup(str);
    char* pos = buf;

    while (char* line = LineFromMessage(pos))
    {
        FillSystemMessageData(&data, line);
        sWorld->SendGlobalMessage(&data);
    }

    free(buf);
}

void ChatHandler::SendGlobalGMSysMessage(const char *str)
{
    // Chat output
    WorldPacket data;

    // need copy to prevent corruption by strtok call in LineFromMessage original string
    char* buf = strdup(str);
    char* pos = buf;

    while (char* line = LineFromMessage(pos))
    {
        FillSystemMessageData(&data, line);
        sWorld->SendGlobalGMMessage(&data);
     }
    free(buf);
}

void ChatHandler::SendSysMessage(int32 entry)
{
    SendSysMessage(GetTrinityString(entry));
}

void ChatHandler::PSendSysMessage(int32 entry, ...)
{
    const char *format = GetTrinityString(entry);
    va_list ap;
    char str [2048];
    va_start(ap, entry);
    vsnprintf(str, 2048, format, ap);
    va_end(ap);
    SendSysMessage(str);
}

void ChatHandler::PSendSysMessage(const char *format, ...)
{
    va_list ap;
    char str [2048];
    va_start(ap, format);
    vsnprintf(str, 2048, format, ap);
    va_end(ap);
    SendSysMessage(str);
}

bool ChatHandler::ExecuteCommandInTable(ChatCommand* table, const char* text, const std::string& fullcmd)
{
    char const* oldtext = text;
    std::string cmd = "";

    while (*text != ' ' && *text != '\0')
    {
        cmd += *text;
        ++text;
    }

    while (*text == ' ') ++text;

    for (uint32 i = 0; table[i].Name != NULL; ++i)
    {
        if (!hasStringAbbr(table[i].Name, cmd.c_str()))
            continue;

        bool match = false;
        if (strlen(table[i].Name) > cmd.length())
        {
            for (uint32 j = 0; table[j].Name != NULL; ++j)
            {
                if (!hasStringAbbr(table[j].Name, cmd.c_str()))
                    continue;

                if (strcmp(table[j].Name, cmd.c_str()) != 0)
                    continue;
                else
                {
                    match = true;
                    break;
                }
            }
        }
        if (match)
            continue;

        // select subcommand from child commands list
        if (table[i].ChildCommands != NULL)
        {
            if (!ExecuteCommandInTable(table[i].ChildCommands, text, fullcmd))
            {
                if (text [0] != '\0')
                    SendSysMessage(LANG_NO_SUBCMD);
                else
                    SendSysMessage(LANG_CMD_SYNTAX);

                ShowHelpForCommand(table[i].ChildCommands, text);
            }

            return true;
        }

        // must be available and have handler
        if (!table[i].Handler || !isAvailable(table[i]))
            continue;

        SetSentErrorMessage(false);

        bool   l_CommandResult = false;
        uint32 l_TimeStamp     = getMSTime();

        //MS::SignalHandler::EnableThrowExceptionAtFailure();
        try
        {
            l_CommandResult = (table[i].Handler)(this, table[i].Name[0] != '\0' ? text : oldtext);
        }
        catch (std::exception&)
        {
            l_CommandResult = false;

            // Disable the command
            table[i].SecurityLevel = (uint32)SpecificSecurityLevel::DisableByFailure;

            sLog->outError(LOG_FILTER_WORLDSERVER, "Crash intercepted => ChatHandler::ExecuteCommandInTable(%p, %s, %s)", table, text, fullcmd.c_str());
        }
        //MS::SignalHandler::DisableThrowExceptionAtFailure();

        uint32 l_CommandExecutionTime = getMSTime() - l_TimeStamp;
        if (l_CommandExecutionTime > 10)
            sLog->outAshran("Commmand [%s] take %u ms to execute", fullcmd.c_str(), l_CommandExecutionTime);

        // table[i].Name == "" is special case: send original command to handler
        if (l_CommandResult)
        {
            if (!AccountMgr::IsPlayerAccount(table[i].SecurityLevel))
            {
                // chat case
                if (m_session)
                {
                    Player* p = m_session->GetPlayer();
                    Player* selected = p->GetSelectedPlayer();
                    uint32 sel_guid = 0;
                    std::string sel_name = "ERROR";
                    uint32 sel_acc_guid = 0;

                    if (selected)
                    {
                        sel_guid = selected->GetGUIDLow();
                        sel_name = selected->GetName();
                        sel_acc_guid = selected->GetSession()->GetAccountId();
                    }

                    sLog->outCommand(m_session->GetAccountId(), "", m_session->GetPlayer()->GetGUIDLow(), m_session->GetPlayer()->GetName(),
                                    sel_acc_guid, "", sel_guid, sel_name.c_str(),
                                    "Command: %s [Player: %s (Account: %u) X: %f Y: %f Z: %f Map: %u Selected: %s (GUID: %u)]",
                                    fullcmd.c_str(), p->GetName(), m_session->GetAccountId(), p->GetPositionX(), p->GetPositionY(), p->GetPositionZ(), p->GetMapId(),
                                    sel_name.c_str(), sel_guid);
                }
            }
        }
        // some commands have custom error messages. Don't send the default one in these cases.
        else if (!HasSentErrorMessage())
        {
            if (table[i].Help && strlen(table[i].Help))
                SendSysMessage(table[i].Help);
            else
                SendSysMessage(LANG_CMD_SYNTAX);
        }

        return true;
    }

    return false;
}

bool ChatHandler::SetDataForCommandInTable(ChatCommand* table, const char* text, uint32 security, std::string const& help, std::string const& fullcommand)
{
    std::string cmd = "";

    while (*text != ' ' && *text != '\0')
    {
        cmd += *text;
        ++text;
    }

    while (*text == ' ') ++text;

    for (uint32 i = 0; table[i].Name != NULL; i++)
    {
        // for data fill use full explicit command names
        if (table[i].Name != cmd)
            continue;

        // select subcommand from child commands list (including "")
        if (table[i].ChildCommands != NULL)
        {
            if (SetDataForCommandInTable(table[i].ChildCommands, text, security, help, fullcommand))
                return true;
            else if (*text)
                return false;

            // fail with "" subcommands, then use normal level up command instead
        }
        // expected subcommand by full name DB content
        else if (*text)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `command` have unexpected subcommand '%s' in command '%s', skip.", text, fullcommand.c_str());
            return false;
        }

        if (table[i].SecurityLevel != security)
            sLog->outInfo(LOG_FILTER_GENERAL, "Table `command` overwrite for command '%s' default security (%u) by %u", fullcommand.c_str(), table[i].SecurityLevel, security);

        table[i].SecurityLevel = security;

        char * l_NewHelp = new char[help.size() + 1];
        sprintf(l_NewHelp, "%s", help.c_str());
        l_NewHelp[help.size()] = '\0';

        table[i].Help = (char const*)l_NewHelp;

        return true;
    }

    // in case "" command let process by caller
    if (!cmd.empty())
    {
        if (table == getCommandTable())
            sLog->outError(LOG_FILTER_SQL, "Table `command` have not existed command '%s', skip.", cmd.c_str());
        else
            sLog->outError(LOG_FILTER_SQL, "Table `command` have not existed subcommand '%s' in command '%s', skip.", cmd.c_str(), fullcommand.c_str());
    }

    return false;
}

int ChatHandler::ParseCommands(const char* text)
{
    ASSERT(text);
    ASSERT(*text);

    std::string fullcmd = text;

    if (m_session && AccountMgr::IsPlayerAccount(m_session->GetSecurity()) && !sWorld->getBoolConfig(CONFIG_ALLOW_PLAYER_COMMANDS))
       return 0;

    /// chat case (.command or !command format)
    if (m_session)
    {
        if (text[0] != '!' && text[0] != '.')
            return 0;
    }

    /// ignore single . and ! in line
    if (strlen(text) < 2)
        return 0;
    // original `text` can't be used. It content destroyed in command code processing.

    /// ignore messages staring from many dots.
    if ((text[0] == '.' && text[1] == '.') || (text[0] == '!' && text[1] == '!'))
        return 0;

    /// skip first . or ! (in console allowed use command with . and ! and without its)
    if (text[0] == '!' || text[0] == '.')
        ++text;

    if (!ExecuteCommandInTable(getCommandTable(), text, fullcmd))
    {
        if (m_session && AccountMgr::IsPlayerAccount(m_session->GetSecurity()))
            return 0;

        SendSysMessage(LANG_NO_CMD);
    }
    return 1;
}

bool ChatHandler::isValidChatMessage(const char* message)
{
/*
Valid examples:
|cffa335ee|Hitem:812:0:0:0:0:0:0:0:70|h[Glowing Brightwood Staff]|h|r
|cff808080|Hquest:2278:47|h[The Platinum Discs]|h|r
|cffffd000|Htrade:4037:1:150:1:6AAAAAAAAAAAAAAAAAAAAAAOAADAAAAAAAAAAAAAAAAIAAAAAAAAA|h[Engineering]|h|r
|cff4e96f7|Htalent:2232:-1|h[Taste for Blood]|h|r
|cff71d5ff|Hspell:21563|h[Command]|h|r
|cffffd000|Henchant:3919|h[Engineering: Rough Dynamite]|h|r
|cffffff00|Hachievement:546:0000000000000001:0:0:0:-1:0:0:0:0|h[Safe Deposit]|h|r
|cff66bbff|Hglyph:21:762|h[Glyph of Bladestorm]|h|r

| will be escaped to ||
*/

    if (strlen(message) > 255)
        return false;

    // more simple checks
    if (sWorld->getIntConfig(CONFIG_CHAT_STRICT_LINK_CHECKING_SEVERITY) < 3)
    {
        const char validSequence[6] = "cHhhr";
        const char* validSequenceIterator = validSequence;
        const std::string validCommands = "cHhr|";

        while (*message)
        {
            // find next pipe command
            message = strchr(message, '|');

            if (!message)
                return true;

            ++message;
            char commandChar = *message;
            if (validCommands.find(commandChar) == std::string::npos)
                return false;

            ++message;
            // validate sequence
            if (sWorld->getIntConfig(CONFIG_CHAT_STRICT_LINK_CHECKING_SEVERITY) == 2)
            {
                if (commandChar == *validSequenceIterator)
                {
                    if (validSequenceIterator == validSequence + 4)
                        validSequenceIterator = validSequence;
                    else
                        ++validSequenceIterator;
                }
                else
                    return false;
            }
        }
        return true;
    }

    return LinkExtractor(message).IsValidMessage();
}

bool ChatHandler::ShowHelpForSubCommands(ChatCommand* table, char const* cmd, char const* subcmd)
{
    std::string list;
    for (uint32 i = 0; table[i].Name != NULL; ++i)
    {
        // must be available (ignore handler existence for show command with possible available subcommands)
        if (!isAvailable(table[i]))
            continue;

        // for empty subcmd show all available
        if (*subcmd && !hasStringAbbr(table[i].Name, subcmd))
            continue;

        if (m_session)
            list += "\n    ";
        else
            list += "\n\r    ";

        list += table[i].Name;

        if (table[i].ChildCommands)
            list += " ...";
    }

    if (list.empty())
        return false;

    if (table == getCommandTable())
    {
        SendSysMessage(LANG_AVIABLE_CMD);
        PSendSysMessage("%s", list.c_str());
    }
    else
        PSendSysMessage(LANG_SUBCMDS_LIST, cmd, list.c_str());

    return true;
}

bool ChatHandler::ShowHelpForCommand(ChatCommand* table, const char* cmd)
{
    if (*cmd)
    {
        for (uint32 i = 0; table[i].Name != NULL; ++i)
        {
            // must be available (ignore handler existence for show command with possible available subcommands)
            if (!isAvailable(table[i]))
                continue;

            if (!hasStringAbbr(table[i].Name, cmd))
                continue;

            // have subcommand
            char const* subcmd = (*cmd) ? strtok(NULL, " ") : "";

            if (table[i].ChildCommands && subcmd && *subcmd)
            {
                if (ShowHelpForCommand(table[i].ChildCommands, subcmd))
                    return true;
            }

            if (table[i].Help && strlen(table[i].Help))
                SendSysMessage(table[i].Help);

            if (table[i].ChildCommands)
                if (ShowHelpForSubCommands(table[i].ChildCommands, table[i].Name, subcmd ? subcmd : ""))
                    return true;

            return table[i].Help && strlen(table[i].Help);
        }
    }
    else
    {
        for (uint32 i = 0; table[i].Name != NULL; ++i)
        {
            // must be available (ignore handler existence for show command with possible available subcommands)
            if (!isAvailable(table[i]))
                continue;

            if (strlen(table[i].Name))
                continue;

            if (table[i].Help && strlen(table[i].Help))
                SendSysMessage(table[i].Help);

            if (table[i].ChildCommands)
                if (ShowHelpForSubCommands(table[i].ChildCommands, "", ""))
                    return true;

            return (table[i].Help && strlen(table[i].Help));
        }
    }

    return ShowHelpForSubCommands(table, "", cmd);
}

//Note: target_guid used only in CHAT_MSG_WHISPER_INFORM mode (in this case channelName ignored)
void ChatHandler::FillMessageData(WorldPacket* data, WorldSession* session, uint8 type, uint32 language, const char *channelName, uint64 target_guid, const char *message, Unit* speaker, const char* addonPrefix /*= NULL*/, uint32 achievementId /*= 0*/)
{
    uint32 messageLength = message ? strlen(message) : 0; ///< messageLength is unused

    uint32 speakerNameLength = 0;
    if (speaker)
        speakerNameLength = strlen(speaker->GetName());
    else if (session)
        speakerNameLength = strlen(session->GetPlayer()->GetName());

    uint32 targetLength = 0;
    std::string targetName;
    if (target_guid)
    {
        if (Unit* unit = ObjectAccessor::FindUnit(target_guid))
        {
            targetLength = strlen(unit->GetName());
            targetName = unit->GetName();
        }
    }

    Player* speakerPlayer = NULL;
    if (speaker && speaker->IsPlayer())
        speakerPlayer = speaker->ToPlayer();
    else if (session)
        speakerPlayer = session->GetPlayer();

    ObjectGuid speakerGuid = 0;
    if (speaker)
        speakerGuid = speaker->GetGUID();
    else if (session)
        speakerGuid = session->GetPlayer()->GetGUID();

    ObjectGuid speakerGuildGuid = 0;
    if (speakerPlayer && speakerPlayer->GetGuild())
#ifndef CROSS
        speakerGuildGuid = speakerPlayer->GetGuild()->GetGUID();
#else /* CROSS */
        speakerGuildGuid = speakerPlayer->GetGuildId();
#endif /* CROSS */

    ObjectGuid groupGuid = 0;
    if (speakerPlayer && speakerPlayer->GetGroup())
        groupGuid = speakerPlayer->GetGroup()->GetGUID();

    switch (type)
    {
        case CHAT_MSG_SAY:
        case CHAT_MSG_PARTY:
        case CHAT_MSG_PARTY_LEADER:
        case CHAT_MSG_RAID:
        case CHAT_MSG_GUILD:
        case CHAT_MSG_OFFICER:
        case CHAT_MSG_YELL:
        case CHAT_MSG_WHISPER:
        case CHAT_MSG_CHANNEL:
        case CHAT_MSG_RAID_LEADER:
        case CHAT_MSG_RAID_WARNING:
        case CHAT_MSG_BG_SYSTEM_NEUTRAL:
        case CHAT_MSG_BG_SYSTEM_ALLIANCE:
        case CHAT_MSG_BG_SYSTEM_HORDE:
        case CHAT_MSG_INSTANCE_CHAT:
        case CHAT_MSG_INSTANCE_CHAT_LEADER:
            target_guid = speakerPlayer ? speakerPlayer->GetGUID() : 0;
            break;
        default:
            break;
    }

    ObjectGuid targetGuid = target_guid; ///< targetGuid is unused

    data->Initialize(SMSG_CHAT, 800);                   // guess size
    *data << uint8(type);
    *data << uint8(language);
    data->appendPackGUID(speakerGuid);
    data->appendPackGUID(speakerGuildGuid);
    data->appendPackGUID(0);
    data->appendPackGUID(0);
    *data << uint32(g_RealmID);
    *data << uint32(g_RealmID);
    data->appendPackGUID(groupGuid);
    *data << uint32(achievementId);
    *data << float(0);

    data->WriteBits(speakerNameLength, 11);
    data->WriteBits(targetLength, 11);
    data->WriteBits(addonPrefix ? strlen(addonPrefix) : 0, 5);
    data->WriteBits(channelName ? strlen(channelName) : 0, 7);
    data->WriteBits(message ? strlen(message) : 0, 12);
    data->WriteBits(speakerPlayer ? speakerPlayer->GetChatTag() : 0, 11);
    data->WriteBit(false);  ///< hide chat log
    data->WriteBit(false);  ///< Faker sender name
    data->FlushBits();

    if (speaker)
        data->WriteString(speaker->GetName());
    else if (session)
        data->WriteString(session->GetPlayer()->GetName());
    data->WriteString(targetName);
    data->WriteString(addonPrefix ? addonPrefix : "");
    data->WriteString(channelName ? channelName : "");
    data->WriteString(message ? message : "");
}

Player* ChatHandler::getSelectedPlayer()
{
    if (!m_session)
        return NULL;

    uint64 guid  = m_session->GetPlayer()->GetSelection();

    if (guid == 0)
        return m_session->GetPlayer();

    return ObjectAccessor::FindPlayer(guid);
}

Unit* ChatHandler::getSelectedUnit()
{
    if (!m_session)
        return NULL;

    uint64 guid = m_session->GetPlayer()->GetSelection();

    if (guid == 0)
        return m_session->GetPlayer();

    return ObjectAccessor::GetUnit(*m_session->GetPlayer(), guid);
}

WorldObject* ChatHandler::getSelectedObject()
{
    if (!m_session)
        return NULL;

    uint64 guid = m_session->GetPlayer()->GetSelection();

    if (guid == 0)
        return GetNearbyGameObject();

    return ObjectAccessor::GetUnit(*m_session->GetPlayer(), guid);
}

Creature* ChatHandler::getSelectedCreature()
{
    if (!m_session)
        return NULL;

    return ObjectAccessor::GetCreatureOrPetOrVehicle(*m_session->GetPlayer(), m_session->GetPlayer()->GetSelection());
}

char* ChatHandler::extractKeyFromLink(char* text, char const* linkType, char** something1)
{
    // skip empty
    if (!text)
        return NULL;

    // skip spaces
    while (*text == ' '||*text == '\t'||*text == '\b')
        ++text;

    if (!*text)
        return NULL;

    // return non link case
    if (text[0] != '|')
        return strtok(text, " ");

    // [name] Shift-click form |color|linkType:key|h[name]|h|r
    // or
    // [name] Shift-click form |color|linkType:key:something1:...:somethingN|h[name]|h|r

    char* check = strtok(text, "|");                        // skip color
    if (!check)
        return NULL;                                        // end of data

    char* cLinkType = strtok(NULL, ":");                    // linktype
    if (!cLinkType)
        return NULL;                                        // end of data

    if (strcmp(cLinkType, linkType) != 0)
    {
        strtok(NULL, " ");                                  // skip link tail (to allow continue strtok(NULL, s) use after retturn from function
        SendSysMessage(LANG_WRONG_LINK_TYPE);
        return NULL;
    }

    char* cKeys = strtok(NULL, "|");                        // extract keys and values
    char* cKeysTail = strtok(NULL, "");

    char* cKey = strtok(cKeys, ":|");                       // extract key
    if (something1)
        *something1 = strtok(NULL, ":|");                   // extract something

    strtok(cKeysTail, "]");                                 // restart scan tail and skip name with possible spaces
    strtok(NULL, " ");                                      // skip link tail (to allow continue strtok(NULL, s) use after return from function
    return cKey;
}

uint32 ChatHandler::GetItemIDAndBonusesFromLink(char* p_Text, std::vector<uint32>& p_Bonuses, char** p_Something)
{
    /// Skip empty
    if (!p_Text)
        return 0;

    /// Skip spaces
    while (*p_Text == ' ' || *p_Text == '\t' || *p_Text == '\b')
        ++p_Text;

    if (!*p_Text)
        return 0;

    /// Return non link case
    if (p_Text[0] != '|')
        return atoi(strtok(p_Text, " "));

    /// [name] Shift-click form |color|linkType:key|h[name]|h|r
    /// Or
    /// [name] Shift-click form |color|linkType:key:something1:...:somethingN|h[name]|h|r

    char* l_Check = strtok(p_Text, "|");                    ///< Skip color
    if (!l_Check)
        return 0;                                           ///< End of data

    char* l_CLinkType = strtok(nullptr, ":");               ///< Linktype
    if (!l_CLinkType)
        return 0;                                           ///< End of data

    if (strcmp(l_CLinkType, "Hitem") != 0)
    {
        strtok(nullptr, " ");                               ///< Skip link tail (to allow continue strtok(NULL, s) use after return from function
        SendSysMessage(LANG_WRONG_LINK_TYPE);
        return 0;
    }

    char* l_CKeys = strtok(nullptr, "|");                   ///< Extract keys and values
    char* l_CBonuses = l_CKeys;
    char* l_CKeysTail = strtok(nullptr, "");

    /// Can have different bonuses, always at the end
    char* l_CBonus = strtok(l_CBonuses, ":|");
    while (l_CBonus != nullptr)
    {
        if (l_CBonus)
        {
            if (GetItemBonusesByID(atol(l_CBonus)))
                p_Bonuses.push_back(atol(l_CBonus));
        }

        l_CBonus = strtok(nullptr, ":|");
    }

    char* l_CKey = strtok(l_CKeys, ":|");                   ///< Extract key
    if (p_Something)
        *p_Something = strtok(nullptr, ":|");               ///< Extract something

    strtok(l_CKeysTail, "]");                               ///< Restart scan tail and skip name with possible spaces
    strtok(nullptr, " ");                                   ///< Skip link tail (to allow continue strtok(NULL, s) use after return from function
    return l_CKey ? atol(l_CKey) : 0;
}

char* ChatHandler::extractKeyFromLink(char* text, char const* const* linkTypes, int* found_idx, char** something1)
{
    // skip empty
    if (!text)
        return NULL;

    // skip spaces
    while (*text == ' '||*text == '\t'||*text == '\b')
        ++text;

    if (!*text)
        return NULL;

    // return non link case
    if (text[0] != '|')
        return strtok(text, " ");

    // [name] Shift-click form |color|linkType:key|h[name]|h|r
    // or
    // [name] Shift-click form |color|linkType:key:something1:...:somethingN|h[name]|h|r
    // or
    // [name] Shift-click form |linkType:key|h[name]|h|r

    char* tail;

    if (text[1] == 'c')
    {
        char* check = strtok(text, "|");                    // skip color
        if (!check)
            return NULL;                                    // end of data

        tail = strtok(NULL, "");                            // tail
    }
    else
        tail = text+1;                                      // skip first |

    char* cLinkType = strtok(tail, ":");                    // linktype
    if (!cLinkType)
        return NULL;                                        // end of data

    for (int i = 0; linkTypes[i]; ++i)
    {
        if (strcmp(cLinkType, linkTypes[i]) == 0)
        {
            char* cKeys = strtok(NULL, "|");                // extract keys and values
            char* cKeysTail = strtok(NULL, "");

            char* cKey = strtok(cKeys, ":|");               // extract key
            if (something1)
                *something1 = strtok(NULL, ":|");           // extract something

            strtok(cKeysTail, "]");                         // restart scan tail and skip name with possible spaces
            strtok(NULL, " ");                              // skip link tail (to allow continue strtok(NULL, s) use after return from function
            if (found_idx)
                *found_idx = i;
            return cKey;
        }
    }

    strtok(NULL, " ");                                      // skip link tail (to allow continue strtok(NULL, s) use after return from function
    SendSysMessage(LANG_WRONG_LINK_TYPE);
    return NULL;
}

GameObject* ChatHandler::GetNearbyGameObject()
{
    if (!m_session)
        return NULL;

    Player* pl = m_session->GetPlayer();
    GameObject* obj = NULL;
    JadeCore::NearestGameObjectCheck check(*pl);
    JadeCore::GameObjectLastSearcher<JadeCore::NearestGameObjectCheck> searcher(pl, obj, check);
    pl->VisitNearbyGridObject(SIZE_OF_GRIDS, searcher);
    return obj;
}

GameObject* ChatHandler::GetObjectGlobalyWithGuidOrNearWithDbGuid(uint32 lowguid, uint32 entry)
{
    if (!m_session)
        return NULL;

    Player* pl = m_session->GetPlayer();

    GameObject* obj = pl->GetMap()->GetGameObject(MAKE_NEW_GUID(lowguid, entry, HIGHGUID_GAMEOBJECT));

    if (!obj && sObjectMgr->GetGOData(lowguid))                   // guid is DB guid of object
    {
        // search near player then
        CellCoord p(JadeCore::ComputeCellCoord(pl->GetPositionX(), pl->GetPositionY()));
        Cell cell(p);

        JadeCore::GameObjectWithDbGUIDCheck go_check(*pl, lowguid);
        JadeCore::GameObjectSearcher<JadeCore::GameObjectWithDbGUIDCheck> checker(pl, obj, go_check);

        TypeContainerVisitor<JadeCore::GameObjectSearcher<JadeCore::GameObjectWithDbGUIDCheck>, GridTypeMapContainer > object_checker(checker);
        cell.Visit(p, object_checker, *pl->GetMap(), *pl, pl->GetGridActivationRange());
    }

    return obj;
}

enum SpellLinkType
{
    SPELL_LINK_SPELL   = 0,
    SPELL_LINK_TALENT  = 1,
    SPELL_LINK_ENCHANT = 2,
    SPELL_LINK_TRADE   = 3,
    SPELL_LINK_GLYPH   = 4
};

static char const* const spellKeys[] =
{
    "Hspell",                                               // normal spell
    "Htalent",                                              // talent spell
    "Henchant",                                             // enchanting recipe spell
    "Htrade",                                               // profession/skill spell
    "Hglyph",                                               // glyph
    0
};

uint32 ChatHandler::extractSpellIdFromLink(char* text)
{
    // number or [name] Shift-click form |color|Henchant:recipe_spell_id|h[prof_name: recipe_name]|h|r
    // number or [name] Shift-click form |color|Hglyph:glyph_slot_id:glyph_prop_id|h[%s]|h|r
    // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r
    // number or [name] Shift-click form |color|Htalent:talent_id, rank|h[name]|h|r
    // number or [name] Shift-click form |color|Htrade:spell_id, skill_id, max_value, cur_value|h[name]|h|r
    int type = 0;
    char* param1_str = NULL;
    char* idS = extractKeyFromLink(text, spellKeys, &type, &param1_str);
    if (!idS)
        return 0;

    uint32 id = (uint32)atol(idS);

    switch (type)
    {
        case SPELL_LINK_SPELL:
            return id;
        /*case SPELL_LINK_TALENT:
        {
            // talent
            TalentEntry const* talentEntry = sTalentStore.LookupEntry(id);
            if (!talentEntry)
                return 0;

            int32 rank = param1_str ? (uint32)atol(param1_str) : 0;
            if (rank >= MAX_TALENT_RANK)
                return 0;

            if (rank < 0)
                rank = 0;

            return talentEntry->RankID[rank];
        }*/
        case SPELL_LINK_ENCHANT:
        case SPELL_LINK_TRADE:
            return id;
        case SPELL_LINK_GLYPH:
        {
            uint32 glyph_prop_id = param1_str ? (uint32)atol(param1_str) : 0;

            GlyphPropertiesEntry const* glyphPropEntry = sGlyphPropertiesStore.LookupEntry(glyph_prop_id);
            if (!glyphPropEntry)
                return 0;

            return glyphPropEntry->SpellId;
        }
    }

    // unknown type?
    return 0;
}

GameTele const* ChatHandler::extractGameTeleFromLink(char* text)
{
    // id, or string, or [name] Shift-click form |color|Htele:id|h[name]|h|r
    char* cId = extractKeyFromLink(text, "Htele");
    if (!cId)
        return NULL;

    // id case (explicit or from shift link)
    if (cId[0] >= '0' || cId[0] >= '9')
        if (uint32 id = atoi(cId))
            return sObjectMgr->GetGameTele(id);

    return sObjectMgr->GetGameTele(cId);
}

enum GuidLinkType
{
    SPELL_LINK_PLAYER     = 0,                              // must be first for selection in not link case
    SPELL_LINK_CREATURE   = 1,
    SPELL_LINK_GAMEOBJECT = 2
};

static char const* const guidKeys[] =
{
    "Hplayer",
    "Hcreature",
    "Hgameobject",
    0
};

uint64 ChatHandler::extractGuidFromLink(char* text)
{
    int type = 0;

    // |color|Hcreature:creature_guid|h[name]|h|r
    // |color|Hgameobject:go_guid|h[name]|h|r
    // |color|Hplayer:name|h[name]|h|r
    char* idS = extractKeyFromLink(text, guidKeys, &type);
    if (!idS)
        return 0;

    switch (type)
    {
        case SPELL_LINK_PLAYER:
        {
            std::string name = idS;
            if (!normalizePlayerName(name))
                return 0;

            if (Player* player = sObjectAccessor->FindPlayerByName(name.c_str()))
                return player->GetGUID();

#ifndef CROSS
            if (uint64 guid = sObjectMgr->GetPlayerGUIDByName(name))
                return guid;
#else /* CROSS */
            //if (uint64 guid = sObjectMgr->GetPlayerGUIDByName(name))
                //return guid;
#endif /* CROSS */

            return 0;
        }
        case SPELL_LINK_CREATURE:
        {
            uint32 lowguid = (uint32)atol(idS);

            if (CreatureData const* data = sObjectMgr->GetCreatureData(lowguid))
                return MAKE_NEW_GUID(lowguid, data->id, HIGHGUID_UNIT);
            else
                return 0;
        }
        case SPELL_LINK_GAMEOBJECT:
        {
            uint32 lowguid = (uint32)atol(idS);

            if (GameObjectData const* data = sObjectMgr->GetGOData(lowguid))
                return MAKE_NEW_GUID(lowguid, data->id, HIGHGUID_GAMEOBJECT);
            else
                return 0;
        }
    }

    // unknown type?
    return 0;
}

std::string ChatHandler::extractPlayerNameFromLink(char* text)
{
    // |color|Hplayer:name|h[name]|h|r
    char* name_str = extractKeyFromLink(text, "Hplayer");
    if (!name_str)
        return "";

    std::string name = name_str;
    if (!normalizePlayerName(name))
        return "";

    return name;
}

bool ChatHandler::extractPlayerTarget(char* args, Player** player, uint64* player_guid /*=NULL*/, std::string* player_name /*= NULL*/)
{
    if (args && *args)
    {
        std::string name = extractPlayerNameFromLink(args);
        if (name.empty())
        {
            SendSysMessage(LANG_PLAYER_NOT_FOUND);
            SetSentErrorMessage(true);
            return false;
        }

        Player* pl = sObjectAccessor->FindPlayerByName(name.c_str());

        // if allowed player pointer
        if (player)
            *player = pl;

        // if need guid value from DB (in name case for check player existence)
#ifndef CROSS
        uint64 guid = !pl && (player_guid || player_name) ? sObjectMgr->GetPlayerGUIDByName(name) : 0;
#else /* CROSS */
        uint64 guid = 0;// !pl && (player_guid || player_name) ? sObjectMgr->GetPlayerGUIDByName(name) : 0;
#endif /* CROSS */

        // if allowed player guid (if no then only online players allowed)
        if (player_guid)
            *player_guid = pl ? pl->GetGUID() : guid;

        if (player_name)
            *player_name = pl || guid ? name : "";
    }
    else
    {
        Player* pl = getSelectedPlayer();
        // if allowed player pointer
        if (player)
            *player = pl;
        // if allowed player guid (if no then only online players allowed)
        if (player_guid)
            *player_guid = pl ? pl->GetGUID() : 0;

        if (player_name)
            *player_name = pl ? pl->GetName() : "";
    }

    // some from req. data must be provided (note: name is empty if player not exist)
    if ((!player || !*player) && (!player_guid || !*player_guid) && (!player_name || player_name->empty()))
    {
        SendSysMessage(LANG_PLAYER_NOT_FOUND);
        SetSentErrorMessage(true);
        return false;
    }

    return true;
}

void ChatHandler::extractOptFirstArg(char* args, char** arg1, char** arg2)
{
    char* p1 = strtok(args, " ");
    char* p2 = strtok(NULL, " ");

    if (!p2)
    {
        p2 = p1;
        p1 = NULL;
    }

    if (arg1)
        *arg1 = p1;

    if (arg2)
        *arg2 = p2;
}

char* ChatHandler::extractQuotedArg(char* args)
{
    if (!*args)
        return NULL;

    if (*args == '"')
        return strtok(args+1, "\"");
    else
    {
        char* space = strtok(args, "\"");
        if (!space)
            return NULL;
        return strtok(NULL, "\"");
    }
}

bool ChatHandler::needReportToTarget(Player* chr) const
{
    Player* pl = m_session->GetPlayer();
    return pl != chr && pl->IsVisibleGloballyFor(chr);
}

LocaleConstant ChatHandler::GetSessionDbcLocale() const
{
    return m_session->GetSessionDbcLocale();
}

int ChatHandler::GetSessionDbLocaleIndex() const
{
    return m_session->GetSessionDbLocaleIndex();
}

const char *CliHandler::GetTrinityString(int32 entry) const
{
    return sObjectMgr->GetTrinityStringForDBCLocale(entry);
}

bool CliHandler::isAvailable(ChatCommand const& cmd) const
{
    // skip non-console commands in console case
    return cmd.AllowConsole;
}

void CliHandler::SendSysMessage(const char *str)
{
    m_print(m_callbackArg, str);
    m_print(m_callbackArg, "\r\n");
}

std::string CliHandler::GetNameLink() const
{
    return GetTrinityString(LANG_CONSOLE_COMMAND);
}

bool CliHandler::needReportToTarget(Player* /*chr*/) const
{
    return true;
}

bool ChatHandler::GetPlayerGroupAndGUIDByName(const char* cname, Player* &player, Group* &group, uint64 &guid, bool offline)
{
    player  = NULL;
    guid = 0;

    if (cname)
    {
        std::string name = cname;
        if (!name.empty())
        {
            if (!normalizePlayerName(name))
            {
                PSendSysMessage(LANG_PLAYER_NOT_FOUND);
                SetSentErrorMessage(true);
                return false;
            }

            player = sObjectAccessor->FindPlayerByName(name.c_str());
#ifndef CROSS
            if (offline)
                guid = sObjectMgr->GetPlayerGUIDByName(name.c_str());
#else /* CROSS */
            //if (offline)
                //guid = sObjectMgr->GetPlayerGUIDByName(name.c_str());
#endif /* CROSS */
        }
    }

    if (player)
    {
        group = player->GetGroup();
        if (!guid || !offline)
            guid = player->GetGUID();
    }
    else
    {
        if (getSelectedPlayer())
            player = getSelectedPlayer();
        else
            player = m_session->GetPlayer();

        if (!guid || !offline)
            guid  = player->GetGUID();
        group = player->GetGroup();
    }

    return true;
}

LocaleConstant CliHandler::GetSessionDbcLocale() const
{
    return sWorld->GetDefaultDbcLocale();
}

int CliHandler::GetSessionDbLocaleIndex() const
{
    return sObjectMgr->GetDBCLocaleIndex();
}
