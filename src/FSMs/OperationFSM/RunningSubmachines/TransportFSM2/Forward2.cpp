/*
 * Forward2.cpp
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */
#include "TransportBase2.h"
#include "TransportContext2.h"
#include "Forward2.h"
#include "Stop2.h"
#include "ForwardSlow2.h"
#include <sys/neutrino.h>
#include "../../../../PulseMessages.h"
#include <iostream>
namespace M2{
	Forward2::Forward2(){
	}

	void Forward2::entry(TransportContext2& ctx){
		if(ctx.data.itemCount == 0 || ctx.hal.getSensors()->isItemAtExit()){
			exit(ctx);
			new(this) Stop2;
			entry(ctx);
		}
		else{
			ctx.hal.getActors()->motorStop();
		ctx.hal.getActors()->motorForwards();
		ctx.data.handler->resumeAll();
		ctx.data.handler->setNormal();
		}
		if(ctx.data.itemLeftEntry == true){
			ctx.data.itemLeftEntry = false;
			eventItemLeftEntry(ctx);
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
//			ctx.data.itemPusher = false;
			eventItemAtPusher(ctx);
		}
		if(ctx.data.itemLeftExit == true){
			ctx.data.itemLeftExit = false;
			eventItemLeftExit(ctx);
		}

	}

	void Forward2::exit(TransportContext2& ctx){
	}

	void Forward2::eventItemLeftEntry(TransportContext2& ctx){
		if(!ctx.data.section1.empty()){
			ctx.data.section1.back().timer->start(ctx.data.maxEntryTime,0);
		}
		else{
		}
	}

	void Forward2::eventItemArrived(TransportContext2& ctx){
		exit(ctx);
		if(!ctx.data.section1.empty()){
			int diff = ctx.data.entryTime - ctx.data.section1.front().timer->getTime();
			if(ctx.data.section1.front().timer->getTime() < ctx.data.minEntryTime){
				ctx.data.handler->pauseAll();
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 1;
				raiseError(ctx);
			}
			else{
				ctx.data.section1.front().timer->stop();
				new(this) ForwardSlow2;
				entry(ctx);
			}
		}
		else{
			ctx.data.handler->pauseAll();
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 1;
			raiseError(ctx);
		}
	}


	void Forward2::eventItemAtPusher(TransportContext2& ctx){
		exit(ctx);
		if(!ctx.data.section2.empty()){
			int diff = ctx.data.pusherTime - ctx.data.section2.front().timer->getTime();
			if(ctx.data.section2.front().timer->getTime() < ctx.data.minPusherTime){
				ctx.data.handler->pauseAll();
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 2;
				raiseError(ctx);
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
			raiseError(ctx);
		}
	}


	void Forward2::eventItemInSlide(TransportContext2& ctx){
		if(!ctx.data.section2.empty()){
			ctx.data.removeItem(2);
		}
		if(ctx.data.itemCount == 0){
			exit(ctx);
			new(this) Stop2;
			entry(ctx);
		}
	}

	void Forward2::eventItemAtExit(TransportContext2& ctx){
		ctx.data.itemAtExit = true;
		ctx.hal.getActors()->motorStop();
		exit(ctx);
		if(!ctx.data.section3.empty()){
			int diff = ctx.data.exitTime - ctx.data.section3.front().timer->getTime();
			if(ctx.data.section3.front().timer->getTime() < ctx.data.minExitTime){
				ctx.data.handler->pauseAll();
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 3;
				raiseError(ctx);
			}
			else{
				if(!ctx.data.section3.empty()){
					ctx.data.section3.front().timer->stop();
					std::cout << "Werkstück am Ende angekommen | ID: " << ctx.data.section3.front().id
					          << " | Typ: " << profileName(ctx.data.section3.front().item) << std::endl;
				}
				exit(ctx);
				new(this) Stop2;
				entry(ctx);
			}
		}
		else{
			ctx.data.handler->pauseAll();
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 3;
			raiseError(ctx);
		}
	}

	void Forward2::eventItemTransfer(uint32_t itemCode, TransportContext2& ctx){
		itemData item = ctx.data.convertIntToItem(itemCode);
		ctx.data.addTransferredItem(item);
	}

	void Forward2::eventItemLeftExitRemote(TransportContext2& ctx){
		ctx.data.addItem();
	}
	void Forward2::raiseError(TransportContext2& ctx){
		ctx.hal.getActors()->motorStop();
		MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR,0);
	}
}






