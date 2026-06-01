/*
 * WaitForFesto.cpp
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */
#include "TransportBase.h"
#include "TransportContext.h"
#include "Forward.h"
#include "WaitForFesto.h"

	WaitForFesto::WaitForFesto(){
	}

	void WaitForFesto::entry(TransportContext& ctx){
		ctx.hal.getActors()->motorStop();
	}

	void WaitForFesto::exit(TransportContext& ctx){
	}

	void WaitForFesto::eventItemAtSlideRemote(TransportContext& ctx){
//		if(data.getReadyForItemRemote()){					//Datenklasse fehlt noch
//			entry(ctx);
//			new(this) Forward;
//			exit(ctx);
//		}
	}

	void WaitForFesto::eventItemLeftExitRemote(TransportContext& ctx){
//		if(data.getReadyForItemRemote()){					//Datenklasse fehlt noch
//			entry(ctx);
//			new(this) Forward;
//			exit(ctx);
//		}
	}


