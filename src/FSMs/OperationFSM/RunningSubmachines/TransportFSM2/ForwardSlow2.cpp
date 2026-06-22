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
		std::cout<<"ForwardSlow2" << std::endl;
		ctx.hal.getActors()->motorSlow();
		ctx.data.handler->setSlow(ctx.data.slowFactor);
	}

	void ForwardSlow2::exit(TransportContext2& ctx){
	}

	void ForwardSlow2::eventItemGone(TransportContext2& ctx){
		exit(ctx);
		if(!ctx.data.section1.empty()){
			std::cout<< "height time: "<<ctx.data.heightTime << std::endl;
			std::cout<< "getTime: "<<ctx.data.section1.front().timer->getTime() << std::endl;
			int diff = ctx.data.heightTime - ctx.data.section1.front().timer->getTime();
			std::cout<< diff << std::endl;
			if(diff>1500){
				std::cout<< "Error item gone"<< std::endl;
				delete ctx.data.section1.front().timer;
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 1;
				ctx.data.raiseError();
			}
			else{
				delete ctx.data.section1.front().timer;
				ctx.data.section1.front().timer = new DynamicTimer(ctx.data.section1.front().id, *ctx.data.handler);
				ctx.data.section1.front().timer->start(ctx.data.pusherTime+500);
				new(this) Forward2;
				entry(ctx);
			}
		}
		std::cout<< ctx.data.section1.size() << std::endl;
		//ctx.data.moveFrom1To2();
		std::cout<< ctx.data.section2.size() << std::endl;
		new (this) Forward2;
//		if(ctx.data.itemCount != 0){
//			new(this) Forward;
//		}
//		else{
//			new(this) Stop2;
//		}
		entry(ctx);
	}
	void ForwardSlow2::eventItemTransfer(uint32_t itemCode, TransportContext2& ctx){
		itemData item = ctx.data.convertIntToItem(itemCode);
		ctx.data.addTransferredItem(item);
	}
	void ForwardSlow2::eventItemInSlide(TransportContext2& ctx){
		if(!ctx.data.section2.empty()){
			ctx.data.removeItem(2);
		}
		std::cout<< ctx.data.itemCount << std::endl;
		if(ctx.data.itemCount == 0){
			new(this) Stop2;
		}
	}
	void ForwardSlow2::eventItemAtExit(TransportContext2& ctx){
		ctx.data.itemAtExit = true;
//		if(!ctx.data.section3.empty()){
//			ctx.data.removeItem(3);
//		}
		std::cout<< ctx.data.itemCount << std::endl;
		new(this) Stop2;
		entry(ctx);

	}

	void ForwardSlow2::eventItemLeftEntry(TransportContext2& ctx){
		if(!ctx.data.section1.empty()){
			ctx.data.section1.back().timer->start(ctx.data.entryTime);
		}
		else{
			std::cout<<"Queue 1 leer Festo1" << std::endl;
		}
	}

	void ForwardSlow2::eventItemLeftExitRemote(TransportContext2& ctx){
		ctx.data.addItem();
	}
}

