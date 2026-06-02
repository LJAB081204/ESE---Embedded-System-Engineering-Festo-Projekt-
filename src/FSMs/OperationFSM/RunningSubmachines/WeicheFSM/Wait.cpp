/*
 * Wait.cpp
 *
 *  Created on: 02.06.2026
 *      Author: tayla
 */

#include <new>
#include "WeicheBase.h"
#include "WeicheContext.h"
#include "Ready.h"
#include "Wait.h"

Wait::Wait(){

}
void Wait::eventTimerExpired(WeicheContext& ctx){
	exit(ctx);
	new (this) Ready;
	entry(ctx);
}


