////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "boss_iron_maidens.hpp"

DoorData const g_DoorData[] =
{
    { eFoundryGameObjects::GruulSpikeDoor,              eFoundryDatas::DataGruul,               DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::BKFoundrySpikeTrapGate,      eFoundryDatas::DataOregorger,           DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::FurnacePortcullis,           eFoundryDatas::DataOregorger,           DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::BlastFurnaceEncounterDoor,   eFoundryDatas::DataBlastFurnace,        DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::HansgarAndFranzokEntrance,   eFoundryDatas::DataHansgarAndFranzok,   DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::HansgarAndFranzokExit,       eFoundryDatas::DataHansgarAndFranzok,   DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::FirewallDoor,                eFoundryDatas::DataFlamebenderKagraz,   DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::BurningFrontDoor,            eFoundryDatas::DataFlamebenderKagraz,   DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::KromogsDoorSouth,            eFoundryDatas::DataKromog,              DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::KromogsDoorEast,             eFoundryDatas::DataKromog,              DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::BlackForgePortcullis,        eFoundryDatas::DataKromog,              DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::BlackForgeGate,              eFoundryDatas::DataKromog,              DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::KromogDoor,                  eFoundryDatas::DataKromog,              DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::TheBeastGate,                eFoundryDatas::DataBeastlordDarmac,     DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::TerminusDoor,                eFoundryDatas::DataBeastlordDarmac,     DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::FreightElevatorDoor,         eFoundryDatas::DataOperatorThogar,      DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::IronMaidensRampDoor,         eFoundryDatas::DataIronMaidens,         DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::IronMaidensExitDoor,         eFoundryDatas::DataIronMaidens,         DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::BlackhandsGate,              eFoundryDatas::DataBlackhand,           DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { 0,                                                0,                                      DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE } ///< End
};

class instance_blackrock_foundry : public InstanceMapScript
{
    public:
        instance_blackrock_foundry() : InstanceMapScript("instance_blackrock_foundry", 1205) { }

        struct instance_blackrock_foundryMapScript : public InstanceScript
        {
            instance_blackrock_foundryMapScript(Map* p_Map) : InstanceScript(p_Map)
            {
                m_Initialized               = false;

                m_DungeonID                 = 0;

                m_HardenedSlagEntranceGuid  = 0;
                m_HardenedSlagFurnaceGuid   = 0;
                m_CrucibleLeftGuid          = 0;
                m_CrucibleRightGuid         = 0;

                m_SlagworksEntrance         = 0;
                m_GruulGuid                 = 0;
                m_PristineTrueIronOres      = 0;

                m_OregorgerGuid             = 0;
                m_VolatileOreGuid           = 0;
                m_VolatileOreGrinded        = false;

                m_HeartOfTheMountain        = 0;
                m_ForemanFeldspar           = 0;
                m_CosmeticBlackhand         = 0;
                m_FurnaceGate               = 0;
                m_PrimalElementalistTime    = 0;
                m_YaWeveGotTimeAchiev       = false;

                m_BlackForgeEntrance        = 0;

                m_HansgarGuid               = 0;
                m_FranzokGuid               = 0;
                m_StampStampRevolution      = true;

                m_IronTaskmasterAggro       = false;
                m_SteelHasBeenBrought       = true;
                m_FlamebenderKagrazGuid     = 0;
                m_AknorSteelbringerGuid     = 0;
                m_LavaStalkerGuid           = 0;
                m_MoltenTorrentStalkerGuid  = 0;

                m_WouldYouGiveMeAHand       = false;
                m_GraspingEarthHandsCount   = 0;
                m_GraspingEarthHandsTime    = 0;
                m_KromogGuid                = 0;

                m_IronAssemblyEntrance      = 0;
                m_DarmacBeastMountedFirst   = 0;
                m_BeastlordDarmacGuid       = 0;
                m_CruelfangGuid             = 0;
                m_DreadwingGuid             = 0;
                m_IroncrusherGuid           = 0;
                m_ThunderlordPackPens       = 0;
                m_BeastsEnclosureDoor       = 0;

                m_ThogarIntroStarted        = false;
                m_IronGateDoorGuid          = 0;
                m_OperatorThogarGuid        = 0;

                m_AdmiralGaranGuid          = 0;
                m_EnforcerSorkaGuid         = 0;
                m_MarakTheBloodedGuid       = 0;
                m_ZiplineStalkerGuid        = 0;
                m_IronCannonGuid            = 0;
                m_AmmoLoaderGuid            = 0;
                m_IronMaidensKillTime       = 0;
                m_BeQuickOrBeDeadAchiev     = false;

                m_BlackhandGuid             = 0;
                m_SpikeGateGuid             = 0;
                m_CrucibleEntrance          = 0;
                m_PreBlackhandsGate         = 0;
                m_BRFTriggerGuid            = 0;
                m_CeilingStalkerGuid        = 0;
                m_BlackhandPlatform         = 0;
                m_HangingIronStar           = 0;
                m_AchievementStalker        = 0;
                m_CollectedAshes            = 0;
            }

            bool m_Initialized;

            uint32 m_DungeonID;

            uint64 m_HardenedSlagEntranceGuid;
            uint64 m_HardenedSlagFurnaceGuid;
            uint64 m_CrucibleLeftGuid;
            uint64 m_CrucibleRightGuid;

            /// Slagworks
            uint64 m_SlagworksEntrance;
            uint64 m_GruulGuid;
            uint8 m_PristineTrueIronOres;

            uint64 m_OregorgerGuid;
            uint64 m_VolatileOreGuid;
            bool m_VolatileOreGrinded;

            /// Blast Furnace
            uint64 m_HeartOfTheMountain;
            uint64 m_ForemanFeldspar;
            uint64 m_CosmeticBlackhand;
            uint64 m_FurnaceGate;
            uint32 m_PrimalElementalistTime;
            bool m_YaWeveGotTimeAchiev;

            /// The Black Forge
            uint64 m_BlackForgeEntrance;
            /// Slagmill Press
            uint64 m_HansgarGuid;
            uint64 m_FranzokGuid;
            bool m_StampStampRevolution;

            /// Burning Front
            bool m_IronTaskmasterAggro;
            bool m_SteelHasBeenBrought;
            uint64 m_FlamebenderKagrazGuid;
            uint64 m_AknorSteelbringerGuid;
            uint64 m_LavaStalkerGuid;
            uint64 m_MoltenTorrentStalkerGuid;

            /// The Great Anvil
            bool m_WouldYouGiveMeAHand;
            uint32 m_GraspingEarthHandsCount;
            uint32 m_GraspingEarthHandsTime;
            uint64 m_KromogGuid;

            /// Iron Assembly
            uint64 m_IronAssemblyEntrance;
            /// The Breaking Grounds
            uint32 m_DarmacBeastMountedFirst;
            uint64 m_BeastlordDarmacGuid;
            uint64 m_CruelfangGuid;
            uint64 m_DreadwingGuid;
            uint64 m_IroncrusherGuid;
            uint64 m_ThunderlordPackPens;
            uint64 m_BeastsEnclosureDoor;

            /// After Beastlord Darmac fight
            std::set<uint64> m_GromkarMenAtArms;
            std::set<uint64> m_IronRaiders;
            std::set<uint64> m_IronCracksShot;
            std::set<uint64> m_GromkarFiremenders;

            /// Foundry Terminus
            bool m_ThogarIntroStarted;
            uint64 m_IronGateDoorGuid;
            uint64 m_OperatorThogarGuid;
            std::map<uint32, uint64> m_TrackDoorsGuids;

            /// Dread Grotto
            uint64 m_AdmiralGaranGuid;
            uint64 m_EnforcerSorkaGuid;
            uint64 m_MarakTheBloodedGuid;
            uint64 m_ZiplineStalkerGuid;
            uint64 m_IronCannonGuid;
            uint64 m_AmmoLoaderGuid;
            uint32 m_IronMaidensKillTime;
            bool m_BeQuickOrBeDeadAchiev;

            /// Blackhand's Crucible
            uint64 m_BlackhandGuid;
            uint64 m_SpikeGateGuid;
            uint64 m_CrucibleEntrance;
            uint64 m_PreBlackhandsGate;
            uint64 m_BRFTriggerGuid;
            uint64 m_CeilingStalkerGuid;
            uint64 m_BlackhandPlatform;
            uint64 m_HangingIronStar;
            uint64 m_AchievementStalker;
            uint32 m_CollectedAshes;
            std::map<uint32, uint64> m_BlackhandsCrucibles;

            void Initialize() override
            {
                SetBossNumber(eFoundryDatas::MaxBossData);

                LoadDoorData(g_DoorData);

                instance->SetObjectVisibility(200.0f);
            }

            void OnCreatureCreate(Creature* p_Creature) override
            {
                switch (p_Creature->GetEntry())
                {
                    case eFoundryCreatures::BossGruul:
                    {
                        m_GruulGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::BossOregorger:
                    {
                        m_OregorgerGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::HeartOfTheMountain:
                    {
                        m_HeartOfTheMountain = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::ForemanFeldspar:
                    {
                        m_ForemanFeldspar = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::BlackhandCosmetic:
                    {
                        m_CosmeticBlackhand = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::BossHansgar:
                    {
                        m_HansgarGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::BossFranzok:
                    {
                        m_FranzokGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::BossFlamebenderKagraz:
                    {
                        m_FlamebenderKagrazGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::AknorSteelbringer:
                    {
                        m_AknorSteelbringerGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::LavaStalker:
                    {
                        m_LavaStalkerGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::MoltenTorrentStalker:
                    {
                        m_MoltenTorrentStalkerGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::BossKromog:
                    {
                        m_KromogGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::BossBeastlordDarmac:
                    {
                        m_BeastlordDarmacGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::BossCruelfang:
                    {
                        m_CruelfangGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::BossDreadwing:
                    {
                        m_DreadwingGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::BossIroncrusher:
                    {
                        m_IroncrusherGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::ThunderlordPackPens:
                    {
                        m_ThunderlordPackPens = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::GromkarManAtArms:
                    {
                        Position const l_DarmacOuttro = { 409.889f, 3318.73f, 303.685f, 3.12171f };

                        if (p_Creature->IsNearPosition(&l_DarmacOuttro, 1.0f))
                        {
                            m_GromkarMenAtArms.insert(p_Creature->GetGUID());
                            break;
                        }
                        else
                        {
                            Position l_ThogarIntro = { 502.17f, 3275.37f, 305.984f, 0.538223f };

                            if (p_Creature->IsNearPosition(&l_ThogarIntro, 1.0f))
                                break;

                            l_ThogarIntro = { 501.031f, 3348.45f, 306.277f, 5.37476f };

                            if (p_Creature->IsNearPosition(&l_ThogarIntro, 1.0f))
                                break;
                        }

                        Position l_Trash = { 431.22f, 3314.19f, 303.04f, 4.71f };
                        if (p_Creature->IsNearPosition(&l_Trash, 1.0f))
                            break;

                        p_Creature->DespawnOrUnsummon();
                        break;
                    }
                    case eFoundryCreatures::IronRaider:
                    {
                        Position l_Pos = { 415.89f, 3316.49f, 303.685f, 3.38996f };

                        if (p_Creature->IsNearPosition(&l_Pos, 1.0f))
                        {
                            m_IronRaiders.insert(p_Creature->GetGUID());
                            break;
                        }
                        else
                        {
                            l_Pos = { 425.729f, 3316.56f, 303.658f, 3.03271f };

                            if (p_Creature->IsNearPosition(&l_Pos, 1.0f))
                                m_IronRaiders.insert(p_Creature->GetGUID());

                            break;
                        }

                        if (p_Creature->ToTempSummon() && p_Creature->ToTempSummon()->GetSummoner())
                            break;

                        p_Creature->DespawnOrUnsummon();
                        break;
                    }
                    case eFoundryCreatures::IronCrackShot:
                    {
                        Position l_Pos = { 415.781f, 3316.689941f, 303.685f, 3.3621f };

                        if (p_Creature->IsNearPosition(&l_Pos, 1.0f))
                        {
                            m_IronCracksShot.insert(p_Creature->GetGUID());
                            break;
                        }
                        else
                        {
                            l_Pos = { 417.047f, 3318.0f, 303.685f, 3.18233f };

                            if (p_Creature->IsNearPosition(&l_Pos, 1.0f))
                                m_IronCracksShot.insert(p_Creature->GetGUID());

                            break;
                        }

                        if (p_Creature->ToTempSummon() && p_Creature->ToTempSummon()->GetSummoner())
                            break;

                        p_Creature->DespawnOrUnsummon();
                        break;
                    }
                    case eFoundryCreatures::GromkarFiremender:
                    {
                        Position l_Pos = { 415.056f, 3321.31006f, 303.685f, 3.54152f };

                        if (p_Creature->IsNearPosition(&l_Pos, 1.0f))
                        {
                            m_GromkarFiremenders.insert(p_Creature->GetGUID());
                            break;
                        }
                        else
                        {
                            l_Pos = { 425.788f, 3316.83f, 303.66f, 2.9753f };

                            if (p_Creature->IsNearPosition(&l_Pos, 1.0f))
                                m_GromkarFiremenders.insert(p_Creature->GetGUID());

                            break;
                        }

                        if (p_Creature->ToTempSummon() && p_Creature->ToTempSummon()->GetSummoner())
                            break;

                        p_Creature->DespawnOrUnsummon();
                        break;
                    }
                    case eFoundryCreatures::BossOperatorThogar:
                    {
                        m_OperatorThogarGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::BossAdmiralGaran:
                    {
                        m_AdmiralGaranGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::BossEnforcerSorka:
                    {
                        m_EnforcerSorkaGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::BossMarakTheBlooded:
                    {
                        m_MarakTheBloodedGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::ZiplineStalker:
                    {
                        m_ZiplineStalkerGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::IronCannon:
                    {
                        m_IronCannonGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::ForgemistressFlamehand:
                    {
                        if (!p_Creature->isAlive())
                        {
                            instance->SetObjectVisibility(500.0f);

                            if (GameObject* l_Door = instance->GetGameObject(m_PreBlackhandsGate))
                                l_Door->SetGoState(GOState::GO_STATE_ACTIVE);
                        }

                        break;
                    }
                    case eFoundryCreatures::BossBlackhand:
                    {
                        m_BlackhandGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::BlackrockFoundryTrigger:
                    {
                        m_BRFTriggerGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::CeilingStalker:
                    {
                        m_CeilingStalkerGuid = p_Creature->GetGUID();
                        break;
                    }
                    case eFoundryCreatures::AchievementStalker:
                    {
                        m_AchievementStalker = p_Creature->GetGUID();
                        break;
                    }
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* p_GameObject) override
            {
                switch (p_GameObject->GetEntry())
                {
                    case eFoundryGameObjects::GruulSpikeDoor:
                    case eFoundryGameObjects::BKFoundrySpikeTrapGate:
                    case eFoundryGameObjects::FurnacePortcullis:
                    case eFoundryGameObjects::BlastFurnaceEncounterDoor:
                    case eFoundryGameObjects::HansgarAndFranzokEntrance:
                    case eFoundryGameObjects::HansgarAndFranzokExit:
                    case eFoundryGameObjects::FirewallDoor:
                    case eFoundryGameObjects::BurningFrontDoor:
                    case eFoundryGameObjects::KromogsDoorSouth:
                    case eFoundryGameObjects::KromogsDoorEast:
                    case eFoundryGameObjects::KromogDoor:
                    case eFoundryGameObjects::BlackForgePortcullis:
                    case eFoundryGameObjects::BlackForgeGate:
                    case eFoundryGameObjects::TheBeastGate:
                    case eFoundryGameObjects::TerminusDoor:
                    case eFoundryGameObjects::FreightElevatorDoor:
                    case eFoundryGameObjects::IronMaidensRampDoor:
                    case eFoundryGameObjects::IronMaidensExitDoor:
                    case eFoundryGameObjects::BlackhandsGate:
                        AddDoor(p_GameObject, true);
                        break;
                    case eFoundryGameObjects::VolatileBlackrockOre:
                        m_VolatileOreGuid = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::CrucibleLeft:
                        m_CrucibleLeftGuid = p_GameObject->GetGUID();
                        p_GameObject->SetAIAnimKitId(eFoundryVisuals::CrucibleVisuals);
                        break;
                    case eFoundryGameObjects::CrucibleRight:
                        m_CrucibleRightGuid = p_GameObject->GetGUID();
                        p_GameObject->SetAIAnimKitId(eFoundryVisuals::CrucibleVisuals);
                        break;
                    case eFoundryGameObjects::FurnaceGate:
                        m_FurnaceGate = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::ConveyorBelt001:
                    case eFoundryGameObjects::ConveyorBelt002:
                    case eFoundryGameObjects::ConveyorBelt003:
                    case eFoundryGameObjects::ConveyorBelt004:
                    case eFoundryGameObjects::ConveyorBelt005:
                        p_GameObject->SetAIAnimKitId(eFoundryVisuals::ConveyorsStop);
                        break;
                    case eFoundryGameObjects::ConveyorBelt006:
                    case eFoundryGameObjects::ConveyorBelt008:
                    case eFoundryGameObjects::ConveyorBelt010:
                        p_GameObject->SendGameObjectActivateAnimKit(eFoundryVisuals::ConveyorsStart2, true);
                        break;
                    case eFoundryGameObjects::ConveyorBelt007:
                    case eFoundryGameObjects::ConveyorBelt009:
                        p_GameObject->SendGameObjectActivateAnimKit(eFoundryVisuals::ConveyorsStart3, true);
                        break;
                    case eFoundryGameObjects::SlagworksDoor:
                        m_SlagworksEntrance = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::BlackForgeEntrance:
                        m_BlackForgeEntrance = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::IronAssembleyGate:
                        m_IronAssemblyEntrance = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::SpikeGate:
                        m_SpikeGateGuid = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::CrucibleDoor:
                        m_CrucibleEntrance = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::BeastsEnclosureDoor:
                        m_BeastsEnclosureDoor = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::MassiveDoorTrack4Right:
                    case eFoundryGameObjects::MassiveDoorTrack3Right:
                    case eFoundryGameObjects::MassiveDoorTrack2Right:
                    case eFoundryGameObjects::MassiveDoorTrack1Right:
                    case eFoundryGameObjects::MassiveDoorTrack4Left:
                    case eFoundryGameObjects::MassiveDoorTrack3Left:
                    case eFoundryGameObjects::MassiveDoorTrack2Left:
                    case eFoundryGameObjects::MassiveDoorTrack1Left:
                        m_TrackDoorsGuids[p_GameObject->GetEntry()] = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::IronGate:
                        m_IronGateDoorGuid = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::AmmoLoader:
                        m_AmmoLoaderGuid = p_GameObject->GetGUID();
                        p_GameObject->SetAIAnimKitId(eFoundryVisuals::AmmoLoaderAnim);
                        break;
                    case eFoundryGameObjects::HardenedSlagEntrance:
                        m_HardenedSlagEntranceGuid = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::HardenedSlagBlastFurnace:
                        m_HardenedSlagFurnaceGuid = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::PreBlackhandsGate:
                        m_PreBlackhandsGate = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::BlackhandsCrucible01:
                    case eFoundryGameObjects::BlackhandsCrucible02:
                    case eFoundryGameObjects::BlackhandsCrucible03:
                    case eFoundryGameObjects::BlackhandsCrucible04:
                    case eFoundryGameObjects::BlackhandsCrucible05:
                        m_BlackhandsCrucibles[p_GameObject->GetEntry()] = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::BlackhandAPlatform:
                        m_BlackhandPlatform = p_GameObject->GetGUID();
                        instance->AddScriptedCollisionGameObject(m_BlackhandPlatform);
                        break;
                    case eFoundryGameObjects::HangingIronStar:
                        m_HangingIronStar = p_GameObject->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* p_GameObject) override
            {
                switch (p_GameObject->GetEntry())
                {
                    case eFoundryGameObjects::GruulSpikeDoor:
                    case eFoundryGameObjects::BKFoundrySpikeTrapGate:
                    case eFoundryGameObjects::FurnacePortcullis:
                    case eFoundryGameObjects::BlastFurnaceEncounterDoor:
                    case eFoundryGameObjects::HansgarAndFranzokEntrance:
                    case eFoundryGameObjects::HansgarAndFranzokExit:
                    case eFoundryGameObjects::FirewallDoor:
                    case eFoundryGameObjects::BurningFrontDoor:
                    case eFoundryGameObjects::KromogsDoorSouth:
                    case eFoundryGameObjects::KromogsDoorEast:
                    case eFoundryGameObjects::KromogDoor:
                    case eFoundryGameObjects::BlackForgePortcullis:
                    case eFoundryGameObjects::BlackForgeGate:
                    case eFoundryGameObjects::TheBeastGate:
                    case eFoundryGameObjects::TerminusDoor:
                    case eFoundryGameObjects::FreightElevatorDoor:
                    case eFoundryGameObjects::IronMaidensRampDoor:
                    case eFoundryGameObjects::IronMaidensExitDoor:
                    case eFoundryGameObjects::BlackhandsGate:
                        AddDoor(p_GameObject, false);
                        break;
                    case eFoundryGameObjects::BlackhandAPlatform:
                        instance->RemoveScriptedCollisionGameObject(m_BlackhandPlatform);
                        break;
                    default:
                        break;
                }
            }

            void OnCreatureKilled(Creature* p_Creature, Player* p_Killer) override
            {
                switch (p_Creature->GetEntry())
                {
                    case eIronMaidensCreatures::AquaticTechnician:
                    case eIronMaidensCreatures::IronDockworker:
                    case eIronMaidensCreatures::IronEarthbinder:
                    case eIronMaidensCreatures::IronMauler:
                    case eIronMaidensCreatures::IronCleaver:
                    {
                        if (Creature* l_Garan = instance->GetCreature(m_AdmiralGaranGuid))
                        {
                            if (l_Garan->IsAIEnabled)
                                l_Garan->AI()->SetGUID(p_Creature->GetGUID());
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            bool SetBossState(uint32 p_BossID, EncounterState p_State) override
            {
                uint32 l_OldState = GetBossState(p_BossID);

                if (!InstanceScript::SetBossState(p_BossID, p_State))
                    return false;

                /// Handle Blackhand's doors - must be here in case of loading
                /*for (uint8 l_I = 0; l_I < eFoundryDatas::DataBlackhand; ++l_I)
                {
                    if (GetBossState(l_I) != EncounterState::DONE)
                        break;*/

                    if (GameObject* l_SpikeGate = instance->GetGameObject(m_SpikeGateGuid))
                        l_SpikeGate->SetGoState(GOState::GO_STATE_ACTIVE);

                    if (GameObject* l_CrucibleEntrance = instance->GetGameObject(m_CrucibleEntrance))
                        l_CrucibleEntrance->SetGoState(GOState::GO_STATE_ACTIVE);

                    instance->SetObjectVisibility(500.0f);
                //}

                /// Just cosmetic stuff
                if (p_BossID == eFoundryDatas::DataBlastFurnace && p_State == EncounterState::DONE)
                {
                    if (GameObject* l_HardenedSlag = instance->GetGameObject(m_HardenedSlagEntranceGuid))
                        l_HardenedSlag->SetGoState(GOState::GO_STATE_ACTIVE);

                    if (GameObject* l_HardenedSlagFurnace = instance->GetGameObject(m_HardenedSlagFurnaceGuid))
                        l_HardenedSlagFurnace->SetGoState(GOState::GO_STATE_ACTIVE);

                    if (GameObject* l_CrucibleLeft = instance->GetGameObject(m_CrucibleLeftGuid))
                        l_CrucibleLeft->SetAIAnimKitId(0);

                    if (GameObject* l_CrucibleRight = instance->GetGameObject(m_CrucibleRightGuid))
                        l_CrucibleRight->SetAIAnimKitId(0);
                }

                /// Don't handle the DONE state in case of loading
                if (p_State == EncounterState::DONE && l_OldState != EncounterState::IN_PROGRESS)
                    return true;

                switch (p_BossID)
                {
                    case eFoundryDatas::DataGruul:
                    {
                        switch (p_State)
                        {
                            case EncounterState::DONE:
                            {
                                if (m_PristineTrueIronOres >= eFoundryDatas::MaxPristineTrueIronOres && !instance->IsLFR())
                                    DoCompleteAchievement(eFoundryAchievements::TheIronPrince);

                                break;
                            }
                            case EncounterState::NOT_STARTED:
                            {
                                m_PristineTrueIronOres = 0;
                                break;
                            }
                            default:
                                break;
                        }

                        break;
                    }
                    case eFoundryDatas::DataOregorger:
                    {
                        switch (p_State)
                        {
                            case EncounterState::DONE:
                            {
                                if (m_VolatileOreGrinded && !instance->IsLFR())
                                    DoCompleteAchievement(eFoundryAchievements::HeShootsHeOres);

                                break;
                            }
                            case EncounterState::NOT_STARTED:
                            {
                                m_VolatileOreGrinded = false;
                                break;
                            }
                            default:
                                break;
                        }

                        break;
                    }
                    case eFoundryDatas::DataBlastFurnace:
                    {
                        switch (p_State)
                        {
                            case EncounterState::DONE:
                            {
                                if (m_YaWeveGotTimeAchiev && !instance->IsLFR())
                                    DoCompleteAchievement(eFoundryAchievements::YaWeveGotTime);

                                if (GameObject* l_HardenedSlag = instance->GetGameObject(m_HardenedSlagEntranceGuid))
                                    l_HardenedSlag->SetGoState(GOState::GO_STATE_ACTIVE);

                                if (GameObject* l_HardenedSlagFurnace = instance->GetGameObject(m_HardenedSlagFurnaceGuid))
                                    l_HardenedSlagFurnace->SetGoState(GOState::GO_STATE_ACTIVE);

                                if (GameObject* l_CrucibleLeft = instance->GetGameObject(m_CrucibleLeftGuid))
                                    l_CrucibleLeft->SetAIAnimKitId(0);

                                if (GameObject* l_CrucibleRight = instance->GetGameObject(m_CrucibleRightGuid))
                                    l_CrucibleRight->SetAIAnimKitId(0);

                                break;
                            }
                            case EncounterState::NOT_STARTED:
                            {
                                m_PrimalElementalistTime = 0;
                                m_YaWeveGotTimeAchiev = false;
                                break;
                            }
                            default:
                                break;
                        }

                        break;
                    }
                    case eFoundryDatas::DataHansgarAndFranzok:
                    {
                        switch (p_State)
                        {
                            case EncounterState::DONE:
                            {
                                if (m_StampStampRevolution && !instance->IsLFR())
                                    DoCompleteAchievement(eFoundryAchievements::StampStampRevolution);

                                AddTimedDelayedOperation(7 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                                {
                                    if (Creature* l_Hansgar = instance->GetCreature(m_HansgarGuid))
                                    {
                                        if (l_Hansgar->IsAIEnabled)
                                            l_Hansgar->AI()->Talk(8);   ///< Death
                                    }
                                });

                                break;
                            }
                            case EncounterState::NOT_STARTED:
                            {
                                m_StampStampRevolution = true;
                                break;
                            }
                            default:
                                break;
                        }

                        break;
                    }
                    case eFoundryDatas::DataFlamebenderKagraz:
                    {
                        switch (p_State)
                        {
                            case EncounterState::DONE:
                            {
                                if (m_SteelHasBeenBrought && !instance->IsLFR())
                                {
                                    DoCompleteAchievement(eFoundryAchievements::TheSteelHasBeenBrought);
                                    DoCastSpellOnPlayers(eFoundrySpells::FollowerAknorSteelbringer);
                                }

                                break;
                            }
                            case EncounterState::NOT_STARTED:
                            {
                                m_SteelHasBeenBrought = true;
                                break;
                            }
                            default:
                                break;
                        }

                        break;
                    }
                    case eFoundryDatas::DataKromog:
                    {
                        switch (p_State)
                        {
                            case EncounterState::DONE:
                            {
                                if (m_WouldYouGiveMeAHand && !instance->IsLFR())
                                {
                                    if (Creature* l_Kromog = instance->GetCreature(m_KromogGuid))
                                    {
                                        if (!sObjectMgr->IsDisabledEncounter(GetEncounterIDForBoss(l_Kromog), instance->GetDifficultyID()))
                                            DoCompleteAchievement(eFoundryAchievements::WouldYouGiveMeAHand);
                                    }
                                }

                                AddTimedDelayedOperation(7 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                                {
                                    if (Creature* l_Blackhand = instance->GetCreature(m_CosmeticBlackhand))
                                    {
                                        if (l_Blackhand->IsAIEnabled)
                                            l_Blackhand->AI()->Talk(12, 0, TextRange::TEXT_RANGE_MAP);   ///< KromogKilled
                                    }
                                });

                                break;
                            }
                            case EncounterState::NOT_STARTED:
                            {
                                m_WouldYouGiveMeAHand       = false;
                                m_GraspingEarthHandsCount   = 0;
                                m_GraspingEarthHandsTime    = 0;
                                break;
                            }
                            default:
                                break;
                        }

                        break;
                    }
                    case eFoundryDatas::DataBeastlordDarmac:
                    {
                        switch (p_State)
                        {
                            case EncounterState::FAIL:
                            case EncounterState::NOT_STARTED:
                            {
                                SendUpdateWorldState(eFoundryWorldState::WorldStateDarmacAchievement, 0);
                                m_DarmacBeastMountedFirst = 0;
                                break;
                            }
                            case EncounterState::DONE:
                            {
                                if (m_DarmacBeastMountedFirst < eFoundryDatas::DataDreadwingFirst)
                                    break;

                                std::vector<uint32> l_Criterias =
                                {
                                    eFoundryCriterias::DreadwingMountedFirst,
                                    eFoundryCriterias::IroncrusherMountedFirst,
                                    eFoundryCriterias::CruelfangMountedFirst
                                };

                                if (CriteriaEntry const* l_Criteria = sCriteriaStore.LookupEntry(l_Criterias[m_DarmacBeastMountedFirst - 1]))
                                    SetCriteriaProgressOnPlayers(l_Criteria, 1, ProgressType::PROGRESS_ACCUMULATE);

                                if (m_GromkarMenAtArms.size() < eFoundryDatas::DataGromkarManAtArms ||
                                    m_IronRaiders.size() < eFoundryDatas::DataIronRaider ||
                                    m_IronCracksShot.size() < eFoundryDatas::DataIronCrackShot ||
                                    m_GromkarFiremenders.size() < eFoundryDatas::DataGromkarFiremender)
                                    break;

                                for (uint8 l_I = 0; l_I < eFoundryDatas::DataGromkarManAtArms; ++l_I)
                                {
                                    uint64 l_Guid = (*m_GromkarMenAtArms.begin());

                                    if (Creature* l_Creature = instance->GetCreature(l_Guid))
                                    {
                                        l_Creature->Respawn(true);

                                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this, l_Guid]() -> void
                                        {
                                            if (Creature* l_Creature = instance->GetCreature(l_Guid))
                                            {
                                                l_Creature->SetHomePosition(g_GromkarManAtArmsMovePos);
                                                l_Creature->GetMotionMaster()->Clear();
                                                l_Creature->GetMotionMaster()->MoveTargetedHome();
                                            }
                                        });
                                    }

                                    m_GromkarMenAtArms.erase(l_Guid);
                                }

                                for (uint8 l_I = 0; l_I < eFoundryDatas::DataIronRaider; ++l_I)
                                {
                                    uint64 l_Guid = (*m_IronRaiders.begin());

                                    if (Creature* l_Creature = instance->GetCreature(l_Guid))
                                    {
                                        l_Creature->Respawn(true);

                                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this, l_Guid, l_I]() -> void
                                        {
                                            if (Creature* l_Creature = instance->GetCreature(l_Guid))
                                            {
                                                l_Creature->SetHomePosition(g_IronRaiderMovePos[l_I]);
                                                l_Creature->GetMotionMaster()->Clear();
                                                l_Creature->GetMotionMaster()->MoveTargetedHome();
                                            }
                                        });
                                    }

                                    m_IronRaiders.erase(l_Guid);
                                }

                                for (uint8 l_I = 0; l_I < eFoundryDatas::DataIronCrackShot; ++l_I)
                                {
                                    uint64 l_Guid = (*m_IronCracksShot.begin());

                                    if (Creature* l_Creature = instance->GetCreature(l_Guid))
                                    {
                                        l_Creature->Respawn(true);

                                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this, l_Guid, l_I]() -> void
                                        {
                                            if (Creature* l_Creature = instance->GetCreature(l_Guid))
                                            {
                                                l_Creature->SetHomePosition(g_IronCrackShotMovePos[l_I]);
                                                l_Creature->GetMotionMaster()->Clear();
                                                l_Creature->GetMotionMaster()->MoveTargetedHome();
                                            }
                                        });
                                    }

                                    m_IronCracksShot.erase(l_Guid);
                                }

                                for (uint8 l_I = 0; l_I < eFoundryDatas::DataGromkarFiremender; ++l_I)
                                {
                                    uint64 l_Guid = (*m_GromkarFiremenders.begin());

                                    if (Creature* l_Creature = instance->GetCreature(l_Guid))
                                    {
                                        l_Creature->Respawn(true);

                                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this, l_Guid, l_I]() -> void
                                        {
                                            if (Creature* l_Creature = instance->GetCreature(l_Guid))
                                            {
                                                l_Creature->SetHomePosition(g_GromkarFiremenderMovePos[l_I]);
                                                l_Creature->GetMotionMaster()->Clear();
                                                l_Creature->GetMotionMaster()->MoveTargetedHome();
                                            }
                                        });
                                    }

                                    m_GromkarFiremenders.erase(l_Guid);
                                }

                                break;
                            }
                            default:
                                break;
                        }

                        break;
                    }
                    case eFoundryDatas::DataOperatorThogar:
                    {
                        switch (p_State)
                        {
                            case EncounterState::DONE:
                            {
                                instance->SetObjectVisibility(200.0f);

                                if (GameObject* l_IronGate = instance->GetGameObject(m_IronGateDoorGuid))
                                    l_IronGate->SetGoState(GOState::GO_STATE_ACTIVE);

                                break;
                            }
                            case EncounterState::FAIL:
                            {
                                if (GameObject* l_IronGate = instance->GetGameObject(m_IronGateDoorGuid))
                                    l_IronGate->SetGoState(GOState::GO_STATE_ACTIVE);

                                break;
                            }
                            case EncounterState::IN_PROGRESS:
                            {
                                AddTimedDelayedOperation(5 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                                {
                                    if (GameObject* l_IronGate = instance->GetGameObject(m_IronGateDoorGuid))
                                        l_IronGate->SetGoState(GOState::GO_STATE_READY);
                                });

                                break;
                            }
                            default:
                                break;
                        }

                        break;
                    }
                    case eFoundryDatas::DataIronMaidens:
                    {
                        switch (p_State)
                        {
                            case EncounterState::DONE:
                            {
                                if (m_BeQuickOrBeDeadAchiev && !instance->IsLFR())
                                    DoCompleteAchievement(eFoundryAchievements::BeQuickOrBeDead);

                                AddTimedDelayedOperation(10 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                                {
                                    if (Creature* l_Blackhand = instance->GetCreature(m_CosmeticBlackhand))
                                    {
                                        if (l_Blackhand->IsAIEnabled)
                                            l_Blackhand->AI()->Talk(13, 0, TextRange::TEXT_RANGE_MAP);   ///< KromogKilled
                                    }
                                });

                                /// No break needed here
                            }
                            case EncounterState::FAIL:
                            case EncounterState::NOT_STARTED:
                            {
                                m_BeQuickOrBeDeadAchiev = false;

                                if (GameObject* l_AmmoLoader = instance->GetGameObject(m_AmmoLoaderGuid))
                                    l_AmmoLoader->SetFlag(EGameObjectFields::GAMEOBJECT_FIELD_FLAGS, GameObjectFlags::GO_FLAG_NOT_SELECTABLE);

                                DoRemoveAurasDueToSpellOnPlayers(eIronMaidensSpells::OnABoatPeriodic);
                                break;
                            }
                            default:
                                break;
                        }

                        break;
                    }
                    case eFoundryDatas::DataBlackhand:
                    {
                        switch (p_State)
                        {
                            case EncounterState::DONE:
                            {
                                instance->SetObjectVisibility(150.0f);

                                if (m_CollectedAshes >= eFoundryDatas::MaxAshesToCollect && !instance->IsLFR())
                                    DoCompleteAchievement(eFoundryAchievements::AshesAshes);

                                break;
                            }
                            default:
                            {
                                m_CollectedAshes = 0;
                                break;
                            }
                        }

                        break;
                    }
                    default:
                        break;
                }

                return true;
            }

            void SetData(uint32 p_Type, uint32 p_Data) override
            {
                switch (p_Type)
                {
                    case eFoundryDatas::PristineTrueIronOres:
                    {
                        if (instance->IsLFR())
                            break;

                        ++m_PristineTrueIronOres;
                        break;
                    }
                    case eFoundryDatas::VolatileOreGrinded:
                    {
                        if (instance->IsLFR())
                            break;

                        m_VolatileOreGrinded = p_Data != 0;
                        break;
                    }
                    case eFoundryDatas::PrimalElementalistTime:
                    {
                        if (instance->IsLFR())
                            break;

                        if (!m_PrimalElementalistTime)
                            m_PrimalElementalistTime = p_Data;
                        else
                        {
                            /// Defeating all four Primal Elementalists within 10 seconds of each other
                            if (p_Data > (m_PrimalElementalistTime + 10))
                                m_YaWeveGotTimeAchiev = false;
                            else
                                m_YaWeveGotTimeAchiev = true;
                        }

                        break;
                    }
                    case eFoundryDatas::PlayerStamped:
                    {
                        if (instance->IsLFR())
                            break;

                        m_StampStampRevolution = false;
                        break;
                    }
                    case eFoundryDatas::IronTaskmasterAggro:
                    {
                        m_IronTaskmasterAggro = true;
                        break;
                    }
                    case eFoundryDatas::AknorDied:
                    {
                        if (instance->IsLFR())
                            break;

                        m_SteelHasBeenBrought = false;
                        break;
                    }
                    case eFoundryDatas::GraspingEarthTime:
                    {
                        if (instance->IsLFR())
                            break;

                        /// Requirements are already completed, doesn't need to update
                        if (m_WouldYouGiveMeAHand)
                            break;

                        ++m_GraspingEarthHandsCount;

                        /// Register first time kill
                        if (!m_GraspingEarthHandsTime)
                            m_GraspingEarthHandsTime = p_Data;

                        if (m_GraspingEarthHandsCount >= eFoundryDatas::MaxGraspingEarthHands)
                        {
                            if (p_Data < (m_GraspingEarthHandsTime + 5))
                                m_WouldYouGiveMeAHand = true;
                        }

                        break;
                    }
                    case eFoundryDatas::DarmacBeastMountedFirst:
                    {
                        if (instance->IsLFR())
                            break;

                        /// Already set
                        if (m_DarmacBeastMountedFirst > 0)
                            break;

                        m_DarmacBeastMountedFirst = p_Data;
                        SendUpdateWorldState(eFoundryWorldState::WorldStateDarmacAchievement, p_Data);
                        break;
                    }
                    case eFoundryDatas::DataThogarIntroStarted:
                    {
                        m_ThogarIntroStarted = true;
                        break;
                    }
                    case eFoundryDatas::IronMaidensKillTimer:
                    {
                        if (instance->IsLFR())
                            break;

                        if (!m_IronMaidensKillTime)
                            m_IronMaidensKillTime = p_Data;
                        else
                        {
                            /// Defeat the Iron Maidens within 10 seconds of each other
                            if (p_Data > (m_IronMaidensKillTime + 10))
                                m_BeQuickOrBeDeadAchiev = false;
                            else
                                m_BeQuickOrBeDeadAchiev = true;
                        }

                        break;
                    }
                    case eFoundryDatas::BlackhandAshCollected:
                    {
                        if (instance->IsLFR() || GetBossState(eFoundryDatas::DataBlackhand) != EncounterState::IN_PROGRESS)
                            break;

                        ++m_CollectedAshes;
                        break;
                    }
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 p_ID) override
            {
                switch (p_ID)
                {
                    case eFoundryDatas::IronTaskmasterAggro:
                        return (uint32)m_IronTaskmasterAggro;
                    case eFoundryDatas::DataThogarIntroStarted:
                        return (uint32)m_ThogarIntroStarted;
                    default:
                        break;
                }

                return 0;
            }

            uint64 GetData64(uint32 p_Type) override
            {
                switch (p_Type)
                {
                    case eFoundryCreatures::BossGruul:
                        return m_GruulGuid;
                    case eFoundryCreatures::BossOregorger:
                        return m_OregorgerGuid;
                    case eFoundryCreatures::HeartOfTheMountain:
                        return m_HeartOfTheMountain;
                    case eFoundryCreatures::ForemanFeldspar:
                        return m_ForemanFeldspar;
                    case eFoundryCreatures::BlackhandCosmetic:
                        return m_CosmeticBlackhand;
                    case eFoundryGameObjects::VolatileBlackrockOre:
                        return m_VolatileOreGuid;
                    case eFoundryGameObjects::FurnaceGate:
                        return m_FurnaceGate;
                    case eFoundryCreatures::BossHansgar:
                        return m_HansgarGuid;
                    case eFoundryCreatures::BossFranzok:
                        return m_FranzokGuid;
                    case eFoundryCreatures::BossFlamebenderKagraz:
                        return m_FlamebenderKagrazGuid;
                    case eFoundryCreatures::AknorSteelbringer:
                        return m_AknorSteelbringerGuid;
                    case eFoundryCreatures::LavaStalker:
                        return m_LavaStalkerGuid;
                    case eFoundryCreatures::MoltenTorrentStalker:
                        return m_MoltenTorrentStalkerGuid;
                    case eFoundryCreatures::BossKromog:
                        return m_KromogGuid;
                    case eFoundryCreatures::BossBeastlordDarmac:
                        return m_BeastlordDarmacGuid;
                    case eFoundryCreatures::BossCruelfang:
                        return m_CruelfangGuid;
                    case eFoundryCreatures::BossDreadwing:
                        return m_DreadwingGuid;
                    case eFoundryCreatures::BossIroncrusher:
                        return m_IroncrusherGuid;
                    case eFoundryCreatures::ThunderlordPackPens:
                        return m_ThunderlordPackPens;
                    case eFoundryGameObjects::BeastsEnclosureDoor:
                        return m_BeastsEnclosureDoor;
                    case eFoundryGameObjects::MassiveDoorTrack4Right:
                    case eFoundryGameObjects::MassiveDoorTrack3Right:
                    case eFoundryGameObjects::MassiveDoorTrack2Right:
                    case eFoundryGameObjects::MassiveDoorTrack1Right:
                    case eFoundryGameObjects::MassiveDoorTrack1Left:
                    case eFoundryGameObjects::MassiveDoorTrack2Left:
                    case eFoundryGameObjects::MassiveDoorTrack3Left:
                    case eFoundryGameObjects::MassiveDoorTrack4Left:
                        return m_TrackDoorsGuids[p_Type];
                    case eFoundryCreatures::BossOperatorThogar:
                        return m_OperatorThogarGuid;
                    case eFoundryGameObjects::IronGate:
                        return m_IronGateDoorGuid;
                    case eFoundryCreatures::BossAdmiralGaran:
                        return m_AdmiralGaranGuid;
                    case eFoundryCreatures::BossEnforcerSorka:
                        return m_EnforcerSorkaGuid;
                    case eFoundryCreatures::BossMarakTheBlooded:
                        return m_MarakTheBloodedGuid;
                    case eFoundryCreatures::ZiplineStalker:
                        return m_ZiplineStalkerGuid;
                    case eFoundryCreatures::IronCannon:
                        return m_IronCannonGuid;
                    case eFoundryGameObjects::AmmoLoader:
                        return m_AmmoLoaderGuid;
                    case eFoundryGameObjects::PreBlackhandsGate:
                        return m_PreBlackhandsGate;
                    case eFoundryCreatures::BossBlackhand:
                        return m_BlackhandGuid;
                    case eFoundryGameObjects::BlackhandsCrucible01:
                    case eFoundryGameObjects::BlackhandsCrucible02:
                    case eFoundryGameObjects::BlackhandsCrucible03:
                    case eFoundryGameObjects::BlackhandsCrucible04:
                    case eFoundryGameObjects::BlackhandsCrucible05:
                        return m_BlackhandsCrucibles[p_Type];
                    case eFoundryCreatures::BlackrockFoundryTrigger:
                        return m_BRFTriggerGuid;
                    case eFoundryCreatures::CeilingStalker:
                        return m_CeilingStalkerGuid;
                    case eFoundryGameObjects::BlackhandAPlatform:
                        return m_BlackhandPlatform;
                    case eFoundryGameObjects::HangingIronStar:
                        return m_HangingIronStar;
                    case eFoundryCreatures::AchievementStalker:
                        return m_AchievementStalker;
                    default:
                        break;
                }

                return 0;
            }

            bool CheckRequiredBosses(uint32 p_BossID, Player const* p_Player = nullptr) const override
            {
                if (!InstanceScript::CheckRequiredBosses(p_BossID, p_Player))
                    return false;

                switch (p_BossID)
                {
                    case eFoundryDatas::DataBlastFurnace:
                    {
                        /// Oregroger and Gruul must be defeated to starts Blast Furnace encounter
                        if (GetBossState(eFoundryDatas::DataOregorger) != EncounterState::DONE ||
                            GetBossState(eFoundryDatas::DataGruul) != EncounterState::DONE)
                            return false;

                        return true;
                    }
                    case eFoundryDatas::DataFlamebenderKagraz:
                    case eFoundryDatas::DataKromog:
                    case eFoundryDatas::DataOperatorThogar:
                    case eFoundryDatas::DataIronMaidens:
                    {
                        if (GetBossState(p_BossID - 1) != EncounterState::DONE)
                            return false;

                        break;
                    }
                    /*case eFoundryDatas::DataBlackhand:
                    {
                        for (uint8 l_I = 0; l_I < eFoundryDatas::DataBlackhand; ++l_I)
                        {
                            if (GetBossState(l_I) != EncounterState::DONE)
                                return false;
                        }

                        break;
                    }*/
                    default:
                        break;
                }

                return true;
            }

            void OnPlayerEnter(Player* p_Player) override
            {
                InstanceScript::OnPlayerEnter(p_Player);

                /// Disable non available bosses for LFR
                if (!m_Initialized)
                {
                    m_Initialized = true;

                    m_DungeonID = p_Player->GetGroup() ? sLFGMgr->GetDungeon(p_Player->GetGroup()->GetGUID()) : 0;

                    if (!instance->IsLFR())
                        m_DungeonID = 0;

                    std::vector<uint64> m_DisabledGoBs;

                    switch (m_DungeonID)
                    {
                        case eFoundryDungeons::Slagworks:
                        {
                            uint32 l_DisabledMask = 0;

                            l_DisabledMask |= (1 << eFoundryDatas::DataHansgarAndFranzok);
                            l_DisabledMask |= (1 << eFoundryDatas::DataFlamebenderKagraz);
                            l_DisabledMask |= (1 << eFoundryDatas::DataKromog);
                            l_DisabledMask |= (1 << eFoundryDatas::DataBeastlordDarmac);
                            l_DisabledMask |= (1 << eFoundryDatas::DataOperatorThogar);
                            l_DisabledMask |= (1 << eFoundryDatas::DataIronMaidens);
                            l_DisabledMask |= (1 << eFoundryDatas::DataBlackhand);

                            SetDisabledBosses(l_DisabledMask);

                            m_DisabledGoBs.push_back(m_BlackForgeEntrance);
                            m_DisabledGoBs.push_back(m_IronAssemblyEntrance);
                            m_DisabledGoBs.push_back(m_SpikeGateGuid);
                            m_DisabledGoBs.push_back(m_CrucibleEntrance);
                            break;
                        }
                        case eFoundryDungeons::BlackForge:
                        {
                            uint32 l_DisabledMask = 0;

                            l_DisabledMask |= (1 << eFoundryDatas::DataGruul);
                            l_DisabledMask |= (1 << eFoundryDatas::DataOregorger);
                            l_DisabledMask |= (1 << eFoundryDatas::DataBlastFurnace);
                            l_DisabledMask |= (1 << eFoundryDatas::DataBeastlordDarmac);
                            l_DisabledMask |= (1 << eFoundryDatas::DataOperatorThogar);
                            l_DisabledMask |= (1 << eFoundryDatas::DataIronMaidens);
                            l_DisabledMask |= (1 << eFoundryDatas::DataBlackhand);

                            SetDisabledBosses(l_DisabledMask);

                            m_DisabledGoBs.push_back(m_SlagworksEntrance);
                            m_DisabledGoBs.push_back(m_IronAssemblyEntrance);
                            m_DisabledGoBs.push_back(m_SpikeGateGuid);
                            m_DisabledGoBs.push_back(m_CrucibleEntrance);
                            break;
                        }
                        case eFoundryDungeons::IronAssembly:
                        {
                            uint32 l_DisabledMask = 0;

                            l_DisabledMask |= (1 << eFoundryDatas::DataGruul);
                            l_DisabledMask |= (1 << eFoundryDatas::DataOregorger);
                            l_DisabledMask |= (1 << eFoundryDatas::DataBlastFurnace);
                            l_DisabledMask |= (1 << eFoundryDatas::DataHansgarAndFranzok);
                            l_DisabledMask |= (1 << eFoundryDatas::DataFlamebenderKagraz);
                            l_DisabledMask |= (1 << eFoundryDatas::DataKromog);
                            l_DisabledMask |= (1 << eFoundryDatas::DataBlackhand);

                            SetDisabledBosses(l_DisabledMask);

                            m_DisabledGoBs.push_back(m_SlagworksEntrance);
                            m_DisabledGoBs.push_back(m_BlackForgeEntrance);
                            m_DisabledGoBs.push_back(m_SpikeGateGuid);
                            m_DisabledGoBs.push_back(m_CrucibleEntrance);
                            break;
                        }
                        case eFoundryDungeons::BlackhandsCrucible:
                        {
                            uint32 l_DisabledMask = 0;

                            l_DisabledMask |= (1 << eFoundryDatas::DataGruul);
                            l_DisabledMask |= (1 << eFoundryDatas::DataOregorger);
                            l_DisabledMask |= (1 << eFoundryDatas::DataBlastFurnace);
                            l_DisabledMask |= (1 << eFoundryDatas::DataHansgarAndFranzok);
                            l_DisabledMask |= (1 << eFoundryDatas::DataFlamebenderKagraz);
                            l_DisabledMask |= (1 << eFoundryDatas::DataKromog);
                            l_DisabledMask |= (1 << eFoundryDatas::DataBeastlordDarmac);
                            l_DisabledMask |= (1 << eFoundryDatas::DataOperatorThogar);
                            l_DisabledMask |= (1 << eFoundryDatas::DataIronMaidens);

                            SetDisabledBosses(l_DisabledMask);

                            m_DisabledGoBs.push_back(m_SlagworksEntrance);
                            m_DisabledGoBs.push_back(m_BlackForgeEntrance);
                            m_DisabledGoBs.push_back(m_IronAssemblyEntrance);
                            break;
                        }
                        default:
                            break;
                    }

                    for (uint64 l_Guid : m_DisabledGoBs)
                    {
                        if (GameObject* l_Door = sObjectAccessor->FindGameObject(l_Guid))
                            l_Door->SetGoState(GOState::GO_STATE_READY);
                    }
                }
            }

            void OnPlayerExit(Player* p_Player) override
            {
                InstanceScript::OnPlayerExit(p_Player);
            }

            void SendUpdateWorldState(uint32 p_Field, uint32 p_Value)
            {
                Map::PlayerList const& l_Players = instance->GetPlayers();
                for (Map::PlayerList::const_iterator l_Iter = l_Players.begin(); l_Iter != l_Players.end(); ++l_Iter)
                {
                    if (Player* l_Player = l_Iter->getSource())
                        l_Player->SendUpdateWorldState(p_Field, p_Value);
                }
            }

            bool IsPlayerImmuneToFallDamage(Player* p_Player) const override
            {
                float l_X = p_Player->m_positionX;
                float l_Y = p_Player->m_positionY;
                float l_Z = p_Player->m_positionZ;

                /// Elevator for Hans'gar & Franzok is a safe zone for falling damages
                if (l_X >= 239.5f && l_X <= 267.0f && l_Y >= 3487.01f && l_Y <= 3512.08f && l_Z >= 140.281f)
                    return true;

                return false;
            }

            void Update(uint32 p_Diff) override
            {
                UpdateOperations(p_Diff);
                UpdateCombatResurrection(p_Diff);
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* p_Map) const override
        {
            return new instance_blackrock_foundryMapScript(p_Map);
        }
};

#ifndef __clang_analyzer__
void AddSC_instance_blackrock_foundry()
{
    new instance_blackrock_foundry();
}
#endif
