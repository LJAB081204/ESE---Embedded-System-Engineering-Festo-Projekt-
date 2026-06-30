/*
 * I.cpp
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */
#include <iostream>

#include "ProfileContext.h"
#include "I.h"
#include "Default.h"
#include "IS.h"
#include "IHTemp.h"
#include "IU.h"
namespace M2{

I :: I(){}

void I::entry(ProfileContext& ctx) {
	std::cout << "///////////////I detected" << std::endl;
	if(ctx.data.heightDefault == true){
		ctx.data.heightDefault = false;
		std::cout << ctx.data.heightDefault<< std::endl;
		exit(ctx);
		new(this) Default;
		entry(ctx);
		return;
	}
	if(ctx.data.itemGone == true){
		ctx.data.itemGone = false;
		eventItemGone(ctx);
	}
}

void I::exit(ProfileContext& ctx) {}

void I::eventHeightHole(ProfileContext& ctx) {
	exit(ctx);
	new(this) IHTemp;
	entry(ctx);
}

void I::eventHeightFlat(ProfileContext& ctx){
	exit(ctx);
	new(this) IU;
	entry(ctx);
}

void I::eventItemGone(ProfileContext& ctx) {
	exit(ctx);

	if(!ctx.data.section1.empty()){
		ctx.data.section1.front().high = true;
		ctx.data.moveFrom1To2();
	}

	new(this) Default;
	entry(ctx);
}

void I::eventHeightWrong(ProfileContext& ctx) {
	exit(ctx);
	new(this) IS;
	entry(ctx);
}
}

