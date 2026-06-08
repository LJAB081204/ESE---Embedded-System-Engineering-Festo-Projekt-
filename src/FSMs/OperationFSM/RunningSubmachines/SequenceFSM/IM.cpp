/*
 * IM.cpp
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#include "SequenceBase.h"
#include "SequenceContext.h"
#include "IM.h"
#include "IF.h"
#include "../../../../PulseMessages.h"
#include <sys/neutrino.h>

	IM::IM(){
	}

	void IM::entry(SequenceContext& ctx){

	}

	void IM::exit(SequenceContext& ctx){

	}

	void IM::eventUpdate(SequenceContext& ctx){
		if(ctx.data.section2.front().item == IF_PROFILE){
		    exit(ctx);
			raiseItemCorrect(ctx);
			new (this) IF;
			entry(ctx);
		}
		else{
			exit(ctx);
			raiseItemWrong(ctx);
			entry(ctx);
		}
	}



	void IM::raiseItemCorrect(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_CORRECT, 0);
	}
	void IM::raiseItemWrong(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_WRONG, 0);
	}


