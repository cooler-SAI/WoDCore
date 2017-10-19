////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_CREATURE_TEXT_MGR_H
#define TRINITY_CREATURE_TEXT_MGR_H

#include "Creature.h"
#include "GridNotifiers.h"
#include "ObjectAccessor.h"
#include "SharedDefines.h"

struct CreatureTextEntry
{
    uint32 entry;
    uint8 group;
    uint8 id;
    std::string text;
    ChatMsg type;
    Language lang;
    float probability;
    Emote emote;
    uint32 duration;
    uint32 sound;
};

enum TextRange
{
    TEXT_RANGE_NORMAL   = 0,
    TEXT_RANGE_AREA     = 1,
    TEXT_RANGE_ZONE     = 2,
    TEXT_RANGE_MAP      = 3,
    TEXT_RANGE_WORLD    = 4
};

struct CreatureTextLocale
{
    StringVector Text;
};

struct CreatureTextId
{
    CreatureTextId(uint32 e, uint32 g, uint32 i) :  entry(e), textGroup(g), textId(i)
    {
    }

    bool operator<(CreatureTextId const& right) const
    {
        return memcmp(this, &right, sizeof(CreatureTextId)) < 0;
    }

    uint32 entry;
    uint32 textGroup;
    uint32 textId;
};

typedef std::vector<CreatureTextEntry> CreatureTextGroup;              //texts in a group
typedef ACE_Based::LockedMap<uint8, CreatureTextGroup> CreatureTextHolder;    //groups for a creature by groupid
typedef ACE_Based::LockedMap<uint32, CreatureTextHolder> CreatureTextMap;     //all creatures by entry

typedef ACE_Based::LockedMap<CreatureTextId, CreatureTextLocale> LocaleCreatureTextMap;

//used for handling non-repeatable random texts
typedef std::vector<uint8> CreatureTextRepeatIds;
typedef ACE_Based::LockedMap<uint8, CreatureTextRepeatIds> CreatureTextRepeatGroup;
typedef ACE_Based::LockedMap<uint64, CreatureTextRepeatGroup> CreatureTextRepeatMap;//guid based

class CreatureTextMgr
{
    friend class ACE_Singleton<CreatureTextMgr, ACE_Null_Mutex>;
    CreatureTextMgr() {};

    public:
        ~CreatureTextMgr() {};
        void LoadCreatureTexts();
        void LoadCreatureTextLocales();
        CreatureTextMap  const& GetTextMap() const { return mTextMap; }

        void SendSound(Creature* source, uint32 sound, ChatMsg msgType, uint64 whisperGuid, TextRange range, Team team, bool gmOnly);
        void SendEmote(Unit* source, uint32 emote);

        //if sent, returns the 'duration' of the text else 0 if error
        uint32 SendChat(Creature* source, uint8 textGroup, uint64 whisperGuid = 0, ChatMsg msgType = CHAT_MSG_ADDON, Language language = LANG_ADDON, TextRange range = TEXT_RANGE_NORMAL, uint32 sound = 0, Team team = TEAM_OTHER, bool gmOnly = false, Player* srcPlr = NULL);
        bool TextExist(uint32 sourceEntry, uint8 textGroup);
        std::string GetLocalizedChatString(uint32 entry, uint8 textGroup, uint32 id, LocaleConstant locale) const;

        template<class Builder>
        void SendChatPacket(WorldObject* source, Builder const& builder, ChatMsg msgType, uint64 whisperGuid = 0, TextRange range = TEXT_RANGE_NORMAL, Team team = TEAM_OTHER, bool gmOnly = false) const;
    private:
        CreatureTextRepeatIds GetRepeatGroup(Creature* source, uint8 textGroup);
        void SetRepeatId(Creature* source, uint8 textGroup, uint8 id);

        void SendNonChatPacket(WorldObject* source, WorldPacket* data, ChatMsg msgType, uint64 whisperGuid, TextRange range, Team team, bool gmOnly) const;
        float GetRangeForChatType(ChatMsg msgType) const;

        CreatureTextMap mTextMap;
        CreatureTextRepeatMap mTextRepeatMap;
        LocaleCreatureTextMap mLocaleTextMap;
};

#define sCreatureTextMgr ACE_Singleton<CreatureTextMgr, ACE_Null_Mutex>::instance()

