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
		ctx.hal.getActors()->motorForwards();
		ctx.hal.getActors()->motorSlow();
		ctx.data.handler->resumeAll();
		ctx.data.handler->setSlow(ctx.data.slowFactor);
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
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 1;
			ctx.data.raiseError();
		}
	}

	void ForwardSlow2::eventItemInSlide(TransportContext2& ctx){
		if(!ctx.data.section2.empty()){
			ctx.data.removeItem(2);
		}
		std::cout<< ctx.data.itemCount << std::endl;
		if(ctx.data.itemCount == 0){
			exit(ctx);
			new(this) Stop2;
			entry(ctx);
		}
	}

	void ForwardSlow2::eventItemAtPusher(TransportContext2& ctx){
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

	void ForwardSlow2::eventItemAtExit(TransportContext2& ctx){
		ctx.data.itemAtExit = true;
		std::cout<< ctx.data.itemCount << std::endl;
		exit(ctx);
		new(this) Stop2;
		entry(ctx);

	}

	void ForwardSlow2::eventItemLeftEntry(TransportContext2& ctx){
		if(!ctx.data.section1.empty()){
			ctx.data.section1.back().timer->start(ctx.data.entryTime+1000);
		}
		else{
			std::cout<<"Queue 1 leer Festo1" << std::endl;
		}
	}

	void ForwardSlow2::eventItemLeftExitRemote(TransportContext2& ctx){
		ctx.data.addItem();
	}
}

