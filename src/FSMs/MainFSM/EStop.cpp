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
#include <iostream>
using namespace std;


void EStop::entry(MainContext& ctx) {
    ctx.hal.getActors()->motorStop();
    ctx.hal.getActors()->greenLampOff();
    ctx.hal.getActors()->redLampOn();
    ctx.hal.getActors()->ledButtonStartOff();
}
void EStop::exit(MainContext& ctx) {
	ctx.hal.getActors()->redLampOff();
	setBackSequences(ctx);
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
        if(ctx.data.returnToError){
        	ctx.data.returnToError = false;
        	new (this) Error(ctx.hal,ctx.data);
        	entry(ctx);
        }
        else{
			new(this) Operation(ctx.getOperationFSM());
			ctx.getOperationFSM().restoreHistory();
        }
    }
}
void EStop::eventButtonResetPressedRemote(MainContext& ctx) {
    eStopFSM.eventButtonResetPressedRemote();
    if(!eStopFSM.isActive()) {
        exit(ctx);
        ctx.data.eStopWasPressed = false;
        if(ctx.data.returnToError){
			ctx.data.returnToError = false;
			new (this) Error(ctx.hal,ctx.data);
			entry(ctx);
		}
        else{
			new(this) Operation(ctx.getOperationFSM());
			ctx.getOperationFSM().restoreHistory();
        }
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
    ctx.data.reset();
    ctx.getOperationFSM().saveHistory(HistorySource::ESTOP_HISTORY);
	eStopFSM.eventResponseEStopIsPressed();
}

void EStop::eventResponseEStopIsNotPressed(MainContext& ctx) {
	eStopFSM.eventResponseEStopIsNotPressed();
}

void EStop::eventResponseEStopWasPressed(MainContext& ctx) {
    ctx.data.reset();
    ctx.getOperationFSM().saveHistory(HistorySource::ESTOP_HISTORY);
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

void EStop::setBackSequences(MainContext& ctx){
	if(!ctx.data.isMachine1){

	}
}
