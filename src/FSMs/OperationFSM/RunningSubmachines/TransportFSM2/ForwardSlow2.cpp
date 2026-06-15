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
#include "Stop2.h"

#include <iostream>
namespace M2{

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
		//if(ctx.data.itemCount != 0){

		new(this) Forward2;		//Datenklasse fehlt noch
		//}

		entry(ctx);
	}
	void ForwardSlow2::eventItemTransfer(uint32_t itemCode, TransportContext2& ctx){
		itemData item = ctx.data.convertIntToItem(itemCode);
		ctx.data.addTransferredItem(item);
	}
	void ForwardSlow2::eventItemInSlide(TransportContext2& ctx){
		if(!ctx.data.section2.empty()){
			ctx.data.removeItem(2);
		}
		std::cout<< ctx.data.itemCount << std::endl;
		if(ctx.data.itemCount == 0){
			new(this) Stop2;
		}
	}
	void ForwardSlow2::eventItemAtExit(TransportContext2& ctx){
		ctx.data.itemAtExit = true;
		if(!ctx.data.section3.empty()){
			ctx.data.removeItem(3);
		}
		std::cout<< ctx.data.itemCount << std::endl;
		if(ctx.data.itemCount == 0){
			new(this) Stop2;
		}
	}
}

