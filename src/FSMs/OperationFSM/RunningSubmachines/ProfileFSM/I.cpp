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
namespace M1{

I :: I(){}

void I::entry(ProfileContext& ctx) {
	std::cout << "///////////////I detected" << std::endl;
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
	new(this) Default;
	entry(ctx);
}

void I::eventHeightWrong(ProfileContext& ctx) {
	exit(ctx);
	new(this) IS;
	entry(ctx);
}
}

