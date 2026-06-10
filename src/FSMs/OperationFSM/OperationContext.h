/*
 * OperationContext.h
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_OPERATIONCONTEXT_H_
#define SRC_FSMS_OPERATIONFSM_OPERATIONCONTEXT_H_

#include "../../HAL.h"
#include "../../dataClass.h"
#include <new>
#include "OperationBase.h"
#include "Idle.h"
#include "Running.h"
#include "../../HeightSensor.h"

#include <iostream>

class OperationContext{
	//Kontext
	alignas(Running) char stateBuffer[sizeof(Running)];


public:
	HAL& hal;
	int connectionID = -1;
	dataClass& data;
	Running* savedRunning = nullptr;
	OperationBase* currentState = reinterpret_cast<OperationBase*>(stateBuffer);
	HistorySource historySource = HistorySource::NONE_HISTORY;

	OperationContext(HAL& hal,dataClass& data) : hal(hal),data(data){
		new (currentState) Idle;
		currentState->entry(*this);
	}
	~OperationContext(){
		delete savedRunning;
	}
	void saveHistory(HistorySource source){
		historySource = source;
		if (source == HistorySource::STOP_HISTORY){
			savedRunning = static_cast<Running*>(currentState);
			new (stateBuffer) Idle;
			currentState = reinterpret_cast<OperationBase*>(stateBuffer);
			currentState->entry(*this);
		}
	}
	void restoreHistory(){
		currentState->onRestore(historySource, *this);
		currentState->entry(*this);
		historySource = HistorySource::NONE_HISTORY;
	}

    HistorySource getHistorySource() const { return historySource; }


	void eventButtonStartPressed()     { currentState->eventButtonStartPressed(*this); }
    void eventButtonStartPressedRemote()     { currentState->eventButtonStartPressedRemote(*this); }

    void eventButtonStartPressedLong() { currentState->eventButtonStartPressedLong(*this); }
    void eventButtonStartPressedLongRemote() { currentState->eventButtonStartPressedLongRemote(*this); }

    void eventButtonStopPressed()      { currentState->eventButtonStopPressed(*this); }
    void eventButtonStopPressedRemote()      { currentState->eventButtonStopPressedRemote(*this); }

    void eventButtonResetPressed()     { currentState->eventButtonResetPressed(*this); }
    void eventButtonResetPressedRemote()     { currentState->eventButtonResetPressedRemote(*this); }

    void eventButtonEstopPressed()     { currentState->eventButtonEstopPressed(*this); }
    void eventButtonEstopPressedRemote()     { currentState->eventButtonEstopPressedRemote(*this); }

    void eventButtonEstopReleased()    { currentState->eventButtonEstopReleased(*this); }
    void eventButtonEstopReleasedRemote()    { currentState->eventButtonEstopReleasedRemote(*this); }


    void eventItemAtEntry()            { currentState->eventItemAtEntry(*this); }
    void eventItemAtEntryRemote()            { currentState->eventItemAtEntryRemote(*this); }

    void eventItemLeftEntry()          { currentState->eventItemLeftEntry(*this); }
    void eventItemLeftEntryRemote()          { currentState->eventItemLeftEntryRemote(*this); }


    void eventItemAtExit()             { currentState->eventItemAtExit(*this); }
    void eventItemAtExitRemote()             { currentState->eventItemAtExitRemote(*this); }

    void eventItemLeftExit()           { currentState->eventItemLeftExit(*this); }
    void eventItemLeftExitRemote()           { currentState->eventItemLeftExitRemote(*this); }


    void eventItemAtPusher()           { currentState->eventItemAtPusher(*this); }
    void eventItemAtPusherRemote()           { currentState->eventItemAtPusherRemote(*this); }

    void eventItemLeftPusher()         { currentState->eventItemLeftPusher(*this); }
    void eventItemLeftPusherRemote()         { currentState->eventItemLeftPusherRemote(*this); }


    void eventItemInSlide()            { currentState->eventItemInSlide(*this); }
    void eventItemInSlideRemote()            { currentState->eventItemInSlideRemote(*this); }

    void eventItemLeftSlide()          { currentState->eventItemLeftSlide(*this); }
    void eventItemLeftSlideRemote()          { currentState->eventItemLeftSlideRemote(*this); }


    void eventItemMetal()			   { currentState->eventItemMetal(*this); }
    void eventItemMetalRemote()              { currentState->eventItemMetalRemote(*this); }

    void eventItemArrived()			   { std::cout<<"WWWWWWWW" << std::endl; currentState->eventItemArrived(*this);}
    void eventItemGone()			   { currentState->eventItemGone(*this);}

    void eventFestoReady()			   { currentState->eventItemGone(*this);}
    void eventFestoNotReady()		   { currentState->eventItemGone(*this);}

    void eventHeightHigh()			   {currentState->eventHeightHigh(*this); }
    void eventHeightFlat()			   { currentState->eventHeightFlat(*this); }
    void eventHeightHole()			   { currentState->eventHeightHole(*this); }
    void eventHeightWrong()			   { currentState->eventHeightWrong(*this); }

    void eventItemWrong()			   { currentState->eventItemWrong(*this); }
    void eventItemCorrect()			   { currentState->eventItemCorrect(*this); }
    void eventUpdate()				   { currentState->eventUpdate(*this); }

    void eventTimerExpired()			   { currentState->eventTimerExpired(*this); }

    void eventError()                  { currentState->eventError(*this); }



    void eventSlideFull()                    { currentState->eventSlideFull(*this); }
    void eventSlideFullRemote()              { currentState->eventSlideFullRemote(*this); }
};


#endif /* SRC_FSMS_OPERATIONFSM_OPERATIONCONTEXT_H_ */
