/*
 * ConnectionLostEStopWasPressed.h
 *
 *  Created on: 17.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_CONNECTIONLOSTFSM_CONNECTIONLOSTESTOPWASPRESSED_H_
#define SRC_FSMS_CONNECTIONLOSTFSM_CONNECTIONLOSTESTOPWASPRESSED_H_

#include "ConnectionLostBase.h"

class ConnectionLostEStopWasPressed : public ConnectionLostBase{
public:
	ConnectionLostEStopWasPressed();
	void entry(ConnectionLostContext& ctx)override;
	void exit(ConnectionLostContext& ctx)override;
	void eventButtonEstopPressed(ConnectionLostContext& ctx)override;
	void eventConnectionRestored(ConnectionLostContext& ctx)override;
};



#endif /* SRC_FSMS_CONNECTIONLOSTFSM_CONNECTIONLOSTESTOPWASPRESSED_H_ */
