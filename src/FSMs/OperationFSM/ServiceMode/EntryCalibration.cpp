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
#include "../Service.h"

EntryCalibration::EntryCalibration(){};

void EntryCalibration::entry(OperationContext& ctx) {
	ctx.hal.getActors()->motorForwards();
	std::cout<< "EntryCalibration" << std::endl;
}

void EntryCalibration::exit(OperationContext& ctx) {
	int entry = ctx.calTimer->stop();
	std::cout<< entry << std::endl;
}

void EntryCalibration::eventItemLeftEntry(OperationContext& ctx) {
	ctx.calTimer->start(5000);
	std::cout << "kommt an" << std::endl;
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


