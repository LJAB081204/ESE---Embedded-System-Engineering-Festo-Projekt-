/*
 * ForwardSlow2.cpp
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */

#include "TransportBase2.h"
#include "TransportContext2.h"
#include "Forward2.h"
#include "ForwardSlow2.h"

#include <iostream>

	ForwardSlow2::ForwardSlow2(){
	}

	void ForwardSlow2::entry(TransportContext2& ctx){
		std::cout<<"ForwardSlow2" << std::endl;
		ctx.hal.getActors()->motorSlow();
	}

	void ForwardSlow2::exit(TransportContext2& ctx){
	}

	void ForwardSlow2::eventItemGone(TransportContext2& ctx){
		exit(ctx);
		if(ctx.data.itemCount != 0){
			new(this) Forward2;		//Datenklasse fehlt noch
		}
		else{
			new(this) Stop2;
		}
		entry(ctx);
	}


