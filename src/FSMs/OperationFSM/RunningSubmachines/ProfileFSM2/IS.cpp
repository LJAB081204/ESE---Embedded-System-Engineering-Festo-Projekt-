/*
 * IS.cpp
 *
 *  Created on: 01.06.2026
 *      Author: olive
 */

#include <iostream>

#include "ProfileContext.h"
#include "IS.h"
namespace M2{

IS::IS(){};

void IS::entry(ProfileContext& ctx) {
	std::cout << "//////////////////IS detected" << std::endl ;
	//Datenklasse
	//ErrorCode setzten
}

void IS::exit(ProfileContext& ctx) {}


void IS::eventItemGone(ProfileContext& ctx){
	exit(ctx);
	new(this) Default;
	entry(ctx);
}
}
