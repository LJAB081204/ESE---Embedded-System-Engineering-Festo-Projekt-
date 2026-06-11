/*
 * ExitCalibration.h
 *
 *  Created on: 08.06.2026
 *      Author: olive
 */

#ifndef SRC_FSMS_OPERATIONFSM_SERVICEMODE_EXITCALIBRATION_H_
#define SRC_FSMS_OPERATIONFSM_SERVICEMODE_EXITCALIBRATION_H_

#include "../OperationBase.h"

class ExitCalibration : public OperationBase{
public:
	ExitCalibration();
	void entry(OperationContext& ctx) override;
	void exit(OperationContext& ctx) override;
	void eventItemLeftExit(OperationContext& ctx)override;
	void eventButtonStartPressedLong(OperationContext& ctx)override;
	void eventButtonStopPressed(OperationContext& ctx)override;
	void eventButtonStopPressedRemote(OperationContext& ctx)override;

};


#endif /* SRC_FSMS_OPERATIONFSM_SERVICEMODE_EXITCALIBRATION_H_ */
