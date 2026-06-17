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

namespace M2{

	waitingForProfile::waitingForProfile(){
		this->item = UNDEFINED_PROFILE;
	}

	void waitingForProfile::entry(SequenceContext& ctx){
		ctx.oneRound = false;
		std::cout << "Next expected -> IH" << std::endl;
	}

	void waitingForProfile::exit(SequenceContext& ctx){
		//nichts?
	}

	void waitingForProfile::eventUpdate(SequenceContext& ctx){
		if(ctx.data.section2.empty()) return;
		ctx.resolveProfile();	//Profilierung
		if(ctx.data.nextItem.empty()) return;
		item = ctx.data.nextItem.front().item;
		ctx.data.nextItem.pop();
		if(item != ctx.data.section2.front().item){
			std::cout << "ITEM HAS CHANGED -> GO BACK" << std::endl;
			raiseGoBackWFP(ctx);
			raiseItemWrong(ctx);
		}

		else{
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
	}

	/*void waitingForProfile::eventItemAtPusher(SequenceContext& ctx){
		itemProfile item = ctx.resolveProfile();
		if(item != ctx.data.nextItem.front().item){
			raiseGoBack(ctx);
			std::cout << "Wrong Item Detected" << std::endl;
			//error falsches werkstück erkannt
		}
	}*/

	void waitingForProfile::raiseItemCorrect(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_CORRECT, 0);
	}
	void waitingForProfile::raiseItemWrong(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_WRONG, 0);
	}

	void waitingForProfile::raiseGoBackWFP(SequenceContext& ctx){
		MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_WFP, 0);
	}
}

