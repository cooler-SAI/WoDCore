////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_CONFUSEDGENERATOR_H
#define TRINITY_CONFUSEDGENERATOR_H

#include "MovementGenerator.h"
#include "Timer.h"

template<class T>
class ConfusedMovementGenerator : public MovementGeneratorMedium< T, ConfusedMovementGenerator<T> >
{
    public:
        explicit ConfusedMovementGenerator() : i_nextMoveTime(0), i_x(0), i_y(0), i_z(0) { }

        void DoInitialize(T*);
        void DoFinalize(T*);
        void DoReset(T*);
        bool DoUpdate(T*, uint32);

        MovementGeneratorType GetMovementGeneratorType() const override { return CONFUSED_MOTION_TYPE; }
    private:
        TimeTracker i_nextMoveTime;
        float i_x, i_y, i_z;
};
#endif
