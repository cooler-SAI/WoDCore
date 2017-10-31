////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
Name: npc_commandscript
%Complete: 100
Comment: All npc related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Chat.h"
#include "Transport.h"
#include "CreatureGroups.h"
#include "TargetedMovementGenerator.h"                      // for HandleNpcUnFollowCommand
#include "CreatureAI.h"

struct MechanicImmune
{
    uint32 flag;
    char const* text;
};

#define MECHANIC_MAX   33

MechanicImmune const mechanicImmunes[MECHANIC_MAX] =
{
    { MECHANIC_NONE            , "MECHANIC_NONE"            },
    { MECHANIC_CHARM           , "MECHANIC_CHARM"           },
    { MECHANIC_DISORIENTED     , "MECHANIC_DISORIENTED"     },
    { MECHANIC_DISARM          , "MECHANIC_DISARM"          },
    { MECHANIC_DISTRACT        , "MECHANIC_DISTRACT"        },
    { MECHANIC_FEAR            , "MECHANIC_FEAR"            },
    { MECHANIC_GRIP            , "MECHANIC_GRIP"            },
    { MECHANIC_ROOT            , "MECHANIC_ROOT"            },
    { MECHANIC_SLOW_ATTACK     , "MECHANIC_SLOW_ATTACK"     },
    { MECHANIC_SILENCE         , "MECHANIC_SILENCE"         },
    { MECHANIC_SLEEP           , "MECHANIC_SLEEP"           },
    { MECHANIC_SNARE           , "MECHANIC_SNARE"           },
    { MECHANIC_STUN            , "MECHANIC_STUN"            },
    { MECHANIC_FREEZE          , "MECHANIC_FREEZE"          },
    { MECHANIC_KNOCKOUT        , "MECHANIC_KNOCKOUT"        },
    { MECHANIC_BLEED           , "MECHANIC_BLEED"           },
    { MECHANIC_BANDAGE         , "MECHANIC_BANDAGE"         },
    { MECHANIC_POLYMORPH       , "MECHANIC_POLYMORPH"       },
    { MECHANIC_BANISH          , "MECHANIC_BANISH"          },
    { MECHANIC_SHIELD          , "MECHANIC_SHIELD"          },
    { MECHANIC_SHACKLE         , "MECHANIC_SHACKLE"         },
    { MECHANIC_MOUNT           , "MECHANIC_MOUNT"           },
    { MECHANIC_INFECTED        , "MECHANIC_INFECTED"        },
    { MECHANIC_TURN            , "MECHANIC_TURN"            },
    { MECHANIC_HORROR          , "MECHANIC_HORROR"          },
    { MECHANIC_INVULNERABILITY , "MECHANIC_INVULNERABILITY" },
    { MECHANIC_INTERRUPT       , "MECHANIC_INTERRUPT"       },
    { MECHANIC_DAZE            , "MECHANIC_DAZE"            },
    { MECHANIC_DISCOVERY       , "MECHANIC_DISCOVERY"       },
    { MECHANIC_IMMUNE_SHIELD   , "MECHANIC_IMMUNE_SHIELD"   },
    { MECHANIC_SAPPED          , "MECHANIC_SAPPED"          },
    { MECHANIC_ENRAGED         , "MECHANIC_ENRAGED"         },
    { MECHANIC_WOUNDED         , "MECHANIC_WOUNDED"         }
};

struct UnitFlag
{
    uint32 flag;
    char const* text;
};

#define UNIT_FLAGS_MAX   33

UnitFlag const unitFlags[UNIT_FLAGS_MAX] =
{
    { (uint32)UNIT_FLAG_SERVER_CONTROLLED   , "UNIT_FLAG_SERVER_CONTROLLED"     },
    { (uint32)UNIT_FLAG_NON_ATTACKABLE      , "UNIT_FLAG_NON_ATTACKABLE"        },
    { (uint32)UNIT_FLAG_DISABLE_MOVE        , "UNIT_FLAG_DISABLE_MOVE"          },
    { (uint32)UNIT_FLAG_PVP_ATTACKABLE      , "UNIT_FLAG_PVP_ATTACKABLE"        },
    { (uint32)UNIT_FLAG_RENAME              , "UNIT_FLAG_RENAME"                },
    { (uint32)UNIT_FLAG_PREPARATION         , "UNIT_FLAG_PREPARATION"           },
    { (uint32)UNIT_FLAG_UNK_6               , "UNIT_FLAG_UNK_6"                 },
    { (uint32)UNIT_FLAG_NOT_ATTACKABLE_1    , "UNIT_FLAG_NOT_ATTACKABLE_1"      },
    { (uint32)UNIT_FLAG_IMMUNE_TO_PC        , "UNIT_FLAG_IMMUNE_TO_PC"          },
    { (uint32)UNIT_FLAG_IMMUNE_TO_NPC       , "UNIT_FLAG_IMMUNE_TO_NPC"         },
    { (uint32)UNIT_FLAG_LOOTING             , "UNIT_FLAG_LOOTING"               },
    { (uint32)UNIT_FLAG_PET_IN_COMBAT       , "UNIT_FLAG_PET_IN_COMBAT"         },
    { (uint32)UNIT_FLAG_PVP                 , "UNIT_FLAG_PVP"                   },
    { (uint32)UNIT_FLAG_SILENCED            , "UNIT_FLAG_SILENCED"              },
    { (uint32)UNIT_FLAG_UNK_14              , "UNIT_FLAG_UNK_14"                },
    { (uint32)UNIT_FLAG_UNK_15              , "UNIT_FLAG_UNK_15"                },
    { (uint32)UNIT_FLAG_UNK_16              , "UNIT_FLAG_UNK_16"                },
    { (uint32)UNIT_FLAG_PACIFIED            , "UNIT_FLAG_PACIFIED"              },
    { (uint32)UNIT_FLAG_STUNNED             , "UNIT_FLAG_STUNNED"               },
    { (uint32)UNIT_FLAG_IN_COMBAT           , "UNIT_FLAG_IN_COMBAT"             },
    { (uint32)UNIT_FLAG_TAXI_FLIGHT         , "UNIT_FLAG_TAXI_FLIGHT"           },
    { (uint32)UNIT_FLAG_DISARMED            , "UNIT_FLAG_DISARMED"              },
    { (uint32)UNIT_FLAG_CONFUSED            , "UNIT_FLAG_CONFUSED"              },
    { (uint32)UNIT_FLAG_FLEEING             , "UNIT_FLAG_FLEEING"               },
    { (uint32)UNIT_FLAG_PLAYER_CONTROLLED   , "UNIT_FLAG_PLAYER_CONTROLLED"     },
    { (uint32)UNIT_FLAG_NOT_SELECTABLE      , "UNIT_FLAG_NOT_SELECTABLE"        },
    { (uint32)UNIT_FLAG_SKINNABLE           , "UNIT_FLAG_SKINNABLE"             },
    { (uint32)UNIT_FLAG_MOUNT               , "UNIT_FLAG_MOUNT"                 },
    { (uint32)UNIT_FLAG_UNK_28              , "UNIT_FLAG_UNK_28"                },
    { (uint32)UNIT_FLAG_UNK_29              , "UNIT_FLAG_UNK_29"                },
    { (uint32)UNIT_FLAG_SHEATHE             , "UNIT_FLAG_SHEATHE"               },
    { (uint32)UNIT_FLAG_UNK_31              , "UNIT_FLAG_UNK_31"                }
};

