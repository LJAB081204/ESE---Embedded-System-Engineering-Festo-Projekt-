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
	ctx.hal.getActors()->ledButtonStartOff();
	ctx.hal.getActors()->closePusher();
	cout << "--- [VERBINDUNG VERLOREN] ---" << endl;
	cout << "Bitte stelle Verbindung wieder her." << endl;
}
void ConnectionLost::exit(MainContext& ctx) {
	ctx.hal.getActors()->redLampBlinkOff();
	cout << "Verbindung wiederhergestellt – bitte Reset drücken." << endl;
}

void ConnectionLost::eventButtonEstopPressed(MainContext& ctx) {
	ctx.data.reset();
    ctx.getOperationFSM().saveHistory(HistorySource::ESTOP_HISTORY);
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
