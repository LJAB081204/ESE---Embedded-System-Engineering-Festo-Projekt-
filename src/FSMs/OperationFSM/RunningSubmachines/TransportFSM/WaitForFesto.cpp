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
		ctx.hal.getActors()->motorStop();
	}

	void WaitForFesto::exit(TransportContext& ctx){
	}

	void WaitForFesto::eventFestoReady(TransportContext& ctx){
		exit(ctx);
//		if(!data.getReadyForItemRemote()){					//Datenklasse fehlt noch
		new(this) Forward;
//		}
		entry(ctx);
	}

	void WaitForFesto::eventItemAtEntry(TransportContext& ctx){
		ctx.data.itemCount = ctx.data.itemCount + 1;
		std::cout<< ctx.data.itemCount << std::endl;
	}

}

