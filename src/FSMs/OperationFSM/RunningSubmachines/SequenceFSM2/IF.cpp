/*
 * IF.cpp
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#include "SequenceBase.h"
#include "SequenceContext.h"
#include "IF.h"
#include "IHM.h"
#include "IH.h"
#include "IM.h"
#include "../../../../PulseMessages.h"
#include <sys/neutrino.h>
#include <iostream>
namespace M2{

	IF::IF(){
		this->item = UNDEFINED_PROFILE;
	}

	void IF::entry(SequenceContext& ctx){
		std::cout << "IF --- next expected -> IHM" << std::endl;
		ctx.data.sequenceState = IF_State;
	}

	void IF::exit(SequenceContext& ctx){

	}

	void IF::eventUpdate(SequenceContext& ctx){
		if(ctx.data.section2.empty()) return;
		ctx.resolveProfile();	//Profilierung
		if(ctx.data.nextItem.empty()) return;
		item = ctx.data.nextItem.front().item;
		ctx.data.nextItem.pop();
		if(item != ctx.data.section2.front().item){
			raiseGoBackIF(ctx);
			raiseItemWrong(ctx);

		}
		else{
			if(ctx.data.section2.front().item == IHM_PROFILE){	//wenn vorderes(Item am Pusher) richtig dann zustand wechseln
				exit(ctx);
				raiseItemCorrect(ctx);
				new (this) IHM;
				entry(ctx);
			}
			else{
				exit(ctx);
				raiseItemWrong(ctx);
				entry(ctx);
			}
		}
	}

	void IF::eventGoBack(SequenceContext&ctx){
		exit(ctx);
		new (this) IM;
		entry(ctx);
	}

	/*void IF::eventGoBack(SequenceContext& ctx){
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
		else if(ctx.data.nextItem.front().item == IM_PROFILE){
			exit(ctx);
			new(this) IM;
			entry(ctx);
		}
		else{
			exit(ctx);
			new(this) IH;
			entry(ctx);
		}
	}*/



	void IF::raiseItemCorrect(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_CORRECT, 0);
	}
	void IF::raiseItemWrong(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_WRONG, 0);
	}
	void IF::raiseGoBackIF(SequenceContext& ctx){
		MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IF	, 0);
	}
}
