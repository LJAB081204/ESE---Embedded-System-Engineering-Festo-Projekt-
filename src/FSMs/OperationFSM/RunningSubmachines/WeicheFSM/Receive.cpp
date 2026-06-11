/*
 * Receive.cpp
 *
 *  Created on: 01.06.2026
 *      Author: tayla
 */
#include <new>
#include <iostream>
#include <sys/neutrino.h>
#include "Receive.h"
#include "Ready.h"
#include "WeicheBase.h"
#include "WeicheContext.h"
#include "../../../../PulseMessages.h"

namespace M1{

Receive::Receive(){

}

void Receive::entry(WeicheContext& ctx){
	ctx.hal.getActors()->closePusher();

}
void Receive::exit(WeicheContext& ctx){

}

void Receive::eventItemAtPusher(WeicheContext& ctx){
	exit(ctx);
	raiseUpdate(ctx);
	new (this) Ready;
	entry(ctx);
}
void Receive::eventItemMetal(WeicheContext& ctx){
	if(!ctx.data.section2.empty()){
			ctx.data.section2.front().metal = true;
	}
	std::cout << "ITEM DURCHLASSEN";
	ctx.hal.getActors()->openPusher();
	sleep(1);
	exit(ctx);
	new (this) Receive;
	entry(ctx);
}
void Receive::raiseUpdate(WeicheContext& ctx){
	MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_UPDATE,0);
}

}

