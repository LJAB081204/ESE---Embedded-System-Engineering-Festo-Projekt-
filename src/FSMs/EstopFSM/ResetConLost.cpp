/*
 * ResetConLost.cpp
 *
 *  Created on: 22.06.2026
 *      Author: mika2
 */

#include "ResetConLost.h"
#include "EstopContext.h"

ResetConLost::ResetConLost(){
}

void ResetConLost::entry(EStopContext& ctx){
	ctx.hal.getActors()->ledButtonReset();
}

void ResetConLost::exit(EStopContext& ctx){
	ctx.hal.getActors()->ledButtonResetOff();
}
void ResetConLost::eventButtonResetPressedRemote(EStopContext& ctx){
	exit(ctx);
	ctx.setActive(false); //KI
}
void ResetConLost::eventButtonResetPressed(EStopContext& ctx){
	exit(ctx);
	ctx.setActive(false); //KI
}




