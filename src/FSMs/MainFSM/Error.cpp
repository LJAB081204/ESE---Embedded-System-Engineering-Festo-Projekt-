/*
 * Error.cpp
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */
#include "Error.h"
#include "MainContext.h"
#include "ConnectionLost.h"

Error::Error(HAL& hal, dataClass& data) : errorFSM(hal, data) {}

void Error::entry(MainContext& ctx) {
    ctx.hal.getActors()->motorStop();
    ctx.hal.getActors()->greenLampOff();
    ctx.hal.getActors()->redLampBlink(1000);
    ctx.hal.getActors()->ledButtonStartOff();
}
void Error::exit(MainContext& ctx) {
	ctx.hal.getActors()->redLampBlinkOff();
}
void Error::eventButtonResetPressed(MainContext& ctx) {
	errorFSM.eventButtonResetPressed();
}
void Error::eventItemGone(MainContext& ctx) {
	errorFSM.eventItemGone();
}
void Error::eventItemLeftSlide(MainContext& ctx) {
	errorFSM.eventItemLeftSlide();
}
void Error::eventItemLeftSlideRemote(MainContext& ctx) {
	errorFSM.eventItemLeftSlideRemote();
}
void Error::eventItemLeftPusher(MainContext& ctx) {
	errorFSM.eventItemLeftPusher();
}
void Error::eventItemLeftExit(MainContext& ctx) {
	errorFSM.eventItemLeftExit();
}
void Error::eventConnectionRestored(MainContext& ctx) {
	errorFSM.eventConnectionRestored();
}
void Error::errorPusherTimerExpired(MainContext& ctx) {
	errorFSM.errorPusherTimerExpired();
}
void Error::eventButtonStartPressed(MainContext& ctx) {
	errorFSM.eventButtonStartPressed();
	if(errorFSM.isActive() == false){
		exit(ctx);
		new (this) Operation(ctx.getOperationFSM());
		ctx.getOperationFSM().restoreHistory();
		MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ERROR_RESOLVED, 0);
	}
}
void Error::eventErrorResolved(MainContext& ctx) {
    exit(ctx);
    new(this) Operation(ctx.getOperationFSM());
    ctx.getOperationFSM().restoreHistory();
}
void Error::eventGoBackIH(MainContext& ctx){
	ctx.getOperationFSM().eventGoBackIH();
}
void Error::eventGoBackIM(MainContext& ctx){
	ctx.getOperationFSM().eventGoBackIM();
}
void Error::eventGoBackIF(MainContext& ctx){
	ctx.getOperationFSM().eventGoBackIF();
}
void Error::eventGoBackIHM(MainContext& ctx){
	ctx.getOperationFSM().eventGoBackIHM();
}

void Error::eventGoBack(MainContext& ctx){
	ctx.getOperationFSM().eventGoBack();
}


void Error::eventConnectionLost(MainContext& ctx) {
	exit(ctx);
	new(this) ConnectionLost(ctx.hal, ctx.hs, PreviousFsmState::ERROR);
	entry(ctx);
}


