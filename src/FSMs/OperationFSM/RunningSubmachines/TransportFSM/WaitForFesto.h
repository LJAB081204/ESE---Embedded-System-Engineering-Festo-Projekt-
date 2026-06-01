/*
 * WaitForFesto.h
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_WAITFORFESTO_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_WAITFORFESTO_H_

#include "TransportBase.h"

class WaitForFesto : public TransportBase{
public:
	WaitForFesto();
	void entry(TransportContext& ctx)override;
	void exit(TransportContext& ctx)override;

	void eventItemLeftExitRemote(TransportContext& ctx)override;
	void eventItemAtSlideRemote(TransportContext& ctx)override;
};



#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_WAITFORFESTO_H_ */
