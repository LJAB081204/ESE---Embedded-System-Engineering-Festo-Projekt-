/*
 * IH.cpp
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#include "SequenceBase.h"
#include "SequenceContext.h"
#include "IH.h"
#include "IM.h"
#include "../../../../PulseMessages.h"
#include <sys/neutrino.h>

	IH::IH(){
	}

	void IH::entry(SequenceContext& ctx){
		//nichts?
	}

	void IH::exit(SequenceContext& ctx){

	}

	void IH::eventUpdate(SequenceContext& ctx){
		//if IM == true -> IM && raise eventItemCorrect
		//else raise eventItemWrong
		if(ctx.data.section2.front().item == IM_PROFILE){
					exit(ctx);
					raiseItemCorrect(ctx);
					new (this) IM;
					entry(ctx);
				}
				else{
					exit(ctx);
					raiseItemWrong(ctx);
					entry(ctx);
				}
		}

	void IH::raiseItemCorrect(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_CORRECT, 0);
	}
	void IH::raiseItemWrong(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_WRONG, 0);
	}

