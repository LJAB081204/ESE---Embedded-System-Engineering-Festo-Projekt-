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
	std::cout<< "6" << std::endl;
	ctx.hal.getActors()->motorStop();
}

void ExitCalibration::exit(OperationContext& ctx) {
	//timer setzten
}

void ExitCalibration::eventItemAtExit(OperationContext& ctx) {
	exit(ctx);
	new(this) Service;
	entry(ctx);
}

void ExitCalibration::eventButtonStartPressedLong(OperationContext& ctx){
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}



