/*
 * Forward.cpp
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */
#include "TransportBase.h"
#include "TransportContext.h"
#include "Forward.h"
#include "Stop.h"
#include "WaitForFesto.h"
#include "ForwardSlow.h"
#include "../../../../PulseMessages.h"
#include <iostream>
#include <sys/neutrino.h>
#include "../../../../timer.h"

namespace M1{

	Forward::Forward(){
	}

	void Forward::entry(TransportContext& ctx){
		if(ctx.data.itemCount == 0){
			exit(ctx);
			new(this) Stop;
			entry(ctx);
		}
		else{
		ctx.hal.getActors()->motorStop();
		ctx.hal.getActors()->motorForwards();
		ctx.data.handler->resumeAll();
		ctx.data.handler->setNormal();
		}
	}

	void Forward::exit(TransportContext& ctx){
	}

	void Forward::eventItemAtEntry(TransportContext& ctx){
		if(!ctx.data.readyForEntry){
			ctx.hal.getActors()->motorStop();
			ctx.data.isErrorLocal = true;
			ctx.data.err = ENTRY_TOO_FAST;
			ctx.data.errorLoc = 1;
			ctx.data.handler->pauseAll();
			raiseErrorEntry(ctx);
		}
		else{
			ctx.data.addItem();
			if(ctx.data.timerIS == nullptr){
				ctx.data.timerIS = new DynamicTimer(102,*ctx.data.handler);
			}
			ctx.data.timerIS->start(1000,0);
		}
	}

	void Forward::eventItemLeftEntry(TransportContext& ctx){
		if(!ctx.data.section1.empty()){
			ctx.data.section1.back().timer->start(ctx.data.maxEntryTime,0);
			if(ctx.data.timerIS != nullptr){
				ctx.data.timerIS->stop();
			}
		}
		else{
			std::cout<<"Queue 1 leer Festo1" << std::endl;
		}
		ctx.data.readyForEntry = false;
		ctx.hal.getActors()->lampQ1();
		if(ctx.data.minTimeEntryTimer == nullptr){
			ctx.data.minTimeEntryTimer = new DynamicTimer(101,*ctx.data.handler);
		}
		ctx.data.minTimeEntryTimer->start(ctx.data.minTimeEntry,0);
	}


	void Forward::eventItemArrived(TransportContext& ctx){
		if(!ctx.data.section1.empty()){
			std::cout<< "entry time: "<<ctx.data.entryTime << std::endl;
			std::cout<< "getTime: "<<ctx.data.section1.front().timer->getTime() << std::endl;
//			int diff = ctx.data.entryTime - ctx.data.section1.front().timer->getTime();
			//std::cout<< diff << std::endl;
			if(ctx.data.section1.front().timer->getTime() < ctx.data.minEntryTime){
				ctx.data.handler->pauseAll();
				ctx.hal.getActors()->motorStop();
				std::cout<< "Error item arrived"<< std::endl;
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 1;
				raiseError(ctx);
			}
			else{
				if(!ctx.data.section1.empty()){
					ctx.data.section1.front().timer->stop();
				}
				exit(ctx);
				new(this) ForwardSlow;
				entry(ctx);
			}
		}
		else{
			ctx.data.handler->pauseAll();
			if(!ctx.data.section1.empty()){
				ctx.data.section1.front().timer->stop();
			}
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 1;
			raiseError(ctx);
		}

	}

	void Forward::eventItemAtPusher(TransportContext& ctx){
		if(!ctx.data.section2.empty()){
			std::cout<< "pusher time: "<<ctx.data.pusherTime << std::endl;
			std::cout<< "getTime: "<<ctx.data.section2.front().timer->getTime() << std::endl;
			int diff = ctx.data.pusherTime - ctx.data.section2.front().timer->getTime();
			std::cout<< diff << std::endl;
			if(ctx.data.section2.front().timer->getTime() < ctx.data.minPusherTime){
				ctx.hal.getActors()->motorStop();
				ctx.data.handler->pauseAll();
				std::cout<< "Error item at Pusher"<< std::endl;
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 2;
				raiseError(ctx);
			}
			else{
				ctx.data.section2.front().timer->stop();
			}
		}
		else{
			ctx.hal.getActors()->motorStop();
			ctx.data.handler->pauseAll();
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 2;
			raiseError(ctx);
		}
	}

