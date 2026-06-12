/*
 * ProfileIF.cpp
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */

#include <iostream>

#include "ProfileContext.h"
#include "ProfileIF.h"
#include "IU.h"
#include "Default.h"
#include "IS.h"
namespace M1{

ProfileIF::ProfileIF(){};

void ProfileIF::entry(ProfileContext& ctx) {
	std::cout << "//////////////////////ProfileIF detected" << std::endl;
	ctx.data.section1.front().flat = true;
}

void ProfileIF::exit(ProfileContext& ctx) {}

void ProfileIF::eventHeightHigh(ProfileContext& ctx) {
	exit(ctx);
	new(this) IU;
	entry(ctx);
}

void ProfileIF::eventHeightHole(ProfileContext& ctx) {
	exit(ctx);
	new(this) IU;
	entry(ctx);
}

void ProfileIF::eventItemGone(ProfileContext& ctx) {
	exit(ctx);
	std::cout << "ProfileIF" << std::endl;
	if(!ctx.data.section1.empty()){
		ctx.data.moveFrom1To2();
	}
	new(this) Default;
	entry(ctx);
}

void ProfileIF::eventHeightWrong(ProfileContext& ctx) {
	exit(ctx);
	new(this) IS;
	entry(ctx);
}
}

