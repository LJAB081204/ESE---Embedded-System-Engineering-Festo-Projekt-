/*
 * OperationBase.cpp
 *
 *  Created on: 27.05.2026
 *      Author: tayla
 */
#include <new>
#include "OperationBase.h"
#include "Idle.h"

class OperationContext{
	//Kontext
	alignas(Idle) char stateBuffer[sizeof(Idle)];
	OperationBase* currentState = reinterpret_cast<OperationBase*>(stateBuffer);

public:
	OperationContext(){
		new (currentState) Idle;
		currentState->entry(*this);
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


