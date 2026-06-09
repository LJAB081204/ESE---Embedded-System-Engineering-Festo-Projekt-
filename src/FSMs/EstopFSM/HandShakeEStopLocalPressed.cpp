/*
 * HandShakeEStopLocalPressed.cpp
 *
 *  Created on: 09.06.2026
 *      Author: tayla
 */


#include "HandShakeEStopLocalPressed.h"
#include "EstopContext.h"
#include "EStopState.h"
#include "BothEStop.h"

HandShakeEStopLocalPressed::HandShakeEStopLocalPressed(){
}

void HandShakeEStopLocalPressed::entry(EStopContext& ctx){
	ctx.hal.getActors()->motorStop();
	ctx.hal.getActors()->closePusher();
	ctx.hal.getActors()->redLampOn();
	ctx.setActive(true);
}

void HandShakeEStopLocalPressed::exit(EStopContext& ctx){

}
void HandShakeEStopLocalPressed::handshakeEStopRemoteIsPressed(EStopContext& ctx){
	exit(ctx);
	new (this) BothEStop;
	entry(ctx);
}
void HandShakeEStopLocalPressed::handshakeEStopRemoteIsNotPressed(EStopContext& ctx){
	exit(ctx);
	new (this) EStopState;
	entry(ctx);
}