class npc_commandscript: public CommandScript
{
public:
    npc_commandscript() : CommandScript("npc_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand npcAddCommandTable[] =
        {
            { "formation",      SEC_MODERATOR,      false, &HandleNpcAddFormationCommand,      "", NULL },
            { "item",           SEC_GAMEMASTER,     false, &HandleNpcAddVendorItemCommand,     "", NULL },
            { "move",           SEC_GAMEMASTER,     false, &HandleNpcAddMoveCommand,           "", NULL },
            { "temp",           SEC_GAMEMASTER,     false, &HandleNpcAddTempSpawnCommand,      "", NULL },
            //{ TODO: fix or remove this command
            { "weapon",         SEC_ADMINISTRATOR,  false, &HandleNpcAddWeaponCommand,         "", NULL },
            //}
            { "",               SEC_GAMEMASTER,     false, &HandleNpcAddCommand,               "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        static ChatCommand npcDeleteCommandTable[] =
        {
            { "item",           SEC_GAMEMASTER,     false, &HandleNpcDeleteVendorItemCommand,  "", NULL },
            { "",               SEC_GAMEMASTER,     false, &HandleNpcDeleteCommand,            "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        static ChatCommand npcFollowCommandTable[] =
        {
            { "stop",           SEC_GAMEMASTER,     false, &HandleNpcUnFollowCommand,          "", NULL },
            { "",               SEC_GAMEMASTER,     false, &HandleNpcFollowCommand,            "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        static ChatCommand npcSetCommandTable[] =
        {
            { "allowmove",      SEC_ADMINISTRATOR,  false, &HandleNpcSetAllowMovementCommand,  "", NULL },
            { "entry",          SEC_ADMINISTRATOR,  false, &HandleNpcSetEntryCommand,          "", NULL },
            { "factionid",      SEC_GAMEMASTER,     false, &HandleNpcSetFactionIdCommand,      "", NULL },
            { "flag",           SEC_GAMEMASTER,     false, &HandleNpcSetFlagCommand,           "", NULL },
            { "flag2",           SEC_GAMEMASTER,     false, &HandleNpcSetFlag2Command,         "", NULL },
            { "level",          SEC_GAMEMASTER,     false, &HandleNpcSetLevelCommand,          "", NULL },
            { "link",           SEC_GAMEMASTER,     false, &HandleNpcSetLinkCommand,           "", NULL },
            { "model",          SEC_GAMEMASTER,     false, &HandleNpcSetModelCommand,          "", NULL },
            { "movetype",       SEC_GAMEMASTER,     false, &HandleNpcSetMoveTypeCommand,       "", NULL },
            { "phase",          SEC_GAMEMASTER,     false, &HandleNpcSetPhaseCommand,          "", NULL },
            { "spawndist",      SEC_GAMEMASTER,     false, &HandleNpcSetSpawnDistCommand,      "", NULL },
            { "spawntime",      SEC_GAMEMASTER,     false, &HandleNpcSetSpawnTimeCommand,      "", NULL },
            { "data",           SEC_ADMINISTRATOR,  false, &HandleNpcSetDataCommand,           "", NULL },
            //{ TODO: fix or remove these commands
            { "name",           SEC_GAMEMASTER,     false, &HandleNpcSetNameCommand,           "", NULL },
            { "subname",        SEC_GAMEMASTER,     false, &HandleNpcSetSubNameCommand,        "", NULL },
            //}
            { "animkit",        SEC_GAMEMASTER,     false, &HandleNpcSetAnimKitCommand,        "", NULL },
            { "emotestate",     SEC_GAMEMASTER,     false, &HandleNpcSetEmoteStateCommand,     "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        static ChatCommand npcCommandTable[] =
        {
            { "info",           SEC_ADMINISTRATOR,  false, &HandleNpcInfoCommand,              "", NULL },
            { "move",           SEC_GAMEMASTER,     false, &HandleNpcMoveCommand,              "", NULL },
            { "playemote",      SEC_ADMINISTRATOR,  false, &HandleNpcPlayEmoteCommand,         "", NULL },
            { "say",            SEC_MODERATOR,      false, &HandleNpcSayCommand,               "", NULL },
            { "textemote",      SEC_MODERATOR,      false, &HandleNpcTextEmoteCommand,         "", NULL },
            { "whisper",        SEC_MODERATOR,      false, &HandleNpcWhisperCommand,           "", NULL },
            { "yell",           SEC_MODERATOR,      false, &HandleNpcYellCommand,              "", NULL },
            { "tame",           SEC_GAMEMASTER,     false, &HandleNpcTameCommand,              "", NULL },
            { "map_activate",   SEC_GAMEMASTER,     false, &HandleNpcActivateCommand,          "", NULL },
            { "near",           SEC_GAMEMASTER,     false, &HandleNpcNearCommand,              "", NULL },
            { "add",            SEC_GAMEMASTER,     false, NULL,                 "", npcAddCommandTable },
            { "delete",         SEC_GAMEMASTER,     false, NULL,              "", npcDeleteCommandTable },
            { "follow",         SEC_GAMEMASTER,     false, NULL,              "", npcFollowCommandTable },
            { "set",            SEC_GAMEMASTER,     false, NULL,                 "", npcSetCommandTable },
            { "groupscaling",   SEC_GAMEMASTER,     false, &HandleNpcGroupScaling,             "", NULL },
            { "listaggro",      SEC_GAMEMASTER,     false, &HandleNpcListAggro,                "", NULL },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "npc",            SEC_MODERATOR,      false, NULL,                    "", npcCommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    //add spawn of creature
    static bool HandleNpcAddCommand(ChatHandler* p_Handler, const char* p_Args)
    {
        if (!*p_Args)
            return false;

        char* l_CharID = p_Handler->extractKeyFromLink((char*)p_Args, "Hcreature_entry");
        if (!l_CharID)
            return false;

        char* l_Team = strtok(NULL, " ");
        int32 l_Teamval = 0;

        if (l_Team)
            l_Teamval = atoi(l_Team);

        if (l_Teamval < 0)
            l_Teamval = 0;

        uint32 l_Id  = atoi(l_CharID);

        if (p_Handler->GetSession()->GetPlayer() == nullptr)
            return false;

        uint64 l_CharacterGuid = p_Handler->GetSession()->GetPlayer()->GetGUID();

        PreparedStatement* l_Statement = WorldDatabase.GetPreparedStatement(WORLD_SEL_MAX_CREATURE_GUID);
        AsyncQuery(WorldDatabase, l_Statement, [l_Id, l_Teamval, l_CharacterGuid](PreparedQueryResult p_Result) -> void
        {
            if (!p_Result)
                return;

            Player* l_Character = sObjectAccessor->FindPlayer(l_CharacterGuid);
            if (l_Character == nullptr)
                return;

            float x = l_Character->GetPositionX();
            float y = l_Character->GetPositionY();
            float z = l_Character->GetPositionZ();
            float o = l_Character->GetOrientation();
            Map* l_Map = l_Character->GetMap();

            if (Transport* l_Transport = l_Character->GetTransport())
            {
                uint32 l_Guid = p_Result->Fetch()[0].GetUInt32() + 1;
                CreatureData& l_Data = sObjectMgr->NewOrExistCreatureData(l_Guid);
                l_Data.id = l_Id;
                l_Data.phaseMask = l_Character->GetPhaseMgr().GetPhaseMaskForSpawn();
                l_Data.posX = l_Character->GetTransOffsetX();
                l_Data.posY = l_Character->GetTransOffsetY();
                l_Data.posZ = l_Character->GetTransOffsetZ();
                l_Data.orientation = l_Character->GetTransOffsetO();

                Creature* l_Creature = l_Transport->CreateNPCPassenger(l_Guid, &l_Data);
                l_Creature->SaveToDB(l_Transport->GetGOInfo()->moTransport.mapID, 1 << l_Map->GetSpawnMode(), l_Character->GetPhaseMgr().GetPhaseMaskForSpawn());
                return;
            }

            Creature* l_Creature = new Creature();
            if (!l_Creature->Create(p_Result->Fetch()[0].GetUInt32() + 1, l_Map, l_Character->GetPhaseMgr().GetPhaseMaskForSpawn(), l_Id, 0, (uint32)l_Teamval, x, y, z, o))
            {
                delete l_Creature;
                return;
            }

            l_Creature->SaveToDB(l_Map->GetId(), (1 << l_Map->GetSpawnMode()), l_Character->GetPhaseMgr().GetPhaseMaskForSpawn());

            uint32 l_DbGuid = l_Creature->GetDBTableGUIDLow();

            // To call _LoadGoods(); _LoadQuests(); CreateTrainerSpells();
            if (!l_Creature->LoadCreatureFromDB(l_DbGuid, l_Map))
            {
                delete l_Creature;
                return;
            }

            sObjectMgr->AddCreatureToGrid(l_DbGuid, sObjectMgr->GetCreatureData(l_DbGuid));
        });

        return true;
    }

    //add item in vendorlist
    static bool HandleNpcAddVendorItemCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        const uint8 type = 1; // @todo : make type (1 item, 2 currency) an argument

        char* pitem  = handler->extractKeyFromLink((char*)args, "Hitem");
        if (!pitem)
        {
            handler->SendSysMessage(LANG_COMMAND_NEEDITEMSEND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        int32 item_int = atol(pitem);
        if (item_int <= 0)
            return false;

        uint32 itemId = item_int;

        char* fmaxcount = strtok(NULL, " ");                    //add maxcount, default: 0
        uint32 maxcount = 0;
        if (fmaxcount)
            maxcount = atol(fmaxcount);

        char* fincrtime = strtok(NULL, " ");                    //add incrtime, default: 0
        uint32 incrtime = 0;
        if (fincrtime)
            incrtime = atol(fincrtime);

        char* fextendedcost = strtok(NULL, " ");                //add ExtendedCost, default: 0
        uint32 extendedcost = fextendedcost ? atol(fextendedcost) : 0;
        Creature* vendor = handler->getSelectedCreature();
        if (!vendor)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 vendor_entry = vendor ? vendor->GetEntry() : 0;

        if (!sObjectMgr->IsVendorItemValid(vendor_entry, itemId, maxcount, incrtime, extendedcost, type, handler->GetSession()->GetPlayer()))
        {
            handler->SetSentErrorMessage(true);
            return false;
        }

        sObjectMgr->AddVendorItem(vendor_entry, itemId, maxcount, incrtime, extendedcost, type);

        ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemId);

        handler->PSendSysMessage(LANG_ITEM_ADDED_TO_LIST, itemId, itemTemplate->Name1->Get(handler->GetSessionDbcLocale()), maxcount, incrtime, extendedcost);
        return true;
    }

    //add move for creature
    static bool HandleNpcAddMoveCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        char* guidStr = strtok((char*)args, " ");
        char* waitStr = strtok((char*)NULL, " ");

        uint32 lowGuid = atoi((char*)guidStr);

        Creature* creature = NULL;

        /* @todo : impossible without entry
        if (lowguid)
            creature = ObjectAccessor::GetCreature(*handler->GetSession()->GetPlayer(), MAKE_GUID(lowguid, HIGHGUID_UNIT));
        */

        // attempt check creature existence by DB data
        if (!creature)
        {
            CreatureData const* data = sObjectMgr->GetCreatureData(lowGuid);
            if (!data)
            {
                handler->PSendSysMessage(LANG_COMMAND_CREATGUIDNOTFOUND, lowGuid);
                handler->SetSentErrorMessage(true);
                return false;
            }
        }
        else
        {
            // obtain real GUID for DB operations
            lowGuid = creature->GetDBTableGUIDLow();
        }

        int wait = waitStr ? atoi(waitStr) : 0;

        if (wait < 0)
            wait = 0;

        // Update movement type
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_UPD_CREATURE_MOVEMENT_TYPE);

        stmt->setUInt8(0, uint8(WAYPOINT_MOTION_TYPE));
        stmt->setUInt32(1, lowGuid);

        WorldDatabase.Execute(stmt);

        if (creature && creature->GetWaypointPath())
        {
            creature->SetDefaultMovementType(WAYPOINT_MOTION_TYPE);
            creature->GetMotionMaster()->Initialize();
            if (creature->isAlive())                            // dead creature will reset movement generator at respawn
            {
                creature->setDeathState(JUST_DIED);
                creature->Respawn(true);
            }
            creature->SaveToDB();
        }

        handler->SendSysMessage(LANG_WAYPOINT_ADDED);

        return true;
    }

    static bool HandleNpcSetAllowMovementCommand(ChatHandler* handler, const char* /*args*/)
    {
        if (sWorld->getAllowMovement())
        {
            sWorld->SetAllowMovement(false);
            handler->SendSysMessage(LANG_CREATURE_MOVE_DISABLED);
        }
        else
        {
            sWorld->SetAllowMovement(true);
            handler->SendSysMessage(LANG_CREATURE_MOVE_ENABLED);
        }
        return true;
    }

    static bool HandleNpcSetEntryCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        uint32 newEntryNum = atoi(args);
        if (!newEntryNum)
            return false;

        Unit* unit = handler->getSelectedUnit();
        if (!unit || unit->GetTypeId() != TYPEID_UNIT)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }
        Creature* creature = unit->ToCreature();
        if (creature->UpdateEntry(newEntryNum))
            handler->SendSysMessage(LANG_DONE);
        else
            handler->SendSysMessage(LANG_ERROR);
        return true;
    }

    //change level of creature or pet
    static bool HandleNpcSetLevelCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        uint8 lvl = (uint8) atoi((char*)args);
        if (lvl < 1 || lvl > sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL) + 3)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Creature* creature = handler->getSelectedCreature();
        if (!creature)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (creature->isPet())
        {
            if (((Pet*)creature)->getPetType() == HUNTER_PET)
            {
                creature->SetUInt32Value(UNIT_FIELD_PET_NEXT_LEVEL_EXPERIENCE, sObjectMgr->GetXPForLevel(lvl)/4);
                creature->SetUInt32Value(UNIT_FIELD_PET_EXPERIENCE, 0);
            }
            ((Pet*)creature)->GivePetLevel(lvl);
        }
        else
        {
            creature->SetMaxHealth(100 + 30*lvl);
            creature->SetHealth(100 + 30*lvl);
            creature->SetLevel(lvl);
            creature->SaveToDB();
        }

        return true;
    }

    static bool HandleNpcDeleteCommand(ChatHandler* handler, const char* args)
    {
        Creature* unit = NULL;

        if (*args)
        {
            // number or [name] Shift-click form |color|Hcreature:creature_guid|h[name]|h|r
            char* cId = handler->extractKeyFromLink((char*)args, "Hcreature");
            if (!cId)
                return false;

            uint32 lowguid = atoi(cId);
            if (!lowguid)
                return false;

            if (CreatureData const* cr_data = sObjectMgr->GetCreatureData(lowguid))
                unit = handler->GetSession()->GetPlayer()->GetMap()->GetCreature(MAKE_NEW_GUID(lowguid, cr_data->id, HIGHGUID_UNIT));
        }
        else
            unit = handler->getSelectedCreature();

        if (!unit || unit->isPet() || unit->isTotem())
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // Delete the creature
        unit->CombatStop();
        unit->DeleteFromDB();
        unit->AddObjectToRemoveList();

        handler->SendSysMessage(LANG_COMMAND_DELCREATMESSAGE);

        return true;
    }

    //del item from vendor list
    static bool HandleNpcDeleteVendorItemCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        Creature* vendor = handler->getSelectedCreature();
        if (!vendor || !vendor->isVendor())
        {
            handler->SendSysMessage(LANG_COMMAND_VENDORSELECTION);
            handler->SetSentErrorMessage(true);
            return false;
        }

        char* pitem  = handler->extractKeyFromLink((char*)args, "Hitem");
        if (!pitem)
        {
            handler->SendSysMessage(LANG_COMMAND_NEEDITEMSEND);
            handler->SetSentErrorMessage(true);
            return false;
        }
        uint32 itemId = atol(pitem);

        const uint8 type = 1; // FIXME: make type (1 item, 2 currency) an argument

        if (!sObjectMgr->RemoveVendorItem(vendor->GetEntry(), itemId, type))
        {
            handler->PSendSysMessage(LANG_ITEM_NOT_IN_LIST, itemId);
            handler->SetSentErrorMessage(true);
            return false;
        }

        ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(itemId);

        handler->PSendSysMessage(LANG_ITEM_DELETED_FROM_LIST, itemId, itemTemplate->Name1->Get(handler->GetSessionDbcLocale()));
        return true;
    }

    //set faction of creature
    static bool HandleNpcSetFactionIdCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        uint32 factionId = (uint32) atoi((char*)args);

        if (!sFactionTemplateStore.LookupEntry(factionId))
        {
            handler->PSendSysMessage(LANG_WRONG_FACTION, factionId);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Creature* creature = handler->getSelectedCreature();

        if (!creature)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        creature->setFaction(factionId);

        // Faction is set in creature_template - not inside creature

        // Update in memory..
        if (CreatureTemplate const* cinfo = creature->GetCreatureTemplate())
            const_cast<CreatureTemplate*>(cinfo)->faction = factionId;

        // ..and DB
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_UPD_CREATURE_FACTION);

        stmt->setUInt16(0, uint16(factionId));
        stmt->setUInt32(2, creature->GetEntry());

        WorldDatabase.Execute(stmt);

        return true;
    }

    //set npcflag of creature
    static bool HandleNpcSetFlagCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        uint32 npcFlags = (uint32) atoi((char*)args);

        Creature* creature = handler->getSelectedCreature();

        if (!creature)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        creature->SetUInt32Value(UNIT_FIELD_NPC_FLAGS, npcFlags);

        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_UPD_CREATURE_NPCFLAG);

        stmt->setUInt32(0, npcFlags);
        stmt->setUInt32(1, creature->GetEntry());

        WorldDatabase.Execute(stmt);

        handler->SendSysMessage(LANG_VALUE_SAVED_REJOIN);

        return true;
    }

