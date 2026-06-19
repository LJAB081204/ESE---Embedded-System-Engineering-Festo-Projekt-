/*
 * LetItemPass.h
 *
 *  Created on: 02.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_LETITEMPASS_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_LETITEMPASS_H_

#include "WeicheBase.h"

namespace M1{

class LetItemPass : public WeicheBase{
public:
	LetItemPass();
	void entry(WeicheContext& ctx) override;
	void exit(WeicheContext& ctx) override;

	void eventItemLeftPusher(WeicheContext& ctx)override;


};
}


#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_LETITEMPASS_H_ */
