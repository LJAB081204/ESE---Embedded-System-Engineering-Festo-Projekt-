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
namespace M1{

IU::IU(){};

void IU::entry(ProfileContext& ctx) {
	std::cout << "///////////////////IU detected" << std::endl;
}

void IU::exit(ProfileContext& ctx) {}

void IU::eventItemGone(ProfileContext& ctx) {
	exit(ctx);
	if(!ctx.data.section1.empty()){
		ctx.data.section1.front().unknown = true;
		ctx.data.moveFrom1To2();
	}
	new(this) Default;
	entry(ctx);
}

void IU::eventHeightWrong(ProfileContext& ctx) {
	exit(ctx);
	new(this) IS;
	//error raisen
	entry(ctx);
}

}

