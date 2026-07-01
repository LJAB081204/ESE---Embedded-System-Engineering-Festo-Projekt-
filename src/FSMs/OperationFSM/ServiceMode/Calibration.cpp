/*
 * Calibration.cpp
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */

#include <new>
#include "../OperationBase.h"
#include "../OperationContext.h"
#include "../Service.h"
#include "../Idle.h"
#include "Calibration.h"
#include "EntryCalibration.h"
#include <iostream>

	Calibration::Calibration(){
	}

	void Calibration::entry(OperationContext& ctx){
		ctx.hal.getActors()->greenLampBlink(500);
		ctx.hal.getActors()->lampQ1();
		ctx.hal.getActors()->ledButtonStart();
		ctx.hal.getActors()->ledButtonResetOff();
		ctx.hal.getActors()->closePusher();
		if(ctx.round == 0) {
			std::cout << "Kalibrierung gestartet." << std::endl;
			std::cout << "Führe Werkstück mit Bohrung ein." << std::endl;
		} else {
			std::cout << "Führe flaches Werkstück ein." << std::endl;
		}
	}

	void Calibration::exit(OperationContext& ctx){
		ctx.hal.getActors()->lampQ1Off();
	}

	void Calibration::eventItemAtEntry(OperationContext& ctx){
		exit(ctx);
		new (this) EntryCalibration;
		entry(ctx);
	}

	void Calibration::eventButtonStartPressedLong(OperationContext& ctx){
		if(ctx.data.calibrated) {
			std::cout << "Kalibrierung abgeschlossen." << std::endl;
		} else {
			std::cout << "Kalibrierung abgebrochen." << std::endl;
		}
		exit(ctx);
		new (this) Idle;
		entry(ctx);
	}
	void Calibration::eventButtonStartPressedLongRemote(OperationContext& ctx){
		if(ctx.data.calibrated) {
			std::cout << "Kalibrierung abgeschlossen." << std::endl;
		} else {
			std::cout << "Kalibrierung abgebrochen." << std::endl;
		}
		exit(ctx);
		new (this) Idle;
		entry(ctx);
	}

	void Calibration::eventButtonStopPressed(OperationContext& ctx){
//		exit(ctx);
//		new (this) Service;
//		entry(ctx);
	}

	void Calibration::eventButtonStopPressedRemote(OperationContext& ctx){
//		exit(ctx);
//		new (this) Service;
//		entry(ctx);
	}



