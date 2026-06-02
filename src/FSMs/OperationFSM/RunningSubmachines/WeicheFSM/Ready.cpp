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
Ready::Ready(){
}

void Ready::eventItemCorrect(WeicheContext& ctx){
	exit(ctx);
	new (this) LetItemPass;
	entry(ctx);
}
void Ready::eventItemWrong(WeicheContext& ctx){
	/*
	 * if (slideFull == false){
	 * 		new (this) putItemInSlide;
	 * 		entry(ctx)
	 * }
	 * else if(slideFull == true && slideFullRemote){
	 * 	     new(this) letItemPass;
	 * 	     entry(ctx);
	 * } else if(slideFull == true && slideFullRemote == true){
	 * 		raise Error
	 */
}


