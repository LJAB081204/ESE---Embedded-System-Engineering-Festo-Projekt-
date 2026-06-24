/*
 * ConnectionLostNormal.cpp
 *
 *  Created on: 17.06.2026
 *      Author: mika2
 */

#include "ConnectionLostContext.h"
#include "ConnectionLostNormal.h"

#include "ConnectionLostEStopIsPressed.h"

using namespace std;

ConnectionLostNormal::ConnectionLostNormal(){
}

void ConnectionLostNormal::entry(ConnectionLostContext& ctx){
	ctx.estopState = EStopButtonState::WASNOTPRESSED;
	cout << "Connection Lost" << endl;
}

void ConnectionLostNormal::exit(ConnectionLostContext& ctx){

}

void ConnectionLostNormal::eventButtonEstopPressed(ConnectionLostContext& ctx){
	exit(ctx);
	new (this) ConnectionLostEStopIsPressed;
	entry(ctx);
}

void ConnectionLostNormal::eventConnectionRestored(ConnectionLostContext& ctx){
}



