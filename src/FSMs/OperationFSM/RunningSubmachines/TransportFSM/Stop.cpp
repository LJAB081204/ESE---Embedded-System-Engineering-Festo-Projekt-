/*
 * Stop.cpp
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */
#include "TransportBase.h"
#include "TransportContext.h"
#include "Stop.h"
#include "Forward.h"

#include <iostream>

namespace M1{

	Stop::Stop(){
	}

	void Stop::entry(TransportContext& ctx){
		std::cout<<"Stop" << std::endl;
		ctx.hal.getActors()->motorStop();
	}

	void Stop::exit(TransportContext& ctx){
	}

	void Stop::eventItemAtEntry(TransportContext& ctx){
		exit(ctx);
		std::cout<<"ITEM AT ENTRY"<< std::endl;
		ctx.data.addItem();
		std::cout<< ctx.data.section1.size() << std::endl;

		std::cout<< ctx.data.itemCount << std::endl;
		new(this) Forward;
		entry(ctx);
	}
}
