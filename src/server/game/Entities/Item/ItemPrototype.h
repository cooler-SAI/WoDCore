////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ITEMPROTOTYPE_H
#define _ITEMPROTOTYPE_H

#include "Common.h"
#include "SharedDefines.h"
#include "DBCStores.h"

enum ItemModType
{
    ITEM_MOD_MANA                     = 0,
    ITEM_MOD_HEALTH                   = 1,
    ITEM_MOD_AGILITY                  = 3,
    ITEM_MOD_STRENGTH                 = 4,
    ITEM_MOD_INTELLECT                = 5,
    ITEM_MOD_SPIRIT                   = 6,
    ITEM_MOD_STAMINA                  = 7,
    ITEM_MOD_DEFENSE_SKILL_RATING     = 12, // Deprecated
    ITEM_MOD_DODGE_RATING             = 13,
    ITEM_MOD_PARRY_RATING             = 14,
    ITEM_MOD_BLOCK_RATING             = 15,
    ITEM_MOD_HIT_MELEE_RATING         = 16, // Deprecated
    ITEM_MOD_HIT_RANGED_RATING        = 17, // Deprecated
    ITEM_MOD_HIT_SPELL_RATING         = 18, // Deprecated
    ITEM_MOD_CRIT_MELEE_RATING        = 19, // Deprecated, merged in ITEM_MOD_CRIT_RATING
    ITEM_MOD_CRIT_RANGED_RATING       = 20, // Deprecated, merged in ITEM_MOD_CRIT_RATING
    ITEM_MOD_CRIT_SPELL_RATING        = 21, // Deprecated, merged in ITEM_MOD_CRIT_RATING
    ITEM_MOD_HIT_TAKEN_MELEE_RATING   = 22, // Deprecated
    ITEM_MOD_HIT_TAKEN_RANGED_RATING  = 23, // Deprecated
    ITEM_MOD_HIT_TAKEN_SPELL_RATING   = 24, // Deprecated
    ITEM_MOD_CRIT_TAKEN_MELEE_RATING  = 25, // Deprecated
    ITEM_MOD_CRIT_TAKEN_RANGED_RATING = 26, // Deprecated
    ITEM_MOD_CRIT_TAKEN_SPELL_RATING  = 27, // Deprecated
    ITEM_MOD_HASTE_MELEE_RATING       = 28, // Deprecated, merged in ITEM_MOD_HASTE_RATING
    ITEM_MOD_HASTE_RANGED_RATING      = 29, // Deprecated, merged in ITEM_MOD_HASTE_RATING
    ITEM_MOD_HASTE_SPELL_RATING       = 30, // Deprecated, merged in ITEM_MOD_HASTE_RATING
    ITEM_MOD_HIT_RATING               = 31, // Deprecated
    ITEM_MOD_CRIT_RATING              = 32,
    ITEM_MOD_HIT_TAKEN_RATING         = 33, // Deprecated
    ITEM_MOD_CRIT_TAKEN_RATING        = 34, // Deprecated
    ITEM_MOD_RESILIENCE_RATING        = 35,
    ITEM_MOD_HASTE_RATING             = 36,
    ITEM_MOD_EXPERTISE_RATING         = 37, // Deprecated
    ITEM_MOD_ATTACK_POWER             = 38,
    ITEM_MOD_RANGED_ATTACK_POWER      = 39, // Deprecated, merged in ITEM_MOD_ATTACK_POWER
    ITEM_MOD_VERSATILITY_RATING       = 40,
    ITEM_MOD_SPELL_HEALING_DONE       = 41, // Deprecated
    ITEM_MOD_SPELL_DAMAGE_DONE        = 42, // Deprecated
    ITEM_MOD_MANA_REGENERATION        = 43,
    ITEM_MOD_ARMOR_PENETRATION_RATING = 44, // Deprecated
    ITEM_MOD_SPELL_POWER              = 45,
    ITEM_MOD_HEALTH_REGEN             = 46,
    ITEM_MOD_SPELL_PENETRATION        = 47, // Deprecated
    ITEM_MOD_BLOCK_VALUE              = 48, // Deprecated
    ITEM_MOD_MASTERY_RATING           = 49,
    ITEM_MOD_EXTRA_ARMOR              = 50,
    ITEM_MOD_FIRE_RESISTANCE          = 51,
    ITEM_MOD_FROST_RESISTANCE         = 52,
    ITEM_MOD_HOLY_RESISTANCE          = 53,
    ITEM_MOD_SHADOW_RESISTANCE        = 54,
    ITEM_MOD_NATURE_RESISTANCE        = 55,
    ITEM_MOD_ARCANE_RESISTANCE        = 56,
    ITEM_MOD_PVP_POWER                = 57,
    ITEM_MOD_CR_AMPLIFY               = 58,
    ITEM_MOD_MULTISTRIKE_RATING       = 59,
    ITEM_MOD_READINESS_RATING         = 60, // Deprecated
    ITEM_MOD_SPEED_RATING             = 61,
    ITEM_MOD_LEECH_RATING             = 62,
    ITEM_MOD_AVOIDANCE_RATING         = 63,
    ITEM_MOD_CR_STURDINESS            = 64,
    ITEM_MOD_CR_UNUSED_7              = 65,
    ITEM_MOD_CR_CLEAVE                = 66,
    ITEM_MOD_CR_UNUSED_9              = 67,
    ITEM_MOD_CR_UNUSED_10             = 68,
    ITEM_MOD_CR_UNUSED_11             = 69,
    ITEM_MOD_CR_UNUSED_12             = 70,
    ITEM_MOD_DYNAMIC_STAT_AGI_STR_INT = 71,
    ITEM_MOD_DYNAMIC_STAT_AGI_STR     = 72,
    ITEM_MOD_DYNAMIC_STAT_AGI_INT     = 73,
    ITEM_MOD_DYNAMIC_STAT_STR_INT     = 74,
};

#define MAX_ITEM_MOD                    75

enum ItemSpelltriggerType
{
    ITEM_SPELLTRIGGER_ON_USE          = 0,                  // use after equip cooldown
    ITEM_SPELLTRIGGER_ON_EQUIP        = 1,
    ITEM_SPELLTRIGGER_CHANCE_ON_HIT   = 2,
    ITEM_SPELLTRIGGER_SOULSTONE       = 4,
    /*
     * ItemSpelltriggerType 5 might have changed on 2.4.3/3.0.3: Such auras
     * will be applied on item pickup and removed on item loss - maybe on the
     * other hand the item is destroyed if the aura is removed ("removed on
     * death" of spell 57348 makes me think so)
     */
    ITEM_SPELLTRIGGER_ON_NO_DELAY_USE = 5,                  // no equip cooldown
    ITEM_SPELLTRIGGER_LEARN_SPELL_ID  = 6                   // used in item_template.spell_2 with spell_id with SPELL_GENERIC_LEARN in spell_1
};

#define MAX_ITEM_SPELLTRIGGER           7

