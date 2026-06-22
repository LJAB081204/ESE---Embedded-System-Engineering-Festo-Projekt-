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
    std::cout << "Wait::entry - timer start" << std::endl;
	ctx.pusherTimer.start(750);

}

void Wait::exit(WeicheContext& ctx){
	ctx.hal.getActors()->closePusher();
}
void Wait::eventTimerExpired(WeicheContext& ctx){
    std::cout << "Wait::entry - timer end" << std::endl;
	exit(ctx);
	new (this) Receive;
	entry(ctx);
}
}

