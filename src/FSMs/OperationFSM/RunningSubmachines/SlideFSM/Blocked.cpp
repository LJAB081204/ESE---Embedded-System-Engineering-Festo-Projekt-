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

#include <iostream>

namespace M1{

	Blocked::Blocked(){
		}

		void Blocked::entry(SlideContext& ctx){
			ctx.slideTimer->start(2000);
			std::cout << "Slide Blocked"<<std::endl;
		}
		void Blocked::exit(SlideContext& ctx){
			ctx.slideTimer->stop();
		}

		void Blocked::eventSlideTimerExpired(SlideContext& ctx){
			exit(ctx);
			new (this) Full;
			entry(ctx);
		}

		void Blocked::eventItemLeftSlide(SlideContext& ctx){
			exit(ctx);
			new (this) Free;
			entry(ctx);

		}
}
