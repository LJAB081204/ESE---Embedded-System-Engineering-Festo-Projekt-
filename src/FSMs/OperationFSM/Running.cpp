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
#include "Idle.h"
#include "Running.h"


	Running::Running(HAL& hal){
		transportFSM = new TransportContext(hal);
	}


	void Running::entry(OperationContext& ctx){
		ctx.hal.getActors()->greenLampOn();
	}

	void Running::exit(OperationContext& ctx){
		delete transportFSM;
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

