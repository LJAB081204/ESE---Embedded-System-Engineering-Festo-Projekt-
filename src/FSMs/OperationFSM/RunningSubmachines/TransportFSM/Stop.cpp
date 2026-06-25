/*
 * Stop.cpp
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */
#include "TransportBase.h"
#include "TransportContext.h"
#include "Stop.h"
#include "Forward.h"

#include <iostream>

namespace M1{

	Stop::Stop(){
	}

	void Stop::entry(TransportContext& ctx){
		ctx.hal.getActors()->motorStop();
		ctx.data.handler->pauseAll();
		if(ctx.hal.getSensors()->isItemAtEntry()){
			exit(ctx);
			ctx.data.addItem();
			new (this) Forward;
			entry(ctx);
		}
	}

	void Stop::exit(TransportContext& ctx){}

	void Stop::eventItemAtEntry(TransportContext& ctx){
		exit(ctx);
		ctx.data.addItem();
		new(this) Forward;
		entry(ctx);
		if(ctx.data.timerIS == nullptr){
			ctx.data.timerIS = new DynamicTimer(102,*ctx.data.handler);
		}
		ctx.data.timerIS->start(1000,0);
	}
	void Stop::eventItemArrived(TransportContext& ctx){
		ctx.hal.getActors()->motorStop();
		std::cout<< "Error item arrived"<< std::endl;
		ctx.data.err = ITEM_APPEARED;
		ctx.data.errorLoc = 1;
		ctx.data.raiseError();
		}
		else{
			ctx.data.handler->pauseAll();
			if(!ctx.data.section1.empty()){
				ctx.data.section1.front().timer->stop();
			}
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 1;
			ctx.data.raiseError();
		}
	}
	void Stop::eventItemAtPusher(TransportContext& ctx){
		exit(ctx);
		if(!ctx.data.section2.empty()){
			std::cout<< "pusher time: "<<ctx.data.pusherTime << std::endl;
			std::cout<< "getTime: "<<ctx.data.section2.front().timer->getTime() << std::endl;
			int diff = ctx.data.pusherTime - ctx.data.section2.front().timer->getTime();
			std::cout<< diff << std::endl;
			if(diff>ctx.data.minPusherTime){
				ctx.hal.getActors()->motorStop();
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
			ctx.hal.getActors()->motorStop();
			ctx.data.handler->pauseAll();
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 2;
			ctx.data.raiseError();
		}
	}
	void Stop::eventItemInSlide(TransportContext& ctx){
		if(!ctx.data.section2.empty()){
			ctx.data.removeItem(2);
		}
		if(ctx.data.itemCount == 0){
			new(this) Stop;
		}
		entry(ctx);
	}
	void Stop::eventItemAtExit(TransportContext& ctx){
		exit(ctx);
		if(!ctx.data.section3.empty()){
			std::cout<< "exit time: "<<ctx.data.exitTime << std::endl;
			std::cout<< "getTime: "<<ctx.data.section3.front().timer->getTime() << std::endl;
			int diff = ctx.data.exitTime - ctx.data.section3.front().timer->getTime();
			std::cout<< diff << std::endl;
			if(diff>ctx.data.minExitTime){
				ctx.data.handler->pauseAll();
				std::cout<< "Error item at exit"<< std::endl;
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 3;
				ctx.data.raiseError();
				ctx.hal.getActors()->motorStop();
			}
			else{
				exit(ctx);
				if(!ctx.data.section3.empty()){
					ctx.data.section3.front().timer->stop();
					uint32_t runnedTime = ctx.data.section3.front().timer->getTime();
					ctx.data.section3.front().timer->stop();
					std::cout<<"runnedTime:"<<runnedTime<<std::endl;
					delete ctx.data.section3.front().timer;
					ctx.data.section3.front().timer = new DynamicTimer(ctx.data.section3.front().id, *ctx.data.handler);
					ctx.data.section3.front().timer->start(ctx.data.maxExitTime,runnedTime);
				}
			}
		}
		else{
			ctx.data.handler->pauseAll();
			ctx.hal.getActors()->motorStop();
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 3;
			ctx.data.raiseError();

		}
	}
}
