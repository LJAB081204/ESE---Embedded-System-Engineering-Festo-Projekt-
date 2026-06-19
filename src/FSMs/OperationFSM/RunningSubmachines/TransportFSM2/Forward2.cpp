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
//		if(ctx.data.itemCount == 0){
//			exit(ctx);
//			new(this) Stop2;
//			entry(ctx);
//		}
//		else{
		std::cout<<"Forward2" << std::endl;
		ctx.hal.getActors()->motorForwards();
//		}
	}

	void Forward2::exit(TransportContext2& ctx){
	}

	void Forward2::eventItemAtExit(TransportContext2& ctx){
		ctx.data.itemAtExit = true;
		std::cout<< ctx.data.itemCount << std::endl;
		exit(ctx);
//		if(ctx.data.section3.front().timer->getTime()>100){
//			ctx.data.err = ITEM_APPEARED;
//			ctx.data.errorLoc = 3;
//			raiseError(ctx);
//			new(this) Stop2;
//			entry(ctx);
//
//		}
//		else{
		new(this) Stop2;
		entry(ctx);
//		}
	}


	void Forward2::eventItemArrived(TransportContext2& ctx){
		exit(ctx);
//		if(ctx.data.section1.front().timer->getTime()>100){
//			ctx.data.err = ITEM_APPEARED;
//			ctx.data.errorLoc = 1;
//			raiseError(ctx);
//			new(this) ForwardSlow2;
//			entry(ctx);
//		}
//		else{
			new(this) ForwardSlow2;
			entry(ctx);
//		}
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
//		exit(ctx);
//		if(ctx.data.section2.front().timer->getTime()>100){
//			ctx.data.err = ITEM_APPEARED;
//			ctx.data.errorLoc = 2;
//			raiseError(ctx);
//		}
	}

	void Forward2::eventItemTransfer(uint32_t itemCode, TransportContext2& ctx){
		itemData item = ctx.data.convertIntToItem(itemCode);
		ctx.data.addTransferredItem(item);
	}

	void Forward2::eventItemLeftExitRemote(TransportContext2& ctx){
		ctx.data.addItem();
	}
	void Forward2::raiseError(TransportContext2& ctx){
		MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR,0);
	}
}






