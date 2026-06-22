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
namespace M1{

	WaitForFesto::WaitForFesto(){
	}

	void WaitForFesto::entry(TransportContext& ctx){
		std::cout<<"WaitForFesto" << std::endl;
		ctx.data.handler->pauseAll();
	}

	void WaitForFesto::exit(TransportContext& ctx){
		ctx.data.handler->resumeAll();
	}

	void WaitForFesto::eventFestoNotReady(TransportContext& ctx){
		ctx.hal.getActors()->motorStop();
	}
	void WaitForFesto::eventFestoReady(TransportContext& ctx){
		exit(ctx);
		new(this) Forward;
		entry(ctx);
	}

	void WaitForFesto::eventItemAtEntry(TransportContext& ctx){
		ctx.data.addItem();
		std::cout<< ctx.data.itemCount << std::endl;
	}

}

