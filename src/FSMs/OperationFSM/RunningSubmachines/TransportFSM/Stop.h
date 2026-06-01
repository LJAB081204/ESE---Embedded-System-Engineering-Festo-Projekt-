/*
 * Stop.h
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_STOP_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_STOP_H_

#include "TransportBase.h"

class Stop : public TransportBase{
public:
	Stop();
	void entry(TransportContext& ctx)override;
	void exit(TransportContext& ctx)override;

	void eventItemAtEntry(TransportContext& ctx)override;
};



#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM_STOP_H_ */
