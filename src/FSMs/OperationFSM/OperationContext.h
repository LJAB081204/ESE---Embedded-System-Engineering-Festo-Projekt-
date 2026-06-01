/*
 * OperationContext.h
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_OPERATIONCONTEXT_H_
#define SRC_FSMS_OPERATIONFSM_OPERATIONCONTEXT_H_

#include "../../HAL.h"
#include <new>
#include "OperationBase.h"
#include "Idle.h"
#include "Running.h"

class OperationContext{
	//Kontext
	alignas(Running) char stateBuffer[sizeof(Running)];
	OperationBase* currentState = reinterpret_cast<OperationBase*>(stateBuffer);

public:
	HAL& hal;
	OperationContext(HAL& hal) : hal(hal){
		new (currentState) Idle;
		currentState->entry(*this);
	}
	void eventButtonStartPressed()     { currentState->eventButtonStartPressed(*this); }
    void eventButtonStartPressedLong() { currentState->eventButtonStartPressedLong(*this); }
    void eventButtonStopPressed()      { currentState->eventButtonStopPressed(*this); }
    void eventButtonResetPressed()     { currentState->eventButtonResetPressed(*this); }
    void eventButtonEstopPressed()     { currentState->eventButtonEstopPressed(*this); }
    void eventButtonEstopReleased()    { currentState->eventButtonEstopReleased(*this); }

    void eventItemAtEntry()            { currentState->eventItemAtEntry(*this); }
    void eventItemLeftEntry()          { currentState->eventItemLeftEntry(*this); }

    void eventItemAtExit()             { currentState->eventItemAtExit(*this); }
    void eventItemLeftExit()           { currentState->eventItemLeftExit(*this); }

    void eventItemAtPusher()           { currentState->eventItemAtPusher(*this); }
    void eventItemLeftPusher()         { currentState->eventItemLeftPusher(*this); }

    void eventItemInSlide()            { currentState->eventItemInSlide(*this); }
    void eventItemLeftSlide()          { currentState->eventItemLeftSlide(*this); }

    void eventItemMetal()			   { currentState->eventItemMetal(*this); }
    void eventItemGone()			   { currentState->eventItemGone(*this);}

    void eventHeightHigh()			   { currentState->eventHeightHigh(*this); }
    void eventHeightFlat()			   { currentState->eventHeightFlat(*this); }
    void eventHeightHole()			   { currentState->eventHeightHole(*this); }
    void eventHeightWrong()			   { currentState->eventHeightWrong(*this); }

    void eventItemWrong()			   { currentState->eventItemWrong(*this); }
    void eventItemCorrect()			   { currentState->eventItemCorrect(*this); }
    void eventUpdate()				   { currentState->eventUpdate(*this); }

    void eventTimerExpired()			   { currentState->eventTimerExpired(*this); }

    void eventError()                  { currentState->eventError(*this); }
};


#endif /* SRC_FSMS_OPERATIONFSM_OPERATIONCONTEXT_H_ */