enum ItemBondingType
{
    NO_BIND                                     = 0,
    BIND_WHEN_PICKED_UP                         = 1,
    BIND_WHEN_EQUIPED                           = 2,
    BIND_WHEN_USE                               = 3,
    BIND_QUEST_ITEM                             = 4,
    BIND_QUEST_ITEM1                            = 5         // not used in game
};

#define MAX_BIND_TYPE                             6

enum ItemFieldFlags
{
    ITEM_FIELD_FLAG_SOULBOUND     = 0x00000001, // Item is soulbound and cannot be traded <<--
    ITEM_FIELD_FLAG_TRANSLATED    = 0x00000002, // Item text will not read as garbage when player does not know the language
    ITEM_FIELD_FLAG_UNLOCKED      = 0x00000004, // Item had lock but can be opened now
    ITEM_FIELD_FLAG_WRAPPED       = 0x00000008, // Item is wrapped and contains another item
    ITEM_FIELD_FLAG_DISABLE       = 0x00000010, // Item is disable (red filter)
    ITEM_FIELD_FLAG_UNK3          = 0x00000020, // ?
    ITEM_FIELD_FLAG_UNK4          = 0x00000040, // ?
    ITEM_FIELD_FLAG_UNK5          = 0x00000080, // ?
    ITEM_FIELD_FLAG_BOP_TRADEABLE = 0x00000100, // Allows trading soulbound items
    ITEM_FIELD_FLAG_READABLE      = 0x00000200, // Opens text page when right clicked
    ITEM_FIELD_FLAG_UNK6          = 0x00000400, // ?
    ITEM_FIELD_FLAG_UNK7          = 0x00000800, // ?
    ITEM_FIELD_FLAG_REFUNDABLE    = 0x00001000, // Item can be returned to vendor for its original cost (extended cost)
    ITEM_FIELD_FLAG_UNK8          = 0x00002000, // ?
    ITEM_FIELD_FLAG_UNK9          = 0x00004000, // ?
    ITEM_FIELD_FLAG_UNK10         = 0x00008000, // ?
    ITEM_FIELD_FLAG_UNK11         = 0x00010000, // ?
    ITEM_FIELD_FLAG_UNK12         = 0x00020000, // ?
    ITEM_FIELD_FLAG_UNK13         = 0x00040000, // ?
    ITEM_FIELD_FLAG_UNK14         = 0x00080000, // ?
    ITEM_FIELD_FLAG_UNK15         = 0x00100000, // ?
    ITEM_FIELD_FLAG_UNK16         = 0x00200000, // ?
    ITEM_FIELD_FLAG_UNK17         = 0x00400000, // ?
    ITEM_FIELD_FLAG_UNK18         = 0x00800000, // ?
    ITEM_FIELD_FLAG_UNK19         = 0x01000000, // ?
    ITEM_FIELD_FLAG_UNK20         = 0x02000000, // ?
    ITEM_FIELD_FLAG_UNK21         = 0x04000000, // ?
    ITEM_FIELD_FLAG_UNK22         = 0x08000000, // ?
    ITEM_FIELD_FLAG_UNK23         = 0x10000000, // ?
    ITEM_FIELD_FLAG_UNK24         = 0x20000000, // ?
    ITEM_FIELD_FLAG_UNK25         = 0x40000000, // ?
    ITEM_FIELD_FLAG_UNK26         = 0x80000000, // ?

    ITEM_FIELD_FLAG_MAIL_TEXT_MASK = ITEM_FIELD_FLAG_READABLE | ITEM_FIELD_FLAG_UNK13 | ITEM_FIELD_FLAG_UNK14,
};

enum ItemFlags
{
    ITEM_FLAG_UNK1                        = 0x00000001, // ?
    ITEM_FLAG_CONJURED                    = 0x00000002, // Conjured item
    ITEM_FLAG_OPENABLE                    = 0x00000004, // Item can be right clicked to open for loot
    ITEM_FLAG_HEROIC                      = 0x00000008, // Makes green "Heroic" text appear on item
    ITEM_FLAG_DEPRECATED                  = 0x00000010, // Cannot equip or use
    ITEM_FLAG_INDESTRUCTIBLE              = 0x00000020, // Item can not be destroyed, except by using spell (item can be reagent for spell)
    ITEM_FLAG_UNK2                        = 0x00000040, // ?
    ITEM_FLAG_NO_EQUIP_COOLDOWN           = 0x00000080, // No default 30 seconds cooldown when equipped
    ITEM_FLAG_UNK3                        = 0x00000100, // ?
    ITEM_FLAG_WRAPPER                     = 0x00000200, // Item can wrap other items
    ITEM_FLAG_IGNORE_BAG_SPACE            = 0x00000400, ///< ? source: SimulationCraft
    ITEM_FLAG_PARTY_LOOT                  = 0x00000800, // Looting this item does not remove it from available loot
    ITEM_FLAG_REFUNDABLE                  = 0x00001000, // Item can be returned to vendor for its original cost (extended cost)
    ITEM_FLAG_CHARTER                     = 0x00002000, // Item is guild or arena charter
    ITEM_FLAG_UNK5                        = 0x00004000, // Only readable items have this (but not all)
    ITEM_FLAG_UNK6                        = 0x00008000, // ?
    ITEM_FLAG_UNK7                        = 0x00010000, // ?
    ITEM_FLAG_UNK8                        = 0x00020000, // ?
    ITEM_FLAG_PROSPECTABLE                = 0x00040000, // Item can be prospected
    ITEM_FLAG_UNIQUE_EQUIPPED             = 0x00080000, // You can only equip one of these
    ITEM_FLAG_UNK9                        = 0x00100000, // ?
    ITEM_FLAG_USEABLE_IN_ARENA            = 0x00200000, // Item can be used during arena match
    ITEM_FLAG_THROWABLE                   = 0x00400000, // Some Thrown weapons have it (and only Thrown) but not all
    ITEM_FLAG_USABLE_WHEN_SHAPESHIFTED    = 0x00800000, // Item can be used in shapeshift forms
    ITEM_FLAG_UNK10                       = 0x01000000, // ?
    ITEM_FLAG_SMART_LOOT                  = 0x02000000, // Profession recipes: can only be looted if you meet requirements and don't already know it
    ITEM_FLAG_NOT_USEABLE_IN_ARENA        = 0x04000000, // Item cannot be used in arena
    ITEM_FLAG_BIND_TO_ACCOUNT             = 0x08000000, // Item binds to account and can be sent only to your own characters
    ITEM_FLAG_TRIGGERED_CAST              = 0x10000000, // Spell is cast with triggered flag
    ITEM_FLAG_MILLABLE                    = 0x20000000, // Item can be milled
    ITEM_FLAG_UNK11                       = 0x40000000, // ?
    ITEM_FLAG_BOP_TRADEABLE               = 0x80000000  // bound item that can be traded
};

