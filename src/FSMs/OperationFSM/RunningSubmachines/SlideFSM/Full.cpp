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

	Full::Full(){

	}
	void Full::entry(SlideContext& ctx){

	}
	void Full::exit(SlideContext& ctx){

	}

	void Full::eventItemLeftSlide(SlideContext& ctx){
		exit(ctx);
		new (this) Free;
		entry(ctx);
	}

