/*
 * ConnectionLostEStopIsPressed.h
 *
 *  Created on: 17.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_CONNECTIONLOSTFSM_CONNECTIONLOSTESTOPISPRESSED_H_
#define SRC_FSMS_CONNECTIONLOSTFSM_CONNECTIONLOSTESTOPISPRESSED_H_

#include "ConnectionLostBase.h"

class ConnectionLostEStopIsPressed : public ConnectionLostBase {
public:
	ConnectionLostEStopIsPressed();
	void entry(ConnectionLostContext& ctx)override;
	void exit(ConnectionLostContext& ctx)override;
	void eventButtonEstopReleased(ConnectionLostContext& ctx)override;
	void eventConnectionRestored(ConnectionLostContext& ctx)override;
};



#endif /* SRC_FSMS_CONNECTIONLOSTFSM_CONNECTIONLOSTESTOPISPRESSED_H_ */
