/*
 * WeicheContext.cpp
 *
 *  Created on: 01.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM2_WEICHECONTEXT_CPP_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM2_WEICHECONTEXT_CPP_

#include <new>
#include "../../../../HAL.h"
#include "../../../../timer.h"
#include "../../../../dataClass.h"
#include "WeicheBase.h"
#include "Receive.h"
namespace M2{

class WeicheContext{
	alignas(Receive) char stateBuffer[sizeof(Receive)];
	WeicheBase* currentState = reinterpret_cast<WeicheBase*>(stateBuffer);
public:
	HAL& hal;
	dataClass& data;
	TimerHandler& timerHandler;
	WeicheContext(HAL& hal,dataClass& data,TimerHandler& timerHandler)
	: hal(hal), data(data),timerHandler(timerHandler) {
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


};
}


#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM2_WEICHECONTEXT_CPP_ */
