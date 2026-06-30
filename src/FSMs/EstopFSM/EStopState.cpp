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
#include <string>

EStopState::EStopState(){
}

void EStopState::entry(EStopContext& ctx){
	ctx.hal.getActors()->motorStop();
	ctx.hal.getActors()->closePusher();
	ctx.hal.getActors()->redLampOn();
	ctx.hal.getActors()->greenLampBlinkOff();
	ctx.hal.getActors()->yellowLampBlinkOff();
	ctx.hal.getActors()->ledButtonResetOff();
	ctx.hal.getActors()->redLampBlinkOff();
	ctx.hs.stopCalibration();
	ctx.setActive(true);
	std::string machine;
	if(ctx.data.isMachine1){
		machine = "Anlage 1";
	}
	else{
		machine = "Anlage 2";
	}
	std::cout << "E-Stop auf " << machine <<" betätigt!" << std::endl;
	std::cout << "E-Stop auf " << machine <<" rausziehen." << std::endl;
}

void EStopState::exit(EStopContext& ctx){

}
void EStopState::eventButtonEstopPressedRemote(EStopContext& ctx){
	exit(ctx);
	new (this) BothEStop;
	entry(ctx);
}
void EStopState::eventButtonEstopReleased(EStopContext& ctx){
	exit(ctx);
	new (this) BothReleased;
	entry(ctx);
}

void EStopState::eventGoToHandshakeEStopIsPressed(EStopContext& ctx){
	exit(ctx);
	new (this) HandShakeEStopIsPressed;
	entry(ctx);
}

void EStopState::eventGoToHandshakeEStopWasPressed(EStopContext& ctx){
	exit(ctx);
	new (this) HandShakeEStopWasPressed;
	entry(ctx);
}

void EStopState::eventGoToHandshakeEStopWasNotPressed(EStopContext& ctx){
	exit(ctx);
	new (this) HandShakeEStopWasNotPressed;
	entry(ctx);
}



