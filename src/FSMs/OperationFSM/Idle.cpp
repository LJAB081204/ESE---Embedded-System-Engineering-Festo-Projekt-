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
#include "ServiceMode/Calibration.h"
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
		ctx.data.handler->pauseAll();
		if(ctx.data.calibrated){
		ctx.data.calibrated = false;
		ctx.data.reset();
		}
		if(ctx.firstBoot || ctx.historySource == HistorySource::ESTOP_HISTORY){
		    ctx.firstBoot = false;
//			if(ctx.hal.getSensors()->isItemAtEntry()){
//				std::cout << "Warnung: Werkstück am Eingang erkannt." << std::endl;
//				ctx.hal.getActors()->yellowLampBlink(1000);
//			}
			if(ctx.hal.getSensors()->isItemInPusher()){
				std::cout << "Warnung: Werkstück am Pusher erkannt." << std::endl;
				ctx.hal.getActors()->yellowLampBlink(1000);
			}
			if(ctx.hal.getSensors()->isItemAtExit()){
				std::cout << "Warnung: Werkstück am Ausgang erkannt." << std::endl;
				ctx.hal.getActors()->yellowLampBlink(1000);
			}

		}
		std::cout << "--- [IDLE] ---" << std::endl;
		if(ctx.data.isMachine1 && ctx.data.entryTime == 0) {
			std::cout << "Warnung: Keine Kalibrierungsdaten gefunden." << std::endl;
			std::cout << "Bitte Kalibrierung im Service-Modus durchführen." << std::endl;
		}
		std::cout << "Starttaste drücken für Betrieb." << std::endl;
		std::cout << "Starttaste lange drücken für Service-Modus." << std::endl;
		seqUpdate(ctx);
//		ctx.blockStartUntilRelease = false;
	}
	void Idle::exit(OperationContext& ctx){
		ctx.hal.getActors()->ledButtonStartOff();
		ctx.hal.getActors()->yellowLampBlinkOff();
		ctx.data.handler->resumeAll();


	}

	void Idle::eventButtonStartPressed(OperationContext& ctx){
//	    if(ctx.blockStartUntilRelease){
//	        return;
//	    }
		exit(ctx);
	    if (ctx.savedRunning){
	        ctx.currentState = ctx.savedRunning;
	        ctx.savedRunning = nullptr;
	        ctx.currentState->onRestore(ctx.historySource, ctx);
	        ctx.historySource = HistorySource::NONE_HISTORY;
	        ctx.currentState->entry(ctx);
	    } else {
	        ctx.currentState =
	            ctx.factory.createRunning(
	                ctx.hal,
	                ctx.data,
	                ctx.timerHandler);

	        ctx.currentState->entry(ctx);
	    }
	}
	void Idle::eventButtonStartPressedRemote(OperationContext& ctx){
	    if(ctx.longPressTransition){
	        ctx.longPressTransition = false;
	        return;
	    }
//	    if(ctx.blockStartUntilRelease){
//	        return;
//	    }
	    exit(ctx);
	    if (ctx.savedRunning){
	        ctx.currentState = ctx.savedRunning;
	        ctx.savedRunning = nullptr;
	        ctx.currentState->onRestore(ctx.historySource, ctx);
	        ctx.historySource = HistorySource::NONE_HISTORY;
	        ctx.currentState->entry(ctx);
	    } else {
	        ctx.currentState =
	            ctx.factory.createRunning(
	                ctx.hal,
	                ctx.data,
	                ctx.timerHandler);

	        ctx.currentState->entry(ctx);
	    }
	}
	void Idle::eventButtonStartPressedLong(OperationContext& ctx){
		exit(ctx);
		//ctx.blockStartUntilRelease = true;
		new (this) Service;
		entry(ctx);
	}
	void Idle::eventButtonStartPressedLongRemote(OperationContext& ctx){
		exit(ctx);
		//ctx.blockStartUntilRelease = true;
		new (this) Service;
		entry(ctx);
	}

	void Idle:: seqUpdate(OperationContext& ctx){
		switch(ctx.data.sequenceState){
				case WFP_State:
					eventGoBackWFP(ctx);
					break;
				case IH_State:
					eventGoBackIH(ctx);
					break;
				case IM_State:
					eventGoBackIM(ctx);
					break;
				case IF_State:
					eventGoBackIF(ctx);
					break;
				case IHM_State:
					eventGoBackIHM(ctx);
					break;
	}
	}

	void Idle::eventGoBack(OperationContext& ctx){
		if (ctx.savedRunning) ctx.savedRunning->eventGoBack(ctx);
	}
	void Idle::eventGoBackIH(OperationContext& ctx){
		if (ctx.savedRunning) ctx.savedRunning->eventGoBackIH(ctx);
	}
	void Idle::eventGoBackIM(OperationContext& ctx){
		if (ctx.savedRunning) ctx.savedRunning->eventGoBackIM(ctx);
	}
	void Idle::eventGoBackIF(OperationContext& ctx){
		if (ctx.savedRunning) ctx.savedRunning->eventGoBackIF(ctx);
	}
	void Idle::eventGoBackIHM(OperationContext& ctx){
		if (ctx.savedRunning) ctx.savedRunning->eventGoBackIHM(ctx);
	}

	void Idle::eventGoBackWFP(OperationContext& ctx){
		if (ctx.savedRunning) ctx.savedRunning->eventGoBackWFP(ctx);
	}


