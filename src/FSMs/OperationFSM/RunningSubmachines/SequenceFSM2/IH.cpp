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
namespace M2{

	IH::IH(){
		this->item = UNDEFINED_PROFILE;
	}

	void IH::entry(SequenceContext& ctx){
		ctx.oneRound = false;
		std::cout << "Nächstes erwartet: IM" << std::endl;
		ctx.data.sequenceState = IH_State;
	}

	void IH::exit(SequenceContext& ctx){

	}

	void IH::eventUpdate(SequenceContext& ctx){
		if(ctx.data.section2.empty()) return;
		ctx.resolveProfile();	//Profilierung
		if(ctx.data.nextItem.empty()) return;
		item = ctx.data.nextItem.front().item;
		ctx.data.nextItem.pop();
		if(item != ctx.data.section2.front().item){
			ctx.data.section2.front().sortOut = true;
			ctx.data.section2.front().expectedProfile = item;
			ctx.data.section2.front().sortOutReason = SortOutReason::ITEM_SWAPPED;
			raiseGoBackIH(ctx);
			raiseItemWrong(ctx);

		}
		else{
			if(ctx.data.section2.front().item == IM_PROFILE){	//wenn vorderes(Item am Pusher) richtig dann zustand wechseln
				exit(ctx);
				raiseItemCorrect(ctx);
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
	}

	void IH::eventGoBack(SequenceContext& ctx){
		if(ctx.data.oneRound == true){
			exit(ctx);
			new (this) IHM;
			entry(ctx);
		}
		else{
			exit(ctx);
			new (this) waitingForProfile;
			entry(ctx);
		}

	}




	void IH::raiseItemCorrect(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_CORRECT, 0);
	}
	void IH::raiseItemWrong(SequenceContext& ctx){
		MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_ITEM_WRONG, 0);
	}

	void IH::raiseGoBackIH(SequenceContext& ctx){
		MsgSendPulse(ctx.data.controllerConId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_GO_BACK_IH	, 0);
	}



	/*void IH::eventGoBack(SequenceContext& ctx){
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
				new(this) IF;
				entry(ctx);
			}
		}*/

			/*void IH::eventItemAtPusher(SequenceContext& ctx){
				itemProfile item = ctx.resolveProfile();
				if(item != ctx.data.nextItem.front().item){
					ctx.data.nextItem.pop();
					raiseGoBack(ctx);
					std::cout << "Wrong Item Detected" << std::endl;
					//error falsches werkstück erkannt
				}
				else{
					ctx.data.nextItem.pop();
				}
			}*/

}
