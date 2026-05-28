/*
 * OperationFSM.cpp
 *
 *  Created on: 27.05.2026
 *      Author: tayla
 */

#include "OperationState.h"

class OperationFSM{
	OperationState* currentState;
public:
	void setState(OperationState* newState){
		currentState = newState;
	}
	void eventButtonStartPressed(){
		currentState->eventButtonStartPressed(*this);
	}
	void eventButtonStartPressedLong(){
		currentState->eventButtonStartPressedLong(*this);
	}
	void eventButtonStopPressed(){
		currentState->eventButtonStopPressed(*this);
	}
	void eventButtonEstopPressed(){
		currentState->eventButtonEstopPressed(*this);
	}
	void eventButtonResetPressed(){
		currentState->eventButtonResetPressed(*this);
	}

};


