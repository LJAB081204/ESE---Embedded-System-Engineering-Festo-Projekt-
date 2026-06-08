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
//		if(data.getItemCount() == 0){
//			new(this) Stop;
//		} else {
//			new(this) Forward;
//		}
		new(this) Forward;		//Datenklasse fehlt noch
		entry(ctx);
	}

	void ForwardSlow::eventItemAtExit(TransportContext& ctx){
		exit(ctx);
		new(this) SlowWaitForFesto;
		entry(ctx);
	}

