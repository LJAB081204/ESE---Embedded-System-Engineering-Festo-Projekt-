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
		std::cout<<"ForwardSlow" << std::endl;
		ctx.hal.getActors()->motorForwards();
		ctx.hal.getActors()->motorSlow();
		ctx.data.handler->setSlow(ctx.data.slowFactor);
	}

	void ForwardSlow::exit(TransportContext& ctx){
		ctx.data.handler->resumeAll();
	}

	void ForwardSlow::eventItemGone(TransportContext& ctx){
		exit(ctx);
		if(!ctx.data.section1.empty()){
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
		new (this) Forward;
		entry(ctx);
	}

	void ForwardSlow::eventItemAtExit(TransportContext& ctx){
		exit(ctx);
		raiseRequestTransfer(ctx);
		new(this) SlowWaitForFesto;
		entry(ctx);
	}

	void ForwardSlow::eventItemAtEntry(TransportContext& ctx){
		ctx.data.addItem();
	}

	void ForwardSlow::eventItemLeftEntry(TransportContext& ctx){
		if(!ctx.data.section1.empty()){
			ctx.data.section1.back().timer->start(ctx.data.entryTime+1000);
		}
		else{
			std::cout<<"Queue 1 leer Festo1" << std::endl;
		}
	}

	void ForwardSlow::eventItemAtEntryRemote(TransportContext& ctx){
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

	void ForwardSlow::eventItemAtPusher(TransportContext& ctx){
		exit(ctx);
		if(!ctx.data.section2.empty()){
			std::cout<< "pusher time: "<<ctx.data.pusherTime << std::endl;
			std::cout<< "getTime: "<<ctx.data.section2.front().timer->getTime() << std::endl;
			int diff = ctx.data.pusherTime - ctx.data.section2.front().timer->getTime();
			std::cout<< diff << std::endl;
			if(diff>1000){
				ctx.data.handler->pauseAll();
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
}
