/*
 * Running2.cpp
 *
 *  Created on: 28.05.2026
 *      Author: tayla
 */
#include <new>
#include "../../HAL.h"
#include "../../timer.h"
#include "OperationBase.h"
#include "OperationContext.h"
#include "RunningSubmachines/TransportFSM2/TransportContext2.h"
#include "RunningSubmachines/WeicheFSM2/WeicheContext.h"
#include "RunningSubmachines/SequenceFSM2/SequenceContext.h"
#include "RunningSubmachines/ProfileFSM2/ProfileContext.h"
#include "RunningSubmachines/SlideFSM2/SlideContext.h"
#include "Idle.h"
#include "Running2.h"

#include <iostream>

namespace M2{
Running2::Running2(HAL& hal,dataClass& data,SequenceContext& sequence,TimerHandler& timerHandler){
		transportFSM = new TransportContext2(hal,data);
		weicheFSM = new WeicheContext(hal,data,timerHandler);
		sequenceFSM = &sequence;
		profileFSM = new ProfileContext(data);
		slideFSM = new SlideContext(hal,data);
	}


	void Running2::entry(OperationContext& ctx){
		ctx.hal.getActors()->greenLampOn();
	}

	void Running2::exit(OperationContext& ctx){
		delete transportFSM;
		delete weicheFSM;
		delete profileFSM;
		delete slideFSM;
	}

	void Running2::onRestore(HistorySource source, OperationContext& ctx){
		if (source == HistorySource::ESTOP_HISTORY){
			delete transportFSM;
			transportFSM = new TransportContext2(ctx.hal,ctx.data);
			delete weicheFSM;
			weicheFSM = new WeicheContext(ctx.hal,ctx.data,ctx.timerHandler);
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
	void Running2::eventButtonStopPressed(OperationContext& ctx){
	    ctx.saveHistory(HistorySource::STOP_HISTORY);
	}
	void Running2::eventButtonStopPressedRemote(OperationContext& ctx){
	    ctx.saveHistory(HistorySource::STOP_HISTORY);
	}

	// Sensoren lokal
	void Running2::eventItemAtEntry(OperationContext& ctx){
	    transportFSM->eventItemAtEntry();
	}
	void Running2::eventItemAtExit(OperationContext& ctx){
	    transportFSM->eventItemAtExit();
	}
	void Running2::eventItemLeftExit(OperationContext& ctx){
	    transportFSM->eventItemLeftExit();
	}
	void Running2::eventItemAtPusher(OperationContext& ctx){
	    weicheFSM->eventItemAtPusher();
	}
	void Running2::eventItemLeftPusher(OperationContext& ctx){
	    weicheFSM->eventItemLeftPusher();
	}
	void Running2::eventItemInSlide(OperationContext& ctx){
		slideFSM->eventItemInSlide();
	    transportFSM->eventItemInSlide();
	}
	void Running2::eventItemLeftSlide(OperationContext& ctx){
		slideFSM->eventItemLeftSlide();
	    weicheFSM->eventItemLeftSlide();
	}
	void Running2::eventItemMetal(OperationContext& ctx){
	    weicheFSM->eventItemMetal();
	}
	void Running2::eventItemGone(OperationContext& ctx){
	    transportFSM->eventItemGone();
	    profileFSM->eventItemGone();
	}
	void Running2::eventItemArrived(OperationContext& ctx){
	    transportFSM->eventItemArrived();
	}

	// Sensoren remote
	void Running2::eventItemAtEntryRemote(OperationContext& ctx){
	    transportFSM->eventItemAtEntryRemote();
	}
	void Running2::eventItemLeftEntryRemote(OperationContext& ctx){
	    // kein Sub-FSM braucht das aktuell
	}
	void Running2::eventItemAtExitRemote(OperationContext& ctx){
	    // kein Sub-FSM braucht das aktuell
	}
	void Running2::eventItemLeftExitRemote(OperationContext& ctx){
	    transportFSM->eventItemLeftExitRemote();
	}
	void Running2::eventItemAtPusherRemote(OperationContext& ctx){
	    // kein Sub-FSM braucht das aktuell
	}
	void Running2::eventItemLeftPusherRemote(OperationContext& ctx){
	    // kein Sub-FSM braucht das aktuell
	}
	void Running2::eventItemInSlideRemote(OperationContext& ctx){
	    transportFSM->eventItemInSlideRemote();
	}
	void Running2::eventItemLeftSlideRemote(OperationContext& ctx){
	    // kein Sub-FSM braucht das aktuell
	}
	void Running2::eventItemMetalRemote(OperationContext& ctx){
	    // kein Sub-FSM braucht das aktuell
	}
	void Running2::eventSlideFullRemote(OperationContext& ctx){
	    // kein Sub-FSM braucht das aktuell
	}

	void Running2::eventSlideTimerExpired(OperationContext& ctx){
		slideFSM->eventSlideTimerExpired();
	}

	// Höhensensor / Profil
	void Running2::eventHeightHigh(OperationContext& ctx){
	    profileFSM->eventHeightHigh();
	}
	void Running2::eventHeightFlat(OperationContext& ctx){
	    profileFSM->eventHeightFlat();
	}
	void Running2::eventHeightHole(OperationContext& ctx){
	    profileFSM->eventHeightHole();
	}
	void Running2::eventHeightWrong(OperationContext& ctx){
	    profileFSM->eventHeightWrong();
	}

	// Festo
	void Running2::eventFestoReady(OperationContext& ctx){
	    transportFSM->eventFestoReady();
	}
	void Running2::eventFestoNotReady(OperationContext& ctx){
	    transportFSM->eventFestoNotReady();
	}

	// Logik
	void Running2::eventItemCorrect(OperationContext& ctx){
	    weicheFSM->eventItemCorrect();
	}
	void Running2::eventItemWrong(OperationContext& ctx){
	    weicheFSM->eventItemWrong();
	}
	void Running2::eventUpdate(OperationContext& ctx){
	    weicheFSM->eventUpdate();
	    sequenceFSM->eventUpdate();
	}
	void Running2::eventError(OperationContext& ctx){}
	void Running2::eventTimerExpired(OperationContext& ctx){}

	void Running2::eventItemTransfer(uint32_t itemCode, OperationContext& ctx){
			transportFSM ->eventItemTransfer(itemCode);
		}
}
