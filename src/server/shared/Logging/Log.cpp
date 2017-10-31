////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Log.h"
#include "Common.h"
#include "Config.h"
#include "Util.h"
#include "AppenderConsole.h"
#include "AppenderFile.h"
#include "AppenderDB.h"
#include "LogOperation.h"

#include <cstdarg>
#include <cstdio>
#include <future>

Log::Log() : worker(NULL)
{
    memset(m_LogLevelTypeByFilterCache, LOG_LEVEL_DISABLED, MAX_LOG_FILTER);
    memset(m_LogTypePresentCache, 0, MAX_LOG_FILTER);

    SetRealmID(0);
    m_logsTimestamp = "_" + GetTimestampStr();
    LoadFromConfig();

    std::string ashranLogPath = m_logsDir + "ashran.log";
    ashranLog = fopen(ashranLogPath.c_str(), "a");
}

Log::~Log()
{
    Close();

    fclose(ashranLog);
    ashranLog = NULL;
}

uint8 Log::NextAppenderId()
{
    return AppenderId++;
}

int32 GetConfigIntDefault(std::string base, const char* name, int32 value)
{
    base.append(name);
    return ConfigMgr::GetIntDefault(base.c_str(), value);
}

std::string GetConfigStringDefault(std::string base, const char* name, const char* value)
{
    base.append(name);
    return ConfigMgr::GetStringDefault(base.c_str(), value);
}

Appender* Log::GetAppenderByName(std::string const& name)
{
    AppenderMap::iterator it = appenders.begin();
    while (it != appenders.end() && it->second && it->second->getName() != name)
        ++it;

    return it == appenders.end() ? NULL : it->second;
}

void Log::CreateAppenderFromConfig(const char* name)
{
    if (!name || *name == '\0')
        return;

    // Format=type,level,flags,optional1,optional2
    // if type = File. optional1 = file and option2 = mode
    // if type = Console. optional1 = Color
    std::string options = "Appender.";
    options.append(name);
    options = ConfigMgr::GetStringDefault(options.c_str(), "");
    Tokenizer tokens(options, ',');
    Tokenizer::const_iterator iter = tokens.begin();

    if (tokens.size() < 2)
    {
        fprintf(stderr, "Log::CreateAppenderFromConfig: Wrong configuration for appender %s. Config line: %s\n", name, options.c_str());
        return;
    }

    AppenderFlags flags = APPENDER_FLAGS_NONE;
    AppenderType type = AppenderType(atoi(*iter));
    ++iter;
    LogLevel level = LogLevel(atoi(*iter));
    if (level > LOG_LEVEL_FATAL)
    {
        fprintf(stderr, "Log::CreateAppenderFromConfig: Wrong Log Level %u for appender %s\n", level, name);
        return;
    }

    if (++iter != tokens.end())
        flags = AppenderFlags(atoi(*iter));

    switch (type)
    {
        case APPENDER_CONSOLE:
        {
            AppenderConsole* appender = new AppenderConsole(NextAppenderId(), name, level, flags);
            appenders[appender->getId()] = appender;
            if (++iter != tokens.end())
                appender->InitColors(*iter);
            //fprintf(stdout, "Log::CreateAppenderFromConfig: Created Appender %s (%u), Type CONSOLE, Mask %u\n", appender->getName().c_str(), appender->getId(), appender->getLogLevel()); // DEBUG - RemoveMe
            break;
        }
        case APPENDER_FILE:
        {
            std::string filename;
            std::string mode = "a";

            if (++iter == tokens.end())
            {
                fprintf(stderr, "Log::CreateAppenderFromConfig: Missing file name for appender %s\n", name);
                return;
            }

            filename = *iter;

            if (++iter != tokens.end())
                mode = *iter;

            if (flags & APPENDER_FLAGS_USE_TIMESTAMP)
            {
                size_t dot_pos = filename.find_last_of(".");
                if (dot_pos != filename.npos)
                    filename.insert(dot_pos, m_logsTimestamp);
                else
                    filename += m_logsTimestamp;
            }

            uint8 id = NextAppenderId();
            appenders[id] = new AppenderFile(id, name, level, filename.c_str(), m_logsDir.c_str(), mode.c_str(), flags);
            //fprintf(stdout, "Log::CreateAppenderFromConfig: Created Appender %s (%u), Type FILE, Mask %u, File %s, Mode %s\n", name, id, level, filename.c_str(), mode.c_str()); // DEBUG - RemoveMe
            break;
        }
        case APPENDER_DB:
        {
            uint8 id = NextAppenderId();
            appenders[id] = new AppenderDB(id, name, level, realm);
            break;
        }
        default:
            fprintf(stderr, "Log::CreateAppenderFromConfig: Unknown type %u for appender %s\n", type, name);
            break;
    }
}

