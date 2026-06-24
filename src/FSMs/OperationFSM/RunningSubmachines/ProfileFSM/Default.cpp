/*
 * Default.cpp
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */

#include <iostream>
#include "ProfileContext.h"
#include "Default.h"
#include "I.h"
#include "IS.h"
#include "ProfileIF.h"
#include "IU.h"
#include <sys/neutrino.h>
#include "../../../../PulseMessages.h"
namespace M1{
	Default::Default(){};

	void Default::entry(ProfileContext& ctx) {
		std::cout<< "//////////////////Default detected" << std::endl;
		ctx.data.heightDefault = false;
	}

	void Default::exit(ProfileContext& ctx) {}

	void Default::eventHeightHigh(ProfileContext& ctx) {
		exit(ctx);
		new(this) I;
		entry(ctx);
	}

	void Default::eventHeightFlat(ProfileContext& ctx) {
		exit(ctx);
		new(this) ProfileIF;
		entry(ctx);
	}

	void Default::eventHeightHole(ProfileContext& ctx){
		exit(ctx);
		new(this) IU;
		entry(ctx);
	}

	void Default::eventHeightWrong(ProfileContext& ctx) {
		exit(ctx);
		new(this) IS;
		entry(ctx);
	}
}
