/*
 * Receive.h
 *
 *  Created on: 01.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_RECEIVE_H_
#define SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_RECEIVE_H_

#include "WeicheBase.h"
namespace M1{

class Receive : public WeicheBase{
public:
	Receive();
	void entry(WeicheContext& ctx)override;
	void exit(WeicheContext& ctx)override;
	void eventItemAtPusher(WeicheContext& ctx)override;
	void eventItemMetal(WeicheContext& ctx)override;
	void raiseUpdate(WeicheContext& ctx);
	void eventTimerExpired(WeicheContext& ctx) override;

};
}

#endif /* SRC_FSMS_OPERATIONFSM_RUNNINGSUBMACHINES_WEICHEFSM_RECEIVE_H_ */
