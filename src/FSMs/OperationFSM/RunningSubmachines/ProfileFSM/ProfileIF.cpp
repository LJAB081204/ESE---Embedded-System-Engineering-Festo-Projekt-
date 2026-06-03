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

ProfileIF::ProfileIF(){};

void ProfileIF::entry(ProfileContext& ctx) {
	std::cout << "ProfileIF detected" << std::endl;
}

void ProfileIF::exit(ProfileContext& ctx) {}

void ProfileIF::eventHeightHole(ProfileContext& ctx) {
	exit(ctx);
	new(this) IU;
	entry(ctx);
}

void ProfileIF::eventItemGone(ProfileContext& ctx) {
	exit(ctx);
	std::cout << "ProfileIF" << std::endl;
	//in die Datenklasse schreiben
	new(this) Default;
	entry(ctx);
}

void ProfileIF::eventHeightWrong(ProfileContext& ctx) {
	exit(ctx);
	new(this) IS;
	entry(ctx);
}