void Log::CreateLoggerFromConfig(const char* name)
{
    if (!name || *name == '\0')
        return;

    LogLevel level = LOG_LEVEL_DISABLED;
    int32 type = -1;

    std::string options = "Logger.";
    options.append(name);
    options = ConfigMgr::GetStringDefault(options.c_str(), "");

    if (options.empty())
    {
        fprintf(stderr, "Log::CreateLoggerFromConfig: Missing config option Logger.%s\n", name);
        return;
    }

    Tokenizer tokens(options, ',');
    Tokenizer::const_iterator iter = tokens.begin();

    if (tokens.size() != 3)
    {
        fprintf(stderr, "Log::CreateLoggerFromConfig: Wrong config option Logger.%s=%s\n", name, options.c_str());
        return;
    }

    type = atoi(*iter);
    if (type > MaxLogFilter)
    {
        fprintf(stderr, "Log::CreateLoggerFromConfig: Wrong type %u for logger %s\n", type, name);
        return;
    }

    Logger& logger = loggers[type];
    if (!logger.getName().empty())
    {
        fprintf(stderr, "Error while configuring Logger %s. Already defined\n", name);
        return;
    }

    ++iter;
    level = LogLevel(atoi(*iter));
    if (level > LOG_LEVEL_FATAL)
    {
        fprintf(stderr, "Log::CreateLoggerFromConfig: Wrong Log Level %u for logger %s\n", type, name);
        return;
    }

    m_LogLevelTypeByFilterCache[type] = level;
    m_LogTypePresentCache[type] = 1;

    if (level < lowestLogLevel)
        lowestLogLevel = level;

    logger.Create(name, LogFilterType(type), level);
    //fprintf(stdout, "Log::CreateLoggerFromConfig: Created Logger %s, Type %u, mask %u\n", name, LogFilterType(type), level); // DEBUG - RemoveMe

    ++iter;
    std::istringstream ss(*iter);
    std::string str;

    ss >> str;
    while (ss)
    {
        if (Appender* appender = GetAppenderByName(str))
        {
            logger.addAppender(appender->getId(), appender);
            //fprintf(stdout, "Log::CreateLoggerFromConfig: Added Appender %s to Logger %s\n", appender->getName().c_str(), name); // DEBUG - RemoveMe
        }
        else
            fprintf(stderr, "Error while configuring Appender %s in Logger %s. Appender does not exist", str.c_str(), name);
        ss >> str;
    }
}

void Log::ReadAppendersFromConfig()
{
    std::istringstream ss(ConfigMgr::GetStringDefault("Appenders", ""));
    std::string name;

    do
    {
        ss >> name;
        CreateAppenderFromConfig(name.c_str());
        name = "";
    }
    while (ss);
}

void Log::ReadLoggersFromConfig()
{
    std::istringstream ss(ConfigMgr::GetStringDefault("Loggers", ""));
    std::string name;

    do
    {
        ss >> name;
        CreateLoggerFromConfig(name.c_str());
        name = "";
    }
    while (ss);

    LoggerMap::const_iterator it = loggers.begin();

    while (it != loggers.end() && it->first)
      ++it;

    // root logger must exist. Marking as disabled as its not configured
    if (it == loggers.end())
        loggers[0].Create("root", LOG_FILTER_GENERAL, LOG_LEVEL_DISABLED);
}

void Log::EnableDBAppenders()
{
    for (AppenderMap::iterator it = appenders.begin(); it != appenders.end(); ++it)
        if (it->second && it->second->getType() == APPENDER_DB)
            ((AppenderDB *)it->second)->setEnable(true);
}

void Log::vlog(LogFilterType filter, LogLevel level, char const* str, va_list argptr)
{
    char text[MAX_QUERY_LEN];
    vsnprintf(text, MAX_QUERY_LEN, str, argptr);
    write(new LogMessage(level, filter, text));
}

void Log::write(LogMessage* msg)
{
    if (worker)
    {
        msg->text.append("\n");
        Logger* logger = GetLoggerByType(msg->type);
        worker->enqueue(new LogOperation(logger, msg));
    }
    else
        delete msg;
}

