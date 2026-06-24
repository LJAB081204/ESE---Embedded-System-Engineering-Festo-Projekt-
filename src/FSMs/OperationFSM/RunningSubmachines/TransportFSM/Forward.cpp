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
			ctx.data.isErrorLocal = true;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 1;
			ctx.data.handler->pauseAll();
			raiseErrorEntry(ctx);
		}
		else{
		ctx.data.addItem();
		}
	}

	void Forward::eventItemLeftEntry(TransportContext& ctx){
		if(!ctx.data.section1.empty()){
			ctx.data.section1.back().timer->start(ctx.data.entryTime+1000);
		}
		else{
			std::cout<<"Queue 1 leer Festo1" << std::endl;
		}
		ctx.data.readyForEntry = false;
		ctx.hal.getActors()->lampQ1();
		if(ctx.data.minTimeEntryTimer == nullptr){
			ctx.data.minTimeEntryTimer = new DynamicTimer(101,*ctx.data.handler);
		}
		ctx.data.minTimeEntryTimer->start(ctx.data.minTimeEntry);
	}
	void Forward::eventItemArrived(TransportContext& ctx){
		exit(ctx);
		if(!ctx.data.section1.empty()){
			std::cout<< "entry time: "<<ctx.data.entryTime << std::endl;
			std::cout<< "getTime: "<<ctx.data.section1.front().timer->getTime() << std::endl;
			int diff = ctx.data.entryTime - ctx.data.section1.front().timer->getTime();
			std::cout<< diff << std::endl;
			if(diff>300){
				ctx.data.handler->pauseAll();
				std::cout<< "Error item arrived"<< std::endl;
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 1;
				raiseError(ctx);
			}
			else{
				ctx.data.section1.front().timer->stop();
				new(this) ForwardSlow;
				entry(ctx);
			}
		}
		else{
			ctx.data.handler->pauseAll();
			ctx.data.section1.front().timer->stop();
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 1;
			raiseError(ctx);
		}

	}

	void Forward::eventItemAtPusher(TransportContext& ctx){
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
				raiseError(ctx);
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
			raiseError(ctx);
		}
	}

	void Forward::eventItemInSlide(TransportContext& ctx){
		exit(ctx);
		ctx.data.removeItem(2);
		if(ctx.data.itemCount == 0){
			new(this) Stop;
		}
		entry(ctx);
	}

	void Forward::eventItemAtExit(TransportContext& ctx){
		exit(ctx);
		if(!ctx.data.section3.empty()){
			std::cout<< "exit time: "<<ctx.data.exitTime << std::endl;
			std::cout<< "getTime: "<<ctx.data.section3.front().timer->getTime() << std::endl;
			int diff = ctx.data.exitTime - ctx.data.section3.front().timer->getTime();
			std::cout<< diff << std::endl;
			if(diff>1000){
				ctx.data.handler->pauseAll();
				std::cout<< "Error item at exit"<< std::endl;
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 3;
				raiseError(ctx);
			}
			else{
				exit(ctx);
				ctx.data.section3.front().timer->stop();
				raiseRequestTransfer(ctx);
				new(this) WaitForFesto;
				entry(ctx);
			}
		}
		else{
			ctx.data.handler->pauseAll();
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 3;
			raiseError(ctx);
		}
	}

	void Forward::eventItemAtEntryRemote(TransportContext& ctx){
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


	void Forward::raiseRequestTransfer(TransportContext& ctx){
		MsgSendPulse(ctx.data.controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_TRANSFER_REQUEST,0);

	}
	void Forward::raiseError(TransportContext& ctx){
		MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR,0);
	}
	void Forward::raiseErrorEntry(TransportContext& ctx){
		MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR_ENTRY,0);
	}
}





