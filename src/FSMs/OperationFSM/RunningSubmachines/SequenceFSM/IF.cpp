/*
 * IF.cpp
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#include "SequenceBase.h"
#include "SequenceContext.h"
#include "IF.h"
#include "IHM.h"

	IF::IF(){
	}

	void IF::entry(SequenceContext& ctx){

	}

	void IF::exit(SequenceContext& ctx){

	}

	void IF::eventUpdate(SequenceContext& ctx){
		//if IF == true -> IHM && raise eventItemCorrect
		//else raise eventItemWrong
	}


