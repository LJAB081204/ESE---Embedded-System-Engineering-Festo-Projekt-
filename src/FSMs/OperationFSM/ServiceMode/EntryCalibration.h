/*
 * EntryCalibration.h
 *
 *  Created on: 08.06.2026
 *      Author: olive
 */

#ifndef SRC_FSMS_OPERATIONFSM_SERVICEMODE_ENTRYCALIBRATION_H_
#define SRC_FSMS_OPERATIONFSM_SERVICEMODE_ENTRYCALIBRATION_H_

#include "../OperationBase.h"

class EntryCalibration : public OperationBase{
public:
	EntryCalibration();
	void entry(OperationContext& ctx) override;
	void exit(OperationContext& ctx) override;
//	void eventHeightHigh(OperationContext& ctx)override;
//	void eventHeightFlat(OperationContext& ctx)override;
	void eventItemArrived(OperationContext& ctx)override;
	void eventButtonStartPressedLong(OperationContext& ctx)override;

};


#endif /* SRC_FSMS_OPERATIONFSM_SERVICEMODE_ENTRYCALIBRATION_H_ */
