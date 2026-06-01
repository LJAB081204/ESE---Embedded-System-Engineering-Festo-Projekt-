/*
 * EStop.cpp
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */
#include "MainContext.h"
#include "EStop.h"
#include "Operation.h"

void EStop::entry(MainContext& ctx) {
    ctx.hal.getActors()->motorStop();
    ctx.hal.getActors()->greenLampOff();
    ctx.hal.getActors()->redLampOn();
}
void EStop::exit(MainContext& ctx) {}

void EStop::eventButtonResetPressed(MainContext& ctx) {
	if(!ctx.hal.getSensors()->isEStopTriggered()){
		exit(ctx);
		new(this) Operation(ctx.hal);
		entry(ctx);
	}
}
