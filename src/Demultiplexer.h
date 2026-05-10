/*
 * Demultiplexer.h
 *
 *  Created on: 30.04.2026
 *      Author: tayla
 */

#ifndef SRC_DEMULTIPLEXER_H_
#define SRC_DEMULTIPLEXER_H_
#include "hal.h"
#include "PulseMessages.h"
#include <thread>


class Demultiplexer{
private:
	int channelID;
	int connectionID;
	int dispatcherID;
	std::string channel_name;
	bool waitForMessages;
	std::thread receivingThread;
	_pulse msg;
	uintptr_t gpio_bank_0;

public:
	Demultiplexer(int DispatcherConnectionID);
	~Demultiplexer();

	void run(int channelID);

	int getChannelID();
	int getConnectionID();

};


#endif /* SRC_DEMULTIPLEXER_H_ */
