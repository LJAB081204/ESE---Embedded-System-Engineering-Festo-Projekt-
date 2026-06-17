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
}





