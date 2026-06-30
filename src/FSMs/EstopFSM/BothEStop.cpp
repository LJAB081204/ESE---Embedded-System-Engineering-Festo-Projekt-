/*
 * BothEStop.cpp
 *
 *  Created on: 08.06.2026
 *      Author: tayla
 */


#include "EStopState.h"
#include "EStopContext.h"
#include "EStopRemote.h"
#include "BothEStop.h"
#include "Reset.h"
#include <iostream>
#include <string.h>


BothEStop::BothEStop(){
}

void BothEStop::entry(EStopContext& ctx){
	ctx.hal.getActors()->motorStop();
	ctx.hal.getActors()->closePusher();
	ctx.hal.getActors()->redLampBlinkOff();
	ctx.hal.getActors()->redLampOn();
	ctx.setActive(true);
	std::string machine;
	if(ctx.data.isMachine1){
		machine = "Anlage 1";
	}
	else{
		machine = "Anlage 2";
	}
	std::cout << "E-Stop auf beiden Anlagen gedrückt!" << std::endl;
}

void BothEStop::exit(EStopContext& ctx){

}
void BothEStop::eventButtonEstopReleasedRemote(EStopContext& ctx){
	exit(ctx);
	new (this) EStopState;
	entry(ctx);
}
void BothEStop::eventButtonEstopReleased(EStopContext& ctx){
	exit(ctx);
	new (this) EStopRemote;
	entry(ctx);
}



