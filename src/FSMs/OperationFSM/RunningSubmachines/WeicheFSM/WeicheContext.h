/*
 * WeicheContext.cpp
 *
 *  Created on: 01.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_WEICHECONTEXT_CPP_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_WEICHECONTEXT_CPP_

#include <new>
#include "../../../../HAL.h"
#include "../../../../timer.h"
#include "../../../../dataClass.h"
#include "WeicheBase.h"
#include "Receive.h"
#include "Wait.h"
namespace M1{

class WeicheContext{
	alignas(Wait) char stateBuffer[sizeof(Wait)];
	WeicheBase* currentState = reinterpret_cast<WeicheBase*>(stateBuffer);
public:
	HAL& hal;
	dataClass& data;
	DynamicTimer pusherTimer;
	WeicheContext(HAL& hal,dataClass& data,TimerHandler& timerHandler)
	: hal(hal), data(data),pusherTimer(100,timerHandler) {
		new(currentState) Receive;
		currentState->entry(*this);
	}

    void eventItemAtPusher()			   { currentState->eventItemAtPusher(*this); }
    void eventItemLeftPusher() 		   { currentState->eventItemLeftPusher(*this);}

    void eventItemInSlide()			   { currentState->eventItemInSlide(*this);}
    void eventItemLeftSlide()			   { currentState->eventItemLeftSlide(*this);}


    void eventItemMetal()			   { currentState->eventItemMetal(*this); }

    void eventItemWrong() 		   { currentState->eventItemWrong(*this);}
    void eventItemCorrect()			   { currentState->eventItemCorrect(*this);}
    void eventUpdate()			   { currentState->eventUpdate(*this);}
    void eventError()			   { currentState->eventError(*this);}
    void eventTimerExpired()	   { currentState->eventTimerExpired(*this);}



};
}


#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_WEICHECONTEXT_CPP_ */
