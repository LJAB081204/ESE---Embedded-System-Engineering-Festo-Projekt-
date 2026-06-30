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
	if(ctx.round == 2){
		ctx.calTimer->start(5000);
	}
	ctx.hal.getActors()->motorForwards();
	std::cout << "Kalibriere Pusherzeit..." << std::endl;
}

void PusherCalibration::exit(OperationContext& ctx) {}

void PusherCalibration::eventItemAtPusher(OperationContext& ctx) {
	if(ctx.round == 2){
		ctx.data.pusherTime = ctx.calTimer->stop();
		ctx.saveCalibrationTime("timePusher", ctx.data.pusherTime);
	}
	ctx.hal.getActors()->letItemPass();
}

void PusherCalibration::eventItemLeftPusher(OperationContext& ctx) {
	exit(ctx);
	new(this) ExitCalibration;
	entry(ctx);
}

void PusherCalibration::eventButtonStartPressedLong(OperationContext& ctx){
	std::cout << "Kalibrierung abgebrochen." << std::endl;
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}

void PusherCalibration::eventButtonStartPressedLongRemote(OperationContext& ctx){
	std::cout << "Kalibrierung abgebrochen." << std::endl;
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}

void PusherCalibration::eventButtonStopPressed(OperationContext& ctx){
//	exit(ctx);
//	new (this) Service;
//	entry(ctx);
}

void PusherCalibration::eventButtonStopPressedRemote(OperationContext& ctx){
//	exit(ctx);
//	new (this) Service;
//	entry(ctx);
}
