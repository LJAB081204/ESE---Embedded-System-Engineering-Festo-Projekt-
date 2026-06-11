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
		if(ctx.data.itemCount != 0){
			new(this) Forward;		//Datenklasse fehlt noch
		}
		else{
			new(this) Stop;
		}
		entry(ctx);
	}

	void ForwardSlow::eventItemAtExit(TransportContext& ctx){
		exit(ctx);
//		if(!ctx.data.readyForItemRemote){ (raise requestTransfer)
//		Datenklasse fehlt noch
			new(this) SlowWaitForFesto;
//		}
		entry(ctx);
	}

	void ForwardSlow::eventItemAtEntry(TransportContext& ctx){
		ctx.data.itemCount = ctx.data.itemCount + 1;
		std::cout<< ctx.data.itemCount << std::endl;
	}

	void ForwardSlow::eventItemInSlide(TransportContext& ctx){
		ctx.data.itemCount = ctx.data.itemCount - 1;
		std::cout<< ctx.data.itemCount << std::endl;
		if(ctx.data.itemCount == 0){
			new(this) Stop;
		}
	}
}
