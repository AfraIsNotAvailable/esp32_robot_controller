#pragma once
#include "Looper.h"

class Subsystem {
public:
    virtual void readPeriodicInputs() = 0;
    virtual void writePeriodicOutputs() = 0;
    virtual void stop() = 0;
    virtual void registerEnabledLoops(Looper* looper) = 0;
};