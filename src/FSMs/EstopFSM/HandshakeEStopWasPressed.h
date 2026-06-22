/*
 * HandshakeEStopWasPressed.h
 *
 *  Created on: 17.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPWASPRESSED_H_
#define SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPWASPRESSED_H_

#include "EstopBase.h"

class HandShakeEStopWasPressed : public EStopBase{
public:
	HandShakeEStopWasPressed();
	void entry(EStopContext& ctx)override;
	void exit(EStopContext& ctx)override;
	void eventResponseEStopIsPressed(EStopContext& ctx)override;
	void eventResponseEStopIsNotPressed(EStopContext& ctx)override;

};



#endif /* SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPWASPRESSED_H_ */
