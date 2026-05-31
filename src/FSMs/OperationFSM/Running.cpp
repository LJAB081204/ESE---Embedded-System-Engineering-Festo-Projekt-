/*
 * Running.cpp
 *
 *  Created on: 28.05.2026
 *      Author: tayla
 */
#include <new>
#include "OperationBase.h"
#include "Idle.h"
#include "Running.h"


	Running::Running(){

	}


	void Running::entry(OperationContext& ctx){
		//greenLampOn
	}

	void Running::exit(OperationContext& ctx){

	}

	void Running::eventButtonStopPressed(OperationContext& ctx){
		exit(ctx);
		new (this) Idle;
		entry(ctx);
	}

