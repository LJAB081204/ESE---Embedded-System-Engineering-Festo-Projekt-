/*
 * waitForExit.cpp
 *
 *  Created on: 08.06.2026
 *      Author: olive
 */

#include <new>
#include <iostream>
#include "../OperationBase.h"
#include "../OperationContext.h"
#include "waitForExit.h"
#include "../Service.h"

#include "ExitCalibration.h"

waitForExit::waitForExit(){};

void waitForExit::entry(OperationContext& ctx) {
	std::cout<< "waitForExit" << std::endl;
}

void waitForExit::exit(OperationContext& ctx) {
	ctx.hal.getActors()->closePusher();

}

void waitForExit::eventItemAtExit(OperationContext& ctx) {
	exit(ctx);
	new(this) ExitCalibration;
	entry(ctx);
}

void waitForExit::eventButtonStartPressedLong(OperationContext& ctx){
	exit(ctx);
	new (this) Idle;
	entry(ctx);
}

void waitForExit::eventButtonStopPressed(OperationContext& ctx){
	/*exit(ctx);
	new (this) Service;
	entry(ctx);*/
}

void waitForExit::eventButtonStopPressedRemote(OperationContext& ctx){
	/*exit(ctx);
	new (this) Service;
	entry(ctx);*/
}