enum ItemFlags2
{
    ITEM_FLAG2_HORDE_ONLY              = 0x00000001,
    ITEM_FLAG2_ALLIANCE_ONLY           = 0x00000002,
    ITEM_FLAG2_EXT_COST_REQUIRES_GOLD  = 0x00000004, // when item uses extended cost, gold is also required
    ITEM_FLAG2_UNK1                    = 0x00000008,
    ITEM_FLAG2_UNK2                    = 0x00000010,
    ITEM_FLAG2_UNK3                    = 0x00000020,
    ITEM_FLAG2_UNK4                    = 0x00000040,
    ITEM_FLAG2_UNK5                    = 0x00000080,
    ITEM_FLAG2_NEED_ROLL_DISABLED      = 0x00000100,
    ITEM_FLAG2_CASTER_WEAPON           = 0x00000200,
    ITEM_FLAG2_UNK6                    = 0x00000400,
    ITEM_FLAG2_UNK7                    = 0x00000800,
    ITEM_FLAG2_UNK8                    = 0x00001000,
    ITEM_FLAG2_UNK9                    = 0x00002000,
    ITEM_FLAG2_HAS_NORMAL_PRICE        = 0x00004000,
    ITEM_FLAG2_UNK10                   = 0x00008000,
    ITEM_FLAG2_UNK11                   = 0x00010000,
    ITEM_FLAG2_BNET_ACCOUNT_BOUND      = 0x00020000,
    ITEM_FLAG2_UNK12                   = 0x00040000,
    ITEM_FLAG2_UNK13                   = 0x00080000,
    ITEM_FLAG2_UNK14                   = 0x00100000,
    ITEM_FLAG2_CANNOT_BE_TRANSMOG      = 0x00200000,
    ITEM_FLAG2_CANNOT_TRANSMOG         = 0x00400000,
    ITEM_FLAG2_CAN_TRANSMOG            = 0x00800000,
    ITEM_FLAG2_UNK15                   = 0x01000000,
    ITEM_FLAG2_UNK16                   = 0x02000000,
    ITEM_FLAG2_UNK17                   = 0x04000000,
    ITEM_FLAG2_UNK18                   = 0x08000000,
    ITEM_FLAG2_UNK19                   = 0x10000000,
    ITEM_FLAG2_UNK20                   = 0x20000000,
    ITEM_FLAG2_HEROIC                  = 0x40000000,  ///< Source Simulationcraft (MoP Heroic item)
    ITEM_FLAG2_CRAFTING_MATERIAL       = 0x80000000
};

enum ItemFlags3
{
    ITEM_FLAG3_UNK1                        = 0x00000001,
    ITEM_FLAG3_UNK2                        = 0x00000002,
    ITEM_FLAG3_UNK3                        = 0x00000004,
    ITEM_FLAG3_UNK4                        = 0x00000008,
    ITEM_FLAG3_UNK5                        = 0x00000010,
    ITEM_FLAG3_UNK6                        = 0x00000020,
    ITEM_FLAG3_UNK7                        = 0x00000040,
    ITEM_FLAG3_IGNORE_ITEM_LEVEL_DELTAS    = 0x00000080, // Ignore item level adjustments from PLAYER_FIELD_ITEM_LEVEL_DELTA
    ITEM_FLAG3_IGNORE_PVP_ITEM_LEVEL_CAP   = 0x00000100,
    ITEM_FLAG3_HEIRLOOM_QUALITY            = 0x00000200, // Item appears as having heirloom quality ingame regardless of its real quality (does not affect stat calculation)
    ITEM_FLAG3_UNK8                        = 0x00000400,
    ITEM_FLAG3_UNK9                        = 0x00000800,
    ITEM_FLAG3_DOESNT_APPEAR_IN_GUILD_NEWS = 0x00001000, // Item is not included in the guild news panel
    ITEM_FLAG3_WARGAME_ONLY                = 0x00002000,
    ITEM_FLAG3_UNK11                       = 0x00004000,
    ITEM_FLAG3_UNK12                       = 0x00008000,
    ITEM_FLAG3_UNK13                       = 0x00010000,
    ITEM_FLAG3_SEND_TOAST_ON_LOOT          = 0x00020000, ///< Guessed
    ITEM_FLAG3_UNK15                       = 0x00040000
};

enum ItemFlagsCustom
{
    ITEM_FLAGS_CU_DURATION_REAL_TIME    = 0x0001,   ///< Item duration will tick even if player is offline
    ITEM_FLAGS_CU_IGNORE_QUEST_STATUS   = 0x0002,   ///< No quest status will be checked when this item drops
    ITEM_FLAGS_CU_FOLLOW_LOOT_RULES     = 0x0004,   ///< Item will always follow group/master/need before greed looting rules
    ITEM_FLAGS_CU_CANT_BE_SELL          = 0x0008,   ///< Item can't be sell
    ITEM_FLAGS_CU_BYPASS_VENDOR_FILTER  = 0x0010    ///< Bypass vendor filter (always shown)
};

enum CurrencyCategory
{
    CURRENCY_CATEGORY_MISC              = 1,   ///< Miscellaneous
    CURRENCY_CATEGORY_PVP               = 2,   ///< Player vs. Player
    CURRENCY_CATEGORY_UNUSED            = 3,   ///< Unused
    CURRENCY_CATEGORY_CLASSIC           = 4,   ///< Classic
    CURRENCY_CATEGORY_WOTLK             = 21,  ///< Wrath of the Lich King
    CURRENCY_CATEGORY_DUNGEON_AND_RAID  = 22,  ///< Dungeon and Raid
    CURRENCY_CATEGORY_BC                = 23,  ///< Burning Crusade
    CURRENCY_CATEGORY_TEST              = 41,  ///< Test
    CURRENCY_CATEGORY_CATACLYSM         = 81,  ///< Cataclysm
    CURRENCY_CATEGORY_ARCHAEOLOGY       = 82,  ///< Archaeology
    CURRENCY_CATEGORY_META_CONQUEST     = 89,  ///< Meta
    CURRENCY_CATEGORY_MOP               = 133, ///< Mists of Pandaria
    CURRENCY_CATEGORY_WOD               = 137  ///< Warlords of Draenor
};

enum ItemVendorType
{
    ITEM_VENDOR_TYPE_NONE     = 0,
    ITEM_VENDOR_TYPE_ITEM     = 1,
    ITEM_VENDOR_TYPE_CURRENCY = 2,
};

