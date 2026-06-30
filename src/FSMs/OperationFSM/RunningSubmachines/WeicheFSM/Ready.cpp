/*
 * Ready.cpp
 *
 *  Created on: 01.06.2026
 *      Author: tayla
 */
#include <new>
#include "WeicheBase.h"
#include "WeicheContext.h"
#include "Ready.h"
#include "LetItemPass.h"
#include "PutItemInSlide.h"
#include <sys/neutrino.h>
#include "../../../../PulseMessages.h"
#include <iostream>
namespace M1{

Ready::Ready(){
}

void Ready::eventItemCorrect(WeicheContext& ctx){
	exit(ctx);
	new (this) LetItemPass;
	entry(ctx);
}
void Ready::eventItemWrong(WeicheContext& ctx){

	if (ctx.data.slideFull == false){
		new (this) PutItemInSlide;
	  	entry(ctx);
	}
	else if(ctx.data.slideFull == true){
		exit(ctx);
		new(this) LetItemPass;
	  	entry(ctx);
	}
}

void Ready::eventSkipUpdate(WeicheContext& ctx){
	exit(ctx);
	new(this)Receive;
	entry(ctx);
}

}

