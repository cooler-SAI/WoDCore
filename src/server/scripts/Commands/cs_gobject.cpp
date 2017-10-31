////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "GameEventMgr.h"
#include "ObjectMgr.h"
#include "PoolMgr.h"
#include "MapManager.h"
#include "Chat.h"

class gobject_commandscript: public CommandScript
{
    public:
        gobject_commandscript() : CommandScript("gobject_commandscript") { }

        ChatCommand* GetCommands() const
        {
            static ChatCommand gobjectAddCommandTable[] =
            {
                { "temp",           SEC_GAMEMASTER,     false, &HandleGameObjectAddTempCommand,   "", NULL },
                { "",               SEC_GAMEMASTER,     false, &HandleGameObjectAddCommand,       "", NULL },
                { NULL,             0,                  false, NULL,                              "", NULL }
            };
            static ChatCommand gobjectSetCommandTable[] =
            {
                { "phase",          SEC_GAMEMASTER,     false, &HandleGameObjectSetPhaseCommand,  "", NULL },
                { "state",          SEC_GAMEMASTER,     false, &HandleGameObjectSetStateCommand,  "", NULL },
                { "flags",          SEC_ADMINISTRATOR,  false, &HandleGameObjectSetFlagsCommand,  "", NULL },
                { "field",          SEC_ADMINISTRATOR,  false, &HandleGameObjectSetFieldCommand,  "", NULL },
                { "animkit",        SEC_ADMINISTRATOR,  false, &HandleGameObjectSetAnimKitCommand,"", NULL },
                { "activateanim",   SEC_ADMINISTRATOR,  false, &HandleGameObjectActivateAnim,     "", NULL },
                { "destructible",   SEC_ADMINISTRATOR,  false, &HandleGameObjectSetDestructible,  "", NULL },
                { NULL,             0,                  false, NULL,                              "", NULL }
            };
            static ChatCommand gobjectGetCommandTable[] =
            {
                { "state",          SEC_GAMEMASTER,     false, &HandleGameObjectGetStateCommand,  "", NULL },
                { "flags",          SEC_ADMINISTRATOR,  false, &HandleGameObjectGetFlagsCommand,  "", NULL },
                { NULL,             0,                  false, NULL,                              "", NULL }
            };
            static ChatCommand gobjectCommandTable[] =
            {
                { "activate",       SEC_GAMEMASTER,     false, &HandleGameObjectActivateCommand,  "", NULL },
                { "map_activate",   SEC_GAMEMASTER,     false, &HandleGameObjectActivateMapCommand, "", NULL },
                { "delete",         SEC_GAMEMASTER,     false, &HandleGameObjectDeleteCommand,    "", NULL },
                { "info",           SEC_GAMEMASTER,     false, &HandleGameObjectInfoCommand,      "", NULL },
                { "move",           SEC_GAMEMASTER,     false, &HandleGameObjectMoveCommand,      "", NULL },
                { "near",           SEC_GAMEMASTER,     false, &HandleGameObjectNearCommand,      "", NULL },
                { "target",         SEC_GAMEMASTER,     false, &HandleGameObjectTargetCommand,    "", NULL },
                { "turn",           SEC_GAMEMASTER,     false, &HandleGameObjectTurnCommand,      "", NULL },
                { "bindportal",     SEC_GAMEMASTER,     false, &HandleBindPortalCommand,          "", NULL },
                { "damage",         SEC_GAMEMASTER,     false, &HandleDamageObjectCommand,        "", NULL },
                { "add",            SEC_GAMEMASTER,     false, NULL,            "", gobjectAddCommandTable },
                { "set",            SEC_GAMEMASTER,     false, NULL,            "", gobjectSetCommandTable },
                { "get",            SEC_GAMEMASTER,     false, NULL,            "", gobjectGetCommandTable },
                { NULL,             0,                  false, NULL,                              "", NULL }
            };
            static ChatCommand commandTable[] =
            {
                { "gobject",        SEC_GAMEMASTER,     false, NULL,                "", gobjectCommandTable },
                { NULL,             0,                  false, NULL,                               "", NULL }
            };
            return commandTable;
        }

