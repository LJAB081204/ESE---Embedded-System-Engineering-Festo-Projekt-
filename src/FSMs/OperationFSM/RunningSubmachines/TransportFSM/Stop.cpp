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
			if(ctx.data.timerIS == nullptr){
				ctx.data.timerIS = new DynamicTimer(102,*ctx.data.handler);
			}
			ctx.data.timerIS->start(1000,0);
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
}
