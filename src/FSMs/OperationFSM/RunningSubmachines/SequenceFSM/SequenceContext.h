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

private:
	bool oneRound = false;
public:
	dataClass& data;
	SequenceContext(dataClass& data) : data(data){
		new (currentState) waitingForProfile;
		currentState->entry(*this);
	}

	void resolveProfile() {
	        if (item.special)                                         { data.section2.front() = IS_PROFILE;  return; }
	        if (item.flat && !item.high && !item.hole && !item.metal) { data.section2.front() = IF_PROFILE;  return; }
	        if (item.high && item.hole && item.metal)                 { data.section2.front() = IHM_PROFILE; return; }
	        if (item.high && item.metal && !item.hole)                { data.section2.front() = IM_PROFILE;  return; }
	        if (item.high && item.hole && !item.metal)                { data.section2.front() = IH_PROFILE;  return; }
	        if (item.high && !item.hole && !item.metal)               { data.section2.front() = I_PROFILE;   return; }
	        data.section2.front() = IU_PROFILE;
	    }

	virtual void eventUpdate(SequenceContext &fsm)			{currentState->eventUpdate(*this);}
	virtual void eventGoBack(SequenceContext &fsm)			{currentState->eventGoBack(*this);}
	virtual void eventItemCorrect(SequenceContext &fsm)		{currentState->eventItemCorrect(*this);}
	virtual void eventItemWrong(SequenceContext &fsm)		{currentState->eventItemWrong(*this);}

};



#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_SEQUENCECONTEXT_H_ */
