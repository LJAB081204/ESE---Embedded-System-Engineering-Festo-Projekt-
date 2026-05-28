/*
 * Operation.h
 *
 *  Created on: 27.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_OPERATIONSTATE_H_
#define SRC_FSMS_OPERATIONFSM_OPERATIONSTATE_H_

//class OperationFSM; // Vorwärtsdekleration

class OperationState{
public:
	virtual ~OperationState();
	/*
	virtual void eventButtonStartPressed(OperationFSM& fsm) {}
	virtual void eventButtonStartPressedLong(OperationFSM& fsm){}
	virtual void eventButtonStopPressed(OperationFSM& fsm)  {}
	virtual void eventButtonResetPressed(OperationFSM& fsm) {}
	virtual void eventButtonEstopPressed(OperationFSM& fsm) {}
	*/
};




#endif /* SRC_FSMS_OPERATIONFSM_OPERATIONSTATE_H_ */
