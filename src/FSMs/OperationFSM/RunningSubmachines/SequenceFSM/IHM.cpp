/*
 * IHM.cpp
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#include "SequenceBase.h"
#include "SequenceContext.h"
#include "IHM.h"
#include "IH.h"
#include "../../../../PulseMessages.h"
#include <sys/neutrino.h>

	IHM::IHM(){
	}

	void IHM::entry(SequenceContext& ctx){

	}

	void IHM::exit(SequenceContext& ctx){

	}

	void IHM::eventUpdate(SequenceContext& ctx){
		//if IHM == true -> IH && raise eventItemCorrect
		//else raise eventItemWrong
		if(ctx.data.section2.front().item == IH_PROFILE){
			exit(ctx);
			raiseItemCorrect(ctx);
			new (this) IH;
			entry(ctx);
		}
		else{
			exit(ctx);
			raiseItemWrong(ctx);
			entry(ctx);
		}
}

	void IHM::raiseItemCorrect(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_CORRECT, 0);
	}
	void IHM::raiseItemWrong(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_WRONG, 0);
	}


