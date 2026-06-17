/*
 * Error.cpp
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */
#include "Error.h"
#include "MainContext.h"

Error::Error(HAL& hal, dataClass& data) : errorFSM(hal, data) {}

void Error::entry(MainContext& ctx) {
    ctx.hal.getActors()->motorStop();
    ctx.hal.getActors()->closePusher();
    ctx.hal.getActors()->greenLampOff();
    ctx.hal.getActors()->redLampBlink(100);
    ctx.hal.getActors()->ledButtonStartOff();
}
void Error::exit(MainContext& ctx) {
	ctx.hal.getActors()->redLampBlinkOff();
}



