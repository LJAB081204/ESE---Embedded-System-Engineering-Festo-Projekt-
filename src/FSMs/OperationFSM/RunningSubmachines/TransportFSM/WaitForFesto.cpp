/*
 * WaitForFesto.cpp
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */
#include "TransportBase.h"
#include "TransportContext.h"
#include "Forward.h"
#include "WaitForFesto.h"
#include "../../../../PulseMessages.h"
namespace M1{

	WaitForFesto::WaitForFesto(){
	}

	void WaitForFesto::entry(TransportContext& ctx){
		std::cout<<"WaitForFesto" << std::endl;
		MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_TRANSFER_REQUEST, 0);
	}

	void WaitForFesto::exit(TransportContext& ctx){}

	void WaitForFesto::eventFestoNotReady(TransportContext& ctx){
		ctx.hal.getActors()->motorStop();
		ctx.data.handler->pauseAll();
	}
	void WaitForFesto::eventFestoReady(TransportContext& ctx){
		exit(ctx);
		new(this) Forward;
		entry(ctx);
	}

	void WaitForFesto::eventItemAtEntry(TransportContext& ctx){
		ctx.data.addItem();
		if(ctx.data.timerIS == nullptr){
			ctx.data.timerIS = new DynamicTimer(102,*ctx.data.handler);
		}
		ctx.data.timerIS->start(1000,0);
	}

	void WaitForFesto::eventItemLeftEntry(TransportContext& ctx){
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
		    ctx.data.minTimeEntryTimer = new DynamicTimer(101, *ctx.data.handler);
		}
		ctx.data.minTimeEntryTimer->start(ctx.data.minTimeEntry, 0);
	}

	void WaitForFesto::eventItemAtPusher(TransportContext& ctx){
		if(!ctx.data.section2.empty()){
			std::cout<< "min pusher time: "<<ctx.data.minPusherTime << std::endl;
			std::cout<< "getTime: "<<ctx.data.section2.front().timer->getTime() << std::endl;
			if(ctx.data.section2.front().timer->getTime() < ctx.data.minPusherTime){
				ctx.hal.getActors()->motorStop();
				ctx.data.handler->pauseAll();
				std::cout<< "Error item at Pusher"<< std::endl;
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 2;
				ctx.data.raiseError();
			}
			else{
				if(!ctx.data.section2.empty()){
					ctx.data.section2.front().timer->stop();
				}
			}
		}
		else{
			ctx.hal.getActors()->motorStop();
			ctx.data.handler->pauseAll();
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 2;
			ctx.data.raiseError();
		}
	}

	void WaitForFesto::eventItemInSlide(TransportContext& ctx){
		if(!ctx.data.section2.empty()){
			delete ctx.data.section2.front().timer;
			ctx.data.section2.pop();
			ctx.data.itemCount--;
		}
	}

}

