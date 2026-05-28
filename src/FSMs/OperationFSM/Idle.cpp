/*
 * Idle.cpp
 *
 *  Created on: 27.05.2026
 *      Author: tayla
 */
#include <new>
#include "OperationBase.h"
#include "Idle.h"
#include "Running.h"


	Idle::Idle(){
	}

	void Idle::entry(OperationContext& ctx){
	}
	void Idle::exit(OperationContext& ctx){
	}

	void Idle::eventButtonStartPressed(OperationContext& ctx){
		exit(ctx);
		new (this) Running;
		entry(ctx);
	}
	void Idle::eventButtonStartPressedLong(OperationContext& ctx){
//		exit(ctx);
//		new (this) Service;
//		entry(ctx);
	}

	void Idle::eventButtonEstopPressed(OperationContext& ctx){
//		exit(ctx);
//		new (this) Estop;
//		entry(ctx);
	}




