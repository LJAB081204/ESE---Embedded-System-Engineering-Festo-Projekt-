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
		std::cout<<"Forward" << std::endl;
		ctx.hal.getActors()->motorStop();
		ctx.hal.getActors()->motorForwards();
		}

		ctx.data.handler->setNormal();
	}

	void Forward::exit(TransportContext& ctx){
	}

	void Forward::eventItemAtExit(TransportContext& ctx){
		exit(ctx);
		if(!ctx.data.section3.empty()){
			if(ctx.data.exitTime - ctx.data.section3.front().timer->getTime()>1500){
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 3;
				raiseError(ctx);
				new(this) WaitForFesto;
				entry(ctx);

			}
			else{
				raiseRequestTransfer(ctx);
				new(this) WaitForFesto;
				entry(ctx);
			}
		}


	}


	void Forward::eventItemAtEntry(TransportContext& ctx){
		exit(ctx);
		std::cout<<"ITEM AT ENTRY"<< std::endl;
		ctx.data.addItem();
		std::cout<< ctx.data.itemCount << std::endl;
		entry(ctx);
	}

	void Forward::eventItemLeftEntry(TransportContext& ctx){
		if(!ctx.data.section1.empty()){
			ctx.data.section1.back().timer->start(ctx.data.entryTime);
		}
		else{
			std::cout<<"Queue 1 leer Festo1" << std::endl;
		}
	}
	void Forward::eventItemArrived(TransportContext& ctx){
		exit(ctx);
		if(!ctx.data.section1.empty()){
			if(ctx.data.entryTime - ctx.data.section1.front().timer->getTime()>1500){
				std::cout<< "Error item arrived"<< std::endl;
				delete ctx.data.section1.front().timer;
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 1;
				raiseError(ctx);
			}
			else{
				delete ctx.data.section1.front().timer;
				ctx.data.section1.front().timer = new DynamicTimer(ctx.data.section1.front().id, *ctx.data.handler);
				ctx.data.section1.front().timer->start(ctx.data.heightTime);
				new(this) ForwardSlow;
				entry(ctx);
			}
		}

	}

	void Forward::eventItemGone(TransportContext& ctx){
		if(!ctx.data.section2.empty()){

		}
	}

	void Forward::eventItemAtEntryRemote(TransportContext& ctx){
		exit(ctx);
		itemData item = ctx.data.section3.front();

		uint32_t itemCode = ctx.data.convertItemToInt(item);
		std::cout<< itemCode<< std::endl;

		MsgSendPulse(ctx.data.controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_ITEM_TRANSFER,itemCode);
		if(!ctx.data.section3.empty()){
			ctx.data.removeItem(3);
		}
		if(ctx.data.itemCount == 0){
			new(this) Stop;
		}
		entry(ctx);
	}

	void Forward::eventItemInSlide(TransportContext& ctx){
		exit(ctx);
		ctx.data.removeItem(2);
		if(ctx.data.itemCount == 0){
			new(this) Stop;
		}
		entry(ctx);
	}

	void Forward::eventItemAtPusher(TransportContext& ctx){
		exit(ctx);
		if(!ctx.data.section2.empty()){
			if(ctx.data.pusherTime - ctx.data.section2.front().timer->getTime()>1500){
				ctx.data.err = ITEM_APPEARED;
				ctx.data.errorLoc = 2;
				raiseError(ctx);
			}
		}

	}


	void Forward::raiseRequestTransfer(TransportContext& ctx){
		MsgSendPulse(ctx.data.controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_TRANSFER_REQUEST,0);

	}
	void Forward::raiseError(TransportContext& ctx){
		MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR,0);
	}
}





