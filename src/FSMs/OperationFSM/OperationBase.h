/*
 * Operation.h
 *
 *  Created on: 27.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_OPERATIONBASE_H_
#define SRC_FSMS_OPERATIONFSM_OPERATIONBASE_H_

class OperationContext; // Vorwärtsdekleration

class OperationBase{
public:
	virtual ~OperationBase() = default;
	virtual void entry(OperationContext& fsm){}
	virtual void exit(OperationContext& fsm){}

	virtual void eventButtonStartPressed(OperationContext& fsm) {}
	virtual void eventButtonStartPressedLong(OperationContext& fsm){}
	virtual void eventButtonStopPressed(OperationContext& fsm)  {}
	virtual void eventButtonResetPressed(OperationContext& fsm) {}
	virtual void eventButtonEstopPressed(OperationContext& fsm) {}
	virtual void eventButtonEstopReleased(OperationContext& fsm) {}

	virtual void eventItemAtEntry(OperationContext& fsm) {}
	virtual void eventItemLeftEntry(OperationContext& fsm) {}

	virtual void eventItemAtExit(OperationContext& fsm) {}
	virtual void eventItemLeftExit(OperationContext& fsm) {}

	virtual void eventItemAtPusher(OperationContext& fsm) {}
	virtual void eventItemLeftPusher(OperationContext& fsm) {}

	virtual void eventItemInSlide(OperationContext& fsm) {}
	virtual void eventItemLeftSlide(OperationContext& fsm) {}

	virtual void eventItemMetal(OperationContext& fsm) {}

	virtual void eventItemGone (OperationContext& fsm){}

	virtual void eventHeightHigh(OperationContext& fsm){}
	virtual void eventHeightFlat(OperationContext& fsm){}
	virtual void eventHeightHole(OperationContext& fsm){}
	virtual void eventHeightWrong(OperationContext& fsm){}

	virtual void eventItemWrong(OperationContext& fsm){}
	virtual void eventItemCorrect(OperationContext& fsm){}
	virtual void eventUpdate(OperationContext& fsm){}

	virtual void eventTimerExpired(OperationContext& fsm){}
	virtual void eventError(OperationContext& fsm) {}
};




#endif /* SRC_FSMS_OPERATIONFSM_OPERATIONBASE_H_ */
