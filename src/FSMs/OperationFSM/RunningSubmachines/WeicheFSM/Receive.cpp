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
#include "LetItemPass.h"
#include "PutItemInSlide.h"
#include "WeicheBase.h"
#include "WeicheContext.h"
#include "../../../../PulseMessages.h"

namespace M1{

Receive::Receive(){

}

void Receive::entry(WeicheContext& ctx){
//	std::cout << "READY TO RECEIVE ITEM" << std::endl;
	if(ctx.data.itemPusher == true){
		ctx.data.itemPusher = false;
//		eventItemAtPusher(ctx);
		eventTimerExpired(ctx);
//		ctx.hal.getActors()->sortItemOut();
		return;
	}
	if(ctx.data.itemLeftPusher == true){
		ctx.data.itemLeftPusher = false;
		exit(ctx);
		new(this) LetItemPass;
		entry(ctx);
		eventItemLeftPusher(ctx);
		return;
	}
	ctx.hal.getActors()->closePusher();
}
void Receive::exit(WeicheContext& ctx){

}

void Receive::eventItemAtPusher(WeicheContext& ctx){
	if(ctx.hal.getActors()->isJunction()){
		ctx.pusherTimer.start(ctx.data.metalDelayJunction,0);
	}
	else {
		ctx.pusherTimer.start(ctx.data.meatlDelayPusher,0);
	}
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

