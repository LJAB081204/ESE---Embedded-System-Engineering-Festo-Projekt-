/*
 * SlowWaitForFesto.h
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_SLOWWAITFORFESTO_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_SLOWWAITFORFESTO_H_

#include "TransportBase.h"

class SlowWaitForFesto : public TransportBase{
public:
	SlowWaitForFesto();
	void entry(TransportContext& ctx)override;
	void exit(TransportContext& ctx)override;

	void eventItemLeftExitRemote(TransportContext& ctx)override;
	void eventItemInSlideRemote(TransportContext& ctx)override;
};


#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_SLOWWAITFORFESTO_H_ */
