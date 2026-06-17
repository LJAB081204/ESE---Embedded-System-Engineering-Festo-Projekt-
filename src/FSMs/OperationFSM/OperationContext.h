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
#include "RunningSubmachines/SequenceFSM/SequenceContext.h"
#include <iostream>
#include "../../HeightSensor.h"
#include "RunningFactory.h"
#include "../../PulseMessages.h"
#include "../../timer.h"


class OperationContext{
	//Kontext
	alignas(Idle) char stateBuffer[sizeof(Idle)];


public:
	HAL& hal;
	int connectionID = -1;
	dataClass& data;
	HeightSensor& hs;
	RunningFactory& factory;
	TimerHandler& timerHandler;
	OperationBase* savedRunning = nullptr;
	OperationBase* currentState = reinterpret_cast<OperationBase*>(stateBuffer);
	HistorySource historySource = HistorySource::NONE_HISTORY;
	SimpleTimer* calTimer;
	int round;


	OperationContext(HAL& hal,dataClass& data, HeightSensor& hs,RunningFactory& factory,TimerHandler& timerHandler)
	: hal(hal),data(data),hs(hs),factory(factory),timerHandler(timerHandler){
		calTimer = new SimpleTimer(0,0);
		new (currentState) Idle;
		currentState->entry(*this);
		round = 0;
	}
	~OperationContext(){
		delete savedRunning;
	}
	void saveHistory(HistorySource source){
		historySource = source;
		if (source == HistorySource::STOP_HISTORY){
			savedRunning = currentState;
			new (stateBuffer) Idle;
			currentState = reinterpret_cast<OperationBase*>(stateBuffer);
			currentState->entry(*this);
		}
		else if (source == HistorySource::ESTOP_HISTORY){
			if (currentState != reinterpret_cast<OperationBase*>(stateBuffer)){
				currentState->exit(*this);
				delete currentState;
			}
			if (savedRunning != nullptr){
				savedRunning->exit(*this);
				delete savedRunning;
				savedRunning = nullptr;
			}
			new (stateBuffer) Idle;
			currentState = reinterpret_cast<OperationBase*>(stateBuffer);
		}


	}
	void restoreHistory(){
		currentState->onRestore(historySource, *this);
		currentState->entry(*this);
		historySource = HistorySource::NONE_HISTORY;
	}

	void saveCalibrationTime(const std::string& key, uint32_t value) {
	    std::vector<std::string> lines;
	    std::ifstream in(CALIBRATION_FILE);
	    std::string line;
	    bool found = false;

	    while (std::getline(in, line)) {
	        if (line.find(key + "=") == 0) {
	            lines.push_back(key + "=" + std::to_string(value));
	            found = true;
	        } else {
	            lines.push_back(line);
	        }
	    }
	    in.close();

	    if (!found) {
	        lines.push_back(key + "=" + std::to_string(value));
	    }

	    std::ofstream out(CALIBRATION_FILE);
	    for (const auto& l : lines) {
	        out << l << "\n";
	    }
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

    void eventItemArrived()			   { currentState->eventItemArrived(*this);}
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
    void eventItemTransfer(uint32_t itemCode)				{ currentState->eventItemTransfer(itemCode,*this);}
    void eventSlideTimerExpired()              { currentState->eventSlideTimerExpired(*this); }

    void eventGoBack()						{currentState->eventGoBack(*this);}

    void eventGoBackWFP()						{currentState->eventGoBack(*this);}
    void eventGoBackIH()						{currentState->eventGoBack(*this);}
    void eventGoBackIM()						{currentState->eventGoBack(*this);}
    void eventGoBackIF()						{currentState->eventGoBack(*this);}
    void eventGoBackIHM()						{currentState->eventGoBack(*this);}

};



#endif /* SRC_FSMS_OPERATIONFSM_OPERATIONCONTEXT_H_ */
