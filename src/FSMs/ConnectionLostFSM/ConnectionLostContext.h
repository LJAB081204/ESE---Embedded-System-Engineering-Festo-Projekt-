/*
 * ConnectionLostContext.h
 *
 *  Created on: 17.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_CONNECTIONLOSTFSM_CONNECTIONLOSTCONTEXT_H_
#define SRC_FSMS_CONNECTIONLOSTFSM_CONNECTIONLOSTCONTEXT_H_

#include "ConnectionLostBase.h"
#include "ConnectionLostNormal.h"
#include "ConnectionLostEStopIsPressed.h"
#include "ConnectionLostEStopWasPressed.h"
#include "../../HAL.h"
#include "../../HeightSensor.h"


enum class PreviousFsmState {ACTIVE_E_STOP, PASSIVE_E_STOP, OPERATION, ERROR};
enum class EStopButtonState {ISPRESSED, WASPRESSED, WASNOTPRESSED};

class ConnectionLostContext {
	alignas(ConnectionLostNormal) char stateBuffer[sizeof(ConnectionLostNormal)];
	ConnectionLostBase* currentState = reinterpret_cast<ConnectionLostBase*>(stateBuffer);

public:
	EStopButtonState estopState;
	HAL& hal;
	HeightSensor& hs;
	bool wasPulled = false;
	ConnectionLostContext(HAL& hal, HeightSensor& hs, PreviousFsmState prevState) : hal(hal), hs(hs) {
		if(prevState == PreviousFsmState::ACTIVE_E_STOP){
			new(currentState) ConnectionLostEStopIsPressed;
			estopState = EStopButtonState::ISPRESSED;
		} else if (prevState == PreviousFsmState::PASSIVE_E_STOP){
			new(currentState) ConnectionLostEStopWasPressed;
			estopState = EStopButtonState::WASPRESSED;
		} else {
			new(currentState) ConnectionLostNormal;
			estopState = EStopButtonState::WASNOTPRESSED;
		}
		currentState->entry(*this);
	}

	void eventButtonEstopPressed()    { currentState->eventButtonEstopPressed(*this); }
	void eventButtonEstopReleased()   { currentState->eventButtonEstopReleased(*this); }
	void eventConnectionLost()        { currentState->eventConnectionLost(*this); }
	void eventConnectionRestored()    { currentState->eventConnectionRestored(*this); }
};



#endif /* SRC_FSMS_CONNECTIONLOSTFSM_CONNECTIONLOSTCONTEXT_H_ */
