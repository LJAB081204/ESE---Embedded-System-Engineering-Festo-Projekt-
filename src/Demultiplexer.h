/*
 * Demultiplexer.h
 *
 *  Created on: 30.04.2026
 *      Author: tayla
 */

#ifndef SRC_DEMULTIPLEXER_H_
#define SRC_DEMULTIPLEXER_H_

#include <thread>
#include <stdint.h>


class Demultiplexer{
private:
	int channelID;
	int connectionID;
	int dispatcherID;
	bool waitForMessages;
	std::thread receivingThread;
	uintptr_t gpio_bank_0;

public:
	Demultiplexer(int DispatcherConnectionID);
	~Demultiplexer();

	void run(int channelID);

	int getChannelID();
	int getConnectionID();

};


#endif /* SRC_DEMULTIPLEXER_H_ */
