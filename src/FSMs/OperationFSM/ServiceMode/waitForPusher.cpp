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
#include "../Service.h"

#include "PusherCalibration.h"

waitForPusher::waitForPusher(){};

void waitForPusher::entry(OperationContext& ctx) {
	ctx.hal.getActors()->motorForwards();
	std::cout<< "waitForPusher" << std::endl;
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

void waitForPusher::eventButtonStopPressed(OperationContext& ctx){
	exit(ctx);
	new (this) Service;
	entry(ctx);
}

void waitForPusher::eventButtonStopPressedRemote(OperationContext& ctx){
	exit(ctx);
	new (this) Service;
	entry(ctx);
}
