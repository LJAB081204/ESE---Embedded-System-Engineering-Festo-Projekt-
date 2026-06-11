/*
 * SlideContext.h
 *
 *  Created on: 08.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_SLIDECONTEXT_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_SLIDECONTEXT_H_

#include "../../../../dataClass.h"
#include "../../../../HAL.h"
#include <new>
#include "SlideBase.h"
#include "Free.h"

namespace M1{

class SlideContext{
	//Kontext
	alignas(Free) char stateBuffer[sizeof(Free)];
	SlideBase* currentState = reinterpret_cast<SlideBase*>(stateBuffer);


public:
	HAL& hal;
	dataClass& data;
	SlideContext(HAL& hal,dataClass& data) : hal(hal),data(data){
		new (currentState) Free;
		currentState->entry(*this);
	}

	virtual void eventItemLeftSlide(SlideContext &fsm)		{currentState->eventItemLeftSlide(*this);}
	virtual void eventItemAtSlide(SlideContext &fsm)		{currentState->eventItemAtSlide(*this);}
	virtual void eventSlideTimerExpired(SlideContext &fsm)	{currentState->eventSlideTimerExpired(*this);}
	virtual void eventStopSlideTimer(SlideContext &fsm)		{currentState->eventStopSlideTimer(*this);}

};
}


#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_SLIDECONTEXT_H_ */
