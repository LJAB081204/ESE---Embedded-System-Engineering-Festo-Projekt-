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
		ctx.hal.getActors()->ledButtonResetOff();

		ctx.hal.getActors()->ledButtonStart();
	}
	void Idle::exit(OperationContext& ctx){
		ctx.hal.getActors()->ledButtonStartOff();

	}

	void Idle::eventButtonStartPressed(OperationContext& ctx){
		exit(ctx);
		if (ctx.savedRunning){
			ctx.currentState = ctx.savedRunning;
			ctx.savedRunning = nullptr;
			ctx.currentState->onRestore(ctx.historySource, ctx);
			ctx.historySource = HistorySource::NONE_HISTORY;
			ctx.currentState->entry(ctx);
		} else {
	        ctx.currentState = ctx.factory.createRunning(ctx.hal, ctx.data,ctx.timerHandler);
	        ctx.currentState->entry(ctx);
		}
	}
	void Idle::eventButtonStartPressedRemote(OperationContext& ctx){
		exit(ctx);
		if (ctx.savedRunning){
			ctx.currentState = ctx.savedRunning;
			ctx.savedRunning = nullptr;
			ctx.currentState->onRestore(ctx.historySource, ctx);
			ctx.historySource = HistorySource::NONE_HISTORY;
			ctx.currentState->entry(ctx);
		} else {
	        ctx.currentState = ctx.factory.createRunning(ctx.hal, ctx.data,ctx.timerHandler);
	        ctx.currentState->entry(ctx);
		}
	}
	void Idle::eventButtonStartPressedLong(OperationContext& ctx){
		exit(ctx);
		new (this) Service;
		entry(ctx);
	}
	void Idle::eventButtonStartPressedLongRemote(OperationContext& ctx){
		exit(ctx);
		new (this) Service;
		entry(ctx);
	}




