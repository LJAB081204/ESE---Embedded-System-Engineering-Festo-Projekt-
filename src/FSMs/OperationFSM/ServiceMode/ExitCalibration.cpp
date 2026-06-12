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

ExitCalibration::ExitCalibration(){};

void ExitCalibration::entry(OperationContext& ctx) {
	ctx.hal.getActors()->motorStop();
	std::cout<< "ExitCalibration" << std::endl;
}

void ExitCalibration::exit(OperationContext& ctx) {
	//timer setzten
}

void ExitCalibration::eventItemLeftExit(OperationContext& ctx) {
	exit(ctx);
	new(this) Service;
	entry(ctx);
}


void ExitCalibration::eventButtonStartPressedLong(OperationContext& ctx){
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}

void ExitCalibration::eventButtonStopPressed(OperationContext& ctx){
	exit(ctx);
	new (this) Service;
	entry(ctx);
}

void ExitCalibration::eventButtonStopPressedRemote(OperationContext& ctx){
	exit(ctx);
	new (this) Service;
	entry(ctx);
}



