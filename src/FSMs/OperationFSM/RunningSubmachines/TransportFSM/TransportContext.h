/*
 * TransportContext.h
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_TRANSPORTCONTEXT_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_TRANSPORTCONTEXT_H_

#include "../../../../HAL.h"
#include "../../../../dataClass.h"
#include <new>
#include "TransportBase.h"
#include "Stop.h"
#include "../../Running.h"
#include "Forward.h"

#include <iostream>

class TransportContext{
	//Kontext
	alignas(Forward) char stateBuffer[sizeof(Forward)];
	TransportBase* currentState = reinterpret_cast<TransportBase*>(stateBuffer);


public:
	HAL& hal;
	dataClass& data;
	TransportContext(HAL& hal,dataClass& data) : hal(hal),data(data){
		new (currentState) Stop;
		currentState->entry(*this);
	}

	void eventItemAtEntry()			 {currentState->eventItemAtEntry(*this); }
	void eventItemInSlide()          { currentState->eventItemInSlide(*this); }
	void eventItemAtEntryRemote()    { currentState->eventItemAtEntryRemote(*this); }
	void eventItemAtExit()           { currentState->eventItemAtExit(*this); }
	void eventItemLeftExit()   		 { currentState->eventItemLeftExit(*this); }
	void eventItemLeftExitRemote()   { currentState->eventItemLeftExitRemote(*this); }
	void eventItemInSlideRemote()    { currentState->eventItemInSlideRemote(*this); }
	void eventItemGone()             { currentState->eventItemGone(*this); }
	void eventItemArrived()          { currentState->eventItemArrived(*this); }

};

#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_TRANSPORTCONTEXT_H_ */
