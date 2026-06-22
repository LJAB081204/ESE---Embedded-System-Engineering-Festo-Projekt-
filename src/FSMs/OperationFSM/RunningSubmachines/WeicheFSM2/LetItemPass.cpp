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

namespace M2{

LetItemPass::LetItemPass(){
}

void LetItemPass::entry(WeicheContext& ctx){
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
	if(ctx.hal.getActors()->isJunction()){
		new (this) Wait;
		entry(ctx);
	}
	else{
		new (this) Receive;
	}
}
}

