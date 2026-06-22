/*
 * errorPusherTooLong.cpp
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */
#include <new>
#include "ErrorBase.h"
#include "ErrorContext.h"
#include "waitingForStart2.h"
#include <iostream>
#include "errorPusherTooLong.h"
#include "../../PulseMessages.h"

	errorPusherTooLong::errorPusherTooLong(){

	}

	void errorPusherTooLong::entry(ErrorContext& ctx){
		ctx.data.pusherErrorActive = true;
	    ctx.hal.getActors()->closePusher();
	    ctx.hal.getActors()->ledButtonReset();

		std::cout << "Pusher seit 20 Sekunden offen. Quittiere den Fehler mit Reset." << std::endl;
	}

void errorPusherTooLong::exit(ErrorContext& ctx){
    ctx.hal.getActors()->ledButtonResetOff();
    if(ctx.data.weiche == LET_ITEM_PASS_STATE){
        if(!ctx.data.section2.empty()){
            if(ctx.data.isMachine1){
                switch(ctx.data.section2.front().item){
                case IH_PROFILE:  MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IHM, 0); break;
                case IM_PROFILE:  MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IH,  0); break;
                case IF_PROFILE:  MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IM,  0); break;
                case IHM_PROFILE: MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IF,  0); break;
                default: break;
                }
                MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_LOCAL, 0);
            } else {
                MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_LOCAL, 0);
                switch(ctx.data.section2.front().item){
                case IH_PROFILE:  MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IHM, 0); break;
                case IM_PROFILE:  MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IH,  0); break;
                case IF_PROFILE:  MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IM,  0); break;
                case IHM_PROFILE: MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IF,  0); break;
                default: break;
                }
            }
            ctx.data.removeItem(2);
        }
    }
    else if(ctx.data.weiche == WAIT_STATE){
        if(!ctx.data.section3.empty()){
            if(ctx.data.isMachine1){
                switch(ctx.data.section3.front().item){
                case IH_PROFILE:  MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IHM, 0); break;
                case IM_PROFILE:  MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IH,  0); break;
                case IF_PROFILE:  MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IM,  0); break;
                case IHM_PROFILE: MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IF,  0); break;
                default: break;
                MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_LOCAL, 0);
                }
            } else {
                MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_LOCAL, 0);
                switch(ctx.data.section3.front().item){
                case IH_PROFILE:  MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IHM, 0); break;
                case IM_PROFILE:  MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IH,  0); break;
                case IF_PROFILE:  MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IM,  0); break;
                case IHM_PROFILE: MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IF,  0); break;
                default: break;
                }
            }
            ctx.data.removeItem(3);  // außerhalb beider Branches
        }
    }
}

void errorPusherTooLong::eventButtonResetPressed(ErrorContext& ctx){
    exit(ctx);
    new(this) waitingForStart2;
    entry(ctx);
}

