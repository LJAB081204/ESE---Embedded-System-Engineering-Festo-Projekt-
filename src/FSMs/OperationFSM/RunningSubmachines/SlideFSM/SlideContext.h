/*
 * SlideContext.h
 *
 *  Created on: 08.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_SLIDECONTEXT_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_SLIDECONTEXT_H_


class SlideContext{
	//Kontext
	alignas(Free) char stateBuffer[sizeof(Free)];
	SlideBase* currentState = reinterpret_cast<SlideBase*>(stateBuffer);


public:
	HAL& hal;
	dataClass& data;
	SlideContext(HAL& hal,dataClass& data) : hal(hal),data(data){
		new (currentState) Free;
		currentState->entry(*this);
	}

	void eventItemLeftSlide()		{currentState->eventItemLeftSide(*this); }
	void

};



#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_SLIDECONTEXT_H_ */
