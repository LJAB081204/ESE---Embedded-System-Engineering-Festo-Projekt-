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
namespace M2{

LetItemPass::LetItemPass(){
}

void LetItemPass::entry(WeicheContext& ctx){
	ctx.hal.getActors()->letItemPass();

}

void LetItemPass::exit(WeicheContext& ctx){
}
void LetItemPass::eventItemLeftPusher(WeicheContext& ctx){
	exit(ctx);
	if(!ctx.data.section2.empty()){
		ctx.data.moveFrom2To3();
	}
	if(ctx.hal.getActors()->isJunction()){
		new (this) Wait;
		entry(ctx);
	}
	else{
		new (this) Receive;
	}
}
}

