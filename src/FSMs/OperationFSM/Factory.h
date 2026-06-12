/*
 * Factory.h
 *
 *  Created on: 11.06.2026
 *      Author: tayla
 */

#pragma once
#include "RunningFactory.h"
#include "Running.h"
#include "RunningSubmachines/SequenceFSM/SequenceContext.h"

namespace M1{

class Factory : public RunningFactory{
	SequenceContext seq;
public:
	Factory(dataClass& data) : seq(data){}
	OperationBase* createRunning(HAL& hal,dataClass& data) override;
};
}


