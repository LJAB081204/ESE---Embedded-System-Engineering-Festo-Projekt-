/*
 * PusherCalibration.cpp
 *
 *  Created on: 08.06.2026
 *      Author: olive
 */

#include <new>
#include <iostream>
#include "../OperationBase.h"
#include "../OperationContext.h"
#include "PusherCalibration.h"
#include "../Service.h"

#include "waitForExit.h"

#include "ExitCalibration.h"

PusherCalibration::PusherCalibration(){};

void PusherCalibration::entry(OperationContext& ctx) {
	ctx.calTimer->start(5000);
	ctx.hal.getActors()->motorForwards();
	std::cout<< "PusherCalibration" << std::endl;
}

void PusherCalibration::exit(OperationContext& ctx) {
	int pusher = ctx.calTimer->stop();
	std::cout<< pusher << std::endl;
}

void PusherCalibration::eventItemAtPusher(OperationContext& ctx) {
	exit(ctx);
	ctx.hal.getActors()->letItemPass();
	new(this) ExitCalibration;
	entry(ctx);
}

void PusherCalibration::eventButtonStartPressedLong(OperationContext& ctx){
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}

void PusherCalibration::eventButtonStopPressed(OperationContext& ctx){
	exit(ctx);
	new (this) Service;
	entry(ctx);
}

void PusherCalibration::eventButtonStopPressedRemote(OperationContext& ctx){
	exit(ctx);
	new (this) Service;
	entry(ctx);
}
