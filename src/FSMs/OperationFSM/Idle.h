/*
 * Idle.h
 *
 *  Created on: 28.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_IDLE_H_
#define SRC_FSMS_OPERATIONFSM_IDLE_H_

#include "OperationBase.h"

class Idle : public OperationBase{
public:
	Idle();
	void entry(OperationContext& ctx)override;
	void exit(OperationContext& ctx)override;
	void eventButtonStartPressed(OperationContext& ctx)override;
	void eventButtonStartPressedLong(OperationContext& ctx)override;
	void eventButtonEstopPressed(OperationContext& ctx)override;


};



#endif /* SRC_FSMS_OPERATIONFSM_IDLE_H_ */
