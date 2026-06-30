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
#include "IF.h"
#include "IM.h"
#include <iostream>
namespace M1{

	IHM::IHM(){
	}

	void IHM::entry(SequenceContext& ctx){
		switch(ctx.data.sequenceState){
			case WFP_State:
				eventGoBackWFP(ctx);
				break;
			case IH_State:
				eventGoBackIH(ctx);
				break;
			case IM_State:
				eventGoBackIM(ctx);
				break;
			case IF_State:
				eventGoBackIF(ctx);
				break;
			case IHM_State:
				std::cout << "IHM --- next expected -> IH" << std::endl;
				ctx.data.oneRound = true;
				break;
			}
	}

	void IHM::exit(SequenceContext& ctx){

	}

	void IHM::eventUpdate(SequenceContext& ctx){
		if(ctx.data.section2.empty()){
			MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_SKIP_UPDATE, 0);
			return;

		}
		ctx.resolveProfile();		//Profilierung
		if(ctx.data.section2.front().item == IH_PROFILE){	//wenn vorderes(Item am Pusher) richtig dann zustand wechseln
			exit(ctx);
			raiseItemCorrect(ctx);
			ctx.data.sequenceState = IH_State;
			new (this) IH;
			entry(ctx);
		}
		else{
			exit(ctx);
			ctx.data.section2.front().sortOut = true;
			raiseItemWrong(ctx);
			entry(ctx);
		}
	}

	/*void IHM::eventGoBack(SequenceContext& ctx){
		if(ctx.data.nextItem.front().item == UNDEFINED_PROFILE){ //oneRound raus?
			exit(ctx);
			new(this) waitingForProfile;
			entry(ctx);
		}
		else if (ctx.data.nextItem.front().item == IH_PROFILE){
			exit(ctx);
			new(this) IH;
			entry(ctx);
		}
		else if(ctx.data.nextItem.front().item == IF_PROFILE){
			exit(ctx);
			new(this) IF;
			entry(ctx);
		}
		else{
			exit(ctx);
			new(this) IM;
			entry(ctx);
		}
	}*/

	void IHM::eventGoBackWFP(SequenceContext& ctx){
		exit(ctx);
		new(this) waitingForProfile;
		entry(ctx);
	}

	void IHM::eventGoBackIH(SequenceContext& ctx){
		exit(ctx);
		new(this) IH;
		entry(ctx);
	}
	void IHM::eventGoBackIF(SequenceContext& ctx){
		exit(ctx);
		new(this) IF;
		entry(ctx);
	}
	void IHM::eventGoBackIM(SequenceContext& ctx){
		exit(ctx);
		new(this) IM;
		entry(ctx);
	}

	void IHM::raiseItemCorrect(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_CORRECT, 0);
	}
	void IHM::raiseItemWrong(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_WRONG, 0);
	}

}
