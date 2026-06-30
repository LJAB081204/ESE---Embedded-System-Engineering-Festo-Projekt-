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
    ctx.data.handler->pauseAll();

}
void Error::exit(MainContext& ctx) {
	ctx.hal.getActors()->redLampBlinkOff();
	ctx.hal.getActors()->redLampOff();
	ctx.data.handler->resumeAll();
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
		ctx.data.errorState = CURRENT_NACK_STATE;
		new (this) Operation(ctx.getOperationFSM());
		ctx.getOperationFSM().restoreHistory();
		MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ERROR_RESOLVED, 0);
	}
}
void Error::eventErrorResolved(MainContext& ctx) {
    exit(ctx);
    ctx.data.errorState = CURRENT_NACK_STATE;
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

void Error::eventGoBackWFP(MainContext& ctx){
	ctx.getOperationFSM().eventGoBackWFP();
}

void Error::eventGoBack(MainContext& ctx){
	ctx.getOperationFSM().eventGoBack();
}

void Error::eventButtonEstopPressed(MainContext& ctx) {
	errorFSM.setActive(false);
	ctx.data.errorState = CURRENT_NACK_STATE;
	ctx.getOperationFSM().saveHistory(HistorySource::ESTOP_HISTORY);
    exit(ctx);
    new(this) EStop(ctx.hal, ctx.hs, ctx.data, EStopSource::LOCAL);
    entry(ctx);
}
void Error::eventButtonEstopPressedRemote(MainContext& ctx) {
	errorFSM.setActive(false);
	ctx.data.errorState = CURRENT_NACK_STATE;
	ctx.getOperationFSM().saveHistory(HistorySource::ESTOP_HISTORY);
    exit(ctx);
    new(this) EStop(ctx.hal, ctx.hs, ctx.data, EStopSource::REMOTE);
    entry(ctx);
}
void Error::eventConnectionLost(MainContext& ctx) {
	ctx.data.returnToError = true;
	exit(ctx);
	new(this) ConnectionLost(ctx.hal, ctx.hs, PreviousFsmState::ERROR);
	entry(ctx);
}
void Error::eventItemLeftEntry(MainContext& ctx){
	errorFSM.eventItemLeftEntry();
}
void Error::eventButtonResetPressedRemote(MainContext& ctx){
	errorFSM.eventButtonResetPressedRemote();
}
void Error::eventButtonStartPressedRemote(MainContext& ctx){
	errorFSM.eventButtonStartPressedRemote();
}
void Error::eventItemAtPusher(MainContext& ctx) {
	errorFSM.eventItemAtPusher();
}
void Error::eventItemAtExit(MainContext& ctx) {
	errorFSM.eventItemAtExit();
}
void Error::eventItemArrived(MainContext& ctx) {
	errorFSM.eventItemArrived();
}
//void Error::raiseItemTimer(MainContext& ctx){
//	if(ctx.data.itemArrived == true){
//		ctx.data.itemArrived = false;
//		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_ARRIVED, 0);
//	}
//	if(ctx.data.itemExit == true){
//		ctx.data.itemExit = false;
//		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_EXIT_LOCAL, 0);
//	}
//	if(ctx.data.itemPusher == true){
//		ctx.data.itemPusher = false;
//		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_ITEM_AT_PUSHER_LOCAL, 0);
//		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_UPDATE, 0);
//	}
//}
