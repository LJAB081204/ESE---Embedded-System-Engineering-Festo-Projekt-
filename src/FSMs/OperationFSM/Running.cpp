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
#include "Idle.h"
#include "Running.h"


	Running::Running(HAL& hal){
		transportFSM = new TransportContext(hal);
		weicheFSM = new WeicheContext(hal);
		//sequenceFSM = new SequenceContext();
		//profileFSM = new ProfileContext();
	}


	void Running::entry(OperationContext& ctx){
		ctx.hal.getActors()->greenLampOn();
	}

	void Running::exit(OperationContext& ctx){
		delete transportFSM;
		delete weicheFSM;
		//delete sequenceFSM;
		//delete profileFSM;
	}

	void Running::eventButtonStopPressed(OperationContext& ctx){
		exit(ctx);
		new (this) Idle;
		entry(ctx);
	}
	void Running::eventItemAtEntry(OperationContext& ctx){
		transportFSM->eventItemAtEntry();
	}
	void Running::eventItemAtExit(OperationContext& ctx){
		transportFSM->eventItemAtExit();
	}
	void Running::eventItemInSlide(OperationContext& ctx){
		transportFSM->eventItemInSlide();
	}
	void Running::eventItemGone(OperationContext& ctx){
		transportFSM->eventItemGone();
	}
	void Running::eventItemArrived(OperationContext& ctx){
		transportFSM->eventItemArrived();
	}
	void Running::eventItemMetal(OperationContext& ctx){
		weicheFSM->eventItemMetal();
	}
	void Running::eventItemLeftSlide(OperationContext& ctx){

	}
	void Running::eventItemAtPusher(OperationContext& ctx){
	    weicheFSM->eventItemAtPusher();
	}
	void Running::eventItemLeftPusher(OperationContext& ctx){
	    weicheFSM->eventItemLeftPusher();
	}
	void Running::eventItemWrong(OperationContext& ctx){
		weicheFSM->eventItemWrong();
	}
	void Running::eventItemCorrect(OperationContext& ctx){
		weicheFSM->eventItemCorrect();
	}
	void Running::eventUpdate(OperationContext& ctx){
		weicheFSM->eventUpdate();
	}
	void Running::eventTimerExpired(OperationContext& ctx){

	}
	void Running::eventError(OperationContext& ctx){}
