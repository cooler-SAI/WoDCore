////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "throne_of_the_tides.h"

#define MAX_ENCOUNTER 4

static const DoorData doordata[] =
{
    {GO_LADY_NAZJAR_DOOR, DATA_LADY_NAZJAR, DOOR_TYPE_ROOM, BOUNDARY_NONE},
    {GO_LADY_NAZJAR_DOOR, DATA_COMMANDER_ULTHOK, DOOR_TYPE_ROOM, BOUNDARY_NONE},
    {GO_COMMANDER_ULTHOK_DOOR, DATA_COMMANDER_ULTHOK, DOOR_TYPE_ROOM, BOUNDARY_NONE},
    {GO_ERUNAK_STONESPEAKER_DOOR, DATA_MINDBENDER_GHURSHA, DOOR_TYPE_ROOM, BOUNDARY_NONE},
    {GO_OZUMAT_DOOR, DATA_OZUMAT, DOOR_TYPE_ROOM, BOUNDARY_NONE},
    {0, 0, DOOR_TYPE_ROOM, BOUNDARY_NONE}
};

class instance_throne_of_the_tides : public InstanceMapScript
{
public:
    instance_throne_of_the_tides() : InstanceMapScript("instance_throne_of_the_tides", 643) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_throne_of_the_tides_InstanceMapScript(map);
    }

    struct instance_throne_of_the_tides_InstanceMapScript : public InstanceScript
    {
        instance_throne_of_the_tides_InstanceMapScript(Map* map) : InstanceScript(map)
        {
            SetBossNumber(MAX_ENCOUNTER);
            LoadDoorData(doordata);
            uiLadyNazjarGUID = 0;
            uiCommanderUlthokGUID = 0;
            uiErunakStonespeakerGUID = 0;
            uiMindbenderGhurshaGUID = 0;
            uiOzumatGUID = 0;
            uiNeptulonGUID = 0;
            uiLadyNazjarEventGUID = 0;

            uiCoralesGUID = 0,
            uiLadyNazjarDoorGUID = 0;
            uiCommanderUlthokDoorGUID = 0;
            uiMindebenderGhurshaDoorGUID = 0;
            uiOzumatDoorGUID = 0;
            uiControlSystemGUID = 0;
            uiTentacleRightGUID = 0;
            uiTentacleLeftGUID = 0;
            uiInvisibleDoor1GUID = 0;
            uiInvisibleDoor2GUID = 0;
            uiNeptulonCache = 0;

            memset(m_uiEvents, 0, sizeof(m_uiEvents));
        }

        void OnPlayerEnter(Player* pPlayer)
        {
            if (!uiTeamInInstance)
                uiTeamInInstance = pPlayer->GetTeam();
        }

        void OnCreatureCreate(Creature* pCreature)
        {
            if (!uiTeamInInstance)
            {
                Map::PlayerList const &players = instance->GetPlayers();
                if (!players.isEmpty())
                    if (Player* player = players.begin()->getSource())
                        uiTeamInInstance = player->GetTeam();
            }

            switch(pCreature->GetEntry())
            {
            case NPC_LADY_NAZJAR_EVENT:
                uiLadyNazjarEventGUID = pCreature->GetGUID();
                break;
            case NPC_LADY_NAZJAR:
                uiLadyNazjarGUID = pCreature->GetGUID();
                break;
            case NPC_COMMANDER_ULTHOK:
                uiCommanderUlthokGUID = pCreature->GetGUID();
                sLog->outError(LOG_FILTER_SERVER_LOADING, "ulthok1");
                if (GetData(DATA_COMMANDER_ULTHOK_EVENT) == DONE)
                {
                    sLog->outError(LOG_FILTER_SERVER_LOADING, "ulthok2");
                    pCreature->SetPhaseMask(PHASEMASK_NORMAL, true);
                }
                else
                {
                    sLog->outError(LOG_FILTER_SERVER_LOADING, "ulthok3");
                    pCreature->SetPhaseMask(2, true);
                }
                break;
            case NPC_ERUNAK_STONESPEAKER:
                uiErunakStonespeakerGUID = pCreature->GetGUID();
                break;
            case NPC_MINDBENDER_GHURSHA:
                uiMindbenderGhurshaGUID = pCreature->GetGUID();
                break;
            case NPC_OZUMAT:
                uiOzumatGUID = pCreature->GetGUID();
                break;
            case NPC_NEPTULON:
                uiNeptulonGUID = pCreature->GetGUID();
                break;
            case NPC_CAPTAIN_TAYLOR:
                if (uiTeamInInstance == HORDE)
                    pCreature->UpdateEntry(NPC_LEGIONNAIRE_NAZGRIM);
                break;
            case NPC_LEGIONNAIRE_NAZGRIM:
                if (uiTeamInInstance == ALLIANCE)
                    pCreature->UpdateEntry(NPC_CAPTAIN_TAYLOR);
                break;
            }
        }

        void OnGameObjectCreate(GameObject* pGo)
        {
            switch(pGo->GetEntry())
            {
            case GO_CORALES:
                uiCoralesGUID = pGo->GetGUID();
                if (GetBossState(DATA_LADY_NAZJAR) == DONE)
                {
                    pGo->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
                    pGo->SetPhaseMask(2, true);
                }
                break;
            case GO_LADY_NAZJAR_DOOR:
                uiLadyNazjarDoorGUID = pGo->GetGUID();
                AddDoor(pGo, true);
                break;
            case GO_COMMANDER_ULTHOK_DOOR:
                uiCommanderUlthokDoorGUID = pGo->GetGUID();
                AddDoor(pGo, true);
                break;
            case GO_ERUNAK_STONESPEAKER_DOOR:
                uiMindebenderGhurshaDoorGUID = pGo->GetGUID();
                AddDoor(pGo, true);
                break;
            case GO_OZUMAT_DOOR:
                uiOzumatDoorGUID = pGo->GetGUID();
                AddDoor(pGo, true);
                break;
            case GO_CONTROL_SYSTEM:
                uiControlSystemGUID = pGo->GetGUID();
                break;
            case GO_TENTACLE_RIGHT:
                uiTentacleRightGUID = pGo->GetGUID();
                if (GetBossState(DATA_COMMANDER_ULTHOK) == DONE)
                    pGo->SetPhaseMask(2, true);
                break;
            case GO_TENTACLE_LEFT:
                uiTentacleLeftGUID = pGo->GetGUID();
                if (GetBossState(DATA_COMMANDER_ULTHOK) == DONE)
                    pGo->SetPhaseMask(2, true);
                break;
            case GO_INVISIBLE_DOOR_1:
                uiInvisibleDoor1GUID = pGo->GetGUID();
                if (GetBossState(DATA_COMMANDER_ULTHOK) == DONE)
                    pGo->SetPhaseMask(2, true);
                break;
            case GO_INVISIBLE_DOOR_2:
                uiInvisibleDoor2GUID = pGo->GetGUID();
                if (GetBossState(DATA_COMMANDER_ULTHOK) == DONE)
                    pGo->SetPhaseMask(2, true);
                break;
            case GO_NEPTULON_CACHE:
            case GO_NEPTULON_CACHE_H:
                uiNeptulonCache = pGo->GetGUID();
                break;
            }
        }

        void SetData(uint32 type, uint32 data)
        {
            switch (type)
            {
            case DATA_LADY_NAZJAR_EVENT:
                m_uiEvents[0] = data;
                break;
            case DATA_COMMANDER_ULTHOK_EVENT:
                m_uiEvents[1] = data;
                break;
            case DATA_NEPTULON_EVENT:
                m_uiEvents[2] = data;
            }
            if (data == DONE)
                SaveToDB();
        }

        uint32 GetData(uint32 type)
        {
            switch (type)
            {
            case DATA_LADY_NAZJAR_EVENT:
                return m_uiEvents[0];
            case DATA_COMMANDER_ULTHOK_EVENT:
                return m_uiEvents[1];
            case DATA_NEPTULON_EVENT:
                return m_uiEvents[2];
            }
            return 0;
        }

        uint64 GetData64(uint32 type)
        {
            switch(type)
            {
            case DATA_LADY_NAZJAR:
                return uiLadyNazjarGUID;
            case DATA_COMMANDER_ULTHOK:
                return uiCommanderUlthokGUID;
            case DATA_ERUNAK_STONESPEAKER:
                return uiErunakStonespeakerGUID;
            case DATA_MINDBENDER_GHURSHA:
                return uiMindbenderGhurshaGUID;
            case DATA_OZUMAT:
                return uiOzumatGUID;
            case DATA_NEPTULON:
                return uiNeptulonGUID;
            case DATA_LADY_NAZJAR_EVENT:
                return uiLadyNazjarEventGUID;
            case DATA_CORALES:
                return uiCoralesGUID;
            }
            return 0;
        }

        bool SetBossState(uint32 type, EncounterState state)
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;

            switch (type)
            {
            case DATA_LADY_NAZJAR:
                break;
            case DATA_COMMANDER_ULTHOK:
                if (state == DONE)
                {
                    if (GameObject* pTentacleRight = instance->GetGameObject(uiTentacleRightGUID))
                        pTentacleRight->SetPhaseMask(2, true);
                    if (GameObject* pTentacleLeft = instance->GetGameObject(uiTentacleLeftGUID))
                        pTentacleLeft->SetPhaseMask(2, true);
                    if (GameObject* pInvisibleDoor1 = instance->GetGameObject(uiInvisibleDoor1GUID))
                        pInvisibleDoor1->SetPhaseMask(2, true);
                    if (GameObject* pInvisibleDoor2 = instance->GetGameObject(uiInvisibleDoor2GUID))
                        pInvisibleDoor2->SetPhaseMask(2, true);
                }
                break;
            case DATA_MINDBENDER_GHURSHA:
                break;
            case DATA_OZUMAT:
                if (state == DONE)
                    DoRespawnGameObject(uiNeptulonCache, DAY);
                break;
             }
            return true;
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::string str_data;

            std::ostringstream saveStream;
            saveStream << "T o t T " << GetBossSaveData() << m_uiEvents[0] << " " << m_uiEvents[1] << " " << m_uiEvents[2] << " ";

            str_data = saveStream.str();

            OUT_SAVE_INST_DATA_COMPLETE;
            return str_data;
        }

        void Load(const char* in)
        {
            if (!in)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(in);

            char dataHead1, dataHead2, dataHead3, dataHead4;

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2 >> dataHead3 >> dataHead4;

            if (dataHead1 == 'T' && dataHead2 == 'o' && dataHead3 == 't' && dataHead4 == 'T')
            {
                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                {
                    uint32 tmpState;
                    loadStream >> tmpState;
                    if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                        tmpState = NOT_STARTED;
                    SetBossState(i, EncounterState(tmpState));
                }
                for (uint8 i = 0; i < 3; ++i)
                {
                    uint32 tmpEvent;
                    loadStream >> tmpEvent;
                    if (tmpEvent == IN_PROGRESS || tmpEvent > SPECIAL)
                        tmpEvent = NOT_STARTED;
                    m_uiEvents[i] = tmpEvent;
                }

            } else OUT_LOAD_INST_DATA_FAIL;

            OUT_LOAD_INST_DATA_COMPLETE;
        }

        private:
            uint64 uiLadyNazjarGUID;
            uint64 uiCommanderUlthokGUID;
            uint64 uiErunakStonespeakerGUID;
            uint64 uiMindbenderGhurshaGUID;
            uint64 uiOzumatGUID;
            uint64 uiNeptulonGUID;
            uint64 uiLadyNazjarEventGUID;
            uint64 uiCoralesGUID;
            uint64 uiLadyNazjarDoorGUID;
            uint64 uiCommanderUlthokDoorGUID;
            uint64 uiMindebenderGhurshaDoorGUID;
            uint64 uiOzumatDoorGUID;
            uint64 uiControlSystemGUID;
            uint64 uiTentacleRightGUID;
            uint64 uiTentacleLeftGUID;
            uint64 uiInvisibleDoor1GUID;
            uint64 uiInvisibleDoor2GUID;
            uint64 uiNeptulonCache;
            uint32 uiTeamInInstance;
            uint32 m_uiEvents[3];
    };
};

#ifndef __clang_analyzer__
void AddSC_instance_throne_of_the_tides()
{
    new instance_throne_of_the_tides();
}
#endif
