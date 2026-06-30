/*
 * ExitCalibration.cpp
 *
 *  Created on: 08.06.2026
 *      Author: olive
 */

#include <new>
#include <iostream>
#include "../OperationBase.h"
#include "../OperationContext.h"
#include "ExitCalibration.h"

#include "../Service.h"
#include "Calibration.h"

ExitCalibration::ExitCalibration(){};

void ExitCalibration::entry(OperationContext& ctx) {
	if(ctx.round == 2){
		ctx.calTimer->start(4999);
	}
	std::cout<< "ExitCalibration" << std::endl;
}

void ExitCalibration::exit(OperationContext& ctx) {}

void ExitCalibration::eventItemAtExit(OperationContext& ctx) {
	if(ctx.round == 2){
		ctx.data.exitTime = ctx.calTimer->stop();
		ctx.saveCalibrationTime("timeExit", ctx.data.exitTime);
	}
	ctx.hal.getActors()->motorStop();
	ctx.hal.getActors()->closePusher();
}

void ExitCalibration::eventItemLeftExit(OperationContext& ctx) {
	exit(ctx);
	new(this) Calibration;
	entry(ctx);
}

void ExitCalibration::eventButtonStartPressedLong(OperationContext& ctx){
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}

void ExitCalibration::eventButtonStartPressedLongRemote(OperationContext& ctx){
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}

void ExitCalibration::eventButtonStopPressed(OperationContext& ctx){
//	exit(ctx);
//	ctx.hs.stopCalibration();
//	new (this) Service;
//	entry(ctx);
}

void ExitCalibration::eventButtonStopPressedRemote(OperationContext& ctx){
//	exit(ctx);
//	ctx.hs.stopCalibration();
//	new (this) Service;
//	entry(ctx);
}



