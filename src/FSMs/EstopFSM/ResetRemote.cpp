/*
 * ResetRemote.cpp
 *
 *  Created on: 08.06.2026
 *      Author: tayla
 */
#include <iostream>
#include "ResetRemote.h"
#include "EStopState.h"
#include "EStopContext.h"
#include "EStopRemote.h"
#include "BothEStop.h"
#include "Reset.h"


ResetRemote::ResetRemote(){
}

void ResetRemote::entry(EStopContext& ctx){
	ctx.setActive(true);
}

void ResetRemote::exit(EStopContext& ctx){
	ctx.hal.getActors()->ledButtonResetOff();
	ctx.data.updateSequence();
	ctx.data.reset();
}
void ResetRemote::eventButtonEstopPressedRemote(EStopContext& ctx){
	exit(ctx);
	new (this) EStopRemote;
	entry(ctx);
}
void ResetRemote::eventButtonEstopPressed(EStopContext& ctx){
	exit(ctx);
	new (this) EStopState;
	entry(ctx);
}
void ResetRemote::eventButtonResetPressed(EStopContext& ctx){
	exit(ctx);
	ctx.setActive(false);
}



