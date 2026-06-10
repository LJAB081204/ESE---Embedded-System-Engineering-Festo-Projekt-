/*
 * waitForPusher.cpp
 *
 *  Created on: 08.06.2026
 *      Author: olive
 */

#include <new>
#include <iostream>
#include "../OperationBase.h"
#include "../OperationContext.h"
#include "waitForPusher.h"

#include "PusherCalibration.h"

waitForPusher::waitForPusher(){};

void waitForPusher::entry(OperationContext& ctx) {
	std::cout<< "3" << std::endl;
	ctx.hal.getActors()->motorForwards();
}

void waitForPusher::exit(OperationContext& ctx) {}

void waitForPusher::eventItemAtPusher(OperationContext& ctx) {
	exit(ctx);
	new(this) PusherCalibration;
	entry(ctx);
}

void waitForPusher::eventButtonStartPressedLong(OperationContext& ctx){
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}