enum BAG_FAMILY_MASK
{
    BAG_FAMILY_MASK_NONE                        = 0x00000000,
    BAG_FAMILY_MASK_ARROWS                      = 0x00000001,
    BAG_FAMILY_MASK_BULLETS                     = 0x00000002,
    BAG_FAMILY_MASK_SOUL_SHARDS                 = 0x00000004,
    BAG_FAMILY_MASK_LEATHERWORKING_SUPPLIES     = 0x00000008,
    BAG_FAMILY_MASK_INSCRIPTION_SUPPLIES        = 0x00000010,
    BAG_FAMILY_MASK_HERBS                       = 0x00000020,
    BAG_FAMILY_MASK_ENCHANTING_SUPPLIES         = 0x00000040,
    BAG_FAMILY_MASK_ENGINEERING_SUPPLIES        = 0x00000080,
    BAG_FAMILY_MASK_KEYS                        = 0x00000100,
    BAG_FAMILY_MASK_GEMS                        = 0x00000200,
    BAG_FAMILY_MASK_MINING_SUPPLIES             = 0x00000400,
    BAG_FAMILY_MASK_SOULBOUND_EQUIPMENT         = 0x00000800,
    BAG_FAMILY_MASK_VANITY_PETS                 = 0x00001000,
    BAG_FAMILY_MASK_CURRENCY_TOKENS             = 0x00002000,
    BAG_FAMILY_MASK_QUEST_ITEMS                 = 0x00004000,
    BAG_FAMILY_MASK_FISHING_SUPPLIES            = 0x00008000,
    BAG_FAMILY_MASK_COOKING_SUPPLIES            = 0x00010000,
    BAG_FAMILY_MASK_TOYS                        = 0x00020000,
    BAG_FAMILY_MASK_ARCHAEOLOGY                 = 0x00040000,
    BAG_FAMILY_MASK_ALCHEMY                     = 0x00080000,
    BAG_FAMILY_MASK_BLACKSMITHING               = 0x00100000,
    BAG_FAMILY_MASK_FIRSTAID                    = 0x00200000,
    BAG_FAMILY_MASK_JEWELCRAFTING               = 0x00400000,
    BAG_FAMILY_MASK_SKINING                     = 0x00800000,
    BAG_FAMILY_MASK_TAILORING                   = 0x01000000
};

enum SocketColor
{
    SOCKET_COLOR_META                           = 1,
    SOCKET_COLOR_RED                            = 2,
    SOCKET_COLOR_YELLOW                         = 4,
    SOCKET_COLOR_BLUE                           = 8,
    SOCKET_COLOR_PRISMATIC                      = 14,
    SOCKET_COLOR_SHA_TOUCHED                    = 16,
    SOCKET_COLOR_COGWHEEL                       = 32
};

#define SOCKET_COLOR_ALL (SOCKET_COLOR_META | SOCKET_COLOR_RED | SOCKET_COLOR_YELLOW | SOCKET_COLOR_BLUE | SOCKET_COLOR_PRISMATIC | SOCKET_COLOR_SHA_TOUCHED| SOCKET_COLOR_COGWHEEL)

enum InventoryType
{
    INVTYPE_NON_EQUIP                           = 0,
    INVTYPE_HEAD                                = 1,
    INVTYPE_NECK                                = 2,
    INVTYPE_SHOULDERS                           = 3,
    INVTYPE_BODY                                = 4,
    INVTYPE_CHEST                               = 5,
    INVTYPE_WAIST                               = 6,
    INVTYPE_LEGS                                = 7,
    INVTYPE_FEET                                = 8,
    INVTYPE_WRISTS                              = 9,
    INVTYPE_HANDS                               = 10,
    INVTYPE_FINGER                              = 11,
    INVTYPE_TRINKET                             = 12,
    INVTYPE_WEAPON                              = 13,
    INVTYPE_SHIELD                              = 14,
    INVTYPE_RANGED                              = 15,
    INVTYPE_CLOAK                               = 16,
    INVTYPE_2HWEAPON                            = 17,
    INVTYPE_BAG                                 = 18,
    INVTYPE_TABARD                              = 19,
    INVTYPE_ROBE                                = 20,
    INVTYPE_WEAPONMAINHAND                      = 21,
    INVTYPE_WEAPONOFFHAND                       = 22,
    INVTYPE_HOLDABLE                            = 23,
    INVTYPE_AMMO                                = 24,
    INVTYPE_THROWN                              = 25,
    INVTYPE_RANGEDRIGHT                         = 26,
    INVTYPE_QUIVER                              = 27,
    INVTYPE_RELIC                               = 28,

    MAX_INVTYPE
};

enum ItemClass
{
    ITEM_CLASS_CONSUMABLE                       = 0,
    ITEM_CLASS_CONTAINER                        = 1,
    ITEM_CLASS_WEAPON                           = 2,
    ITEM_CLASS_GEM                              = 3,
    ITEM_CLASS_ARMOR                            = 4,
    ITEM_CLASS_REAGENT                          = 5,
    ITEM_CLASS_PROJECTILE                       = 6,
    ITEM_CLASS_TRADE_GOODS                      = 7,
    ITEM_CLASS_GENERIC                          = 8,  // OBSOLETE
    ITEM_CLASS_RECIPE                           = 9,
    ITEM_CLASS_MONEY                            = 10, // OBSOLETE
    ITEM_CLASS_QUIVER                           = 11,
    ITEM_CLASS_QUEST                            = 12,
    ITEM_CLASS_KEY                              = 13,
    ITEM_CLASS_PERMANENT                        = 14, // OBSOLETE
    ITEM_CLASS_MISCELLANEOUS                    = 15,
    ITEM_CLASS_GLYPH                            = 16,
    ITEM_CLASS_BATTLE_PET                       = 17,
    ITEM_CLASS_WOW_TOKEN                        = 18  ///< WoW Token system
};

#define MAX_ITEM_CLASS                            19

enum ItemSubclassConsumable
{
    ITEM_SUBCLASS_CONSUMABLE                    = 0,
    ITEM_SUBCLASS_POTION                        = 1,
    ITEM_SUBCLASS_ELIXIR                        = 2,
    ITEM_SUBCLASS_FLASK                         = 3,
    ITEM_SUBCLASS_SCROLL                        = 4,
    ITEM_SUBCLASS_FOOD_DRINK                    = 5,
    ITEM_SUBCLASS_ITEM_ENHANCEMENT              = 6,
    ITEM_SUBCLASS_BANDAGE                       = 7,
    ITEM_SUBCLASS_CONSUMABLE_OTHER              = 8
};

#define MAX_ITEM_SUBCLASS_CONSUMABLE              9

enum ItemSubclassContainer
{
    ITEM_SUBCLASS_CONTAINER                     = 0,
    ITEM_SUBCLASS_SOUL_CONTAINER                = 1,
    ITEM_SUBCLASS_HERB_CONTAINER                = 2,
    ITEM_SUBCLASS_ENCHANTING_CONTAINER          = 3,
    ITEM_SUBCLASS_ENGINEERING_CONTAINER         = 4,
    ITEM_SUBCLASS_GEM_CONTAINER                 = 5,
    ITEM_SUBCLASS_MINING_CONTAINER              = 6,
    ITEM_SUBCLASS_LEATHERWORKING_CONTAINER      = 7,
    ITEM_SUBCLASS_INSCRIPTION_CONTAINER         = 8,
    ITEM_SUBCLASS_TACKLE_CONTAINER              = 9,
    ITEM_SUBCLASS_COOKING_CONTAINER             = 10,
    ITEM_SUBCLASS_OTHER_CONTAINER               = 11,
    ITEM_SUBCLASS_ENCHANTING_CONTAINER_2        = 12,
    ITEM_SUBCLASS_MATERIALS_CONTAINER           = 13,
    ITEM_SUBCLASS_ITEM_ENCHANTMENT_CONTAINER    = 14,
    ITEM_SUBCLASS_WEAPON_ENCHANTMENT_CONTAINER  = 15
};

