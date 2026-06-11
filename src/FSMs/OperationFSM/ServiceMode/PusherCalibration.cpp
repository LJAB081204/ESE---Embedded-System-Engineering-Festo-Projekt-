/*
 * PusherCalibration.cpp
 *
 *  Created on: 08.06.2026
 *      Author: olive
 */

#include <new>
#include <iostream>
#include "../OperationBase.h"
#include "../OperationContext.h"
#include "PusherCalibration.h"
#include "../Service.h"

#include "waitForExit.h"

#include "ExitCalibration.h"

PusherCalibration::PusherCalibration(){};

void PusherCalibration::entry(OperationContext& ctx) {
	std::cout<< "PusherCalibration" << std::endl;
	ctx.hal.getActors()->openPusher();
}

void PusherCalibration::exit(OperationContext& ctx) {
	//timer setzten
}

void PusherCalibration::eventItemLeftPusher(OperationContext& ctx) {
	exit(ctx);
	//timer starten
	new(this) waitForExit;
	entry(ctx);
}

void PusherCalibration::eventButtonStartPressedLong(OperationContext& ctx){
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}

void PusherCalibration::eventButtonStopPressed(OperationContext& ctx){
	exit(ctx);
	new (this) Service;
	entry(ctx);
}
