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
	ctx.timerPusherOpen = new SimpleTimer(ctx.data.conId,PULSE_EVENT_ERROR_PUSHER_TIMER);
	ctx.timerPusherOpen->start(10000);

}

void LetItemPass::exit(WeicheContext& ctx){
	delete ctx.timerPusherOpen;
	ctx.timerPusherOpen = nullptr;

}
void LetItemPass::eventItemLeftPusher(WeicheContext& ctx){
	exit(ctx);
	if(!ctx.data.section2.empty()){
		ctx.data.moveFrom2To3();
	}
	new (this) Wait;
	entry(ctx);
}

void LetItemPass::reactivate(WeicheContext& ctx){
	exit(ctx);
	new (this) Receive;
	entry(ctx);
}
}

