/*
 * Factory.h
 *
 *  Created on: 11.06.2026
 *      Author: tayla
 */

#pragma once
#include "RunningFactory.h"
#include "Running2.h"
#include "RunningSubmachines/SequenceFSM2/SequenceContext.h"

namespace M2{

class Factory2 : public RunningFactory{
	SequenceContext seq;
public:
	Factory2(dataClass& data) : seq(data){}
	OperationBase* createRunning(HAL& hal,dataClass& data,TimerHandler& timerHandler) override;
};
}