	void Forward::eventItemInSlide(TransportContext& ctx){
		if(!ctx.data.section2.empty()){
			ctx.data.removeItem(2);
		}
		if(ctx.data.itemCount == 0){
			exit(ctx);
			new(this) Stop;
			entry(ctx);
		}
	}

	void Forward::eventItemAtExit(TransportContext& ctx){
		if(!ctx.data.section3.empty()){
			std::cout<< "exit time: "<<ctx.data.exitTime << std::endl;
			std::cout<< "getTime: "<<ctx.data.section3.front().timer->getTime() << std::endl;
			if(ctx.data.section3.front().timer->getTime() < ctx.data.minExitTime){
				ctx.data.handler->pauseAll();
				std::cout<< "Error item at exit"<< std::endl;
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 3;
				raiseError(ctx);
				ctx.hal.getActors()->motorStop();
			}
			else{
				ctx.data.section3.front().timer->stop();
				uint32_t runnedTime = ctx.data.section3.front().timer->getTime();
				ctx.data.section3.front().timer->stop();
				std::cout<<"runnedTime:"<<runnedTime<<std::endl;
				delete ctx.data.section3.front().timer;
				ctx.data.section3.front().timer = new DynamicTimer(ctx.data.section3.front().id, *ctx.data.handler);
				ctx.data.section3.front().timer->start(ctx.data.maxExitTime,runnedTime);
				exit(ctx);
				if(!ctx.data.section3.empty()){
					uint32_t runnedTime = ctx.data.section3.front().timer->getTime();
					ctx.data.section3.front().timer->stop();
					std::cout<<"runnedTime:"<<runnedTime<<std::endl;
					delete ctx.data.section3.front().timer;
					ctx.data.section3.front().timer = new DynamicTimer(ctx.data.section3.front().id, *ctx.data.handler);
					ctx.data.section3.front().timer->start(ctx.data.maxExitTime,runnedTime);
				}
				raiseRequestTransfer(ctx);
				new(this) WaitForFesto;
				entry(ctx);
			}
		}
		else{
			ctx.data.handler->pauseAll();
			ctx.hal.getActors()->motorStop();
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 3;
			raiseError(ctx);

		}
	}

	void Forward::eventItemAtEntryRemote(TransportContext& ctx){
		itemData item;
		if(!ctx.data.section3.empty()){
			item = ctx.data.section3.front();
		}
		uint32_t itemCode = ctx.data.convertItemToInt(item);
		MsgSendPulse(ctx.data.controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_ITEM_TRANSFER,itemCode);
		if(!ctx.data.section3.empty()){
			ctx.data.removeItem(3);
		}
		if(ctx.data.itemCount == 0){
			exit(ctx);
			new(this) Stop;
			entry(ctx);
		}
	}

	void Forward::eventTransportTimerExpired(TransportContext& ctx){
		raiseErrorIS(ctx);
	}

	void Forward::raiseRequestTransfer(TransportContext& ctx){
		MsgSendPulse(ctx.data.controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_TRANSFER_REQUEST,0);

	}
	void Forward::raiseError(TransportContext& ctx){
		ctx.hal.getActors()->motorStop();
		MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR,0);
        MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ERROR_REMOTE, 0);
	}
	void Forward::raiseErrorEntry(TransportContext& ctx){
		MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR_ENTRY,0);
	}
	void Forward::raiseErrorIS(TransportContext& ctx){
		ctx.data.err = ITEM_SPECIAL_0;
		ctx.data.errorLoc = 0;
		MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR,0);
	}
}





