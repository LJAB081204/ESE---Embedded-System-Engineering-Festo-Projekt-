/*
 * Wait.h
 *
 *  Created on: 02.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM2_WAIT_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM2_WAIT_H_


#include "WeicheBase.h"
#include "../../../../timer.h"

namespace M2{

class Wait : public WeicheBase{
public:
	Wait();
	void entry(WeicheContext& ctx)override;
	void exit(WeicheContext& ctx) override;

	void eventTimerExpired(WeicheContext& ctx) override;
};
}

#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM2_WAIT_H_ */
