/*
 * ConnectionLostEStopWasPressed.cpp
 *
 *  Created on: 17.06.2026
 *      Author: mika2
 */

#include "ConnectionLostContext.h"
#include "ConnectionLostEStopWasPressed.h"
#include "ConnectionLostEStopIsPressed.h"

ConnectionLostEStopWasPressed::ConnectionLostEStopWasPressed(){

}

void ConnectionLostEStopWasPressed::entry(ConnectionLostContext& ctx){
	ctx.estopState = EStopButtonState::WASPRESSED;
}

void ConnectionLostEStopWasPressed::exit(ConnectionLostContext& ctx){

}

void ConnectionLostEStopWasPressed::eventButtonEstopPressed(ConnectionLostContext& ctx){
	exit(ctx);
	new (this) ConnectionLostEStopIsPressed;
	entry(ctx);
}

void ConnectionLostEStopWasPressed::eventConnectionRestored(ConnectionLostContext& ctx){

}


