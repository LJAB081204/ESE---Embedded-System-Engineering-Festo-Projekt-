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




class ConnectionController {
private:
	name_attach_t* remoteIn;

	int dispatcherInChid;

	int remoteOutCoid;
	int dispatcherOutCoid = -1;
	int dispatcherInCoid;

	int wakeRemoteCoid;
	int wakeDispatcherCoid;

	std::atomic<bool> running{true};

	std::thread tRemote;
	std::thread tDispatcher;

	_pulse msgFromRemote;
	_pulse msgFromDispatcher;
    void runReceiveFromDisp();
    void runReceiveFromRemote();

public:
    ConnectionController(const char* localName, const char* remoteName);
    ~ConnectionController();
    int getConnectionID();
    void start(int dispConID);
};



#endif /* SRC_CONNECTIONCONTROLLER_H_ */
