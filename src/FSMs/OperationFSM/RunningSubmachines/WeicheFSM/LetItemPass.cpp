/*
 * LetItemPass.cpp
 *
 *  Created on: 02.06.2026
 *      Author: tayla
 */

#include <new>
#include "WeicheBase.h"
#include "WeicheContext.h"
#include "Receive.h"
#include "Wait.h"
#include "LetItemPass.h"
namespace M1{

LetItemPass::LetItemPass(){
}

void LetItemPass::entry(WeicheContext& ctx){
	ctx.hal.getActors()->letItemPass();
	// start Timer Pusher
}

void LetItemPass::exit(WeicheContext& ctx){
	// Start Timer 3s
}
void LetItemPass::eventItemLeftPusher(WeicheContext& ctx){
	exit(ctx);
	new (this) Wait;
	entry(ctx);
}
}

