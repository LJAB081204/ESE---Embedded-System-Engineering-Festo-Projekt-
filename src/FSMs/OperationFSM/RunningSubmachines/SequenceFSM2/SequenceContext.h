/*
 * SequenceContext.h
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM2_SEQUENCECONTEXT_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM2_SEQUENCECONTEXT_H_

#include <new>
#include "SequenceBase.h"
#include "IHM.h"
#include "../../../../dataClass.h"
#include "waitingForProfile.h"
#include "../../../../HAL.h"
namespace M2{

class SequenceContext{
	//Kontext
	alignas(IHM) char stateBuffer[sizeof(IHM)];
	SequenceBase* currentState = reinterpret_cast<SequenceBase*>(stateBuffer);



public:
//	HAL& hal;
	dataClass& data;
	SequenceContext(dataClass& data) : data(data){
		new (currentState) waitingForProfile;
		currentState->entry(*this);
	}

	bool oneRound = false;

	itemProfile resolveProfile() {
//			if(hal.getSensors()->isItemMetal() == true){
//				data.section2.front().metal = true;
//			}
		    if (data.section2.empty()) return UNDEFINED_PROFILE;
		    if (data.section2.front().special)                                                                                { data.section2.front().item = IS_PROFILE;  return IS_PROFILE; }
		    if (data.section2.front().flat && !data.section2.front().high && !data.section2.front().hole && !data.section2.front().metal) { data.section2.front().item = IF_PROFILE;  return IF_PROFILE; }
		    if (data.section2.front().high && data.section2.front().hole && data.section2.front().metal)                      { data.section2.front().item = IHM_PROFILE; return IHM_PROFILE; }
		    if (data.section2.front().high && data.section2.front().metal && !data.section2.front().hole)                     { data.section2.front().item = IM_PROFILE;  return IM_PROFILE; }
		    if (data.section2.front().high && data.section2.front().hole && !data.section2.front().metal)                     { data.section2.front().item = IH_PROFILE;  return IH_PROFILE; }
		    if (data.section2.front().high && !data.section2.front().hole && !data.section2.front().metal)                    { data.section2.front().item = I_PROFILE;   return I_PROFILE; }
		    data.section2.front().item = IU_PROFILE;
		    return IU_PROFILE;
		}

	void eventUpdate()      { currentState->eventUpdate(*this); }
	void eventGoBack()      { currentState->eventGoBack(*this); }
	void eventItemCorrect() { currentState->eventItemCorrect(*this); }
	void eventItemWrong()   { currentState->eventItemWrong(*this); }
	void eventItemAtPusher(){ currentState->eventItemAtPusher(*this); }
	void reactivate()       { currentState->entry(*this); }

};


}
#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SEQUENCEFSM2_SEQUENCECONTEXT_H_ */
