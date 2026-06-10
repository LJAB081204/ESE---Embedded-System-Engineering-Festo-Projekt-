/*
 * Reset.cpp
 *
 *  Created on: 08.06.2026
 *      Author: tayla
 */

#include "EStopState.h"
#include "EStopContext.h"
#include "EStopRemote.h"
#include "BothEStop.h"
#include "Reset.h"

Reset::Reset(){
}

void Reset::entry(EStopContext& ctx){
	ctx.hal.getActors()->ledButtonResetOff();
}

void Reset::exit(EStopContext& ctx){
	ctx.hal.getActors()->ledButtonResetOff();
}
void Reset::eventButtonEstopPressedRemote(EStopContext& ctx){
	exit(ctx);
	new (this) EStopRemote;
	entry(ctx);
}
void Reset::eventButtonEstopPressed(EStopContext& ctx){
	exit(ctx);
	new (this) EStopState;
	entry(ctx);
}


void Reset::eventButtonResetPressedRemote(EStopContext& ctx){
	exit(ctx);
	ctx.setActive(false);
}



