/*
 * Running.h
 *
 *  Created on: 28.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNING_H_
#define SRC_FSMS_OPERATIONFSM_RUNNING_H_

#include "OperationBase.h"

class Running : public OperationBase{
public:
	Running();
	void eventButtonStopPressed(OperationContext& ctx)override;
};


#endif /* SRC_FSMS_OPERATIONFSM_RUNNING_H_ */
