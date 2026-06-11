/*
 * HeightDetect.h
 *
 *  Created on: 08.06.2026
 *      Author: olive
 */

#ifndef SRC_FSMS_OPERATIONFSM_SERVICEMODE_HEIGHTDETECT_H_
#define SRC_FSMS_OPERATIONFSM_SERVICEMODE_HEIGHTDETECT_H_

#include "../OperationBase.h"

class HeightDetect : public OperationBase{
public:
	HeightDetect();
	void entry(OperationContext& ctx) override;
	void exit(OperationContext& ctx) override;
	void eventItemGone(OperationContext& ctx)override;
	void eventButtonStartPressedLong(OperationContext& ctx)override;
	void eventButtonStopPressed(OperationContext& ctx)override;

};


#endif /* SRC_FSMS_OPERATIONFSM_SERVICEMODE_HEIGHTDETECT_H_ */
