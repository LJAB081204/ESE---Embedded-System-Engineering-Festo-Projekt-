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
namespace M1{

	ForwardSlow::ForwardSlow(){
	}

	void ForwardSlow::entry(TransportContext& ctx){
		std::cout<<"ForwardSlow" << std::endl;
		ctx.hal.getActors()->motorSlow();
	}

	void ForwardSlow::exit(TransportContext& ctx){
	}

	void ForwardSlow::eventItemGone(TransportContext& ctx){
		exit(ctx);
		std::cout<< ctx.data.section1.size() << std::endl;
		//ctx.data.moveFrom1To2();
		std::cout<< ctx.data.section2.size() << std::endl;
		new (this) Forward;
//		if(ctx.data.itemCount != 0){
//			new(this) Forward;
//		}
//		else{
//			new(this) Stop;
//		}
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

	void ForwardSlow::eventItemInSlide(TransportContext& ctx){
		ctx.data.removeItem(2);
		std::cout<< ctx.data.itemCount << std::endl;
		if(ctx.data.itemCount == 0){
			new(this) Stop;
		}
	}
	void ForwardSlow::raiseRequestTransfer(TransportContext& ctx){
			MsgSendPulse(ctx.data.controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_TRANSFER_REQUEST,0);

		}
}