#define MAX_ITEM_SUBCLASS_CONTAINER               16

enum ItemSubclassWeapon
{
    ITEM_SUBCLASS_WEAPON_AXE                    = 0,  // One-Handed Axes
    ITEM_SUBCLASS_WEAPON_AXE2                   = 1,  // Two-Handed Axes
    ITEM_SUBCLASS_WEAPON_BOW                    = 2,
    ITEM_SUBCLASS_WEAPON_GUN                    = 3,
    ITEM_SUBCLASS_WEAPON_MACE                   = 4,  // One-Handed Maces
    ITEM_SUBCLASS_WEAPON_MACE2                  = 5,  // Two-Handed Maces
    ITEM_SUBCLASS_WEAPON_POLEARM                = 6,
    ITEM_SUBCLASS_WEAPON_SWORD                  = 7,  // One-Handed Swords
    ITEM_SUBCLASS_WEAPON_SWORD2                 = 8,  // Two-Handed Swords
    ITEM_SUBCLASS_WEAPON_Obsolete               = 9,
    ITEM_SUBCLASS_WEAPON_STAFF                  = 10,
    ITEM_SUBCLASS_WEAPON_EXOTIC                 = 11, // One-Handed Exotics
    ITEM_SUBCLASS_WEAPON_EXOTIC2                = 12, // Two-Handed Exotics
    ITEM_SUBCLASS_WEAPON_FIST_WEAPON            = 13,
    ITEM_SUBCLASS_WEAPON_MISCELLANEOUS          = 14,
    ITEM_SUBCLASS_WEAPON_DAGGER                 = 15,
    ITEM_SUBCLASS_WEAPON_THROWN                 = 16,
    ITEM_SUBCLASS_WEAPON_SPEAR                  = 17,
    ITEM_SUBCLASS_WEAPON_CROSSBOW               = 18,
    ITEM_SUBCLASS_WEAPON_WAND                   = 19,
    ITEM_SUBCLASS_WEAPON_FISHING_POLE           = 20
};

#define ITEM_SUBCLASS_MASK_WEAPON_RANGED (\
    (1 << ITEM_SUBCLASS_WEAPON_BOW) | (1 << ITEM_SUBCLASS_WEAPON_GUN) |\
    (1 << ITEM_SUBCLASS_WEAPON_CROSSBOW) | (1 << ITEM_SUBCLASS_WEAPON_THROWN))

#define MAX_ITEM_SUBCLASS_WEAPON                  21

enum ItemSubclassGem
{
    ITEM_SUBCLASS_GEM_RED                       = 0,
    ITEM_SUBCLASS_GEM_BLUE                      = 1,
    ITEM_SUBCLASS_GEM_YELLOW                    = 2,
    ITEM_SUBCLASS_GEM_PURPLE                    = 3,
    ITEM_SUBCLASS_GEM_GREEN                     = 4,
    ITEM_SUBCLASS_GEM_ORANGE                    = 5,
    ITEM_SUBCLASS_GEM_META                      = 6,
    ITEM_SUBCLASS_GEM_SIMPLE                    = 7,
    ITEM_SUBCLASS_GEM_PRISMATIC                 = 8,
    ITEM_SUBCLASS_GEM_SHA_TOUCHED               = 9,
    ITEM_SUBCLASS_GEM_COGWHEEL                  = 10
};

#define MAX_ITEM_SUBCLASS_GEM                     11

enum ItemSubclassArmor
{
    ITEM_SUBCLASS_ARMOR_MISCELLANEOUS           = 0,
    ITEM_SUBCLASS_ARMOR_CLOTH                   = 1,
    ITEM_SUBCLASS_ARMOR_LEATHER                 = 2,
    ITEM_SUBCLASS_ARMOR_MAIL                    = 3,
    ITEM_SUBCLASS_ARMOR_PLATE                   = 4,
    ITEM_SUBCLASS_ARMOR_COSMETIC                = 5, // New 5.4 (old : BUCKLER)
    ITEM_SUBCLASS_ARMOR_SHIELD                  = 6,
    ITEM_SUBCLASS_ARMOR_LIBRAM                  = 7,
    ITEM_SUBCLASS_ARMOR_IDOL                    = 8,
    ITEM_SUBCLASS_ARMOR_TOTEM                   = 9,
    ITEM_SUBCLASS_ARMOR_SIGIL                   = 10,
    ITEM_SUBCLASS_ARMOR_RELIC                   = 11,
};

#define MAX_ITEM_SUBCLASS_ARMOR                   12

enum ItemSubclassReagent
{
    ITEM_SUBCLASS_REAGENT                       = 0
};

#define MAX_ITEM_SUBCLASS_REAGENT                 1

enum ItemSubclassProjectile
{
    ITEM_SUBCLASS_WAND                          = 0, // OBSOLETE
    ITEM_SUBCLASS_BOLT                          = 1, // OBSOLETE
    ITEM_SUBCLASS_ARROW                         = 2,
    ITEM_SUBCLASS_BULLET                        = 3,
    ITEM_SUBCLASS_THROWN                        = 4  // OBSOLETE
};

#define MAX_ITEM_SUBCLASS_PROJECTILE              5

enum ItemSubclassTradeGoods
{
    ITEM_SUBCLASS_TRADE_GOODS                   = 0,
    ITEM_SUBCLASS_PARTS                         = 1,
    ITEM_SUBCLASS_EXPLOSIVES                    = 2,
    ITEM_SUBCLASS_DEVICES                       = 3,
    ITEM_SUBCLASS_JEWELCRAFTING                 = 4,
    ITEM_SUBCLASS_CLOTH                         = 5,
    ITEM_SUBCLASS_LEATHER                       = 6,
    ITEM_SUBCLASS_METAL_STONE                   = 7,
    ITEM_SUBCLASS_MEAT                          = 8,
    ITEM_SUBCLASS_HERB                          = 9,
    ITEM_SUBCLASS_ELEMENTAL                     = 10,
    ITEM_SUBCLASS_TRADE_GOODS_OTHER             = 11,
    ITEM_SUBCLASS_ENCHANTING                    = 12,
    ITEM_SUBCLASS_MATERIAL                      = 13,
    ITEM_SUBCLASS_ENCHANTMENT                   = 14
};

#define MAX_ITEM_SUBCLASS_TRADE_GOODS             15

enum ItemSubclassGeneric
{
    ITEM_SUBCLASS_GENERIC                       = 0  // OBSOLETE
};

#define MAX_ITEM_SUBCLASS_GENERIC                 1

enum ItemSubclassRecipe
{
    ITEM_SUBCLASS_BOOK                          = 0,
    ITEM_SUBCLASS_LEATHERWORKING_PATTERN        = 1,
    ITEM_SUBCLASS_TAILORING_PATTERN             = 2,
    ITEM_SUBCLASS_ENGINEERING_SCHEMATIC         = 3,
    ITEM_SUBCLASS_BLACKSMITHING                 = 4,
    ITEM_SUBCLASS_COOKING_RECIPE                = 5,
    ITEM_SUBCLASS_ALCHEMY_RECIPE                = 6,
    ITEM_SUBCLASS_FIRST_AID_MANUAL              = 7,
    ITEM_SUBCLASS_ENCHANTING_FORMULA            = 8,
    ITEM_SUBCLASS_FISHING_MANUAL                = 9,
    ITEM_SUBCLASS_JEWELCRAFTING_RECIPE          = 10,
    ITEM_SUBCLASS_INSCRIPTION_TECHNIQUE         = 11
};

