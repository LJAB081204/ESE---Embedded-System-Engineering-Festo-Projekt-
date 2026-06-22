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
#include "../../../../PulseMessages.h"
#include "Wait.h"
namespace M2{

Wait::Wait(){

}
void Wait::entry(WeicheContext& ctx){
	ctx.pusherTimer.start(750);
	timerPusherOpen = new SimpleTimer(ctx.data.conId,PULSE_EVENT_ERROR);
	timerPusherOpen->start(20000);
}

void Wait::exit(WeicheContext& ctx){
	ctx.hal.getActors()->closePusher();
	timerPusherOpen->stop();

}
void Wait::eventTimerExpired(WeicheContext& ctx){
	exit(ctx);
	new (this) Receive;
	entry(ctx);
}
}