std::string Log::GetTimestampStr()
{
    time_t t = time(NULL);
    tm* aTm = localtime(&t);
    //       YYYY   year
    //       MM     month (2 digits 01-12)
    //       DD     day (2 digits 01-31)
    //       HH     hour (2 digits 00-23)
    //       MM     minutes (2 digits 00-59)
    //       SS     seconds (2 digits 00-59)
    char buf[20];
    snprintf(buf, 20, "%04d-%02d-%02d_%02d-%02d-%02d", aTm->tm_year+1900, aTm->tm_mon+1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
    return std::string(buf);
}

bool Log::SetLogLevel(std::string const& name, const char* newLevelc, bool isLogger /* = true */)
{
    LogLevel newLevel = LogLevel(atoi(newLevelc));
    if (newLevel < 0)
        return false;

    if (isLogger)
    {
        LoggerMap::iterator it = loggers.begin();
        while (it != loggers.end() && it->second.getName() != name)
            ++it;

        if (it == loggers.end())
            return false;

        it->second.setLogLevel(newLevel);
        m_LogLevelTypeByFilterCache[it->second.getType()] = newLevel;
    }
    else
    {
        Appender* appender = GetAppenderByName(name);
        if (!appender)
            return false;

        appender->setLogLevel(newLevel);
    }
    return true;
}

void Log::outCommand(uint32 gm_account_id  , std::string gm_account_name,
                     uint32 gm_character_id, std::string gm_character_name,
                     uint32 sc_account_id  , std::string sc_account_name,
                     uint32 sc_character_id, std::string sc_character_name,
                     const char * str, ...)
{
    if (!str)
        return;

    GmCommand * new_command = new GmCommand;
    new_command->accountID[0]       = gm_account_id;
    new_command->accountID[1]       = sc_account_id;
    new_command->accountName[0]     = gm_account_name;
    new_command->accountName[1]     = sc_account_name;
    new_command->characterID[0]     = gm_character_id;
    new_command->characterID[1]     = sc_character_id;
    new_command->characterName[0]   = gm_character_name;
    new_command->characterName[1]   = sc_character_name;

    va_list ap;
    va_start(ap, str);
    char buffer[1024]; //buffer.
    vsprintf(buffer, str, ap);
    va_end(ap);
    new_command->command = buffer;

    GmLogQueue.add(new_command);
}

void Log::SetRealmID(uint32 id)
{
    realm = id;
}

void Log::Close()
{
    delete worker;
    worker = NULL;
    loggers.clear();
    for (AppenderMap::iterator it = appenders.begin(); it != appenders.end(); ++it)
    {
        delete it->second;
        it->second = NULL;
    }
    appenders.clear();
}

void Log::LoadFromConfig()
{
    Close();

    lowestLogLevel = LOG_LEVEL_FATAL;
    AppenderId = 0;
    worker = new LogWorker();
    m_logsDir = ConfigMgr::GetStringDefault("LogsDir", "");
    if (!m_logsDir.empty())
        if ((m_logsDir.at(m_logsDir.length() - 1) != '/') && (m_logsDir.at(m_logsDir.length() - 1) != '\\'))
            m_logsDir.push_back('/');
    ReadAppendersFromConfig();
    ReadLoggersFromConfig();

    /// Init slack
    m_SlackEnable  = ConfigMgr::GetBoolDefault("Slack.Enable", false);
    m_SlackApiUrl  = ConfigMgr::GetStringDefault("Slack.ApiUrl", "");
    m_SlackAppName = ConfigMgr::GetStringDefault("Slack.AppName", "Firestorm - WoD");
}

void Log::outGmChat( uint32 message_type,
                     uint32 from_account_id  , std::string from_account_name,
                     uint32 from_character_id, std::string from_character_name,
                     uint32 to_account_id  , std::string to_account_name,
                     uint32 to_character_id, std::string to_character_name,
                     const char * str)
{
    if (!str)
        return;

    GmChat * new_message = new GmChat;
    new_message->type               = message_type;
    new_message->accountID[0]       = from_account_id;
    new_message->accountID[1]       = to_account_id;
    new_message->accountName[0]     = from_account_name;
    new_message->accountName[1]     = to_account_name;
    new_message->characterID[0]     = from_character_id;
    new_message->characterID[1]     = to_character_id;
    new_message->characterName[0]   = from_character_name;
    new_message->characterName[1]   = to_character_name;
    new_message->message            = str;

    GmChatLogQueue.add(new_message);
}

void Log::outArena(const char * str, ...)
{
    if (!str)
        return;

    char result[MAX_QUERY_LEN];
    va_list ap;

    va_start(ap, str);
    vsnprintf(result, MAX_QUERY_LEN, str, ap);
    va_end(ap);

    std::string query = result;

    ArenaLog * log = new ArenaLog;
    log->timestamp = time(NULL);
    log->str = query;

    ArenaLogQueue.add(log);
}

void Log::outAshran(const char* str, ...)
{
    if (!str)
        return;

    char result[MAX_QUERY_LEN];
    va_list ap;

    va_start(ap, str);
    vsnprintf(result, MAX_QUERY_LEN, str, ap);
    va_end(ap);

    std::string date = GetTimestampStr();
    fprintf(ashranLog, "[%s] Ashran LOG : %s\n", date.c_str(), result);
    fflush(ashranLog);
}
