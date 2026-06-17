/*
 * HeightCalibration.h
 *
 *  Created on: 11.06.2026
 *      Author: olive
 */

#ifndef SRC_FSMS_OPERATIONFSM_SERVICEMODE_CALIBRATION_H_
#define SRC_FSMS_OPERATIONFSM_SERVICEMODE_CALIBRATION_H_

#include "../OperationBase.h"

class Calibration : public OperationBase{
public:
	Calibration();
	void entry(OperationContext& ctx)override;
	void exit(OperationContext& ctx)override;
	void eventButtonStartPressedLong(OperationContext& ctx)override;
	void eventButtonStartPressedLongRemote(OperationContext& ctx)override;
	void eventItemAtEntry(OperationContext& ctx)override;
	void eventButtonStopPressed(OperationContext& ctx)override;
	void eventButtonStopPressedRemote(OperationContext& ctx)override;

};


#endif /* SRC_FSMS_OPERATIONFSM_SERVICEMODE_CALIBRATION_H_ */
