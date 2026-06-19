/*
 * TransportContext2.h
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM2_TRANSPORTCONTEXT_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM2_TRANSPORTCONTEXT_H_

#include "../../../../HAL.h"
#include "../../../../dataClass.h"
#include <new>
#include "TransportBase2.h"
#include "Stop2.h"
#include "Forward2.h"

#include <iostream>
namespace M2{

class TransportContext2{
	//Kontext
	alignas(Forward2) char stateBuffer[sizeof(Forward2)];
	TransportBase2* currentState = reinterpret_cast<TransportBase2*>(stateBuffer);


public:
	HAL& hal;
	dataClass& data;
	TransportContext2(HAL& hal,dataClass& data) : hal(hal),data(data){
		new (currentState) Stop2;
		currentState->entry(*this);
	}

	void eventItemAtEntry()			 {currentState->eventItemAtEntry(*this); }
	void eventItemInSlide()          { currentState->eventItemInSlide(*this); }
	void eventItemAtEntryRemote()    { currentState->eventItemAtEntryRemote(*this); }
	void eventItemAtExit()           { currentState->eventItemAtExit(*this); }
	void eventItemLeftExit()   		 { currentState->eventItemLeftExit(*this); }
	void eventItemLeftEntry()   	{ currentState->eventItemLeftEntry(*this); }
	void eventItemLeftExitRemote()   { currentState->eventItemLeftExitRemote(*this); }
	void eventItemInSlideRemote()    { currentState->eventItemInSlideRemote(*this); }
	void eventItemGone()             { currentState->eventItemGone(*this); }
	void eventItemArrived()          { currentState->eventItemArrived(*this); }
	void eventItemAtPusher()		{ currentState->eventItemAtPusher(*this); }

	void eventFestoEmpty() 			 { currentState->eventItemArrived(*this); }

	void eventFestoReady() 			 { currentState->eventFestoReady(*this); }
	void eventFestoNotReady() 			 { currentState->eventFestoNotReady(*this); }
	void reactivate() 			 { currentState->entry(*this); }
	void eventItemTransfer(uint32_t itemCode)	{currentState-> eventItemTransfer(itemCode,*this); }


	//Festo2

	void eventItemAtSlide() { currentState->eventItemAtSlide(*this); }



};
}

#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM2_TRANSPORTCONTEXT_H_ */
