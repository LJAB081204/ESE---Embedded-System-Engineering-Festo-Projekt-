/*
 * Receive.cpp
 *
 *  Created on: 01.06.2026
 *      Author: tayla
 */
#include <new>
#include <iostream>
#include "Receive.h"
#include "Ready.h"
#include "WeicheBase.h"
#include "WeicheContext.h"


Receive::Receive(){

}

void Receive::entry(WeicheContext& ctx){
	ctx.hal.getActors()->closePusher();

}
void Receive::exit(WeicheContext& ctx){

}

void Receive::eventItemAtPusher(WeicheContext& ctx){
	//exit(ctx);
	//new (this) Ready;
	//entry(ctx);
}
void Receive::eventItemMetal(WeicheContext& ctx){
	ctx.hal.getActors()->openPusher();
	sleep(1);
	ctx.hal.getActors()->closePusher();
	new (this) Receive;
	//bool metal == true
}
void Receive::eventUpdate(WeicheContext& ctx){

}



