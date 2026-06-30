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
namespace M2{

PutItemInSlide::PutItemInSlide(){
}
void PutItemInSlide::entry(WeicheContext& ctx){
	if(!ctx.data.section2.empty()) {
		const auto& front = ctx.data.section2.front();
		std::cout << "Werkstück aussortiert | ID: " << front.id
		          << " | Typ: " << profileName(front.item);
		if(front.sortOutReason == SortOutReason::ITEM_SWAPPED) {
			std::cout << " | FM1 meldete: " << profileName(front.expectedProfile)
			          << " | Grund: Werkstück getauscht";
		} else if(front.sortOutReason == SortOutReason::WRONG_SEQUENCE) {
			std::cout << " | Erwartet: " << profileName(front.expectedProfile)
			          << " | Grund: falsche Reihenfolge";
		} else if(front.item == IU_PROFILE || front.item == UNDEFINED_PROFILE) {
			std::cout << " | Grund: Profil nicht erkannt";
		}
		std::cout << std::endl;
	}
	ctx.hal.getActors()->sortItemOut();
}
void PutItemInSlide::eventItemLeftPusher(WeicheContext& ctx){
	exit(ctx);
	new (this) Receive;
	entry(ctx);
}
}
