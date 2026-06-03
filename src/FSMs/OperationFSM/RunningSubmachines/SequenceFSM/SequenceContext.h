/*
 * SequenceContext.h
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_SEQUENCECONTEXT_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_SEQUENCECONTEXT_H_

#include <new>
#include "SequenceBase.h"
#include "IHM.h"
#include "../../../../dataClass.h"
#include "waitingForProfile.h"

class SequenceContext{
	//Kontext
	alignas(IHM) char stateBuffer[sizeof(IHM)];
	SequenceBase* currentState = reinterpret_cast<SequenceBase*>(stateBuffer);


public:
	dataClass& data;
	SequenceContext(dataClass& data) : data(data){
		new (currentState) waitingForProfile;
		currentState->entry(*this);
	}

	virtual void eventUpdate(SequenceContext &fsm)			{currentState->eventUpdate(*this);}
	virtual void eventItemCorrect(SequenceContext &fsm)		{currentState->eventItemCorrect(*this);}
	virtual void eventItemWrong(SequenceContext &fsm)		{currentState->eventItemWrong(*this);}

};



#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_SEQUENCECONTEXT_H_ */
