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
#include "../../PulseMessages.h"

using namespace std;


HandShakeEStopIsPressed::HandShakeEStopIsPressed(){
}

void HandShakeEStopIsPressed::entry(EStopContext& ctx){
	MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_HANDSHAKE_ESTOP_STATUS, 0);
}

void HandShakeEStopIsPressed::exit(EStopContext& ctx){

}
void HandShakeEStopIsPressed::eventResponseEStopIsPressed(EStopContext& ctx){
	exit(ctx);
	new (this) BothEStop;
	entry(ctx);
}
void HandShakeEStopIsPressed::eventResponseEStopWasPressed(EStopContext& ctx){
	exit(ctx);
	new (this) EStopState;
	entry(ctx);
}

void HandShakeEStopIsPressed::eventResponseEStopWasNotPressed(EStopContext& ctx){
	exit(ctx);
	new (this) EStopState;
	entry(ctx);
}

