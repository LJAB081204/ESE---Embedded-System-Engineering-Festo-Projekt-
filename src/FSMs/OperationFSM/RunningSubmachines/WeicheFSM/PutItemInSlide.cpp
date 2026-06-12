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
#include <iostream>
namespace M1{

PutItemInSlide::PutItemInSlide(){
}
void PutItemInSlide::entry(WeicheContext& ctx){
	ctx.hal.getActors()->sortItemOut();
}
void PutItemInSlide::eventItemInSlide(WeicheContext& ctx){
	exit(ctx);
	std::cout << "ITEM SORTED OUT" << std::endl;
	if(!ctx.data.section2.empty()){
		ctx.data.removeItem(2);
	}
	new (this) Receive;
	entry(ctx);
}
}
