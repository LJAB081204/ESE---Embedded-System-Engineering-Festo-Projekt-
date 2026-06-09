/*
 * HandShakeEStopLocalNotPressed.h
 *
 *  Created on: 09.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPLOCALNOTPRESSED_H_
#define SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPLOCALNOTPRESSED_H_

#include "EstopBase.h"

class HandShakeEStopLocalNotPressed : public EStopBase{
public:
	HandShakeEStopLocalNotPressed();
	void entry(EStopContext& ctx)override;
	void exit(EStopContext& ctx)override;
	void handshakeEStopRemoteIsPressed(EStopContext& ctx)override;
	void handshakeEStopRemoteIsNotPressed(EStopContext& ctx)override;

};



#endif /* SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPLOCALNOTPRESSED_H_ */
