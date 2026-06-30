/*
 * LetItemPass.cpp
 *
 *  Created on: 02.06.2026
 *      Author: tayla
 */

#include <new>
#include "WeicheBase.h"
#include "WeicheContext.h"
#include "Receive.h"
#include "Wait.h"
#include "LetItemPass.h"
#include "../../../../PulseMessages.h"
#include <iostream>

namespace M1{

LetItemPass::LetItemPass(){
}

void LetItemPass::entry(WeicheContext& ctx){
	std::cout<<"Let Item Pass"<<std::endl;
	ctx.data.weiche = LET_ITEM_PASS_STATE;
	ctx.hal.getActors()->letItemPass();
	if(ctx.hal.getActors()->isJunction()){
		ctx.timerPusherOpen = new SimpleTimer(ctx.data.conId,PULSE_EVENT_ERROR_PUSHER_TIMER);
		ctx.timerPusherOpen->start(20000);
	}

}

void LetItemPass::exit(WeicheContext& ctx){
	if(ctx.timerPusherOpen != nullptr){
		ctx.timerPusherOpen->stop();
		delete ctx.timerPusherOpen;
		ctx.timerPusherOpen = nullptr;
	}

}
void LetItemPass::eventItemLeftPusher(WeicheContext& ctx){
	std::cout<<"sec2:" << ctx.data.section2.size() <<std::endl;

	if(!ctx.data.section2.empty()){
		ctx.data.moveFrom2To3();
	}
	if(ctx.hal.getActors()->isJunction()){
		new (this) Wait;
		entry(ctx);
	}
	else{
		exit(ctx);
		new (this) Receive;
		entry(ctx);
	}
}

void LetItemPass::reactivate(WeicheContext& ctx){
	exit(ctx);
	new (this) Receive;
	entry(ctx);
}
}

