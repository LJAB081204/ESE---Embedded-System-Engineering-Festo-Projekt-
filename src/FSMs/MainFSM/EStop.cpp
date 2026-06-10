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
    ctx.hal.getActors()->ledButtonStartOff();
}
void EStop::exit(MainContext& ctx) {
	ctx.hal.getActors()->redLampOff();
}

void EStop::eventButtonEstopPressed(MainContext& ctx) {
    eStopFSM.eventButtonEstopPressed();
}
void EStop::eventButtonEstopReleased(MainContext& ctx) {
    eStopFSM.eventButtonEstopReleased();
}
void EStop::eventButtonResetPressed(MainContext& ctx) {
    eStopFSM.eventButtonResetPressed();
    if(!eStopFSM.isActive()) {
        exit(ctx);
        new(this) Operation(ctx.getOperationFSM());
        ctx.getOperationFSM().restoreHistory();
    }
}
void EStop::eventButtonResetPressedRemote(MainContext& ctx) {
    eStopFSM.eventButtonResetPressedRemote();
    if(!eStopFSM.isActive()) {
        exit(ctx);
        new(this) Operation(ctx.getOperationFSM());
        ctx.getOperationFSM().restoreHistory();
    }
}

void EStop::eventButtonEstopPressedRemote(MainContext& ctx) {
    eStopFSM.eventButtonEstopPressedRemote();
}
void EStop::eventButtonEstopReleasedRemote(MainContext& ctx) {
    eStopFSM.eventButtonEstopReleasedRemote();
}


