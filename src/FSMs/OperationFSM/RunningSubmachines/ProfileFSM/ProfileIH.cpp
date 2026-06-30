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
	if(ctx.data.heightDefault == true){
		ctx.data.heightDefault = false;
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
	if(!ctx.data.section1.empty()){
		ctx.data.section1.front().hole = true;
		ctx.data.section1.front().high = true;
		ctx.data.moveFrom1To2();
	}
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

