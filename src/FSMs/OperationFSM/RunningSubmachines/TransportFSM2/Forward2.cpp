/*
 * Forward2.cpp
 *
 *  Created on: 01.06.2026
 *      Author: mika2
 */
#include "TransportBase2.h"
#include "TransportContext2.h"
#include "Forward2.h"
#include "Stop2.h"
#include "ForwardSlow2.h"

#include <iostream>
namespace M2{
	Forward2::Forward2(){
	}

	void Forward2::entry(TransportContext2& ctx){
		std::cout<<"Forward2" << std::endl;
		ctx.hal.getActors()->motorForwards();
	}

	void Forward2::exit(TransportContext2& ctx){
	}

	void Forward2::eventItemAtExit(TransportContext2& ctx){
		ctx.data.itemAtExit = true;
		std::cout<<"-1 Item" << std::endl;
		exit(ctx);
		new(this) Stop2;
		entry(ctx);
	}


	void Forward2::eventItemArrived(TransportContext2& ctx){
		exit(ctx);
		new(this) ForwardSlow2;
		entry(ctx);
	}


	void Forward2::eventItemInSlide(TransportContext2& ctx){
		exit(ctx);
		std::cout<<"-1 Item" << std::endl;
		if(!ctx.data.section2.empty()){
			ctx.data.removeItem(2);
		}		if(ctx.data.itemCount == 0){
			new(this) Stop2;
		}
		entry(ctx);
	}

	void Forward2::eventItemTransfer(uint32_t itemCode, TransportContext2& ctx){
		itemData item = ctx.data.convertIntToItem(itemCode);
		ctx.data.addTransferredItem(item);
	}
}






