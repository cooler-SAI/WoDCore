////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef COMMON_H
#define COMMON_H

// config.h needs to be included 1st
// TODO this thingy looks like hack, but its not, need to
// make separate header however, because It makes mess here.
#ifdef HAVE_CONFIG_H
// Remove Some things that we will define
// This is in case including another config.h
// before trinity config.h
#ifdef PACKAGE
#undef PACKAGE
#endif //PACKAGE
#ifdef PACKAGE_BUGREPORT
#undef PACKAGE_BUGREPORT
#endif //PACKAGE_BUGREPORT
#ifdef PACKAGE_NAME
#undef PACKAGE_NAME
#endif //PACKAGE_NAME
#ifdef PACKAGE_STRING
#undef PACKAGE_STRING
#endif //PACKAGE_STRING
#ifdef PACKAGE_TARNAME
#undef PACKAGE_TARNAME
#endif //PACKAGE_TARNAME
#ifdef PACKAGE_VERSION
#undef PACKAGE_VERSION
#endif //PACKAGE_VERSION
#ifdef VERSION
#undef VERSION
#endif //VERSION

# include "Config.h"

#undef PACKAGE
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef VERSION
#endif //HAVE_CONFIG_H

#include "Define.h"

#include <unordered_map>
#include <unordered_set>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include <time.h>
#include <cmath>
#include <math.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <random>
#include <limits>

#if PLATFORM == PLATFORM_WINDOWS
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

#include <set>
#include <list>
#include <string>
#include <map>
#include <queue>
#include <sstream>
#include <forward_list>

#include <iostream>     ///< std::cout
#include <functional>   ///< std::less
#include <algorithm>    ///< std::sort, std::includes

#include <atomic>
#include <mutex>
#include <thread>
#include <array>
#include <vector>
#include "Threading/LockedQueue.h"
#include "Threading/Threading.h"

#include <ace/Basic_Types.h>
#include <ace/Guard_T.h>
#include <ace/RW_Thread_Mutex.h>
#include <ace/Thread_Mutex.h>
#include <ace/Singleton.h>

/// MSVC 18 (2013) have only partial support of thread_local (c++11)
#ifdef __GNUC__
# define thread_local __thread
#elif __STDC_VERSION__ >= 201112L
# define thread_local _Thread_local
#elif defined(_MSC_VER)
# define thread_local __declspec(thread)
#endif

#if PLATFORM == PLATFORM_WINDOWS
#  include <ace/config-all.h>
// XP winver - needed to compile with standard leak check in MemoryLeaks.h
// uncomment later if needed
//#define _WIN32_WINNT 0x0501
#  include <ws2tcpip.h>
//#undef WIN32_WINNT
#else
#  include <sys/types.h>
#  include <sys/ioctl.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <unistd.h>
#  include <netdb.h>
#endif

#if COMPILER == COMPILER_MICROSOFT

#include <float.h>

#define I32FMT "%08I32X"
#define I64FMT "%016I64X"
#define snprintf _snprintf
#define atoll _atoi64
#define vsnprintf _vsnprintf
#define llabs _abs64

#else

#define stricmp strcasecmp
#define strnicmp strncasecmp
#define I32FMT "%08X"
#define I64FMT "%016llX"

#endif

inline float finiteAlways(float f) { return std::isfinite(f) ? f : 0.0f; }

#define atol(a) strtoul( a, NULL, 10)

#define STRINGIZE(a) #a

enum TimeConstants
{
    MINUTE          = 60,
    HOUR            = MINUTE*60,
    DAY             = HOUR*24,
    WEEK            = DAY*7,
    MONTH           = DAY*30,
    YEAR            = MONTH*12,
    IN_MILLISECONDS = 1000
};

enum AccountTypes
{
    SEC_PLAYER                = 0,
    SEC_MODERATOR             = 1,
    SEC_GAMEMASTER            = 2,
    SEC_CONFIRMED_GAMEMASTER  = 3,
    SEC_REALM_LEADER          = 4,
    SEC_GM_LEADER             = 5,
    SEC_ADMINISTRATOR         = 6,
    SEC_CONSOLE               = 7                                  // must be always last in list, accounts must have less security level always also
};

