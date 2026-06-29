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
    updateSequence(ctx);
}
void EStop::exit(MainContext& ctx) {
	ctx.hal.getActors()->redLampOff();
	ctx.data.reset();

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
        	std::cout << "Restore nach Estop" << std::endl;
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
        	std::cout << "Restore nach Estop" << std::endl;
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

void EStop::updateSequence(MainContext& ctx) {
	if(!ctx.data.isMachine1){
		if(ctx.data.section3.empty()){
			cout << "Section empty" << endl;
			switch(ctx.data.sequenceState){
				case WFP_State: MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_WFP, 0); break;
				case IH_State: MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IH, 0); break;
				case IM_State: MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IM, 0); break;
				case IF_State: MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IF, 0); break;
				case IHM_State: MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IHM, 0); break;
				default: break;
					}
			}
		else {
			cout << "Section not empty" << endl;
			switch(ctx.data.sequenceState){
				case IH_State:
					if (ctx.data.oneRound){
						MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IHM, 0);
					} else {
						MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_WFP, 0);
					}
					break;
				case IM_State: MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IH, 0); break;
				case IF_State: MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IM, 0); break;
				case IHM_State: MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IF, 0); break;
				default: break;
					}
			MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_LOCAL, 0);
		}
		}
	}

void EStop::eventGoBackIH(MainContext& ctx){
	cout << "Go Back IH in Estop" << endl;
	ctx.getOperationFSM().eventGoBackIH();
}
void EStop::eventGoBackIM(MainContext& ctx){
	ctx.getOperationFSM().eventGoBackIM();
}
void EStop::eventGoBackIF(MainContext& ctx){
	ctx.getOperationFSM().eventGoBackIF();
}
void EStop::eventGoBackIHM(MainContext& ctx){
	ctx.getOperationFSM().eventGoBackIHM();
}

void EStop::eventGoBackWFP(MainContext& ctx){
	cout << "Go Back WFP in Estop" << endl;
	ctx.getOperationFSM().eventGoBackWFP();
}

void EStop::eventGoBack(MainContext& ctx){
	cout << "Go Back (1) in Estop" << endl;
	ctx.getOperationFSM().eventGoBack();
}
