/*
 * HandShakeEStopLocalNotPressed.cpp
 *
 *  Created on: 09.06.2026
 *      Author: tayla
 */


#include "HandShakeEStopLocalNotPressed.h"
#include "EStopState.h"
#include "EstopContext.h"
#include "EStopRemote.h"
#include "BothReleased.h"

HandShakeEStopLocalNotPressed::HandShakeEStopLocalNotPressed(){
}

void HandShakeEStopLocalNotPressed::entry(EStopContext& ctx){
	ctx.hal.getActors()->motorStop();
	ctx.hal.getActors()->closePusher();
	ctx.hal.getActors()->redLampOn();
	ctx.setActive(true);
}

void HandShakeEStopLocalNotPressed::exit(EStopContext& ctx){

}
void HandShakeEStopLocalNotPressed::handshakeEStopRemoteIsPressed(EStopContext& ctx){
	exit(ctx);
	new (this) EStopRemote;
	entry(ctx);
}
void HandShakeEStopLocalNotPressed::handshakeEStopRemoteIsNotPressed(EStopContext& ctx){
	exit(ctx);
	new (this) BothReleased;
	entry(ctx);
}



