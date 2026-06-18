/*
 * ConnectionLost.h
 *
 *  Created on: 17.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_MAINFSM_CONNECTIONLOST_H_
#define SRC_FSMS_MAINFSM_CONNECTIONLOST_H_

#include "MainBase.h"
#include "../ConnectionLostFSM/ConnectionLostContext.h"

class ConnectionLost : public MainBase {
	ConnectionLostContext conLostFSM;
public:
	ConnectionLost(HAL& hal, HeightSensor& hs, PreviousFsmState prevState) : conLostFSM(hal, hs, prevState) {}
	void entry(MainContext& ctx)override;
	void exit(MainContext& ctx)override;
	void eventButtonEstopPressed(MainContext& ctx)override;
	void eventButtonEstopReleased(MainContext& ctx)override;
	void eventConnectionLost(MainContext& ctx)override;
	void eventConnectionRestored(MainContext& ctx)override;
};



#endif /* SRC_FSMS_MAINFSM_CONNECTIONLOST_H_ */
