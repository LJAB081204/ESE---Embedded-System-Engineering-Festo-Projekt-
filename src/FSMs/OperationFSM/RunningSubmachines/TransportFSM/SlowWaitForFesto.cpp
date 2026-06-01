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

	SlowWaitForFesto::SlowWaitForFesto(){
	}

	void SlowWaitForFesto::entry(TransportContext& ctx){
		ctx.hal.getActors()->motorStop();
	}

	void SlowWaitForFesto::exit(TransportContext& ctx){
	}

	void SlowWaitForFesto::eventItemAtSlideRemote(TransportContext& ctx){
//		if(data.getReadyForItemRemote()){					//Datenklasse fehlt noch
//			entry(ctx);
//			new(this) ForwardSlow;
//			exit(ctx);
//		}
	}

	void SlowWaitForFesto::eventItemLeftExitRemote(TransportContext& ctx){
//		if(data.getReadyForItemRemote()){					//Datenklasse fehlt noch
//			entry(ctx);
//			new(this) ForwardSlow;
//			exit(ctx);
//		}
	}


