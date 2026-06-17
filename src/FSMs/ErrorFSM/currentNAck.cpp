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


	currentNAck::currentNAck(){

	}

	void currentNAck::entry(ErrorContext& ctx){
		ctx.hal.getActors()->yellowLampBlinkOff();
		ctx.hal.getActors()->redLampBlink(100);
		ctx.hal.getActors()->motorStop();
	}

	void currentNAck::exit(ErrorContext& ctx){

	}

	void currentNAck::eventItemLeftSlide(ErrorContext& ctx){
		exit(ctx);
		new (this) goneNAck;
		entry(ctx);
	}

	void currentNAck::eventButtonStartPressed(ErrorContext& ctx){
		exit(ctx);
		//exit -> später implementieren
		entry(ctx);
	}

	void currentNAck::eventButtonResetPressed(ErrorContext& ctx){
		if(ctx.data.err != ITEM_GONE && ctx.data.err != ERROR_CLEARED){
			exit(ctx);
			new (this)currentAck;
			entry(ctx);
		}
		else if(ctx.data.err == ITEM_GONE){
			ctx.data.itemCount--;
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

