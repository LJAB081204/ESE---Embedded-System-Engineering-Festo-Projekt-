/*
 * IF.cpp
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#include "SequenceBase.h"
#include "SequenceContext.h"
#include "IF.h"
#include "IHM.h"
#include "../../../../PulseMessages.h"
#include <sys/neutrino.h>

	IF::IF(){
	}

	void IF::entry(SequenceContext& ctx){

	}

	void IF::exit(SequenceContext& ctx){

	}

	void IF::eventUpdate(SequenceContext& ctx){
		if(ctx.data.section2.front().item == IHM_PROFILE){
			exit(ctx);
			raiseItemCorrect(ctx);
			new (this) IHM;
			entry(ctx);
		}
		else{
			exit(ctx);
			raiseItemWrong(ctx);
			entry(ctx);
		}
	}

	void IF::raiseItemCorrect(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_CORRECT, 0);
	}
	void IF::raiseItemWrong(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_WRONG, 0);
	}


