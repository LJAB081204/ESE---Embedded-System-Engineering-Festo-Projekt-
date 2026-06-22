/*
 * Ready.cpp
 *
 *  Created on: 01.06.2026
 *      Author: tayla
 */
#include <new>
#include "WeicheBase.h"
#include "WeicheContext.h"
#include "Ready.h"
#include "LetItemPass.h"
#include "PutItemInSlide.h"
#include <sys/neutrino.h>
#include "../../../../PulseMessages.h"
namespace M2{

Ready::Ready(){
}
void Ready::raiseError(WeicheContext& ctx){
	ctx.data.err = BOTH_SLIDES_FULL;
	MsgSendPulse(ctx.data.conId,SIGEV_PULSE_PRIO_INHERIT,PULSE_EVENT_ERROR,0);
}
void Ready::eventItemCorrect(WeicheContext& ctx){
	exit(ctx);
	new (this) LetItemPass;
	entry(ctx);
}
void Ready::eventItemWrong(WeicheContext& ctx){

	if (ctx.data.slideFull == false){
		new (this) PutItemInSlide;
	  	entry(ctx);
	}
	else if(ctx.data.slideFull == true){
		exit(ctx);
		raiseError(ctx);
		new(this) Receive;
	  	entry(ctx);
	}
}


}

