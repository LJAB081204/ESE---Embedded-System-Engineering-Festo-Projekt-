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
namespace M1{

	SlowWaitForFesto::SlowWaitForFesto(){
	}

	void SlowWaitForFesto::entry(TransportContext& ctx){
		std::cout<<"SlowWaitForFesto" << std::endl;
		ctx.hal.getActors()->motorStop();
		ctx.data.handler->pauseAll();
	}

	void SlowWaitForFesto::exit(TransportContext& ctx){}


	void SlowWaitForFesto::eventFestoReady(TransportContext& ctx){
		exit(ctx);
		new(this) ForwardSlow;
		entry(ctx);
	}

	void SlowWaitForFesto::eventItemAtEntry(TransportContext& ctx){
		ctx.data.addItem();
	}

	void SlowWaitForFesto::eventItemLeftEntry(TransportContext& ctx){
		if(!ctx.data.section1.empty()){
			ctx.data.section1.back().timer->start(ctx.data.entryTime+1000);
		}
		else{
			std::cout<<"Queue 1 leer Festo1" << std::endl;
		}
	}

	void SlowWaitForFesto::eventItemInSlide(TransportContext& ctx){
		if(!ctx.data.section2.empty()){
			delete ctx.data.section2.front().timer;
			ctx.data.section2.pop();
			ctx.data.itemCount--;
		}
	}

}
