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
		if(ctx.data.itemCount == 0){
			exit(ctx);
			new(this) Stop2;
			entry(ctx);
		}
		else{
			std::cout<<"Forward2" << std::endl;
			ctx.hal.getActors()->motorStop();
			ctx.hal.getActors()->motorForwards();
		}
	}

	void Forward2::exit(TransportContext2& ctx){
	}

	void Forward2::eventItemAtExit(TransportContext2& ctx){
		ctx.data.itemAtExit = true;
		std::cout<< ctx.data.itemCount << std::endl;
		exit(ctx);
		if(!ctx.data.section3.empty()){
		std::cout<< "exit time: "<<ctx.data.exitTime << std::endl;
		std::cout<< "getTime: "<<ctx.data.section3.front().timer->getTime() << std::endl;
		int diff = ctx.data.exitTime - ctx.data.section3.front().timer->getTime();
		std::cout<< diff << std::endl;
		if(diff>800){
			std::cout<< "Error item at exit"<< std::endl;
			delete ctx.data.section3.front().timer;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 3;
			raiseError(ctx);
			//new(this) WaitForFesto;		erstmal auskommentiert
			//entry(ctx);
		}
		/*else{
			raiseRequestTransfer(ctx);
			new(this) WaitForFesto;
			entry(ctx);
		}*/
	}
	}

	void Forward2::eventItemLeftEntry(TransportContext2& ctx){
		if(!ctx.data.section1.empty()){
			ctx.data.section1.back().timer->start(ctx.data.entryTime+500);
//			std::cout<< "getTime: "<<ctx.data.section1.front().timer->getTime() << std::endl;
		}
		else{
			std::cout<<"Queue 1 leer Festo1" << std::endl;
		}
	}


	void Forward2::eventItemArrived(TransportContext2& ctx){
		exit(ctx);
		if(!ctx.data.section1.empty()){
			std::cout<< "entry time: "<<ctx.data.entryTime << std::endl;
			std::cout<< "getTime: "<<ctx.data.section1.front().timer->getTime() << std::endl;
			int diff = ctx.data.entryTime - ctx.data.section1.front().timer->getTime();
			std::cout<< diff << std::endl;
			if(diff>500){
				std::cout<< "Error item arrived"<< std::endl;
				delete ctx.data.section1.front().timer;
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 1;
				raiseError(ctx);
			}
			else{
				delete ctx.data.section1.front().timer;
				ctx.data.section1.front().timer = new DynamicTimer(ctx.data.section1.front().id, *ctx.data.handler);
				ctx.data.section1.front().timer->start(ctx.data.heightTime+500);
				new(this) ForwardSlow2;
				entry(ctx);
			}
		}
	}


	void Forward2::eventItemInSlide(TransportContext2& ctx){
		exit(ctx);
		if(!ctx.data.section2.empty()){
			ctx.data.removeItem(2);
		}		if(ctx.data.itemCount == 0){
			new(this) Stop2;
		}
		entry(ctx);
	}

	void Forward2::eventItemAtPusher(TransportContext2& ctx){
		if(!ctx.data.section2.empty()){
			std::cout<< "pusher time: "<<ctx.data.pusherTime << std::endl;
			std::cout<< "getTime: "<<ctx.data.section2.front().timer->getTime() << std::endl;
			int diff = ctx.data.pusherTime - ctx.data.section2.front().timer->getTime();
			std::cout<< diff << std::endl;
			if(diff>500){
				std::cout<< "Error item at Pusher"<< std::endl;
				delete ctx.data.section2.front().timer;
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 2;
				raiseError(ctx);
			}
		}
	}

	void Forward2::eventItemTransfer(uint32_t itemCode, TransportContext2& ctx){
		//itemData item = ctx.data.convertIntToItem(itemCode);
		//ctx.data.addTransferredItem(item);
	}

	void Forward2::eventItemLeftExitRemote(TransportContext2& ctx){
		ctx.data.addItem();
	}
	void Forward2::raiseError(TransportContext2& ctx){
		MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR,0);
	}
}






