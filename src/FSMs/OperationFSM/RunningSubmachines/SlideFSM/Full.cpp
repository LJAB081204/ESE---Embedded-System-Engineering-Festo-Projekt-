/*
 * Full.cpp
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

	Full::Full(){

	}
	void Full::entry(SlideContext& ctx){
		if(ctx.hal.getSensors()->isSlideFull()){
			ctx.data.slideFull = true;
			ctx.hal.getActors()->yellowLampBlink(1000);
			std::cout << "Slide Full"<<std::endl;
		}
		else{
			exit(ctx);
			new(this) Free;
			entry(ctx);
		}
	}
	void Full::exit(SlideContext& ctx){
//		ctx.hal.getActors()->yellowLampBlinkOff();
	}

	void Full::eventItemLeftSlide(SlideContext& ctx){
		exit(ctx);
		new (this) Free;
		entry(ctx);
	}

}
