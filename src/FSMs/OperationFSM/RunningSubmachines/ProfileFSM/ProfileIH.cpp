/*
 * ProfileIH.cpp
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */

#include <iostream>

#include "ProfileContext.h"
#include "ProfileIH.h"
#include "Default.h"
#include "IS.h"
#include "IU.h"
namespace M1{

ProfileIH::ProfileIH(){};

void ProfileIH::entry(ProfileContext& ctx) {
	std::cout << "////////////////////ProfileIH detected" << std::endl;
}

void ProfileIH::exit(ProfileContext& ctx) {}

void ProfileIH::eventItemGone(ProfileContext& ctx) {
	exit(ctx);
	//in die Datenklasse schreiben
	new(this) Default;
	entry(ctx);
}
void ProfileIH::eventHeightFlat(ProfileContext& ctx) {
	exit(ctx);
	new(this) IU;
	entry(ctx);
}

void ProfileIH::eventHeightHigh(ProfileContext& ctx) {
	exit(ctx);
	new(this) IU;
	entry(ctx);
}

void ProfileIH::eventHeightWrong(ProfileContext& ctx) {
	exit(ctx);
	new(this) IS;
	entry(ctx);
}
}

