/*
 * Running.cpp
 *
 *  Created on: 28.05.2026
 *      Author: tayla
 */
#include <new>
#include "../../HAL.h"
#include "OperationBase.h"
#include "OperationContext.h"
#include "RunningSubmachines/TransportFSM/TransportContext.h"
#include "RunningSubmachines/WeicheFSM/WeicheContext.h"
#include "RunningSubmachines/SequenceFSM/SequenceContext.h"
#include "RunningSubmachines/ProfileFSM/ProfileContext.h"
#include "RunningSubmachines/SlideFSM/SlideContext.h"
#include "Idle.h"
#include "Running.h"

#include <iostream>

namespace M1{
	Running::Running(HAL& hal,dataClass& data,SequenceContext& sequence){
		transportFSM = new TransportContext(hal,data);
		weicheFSM = new WeicheContext(hal,data);
		sequenceFSM = &sequence;
		profileFSM = new ProfileContext(data);
		slideFSM = new SlideContext(hal,data);
	}


	void Running::entry(OperationContext& ctx){
		ctx.hal.getActors()->greenLampOn();
	}

	void Running::exit(OperationContext& ctx){
		delete transportFSM;
		delete weicheFSM;
		delete profileFSM;
		delete slideFSM;
	}

	void Running::onRestore(HistorySource source, OperationContext& ctx){
		if (source == HistorySource::ESTOP_HISTORY){
			delete transportFSM;
			transportFSM = new TransportContext(ctx.hal,ctx.data);
			delete weicheFSM;
			weicheFSM = new WeicheContext(ctx.hal,ctx.data);
			delete profileFSM;
			profileFSM = new ProfileContext(ctx.data);
			delete slideFSM;
			slideFSM = new SlideContext(ctx.hal,ctx.data);
		}
		else{
			transportFSM->reactivate();
			slideFSM->reactivate();
		}
	}

	// Buttons
	void Running::eventButtonStopPressed(OperationContext& ctx){
	    ctx.saveHistory(HistorySource::STOP_HISTORY);
	}
	void Running::eventButtonStopPressedRemote(OperationContext& ctx){
	    ctx.saveHistory(HistorySource::STOP_HISTORY);
	}

	// Sensoren lokal
	void Running::eventItemAtEntry(OperationContext& ctx){
	    transportFSM->eventItemAtEntry();
	}
	void Running::eventItemAtExit(OperationContext& ctx){
	    transportFSM->eventItemAtExit();
	}
	void Running::eventItemLeftExit(OperationContext& ctx){
	    transportFSM->eventItemLeftExit();
	}
	void Running::eventItemAtPusher(OperationContext& ctx){
	    weicheFSM->eventItemAtPusher();
	}
	void Running::eventItemLeftPusher(OperationContext& ctx){
	    weicheFSM->eventItemLeftPusher();
	}
	void Running::eventItemInSlide(OperationContext& ctx){
		slideFSM->eventItemInSlide();
	    transportFSM->eventItemInSlide();
	}
	void Running::eventItemLeftSlide(OperationContext& ctx){
		slideFSM->eventItemLeftSlide();
	    weicheFSM->eventItemLeftSlide();
	}
	void Running::eventItemMetal(OperationContext& ctx){
	    weicheFSM->eventItemMetal();
	}
	void Running::eventItemGone(OperationContext& ctx){
	    transportFSM->eventItemGone();
	    profileFSM->eventItemGone();
	}
	void Running::eventItemArrived(OperationContext& ctx){
	    transportFSM->eventItemArrived();
	}

	// Sensoren remote
	void Running::eventItemAtEntryRemote(OperationContext& ctx){
	    transportFSM->eventItemAtEntryRemote();
	}
	void Running::eventItemLeftEntryRemote(OperationContext& ctx){
	    // kein Sub-FSM braucht das aktuell
	}
	void Running::eventItemAtExitRemote(OperationContext& ctx){
	    // kein Sub-FSM braucht das aktuell
	}
	void Running::eventItemLeftExitRemote(OperationContext& ctx){
	    transportFSM->eventItemLeftExitRemote();
	}
	void Running::eventItemAtPusherRemote(OperationContext& ctx){
	    // kein Sub-FSM braucht das aktuell
	}
	void Running::eventItemLeftPusherRemote(OperationContext& ctx){
	    // kein Sub-FSM braucht das aktuell
	}
	void Running::eventItemInSlideRemote(OperationContext& ctx){
	    transportFSM->eventItemInSlideRemote();
	}
	void Running::eventItemLeftSlideRemote(OperationContext& ctx){
	    // kein Sub-FSM braucht das aktuell
	}
	void Running::eventItemMetalRemote(OperationContext& ctx){
	    // kein Sub-FSM braucht das aktuell
	}
	void Running::eventSlideFullRemote(OperationContext& ctx){
	    // kein Sub-FSM braucht das aktuell
	}

	void Running::eventSlideTimerExpired(OperationContext& ctx){
		slideFSM->eventSlideTimerExpired();
	}

	// Höhensensor / Profil
	void Running::eventHeightHigh(OperationContext& ctx){
	    profileFSM->eventHeightHigh();
	}
	void Running::eventHeightFlat(OperationContext& ctx){
	    profileFSM->eventHeightFlat();
	}
	void Running::eventHeightHole(OperationContext& ctx){
	    profileFSM->eventHeightHole();
	}
	void Running::eventHeightWrong(OperationContext& ctx){
	    profileFSM->eventHeightWrong();
	}

	// Festo
	void Running::eventFestoReady(OperationContext& ctx){
	    transportFSM->eventFestoReady();
	}
	void Running::eventFestoNotReady(OperationContext& ctx){
	    transportFSM->eventFestoNotReady();
	}

	// Logik
	void Running::eventItemCorrect(OperationContext& ctx){
	    weicheFSM->eventItemCorrect();
	}
	void Running::eventItemWrong(OperationContext& ctx){
	    weicheFSM->eventItemWrong();
	}
	void Running::eventUpdate(OperationContext& ctx){
	    weicheFSM->eventUpdate();
	    sequenceFSM->eventUpdate();
	}
	void Running::eventError(OperationContext& ctx){}
	void Running::eventTimerExpired(OperationContext& ctx){}
}
