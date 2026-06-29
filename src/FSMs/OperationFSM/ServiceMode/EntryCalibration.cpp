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
#include "HeightCalibration.h"
#include "../Service.h"

EntryCalibration::EntryCalibration(){};

void EntryCalibration::entry(OperationContext& ctx) {
	ctx.round++;
	if(ctx.round == 2){
		ctx.data.calibrated = true;
		ctx.hal.getActors()->motorForwards();
		ctx.hal.getActors()->motorSlow();
		std::cout<< "EntryCalibrationSlow" << std::endl;
	}
	else{
		ctx.hal.getActors()->motorForwards();
		std::cout<< "EntryCalibration" << std::endl;
	}

}

void EntryCalibration::exit(OperationContext& ctx) {
	if(ctx.round == 1){
		ctx.data.entryTime = ctx.calTimer->stop();
	}
	else if(ctx.round == 2){
		ctx.data.entryTimeSlow = ctx.calTimer->stop();
	}
}

void EntryCalibration::eventItemLeftEntry(OperationContext& ctx) {
	if(ctx.round == 1 || ctx.round == 2){
		ctx.calTimer->start(10000);
	}
}

void EntryCalibration::eventItemArrived(OperationContext& ctx){
	exit(ctx);
	new(this) HeightCalibration;
	entry(ctx);
}

void EntryCalibration::eventButtonStartPressedLong(OperationContext& ctx){
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}

void EntryCalibration::eventButtonStartPressedLongRemote(OperationContext& ctx){
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}

void EntryCalibration::eventButtonStopPressed(OperationContext& ctx){
	/*exit(ctx);
	new (this) Service;
	entry(ctx);*/
}

void EntryCalibration::eventButtonStopPressedRemote(OperationContext& ctx){
	/*exit(ctx);
	new (this) Service;
	entry(ctx);*/
}


