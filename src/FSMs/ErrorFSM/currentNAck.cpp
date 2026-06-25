/*
 * currentNAck.cpp
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */

#include <new>
#include "ErrorBase.h"
#include "ErrorContext.h"
#include "currentNAck.h"
#include "goneNAck.h"
#include "waitingForStart.h"
#include "currentAck.h"
#include "../../PulseMessages.h"
#include <sys/neutrino.h>
#include "waitingForStart.h"
#include "errorPusherTooLong.h"
#include <iostream>
#include <string>

	currentNAck::currentNAck(){

	}

	void currentNAck::entry(ErrorContext& ctx){
		ctx.active = true;
		ctx.hal.getActors()->ledButtonReset();
		ctx.hal.getActors()->redLampBlinkOff();
		ctx.hal.getActors()->redLampBlink(1000);
		ctx.hal.getActors()->motorStop();
		ctx.data.errorState = CURRENT_NACK_STATE;
		std::string location;
		switch (ctx.data.errorLoc){
		case 1:
			location = "Höhensensor";
			break;
		case 2:
			location = "Pusher";
			break;
		case 3:
			location = "Ausgang";
			break;
		default: break;

		}
		switch (ctx.data.err){
		case ITEM_SPECIAL: std::cout << "Fehler: Spezialitem erkannt."<< std::endl;
			break;
		case ITEM_GONE: std::cout << "Fehler: Werkstück verschwunden. Ort: " << location << std::endl;
			break;
		case BOTH_SLIDES_FULL: std::cout << "Fehler: Versuchte Aussortierung bei voller Rutsche." << std::endl;
			break;
		case ITEM_APPEARED: std::cout << "Fehler: Werkstück aufgetaucht. Ort: "  << location << std::endl;
			break;
		case ENTRY_TOO_FAST: std::cout << "Fehler: Wartezeit beim Einlegen nicht eingehalten." << std::endl;
			break;
		default:
			std::cout << "Unbekannter Fehler"<< ctx.data.err << std::endl;
			break;
		}
		std::cout << "Bitte Fehler mit Resettaste quittieren." << std::endl;
	}

	void currentNAck::exit(ErrorContext& ctx){
		ctx.hal.getActors()->ledButtonResetOff();
	}

	void currentNAck::eventItemLeftSlide(ErrorContext& ctx){
		if(ctx.data.err == BOTH_SLIDES_FULL){
			exit(ctx);
			new (this) goneNAck;
			entry(ctx);
		}
	}

	void currentNAck::eventButtonStartPressed(ErrorContext& ctx){
//		exit(ctx);
//		//exit -> später implementieren
//		entry(ctx);
	}

	void currentNAck::eventButtonResetPressed(ErrorContext& ctx){
		if(ctx.data.err != ITEM_GONE && ctx.data.err != ERROR_CLEARED){
			exit(ctx);
			new (this)currentAck;
			entry(ctx);
		}
		else if(ctx.data.err == ITEM_GONE){
//			ctx.data.itemCount--;
			exit(ctx);
			new (this) waitingForStart;
			entry(ctx);
		}
	}

	void currentNAck::errorPusherTimerExpired(ErrorContext& ctx){
		exit(ctx);
		new (this) errorPusherTooLong;
		entry(ctx);
	}