#define MAX_ITEM_SUBCLASS_RECIPE                  12

enum ItemSubclassMoney
{
    ITEM_SUBCLASS_MONEY                         = 0,  // OBSOLETE
    ITEM_SUBCLASS_MONEY_UNK_7                   = 7,  // OBSOLETE, 1 item (41749)
};

#define MAX_ITEM_SUBCLASS_MONEY                   8

enum ItemSubclassQuiver
{
    ITEM_SUBCLASS_QUIVER0                       = 0, // OBSOLETE
    ITEM_SUBCLASS_QUIVER1                       = 1, // OBSOLETE
    ITEM_SUBCLASS_QUIVER                        = 2,
    ITEM_SUBCLASS_AMMO_POUCH                    = 3
};

#define MAX_ITEM_SUBCLASS_QUIVER                  4

enum ItemSubclassQuest
{
    ITEM_SUBCLASS_QUEST                         = 0,
    ITEM_SUBCLASS_QUEST_UNK3                    = 3, // 1 item (33604)
    ITEM_SUBCLASS_QUEST_UNK8                    = 8, // 2 items (37445, 49700)
};

#define MAX_ITEM_SUBCLASS_QUEST                   9

enum ItemSubclassKey
{
    ITEM_SUBCLASS_KEY                           = 0,
    ITEM_SUBCLASS_LOCKPICK                      = 1
};

#define MAX_ITEM_SUBCLASS_KEY                     2

enum ItemSubclassPermanent
{
    ITEM_SUBCLASS_PERMANENT                     = 0
};

#define MAX_ITEM_SUBCLASS_PERMANENT               1

enum ItemSubclassJunk
{
    ITEM_SUBCLASS_JUNK                          = 0,
    ITEM_SUBCLASS_JUNK_REAGENT                  = 1,
    ITEM_SUBCLASS_JUNK_PET                      = 2,
    ITEM_SUBCLASS_JUNK_HOLIDAY                  = 3,
    ITEM_SUBCLASS_JUNK_OTHER                    = 4,
    ITEM_SUBCLASS_JUNK_MOUNT                    = 5,
    ITEM_SUBCLASS_JUNK_UNK12                    = 12, // 1 item (37677)
};

#define MAX_ITEM_SUBCLASS_JUNK                    13

enum ItemSubclassGlyph
{
    ITEM_SUBCLASS_GLYPH_WARRIOR                 = 1,
    ITEM_SUBCLASS_GLYPH_PALADIN                 = 2,
    ITEM_SUBCLASS_GLYPH_HUNTER                  = 3,
    ITEM_SUBCLASS_GLYPH_ROGUE                   = 4,
    ITEM_SUBCLASS_GLYPH_PRIEST                  = 5,
    ITEM_SUBCLASS_GLYPH_DEATH_KNIGHT            = 6,
    ITEM_SUBCLASS_GLYPH_SHAMAN                  = 7,
    ITEM_SUBCLASS_GLYPH_MAGE                    = 8,
    ITEM_SUBCLASS_GLYPH_WARLOCK                 = 9,
    ITEM_SUBCLASS_GLYPH_MONK                    = 10,
    ITEM_SUBCLASS_GLYPH_DRUID                   = 11
};

#define MAX_ITEM_SUBCLASS_GLYPH                   12

enum ItemSubclassBattlePet
{
    ITEM_SUBCLASS_BATTLE_PET_AQUATIC            = 1,
    ITEM_SUBCLASS_BATTLE_PET_BEAST              = 2,
    ITEM_SUBCLASS_BATTLE_PET_CRITTER            = 3,
    ITEM_SUBCLASS_BATTLE_PET_DRAGONKIN          = 4,
    ITEM_SUBCLASS_BATTLE_PET_ELEMENTAL          = 5,
    ITEM_SUBCLASS_BATTLE_PET_FLYING             = 6,
    ITEM_SUBCLASS_BATTLE_PET_HUMANOID           = 7,
    ITEM_SUBCLASS_BATTLE_PET_MAGICAL            = 8,
    ITEM_SUBCLASS_BATTLE_PET_MECHANICAL         = 9,
    ITEM_SUBCLASS_BATTLE_PET_UNDEAD             = 10
};

#define MAX_ITEM_SUBCLASS_BATTLE_PET              11

enum ItemSubclassWowToken
{
    ITEM_SUBCLASS_WOW_TOKEN                     = 1
};

#define MAX_ITEM_SUBCLASS_WOW_TOKEN              1

const uint32 MaxItemSubclassValues[MAX_ITEM_CLASS] =
{
    MAX_ITEM_SUBCLASS_CONSUMABLE,
    MAX_ITEM_SUBCLASS_CONTAINER,
    MAX_ITEM_SUBCLASS_WEAPON,
    MAX_ITEM_SUBCLASS_GEM,
    MAX_ITEM_SUBCLASS_ARMOR,
    MAX_ITEM_SUBCLASS_REAGENT,
    MAX_ITEM_SUBCLASS_PROJECTILE,
    MAX_ITEM_SUBCLASS_TRADE_GOODS,
    MAX_ITEM_SUBCLASS_GENERIC,
    MAX_ITEM_SUBCLASS_RECIPE,
    MAX_ITEM_SUBCLASS_MONEY,
    MAX_ITEM_SUBCLASS_QUIVER,
    MAX_ITEM_SUBCLASS_QUEST,
    MAX_ITEM_SUBCLASS_KEY,
    MAX_ITEM_SUBCLASS_PERMANENT,
    MAX_ITEM_SUBCLASS_JUNK,
    MAX_ITEM_SUBCLASS_GLYPH,
    MAX_ITEM_SUBCLASS_BATTLE_PET,
    MAX_ITEM_SUBCLASS_WOW_TOKEN
};

inline uint8 ItemSubClassToDurabilityMultiplierId(uint32 ItemClass, uint32 ItemSubClass)
{
    switch (ItemClass)
    {
        case ITEM_CLASS_WEAPON: return ItemSubClass;
        case ITEM_CLASS_ARMOR:  return ItemSubClass + 21;
    }
    return 0;
}

// GCC have alternative #pragma pack(N) syntax and old gcc version not support pack(push, N), also any gcc version not support it at some platform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct _ItemStat
{
    uint32  ItemStatType;
    int32   ItemStatValue;
    int32   ScalingValue;
    float   SocketCostRate;
};

struct _Spell
{
    int32 SpellId;                                         // id from Spell.dbc
    uint32 SpellTrigger;
    int32  SpellCharges;
    int32  SpellCooldown;
    uint32 SpellCategory;                                   // id from SpellCategory.dbc
    int32  SpellCategoryCooldown;
};

