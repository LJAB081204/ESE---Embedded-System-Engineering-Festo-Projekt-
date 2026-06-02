/*
 * IHM.cpp
 *
 *  Created on: 01.06.2026
 *      Author: lewis
 */

#include "SequenceBase.h"
#include "SequenceContext.h"
#include "IHM.h"
#include "IH.h"

	IHM::IHM(){
	}

	void IHM::entry(SequenceContext& ctx){

	}

	void IHM::exit(SequenceContext& ctx){

	}

	void IHM::eventUpdate(SequenceContext& ctx){
		//if IHM == true -> IH && raise eventItemCorrect
		//else raise eventItemWrong
	}
	void IHM::eventItemCorrect(SequenceContext& ctx){

	}
	void IHM::eventItemWrong(SequenceContext& ctx){

	}


