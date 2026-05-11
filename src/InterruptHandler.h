/*
 * InterruptHandler.h
 *
 *  Created on: 02.05.2026
 *      Author: tayla
 */

#ifndef SRC_INTERRUPTHANDLER_H_
#define SRC_INTERRUPTHANDLER_H_

#include <sys/neutrino.h>
#include <stdint.h>
#include "HAL.h"



class InterruptHandler{
private:
	uintptr_t gpio_bank_0;
	int connectionID;
	int idButtonStart;
	int idButtonStop;
	int idButtonReset;
	int idEstop;
	int idItemAtEntry;
	int idItemAtExit;
	int iditemAtPusher;
	int iditemMetal;
	int idPusherOpen;
	int idSlideFull;


	struct sigevent eventStart;
	struct sigevent eventStop;
	struct sigevent eventReset;
	struct sigevent eventEstop;
	struct sigevent eventItemAtEntry;
	struct sigevent eventItemAtExit;
	struct sigevent eventItemAtPusher;
	struct sigevent eventItemMetal;
	struct sigevent eventPusherOpen;
	struct sigevent eventSlideFull;







public:
	InterruptHandler(int connectionID);
	~InterruptHandler();
	void initInterrupts(int coid);

};


#endif /* SRC_INTERRUPTHANDLER_H_ */