struct _Socket
{
    uint32 Color;
    uint32 Content;
};

#define MAX_ITEM_PROTO_DAMAGES 2                            // changed in 3.1.0
#define MAX_ITEM_PROTO_SOCKETS 3
#define MAX_ITEM_PROTO_SPELLS  5
#define MAX_ITEM_PROTO_STATS  10

typedef std::list<SpecIndex> SpecList;

namespace ItemBonus
{
    using GroupIdContainer = std::list<uint32>;
};

struct ItemTemplate
{
    uint32 ItemId;
    uint32 Class;                                           // id from ItemClass.dbc
    uint32 SubClass;                                        // id from ItemSubClass.dbc
    int32  SoundOverrideSubclass;                           // < 0: id from ItemSubClass.dbc, used to override weapon sound from actual SubClass
    LocalizedString const* Name1;
    uint32 DisplayInfoID;                                   // id from ItemDisplayInfo.dbc
    uint32 Quality;
    uint32 Flags;
    uint32 Flags2;
    uint32 Flags3;
    float Unk430_1;
    float Unk430_2;
    uint32 BuyCount;
    int32  BuyPrice;
    uint32 SellPrice;
    uint32 InventoryType;
    uint32 AllowableClass;
    uint32 AllowableRace;
    uint32 ItemLevel;
    uint32 RequiredLevel;
    uint32 RequiredSkill;                                   // id from SkillLine.dbc
    uint32 RequiredSkillRank;
    uint32 RequiredSpell;                                   // id from Spell.dbc
    uint32 RequiredHonorRank;
    uint32 RequiredCityRank;
    uint32 RequiredReputationFaction;                       // id from Faction.dbc
    uint32 RequiredReputationRank;
    int32  MaxCount;                                        // <= 0: no limit
    int32  Stackable;                                       // 0: not allowed, -1: put in player coin info tab and don't limit stacking (so 1 slot)
    uint32 ContainerSlots;
    _ItemStat ItemStat[MAX_ITEM_PROTO_STATS];
    uint32 ScalingStatDistribution;                         // id from ScalingStatDistribution.dbc
    uint32 DamageType;                                      // id from Resistances.dbc
    uint32 Delay;
    float  RangedModRange;
    _Spell Spells[MAX_ITEM_PROTO_SPELLS];
    uint32 Bonding;
    LocalizedString const* Description;
    uint32 PageText;
    uint32 LanguageID;
    uint32 PageMaterial;
    uint32 StartQuest;                                      // id from QuestCache.wdb
    uint32 LockID;
    int32  Material;                                        // id from Material.dbc
    uint32 Sheath;
    int32  RandomProperty;                                  // id from ItemRandomProperties.dbc
    int32  RandomSuffix;                                    // id from ItemRandomSuffix.dbc
    uint32 ItemSet;                                         // id from ItemSet.dbc
    uint32 MaxDurability;
    uint32 Area;                                            // id from AreaTable.dbc
    uint32 Map;                                             // id from Map.dbc
    uint32 BagFamily;                                       // bit mask (1 << id from ItemBagFamily.dbc)
    uint32 TotemCategory;                                   // id from TotemCategory.dbc
    _Socket Socket[MAX_ITEM_PROTO_SOCKETS];
    uint32 socketBonus;                                     // id from SpellItemEnchantment.dbc
    uint32 GemProperties;                                   // id from GemProperties.dbc
    float  ArmorDamageModifier;
    uint32 Duration;
    uint32 ItemLimitCategory;                               // id from ItemLimitCategory.dbc
    uint32 HolidayId;                                       // id from Holidays.dbc
    float  StatScalingFactor;
    uint32 CurrencySubstitutionId;                          // May be used instead of a currency
    uint32 CurrencySubstitutionCount;
    uint32 ItemNameDescriptionID;

    // extra fields, not part of db2 files
    float  DamageMin;
    float  DamageMax;
    float  DPS;
    uint32 Armor;
    float  SpellPPMRate;
    uint32 ScriptId;
    uint32 DisenchantID;
    uint32 RequiredDisenchantSkill;
    uint32 FoodType;
    uint32 MinMoneyLoot;
    uint32 MaxMoneyLoot;
    uint32 FlagsCu;
    SpecList specs[2];
    uint32 PvPScalingLevel;

    /// Item bonus group
    ItemBonus::GroupIdContainer m_ItemBonusGroups;

    // helpers
    bool CanChangeEquipStateInCombat() const
    {
        switch (InventoryType)
        {
            case INVTYPE_RELIC:
            case INVTYPE_SHIELD:
            case INVTYPE_HOLDABLE:
                return true;
        }

        switch (Class)
        {
            case ITEM_CLASS_WEAPON:
            case ITEM_CLASS_PROJECTILE:
                return true;
        }

        return false;
    }

    bool IsCurrencyToken() const { return BagFamily & BAG_FAMILY_MASK_CURRENCY_TOKENS; }

    uint32 GetMaxStackSize() const
    {
        return (Stackable == 2147483647 || Stackable <= 0) ? uint32(0x7FFFFFFF-1) : uint32(Stackable);
    }

    int GetItemLevelIncludingQuality(int l_ScalingItemLevel = 0) const
    {
        int l_ItemLevel = l_ScalingItemLevel ? l_ScalingItemLevel : (int)ItemLevel;
        switch (Quality)
        {
            case ITEM_QUALITY_POOR:
            case ITEM_QUALITY_NORMAL:
            case ITEM_QUALITY_UNCOMMON:
            case ITEM_QUALITY_ARTIFACT:
            case ITEM_QUALITY_HEIRLOOM:
                l_ItemLevel -= 13; // leaving this as a separate statement since we do not know the real behavior in this case
                break;
            case ITEM_QUALITY_EPIC:
            case ITEM_QUALITY_LEGENDARY:
            default:
                break;
        }
        return l_ItemLevel >= 0 ? l_ItemLevel : 1;
    }

    bool IsPotion() const { return Class == ITEM_CLASS_CONSUMABLE && (SubClass == ITEM_SUBCLASS_POTION || SubClass == ITEM_SUBCLASS_FOOD_DRINK); }
    bool IsVellum() const { return Class == ITEM_CLASS_TRADE_GOODS && SubClass == ITEM_SUBCLASS_ENCHANTMENT; }
    bool IsConjuredConsumable() const { return Class == ITEM_CLASS_CONSUMABLE && (Flags & ITEM_FLAG_CONJURED); }

    bool IsRangedWeapon() const
    {
        return Class == ITEM_CLASS_WEAPON && (
               SubClass == ITEM_SUBCLASS_WEAPON_BOW ||
               SubClass == ITEM_SUBCLASS_WEAPON_GUN ||
               SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW);
    }

    bool IsOneHanded() const
    {
        return Class == ITEM_CLASS_WEAPON && (
            SubClass == ITEM_SUBCLASS_WEAPON_SWORD ||
            SubClass == ITEM_SUBCLASS_WEAPON_AXE ||
            SubClass == ITEM_SUBCLASS_WEAPON_MACE ||
            SubClass == ITEM_SUBCLASS_WEAPON_DAGGER ||
            SubClass == ITEM_SUBCLASS_WEAPON_FIST_WEAPON ||
            SubClass == ITEM_SUBCLASS_WEAPON_EXOTIC);
    }

