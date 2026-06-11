/*
 * Ready.h
 *
 *  Created on: 01.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_READY_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_READY_H_

#include "WeicheBase.h"
namespace M1{

class Ready : public WeicheBase{
public:
	Ready();
	void eventItemWrong(WeicheContext& ctx)override;
	void eventItemCorrect(WeicheContext& ctx)override;
};


}
#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_READY_H_ */
