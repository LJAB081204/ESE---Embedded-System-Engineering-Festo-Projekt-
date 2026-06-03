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
		if(!ctx.data.readyForItemRemote){					//Datenklasse fehlt noch
			exit(ctx);
			new(this) WaitForFesto;
			entry(ctx);
		}
		new (this) Stop;
		entry(ctx);

	}

	void Forward::eventItemArrived(TransportContext& ctx){
		exit(ctx);
		new(this) ForwardSlow;
		entry(ctx);
	}





