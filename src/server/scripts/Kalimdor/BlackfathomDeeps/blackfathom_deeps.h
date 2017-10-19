////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DEF_BFD_H
#define DEF_BFD_H

enum Data64
{
    DATA_SHRINE1,
    DATA_SHRINE2,
    DATA_SHRINE3,
    DATA_SHRINE4,
    DATA_TWILIGHT_LORD_KELRIS,
    DATA_SHRINE_OF_GELIHAST,
    DATA_ALTAR_OF_THE_DEEPS,
    DATA_MAINDOOR
};

enum Data
{
    TYPE_GELIHAST,
    TYPE_KELRIS,
    TYPE_SHRINE,
    TYPE_AKU_MAI,
    DATA_FIRE,
    DATA_EVENT
};

enum Creatures
{
    NPC_TWILIGHT_LORD_KELRIS                               = 4832,
    NPC_LORGUS_JETT                                        = 12902,

    NPC_AKU_MAI_SNAPJAW                                    = 4825,
    NPC_MURKSHALLOW_SOFTSHELL                              = 4977,
    NPC_AKU_MAI_SERVANT                                    = 4978,
    NPC_BARBED_CRUSTACEAN                                  = 4823,

    NPC_MORRIDUNE                                          = 6729
};

enum GameObjects
{
    GO_SHRINE_OF_GELIHAST                                  = 103015,
    GO_FIRE_OF_AKU_MAI_1                                   = 21118,
    GO_FIRE_OF_AKU_MAI_2                                   = 21119,
    GO_FIRE_OF_AKU_MAI_3                                   = 21120,
    GO_FIRE_OF_AKU_MAI_4                                   = 21121,
    GO_AKU_MAI_DOOR                                        = 21117,
    GO_ALTAR_OF_THE_DEEPS                                  = 103016
};
#endif
