/*
 * ConnectionLostBase.h
 *
 *  Created on: 17.06.2026
 *      Author: mika2
 */

#ifndef SRC_FSMS_CONNECTIONLOSTFSM_CONNECTIONLOSTBASE_H_
#define SRC_FSMS_CONNECTIONLOSTFSM_CONNECTIONLOSTBASE_H_

class ConnectionLostContext;

class ConnectionLostBase {
public:
	virtual ~ConnectionLostBase() = default;
	virtual void entry(ConnectionLostContext& ctx) {}
	virtual void exit(ConnectionLostContext& ctx)  {}
	virtual void eventButtonEstopPressed(ConnectionLostContext& ctx)       {}
	virtual void eventButtonEstopReleased(ConnectionLostContext& ctx)      {}
	virtual void eventConnectionLost(ConnectionLostContext& ctx)  {}
	virtual void eventConnectionRestored(ConnectionLostContext& ctx)  {}
};



#endif /* SRC_FSMS_CONNECTIONLOSTFSM_CONNECTIONLOSTBASE_H_ */
