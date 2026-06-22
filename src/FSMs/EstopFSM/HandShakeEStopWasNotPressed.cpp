/*
 * HandShakeEStopLocalNotPressed.cpp
 *
 *  Created on: 09.06.2026
 *      Author: tayla
 */


#include "HandShakeEStopWasNotPressed.h"
#include "EStopState.h"
#include "EstopContext.h"
#include "EStopRemote.h"
#include "BothReleased.h"


HandShakeEStopWasNotPressed::HandShakeEStopWasNotPressed(){
}

void HandShakeEStopWasNotPressed::entry(EStopContext& ctx){
	//Handshake Pulse senden
}

void HandShakeEStopWasNotPressed::exit(EStopContext& ctx){

}
void HandShakeEStopWasNotPressed::eventResponseEStopWasPressed(EStopContext& ctx){
	exit(ctx);
	new (this) BothReleased;
	entry(ctx);
}
void HandShakeEStopWasNotPressed::eventResponseEStopIsPressed(EStopContext& ctx){
	exit(ctx);
	new (this) EStopRemote;
	entry(ctx);
}

void HandShakeEStopWasNotPressed::eventResponseEStopWasNotPressed(EStopContext& ctx){
	// History von Operation oder Error (?)
}
