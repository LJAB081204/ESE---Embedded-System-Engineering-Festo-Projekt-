/*
 * Service.cpp
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */

#include <new>
#include "OperationBase.h"
#include "OperationContext.h"
#include "Idle.h"
#include "Service.h"

#include "ServiceMode/HeightCalibration.h"
#include "ServiceMode/EntryCalibration.h"

	Service::Service(){
	}

	void Service::entry(OperationContext& ctx){
		ctx.hal.getActors()->greenLampBlink(1000);
		ctx.hal.getActors()->ledButtonReset();
		ctx.hal.getActors()->ledButtonStartOff();
		ctx.hal.getActors()->motorStop();
	}

	void Service::exit(OperationContext& ctx){
	}

	void Service::eventButtonStartPressedLong(OperationContext& ctx){
		exit(ctx);
		new (this) Idle;
		entry(ctx);
	}

	void Service::eventItemAtEntry(OperationContext& ctx){
		exit(ctx);
		new (this) EntryCalibration;
		entry(ctx);
	}

	void Service::eventButtonResetPressed(OperationContext& ctx){
		exit(ctx);
		new (this) HeightCalibration;
		entry(ctx);
	}



