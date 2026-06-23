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
		ctx.hal.getActors()->motorForwards();
		ctx.hal.getActors()->motorSlow();
		ctx.data.handler->setSlow(ctx.data.slowFactor);
	}

	void ForwardSlow2::exit(TransportContext2& ctx){
	}

	void ForwardSlow2::eventItemGone(TransportContext2& ctx){
		if(!ctx.data.section1.empty()){
//			std::cout<< "height time: "<<ctx.data.heightTime << std::endl;
//			std::cout<< "getTime: "<<ctx.data.section1.front().timer->getTime() << std::endl;
//			int diff = ctx.data.heightTime - ctx.data.section1.front().timer->getTime();
//			ctx.data.section1.front().timer->stop();
//			std::cout<< diff << std::endl;
//			if(diff>1500){
//				std::cout<< "Error item gone"<< std::endl;
//				delete ctx.data.section1.front().timer;
//				ctx.data.err = ITEM_APPEARED;
//				ctx.data.errorLoc = 1;
//				ctx.data.raiseError();
//			}
//			else{
				exit(ctx);
				new(this) Forward2;
				entry(ctx);
//			}
		}
		else{
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 1;
			ctx.data.raiseError();
		}
//		new(this) Forward2;
//		entry(ctx);
	}
//	void ForwardSlow2::eventItemTransfer(uint32_t itemCode, TransportContext2& ctx){
//		itemData item = ctx.data.convertIntToItem(itemCode);
//		ctx.data.addTransferredItem(item);
//	}
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
	void ForwardSlow2::eventItemAtExit(TransportContext2& ctx){
		ctx.data.itemAtExit = true;
		std::cout<< ctx.data.itemCount << std::endl;
		exit(ctx);
		new(this) Stop2;
		entry(ctx);

	}

	void ForwardSlow2::eventItemLeftExitRemote(TransportContext2& ctx){
		ctx.data.addItem();
	}
}

