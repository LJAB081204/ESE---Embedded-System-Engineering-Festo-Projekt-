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

	bool oneRound = false;

	void resolveProfile() {
	    if (data.section2.front().special)                                                                                { data.section2.front().item = IS_PROFILE;  return; }
	    if (data.section2.front().flat && !data.section2.front().high && !data.section2.front().hole && !data.section2.front().metal) { data.section2.front().item = IF_PROFILE;  return; }
	    if (data.section2.front().high && data.section2.front().hole && data.section2.front().metal)                      { data.section2.front().item = IHM_PROFILE; return; }
	    if (data.section2.front().high && data.section2.front().metal && !data.section2.front().hole)                     { data.section2.front().item = IM_PROFILE;  return; }
	    if (data.section2.front().high && data.section2.front().hole && !data.section2.front().metal)                     { data.section2.front().item = IH_PROFILE;  return; }
	    if (data.section2.front().high && !data.section2.front().hole && !data.section2.front().metal)                    { data.section2.front().item = I_PROFILE;   return; }
	    data.section2.front().item = IU_PROFILE;
	}

	void eventUpdate()      { currentState->eventUpdate(*this); }
	void eventGoBack()      { currentState->eventGoBack(*this); }
	void eventItemCorrect() { currentState->eventItemCorrect(*this); }
	void eventItemWrong()   { currentState->eventItemWrong(*this); }

};



#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM_SEQUENCECONTEXT_H_ */
