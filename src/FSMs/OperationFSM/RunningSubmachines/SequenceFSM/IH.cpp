/*
 * IH.cpp
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#include "SequenceBase.h"
#include "SequenceContext.h"
#include "IH.h"
#include "IM.h"

	IH::IH(){
	}

	void IH::entry(SequenceContext& ctx){
		//nichts?
	}

	void IH::exit(SequenceContext& ctx){

	}

	void IH::eventUpdate(SequenceContext& ctx){
		//if IM == true -> IF && raise eventItemCorrect
		//else raise eventItemWrong
	}
	void IH::eventItemCorrect(SequenceContext& ctx){

	}
	void IH::eventItemWrong(SequenceContext& ctx){

	}

