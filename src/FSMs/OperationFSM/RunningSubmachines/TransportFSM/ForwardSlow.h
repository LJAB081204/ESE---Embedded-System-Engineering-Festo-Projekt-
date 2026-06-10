/*
 * ForwardSlow.h
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_FORWARDSLOW_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_FORWARDSLOW_H_

#include "TransportBase.h"

class ForwardSlow : public TransportBase{
public:
	ForwardSlow();
	void entry(TransportContext& ctx)override;
	void exit(TransportContext& ctx)override;

	void eventItemGone(TransportContext& ctx)override;
	void eventItemAtExit(TransportContext& ctx)override;
	void eventItemAtEntry(TransportContext& ctx)override;
	void eventItemInSlide(TransportContext& ctx)override;
};



#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_FORWARDSLOW_H_ */
