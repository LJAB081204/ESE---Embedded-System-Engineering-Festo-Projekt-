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
#include "IH.h"
#include "../../../../PulseMessages.h"
#include <sys/neutrino.h>

	IM::IM(){
	}

	void IM::entry(SequenceContext& ctx){

	}

	void IM::exit(SequenceContext& ctx){

	}

	void IM::eventUpdate(SequenceContext& ctx){
		ctx.resolveProfile();	//profilierung
		if(ctx.data.section2.front().item == IF_PROFILE){	//wenn vorderes(Item am Pusher) richtig dann zustand wechseln
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

	void IM::eventGoBack(SequenceContext& ctx){
		if(ctx.data.nextItem.front().item == UNDEFINED_PROFILE){ //oneRound raus?
			exit(ctx);
			new(this) waitingForProfile;
			entry(ctx);
		}
		else if (ctx.data.nextItem.front().item == IHM_PROFILE){
			exit(ctx);
			new(this) IHM;
			entry(ctx);
		}
		else if(ctx.data.nextItem.front().item == IF_PROFILE){
			exit(ctx);
			new(this) IF;
			entry(ctx);
		}
		else{
			exit(ctx);
			new(this) IH;
			entry(ctx);
		}
	}



	void IM::raiseItemCorrect(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_CORRECT, 0);
	}
	void IM::raiseItemWrong(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_WRONG, 0);
	}


