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
		MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_TRANSFER_REQUEST, 0);
		if(ctx.data.itemArrived == true){
			ctx.data.itemArrived = false;
			eventItemArrived(ctx);
		}
		if(ctx.data.itemArrived == true){
			ctx.data.itemArrived = false;
			eventItemArrived(ctx);
		}
		if(ctx.data.itemExit == true){
			ctx.data.itemExit = false;
			eventItemAtExit(ctx);

		}
		if(ctx.data.itemPusher == true){
			ctx.data.itemPusher = false;
			eventItemAtPusher(ctx);
		}
		if(ctx.data.itemLeftExit == true){
			ctx.data.itemLeftExit = false;
			eventItemLeftExit(ctx);
		}
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
		}
		ctx.data.readyForEntry = false;
		ctx.hal.getActors()->lampQ1();
		if(ctx.data.minTimeEntryTimer == nullptr){
		    ctx.data.minTimeEntryTimer = new DynamicTimer(101, *ctx.data.handler);
		}
		ctx.data.minTimeEntryTimer->start(ctx.data.minTimeEntry, 0);
	}

	void WaitForFesto::eventItemArrived(TransportContext& ctx){
		if(!ctx.data.section1.empty()){
			if(ctx.data.section1.front().timer->getTime() < ctx.data.minEntryTime){
				ctx.data.handler->pauseAll();
				ctx.hal.getActors()->motorStop();
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 1;
				ctx.data.raiseError();
			}
			else{
				if(!ctx.data.section1.empty()){
					ctx.data.section1.front().timer->stop();
				}
			}
		}
		else{
			ctx.data.handler->pauseAll();
			if(!ctx.data.section1.empty()){
				ctx.data.section1.front().timer->stop();
			}
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 1;
			ctx.data.raiseError();
		}

	}

	void WaitForFesto::eventItemAtPusher(TransportContext& ctx){
		if(!ctx.data.section2.empty()){
			if(ctx.data.section2.front().timer->getTime() < ctx.data.minPusherTime){
				ctx.hal.getActors()->motorStop();
				ctx.data.handler->pauseAll();
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

