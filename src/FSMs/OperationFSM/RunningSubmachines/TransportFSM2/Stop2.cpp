/*
 * Stop2.cpp
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */
#include "TransportBase2.h"
#include "TransportContext2.h"
#include "Stop2.h"

#include "Forward2.h"

#include <iostream>
namespace M2{


	Stop2::Stop2(){
	}

	void Stop2::entry(TransportContext2& ctx){
		std::cout<<"Stop2" << std::endl;
		ctx.hal.getActors()->motorStop();
	}

	void Stop2::exit(TransportContext2& ctx){
	}

	void Stop2::eventItemLeftExitRemote(TransportContext2& ctx){
		exit(ctx);
		ctx.data.itemCount = ctx.data.itemCount + 1;
		new(this) Forward2;
		entry(ctx);
	}

	void Stop2::eventItemLeftExit(TransportContext2& ctx){
		exit(ctx);
 		if(ctx.data.itemCount != 0){
 			new(this) Forward2;
		}
		entry(ctx);
	}

}
