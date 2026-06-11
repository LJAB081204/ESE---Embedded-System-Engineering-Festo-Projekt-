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
		std::cout << "HeightCalibration" << std::endl;
		ctx.hal.getActors()->greenLampBlink(500);
		ctx.hal.getActors()->ledButtonStart();
		ctx.hal.getActors()->ledButtonResetOff();
		ctx.hs.setStartCalibration();
	}

	void HeightCalibration::exit(OperationContext& ctx){
		ctx.hal.getActors()->greenLampBlinkOff();
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

	void HeightCalibration::eventItemAtEntry(OperationContext& ctx){
		ctx.hal.getActors()->motorForwards();
//		ctx.hs.calibrate();
	}



