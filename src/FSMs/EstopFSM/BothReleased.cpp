/*
 * BothReleased.cpp
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
#include <iostream>


BothReleased::BothReleased(){
}

void BothReleased::entry(EStopContext& ctx){
	ctx.hal.getActors()->motorStop();
	ctx.hal.getActors()->closePusher();
	ctx.hal.getActors()->redLampOn();
	ctx.hal.getActors()->ledButtonReset();
	ctx.setActive(true);
	std::cout "Kein E-Stop mehr gedrückt. Beide Anlagen leeren mit Reset zurücksetzen." << std::endl;
}

void BothReleased::exit(EStopContext& ctx){

}

void BothReleased::eventButtonEstopPressedRemote(EStopContext& ctx){
	exit(ctx);
	new (this) EStopRemote;
	entry(ctx);
}
void BothReleased::eventButtonEstopPressed(EStopContext& ctx){
	exit(ctx);
	new (this) EStopState;
	entry(ctx);
}

void BothReleased::eventButtonResetPressed(EStopContext& ctx){
	exit(ctx);
	new (this) Reset;
	entry(ctx);
}void BothReleased::eventButtonResetPressedRemote(EStopContext& ctx){
	exit(ctx);
	new (this) ResetRemote;
	entry(ctx);
}

