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

#include <iostream>

IHTemp::IHTemp(){};

void IHTemp::entry(ProfileContext& ctx) {
	std::cout << "IHTemp detected" << std::endl;
}

void IHTemp::exit(ProfileContext& ctx) {}

void IHTemp::eventHeightHigh(ProfileContext& ctx){
	exit(ctx);
	new(this) ProfileIH;
	entry(ctx);
}

void IHTemp::eventHeightWrong(ProfileContext& ctx) {
	exit(ctx);
	new(this) IS;
	entry(ctx);
}



