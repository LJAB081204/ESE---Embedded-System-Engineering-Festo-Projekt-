/*
 * Running.h
 *
 *  Created on: 28.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNING_H_
#define SRC_FSMS_OPERATIONFSM_RUNNING_H_

#include "OperationBase.h"

class TransportContext;

class Running : public OperationBase{
	TransportContext* transportFSM;
	WeicheContext* weicheFSM;
	//SequenceContext* sequenceFSM;
	//ProfileContext* profileFSM;
public:
	Running(HAL& hal);
	void entry(OperationContext& ctx)override;
	void exit(OperationContext& ctx)override;
	void eventButtonStopPressed(OperationContext& ctx)override;
	void eventItemAtEntry(OperationContext& ctx)override;
	void eventItemAtExit(OperationContext& ctx)override;
	void eventItemInSlide(OperationContext& ctx)override;
	void eventItemGone(OperationContext& ctx)override;
	void eventItemArrived(OperationContext& ctx)override;
};


#endif /* SRC_FSMS_OPERATIONFSM_RUNNING_H_ */
