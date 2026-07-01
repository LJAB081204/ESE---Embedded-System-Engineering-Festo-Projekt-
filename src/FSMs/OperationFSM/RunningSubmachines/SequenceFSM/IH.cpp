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
#include "IF.h"
#include <iostream>
namespace M1{

	IH::IH(){
	}

	void IH::entry(SequenceContext& ctx){
		switch(ctx.data.sequenceState){
				case WFP_State:
					eventGoBackWFP(ctx);
					break;
				case IH_State:
					std::cout << "Nächstes erwartet: IM" << std::endl;
					break;
				case IM_State:
					eventGoBackIM(ctx);
					break;
				case IF_State:
					eventGoBackIF(ctx);
					break;
				case IHM_State:
					eventGoBackIHM(ctx);
					break;
				}
	}

	void IH::exit(SequenceContext& ctx){

	}

	void IH::eventUpdate(SequenceContext& ctx){
		if(ctx.data.section2.empty()){
			MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_SKIP_UPDATE, 0);
			return;

		}
		ctx.resolveProfile();		//profilierung
		if(ctx.data.section2.front().item == IM_PROFILE){	//wenn vorderes(Item am Pusher) richtig dann zustand wechseln
			exit(ctx);
			raiseItemCorrect(ctx);
			ctx.data.sequenceState = IM_State;
			new (this) IM;
			entry(ctx);
		}
		else{
			exit(ctx);
			ctx.data.section2.front().sortOut = true;
			ctx.data.section2.front().expectedProfile = IM_PROFILE;
			ctx.data.section2.front().sortOutReason = SortOutReason::WRONG_SEQUENCE;
			raiseItemWrong(ctx);
			entry(ctx);
		}
	}

	/*void IH::eventGoBack(SequenceContext& ctx){

		else if (ctx.data.nextItem.front().item == IHM_PROFILE){
			exit(ctx);
			new(this) IHM;
			entry(ctx);
		}
		else if(ctx.data.nextItem.front().item == IM_PROFILE){
			exit(ctx);
			new(this) IM;
			entry(ctx);
		}
		else{
			exit(ctx);
			new(this) IF;
			entry(ctx);
		}
	}*/

	void IH::eventGoBackWFP(SequenceContext& ctx){
		exit(ctx);
		new(this) waitingForProfile;
		entry(ctx);
	}

	void IH::eventGoBackIM(SequenceContext& ctx){
		exit(ctx);
		new(this) IM;
		entry(ctx);
	}
	void IH::eventGoBackIF(SequenceContext& ctx){
		exit(ctx);
		new(this) IF;
		entry(ctx);
	}
	void IH::eventGoBackIHM(SequenceContext& ctx){
		exit(ctx);
		new(this) IHM;
		entry(ctx);
	}

	void IH::raiseItemCorrect(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_CORRECT, 0);
	}
	void IH::raiseItemWrong(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_WRONG, 0);
	}

}
