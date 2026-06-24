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
		ctx.hal.getActors()->motorStop();
		ctx.data.handler->pauseAll();
	}

	void Stop2::exit(TransportContext2& ctx){}

	void Stop2::eventItemLeftExitRemote(TransportContext2& ctx){
		exit(ctx);
		ctx.data.addItem();
		new(this) Forward2;
		entry(ctx);
	}

	void Stop2::eventItemLeftExit(TransportContext2& ctx){
		ctx.data.itemAtExit = false;
//		std::cout<< "STATE OF TRANSFER" << std::endl;
//		std::cout<< ctx.data.itemAtExit << std::endl;
//		std::cout<< ctx.data.itemCount << std::endl;
//		std::cout<< ctx.data.section1.size() << std::endl;
		ctx.data.removeItem(3);
 		if(ctx.data.itemCount != 0){
 			exit(ctx);
 			new(this) Forward2;
 			entry(ctx);
		}
	}

}
