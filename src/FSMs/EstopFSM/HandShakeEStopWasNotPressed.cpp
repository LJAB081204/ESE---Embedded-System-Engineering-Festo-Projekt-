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
#include "../../PulseMessages.h"

using namespace std;

HandShakeEStopWasNotPressed::HandShakeEStopWasNotPressed(){
}

void HandShakeEStopWasNotPressed::entry(EStopContext& ctx){
	MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_HANDSHAKE_ESTOP_STATUS, 0);
	cout << "Handshake EStop Was Not Pressed" << endl;
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
	exit(ctx);
	ctx.setActive(false); //KI
}
