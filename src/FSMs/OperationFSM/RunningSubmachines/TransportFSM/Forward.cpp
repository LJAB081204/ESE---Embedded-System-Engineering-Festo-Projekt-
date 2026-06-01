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

	Forward::Forward(){
	}

	void Forward::entry(TransportContext& ctx){
		ctx.hal.getActors()->motorForwards();
	}

	void Forward::exit(TransportContext& ctx){
	}

	void Forward::eventItemAtExit(TransportContext& ctx){
//		if(!data.getReadyForItemRemote()){					//Datenklasse fehlt noch
//			entry(ctx);
//			new(this) WaitForFesto;
//			exit(ctx);
//		}
	}

	void Forward::eventItemArrived(TransportContext& ctx){
		entry(ctx);
		new(this) ForwardSlow;
		exit(ctx);
	}





