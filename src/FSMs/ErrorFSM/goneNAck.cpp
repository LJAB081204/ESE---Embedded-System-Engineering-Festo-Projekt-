/*
 * goneNAck.cpp
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */

#include <new>
#include "ErrorBase.h"
#include "ErrorContext.h"
#include "goneNAck.h"
#include "../../PulseMessages.h"
#include "errorPusherTooLong.h"
#include "waitingForStart.h"


	goneNAck::goneNAck(){

	}

	void goneNAck::entry(ErrorContext& ctx){
		ctx.hal.getActors()->redLampBlinkOff();
		ctx.hal.getActors()->redLampBlink(500);
		ctx.data.err = ERROR_CLEARED;
	}

	void goneNAck::exit(ErrorContext& ctx){
		ctx.hal.getActors()->redLampBlinkOff();
	}

	void goneNAck::eventButtonResetPressed(ErrorContext& ctx){
		exit(ctx);
		new (this) waitingForStart;
		entry(ctx);
	}



	void goneNAck::errorPusherTimerExpired(ErrorContext& ctx){
		exit(ctx);
		new (this) errorPusherTooLong;
		entry(ctx);
	}
