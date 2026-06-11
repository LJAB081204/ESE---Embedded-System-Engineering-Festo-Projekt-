/*
 * EntryCalibration.cpp
 *
 *  Created on: 08.06.2026
 *      Author: olive
 */

#include <new>
#include "../OperationBase.h"
#include "../OperationContext.h"
#include "EntryCalibration.h"
#include "HeightDetect.h"

EntryCalibration::EntryCalibration(){};

void EntryCalibration::entry(OperationContext& ctx) {
	std::cout<< "1" << std::endl;
	ctx.hal.getActors()->motorForwards();
}

void EntryCalibration::exit(OperationContext& ctx) {
	//Timer setzten
}

void EntryCalibration::eventItemArrived(OperationContext& ctx){
	exit(ctx);
	new(this) HeightDetect;
	entry(ctx);
}

void EntryCalibration::eventButtonStartPressedLong(OperationContext& ctx){
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}



