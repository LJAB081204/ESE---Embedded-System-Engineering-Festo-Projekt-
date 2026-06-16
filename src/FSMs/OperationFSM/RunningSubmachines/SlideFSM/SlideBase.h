/*
 * SlideBase.h
 *
 *  Created on: 08.06.2026
 *      Author: lewis
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_SLIDEBASE_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_SLIDEBASE_H_
namespace M1{

class SlideContext;

class SlideBase{

public:
	virtual ~SlideBase() = default;

	virtual void eventItemLeftSlide(SlideContext& fsm)		{}
	virtual void eventItemAtSlide(SlideContext& fsm)			{}
	virtual void eventSlideTimerExpired(SlideContext& fsm)	{}
	virtual void eventStopSlideTimer(SlideContext& fsm)		{}

	virtual void entry(SlideContext &fsm){}
	virtual void exit(SlideContext &fsm) {}
};

}

#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_SLIDEFSM_SLIDEBASE_H_ */
