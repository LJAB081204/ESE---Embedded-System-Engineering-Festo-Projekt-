/*
 * waitingForStart.cpp
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */

#include <new>
#include "ErrorBase.h"
#include "ErrorContext.h"
#include "waitingForStart.h"
#include "../../PulseMessages.h"
#include "errorPusherTooLong.h"
#include <iostream>


	waitingForStart::waitingForStart(){

	}

void waitingForStart::entry(ErrorContext& ctx){
    if(ctx.data.err == ITEM_GONE){
        itemProfile item;
        switch(ctx.data.errorLoc){
            case 1:
                ctx.data.removeItem(1);
                if(!ctx.data.nextItem.empty()){
                    item = ctx.data.nextItem.front().item;
                    bool sortOut = ctx.data.nextItem.front().sortOut;
                    ctx.data.nextItem.pop();
                    if(!ctx.data.isMachine1){
                        if(!sortOut){
                            switch(item){
                                case IH_PROFILE:
                                    if(ctx.data.oneRound){
                                        MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IHM, 0);
                                    } else {
                                        MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_WFP, 0);
                                    }
                                    break;
                                case IM_PROFILE:
                                    MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IH, 0);
                                    break;
                                case IF_PROFILE:
                                    MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IM, 0);
                                    break;
                                case IHM_PROFILE:
                                    MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IF, 0);
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
                break;

            case 2:
                ctx.data.removeItem(2);
                if(!ctx.data.nextItem.empty()){
                    item = ctx.data.nextItem.front().item;
                    bool sortOut = ctx.data.nextItem.front().sortOut;
                    ctx.data.nextItem.pop();
                    if(!ctx.data.isMachine1){
                        if(!sortOut){
                            switch(item){
                                case IH_PROFILE:
                                    if(ctx.data.oneRound){
                                        MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IHM, 0);
                                    } else {
                                        MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_WFP, 0);
                                    }
                                    break;
                                case IM_PROFILE:
                                    MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IH, 0);
                                    break;
                                case IF_PROFILE:
                                    MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IM, 0);
                                    break;
                                case IHM_PROFILE:
                                    MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IF, 0);
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
                break;

            case 3:
                if(!ctx.data.section3.empty()){
                    item = ctx.data.section3.front().item;
                    ctx.data.removeItem(3);
                    if(ctx.data.isMachine1){
                        switch(item){
                            case IH_PROFILE:
                                if(ctx.data.oneRound){
                                    MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IHM, 0);
                                } else {
                                    MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_WFP, 0);
                                }
                                break;
                            case IM_PROFILE:
                                MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IH, 0);
                                break;
                            case IF_PROFILE:
                                MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IM, 0);
                                break;
                            case IHM_PROFILE:
                                MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IF, 0);
                                break;
                            default:
                                break;
                        }
                    } else {
                        switch(item){
                            case IH_PROFILE:
                                if(ctx.data.oneRound){
                                    MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IHM, 0);
                                    MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_LOCAL, 0);
                                } else {
                                    MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_WFP, 0);
                                    MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_LOCAL, 0);
                                }
                                break;
                            case IM_PROFILE:
                                MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IH, 0);
                                MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_LOCAL, 0);
                                break;
                            case IF_PROFILE:
                                MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IM, 0);
                                MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_LOCAL, 0);
                                break;
                            case IHM_PROFILE:
                                MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IF, 0);
                                MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_LOCAL, 0);
                                break;
                            default:
                                break;
                        }
                    }
                }
                break;

        }  // Ende switch(errorLoc)
    }  // Ende if(err == ITEM_GONE)

    ctx.data.err = ERROR_CLEARED;
    ctx.hal.getActors()->ledButtonStart();
    std::cout << "Starttaste betätigen um Betrieb wiederaufzunehmen." << std::endl;
}

	void waitingForStart::exit(ErrorContext& ctx){
		ctx.hal.getActors()->ledButtonStartOff();
		 std::cout << ctx.data.itemCount << std::endl;
		 if(ctx.data.checkReady()){
		 MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_TRANSFER_REPLY, 1);
		 ctx.data.err = ERROR_CLEARED;
		 }
	}

	void waitingForStart::eventButtonStartPressed(ErrorContext& ctx){
		exit(ctx);
		ctx.data.errorState = CURRENT_NACK_STATE;
		ctx.active = false;
	}

	void waitingForStart::errorPusherTimerExpired(ErrorContext& ctx){
		exit(ctx);
		new (this) errorPusherTooLong;
		entry(ctx);
	}


