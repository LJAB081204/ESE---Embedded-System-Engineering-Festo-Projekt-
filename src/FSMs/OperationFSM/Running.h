/*
 * Running.h
 *
 *  Created on: 28.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNING_H_
#define SRC_FSMS_OPERATIONFSM_RUNNING_H_

#include "OperationBase.h"
#include "../../dataClass.h"


class TransportContext;
class WeicheContext;
class SequenceContext;
class ProfileContext;

class Running : public OperationBase{
	TransportContext* transportFSM;
	WeicheContext* weicheFSM;
	SequenceContext* sequenceFSM;
	ProfileContext* profileFSM;
public:
	Running(HAL& hal,dataClass& data);
	void entry(OperationContext& ctx)override;
	void exit(OperationContext& ctx)override;
	void eventButtonStopPressed(OperationContext& ctx)override;
	void eventItemAtEntry(OperationContext& ctx)override;
	void eventItemAtExit(OperationContext& ctx)override;
	void eventItemInSlide(OperationContext& ctx)override;
	void eventItemLeftSlide(OperationContext& ctx)override;
	void eventItemGone(OperationContext& ctx)override;
	void eventItemArrived(OperationContext& ctx)override;
	void eventItemMetal(OperationContext& ctx)override;
	void eventItemAtPusher(OperationContext& ctx)override;
	void eventItemLeftPusher(OperationContext& ctx)override;
	void eventItemWrong(OperationContext& ctx)override;
	void eventItemCorrect(OperationContext& ctx)override;
	void eventUpdate(OperationContext& ctx)override;
	void eventTimerExpired(OperationContext& ctx)override;
	void eventError(OperationContext& ctx)override;

	//Profile
	void eventHeightHigh(OperationContext& ctx)override;
	void eventHeightFlat(OperationContext& ctx)override;
	void eventHeightHole(OperationContext& ctx)override;
	void eventHeightWrong(OperationContext& ctx)override;




};


#endif /* SRC_FSMS_OPERATIONFSM_RUNNING_H_ */
