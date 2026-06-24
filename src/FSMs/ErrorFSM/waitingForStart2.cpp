/*
 * waitingForStart2.cpp
 *
 *  Created on: 09.06.2026
 *      Author: lewis
 */

#include <new>
#include <iostream>
#include "ErrorBase.h"
#include "ErrorContext.h"
#include "waitingForStart2.h"
#include "../../PulseMessages.h"

	waitingForStart2::waitingForStart2(){

	}

	void waitingForStart2::entry(ErrorContext& ctx){
		ctx.active = true;
		ctx.data.errorState = WAITING_FOR_START_STATE2;
		std::cout << "Entferne das Werkstück aus dem Pusher und drücke danach Start." << std::endl;
		ctx.hal.getActors()->closePusher();
	    ctx.hal.getActors()->redLampBlink(1500);
	    ctx.hal.getActors()->ledButtonStart();

	}

	void waitingForStart2::exit(ErrorContext& ctx){
		 ctx.hal.getActors()->ledButtonStartOff();

	}

	void waitingForStart2::eventButtonStartPressed(ErrorContext& ctx){
		exit(ctx);
		if (ctx.onlyPusher == true){
			ctx.active = false;
		}
		else{
			ctx.data.errorState = CURRENT_NACK_STATE;
			new (this) currentNAck;
			entry(ctx);
		}
	}
