////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_CREATUREAISELECTOR_H
#define TRINITY_CREATUREAISELECTOR_H

class CreatureAI;
class Creature;
class MovementGenerator;
class GameObjectAI;
class GameObject;

namespace FactorySelector
{
    CreatureAI* selectAI(Creature*);
    MovementGenerator* selectMovementGenerator(Creature*);
    GameObjectAI* SelectGameObjectAI(GameObject*);
}
#endif

