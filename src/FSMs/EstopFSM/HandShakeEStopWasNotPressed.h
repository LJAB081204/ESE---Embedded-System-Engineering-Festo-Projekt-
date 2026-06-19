/*
 * HandShakeEStopWasNotPressed.h
 *
 *  Created on: 09.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPWASNOTPRESSED_H_
#define SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPWASNOTPRESSED_H_

#include "EstopBase.h"

class HandShakeEStopWasNotPressed : public EStopBase{
public:
	HandShakeEStopWasNotPressed();
	void entry(EStopContext& ctx)override;
	void exit(EStopContext& ctx)override;
	void eventResponseEStopIsPressed(EStopContext& ctx)override;
	void eventResponseEStopWasNotPressed(EStopContext& ctx)override;
	void eventResponseEStopWasPressed(EStopContext& ctx)override;

};


#endif /* SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPWASNOTPRESSED_H_ */
