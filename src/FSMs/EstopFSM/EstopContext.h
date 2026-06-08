#ifndef ESTOPCONTEXT_H
#define ESTOPCONTEXT_H

#include <new>
#include "../../HAL.h"
#include "EStopBase.h"
#include "EStopRemote.h"
#include "BothEStop.h"
#include "EStopState.h"
#include "Reset.h"
#include "ResetRemote.h"

class EStopContext {
    alignas(BothEStop) char stateBuffer[sizeof(BothEStop)];
    EStopBase* currentState = reinterpret_cast<EStopBase*>(stateBuffer);
    bool active = false;
public:
    HAL& hal;
    bool isActive() { return active; }
    void setActive(bool a) { active = a; }

    EStopContext(HAL& hal) : hal(hal) {
        new(currentState) EStopState;
        currentState->entry(*this);
    }

    void eventButtonEstopPressed()        { currentState->eventButtonEstopPressed(*this); }
    void eventButtonEstopReleased()       { currentState->eventButtonEstopReleased(*this); }
    void eventButtonEstopPressedRemote()  { currentState->eventButtonEstopPressedRemote(*this); }
    void eventButtonEstopReleasedRemote() { currentState->eventButtonEstopReleasedRemote(*this); }
    void eventButtonResetPressed()        { currentState->eventButtonResetPressed(*this); }
    void eventButtonResetPressedRemote()  { currentState->eventButtonResetPressedRemote(*this); }
};

#endif
