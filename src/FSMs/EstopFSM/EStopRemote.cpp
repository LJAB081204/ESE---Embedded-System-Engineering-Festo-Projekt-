/*
 * EStop.cpp
 *
 *  Created on: 08.06.2026
 *      Author: tayla
 */

#include "EStopState.h"
#include "EStopContext.h"
#include "EStopRemote.h"
#include "BothEStop.h"
#include "BothReleased.h"
#include "Reset.h"

EStopRemote::EStopRemote(){
}

void EStopRemote::entry(EStopContext& ctx){
	ctx.hal.getActors()->motorStop();
	ctx.hal.getActors()->closePusher();
	ctx.hal.getActors()->redLampOn();
	ctx.hal.getActors()->ledButtonResetOff();
	ctx.hs.stopCalibration();
	ctx.setActive(true);
}

void EStopRemote::exit(EStopContext& ctx){

}
void EStopRemote::eventButtonEstopPressed(EStopContext& ctx){
	exit(ctx);
	new (this) BothEStop;
	entry(ctx);
}
void EStopRemote::eventButtonEstopReleasedRemote(EStopContext& ctx){
	exit(ctx);
	new (this) BothReleased;
	entry(ctx);
}




