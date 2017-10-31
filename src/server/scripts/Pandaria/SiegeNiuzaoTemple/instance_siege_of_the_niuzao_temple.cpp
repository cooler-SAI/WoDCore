/*
    Dungeon : Siege of niuzao
    Instance General Script
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "siege_of_the_niuzao_temple.h"

class instance_siege_of_the_niuzao_temple : public InstanceMapScript
{
public:
    instance_siege_of_the_niuzao_temple() : InstanceMapScript("instance_siege_of_the_niuzao_temple", 1011) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_siege_of_the_niuzao_temple_InstanceMapScript(map);
    }

    struct instance_siege_of_the_niuzao_temple_InstanceMapScript : public InstanceScript
    {
        uint64 jinbakGuid;
        uint64 vojakGuid;
        uint64 pavalakGuid;
        uint64 neronokGuid;

        instance_siege_of_the_niuzao_temple_InstanceMapScript(Map* map) : InstanceScript(map)
        {}

        void Initialize()
        {
            jinbakGuid  = 0;
            vojakGuid   = 0;
            pavalakGuid = 0;
            neronokGuid = 0;
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_JINBAK:
                    jinbakGuid = creature->GetGUID();
                    break;
                case NPC_VOJAK:
                    vojakGuid = creature->GetGUID();
                    break;
                case NPC_PAVALAK:
                    pavalakGuid = creature->GetGUID();
                    break;
                case NPC_NERONOK:
                    neronokGuid = creature->GetGUID();
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* /*go*/)
        {
        }

        void SetData(uint32 /*type*/, uint32 /*p_Data*/)
        {}

        uint32 GetData(uint32 /*type*/)
        {
            return 0;
        }

        uint64 GetData64(uint32 type)
        {
            switch (type)
            {
                case NPC_JINBAK:    return jinbakGuid;
                case NPC_VOJAK:     return vojakGuid;
                case NPC_PAVALAK:   return pavalakGuid;
                case NPC_NERONOK:   return neronokGuid;
            }

            return 0;
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_instance_siege_of_the_niuzao_temple()
{
    new instance_siege_of_the_niuzao_temple();
}
#endif
