/*
 * Blocked.cpp
 *
 *  Created on: 08.06.2026
 *      Author: lewis
 */

#include "SlideBase.h"
#include "SlideContext.h"
#include "Free.h"
#include "Blocked.h"
#include "Free.h"
#include "Full.h"
#include <sys/neutrino.h>
#include "../../../../PulseMessages.h"


	Blocked::Blocked(){
		}

		void Blocked::entry(SlideContext& ctx){

		}
		void Blocked::exit(SlideContext& ctx){

		}

		void Blocked::eventSlideTimerExpired(SlideContext& ctx){
			exit(ctx);
			new (this) Full;
			entry(ctx);
		}

		void Blocked::eventItemLeftSlide(SlideContext& ctx){
			exit(ctx);
			raiseStopSlideTimer(ctx);
			new (this) Free;
		}

		void Blocked::raiseStopSlideTimer(SlideContext& ctx){
			MsgSendPulse(ctx.data.conId, SIGEV_PULSE_PRIO_INHERIT, PULSE_EVENT_STOP_SLIDE_TIMER, 0);
		}

