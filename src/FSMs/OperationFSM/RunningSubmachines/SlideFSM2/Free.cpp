/*
 * Free.cpp
 *
 *  Created on: 08.06.2026
 *      Author: lewis
 */

#include "SlideBase.h"
#include "SlideContext.h"
#include "Free.h"
#include "Blocked.h"
#include <sys/neutrino.h>
#include "../../../../PulseMessages.h"
namespace M2{

	Free::Free(){
	}

	void Free::entry(SlideContext& ctx){

	}
	void Free::exit(SlideContext& ctx){

	}

	void Free::eventItemAtSlide(SlideContext& ctx){
		exit(ctx);
		new (this) Blocked;
		entry(ctx);
	}
}