enum LocaleConstant
{
    LOCALE_enUS =  0,
    LOCALE_koKR =  1,
    LOCALE_frFR =  2,
    LOCALE_deDE =  3,
    LOCALE_zhCN =  4,
    LOCALE_zhTW =  5,
    LOCALE_esES =  6,
    LOCALE_esMX =  7,
    LOCALE_ruRU =  8,
    LOCALE_ptPT =  9,
    LOCALE_itIT = 10,
};

const uint8 TOTAL_LOCALES = 11;
const LocaleConstant DEFAULT_LOCALE = LOCALE_enUS;

#define MAX_LOCALES TOTAL_LOCALES
#define MAX_ACCOUNT_TUTORIAL_VALUES 8

extern char const* localeNames[TOTAL_LOCALES];

LocaleConstant GetLocaleByName(const std::string& name);

typedef std::vector<std::string> StringVector;

#pragma pack(push, 1)

static char const* g_EmptyStr = "";

struct LocalizedString
{
    char const* Str[TOTAL_LOCALES];

    LocalizedString()
    {
        for (uint32 l_I = 0; l_I < TOTAL_LOCALES; l_I++)
            Str[l_I] = "";
    }

    LocalizedString(char const* p_DefaultValue)
    {
        for (uint32 l_I = 0; l_I < TOTAL_LOCALES; l_I++)
            Str[l_I] = p_DefaultValue;
    }

    inline char const* Get(uint32 p_Locale) const
    {
        if ((Str[p_Locale] == nullptr || strlen(Str[p_Locale]) == 0) && Str[LOCALE_enUS] != nullptr)
            return Str[LOCALE_enUS];

        if (Str[p_Locale] == nullptr)
            return g_EmptyStr;

        return Str[p_Locale];
    }
};

#pragma pack(pop)

enum GM_COMMAND_TAB
{
    GM,
    PLAYER
};

struct GmCommand
{
    uint32 accountID[2];
    std::string accountName[2];
    uint32 characterID[2];
    std::string characterName[2];
    std::string command;
};

struct GmChat
{
    uint32 type;
    uint32 accountID[2];
    std::string accountName[2];
    uint32 characterID[2];
    std::string characterName[2];
    std::string message;
};

struct ArenaLog
{
    uint32 timestamp;
    std::string str;
};

extern ACE_Based::LockedQueue<GmCommand*, ACE_Thread_Mutex> GmLogQueue;
extern ACE_Based::LockedQueue<GmChat*,    ACE_Thread_Mutex> GmChatLogQueue;
extern ACE_Based::LockedQueue<ArenaLog*,  ACE_Thread_Mutex> ArenaLogQueue;

// we always use stdlibc++ std::max/std::min, undefine some not C++ standard defines (Win API and some other platforms)
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#ifndef M_PI
#define M_PI            3.14159265358979323846f
#endif

#define MAX_QUERY_LEN 32*1024

#define TRINITY_GUARD(MUTEX, LOCK) \
  ACE_Guard< MUTEX > TRINITY_GUARD_OBJECT (LOCK); \
    if (TRINITY_GUARD_OBJECT.locked() == 0) ASSERT(false);

//! For proper implementation of multiple-read, single-write pattern, use
//! ACE_RW_Mutex as underlying @MUTEX
# define TRINITY_WRITE_GUARD(MUTEX, LOCK) \
  ACE_Write_Guard< MUTEX > TRINITY_GUARD_OBJECT (LOCK); \
    if (TRINITY_GUARD_OBJECT.locked() == 0) ASSERT(false);

//! For proper implementation of multiple-read, single-write pattern, use
//! ACE_RW_Mutex as underlying @MUTEX
# define TRINITY_READ_GUARD(MUTEX, LOCK) \
  ACE_Read_Guard< MUTEX > TRINITY_GUARD_OBJECT (LOCK); \
    if (TRINITY_GUARD_OBJECT.locked() == 0) ASSERT(false);

namespace std
{
    template<class K, class V>
    struct hash<std::pair<K, V>>
    {
    public:
        size_t operator()(std::pair<K, V> const& key) const
        {
            return std::hash<K>()(key.first) ^ std::hash<V>()(key.second);
        }
    };
}

#endif
