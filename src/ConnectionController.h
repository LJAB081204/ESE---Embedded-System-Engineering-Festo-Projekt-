/*
 * ConnectionController.h
 *
 *  Created on: 09.06.2026
 *      Author: mika2
 */

#ifndef SRC_CONNECTIONCONTROLLER_H_
#define SRC_CONNECTIONCONTROLLER_H_


#include <thread>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include "Timer.h"
#include <chrono>




class ConnectionController {
private:
	const char* remoteName;
	const char* remoteHbName;

	name_attach_t* remoteIn;

	int dispatcherInChid;

	int remoteOutCoid;
	int dispatcherOutCoid = -1;
	int dispatcherInCoid;

	int wakeRemoteCoid;
	int wakeDispatcherCoid;

	// Heartbeat-Channel (neu)
	name_attach_t* heartbeatIn;
	int            heartbeatOutCoid;
	int            wakeHeartbeatCoid;
	std::thread    tHeartbeatReceive;

	std::atomic<bool> running{true};
	std::atomic<bool> connected{false};

	std::thread tRemote;
	std::thread tDispatcher;
	std::thread tHeartbeat;

	int heartbeatTimeoutMs = 600;
	int heartbeatIntervalMs = 400;

	_pulse msgFromRemote;
	_pulse msgFromDispatcher;
    void runReceiveFromDisp();
    void runReceiveFromRemote();
    void runHeartbeat();
    void runReceiveFromHeartbeat();

public:
    ConnectionController(const char* localName, const char* remoteName);
    ~ConnectionController();
    int getConnectionID();
    void start(int dispConID);
};



#endif /* SRC_CONNECTIONCONTROLLER_H_ */