    bool IsTwoHandedWeapon() const
    {
        return Class == ITEM_CLASS_WEAPON && !IsOneHanded();
    }

    bool IsQuestItem() const
    {
        return Class == ITEM_CLASS_QUEST;
    }

    uint32 GetSkill() const
    {
        const static uint32 item_weapon_skills[MAX_ITEM_SUBCLASS_WEAPON] =
        {
            SKILL_AXES,     SKILL_2H_AXES,  SKILL_BOWS,          SKILL_GUNS,      SKILL_MACES,
            SKILL_2H_MACES, SKILL_POLEARMS, SKILL_SWORDS,        SKILL_2H_SWORDS, 0,
            SKILL_STAVES,   0,              0,                   SKILL_FIST_WEAPONS,   0,
            SKILL_DAGGERS,  SKILL_THROWN,   SKILL_ASSASSINATION, SKILL_CROSSBOWS, SKILL_WANDS,
            SKILL_FISHING
        };

        const static uint32 item_armor_skills[MAX_ITEM_SUBCLASS_ARMOR] =
        {
            0, SKILL_CLOTH, SKILL_LEATHER, SKILL_MAIL, SKILL_PLATE_MAIL, 0, SKILL_SHIELD, 0, 0, 0, 0
        };

        switch (Class)
        {
            case ITEM_CLASS_WEAPON:
                if (SubClass >= MAX_ITEM_SUBCLASS_WEAPON)
                    return 0;
                else
                    return item_weapon_skills[SubClass];

            case ITEM_CLASS_ARMOR:
                if (SubClass >= MAX_ITEM_SUBCLASS_ARMOR)
                    return 0;
                else
                    return item_armor_skills[SubClass];

            default:
                return 0;
        }
    }

    void AddSpec(SpecIndex index, uint32 level)
    {
        uint8 rangeIndex = level > 40;
        specs[rangeIndex].push_back(index);
    }

    bool HasSpec() const { return !specs[1].empty() || !specs[0].empty(); }
    
    bool HasClassSpec(uint8 Class, uint32 level) const
    {
        uint8 rangeIndex = level > 40;
        for (auto itr : specs[rangeIndex])
            if (GetClassBySpec(itr) == Class)
                return true;
        return false;
    }

    bool HasSpec(SpecIndex index, uint32 level) const
    {
        uint8 rangeIndex = level > 40;
        for (auto itr : specs[rangeIndex])
            if (itr == index)
                return true;
        return false;
    }

    bool IsWeapon() const { return Class == ITEM_CLASS_WEAPON; }

    float GetScalingDamageValue(uint32 ilvl) const;
    int32 GetRandomPointsOffset() const;
    float GetSocketCost(uint32 ilvl) const;
    int32 CalculateStatScaling(uint32 index, uint32 ilvl) const;
    int32 CalculateStatScaling(int32 scalingValue, float socketCost, uint32 ilvl) const;
    uint32 CalculateScalingStatDBCValue(uint32 ilvl) const;
    uint32 CalculateArmorScaling(uint32 ilvl) const;
    void CalculateMinMaxDamageScaling(uint32 ilvl, uint32& minDamage, uint32& maxDamage) const;

    bool CanBeTransmogrified() const
    {
        //if (Quality == ITEM_QUALITY_LEGENDARY)
        //    return false;

        if (Class != ITEM_CLASS_ARMOR &&
            Class != ITEM_CLASS_WEAPON)
            return false;

        if (Class == ITEM_CLASS_WEAPON && SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
            return false;

        if (Flags2 & ITEM_FLAG2_CANNOT_BE_TRANSMOG)
            return false;

        return true;
    }

    bool CanTransmogrify() const
    {
        if (Flags2 & ITEM_FLAG2_CANNOT_TRANSMOG)
            return false;

        /// Custom removed for selling old legendary item
        /// Ask by Peluche
        //if (Quality == ITEM_QUALITY_LEGENDARY)
        //    return false;

        if (Class != ITEM_CLASS_ARMOR &&
            Class != ITEM_CLASS_WEAPON)
            return false;

        if (Class == ITEM_CLASS_WEAPON && SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
            return false;

        if (Flags2 & ITEM_FLAG2_CAN_TRANSMOG)
            return true;

        return true;
    }

    bool HasStats() const
    {
        for (uint8 l_I = 0; l_I < MAX_ITEM_PROTO_STATS; ++l_I)
        {
            if (ItemStat[l_I].ItemStatValue != 0)
                return true;
        }

        return false;
    }
};

enum ItemSpecStat
{
    ITEM_SPEC_STAT_INTELLECT = 0,
    ITEM_SPEC_STAT_AGILITY = 1,
    ITEM_SPEC_STAT_STRENGTH = 2,
    ITEM_SPEC_STAT_SPIRIT = 3,
    ITEM_SPEC_STAT_HIT = 4,
    ITEM_SPEC_STAT_DODGE = 5,
    ITEM_SPEC_STAT_PARRY = 6,
    ITEM_SPEC_STAT_ONE_HANDED_AXE = 7,
    ITEM_SPEC_STAT_TWO_HANDED_AXE = 8,
    ITEM_SPEC_STAT_ONE_HANDED_SWORD = 9,
    ITEM_SPEC_STAT_TWO_HANDED_SWORD = 10,
    ITEM_SPEC_STAT_ONE_HANDED_MACE = 11,
    ITEM_SPEC_STAT_TWO_HANDED_MACE = 12,
    ITEM_SPEC_STAT_DAGGER = 13,
    ITEM_SPEC_STAT_FIST_WEAPON = 14,
    ITEM_SPEC_STAT_GUN = 15,
    ITEM_SPEC_STAT_BOW = 16,
    ITEM_SPEC_STAT_CROSSBOW = 17,
    ITEM_SPEC_STAT_STAFF = 18,
    ITEM_SPEC_STAT_POLEARM = 19,
    ITEM_SPEC_STAT_THROWN = 20,
    ITEM_SPEC_STAT_WAND = 21,
    ITEM_SPEC_STAT_SHIELD = 22,
    ITEM_SPEC_STAT_RELIC = 23,
    ITEM_SPEC_STAT_CRIT = 24,
    ITEM_SPEC_STAT_HASTE = 25,
    ITEM_SPEC_STAT_BONUS_ARMOR = 26,
    ITEM_SPEC_STAT_CLOAK = 27,

    ITEM_SPEC_STAT_NONE = 28
};

extern float GetCurveValue(uint32 CurveParameter, float level);

// Benchmarked: Faster than std::map (insert/find)
typedef std::unordered_map<uint32, ItemTemplate> ItemTemplateContainer;

struct ItemLocale
{
    StringVector Name;
    StringVector Description;
};

// GCC have alternative #pragma pack() syntax and old gcc version not support pack(pop), also any gcc version not support it at some platform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif
#endif
