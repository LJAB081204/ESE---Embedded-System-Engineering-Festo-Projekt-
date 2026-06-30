/*
 * waitingForResolve.cpp
 *
 *  Created on: 23.06.2026
 *      Author: tayla
 */


#include "ErrorBase.h"
#include "ErrorContext.h"
#include "waitingForResolve.h"
#include <iostream>
#include <string>

waitingForResolve::waitingForResolve(){}

void waitingForResolve::entry(ErrorContext& ctx){
	ctx.active = true;
	std::string machine;
	if(ctx.data.isMachine1){
		machine = "Anlage 2";
	}
	else{
		machine = "Anlage 1";
	}
    std::cout << "Error auf " << machine << "." << std::endl;
    std::cout << "Warte auf Behebung..."  << std::endl;
    ctx.hal.getActors()->motorStop();
	ctx.hal.getActors()->redLampBlinkOff();
    ctx.hal.getActors()->redLampBlink(1000);
}

void waitingForResolve::eventButtonResetPressedRemote(ErrorContext& ctx){
	ctx.hal.getActors()->redLampBlinkOff();
	ctx.hal.getActors()->redLampOn();
	ctx.resetPressed = true;
}
void waitingForResolve::eventButtonStartPressedRemote(ErrorContext& ctx){
	if(ctx.resetPressed){
		ctx.hal.getActors()->redLampBlinkOff();
		ctx.hal.getActors()->redLampBlink(1000);
	}
}

void waitingForResolve::exit(ErrorContext& ctx){
	ctx.data.errorState = CURRENT_NACK_STATE;

}

