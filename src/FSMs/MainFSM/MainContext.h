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
#include "../../HeightSensor.h"
#include "../OperationFSM/RunningFactory.h"
#include "../../timer.h"
#include <iostream>



class MainContext {
	OperationContext* operationFSM;
    alignas(EStop) char stateBuffer[sizeof(EStop)];
    MainBase* currentState = reinterpret_cast<MainBase*>(stateBuffer);
public:
    HAL& hal;
    dataClass& data;
    HeightSensor& hs;
    RunningFactory& factory;
    TimerHandler& timerHandler;
    MainContext(HAL& hal,dataClass& data,HeightSensor& hs,RunningFactory& factory,TimerHandler& timerHandler)
    : hal(hal),data(data), hs(hs), factory(factory),timerHandler(timerHandler) {
    	operationFSM = new OperationContext(hal,data,hs,factory,timerHandler);
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

        void eventTransportTimerExpired()          {currentState->eventTransportTimerExpired(*this); }

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
        void eventItemTransfer(uint32_t itemCode) 			{currentState->eventItemTransfer(itemCode,*this);}
        void eventSlideTimerExpired()              { currentState->eventSlideTimerExpired(*this); }
        void eventGoBack()						{currentState->eventGoBack(*this);}

        void eventGoBackWFP()						{currentState->eventGoBackWFP(*this);}

        void eventGoBackIH()						{currentState->eventGoBackIH(*this);}
        void eventGoBackIM()						{currentState->eventGoBackIM(*this);}
        void eventGoBackIF()						{currentState->eventGoBackIF(*this);}
        void eventGoBackIHM()						{currentState->eventGoBackIHM(*this);}
        void eventConnectionLost()						{currentState->eventConnectionLost(*this);}
        void eventConnectionRestored()						{currentState->eventConnectionRestored(*this);}

        void errorPusherTimerExpired() 				{currentState->errorPusherTimerExpired(*this);}
        void eventErrorResolved()					{currentState->eventErrorResolved(*this);}

        void eventResponseEStopIsPressed()					{currentState->eventResponseEStopIsPressed(*this);}
        void eventResponseEStopIsNotPressed()					{currentState->eventResponseEStopIsNotPressed(*this);}
        void eventResponseEStopWasPressed()					{currentState->eventResponseEStopWasPressed(*this);}
        void eventResponseEStopWasNotPressed()					{currentState->eventResponseEStopWasNotPressed(*this);}

    };



#endif /* SRC_FSMS_MAINFSM_MAINCONTEXT_H_ */