template<class Builder>
class CreatureTextLocalizer
{
    public:
        CreatureTextLocalizer(Builder const& builder, ChatMsg msgType) : _builder(builder), _msgType(msgType)
        {
            _packetCache.resize(TOTAL_LOCALES, NULL);
        }

        ~CreatureTextLocalizer()
        {
            for (size_t i = 0; i < _packetCache.size(); ++i)
            {
                delete _packetCache[i];
            }
        }

        void operator()(Player* player)
        {
            LocaleConstant loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
            WorldPacket* messageTemplate;

            uint64 tguid = 0LL;
            switch (_msgType)
            {
                case CHAT_MSG_MONSTER_WHISPER:
                case CHAT_MSG_RAID_BOSS_WHISPER:
                    tguid = player->GetGUID();
                    break;
                default:
                    break;
            }
            // create if not cached yet
            if (!_packetCache[loc_idx])
            {
                messageTemplate = new WorldPacket(SMSG_CHAT, 200);
                _builder(messageTemplate, loc_idx, tguid);
                _packetCache[loc_idx] = messageTemplate;
            }
            else
                messageTemplate = _packetCache[loc_idx];

            WorldPacket data(*messageTemplate);

            player->SendDirectMessage(&data);
        }

    private:
        std::vector<WorldPacket*> _packetCache;
        Builder const& _builder;
        ChatMsg _msgType;
};

template<class Builder>
void CreatureTextMgr::SendChatPacket(WorldObject* source, Builder const& builder, ChatMsg msgType, uint64 whisperGuid, TextRange range, Team team, bool gmOnly) const
{
    if (!source)
        return;

    CreatureTextLocalizer<Builder> localizer(builder, msgType);

    switch (msgType)
    {
        case CHAT_MSG_MONSTER_WHISPER:
        case CHAT_MSG_RAID_BOSS_WHISPER:
        {
            if (range == TEXT_RANGE_NORMAL) //ignores team and gmOnly
            {
                Player* player = ObjectAccessor::FindPlayer(whisperGuid);
                if (!player || !player->GetSession())
                    return;

                localizer(player);
                return;
            }
            break;
        }
        default:
            break;
    }

    switch (range)
    {
        case TEXT_RANGE_AREA:
        {
            uint32 areaId = source->GetAreaId();
            Map::PlayerList const& players = source->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                if (itr->getSource()->GetAreaId() == areaId && itr->getSource()->InSamePhase(source) && (!team || Team(itr->getSource()->GetTeam()) == team) && (!gmOnly || itr->getSource()->isGameMaster()))
                    localizer(itr->getSource());
            return;
        }
        case TEXT_RANGE_ZONE:
        {
            uint32 zoneId = source->GetZoneId();
            Map::PlayerList const& players = source->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                if (itr->getSource()->GetZoneId() == zoneId && (!team || Team(itr->getSource()->GetTeam()) == team) && (!gmOnly || itr->getSource()->isGameMaster()))
                    localizer(itr->getSource());
            return;
        }
        case TEXT_RANGE_MAP:
        {
            Map::PlayerList const& players = source->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                if ((!team || Team(itr->getSource()->GetTeam()) == team) && (!gmOnly || itr->getSource()->isGameMaster()))
                    localizer(itr->getSource());
            return;
        }
        case TEXT_RANGE_WORLD:
        {
#ifndef CROSS
            SessionMap const& smap = sWorld->GetAllSessions();
            for (SessionMap::const_iterator iter = smap.begin(); iter != smap.end(); ++iter)
                if (Player* player = iter->second->GetPlayer())
#else /* CROSS */
            PlayerMap const& players = sWorld->GetAllPlayers();
            for (PlayerMap::const_iterator iter = players.begin(); iter != players.end(); ++iter)
            {
                if (Player* player = iter->second)
#endif /* CROSS */
                    if (player->GetSession()  && (!team || Team(player->GetTeam()) == team) && (!gmOnly || player->isGameMaster()))
                        localizer(player);
#ifdef CROSS
            }
#endif /* CROSS */
            return;
        }
        case TEXT_RANGE_NORMAL:
        default:
            break;
    }

    float dist = GetRangeForChatType(msgType);
    JadeCore::PlayerDistWorker<CreatureTextLocalizer<Builder> > worker(source, dist, localizer);
    source->VisitNearbyWorldObject(dist, worker);
}

#endif
