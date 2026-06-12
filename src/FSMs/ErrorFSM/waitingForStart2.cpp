/*
 * waitingForStart2.cpp
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */

#include <new>
#include "ErrorBase.h"
#include "ErrorContext.h"
#include "waitingForStart2.h"
#include "../../PulseMessages.h"

	waitingForStart2::waitingForStart2(){

	}

	void waitingForStart2::entry(ErrorContext& ctx){
		ctx.hal.getActors()->closePusher();
	}

	void waitingForStart2::exit(ErrorContext& ctx){

	}

	void waitingForStart2::eventButtonStartPressed(ErrorContext& ctx){
		exit(ctx);
		new (this) currentNAck;
		entry(ctx);
	}
