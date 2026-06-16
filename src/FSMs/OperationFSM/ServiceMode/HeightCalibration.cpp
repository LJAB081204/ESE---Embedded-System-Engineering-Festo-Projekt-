/*
 * HeightCalibration.cpp
 *
 *  Created on: 08.06.2026
 *      Author: olive
 */

#include <new>
#include <iostream>
#include "../OperationBase.h"
#include "../OperationContext.h"
#include "../Service.h"
#include "HeightCalibration.h"
#include "PusherCalibration.h"

HeightCalibration::HeightCalibration(){};

void HeightCalibration::entry(OperationContext& ctx) {
	ctx.calTimer->start(10000);
	ctx.hal.getActors()->motorSlow();
	std::cout<< "HeightCalibration" << std::endl;
}

void HeightCalibration::exit(OperationContext& ctx) {
	uint32_t HeightCalibration = ctx.calTimer->stop();
	ctx.saveCalibrationTime("timeHeight", HeightCalibration);
}

void HeightCalibration::eventItemGone(OperationContext& ctx) {
	exit(ctx);
	new(this) PusherCalibration;
	entry(ctx);
}

void HeightCalibration::eventButtonStartPressedLong(OperationContext& ctx){
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}

void HeightCalibration::eventButtonStopPressed(OperationContext& ctx){
	exit(ctx);
	new (this) Service;
	entry(ctx);
}

void HeightCalibration::eventButtonStopPressedRemote(OperationContext& ctx){
	exit(ctx);
	new (this) Service;
	entry(ctx);
}
