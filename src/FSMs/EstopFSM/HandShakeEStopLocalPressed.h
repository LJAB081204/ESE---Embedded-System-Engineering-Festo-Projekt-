/*
 * HandShakeEStopLocalPressed.h
 *
 *  Created on: 09.06.2026
 *      Author: tayla
 */

#ifndef SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPLOCALPRESSED_H_
#define SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPLOCALPRESSED_H_

#include "EstopBase.h"

class HandShakeEStopLocalPressed : public EStopBase{
public:
	HandShakeEStopLocalPressed();
	void entry(EStopContext& ctx)override;
	void exit(EStopContext& ctx)override;
	void handshakeEStopRemoteIsPressed(EStopContext& ctx)override;
	void handshakeEStopRemoteIsNotPressed(EStopContext& ctx)override;

};



#endif /* SRC_FSMS_ESTOPFSM_HANDSHAKEESTOPLOCALPRESSED_H_ */
