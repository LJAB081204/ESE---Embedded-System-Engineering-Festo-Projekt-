/*
 * HandShakeEStopLocalPressed.cpp
 *
 *  Created on: 09.06.2026
 *      Author: tayla
 */


#include "HandShakeEStopIsPressed.h"
#include "EstopContext.h"
#include "EStopState.h"
#include "BothEStop.h"


HandShakeEStopIsPressed::HandShakeEStopIsPressed(){
}

void HandShakeEStopIsPressed::entry(EStopContext& ctx){
	//Handshake Pulse senden
}

void HandShakeEStopIsPressed::exit(EStopContext& ctx){

}
void HandShakeEStopIsPressed::eventResponseEStopIsPressed(EStopContext& ctx){
	exit(ctx);
	new (this) BothEStop;
	entry(ctx);
}
void HandShakeEStopIsPressed::eventResponseEStopIsNotPressed(EStopContext& ctx){
	exit(ctx);
	new (this) EStopState;
	entry(ctx);
}

