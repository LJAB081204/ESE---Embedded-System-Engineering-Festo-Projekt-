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
#include "HandShakeEStopLocalNotPressed.h"
#include "HandShakeEStopLocalPressed.h"
#include "../../HeightSensor.h"


enum class EStopSource {LOCAL,REMOTE};

class EStopContext {
    alignas(BothEStop) char stateBuffer[sizeof(BothEStop)];
    EStopBase* currentState = reinterpret_cast<EStopBase*>(stateBuffer);
    bool active = false;
public:
    HAL& hal;
    HeightSensor& hs;
    bool isActive() { return active; }
    void setActive(bool a) { active = a; }

    EStopContext(HAL& hal,HeightSensor& hs,EStopSource source) : hal(hal), hs(hs) {
    	if(source == EStopSource::LOCAL)
    		new(currentState) EStopState;
    	else{
    		new(currentState) EStopRemote;
    	}
        currentState->entry(*this);
    }

    void eventButtonEstopPressed()        { currentState->eventButtonEstopPressed(*this); }
    void eventButtonEstopReleased()       { currentState->eventButtonEstopReleased(*this); }
    void eventButtonEstopPressedRemote()  { currentState->eventButtonEstopPressedRemote(*this); }
    void eventButtonEstopReleasedRemote() { currentState->eventButtonEstopReleasedRemote(*this); }
    void eventButtonResetPressed()        { currentState->eventButtonResetPressed(*this); }
    void eventButtonResetPressedRemote()  { currentState->eventButtonResetPressedRemote(*this); }
    void handshakeEStopRemoteIsPressed()    { currentState->handshakeEStopRemoteIsPressed(*this); }
    void handshakeEStopRemoteIsNotPressed() { currentState->handshakeEStopRemoteIsNotPressed(*this); }

    void enterHandshakeLocalPressed() {
        new(currentState) HandShakeEStopLocalPressed;
        currentState->entry(*this);
    }

    void enterHandshakeLocalNotPressed() {
        new(currentState) HandShakeEStopLocalNotPressed;
        currentState->entry(*this);
    }

};

#endif
