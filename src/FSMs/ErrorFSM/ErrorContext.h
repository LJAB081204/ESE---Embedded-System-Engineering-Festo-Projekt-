/*
 * ErrorContext.h
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_ERRORFSM_ERRORCONTEXT_H_
#define SRC_FSMS_ERRORFSM_ERRORCONTEXT_H_

#include <new>
#include "../../HAL.h"
#include "ErrorBase.h"
#include "../../dataClass.h"
#include "currentNAck.h"

class ErrorContext {
	alignas(currentNAck) char stateBuffer[sizeof(currentNAck)];
	ErrorBase* currentState = reinterpret_cast<ErrorBase*>(stateBuffer);


public:
	HAL& hal;
	dataClass& data;
	bool active = true;
	bool onlyPusher = false;
	ErrorContext(HAL& hal,dataClass& data) : hal(hal),data(data){
		new (currentState) currentNAck;
		currentState->entry(*this);

	}

	void eventButtonResetPressed() { currentState->eventButtonResetPressed(*this); }
	void eventItemGone() { currentState->eventItemGone(*this); }
	void eventItemLeftSlide() { currentState->eventItemLeftSlide(*this); }
	void eventItemLeftSlideRemote() { currentState->eventItemLeftSlideRemote(*this); }
	void eventItemLeftPusher() { currentState->eventItemLeftPusher(*this); }
	void eventItemLeftExit() { currentState->eventItemLeftExit(*this); }
	void eventConnectionRestored() { currentState->eventConnectionRestored(*this); }
	void errorPusherTimerExpired() { currentState->errorPusherTimerExpired(*this); }
	void eventButtonStartPressed() { currentState->eventButtonStartPressed(*this); }
	bool isActive() const { return active; }
};




#endif /* SRC_FSMS_ERRORFSM_ERRORCONTEXT_H_ */
