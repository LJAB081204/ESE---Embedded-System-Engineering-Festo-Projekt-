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

void Running::eventButtonStopPressed(OperationContext& ctx){
	new (this) Idle;
}

