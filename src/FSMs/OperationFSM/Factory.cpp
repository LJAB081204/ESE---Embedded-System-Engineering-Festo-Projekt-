/*
 * Factory.cpp
 *
 *  Created on: 11.06.2026
 *      Author: tayla
 */

#include "Factory.h"

namespace M1{
	OperationBase* Factory::createRunning(HAL& hal,dataClass& data,TimerHandler& timerHandler){
		return new Running(hal,data,seq,timerHandler);
	}

}


