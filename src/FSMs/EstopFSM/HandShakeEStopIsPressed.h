/*
 * HandShakeEStopIsPressed..h
 *
 *  Created on: 09.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPISPRESSED_H_
#define SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPISPRESSED_H_

#include "EstopBase.h"

class HandShakeEStopIsPressed : public EStopBase{
public:
	HandShakeEStopIsPressed();
	void entry(EStopContext& ctx)override;
	void exit(EStopContext& ctx)override;
	void eventResponseEStopIsPressed(EStopContext& ctx)override;
	void eventResponseEStopWasPressed(EStopContext& ctx)override;
	void eventResponseEStopWasNotPressed(EStopContext& ctx)override;
};



#endif /* SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPISPRESSED_H_ */
