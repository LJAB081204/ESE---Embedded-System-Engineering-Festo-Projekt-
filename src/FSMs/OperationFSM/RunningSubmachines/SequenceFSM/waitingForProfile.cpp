/*
 * waitingForProfile.cpp
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#include "SequenceBase.h"
#include "SequenceContext.h"
#include "waitingForProfile.h"
#include "IH.h"
#include "../../../../PulseMessages.h"
#include <sys/neutrino.h>
namespace M1{


	waitingForProfile::waitingForProfile(){
	}

	void waitingForProfile::entry(SequenceContext& ctx){
		ctx.oneRound = false;
	}

	void waitingForProfile::exit(SequenceContext& ctx){
		//nichts?
	}

	void waitingForProfile::eventUpdate(SequenceContext& ctx){
		if(ctx.data.section2.empty()) return;
		ctx.resolveProfile();									//Profilierung
		if(ctx.data.section2.front().item == IH_PROFILE){	//wenn vorderes(Item am Pusher) richtig dann zustand wechseln
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

	void waitingForProfile::raiseItemCorrect(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_CORRECT, 0);
	}
	void waitingForProfile::raiseItemWrong(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_WRONG, 0);
	}
}
