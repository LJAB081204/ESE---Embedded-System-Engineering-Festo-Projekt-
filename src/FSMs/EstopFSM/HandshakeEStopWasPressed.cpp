/*
 * HandshakeEStopWasPressed.cpp
 *
 *  Created on: 17.06.2026
 *      Author: mika2
 */

#include "HandShakeEStopWasPressed.h"
#include "EStopState.h"
#include "EstopContext.h"
#include "EStopRemote.h"
#include "BothReleased.h"


HandShakeEStopWasPressed::HandShakeEStopWasPressed(){

}

void HandShakeEStopWasPressed::entry(EStopContext& ctx){
	//Handshake Pulse senden
}

void HandShakeEStopWasPressed::exit(EStopContext& ctx){

}
void HandShakeEStopWasPressed::eventResponseEStopIsPressed(EStopContext& ctx){
	exit(ctx);
	new (this) EStopRemote;
	entry(ctx);
}
void HandShakeEStopWasPressed::eventResponseEStopIsNotPressed(EStopContext& ctx){
	exit(ctx);
	new (this) BothReleased;
	entry(ctx);
}


