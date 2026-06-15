/*
 * IHTemp.cpp
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */

#include "ProfileContext.h"
#include "IHTemp.h"
#include "IS.h"
#include "ProfileIH.h"
#include "IU.h"

#include <iostream>
namespace M2{

IHTemp::IHTemp(){};

void IHTemp::entry(ProfileContext& ctx) {
	std::cout << "///////////////IHTemp detected" << std::endl;
}

void IHTemp::exit(ProfileContext& ctx) {}

void IHTemp::eventItemGone(ProfileContext& ctx){
	exit(ctx);

	if(!ctx.data.section1.empty()){
		ctx.data.section1.front().unknown = true;
		ctx.data.moveFrom1To2();
	}

	//Datenklasse IU schreiben
	new(this) Default;
	entry(ctx);
}

void IHTemp::eventHeightHigh(ProfileContext& ctx){
	exit(ctx);
	new(this) ProfileIH;
	entry(ctx);
}

void IHTemp::eventHeightHole(ProfileContext& ctx){
	exit(ctx);
	new(this) IU;
	entry(ctx);
}

void IHTemp::eventHeightWrong(ProfileContext& ctx) {
	exit(ctx);
	new(this) IS;
	entry(ctx);
}
}


