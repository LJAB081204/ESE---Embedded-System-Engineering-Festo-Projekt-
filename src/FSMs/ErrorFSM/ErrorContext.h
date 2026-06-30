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
#include "currentAck.h"
#include "goneNAck.h"
#include "waitingForStart.h"
#include "waitingForStart2.h"
#include "errorPusherTooLong.h"
#include "waitingForResolve.h"

class ErrorContext {
	alignas(currentNAck) char stateBuffer[sizeof(currentNAck)];
	ErrorBase* currentState = reinterpret_cast<ErrorBase*>(stateBuffer);


public:
	HAL& hal;
	dataClass& data;
	bool active = true;
	bool onlyPusher = false;
	bool resetPressed = false;
	ErrorContext(HAL& hal,dataClass& data) : hal(hal),data(data){
		if(!data.isErrorLocal){
			new (currentState) waitingForResolve;
		}
		else{
			switch(data.errorState){
			case CURRENT_NACK_STATE 	  : new (currentState) currentNAck; break;
			case CURRENT_ACK_STATE   	  : new (currentState) currentAck; break;
			case GONE_NACK_STATE    	  : new (currentState) goneNAck; break;
			case WAITING_FOR_START_STATE  : new (currentState) waitingForStart; break;
			case WAITING_FOR_START_STATE2 : new (currentState) waitingForStart2; break;
			case PUSHER_TOO_LONG_STATE 	  : new (currentState) errorPusherTooLong; break;
			}
		}
		currentState->entry(*this);

	}

	void eventButtonResetPressed() { currentState->eventButtonResetPressed(*this); }
	void eventButtonResetPressedRemote() { currentState->eventButtonResetPressedRemote(*this); }
	void eventButtonStartPressedRemote() { currentState->eventButtonStartPressedRemote(*this);}
	void eventItemGone() { currentState->eventItemGone(*this); }
	void eventItemLeftSlide() { currentState->eventItemLeftSlide(*this); }
	void eventItemLeftSlideRemote() { currentState->eventItemLeftSlideRemote(*this); }
	void eventItemLeftPusher() { currentState->eventItemLeftPusher(*this); }
	void eventItemLeftExit() { currentState->eventItemLeftExit(*this); }
	void eventConnectionRestored() { currentState->eventConnectionRestored(*this); }
	void errorPusherTimerExpired() { currentState->errorPusherTimerExpired(*this); }
	void eventButtonStartPressed() { currentState->eventButtonStartPressed(*this); }
	void eventItemLeftEntry() 	   {currentState->eventItemLeftEntry(*this);}
	void eventItemArrived() 	   {currentState->eventItemArrived(*this);}
	void eventItemAtExit() 	   {currentState->eventItemAtExit(*this);}
	void eventItemAtPusher() 	   {currentState->eventItemAtPusher(*this);}
	bool isActive() const { return active; }
	void setActive(bool b) {active = b;}

};




#endif /* SRC_FSMS_ERRORFSM_ERRORCONTEXT_H_ */
