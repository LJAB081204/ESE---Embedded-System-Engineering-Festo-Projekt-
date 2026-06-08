/*
 * waitForExit.h
 *
 *  Created on: 08.06.2026
 *      Author: olive
 */

#ifndef SRC_FSMS_OPERATIONFSM_SERVICEMODE_WAITFOREXIT_H_
#define SRC_FSMS_OPERATIONFSM_SERVICEMODE_WAITFOREXIT_H_

#include "../OperationBase.h"

class waitForExit : public OperationBase{
public:
	waitForExit();
	void entry(OperationContext& ctx) override;
	void exit(OperationContext& ctx) override;
	void eventItemAtExit(OperationContext& ctx)override;
	void eventButtonStartPressedLong(OperationContext& ctx)override;

};


#endif /* SRC_FSMS_OPERATIONFSM_SERVICEMODE_WAITFOREXIT_H_ */
