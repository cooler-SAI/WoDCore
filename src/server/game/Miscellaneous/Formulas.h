////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_FORMULAS_H
#define TRINITY_FORMULAS_H

#include "World.h"
#include "SharedDefines.h"
#include "ScriptMgr.h"

namespace JadeCore
{
    namespace Honor
    {
        inline float hk_honor_at_level_f(uint8 level, float multiplier = 1.0f)
        {
            float honor = multiplier * level * 1.55f;
            sScriptMgr->OnHonorCalculation(honor, level, multiplier);
            return honor;
        }

        inline uint32 hk_honor_at_level(uint8 level, float multiplier = 1.0f)
        {
            return uint32(ceil(hk_honor_at_level_f(level, multiplier)));
        }
    }
    namespace XP
    {
        inline uint8 GetGrayLevel(uint8 pl_level)
        {
            uint8 level;

            if (pl_level <= 5)
                level = 0;
            else if (pl_level <= 39)
                level = pl_level - 5 - pl_level / 10;
            else if (pl_level <= 59)
                level = pl_level - 1 - pl_level / 5;
            else
                level = pl_level - 9;

            sScriptMgr->OnGrayLevelCalculation(level, pl_level);
            return level;
        }

        inline XPColorChar GetColorCode(uint8 pl_level, uint8 mob_level)
        {
            XPColorChar color;

            if (mob_level >= pl_level + 5)
                color = XP_RED;
            else if (mob_level >= pl_level + 3)
                color = XP_ORANGE;
            else if (mob_level >= pl_level - 2)
                color = XP_YELLOW;
            else if (mob_level > GetGrayLevel(pl_level))
                color = XP_GREEN;
            else
                color = XP_GRAY;

            sScriptMgr->OnColorCodeCalculation(color, pl_level, mob_level);
            return color;
        }

        inline uint8 GetZeroDifference(uint8 pl_level)
        {
            uint8 diff;

            if (pl_level < 8)
                diff = 5;
            else if (pl_level < 10)
                diff = 6;
            else if (pl_level < 12)
                diff = 7;
            else if (pl_level < 16)
                diff = 8;
            else if (pl_level < 20)
                diff = 9;
            else if (pl_level < 30)
                diff = 11;
            else if (pl_level < 40)
                diff = 12;
            else if (pl_level < 45)
                diff = 13;
            else if (pl_level < 50)
                diff = 14;
            else if (pl_level < 55)
                diff = 15;
            else if (pl_level < 60)
                diff = 16;
            else
                diff = 17;

            sScriptMgr->OnZeroDifferenceCalculation(diff, pl_level);
            return diff;
        }

        inline uint32 BaseGain(uint8 pl_level, uint8 mob_level, ContentLevels content)
        {
            uint32 baseGain;
            uint32 nBaseExp;

            switch (content)
            {
                case CONTENT_1_60:
                    nBaseExp = 35;
                    break;
                case CONTENT_61_70:
                    nBaseExp = 35;
                    break;
                case CONTENT_71_80:
                    nBaseExp = 35;
                    break;
                case CONTENT_81_85:
                    nBaseExp = 35;
                    break;
                case CONTENT_86_90:
                    nBaseExp = 35;
                    break;
                case CONTENT_91_100:
                    nBaseExp = 40;
                    break;
                default:
                    sLog->outError(LOG_FILTER_GENERAL, "BaseGain: Unsupported content level %u", content);
                    nBaseExp = 45;
                    break;
            }

            if (mob_level >= pl_level)
            {
                uint8 nLevelDiff = mob_level - pl_level;
                if (nLevelDiff > 6)
                    baseGain = 0;
                else
                {
                    if (nLevelDiff > 4)
                        nLevelDiff = 4;

                    baseGain = ((pl_level * 5 + nBaseExp) * (20 + nLevelDiff) / 10 + 1) / 2;
                }
            }
            else
            {
                uint8 gray_level = GetGrayLevel(pl_level);
                if (mob_level > gray_level)
                {
                    uint8 ZD = GetZeroDifference(pl_level);
                    baseGain = (pl_level * 5 + nBaseExp) * (ZD + mob_level - pl_level) / ZD;
                }
                else
                    baseGain = 0;
            }

            sScriptMgr->OnBaseGainCalculation(baseGain, pl_level, mob_level, content);
            return baseGain;
        }

        inline uint32 Gain(Player* player, Unit* u)
        {
            uint32 gain;

            if (u->GetTypeId() == TYPEID_UNIT &&
                (((Creature*)u)->isTotem() || ((Creature*)u)->isPet() ||
                (((Creature*)u)->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_NO_XP_AT_KILL) ||
                ((Creature*)u)->GetCreatureTemplate()->type == CREATURE_TYPE_CRITTER))
                gain = 0;
            else
            {
                gain = BaseGain(player->getLevel(), u->getLevel(), GetContentLevelsForMapAndZone(u->GetMapId(), u->GetZoneId()));

                if (gain != 0 && u->GetTypeId() == TYPEID_UNIT && ((Creature*)u)->isElite())
                {
                    // Elites in instances have a 2.75x XP bonus instead of the regular 2x world bonus.
                    if (u->GetMap() && u->GetMap()->IsDungeon())
                       gain = uint32(gain * 2.75);
                    else
                        gain *= 2;
                }

                float KillXpRate = 1;

                if (player->GetPersonnalXpRate())
                    KillXpRate = player->GetPersonnalXpRate();
                else
                    KillXpRate = sWorld->getRate(RATE_XP_KILL);

                gain = uint32(gain * KillXpRate);

                float premium_rate = player->GetSession()->IsPremium() ? sWorld->getRate(RATE_XP_KILL_PREMIUM) : 1.0f;
                gain *= premium_rate;
            }

            sScriptMgr->OnGainCalculation(gain, player, u);
            return gain;
        }

        inline float xp_in_group_rate(uint32 count, bool isRaid)
        {
            float rate;

            if (isRaid)
            {
                // FIXME: Must apply decrease modifiers depending on raid size.
                rate = 1.0f;
            }
            else
            {
                switch (count)
                {
                    case 0:
                    case 1:
                    case 2:
                        rate = 1.0f;
                        break;
                    case 3:
                        rate = 1.166f;
                        break;
                    case 4:
                        rate = 1.3f;
                        break;
                    case 5:
                    default:
                        rate = 1.4f;
                }
            }

            sScriptMgr->OnGroupRateCalculation(rate, count, isRaid);
            return rate;
        }
    }

    namespace Currency
    {
        inline uint32 ConquestRatingCalculator(uint32 rate)
        {
            if (rate <= 1500)
                return 1800; // Default conquest points
            else if (rate > 3000)
                rate = 3600;

            // http://www.arenajunkies.com/topic/179536-conquest-point-cap-vs-personal-rating-chart/page__st__60#entry3085246
            return uint32(1.4326 * ((1511.26 / (1 + 1639.28 / exp(0.00412 * rate))) + 1050.15));
        }

        inline uint32 BgConquestRatingCalculator(uint32 rate)
        {
            // WowWiki: Battleground ratings receive a bonus of 22.2% to the cap they generate
            return uint32((ConquestRatingCalculator(rate) * 1.222f) + 0.5f);
        }
    }
}

#endif
