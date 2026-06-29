/*
 * ForwardSlow.cpp
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#include "TransportBase.h"
#include "TransportContext.h"
#include "Forward.h"
#include "ForwardSlow.h"
#include "SlowWaitForFesto.h"
#include "sys/neutrino.h"
#include "../../../../PulseMessages.h"
#include <iostream>
#include "../../../../timer.h"
namespace M1{

	ForwardSlow::ForwardSlow(){
	}

	void ForwardSlow::entry(TransportContext& ctx){
		std::cout << "ForwardSlow::entry skipSlow=" << ctx.data.skipSlow << std::endl;
		if(ctx.data.itemCount == 0){
			exit(ctx);
			new(this) Stop;
			entry(ctx);
		}
		if(ctx.data.skipSlow){
		    ctx.data.skipSlow = false;
		    exit(ctx);
		    new(this) Forward;
		    entry(ctx);
		}
		if(ctx.data.itemAtExit == true){
			ctx.data.itemAtExit = false;
			eventItemAtExit(ctx);
		}
		if(ctx.data.itemPusher == true){
			ctx.data.itemPusher = false;
			eventItemAtPusher(ctx);
		}
		if(ctx.data.itemCount != 0){
			ctx.hal.getActors()->motorForwards();
			ctx.hal.getActors()->motorSlow();
			ctx.data.handler->resumeAll();
			ctx.data.handler->setSlow(ctx.data.slowFactor);
			std::cout<<"slow factor set"<<ctx.data.slowFactor<<std::endl;
		}
	}

	void ForwardSlow::exit(TransportContext& ctx){}

	void ForwardSlow::eventItemGone(TransportContext& ctx){
		if(!ctx.data.section1.empty()){
			exit(ctx);
			new(this) Forward;
			entry(ctx);
		}
		else{
			ctx.data.handler->pauseAll();
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 1;
			ctx.data.raiseError();
		}
	}

	void ForwardSlow::eventItemAtExit(TransportContext& ctx){
		if(!ctx.data.section3.empty()){
			std::cout<< "min exit time: "<<ctx.data.minExitTime << std::endl;
			std::cout<< "getTime: "<<ctx.data.section3.front().timer->getTime() << std::endl;
			if(ctx.data.section3.front().timer->getTime() < ctx.data.minExitTime){
				ctx.data.handler->pauseAll();
				std::cout<< "Error item at exit"<< std::endl;
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 3;
				ctx.data.raiseError();
			}
			else{
				if(!ctx.data.section3.empty()){
					uint32_t runnedTime = ctx.data.section3.front().timer->getTime();
					ctx.data.section3.front().timer->stop();
					std::cout<<"runnedTime:"<<runnedTime<<std::endl;
					delete ctx.data.section3.front().timer;
					ctx.data.section3.front().timer = new DynamicTimer(ctx.data.section3.front().id, *ctx.data.handler);
					ctx.data.section3.front().timer->start(ctx.data.maxExitTime,runnedTime);
				}
				exit(ctx);
				raiseRequestTransfer(ctx);
				new(this) SlowWaitForFesto;
				entry(ctx);
			}
		}
		else{
			ctx.data.handler->pauseAll();
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 3;
			ctx.data.raiseError();
		}
	}

	void ForwardSlow::eventItemAtEntry(TransportContext& ctx){
		if(!ctx.data.readyForEntry){
			ctx.data.isErrorLocal = true;
			ctx.data.err = ENTRY_TOO_FAST;
			ctx.data.errorLoc = 1;
			ctx.data.handler->pauseAll();
			raiseErrorEntry(ctx);
			return;
		}
		ctx.data.addItem();
		if(ctx.data.timerIS == nullptr){
			ctx.data.timerIS = new DynamicTimer(102,*ctx.data.handler);
		}
		ctx.data.timerIS->start(1000,0);
	}

	void ForwardSlow::eventItemLeftEntry(TransportContext& ctx){
		if(!ctx.data.section1.empty()){
			ctx.data.section1.back().timer->start(ctx.data.maxEntryTime,0);
			ctx.data.timerIS->stop();
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

	void ForwardSlow::eventItemAtEntryRemote(TransportContext& ctx){
		exit(ctx);
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
			new(this) Stop;
		}
		entry(ctx);
	}

	void ForwardSlow::eventItemAtPusher(TransportContext& ctx){
		exit(ctx);
		if(!ctx.data.section2.empty()){
			std::cout<< "min pusher time: "<<ctx.data.minPusherTime << std::endl;
			std::cout<< "getTime: "<<ctx.data.section2.front().timer->getTime() << std::endl;
			int diff = ctx.data.pusherTime - ctx.data.section2.front().timer->getTime();
			std::cout<< diff << std::endl;
			if(ctx.data.section2.front().timer->getTime() < ctx.data.minPusherTime){
				ctx.data.handler->pauseAll();
				ctx.hal.getActors()->motorStop();
				std::cout<< "Error item at Pusher"<< std::endl;
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 2;
				ctx.data.raiseError();
			}
			else{
				ctx.data.section2.front().timer->stop();
			}
		}
		else{
			ctx.data.handler->pauseAll();
			ctx.hal.getActors()->motorStop();
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 2;
			ctx.data.raiseError();
		}
	}

	void ForwardSlow::eventItemInSlide(TransportContext& ctx){
		if(!ctx.data.section2.empty()){
			ctx.data.removeItem(2);
		}
		if(ctx.data.itemCount == 0){
			exit(ctx);
			new(this) Stop;
			entry(ctx);
		}
	}
	void ForwardSlow::raiseRequestTransfer(TransportContext& ctx){
			MsgSendPulse(ctx.data.controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_TRANSFER_REQUEST,0);
	}
	void ForwardSlow::raiseError(TransportContext& ctx){
		MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR,0);
	}
	void ForwardSlow::raiseErrorEntry(TransportContext& ctx){
		MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR_ENTRY,0);
	}
}
