/*
 * ForwardSlow2.h
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_FORWARDSLOW_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_FORWARDSLOW_H_

#include "TransportBase2.h"
namespace M2{

class ForwardSlow2 : public TransportBase2{
public:
	ForwardSlow2();
	void entry(TransportContext2& ctx)override;
	void exit(TransportContext2& ctx)override;

	void eventItemGone(TransportContext2& ctx)override;
};

}

#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_FORWARDSLOW_H_ */
