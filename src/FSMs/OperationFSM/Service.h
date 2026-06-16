/*
 * Service.h
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_SERVICE_H_
#define SRC_FSMS_OPERATIONFSM_SERVICE_H_

#include "OperationBase.h"

class Service : public OperationBase{
public:
	Service();
	void entry(OperationContext& ctx)override;
	void exit(OperationContext& ctx)override;
	void eventButtonStartPressedLong(OperationContext& ctx)override;
	void eventButtonStartPressedLongRemote(OperationContext& ctx)override;
	void eventItemAtEntry(OperationContext& ctx)override;
	void eventButtonResetPressed(OperationContext& ctx)override;
	void eventButtonResetPressedRemote(OperationContext& ctx)override;
	void eventItemArrived(OperationContext& ctx)override;
	void eventItemGone(OperationContext& ctx)override;
	void eventItemAtPusher(OperationContext& ctx)override;
	void eventItemAtExit(OperationContext& ctx)override;
};

#endif /* SRC_FSMS_OPERATIONFSM_SERVICE_H_ */
