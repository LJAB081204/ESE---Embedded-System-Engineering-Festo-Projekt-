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
	if(ctx.data.heightDefault == true){
		ctx.data.heightDefault = false;
		std::cout << ctx.data.heightDefault<< std::endl;
		exit(ctx);
		new(this) Default;
		entry(ctx);
	}
	if(ctx.data.itemGone == true){
		ctx.data.itemGone = false;
		eventItemGone(ctx);
	}
}

void ProfileIH::exit(ProfileContext& ctx) {}

void ProfileIH::eventItemGone(ProfileContext& ctx) {
	exit(ctx);
	std::cout<< ctx.data.section1.size() << std::endl;
	if(!ctx.data.section1.empty()){
		std::cout << "////////////////////ProfileIH entered" << std::endl;

		ctx.data.section1.front().hole = true;
		ctx.data.section1.front().high = true;
		std::cout << "Profile set to IH" << std::endl;
		ctx.data.moveFrom1To2();
	}
	std::cout<< ctx.data.section2.size() << std::endl;

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

