/*
 * HeightDetect.cpp
 *
 *  Created on: 08.06.2026
 *      Author: olive
 */

#include <new>
#include <iostream>
#include "../OperationBase.h"
#include "../OperationContext.h"
#include "HeightDetect.h"
#include "../Service.h"

#include "PusherCalibration.h"

HeightDetect::HeightDetect(){};

void HeightDetect::entry(OperationContext& ctx) {
	ctx.calTimer->start(10000);
	ctx.hal.getActors()->motorSlow();
	std::cout<< "HeightDetect" << std::endl;
}

void HeightDetect::exit(OperationContext& ctx) {
	int heightDetect = ctx.calTimer->stop();
	std::cout<< heightDetect << std::endl;
}

void HeightDetect::eventItemGone(OperationContext& ctx) {
	exit(ctx);
	new(this) PusherCalibration;
	entry(ctx);
}

void HeightDetect::eventButtonStartPressedLong(OperationContext& ctx){
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}

void HeightDetect::eventButtonStopPressed(OperationContext& ctx){
	exit(ctx);
	new (this) Service;
	entry(ctx);
}

void HeightDetect::eventButtonStopPressedRemote(OperationContext& ctx){
	exit(ctx);
	new (this) Service;
	entry(ctx);
}
