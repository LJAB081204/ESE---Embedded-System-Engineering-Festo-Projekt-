/*
 * Idle.cpp
 *
 *  Created on: 27.05.2026
 *      Author: tayla
 */
#include <new>
#include "OperationBase.h"
#include "OperationContext.h"
#include "Idle.h"
#include "Running.h"
#include "Service.h"

	Idle::Idle(){
	}

	void Idle::entry(OperationContext& ctx){
		ctx.hal.getActors()->motorStop();
		ctx.hal.getActors()->greenLampOff();
		ctx.hal.getActors()->greenLampBlinkOff();
		ctx.hal.getActors()->yellowLampOff();
		ctx.hal.getActors()->yellowLampBlinkOff();
		ctx.hal.getActors()->redLampOff();
		ctx.hal.getActors()->redLampBlinkOff();
	}
	void Idle::exit(OperationContext& ctx){
	}

	void Idle::eventButtonStartPressed(OperationContext& ctx){
		exit(ctx);
		new (this) Running;
		entry(ctx);
		}
	void Idle::eventButtonStartPressedLong(OperationContext& ctx){
		exit(ctx);
		new (this) Service;
		entry(ctx);
				}





