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

namespace M2{

	IHM::IHM(){
		this->item = UNDEFINED_PROFILE;
	}

	void IHM::entry(SequenceContext& ctx){
		std::cout << "IHM --- next expected -> IH" << std::endl;
		ctx.data.oneRound = true;
	}

	void IHM::exit(SequenceContext& ctx){

	}

	void IHM::eventUpdate(SequenceContext& ctx){
		if(ctx.data.section2.empty()) return;
		ctx.resolveProfile();	//Profilierung
		if(ctx.data.nextItem.empty()) return;
		item = ctx.data.nextItem.front().item;
		ctx.data.nextItem.pop();
		if(item != ctx.data.section2.front().item){
			std::cout << "ITEM HAS CHANGED -> GO BACK" << std::endl;
			raiseGoBackIHM(ctx);
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

	void IHM::eventGoBack(SequenceContext&ctx){
		exit(ctx);
		new (this) IF;
		entry(ctx);
	}


	void IHM::raiseItemCorrect(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_CORRECT, 0);
	}
	void IHM::raiseItemWrong(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_WRONG, 0);
	}
	void IHM::raiseGoBackIHM(SequenceContext& ctx){
		MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IHM	, 0);
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


}
