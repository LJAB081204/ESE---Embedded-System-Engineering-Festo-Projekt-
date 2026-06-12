/*
 * HeighCalibration.cpp
 *
 *  Created on: 11.06.2026
 *      Author: olive
 */

#include <new>
#include "../OperationBase.h"
#include "../OperationContext.h"
#include "HeightCalibration.h"
#include "EntryCalibration.h"
#include "../Idle.h"
#include "../Service.h"
#include <iostream>

	HeightCalibration::HeightCalibration(){
	}

	void HeightCalibration::entry(OperationContext& ctx){
		ctx.hal.getActors()->greenLampBlink(500);
		ctx.hal.getActors()->ledButtonStart();
		ctx.hal.getActors()->ledButtonResetOff();
		ctx.hs.setStartCalibration();
		std::cout << "HeightCalibration" << std::endl;
	}

	void HeightCalibration::exit(OperationContext& ctx){
		ctx.hs.stopCalibration();
		std::cout << "exit calibration" << std::endl;
	}

	void HeightCalibration::eventButtonStartPressedLong(OperationContext& ctx){
		exit(ctx);
		new (this) Idle;
		entry(ctx);
	}

	void HeightCalibration::eventButtonStartPressed(OperationContext& ctx){
		exit(ctx);
		new (this) Service;
		entry(ctx);
	}

	void HeightCalibration::eventButtonStartPressedLongRemote(OperationContext& ctx){
		exit(ctx);
		new (this) Idle;
		entry(ctx);
	}

	void HeightCalibration::eventButtonStartPressedRemote(OperationContext& ctx){
		exit(ctx);
		new (this) Service;
		entry(ctx);
	}

	void HeightCalibration::eventItemAtEntry(OperationContext& ctx){
		ctx.hal.getActors()->motorForwards();
	}

	void HeightCalibration::eventItemArrived(OperationContext& ctx){
		ctx.hal.getActors()->motorSlow();
	}

	void HeightCalibration::eventItemGone(OperationContext& ctx){
		ctx.hal.getActors()->motorForwards();
	}

	void HeightCalibration::eventItemAtPusher(OperationContext& ctx){
			ctx.hal.getActors()->letItemPass();
	}

	void HeightCalibration::eventItemAtExit(OperationContext& ctx){
			ctx.hal.getActors()->motorStop();
			ctx.hal.getActors()->closePusher();
	}



