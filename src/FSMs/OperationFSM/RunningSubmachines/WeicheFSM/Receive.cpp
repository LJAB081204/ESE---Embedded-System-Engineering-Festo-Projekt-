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
	std::cout << "READY TO RECEIVE ITEM" << std::endl;
	if(!ctx.data.section2.empty()){
			ctx.data.section2.front().metal = false;
	}
}
void Receive::exit(WeicheContext& ctx){

}

void Receive::eventItemAtPusher(WeicheContext& ctx){
	ctx.pusherTimer.start(ctx.data.pusherTime/8,0);
}
void Receive::eventItemMetal(WeicheContext& ctx){
	std::cout << "section2 size: " << ctx.data.section2.size() << std::endl;
	if(!ctx.data.section2.empty()){
			ctx.data.section2.front().metal = true;
	}

}

void Receive::eventTimerExpired(WeicheContext& ctx){
	exit(ctx);
	std::cout<<"Weiche Ready"<<std::endl;
	raiseUpdate(ctx);
	new (this) Ready;
	entry(ctx);

}
void Receive::raiseUpdate(WeicheContext& ctx){
	MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_UPDATE,0);
}

}

