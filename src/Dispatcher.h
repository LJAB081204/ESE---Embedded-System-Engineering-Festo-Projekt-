/*
 * Dispatcher.h
 *
 *  Created on: 6.5.2026
 *      Author: Lewis
 */
#ifndef SRC_DISPATCHER_H_
#define SRC_DISPATCHER_H_
#include "hal.h"
#include "PulseMessages.h"


struct DispatcherMsg {
    struct _pulse pulse;
    int type;
    int data;
};

class Dispatcher{
private:
	int channelID;
	int connectionID;
	bool waitForMessages;
	std::thread receivingThread;
	DispatcherMsg msg;
};