    // Set npcflag2 of creature
    static bool HandleNpcSetFlag2Command(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        uint32 npcFlags = (uint32) atoi((char*)args);

        Creature* creature = handler->getSelectedCreature();

        if (!creature)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        creature->SetUInt32Value(UNIT_FIELD_NPC_FLAGS + 1, npcFlags);

        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_UPD_CREATURE_NPCFLAG2);

        stmt->setUInt32(0, npcFlags);
        stmt->setUInt32(1, creature->GetEntry());

        WorldDatabase.Execute(stmt);

        handler->SendSysMessage(LANG_VALUE_SAVED_REJOIN);

        return true;
    }

    //set data of creature for testing scripting
    static bool HandleNpcSetDataCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        char* arg1 = strtok((char*)args, " ");
        char* arg2 = strtok((char*)NULL, "");

        if (!arg1 || !arg2)
            return false;

        uint32 data_1 = (uint32)atoi(arg1);
        uint32 data_2 = (uint32)atoi(arg2);

        if (!data_1 || !data_2)
            return false;

        Creature* creature = handler->getSelectedCreature();

        if (!creature)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        creature->AI()->SetData(data_1, data_2);
        std::string AIorScript = creature->GetAIName() != "" ? "AI type: " + creature->GetAIName() : (creature->GetScriptName() != "" ? "Script Name: " + creature->GetScriptName() : "No AI or Script Name Set");
        handler->PSendSysMessage(LANG_NPC_SETDATA, creature->GetGUID(), creature->GetEntry(), creature->GetName(), data_1, data_2, AIorScript.c_str());
        return true;
    }

    //npc follow handling
    static bool HandleNpcFollowCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        Creature* creature = handler->getSelectedCreature();

        if (!creature)
        {
            handler->PSendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // Follow player - Using pet's default dist and angle
        creature->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, creature->GetFollowAngle());

        handler->PSendSysMessage(LANG_CREATURE_FOLLOW_YOU_NOW, creature->GetName());
        return true;
    }

    static bool HandleNpcInfoCommand(ChatHandler* handler, const char* /*args*/)
    {
        Creature* target = handler->getSelectedCreature();

        if (!target)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        CreatureTemplate const* cInfo = target->GetCreatureTemplate();

        uint32 faction = target->getFaction();
        uint32 npcflags = target->GetUInt32Value(UNIT_FIELD_NPC_FLAGS);
        uint32 mechanicImmuneMask = cInfo->MechanicImmuneMask;
        uint32 displayid = target->GetDisplayId();
        uint32 nativeid = target->GetNativeDisplayId();
        uint32 Entry = target->GetEntry();

        int64 curRespawnDelay = target->GetRespawnTimeEx()-time(NULL);
        if (curRespawnDelay < 0)
            curRespawnDelay = 0;
        std::string curRespawnDelayStr = secsToTimeString(uint64(curRespawnDelay), true);
        std::string defRespawnDelayStr = secsToTimeString(target->GetRespawnDelay(), true);

        handler->PSendSysMessage(LANG_NPCINFO_CHAR,  target->GetDBTableGUIDLow(), target->GetGUIDLow(), faction, npcflags, Entry, displayid, nativeid);
        handler->PSendSysMessage(LANG_NPCINFO_LEVEL, target->getLevel());
        handler->PSendSysMessage("EquipmentId: %u (Original: %u).", target->GetCurrentEquipmentId(), target->GetOriginalEquipmentId());
        handler->PSendSysMessage(LANG_NPCINFO_HEALTH, target->GetCreateHealth(), target->GetMaxHealth(), target->GetHealth());

        handler->PSendSysMessage(LANG_NPCINFO_UNIT_FIELD_FLAGS, target->GetUInt32Value(UNIT_FIELD_FLAGS));
        for (uint8 i = 0; i < UNIT_FLAGS_MAX; ++i)
            if (target->GetUInt32Value(UNIT_FIELD_FLAGS) & unitFlags[i].flag)
                handler->PSendSysMessage(unitFlags[i].text, unitFlags[i].flag);

        handler->PSendSysMessage(LANG_NPCINFO_FLAGS, target->GetUInt32Value(UNIT_FIELD_FLAGS_2), target->GetUInt32Value(OBJECT_FIELD_DYNAMIC_FLAGS), target->getFaction());
        handler->PSendSysMessage(LANG_COMMAND_RESPAWNTIMES, defRespawnDelayStr.c_str(), curRespawnDelayStr.c_str());
        handler->PSendSysMessage(LANG_NPCINFO_LOOT,  cInfo->lootid, cInfo->pickpocketLootId, cInfo->SkinLootId);
        handler->PSendSysMessage(LANG_NPCINFO_DUNGEON_ID, target->GetInstanceId());
        handler->PSendSysMessage(LANG_NPCINFO_PHASEMASK, target->GetPhaseMask());
        handler->PSendSysMessage(LANG_NPCINFO_ARMOR, target->GetArmor());
        handler->PSendSysMessage(LANG_NPCINFO_POSITION, float(target->GetPositionX()), float(target->GetPositionY()), float(target->GetPositionZ()));
        handler->PSendSysMessage(LANG_NPCINFO_AIINFO, target->GetAIName().c_str(), target->GetScriptName().c_str());

        if (npcflags & UNIT_NPC_FLAG_VENDOR)
            handler->SendSysMessage(LANG_NPCINFO_VENDOR);

        if (npcflags & UNIT_NPC_FLAG_TRAINER)
            handler->SendSysMessage(LANG_NPCINFO_TRAINER);

         handler->PSendSysMessage(LANG_NPCINFO_MECHANIC_IMMUNE, mechanicImmuneMask);
        for (uint8 i = 0; i < MECHANIC_MAX; ++i)
            if ((mechanicImmuneMask << 1) & mechanicImmunes[i].flag)
                handler->PSendSysMessage(mechanicImmunes[i].text, mechanicImmunes[i].flag);

        return true;
    }

    //move selected creature
    static bool HandleNpcMoveCommand(ChatHandler* handler, const char* args)
    {
        uint32 lowguid = 0;

        Creature* creature = handler->getSelectedCreature();

        if (!creature)
        {
            // number or [name] Shift-click form |color|Hcreature:creature_guid|h[name]|h|r
            char* cId = handler->extractKeyFromLink((char*)args, "Hcreature");
            if (!cId)
                return false;

            lowguid = atoi(cId);

            /* FIXME: impossible without entry
            if (lowguid)
                creature = ObjectAccessor::GetCreature(*handler->GetSession()->GetPlayer(), MAKE_GUID(lowguid, HIGHGUID_UNIT));
            */

            // Attempting creature load from DB data
            if (!creature)
            {
                CreatureData const* data = sObjectMgr->GetCreatureData(lowguid);
                if (!data)
                {
                    handler->PSendSysMessage(LANG_COMMAND_CREATGUIDNOTFOUND, lowguid);
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                uint32 map_id = data->mapid;

                if (handler->GetSession()->GetPlayer()->GetMapId() != map_id)
                {
                    handler->PSendSysMessage(LANG_COMMAND_CREATUREATSAMEMAP, lowguid);
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }
            else
            {
                lowguid = creature->GetDBTableGUIDLow();
            }
        }
        else
        {
            lowguid = creature->GetDBTableGUIDLow();
        }

        float x = handler->GetSession()->GetPlayer()->GetPositionX();
        float y = handler->GetSession()->GetPlayer()->GetPositionY();
        float z = handler->GetSession()->GetPlayer()->GetPositionZ();
        float o = handler->GetSession()->GetPlayer()->GetOrientation();

        if (creature)
        {
            if (CreatureData const* data = sObjectMgr->GetCreatureData(creature->GetDBTableGUIDLow()))
            {
                const_cast<CreatureData*>(data)->posX = x;
                const_cast<CreatureData*>(data)->posY = y;
                const_cast<CreatureData*>(data)->posZ = z;
                const_cast<CreatureData*>(data)->orientation = o;
            }

            creature->SetPosition(x, y, z, o);
            creature->Relocate(x, y, z, o);

            Position l_NewPosition;
            creature->GetPosition(&l_NewPosition);
            creature->SendTeleportPacket(l_NewPosition);

            if (creature->isAlive())                            // dead creature will reset movement generator at respawn
            {
                creature->setDeathState(JUST_DIED);
                creature->Respawn();
            }
        }

        handler->PSendSysMessage(LANG_COMMAND_CREATUREMOVED);
        return true;
    }

    //play npc emote
    static bool HandleNpcPlayEmoteCommand(ChatHandler* handler, const char* args)
    {
        uint32 emote = atoi((char*)args);

        Creature* target = handler->getSelectedCreature();
        if (!target)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target->GetTransport() && target->GetGUIDTransport())
        {
            PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_UPD_CREATURE_TRANSPORT_EMOTE);

            stmt->setInt32(0, int32(emote));
            stmt->setInt32(1, target->GetTransport()->GetEntry());
            stmt->setInt32(2, target->GetGUIDTransport());

            WorldDatabase.Execute(stmt);
        }

        target->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, emote);

        return true;
    }

    //set model of creature
    static bool HandleNpcSetModelCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        uint32 displayId = (uint32) atoi((char*)args);

        Creature* creature = handler->getSelectedCreature();

        if (!creature || creature->isPet())
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        creature->SetDisplayId(displayId);
        creature->SetNativeDisplayId(displayId);

        creature->SaveToDB();

        return true;
    }

    /**HandleNpcSetMoveTypeCommand
    * Set the movement type for an NPC.<br/>
    * <br/>
    * Valid movement types are:
    * <ul>
    * <li> stay - NPC wont move </li>
    * <li> random - NPC will move randomly according to the spawndist </li>
    * <li> way - NPC will move with given waypoints set </li>
    * </ul>
    * additional parameter: NODEL - so no waypoints are deleted, if you
    *                       change the movement type
    */
    /// @todo refactor this command and other move command lowguidi never used
    static bool HandleNpcSetMoveTypeCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        // 3 arguments:
        // GUID (optional - you can also select the creature)
        // stay|random|way (determines the kind of movement)
        // NODEL (optional - tells the system NOT to delete any waypoints)
        //        this is very handy if you want to do waypoints, that are
        //        later switched on/off according to special events (like escort
        //        quests, etc)
        char* guid_str = strtok((char*)args, " ");
        char* type_str = strtok((char*)NULL, " ");
        char* dontdel_str = strtok((char*)NULL, " ");

        bool doNotDelete = false;

        if (!guid_str)
            return false;

        uint32 lowguid = 0;
        Creature* creature = NULL;

        if (dontdel_str)
        {
            //sLog->outError(LOG_FILTER_GENERAL, "DEBUG: All 3 params are set");

            // All 3 params are set
            // GUID
            // type
            // doNotDEL
            if (stricmp(dontdel_str, "NODEL") == 0)
            {
                //sLog->outError(LOG_FILTER_GENERAL, "DEBUG: doNotDelete = true;");
                doNotDelete = true;
            }
        }
        else
        {
            // Only 2 params - but maybe NODEL is set
            if (type_str)
            {
                sLog->outError(LOG_FILTER_GENERAL, "DEBUG: Only 2 params ");
                if (stricmp(type_str, "NODEL") == 0)
                {
                    //sLog->outError(LOG_FILTER_GENERAL, "DEBUG: type_str, NODEL ");
                    doNotDelete = true;
                    type_str = NULL;
                }
            }
        }

        if (!type_str)                                           // case .setmovetype $move_type (with selected creature)
        {
            type_str = guid_str;
            creature = handler->getSelectedCreature();
            if (!creature || creature->isPet())
                return false;
            lowguid = creature->GetDBTableGUIDLow();
        }
        else                                                    // case .setmovetype #creature_guid $move_type (with selected creature)
        {
            lowguid = atoi((char*)guid_str);

            /* impossible without entry
            if (lowguid)
                creature = ObjectAccessor::GetCreature(*handler->GetSession()->GetPlayer(), MAKE_GUID(lowguid, HIGHGUID_UNIT));
            */

            // attempt check creature existence by DB data
            if (!creature)
            {
                CreatureData const* data = sObjectMgr->GetCreatureData(lowguid);
                if (!data)
                {
                    handler->PSendSysMessage(LANG_COMMAND_CREATGUIDNOTFOUND, lowguid);
                    handler->SetSentErrorMessage(true);
                    return false;
                }
            }
            else
            {
                lowguid = creature->GetDBTableGUIDLow();
            }
        }

        // now lowguid is low guid really existed creature
        // and creature point (maybe) to this creature or NULL

        MovementGeneratorType move_type;

        std::string type = type_str;

        if (type == "stay")
            move_type = IDLE_MOTION_TYPE;
        else if (type == "random")
            move_type = RANDOM_MOTION_TYPE;
        else if (type == "way")
            move_type = WAYPOINT_MOTION_TYPE;
        else
            return false;

        // update movement type
        //if (doNotDelete == false)
        //    WaypointMgr.DeletePath(lowguid);

        if (creature)
        {
            // update movement type
            if (doNotDelete == false)
                creature->LoadPath(0);

            creature->SetDefaultMovementType(move_type);
            creature->GetMotionMaster()->Initialize();
            if (creature->isAlive())                            // dead creature will reset movement generator at respawn
            {
                creature->setDeathState(JUST_DIED);
                creature->Respawn();
            }
            creature->SaveToDB();
        }
        if (doNotDelete == false)
        {
            handler->PSendSysMessage(LANG_MOVE_TYPE_SET, type_str);
        }
        else
        {
            handler->PSendSysMessage(LANG_MOVE_TYPE_SET_NODEL, type_str);
        }

        return true;
    }

    //npc phasemask handling
    //change phasemask of creature or pet
    static bool HandleNpcSetPhaseCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        uint32 phasemask = (uint32) atoi((char*)args);
        if (phasemask == 0)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Creature* creature = handler->getSelectedCreature();
        if (!creature)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        creature->SetPhaseMask(phasemask, true);

        if (!creature->isPet())
            creature->SaveToDB();

        return true;
    }

    //set spawn dist of creature
    static bool HandleNpcSetSpawnDistCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        float option = (float)(atof((char*)args));
        if (option < 0.0f)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            return false;
        }

        MovementGeneratorType mtype = IDLE_MOTION_TYPE;
        if (option >0.0f)
            mtype = RANDOM_MOTION_TYPE;

        Creature* creature = handler->getSelectedCreature();
        uint32 guidLow = 0;

        if (creature)
            guidLow = creature->GetDBTableGUIDLow();
        else
            return false;

        creature->SetRespawnRadius((float)option);
        creature->SetDefaultMovementType(mtype);
        creature->GetMotionMaster()->Initialize();
        if (creature->isAlive())                                // dead creature will reset movement generator at respawn
        {
            creature->setDeathState(JUST_DIED);
            creature->Respawn();
        }

        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_UPD_CREATURE_SPAWN_DISTANCE);

        stmt->setFloat(0, option);
        stmt->setUInt8(1, uint8(mtype));
        stmt->setUInt32(2, guidLow);

        WorldDatabase.Execute(stmt);

        handler->PSendSysMessage(LANG_COMMAND_SPAWNDIST, option);
        return true;
    }

    //spawn time handling
    static bool HandleNpcSetSpawnTimeCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        char* stime = strtok((char*)args, " ");

        if (!stime)
            return false;

        int spawnTime = atoi((char*)stime);

        if (spawnTime < 0)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Creature* creature = handler->getSelectedCreature();
        uint32 guidLow = 0;

        if (creature)
            guidLow = creature->GetDBTableGUIDLow();
        else
            return false;

        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_UPD_CREATURE_SPAWN_TIME_SECS);

        stmt->setUInt32(0, uint32(spawnTime));
        stmt->setUInt32(1, guidLow);

        WorldDatabase.Execute(stmt);

        creature->SetRespawnDelay((uint32)spawnTime);
        handler->PSendSysMessage(LANG_COMMAND_SPAWNTIME, spawnTime);

        return true;
    }

    static bool HandleNpcSayCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        Creature* creature = handler->getSelectedCreature();
        if (!creature)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        creature->MonsterSay(args, LANG_UNIVERSAL, 0);

        // make some emotes
        char lastchar = args[strlen(args) - 1];
        switch (lastchar)
        {
        case '?':   creature->HandleEmoteCommand(EMOTE_ONESHOT_QUESTION);      break;
        case '!':   creature->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);   break;
        default:    creature->HandleEmoteCommand(EMOTE_ONESHOT_TALK);          break;
        }

        return true;
    }

    //show text emote by creature in chat
    static bool HandleNpcTextEmoteCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        Creature* creature = handler->getSelectedCreature();

        if (!creature)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        creature->MonsterTextEmote(args, 0);

        return true;
    }

    //npc unfollow handling
    static bool HandleNpcUnFollowCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        Creature* creature = handler->getSelectedCreature();

        if (!creature)
        {
            handler->PSendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (/*creature->GetMotionMaster()->empty() ||*/
            creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != FOLLOW_MOTION_TYPE)
        {
            handler->PSendSysMessage(LANG_CREATURE_NOT_FOLLOW_YOU, creature->GetName());
            handler->SetSentErrorMessage(true);
            return false;
        }

        FollowMovementGenerator<Creature> const* mgen = static_cast<FollowMovementGenerator<Creature> const*>((creature->GetMotionMaster()->top()));

        if (mgen->GetTarget() != player)
        {
            handler->PSendSysMessage(LANG_CREATURE_NOT_FOLLOW_YOU, creature->GetName());
            handler->SetSentErrorMessage(true);
            return false;
        }

        // reset movement
        creature->GetMotionMaster()->MovementExpired(true);

        handler->PSendSysMessage(LANG_CREATURE_NOT_FOLLOW_YOU_NOW, creature->GetName());
        return true;
    }

    // make npc whisper to player
    static bool HandleNpcWhisperCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        char* receiver_str = strtok((char*)args, " ");
        char* text = strtok(NULL, "");

        uint64 guid = handler->GetSession()->GetPlayer()->GetSelection();
        Creature* creature = handler->GetSession()->GetPlayer()->GetMap()->GetCreature(guid);

        if (!creature || !receiver_str || !text)
        {
            return false;
        }

        uint64 receiver_guid= atol(receiver_str);

        // check online security
        if (handler->HasLowerSecurity(ObjectAccessor::FindPlayer(receiver_guid), 0))
            return false;

        creature->MonsterWhisper(text, receiver_guid);

        return true;
    }

    static bool HandleNpcYellCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        Creature* creature = handler->getSelectedCreature();
        if (!creature)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        creature->MonsterYell(args, LANG_UNIVERSAL, 0);

        // make an emote
        creature->HandleEmoteCommand(EMOTE_ONESHOT_SHOUT);

        return true;
    }

    // add creature, temp only
    static bool HandleNpcAddTempSpawnCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;
        char* charID = strtok((char*)args, " ");
        if (!charID)
            return false;

        Player* chr = handler->GetSession()->GetPlayer();

        uint32 id = atoi(charID);
        if (!id)
            return false;

        chr->SummonCreature(id, *chr, TEMPSUMMON_CORPSE_DESPAWN, 120);

        return true;
    }

    //npc tame handling
    static bool HandleNpcTameCommand(ChatHandler* handler, const char* /*args*/)
    {
        Creature* creatureTarget = handler->getSelectedCreature();
        if (!creatureTarget || creatureTarget->isPet())
        {
            handler->PSendSysMessage (LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage (true);
            return false;
        }

        Player* player = handler->GetSession()->GetPlayer();

        if (player->GetPetGUID())
        {
            handler->SendSysMessage (LANG_YOU_ALREADY_HAVE_PET);
            handler->SetSentErrorMessage (true);
            return false;
        }

        CreatureTemplate const* cInfo = creatureTarget->GetCreatureTemplate();

        if (!cInfo->isTameable (player->CanTameExoticPets()))
        {
            handler->PSendSysMessage (LANG_CREATURE_NON_TAMEABLE, cInfo->Entry);
            handler->SetSentErrorMessage (true);
            return false;
        }

        // Everything looks OK, create new pet
        Pet* pet = player->CreateTamedPetFrom(creatureTarget);
        if (!pet)
        {
            handler->PSendSysMessage (LANG_CREATURE_NON_TAMEABLE, cInfo->Entry);
            handler->SetSentErrorMessage (true);
            return false;
        }

        // place pet before player
        float x, y, z;
        player->GetClosePoint (x, y, z, creatureTarget->GetObjectSize(), CONTACT_DISTANCE);
        pet->Relocate(x, y, z, M_PI-player->GetOrientation());

        // set pet to defensive mode by default (some classes can't control controlled pets in fact).
        pet->SetReactState(REACT_DEFENSIVE);

        // calculate proper level
        uint8 level = (creatureTarget->getLevel() < (player->getLevel() - 5)) ? (player->getLevel() - 5) : creatureTarget->getLevel();

        // prepare visual effect for levelup
        pet->SetUInt32Value(UNIT_FIELD_LEVEL, level - 1);

        // add to world
        pet->GetMap()->AddToMap(pet->ToCreature());

        // visual effect for levelup
        pet->SetUInt32Value(UNIT_FIELD_LEVEL, level);

        // caster have pet now
        player->SetMinion(pet, true, PET_SLOT_UNK_SLOT, pet->m_Stampeded);

        pet->SavePetToDB(PET_SLOT_ACTUAL_PET_SLOT, pet->m_Stampeded);
        player->PetSpellInitialize();

        return true;
    }

    static bool HandleNpcAddFormationCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        uint32 leaderGUID = (uint32) atoi((char*)args);
        Creature* creature = handler->getSelectedCreature();

        if (!creature || !creature->GetDBTableGUIDLow())
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 lowguid = creature->GetDBTableGUIDLow();
        if (creature->GetFormation())
        {
            handler->PSendSysMessage("Selected creature is already member of group %u", creature->GetFormation()->GetId());
            return false;
        }

        if (!lowguid)
            return false;

        Player* chr = handler->GetSession()->GetPlayer();
        FormationInfo* group_member;

        group_member                 = new FormationInfo;
        group_member->follow_angle   = (creature->GetAngle(chr) - chr->GetOrientation()) * 180 / M_PI;
        group_member->follow_dist    = sqrtf(pow(chr->GetPositionX() - creature->GetPositionX(), int(2))+pow(chr->GetPositionY() - creature->GetPositionY(), int(2)));
        group_member->leaderGUID     = leaderGUID;
        group_member->groupAI        = 0;

        sFormationMgr->CreatureGroupMap[lowguid] = group_member;
        creature->SearchFormation();

        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_INS_CREATURE_FORMATION);

        stmt->setUInt32(0, leaderGUID);
        stmt->setUInt32(1, lowguid);
        stmt->setFloat(2, group_member->follow_dist);
        stmt->setFloat(3, group_member->follow_angle);
        stmt->setUInt32(4, uint32(group_member->groupAI));

        WorldDatabase.Execute(stmt);

        handler->PSendSysMessage("Creature %u added to formation with leader %u", lowguid, leaderGUID);

        return true;
    }

    static bool HandleNpcSetLinkCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        uint32 linkguid = (uint32) atoi((char*)args);

        Creature* creature = handler->getSelectedCreature();

        if (!creature)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!creature->GetDBTableGUIDLow())
        {
            handler->PSendSysMessage("Selected creature %u isn't in creature table", creature->GetGUIDLow());
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sObjectMgr->SetCreatureLinkedRespawn(creature->GetDBTableGUIDLow(), linkguid))
        {
            handler->PSendSysMessage("Selected creature can't link with guid '%u'", linkguid);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage("LinkGUID '%u' added to creature with DBTableGUID: '%u'", linkguid, creature->GetDBTableGUIDLow());
        return true;
    }

    //TODO: NpcCommands that need to be fixed :
    static bool HandleNpcAddWeaponCommand(ChatHandler* /*handler*/, const char* /*args*/)
    {
        /*if (!*args)
            return false;

        uint64 guid = handler->GetSession()->GetPlayer()->GetSelection();
        if (guid == 0)
        {
            handler->SendSysMessage(LANG_NO_SELECTION);
            return true;
        }

        Creature* creature = ObjectAccessor::GetCreature(*handler->GetSession()->GetPlayer(), guid);

        if (!creature)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            return true;
        }

        char* pSlotID = strtok((char*)args, " ");
        if (!pSlotID)
            return false;

        char* pItemID = strtok(NULL, " ");
        if (!pItemID)
            return false;

        uint32 ItemID = atoi(pItemID);
        uint32 SlotID = atoi(pSlotID);

        ItemTemplate* tmpItem = sObjectMgr->GetItemTemplate(ItemID);

        bool added = false;
        if (tmpItem)
        {
            switch (SlotID)
            {
                case 1:
                    creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, ItemID);
                    added = true;
                    break;
                case 2:
                    creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, ItemID);
                    added = true;
                    break;
                case 3:
                    creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_02, ItemID);
                    added = true;
                    break;
                default:
                    handler->PSendSysMessage(LANG_ITEM_SLOT_NOT_EXIST, SlotID);
                    added = false;
                    break;
            }

            if (added)
                handler->PSendSysMessage(LANG_ITEM_ADDED_TO_SLOT, ItemID, tmpItem->Name1, SlotID);
        }
        else
        {
            handler->PSendSysMessage(LANG_ITEM_NOT_FOUND, ItemID);
            return true;
        }
        */
        return true;
    }

    static bool HandleNpcSetNameCommand(ChatHandler* /*handler*/, const char* /*args*/)
    {
        /* Temp. disabled
        if (!*args)
            return false;

        if (strlen((char*)args)>75)
        {
            handler->PSendSysMessage(LANG_TOO_LONG_NAME, strlen((char*)args)-75);
            return true;
        }

        for (uint8 i = 0; i < strlen(args); ++i)
        {
            if (!isalpha(args[i]) && args[i] != ' ')
            {
                handler->SendSysMessage(LANG_CHARS_ONLY);
                return false;
            }
        }

        uint64 guid;
        guid = handler->GetSession()->GetPlayer()->GetSelection();
        if (guid == 0)
        {
            handler->SendSysMessage(LANG_NO_SELECTION);
            return true;
        }

        Creature* creature = ObjectAccessor::GetCreature(*handler->GetSession()->GetPlayer(), guid);

        if (!creature)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            return true;
        }

        creature->SetName(args);
        uint32 idname = sObjectMgr->AddCreatureTemplate(creature->GetName());
        creature->SetUInt32Value(OBJECT_FIELD_ENTRY, idname);

        creature->SaveToDB();
        */

        return true;
    }

    static bool HandleNpcSetSubNameCommand(ChatHandler* /*handler*/, const char* /*args*/)
    {
        /* Temp. disabled

        if (!*args)
            args = "";

        if (strlen((char*)args)>75)
        {
            handler->PSendSysMessage(LANG_TOO_LONG_SUBNAME, strlen((char*)args)-75);
            return true;
        }

        for (uint8 i = 0; i < strlen(args); i++)
        {
            if (!isalpha(args[i]) && args[i] != ' ')
            {
                handler->SendSysMessage(LANG_CHARS_ONLY);
                return false;
            }
        }
        uint64 guid;
        guid = handler->GetSession()->GetPlayer()->GetSelection();
        if (guid == 0)
        {
            handler->SendSysMessage(LANG_NO_SELECTION);
            return true;
        }

        Creature* creature = ObjectAccessor::GetCreature(*handler->GetSession()->GetPlayer(), guid);

        if (!creature)
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            return true;
        }

        uint32 idname = sObjectMgr->AddCreatureSubName(creature->GetName(), args, creature->GetUInt32Value(UNIT_FIELD_DISPLAY_ID));
        creature->SetUInt32Value(OBJECT_FIELD_ENTRY, idname);

        creature->SaveToDB();
        */
        return true;
    }

    static bool HandleNpcSetAnimKitCommand(ChatHandler* p_Handler, char const* p_Args)
    {
        if (!*p_Args)
            return false;

        uint32 l_ID = atoi((char*)p_Args);
        Creature* l_Target = p_Handler->getSelectedCreature();
        if (!l_Target)
        {
            p_Handler->SendSysMessage(LANG_SELECT_CREATURE);
            p_Handler->SetSentErrorMessage(true);
            return false;
        }

        if (l_Target->GetEntry() == 1)
        {
            p_Handler->PSendSysMessage("%s%s|r", "|cffff33ff", "You want to load path to a waypoint? Aren't you?");
            p_Handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 l_GuidLow = l_Target->GetDBTableGUIDLow();

        PreparedStatement* l_Statement = WorldDatabase.GetPreparedStatement(WorldDatabaseStatements::WORLD_SEL_CREATURE_ADDON_BY_GUID);
        l_Statement->setUInt32(0, l_GuidLow);

        PreparedQueryResult l_Result = WorldDatabase.Query(l_Statement);
        if (l_Result)
        {
            l_Statement = WorldDatabase.GetPreparedStatement(WorldDatabaseStatements::WORLD_UPD_CREATURE_ADDON_ANIMKIT);
            l_Statement->setUInt32(0, l_ID);
            l_Statement->setUInt32(1, l_GuidLow);
        }
        else
        {
            l_Statement = WorldDatabase.GetPreparedStatement(WorldDatabaseStatements::WORLD_INS_CREATURE_ADDON_BY_ANIMKIT);
            l_Statement->setUInt32(0, l_GuidLow);
            l_Statement->setUInt32(1, l_ID);
        }

        WorldDatabase.Execute(l_Statement);

        l_Target->SetAIAnimKitId(l_ID);
        return true;
    }

    static bool HandleNpcSetEmoteStateCommand(ChatHandler* p_Handler, char const* p_Args)
    {
        if (!*p_Args)
            return false;

        uint32 l_ID = atoi((char*)p_Args);
        Creature* l_Target = p_Handler->getSelectedCreature();
        if (!l_Target)
        {
            p_Handler->SendSysMessage(LANG_SELECT_CREATURE);
            p_Handler->SetSentErrorMessage(true);
            return false;
        }

        if (l_Target->GetEntry() == 1)
        {
            p_Handler->PSendSysMessage("%s%s|r", "|cffff33ff", "You want to load path to a waypoint? Aren't you?");
            p_Handler->SetSentErrorMessage(true);
            return false;
        }

        if (sEmotesStore.LookupEntry(l_ID) == nullptr)
        {
            p_Handler->PSendSysMessage("The Emote you're trying to set doesn't exists!");
            p_Handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 l_GuidLow = l_Target->GetDBTableGUIDLow();

        PreparedStatement* l_Statement = WorldDatabase.GetPreparedStatement(WorldDatabaseStatements::WORLD_SEL_CREATURE_ADDON_BY_GUID);
        l_Statement->setUInt32(0, l_GuidLow);

        PreparedQueryResult l_Result = WorldDatabase.Query(l_Statement);
        if (l_Result)
        {
            l_Statement = WorldDatabase.GetPreparedStatement(WorldDatabaseStatements::WORLD_UPD_CREATURE_ADDON_EMOTESTATE);
            l_Statement->setUInt32(0, l_ID);
            l_Statement->setUInt32(1, l_GuidLow);
        }
        else
        {
            l_Statement = WorldDatabase.GetPreparedStatement(WorldDatabaseStatements::WORLD_INS_CREATURE_ADDON_BY_EMOTESTATE);
            l_Statement->setUInt32(0, l_GuidLow);
            l_Statement->setUInt32(1, l_ID);
        }

        WorldDatabase.Execute(l_Statement);

        l_Target->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, l_ID);
        return true;
    }

    static bool HandleNpcActivateCommand(ChatHandler* handler, const char* args)
    {
        Creature* unit = NULL;

        if (*args)
        {
            // number or [name] Shift-click form |color|Hcreature:creature_guid|h[name]|h|r
            char* cId = handler->extractKeyFromLink((char*)args, "Hcreature");
            if (!cId)
                return false;

            uint32 lowguid = atoi(cId);
            if (!lowguid)
                return false;

            if (CreatureData const* cr_data = sObjectMgr->GetCreatureData(lowguid))
                unit = handler->GetSession()->GetPlayer()->GetMap()->GetCreature(MAKE_NEW_GUID(lowguid, cr_data->id, HIGHGUID_UNIT));
        }
        else
            unit = handler->getSelectedCreature();

        if (!unit || unit->isPet() || unit->isTotem())
        {
            handler->SendSysMessage(LANG_SELECT_CREATURE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // Activate
        unit->setActive(!unit->isActiveObject());
        WorldDatabase.PExecute("UPDATE creature SET isActive = %u WHERE guid = %u", uint8(unit->isActiveObject()), unit->GetGUIDLow());

        if (unit->isActiveObject())
            handler->PSendSysMessage("Creature added to actived creatures !");
        else
            handler->PSendSysMessage("Creature removed from actived creatures !");

        return true;
    }

    static bool HandleNpcNearCommand(ChatHandler* handler, char const* args)
    {
        float distance = (!*args) ? 10.0f : (float)(atof(args));
        uint32 count = 0;

        Player* player = handler->GetSession()->GetPlayer();

        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_CREATURE_NEAREST);
        stmt->setFloat(0, player->GetPositionX());
        stmt->setFloat(1, player->GetPositionY());
        stmt->setFloat(2, player->GetPositionZ());
        stmt->setUInt32(3, player->GetMapId());
        stmt->setFloat(4, player->GetPositionX());
        stmt->setFloat(5, player->GetPositionY());
        stmt->setFloat(6, player->GetPositionZ());
        stmt->setFloat(7, distance * distance);
        PreparedQueryResult result = WorldDatabase.Query(stmt);

        if (result)
        {
            do
            {
                Field* fields = result->Fetch();
                uint32 guid = fields[0].GetUInt32();
                uint32 entry = fields[1].GetUInt32();
                float x = fields[2].GetFloat();
                float y = fields[3].GetFloat();
                float z = fields[4].GetFloat();
                uint16 mapId = fields[5].GetUInt16();

                CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(entry);

                if (!cInfo)
                    continue;

                handler->PSendSysMessage(LANG_NPC_LIST_CHAT, guid, entry, guid, cInfo->Name.c_str(), x, y, z, mapId);

                ++count;
            }
            while (result->NextRow());
        }

        handler->PSendSysMessage(LANG_COMMAND_NEAROBJMESSAGE, distance, count);
        return true;
    }

    static bool HandleNpcGroupScaling(ChatHandler* p_Handler, char const* p_Args)
    {
        Creature* l_Creature = NULL;

        if (*p_Args)
        {
            // number or [name] Shift-click form |color|Hcreature:creature_guid|h[name]|h|r
            char* l_CreatureID = p_Handler->extractKeyFromLink((char*)p_Args, "Hcreature");
            if (!l_CreatureID)
                return false;

            uint32 l_LowGUID = atoi(l_CreatureID);
            if (!l_LowGUID)
                return false;

            if (CreatureData const* l_CreatureData = sObjectMgr->GetCreatureData(l_LowGUID))
                l_Creature = p_Handler->GetSession()->GetPlayer()->GetMap()->GetCreature(MAKE_NEW_GUID(l_LowGUID, l_CreatureData->id, HIGHGUID_UNIT));
        }
        else
            l_Creature = p_Handler->getSelectedCreature();

        if (!l_Creature || l_Creature->isPet() || l_Creature->isTotem())
        {
            p_Handler->SendSysMessage(LANG_SELECT_CREATURE);
            p_Handler->SetSentErrorMessage(true);
            return false;
        }

        l_Creature->UpdateGroupSizeStats();
        return true;
    }

    static bool HandleNpcListAggro(ChatHandler* p_Handler, char const* /*p_Args*/)
    {
        if (Creature* l_Creature = p_Handler->getSelectedCreature())
        {
            std::list<HostileReference*> l_Aggro = l_Creature->getThreatManager().GetThreatList();

            for (auto l_Threat : l_Aggro)
            {
                if (l_Threat->getSource())
                {
                    if (Unit* l_Attacker = l_Threat->getTarget())
                        p_Handler->PSendSysMessage("Name : %s, Aggro : %f", l_Attacker->GetName(), l_Threat->getThreat());
                }
            }
        }
        else
            p_Handler->PSendSysMessage("Please select some creature !");

        return true;
    }
};

#ifndef __clang_analyzer__
void AddSC_npc_commandscript()
{
    new npc_commandscript();
}
#endif