        static bool HandleGameObjectSetDestructible(ChatHandler* p_Handler, char const* p_Args)
        {
            uint32 l_GuidLow = GetGuidLowFromArgsOrLastTargetedGo(p_Handler, p_Args);
            if (!l_GuidLow)
                return false;

            GameObject* l_GoB = nullptr;

            if (GameObjectData const* l_GoBData = sObjectMgr->GetGOData(l_GuidLow))
                l_GoB = p_Handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(l_GuidLow, l_GoBData->id);

            if (!l_GoB)
            {
                p_Handler->PSendSysMessage(TrinityStrings::LANG_COMMAND_OBJNOTFOUND, l_GuidLow);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            char* l_State = strtok(nullptr, " ");
            if (!l_State)
                return false;

            uint32 l_DisplayID = 0;
            char* l_Display = strtok(nullptr, " ");
            if (l_Display)
                l_DisplayID = atoi(l_Display);

            l_GoB->SetDestructibleState(GameObjectDestructibleState(atoi(l_State)), p_Handler->GetSession()->GetPlayer());

            if (l_DisplayID)
                l_GoB->SetDisplayId(l_DisplayID);

            p_Handler->PSendSysMessage("Set gobject destructible state %d", atoi(l_State));
            return true;
        }

        static uint32 GetGuidLowFromArgsOrLastTargetedGo(ChatHandler* handler, char const* args)
        {
            Player* player = handler->GetSession()->GetPlayer();

            if (*args)
            {
                // number or [name] Shift-click form |color|Hgameobject:go_guid|h[name]|h|r
                char* cId = handler->extractKeyFromLink((char*)args,"Hgameobject");
                if (!cId)
                    return 0;

                return atoi(cId);
            }
            else
            {
                if (player->GetLastTargetedGO())
                    return player->GetLastTargetedGO();
                else
                    return 0;
            }
        }

        static bool HandleGameObjectActivateCommand(ChatHandler* handler, char const* args)
        {
            uint32 guidLow = GetGuidLowFromArgsOrLastTargetedGo(handler, args);

            if (!guidLow)
                return false;

            GameObject* object = NULL;

            // by DB guid
            if (GameObjectData const* goData = sObjectMgr->GetGOData(guidLow))
                object = handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(guidLow, goData->id);

            if (!object)
            {
                handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, guidLow);
                handler->SetSentErrorMessage(true);
                return false;
            }

            // Activate
            object->SetLootState(GO_READY);
            object->UseDoorOrButton(10000, false, handler->GetSession()->GetPlayer());

            handler->PSendSysMessage("Object activated!");

            return true;
        }

        static bool HandleGameObjectActivateMapCommand(ChatHandler* handler, const char* args)
        {
            uint32 guidLow = GetGuidLowFromArgsOrLastTargetedGo(handler, args);

            if (!guidLow)
                return false;

            GameObject* obj = NULL;

            // by DB guid
            if (GameObjectData const* go_data = sObjectMgr->GetGOData(guidLow))
                obj = handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(guidLow, go_data->id);

            if (!obj)
            {
                handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, guidLow);
                handler->SetSentErrorMessage(true);
                return false;
            }

            // Activate
            obj->setActive(!obj->isActiveObject());
            WorldDatabase.PExecute("UPDATE gameobject SET isActive = %u WHERE guid = %u", uint8(obj->isActiveObject()), obj->GetGUIDLow());

            if (obj->isActiveObject())
                handler->PSendSysMessage("Object added to actived objects !");
            else
                handler->PSendSysMessage("Object removed from actived objects !");

