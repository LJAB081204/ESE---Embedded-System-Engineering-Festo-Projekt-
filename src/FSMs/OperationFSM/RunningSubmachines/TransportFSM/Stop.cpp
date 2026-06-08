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
		std::cout<<"+1 Item" << std::endl;
		//ItemCount erhöhen
		new(this) Forward;
		entry(ctx);
	}

