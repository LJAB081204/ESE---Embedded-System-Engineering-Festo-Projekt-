/*
 * HeightCalibration.h
 *
 *  Created on: 11.06.2026
 *      Author: olive
 */

#ifndef SRC_FSMS_OPERATIONFSM_SERVICEMODE_HEIGHTCALIBRATION_H_
#define SRC_FSMS_OPERATIONFSM_SERVICEMODE_HEIGHTCALIBRATION_H_

#include "../OperationBase.h"

class HeightCalibration : public OperationBase{
public:
	HeightCalibration();
	void entry(OperationContext& ctx)override;
	void exit(OperationContext& ctx)override;
	void eventButtonStartPressedLong(OperationContext& ctx)override;
	void eventButtonStartPressedLongRemote(OperationContext& ctx)override;
	void eventItemAtEntry(OperationContext& ctx)override;
	void eventButtonStartPressed(OperationContext& ctx)override;
	void eventButtonStartPressedRemote(OperationContext& ctx)override;
	void eventItemArrived(OperationContext& ctx)override;
	void eventItemGone(OperationContext& ctx)override;
	void eventItemAtPusher(OperationContext& ctx)override;
	void eventItemAtExit(OperationContext& ctx)override;
};


#endif /* SRC_FSMS_OPERATIONFSM_SERVICEMODE_HEIGHTCALIBRATION_H_ */
