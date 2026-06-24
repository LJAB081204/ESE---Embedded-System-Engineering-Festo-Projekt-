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
	void eventButtonStartPressedRemote(OperationContext& ctx)override;

	void eventButtonStartPressedLong(OperationContext& ctx)override;
	void eventButtonStartPressedLongRemote(OperationContext& ctx)override;
	void eventGoBack(OperationContext& ctx)override;
	void eventGoBackIH(OperationContext& ctx)override;
	void eventGoBackIM(OperationContext& ctx)override;
	void eventGoBackIF(OperationContext& ctx)override;
	void eventGoBackIHM(OperationContext& ctx)override;


};



#endif /* SRC_FSMS_OPERATIONFSM_IDLE_H_ */
