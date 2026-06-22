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
		ctx.hal.getActors()->motorSlow();
		ctx.data.handler->setSlow(ctx.data.slowFactor);
	}

	void ForwardSlow::exit(TransportContext& ctx){
	}

	void ForwardSlow::eventItemGone(TransportContext& ctx){
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
				new(this) Forward;
				entry(ctx);
			}
		}
		else{
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
		std::cout<< ctx.data.itemCount << std::endl;
	}

	void ForwardSlow::eventItemLeftEntry(TransportContext& ctx){
		if(!ctx.data.section1.empty()){
			ctx.data.section1.back().timer->start(ctx.data.entryTime);
		}
		else{
			std::cout<<"Queue 1 leer Festo1" << std::endl;
		}
	}

	void ForwardSlow::eventItemAtEntryRemote(TransportContext& ctx){
		exit(ctx);
		itemData item = ctx.data.section3.front();
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

	void ForwardSlow::eventItemInSlide(TransportContext& ctx){
		if(!ctx.data.section2.empty()){
			ctx.data.removeItem(2);
		}
		std::cout<< ctx.data.itemCount << std::endl;
		if(ctx.data.itemCount == 0){
			new(this) Stop;
		}
	}
	void ForwardSlow::raiseRequestTransfer(TransportContext& ctx){
			MsgSendPulse(ctx.data.controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_TRANSFER_REQUEST,0);

	}
}
