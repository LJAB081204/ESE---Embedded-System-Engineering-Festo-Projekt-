/*
 * OperationContext.h
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_OPERATIONCONTEXT_H_
#define SRC_FSMS_OPERATIONFSM_OPERATIONCONTEXT_H_

#include "../../HAL.h"
#include <new>
#include "OperationBase.h"
#include "Idle.h"
#include "Running.h"

class OperationContext{
	//Kontext
	alignas(Running) char stateBuffer[sizeof(Running)];
	OperationBase* currentState = reinterpret_cast<OperationBase*>(stateBuffer);

public:
	HAL& hal;
	OperationContext(HAL& hal) : hal(hal){
		new (currentState) Idle;
		currentState->entry(*this);
	}
    void eventButtonStartPressed();
    void eventButtonStartPressedLong();
    void eventButtonStopPressed();
    void eventButtonEstopPressed();
    void eventButtonResetPressed();
};


#endif /* SRC_FSMS_OPERATIONFSM_OPERATIONCONTEXT_H_ */
