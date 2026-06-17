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
namespace M1{

IS::IS(){};

void IS::entry(ProfileContext& ctx) {
	std::cout << "//////////////////IS detected" << std::endl ;
	//Datenklasse
	//ErrorCode setzten
}

void IS::exit(ProfileContext& ctx) {}

void IS::raiseError(ProfileContext& ctx){
	ctx.data.err = ITEM_SPECIAL;
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
}
