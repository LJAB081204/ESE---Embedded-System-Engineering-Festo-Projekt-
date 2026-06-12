/*
 * ProfileContext.h
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_PROFILECONTEXT_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_PROFILECONTEXT_H_

#include "../../../../HAL.h"
#include "../../../../dataClass.h"
#include <new>
#include "ProfileBase.h"
#include "Default.h"
namespace M1{

class ProfileContext {
    alignas(Default) char stateBuffer[sizeof(Default)];
    ProfileBase* currentState = reinterpret_cast<ProfileBase*>(stateBuffer);
public:

    dataClass& data;

    ProfileContext(dataClass& data) : data(data){
        new(currentState) Default;
        currentState->entry(*this);
    }
    	void eventItemGone()			   { currentState->eventItemGone(*this);}

        void eventHeightHigh()			   { currentState->eventHeightHigh(*this); }
        void eventHeightFlat()			   { currentState->eventHeightFlat(*this); }
        void eventHeightHole()			   { currentState->eventHeightHole(*this); }
        void eventHeightWrong()			   { currentState->eventHeightWrong(*this); }

        void eventError()                  { currentState->eventError(*this); }
    };
}


#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_PROFILEFSM_PROFILECONTEXT_H_ */
