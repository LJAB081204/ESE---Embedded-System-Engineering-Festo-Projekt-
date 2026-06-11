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
namespace M1{

	Forward::Forward(){
	}

	void Forward::entry(TransportContext& ctx){
		std::cout<<"Forward" << std::endl;
		ctx.hal.getActors()->motorStop();
		ctx.hal.getActors()->motorForwards();
	}

	void Forward::exit(TransportContext& ctx){
	}

	void Forward::eventItemAtExit(TransportContext& ctx){
		exit(ctx);
		raiseRequestTransfer(ctx);
		new(this) WaitForFesto;
		entry(ctx);
	}


	void Forward::eventItemAtEntry(TransportContext& ctx){
		exit(ctx);
		ctx.data.addItem();
		std::cout<< ctx.data.itemCount << std::endl;
		entry(ctx);
	}

	void Forward::eventItemArrived(TransportContext& ctx){
		exit(ctx);
		new(this) ForwardSlow;
		entry(ctx);
	}


	void Forward::eventItemAtEntryRemote(TransportContext& ctx){
		exit(ctx);
		ctx.data.removeItem(3);
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
	void Forward::raiseRequestTransfer(TransportContext& ctx){
		MsgSendPulse(ctx.data.controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_TRANSFER_REQUEST,0);

	}
}





