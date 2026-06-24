/*
 * waitingForStart.cpp
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */

#include <new>
#include "ErrorBase.h"
#include "ErrorContext.h"
#include "waitingForStart.h"
#include "../../PulseMessages.h"
#include "errorPusherTooLong.h"
#include <iostream>

	waitingForStart::waitingForStart(){

	}

	void waitingForStart::entry(ErrorContext& ctx){
		ctx.data.err = ERROR_CLEARED;
		ctx.hal.getActors()->ledButtonStart();
		std::cout << "Starttaste betätigen um Betrieb wiederaufzunehmen." << std::endl;
	}

	void waitingForStart::exit(ErrorContext& ctx){
		ctx.hal.getActors()->ledButtonStartOff();
	}

	void waitingForStart::eventButtonStartPressed(ErrorContext& ctx){
		exit(ctx);
		ctx.active = false;
	}

	void waitingForStart::errorPusherTimerExpired(ErrorContext& ctx){
		exit(ctx);
		new (this) errorPusherTooLong;
		entry(ctx);
	}


