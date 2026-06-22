/*
 * ConnectionLostEStopIsPressed.cpp
 *
 *  Created on: 17.06.2026
 *      Author: mika2
 */

#include "ConnectionLostContext.h"
#include "ConnectionLostEStopIsPressed.h"
#include "ConnectionLostEStopWasPressed.h"
#include "../MainFSM/EStop.h"

ConnectionLostEStopIsPressed::ConnectionLostEStopIsPressed(){
}

void ConnectionLostEStopIsPressed::entry(ConnectionLostContext& ctx){
	ctx.estopState = EStopButtonState::ISPRESSED;
}

void ConnectionLostEStopIsPressed::exit(ConnectionLostContext& ctx){

}

void ConnectionLostEStopIsPressed::eventButtonEstopReleased(ConnectionLostContext& ctx){
	exit(ctx);
	new (this) ConnectionLostEStopWasPressed;
	entry(ctx);
}

void ConnectionLostEStopIsPressed::eventConnectionRestored(ConnectionLostContext& ctx){
	// in Estop
}


