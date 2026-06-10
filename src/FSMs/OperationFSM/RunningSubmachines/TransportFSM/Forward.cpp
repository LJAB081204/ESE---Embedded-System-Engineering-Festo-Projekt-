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

#include <iostream>

	Forward::Forward(){
	}

	void Forward::entry(TransportContext& ctx){
		std::cout<<"Forward" << std::endl;
		ctx.hal.getActors()->motorForwards();
		std::cout<< ctx.data.itemCount << std::endl;
	}

	void Forward::exit(TransportContext& ctx){
	}

	void Forward::eventItemAtExit(TransportContext& ctx){
		exit(ctx);
//		if(!ctx.data.readyForItemRemote){ (raise requestTransfer
			//Datenklasse fehlt noch
//			new(this) WaitForFesto;
//		}
		entry(ctx);

	}

	void Forward::eventItemAtEntry(TransportContext& ctx){
		exit(ctx);
		ctx.data.itemCount = ctx.data.itemCount + 1;
		std::cout<< ctx.data.itemCount << std::endl;
		entry(ctx);
	}

	void Forward::eventItemArrived(TransportContext& ctx){
		exit(ctx);
		new(this) ForwardSlow;
		entry(ctx);
	}


	void Forward::eventItemLeftExit(TransportContext& ctx){
		exit(ctx);
		ctx.data.itemCount = ctx.data.itemCount - 1;
		std::cout<< ctx.data.itemCount << std::endl;
		if(ctx.data.itemCount == 0){
			new(this) Stop;
		}
		entry(ctx);
	}

	void Forward::eventItemInSlide(TransportContext& ctx){
			exit(ctx);
			ctx.data.itemCount = ctx.data.itemCount - 1;
			std::cout<< ctx.data.itemCount << std::endl;
			if(ctx.data.itemCount == 0){
				new(this) Stop;
			}
			entry(ctx);
		}

	//Fehlt noch in MainContext
	void Forward::eventFestoEmpty(TransportContext& ctx){
		exit(ctx);
		new(this) Stop;
		entry(ctx);
	}







