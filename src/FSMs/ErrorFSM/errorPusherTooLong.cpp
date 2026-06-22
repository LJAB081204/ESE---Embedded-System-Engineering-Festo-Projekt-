/*
 * errorPusherTooLong.cpp
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */
#include <new>
#include "ErrorBase.h"
#include "ErrorContext.h"
#include "waitingForStart2.h"
#include <iostream>
#include "errorPusherTooLong.h"

	errorPusherTooLong::errorPusherTooLong(){

	}

	void errorPusherTooLong::entry(ErrorContext& ctx){
	    ctx.hal.getActors()->closePusher();
	    ctx.hal.getActors()->ledButtonReset();

		std::cout << "Pusher seit 20 Sekunden offen. Quittiere den Fehler mit Reset." << std::endl;
	}

	void errorPusherTooLong::exit(ErrorContext& ctx){
    	ctx.hal.getActors()->ledButtonResetOff();

	}

	void errorPusherTooLong::eventButtonResetPressed(ErrorContext& ctx){
		exit(ctx);
		new(this) waitingForStart2;
		entry(ctx);
	}


