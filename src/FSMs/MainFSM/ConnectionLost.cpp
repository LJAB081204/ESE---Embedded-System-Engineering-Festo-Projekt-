/*
 * ConnectionLost.cpp
 *
 *  Created on: 17.06.2026
 *      Author: mika2
 */

#include "MainContext.h"
#include "ConnectionLost.h"

using namespace std;

void ConnectionLost::entry(MainContext& ctx) {
	ctx.hal.getActors()->motorStop();
	ctx.hal.getActors()->greenLampOff();
	ctx.hal.getActors()->redLampOn();
	ctx.hal.getActors()->ledButtonStartOff();
	cout << "Connection Lost" << endl;
}
void ConnectionLost::exit(MainContext& ctx) {
	ctx.hal.getActors()->redLampOff();
	cout << "Connection Restored" << endl;
}

void ConnectionLost::eventButtonEstopPressed(MainContext& ctx) {
	conLostFSM.eventButtonEstopPressed();
}

void ConnectionLost::eventButtonEstopReleased(MainContext& ctx) {
	conLostFSM.eventButtonEstopReleased();
}

void ConnectionLost::eventConnectionLost(MainContext& ctx) {
	conLostFSM.eventConnectionLost();
}

void ConnectionLost::eventConnectionRestored(MainContext& ctx) {
    exit(ctx);
    EStopButtonState estopState = conLostFSM.estopState;
	EStop* estop = new(this) EStop(ctx.hal, ctx.hs, ctx.data, EStopSource::LOCAL);
    estop->entry(ctx);
	switch(estopState){
	case EStopButtonState::ISPRESSED:
		estop->enterHandshakeEStopIsPressed(ctx);
		break;
	case EStopButtonState::WASPRESSED:
		estop->enterHandShakeEStopWasPressed(ctx);
		break;
	case EStopButtonState::WASNOTPRESSED:
		estop->enterHandShakeEStopWasNotPressed(ctx);
		break;
	}

}
