/*
 * Stop2.h
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM2_STOP_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM2_STOP_H_

#include "TransportBase2.h"
namespace M2{

class Stop2 : public TransportBase2{
public:
	Stop2();
	void entry(TransportContext2& ctx)override;
	void exit(TransportContext2& ctx)override;

	void eventItemLeftExit(TransportContext2& ctx)override;
	void eventItemLeftExitRemote(TransportContext2& ctx)override;
};

}

#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_TRANSPORTFSM2_STOP_H_ */
