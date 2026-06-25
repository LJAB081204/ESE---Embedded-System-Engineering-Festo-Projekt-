/*
 * IS.cpp
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */

#include <iostream>

#include "ProfileContext.h"
#include "IS.h"
#include <sys/neutrino.h>
#include "../../../../PulseMessages.h"
#include "Default.h"

using namespace std;

namespace M2{

IS::IS(){};

void IS::entry(ProfileContext& ctx) {
	std::cout << "//////////////////IS detected" << std::endl;
	raiseGoBack(ctx);
	ctx.data.handler->pauseAll();
	raiseError(ctx);
	new (this) Default;
}

void IS::exit(ProfileContext& ctx) {}

void IS::raiseError(ProfileContext& ctx){
	ctx.data.err = ITEM_SPECIAL_1;
	ctx.data.errorLoc = 1;
	MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR,0);
}

void IS::eventItemGone(ProfileContext& ctx){
	exit(ctx);

	if(!ctx.data.section1.empty()){
		ctx.data.section1.front().special = true;
		ctx.data.moveFrom1To2();
	}

	new(this) Default;
	entry(ctx);
}

void IS::raiseGoBack(ProfileContext& ctx){
	switch (ctx.data.nextItem.front().item){
		case IH_PROFILE:
			MsgSendPulse(ctx.data.controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_GO_BACK_IHM,0);
			cout << "Go Back to IHM" << endl;
			break;
		case IM_PROFILE:
			MsgSendPulse(ctx.data.controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_GO_BACK_IH,0);
			cout << "Go Back to IH" << endl;
			break;
		case IF_PROFILE:
			MsgSendPulse(ctx.data.controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_GO_BACK_IM,0);
			cout << "Go Back to IM" << endl;
			break;
		case IHM_PROFILE:
			MsgSendPulse(ctx.data.controllerConId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_GO_BACK_IF,0);
			cout << "Go Back to IF" << endl;
			break;
		default:
			break;
	}
	ctx.data.nextItem.pop();
}


}
