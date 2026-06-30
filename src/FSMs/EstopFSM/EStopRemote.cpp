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
#include <iostream>


EStopRemote::EStopRemote(){
}

void EStopRemote::entry(EStopContext& ctx){
	ctx.hal.getActors()->motorStop();
	ctx.hal.getActors()->closePusher();
	ctx.hal.getActors()->redLampOn();
	ctx.hal.getActors()->greenLampBlinkOff();
	ctx.hal.getActors()->yellowLampBlinkOff();
	ctx.hal.getActors()->redLampBlinkOff();
	ctx.hal.getActors()->ledButtonResetOff();
	ctx.hs.stopCalibration();
	ctx.setActive(true);
	std::string remote = ctx.data.isMachine1 ? "Anlage 2" : "Anlage 1";
	std::cout << "E-Stop auf " << remote << " betätigt!" << std::endl;
	std::cout << "Bitte E-Stop auf " << remote << " rausziehen." << std::endl;
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




