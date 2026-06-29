/*
 * Wait.cpp
 *
 *  Created on: 02.06.2026
 *      Author: tayla
 */

#include <new>
#include "WeicheBase.h"
#include "WeicheContext.h"
#include "Ready.h"
#include "Wait.h"
#include <iostream>
namespace M1{

Wait::Wait(){

}
void Wait::entry(WeicheContext& ctx){
	ctx.data.weiche = WAIT_STATE;
	ctx.pusherTimer.start((ctx.data.exitTime / 5),0);

}

void Wait::exit(WeicheContext& ctx){
	ctx.hal.getActors()->closePusher();
	ctx.pusherTimer.stop();
	delete ctx.timerPusherOpen;
	ctx.timerPusherOpen = nullptr;
}
void Wait::eventTimerExpired(WeicheContext& ctx){
	exit(ctx);
	new (this) Receive;
	entry(ctx);
}

}

