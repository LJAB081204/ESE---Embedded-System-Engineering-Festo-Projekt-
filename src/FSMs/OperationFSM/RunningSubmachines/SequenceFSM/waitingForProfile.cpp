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
#include <iostream>
namespace M1{


	waitingForProfile::waitingForProfile(){
	}

	void waitingForProfile::entry(SequenceContext& ctx){
		switch(ctx.data.sequenceState){
			case WFP_State:
				ctx.data.oneRound = false;
				std::cout << "Next expected -> IH" << std::endl;
				break;
			case IH_State:
				std::cout << "WFP: GoBack IH" << std::endl;
				eventGoBackIH(ctx);
				break;
			case IM_State:
				std::cout << "WFP: GoBack IM" << std::endl;
				eventGoBackIM(ctx);
				break;
			case IF_State:
				std::cout << "WFP: GoBack IF" << std::endl;
				eventGoBackIF(ctx);
				break;
			case IHM_State:
				std::cout << "WFP: GoBack IHM" << std::endl;
				eventGoBackIHM(ctx);
				break;
			}
	}

	void waitingForProfile::exit(SequenceContext& ctx){
		//nichts?
	}

	void waitingForProfile::eventUpdate(SequenceContext& ctx){
//		std::cout << "SECTION 2 SIZE AT UPDATE :" << ctx.data.section2.size() << std::endl;

		if(ctx.data.section2.empty()) return;
//		std::cout << "ENTERED UPDATE FUNCTION" << std::endl;

		ctx.resolveProfile();									//Profilierung
//		std::cout << "SECTION2 NOT EMPTY" << std::endl;

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

	void waitingForProfile::raiseItemCorrect(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_CORRECT, 0);
	}
	void waitingForProfile::raiseItemWrong(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_WRONG, 0);
	}
}
