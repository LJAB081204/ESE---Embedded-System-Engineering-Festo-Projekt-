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
#include <iostream>
namespace M2{

	IM::IM(){
		this->item = UNDEFINED_PROFILE;
	}

	void IM::entry(SequenceContext& ctx){
		std::cout << "IM --- next expected -> IF" << std::endl;
	}

	void IM::exit(SequenceContext& ctx){

	}

	void IM::eventUpdate(SequenceContext& ctx){
		if(ctx.data.section2.empty()) return;
		ctx.resolveProfile();	//Profilierung
		if(ctx.data.nextItem.empty()) return;
		item = ctx.data.nextItem.front().item;
		ctx.data.nextItem.pop();
		if(item != ctx.data.section2.front().item){
			raiseGoBackIM(ctx);
			raiseItemWrong(ctx);

			std::cout << "ITEM HAS CHANGED -> GO BACK" << std::endl;
		}
		else{
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
	}

	void IM::eventGoBack(SequenceContext&ctx){
		exit(ctx);
		new (this) IH;
		entry(ctx);
	}



	void IM::raiseItemCorrect(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_CORRECT, 0);
	}
	void IM::raiseItemWrong(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_WRONG, 0);
	}
	void IM::raiseGoBackIM(SequenceContext& ctx){
		MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IM	, 0);
	}

	/*void IM::eventGoBack(SequenceContext& ctx){
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
		}*/
}

