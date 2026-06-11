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

#include "waitForPusher.h"

HeightDetect::HeightDetect(){};

void HeightDetect::entry(OperationContext& ctx) {
	std::cout<< "HeightDetect" << std::endl;
	ctx.hal.getActors()->motorSlow();
}

void HeightDetect::exit(OperationContext& ctx) {
	//Timer setzten
}

void HeightDetect::eventItemGone(OperationContext& ctx) {
	exit(ctx);
	new(this) waitForPusher;
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
