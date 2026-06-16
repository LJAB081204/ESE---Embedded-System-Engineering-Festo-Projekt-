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
	ctx.hal.getActors()->motorForwards();
	std::cout<< "EntryCalibration" << std::endl;
}

void EntryCalibration::exit(OperationContext& ctx) {
	uint32_t entryTime = ctx.calTimer->stop();
	ctx.saveCalibrationTime("timeEntry", entryTime);
}

void EntryCalibration::eventItemLeftEntry(OperationContext& ctx) {
	ctx.calTimer->start(5000);
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
	exit(ctx);
	new (this) Service;
	entry(ctx);
}

void EntryCalibration::eventButtonStopPressedRemote(OperationContext& ctx){
	exit(ctx);
	new (this) Service;
	entry(ctx);
}


