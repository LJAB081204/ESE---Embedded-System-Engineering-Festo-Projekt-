/*
 * MainContext.h
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_MAINFSM_MAINCONTEXT_H_
#define SRC_FSMS_MAINFSM_MAINCONTEXT_H_

#include <new>
#include "../../HAL.h"
#include "MainBase.h"
#include "Operation.h"
#include "EStop.h"
#include "Error.h"
#include "../../dataClass.h"



class MainContext {
	OperationContext* operationFSM;
    alignas(EStop) char stateBuffer[sizeof(EStop)];
    MainBase* currentState = reinterpret_cast<MainBase*>(stateBuffer);
public:
    HAL& hal;
    dataClass& data;
    MainContext(HAL& hal,dataClass& data) : hal(hal),data(data) {
    	operationFSM = new OperationContext(hal,data);
        new(currentState) Operation(*operationFSM);
        currentState->entry(*this);
    }
    ~MainContext(){
    	delete operationFSM;
    }

    	OperationContext& getOperationFSM() { return *operationFSM; }

    	void eventButtonStartPressed()     { currentState->eventButtonStartPressed(*this); }
        void eventButtonStartPressedRemote()     { currentState->eventButtonStartPressedRemote(*this); }

        void eventButtonStartPressedLong() { currentState->eventButtonStartPressedLong(*this); }
        void eventButtonStartPressedLongRemote() { currentState->eventButtonStartPressedLongRemote(*this); }

        void eventButtonStopPressed()      { currentState->eventButtonStopPressed(*this); }
        void eventButtonStopPressedRemote()      { currentState->eventButtonStopPressedRemote(*this); }

        void eventButtonResetPressed()     { currentState->eventButtonResetPressed(*this); }
        void eventButtonResetPressedRemote()     { currentState->eventButtonResetPressedRemote(*this); }

        void eventButtonEstopPressed()     { currentState->eventButtonEstopPressed(*this); }
        void eventButtonEstopPressedRemote(){ currentState->eventButtonEstopPressedRemote(*this); }

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

        void eventItemArrived() 		   { currentState->eventItemArrived(*this);}
        void eventItemGone()			   { currentState->eventItemGone(*this);}

        void eventFestoReady()			   { currentState->eventFestoReady(*this);}
        void eventFestoNotReady()		   { currentState->eventFestoNotReady(*this);}

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



#endif /* SRC_FSMS_MAINFSM_MAINCONTEXT_H_ */