            return true;
        }

        //spawn go
        static bool HandleGameObjectAddCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            // number or [name] Shift-click form |color|Hgameobject_entry:go_id|h[name]|h|r
            char* id = handler->extractKeyFromLink((char*)args, "Hgameobject_entry");
            if (!id)
                return false;

            uint32 objectId = atol(id);
            if (!objectId)
                return false;

            char* spawntimeSecs = strtok(NULL, " ");

            const GameObjectTemplate* objectInfo = sObjectMgr->GetGameObjectTemplate(objectId);

            if (!objectInfo)
            {
                handler->PSendSysMessage(LANG_GAMEOBJECT_NOT_EXIST, objectId);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (objectInfo->displayId && !sGameObjectDisplayInfoStore.LookupEntry(objectInfo->displayId))
            {
                // report to DB errors log as in loading case
                sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry %u GoType: %u) have invalid displayId (%u), not spawned.", objectId, objectInfo->type, objectInfo->displayId);
                handler->PSendSysMessage(LANG_GAMEOBJECT_HAVE_INVALID_DATA, objectId);
                handler->SetSentErrorMessage(true);
                return false;
            }

            Player* player = handler->GetSession()->GetPlayer();
            float x = float(player->GetPositionX());
            float y = float(player->GetPositionY());
            float z = float(player->GetPositionZ());
            float o = float(player->GetOrientation());
            Map* map = player->GetMap();

            GameObject* object = new GameObject;
            uint32 guidLow = sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT);

            if (!object->Create(guidLow, objectInfo->entry, map, player->GetPhaseMgr().GetPhaseMaskForSpawn(), x, y, z, o, 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY))
            {
                delete object;
                return false;
            }

            if (spawntimeSecs)
            {
                uint32 value = atoi((char*)spawntimeSecs);
                object->SetRespawnTime(value);
            }

            // fill the gameobject data and save to the db
            object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), player->GetPhaseMgr().GetPhaseMaskForSpawn());

            // this will generate a new guid if the object is in an instance
            if (!object->LoadGameObjectFromDB(guidLow, map))
            {
                delete object;
                return false;
            }

            // TODO: is it really necessary to add both the real and DB table guid here ?
            sObjectMgr->AddGameobjectToGrid(guidLow, sObjectMgr->GetGOData(guidLow));

            if (handler->GetSession())
                if (handler->GetSession()->GetPlayer())
                    handler->GetSession()->GetPlayer()->SetLastTargetedGO(guidLow);

            handler->PSendSysMessage(LANG_GAMEOBJECT_ADD, objectId, objectInfo->name.c_str(), guidLow, x, y, z);
            return true;
        }

        // add go, temp only
        static bool HandleGameObjectAddTempCommand(ChatHandler* handler, char const* args)
        {
            if (!*args)
                return false;

            char* id = strtok((char*)args, " ");
            if (!id)
                return false;

            Player* player = handler->GetSession()->GetPlayer();

            char* spawntime = strtok(NULL, " ");
            uint32 spawntm = 300;

            if (spawntime)
                spawntm = atoi((char*)spawntime);

            float x = player->GetPositionX();
            float y = player->GetPositionY();
            float z = player->GetPositionZ();
            float ang = player->GetOrientation();

            float rot2 = std::sin(ang/2);
            float rot3 = std::cos(ang/2);

            uint32 objectId = atoi(id);

            player->SummonGameObject(objectId, x, y, z, ang, 0, 0, rot2, rot3, spawntm);

            return true;
        }

        static bool HandleGameObjectTargetCommand(ChatHandler* handler, char const* args)
        {
            Player* player = handler->GetSession()->GetPlayer();
            QueryResult result;
            GameEventMgr::ActiveEvents const& activeEventsList = sGameEventMgr->GetActiveEventList();

            if (*args)
            {
                // number or [name] Shift-click form |color|Hgameobject_entry:go_id|h[name]|h|r
                char* id = handler->extractKeyFromLink((char*)args, "Hgameobject_entry");
                if (!id)
                    return false;

                uint32 objectId = atol(id);

                if (objectId)
                    result = WorldDatabase.PQuery("SELECT guid, id, position_x, position_y, position_z, orientation, map, phaseMask, (POW(position_x - '%f', 2) + POW(position_y - '%f', 2) + POW(position_z - '%f', 2)) AS order_ FROM gameobject WHERE map = '%i' AND id = '%u' ORDER BY order_ ASC LIMIT 1",
                    player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetMapId(), objectId);
                else
                {
                    std::string name = id;
                    WorldDatabase.EscapeString(name);
                    result = WorldDatabase.PQuery(
                        "SELECT guid, id, position_x, position_y, position_z, orientation, map, phaseMask, (POW(position_x - %f, 2) + POW(position_y - %f, 2) + POW(position_z - %f, 2)) AS order_ "
                        "FROM gameobject, gameobject_template WHERE gameobject_template.entry = gameobject.id AND map = %i AND name " _LIKE_ " " _CONCAT3_ ("'%%'", "'%s'", "'%%'")" ORDER BY order_ ASC LIMIT 1",
                        player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetMapId(), name.c_str());
                }
            }
            else
            {
                std::ostringstream eventFilter;
                eventFilter << " AND (eventEntry IS NULL ";
                bool initString = true;

                for (GameEventMgr::ActiveEvents::const_iterator itr = activeEventsList.begin(); itr != activeEventsList.end(); ++itr)
                {
                    if (initString)
                    {
                        eventFilter  <<  "OR eventEntry IN (" << *itr;
                        initString = false;
                    }
                    else
                        eventFilter << ',' << *itr;
                }

                if (!initString)
                    eventFilter << "))";
                else
                    eventFilter << ')';

                result = WorldDatabase.PQuery("SELECT gameobject.guid, id, position_x, position_y, position_z, orientation, map, phaseMask, "
                    "(POW(position_x - %f, 2) + POW(position_y - %f, 2) + POW(position_z - %f, 2)) AS order_ FROM gameobject "
                    "LEFT OUTER JOIN game_event_gameobject on gameobject.guid = game_event_gameobject.guid WHERE map = '%i' %s ORDER BY order_ ASC LIMIT 10",
                    handler->GetSession()->GetPlayer()->GetPositionX(), handler->GetSession()->GetPlayer()->GetPositionY(), handler->GetSession()->GetPlayer()->GetPositionZ(),
                    handler->GetSession()->GetPlayer()->GetMapId(), eventFilter.str().c_str());
            }

            if (!result)
            {
                handler->SendSysMessage(LANG_COMMAND_TARGETOBJNOTFOUND);
                return true;
            }

            bool found = false;
            float x, y, z, o;
            uint32 guidLow, id;
            uint16 mapId, phase;
            uint32 poolId;

            do
            {
                Field* fields = result->Fetch();
                guidLow = fields[0].GetUInt32();
                id =      fields[1].GetUInt32();
                x =       fields[2].GetFloat();
                y =       fields[3].GetFloat();
                z =       fields[4].GetFloat();
                o =       fields[5].GetFloat();
                mapId =   fields[6].GetUInt16();
                phase =   fields[7].GetUInt16();
                poolId =  sPoolMgr->IsPartOfAPool<GameObject>(guidLow);
                if (!poolId || sPoolMgr->IsSpawnedObject<GameObject>(guidLow))
                    found = true;
            }
            while
                (result->NextRow() && !found);

            if (!found)
            {
                handler->PSendSysMessage(LANG_GAMEOBJECT_NOT_EXIST, id);
                return false;
            }

            GameObjectTemplate const* objectInfo = sObjectMgr->GetGameObjectTemplate(id);

            if (!objectInfo)
            {
                handler->PSendSysMessage(LANG_GAMEOBJECT_NOT_EXIST, id);
                return false;
            }

            GameObject* target = handler->GetSession()->GetPlayer()->GetMap()->GetGameObject(MAKE_NEW_GUID(guidLow, id, HIGHGUID_GAMEOBJECT));

            handler->PSendSysMessage(LANG_GAMEOBJECT_DETAIL, guidLow, objectInfo->name.c_str(), guidLow, id, x, y, z, mapId, o, phase);
            player->SetLastTargetedGO(guidLow);

            if (target)
            {
                int32 curRespawnDelay = int32(target->GetRespawnTimeEx() - time(NULL));
                if (curRespawnDelay < 0)
                    curRespawnDelay = 0;

                std::string curRespawnDelayStr = secsToTimeString(curRespawnDelay, true);
                std::string defRespawnDelayStr = secsToTimeString(target->GetRespawnDelay(), true);

                handler->PSendSysMessage(LANG_COMMAND_RESPAWNTIMES, defRespawnDelayStr.c_str(), curRespawnDelayStr.c_str());
            }
            return true;
        }

        //delete object by selection or guid
        static bool HandleGameObjectDeleteCommand(ChatHandler* handler, char const* args)
        {
            uint32 guidLow = GetGuidLowFromArgsOrLastTargetedGo(handler, args);

            if (!guidLow)
                return false;

            GameObject* object = NULL;

            // by DB guid
            if (GameObjectData const* gameObjectData = sObjectMgr->GetGOData(guidLow))
                object = handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(guidLow, gameObjectData->id);

            if (!object)
            {
                handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, guidLow);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint64 ownerGuid = object->GetOwnerGUID();
            if (ownerGuid)
            {
                Unit* owner = ObjectAccessor::GetUnit(*handler->GetSession()->GetPlayer(), ownerGuid);
                if (!owner || !IS_PLAYER_GUID(ownerGuid))
                {
                    handler->PSendSysMessage(LANG_COMMAND_DELOBJREFERCREATURE, GUID_LOPART(ownerGuid), object->GetGUIDLow());
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                owner->RemoveGameObject(object, false);
            }

            object->SetRespawnTime(0);                                 // not save respawn time
            object->Delete();
            object->DeleteFromDB();

            handler->PSendSysMessage(LANG_COMMAND_DELOBJMESSAGE, object->GetGUIDLow());

            return true;
        }

        //turn selected object
        static bool HandleGameObjectTurnCommand(ChatHandler* handler, char const* args)
        {
            uint32 guidLow = GetGuidLowFromArgsOrLastTargetedGo(handler, args);

            if (!guidLow)
                return false;

            GameObject* object = NULL;

            // by DB guid
            if (GameObjectData const* gameObjectData = sObjectMgr->GetGOData(guidLow))
                object = handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(guidLow, gameObjectData->id);

            if (!object)
            {
                handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, guidLow);
                handler->SetSentErrorMessage(true);
                return false;
            }

            Player* player = handler->GetSession()->GetPlayer();
            float o = player->GetOrientation();

            object->Relocate(object->GetPositionX(), object->GetPositionY(), object->GetPositionZ(), o);
            object->UpdateRotationFields();
            object->DestroyForNearbyPlayers();
            object->UpdateObjectVisibility();

            object->SaveToDB();
            object->Refresh();

            handler->PSendSysMessage(LANG_COMMAND_TURNOBJMESSAGE, object->GetGUIDLow(), object->GetGOInfo()->name.c_str(), object->GetGUIDLow(), o);

            return true;
        }

        //move selected object
        static bool HandleGameObjectMoveCommand(ChatHandler* handler, char const* args)
        {
            uint32 guidLow = GetGuidLowFromArgsOrLastTargetedGo(handler, args);

            if (!guidLow)
                return false;

            GameObject* object = NULL;

            // by DB guid
            if (GameObjectData const* gameObjectData = sObjectMgr->GetGOData(guidLow))
                object = handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(guidLow, gameObjectData->id);

            if (!object)
            {
                handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, guidLow);
                handler->SetSentErrorMessage(true);
                return false;
            }

            char* toX = strtok(NULL, " ");
            char* toY = strtok(NULL, " ");
            char* toZ = strtok(NULL, " ");

            if (!toX)
            {
                Player* player = handler->GetSession()->GetPlayer();
                object->Relocate(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), object->GetOrientation());
                object->DestroyForNearbyPlayers();
                object->UpdateObjectVisibility();
            }
            else
            {
                if (!toY || !toZ)
                    return false;

                float x = (float)atof(toX);
                float y = (float)atof(toY);
                float z = (float)atof(toZ);

                if (!MapManager::IsValidMapCoord(object->GetMapId(), x, y, z))
                {
                    handler->PSendSysMessage(LANG_INVALID_TARGET_COORD, x, y, object->GetMapId());
                    handler->SetSentErrorMessage(true);
                    return false;
                }

                object->Relocate(x, y, z, object->GetOrientation());
                object->DestroyForNearbyPlayers();
                object->UpdateObjectVisibility();
            }

            object->SaveToDB();
            object->Refresh();

            handler->PSendSysMessage(LANG_COMMAND_MOVEOBJMESSAGE, object->GetGUIDLow(), object->GetGOInfo()->name.c_str(), object->GetGUIDLow());

            return true;
        }

        //set phasemask for selected object
        static bool HandleGameObjectSetPhaseCommand(ChatHandler* handler, char const* args)
        {
            uint32 guidLow = GetGuidLowFromArgsOrLastTargetedGo(handler, args);

            if (!guidLow)
                return false;

            GameObject* object = NULL;

            // by DB guid
            if (GameObjectData const* gameObjectData = sObjectMgr->GetGOData(guidLow))
                object = handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(guidLow, gameObjectData->id);

            if (!object)
            {
                handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, guidLow);
                handler->SetSentErrorMessage(true);
                return false;
            }

            char* phase = strtok (NULL, " ");
            uint32 phaseMask = phase ? atoi(phase) : 0;
            if (phaseMask == 0)
            {
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            object->SetPhaseMask(phaseMask, true);
            object->SaveToDB();
            return true;
        }

        static bool HandleGameObjectNearCommand(ChatHandler* handler, char const* args)
        {
            float distance = (!*args) ? 10.0f : (float)(atof(args));
            uint32 count = 0;

            Player* player = handler->GetSession()->GetPlayer();

            PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_GAMEOBJECT_NEAREST);
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

                    GameObjectTemplate const* gameObjectInfo = sObjectMgr->GetGameObjectTemplate(entry);

                    if (!gameObjectInfo)
                        continue;

                    handler->PSendSysMessage(LANG_GO_LIST_CHAT, guid, entry, guid, gameObjectInfo->name.c_str(), x, y, z, mapId);

                    ++count;
                }
                while (result->NextRow());
            }

            handler->PSendSysMessage(LANG_COMMAND_NEAROBJMESSAGE, distance, count);
            return true;
        }

        //show info of gameobject
        static bool HandleGameObjectInfoCommand(ChatHandler* handler, char const* args)
        {
            uint32 entry = 0;
            uint32 type = 0;
            uint32 displayId = 0;
            std::string name;
            uint32 lootId = 0;

            if (!*args)
            {
                if (WorldObject* object = handler->getSelectedObject())
                    entry = object->GetEntry();
                else
                    entry = atoi((char*)args);
            }

            GameObjectTemplate const* gameObjectInfo = sObjectMgr->GetGameObjectTemplate(entry);

            if (!gameObjectInfo)
                return false;

            type = gameObjectInfo->type;
            displayId = gameObjectInfo->displayId;
            name = gameObjectInfo->name;
            if (type == GAMEOBJECT_TYPE_CHEST)
                lootId = gameObjectInfo->chest.chestLoot;
            else if (type == GAMEOBJECT_TYPE_FISHINGHOLE)
                lootId = gameObjectInfo->fishinghole.chestLoot;

            handler->PSendSysMessage(LANG_GOINFO_ENTRY, entry);
            handler->PSendSysMessage(LANG_GOINFO_TYPE, type);
            handler->PSendSysMessage(LANG_GOINFO_LOOTID, lootId);
            handler->PSendSysMessage(LANG_GOINFO_DISPLAYID, displayId);
            handler->PSendSysMessage(LANG_GOINFO_NAME, name.c_str());

            return true;
        }

        static bool HandleGameObjectSetStateCommand(ChatHandler* handler, char const* args)
        {
            uint32 guidLow = GetGuidLowFromArgsOrLastTargetedGo(handler, args);

            if (!guidLow)
                return false;

            GameObject* object = NULL;

            if (GameObjectData const* gameObjectData = sObjectMgr->GetGOData(guidLow))
                object = handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(guidLow, gameObjectData->id);

            if (!object)
            {
                handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, guidLow);
                handler->SetSentErrorMessage(true);
                return false;
            }

            char* type = strtok(NULL, " ");
            if (!type)
                return false;

            int32 objectType = atoi(type);
            if (objectType < 0)
            {
                if (objectType == -1)
                    object->SendObjectDeSpawnAnim(object->GetGUID());
                else if (objectType == -2)
                    return false;
                return true;
            }

            char* state = strtok(NULL, " ");
            if (!state)
                return false;

            int32 objectState = atoi(state);

            if (objectType < 4)
                object->SetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, objectType, objectState);
            else if (objectType == 4)
            {
                WorldPacket l_Data(SMSG_GAMEOBJECT_CUSTOM_ANIM, 8+4);
                l_Data.appendPackGUID(object->GetGUID());
                l_Data << (uint32)(objectState);
                l_Data.WriteBit(objectState != 0);
                object->SendMessageToSet(&l_Data, true);
            }
            handler->PSendSysMessage("Set gobject type %d state %d", objectType, objectState);
            return true;
        }

        static bool HandleGameObjectSetFlagsCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            uint32 l_GuidLow = GetGuidLowFromArgsOrLastTargetedGo(p_Handler, p_Args);
            if (!l_GuidLow)
                return false;

            GameObject* l_GameObject = NULL;

            if (GameObjectData const* l_GameObjectData = sObjectMgr->GetGOData(l_GuidLow))
                l_GameObject = p_Handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(l_GuidLow, l_GameObjectData->id);

            if (!l_GameObject)
            {
                p_Handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, l_GuidLow);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            char* l_Value = strtok(NULL, " ");
            if (!l_Value)
                return false;

            uint32 l_Flags = atoi(l_Value);

            l_GameObject->SetUInt32Value(GAMEOBJECT_FIELD_FLAGS, l_Flags);
            p_Handler->PSendSysMessage("Set gobject flags %d", l_Flags);
            return true;
        }

        static bool HandleGameObjectSetFieldCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            uint32 l_GuidLow = GetGuidLowFromArgsOrLastTargetedGo(p_Handler, p_Args);
            if (!l_GuidLow)
                return false;

            GameObject* l_GameObject = NULL;

            if (GameObjectData const* l_GameObjectData = sObjectMgr->GetGOData(l_GuidLow))
                l_GameObject = p_Handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(l_GuidLow, l_GameObjectData->id);

            if (!l_GameObject)
            {
                p_Handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, l_GuidLow);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            char* l_Field = strtok(NULL, " ");
            if (!l_Field)
                return false;

            uint32 l_IntField = atoi(l_Field);

            char* l_Value = strtok(NULL, " ");
            if (!l_Value)
                return false;

            uint32 l_Val = atoi(l_Value);

            l_GameObject->SetUInt32Value(l_IntField, l_Val);
            p_Handler->PSendSysMessage("Set gobject field %d value %d", l_IntField, l_Val);
            return true;
        }

        static bool HandleGameObjectSetAnimKitCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            uint32 l_GuidLow = GetGuidLowFromArgsOrLastTargetedGo(p_Handler, p_Args);
            if (!l_GuidLow)
                return false;

            GameObject* l_GameObject = nullptr;

            if (GameObjectData const* l_GameObjectData = sObjectMgr->GetGOData(l_GuidLow))
                l_GameObject = p_Handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(l_GuidLow, l_GameObjectData->id);

            if (!l_GameObject)
            {
                p_Handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, l_GuidLow);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            char* l_Value = strtok(nullptr, " ");
            if (!l_Value)
                return false;

            uint32 l_Val = atoi(l_Value);

            l_GameObject->SetAIAnimKitId(l_Val);
            p_Handler->PSendSysMessage("Set AnimkitID %u for GameObject %u.", l_Val, l_GuidLow);
            return true;
        }

        static bool HandleGameObjectActivateAnim(ChatHandler* p_Handler, char const* p_Args)
        {
            uint32 l_GuidLow = GetGuidLowFromArgsOrLastTargetedGo(p_Handler, p_Args);
            if (!l_GuidLow)
                return false;

            GameObject* l_GameObject = nullptr;

            if (GameObjectData const* l_GameObjectData = sObjectMgr->GetGOData(l_GuidLow))
                l_GameObject = p_Handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(l_GuidLow, l_GameObjectData->id);

            if (!l_GameObject)
            {
                p_Handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, l_GuidLow);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            char* l_Value = strtok(nullptr, " ");
            if (!l_Value)
                return false;

            uint32 l_Val = atoi(l_Value);

            char* l_MaintainChar = strtok(nullptr, " ");
            if (!l_MaintainChar)
                return false;

            bool l_Maintain = atoi(l_MaintainChar);

            l_GameObject->SendGameObjectActivateAnimKit(l_Val, l_Maintain);
            p_Handler->PSendSysMessage("Activate anim kit %u, maintained %u for GameObject %u.", l_Val, l_Maintain, l_GuidLow);
            return true;
        }

        static bool HandleGameObjectGetStateCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            uint32 l_GuidLow = GetGuidLowFromArgsOrLastTargetedGo(p_Handler, p_Args);
            if (!l_GuidLow)
                return false;

            GameObject* l_GameObject = NULL;

            if (GameObjectData const* l_GameObjectData = sObjectMgr->GetGOData(l_GuidLow))
                l_GameObject = p_Handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(l_GuidLow, l_GameObjectData->id);

            if (!l_GameObject)
            {
                p_Handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, l_GuidLow);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            p_Handler->PSendSysMessage("Gobject type %d state %d", 0, l_GameObject->GetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 0));
            p_Handler->PSendSysMessage("Gobject type %d state %d", 1, l_GameObject->GetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 1));
            p_Handler->PSendSysMessage("Gobject type %d state %d", 2, l_GameObject->GetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 2));
            p_Handler->PSendSysMessage("Gobject type %d state %d", 3, l_GameObject->GetByteValue(GAMEOBJECT_FIELD_PERCENT_HEALTH, 3));
            return true;
        }

        static bool HandleGameObjectGetFlagsCommand(ChatHandler* p_Handler, char const* p_Args)
        {
            uint32 l_GuidLow = GetGuidLowFromArgsOrLastTargetedGo(p_Handler, p_Args);
            if (!l_GuidLow)
                return false;

            GameObject* l_GameObject = NULL;

            if (GameObjectData const* l_GameObjectData = sObjectMgr->GetGOData(l_GuidLow))
                l_GameObject = p_Handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(l_GuidLow, l_GameObjectData->id);

            if (!l_GameObject)
            {
                p_Handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, l_GuidLow);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            p_Handler->PSendSysMessage("Gobject flags %d", l_GameObject->GetUInt32Value(GAMEOBJECT_FIELD_FLAGS));
            return true;
        }

        static bool HandleBindPortalCommand(ChatHandler* handler, char const* args)
        {
            uint32 guidLow = GetGuidLowFromArgsOrLastTargetedGo(handler, args);

            if (!guidLow)
                return false;

            if (!handler->GetSession())
                return true;

            Player* pPlayer = handler->GetSession()->GetPlayer();

            if (!pPlayer)
                return true;

            uint32 mapId = pPlayer->GetMapId();
            float  posX  = pPlayer->GetPositionX();
            float  posY  = pPlayer->GetPositionY();
            float  posZ  = pPlayer->GetPositionZ();
            float  posO  = pPlayer->GetOrientation();

            WorldDatabase.PExecute("DELETE FROM gameobject_scripts WHERE id = %u AND command = 6;", guidLow);
            WorldDatabase.PExecute("INSERT INTO gameobject_scripts VALUES (%u, 0, 6, %u, 0, 0, %f, %f, %f, %f);", guidLow, mapId, posX, posY, posZ, posO);

            handler->SendGlobalGMSysMessage("Portal Binded");

            if (sScriptMgr->IsScriptScheduled())
            {
                handler->SendSysMessage("DB scripts used currently, please attempt reload later.");
                handler->SetSentErrorMessage(true);
                return true;
            }

            sObjectMgr->LoadGameObjectScripts();
            return true;
        }

        static bool HandleDamageObjectCommand(ChatHandler* handler, char const* args)
        {
            uint32 guidLow = GetGuidLowFromArgsOrLastTargetedGo(handler, args);

            if (!guidLow)
                return false;

            if (!handler->GetSession())
                return false;

            Player* pPlayer = handler->GetSession()->GetPlayer();

            if (!pPlayer)
                return false;

            GameObject* object = NULL;

            // by DB guid
            if (GameObjectData const* gameObjectData = sObjectMgr->GetGOData(guidLow))
                object = handler->GetObjectGlobalyWithGuidOrNearWithDbGuid(guidLow, gameObjectData->id);

            if (!object)
            {
                handler->PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, guidLow);
                handler->SetSentErrorMessage(true);
                return false;

            }
            char* damageStr = strtok (NULL, " ");
            int32 damage = damageStr ? atoi(damageStr) : 0;

            object->ModifyHealth(-damage, pPlayer);

            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_gobject_commandscript()
{
    new gobject_commandscript();
}
#endif
