/*
 * OperationContext.cpp
 *
 *  Created on: 27.05.2026
 *      Author: tayla
 */
#include "OperationContext.h"
#include "OperationBase.h"
#include "Idle.h"
#include "Running.h"

	void OperationContext::eventButtonStartPressed(){
		currentState->eventButtonStartPressed(*this);
	}
	void OperationContext::eventButtonStartPressedLong(){
		currentState->eventButtonStartPressedLong(*this);
	}
	void OperationContext::eventButtonStopPressed(){
		currentState->eventButtonStopPressed(*this);
	}
	void OperationContext::eventButtonEstopPressed(){
		currentState->eventButtonEstopPressed(*this);
	}
	void OperationContext::eventButtonResetPressed(){
		currentState->eventButtonResetPressed(*this);
	}






