/*
 * SlideContext.h
 *
 *  Created on: 08.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_SLIDECONTEXT_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_SLIDECONTEXT_H_

#include <sys/neutrino.h>
#include "../../../../dataClass.h"
#include "../../../../PulseMessages.h"
#include "../../../../HAL.h"
#include <new>
#include "SlideBase.h"
#include "Free.h"
#include "../../../../timer.h"



namespace M2{

class SlideContext{
	//Kontext
	alignas(Free) char stateBuffer[sizeof(Free)];
	SlideBase* currentState = reinterpret_cast<SlideBase*>(stateBuffer);


public:
	HAL& hal;
	dataClass& data;
	SimpleTimer* slideTimer;

	SlideContext(HAL& hal,dataClass& data) : hal(hal),data(data){
		slideTimer = new SimpleTimer(data.conId,PULSE_EVENT_SLIDE_TIMER);
		new (currentState) Free;
		currentState->entry(*this);
	}
	virtual ~SlideContext() = default;

	void eventItemLeftSlide()		{currentState->eventItemLeftSlide(*this);}
	void eventItemInSlide()			{currentState->eventItemAtSlide(*this);}
	void eventSlideTimerExpired()	{currentState->eventSlideTimerExpired(*this);}
	void eventStopSlideTimer()		{currentState->eventStopSlideTimer(*this);}

	void reactivate() 			 { currentState->entry(*this); }

};
}


#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_SLIDECONTEXT_H_ */
