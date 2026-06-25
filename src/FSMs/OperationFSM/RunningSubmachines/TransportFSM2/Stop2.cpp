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
		ctx.data.removeItem(3);
 		if(ctx.data.itemCount != 0){
 			exit(ctx);
 			new(this) Forward2;
 			entry(ctx);
		}
	}
//	void Stop2::eventItemAtEntry(TransportContext2& ctx){
//		ctx.data.isErrorLocal = true;
//		ctx.data.err = ITEM_APPEARED;
//		ctx.data.errorLoc = 0;
//		ctx.data.handler->pauseAll();
//		ctx.data.raiseError();
//	}
	void Stop2::eventItemArrived(TransportContext2& ctx){
		if(ctx.data.section1.empty()){
			std::cout<< "Error item arrived (empty)"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 1;
			ctx.data.raiseError();
		}
	}
	void Stop2::eventItemAtPusher(TransportContext2& ctx){
		if(ctx.data.section2.empty()){
			std::cout<< "Error item at Pusher"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 2;
			ctx.data.raiseError();
		}
	}
	void Stop2::eventItemAtExit(TransportContext2& ctx){
		if(ctx.data.section3.empty()){
			std::cout<< "Error item at exit"<< std::endl;
			ctx.data.err = ITEM_APPEARED;
			ctx.data.errorLoc = 3;
			ctx.data.raiseError();
		}
	}
}
