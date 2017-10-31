////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Instance_Onyxias_Lair
SD%Complete: 100
SDComment:
SDCategory: Onyxia's Lair
EndScriptData */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "onyxias_lair.h"

class instance_onyxias_lair : public InstanceMapScript
{
public:
    instance_onyxias_lair() : InstanceMapScript("instance_onyxias_lair", 249) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_onyxias_lair_InstanceMapScript(map);
    }

    struct instance_onyxias_lair_InstanceMapScript : public InstanceScript
    {
        instance_onyxias_lair_InstanceMapScript(Map* map) : InstanceScript(map) {}

        //Eruption is a BFS graph problem
        //One map to remember all floor, one map to keep floor that still need to erupt and one queue to know what needs to be removed
        std::map<uint64, uint32> FloorEruptionGUID[2];
        std::queue<uint64> FloorEruptionGUIDQueue;

        uint64 OnyxiasGUID;
        uint32 OnyxiaLiftoffTimer;
        uint32 ManyWhelpsCounter;
        uint32 EruptTimer;

        uint8  Encounter[MAX_ENCOUNTER];

        bool   AchievManyWhelpsHandleIt;
        bool   AchievSheDeepBreathMore;

        void Initialize()
        {
            memset(&Encounter, 0, sizeof(Encounter));

            OnyxiasGUID = 0;
            OnyxiaLiftoffTimer = 0;
            ManyWhelpsCounter = 0;
            AchievManyWhelpsHandleIt = false;
            AchievSheDeepBreathMore = true;

            EruptTimer = 0;
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_ONYXIA:
                    OnyxiasGUID = creature->GetGUID();
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            if ((go->GetGOInfo()->displayId == 4392 || go->GetGOInfo()->displayId == 4472) && go->GetGOInfo()->trap.spell == 17731)
            {
                FloorEruptionGUID[0].insert(std::make_pair(go->GetGUID(), 0));
                return;
            }

            switch (go->GetEntry())
            {
                case GO_WHELP_SPAWNER:
                    Position goPos;
                    go->GetPosition(&goPos);
                    if (Creature* temp = go->SummonCreature(NPC_WHELP, goPos, TEMPSUMMON_CORPSE_DESPAWN))
                    {
                        temp->SetInCombatWithZone();
                        ++ManyWhelpsCounter;
                    }
                    break;
            }
        }

        void OnGameObjectRemove(GameObject* p_Go)
        {
            if ((p_Go->GetGOInfo()->displayId == 4392 || p_Go->GetGOInfo()->displayId == 4472) && p_Go->GetGOInfo()->trap.spell == 17731)
            {
                FloorEruptionGUID[0].erase(p_Go->GetGUID());
                return;
            }

            InstanceScript::OnGameObjectRemove(p_Go);
        }

        void FloorEruption(uint64 floorEruptedGUID)
        {
            if (GameObject* pFloorEruption = instance->GetGameObject(floorEruptedGUID))
            {
                //THIS GOB IS A TRAP - What shall i do? =(
                //Cast it spell? Copyed Heigan method
                pFloorEruption->SendCustomAnim(pFloorEruption->GetGoAnimProgress());
                pFloorEruption->CastSpell(NULL, Difficulty(instance->GetSpawnMode()) == Difficulty::Difficulty10N ? 17731 : 69294); //pFloorEruption->GetGOInfo()->trap.spellId

                //Get all immediatly nearby floors
                std::list<GameObject*> nearFloorList;
                JadeCore::GameObjectInRangeCheck check(pFloorEruption->GetPositionX(), pFloorEruption->GetPositionY(), pFloorEruption->GetPositionZ(), 15);
                JadeCore::GameObjectListSearcher<JadeCore::GameObjectInRangeCheck> searcher(pFloorEruption, nearFloorList, check);
                pFloorEruption->VisitNearbyGridObject(999, searcher);
                //remove all that are not present on FloorEruptionGUID[1] and update treeLen on each GUID
                for (std::list<GameObject*>::const_iterator itr = nearFloorList.begin(); itr != nearFloorList.end(); ++itr)
                {
                    if (((*itr)->GetGOInfo()->displayId == 4392 || (*itr)->GetGOInfo()->displayId == 4472) && (*itr)->GetGOInfo()->trap.spell == 17731)
                    {
                        uint64 nearFloorGUID = (*itr)->GetGUID();
                        if (FloorEruptionGUID[1].find(nearFloorGUID) != FloorEruptionGUID[1].end() && (*FloorEruptionGUID[1].find(nearFloorGUID)).second == 0)
                        {
                            (*FloorEruptionGUID[1].find(nearFloorGUID)).second = (*FloorEruptionGUID[1].find(floorEruptedGUID)).second+1;
                            FloorEruptionGUIDQueue.push(nearFloorGUID);
                        }
                    }
                }
            }
            FloorEruptionGUID[1].erase(floorEruptedGUID);
        }

        void SetData(uint32 Type, uint32 Data)
        {
            switch (Type)
            {
                case DATA_ONYXIA:
                    Encounter[0] = Data;
                    if (Data == IN_PROGRESS)
                        SetData(DATA_SHE_DEEP_BREATH_MORE, IN_PROGRESS);
                    break;
                case DATA_ONYXIA_PHASE:
                    if (Data == PHASE_BREATH) //Used to mark the liftoff phase
                    {
                        AchievManyWhelpsHandleIt = false;
                        ManyWhelpsCounter = 0;
                        OnyxiaLiftoffTimer = 10*IN_MILLISECONDS;
                    }
                    break;
                case DATA_SHE_DEEP_BREATH_MORE:
                    if (Data == IN_PROGRESS)
                    {
                        AchievSheDeepBreathMore = true;
                    }
                    else if (Data == FAIL)
                    {
                        AchievSheDeepBreathMore = false;
                    }
                    break;
            }

            if (Type < MAX_ENCOUNTER && Data == DONE)
                SaveToDB();
        }

        void SetData64(uint32 Type, uint64 Data)
        {
            switch (Type)
            {
                case DATA_FLOOR_ERUPTION_GUID:
                    FloorEruptionGUID[1] = FloorEruptionGUID[0];
                    FloorEruptionGUIDQueue.push(Data);
                    EruptTimer = 2500;
                    break;
            }
        }

        uint32 GetData(uint32 Type)
        {
            switch (Type)
            {
                case DATA_ONYXIA:
                    return Encounter[0];
            }

            return 0;
        }

        uint64 GetData64(uint32 Data)
        {
            switch (Data)
            {
                case DATA_ONYXIA_GUID:
                    return OnyxiasGUID;
            }

            return 0;
        }

        void Update(uint32 Diff)
        {
            UpdateOperations(Diff);

            if (GetData(DATA_ONYXIA) == IN_PROGRESS)
            {
                if (OnyxiaLiftoffTimer && OnyxiaLiftoffTimer <= Diff)
                {
                    OnyxiaLiftoffTimer = 0;
                    if (ManyWhelpsCounter >= 50)
                        AchievManyWhelpsHandleIt = true;
                } else OnyxiaLiftoffTimer -= Diff;
            }

            if (!FloorEruptionGUIDQueue.empty())
            {
                if (EruptTimer <= Diff)
                {
                    uint64 frontGuid = FloorEruptionGUIDQueue.front();
                    std::map<uint64, uint32>::iterator itr = FloorEruptionGUID[1].find(frontGuid);
                    if (itr != FloorEruptionGUID[1].end())
                    {
                        uint32 treeHeight = itr->second;

                        do
                        {
                            FloorEruption(frontGuid);
                            FloorEruptionGUIDQueue.pop();
                            if (FloorEruptionGUIDQueue.empty())
                                break;

                            frontGuid = FloorEruptionGUIDQueue.front();
                            itr = FloorEruptionGUID[1].find(frontGuid);
                        } while (itr != FloorEruptionGUID[1].end() && itr->second == treeHeight);
                    }

                    EruptTimer = 1000;
                }
                else
                    EruptTimer -= Diff;
            }
        }

        bool CheckAchievementCriteriaMeet(uint32 criteria_id, Player const* /*source*/, Unit const* /*target*/ = NULL, uint64 /*miscvalue1*/ = 0)
        {
            switch (criteria_id)
            {
                case ACHIEV_CRITERIA_MANY_WHELPS_10_PLAYER:  // Criteria for achievement 4403: Many Whelps! Handle It! (10 player) Hatch 50 eggs in 10s
                case ACHIEV_CRITERIA_MANY_WHELPS_25_PLAYER:  // Criteria for achievement 4406: Many Whelps! Handle It! (25 player) Hatch 50 eggs in 10s
                    return AchievManyWhelpsHandleIt;
                case ACHIEV_CRITERIA_DEEP_BREATH_10_PLAYER:  // Criteria for achievement 4404: She Deep Breaths More (10 player) Everybody evade Deep Breath
                case ACHIEV_CRITERIA_DEEP_BREATH_25_PLAYER:  // Criteria for achievement 4407: She Deep Breaths More (25 player) Everybody evade Deep Breath
                    return AchievSheDeepBreathMore;
            }
            return false;
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_instance_onyxias_lair()
{
    new instance_onyxias_lair();
}
#endif
