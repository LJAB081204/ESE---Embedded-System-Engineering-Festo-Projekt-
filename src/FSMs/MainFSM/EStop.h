/*
 * EStop.h
 *
 *  Created on: 31.05.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_MAINFSM_ESTOP_H_
#define SRC_FSMS_MAINFSM_ESTOP_H_

#include "MainBase.h"
#include "../EstopFSM/EstopContext.h"

class EStop : public MainBase {
	EStopContext eStopFSM;
public:
		EStop(HAL& hal,HeightSensor& hs, EStopSource source) : eStopFSM(hal, hs, source) {}
		void entry(MainContext& ctx) override;
	    void exit(MainContext& ctx) override;
	    void eventButtonEstopPressed(MainContext& ctx) override;
	    void eventButtonEstopReleased(MainContext& ctx) override;
	    void eventButtonEstopPressedRemote(MainContext& ctx) override;
	    void eventButtonEstopReleasedRemote(MainContext& ctx) override;
	    void eventButtonResetPressed(MainContext& ctx) override;
	    void eventButtonResetPressedRemote(MainContext& ctx) override;
};

#endif
