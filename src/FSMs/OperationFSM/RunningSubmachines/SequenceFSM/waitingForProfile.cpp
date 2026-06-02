/*
 * waitingForProfile.cpp
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#include "SequenceBase.h"
#include "SequenceContext.h"
#include "waitingForProfile.h"
#include "IH.h"

	waitingForProfile::waitingForProfile(){
	}

	void waitingForProfile::entry(SequenceContext& ctx){
		//History angucken
	}

	void waitingForProfile::exit(SequenceContext& ctx){
		//nichts?
	}

	void waitingForProfile::eventUpdate(SequenceContext& ctx){
		//if IH == true -> IM && raise eventItemCorrect
		//else raise eventItemWrong
	}

	void waitingForProfile::eventItemCorrect(SequenceContext& ctx){

	}
	void waitingForProfile::eventItemWrong(SequenceContext& ctx){

	}

