/*
 * Factory.cpp
 *
 *  Created on: 11.06.2026
 *      Author: tayla
 */

#include "Factory2.h"

namespace M2{
	OperationBase* Factory2::createRunning(HAL& hal,dataClass& data){
		return new Running2(hal,data,seq);
	}

}


