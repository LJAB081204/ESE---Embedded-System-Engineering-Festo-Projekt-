/*
 * BothEStop.cpp
 *
 *  Created on: 08.06.2026
 *      Author: tayla
 */




/*
 * EStop.cpp
 *
 *  Created on: 08.06.2026
 *      Author: tayla
 */

#include "EStopState.h"
#include "EStopContext.h"
#include "EStopRemote.h"
#include "BothEStop.h"
#include "Reset.h"
#include "../MainFSM/EStop.h"

BothEStop::BothEStop(){
}

void BothEStop::entry(EStopContext& ctx){
	ctx.hal.getActors()->motorStop();
	ctx.hal.getActors()->closePusher();
	ctx.hal.getActors()->redLampOn();
	ctx.setActive(true);
}

void BothEStop::exit(EStopContext& ctx){

}
void BothEStop::eventButtonEstopReleasedRemote(EStopContext& ctx){
	exit(ctx);
	new (this) EStopState;
	entry(ctx);
}
void BothEStop::eventButtonEstopReleased(EStopContext& ctx){
	exit(ctx);
	new (this) EStopRemote;
	entry(ctx);
}




