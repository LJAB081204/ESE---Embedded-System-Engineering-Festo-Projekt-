/*
 * WeicheBase.h
 *
 *  Created on: 01.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_WEICHEBASE_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_WEICHEBASE_H_
namespace M1{

class WeicheContext;

class WeicheBase{
public:
	virtual ~WeicheBase() = default;

	virtual void entry(WeicheContext& fsm) {}
	virtual void exit(WeicheContext& fsm)  {}

	virtual void eventItemAtPusher(WeicheContext& fsm)  {}
	virtual void eventItemLeftPusher(WeicheContext& fsm){}

	virtual void eventItemInSlide(WeicheContext& fsm)   {}
	virtual void eventItemLeftSlide(WeicheContext& fsm) {}

	virtual void eventItemMetal(WeicheContext& fsm){}

	virtual void eventItemWrong(WeicheContext& fsm){}
	virtual void eventItemCorrect(WeicheContext& fsm){}
	virtual void eventUpdate(WeicheContext& fsm){}

	virtual void eventTimerExpired(WeicheContext& fsm){}

	virtual void eventError(WeicheContext& fsm) {}

	virtual void reactivate(WeicheContext& fsm) {}

	virtual void eventSkipUpdate(WeicheContext& ctx){}


};

}

#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_WEICHEBASE_H_ */
