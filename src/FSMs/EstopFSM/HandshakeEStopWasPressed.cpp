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
#include "../../PulseMessages.h"

using namespace std;

HandShakeEStopWasPressed::HandShakeEStopWasPressed(){

}

void HandShakeEStopWasPressed::entry(EStopContext& ctx){
	MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_HANDSHAKE_ESTOP_STATUS, 0);
	cout << "Handshake EStop Was Pressed" << endl;
}

void HandShakeEStopWasPressed::exit(EStopContext& ctx){

}
void HandShakeEStopWasPressed::eventResponseEStopIsPressed(EStopContext& ctx){
	exit(ctx);
	new (this) EStopRemote;
	entry(ctx);
}
void HandShakeEStopWasPressed::eventResponseEStopWasPressed(EStopContext& ctx){
	exit(ctx);
	new (this) BothReleased;
	entry(ctx);
}

void HandShakeEStopWasPressed::eventResponseEStopWasNotPressed(EStopContext& ctx){
	exit(ctx);
	new (this) BothReleased;
	entry(ctx);
}


