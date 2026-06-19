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
		std::cout << "Entferne das Werkstück aus dem Pusher und drücke danach Start." << std::endl;
		ctx.hal.getActors()->closePusher();
	    ctx.hal.getActors()->redLampBlink(1500);
	    ctx.hal.getActors()->ledButtonStart();

	}

	void waitingForStart2::exit(ErrorContext& ctx){
	    ctx.hal.getActors()->ledButtonStartOff();
	    if(ctx.data.weiche == LET_ITEM_PASS_STATE){
	    	ctx.data.removeItem(2);
	    }
	    else if (ctx.data.weiche == WAIT_STATE){
	    	ctx.data.removeItem(3);
	    }

	}

	void waitingForStart2::eventButtonStartPressed(ErrorContext& ctx){
		exit(ctx);
		if (ctx.onlyPusher == true){
			ctx.active = false;
		}
		else{
			new (this) currentNAck;
			entry(ctx);
		}
	}
