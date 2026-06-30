/*
 * ForwardSlow2.cpp
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#include "TransportBase2.h"
#include "TransportContext2.h"
#include "Forward2.h"
#include "ForwardSlow2.h"
#include "Stop2.h"

#include <iostream>
namespace M2{

	ForwardSlow2::ForwardSlow2(){
	}

	void ForwardSlow2::entry(TransportContext2& ctx){
		if(ctx.data.itemCount == 0 || ctx.hal.getSensors()->isItemAtExit()){
			exit(ctx);
			new(this) Stop2;
			entry(ctx);
			return;
		}
		else{
			ctx.hal.getActors()->motorForwards();
			ctx.hal.getActors()->motorSlow();
			ctx.data.handler->resumeAll();
			ctx.data.handler->setSlow(ctx.data.slowFactor);
		}
		if(ctx.data.skipSlow){
		    ctx.data.skipSlow = false;
		    exit(ctx);
		    new(this) Forward2;
		    entry(ctx);
		    return;
		}
		if(ctx.data.itemGone == true){
//			ctx.data.itemGone = false;
			eventItemGone(ctx);
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

	void ForwardSlow2::exit(TransportContext2& ctx){}

	void ForwardSlow2::eventItemGone(TransportContext2& ctx){
		if(!ctx.data.section1.empty()){
			exit(ctx);
			new(this) Forward2;
			entry(ctx);
		}
		else{
			ctx.data.handler->pauseAll();
			ctx.hal.getActors()->motorStop();
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 1;
			ctx.data.raiseError();
		}
	}

	void ForwardSlow2::eventItemInSlide(TransportContext2& ctx){
		if(!ctx.data.section2.empty()){
			ctx.data.removeItem(2);
		}
		if(ctx.data.itemCount == 0){
			exit(ctx);
			new(this) Stop2;
			entry(ctx);
		}
	}

	void ForwardSlow2::eventItemLeftEntry(TransportContext2& ctx){
		if(!ctx.data.section1.empty()){
			ctx.data.section1.back().timer->start(ctx.data.maxEntryTime,0);
		}
		else{
		}
	}

	void ForwardSlow2::eventItemAtPusher(TransportContext2& ctx){
		exit(ctx);
		if(!ctx.data.section2.empty()){
			int diff = ctx.data.pusherTime - ctx.data.section2.front().timer->getTime();
			if(ctx.data.section2.front().timer->getTime() < ctx.data.minPusherTime){
				ctx.data.handler->pauseAll();
				ctx.hal.getActors()->motorStop();
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
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 2;
			ctx.data.raiseError();
		}
	}

	void ForwardSlow2::eventItemAtExit(TransportContext2& ctx){
		ctx.data.itemAtExit = true;
		if(!ctx.data.section3.empty()){
			int diff = ctx.data.exitTime - ctx.data.section3.front().timer->getTime();
			if(ctx.data.section3.front().timer->getTime() < ctx.data.minExitTime){
				ctx.data.handler->pauseAll();
				ctx.hal.getActors()->motorStop();
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 3;
				ctx.data.raiseError();
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
			ctx.hal.getActors()->motorStop();
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 3;
			ctx.data.raiseError();
		}
	}

	void ForwardSlow2::eventItemLeftExitRemote(TransportContext2& ctx){
		ctx.data.addItem();
	}
}

