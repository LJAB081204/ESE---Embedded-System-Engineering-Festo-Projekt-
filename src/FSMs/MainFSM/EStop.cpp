/*
 * EStop.cpp
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */
#include "MainContext.h"
#include "EStop.h"
#include "Operation.h"
#include "ConnectionLost.h"


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
        ctx.data.eStopWasPressed = false;
        new(this) Operation(ctx.getOperationFSM());
        ctx.getOperationFSM().restoreHistory();
    }
}
void EStop::eventButtonResetPressedRemote(MainContext& ctx) {
    eStopFSM.eventButtonResetPressedRemote();
    if(!eStopFSM.isActive()) {
        exit(ctx);
        ctx.data.eStopWasPressed = false;
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

void EStop::enterHandshakeEStopIsPressed(MainContext& ctx) {
	eStopFSM.eventGoToHandshakeEStopIsPressed();
}

void EStop::enterHandShakeEStopWasNotPressed(MainContext& ctx) {
	eStopFSM.eventGoToHandshakeEStopWasNotPressed();
}

void EStop::enterHandShakeEStopWasPressed(MainContext& ctx) {
	eStopFSM.eventGoToHandshakeEStopWasPressed();
}

void EStop::eventResponseEStopIsPressed(MainContext& ctx) {
	eStopFSM.eventResponseEStopIsPressed();
}

void EStop::eventResponseEStopIsNotPressed(MainContext& ctx) {
	eStopFSM.eventResponseEStopIsNotPressed();
}

void EStop::eventResponseEStopWasPressed(MainContext& ctx) {
	eStopFSM.eventResponseEStopWasPressed();
}

void EStop::eventResponseEStopWasNotPressed(MainContext& ctx) {
	eStopFSM.eventResponseEStopWasNotPressed();
	if(!eStopFSM.isActive()) { // KI
	        exit(ctx);
	        ctx.data.eStopWasPressed = false;
	        new(this) Operation(ctx.getOperationFSM());
	        ctx.getOperationFSM().restoreHistory();
	    }
}

void EStop::eventConnectionLost(MainContext& ctx) {
	exit(ctx);
	if(ctx.data.eStopIsPressed){
		new(this) ConnectionLost(ctx.hal, ctx.hs, PreviousFsmState::ACTIVE_E_STOP);
	} else {
		new(this) ConnectionLost(ctx.hal, ctx.hs, PreviousFsmState::PASSIVE_E_STOP);
	}
	entry(ctx);
}

