/*
 * IU.cpp
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */

#include <iostream>

#include "ProfileContext.h"
#include "IU.h"
#include "Default.h"
#include "IS.h"

IU::IU(){};

void IU::entry(ProfileContext& ctx) {
	std::cout << "IU detected" << std::endl;
}

void IU::exit(ProfileContext& ctx) {}

void IU::eventItemGone(ProfileContext& ctx) {
	exit(ctx);
	//in die Datenklasse schreiben
	new(this) Default;
	entry(ctx);
}

void IU::eventHeightWrong(ProfileContext& ctx) {
	exit(ctx);
	new(this) IS;
	entry(ctx);
}



