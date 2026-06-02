/*
 * PutItemInSlide.cpp
 *
 *  Created on: 02.06.2026
 *      Author: tayla
 */

#include <new>
#include "WeicheBase.h"
#include "WeicheContext.h"
#include "PutItemInSlide.h"
#include "Receive.h"

PutItemInSlide::PutItemInSlide(){
}
void PutItemInSlide::entry(WeicheContext& ctx){
	ctx.hal.getActors()->closePusher();
}
void PutItemInSlide::eventItemInSlide(WeicheContext& ctx){
	exit(ctx);
	new (this) Receive;
	entry(ctx);
}

