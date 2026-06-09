/*
 * SlowWaitForFesto.cpp
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#include "TransportBase.h"
#include "TransportContext.h"
#include "ForwardSlow.h"
#include "SlowWaitForFesto.h"

#include <iostream>

	SlowWaitForFesto::SlowWaitForFesto(){
	}

	void SlowWaitForFesto::entry(TransportContext& ctx){
		std::cout<<"SlowWaitForFesto" << std::endl;
		ctx.hal.getActors()->motorStop();
	}

	void SlowWaitForFesto::exit(TransportContext& ctx){
	}


	void SlowWaitForFesto::eventFestoReady(TransportContext& ctx){
		exit(ctx);
//		if(data.getReadyForItemRemote()){					//Datenklasse fehlt noch
		new(this) ForwardSlow;
//		}
		entry(ctx);
	}

	void SlowWaitForFesto::eventFestoNotReady(TransportContext& ctx){
		exit(ctx);
//		if(!data.getReadyForItemRemote()){					//Datenklasse fehlt noch
		new(this) SlowWaitForFesto;
//		}
		entry(ctx);
	}


