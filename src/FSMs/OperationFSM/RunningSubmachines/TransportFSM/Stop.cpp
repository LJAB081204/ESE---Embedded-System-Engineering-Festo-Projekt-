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


	Stop::Stop(){
	}

	void Stop::entry(TransportContext& ctx){
		ctx.hal.getActors()->motorStop();
	}

	void Stop::exit(TransportContext& ctx){
	}

	void Stop::eventItemAtEntry(TransportContext& ctx){
		exit(ctx);
		ctx.data.addItem();
		ctx.data.addItem();
		new(this) Forward;
		entry(ctx);
	}

